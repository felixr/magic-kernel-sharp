#include "double_image_kernel.h"

#define MINIMUM_PARALLEL_ 100000

typedef struct {
  byte channel;
  ulong u;
  double **temp;
  DoubleImage *input;
  DoubleImage *output;
} Info_;

typedef struct {
  ulong start;
  ulong end;
  double (*read)(ulong in, void *x);
  void (*write)(ulong out, double v, void *x);
  Kernel *kernel;
  Info_ info;
} ParallelInfo_;

static void *apply_(void *x);
static double read_x_(ulong in, void *x);
static void write_x_(ulong out, double v, void *x);
static double read_y_(ulong in, void *x);
static void write_y_(ulong out, double v, void *x);

DoubleImage *double_image_new_apply_xy_kernels(
    DoubleImage **pthis,
    DoubleImage *other,
    Kernel *x_kernel,
    Kernel *y_kernel) {
  bool alpha;
  byte channel;
  byte num_channels;
  ulong default_threads;
  ulong in_width;
  ulong in_height;
  ulong num_threads;
  ulong num_threads_x;
  ulong num_threads_y;
  ulong out_width;
  ulong out_height;
  ulong t;
  ulong temp_width;
  ulong temp_height;
  ulong x_operations;
  ulong y_operations;
  double **temp = null;
  void **xs = null;
  Info_ info;
  DoubleImage *this;
  ParallelRange *ranges = null;
  ParallelInfo_ *p;
  ParallelInfo_ *ps = null;

  in_width = other->width;
  in_height = other->height;

  if (x_kernel->n_in != in_width) {
    throw(
        "X kernel n_in (%lu) must equal input image width (%lu)",
        x_kernel->n_in,
        in_width);
  }
  if (y_kernel->n_in != in_height) {
    throw(
        "Y kernel n_in (%lu) must equal input image height (%lu)",
        y_kernel->n_in,
        in_height);
  }

  out_width = x_kernel->n_out;
  out_height = y_kernel->n_out;

  temp_width = out_width;
  temp_height = in_height;

  alpha = other->a != null;

  x_operations = temp_height * x_kernel->num_elements;
  y_operations = out_width * y_kernel->num_elements;

  default_threads = parallel_get_default_num_threads();

  num_threads_x = x_operations > MINIMUM_PARALLEL_ ? default_threads : 1;
  num_threads_y = y_operations > MINIMUM_PARALLEL_ ? default_threads : 1;

  info.input = other;
  info.output = this = double_image_new(pthis, out_width, out_height, alpha);
  info.temp = newm(&temp, temp_width, temp_height);

  num_channels = other->a == null ? 3 : 4;

  for (channel = 0; channel < num_channels; channel++) {
    info.channel = channel;

    num_threads = num_threads_x;

    new(&ps, num_threads);
    new(&xs, num_threads);

    parallel_compute_ranges(&ranges, 0, temp_height, num_threads);

    for (t = 0; t < num_threads; t++) {
      xs[t] = p = &ps[t];
      p->start = ranges[t].start;
      p->end = ranges[t].end;
      p->kernel = x_kernel;
      p->read = read_x_;
      p->write = write_x_;
      p->info = info;
    }

    deletev(&ranges);

    parallel_do(apply_, num_threads, xs);

    deletev(&xs);
    deletev(&ps);

    num_threads = num_threads_y;

    new(&ps, num_threads);
    new(&xs, num_threads);

    parallel_compute_ranges(&ranges, 0, out_width, num_threads);

    for (t = 0; t < num_threads; t++) {
      xs[t] = p = &ps[t];
      p->start = ranges[t].start;
      p->end = ranges[t].end;
      p->kernel = y_kernel;
      p->read = read_y_;
      p->write = write_y_;
      p->info = info;
    }

    deletev(&ranges);

    parallel_do(apply_, num_threads, xs);

    deletev(&xs);
    deletev(&ps);
  }

  deletemv(&temp, temp_width);

  return this;
}

DoubleImage *double_image_new_apply_multiple_xy_kernels(
    DoubleImage **pthis,
    DoubleImage *other,
    ulong num_kernel_pairs,
    Kernel **x_kernels,
    Kernel **y_kernels) {
  byte k;
  DoubleImage *temp_1 = null;
  DoubleImage *temp_2 = null;

  for (k = 0; k < num_kernel_pairs; k++) {
    double_image_new_apply_xy_kernels(
        &temp_2,
        k == 0 ? other : temp_1,
        x_kernels[k],
        y_kernels[k]);
    double_image_delete(&temp_1);
    donate(&temp_2, &temp_1);
  }

  donate(&temp_1, pthis);

  return *pthis;
}

static void *apply_(void *x) {
  ulong end;
  ulong start;
  ulong u;
  ParallelInfo_ *p;
  double (*read)(ulong in, void *x);
  void (*write)(ulong out, double v, void *x);
  Kernel *kernel;

  p = x;
  start = p->start;
  end = p->end;
  kernel = p->kernel;
  read = p->read;
  write = p->write;

  for (u = start; u < end; u++) {
    p->info.u = u;
    kernel_apply(kernel, read, write, &p->info, 1);
  }

  return null;
}

static double read_x_(ulong in, void *x) {
  byte c;
  ulong i;
  ulong j;
  Info_ *info;

  info = x;
  c = info->channel;
  i = in;
  j = info->u;

  return info->input->channels[c][i][j];
}

static void write_x_(ulong out, double v, void *x) {
  ulong i;
  ulong j;
  Info_ *info;

  info = x;
  i = out;
  j = info->u;

  info->temp[i][j] = v;
}

static double read_y_(ulong in, void *x) {
  ulong i;
  ulong j;
  Info_ *info;

  info = x;
  i = info->u;
  j = in;

  return info->temp[i][j];
}

static void write_y_(ulong out, double v, void *x) {
  byte c;
  ulong i;
  ulong j;
  Info_ *info;

  info = x;
  c = info->channel;
  i = info->u;
  j = out;

  info->output->channels[c][i][j] = v;
}

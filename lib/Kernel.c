#include "Kernel.h"
#include <limits.h>
#include <math.h>

typedef struct {
  ulong start;
  ulong end;
  Kernel *this;
  double (*read)(ulong in, void *x);
  void (*write)(ulong out, double v, void *x);
  void *x;
} Info_;

static Kernel *new_(Kernel **pthis, ulong n_in, ulong n_out, bool periodic);
static void *apply_(void *x);
static ulong bound_(long in, ulong n_in, bool periodic);
static KernelPossible identity_possible_(ulong out, void *x);
static double identity_weight_(ulong out, long in, void *x);

static ulong create_(
    ulong **pins,
    double **pweights,
    ulong n_in,
    ulong out,
    double *ws,
    ulong min,
    ulong max);

Kernel *kernel_new(
    Kernel **pthis,
    ulong n_in,
    ulong n_out,
    bool periodic,
    KernelPossible (*possible)(ulong out, void *x),
    double (*weight)(ulong out, long in, void *x),
    void *x) {
  ulong bounded;
  ulong min;
  ulong max;
  ulong n;
  ulong out;
  ulong num_elements;
  long first;
  long in;
  long last;
  double *ws = null;
  KernelPossible possible_in;
  Kernel *this;

  this = new_(pthis, n_in, n_out, periodic);

  if (possible == null) {
    throw("'possible' function pointer cannot be null");
  }

  if (weight == null) {
    throw("'weight' function pointer cannot be null");
  }

  num_elements = 0;

  for (out = 0; out < n_out; out++) {
    new(&ws, n_in);
    possible_in = possible(out, x);
    first = possible_in.first;
    last = possible_in.last;

    if (last < first) {
      throw(
          "last (%ld) cannot be less than first (%ld) for out = %lu",
          last,
          first,
          out);
    }

    min = ULONG_MAX;
    max = 0;

    for (in = first; in <= last; in++) {
      bounded = bound_(in, n_in, periodic);
      min = minimum(min, bounded);
      max = maximum(max, bounded);
      ws[bounded] += weight(out, in, x);
    }

    n = create_(
        &this->ins[out],
        &this->weights[out],
        n_in,
        out,
        ws,
        min,
        max);

    this->ns[out] = n;
    num_elements += n;
    deletev(&ws);
  }

  this->num_elements = num_elements;

  return this;
}

Kernel *kernel_new_compose(Kernel **pthis, Kernel *k1, Kernel *k2) {
  ulong first_1;
  ulong a_1;
  ulong a_2;
  ulong in;
  ulong join;
  ulong last_1;
  ulong n_in;
  ulong n_join;
  ulong n_out;
  ulong num_elements;
  ulong out;
  ulong n;
  ulong n_1;
  ulong n_2;
  ulong *ins_1;
  ulong *ins_2;
  ulong *ns_1;
  ulong *ns_2;
  ulong **all_ins_1;
  ulong **all_ins_2;
  double w;
  double *ws = null;
  double **weights_1;
  double **weights_2;
  Kernel *this;

  if (k2->n_in != (n_join = k1->n_out)) {
    throw(
        "n_in (%lu) for the second kernel must equal "
        "n_out (%lu) of the first kernel",
        k2->n_in,
        k1->n_out);
  }

  if (k1->periodic != k2->periodic) {
    throw(
        "Periodic flag for first kernel (%s) must match that "
        "of the second (%s)",
        bool_string(k1->periodic),
        bool_string(k2->periodic));
  }

  n_out = k2->n_out;
  n_in = k1->n_in;

  this = new_(pthis, n_in, n_out, k1->periodic);
  num_elements = 0;
  ns_1 = k1->ns;
  ns_2 = k2->ns;
  all_ins_1 = k1->ins;
  all_ins_2 = k2->ins;
  weights_1 = k1->weights;
  weights_2 = k2->weights;

  for (out = 0; out < n_out; out++) {
    new(&ws, n_in);
    n_2 = ns_2[out];
    ins_2 = all_ins_2[out];

    for (in = 0; in < n_in; in++) {
      for (a_2 = 0, w = 0; a_2 < n_2; a_2++) {
        join = ins_2[a_2];

        n_1 = ns_1[join];
        ins_1 = all_ins_1[join];
        first_1 = ins_1[0];
        last_1 = ins_1[n_1 - 1];

        if (in < first_1 || in > last_1) {
          continue;
        }

        for (a_1 = 0; a_1 < n_1; a_1++) {
          if (ins_1[a_1] == in) {
            w += weights_2[out][a_2] * weights_1[join][a_1];
            break;
          }
        }
      }

      ws[in] = w;
    }

    n = create_(
        &this->ins[out],
        &this->weights[out],
        n_in,
        out,
        ws,
        0,
        n_in - 1);

    this->ns[out] = n;
    num_elements += n;
    deletev(&ws);
  }

  this->num_elements = num_elements;
  return this;
}

Kernel *kernel_new_identity(Kernel **pthis, ulong n) {
  return kernel_new(
      pthis,
      n,
      n,
      false,
      identity_possible_,
      identity_weight_,
      null);
}

void kernel_delete(Kernel **pthis) {
  Kernel *this;

  if ((this = *pthis) == null) {
    return;
  }

  delete(&this->weights, this->n_out, value_array_delete);
  delete(&this->ins, this->n_out, value_array_delete);
  deletev(&this->ns);

  deleteo(pthis);
}

void kernel_print(Kernel *this, FILE *f, byte n) {
  ulong a;
  ulong nn;
  ulong out;
  ulong *ins;
  double *weights;

  fprintfn(f, n++, "[Kernel]");
  fprintfn(f, n, "n_in: %lu", this->n_in);
  fprintfn(f, n, "n_out: %lu", this->n_out);
  fprintfn(f, n, "num_elements: %lu", this->num_elements);
  fprintfn(f, n, "periodic: %s", bool_string(this->periodic));

  for (out = 0; out < this->n_out; out++) {
    fprintfn(f, n++, "out = %lu:", out);
    nn = this->ns[out];
    ins = this->ins[out];
    weights = this->weights[out];

    for (a = 0; a < nn; a++) {
      fprintfn(f, n, "in = %lu: w = %g", ins[a], weights[a]);
    }

    n--;
  }
}

void kernel_apply(
    Kernel *this,
    double (*read)(ulong in, void *x),
    void (*write)(ulong out, double v, void *x),
    void *x,
    ulong num_threads) {
  ulong u;
  void **xs = null;
  ParallelRange *ranges = null;
  Info_ *infos = null;
  Info_ *info;

  if (num_threads == 0) {
    num_threads = parallel_get_default_num_threads();
  }

  new(&infos, num_threads);
  new(&xs, num_threads);

  parallel_compute_ranges(&ranges, 0, this->n_out, num_threads);

  for (u = 0; u < num_threads; u++) {
    xs[u] = info = &infos[u];
    info->start = ranges[u].start;
    info->end = ranges[u].end;
    info->read = read;
    info->write = write;
    info->x = x;
    info->this = this;
  }

  deletev(&ranges);

  parallel_do(apply_, num_threads, xs);

  deletev(&xs);
  deletev(&infos);
}

static ulong create_(
    ulong **pins,
    double **pweights,
    ulong n_in,
    ulong out,
    double *ws,
    ulong min,
    ulong max) {
  ulong a;
  ulong in;
  ulong n;
  ulong *ins;
  double normalization;
  double sum;
  double w;
  double *weights;

  for (in = min, n = 0, sum = 0; in <= max; in++) {
    if ((w = ws[in]) != 0) {
      n++;
      sum += w;
    }
  }

  if (n == 0) {
    throw("Did not have any nonzero weights for out = %lu", out);
  }

  if (sum < 0) {
    throw(
        "Sum of weights (%g) for out = %lu was negative, which is not "
        "supported", sum, out);
  }

  normalization = 1 / sum;
  ins = new(pins, n);
  weights = new(pweights, n);

  for (in = min, a = 0; in <= max; in++) {
    if ((w = ws[in]) != 0) {
      ins[a] = in;
      weights[a++] = w * normalization;
    }
  }

  return n;
}

static void *apply_(void *x) {
  ulong a;
  ulong end;
  ulong n;
  ulong out;
  ulong start;
  ulong *ins;
  double sum;
  double *weights;
  void *xx;
  Info_ *info;
  double (*read)(ulong in, void *x);
  void (*write)(ulong out, double v, void *x);
  Kernel *this;

  info = x;
  this = info->this;
  start = info->start;
  end = info->end;
  read = info->read;
  write = info->write;
  xx = info->x;

  for (out = start; out < end; out++) {
    n = this->ns[out];
    ins = this->ins[out];
    weights = this->weights[out];

    sum = 0;

    for (a = 0, sum = 0; a < n; a++) {
      sum += weights[a] * read(ins[a], xx);
    }

    write(out, sum, xx);
  }

  return null;
}

bool kernel_equals(Kernel *this, Kernel *other, double tolerance) {
  ulong in;
  ulong n_in;
  ulong n_out;
  ulong out;
  double w_this;
  double w_other;

  if ((n_in = this->n_in) != other->n_in) {
    return false;
  }
  if ((n_out = this->n_out) != other->n_out) {
    return false;
  }

  for (out = 0; out < n_out; out++) {
    for (in = 0; in < n_in; in++) {
      w_this = kernel_get(this, out, in);
      w_other = kernel_get(other, out, in);

      if (fabs(w_other - w_this) > tolerance) {
        return false;
      }
    }
  }

  return true;
}

double kernel_resize_out_from_in(double in, double k) {
  return k * (in + 0.5) - 0.5;
}

double kernel_resize_in_from_out(double out, double k) {
  return (out + 0.5) / k - 0.5;
}

static int ulong_cmp_(void *a, void *b, void *x) {
  return (long)*(ulong *)a - (long)*(ulong *)b;
}

double kernel_get(Kernel *this, ulong out, ulong in) {
  ulong a;
  ulong n;
  ulong *ins;

  if (in >= this->n_in) {
    throw("in (%lu) must be less than n_in (%lu)", in, this->n_in);
  }

  if (out >= this->n_out) {
    throw("out (%lu) must be less than n_out (%lu)", out, this->n_out);
  }

  n = this->ns[out];
  ins = this->ins[out];

  if (in < ins[0] || in > ins[n - 1]) {
    return 0;
  }

  if (!bsearchx_try_find_index(&in, ins, n, ulong_cmp_, null, &a)) {
    return 0;
  }

  return this->weights[out][a];
}

static Kernel *new_(Kernel **pthis, ulong n_in, ulong n_out, bool periodic) {
  Kernel *this;

  if (n_in == 0) {
    throw("n_in cannot be zero");
  }
  if (n_out == 0) {
    throw("n_out cannot be zero");
  }

  this = newo(pthis);

  this->n_in = n_in;
  this->n_out = n_out;
  this->periodic = periodic;

  new(&this->ns, n_out);
  new(&this->ins, n_out);
  new(&this->weights, n_out);

  return this;
}

static KernelPossible identity_possible_(ulong out, void *x) {
  KernelPossible possible;

  possible.first = possible.last = (long)out;

  return possible;
}

static double identity_weight_(ulong out, long in, void *x) {
  return 1;
}

static ulong bound_(long in, ulong n_in, bool periodic) {
  if (in >= 0 && in < n_in) {
    return in;
  }
  if (periodic) {
    return modulo(in, n_in);
  }
  if (in < 0) {
    return 0;
  }
  return n_in - 1;
}

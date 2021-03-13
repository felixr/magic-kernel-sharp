#include "parallel.h"
#include <limits.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

static ulong default_num_threads_ = 0;
static ulong denominator_ = 0;

typedef struct {
  byte *src;
  byte *dst;
  ulong num_bytes;
} MemcpyInfo_;

typedef struct {
  void *base1;
  void *base2;
  void *base3;
  ulong n1;
  ulong n2;
  ulong size;
  int (*cmp)(void *a, void *b, void *x);
  void *x;
} QsortxInfo_;

typedef struct {
  bool deleteA;
  bool deleteB;
  void *result;
  void *a;
  void *b;
} MergeSpec_;

typedef struct {
  void (*merge)(void **pthis, void *a, void *b, void *x);
  void (*destructor)(void **pthis);
  void *x;
  MergeSpec_ *specs;
} MergeCommon_;

typedef struct {
  MergeCommon_ *common;
  ulong start;
  ulong end;
} MergeInfo_;

static void *memcpy_(void *x);
static void *merge_(void *x);

void parallel_do(
    void *(*run)(void *x),
    ulong num_threads,
    void **xs) {
  bool fail = false;
  bool *started = null;
  bool *finished = null;
  ulong t;
  ulong num_waiting;
  void *status;
  pthread_t *threads = null;
  Random *random = null;

  if (num_threads == 0) {
    throw("Must have at least one thread");
  }

  if (num_threads == 1) {
    run(xs[0]);
    return;
  }

  new(&threads, num_threads);
  new(&started, num_threads);
  new(&finished, num_threads);

  if (denominator_ != 0) {
    random_new(&random);
  }

  do {
    for (t = 0, num_waiting = 0; t < num_threads; t++) {
      if (!finished[t]) {
        if (denominator_ != 0) {
          fail = random_modulo(random, denominator_) == 0;
        }
        if (fail || pthread_create(&threads[t], null, run, xs[t]) != 0) {
          num_waiting++;
        } else {
          started[t] = true;
        }
      }
    }

    for (t = 0; t < num_threads; t++) {
      if (started[t] && !finished[t]) {
        if (pthread_join(threads[t], &status) != 0) {
          throw("Thread #%lu did not successfully complete", t);
        } else {
          finished[t] = true;
        }
      }
    }

    if (num_waiting > 0) {
      sleep(1);
    }
  } while (num_waiting > 0);

  for (t = 0; t < num_threads; t++) {
    if (!started[t]) {
      throw("Impossible: thread %lu not started", t);
    }
    if (!finished[t]) {
      throw("Impossible: thread %lu not finished", t);
    }
  }

  if (denominator_ != 0) {
    random_delete(&random);
  }

  deletev(&finished);
  deletev(&started);
  deletev(&threads);
}

void parallel_memcpy(
    void *dst,
    void *src,
    ulong num_bytes,
    ulong num_threads) {
  ulong u;
  long s;
  ParallelRange *ranges = null;
  MemcpyInfo_ *infos = null;
  MemcpyInfo_ *info;
  void **xs = null;

  if (num_threads == 0) {
    num_threads = parallel_get_default_num_threads();
  }

  if (num_threads == 1) {
    memcpy(dst, src, num_bytes);
    return;
  }

  new(&infos, num_threads);
  new(&xs, num_threads);

  parallel_compute_ranges(&ranges, 0, num_bytes, num_threads);

  for (u = 0; u < num_threads; u++) {
    xs[u] = info = &infos[u];
    info->src = (byte *)src + (s = ranges[u].start);
    info->dst = (byte *)dst + s;
    info->num_bytes = ranges[u].end - s;
  }

  deletev(&ranges);

  parallel_do(memcpy_, num_threads, xs);

  deletev(&xs);
  deletev(&infos);
}

ulong parallel_get_default_num_threads(void) {
  ulong total_processors;

  if (default_num_threads_ != 0) {
    return default_num_threads_;
  }

  total_processors = processors_get_total();
  return default_num_threads_ = total_processors > 2 ? total_processors - 1 : 1;
}

ulong parallel_get_num_threads(ulong min_count_per_thread, ulong count) {
  return min_count_per_thread == 0 || count <= min_count_per_thread ? 1 :
    minimum((count + min_count_per_thread - 1) / min_count_per_thread,
        parallel_get_default_num_threads());
}

ParallelRange *parallel_compute_ranges(
    ParallelRange **pranges,
    long start,
    long end,
    ulong num_threads) {
  ulong n;
  ulong u;
  long s;
  ParallelRange *ranges;

  if (num_threads == 0) {
    throw("Must have at least one thread");
  }

  ranges = new(pranges, num_threads);

  n = end - start;

  if (n > LONG_MAX) {
    throw(
        "n (%ld) cannot be greater than can be stored in a long (%lu)",
        n,
        LONG_MAX);
  }
  if ((long)n >= LONG_MAX / num_threads) {
    throw("n (%lu) cannot be greater than %lu", n, LONG_MAX / num_threads);
  }

  for (u = 0, s = start; u < num_threads; u++) {
    ranges[u].start = s;
    ranges[u].end = s = start + (long)((u + 1) * n / num_threads);
  }

  return ranges;
}

void parallel_mergeI(
    void **pthis,
    ulong num_others,
    void **others,
    void (*merge)(void **pthis, void *a, void *b, void *x),
    void (*destructor)(void **pthis),
    void *x,
    bool delete_inputs,
    ulong num_threads) {
  bool *prev_deletes = null;
  bool *new_deletes = null;
  ulong num_prev_objects;
  ulong num_merges;
  ulong num_new_objects;
  ulong p;
  ulong n;
  ulong t;
  MergeCommon_ common;
  MergeSpec_ *spec;
  MergeSpec_ *specs = null;
  MergeInfo_ *info;
  MergeInfo_ *infos = null;
  ParallelRange *ranges = null;
  void **xs = null;
  void **new_objects = null;
  void **prev_objects = null;

  if (num_others < 2) {
    throw(
        "Must have at least two objects to merge; you supplied %lu",
        num_others);
  }

  new(&prev_objects, num_others);
  new(&prev_deletes, num_others);
  for (p = 0; p < num_others; p++) {
    if (delete_inputs) {
      donate(&others[p], &prev_objects[p]);
      prev_deletes[p] = true;
    } else {
      prev_objects[p] = others[p];
      prev_deletes[p] = false;
    }
  }

  num_prev_objects = num_others;

  common.merge = merge;
  common.destructor = destructor;
  common.x = x;

  while (num_prev_objects > 1) {
    for (p = 0; p < num_prev_objects; p++) {
      if (prev_objects[p] == null) {
        throw("Previous object %lu is empty", p);
      }
    }

    num_merges = num_prev_objects / 2;
    num_new_objects = (num_prev_objects + 1) / 2;

    new(&new_objects, num_new_objects);
    new(&new_deletes, num_new_objects);

    common.specs = new(&specs, num_merges);
    for (n = 0, p = 0; n < num_merges; n++) {
      spec = &specs[n];
      donate(&prev_objects[p], &spec->a);
      spec->deleteA = prev_deletes[p++];
      donate(&prev_objects[p], &spec->b);
      spec->deleteB = prev_deletes[p++];
    }

    num_threads = minimum(num_threads, num_merges);

    new(&infos, num_threads);
    new(&xs, num_threads);

    parallel_compute_ranges(&ranges, 0, num_merges, num_threads);

    for (t = 0; t < num_threads; t++) {
      xs[t] = info = &infos[t];
      info->common = &common;
      info->start = ranges[t].start;
      info->end = ranges[t].end;
    }

    deletev(&ranges);

    parallel_do(merge_, num_threads, xs);

    deletev(&xs);
    deletev(&infos);

    for (n = 0, p = 0; n < num_merges; n++) {
      donate(&specs[n].result, &new_objects[n]);
      new_deletes[n] = true;
      spec = &specs[n];
      if (prev_deletes[p++]) {
        if (spec->a != null) {
          throw("a should be empty");
        }
      } else {
        if (spec->a == null) {
          throw("a should not be empty");
        }
      }
      if (prev_deletes[p++]) {
        if (spec->b != null) {
          throw("b should be empty");
        }
      } else {
        if (spec->b == null) {
          throw("b should not be empty");
        }
      }
    }

    if (num_new_objects > num_merges) {
      donate(&prev_objects[p], &new_objects[n]);
      new_deletes[n++] = prev_deletes[p++];
    }

    if (n != num_new_objects) {
      throw(
          "Expected n (%lu) to equal num_new_objects (%lu)",
          n,
          num_new_objects);;
    }
    if (p != num_prev_objects) {
      throw(
          "Expected p (%lu) to equal num_prev_objects (%lu)",
          p,
          num_prev_objects);;
    }

    deletev(&prev_objects);
    deletev(&prev_deletes);
    donate(&new_objects, &prev_objects);
    donate(&new_deletes, &prev_deletes);

    deletev(&specs);

    num_prev_objects = num_new_objects;
  }

  if (num_prev_objects != 1) {
    throw(
        "Expected number of previous objects (%lu) to be 1",
        num_prev_objects);
  }

  donate(&prev_objects[0], pthis);

  deletev(&prev_objects);
  deletev(&prev_deletes);
}

static void *memcpy_(void *x) {
  MemcpyInfo_ *info;

  info = x;

  memcpy(info->dst, info->src, info->num_bytes);

  return null;
}

static void *merge_(void *x) {
  ulong start;
  ulong end;
  ulong u;
  MergeCommon_ *common;
  MergeInfo_ *info;
  MergeSpec_ *spec;
  MergeSpec_ *specs;
  void (*merge)(void **pthis, void *a, void *b, void *x);
  void (*destructor)(void **pthis);

  info = x;

  common = info->common;

  merge = common->merge;
  destructor = common->destructor;
  x = common->x;
  specs = common->specs;

  start = info->start;
  end = info->end;
  if (end <= start) {
    throw("End (%lu) should be greater than start (%lu)", end, start);
  }

  for (u = start; u < end; u++) {
    spec = &specs[u];
    merge(&spec->result, spec->a, spec->b, x);
    if (spec->deleteA) {
      destructor(&spec->a);
    }
    if (spec->deleteB) {
      destructor(&spec->b);
    }
  }

  return null;
}

void parallel_simulate_resource_constraints(ulong denominator) {
  denominator_ = denominator;
}


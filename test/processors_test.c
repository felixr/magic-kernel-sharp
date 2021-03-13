#include "processors_test.h"

START

TEST(Processors, Run) {
  /* Just make sure that the functions work on your operating system. The
   * actual values are hardware-dependent.
   */
  processors_get_total();
  processors_get_memory_per();
  processors_get_free();
}

FINISH

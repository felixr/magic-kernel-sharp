#include "RandomTestDeath.h"

START_DEATH

TEST(Random, Death) {
  Random *random = null;

  random_new(&random);
  EXPECT_DEATH(random_modulo(random, 0), "Modulus cannot be zero");



}

FINISH_DEATH

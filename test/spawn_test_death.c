#include "spawn_test_death.h"

START_DEATH

TEST(Spawn, Simple) {
  /* Erroneous command.
   */
  EXPECT_DEATH_SUBSTRING(
      spawn_simple("bogus-command 2>/dev/null"),
      0,
      56,
      "Command 'bogus-command 2>/dev/null' returned error code ");
}

FINISH_DEATH

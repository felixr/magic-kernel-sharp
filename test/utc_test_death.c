#include "utc_test_death.h"

START_DEATH

TEST(UTC, Death) {
  char *s = null;

  EXPECT_DEATH(
      string_new_utc(&s, "", "", "", "", true, false, true, false, 0),
      "Can't specify minutes, seconds or decimal places without time");
  string_delete(&s);

  EXPECT_DEATH(
      string_new_utc(&s, "", "", "", "", true, false, false, true, 0),
      "Can't specify minutes, seconds or decimal places without time");
  string_delete(&s);

  EXPECT_DEATH(
      string_new_utc(&s, "", "", "", "", true, false, false, false, 1),
      "Can't specify minutes, seconds or decimal places without time");
  string_delete(&s);

  EXPECT_DEATH(
      string_new_utc(&s, "", "", "", "", true, true, false, true, 0),
      "Can't specify seconds or decimal places without minutes");
  string_delete(&s);

  EXPECT_DEATH(
      string_new_utc(&s, "", "", "", "", true, true, false, false, 2),
      "Can't specify seconds or decimal places without minutes");
  string_delete(&s);

  EXPECT_DEATH(
      string_new_utc(&s, "", "", "", "", true, true, true, false, 5),
      "Can't specify decimal places without seconds");
  string_delete(&s);

  EXPECT_DEATH(
      string_new_utc(&s, "", "", "", "", true, true, true, false, 7),
      "Decimal places (7) cannot be more than 6");
  string_delete(&s);
}

FINISH_DEATH

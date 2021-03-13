#include "utc_test.h"

START

TEST(UTC, Regular) {
  char *s = null;
  char *e;
  struct timeval tv = {1393548153, 704907};

  e = "2014-02-28";
  EXPECT_STREQ(
      e,
      string_new_utc_from_timeval(
        &s,
        &tv,
        "-",
        "boo",
        "ha",
        "whoa",
        true,
        false,
        false,
        false,
        0));
  EXPECT_STREQ(e, s);
  string_delete(&s);

  e = "20140228T00";
  EXPECT_STREQ(
      e,
      string_new_utc_from_timeval(
        &s,
        &tv,
        "",
        "T",
        "ha",
        "whoa",
        true,
        true,
        false,
        false,
        0));
  EXPECT_STREQ(e, s);
  string_delete(&s);

  e = "2014-02-28 00:42";
  EXPECT_STREQ(
      e,
      string_new_utc_from_timeval(
        &s,
        &tv,
        "-",
        " ",
        ":",
        "whoa",
        true,
        true,
        true,
        false,
        0));
  EXPECT_STREQ(e, s);
  string_delete(&s);

  e = "2014/02/28 time 004233";
  EXPECT_STREQ(
      e,
      string_new_utc_from_timeval(
        &s,
        &tv,
        "/",
        " time ",
        "",
        "whoa",
        true,
        true,
        true,
        true,
        0));
  EXPECT_STREQ(e, s);
  string_delete(&s);

  e = "2014-02-28T00:42:33.7";
  EXPECT_STREQ(
      e,
      string_new_utc_from_timeval(
        &s,
        &tv,
        "-",
        "T",
        ":",
        ".",
        true,
        true,
        true,
        true,
        1));
  EXPECT_STREQ(e, s);
  string_delete(&s);

  e = "2014-02-28T00:42:33.70";
  EXPECT_STREQ(
      e,
      string_new_utc_from_timeval(
        &s,
        &tv,
        "-",
        "T",
        ":",
        ".",
        true,
        true,
        true,
        true,
        2));
  EXPECT_STREQ(e, s);
  string_delete(&s);

  e = "2014-02-28T00:42:33.705";
  EXPECT_STREQ(
      e,
      string_new_utc_from_timeval(
        &s,
        &tv,
        "-",
        "T",
        ":",
        ".",
        true,
        true,
        true,
        true,
        3));
  EXPECT_STREQ(e, s);
  string_delete(&s);

  e = "2014-02-28T00:42:33.7049";
  EXPECT_STREQ(
      e,
      string_new_utc_from_timeval(
        &s,
        &tv,
        "-",
        "T",
        ":",
        ".",
        true,
        true,
        true,
        true,
        4));
  EXPECT_STREQ(e, s);
  string_delete(&s);

  e = "2014-02-28T00:42:33.70491";
  EXPECT_STREQ(
      e,
      string_new_utc_from_timeval(
        &s,
        &tv,
        "-",
        "T",
        ":",
        ".",
        true,
        true,
        true,
        true,
        5));
  EXPECT_STREQ(e, s);
  string_delete(&s);

  e = "2014-02-28T00:42:33.704907";
  EXPECT_STREQ(
      e,
      string_new_utc_from_timeval(
        &s,
        &tv,
        "-",
        "T",
        ":",
        ".",
        true,
        true,
        true,
        true,
        6));
  EXPECT_STREQ(e, s);
  string_delete(&s);

  e = "00:42:33";
  EXPECT_STREQ(
      e,
      string_new_utc_from_timeval(
        &s,
        &tv,
        "-",
        "T",
        ":",
        ".",
        false,
        true,
        true,
        true,
        0));
  EXPECT_STREQ(e, s);
  string_delete(&s);

  e = "00:42:33.7";
  EXPECT_STREQ(
      e,
      string_new_utc_from_timeval(
        &s,
        &tv,
        "-",
        "T",
        ":",
        ".",
        false,
        true,
        true,
        true,
        1));
  EXPECT_STREQ(e, s);
  string_delete(&s);

  e = "00:42:33.70";
  EXPECT_STREQ(
      e,
      string_new_utc_from_timeval(
        &s,
        &tv,
        "-",
        "T",
        ":",
        ".",
        false,
        true,
        true,
        true,
        2));
  EXPECT_STREQ(e, s);
  string_delete(&s);

  e = "00:42:33.705";
  EXPECT_STREQ(
      e,
      string_new_utc_from_timeval(
        &s,
        &tv,
        "-",
        "T",
        ":",
        ".",
        false,
        true,
        true,
        true,
        3));
  EXPECT_STREQ(e, s);
  string_delete(&s);

  e = "00:42:33.7049";
  EXPECT_STREQ(
      e,
      string_new_utc_from_timeval(
        &s,
        &tv,
        "-",
        "T",
        ":",
        ".",
        false,
        true,
        true,
        true,
        4));
  EXPECT_STREQ(e, s);
  string_delete(&s);

  e = "00:42:33.70491";
  EXPECT_STREQ(
      e,
      string_new_utc_from_timeval(
        &s,
        &tv,
        "-",
        "T",
        ":",
        ".",
        false,
        true,
        true,
        true,
        5));
  EXPECT_STREQ(e, s);
  string_delete(&s);

  e = "00:42:33.704907";
  EXPECT_STREQ(
      e,
      string_new_utc_from_timeval(
        &s,
        &tv,
        "-",
        "T",
        ":",
        ".",
        false,
        true,
        true,
        true,
        6));
  EXPECT_STREQ(e, s);
  string_delete(&s);
}

FINISH

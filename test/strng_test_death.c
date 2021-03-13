#include "strng_test_death.h"

START_DEATH

TEST(String, NewEmpty) {
  char *s = null;

  string_new_empty(&s);
  EXPECT_DEATH(string_new_empty(&s), "Pointer not empty in new() or newo()");
  string_delete(&s);
}

TEST(String, NewChar) {
  char *s = null;

  string_new_char(&s, 'p');
  EXPECT_DEATH(
      string_new_char(&s, 'w'),
      "Pointer not empty in new() or newo()");
  string_delete(&s);
}

TEST(String, NewCopy) {
  char orig[] = "Hi there";
  char *copy = null;
  char *local;

  string_new_copy(&copy, orig);
  local = copy;
  string_delete(&copy);
  EXPECT_DEATH(
      string_new_copy(&local, orig),
      "Pointer not empty in new() or newo()");
}

TEST(String, NewF) {
  char *s = null;
  char *local;

  local = string_new_f(&s, "%d %c %.1f", 42, 'j', 3.14159);
  string_delete(&s);
  EXPECT_DEATH(
      string_new_f(&local, ""),
      "Pointer not empty in new() or newo()");
}

TEST(String, NewConcat) {
  char *s = null;
  char a[] = "Hi there";
  char b[] = "big spender";
  char *c = null;

  new(&c, 2);
  c[0] = '!';
  c[1] = '\0';
  string_new_concat_until_null(&s, a, " - ", b, c, null);
  EXPECT_DEATH(
      string_new_concat_until_null(&s, null),
      "Pointer not empty in new() or newo()");
  string_delete(&s);
}

TEST(String, NewTruncate) {
  char *s = null;

  EXPECT_DEATH(
      string_new_truncate(&s, "cat", 4),
      "Cannot truncate string 'cat' to a length of 4 characters");
  EXPECT_DEATH(
      string_new_truncate(&s, "", 1),
      "Cannot truncate string '' to a length of 1 character");
}

TEST(String, NewShorten) {
  char *s = null;

  EXPECT_DEATH(
      string_new_shorten(&s, "cat", 4),
      "Cannot shorten string 'cat' by 4 characters");
  EXPECT_DEATH(
      string_new_shorten(&s, "", 1),
      "Cannot shorten string '' by 1 character");
}

TEST(String, NewSubstring) {
  char *s = null;

  EXPECT_DEATH(
      string_new_substring(&s, "", 0, 1),
      "First index 0, length 1 invalid for '' (length 0)");
  EXPECT_DEATH(
      string_new_substring(&s, "", 0, 2),
      "First index 0, length 2 invalid for '' (length 0)");
  EXPECT_DEATH(
      string_new_substring(&s, "", 1, 1),
      "First index 1, length 1 invalid for '' (length 0)");
  EXPECT_STREQ("a", string_new_substring(&s, "a", 0, 1));
  EXPECT_STREQ("a", s);
  string_delete(&s);
  EXPECT_DEATH(
      string_new_substring(&s, "a", 1, 1),
      "First index 1, length 1 invalid for 'a' (length 1)");
  EXPECT_DEATH(
      string_new_substring(&s, "a", 0, 2),
      "First index 0, length 2 invalid for 'a' (length 1)");
  string_delete(&s);
  EXPECT_DEATH(
      string_new_substring(&s, "ab", 2, 1),
      "First index 2, length 1 invalid for 'ab' (length 2)");
  EXPECT_DEATH(
      string_new_substring(&s, "ab", 1, 2),
      "First index 1, length 2 invalid for 'ab' (length 2)");
  EXPECT_DEATH(
      string_new_substring(&s, "ab", 0, 3),
      "First index 0, length 3 invalid for 'ab' (length 2)");
}

TEST(String, MaxLen) {
  /* Invalid format strings.
   */
  EXPECT_DEATH(
      string_f_get_max_len("%--d"),
      "'-' was already specified in format string");
  EXPECT_DEATH(
      string_f_get_max_len("%++d"),
      "'+' was already specified in format string");
  EXPECT_DEATH(
      string_f_get_max_len("%  d"),
      "' ' was already specified in format string");
  EXPECT_DEATH(
      string_f_get_max_len("% +d"),
      "Can't specify both '+' and ' ' in format string");
  EXPECT_DEATH(
      string_f_get_max_len("%+ d"),
      "Can't specify both '+' and ' ' in format string");
  EXPECT_DEATH(
      string_f_get_max_len("%00d"),
      "'0' was already specified in format string");
  EXPECT_DEATH(
      string_f_get_max_len("%##d"),
      "'#' was already specified in format string");
  EXPECT_DEATH(
      string_f_get_max_len("%..d"),
      "Character '.' at index 2 of format string '%..d' is invalid");
  EXPECT_DEATH(
      string_f_get_max_len("%D"),
      "Character 'D' at index 1 of format string '%D' is invalid");
  EXPECT_DEATH(
      string_f_get_max_len("%Ld"),
      "Format 'Ld' is invalid");
  EXPECT_DEATH(
      string_f_get_max_len("%hf"),
      "Format 'hf' is invalid");
  EXPECT_DEATH(
      string_f_get_max_len("%lf"),
      "Format 'lf' is invalid");
}

TEST(String, Copy) {
  char s[10];
  char *e = "";

  e = "a";
  EXPECT_DEATH(string_copy(e, s, 0), "max_len 0 but string length 1");
  string_copy(e, s, 1);
  EXPECT_STREQ(e, s);
  string_copy(e, s, 2);
  EXPECT_STREQ(e, s);
  e = "ab";
  EXPECT_DEATH(string_copy(e, s, 0), "max_len 0 but string length 2");
  EXPECT_DEATH(string_copy(e, s, 1), "max_len 1 but string length 2");
  string_copy(e, s, 2);
  EXPECT_STREQ(e, s);
  string_copy(e, s, 3);
  EXPECT_STREQ(e, s);
  e = "123456789";
  EXPECT_DEATH(string_copy(e, s, 8), "max_len 8 but string length 9");
}

TEST(String, Break) {
  char *s = null;

  EXPECT_DEATH(
      string_new_copy_wrap(&s, "Hi", 0, 0, 2),
      "Width cannot be zero");
  string_delete(&s);
  EXPECT_DEATH(
      string_new_copy_wrap(&s, "Hi", 11, 10, 2),
      "Start 11 not valid for width 10");
  string_delete(&s);
  EXPECT_DEATH(
      string_new_copy_wrap(&s, "Hi", 10, 10, 2),
      "Start 10 not valid for width 10");
  EXPECT_DEATH(
      string_new_copy_wrap(&s, "Hi", 0, 10, 11),
      "Indent 11 not valid for width 10");
  EXPECT_DEATH(
      string_new_copy_wrap(&s, "Hi", 0, 10, 10),
      "Indent 10 not valid for width 10");
  EXPECT_DEATH(
      string_new_copy_wrap(&s, "Hi\n", 0, 10, 0),
      "Cannot wrap strings with newlines");
  EXPECT_DEATH(
      string_new_copy_wrap(&s, "\nHi", 0, 10, 0),
      "Cannot wrap strings with newlines");
  EXPECT_DEATH(
      string_new_copy_wrap(&s, "Hi\nthere", 0, 10, 0),
      "Cannot wrap strings with newlines");
  string_delete(&s);
}

FINISH_DEATH

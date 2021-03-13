#include "strng_test.h"

START

TEST(String, NewEmpty) {
  char *s = null;

  EXPECT_STREQ("", string_new_empty(&s));
  EXPECT_STREQ("", s);
  string_delete(&s);
}

TEST(String, NewChar) {
  char *s = null;

  EXPECT_STREQ("p", string_new_char(&s, 'p'));
  EXPECT_STREQ("p", s);
  string_delete(&s);
}

TEST(String, NewCopy) {
  char orig[] = "Hi there";
  char *copy = null;
  char *local;

  EXPECT_STREQ(orig, string_new_copy(&copy, orig));
  EXPECT_STREQ(orig, copy);
  local = copy;
  string_delete(&copy);
}

TEST(String, NewF) {
  char *s = null;
  char *local;
  char *e = "42 j 3.1";
  int n;

  /* Test random combinations just to make sure nothing obvious sticks out. The
   * maximum length is tested comprehensively below.
   */
  local = string_new_f(&s, "%d %c %.1f", 42, 'j', 3.14159);
  EXPECT_STREQ(e, s);
  EXPECT_STREQ(e, local);
  string_delete(&s);
  e = "A42-173!1eb9=F1206-123ddJsswoofd12.300000!2.409197e+071.456700E-07"
    "-1.23457e+08+10.2345.%";
  EXPECT_STREQ(
      e,
      string_new_f(
        &s,
        "A%d-%o!%x=%X-%udd%css%sd%f!%e%E-%g+%G.%n%%",
        42,
        123,
        7865,
        987654,
        123,
        'J',
        "woof",
        12.3,
        24091966.0,
        0.00000014567,
        123456789.0,
        10.234458787,
        &n));
  EXPECT_ULEQ(87, n);
  EXPECT_STREQ(e, s);
  string_delete(&s);
  e = "123442 |+000024| 1|00000000499602d2|     3.140";
  EXPECT_STREQ(
      e,
      string_new_f(
        &s,
        "%-7.2d|%+.6d|% d|%016lx|%10.3f",
        123442,
        24,
        1,
        1234567890,
        3.14));
  EXPECT_STREQ(e, s);
  string_delete(&s);
}

TEST(String, NewConcat) {
  char *s = null;
  char a[] = "Hi there";
  char b[] = "big spender";
  char e[] = "Hi there - big spender!";
  char *c = null;
  char *ss[4];

  new(&c, 2);
  c[0] = '!';
  c[1] = '\0';
  EXPECT_STREQ(e, string_new_concat_until_null(&s, a, " - ", b, c, null));
  EXPECT_STREQ(e, s);
  string_delete(&s);
  EXPECT_STREQ("", string_new_concat_until_null(&s, null));
  EXPECT_STREQ("", s);
  string_delete(&s);

  ss[0] = a;
  ss[1] = " - ";
  ss[2] = b;
  ss[3] = c;
  string_new_concat_array(&s, 4, ss);
  EXPECT_STREQ(e, s);
  string_delete(&s);

  string_new_concat(&s, "cat", "dog");
  EXPECT_STREQ("catdog", s);
  string_delete(&s);

  string_delete(&c);
}

TEST(String, NewTruncate) {
  char *s = null;

  EXPECT_STREQ("cat", string_new_truncate(&s, "catastrophe", 3));
  EXPECT_STREQ("cat", s);
  string_delete(&s);
  EXPECT_STREQ("dog", string_new_truncate(&s, "dog", 3));
  EXPECT_STREQ("dog", s);
  string_delete(&s);
  EXPECT_STREQ("", string_new_truncate(&s, "gold", 0));
  EXPECT_STREQ("", s);
  string_delete(&s);
  EXPECT_STREQ("", string_new_truncate(&s, "", 0));
  EXPECT_STREQ("", s);
  string_delete(&s);
}

TEST(String, NewShorten) {
  char *s = null;

  EXPECT_STREQ("cat", string_new_shorten(&s, "catastrophe", 8));
  EXPECT_STREQ("cat", s);
  string_delete(&s);
  EXPECT_STREQ("dog", string_new_shorten(&s, "dog", 0));
  EXPECT_STREQ("dog", s);
  string_delete(&s);
  EXPECT_STREQ("", string_new_shorten(&s, "gold", 4));
  EXPECT_STREQ("", s);
  string_delete(&s);
  EXPECT_STREQ("", string_new_shorten(&s, "", 0));
  EXPECT_STREQ("", s);
  string_delete(&s);
}

TEST(String, NewIndent) {
  char *s1 = null;
  char *s2 = null;
  char *s3 = null;

  EXPECT_STREQ("", string_new_indent(&s1, "", 0));
  EXPECT_STREQ("", s1);
  EXPECT_STREQ(" ", string_new_indent(&s2, s1, 1));
  EXPECT_STREQ(" ", s2);
  EXPECT_STREQ("   ", string_new_indent(&s3, s2, 2));
  EXPECT_STREQ("   ", s3);
  string_delete(&s3);
  string_delete(&s2);
  string_delete(&s1);
  EXPECT_STREQ("a", string_new_indent(&s1, "a", 0));
  EXPECT_STREQ("a", s1);
  EXPECT_STREQ(" a", string_new_indent(&s2, s1, 1));
  EXPECT_STREQ(" a", s2);
  EXPECT_STREQ("   a", string_new_indent(&s3, s2, 2));
  EXPECT_STREQ("   a", s3);
  string_delete(&s3);
  string_delete(&s2);
  string_delete(&s1);
  EXPECT_STREQ("ab", string_new_indent(&s1, "ab", 0));
  EXPECT_STREQ("ab", s1);
  EXPECT_STREQ(" ab", string_new_indent(&s2, s1, 1));
  EXPECT_STREQ(" ab", s2);
  EXPECT_STREQ("   ab", string_new_indent(&s3, s2, 2));
  EXPECT_STREQ("   ab", s3);
  string_delete(&s3);
  string_delete(&s2);
  string_delete(&s1);
  EXPECT_STREQ("\n", string_new_indent(&s1, "\n", 0));
  EXPECT_STREQ("\n", s1);
  EXPECT_STREQ(" \n ", string_new_indent(&s2, s1, 1));
  EXPECT_STREQ(" \n ", s2);
  EXPECT_STREQ("   \n   ", string_new_indent(&s3, s2, 2));
  EXPECT_STREQ("   \n   ", s3);
  string_delete(&s3);
  string_delete(&s2);
  string_delete(&s1);
  EXPECT_STREQ("a\n", string_new_indent(&s1, "a\n", 0));
  EXPECT_STREQ("a\n", s1);
  EXPECT_STREQ(" a\n ", string_new_indent(&s2, s1, 1));
  EXPECT_STREQ(" a\n ", s2);
  EXPECT_STREQ("   a\n   ", string_new_indent(&s3, s2, 2));
  EXPECT_STREQ("   a\n   ", s3);
  string_delete(&s3);
  string_delete(&s2);
  string_delete(&s1);
  EXPECT_STREQ("a\nb", string_new_indent(&s1, "a\nb", 0));
  EXPECT_STREQ("a\nb", s1);
  EXPECT_STREQ(" a\n b", string_new_indent(&s2, s1, 1));
  EXPECT_STREQ(" a\n b", s2);
  EXPECT_STREQ("   a\n   b", string_new_indent(&s3, s2, 2));
  EXPECT_STREQ("   a\n   b", s3);
  string_delete(&s3);
  string_delete(&s2);
  string_delete(&s1);
  EXPECT_STREQ("a\nb\nc", string_new_indent(&s1, "a\nb\nc", 0));
  EXPECT_STREQ("a\nb\nc", s1);
  EXPECT_STREQ(" a\n b\n c", string_new_indent(&s2, s1, 1));
  EXPECT_STREQ(" a\n b\n c", s2);
  EXPECT_STREQ("   a\n   b\n   c", string_new_indent(&s3, s2, 2));
  EXPECT_STREQ("   a\n   b\n   c", s3);
  string_delete(&s3);
  string_delete(&s2);
  string_delete(&s1);
}

TEST(String, NewFilenameFromPath) {
  char *s = null;

  EXPECT_STREQ(
      "file.txt",
      string_new_filename_from_path(&s, "/path/to/file.txt"));
  EXPECT_STREQ("file.txt", s);
  string_delete(&s);
  EXPECT_STREQ(
      "file.txt",
      string_new_filename_from_path(&s, "file.txt"));
  EXPECT_STREQ("file.txt", s);
  string_delete(&s);
  EXPECT_STREQ(
      "file",
      string_new_filename_from_path(&s, "relative/path/to//file"));
  EXPECT_STREQ("file", s);
  string_delete(&s);
  EXPECT_STREQ(
      "file",
      string_new_filename_from_path(&s, "/file"));
  EXPECT_STREQ("file", s);
  string_delete(&s);
  EXPECT_STREQ(
      "",
      string_new_filename_from_path(&s, "no/filename/here/"));
  EXPECT_STREQ("", s);
  string_delete(&s);
}

TEST(String, RenewStandardizeDirectory) {
  char *s = null;

  string_new_copy(&s, "");
  EXPECT_STREQ("", string_renew_standardize_directory(&s));
  string_delete(&s);

  string_new_copy(&s, "/");
  EXPECT_STREQ("/", string_renew_standardize_directory(&s));
  string_delete(&s);

  string_new_copy(&s, "//");
  EXPECT_STREQ("/", string_renew_standardize_directory(&s));
  string_delete(&s);

  string_new_copy(&s, "path");
  EXPECT_STREQ("path", string_renew_standardize_directory(&s));
  string_delete(&s);

  string_new_copy(&s, "path/");
  EXPECT_STREQ("path", string_renew_standardize_directory(&s));
  string_delete(&s);

  string_new_copy(&s, "path//");
  EXPECT_STREQ("path", string_renew_standardize_directory(&s));
  string_delete(&s);

  string_new_copy(&s, "/path");
  EXPECT_STREQ("/path", string_renew_standardize_directory(&s));
  string_delete(&s);

  string_new_copy(&s, "//path");
  EXPECT_STREQ("/path", string_renew_standardize_directory(&s));
  string_delete(&s);

  string_new_copy(&s, "//path/");
  EXPECT_STREQ("/path", string_renew_standardize_directory(&s));
  string_delete(&s);

  string_new_copy(&s, "//path////");
  EXPECT_STREQ("/path", string_renew_standardize_directory(&s));
  string_delete(&s);

  string_new_copy(&s, "/path/to");
  EXPECT_STREQ("/path/to", string_renew_standardize_directory(&s));
  string_delete(&s);

  string_new_copy(&s, "/path/to/");
  EXPECT_STREQ("/path/to", string_renew_standardize_directory(&s));
  string_delete(&s);

  string_new_copy(&s, "//path//to///");
  EXPECT_STREQ("/path/to", string_renew_standardize_directory(&s));
  string_delete(&s);
}

TEST(String, PathToDirectoryFilename) {
  char *directory = null;
  char *filename = null;

  string_path_to_directory_filename("/path/to/file.txt", &directory, &filename);
  EXPECT_STREQ("/path/to", directory);
  EXPECT_STREQ("file.txt", filename);
  string_delete(&filename);
  string_delete(&directory);

  string_path_to_directory_filename("file.txt", &directory, &filename);
  EXPECT_STREQ("", directory);
  EXPECT_STREQ("file.txt", filename);
  string_delete(&filename);
  string_delete(&directory);

  string_path_to_directory_filename(
      "relative/path/to/file",
      &directory,
      &filename);
  EXPECT_STREQ("relative/path/to", directory);
  EXPECT_STREQ("file", filename);
  string_delete(&filename);
  string_delete(&directory);

  string_path_to_directory_filename(
      "ignore//multiple///slashes////file",
      &directory,
      &filename);
  EXPECT_STREQ("ignore/multiple/slashes", directory);
  EXPECT_STREQ("file", filename);
  string_delete(&filename);
  string_delete(&directory);

  string_path_to_directory_filename("/file", &directory, &filename);
  EXPECT_STREQ("/", directory);
  EXPECT_STREQ("file", filename);
  string_delete(&filename);
  string_delete(&directory);

  string_path_to_directory_filename("no/filename/here/", &directory, &filename);
  EXPECT_STREQ("no/filename/here", directory);
  EXPECT_STREQ("", filename);
  string_delete(&filename);
  string_delete(&directory);

  string_path_to_directory_filename("or/here///", &directory, &filename);
  EXPECT_STREQ("or/here", directory);
  EXPECT_STREQ("", filename);
  string_delete(&filename);
  string_delete(&directory);
}

TEST(String, NewSubstring) {
  char *s = null;

  EXPECT_STREQ("", string_new_substring(&s, "", 0, 0));
  EXPECT_STREQ("", s);
  string_delete(&s);
  EXPECT_STREQ("", string_new_substring(&s, "a", 0, 0));
  EXPECT_STREQ("", s);
  string_delete(&s);
  EXPECT_STREQ("", string_new_substring(&s, "a", 1, 0));
  EXPECT_STREQ("", s);
  string_delete(&s);
  EXPECT_STREQ("", string_new_substring(&s, "ab", 0, 0));
  EXPECT_STREQ("", s);
  string_delete(&s);
  EXPECT_STREQ("", string_new_substring(&s, "ab", 1, 0));
  EXPECT_STREQ("", s);
  string_delete(&s);
  EXPECT_STREQ("", string_new_substring(&s, "ab", 2, 0));
  EXPECT_STREQ("", s);
  string_delete(&s);
  EXPECT_STREQ("a", string_new_substring(&s, "a", 0, 1));
  EXPECT_STREQ("a", s);
  string_delete(&s);
  EXPECT_STREQ("a", string_new_substring(&s, "ab", 0, 1));
  EXPECT_STREQ("a", s);
  string_delete(&s);
  EXPECT_STREQ("b", string_new_substring(&s, "ab", 1, 1));
  EXPECT_STREQ("b", s);
  string_delete(&s);
  EXPECT_STREQ("ab", string_new_substring(&s, "ab", 0, 2));
  EXPECT_STREQ("ab", s);
  string_delete(&s);
}

TEST(String, NewJoin) {
  char *a[] = {"cat", "dog", "pig"};
  char *s = null;

  EXPECT_STREQ("", string_new_join(&s, 0, null, null));
  string_delete(&s);
  EXPECT_STREQ("cat", string_new_join(&s, 1, a, null));
  string_delete(&s);
  EXPECT_STREQ("cat & dog", string_new_join(&s, 2, a, " & "));
  string_delete(&s);
  EXPECT_STREQ("cat and dog and pig", string_new_join(&s, 3, a, " and "));
  string_delete(&s);
}

TEST(String, NewEscape) {
  char *s = null;

  EXPECT_STREQ("", string_new_escape(&s, ""));
  string_delete(&s);
  EXPECT_STREQ("a", string_new_escape(&s, "a"));
  string_delete(&s);
  EXPECT_STREQ("ab", string_new_escape(&s, "ab"));
  string_delete(&s);
  EXPECT_STREQ("abc", string_new_escape(&s, "abc"));
  string_delete(&s);
  EXPECT_STREQ("a\\nbc", string_new_escape(&s, "a\nbc"));
  string_delete(&s);
  EXPECT_STREQ(
      "a\\nb\\ac\\b\\f\\r\\t\\v\\\\\\'\\\"",
      string_new_escape(&s, "a\nb\ac\b\f\r\t\v\\'\""));
  string_delete(&s);
  EXPECT_STREQ(
      "\\x01\\x02\\x1f\\x7f\\x80\\x81\\xfe\\xff",
      string_new_escape(&s, "\x01\x02\x1f\x7f\x80\x81\xfe\xff"));
  string_delete(&s);
}

TEST(String, RenewConcat) {
  char *s = null;
  char a[] = "mate";
  char e[] = "Fair dinkum, mate";

  string_new_copy(&s, "Fair dinkum");
  EXPECT_STREQ(e, string_renew_concat_until_null(&s, ", ", "", a, "", null));
  EXPECT_STREQ(e, s);
  EXPECT_STREQ(e, string_renew_concat_until_null(&s, null));
  EXPECT_STREQ(e, s);
  string_delete(&s);
}

TEST(String, RenewConcatChar) {
  char e[] = "Fair dinkum!";
  char *s = null;

  string_new_copy(&s, "Fair dinkum");
  EXPECT_STREQ(e, string_renew_concat_char(&s, '!'));
  EXPECT_STREQ(e, s);
  string_delete(&s);
}

TEST(String, RenewConcatF) {
  char e[] = "Fair dinkum 42 j 3.1";
  char *s = null;

  string_new_copy(&s, "Fair dinkum");
  EXPECT_STREQ(
      e,
      string_renew_concat_f(&s, " %d %c %.1f", 42, 'j', 3.14159));
  EXPECT_STREQ(e, s);
  string_delete(&s);
}

TEST(String, RenewPrepend) {
  char *s = null;

  string_new_copy(&s, "");
  string_renew_prepend(&s, "");
  EXPECT_STREQ("", s);
  string_renew_prepend(&s, "a");
  EXPECT_STREQ("a", s);
  string_renew_prepend(&s, "");
  EXPECT_STREQ("a", s);
  string_renew_prepend(&s, "b");
  EXPECT_STREQ("ba", s);
  string_renew_prepend(&s, "");
  EXPECT_STREQ("ba", s);
  string_renew_prepend(&s, "c");
  EXPECT_STREQ("cba", s);
  string_renew_prepend(&s, "de");
  EXPECT_STREQ("decba", s);
  string_renew_prepend(&s, "fgh");
  EXPECT_STREQ("fghdecba", s);
  string_delete(&s);
}

TEST(String, RenewEnsureEndsWith) {
  char *e;
  char *s = null;

  e = "";
  string_new_copy(&s, "");
  EXPECT_STREQ(e, string_renew_ensure_ends_with(&s, ""));
  EXPECT_STREQ(e, s);
  e = "a";
  EXPECT_STREQ(e, string_renew_ensure_ends_with(&s, "a"));
  EXPECT_STREQ(e, s);
  EXPECT_STREQ(e, string_renew_ensure_ends_with(&s, "a"));
  EXPECT_STREQ(e, s);
  e = "ab";
  EXPECT_STREQ(e, string_renew_ensure_ends_with(&s, "b"));
  EXPECT_STREQ(e, s);
  e = "ab";
  EXPECT_STREQ(e, string_renew_ensure_ends_with(&s, "b"));
  EXPECT_STREQ(e, s);
  EXPECT_STREQ(e, string_renew_ensure_ends_with(&s, "ab"));
  EXPECT_STREQ(e, s);
  e = "abba";
  EXPECT_STREQ(e, string_renew_ensure_ends_with(&s, "ba"));
  EXPECT_STREQ(e, s);
  EXPECT_STREQ(e, string_renew_ensure_ends_with(&s, "ba"));
  EXPECT_STREQ(e, s);
  e = "abbadog";
  EXPECT_STREQ(e, string_renew_ensure_ends_with(&s, "dog"));
  EXPECT_STREQ(e, s);
  EXPECT_STREQ(e, string_renew_ensure_ends_with(&s, "dog"));
  EXPECT_STREQ(e, s);
  e = "abbadog/";
  EXPECT_STREQ(e, string_renew_ensure_ends_with(&s, "/"));
  EXPECT_STREQ(e, s);
  EXPECT_STREQ(e, string_renew_ensure_ends_with(&s, "/"));
  EXPECT_STREQ(e, s);
  string_delete(&s);
}

TEST(String, RenewShorten) {
  char *s = null;

  string_new_copy(&s, "dog");
  EXPECT_STREQ("dog", string_renew_shorten(&s, 0));
  EXPECT_STREQ("do", string_renew_shorten(&s, 1));
  EXPECT_STREQ("", string_renew_shorten(&s, 2));
  EXPECT_STREQ("", string_renew_shorten(&s, 0));
  string_delete(&s);
}


TEST(String, MaxLen) {
  int n;

  /* Empty.
   */
  EXPECT_ULEQ(0, string_f_get_max_len(""));

  /* Simple characters.
   */
  EXPECT_ULEQ(1, string_f_get_max_len("a"));
  EXPECT_ULEQ(2, string_f_get_max_len("ad"));
  EXPECT_ULEQ(3, string_f_get_max_len("ald"));

  /* %.
   */
  EXPECT_ULEQ(1, string_f_get_max_len("%%"));
  EXPECT_ULEQ(2, string_f_get_max_len("%%%%"));
  EXPECT_ULEQ(2, string_f_get_max_len("a%%"));
  EXPECT_ULEQ(2, string_f_get_max_len("%%f"));
  EXPECT_ULEQ(3, string_f_get_max_len("%%%%%%"));
  EXPECT_ULEQ(3, string_f_get_max_len("%%d%%"));
  EXPECT_ULEQ(3, string_f_get_max_len("d%%%%"));
  EXPECT_ULEQ(3, string_f_get_max_len("%%%%d"));

  /* %n.
   */
  EXPECT_ULEQ(0, string_f_get_max_len("%n"));
  EXPECT_ULEQ(0, string_f_get_max_len("%n%n"));
  EXPECT_ULEQ(1, string_f_get_max_len("%na%n"));

  /* Specifying minimum width.
   */
  EXPECT_ULEQ(17, string_f_get_max_len("%17d", 0));
  EXPECT_ULEQ(17, string_f_get_max_len("%17i", 0));
  EXPECT_ULEQ(99, string_f_get_max_len("%99c", 'x'));
  EXPECT_ULEQ(123, string_f_get_max_len("%123x", 0));
  EXPECT_ULEQ(123, string_f_get_max_len("%123X", 0));
  EXPECT_ULEQ(123, string_f_get_max_len("%123o", 0));
  EXPECT_ULEQ(1234, string_f_get_max_len("%1234u", 0));
  EXPECT_ULEQ(2, string_f_get_max_len("%2s", ""));
  EXPECT_ULEQ(15, string_f_get_max_len("%15f", 0));
  EXPECT_ULEQ(15, string_f_get_max_len("%15e", 0));
  EXPECT_ULEQ(15, string_f_get_max_len("%15E", 0));
  EXPECT_ULEQ(15, string_f_get_max_len("%15g", 0));
  EXPECT_ULEQ(15, string_f_get_max_len("%15G", 0));
  EXPECT_ULEQ(40, string_f_get_max_len("%40p", null));
  EXPECT_ULEQ(0, string_f_get_max_len("%20n", &n));

  /* Check defaults.
   */
  EXPECT_ULEQ(6, string_f_get_max_len("%hd", 0));
  EXPECT_ULEQ(11, string_f_get_max_len("%d", 0));
  EXPECT_ULEQ(21, string_f_get_max_len("%ld", 0));
  EXPECT_ULEQ(6, string_f_get_max_len("%hi", 0));
  EXPECT_ULEQ(11, string_f_get_max_len("%i", 0));
  EXPECT_ULEQ(21, string_f_get_max_len("%li", 0));
  EXPECT_ULEQ(6, string_f_get_max_len("%hu", 0));
  EXPECT_ULEQ(11, string_f_get_max_len("%u", 0));
  EXPECT_ULEQ(21, string_f_get_max_len("%lu", 0));
  EXPECT_ULEQ(7, string_f_get_max_len("%ho", 0));
  EXPECT_ULEQ(12, string_f_get_max_len("%o", 0));
  EXPECT_ULEQ(23, string_f_get_max_len("%lo", 0));
  EXPECT_ULEQ(8, string_f_get_max_len("%#ho", 0));
  EXPECT_ULEQ(13, string_f_get_max_len("%#o", 0));
  EXPECT_ULEQ(24, string_f_get_max_len("%#lo", 0));
  EXPECT_ULEQ(4, string_f_get_max_len("%hx", 0));
  EXPECT_ULEQ(8, string_f_get_max_len("%x", 0));
  EXPECT_ULEQ(16, string_f_get_max_len("%lx", 0));
  EXPECT_ULEQ(16, string_f_get_max_len("%lX", 0));
  EXPECT_ULEQ(6, string_f_get_max_len("%#hx", 0));
  EXPECT_ULEQ(10, string_f_get_max_len("%#x", 0));
  EXPECT_ULEQ(18, string_f_get_max_len("%#lx", 0));
  EXPECT_ULEQ(18, string_f_get_max_len("%#lX", 0));
  EXPECT_ULEQ(1, string_f_get_max_len("%c", 0));
  EXPECT_ULEQ(5, string_f_get_max_len("%s", "bingo"));
  EXPECT_ULEQ(9, string_f_get_max_len("%f", 0.0));
  EXPECT_ULEQ(9, string_f_get_max_len("%f", 0.1));
  EXPECT_ULEQ(9, string_f_get_max_len("%f", 1.0));
  EXPECT_ULEQ(9, string_f_get_max_len("%f", 9.999));
  EXPECT_ULEQ(10, string_f_get_max_len("%f", 10.0));
  EXPECT_ULEQ(10, string_f_get_max_len("%f", 99.9));
  EXPECT_ULEQ(10, string_f_get_max_len("%f", -99.9));
  EXPECT_ULEQ(11, string_f_get_max_len("%f", 100.0));
  EXPECT_ULEQ(11, string_f_get_max_len("%f", -999.9));
  EXPECT_ULEQ(309, string_f_get_max_len("%f", 1e300));
  EXPECT_ULEQ(9, string_f_get_max_len("%Lf", 0.0L));
  EXPECT_ULEQ(9, string_f_get_max_len("%Lf", 0.1L));
  EXPECT_ULEQ(9, string_f_get_max_len("%Lf", 1.0L));
  EXPECT_ULEQ(9, string_f_get_max_len("%Lf", 9.999L));
  EXPECT_ULEQ(10, string_f_get_max_len("%Lf", 10.0L));
  EXPECT_ULEQ(10, string_f_get_max_len("%Lf", 99.9L));
  EXPECT_ULEQ(10, string_f_get_max_len("%Lf", -99.9L));
  EXPECT_ULEQ(11, string_f_get_max_len("%Lf", 100.0L));
  EXPECT_ULEQ(11, string_f_get_max_len("%Lf", -999.9L));
  EXPECT_ULEQ(309, string_f_get_max_len("%Lf", 1e300L));
  EXPECT_ULEQ(15, string_f_get_max_len("%e", 0.0));
  EXPECT_ULEQ(15, string_f_get_max_len("%E", 0.0));
  EXPECT_ULEQ(15, string_f_get_max_len("%Le", 0.0L));
  EXPECT_ULEQ(15, string_f_get_max_len("%LE", 0.0L));
  EXPECT_ULEQ(9, string_f_get_max_len("%g", 0.0));
  EXPECT_ULEQ(9, string_f_get_max_len("%g", 0.1));
  EXPECT_ULEQ(9, string_f_get_max_len("%g", 0.01));
  EXPECT_ULEQ(10, string_f_get_max_len("%g", 0.001));
  EXPECT_ULEQ(11, string_f_get_max_len("%g", 0.0001));
  EXPECT_ULEQ(15, string_f_get_max_len("%g", -0.0000999));
  EXPECT_ULEQ(15, string_f_get_max_len("%g", 0.00001));
  EXPECT_ULEQ(15, string_f_get_max_len("%g", 1234567.0));
  EXPECT_ULEQ(14, string_f_get_max_len("%g", -123456.0));
  EXPECT_ULEQ(13, string_f_get_max_len("%g", 12345.0));
  EXPECT_ULEQ(12, string_f_get_max_len("%g", 1234.0));
  EXPECT_ULEQ(15, string_f_get_max_len("%g", 12345678.0));
  EXPECT_ULEQ(15, string_f_get_max_len("%g", 123456789.0));
  EXPECT_ULEQ(32, string_f_get_max_len("%p", null));

  /* Set precision.
   */
  EXPECT_ULEQ(1, string_f_get_max_len("%.1s", "doggie"));
  EXPECT_ULEQ(2, string_f_get_max_len("%.2s", "doggie"));
  EXPECT_ULEQ(6, string_f_get_max_len("%.9s", "doggie"));
  EXPECT_ULEQ(12, string_f_get_max_len("%.9f", 1.4));
  EXPECT_ULEQ(3, string_f_get_max_len("%.0f", 1.4));
  EXPECT_ULEQ(13, string_f_get_max_len("%.10f", 1.4));
  EXPECT_ULEQ(14, string_f_get_max_len("%.10f", -19.4));

  /* *.
   */
  EXPECT_ULEQ(6, string_f_get_max_len("%*s", 3, "doggie"));
  EXPECT_ULEQ(10, string_f_get_max_len("%*s", 10, "doggie"));
  EXPECT_ULEQ(3, string_f_get_max_len("%.*s", 3, "doggie"));
  EXPECT_ULEQ(6, string_f_get_max_len("%.*s", 10, "doggie"));
}

TEST(String, StartsWith) {
  EXPECT_TRUE(string_starts_with("", ""));
  EXPECT_TRUE(string_starts_with("a", ""));
  EXPECT_TRUE(string_starts_with("ab", ""));
  EXPECT_TRUE(string_starts_with("a", "a"));
  EXPECT_TRUE(string_starts_with("ab", "a"));
  EXPECT_TRUE(string_starts_with("abc", "a"));
  EXPECT_TRUE(string_starts_with("ab", "ab"));
  EXPECT_TRUE(string_starts_with("abc", "ab"));
  EXPECT_TRUE(string_starts_with("abcd", "ab"));
  EXPECT_FALSE(string_starts_with("", "a"));
  EXPECT_FALSE(string_starts_with("", "ab"));
  EXPECT_FALSE(string_starts_with("", "abc"));
  EXPECT_FALSE(string_starts_with("a", "ab"));
  EXPECT_FALSE(string_starts_with("a", "abc"));
  EXPECT_FALSE(string_starts_with("ab", "abc"));
  EXPECT_FALSE(string_starts_with("a", "b"));
  EXPECT_FALSE(string_starts_with("ab", "ba"));
  EXPECT_FALSE(string_starts_with("abc", "acb"));
}

TEST(String, EndsWith) {
  EXPECT_TRUE(string_ends_with("", ""));
  EXPECT_TRUE(string_ends_with("a", ""));
  EXPECT_TRUE(string_ends_with("ab", ""));
  EXPECT_TRUE(string_ends_with("a", "a"));
  EXPECT_TRUE(string_ends_with("ba", "a"));
  EXPECT_TRUE(string_ends_with("cba", "a"));
  EXPECT_TRUE(string_ends_with("ba", "ba"));
  EXPECT_TRUE(string_ends_with("cba", "ba"));
  EXPECT_TRUE(string_ends_with("dcba", "ba"));
  EXPECT_FALSE(string_ends_with("", "a"));
  EXPECT_FALSE(string_ends_with("", "ba"));
  EXPECT_FALSE(string_ends_with("", "cba"));
  EXPECT_FALSE(string_ends_with("a", "ba"));
  EXPECT_FALSE(string_ends_with("a", "cba"));
  EXPECT_FALSE(string_ends_with("ba", "cba"));
  EXPECT_FALSE(string_starts_with("a", "b"));
  EXPECT_FALSE(string_starts_with("ba", "ab"));
  EXPECT_FALSE(string_starts_with("cba", "cab"));
}

TEST(String, FirstIndexOf) {
  /* Simple cases.
   */
  EXPECT_ULEQ(-1, string_first_index_of("", 'a'));
  EXPECT_ULEQ(0, string_first_index_of("a", 'a'));
  EXPECT_ULEQ(-1, string_first_index_of("a", 'b'));
  EXPECT_ULEQ(0, string_first_index_of("as", 'a'));
  EXPECT_ULEQ(1, string_first_index_of("as", 's'));
  EXPECT_ULEQ(-1, string_first_index_of("as", 'b'));
  EXPECT_ULEQ(0, string_first_index_of("dog", 'd'));
  EXPECT_ULEQ(1, string_first_index_of("dog", 'o'));
  EXPECT_ULEQ(2, string_first_index_of("dog", 'g'));
  EXPECT_ULEQ(-1, string_first_index_of("dog", 'b'));

  /* More testing cases.
   */
  EXPECT_ULEQ(0, string_first_index_of("dad", 'd'));
  EXPECT_ULEQ(1, string_first_index_of("appear", 'p'));
  EXPECT_ULEQ(2, string_first_index_of("batt", 't'));
  EXPECT_ULEQ(1, string_first_index_of("Mississippi", 'i'));
}

TEST(String, NthIndexOf) {
  EXPECT_ULEQ(-1, string_nth_index_of("", 0, 'a'));
  EXPECT_ULEQ(-1, string_nth_index_of("", 1, 'a'));
  EXPECT_ULEQ(-1, string_nth_index_of("", 2, 'a'));
  EXPECT_ULEQ(0, string_nth_index_of("a", 0, 'a'));
  EXPECT_ULEQ(-1, string_nth_index_of("a", 1, 'a'));
  EXPECT_ULEQ(-1, string_nth_index_of("a", 2, 'a'));
  EXPECT_ULEQ(-1, string_nth_index_of("a", 0, 'b'));
  EXPECT_ULEQ(-1, string_nth_index_of("a", 1, 'b'));
  EXPECT_ULEQ(-1, string_nth_index_of("a", 2, 'b'));
  EXPECT_ULEQ(0, string_nth_index_of("as", 0, 'a'));
  EXPECT_ULEQ(-1, string_nth_index_of("as", 1, 'a'));
  EXPECT_ULEQ(-1, string_nth_index_of("as", 2, 'a'));
  EXPECT_ULEQ(1, string_nth_index_of("as", 0, 's'));
  EXPECT_ULEQ(-1, string_nth_index_of("as", 1, 's'));
  EXPECT_ULEQ(-1, string_nth_index_of("as", 2, 's'));
  EXPECT_ULEQ(0, string_nth_index_of("dad", 0, 'd'));
  EXPECT_ULEQ(2, string_nth_index_of("dad", 1, 'd'));
  EXPECT_ULEQ(-1, string_nth_index_of("dad", 2, 'd'));
  EXPECT_ULEQ(1, string_nth_index_of("appear", 0, 'p'));
  EXPECT_ULEQ(2, string_nth_index_of("appear", 1, 'p'));
  EXPECT_ULEQ(-1, string_nth_index_of("appear", 2, 'p'));
  EXPECT_ULEQ(2, string_nth_index_of("batt", 0, 't'));
  EXPECT_ULEQ(3, string_nth_index_of("batt", 1, 't'));
  EXPECT_ULEQ(1, string_nth_index_of("Mississippi", 0, 'i'));
  EXPECT_ULEQ(4, string_nth_index_of("Mississippi", 1, 'i'));
  EXPECT_ULEQ(7, string_nth_index_of("Mississippi", 2, 'i'));
  EXPECT_ULEQ(10, string_nth_index_of("Mississippi", 3, 'i'));
  EXPECT_ULEQ(-1, string_nth_index_of("Mississippi", 4, 'i'));
}

TEST(String, LastIndexOf) {
  /* Simple cases.
   */
  EXPECT_ULEQ(-1, string_last_index_of("", 'a'));
  EXPECT_ULEQ(0, string_last_index_of("a", 'a'));
  EXPECT_ULEQ(-1, string_last_index_of("a", 'b'));
  EXPECT_ULEQ(0, string_last_index_of("as", 'a'));
  EXPECT_ULEQ(1, string_last_index_of("as", 's'));
  EXPECT_ULEQ(-1, string_last_index_of("as", 'b'));
  EXPECT_ULEQ(0, string_last_index_of("dog", 'd'));
  EXPECT_ULEQ(1, string_last_index_of("dog", 'o'));
  EXPECT_ULEQ(2, string_last_index_of("dog", 'g'));
  EXPECT_ULEQ(-1, string_last_index_of("dog", 'b'));

  /* More testing cases.
   */
  EXPECT_ULEQ(2, string_last_index_of("dad", 'd'));
  EXPECT_ULEQ(2, string_last_index_of("appear", 'p'));
  EXPECT_ULEQ(3, string_last_index_of("batt", 't'));
  EXPECT_ULEQ(10, string_last_index_of("Mississippi", 'i'));
}

TEST(String, NthLastIndexOf) {
  EXPECT_ULEQ(-1, string_nth_last_index_of("", 0, 'a'));
  EXPECT_ULEQ(-1, string_nth_last_index_of("", 1, 'a'));
  EXPECT_ULEQ(-1, string_nth_last_index_of("", 2, 'a'));
  EXPECT_ULEQ(0, string_nth_last_index_of("a", 0, 'a'));
  EXPECT_ULEQ(-1, string_nth_last_index_of("a", 1, 'a'));
  EXPECT_ULEQ(-1, string_nth_last_index_of("a", 2, 'a'));
  EXPECT_ULEQ(-1, string_nth_last_index_of("a", 0, 'b'));
  EXPECT_ULEQ(-1, string_nth_last_index_of("a", 1, 'b'));
  EXPECT_ULEQ(-1, string_nth_last_index_of("a", 2, 'b'));
  EXPECT_ULEQ(0, string_nth_last_index_of("as", 0, 'a'));
  EXPECT_ULEQ(-1, string_nth_last_index_of("as", 1, 'a'));
  EXPECT_ULEQ(-1, string_nth_last_index_of("as", 2, 'a'));
  EXPECT_ULEQ(1, string_nth_last_index_of("as", 0, 's'));
  EXPECT_ULEQ(-1, string_nth_last_index_of("as", 1, 's'));
  EXPECT_ULEQ(-1, string_nth_last_index_of("as", 2, 's'));
  EXPECT_ULEQ(2, string_nth_last_index_of("dad", 0, 'd'));
  EXPECT_ULEQ(0, string_nth_last_index_of("dad", 1, 'd'));
  EXPECT_ULEQ(-1, string_nth_last_index_of("dad", 2, 'd'));
  EXPECT_ULEQ(2, string_nth_last_index_of("appear", 0, 'p'));
  EXPECT_ULEQ(1, string_nth_last_index_of("appear", 1, 'p'));
  EXPECT_ULEQ(-1, string_nth_last_index_of("appear", 2, 'p'));
  EXPECT_ULEQ(3, string_nth_last_index_of("batt", 0, 't'));
  EXPECT_ULEQ(2, string_nth_last_index_of("batt", 1, 't'));
  EXPECT_ULEQ(10, string_nth_last_index_of("Mississippi", 0, 'i'));
  EXPECT_ULEQ(7, string_nth_last_index_of("Mississippi", 1, 'i'));
  EXPECT_ULEQ(4, string_nth_last_index_of("Mississippi", 2, 'i'));
  EXPECT_ULEQ(1, string_nth_last_index_of("Mississippi", 3, 'i'));
  EXPECT_ULEQ(-1, string_nth_last_index_of("Mississippi", 4, 'i'));
}

TEST(String, Equals) {
  EXPECT_TRUE(string_equals("", ""));
  EXPECT_TRUE(string_equals("a", "a"));
  EXPECT_TRUE(string_equals("ab", "ab"));
  EXPECT_TRUE(string_equals("abc", "abc"));
  EXPECT_FALSE(string_equals("", "a"));
  EXPECT_FALSE(string_equals("", "ab"));
  EXPECT_FALSE(string_equals("", "abc"));
  EXPECT_FALSE(string_equals("a", ""));
  EXPECT_FALSE(string_equals("a", "ab"));
  EXPECT_FALSE(string_equals("a", "abc"));
  EXPECT_FALSE(string_equals("ab", ""));
  EXPECT_FALSE(string_equals("ab", "a"));
  EXPECT_FALSE(string_equals("ab", "abc"));
  EXPECT_FALSE(string_equals("abc", ""));
  EXPECT_FALSE(string_equals("abc", "a"));
  EXPECT_FALSE(string_equals("abc", "ab"));
}

TEST(String, ContainsAt) {
  /* Contains an empty string anywhere, as long as it's within the string.
   */
  EXPECT_TRUE(string_contains_at("", 0, ""));
  EXPECT_FALSE(string_contains_at("", 1, ""));
  EXPECT_TRUE(string_contains_at("a", 0, ""));
  EXPECT_TRUE(string_contains_at("a", 1, ""));
  EXPECT_FALSE(string_contains_at("a", 2, ""));
  EXPECT_TRUE(string_contains_at("ab", 0, ""));
  EXPECT_TRUE(string_contains_at("ab", 1, ""));
  EXPECT_TRUE(string_contains_at("ab", 2, ""));
  EXPECT_FALSE(string_contains_at("ab", 3, ""));

  /* Other is length 1.
   */
  EXPECT_FALSE(string_contains_at("", 0, "a"));
  EXPECT_FALSE(string_contains_at("b", 0, "a"));
  EXPECT_FALSE(string_contains_at("ba", 0, "a"));
  EXPECT_FALSE(string_contains_at("baa", 0, "a"));
  EXPECT_TRUE(string_contains_at("a", 0, "a"));
  EXPECT_TRUE(string_contains_at("ab", 0, "a"));
  EXPECT_TRUE(string_contains_at("abb", 0, "a"));
  EXPECT_FALSE(string_contains_at("", 1, "a"));
  EXPECT_FALSE(string_contains_at("a", 1, "a"));
  EXPECT_FALSE(string_contains_at("ab", 1, "a"));
  EXPECT_FALSE(string_contains_at("aba", 1, "a"));
  EXPECT_FALSE(string_contains_at("abaa", 1, "a"));
  EXPECT_TRUE(string_contains_at("ba", 1, "a"));
  EXPECT_TRUE(string_contains_at("bab", 1, "a"));
  EXPECT_TRUE(string_contains_at("babb", 1, "a"));
  EXPECT_FALSE(string_contains_at("", 2, "a"));
  EXPECT_FALSE(string_contains_at("a", 2, "a"));
  EXPECT_FALSE(string_contains_at("aa", 2, "a"));
  EXPECT_FALSE(string_contains_at("aab", 2, "a"));
  EXPECT_FALSE(string_contains_at("aaba", 2, "a"));
  EXPECT_FALSE(string_contains_at("aabaa", 2, "a"));
  EXPECT_TRUE(string_contains_at("bba", 2, "a"));
  EXPECT_TRUE(string_contains_at("bbab", 2, "a"));
  EXPECT_TRUE(string_contains_at("bbabb", 2, "a"));

  /* Other is length 2.
   */
  EXPECT_FALSE(string_contains_at("", 0, "ab"));
  EXPECT_FALSE(string_contains_at("a", 0, "ab"));
  EXPECT_FALSE(string_contains_at("bb", 0, "ab"));
  EXPECT_FALSE(string_contains_at("ba", 0, "ab"));
  EXPECT_FALSE(string_contains_at("aab", 0, "ab"));
  EXPECT_TRUE(string_contains_at("ab", 0, "ab"));
  EXPECT_TRUE(string_contains_at("aba", 0, "ab"));
  EXPECT_TRUE(string_contains_at("abb", 0, "ab"));
  EXPECT_FALSE(string_contains_at("", 1, "ab"));
  EXPECT_FALSE(string_contains_at("a", 1, "ab"));
  EXPECT_FALSE(string_contains_at("ab", 1, "ab"));
  EXPECT_FALSE(string_contains_at("abb", 1, "ab"));
  EXPECT_TRUE(string_contains_at("aab", 1, "ab"));
  EXPECT_TRUE(string_contains_at("aaba", 1, "ab"));
  EXPECT_TRUE(string_contains_at("aabab", 1, "ab"));

  /* Containing a given character.
   */
  EXPECT_FALSE(string_contains_char("", 'a'));
  EXPECT_FALSE(string_contains_char("b", 'a'));
  EXPECT_FALSE(string_contains_char("bc", 'a'));
  EXPECT_TRUE(string_contains_char("a", 'a'));
  EXPECT_TRUE(string_contains_char("ba", 'a'));
  EXPECT_TRUE(string_contains_char("ab", 'a'));
  EXPECT_TRUE(string_contains_char("bab", 'a'));
  EXPECT_TRUE(string_contains_char("bbab", 'a'));
}

TEST(String, Split) {
  ulong n;
  char **splits = null;

  EXPECT_ULEQ(1, n = string_split("", '.', &splits));
  EXPECT_STREQ("", splits[0]);
  delete(&splits, n, string_delete);
  EXPECT_ULEQ(1, n = string_split("a", '.', &splits));
  EXPECT_STREQ("a", splits[0]);
  delete(&splits, n, string_delete);
  EXPECT_ULEQ(1, n = string_split("ab", '.', &splits));
  EXPECT_STREQ("ab", splits[0]);
  delete(&splits, n, string_delete);
  EXPECT_ULEQ(1, n = string_split("abc", '.', &splits));
  EXPECT_STREQ("abc", splits[0]);
  delete(&splits, n, string_delete);
  EXPECT_ULEQ(2, n = string_split(".", '.', &splits));
  EXPECT_STREQ("", splits[0]);
  EXPECT_STREQ("", splits[1]);
  delete(&splits, n, string_delete);
  EXPECT_ULEQ(2, n = string_split("a.", '.', &splits));
  EXPECT_STREQ("a", splits[0]);
  EXPECT_STREQ("", splits[1]);
  delete(&splits, n, string_delete);
  EXPECT_ULEQ(2, n = string_split(".a", '.', &splits));
  EXPECT_STREQ("", splits[0]);
  EXPECT_STREQ("a", splits[1]);
  delete(&splits, n, string_delete);
  EXPECT_ULEQ(2, n = string_split("a.b", '.', &splits));
  EXPECT_STREQ("a", splits[0]);
  EXPECT_STREQ("b", splits[1]);
  delete(&splits, n, string_delete);
  EXPECT_ULEQ(2, n = string_split("ab.", '.', &splits));
  EXPECT_STREQ("ab", splits[0]);
  EXPECT_STREQ("", splits[1]);
  delete(&splits, n, string_delete);
  EXPECT_ULEQ(2, n = string_split("ab.c", '.', &splits));
  EXPECT_STREQ("ab", splits[0]);
  EXPECT_STREQ("c", splits[1]);
  delete(&splits, n, string_delete);
  EXPECT_ULEQ(2, n = string_split("ab.cd", '.', &splits));
  EXPECT_STREQ("ab", splits[0]);
  EXPECT_STREQ("cd", splits[1]);
  delete(&splits, n, string_delete);
  EXPECT_ULEQ(2, n = string_split("a.bc", '.', &splits));
  EXPECT_STREQ("a", splits[0]);
  EXPECT_STREQ("bc", splits[1]);
  delete(&splits, n, string_delete);
  EXPECT_ULEQ(2, n = string_split(".ab", '.', &splits));
  EXPECT_STREQ("", splits[0]);
  EXPECT_STREQ("ab", splits[1]);
  delete(&splits, n, string_delete);
  EXPECT_ULEQ(3, n = string_split("..", '.', &splits));
  EXPECT_STREQ("", splits[0]);
  EXPECT_STREQ("", splits[1]);
  EXPECT_STREQ("", splits[2]);
  delete(&splits, n, string_delete);
  EXPECT_ULEQ(3, n = string_split("a..", '.', &splits));
  EXPECT_STREQ("a", splits[0]);
  EXPECT_STREQ("", splits[1]);
  EXPECT_STREQ("", splits[2]);
  delete(&splits, n, string_delete);
  EXPECT_ULEQ(3, n = string_split(".a.", '.', &splits));
  EXPECT_STREQ("", splits[0]);
  EXPECT_STREQ("a", splits[1]);
  EXPECT_STREQ("", splits[2]);
  delete(&splits, n, string_delete);
  EXPECT_ULEQ(3, n = string_split("..a", '.', &splits));
  EXPECT_STREQ("", splits[0]);
  EXPECT_STREQ("", splits[1]);
  EXPECT_STREQ("a", splits[2]);
  delete(&splits, n, string_delete);
  EXPECT_ULEQ(3, n = string_split("abc.d.efgh", '.', &splits));
  EXPECT_STREQ("abc", splits[0]);
  EXPECT_STREQ("d", splits[1]);
  EXPECT_STREQ("efgh", splits[2]);
  delete(&splits, n, string_delete);
}

TEST(String, CheckOrSet) {
  char *s = null;

  EXPECT_TRUE(string_check_or_set(&s, "dog"));
  EXPECT_STREQ(s, "dog");
  EXPECT_FALSE(string_check_or_set(&s, "cat"));
  EXPECT_STREQ(s, "dog");
  string_delete(&s);
}

TEST(String, QuoteOrNull) {
  char *s = null;

  EXPECT_STREQ(string_quote_or_null(&s, null), "(null)");
  string_delete(&s);
  EXPECT_STREQ(string_quote_or_null(&s, "dog"), "'dog'");
  string_delete(&s);
}

TEST(String, ReplaceChars) {
  char *s = null;

  string_new_copy(&s, "a:bad/set of\\characters\n");
  EXPECT_STREQ(
      "a|bad|set|of|characters|", string_replace_chars(s, " :/\\\n", '|'));
  string_delete(&s);

  string_new_replace_char_with_string(&s, "", '$', "dog");
  EXPECT_STREQ("", s);
  string_delete(&s);

  string_new_replace_char_with_string(&s, "a", '$', "dog");
  EXPECT_STREQ("a", s);
  string_delete(&s);

  string_new_replace_char_with_string(&s, "ab", '$', "dog");
  EXPECT_STREQ("ab", s);
  string_delete(&s);

  string_new_replace_char_with_string(&s, "$", '$', "dog");
  EXPECT_STREQ("dog", s);
  string_delete(&s);

  string_new_replace_char_with_string(&s, "a$", '$', "dog");
  EXPECT_STREQ("adog", s);
  string_delete(&s);

  string_new_replace_char_with_string(&s, "$a", '$', "dog");
  EXPECT_STREQ("doga", s);
  string_delete(&s);

  string_new_replace_char_with_string(&s, "a$b", '$', "dog");
  EXPECT_STREQ("adogb", s);
  string_delete(&s);

  string_new_replace_char_with_string(&s, "$a$", '$', "dog");
  EXPECT_STREQ("dogadog", s);
  string_delete(&s);

  string_new_replace_char_with_string(&s, "$ab$", '$', "dog");
  EXPECT_STREQ("dogabdog", s);
  string_delete(&s);

  string_new_replace_char_with_string(&s, "a$bc$d", '$', "dog");
  EXPECT_STREQ("adogbcdogd", s);
  string_delete(&s);

  string_new_replace_char_with_string(&s, "$$", '$', "dog");
  EXPECT_STREQ("$", s);
  string_delete(&s);

  string_new_replace_char_with_string(&s, "a$$", '$', "dog");
  EXPECT_STREQ("a$", s);
  string_delete(&s);

  string_new_replace_char_with_string(&s, "$$a", '$', "dog");
  EXPECT_STREQ("$a", s);
  string_delete(&s);
}

TEST(String, Copy) {
  char s[10];
  char *e = "";

  string_copy(e, s, 0);
  EXPECT_STREQ(e, s);
  string_copy(e, s, 1);
  EXPECT_STREQ(e, s);
  e = "a";
  string_copy(e, s, 1);
  EXPECT_STREQ(e, s);
  string_copy(e, s, 2);
  EXPECT_STREQ(e, s);
  e = "ab";
  string_copy(e, s, 2);
  EXPECT_STREQ(e, s);
  string_copy(e, s, 3);
  EXPECT_STREQ(e, s);
  e = "123456789";
  string_copy(e, s, 9);
  EXPECT_STREQ(e, s);
  string_copy(e, s, 9999);
  EXPECT_STREQ(e, s);
}

TEST(String, Wrap) {
  char *orig;
  char *s = null;

  /* Test start, width, and offset, using copy create.
   */
  orig = "This is a long string with alongwordinitman";

  EXPECT_FALSE(string_new_copy_wrap(&s, orig, 0, 80, 0));
  EXPECT_STREQ(orig, s);
  string_delete(&s);

  EXPECT_TRUE(string_new_copy_wrap(&s, orig, 0, 10, 0));
  EXPECT_STREQ("This is a\nlong\nstring\nwith\nalongwordi\nnitman", s);
  string_delete(&s);

  EXPECT_TRUE(string_new_copy_wrap(&s, orig, 0, 5, 0));
  EXPECT_STREQ("This\nis a\nlong\nstrin\ng\nwith\nalong\nwordi\nnitma\nn", s);
  string_delete(&s);

  EXPECT_TRUE(string_new_copy_wrap(&s, orig, 0, 4, 0));
  EXPECT_STREQ("This\nis a\nlong\nstri\nng\nwith\nalon\ngwor\ndini\ntman", s);
  string_delete(&s);

  EXPECT_TRUE(string_new_copy_wrap(&s, orig, 60, 80, 0));
  EXPECT_STREQ("This is a long\nstring with alongwordinitman", s);
  string_delete(&s);

  EXPECT_TRUE(string_new_copy_wrap(&s, orig, 59, 80, 0));
  EXPECT_STREQ("This is a long string\nwith alongwordinitman", s);
  string_delete(&s);

  EXPECT_TRUE(string_new_copy_wrap(&s, orig, 0, 10, 1));
  EXPECT_STREQ("This is a\n long\n string\n with\n alongword\n initman", s);
  string_delete(&s);

  EXPECT_TRUE(string_new_copy_wrap(&s, "More of an indent", 0, 4, 2));
  EXPECT_STREQ("More\n  of\n  an\n  in\n  de\n  nt", s);
  string_delete(&s);

  EXPECT_TRUE(string_new_copy_wrap(&s, "More of an indent", 1, 4, 2));
  EXPECT_STREQ("Mor\n  e\n  of\n  an\n  in\n  de\n  nt", s);
  string_delete(&s);

  EXPECT_TRUE(string_new_copy_wrap(&s, "More of a gap", 2, 4, 3));
  EXPECT_STREQ("Mo\n   r\n   e\n   o\n   f\n   a\n   g\n   a\n   p", s);
  string_delete(&s);

  EXPECT_TRUE(
      string_new_copy_wrap(
        &s,
        "Let's use    some    large  gaps  here",
        0,
        10,
        0));
  EXPECT_STREQ("Let's use\nsome\nlarge\ngaps  here", s);
  string_delete(&s);

  EXPECT_FALSE(string_new_copy_wrap(&s, "Space-end  ", 0, 10, 0));
  EXPECT_STREQ("Space-end", s);
  string_delete(&s);

  EXPECT_TRUE(string_new_copy_wrap(&s, "Space    end            ", 0, 5, 0));
  EXPECT_STREQ("Space\nend", s);
  string_delete(&s);

  /* Test in-place.
   */
  string_new_copy(&s, "More of an indent");
  EXPECT_TRUE(string_renew_wrap(&s, 1, 4, 2));
  EXPECT_STREQ("Mor\n  e\n  of\n  an\n  in\n  de\n  nt", s);
  string_delete(&s);

  /* Test _f version.
   */
  EXPECT_TRUE(
      string_new_f_wrap(&s, 1, 4, 2, "More %s %d indents    ", "of", 27));
  EXPECT_STREQ("Mor\n  e\n  of\n  27\n  in\n  de\n  nt\n  s", s);
  string_delete(&s);

  /* Test concat _f version.
   */
  EXPECT_TRUE(string_new_f_wrap(&s, 0, 6, 2, "This is a great %s", "one"));
  string_renew_concat(&s, "\n\n");
  string_renew_concat_f_wrap(&s, 0, 5, 1, "As is this %d for us %d do", 1, 2);
  EXPECT_STREQ(
      "This\n  is a\n  grea\n  t\n  one\n\nAs is\n this\n 1\n for\n us 2\n do",
      s);
  string_delete(&s);
}

TEST(String, Commaize) {
  char *s = null;

  EXPECT_STREQ("", string_new_commaize(&s, ""));
  string_delete(&s);

  EXPECT_STREQ("a", string_new_commaize(&s, "a"));
  string_delete(&s);

  EXPECT_STREQ("ab", string_new_commaize(&s, "ab"));
  string_delete(&s);

  EXPECT_STREQ("abc", string_new_commaize(&s, "abc"));
  string_delete(&s);

  EXPECT_STREQ("abcd", string_new_commaize(&s, "abcd"));
  string_delete(&s);

  EXPECT_STREQ("abcde", string_new_commaize(&s, "abcde"));
  string_delete(&s);

  EXPECT_STREQ("0", string_new_commaize(&s, "0"));
  string_delete(&s);

  EXPECT_STREQ("12", string_new_commaize(&s, "12"));
  string_delete(&s);

  EXPECT_STREQ("123", string_new_commaize(&s, "123"));
  string_delete(&s);

  EXPECT_STREQ("1234", string_new_commaize(&s, "1234"));
  string_delete(&s);

  EXPECT_STREQ("12,345", string_new_commaize(&s, "12345"));
  string_delete(&s);

  EXPECT_STREQ("123,456", string_new_commaize(&s, "123456"));
  string_delete(&s);

  EXPECT_STREQ("a 0", string_new_commaize(&s, "a 0"));
  string_delete(&s);

  EXPECT_STREQ("a-123456", string_new_commaize(&s, "a-123456"));
  string_delete(&s);

  EXPECT_STREQ("! 12", string_new_commaize(&s, "! 12"));
  string_delete(&s);

  EXPECT_STREQ("= 123", string_new_commaize(&s, "= 123"));
  string_delete(&s);

  EXPECT_STREQ("a 1234", string_new_commaize(&s, "a 1234"));
  string_delete(&s);

  EXPECT_STREQ("= 12,345", string_new_commaize(&s, "= 12345"));
  string_delete(&s);

  EXPECT_STREQ("> 123,456", string_new_commaize(&s, "> 123456"));
  string_delete(&s);

  EXPECT_STREQ("-1,234,567", string_new_commaize(&s, "-1234567"));
  string_delete(&s);

  EXPECT_STREQ("a0", string_new_commaize(&s, "a0"));
  string_delete(&s);

  EXPECT_STREQ("a12", string_new_commaize(&s, "a12"));
  string_delete(&s);

  EXPECT_STREQ("a123", string_new_commaize(&s, "a123"));
  string_delete(&s);

  EXPECT_STREQ("a1234", string_new_commaize(&s, "a1234"));
  string_delete(&s);

  EXPECT_STREQ("a12345", string_new_commaize(&s, "a12345"));
  string_delete(&s);

  EXPECT_STREQ("a123456", string_new_commaize(&s, "a123456"));
  string_delete(&s);

  EXPECT_STREQ("A1234567", string_new_commaize(&s, "A1234567"));
  string_delete(&s);

  EXPECT_STREQ("0a", string_new_commaize(&s, "0a"));
  string_delete(&s);

  EXPECT_STREQ("12a", string_new_commaize(&s, "12a"));
  string_delete(&s);

  EXPECT_STREQ("123a", string_new_commaize(&s, "123a"));
  string_delete(&s);

  EXPECT_STREQ("1234a", string_new_commaize(&s, "1234a"));
  string_delete(&s);

  EXPECT_STREQ("12345a", string_new_commaize(&s, "12345a"));
  string_delete(&s);

  EXPECT_STREQ("123456a", string_new_commaize(&s, "123456a"));
  string_delete(&s);

  EXPECT_STREQ("1234567A", string_new_commaize(&s, "1234567A"));
  string_delete(&s);

  EXPECT_STREQ("0 a", string_new_commaize(&s, "0 a"));
  string_delete(&s);

  EXPECT_STREQ("12 ab", string_new_commaize(&s, "12 ab"));
  string_delete(&s);

  EXPECT_STREQ("123 a", string_new_commaize(&s, "123 a"));
  string_delete(&s);

  EXPECT_STREQ("1234 a", string_new_commaize(&s, "1234 a"));
  string_delete(&s);

  EXPECT_STREQ("12,345 ab", string_new_commaize(&s, "12345 ab"));
  string_delete(&s);

  EXPECT_STREQ("123,456 abc", string_new_commaize(&s, "123456 abc"));
  string_delete(&s);

  EXPECT_STREQ("1,234,567 A!=", string_new_commaize(&s, "1234567 A!="));
  string_delete(&s);

  EXPECT_STREQ("-0.1234", string_new_commaize(&s, "-0.1234"));
  string_delete(&s);

  EXPECT_STREQ("-12.34567", string_new_commaize(&s, "-12.34567"));
  string_delete(&s);

  EXPECT_STREQ("-123.456789!", string_new_commaize(&s, "-123.456789!"));
  string_delete(&s);

  EXPECT_STREQ("-1234.5678,", string_new_commaize(&s, "-1234.5678,"));
  string_delete(&s);

  EXPECT_STREQ("$12,345.6789?", string_new_commaize(&s, "$12345.6789?"));
  string_delete(&s);

  EXPECT_STREQ("-$123,456.7890.", string_new_commaize(&s, "-$123456.7890."));
  string_delete(&s);

  EXPECT_STREQ("#1,234,567.8901,A", string_new_commaize(&s, "#1234567.8901,A"));
  string_delete(&s);

  EXPECT_STREQ("1966-09-24", string_new_commaize(&s, "1966-09-24"));
  string_delete(&s);

  EXPECT_STREQ("January 1, 1901", string_new_commaize(&s, "January 1, 1901"));
  string_delete(&s);

  EXPECT_STREQ("January 1, 1900", string_new_commaize(&s, "January 1, 1900"));
  string_delete(&s);

  EXPECT_STREQ(
      "December 31, 2079",
      string_new_commaize(&s, "December 31, 2079"));
  string_delete(&s);

  EXPECT_STREQ(
      "December 31, 2080",
      string_new_commaize(&s, "December 31, 2080"));
  string_delete(&s);

  string_new_copy(&s, "#1234567.8901,A");
  string_renew_commaize(&s);
  EXPECT_STREQ("#1,234,567.8901,A", s);
  string_delete(&s);

  string_new_copy(&s, "0x023w3y454h");
  string_renew_commaize(&s);
  EXPECT_STREQ("0x023w3y454h", s);
  string_delete(&s);
}

FINISH

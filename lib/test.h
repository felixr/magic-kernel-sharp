/* Test framework, as similar to Googletest as possible, but compatible with
 * c89 and jpC.
 */
#ifndef TEST_H
#define TEST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "jpc.h"
#include <limits.h>

#define g_TEST_setup_(macro) \
  g_TEST_macro_ = macro; \
  g_TEST_file_ = __FILE__; \
  g_TEST_line_ = __LINE__;

#define g_TEST_start_ \
  int main(int argc, char *argv[]) { \
  allow_exit_normally(false);

#define g_TEST_finish_ \
  if (g_TEST_num_failures_ > 0) { \
    throw( \
        "%lu test%s failed in %s", \
        g_TEST_num_failures_, \
        plural_s(g_TEST_num_failures_), \
        argv[0]); \
  } \
  return 0; \
}

#define START \
  g_TEST_start_ \
  g_TEST_death_ = false; \
  {

#define START_DEATH \
  const char *__asan_default_options() {return "detect_leaks=0";} \
  g_TEST_start_ \
  g_TEST_death_ = true;

#define FINISH \
  } \
  g_TEST_finish_

#define FINISH_DEATH \
  g_TEST_finish_

#define TEST(test_name, subtest_name) \
  g_TEST_name_ = #test_name; \
  g_TEST_subname_ = #subtest_name;

#define DISABLE if (false) {
#define ENABLE }

#define ASSERT_ALL g_TEST_assert_all_ = true;
#define NO_ASSERT_ALL g_TEST_assert_all_ = false;

void test_fail(bool assert, char *fmt, ...);

#define EXPECT_FALSE(test) \
  g_TEST_setup_("EXPECT_FALSE"); \
  test_false((test), false);
#define ASSERT_FALSE(test) \
  g_TEST_setup_("ASSERT_FALSE"); \
  test_false((test), true);
void test_false(bool test, bool assert);

#define EXPECT_TRUE(test) \
  g_TEST_setup_("EXPECT_TRUE"); \
  test_true((test), false);
#define ASSERT_TRUE(test) \
  g_TEST_setup_("ASSERT_TRUE"); \
  test_true((test), true);
void test_true(bool test, bool assert);

#define EXPECT_ULEQ(expected, test) \
  g_TEST_setup_("EXPECT_ULEQ"); \
  test_uleq((expected), (test), false);
#define ASSERT_ULEQ(expected, test) \
  g_TEST_setup_("ASSERT_ULEQ"); \
  test_uleq((expected), (test), true);
void test_uleq(ulong expected, ulong test, bool assert);

#define EXPECT_ULNE(expected, test) \
  g_TEST_setup_("EXPECT_ULNE"); \
  test_ulne((expected), (test), false);
#define ASSERT_ULNE(expected, test) \
  g_TEST_setup_("ASSERT_ULNE"); \
  test_ulne((expected), (test), true);
void test_ulne(ulong expected, ulong test, bool assert);

#define EXPECT_LEQ(expected, test) \
  g_TEST_setup_("EXPECT_LEQ"); \
  test_leq((expected), (test), false);
#define ASSERT_LEQ(expected, test) \
  g_TEST_setup_("ASSERT_LEQ"); \
  test_leq((expected), (test), true);
void test_leq(ulong expected, ulong test, bool assert);

#define EXPECT_DEQ(expected, test) \
  g_TEST_setup_("EXPECT_DEQ"); \
  test_deq((expected), (test), false);
#define ASSERT_DEQ(expected, test) \
  g_TEST_setup_("ASSERT_DEQ"); \
  test_deq((expected), (test), true);
void test_deq(double expected, double test, bool assert);

#define EXPECT_PEQ(expected, test) \
  g_TEST_setup_("EXPECT_PEQ"); \
  test_peq((expected), (test), false);
#define ASSERT_PEQ(expected, test) \
  g_TEST_setup_("ASSERT_PEQ"); \
  test_peq((expected), (test), true);
void test_peq(void *expected, void *test, bool assert);

#define EXPECT_NEAR(expected, actual, tolerance) \
  g_TEST_setup_("EXPECT_NEAR"); \
  test_near( \
    #expected, \
    (expected), \
    #actual, \
    (actual), \
    #tolerance, \
    (tolerance), \
    false);
#define ASSERT_NEAR(expected, actual, tolerance) \
  g_TEST_setup_("ASSERT_NEAR"); \
  test_near( \
    #expected, \
    (expected), \
    #actual, \
    (actual), \
    #tolerance, \
    (tolerance), \
    true);
void test_near(
    char *expected_code,
    double expected,
    char *actual_code,
    double actual,
    char *tolerance_code,
    double tolerance,
    bool assert);

#define EXPECT_STREQ(expected, test) \
  g_TEST_setup_("EXPECT_STREQ"); \
  test_streq((expected), (test), false);
#define ASSERT_STREQ(expected, test) \
  g_TEST_setup_("ASSERT_STREQ"); \
  test_streq((expected), (test), true);
void test_streq(char *expected, char *test, bool assert);

#define EXPECT_STRNEQ(expected, test) \
  g_TEST_setup_("EXPECT_STRNEQ"); \
  test_strneq((expected), (test), false);
#define ASSERT_STRNEQ(expected, test) \
  g_TEST_setup_("ASSERT_STRNEQ"); \
  test_strneq((expected), (test), true);
void test_strneq(char *expected, char *test, bool assert);

#define EXPECT_DEATH(code, expected_exception) \
  g_TEST_setup_("EXPECT_DEATH"); \
  DEATH_GENERAL_(code, 0, ULONG_MAX, expected_exception, false)
#define ASSERT_DEATH(code, expected_exception) \
  g_TEST_setup_("ASSERT_DEATH"); \
  DEATH_GENERAL_(code, 0, ULONG_MAX, expected_exception, true)
#define EXPECT_DEATH_SUBSTRING(code, s, e, expected_exception) \
  g_TEST_setup_("EXPECT_DEATH"); \
  DEATH_GENERAL_(code, s, e, expected_exception, false)
#define ASSERT_DEATH_SUBSTRING(code, s, e, expected_exception) \
  g_TEST_setup_("ASSERT_DEATH"); \
  DEATH_GENERAL_(code, s, e, expected_exception, true)
#define DEATH_GENERAL_(code, s, e, expected_exception, assert) \
  if (!g_TEST_death_) { \
    throw("Cannot call DEATH tests without the START_DEATH directive"); \
  } \
  g_TEST_didnt_die_ = false; \
  try { \
    code; \
    g_TEST_didnt_die_ = true; \
  } catch { \
    if (!string_substring_equals(exception, s, e, expected_exception)) { \
      test_fail( \
        assert, \
        "Expected exception\n  %s\nbut got\n  %.*s", \
        expected_exception, \
        e - s, \
        exception + s); \
    } \
  } yrt; \
  if (g_TEST_didnt_die_) { \
    test_fail( \
        assert, \
        "Expected '%s' to be thrown but it wasn't", \
        expected_exception); \
  }

#define EXPECT_NULL(p) \
  g_TEST_setup_("EXPECT_NULL"); \
  test_null((void *)p, false);
#define ASSERT_NULL(p) \
  g_TEST_setup_("ASSERT_NULL"); \
  test_null((void *)p, true);
void test_null(void *p, bool assert);

#define EXPECT_NONNULL(p) \
  g_TEST_setup_("EXPECT_NONNULL"); \
  test_nonnull((void *)p, false);
#define ASSERT_NONNULL(p) \
  g_TEST_setup_("ASSERT_NONNULL"); \
  test_nonnull((void *)p, true);
void test_nonnull(void *p, bool assert);

#define EXPECT(error) \
  g_TEST_setup_("EXPECT"); \
  test_general(error, false);
#define ASSERT(error) \
  g_TEST_setup_("ASSERT"); \
  test_general(error, true);
void test_general(char *error, bool assert);

#ifndef TEST_C
extern bool g_TEST_death_;
extern bool g_TEST_didnt_die_;
extern bool g_TEST_assert_all_;
extern char *g_TEST_macro_;
extern char *g_TEST_file_;
extern char *g_TEST_name_;
extern char *g_TEST_subname_;
extern int g_TEST_line_;
extern ulong g_TEST_num_failures_;
#endif /* TEST_C */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _H */

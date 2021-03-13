#include "args_test.h"

static void cycle_(void) {
  args_try_cleanup();
  args_description("");
}

static void command_(char *first, ...) {
  char *s;
  char **argv = null;
  int argc;
  va_list ap;

  if (first == null) {
    new(&argv, argc = 1);
    argv[0] = "prog";
  } else {
    new(&argv, argc = 2);
    argv[0] = "prog";
    argv[1] = first;
    va_start(ap, first);
    while ((s = va_arg(ap, char *)) != null) {
      renewup(&argv, argc, argc + 1);
      argv[argc] = s;
      argc++;
    }
    va_end(ap);
  }

  args_parse(argc, argv);

  deletev(&argv);
}

START

TEST(Args, Help) {
  char *s = null;

  args_program("prog");

  args_description("Good prog");
  args_help(&s);
  EXPECT_STREQ(
      "prog: Good prog\n\n"
      "Usage:\n  prog [options]\n\n"
      "Options:\n  -?, --help                  Print this help page and exit\n",
      s);
  string_delete(&s);

  args_string("name", "Filename");
  args_help(&s);
  EXPECT_STREQ(
      "prog: Good prog\n\n"
      "Usage:\n  prog [options] <NAME>\n\n"
      "Options:\n  -?, --help                  Print this help page and exit\n"
      "\n"
      "Arguments:\n  NAME              Filename\n",
      s);
  string_delete(&s);

  args_optional_integer("count", 'c', 10, "Repeat $ times");
  args_help(&s);
  EXPECT_STREQ(
      "prog: Good prog\n\n"
      "Usage:\n  prog [options] <NAME>\n\n"
      "Options:\n"
      "  -?, --help                  Print this help page and exit\n"
      "  -c COUNT, --count=COUNT     Repeat COUNT times (default = 10)\n\n"
      "Arguments:\n  NAME              Filename\n",
      s);
  string_delete(&s);
  args_cleanup();

  args_program("prog");
  args_description("Check newline");
  args_string("name", "Filename");
  args_flag("has-header-row", 'h', "The file has a %s row", "header");
  args_help(&s);
  EXPECT_STREQ(
      "prog: Check newline\n\n"
      "Usage:\n  prog [options] <NAME>\n\n"
      "Options:\n"
      "  -?, --help                  Print this help page and exit\n"
      "  -h, --has-header-row        The file has a header row\n\n"
      "Arguments:\n  NAME              Filename\n",
      s);
  string_delete(&s);
  args_cleanup();

  args_program("prog");
  args_description("Check newline");
  args_string("name", "Filename");
  args_flag("has-header-row", 'h', "The file has a header row");
  args_flag(
      "something-else",
      'i',
      "The file has a %s and %s needs a long description",
      "something else",
      "it");
  args_help(&s);
  EXPECT_STREQ(
      "prog: Check newline\n\n"
      "Usage:\n  prog [options] <NAME>\n\n"
      "Options:\n"
      "  -?, --help                  Print this help page and exit\n"
      "  -h, --has-header-row        The file has a header row\n"
      "  -i, --something-else        The file has a something else and "
      "it needs a long\n"
      "                              description\n"
      "Arguments:\n  NAME              Filename\n",
      s);
  string_delete(&s);
  args_cleanup();

  args_program("prog");
  args_description("Check newline");
  args_string("name", "Filename");
  args_flag(
      "something-else",
      'i',
      "The file has a something else and it needs a long description");
  args_flag("has-header-row", 'h', "The file has a header row");
  args_help(&s);
  EXPECT_STREQ(
      "prog: Check newline\n\n"
      "Usage:\n  prog [options] <NAME>\n\n"
      "Options:\n"
      "  -?, --help                  Print this help page and exit\n"
      "  -i, --something-else        The file has a something else and "
      "it needs a long\n"
      "                              description\n"
      "  -h, --has-header-row        The file has a header row\n\n"
      "Arguments:\n  NAME              Filename\n",
      s);
  string_delete(&s);
  args_cleanup();

  args_program("prog");
  args_description("Has strings");
  args_strings("files", "The files to be processed");
  args_help(&s);
  EXPECT_STREQ(
      "prog: Has strings\n\n"
      "Usage:\n  prog [options] [<FILES...>]\n\n"
      "Options:\n"
      "  -?, --help                  Print this help page and exit\n"
      "\nArguments:\n  [FILES...]        The files to be processed\n",
      s);
  string_delete(&s);
  args_cleanup();

  args_program("prog");
  args_description("No extra line after indent");
  args_string("filename", "The filename");
  args_optional_integer("option-name", 'o', 12, "Count");
  args_help(&s);
  EXPECT_STREQ(
      "prog: No extra line after indent\n\n"
      "Usage:\n  prog [options] <FILENAME>\n\n"
      "Options:\n"
      "  -?, --help                  Print this help page and exit\n"
      "  -o OPTION_NAME, --option-name=OPTION_NAME\n"
      "                              Count (default = 12)\n"
      "Arguments:\n  FILENAME          The filename\n",
      s);
  string_delete(&s);
  args_cleanup();

  args_program("prog");
  args_description("No extra line after indent");
  args_string("filename", "The filename");
  args_flag("option-name-causing-indent", 'o', "A useful flag");
  args_help(&s);
  EXPECT_STREQ(
      "prog: No extra line after indent\n\n"
      "Usage:\n  prog [options] <FILENAME>\n\n"
      "Options:\n"
      "  -?, --help                  Print this help page and exit\n"
      "  -o, --option-name-causing-indent\n"
      "                              A useful flag\n"
      "Arguments:\n  FILENAME          The filename\n",
      s);
  string_delete(&s);
  args_cleanup();

  args_program("prog");
  args_description("Unbreakable spaces");
  args_string(
      "filename",
      "The filename\fwhich\fcan't\fbe\fbroken\f"
      "at\funbreakable\fspaces\fexcept\fwhere we say it can");
  args_flag(
      "option",
      'o',
      "A useful flag\fwhich\fcan't\fbe\fbroken\f"
      "at\funbreakable\fspaces\fexcept\fwhere\fwe say it can");
  args_help(&s);
  EXPECT_STREQ(
      "prog: Unbreakable spaces\n\n"
      "Usage:\n  prog [options] <FILENAME>\n\n"
      "Options:\n"
      "  -?, --help                  Print this help page and exit\n"
      "  -o, --option                A useful\n"
      "                              flag which can't be broken at unbreakable "
      "spaces e\n"
      "                              xcept where we say it can\n"
      "Arguments:\n"
      "  FILENAME          The\n"
      "                    filename which can't be broken at unbreakable "
      "spaces except \n"
      "                    where we say it can\n",
      s);
  string_delete(&s);
  args_cleanup();

  args_program("prog");
  args_description("Multi-line");
  args_string(
      "filename",
      "This the filename\nAnd this is a new line "
      "that is very long and must be wrapped across two lines");
  args_help(&s);
  EXPECT_STREQ(
      "prog: Multi-line\n\n"
      "Usage:\n  prog [options] <FILENAME>\n\n"
      "Options:\n"
      "  -?, --help                  Print this help page and exit\n\n"
      "Arguments:\n"
      "  FILENAME          This the filename\n"
      "                    And this is a new line that is very long and "
      "must be wrapped\n"
      "                      across two lines\n",
      s);
  string_delete(&s);
  args_cleanup();

  args_program("prog");
  args_description("Multi-line");
  args_flag(
      "option",
      'o',
      "This the filename\nAnd this is a new line "
      "that is very long and must be wrapped across two lines");
  args_help(&s);
  EXPECT_STREQ(
      "prog: Multi-line\n\n"
      "Usage:\n  prog [options]\n\n"
      "Options:\n"
      "  -?, --help                  Print this help page and exit\n"
      "  -o, --option                This the filename\n"
      "                              And this is a new line that "
      "is very long and must\n"
      "                                be wrapped across two lines\n",
      s);
  string_delete(&s);
  args_cleanup();

  args_program("prog");
  args_description("Tabs");
  args_flag(
      "option",
      'o',
      "Pick:\n\t1. Good\n\t\ta. Right\n\t2. Bad. This is an option that "
      "is being described with a very long description line.");
  args_help(&s);
  EXPECT_STREQ(
      "prog: Tabs\n\n"
      "Usage:\n  prog [options]\n\n"
      "Options:\n"
      "  -?, --help                  Print this help page and exit\n"
      "  -o, --option                Pick:\n"
      "                                1. Good\n"
      "                                  a. Right\n"
      "                                2. Bad. This is an option that "
      "is being\n"
      "                                  described with a very long "
      "description line.\n",
      s);
  string_delete(&s);
  args_cleanup();

  args_description("Default after set");
  args_optional_integer("cd", 'a', 1, "desc");
  command_("-a2", null);
  args_help(&s);
  EXPECT_STREQ(
      "prog: Default after set\n\n"
      "Usage:\n  prog [options]\n\nOptions:\n"
      "  -?, --help                  Print this help page and exit\n"
      "  -a CD, --cd=CD              desc (default = 1)\n",
      s);
  string_delete(&s);
  args_cleanup();
}

TEST(Args, Get) {
  char **strings;
  ulong n;

  cycle_();

  /* Get one positional argument.
  */
  args_string("ab", "desc");
  command_("woof", null);
  EXPECT_STREQ("woof", args_get_string("ab"));
  cycle_();

  args_integer("ab", "desc");
  command_("42", null);
  EXPECT_LEQ(42, args_get_integer("ab"));
  cycle_();

  args_real("ab", "desc");
  command_("--", "-3.1", null);
  EXPECT_DEQ(-3.1, args_get_real("ab"));
  cycle_();

  /* Get default of one optional argument. We can't just call the program with
   * no arguments at all, since that will try to print the help page and exit.
  */
  args_flag("ab", ' ', "desc");
  args_string("c", "desc");
  command_("dog", null);
  EXPECT_FALSE(args_get_flag("ab"));
  cycle_();

  args_optional_string("ab", ' ', "woof", "desc");
  args_string("c", "desc");
  command_("dog", null);
  EXPECT_STREQ("woof", args_get_string("ab"));
  cycle_();

  args_optional_integer("ab", 'x', -12, "desc");
  args_string("c", "desc");
  command_("dog", null);
  EXPECT_LEQ(-12, args_get_integer("ab"));
  cycle_();

  args_optional_real("ab", 'V', -1e-3, "desc");
  args_string("c", "desc");
  command_("dog", null);
  EXPECT_DEQ(-1e-3, args_get_real("ab"));
  cycle_();

  /* Get value of one set flag.
  */
  args_flag("ab", ' ', "desc");
  command_("--ab", null);
  EXPECT_TRUE(args_get_flag("ab"));
  cycle_();

  args_flag("ab", ' ', "desc");
  command_(" --ab", null);
  EXPECT_TRUE(args_get_flag("ab"));
  cycle_();

  args_flag("ab", ' ', "desc");
  command_("--ab ", null);
  EXPECT_TRUE(args_get_flag("ab"));
  cycle_();

  args_flag("ab", ' ', "desc");
  command_(" --ab ", null);
  EXPECT_TRUE(args_get_flag("ab"));
  cycle_();

  args_flag("ab", ' ', "desc");
  command_("   --ab ", null);
  EXPECT_TRUE(args_get_flag("ab"));
  cycle_();

  args_flag("ab", ' ', "desc");
  command_(" --ab   ", null);
  EXPECT_TRUE(args_get_flag("ab"));
  cycle_();

  /* Get supplied value of one optional string.
   */
  args_optional_string("ab", 'a', "woof", "desc");
  command_("-a", "woof", null);
  EXPECT_STREQ("woof", args_get_string("ab"));
  cycle_();

  args_optional_string("ab", 'a', "woof", "desc");
  command_("-awoof", null);
  EXPECT_STREQ("woof", args_get_string("ab"));
  cycle_();

  args_optional_string("ab", 'a', "woof", "desc");
  command_("-awoof ", null);
  EXPECT_STREQ("woof", args_get_string("ab"));
  cycle_();

  args_optional_string("ab", 'a', "woof", "desc");
  command_("-awoof  ", null);
  EXPECT_STREQ("woof", args_get_string("ab"));
  cycle_();

  args_optional_string("ab", 'a', "woof", "desc");
  command_("-a woof", null);
  EXPECT_STREQ("woof", args_get_string("ab"));
  cycle_();

  args_optional_string("ab", 'a', "woof", "desc");
  command_("-a woof ", null);
  EXPECT_STREQ("woof", args_get_string("ab"));
  cycle_();

  args_optional_string("ab", 'a', "woof", "desc");
  command_("-a woof  ", null);
  EXPECT_STREQ("woof", args_get_string("ab"));
  cycle_();

  args_optional_string("ab", 'a', "woof", "desc");
  command_(" -a    woof ", null);
  EXPECT_STREQ("woof", args_get_string("ab"));
  cycle_();

  args_optional_string("ab", 'a', "woof", "desc");
  command_("-a", "woof", null);
  EXPECT_STREQ("woof", args_get_string("ab"));
  cycle_();

  args_optional_string("ab", 'a', "woof", "desc");
  command_(" -a", "woof", null);
  EXPECT_STREQ("woof", args_get_string("ab"));
  cycle_();

  args_optional_string("ab", 'a', "woof", "desc");
  command_("-a ", "woof", null);
  EXPECT_STREQ("woof", args_get_string("ab"));
  cycle_();

  args_optional_string("ab", 'a', "woof", "desc");
  command_("  -a    ", "woof", null);
  EXPECT_STREQ("woof", args_get_string("ab"));
  cycle_();

  args_optional_string("ab", 'a', "woof", "desc");
  command_("-a", " woof", null);
  EXPECT_STREQ(" woof", args_get_string("ab"));
  cycle_();

  args_optional_string("ab", 'a', "woof", "desc");
  command_("-a", "woof ", null);
  EXPECT_STREQ("woof ", args_get_string("ab"));
  cycle_();

  args_optional_string("ab", 'a', "woof", "desc");
  command_("-a", " woof ", null);
  EXPECT_STREQ(" woof ", args_get_string("ab"));
  cycle_();

  args_optional_string("ab", 'a', "woof", "desc");
  command_("-a", "   woof     ", null);
  EXPECT_STREQ("   woof     ", args_get_string("ab"));
  cycle_();

  args_optional_string("ab", 'a', "woof", "desc");
  command_("--ab", "dog", null);
  EXPECT_STREQ("dog", args_get_string("ab"));
  cycle_();

  args_optional_string("ab", 'a', "woof", "desc");
  command_("--ab dog", null);
  EXPECT_STREQ("dog", args_get_string("ab"));
  cycle_();

  args_optional_string("ab", 'a', "woof", "desc");
  command_("--ab=dog", null);
  EXPECT_STREQ("dog", args_get_string("ab"));
  cycle_();

  args_optional_string("ab", 'a', "woof", "desc");
  command_("--ab= dog", null);
  EXPECT_STREQ("dog", args_get_string("ab"));
  cycle_();

  args_optional_string("ab", 'a', "woof", "desc");
  command_("--ab=dog ", null);
  EXPECT_STREQ("dog", args_get_string("ab"));
  cycle_();

  args_optional_string("ab", 'a', "woof", "desc");
  command_("--ab= dog ", null);
  EXPECT_STREQ("dog", args_get_string("ab"));
  cycle_();

  args_optional_string("ab", 'a', "woof", "desc");
  command_("--ab=   dog    ", null);
  EXPECT_STREQ("dog", args_get_string("ab"));
  cycle_();

  /* Get supplied value of one optional integer.
   */
  args_optional_integer("ab", 'a', 0, "desc");
  command_("-a", "42", null);
  EXPECT_LEQ(42, args_get_integer("ab"));
  cycle_();

  args_optional_integer("ab", 'a', 0, "desc");
  command_("-a42", null);
  EXPECT_LEQ(42, args_get_integer("ab"));
  cycle_();

  args_optional_integer("ab", 'a', 0, "desc");
  command_("-a42 ", null);
  EXPECT_LEQ(42, args_get_integer("ab"));
  cycle_();

  args_optional_integer("ab", 'a', 0, "desc");
  command_("-a42  ", null);
  EXPECT_LEQ(42, args_get_integer("ab"));
  cycle_();

  args_optional_integer("ab", 'a', 0, "desc");
  command_("-a 42", null);
  EXPECT_LEQ(42, args_get_integer("ab"));
  cycle_();

  args_optional_integer("ab", 'a', 0, "desc");
  command_("-a 42 ", null);
  EXPECT_LEQ(42, args_get_integer("ab"));
  cycle_();

  args_optional_integer("ab", 'a', 0, "desc");
  command_("-a 42  ", null);
  EXPECT_LEQ(42, args_get_integer("ab"));
  cycle_();

  args_optional_integer("ab", 'a', 0, "desc");
  command_(" -a    42 ", null);
  EXPECT_LEQ(42, args_get_integer("ab"));
  cycle_();

  args_optional_integer("ab", 'a', 0, "desc");
  command_("-a", "42", null);
  EXPECT_LEQ(42, args_get_integer("ab"));
  cycle_();

  args_optional_integer("ab", 'a', 0, "desc");
  command_(" -a", "42", null);
  EXPECT_LEQ(42, args_get_integer("ab"));
  cycle_();

  args_optional_integer("ab", 'a', 0, "desc");
  command_("-a ", "42", null);
  EXPECT_LEQ(42, args_get_integer("ab"));
  cycle_();

  args_optional_integer("ab", 'a', 0, "desc");
  command_("  -a    ", "42", null);
  EXPECT_LEQ(42, args_get_integer("ab"));
  cycle_();

  args_optional_integer("ab", 'a', 0, "desc");
  command_("-a", " 42", null);
  EXPECT_LEQ(42, args_get_integer("ab"));
  cycle_();

  args_optional_integer("ab", 'a', 0, "desc");
  command_("-a", "42 ", null);
  EXPECT_LEQ(42, args_get_integer("ab"));
  cycle_();

  args_optional_integer("ab", 'a', 0, "desc");
  command_("-a", " 42 ", null);
  EXPECT_LEQ(42, args_get_integer("ab"));
  cycle_();

  args_optional_integer("ab", 'a', 0, "desc");
  command_("-a", "   42     ", null);
  EXPECT_LEQ(42, args_get_integer("ab"));
  cycle_();

  args_optional_integer("ab", 'a', 0, "desc");
  command_("--ab", "-12", null);
  EXPECT_LEQ(-12, args_get_integer("ab"));
  cycle_();

  args_optional_integer("ab", 'a', 0, "desc");
  command_("--ab -12", null);
  EXPECT_LEQ(-12, args_get_integer("ab"));
  cycle_();

  args_optional_integer("ab", 'a', 0, "desc");
  command_("--ab=-12", null);
  EXPECT_LEQ(-12, args_get_integer("ab"));
  cycle_();

  args_optional_integer("ab", 'a', 0, "desc");
  command_("--ab= -12", null);
  EXPECT_LEQ(-12, args_get_integer("ab"));
  cycle_();

  args_optional_integer("ab", 'a', 0, "desc");
  command_("--ab=-12 ", null);
  EXPECT_LEQ(-12, args_get_integer("ab"));
  cycle_();

  args_optional_integer("ab", 'a', 0, "desc");
  command_("--ab= -12 ", null);
  EXPECT_LEQ(-12, args_get_integer("ab"));
  cycle_();

  args_optional_integer("ab", 'a', 0, "desc");
  command_("--ab=   -12    ", null);
  EXPECT_LEQ(-12, args_get_integer("ab"));
  cycle_();

  /* Get supplied value of one optional real.
   */
  args_optional_real("ab", 'a', 3.14, "desc");
  command_("-a", "42", null);
  EXPECT_DEQ(42, args_get_real("ab"));
  cycle_();

  args_optional_real("ab", 'a', 3.14, "desc");
  command_("-a42", null);
  EXPECT_DEQ(42, args_get_real("ab"));
  cycle_();

  args_optional_real("ab", 'a', 3.14, "desc");
  command_("-a42 ", null);
  EXPECT_DEQ(42, args_get_real("ab"));
  cycle_();

  args_optional_real("ab", 'a', 3.14, "desc");
  command_("-a42  ", null);
  EXPECT_DEQ(42, args_get_real("ab"));
  cycle_();

  args_optional_real("ab", 'a', 3.14, "desc");
  command_("-a 42", null);
  EXPECT_DEQ(42, args_get_real("ab"));
  cycle_();

  args_optional_real("ab", 'a', 3.14, "desc");
  command_("-a 42 ", null);
  EXPECT_DEQ(42, args_get_real("ab"));
  cycle_();

  args_optional_real("ab", 'a', 3.14, "desc");
  command_("-a 42  ", null);
  EXPECT_DEQ(42, args_get_real("ab"));
  cycle_();

  args_optional_real("ab", 'a', 3.14, "desc");
  command_(" -a    42 ", null);
  EXPECT_DEQ(42, args_get_real("ab"));
  cycle_();

  args_optional_real("ab", 'a', 3.14, "desc");
  command_("-a", "42", null);
  EXPECT_DEQ(42, args_get_real("ab"));
  cycle_();

  args_optional_real("ab", 'a', 3.14, "desc");
  command_(" -a", "42", null);
  EXPECT_DEQ(42, args_get_real("ab"));
  cycle_();

  args_optional_real("ab", 'a', 3.14, "desc");
  command_("-a ", "42", null);
  EXPECT_DEQ(42, args_get_real("ab"));
  cycle_();

  args_optional_real("ab", 'a', 3.14, "desc");
  command_("  -a    ", "42", null);
  EXPECT_DEQ(42, args_get_real("ab"));
  cycle_();

  args_optional_real("ab", 'a', 3.14, "desc");
  command_("-a", " 42", null);
  EXPECT_DEQ(42, args_get_real("ab"));
  cycle_();

  args_optional_real("ab", 'a', 3.14, "desc");
  command_("-a", "42 ", null);
  EXPECT_DEQ(42, args_get_real("ab"));
  cycle_();

  args_optional_real("ab", 'a', 3.14, "desc");
  command_("-a", " 42 ", null);
  EXPECT_DEQ(42, args_get_real("ab"));
  cycle_();

  args_optional_real("ab", 'a', 3.14, "desc");
  command_("-a", "   42     ", null);
  EXPECT_DEQ(42, args_get_real("ab"));
  cycle_();

  args_optional_real("ab", 'a', 3.14, "desc");
  command_("--ab", "-1.3e-6", null);
  EXPECT_DEQ(-1.3e-6, args_get_real("ab"));
  cycle_();

  args_optional_real("ab", 'a', 3.14, "desc");
  command_("--ab -1.3e-6", null);
  EXPECT_DEQ(-1.3e-6, args_get_real("ab"));
  cycle_();

  args_optional_real("ab", 'a', 3.14, "desc");
  command_("--ab=-1.3e-6", null);
  EXPECT_DEQ(-1.3e-6, args_get_real("ab"));
  cycle_();

  args_optional_real("ab", 'a', 3.14, "desc");
  command_("--ab= -1.3e-6", null);
  EXPECT_DEQ(-1.3e-6, args_get_real("ab"));
  cycle_();

  args_optional_real("ab", 'a', 3.14, "desc");
  command_("--ab=-1.3e-6 ", null);
  EXPECT_DEQ(-1.3e-6, args_get_real("ab"));
  cycle_();

  args_optional_real("ab", 'a', 3.14, "desc");
  command_("--ab= -1.3e-6 ", null);
  EXPECT_DEQ(-1.3e-6, args_get_real("ab"));
  cycle_();

  args_optional_real("ab", 'a', 3.14, "desc");
  command_("--ab=   -1.3e-6    ", null);
  EXPECT_DEQ(-1.3e-6, args_get_real("ab"));
  cycle_();

  /* Test flag runs.
   */
  args_flag("dog", 'd', "desc");
  args_flag("cat", 'c', "desc");
  command_("-cd", null);
  EXPECT_TRUE(args_get_flag("dog"));
  EXPECT_TRUE(args_get_flag("cat"));
  cycle_();

  args_flag("dog", 'd', "desc");
  args_flag("cat", 'c', "desc");
  args_flag("elephant", 'e', "desc");
  args_optional_string("fish", 'f', "big", "desc");
  command_("-cfgold", null);
  EXPECT_FALSE(args_get_flag("dog"));
  EXPECT_TRUE(args_get_flag("cat"));
  EXPECT_FALSE(args_get_flag("elephant"));
  EXPECT_STREQ("gold", args_get_string("fish"));
  cycle_();

  args_flag("dog", 'd', "desc");
  args_flag("cat", 'c', "desc");
  args_flag("elephant", 'e', "desc");
  args_optional_string("fish", 'f', "big", "desc");
  command_("-cdf", "gold", null);
  EXPECT_TRUE(args_get_flag("dog"));
  EXPECT_TRUE(args_get_flag("cat"));
  EXPECT_FALSE(args_get_flag("elephant"));
  EXPECT_STREQ("gold", args_get_string("fish"));
  cycle_();

  args_flag("dog", 'd', "desc");
  args_flag("cat", 'c', "desc");
  args_flag("elephant", 'e', "desc");
  args_optional_string("fish", 'f', "big", "desc");
  command_("-ecdf gold", null);
  EXPECT_TRUE(args_get_flag("dog"));
  EXPECT_TRUE(args_get_flag("cat"));
  EXPECT_TRUE(args_get_flag("elephant"));
  EXPECT_STREQ("gold", args_get_string("fish"));
  cycle_();

  /* Get multiple positional arguments.
   */
  args_string("dog", "desc");
  args_string("cat", "desc");
  command_("woof", "a b", null);
  EXPECT_STREQ("woof", args_get_string("dog"));
  EXPECT_STREQ("a b", args_get_string("cat"));
  cycle_();

  args_integer("count", "desc");
  args_real("size", "desc");
  args_string("dog", "desc");
  args_string("cat", "desc");
  command_("12", "3.14", "woof", "  a b ", null);
  EXPECT_LEQ(12, args_get_integer("count"));
  EXPECT_DEQ(3.14, args_get_real("size"));
  EXPECT_STREQ("woof", args_get_string("dog"));
  EXPECT_STREQ("  a b ", args_get_string("cat"));
  cycle_();

  /* Test general combinations of arguments.
   */
  args_flag("dog", 'd', "desc");
  args_flag("cat", 'c', "desc");
  args_optional_string("fish", 'f', "big", "desc");
  args_optional_real("size", 's', 1.5, "desc");
  args_string("name", "desc");
  args_integer("count", "desc");
  command_("-d", "--fish=gold", "file", "12", null);
  EXPECT_TRUE(args_get_flag("dog"));
  EXPECT_FALSE(args_get_flag("cat"));
  EXPECT_STREQ("gold", args_get_string("fish"));
  EXPECT_DEQ(1.5, args_get_real("size"));
  EXPECT_STREQ("file", args_get_string("name"));
  EXPECT_LEQ(12, args_get_integer("count"));
  cycle_();

  args_flag("dog", 'd', "desc");
  args_flag("cat", 'c', "desc");
  args_optional_string("fish", 'f', "big", "desc");
  args_optional_real("size", 's', 1.5, "desc");
  args_string("name", "desc");
  args_integer("count", "desc");
  command_("--cat", "-s3.14", "file", "12", null);
  EXPECT_FALSE(args_get_flag("dog"));
  EXPECT_TRUE(args_get_flag("cat"));
  EXPECT_STREQ("big", args_get_string("fish"));
  EXPECT_DEQ(3.14, args_get_real("size"));
  EXPECT_STREQ("file", args_get_string("name"));
  EXPECT_LEQ(12, args_get_integer("count"));
  cycle_();

  /* Test strings.
   */
  args_string("c", "desc");
  args_strings("rest", "desc");
  command_("dog", null);
  EXPECT_ULEQ(0, n = args_get_strings(&strings));
  cycle_();

  args_strings("rest", "desc");
  command_("dog", null);
  EXPECT_ULEQ(1, n = args_get_strings(&strings));
  EXPECT_STREQ("dog", strings[0]);
  cycle_();

  args_strings("rest", "desc");
  command_("dog", "a b", null);
  EXPECT_ULEQ(2, n = args_get_strings(&strings));
  EXPECT_STREQ("dog", strings[0]);
  EXPECT_STREQ("a b", strings[1]);
  cycle_();

  args_strings("rest", "desc");
  command_("dog", "--", "a b", null);
  EXPECT_ULEQ(2, n = args_get_strings(&strings));
  EXPECT_STREQ("dog", strings[0]);
  EXPECT_STREQ("a b", strings[1]);
  cycle_();

  args_strings("rest", "desc");
  command_("--", "dog", "a b", null);
  EXPECT_ULEQ(2, n = args_get_strings(&strings));
  EXPECT_STREQ("dog", strings[0]);
  EXPECT_STREQ("a b", strings[1]);
  cycle_();

  args_strings("rest", "desc");
  command_("dog", "a b", "  1.3 4   ", null);
  EXPECT_ULEQ(3, n = args_get_strings(&strings));
  EXPECT_STREQ("dog", strings[0]);
  EXPECT_STREQ("a b", strings[1]);
  EXPECT_STREQ("  1.3 4   ", strings[2]);
  cycle_();

  args_strings("rest", "desc");
  args_flag("woof", 'w', "desc");
  command_("dog", "a b", null);
  EXPECT_ULEQ(2, n = args_get_strings(&strings));
  EXPECT_STREQ("dog", strings[0]);
  EXPECT_STREQ("a b", strings[1]);
  cycle_();

  args_strings("rest", "desc");
  args_flag("woof", 'w', "desc");
  command_("dog", "a b", null);
  EXPECT_ULEQ(2, n = args_get_strings(&strings));
  EXPECT_STREQ("dog", strings[0]);
  EXPECT_STREQ("a b", strings[1]);
  cycle_();

  args_strings("rest", "desc");
  args_flag("woof", 'w', "desc");
  command_("-w", "dog", "a b", null);
  EXPECT_ULEQ(2, n = args_get_strings(&strings));
  EXPECT_STREQ("dog", strings[0]);
  EXPECT_STREQ("a b", strings[1]);
  cycle_();

  args_strings("rest", "desc");
  args_flag("woof", 'w', "desc");
  command_("-w", "--", "dog", "a b", null);
  EXPECT_ULEQ(2, n = args_get_strings(&strings));
  EXPECT_STREQ("dog", strings[0]);
  EXPECT_STREQ("a b", strings[1]);
  cycle_();

  args_strings("rest", "desc");
  args_flag("woof", 'w', "desc");
  command_("-w -- dog", "a b", null);
  EXPECT_ULEQ(2, n = args_get_strings(&strings));
  EXPECT_STREQ("dog", strings[0]);
  EXPECT_STREQ("a b", strings[1]);
  cycle_();

  args_strings("rest", "desc");
  args_flag("woof", 'w', "desc");
  command_("-w -- dog", "a b", null);
  EXPECT_ULEQ(2, n = args_get_strings(&strings));
  EXPECT_STREQ("dog", strings[0]);
  EXPECT_STREQ("a b", strings[1]);
  cycle_();

  args_strings("files", "desc");
  command_("-", null);
  EXPECT_ULEQ(1, n = args_get_strings(&strings));
  EXPECT_STREQ("-", strings[0]);
  cycle_();

  args_cleanup();
}

FINISH

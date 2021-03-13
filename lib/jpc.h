/* jpC: a very thin wrapper around ANSI C (C89).
 *
 * These are the main themes of jpC:
 *
 * CODE CONVENTIONS: These follow most modern usage, and VIM's default syntax
 * formatting for C is generally applied. The braceless block is avoided.
 * Pointers are written in the original form 'char *s' rather than 'char* s'.
 * Fundamental types are given names used by C# (rather than C99). For clarity
 * and definiteness, all variable declarations are made at the start of a
 * function (not at the start of any internal block, as ANSI C allows), with
 * only one variable declared per line, with those lines appearing in
 * increasing "type complexity," with variables sorted alphabetically within
 * the same type. Except as noted below, variables and functions use underscore
 * case. Static global variable names end with an underscore; static local
 * variables end with two underscores. Global variables externally referenced
 * are always considered special "tricks" of the library in question, not
 * intended to be directly used in any other code, and the variable names are
 * of the form g_<LIB>_<name>_ to avoid any potential clash with normal
 * variables.
 *
 * C++ COMPATIBILITY: For a period of time, these libraries were tested using
 * Googletest, and so all header files are wrapped in C++ guards, and the code
 * was confirmed to be compatible with C++. As described below, all unit
 * testing is now written in jpC itself, but all code should still be
 * C++-compatible; please contact me if you find any bugs.
 *
 * SOURCE FILE STRUCTURE: To allow automatic dependency inference and build,
 * without ever needing to write a makefile or equivalent, every file [name].c
 * is always accompanied by a header file [name].h. The only local #include
 * (i.e. with "") that may appear in [name].c is '#include "[name].h"'; any
 * other local includes must be done in [name].h itself, or else the automated
 * make will fail. System includes, i.e. with <>, may appear in [name].c, since
 * they do not need to be rebuilt; they only need to be in [name].h if they
 * define something required in [name].h itself.
 *
 * MEMORY HANDLING: jpC seeks to retain C's native, direct memory allocation
 * and freeing paradigm, but provides a lightweight wrapper around it. The
 * paradigm is that you can only allocate memory to a pointer that is currently
 * empty (null). When you free that memory, the pointer is again set to empty.
 * This doesn't prevent all mistakes, but it tends to bring in just enough
 * discipline to make them rare; and now that we have ASAN and LSAN, bugs can,
 * with the right unit testing, be essentially eliminated. Allocated memory is
 * always guaranteed to be zeroed, which provides a useful deterministic
 * default for all members, and means that member pointers are initially empty
 * (null) without the need for lots of boilerplate initializers. Allocating
 * zero bytes or zero elements is always safe, on any compiler. Attempting to
 * delete an already-empty pointer is always safe: it is a no-op. Arrays of
 * pointers to objects are deleted with an explicitly-specified destructor
 * called on each element. Arrays of value-type structs can also be deleted
 * with a "cleanup" function called on each element (if needed).
 *
 * CLASSES: As with other wrappings of C, these are emulated using 'typedef
 * struct'. Class names use upper camel case; members use lower camel case.
 * Methods and static functions for a class are, of course, regular C
 * functions, and always use underscore case, with the first words being the
 * name of the class; for example, MyClass has methods and static functions
 * my_class_<fname>(). The filenames for a class are always the same as the
 * class name, i.e. in upper camel case, and are located in the lib/ directory;
 * for example, MyClass is defined in lib/MyClass.h and lib/MyClass.c. All
 * classes include at least one "constructor," with a prototype like
 *
 *   MyClass *my_class_new[_<desc>](MyClass **pthis[, args...]);
 *
 * Per the above memory paradigm, a pointer 'pthis' to the empty pointer that
 * will store the object is passed in to the constructor, so that it can be
 * both checked to be empty, as well as set to the memory location allocated.
 * Within the constructor, a simple pointer reference 'this = *pthis' is
 * generally defined and used, for convenience (e.g. this->member), and is
 * returned by the constructor for convenience. All classes also contain a
 * destructor, always with the prototype
 *
 *   void my_class_delete(MyClass **pthis);
 *
 * which destroys all of the instance's members and then frees the instance
 * itself. (This implies that a class struct must contain all information
 * needed for the destructor, such as the lengths of pointer arrays, etc.) Many
 * classes also include a "print" function
 *
 *   void my_class_print(MyClass *this, FILE *s, byte n[, args...]);
 *
 * which allows a convenient "debug print" of classes and its members, where
 * members are passed an incremented value of the indent level n
 *
 * STATIC CLASSES: These are simulated by files located in the lib/ directory
 * with underscore case, with the filenames generally being the first words of
 * each static function; for example, bit_array.c and bit_array.h define static
 * functions bit_array_<fname>(). For such static libraries, the naming
 * convention is not as strict as for classes; in some cases, it is convenient
 * to include other static functions in the same file; for example, bit_array.c
 * includes some bit_matrix_<fname>() static functions as well, and the
 * string_<fname>() functions are in files named strng.c and strng.h, to avoid
 * clashing with the system include file string.h.
 *
 * PROGRAMS: All programs have lower camel case names, and must be in the prog/
 * directory. As with libraries, [prog-name].c must include exactly one local
 * header file with #include "[prog-name].h" which contains all of its local
 * includes. The Makefile automatically figures out how to build each program,
 * in both debug and optimized modes.
 *
 * COMMAND-LINE ARGUMENTS: jpC includes a native 'args' library which provides
 * comprehensive yet simple command-line argument handling, similar to that
 * provided in many frameworks. Look at one of the programs in prog/ for an
 * example of how to use it.
 *
 * STRINGS: C doesn't have string classes, just C arrays of chars terminated by
 * the '\0' sentinel. Since efficiency is paramount, jpC doesn't create any
 * bloated class to represent a string, but simply provides many string_<...>()
 * functions in the static library strng.c and strng.h. These static functions
 * are so fundamental to jpC that strng.h is explicitly #include-d below, so
 * that it never needs to be explicitly included in any library or program.
 *
 * EXCEPTION HANDLING: jpC includes a simple yet effective simulation of
 * exceptions. The function throw() constructs, from its arguments, a
 * printf-formatted error string, with numbers five digits or longer
 * automatically reformatted to include thousands separators (where
 * appropriate). By default, throw() will print this string to stderr,
 * including the file and line of the call, and then exits the program. In
 * debug mode (specified by JPC_DEBUG being passed through from the compiler),
 * the exit actually occurs by trying to reference memory location zero, which
 * triggers a trap by ASAN, if present. and hence shows a strack trace. Without
 * the JPC_DEBUG flag being set, the program simply exits with an error code
 * via exit(1). However, if the throw() is wrapped in a try {} catch {} yrt
 * block, the exception will effectively be "caught," and the exception string
 * is made available in the global string 'exception'. If an exception is
 * subsequently rethrown in the catch {} block, then both the original
 * exception and the newly-thrown exception are printed as the program exits as
 * described above. Note that this is only a simulation of exception handling,
 * and so try {} catch {} yrt blocks cannot be nested. Also note that such a
 * try block will in general leak memory, so LSAN should be turned off when
 * executing any programs using it (e.g. death tests).
 *
 * UNIT TESTS: As noted above, unit testing in jpC was previously performed
 * using Googletest, but is now natively coded in jpC in the library 'test',
 * hewing as closely to Googletest syntax as possible. The tests with a given
 * file are all actually executed as a single program, since wrapping the
 * executable isn't feasible with a code-level C implementation; but the
 * results are remarkably faithful to how they used to run under Googletest.
 * All tests are located in the test/ directory, and named after the class or
 * static library being tested; for example, ExternalFileTest.c (for a class)
 * or strng_test.c (for a static library), each with its .h file as stipulated
 * above. Death tests make use of the simulated try / catch framework described
 * above to catch all throw()s. Such death tests are always kept in separate
 * tests ending with 'Death' or '_death' respectively; for example,
 * ExternalFileTestDeath.c and strng_test_death.c. Keeping them separate allows
 * the Makefile to run them without LSAN, while keeping LSAN on for regular
 * (non-death) tests. To make the test framework as similar to Googletest as
 * possible, the macros START and FINISH must be called around all the TEST(){}
 * blocks; these actually provide the boilerplate of the program that is
 * executed. (I may enhance this in future to run the tests more similarly to
 * how Googletest does, as parallel processes, but the current implementation
 * is good enough for current practical purposes.) Calling the macro DISABLE
 * between TESTs disables all tests until the matching ENABLE. Calling the
 * macro ASSERT_ALL turns every subsequent EXPECT[...] call into its equivalent
 * ASSERT[...] call, which is useful for providing stack traces to debug test
 * failures (only available if the program throws and is trapped by ASAN); this
 * is canceled by a NO_ASSERT_ALL. Because ANSI C does not have operator
 * overloading like C++, the _EQ tests are split into separate, type-dependent
 * calls. (If there is no test/ directory in the distribution you are looking
 * at, you can contact me for some of the unit tests, to use as examples.)
 *
 * PARALLEL PROCESSING: The 'parallel' static library and the Lock class
 * provide a useful lightweight wrapper around pthreads.
 */
#ifndef JPC_H
#define JPC_H

/* In case this gets included in a C++ program.
 */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>

/* C99 introduced other names for these. These are more convenient, and
 * follow the C# convention. A unit test confirms that sizes are correct.
 */
typedef unsigned char byte;
typedef signed char sbyte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

/* C++ added these.
 */
#ifndef __cplusplus
typedef byte bool;
#define false ((bool)0)
#define true ((bool)!false)
#endif /* !__cplusplus */

/* C89 only allows int types to be bitfields.
 */
typedef uint boolbit;

/* We don't yell about null these days.
 */
#define null NULL

/* 'this' is a keyword in C++ so it will get upset if it's used as an argument.
 */
#ifdef __cplusplus
#define this this_
#endif

/* Memory management functions, per the description above.
 *
 * new(pthis, n):
 *   Allocates an array of n objects. These are standard C arrays, so there is
 *   no length stored or bounds checking. This function automatically infers
 *   the byte-width of each element based on the type of the pointer.
 *
 * delete(pthis, n, destructor):
 *   Frees an array of object pointers, and calls 'destructor' on each non-null
 *   element, which must have signature void f(T **). If each element is an
 *   array of value type, then pass in the function 'value_array_delete' for
 *   'destructor', which frees a simple array of values.
 *
 * deletev(pthis):
 *   Frees an array of value type.
 *
 * devetevc(pthis, n, cleanup):
 *   Frees an array of value type, calling 'cleanup' on each element, which
 *   must have signature void f(T *).
 *
 * renew(pthis, old_n, new_n, destructor):
 *   Resizes the array. Note that, as with realloc() (which this wraps), the
 *   pointer pointed to by pthis may or may not change, so any references to it
 *   must be refreshed after this call. If the length of the array is reduced,
 *   'destructor' is called on the deleted elements. (Use null if the array is
 *   of value type, or value_array_delete if each element is an array of value
 *   type.) If the length of the array is increased, the new elements are
 *   zeroed. In either case, any surviving elements are never changed.
 *
 * renewv(pthis, old_n, new_n):
 *   Same as renew(), but for a value type, so that no destructor is needed
 *   even if new_n < old_n. (Convenience function for passing null as the
 *   destructor.)
 *
 * renewup(pthis, old_n, new_n):
 *   Same as renew(), but when you're definite that new_n >= old_n, so that no
 *   destructor will ever be needed. Throws if this is not true.
 *
 * donate(psrc, pdst):
 *   Gives *psrc's object to *pdst. *psrc must be non-empty and *pdst must be
 *   empty. *psrc is marked empty after the transfer.
 *
 * newm(pthis, m, n):
 *   Allocates a matrix (rectangular two-dimensional array) of m x n objects.
 *
 * deletem(pthis, m, n, destructor):
 *   Frees a matrix of objects, as per delete().
 *
* deletemv(pthis, m):
 *   Frees a matrix of values, as per deletev().
 *
 * newt3(pthis, n0, n1, n2):
 *   Allocates a 3-dimensional tensor of n0 x n1 x n2 objects.
 *
 * deletet3(pthis, n0, n1, n2, destructor):
 *   Frees a 3-dimensional tensor of objects, as per delete().
 *
 * deletet3v(pthis, m):
 *   Frees a 3-dimensional tensor of values, as per deletev().
 *
 * newt4(pthis, n0, n1, n2, n3):
 *   Allocates a 4-dimensional tensor of n0 x n1 x n2 x n3 objects.
 *
 * deletet4(pthis, n0, n1, n2, n3, destructor):
 *   Frees a 4-dimensional tensor of objects, as per delete().
 *
 * deletet4v(pthis, m):
 *   Frees a 4-dimensional tensor of values, as per deletev().
 *
 * newo(pthis):
 *   Allocates memory for a single object, typically within the constructor of
 *   a class. *pthis is set to the allocated memory location, which is also
 *   returned by the function, allowing the canonical usage
 *
 *     MyClass *this;
 *     this = newo(pthis);
 *
 *   within the constructor, where 'this' is used in the rest of the
 *   constructor. An instance variable for class ExClass must be defined via
 *
 *     MyClass *my_class = null;
 *
 *   so that it is initially empty, and the constructor is then called via
 *
 *     my_class_new[_<desc>](&my_class[, args...]);
 *
 *   The constructor will always end with
 *
 *     return this;
 *
 *   or, if there has been no need to define 'this',
 *
 *     return *pthis;
 *
 * deleteo(pthis);
 *   Frees the memory for a single object and marks the pointer as empty:
 *
 *     deleteo(pthis);
 *
 *   Note that, when used inside a class destructor, this call only frees the
 *   memory for the class struct itself; it doesn't destroy any members of it,
 *   which must be done explicitly in the destructor prior to this call. When
 *   that is needed, the destructor will start with
 *
 *     MyClass *this;
 *     this = *pthis;
 *
 *   so that member destructors can be called with code such as
 *
 *     my_other_class_delete(&this->otherClassMember);
 */
#define newo(pthis) new((pthis), 1)
#define deleteo(pthis) deleteoI((void **)(pthis), __FILE__, __LINE__)
#define new(pthis, n) \
  newI((void **)(pthis), (n) * sizeof(**(pthis)), __FILE__, __LINE__)
#define delete(pthis, n, d) \
  deleteI((void **)(pthis), (n), (void (*)(void **))(d), __FILE__, __LINE__)
#define deletev(pthis) deleteo((pthis))
#define deletevc(pthis, n, c) \
  deletevcI( \
      (void **)(pthis), \
      (n), \
      sizeof(**(pthis)), \
      (void (*)(void *))(c), \
      __FILE__, \
      __LINE__)
#define renew(pthis, old_n, new_n, d) \
  renewI( \
      (void **)(pthis), \
      (old_n), \
      (new_n), \
      sizeof(**(pthis)), \
      (d), \
      __FILE__, \
      __LINE__)
#define renewv(pthis, old_n, new_n) renew((pthis), (old_n), (new_n), null)
#define renewup(pthis, old_n, new_n) \
  renewupI( \
      (void **)(pthis), \
      (old_n), \
      (new_n), \
      sizeof(**(pthis)), \
      __FILE__, \
      __LINE__)
#define newm(pthis, n0, n1) \
  newmI((void ***)(pthis), (n0), (n1) * sizeof(***(pthis)), __FILE__, __LINE__)
#define deletem(pthis, n0, n1, d) \
  deletemI( \
      (void ***)(pthis), \
      (n0), \
      (n1), \
      (void (*)(void **))(d), \
      __FILE__, \
      __LINE__)
#define deletemv(pthis, n0) \
  deletemvI( \
      (void ***)(pthis), \
      (n0), \
      __FILE__, \
      __LINE__)
#define newt3(pthis, n0, n1, n2) \
  newt3I( \
      (void ****)(pthis), \
      (n0), \
      (n1), \
      (n2) * sizeof(****(pthis)), \
      __FILE__, \
      __LINE__)
#define deletet3(pthis, n0, n1, n2, d) \
  deletet3I( \
      (void ****)(pthis), \
      (n0), \
      (n1), \
      (n2), \
      (void (*)(void **))(d), \
      __FILE__, \
      __LINE__)
#define deletet3v(pthis, n0, n1) \
  deletet3vI( \
      (void ****)(pthis), \
      (n0), \
      (n1), \
      __FILE__, \
      __LINE__)
#define newt4(pthis, n0, n1, n2, n3) \
  newt4I( \
      (void *****)(pthis), \
      (n0), \
      (n1), \
      (n2), \
      (n3) * sizeof(****(pthis)), \
      __FILE__, \
      __LINE__)
#define deletet4(pthis, n0, n1, n2, n3, d) \
  deletet4I( \
      (void *****)(pthis), \
      (n0), \
      (n1), \
      (n2), \
      (n3), \
      (void (*)(void **))(d), \
      __FILE__, \
      __LINE__)
#define deletet4v(pthis, n0, n1, n2) \
  deletet4vI( \
      (void *****)(pthis), \
      (n0), \
      (n1), \
      (n2), \
      __FILE__, \
      __LINE__)
#define donate(psrc, pdst) \
  donateI((void **)(psrc), (void **)(pdst), __FILE__, __LINE__)
void *newI(void **pthis, size_t bytes, char *file, int line);
void deleteoI(
    void **pthis,
    char *file,
    int line);
void deleteI(
    void **pthis,
    size_t n,
    void (*destructor)(void **pthis),
    char *file,
    int line);
void deletevcI(
    void **pthis,
    size_t n,
    size_t size,
    void (*cleanup)(void *this),
    char *file,
    int line);
void *renewI(
    void **pthis,
    size_t old_n,
    size_t new_n,
    size_t size,
    void (*destructor)(void **pthis),
    char *file,
    int line);
void *renewupI(
    void **pthis,
    size_t old_n,
    size_t new_n,
    size_t size,
    char *file,
    int line);
void *donateI(void **psrc, void **pdst, char *file, int line);
void value_array_delete(void **pthis);
void *newmI(void ***pthis, size_t m, size_t bytes, char *file, int line);
void deletemI(
    void ***pthis,
    size_t m,
    size_t n,
    void (*destructor)(void **pthis),
    char *file,
    int line);
void deletemvI(
    void ***pthis,
    size_t m,
    char *file,
    int line);
void *newt3I(
    void ****pthis,
    size_t n0,
    size_t n1,
    size_t bytes,
    char *file,
    int line);
void deletet3I(
    void ****pthis,
    size_t n0,
    size_t n1,
    size_t n2,
    void (*destructor)(void **pthis),
    char *file,
    int line);
void deletet3vI(
    void ****pthis,
    size_t n0,
    size_t n1,
    char *file,
    int line);
void *newt4I(
    void *****pthis,
    size_t n0,
    size_t n1,
    size_t n2,
    size_t bytes,
    char *file,
    int line);
void deletet4I(
    void *****pthis,
    size_t n0,
    size_t n1,
    size_t n2,
    size_t n3,
    void (*destructor)(void **pthis),
    char *file,
    int line);
void deletet4vI(
    void *****pthis,
    size_t n0,
    size_t n1,
    size_t n2,
    char *file,
    int line);

/* Simulate exceptions, as described above.
 */
#define throw \
  g_JPC_internal_throw_ = false, \
  g_JPC_file_2_ = g_JPC_file_1_, \
  g_JPC_line_2_ = g_JPC_line_1_, \
  g_JPC_file_1_ = __FILE__, \
  g_JPC_line_1_ = __LINE__, \
  throwI
void throwI(char *fmt, ...);
#define try \
  if (g_JPC_try_ == 1) {g_JPC_try_ = 0; throw("Already in try");} \
  if (g_JPC_try_ == 2) {g_JPC_try_ = 0; throw("Already in catch");} \
  g_JPC_try_ = 1; switch (setjmp(g_JPC_jmp_buf_)) {case 0:
#define catch \
  if (g_JPC_try_ == 0) {g_JPC_try_ = 0; throw("Not in try");} \
  if (g_JPC_try_ == 2) {g_JPC_try_ = 0; throw("Already in catch");} \
  break; case 1: g_JPC_try_ = 2;
#define yrt \
  if (g_JPC_try_ == 0) {g_JPC_try_ = 0; throw("Not in try");} \
  if (g_JPC_try_ == 1) {g_JPC_try_ = 0; throw("No catch");} \
  break; default: throw("Uncaught throw");} g_JPC_try_ = 0;

/* Exit normally (with return code 0). If allow_exit_normally(false) has been
 * called, this will throw rather than exit normally. This allows tests to
 * prevent a library from exiting the test directly back to the operating
 * system, rather than returning control to the test. (Tests do not truly wrap
 * the process, but simulate it through the macros here and in test.h.)
 */
void exit_normally();
void allow_exit_normally(bool allow);

/* fprintfn() is fprintf with an "indent level," n. This is generally used in
 * the "debug print" function defined in many classes, as described above. If
 * the indent level gets to 255, the function dies with throw(), as that many
 * levels of nesting generally indicates that something is broken (and won't be
 * sensible to print anyway). This function also automatically "commaizes" its
 * output with thousands separators, as described above for throw(). If 'f is
 * null, the line is printed using dlog (see log.h).
 */
void fprintfn(FILE *f, byte n, char *fmt, ...);

/* Should have been in math.h from day one.
 */
#ifndef __cplusplus
double round(double x);
#endif

/* Useful macros. Note that max() and min() are not used as they are elsewhere,
 * both so that max and min can be used as variable names without confusion or
 * syntax highlighting, and also to avoid clashes with those other definitions
 * of max() and min(). modulo(a, b) works even for negative values of a, which
 * is not true for the raw C operator % because integer division / may truncate
 * towards zero (if a machine chooses to implement it that way), and % is
 * defined in terms of integer division.
 */
#define bool_string(b) ((b) ? "true" : "false")
#define plural_s(n) ((n) == 1 ? "" : "s")
#define maximum(a, b) ((a) > (b) ? (a) : (b))
#define minimum(a, b) ((a) < (b) ? (a) : (b))
#define modulo(a, b) ((((long)(a)) % ((long)(b)) + ((long)(b))) % ((long)(b)))

/* "Defrags" an array of pointers by removing any nulls and shortening the
 * array to suit. The address of the array and of its length must be passed in,
 * and are updated.
 */
#define defrag_pointer_array(pthis, pn) \
  defrag_pointer_arrayI((void ***)(pthis), (pn))

void defrag_pointer_arrayI(void ***pthis, ulong *pn);

/* Useful for debugging.
 */
#define HERE dlog("--> HERE <--");
#define TODO

/* External decalarations of variables defined in jpc.c that are used to
 * deliver all of the above trickery.
 */
#ifndef JPC_C
extern bool g_JPC_internal_throw_;
extern byte g_JPC_try_;
extern char *exception;
extern char *g_JPC_file_1_;
extern char *g_JPC_file_2_;
extern char *g_JPC_orig_exception__;
extern int g_JPC_line_1_;
extern int g_JPC_line_2_;
extern jmp_buf g_JPC_jmp_buf_;
#endif /* JPC_C */

/* Used in jpc.c. Include here to avoid dependency problems. This also implies
 * that strings are a core part of jpC.
 */
#include "strng.h"

/* Also consider logging to be a core part of jpC.
 */
#include "log.h"

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _H */

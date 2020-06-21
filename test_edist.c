#include <astrounit.h>
#include <stdlib.h>
#include "dym.h"

ASTRO_TEST_BEGIN(test_empty_strings)
{
	int dist = dym_edist("", "");
	assert_int_eq(0, dist, "Empty strings should have 0 distance");
}
ASTRO_TEST_END

ASTRO_TEST_BEGIN(test_empty_vs_nonempty)
{
	int dist = dym_edist("", "abc");
	assert_int_eq(3, dist, "Distance was not length of non-empty string");
	dist = dym_edist("abc", "");
	assert_int_eq(3, dist, "Distance was not length of non-empty string");
}
ASTRO_TEST_END

ASTRO_TEST_BEGIN(test_single_swap)
{
	int dist = dym_edist("abc", "azc");
	assert_int_eq(1, dist, "Strings should have distance 1, swap");
}
ASTRO_TEST_END

ASTRO_TEST_BEGIN(test_single_addition)
{
	int dist = dym_edist("abc", "abcd");
	assert_int_eq(1, dist, "Strings should have distance 1, addition");
}
ASTRO_TEST_END

ASTRO_TEST_BEGIN(test_complex_strings)
{
	int dist = dym_edist("kitten", "sitting");
	assert_int_eq(3, dist, "Strings should have distance 3");
}
ASTRO_TEST_END

ASTRO_TEST_BEGIN(test_complex_strings_2)
{
	int dist = dym_edist("Saturday", "Sunday");
	assert_int_eq(3, dist, "Strings should have distance 3");

	dist = dym_edist("outset", "tset");
	assert_int_eq(2, dist, "Strings should have distance 2");
}
ASTRO_TEST_END

ASTRO_TEST_BEGIN(test_damerau_levenshtein)
{
	int dist;
	dist = dym_edist("tset", "test");
	assert_int_eq(2, dist, "Strings should have distance 2");

	/* Basic tests */
	dist = dym_dl_edist("", "");
	assert_int_eq(0, dist, "Empty strings should have distance 0");
	dist = dym_dl_edist("t", "t");
	assert_int_eq(0, dist, "Strings should have distance 0");
	dist = dym_dl_edist("t", "te");
	assert_int_eq(1, dist, "Strings should have distance 1: add/delete");
	dist = dym_dl_edist("tt", "te");
	assert_int_eq(1, dist, "Strings should have distance 1: substitute");
	dist = dym_dl_edist("tts", "te");
	assert_int_eq(2, dist, "Strings should have distance 2");
	dist = dym_dl_edist("ttst", "tet");
	assert_int_eq(2, dist, "Strings should have distance 2");
	dist = dym_dl_edist("outset", "tset");
	assert_int_eq(2, dist, "Strings should have distance 2");

	/* Swap tests */
	dist = dym_dl_edist("tset", "test");
	assert_int_eq(1, dist, "Strings should have distance 1: swap");
}
ASTRO_TEST_END

int main()
{
	int num_failures = 0;
	struct astro_suite *suite = astro_suite_create();
	astro_suite_add_test(suite, test_empty_strings, NULL);
	astro_suite_add_test(suite, test_empty_vs_nonempty, NULL);
	astro_suite_add_test(suite, test_single_swap, NULL);
	astro_suite_add_test(suite, test_single_addition, NULL);
	astro_suite_add_test(suite, test_complex_strings, NULL);
	astro_suite_add_test(suite, test_complex_strings_2, NULL);
	astro_suite_add_test(suite, test_damerau_levenshtein, NULL);
	num_failures = astro_suite_run(suite);
	astro_suite_destroy(suite);
	return num_failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

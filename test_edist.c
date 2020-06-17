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
	assert_int_eq(1, dist, "Strings should have 1 distance, swap");
}
ASTRO_TEST_END

ASTRO_TEST_BEGIN(test_single_addition)
{
	int dist = dym_edist("abc", "abcd");
	assert_int_eq(1, dist, "Strings should have 1 distance, addition");
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
	num_failures = astro_suite_run(suite);
	astro_suite_destroy(suite);
	return num_failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

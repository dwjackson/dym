#include <astrounit.h>
#include <stdlib.h>
#include "dym.h"

ASTRO_TEST_BEGIN(test_empty_strings)
{
	int dist = dym_edist("", "");
	assert_int_eq(0, dist, "Empty strings should have 0 distance");
}
ASTRO_TEST_END

int main()
{
	int num_failures = 0;
	struct astro_suite *suite = astro_suite_create();
	astro_suite_add_test(suite, test_empty_strings, NULL);
	num_failures = astro_suite_run(suite);
	astro_suite_destroy(suite);
	return num_failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

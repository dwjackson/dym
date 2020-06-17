#include "dym.h"
#include <string.h>

int dym_edist(const char *s1, const char *s2)
{
	size_t s1_len = strlen(s1);
	size_t s2_len = strlen(s2);
	if (s1_len == 0 && s2_len == 0) {
		return 0;
	}
	if (s1_len == 0) {
		return s2_len;
	}
	if (s2_len == 0) {
		return s1_len;
	}
	return -1;
}

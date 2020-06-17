#include "dym.h"
#include <string.h>

int dym_edist(const char *s1, const char *s2)
{
	size_t s1_len = strlen(s1);
	size_t s2_len = strlen(s2);
	size_t i;
	int distance = 0;
	size_t len = s1_len > s2_len ? s1_len : s2_len;
	char ch1, ch2;

	if (s1_len == 0 && s2_len == 0) {
		return 0;
	}
	if (s1_len == 0) {
		return s2_len;
	}
	if (s2_len == 0) {
		return s1_len;
	}
	for (i = 0; i < len; i++) {
		ch1 = s1[i];
		ch2 = s2[i];
		if (ch1 != ch2) {
			distance++;
		}
	}

	return distance;
}

#include <string.h>
#include <stdlib.h>
#include <wctype.h>
#include "lowercase.h"

int lowercase(char *str)
{
	size_t wstr_size;
	wchar_t *wstr;
	size_t i;
	size_t len = strlen(str);
	int retval = 0;

	wstr_size = mbstowcs(NULL, str, 0);
	if (wstr_size == (size_t) -1) {
		return -1;
	}
	wstr = calloc(wstr_size + 1, sizeof(wchar_t));
	if (wstr == NULL) {
		return -1;
	}
	if (mbstowcs(wstr, str, wstr_size) == (size_t) -1) {
		retval = -1;
		goto done;
	}
	for (i = 0; i <= wstr_size; i++) {
		wstr[i] = towlower(wstr[i]);
	}
	if (wcstombs(str, wstr, len) == (size_t) -1) {
		retval = -1;
		goto done;
	}
	str[len] = '\0';

done:
	free(wstr);

	return retval;
}

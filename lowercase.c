#include <string.h>
#include <ctype.h>

void lowercase(char *str)
{
	size_t i;
	size_t len = strlen(str);
	for (i = 0; i < len; i++) {
		str[i] = tolower(str[i]);
	}
}

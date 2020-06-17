#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USAGE_FMT "USAGE: %s [ARGS...] [STRING]\n"

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf(USAGE_FMT, argv[0]);
		exit(EXIT_FAILURE);
	}

	return 0;
}

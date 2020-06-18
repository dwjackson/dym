#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "dym.h"

#define USAGE_FMT "USAGE: %s [ARGS...] [STRING]\n"
#define LINE_BUFSIZE 100
#define UNSET_DISTANCE -1

int main(int argc, char *argv[])
{
	char line[LINE_BUFSIZE];
	FILE *fp = stdin;
	char closest[LINE_BUFSIZE];
	int closest_dist = UNSET_DISTANCE;
	int dist;
	char *input;
	size_t line_len;
	int opt;
	extern int optind;
	extern char *optarg;
	char *filename;

	if (argc < 2) {
		printf(USAGE_FMT, argv[0]);
		exit(EXIT_FAILURE);
	}
	while ((opt = getopt(argc, argv, "f:")) != -1) {
		switch (opt) {
			case 'f':
				filename = optarg;
				fp = fopen(filename, "r");
				if (fp == NULL) {
					perror(filename);
					exit(EXIT_FAILURE);
				}
				break;
			default:
				break;
		}
	}

	input = argv[optind];

	while (fgets(line, LINE_BUFSIZE, fp) != NULL) {
		line_len = strlen(line);
		if (line_len == 0) {
			/* Skip blank lines */
			continue;
		}
		line[strlen(line)-1] = '\0';
		dist = dym_edist(input, line);
		if (closest_dist == UNSET_DISTANCE || dist < closest_dist) {
			closest_dist = dist;
			strcpy(closest, line);
		}
	}
	printf("%s\n", closest);

	fclose(fp);

	return 0;
}

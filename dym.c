#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "dym.h"

#define USAGE_FMT "USAGE: %s [ARGS...] [STRING]\n"
#define LINE_BUFSIZE 100
#define UNSET_DISTANCE -1
#define COUNT_MAX 1000

struct dym_match {
	int dist;
	char str[LINE_BUFSIZE];
};

static void lowercase(char *str);

int main(int argc, char *argv[])
{
	char line[LINE_BUFSIZE];
	FILE *fp = stdin;
	int dist;
	char *input;
	size_t line_len;
	int opt;
	extern int optind;
	extern char *optarg;
	char *filename;
	char *message = NULL;
	int mflag = 0;
	int vflag = 0;
	int iflag = 0;
	struct dym_match *closest = NULL;
	int count = 1;
	struct dym_match *match;
	int i;

	if (argc < 2) {
		printf(USAGE_FMT, argv[0]);
		exit(EXIT_FAILURE);
	}
	while ((opt = getopt(argc, argv, "c:f:im:v")) != -1) {
		switch (opt) {
			case 'c':
				count = atoi(optarg);
				if (count == 0 || count > COUNT_MAX) {
					printf("count must be > 0 and < %d\n", COUNT_MAX + 1);
					exit(EXIT_FAILURE);
				}
				break;
			case 'f':
				filename = optarg;
				fp = fopen(filename, "r");
				if (fp == NULL) {
					perror(filename);
					exit(EXIT_FAILURE);
				}
				break;
			case 'i':
				iflag = 1;
				break;
			case 'm':
				message = optarg;
				mflag = 1;
				break;
			case 'v':
				vflag = 1;
				break;
			default:
				exit(EXIT_FAILURE);
				break;
		}
	}

	closest = malloc(sizeof(struct dym_match) * count);
	if (closest == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	for (i = 0; i < count; i++) {
		closest[i].dist = UNSET_DISTANCE;
		memset(closest[i].str, 0, LINE_BUFSIZE);
	}

	input = argv[optind];
	if (iflag) {
		lowercase(input);
	}

	while (fgets(line, LINE_BUFSIZE, fp) != NULL) {
		line_len = strlen(line);
		if (line_len == 0) {
			/* Skip blank lines */
			continue;
		}
		line[strlen(line)-1] = '\0';
		if (iflag) {
			lowercase(line);
		}
		dist = dym_edist(input, line);

		for (i = 0; i < count; i++) {
			match = &closest[i];
			if (match->dist == UNSET_DISTANCE || dist < match->dist) {
				if (i + 1 < count) {
					/* Shift things down, lowest dist is always first */
					memmove(&closest[i+1], &closest[i], sizeof(struct dym_match) * (count - i - 1));
				}
				match->dist = dist;
				strcpy(match->str, line);
				break;
			}
		}
	}

	for (i = 0; i < count; i++) {
		match = &closest[i];
		if (match->dist == UNSET_DISTANCE) {
			break;
		}
		if (mflag) {
			printf("%s", message);
		}
		printf("%s", match->str);
		if (vflag) {
			printf(" %d", match->dist);
		}
		printf("\n");
	}

	free(closest);
	fclose(fp);

	return 0;
}

static void lowercase(char *str)
{
	size_t i;
	size_t len = strlen(str);
	for (i = 0; i < len; i++) {
		str[i] = tolower(str[i]);
	}
}

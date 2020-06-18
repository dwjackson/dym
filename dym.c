#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "dym.h"

#define USAGE_FMT "USAGE: %s [ARGS...] [STRING]\n"
#define LINE_BUFSIZE 100
#define UNSET_DISTANCE -1
#define COUNT_MAX 1000
#define VERSION "0.1.0"

struct dym_match {
	int dist;
	char str[LINE_BUFSIZE];
};

static void help(const char *progname);
static char *next();

int eflag = 0;
const char *explicit_list = NULL;
int fflag = 0;
int iflag = 0;
FILE *fp = NULL;
char delim = ' ';

int main(int argc, char *argv[])
{
	char line[LINE_BUFSIZE];
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
	struct dym_match *closest = NULL;
	int count = 1;
	struct dym_match *match;
	int i;

	if (argc < 2) {
		printf(USAGE_FMT, argv[0]);
		exit(EXIT_FAILURE);
	}
	fp = stdin;
	while ((opt = getopt(argc, argv, "c:e:f:F:him:vV")) != -1) {
		switch (opt) {
			case 'c':
				count = atoi(optarg);
				if (count == 0 || count > COUNT_MAX) {
					printf("count must be > 0 and < %d\n", COUNT_MAX + 1);
					exit(EXIT_FAILURE);
				}
				break;
			case 'e':
				eflag = 1;
				explicit_list = optarg;
				break;
			case 'f':
				fflag = 1;
				filename = optarg;
				break;
			case 'F':
				delim = optarg[0];
				break;
			case 'h':
				help(argv[0]);
				exit(EXIT_SUCCESS);
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
			case 'V':
				printf("%s version %s\n", argv[0], VERSION);
				exit(EXIT_SUCCESS);
				break;
			default:
				exit(EXIT_FAILURE);
				break;
		}
	}

	if (eflag && fflag) {
		printf("Cannot pass explicit list and open file\n");
		exit(EXIT_FAILURE);
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

	if (fflag) {
		fp = fopen(filename, "r");
		if (fp == NULL) {
			perror(filename);
			exit(EXIT_FAILURE);
		}
	}

	while (next(line) != NULL) {
		line_len = strlen(line);
		if (line_len == 0) {
			/* Skip blank lines */
			continue;
		}
		if (line[line_len-1] == '\n') {
			line[line_len-1] = '\0';
		}
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

	if (mflag) {
		printf("%s\n", message);
	}
	for (i = 0; i < count; i++) {
		match = &closest[i];
		if (match->dist == UNSET_DISTANCE) {
			break;
		}
		if (mflag) {
			printf("\t");
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

static void help(const char *progname)
{
	printf(USAGE_FMT, progname);
	printf("Options:\n");
	printf("\t-c [COUNT] print up to COUNT closest matches\n");
	printf("\t-f [FILE] read possible matches from file vs. stdin\n");
	printf("\t-h show this help text\n");
	printf("\t-i ignore case--i.e. enable case-insensitive mode\n");
	printf("\t-m [MESSAGE] print MESSAGE before the match(es)\n");
	printf("\t-v verbose mode, print edit distance of each closest match\n");
	printf("\t-V print version number\n");
}

static char *next(char line[LINE_BUFSIZE])
{
	int ch;
	int i = 0;
	if (eflag) {
		if (*explicit_list == '\0') {
			return NULL;
		}
		while ((ch = *explicit_list++) != '\0' && ch != delim && i < LINE_BUFSIZE - 2) {
			line[i++] = ch;
		}
		if (ch == '\0') {
			explicit_list--;
		}
		line[i] = '\0';
		return line;
	}
	return fgets(line, LINE_BUFSIZE, fp);
}

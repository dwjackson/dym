#include "dym.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>
#include <stdarg.h>
#include <errno.h>

#define LINE_BUFSIZE 100
#define UNSET_DISTANCE -1
#define COUNT_MAX 1000

struct dym_match {
	int dist;
	char str[LINE_BUFSIZE];
};

static void usage(const char *progname);
static void help(const char *progname);
static size_t next();
static void fatal(const char *fmt, ...);

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
	char *filename = NULL;
	char *message = NULL;
	int dflag = 0;
	int Dflag = 0;
	int mflag = 0;
	int vflag = 0;
	int Fflag = 0;
	struct dym_match *closest = NULL;
	int count = 1;
	struct dym_match *match;
	int i;

	setlocale(LC_ALL, "");

	if (argc < 2) {
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}
	fp = stdin;
	while ((opt = getopt(argc, argv, "c:Dde:f:F:him:vV")) != -1) {
		switch (opt) {
			case 'c':
				count = atoi(optarg);
				if (count == 0 || count > COUNT_MAX) {
					fatal("count must be between 1 and %d", COUNT_MAX);
				}
				break;
			case 'd':
				dflag = 1;
				break;
			case 'D':
				Dflag = 1;
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
				if (strlen(optarg) != 1) {
					fatal("Delimiter must be 1 char");
				}
				delim = optarg[0];
				Fflag = 1;
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
				printf("%s version %s\n", argv[0], DYM_VERSION);
				exit(EXIT_SUCCESS);
				break;
			default:
				exit(EXIT_FAILURE);
				break;
		}
	}

	if (eflag && fflag) {
		fatal("Cannot use both explicit list and file name");
	}
	if (dflag && vflag) {
		fatal("Cannot mix the -d and -v flags because they conflict");
	}
	if (Fflag && !eflag) {
		fatal("Cannot use custom delimiter without an explicit list");
	}

	closest = calloc(count, sizeof(struct dym_match));
	if (closest == NULL) {
		fatal("calloc");
	}
	for (i = 0; i < count; i++) {
		closest[i].dist = UNSET_DISTANCE;
	}

	input = argv[optind];
	if (iflag) {
		if (lowercase(input) != 0) {
			fatal("Could not format string as lowercase: %s", input);
		}
	}

	if (fflag) {
		fp = fopen(filename, "r");
		if (fp == NULL) {
			fatal(filename);
		}
	}

	while ((line_len = next(line)) != 0) {
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
		if (Dflag) {
			dist = dym_dl_edist(input, line);
		} else {
			dist = dym_edist(input, line);
		}

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
		if (dflag) {
			printf("%d", match->dist);
		} else {
			printf("%s", match->str);
			if (vflag) {
				printf(" %d", match->dist);
			}
		}
		printf("\n");
	}

	free(closest);
	fclose(fp);

	return 0;
}

static void help(const char *progname)
{

	usage(progname);
	printf("Options:\n");
	printf("\t-c [COUNT] print up to COUNT closest matches\n");
	printf("\t-D use the Damerau-Levenshtein distance\n");
	printf("\t-d print only the edit distance to closest match(es)\n");
	printf("\t-e [EXPLICIT_LIST] pass valid word list explicitly\n");
	printf("\t-F [DELIM] set the explicit list delimiter (default is space)\n");
	printf("\t-f [FILE] read possible matches from file vs. stdin\n");
	printf("\t-h show this help text\n");
	printf("\t-i ignore case--i.e. enable case-insensitive mode\n");
	printf("\t-m [MESSAGE] print MESSAGE before the match(es)\n");
	printf("\t-v verbose mode, print edit distance of each closest match\n");
	printf("\t-V print version number\n");
}

static size_t next(char line[LINE_BUFSIZE])
{
	int ch;
	size_t i = 0;
	if (eflag) {
		if (*explicit_list == '\0') {
			return 0;
		}
		while ((ch = *explicit_list++) != '\0' && ch != delim && i < LINE_BUFSIZE - 2) {
			line[i++] = ch;
		}
		if (ch == '\0') {
			explicit_list--;
		}
		line[i] = '\0';
		return i-1;
	}
	if (fgets(line, LINE_BUFSIZE, fp) == NULL) {
		return 0;
	}
	return strlen(line);
}

static void usage(const char *progname)
{
	printf("USAGE: %s [ARGS...] [STRING]\n", progname);
	printf("To see all options: %s -h\n", progname);
}

static void fatal(const char *fmt, ...)
{
	va_list ap;

	if (errno != 0) {
		perror(fmt);
	} else {
		va_start(ap, fmt);
		printf("ERROR: ");
		vprintf(fmt, ap);
		printf("\n");
	}

	exit(EXIT_FAILURE);
}

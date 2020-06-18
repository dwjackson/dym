#include "dym.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h> // DEBUG

static int levenshtein(const char *s1, size_t l1, const char *s2, size_t l2);
static int indicator(char a, char b);
static int max(int x, int y);
static int min(int x, int y);
static int min3(int x, int y, int z);
static size_t levindex(size_t i, size_t j, size_t l);

int dym_edist(const char *s1, const char *s2)
{
	size_t l1 = strlen(s1);
	size_t l2 = strlen(s2);
	return levenshtein(s1, l1, s2, l2);
}

static int levenshtein(const char *s1, size_t l1, const char *s2, size_t l2)
{
	size_t i, j;
	size_t lev_size = (l1 + 1) * (l2 + 1);
	size_t idx;
	size_t distance;
	int *lev;

	if (l1 == 0) {
		return l2;
	}
	if (l2 == 0) {
		return l1;
	}

	lev = malloc(lev_size * sizeof(int));
	if (lev == NULL) {
		return -1;
	}
	memset(lev, -1, lev_size * sizeof(int));

	for (i = 0; i <= l1; i++) {
		for (j = 0; j <= l2; j++) {
			idx = levindex(i, j, l1);
			if (min(i, j) == 0) {
				lev[idx] = max(i, j);
			} else {
				lev[idx] = min3(
					lev[levindex(i-1,j,l1)]+1,
					lev[levindex(i,j-1,l1)]+1,
					lev[levindex(i-1,j-1,l1)] 
						+ indicator(s1[i-1], s2[j-1]));
			}
		}
	}

	distance = lev[levindex(l1, l2, l1)];

	free(lev);

	return distance;
}

static int indicator(char a, char b)
{
	return a == b ? 0 : 1;
}

static int max(int x, int y)
{
	return x > y ? x : y;
}

static int min(int x, int y)
{
	return x < y ? x : y;
}

static int min3(int x, int y, int z)
{
	return min(x, min(y, z));
}

static size_t levindex(size_t i, size_t j, size_t l)
{
	size_t index = i * l + j;
	return index;
}

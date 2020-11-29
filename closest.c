#include "dym.h"
#include "lowercase.h"
#include <stdlib.h>
#include <string.h>

#define UNSET_DISTANCE -1

static void shift_down(struct dym_match *closest, int count, int i);

struct dym_match *dym_closest_create(size_t count)
{
	return calloc(count, sizeof(struct dym_match));
}

void dym_closest_destroy(struct dym_match *closest)
{
	free(closest);
}

int dym_closest(struct dym_ops *ops, const char *input, struct dym_match *closest, int count)
{
	int i;
	char line[DYM_LINE_BUFSIZE];
	int line_len;
	int dist;
	struct dym_match *match;
	int found = 0;

	for (i = 0; i < count; i++) {
		closest[i].dist = UNSET_DISTANCE;
	}

	while ((line_len = ops->next(line)) >= 0) {
		if (line_len == 0) {
			/* Skip blank lines */
			continue;
		}
		if (line[line_len-1] == '\n') {
			line[line_len-1] = '\0';
		}
		if (line_len > DYM_LINE_BUFSIZE - 1) {
			/* Line is too long, ignore it */
			continue;
		}
		dist = ops->edist(input, line);

		for (i = 0; i < count; i++) {
			match = &closest[i];
			if (match->dist == UNSET_DISTANCE || dist < match->dist) {
				if (match->dist == UNSET_DISTANCE) {
					found++;
				}
				if (i + 1 < count) {
					shift_down(closest, count, i);
				}
				match->dist = dist;
				strcpy(match->str, line);
				break;
			}
		}
	}

	return found;
}

/* Shift things down, lowest dist is always first */
static void shift_down(struct dym_match *closest, int count, int i)
{
	size_t s = sizeof(struct dym_match) * (count - i - 1);
	memmove(&closest[i+1], &closest[i], s);
}

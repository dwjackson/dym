#ifndef DYM_H
#define DYM_H

#include <stdlib.h>

#define DYM_LINE_BUFSIZE 100

struct dym_ops {
	int (*next)(char line[DYM_LINE_BUFSIZE]);
	int (*edist)(const char *s1, const char *s2);
};

struct dym_match {
	int dist;
	char str[DYM_LINE_BUFSIZE];
};

int dym_edist(const char *s1, const char *s2);
int dym_dl_edist(const char *s1, const char *s2);
struct dym_match *dym_closest_create(size_t count);
void dym_closest_destroy(struct dym_match *closest);
int dym_closest(struct dym_ops *ops, const char *input, struct dym_match *closest, int count);

#endif /* DYM_H */

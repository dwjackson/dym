#ifndef DYM_H
#define DYM_H

struct dym_ops {
	int (*edist)(const char *s1, const char *s2);
};

int dym_edist(const char *s1, const char *s2);
int dym_dl_edist(const char *s1, const char *s2);

#endif /* DYM_H */

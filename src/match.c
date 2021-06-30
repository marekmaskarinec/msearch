#include <string.h>

#include "msearch.h"

extern menu m;

int match(char *query, char **out, int n) {
	unsigned long querylen = strlen(query);
	int outc = 0;

	if (!querylen)
		return 0;

	for (item *i = m.items; i && outc < n; i = i->next) {
		if (!i->content)
			continue;

		if (strlen(i->content) < querylen)
			continue;

		if (strncmp(query, i->content, querylen))
			continue;

		bool duplicate = false;
		for (int j=0; j < outc; j++) {
			if (strcmp(out[j], i->content) == 0) {
				duplicate = true;
				break;
			}
		}

		if (duplicate)
			continue;

		out[outc] = i->content;
		outc++;

		if (outc >= n)
			break;
	}

	return outc;
}

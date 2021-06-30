#include <stddef.h>
#include <stdlib.h>
#include <time.h>

#include "msearch.h"

extern menu m;

double get_clock() {
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    return (double)t.tv_sec + (double)t.tv_nsec * 1e-9;
}

void die() {
	while (m.items) {
		item *next = m.items->next;
		free(m.items->content);
		free(m.items);
		m.items = next;
	}
	m.items = NULL;

	exit(0);
}


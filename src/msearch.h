#ifndef MSEARCH_H
#define MSEARCH_H

#include <stdbool.h>

#define BUFFER_SIZE 512

typedef struct _item {
	char *content;
	struct _item *next;
} item;

typedef struct {
	// items
	item *items;
	char *results[256];
	int resultc;
	int max_results;
	int selection;

	// input
	char buffer[BUFFER_SIZE];
	int buf_len;
	int cursor;
	bool shift;
} menu;

char uppercase(char c);
int match(char *query, char **out, int n); // returns max of n items that match a query
double get_clock(); // returns time since start in seconds
void die(); // dies
void draw(); // draws
void load_items();

#endif
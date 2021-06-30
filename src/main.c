#include <ctype.h>
#include <stdbool.h>
#include <X11/Xlib.h>

#define CNFG_IMPLEMENTATION
#define CNFGOGL
#include "../rawdraw/CNFG.h"

#include "msearch.h"
#include "config.h"

// window
short w, h;
menu m = {0};

void HandleMotion( int x, int y, int mask ) { }
void HandleDestroy() {
	die();
}

void load_items() {
	item *i = malloc(sizeof(item));
	m.items = i;

	char buf[BUFSIZ];
	fgets(buf, sizeof(buf), stdin);
	while (strlen(buf) && buf[strlen(buf)-1] == '\n') {
		i->content = calloc(sizeof(char), strlen(buf) + 1);
		if (!i->content)
			continue;

		strcpy(i->content, buf);

		i->next = malloc(sizeof(item));
		i = i->next;

		memset(buf, 0, sizeof(buf));

		fgets(buf, sizeof(buf), stdin);
	}
	i->next = NULL;
}

void draw() {
	CNFGColor(color); 

	CNFGPenX = padding; CNFGPenY = padding;
	CNFGDrawText(m.buffer, font_size);
	if ((long)(get_clock()) % 2) {
		CNFGTackRectangle(padding + m.cursor * 3 * font_size,
			padding,
			font_size/4 + m.cursor * 3 * font_size + padding,
			padding + 6 * font_size);
	}

	if (m.selection > m.resultc - 1 && m.resultc)
		m.selection = m.resultc - 1;

	for (int i=0; i < m.resultc; i++) {
		if (m.selection == i) {
			CNFGTackRectangle(padding/8 * 7,
				(i + 1) * 6 * font_size + padding/8 * 7,
				w - padding/8 * 7,
				(i + 2) * 6 * font_size + padding/8 * 7);
			CNFGColor(bg_color);
		}

		CNFGPenX = padding; CNFGPenY = (i + 1) * 6 * font_size + padding;
		CNFGDrawText(m.results[i], font_size);
		CNFGColor(color);
	}
}

int main(int argc, char *argv[]) {
	load_items();

	CNFGSetupWMClass("msearch", win_size, win_size, "msearch", "msearch");

	CNFGSetLineWidth(font_thickness);

	int snum = DefaultScreen(CNFGDisplay);
	int displayw = DisplayWidth(CNFGDisplay, snum);
	int displayh = DisplayHeight(CNFGDisplay, snum);
	XWindowChanges changes = (XWindowChanges){.x = (displayw-win_size)/2, .y = (displayh-win_size)/2, .width = win_size, .height = win_size};

	for (;;) {
		CNFGBGColor = bg_color;

		XConfigureWindow(CNFGDisplay, CNFGWindow, 1<<0 | 1<<1 | 1<<2 | 1<<3, &changes);
		CNFGClearFrame();
		CNFGHandleInput();
		CNFGGetDimensions(&w, &h);

		m.max_results = (h - padding * 2) / (6 * font_size) - 1;
		if (m.max_results >= 256)
			m.max_results = 255;

		draw();

		CNFGSwapBuffers();
	}
}

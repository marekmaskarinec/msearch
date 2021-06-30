#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

unsigned color = 0xafaf87ff;
unsigned bg_color = 0x262626ff;

short font_thickness = 2;
short font_size = 4;

short padding = 20; // padding of all content
short win_size = 200; // size of the window. both w and h

bool keep_window = true; // if true, window can't be resized and moved from the center

#endif

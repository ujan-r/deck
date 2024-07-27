#include <assert.h>
#include <curses.h>
#include <string.h>

#include "display.h"
#include "render.h"
#include "slide.h"

void renderSlide(Slide slide, Display *display) {
    int count = slide.length;

    memset(display->cells, 0, display->width * display->height * sizeof *display->cells);

    size_t row = 0, col = 0;
    for (size_t i = 0; i < count && row < display->height; i++) {
        Element element = slide.elements[i];

        char const *start = element.text;
        for (char const *c = start; *c != '\0'; c++) {
            if (*c == '\n') {
                row++;
                col = 0;
                continue;
            }

            if (col >= display->width) {
                row++;
                col = 0;
            }

            if (row >= display->height) {
                break;
            }

            chtype cell = *c;
	    switch (element.type) {
	        case HEADING:
                    cell |= A_BOLD;
                    break;
                case CODE:
                    cell |= COLOR_PAIR(1);
                    break;
                default:
                    break;
            }
            display->cells[row * display->width + col++] = cell;
        }

        row++;
        col = 0;

        if (element.type == HEADING) {
            row++;
        }
    }
}

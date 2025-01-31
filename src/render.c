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

        size_t startCol;
        if (element.type == HEADING && element.width < display->width) {
            startCol = (display->width - element.width) / 2;
        } else {
            startCol = 0;
        }

        col = startCol;

        for (char const *c = element.text; *c != '\0'; c++) {
            if (*c == '\n') {
                row++;
                col = startCol;
                continue;
            }

            if (col >= display->width) {
                row++;
                col = startCol;
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

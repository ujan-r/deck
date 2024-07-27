#include <curses.h>
#include <stdio.h>
#include <stdlib.h>

#include "deck.h"
#include "display.h"
#include "parser.h"
#include "present.h"
#include "render.h"
#include "utils.h"

Display display;
Deck deck;
size_t currentSlide = 0;

static void gotoSlide(size_t);
static void refreshScreen(void);

void present(char const *text) {
    parseDeck(text, &deck);

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    start_color();

#ifdef NCURSES_VERSION
    use_default_colors();
    #define COLOR_DEFAULT -1
#else
    #define COLOR_DEFAULT 0
#endif

    init_pair(1, COLOR_GREEN, COLOR_DEFAULT);

    getmaxyx(stdscr, display.height, display.width);
    display.cells = calloc(display.width * display.height, sizeof *display.cells);
    if (!display.cells) {
        endwin();
        exitWithError("memory allocation failed\n");
    }

    gotoSlide(0);

    int c;
    while ((c = getch()) != 'q') {
        switch (c) {
            case ' ': case KEY_RIGHT:
                gotoSlide(currentSlide + 1);
                break;
            case KEY_LEFT:
                gotoSlide(currentSlide - 1);
                break;
        }
    }

    endwin();
}

static void gotoSlide(size_t index) {
    currentSlide = index < deck.length ? index : currentSlide;
    refreshScreen();
}

static void refreshScreen(void) {
    renderSlide(deck.slides[currentSlide], &display);

    for (size_t row = 0; row < display.height; row++) {
        for (size_t col = 0; col < display.width; col++) {
            chtype c = display.cells[row * display.width + col];
            mvaddch(row, col, c & A_CHARTEXT ? c : ' ');
        }
    }
    refresh();
}

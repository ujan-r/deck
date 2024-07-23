#include <curses.h>

#include "present.h"

void present(char const *text) {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    printw("%s", text);
    refresh();

    int c;
    while ((c = getch()) != 'q') {}

    endwin();
}

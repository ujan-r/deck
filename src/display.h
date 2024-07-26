#ifndef INCLUDE_GUARD_FC555D26_2ECE_4106_8E9A_4B34829DF67C
#define INCLUDE_GUARD_FC555D26_2ECE_4106_8E9A_4B34829DF67C

#include <curses.h>
#include <stddef.h>

typedef struct display {
    size_t width;
    size_t height;
    chtype *cells;
} Display;

#endif /* !INCLUDE_GUARD_FC555D26_2ECE_4106_8E9A_4B34829DF67C */

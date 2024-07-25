#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "element.h"
#include "lexer.h"

static bool readHeading(char const **, Element *);
static bool readParagraph(char const **, Element *);
static bool readCodeBlock(char const **, Element *);

char * readLine(char const **);
static void skipBlankLines(char const **);

Element const *lex(char const *text, size_t *slideCount) {
    *slideCount = 1;
    size_t n = 0, capacity = 8;

    Element *elements = calloc(capacity, sizeof *elements);
    if (!elements) {
        fprintf(stderr, "memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    while (*text) {
        if (n >= capacity) {
            capacity *= 2;
            Element *p = realloc(elements, capacity * sizeof *elements);
            if (!p) {
                fprintf(stderr, "memory allocation failed\n");
                exit(EXIT_FAILURE);
            }
            elements = p;
        }

        skipBlankLines(&text);
        if (!*text) { break; }

        if (strncmp(text, "---\n", 4) == 0) {
            readLine(&text);
            elements[n++].type = SEP;
            ++*slideCount;
            continue;
        }

        if (readHeading(&text, &elements[n]))   { n++; continue; }
        if (readCodeBlock(&text, &elements[n])) { n++; continue; }
        if (readParagraph(&text, &elements[n])) { n++; continue; }

        fprintf(stderr, "unexpected character: %c\n", *text);
        exit(EXIT_FAILURE);
    }

    elements[n].type = END;
    return elements;
}

static bool readHeading(char const **text, Element *element) {
    assert(text && *text && **text);

    char const *start = *text;

    int level = 0;
    while (**text == '#' && level < 7) {
        ++*text;
        level++;
    }

    if (level == 0 || level > 6 || **text != ' ') {
        *text = start;
        return false;
    }

    while (isblank(**text)) {
        ++*text;
    }

    element->type = HEADING;
    element->text = readLine(text);
    return true;
}

static bool readCodeBlock(char const **text, Element *element) {
    assert(text && *text && **text);

    if (strncmp(*text, "```", 3) != 0) {
        return false;
    }

    readLine(text);

    char const *start = *text;
    while (**text && strncmp(*text, "```\n", 4) != 0) {
        readLine(text);
    }

    size_t length = *text - start;

    readLine(text);

    char *string = calloc(length + 1, sizeof *element->text);
    if (!string) {
        fprintf(stderr, "memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    element->type = CODE;
    element->text = memcpy(string, start, length);
    return true;
}

static bool readParagraph(char const **text, Element *element) {
    assert(text && *text && **text);

    char const *start = *text;
    while (**text && **text != '\n') {
        readLine(text);
    }

    size_t length = *text - start;

    readLine(text);

    char *string = calloc(length + 1, sizeof *element->text);
    if (!string) {
        fprintf(stderr, "memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    element->type = PARAGRAPH;
    element->text = memcpy(string, start, length);
    return true;
}

char * readLine(char const **text) {
    assert(text && *text);

    char const *start = *text;
    size_t length = 0;

    while (**text && **text != '\n') {
        ++*text;
        length++;
    }

    if (**text == '\n') {
        ++*text;
    }

    char *string = calloc(length + 1, sizeof *string);
    if (!string) {
        fprintf(stderr, "memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    return memcpy(string, start, length);
}

static void skipBlankLines(char const **text) {
    assert(text && *text);

    while (**text == '\n') {
        ++*text;
    }
}

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "element.h"
#include "lexer.h"
#include "utils.h"

static bool readHeading(char const **, Element *);
static bool readParagraph(char const **, Element *);
static bool readCodeBlock(char const **, Element *);

char * readLine(char const **, size_t *width);
static void skipBlankLines(char const **);

Element const *lex(char const *text, size_t *slideCount) {
    *slideCount = 1;
    size_t n = 0, capacity = 8;

    Element *elements = calloc(capacity, sizeof *elements);
    if (!elements) {
        exitWithError("memory allocation failed\n");
    }

    while (*text) {
        if (n >= capacity) {
            capacity *= 2;
            Element *p = realloc(elements, capacity * sizeof *elements);
            if (!p) {
                exitWithError("memory allocation failed\n");
            }
            elements = p;
        }

        skipBlankLines(&text);
        if (!*text) { break; }

        if (strncmp(text, "---\n", 4) == 0) {
            readLine(&text, NULL);
            elements[n++].type = SEP;
            ++*slideCount;
            continue;
        }

        if (readHeading(&text, &elements[n]))   { n++; continue; }
        if (readCodeBlock(&text, &elements[n])) { n++; continue; }
        if (readParagraph(&text, &elements[n])) { n++; continue; }

        exitWithError("unexpected character: %c\n", *text);
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
    element->text = readLine(text, &element->width);
    return true;
}

static bool readCodeBlock(char const **text, Element *element) {
    assert(text && *text && **text);

    size_t width = 0, temp;

    if (strncmp(*text, "```", 3) != 0) {
        return false;
    }

    readLine(text, NULL);

    char const *start = *text;
    while (**text && strncmp(*text, "```\n", 4) != 0) {
        readLine(text, &temp);
        width = temp > width ? temp : width;
    }

    size_t length = *text - start;

    readLine(text, NULL);

    char *string = calloc(length + 1, sizeof *element->text);
    if (!string) {
        exitWithError("memory allocation failed\n");
    }

    element->type = CODE;
    element->text = memcpy(string, start, length);
    element->width = width;
    return true;
}

static bool readParagraph(char const **text, Element *element) {
    assert(text && *text && **text);

    size_t width = 0, temp;

    char const *start = *text;
    while (**text && **text != '\n') {
        readLine(text, &temp);
        width = temp > width ? temp : width;
    }

    size_t length = *text - start;

    readLine(text, NULL);

    char *string = calloc(length + 1, sizeof *element->text);
    if (!string) {
        exitWithError("memory allocation failed\n");
    }

    element->type = PARAGRAPH;
    element->text = memcpy(string, start, length);
    element->width = width;
    return true;
}

char * readLine(char const **text, size_t *width) {
    assert(text && *text);

    if (!width) {
        size_t temp;
        return readLine(text, &temp);
    }

    char const *start = *text;
    *width = 0;

    while (**text && **text != '\n') {
        ++*text;
        ++*width;
    }

    if (**text == '\n') {
        ++*text;
    }

    char *string = calloc(*width + 1, sizeof *string);
    if (!string) {
        exitWithError("memory allocation failed\n");
    }

    return memcpy(string, start, *width);
}

static void skipBlankLines(char const **text) {
    assert(text && *text);

    while (**text == '\n') {
        ++*text;
    }
}

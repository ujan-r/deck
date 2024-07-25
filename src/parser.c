#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deck.h"
#include "element.h"
#include "lexer.h"
#include "parser.h"
#include "slide.h"
#include "utils.h"

static Slide parseSlide(Element const *, Element const **next);

void parseDeck(char const *text, Deck *deck) {
    Element const *elements = lex(text, &deck->length);

    deck->slides = calloc(deck->length, sizeof *deck->slides);
    if (!deck->slides) {
        exitWithError("memory allocation failed\n");
    }

    size_t n = 0;
    while (elements) {
        deck->slides[n++] = parseSlide(elements, &elements);
    }
}

static Slide parseSlide(Element const *elements, Element const **next) {
    assert(elements);

    size_t count = 0;
    Element const *e;
    for (e = elements; e->type != SEP && e->type != END; e++) {
        count++;
    }

    *next = e->type == SEP ? e + 1 : NULL;

    Element *array = calloc(count, sizeof *array);
    if (!array) {
        exitWithError("memory allocation failed\n");
    }

    memcpy(array, elements, count * sizeof *array);

    return (Slide){array, count};
}

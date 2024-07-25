#ifndef INCLUDE_GUARD_1AC4797E_80D3_4986_B2D6_BCA926570B4C
#define INCLUDE_GUARD_1AC4797E_80D3_4986_B2D6_BCA926570B4C

typedef enum element_type {
    PARAGRAPH,
    HEADING,
    CODE,
    SEP,
    END
} ElementType;

typedef struct element {
    ElementType type;
    char const *text;
} Element;

#endif /* !INCLUDE_GUARD_1AC4797E_80D3_4986_B2D6_BCA926570B4C */

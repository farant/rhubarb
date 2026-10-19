/* typus.c - Fixture pro typi (struct, enum, typedef) */

struct Punctum {
    int x;
    int y;
};

union Valor {
    int i;
    float f;
};

enum Color {
    COLOR_RUBRUM,
    COLOR_VIRIDIS,
    COLOR_CAERULEUM
};

typedef int Integer32;
typedef struct Punctum Punctum;

struct Punctum creare_punctum(int x, int y)
{
    struct Punctum p;
    p.x = x;
    p.y = y;
    return p;
}

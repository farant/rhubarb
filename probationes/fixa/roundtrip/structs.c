/* Test file for structs, unions, and enums */

/* Simple struct */
struct Point
{
    int x;
    int y;
};

/* Struct with multiple types */
struct Data
{
    int count;
    float value;
    char name;
    long size;
};

/* Anonymous struct variable */
struct { int a; int b; } pair;

/* Union */
union Value
{
    int i;
    float f;
    char c;
};

/* Enum */
enum Color { RED, GREEN, BLUE };

/* Enum with values */
enum Status
{
    OK = 0,
    ERROR = 1,
    PENDING = 2
};

/* Typedef struct */
typedef struct Vec3
{
    float x;
    float y;
    float z;
} Vec3;

/* Global struct variable */
struct Point origin;

/* Global with initializer */
struct Point center = {0, 0};

/* Function taking struct pointer */
void move_point(struct Point *p, int dx, int dy)
{
    p->x = p->x + dx;
    p->y = p->y + dy;
}

/* Function returning struct by value */
struct Point make_point(int x, int y)
{
    struct Point p;
    p.x = x;
    p.y = y;
    return p;
}

/* Function with enum parameter */
int color_to_int(enum Color c)
{
    if (c == RED)
    {
        return 0;
    }
    else if (c == GREEN)
    {
        return 1;
    }
    else
    {
        return 2;
    }
}

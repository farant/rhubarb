struct Point { int x; int y; };
enum Color { RED, GREEN, BLUE };
union Data { int i; float f; };
void f(void) {
    struct Point p;
    struct Point *ptr;
    struct Point **pp;
    enum Color c;
    union Data d;
    union Data *dp;
}

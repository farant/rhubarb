struct Point { int x; int y; };
void f(void) {
    struct Point p;
    struct Point *ptr;
    int a = p. /* dot */ x;
    int b = ptr-> /* arrow */ y;
}

struct S { int x; };
void f(struct S *s, int *y) {
    *y = s->x;
}

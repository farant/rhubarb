struct Point { int x; int y; };
void move(struct Point *p, int dx) {
    p->x = p->x + dx;
}

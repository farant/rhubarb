struct Point { int x; int y; };
void foo(struct Point *p) { }
void bar(struct Point p) { }
union Val { int i; float f; };
void baz(union Val v) { }
enum Color { RED, GREEN, BLUE };
void qux(enum Color c) { }

struct Point { int int x; int int y; };
void foo(struct Point *p) { }
void bar(struct Point p) { }
union Val { int int i; float float f; };
void baz(union Val v) { }
enum Color { RED, GREEN, BLUE };
void qux(enum Color c) { }

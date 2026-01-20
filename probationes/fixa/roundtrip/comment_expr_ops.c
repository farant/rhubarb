void f(void) {
    int a = 1 /* before op */ + 2;
    int b = 1 + /* after op */ 2;
    int c = 1 /* both */ + /* sides */ 2;
    int d = 1 * /* mul */ 2 + /* add */ 3;
}

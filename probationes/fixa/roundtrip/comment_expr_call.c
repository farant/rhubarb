int add(int a, int b) { return a + b; }
void f(void) {
    int x = add( /* first */ 1, /* second */ 2);
    int y = add(1 /* after1 */, 2 /* after2 */);
}

void f(void) {
    int a = ( /* inside open */ 1 + 2);
    int b = (1 + 2 /* before close */ );
    int c = ( /* open */ 1 /* mid */ + /* op */ 2 /* close */ );
}

/* NOTE: Cast expressions in assignment RHS (e.g. "long a = (long)x;")
 * don't parse yet. Only statement-level casts work: */
void f(void) {
    int x = 1;
    (void)x;  /* statement-level cast works */
}

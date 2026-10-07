/*
 * d112_overflow_probe.c - provenance tool for Demo 114 (not part of the
 * demo build): runs Demo 112's own crossing + determinant + Alexander code
 * on ONE construction-word alternative of its 6_3 polygon, compiled with
 * the signed-overflow sanitizer set to abort on the first overflow.
 *
 * UBSan reports each source location only once per process, so counting
 * reports in one run of all 256 alternatives undercounts badly (it shows
 * 1). One process per alternative:
 *
 *   cc -std=c89 -w -O1 -fsanitize=signed-integer-overflow \
 *      -fno-sanitize-recover=signed-integer-overflow \
 *      -o d112_overflow_probe d112_overflow_probe.c
 *   for c in $(seq 0 255); do ./d112_overflow_probe $c >/dev/null 2>&1 \
 *      || echo $c; done | wc -l          # -> 221 (2026-10-07)
 */
#define main d112_main
#include "../demo_112_alexander_polynomial/main.c"
#undef main

int main(int argc, char **argv)
{
    BraidWord b = braid_6_3();
    StickKnot k;
    Vec3Q alt[MAX_STICK_VERTICES];
    Crossing cx[MAX_CROSSINGS];
    int n, nc;
    RatPoly a;
    unsigned c;

    if (argc < 2) return 2;
    c = (unsigned)atoi(argv[1]);
    braid_to_polygon(&b, &k);
    simplify_polygon_topology(&k);
    evaluate_inplace(&k, 0, 1, 2, c, alt, &n);
    nc = find_crossings_clean(alt, n, cx, MAX_CROSSINGS);
    if (nc > 0) (void)knot_determinant(n, cx, nc);
    a = alexander_polynomial(n, cx, nc);
    (void)a;
    return 0;
}

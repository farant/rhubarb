/*
 * d112_export.c - provenance tool for Demo 114 (not part of the demo build)
 *
 * Compiles Demo 112's main.c UNMODIFIED (its main renamed) and prints, in a
 * line format Demo 114 embeds, exactly what D112 computed:
 *   - each base knot's raw braid polygon and its det-greedy simplification
 *     (reachability: target = expected det; spectrum/stability: auto det)
 *   - D112's per-alternative verdict for the reachability census:
 *       S = rejected as self-intersecting (sign 0 in all 8 directions)
 *       M = rejected because |Delta(-1)| != det (mismatch)
 *       N = dropped as non-palindromic
 *       V = accepted
 *   - the 20 randomized 6_3 stability polygons (seeds trial*7919+42)
 *
 * Build/run (from this directory):
 *   cc -std=c89 -w -O2 -o d112_export d112_export.c && ./d112_export > d112_export.txt
 */
#define main d112_main
#include "../demo_112_alexander_polynomial/main.c"
#undef main

/* "trefoil 3_1" -> "3_1": one token per name in the export */
static const char *code_of(const char *name)
{
    const char *sp = strrchr(name, ' ');
    return sp ? sp + 1 : name;
}

static void print_rat(Rat r)
{
    if (r.q == 1) printf("%ld", r.p);
    else printf("%ld/%ld", r.p, r.q);
}

static void print_polygon(const char *tag, const char *name, const StickKnot *k)
{
    int i;
    printf("%s %s %d", tag, code_of(name), k->n_vertices);
    for (i = 0; i < k->n_vertices; i++) {
        printf(" ");
        print_rat(k->vertices[i].x); printf(",");
        print_rat(k->vertices[i].y); printf(",");
        print_rat(k->vertices[i].z);
    }
    printf("\n");
}

/* simplify_polygon_topology_ex / simplify_polygon_random_ex, line for line,
 * printing each removed index (in the then-current polygon). seed < 0 =
 * deterministic scan. The caller checks the result equals D112's own. */
static void simplify_logged(StickKnot *knot, int known_det, long seed_in,
                            const char *tag, const char *name)
{
    int changed = 1, target_det, order[MAX_STICK_VERTICES], i;
    unsigned int seed = (unsigned int)(seed_in < 0 ? 0 : seed_in);

    if (known_det > 0) target_det = known_det;
    else {
        Crossing cx[MAX_CROSSINGS];
        int nc = find_crossings(knot->vertices, knot->n_vertices, cx, MAX_CROSSINGS);
        target_det = (nc > 0) ? knot_determinant(knot->n_vertices, cx, nc) : 1;
    }
    printf("STEPS %s %s target_det=%d", tag, code_of(name), target_det);
    while (changed) {
        int n = knot->n_vertices;
        changed = 0;
        for (i = 0; i < n; i++) order[i] = i;
        if (seed_in >= 0) {
            for (i = n - 1; i > 0; i--) {
                int j;
                seed = seed * 1103515245u + 12345u;
                j = (int)((seed >> 16) % (unsigned)(i + 1));
                { int tmp = order[i]; order[i] = order[j]; order[j] = tmp; }
            }
        }
        for (i = 0; i < n; i++) {
            int vi = order[i];
            Vec3Q temp[MAX_STICK_VERTICES];
            int tn = 0, j, nc, det;
            Crossing cx[MAX_CROSSINGS];
            for (j = 0; j < knot->n_vertices; j++)
                if (j != vi) temp[tn++] = knot->vertices[j];
            if (tn < 4) continue;
            nc = find_crossings(temp, tn, cx, MAX_CROSSINGS);
            det = (nc > 0) ? knot_determinant(tn, cx, nc) : 1;
            if (det == target_det) {
                for (j = 0; j < tn; j++) knot->vertices[j] = temp[j];
                knot->n_vertices = tn;
                printf(" %d", vi);
                changed = 1;
                break;
            }
        }
    }
    printf("\n");
}

static int same_polygon(const StickKnot *a, const StickKnot *b)
{
    int i;
    if (a->n_vertices != b->n_vertices) return 0;
    for (i = 0; i < a->n_vertices; i++)
        if (!rat_eq(a->vertices[i].x, b->vertices[i].x) ||
            !rat_eq(a->vertices[i].y, b->vertices[i].y) ||
            !rat_eq(a->vertices[i].z, b->vertices[i].z)) return 0;
    return 1;
}

static char verdict(const StickKnot *knot, unsigned int choices)
{
    Vec3Q alt[MAX_STICK_VERTICES];
    Crossing cx[MAX_CROSSINGS];
    int alt_n, alt_nc, alt_det, ck;
    RatPoly alex;
    Rat ev;

    evaluate_inplace(knot, 0, 1, 2, choices, alt, &alt_n);
    alt_nc = find_crossings_clean(alt, alt_n, cx, MAX_CROSSINGS);
    for (ck = 0; ck < alt_nc; ck++)
        if (cx[ck].sign == 0) return 'S';
    alt_det = (alt_nc > 0) ? knot_determinant(alt_n, cx, alt_nc) : 1;
    alex = alexander_polynomial(alt_n, cx, alt_nc);
    ev = rp_eval(&alex, rat_from_int(-1));
    if (ev.q != 1 || rat_abs(ev.p) != (i64)alt_det) return 'M';
    if (!rp_is_palindromic(&alex)) return 'N';
    return 'V';
}

int main(void)
{
    BraidWord braids[12];
    int bi, trial;

    braids[0] = braid_trefoil();     braids[1] = braid_figure_eight();
    braids[2] = braid_cinquefoil();  braids[3] = braid_5_2();
    braids[4] = braid_6_1();         braids[5] = braid_6_2();
    braids[6] = braid_6_3();         braids[7] = braid_7_1();
    braids[8] = braid_7_2();         braids[9] = braid_7_3();
    braids[10] = braid_7_4();        braids[11] = braid_8_18();

    for (bi = 0; bi < 12; bi++) {
        StickKnot raw, reach, spec;
        unsigned int c, max_c;

        braid_to_polygon(&braids[bi], &raw);
        print_polygon("RAW", braids[bi].name, &raw);
        reach = raw;
        simplify_polygon_topology_ex(&reach, braids[bi].expected_det);
        print_polygon("REACH", braids[bi].name, &reach);
        spec = raw;
        simplify_polygon_topology(&spec);
        print_polygon("SPEC", braids[bi].name, &spec);
        {
            StickKnot l1 = raw, l2 = raw;
            simplify_logged(&l1, braids[bi].expected_det, -1, "REACH", braids[bi].name);
            simplify_logged(&l2, 0, -1, "SPEC", braids[bi].name);
            if (!same_polygon(&l1, &reach) || !same_polygon(&l2, &spec)) {
                printf("REPLAY MISMATCH %s\n", braids[bi].name);
                return 1;
            }
        }

        max_c = 1u << (unsigned)(reach.n_vertices - 3);
        printf("VERDICT %s %u ", code_of(braids[bi].name), max_c);
        putchar('V');                       /* choices = 0: D112 used Fox */
        for (c = 1; c < max_c; c++) putchar(verdict(&reach, c));
        printf("\n");
        fflush(stdout);
    }
    {
        BraidWord b = braid_6_3();
        StickKnot base;
        braid_to_polygon(&b, &base);
        for (trial = 0; trial < 20; trial++) {
            StickKnot k = base;
            char tag[32];
            StickKnot l = base;
            if (trial == 0) simplify_polygon_topology(&k);
            else simplify_polygon_random(&k, (unsigned)(trial * 7919 + 42));
            sprintf(tag, "TRIAL%d", trial);
            print_polygon(tag, "6_3", &k);
            simplify_logged(&l, 0, trial == 0 ? -1 : (long)(trial * 7919 + 42), tag, "6_3");
            if (!same_polygon(&l, &k)) { printf("REPLAY MISMATCH %s\n", tag); return 1; }
        }
    }
    return 0;
}

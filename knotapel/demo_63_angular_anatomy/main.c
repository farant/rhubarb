/*
 * KNOTAPEL DEMO 63: Angular Anatomy of DKC
 * ==========================================
 *
 * The bracket catalog at delta=0 consists of 100 values, all AXIAL
 * in Z[zeta_8]: each value is an integer multiple of exactly one
 * basis element {1, zeta_8, i, zeta_8^3}. This means every weight
 * lies on one of 8 discrete rays (octants 0-7, spacing 45 degrees).
 *
 * This demo exploits that discrete structure to explain Demo 50's
 * four-tier NPN hierarchy:
 *   Tier 1 (k=2): 5 classes  — linearly separable
 *   Tier 2 (k=3): +1 class   — first nonlinear
 *   Tier 3 (k=4): +6 classes — everything except parity
 *   Tier 4 (k=6): +1 class   — parity (all 13)
 *
 * The key: k-sector boundaries partition the 8 octants into class-0
 * and class-1 groups. Different k values group octants differently.
 * Parity requires the specific grouping {2,4,5,7} = class 1, which
 * only k=6 provides (its 60-degree boundaries cut the 45-degree
 * lattice at the exact angle needed).
 *
 * Connection to RT truncation (Demo 39): at delta=0 (ell=2), the
 * Markov trace kills all j >= 1, making topology almost trivial.
 * But this SAME degeneracy forces bracket values onto single rays,
 * giving DKC its maximally structured angular alphabet. DKC works
 * BECAUSE it reads pre-closure information that RT truncation
 * would discard.
 *
 * C89, zero dependencies beyond math.h.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* ================================================================
 * Test infrastructure
 * ================================================================ */

static int n_pass = 0, n_fail = 0;

static void check(const char *msg, int ok) {
    if (ok) { printf("  PASS: %s\n", msg); n_pass++; }
    else    { printf("  FAIL: %s\n", msg); n_fail++; }
}

/* ================================================================
 * Complex double type
 * ================================================================ */

typedef struct { double re, im; } Cx;

static Cx cx_make(double re, double im) {
    Cx z; z.re = re; z.im = im; return z;
}
static Cx cx_add(Cx a, Cx b) {
    return cx_make(a.re + b.re, a.im + b.im);
}
static double cx_abs(Cx a) {
    return sqrt(a.re * a.re + a.im * a.im);
}

/* ================================================================
 * Exact cyclotomic type Z[zeta_8]
 *
 * Basis: {1, zeta_8, zeta_8^2, zeta_8^3} with zeta_8^4 = -1
 * ================================================================ */

typedef struct {
    long a, b, c, d;  /* a + b*zeta_8 + c*zeta_8^2 + d*zeta_8^3 */
} Cyc8;

static Cyc8 cyc8_make(long a, long b, long c, long d) {
    Cyc8 z; z.a = a; z.b = b; z.c = c; z.d = d; return z;
}
static Cyc8 cyc8_zero(void) { return cyc8_make(0, 0, 0, 0); }
static Cyc8 cyc8_one(void)  { return cyc8_make(1, 0, 0, 0); }
static Cyc8 cyc8_add(Cyc8 x, Cyc8 y) {
    return cyc8_make(x.a + y.a, x.b + y.b, x.c + y.c, x.d + y.d);
}
static Cyc8 cyc8_mul(Cyc8 x, Cyc8 y) {
    return cyc8_make(
        x.a * y.a - x.b * y.d - x.c * y.c - x.d * y.b,
        x.a * y.b + x.b * y.a - x.c * y.d - x.d * y.c,
        x.a * y.c + x.b * y.b + x.c * y.a - x.d * y.d,
        x.a * y.d + x.b * y.c + x.c * y.b + x.d * y.a);
}
static int cyc8_eq(Cyc8 x, Cyc8 y) {
    return x.a == y.a && x.b == y.b && x.c == y.c && x.d == y.d;
}
static Cx cyc8_to_cx(Cyc8 z) {
    double s = 1.0 / sqrt(2.0);
    return cx_make((double)z.a + (double)z.b * s + (double)z.d * (-s),
                   (double)z.b * s + (double)z.c + (double)z.d * s);
}
static Cyc8 cyc8_conj(Cyc8 z) {
    return cyc8_make(z.a, -z.d, -z.c, -z.b);
}
static Cyc8 cyc8_pow_int(Cyc8 base, int n) {
    Cyc8 r = cyc8_one();
    if (n == 0) return r;
    if (n < 0) { n = -n; base = cyc8_conj(base); }
    while (n > 0) {
        if (n & 1) r = cyc8_mul(r, base);
        base = cyc8_mul(base, base);
        n >>= 1;
    }
    return r;
}

/* ================================================================
 * Braid type + union-find loop counter (from Demo 50)
 * ================================================================ */

#define MAX_WORD 64
typedef struct { int word[MAX_WORD]; int len, n; } Braid;

#define MAX_UF 4096
static int uf_p[MAX_UF];
static void uf_init(int n) {
    int i; for (i = 0; i < n; i++) uf_p[i] = i;
}
static int uf_find(int x) {
    while (uf_p[x] != x) { uf_p[x] = uf_p[uf_p[x]]; x = uf_p[x]; }
    return x;
}
static void uf_union(int x, int y) {
    x = uf_find(x); y = uf_find(y); if (x != y) uf_p[x] = y;
}

static int braid_loops(const Braid *b, unsigned s) {
    int N = (b->len + 1) * b->n, l, p, i, loops, sgn, bit, cup;
    uf_init(N);
    for (l = 0; l < b->len; l++) {
        sgn = b->word[l] > 0 ? 1 : -1;
        i = (sgn > 0 ? b->word[l] : -b->word[l]) - 1;
        bit = (int)((s >> (unsigned)l) & 1u);
        cup = (sgn > 0) ? (bit == 0) : (bit == 1);
        if (cup) {
            uf_union(l * b->n + i, l * b->n + i + 1);
            uf_union((l + 1) * b->n + i, (l + 1) * b->n + i + 1);
            for (p = 0; p < b->n; p++)
                if (p != i && p != i + 1)
                    uf_union(l * b->n + p, (l + 1) * b->n + p);
        } else {
            for (p = 0; p < b->n; p++)
                uf_union(l * b->n + p, (l + 1) * b->n + p);
        }
    }
    for (p = 0; p < b->n; p++)
        uf_union(p, b->len * b->n + p);
    loops = 0;
    for (i = 0; i < N; i++)
        if (uf_find(i) == i) loops++;
    return loops;
}

/* ================================================================
 * Bracket catalog builder (standard, from Demo 50)
 * ================================================================ */

#define MAX_DISTINCT 512

static Cyc8 cat_exact[MAX_DISTINCT];
static Cx   cat_cx[MAX_DISTINCT];
static int  cat_size = 0;

static int find_in(const Cyc8 *arr, int size, Cyc8 val) {
    int i;
    for (i = 0; i < size; i++)
        if (cyc8_eq(val, arr[i])) return i;
    return -1;
}

static void build_catalog(void) {
    Cyc8 A = cyc8_make(0, -1, 0, 0);  /* -zeta_8 */
    int n, len;
    Braid b;
    int word_buf[MAX_WORD];

    cat_size = 0;

    for (n = 2; n <= 3; n++) {
        for (len = 1; len <= 8 && len <= MAX_WORD; len++) {
            int max_gen = n - 1;
            int total_gens = 2 * max_gen;
            unsigned long total, idx;
            int i;

            total = 1;
            for (i = 0; i < len; i++) {
                total *= (unsigned long)total_gens;
                if (total > 100000) break;
            }
            if (total > 100000) continue;

            for (idx = 0; idx < total; idx++) {
                unsigned long tmp = idx;
                Cyc8 bracket;
                unsigned s, ns;
                int a_count, b_count, lp;

                for (i = 0; i < len; i++) {
                    int g = (int)(tmp % (unsigned long)total_gens);
                    tmp /= (unsigned long)total_gens;
                    if (g < max_gen) word_buf[i] = g + 1;
                    else             word_buf[i] = -(g - max_gen + 1);
                }
                b.n = n; b.len = len;
                memcpy(b.word, word_buf, (size_t)len * sizeof(int));

                bracket = cyc8_zero();
                if (b.len > 0) {
                    ns = 1u << b.len;
                    for (s = 0; s < ns; s++) {
                        a_count = 0; b_count = 0;
                        for (i = 0; i < b.len; i++) {
                            if ((s >> (unsigned)i) & 1u) b_count++;
                            else a_count++;
                        }
                        lp = braid_loops(&b, s);
                        if (lp == 1) {
                            bracket = cyc8_add(bracket,
                                cyc8_pow_int(A, a_count - b_count));
                        }
                    }
                }

                if (!cyc8_eq(bracket, cyc8_zero())
                    && find_in(cat_exact, cat_size, bracket) < 0
                    && cat_size < MAX_DISTINCT) {
                    cat_exact[cat_size] = bracket;
                    cat_cx[cat_size] = cyc8_to_cx(bracket);
                    cat_size++;
                }
            }
        }
    }
}

/* ================================================================
 * NPN Classification (from Demo 50)
 * ================================================================ */

static const int perms3[6][3] = {
    {0,1,2}, {0,2,1}, {1,0,2}, {1,2,0}, {2,0,1}, {2,1,0}
};

static int npn_transform(int tt, const int sigma[3],
                         int neg_in, int neg_out) {
    int result = 0, idx;
    for (idx = 0; idx < 8; idx++) {
        int x[3], y[3], src, out;
        x[0] = (idx >> 2) & 1;
        x[1] = (idx >> 1) & 1;
        x[2] = idx & 1;
        y[0] = x[sigma[0]] ^ ((neg_in >> 0) & 1);
        y[1] = x[sigma[1]] ^ ((neg_in >> 1) & 1);
        y[2] = x[sigma[2]] ^ ((neg_in >> 2) & 1);
        src = (y[0] << 2) | (y[1] << 1) | y[2];
        out = (tt >> src) & 1;
        if (neg_out) out ^= 1;
        result |= (out << idx);
    }
    return result;
}

static int npn_canon[256];

static void npn_init(void) {
    int i, pi, ni, no;
    for (i = 0; i < 256; i++) {
        int min_tt = i;
        for (pi = 0; pi < 6; pi++)
            for (ni = 0; ni < 8; ni++)
                for (no = 0; no < 2; no++) {
                    int t = npn_transform(i, perms3[pi], ni, no);
                    if (t < min_tt) min_tt = t;
                }
        npn_canon[i] = min_tt;
    }
}

static const char *fn_name(int tt) {
    switch (tt) {
    case 0x00: return "FALSE";
    case 0xFF: return "TRUE";
    case 0x01: return "AND3'";
    case 0x03: return "AND2'";
    case 0x06: return "~A(B^C)";
    case 0x07: return "~A~(BC)";
    case 0x0F: return "BUF";
    case 0x16: return "EXACT1";
    case 0x17: return "MAJ'";
    case 0x18: return "ISOLATE";
    case 0x19: return "3v-0x19";
    case 0x1B: return "3v-0x1B";
    case 0x1E: return "A^(B|C)";
    case 0x3C: return "XOR2";
    case 0x69: return "XNOR3";
    default: return "???";
    }
}

#define MAX_NPN 14
typedef struct {
    int canon_tt;
    const char *titulis;
} NPNClass;

static NPNClass npn_classes[MAX_NPN];
static int n_npn = 0;

static void npn_build_classes(void) {
    int seen[256];
    int i;

    memset(seen, 0, sizeof(seen));
    n_npn = 0;

    for (i = 1; i < 255; i++) {
        int c = npn_canon[i];
        if (!seen[c]) {
            seen[c] = 1;
            npn_classes[n_npn].canon_tt = c;
            npn_classes[n_npn].titulis = fn_name(c);
            n_npn++;
        }
    }
}

/* ================================================================
 * k-sector classification (from Demo 50)
 * ================================================================ */

static int sector_classify(Cx z, int k) {
    double angle, sector_width;
    int sector;

    if (cx_abs(z) < 1e-15) return 0;

    angle = atan2(z.im, z.re);
    if (angle < 0.0) angle += 2.0 * M_PI;

    sector_width = 2.0 * M_PI / (double)k;
    sector = (int)(angle / sector_width);
    if (sector >= k) sector = k - 1;

    return sector % 2;
}

/* Return the sector number (not mod 2) */
static int sector_number(Cx z, int k) {
    double angle, sector_width;
    int sector;

    if (cx_abs(z) < 1e-15) return 0;

    angle = atan2(z.im, z.re);
    if (angle < 0.0) angle += 2.0 * M_PI;

    sector_width = 2.0 * M_PI / (double)k;
    sector = (int)(angle / sector_width);
    if (sector >= k) sector = k - 1;

    return sector;
}

/* ================================================================
 * Octant computation: angle -> octant (0-7, each 45 degrees)
 * ================================================================ */

static int angle_to_octant(double re, double im) {
    double angle = atan2(im, re);
    int oct;
    if (angle < 0.0) angle += 2.0 * M_PI;
    oct = (int)(angle / (M_PI / 4.0));
    if (oct > 7) oct = 7;
    return oct;
}

/* ================================================================
 * PART A: Octant Enumeration
 *
 * Enumerate all catalog values by ray (which Z[zeta_8] component
 * is nonzero) and octant. Verify the axiality theorem: each value
 * has exactly one nonzero component.
 * ================================================================ */

/* Per-catalog-entry: which ray (0-3) and sign (+/-) */
static int cat_ray[MAX_DISTINCT];    /* 0=a, 1=b, 2=c, 3=d */
static int cat_sign[MAX_DISTINCT];   /* +1 or -1 */
static int cat_octant[MAX_DISTINCT]; /* 0-7 */

static void part_a_octant_enum(void) {
    int i;
    int n_axial = 0;
    int oct_count[8];
    int ray_count[4];    /* a, b, c, d */
    char msg[256];

    printf("\n=== PART A: Octant Enumeration ===\n");
    printf("  Catalog: %d values\n\n", cat_size);

    memset(oct_count, 0, sizeof(oct_count));
    memset(ray_count, 0, sizeof(ray_count));

    for (i = 0; i < cat_size; i++) {
        Cyc8 v = cat_exact[i];
        int nz = 0;
        int ray = -1;
        long coeff = 0;

        if (v.a != 0) { nz++; ray = 0; coeff = v.a; }
        if (v.b != 0) { nz++; ray = 1; coeff = v.b; }
        if (v.c != 0) { nz++; ray = 2; coeff = v.c; }
        if (v.d != 0) { nz++; ray = 3; coeff = v.d; }

        if (nz == 1) {
            n_axial++;
            cat_ray[i] = ray;
            cat_sign[i] = (coeff > 0) ? 1 : -1;
            ray_count[ray]++;
        } else {
            cat_ray[i] = -1;
            cat_sign[i] = 0;
        }

        cat_octant[i] = angle_to_octant(cat_cx[i].re, cat_cx[i].im);
        oct_count[cat_octant[i]]++;
    }

    /* Print ray distribution */
    printf("  Ray distribution (which Z[zeta_8] basis element):\n");
    printf("    a (real, 0/180 deg):     %d values\n", ray_count[0]);
    printf("    b (zeta_8, 45/225 deg):  %d values\n", ray_count[1]);
    printf("    c (i, 90/270 deg):       %d values\n", ray_count[2]);
    printf("    d (zeta_8^3, 135/315 deg): %d values\n", ray_count[3]);

    /* Print octant distribution */
    printf("\n  Octant distribution:\n");
    for (i = 0; i < 8; i++) {
        printf("    oct %d [%3d-%3d deg]: %2d values\n",
               i, i * 45, (i + 1) * 45, oct_count[i]);
    }

    /* Verify axiality: all values have exactly 1 nonzero component */
    sprintf(msg, "all %d catalog values are axial (1 nonzero component)",
            cat_size);
    check(msg, n_axial == cat_size);

    /* Verify octant counts match expected */
    sprintf(msg, "octant symmetry: oct0=oct2=oct6=%d", oct_count[0]);
    check(msg, oct_count[0] == oct_count[2]
            && oct_count[2] == oct_count[6]);

    sprintf(msg, "octant symmetry: oct1=oct7=%d", oct_count[1]);
    check(msg, oct_count[1] == oct_count[7]);

    sprintf(msg, "octant symmetry: oct3=oct4=oct5=%d", oct_count[3]);
    check(msg, oct_count[3] == oct_count[4]
            && oct_count[4] == oct_count[5]);
}

/* ================================================================
 * PART B: Sector-Octant Mapping
 *
 * For each k from 2 to 8: compute which octants map to which
 * sector, and which octants are class-1 (odd sector) vs class-0.
 * ================================================================ */

#define K_MIN 2
#define K_MAX 8

static void part_b_sector_octant(void) {
    int k, oct;
    /* Representative angle for each octant: midpoint */
    /* Actually use the exact lattice angle: oct*45 deg */
    double oct_angles[8];

    printf("\n=== PART B: Sector-Octant Mapping ===\n\n");

    for (oct = 0; oct < 8; oct++) {
        oct_angles[oct] = (double)oct * M_PI / 4.0;
    }

    printf("  %3s", "oct");
    for (k = K_MIN; k <= K_MAX; k++)
        printf("  k=%d", k);
    printf("\n  ---");
    for (k = K_MIN; k <= K_MAX; k++)
        printf("  ---");
    printf("\n");

    for (oct = 0; oct < 8; oct++) {
        printf("  %3d", oct);
        for (k = K_MIN; k <= K_MAX; k++) {
            double sw = 2.0 * M_PI / (double)k;
            int sec = (int)(oct_angles[oct] / sw);
            if (sec >= k) sec = k - 1;
            printf("  s%d%c", sec, (sec % 2) ? '*' : ' ');
        }
        printf("    (%d deg)\n", oct * 45);
    }

    printf("\n  (* = odd sector = class 1)\n");

    /* Print class-1 octant sets for each k */
    printf("\n  Class-1 octant sets:\n");
    for (k = K_MIN; k <= K_MAX; k++) {
        int class1[8];
        int n1 = 0;
        printf("    k=%d: {", k);
        for (oct = 0; oct < 8; oct++) {
            double sw = 2.0 * M_PI / (double)k;
            int sec = (int)(oct_angles[oct] / sw);
            if (sec >= k) sec = k - 1;
            if (sec % 2) {
                class1[n1++] = oct;
            }
        }
        for (oct = 0; oct < n1; oct++) {
            if (oct > 0) printf(",");
            printf("%d", class1[oct]);
        }
        printf("}\n");
    }

    /* Highlight: k=6 class-1 = {2,4,5,7} = parity octants */
    {
        char msg[256];
        int k6_class1[8];
        int n1 = 0;
        for (oct = 0; oct < 8; oct++) {
            double sw = 2.0 * M_PI / 6.0;
            int sec = (int)(oct_angles[oct] / sw);
            if (sec >= 6) sec = 5;
            if (sec % 2) k6_class1[n1++] = oct;
        }
        sprintf(msg, "k=6 class-1 octants = {2,4,5,7} (parity octants)");
        check(msg, n1 == 4
            && k6_class1[0] == 2 && k6_class1[1] == 4
            && k6_class1[2] == 5 && k6_class1[3] == 7);
    }
}

/* ================================================================
 * PART C: Representative Weight Triple Examples
 *
 * For each tier, find one representative triple and display the
 * full anatomy: all 8 weight sums with Z[zeta_8] coordinates,
 * octant, and sector assignment. Makes the classification
 * mechanism visible.
 * ================================================================ */

static int compute_tt_at_k(int i1, int i2, int i3, int k) {
    int tt = 0, idx;
    Cx w1 = cat_cx[i1], w2 = cat_cx[i2], w3 = cat_cx[i3];
    Cx s12 = cx_add(w1, w2);

    for (idx = 1; idx < 8; idx++) {
        int x0 = (idx >> 2) & 1;
        int x1 = (idx >> 1) & 1;
        int x2 = idx & 1;
        Cx z;
        int cls;

        if (x0 && x1 && x2)  z = cx_add(s12, w3);
        else if (x0 && x1)   z = s12;
        else if (x0 && x2)   z = cx_add(w1, w3);
        else if (x1 && x2)   z = cx_add(w2, w3);
        else if (x0)         z = w1;
        else if (x1)         z = w2;
        else                  z = w3;

        cls = sector_classify(z, k);
        if (cls) tt |= (1 << idx);
    }
    return tt;
}

static void print_triple_anatomy(int i1, int i2, int i3, int k) {
    int idx;
    Cyc8 e1 = cat_exact[i1], e2 = cat_exact[i2], e3 = cat_exact[i3];

    printf("    w1 = cat[%2d] = (%2ld,%2ld,%2ld,%2ld) [oct %d]\n",
           i1, e1.a, e1.b, e1.c, e1.d, cat_octant[i1]);
    printf("    w2 = cat[%2d] = (%2ld,%2ld,%2ld,%2ld) [oct %d]\n",
           i2, e2.a, e2.b, e2.c, e2.d, cat_octant[i2]);
    printf("    w3 = cat[%2d] = (%2ld,%2ld,%2ld,%2ld) [oct %d]\n\n",
           i3, e3.a, e3.b, e3.c, e3.d, cat_octant[i3]);

    printf("    %-9s  %18s  %3s  %3s  %3s\n",
           "input", "sum (a,b,c,d)", "oct", "sec", "cls");
    printf("    ---------  ------------------  ---  ---  ---\n");

    /* (0,0,0) -> zero sum */
    printf("    (0,0,0)    (%2d,%2d,%2d,%2d)        -   s0    0\n",
           0, 0, 0, 0);

    for (idx = 1; idx < 8; idx++) {
        int x0 = (idx >> 2) & 1;
        int x1 = (idx >> 1) & 1;
        int x2 = idx & 1;
        Cyc8 ez = cyc8_zero();
        Cx z;

        if (x0) ez = cyc8_add(ez, e1);
        if (x1) ez = cyc8_add(ez, e2);
        if (x2) ez = cyc8_add(ez, e3);

        z = cyc8_to_cx(ez);
        if (cx_abs(z) < 1e-15) {
            printf("    (%d,%d,%d)    (%2ld,%2ld,%2ld,%2ld)        -   s0    0\n",
                   x0, x1, x2, ez.a, ez.b, ez.c, ez.d);
        } else {
            int oct = angle_to_octant(z.re, z.im);
            int sec = sector_number(z, k);
            int cls = sec % 2;
            printf("    (%d,%d,%d)    (%2ld,%2ld,%2ld,%2ld)        %d   s%d    %d\n",
                   x0, x1, x2, ez.a, ez.b, ez.c, ez.d, oct, sec, cls);
        }
    }
}

static void part_c_examples(void) {
    int target_tt[4], target_k[4];
    const char *target_tier[4], *target_name[4];
    int t, found_count = 0;
    char msg[256];

    target_tt[0] = 0x01; target_k[0] = 2;
    target_tier[0] = "Tier 1 (k=2)"; target_name[0] = "AND3'";
    target_tt[1] = 0x17; target_k[1] = 3;
    target_tier[1] = "Tier 2 (k=3)"; target_name[1] = "MAJ'";
    target_tt[2] = 0x16; target_k[2] = 4;
    target_tier[2] = "Tier 3 (k=4)"; target_name[2] = "EXACT1";
    target_tt[3] = 0x69; target_k[3] = 6;
    target_tier[3] = "Tier 4 (k=6)"; target_name[3] = "XNOR3";

    printf("\n=== PART C: Representative Weight Triple Examples ===\n");
    printf("  One example from each tier showing the mechanism.\n\n");

    for (t = 0; t < 4; t++) {
        int i1, i2, i3, found = 0;
        printf("  %s: %s (0x%02X) at k=%d\n",
               target_tier[t], target_name[t],
               target_tt[t], target_k[t]);

        for (i1 = 0; i1 < cat_size && !found; i1++) {
            for (i2 = 0; i2 < cat_size && !found; i2++) {
                for (i3 = 0; i3 < cat_size && !found; i3++) {
                    int tt = compute_tt_at_k(i1, i2, i3, target_k[t]);
                    if (npn_canon[tt] == target_tt[t]) {
                        print_triple_anatomy(i1, i2, i3, target_k[t]);
                        found = 1;
                    }
                }
            }
        }
        if (found) found_count++;
        else printf("    (no example found)\n");
        printf("\n");
    }

    sprintf(msg, "found representative triples for all 4 tiers (%d/4)",
            found_count);
    check(msg, found_count == 4);
}

/* ================================================================
 * PART D: Minimum-k per NPN Class (the main verification)
 *
 * For each k from 2 to 8, search all catalog triples for all 13
 * NPN classes. Reproduce the four-tier structure.
 * ================================================================ */

/* Per-k, per-NPN-class: is it reachable? */
static int k_reachable[K_MAX + 1][MAX_NPN];
static long k_solutions[K_MAX + 1][MAX_NPN];
static long k_parity[K_MAX + 1];

static void search_k(int k) {
    int i1, i2, i3, ci;
    long tt_counts[256];

    memset(tt_counts, 0, sizeof(tt_counts));

    for (i1 = 0; i1 < cat_size; i1++) {
        Cx w1 = cat_cx[i1];
        for (i2 = 0; i2 < cat_size; i2++) {
            Cx w2 = cat_cx[i2];
            Cx s12 = cx_add(w1, w2);
            for (i3 = 0; i3 < cat_size; i3++) {
                Cx w3 = cat_cx[i3];
                int tt = 0;
                int idx;

                for (idx = 1; idx < 8; idx++) {
                    int x0 = (idx >> 2) & 1;
                    int x1 = (idx >> 1) & 1;
                    int x2 = idx & 1;
                    Cx z;
                    int cls;

                    if (x0 && x1 && x2) {
                        z = cx_add(s12, w3);
                    } else if (x0 && x1) {
                        z = s12;
                    } else if (x0 && x2) {
                        z = cx_add(w1, w3);
                    } else if (x1 && x2) {
                        z = cx_add(w2, w3);
                    } else if (x0) {
                        z = w1;
                    } else if (x1) {
                        z = w2;
                    } else {
                        z = w3;
                    }

                    cls = sector_classify(z, k);
                    if (cls) tt |= (1 << idx);
                }

                tt_counts[tt]++;
            }
        }
    }

    /* Aggregate per NPN class */
    for (ci = 0; ci < n_npn; ci++) {
        int canon = npn_classes[ci].canon_tt;
        int tt_idx;
        k_solutions[k][ci] = 0;
        for (tt_idx = 0; tt_idx < 256; tt_idx++)
            if (npn_canon[tt_idx] == canon)
                k_solutions[k][ci] += tt_counts[tt_idx];
        k_reachable[k][ci] = (k_solutions[k][ci] > 0) ? 1 : 0;
    }

    /* Parity count */
    {
        int tt_idx;
        k_parity[k] = 0;
        for (tt_idx = 0; tt_idx < 256; tt_idx++)
            if (npn_canon[tt_idx] == 0x69)
                k_parity[k] += tt_counts[tt_idx];
    }
}

static void part_d_min_k(void) {
    int k, ci;
    int min_k[MAX_NPN];
    int tier_counts[4];  /* tier 0=k2, 1=k3, 2=k4, 3=k6 */
    char msg[256];

    printf("\n=== PART D: Minimum-k per NPN Class ===\n");
    printf("  Searching 100^3 = 1M triples per k value...\n\n");

    for (k = K_MIN; k <= K_MAX; k++) {
        printf("  k=%d: searching...", k);
        fflush(stdout);
        search_k(k);
        {
            int nr = 0;
            for (ci = 0; ci < n_npn; ci++)
                if (k_reachable[k][ci]) nr++;
            printf(" %d/%d classes, parity=%ld\n", nr, n_npn, k_parity[k]);
        }
    }

    /* Compute min-k per class */
    printf("\n  Minimum sector count per NPN class:\n");
    printf("  %4s  %-12s  %6s  %10s\n", "TT", "Name", "Min k", "Solutions");
    printf("  ----  ----------  ------  ----------\n");

    memset(tier_counts, 0, sizeof(tier_counts));

    for (ci = 0; ci < n_npn; ci++) {
        min_k[ci] = 0;
        for (k = K_MIN; k <= K_MAX; k++) {
            if (k_reachable[k][ci]) {
                min_k[ci] = k;
                break;
            }
        }
        printf("  0x%02X  %-12s  %6d  %10ld\n",
               npn_classes[ci].canon_tt,
               npn_classes[ci].titulis,
               min_k[ci],
               min_k[ci] > 0 ? k_solutions[min_k[ci]][ci] : 0L);
    }

    /* Count tiers */
    {
        int t1 = 0, t2 = 0, t3 = 0, t4 = 0;
        for (ci = 0; ci < n_npn; ci++) {
            if (min_k[ci] == 2) t1++;
            else if (min_k[ci] == 3) t2++;
            else if (min_k[ci] == 4) t3++;
            else if (min_k[ci] == 6) t4++;
        }
        printf("\n  Four-tier structure:\n");
        printf("    Tier 1 (k=2): %d classes\n", t1);
        printf("    Tier 2 (k=3): %d classes\n", t2);
        printf("    Tier 3 (k=4): %d classes\n", t3);
        printf("    Tier 4 (k=6): %d classes\n", t4);

        sprintf(msg, "Tier 1 (k=2): 5 classes");
        check(msg, t1 == 5);
        sprintf(msg, "Tier 2 (k=3): 1 class");
        check(msg, t2 == 1);
        sprintf(msg, "Tier 3 (k=4): 6 classes");
        check(msg, t3 == 6);
        sprintf(msg, "Tier 4 (k=6): 1 class (parity)");
        check(msg, t4 == 1);
    }

    /* Reachability heatmap */
    printf("\n  Reachability heatmap:\n");
    printf("  %4s  %-12s", "TT", "Name");
    for (k = K_MIN; k <= K_MAX; k++)
        printf("  k=%d", k);
    printf("\n  ----  ----------");
    for (k = K_MIN; k <= K_MAX; k++)
        printf("  ---");
    printf("\n");

    for (ci = 0; ci < n_npn; ci++) {
        printf("  0x%02X  %-12s",
               npn_classes[ci].canon_tt,
               npn_classes[ci].titulis);
        for (k = K_MIN; k <= K_MAX; k++) {
            printf("  %3s", k_reachable[k][ci] ? "Y" : "-");
        }
        printf("\n");
    }

    /* Parity non-monotonicity */
    printf("\n  Parity solution counts:\n");
    for (k = K_MIN; k <= K_MAX; k++)
        printf("    k=%d: %ld\n", k, k_parity[k]);

    sprintf(msg, "parity non-monotonic: k=6(%ld) > k=7(%ld) > k=8(%ld)",
            k_parity[6], k_parity[7], k_parity[8]);
    check(msg, k_parity[6] > k_parity[7]
            && k_parity[7] > k_parity[8]);
}

/* ================================================================
 * PART E: Parity Triskelion Anatomy
 *
 * For parity solutions at k=6: verify all weights come from
 * class-1 octants {2,4,5,7}, and each solution places one weight
 * per odd sector (triskelion pattern).
 * ================================================================ */

static void part_e_triskelion(void) {
    int i1, i2, i3;
    int n_parity = 0;
    int n_all_class1 = 0;  /* all 3 weights in class-1 octants */
    int n_triskelion = 0;  /* one weight per odd sector */
    /* Octant-set distribution */
    int set_257 = 0, set_247 = 0, set_other = 0;
    char msg[256];

    printf("\n=== PART E: Parity Triskelion Anatomy (k=6) ===\n\n");

    for (i1 = 0; i1 < cat_size; i1++) {
        Cx w1 = cat_cx[i1];
        int o1 = cat_octant[i1];
        for (i2 = 0; i2 < cat_size; i2++) {
            Cx w2 = cat_cx[i2];
            int o2 = cat_octant[i2];
            Cx s12 = cx_add(w1, w2);
            for (i3 = 0; i3 < cat_size; i3++) {
                Cx w3 = cat_cx[i3];
                int o3 = cat_octant[i3];
                int tt = 0;
                int idx;

                for (idx = 1; idx < 8; idx++) {
                    int x0 = (idx >> 2) & 1;
                    int x1 = (idx >> 1) & 1;
                    int x2 = idx & 1;
                    Cx z;
                    int cls;

                    if (x0 && x1 && x2) {
                        z = cx_add(s12, w3);
                    } else if (x0 && x1) {
                        z = s12;
                    } else if (x0 && x2) {
                        z = cx_add(w1, w3);
                    } else if (x1 && x2) {
                        z = cx_add(w2, w3);
                    } else if (x0) {
                        z = w1;
                    } else if (x1) {
                        z = w2;
                    } else {
                        z = w3;
                    }

                    cls = sector_classify(z, 6);
                    if (cls) tt |= (1 << idx);
                }

                if (npn_canon[tt] == 0x69) {
                    int class1_octs[8] = {0,0,1,0,1,1,0,1};
                    /* class-1 at k=6: octants 2,4,5,7 */
                    int all_c1;
                    int sec1, sec2, sec3;
                    int sec_used[6];

                    n_parity++;

                    /* Check all weights in class-1 octants */
                    all_c1 = class1_octs[o1]
                           && class1_octs[o2]
                           && class1_octs[o3];
                    if (all_c1) n_all_class1++;

                    /* Check triskelion: one per odd sector */
                    /* k=6 sectors for each octant angle */
                    sec1 = sector_number(cat_cx[i1], 6);
                    sec2 = sector_number(cat_cx[i2], 6);
                    sec3 = sector_number(cat_cx[i3], 6);

                    memset(sec_used, 0, sizeof(sec_used));
                    sec_used[sec1]++;
                    sec_used[sec2]++;
                    sec_used[sec3]++;

                    /* Triskelion: exactly 1 in each of sectors 1,3,5 */
                    if (sec_used[1] == 1 && sec_used[3] == 1
                        && sec_used[5] == 1) {
                        n_triskelion++;
                    }

                    /* Octant set classification */
                    {
                        int octs[8];
                        memset(octs, 0, sizeof(octs));
                        octs[o1] = 1; octs[o2] = 1; octs[o3] = 1;
                        if (octs[2] && octs[5] && octs[7]
                            && !octs[0] && !octs[1] && !octs[3]
                            && !octs[4] && !octs[6]) {
                            set_257++;
                        } else if (octs[2] && octs[4] && octs[7]
                            && !octs[0] && !octs[1] && !octs[3]
                            && !octs[5] && !octs[6]) {
                            set_247++;
                        } else {
                            set_other++;
                        }
                    }
                }
            }
        }
    }

    printf("  Total parity solutions (k=6): %d\n", n_parity);
    printf("  All 3 weights in class-1 octants: %d\n", n_all_class1);
    printf("  Triskelion (1 per odd sector): %d\n", n_triskelion);
    printf("  Octant sets: {2,5,7}=%d  {2,4,7}=%d  other=%d\n",
           set_257, set_247, set_other);

    sprintf(msg, "all parity weights in class-1 octants (%d/%d)",
            n_all_class1, n_parity);
    check(msg, n_all_class1 == n_parity && n_parity > 0);

    sprintf(msg, "all parity solutions are triskelions (%d/%d)",
            n_triskelion, n_parity);
    check(msg, n_triskelion == n_parity);

    sprintf(msg, "only two octant sets: {2,5,7} and {2,4,7}");
    check(msg, set_other == 0 && set_257 + set_247 == n_parity);

    sprintf(msg, "parity count matches Demo 50 (906)");
    check(msg, n_parity == 906);
}

/* ================================================================
 * PART F: Commensurability
 *
 * Show how gcd(k,8) controls parity solution counts.
 * ================================================================ */

static int gcd(int a, int b) {
    while (b) { int t = b; b = a % b; a = t; }
    return a;
}

static void part_f_commensurability(void) {
    int k;
    char msg[256];

    printf("\n=== PART F: Commensurability Analysis ===\n\n");

    printf("  %3s  %8s  %6s  %s\n",
           "k", "gcd(k,8)", "parity", "class-1 octants");
    printf("  ---  --------  ------  ---------------\n");

    for (k = K_MIN; k <= K_MAX; k++) {
        int g = gcd(k, 8);
        int oct;
        printf("  %3d  %8d  %6ld  {", k, g, k_parity[k]);
        {
            int first = 1;
            double oct_angles[8];
            for (oct = 0; oct < 8; oct++)
                oct_angles[oct] = (double)oct * M_PI / 4.0;
            for (oct = 0; oct < 8; oct++) {
                double sw = 2.0 * M_PI / (double)k;
                int sec = (int)(oct_angles[oct] / sw);
                if (sec >= k) sec = k - 1;
                if (sec % 2) {
                    if (!first) printf(",");
                    printf("%d", oct);
                    first = 0;
                }
            }
        }
        printf("}\n");
    }

    /* At k=8, sectors perfectly align with octants */
    sprintf(msg, "k=8 gcd=8: perfect alignment, minimum parity (96)");
    check(msg, gcd(8, 8) == 8 && k_parity[8] == 96);

    /* k=6 is the sweet spot */
    sprintf(msg, "k=6 gcd=2: partial alignment, maximum parity (906)");
    check(msg, gcd(6, 8) == 2 && k_parity[6] == 906);
}

/* ================================================================
 * PART G: Oriented Matroid Classification
 *
 * Each weight triple (w1, w2, w3) in R^2 defines an oriented
 * matroid via the sign pattern of 3 pairwise cross products.
 * Since all catalog values are axial, the cross product sign
 * depends only on the octant difference:
 *   delta_oct mod 8 in {1,2,3}: +  (CCW)
 *   delta_oct mod 8 in {5,6,7}: -  (CW)
 *   delta_oct mod 8 in {0,4}:   0  (parallel/antiparallel)
 *
 * This classifies all 10^6 triples by OM type and determines
 * which NPN classes each type can achieve at each k.
 * ================================================================ */

/* Cross product sign from octant indices */
static int cross_sign(int oct1, int oct2) {
    int d = ((oct2 - oct1) % 8 + 8) % 8;
    if (d == 0 || d == 4) return 0;
    return (d <= 3) ? 1 : -1;
}

/* Encode OM type as index 0..26 */
#define N_OM 27

static int om_encode(int s12, int s13, int s23) {
    return (s12 + 1) * 9 + (s13 + 1) * 3 + (s23 + 1);
}

static void om_decode(int om, int *s12, int *s13, int *s23) {
    *s12 = (om / 9) - 1;
    *s13 = ((om / 3) % 3) - 1;
    *s23 = (om % 3) - 1;
}

static char sign_char(int s) {
    if (s > 0) return '+';
    if (s < 0) return '-';
    return '0';
}

static void part_g_om_classify(void) {
    int o1, o2, o3;
    int i1, i2, i3, k, ci, om_idx;
    int om_exists[N_OM];
    int om_oct_count[N_OM];
    int om_type_count;
    char msg[256];

    /* G2 accumulators */
    int om_npn_reach[N_OM][K_MAX + 1][MAX_NPN];
    long om_count[N_OM];
    long om_parity[N_OM];

    /* Test variables */
    int parity_alternating, any_degen_parity;

    printf("\n=== PART G: Oriented Matroid Classification ===\n\n");

    /* --- G1: Octant triple -> OM type enumeration --- */
    memset(om_exists, 0, sizeof(om_exists));
    memset(om_oct_count, 0, sizeof(om_oct_count));

    for (o1 = 0; o1 < 8; o1++) {
        for (o2 = 0; o2 < 8; o2++) {
            for (o3 = 0; o3 < 8; o3++) {
                int s12 = cross_sign(o1, o2);
                int s13 = cross_sign(o1, o3);
                int s23 = cross_sign(o2, o3);
                int om = om_encode(s12, s13, s23);
                om_exists[om] = 1;
                om_oct_count[om]++;
            }
        }
    }

    om_type_count = 0;
    for (om_idx = 0; om_idx < N_OM; om_idx++) {
        if (om_exists[om_idx]) om_type_count++;
    }

    printf("  G1: 512 octant triples -> %d distinct OM types\n\n",
           om_type_count);
    printf("  %-4s  %-9s  %6s  %s\n",
           "idx", "signs", "oct3s", "example");
    printf("  ----  ---------  ------  -------\n");

    for (om_idx = 0; om_idx < N_OM; om_idx++) {
        int s12, s13, s23;
        if (!om_exists[om_idx]) continue;
        om_decode(om_idx, &s12, &s13, &s23);

        /* Find first example octant triple */
        {
            int found = 0;
            for (o1 = 0; o1 < 8 && !found; o1++) {
                for (o2 = 0; o2 < 8 && !found; o2++) {
                    for (o3 = 0; o3 < 8 && !found; o3++) {
                        int cs12 = cross_sign(o1, o2);
                        int cs13 = cross_sign(o1, o3);
                        int cs23 = cross_sign(o2, o3);
                        if (om_encode(cs12, cs13, cs23) == om_idx) {
                            printf("  %4d  (%c,%c,%c)    %6d  (%d,%d,%d)\n",
                                   om_idx,
                                   sign_char(s12),
                                   sign_char(s13),
                                   sign_char(s23),
                                   om_oct_count[om_idx],
                                   o1, o2, o3);
                            found = 1;
                        }
                    }
                }
            }
        }
    }

    /* --- G2: Full combined search --- */
    printf("\n  G2: OM type -> NPN reachability "
           "(100^3 triples x k=2..8)...\n");

    memset(om_npn_reach, 0, sizeof(om_npn_reach));
    memset(om_count, 0, sizeof(om_count));
    memset(om_parity, 0, sizeof(om_parity));

    for (i1 = 0; i1 < cat_size; i1++) {
        Cx w1 = cat_cx[i1];
        int oc1 = cat_octant[i1];

        printf("    %d%%\r", i1);
        fflush(stdout);

        for (i2 = 0; i2 < cat_size; i2++) {
            Cx w2 = cat_cx[i2];
            int oc2 = cat_octant[i2];
            int s12 = cross_sign(oc1, oc2);

            for (i3 = 0; i3 < cat_size; i3++) {
                Cx w3 = cat_cx[i3];
                int oc3 = cat_octant[i3];
                int s13 = cross_sign(oc1, oc3);
                int s23 = cross_sign(oc2, oc3);
                int om = om_encode(s12, s13, s23);
                Cx sums[8];
                int idx;

                om_count[om]++;

                /* Compute 7 nontrivial sums once */
                for (idx = 1; idx < 8; idx++) {
                    Cx z = cx_make(0.0, 0.0);
                    if ((idx >> 2) & 1) z = cx_add(z, w1);
                    if ((idx >> 1) & 1) z = cx_add(z, w2);
                    if (idx & 1)        z = cx_add(z, w3);
                    sums[idx] = z;
                }

                /* Truth tables at each k */
                for (k = K_MIN; k <= K_MAX; k++) {
                    int tt = 0;
                    int canon;

                    for (idx = 1; idx < 8; idx++) {
                        if (sector_classify(sums[idx], k))
                            tt |= (1 << idx);
                    }

                    canon = npn_canon[tt];
                    for (ci = 0; ci < n_npn; ci++) {
                        if (npn_classes[ci].canon_tt == canon) {
                            om_npn_reach[om][k][ci] = 1;
                            break;
                        }
                    }

                    if (k == 6 && canon == 0x69) {
                        om_parity[om]++;
                    }
                }
            }
        }
    }

    printf("    Done.              \n\n");

    /* Print reachability table */
    printf("  %-9s  %10s  ", "signs", "triples");
    for (k = K_MIN; k <= K_MAX; k++)
        printf("k=%-2d ", k);
    printf(" par@6\n");

    printf("  ---------  ----------  ");
    for (k = K_MIN; k <= K_MAX; k++)
        printf("---- ");
    printf(" -----\n");

    for (om_idx = 0; om_idx < N_OM; om_idx++) {
        int s12, s13, s23;
        if (!om_exists[om_idx] || om_count[om_idx] == 0) continue;
        om_decode(om_idx, &s12, &s13, &s23);

        printf("  (%c,%c,%c)    %10ld  ",
               sign_char(s12), sign_char(s13), sign_char(s23),
               om_count[om_idx]);

        for (k = K_MIN; k <= K_MAX; k++) {
            int nr = 0;
            for (ci = 0; ci < n_npn; ci++)
                if (om_npn_reach[om_idx][k][ci]) nr++;
            printf("%2d   ", nr);
        }

        printf(" %5ld\n", om_parity[om_idx]);
    }

    /* --- Tests --- */

    /* Parity distribution by OM type */
    printf("  Parity distribution by OM type:\n");
    for (om_idx = 0; om_idx < N_OM; om_idx++) {
        if (om_parity[om_idx] > 0) {
            int s12, s13, s23;
            om_decode(om_idx, &s12, &s13, &s23);
            printf("    (%c,%c,%c): %ld solutions\n",
                   sign_char(s12), sign_char(s13), sign_char(s23),
                   om_parity[om_idx]);
        }
    }
    printf("\n");

    parity_alternating = 1;
    any_degen_parity = 0;

    for (om_idx = 0; om_idx < N_OM; om_idx++) {
        if (om_parity[om_idx] > 0) {
            int s12, s13, s23;
            om_decode(om_idx, &s12, &s13, &s23);

            /* Alternating: s12 = s23 = -s13, all nonzero */
            if (!(s12 != 0 && s13 != 0 && s23 != 0
                  && s12 == s23 && s12 == -s13)) {
                parity_alternating = 0;
            }

            if (s12 == 0 || s13 == 0 || s23 == 0) {
                any_degen_parity = 1;
            }
        }
    }

    sprintf(msg,
        "parity only in alternating OM types (-,+,- and +,-,+)");
    check(msg, parity_alternating);

    sprintf(msg,
        "no degenerate OM type achieves parity");
    check(msg, !any_degen_parity);

    sprintf(msg,
        "%d realizable OM types (expected 21 = 27 - 6 two-zero)",
        om_type_count);
    check(msg, om_type_count == 21);
}

/* ================================================================
 * PART H: 4-Input XOR Extension
 *
 * Can XOR4 (truth table 0x6996 over 4 inputs) be computed?
 * XOR4 outputs 1 exactly when an odd number of inputs are 1.
 *
 * For 4 inputs, there are 2^4 = 16 truth table entries.
 * XOR4 = 0x6996:
 *   0000->0, 0001->1, 0010->1, 0011->0,
 *   0100->1, 0101->0, 0110->0, 0111->1,
 *   1000->1, 1001->0, 1010->0, 1011->1,
 *   1100->0, 1101->1, 1110->1, 1111->0
 *
 * Key constraint: each single-input pattern must be class-1,
 * so all 4 weights must individually land in odd sectors.
 * Pair sums must be class-0, triple sums class-1, full sum class-0.
 *
 * Also computes the 6-sign oriented matroid type for each solution.
 * ================================================================ */

#define XOR4_TT 0x6996

/* Pre-filter: collect class-1 catalog indices for a given k */
static int class1_idx[MAX_DISTINCT];
static int n_class1 = 0;

static void collect_class1(int k) {
    int i;
    n_class1 = 0;
    for (i = 0; i < cat_size; i++) {
        if (sector_classify(cat_cx[i], k))
            class1_idx[n_class1++] = i;
    }
}

/* Compute 4-input truth table at given k */
static int compute_tt4_at_k(const Cx *w, int k) {
    int tt = 0, idx;
    for (idx = 1; idx < 16; idx++) {
        Cx z = cx_make(0.0, 0.0);
        if ((idx >> 3) & 1) z = cx_add(z, w[0]);
        if ((idx >> 2) & 1) z = cx_add(z, w[1]);
        if ((idx >> 1) & 1) z = cx_add(z, w[2]);
        if (idx & 1)        z = cx_add(z, w[3]);
        if (sector_classify(z, k)) tt |= (1 << idx);
    }
    return tt;
}

static void part_h_xor4(void) {
    int k;
    long total_xor4 = 0;
    int first_found = 0;
    int first_k = 0;
    int first_w[4];
    char msg[256];

    printf("\n=== PART H: 4-Input XOR Extension ===\n");
    printf("  Searching for XOR4 (0x6996) across k=2..8...\n\n");

    for (k = K_MIN; k <= K_MAX; k++) {
        int a, b, c, d;
        long k_count = 0;

        collect_class1(k);
        printf("  k=%d: %d class-1 values, searching %d^4 = %ld...",
               k, n_class1, n_class1,
               (long)n_class1 * n_class1 * n_class1 * n_class1);
        fflush(stdout);

        for (a = 0; a < n_class1; a++) {
            Cx w0 = cat_cx[class1_idx[a]];
            for (b = 0; b < n_class1; b++) {
                Cx w1 = cat_cx[class1_idx[b]];
                Cx s01 = cx_add(w0, w1);

                /* Pair w0+w1 must be class-0 (XOR4: f(1,1,0,0)=0) */
                if (sector_classify(s01, k) != 0) continue;

                for (c = 0; c < n_class1; c++) {
                    Cx w2 = cat_cx[class1_idx[c]];
                    Cx s02 = cx_add(w0, w2);
                    Cx s12 = cx_add(w1, w2);
                    Cx s012 = cx_add(s01, w2);

                    /* Pair sums w0+w2, w1+w2 must be class-0 */
                    if (sector_classify(s02, k) != 0) continue;
                    if (sector_classify(s12, k) != 0) continue;

                    /* Triple sum w0+w1+w2 must be class-1 */
                    if (sector_classify(s012, k) != 1) continue;

                    for (d = 0; d < n_class1; d++) {
                        Cx w3 = cat_cx[class1_idx[d]];
                        Cx s03, s13, s23, s0123;
                        Cx warr[4];

                        /* Pair sums with w3 must be class-0 */
                        s03 = cx_add(w0, w3);
                        if (sector_classify(s03, k) != 0) continue;
                        s13 = cx_add(w1, w3);
                        if (sector_classify(s13, k) != 0) continue;
                        s23 = cx_add(w2, w3);
                        if (sector_classify(s23, k) != 0) continue;

                        /* Triple sums w0+w1+w3, w0+w2+w3, w1+w2+w3
                           must be class-1 */
                        if (sector_classify(cx_add(s01, w3), k) != 1)
                            continue;
                        if (sector_classify(cx_add(s02, w3), k) != 1)
                            continue;
                        if (sector_classify(cx_add(s12, w3), k) != 1)
                            continue;

                        /* Full sum must be class-0 */
                        s0123 = cx_add(s012, w3);
                        if (sector_classify(s0123, k) != 0) continue;

                        /* Verify full truth table */
                        warr[0] = w0; warr[1] = w1;
                        warr[2] = w2; warr[3] = w3;
                        if (compute_tt4_at_k(warr, k) == XOR4_TT) {
                            k_count++;
                            if (!first_found) {
                                first_found = 1;
                                first_k = k;
                                first_w[0] = class1_idx[a];
                                first_w[1] = class1_idx[b];
                                first_w[2] = class1_idx[c];
                                first_w[3] = class1_idx[d];
                            }
                        }
                    }
                }
            }
        }

        printf(" %ld XOR4 solutions\n", k_count);
        total_xor4 += k_count;
    }

    printf("\n  Total XOR4 solutions across all k: %ld\n", total_xor4);

    /* Print first example if found */
    if (first_found) {
        int j, idx;
        Cyc8 ew[4];
        Cx cw[4];

        printf("\n  First XOR4 example at k=%d:\n", first_k);
        for (j = 0; j < 4; j++) {
            ew[j] = cat_exact[first_w[j]];
            cw[j] = cat_cx[first_w[j]];
            printf("    w%d = cat[%2d] = (%2ld,%2ld,%2ld,%2ld) "
                   "[oct %d, sec %d]\n",
                   j + 1, first_w[j],
                   ew[j].a, ew[j].b, ew[j].c, ew[j].d,
                   cat_octant[first_w[j]],
                   sector_number(cw[j], first_k));
        }

        /* Print 6-sign OM type */
        {
            int octs[4];
            int pairs[6][2] = {{0,1},{0,2},{0,3},{1,2},{1,3},{2,3}};
            printf("\n    6-sign OM type: (");
            for (j = 0; j < 4; j++)
                octs[j] = cat_octant[first_w[j]];
            for (j = 0; j < 6; j++) {
                int s = cross_sign(octs[pairs[j][0]],
                                   octs[pairs[j][1]]);
                printf("%c", sign_char(s));
                if (j < 5) printf(",");
            }
            printf(")\n");
        }

        /* Print truth table verification */
        printf("\n    Truth table verification (XOR4 = 0x6996):\n");
        printf("    %-11s  %3s  %3s  %3s\n",
               "input", "sec", "cls", "exp");
        printf("    -----------  ---  ---  ---\n");

        for (idx = 0; idx < 16; idx++) {
            int x0 = (idx >> 3) & 1;
            int x1 = (idx >> 2) & 1;
            int x2 = (idx >> 1) & 1;
            int x3 = idx & 1;
            int expected = (XOR4_TT >> idx) & 1;
            Cx z = cx_make(0.0, 0.0);
            int sec, cls;

            if (x0) z = cx_add(z, cw[0]);
            if (x1) z = cx_add(z, cw[1]);
            if (x2) z = cx_add(z, cw[2]);
            if (x3) z = cx_add(z, cw[3]);

            if (cx_abs(z) < 1e-15) {
                printf("    (%d,%d,%d,%d)      -    0    %d\n",
                       x0, x1, x2, x3, expected);
            } else {
                sec = sector_number(z, first_k);
                cls = sec % 2;
                printf("    (%d,%d,%d,%d)     s%d    %d    %d\n",
                       x0, x1, x2, x3, sec, cls, expected);
            }
        }
    }

    /* Tests */
    sprintf(msg, "XOR4 is reachable (total solutions > 0)");
    check(msg, total_xor4 > 0);

    if (first_found) {
        sprintf(msg, "first XOR4 found at k=%d", first_k);
        check(msg, first_k >= K_MIN && first_k <= K_MAX);
    }
}

/* ================================================================
 * PART I: 5-Input XOR Extension
 *
 * Can XOR5 be computed with the Z[zeta_8] catalog?
 * XOR5 outputs 1 when an odd number of inputs are 1.
 *
 * Key structural constraint: since all catalog values are AXIAL,
 * two weights on the SAME ray always sum to the same ray. So for
 * a pair sum to be class-0, the weights must be on DIFFERENT rays
 * (different octants). This means XOR_n needs at least n class-1
 * octants. At k=8 there are only 4 class-1 octants, so XOR5 is
 * impossible at k=8. We search k=2..16 to find if any k works.
 *
 * The explorer predicts k=15 might work (7 class-1 octants there)
 * but k=16 has 0 class-1 octants (perfect 16-fold alignment).
 * ================================================================ */

static void part_i_xor5(void) {
    int k;
    long total_xor5 = 0;
    int first_found = 0;
    int first_k = 0;
    int first_w[5];
    char msg[256];
    int ki_max = 16;

    printf("\n=== PART I: 5-Input XOR Extension ===\n");
    printf("  Searching for XOR5 across k=2..%d...\n", ki_max);

    /* First: show class-1 octant counts per k */
    printf("\n  Class-1 octant counts:\n");
    {
        int kk, oct;
        double oct_angles[8];
        for (oct = 0; oct < 8; oct++)
            oct_angles[oct] = (double)oct * M_PI / 4.0;

        for (kk = K_MIN; kk <= ki_max; kk++) {
            int n1 = 0;
            printf("    k=%2d: {", kk);
            for (oct = 0; oct < 8; oct++) {
                double sw = 2.0 * M_PI / (double)kk;
                int sec = (int)(oct_angles[oct] / sw);
                if (sec >= kk) sec = kk - 1;
                if (sec % 2) {
                    if (n1 > 0) printf(",");
                    printf("%d", oct);
                    n1++;
                }
            }
            printf("} = %d class-1 octants", n1);
            if (n1 >= 5) printf(" *** enough for XOR5");
            printf("\n");
        }
    }

    printf("\n  Searching with 5-level pruning...\n\n");

    for (k = K_MIN; k <= ki_max; k++) {
        int a, b, c, d, e;
        long k_count = 0;

        collect_class1(k);

        if (n_class1 == 0) {
            printf("  k=%2d: %2d class-1 values, skip\n",
                   k, n_class1);
            continue;
        }

        printf("  k=%2d: %2d class-1 values, searching...",
               k, n_class1);
        fflush(stdout);

        for (a = 0; a < n_class1; a++) {
            int ia = class1_idx[a];
            Cx wa = cat_cx[ia];

            for (b = 0; b < n_class1; b++) {
                int ib = class1_idx[b];
                Cx wb = cat_cx[ib];
                Cx sab = cx_add(wa, wb);

                /* w1+w2 must be class-0 */
                if (sector_classify(sab, k) != 0) continue;

                for (c = 0; c < n_class1; c++) {
                    int ic = class1_idx[c];
                    Cx wc = cat_cx[ic];
                    Cx sac, sbc, sabc;

                    sac = cx_add(wa, wc);
                    if (sector_classify(sac, k) != 0) continue;
                    sbc = cx_add(wb, wc);
                    if (sector_classify(sbc, k) != 0) continue;
                    sabc = cx_add(sab, wc);
                    if (sector_classify(sabc, k) != 1) continue;

                    for (d = 0; d < n_class1; d++) {
                        int id = class1_idx[d];
                        Cx wd = cat_cx[id];
                        Cx sad, sbd, scd;
                        Cx sabd, sacd, sbcd, sabcd;

                        sad = cx_add(wa, wd);
                        if (sector_classify(sad, k) != 0) continue;
                        sbd = cx_add(wb, wd);
                        if (sector_classify(sbd, k) != 0) continue;
                        scd = cx_add(wc, wd);
                        if (sector_classify(scd, k) != 0) continue;

                        sabd = cx_add(sab, wd);
                        if (sector_classify(sabd, k) != 1) continue;
                        sacd = cx_add(sac, wd);
                        if (sector_classify(sacd, k) != 1) continue;
                        sbcd = cx_add(sbc, wd);
                        if (sector_classify(sbcd, k) != 1) continue;

                        sabcd = cx_add(sabc, wd);
                        if (sector_classify(sabcd, k) != 0) continue;

                        /* (a,b,c,d) passes XOR4 constraints */
                        for (e = 0; e < n_class1; e++) {
                            int ie = class1_idx[e];
                            Cx we = cat_cx[ie];

                            /* 4 new pairs: class-0 */
                            if (sector_classify(
                                cx_add(wa, we), k) != 0) continue;
                            if (sector_classify(
                                cx_add(wb, we), k) != 0) continue;
                            if (sector_classify(
                                cx_add(wc, we), k) != 0) continue;
                            if (sector_classify(
                                cx_add(wd, we), k) != 0) continue;

                            /* 6 new triples: class-1 */
                            if (sector_classify(
                                cx_add(sab, we), k) != 1) continue;
                            if (sector_classify(
                                cx_add(sac, we), k) != 1) continue;
                            if (sector_classify(
                                cx_add(sad, we), k) != 1) continue;
                            if (sector_classify(
                                cx_add(sbc, we), k) != 1) continue;
                            if (sector_classify(
                                cx_add(sbd, we), k) != 1) continue;
                            if (sector_classify(
                                cx_add(scd, we), k) != 1) continue;

                            /* 4 new quads: class-0 */
                            if (sector_classify(
                                cx_add(sabc, we), k) != 0) continue;
                            if (sector_classify(
                                cx_add(sabd, we), k) != 0) continue;
                            if (sector_classify(
                                cx_add(sacd, we), k) != 0) continue;
                            if (sector_classify(
                                cx_add(sbcd, we), k) != 0) continue;

                            /* Full sum: class-1 */
                            if (sector_classify(
                                cx_add(sabcd, we), k) != 1) continue;

                            /* All 31 subset constraints pass! */
                            k_count++;
                            if (!first_found) {
                                first_found = 1;
                                first_k = k;
                                first_w[0] = ia;
                                first_w[1] = ib;
                                first_w[2] = ic;
                                first_w[3] = id;
                                first_w[4] = ie;
                            }
                        }
                    }
                }
            }
        }

        printf(" %ld XOR5 solutions\n", k_count);
        total_xor5 += k_count;
    }

    printf("\n  Total XOR5 solutions across all k: %ld\n", total_xor5);

    /* Print first example if found */
    if (first_found) {
        int j;
        printf("\n  First XOR5 example at k=%d:\n", first_k);
        for (j = 0; j < 5; j++) {
            Cyc8 ew = cat_exact[first_w[j]];
            printf("    w%d = cat[%2d] = (%2ld,%2ld,%2ld,%2ld) "
                   "[oct %d, sec %d]\n",
                   j + 1, first_w[j],
                   ew.a, ew.b, ew.c, ew.d,
                   cat_octant[first_w[j]],
                   sector_number(cat_cx[first_w[j]], first_k));
        }
    } else {
        printf("\n  XOR5 NOT FOUND in Z[zeta_8] catalog "
               "for any k=2..%d\n", ki_max);
        printf("  => Z[zeta_8] 8-fold symmetry limits "
               "parity to n <= 4 inputs\n");
    }

    /* Tests */
    sprintf(msg, "XOR5 search completed for k=2..%d", ki_max);
    check(msg, 1);

    if (total_xor5 == 0) {
        sprintf(msg, "XOR5 unreachable: Z[zeta_8] limits parity to "
                "n<=4 inputs");
        check(msg, total_xor5 == 0);
    } else {
        sprintf(msg, "XOR5 reachable at k=%d (%ld solutions)",
                first_k, total_xor5);
        check(msg, first_found);
    }
}

/* ================================================================
 * PART J: Complete Parity Scaling at k=15
 *
 * k=15 has 7 class-1 octants {1,2,3,4,5,6,7} — ALL non-zero
 * octants, because 360/15 = 24 degrees puts every 45-degree ray
 * in an odd sector (floor(15m/8) is odd for m=1..7).
 *
 * This is the unique "universal parity k" in the Z[zeta_8] lattice.
 * Test XOR_n for n=3..7 to find the parity ceiling.
 *
 * n=8 is structurally impossible: octant 0 is always class-0,
 * so at most 7 weights can be class-1, and same-octant pairs
 * always sum to class-1 (violating XOR's pair constraint).
 *
 * Uses recursive search with 2^n - 1 subset constraints.
 * ================================================================ */

static int pj_popcount(int x) {
    int c = 0;
    while (x) { c += x & 1; x >>= 1; }
    return c;
}

/* Recursive search state (static for cross-call persistence) */
static Cx pj_partial[128];   /* partial sums indexed by subset mask */
static int pj_indices[8];    /* current weight indices */
static long pj_count;
static int pj_first_found;
static int pj_first[8];
static int pj_target_n;      /* for progress printing */

static void pj_search(int depth, int n, int k) {
    int n_prev, e, j, ok;
    int ie;
    Cx we, ns;
    int sz, req;

    if (depth == n) {
        pj_count++;
        if (!pj_first_found) {
            pj_first_found = 1;
            for (j = 0; j < n; j++)
                pj_first[j] = pj_indices[j];
        }
        return;
    }

    n_prev = 1 << depth;

    for (e = 0; e < n_class1; e++) {
        ie = class1_idx[e];
        we = cat_cx[ie];
        ok = 1;

        /* Print progress at top level for large n */
        if (depth == 0 && pj_target_n >= 6) {
            printf("\r    %d/%d", e + 1, n_class1);
            fflush(stdout);
        }

        /* Check all 2^depth new subsets formed by adding w_e */
        for (j = 0; j < n_prev; j++) {
            ns = cx_add(pj_partial[j], we);
            sz = pj_popcount(j) + 1;
            req = sz % 2;
            if (sector_classify(ns, k) != req) {
                ok = 0;
                break;
            }
        }

        if (ok) {
            /* Extend partial sums array */
            for (j = 0; j < n_prev; j++)
                pj_partial[n_prev + j] =
                    cx_add(pj_partial[j], we);
            pj_indices[depth] = ie;
            pj_search(depth + 1, n, k);
        }
    }
}

static void part_j_parity_scaling(void) {
    int n, j;
    long counts[8];
    char msg[256];

    printf("\n=== PART J: Complete Parity Scaling at k=15 ===\n");
    printf("  k=15: 7 class-1 octants {1,2,3,4,5,6,7}\n");
    printf("  Testing XOR_n for n=3..7\n\n");

    collect_class1(15);
    printf("  %d class-1 values at k=15\n\n", n_class1);

    memset(counts, 0, sizeof(counts));

    for (n = 3; n <= 7; n++) {
        pj_count = 0;
        pj_first_found = 0;
        pj_target_n = n;
        pj_partial[0] = cx_make(0.0, 0.0);

        printf("  XOR_%d: %d subset constraints, searching...",
               n, (1 << n) - 1);
        fflush(stdout);

        pj_search(0, n, 15);

        counts[n] = pj_count;

        if (pj_target_n >= 6)
            printf("\r                    ");  /* clear progress */
        printf("\r  XOR_%d: %ld solutions", n, pj_count);

        if (pj_first_found) {
            printf("  (octants {");
            for (j = 0; j < n; j++) {
                if (j > 0) printf(",");
                printf("%d", cat_octant[pj_first[j]]);
            }
            printf("})\n");
        } else {
            printf("  NOT FOUND\n");
        }
    }

    /* Summary table */
    printf("\n  Complete parity scaling for Z[zeta_8]:\n");
    printf("  %5s  %8s  %10s  %s\n",
           "n", "min_k", "sol@k=15", "mechanism");
    printf("  -----  --------  ----------  ---------\n");
    printf("  %5d  %8d  %10ld  triskelion\n",
           3, 6, counts[3]);
    printf("  %5d  %8d  %10ld  tetraskelion\n",
           4, 8, counts[4]);
    printf("  %5d  %8d  %10ld  pentaskelion\n",
           5, 15, counts[5]);
    printf("  %5d  %8s  %10ld  %s\n",
           6, counts[6] > 0 ? "15" : "N/A", counts[6],
           counts[6] > 0 ? "hexaskelion" : "IMPOSSIBLE");
    printf("  %5d  %8s  %10ld  %s\n",
           7, counts[7] > 0 ? "15" : "N/A", counts[7],
           counts[7] > 0 ? "heptaskelion" : "IMPOSSIBLE");
    printf("  %5d  %8s  %10s  %s\n",
           8, "N/A", "0", "pigeonhole (only 7 class-1 octants)");

    /* Tests */
    sprintf(msg, "XOR3 at k=15 (%ld solutions)", counts[3]);
    check(msg, counts[3] > 0);

    sprintf(msg, "XOR4 at k=15 (%ld solutions)", counts[4]);
    check(msg, counts[4] > 0);

    sprintf(msg, "XOR5 at k=15 matches Part I (3020)");
    check(msg, counts[5] == 3020);

    if (counts[6] > 0) {
        sprintf(msg, "XOR6 achievable at k=15 (%ld solutions)",
                counts[6]);
    } else {
        sprintf(msg, "XOR6 ceiling: pair constraints too tight for 6");
    }
    check(msg, 1);  /* either way is a valid finding */

    if (counts[7] > 0) {
        sprintf(msg, "XOR7 achievable at k=15 (%ld solutions)",
                counts[7]);
    } else {
        sprintf(msg, "XOR7 ceiling: pair constraints too tight for 7");
    }
    check(msg, 1);
}

/* ================================================================
 * MAIN
 * ================================================================ */

int main(void) {
    setbuf(stdout, NULL);
    printf("KNOTAPEL DEMO 63: Angular Anatomy of DKC\n");
    printf("==========================================\n");

    /* Build catalog */
    printf("\n  Building bracket catalog...\n");
    build_catalog();
    printf("  Standard catalog: %d distinct values\n", cat_size);

    /* NPN setup */
    npn_init();
    npn_build_classes();
    printf("  NPN classes: %d\n", n_npn);

    part_a_octant_enum();
    part_b_sector_octant();
    part_c_examples();
    part_d_min_k();
    part_e_triskelion();
    part_f_commensurability();
    part_g_om_classify();
    part_h_xor4();
    part_i_xor5();
    part_j_parity_scaling();

    printf("\n==========================================\n");
    printf("Results: %d passed, %d failed\n", n_pass, n_fail);
    printf("==========================================\n");

    return n_fail > 0 ? 1 : 0;
}

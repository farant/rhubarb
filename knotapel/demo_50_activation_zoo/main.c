/*
 * KNOTAPEL DEMO 50: Activation Function Zoo
 * ==========================================
 *
 * Demo 49 proved that neglecton weights add no new angular information
 * to the bracket catalog and parity remains unreachable with split-sigmoid.
 * But Aizenberg's known parity construction uses weights IN Z[zeta_8]:
 *   w1 = zeta_8, w2 = zeta_8^2 = i, w3 = zeta_8^4 = -1
 *
 * The catch: Aizenberg requires k=8 sector classification, not k=2
 * half-plane. In fact, k=2 half-plane provably CANNOT compute 3-input
 * parity with ANY weights (complex Minsky-Papert theorem).
 *
 * This demo maps the relationship between activation geometry (number
 * of sectors k) and Boolean function computability over Z[zeta_8]:
 *
 *   Part A: k=2 impossibility (computational verification)
 *   Part B: k-sector MVN search for k=2..8 (standard catalog, no bias)
 *   Part C: Per-NPN-class minimum sector count table
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
        x.a * y.d + x.b * y.c + x.c * y.b + x.d * y.a
    );
}
static int cyc8_eq(Cyc8 x, Cyc8 y) {
    return x.a == y.a && x.b == y.b && x.c == y.c && x.d == y.d;
}
static Cx cyc8_to_cx(Cyc8 z) {
    double inv_sqrt2 = 1.0 / sqrt(2.0);
    double re = (double)z.a + (double)z.b * inv_sqrt2
              + (double)z.d * (-inv_sqrt2);
    double im = (double)z.b * inv_sqrt2 + (double)z.c
              + (double)z.d * inv_sqrt2;
    return cx_make(re, im);
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
 * Braid type + union-find loop counter
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
        bit = (int)((s >> l) & 1u);
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
 * Bracket catalog builder (standard only, from Demo 48)
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

                /* Standard bracket: loops=1 only */
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

                /* Add to catalog if nonzero and new */
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
 * NPN Classification (from Demo 48/49)
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
    int orbit_size;
    const char *titulis;
} NPNClass;

static NPNClass npn_classes[MAX_NPN];
static int n_npn = 0;

static void npn_build_classes(void) {
    int seen[256];
    int i, j;

    memset(seen, 0, sizeof(seen));
    n_npn = 0;

    for (i = 1; i < 255; i++) {
        int c = npn_canon[i];
        if (!seen[c]) {
            seen[c] = 1;
            npn_classes[n_npn].canon_tt = c;
            npn_classes[n_npn].orbit_size = 0;
            npn_classes[n_npn].titulis = fn_name(c);
            n_npn++;
        }
    }

    for (i = 1; i < 255; i++) {
        int c = npn_canon[i];
        for (j = 0; j < n_npn; j++) {
            if (npn_classes[j].canon_tt == c) {
                npn_classes[j].orbit_size++;
                break;
            }
        }
    }
}

static int npn_class_index(int tt) {
    int c = npn_canon[tt], j;
    for (j = 0; j < n_npn; j++)
        if (npn_classes[j].canon_tt == c) return j;
    return -1;
}

/* ================================================================
 * k-sector MVN classification
 *
 * Divide unit circle into k equal sectors.
 * Sector j spans [2*pi*j/k, 2*pi*(j+1)/k).
 * Output = j mod 2.
 *
 * For z = 0: output = 0 (convention).
 * ================================================================ */

static int sector_classify(Cx z, int k) {
    double angle, sector_width;
    int sector;

    if (cx_abs(z) < 1e-15) return 0;

    angle = atan2(z.im, z.re);
    if (angle < 0.0) angle += 2.0 * M_PI;

    sector_width = 2.0 * M_PI / (double)k;
    sector = (int)(angle / sector_width);
    if (sector >= k) sector = k - 1;  /* guard against rounding */

    return sector % 2;
}

/* ================================================================
 * PART A: k=2 Impossibility (Computational Verification)
 *
 * Verify that no triple (w1,w2,w3) from the catalog computes
 * XOR3 under half-plane classification (Im(z) > 0 -> 1).
 *
 * Also test XNOR3 (complementary parity).
 * No bias, so z(0,0,0) = 0 -> class 0.
 * XOR3: f(0,0,0) = 0 (compatible with z=0 -> 0)
 * XNOR3: f(0,0,0) = 1 (incompatible with z=0 -> 0, auto-fail)
 * ================================================================ */

static void part_a_impossibility(void) {
    int i1, i2, i3;
    long n_xor3 = 0;
    long n_triples = 0;
    char msg[256];

    printf("\n=== PART A: k=2 Impossibility Verification ===\n");
    printf("  Catalog: %d values\n", cat_size);
    printf("  Triples: %ld\n",
           (long)cat_size * (long)cat_size * (long)cat_size);
    printf("  Classification: half-plane (Im(z) > 0 -> 1)\n");
    printf("  No bias (z(0,0,0) = 0 -> class 0)\n\n");

    for (i1 = 0; i1 < cat_size; i1++) {
        Cx w1 = cat_cx[i1];
        for (i2 = 0; i2 < cat_size; i2++) {
            Cx w2 = cat_cx[i2];
            for (i3 = 0; i3 < cat_size; i3++) {
                Cx w3 = cat_cx[i3];
                int tt = 0;
                int idx;

                n_triples++;

                /* Bit 0: input (0,0,0) -> z=0 -> class 0 */
                /* tt bit 0 = 0 (by z=0 convention) */

                for (idx = 1; idx < 8; idx++) {
                    int x0 = (idx >> 2) & 1;
                    int x1 = (idx >> 1) & 1;
                    int x2 = idx & 1;
                    Cx z = cx_make(0.0, 0.0);
                    int cls;

                    if (x0) z = cx_add(z, w1);
                    if (x1) z = cx_add(z, w2);
                    if (x2) z = cx_add(z, w3);

                    /* k=2 half-plane */
                    cls = sector_classify(z, 2);
                    if (cls) tt |= (1 << idx);
                }

                if (npn_canon[tt] == 0x69) {
                    n_xor3++;
                }
            }
        }
    }

    printf("  Triples checked: %ld\n", n_triples);
    printf("  XOR3/XNOR3 solutions (k=2): %ld\n\n", n_xor3);

    sprintf(msg, "k=2 half-plane: zero parity solutions (got %ld)",
            n_xor3);
    check(msg, n_xor3 == 0);
}

/* ================================================================
 * PART B: k-Sector MVN Search (k=2..8)
 *
 * For each k, search all triples (w1,w2,w3) from the standard
 * catalog. No bias. For each triple, compute 8-bit truth table
 * using k-sector classification. Track solutions per NPN class.
 * ================================================================ */

#define K_MIN 2
#define K_MAX 8

/* Per-k, per-NPN-class solution counts */
static long k_class_solutions[K_MAX + 1][MAX_NPN];

/* Per-k parity solution count */
static long k_parity_solutions[K_MAX + 1];

/* Per-k total non-constant non-trivial solutions */
static long k_total_nontrivial[K_MAX + 1];

/* Per-k truth table counts */
static long k_tt_counts[K_MAX + 1][256];

/* Store first few parity examples for each k */
#define MAX_PARITY_EX 2000
typedef struct { int w1, w2, w3; } Triple;
static Triple k_parity_examples[K_MAX + 1][MAX_PARITY_EX];
static int k_n_parity_ex[K_MAX + 1];

static void search_for_k(int k) {
    int i1, i2, i3, ci, tt_idx;

    memset(k_tt_counts[k], 0, sizeof(k_tt_counts[k]));
    k_n_parity_ex[k] = 0;

    for (i1 = 0; i1 < cat_size; i1++) {
        Cx w1 = cat_cx[i1];

        for (i2 = 0; i2 < cat_size; i2++) {
            Cx w2 = cat_cx[i2];
            Cx s12 = cx_add(w1, w2);

            for (i3 = 0; i3 < cat_size; i3++) {
                Cx w3 = cat_cx[i3];
                int tt = 0;
                int idx;

                /* Bit 0: (0,0,0) -> z=0 -> sector 0 -> class 0 */
                for (idx = 1; idx < 8; idx++) {
                    int x0 = (idx >> 2) & 1;
                    int x1 = (idx >> 1) & 1;
                    int x2 = idx & 1;
                    Cx z;
                    int cls;

                    /* Build z efficiently from precomputed sums */
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

                k_tt_counts[k][tt]++;

                /* Track parity examples */
                if (npn_canon[tt] == 0x69
                    && k_n_parity_ex[k] < MAX_PARITY_EX) {
                    k_parity_examples[k][k_n_parity_ex[k]].w1 = i1;
                    k_parity_examples[k][k_n_parity_ex[k]].w2 = i2;
                    k_parity_examples[k][k_n_parity_ex[k]].w3 = i3;
                    k_n_parity_ex[k]++;
                }
            }
        }
    }

    /* Aggregate per NPN class */
    for (ci = 0; ci < n_npn; ci++) {
        int canon = npn_classes[ci].canon_tt;
        k_class_solutions[k][ci] = 0;
        for (tt_idx = 0; tt_idx < 256; tt_idx++)
            if (npn_canon[tt_idx] == canon)
                k_class_solutions[k][ci] += k_tt_counts[k][tt_idx];
    }

    /* Parity count */
    k_parity_solutions[k] = 0;
    for (tt_idx = 0; tt_idx < 256; tt_idx++)
        if (npn_canon[tt_idx] == 0x69)
            k_parity_solutions[k] += k_tt_counts[k][tt_idx];

    /* Total non-constant */
    k_total_nontrivial[k] = 0;
    for (ci = 0; ci < n_npn; ci++)
        k_total_nontrivial[k] += k_class_solutions[k][ci];
}

static void part_b_search(void) {
    int k;
    char msg[256];

    printf("\n=== PART B: k-Sector MVN Search (k=%d..%d) ===\n",
           K_MIN, K_MAX);
    printf("  Catalog: %d values, no bias\n", cat_size);
    printf("  Triples per k: %ld\n",
           (long)cat_size * (long)cat_size * (long)cat_size);
    printf("  Sector j in [2*pi*j/k, 2*pi*(j+1)/k)\n");
    printf("  Output = sector_number mod 2\n\n");

    for (k = K_MIN; k <= K_MAX; k++) {
        int ci;

        printf("  --- k=%d ---\n", k);
        search_for_k(k);

        /* Summary: which classes are reachable? */
        printf("  Parity (XOR3/XNOR3): %ld solutions\n",
               k_parity_solutions[k]);

        /* Print reachable classes */
        {
            int n_reachable = 0;
            for (ci = 0; ci < n_npn; ci++)
                if (k_class_solutions[k][ci] > 0) n_reachable++;
            printf("  Reachable NPN classes: %d/%d\n",
                   n_reachable, n_npn);
        }

        /* Per-class counts */
        printf("    %4s  %-12s  %10s\n", "TT", "Name", "Solutions");
        printf("    ----  ----------  ----------\n");
        for (ci = 0; ci < n_npn; ci++) {
            printf("    0x%02X  %-12s  %10ld%s\n",
                   npn_classes[ci].canon_tt,
                   npn_classes[ci].titulis
                       ? npn_classes[ci].titulis : "???",
                   k_class_solutions[k][ci],
                   npn_classes[ci].canon_tt == 0x69
                       ? (k_class_solutions[k][ci] > 0
                          ? "  <<<" : "")
                       : "");
        }

        /* Constants */
        printf("    Constants: FALSE=%ld  TRUE=%ld\n",
               k_tt_counts[k][0x00], k_tt_counts[k][0xFF]);

        /* Print parity examples if any */
        if (k_n_parity_ex[k] > 0) {
            int ei;
            printf("    First parity examples:\n");
            for (ei = 0; ei < k_n_parity_ex[k] && ei < 5; ei++) {
                Triple t = k_parity_examples[k][ei];
                printf("      #%d: w1=(%ld,%ld,%ld,%ld)"
                       " w2=(%ld,%ld,%ld,%ld)"
                       " w3=(%ld,%ld,%ld,%ld)\n",
                       ei,
                       cat_exact[t.w1].a, cat_exact[t.w1].b,
                       cat_exact[t.w1].c, cat_exact[t.w1].d,
                       cat_exact[t.w2].a, cat_exact[t.w2].b,
                       cat_exact[t.w2].c, cat_exact[t.w2].d,
                       cat_exact[t.w3].a, cat_exact[t.w3].b,
                       cat_exact[t.w3].c, cat_exact[t.w3].d);
            }
        }
        printf("\n");
    }

    /* Check: k=2 confirms impossibility */
    sprintf(msg, "k=2 parity = 0 (got %ld)", k_parity_solutions[2]);
    check(msg, k_parity_solutions[2] == 0);

    /* Check: Aizenberg predicts k=8 should work */
    sprintf(msg, "k=8 parity > 0 (got %ld)", k_parity_solutions[8]);
    check(msg, k_parity_solutions[8] > 0);
}

/* ================================================================
 * PART C: Per-NPN-Class Minimum Sector Count
 * ================================================================ */

static void part_c_min_sectors(void) {
    int ci, k;
    char msg[256];
    int parity_min_k = 0;

    printf("\n=== PART C: Minimum Sector Count per NPN Class ===\n\n");
    printf("  %4s  %-12s  %6s  %s\n",
           "TT", "Name", "Min k", "Solutions at min k");
    printf("  ----  ----------  ------  ------------------\n");

    for (ci = 0; ci < n_npn; ci++) {
        int min_k = 0;
        long min_k_solutions = 0;

        for (k = K_MIN; k <= K_MAX; k++) {
            if (k_class_solutions[k][ci] > 0) {
                min_k = k;
                min_k_solutions = k_class_solutions[k][ci];
                break;
            }
        }

        printf("  0x%02X  %-12s  %6s  %ld\n",
               npn_classes[ci].canon_tt,
               npn_classes[ci].titulis
                   ? npn_classes[ci].titulis : "???",
               min_k > 0 ? (sprintf(msg, "%d", min_k), msg) : "NEVER",
               min_k_solutions);

        if (npn_classes[ci].canon_tt == 0x69)
            parity_min_k = min_k;
    }

    printf("\n");

    /* The big question: at what k does parity first appear? */
    if (parity_min_k > 0) {
        sprintf(msg, "parity first appears at k=%d", parity_min_k);
        check(msg, 1);
    } else {
        sprintf(msg, "parity never appears for k=2..8");
        check(msg, 0);
    }

    /* Verify Aizenberg's specific construction */
    {
        /* w1 = zeta_8 = (0,1,0,0), w2 = i = zeta_8^2 = (0,0,1,0),
         * w3 = -1 = (-1,0,0,0) */
        Cyc8 w1_cyc = cyc8_make(0, 1, 0, 0);
        Cyc8 w2_cyc = cyc8_make(0, 0, 1, 0);
        Cyc8 w3_cyc = cyc8_make(-1, 0, 0, 0);
        Cx w1 = cyc8_to_cx(w1_cyc);
        Cx w2 = cyc8_to_cx(w2_cyc);
        Cx w3 = cyc8_to_cx(w3_cyc);
        int idx;
        int tt_k8 = 0;
        int in_catalog;

        printf("  Aizenberg construction verification:\n");
        printf("    w1 = zeta_8    = (0,1,0,0) = %.4f + %.4fi\n",
               w1.re, w1.im);
        printf("    w2 = zeta_8^2  = (0,0,1,0) = %.4f + %.4fi\n",
               w2.re, w2.im);
        printf("    w3 = zeta_8^4  = (-1,0,0,0) = %.4f + %.4fi\n",
               w3.re, w3.im);

        /* Check: are these in the catalog? */
        in_catalog = (find_in(cat_exact, cat_size, w1_cyc) >= 0)
                  && (find_in(cat_exact, cat_size, w2_cyc) >= 0)
                  && (find_in(cat_exact, cat_size, w3_cyc) >= 0);

        sprintf(msg, "Aizenberg weights in catalog");
        check(msg, in_catalog);

        /* Compute truth table under k=8 */
        for (idx = 0; idx < 8; idx++) {
            int x0 = (idx >> 2) & 1;
            int x1 = (idx >> 1) & 1;
            int x2 = idx & 1;
            Cx z = cx_make(0.0, 0.0);
            int cls;

            if (x0) z = cx_add(z, w1);
            if (x1) z = cx_add(z, w2);
            if (x2) z = cx_add(z, w3);

            cls = sector_classify(z, 8);
            if (cls) tt_k8 |= (1 << idx);

            printf("    (%d,%d,%d): z = %.4f + %.4fi,"
                   " angle = %.4f deg,"
                   " sector = %d, class = %d\n",
                   x0, x1, x2, z.re, z.im,
                   atan2(z.im, z.re) * 180.0 / M_PI,
                   (int)(((atan2(z.im, z.re) < 0
                       ? atan2(z.im, z.re) + 2.0 * M_PI
                       : atan2(z.im, z.re))
                     / (2.0 * M_PI / 8.0))),
                   cls);
        }

        printf("    Truth table: 0x%02X (NPN canon: 0x%02X = %s)\n",
               tt_k8, npn_canon[tt_k8], fn_name(npn_canon[tt_k8]));

        sprintf(msg, "Aizenberg (k=8) computes parity (got 0x%02X,"
                " NPN=0x%02X)", tt_k8, npn_canon[tt_k8]);
        check(msg, npn_canon[tt_k8] == 0x69);
    }

    /* k-sector reachability summary table */
    {
        int ki;
        printf("\n  Reachability heatmap (1=reachable, 0=not):\n");
        printf("  %4s  %-12s", "TT", "Name");
        for (ki = K_MIN; ki <= K_MAX; ki++)
            printf("  k=%d", ki);
        printf("\n  ----  ----------");
        for (ki = K_MIN; ki <= K_MAX; ki++)
            printf("  ---");
        printf("\n");

        for (ci = 0; ci < n_npn; ci++) {
            printf("  0x%02X  %-12s",
                   npn_classes[ci].canon_tt,
                   npn_classes[ci].titulis
                       ? npn_classes[ci].titulis : "???");
            for (ki = K_MIN; ki <= K_MAX; ki++) {
                printf("  %3s",
                       k_class_solutions[ki][ci] > 0 ? "Y" : "-");
            }
            printf("\n");
        }
    }
}

/* ================================================================
 * PART D: Parity Solution Analysis (k=6)
 *
 * Deep dive into the 906 parity solutions:
 *   - Which catalog entries appear as weights?
 *   - Angular distribution of weights
 *   - Symmetry patterns (permutation, conjugation)
 * ================================================================ */

static void part_d_parity_analysis(void) {
    int k = 6;
    int n_solutions = k_n_parity_ex[k];
    int weight_used[MAX_DISTINCT];  /* count per catalog index */
    int i, j;
    int n_distinct_weights = 0;
    int n_symmetric_pairs = 0;  /* (w1,w2,w3) and some permutation */

    printf("\n=== PART D: Parity Solution Analysis (k=%d) ===\n", k);
    printf("  Total parity solutions: %d\n\n", n_solutions);

    if (n_solutions == 0) {
        printf("  (no parity solutions to analyze)\n");
        return;
    }

    /* Count which catalog indices appear */
    memset(weight_used, 0, sizeof(weight_used));
    for (i = 0; i < n_solutions; i++) {
        weight_used[k_parity_examples[k][i].w1]++;
        weight_used[k_parity_examples[k][i].w2]++;
        weight_used[k_parity_examples[k][i].w3]++;
    }

    for (i = 0; i < cat_size; i++)
        if (weight_used[i] > 0) n_distinct_weights++;

    printf("  Distinct catalog values used: %d / %d\n",
           n_distinct_weights, cat_size);

    /* Print the catalog entries that appear, sorted by frequency */
    printf("\n  Weight frequency (top 20):\n");
    printf("  %4s  %16s  %8s  %5s\n",
           "Idx", "Cyc8", "|z|", "Count");
    printf("  ----  ----------------  --------  -----\n");
    {
        /* Simple selection sort for top 20 */
        int shown = 0;
        int used_flag[MAX_DISTINCT];
        memset(used_flag, 0, sizeof(used_flag));

        while (shown < 20 && shown < n_distinct_weights) {
            int best = -1, best_count = 0;
            for (i = 0; i < cat_size; i++) {
                if (!used_flag[i] && weight_used[i] > best_count) {
                    best = i;
                    best_count = weight_used[i];
                }
            }
            if (best < 0) break;
            used_flag[best] = 1;

            printf("  %4d  (%3ld,%3ld,%3ld,%3ld)  %8.4f  %5d\n",
                   best,
                   cat_exact[best].a, cat_exact[best].b,
                   cat_exact[best].c, cat_exact[best].d,
                   cx_abs(cat_cx[best]),
                   weight_used[best]);
            shown++;
        }
    }

    /* Angle distribution of weights used */
    printf("\n  Angle distribution of weights in parity solutions:\n");
    {
        /* Bucket angles into 8 octants (pi/4 each) */
        int octant_count[8];
        memset(octant_count, 0, sizeof(octant_count));

        for (i = 0; i < n_solutions; i++) {
            int widx[3];
            widx[0] = k_parity_examples[k][i].w1;
            widx[1] = k_parity_examples[k][i].w2;
            widx[2] = k_parity_examples[k][i].w3;

            for (j = 0; j < 3; j++) {
                double angle = atan2(cat_cx[widx[j]].im,
                                     cat_cx[widx[j]].re);
                int oct;
                if (angle < 0.0) angle += 2.0 * M_PI;
                oct = (int)(angle / (M_PI / 4.0));
                if (oct > 7) oct = 7;
                octant_count[oct]++;
            }
        }

        printf("    Octant  Angle range          Count\n");
        printf("    ------  -------------------  -----\n");
        for (i = 0; i < 8; i++) {
            printf("    %d       [%3d deg, %3d deg)  %5d\n",
                   i, i * 45, (i + 1) * 45,
                   octant_count[i]);
        }
    }

    /* Permutation symmetry analysis */
    printf("\n  Permutation symmetry:\n");
    {
        int n_all_distinct = 0;  /* w1 != w2 != w3 */
        int n_has_pair = 0;      /* exactly two equal */
        int n_all_same = 0;      /* w1 = w2 = w3 */

        for (i = 0; i < n_solutions; i++) {
            int w1 = k_parity_examples[k][i].w1;
            int w2 = k_parity_examples[k][i].w2;
            int w3 = k_parity_examples[k][i].w3;

            if (w1 == w2 && w2 == w3) n_all_same++;
            else if (w1 == w2 || w1 == w3 || w2 == w3) n_has_pair++;
            else n_all_distinct++;
        }

        printf("    All three distinct: %d\n", n_all_distinct);
        printf("    Exactly one pair:   %d\n", n_has_pair);
        printf("    All three same:     %d\n", n_all_same);

        /* Check how many are permutations of each other */
        for (i = 0; i < n_solutions; i++) {
            int w1 = k_parity_examples[k][i].w1;
            int w2 = k_parity_examples[k][i].w2;
            int w3 = k_parity_examples[k][i].w3;

            for (j = i + 1; j < n_solutions; j++) {
                int u1 = k_parity_examples[k][j].w1;
                int u2 = k_parity_examples[k][j].w2;
                int u3 = k_parity_examples[k][j].w3;

                /* Check all 6 permutations */
                if ((u1==w1 && u2==w2 && u3==w3) ||
                    (u1==w1 && u2==w3 && u3==w2) ||
                    (u1==w2 && u2==w1 && u3==w3) ||
                    (u1==w2 && u2==w3 && u3==w1) ||
                    (u1==w3 && u2==w1 && u3==w2) ||
                    (u1==w3 && u2==w2 && u3==w1)) {
                    n_symmetric_pairs++;
                }
            }
        }

        /* Count distinct unordered triples */
        {
            int n_ordered = n_solutions;
            /* Rough estimate: if all solutions come in groups of 6
             * permutations, n_unordered = n_ordered / 6 */
            printf("    Permutation pairs: %d\n", n_symmetric_pairs);
            printf("    (If all permutations present: ~%d"
                   " distinct unordered triples)\n",
                   n_ordered > 0
                       ? (n_symmetric_pairs > 0
                          ? n_ordered * (n_ordered - 1)
                            / (2 * n_symmetric_pairs)
                          : n_ordered)
                       : 0);
        }
    }

    /* Print first 10 solutions with full detail */
    printf("\n  First 10 parity solutions (k=%d):\n", k);
    for (i = 0; i < n_solutions && i < 10; i++) {
        Triple t = k_parity_examples[k][i];
        int idx;
        printf("    #%d: w1[%d]=(%ld,%ld,%ld,%ld)"
               " w2[%d]=(%ld,%ld,%ld,%ld)"
               " w3[%d]=(%ld,%ld,%ld,%ld)\n",
               i,
               t.w1, cat_exact[t.w1].a, cat_exact[t.w1].b,
               cat_exact[t.w1].c, cat_exact[t.w1].d,
               t.w2, cat_exact[t.w2].a, cat_exact[t.w2].b,
               cat_exact[t.w2].c, cat_exact[t.w2].d,
               t.w3, cat_exact[t.w3].a, cat_exact[t.w3].b,
               cat_exact[t.w3].c, cat_exact[t.w3].d);

        /* Show the truth table classification */
        printf("        inputs: ");
        for (idx = 0; idx < 8; idx++) {
            int x0 = (idx >> 2) & 1;
            int x1 = (idx >> 1) & 1;
            int x2 = idx & 1;
            Cx z = cx_make(0.0, 0.0);
            int cls;

            if (x0) z = cx_add(z, cat_cx[t.w1]);
            if (x1) z = cx_add(z, cat_cx[t.w2]);
            if (x2) z = cx_add(z, cat_cx[t.w3]);

            cls = sector_classify(z, k);
            printf("%d%d%d->%d ", x0, x1, x2, cls);
        }
        printf("\n");
    }

    /* Octant-set analysis: which combinations of octants appear? */
    printf("\n  Octant-set distribution (unordered):\n");
    {
        /* 4 class-1 octants: 2, 4, 5, 7. Encode combos as bitmask. */
        int combo_count[16];  /* 4-bit mask: bit0=oct2, bit1=oct4,
                                 bit2=oct5, bit3=oct7 */
        int oct_map[8];  /* map octant -> bit index, -1 if class-0 */

        memset(combo_count, 0, sizeof(combo_count));
        memset(oct_map, -1, sizeof(oct_map));
        oct_map[2] = 0; oct_map[4] = 1;
        oct_map[5] = 2; oct_map[7] = 3;

        for (i = 0; i < n_solutions; i++) {
            int widx[3];
            int mask = 0;

            widx[0] = k_parity_examples[k][i].w1;
            widx[1] = k_parity_examples[k][i].w2;
            widx[2] = k_parity_examples[k][i].w3;

            for (j = 0; j < 3; j++) {
                double angle = atan2(cat_cx[widx[j]].im,
                                     cat_cx[widx[j]].re);
                int oct, bit;
                if (angle < 0.0) angle += 2.0 * M_PI;
                oct = (int)(angle / (M_PI / 4.0));
                if (oct > 7) oct = 7;
                bit = oct_map[oct];
                if (bit >= 0) mask |= (1 << bit);
            }

            combo_count[mask]++;
        }

        {
            const char *oct_names[] = {
                "none", "{2}", "{4}", "{2,4}",
                "{5}", "{2,5}", "{4,5}", "{2,4,5}",
                "{7}", "{2,7}", "{4,7}", "{2,4,7}",
                "{5,7}", "{2,5,7}", "{4,5,7}", "{2,4,5,7}"
            };
            int m;
            for (m = 0; m < 16; m++) {
                if (combo_count[m] > 0) {
                    printf("    %-12s  %d solutions\n",
                           oct_names[m], combo_count[m]);
                }
            }
        }
    }

    /* Sector-class mapping for explanation */
    printf("\n  k=6 sector-class map for zeta_8 angles:\n");
    {
        int oi;
        for (oi = 0; oi < 8; oi++) {
            double angle = (double)oi * M_PI / 4.0;
            int sector = (int)(angle / (2.0 * M_PI / 6.0));
            if (sector >= 6) sector = 5;
            printf("    zeta_8^%d (%3d deg) -> sector %d -> class %d\n",
                   oi, oi * 45, sector, sector % 2);
        }
    }

    /* GCD analysis: incommensurability hypothesis */
    printf("\n  Incommensurability hypothesis:\n");
    printf("    k   gcd(k,8)  Parity solutions\n");
    printf("    --  --------  ----------------\n");
    {
        int ki;
        for (ki = K_MIN; ki <= K_MAX; ki++) {
            int g = ki, h = 8;
            while (h) { int t2 = h; h = g % h; g = t2; }
            printf("    %d   %8d  %16ld\n",
                   ki, g, k_parity_solutions[ki]);
        }
    }
}

/* ================================================================
 * MAIN
 * ================================================================ */

int main(void) {
    setbuf(stdout, NULL);
    printf("KNOTAPEL DEMO 50: Activation Function Zoo\n");
    printf("==========================================\n");

    /* Build catalog */
    printf("\n  Building bracket catalog...\n");
    build_catalog();
    printf("  Standard catalog: %d distinct values\n", cat_size);

    /* NPN setup */
    npn_init();
    npn_build_classes();
    printf("  NPN classes: %d\n", n_npn);

    /* Suppress unused warning */
    (void)npn_class_index;
    (void)cx_abs;

    part_a_impossibility();
    part_b_search();
    part_c_min_sectors();
    part_d_parity_analysis();

    printf("\n==========================================\n");
    printf("Results: %d passed, %d failed\n", n_pass, n_fail);
    printf("==========================================\n");

    return n_fail > 0 ? 1 : 0;
}

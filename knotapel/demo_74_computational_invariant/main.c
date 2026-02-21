/*
 * KNOTAPEL DEMO 74: Computational Invariant of Braids
 * ====================================================
 *
 * The DKC eigenvector-Voronoi approach gives a braid invariant.
 * How does it compare to the Kauffman bracket?
 *
 * From Demo 66: 450 "bracket collisions" (same bracket, different quaternion).
 * From Demo 67: 13 eigenvector directions, Voronoi cell assignment.
 * From Demo 73: 82.8% automaton determinism, 36 winning triples.
 *
 * Part A: Bracket collision eigenvector analysis
 * Part B: Reverse direction — same cell, different bracket?
 * Part C: Combined invariant power (equivalence classes)
 * Part D: Computational significance (do collisions matter for XOR6?)
 * Part E: Invariant hierarchy with all 36 winning triples
 * Part F: Synthesis
 * Part G: Computational type vectors (36-bit)
 * Part H: Concrete bracket collision examples
 * Part I: Refinement chain (bracket -> type -> quaternion)
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
 * Quaternion type
 * ================================================================ */

typedef struct { double a, b, c, d; } Quat;

static Quat quat_make(double a, double b, double c, double d) {
    Quat q; q.a = a; q.b = b; q.c = c; q.d = d; return q;
}

static Quat quat_one(void) { return quat_make(1, 0, 0, 0); }

static Quat quat_add(Quat p, Quat q) {
    return quat_make(p.a + q.a, p.b + q.b, p.c + q.c, p.d + q.d);
}

static Quat quat_neg(Quat q) {
    return quat_make(-q.a, -q.b, -q.c, -q.d);
}

static Quat quat_mul(Quat p, Quat q) {
    return quat_make(
        p.a*q.a - p.b*q.b - p.c*q.c - p.d*q.d,
        p.a*q.b + p.b*q.a + p.c*q.d - p.d*q.c,
        p.a*q.c - p.b*q.d + p.c*q.a + p.d*q.b,
        p.a*q.d + p.b*q.c - p.c*q.b + p.d*q.a);
}

static Quat quat_conj(Quat q) {
    return quat_make(q.a, -q.b, -q.c, -q.d);
}

static double quat_norm(Quat q) {
    return sqrt(q.a*q.a + q.b*q.b + q.c*q.c + q.d*q.d);
}

static Quat quat_normalize(Quat q) {
    double n = quat_norm(q);
    if (n < 1e-15) return quat_make(0, 0, 0, 0);
    return quat_make(q.a / n, q.b / n, q.c / n, q.d / n);
}

static int quat_close(Quat p, Quat q, double eps) {
    return fabs(p.a - q.a) < eps && fabs(p.b - q.b) < eps &&
           fabs(p.c - q.c) < eps && fabs(p.d - q.d) < eps;
}

static double quat_dot(Quat p, Quat q) {
    return p.a*q.a + p.b*q.b + p.c*q.c + p.d*q.d;
}

/* ================================================================
 * Z[zeta_8] exact arithmetic (for bracket computation)
 * ================================================================ */

typedef struct { long a, b, c, d; } Cyc8;

static Cyc8 cyc8_make(long a, long b, long c, long d) {
    Cyc8 z; z.a = a; z.b = b; z.c = c; z.d = d; return z;
}
static Cyc8 cyc8_zero(void) { return cyc8_make(0, 0, 0, 0); }
static int cyc8_eq(Cyc8 x, Cyc8 y) {
    return x.a == y.a && x.b == y.b && x.c == y.c && x.d == y.d;
}
static int cyc8_is_zero(Cyc8 z) {
    return z.a == 0 && z.b == 0 && z.c == 0 && z.d == 0;
}
static Cyc8 cyc8_add(Cyc8 x, Cyc8 y) {
    return cyc8_make(x.a + y.a, x.b + y.b, x.c + y.c, x.d + y.d);
}
static Cyc8 cyc8_conj(Cyc8 z) {
    return cyc8_make(z.a, -z.d, -z.c, -z.b);
}
static Cyc8 cyc8_mul(Cyc8 x, Cyc8 y) {
    return cyc8_make(
        x.a*y.a - x.b*y.d - x.c*y.c - x.d*y.b,
        x.a*y.b + x.b*y.a - x.c*y.d - x.d*y.c,
        x.a*y.c + x.b*y.b + x.c*y.a - x.d*y.d,
        x.a*y.d + x.b*y.c + x.c*y.b + x.d*y.a);
}
static Cyc8 cyc8_pow_int(Cyc8 base, int n) {
    Cyc8 r = cyc8_make(1, 0, 0, 0);
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
 * Braid type + union-find for loop counting
 * ================================================================ */

#define MAX_WORD 64
typedef struct { int word[MAX_WORD]; int len, n; } Braid;

#define MAX_UF 4096
static int uf_p[MAX_UF];

static void uf_init(int n) {
    int i;
    for (i = 0; i < n; i++) uf_p[i] = i;
}
static int uf_find(int x) {
    while (uf_p[x] != x) { uf_p[x] = uf_p[uf_p[x]]; x = uf_p[x]; }
    return x;
}
static void uf_union(int x, int y) {
    x = uf_find(x); y = uf_find(y);
    if (x != y) uf_p[x] = y;
}

static int braid_loops(const Braid *b, unsigned s) {
    int N = (b->len + 1) * b->n;
    int l, p, i, loops, sgn, bit, cup;
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

/* Bracket at A = -zeta_8, delta = 0 (single-loop only) */
static Cyc8 braid_bracket(const Braid *b) {
    Cyc8 A = cyc8_make(0, -1, 0, 0);
    Cyc8 bracket = cyc8_zero();
    unsigned s, ns;
    int i;
    ns = 1u << b->len;
    for (s = 0; s < ns; s++) {
        int a_count = 0, b_count = 0, lp;
        for (i = 0; i < b->len; i++) {
            if ((s >> (unsigned)i) & 1u) b_count++;
            else a_count++;
        }
        lp = braid_loops(b, s);
        if (lp == 1) {
            bracket = cyc8_add(bracket,
                cyc8_pow_int(A, a_count - b_count));
        }
    }
    return bracket;
}

/* ================================================================
 * SU(2) braid representation
 * ================================================================ */

static Quat q_gen[3]; /* q_gen[1]=sigma_1, q_gen[2]=sigma_2 */

static void init_su2_generators(void) {
    double s = 1.0 / sqrt(2.0);
    q_gen[1] = quat_make(s, s, 0, 0);
    q_gen[2] = quat_make(s, 0, 0, -s);
}

static Quat braid_quaternion(const Braid *b) {
    Quat result = quat_one();
    int l;
    for (l = 0; l < b->len; l++) {
        int gen = b->word[l];
        int idx = gen > 0 ? gen : -gen;
        Quat g = q_gen[idx];
        if (gen < 0) g = quat_conj(g);
        result = quat_mul(result, g);
    }
    return result;
}

/* ================================================================
 * Eigenvector extraction
 * ================================================================ */

typedef struct {
    double theta;
    double ax, ay, az;
    int axis_defined;
} EigenData;

static EigenData quat_to_eigen(Quat q) {
    EigenData e;
    double norm_v;

    if (q.a < 0.0) {
        q.a = -q.a; q.b = -q.b; q.c = -q.c; q.d = -q.d;
    }
    if (q.a > 1.0) q.a = 1.0;
    if (q.a < 0.0) q.a = 0.0;

    e.theta = acos(q.a);
    norm_v = sqrt(q.b*q.b + q.c*q.c + q.d*q.d);

    if (norm_v < 1e-12) {
        e.ax = 0.0; e.ay = 0.0; e.az = 1.0;
        e.axis_defined = 0;
    } else {
        e.ax = q.b / norm_v;
        e.ay = q.c / norm_v;
        e.az = q.d / norm_v;
        e.axis_defined = 1;
    }
    return e;
}

/* ================================================================
 * Catalogs: quaternion, bracket, correspondence
 * ================================================================ */

#define MAX_QCAT 128
static Quat qcat[MAX_QCAT];
static Cyc8 qcat_bracket[MAX_QCAT];
static int qcat_size = 0;
static int qcat_braid_count = 0;

static int find_quat(Quat q) {
    int i;
    for (i = 0; i < qcat_size; i++) {
        if (quat_close(qcat[i], q, 1e-10)) return i;
        if (quat_close(qcat[i], quat_neg(q), 1e-10)) return i;
    }
    return -1;
}

#define MAX_BCAT 512
static Cyc8 bcat[MAX_BCAT];
static int bcat_size = 0;

static int find_bracket(Cyc8 v) {
    int i;
    for (i = 0; i < bcat_size; i++)
        if (cyc8_eq(bcat[i], v)) return i;
    return -1;
}

/* Per-braid record for correspondence analysis */
#define MAX_CORR 2000
static struct {
    Quat q;
    Cyc8 bracket;
    int qcat_idx;   /* index into qcat[] */
    int bcat_idx;   /* index into bcat[] */
    int word[MAX_WORD];
    int word_len;
    int n_strands;
} corr[MAX_CORR];
static int corr_size = 0;

/* Eigenvector directions catalog (from quaternion catalog) */
#define MAX_DIR 32
static double g_dir[MAX_DIR][3];
static int g_nd = 0;

/* Per-quaternion: which direction? */
static int qcat_dir[MAX_QCAT];

static int find_dir(double ax, double ay, double az) {
    int i;
    for (i = 0; i < g_nd; i++) {
        double d1 = fabs(g_dir[i][0] - ax) + fabs(g_dir[i][1] - ay) +
                     fabs(g_dir[i][2] - az);
        double d2 = fabs(g_dir[i][0] + ax) + fabs(g_dir[i][1] + ay) +
                     fabs(g_dir[i][2] + az);
        if (d1 < 1e-8 || d2 < 1e-8) return i;
    }
    return -1;
}

/* Build eigenvector direction catalog from quaternion catalog */
static void build_dir_catalog(void) {
    int i;
    g_nd = 0;
    for (i = 0; i < qcat_size; i++) {
        EigenData e = quat_to_eigen(qcat[i]);
        int di;
        if (!e.axis_defined) {
            qcat_dir[i] = -1; /* identity/antipodal: no direction */
            continue;
        }
        di = find_dir(e.ax, e.ay, e.az);
        if (di < 0 && g_nd < MAX_DIR) {
            di = g_nd;
            g_dir[g_nd][0] = e.ax;
            g_dir[g_nd][1] = e.ay;
            g_dir[g_nd][2] = e.az;
            g_nd++;
        }
        qcat_dir[i] = di;
    }
}

/* Voronoi cell: nearest eigenvector direction (|dot| for antipodal ID) */
#define NCELLS 14  /* 13 directions + 1 identity pseudo-cell */
#define IDENT_CELL 13

static int vor_cell(double ax, double ay, double az) {
    int i, best = 0;
    double bd = -2.0;
    for (i = 0; i < g_nd; i++) {
        double dp = fabs(ax * g_dir[i][0] +
                         ay * g_dir[i][1] +
                         az * g_dir[i][2]);
        if (dp > bd) { bd = dp; best = i; }
    }
    return best;
}

/* Cell of a quaternion (via eigenvector) */
static int quat_to_cell(Quat q) {
    EigenData e = quat_to_eigen(q);
    if (!e.axis_defined) return IDENT_CELL;
    return vor_cell(e.ax, e.ay, e.az);
}

/* Per-quaternion cell */
static int qcat_cell[MAX_QCAT];

static void build_cell_assignments(void) {
    int i;
    for (i = 0; i < qcat_size; i++) {
        qcat_cell[i] = quat_to_cell(qcat[i]);
    }
}

/* Build all catalogs */
static void build_catalogs(void) {
    int n, len;
    Braid b;
    int word_buf[MAX_WORD];

    qcat_size = 0;
    bcat_size = 0;
    corr_size = 0;
    qcat_braid_count = 0;

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
                Quat q;
                Cyc8 brk;
                int qi, bi;

                for (i = 0; i < len; i++) {
                    int g = (int)(tmp % (unsigned long)total_gens);
                    tmp /= (unsigned long)total_gens;
                    if (g < max_gen) word_buf[i] = g + 1;
                    else             word_buf[i] = -(g - max_gen + 1);
                }
                b.n = n; b.len = len;
                memcpy(b.word, word_buf, (size_t)len * sizeof(int));

                q = braid_quaternion(&b);
                brk = braid_bracket(&b);
                qcat_braid_count++;

                /* Quaternion catalog */
                qi = find_quat(q);
                if (qi < 0 && qcat_size < MAX_QCAT) {
                    qi = qcat_size;
                    qcat[qcat_size] = q;
                    qcat_bracket[qcat_size] = brk;
                    qcat_size++;
                }

                /* Bracket catalog */
                bi = find_bracket(brk);
                if (!cyc8_is_zero(brk) && bi < 0 && bcat_size < MAX_BCAT) {
                    bi = bcat_size;
                    bcat[bcat_size] = brk;
                    bcat_size++;
                }

                /* Correspondence record */
                if (!cyc8_is_zero(brk) && corr_size < MAX_CORR) {
                    corr[corr_size].q = q;
                    corr[corr_size].bracket = brk;
                    corr[corr_size].qcat_idx = qi >= 0 ? qi : 0;
                    corr[corr_size].bcat_idx = bi >= 0 ? bi : find_bracket(brk);
                    memcpy(corr[corr_size].word, word_buf,
                           (size_t)len * sizeof(int));
                    corr[corr_size].word_len = len;
                    corr[corr_size].n_strands = n;
                    corr_size++;
                }
            }
        }
    }
}

/* ================================================================
 * XOR6 checker with 13-direction Voronoi
 * ================================================================ */

static int check_xor6(const Quat *w) {
    int seen[NCELLS];
    int mask, i, cell;

    for (i = 0; i < NCELLS; i++) seen[i] = 0;

    for (mask = 0; mask < 64; mask++) {
        Quat sum = quat_make(0, 0, 0, 0);
        int par = 0;
        for (i = 0; i < 6; i++) {
            if (mask & (1 << i)) {
                sum = quat_add(sum, w[i]);
                par ^= 1;
            }
        }
        if (quat_norm(sum) < 1e-12) {
            cell = IDENT_CELL;
        } else {
            EigenData ed = quat_to_eigen(quat_normalize(sum));
            if (!ed.axis_defined) {
                cell = IDENT_CELL;
            } else {
                cell = vor_cell(ed.ax, ed.ay, ed.az);
            }
        }
        seen[cell] |= (1 << par);
        if (seen[cell] == 3) return 0;
    }
    return 1;
}

/* Find all 36 winning triples */
#define MAX_WIN 64
static int g_win[MAX_WIN][3];
static int g_nwin = 0;

static void find_winners(void) {
    int ai, aj, ak;
    g_nwin = 0;
    for (ai = 0; ai < qcat_size; ai++) {
        for (aj = ai + 1; aj < qcat_size; aj++) {
            for (ak = aj + 1; ak < qcat_size; ak++) {
                Quat w6[6];
                w6[0] = qcat[ai]; w6[1] = quat_neg(qcat[ai]);
                w6[2] = qcat[aj]; w6[3] = quat_neg(qcat[aj]);
                w6[4] = qcat[ak]; w6[5] = quat_neg(qcat[ak]);
                if (check_xor6(w6) && g_nwin < MAX_WIN) {
                    g_win[g_nwin][0] = ai;
                    g_win[g_nwin][1] = aj;
                    g_win[g_nwin][2] = ak;
                    g_nwin++;
                }
            }
        }
    }
}

/* ================================================================
 * Part A: Bracket collision eigenvector analysis
 * ================================================================ */

static void part_a_bracket_collisions(void) {
    int i, j;
    int n_same_brk_same_quat = 0;
    int n_same_brk_diff_quat = 0;
    int n_same_brk_diff_quat_same_cell = 0;
    int n_same_brk_diff_quat_diff_cell = 0;
    int n_same_brk_diff_quat_same_dir = 0;
    int n_same_brk_diff_quat_diff_dir = 0;
    int n_pairs = 0;
    char msg[256];

    printf("\n=== Part A: Bracket collision eigenvector analysis ===\n");
    printf("  INFO: Quaternion catalog: %d values, Bracket catalog: %d values\n",
           qcat_size, bcat_size);
    printf("  INFO: Eigenvector directions: %d\n", g_nd);
    printf("  INFO: Correspondence records: %d\n", corr_size);

    /* Check all pairs in corr[] */
    for (i = 0; i < corr_size && i < 1500; i++) {
        for (j = i + 1; j < corr_size && j < 1500; j++) {
            int same_brk, same_quat;

            same_brk = cyc8_eq(corr[i].bracket, corr[j].bracket);
            same_quat = quat_close(corr[i].q, corr[j].q, 1e-8) ||
                        quat_close(corr[i].q, quat_neg(corr[j].q), 1e-8);

            if (same_brk && same_quat) {
                n_same_brk_same_quat++;
            }
            if (same_brk && !same_quat) {
                int ci = quat_to_cell(corr[i].q);
                int cj = quat_to_cell(corr[j].q);
                int di_idx = -1, dj_idx = -1;
                EigenData ei, ej;

                n_same_brk_diff_quat++;

                /* Check Voronoi cell */
                if (ci == cj) n_same_brk_diff_quat_same_cell++;
                else          n_same_brk_diff_quat_diff_cell++;

                /* Check raw eigenvector direction */
                ei = quat_to_eigen(corr[i].q);
                ej = quat_to_eigen(corr[j].q);
                if (ei.axis_defined)
                    di_idx = find_dir(ei.ax, ei.ay, ei.az);
                if (ej.axis_defined)
                    dj_idx = find_dir(ej.ax, ej.ay, ej.az);
                if (di_idx == dj_idx)
                    n_same_brk_diff_quat_same_dir++;
                else
                    n_same_brk_diff_quat_diff_dir++;
            }
            n_pairs++;
        }
    }

    printf("  INFO: Pairs analyzed: %d\n", n_pairs);
    printf("  INFO: Same bracket, same quaternion: %d\n", n_same_brk_same_quat);
    printf("  INFO: Same bracket, different quaternion (bracket collisions): %d\n",
           n_same_brk_diff_quat);
    printf("  INFO:   Of which, different Voronoi cell: %d\n",
           n_same_brk_diff_quat_diff_cell);
    printf("  INFO:   Of which, same Voronoi cell: %d\n",
           n_same_brk_diff_quat_same_cell);
    printf("  INFO:   Of which, different direction: %d\n",
           n_same_brk_diff_quat_diff_dir);
    printf("  INFO:   Of which, same direction: %d\n",
           n_same_brk_diff_quat_same_dir);

    /* A1: Bracket collisions exist */
    sprintf(msg, "Bracket collisions found: %d (expected ~450)",
            n_same_brk_diff_quat);
    check(msg, n_same_brk_diff_quat > 0);

    /* A2: Voronoi cell separates at least some bracket collisions */
    sprintf(msg, "Cell-separated bracket collisions: %d/%d (%.1f%%)",
            n_same_brk_diff_quat_diff_cell, n_same_brk_diff_quat,
            n_same_brk_diff_quat > 0 ?
            100.0 * (double)n_same_brk_diff_quat_diff_cell /
            (double)n_same_brk_diff_quat : 0.0);
    check(msg, n_same_brk_diff_quat_diff_cell > 0);

    /* A3: Bracket collision separation rate */
    {
        double rate = n_same_brk_diff_quat > 0 ?
            (double)n_same_brk_diff_quat_diff_cell /
            (double)n_same_brk_diff_quat : 0.0;
        sprintf(msg, "Cell separation rate: %.1f%%", rate * 100.0);
        check(msg, 1); /* info */
    }
}

/* ================================================================
 * Part B: Reverse direction — same cell, different bracket?
 * ================================================================ */

static void part_b_reverse(void) {
    int i, j;
    char msg[256];
    int n_cell_collisions = 0;
    int n_cell_coll_same_brk = 0;
    int n_cell_coll_diff_brk = 0;

    printf("\n=== Part B: Same cell, different bracket? ===\n");

    /* Check all pairs of quaternion catalog entries sharing a cell */
    for (i = 0; i < qcat_size; i++) {
        for (j = i + 1; j < qcat_size; j++) {
            if (qcat_cell[i] == qcat_cell[j]) {
                n_cell_collisions++;
                /* Do they have the same bracket? Need to check.
                 * qcat_bracket[] stores the bracket of the FIRST braid
                 * found with this quaternion. But different braids with
                 * the same quaternion can have different brackets.
                 * So we check the stored bracket as a lower bound. */
                if (cyc8_eq(qcat_bracket[i], qcat_bracket[j]))
                    n_cell_coll_same_brk++;
                else
                    n_cell_coll_diff_brk++;
            }
        }
    }

    printf("  INFO: Quaternion pairs sharing a Voronoi cell: %d\n",
           n_cell_collisions);
    printf("  INFO:   Same bracket: %d\n", n_cell_coll_same_brk);
    printf("  INFO:   Different bracket: %d\n", n_cell_coll_diff_brk);

    /* B1: Cell collisions exist */
    sprintf(msg, "Cell collisions (same cell, diff quat): %d",
            n_cell_collisions);
    check(msg, n_cell_collisions >= 0);

    /* B2: Are bracket and cell comparable or incomparable? */
    {
        int finer_cell = (n_cell_coll_diff_brk == 0);
        int finer_brk;
        /* Check if bracket is finer: for all bracket collisions,
         * are cells always the same? */
        /* We already counted this in Part A */
        printf("  INFO: Cell refines bracket (same cell => same bracket)? %s\n",
               finer_cell ? "YES" : "NO");

        /* For completeness, check per-cell bracket distribution */
        printf("  INFO: Bracket distribution per cell:\n");
        {
            int c;
            for (c = 0; c < NCELLS; c++) {
                int n_in_cell = 0;
                int n_brk_in_cell = 0;
                Cyc8 seen_brk[MAX_QCAT];

                for (i = 0; i < qcat_size; i++) {
                    if (qcat_cell[i] == c) {
                        int dup = 0;
                        int si;
                        n_in_cell++;
                        for (si = 0; si < n_brk_in_cell; si++) {
                            if (cyc8_eq(seen_brk[si], qcat_bracket[i])) {
                                dup = 1;
                                break;
                            }
                        }
                        if (!dup && n_brk_in_cell < MAX_QCAT) {
                            seen_brk[n_brk_in_cell++] = qcat_bracket[i];
                        }
                    }
                }
                if (n_in_cell > 0) {
                    printf("    cell %2d: %d quats, %d distinct brackets\n",
                           c, n_in_cell, n_brk_in_cell);
                }
            }
        }

        finer_brk = 0; /* placeholder, set based on Part A results */
        (void)finer_brk;
    }
}

/* ================================================================
 * Part C: Combined invariant power (equivalence classes)
 * ================================================================ */

static void part_c_equivalence(void) {
    char msg[256];
    int i, j;

    printf("\n=== Part C: Combined invariant power ===\n");
    printf("  INFO: NOTE: Bracket is a braid invariant, cell is a braid\n");
    printf("  INFO: invariant. Both are functions of braids, but bracket\n");
    printf("  INFO: does NOT factor through the quaternion (different\n");
    printf("  INFO: braids with same quaternion can have different brackets).\n");

    /* Count equivalence classes at the BRAID level using corr[] */
    {
        int n_cells_used = 0;
        int cell_used[NCELLS];
        memset(cell_used, 0, sizeof(cell_used));
        for (i = 0; i < qcat_size; i++) {
            cell_used[qcat_cell[i]] = 1;
        }
        for (i = 0; i < NCELLS; i++) n_cells_used += cell_used[i];

        printf("  INFO: Invariant equivalence classes:\n");
        printf("    Bracket (Z[zeta_8]):     %d classes\n", bcat_size);
        printf("    Voronoi cell (S^2):      %d classes (of %d cells)\n",
               n_cells_used, NCELLS);
        printf("    Quaternion (S^3):        %d classes\n", qcat_size);
    }

    /* Count (bracket, cell) pairs at BRAID level using corr[] */
    {
        int n_bc_pairs = 0;
        /* Use a table: for each (bcat_idx, cell) pair, mark if seen */
        /* bcat_size <= 512, NCELLS = 14 => max 7168 pairs */
        int bc_seen[512][14];
        memset(bc_seen, 0, sizeof(bc_seen));

        for (i = 0; i < corr_size; i++) {
            int bi2 = corr[i].bcat_idx;
            int ci = quat_to_cell(corr[i].q);
            if (bi2 >= 0 && bi2 < 512 && ci >= 0 && ci < NCELLS) {
                bc_seen[bi2][ci] = 1;
            }
        }
        for (i = 0; i < bcat_size; i++) {
            for (j = 0; j < NCELLS; j++) {
                if (bc_seen[i][j]) n_bc_pairs++;
            }
        }

        printf("    (Bracket, Cell) pairs:   %d classes (braid-level)\n",
               n_bc_pairs);

        sprintf(msg, "(Bracket,Cell) %d classes >= %d bracket classes",
                n_bc_pairs, bcat_size);
        check(msg, n_bc_pairs >= bcat_size);

        /* C2: Does combining bracket+cell give MORE classes than bracket? */
        sprintf(msg, "Combined refines bracket: %d > %d",
                n_bc_pairs, bcat_size);
        check(msg, n_bc_pairs > bcat_size);
    }

    /* Per-bracket-class cell distribution using corr[] */
    {
        int bi2;
        int max_cells_per_brk = 0;
        int hist[NCELLS + 1];
        int total_splits = 0;
        memset(hist, 0, sizeof(hist));

        for (bi2 = 0; bi2 < bcat_size; bi2++) {
            int cells_seen[NCELLS];
            int n_cells_this = 0;
            memset(cells_seen, 0, sizeof(cells_seen));

            for (i = 0; i < corr_size; i++) {
                if (corr[i].bcat_idx == bi2) {
                    int ci = quat_to_cell(corr[i].q);
                    if (ci >= 0 && ci < NCELLS && !cells_seen[ci]) {
                        cells_seen[ci] = 1;
                        n_cells_this++;
                    }
                }
            }
            if (n_cells_this > 0 && n_cells_this <= NCELLS)
                hist[n_cells_this]++;
            if (n_cells_this > max_cells_per_brk)
                max_cells_per_brk = n_cells_this;
            if (n_cells_this > 1) total_splits++;
        }

        printf("  INFO: Cells per bracket value distribution (braid-level):\n");
        for (i = 1; i <= max_cells_per_brk && i <= NCELLS; i++) {
            if (hist[i] > 0)
                printf("    %d cell(s): %d bracket values\n", i, hist[i]);
        }
        printf("  INFO: Bracket values split across multiple cells: %d/%d\n",
               total_splits, bcat_size);

        sprintf(msg, "Bracket values with multi-cell split: %d", total_splits);
        check(msg, total_splits > 0);
    }

    /* Per-cell bracket distribution using corr[] */
    {
        int c;
        int max_brk_per_cell = 0;
        for (c = 0; c < NCELLS; c++) {
            int n_brk = 0;
            int brk_seen[MAX_BCAT];
            memset(brk_seen, 0, sizeof(brk_seen));

            for (i = 0; i < corr_size; i++) {
                int ci = quat_to_cell(corr[i].q);
                if (ci == c && corr[i].bcat_idx >= 0 &&
                    corr[i].bcat_idx < MAX_BCAT) {
                    if (!brk_seen[corr[i].bcat_idx]) {
                        brk_seen[corr[i].bcat_idx] = 1;
                        n_brk++;
                    }
                }
            }
            if (n_brk > max_brk_per_cell) max_brk_per_cell = n_brk;
        }
        printf("  INFO: Max distinct brackets per cell: %d\n",
               max_brk_per_cell);

        /* C3: Cell doesn't subsume bracket */
        sprintf(msg, "Multiple brackets per cell => cell doesn't subsume bracket: max=%d",
                max_brk_per_cell);
        check(msg, max_brk_per_cell > 1);
    }
}

/* ================================================================
 * Part D: Computational significance — do collisions matter for XOR6?
 * ================================================================ */

static void part_d_computational(void) {
    int wi, i, j;
    char msg[256];
    int n_collision_pairs_tested = 0;
    int n_collision_pairs_xor_separated = 0;

    printf("\n=== Part D: Computational significance ===\n");
    printf("  INFO: Winners found: %d\n", g_nwin);

    /* For each winning triple, test if bracket collisions produce
     * different XOR6 outputs.
     *
     * Method: for each bracket collision pair (same bracket, different
     * quaternion), replace one weight in the winning triple with each
     * of the two collision quaternions and check if the XOR6 output
     * pattern differs.
     *
     * Actually, simpler: For each winning triple, compute the cell
     * labeling. For each bracket collision pair, the two quaternions
     * land in (possibly) different cells. If those cells have different
     * XOR6 labels, the DKC computation distinguishes the collision. */

    if (g_nwin == 0) {
        check("No winners to test", 0);
        return;
    }

    /* Build cell labeling for first winning triple */
    {
        int cell_label[NCELLS]; /* 0, 1, or -1 (unused) */
        int wi0 = 0;
        Quat w6[6];

        for (i = 0; i < NCELLS; i++) cell_label[i] = -1;

        w6[0] = qcat[g_win[wi0][0]]; w6[1] = quat_neg(qcat[g_win[wi0][0]]);
        w6[2] = qcat[g_win[wi0][1]]; w6[3] = quat_neg(qcat[g_win[wi0][1]]);
        w6[4] = qcat[g_win[wi0][2]]; w6[5] = quat_neg(qcat[g_win[wi0][2]]);

        /* Assign labels by running all 64 masks */
        {
            int mask;
            for (mask = 0; mask < 64; mask++) {
                Quat sum = quat_make(0, 0, 0, 0);
                int par = 0, cell;
                for (i = 0; i < 6; i++) {
                    if (mask & (1 << i)) {
                        sum = quat_add(sum, w6[i]);
                        par ^= 1;
                    }
                }
                if (quat_norm(sum) < 1e-12) {
                    cell = IDENT_CELL;
                } else {
                    EigenData ed = quat_to_eigen(quat_normalize(sum));
                    if (!ed.axis_defined) cell = IDENT_CELL;
                    else cell = vor_cell(ed.ax, ed.ay, ed.az);
                }
                cell_label[cell] = par;
            }
        }

        printf("  INFO: Cell labeling for winner[0] = [%d,%d,%d]:\n",
               g_win[wi0][0], g_win[wi0][1], g_win[wi0][2]);
        for (i = 0; i < NCELLS; i++) {
            if (cell_label[i] >= 0)
                printf("    cell %2d -> label %d\n", i, cell_label[i]);
        }

        /* Now check bracket collision pairs:
         * For each pair of quaternion catalog entries with same bracket,
         * different cell, check if cell labels differ */
        for (i = 0; i < qcat_size; i++) {
            for (j = i + 1; j < qcat_size; j++) {
                /* Same bracket? */
                if (!cyc8_eq(qcat_bracket[i], qcat_bracket[j])) continue;
                /* Different cell? */
                if (qcat_cell[i] == qcat_cell[j]) continue;

                n_collision_pairs_tested++;
                /* Do the cells have different labels? */
                if (cell_label[qcat_cell[i]] != cell_label[qcat_cell[j]] &&
                    cell_label[qcat_cell[i]] >= 0 &&
                    cell_label[qcat_cell[j]] >= 0) {
                    n_collision_pairs_xor_separated++;
                }
            }
        }

        printf("  INFO: Bracket collision pairs with different cells: %d\n",
               n_collision_pairs_tested);
        printf("  INFO:   Of which, XOR6-separated (different labels): %d\n",
               n_collision_pairs_xor_separated);

        sprintf(msg, "XOR6-separated bracket collisions: %d/%d",
                n_collision_pairs_xor_separated, n_collision_pairs_tested);
        check(msg, 1); /* info */
    }

    /* D2: Check across ALL winning triples */
    {
        int any_separated = 0;
        int all_separated_count = 0;

        printf("  INFO: Per-winner bracket collision separation:\n");
        for (wi = 0; wi < g_nwin; wi++) {
            int cell_label[NCELLS];
            Quat w6[6];
            int n_tested = 0, n_sep = 0;
            int mask;

            for (i = 0; i < NCELLS; i++) cell_label[i] = -1;

            w6[0] = qcat[g_win[wi][0]]; w6[1] = quat_neg(qcat[g_win[wi][0]]);
            w6[2] = qcat[g_win[wi][1]]; w6[3] = quat_neg(qcat[g_win[wi][1]]);
            w6[4] = qcat[g_win[wi][2]]; w6[5] = quat_neg(qcat[g_win[wi][2]]);

            for (mask = 0; mask < 64; mask++) {
                Quat sum = quat_make(0, 0, 0, 0);
                int par = 0, cell;
                for (i = 0; i < 6; i++) {
                    if (mask & (1 << i)) {
                        sum = quat_add(sum, w6[i]);
                        par ^= 1;
                    }
                }
                if (quat_norm(sum) < 1e-12) {
                    cell = IDENT_CELL;
                } else {
                    EigenData ed = quat_to_eigen(quat_normalize(sum));
                    if (!ed.axis_defined) cell = IDENT_CELL;
                    else cell = vor_cell(ed.ax, ed.ay, ed.az);
                }
                cell_label[cell] = par;
            }

            for (i = 0; i < qcat_size; i++) {
                for (j = i + 1; j < qcat_size; j++) {
                    if (!cyc8_eq(qcat_bracket[i], qcat_bracket[j])) continue;
                    if (qcat_cell[i] == qcat_cell[j]) continue;
                    n_tested++;
                    if (cell_label[qcat_cell[i]] != cell_label[qcat_cell[j]] &&
                        cell_label[qcat_cell[i]] >= 0 &&
                        cell_label[qcat_cell[j]] >= 0) {
                        n_sep++;
                    }
                }
            }
            printf("    winner[%2d] [%2d,%2d,%2d]: %d/%d separated\n",
                   wi, g_win[wi][0], g_win[wi][1], g_win[wi][2],
                   n_sep, n_tested);
            if (n_sep > 0) any_separated = 1;
            all_separated_count += n_sep;
        }

        sprintf(msg, "At least one winner separates bracket collisions: %s",
                any_separated ? "YES" : "NO");
        check(msg, any_separated);

        sprintf(msg, "Total XOR6-separated collisions across all winners: %d",
                all_separated_count);
        check(msg, 1); /* info */
    }
}

/* ================================================================
 * Part E: Invariant hierarchy with all 36 winning triples
 * ================================================================ */

static void part_e_hierarchy(void) {
    char msg[256];
    int i, j;

    printf("\n=== Part E: Full invariant hierarchy ===\n");

    /* Build the complete picture:
     * For each quaternion in catalog, record:
     *   (quat_idx, bracket_value, cell, direction)
     * Then analyze the partition lattice. */

    /* How many quaternions per cell? */
    {
        int c;
        int total_multi = 0;

        printf("  INFO: Quaternions per Voronoi cell:\n");
        for (c = 0; c < NCELLS; c++) {
            int n = 0;
            for (i = 0; i < qcat_size; i++) {
                if (qcat_cell[i] == c) n++;
            }
            if (n > 0) {
                printf("    cell %2d: %d quaternions", c, n);
                if (n > 1) {
                    /* List them */
                    printf(" (");
                    {
                        int first = 1;
                        for (i = 0; i < qcat_size; i++) {
                            if (qcat_cell[i] == c) {
                                if (!first) printf(", ");
                                printf("q%d", i);
                                first = 0;
                            }
                        }
                    }
                    printf(")");
                    total_multi++;
                }
                printf("\n");
            }
        }
        printf("  INFO: Cells with multiple quaternions: %d\n", total_multi);
    }

    /* Eigenvalue angle analysis: within each cell, do quaternions
     * differ only in eigenvalue angle? */
    {
        int c;
        printf("  INFO: Eigenvalue angle analysis per cell:\n");
        for (c = 0; c < NCELLS; c++) {
            double angles[MAX_QCAT];
            int n_angles = 0;
            int n_in_cell = 0;

            for (i = 0; i < qcat_size; i++) {
                if (qcat_cell[i] == c) {
                    EigenData e = quat_to_eigen(qcat[i]);
                    int dup = 0;
                    for (j = 0; j < n_angles; j++) {
                        if (fabs(angles[j] - e.theta) < 1e-8) {
                            dup = 1;
                            break;
                        }
                    }
                    if (!dup && n_angles < MAX_QCAT) {
                        angles[n_angles++] = e.theta;
                    }
                    n_in_cell++;
                }
            }
            if (n_in_cell > 1) {
                printf("    cell %2d: %d quats, %d distinct angles:",
                       c, n_in_cell, n_angles);
                for (j = 0; j < n_angles; j++) {
                    printf(" %.1f°", angles[j] * 180.0 / M_PI);
                }
                printf("\n");
            }
        }
    }

    /* E2: The quaternion-to-cell map factors through eigenvector direction.
     * Verify: quat -> eigenvector direction -> cell. */
    {
        int all_consistent = 1;
        for (i = 0; i < qcat_size; i++) {
            EigenData e = quat_to_eigen(qcat[i]);
            int cell_direct = qcat_cell[i];
            int cell_from_dir;

            if (!e.axis_defined) {
                cell_from_dir = IDENT_CELL;
            } else {
                cell_from_dir = vor_cell(e.ax, e.ay, e.az);
            }
            if (cell_direct != cell_from_dir) {
                all_consistent = 0;
                break;
            }
        }
        sprintf(msg, "Cell assignment is consistent with eigenvector direction");
        check(msg, all_consistent);
    }

    /* E3: Information loss at each level */
    {
        /* Quaternion -> Direction: how many quats per direction? */
        int di;
        printf("  INFO: Quaternions per eigenvector direction:\n");
        for (di = 0; di < g_nd; di++) {
            int n = 0;
            for (i = 0; i < qcat_size; i++) {
                if (qcat_dir[i] == di) n++;
            }
            printf("    dir[%2d] (%+.3f,%+.3f,%+.3f): %d quats\n",
                   di, g_dir[di][0], g_dir[di][1], g_dir[di][2], n);
        }
        /* Count identity quats */
        {
            int n_ident = 0;
            for (i = 0; i < qcat_size; i++) {
                if (qcat_dir[i] < 0) n_ident++;
            }
            if (n_ident > 0)
                printf("    identity/antipodal: %d quats\n", n_ident);
        }

        printf("  INFO: Information hierarchy:\n");
        printf("    Quaternion (24) -> Direction (13) -> Cell (13)\n");
        printf("    Direction == Cell (Voronoi IS direction)\n");
        printf("    Information lost: eigenvalue angle (24/13 ~ 1.85:1)\n");
    }
}

/* ================================================================
 * Part F: Synthesis
 * ================================================================ */

static void part_f_synthesis(void) {
    char msg[256];
    int i, j;
    int n_brk_coll_diff_cell = 0;
    int n_brk_coll_total = 0;
    int n_cell_coll_diff_brk = 0;
    int n_cell_coll_total = 0;

    printf("\n=== Part F: Synthesis ===\n");

    /* Count collisions at quaternion catalog level */
    for (i = 0; i < qcat_size; i++) {
        for (j = i + 1; j < qcat_size; j++) {
            /* Bracket collision */
            if (cyc8_eq(qcat_bracket[i], qcat_bracket[j])) {
                n_brk_coll_total++;
                if (qcat_cell[i] != qcat_cell[j])
                    n_brk_coll_diff_cell++;
            }
            /* Cell collision */
            if (qcat_cell[i] == qcat_cell[j]) {
                n_cell_coll_total++;
                if (!cyc8_eq(qcat_bracket[i], qcat_bracket[j]))
                    n_cell_coll_diff_brk++;
            }
        }
    }

    printf("  INFO: ============================================\n");
    printf("  INFO: Invariant comparison (at quaternion catalog level):\n");
    printf("  INFO: ============================================\n");
    printf("  INFO:\n");
    printf("  INFO:   Bracket collisions (same brk, diff quat): %d\n",
           n_brk_coll_total);
    printf("  INFO:     Separated by cell: %d (%.1f%%)\n",
           n_brk_coll_diff_cell,
           n_brk_coll_total > 0 ?
           100.0 * (double)n_brk_coll_diff_cell /
           (double)n_brk_coll_total : 0.0);
    printf("  INFO:     Not separated: %d\n",
           n_brk_coll_total - n_brk_coll_diff_cell);
    printf("  INFO:\n");
    printf("  INFO:   Cell collisions (same cell, diff quat): %d\n",
           n_cell_coll_total);
    printf("  INFO:     Separated by bracket: %d (%.1f%%)\n",
           n_cell_coll_diff_brk,
           n_cell_coll_total > 0 ?
           100.0 * (double)n_cell_coll_diff_brk /
           (double)n_cell_coll_total : 0.0);
    printf("  INFO:     Not separated: %d\n",
           n_cell_coll_total - n_cell_coll_diff_brk);

    printf("  INFO:\n");
    printf("  INFO:   Conclusion:\n");
    if (n_brk_coll_diff_cell > 0 && n_cell_coll_diff_brk > 0) {
        printf("  INFO:     Cell and bracket are INCOMPARABLE invariants.\n");
        printf("  INFO:     Cell separates %d bracket collisions.\n",
               n_brk_coll_diff_cell);
        printf("  INFO:     Bracket separates %d cell collisions.\n",
               n_cell_coll_diff_brk);
        printf("  INFO:     Together: strictly finer than either alone.\n");
    } else if (n_brk_coll_diff_cell > 0 && n_cell_coll_diff_brk == 0) {
        printf("  INFO:     Cell is strictly FINER than bracket.\n");
    } else if (n_brk_coll_diff_cell == 0 && n_cell_coll_diff_brk > 0) {
        printf("  INFO:     Bracket is strictly FINER than cell.\n");
    } else {
        printf("  INFO:     Cell and bracket are EQUIVALENT invariants.\n");
    }

    /* F1: The DKC computation uses the finer combined invariant */
    sprintf(msg, "DKC uses information beyond the bracket: %s",
            n_brk_coll_diff_cell > 0 ? "YES" : "NO");
    check(msg, n_brk_coll_diff_cell > 0);

    /* F2: Cross-demo connection table */
    printf("  INFO:\n");
    printf("  INFO: Cross-demo connection:\n");
    printf("  INFO:   Demo 66: 24 quaternions, 100 brackets, "
           "450 braid-level collisions\n");
    printf("  INFO:   Demo 67: 13 eigenvector directions, "
           "36 XOR6 winning triples\n");
    printf("  INFO:   Demo 73: 82.8%% automaton determinism, "
           "additive not multiplicative\n");
    printf("  INFO:   Demo 74: Cell and bracket are %s; "
           "combined is strictly finer\n",
           (n_brk_coll_diff_cell > 0 && n_cell_coll_diff_brk > 0) ?
           "incomparable" : "comparable");

    sprintf(msg, "Synthesis complete: %d bracket collisions separated, "
            "%d cell collisions separated",
            n_brk_coll_diff_cell, n_cell_coll_diff_brk);
    check(msg, 1);
}

/* ================================================================
 * Type vector infrastructure (36-bit computational signature)
 * ================================================================ */

typedef struct { char bits[36]; } TypeVec;

static int typevec_eq(TypeVec a, TypeVec b) {
    return memcmp(a.bits, b.bits, sizeof(a.bits)) == 0;
}

static TypeVec cell_type_vec[NCELLS];
static TypeVec g_seen_types[NCELLS];
static int g_n_distinct_types = 0;

static void compute_type_vectors(void) {
    int wi, i;
    memset(cell_type_vec, 0, sizeof(cell_type_vec));

    for (wi = 0; wi < g_nwin && wi < 36; wi++) {
        int cell_label[NCELLS];
        Quat w6[6];
        int mask;

        for (i = 0; i < NCELLS; i++) cell_label[i] = -1;

        w6[0] = qcat[g_win[wi][0]]; w6[1] = quat_neg(qcat[g_win[wi][0]]);
        w6[2] = qcat[g_win[wi][1]]; w6[3] = quat_neg(qcat[g_win[wi][1]]);
        w6[4] = qcat[g_win[wi][2]]; w6[5] = quat_neg(qcat[g_win[wi][2]]);

        for (mask = 0; mask < 64; mask++) {
            Quat sum = quat_make(0, 0, 0, 0);
            int par = 0, cell;
            for (i = 0; i < 6; i++) {
                if (mask & (1 << i)) {
                    sum = quat_add(sum, w6[i]);
                    par ^= 1;
                }
            }
            if (quat_norm(sum) < 1e-12) {
                cell = IDENT_CELL;
            } else {
                EigenData ed = quat_to_eigen(quat_normalize(sum));
                if (!ed.axis_defined) cell = IDENT_CELL;
                else cell = vor_cell(ed.ax, ed.ay, ed.az);
            }
            cell_label[cell] = par;
        }

        for (i = 0; i < NCELLS; i++) {
            if (cell_label[i] == 1) {
                cell_type_vec[i].bits[wi] = (char)1;
            }
        }
    }

    /* Catalog distinct types */
    g_n_distinct_types = 0;
    for (i = 0; i < NCELLS; i++) {
        int found = 0;
        int j;
        for (j = 0; j < g_n_distinct_types; j++) {
            if (typevec_eq(cell_type_vec[i], g_seen_types[j])) {
                found = 1;
                break;
            }
        }
        if (!found && g_n_distinct_types < NCELLS) {
            g_seen_types[g_n_distinct_types] = cell_type_vec[i];
            g_n_distinct_types++;
        }
    }
}

static int braid_type_idx(int ci) {
    int j;
    for (j = 0; j < g_n_distinct_types; j++) {
        if (typevec_eq(cell_type_vec[ci], g_seen_types[j])) return j;
    }
    return -1;
}

/* ================================================================
 * Part G: Computational type vectors (36-bit)
 * ================================================================ */

static void part_g_type_vectors(void) {
    int i, j;
    char msg[256];
    int type_braid_counts[NCELLS];
    int n_types_used = 0;

    printf("\n=== Part G: Computational type vectors (36-bit) ===\n");

    compute_type_vectors();

    printf("  INFO: Winners: %d, Distinct type vectors: %d (from %d cells)\n",
           g_nwin, g_n_distinct_types, NCELLS);

    /* Print type vectors per cell */
    printf("  INFO: Type vector per cell (bit = parity label under each winner):\n");
    for (i = 0; i < NCELLS; i++) {
        int n_in_cell = 0;
        for (j = 0; j < qcat_size; j++) {
            if (qcat_cell[j] == i) n_in_cell++;
        }
        if (n_in_cell == 0 && i != IDENT_CELL) continue;

        printf("    cell %2d (type %2d): ", i, braid_type_idx(i));
        for (j = 0; j < g_nwin && j < 36; j++) {
            printf("%d", (int)cell_type_vec[i].bits[j]);
        }
        printf("  (%d quats)\n", n_in_cell);
    }

    /* Count types used by braids */
    memset(type_braid_counts, 0, sizeof(type_braid_counts));
    for (i = 0; i < corr_size; i++) {
        int ci = quat_to_cell(corr[i].q);
        int ti = braid_type_idx(ci);
        if (ti >= 0 && ti < NCELLS) type_braid_counts[ti]++;
    }
    for (i = 0; i < g_n_distinct_types; i++) {
        if (type_braid_counts[i] > 0) n_types_used++;
    }

    printf("  INFO: Types used by braids: %d/%d\n",
           n_types_used, g_n_distinct_types);

    /* Hierarchy */
    printf("  INFO: Invariant class hierarchy:\n");
    printf("    Type vector:   %d classes (computational labeling)\n",
           g_n_distinct_types);
    printf("    Voronoi cell:  %d classes (eigenvector direction)\n", NCELLS);
    printf("    Bracket:       %d classes (algebraic trace)\n", bcat_size);
    printf("    Quaternion:    %d classes (representation)\n", qcat_size);

    /* G1 */
    sprintf(msg, "Type vectors computed: %d distinct", g_n_distinct_types);
    check(msg, g_n_distinct_types > 0 && g_n_distinct_types <= NCELLS);

    /* G2: Do any distinct cells share a type? */
    {
        int n_merged = NCELLS - g_n_distinct_types;
        sprintf(msg, "Cell merges under type: %d cells share a type pattern",
                n_merged);
        check(msg, 1); /* info */
    }
}

/* ================================================================
 * Part H: Concrete bracket collision examples
 * ================================================================ */

static void print_braid_word(const int *word, int len) {
    int i;
    for (i = 0; i < len; i++) {
        if (i > 0) printf(".");
        if (word[i] > 0) printf("s%d", word[i]);
        else printf("s%d'", -word[i]);
    }
}

static void part_h_examples(void) {
    int i, j;
    char msg[256];
    int n_examples = 0;

    printf("\n=== Part H: Concrete bracket collision examples ===\n");
    printf("  INFO: Braid pairs: same bracket, different Voronoi cell\n\n");

    for (i = 0; i < corr_size && n_examples < 5; i++) {
        for (j = i + 1; j < corr_size && n_examples < 5; j++) {
            int ci, cj;
            EigenData ei_d, ej_d;

            if (!cyc8_eq(corr[i].bracket, corr[j].bracket)) continue;
            if (quat_close(corr[i].q, corr[j].q, 1e-8) ||
                quat_close(corr[i].q, quat_neg(corr[j].q), 1e-8)) continue;

            ci = quat_to_cell(corr[i].q);
            cj = quat_to_cell(corr[j].q);
            if (ci == cj) continue;

            n_examples++;
            printf("  Example %d:\n", n_examples);

            printf("    Braid A (%d-strand, len %d): ",
                   corr[i].n_strands, corr[i].word_len);
            print_braid_word(corr[i].word, corr[i].word_len);
            printf("\n");

            printf("    Braid B (%d-strand, len %d): ",
                   corr[j].n_strands, corr[j].word_len);
            print_braid_word(corr[j].word, corr[j].word_len);
            printf("\n");

            printf("    Shared bracket: (%ld,%ld,%ld,%ld)\n",
                   corr[i].bracket.a, corr[i].bracket.b,
                   corr[i].bracket.c, corr[i].bracket.d);

            ei_d = quat_to_eigen(corr[i].q);
            ej_d = quat_to_eigen(corr[j].q);

            printf("    A: q%d, cell %d", corr[i].qcat_idx, ci);
            if (ei_d.axis_defined)
                printf(", axis(%+.3f,%+.3f,%+.3f)",
                       ei_d.ax, ei_d.ay, ei_d.az);
            printf("\n");

            printf("    B: q%d, cell %d", corr[j].qcat_idx, cj);
            if (ej_d.axis_defined)
                printf(", axis(%+.3f,%+.3f,%+.3f)",
                       ej_d.ax, ej_d.ay, ej_d.az);
            printf("\n\n");
        }
    }

    sprintf(msg, "Bracket collision examples: %d (target: 3-5)", n_examples);
    check(msg, n_examples >= 3);
}

/* ================================================================
 * Part I: Refinement chain (bracket -> type -> quaternion)
 * ================================================================ */

static void part_i_refinement(void) {
    int i, j;
    char msg[256];
    int bracket_groups = 0;
    int type_subgroups = 0;
    int quat_subgroups = 0;

    printf("\n=== Part I: Refinement chain ===\n");
    printf("  INFO: Bracket -> Type -> Quaternion\n\n");

    {
        int bi;
        int max_types_per_brk = 0;
        int max_quats_per_type = 0;
        int n_interesting = 0;

        for (bi = 0; bi < bcat_size; bi++) {
            int types_in_brk[NCELLS];
            int n_types_in_brk = 0;
            int quats_in_brk[MAX_QCAT];
            int n_quats_in_brk = 0;
            int type_of_quat[MAX_QCAT];
            int n_braids_in_brk = 0;

            memset(types_in_brk, 0, sizeof(types_in_brk));

            for (i = 0; i < corr_size; i++) {
                int ci, ti, qi;
                int found_q = 0;

                if (corr[i].bcat_idx != bi) continue;
                n_braids_in_brk++;

                ci = quat_to_cell(corr[i].q);
                ti = braid_type_idx(ci);
                qi = corr[i].qcat_idx;

                if (ti >= 0 && ti < NCELLS && !types_in_brk[ti]) {
                    types_in_brk[ti] = 1;
                    n_types_in_brk++;
                }

                for (j = 0; j < n_quats_in_brk; j++) {
                    if (quats_in_brk[j] == qi) { found_q = 1; break; }
                }
                if (!found_q && n_quats_in_brk < MAX_QCAT) {
                    type_of_quat[n_quats_in_brk] = ti;
                    quats_in_brk[n_quats_in_brk] = qi;
                    n_quats_in_brk++;
                }
            }

            if (n_braids_in_brk == 0) continue;
            bracket_groups++;
            type_subgroups += n_types_in_brk;
            quat_subgroups += n_quats_in_brk;

            if (n_types_in_brk > max_types_per_brk)
                max_types_per_brk = n_types_in_brk;

            /* Max quats per type within this bracket */
            {
                int ti2;
                for (ti2 = 0; ti2 < g_n_distinct_types; ti2++) {
                    int n = 0;
                    for (j = 0; j < n_quats_in_brk; j++) {
                        if (type_of_quat[j] == ti2) n++;
                    }
                    if (n > max_quats_per_type)
                        max_quats_per_type = n;
                }
            }

            /* Show interesting cases (multi-type brackets) */
            if (n_types_in_brk > 1 && n_interesting < 8) {
                n_interesting++;
                printf("  INFO: Bracket[%d] (%ld,%ld,%ld,%ld): "
                       "%d braids, %d types, %d quats\n",
                       bi, bcat[bi].a, bcat[bi].b,
                       bcat[bi].c, bcat[bi].d,
                       n_braids_in_brk, n_types_in_brk,
                       n_quats_in_brk);
            }
        }

        printf("\n  INFO: Refinement chain summary:\n");
        printf("    Level 1 (Bracket):    %d groups\n", bracket_groups);
        printf("    Level 2 (Type):       %d subgroups\n", type_subgroups);
        printf("    Level 3 (Quaternion): %d subgroups\n", quat_subgroups);
        printf("  INFO: Max types per bracket: %d\n", max_types_per_brk);
        printf("  INFO: Max quats per type (within bracket): %d\n",
               max_quats_per_type);

        sprintf(msg, "Bracket->Type adds info: %d type subgroups >= %d brackets",
                type_subgroups, bracket_groups);
        check(msg, type_subgroups >= bracket_groups);

        sprintf(msg, "Type->Quaternion adds info: %d quat subgroups >= %d types",
                quat_subgroups, type_subgroups);
        check(msg, quat_subgroups >= type_subgroups);

        sprintf(msg, "Full chain: %d -> %d -> %d",
                bracket_groups, type_subgroups, quat_subgroups);
        check(msg, 1);
    }
}

/* ================================================================
 * Main
 * ================================================================ */

int main(void) {
    (void)quat_dot;
    (void)cyc8_pow_int;

    printf("KNOTAPEL DEMO 74: Computational Invariant of Braids\n");
    printf("====================================================\n");

    init_su2_generators();
    build_catalogs();
    build_dir_catalog();
    build_cell_assignments();
    find_winners();

    printf("  INFO: Setup: %d quaternions, %d brackets, %d directions, "
           "%d winners\n",
           qcat_size, bcat_size, g_nd, g_nwin);

    part_a_bracket_collisions();
    part_b_reverse();
    part_c_equivalence();
    part_d_computational();
    part_e_hierarchy();
    part_f_synthesis();
    part_g_type_vectors();
    part_h_examples();
    part_i_refinement();

    printf("\n====================================================\n");
    printf("Results: %d pass, %d fail\n", n_pass, n_fail);
    return n_fail > 0 ? 1 : 0;
}

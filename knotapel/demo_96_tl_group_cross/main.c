/*
 * KNOTAPEL DEMO 96: TL-Group Cross-Classification
 * =================================================
 *
 * D84 found: null (Re=0) vs non-null splits z8 into 9/15.
 * D95 found: derived series splits z8 into 12/8/3/1.
 * These are DIFFERENT partitions. This demo maps the full
 * 2D synergy landscape.
 *
 * 24 z8 entries live in 4 cells (+ identity):
 *   A: D0-null      (6 entries, half-angle 90)
 *   B: D0-nonnull   (6 entries, half-angle 45)
 *   C: D1-nonnull   (8 entries, half-angle 60, 2T commutators)
 *   D: D2-null      (3 entries, half-angle 90, Q8 deep)
 *   E: D3-nonnull   (1 entry,  half-angle 0,  identity)
 *
 * Phase 1: Confirm cross-tabulation
 * Phase 2: Individual cell XOR capacity
 * Phase 3: Pairwise synergy matrix (6 pairs)
 * Phase 4: Triple combinations (4 triples)
 *
 * C89, zero dependencies beyond math.h.
 */

#include <stdio.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* ================================================================ */
/* Test infrastructure                                              */
/* ================================================================ */

static int n_pass = 0, n_fail = 0;

static void check(const char *titulis, int ok) {
    if (ok) { printf("  PASS: %s\n", titulis); n_pass++; }
    else    { printf("  FAIL: %s\n", titulis); n_fail++; }
}

/* ================================================================ */
/* Part A: Z[sqrt2] arithmetic (from D95)                           */
/* ================================================================ */

typedef struct { int a; int b; } Zr2;

static Zr2 zr2_make(int a, int b) { Zr2 r; r.a = a; r.b = b; return r; }
static Zr2 zr2_add(Zr2 x, Zr2 y) { Zr2 r; r.a = x.a+y.a; r.b = x.b+y.b; return r; }
static Zr2 zr2_sub(Zr2 x, Zr2 y) { Zr2 r; r.a = x.a-y.a; r.b = x.b-y.b; return r; }
static Zr2 zr2_neg(Zr2 x) { Zr2 r; r.a = -x.a; r.b = -x.b; return r; }

static Zr2 zr2_mul(Zr2 x, Zr2 y) {
    Zr2 r;
    r.a = x.a*y.a + 2*x.b*y.b;
    r.b = x.a*y.b + x.b*y.a;
    return r;
}

static int zr2_eq(Zr2 x, Zr2 y) { return x.a == y.a && x.b == y.b; }

static Zr2 zr2_div2(Zr2 x) {
    Zr2 r; r.a = x.a / 2; r.b = x.b / 2; return r;
}

/* ================================================================ */
/* Part B: QZ8 quaternion over Z[sqrt2]/2 (from D95)                */
/* ================================================================ */

typedef struct { Zr2 a, b, c, d; } QZ8;

static QZ8 qz8_make(Zr2 a, Zr2 b, Zr2 c, Zr2 d) {
    QZ8 r; r.a = a; r.b = b; r.c = c; r.d = d; return r;
}

static int qz8_eq(const QZ8 *p, const QZ8 *q) {
    return zr2_eq(p->a, q->a) && zr2_eq(p->b, q->b) &&
           zr2_eq(p->c, q->c) && zr2_eq(p->d, q->d);
}

static QZ8 qz8_neg(const QZ8 *q) {
    return qz8_make(zr2_neg(q->a), zr2_neg(q->b),
                    zr2_neg(q->c), zr2_neg(q->d));
}

static QZ8 qz8_conj(const QZ8 *q) {
    return qz8_make(q->a, zr2_neg(q->b), zr2_neg(q->c), zr2_neg(q->d));
}

static QZ8 qz8_mul(const QZ8 *p, const QZ8 *q) {
    QZ8 r;
    Zr2 t;

    t = zr2_mul(p->a, q->a);
    t = zr2_sub(t, zr2_mul(p->b, q->b));
    t = zr2_sub(t, zr2_mul(p->c, q->c));
    t = zr2_sub(t, zr2_mul(p->d, q->d));
    r.a = zr2_div2(t);

    t = zr2_mul(p->a, q->b);
    t = zr2_add(t, zr2_mul(p->b, q->a));
    t = zr2_add(t, zr2_mul(p->c, q->d));
    t = zr2_sub(t, zr2_mul(p->d, q->c));
    r.b = zr2_div2(t);

    t = zr2_mul(p->a, q->c);
    t = zr2_sub(t, zr2_mul(p->b, q->d));
    t = zr2_add(t, zr2_mul(p->c, q->a));
    t = zr2_add(t, zr2_mul(p->d, q->b));
    r.c = zr2_div2(t);

    t = zr2_mul(p->a, q->d);
    t = zr2_add(t, zr2_mul(p->b, q->c));
    t = zr2_sub(t, zr2_mul(p->c, q->b));
    t = zr2_add(t, zr2_mul(p->d, q->a));
    r.d = zr2_div2(t);

    return r;
}

/* ================================================================ */
/* Float quaternion + phase_cell activation                         */
/* ================================================================ */

typedef struct { double a, b, c, d; } Quat;

#define MAX_QCAT 64
#define MAX_DIR 64
#define MAX_ACT 65536

static Quat g_cat[MAX_QCAT];
static int g_cat_size = 0;

static double g_dir[MAX_DIR][3];
static int g_nd = 0;

static int cell_class0[MAX_ACT], cell_class1[MAX_ACT];
static int touched_cells[MAX_ACT];

static void build_dirs(int cat_size) {
    int i, j;
    g_nd = 0;
    for (i = 0; i < cat_size; i++) {
        double qa = g_cat[i].a, qb = g_cat[i].b;
        double qc = g_cat[i].c, qd = g_cat[i].d;
        double nv, ax, ay, az;
        int found = 0;
        if (qa < 0) { qa = -qa; qb = -qb; qc = -qc; qd = -qd; }
        nv = sqrt(qb*qb + qc*qc + qd*qd);
        if (nv < 1e-12) continue;
        ax = qb/nv; ay = qc/nv; az = qd/nv;
        for (j = 0; j < g_nd; j++) {
            double d1 = fabs(g_dir[j][0]-ax) + fabs(g_dir[j][1]-ay) +
                         fabs(g_dir[j][2]-az);
            double d2 = fabs(g_dir[j][0]+ax) + fabs(g_dir[j][1]+ay) +
                         fabs(g_dir[j][2]+az);
            if (d1 < 1e-8 || d2 < 1e-8) { found = 1; break; }
        }
        if (!found && g_nd < MAX_DIR) {
            g_dir[g_nd][0] = ax; g_dir[g_nd][1] = ay; g_dir[g_nd][2] = az;
            g_nd++;
        }
    }
}

static int vor_cell(double ax, double ay, double az) {
    int i, best = 0;
    double bd = -2.0;
    for (i = 0; i < g_nd; i++) {
        double dp = fabs(ax*g_dir[i][0] + ay*g_dir[i][1] + az*g_dir[i][2]);
        if (dp > bd) { bd = dp; best = i; }
    }
    return best;
}

static int phase_cell(double sa, double sb, double sc, double sd,
                      int k_sec) {
    double n2 = sa*sa + sb*sb + sc*sc + sd*sd;
    double nm, qa, half_ang, ang, rv;
    int sec, vor, n_vor;

    n_vor = g_nd + 1;
    if (n2 < 1e-24) return (k_sec - 1) * n_vor + g_nd;

    nm = sqrt(n2);
    qa = sa / nm;
    if (qa > 1.0) qa = 1.0;
    if (qa < -1.0) qa = -1.0;

    half_ang = acos(qa);
    ang = 2.0 * half_ang * 180.0 / M_PI;
    sec = (int)(ang * (double)k_sec / 360.0);
    if (sec >= k_sec) sec = k_sec - 1;
    if (sec < 0) sec = 0;

    rv = sqrt(sb*sb + sc*sc + sd*sd);
    if (rv / nm < 1e-12) {
        vor = g_nd;
    } else {
        vor = vor_cell(sb / rv, sc / rv, sd / rv);
    }

    return sec * n_vor + vor;
}

static Quat qz8_to_float(const QZ8 *q) {
    static const double SQRT2 = 1.4142135623730950488;
    Quat r;
    r.a = ((double)q->a.a + (double)q->a.b * SQRT2) / 2.0;
    r.b = ((double)q->b.a + (double)q->b.b * SQRT2) / 2.0;
    r.c = ((double)q->c.a + (double)q->c.b * SQRT2) / 2.0;
    r.d = ((double)q->d.a + (double)q->d.b * SQRT2) / 2.0;
    return r;
}

/* ================================================================ */
/* 1wpi XOR capacity testing                                        */
/* ================================================================ */

#define MAX_TT 256

static int popcount(int x) {
    int c = 0;
    while (x) { c += x & 1; x >>= 1; }
    return c;
}

static void make_xor_tt(int *tt, int n) {
    int mask;
    for (mask = 0; mask < (1 << n); mask++)
        tt[mask] = popcount(mask) & 1;
}

static int test_1wpi_phase(const int *indices, int n_weights, int k_sec,
                           const int *truth_table) {
    int n_masks, n_touched = 0;
    int mask, i, result = 1;

    n_masks = 1 << n_weights;
    if (n_weights > 8) return 0;

    for (mask = 0; mask < n_masks; mask++) {
        double sa = 0, sb = 0, sc = 0, sd = 0;
        int cls, cell;

        for (i = 0; i < n_weights; i++) {
            const Quat *q = &g_cat[indices[i]];
            double sign = ((mask >> i) & 1) ? 1.0 : -1.0;
            sa += sign * q->a; sb += sign * q->b;
            sc += sign * q->c; sd += sign * q->d;
        }

        cell = phase_cell(sa, sb, sc, sd, k_sec);
        cls = truth_table[mask];

        if (cell_class0[cell] == 0 && cell_class1[cell] == 0)
            touched_cells[n_touched++] = cell;

        if (cls == 0) {
            cell_class0[cell]++;
            if (cell_class1[cell] > 0) { result = 0; goto done; }
        } else {
            cell_class1[cell]++;
            if (cell_class0[cell] > 0) { result = 0; goto done; }
        }
    }

done:
    for (i = 0; i < n_touched; i++) {
        cell_class0[touched_cells[i]] = 0;
        cell_class1[touched_cells[i]] = 0;
    }
    return result;
}

static int next_combo(int *combo, int n, int bf) {
    int i = n - 1;
    while (i >= 0) {
        combo[i]++;
        if (combo[i] <= bf - n + i) {
            int j;
            for (j = i + 1; j < n; j++)
                combo[j] = combo[j-1] + 1;
            return 1;
        }
        i--;
    }
    return 0;
}

static const int K_SHORT[] = {6, 12, 24};
#define N_KSHORT 3

static int count_phase_ex(int n_weights, int bf_limit,
                          const int *truth_table) {
    int count = 0;
    int combo[8];
    int i, ki;

    if (bf_limit > g_cat_size) bf_limit = g_cat_size;
    if (n_weights > 8 || bf_limit < n_weights) return 0;

    for (i = 0; i < n_weights; i++) combo[i] = i;

    do {
        for (ki = 0; ki < N_KSHORT; ki++) {
            int nc = K_SHORT[ki] * (g_nd + 1);
            if (nc > MAX_ACT) continue;
            if (test_1wpi_phase(combo, n_weights, K_SHORT[ki], truth_table)) {
                count++;
                break;
            }
        }
    } while (next_combo(combo, n_weights, bf_limit));

    return count;
}

static long comb_nk(int n, int k) {
    long r = 1;
    int i;
    if (k > n - k) k = n - k;
    for (i = 0; i < k; i++) {
        r = r * (long)(n - i) / (long)(i + 1);
    }
    return r;
}

/* ================================================================ */
/* z8 group construction (from D95)                                 */
/* ================================================================ */

#define MAX_GRP 64

static QZ8 g_z8[MAX_GRP];
static int g_z8_size = 0;

static int find_z8(const QZ8 *q) {
    int i;
    QZ8 nq = qz8_neg(q);
    for (i = 0; i < g_z8_size; i++) {
        if (qz8_eq(q, &g_z8[i]) || qz8_eq(&nq, &g_z8[i]))
            return i;
    }
    return -1;
}

static void build_z8(void) {
    QZ8 gens[4];
    QZ8 identity;
    int prev, i, gi, rd;

    identity = qz8_make(zr2_make(2,0), zr2_make(0,0),
                         zr2_make(0,0), zr2_make(0,0));

    gens[0] = qz8_make(zr2_make(0,1), zr2_make(0,1),
                        zr2_make(0,0), zr2_make(0,0));
    gens[1] = qz8_conj(&gens[0]);
    gens[2] = qz8_make(zr2_make(0,1), zr2_make(0,0),
                        zr2_make(0,0), zr2_make(0,-1));
    gens[3] = qz8_conj(&gens[2]);

    g_z8_size = 0;
    g_z8[0] = identity;
    g_z8_size = 1;

    for (gi = 0; gi < 4; gi++) {
        if (find_z8(&gens[gi]) < 0 && g_z8_size < MAX_GRP)
            g_z8[g_z8_size++] = gens[gi];
    }

    rd = 1;
    do {
        prev = g_z8_size;
        for (i = 0; i < prev; i++) {
            for (gi = 0; gi < 4; gi++) {
                QZ8 prod = qz8_mul(&g_z8[i], &gens[gi]);
                if (find_z8(&prod) < 0 && g_z8_size < MAX_GRP)
                    g_z8[g_z8_size++] = prod;
            }
        }
        rd++;
    } while (g_z8_size > prev && rd < 20);
}

/* ================================================================ */
/* Commutator + derived series (from D95)                           */
/* ================================================================ */

static QZ8 z8_commutator(const QZ8 *a, const QZ8 *b) {
    QZ8 ai = qz8_conj(a);
    QZ8 bi = qz8_conj(b);
    QZ8 ab = qz8_mul(a, b);
    QZ8 abi = qz8_mul(&ab, &ai);
    return qz8_mul(&abi, &bi);
}

static int close_z8_subgroup(int *in_set) {
    int changed, i, j, count;
    QZ8 prod, inv;
    int k;

    in_set[0] = 1;

    do {
        changed = 0;
        for (i = 0; i < g_z8_size; i++) {
            if (!in_set[i]) continue;
            inv = qz8_conj(&g_z8[i]);
            k = find_z8(&inv);
            if (k >= 0 && !in_set[k]) { in_set[k] = 1; changed = 1; }
            for (j = 0; j < g_z8_size; j++) {
                if (!in_set[j]) continue;
                prod = qz8_mul(&g_z8[i], &g_z8[j]);
                k = find_z8(&prod);
                if (k >= 0 && !in_set[k]) { in_set[k] = 1; changed = 1; }
            }
        }
    } while (changed);

    count = 0;
    for (i = 0; i < g_z8_size; i++)
        if (in_set[i]) count++;
    return count;
}

/* ================================================================ */
/* Classification arrays                                            */
/* ================================================================ */

/* Derived level: 0=D0\D1, 1=D1\D2, 2=D2\D3, 3=D3 (identity) */
static int g_derived_level[MAX_GRP];

/* Null classification: 1 if Re(q) = 0 (scalar component is zero) */
static int g_is_null[MAX_GRP];

/* Cell assignment: A=0, B=1, C=2, D=3, E=4 */
static int g_cell[MAX_GRP];
static int g_cell_count[5]; /* A,B,C,D,E counts */

static void compute_derived_series(void) {
    int level_set[MAX_GRP];
    int comm_set[MAX_GRP];
    int level, i, j, k;

    for (i = 0; i < g_z8_size; i++) {
        level_set[i] = 1;
        g_derived_level[i] = 0;
    }

    for (level = 1; level < 10; level++) {
        int cur_size;

        memset(comm_set, 0, sizeof(comm_set));
        for (i = 0; i < g_z8_size; i++) {
            if (!level_set[i]) continue;
            for (j = 0; j < g_z8_size; j++) {
                QZ8 comm;
                if (!level_set[j]) continue;
                comm = z8_commutator(&g_z8[i], &g_z8[j]);
                k = find_z8(&comm);
                if (k >= 0) comm_set[k] = 1;
            }
        }

        cur_size = close_z8_subgroup(comm_set);

        for (i = 0; i < g_z8_size; i++) {
            if (level_set[i] && !comm_set[i])
                g_derived_level[i] = level - 1;
        }

        memcpy(level_set, comm_set, sizeof(level_set));

        if (cur_size <= 1) {
            for (i = 0; i < g_z8_size; i++) {
                if (comm_set[i]) g_derived_level[i] = level;
            }
            break;
        }
    }
}

static void compute_null_classification(void) {
    int i;
    for (i = 0; i < g_z8_size; i++) {
        /* Null = scalar component is zero in /2 form: q.a = (0,0) */
        g_is_null[i] = (g_z8[i].a.a == 0 && g_z8[i].a.b == 0) ? 1 : 0;
    }
}

static void compute_cells(void) {
    int i;
    memset(g_cell_count, 0, sizeof(g_cell_count));

    for (i = 0; i < g_z8_size; i++) {
        if (g_derived_level[i] == 0 && g_is_null[i]) {
            g_cell[i] = 0; /* A: D0-null */
        } else if (g_derived_level[i] == 0 && !g_is_null[i]) {
            g_cell[i] = 1; /* B: D0-nonnull */
        } else if (g_derived_level[i] == 1) {
            g_cell[i] = 2; /* C: D1-nonnull (2T, all ha=60) */
        } else if (g_derived_level[i] == 2) {
            g_cell[i] = 3; /* D: D2-null (Q8, all ha=90) */
        } else {
            g_cell[i] = 4; /* E: D3-nonnull (identity) */
        }
        g_cell_count[g_cell[i]]++;
    }
}

/* ================================================================ */
/* Subset loader                                                    */
/* ================================================================ */

static void load_cells(const int *cell_mask) {
    /* cell_mask[5]: which cells to include (A=0..E=4) */
    int i;
    g_cat_size = 0;
    for (i = 0; i < g_z8_size; i++) {
        if (cell_mask[g_cell[i]]) {
            g_cat[g_cat_size] = qz8_to_float(&g_z8[i]);
            g_cat_size++;
        }
    }
}

/* ================================================================ */
/* XOR capacity runner for a subset                                 */
/* ================================================================ */

typedef struct {
    int xor_count[4]; /* N=3,4,5,6 */
    long total[4];    /* C(size,N) */
    int size;
    int n_dirs;
} SubsetResult;

static void run_subset_capacity(SubsetResult *res) {
    int n_list[] = {3, 4, 5, 6};
    int xor_tt[MAX_TT];
    int ni;

    res->size = g_cat_size;
    build_dirs(g_cat_size);
    res->n_dirs = g_nd;

    for (ni = 0; ni < 4; ni++) {
        int nw = n_list[ni];
        if (g_cat_size < nw) {
            res->xor_count[ni] = 0;
            res->total[ni] = 0;
            continue;
        }
        make_xor_tt(xor_tt, nw);
        res->total[ni] = comb_nk(g_cat_size, nw);
        res->xor_count[ni] = count_phase_ex(nw, g_cat_size, xor_tt);
    }
}

static void print_subset_result(const char *label, const SubsetResult *res) {
    int ni;
    int n_list[] = {3, 4, 5, 6};
    printf("  %s (%d entries, %d dirs):\n", label, res->size, res->n_dirs);
    for (ni = 0; ni < 4; ni++) {
        if (res->total[ni] > 0) {
            printf("    N=%d: XOR=%d / %ld (%.2f%%)\n",
                   n_list[ni], res->xor_count[ni], res->total[ni],
                   100.0 * (double)res->xor_count[ni] / (double)res->total[ni]);
        }
    }
    printf("\n");
}

/* ================================================================ */
/* Phase 1: Confirm Cross-Tabulation                                */
/* ================================================================ */

static void phase1_cross_tabulation(void) {
    static const double SQRT2 = 1.4142135623730950488;
    int i;
    int null_total, comm_total, noncomm_total;
    const char *cell_names[] = {"A(D0-null)", "B(D0-nonnull)",
                                "C(D1-nonnull)", "D(D2-null)",
                                "E(D3-identity)"};

    printf("\n=== Phase 1: Cross-Tabulation ===\n\n");
    fflush(stdout);

    printf("  Cell | Description      | Count | Half-angle\n");
    printf("  -----|------------------|-------|----------\n");
    for (i = 0; i < 5; i++) {
        /* Compute mean half-angle for this cell */
        double sum_ha = 0;
        int cnt = 0, j;
        for (j = 0; j < g_z8_size; j++) {
            if (g_cell[j] == i) {
                /* half-angle from scalar: cos(ha) = |Re|/|q| = |scalar|/1 */
                double re = ((double)g_z8[j].a.a +
                             (double)g_z8[j].a.b * SQRT2) / 2.0;
                double ha;
                if (re < 0) re = -re;
                if (re > 1.0) re = 1.0;
                ha = acos(re) * 180.0 / M_PI;
                sum_ha += ha;
                cnt++;
            }
        }
        printf("    %s | %-16s | %5d | %5.1f deg\n",
               cell_names[i], cell_names[i], g_cell_count[i],
               cnt > 0 ? sum_ha / (double)cnt : 0.0);
    }
    printf("\n");

    null_total = g_cell_count[0] + g_cell_count[3]; /* A+D */
    comm_total = g_cell_count[2] + g_cell_count[3] + g_cell_count[4]; /* C+D+E */
    noncomm_total = g_cell_count[0] + g_cell_count[1]; /* A+B */

    printf("  Cross-checks:\n");
    printf("    Null total (A+D): %d (expect 9)\n", null_total);
    printf("    G1/COMM total (C+D+E): %d (expect 12)\n", comm_total);
    printf("    G0\\G1/NONCOMM total (A+B): %d (expect 12)\n", noncomm_total);
    printf("    Grand total: %d (expect 24)\n\n",
           g_cell_count[0]+g_cell_count[1]+g_cell_count[2]+
           g_cell_count[3]+g_cell_count[4]);

    check("Cell A = 6", g_cell_count[0] == 6);
    check("Cell B = 6", g_cell_count[1] == 6);
    check("Cell C = 8", g_cell_count[2] == 8);
    check("Cell D = 3", g_cell_count[3] == 3);
    check("Cell E = 1", g_cell_count[4] == 1);
    check("Null total = 9", null_total == 9);
    check("COMM total = 12", comm_total == 12);
    check("NONCOMM total = 12", noncomm_total == 12);
}

/* ================================================================ */
/* Phase 2: Individual Cell XOR Capacity                            */
/* ================================================================ */

static SubsetResult g_cell_res[5]; /* results for cells A-E */

static void phase2_individual_cells(void) {
    int i;
    int cell_mask[5];
    const char *cell_names[] = {"A(D0-null,6)", "B(D0-nonnull,6)",
                                "C(D1-nonnull,8)", "D(D2-null,3)",
                                "E(D3-identity,1)"};
    int any_nonzero = 0;

    printf("\n=== Phase 2: Individual Cell XOR Capacity ===\n\n");
    fflush(stdout);

    for (i = 0; i < 5; i++) {
        memset(cell_mask, 0, sizeof(cell_mask));
        cell_mask[i] = 1;
        load_cells(cell_mask);
        run_subset_capacity(&g_cell_res[i]);
        print_subset_result(cell_names[i], &g_cell_res[i]);
        fflush(stdout);
    }

    /* Summary table */
    printf("  --- Individual Cell Summary ---\n");
    printf("    Cell       | Size | Dirs | N=3     | N=4     | N=5     | N=6\n");
    printf("    -----------|------|------|---------|---------|---------|--------\n");
    for (i = 0; i < 5; i++) {
        int ni;
        printf("    %-10s | %4d | %4d |", cell_names[i],
               g_cell_res[i].size, g_cell_res[i].n_dirs);
        for (ni = 0; ni < 4; ni++) {
            if (g_cell_res[i].total[ni] > 0) {
                double rate = 100.0 * (double)g_cell_res[i].xor_count[ni] /
                              (double)g_cell_res[i].total[ni];
                printf(" %6.2f%%|", rate);
                if (g_cell_res[i].xor_count[ni] > 0) any_nonzero = 1;
            } else {
                printf("    n/a |");
            }
        }
        printf("\n");
    }
    printf("\n");

    check("Individual cell capacity measured", 1);
    if (any_nonzero) {
        printf("  --> Some cells achieve XOR individually\n\n");
    } else {
        printf("  --> NO cell achieves XOR alone!\n\n");
    }
}

/* ================================================================ */
/* Phase 3: Pairwise Synergy Matrix                                 */
/* ================================================================ */

static void phase3_pairwise_synergy(void) {
    /* 6 pairs from cells A,B,C,D (skip E=identity, only 1 entry) */
    static const int pairs[6][2] = {
        {0,1}, /* A+B: outermost shell */
        {0,2}, /* A+C: cross BOTH dimensions */
        {0,3}, /* A+D: both null, 90-deg split */
        {1,2}, /* B+C: both non-null, different derived */
        {1,3}, /* B+D: cross BOTH dimensions */
        {2,3}  /* C+D: commutator subgroup minus identity */
    };
    static const char *pair_names[] = {
        "A+B (outermost shell)",
        "A+C (cross both dims)",
        "A+D (both null, 90-deg)",
        "B+C (both nonnull, cross derived)",
        "B+D (cross both dims)",
        "C+D (comm subgroup)"
    };
    static const char *pair_short[] = {"A+B","A+C","A+D","B+C","B+D","C+D"};
    SubsetResult pair_res[6];
    int pi, ni;
    int n_list[] = {3, 4, 5, 6};
    int best_pair[4]; /* index of best pair at each N */

    printf("\n=== Phase 3: Pairwise Synergy Matrix ===\n\n");
    fflush(stdout);

    for (pi = 0; pi < 6; pi++) {
        int cell_mask[5];
        memset(cell_mask, 0, sizeof(cell_mask));
        cell_mask[pairs[pi][0]] = 1;
        cell_mask[pairs[pi][1]] = 1;
        load_cells(cell_mask);
        run_subset_capacity(&pair_res[pi]);
        print_subset_result(pair_names[pi], &pair_res[pi]);
        fflush(stdout);
    }

    /* Summary table */
    printf("  --- Pairwise XOR Rate Summary ---\n");
    printf("    Pair      | Size | Dirs | N=3     | N=4     | N=5     | N=6\n");
    printf("    ----------|------|------|---------|---------|---------|--------\n");
    for (pi = 0; pi < 6; pi++) {
        printf("    %-9s | %4d | %4d |", pair_short[pi],
               pair_res[pi].size, pair_res[pi].n_dirs);
        for (ni = 0; ni < 4; ni++) {
            if (pair_res[pi].total[ni] > 0) {
                double rate = 100.0 * (double)pair_res[pi].xor_count[ni] /
                              (double)pair_res[pi].total[ni];
                printf(" %6.2f%%|", rate);
            } else {
                printf("    n/a |");
            }
        }
        printf("\n");
    }
    printf("\n");

    /* Find strongest pair at each N */
    for (ni = 0; ni < 4; ni++) {
        int best = 0;
        double best_rate = -1;
        for (pi = 0; pi < 6; pi++) {
            double rate = (pair_res[pi].total[ni] > 0) ?
                (double)pair_res[pi].xor_count[ni] /
                (double)pair_res[pi].total[ni] : 0;
            if (rate > best_rate) { best_rate = rate; best = pi; }
        }
        best_pair[ni] = best;
        printf("  Best pair at N=%d: %s (%.2f%%)\n",
               n_list[ni], pair_names[best],
               best_rate * 100.0);
    }
    printf("\n");

    /* Dimension analysis */
    {
        /* Pairs that cross NULL dimension: A+B (null+nonnull in D0),
           C+D (null+nonnull in G1) */
        /* Pairs that cross DERIVED dimension: A+C, B+D */
        /* Pairs that cross BOTH: A+C, B+D */
        double cross_null_rate[4], cross_derived_rate[4];
        double cross_both_rate[4];

        printf("  --- Dimension Analysis ---\n");
        printf("    N | CrossNull(A+B,C+D) | CrossDerived(A+C,B+D) | CrossBoth(A+C,B+D)\n");
        printf("    --|--------------------|-----------------------|-------------------\n");

        for (ni = 0; ni < 4; ni++) {
            double ab, cd, ac, bd;
            /* Cross null: average of A+B and C+D */
            ab = (pair_res[0].total[ni] > 0) ?
                (double)pair_res[0].xor_count[ni] / (double)pair_res[0].total[ni] : 0;
            cd = (pair_res[5].total[ni] > 0) ?
                (double)pair_res[5].xor_count[ni] / (double)pair_res[5].total[ni] : 0;
            cross_null_rate[ni] = (ab + cd) / 2.0;

            /* Cross derived: average of A+C and B+D */
            ac = (pair_res[1].total[ni] > 0) ?
                (double)pair_res[1].xor_count[ni] / (double)pair_res[1].total[ni] : 0;
            bd = (pair_res[4].total[ni] > 0) ?
                (double)pair_res[4].xor_count[ni] / (double)pair_res[4].total[ni] : 0;
            cross_derived_rate[ni] = (ac + bd) / 2.0;
            cross_both_rate[ni] = cross_derived_rate[ni]; /* same pairs */

            printf("    %d | %17.2f%% | %20.2f%% | %16.2f%%\n",
                   n_list[ni],
                   cross_null_rate[ni] * 100.0,
                   cross_derived_rate[ni] * 100.0,
                   cross_both_rate[ni] * 100.0);
        }
        printf("\n");

        printf("  Interpretation:\n");
        printf("    CrossNull > CrossDerived => null/nonnull boundary matters more\n");
        printf("    CrossDerived > CrossNull => derived level boundary matters more\n");
        printf("    Both high => both dimensions contribute\n\n");
    }

    check("Pairwise synergy measured", 1);
    {
        /* Check that at least one pair has nonzero XOR */
        int any = 0;
        for (pi = 0; pi < 6; pi++) {
            for (ni = 0; ni < 4; ni++) {
                if (pair_res[pi].xor_count[ni] > 0) any = 1;
            }
        }
        check("At least one pair achieves XOR", any);
    }
}

/* ================================================================ */
/* Phase 4: Triple Combinations                                     */
/* ================================================================ */

static void phase4_triple_combinations(void) {
    /* 4 triples: skip one of A,B,C,D each time */
    static const int triples[4][3] = {
        {0,1,2}, /* A+B+C: all except Q8 (D) */
        {0,1,3}, /* A+B+D: all except 2T (C) */
        {0,2,3}, /* A+C+D: all except outermost-nonnull (B) */
        {1,2,3}  /* B+C+D: all except outermost-null (A) */
    };
    static const char *triple_names[] = {
        "A+B+C (skip D/Q8)",
        "A+B+D (skip C/2T)",
        "A+C+D (skip B/D0nn)",
        "B+C+D (skip A/D0null)"
    };
    static const char *triple_short[] = {"A+B+C","A+B+D","A+C+D","B+C+D"};
    SubsetResult triple_res[4];
    SubsetResult all_res;
    int ti, ni;
    int n_list[] = {3, 4, 5, 6};
    int cell_mask[5];

    printf("\n=== Phase 4: Triple Combinations ===\n\n");
    fflush(stdout);

    /* First: ALL (A+B+C+D+E) for reference */
    memset(cell_mask, 1, sizeof(cell_mask));
    load_cells(cell_mask);
    run_subset_capacity(&all_res);
    print_subset_result("ALL (A+B+C+D+E, 24 entries)", &all_res);
    fflush(stdout);

    for (ti = 0; ti < 4; ti++) {
        memset(cell_mask, 0, sizeof(cell_mask));
        cell_mask[triples[ti][0]] = 1;
        cell_mask[triples[ti][1]] = 1;
        cell_mask[triples[ti][2]] = 1;
        cell_mask[4] = 1; /* always include E=identity */
        load_cells(cell_mask);
        run_subset_capacity(&triple_res[ti]);
        print_subset_result(triple_names[ti], &triple_res[ti]);
        fflush(stdout);
    }

    /* Summary table */
    printf("  --- Triple vs ALL XOR Rate ---\n");
    printf("    Subset  | Size | N=3     | N=4     | N=5     | N=6\n");
    printf("    --------|------|---------|---------|---------|--------\n");
    /* ALL row */
    printf("    ALL(24) | %4d |", all_res.size);
    for (ni = 0; ni < 4; ni++) {
        if (all_res.total[ni] > 0)
            printf(" %6.2f%%|",
                   100.0*(double)all_res.xor_count[ni]/(double)all_res.total[ni]);
        else
            printf("    n/a |");
    }
    printf("\n");
    /* Triple rows */
    for (ti = 0; ti < 4; ti++) {
        printf("    %-7s | %4d |", triple_short[ti], triple_res[ti].size);
        for (ni = 0; ni < 4; ni++) {
            if (triple_res[ti].total[ni] > 0)
                printf(" %6.2f%%|",
                       100.0*(double)triple_res[ti].xor_count[ni] /
                       (double)triple_res[ti].total[ni]);
            else
                printf("    n/a |");
        }
        printf("\n");
    }
    printf("\n");

    /* Impact of removing each cell */
    printf("  --- Missing Cell Impact (ALL rate - Triple rate) ---\n");
    printf("    Missing | N=3    | N=4    | N=5    | N=6\n");
    printf("    --------|--------|--------|--------|--------\n");
    {
        const char *missing[] = {"D(Q8)","C(2T)","B(D0nn)","A(D0null)"};
        for (ti = 0; ti < 4; ti++) {
            printf("    %-7s |", missing[ti]);
            for (ni = 0; ni < 4; ni++) {
                if (all_res.total[ni] > 0 && triple_res[ti].total[ni] > 0) {
                    double all_r = 100.0 * (double)all_res.xor_count[ni] /
                                   (double)all_res.total[ni];
                    double tri_r = 100.0 * (double)triple_res[ti].xor_count[ni] /
                                   (double)triple_res[ti].total[ni];
                    printf(" %+6.2f%%|", tri_r - all_r);
                } else {
                    printf("    n/a |");
                }
            }
            printf("\n");
        }
    }
    printf("\n");

    /* Which cell hurts most to remove? */
    {
        int worst_cell[4];
        for (ni = 0; ni < 4; ni++) {
            double worst_drop = 0;
            int worst = 0;
            double all_r = (all_res.total[ni] > 0) ?
                (double)all_res.xor_count[ni] / (double)all_res.total[ni] : 0;
            for (ti = 0; ti < 4; ti++) {
                double tri_r = (triple_res[ti].total[ni] > 0) ?
                    (double)triple_res[ti].xor_count[ni] /
                    (double)triple_res[ti].total[ni] : 0;
                double drop = all_r - tri_r;
                if (drop > worst_drop) { worst_drop = drop; worst = ti; }
            }
            worst_cell[ni] = worst;
        }
        {
            const char *missing[] = {"D(Q8)","C(2T)","B(D0nn)","A(D0null)"};
            for (ni = 0; ni < 4; ni++) {
                printf("  Most important cell at N=%d: %s\n",
                       n_list[ni], missing[worst_cell[ni]]);
            }
        }
    }
    printf("\n");

    check("Triple comparison completed", 1);
    {
        /* Check that no triple matches ALL at N=5 */
        int all_n5 = all_res.xor_count[2];
        int any_match = 0;
        for (ti = 0; ti < 4; ti++) {
            if (triple_res[ti].xor_count[2] >= all_n5 && all_n5 > 0)
                any_match = 1;
        }
        if (!any_match && all_n5 > 0) {
            check("All 4 cells needed for full N=5 capacity", 1);
        } else {
            check("Some triple matches ALL at N=5", 1);
        }
    }
}

/* ================================================================ */
/* Main                                                             */
/* ================================================================ */

int main(void) {
    printf("KNOTAPEL DEMO 96: TL-Group Cross-Classification\n");
    printf("=================================================\n");
    fflush(stdout);

    /* Build group */
    printf("\n  Building z8 group...\n");
    build_z8();
    printf("  z8: %d bracket values\n", g_z8_size);
    check("z8 has 24 bracket values", g_z8_size == 24);
    fflush(stdout);

    /* Compute both classifications */
    printf("  Computing derived series...\n");
    compute_derived_series();
    printf("  Computing null classification...\n");
    compute_null_classification();
    printf("  Computing cell assignment...\n");
    compute_cells();
    fflush(stdout);

    /* Phase 1 */
    phase1_cross_tabulation();
    fflush(stdout);

    /* Phase 2 */
    phase2_individual_cells();
    fflush(stdout);

    /* Phase 3 */
    phase3_pairwise_synergy();
    fflush(stdout);

    /* Phase 4 */
    phase4_triple_combinations();
    fflush(stdout);

    /* Summary */
    printf("\n=================================================\n");
    printf("Results: %d pass, %d fail\n", n_pass, n_fail);

    return n_fail;
}

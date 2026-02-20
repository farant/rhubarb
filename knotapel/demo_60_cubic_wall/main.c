/*
 * KNOTAPEL DEMO 60: ell=7 Cubic Wall
 * ====================================
 * Tests radical anatomy predictions at the first cubic number field.
 * delta = 2cos(pi/7), root of x^3 - x^2 - 2x + 1 = 0.
 *
 * At ell=7, the Chebyshev arithmetic enters the first cubic number field.
 * The minimal polynomial is x^3-x^2-2x+1=0, and delta is a root.
 * This is the first non-quadratic case and tests whether all dimension
 * formulas from Demos 51-52 extend universally.
 *
 * Architecture:
 *   Part A: Modular arithmetic + cubic root finding via polynomial GCD
 *   Part B: Planar matching enumeration + TL algebra (from Demo 52)
 *   Part C: Gram matrix, radical dimension at general delta (mod p)
 *   Part D: Radical filtration (rad^2, rad^3, nilpotency)
 *   Part E: Cross-validation at a second prime
 *   Part H: Fibonacci rank test (through-strand sector decomposition)
 *
 * C89, zero dependencies beyond stdio/stdlib/string.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ================================================================
 * Constants
 * ================================================================ */

#define MAX_N 8
#define MAX_2N 16
#define MAX_BASIS 1430   /* C_8 = 1430 */
#define MAX_SEGS 16
#define MAX_LOOPS 20
#define MAX_RAD 200      /* max radical vectors for filtration */
#define MAX_SECTOR_DIM 200  /* max unique top/bottom patterns per sector */

/* Catalan numbers C_0 through C_8 */
static const int catalan[] = {1, 1, 2, 5, 14, 42, 132, 429, 1430};

/* ================================================================
 * Test infrastructure
 * ================================================================ */

static int n_pass = 0, n_fail = 0;

static void check(const char *tname, int ok) {
    if (ok) { printf("  PASS: %s\n", tname); n_pass++; }
    else    { printf("  FAIL: %s\n", tname); n_fail++; }
}

/* ================================================================
 * Types
 * ================================================================ */

typedef struct { int match[MAX_2N]; } PlanarMatch;
typedef struct { int points[MAX_2N]; int count; } Segment;

typedef struct {
    int n, dim, id_idx, gen_idx[MAX_N], n_gens;
    PlanarMatch basis[MAX_BASIS];
    int mt_result[MAX_BASIS][MAX_BASIS];
    int mt_loops[MAX_BASIS][MAX_BASIS];
} TLAlgebra;

typedef struct {
    int dim, gram_rank, rad_dim, rad2_dim, nilpotency;
} AnalysisResult;

/* ================================================================
 * PART A: Modular Arithmetic
 * ================================================================ */

static long g_mod_p;

static long mod_reduce(long x) {
    long r = x % g_mod_p;
    return r < 0 ? r + g_mod_p : r;
}

static long mod_inv(long a) {
    long t = 0, newt = 1, r = g_mod_p, newr = mod_reduce(a);
    while (newr != 0) {
        long q = r / newr, tmp;
        tmp = t - q * newt; t = newt; newt = tmp;
        tmp = r - q * newr; r = newr; newr = tmp;
    }
    return t < 0 ? t + g_mod_p : t;
}



static int is_prime(long n) {
    long i;
    if (n < 2) return 0;
    if (n < 4) return 1;
    if (n % 2 == 0 || n % 3 == 0) return 0;
    for (i = 5; i * i <= n; i += 6)
        if (n % i == 0 || n % (i + 2) == 0) return 0;
    return 1;
}

/* ================================================================
 * Polynomial arithmetic over F_p mod f(x) = x^3 - x^2 - 2x + 1
 *
 * Elements of F_p[x]/(f(x)) are triples (c0, c1, c2) = c0+c1*x+c2*x^2.
 * Reduction: x^3 = x^2 + 2x - 1, x^4 = 3x^2 + x - 1.
 * ================================================================ */

/* Multiply two elements of F_p[x]/(f), result in r[3] */
static void poly_mul(const long *a, const long *b, long *r) {
    long c0, c1, c2, c3, c4;
    c0 = mod_reduce(a[0] * b[0]);
    c1 = mod_reduce(mod_reduce(a[0]*b[1]) + mod_reduce(a[1]*b[0]));
    c2 = mod_reduce(mod_reduce(a[0]*b[2]) + mod_reduce(a[1]*b[1])
                     + mod_reduce(a[2]*b[0]));
    c3 = mod_reduce(mod_reduce(a[1]*b[2]) + mod_reduce(a[2]*b[1]));
    c4 = mod_reduce(a[2] * b[2]);
    /* Reduce x^4 -> 3x^2 + x - 1 */
    c0 = mod_reduce(c0 - c4);
    c1 = mod_reduce(c1 + c4);
    c2 = mod_reduce(c2 + mod_reduce(3 * c4));
    /* Reduce x^3 -> x^2 + 2x - 1 */
    c0 = mod_reduce(c0 - c3);
    c1 = mod_reduce(c1 + mod_reduce(2 * c3));
    c2 = mod_reduce(c2 + c3);
    r[0] = c0; r[1] = c1; r[2] = c2;
}

/* base^exp in F_p[x]/(f) by repeated squaring */
static void poly_pow(const long *base, long exp, long *r) {
    long acc[3], b[3], tmp[3];
    acc[0] = 1; acc[1] = 0; acc[2] = 0;
    b[0] = base[0]; b[1] = base[1]; b[2] = base[2];
    while (exp > 0) {
        if (exp & 1) {
            poly_mul(acc, b, tmp);
            acc[0]=tmp[0]; acc[1]=tmp[1]; acc[2]=tmp[2];
        }
        poly_mul(b, b, tmp);
        b[0]=tmp[0]; b[1]=tmp[1]; b[2]=tmp[2];
        exp >>= 1;
    }
    r[0] = acc[0]; r[1] = acc[1]; r[2] = acc[2];
}

/* Degree of polynomial stored in c[0..maxd] */
static int pdeg(const long *c, int maxd) {
    int i;
    for (i = maxd; i >= 0; i--) if (c[i] != 0) return i;
    return -1;
}

/* Polynomial remainder: a mod b. deg(a)=da, deg(b)=db.
 * Result in r[4]. Returns degree of result (-1 for zero). */
static int pmod(const long *a, int da, const long *b, int db, long *r) {
    long w[4];
    int i, dw;
    for (i = 0; i < 4; i++) w[i] = (i <= da) ? a[i] : 0;
    dw = da;
    while (dw >= db && dw >= 0) {
        long lc = mod_reduce(w[dw] * mod_inv(b[db]));
        for (i = 0; i <= db; i++)
            w[dw-db+i] = mod_reduce(w[dw-db+i] - mod_reduce(lc * b[i]));
        dw = pdeg(w, dw);
    }
    for (i = 0; i < 4; i++) r[i] = (dw >= 0 && i <= dw) ? w[i] : 0;
    return dw;
}

/* GCD of two polynomials. Result in g[4] (monic). Returns degree. */
static int pgcd(const long *a, int da, const long *b, int db, long *g) {
    long u[4], v[4], t[4];
    int du, dv, dt, i;
    for (i = 0; i < 4; i++) {
        u[i] = (da >= 0 && i <= da) ? a[i] : 0;
        v[i] = (db >= 0 && i <= db) ? b[i] : 0;
    }
    du = da; dv = db;
    while (dv >= 0) {
        dt = pmod(u, du, v, dv, t);
        for (i = 0; i < 4; i++) u[i] = v[i];
        du = dv;
        for (i = 0; i < 4; i++) v[i] = t[i];
        dv = dt;
    }
    if (du >= 0) {
        long inv = mod_inv(u[du]);
        for (i = 0; i <= du; i++) u[i] = mod_reduce(u[i] * inv);
    }
    for (i = 0; i < 4; i++) g[i] = u[i];
    return du;
}

/* Find a root of x^3-x^2-2x+1 mod g_mod_p. Returns root or -1. */
static long find_cubic_root(void) {
    long f[4], base[3], xp[3], diff[4], g[4];
    int dg, dd;

    f[0] = 1; f[1] = mod_reduce(-2); f[2] = mod_reduce(-1); f[3] = 1;

    /* Compute x^p mod f(x) in F_p[x]/(f) */
    base[0] = 0; base[1] = 1; base[2] = 0;
    poly_pow(base, g_mod_p, xp);

    /* diff = x^p - x (degree <= 2 since xp is reduced) */
    diff[0] = xp[0];
    diff[1] = mod_reduce(xp[1] - 1);
    diff[2] = xp[2];
    diff[3] = 0;
    dd = pdeg(diff, 3);

    if (dd < 0) {
        /* x^p = x mod f => all roots exist, gcd = f */
        dg = 3;
        g[0]=f[0]; g[1]=f[1]; g[2]=f[2]; g[3]=f[3];
    } else {
        dg = pgcd(f, 3, diff, dd, g);
    }

    if (dg <= 0) return -1;
    if (dg == 1) return mod_reduce(g_mod_p - g[0]);
    if (dg == 2) {
        /* f = (x^2+bx+c)(x-r), matching x^2 coeff: b-r=-1 => r=b+1 */
        return mod_reduce(g[1] + 1);
    }
    /* dg == 3: f splits completely, use Cantor-Zassenhaus */
    {
        int a;
        for (a = 0; a < 200; a++) {
            long shifted[3], h[3], hm1[4], g2[4];
            int dh, dg2;
            shifted[0] = mod_reduce((long)a);
            shifted[1] = 1; shifted[2] = 0;
            poly_pow(shifted, (g_mod_p - 1) / 2, h);
            hm1[0] = mod_reduce(h[0] - 1);
            hm1[1] = h[1]; hm1[2] = h[2]; hm1[3] = 0;
            dh = pdeg(hm1, 3);
            if (dh < 0) continue;
            dg2 = pgcd(f, 3, hm1, dh, g2);
            if (dg2 == 1) return mod_reduce(g_mod_p - g2[0]);
            if (dg2 == 2) return mod_reduce(g2[1] + 1);
        }
    }
    return -1;
}

/* Find a prime >= start where x^3-x^2-2x+1 splits.
 * Returns the prime; stores root in *root_out. */
static long find_good_prime(long start, long *root_out) {
    long p;
    for (p = start; ; p++) {
        long root, v;
        if (!is_prime(p)) continue;
        g_mod_p = p;
        root = find_cubic_root();
        if (root < 0) continue;
        /* Verify: delta^3 - delta^2 - 2*delta + 1 = 0 mod p */
        v = mod_reduce(
            mod_reduce(mod_reduce(root*root)*root)
            - mod_reduce(root*root)
            - mod_reduce(2*root)
            + 1);
        if (v == 0) {
            *root_out = root;
            return p;
        }
    }
}

/* ================================================================
 * PART B: Planar Matching Enumeration + TL Algebra (from Demo 52)
 * ================================================================ */

static void build_boundary_order(int n, int *bp) {
    int i;
    for (i = 0; i < n; i++) bp[i] = i;
    for (i = 0; i < n; i++) bp[n+i] = 2*n-1-i;
}

static void enum_segments(Segment *segs, int n_segs, int *mb,
                          PlanarMatch *basisarr, int *nb, int n) {
    int s, j, k, first_seg = -1;
    Segment ns[MAX_SEGS]; int nn; int *pts; int cnt;
    for (s = 0; s < n_segs; s++)
        if (segs[s].count > 0) { first_seg = s; break; }
    if (first_seg == -1) {
        if (*nb < MAX_BASIS) {
            memcpy(basisarr[*nb].match, mb,
                   (size_t)(2*n) * sizeof(int));
            (*nb)++;
        }
        return;
    }
    pts = segs[first_seg].points;
    cnt = segs[first_seg].count;
    for (j = 1; j < cnt; j += 2) {
        mb[pts[0]] = pts[j]; mb[pts[j]] = pts[0];
        nn = 0;
        for (k = 0; k < n_segs; k++) {
            if (k == first_seg) {
                if (j > 1) {
                    memcpy(ns[nn].points, &pts[1],
                           (size_t)(j-1)*sizeof(int));
                    ns[nn].count = j-1; nn++;
                }
                if (cnt-j-1 > 0) {
                    memcpy(ns[nn].points, &pts[j+1],
                           (size_t)(cnt-j-1)*sizeof(int));
                    ns[nn].count = cnt-j-1; nn++;
                }
            } else {
                ns[nn] = segs[k]; nn++;
            }
        }
        enum_segments(ns, nn, mb, basisarr, nb, n);
    }
}

static int enumerate_basis(int n, PlanarMatch *basisarr) {
    Segment segs[1]; int mb[MAX_2N]; int nb = 0;
    build_boundary_order(n, segs[0].points);
    segs[0].count = 2*n;
    memset(mb, -1, sizeof(mb));
    enum_segments(segs, 1, mb, basisarr, &nb, n);
    return nb;
}

static void compose_diagrams(int n, const PlanarMatch *d1,
                              const PlanarMatch *d2,
                              PlanarMatch *r, int *loops_out) {
    int gv[MAX_N];
    int i, loops = 0;
    memset(r->match, -1, (size_t)(2*n)*sizeof(int));
    memset(gv, 0, (size_t)n*sizeof(int));
    for (i = 0; i < 2*n; i++) {
        int in1, cur;
        if (r->match[i] >= 0) continue;
        if (i < n) { in1=1; cur=i; } else { in1=0; cur=i; }
        for (;;) {
            int p;
            if (in1) {
                p = d1->match[cur];
                if (p < n) { r->match[i]=p; r->match[p]=i; break; }
                gv[p-n]=1; in1=0; cur=p-n;
            } else {
                p = d2->match[cur];
                if (p >= n) { r->match[i]=p; r->match[p]=i; break; }
                gv[p]=1; in1=1; cur=n+p;
            }
        }
    }
    for (i = 0; i < n; i++) {
        int cur, p, q;
        if (gv[i]) continue;
        loops++; cur = i;
        do {
            gv[cur]=1; p=d2->match[cur]; gv[p]=1;
            q=d1->match[n+p]; cur=q-n;
        } while (cur != i);
    }
    *loops_out = loops;
}

static int find_basis_index(const PlanarMatch *m, const PlanarMatch *b,
                             int nb, int n) {
    int i, j;
    for (i = 0; i < nb; i++) {
        int eq = 1;
        for (j = 0; j < 2*n; j++)
            if (m->match[j] != b[i].match[j]) { eq=0; break; }
        if (eq) return i;
    }
    return -1;
}

static void init_tl(TLAlgebra *a, int n) {
    PlanarMatch d;
    int g, i, j;
    a->n = n;
    a->dim = enumerate_basis(n, a->basis);
    a->n_gens = n - 1;
    /* Identity */
    memset(&d, 0, sizeof(d));
    for (g = 0; g < n; g++) {
        d.match[g] = n + g;
        d.match[n+g] = g;
    }
    a->id_idx = find_basis_index(&d, a->basis, a->dim, n);
    /* Generators */
    for (g = 0; g < n-1; g++) {
        PlanarMatch gen; int k;
        for (k = 0; k < n; k++) {
            gen.match[k] = n+k; gen.match[n+k] = k;
        }
        gen.match[g] = g+1; gen.match[g+1] = g;
        gen.match[n+g] = n+g+1; gen.match[n+g+1] = n+g;
        a->gen_idx[g] = find_basis_index(&gen, a->basis, a->dim, n);
    }
    /* Multiplication table */
    printf("    Building mult table (%d x %d)...\n", a->dim, a->dim);
    for (i = 0; i < a->dim; i++) {
        for (j = 0; j < a->dim; j++) {
            PlanarMatch r; int loops;
            compose_diagrams(n, &a->basis[i], &a->basis[j], &r, &loops);
            a->mt_result[i][j] =
                find_basis_index(&r, a->basis, a->dim, n);
            a->mt_loops[i][j] = loops;
        }
    }
}

/* ================================================================
 * PART C: Gram Matrix, Rank, Radical Extraction
 * ================================================================ */

static long g_aug[MAX_BASIS][2 * MAX_BASIS];
static long g_rad[MAX_RAD][MAX_BASIS];

/* Echelon form for filtration */
static long g_ech[MAX_RAD][MAX_BASIS];
static int g_ech_pivots[MAX_RAD];

static void analyze(const TLAlgebra *alg, long delta_val,
                     AnalysisResult *res) {
    long dpow[MAX_LOOPS + 1];
    long fixpt_d[MAX_BASIS];
    int i, j, k, rank, n_rad, max_loops;
    int dim = alg->dim;

    res->dim = dim;
    res->rad2_dim = -1;
    res->nilpotency = -1;

    /* Precompute delta powers */
    max_loops = 0;
    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++)
            if (alg->mt_loops[i][j] > max_loops)
                max_loops = alg->mt_loops[i][j];
    dpow[0] = 1;
    for (k = 1; k <= max_loops && k <= MAX_LOOPS; k++)
        dpow[k] = mod_reduce(dpow[k-1] * delta_val);

    /* Weighted fixed points:
     * fixpt_d[p] = sum_{k: result(p,k)==k} delta^loops(p,k) */
    for (i = 0; i < dim; i++) {
        fixpt_d[i] = 0;
        for (k = 0; k < dim; k++)
            if (alg->mt_result[i][k] == k)
                fixpt_d[i] = mod_reduce(
                    fixpt_d[i] + dpow[alg->mt_loops[i][k]]);
    }

    /* Build augmented [Gram | I] matrix */
    for (i = 0; i < dim; i++) {
        for (j = 0; j < dim; j++) {
            int p = alg->mt_result[i][j];
            g_aug[i][j] = mod_reduce(
                dpow[alg->mt_loops[i][j]] * fixpt_d[p]);
        }
        for (j = 0; j < dim; j++)
            g_aug[i][dim + j] = (i == j) ? 1L : 0L;
    }

    /* Gaussian elimination on left half */
    printf("    Gaussian elimination (%d x %d)...\n", dim, dim);
    rank = 0;
    for (k = 0; k < dim; k++) {
        int pivot = -1;
        for (i = rank; i < dim; i++)
            if (g_aug[i][k] != 0) { pivot = i; break; }
        if (pivot == -1) continue;
        if (pivot != rank) {
            for (j = 0; j < 2*dim; j++) {
                long t = g_aug[rank][j];
                g_aug[rank][j] = g_aug[pivot][j];
                g_aug[pivot][j] = t;
            }
        }
        {
            long inv = mod_inv(g_aug[rank][k]);
            for (i = rank + 1; i < dim; i++) {
                if (g_aug[i][k] != 0) {
                    long fac = mod_reduce(g_aug[i][k] * inv);
                    for (j = k; j < 2*dim; j++)
                        g_aug[i][j] = mod_reduce(
                            g_aug[i][j] - mod_reduce(fac * g_aug[rank][j]));
                }
            }
        }
        rank++;
    }

    res->gram_rank = rank;
    res->rad_dim = dim - rank;

    if (res->rad_dim == 0) return;
    if (res->rad_dim > MAX_RAD) {
        printf("    Radical dim %d > MAX_RAD %d, skipping filtration\n",
               res->rad_dim, MAX_RAD);
        return;
    }

    /* Extract radical basis vectors from right half of augmented */
    n_rad = 0;
    for (i = rank; i < dim && n_rad < MAX_RAD; i++) {
        int nonzero = 0;
        for (j = 0; j < dim; j++) {
            g_rad[n_rad][j] = g_aug[i][dim + j];
            if (g_rad[n_rad][j] != 0) nonzero = 1;
        }
        if (nonzero) n_rad++;
    }
    printf("    Extracted %d radical basis vectors\n", n_rad);

    /* ============================================================
     * PART D: Radical Filtration
     * ============================================================ */
    {
        int n_ech = 0;
        long prod[MAX_BASIS];

        printf("    Computing rad^2 (%d x %d products)...\n",
               n_rad, n_rad);

        /* rad^2 = span of {r_i * r_j : r_i, r_j in rad} */
        for (i = 0; i < n_rad && n_ech < MAX_RAD; i++) {
            for (j = 0; j < n_rad && n_ech < MAX_RAD; j++) {
                int m, q;
                /* Compute product r_i * r_j with delta weighting */
                memset(prod, 0, sizeof(long) * (size_t)dim);
                for (m = 0; m < dim; m++) {
                    if (g_rad[i][m] == 0) continue;
                    for (q = 0; q < dim; q++) {
                        int kr;
                        if (g_rad[j][q] == 0) continue;
                        kr = alg->mt_result[m][q];
                        prod[kr] = mod_reduce(prod[kr] +
                            mod_reduce(
                                mod_reduce(g_rad[i][m] * g_rad[j][q])
                                * dpow[alg->mt_loops[m][q]]));
                    }
                }
                /* Try to add to echelon form */
                {
                    long tmp[MAX_BASIS];
                    int pc, ej;
                    memcpy(tmp, prod, sizeof(long) * (size_t)dim);
                    for (ej = 0; ej < n_ech; ej++) {
                        pc = g_ech_pivots[ej];
                        if (tmp[pc] != 0) {
                            long fac = mod_reduce(
                                tmp[pc] * mod_inv(g_ech[ej][pc]));
                            for (m = 0; m < dim; m++)
                                tmp[m] = mod_reduce(tmp[m]
                                    - mod_reduce(fac * g_ech[ej][m]));
                        }
                    }
                    for (pc = 0; pc < dim; pc++)
                        if (tmp[pc] != 0) break;
                    if (pc < dim) {
                        memcpy(g_ech[n_ech], tmp,
                               sizeof(long) * (size_t)dim);
                        g_ech_pivots[n_ech] = pc;
                        n_ech++;
                    }
                }
            }
        }
        res->rad2_dim = n_ech;
        printf("    rad^2 dim = %d\n", n_ech);

        /* rad^3: multiply rad^2 basis by radical basis */
        if (n_ech > 0 && n_ech < MAX_RAD) {
            /* Store rad^2 basis separately */
            static long rad2_copy[MAX_RAD][MAX_BASIS];
            static long ech3[MAX_RAD][MAX_BASIS];
            static int piv3[MAX_RAD];
            int n_rad2 = n_ech;
            int n_ech3 = 0;

            memcpy(rad2_copy, g_ech,
                   sizeof(long) * (size_t)n_rad2 * MAX_BASIS);

            printf("    Computing rad^3 (%d x %d + %d x %d products)...\n",
                   n_rad2, n_rad, n_rad, n_rad2);

            /* Left: rad^2 * rad */
            for (i = 0; i < n_rad2 && n_ech3 < MAX_RAD; i++) {
                for (j = 0; j < n_rad && n_ech3 < MAX_RAD; j++) {
                    int m, q;
                    memset(prod, 0, sizeof(long) * (size_t)dim);
                    for (m = 0; m < dim; m++) {
                        if (rad2_copy[i][m] == 0) continue;
                        for (q = 0; q < dim; q++) {
                            int kr;
                            if (g_rad[j][q] == 0) continue;
                            kr = alg->mt_result[m][q];
                            prod[kr] = mod_reduce(prod[kr] +
                                mod_reduce(
                                    mod_reduce(rad2_copy[i][m]
                                               * g_rad[j][q])
                                    * dpow[alg->mt_loops[m][q]]));
                        }
                    }
                    {
                        long tmp[MAX_BASIS];
                        int pc, ej;
                        memcpy(tmp, prod,
                               sizeof(long) * (size_t)dim);
                        for (ej = 0; ej < n_ech3; ej++) {
                            pc = piv3[ej];
                            if (tmp[pc] != 0) {
                                long fac = mod_reduce(
                                    tmp[pc] * mod_inv(ech3[ej][pc]));
                                for (m = 0; m < dim; m++)
                                    tmp[m] = mod_reduce(tmp[m]
                                        - mod_reduce(fac*ech3[ej][m]));
                            }
                        }
                        for (pc = 0; pc < dim; pc++)
                            if (tmp[pc] != 0) break;
                        if (pc < dim) {
                            memcpy(ech3[n_ech3], tmp,
                                   sizeof(long) * (size_t)dim);
                            piv3[n_ech3] = pc;
                            n_ech3++;
                        }
                    }
                }
            }
            /* Right: rad * rad^2 */
            for (i = 0; i < n_rad && n_ech3 < MAX_RAD; i++) {
                for (j = 0; j < n_rad2 && n_ech3 < MAX_RAD; j++) {
                    int m, q;
                    memset(prod, 0, sizeof(long) * (size_t)dim);
                    for (m = 0; m < dim; m++) {
                        if (g_rad[i][m] == 0) continue;
                        for (q = 0; q < dim; q++) {
                            int kr;
                            if (rad2_copy[j][q] == 0) continue;
                            kr = alg->mt_result[m][q];
                            prod[kr] = mod_reduce(prod[kr] +
                                mod_reduce(
                                    mod_reduce(g_rad[i][m]
                                               * rad2_copy[j][q])
                                    * dpow[alg->mt_loops[m][q]]));
                        }
                    }
                    {
                        long tmp[MAX_BASIS];
                        int pc, ej;
                        memcpy(tmp, prod,
                               sizeof(long) * (size_t)dim);
                        for (ej = 0; ej < n_ech3; ej++) {
                            pc = piv3[ej];
                            if (tmp[pc] != 0) {
                                long fac = mod_reduce(
                                    tmp[pc] * mod_inv(ech3[ej][pc]));
                                for (m = 0; m < dim; m++)
                                    tmp[m] = mod_reduce(tmp[m]
                                        - mod_reduce(fac*ech3[ej][m]));
                            }
                        }
                        for (pc = 0; pc < dim; pc++)
                            if (tmp[pc] != 0) break;
                        if (pc < dim) {
                            memcpy(ech3[n_ech3], tmp,
                                   sizeof(long) * (size_t)dim);
                            piv3[n_ech3] = pc;
                            n_ech3++;
                        }
                    }
                }
            }

            printf("    rad^3 dim = %d\n", n_ech3);
            if (n_ech3 == 0)
                res->nilpotency = 3;
            else
                res->nilpotency = -1; /* would need more iterations */
        } else if (n_ech == 0) {
            res->nilpotency = 2;
        }
    }
}

/* ================================================================
 * PART H: Fibonacci Rank Test (through-strand sector decomposition)
 *
 * The rad^2 generator (1-dimensional) decomposes by through-strand
 * count into sector matrices. The sum of bilinear ranks across all
 * sectors should equal F(ell-1).
 * ================================================================ */

/* Saved rad^2 generator and basis for n=ell=7 */
static long g_fib_rad2[MAX_BASIS];
static PlanarMatch g_fib_basis[MAX_BASIS];
static int g_fib_dim = 0;

static int count_through_strands(const PlanarMatch *m, int n) {
    int i, t = 0;
    for (i = 0; i < n; i++)
        if (m->match[i] >= n) t++;
    return t;
}

static int patterns_equal(const int *a, const int *b, int n) {
    int i;
    for (i = 0; i < n; i++)
        if (a[i] != b[i]) return 0;
    return 1;
}

/* Gaussian elimination on a flat rows x cols matrix mod g_mod_p.
 * Destructive. Returns rank. */
static int matrix_rank_modp(long *mat, int rows, int cols) {
    int r = 0, c, i, j;
    for (c = 0; c < cols && r < rows; c++) {
        int pivot = -1;
        for (i = r; i < rows; i++)
            if (mat[i * cols + c] != 0) { pivot = i; break; }
        if (pivot == -1) continue;
        if (pivot != r)
            for (j = 0; j < cols; j++) {
                long t = mat[r * cols + j];
                mat[r * cols + j] = mat[pivot * cols + j];
                mat[pivot * cols + j] = t;
            }
        {
            long inv = mod_inv(mat[r * cols + c]);
            for (i = r + 1; i < rows; i++) {
                if (mat[i * cols + c] != 0) {
                    long fac = mod_reduce(mat[i * cols + c] * inv);
                    for (j = c; j < cols; j++)
                        mat[i * cols + j] = mod_reduce(
                            mat[i * cols + j]
                            - mod_reduce(fac * mat[r * cols + j]));
                }
            }
        }
        r++;
    }
    return r;
}

static int fibonacci_rank_test(int n) {
    static int top_pat[MAX_BASIS][MAX_N];
    static int bot_pat[MAX_BASIS][MAX_N];
    static int n_through[MAX_BASIS];
    static long sector_mat[MAX_SECTOR_DIM * MAX_SECTOR_DIM];
    int total_rank = 0;
    int t, i, b;

    printf("\n  Fibonacci Rank Test (n=%d):\n", n);

    /* Step 1: Compute through-strand counts and half-patterns */
    for (b = 0; b < g_fib_dim; b++) {
        n_through[b] = count_through_strands(&g_fib_basis[b], n);
        for (i = 0; i < n; i++)
            top_pat[b][i] = (g_fib_basis[b].match[i] < n) ?
                             g_fib_basis[b].match[i] : -1;
        for (i = 0; i < n; i++)
            bot_pat[b][i] = (g_fib_basis[b].match[n+i] >= n) ?
                             (g_fib_basis[b].match[n+i] - n) : -1;
    }

    /* Step 2: For each through-strand sector */
    for (t = (n % 2 == 0 ? 0 : 1); t <= n; t += 2) {
        int sector_elems[MAX_BASIS];
        int s_top_idx[MAX_BASIS];
        int s_bot_idx[MAX_BASIS];
        int unique_top[MAX_SECTOR_DIM][MAX_N];
        int unique_bot[MAX_SECTOR_DIM][MAX_N];
        int n_sect = 0, n_utop = 0, n_ubot = 0;
        int rank, si;

        /* Collect elements in this sector */
        for (b = 0; b < g_fib_dim; b++) {
            if (n_through[b] != t) continue;
            sector_elems[n_sect] = b;

            /* Find or add top pattern */
            s_top_idx[n_sect] = -1;
            for (i = 0; i < n_utop; i++) {
                if (patterns_equal(top_pat[b], unique_top[i], n)) {
                    s_top_idx[n_sect] = i; break;
                }
            }
            if (s_top_idx[n_sect] == -1) {
                s_top_idx[n_sect] = n_utop;
                memcpy(unique_top[n_utop], top_pat[b],
                       sizeof(int) * (size_t)n);
                n_utop++;
            }

            /* Find or add bottom pattern */
            s_bot_idx[n_sect] = -1;
            for (i = 0; i < n_ubot; i++) {
                if (patterns_equal(bot_pat[b], unique_bot[i], n)) {
                    s_bot_idx[n_sect] = i; break;
                }
            }
            if (s_bot_idx[n_sect] == -1) {
                s_bot_idx[n_sect] = n_ubot;
                memcpy(unique_bot[n_ubot], bot_pat[b],
                       sizeof(int) * (size_t)n);
                n_ubot++;
            }

            n_sect++;
        }

        if (n_sect == 0) continue;

        /* Build sector coefficient matrix */
        memset(sector_mat, 0,
               sizeof(long) * (size_t)n_utop * (size_t)n_ubot);
        for (si = 0; si < n_sect; si++) {
            int r = s_top_idx[si], c = s_bot_idx[si];
            sector_mat[r * n_ubot + c] =
                g_fib_rad2[sector_elems[si]];
        }

        /* Compute rank */
        rank = matrix_rank_modp(sector_mat, n_utop, n_ubot);
        printf("    Sector %d: %d elems, %d x %d matrix, rank = %d\n",
               t, n_sect, n_utop, n_ubot, rank);
        total_rank += rank;
    }

    printf("    Total bilinear rank = %d (predicted F(6) = 8)\n",
           total_rank);
    return total_rank;
}

/* ================================================================
 * MAIN
 * ================================================================ */

int main(void) {
    static TLAlgebra alg;
    long p1, p2, d1, d2;
    AnalysisResult res1[9], res2[9]; /* indexed by n */
    int n;
    char msg[256];

    setbuf(stdout, NULL);

    printf("KNOTAPEL DEMO 60: ell=7 Cubic Wall\n");
    printf("====================================\n");
    printf("delta = 2cos(pi/7), root of x^3 - x^2 - 2x + 1 = 0\n\n");

    /* Find two primes where the cubic splits */
    printf("Finding primes where x^3-x^2-2x+1 splits...\n");
    p1 = find_good_prime(1000000007L, &d1);
    printf("  Prime 1: p=%ld, delta mod p = %ld\n", p1, d1);
    p2 = find_good_prime(p1 + 1, &d2);
    printf("  Prime 2: p=%ld, delta mod p = %ld\n\n", p2, d2);

    /* Verify cubic roots */
    g_mod_p = p1;
    {
        long v = mod_reduce(
            mod_reduce(mod_reduce(d1*d1)*d1)
            - mod_reduce(d1*d1) - mod_reduce(2*d1) + 1);
        sprintf(msg, "Cubic root verified at p1=%ld", p1);
        check(msg, v == 0);
    }
    g_mod_p = p2;
    {
        long v = mod_reduce(
            mod_reduce(mod_reduce(d2*d2)*d2)
            - mod_reduce(d2*d2) - mod_reduce(2*d2) + 1);
        sprintf(msg, "Cubic root verified at p2=%ld", p2);
        check(msg, v == 0);
    }

    /* Analyze for n=2..8 */
    for (n = 2; n <= 8; n++) {
        printf("\n=== n=%d (dim=C_%d=%d) ===\n", n, n, catalan[n]);
        init_tl(&alg, n);

        printf("  At p1=%ld:\n", p1);
        g_mod_p = p1;
        analyze(&alg, d1, &res1[n]);
        printf("    rank=%d, rad=%d",
               res1[n].gram_rank, res1[n].rad_dim);
        if (res1[n].rad2_dim >= 0)
            printf(", rad2=%d", res1[n].rad2_dim);
        if (res1[n].nilpotency >= 0)
            printf(", nilpotency=%d", res1[n].nilpotency);
        printf("\n");

        /* Save rad^2 generator and basis for Fibonacci rank test */
        if (n == 7 && res1[7].rad2_dim == 1) {
            memcpy(g_fib_rad2, g_ech[0],
                   sizeof(long) * (size_t)alg.dim);
            memcpy(g_fib_basis, alg.basis,
                   sizeof(PlanarMatch) * (size_t)alg.dim);
            g_fib_dim = alg.dim;
        }

        printf("  At p2=%ld:\n", p2);
        g_mod_p = p2;
        analyze(&alg, d2, &res2[n]);
        printf("    rank=%d, rad=%d",
               res2[n].gram_rank, res2[n].rad_dim);
        if (res2[n].rad2_dim >= 0)
            printf(", rad2=%d", res2[n].rad2_dim);
        if (res2[n].nilpotency >= 0)
            printf(", nilpotency=%d", res2[n].nilpotency);
        printf("\n");
    }

    /* Summary table */
    printf("\n============ Summary ============\n\n");
    printf("  n | dim  | rank | rad | rad2 | nilp | xval\n");
    printf("  --|------|------|-----|------|------|-----\n");
    for (n = 2; n <= 8; n++) {
        int xval = (res1[n].gram_rank == res2[n].gram_rank
                    && res1[n].rad_dim == res2[n].rad_dim);
        printf("  %d | %4d | %4d | %3d | %4d | %4d | %s\n",
               n, res1[n].dim, res1[n].gram_rank, res1[n].rad_dim,
               res1[n].rad2_dim, res1[n].nilpotency,
               xval ? "OK" : "FAIL");
    }

    /* Tests */
    printf("\n============ Tests ============\n\n");

    /* Semisimple for n < ell=7 */
    for (n = 2; n <= 6; n++) {
        sprintf(msg, "n=%d semisimple: rank=%d == dim=%d",
                n, res1[n].gram_rank, catalan[n]);
        check(msg, res1[n].gram_rank == catalan[n]);
    }

    /* Radical structure for n >= 7 */
    if (res1[7].rad_dim > 0) {
        sprintf(msg, "n=7: non-trivial radical (dim=%d)",
                res1[7].rad_dim);
        check(msg, 1);
    } else {
        sprintf(msg, "n=7: semisimple (unexpected if n >= ell)");
        check(msg, 0);
    }

    if (res1[8].rad_dim > 0) {
        sprintf(msg, "n=8: non-trivial radical (dim=%d)",
                res1[8].rad_dim);
        check(msg, 1);
    } else {
        sprintf(msg, "n=8: semisimple (unexpected if n >= ell)");
        check(msg, 0);
    }

    /* Nilpotency */
    if (res1[7].nilpotency > 0) {
        sprintf(msg, "n=7: nilpotency=%d (predicted 3)",
                res1[7].nilpotency);
        check(msg, res1[7].nilpotency == 3);
    }
    if (res1[8].nilpotency > 0) {
        sprintf(msg, "n=8: nilpotency=%d (predicted 3)",
                res1[8].nilpotency);
        check(msg, res1[8].nilpotency == 3);
    }

    /* Cross-validation */
    {
        int all_match = 1;
        for (n = 2; n <= 8; n++) {
            if (res1[n].gram_rank != res2[n].gram_rank
                || res1[n].rad_dim != res2[n].rad_dim) {
                all_match = 0;
                printf("    MISMATCH at n=%d: p1 rank=%d rad=%d,"
                       " p2 rank=%d rad=%d\n",
                       n, res1[n].gram_rank, res1[n].rad_dim,
                       res2[n].gram_rank, res2[n].rad_dim);
            }
        }
        check("Cross-validation: both primes agree on all ranks",
              all_match);
    }

    /* Fibonacci Rank Test (Part H) */
    if (g_fib_dim > 0) {
        int fib_rank;
        g_mod_p = p1;
        fib_rank = fibonacci_rank_test(7);
        sprintf(msg, "Fibonacci rank = %d (predicted F(6) = 8)",
                fib_rank);
        check(msg, fib_rank == 8);

        /* Dimension formula tests */
        sprintf(msg, "rad(TL_7) = %d (predicted 2*7-3 = 11)",
                res1[7].rad_dim);
        check(msg, res1[7].rad_dim == 11);
        sprintf(msg, "rad(TL_8) = %d (predicted 7^2-7-3 = 39)",
                res1[8].rad_dim);
        check(msg, res1[8].rad_dim == 39);
    }

    printf("\n====================================\n");
    printf("Final: %d passed, %d failed\n", n_pass, n_fail);
    printf("====================================\n");

    return n_fail > 0 ? 1 : 0;
}

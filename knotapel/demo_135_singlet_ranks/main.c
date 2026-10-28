/*
 * KNOTAPEL DEMO 135: exact ranks of the singlet pieces
 * ====================================================
 *
 * Plan: plan.md (v2 after a design review before any data; T0 and T1
 * amendments). Brainstorm (project-specs/crossings-contraction-counting-
 * brainstorm.md) section 11, "Singlet pieces in closed form", derives the
 * ranks of ANT's one-bit pieces. The lambda-bit crossing [x = v][y = u]
 * is the sum of pieces A_0 .. A_lambda, where A_i collects the expansion
 * terms in which i bits take the singlet term:
 *
 *   per bit  E = [x_j = u_j][y_j = v_j],
 *            S = [x_j != y_j](-1)^x_j [u_j != v_j](-1)^u_j,
 *            crossing bit = E - S (ANT Appendix H);
 *   A_i = (-1)^i sum_{|T| = i} prod_{j in T} S_j prod_{j not in T} E_j.
 *
 * Rows cut (x,u | y,v): rank F(lambda, i); columns cut (x,y | u,v): rank
 * F(lambda, lambda - i); F(l, k) = sum_{d <= k} binom(l, d) 3^d, because
 * each piece is diagonal in a per-bit rotated basis. Checked here:
 *
 *   R1   the pieces sum to the crossing (whole-word comparison)
 *   R2   each piece is diagonal with the predicted eigenvalues
 *   R2b  the rotated basis is a basis (per-bit determinants, tuples)
 *   R3   ranks by elimination: over Z (lambda <= 4), mod 2^31 - 1 above
 *   R4   ranks mod 3, 5, 7 against the registered LITERAL table
 *   plus "formula = table" (the formula code reproduces the table), the
 *   cross-check (own elimination against matrix_gradus) and descriptive
 *   rows (ranks mod 2, sign gauge, cost curve, factor matrices).
 *
 * Every piece matrix is built from the indicator definitions (word
 * operations on x, y, u, v), never from the per-bit E/S matrices. The
 * predicted eigenvalues (mu_rows, mu_cols), the rank formula
 * (formula_rank) and the literal tables are three separate pieces of
 * code. Ranks are blind to signs (plan.md, "Known blind spot"): R1 and
 * R2 are the sign-sensitive checks.
 *
 * Index layout: per bit a pair (first, second) -> 2*first + second, bit
 * pairs concatenated low bit first (bit j of the words sits in bits
 * 2j, 2j+1 of the index). Rows matrix: row (x,u), column (y,v). Columns
 * matrix: row (x,y), column (u,v). Basis tensors use the same layout.
 *
 * House libraries: matrix, anulus, magnus, congruentia (and closure),
 * hence latina.h: Roman numerals and Latin keywords are macros here
 * (never name a variable I, V, X, L, C, D or M, or per, si, dum, ...).
 * Build and run from the repo root:
 *   ./bin/aedilis knotapel/demo_135_singlet_ranks/main.c &&
 *   bash build/aedilis/main/struere.sh && ./build/aedilis/main/main
 * D135_LAMBDA=n stops after lambda = n (default 6 = the registered run;
 * anything less prints PARTIAL RUN); D135_TEMPORA=1 prints times. Run
 * lambda = 6 under tools/custos.sh.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "latina.h"
#include "piscina.h"
#include "magnus.h"
#include "anulus.h"
#include "matrix.h"
#include "congruentia.h"

#define LAMBDA_SUMMUM 6
#define PRIMUS_MAGNUS 2147483647L
#define NUM_PRIMES 5

#define CUT_ROWS 0
#define CUT_COLS 1

/* ---------- checks and verdicts ---------- */

enum {
    P_R1, P_R2, P_R2B, P_R3, P_R4, P_FORMULA, P_XCHK, P_RESID, P_GAUGE,
    P_NUM
};

static const char *pred_name[P_NUM] = {
    "R1", "R2", "R2b", "R3", "R4", "formula=table", "cross-check",
    "residues", "sign gauge (descriptive)"
};

static long pred_checks[P_NUM];
static long pred_fails[P_NUM];
static int n_pass = 0;
static int n_fail = 0;
static int show_times = 0;

static void
check (
    int         pred,
    const char *msg,
    int         ok)
{
    pred_checks[pred]++;
    if (ok) {
        n_pass++;
        printf("  PASS: %s: %s\n", pred_name[pred], msg);
    } else {
        n_fail++;
        pred_fails[pred]++;
        printf("  FAIL: %s: %s\n", pred_name[pred], msg);
    }
}

static void
fatal (
    const char *msg)
{
    printf("  FATAL: %s\n", msg);
    exit(1);
}

static double
seconds_since (
    clock_t start)
{
    return (double)(clock() - start) / (double)CLOCKS_PER_SEC;
}

/* ---------- small integer helpers ---------- */

static long
binom (
    long n,
    long k)
{
    long r = 1;
    long j;

    if (k < 0 || k > n) return 0;
    for (j = 1; j <= k; j++) r = r * (n - k + j) / j;
    return r;
}

static long
pow3 (
    long d)
{
    long r = 1;

    while (d-- > 0) r *= 3;
    return r;
}

static int
popcount (
    unsigned long w)
{
    int n = 0;

    while (w != 0) {
        n += (int)(w & 1UL);
        w >>= 1;
    }
    return n;
}

/* ---------- the registered literal tables (plan.md v2, R3 and R4) ---------- */

/* Q ranks, rows cut, F(lambda, i); columns are the rows reversed */
static const long tab_q[LAMBDA_SUMMUM + 1][LAMBDA_SUMMUM + 1] = {
    { 1 },
    { 1, 4 },
    { 1, 7, 16 },
    { 1, 10, 37, 64 },
    { 1, 13, 67, 175, 256 },
    { 1, 16, 106, 376, 781, 1024 },
    { 1, 19, 154, 694, 1909, 3367, 4096 }
};

struct drop_row {
    int  lam;
    long p;
    long rows[LAMBDA_SUMMUM + 1];
};

/* the cases with drops mod small primes, rows cut (plan.md R4 table) */
static const struct drop_row tab_drop[] = {
    { 3, 3, { 1, 9, 36, 64 } },
    { 4, 3, { 1, 13, 54, 163, 256 } },
    { 5, 3, { 1, 16, 106, 271, 691, 1024 } },
    { 5, 5, { 1, 15, 105, 375, 780, 1024 } },
    { 6, 3, { 1, 18, 153, 694, 1233, 2826, 4096 } },
    { 6, 5, { 1, 19, 135, 675, 1890, 3349, 4096 } }
};

/* registered rank: p == 0 or p > every listed prime = the Q table */
static long
table_rank (
    int  lam,
    int  i,
    long p,
    int  cut)
{
    int k = cut == CUT_ROWS ? i : lam - i;
    size_t j;

    for (j = 0; j < sizeof tab_drop / sizeof tab_drop[0]; j++)
        if (tab_drop[j].lam == lam && tab_drop[j].p == p)
            return tab_drop[j].rows[k];
    return tab_q[lam][k];
}

/* ---------- the rank formula (separate code; "formula = table") ---------- */

static long
formula_rank (
    int  lam,
    int  i,
    long p,
    int  cut)
{
    long total = 0;
    int top = cut == CUT_ROWS ? i : lam - i;
    int d;
    int cols = cut == CUT_COLS;

    for (d = 0; d <= top; d++) {
        if (p != 0 && !cols && binom(lam - d, i - d) % p == 0) continue;
        if (p != 0 && cols && binom(lam - d, i) % p == 0) continue;
        total += binom(lam, d) * pow3(d);
    }
    return total;
}

/* ---------- predicted eigenvalues (separate code; R2) ---------- */

static long
mu_rows (
    int lam,
    int i,
    int d)
{
    long v;

    if (d > i) return 0;
    v = binom(lam - d, i - d);
    v = v * (1L << (lam - i));
    return (i & 1) ? -v : v;
}

static long
mu_cols (
    int lam,
    int i,
    int d)
{
    long v;

    if (d > lam - i) return 0;
    v = binom(lam - d, i);
    v = v * (1L << i);
    return (i & 1) ? -v : v;
}

/* ---------- per-bit bases (R2, R2b) ---------- */

/* vector q in layout 2*first + second: |00>, |01>, |10>, |11> */
static const long basis_rows[4][4] = {
    { 1, 0, 0, 1 },     /* a = |00> + |11> (special) */
    { 1, 0, 0, -1 },    /* |00> - |11> */
    { 0, 1, 0, 0 },     /* |01> */
    { 0, 0, 1, 0 }      /* |10> */
};

static const long basis_cols[4][4] = {
    { 0, 1, -1, 0 },    /* sigma = |01> - |10> (special) */
    { 1, 0, 0, 0 },     /* |00> */
    { 0, 0, 0, 1 },     /* |11> */
    { 0, 1, 1, 0 }      /* |01> + |10> */
};

static long
det_small (
    const long *m,
    int         n)
{
    long sub[16];
    long total = 0;
    int col;

    if (n == 1) return m[0];
    for (col = 0; col < n; col++) {
        int r, c, k = 0;
        long minor;

        for (r = 1; r < n; r++)
            for (c = 0; c < n; c++)
                if (c != col) sub[k++] = m[r * n + c];
        minor = det_small(sub, n - 1);
        total += ((col & 1) ? -1 : 1) * m[col] * minor;
    }
    return total;
}

/* ---------- the piece, from the indicator definitions ---------- */

static unsigned long masks[1 << LAMBDA_SUMMUM];
static int mask_start[LAMBDA_SUMMUM + 2];   /* masks grouped by popcount */
static unsigned long *first_word;           /* index -> first word */
static unsigned long *second_word;          /* index -> second word */
static unsigned long nn;                    /* 4^lambda */

static void
setup_lambda (
    int lam)
{
    unsigned long idx, t;
    int k = 0, pc, j;

    nn = 1UL << (2 * lam);
    for (pc = 0; pc <= lam; pc++) {
        mask_start[pc] = k;
        for (t = 0; t < (1UL << lam); t++)
            if (popcount(t) == pc) masks[k++] = t;
    }
    mask_start[lam + 1] = k;
    free(first_word);
    free(second_word);
    first_word = malloc(nn * sizeof *first_word);
    second_word = malloc(nn * sizeof *second_word);
    if (first_word == NULL || second_word == NULL) fatal("out of memory");
    for (idx = 0; idx < nn; idx++) {
        unsigned long a = 0, b = 0;

        for (j = 0; j < lam; j++) {
            unsigned long pair = (idx >> (2 * j)) & 3UL;

            a |= (pair >> 1) << j;
            b |= (pair & 1UL) << j;
        }
        first_word[idx] = a;
        second_word[idx] = b;
    }
}

static long
sign_of_piece (
    int i)
{
    return (i & 1) ? -1 : 1;
}

static long
piece_value (
    unsigned long x,
    unsigned long y,
    unsigned long u,
    unsigned long v,
    int           lam,
    int           i)
{
    unsigned long full = (1UL << lam) - 1UL;
    long sum = 0;
    int k;

    for (k = mask_start[i]; k < mask_start[i + 1]; k++) {
        unsigned long t = masks[k];
        unsigned long rest = full & ~t;

        /* E off T: x_j = u_j and y_j = v_j */
        if ((((x ^ u) | (y ^ v)) & rest) != 0) continue;
        /* S on T: x_j != y_j and u_j != v_j */
        if (((~(x ^ y) | ~(u ^ v)) & t) != 0) continue;
        /* (-1)^x_j (-1)^u_j over T */
        sum += (popcount((x ^ u) & t) & 1) ? -1 : 1;
    }
    return sum * sign_of_piece(i);
}

static void
cut_words (
    int            cut,
    unsigned long  r,
    unsigned long  c,
    unsigned long *x,
    unsigned long *y,
    unsigned long *u,
    unsigned long *v)
{
    if (cut == CUT_ROWS) {
        *x = first_word[r]; *u = second_word[r]; *y = first_word[c]; *v = second_word[c];
    } else {
        *x = first_word[r]; *y = second_word[r]; *u = first_word[c]; *v = second_word[c];
    }
}

/* the matrix of piece i in one cut; gauge: wrong-signed nonzero entries
 * against (-1)^i (-1)^popcount(x ^ u) (descriptive) */
static void
build_matrix (
    long *mat,
    int   lam,
    int   i,
    int   cut,
    long *gauge_bad)
{
    unsigned long r, c;

    for (r = 0; r < nn; r++)
        for (c = 0; c < nn; c++) {
            unsigned long x, y, u, v;
            long val;

            cut_words(cut, r, c, &x, &y, &u, &v);
            val = piece_value(x, y, u, v, lam, i);
            mat[r * nn + c] = val;
            if (val != 0) {
                long want = ((i & 1) ? -1 : 1)
                    * ((popcount(x ^ u) & 1) ? -1 : 1);

                if ((val > 0) != (want > 0)) (*gauge_bad)++;
            }
        }
}

/* ---------- R1 ---------- */

static long
check_sum (
    int lam)
{
    unsigned long x, y, u, v, side = 1UL << lam;
    long bad = 0;
    int i;

    for (x = 0; x < side; x++)
        for (y = 0; y < side; y++)
            for (u = 0; u < side; u++)
                for (v = 0; v < side; v++) {
                    long total = 0;
                    long want = (x == v && y == u) ? 1 : 0;

                    for (i = 0; i <= lam; i++)
                        total += piece_value(x, y, u, v, lam, i);
                    if (total != want) bad++;
                }
    return bad;
}

/* ---------- R2: diagonal with the predicted eigenvalues ---------- */

/* S^(x) lambda in the rows layout, from the definition: prod_j
 * s(x_j, y_j) s(u_j, v_j), s(a, b) = [a != b](-1)^a */
static long
s_tensor (
    unsigned long r,
    unsigned long c,
    int           lam)
{
    unsigned long full = (1UL << lam) - 1UL;
    unsigned long x = first_word[r], u = second_word[r];
    unsigned long y = first_word[c], v = second_word[c];

    if ((x ^ y) != full || (u ^ v) != full) return 0;
    return ((popcount(x) + popcount(u)) & 1) ? -1 : 1;
}

/* returns the number of basis tensors that fail; *tuples counts them */
static long
check_diagonal (
    const long *mat,
    int         lam,
    int         i,
    int         cut,
    long       *tuples)
{
    const long (*basis)[4] = cut == CUT_ROWS ? basis_rows : basis_cols;
    unsigned long idx_list[1 << LAMBDA_SUMMUM], next_idx[1 << LAMBDA_SUMMUM];
    long val_list[1 << LAMBDA_SUMMUM], next_val[1 << LAMBDA_SUMMUM];
    long *dense = calloc(nn, sizeof *dense);
    unsigned long t, r;
    long bad = 0;

    if (dense == NULL) fatal("out of memory");
    for (t = 0; t < nn; t++) {
        int nnz = 1, d = 0, j, k;
        long mu;
        int this_bad = 0;

        idx_list[0] = 0;
        val_list[0] = 1;
        for (j = 0; j < lam; j++) {
            int digit = (int)((t >> (2 * j)) & 3UL);
            int m = 0, q;

            if (digit != 0) d++;
            for (k = 0; k < nnz; k++)
                for (q = 0; q < 4; q++)
                    if (basis[digit][q] != 0) {
                        next_idx[m] = idx_list[k] | ((unsigned long)q << (2 * j));
                        next_val[m] = val_list[k] * basis[digit][q];
                        m++;
                    }
            memcpy(idx_list, next_idx, (size_t)m * sizeof idx_list[0]);
            memcpy(val_list, next_val, (size_t)m * sizeof val_list[0]);
            nnz = m;
        }
        (*tuples)++;
        mu = cut == CUT_ROWS ? mu_rows(lam, i, d) : mu_cols(lam, i, d);
        for (k = 0; k < nnz; k++) dense[idx_list[k]] = val_list[k];
        for (r = 0; r < nn && !this_bad; r++) {
            long w = 0, target = 0;

            for (k = 0; k < nnz; k++)
                w += mat[r * nn + idx_list[k]] * val_list[k];
            if (cut == CUT_ROWS) {
                for (k = 0; k < nnz; k++)
                    target += s_tensor(r, idx_list[k], lam) * val_list[k];
                target *= mu;
            } else {
                target = mu * dense[r];
            }
            if (w != target) this_bad = 1;
        }
        for (k = 0; k < nnz; k++) dense[idx_list[k]] = 0;
        bad += this_bad;
    }
    free(dense);
    return bad;
}

/* ---------- ranks ---------- */

/* rank mod p by row elimination, one inverse per pivot; destroys a */
static long
rank_plain (
    i32           *a,
    unsigned long  n,
    long           p)
{
    unsigned long rank = 0, col, row, k, j;

    for (col = 0; col < n && rank < n; col++) {
        unsigned long pivot = n;
        i32 inv;

        for (row = rank; row < n; row++)
            if (a[row * n + col] != 0) {
                pivot = row;
                break;
            }
        if (pivot == n) continue;
        if (pivot != rank)
            for (j = 0; j < n; j++) {
                i32 tmp = a[pivot * n + j];

                a[pivot * n + j] = a[rank * n + j];
                a[rank * n + j] = tmp;
            }
        if (!congruentia_inversa(a[rank * n + col], (i32)p, &inv))
            fatal("pivot not invertible");
        for (k = rank + 1; k < n; k++) {
            unsigned long f = a[k * n + col];

            if (f == 0) continue;
            f = (f * (unsigned long)inv) % (unsigned long)p;
            for (j = col; j < n; j++) {
                unsigned long w = (unsigned long)a[k * n + j] + (unsigned long)p
                    - (f * (unsigned long)a[rank * n + j]) % (unsigned long)p;

                a[k * n + j] = (i32)(w % (unsigned long)p);
            }
        }
        rank++;
    }
    return (long)rank;
}

/* residues in [0, p); returns how many fall outside (must be 0) */
static long
fill_residues (
    const long *mat,
    i32        *res,
    long        p)
{
    unsigned long k, count = nn * nn;
    long bad = 0;

    for (k = 0; k < count; k++) {
        long w = mat[k] % p;

        if (w < 0) w += p;
        if (w < 0 || w >= p) bad++;
        res[k] = (i32)w;
    }
    return bad;
}

/* matrix_gradus over Z (p == 0) or Z/p; FALSE if the library refuses */
static int
rank_library (
    const long *mat,
    long        p,
    long       *rank)
{
    Piscina *pool = piscina_generare_dynamicum("d135", (memoriae_index)1 << 24);
    const Anulus *ring = p != 0 ? anulus_residuorum((i32)p, pool) : &ANULUS_INTEGRORUM;
    Matrix m;
    unsigned long r, c;
    i32 g = 0;
    int ok;

    if (!matrix_nulla(ring, (i32)nn, (i32)nn, pool, &m)) fatal("matrix_nulla");
    for (r = 0; r < nn; r++)
        for (c = 0; c < nn; c++) {
            long w = mat[r * nn + c];

            if (p != 0) {
                i32 e;

                w %= p;
                if (w < 0) w += p;
                e = (i32)w;
                matrix_pone(&m, (i32)r, (i32)c, &e);
            } else {
                Magnus e = magnus_ex_s64((s64)w);

                matrix_pone(&m, (i32)r, (i32)c, &e);
            }
        }
    ok = matrix_gradus(m, pool, &g) ? 1 : 0;
    piscina_destruere(pool);
    *rank = (long)g;
    return ok;
}

/* ---------- descriptive parts ---------- */

static double
f_double (
    int l,
    int k)
{
    double total = 0.0, b = 1.0, p3 = 1.0;
    int d;

    for (d = 0; d <= k; d++) {
        total += b * p3;
        b = b * (double)(l - d) / (double)(d + 1);
        p3 *= 3.0;
    }
    return total;
}

static void
part_cost_curve (
    int lam_done)
{
    static const int lams[] = { 1, 2, 3, 4, 5, 6, 8, 10, 12, 16, 20, 24, 30, 40, 60, 100 };
    size_t j;

    printf("\nDescriptive: cost curve from F (rank level, one gadget; plan.md, brainstorm 11 item 6)\n");
    printf("  lambda  crossing one direction  sum of pieces, each cheaper direction  saving   verified here\n");
    for (j = 0; j < sizeof lams / sizeof lams[0]; j++) {
        int l = lams[j], i;
        double total = 0.0;

        for (i = 0; i <= l; i++) {
            double fr = f_double(l, i), fc = f_double(l, l - i);

            total += fr < fc ? fr : fc;
        }
        printf("  %6d  2^%-21.2f  2^%-36.2f  %-8.3g %s\n", l, 2.0 * l,
            log(total) / log(2.0), pow(2.0, 2.0 * l) / total,
            l <= lam_done ? "yes" : "formula only");
    }
}

static void
part_factor_reference (void)
{
    int cut, a, b, q, lam, i, d;

    printf("\nDescriptive: factor matrices for hand reference (lambda <= 2)\n");
    for (cut = CUT_ROWS; cut <= CUT_COLS; cut++) {
        const long (*basis)[4] = cut == CUT_ROWS ? basis_rows : basis_cols;

        printf("  %s cut, per-bit basis (columns of B; layout |00>,|01>,|10>,|11>), digit 0 special:\n",
            cut == CUT_ROWS ? "rows" : "columns");
        for (a = 0; a < 4; a++) {
            printf("    digit %d: [", a);
            for (q = 0; q < 4; q++) printf("%3ld", basis[a][q]);
            printf(" ]\n");
        }
        printf("    Gram matrix B^T B:");
        for (a = 0; a < 4; a++) {
            printf(" [");
            for (b = 0; b < 4; b++) {
                long g = 0;

                for (q = 0; q < 4; q++) g += basis[a][q] * basis[b][q];
                printf("%2ld", g);
            }
            printf(" ]");
        }
        printf("\n    (diagonal: B^-1 = Gram^-1 B^T, so the factorization lives in Z[1/2])\n");
        for (lam = 1; lam <= 2; lam++)
            for (i = 0; i <= lam; i++) {
                printf("    lambda %d, piece %d: eigenvalue by d =", lam, i);
                for (d = 0; d <= lam; d++)
                    printf(" %ld", cut == CUT_ROWS ? mu_rows(lam, i, d) : mu_cols(lam, i, d));
                printf("%s\n", cut == CUT_ROWS ? "   (times S^(x)lambda)" : "");
            }
    }
}

static void
print_matrix_lambda1 (
    const long *mat,
    int         i,
    int         cut)
{
    unsigned long r, c;

    printf("    lambda 1, piece %d, %s matrix:", i, cut == CUT_ROWS ? "rows" : "columns");
    for (r = 0; r < nn; r++) {
        printf(" [");
        for (c = 0; c < nn; c++) printf("%3ld", mat[r * nn + c]);
        printf(" ]");
    }
    printf("\n");
}

/* ---------- one lambda ---------- */

static const long primes[NUM_PRIMES] = { 2, 3, 5, 7, PRIMUS_MAGNUS };

static void
print_list (
    const char *label,
    const long *vals,
    int         count)
{
    int k;

    printf("%s [", label);
    for (k = 0; k < count; k++) printf("%s%ld", k ? ", " : "", vals[k]);
    printf("]");
}

static void
run_lambda (
    int lam)
{
    long *mat;
    i32 *res;
    int cut, i, pi;
    clock_t start = clock();
    char msg[512];
    long bad;

    setup_lambda(lam);
    printf("\nlambda = %d (%lu x %lu matrices)\n", lam, nn, nn);

    bad = check_sum(lam);
    sprintf(msg, "lambda=%d: sum of pieces == [x = v][y = u] at all %lu (x,y,u,v) (%ld wrong)",
        lam, nn * nn, bad);
    check(P_R1, msg, bad == 0);

    mat = malloc(nn * nn * sizeof *mat);
    res = malloc(nn * nn * sizeof *res);
    if (mat == NULL || res == NULL) fatal("out of memory");

    for (cut = CUT_ROWS; cut <= CUT_COLS; cut++) {
        const char *cname = cut == CUT_ROWS ? "rows" : "cols";
        long plain[NUM_PRIMES][LAMBDA_SUMMUM + 1];
        long lib[NUM_PRIMES + 1][LAMBDA_SUMMUM + 1];   /* last row: over Z */
        int lib_ran[NUM_PRIMES + 1];
        long diag_bad = 0, tuples = 0, resid_bad = 0, gauge_bad = 0;
        long want[LAMBDA_SUMMUM + 1];

        for (pi = 0; pi <= NUM_PRIMES; pi++) lib_ran[pi] = 0;
        for (i = 0; i <= lam; i++) {
            build_matrix(mat, lam, i, cut, &gauge_bad);
            if (lam == 1) print_matrix_lambda1(mat, i, cut);
            diag_bad += check_diagonal(mat, lam, i, cut, &tuples);
            for (pi = 0; pi < NUM_PRIMES; pi++) {
                long p = primes[pi];
                int use_lib = lam <= 4 || (lam == 5 && p == PRIMUS_MAGNUS);

                resid_bad += fill_residues(mat, res, p);
                plain[pi][i] = rank_plain(res, nn, p);
                printf("  RANK lam=%d cut=%s i=%d p=%ld method=plain rank=%ld\n",
                    lam, cname, i, p, plain[pi][i]);
                if (use_lib && p != 2) {
                    if (!rank_library(mat, p, &lib[pi][i])) fatal("matrix_gradus refused");
                    lib_ran[pi] = 1;
                    printf("  RANK lam=%d cut=%s i=%d p=%ld method=matrix_gradus rank=%ld\n",
                        lam, cname, i, p, lib[pi][i]);
                }
            }
            if (lam <= 4) {
                if (!rank_library(mat, 0, &lib[NUM_PRIMES][i])) fatal("matrix_gradus over Z refused");
                lib_ran[NUM_PRIMES] = 1;
                printf("  RANK lam=%d cut=%s i=%d p=0 method=matrix_gradus rank=%ld\n",
                    lam, cname, i, lib[NUM_PRIMES][i]);
            }
        }

        sprintf(msg, "lambda=%d %s: %d pieces x %lu basis tensors diagonal with the predicted eigenvalues (%ld fail)",
            lam, cname, lam + 1, nn, diag_bad);
        check(P_R2, msg, diag_bad == 0);
        sprintf(msg, "lambda=%d %s: basis tensors enumerated over all 4^lambda digit tuples (%ld of %lu per piece)",
            lam, cname, tuples / (lam + 1), nn);
        check(P_R2B, msg, tuples == (long)nn * (lam + 1));
        sprintf(msg, "lambda=%d %s: every residue in [0, p) (%ld outside)", lam, cname, resid_bad);
        check(P_RESID, msg, resid_bad == 0);
        if (cut == CUT_ROWS) {
            sprintf(msg, "lambda=%d: every nonzero entry has sign (-1)^i (-1)^popcount(x ^ u) (%ld not)",
                lam, gauge_bad);
            check(P_GAUGE, msg, gauge_bad == 0);
        }

        /* R3 */
        for (i = 0; i <= lam; i++) want[i] = table_rank(lam, i, 0, cut);
        if (lam <= 4) {
            int ok = 1;

            for (i = 0; i <= lam; i++) ok &= lib[NUM_PRIMES][i] == want[i];
            sprintf(msg, "lambda=%d %s: rank over Z (matrix_gradus) == F", lam, cname);
            print_list("    measured", lib[NUM_PRIMES], lam + 1);
            print_list("  registered", want, lam + 1);
            printf("\n");
            check(P_R3, msg, ok);
        } else {
            int ok = 1;

            for (i = 0; i <= lam; i++) ok &= plain[NUM_PRIMES - 1][i] == want[i];
            sprintf(msg, "lambda=%d %s: rank mod 2^31-1 (plain elimination) == F (lower bound on Q rank)",
                lam, cname);
            print_list("    measured", plain[NUM_PRIMES - 1], lam + 1);
            print_list("  registered", want, lam + 1);
            printf("\n");
            check(P_R3, msg, ok);
        }

        /* R4 */
        for (pi = 1; pi <= 3; pi++) {
            long p = primes[pi];
            int ok = 1;

            for (i = 0; i <= lam; i++) {
                want[i] = table_rank(lam, i, p, cut);
                ok &= plain[pi][i] == want[i];
            }
            sprintf(msg, "lambda=%d %s: rank mod %ld == registered table", lam, cname, p);
            print_list("    measured", plain[pi], lam + 1);
            print_list("  registered", want, lam + 1);
            printf("\n");
            check(P_R4, msg, ok);
        }

        /* cross-check: own elimination against matrix_gradus */
        for (pi = 1; pi < NUM_PRIMES; pi++) {
            int ok = 1;

            if (!lib_ran[pi]) continue;
            for (i = 0; i <= lam; i++) ok &= plain[pi][i] == lib[pi][i];
            sprintf(msg, "lambda=%d %s: plain elimination == matrix_gradus mod %ld",
                lam, cname, primes[pi]);
            check(P_XCHK, msg, ok);
        }

        /* descriptive: mod 2 */
        print_list("  descriptive: rank mod 2", plain[0], lam + 1);
        printf("  (%s)\n", cname);
    }
    free(mat);
    free(res);
    if (show_times) printf("  time lambda=%d: %.1f s\n", lam, seconds_since(start));
}

/* ---------- main ---------- */

int
main (void)
{
    const char *env = getenv("D135_LAMBDA");
    int lam_max = LAMBDA_SUMMUM, lam, p_idx, cut, i, pred;
    static const long small_primes[] = { 0, 3, 5, 7 };
    long rows[4], cols[4];

    if (env != NULL) lam_max = atoi(env);
    if (lam_max < 1 || lam_max > LAMBDA_SUMMUM) fatal("D135_LAMBDA out of range 1..6");
    show_times = getenv("D135_TEMPORA") != NULL;

    printf("KNOTAPEL DEMO 135: exact ranks of the singlet pieces\n");
    printf("lambda = 1 .. %d%s\n", lam_max,
        lam_max < LAMBDA_SUMMUM ? "   PARTIAL RUN (not the registered run)" : "");

    printf("\nR2b: per-bit bases are bases (determinants from the arrays R2 uses)\n");
    {
        long m[16];
        int a, q;

        for (cut = CUT_ROWS; cut <= CUT_COLS; cut++) {
            const long (*basis)[4] = cut == CUT_ROWS ? basis_rows : basis_cols;
            long det;
            char msg[256];

            for (a = 0; a < 4; a++)
                for (q = 0; q < 4; q++) m[q * 4 + a] = basis[a][q];
            det = det_small(m, 4);
            sprintf(msg, "%s basis determinant = %ld (registered: +-2)",
                cut == CUT_ROWS ? "rows" : "columns", det);
            check(P_R2B, msg, det == 2 || det == -2);
        }
    }

    printf("\nformula = table: the formula code reproduces the registered table (lambda <= 6, p in Q,3,5,7, both cuts)\n");
    {
        long mismatches = 0;
        char msg[256];

        for (lam = 1; lam <= LAMBDA_SUMMUM; lam++)
            for (p_idx = 0; p_idx < 4; p_idx++)
                for (cut = CUT_ROWS; cut <= CUT_COLS; cut++)
                    for (i = 0; i <= lam; i++)
                        if (formula_rank(lam, i, small_primes[p_idx], cut)
                            != table_rank(lam, i, small_primes[p_idx], cut))
                            mismatches++;
        sprintf(msg, "every (lambda, p, cut, i): formula == table (%ld mismatches)", mismatches);
        check(P_FORMULA, msg, mismatches == 0);
        for (p_idx = 0; p_idx < 4; p_idx++) {
            rows[p_idx] = formula_rank(4, 2, small_primes[p_idx], CUT_ROWS);
            cols[p_idx] = formula_rank(4, 2, small_primes[p_idx], CUT_COLS);
        }
        printf("  example lambda=4, i=2: rows Q/3/5/7 = %ld/%ld/%ld/%ld, cols = %ld/%ld/%ld/%ld\n",
            rows[0], rows[1], rows[2], rows[3], cols[0], cols[1], cols[2], cols[3]);
    }

    for (lam = 1; lam <= lam_max; lam++) run_lambda(lam);

    part_factor_reference();
    part_cost_curve(lam_max);

    printf("\nVerdicts (plan.md v2, Lambda = 6):\n");
    for (pred = 0; pred < P_NUM; pred++) {
        const char *state;

        if (pred_checks[pred] == 0) state = "NOT TESTED";
        else if (pred_fails[pred] != 0) state = "FALLS";
        else if (lam_max < LAMBDA_SUMMUM && pred != P_FORMULA) state = "holds so far (PARTIAL RUN)";
        else state = "HOLDS";
        printf("  %-26s %s (%ld checks, %ld failed)\n", pred_name[pred], state,
            pred_checks[pred], pred_fails[pred]);
    }
    printf("\n%d passed, %d failed\n", n_pass, n_fail);
    free(first_word);
    free(second_word);
    return n_fail == 0 ? 0 : 1;
}

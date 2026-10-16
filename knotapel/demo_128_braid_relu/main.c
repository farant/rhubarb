/*
 * KNOTAPEL DEMO 128: Braids as Exact ReLU Networks (N1)
 * =====================================================
 *
 * Is a braid word an exact, untrained ReLU network? Each Artin generator
 * acts on Dynnikov coordinates u = (a_1 .. a_{n-2}, b_1 .. b_{n-2}) by a
 * piecewise-linear update built from f+ = max(0, f) and f- = min(0, f)
 * (Thiffeault, "Braids of entangled particle trajectories", Chaos 20
 * (2010), arXiv:0906.3647, eq. 8-15). Here each update is written as a
 * straight-line program over integer registers with two instructions,
 *   AFFINE  dst = sum coef * src
 *   RELU    dst = max(0, sum coef * src)
 * and f- as f - relu(f), so a braid word of length m on n strands is a
 * network of depth O(m) - two ReLU layers per letter - whose state is
 * 2n - 4 coordinates (hidden width at most 2n + 1: the state plus a
 * middle generator's 5 ReLUs), and every letter touches at most 4
 * coordinates. Plan: plan.md (approved 2026-10-15); plexus
 * (house library) supplies the reference action and everything else.
 *
 *   Part A  the layer programs (typed from the paper's equations, not
 *           from lib/plexus.c), an interpreter on exact integers
 *           (Magnus), the anchor: interpreter = ansa_agere on random
 *           words and vectors, n = 3..8, values up to 2^200; operation
 *           counts per generator and the shape of a word's network.
 *   Part B  (T6b) growth census of every braid up to length 8 on B_3
 *           and 6 on B_4.
 *   Part C  (T6b) audit of D106's Burau classification.
 *   Part D  (T6c) the active-ReLU pattern along each iteration.
 *
 * House libraries: plexus (and its closure: magnus, polynomium, matrix,
 * radices, ...), hence latina.h: Roman numerals and Latin keywords are
 * macros here (never name a variable I, V, X, L, C, D or M).
 * Build and run from the repo root:
 *   ./bin/aedilis knotapel/demo_128_braid_relu/main.c &&
 *   bash build/aedilis/main/struere.sh && ./build/aedilis/main/main
 */

#include "plexus.h"
#include "sors.h"
#include "chorda_aedificator.h"
#include "cyclotomia.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ================================================================
 * Test infrastructure
 * ================================================================ */

static int n_pass = 0;
static int n_fail = 0;

static void
check (
    const char *msg,
    int         ok)
{
    if (ok) {
        n_pass++;
        printf("  PASS: %s\n", msg);
    } else {
        n_fail++;
        printf("  FAIL: %s\n", msg);
    }
}

static Piscina *pool;

static void
fatal (
    const char *msg)
{
    printf("  FATAL: %s\n", msg);
    exit(1);
}

/* a Magnus as a C string (chorda is not NUL-terminated); the buffer is
 * reused, so print one at a time */
static const char *
mstr (
    Magnus x)
{
    static char buf[256];
    chorda      c = magnus_ad_chordam(x, pool);
    size_t      n = c.mensura < 255 ? c.mensura : 255;

    memcpy(buf, c.datum, n);
    buf[n] = '\0';
    return buf;
}

static Plexus
braid (
    int         n,
    const char *text)
{
    Plexus p;

    if (!plexus_ex_chorda((i32)n, chorda_ex_literis(text, pool), pool,
            &p)) {
        printf("  FATAL: bad braid %s\n", text);
        exit(1);
    }
    return p;
}

/* ================================================================
 * Part A: the layer programs
 * ================================================================
 *
 * Registers 0 .. 2m-1 hold the coordinates (a_j in register j - 1,
 * b_j in register m + j - 1, m = n - 2); temporaries follow. A letter's
 * program reads the OLD coordinates and writes NEW values into fresh
 * registers; the last instructions are the outputs, and "out" maps each
 * changed coordinate to the register that holds its new value. In a
 * network the outputs are simply the next layer's neurons, so the
 * final copy is bookkeeping, not an operation, and is not counted.
 */

#define OP_AFFINE 0
#define OP_RELU   1
#define MAX_TERMS 8
#define MAX_INSTR 16

typedef struct {
    int  op;
    int  dst;
    int  nterms;
    int  src[MAX_TERMS];
    long coef[MAX_TERMS];
} Instr;

typedef struct {
    int   ninstr;
    Instr in[MAX_INSTR];
    int   nregs;                 /* coordinate registers + temporaries */
    int   nout;
    int   out_coord[4];          /* coordinate register changed ... */
    int   out_reg[4];            /* ... takes the value of this register */
} Program;

static int
emit (
    Program *pr,
    int      op,
    int      nterms,
    const int  *src,
    const long *coef)
{
    Instr *ins = &pr->in[pr->ninstr++];
    int    k;

    if (pr->ninstr > MAX_INSTR || nterms > MAX_TERMS) {
        fatal("program too long");
    }
    ins->op = op;
    ins->dst = pr->nregs++;
    ins->nterms = nterms;
    for (k = 0; k < nterms; k++) {
        ins->src[k] = src[k];
        ins->coef[k] = coef[k];
    }
    return ins->dst;
}

/* shorthands: up to five terms, coefficient 0 ends the list */
static int
affine (
    Program *pr,
    int s0, long c0, int s1, long c1, int s2, long c2, int s3, long c3,
    int s4, long c4)
{
    int  src[5];
    long coef[5];
    int  n = 0;

    if (c0) { src[n] = s0; coef[n++] = c0; }
    if (c1) { src[n] = s1; coef[n++] = c1; }
    if (c2) { src[n] = s2; coef[n++] = c2; }
    if (c3) { src[n] = s3; coef[n++] = c3; }
    if (c4) { src[n] = s4; coef[n++] = c4; }
    return emit(pr, OP_AFFINE, n, src, coef);
}

static int
relu (
    Program *pr,
    int s0, long c0, int s1, long c1, int s2, long c2)
{
    int  src[3];
    long coef[3];
    int  n = 0;

    if (c0) { src[n] = s0; coef[n++] = c0; }
    if (c1) { src[n] = s1; coef[n++] = c1; }
    if (c2) { src[n] = s2; coef[n++] = c2; }
    return emit(pr, OP_RELU, n, src, coef);
}

static void
output (
    Program *pr,
    int      coord,
    int      reg)
{
    pr->out_coord[pr->nout] = coord;
    pr->out_reg[pr->nout] = reg;
    pr->nout++;
}

/* The program for one letter on n strands (eq. 8-15 of the paper, with
 * f- = f - relu(f)). Source indices are 1-based (a_j, b_j, j = 1 ..
 * n-2); A(j), B(j) are their registers. */
static void
build_letter (
    int      n,
    long     letter,
    Program *pr)
{
    int m = n - 2;
    int i = (int)(letter > 0 ? letter : -letter);

#define A(j) ((j) - 1)
#define B(j) (m + (j) - 1)
    memset(pr, 0, sizeof(*pr));
    pr->nregs = 2 * m;
    if (i == 1 && letter > 0) {
        /* (10)  b1' = a1 + b1+,  a1' = -b1 + (b1')+ */
        int t1 = relu(pr, B(1), 1, 0, 0, 0, 0);
        int nb = affine(pr, A(1), 1, t1, 1, 0, 0, 0, 0, 0, 0);
        int t2 = relu(pr, nb, 1, 0, 0, 0, 0);
        int na = affine(pr, B(1), -1, t2, 1, 0, 0, 0, 0, 0, 0);

        output(pr, A(1), na);
        output(pr, B(1), nb);
    } else if (i == 1) {
        /* (14)  b1' = b1+ - a1,  a1' = b1 - (b1')+ */
        int t1 = relu(pr, B(1), 1, 0, 0, 0, 0);
        int nb = affine(pr, t1, 1, A(1), -1, 0, 0, 0, 0, 0, 0);
        int t2 = relu(pr, nb, 1, 0, 0, 0, 0);
        int na = affine(pr, B(1), 1, t2, -1, 0, 0, 0, 0, 0, 0);

        output(pr, A(1), na);
        output(pr, B(1), nb);
    } else if (i == n - 1 && letter > 0) {
        /* (11)  b' = a + b-,  a' = -b + (b')-   (index m) */
        int t1 = relu(pr, B(m), 1, 0, 0, 0, 0);
        int nb = affine(pr, A(m), 1, B(m), 1, t1, -1, 0, 0, 0, 0);
        int t2 = relu(pr, nb, 1, 0, 0, 0, 0);
        int na = affine(pr, B(m), -1, nb, 1, t2, -1, 0, 0, 0, 0);

        output(pr, A(m), na);
        output(pr, B(m), nb);
    } else if (i == n - 1) {
        /* (15)  b' = b- - a,  a' = b - (b')- */
        int t1 = relu(pr, B(m), 1, 0, 0, 0, 0);
        int nb = affine(pr, B(m), 1, t1, -1, A(m), -1, 0, 0, 0, 0);
        int t2 = relu(pr, nb, 1, 0, 0, 0, 0);
        int na = affine(pr, B(m), 1, nb, -1, t2, 1, 0, 0, 0, 0);

        output(pr, A(m), na);
        output(pr, B(m), nb);
    } else if (letter > 0) {
        /* (8)-(9), p = i-1, q = i:
         *   c    = a_p - a_q - b_q+ + b_p-
         *   a_p' = a_p - b_p+ - (b_q+ + c)+
         *   b_p' = b_q + c-
         *   a_q' = a_q - b_q- - (b_p- - c)-
         *   b_q' = b_p - c- */
        int p = i - 1, q = i;
        int bq_pos = relu(pr, B(q), 1, 0, 0, 0, 0);
        int bp_pos = relu(pr, B(p), 1, 0, 0, 0, 0);
        int c = affine(pr, A(p), 1, A(q), -1, bq_pos, -1, B(p), 1,
            bp_pos, -1);
        int u = relu(pr, bq_pos, 1, c, 1, 0, 0);      /* (b_q+ + c)+ */
        int c_pos = relu(pr, c, 1, 0, 0, 0, 0);
        /* (b_p- - c)- = w - relu(w), w = b_p - b_p+ - c */
        int w_pos = relu(pr, B(p), 1, bp_pos, -1, c, -1);
        int nap = affine(pr, A(p), 1, bp_pos, -1, u, -1, 0, 0, 0, 0);
        int nbp = affine(pr, B(q), 1, c, 1, c_pos, -1, 0, 0, 0, 0);
        /* a_q' = a_q - (b_q - b_q+) - (w - w+) */
        int naq_part = affine(pr, A(q), 1, B(q), -1, bq_pos, 1, B(p), -1,
            bp_pos, 1);
        int naq = affine(pr, naq_part, 1, c, 1, w_pos, 1, 0, 0, 0, 0);
        int nbq = affine(pr, B(p), 1, c, -1, c_pos, 1, 0, 0, 0, 0);

        output(pr, A(p), nap);
        output(pr, B(p), nbp);
        output(pr, A(q), naq);
        output(pr, B(q), nbq);
    } else {
        /* (12)-(13):
         *   d    = a_p - a_q + b_q+ - b_p-
         *   a_p' = a_p + b_p+ + (b_q+ - d)+
         *   b_p' = b_q - d+
         *   a_q' = a_q + b_q- + (b_p- + d)-
         *   b_q' = b_p + d+ */
        int p = i - 1, q = i;
        int bq_pos = relu(pr, B(q), 1, 0, 0, 0, 0);
        int bp_pos = relu(pr, B(p), 1, 0, 0, 0, 0);
        int d = affine(pr, A(p), 1, A(q), -1, bq_pos, 1, B(p), -1,
            bp_pos, 1);
        int u = relu(pr, bq_pos, 1, d, -1, 0, 0);     /* (b_q+ - d)+ */
        int d_pos = relu(pr, d, 1, 0, 0, 0, 0);
        /* (b_p- + d)- = w - relu(w), w = b_p - b_p+ + d */
        int w_pos = relu(pr, B(p), 1, bp_pos, -1, d, 1);
        int nap = affine(pr, A(p), 1, bp_pos, 1, u, 1, 0, 0, 0, 0);
        int nbp = affine(pr, B(q), 1, d_pos, -1, 0, 0, 0, 0, 0, 0);
        /* a_q' = a_q + (b_q - b_q+) + (w - w+) */
        int naq_part = affine(pr, A(q), 1, B(q), 1, bq_pos, -1, B(p), 1,
            bp_pos, -1);
        int naq = affine(pr, naq_part, 1, d, 1, w_pos, -1, 0, 0, 0, 0);
        int nbq = affine(pr, B(p), 1, d_pos, 1, 0, 0, 0, 0, 0, 0);

        output(pr, A(p), nap);
        output(pr, B(p), nbp);
        output(pr, A(q), naq);
        output(pr, B(q), nbq);
    }
#undef A
#undef B
}

/* run one letter's program on the coordinate registers (exact) */
static void
run_letter (
    const Program *pr,
    Magnus        *coord,       /* 2m registers, updated in place */
    Magnus        *scratch)     /* pr->nregs registers */
{
    int k;
    int t;
    int m2 = pr->nregs - pr->ninstr;   /* the coordinate registers */

    for (k = 0; k < m2; k++) {
        scratch[k] = coord[k];
    }
    for (k = 0; k < pr->ninstr; k++) {
        const Instr *ins = &pr->in[k];
        Magnus       sum = magnus_ex_s64(0);

        for (t = 0; t < ins->nterms; t++) {
            sum = magnus_adde(sum, magnus_multiplica(scratch[ins->src[t]],
                magnus_ex_s64((s64)ins->coef[t]), pool), pool);
        }
        if (ins->op == OP_RELU && magnus_signum(sum) < 0) {
            sum = magnus_ex_s64(0);
        }
        scratch[ins->dst] = sum;
    }
    for (k = 0; k < pr->nout; k++) {
        coord[pr->out_coord[k]] = scratch[pr->out_reg[k]];
    }
}

/* depth: longest chain of instructions (inputs at depth 0); relu_depth:
 * the most ReLUs on any chain */
static void
program_depth (
    const Program *pr,
    int           *depth,
    int           *relu_depth)
{
    int lvl[64];
    int rl[64];
    int k;
    int t;
    int m2 = pr->nregs - pr->ninstr;

    for (k = 0; k < pr->nregs; k++) {
        lvl[k] = 0;
        rl[k] = 0;
    }
    *depth = 0;
    *relu_depth = 0;
    for (k = 0; k < pr->ninstr; k++) {
        const Instr *ins = &pr->in[k];
        int          l = 0;
        int          r = 0;

        for (t = 0; t < ins->nterms; t++) {
            if (lvl[ins->src[t]] > l) l = lvl[ins->src[t]];
            if (rl[ins->src[t]] > r) r = rl[ins->src[t]];
        }
        lvl[m2 + k] = l + 1;
        rl[m2 + k] = r + (ins->op == OP_RELU ? 1 : 0);
        if (lvl[m2 + k] > *depth) *depth = lvl[m2 + k];
        if (rl[m2 + k] > *relu_depth) *relu_depth = rl[m2 + k];
    }
}

static void
print_program (
    const Program *pr,
    int            n)
{
    int m = n - 2;
    int m2 = pr->nregs - pr->ninstr;
    int k;
    int t;

    for (k = 0; k < pr->ninstr; k++) {
        const Instr *ins = &pr->in[k];

        printf("      t%-2d = %s", ins->dst - m2,
            ins->op == OP_RELU ? "relu(" : "");
        for (t = 0; t < ins->nterms; t++) {
            int  s = ins->src[t];
            long c = ins->coef[t];

            printf("%s", t == 0 ? (c < 0 ? "-" : "") : (c < 0 ? " - " :
                " + "));
            if (c != 1 && c != -1) printf("%ld ", c < 0 ? -c : c);
            if (s < m) printf("a%d", s + 1);
            else if (s < 2 * m) printf("b%d", s - m + 1);
            else printf("t%d", s - m2);
        }
        printf("%s\n", ins->op == OP_RELU ? ")" : "");
    }
    for (k = 0; k < pr->nout; k++) {
        int c = pr->out_coord[k];

        printf("      %s%d' = t%d\n", c < m ? "a" : "b",
            c < m ? c + 1 : c - m + 1, pr->out_reg[k] - m2);
    }
}

/* run a whole word through the programs */
static void
run_word (
    int     n,
    Plexus  p,
    Magnus *coord)
{
    Program pr;
    Magnus  scratch[64];
    i32     k;

    for (k = 0; k < p.longitudo; k++) {
        build_letter(n, (long)p.litterae[k], &pr);
        run_letter(&pr, coord, scratch);
    }
}

static Plexus
random_word (
    Sors *s,
    int   n,
    int   len)
{
    s32    lit[64];
    Plexus p;
    int    k;

    for (k = 0; k < len; k++) {
        s32 i = (s32)sors_intra(s, (i32)(n - 1)) + 1;

        lit[k] = sors_intra(s, 2) == 0 ? i : -i;
    }
    if (!plexus_creare((i32)n, lit, (i32)len, pool, &p)) {
        fatal("random word refused");
    }
    return p;
}

static void
part_a (void)
{
    Sors   s;
    int    trial;
    int    compared = 0;
    int    disagree = 0;
    int    big_compared = 0;
    int    big_disagree = 0;
    Magnus scale;
    int    n;

    printf("\n=== Part A: the layer programs ===\n\n");

    /* the program of a middle generator, printed */
    {
        Program pr;
        int     depth;
        int     rdepth;

        build_letter(4, 2, &pr);
        printf("  sigma_2 on B_4 (registers a1 a2 b1 b2):\n");
        print_program(&pr, 4);
        program_depth(&pr, &depth, &rdepth);
        printf("    %d instructions, depth %d, ReLU depth %d\n\n",
            pr.ninstr, depth, rdepth);
    }

    /* operation counts per generator type */
    {
        const char *names[6] = { "sigma_1", "sigma_1^-1", "sigma_i (mid)",
            "sigma_i^-1 (mid)", "sigma_{n-1}", "sigma_{n-1}^-1" };
        long        letters[6] = { 1, -1, 2, -2, 4, -4 };
        int         k;

        printf("  per generator (n = 5; middle = sigma_2):\n");
        printf("    %-18s %5s %7s %6s %10s %8s\n", "generator", "ReLU",
            "affine", "depth", "ReLU depth", "touches");
        for (k = 0; k < 6; k++) {
            Program pr;
            int     relus = 0;
            int     affs = 0;
            int     depth;
            int     rdepth;
            int     t;

            build_letter(5, letters[k], &pr);
            for (t = 0; t < pr.ninstr; t++) {
                if (pr.in[t].op == OP_RELU) relus++;
                else affs++;
            }
            program_depth(&pr, &depth, &rdepth);
            printf("    %-18s %5d %7d %6d %10d %8d\n", names[k], relus,
                affs, depth, rdepth, pr.nout);
        }
        printf("\n");
    }

    /* structure: every program for n = 3..8 touches <= 4 coordinates
     * with ReLU depth 2; sigma then sigma^-1 through the networks is the
     * identity on random vectors (a group relation, independent of
     * plexus) */
    {
        int shape_ok = 1;
        int inverse_ok = 1;
        int nn;
        int letter;

        sors_seminare(&s, 1280, 1);
        for (nn = 3; nn <= 8; nn++) {
            for (letter = -(nn - 1); letter <= nn - 1; letter++) {
                Program pr;
                Program inv;
                int     depth;
                int     rdepth;
                int     t;

                if (letter == 0) continue;
                build_letter(nn, (long)letter, &pr);
                program_depth(&pr, &depth, &rdepth);
                if (pr.nout > 4 || rdepth != 2) shape_ok = 0;
                build_letter(nn, (long)-letter, &inv);
                for (t = 0; t < 50; t++) {
                    Magnus coord[16];
                    Magnus orig[16];
                    Magnus scratch[64];
                    int    k;

                    for (k = 0; k < 2 * (nn - 2); k++) {
                        orig[k] = coord[k] = magnus_ex_s64(
                            (s64)sors_inter(&s, -50, 50));
                    }
                    run_letter(&pr, coord, scratch);
                    run_letter(&inv, coord, scratch);
                    for (k = 0; k < 2 * (nn - 2); k++) {
                        if (!magnus_aequalis(coord[k], orig[k])) {
                            inverse_ok = 0;
                        }
                    }
                }
            }
        }
        check("every generator program (n = 3..8) touches <= 4 "
            "coordinates, ReLU depth 2", shape_ok);
        check("sigma then sigma^-1 through the networks is the identity "
            "(50 vectors per generator, n = 3..8)", inverse_ok);
    }

    /* the anchor: interpreter = ansa_agere, n = 3..8, random words */
    sors_seminare(&s, 128, 1);
    (void)magnus_ex_chorda(chorda_ex_literis(
        "1606938044258990275541962092341162602522202993782792835301376",
        pool), pool, &scale);                              /* 2^200 */
    for (trial = 0; trial < 4000; trial++) {
        PiscinaNotatio mark = piscina_notare(pool);
        int            nn = 3 + (int)sors_intra(&s, 6);
        int            m = nn - 2;
        Plexus         p = random_word(&s, nn, (int)sors_intra(&s, 20));
        Magnus         a[8];
        Magnus         b[8];
        Magnus         coord[16];
        Ansa           u;
        Ansa           v;
        int            big = trial % 4 == 3;
        int            same = 1;
        int            k;

        for (k = 0; k < m; k++) {
            a[k] = magnus_ex_s64((s64)sors_inter(&s, -1000, 1000));
            b[k] = magnus_ex_s64((s64)sors_inter(&s, -1000, 1000));
            if (big) {
                a[k] = magnus_multiplica(a[k], scale, pool);
                b[k] = magnus_multiplica(b[k], scale, pool);
            }
            coord[k] = a[k];
            coord[m + k] = b[k];
        }
        if (!ansa_ex_coordinatis((i32)nn, a, b, pool, &u)
                || !ansa_agere(u, p, pool, &v)) {
            fatal("plexus refused");
        }
        run_word(nn, p, coord);
        for (k = 0; k < m; k++) {
            if (!magnus_aequalis(coord[k], v.a[k])
                    || !magnus_aequalis(coord[m + k], v.b[k])) {
                same = 0;
            }
        }
        if (big) {
            big_compared++;
            if (!same) big_disagree++;
        } else {
            compared++;
            if (!same) disagree++;
        }
        piscina_reficere(pool, mark);
    }
    printf("  anchor: %d random (word, vector) pairs, n = 3..8, |coord| "
        "< 10^3: %d disagree\n", compared, disagree);
    printf("          %d with |coord| up to 1000 * 2^200: %d disagree\n",
        big_compared, big_disagree);
    check("layer programs = ansa_agere on every random pair (small and "
        "2^200-scaled)", disagree == 0 && big_disagree == 0
        && compared + big_compared == 4000);

    /* the paper's worked example and growth through the network */
    {
        Magnus coord[16];
        Ansa   e1;
        Ansa   v;
        Plexus w = braid(3, "1 -2");
        int    k;
        int    ok = 1;

        (void)ansa_circa(3, 1, pool, &e1);
        coord[0] = e1.a[0];
        coord[1] = e1.b[0];
        for (k = 0; k < 20; k++) {
            run_word(3, w, coord);
        }
        (void)ansa_circa(3, 1, pool, &v);
        for (k = 0; k < 20; k++) {
            (void)ansa_agere(v, w, pool, &v);
        }
        ok = magnus_aequalis(coord[0], v.a[0])
            && magnus_aequalis(coord[1], v.b[0]);
        printf("  (s1 s2^-1)^20 on E_1 through the network: (%s; ",
            mstr(coord[0]));
        printf("%s), L = %s\n", mstr(coord[1]),
            mstr(ansa_sectiones(v, pool)));
        check("(s1 s2^-1)^20 through the network = plexus iteration", ok);
    }

    /* shape of a word's network */
    printf("\n  a word as a network (state 2n - 4 coordinates, hidden "
        "width <= 2n + 1, 2 ReLU layers\n  per letter, each letter "
        "touches <= 4 coordinates):\n");
    printf("    %-34s %3s %7s %7s %9s %11s\n", "word", "n", "letters",
        "ReLUs", "depth", "ReLU depth");
    for (n = 0; n < 3; n++) {
        const char *words[3] = { "1 -2", "-3 -2 -3 2 1",
            "1 1 2 -1 -3 2 -3" };
        int         ns[3] = { 3, 4, 4 };
        Plexus      p = braid(ns[n], words[n]);
        int         relus = 0;
        int         depth = 0;
        int         rdepth = 0;
        i32         k;

        for (k = 0; k < p.longitudo; k++) {
            Program pr;
            int     d;
            int     r;
            int     t;

            build_letter(ns[n], (long)p.litterae[k], &pr);
            for (t = 0; t < pr.ninstr; t++) {
                if (pr.in[t].op == OP_RELU) relus++;
            }
            program_depth(&pr, &d, &r);
            depth += d;
            rdepth += r;
        }
        printf("    %-34s %3d %7u %7d %9d %11d\n", words[n], ns[n],
            p.longitudo, relus, depth, rdepth);
    }
    printf("    (depth adds along the word: letters on far-apart strands "
        "commute and could share a layer; not merged here)\n");
}

/* ================================================================
 * Part B: growth census; Part C: audit of D106's classification
 * ================================================================
 *
 * Distinct braids are enumerated breadth-first: level k holds the
 * braids whose shortest word has k letters, found by appending each
 * generator to every braid of level k-1 and keeping the new equality
 * classes. Equality key: the images of E_1 .. E_{n+1} on D_{n+2}
 * (plexus_aequales_ansis's family, exhaustively congruent with Artin in
 * plexus T2 and in the T5 review). Classes of each braid:
 *   P  periodic, EXACT: beta^n or beta^(n-1) equals a power of Delta^2
 *      (every periodic braid is conjugate to a power of delta = s1..s_{n-1}
 *      or epsilon = s1 delta: Brouwer, Kerekjarto, Eilenberg; cited);
 *   E  positive entropy: CERTIFIED if Burau at t = -1 or at a primitive
 *      cube root of unity has spectral radius > 1 (plexus: Kolev's lower
 *      bound, cited, decided exactly with Kronecker), otherwise
 *      MEASURED: max_k L^(k)_60 / L^(k)_40 > 2 over the loops E_k on
 *      D_n (and then, checked, not quasi-affine and > 10^6);
 *   Z  zero entropy, not periodic: not certified, growth quasi-affine.
 * (periodic braids: Gonzalez-Meneses and Wiest 2004; minimal
 * dilatations: phi^2 on B_3, Ko, Los and Song 2002 on B_4, the largest
 * root of x^4 - 2x^3 - 2x + 1 = 2.29663...)
 * The gap is wide: a pseudo-Anosov piece on <= 4 strands has dilatation
 * >= 2.29 (minimal dilatations of B_3, B_4; cited), so its ratio over
 * 20 periods is > 10^7, while a zero-entropy braid grows at most
 * linearly (ratio about 1.5). */

typedef struct {
    Plexus word;
    int    len;
    chorda key;
    int    cls;          /* CLS_P, CLS_E, CLS_Z */
    int    certified;    /* Burau(-1) radix > 1 */
    int    cert_omega;   /* Burau at a cube root of unity: radius > 1 */
    int    period;       /* P: k with beta^k a power of Delta^2 */
    double rate;         /* measured (L60/L40)^(1/20), display only */
    int    d106_pa;      /* D106's rule says pseudo-Anosov */
    long   tr;           /* trace, c1 of Burau(-1) (B_4 only) */
    long   c1;
    chorda tlkey;        /* D106's TL matrix (Part C) */
} Census;

#define CLS_P 0
#define CLS_E 1
#define CLS_Z 2

static int celer = 0;     /* D128_CELER: B_3 <= 5, B_4 <= 4 */

static chorda
class_key (
    Plexus p)
{
    int                n = (int)p.fila + 2;
    Plexus             big;
    ChordaAedificator *ae = chorda_aedificator_creare(pool, 64);
    int                k;

    if (!plexus_in_maiore(p, (i32)n, 1, pool, &big)) {
        fatal("embedding refused");
    }
    for (k = 1; k < n; k++) {
        Ansa e;
        Ansa img;

        memset(&img, 0, sizeof(img));
        if (!ansa_circa((i32)n, (i32)k, pool, &e)
                || !ansa_agere(e, big, pool, &img)) {
            fatal("loop action refused");
        }
        (void)chorda_aedificator_appendere_chorda(ae,
            ansa_ad_chordam(img, pool));
    }
    return chorda_aedificator_finire(ae);
}

static int
key_cmp (
    const void *x,
    const void *y)
{
    const chorda *a = (const chorda *)x;
    const chorda *b = (const chorda *)y;

    return (int)chorda_comparare(*a, *b);
}

static int
census_cmp (
    const void *x,
    const void *y)
{
    const Census *a = (const Census *)x;
    const Census *b = (const Census *)y;

    return (int)chorda_comparare(a->key, b->key);
}

/* breadth-first enumeration of distinct braids up to length maxlen */
static Census *
enumerate (
    int  n,
    int  maxlen,
    int *count)
{
    Census *all = NULL;
    int     nall = 0;
    int     cap = 0;
    chorda *seen = NULL;     /* sorted keys of every class found */
    int     nseen = 0;
    int     level_start = 0;
    int     level;

    cap = 1024;
    all = (Census *)malloc((size_t)cap * sizeof(Census));
    seen = (chorda *)malloc((size_t)cap * sizeof(chorda));
    memset(&all[0], 0, sizeof(Census));
    all[0].word = braid(n, "");
    all[0].len = 0;
    all[0].key = class_key(all[0].word);
    nall = 1;
    seen[0] = all[0].key;
    nseen = 1;
    for (level = 1; level <= maxlen; level++) {
        int     level_end = nall;
        int     first_new = nall;
        int     i;
        int     letter;

        for (i = level_start; i < level_end; i++) {
            for (letter = -(n - 1); letter <= n - 1; letter++) {
                s32    lit;
                Plexus g;
                Plexus w;
                chorda key;

                if (letter == 0) continue;
                lit = (s32)letter;
                (void)plexus_creare((i32)n, &lit, 1, pool, &g);
                (void)plexus_iungere(all[i].word, g, pool, &w);
                key = class_key(w);
                if (bsearch(&key, seen, (size_t)nseen, sizeof(chorda),
                        key_cmp) != NULL) {
                    continue;
                }
                /* new at this level (may repeat within the level) */
                if (nall == cap) {
                    cap *= 2;
                    all = (Census *)realloc(all, (size_t)cap
                        * sizeof(Census));
                    seen = (chorda *)realloc(seen, (size_t)cap
                        * sizeof(chorda));
                }
                memset(&all[nall], 0, sizeof(Census));
                all[nall].word = plexus_reducere(w, pool);
                all[nall].len = level;
                all[nall].key = key;
                nall++;
            }
        }
        /* dedupe this level's new classes by key */
        qsort(all + first_new, (size_t)(nall - first_new), sizeof(Census),
            census_cmp);
        {
            int w = first_new;
            int r;

            for (r = first_new; r < nall; r++) {
                if (w > first_new && chorda_aequalis(all[r].key,
                        all[w - 1].key)) {
                    continue;
                }
                all[w++] = all[r];
            }
            nall = w;
        }
        for (i = first_new; i < nall; i++) {
            seen[nseen++] = all[i].key;
        }
        qsort(seen, (size_t)nseen, sizeof(chorda), key_cmp);
        level_start = first_new;
    }
    *count = nall;
    return all;
}

/* beta^k as a word */
static Plexus
power (
    Plexus p,
    int    k)
{
    Plexus r = braid((int)p.fila, "");
    int    t;

    for (t = 0; t < k; t++) {
        (void)plexus_iungere(r, p, pool, &r);
    }
    return r;
}

/* periodic, exactly: beta^k = Delta^(2j) for k = n or n-1; returns
 * that k, or 0 */
static int
is_periodic (
    Plexus p)
{
    int    n = (int)p.fila;
    long   e = (long)plexus_summa_exponentium(p);
    int    ks[2];
    int    t;
    Plexus delta;
    Plexus twist;

    ks[0] = n;
    ks[1] = n - 1;
    (void)plexus_dimidia_torsio((i32)n, pool, &delta);
    (void)plexus_iungere(delta, delta, pool, &twist);
    for (t = 0; t < 2; t++) {
        long   num = (long)ks[t] * e;
        long   den = (long)n * (long)(n - 1);
        long   j;
        Plexus lhs;
        Plexus rhs;
        b32    same = FALSUM;

        if (num % den != 0) continue;
        j = num / den;
        lhs = power(p, ks[t]);
        rhs = power(j >= 0 ? twist : plexus_inversum(twist, pool),
            (int)(j >= 0 ? j : -j));
        if (!plexus_aequales_ansis(lhs, rhs, pool, &same)) {
            fatal("equality refused");
        }
        if (same) return ks[t];
    }
    return 0;
}

/* x^(1/20) by bisection (display only, no libm) */
static double
root20 (
    double x)
{
    double lo = 1.0;
    double hi = x > 1.0 ? x : 1.0;
    int    it;

    for (it = 0; it < 200; it++) {
        double mid = 0.5 * (lo + hi);
        double p = 1.0;
        int    k;

        for (k = 0; k < 20; k++) p *= mid;
        if (p > x) hi = mid; else lo = mid;
    }
    return lo;
}

/* L60 / L40 as a double from the leading digits (display only) */
static double
ratio_display (
    Magnus num,
    Magnus den)
{
    char   a[256];
    char   b[256];
    size_t la;
    size_t lb;
    double x;
    double y;
    long   shift;

    strcpy(a, mstr(num));
    strcpy(b, mstr(den));
    la = strlen(a);
    lb = strlen(b);
    /* compare the first 15 digits, then scale by 10^(la - lb) */
    a[la > 15 ? 15 : la] = '\0';
    b[lb > 15 ? 15 : lb] = '\0';
    x = atof(a);
    y = atof(b);
    shift = (long)(la > 15 ? la - 15 : 0) - (long)(lb > 15 ? lb - 15 : 0);
    while (shift > 0) { x *= 10.0; shift--; }
    while (shift < 0) { y *= 10.0; shift++; }
    return y > 0.0 ? x / y : 0.0;
}

/* growth over 60 periods on every E_k of D_n. Returns 1 if some
 * L^(k)_60 > 2 L^(k)_40 (exact comparison); *rate = the largest
 * (L60/L40)^(1/20) (display). Exact characterizations, for the
 * consistency checks:
 *   *repeats = 1 if L^(k)_{m+period} = L^(k)_m for all m (period > 0);
 *   *affine  = 1 if for every loop some p <= 24 makes L^(k)_{m+p} -
 *              L^(k)_m constant for m = 30 .. 60 - p (quasi-affine) */
static int
grows_exponentially (
    Plexus  p,
    int     period,
    double *rate,
    int    *repeats,
    int    *affine,
    int    *wide)
{
    int    n = (int)p.fila;
    int    k;
    int    expo = 0;
    double best = 1.0;

    *repeats = period > 0;
    *affine = 1;
    *wide = 0;
    for (k = 1; k < n; k++) {
        Ansa    e;
        Magnus *lengths = NULL;
        double  r;
        int     m;

        if (!ansa_circa((i32)n, (i32)k, pool, &e)
                || !plexus_incrementum(p, e, 60, pool, &lengths)) {
            fatal("growth refused");
        }
        if (magnus_compara(lengths[60], magnus_multiplica(lengths[40],
                magnus_ex_s64(2), pool)) > 0) {
            expo = 1;
        }
        /* wide margin: lambda >= 2.29 gives lambda^20 > 1.6 * 10^7 */
        if (magnus_compara(lengths[60], magnus_multiplica(lengths[40],
                magnus_ex_s64(1000000), pool)) > 0) {
            *wide = 1;
        }
        r = root20(ratio_display(lengths[60], lengths[40]));
        if (r > best) best = r;
        if (period > 0) {
            for (m = 0; m + period <= 60; m++) {
                if (!magnus_aequalis(lengths[m + period], lengths[m])) {
                    *repeats = 0;
                }
            }
        }
        {
            int found = 0;
            int q;

            for (q = 1; q <= 24 && !found; q++) {
                Magnus step = magnus_subtrahe(lengths[30 + q], lengths[30],
                    pool);
                int    same = 1;

                for (m = 30; m + q <= 60; m++) {
                    if (!magnus_aequalis(magnus_subtrahe(lengths[m + q],
                            lengths[m], pool), step)) {
                        same = 0;
                    }
                }
                if (same) found = 1;
            }
            if (!found) *affine = 0;
        }
    }
    *rate = best;
    return expo;
}

/* D106's rule on B_4: p(x) = det(xI - M) = x^3 - tr x^2 + c1 x - det,
 * det = 1 (checked); "pA" iff p(1) = c1 - tr != 0, else iff
 * |1 - tr| > 2 */
static void
d106_rule (
    Census *c)
{
    Matrix m;
    long   a[3][3];
    int    i;
    int    j;
    long   det;

    if (!plexus_burau_minus_uno(c->word, pool, &m)) {
        fatal("Burau(-1) refused");
    }
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            s64 v = 0;

            if (!magnus_ad_s64(*(const Magnus *)matrix_elementum(m,
                    (i32)i, (i32)j), &v)) {
                fatal("Burau(-1) entry beyond s64");
            }
            a[i][j] = (long)v;
        }
    }
    c->tr = a[0][0] + a[1][1] + a[2][2];
    c->c1 = a[0][0] * a[1][1] - a[0][1] * a[1][0]
        + a[0][0] * a[2][2] - a[0][2] * a[2][0]
        + a[1][1] * a[2][2] - a[1][2] * a[2][1];
    det = a[0][0] * (a[1][1] * a[2][2] - a[1][2] * a[2][1])
        - a[0][1] * (a[1][0] * a[2][2] - a[1][2] * a[2][0])
        + a[0][2] * (a[1][0] * a[2][1] - a[1][1] * a[2][0]);
    if (det != 1) {
        fatal("Burau(-1) det != 1 on B_4");
    }
    if (c->c1 - c->tr != 0) {
        c->d106_pa = 1;
    } else {
        long s = 1 - c->tr;

        c->d106_pa = (s > 2 || s < -2) ? 1 : 0;
    }
}

static const char *
word_text (
    Plexus p)
{
    static char buf[256];
    chorda      c = plexus_ad_chordam(p, pool);
    size_t      n = c.mensura < 255 ? c.mensura : 255;

    if (n == 0) {
        return "(empty)";
    }
    memcpy(buf, c.datum, n);
    buf[n] = '\0';
    return buf;
}

static RadixRealis
radix_one (void)
{
    return radix_ex_fractione(fractio_ex_s64(1), pool);
}

/* classify every census entry */
static void
classify (
    Census *cs,
    int     count,
    int    *consistent)
{
    int i;

    *consistent = 1;
    for (i = 0; i < count; i++) {
        RadixRealis rho;
        s32         sg = 0;
        int         periodic = is_periodic(cs[i].word);
        int         repeats;
        int         affine;
        int         wide;
        int         expo = grows_exponentially(cs[i].word, periodic,
            &cs[i].rate, &repeats, &affine, &wide);

        if (!plexus_burau_radix_maxima(cs[i].word, pool, &rho)
                || !radix_compara(rho, radix_one(), pool, &sg)) {
            fatal("radix refused");
        }
        cs[i].certified = sg > 0;
        cs[i].period = periodic;
        {
            b32 maior = FALSUM;

            if (!plexus_burau_radius_maior(cs[i].word, 3, &maior)) {
                fatal("certificate refused");
            }
            cs[i].cert_omega = maior ? 1 : 0;
        }
        if (periodic) {
            cs[i].cls = CLS_P;
            /* periodic: the lengths repeat with the period; no
             * certificate (Kolev) */
            if (expo || !repeats || cs[i].certified || cs[i].cert_omega) {
                *consistent = 0;
            }
        } else if (cs[i].certified || cs[i].cert_omega || expo) {
            cs[i].cls = CLS_E;
            /* a certificate must show in the growth; measured growth
             * must not be quasi-affine and must clear a wide margin */
            if (!expo || affine || !wide) {
                *consistent = 0;
            }
        } else {
            cs[i].cls = CLS_Z;
            /* zero entropy: eventually quasi-affine growth */
            if (!affine) {
                *consistent = 0;
            }
        }
    }
}

static void
census_table (
    Census *cs,
    int     count,
    int     maxlen)
{
    int len;

    printf("    %4s %8s %6s %11s %11s %10s %6s %9s\n", "len", "braids",
        "P", "E cert t=-1", "E cert t=w", "E measured", "Z", "E share");
    for (len = 0; len <= maxlen; len++) {
        int tot = 0, p = 0, ec = 0, ew = 0, em = 0, z = 0;
        int i;

        for (i = 0; i < count; i++) {
            if (cs[i].len != len) continue;
            tot++;
            if (cs[i].cls == CLS_P) p++;
            else if (cs[i].cls == CLS_Z) z++;
            else if (cs[i].certified) ec++;
            else if (cs[i].cert_omega) ew++;
            else em++;
        }
        printf("    %4d %8d %6d %11d %11d %10d %6d %8.1f%%\n", len, tot, p,
            ec, ew, em, z, tot ? 100.0 * (ec + ew + em) / tot : 0.0);
    }
}

static void
part_b (
    int      n,
    int      maxlen,
    int      expected,
    Census **out,
    int     *out_count)
{
    Census *cs;
    int     count;
    int     consistent;
    int     i;
    double  min_rate = 1e300;
    int     min_i = -1;
    char    msg[256];

    printf("\n=== Part B: growth census, B_%d up to length %d ===\n\n", n,
        maxlen);
    cs = enumerate(n, maxlen, &count);
    printf("  %d distinct braids (breadth-first, exact classes)\n", count);
    if (expected > 0) {
        sprintf(msg, "B_%d up to length %d: %d distinct braids (the T5 "
            "review's independent Artin count, or its prefix)", n, maxlen,
            expected);
        check(msg, count == expected);
    }
    classify(cs, count, &consistent);
    census_table(cs, count, maxlen);
    if (n == 4) {
        /* delta, epsilon periodic; Thiffeault's braid positive entropy */
        const char *periodic_words[2] = { "1 2 3", "1 1 2 3" };
        int         k;
        int         ok = 1;
        int         found = 0;

        for (k = 0; k < 2; k++) {
            if (!is_periodic(braid(4, periodic_words[k]))) ok = 0;
        }
        for (i = 0; i < count; i++) {
            if (chorda_aequalis(cs[i].key, class_key(braid(4,
                    "-3 -2 -3 2 1")))) {
                found = 1;
                if (cs[i].cls != CLS_E || cs[i].rate < 2.2965
                        || cs[i].rate > 2.2968) ok = 0;
            }
        }
        if (maxlen < 5) found = 1;   /* length 5: full mode only */
        check("B_4: delta = s1 s2 s3 and epsilon = s1 delta periodic "
            "(exact); s3^-1 s2^-1 s3^-1 s2 s1 positive entropy, rate "
            "2.2966", ok && found);
    }
    for (i = 0; i < count; i++) {
        if (cs[i].cls == CLS_E && cs[i].rate < min_rate) {
            min_rate = cs[i].rate;
            min_i = i;
        }
    }
    if (min_i >= 0) {
        printf("  smallest measured rate among E: %.5f (%s)\n", min_rate,
            word_text(cs[min_i].word));
    }
    sprintf(msg, "B_%d: periodic braids' loop lengths repeat with their "
        "period, uncertified; certified and measured E grow beyond "
        "10^6 and not quasi-affinely; Z grows quasi-affinely", n);
    check(msg, consistent);
    /* pinned per-level class counts: the D128 review's independent
     * oracle (free-group growth, periodicity for every k <= 12) */
    {
        int p3[9] = { 1, 0, 4, 10, 20, 16, 34, 32, 92 };
        int e3[9] = { 0, 0, 4, 12, 32, 92, 248, 544, 1176 };
        int z3[9] = { 0, 4, 4, 8, 16, 40, 32, 80, 88 };
        int p4[7] = { 1, 0, 0, 8, 24, 36, 102 };
        int e4[7] = { 0, 0, 8, 48, 198, 802, 2958 };
        int z4[7] = { 0, 6, 18, 42, 116, 272, 482 };
        int same = 1;
        int measured_only = 0;
        int len;

        for (len = 0; len <= maxlen; len++) {
            int pp = 0, ee = 0, zz = 0;

            for (i = 0; i < count; i++) {
                if (cs[i].len != len) continue;
                if (cs[i].cls == CLS_P) pp++;
                else if (cs[i].cls == CLS_E) ee++;
                else zz++;
            }
            if (n == 3 && (pp != p3[len] || ee != e3[len]
                    || zz != z3[len])) {
                same = 0;
            }
            if (n == 4 && (pp != p4[len] || ee != e4[len]
                    || zz != z4[len])) {
                same = 0;
            }
        }
        for (i = 0; i < count; i++) {
            if (cs[i].cls == CLS_E && !cs[i].certified
                    && !cs[i].cert_omega) {
                measured_only++;
            }
        }
        sprintf(msg, "B_%d: per-level P / E / Z counts = the review's "
            "independent counts", n);
        check(msg, same);
        sprintf(msg, "B_%d: every positive-entropy braid is CERTIFIED "
            "(Burau at t = -1 or at a cube root of unity): %d measured "
            "only", n, measured_only);
        check(msg, measured_only == 0);
        check(n == 3 ? "B_3: the identity is periodic"
            : "B_4: the identity is periodic", cs[0].len == 0
            && cs[0].cls == CLS_P);
    }
    /* distinct classes are unequal under Artin (sample) */
    {
        Sors s;
        int  t;
        int  wrong = 0;

        sors_seminare(&s, (i64)(1000 + n), 1);
        for (t = 0; t < 300 && count > 1; t++) {
            int a = (int)sors_intra(&s, (i32)count);
            int b = (int)sors_intra(&s, (i32)count);
            b32 same = FALSUM;

            if (a == b) continue;
            if (!plexus_aequales(cs[a].word, cs[b].word, 100000000UL,
                    pool, &same) || same) {
                wrong++;
            }
        }
        sprintf(msg, "B_%d: 300 random pairs of distinct classes are "
            "unequal under Artin (plexus_aequales)", n);
        check(msg, wrong == 0);
    }
    *out = cs;
    *out_count = count;
}

/* D106's TL representation of B_4 on W_{4,2} (D106 main.c, from
 * D100): 3x3 over Z[zeta_8], A = -zeta_8, A^-1 = conj(A);
 * sigma_i = A I + A^-1 e_i, sigma_i^-1 = A^-1 I + A e_i with
 * e_1 = E(0,1), e_2 = E(1,0) + E(1,2), e_3 = E(2,1). Here only to
 * replicate D106's catalog, which deduplicated by this matrix ALONE
 * (D106 main.c:409, "dedup by TL matrix"). */
typedef struct {
    Cyclotomicus m[3][3];
} TL3;

static Cyclotomia *z8;

static TL3
tl_mul (
    const TL3 *a,
    const TL3 *b)
{
    TL3 r;
    int i;
    int j;
    int k;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            Cyclotomicus sum = cyclotomicus_nullum(z8);

            for (k = 0; k < 3; k++) {
                sum = cyclotomicus_adde(sum, cyclotomicus_multiplica(
                    a->m[i][k], b->m[k][j], pool), pool);
            }
            r.m[i][j] = sum;
        }
    }
    return r;
}

static TL3
tl_generator (
    int letter)        /* +-1, +-2, +-3 */
{
    TL3          g;
    Cyclotomicus a = cyclotomicus_nega(cyclotomicus_radix(z8, 1, pool),
        pool);                                         /* -zeta */
    Cyclotomicus ai = cyclotomicus_conjugatum(a, pool);
    Cyclotomicus diag = letter > 0 ? a : ai;
    Cyclotomicus off = letter > 0 ? ai : a;
    int          i = letter > 0 ? letter : -letter;
    int          r;
    int          c;

    for (r = 0; r < 3; r++) {
        for (c = 0; c < 3; c++) {
            g.m[r][c] = r == c ? diag : cyclotomicus_nullum(z8);
        }
    }
    if (i == 1) {
        g.m[0][1] = cyclotomicus_adde(g.m[0][1], off, pool);
    } else if (i == 2) {
        g.m[1][0] = cyclotomicus_adde(g.m[1][0], off, pool);
        g.m[1][2] = cyclotomicus_adde(g.m[1][2], off, pool);
    } else {
        g.m[2][1] = cyclotomicus_adde(g.m[2][1], off, pool);
    }
    return g;
}

static chorda
tl_key (
    const TL3 *t)
{
    ChordaAedificator *ae = chorda_aedificator_creare(pool, 64);
    int                r;
    int                c;

    for (r = 0; r < 3; r++) {
        for (c = 0; c < 3; c++) {
            (void)chorda_aedificator_appendere_chorda(ae,
                cyclotomicus_ad_chordam(t->m[r][c], pool));
            (void)chorda_aedificator_appendere_character(ae, '|');
        }
    }
    return chorda_aedificator_finire(ae);
}

static TL3
tl_of_word (
    Plexus p)
{
    TL3 t = tl_generator(1);
    i32 k;
    int r;
    int c;

    for (r = 0; r < 3; r++) {
        for (c = 0; c < 3; c++) {
            t.m[r][c] = r == c ? cyclotomicus_integer(z8,
                magnus_ex_s64(1), pool) : cyclotomicus_nullum(z8);
        }
    }
    for (k = 0; k < p.longitudo; k++) {
        TL3 g = tl_generator((int)p.litterae[k]);

        t = tl_mul(&t, &g);
    }
    return t;
}

/* D106's BFS replicated: level by level, entries of the previous level
 * in catalog order, generators in D106's order (s1, s1^-1, s2, s2^-1,
 * s3, s3^-1), right multiplication, kept iff the TL matrix is new */
static void
d106_bfs (
    int     maxdepth,
    int    *level_count,
    int    *level_pa,
    Plexus *first,        /* out: the catalog's first nfirst words */
    int     nfirst)
{
    TL3    *tls = NULL;
    Plexus *words = NULL;
    chorda *keys = NULL;    /* unsorted, parallel to tls */
    chorda *seen = NULL;    /* sorted */
    int     n = 0;
    int     cap = 4096;
    int     nseen = 0;
    int     start = 0;
    int     depth;
    int     gens[6] = { 1, -1, 2, -2, 3, -3 };

    tls = (TL3 *)malloc((size_t)cap * sizeof(TL3));
    words = (Plexus *)malloc((size_t)cap * sizeof(Plexus));
    keys = (chorda *)malloc((size_t)cap * sizeof(chorda));
    seen = (chorda *)malloc((size_t)cap * sizeof(chorda));
    words[0] = braid(4, "");
    tls[0] = tl_of_word(words[0]);
    keys[0] = tl_key(&tls[0]);
    seen[0] = keys[0];
    nseen = 1;
    n = 1;
    level_count[0] = 1;
    level_pa[0] = 0;
    for (depth = 1; depth <= maxdepth; depth++) {
        int end = n;
        int i;
        int g;

        for (i = start; i < end; i++) {
            for (g = 0; g < 6; g++) {
                TL3    gen = tl_generator(gens[g]);
                TL3    prod = tl_mul(&tls[i], &gen);
                chorda key = tl_key(&prod);
                s32    lit = (s32)gens[g];
                Plexus one;
                int    lo = 0;
                int    hi = nseen;
                int    found = 0;
                int    k;

                /* seen is sorted: binary search, then linear insert */
                while (lo < hi) {
                    int mid = (lo + hi) / 2;
                    int cmp = (int)chorda_comparare(seen[mid], key);

                    if (cmp == 0) { found = 1; break; }
                    if (cmp < 0) lo = mid + 1; else hi = mid;
                }
                if (found) continue;
                if (n == cap) {
                    cap *= 2;
                    tls = (TL3 *)realloc(tls, (size_t)cap * sizeof(TL3));
                    words = (Plexus *)realloc(words, (size_t)cap
                        * sizeof(Plexus));
                    keys = (chorda *)realloc(keys, (size_t)cap
                        * sizeof(chorda));
                    seen = (chorda *)realloc(seen, (size_t)cap
                        * sizeof(chorda));
                }
                (void)plexus_creare(4, &lit, 1, pool, &one);
                (void)plexus_iungere(words[i], one, pool, &words[n]);
                tls[n] = prod;
                keys[n] = key;
                for (k = nseen; k > lo; k--) {
                    seen[k] = seen[k - 1];
                }
                seen[lo] = key;
                nseen++;
                n++;
            }
        }
        level_count[depth] = n - end;
        level_pa[depth] = 0;
        for (i = end; i < n; i++) {
            Census c;

            memset(&c, 0, sizeof(c));
            c.word = words[i];
            d106_rule(&c);
            if (c.d106_pa) level_pa[depth]++;
        }
        start = end;
    }
    free(tls);
    for (depth = 0; depth < nfirst && depth < n; depth++) {
        first[depth] = words[depth];
    }
    free(words);
    free(keys);
    free(seen);
}

static int
tlkey_cmp (
    const void *x,
    const void *y)
{
    const Census *a = (const Census *)x;
    const Census *b = (const Census *)y;

    return (int)chorda_comparare(a->tlkey, b->tlkey);
}

static void
part_c (
    Census *cs,
    int     count,
    int     maxlen)
{
    int  cross[2][3];
    int  i;
    int  r;
    int  c;
    int  first[2][3];
    char msg[256];
    int  p1_zero = 1;

    printf("\n=== Part C: D106's rule against the exact classes "
        "(B_4 up to length %d) ===\n\n", maxlen);
    memset(cross, 0, sizeof(cross));
    for (r = 0; r < 2; r++) for (c = 0; c < 3; c++) first[r][c] = -1;
    for (i = 0; i < count; i++) {
        d106_rule(&cs[i]);
        if (cs[i].c1 - cs[i].tr != 0) p1_zero = 0;
        r = cs[i].d106_pa ? 0 : 1;
        cross[r][cs[i].cls]++;
        if (first[r][cs[i].cls] < 0) first[r][cs[i].cls] = i;
    }
    /* lemma (D128 review, L1): on B_4 at t = -1 every reduced Burau
     * matrix has eigenvalue 1 - unreduced Burau fixes (1,1,1,1), which
     * lies in ker (1, t, t^2, t^3) when t^4 = 1, t != 1. So p(1) = 0,
     * D106's first branch never fires, and the other two eigenvalues
     * are real or on the unit circle */
    check("B_4: p(1) = 0 for the Burau(-1) of every braid (D106's first "
        "branch never fires)", p1_zero);
    printf("    %-24s %8s %8s %8s\n", "D106's rule says", "P", "Z", "E");
    printf("    %-24s %8d %8d %8d\n", "pseudo-Anosov", cross[0][CLS_P],
        cross[0][CLS_Z], cross[0][CLS_E]);
    printf("    %-24s %8d %8d %8d\n", "periodic", cross[1][CLS_P],
        cross[1][CLS_Z], cross[1][CLS_E]);
    printf("  (P periodic, exact; Z zero entropy, not periodic; E positive "
        "entropy, certified)\n");
    if (maxlen >= 6) {
        check("cross table = the review's independent table: 'pA' row "
            "0 / 0 / 3006, 'periodic' row 171 / 936 / 1008",
            cross[0][CLS_P] == 0 && cross[0][CLS_Z] == 0
            && cross[0][CLS_E] == 3006 && cross[1][CLS_P] == 171
            && cross[1][CLS_Z] == 936 && cross[1][CLS_E] == 1008);
    }
    printf("  shortest disagreement (breadth-first order):\n");
    if (first[1][CLS_E] >= 0) {
        Census *d = &cs[first[1][CLS_E]];

        printf("    D106 'periodic', positive entropy: %s (rate %.5f; "
            "Burau(-1) char. poly (x-1)^3: tr %ld, c1 %ld; certified by "
            "Burau at a cube root of unity: %s)\n", word_text(d->word),
            d->rate, d->tr, d->c1, d->cert_omega ? "yes" : "no");
    }
    /* what the rule decides: p(1) = 0 always, so 'pA' iff |1 - tr| > 2
     * iff an eigenvalue is real with modulus > 1 iff Burau(-1) radius
     * > 1 - exactly the plexus certificate at t = -1 */
    {
        int same = 1;

        for (i = 0; i < count; i++) {
            if (cs[i].d106_pa != cs[i].certified) same = 0;
        }
        check("D106's 'pA' = Burau(-1) spectral radius > 1 on every "
            "braid", same);
    }
    /* by length: D106's rule on the exact set against certified E */
    {
        int len;

        printf("\n    %4s %8s %16s %16s\n", "len", "braids",
            "D106 rule 'pA'", "E (certified)");
        for (len = 0; len <= maxlen; len++) {
            int tot = 0, pa = 0, e = 0;

            for (i = 0; i < count; i++) {
                if (cs[i].len != len) continue;
                tot++;
                if (cs[i].d106_pa) pa++;
                if (cs[i].cls == CLS_E) e++;
            }
            printf("    %4d %8d %15.1f%% %15.1f%%\n", len, tot,
                tot ? 100.0 * pa / tot : 0.0, tot ? 100.0 * e / tot : 0.0);
        }
        printf("    (the 'pA' column is D106's RULE applied to the exact "
            "braids; D106's published\n     share differs only at depth "
            "6, where its catalog is smaller: see below)\n");
    }
    /* D106's catalog: replicate its TL-only BFS */
    if (maxlen >= 6) {
        int d106_total[7] = { 1, 6, 26, 98, 338, 1110, 3444 };
        int d106_pa[7]    = { 0, 0, 8, 32, 146, 562, 2234 };
        int lc[7];
        int lp[7];
        int same = 1;
        int len;
        int tot6 = 0;
        int pa6 = 0;

        Plexus head[128];

        d106_bfs(6, lc, lp, head, 128);
        for (len = 0; len <= 6; len++) {
            if (lc[len] != d106_total[len] || lp[len] != d106_pa[len]) {
                same = 0;
            }
        }
        for (i = 0; i < count; i++) {
            if (cs[i].len == 6) {
                tot6++;
                if (cs[i].d106_pa) pa6++;
            }
        }
        printf("\n  D106's catalog, replicated (TL-only dedup): %d %d %d "
            "%d %d %d %d braids, %d %d %d %d %d %d %d 'pA'\n", lc[0],
            lc[1], lc[2], lc[3], lc[4], lc[5], lc[6], lp[0], lp[1], lp[2],
            lp[3], lp[4], lp[5], lp[6]);
        check("D106's table, depths 0-6, is reproduced EXACTLY by its "
            "TL-only breadth-first search", same);
        check("exact census at length 6: 3542 braids, 2258 'pA' under "
            "D106's rule", tot6 == 3542 && pa6 == 2258);
        /* the merges: exact classes sharing a TL matrix */
        {
            int pairs = 0;
            int all_len6 = 1;
            int same_burau = 1;
            int bigger = 0;

            Census *cs_sorted = (Census *)malloc((size_t)count
                * sizeof(Census));

            for (i = 0; i < count; i++) {
                TL3 t = tl_of_word(cs[i].word);

                cs[i].tlkey = tl_key(&t);
            }
            /* sort a copy: the census order stays breadth-first */
            memcpy(cs_sorted, cs, (size_t)count * sizeof(Census));
            qsort(cs_sorted, (size_t)count, sizeof(Census), tlkey_cmp);
            for (i = 0; i < count; ) {
                int j = i + 1;

                while (j < count && chorda_aequalis(cs_sorted[j].tlkey,
                        cs_sorted[i].tlkey)) {
                    j++;
                }
                if (j - i == 2) {
                    pairs++;
                    if (cs_sorted[i].len != 6 || cs_sorted[i + 1].len != 6) {
                        all_len6 = 0;
                    }
                    if (cs_sorted[i].tr != cs_sorted[i + 1].tr
                            || cs_sorted[i].c1 != cs_sorted[i + 1].c1) {
                        same_burau = 0;
                    }
                    if (pairs == 1) {
                        printf("  first merged pair: %s", word_text(
                            cs_sorted[i].word));
                        printf("  and  %s\n", word_text(cs_sorted[i + 1].word));
                    }
                } else if (j - i > 2) {
                    bigger++;
                }
                i = j;
            }
            free(cs_sorted);
            printf("  exact classes sharing a TL matrix: %d pairs, %d "
                "larger groups\n", pairs, bigger);
            check("exactly 98 pairs of distinct braids share a TL matrix, "
                "all at length 6, with equal Burau(-1) char. poly: "
                "3542 - 98 = 3444", pairs == 98 && bigger == 0 && all_len6
                && same_burau);
        }
    }
    /* D106's contingency table used its catalog's first 128 entries:
     * depths 0-2 and all but the last 3 of depth 3 (1 + 6 + 26 + 98 =
     * 131). Classify exactly those. The split does not depend on the
     * generator order (plants: three other orders give the same) */
    if (maxlen >= 6) {
        Plexus  head[128];
        int     lc[7];
        int     lp[7];
        Census *by_key = (Census *)malloc((size_t)count * sizeof(Census));
        int     rule[2] = { 0, 0 };    /* 'pA', 'periodic' (non-identity) */
        int     exact[2][3];
        int     h;

        memset(exact, 0, sizeof(exact));
        memcpy(by_key, cs, (size_t)count * sizeof(Census));
        qsort(by_key, (size_t)count, sizeof(Census), census_cmp);
        d106_bfs(4, lc, lp, head, 128);
        for (h = 1; h < 128; h++) {
            Census  probe;
            Census *hit;

            memset(&probe, 0, sizeof(probe));
            probe.key = class_key(head[h]);
            hit = (Census *)bsearch(&probe, by_key, (size_t)count,
                sizeof(Census), census_cmp);
            if (hit == NULL) fatal("catalog entry not in the census");
            r = hit->d106_pa ? 0 : 1;
            rule[r]++;
            exact[r][hit->cls]++;
        }
        free(by_key);
        printf("\n  D106's first 128 catalog entries (its contingency "
            "table): identity + %d 'periodic' + %d 'pA'\n", rule[1],
            rule[0]);
        printf("    exact classes of its 'periodic' entries: P %d, Z %d, "
            "E %d; of its 'pA' entries: E %d\n", exact[1][CLS_P],
            exact[1][CLS_Z], exact[1][CLS_E], exact[0][CLS_E]);
        check("D106's first 128 entries split as its table: identity, 88 "
            "'periodic', 39 'pA'",
            rule[1] == 88 && rule[0] == 39 && exact[0][CLS_E] == 39);
        /* regression pin (D128's own exact classes, not D106's) */
        check("its 88 'periodic' entries are exactly P 8, Z 64, E 16",
            exact[1][CLS_P] == 8 && exact[1][CLS_Z] == 64
            && exact[1][CLS_E] == 16);
    }
    /* P2, registered: both directions */
    printf("\n  P2 (registered): zero-entropy braids called 'pA': %d (this "
        "direction was impossible:\n      'pA' means Burau(-1) radius "
        "> 1, which forces positive entropy by Kolev's bound);\n"
        "      positive-entropy braids called 'periodic': %d\n",
        cross[0][CLS_P] + cross[0][CLS_Z], cross[1][CLS_E]);
    /* D106's depth-2 claim */
    {
        int  len2_pa = 0;
        int  len2_ok = 1;
        int  len2_reducible = 1;
        RadixRealis phi2;
        Polynomium  f;
        RadixRealis *roots;
        i32          nroots;

        (void)polynomium_ex_chorda(chorda_ex_literis("t^2 - 3t + 1", pool),
            't', pool, &f);
        (void)radices_reales(f, pool, &roots, &nroots);
        phi2 = roots[nroots - 1];
        for (i = 0; i < count; i++) {
            if (cs[i].len == 2 && cs[i].d106_pa) {
                RadixRealis rho;
                s32         sg = 1;
                s32         a = cs[i].word.litterae[0];
                s32         b = cs[i].word.litterae[1];
                s32         ia = a > 0 ? a : -a;
                s32         ib = b > 0 ? b : -b;

                len2_pa++;
                (void)plexus_burau_radix_maxima(cs[i].word, pool, &rho);
                (void)radix_compara(rho, phi2, pool, &sg);
                if (cs[i].tr != 4 || cs[i].c1 != 4 || sg != 0
                        || cs[i].cls != CLS_E) {
                    len2_ok = 0;
                }
                /* two adjacent generators: a braid on 3 consecutive
                 * strands inside B_4, hence reducible */
                if (!(ia - ib == 1 || ib - ia == 1)) len2_reducible = 0;
            }
        }
        printf("  D106's depth-2 'pA' braids: %d; each is a word in two "
            "adjacent generators (a 3-strand braid inside B_4: REDUCIBLE, "
            "not pseudo-Anosov); Burau bound = phi^2, exact on the "
            "3-strand piece\n", len2_pa);
        check("D106's depth-2 'pA' braids: char. poly x^3 - 4x^2 + 4x - 1, "
            "Burau radix (3 + sqrt 5)/2 exactly, positive entropy, and "
            "reducible (two adjacent generators)", len2_pa == 8 && len2_ok
            && len2_reducible);
        sprintf(msg, "D106's rule disagrees with the exact classes on B_4 "
            "up to length %d in %d braids", maxlen,
            cross[0][CLS_P] + cross[0][CLS_Z] + cross[1][CLS_E]);
        printf("  %s\n", msg);
    }
}

/* ================================================================
 * Part D: the active-ReLU pattern along each braid's iteration
 * ================================================================
 *
 * Run beta on a loop again and again and record, in each pass through
 * the word, which ReLUs fire (argument > 0; at 0 both pieces give 0).
 * A pass's pattern names the linear region the network is in.
 * Descriptive (plan.md, Part D): does the pattern become periodic,
 * from which pass s, with what period q? Two exact checks: periodic
 * braids repeat their pattern from pass 0; and on every stabilized
 * tail the network IS one integer linear map M per q passes
 * (v_{s+jq} = M^j v_s), since the programs have no biases. M is the
 * bridge to the exact dilatation (Hall and Yurttas), which Part D does
 * not compute.
 */

#define D_PASSES 60
#define D_BITS   64
#define D_QMAX   12
#define D_TAIL   20

/* one letter, recording ReLU activity; with force != NULL each ReLU
 * follows force (1: identity, 0: zero) instead of its sign, which
 * makes the letter linear. Returns the number of ReLUs. */
static int
run_letter_pattern (
    const Program       *pr,
    Magnus              *coord,
    Magnus              *scratch,
    unsigned char       *bits,
    const unsigned char *force)
{
    int k;
    int t;
    int r = 0;
    int m2 = pr->nregs - pr->ninstr;

    for (k = 0; k < m2; k++) {
        scratch[k] = coord[k];
    }
    for (k = 0; k < pr->ninstr; k++) {
        const Instr *ins = &pr->in[k];
        Magnus       sum = magnus_ex_s64(0);

        for (t = 0; t < ins->nterms; t++) {
            sum = magnus_adde(sum, magnus_multiplica(scratch[ins->src[t]],
                magnus_ex_s64((s64)ins->coef[t]), pool), pool);
        }
        if (ins->op == OP_RELU) {
            int on = force != NULL ? (int)force[r]
                : magnus_signum(sum) > 0;

            if (bits != NULL) bits[r] = (unsigned char)on;
            if (!on) sum = magnus_ex_s64(0);
            r++;
        }
        scratch[ins->dst] = sum;
    }
    for (k = 0; k < pr->nout; k++) {
        coord[pr->out_coord[k]] = scratch[pr->out_reg[k]];
    }
    return r;
}

/* one pass through the word; returns the number of ReLUs */
static int
run_pass (
    const Program       *prs,
    int                  len,
    Magnus              *coord,
    unsigned char       *bits,
    const unsigned char *force)
{
    Magnus scratch[64];
    int    k;
    int    nb = 0;

    for (k = 0; k < len; k++) {
        nb += run_letter_pattern(&prs[k], coord, scratch,
            bits != NULL ? bits + nb : NULL,
            force != NULL ? force + nb : NULL);
    }
    return nb;
}

typedef struct {
    int stable;      /* the last >= D_TAIL passes repeat with q */
    int start;       /* first pass of the periodic tail */
    int period;      /* q */
    int regions;     /* distinct patterns over all passes */
    int linear_ok;   /* v_{s+jq} = M^j v_s on the whole tail */
} Orbita;

static void
pattern_orbit (
    Plexus  p,
    int     k,          /* the loop E_k */
    Orbita *o)
{
    int           n = (int)p.fila;
    int           d = 2 * (n - 2);
    int           len = (int)p.longitudo;
    Program       prs[16];
    unsigned char pat[D_PASSES][D_BITS];
    Magnus        orbit[D_PASSES + 1][16];
    Ansa          e;
    int           nb = 0;
    int           t;
    int           j;
    int           q;

    memset(o, 0, sizeof(*o));
    memset(pat, 0, sizeof(pat));
    if (len > 16) fatal("word too long for Part D");
    for (t = 0; t < len; t++) {
        build_letter(n, (long)p.litterae[t], &prs[t]);
    }
    if (!ansa_circa((i32)n, (i32)k, pool, &e)) fatal("loop refused");
    for (j = 0; j < n - 2; j++) {
        orbit[0][j] = e.a[j];
        orbit[0][n - 2 + j] = e.b[j];
    }
    for (t = 0; t < D_PASSES; t++) {
        for (j = 0; j < d; j++) orbit[t + 1][j] = orbit[t][j];
        nb = run_pass(prs, len, orbit[t + 1], pat[t], NULL);
    }
    if (nb > D_BITS) fatal("too many ReLUs for Part D");
    /* distinct regions */
    for (t = 0; t < D_PASSES; t++) {
        int seen = 0;

        for (j = 0; j < t && !seen; j++) {
            if (memcmp(pat[j], pat[t], (size_t)nb) == 0) seen = 1;
        }
        if (!seen) o->regions++;
    }
    /* the smallest q whose periodic tail is long enough */
    for (q = 1; q <= D_QMAX && !o->stable; q++) {
        int s = 0;

        for (t = D_PASSES - 1 - q; t >= 0; t--) {
            if (memcmp(pat[t], pat[t + q], (size_t)nb) != 0) {
                s = t + 1;
                break;
            }
        }
        if (D_PASSES - s >= D_TAIL && D_PASSES - s >= 3 * q) {
            o->stable = 1;
            o->start = s;
            o->period = q;
        }
    }
    if (!o->stable) return;
    /* M: the forced q-pass map on the basis vectors (column c = M e_c),
     * then v_{s+jq} = M^j v_s along the tail */
    {
        Magnus mat[16][16];
        Magnus v[16];
        int    c;
        int    r;
        int    s = o->start;

        for (c = 0; c < d; c++) {
            Magnus col[16];

            for (r = 0; r < d; r++) {
                col[r] = magnus_ex_s64(r == c ? 1 : 0);
            }
            for (t = 0; t < o->period; t++) {
                (void)run_pass(prs, len, col, NULL, pat[s + t]);
            }
            for (r = 0; r < d; r++) mat[r][c] = col[r];
        }
        o->linear_ok = 1;
        for (r = 0; r < d; r++) v[r] = orbit[s][r];
        for (t = s + o->period; t <= D_PASSES; t += o->period) {
            Magnus w[16];

            for (r = 0; r < d; r++) {
                Magnus sum = magnus_ex_s64(0);

                for (c = 0; c < d; c++) {
                    sum = magnus_adde(sum, magnus_multiplica(mat[r][c],
                        v[c], pool), pool);
                }
                w[r] = sum;
            }
            for (r = 0; r < d; r++) {
                v[r] = w[r];
                if (!magnus_aequalis(v[r], orbit[t][r])) o->linear_ok = 0;
            }
        }
    }
}

static int
lcm_int (
    int a,
    int b)
{
    int x = a;
    int y = b;

    while (y != 0) {
        int r = x % y;

        x = y;
        y = r;
    }
    return a / x * b;
}

static int
int_cmp (
    const void *x,
    const void *y)
{
    return *(const int *)x - *(const int *)y;
}

static void
part_d (
    Census *cs,
    int     count,
    int     n,
    int     maxlen)
{
    const char *names[3] = { "P", "E", "Z" };
    int         cls;
    int         periodic_ok = 1;
    int         linear_ok = 1;
    int         orbits = 0;
    int         stable_orbits = 0;
    int         ecross[2][3];
    int        *starts = (int *)malloc((size_t)(count + 1) * sizeof(int));
    char        msg[256];

    memset(ecross, 0, sizeof(ecross));
    printf("\n=== Part D: the active-ReLU pattern, B_%d up to length %d "
        "(%d passes on every E_k) ===\n\n", n, maxlen, D_PASSES);
    printf("    %-5s %7s %10s %9s %9s %6s %6s %6s %13s\n", "class",
        "braids", "stabilized", "start med", "start max", "q = 1", "q = 2",
        "q > 2", "regions mean");
    for (cls = 0; cls < 3; cls++) {
        int    braids = 0;
        int    stab = 0;
        int    q1 = 0;
        int    q2 = 0;
        int    qn = 0;
        int    ns = 0;
        long   regions = 0;
        int    i;

        for (i = 0; i < count; i++) {
            PiscinaNotatio mark;
            int            all_stable = 1;
            int            start = 0;
            int            period = 1;
            int            k;

            if (cs[i].cls != cls) continue;
            braids++;
            mark = piscina_notare(pool);
            for (k = 1; k < n; k++) {
                Orbita o;

                pattern_orbit(cs[i].word, k, &o);
                orbits++;
                regions += o.regions;
                if (!o.stable) {
                    all_stable = 0;
                    continue;
                }
                stable_orbits++;
                if (!o.linear_ok) linear_ok = 0;
                if (o.start > start) start = o.start;
                period = lcm_int(period, o.period);
                if (cls == CLS_P && (o.start != 0
                        || cs[i].period % o.period != 0)) {
                    periodic_ok = 0;
                }
            }
            piscina_reficere(pool, mark);
            if (cls == CLS_P && !all_stable) periodic_ok = 0;
            if (!all_stable) continue;
            if (cls == CLS_E) {
                ecross[cs[i].certified ? 0 : 1][period == 1 ? 0
                    : period == 2 ? 1 : 2]++;
            }
            stab++;
            starts[ns++] = start;
            if (period == 1) q1++;
            else if (period == 2) q2++;
            else qn++;
        }
        qsort(starts, (size_t)ns, sizeof(int), int_cmp);
        printf("    %-5s %7d %10d %9d %9d %6d %6d %6d %13.1f\n", names[cls],
            braids, stab, ns ? starts[ns / 2] : 0, ns ? starts[ns - 1] : 0,
            q1, q2, qn, braids ? (double)regions / (double)(braids
            * (n - 1)) : 0.0);
    }
    printf("  (per braid: stabilized = every loop's pattern repeats over "
        "its last >= %d passes; start\n   and q are the largest over the "
        "loops; regions = distinct patterns per loop over %d passes)\n",
        D_TAIL, D_PASSES);
    printf("  %d of %d orbits stabilized within the run (observed, not "
        "proved)\n", stable_orbits, orbits);
    printf("  E braids by certificate and settled period q:\n");
    printf("    %-30s %6s %6s %6s\n", "", "q = 1", "q = 2", "q > 2");
    printf("    %-30s %6d %6d %6d\n", "Burau(-1) radius > 1",
        ecross[0][0], ecross[0][1], ecross[0][2]);
    printf("    %-30s %6d %6d %6d\n", "only at a cube root of unity",
        ecross[1][0], ecross[1][1], ecross[1][2]);
    sprintf(msg, "B_%d: periodic braids repeat their ReLU pattern from "
        "pass 0, with a period dividing the braid's", n);
    check(msg, periodic_ok);
    sprintf(msg, "B_%d: on every stabilized tail the network is ONE "
        "integer linear map per period: v_{s+jq} = M^j v_s exactly", n);
    check(msg, linear_ok && stable_orbits > 0);
    free(starts);
}

/* ================================================================
 * main
 * ================================================================ */

int
main (void)
{
    pool = piscina_generare_dynamicum("demo128", 1 << 20);
    if (pool == NULL) {
        printf("cannot create pool\n");
        return 1;
    }
    printf("KNOTAPEL DEMO 128: Braids as Exact ReLU Networks\n");
    printf("================================================\n");
    celer = getenv("D128_CELER") != NULL;
    z8 = cyclotomia_creare(8, pool);
    part_a();
    {
        Census *b3;
        Census *b4;
        int     n3;
        int     n4;
        int     l3 = celer ? 5 : 8;
        int     l4 = celer ? 4 : 6;

        /* counts: full mode = the T5 review's independent Artin counts;
         * quick mode = the same breadth-first prefixes (B_3 levels 0-5,
         * B_4 levels 0-4 of the full run) */
        part_b(3, l3, celer ? 263 : 2589, &b3, &n3);
        part_b(4, l4, celer ? 469 : 5121, &b4, &n4);
        part_c(b4, n4, l4);
        part_d(b3, n3, 3, l3);
        part_d(b4, n4, 4, l4);
        free(b3);
        free(b4);
    }
    printf("\n%d passed, %d failed\n", n_pass, n_fail);
    piscina_destruere(pool);
    return n_fail == 0 ? 0 : 1;
}

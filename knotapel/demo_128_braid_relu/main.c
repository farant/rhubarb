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
 * network of depth O(m) and width 2n - 4 in which every layer touches
 * at most 4 coordinates. Plan: plan.md (approved 2026-10-15); plexus
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
    printf("\n  a word as a network (width = 2n - 4 coordinates; each "
        "layer touches <= 4):\n");
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
 *   E  positive entropy: CERTIFIED if the Burau(-1) radix is > 1
 *      (lower bound, cited), otherwise MEASURED: max_k L^(k)_60 /
 *      L^(k)_40 > 2 over the loops E_k on D_n;
 *   Z  zero entropy, not periodic (measured: the ratio is <= 2).
 * The gap is wide: a pseudo-Anosov piece on <= 4 strands has dilatation
 * >= 2.29 (minimal dilatations of B_3, B_4; cited), so its ratio over
 * 20 periods is > 10^7, while a zero-entropy braid grows at most
 * linearly (ratio about 1.5). */

typedef struct {
    Plexus word;
    int    len;
    chorda key;
    int    cls;          /* CLS_P, CLS_E, CLS_Z */
    int    certified;    /* E: Burau radix > 1 */
    double rate;         /* measured (L60/L40)^(1/20), display only */
    int    d106_pa;      /* D106's rule says pseudo-Anosov */
    long   tr;           /* trace, c1 of Burau(-1) (B_4 only) */
    long   c1;
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
    int    *affine)
{
    int    n = (int)p.fila;
    int    k;
    int    expo = 0;
    double best = 1.0;

    *repeats = period > 0;
    *affine = 1;
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
        int         expo = grows_exponentially(cs[i].word, periodic,
            &cs[i].rate, &repeats, &affine);

        if (!plexus_burau_radix_maxima(cs[i].word, pool, &rho)
                || !radix_compara(rho, radix_one(), pool, &sg)) {
            fatal("radix refused");
        }
        cs[i].certified = sg > 0;
        if (periodic) {
            cs[i].cls = CLS_P;
            /* periodic: the lengths repeat with the period, radix 1 */
            if (expo || !repeats || cs[i].certified) {
                *consistent = 0;
            }
        } else if (cs[i].certified || expo) {
            cs[i].cls = CLS_E;
            /* a certified bound must show in the growth */
            if (cs[i].certified && !expo) {
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

    printf("    %4s %8s %8s %10s %9s %8s %9s\n", "len", "braids", "P",
        "E (cert)", "E (meas)", "Z", "E share");
    for (len = 0; len <= maxlen; len++) {
        int tot = 0, p = 0, ec = 0, em = 0, z = 0;
        int i;

        for (i = 0; i < count; i++) {
            if (cs[i].len != len) continue;
            tot++;
            if (cs[i].cls == CLS_P) p++;
            else if (cs[i].cls == CLS_Z) z++;
            else if (cs[i].certified) ec++;
            else em++;
        }
        printf("    %4d %8d %8d %10d %9d %8d %8.1f%%\n", len, tot, p, ec,
            em, z, tot ? 100.0 * (ec + em) / tot : 0.0);
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
        "period (radix 1); certified ones grow exponentially; Z grows "
        "quasi-affinely", n);
    check(msg, consistent);
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

    printf("\n=== Part C: D106's rule against the exact classes "
        "(B_4 up to length %d) ===\n\n", maxlen);
    memset(cross, 0, sizeof(cross));
    for (r = 0; r < 2; r++) for (c = 0; c < 3; c++) first[r][c] = -1;
    for (i = 0; i < count; i++) {
        d106_rule(&cs[i]);
        r = cs[i].d106_pa ? 0 : 1;
        cross[r][cs[i].cls]++;
        if (first[r][cs[i].cls] < 0) first[r][cs[i].cls] = i;
    }
    printf("    %-18s %8s %8s %8s\n", "D106 says", "P", "Z", "E");
    printf("    %-18s %8d %8d %8d\n", "pseudo-Anosov", cross[0][CLS_P],
        cross[0][CLS_Z], cross[0][CLS_E]);
    printf("    %-18s %8d %8d %8d\n", "periodic", cross[1][CLS_P],
        cross[1][CLS_Z], cross[1][CLS_E]);
    printf("  (P periodic, exact; Z zero entropy, not periodic; E positive "
        "entropy)\n");
    printf("  shortest disagreements (breadth-first order):\n");
    if (first[0][CLS_P] >= 0) {
        printf("    D106 'pA', periodic:          %s\n",
            word_text(cs[first[0][CLS_P]].word));
    }
    if (first[0][CLS_Z] >= 0) {
        printf("    D106 'pA', zero entropy:      %s\n",
            word_text(cs[first[0][CLS_Z]].word));
    }
    if (first[1][CLS_E] >= 0) {
        printf("    D106 'periodic', positive:    %s (rate %.5f)\n",
            word_text(cs[first[1][CLS_E]].word),
            cs[first[1][CLS_E]].rate);
    }
    /* anchor: D106's published table (findings.md, "Pseudo-Anosov
     * Fraction Increases with Depth"). Depths 0-5: its catalog's counts
     * and its rule's 'pA' counts are reproduced exactly. Depth 6: D106
     * has 3,444 braids (2,234 'pA'), against the exact count here -
     * its catalog deduplicated by (TL, Burau) matrices, which merges
     * distinct braids (presumably: neither representation is faithful
     * as specialized there). */
    if (maxlen >= 6) {
        int d106_total[7] = { 1, 6, 26, 98, 338, 1110, 3444 };
        int d106_pa[7]    = { 0, 0, 8, 32, 146, 562, 2234 };
        int same = 1;
        int len;
        int tot6 = 0;
        int pa6 = 0;

        for (len = 0; len <= 6; len++) {
            int tot = 0, pa = 0;

            for (i = 0; i < count; i++) {
                if (cs[i].len != len) continue;
                tot++;
                if (cs[i].d106_pa) pa++;
            }
            if (len <= 5 && (tot != d106_total[len]
                    || pa != d106_pa[len])) {
                same = 0;
            }
            if (len == 6) {
                tot6 = tot;
                pa6 = pa;
            }
        }
        check("D106's table, depths 0-5 (braids and 'pA' counts), is "
            "reproduced exactly: its catalog = the distinct braids by "
            "shortest length, its rule as stated", same);
        printf("  depth 6: D106 has %d braids (%d 'pA'); exact: %d "
            "braids (%d 'pA' under its rule)\n", d106_total[6],
            d106_pa[6], tot6, pa6);
    }
    /* what the rule is: on det-1 3x3 matrices, p(1) = 0 means p(x) =
     * (x - 1)(x^2 + (1 - tr) x + 1), whose other roots lie ON the unit
     * circle iff |1 - tr| <= 2. So 'pA' = "Burau(-1) has spectral radius
     * > 1", i.e. exactly the certified set */
    {
        int same = 1;

        for (i = 0; i < count; i++) {
            if (cs[i].d106_pa != cs[i].certified) same = 0;
        }
        check("D106's 'pA' = Burau(-1) radix > 1 on every braid (its rule "
            "decides Burau's spectral radius, exactly)", same);
    }
    /* by length: D106's pA share against the positive-entropy share */
    {
        int len;

        printf("\n    %4s %8s %12s %12s\n", "len", "braids", "D106 'pA'",
            "E (exact)");
        for (len = 0; len <= maxlen; len++) {
            int tot = 0, pa = 0, e = 0;

            for (i = 0; i < count; i++) {
                if (cs[i].len != len) continue;
                tot++;
                if (cs[i].d106_pa) pa++;
                if (cs[i].cls == CLS_E) e++;
            }
            printf("    %4d %8d %11.1f%% %11.1f%%\n", len, tot,
                tot ? 100.0 * pa / tot : 0.0, tot ? 100.0 * e / tot : 0.0);
        }
    }
    /* P2, registered: disagreement in both directions */
    printf("\n  P2 (registered): D106's rule calls some zero-entropy braid "
        "'pA': %s; some positive-entropy braid 'periodic': %s\n",
        cross[0][CLS_P] + cross[0][CLS_Z] > 0 ? "YES" : "no",
        cross[1][CLS_E] > 0 ? "YES" : "no");
    /* D106's claims */
    {
        int  len1_periodic = 1;
        int  len2_pa = 0;
        int  len2_ok = 1;
        RadixRealis phi2;
        Polynomium  f;
        RadixRealis *roots;
        i32          nroots;

        (void)polynomium_ex_chorda(chorda_ex_literis("t^2 - 3t + 1", pool),
            't', pool, &f);
        (void)radices_reales(f, pool, &roots, &nroots);
        phi2 = roots[nroots - 1];
        for (i = 0; i < count; i++) {
            if (cs[i].len == 1 && cs[i].d106_pa) len1_periodic = 0;
            if (cs[i].len == 2 && cs[i].d106_pa) {
                RadixRealis rho;
                s32         sg = 1;

                len2_pa++;
                (void)plexus_burau_radix_maxima(cs[i].word, pool, &rho);
                (void)radix_compara(rho, phi2, pool, &sg);
                if (cs[i].tr != 4 || cs[i].c1 != 4 || sg != 0
                        || cs[i].cls != CLS_E) {
                    len2_ok = 0;
                }
            }
        }
        printf("  D106 claim, depth 1 all 'periodic': %s (but the six "
            "s_i are Z here: half twists, reducible)\n",
            len1_periodic ? "holds under its rule" : "FAILS");
        printf("  D106 claim, depth 2: %d 'pA' braids, all with "
            "x^3 - 4x^2 + 4x - 1 and dilatation phi^2 exactly: %s\n",
            len2_pa, len2_ok ? "holds" : "FAILS");
        check("D106 depth-2 'pA' braids: char. poly x^3 - 4x^2 + 4x - 1, "
            "Burau radix = (3 + sqrt 5)/2 exactly, positive entropy",
            len2_pa > 0 && len2_ok);
        sprintf(msg, "D106's rule disagrees with the exact classes on B_4 "
            "up to length %d in %d braids", maxlen,
            cross[0][CLS_P] + cross[0][CLS_Z] + cross[1][CLS_E]);
        printf("  %s\n", msg);
    }
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
        free(b3);
        free(b4);
    }
    printf("\n%d passed, %d failed\n", n_pass, n_fail);
    piscina_destruere(pool);
    return n_fail == 0 ? 0 : 1;
}

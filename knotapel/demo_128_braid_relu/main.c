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
    part_a();
    printf("\n%d passed, %d failed\n", n_pass, n_fail);
    piscina_destruere(pool);
    return n_fail == 0 ? 0 : 1;
}

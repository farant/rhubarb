/*
 * catalog_test.c — Quick test: what is the actual zeta_12 group size?
 *
 * Raises MAX_QCAT to 4096, extends word length to 20, raises search limit.
 * If the group stabilizes below 256, we know Demo 79's catalog was complete.
 * If it's larger, we need to retest XOR12 with the full catalog.
 *
 * C89, zero dependencies beyond math.h.
 */

#include <stdio.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct { double a, b, c, d; } Quat;

static Quat quat_make(double a, double b, double c, double d) {
    Quat q; q.a = a; q.b = b; q.c = c; q.d = d; return q;
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
static Quat quat_one(void) { return quat_make(1, 0, 0, 0); }

/* Large catalog */
#define MAX_QCAT 4096
static Quat qcat[MAX_QCAT];
static int qcat_size = 0;

static int find_quat(Quat q) {
    int i;
    for (i = 0; i < qcat_size; i++) {
        if (fabs(qcat[i].a-q.a)<1e-10 && fabs(qcat[i].b-q.b)<1e-10 &&
            fabs(qcat[i].c-q.c)<1e-10 && fabs(qcat[i].d-q.d)<1e-10)
            return i;
        if (fabs(qcat[i].a+q.a)<1e-10 && fabs(qcat[i].b+q.b)<1e-10 &&
            fabs(qcat[i].c+q.c)<1e-10 && fabs(qcat[i].d+q.d)<1e-10)
            return i;
    }
    return -1;
}

static Quat q_gen[3];

static void init_su2_generators(double half_angle) {
    double c = cos(half_angle);
    double s = sin(half_angle);
    q_gen[1] = quat_make(c, s, 0, 0);
    q_gen[2] = quat_make(c, 0, 0, -s);
}

/* Closure-based approach: multiply all known elements by generators
 * until no new elements are found. Much more reliable than word
 * enumeration for finding the full group. */
static void build_catalog_closure(void) {
    int prev_size, i;
    int round = 0;

    qcat_size = 0;

    /* Seed with identity */
    qcat[qcat_size++] = quat_one();

    /* Seed with generators and their inverses */
    {
        Quat gens[4];
        int ng = 0;
        gens[ng++] = q_gen[1];
        gens[ng++] = quat_conj(q_gen[1]);
        gens[ng++] = q_gen[2];
        gens[ng++] = quat_conj(q_gen[2]);
        for (i = 0; i < ng; i++) {
            if (find_quat(gens[i]) < 0 && qcat_size < MAX_QCAT) {
                qcat[qcat_size++] = gens[i];
            }
        }
    }

    /* Iterate: multiply every element by every generator */
    do {
        prev_size = qcat_size;
        round++;
        for (i = 0; i < prev_size; i++) {
            Quat gens[4];
            int ng = 0, gi;
            gens[ng++] = q_gen[1];
            gens[ng++] = quat_conj(q_gen[1]);
            gens[ng++] = q_gen[2];
            gens[ng++] = quat_conj(q_gen[2]);
            for (gi = 0; gi < ng; gi++) {
                Quat prod = quat_mul(qcat[i], gens[gi]);
                if (find_quat(prod) < 0) {
                    if (qcat_size < MAX_QCAT) {
                        qcat[qcat_size++] = prod;
                    } else {
                        printf("  WARNING: Hit MAX_QCAT=%d at round %d!\n",
                               MAX_QCAT, round);
                        printf("  Group is larger than %d.\n", MAX_QCAT);
                        return;
                    }
                }
            }
        }
        printf("  Round %d: %d -> %d elements\n", round, prev_size, qcat_size);
    } while (qcat_size > prev_size);

    printf("  Group CLOSED at %d elements (round %d)\n", qcat_size, round);
}

/* Also test with word enumeration for comparison */
static void build_catalog_words(void) {
    int n, len;
    qcat_size = 0;
    for (n = 2; n <= 3; n++) {
        for (len = 1; len <= 20; len++) {
            int max_gen = n - 1;
            int total_gens = 2 * max_gen;
            unsigned long total, idx_l;
            int i;
            int added_this_len = 0;

            total = 1;
            for (i = 0; i < len; i++) {
                total *= (unsigned long)total_gens;
                if (total > 2000000) break;
            }
            if (total > 2000000) {
                printf("  n=%d len=%d: skipped (%lu words)\n",
                       n, len, total);
                continue;
            }

            for (idx_l = 0; idx_l < total; idx_l++) {
                unsigned long tmp = idx_l;
                Quat result = quat_one();
                for (i = 0; i < len; i++) {
                    int g = (int)(tmp % (unsigned long)total_gens);
                    int idx;
                    Quat gen;
                    tmp /= (unsigned long)total_gens;
                    if (g < max_gen) idx = g + 1;
                    else             idx = -(g - max_gen + 1);
                    gen = idx > 0 ? q_gen[idx] : quat_conj(q_gen[-idx]);
                    result = quat_mul(result, gen);
                }
                if (find_quat(result) < 0 && qcat_size < MAX_QCAT) {
                    qcat[qcat_size] = result;
                    qcat_size++;
                    added_this_len++;
                }
            }
            if (added_this_len > 0)
                printf("  n=%d len=%d: +%d (total=%d)\n",
                       n, len, added_this_len, qcat_size);
        }
    }
    printf("  Word enumeration total: %d\n", qcat_size);
}

/* Count directions */
#define MAX_DIR 512
static double g_dir[MAX_DIR][3];
static int g_nd = 0;

static void count_directions(void) {
    int i, j;
    g_nd = 0;
    for (i = 0; i < qcat_size; i++) {
        Quat q = qcat[i];
        double norm_v, ax, ay, az;
        int found = 0;
        if (q.a < 0) { q.a = -q.a; q.b = -q.b; q.c = -q.c; q.d = -q.d; }
        norm_v = sqrt(q.b*q.b + q.c*q.c + q.d*q.d);
        if (norm_v < 1e-12) continue;
        ax = q.b / norm_v; ay = q.c / norm_v; az = q.d / norm_v;
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

/* Count distinct half-angles */
static void count_angles(void) {
    double angles[MAX_QCAT];
    int n_angles = 0;
    int i, j;

    for (i = 0; i < qcat_size; i++) {
        Quat q = qcat[i];
        double norm_v, ha;
        int found = 0;
        if (q.a < 0) { q.a = -q.a; q.b = -q.b; q.c = -q.c; q.d = -q.d; }
        if (q.a > 1.0) q.a = 1.0;
        norm_v = sqrt(q.b*q.b + q.c*q.c + q.d*q.d);
        ha = atan2(norm_v, q.a) * 180.0 / M_PI;
        for (j = 0; j < n_angles; j++) {
            if (fabs(angles[j] - ha) < 0.1) { found = 1; break; }
        }
        if (!found && n_angles < MAX_QCAT) {
            angles[n_angles++] = ha;
        }
    }
    printf("  Distinct half-angles: %d\n", n_angles);
}

int main(void) {
    printf("=== zeta_12 Catalog Size Test ===\n\n");

    printf("Generator half-angle: pi/6 = 30 degrees\n");
    printf("MAX_QCAT = %d\n\n", MAX_QCAT);

    init_su2_generators(M_PI / 6.0);

    printf("--- Method 1: Group Closure ---\n");
    build_catalog_closure();
    printf("  Catalog: %d quaternions\n", qcat_size);
    count_directions();
    printf("  Directions: %d\n", g_nd);
    count_angles();

    printf("\n--- Method 2: Word Enumeration (up to len 20) ---\n");
    build_catalog_words();
    count_directions();
    printf("  Directions: %d\n", g_nd);
    count_angles();

    printf("\n--- Also test zeta_8 for reference ---\n");
    init_su2_generators(M_PI / 4.0);
    printf("Generator half-angle: pi/4 = 45 degrees\n");
    build_catalog_closure();
    printf("  Catalog: %d quaternions\n", qcat_size);
    count_directions();
    printf("  Directions: %d\n", g_nd);
    count_angles();

    return 0;
}

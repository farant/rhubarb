/*
 * eisenstein.h -- Exact Arithmetic for Eisenstein Integers Z[omega]
 * ================================================================
 *
 * Single-header C89 library. Arithmetic in Z[omega] where
 * omega = e^{2*pi*i/3}, minimal polynomial omega^2 + omega + 1 = 0.
 *
 * Basis: {1, omega}, with omega^2 = -1 - omega.
 * Components: a + b*omega.
 *
 * Ring properties:
 *   - 6 units: {+/-1, +/-omega, +/-omega^2}
 *   - Galois group: Z/2 (conjugation: omega -> omega^2)
 *   - Norm: N(a + b*omega) = a^2 - a*b + b^2 (integer, >= 0)
 *   - Geometry: hexagonal lattice (vs Z[i]'s square lattice)
 *   - 3 ramifies: 3 = -omega^2 * (1 - omega)^2
 *
 * Name: Gotthold Eisenstein (1823-1852).
 *
 * C89, zero dependencies beyond stdlib.h/string.h.
 */

#ifndef EISENSTEIN_H
#define EISENSTEIN_H

#include <stdlib.h>
#include <string.h>

/* ================================================================
 * Eis_Z -- Element of Z[omega]
 *
 * Representation: a + b*omega, where omega^2 = -1 - omega.
 * ================================================================ */

typedef struct {
    long a, b;
} Eis_Z;

static Eis_Z eis_make(long a, long b) {
    Eis_Z z; z.a = a; z.b = b; return z;
}

static Eis_Z eis_zero(void) { return eis_make(0, 0); }
static Eis_Z eis_one(void)  { return eis_make(1, 0); }

static int eis_eq(Eis_Z x, Eis_Z y) {
    return x.a == y.a && x.b == y.b;
}

static int eis_is_zero(Eis_Z x) {
    return x.a == 0 && x.b == 0;
}

/* ---- Ring operations ---- */

static Eis_Z eis_add(Eis_Z x, Eis_Z y) {
    return eis_make(x.a + y.a, x.b + y.b);
}

static Eis_Z eis_neg(Eis_Z x) {
    return eis_make(-x.a, -x.b);
}

static Eis_Z eis_sub(Eis_Z x, Eis_Z y) {
    return eis_add(x, eis_neg(y));
}

/* (a+bw)(c+dw) = (ac - bd) + (ad + bc - bd)w
 * since w^2 = -1 - w */
static Eis_Z eis_mul(Eis_Z x, Eis_Z y) {
    return eis_make(
        x.a * y.a - x.b * y.b,
        x.a * y.b + x.b * y.a - x.b * y.b
    );
}

/* ---- Conjugation and norm ---- */

/* Galois conjugation: w -> w^2 = -1-w
 * So a + bw -> a + b(-1-w) = (a-b) + (-b)w */
static Eis_Z eis_conj(Eis_Z z) {
    return eis_make(z.a - z.b, -z.b);
}

/* Norm: N(a+bw) = (a+bw)(conj(a+bw)) = a^2 - ab + b^2
 * Always a non-negative integer. */
static long eis_norm(Eis_Z z) {
    return z.a * z.a - z.a * z.b + z.b * z.b;
}

/* ---- Unit group (order 6) ---- */

/* Multiply by omega: (a+bw)*w = -b + (a-b)w */
static Eis_Z eis_mul_omega(Eis_Z z) {
    return eis_make(-z.b, z.a - z.b);
}

/* Multiply by -omega (generator of full unit group, order 6):
 * (-w)^1 = -w, (-w)^2 = w^2, (-w)^3 = -1,
 * (-w)^4 = w, (-w)^5 = -w^2, (-w)^6 = 1.
 * (-w)(a+bw) = -(aw + bw^2) = -(-b + (a-b)w) = b + (b-a)w */
static Eis_Z eis_mul_neg_omega(Eis_Z z) {
    return eis_make(z.b, z.b - z.a);
}

/* ---- Utility functions ---- */

/* Count zero components (0-2) */
static int eis_zero_count(Eis_Z z) {
    int cnt = 0;
    if (z.a == 0) cnt++;
    if (z.b == 0) cnt++;
    return cnt;
}

/* Sign distance (ternary Hamming on 2 components) */
static int eis_sign_distance(Eis_Z x, Eis_Z y) {
    int d = 0;
    int sx_a = (x.a > 0 ? 2 : (x.a < 0 ? 0 : 1));
    int sy_a = (y.a > 0 ? 2 : (y.a < 0 ? 0 : 1));
    int sx_b = (x.b > 0 ? 2 : (x.b < 0 ? 0 : 1));
    int sy_b = (y.b > 0 ? 2 : (y.b < 0 ? 0 : 1));
    if (sx_a != sy_a) d++;
    if (sx_b != sy_b) d++;
    return d;
}

/* Axis identification for hexagonal lattice.
 * Three natural axes:
 *   0: real axis       (b == 0, multiples of 1)
 *   1: omega axis      (a == 0, multiples of omega)
 *   2: omega^2 axis    (a + b == 0, multiples of omega^2 = -1-omega)
 *  -1: zero
 *  -2: off-axis (generic position)
 */
static int eis_axis(Eis_Z z) {
    if (z.a == 0 && z.b == 0) return -1;
    if (z.b == 0) return 0;             /* real */
    if (z.a == 0) return 1;             /* omega */
    if (z.a + z.b == 0) return 2;       /* omega^2 */
    return -2;                           /* off-axis */
}

/* 3-adic valuation of an integer */
static int eis_int_3adic_val(long n) {
    int v = 0;
    if (n == 0) return -1;
    if (n < 0) n = -n;
    while (n % 3 == 0) { v++; n /= 3; }
    return v;
}

/* Norm 3-adic valuation: v_3(N(z)) */
static int eis_norm_3adic_val(Eis_Z z) {
    long n = eis_norm(z);
    return eis_int_3adic_val(n);
}

/* Hash */
static unsigned long eis_hash(Eis_Z z) {
    unsigned long h = 7919;
    h = h * 37 + (unsigned long)(z.a + 1000000);
    h = h * 37 + (unsigned long)(z.b + 1000000);
    return h;
}

/* ================================================================
 * Print layer (optional, requires stdio.h before include)
 * ================================================================ */

#ifdef EIS_PRINT

static void eis_print_z(Eis_Z z) {
    if (z.b == 0) {
        printf("%ld", z.a);
    } else if (z.a == 0) {
        if (z.b == 1) printf("w");
        else if (z.b == -1) printf("-w");
        else printf("%ldw", z.b);
    } else {
        if (z.b == 1) printf("%ld+w", z.a);
        else if (z.b == -1) printf("%ld-w", z.a);
        else if (z.b > 0) printf("%ld+%ldw", z.a, z.b);
        else printf("%ld%ldw", z.a, z.b);
    }
}

#endif /* EIS_PRINT */

#endif /* EISENSTEIN_H */

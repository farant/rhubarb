#!/usr/bin/env python3
"""Demo 122 oracle: Cell B's swept family at a given u = tan^2(t), exact,
independent of main.c's breakpoint ranking.

Entry e = (cos t, eps_e sin t d_{e//2}), eps = +1 for even e, -1 for odd
(D97's sweep order); the three axes are orthonormal. A signed sum has
real part n0 cos t and vector sin t (n_0, n_1, n_2) in the axis frame.

Sector, the D119 oracle's way: sign(c - cos(m pi/k)) with
c = n0 / sqrt(n0^2 + r u), so c^2 = n0^2 / (n0^2 + r u); same signs
compare c^2 with cos^2(m pi/k) = (1 + cos(2 m pi/k)) / 2 in Q(sqrt2, sqrt3)
(Fractions + 80-digit Decimal signs; no tan, no breakpoints). Axis: the
largest |n_j|, first in order. Verdicts: rule, robust, possible (search
with free reuse of owned cells, as in the D121 oracle).

Usage: python3 -I oracle.py U [N]
  U = a fraction like 49/100; or A,B,jj for u = A tan^2(jj pi/24) / B
  (an exact breakpoint value); or inf (90 degrees)
"""
import sys
from fractions import Fraction as F
from decimal import Decimal, getcontext
from itertools import combinations

getcontext().prec = 80
R2, R3 = Decimal(2).sqrt(), Decimal(3).sqrt()
BASIS = {(0, 0): Decimal(1), (1, 0): R2, (0, 1): R3, (1, 1): R2 * R3}


def add(x, y):
    r = dict(x)
    for k, v in y.items():
        r[k] = r.get(k, F(0)) + v
        if r[k] == 0:
            del r[k]
    return r


def mul(x, y):
    r = {}
    for (a2, a3), u in x.items():
        for (b2, b3), v in y.items():
            c = u * v
            e2, e3 = a2 + b2, a3 + b3
            if e2 == 2:
                c *= 2
                e2 = 0
            if e3 == 2:
                c *= 3
                e3 = 0
            r[(e2, e3)] = r.get((e2, e3), F(0)) + c
    return {k: v for k, v in r.items() if v != 0}


def scale(x, c):
    return {k: v * c for k, v in x.items() if v * c != 0}


def sign(x):
    if not x:
        return 0
    d = sum(Decimal(v.numerator) / Decimal(v.denominator) * BASIS[k]
            for k, v in x.items())
    assert abs(d) > Decimal(10) ** -60
    return 1 if d > 0 else -1


COS12 = {0: {(0, 0): F(1)}, 1: {(1, 1): F(1, 4), (1, 0): F(1, 4)},
         2: {(0, 1): F(1, 2)}, 3: {(1, 0): F(1, 2)}, 4: {(0, 0): F(1, 2)},
         5: {(1, 1): F(1, 4), (1, 0): F(-1, 4)}, 6: {}}


def cos_pi12(j):
    j %= 24
    if j > 12:
        j = 24 - j
    return COS12[j] if j <= 6 else scale(COS12[12 - j], -1)


def cmp_boundary(n0, r, u, m, k):
    """sign(c - cos(m pi/k)), c = n0 / sqrt(n0^2 + r u); u None = inf"""
    sb = (k > 2 * m) - (k < 2 * m)
    sa = 0 if u is None else (n0 > 0) - (n0 < 0)
    if sb == 0:
        return sa
    if sa == 0:
        return -sb
    if sa != sb:
        return sa
    # same signs: sa * sign(c^2 - b^2), c^2 = n0^2/(n0^2 + r u)
    # -> n0^2 - b^2 (n0^2 + r u), b^2 = (1 + cos(2 m pi/k)) / 2
    b2 = scale(add({(0, 0): F(1)}, cos_pi12(24 * m // k)), F(1, 2))
    if isinstance(u, tuple):
        # u = A tan^2(jj pi/24) / B = A (1 - C) / (B (1 + C)), C =
        # cos(jj pi/12): multiply through by B (1 + C) > 0
        A, B, jj = u
        one = {(0, 0): F(1)}
        cu = cos_pi12(jj)
        den = scale(add(one, cu), B)                 # B (1 + C)
        num = scale(add(one, scale(cu, -1)), A)      # A (1 - C)
        lhs = scale(den, n0 * n0)
        rhs = mul(b2, add(scale(den, n0 * n0), scale(num, r)))
        return sa * sign(add(lhs, scale(rhs, -1)))
    return sa * sign(add({(0, 0): F(n0 * n0)}, scale(b2, -(n0 * n0 + r * u))))


def cells(n0, nv, u, k):
    nvor = 4
    r = sum(x * x for x in nv)
    real = 0 if u is None else n0
    if r == 0:
        if real == 0:
            z = (k - 1) * nvor + 3
            return z, [z]
        c = (0 if real > 0 else k - 1) * nvor + 3
        return c, [c]
    rule = 0
    tie = None
    for m in range(1, k):
        s = cmp_boundary(n0, r, u, m, k)
        if s <= 0:
            rule = m
        if s == 0:
            tie = m
    secs = [tie - 1, tie] if tie else [rule]
    mx = max(abs(x) for x in nv)
    axes = [j for j in range(3) if abs(nv[j]) == mx]
    return rule * nvor + axes[0], [s * nvor + a for s in secs for a in axes]


def possible(items):
    lab = {}

    def go(rem):
        while True:
            free, best = None, None
            for i in rem:
                ties, t = items[i]
                viable = [c for c in ties if lab.get(c, t) == t]
                if not viable:
                    return False
                if any(lab.get(c) == t for c in viable):
                    free = i
                    break
                if best is None or len(viable) < len(best[1]):
                    best = (i, viable)
            if free is None:
                break
            rem = rem - {free}
        if not rem:
            return True
        i, viable = best
        rest = rem - {i}
        for c in viable:
            lab[c] = items[i][1]
            if go(rest):
                return True
            del lab[c]
        return False

    return go(frozenset(range(len(items))))


def judge(idx, u):
    n = len(idx)
    pts = []
    for mask in range(1 << n):
        n0, nv = 0, [0, 0, 0]
        for i, e in enumerate(idx):
            s = 1 if (mask >> i) & 1 else -1
            n0 += s
            nv[e // 2] += s * (1 if e % 2 == 0 else -1)
        pts.append((n0, tuple(nv), bin(mask).count('1') & 1))
    rule = rob = pos = False
    for k in (6, 12, 24):
        cl = [cells(n0, nv, u, k) for n0, nv, _ in pts]
        own = {}
        ok = True
        for (c, _), (_, _, t) in zip(cl, pts):
            if own.setdefault(c, t) != t:
                ok = False
        rule |= ok
        reach = {}
        for (_, ties), (_, _, t) in zip(cl, pts):
            for c in ties:
                reach.setdefault(c, set()).add(t)
        rob |= all(len(v) < 2 for v in reach.values())
        if not pos:
            byvec = {}
            for (_, ties), (n0, nv, t) in zip(cl, pts):
                key = (0 if u is None else n0, nv)
                byvec.setdefault(key, (ties, set()))[1].add(t)
            if all(len(ts) == 1 for _, ts in byvec.values()):
                pos = possible([(ties, next(iter(ts)))
                                for ties, ts in byvec.values()])
    return rule, rob, pos


if sys.argv[1] == 'inf':
    u = None
elif ',' in sys.argv[1]:
    u = tuple(int(x) for x in sys.argv[1].split(','))   # A,B,jj
else:
    u = F(sys.argv[1])
ns = [int(sys.argv[2])] if len(sys.argv) > 2 else [3, 4, 5, 6]
out = []
for n in ns:
    r = rb = p = 0
    for idx in combinations(range(6), n):
        a, b, c = judge(idx, u)
        r += a
        rb += b
        p += c
    out.append('%d/%d/%d' % (r, rb, p))
print('u', sys.argv[1], 'N=3..6 rule/robust/possible:', ' '.join(out))

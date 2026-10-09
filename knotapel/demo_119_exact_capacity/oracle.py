#!/usr/bin/env python3
"""Demo 119 oracle (zeta_8 part) - independent of the house libraries
and of main.c's float filter.

Numbers: Q(sqrt2, sqrt3) as dicts {basis: Fraction} over the basis
1, sqrt2, sqrt3, sqrt6. Zero is decided exactly (all coefficients 0);
the sign of a nonzero number by 80-digit decimal evaluation (the numbers
here have tiny height: a nonzero one is nowhere near 10^-80).

Rebuilds D94's zeta_8 catalog (BFS order, +-q identified) and its
directions, then for every weight set of N = 3 and 4 (exhaustive, as
D94) and each truth table (XOR, AND, MAJ) decides, EXACTLY for every
mask and every k in {6, 12, 24}:
  rule      D94's formula exactly: sector = #{m : c <= cos(m pi/k)},
            direction = first axis maximizing |cos|
  robust    no cell reachable (over all ties) from both truth values
  possible  some choice of one cell per DISTINCT sum vector (from its
            tie set) puts no cell under both truth values
each OR-ed over k. Prints the counts per (N, function).

Usage: python3 -I oracle.py [maxN]   (default 4)
"""
import sys
from fractions import Fraction as F
from decimal import Decimal, getcontext
from itertools import combinations

getcontext().prec = 80
R2 = Decimal(2).sqrt()
R3 = Decimal(3).sqrt()
R6 = Decimal(6).sqrt()
BASIS = {(0, 0): Decimal(1), (1, 0): R2, (0, 1): R3, (1, 1): R6}


def num(c=0, b=None):
    """c (rational) or a dict"""
    if b is not None:
        return b
    return {(0, 0): F(c)} if c else {}


def add(x, y):
    r = dict(x)
    for k, v in y.items():
        r[k] = r.get(k, F(0)) + v
        if r[k] == 0:
            del r[k]
    return r


def neg(x):
    return {k: -v for k, v in x.items()}


def sub(x, y):
    return add(x, neg(y))


def mul(x, y):
    r = {}
    for (a2, a3), u in x.items():
        for (b2, b3), v in y.items():
            c = u * v
            # sqrt2^(a2+b2) sqrt3^(a3+b3)
            e2, e3 = a2 + b2, a3 + b3
            if e2 == 2:
                c *= 2
                e2 = 0
            if e3 == 2:
                c *= 3
                e3 = 0
            r[(e2, e3)] = r.get((e2, e3), F(0)) + c
    return {k: v for k, v in r.items() if v != 0}


def sign(x):
    if not x:
        return 0
    d = sum(Decimal(v.numerator) / Decimal(v.denominator) * BASIS[k]
            for k, v in x.items())
    assert abs(d) > Decimal(10) ** -60, "sign too close to 0"
    return 1 if d > 0 else -1


HALF_R2 = {(1, 0): F(1, 2)}
ONE = num(1)
ZERO = {}


def q_mul(p, q):
    a1, b1, c1, d1 = p
    a2, b2, c2, d2 = q
    return (
        sub(sub(sub(mul(a1, a2), mul(b1, b2)), mul(c1, c2)), mul(d1, d2)),
        sub(add(add(mul(a1, b2), mul(b1, a2)), mul(c1, d2)), mul(d1, c2)),
        add(add(sub(mul(a1, c2), mul(b1, d2)), mul(c1, a2)), mul(d1, b2)),
        add(sub(add(mul(a1, d2), mul(b1, c2)), mul(c1, b2)), mul(d1, a2)),
    )


def q_neg(q):
    return tuple(neg(x) for x in q)


def key(q):
    return tuple(tuple(sorted(x.items())) for x in q)


# ---- D94's zeta_8 catalog (BFS rounds) ----
s1 = (HALF_R2, HALF_R2, ZERO, ZERO)
s1i = (HALF_R2, neg(HALF_R2), ZERO, ZERO)
s2 = (HALF_R2, ZERO, ZERO, neg(HALF_R2))
s2i = (HALF_R2, ZERO, ZERO, HALF_R2)
gens = [s1, s1i, s2, s2i]
cat = [(ONE, ZERO, ZERO, ZERO)]
seen = {key(cat[0]), key(q_neg(cat[0]))}
for g in gens:
    if key(g) not in seen:
        cat.append(g)
        seen.add(key(g))
        seen.add(key(q_neg(g)))
while True:
    prev = len(cat)
    for i in range(prev):
        for g in gens:
            p = q_mul(cat[i], g)
            if key(p) not in seen:
                cat.append(p)
                seen.add(key(p))
                seen.add(key(q_neg(p)))
    if len(cat) == prev:
        break
assert len(cat) == 24, len(cat)

# ---- directions (D94 build_dirs order; parallel = cross product 0) ----


def cross_zero(u, v):
    b1, c1, d1 = u[1:]
    b2, c2, d2 = v[1:]
    return (not sub(mul(c1, d2), mul(d1, c2))
            and not sub(mul(d1, b2), mul(b1, d2))
            and not sub(mul(b1, c2), mul(c1, b2)))


dirs = []
for q in cat:
    if not q[1] and not q[2] and not q[3]:
        continue
    if not any(cross_zero(q, cat[j]) for j in dirs):
        dirs.append(cat.index(q))
assert len(dirs) == 13, len(dirs)
ND = len(dirs)


def vdot(p, q):
    return add(add(mul(p[1], q[1]), mul(p[2], q[2])), mul(p[3], q[3]))


# boundaries cos(m pi/k) exactly in Q(sqrt2, sqrt3)
COS = {}
# cos(j pi/12) for j = 0..12 (k = 24 boundaries are cos(m pi/24): need
# cos(pi/24) - not in Q(sqrt2, sqrt3)!)

# cos(m pi/24) has degree 8; handle k = 24 by comparing squares:
# c <= cos(t) with c = a/|S|: sign analysis with cos^2(t) = (1 + cos 2t)/2,
# cos(2t) = cos(m pi/12) in Q(sqrt2, sqrt3), and sign(cos t) known.
COS12 = {
    0: num(1),
    1: {(1, 1): F(1, 4), (1, 0): F(1, 4)},          # (sqrt6 + sqrt2)/4
    2: {(0, 1): F(1, 2)},                           # sqrt3/2
    3: {(1, 0): F(1, 2)},                           # sqrt2/2
    4: num(F(1, 2)),
    5: {(1, 1): F(1, 4), (1, 0): F(-1, 4)},         # (sqrt6 - sqrt2)/4
    6: {},
}


def cos_pi12(j):
    """cos(j pi/12), any integer j"""
    j %= 24
    if j > 12:
        j = 24 - j
    if j <= 6:
        return COS12[j]
    return neg(COS12[12 - j])


def cmp_c_boundary(a, nn, m, k):
    """sign(a/sqrt(nn) - cos(m pi/k)), exact. cos(m pi/k) = cos(t),
    t = m pi/k in (0, pi): sign(cos t) = sign of (k - 2m); cos^2 t =
    (1 + cos(2m pi/k))/2 and 2m pi/k = (24m/k) pi/12."""
    sb = (k > 2 * m) - (k < 2 * m)
    sa = sign(a)
    if sb == 0:
        return sa
    if sa == 0:
        return -sb
    if sa != sb:
        return sa
    c2 = mul(add(num(1), cos_pi12(24 * m // k)), num(F(1, 2)))
    return sa * sign(sub(mul(a, a), mul(c2, nn)))


def cells(sum_q, k):
    """(rule cell, tie set) for one exact sum and k"""
    n_vor = ND + 1
    a, b, c, d = sum_q
    nn = add(add(mul(a, a), mul(b, b)), add(mul(c, c), mul(d, d)))
    if not nn:
        z = (k - 1) * n_vor + ND
        return z, [z]
    rule_sec = 0
    tie = None
    for m in range(1, k):
        s = cmp_c_boundary(a, nn, m, k)
        if s <= 0:
            rule_sec = m
        if s == 0:
            tie = m
    secs = [tie - 1, tie] if tie else [rule_sec]
    if not b and not c and not d:
        dset, drule = [ND], ND
    else:
        best = None
        dset = []
        for j, ci in enumerate(dirs):
            u = cat[ci]
            dv = vdot(sum_q, u)
            val = (mul(dv, dv), vdot(u, u))     # (dot^2, |u|^2)
            if best is None:
                best, dset = val, [j]
                continue
            s = sign(sub(mul(val[0], best[1]), mul(best[0], val[1])))
            if s > 0:
                best, dset = val, [j]
            elif s == 0:
                dset.append(j)
        drule = min(dset)
    rule = rule_sec * n_vor + drule
    ties = [s * n_vor + dd for s in secs for dd in dset]
    return rule, ties


def truth(fn, mask, n):
    pc = bin(mask).count("1")
    return [pc % 2, int(mask == (1 << n) - 1), int(pc > n // 2)][fn]


def verdicts(idx, n, fn, cache):
    rule_any = robust_any = poss_any = False
    sums = []
    for mask in range(1 << n):
        s = (ZERO, ZERO, ZERO, ZERO)
        for i, j in enumerate(idx):
            q = cat[j]
            s = tuple(add(x, y) if (mask >> i) & 1 else sub(x, y)
                      for x, y in zip(s, q))
        sums.append(s)
    # distinct vectors with their truth values
    byvec = {}
    for mask, s in enumerate(sums):
        byvec.setdefault(key(s), (s, set()))[1].add(truth(fn, mask, n))
    clash = any(len(t) == 2 for _, t in byvec.values())
    for k in (6, 12, 24):
        cl = {}
        for kk, (s, _) in byvec.items():
            ck = (kk, k)
            if ck not in cache:
                cache[ck] = cells(s, k)
            cl[kk] = cache[ck]
        # rule
        owner = {}
        ok = True
        for mask, s in enumerate(sums):
            c = cl[key(s)][0]
            t = truth(fn, mask, n)
            if owner.setdefault(c, t) != t:
                ok = False
                break
        rule_any |= ok
        # robust
        reach = {}
        for mask, s in enumerate(sums):
            for c in cl[key(s)][1]:
                reach.setdefault(c, set()).add(truth(fn, mask, n))
        robust_any |= all(len(v) < 2 for v in reach.values())
        # possible: per distinct vector, backtracking
        if not clash and not poss_any:
            items = sorted(((cl[kk][1], next(iter(t)))
                            for kk, (_, t) in byvec.items()),
                           key=lambda it: len(it[0]))
            own = {}

            def go(i):
                if i == len(items):
                    return True
                ties, t = items[i]
                for c in ties:
                    p = own.get(c)
                    if p is not None and p != t:
                        continue
                    fresh = p is None
                    own[c] = t
                    if go(i + 1):
                        return True
                    if fresh:
                        del own[c]
                return False

            poss_any |= go(0)
    return rule_any, robust_any, poss_any


maxn = int(sys.argv[1]) if len(sys.argv) > 1 else 4
cache = {}
for n in range(3, maxn + 1):
    for fn, name in enumerate(("XOR", "AND", "MAJ")):
        r = rb = p = 0
        for idx in combinations(range(24), n):
            a, b, c = verdicts(idx, n, fn, cache)
            r += a
            rb += b
            p += c
        print(n, name, "rule", r, "robust", rb, "possible", p, flush=True)

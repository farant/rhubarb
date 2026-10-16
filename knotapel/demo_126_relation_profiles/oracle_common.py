"""Exact arithmetic for oracle_nulls.py. Written by the independent
reviewer of D126 (2026-10-13), adopted unchanged except for this header;
shares no code with oracle.py or the house libraries.

Q(sqrt d) elements are pairs (a, b) of Fractions meaning a + b sqrt d.
Quaternions are 4-tuples of such pairs, Hamilton product.
"""
from fractions import Fraction as F
from itertools import combinations, product


class K:
    def __init__(self, d):
        self.d = d

    def add(self, x, y):
        return (x[0] + y[0], x[1] + y[1])

    def sub(self, x, y):
        return (x[0] - y[0], x[1] - y[1])

    def mul(self, x, y):
        return (x[0] * y[0] + self.d * x[1] * y[1], x[0] * y[1] + x[1] * y[0])

    def sign(self, x):
        a, b = x
        sa = (a > 0) - (a < 0)
        sb = (b > 0) - (b < 0)
        if sb == 0:
            return sa
        if sa == 0:
            return sb
        if sa == sb:
            return sa
        # a and b sqrt d of opposite signs: compare a^2 with d b^2
        c = a * a - self.d * b * b
        if c == 0:
            return 0
        return sa if c > 0 else sb

    def qmul(self, p, q):
        a1, b1, c1, d1 = p
        a2, b2, c2, d2 = q
        m, ad, sb = self.mul, self.add, self.sub
        return (
            sb(sb(sb(m(a1, a2), m(b1, b2)), m(c1, c2)), m(d1, d2)),
            sb(ad(ad(m(a1, b2), m(b1, a2)), m(c1, d2)), m(d1, c2)),
            ad(ad(sb(m(a1, c2), m(b1, d2)), m(c1, a2)), m(d1, b2)),
            ad(sb(ad(m(a1, d2), m(b1, c2)), m(c1, b2)), m(d1, a2)),
        )

    def qneg(self, p):
        return tuple((-x[0], -x[1]) for x in p)

    def qnorm(self, p):
        s = (F(0), F(0))
        for x in p:
            s = self.add(s, self.mul(x, x))
        return s

    def qdot(self, p, q):
        s = (F(0), F(0))
        for x, y in zip(p, q):
            s = self.add(s, self.mul(x, y))
        return s

    def canon(self, p):
        for x in p:
            s = self.sign(x)
            if s != 0:
                return p if s > 0 else self.qneg(p)
        return p


def kq(*parts):
    """parts: each either a Fraction-like (rational) or a pair (a, b)."""
    out = []
    for x in parts:
        if isinstance(x, tuple):
            out.append((F(x[0]), F(x[1])))
        else:
            out.append((F(x), F(0)))
    return tuple(out)


def close(k, gens):
    """BFS closure of {1} under right multiplication by gens, in order
    (same order as the demo's group_close, to replay its null draws)."""
    one = kq(1, 0, 0, 0)
    el = [one]
    seen = {one}
    cur = 0
    while cur < len(el):
        for g in gens:
            x = k.qmul(el[cur], g)
            if x not in seen:
                seen.add(x)
                el.append(x)
                if len(el) > 500:
                    raise RuntimeError("not finite")
        cur += 1
    return el


def order(k, x):
    one = kq(1, 0, 0, 0)
    y = x
    o = 1
    while y != one:
        y = k.qmul(y, x)
        o += 1
    return o


def mod_sign(k, els):
    reps = []
    seen = set()
    for g in els:
        c = k.canon(g)
        if c not in seen:
            seen.add(c)
            reps.append(c)
    return reps


def coords(p):
    """Q^8 vector: (a0, b0, a1, b1, ...)."""
    v = []
    for x in p:
        v.append(x[0])
        v.append(x[1])
    return tuple(v)


def int_coords(pts):
    """Scale all Q^8 coordinate vectors by one common denominator."""
    from math import lcm
    vs = [coords(p) for p in pts]
    L = 1
    for v in vs:
        for c in v:
            L = lcm(L, c.denominator)
    return [tuple(int(c * L) for c in v) for v in vs], L


def rank(vectors):
    """rank over Q of a list of vectors (Gaussian elimination)."""
    rows = [list(map(F, v)) for v in vectors]
    r = 0
    ncol = len(rows[0]) if rows else 0
    for c in range(ncol):
        piv = None
        for i in range(r, len(rows)):
            if rows[i][c] != 0:
                piv = i
                break
        if piv is None:
            continue
        rows[r], rows[piv] = rows[piv], rows[r]
        for i in range(len(rows)):
            if i != r and rows[i][c] != 0:
                f = rows[i][c] / rows[r][c]
                rows[i] = [a - f * b for a, b in zip(rows[i], rows[r])]
        r += 1
    return r


def vadd(u, v):
    return tuple(a + b for a, b in zip(u, v))


def vneg(u):
    return tuple(-a for a in u)


def signed_subsets(vs, size, idxs=None):
    """yield (indices, signs, sum) for every signed subset of exact size;
    the first element's sign is NOT fixed here."""
    n = len(vs)
    dim = len(vs[0])
    zero = (0,) * dim
    for comb in combinations(range(n) if idxs is None else idxs, size):
        for sg in product((1, -1), repeat=size):
            s = zero
            for i, e in zip(comb, sg):
                s = vadd(s, vs[i]) if e > 0 else vadd(s, vneg(vs[i]))
            yield comb, sg, s


def pm1_counts(vs, max_support):
    """Number of x in {-1,0,1}^N, support s, sum x_i v_i = 0, modulo
    overall sign, for s = 1..max_support. Meet in the middle: the support
    is split into its a = ceil(s/2) smallest indices and the rest."""
    from collections import defaultdict
    out = {}
    cache = {}

    def table(size):
        if size not in cache:
            t = defaultdict(list)
            for comb, sg, s in signed_subsets(vs, size):
                t[s].append(comb[-1])
            for key in t:
                t[key].sort()
            cache[size] = t
        return cache[size]

    import bisect
    for s in range(1, max_support + 1):
        a = (s + 1) // 2
        b = s - a
        if b == 0:
            cnt = sum(1 for comb, sg, sm in signed_subsets(vs, a)
                      if not any(sm))
        else:
            ta = table(a)
            cnt = 0
            for comb, sg, sm in signed_subsets(vs, b):
                lst = ta.get(vneg(sm))
                if lst:
                    cnt += bisect.bisect_left(lst, comb[0])
        assert cnt % 2 == 0
        out[s] = cnt // 2
    return out

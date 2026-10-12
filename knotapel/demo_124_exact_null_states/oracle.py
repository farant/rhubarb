#!/usr/bin/env python3
"""Demo 124 oracle: D84's zeta_8 capacity, exactly, independently.

No radices, no S_N polynomials, no code shared with main.c. Q(sqrt2)
numbers are pairs of Fractions (p + q sqrt2). The catalog is D84's BFS
closure (generators (cos pi/4, sin pi/4, 0, 0), (cos pi/4, 0, 0,
-sin pi/4) and conjugates), rebuilt here with exact equality up to sign.

Cells follow D84's combined_cell with exact arithmetic:
  sector: floor(2 h k / 360) for the half-angle h of the sum (sign of
          the real part flipped away); h = 90 exactly when Re = 0;
          compared through tan^2 h = |v|^2 / a^2 in Q(sqrt2) against
          tan^2(m pi / k). tan^2(m pi/k) lies in Q(sqrt2) only for m/k
          in {1/8, 1/6, 1/4, 1/3, 3/8} (values 3 - 2 sqrt2, 1/3, 1, 3,
          3 + 2 sqrt2); there the comparison is exact, elsewhere a
          Q(sqrt2) value cannot equal it and 60-digit Decimal decides.
  axis:   first maximum of (v.u)^2 / |u|^2 over the subset's axes in
          first-appearance order (ties: every maximum).
Separability uses the 3^n coefficient vectors of a weight set (a mask's
parity = number of nonzero coefficients mod 2); rule = the exact cells,
robust = every tied cell, fixed rules = sector lower/upper x axis
first/last. Level-wise search over all weight sets (downward closed per
k), union over D84's ladder k = 1, 6, 8, 10, 12, 16, 20, 24.

Run: python3 -I knotapel/demo_124_exact_null_states/oracle.py
"""
from fractions import Fraction as F
from decimal import Decimal, getcontext
from itertools import combinations

getcontext().prec = 60


class Q2:
    __slots__ = ("p", "q")

    def __init__(self, p, q=0):
        self.p = F(p)
        self.q = F(q)

    def __add__(self, o):
        return Q2(self.p + o.p, self.q + o.q)

    def __sub__(self, o):
        return Q2(self.p - o.p, self.q - o.q)

    def __neg__(self):
        return Q2(-self.p, -self.q)

    def __mul__(self, o):
        return Q2(self.p * o.p + 2 * self.q * o.q, self.p * o.q + self.q * o.p)

    def inv(self):
        d = self.p * self.p - 2 * self.q * self.q
        return Q2(self.p / d, -self.q / d)

    def zero(self):
        return self.p == 0 and self.q == 0

    def __eq__(self, o):
        return self.p == o.p and self.q == o.q

    def __hash__(self):
        return hash((self.p, self.q))

    def sign(self):
        p, q = self.p, self.q
        if p >= 0 and q >= 0:
            return 0 if (p == 0 and q == 0) else 1
        if p <= 0 and q <= 0:
            return -1
        if p > 0:
            return 1 if p * p > 2 * q * q else -1
        return 1 if 2 * q * q > p * p else -1

    def dec(self):
        return Decimal(self.p.numerator) / Decimal(self.p.denominator) + \
            Decimal(self.q.numerator) / Decimal(self.q.denominator) * \
            Decimal(2).sqrt()


def qmul(x, y):
    a, b, c, d = x
    e, f, g, h = y
    return (a * e - b * f - c * g - d * h,
            a * f + b * e + c * h - d * g,
            a * g - b * h + c * e + d * f,
            a * h + b * g - c * f + d * e)


def qneg(x):
    return tuple(-t for t in x)


def build_catalog():
    r = Q2(0, F(1, 2))
    zero = Q2(0)
    g1 = (r, r, zero, zero)
    g2 = (r, zero, zero, -r)
    gens = [g1, (r, -r, zero, zero), g2, (r, zero, zero, r)]
    cat = [(Q2(1), zero, zero, zero)]

    def find(q):
        for i, c in enumerate(cat):
            if c == q or c == qneg(q):
                return i
        return -1

    for g in gens:
        if find(g) < 0:
            cat.append(g)
    while True:
        prev = len(cat)
        for i in range(prev):
            for g in gens:
                p = qmul(cat[i], g)
                if find(p) < 0:
                    cat.append(p)
        if len(cat) == prev:
            break
    return cat


def pi_dec():
    getcontext().prec += 2
    three = Decimal(3)
    lasts, t, s, n, na, d, da = 0, three, 3, 1, 0, 0, 24
    while s != lasts:
        lasts = s
        n, na = n + na, na + 8
        d, da = d + da, da + 32
        t = (t * n) / d
        s += t
    getcontext().prec -= 2
    return +s


def sin_cos(x):
    getcontext().prec += 2
    i, lasts, s, fact, num, sign = 0, 0, Decimal(1), 1, Decimal(1), 1
    while s != lasts:
        lasts = s
        i += 2
        fact *= i * (i - 1)
        num *= x * x
        sign *= -1
        s += num / fact * sign
    c = s
    i, lasts, s, fact, num, sign = 1, 0, x, 1, x, 1
    while s != lasts:
        lasts = s
        i += 2
        fact *= i * (i - 1)
        num *= x * x
        sign *= -1
        s += num / fact * sign
    getcontext().prec -= 2
    return +s, +c


PI = pi_dec()
EXACT_B = {F(1, 8): Q2(3, -2), F(1, 6): Q2(F(1, 3)), F(1, 4): Q2(1),
           F(1, 3): Q2(3), F(3, 8): Q2(3, 2)}
BOUND = {}


def boundaries(k):
    if k not in BOUND:
        out = []
        for m in range(1, (k + 1) // 2):
            if 2 * m >= k:
                break
            fr = F(m, k)
            if fr in EXACT_B:
                out.append(("x", EXACT_B[fr]))
            else:
                s, c = sin_cos(PI * m / k)
                out.append(("d", (s * s) / (c * c)))
        BOUND[k] = out
    return BOUND[k]


def axes_of(cat, idx):
    axes = []
    for i in idx:
        v = cat[i][1:]
        if all(t.zero() for t in v):
            continue
        par = False
        for u in axes:
            cx = v[1] * u[2] - v[2] * u[1]
            cy = v[2] * u[0] - v[0] * u[2]
            cz = v[0] * u[1] - v[1] * u[0]
            if cx.zero() and cy.zero() and cz.zero():
                par = True
                break
        if not par:
            axes.append(v)
    return axes


class Cells:
    def __init__(self, axes):
        self.axes = axes
        self.n2 = [u[0] * u[0] + u[1] * u[1] + u[2] * u[2] for u in axes]
        self.memo = {}

    def info(self, s):
        if s in self.memo:
            return self.memo[s]
        a, v = s[0], s[1:]
        v2 = v[0] * v[0] + v[1] * v[1] + v[2] * v[2]
        if v2.zero():
            r = ("zero",) if a.zero() else ("real",)
        else:
            best = None
            ties = []
            for j, u in enumerate(self.axes):
                dot = v[0] * u[0] + v[1] * u[1] + v[2] * u[2]
                num = dot * dot
                if best is None:
                    c = 1
                else:
                    c = (num * self.n2[best[1]] - best[0] * self.n2[j]).sign()
                if c > 0:
                    best = (num, j)
                    ties = [j]
                elif c == 0:
                    ties.append(j)
            if a.zero():
                r = ("null", ties)
            else:
                t = v2 * (a * a).inv()
                r = ("gen", ties, t, t.dec())
        self.memo[s] = r
        return r

    def secs(self, inf, k):
        """(rule sector, tie flag)"""
        if inf[0] == "null":
            return k // 2, k % 2 == 0
        t, td = inf[2], inf[3]
        sec, tie = 0, False
        for kind, b in boundaries(k):
            if kind == "x":
                c = (t - b).sign()
            else:
                c = 1 if td > b else -1
            if c < 0:
                break
            sec += 1
            tie = c == 0
        return sec, tie

    def cells(self, s, k, mode):
        """mode: 'rule', 'robust', ('fixed', upper, last)"""
        nd = len(self.axes)
        nv = nd + 1
        inf = self.info(s)
        if inf[0] == "zero":
            return [(k - 1) * nv + nd]
        if inf[0] == "real":
            return [nd]
        sec, tie = self.secs(inf, k)
        ties = inf[1]
        if mode == "rule":
            return [sec * nv + ties[0]]
        if mode == "robust":
            ss = [sec, sec - 1] if tie else [sec]
            return [x * nv + j for x in ss for j in ties]
        _, upper, last = mode
        x = sec - 1 if (tie and not upper) else sec
        return [x * nv + (ties[-1] if last else ties[0])]


def separable(cat, cells, w, k, mode):
    n = len(w)
    seen = {}
    for coeffs in range(3 ** n):
        x = coeffs
        s = (Q2(0), Q2(0), Q2(0), Q2(0))
        par = 0
        for i in range(n):
            dgt = x % 3
            x //= 3
            if dgt == 1:
                s = tuple(p + q for p, q in zip(s, cat[w[i]]))
                par ^= 1
            elif dgt == 2:
                s = tuple(p - q for p, q in zip(s, cat[w[i]]))
                par ^= 1
        for c in cells.cells(s, k, mode):
            if seen.setdefault(c, par) != par:
                return False
    return True


LADDER = [1, 6, 8, 10, 12, 16, 20, 24]


def capacity(cat, idx, mode, axes=None):
    cells = Cells(axes_of(cat, idx) if axes is None else axes)
    union = {n: set() for n in range(1, 8)}
    for k in LADDER:
        lvl = {1: set()}
        for i in range(len(idx)):
            if separable(cat, cells, [idx[i]], k, mode):
                lvl[1].add(frozenset([i]))
        for n in range(2, 8):
            lvl[n] = set()
            for base in lvl[n - 1]:
                hi = max(base)
                for j in range(hi + 1, len(idx)):
                    cand = base | {j}
                    if all((cand - {b}) in lvl[n - 1] for b in base):
                        if separable(cat, cells, [idx[t] for t in sorted(cand)],
                                     k, mode):
                            lvl[n].add(cand)
        for n in range(1, 8):
            union[n] |= lvl[n]
    counts = [len(union[n]) for n in range(3, 8)]
    mx = max([2 * n for n in range(3, 8) if union[n]] or [0])
    return mx, counts, union


def main():
    cat = build_catalog()
    nulls = [i for i, q in enumerate(cat) if q[0].zero()]
    nonnull = [i for i, q in enumerate(cat) if not q[0].zero()]
    full = list(range(len(cat)))
    print("catalog", len(cat), "null", len(nulls), "axes",
          len(axes_of(cat, full)))
    for mode in ("rule", "robust"):
        for name, idx in (("full", full), ("nonnull", nonnull),
                          ("null", nulls)):
            mx, counts, union = capacity(cat, idx, mode)
            line = "%s %s max %d counts %s" % (mode, name, mx,
                                               " ".join(map(str, counts)))
            if name == "full" and mode == "rule":
                for n in (3, 4):
                    nm = sum(sum(1 for t in s if cat[full[t]][0].zero())
                             for s in union[n])
                    line += " nullmembers%d %d" % (n, nm)
            print(line)
    for upper in (0, 1):
        for last in (0, 1):
            res = [capacity(cat, idx, ("fixed", upper, last))[0]
                   for idx in (full, nonnull, nulls)]
            print("fixed upper %d last %d max %s" % (upper, last,
                                                     " ".join(map(str, res))))
    full_axes = axes_of(cat, full)
    for mode in ("rule", "robust"):
        for name, idx in (("nonnull", nonnull), ("null", nulls)):
            mx, counts, _ = capacity(cat, idx, mode, full_axes)
            print("%s %s fullaxes max %d counts %s" % (
                mode, name, mx, " ".join(map(str, counts))))
    for mode in ("rule", "robust"):
        for name, idx in (("full", full), ("nonnull", nonnull),
                          ("null", nulls)):
            cells = Cells(axes_of(cat, idx))
            row = []
            for k in (1, 2, 4, 6, 8):
                row.append(sum(1 for w in combinations(idx, 3)
                               if separable(cat, cells, list(w), k, mode)))
            print("sweep %s %s %s" % (mode, name, " ".join(map(str, row))))


main()

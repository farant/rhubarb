#!/usr/bin/env python3
"""Independent oracle for Demo 125.

Rebuilds the zeta_12 catalog exactly (Q(sqrt3) with Fractions, D82's BFS
order), recomputes the census, and re-decides every winner that main.c
wrote to $D125_VICTORES: each winner must separate at its recorded k and
at no earlier k of the ladder (under its mode: rule or robust, with its
axis table: the first 512 axes or all).

Shares no code with cellae. Built on the cellae reviewer's independent
zeta_12 code (recensor-extensio, cellae T8 review, 2026-10-12): closure,
axis keys and the cell oracle; the separation test and the robust mode
are new here.

  D125_VICTORES=/tmp/v.tsv ./build/aedilis/main/main > out.txt
  python3 knotapel/demo_125_exact_crossing_depth/oracle.py /tmp/v.tsv [-j N]

Needs numpy (used only to shortlist axes within a relative 1e-6 of the
float maximum; the decision among the shortlist is exact).
"""
import itertools
import math
import sys
import time
from collections import Counter
from decimal import Decimal, getcontext
from fractions import Fraction as F
from multiprocessing import Pool

import numpy as np

# ---------------------------------------------------------------- Q(sqrt3)
Z = (F(0), F(0))


def add(x, y): return (x[0] + y[0], x[1] + y[1])
def sub(x, y): return (x[0] - y[0], x[1] - y[1])
def neg(x): return (-x[0], -x[1])
def mul(x, y): return (x[0] * y[0] + 3 * x[1] * y[1], x[0] * y[1] + x[1] * y[0])


def sgn(x):
    p, q = x
    if p >= 0 and q >= 0:
        return 0 if p == 0 and q == 0 else 1
    if p <= 0 and q <= 0:
        return -1
    if p > 0:
        return 1 if p * p > 3 * q * q else -1
    return 1 if 3 * q * q > p * p else -1


def sumz(t):
    r = Z
    for x in t:
        r = add(r, x)
    return r


def div(x, y):
    n = y[0] * y[0] - 3 * y[1] * y[1]
    z = mul(x, (y[0], -y[1]))
    return (z[0] / n, z[1] / n)


S3 = math.sqrt(3)
def fl(x): return float(x[0]) + float(x[1]) * S3


# ---------------------------------------------------------------- catalog
def qm(x, y):
    a, b, c, d = x
    e, f, g, h = y
    return (sumz([mul(a, e), neg(mul(b, f)), neg(mul(c, g)), neg(mul(d, h))]),
            sumz([mul(a, f), mul(b, e), mul(c, h), neg(mul(d, g))]),
            sumz([mul(a, g), neg(mul(b, h)), mul(c, e), mul(d, f)]),
            sumz([mul(a, h), mul(b, g), neg(mul(c, f)), mul(d, e)]))


def canon(q):
    for c in q:
        s = sgn(c)
        if s:
            return q if s > 0 else tuple(neg(t) for t in q)
    return q


def closure(limit=4096):
    co = (F(0), F(1, 2))       # sqrt3 / 2
    sn = (F(1, 2), F(0))
    nsn = (F(-1, 2), F(0))
    gens = [(co, sn, Z, Z), (co, nsn, Z, Z), (co, Z, Z, nsn), (co, Z, Z, sn)]
    cat = [((F(1), F(0)), Z, Z, Z)]
    dep = [0]
    seen = {canon(cat[0])}
    trunc = False
    for g in gens:
        if canon(g) not in seen:
            seen.add(canon(g))
            cat.append(g)
            dep.append(0)
    nr, rd = 1, 1
    while True:
        prev = len(cat)
        for i in range(prev):
            for g in gens:
                p = qm(cat[i], g)
                c = canon(p)
                if c in seen:
                    continue
                if len(cat) >= limit:
                    trunc = True
                    continue
                seen.add(c)
                cat.append(p)
                dep.append(rd)
        if len(cat) > prev:
            nr += 1
        rd += 1
        if not (len(cat) > prev and nr < 16):
            break
    return cat, dep, trunc


def dirkey(v):
    for c in v:
        if c != Z:
            n = c[0] * c[0] - 3 * c[1] * c[1]
            cj = (c[0], -c[1])
            return tuple((lambda y: (y[0] / n, y[1] / n))(mul(x, cj)) for x in v)
    return None


def axes_of(cat, idx, cap):
    ax, seen = [], set()
    for i in idx:
        v = cat[i][1:]
        k = dirkey(v)
        if k is None or k in seen:
            continue
        if cap and len(ax) >= cap:
            continue
        seen.add(k)
        ax.append(v)
    return ax


# ---------------------------------------------------------------- sectors
getcontext().prec = 60
R3 = Decimal(3).sqrt()


def dec(x):
    return (Decimal(x[0].numerator) / Decimal(x[0].denominator)
            + Decimal(x[1].numerator) / Decimal(x[1].denominator) * R3)


def machin():
    getcontext().prec += 10

    def at(n):
        n = Decimal(n)
        x = 1 / n
        s = x
        t = x
        k = 1
        while True:
            t = -t / (n * n)
            k += 2
            term = t / k
            if abs(term) < Decimal(10) ** -75:
                break
            s += term
        return s
    p = 4 * (4 * at(5) - at(239))
    getcontext().prec -= 10
    return +p


PI = machin()


def tan2(x):
    getcontext().prec += 10
    c = Decimal(1)
    t = Decimal(1)
    i = 0
    while True:
        i += 2
        t = -t * x * x / (i * (i - 1))
        c += t
        if abs(t) < Decimal(10) ** -75:
            break
    s = x
    t = x
    i = 1
    while True:
        i += 2
        t = -t * x * x / (i * (i - 1))
        s += t
        if abs(t) < Decimal(10) ** -75:
            break
    getcontext().prec -= 10
    return +((s / c) ** 2)


# tan^2(m pi / k) exactly when it lies in Q(sqrt3)
EXACT = {F(1, 12): (F(7), F(-4)), F(1, 6): (F(1, 3), F(0)), F(1, 4): (F(1), F(0)),
         F(1, 3): (F(3), F(0)), F(5, 12): (F(7), F(4))}
BOUND = {}


def cmp_bound(T, m, k):
    key = (m, k)
    if key not in BOUND:
        r = F(m, k)
        BOUND[key] = ('x', EXACT[r]) if r in EXACT else ('d', tan2(Decimal(m) * PI / Decimal(k)))
    kind, b = BOUND[key]
    if kind == 'x':
        return sgn(sub(T, b))
    d = dec(T) - b
    assert abs(d) > Decimal(10) ** -40, 'irrational boundary too close'
    return 1 if d > 0 else -1


# ---------------------------------------------------------------- cells
class Table:
    def __init__(self, cat, idx, cap):
        self.cat = cat
        self.ax = axes_of(cat, idx, cap)
        self.nd = len(self.ax)
        U = np.array([[fl(c) for c in v] for v in self.ax])
        self.Un = U / np.linalg.norm(U, axis=1)[:, None]
        self.n2 = [sumz([mul(c, c) for c in v]) for v in self.ax]
        self.cache = {}

    def point(self, s):
        key = canon(s)
        if key in self.cache:
            return key, self.cache[key]
        a, v = s[0], s[1:]
        if all(c == Z for c in v):
            info = ('zero' if a == Z else 'real', None, None)
        else:
            vf = np.array([fl(c) for c in v])
            sc = (self.Un @ vf) ** 2
            mx = sc.max()
            cand = [j for j in range(self.nd) if sc[j] >= mx * (1 - 1e-6)]
            best, bn, bd = [], None, None
            for j in cand:
                d = sumz([mul(x, y) for x, y in zip(v, self.ax[j])])
                num, den = mul(d, d), self.n2[j]
                if bn is None:
                    best, bn, bd = [j], num, den
                    continue
                c = sgn(sub(mul(num, bd), mul(bn, den)))
                if c > 0:
                    best, bn, bd = [j], num, den
                elif c == 0:
                    best.append(j)
            if a == Z:
                info = ('null', best, None)
            else:
                V2 = sumz([mul(c, c) for c in v])
                info = ('gen', best, div(V2, mul(a, a)))
        self.cache[key] = info
        return key, info

    def cells(self, info, k):
        """rule cell and the tie set (all combinations)"""
        kind, axes, T = info
        nv = self.nd + 1
        if kind == 'zero':
            c = (k - 1) * nv + self.nd
            return c, [c]
        if kind == 'real':
            return self.nd, [self.nd]
        if kind == 'null':
            secs = [k // 2] + ([k // 2 - 1] if k % 2 == 0 else [])
        else:
            sec, tie = 0, False
            for m in range(1, (k - 1) // 2 + 1):
                cc = cmp_bound(T, m, k)
                if cc < 0:
                    break
                sec, tie = m, cc == 0
            secs = [sec] + ([sec - 1] if tie else [])
        return secs[0] * nv + axes[0], [x * nv + y for x in secs for y in axes]


def points(tab, W):
    """distinct exact points of the 3^n sums (folded by +-), with parity;
    None if one point carries both parities"""
    cat = tab.cat
    pts = {}
    for cvec in itertools.product((-1, 0, 1), repeat=len(W)):
        nz = [c for c in cvec if c]
        if nz and nz[0] < 0:
            continue
        s = (Z, Z, Z, Z)
        for i, c in zip(W, cvec):
            if c:
                q = cat[i]
                s = tuple(add(s[j], q[j]) if c > 0 else sub(s[j], q[j]) for j in range(4))
        key, info = tab.point(s)
        p = len(nz) & 1
        if pts.setdefault(key, (p, info))[0] != p:
            return None
    return pts


def separates(tab, pts, k, robust):
    if pts is None:
        return False
    own = {}
    for p, info in pts.values():
        rule, ties = tab.cells(info, k)
        for c in (ties if robust else [rule]):
            if own.setdefault(c, p) != p:
                return False
    return True


LADDER = [1, 6, 8, 10, 12, 16, 20, 24]


def check_conf(job):
    name, mode, table, idx, winners = job
    cat = CAT
    cap = 512 if table == 'capped' else 0
    tab = Table(cat, idx, cap)
    robust = mode == 'robust'
    bad = []
    for n, k, w in winners:
        W = [idx[i] for i in w]
        pts = points(tab, W)
        ok = separates(tab, pts, k, robust)
        for kk in LADDER:
            if kk >= k:
                break
            if cap and kk * (tab.nd + 1) > 16384:
                continue
            if separates(tab, pts, kk, robust):
                ok = False
        if not ok:
            bad.append((n, k, w))
    return name, mode, table, tab.nd, len(winners), bad


CAT = None


def init(cat):
    global CAT
    CAT = cat


def main():
    path = sys.argv[1]
    jobs_n = int(sys.argv[sys.argv.index('-j') + 1]) if '-j' in sys.argv else 1
    t0 = time.time()
    cat, dep, trunc = closure()
    census = sorted(Counter(dep).items())
    nulls = sum(1 for q in cat if q[0] == Z)
    print('catalog: %d entries, truncated %s, census %s, nulls %d (%.0f s)'
          % (len(cat), trunc, [c for _, c in census], nulls, time.time() - t0))
    ok_census = (len(cat) == 4096 and trunc and nulls == 121
                 and [c for _, c in census] == [5, 12, 34, 76, 148, 289, 576, 1152, 1804])
    nonnull = [i for i in range(4096) if cat[i][0] != Z]
    for name, idx in (('full', list(range(4096))), ('non-null', nonnull),
                      ('strided 564', [i * 4096 // 564 for i in range(564)]),
                      ('deep 564', list(range(4096 - 564, 4096)))):
        print('  axes %-12s %d' % (name, len(axes_of(cat, idx, 0))))
    jobs = []
    cur = None
    for line in open(path):
        f = line.rstrip('\n').split('\t')
        if f[0] == 'C':
            cur = [f[1], f[2], f[3], [int(x) for x in f[5].split()], []]
            jobs.append(cur)
        elif f[0] == 'W':
            cur[4].append((int(f[1]), int(f[2]), [int(x) for x in f[3].split()]))
    jobs = [j for j in jobs if j[4]]
    total = sum(len(j[4]) for j in jobs)
    print('re-deciding %d winners in %d searches (-j %d)' % (total, len(jobs), jobs_n))
    with Pool(jobs_n, initializer=init, initargs=(cat,)) as pool:
        results = pool.map(check_conf, jobs, chunksize=1)
    n_bad = 0
    for name, mode, table, nd, nw, bad in results:
        n_bad += len(bad)
        print('  %-22s %-6s %-8s axes %4d  winners %3d  wrong %d'
              % (name, mode, table, nd, nw, len(bad)))
        for b in bad[:5]:
            print('    WRONG', b)
    print('census %s; winners re-decided %d, wrong %d (%.0f s)'
          % ('OK' if ok_census else 'WRONG', total, n_bad, time.time() - t0))
    sys.exit(0 if ok_census and n_bad == 0 else 1)


if __name__ == '__main__':
    main()

#!/usr/bin/env python3
"""Demo 121 oracle: zeta_8 SUBSETS by cell, exact, independent of main.c.

Arithmetic and verdicts from knotapel/demo_119_exact_capacity/oracle_2i.py
(the D119 review agent's oracle): Q(sqrt2, sqrt3, sqrt5) as dicts, zero
exact, signs by 110-digit Decimal, every sector boundary compared (no
float filter), 'possible' per distinct sum vector by backtracking.

Cells are defined from first principles, not from main.c's derived
series: E identity; B |Re| = sqrt2/2 (45 degrees); C |Re| = 1/2 (60
degrees); D = Q8 (+-i, +-j, +-k); A = the other Re = 0 elements. The
subset keeps zeta_8's BFS order and builds its own axes (D95/D96
build_dirs: first occurrence).

Usage: python3 -I oracle.py CELLS N     e.g.  oracle.py AD 4   (XOR only)
"""
import sys, itertools
from fractions import Fraction as F
from decimal import Decimal, getcontext
getcontext().prec = 110
RT = {2: Decimal(2).sqrt(), 3: Decimal(3).sqrt(), 5: Decimal(5).sqrt()}
PR = (2, 3, 5)

def mq(c=0): return {(0, 0, 0): F(c)} if c else {}
def add(x, y):
    r = dict(x)
    for k, v in y.items():
        r[k] = r.get(k, F(0)) + v
        if r[k] == 0: del r[k]
    return r
def neg(x): return {k: -v for k, v in x.items()}
def sub(x, y): return add(x, neg(y))
def mul(x, y):
    r = {}
    for ka, u in x.items():
        for kb, v in y.items():
            c = u * v; e = []
            for i, p in enumerate(PR):
                s = ka[i] + kb[i]
                if s == 2: c *= p; s = 0
                e.append(s)
            e = tuple(e); r[e] = r.get(e, F(0)) + c
    return {k: v for k, v in r.items() if v != 0}
def dec(x):
    s = Decimal(0)
    for k, v in x.items():
        t = Decimal(v.numerator) / Decimal(v.denominator)
        for i, p in enumerate(PR):
            if k[i]: t *= RT[p]
        s += t
    return s
def sign(x):
    if not x: return 0
    d = dec(x)
    assert abs(d) > Decimal(10) ** -80
    return 1 if d > 0 else -1

cellulae, N = sys.argv[1], int(sys.argv[2])
which = 'z8'

# ---- catalog ----
def qmul(p, q):
    a1, b1, c1, d1 = p; a2, b2, c2, d2 = q
    return (sub(sub(sub(mul(a1, a2), mul(b1, b2)), mul(c1, c2)), mul(d1, d2)),
            sub(add(add(mul(a1, b2), mul(b1, a2)), mul(c1, d2)), mul(d1, c2)),
            add(add(sub(mul(a1, c2), mul(b1, d2)), mul(c1, a2)), mul(d1, b2)),
            add(sub(add(mul(a1, d2), mul(b1, c2)), mul(c1, b2)), mul(d1, a2)))
def qneg(q): return tuple(neg(x) for x in q)
def key(q): return tuple(tuple(sorted(x.items())) for x in q)
def conj(q): return (q[0], neg(q[1]), neg(q[2]), neg(q[3]))

if which == 'z8':
    h = {(1, 0, 0): F(1, 2)}
    s1 = (h, h, {}, {}); s2 = (h, {}, {}, neg(h))
    gens = [s1, conj(s1), s2, conj(s2)]
else:
    half = mq(F(1, 2)); q4 = lambda x, y: add(mq(F(x, 4)), {(0, 0, 1): F(y, 4)}) if y else mq(F(x, 4))
    s = (half, half, half, half)
    t = (q4(1, 1), q4(-1, 1), half, {})
    gens = [s, conj(s), t, conj(t)]
one = (mq(1), {}, {}, {})
cat = [one]; seen = {key(one), key(qneg(one))}
for g in gens:
    if key(g) not in seen: cat.append(g); seen |= {key(g), key(qneg(g))}
while True:
    prev = len(cat)
    for i in range(prev):
        for g in gens:
            p = qmul(cat[i], g)
            if key(p) not in seen: cat.append(p); seen |= {key(p), key(qneg(p))}
    if len(cat) == prev: break
def cell_of(q):
    a, b, c, d = q
    h = {(1, 0, 0): F(1, 2)}
    if a == mq(1) or a == mq(-1):
        return 'E'
    if a == h or a == neg(h):
        return 'B'
    if a == mq(F(1, 2)) or a == mq(F(-1, 2)):
        return 'C'
    if not a:
        units = [x for x in (b, c, d) if x]
        if len(units) == 1 and (units[0] == mq(1) or units[0] == mq(-1)):
            return 'D'
        return 'A'
    raise SystemExit('cellula ignota')
counts = {}
for q in cat:
    counts[cell_of(q)] = counts.get(cell_of(q), 0) + 1
print('cells', sorted(counts.items()), file=sys.stderr)
assert counts == {'A': 6, 'B': 6, 'C': 8, 'D': 3, 'E': 1}, counts
cat = [q for q in cat if cell_of(q) in cellulae]
n_cat = len(cat)
print("catalog", n_cat, file=sys.stderr)

# directions: D94 order (first occurrence), parallel by cross product
def cross_zero(u, v):
    return (not sub(mul(u[2], v[3]), mul(u[3], v[2])) and not sub(mul(u[3], v[1]), mul(u[1], v[3]))
            and not sub(mul(u[1], v[2]), mul(u[2], v[1])))
dirs = []
for q in cat:
    if not q[1] and not q[2] and not q[3]: continue
    if not any(cross_zero(q, cat[j]) for j in dirs): dirs.append(cat.index(q))
ND = len(dirs); NV = ND + 1
print("dirs", ND, file=sys.stderr)
def vdot(p, q): return add(add(mul(p[1], q[1]), mul(p[2], q[2])), mul(p[3], q[3]))
UU = [vdot(cat[j], cat[j]) for j in dirs]

# cos^2(m pi/k) = (1 + cos(2 m pi/k))/2 ; cos(j pi/12) exact in Q(sqrt2, sqrt3)
C12 = {0: mq(1), 1: {(1, 1, 0): F(1, 4), (1, 0, 0): F(1, 4)}, 2: {(0, 1, 0): F(1, 2)}, 3: {(1, 0, 0): F(1, 2)},
       4: mq(F(1, 2)), 5: {(1, 1, 0): F(1, 4), (1, 0, 0): F(-1, 4)}, 6: {}}
def cos12(j):
    j %= 24
    if j > 12: j = 24 - j
    return C12[j] if j <= 6 else neg(C12[12 - j])
def cmpb(a, nn, m, k):
    sb = (k > 2 * m) - (k < 2 * m); sa = sign(a)
    if sb == 0: return sa
    if sa == 0: return -sb
    if sa != sb: return sa
    c2 = mul(add(mq(1), cos12(24 * m // k)), mq(F(1, 2)))
    return sa * sign(sub(mul(a, a), mul(c2, nn)))

def cells(S, k):
    a, b, c, d = S
    nn = add(add(mul(a, a), mul(b, b)), add(mul(c, c), mul(d, d)))
    if not nn:
        z = (k - 1) * NV + ND; return z, [z]
    rule = 0; tie = None
    for m in range(1, k):
        s = cmpb(a, nn, m, k)
        if s <= 0: rule = m
        if s == 0: tie = m
    secs = [tie - 1, tie] if tie else [rule]
    if not b and not c and not d:
        ds = [ND]
    else:
        vals = []
        for j, ci in enumerate(dirs):
            dv = vdot(S, cat[ci]); vals.append((mul(dv, dv), UU[j]))
        ds = [0]
        for j in range(1, ND):
            sgn = sign(sub(mul(vals[j][0], vals[ds[0]][1]), mul(vals[ds[0]][0], vals[j][1])))
            if sgn > 0: ds = [j]
            elif sgn == 0: ds.append(j)
    return rule * NV + min(ds), [s_ * NV + x for s_ in secs for x in ds]

def possible(items):
    """some choice of one tied cell per distinct vector with no cell under
    both truth values. Search: a vector that can reuse a cell already
    carrying its own label is satisfied at no cost (it adds no constraint,
    so taking it never hurts); otherwise branch on the most constrained
    vector (fewest viable cells), failing as soon as one has none."""
    lab = {}

    def go(remaining):
        while True:
            free = None
            best = None
            for i in remaining:
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
            remaining = remaining - {free}
        if not remaining:
            return True
        i, viable = best
        t = items[i][1]
        rest = remaining - {i}
        for c in viable:
            lab[c] = t
            if go(rest):
                return True
            del lab[c]
        return False

    return go(frozenset(range(len(items))))

def tt_of(fn, mask, n):
    pc = bin(mask).count('1')
    return [pc % 2, int(mask == (1 << n) - 1), int(pc > n // 2)][fn]

def run(idx_list):
    res = [[0, 0, 0] for _ in range(3)]
    cache = {}
    for idx in idx_list:
        sums = []
        for mask in range(1 << N):
            S = ({}, {}, {}, {})
            for i, j in enumerate(idx):
                S = tuple(add(x, y) if (mask >> i) & 1 else sub(x, y) for x, y in zip(S, cat[j]))
            sums.append(S)
        cl = {}
        for k in (6, 12, 24):
            for mask, S in enumerate(sums):
                kk = (key(S), k)
                if kk not in cache: cache[kk] = cells(S, k)
                cl[(mask, k)] = cache[kk]
        for fn in range(3):
            rule_any = rob_any = pos_any = False
            byvec = {}
            for mask, S in enumerate(sums): byvec.setdefault(key(S), set()).add(tt_of(fn, mask, N))
            clash = any(len(v) == 2 for v in byvec.values())
            for k in (6, 12, 24):
                own = {}; ok = True
                for mask in range(1 << N):
                    c = cl[(mask, k)][0]; t = tt_of(fn, mask, N)
                    if own.setdefault(c, t) != t: ok = False; break
                rule_any |= ok
                reach = {}
                for mask in range(1 << N):
                    for c in cl[(mask, k)][1]: reach.setdefault(c, set()).add(tt_of(fn, mask, N))
                rob_any |= all(len(v) < 2 for v in reach.values())
                if not clash and not pos_any:
                    items = {}
                    for mask, S in enumerate(sums): items[key(S)] = (cl[(mask, k)][1], tt_of(fn, mask, N))
                    items = list(items.values())
                    pos_any |= possible(items)
            res[fn][0] += rule_any; res[fn][1] += rob_any; res[fn][2] += pos_any
    return res

idxs = list(itertools.combinations(range(n_cat), N))
res = run(idxs)
for fn, name in [(0, 'XOR')]:
    print(cellulae, N, name, 'sets', len(idxs), 'rule', res[fn][0], 'robust', res[fn][1], 'possible', res[fn][2], flush=True)

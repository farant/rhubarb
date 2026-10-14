#!/usr/bin/env python3
"""Independent oracle for Demo 126 (relation profiles of 2T, 2O, 2I).

No house code. Python Fractions only:
- its own arithmetic in Q(sqrt d) (pairs (a, b) = a + b sqrt d) and its
  own quaternion product;
- the groups by closure from the same generators as main.c (D66's for
  2O over Q(sqrt 2), D94's for 2I over Q(sqrt 5)); 2T = the rational
  elements of 2O;
- points modulo sign (first nonzero coordinate positive, exact sign);
- ranks by Gaussian elimination over Q on the 8 x N coordinates;
- signed coincidences (relations with entries in {-1, 0, 1}) by support
  size, by meet-in-the-middle over signed subset sums: no lattice, no
  LLL, no enumeration tree. Each zero-sum signed set is counted once
  modulo overall sign;
- for 2T, ALL relations (any integer entries) up to norm 10, by splitting
  the 12 points into two halves of 6 with entries in [-3, 3].

Run: python3 -I oracle.py  (about a minute)
"""
from fractions import Fraction as F
from itertools import combinations, product
import sys


# ---------------- Q(sqrt d) and quaternions ----------------

def fmul(x, y, d):
    return (x[0] * y[0] + d * x[1] * y[1], x[0] * y[1] + x[1] * y[0])


def fadd(x, y):
    return (x[0] + y[0], x[1] + y[1])


def fneg(x):
    return (-x[0], -x[1])


def fsign(x, d):
    """exact sign of a + b sqrt d (d > 0, not a square)"""
    a, b = x
    if a == 0 and b == 0:
        return 0
    if a >= 0 and b >= 0:
        return 1
    if a <= 0 and b <= 0:
        return -1
    # opposite signs: compare a^2 with d b^2
    if a > 0:
        return 1 if a * a > d * b * b else -1
    return 1 if d * b * b > a * a else -1


def qmul(p, q, d):
    a1, b1, c1, d1 = p
    a2, b2, c2, d2 = q

    def m(x, y):
        return fmul(x, y, d)

    def s(*terms):
        r = (F(0), F(0))
        for sign, t in terms:
            r = fadd(r, t if sign > 0 else fneg(t))
        return r
    return (s((1, m(a1, a2)), (-1, m(b1, b2)), (-1, m(c1, c2)), (-1, m(d1, d2))),
            s((1, m(a1, b2)), (1, m(b1, a2)), (1, m(c1, d2)), (-1, m(d1, c2))),
            s((1, m(a1, c2)), (-1, m(b1, d2)), (1, m(c1, a2)), (1, m(d1, b2))),
            s((1, m(a1, d2)), (1, m(b1, c2)), (-1, m(c1, b2)), (1, m(d1, a2))))


def closure(gens, d):
    one = ((F(1), F(0)), (F(0), F(0)), (F(0), F(0)), (F(0), F(0)))
    els = [one]
    seen = {one}
    i = 0
    while i < len(els):
        for g in gens:
            x = qmul(els[i], g, d)
            if x not in seen:
                seen.add(x)
                els.append(x)
        i += 1
        if len(els) > 200:
            raise SystemExit("closure too large")
    return els


def canonical(q, d):
    for part in q:
        s = fsign(part, d)
        if s != 0:
            return q if s > 0 else tuple(fneg(x) for x in q)
    return q


def mod_sign(els, d):
    return sorted({canonical(q, d) for q in els})


def coords(q, scale):
    """8 integer coordinates (a, b per part), multiplied by scale"""
    out = []
    for a, b in q:
        for x in (a, b):
            y = x * scale
            assert y.denominator == 1
            out.append(int(y))
    return tuple(out)


# ---------------- linear algebra ----------------

def rank(columns):
    rows = [list(map(F, r)) for r in zip(*columns)]
    r = 0
    ncols = len(columns)
    for c in range(ncols):
        piv = next((i for i in range(r, len(rows)) if rows[i][c] != 0), None)
        if piv is None:
            continue
        rows[r], rows[piv] = rows[piv], rows[r]
        for i in range(len(rows)):
            if i != r and rows[i][c] != 0:
                f = rows[i][c] / rows[r][c]
                rows[i] = [x - f * y for x, y in zip(rows[i], rows[r])]
        r += 1
    return r


# ---------------- signed coincidences ----------------

def signed_sums(pts, k):
    """dict: sum vector -> list of signed k-subsets ((i, s), ...)"""
    out = {}
    dim = len(pts[0])
    for idx in combinations(range(len(pts)), k):
        for signs in product((1, -1), repeat=k):
            v = tuple(sum(s * pts[i][t] for i, s in zip(idx, signs))
                      for t in range(dim))
            out.setdefault(v, []).append(tuple(zip(idx, signs)))
    return out


def key(signed):
    """canonical key of a signed set modulo overall sign"""
    items = sorted(signed)
    if items[0][1] < 0:
        items = [(i, -s) for i, s in items]
    return tuple(items)


def coincidences(pts, support, cache):
    """number of zero-sum signed sets of the given support, mod sign"""
    k = support // 2
    rest = support - k
    for size in (k, rest):
        if size not in cache:
            cache[size] = signed_sums(pts, size)
    found = set()
    left = cache[k]
    right = cache[rest]
    for v, lst in left.items():
        neg = tuple(-x for x in v)
        other = right.get(neg)
        if not other:
            continue
        for a in lst:
            ia = {i for i, _ in a}
            for b in other:
                if ia.isdisjoint(i for i, _ in b):
                    found.add(key(a + b))
    return len(found)


def all_relations_2t(pts, radius):
    """every relation (integer entries) of norm <= radius, mod sign, by
    halves of 6 with entries in [-3, 3] (norm <= 10 forces |x_i| <= 3)"""
    n = len(pts)
    h = n // 2
    dim = len(pts[0])

    def half(lo, hi):
        out = {}
        for xs in product(range(-3, 4), repeat=hi - lo):
            nrm = sum(x * x for x in xs)
            if nrm > radius:
                continue
            v = tuple(sum(x * pts[lo + j][t] for j, x in enumerate(xs))
                      for t in range(dim))
            out.setdefault(v, []).append((xs, nrm))
        return out
    left = half(0, h)
    right = half(h, n)
    by_norm = {}
    for v, lst in left.items():
        neg = tuple(-x for x in v)
        for xs, n1 in lst:
            for ys, n2 in right.get(neg, []):
                tot = n1 + n2
                if 0 < tot <= radius:
                    by_norm[tot] = by_norm.get(tot, 0) + 1
    return {k: c // 2 for k, c in sorted(by_norm.items())}   # mod sign


# ---------------- catalogs ----------------

def q(*parts):
    return tuple((F(a), F(b)) for a, b in parts)


def main():
    h = F(1, 2)
    # D66: (1 + i)/sqrt2 = sqrt2/2 (1 + i), etc. (b = coefficient of sqrt2)
    gens_2o = [q((0, h), (0, h), (0, 0), (0, 0)),
               q((0, h), (0, -h), (0, 0), (0, 0)),
               q((0, h), (0, 0), (0, 0), (0, -h)),
               q((0, h), (0, 0), (0, 0), (0, h))]
    g48 = closure(gens_2o, 2)
    assert len(g48) == 48, len(g48)
    # D94: s = (1 + i + j + k)/2, t = (phi + phi^-1 i + j)/2, phi = (1 + sqrt5)/2
    f = F(1, 4)
    gens_2i = [q((h, 0), (h, 0), (h, 0), (h, 0)),
               q((f, f), (-f, f), (h, 0), (0, 0))]
    g120 = closure(gens_2i, 5)
    assert len(g120) == 120, len(g120)
    g24 = [x for x in g48 if all(b == 0 for _, b in x)]
    assert len(g24) == 24

    cats = [("2T", mod_sign(g24, 2), 2, 2, 10),
            ("2O", mod_sign(g48, 2), 2, 2, 8),
            ("2I", mod_sign(g120, 5), 5, 4, 6)]
    for name, pts_q, d, scale, max_support in cats:
        pts = [coords(x, scale) for x in pts_q]
        rk = rank(pts)
        print(f"{name}: N {len(pts)}, ambient rank {rk}, "
              f"relation rank {len(pts) - rk}")
        cache = {}
        for s in range(2, max_support + 1):
            if name != "2T" and s > max_support:
                break
            c = coincidences(pts, s, cache)
            print(f"  signed coincidences, support {s}: {c}")
            sys.stdout.flush()
        if name == "2T":
            print("  all relations to norm 10 (halves of 6, entries in "
                  "[-3, 3]):")
            for nrm, c in all_relations_2t(pts, 10).items():
                print(f"    norm {nrm}: {c}")


if __name__ == "__main__":
    main()

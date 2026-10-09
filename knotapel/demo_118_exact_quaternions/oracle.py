#!/usr/bin/env python3
"""Demo 118 Part C oracle - independent of the house libraries.

Exact arithmetic in Q(sqrt 2) as pairs (a, b) = a + b*sqrt(2) of
Fractions; signs decided exactly (a + b sqrt2 > 0 by comparing a^2 with
2 b^2). Rebuilds 2O from D66's generators, replays D66's word order to
get its 24-entry catalog, and recounts XOR6 under the 24-cell Voronoi
activation on all antipodal triples with three tie rules:

  first     ties go to the lowest catalog index
  robust    passes under every tie resolution
  possible  passes under some tie resolution (backtracking over the
            per-sum choices - a different algorithm from main.c's
            cell-labeling search); a tie rule is a function of the
            POINT: one sum vector reached with both parities fails

and checks the characterization independently: possible <=> the three
classes are mutually orthogonal; robust <=> they lie in one coset of
Q8 = {+-1, +-i, +-j, +-k}; a non-orthogonal set has a pair sum tied
between its own two members.

Usage: python3 oracle.py   (prints the counts; exit 0)
"""
from fractions import Fraction as F
from itertools import combinations, product


def add(x, y):
    return (x[0] + y[0], x[1] + y[1])


def sub(x, y):
    return (x[0] - y[0], x[1] - y[1])


def mul(x, y):
    return (x[0] * y[0] + 2 * x[1] * y[1], x[0] * y[1] + x[1] * y[0])


def sign(x):
    a, b = x
    if a == 0 and b == 0:
        return 0
    if a >= 0 and b >= 0:
        return 1
    if a <= 0 and b <= 0:
        return -1
    # opposite signs: compare a^2 with 2 b^2
    d = a * a - 2 * b * b
    return (1 if a > 0 else -1) if d > 0 else (1 if b > 0 else -1)


ZERO = (F(0), F(0))


def qmul(p, q):
    a1, b1, c1, d1 = p
    a2, b2, c2, d2 = q
    return (
        sub(sub(sub(mul(a1, a2), mul(b1, b2)), mul(c1, c2)), mul(d1, d2)),
        sub(add(add(mul(a1, b2), mul(b1, a2)), mul(c1, d2)), mul(d1, c2)),
        add(add(sub(mul(a1, c2), mul(b1, d2)), mul(c1, a2)), mul(d1, b2)),
        add(sub(add(mul(a1, d2), mul(b1, c2)), mul(c1, b2)), mul(d1, a2)),
    )


def qconj(q):
    return (q[0], sub(ZERO, q[1]), sub(ZERO, q[2]), sub(ZERO, q[3]))


def qneg(q):
    return tuple(sub(ZERO, x) for x in q)


def dot(p, q):
    s = ZERO
    for x, y in zip(p, q):
        s = add(s, mul(x, y))
    return s


H = (F(0), F(1, 2))          # sqrt(2)/2
ONE = (F(1), F(0))
s1 = (H, H, ZERO, ZERO)
s2 = (H, ZERO, ZERO, sub(ZERO, H))
gens = {1: s1, 2: s2, -1: qconj(s1), -2: qconj(s2)}

# D66 enumeration order
catalog = []
reached = set()
for n in (2, 3):
    max_gen = n - 1
    tg = 2 * max_gen
    for length in range(1, 9):
        total = tg ** length
        if total > 100000:
            continue
        for idx in range(total):
            tmp = idx
            r = (ONE, ZERO, ZERO, ZERO)
            for _ in range(length):
                g = tmp % tg
                tmp //= tg
                w = g + 1 if g < max_gen else -(g - max_gen + 1)
                r = qmul(r, gens[w])
            reached.add(r)
            if r not in catalog and qneg(r) not in catalog:
                catalog.append(r)

assert len(reached) == 48, len(reached)
assert len(catalog) == 24, len(catalog)

gram = [[dot(p, q) for q in catalog] for p in catalog]


def vec_sum(xs, idx):
    v = (ZERO, ZERO, ZERO, ZERO)
    for x, j in zip(xs, idx):
        if x == 1:
            v = tuple(add(p, q) for p, q in zip(v, catalog[j]))
        elif x == -1:
            v = tuple(sub(p, q) for p, q in zip(v, catalog[j]))
    return v


def clash(a, b, c):
    seen = {}
    for xs in product((-1, 0, 1), repeat=3):
        parity = sum(1 for x in xs if x) % 2
        seen.setdefault(vec_sum(xs, (a, b, c)), set()).add(parity)
    return any(len(v) == 2 for v in seen.values())


def cells(a, b, c):
    out = []
    for xs in product((-1, 0, 1), repeat=3):
        parity = sum(1 for x in xs if x) % 2
        dots = []
        for m in range(24):
            d = ZERO
            for x, j in zip(xs, (a, b, c)):
                if x == 1:
                    d = add(d, gram[j][m])
                elif x == -1:
                    d = sub(d, gram[j][m])
            dots.append(d)
        if all(d == ZERO for d in dots):
            out.append((parity, [24], xs))
            continue
        sq = [mul(d, d) for d in dots]
        best = sq[0]
        for v in sq[1:]:
            if sign(sub(v, best)) > 0:
                best = v
        ties = [m for m in range(24) if sq[m] == best]
        out.append((parity, ties, xs))
    return out


def passes(choice, sums):
    seen = {}
    for (parity, _, _), cell in zip(sums, choice):
        seen.setdefault(cell, set()).add(parity)
        if len(seen[cell]) == 2:
            return False
    return True


def possible(sums):
    # backtracking over per-sum cell choices
    order = sorted(range(len(sums)), key=lambda t: len(sums[t][1]))
    owner = {}

    def go(k):
        if k == len(order):
            return True
        parity, ties, _ = sums[order[k]]
        for cell in ties:
            prev = owner.get(cell)
            if prev is not None and prev != parity:
                continue
            fresh = prev is None
            owner[cell] = parity
            if go(k + 1):
                return True
            if fresh:
                del owner[cell]
        return False

    return go(0)


# Q8 and its cosets, by left multiplication
q8 = [e for e in reached if sum(1 for x in e if x != ZERO) == 1]
assert len(q8) == 8


def cls(e):
    for i, c in enumerate(catalog):
        if e == c or qneg(e) == c:
            return i
    raise ValueError


coset = {}
for i, c in enumerate(catalog):
    if i not in coset:
        k = len(set(coset.values()))
        for h in q8:
            coset[cls(qmul(c, h))] = k
assert len(set(coset.values())) == 6

first = robust = poss = tied = 0
n_clash = bad_char = n_orth = n_pair_tie = 0
for a, b, c in combinations(range(24), 3):
    sums = cells(a, b, c)
    tied += any(len(t) > 1 for _, t, _ in sums)
    v_first = passes([t[0] for _, t, _ in sums], sums)
    # robust: no cell reachable from both parities
    reach = {}
    for parity, ties, _ in sums:
        for cell in ties:
            reach.setdefault(cell, set()).add(parity)
    v_robust = all(len(v) < 2 for v in reach.values())
    cl = clash(a, b, c)
    n_clash += cl
    v_poss = (not cl) and possible(sums)
    first += v_first
    robust += v_robust
    poss += v_poss
    orth = all(gram[x][y] == ZERO for x, y in ((a, b), (a, c), (b, c)))
    in_coset = coset[a] == coset[b] == coset[c]
    pair_tie = any(sum(1 for x in xs if x) == 2
                   and all(j in ties for x, j in zip(xs, (a, b, c)) if x)
                   for _, ties, xs in sums)
    n_orth += orth
    n_pair_tie += pair_tie
    bad_char += (v_poss != orth) + (v_robust != in_coset) + \
        (pair_tie == orth)

print("catalog 24, group 48")
print("first-index", first)
print("robust", robust)
print("possible", poss)
print("tied sets", tied)
print("vector clash sets", n_clash)
print("orthogonal", n_orth, "own-pair tie", n_pair_tie,
      "characterization violations", bad_char)

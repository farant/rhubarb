#!/usr/bin/env python3
"""Demo 123 oracle: integer degrees among Cell B's breakpoints.

Independent of the demo's arithmetic: no radices, no surdus, no S_N
polynomials. Breakpoint pairs (|n0|, r) are re-derived from Cell B's
six elements (+-45 degrees about three orthogonal axes; element e is
axis e // 2 with sign +1 for even e, -1 for odd), every subset of size
3..6, every sign mask. Values n0^2 tan^2(j pi/24) / r and tan^2(k deg)
are computed with 100-digit Decimal (pi and sin/cos from the decimal
module's documented recipes). Not exact: the printed minimum gap says
how far the placements are from being decided by rounding.

Output per k = 1..89: kind (0 between values, 1 on a value) and rank
(interval: number of distinct values below; point: index of the value),
then the closest approaches.

Run: python3 -I knotapel/demo_123_cell_b_integer_degrees/oracle.py
"""
from decimal import Decimal, getcontext
from itertools import combinations

getcontext().prec = 100
EQ = Decimal(10) ** -80


def pi():
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


def cos(x):
    getcontext().prec += 2
    i, lasts, s, fact, num, sign = 0, 0, 1, 1, 1, 1
    while s != lasts:
        lasts = s
        i += 2
        fact *= i * (i - 1)
        num *= x * x
        sign *= -1
        s += num / fact * sign
    getcontext().prec -= 2
    return +s


def sin(x):
    getcontext().prec += 2
    i, lasts, s, fact, num, sign = 1, 0, x, 1, x, 1
    while s != lasts:
        lasts = s
        i += 2
        fact *= i * (i - 1)
        num *= x * x
        sign *= -1
        s += num / fact * sign
    getcontext().prec -= 2
    return +s


PI = pi()


def tan2(x):
    c = cos(x)
    s = sin(x)
    return (s * s) / (c * c)


def pairs():
    seen = set()
    for n_w in range(3, 7):
        for combo in combinations(range(6), n_w):
            for mask in range(1 << n_w):
                n0 = 0
                nv = [0, 0, 0]
                for i, e in enumerate(combo):
                    sg = 1 if (mask >> i) & 1 else -1
                    n0 += sg
                    nv[e // 2] += sg * (1 if e % 2 == 0 else -1)
                r = sum(v * v for v in nv)
                if n0 != 0 and r > 0:
                    seen.add((abs(n0), r))
    return sorted(seen)


def main():
    pr = pairs()
    t24 = [tan2(PI * j / 24) for j in range(1, 12)]
    vals = sorted(Decimal(a * a) * t24[j - 1] / Decimal(r)
                  for a, r in pr for j in range(1, 12))
    distinct = []
    for v in vals:
        if distinct and abs(v - distinct[-1]) <= EQ * v:
            continue
        distinct.append(v)
    print("pairs", len(pr), "breakpoints", len(vals), "distinct",
          len(distinct))
    gaps = []
    for k in range(1, 90):
        u = tan2(PI * k / 180)
        on = [i for i, v in enumerate(distinct) if abs(u - v) <= EQ * v]
        if on:
            print("deg", k, "kind 1 rank", on[0])
            continue
        below = sum(1 for v in distinct if v < u)
        print("deg", k, "kind 0 rank", below)
        g = min(abs(u - v) / u for v in distinct)
        gaps.append((g, k))
    gaps.sort()
    for g, k in gaps[:5]:
        print("closest deg %d relative gap %.3e" % (k, g))


main()

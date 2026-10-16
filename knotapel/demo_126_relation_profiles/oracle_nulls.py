"""Null oracle for D126 (Part D). Written by the independent reviewer
(2026-10-13), adopted unchanged except for this header and the module
name. Replays the demo's 30 null sets (same LCG, same
draws, same closure order) and check "no relation of norm <= R" WITHOUT a
lattice: every integer vector of norm <= R in Z^N is classified by type
(entries <= 2; a +-2 entry forces 2 p_a = signed sum of k other unit
points), and each type is counted by meet in the middle / direct ball.
Run: python3 -I oracle_nulls.py   (about 85 s)
"""
import sys
import time
from fractions import Fraction as F
from collections import defaultdict
sys.path.insert(0, __file__.rsplit('/', 1)[0])
from oracle_common import (K, kq, close, mod_sign, int_coords, rank,
                       pm1_counts, signed_subsets, vneg)

h = F(1, 2)
q4 = F(1, 4)
k2 = K(2)
k5 = K(5)
s = (F(0), h)
ms = (F(0), -h)
g48 = close(k2, [kq(s, s, 0, 0), kq(s, ms, 0, 0), kq(s, 0, 0, ms),
                 kq(s, 0, 0, s)])
g120 = close(k5, [kq(h, h, h, h), kq((q4, q4), (-q4, q4), h, 0)])
members_2t = [i for i, x in enumerate(g48) if all(c[1] == 0 for c in x)]

state = [0]


def rng(mod):
    state[0] = (state[0] * 1103515245 + 12345) & 0x7fffffff
    return state[0] % mod


def rational_unit():
    n = [0, 0, 0]
    m = [0, 0, 0]
    for k in range(3):
        n[k] = rng(19) - 9
        m[k] = 1 + rng(9)
    M = m[0] * m[1] * m[2]
    u = [n[k] * (M // m[k]) for k in range(3)]
    ss = sum(x * x for x in u)
    den = ss + M * M
    return kq(F(ss - M * M, den), F(2 * u[0] * M, den),
              F(2 * u[1] * M, den), F(2 * u[2] * M, den))


def null_set(kf, n_pts, group, members, kind, seed):
    state[0] = seed
    pts = []
    redrawn = 0
    while len(pts) < n_pts:
        r = rational_unit()
        x = r
        if kind == 0:
            x = kf.qmul(r, group[members[rng(len(members))]])
        assert kf.qnorm(x) == (1, 0)
        x = kf.canon(x)
        if x in pts:
            redrawn += 1
            continue
        pts.append(x)
    return pts, redrawn


def ball_count(vs, R):
    """every x in Z^n, 0 < |x|^2 <= R, sum x_i v_i = 0 (with sign)"""
    n = len(vs)
    found = [0]

    def dfs(i, budget, sm, nz):
        if i == n:
            if nz and not any(sm):
                found[0] += 1
            return
        c = 0
        while c * c <= budget:
            for cc in ((0,) if c == 0 else (c, -c)):
                dfs(i + 1, budget - c * c,
                    tuple(a + cc * b for a, b in zip(sm, vs[i])),
                    nz or cc != 0)
            c += 1
    dfs(0, R, (0,) * len(vs[0]), False)
    return found[0]


def two_entry_count(vs, kmax):
    """x_a = +2 and k in 2..kmax entries +-1 elsewhere: 2 v_a = -sum"""
    total = 0
    for k in range(2, kmax + 1):
        tab = defaultdict(list)
        for comb, sg, sm in signed_subsets(vs, k):
            tab[sm].append(comb)
        for a in range(len(vs)):
            tgt = vneg(tuple(2 * c for c in vs[a]))
            total += sum(1 for comb in tab.get(tgt, []) if a not in comb)
    return total


cases = [("2T", k2, 12, g48, members_2t, 10, 0),
         ("2O", k2, 24, g48, list(range(48)), 8, 1),
         ("2I", k5, 60, g120, list(range(120)), 5, 2)]
t0 = time.time()
grand = 0
for name, kf, n_pts, group, members, R, i in cases:
    for kind in (0, 1):
        for seed in range(1, 6):
            sd = seed + 100 * i
            pts, red = null_set(kf, n_pts, group, members, kind, sd)
            vs, L = int_coords(pts)
            amb = rank(vs)
            t1 = time.time()
            if name == "2T":
                hits = ball_count(vs, R)          # whole ball, all types
                how = "ball"
            elif name == "2O":
                c1 = pm1_counts(vs, 8)            # 1^s, s <= 8
                c2 = two_entry_count(vs, 4)       # 2 1^k, k <= 4
                hits = sum(c1.values()) + c2
                how = f"pm1 {sum(c1.values())} + two {c2}"
            else:
                c1 = pm1_counts(vs, 5)            # norm <= 5: only 1^s
                hits = sum(c1.values())
                how = f"pm1 {hits}"
            grand += hits
            # height: largest coordinate denominator
            print(f"{name} {'ab'[kind]} seed {sd}: ambient {amb}, "
                  f"relation rank {n_pts - amb}, redrawn {red}, "
                  f"denominator lcm {L}, relations |x|^2<={R}: {hits} "
                  f"({how}, {time.time() - t1:.1f} s)", flush=True)
print("all nulls together:", grand, f"({time.time() - t0:.0f} s)")

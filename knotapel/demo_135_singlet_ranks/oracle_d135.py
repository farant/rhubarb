#!/usr/bin/env python3
"""Oracle for Demo 135: ranks of the singlet pieces, written apart from main.c.

Construction differs from main.c on purpose:
- the piece is built as a 4-D array A[x, y, u, v] over whole words, as a
  sum over subsets T of per-BIT products (E_j off T, S_j on T), from the
  indicator definitions, with numpy broadcasting (main.c uses word
  operations per entry);
- matrices use plain index order (rows: x * 2^lam + u, columns:
  y * 2^lam + v; columns cut: (x, y) against (u, v)) instead of main.c's
  interleaved pair layout - a row/column permutation, so ranks agree;
- rank mod p by vectorized row elimination (Fermat inverses).

Usage:
  python3 oracle_d135.py LAMBDA_MAX [--compare DEMO_OUTPUT]
prints 'RANK lam=.. cut=.. i=.. p=.. method=oracle rank=..' lines; with
--compare, checks every 'method=plain' RANK line of the demo output
against the oracle's rank for the same key and prints the mismatches.
"""
import itertools
import sys

import numpy as np

PRIMES = (2, 3, 5, 7, 2147483647)


def piece(lam, i):
    """A_i[x, y, u, v] as int64, from E_j = [x_j = u_j][y_j = v_j] and
    S_j = [x_j != y_j](-1)^x_j [u_j != v_j](-1)^u_j."""
    side = 1 << lam
    w = np.arange(side)
    x = w[:, None, None, None]
    y = w[None, :, None, None]
    u = w[None, None, :, None]
    v = w[None, None, None, :]
    bits = [((x >> j) & 1, (y >> j) & 1, (u >> j) & 1, (v >> j) & 1)
            for j in range(lam)]
    total = np.zeros((side,) * 4, dtype=np.int64)
    for subset in itertools.combinations(range(lam), i):
        term = np.ones((side,) * 4, dtype=np.int64)
        for j in range(lam):
            xj, yj, uj, vj = bits[j]
            if j in subset:
                fx = np.where(xj == 1, -1, 1)
                fu = np.where(uj == 1, -1, 1)
                factor = (xj != yj) * fx * (uj != vj) * fu
            else:
                factor = (xj == uj) * (yj == vj)
            term = term * factor
        total += term
    return total * (-1) ** i


def matrix_of(a, lam, cut):
    side = 1 << lam
    if cut == 'rows':   # (x, u) against (y, v)
        m = a.transpose(0, 2, 1, 3)
    else:               # (x, y) against (u, v)
        m = a
    return m.reshape(side * side, side * side)


def rank_mod(m, p):
    a = np.array(m % p, dtype=np.int64)
    n_rows, n_cols = a.shape
    r = 0
    for c in range(n_cols):
        if r == n_rows:
            break
        nz = np.nonzero(a[r:, c])[0]
        if len(nz) == 0:
            continue
        piv = r + int(nz[0])
        if piv != r:
            a[[r, piv]] = a[[piv, r]]
        inv = pow(int(a[r, c]), p - 2, p) if p > 2 else 1
        a[r, c:] = (a[r, c:] * inv) % p
        below = a[r + 1:, c]
        rows = np.nonzero(below)[0]
        if len(rows):
            f = below[rows][:, None]
            idx = r + 1 + rows
            a[idx, c:] = (a[idx, c:] - (f * a[r, c:][None, :]) % p) % p
        r += 1
    return r


def main():
    lam_max = int(sys.argv[1])
    compare = None
    if len(sys.argv) > 3 and sys.argv[2] == '--compare':
        compare = sys.argv[3]
    ranks = {}
    for lam in range(1, lam_max + 1):
        for i in range(lam + 1):
            a = piece(lam, i)
            for cut in ('rows', 'cols'):
                m = matrix_of(a, lam, cut)
                for p in PRIMES:
                    rk = rank_mod(m, p)
                    ranks[(lam, cut, i, p)] = rk
                    print('RANK lam=%d cut=%s i=%d p=%d method=oracle rank=%d'
                          % (lam, cut, i, p, rk), flush=True)
        # R1 in the oracle too: sum over i equals [x = v][y = u]
        side = 1 << lam
        w = np.arange(side)
        want = ((w[:, None, None, None] == w[None, None, None, :])
                & (w[None, :, None, None] == w[None, None, :, None]))
        got = sum(piece(lam, i) for i in range(lam + 1))
        print('R1 lam=%d %s' % (lam, 'ok' if np.array_equal(got, want.astype(np.int64))
                                else 'MISMATCH'), flush=True)
    if compare:
        seen = 0
        bad = 0
        with open(compare) as f:
            for line in f:
                line = line.strip()
                if not line.startswith('RANK ') or 'method=plain' not in line:
                    continue
                kv = dict(tok.split('=') for tok in line.split()[1:])
                key = (int(kv['lam']), kv['cut'], int(kv['i']), int(kv['p']))
                if key not in ranks:
                    continue
                seen += 1
                if ranks[key] != int(kv['rank']):
                    bad += 1
                    print('MISMATCH %s demo=%s oracle=%d' % (key, kv['rank'], ranks[key]))
        print('COMPARE: %d demo ranks checked against the oracle, %d mismatches'
              % (seen, bad))


if __name__ == '__main__':
    main()

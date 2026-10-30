#!/usr/bin/env python3
"""D134 oracle: umbrae, width, depth, competition and retrieval labels
and the garden-path share B, counted independently of main.c.

The world comes from D131's oracle_partb.py (Bennett's paradigms again,
analyses TREE FIRST, projectivity by interval yields, displaced arcs by
domination - none of it main.c's code). Written here: relations,
compatibility comp(r, x, y), umbrae from arc pairs (width by counting
spans over each boundary, depth by pairwise crossing - no left-to-right
sweep), competition, retrieval and their variants, the misread routes,
the strata, the subset key. Misread routes: a P analysis puts a
displaced word on a word as near as its head outside its own subtree,
inside it (a reversed dependency), only farther, or moves none.

Usage:
  python3 -I oracle_d134.py 5       every pair up to five words
  python3 -I oracle_d134.py 4       up to four words (main.c's quick mode)
  python3 -I oracle_d134.py 6 sub   six-word pairs of the subset: the L1
                                    bags (multisets of readings) of rank
                                    r with r % 128 == 0, in bag-code
                                    order; B at L0 and L1 only (PARTIAL)
Prints the totals in main.c's flatten() order as a C initializer, and a
few readable lines.
"""
import itertools
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.join(HERE, '..', 'demo_131_latin_sudoku'))
MAXW = int(sys.argv[1]) if len(sys.argv) > 1 else 5
SUB = len(sys.argv) > 2 and sys.argv[2] == 'sub'
K = 128
_argv = sys.argv
sys.argv = [_argv[0], str(MAXW)]
import oracle_partb as W  # noqa: E402  (the world)
sys.argv = _argv

LEVELS = range(4)
READ_IX = {r: i for i, r in enumerate(W.READINGS)}   # main.c's reading order
FORM = [{r: W.form(lv, r) for r in W.READINGS} for lv in LEVELS]
RDS = [W.readings_of(lv) for lv in LEVELS]           # form -> readings
CASE_REL = {'nom': 'nsubj', 'acc': 'obj', 'dat': 'iobj', 'abl': 'obl'}
REQUIRED = ('nsubj', 'obj', 'iobj')
RELS = ('nsubj', 'obj', 'iobj', 'obl', 'nmod', 'amod')
NSTRATA = 9


def relation(heads, rd, d):
    h = heads[d - 1]
    x = rd[d - 1]
    if h == 0:
        return 'root'
    if x[0] == 'A':
        return 'amod'
    if rd[h - 1][0] == 'N':
        return 'nmod'
    return CASE_REL[x[2]]


def satisfies(x, y, r):
    """reading x can depend on reading y by relation r"""
    if r == 'amod':
        return x[0] == 'A' and y[0] == 'N' and x[2:] == y[2:]
    if x[0] != 'N':
        return False
    if r == 'nmod':
        return x[2] == 'gen' and y[0] == 'N'
    if y[0] != 'V':
        return False
    number, ditransitive = W.VERBS[y[1]]
    if r == 'nsubj':
        return x[2] == 'nom' and x[4] == number
    if r == 'obj':
        return x[2] == 'acc'
    if r == 'iobj':
        return x[2] == 'dat' and ditransitive
    return x[2] == 'abl'


COMP = {}


def comp(lv, fx, fy):
    """the relations some reading of form fx and some of fy satisfy"""
    key = (lv, fx, fy)
    if key not in COMP:
        COMP[key] = frozenset(r for r in RELS for x in RDS[lv][fx]
                              for y in RDS[lv][fy] if satisfies(x, y, r))
    return COMP[key]


YIELDS = {}


def yields(heads):
    if heads not in YIELDS:
        n = len(heads)
        kids = {k: [] for k in range(n + 1)}
        for d, h in enumerate(heads, 1):
            kids[h].append(d)
        Y = {}

        def walk(k):
            s = {k}
            for c in kids[k]:
                s |= walk(c)
            Y[k] = frozenset(s)
            return s
        walk(0)
        YIELDS[heads] = Y
    return YIELDS[heads]


UMBRAE = {}


def umbrae(heads, rels):
    """(umbrae as (type, opened, closed, word), backward attachments,
    width, depths); width counts the spans over each boundary, depth
    counts the umbra arcs that open inside a span and outlive it"""
    key = (heads, rels)
    if key not in UMBRAE:
        n = len(heads)
        U = []
        back = 0
        for d, h in enumerate(heads, 1):
            if h == 0:
                U.append(('root', 0, d, d))
            elif d < h:
                U.append(('dangling', d, h, d))
            elif rels[d - 1] in REQUIRED:
                U.append(('slot', h, d, d))
            else:
                back += 1
        width = max(sum(1 for u in U if u[1] <= t < u[2]) for t in range(n))
        depth = [sum(1 for v in U if u[1] < v[1] < u[2] < v[2]) for u in U]
        UMBRAE[key] = (U, back, width, depth)
    return UMBRAE[key]


def labels(lv, heads, rd, rels, disp, U):
    """per displaced arc: (word, dependent first, competed, retrieved,
    competed by own relation, retrieved by own relation, blind competed,
    retrieved with cycle-closing candidates set aside)"""
    n = len(heads)
    f = [FORM[lv][r] for r in rd]
    Y = yields(heads)
    out = []
    for d in disp:
        h = heads[d - 1]
        r = rels[d - 1]
        dist = abs(h - d)
        comp_c = comp_r = blind = False
        for w in range(1, n + 1):
            if w == h or abs(w - d) > dist or w in Y[d]:
                continue
            cs = comp(lv, f[d - 1], f[w - 1])
            comp_c = comp_c or bool(cs)
            comp_r = comp_r or r in cs
            blind = blind or RDS[lv][f[w - 1]][0][0] == 'N'
        if d < h:
            cands = [u[3] for u in U if u[0] == 'dangling' and u[1] < h < u[2]]
            hits = [x for x in cands if comp(lv, f[x - 1], f[h - 1])]
            ret = not hits
            ret_r = not any(r in comp(lv, f[x - 1], f[h - 1]) for x in cands)
            ret_ns = not [x for x in hits if h not in Y[x]]
        else:
            ys = [y for y in range(1, d) if y != h]
            hits = [y for y in ys if comp(lv, f[d - 1], f[y - 1])]
            ret = not hits
            ret_r = not any(r in comp(lv, f[d - 1], f[y - 1]) for y in ys)
            ret_ns = not [y for y in hits if y not in Y[d]]
        out.append((d, d < h, comp_c, ret, comp_r, ret_r, blind, ret_ns))
    return out


def stratum(rd, lab):
    adj = sum(1 for x in lab if rd[x[0] - 1][0] == 'A')
    gen = len(lab) - adj
    df = sum(1 for x in lab if x[1])
    kind = 0 if gen == 0 else 1 if adj == 0 else 2
    direction = 0 if df == len(lab) else 1 if df == 0 else 2
    return kind * 3 + direction


def code(rd):
    """main.c's bag_code of the L1 bag: the sorted reading indices"""
    ix = sorted(READ_IX[r] for r in rd)
    c = len(ix)
    for i, v in enumerate(ix):
        c |= v << (3 + 7 * i)
    return c


def orders(bag, canon):
    """every distinct order of the bag, with the canonical analyses
    carried to it: (heads, readings) per analysis"""
    n = len(bag)
    seen = set()
    for p in itertools.permutations(range(n)):
        s = tuple(bag[i] for i in p)
        if s in seen:
            continue
        seen.add(s)
        inv = [0] * n
        for q, i in enumerate(p):
            inv[i] = q
        an = []
        for heads, rd in canon:
            h2 = [0] * n
            r2 = [None] * n
            for i in range(n):
                h2[inv[i]] = 0 if heads[i] == 0 else inv[heads[i] - 1] + 1
                r2[inv[i]] = rd[i]
            an.append((tuple(h2), tuple(r2)))
        yield an


def new_acc():
    return {
        'cell': [[[[[0, 0] for _ in range(2)] for _ in range(2)]
                  for _ in range(NSTRATA)] for _ in range(2)],
        'cell_rel': [[[[[0, 0] for _ in range(2)] for _ in range(2)]
                      for _ in range(NSTRATA)] for _ in range(2)],
        'blind': [[[[0, 0] for _ in range(2)] for _ in range(NSTRATA)]
                  for _ in range(2)],
        'route': [[[0, 0, 0, 0] for _ in range(2)] for _ in range(NSTRATA)],
        'covered_clear': 0,
        'arcs': [[[0] * 6 for _ in range(2)] for _ in range(2)],
        'width': [[0] * (MAXW_ARR + 2) for _ in range(NSTRATA + 1)],
        'depth': [[0] * (MAXW_ARR + 2) for _ in range(NSTRATA + 1)],
        'utype': [0, 0, 0],
        'back': 0,
    }


MAXW_ARR = 6        # main.c's MAXW: the width and depth rows have 8 slots
UTYPE = {'dangling': 0, 'slot': 1, 'root': 2}


def pair_level_free(A, heads, rd, lab_by_lv, proj, s):
    """width, depth, umbra types (into A[0]); arc labels at every level"""
    rels = tuple(relation(heads, rd, d) for d in range(1, len(heads) + 1))
    U, back, width, depth = umbrae(heads, rels)
    A[0]['width'][s][width] += 1
    for u, dp in zip(U, depth):
        A[0]['depth'][s][dp] += 1
        A[0]['utype'][UTYPE[u[0]]] += 1
    A[0]['back'] += back
    if proj:
        return
    for lv in LEVELS:
        for x in lab_by_lv[lv]:
            row = A[lv]['arcs'][0 if rd[x[0] - 1][0] == 'A' else 1][0 if x[1] else 1]
            row[0] += 1
            row[1] += x[3]
            row[2] += not x[2]
            row[3] += x[5]
            row[4] += not x[4]
            row[5] += (not x[3]) and x[7]


def main():
    abstracts = [t for t in W.abstract(MAXW) if not SUB or len(t) == MAXW]
    print('abstract analyses (with repeats):', len(abstracts), file=sys.stderr)
    sel = None
    if SUB:
        codes = sorted({code([r for r, _ in t]) for t in abstracts})
        sel = {c for k, c in enumerate(codes) if k % K == 0}
        print('six-word L1 bags %d, subset %d' % (len(codes), len(sel)),
              file=sys.stderr)
        abstracts = [t for t in abstracts if code([r for r, _ in t]) in sel]
    blevels = (0, 1) if SUB else (0, 1, 2, 3)
    A = [new_acc() for _ in LEVELS]

    # E1: every pair once, by the L1 bags: width, depth, arc labels at
    # every level
    l1bags = {tuple(sorted(FORM[1][r] for r, _ in t)) for t in abstracts}
    npairs = 0
    for bag in l1bags:
        canon = W.analyses(1, list(bag), RDS[1])
        for an in orders(bag, canon):
            for heads, rd in an:
                npairs += 1
                proj, _mild, disp = W.measures(heads)
                rels = tuple(relation(heads, rd, d) for d in range(1, len(heads) + 1))
                U = umbrae(heads, rels)[0]
                lab = [labels(lv, heads, rd, rels, disp, U) for lv in LEVELS] \
                    if not proj else [[] for _ in LEVELS]
                s = NSTRATA if proj else stratum(rd, lab[0])
                pair_level_free(A, heads, rd, lab, proj, s)
    print('E1 pairs', npairs, file=sys.stderr)

    # E2: B by each level's own bags (F(s) and P(s) the string's own)
    for lv in blevels:
        bags = {tuple(sorted(FORM[lv][r] for r, _ in t)) for t in abstracts}
        npairs = 0
        for bag in bags:
            canon = W.analyses(lv, list(bag), RDS[lv])
            keep = [sel is None or code(rd) in sel for _, rd in canon]
            for an in orders(bag, canon):
                inP = [W.measures(h)[0] for h, _ in an]
                hasP = any(inP)
                hp = {}
                for (h, _), p in zip(an, inP):
                    if p:
                        for d, x in enumerate(h, 1):
                            hp.setdefault(d, set()).add(x)
                for (heads, rd), k in zip(an, keep):
                    if not k:
                        continue
                    npairs += 1
                    proj, _mild, disp = W.measures(heads)
                    if proj:
                        continue
                    rels = tuple(relation(heads, rd, d) for d in range(1, len(heads) + 1))
                    U = umbrae(heads, rels)[0]
                    lab = labels(lv, heads, rd, rels, disp, U)
                    s = stratum(rd, lab)
                    c = int(any(x[2] for x in lab))
                    it = int(any(not x[3] for x in lab))
                    cr = int(any(x[4] for x in lab))
                    ir = int(any(not x[5] for x in lab))
                    cb = int(any(x[6] for x in lab))
                    mis = int(hasP)
                    for pop in (0, 1):
                        if pop == 1 and len(an) < 2:
                            continue
                        A[lv]['cell'][pop][s][c][it][0] += 1
                        A[lv]['cell'][pop][s][c][it][1] += mis
                        A[lv]['cell_rel'][pop][s][cr][ir][0] += 1
                        A[lv]['cell_rel'][pop][s][cr][ir][1] += mis
                        A[lv]['blind'][pop][s][cb][0] += 1
                        A[lv]['blind'][pop][s][cb][1] += mis
                    if mis:
                        # a P analysis moves a displaced word: as near,
                        # outside its own (true) subtree; as near, inside it
                        # (the dependency reversed); farther; or none moves
                        near = flip = far = False
                        Y = yields(heads)
                        for x in lab:
                            d = x[0]
                            h = heads[d - 1]
                            for w in hp.get(d, ()):
                                if w == h:
                                    continue
                                if abs(w - d) > abs(h - d):
                                    far = True
                                elif w in Y[d]:
                                    flip = True
                                else:
                                    near = True
                        route = 0 if near else 1 if flip else 2 if far else 3
                        A[lv]['route'][s][c][route] += 1
                        if near and not c:
                            A[lv]['covered_clear'] += 1
        print('E2 level', lv, 'pairs', npairs, file=sys.stderr)

    v = []
    for lv in blevels:
        for pop in (0, 1):
            for s in range(NSTRATA):
                for c in (0, 1):
                    for i in (0, 1):
                        v.extend(A[lv]['cell'][pop][s][c][i])
    for pop in (0, 1):
        for s in range(NSTRATA):
            for c in (0, 1):
                for i in (0, 1):
                    v.extend(A[0]['cell_rel'][pop][s][c][i])
    for pop in (0, 1):
        for s in range(NSTRATA):
            for c in (0, 1):
                v.extend(A[0]['blind'][pop][s][c])
    for s in range(NSTRATA):
        for c in (0, 1):
            v.extend(A[0]['route'][s][c])
    v.append(A[0]['covered_clear'])
    for lv in LEVELS:
        for k in (0, 1):
            for i in (0, 1):
                v.extend(A[lv]['arcs'][k][i])
    for s in range(NSTRATA + 1):
        v.extend(A[0]['width'][s])
    for s in range(NSTRATA + 1):
        v.extend(A[0]['depth'][s])
    v.extend(A[0]['utype'])
    v.append(A[0]['back'])
    print('/* oracle_d134.py %s: %d totals */' % (' '.join(sys.argv[1:]), len(v)))
    for k in range(0, len(v), 8):
        print('      ' + ', '.join(str(x) for x in v[k:k + 8]) + ',')
    adj = A[0]['arcs'][0]
    print('adjective arcs %d; RETRIEVED at L0..L3: %s' % (
        adj[0][0] + adj[1][0],
        [A[lv]['arcs'][0][0][1] + A[lv]['arcs'][0][1][1] for lv in LEVELS]),
        file=sys.stderr)


if __name__ == '__main__':
    main()

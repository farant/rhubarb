#!/usr/bin/env python3
"""D131 Part B oracle: an independent count of the measures A and B.

Independent of main.c in algorithm, not in data: the paradigms are
Bennett's tables again (macrons dropped), but
  - analyses are found TREE FIRST (head arrays constrained by part of
    speech, then readings), where main.c goes readings first;
  - the bags come from a structural enumeration of abstract analyses;
  - projectivity is tested by interval yields, non-projective arcs by
    domination, mildness by gaps and interleaving, all written here.

Usage: python3 -I oracle_partb.py [maxw]   (default 5; main.c's quick
mode is 4). Prints per level: A = (num, den), the reading-cell version,
B = (num, den), B over pairs with a syncretic displaced word and with
unambiguous displaced words, R_M misreads, per-length rows, and the
descriptive splits (what is displaced, mildness, crossing count, the
open cells behind A).
"""
import itertools
import sys

MAXW = int(sys.argv[1]) if len(sys.argv) > 1 else 5
CASES = ['nom', 'gen', 'dat', 'acc', 'abl']

NOUNS = {  # lemma: (gender, sg forms, pl forms), Bennett 20, 23, 32
    'puella': ('f', 'puella puellae puellae puellam puella',
               'puellae puellarum puellis puellas puellis'),
    'dominus': ('m', 'dominus domini domino dominum domino',
                'domini dominorum dominis dominos dominis'),
    'donum': ('n', 'donum doni dono donum dono',
              'dona donorum donis dona donis'),
    'rex': ('m', 'rex regis regi regem rege',
            'reges regum regibus reges regibus'),
}
ADJS = {  # lemma: {gender: (sg, pl)}, Bennett 63, 69
    'bonus': {'m': ('bonus boni bono bonum bono', 'boni bonorum bonis bonos bonis'),
              'f': ('bona bonae bonae bonam bona', 'bonae bonarum bonis bonas bonis'),
              'n': ('bonum boni bono bonum bono', 'bona bonorum bonis bona bonis')},
    'fortis': {'m': ('fortis fortis forti fortem forti', 'fortes fortium fortibus fortes fortibus'),
               'f': ('fortis fortis forti fortem forti', 'fortes fortium fortibus fortes fortibus'),
               'n': ('forte fortis forti forte forti', 'fortia fortium fortibus fortia fortibus')},
}
VERBS = {'amat': ('sg', False), 'amant': ('pl', False), 'dat': ('sg', True), 'dant': ('pl', True)}


def l0(r):
    kind = r[0]
    if kind == 'V':
        return r[1]
    if kind == 'N':
        _, lem, c, g, n = r
        return NOUNS[lem][1 if n == 'sg' else 2].split()[CASES.index(c)]
    _, lem, c, g, n = r
    return ADJS[lem][g][0 if n == 'sg' else 1].split()[CASES.index(c)]


def form(level, r):
    kind = r[0]
    if kind == 'V':
        return r[1]
    _, lem, c, g, n = r
    if level == 1:
        return '%s.%s.%s.%s' % (lem, c, g, n)
    if level == 0:
        return l0(r)
    if kind == 'N' or level == 2:
        return l0((kind, lem, 'nom', g, n))
    return l0(('A', lem, 'nom', 'm', 'sg'))


READINGS = ([('N', lem, c, NOUNS[lem][0], n) for lem in NOUNS for n in ('sg', 'pl') for c in CASES]
            + [('A', lem, c, g, n) for lem in ADJS for g in 'mfn' for n in ('sg', 'pl') for c in CASES]
            + [('V', v, None, None, VERBS[v][0]) for v in VERBS])


def readings_of(level):
    out = {}
    for r in READINGS:
        out.setdefault(form(level, r), []).append(r)
    return out


# ---- abstract analyses (structure only; tokens = readings, heads by token index)
def nps(case, budget, head):
    """NPs in `case` under token `head`: lists of (reading, head) with the
    noun first; heads inside are offsets relative to the NP start."""
    if budget < 1:
        return
    for lem, (g, _, _) in NOUNS.items():
        for n in ('sg', 'pl'):
            noun = ('N', lem, case, g, n)
            for k in range(0, budget):
                for adjl in itertools.combinations_with_replacement(sorted(ADJS), k):
                    base = [(noun, head)] + [(('A', a, case, g, n), 'NP0') for a in adjl]
                    if len(base) > budget:
                        continue
                    yield base
                    for gen in nps('gen', budget - len(base), 'NP0'):
                        yield base + gen


def abstract(maxw):
    for v, (vn, dat) in VERBS.items():
        cases = ['nom', 'acc'] + (['dat'] if dat else [])

        def fill(i, toks):
            if i == len(cases):
                yield from abl(toks)
                return
            for np in nps(cases[i], maxw - len(toks), 0):
                if cases[i] == 'nom' and np[0][0][4] != vn:
                    continue
                yield from fill(i + 1, place(toks, np))

        def abl(toks):
            yield toks
            for np in nps('abl', maxw - len(toks), 0):
                yield from abl(place(toks, np))

        yield from fill(0, [(('V', v, None, None, vn), None)])


def place(toks, np):
    """append an NP; 'NP0' heads point at the NP's own noun, nested
    genitives keep their own noun (resolved by a stack walk)."""
    out = list(toks)
    nounstack = []
    for r, h in np:
        if r[0] == 'N':
            if h == 'NP0':
                h = nounstack[-1]
            nounstack.append(len(out))
            out.append((r, h))
        else:
            out.append((r, nounstack[-1]))
    return out


# ---- tree-first analysis of a canonical token sequence of forms
def analyses(level, bag, R):
    n = len(bag)
    kinds = [R[f][0][0] for f in bag]
    if kinds.count('V') != 1:
        return []
    v = kinds.index('V')
    vr = R[bag[v]][0]
    vn, dat = VERBS[vr[1]]
    nouns = [i for i in range(n) if kinds[i] == 'N']
    choices = []
    for i in range(n):
        if kinds[i] == 'V':
            choices.append([0])
        elif kinds[i] == 'A':
            choices.append([j + 1 for j in nouns])
        else:
            choices.append([v + 1] + [j + 1 for j in nouns if j != i])
    out = []
    for heads in itertools.product(*choices):
        # tree?
        ok = True
        for i in range(n):
            seen = 0
            j = i
            while heads[j] != 0:
                j = heads[j] - 1
                seen += 1
                if seen > n:
                    ok = False
                    break
            if not ok:
                break
        if not ok:
            continue
        gh = [heads[i] for i in nouns if heads[i] != v + 1]
        if len(gh) != len(set(gh)):
            continue
        # noun readings by relation
        opts = []
        for i in nouns:
            if heads[i] == v + 1:
                rs = [r for r in R[bag[i]] if r[2] in ('nom', 'acc', 'abl') or (r[2] == 'dat' and dat)]
                rs = [r for r in rs if r[2] != 'nom' or r[4] == vn]
            else:
                rs = [r for r in R[bag[i]] if r[2] == 'gen']
            opts.append(rs)
        for nr in itertools.product(*opts):
            rd = [None] * n
            rd[v] = vr
            for i, r in zip(nouns, nr):
                rd[i] = r
            roles = [rd[i][2] for i in nouns if heads[i] == v + 1]
            if roles.count('nom') != 1 or roles.count('acc') != 1 or roles.count('dat') != (1 if dat else 0):
                continue
            good = True
            for i in range(n):
                if kinds[i] == 'A':
                    hr = rd[heads[i] - 1]
                    want = [r for r in R[bag[i]] if r[2:] == hr[2:]]
                    if len(want) != 1:
                        good = False
                        break
                    rd[i] = want[0]
            if good:
                out.append((tuple(heads), tuple(rd)))
    return out


# ---- tree measures, written here
MEMO = {}


def measures(heads):
    if heads in MEMO:
        return MEMO[heads]
    n = len(heads)
    kids = {k: [] for k in range(0, n + 1)}
    for d, h in enumerate(heads, 1):
        kids[h].append(d)
    yld = {}

    def y(k):
        s = {k}
        for c in kids[k]:
            s |= y(c)
        yld[k] = s
        return s
    for k in kids[0]:
        y(k)

    def gaps(s):
        t = sorted(s)
        return sum(1 for a, b in zip(t, t[1:]) if b - a > 1)
    proj = all(gaps(yld[k]) == 0 for k in range(1, n + 1))
    wn = True
    for a, b in itertools.combinations(range(1, n + 1), 2):
        if b in yld[a] or a in yld[b]:
            continue
        A, B = sorted(yld[a]), sorted(yld[b])
        for x1, x2 in itertools.combinations(A, 2):
            for z1, z2 in itertools.combinations(B, 2):
                if x1 < z1 < x2 < z2 or z1 < x1 < z2 < x2:
                    wn = False
    mild = max(gaps(yld[k]) for k in range(1, n + 1)) <= 1 and wn
    displaced = []
    for d, h in enumerate(heads, 1):
        if h == 0:
            continue
        lo, hi = min(h, d), max(h, d)
        if any(w not in yld[h] for w in range(lo + 1, hi)):
            displaced.append(d)
    MEMO[heads] = (proj, mild, tuple(displaced))
    return MEMO[heads]


def crossings(heads):
    """pairs of arcs that interleave strictly, the root arc (0, root)
    included"""
    arcs = [(min(h, d), max(h, d)) for d, h in enumerate(heads, 1)]
    cnt = 0
    for (a1, b1), (a2, b2) in itertools.combinations(arcs, 2):
        if a1 < a2 < b1 < b2 or a2 < a1 < b2 < b1:
            cnt += 1
    return cnt


def forced(an, sel, n, which):
    cnt = 0
    for i in range(n):
        vals = {a[which][i] for a, s in zip(an, sel) if s}
        cnt += len(vals) == 1
    return cnt


def main():
    abstracts = list(abstract(MAXW))
    print('abstract analyses (with repeats):', len(abstracts))
    for level in range(4):
        R = readings_of(level)
        bags = {tuple(sorted(form(level, r) for r, _ in t)) for t in abstracts}
        M = dict(a_num=0, a_den=0, ar_num=0, b_num=0, b_den=0, bs_num=0, bs_den=0,
                 bu_num=0, bu_den=0, bm_num=0, pairs=0, open_f=0, open_p=0, vdisp=0)
        # what is displaced: adjectives only, genitives only, both; each
        # with its syncretic / unambiguous split: [num, den, s_num, s_den, u_num, u_den]
        CLS = {c: [0] * 6 for c in ('adj', 'gen', 'mix')}
        MILD = {True: [0, 0], False: [0, 0]}     # crossed pairs by mildness: [num, den]
        CROSS = {c: [0, 0] for c in (1, 2, 3, 4)}  # by crossings (root arc included), 4 = 4+
        PAIRS = {}
        bylen = {}
        for bag in bags:
            canon = analyses(level, list(bag), R)
            n = len(bag)
            seen = set()
            for p in itertools.permutations(range(n)):  # new position q holds canonical p[q]
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
                        r2[inv[i]] = rd[i][2:]
                    an.append((tuple(h2), tuple(r2)))
                fl = [measures(a[0]) for a in an]
                inP = [f[0] for f in fl]
                inM = [f[1] for f in fl]
                allsel = [True] * len(an)
                faF = forced(an, allsel, n, 0)
                frF = forced(an, allsel, n, 1)
                hasP = any(inP)
                if hasP:
                    faP = forced(an, inP, n, 0)
                    frP = forced(an, inP, n, 1)
                L = bylen.setdefault(n, [0, 0, 0, 0])
                for k, a in enumerate(an):
                    M['pairs'] += 1
                    PAIRS[n] = PAIRS.get(n, 0) + 1
                    if inP[k]:
                        M['open_f'] += n - faF
                        M['open_p'] += n - faP
                        M['a_num'] += faP - faF
                        M['ar_num'] += frP - frF
                        M['a_den'] += n
                        L[0] += faP - faF
                        L[1] += n
                    else:
                        mis = hasP
                        M['b_den'] += 1
                        M['b_num'] += mis
                        L[2] += mis
                        L[3] += 1
                        syn = any(len(R[s[d - 1]]) >= 2 for d in fl[k][2])
                        if syn:
                            M['bs_den'] += 1
                            M['bs_num'] += mis
                        else:
                            M['bu_den'] += 1
                            M['bu_num'] += mis
                        M['bm_num'] += (not inM[k]) and any(inM)
                        # descriptive: what is displaced (by the analysis's own heads)
                        heads = a[0]
                        kinds = [R[s[i]][0][0] for i in range(n)]
                        disp = fl[k][2]
                        nadj = sum(1 for d in disp if kinds[d - 1] == 'A')
                        ngen = sum(1 for d in disp if kinds[d - 1] == 'N' and heads[d - 1] != 0
                                   and kinds[heads[d - 1] - 1] == 'N')
                        if nadj + ngen != len(disp):
                            M['vdisp'] += 1
                        c = 'adj' if ngen == 0 else 'gen' if nadj == 0 else 'mix'
                        row = CLS[c]
                        row[0] += mis
                        row[1] += 1
                        row[2 if syn else 4] += mis
                        row[3 if syn else 5] += 1
                        MILD[inM[k]][0] += mis
                        MILD[inM[k]][1] += 1
                        cr = min(crossings(heads), 4)
                        CROSS[cr][0] += mis
                        CROSS[cr][1] += 1
        print('level', level, ' '.join('%s=%d' % kv for kv in M.items()))
        print('  classes', ' '.join('%s=%s' % (c, CLS[c]) for c in ('adj', 'gen', 'mix')))
        print('  mild', MILD[True], 'not mild', MILD[False])
        print('  crossings', ' '.join('%d:%s' % (c, CROSS[c]) for c in (1, 2, 3, 4)))
        print('  pairs by length', sorted(PAIRS.items()))
        for n in sorted(bylen):
            print('  len', n, 'a_num=%d a_den=%d b_num=%d b_den=%d' % tuple(bylen[n]))


if __name__ == '__main__':
    main()

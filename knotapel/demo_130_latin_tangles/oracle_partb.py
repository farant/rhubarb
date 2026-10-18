"""D130 Part B oracle (repo treebanks only), independent of the C demo
and of lib/dependentia.c: own CoNLL-U parsing, own PUNCT removal
(dependents re-attached to the nearest kept ancestor), crossings and
disjoint pairs over all arc pairs, projectivity by interval yields,
non-projective arcs by head walks. Prints the constants pinned in
main.c's Part B checks. Run from the repo root:
    python3 -I knotapel/demo_130_latin_tangles/oracle_partb.py
"""
import sys

UD = 'oratio/probationes/fixa/ud/'
BANKS = [('CIRCSE', ['la_circse-ud-test.conllu']),
         ('LLCT', ['la_llct-ud-dev.conllu', 'la_llct-ud-test.conllu']),
         ('EWT', ['en_ewt-ud-dev.conllu', 'en_ewt-ud-test.conllu'])]


def sentences(path):
    cur = []
    for line in open(path, encoding='utf-8'):
        line = line.rstrip('\n').rstrip('\r')
        if not line:
            if cur:
                yield cur
            cur = []
            continue
        if line.startswith('#'):
            continue
        f = line.split('\t')
        if '-' in f[0] or '.' in f[0]:
            continue
        cur.append((int(f[0]), f[3], int(f[6])))
    if cur:
        yield cur


def strip_punct(words):
    up = {i: u for i, u, h in words}
    head = {i: h for i, u, h in words}
    kept = [i for i, u, h in words if u != 'PUNCT']
    if any(head[i] == 0 for i, u, h in words if u == 'PUNCT'):
        return None
    new = {old: k + 1 for k, old in enumerate(kept)}
    out = []
    for i in kept:
        h = head[i]
        while h != 0 and up[h] == 'PUNCT':
            h = head[h]
        out.append(0 if h == 0 else new[h])
    return out


def ancestors(heads, v):
    s = set()
    while v != 0:
        s.add(v)
        v = heads[v - 1]
    return s


def measure(heads):
    n = len(heads)
    arcs = [(min(h, d), max(h, d), h, d) for d, h in
            enumerate(heads, 1) if h != 0]
    root = heads.index(0) + 1
    cross = pairs = rootc = 0
    crossed = set()
    for i in range(len(arcs)):
        a, b = arcs[i][0], arcs[i][1]
        if a < root < b:
            rootc += 1
        for j in range(i + 1, len(arcs)):
            c, e = arcs[j][0], arcs[j][1]
            if len({a, b, c, e}) == 4:
                pairs += 1
                if a < c < b < e or c < a < e < b:
                    cross += 1
                    crossed.add(i)
                    crossed.add(j)
    anc = [ancestors(heads, v) for v in range(1, n + 1)]
    # yield of u = {v : u in anc[v-1]}
    proj = True
    for u in range(1, n + 1):
        ys = [v for v in range(1, n + 1) if u in anc[v - 1]]
        if ys[-1] - ys[0] + 1 != len(ys):
            proj = False
    nonproj = 0
    for a, b, h, d in arcs:
        if any(h not in anc[p - 1] for p in range(a + 1, b)):
            nonproj += 1
    return dict(cross=cross, pairs=pairs, rootc=rootc, nonplan=len(crossed),
                nonproj=nonproj, proj=int(proj), planar=int(cross == 0),
                words=n)


def len_bin(n):
    return 0 if n <= 10 else 1 if n <= 20 else 2 if n <= 30 else \
        3 if n <= 50 else 4


def dist_bin(c):
    return c if c <= 3 else 4 if c <= 7 else 5


for name, files in BANKS:
    for col in (0, 1):
        lens = [[0, 0, 0, 0] for _ in range(5)]   # trees, crossings, pairs, words
        dist = [0] * 6
        tot = dict(trees=0, aside=0, words=0, cross=0, pairs=0, rootc=0,
                   nonplan=0, nonproj=0, proj=0, planar=0)
        for fn in files:
            for s in sentences(UD + fn):
                heads = [h for i, u, h in s]
                if col == 0:
                    heads = strip_punct(s)
                    if heads is None:
                        tot['aside'] += 1
                        continue
                m = measure(heads)
                tot['trees'] += 1
                b = len_bin(m['words'])
                lens[b][0] += 1
                lens[b][1] += m['cross']
                lens[b][2] += m['pairs']
                lens[b][3] += m['words']
                dist[dist_bin(m['cross'])] += 1
                for k in ('words', 'cross', 'pairs', 'rootc', 'nonplan',
                          'nonproj', 'proj', 'planar'):
                    tot[k] += m[k]
        print(name, 'headline' if col == 0 else 'annotated',
              ' '.join('%s=%d' % (k, tot[k]) for k in
                       ('trees', 'aside', 'words', 'cross', 'pairs', 'rootc',
                        'nonproj', 'nonplan', 'proj', 'planar')))
        if col == 0:
            print('  lens', ' '.join('%d/%d/%d/%d' % tuple(x) for x in lens))
            print('  dist', ' '.join(str(x) for x in dist))
        sys.stdout.flush()

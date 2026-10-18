"""D130 Parts C and D oracle (repo treebanks, headline = without
PUNCT), independent of the C demo and of lib/dependentia.c: own
CoNLL-U and FEATS parsing, non-projective arcs by head walks,
well-nestedness over ALL disjoint subtree pairs (no sibling reduction),
edge degree by union-find components of the induced subgraph with the
root taken as the member of least depth, crossing-graph components by
union-find. Prints the constants pinned in main.c. Run from the repo
root:  python3 -I knotapel/demo_130_latin_tangles/oracle_partcd.py
"""
import sys

UD = 'oratio/probationes/fixa/ud/'
BANKS = [('CIRCSE', ['la_circse-ud-test.conllu']),
         ('LLCT', ['la_llct-ud-dev.conllu', 'la_llct-ud-test.conllu']),
         ('EWT', ['en_ewt-ud-dev.conllu', 'en_ewt-ud-test.conllu'])]
FEATS = ('Case', 'Gender', 'Number')


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
        feats = {}
        if f[5] != '_':
            for kv in f[5].split('|'):
                k, v = kv.split('=', 1)
                feats[k] = v
        cur.append(dict(id=int(f[0]), upos=f[3], head=int(f[6]),
                        rel=f[7].split(':')[0], feats=feats))
    if cur:
        yield cur


def strip(words):
    by = {w['id']: w for w in words}
    if any(w['head'] == 0 and w['upos'] == 'PUNCT' for w in words):
        return None
    kept = [w for w in words if w['upos'] != 'PUNCT']
    new = {w['id']: k + 1 for k, w in enumerate(kept)}
    out = []
    for w in kept:
        h = w['head']
        while h != 0 and by[h]['upos'] == 'PUNCT':
            h = by[h]['head']
        out.append(dict(w, head=0 if h == 0 else new[h]))
    return out


def find(p, x):
    while p[x] != x:
        x = p[x]
    return x


def analyse(ws, tot):
    n = len(ws)
    heads = [w['head'] for w in ws]
    anc = []
    depth = []
    for v in range(1, n + 1):
        s, d, x = set(), 0, v
        while x != 0:
            s.add(x)
            x = heads[x - 1]
            d += 1
        anc.append(s)
        depth.append(d)
    dom = lambda u, v: u in anc[v - 1]
    yields = [[v for v in range(1, n + 1) if dom(u, v)] for u in range(1, n + 1)]
    gd = max(sum(1 for a, b in zip(y, y[1:]) if b - a > 1) for y in yields)
    # well-nested: all ordered disjoint pairs whose spans overlap
    wn = True
    for u in range(1, n + 1):
        yu = set(yields[u - 1])
        for v in range(1, n + 1):
            if u == v or dom(u, v) or dom(v, u):
                continue
            a, b = yields[u - 1], yields[v - 1]
            if a[-1] < b[0] or b[-1] < a[0]:
                continue
            seq = []
            for p in range(min(a[0], b[0]), max(a[-1], b[-1]) + 1):
                lab = 'A' if p in yu else 'B' if dom(v, p) else None
                if lab and (not seq or seq[-1] != lab):
                    seq.append(lab)
            if len(seq) >= 4:
                wn = False
                break
        if not wn:
            break
    # arcs
    arcs = [(min(h, d), max(h, d), h, d) for d, h in enumerate(heads, 1) if h]
    crossing = []
    for i in range(len(arcs)):
        for j in range(i + 1, len(arcs)):
            a, b = arcs[i][:2]
            c, e = arcs[j][:2]
            if a < c < b < e or c < a < e < b:
                crossing.append((i, j))
    ed = 0
    for a, b, h, d in arcs:
        span = list(range(a, b + 1))
        p = {x: x for x in span}
        for x in span:
            hx = heads[x - 1]
            if a <= hx <= b and hx != 0:
                p[find(p, x)] = find(p, hx)
        comps = {}
        for x in span:
            comps.setdefault(find(p, x), []).append(x)
        deg = 0
        for members in comps.values():
            root = min(members, key=lambda x: depth[x - 1])
            if not dom(h, root):
                deg += 1
        ed = max(ed, deg)
    tot['gd'][min(gd, 3)] += 1
    tot['ed'][min(ed, 3)] += 1
    tot['wn'] += wn
    tot['mild'] += (gd <= 1 and wn)
    # crossing-graph components
    p = list(range(len(arcs)))
    for i, j in crossing:
        p[find(p, i)] = find(p, j)
    size, cr = {}, {}
    for i in range(len(arcs)):
        size[find(p, i)] = size.get(find(p, i), 0) + 1
    for i, j in crossing:
        cr[find(p, i)] = cr.get(find(p, i), 0) + 1
    for r, sz in size.items():
        if sz < 2:
            continue
        tot['comps'] += 1
        tot['size'][0 if sz == 2 else 1 if sz == 3 else 2 if sz == 4
                    else 3 if sz <= 8 else 4] += 1
        tot['ccross'] += cr.get(r, 0)
    # relations and agreement
    for a, b, h, d in arcs:
        w, hw = ws[d - 1], ws[h - 1]
        nonproj = any(not dom(h, x) for x in range(a + 1, b))
        agree = [f in w['feats'] and f in hw['feats']
                 and w['feats'][f] == hw['feats'][f] for f in FEATS]
        agree.append(all(agree))
        ad = w['rel'] in ('amod', 'det')
        for k in range(4):
            tot['agr_all'][k] += agree[k]
            if nonproj:
                tot['agr_np'][k] += agree[k]
            if ad:
                tot['ad_agr_all'][k] += agree[k]
                if nonproj:
                    tot['ad_agr_np'][k] += agree[k]
        tot['ad_all'] += ad
        tot['ad_np'] += ad and nonproj
        tot['amod_np'] += w['rel'] == 'amod' and nonproj
        tot['det_np'] += w['rel'] == 'det' and nonproj


for name, files in BANKS:
    tot = dict(gd=[0] * 4, ed=[0] * 4, wn=0, mild=0, comps=0, size=[0] * 5,
               ccross=0, agr_all=[0] * 4, agr_np=[0] * 4, ad_all=0, ad_np=0,
               ad_agr_all=[0] * 4, ad_agr_np=[0] * 4, amod_np=0, det_np=0)
    for fn in files:
        for s in sentences(UD + fn):
            ws = strip(s)
            if ws is not None:
                analyse(ws, tot)
    print(name, ' '.join('%s=%s' % (k, v) for k, v in tot.items()))
    sys.stdout.flush()

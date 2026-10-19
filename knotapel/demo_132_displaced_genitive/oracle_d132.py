#!/usr/bin/env python3
"""D132 oracle: an independent count of displaced genitives, safe ones,
the positional null and the adjective contrast.

Independent of main.c in algorithm:
  - its own CoNLL-U reader and PUNCT removal (a kept word's head climbs
    past removed words to the nearest kept ancestor; a sentence whose
    root is PUNCT is set aside);
  - per-arc non-projectivity by walking each in-span word's ancestors;
  - alternatives by RE-ATTACHING the genitive and testing the new tree;
  - the null by REBUILDING each moved sentence (words reordered, heads
    renumbered) and testing it from scratch (main.c moves positions over
    a dominance matrix);
  - E and the null's tail probability with fractions.Fraction.

Usage: python3 -I oracle_d132.py [--celer] [NC_DIR]
  --celer: repo treebanks only (main.c's quick mode).
Prints per row the totals main.c pins, E as an exact fraction, and
P(null safe count >= S) rounded to millionths (descriptive).
Aggregates only; no sentence is printed.
"""
import sys
from fractions import Fraction

REPO = 'oratio/probationes/fixa/ud/'
NC = '/Users/francisarant/Documents/projects/rhubarb/oratio/build/ud/'
CELER = '--celer' in sys.argv
args = [a for a in sys.argv[1:] if a != '--celer']
if args:
    NC = args[0].rstrip('/') + '/'

FILES = [('CIRCSE', REPO + 'la_circse-ud-test.conllu', False),
         ('LLCT', REPO + 'la_llct-ud-dev.conllu', False),
         ('LLCT', REPO + 'la_llct-ud-test.conllu', False),
         ('PROIEL', NC + 'la_proiel-ud-test.conllu', True),
         ('Perseus', NC + 'la_perseus-ud-test.conllu', True),
         ('ITTB', NC + 'la_ittb-ud-test.conllu', True),
         ('UDante', NC + 'la_udante-ud-test.conllu', True)]
ROWS = ['CIRCSE', 'LLCT', 'PROIEL', 'Perseus', 'ITTB', 'UDante',
        'Perseus prose', 'Perseus verse']


def read(path):
    sid, words = '', []
    for line in open(path, encoding='utf-8'):
        line = line.rstrip('\n')
        if not line:
            if words:
                yield sid, words
            sid, words = '', []
            continue
        if line.startswith('# sent_id'):
            sid = line.split('=', 1)[1].strip()
            continue
        if line.startswith('#'):
            continue
        c = line.split('\t')
        if '-' in c[0] or '.' in c[0]:
            continue
        feats = {}
        if c[5] != '_':
            for kv in c[5].split('|'):
                if '=' in kv:
                    k, v = kv.split('=', 1)
                    feats[k] = v
        words.append({'upos': c[3], 'feats': feats, 'head': int(c[6]),
                      'rel': c[7]})
    if words:
        yield sid, words


def strip_punct(words):
    """kept words in order, heads renumbered; None if the root is PUNCT"""
    n = len(words)
    keep = [w['upos'] != 'PUNCT' for w in words]
    newid = {}
    for i in range(n):
        if keep[i]:
            newid[i + 1] = len(newid) + 1
    out = []
    for i in range(n):
        if not keep[i]:
            if words[i]['head'] == 0:
                return None
            continue
        h = words[i]['head']
        while h != 0 and not keep[h - 1]:
            h = words[h - 1]['head']
        w = dict(words[i])
        w['head'] = 0 if h == 0 else newid[h]
        out.append(w)
    return out


def ancestors(heads, w):
    """set of 1-based ancestors of word w (itself included)"""
    s = {w}
    seen = 0
    while heads[w - 1] != 0:
        w = heads[w - 1]
        s.add(w)
        seen += 1
        if seen > len(heads):
            raise ValueError('cycle')
    return s


def nonproj(heads, d):
    """arc into word d (1-based) is non-projective"""
    h = heads[d - 1]
    lo, hi = min(h, d), max(h, d)
    return any(h not in ancestors(heads, w) for w in range(lo + 1, hi))


def descendants(heads, g):
    return {w for w in range(1, len(heads) + 1) if g in ancestors(heads, w)}


def n_alternatives(heads, g, host):
    """candidate hosts c (host[c-1]) giving a projective arc to g"""
    sub = descendants(heads, g)
    alt = 0
    for c in range(1, len(heads) + 1):
        if not host[c - 1] or c == heads[g - 1] or c in sub:
            continue
        h2 = list(heads)
        h2[g - 1] = c
        if not nonproj(h2, g):
            alt += 1
    return alt


def moved(heads, host, g, p):
    """the sentence with word g moved to index p (0-based) among the
    others in their order: (new heads, new host flags, new g)"""
    n = len(heads)
    others = [w for w in range(1, n + 1) if w != g]
    order = others[:p] + [g] + others[p:]
    newpos = {w: i + 1 for i, w in enumerate(order)}
    nh = [0] * n
    nhost = [False] * n
    for w in range(1, n + 1):
        nh[newpos[w] - 1] = 0 if heads[w - 1] == 0 else newpos[heads[w - 1]]
        nhost[newpos[w] - 1] = host[w - 1]
    return nh, nhost, newpos[g]


def null_share(heads, host, g, maxdist=None):
    """maxdist: keep only placements no farther from the head than that
    (the distance-limited null, T4 review)"""
    m = k = 0
    for p in range(len(heads)):
        nh, nhost, ng = moved(heads, host, g, p)
        if maxdist is not None and abs(ng - nh[ng - 1]) > maxdist:
            continue
        if nonproj(nh, ng):
            m += 1
            if n_alternatives(nh, ng, nhost) == 0:
                k += 1
    return m, k


def null_block(heads, host, g):
    """the genitive's whole subtree moved as a block through every gap"""
    n = len(heads)
    sub = sorted(descendants(heads, g))
    others = [w for w in range(1, n + 1) if w not in sub]
    m = k = 0
    for p in range(len(others) + 1):
        order = others[:p] + sub + others[p:]
        newpos = {w: i + 1 for i, w in enumerate(order)}
        nh = [0] * n
        nhost = [False] * n
        for w in range(1, n + 1):
            nh[newpos[w] - 1] = 0 if heads[w - 1] == 0 else newpos[heads[w - 1]]
            nhost[newpos[w] - 1] = host[w - 1]
        if nonproj(nh, newpos[g]):
            m += 1
            k += n_alternatives(nh, newpos[g], nhost) == 0
    return m, k


def nearest_alt(heads, g, host):
    sub = descendants(heads, g)
    best = 0
    for c in range(1, len(heads) + 1):
        if not host[c - 1] or c == heads[g - 1] or c in sub:
            continue
        h2 = list(heads)
        h2[g - 1] = c
        if not nonproj(h2, g):
            d = abs(c - g)
            best = d if best == 0 else min(best, d)
    return best


def tail(probs, s):
    """P(sum of independent Bernoulli(p) >= s), exact"""
    dist = [Fraction(1)]
    for p in probs:
        nd = [Fraction(0)] * (len(dist) + 1)
        for i, q in enumerate(dist):
            nd[i] += q * (1 - p)
            nd[i + 1] += q * p
        dist = nd
    return sum(dist[s:], Fraction(0))


def agree(a, b):
    return all(a['feats'].get(k) and a['feats'].get(k) == b['feats'].get(k)
               for k in ('Case', 'Gender', 'Number'))


def new_row():
    return dict(trees=0, gen=0, gen_nh=0, nested=0, N=0, S=0, E=Fraction(0),
                probs=[], nhN=0, nhS=0, nhE=Fraction(0), alt=[0, 0, 0, 0],
                adj=0, aN=0, adeg=0, aS=0, aE=Fraction(0), auS=0,
                auE=Fraction(0), nearE=Fraction(0), nearp=[],
                blkE=Fraction(0), gdeg=0, adist=[0] * 6)


def genre(sid):
    if sid.startswith('phi0972'):
        return 'Perseus prose'
    if sid[:7] in ('phi0959', 'phi0975', 'phi0690'):
        return 'Perseus verse'
    return None


def main():
    rows = {r: new_row() for r in ROWS}
    for bank, path, nc in FILES:
        if CELER and nc:
            continue
        for sid, words in read(path):
            kept = strip_punct(words)
            targets = [bank]
            if bank == 'Perseus' and genre(sid):
                targets.append(genre(sid))
            if kept is None:
                continue
            heads = [w['head'] for w in kept]
            n = len(kept)
            noun = [w['upos'] in ('NOUN', 'PROPN') for w in kept]
            for t in targets:
                rows[t]['trees'] += 1
            for g in range(1, n + 1):
                w = kept[g - 1]
                h = heads[g - 1]
                if h == 0:
                    continue
                base = w['rel'].split(':')[0]
                is_gen = base == 'nmod' and w['feats'].get('Case') == 'Gen'
                is_adj = (base == 'amod' and w['upos'] == 'ADJ'
                          and all(k in w['feats'] for k in ('Case', 'Gender', 'Number')))
                if is_gen:
                    disp = nonproj(heads, g)
                    if disp:
                        alt = n_alternatives(heads, g, noun)
                        m, k = null_share(heads, noun, g)
                        mn, kn = null_share(heads, noun, g, abs(g - h))
                        mb, kb = null_block(heads, noun, g)
                        sub = descendants(heads, g)
                        nohost = not any(noun[c - 1] and c != h and c not in sub
                                         for c in range(1, n + 1))
                        dn = nearest_alt(heads, g, noun) if alt else 0
                    for t in targets:
                        r = rows[t]
                        r['gen'] += 1
                        r['gen_nh'] += noun[h - 1]
                        if not disp:
                            r['nested'] += 1
                            continue
                        r['N'] += 1
                        r['S'] += alt == 0
                        if alt:
                            r['alt'][min(alt, 4) - 1] += 1
                        r['E'] += Fraction(k, m)
                        r['probs'].append(Fraction(k, m))
                        r['nearE'] += Fraction(kn, mn)
                        r['nearp'].append(Fraction(kn, mn))
                        r['blkE'] += Fraction(kb, mb)
                        r['gdeg'] += nohost
                        if alt:
                            r['adist'][5 if dn >= 10 else 4 if dn >= 5 else dn - 1] += 1
                        if noun[h - 1]:
                            r['nhN'] += 1
                            r['nhS'] += alt == 0
                            r['nhE'] += Fraction(k, m)
                elif is_adj:
                    for t in targets:
                        rows[t]['adj'] += 1
                    if not nonproj(heads, g):
                        continue
                    agr = [noun[i] and agree(kept[i], w) for i in range(n)]
                    sub = descendants(heads, g)
                    other = any(agr[i] and i + 1 != h and (i + 1) not in sub
                                for i in range(n))
                    af = n_alternatives(heads, g, agr)
                    au = n_alternatives(heads, g, noun)
                    mf, kf = null_share(heads, agr, g)
                    mu, ku = null_share(heads, noun, g)
                    for t in targets:
                        r = rows[t]
                        r['aN'] += 1
                        r['adeg'] += not other
                        r['aS'] += af == 0
                        r['aE'] += Fraction(kf, mf)
                        r['auS'] += au == 0
                        r['auE'] += Fraction(ku, mu)
    for name in ROWS:
        r = rows[name]
        if r['trees'] == 0:
            continue
        p = tail(r['probs'], r['S'])
        pn = tail(r['nearp'], r['S'])
        print('%-14s trees=%d gen=%d gen_nh=%d nested=%d N=%d S=%d E=%s '
              'nhN=%d nhS=%d nhE=%s alt=%s adj=%d aN=%d adeg=%d aS=%d aE=%s '
              'auS=%d auE=%s tail_e6=%d nearE=%s near_tail_e6=%d blkE=%s '
              'gdeg=%d adist=%s'
              % (name, r['trees'], r['gen'], r['gen_nh'], r['nested'], r['N'],
                 r['S'], r['E'], r['nhN'], r['nhS'], r['nhE'],
                 ','.join(map(str, r['alt'])), r['adj'], r['aN'], r['adeg'],
                 r['aS'], r['aE'], r['auS'], r['auE'], round(p * 1000000),
                 r['nearE'], round(pn * 1000000), r['blkE'], r['gdeg'],
                 ','.join(map(str, r['adist']))))


if __name__ == '__main__':
    main()

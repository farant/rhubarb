#!/usr/bin/env python3
"""D133 oracle: an independent count of the populations, the operand F,
the exact stratified permutation tests (L1, L1u, L3, sensitivities), the
descriptive excess beyond distance, the baselines and percentiles.

Independent of main.c in algorithm:
  - its own CoNLL-U reader and PUNCT removal (a kept word's head climbs
    to the nearest kept ancestor; a PUNCT-root sentence is set aside);
  - F from explicit ancestor sets (main.c: a dominance matrix);
  - the exact test from VALUE MULTISETS: within a stratum the values are
    1..10, so the number of k-subsets with sum s is a sum over
    compositions of products of binomials (math.comb), computed by a
    dynamic programme over the ten values (main.c: over the items, in
    magnus); strata convolved as integer dictionaries; p a Fraction;
  - the nulls by REBUILDING moved sentences (as oracle_d132.py).

Usage: python3 -I oracle_d133.py [--celer] [NC_DIR]
Aggregates only; no sentence is printed.
"""
import sys
from fractions import Fraction
from math import comb

REPO = 'oratio/probationes/fixa/ud/'
NC = '/Users/francisarant/Documents/projects/rhubarb/oratio/build/ud/'
CELER = '--celer' in sys.argv
args = [a for a in sys.argv[1:] if a != '--celer']
if args:
    NC = args[0].rstrip('/') + '/'
FCAP = 10
BANKS = ['CIRCSE', 'LLCT', 'PROIEL', 'Perseus', 'ITTB', 'UDante']
FILES = [(0, REPO + 'la_circse-ud-test.conllu', False),
         (1, REPO + 'la_llct-ud-dev.conllu', False),
         (1, REPO + 'la_llct-ud-test.conllu', False),
         (2, NC + 'la_proiel-ud-test.conllu', True),
         (3, NC + 'la_perseus-ud-test.conllu', True),
         (4, NC + 'la_ittb-ud-test.conllu', True),
         (5, NC + 'la_udante-ud-test.conllu', True)]
PARTICLES = {'autem', 'enim', 'igitur', 'vero', 'quidem', 'quoque', 'tamen', 'ergo'}


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
                    feats[k] = set(v.split(','))
        words.append({'lemma': c[2], 'upos': c[3], 'feats': feats,
                      'head': int(c[6]), 'rel': c[7].split(':')[0]})
    if words:
        yield sid, words


def strip_punct(words):
    keep = [w['upos'] != 'PUNCT' for w in words]
    newid, out = {}, []
    for i in range(len(words)):
        if keep[i]:
            newid[i + 1] = len(newid) + 1
    for i, w in enumerate(words):
        if not keep[i]:
            if w['head'] == 0:
                return None
            continue
        h = w['head']
        while h != 0 and not keep[h - 1]:
            h = words[h - 1]['head']
        w2 = dict(w)
        w2['head'] = 0 if h == 0 else newid[h]
        out.append(w2)
    return out


def ancestors(heads, w):
    s = {w}
    while heads[w - 1] != 0:
        w = heads[w - 1]
        s.add(w)
    return s


def foreign(heads, h, d):
    lo, hi = min(h, d), max(h, d)
    return sum(1 for w in range(lo + 1, hi) if h not in ancestors(heads, w))


def nonproj(heads, d):
    return foreign(heads, heads[d - 1], d) > 0


def descendants(heads, g):
    return {w for w in range(1, len(heads) + 1) if g in ancestors(heads, w)}


def n_alternatives(heads, g, host):
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


def null_shares(heads, host, g, obs):
    """move g alone through every position: (k, m) all placements,
    (kn, mn) placements within obs of the head, and (less, equal) for the
    percentile, over displaced placements"""
    n = len(heads)
    others = [w for w in range(1, n + 1) if w != g]
    k = m = kn = mn = less = equal = 0
    for p in range(n):
        order = others[:p] + [g] + others[p:]
        newpos = {w: i + 1 for i, w in enumerate(order)}
        nh = [0] * n
        nhost = [False] * n
        for w in range(1, n + 1):
            nh[newpos[w] - 1] = 0 if heads[w - 1] == 0 else newpos[heads[w - 1]]
            nhost[newpos[w] - 1] = host[w - 1]
        ng = newpos[g]
        if not nonproj(nh, ng):
            continue
        dist = abs(ng - nh[ng - 1])
        safe = n_alternatives(nh, ng, nhost) == 0
        m += 1
        k += safe
        if dist <= obs:
            mn += 1
            kn += safe
        less += dist < obs
        equal += dist == obs
    return k, m, kn, mn, less, equal


def is_fw(w):
    return w['upos'] in ('PART', 'CCONJ', 'ADV') or w['lemma'] in PARTICLES


def meets(a, b, key):
    return bool(a['feats'].get(key, set()) & b['feats'].get(key, set()))


# ---------------- the exact test ----------------
def stratum_counts(values, k):
    """{sum: number of k-subsets} from the value multiset"""
    cnt = {}
    for v in values:
        cnt[v] = cnt.get(v, 0) + 1
    table = {(0, 0): 1}            # (chosen, sum) -> count
    for v, c in cnt.items():
        new = {}
        for (j, s), w in table.items():
            for x in range(0, min(c, k - j) + 1):
                key = (j + x, s + v * x)
                new[key] = new.get(key, 0) + w * comb(c, x)
        table = new
    return {s: w for (j, s), w in table.items() if j == k}


def conv(a, b):
    out = {}
    for s1, w1 in a.items():
        for s2, w2 in b.items():
            out[s1 + s2] = out.get(s1 + s2, 0) + w1 * w2
    return out


def run_test(items, pop, grp, key, value):
    strata = {}
    for it in items:
        if not pop(it):
            continue
        strata.setdefault(key(it), []).append(it)
    total = {0: 1}
    t_obs = 0
    e_perm = Fraction(0)
    n_pop = n_grp = 0
    per_tb = {}
    for sk, its in strata.items():
        vals = [min(value(it), FCAP) for it in its]
        kx = sum(1 for it in its if grp(it))
        n_pop += len(its)
        n_grp += kx
        t_obs += sum(v for v, it in zip(vals, its) if grp(it))
        e = Fraction(kx * sum(vals), len(vals))
        e_perm += e
        d = stratum_counts(vals, kx)
        total = conv(total, d)
        tb = its[0]['bank']
        r = per_tb.setdefault(tb, {'d': {0: 1}, 't': 0, 'e': Fraction(0)})
        r['d'] = conv(r['d'], d)
        r['t'] += sum(v for v, it in zip(vals, its) if grp(it))
        r['e'] += e
    allw = sum(total.values())
    p = Fraction(sum(w for s, w in total.items() if s >= t_obs), allw)
    mean = Fraction(sum(s * w for s, w in total.items()), allw)
    q = {}
    for tb, r in per_tb.items():
        w = sum(r['d'].values())
        q[tb] = Fraction(sum(c for s, c in r['d'].items() if s > r['e']), w)
    return dict(t=t_obs, p=p, e=e_perm, mean=mean, n=n_pop, g=n_grp,
                strata=len(strata), tb=per_tb, q=q)


def main():
    pops = {b: dict(gen_all=0, gen_rel=0, gen_prs=0, gen_non=0, gen_pop=0,
                    gen_disp=0, adj_all=0, adj_inc=0, adj_multi=0, adj_non=0,
                    adj_pop=0, adj_disp=0, nest_n=[0, 0], nest_d=[0, 0])
            for b in range(6)}
    items = []
    present = set()
    for bank, path, nc in FILES:
        if CELER and nc:
            continue
        present.add(bank)
        for sid, words in read(path):
            if bank == 3:
                if sid.startswith('phi0972'):
                    b7 = 3
                elif sid[:7] in ('phi0959', 'phi0975', 'phi0690'):
                    b7 = 6
                else:
                    raise SystemExit('unknown Perseus work')
            else:
                b7 = bank
            kept = strip_punct(words)
            if kept is None:
                continue
            heads = [w['head'] for w in kept]
            n = len(kept)
            lenbin = 0 if n <= 10 else 1 if n <= 20 else 2
            noun = [w['upos'] in ('NOUN', 'PROPN') for w in kept]
            P = pops[bank]
            first = [True, True]
            for g in range(1, n + 1):
                w = kept[g - 1]
                h = heads[g - 1]
                if h == 0:
                    continue
                kind = None
                if w['rel'] == 'nmod' and w['feats'].get('Case') == {'Gen'}:
                    P['gen_all'] += 1
                    pt = w['feats'].get('PronType', set())
                    if 'Rel' in pt or 'Int' in pt:
                        P['gen_rel'] += 1
                    elif not noun[h - 1]:
                        P['gen_non'] += 1
                    else:
                        P['gen_prs'] += 'Prs' in pt
                        kind = 1
                elif w['rel'] == 'amod' and w['upos'] == 'ADJ':
                    P['adj_all'] += 1
                    if not all(k in w['feats'] for k in ('Case', 'Gender', 'Number')):
                        P['adj_inc'] += 1
                    else:
                        P['adj_multi'] += len(w['feats']['Gender'] & {'Fem', 'Masc', 'Neut'}) > 1
                        if not noun[h - 1]:
                            P['adj_non'] += 1
                        else:
                            kind = 0
                if kind is None:
                    continue
                P['adj_pop' if kind == 0 else 'gen_pop'] += 1
                f = foreign(heads, h, g)
                dist = abs(g - h)
                if f == 0:
                    P['nest_n'][kind] += 1
                    P['nest_d'][kind] += dist
                    continue
                P['adj_disp' if kind == 0 else 'gen_disp'] += 1
                lo, hi = min(g, h), max(g, h)
                anc_h_desc = {x for x in range(lo + 1, hi) if h not in ancestors(heads, x)}
                fw = all(is_fw(kept[x - 1]) for x in anc_h_desc)
                unique = None
                if kind == 0:
                    sub = descendants(heads, g)
                    unique = not any(noun[c - 1] and c != h and c not in sub
                                     and all(meets(kept[c - 1], w, k) for k in ('Case', 'Gender', 'Number'))
                                     for c in range(1, n + 1))
                safe = n_alternatives(heads, g, noun) == 0
                k, m, kn, mn, less, equal = null_shares(heads, noun, g, dist)
                items.append(dict(kind=kind, bank=bank, b7=b7, dep_first=g < h,
                                  lenbin=lenbin, f=f, dist=dist, unique=unique,
                                  fw=fw, first=first[kind], safe=safe, kn=kn,
                                  mn=mn, less=less, equal=equal, m=m))
                first[kind] = False

    for b in sorted(present):
        P = pops[b]
        print('pop %s gen %d %d %d %d %d %d adj %d %d %d %d %d %d nest %d %d %d %d'
              % (BANKS[b], P['gen_all'], P['gen_rel'], P['gen_prs'], P['gen_non'],
                 P['gen_pop'], P['gen_disp'], P['adj_all'], P['adj_inc'],
                 P['adj_multi'], P['adj_non'], P['adj_pop'], P['adj_disp'],
                 P['nest_n'][0], P['nest_d'][0], P['nest_n'][1], P['nest_d'][1]))

    def key_l1(it, merge=False):
        b = it['bank'] if merge else it['b7']
        return (b, it['dep_first'], it['lenbin'])

    def key_nodir(it, merge=False):
        b = it['bank'] if merge else it['b7']
        return (b, it['lenbin'])

    F = lambda it: it['f']
    R = lambda it: it['dist']
    adj = lambda it: it['kind'] == 0
    uniq = lambda it: it['unique']
    tests = [
        ('L1', lambda it: True, adj, key_l1, F),
        ('L1u', lambda it: it['dep_first'], adj, key_nodir, F),
        ('L3', lambda it: it['kind'] == 0 and it['bank'] != 1, uniq, key_nodir, F),
        ('L1_first', lambda it: it['first'], adj, key_l1, F),
        ('L3_first', lambda it: it['kind'] == 0 and it['bank'] != 1 and it['first'], uniq, key_nodir, F),
        ('L1_nofw', lambda it: not it['fw'], adj, key_l1, F),
        ('L3_nofw', lambda it: it['kind'] == 0 and it['bank'] != 1 and not it['fw'], uniq, key_nodir, F),
        ('L1_raw', lambda it: True, adj, key_l1, R),
        ('L3_raw', lambda it: it['kind'] == 0 and it['bank'] != 1, uniq, key_nodir, R),
        ('L1_merge', lambda it: True, adj, lambda it: key_l1(it, True), F),
        ('L3_merge', lambda it: it['kind'] == 0 and it['bank'] != 1, uniq, lambda it: key_nodir(it, True), F),
    ]
    for name, pop, grp, key, value in tests:
        r = run_test(items, pop, grp, key, value)
        print('test %s t=%d n=%d g=%d strata=%d e=%s p_e9=%d mean_ok=%s'
              % (name, r['t'], r['n'], r['g'], r['strata'], r['e'],
                 round(r['p'] * 10**9), r['mean'] == r['e']))
        if name == 'L1':
            qs = []
            for b in sorted(r['tb']):
                tb = r['tb'][b]
                print('  tb %s t=%d e=%s q_e6=%d' % (BANKS[b], tb['t'], tb['e'], round(r['q'][b] * 10**6)))
                if b != 1:
                    qs.append(r['q'][b])
            dist = [Fraction(1)]
            for q in qs:
                nd = [Fraction(0)] * (len(dist) + 1)
                for i, x in enumerate(dist):
                    nd[i] += x * (1 - q)
                    nd[i + 1] += x * q
                dist = nd
            print('  chance4_e6=%d' % round(sum(dist[4:], Fraction(0)) * 10**6))

    for b in sorted(present):
        for kind in (0, 1):
            sel = [it for it in items if it['bank'] == b and it['kind'] == kind]
            if not sel:
                continue
            e = sum((Fraction(it['kn'], it['mn']) for it in sel), Fraction(0))
            print('l2 %s %d N=%d S=%d E=%s' % (BANKS[b], kind, len(sel), sum(it['safe'] for it in sel), e))
        for grpname, sel in (('adj_unique', [it for it in items if it['bank'] == b and it['kind'] == 0 and it['unique']]),
                             ('adj_shared', [it for it in items if it['bank'] == b and it['kind'] == 0 and not it['unique']]),
                             ('gen', [it for it in items if it['bank'] == b and it['kind'] == 1])):
            if not sel:
                continue
            s = sum((Fraction(2 * it['less'] + it['equal'], 2 * it['m']) for it in sel), Fraction(0))
            print('pct %s %s n=%d sum=%s' % (BANKS[b], grpname, len(sel), s))


if __name__ == '__main__':
    main()

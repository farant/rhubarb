# genus.py - census generis (T30 a): genus aureum per upos, adiectiva/determinantia/pronomina concordantia cum capite, commune; argumenta = plagulae CoNLL-U
import sys, collections
from conllu import NOMINAL, VERBAL, sentences
from conllu import feats_ordinis as feats
def census(via):
    C = collections.Counter(); G = collections.defaultdict(collections.Counter)
    for s in sentences(via):
        pos = {c[0]: i for i, c in enumerate(s)}
        F = [feats(c) for c in s]
        for i, c in enumerate(s):
            g = F[i].get('Gender')
            if g is None: continue
            C['genus'] += 1; G[c[3]][g] += 1
            if c[3] not in ('ADJ', 'DET', 'NUM', 'PRON'): continue
            if c[7].split(':')[0] not in ('amod', 'det', 'nummod', 'nmod'): continue
            h = pos.get(c[6])
            if h is None or s[h][3] not in NOMINAL or 'Gender' not in F[h]: continue
            C['arcs'] += 1
            C['caput_idem'] += F[h]['Gender'] == g
            # nearest nominal neighbour bearing gender, either side, right on tie
            best = None
            for d in range(1, len(s)):
                for j in (i + d, i - d):
                    if 0 <= j < len(s) and s[j][3] in NOMINAL and 'Gender' in F[j]:
                        best = j; break
                if best is not None: break
            if best is not None:
                C['proximus'] += 1
                C['proximus_idem'] += F[best]['Gender'] == g
                # nearest agreeing in case+number (the T28 law) same gender?
            best2 = None
            for d in range(1, len(s)):
                for j in (i + d, i - d):
                    if 0 <= j < len(s) and s[j][3] in NOMINAL and 'Gender' in F[j] \
                       and F[j].get('Case') == F[i].get('Case') and F[j].get('Number') == F[i].get('Number'):
                        best2 = j; break
                if best2 is not None: break
            if best2 is not None:
                C['concors'] += 1
                C['concors_idem'] += F[best2]['Gender'] == g
    return C, G
for via in sys.argv[1:]:
    C, G = census(via)
    nm = via.split('/')[-1]
    print(nm, 'genus', C['genus'])
    for upos in sorted(G, key=lambda u: -sum(G[u].values())):
        t = sum(G[upos].values()); top = G[upos].most_common(1)[0]
        print('  %-6s %6d  %s' % (upos, t, ' '.join('%s %d%%' % (k, 100*v//t) for k, v in G[upos].most_common())))
    a = C['arcs'] or 1
    print('  arcs %d head-same %d%%  nearest-nominal-same %d/%d %d%%  nearest-case+number-agreeing-same %d/%d %d%%' % (
        C['arcs'], 100*C['caput_idem']//a, C['proximus_idem'], C['proximus'], 100*C['proximus_idem']//max(1,C['proximus']),
        C['concors_idem'], C['concors'], 100*C['concors_idem']//max(1,C['concors'])))

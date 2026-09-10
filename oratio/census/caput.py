# caput.py - census capitis (T28): substantivum proximum concordans caput adiectivi/determinantis est? per distantiam et directionem; argumenta = plagulae CoNLL-U
import sys, collections
from conllu import NOMINAL, VERBAL, sentences
from conllu import feats_ordinis as feats
def agrees(a, b):
    fa, fb = feats(a), feats(b)
    for k in ('Case', 'Number', 'Gender'):
        if k in fa and k in fb and fa[k] != fb[k]: return False
    return 'Case' in fa and 'Case' in fb
def census(via):
    C = collections.Counter()
    for s in sentences(via):
        pos = {c[0]: i for i, c in enumerate(s)}
        verbs = [i for i, c in enumerate(s) if c[3] in VERBAL]
        for i, c in enumerate(s):
            d = c[7].split(':')[0]
            if d not in ('amod', 'det') or c[3] not in ('ADJ', 'DET', 'NUM', 'PRON'): continue
            h = pos.get(c[6])
            if h is None or s[h][3] not in NOMINAL: continue
            C['arcs'] += 1
            dist = abs(h - i); C['adjacent'] += dist == 1
            C['head_after'] += h > i
            # candidates agreeing by gold features, nominal, not self
            cands = [j for j, x in enumerate(s) if j != i and x[3] in ('NOUN', 'PROPN', 'PRON', 'NUM') and agrees(c, x)]
            if h not in cands: C['head_not_agreeing_gold'] += 1; continue
            C['head_agrees'] += 1
            nearest = min(cands, key=lambda j: (abs(j - i), j))
            C['nearest_agreeing'] += nearest == h
            # nearest with no verb between
            def no_verb_between(j):
                lo, hi = (i, j) if i < j else (j, i)
                return not any(lo < v < hi for v in verbs)
            cb = [j for j in cands if no_verb_between(j)]
            if cb:
                nb = min(cb, key=lambda j: (abs(j - i), j))
                C['nearest_noverb'] += nb == h
                C['has_noverb'] += 1
            # nearest agreeing AFTER, else nearest BEFORE (direction preference)
            after = [j for j in cands if j > i]; before = [j for j in cands if j < i]
            if after: C['pref_after'] += min(after) == h
            elif before: C['pref_after'] += max(before) == h
            if before: C['pref_before'] += max(before) == h
            elif after: C['pref_before'] += min(after) == h
            C['single_candidate'] += len(cands) == 1
            C['single_right'] += len(cands) == 1 and cands[0] == h
            C['adjacent_agreeing'] += (nearest == h and dist == 1)
    n = C['head_agrees'] or 1
    print('%-10s arcs %5d adjacent %3d%% head-after %3d%% | head agrees (gold) %3d%% | of those: nearest %3d%%  nearest-no-verb-between %3d%% (%3d%% have one)  prefer-after %3d%%  prefer-before %3d%%  single-candidate %3d%% (right %3d%%)' % (
        via.split('/')[-1].replace('-ud-', '-').replace('.conllu', '').replace('la_', ''), C['arcs'], 100*C['adjacent']//C['arcs'], 100*C['head_after']//C['arcs'],
        100*C['head_agrees']//C['arcs'], 100*C['nearest_agreeing']//n, 100*C['nearest_noverb']//n, 100*C['has_noverb']//n,
        100*C['pref_after']//n, 100*C['pref_before']//n, 100*C['single_candidate']//n, 100*C['single_right']//n))
for via in sys.argv[1:]: census(via)

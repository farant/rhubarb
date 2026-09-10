# stella.py - census stellae verbi (T31 a): subiecta explicita, casus, positio, distantia, concordantia numeri, nominativus proximus concordans = subiectum?, obiecta accusativa
import collections, sys
from conllu import NOMINAL, THESAURI, feats, sentences, pc
files = THESAURI
import sys
for via in files:
    C = collections.Counter(); SC = collections.Counter(); SP = collections.Counter(); SU = collections.Counter(); OC = collections.Counter(); DIST = collections.Counter()
    for s in sentences(via):
        pos = {c[0]: i for i, c in enumerate(s)}
        F = [feats(c[5]) for c in s]
        deps = collections.defaultdict(list)
        for i, c in enumerate(s):
            h = pos.get(c[6])
            if h is not None: deps[h].append(i)
        for i, c in enumerate(s):
            if c[3] != 'VERB': continue
            vf = F[i].get('VerbForm')
            fin = vf == 'Fin'
            C['verba'] += 1
            if fin: C['finita'] += 1
            subs = [j for j in deps[i] if s[j][7].split(':')[0] in ('nsubj', 'csubj')]
            objs = [j for j in deps[i] if s[j][7].split(':')[0] == 'obj']
            if fin:
                C['fin+subj'] += bool(subs); C['fin+obj'] += bool(objs)
                if 'Pass' in F[i].get('Voice', '') or s[i][7] == 'aux:pass': C['fin-pass'] += 1
            for j in subs:
                if s[j][7].startswith('csubj'): C['csubj'] += 1; continue
                C['nsubj'] += 1
                if fin: C['nsubj-fin'] += 1
                SC[F[j].get('Case', '-')] += 1
                SU[s[j][3]] += 1
                SP['ante' if j < i else 'post'] += 1
                d = abs(j - i); DIST['1' if d == 1 else '2-3' if d <= 3 else '4-6' if d <= 6 else '7+'] += 1
                if F[j].get('Number') and F[i].get('Number'):
                    C['num-cmp'] += 1; C['num-eq'] += F[j]['Number'] == F[i]['Number']
                    if F[j]['Number'] != F[i]['Number'] and any(s[k][7].split(':')[0] == 'conj' for k in deps[j]): C['num-ne-conj'] += 1
                if s[j][3] == 'PRON' and F[j].get('PronType') == 'Rel': C['subj-rel'] += 1
                # nearest gold-nominative nominal agreeing in number with the verb = the subject?
                if fin and F[j].get('Case') == 'Nom':
                    C['nom-subj-fin'] += 1
                    best = None
                    for d in range(1, len(s)):
                        for k in (i - d, i + d):
                            if 0 <= k < len(s) and s[k][3] in NOMINAL and F[k].get('Case') == 'Nom' and (not F[i].get('Number') or F[k].get('Number') == F[i].get('Number')):
                                best = k; break
                        if best is not None: break
                    if best == j: C['nearest-nom=subj'] += 1
                    # competitors: other Nom nominals within 5
                    comp = sum(1 for k in range(max(0, i - 5), min(len(s), i + 6)) if k != j and k != i and s[k][3] in NOMINAL and F[k].get('Case') == 'Nom')
                    C['nom-competitor'] += comp > 0
            for j in objs:
                C['obj'] += 1; OC[F[j].get('Case', '-')] += 1
                if fin:
                    C['obj-fin'] += 1
                    if F[j].get('Case') == 'Acc':
                        C['acc-obj-fin'] += 1
                        best = None
                        for d in range(1, len(s)):
                            for k in (i - d, i + d):
                                if 0 <= k < len(s) and s[k][3] in NOMINAL and F[k].get('Case') == 'Acc':
                                    best = k; break
                            if best is not None: break
                        if best == j: C['nearest-acc=obj'] += 1
                        # adposition object? (case dependent)
                        if any(s[k][7].split(':')[0] == 'case' for k in deps[j]): C['obj-with-case'] += 1
        for i, c in enumerate(s):
            if c[7].split(':')[0] == 'cop': C['cop'] += 1
    n = via.split('/')[-1].replace('la_', '').replace('-ud-', ' ').replace('.conllu', '')
    print('=== %s: VERB %d, finite %d; finite with subject %s, with object %s, passive %s; copulas %d' % (n, C['verba'], C['finita'], pc(C['fin+subj'], C['finita']), pc(C['fin+obj'], C['finita']), pc(C['fin-pass'], C['finita']), C['cop']))
    print('   subjects %d (finite %d, clausal %d): case %s | upos %s | before verb %s | distance %s' % (C['nsubj'], C['nsubj-fin'], C['csubj'],
        ' '.join('%s %s' % (k, pc(v, C['nsubj'])) for k, v in SC.most_common(4)), ' '.join('%s %s' % (k, pc(v, C['nsubj'])) for k, v in SU.most_common(5)),
        pc(SP['ante'], C['nsubj']), ' '.join('%s %s' % (k, pc(DIST[k], C['nsubj'])) for k in ('1', '2-3', '4-6', '7+'))))
    print('   number agrees %s (disagreeing with conj %d); relative-pronoun subjects %s; nearest agreeing nominative IS the subject %s of %d; another nominative within 5: %s' % (
        pc(C['num-eq'], C['num-cmp']), C['num-ne-conj'], pc(C['subj-rel'], C['nsubj']), pc(C['nearest-nom=subj'], C['nom-subj-fin']), C['nom-subj-fin'], pc(C['nom-competitor'], C['nom-subj-fin'])))
    print('   objects %d (finite %d): case %s | nearest accusative IS the object %s of %d; objects carrying an adposition %d' % (C['obj'], C['obj-fin'],
        ' '.join('%s %s' % (k, pc(v, C['obj'])) for k, v in OC.most_common(4)), pc(C['nearest-acc=obj'], C['acc-obj-fin']), C['acc-obj-fin'], C['obj-with-case']))

# participium.py - census participiorum activorum per tempus (T31 c): accusativus liber vicinus obiectum participii | frater eodem capite | caput ipsum | alienum; deprels participiorum
import collections, sys
from conllu import NOMINAL, THESAURI, feats, sentences, pc
files = [f for f in THESAURI if "llct-ud-test" not in f]
for via in files:
    C = collections.Counter(); ADJ = collections.Counter(); DEPREL = collections.Counter()
    for s in sentences(via):
        pos = {c[0]: i for i, c in enumerate(s)}
        F = [feats(c[5]) for c in s]
        deps = collections.defaultdict(list)
        for i, c in enumerate(s):
            h = pos.get(c[6])
            if h is not None: deps[h].append(i)
        has_case = set(i for i in range(len(s)) if any(s[k][7].split(':')[0] == 'case' for k in deps[i]))
        for i, c in enumerate(s):
            if c[3] != 'VERB' or F[i].get('VerbForm') != 'Part' or F[i].get('Voice') == 'Pass': continue
            t = F[i].get('Tense', '-')
            rel = {j: s[j][7].split(':')[0] for j in deps[i]}
            objs = [j for j in deps[i] if rel[j] == 'obj']
            C[t] += 1; C[t + '+obj'] += bool(objs)
            DEPREL[(t, s[i][7].split(':')[0])] += 1
            for k in (i - 1, i + 1):
                if 0 <= k < len(s) and s[k][3] in NOMINAL and F[k].get('Case') == 'Acc' and k not in has_case:
                    r = rel.get(k, '-')
                    # where does a non-object adjacent accusative hang? on the participle's own head?
                    hk = pos.get(s[k][6]); hi = pos.get(s[i][6])
                    ADJ[(t, 'obj' if r == 'obj' else 'eodem-capite' if hk == hi else 'caput-ipsum' if hk == k else 'alienum')] += 1
    n = via.split('/')[-1].replace('la_', '').replace('-ud-', ' ').replace('.conllu', '')
    print('=== %s' % n)
    for t in ('Pres', 'Past', 'Perf', 'Fut', '-'):
        if not C[t]: continue
        tot = sum(v for k, v in ADJ.items() if k[0] == t)
        print('   %-4s %4d with object %s | adjacent free accusative %3d: object %s  sibling-under-same-head %s  the participle\'s own head %s  elsewhere %s | deprels %s' % (t, C[t], pc(C[t + '+obj'], C[t]), tot, pc(ADJ[(t, 'obj')], tot), pc(ADJ[(t, 'eodem-capite')], tot), pc(ADJ[(t, 'caput-ipsum')], tot), pc(ADJ[(t, 'alienum')], tot), ' '.join('%s %d' % (k[1], v) for k, v in DEPREL.most_common() if k[0] == t)[:80]))

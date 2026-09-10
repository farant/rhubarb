# infinitivum.py - census obiectorum formarum infinitarum (T31 c): infinitivi/participia/gerundia cum obiecto, subiecta accusativa infinitivorum, accusativus liber vicinus = obiectum | subiectum | alterius?
import collections, sys
from conllu import NOMINAL, THESAURI, feats, sentences, pc
files = THESAURI
for via in files:
    C = collections.Counter(); ADJ = collections.Counter()
    for s in sentences(via):
        pos = {c[0]: i for i, c in enumerate(s)}
        F = [feats(c[5]) for c in s]
        deps = collections.defaultdict(list)
        for i, c in enumerate(s):
            h = pos.get(c[6])
            if h is not None: deps[h].append(i)
        has_case = set(i for i in range(len(s)) if any(s[k][7].split(':')[0] == 'case' for k in deps[i]))
        for i, c in enumerate(s):
            if c[3] != 'VERB': continue
            vf = F[i].get('VerbForm')
            if vf not in ('Inf', 'Part', 'Ger'): continue
            passive = F[i].get('Voice') == 'Pass'
            rel = {j: s[j][7].split(':')[0] for j in deps[i]}
            objs = [j for j in deps[i] if rel[j] == 'obj']
            accsub = [j for j in deps[i] if rel[j] in ('nsubj', 'csubj') and F[j].get('Case') == 'Acc']
            key = vf + ('-pass' if passive else '-act')
            C[key] += 1; C[key + '+obj'] += bool(objs)
            if vf == 'Inf':
                C['inf+accsubj'] += bool(accsub)
                C['inf+accsubj+obj'] += bool(accsub) and bool(objs)
                for j in accsub:
                    C['accsubj'] += 1; C['accsubj-ante'] += j < i; C['accsubj-vicinus'] += abs(j - i) == 1
            for j in objs:
                C[vf + '-obj'] += 1
                C[vf + '-obj-ante'] += j < i; C[vf + '-obj-vicinus'] += abs(j - i) == 1
                if F[j].get('Case') == 'Acc': C[vf + '-obj-acc'] += 1
            # what is an ADJACENT free accusative nominal to this head?
            if not passive:
                for k in (i - 1, i + 1):
                    if 0 <= k < len(s) and s[k][3] in NOMINAL and F[k].get('Case') == 'Acc' and k not in has_case:
                        r = rel.get(k, '-')
                        ADJ[(vf, 'ante' if k < i else 'post', 'obj' if r == 'obj' else 'subj' if r in ('nsubj', 'csubj') else 'alter' if r != '-' else 'alienum')] += 1
    n = via.split('/')[-1].replace('la_', '').replace('-ud-', ' ').replace('.conllu', '')
    print('=== %s' % n)
    for vf in ('Inf', 'Part', 'Ger'):
        a, p = C[vf + '-act'], C[vf + '-pass']
        print('   %-4s active %4d with object %s | passive %4d with object %s | objects %3d: acc %s before %s adjacent %s' % (vf, a, pc(C[vf + '-act+obj'], a), p, pc(C[vf + '-pass+obj'], p), C[vf + '-obj'], pc(C[vf + '-obj-acc'], C[vf + '-obj']), pc(C[vf + '-obj-ante'], C[vf + '-obj']), pc(C[vf + '-obj-vicinus'], C[vf + '-obj'])))
    print('   infinitives with an accusative subject %d (of %d; with an object too %d); accusative subjects %d: before %s adjacent %s' % (C['inf+accsubj'], C['Inf-act'] + C['Inf-pass'], C['inf+accsubj+obj'], C['accsubj'], pc(C['accsubj-ante'], C['accsubj']), pc(C['accsubj-vicinus'], C['accsubj'])))
    for vf in ('Inf', 'Part'):
        for side in ('ante', 'post'):
            tot = sum(v for k, v in ADJ.items() if k[0] == vf and k[1] == side)
            if tot: print('   adjacent free accusative %s active %-4s (%3d): object %s subject %s other-dependent %s not-a-dependent %s' % (side, vf, tot, pc(ADJ[(vf, side, 'obj')], tot), pc(ADJ[(vf, side, 'subj')], tot), pc(ADJ[(vf, side, 'alter')], tot), pc(ADJ[(vf, side, 'alienum')], tot)))

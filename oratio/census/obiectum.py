# obiectum.py - census obiecti verbi (T31 b): verba finita activa/passiva cum obiecto, casus/upos/positio/distantia obiecti, accusativus proximus = obiectum? (sine obiectis adpositionum, sine subiecto), capita infinita cum obiecto
import collections, sys
from conllu import NOMINAL, THESAURI, feats, sentences, pc
files = THESAURI
for via in files:
    C = collections.Counter(); OC = collections.Counter(); OU = collections.Counter(); DIST = collections.Counter(); HF = collections.Counter()
    for s in sentences(via):
        pos = {c[0]: i for i, c in enumerate(s)}
        F = [feats(c[5]) for c in s]
        deps = collections.defaultdict(list)
        for i, c in enumerate(s):
            h = pos.get(c[6])
            if h is not None: deps[h].append(i)
        has_case = set(i for i in range(len(s)) if any(s[k][7].split(':')[0] == 'case' for k in deps[i]))
        acc_all = [i for i in range(len(s)) if s[i][3] in NOMINAL and F[i].get('Case') == 'Acc']
        C['acc-nominalia'] += len(acc_all); C['acc-cum-adpositione'] += sum(1 for i in acc_all if i in has_case)
        for i, c in enumerate(s):
            if c[3] != 'VERB': continue
            vf = F[i].get('VerbForm'); fin = vf == 'Fin'
            passive = F[i].get('Voice') == 'Pass'
            objs = [j for j in deps[i] if s[j][7].split(':')[0] == 'obj']
            subs = set(j for j in deps[i] if s[j][7].split(':')[0] in ('nsubj', 'csubj'))
            if fin:
                C['finita'] += 1
                if passive: C['fin-pass'] += 1; C['fin-pass+obj'] += bool(objs)
                else: C['fin-act'] += 1; C['fin-act+obj'] += bool(objs)
            HF[(vf or '-', bool(objs))] += 1
            for j in objs:
                C['obj'] += 1; OC[F[j].get('Case', '-')] += 1; OU[s[j][3]] += 1
                if not fin: C['obj-non-fin'] += 1; continue
                C['obj-fin'] += 1
                C['obj-ante'] += j < i
                d = abs(j - i); DIST['1' if d == 1 else '2-3' if d <= 3 else '4-6' if d <= 6 else '7+'] += 1
                if F[j].get('Case') != 'Acc': continue
                C['acc-obj-fin'] += 1
                # nearest accusative nominal (any) = object?
                def nearest(pred):
                    for d in range(1, len(s)):
                        for k in (i - d, i + d):
                            if 0 <= k < len(s) and k != i and s[k][3] in NOMINAL and F[k].get('Case') == 'Acc' and pred(k): return k
                    return None
                b1 = nearest(lambda k: True); b2 = nearest(lambda k: k not in has_case); b3 = nearest(lambda k: k not in has_case and k not in subs)
                C['n1'] += b1 == j; C['n2'] += b2 == j; C['n3'] += b3 == j
                comp = sum(1 for k in range(max(0, i - 5), min(len(s), i + 6)) if k != j and k != i and s[k][3] in NOMINAL and F[k].get('Case') == 'Acc' and k not in has_case)
                C['acc-competitor'] += comp > 0
                if F[j].get('Gender') == 'Neut': C['obj-neut'] += 1
    n = via.split('/')[-1].replace('la_', '').replace('-ud-', ' ').replace('.conllu', '')
    print('=== %s: finite active %d with object %s | finite passive %d with object %s | objects %d (of finite %d, non-finite %d)' % (n, C['fin-act'], pc(C['fin-act+obj'], C['fin-act']), C['fin-pass'], pc(C['fin-pass+obj'], C['fin-pass']), C['obj'], C['obj-fin'], C['obj-non-fin']))
    print('   case %s | upos %s | before verb %s | distance %s' % (' '.join('%s %s' % (k, pc(v, C['obj'])) for k, v in OC.most_common(4)), ' '.join('%s %s' % (k, pc(v, C['obj'])) for k, v in OU.most_common(5)), pc(C['obj-ante'], C['obj-fin']), ' '.join('%s %s' % (k, pc(DIST[k], C['obj-fin'])) for k in ('1', '2-3', '4-6', '7+'))))
    print('   nearest accusative = object: any %s | not an adposition object %s | nor the subject %s (of %d) | another free accusative within 5: %s | neuter objects %s' % (pc(C['n1'], C['acc-obj-fin']), pc(C['n2'], C['acc-obj-fin']), pc(C['n3'], C['acc-obj-fin']), C['acc-obj-fin'], pc(C['acc-competitor'], C['acc-obj-fin']), pc(C['obj-neut'], C['obj'])))
    print('   accusative nominals %d, of which adposition objects %s | non-finite heads with object: %s' % (C['acc-nominalia'], pc(C['acc-cum-adpositione'], C['acc-nominalia']), ' '.join('%s %d/%d' % (k, HF[(k, True)], HF[(k, True)] + HF[(k, False)]) for k in ('Inf', 'Part', 'Ger', 'Gdv') if HF[(k, True)] + HF[(k, False)])))

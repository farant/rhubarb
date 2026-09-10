# aurum.py - census generalis thesauri (T13/T26): lexemata, sententiae, radices, arcus vicini, upos, deprels, claves notarum, notae verborum; argumenta = plagulae CoNLL-U
import sys, collections
def census(via):
    deprel = collections.Counter(); feats = collections.Counter(); upos = collections.Counter()
    tokens = 0; sents = 0; ranges = 0; roots = 0; adjacent = 0; arcs = 0
    verbfeats = collections.Counter(); nounfeats = collections.Counter()
    for linea in open(via, encoding='utf-8'):
        linea = linea.rstrip('\n')
        if not linea:
            sents += 1; continue
        if linea[0] == '#': continue
        c = linea.split('\t')
        if '-' in c[0]: ranges += 1; continue
        if '.' in c[0]: continue
        tokens += 1
        upos[c[3]] += 1
        d = c[7].split(':')[0]
        deprel[d] += 1
        if c[6] == '0': roots += 1
        elif c[3] != 'PUNCT':
            arcs += 1
            if abs(int(c[6]) - int(c[0])) == 1: adjacent += 1
        if c[5] != '_':
            for f in c[5].split('|'):
                k = f.split('=')[0]
                feats[k] += 1
                if c[3] == 'VERB': verbfeats[f] += 1
                if c[3] in ('NOUN','ADJ','PRON','DET','PROPN'): nounfeats[k] += 1
    print('==', via.split('/')[-1], 'tokens', tokens, 'sents', sents, 'ranges', ranges, 'roots', roots)
    print('  non-punct non-root arcs', arcs, 'adjacent', adjacent, '=', adjacent*1000//max(arcs,1), 'permille')
    print('  upos:', ', '.join('%s %d' % kv for kv in upos.most_common()))
    print('  deprel:', ', '.join('%s %d' % kv for kv in deprel.most_common(24)))
    print('  feat keys:', ', '.join('%s %d' % kv for kv in feats.most_common()))
    print('  verb feats:', ', '.join('%s %d' % kv for kv in verbfeats.most_common(28)))
for via in sys.argv[1:]: census(via)

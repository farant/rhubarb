# conllu.py - lector CoNLL-U communis censuum auri (oratio/census)
#
# Census auri = dimidium 'praedicere' cycli praedicere-deinde-verificare
# (T31 d, 2026-09-09): antequam lex aedificetur, aurum ipsum rogatur
# quid lex attingere possit (quotiens X proximus Y est, distantiae,
# vicinitas, per relationem, per thesaurum). Thesauri VII Latini:
# V pinnati (fixa/ud) et IV scrinii (build/ud, ./oratio/oraculum.sh
# -petere eos adfert; git eos ignorat).
#
# Sententia = lista ordinum (columnae CoNLL-U X: id forma lemma upos
# xpos feats caput deprel deps misc); ordines multiverborum (1-2) et
# vacui (1.1) praetermissi.
NOMINAL = ('NOUN', 'PROPN', 'PRON', 'ADJ', 'NUM', 'DET')
VERBAL = ('VERB', 'AUX')
THESAURI = ['oratio/probationes/fixa/ud/la_circse-ud-test.conllu', 'oratio/probationes/fixa/ud/la_llct-ud-dev.conllu', 'oratio/probationes/fixa/ud/la_llct-ud-test.conllu', 'oratio/build/ud/la_ittb-ud-test.conllu', 'oratio/build/ud/la_perseus-ud-test.conllu', 'oratio/build/ud/la_proiel-ud-test.conllu', 'oratio/build/ud/la_udante-ud-test.conllu']
def feats(s):
    d = {}
    if s != '_':
        for x in s.split('|'):
            k, _, v = x.partition('='); d[k] = v
    return d
def feats_ordinis(c):
    return feats(c[5])
def sentences(via):
    s = []
    for l in open(via, encoding='utf-8'):
        l = l.rstrip('\n')
        if not l:
            if s: yield s; s = []
            continue
        if l[0] == '#': continue
        c = l.split('\t')
        if '-' in c[0] or '.' in c[0]: continue
        s.append(c)
    if s: yield s
def pc(a, b): return '%3d%%' % (100 * a // b) if b else '  -'
def titulus(via): return via.split('/')[-1].replace('la_', '').replace('-ud-', ' ').replace('.conllu', '')

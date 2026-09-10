# fiducia.py - tabula fiduciae ex ordinibus AUCTOR (T32 a): per titulum regulae, arcus et rectae plagularum PINNATARUM
# solarum (circse, llct dev/test, ewt dev - thesauri scrinii validatio manent), permille praecisionis; scribit
# oratio/probationes/fixa/auctores.tsv (artefactum fixum quod exsecutor legit: ORATIO_FIDUCIA=1). Usus: fiducia.py <mensura.tsv> [exitus]
import sys, collections
PINNATAE = ('la_circse-ud-test', 'la_llct-ud-dev', 'la_llct-ud-test', 'en_ewt-ud-dev')
A = collections.defaultdict(lambda: [0, 0])
for l in open(sys.argv[1], encoding='utf-8'):
    c = l.rstrip('\n').split('\t')
    if len(c) < 15 or c[1] != 'AUCTOR' or not any(p in c[0] for p in PINNATAE): continue
    A[c[2]][0] += int(c[11]); A[c[2]][1] += int(c[12])
out = sys.argv[2] if len(sys.argv) > 2 else 'oratio/probationes/fixa/auctores.tsv'
with open(out, 'w', encoding='utf-8') as f:
    f.write('# auctores.tsv - tabula fiduciae (T32 a, 2026-09-09): titulus regulae, arcus, rectae, permille - plagulae pinnatae solae; regeneratio: python3 oratio/census/fiducia.py <mensura.tsv> (causa nominata)\n')
    for t, (n, r) in sorted(A.items(), key=lambda kv: (-(1000 * kv[1][1] // kv[1][0] if kv[1][0] else 0), kv[0])):
        if n: f.write('%s\t%d\t%d\t%d\n' % (t, n, r, 1000 * r // n))
print('scriptum', out, len([1 for n, r in A.values() if n]), 'auctores')

# contentiones.py - census contentionum (T32 b): ex ordinibus CONTENTIO unius tsv (metire.sh), per par (victor, victa)
# thesauris Latinis summatis: alternae, victae rectae (permille) - iudicis contentionum materia (quae regula cui cedere
# debeat); ordine numeri. Usus: contentiones.py <mensura.tsv> [plagula-pars]
#        contentiones.py <mensura.tsv> -scribere [exitus]   (T32 c): tabulam IUDICIS scribit ex plagulis PINNATIS solis
#        (oratio/probationes/fixa/contentiones.tsv: victor victa numerus victae-rectae permille) - regeneratio causa nominata
import sys, collections
PINNATAE = ('la_circse-ud-test', 'la_llct-ud-dev', 'la_llct-ud-test', 'en_ewt-ud-dev')
C = collections.defaultdict(lambda: [0, 0, 0, 0])
scribere = len(sys.argv) > 2 and sys.argv[2] == '-scribere'
pars = None if scribere or len(sys.argv) < 3 else sys.argv[2]
for l in open(sys.argv[1], encoding='utf-8'):
    c = l.rstrip('\n').split('\t')
    if len(c) < 6 or c[1] != 'CONTENTIO': continue
    if scribere:
        if not any(p in c[0] for p in PINNATAE): continue
    elif 'en_ewt' in c[0] or (pars and pars not in c[0]): continue
    C[(c[2], c[3])][0] += int(c[4]); C[(c[2], c[3])][1] += int(c[5])
    if len(c) >= 8: C[(c[2], c[3])][2] += int(c[6]); C[(c[2], c[3])][3] += int(c[7])
def pm(a, b): return 1000 * a // b if b else 0
if scribere:
    out = sys.argv[3] if len(sys.argv) > 3 else 'oratio/probationes/fixa/contentiones.tsv'
    with open(out, 'w', encoding='utf-8') as f:
        f.write('# contentiones.tsv - tabula iudicis contentionum (T32 c, 2026-09-09): victor victa DISCRIMINANTES (victa sola + victor solus) victa-sola permille(victa sola / discriminantes) - plagulae pinnatae solae, statu e6866ffa (ante iudicem) mensurata; contentiones arcu eodem (subiectum/obiectum eiusdem verbi) non discriminant; regeneratio: python3 oratio/census/contentiones.py <mensura.tsv> -scribere (causa nominata)\n')
        for (v, w), (n, r, vs, vr) in sorted(C.items(), key=lambda kv: (-(kv[1][2] + kv[1][3]), kv[0])):
            if vs + vr: f.write('%s\t%s\t%d\t%d\t%d\n' % (v, w, vs + vr, vs, pm(vs, vs + vr)))
    print('scriptum', out, len(C), 'paria')
else:
    print('%-44s > %-44s %6s %6s %6s %6s' % ('victor', 'victa', 'n', 'victa', 'v.sola', 'vr.sol'))
    for (v, w), (n, r, vs, vr) in sorted(C.items(), key=lambda kv: -kv[1][0])[:60]:
        print('%-44s > %-44s %6d %6d %6d %6d' % (v, w, n, pm(r, n), vs, vr))

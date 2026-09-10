# contentiones.py - census contentionum (T32 b/c/d): ex ordinibus CONTENTIO unius tsv (metire.sh), per par (victor, victa)
# thesauris Latinis summatis. Columnae ARCUS (T32 c): victa recta (permille), victa SOLA recta (victor capite falso),
# victor SOLUS rectus; contentiones capite eodem (subiectum contra obiectum eiusdem verbi) in neutro. Columnae CASUS
# (T32 d): contentiones casu iudicatae (casus aureus dependentis, lectio victae, lectio victoris omnes noti), victa casu
# sola recta, victor casu solus rectus - contentio lectionis in arcu uno hic sola discriminat. LUCRUM flexionis =
# (victa sola - victor solus) arcuum et casuum.
# Usus: contentiones.py <mensura.tsv> [plagula-pars] [-casus]      (-casus: ordine discriminationis casuum)
#        contentiones.py <mensura.tsv> -scribere [exitus]   (T32 c): tabulam IUDICIS (recusati) scribit ex plagulis PINNATIS
#        solis (oratio/probationes/fixa/contentiones.tsv: victor victa numerus victae-rectae permille) - regeneratio causa nominata
import sys, collections
PINNATAE = ('la_circse-ud-test', 'la_llct-ud-dev', 'la_llct-ud-test', 'en_ewt-ud-dev')
# n, victae rectae, victa sola, victor solus, casus iudicati, victa casu sola, victor casu solus
C = collections.defaultdict(lambda: [0] * 7)
argumenta = sys.argv[2:]
scribere = argumenta[:1] == ['-scribere']
casu = '-casus' in argumenta
pars = None
for a in argumenta:
    if not a.startswith('-'): pars = a
if scribere: pars = None
for l in open(sys.argv[1], encoding='utf-8'):
    c = l.rstrip('\n').split('\t')
    if len(c) < 6 or c[1] != 'CONTENTIO': continue
    if scribere:
        if not any(p in c[0] for p in PINNATAE): continue
    elif 'en_ewt' in c[0] or (pars and pars not in c[0]): continue
    r = C[(c[2], c[3])]
    for i, col in enumerate(range(4, 11)):
        if len(c) > col: r[i] += int(c[col])
def pm(a, b): return 1000 * a // b if b else 0
if scribere:
    out = argumenta[1] if len(argumenta) > 1 else 'oratio/probationes/fixa/contentiones.tsv'
    with open(out, 'w', encoding='utf-8') as f:
        f.write('# contentiones.tsv - tabula iudicis contentionum (T32 c, 2026-09-09): victor victa DISCRIMINANTES (victa sola + victor solus) victa-sola permille(victa sola / discriminantes) - plagulae pinnatae solae, statu e6866ffa (ante iudicem) mensurata; contentiones arcu eodem (subiectum/obiectum eiusdem verbi) non discriminant; regeneratio: python3 oratio/census/contentiones.py <mensura.tsv> -scribere (causa nominata)\n')
        for (v, w), (n, r, vs, vr, ci, cs, cr) in sorted(C.items(), key=lambda kv: (-(kv[1][2] + kv[1][3]), kv[0])):
            if vs + vr: f.write('%s\t%s\t%d\t%d\t%d\n' % (v, w, vs + vr, vs, pm(vs, vs + vr)))
    print('scriptum', out, len(C), 'paria')
else:
    print('%-40s > %-40s %6s %5s | %6s %6s %6s | %6s %6s %6s %6s' % (
        'victor', 'victa', 'n', 'victa', 'a.sola', 'a.solus', 'lucrum', 'c.iud', 'c.sola', 'c.solus', 'lucrum'))
    clavis = (lambda kv: -(kv[1][5] + kv[1][6])) if casu else (lambda kv: -kv[1][0])
    for (v, w), (n, r, vs, vr, ci, cs, cr) in sorted(C.items(), key=clavis)[:60]:
        print('%-40s > %-40s %6d %5d | %6d %6d %+6d | %6d %6d %6d %+6d' % (
            v[:40], w[:40], n, pm(r, n), vs, vr, vs - vr, ci, cs, cr, cs - cr))

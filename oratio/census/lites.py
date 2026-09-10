# lites.py - census litium (T32 e): ex ordinibus LIS (oraculum -machina -lites; metire.sh x.tsv -lites) - contentio singula
# cum notis quas regula videre potuit. QUAESTIO: quae nota sortem findat - in pari subiectum/obiectum verbi eiusdem (contentio
# lectionis: caput victoris idem, casus aureus nom|acc) casus aureus contra notas: ante/post, nominativi alii in clausula
# (ulli | certi = sine accusativa | concordes numero capitis), accusativi certi, genus victae neutrum, primum clausulae,
# numerus/persona/vox capitis. Pro nota quaque: quot lites notam ferunt et quot earum aureae accusativae; quot sine nota et quot
# aureae nominativae; PURITAS regulae 'nota -> accusativus, aliter nominativus' contra maiorem partem (basis).
# -meet: paria notarum (cellae IV, maior pars cuiusque cellae = regula) - puritas optima in specimine (optimistica: intra specimen apta).
# Usus: lites.py <x.tsv> [plagula-pars] [-victor V] [-victa W] [-meet]   (ordinarius: subiectum-*-proximo > obiectum-verbi-*-proximo)
import sys, collections
argumenta = sys.argv[2:]
pars = None; victor_p = None; victa_p = None; meet = False
i = 0
while i < len(argumenta):
    a = argumenta[i]
    if a == '-victor': victor_p = argumenta[i + 1]; i += 2
    elif a == '-victa': victa_p = argumenta[i + 1]; i += 2
    elif a == '-meet': meet = True; i += 1
    else: pars = a; i += 1
def par_est(v, w):
    if victor_p or victa_p: return (not victor_p or v == victor_p) and (not victa_p or w == victa_p)
    return v.startswith('umbra-subiectum-') and v.endswith('-proximo') and w.startswith('umbra-obiectum-verbi-') and w.endswith('-proximo')
L = collections.defaultdict(list)   # plagula -> lites
for l in open(sys.argv[1], encoding='utf-8'):
    c = l.rstrip('\n').split('\t')
    if len(c) < 23 or c[1] != 'LIS': continue
    if 'en_ewt' in c[0] or (pars and pars not in c[0]): continue
    if not par_est(c[2], c[3]): continue
    if c[12] != '1' or c[7] not in ('nominativus', 'accusativus'): continue
    f = c[0].split('/')[-1].replace('.conllu', '')
    L[f].append(c)
NOTAE = [
    ('ante caput',            lambda c: c[6] == '1'),
    ('post caput',            lambda c: c[6] == '0'),
    ('nominativi alii > 0',   lambda c: int(c[15]) > 0),
    ('nominativi certi > 0',  lambda c: int(c[16]) > 0),
    ('nom. concordes > 0',    lambda c: int(c[17]) > 0),
    ('accusativi certi > 0',  lambda c: int(c[18]) > 0),
    ('genus victae neutrum',  lambda c: c[19] == 'neutrum'),
    ('primum clausulae',      lambda c: c[14] == '1'),
    ('caput pluralis',        lambda c: c[20] == 'pluralis'),
    ('caput III persona',     lambda c: c[21] == 'III'),
    ('caput deponens',        lambda c: c[22] == 'deponens'),
    ('deprel nsubj',          lambda c: c[10].startswith('nsubj')),
    ('deprel obj',            lambda c: c[10] == 'obj'),
    ('caput aureum idem',     lambda c: c[11] == '1'),
]
def pm(a, b): return 1000 * a // b if b else 0
def tabula(titulus, lites):
    n = len(lites); acc = sum(1 for c in lites if c[7] == 'accusativus'); nom = n - acc
    print('%s: lites %d  aureae nom %d acc %d  basis (maior pars) %d permille' % (titulus, n, nom, acc, pm(max(nom, acc), n)))
    print('  %-24s %6s %6s %6s | %6s %6s %6s | %8s' % ('nota', 'cum', 'c.acc', 'c.nom', 'sine', 's.acc', 's.nom', 'puritas'))
    for t, f in NOTAE:
        cum = [c for c in lites if f(c)]; sine = [c for c in lites if not f(c)]
        ca = sum(1 for c in cum if c[7] == 'accusativus'); sa = sum(1 for c in sine if c[7] == 'accusativus')
        # regula: nota -> accusativus, sine nota -> nominativus
        recta = ca + (len(sine) - sa)
        print('  %-24s %6d %6d %6d | %6d %6d %6d | %8d' % (t, len(cum), ca, len(cum) - ca, len(sine), sa, len(sine) - sa, pm(recta, n)))
    if meet:
        optima = []
        for x in range(len(NOTAE)):
            for y in range(x + 1, len(NOTAE)):
                if NOTAE[x][0].startswith('deprel') or NOTAE[y][0].startswith('deprel') or NOTAE[x][0].startswith('caput aureum') or NOTAE[y][0].startswith('caput aureum'): continue
                cellae = collections.defaultdict(lambda: [0, 0])
                for c in lites:
                    k = (NOTAE[x][1](c), NOTAE[y][1](c)); cellae[k][c[7] == 'accusativus'] += 1
                recta = sum(max(v) for v in cellae.values())
                optima.append((pm(recta, n), NOTAE[x][0], NOTAE[y][0], dict((k, tuple(v)) for k, v in cellae.items())))
        optima.sort(key=lambda t: -t[0])
        for p, a, b, cellae in optima[:5]:
            print('  meet %4d permille  %s x %s  cellae (nom, acc):' % (p, a, b), ' '.join('%s%s=%d/%d' % ('A' if k[0] else 'a', 'B' if k[1] else 'b', v[0], v[1]) for k, v in sorted(cellae.items())))
    # distributiones deprel aurei
    d = collections.Counter((c[7][:3], c[10]) for c in lites)
    print('  deprel aurei:', ', '.join('%s/%s %d' % (k[0], k[1], v) for k, v in sorted(d.items(), key=lambda kv: -kv[1])[:10]))
omnes = []
for f in sorted(L):
    tabula(f, L[f]); omnes += L[f]
if len(L) > 1: tabula('SUMMA', omnes)

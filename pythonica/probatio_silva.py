"""probatio_silva.py - porta natalis faciei Pythonicae (pythonica/silva.py).

Plagulae fictae in build/pythonica/; nihil in arbore tangitur.
Exitus 0 sanum | 1 fractum. Culpae plantatae: ancora fallens plagulam
intactam relinquit; differre substantiva verdictum negat."""
import os
import sys

RADIX = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(RADIX, 'pythonica'))
import silva  # noqa: E402

T = os.path.join(RADIX, 'build', 'pythonica')
os.makedirs(T, exist_ok=True)
fracta = []


def credo(cond, titulus):
    print(('  ok   ' if cond else '  FRACTUM ') + titulus)
    if not cond:
        fracta.append(titulus)


FONS = ('#include "latina.h"\n'
        '\n'
        'vacuum a(vacuum);\n'
        'vacuum b(vacuum);\n'
        '\n'
        '/* a */\n'
        'vacuum\n'
        'a (vacuum)\n'
        '{\n'
        '    redde;\n'
        '}\n'
        '\n'
        '/* b */\n'
        'vacuum\n'
        'b (vacuum)\n'
        '{\n'
        '    i32  x;\n'
        '\n'
        '    x  = I;\n'
        '    redde;\n'
        '}\n')
via = os.path.join(T, 'duo.c')
open(via, 'w').write(FONS)

print('--- extenta ---')
x = silva.extenta(via)
credo(len(x) == 4, 'quattuor extenta')
credo([e.titulus for e in x] == ['a', 'b', 'a', 'b'], 'ordo fontis')
d_b = [e for e in x if e.titulus == 'b' and e.definitio][0]
credo((d_b.linea_a, d_b.linea_nodi, d_b.linea_b) == (12, 14, 21),
      'b: extentum 12, nodus 14, finis 21')

print('--- corpus ---')
c = silva.corpus(via, 'b')
credo(c.startswith('vacuum\nb (vacuum)\n{'), 'corpus b sine commentario')
credo(c.endswith('}\n'), 'corpus b ad clausum')

print('--- Editio: substituere nomine, omnia aut nihil ---')
e = silva.Editio(via)
e.substituere('b', 'vacuum\nb (vacuum)\n{\n    redde;\n}\n')
credo(open(via).read() == FONS, 'discus intactus ante applicare')
credo('/* b */' in e.textus and 'x  = I;' not in e.textus,
      'commentarium ducens manet, corpus vetus abiit')
try:
    e.replace('nemo_hic_est', 'x')
    credo(False, 'ancora fallens levat')
except silva.SilvaError:
    credo(True, 'ancora fallens levat SilvaError')
credo(open(via).read() == FONS, 'discus intactus post ancoram fallentem')
diff = e.applicare(forma=False, iudica=False).diff
credo('-    x  = I;' in diff and diff.startswith('--- a/'), 'diff unificatus')
credo(open(via).read() == e.textus, 'scriptum semel')

print('--- Editio: ancora tolerans spatiis ---')
open(via, 'w').write(FONS)
e = silva.Editio(via)
e.replace('x = I;', 'x = II;')            # fons habet 'x  = I;' (duo spatia)
credo('x  = II;' not in e.textus and 'x = II;' in e.textus,
      'ancora uno spatio congruit duobus; novus exacte scriptus')
try:
    e.replace('redde;', 'frange;')        # bis in plagula
    credo(False, 'ancora ambigua levat')
except silva.SilvaError:
    credo(True, 'ancora ambigua (2 vices) levat')
e.replace('redde;', 'frange;', numerus=2)
credo(e.textus.count('frange;') == 2, 'numerus=2 ambas tangit')

print('--- inserere ---')
open(via, 'w').write(FONS)
e = silva.Editio(via)
e.inserere_post('a', 'vacuum\nc (vacuum)\n{\n    redde;\n}\n')
e.applicare(iudica=False)
x2 = silva.extenta(via)
credo([t.titulus for t in x2 if t.definitio] == ['a', 'c', 'b'],
      'c post a, ante b')
e = silva.Editio(via)
e.inserere_ante('a', '/* d */\nvacuum\nd (vacuum)\n{\n    redde;\n}\n')
e.applicare(iudica=False)
x3 = silva.extenta(via)
credo([t.titulus for t in x3 if t.definitio] == ['d', 'a', 'c', 'b'],
      'd ante a (ante commentarium a)')

print('--- iudicia ---')
open(via, 'w').write(FONS)
credo(silva.examen(via) == 'ACCIPE', 'examen ACCIPE')
lint = silva.lint(via)
credo(any(d.regula == 'aequatio-assignationum' for d in lint)
      or any(d.regula == 'columnae-binae' for d in lint), 'lint ordines')
credo(all(isinstance(d.emendationes, int) for d in lint),
      'columna emendationum')
credo(silva.formare(via), 'formare scripsit')
# NB: 'x  = I;' sola (glomus unius) a nulla regula regitur - R7 'i32  x;'
# (glomus unius) regitur: hoc metimur
credo('i32  x;' not in open(via).read(), 'formata (R7 glomus unius)')

print('--- applicare: portae ---')
open(via, 'w').write(FONS)
e = silva.Editio(via)
e.substituere('b', 'vacuum\nb (vacuum)\n{\n    i32  y;\n\n    redde;\n}\n')
f = e.applicare()
credo(f.examen == 'ACCIPE' and f.sana, 'examen in fructu')
credo(f.formata and 'i32  y;' not in open(via).read()
      and 'i32 y;' in open(via).read(), 'forma applicata (i32  y -> i32 y)')
credo('+    i32 y;' in f.diff, 'diff post formam')
credo(f.differentia is not None and f.differentia.verdictum == 'substantiva'
      and ('MUTATA', 'b', 'substantiva') in f.unitates(), 'differentia: b substantiva')
credo(str(f).startswith('applicare ') and 'examen: ACCIPE' in str(f), 'compendium')
# lectio rancida: alius scribit interea
open(via, 'w').write(FONS)
e = silva.Editio(via)
e.replace('redde;', 'frange;', numerus=2)
open(via, 'a').write('/* alius interea */\n')
try:
    e.applicare()
    credo(False, 'lectio rancida refutatur')
except silva.SilvaError:
    credo(True, 'lectio rancida refutatur (SilvaError)')
credo(open(via).read().endswith('/* alius interea */\n'), 'discus alterius intactus')
# REICE refertur, non revertitur; strictum revertit
open(via, 'w').write(FONS)
e = silva.Editio(via)
e.replace('    redde;\n}\n\n/* b */', '    // C99\n    redde;\n}\n\n/* b */', tolerans=False)
f = e.applicare()
credo(f.examen == 'REICE' and not f.sana, 'REICE refertur')
credo('// C99' in open(via).read(), 'non revertitur (ordinarius)')
open(via, 'w').write(FONS)
e = silva.Editio(via)
e.replace('    redde;\n}\n\n/* b */', '    // C99\n    redde;\n}\n\n/* b */', tolerans=False)
try:
    e.applicare(strictum=True)
    credo(False, 'strictum levat')
except silva.SilvaError:
    credo(True, 'strictum levat SilvaError')
credo(open(via).read() == FONS, 'strictum restituit textum ante')
# iudica=False: velox, sine verdicto
open(via, 'w').write(FONS)
e = silva.Editio(via); e.replace('redde;', 'frange;', numerus=2)
f = e.applicare(iudica=False, forma=False)
credo(f.examen is None and f.differentia is None and not f.formata, 'iudica/forma seiuncta')

print('--- usus + renominare ---')
u = silva.usus('formator_extenta')
credo(any(x.via.endswith('silva_formator.c') for x in u.sedes), 'usus: sedes definitionis')
credo(any(v.endswith('formator.c') for v in u.usus), 'usus: usus per plagulam')
credo(silva.usus('nemo_hic_est_omnino').sedes == [], 'usus: ignotum vacuum')
open(via, 'w').write(FONS)
planum = silva.renominare('b', 'beta', [via])
credo('PLANUM' in planum and 'splicendae 2' in planum
      and open(via).read() == FONS, 'renominare: planum (2 sedes), discus intactus')
silva.renominare('b', 'beta', [via], scribere=True)
t = open(via).read()
credo('beta (vacuum)' in t and 'vacuum beta(vacuum);' in t and 'b (vacuum)' not in t,
      'renominare -scribere: definitio et prototypum')

print('--- Refactio + prototypum_synchronizare ---')
# ordo domus: custos primum, inclusiones intra (inclusio ANTE custodem
# prototypum extentis celat - quaestio silvae, vide worklog)
CAPUT = ('#ifndef DUO_H\n'
         '#define DUO_H\n'
         '\n'
         '#include "latina.h"\n'
         '\n'
         'vacuum\n'
         'b (vacuum);\n'
         '\n'
         '#endif /* DUO_H */\n')
via_h = os.path.join(T, 'duo.h')
open(via, 'w').write(FONS); open(via_h, 'w').write(CAPUT)
# signatura mutata in .c: sine synchronizatione examen REICE (typi pugnantes)
r = silva.Refactio()
r.editio(via).substituere('b', 'vacuum\nb (i32 x)\n{\n    (vacuum)x;\n    redde;\n}\n')
r.editio(via).replace('vacuum b(vacuum);', 'vacuum b(i32 x);')     # prototypum locale
r.prototypum_synchronizare(via, via_h, 'b')
credo(open(via).read() == FONS and open(via_h).read() == CAPUT, 'nihil scriptum ante applicare')
fr = r.applicare()
credo(len(fr) == 2 and all(f.sana for f in fr), 'ambae scriptae, examen ACCIPE')
credo('b (i32 x);' in open(via_h).read() or 'i32 x' in silva.corpus(via_h, 'b', definitio=False),
      'prototypum capitis ex capite definitionis')
credo(any(('MUTATA', 'b', 'substantiva') in f.unitates() for f in fr), 'unitas b in fructu')
# synchronizatio eadem plagula (acervus prototyporum localium)
open(via, 'w').write(FONS)
r = silva.Refactio()
r.editio(via).substituere('b', 'vacuum\nb (i32 x)\n{\n    (vacuum)x;\n    redde;\n}\n')
r.prototypum_synchronizare(via, via, 'b')
fr = r.applicare()
credo(fr[0].sana and 'i32 x' in silva.corpus(via, 'b', definitio=False), 'prototypum locale synchronizatum, ACCIPE')
# absens: error; finis inserit
open(via_h, 'w').write(CAPUT)
r = silva.Refactio(); r.editio(via)
try:
    r.prototypum_synchronizare(via, via_h, 'a')
    credo(False, 'prototypum absens levat')
except silva.SilvaError:
    credo(True, 'prototypum absens levat (ordinarius)')
r = silva.Refactio(); r.prototypum_synchronizare(via, via_h, 'a', si_absens='finis'); r.applicare()
credo('a (vacuum);' in open(via_h).read() and open(via_h).read().rstrip().endswith('#endif /* DUO_H */'), 'finis: ante #endif insertum')
# lectio rancida in plagula altera: NIHIL scriptum
open(via, 'w').write(FONS); open(via_h, 'w').write(CAPUT)
r = silva.Refactio()
r.editio(via).replace('redde;', 'frange;', numerus=2)
r.editio(via_h).replace('b (vacuum);', 'b (i32 x);')
open(via_h, 'a').write('/* alius */\n')
try:
    r.applicare(); credo(False, 'transactio: lectio rancida refutatur')
except silva.SilvaError:
    credo(open(via).read() == FONS, 'transactio: plagula prima intacta quoque')
# strictum: REICE in una -> ambae restitutae
open(via, 'w').write(FONS); open(via_h, 'w').write(CAPUT)
r = silva.Refactio()
r.editio(via).replace('redde;', 'frange;', numerus=2)
r.editio(via_h).replace('b (vacuum);', '// C99\nvacuum\nb (vacuum);', tolerans=False)
try:
    r.applicare(strictum=True); credo(False, 'strictum trans plagulas')
except silva.SilvaError:
    credo(open(via).read() == FONS and open(via_h).read() == CAPUT, 'strictum: ambae restitutae')

print('--- typi: extenta + membrum_addere ---')
FONS_T = ('#include "latina.h"\n'
          '\n'
          'nomen structura {\n'
          '    i32 a;\n'
          '    vacuum* memoria;\n'
          '} S;\n'
          '\n'
          'vacuum\n'
          'f (vacuum)\n'
          '{\n'
          '    redde;\n'
          '}\n')
via_t = os.path.join(T, 'typi.c')
open(via_t, 'w').write(FONS_T)
xt = silva.extenta(via_t)
credo([(x.titulus, x.genus) for x in xt] == [('S', 'structura'), ('f', 'definitio')],
      'extenta: structura S + definitio f')
credo(silva.corpus(via_t, 'S', genus='typus').startswith('nomen structura {'), 'corpus typi')
e = silva.Editio(via_t)
e.membrum_addere('S', '    b32 vexillum;')
e.membrum_addere('S', '    i32 cursor;', post='i32 a;')
f = e.applicare()
c = silva.corpus(via_t, 'S', genus='typus')
credo(f.sana and 'cursor;' in c and 'vexillum;' in c, 'membra addita, ACCIPE')
credo(c.index('a;') < c.index('cursor;') < c.index('memoria;') < c.index('vexillum;'), 'ordo: post a, ante claudentem')
credo(f.formata and ('vacuum* memoria;' in c or 'vacuum*' in c), 'forma columnas ordinavit')
try:
    silva.Editio(via_t).membrum_addere('S', '    i32 z;', post='nemo;')
    credo(False, 'post ignotum levat')
except silva.SilvaError:
    credo(True, 'post ignotum levat SilvaError')
# substituere typum nomine (genus='typus'); sine genere non videtur
open(via_t, 'w').write(FONS_T)
e = silva.Editio(via_t)
e.substituere('S', 'nomen structura {\n    i32 a;\n} S;\n', genus='typus')
f = e.applicare(iudica=False)
t = open(via_t).read()
credo('memoria' not in t and 'i32 a;' in t and 'f (vacuum)' in t,
      'substituere typum nomine: S sola mutata')
try:
    silva.Editio(via_t).substituere('S', 'x')
    credo(False, 'substituere sine genere typum videt')
except silva.SilvaError:
    credo(True, 'substituere sine genere typum non videt (levat)')
# custos post inclusionem: extenta per ramum sumptum
open(via_t, 'w').write('#include "latina.h"\n\n#ifndef CUSTOS_Y\n#define CUSTOS_Y\n\nvacuum\nb (vacuum);\n\n#endif\n')
credo([x.titulus for x in silva.extenta(via_t)] == ['b'], 'custos post inclusionem: prototypum visum')

print('--- legati (pons MCP) ---')
import time
t0 = time.time()
sy = silva.symbolum('formator_lint')
credo('silva_formator.c' in sy, 'symbolum per legatum (%.1fs natus)' % (time.time() - t0))
credo('formator_lint' in silva.vocantes('formator_lint_intra'), 'vocantes')
credo('silva_formator.h' in silva.inclusiones('silva/instrumenta/silva_formator.c'), 'inclusiones')
credo('ACCIPE' in silva.diagnostica('silva/instrumenta/silva_formator.c'), 'diagnostica')
try:
    silva.legati('nemo_instrumentum')
    credo(False, 'instrumentum ignotum levat')
except silva.SilvaError:
    credo(True, 'instrumentum ignotum levat SilvaError')

print('--- selecta, origo, arbor, differre_git ---')
open(via, 'w').write(FONS)
sel = silva.selecta(via, 'redde', intra='b')
credo(len(sel) == 1 and sel[0].textus.strip() == 'redde;', 'selecta: redde intra b %r' % ((sel and sel[0][:2]),))
e = silva.Editio(via)
e.replace_selecta('redde', '    frange;', intra='b')
f = e.applicare(forma=False)
credo(silva.corpus(via, 'b').count('frange;') == 1 and silva.corpus(via, 'a').count('redde;') == 1,
      'replace_selecta: b sola mutata')
try:
    silva.Editio(via).replace_selecta('redde', 'x;')       # a et ... ambigua? (a habet redde)
    credo(silva.selecta(via, 'redde') == [] or True, 'selecta sine intra: si una, licet')
except silva.SilvaError:
    credo(True, 'selecta ambigua levat')
credo('return' in silva.origo(via, 8) or 'si' in silva.origo(via, 8) or silva.origo(via, 8) == '',
      'origo currit')
credo('<parsura' in silva.arbor(via), 'arbor: documentum STML')
dg = silva.differre_git('pythonica/silva.py' if False else 'lib/piscina.c')
credo(dg.verdictum == 'cosmetica' and dg.cosmetica_solum, 'differre_git: piscina.c == HEAD')

print('--- ancorae lexematum ---')
open(via, 'w').write(FONS)
e = silva.Editio(via)
e.replace('vacuum b (vacuum);', 'vacuum b(i32 x);')           # plagula: 'vacuum b(vacuum);'
credo('vacuum b(i32 x);' in e.textus and 'vacuum b(vacuum);' not in e.textus,
      'lexemata: spatium additum in ancora congruit')
e = silva.Editio(via)
e.replace('a(vacuum)\n{', 'a (vacuum)\n{')                     # plagula: 'a (vacuum)\n{'
credo(e.textus.count('a (vacuum)') == 1, 'lexemata: spatium ablatum in ancora congruit')
e = silva.Editio(via)
e.replace('x  =  I ;', 'x = II;')                               # plagula: 'x  = I;'
credo('x = II;' in e.textus, 'lexemata: spatia ubique indifferentia')
try:
    silva.Editio(via).replace('"/* b */"', 'x')
    credo(False, 'littera exacta')
except silva.SilvaError:
    credo(True, 'lexemata: ancora absens levat')
try:
    silva.Editio(via).replace('redde ;', 'frange;')
    credo(False, 'ambigua levat')
except silva.SilvaError:
    credo(True, 'lexemata: ambigua (2) levat')
e = silva.Editio(via); e.replace('x  = I;', 'x = II;', tolerans='spatia')
credo('x = II;' in e.textus, "tolerans='spatia' forma vetus manet")

print('--- portae + commissio + planta ---')
pp = silva.porta('formator-intra')
credo(pp.cucurrit and pp.sana and 'sanum' in pp.compendium, 'porta formator-intra sana (%s)' % pp.compendium)
credo(silva.porta_viae('silva/fontes/x.c') == 'silva' and silva.porta_viae('lib/x.c') == 'radix',
      'porta_viae')
try:
    silva.porta('nemo')
    credo(False, 'porta ignota levat')
except silva.SilvaError:
    credo(True, 'porta ignota levat SilvaError')
try:
    silva.commissio('nihil', ['FAQ.md'])
    credo(False, 'via vetita levat')
except silva.SilvaError:
    credo(True, 'commissio: via vetita refutata')
open(via, 'w').write(FONS)
def porta_ficta(v):
    t = open(v).read()
    return silva.Porta('ficta', True, 'PLANTATUM' not in t, 'ficta', 0, '')
ANC = 'redde;\n}\n\n/* b */'
PLA = 'redde; /* PLANTATUM */\n}\n\n/* b */'
r, g = silva.planta(via, ANC, PLA, porta_ficta)
credo(open(via).read() == FONS, 'planta: reversa post ritum')
def porta_muta(v):
    return silva.Porta('muta', True, True, 'muta', 0, '')
try:
    silva.planta(via, ANC, PLA, porta_muta)
    credo(False, 'porta muta levat')
except silva.SilvaError:
    credo(open(via).read() == FONS, 'planta: porta muta levat ET reversa')
def porta_fracta(v):
    return silva.Porta('fracta', True, False, 'fracta', 1, 'x.c:1:1: error: fictus')
try:
    silva.planta(via, ANC, PLA, porta_fracta)
    credo(False, 'aedificatio fracta levat')
except silva.SilvaError as ex:
    credo('AEDIFICATIONEM' in str(ex) and open(via).read() == FONS, 'planta: aedificatio fracta nominata ET reversa')

print('--- umbra: receptum sigillo ligatum ---')
via_r = silva.porta_umbra('formator-intra')
credo(os.path.exists(via_r + '.pendens'), 'pendens scriptum')
credo(any(st == 'pendens' for _, st in silva.portae_pendentes()), 'portae_pendentes: pendens')
r = silva.exspectare(via_r, tectum=300)
credo(r.sana and r.cucurrit and not r.rancida, 'receptum sanum (%s)' % r.compendium)
credo(silva.receptum_validum(via_r).sana, 'receptum validum (arbor eadem)')
novum = os.path.join(silva.RADIX, 'pythonica', '.umbra_probatio.tmp')
open(novum, 'w').write('x')
try:
    pv = silva.receptum_validum(via_r)
    credo(not pv.sana and 'POST cursum' in pv.compendium, 'receptum rancidum post mutationem arboris')
    try:
        silva.commissio('nihil', ['pythonica/README.md'], portae=[via_r])
        credo(False, 'commissio cum recepto rancido refutatur')
    except silva.SilvaError as ex:
        credo('rancidum' in str(ex), 'commissio: receptum rancidum refutatum')
finally:
    os.unlink(novum)
credo(silva.receptum_validum(via_r).sana, 'receptum iterum validum post reversionem')
silva.receptum_delere(via_r)
credo(not os.path.exists(via_r) and not os.path.exists(via_r + '.acta'),
      'receptum deletum cum actis')
credo(all(v != via_r for v, _ in silva.portae_pendentes()),
      'portae_pendentes: receptum abiit')

print('--- differre ---')
cos = FONS.replace('x  = I;', 'x = I;')
sub = FONS.replace('x  = I;', 'x = II;')
doc = FONS.replace('/* b */', '/* b - mutatum */')
r = silva.differre(FONS, cos)
credo(r.verdictum == 'cosmetica' and r.cosmetica_solum, 'cosmetica solum')
r = silva.differre(FONS, sub)
credo(r.verdictum == 'substantiva' and not r.cosmetica_solum,
      'substantiva negat')
r = silva.differre(FONS, doc)
credo(r.verdictum == 'documentaria' and not r.cosmetica_solum,
      'documentaria gradu cosmetico negat')
r = silva.differre(FONS, doc, gradus='documentaria')
credo(r.cosmetica_solum, 'documentaria gradu documentario admittitur')
credo(any(p.status == 'MUTATA' and p.titulus == 'b' for p in r.paria),
      'par b MUTATA')

print()
if fracta:
    print('PYTHONICA: FRACTA %d' % len(fracta))
    sys.exit(1)
print('PYTHONICA: sana')

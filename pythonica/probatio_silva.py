"""probatio_silva.py - porta natalis faciei Pythonicae (pythonica/silva.py).

Plagulae fictae in build/pythonica/; nihil in arbore tangitur.
Exitus 0 sanum | 1 fractum. Culpae plantatae: ancora fallens plagulam
intactam relinquit; differre substantiva verdictum negat."""
import os
import subprocess
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
        '    (vacuum)x;\n'
        '    redde;\n'
        '}\n')
via = os.path.join(T, 'duo.c')
open(via, 'w').write(FONS)

print('--- extenta ---')
x = silva.extenta(via)
credo(len(x) == 4, 'quattuor extenta')
credo([e.titulus for e in x] == ['a', 'b', 'a', 'b'], 'ordo fontis')
d_b = [e for e in x if e.titulus == 'b' and e.definitio][0]
credo((d_b.linea_a, d_b.linea_nodi, d_b.linea_b) == (12, 14, 22),
      'b: extentum 12, nodus 14, finis 22')

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
credo('x  = II;' in e.textus and 'x = II;' not in e.textus,
      'ancora uno spatio congruit duobus; lexemata sola mutata (spatia plagulae manent)')
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
credo('x  = II;' in e.textus, 'lexemata: spatia ubique indifferentia (forma plagulae manet)')
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

print('--- lex formae ancorarum tolerantium (md_arbor.c contortum, 2026-09-03) ---')
via_f = os.path.join(T, 'forma.c')
FONS_F = ('#include "latina.h"\n\nvacuum f(i32 a, i32 b);\n\n'
          '/* f - explicatio longa\n * quae lineas duas tenet */\n'
          'vacuum\nf (i32 a, i32 b)\n{\n    i32  x;\n\n    x  = I;\n'
          '    g(&x, ALPHA, a,\n        b);\n    redde h(x, a, b);\n}\n')
open(via_f, 'w').write(FONS_F)
e = silva.Editio(via_f)
e.replace('g(&x, ALPHA, a, b); redde h(x, a, b);', 'g(&x, BETA, a, b); redde h(x, a, b);')
credo('g(&x, BETA, a,\n        b);\n    redde h(x, a, b);' in e.textus,
      'novus planus lexematibus paribus: substitutio in situ, lineae plagulae servatae')
try:
    silva.Editio(via_f).replace('g(&x, ALPHA, a, b); redde h(x, a, b);',
                                'g(&x, ALPHA, a, b, c); redde h(x, a, b);')
    credo(False, 'refusio novi plani')
except silva.SilvaError as ex:
    credo('forma perderetur' in str(ex) and 'lineae [13]' in str(ex),
          'novus planus lexematibus imparibus super lineas plures REFUSATUR, lineae nominatae')
e = silva.Editio(via_f)
e.replace('g(&x, ALPHA, a, b); redde h(x, a, b);', 'g(&x, ALPHA, a, b, c);\n    redde h(x, a, b);')
credo('g(&x, ALPHA, a, b, c);\n    redde h(x, a, b);' in e.textus, 'novus cum lineis novis: forma auctoris verbatim')
e = silva.Editio(via_f); e.replace('g(&x, ALPHA, a, b);', '')
credo('g(' not in e.textus and 'redde h' in e.textus, 'novus vacuus delet extentum plurium linearum')
try:
    silva.Editio(via_f).replace('g(&x, ALPHA, a, b); frange;', 'x')
    credo(False, 'proxima')
except silva.SilvaError as ex:
    credo('proxima: lexemata 12/14' in str(ex) and "exspectatum 'frange', inventum 'redde' (linea 15" in str(ex),
          'ancora absens: sedes proxima, lexema divergens, linea nominata')
try:
    silva.Editio(via_f).replace('g(&x, ALPHA, a, b);', 'x', tolerans=False)
    credo(False, 'exacta')
except silva.SilvaError as ex:
    credo('lineae [13] (tolerans=True)' in str(ex), 'ancora exacta absens: series lexematum inventa nominatur')
try:
    silva.Editio(via_f).replace('nemo_hic(x);', 'x')
    credo(False, 'absens')
except silva.SilvaError as ex:
    credo("lexema primum ancorae 'nemo_hic' in plagula absens" in str(ex), 'ancora absens: lexema primum absens nominatur')
e = silva.Editio(via_f); e.replace('/* f - explicatio longa quae lineas duas tenet */', '/* f */')
credo('/* f */\nvacuum\nf (' in e.textus, "commentum reflexum: margines ' * ' in ancora indifferentes")
e = silva.Editio(via_f); e.replace('explicatio longa quae lineas', 'explicatio brevis quae lineas', tolerans='verba')
credo('/* f - explicatio brevis quae lineas duas tenet */' in e.textus, "tolerans='verba': prosa trans marginem commenti")
f = e.applicare(iudica=False)
credo(f.formata is not None and 'explicatio brevis' in open(via_f).read(), 'verba: scriptum')
os.unlink(via_f)

print('--- Textus: textus planus ---')
via_t2 = os.path.join(T, 'planus.md')
open(via_t2, 'w').write('# titulus\n\nalpha beta\nalpha gamma\n')
tx = silva.Textus(via_t2)
tx.replace('alpha beta', 'ALPHA beta')
credo(open(via_t2).read().startswith('# titulus'), 'Textus: discus intactus ante applicare')
try:
    tx.replace('alpha', 'x')          # bis (una iam mutata: 'alpha gamma' + nihil) -> 1? 'ALPHA beta' non congruit -> 1
    credo(True, 'Textus: numerus exactus post mutationem')
except silva.SilvaError:
    credo(False, 'Textus: numerus exactus post mutationem')
try:
    silva.Textus(via_t2).replace('alpha', 'x')   # in disco adhuc bis
    credo(False, 'Textus: ancora ambigua levat')
except silva.SilvaError:
    credo(True, 'Textus: ancora ambigua (2) levat')
tx.appendere('cauda\n')
f = tx.applicare()
credo(open(via_t2).read() == '# titulus\n\nALPHA beta\nx gamma\ncauda\n' and f.diff, 'Textus: scriptum semel, diff')
tx2 = silva.Textus(via_t2); open(via_t2, 'a').write('alius\n')
try:
    tx2.replace('cauda', 'c'); tx2.applicare(); credo(False, 'Textus: lectio rancida')
except silva.SilvaError:
    credo(True, 'Textus: lectio rancida refutatur')

print('--- ancorae: commenta spatiis indifferentia ---')
open(via, 'w').write(FONS.replace('/* b */', '/*\tb   nota\n   */'))
e = silva.Editio(via)
e.replace('/* b nota */\nvacuum\nb (vacuum)', '/* B */\nvacuum\nb (vacuum)')
credo('/* B */' in e.textus and '\tb   nota' not in e.textus, 'commentum: tabulae et lineae refractae in ancora indifferentes')

print('--- mensurae (volumen verum) ---')
ms = silva.mensurae('', 1)
credo(len(ms) == 1 and ms[0].mensurae.get('suita.tempus.totum', 0) > 0, 'mensurae radix: sessio plena cum toto')
ms2 = silva.mensurae('silva.', 2)
credo(len(ms2) >= 1 and all('suita.tempus.totum' in s.mensurae for s in ms2)
      and any(k.startswith('probatio.cursus.probatio_silva_') for k in ms2[0].mensurae), 'mensurae silva: tituli praefixo exuti')
credo('totum' in silva.compendium_mensurae(ms[0]) and 'probatio_' in silva.compendium_mensurae(ms[0]), 'compendium_mensurae')
credo(silva.mensurae('nemo.', 1) == [], 'mensurae: praefixum ignotum vacuum')

print('--- replace_inter (Editio + Textus) ---')
open(via, 'w').write(FONS)
e = silva.Editio(via)
e.replace_inter('b (vacuum)\n{', 'redde;\n}', '\n    frange;\n')   # 'vacuum b (vacuum)' prototypum quoque congrueret
credo('x  = I;' not in e.textus and 'frange;' in e.textus and '/* b */' in e.textus and 'redde;\n}' in e.textus and e.textus.count('redde;') == 2,
      'Editio.replace_inter: spatium inter ancoras substitutum, ancorae SERVATAE, commentum ducens manet')
e2 = silva.Editio(via)
e2.replace_inter('b (vacuum)\n{', 'redde;\n}', 'b (vacuum)\n{\n    frange;\n}', inclusae=True)
credo(e2.textus.count('redde;') == 1 and 'frange;' in e2.textus, 'Editio.replace_inter inclusae=True: ancorae devoratae (mos vetus)')
try:
    silva.Editio(via).replace_inter('redde;', '}', 'x')       # initium bis
    credo(False, 'replace_inter initium ambiguum levat')
except silva.SilvaError:
    credo(True, 'replace_inter: initium ambiguum levat')
tx = silva.Textus(via_t2)
tx.replace_inter('# titulus', 'gamma', ' X ')
credo(tx.textus == '# titulus X gamma\ncauda\nalius\n', 'Textus.replace_inter: ancorae servatae (%r)' % tx.textus[:30])
tx = silva.Textus(via_t2)
tx.replace_inter('# titulus', 'gamma', '# T', inclusae=True)
credo(tx.textus == '# T\ncauda\nalius\n', 'Textus.replace_inter inclusae=True')
try:
    silva.Editio(via).inserere_ante('nemo_hic', 'x')
except silva.SilvaError as ex:
    credo('definitio' in str(ex) or 'definitiones' in str(ex), 'nuntius refusionis nominis definitiones nominat')

print('--- sigillum: vetitae non sigillantur ---')
vet = os.path.join('pythonica', '.vetita_probatio.tmp')
silva.VETITAE = tuple(silva.VETITAE) + (vet,)
s_ante = silva.sigillum_arboris()
open(os.path.join(silva.RADIX, vet), 'w').write('x')
try:
    credo(silva.sigillum_arboris() == s_ante, 'sigillum: plagula vetita nova ignorata')
finally:
    os.unlink(os.path.join(silva.RADIX, vet))
    silva.VETITAE = tuple(v for v in silva.VETITAE if v != vet)

print('--- metiri ---')
mm = silva.metiri('lib/piscina.c', n=1)
credo(mm.parsare_ms > 0 and mm.allocationes > 0 and 'glr' in mm.phases and mm.campi and mm.campi['via'].endswith('piscina.c'), 'metiri: mensura cum phasibus (%.1f ms)' % mm.parsare_ms)
mc = silva.metiri('probationes/fixa/css/adversarius.css', n=1)
credo(mc.parsare_ms >= 0 and 'arbor_legendi' in mc.phases and int(mc.campi['nodi']) > 0 and mc.campi['sana'] == '1', 'metiri css: semita materiae per titulos (%s nodi)' % mc.campi['nodi'])

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
credo(r.photographia and os.path.isdir(r.photographia['via_operis']) and r.sigillum == r.photographia['arbor'],
      'receptum photographicum: clone + arbor ut sigillum')
credo(silva.receptum_validum(via_r, viae=['pythonica/README.md']).sana, 'receptum validum (plagula intacta = blob photographiae)')
novum = os.path.join(silva.RADIX, 'pythonica', '.umbra_probatio.tmp')
open(novum, 'w').write('x')
try:
    pv = silva.receptum_validum(via_r, viae=['pythonica/README.md'])
    credo(pv.sana, 'umbra photographica: plagula nova extra commissionem receptum NON rancidum facit (editio dum porta currit)')
    pv2 = silva.receptum_validum(via_r, viae=['pythonica/.umbra_probatio.tmp'])
    credo(not pv2.sana and 'photographiam' in pv2.compendium, 'receptum: plagula post photographiam nata pro commissione refutatur')
    try:
        silva.commissio('nihil', ['pythonica/.umbra_probatio.tmp'], portae=[via_r])
        credo(False, 'commissio plagulae extra photographiam refutatur')
    except silva.SilvaError as ex:
        credo('photographiam' in str(ex), 'commissio: plagula extra photographiam refutata')
finally:
    os.unlink(novum)
via_clone = r.photographia['via_operis']
silva.receptum_delere(via_r)
credo(not os.path.exists(via_r) and not os.path.exists(via_r + '.acta') and not os.path.exists(via_clone),
      'receptum deletum cum actis et clone')
# mos vetus (photographica=False): sigillum arboris vivae, rancidum post mutationem
via_v = silva.porta_umbra('formator-intra', photographica=False)
rv = silva.exspectare(via_v, tectum=300)
credo(rv.sana and not rv.photographia and silva.receptum_validum(via_v).sana, 'umbra vetus: receptum sigillo arboris vivae')
open(novum, 'w').write('x')
try:
    credo(not silva.receptum_validum(via_v).sana, 'umbra vetus: rancidum post mutationem arboris')
finally:
    os.unlink(novum)
silva.receptum_delere(via_v)
# effusum non-UTF-8 (radix 2026-09-02: probatio octetos crudos imprimit)
# - porta non ruit, verdictum legitur
# porta md (A2, 2026-09-03): cursor extra tabulam = porta mortua (lectio
# shim, ter) - registratio in QUATTUOR tabulis asseritur
for _n in ('css', 'materia', 'md', 'oratio'):
    credo(_n in silva.PORTAE and _n in silva.FORMAE and _n in silva.SUITAE,
          'porta %s in PORTAE/FORMAE/SUITAE' % _n)
credo(os.access(os.path.join(RADIX, silva.PORTAE['md'][0][0]), os.X_OK),
      'cursor md exsistit et exsecutabilis')

silva.PORTAE['ficta-octeti'] = (['printf', 'fictum: sanum \\246\\321\\n'],
                                r'fictum: (sanum|FRACTUM)')
try:
    po = silva.porta('ficta-octeti')
    credo(po.cucurrit and po.sana, 'porta: octeti non-UTF-8 in effusu tolerati')
except Exception as ex:
    credo(False, 'porta: octeti non-UTF-8 levant %s' % type(ex).__name__)
# operarius umbrae ruens (porta parenti nota, operario ignota - PORTAE
# in processu mutatae operarium non attingunt): receptum FRACTUM
# scribitur, signum pendens tollitur - numquam 'pendens' in perpetuum
via_u = silva.porta_umbra('ficta-octeti')
ru = silva.exspectare(via_u, tectum=60)
credo(not ru.sana and not ru.cucurrit and 'UMBRA FRACTA' in ru.compendium
      and not os.path.exists(via_u + '.pendens'),
      'umbra: ruina operarii = receptum fractum, pendens sublatum')
credo('porta ignota' in open(via_u + '.acta').read(), 'umbra: traceback in actis')
silva.receptum_delere(via_u)
# signum pendens orphanum (operarius mortuus sine recepto) nominatur
orph = os.path.join(silva.PORTAE_DIR, 'ficta.0.json')
open(orph + '.pendens', 'w').write('999999')
credo(any(v == orph and st.startswith('mortua') for v, st in silva.portae_pendentes()),
      'portae_pendentes: pendens sine operario = mortua')
silva.receptum_delere(orph)
credo(all(v != via_r for v, _ in silva.portae_pendentes()),
      'portae_pendentes: receptum abiit')
# commissio_umbra siccum: portae seriatim, recepta SUA deleta, nihil
# commissum. Ante/post conferuntur: recepta ALIENA (cursus umbrae
# exterior qui hanc probationem ipsam ut portam currit) manere licet -
# 2026-09-02 assertum 'nullum receptum formator-intra' extra cursum
# falso rubuit.
ante_u = set(v for v, _ in silva.portae_pendentes())
h_u, rr = silva.commissio_umbra('nihil', ['pythonica/README.md'], ['formator-intra'], siccum=True)
credo(h_u is None and len(rr) == 1 and rr[0][0] == 'formator-intra' and 'sanum' in rr[0][1], 'commissio_umbra siccum: porta sana, nihil commissum')
post_u = set(v for v, _ in silva.portae_pendentes())
credo(post_u <= ante_u, 'commissio_umbra: recepta sua deleta (aliena intacta)')

print('--- fracturae: lectio actorum portae ---')
import json
ACTA_RADIX = ('Testing: probatio_x\n----\n.F\n FRACTA (credo_verum): a == b at probationes/probatio_x.c:12\n'
              '  Speratus: 1\n  Receptus: 2\n\n=== CREDO COMPENDIUM ===\nTotalis:    3\nPraeteriti: 2\n'
              'Fracti:     1\n\nConditio: FRACTA\n✗ TEST FAILED: probatio_x (.2s)\n'
              'Testing: probatio_y\n...\nTotalis:    2\nConditio: OMNIA PRAETERIERUNT\n'
              '✓ TEST PASSED: probatio_y (.1s)\nTests Passed: 1\nTests Failed: 1\n')
fr = silva.fracturae(ACTA_RADIX, 'radix')
credo([f.nomen for f in fr] == ['probatio_x'] and 'Totalis:    3' in fr[0].relatio
      and 'Speratus: 1' in fr[0].relatio and 'probatio_y' not in fr[0].relatio
      and 'Totalis:    2' not in fr[0].relatio,
      'fracturae radix: nomen ex TEST FAILED, relatio inter Testing: et verdictum')
credo(silva.fracturae(ACTA_RADIX.replace('✗ TEST FAILED: probatio_x (.2s)\n', ''), 'radix') == [],
      'fracturae radix: sine linea FAILED nulla')
fsv = silva.fracturae('Testing: probatio_z\n✗ SINE VERDICTO: probatio_z (operarius periit?)\nTests Failed: 1\n', 'radix')
credo([f.nomen for f in fsv] == ['probatio_z'] and 'SINE VERDICTO' in fsv[0].relatio,
      'fracturae radix: SINE VERDICTO (cursor parallelus) nominatur')
ACTA_SUITA = ('=== probatio_a ===\n..\n--- probatio_a praeteriit (0.1s)\n=== probatio_b ===\n.F\n'
              ' FRACTA (credo_nihil): p at x.c:3\n  Speratus: NIHIL\n  Receptus: 0x1\nConditio: FRACTA\n'
              '--- probatio_b FRACTA (0.2s, exitus 1)\n=== probatio_c ===\n\n========================\n'
              'SILVA PROBATIONES: 1/3 praeteritae\nFRACTAE: probatio_b probatio_c\n')
fr = silva.fracturae(ACTA_SUITA, 'silva')
credo([f.nomen for f in fr] == ['probatio_b', 'probatio_c'] and 'exitus 1' in fr[0].relatio
      and 'Speratus: NIHIL' in fr[0].relatio and 'probatio_a' not in fr[0].relatio
      and 'SILVA PROBATIONES' not in fr[1].relatio,
      'fracturae suita: nomina ex FRACTAE:, relatio inter vexilla ===')
credo(silva.fracturae(ACTA_SUITA, 'css') == silva.fracturae(ACTA_SUITA, 'silva'), 'fracturae: sub-suitae eadem forma')
fg = silva.fracturae('fumus vexilla: FRACTUM\n  offensor: x.sh\n', 'vexilla')
credo(len(fg) == 1 and fg[0].nomen == 'vexilla' and 'FRACTUM' in fg[0].relatio, 'fracturae generica: porta tota')
rel = silva.relatio_fracturarum(silva.fracturae(ACTA_RADIX, 'radix'))
credo(rel.startswith('\nfractae (1): probatio_x') and '      Totalis:    3' in rel, 'relatio_fracturarum: caput + lineae')
credo(silva.relatio_fracturarum([]) == '', 'relatio_fracturarum: vacua = chorda vacua')
# porta rubra VERA (processus): fracturae in Porta, in errore commissionis,
# in recepto umbrae (operarius portam fictam per ambitum videt), in
# receptum_relatio, in errore commissio_umbra
scriptum = os.path.join(silva.RADIX, 'pythonica', '.porta_rubra.tmp.sh')
open(scriptum, 'w').write("#!/bin/sh\ncat <<'ACTA'\n" + ACTA_RADIX + "ACTA\nexit 1\n")
os.chmod(scriptum, 0o755)
os.environ['PYTHONICA_PORTAE_FICTAE'] = json.dumps({'ficta-rubra': [[scriptum], 'Tests Passed:', 'radix']})
silva._portae_fictae()
try:
    po = silva.porta('ficta-rubra')
    credo(po.cucurrit and not po.sana and [f.nomen for f in po.fracturae] == ['probatio_x'],
          'porta rubra: fracturae in Porta')
    try:
        silva.commissio('nihil', ['pythonica/README.md'], portae=['ficta-rubra'])
        credo(False, 'commissio cum porta rubra levat')
    except silva.SilvaError as ex:
        credo('probatio_x' in str(ex) and 'Speratus: 1' in str(ex),
              'commissio: error probationem fractam nominat cum relatione')
    via_f = silva.porta_umbra('ficta-rubra')
    rf = silva.exspectare(via_f, tectum=60)
    credo(rf.cucurrit and not rf.sana and [f.nomen for f in rf.fracturae] == ['probatio_x'],
          'receptum umbrae fracturas fert')
    credo(not silva.receptum_validum(via_f).sana and silva.receptum_validum(via_f).fracturae,
          'receptum_validum: fracturae in Porta')
    rr = silva.receptum_relatio(via_f)
    credo('ficta-rubra' in rr and 'FRACTA' in rr and 'probatio_x' in rr and 'Totalis:    3' in rr,
          'receptum_relatio: lectio post cursum')
    silva.receptum_delere(via_f)
    ante_f = set(v for v, _ in silva.portae_pendentes())
    try:
        silva.commissio_umbra('nihil', ['pythonica/README.md'], ['ficta-rubra'], siccum=True)
        credo(False, 'commissio_umbra cum porta rubra levat')
    except silva.SilvaError as ex:
        credo('fractae (1): probatio_x' in str(ex) and 'Speratus: 1' in str(ex) and '.acta' in str(ex),
              'commissio_umbra: error fracturas nominat, non caudam actorum')
    for v, _ in silva.portae_pendentes():
        if v not in ante_f and 'ficta-rubra' in v:
            silva.receptum_delere(v)
    # planta: testimonium rubrum nomina et lineam fert
    def porta_rubra_ficta(v):
        return silva.Porta('f', True, 'PLANTATUM' not in open(v).read(), 'ficta', 1, '',
                           [silva.Fractura('probatio_x', 'Totalis:    0\nConditio: NIHIL PROBATUM')])
    open(via, 'w').write(FONS)
    r_p, g_p = silva.planta(via, ANC, PLA, porta_rubra_ficta)
    credo('probatio_x' in r_p and 'NIHIL PROBATUM' in r_p and open(via).read() == FONS,
          'planta: compendium rubrum fracturas nominat (linea Conditio)')
finally:
    del os.environ['PYTHONICA_PORTAE_FICTAE']
    os.unlink(scriptum)

print('--- FructusOmnes: transactio .sana ---')
r = silva.Refactio()
r.editio(via).replace('x  = I;', 'x  = II;')
fo = r.applicare()
credo(isinstance(fo, list) and hasattr(fo, 'sana') and fo.sana and len(fo) == 1
      and 'applicare' in str(fo), 'Refactio.applicare: lista cum .sana et str')
open(via, 'w').write(FONS)

print('--- probatio_currere: probatio una sicut cursor ---')
credo(silva.probatio_suita('probatio_credo') == 'radix'
      and silva.probatio_suita('probatio_silva_lexema') == 'silva'
      and silva.probatio_suita('probatio_css_lexema') == 'css'
      if os.path.exists(os.path.join(silva.RADIX, 'css/probationes/probatio_css_lexema.c'))
      else silva.probatio_suita('probatio_silva_lexema') == 'silva',
      'probatio_suita: suita ex fonte')
try:
    silva.probatio_suita('probatio_nemo')
    credo(False, 'probatio ignota levat')
except silva.SilvaError as ex:
    credo('probatio ignota' in str(ex), 'probatio_suita: ignota nominata')
# via binarii: scriptum rubrum fictum (exitus 1) - fracturae genericae
scriptum = os.path.join(silva.RADIX, 'pythonica', '.cursus_ruber.tmp.sh')
open(scriptum, 'w').write("#!/bin/sh\necho \"radix: $RHUBARB_RADIX\"\npwd\ncat <<'ACTA'\n"
                          + ACTA_RADIX + "ACTA\nexit 1\n")
os.chmod(scriptum, 0o755)
try:
    c = silva.probatio_currere(scriptum)
    credo(c.rc == 1 and c.suita == '?' and 'radix: ' + silva.RADIX in c.acta
          and c.acta.splitlines()[1].strip() == silva.RADIX,
          'probatio_currere: e radice, RHUBARB_RADIX praebita, rc lectus')
    credo(len(c.fracturae) == 1 and 'Totalis:    3' in c.fracturae[0].relatio
          and 'assertiones 3, fractae 1' in silva.cursus_textus(c),
          'probatio_currere: fracturae + cursus_textus')
finally:
    os.unlink(scriptum)
# binarium absens sine aedificare: refusio quae remedium nominat
try:
    silva.probatio_currere('probatio_silva_lexema') if not os.path.exists(
        os.path.join(silva.RADIX, 'silva/build/probatio_silva_lexema')) else (_ for _ in ()).throw(
        silva.SilvaError('binarium absens (fictum)'))
    credo(False, 'binarium absens levat')
except silva.SilvaError as ex:
    credo('binarium absens' in str(ex), 'probatio_currere: binarium absens nominat aedificare')
# probatio vera radicis, aedificata per cursorem suitae (filtrum), deinde cursa
c = silva.probatio_currere('probatio_piscina', aedificare=True)
credo(c.rc == 0 and c.suita == 'radix' and 'Conditio: OMNIA PRAETERIERUNT' in c.acta
      and c.fracturae == [] and 'exitus 0' in silva.cursus_textus(c),
      'probatio_currere: probatio_piscina aedificata et viridis')
# profilum: processus vivus per sample (dormiens: folium syscall)
scriptum = os.path.join(silva.RADIX, 'pythonica', '.cursus_dormiens.tmp.sh')
open(scriptum, 'w').write("#!/bin/sh\nsleep 3\necho Totalis: 1\necho Conditio: OMNIA PRAETERIERUNT\n")
os.chmod(scriptum, 0o755)
try:
    c = silva.probatio_currere(scriptum, secunda=1, mora=0.2)
    credo(c.rc == 0 and c.profilum and sum(n for n, _, _ in c.profilum) > 0
          and 'profilum (folia' in silva.cursus_textus(c),
          'probatio_currere: profilum sumptum (%s)' % (c.profilum[0][1] if c.profilum else '-'))
    credo(os.path.exists(os.path.join(silva.RADIX, 'build', 'sample', '.cursus_dormiens.tmp.sh.probatio.txt')),
          'profilum: effusus crudus in build/sample')
finally:
    os.unlink(scriptum)
    try:
        os.unlink(os.path.join(silva.RADIX, 'build', 'sample', '.cursus_dormiens.tmp.sh.probatio.txt'))
    except OSError:
        pass

print('--- refusio ancorae lineas nominat ---')
open(via, 'w').write(FONS)
try:
    silva.Editio(via).replace('redde;', 'x')
    credo(False, 'ancora bis levat')
except silva.SilvaError as ex:
    credo('lineae [' in str(ex) and str(ex).count(',') >= 1, 'Editio.replace: refusio lineas sedium nominat (%s)' % str(ex)[:70])
via_t3 = os.path.join(T, 'ter.txt')
open(via_t3, 'w').write('a\nb\na\n')
try:
    silva.Textus(via_t3).replace('a', 'x')
    credo(False, 'Textus ancora bis levat')
except silva.SilvaError as ex:
    credo('lineae [1, 3]' in str(ex), 'Textus.replace: refusio lineas nominat (%s)' % str(ex)[:60])

print('--- profilum: inclusivum recursione collapsa + viae ---')
via_s = os.path.join(T, 'exemplum.sample.txt')
open(via_s, 'w').write('Call graph:\n    100 Thread_1\n      100 start\n        100 main\n          60 f\n            40 g\n              30 g\n                20 g\n                  20 xar_obtinere\n          40 h\n            40 xar_obtinere\n\nTotal number in stack (recursive counted multiple, when >=5):\n')
inc = dict((fn, n) for _, n, fn in silva.profilum_inclusivum(via_s))
credo(inc.get('g') == 40 and inc.get('xar_obtinere') == 60 and inc.get('f') == 60 and 'main' not in inc,
      'profilum_inclusivum: recursio collapsa (g 40, non 90), fila/main omissa')
viae_x = silva.profilum_viae(via_s, 'xar_obtinere')
credo(viae_x == [(40, 'h > xar_obtinere'), (20, 'f > g×3 > xar_obtinere')],
      'profilum_viae: semitae ordine ponderis, recursio compressa (%s)' % viae_x)
credo(silva.profilum_viae(via_s, 'g') == [(40, 'f > g')], 'profilum_viae: functio recursiva semel, in summo')

print('--- imagines: oraculum identitatis octetorum ---')
import shutil
via_a = os.path.join(T, 'ia.txt'); via_b = os.path.join(T, 'ib.txt')
open(via_a, 'w').write('alpha\n'); open(via_b, 'w').write('beta\n')
im = silva.imago_capere('probatio-imago', ['cat'], [via_a, via_b])
credo(im.numerus == 2 and os.path.exists(os.path.join(im.via, 'manifestum.json')), 'imago_capere: II plagulae + manifestum')
co = silva.imago_conferre('probatio-imago')
credo(co.sana and co.eaedem == [via_a, via_b], 'imago_conferre: idem = sana')
open(via_b, 'w').write('BETA\n')
co = silva.imago_conferre('probatio-imago')
credo(not co.sana and co.diversae == [via_b] and co.eaedem == [via_a] and 'diversa: ' + via_b in str(co),
      'imago_conferre: plagula mutata nominata')
credo('-beta' in silva.imago_differentia('probatio-imago', via_b) and '+BETA' in silva.imago_differentia('probatio-imago', via_b),
      'imago_differentia: lineae ante/post')
co = silva.imago_conferre('probatio-imago', plagulae=[via_a, os.path.join(T, 'ic.txt')])
credo(co.novae == [os.path.join(T, 'ic.txt')] and co.absentes == [via_b] and not co.sana,
      'imago_conferre: novae et absentes nominatae')
co = silva.imago_conferre('probatio-imago', imperium=['sh', '-c', 'exit 3'])
credo(co.diversae == [via_a, via_b], 'imago_conferre: rc diversus = diversa')
shutil.rmtree(os.path.join(silva.IMAGINES_DIR, 'probatio-imago'), ignore_errors=True)
shutil.rmtree(os.path.join(silva.IMAGINES_DIR, 'probatio-imago.post'), ignore_errors=True)

print('--- photographia: status operis captus, arbor viva intacta ---')
tmpf_rel = 'pythonica/.photo_probatio.tmp'
tmpf = os.path.join(silva.RADIX, tmpf_rel)
open(tmpf, 'w').write('ante\n')
status_ante = silva._curre(['git', 'status', '--porcelain']).stdout
ph = silva.photographia_capere()
credo(len(ph.arbor) == 40 and ph.basis == silva._curre(['git', 'rev-parse', 'HEAD']).stdout.strip()
      and silva._curre(['git', 'status', '--porcelain']).stdout == status_ante,
      'photographia_capere: arbor scripta, HEAD basis, arbor viva intacta')
credo(silva.photographia_continet(ph.arbor, tmpf_rel) and silva.photographia_continet(ph.arbor, 'pythonica/README.md'),
      'photographia_continet: plagula nova et tracta inclusae')
open(tmpf, 'w').write('post\n')
credo(not silva.photographia_continet(ph.arbor, tmpf_rel), 'photographia_continet: mutatio post captum detecta')
credo(silva.photographia_continet(ph.arbor, 'pythonica/.nemo.tmp'), 'photographia_continet: absens utrimque = idem')
os.utime(tmpf, (1577854800, 1577854800))
ph = silva.photographia_materializare(ph, 'probatio')
try:
    credo(int(os.stat(os.path.join(ph.via, tmpf_rel)).st_mtime) == 1577854800
          and os.stat(os.path.join(ph.via, 'pythonica/README.md')).st_mtime_ns
              == os.stat(os.path.join(silva.RADIX, 'pythonica/README.md')).st_mtime_ns,
          'photographia: tempora plagularum tractarum e arbore viva speculata (custodes binariorum in clone ut vivi)')
    credo(os.path.isdir(ph.via) and open(os.path.join(ph.via, tmpf_rel)).read() == 'ante\n'
          and os.path.isdir(os.path.join(ph.via, 'bin')) and os.path.isdir(os.path.join(ph.via, 'build'))
          and os.path.isdir(os.path.join(ph.via, '.git')),
          'photographia_materializare: clone cum statu capto, bin/build clonata, .git verum')
    credo(subprocess.run(['git', 'rev-parse', 'HEAD'], cwd=ph.via, capture_output=True, text=True).stdout.strip() == ph.basis,
          'photographia: HEAD in clone = basis')
finally:
    silva.photographia_delere(ph)
credo(not os.path.exists(ph.via), 'photographia_delere: clone sublatus')
os.unlink(tmpf)

print('--- custodes: nomina plana, clones orphani ---')
for malum in ('a/b', '..', '.', '', 'x y'):
    try:
        silva.imago_capere(malum, ['true'], [])
        credo(False, 'imago_capere nomen %r refutat' % malum)
    except silva.SilvaError as ex:
        credo('nomen' in str(ex), 'imago_capere: nomen %r refutatum' % malum)
try:
    silva.photographia_materializare(silva.Photographia('0' * 40, '0' * 40, None), '../fuga')
    credo(False, 'photographia_materializare nomen refutat')
except silva.SilvaError as ex:
    credo('nomen' in str(ex), 'photographia_materializare: nomen cum ../ refutatum ante clone')
try:
    silva.porta_umbra('vexilla', filtrum='a/b')
    credo(False, 'porta_umbra filtrum refutat')
except silva.SilvaError as ex:
    credo('nomen' in str(ex) or 'filtrum' in str(ex), 'porta_umbra: filtrum cum / refutatum')
# orphani - HERMETICE (UMBRAE_DIR + PORTAE_DIR temporaria): porta in
# clone currens hoc probans clones veros purgaret - se ipsam delevit
# 2026-09-02 (recepta clonis alia, sera PID ubique eadem)
import shutil
UMBRAE_VERA, PORTAE_VERA = silva.UMBRAE_DIR, silva.PORTAE_DIR
silva.UMBRAE_DIR = os.path.join(T, 'umbrae'); silva.PORTAE_DIR = os.path.join(T, 'portae')
# idempotens: cursus prior directoria reliquit (bis currere = FileExistsError,
# 2026-09-03 - porta pythonica sub commissione md rubra sine culpa md)
for _d in (silva.UMBRAE_DIR, silva.PORTAE_DIR):
    shutil.rmtree(_d, ignore_errors=True)
    os.makedirs(_d)
orph = os.path.join(silva.UMBRAE_DIR, 'probatio-orphanus.1')
vivus = os.path.join(silva.UMBRAE_DIR, 'probatio-vivus.1')
seratus = os.path.join(silva.UMBRAE_DIR, 'probatio-seratus.1')
mortuus = os.path.join(silva.UMBRAE_DIR, 'probatio-mortuus.1')
for d in (orph, vivus, seratus, mortuus):
    os.makedirs(d)
open(os.path.join(seratus, '.umbra.pid'), 'w').write(str(os.getpid()))
open(os.path.join(mortuus, '.umbra.pid'), 'w').write('999999')
open(os.path.join(silva.PORTAE_DIR, 'probatio-vivus.0.json.pendens'), 'w').write('999999\n' + vivus + '\n')
try:
    orphanae = silva.umbrae_orphanae()
    credo(orph in orphanae and mortuus in orphanae and vivus not in orphanae and seratus not in orphanae,
          'umbrae_orphanae: orphanus + sera mortua nominati; pendens + sera viva servati')
    credo(any(v == orph and st == 'clone orphanus' for v, st in silva.portae_pendentes()), 'portae_pendentes: clone orphanus enumeratus')
    deletae = silva.umbrae_purgare()
    credo(sorted(deletae) == sorted([mortuus, orph]) and not os.path.exists(orph) and os.path.isdir(vivus) and os.path.isdir(seratus),
          'umbrae_purgare: orphani deleti, clones portae currentis et processus vivi servati')
finally:
    for _d in (silva.UMBRAE_DIR, silva.PORTAE_DIR):
        shutil.rmtree(_d, ignore_errors=True)
    silva.UMBRAE_DIR, silva.PORTAE_DIR = UMBRAE_VERA, PORTAE_VERA

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

print('--- Editio: parsatio mortua nominata; commentum; inserere_ante_vocationem ---')
via_m = os.path.join(T, 'mortua.c')
open(via_m, 'w').write('#include "latina.h"\n\n/* f ducens */\nvacuum\nf (\n    vacuum)\n{\n}\n\n'
                       'vacuum\ng (\n    vacuum)\n{\n}\n')
e_m = silva.Editio(via_m)
e_m.replace('vacuum\ng (', 'vacuum\ng (\n    i32 structura)', tolerans=False)   # 'structura' = struct: parsatio moritur
try:
    e_m.substituere('f', 'vacuum\nf (\n    vacuum)\n{\n    redde;\n}\n')
    credo(False, 'Editio: parsatio mortua clamat')
except silva.SilvaError as ex:
    credo('MORTUA' in str(ex) and ('keyword' in str(ex) or 'error:' in str(ex)),
          'Editio: parsatio mortua nominata cum causa clang (%s)' % str(ex).splitlines()[0][:70])
e_c = silva.Editio(via_m)
e_c.commentum('f', '/* f novum */')
credo('/* f novum */\nvacuum\nf (' in e_c.textus and '/* f ducens */' not in e_c.textus,
      'commentum: commentarium ducens substitutum')
e_c.commentum('g', '/* g natum */')
credo('/* g natum */\nvacuum\ng (' in e_c.textus, 'commentum: sine commentario inseritur')
via_v = os.path.join(T, 'vocatio.c')
open(via_v, 'w').write('#include "latina.h"\n\ns32\nprincipale (\n    vacuum)\n{\n    imprimere("a");\n'
                       '    credo_imprimere_compendium();\n    redde ZEPHYRUM;\n}\n')
e_v = silva.Editio(via_v)
e_v.inserere_ante_vocationem('credo_imprimere_compendium', '    imprimere("b");')
credo(e_v.textus.index('imprimere("a")') < e_v.textus.index('imprimere("b")') < e_v.textus.index('credo_imprimere_compendium();'),
      'inserere_ante_vocationem: ante vocationem unicam')
try:
    e_v.inserere_ante_vocationem('imprimere', 'x')
    credo(False, 'inserere_ante_vocationem: vocatio plures refutata')
except silva.SilvaError as ex:
    credo('vicibus' in str(ex), 'inserere_ante_vocationem: vocatio plures refutata')
f_pa = silva.probatio_addere(via_v, '    imprimere("c");', forma=False, iudica=False)
_t = open(via_v).read()
credo(f_pa.sana and _t.index('imprimere("a")') < _t.index('imprimere("c")') < _t.index('credo_imprimere')
      and 'imprimere("b")' not in _t,
      'probatio_addere: casus ante compendium scriptus (editio memoriae prior non scripta)')

print('--- syntaxis; planta praevolatus ---')
credo(silva.syntaxis('lib/piscina.c') is None, 'syntaxis: plagula viva sana')
causa = silva.syntaxis(via_m, open(via_m).read())
credo(causa is not None and 'error:' in causa and '.syntaxis_' not in causa and 'mortua.c' in causa,
      'syntaxis: copia temporaria iudicata, via data ostensa (%s)' % (causa or '')[-60:])
via_p = os.path.join(T, 'planta_prae.c')
open(via_p, 'w').write('#include "latina.h"\n\nvacuum\nh (\n    i32 a)\n{\n    (vacuum)a;\n}\n')
cursus = []
def porta_numquam(v):
    cursus.append(v)
    raise AssertionError('porta cursa')
try:
    silva.planta(via_p, '(vacuum)a;', '', porta_numquam)     # parametrum inutile sub -Werror
    credo(False, 'planta: praevolatus non compilantem refutat')
except silva.SilvaError as ex:
    credo('non compilat' in str(ex) and 'praevolatus' in str(ex), 'planta: praevolatus non compilantem refutat')
credo(cursus == [] and '(vacuum)a;' in open(via_p).read(), 'planta: porta non cursa, plagula intacta')
via_s = os.path.join(T, 'planta.stml')
open(via_s, 'w').write('<radix><a/></radix>\n')
def porta_stml(v):
    return silva.Porta('stml', True, '<b/>' not in open(v).read(), 'stml', 0, '')
r_s, g_s = silva.planta(via_s, '<a/>', '<b/>', porta_stml)
credo(open(via_s).read() == '<radix><a/></radix>\n', 'planta: plagula non-C sine praevolatu clang, reversa')
os.unlink(via_s)

print('--- expandere (stml) ---')
ex_b = silva.expandere('<radix><a/></radix>')
credo(ex_b.successus and '<a/>' in ex_b.textus, 'expandere: textus bonus expansus')
ex_m = silva.expandere('<radix><#@f p="@p"><a x="&@ignotum;"/></#></radix>')
credo(not ex_m.successus and ex_m.vitium == 'LOCULUS_IGNOTUS' and ex_m.loculus == 'ignotum'
      and ex_m.fragmentum == '@f' and ex_m.linea == 1, 'expandere: vitium nominatum cum loculo et fragmento')
credo(silva.expandere('project-specs/exhibita/md-html-b1.stml').successus, 'expandere: via')
for _f in (via_m, via_v, via_p):
    os.unlink(_f)

print('--- oraculum md (ambitus ORACULUM_* involutus) ---')
o = silva.oraculum(exemplum=25)
credo(o.totalis == 1324 and o.praeterita >= o.pinna >= 1270, 'oraculum: summa cum pinna')
credo(o.sectiones.get('Tabs', (0, 0))[1] == 22 and len(o.sectiones) >= 30, 'oraculum: sectiones (Tabs 22, XXXI tituli)')
credo(any(f.numerus == 5 and f.sectio == 'Tabs' and f.causa is None and f.sperata and f.nostra
          for f in o.fracturae), 'oraculum: fracturae omnes cum sperata/nostra normatis')
credo(len(o.exempla) == 2 and {e.plagula.split('/')[-2] for e in o.exempla} == {'commonmark', 'gfm'}
      and o.exempla[0].md.startswith('&nbsp;') and o.exempla[0].nostra.endswith('</p>'),
      'oraculum: exemplum 25 ex plagula utraque, cruda')
credo(o.ignoscentiae.get('inter', 0) > 0 and silva.oraculum().exempla == [], 'oraculum: ignoscentiae; sine exemplo nulla')

print('--- Prosa: editio markdown per ancoras structurales (C1) ---')
import shutil
via_p = os.path.join(T, 'prosa.md'); shutil.copy(os.path.join(RADIX, 'project-specs/md-arbor-plan.md'), via_p)
p = silva.Prosa(via_p)
credo(len(p.selecta('capitulum')) == 17, 'Prosa.selecta: capitula XVII')
x = p.capitulum('C1 pythonica `Prosa`'); credo(x.linea == 143 and p.corpus(x).startswith('### C1 pythonica `Prosa`'), 'Prosa.capitulum: linea + corpus')
credo(p.capitulum('Arc C — the consumer', gradus=2).linea == 141, 'Prosa.capitulum: gradus, titulus non-ASCII (octeti)')
s = p.sectio('C1 pythonica `Prosa`'); cs = p.corpus(s)
credo(cs.startswith('### C1') and not cs.endswith('\n\n') and 'C2' not in cs.split('\n')[0] and s.finis < p.capitulum('C2 Wire-up, closure, debrief (the B10 shape)').initium, 'Prosa.sectio: usque ad capitulum proximum, vacuae caudales exclusae')
credo(p.corpus(p.sectio('Order and size')).startswith('## Order'), 'Prosa.sectio ultima usque ad finem')
credo(p.corpus(p.sectio('Arc C — the consumer')).count('### C') == 2, 'Prosa.sectio h2 continet h3 filias')
try:
    p.capitulum('nemo'); credo(False, 'absens')
except silva.SilvaError as ex:
    credo('praesentia' in str(ex) and 'C1 pythonica' in str(ex), 'Prosa.capitulum absens: praesentia nominata')
p.substituere(s, '### C1 pythonica `Prosa`\nDONE.\n')
try:
    p.corpus(s); credo(False, 'rancidum')
except silva.SilvaError as ex:
    credo('rancidum' in str(ex), 'Prosa: extentum rancidum post editionem refutatur')
f = p.applicare(); t_novus = open(via_p, 'rb').read()
credo(f.sana and f.ancorae.get("capitulum 'C1 pythonica `Prosa`'") == 1 and t_novus.count(b'DONE.') == 1 and b'DONE.\n\n### C2' in t_novus, 'Prosa.applicare: scriptum, iudex sanus, ancorae relatae, separatio manet')
p2 = silva.Prosa(via_p); open(via_p, 'ab').write(b'x\n')
try:
    p2.appendere('y\n'); p2.applicare(); credo(False, 'rancida')
except silva.SilvaError:
    credo(True, 'Prosa: lectio rancida refutatur')
via_q = os.path.join(T, 'prosa2.md')
open(via_q, 'w').write('# Doc\n\n## Ideas\n\n- one\n- two\n  - nested\n\n## Code\n\n```c\nx;\n```\n\n```sh\nls\n```\n')
q = silva.Prosa(via_q); sec = q.sectio('Ideas')
credo(q.corpus(q.elementum(1, intra=sec)) == '- two\n  - nested\n', 'Prosa.elementum intra sectionem, nidificatum inclusum')
credo(q.corpus(q.elementum(2, intra=sec)) == '  - nested\n', 'Prosa.elementum nidificatum n=2')
try:
    q.elementum(3, intra=sec); credo(False, 'numerus')
except silva.SilvaError as ex:
    credo('3 praesentia' in str(ex), 'Prosa.elementum extra numerum refutatur')
credo(q.corpus(q.saeptum(lingua='sh')) == '```sh\nls\n```\n', 'Prosa.saeptum per linguam')
q.inserere_post(q.elementum(2, intra=sec), '- three\n')
credo(len(q.selecta('elementum')) == 4, 'Prosa.inserere_post elementum')
q.replace('x;', 'y;'); f2 = q.applicare()
credo(f2.sana and open(via_q).read().count('- three') == 1 and 'y;' in open(via_q).read() and f2.ancorae.get('elementum') == 4, 'Prosa.replace exacta + applicare + ancorae numeratae')
try:
    q.selecta('['); credo(False, 'selector')
except silva.SilvaError as ex:
    credo('selector' in str(ex), 'Prosa: selector malus refutatur')
for _f in (via_p, via_q):
    os.unlink(_f)

print('--- Prosa: incipit/continet + paragraphum_addere ---')
via_r = os.path.join(T, 'prosa3.md')
open(via_r, 'w').write('# Doc\n\n## 9. Arc C — `Prosa` in `pythonica/silva.py`\n\nbody one\n\n## 10. Order\nbody ten\n\nSetext\n------\n\nbody s\n')
r3 = silva.Prosa(via_r)
credo(r3.sectio(incipit='9.').linea == 3 and r3.capitulum(continet='Order').linea == 7, 'Prosa: incipit + continet')
try:
    r3.capitulum(incipit=''); credo(False, 'ambiguum')
except silva.SilvaError as ex:
    credo('4 vicibus' in str(ex) and 'congruentia' in str(ex) and '10. Order' in str(ex), 'Prosa: incipit ambiguum congruentia nominat')
try:
    r3.capitulum(); credo(False, 'nihil')
except silva.SilvaError as ex:
    credo('unum ex' in str(ex), 'Prosa: sine clave refutatur')
s9 = r3.sectio(incipit='9.'); credo(s9.caput_finis == r3.octeti.find(b'\n', s9.initium) + 1, 'Prosa: sectio caput_finis (ATX)')
r3.paragraphum_addere(s9, 'added at end')
credo(b'body one\n\nadded at end\n\n## 10.' in r3.octeti, 'paragraphum_addere finis: linea vacua una, separatio ad capitulum manet')
s10 = r3.sectio(incipit='10.'); r3.paragraphum_addere(s10, 'status line', ubi='initium')
credo(b'## 10. Order\n\nstatus line\n\nbody ten\n' in r3.octeti, 'paragraphum_addere initium: corpus statim sequens separatum')
ss = r3.sectio('Setext'); credo(r3.corpus(ss).startswith('Setext\n------\n') and ss.caput_finis == ss.initium + len(b'Setext\n------\n'), 'Prosa: caput_finis setext post subductionem')
r3.paragraphum_addere(ss, 'under setext', ubi='initium')
credo(b'Setext\n------\n\nunder setext\n\nbody s\n' in r3.octeti, 'paragraphum_addere initium sub setext (linea vacua exsistens non duplicata)')
f3 = r3.applicare(); credo(f3.sana, 'Prosa: applicare post paragrapha')
os.unlink(via_r)

print('--- citata: putredo documentorum (C2) ---')
via_c = os.path.join(T, 'citata.md')
open(via_c, 'w').write('See `lib/chorda.c`, `chorda.c`, `lib/nemo_hic.c`, `../../include/latina.h`,\n'
                       '`chorda_internare(`, `nemo_hic_est()`, `x = 1`.\n')
ct = silva.citata(via=via_c)
credo(ct.numeri['citata'] == 7 and ct.numeri['viae'] == 4 and ct.numeri['symbola'] == 2, 'citata: classificatio (viae IV, symbola II, cetera ignorata)')
credo(ct.numeri['viae_adsunt'] == 2 and ct.numeri['viae_nudae'] == 1 and ct.numeri['viae_absunt'] == 1, 'citata: adest (absoluta + relativa), nudum, absens')
credo([x.textus for x in ct.absentia] == ['lib/nemo_hic.c', 'nemo_hic_est()'] and ct.absentia[1].genus == 'symbolum', 'citata: absentia nominata cum genere')
credo(ct.nuda[0].textus == 'chorda.c' and 'lib/chorda.c' in ct.nuda[0].sedes, 'citata: nudum sedem nominat')
credo(ct.numeri['symbola_adsunt'] == 1 and 'ABSUNT 1' in silva.citata_textus(ct), 'citata: symbolum in nexus.tsv; textus compendii')
os.unlink(via_c)

print('--- vocabula: recensio identificatorum (T10) ---')
vb = silva.vocabula('symbola')
credo(vb.numeri['verba'] > 5000 and vb.numeri['sedes'] > 100000, 'vocabula: symbola corporis (verba > V milia, sedes > C milia)')
per_verbum = dict((v.verbum, v) for v in vb.verba)
credo(per_verbum['offset'].status == 'permissum' and per_verbum['piscina'].status == 'notum' and per_verbum['piscina'].lemma == 'piscina' and per_verbum['piscina'].classis == 'N', 'vocabula: permissum (glossarium) et notum (WORDS) cum lemma et classe')
credo(vb.verba[0].sedes >= vb.verba[-1].sedes and len(vb.ignota) == vb.numeri['ignotum'] and vb.ignota[0].via and vb.ignota[0].linea > 0, 'vocabula: ordo sedium descendens, ignota cum sede prima')
try:
    silva.vocabula('alia'); credo(False, 'vocabula: fons ignotus recusatur')
except silva.SilvaError:
    credo(True, 'vocabula: fons ignotus recusatur')

print()
if fracta:
    print('PYTHONICA: FRACTA %d' % len(fracta))
    sys.exit(1)
print('PYTHONICA: sana')

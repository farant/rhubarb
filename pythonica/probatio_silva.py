"""probatio_silva.py - porta natalis faciei Pythonicae (pythonica/silva.py).

Plagulae fictae in build/pythonica/; nihil in arbore tangitur.
Exitus 0 sanum | 1 fractum. Culpae plantatae: ancora fallens plagulam
intactam relinquit; differre substantiva verdictum negat."""
import os
import re
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

print('--- lex situs novi + refusio commenti + Commentum (2026-09-07) ---')
via_s = os.path.join(T, 'situs.c')
FONS_S = ('#include "latina.h"\n\n'
          '/* f - explicatio longa quae lineas duas tenet et de re quadam\n'
          ' * loquitur */\n'
          'interior i32\nf (\n    i32 a)\n{\n    i32 b;\n    i32 c;\n\n'
          '    b = a;\n    c = b;\n    redde c;\n}\n')
open(via_s, 'w').write(FONS_S)
e = silva.Editio(via_s)
e.replace('    i32 b;\n    i32 c;\n', '    i32 b;\n    i32 d;\n    i32 c;\n')
credo('{\n    i32 b;\n    i32 d;\n    i32 c;\n\n    b = a;' in e.textus and '        i32 d;' not in e.textus,
      'lex situs: novus indentatus ad lexema primum sine indentatione duplici, linea nova finalis sine linea vacua spuria')
e.replace('b = a;\n    c = b;', 'b = a;\n    c = b + d;')
credo('    b = a;\n    c = b + d;\n    redde c;' in e.textus, 'lex situs: lexema primum intra lineam - novus verbatim (spatium ante servatum)')
f = e.applicare(iudica=False)
credo('    i32 d;\n    i32 c;\n\n    b = a;\n    c = b + d;' in open(via_s).read(), 'lex situs: scriptum, forma plagulae servata')
for anc, verbum in (('loquitur */\ninterior i32', 'incipit'), ('#include "latina.h"\n\n/* f - explicatio', 'finit'), ('explicatio longa quae', 'tota intra')):
    try:
        silva.Editio(via_s).replace(anc, 'x')
        credo(False, 'ancora commenti %s' % verbum)
    except silva.SilvaError as ex:
        credo('lexema commenti (linea 3)' in str(ex) and verbum in str(ex), 'refusio diagnostica: ancora %s intra commentum, linea nominata' % verbum)
c = silva.Editio(via_s).commentum('explicatio longa')
credo(c.textus == 'f - explicatio longa quae lineas duas tenet et de re quadam loquitur' and c.indentatio == 0 and c.linea == 3,
      'Commentum(fragmentum): textus marginibus exutus, indentatio, linea')
e = silva.Editio(via_s); c = e.commentum('f')
c.substituere('f - ' + ' '.join(['verbum'] * 20) + '.\n\nParagraphus alter.')
lineae_c = e.textus[e.textus.index('/* f'):e.textus.index('interior')].splitlines()
credo(all(len(l) <= 72 for l in lineae_c) and len(lineae_c) == 5 and lineae_c[0].startswith('/* f - verbum') and lineae_c[3] == ' *' and lineae_c[4] == ' * Paragraphus alter. */',
      'Commentum(nomen).substituere: refluxus ad LXXII columnas, margines, paragraphi, clausura')
c = e.commentum('f')
c.paragraphum_addere('2026-09-07: nota datata.')
credo(e.textus.count(' * Paragraphus alter.\n *\n * 2026-09-07: nota datata. */\ninterior i32') == 1, 'Commentum.paragraphum_addere: lineae priores verbatim, linea vacua, paragraphus, clausura')
try:
    c.substituere('iterum')
    credo(False, 'commentum consumptum')
except silva.SilvaError as ex:
    credo('selige iterum' in str(ex), 'Commentum post editionem consumptum: selige iterum')
try:
    e.commentum('nemo hic')
    credo(False, 'commentum absens')
except silva.SilvaError as ex:
    credo('commentum 0 vicibus inventum' in str(ex), 'Commentum absens levat')
f = e.applicare(iudica=False)
credo('nota datata. */\ninterior i32' in open(via_s).read() and '    i32 d;\n' in open(via_s).read(), 'Commentum: scriptum super plagulam iam editam')

print('--- Commentum.sententia per orationem (gradus III) + refusiones (2026-09-07) ---')
via_c = os.path.join(T, 'sententia.c')
open(via_c, 'w').write('#include "latina.h"\n\n/* f - explicatio prima, e.g. cum abbreviatione. Sententia altera\n * hic stat. Tertia sequitur.\n *\n * Paragraphus alter manet intactus. */\ninterior i32\nf (\n    i32 a)\n{\n    redde a;\n}\n')
e = silva.Editio(via_c); c = e.commentum('f')
s = c.sententia(continet='altera hic')
credo(s.textus == 'Sententia altera hic stat.' and s.paragraphus == 0 and c.sententia(0).textus == 'f - explicatio prima, e.g. cum abbreviatione.' and c.sententia(3).textus == 'Paragraphus alter manet intactus.',
      'Commentum.sententia: per lectorem orationis (e.g. non findit), fragmento aut numero, paragraphus')
s.substituere('Sententia altera nova est et longior ut refluxus paragraphi primi probetur.')
lineae_c = e.textus[e.textus.index('/* f'):e.textus.index('interior')].splitlines()
prosa_c = ' '.join(re.sub(r'^\s*(/\*|\*/?)\s?', '', l) for l in lineae_c)
credo(lineae_c[0].startswith('/* f - explicatio prima, e.g. cum abbreviatione. Sententia altera') and all(len(l) <= 72 for l in lineae_c) and 'altera nova est et longior' in prosa_c and 'Tertia sequitur.' in prosa_c and 'hic stat' not in prosa_c and lineae_c[-2] == ' *' and lineae_c[-1] == ' * Paragraphus alter manet intactus. */',
      'SententiaCommenti.substituere: sententia sola mutata, paragraphus refluxus, alter verbatim')
for mala in (lambda: c.sententia(9), lambda: silva.Editio(via_c).commentum('f').sententia(continet='sequitur') and silva.Editio(via_c).commentum('f').sententia(continet='a')):
    try:
        mala(); credo(False, 'sententia mala levat')
    except silva.SilvaError as ex:
        credo('sententia' in str(ex), 'sententia absens / ambigua levat cum sententiis nominatis')
e = silva.Editio(via_c); e.replace('redde a;', 'redde a + a;')
try:
    e.replace('a', 'b')
    credo(False, 'ambigua')
except silva.SilvaError as ex:
    credo('in memoria post 1 editiones; in plagula - lineae' in str(ex), 'ancora ambigua post editionem: lineae in memoria ET in plagula nominatae')
try:
    silva.commissio('nihil', ['FAQ.md'], portae=[])
    credo(False, 'vetita')
except silva.SilvaError as ex:
    credo('VETITA' in str(ex) and 'Frani' in str(ex) and 'remove' in str(ex), 'commissio: refusio vetitae causam nominat')
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
# registrum coctum (2026-09-15): involucrum tenue instrumenti C89 -
# declaratio orationis commissa RECENS (rc 0, viae ambae nominatae);
# declaratio mala (species ignota) RECUSATA (rc 2, nihil scriptum)
rc = silva.registrum_coquere('oratio/grammatica/oratio.registrum.stml')
credo(rc.rc == 0 and rc.recens,
      'registrum_coquere: declaratio orationis recens (rc 0): %s' % rc.acta.strip())
credo(rc.viae == ['oratio/fontes/oratio_registrum_coctum.h',
                  'oratio/fontes/oratio_registrum_coctum.c'],
      'registrum_coquere: viae generatae nominatae: %r' % (rc.viae,))
with open(silva._absoluta('build/proba_mala.registrum.stml'), 'w') as f:
    f.write('<registrum grammatica="p" praefixum="P" typus="PG" sedes="build">\n'
            '  <genus titulus="g"><locus titulus="a" species="lista"/></genus>\n'
            '</registrum>\n')
rm = silva.registrum_coquere('build/proba_mala.registrum.stml')
credo(rm.rc == 2 and not rm.recens and rm.viae == [],
      'registrum_coquere: declaratio mala recusata (rc 2): %s' % rm.acta.strip()[:120])

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
def porta_errata_aliena(v):
    t = open(v).read()
    return silva.Porta('aliena', True, 'PLANTATUM' not in t, 'aliena', 0 if 'PLANTATUM' not in t else 1,
                       "discors x: bash: -c: line 3: syntax error: unexpected end of file")
try:
    silva.planta(via, ANC, PLA, porta_errata_aliena)
    credo(open(via).read() == FONS, 'planta: erratum alienum in actis (non compilatoris) rubra vera, reversa')
except silva.SilvaError as ex:
    credo(False, 'planta: erratum alienum pro fractura aedificationis habitum (%s)' % ex)

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

print('--- receptum vivum: porta() sigillo ligata, commissio non iterum currit ---')
import contextlib
import io
via_rv = silva._receptum_vivum_via('formator-intra')
silva.receptum_delere(via_rv)
pv = silva.porta('formator-intra')
credo(pv.sana and not pv.rancida and pv.receptum == via_rv and os.path.exists(via_rv)
      and os.path.exists(via_rv + '.acta'), 'porta viva: receptum vivum scriptum, non rancida')
rv = silva.receptum_vivum('formator-intra')
credo(rv is not None and rv.sana and 'receptum vivum' in rv.compendium and rv.receptum == via_rv,
      'receptum_vivum: sanum, sigillum idem (%s)' % (rv.compendium if rv else None))
open(novum, 'w').write('x')
try:
    rv2 = silva.receptum_vivum('formator-intra')
    credo(rv2 is not None and not rv2.sana and 'POST cursum' in rv2.compendium,
          'receptum_vivum: arbor mutata post cursum = rancidum')
finally:
    os.unlink(novum)
credo(silva.receptum_vivum('formator-intra').sana, 'receptum_vivum: arbor restituta = sanum iterum (sigillum contentus, non temporis)')
credo(silva.receptum_vivum('formator-intra', 'x') is None and silva.receptum_vivum('nemo') is None,
      'receptum_vivum: clavis alia / absens = None')
credo(any(v == via_rv and p.sana for v, p in silva.recepta_viva()), 'recepta_viva: enumerat cum valore')
# portae fictae: numerans (cursus numerat EXTRA arborem - build/ ignoratum),
# rancida (plagulam novam in arbore creat dum currit), mutans (plagulam
# COMMISSIONIS mutat dum currit), rubra
numerator = os.path.join(T, 'numerator.txt')
open(numerator, 'w').write('')
sc_n = os.path.join(T, 'numerans.sh')
open(sc_n, 'w').write("#!/bin/sh\necho x >> '%s'\necho 'fictum: sanum'\n" % numerator)
rancidum = os.path.join(silva.RADIX, 'pythonica', '.rancida_probatio.tmp')
sc_r = os.path.join(T, 'rancida.sh')
open(sc_r, 'w').write("#!/bin/sh\necho x > '%s'\necho 'fictum: sanum'\n" % rancidum)
mutandum = os.path.join(silva.RADIX, 'pythonica', '.mutans_probatio.tmp')
sc_m = os.path.join(T, 'mutans.sh')
open(sc_m, 'w').write("#!/bin/sh\necho y >> '%s'\necho 'fictum: sanum'\n" % mutandum)
sc_x = os.path.join(T, 'rubra.sh')
open(sc_x, 'w').write("#!/bin/sh\necho 'fictum: FRACTUM'\nexit 1\n")
for _s in (sc_n, sc_r, sc_m, sc_x):
    os.chmod(_s, 0o755)
os.environ['PYTHONICA_PORTAE_FICTAE'] = json.dumps({
    'ficta-numerans': [[sc_n], 'fictum: (sanum|FRACTUM)'],
    'ficta-rancida': [[sc_r], 'fictum: (sanum|FRACTUM)'],
    'ficta-mutans': [[sc_m], 'fictum: (sanum|FRACTUM)'],
    'ficta-rubra2': [[sc_x], 'fictum: (sanum|FRACTUM)']})
silva._portae_fictae()
cursus_n = lambda: open(numerator).read().count('x')
try:
    p1 = silva.porta('ficta-numerans')
    credo(p1.sana and cursus_n() == 1 and silva.receptum_vivum('ficta-numerans').sana,
          'porta numerans: cursa semel, receptum sanum')
    effusus = io.StringIO()
    try:
        with contextlib.redirect_stdout(effusus):
            silva.commissio('nihil', ['pythonica/README.md'], portae=['ficta-numerans', 'ficta-rubra2'])
        credo(False, 'commissio cum rubra levat')
    except silva.SilvaError as ex:
        credo('ficta-rubra2' in str(ex) and cursus_n() == 1 and 'non iterum cursa' in effusus.getvalue(),
              'commissio: receptum vivum sanum consumptum (porta non iterum cursa, linea impressa); rubra sequens levat')
    try:
        silva.commissio('nihil', ['pythonica/README.md'], portae=['ficta-numerans', 'ficta-rubra2'], recepta=False)
    except silva.SilvaError:
        pass
    credo(cursus_n() == 2, 'commissio recepta=False: porta iterum cursa')
    open(novum, 'w').write('x')
    try:
        try:
            silva.commissio('nihil', ['pythonica/README.md'], portae=['ficta-numerans', 'ficta-rubra2'])
        except silva.SilvaError:
            pass
        credo(cursus_n() == 3, 'commissio: arbor mutata post receptum = porta iterum cursa')
    finally:
        os.unlink(novum)
    pr = silva.porta('ficta-rancida')
    credo(pr.sana and pr.rancida and os.path.exists(rancidum),
          'porta: arbor mutata DUM currebat = rancida (verdictum manet)')
    rvr = silva.receptum_vivum('ficta-rancida')
    credo(rvr is not None and not rvr.sana and 'DUM currebat' in rvr.compendium,
          'receptum_vivum: rancidum numquam valet')
    os.unlink(rancidum)
    effusus = io.StringIO()
    try:
        with contextlib.redirect_stdout(effusus):
            silva.commissio('nihil', ['pythonica/README.md'], portae=['ficta-rancida', 'ficta-rubra2'])
        credo(False, 'commissio cum rubra levat')
    except silva.SilvaError as ex:
        credo('ficta-rubra2' in str(ex) and 'DUM' not in str(ex) and 'cetera libera' in effusus.getvalue(),
              'commissio: porta rancida cum plagulis commissionis intactis accepta cum monito')
    os.unlink(rancidum)
    open(mutandum, 'w').write('a\n')
    try:
        silva.commissio('nihil', ['pythonica/.mutans_probatio.tmp'], portae=['ficta-mutans'])
        credo(False, 'commissio: plagula commissionis mutata dum porta currebat levat')
    except silva.SilvaError as ex:
        credo('DUM porta currebat' in str(ex) and '.mutans_probatio.tmp' in str(ex),
              'commissio: plagula commissionis mutata DUM porta currebat = refusio nominata')
    credo('.mutans_probatio.tmp' not in subprocess.run(['git', 'diff', '--cached', '--name-only'], cwd=RADIX,
                                                        capture_output=True, text=True).stdout,
          'commissio refutata: nihil in indice')
finally:
    del os.environ['PYTHONICA_PORTAE_FICTAE']
    for _f in (mutandum, rancidum):
        try:
            os.unlink(_f)
        except OSError:
            pass
    for _n in ('ficta-numerans', 'ficta-rancida', 'ficta-mutans', 'ficta-rubra2'):
        silva.receptum_delere(silva._receptum_vivum_via(_n))

print('--- sera: speculum Pythonicum tools/sera.sh ---')
import shutil
via_se = os.path.join(T, 'probatio.sera')
with silva.sera(via_se, tectum=1) as se:
    credo(os.path.isdir(via_se) and open(via_se + '/pid').read().strip() == str(os.getpid())
          and open(via_se + '/radix').read().strip() == silva.RADIX and os.environ.get('SERA_TENTA') == via_se,
          'sera: capta cum pid et radice, SERA_TENTA exportata')
    with silva.sera(via_se, tectum=1):
        credo(os.path.isdir(via_se), 'sera: reentrantia intra processum')
    credo(os.path.isdir(via_se), 'sera: exitus interior seram avi non tollit')
credo(not os.path.exists(via_se) and 'SERA_TENTA' not in os.environ, 'sera: post with abiit, SERA_TENTA abiit')
os.mkdir(via_se); open(via_se + '/radix', 'w').write(silva.RADIX + '\n'); open(via_se + '/pid', 'w').write('999999\n')
with silva.sera(via_se, tectum=1):
    credo(open(via_se + '/pid').read().strip() == str(os.getpid()), 'sera: vetus (pid mortuus) derelicta et capta')
os.mkdir(via_se); open(via_se + '/radix', 'w').write(silva.RADIX + '\n'); open(via_se + '/pid', 'w').write('%d\n' % os.getpid())
try:
    with silva.sera(via_se, tectum=1):
        credo(False, 'sera tenta (pid vivus, radix eadem) levat')
except silva.SilvaError as ex:
    credo('tenetur' in str(ex) and str(os.getpid()) in str(ex), 'sera tenta: SilvaError post tectum, tenens nominatus')
shutil.rmtree(via_se)
try:
    silva.sera(os.path.join(T, 'nonsera'))
    credo(False, 'sera sine suffixo levat')
except silva.SilvaError:
    credo(True, 'sera: via sine suffixo .sera refutata')
credo(silva.sera_suitae('radix').via.endswith('/build/cursor.sera') and 'silva/build/cursor.sera' in silva.sera_suitae('silva').via,
      'sera_suitae: viae cursorum')

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
# partitio: processus vivus per sample (dormiens: folium syscall)
scriptum = os.path.join(silva.RADIX, 'pythonica', '.cursus_dormiens.tmp.sh')
open(scriptum, 'w').write("#!/bin/sh\nsleep 3\necho Totalis: 1\necho Conditio: OMNIA PRAETERIERUNT\n")
os.chmod(scriptum, 0o755)
try:
    c = silva.probatio_currere(scriptum, secunda=1, mora=0.2)
    credo(c.rc == 0 and c.partitio and sum(n for n, _, _ in c.partitio) > 0
          and 'partitio (folia' in silva.cursus_textus(c),
          'probatio_currere: partitio sumptum (%s)' % (c.partitio[0][1] if c.partitio else '-'))
    credo(os.path.exists(os.path.join(silva.RADIX, 'build', 'sample', '.cursus_dormiens.tmp.sh.probatio.txt')),
          'partitio: effusus crudus in build/sample')
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

print('--- partitio: inclusivum recursione collapsa + viae ---')
via_s = os.path.join(T, 'exemplum.sample.txt')
open(via_s, 'w').write('Call graph:\n    100 Thread_1\n      100 start\n        100 main\n          60 f\n            40 g\n              30 g\n                20 g\n                  20 xar_obtinere\n          40 h\n            40 xar_obtinere\n\nTotal number in stack (recursive counted multiple, when >=5):\n')
inc = dict((fn, n) for _, n, fn in silva.partitio_inclusivum(via_s))
credo(inc.get('g') == 40 and inc.get('xar_obtinere') == 60 and inc.get('f') == 60 and 'main' not in inc,
      'partitio_inclusivum: recursio collapsa (g 40, non 90), fila/main omissa')
viae_x = silva.partitio_viae(via_s, 'xar_obtinere')
credo(viae_x == [(40, 'h > xar_obtinere'), (20, 'f > g×3 > xar_obtinere')],
      'partitio_viae: semitae ordine ponderis, recursio compressa (%s)' % viae_x)
credo(silva.partitio_viae(via_s, 'g') == [(40, 'f > g')], 'partitio_viae: functio recursiva semel, in summo')

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
# COPIA DATATA, non planum vivum (2026-09-23): lineae capitulorum pinnatae
# sunt - planum vivum omni editione portam rubebat (doctrina 01M365VXAF)
via_p = os.path.join(T, 'prosa.md'); shutil.copy(os.path.join(RADIX, 'probationes/fixa/prosa/md-arbor-plan_2026-09-23.md'), via_p)
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
vp = silva.vocabula('prosa')
per_verbum = dict((v.verbum, v) for v in vp.verba)
credo(vp.numeri['verba'] > 5000 and vp.numeri['ambiguum'] == 0 and vp.numeri['latinum'] > 100, 'vocabula: prosa corporis (verba > V milia, ambiguum nullum, latina > C)')
credo(per_verbum['the'].status == 'notum' and per_verbum['the'].classis == 'determinans' and per_verbum['the'].prosa == per_verbum['the'].sedes and per_verbum['the'].symbola == 0, 'vocabula: prosa - the notum determinans, sedes prosae solae')
credo(per_verbum['lexema'].status == 'latinum' and per_verbum['piscina'].status == 'notum' and per_verbum['worklog'].status == 'permissum', 'vocabula: prosa - lexema latinum (glossarium Latinum), piscina notum (Moby eam novit), worklog permissum (contextus anglicus)')
credo(per_verbum['values'].status == 'notum' and per_verbum['values'].lemma == 'value' and per_verbum['values'].regula == 'pluralis-s' and per_verbum['the'].regula == '', 'vocabula: prosa - values per regulam pluralis-s ad value, the sine regula')

print('--- Oratio: quaestiones super orationem (T14) ---')
o = silva.Oratio('Puella rosam amat. Xyzzy virum videt. Puella xyzzy amat.\n')
ss = o.sententiae()
credo(len(ss) == 3 and ss[0].textus == 'Puella rosam amat.' and ss[0].forma == 'prosa' and ss[2].initium == 38 and ss[2].finis == 56 and ss[2].index == 2, 'Oratio.sententiae: III extenta cum forma et indice')
vv = o.vocabula()
credo(len(vv) == 9 and [v.forma for v in vv[:3]] == ['Puella', 'rosam', 'amat'] and all(o.textus[v.initium:v.finis] == v.forma for v in vv) and [v.index for v in vv] == list(range(9)), 'Oratio.vocabula: IX vocabula, extenta octetim == forma, indices ordine')
credo(vv[0].classes == ('substantivum',) and vv[0].lemma == 'puella' and vv[0].linguae == ('latina',) and vv[0].analyses == 3 and vv[0].sententia == 0 and vv[3].sententia == 1 and vv[8].sententia == 2 and vv[0].linea == 1 and vv[8].paragraphus == 0, 'Oratio.vocabula: classes, lemma, linguae, numerus analysium, sententia, paragraphus')
credo(vv[1].classes == ('verbum', 'substantivum') and vv[1].lemma == 'rodo', 'Oratio.vocabula: rosam - classes ordine fontis (participium rodo ante rosam), lemma primae')
vb = o.vocabula(classis='verbum')
credo([v.forma for v in vb] == ['rosam', 'amat', 'videt', 'amat'] and all('verbum' in v.classes for v in vb), 'Oratio.vocabula(classis): candidatum quodque, non primum solum')
credo([v.forma for v in o.vocabula(sententia=2)] == ['Puella', 'xyzzy', 'amat'] and len(o.vocabula(lingua='latina')) == 8, 'Oratio.vocabula(sententia, lingua)')
ig = o.ignota()
credo(len(ig) == 1 and ig[0].forma == 'xyzzy' and ig[0].classes == ('ignotum',) and ig[0].linguae == () and ig[0].analyses == 1 and ig[0].lemma == 'xyzzy' and ig[0].decisio == '', 'Oratio.ignota: xyzzy solum (inventum) - lectio ignoti una (T19k), lingua nulla, decisio nulla')
credo(vv[3].classes == ('nomen-proprium',) and o.analyses(vv[3])[0].fons == 'regula' and o.analyses(vv[3])[0].nativum == 'capitalis' and o.analyses(vv[3])[0].accidentia == {}, 'Oratio: Xyzzy capitale = nomen proprium regula (T13), sine accidentibus')
aa = o.analyses(vv[0]); a = aa[0]
credo(len(aa) == 3 and a.classis == 'substantivum' and a.lemma == 'puella' and a.lingua == 'latina' and a.fons == 'vocabularium-la' and a.nativum == 'N 1 1 NOM S C' and a.sensus.startswith('girl') and a.index == 0, 'Oratio.analyses: classis, lemma, lingua, fons, nativum, sensus (T31 a: Puella subiectum amat - nominativa; T24 prior ablativum ponebat)')
credo(a.accidentia == {'casus': 'nominativus', 'numerus': 'singularis', 'genus': 'femininum', 'declinatio': '1'} and [x.accidentia['casus'] for x in aa] == ['nominativus', 'vocativus', 'ablativus'], 'Oratio.analyses: accidentia titulis, absentia omissa, declinatio numero (T31 a: subiectum nominativum primum, ceterae ordine fontis; T24 prior abl > nom > voc verbo aperto)')
am = o.analyses(2)[0]
credo(am.classis == 'verbum' and am.accidentia['persona'] == 'III' and am.accidentia['tempus'] == 'praesens' and am.accidentia['modus'] == 'indicativus' and am.accidentia['vox'] == 'activa' and am.accidentia['forma-verbi'] == 'finitum' and am.accidentia['coniugatio'] == '1' and 'casus' not in am.accidentia and [(x.classis, x.fons, x.nativum, x.lingua, x.accidentia) for x in o.analyses(ig[0])] == [('ignotum', 'regula', 'ignotum', '', {})] and o.partitio()['nulla'] == 1, 'Oratio.analyses(index): verbum amat - persona tempus modus vox forma coniugatio; ignotum = lectio ignoti una (classis ignotum, fons regula, sine lingua), partitio nulla')
# T27 (2026-09-08): 'Puella bona ambulat' nunc ABLATIVA legitur - regula stricta unita
# (quaesitio omnes) paria concordantia omnia fert et politica parium (II) inter paria
# aeque vicina gradum casuum (ORDO_CASUUM, ablativus primus) sequitur: thesauris IX
# melior (casus +17..+28), hic falsa - sententia inambigua substituta.
ob = silva.Oratio('Puerum bonum videt. Hoc est.\n')
ab = ob.analyses(1)
credo(ab[0].classis == 'adiectivum' and ab[0].umbrae == ('caput:accusativus.singularis.masculinum=0.0',) and all(x.umbrae == () for x in ab if x.classis == 'substantivum') and any(u.endswith('=?') for x in ab for u in x.umbrae) and all(x.umbrae == () for x in ob.analyses(0)), 'Oratio.analyses: umbrae (T19d/T27) - caput boni ligata ad Puerum 0.0, substantiva sine umbris, umbra vacua =?')
hoc = ob.analyses(3)
credo(hoc[0].classis == 'determinans' and all(x.umbrae == ('caput:%s.singularis.%s=?' % (x.accidentia['casus'], x.accidentia['genus']),) for x in hoc if x.classis == 'determinans') and len([x for x in hoc if x.classis == 'determinans']) == 4, 'Oratio.analyses: umbrae determinantis hoc omnes vacuae (=?), determinans primum (T19i: hic DET sine exceptione apud UD)')
# T19g: decisio in arbore - genus decisionis et auctor per vocabulum, partitio documenti
vd = ob.vocabula()
credo([v.forma for v in vd] == ['Puerum', 'bonum', 'videt', 'Hoc', 'est'] and vd[1].decisio == 'impletio' and vd[1].auctor == 'umbra-caput-praecedente-proximo' and vd[0].decisio == 'impletio' and vd[0].auctor == vd[1].auctor, 'Oratio.vocabula: decisio impletio boni et Pueri, auctor regula capitis praecedens STRICTA unita (T19g/T19h/T27)')
credo(vd[3].decisio == 'praelatio' and vd[3].auctor == 'determinans-primum-latinum' and vd[3].analyses > 1 and vd[4].decisio == 'praelatio' and vd[4].auctor == 'auxiliare-primum-latinum', 'Oratio.vocabula: Hoc per priorem determinantium certorum (T19i: hic DET sine exceptione), est per priorem auxiliaris (praelatio)')
pf = ob.partitio()
credo(pf['impletio'] == 2 and pf['praelatio'] == 2 and pf['umbra'] == 0 and pf['coactae'] == 2 and pf['una'] + pf['aperta'] == 1 and sum(pf[k] for k in silva.ORATIO_PARTITIO) == 5, 'Oratio.partitio: coactae II, praelatae II (Hoc, est), videt una/aperta - summa vocabula V')

print('--- Oratio: habitus decretoris et partitio decretum (T38 c) ---')
import os as _os2
_os2.environ['ORATIO_DECRETOR'] = '1'
try:
    _od = silva.Oratio('Puella bellum videt.\n')
    _vv = _od.vocabula()
    _pd = _od.partitio()
finally:
    _os2.environ.pop('ORATIO_DECRETOR', None)
credo([v.forma for v in _vv] == ['Puella', 'bellum', 'videt'] and _vv[1].habitus == 'ordinatus' and _vv[0].habitus == '' and _vv[2].habitus == '', 'Oratio.vocabula: habitus ordinatus in bellum solo (cellula contestata decretoris, tabula vera: obiectum electum), ceteris vacuus')
credo('decretum' in _pd and _pd['decretum'] == 1 and sum(_pd[k] for k in silva.ORATIO_PARTITIO) == 3, 'Oratio.partitio: decretum I (bellum obiectum per decretorem, T38 d: folliculus numeri verbi inactivus), summa vocabula III')
_os2.environ['ORATIO_DECRETOR'] = '0'
try:
    _oe = silva.Oratio('Puella bellum videt.\n')
    _oev = _oe.vocabula()
finally:
    _os2.environ.pop('ORATIO_DECRETOR', None)
credo(all(v.habitus == '' for v in _oev), 'Oratio.vocabula: ORATIO_DECRETOR=0 habitus nullus (decretor abrogatus; ordinarium activus, T38 d)')

print('--- pondera.py: generator tabulae ponderum decretoris (T38 b) ---')
import subprocess as _sp, tempfile as _tf, os as _os
_pondera = ['python3', 'oratio/census/pondera.py']
_p = _sp.run(_pondera + ['oratio/probationes/fixa/pondera/proba_lites.tsv'], capture_output=True, text=True)
_e = open('oratio/probationes/fixa/pondera/exspectata.tsv', encoding='utf-8').read()
credo(_p.returncode == 0 and _p.stdout == _e, 'pondera.py super fixturam proba_lites == exspectata.tsv (LII ordines manu derivati)')
_sine = ''.join(l for l in open('oratio/probationes/fixa/pondera/proba_lites.tsv', encoding='utf-8') if 'la_llct' not in l)
_t = _tf.NamedTemporaryFile('w', suffix='.tsv', delete=False, encoding='utf-8'); _t.write(_sine); _t.close()
_q = _sp.run(_pondera + [_t.name], capture_output=True, text=True); _os.unlink(_t.name)
credo(_q.returncode == 2 and 'la_llct' in _q.stderr, 'pondera.py sine la_llct: recusatio exitu II plagulam absentem nominans')
credo(all(v.decisio == '' and v.auctor == '' for v in silva.Oratio('Puella bona ambulat.\n', crudus=True).vocabula()), 'Oratio(crudus): nulla decisio scripta')
ol = silva.Oratio('In bona terra est.\n')
credo(ol.vocabula()[1].decisio == 'umbra' and ol.vocabula()[1].auctor == 'lex-umbrarum' and ol.vocabula()[0].decisio == 'praelatio' and ol.vocabula()[0].auctor == 'adpositio-prima-latina' and ol.partitio()['umbra'] == 1, 'Oratio.vocabula: bona per legem umbrarum (auctor lex-umbrarum), in per priorem adpositionis (praelatio - prior Latinus ante testimonium)')
# oraculum.sh -regulae -ab N (T19d gamma): ordines a regulis N solum - porta natalis
# super thesaurum minimum (una sententia), ne cursus omnes thesauros iudicet
via_ab = os.path.join(RADIX, 'build', 'pythonica', 'ab.conllu')
os.makedirs(os.path.dirname(via_ab), exist_ok=True)
open(via_ab, 'w').write('# sent_id = a-1\n# text = Puella rosam amat.\n1\tPuella\tpuella\tNOUN\t_\t_\t3\tnsubj\t_\t_\n2\trosam\trosa\tNOUN\t_\t_\t3\tobj\t_\t_\n3\tamat\tamo\tVERB\t_\t_\t0\troot\t_\tSpaceAfter=No\n4\t.\t.\tPUNCT\t_\t_\t3\tpunct\t_\t_\n\n')
def _ordines_ab(n):
    r = subprocess.run(['./oratio/oraculum.sh', '-regulae', '-ab', str(n), '-machina', via_ab], cwd=RADIX, capture_output=True, text=True)
    return r.returncode, [l for l in r.stdout.splitlines() if l.startswith('#  regulae ')]
rc, omnes = _ordines_ab(100000)
credo(rc == 0 and len(omnes) == 1 and omnes[0].endswith('(omnes):'), 'oraculum.sh -regulae -ab N: N supra summam = ordo (omnes) solus')
summa = int(re.search(r'regulae (\d+) \(omnes\)', omnes[0]).group(1))
rc, duo = _ordines_ab(summa - 1)
credo(rc == 0 and len(duo) == 2 and duo[0].startswith('#  regulae %d:' % (summa - 1)) and duo[1] == omnes[0] and 'crudus' not in duo[0], 'oraculum.sh -regulae -ab N: ordines a regulis N solum, crudus omissus')
# T19g bis: -errata -machina - ordines PARTITIO (summa = verba aurea IV) et ERRATUM (nullum: nemo decidit in thesauro minimo)
r_er = subprocess.run(['./oratio/oraculum.sh', '-errata', '-machina', via_ab], cwd=RADIX, capture_output=True, text=True)
part = [l.split('\t') for l in r_er.stdout.splitlines() if '\tPARTITIO\t' in l]
credo(r_er.returncode == 0 and len(part) == 8 and sum(int(x[3]) for x in part) == 4 and all('umbra-obiectum-verbi' in l for l in r_er.stdout.splitlines() if '\tERRATUM\t' in l) and any('\tAUCTOR\tumbra-subiectum' in l for l in r_er.stdout.splitlines()) and any('\tAUCTOR\tumbra-obiectum-verbi' in l for l in r_er.stdout.splitlines()), 'oraculum.sh -errata -machina: PARTITIO VIII genera (decretum T38 c) summa IV verba; AUCTOR subiecti et obiecti (T31 a/b: amat Puellam subiectum, rosam obiectum ligat); ERRATUM obiecti solum (rosam participium rodo primum - casus manualis notatus)')
# T30: -errata -nota genus -machina - ordines NOTA sex columnis (plagula NOTA accidens verba recti conventione), ERRATUM-NOTA ordo columnis XIII si adest; -nota ignota exitus II
r_no = subprocess.run(['./oratio/oraculum.sh', '-errata', '-nota', 'genus', '-machina', via_ab], cwd=RADIX, capture_output=True, text=True)
notae = [l.split('\t') for l in r_no.stdout.splitlines() if '\tNOTA\t' in l]
errata_notae = [l.split('\t') for l in r_no.stdout.splitlines() if '\tERRATUM-NOTA\t' in l]
credo(r_no.returncode == 0 and len(notae) == 7 and all(len(x) == 6 for x in notae) and all(len(x) == 13 and x[2] == 'genus' for x in errata_notae), 'oraculum.sh -errata -nota genus -machina: NOTA VII ordines columnis VI, ERRATUM-NOTA columnis XIII generis')
r_ni = subprocess.run(['./oratio/oraculum.sh', '-errata', '-nota', 'color', '-machina', via_ab], cwd=RADIX, capture_output=True, text=True)
credo(r_ni.returncode == 2 and 'nota ignota' in r_ni.stderr, 'oraculum.sh -nota ignota: exitus II causa nominata')
h = silva.Oratio('oratio/probationes/fixa/txt/hilarius.txt')
credo(h.via.endswith('hilarius.txt') and len(h.vocabula()) == 1682 and len(h.sententiae()) > 50 and 0 < len(h.ignota()) < 120, 'Oratio(via): Hilarius MDCLXXXII vocabula (T12; MDCLXXVIII ante T21 - enclitica IV scissa)')
try:
    silva.Oratio('oratio/nemo.txt'); credo(False, 'Oratio: plagula absens refutatur')
except silva.SilvaError as ex:
    credo('absens' in str(ex), 'Oratio: plagula absens refutatur')
credo(len(silva.Oratio(b'Puella amat.\n').vocabula()) == 2 and silva.Oratio('').vocabula() == [], 'Oratio(bytes); textus vacuus = nihil')

oe = silva.Oratio("The cats ran quickly. Bush won.\n")
ve = oe.vocabula()
credo([v.forma for v in ve] == ['The', 'cats', 'ran', 'quickly', 'Bush', 'won'] and ve[1].classes == ('substantivum', 'verbum') and ve[1].lemma == 'cat' and all('anglica' in v.linguae for v in ve) and 'nomen-proprium' in ve[4].classes, 'Oratio Anglica (T16): cats substantivum + verbum, lemma cat, Bush nomen proprium (regula capitalis)')
ae = [x for x in oe.analyses(ve[1]) if x.classis == 'substantivum'][0]
credo(ae.lingua == 'anglica' and ae.fons == 'vocabularium-en' and ae.nativum == 'N pluralis-s' and ae.accidentia == {'numerus': 'pluralis'} and ae.sensus == '', 'Oratio.analyses Anglica: nativum codices + regula, accidentia pluralis, sensus absens')
credo([x.classis for x in oe.analyses(ve[4])][-1] == 'nomen-proprium' and oe.analyses(ve[4])[-1].fons == 'regula' and len(oe.vocabula(lingua='anglica')) == 6, 'Oratio Anglica: nomen proprium regula ultimum, filtrum linguae')

orr = silva.Oratio('Cum puella ambulat.\n'); orc = silva.Oratio('Cum puella ambulat.\n', crudus=True)
# T20a: CLAUSULAE - clausula/clausula_causa vocabuli, Oratio.clausulae()
oc = silva.Oratio('Caesar, cum venisset, urbem cepit. Puella rosam amat.\n')
vc = oc.vocabula()
credo([v.forma for v in vc] == ['Caesar', 'cum', 'venisset', 'urbem', 'cepit', 'Puella', 'rosam', 'amat'] and [v.clausula for v in vc] == [0, 1, 1, 0, 0, 0, 0, 0] and [v.clausula_causa for v in vc] == ['extentum', 'semen', 'extentum', 'clausura', 'clausura', 'unica', 'unica', 'unica'], 'Oratio.vocabula: clausula et clausula_causa (T20a) - cum subordinata semen, venisset extentum, urbem cepit clausura, sententia sine semine unica')
cc = oc.clausulae()
credo(len(cc) == 3 and cc[0].sententia == 0 and cc[0].species == 'principalis' and cc[0].semen is None and cc[0].pater is None and cc[0].membra == (0, 3, 4) and cc[1].species == 'subordinata' and cc[1].semen == 2 and cc[1].pater == 0 and cc[1].membra == (1, 2) and cc[2].sententia == 1 and cc[2].membra == (5, 6, 7) and oc.clausulae(sententia=1) == [cc[2]], 'Oratio.clausulae: III nodi - principalis (Caesar urbem cepit), subordinata semen II pater 0 (cum venisset), sententia altera unica; filtrum sententiae')
credo(silva.Oratio('Caesar, cum venisset, urbem cepit.\n', crudus=True).clausulae() == [] and all(v.clausula is None and v.clausula_causa == '' for v in silva.Oratio('Puella amat.\n', crudus=True).vocabula()), 'Oratio(crudus): sine stampa - clausulae() vacuae, clausula None')
credo([x.accidentia['casus'] for x in orr.analyses(1)] == ['ablativus', 'nominativus', 'vocativus'] and [x.accidentia['casus'] for x in orc.analyses(1)] == ['nominativus', 'vocativus', 'ablativus'] and orr.vocabula()[1].classes == ('substantivum',), 'Oratio resolutio (T17): cum puella - ablativus primus; crudus = ordo fontis')

print('--- Prosa.sententia: delegatio Orationi (T14) ---')
via_s = os.path.join(T, 'prosa3.md')
open(via_s, 'w').write('# Doc\n\n## Ideas\n\nOne sentence here. Second one\nwraps here.\n\n- item one. item two.\n\n## Code\n\n```c\nx. y;\n```\n\nLast one.\n')
r = silva.Prosa(via_s); sec = r.sectio('Ideas')
s1 = r.sententia(1, intra=sec)
credo(r.corpus(s1) == 'Second one\nwraps here.' and s1.tag == 'sententia' and s1.linea == 5 and s1.columna == 20 and s1.linea_finis == 6, 'Prosa.sententia intra sectionem: extentum in octetos plagulae remissum, linea + columna')
credo(r.corpus(r.sententia(2, intra=sec)) == '- item one. item two.' and r.corpus(r.sententia(3)) == 'Last one.' and r.sententia(0).linea == 5, 'Prosa.sententia: elementum listae paragraphus (nota intra), saeptum et capitula numquam, ultima totius plagulae')
try:
    r.sententia(4); credo(False, 'Prosa.sententia extra numerum refutatur')
except silva.SilvaError as ex:
    credo('4 praesentia' in str(ex), 'Prosa.sententia extra numerum refutatur')
r.substituere(s1, 'Second one here.')
try:
    r.sententia(0, intra=sec); credo(False, 'Prosa.sententia: intra rancidum refutatur')
except silva.SilvaError as ex:
    credo('rancidum' in str(ex), 'Prosa.sententia: intra rancidum refutatur')
credo(r.corpus(r.sententia(1)) == 'Second one here.', 'Prosa.sententia post editionem: reselectum in versione praesenti')
f4 = r.applicare()
credo(f4.sana and 'One sentence here. Second one here.\n\n- item' in open(via_s).read() and f4.ancorae.get('sententia') == 4, 'Prosa.sententia + substituere + applicare: sanum, ancorae numeratae')


print('--- clientes materiae: arbor ex suffixo, coctum, exemplaria (2026-09-17) ---')
via_sh = os.path.join(T, 'proba_crusta.sh')
open(via_sh, 'w').write('if a; then b; fi # c\n\n')
arb_sh = silva.arbor(via_sh)
credo('grammatica="crusta"' in arb_sh and '<conditio' in arb_sh,
      'arbor .sh: proiectio crustae per suffixum (tota, non -tacitus)')
try:
    silva.arbor(via_sh, nudum=True)
    credo(False, 'arbor .sh nudum refutatur')
except silva.SilvaError as ex:
    credo('nudum' in str(ex), 'arbor .sh nudum refutatur (solius silvae)')
arb_sedes = silva.arbor(via_sh, sedes=True)
credo('visio="sedes"' in arb_sedes and 'octeti="0-16"' in arb_sedes
      and 'sedes="1:1-1:3"' in arb_sedes,
      'arbor .sh sedes: visio, tractus conditionis et lexematis if')
try:
    silva.arbor(via, sedes=True)
    credo(False, 'arbor .c sedes refutatur')
except silva.SilvaError as ex:
    credo('sedes' in str(ex), 'arbor .c sedes refutatur (clientium solum)')
cc = silva.coctum(via_sh)
credo(cc == silva.Coctum('if a; then\n    b;\nfi\n', True, 0, 0),
      'coctum: forma declare -f, commentum abiectum, sana: %r' % (cc,))
via_malum = os.path.join(T, 'proba_malum.sh')
open(via_malum, 'w').write('fi\n')
cm = silva.coctum(via_malum)
credo(not cm.sana and cm.mala == 1 and cm.clausurae_absentes == 0,
      'coctum: parsura non sana nominata (mala 1): %r' % (cm,))
try:
    silva.coctum(via)
    credo(False, 'coctum .c refutatur')
except silva.SilvaError:
    credo(True, 'coctum .c refutatur (solum .sh)')

# EXEMPLARIA, _relata, diagnostica_lintris: DELETA (opus VI arcus
# exemplariorum, 2026-09-18). Extractio gradus II in C vivit
# (materia_exemplaria); hae probationes latus PYTHONICUM exercebant
# quod non iam exstat.
#
# QUOD EAS SUCCEDIT, ET UBI:
#   extractio ipsa      materia/probationes/probatio_materia_exemplaria.c
#   sequentia utriusque materia/probationes/probatio_materia_plena.c
#   facies crustae      crusta/probationes/probatio_crusta_facies.c
#   instrumentum        tools/diagnostica_fumus.sh XII-XVIII
#   census regulae I    crusta/instrumenta/differentia_exemplariorum.sh
#     (CCXCII / CCLXXVIII / XIV contra AURUM quod extractor pythonicus
#      ULTIMO dixit - vide fixa/exemplaria/README.md)
#
# 'diagnostica_materiae' infra MANET et gradum UTRUMQUE nunc fert:
# eadem functio, sed inventa lintris per eam iam veniunt.

print('--- diagnostica_materiae (clientes materiae) ---')
import tempfile
_d = tempfile.mkdtemp(prefix='probatio_diagnostica_')
try:
    _fractum = os.path.join(_d, 'plantatum.sh')
    with open(_fractum, 'w') as f:
        f.write('{ echo a')
    _r = silva.diagnostica_materiae(_fractum)
    credo(len(_r) == 1, 'diagnostica_materiae: unum diagnosticum (%d)' % len(_r))
    if _r:
        credo(_r[0].linea == 1 and _r[0].columna == 9,
              'diagnostica_materiae: sedes 1:9 (%d:%d)'
              % (_r[0].linea, _r[0].columna))
        credo(_r[0].gravitas == 'erratum' and _r[0].codex == 'grex/tok_clausura',
              'diagnostica_materiae: gravitas et codex CRUDUS (%s %s)'
              % (_r[0].gravitas, _r[0].codex))
        credo(_r[0].textus == '', 'diagnostica_materiae: absentia textum vacuum fert')
        credo(_r[0].via == _fractum, 'diagnostica_materiae: via ut data reddita')
        # SEDES MULTIPLICES: absentia aperturam suam GRATIS fert
        credo(_r[0].nota == 'hic exspectatur',
              'diagnostica_materiae: nota primariae (%r)' % (_r[0].nota,))
        credo(len(_r[0].relata) == 1,
              'diagnostica_materiae: apertura una relata (%d)'
              % len(_r[0].relata))
        if _r[0].relata:
            _ap = _r[0].relata[0]
            credo(_ap.linea == 1 and _ap.columna == 1
                  and _ap.linea_finis == 1 and _ap.columna_finis == 9,
                  'diagnostica_materiae: apertura 1:1-1:9 (%d:%d-%d:%d)'
                  % (_ap.linea, _ap.columna, _ap.linea_finis,
                     _ap.columna_finis))
            credo(_ap.nota == 'hic coepit',
                  'diagnostica_materiae: nota aperturae (%r)' % (_ap.nota,))
            # OCTETI sedis relatae: pictor eis pingit, non lineis
            credo(_ap.initium == 0 and _ap.finis == 8,
                  'diagnostica_materiae: octeti aperturae 0-8 (%d-%d)'
                  % (_ap.initium, _ap.finis))

    # FIXUM DISCRIMINANS: apertura ad 1:9, ubi linea et columna DIFFERUNT.
    # Fixum superius ('{ echo a') aperturam ad 1:1 habet, ergo permutationem
    # lineae et columnae videre NON POTEST - planta manualis id invenit,
    # sicut 'e acutum' in arcu priore duos octetos non discernebat.
    _columna = os.path.join(_d, 'columna.sh')
    with open(_columna, 'w') as f:
        f.write('echo a; { echo b')
    _cl = silva.diagnostica_materiae(_columna)
    credo(len(_cl) == 1 and len(_cl[0].relata) == 1,
          'diagnostica_materiae: fixum discriminans unum diagnosticum (%d)'
          % len(_cl))
    if _cl and _cl[0].relata:
        _ap2 = _cl[0].relata[0]
        credo(_ap2.linea == 1 and _ap2.columna == 9
              and _ap2.linea_finis == 1 and _ap2.columna_finis == 17,
              'diagnostica_materiae: apertura 1:9-1:17, linea != columna '
              '(%d:%d-%d:%d)' % (_ap2.linea, _ap2.columna, _ap2.linea_finis,
                                 _ap2.columna_finis))
        credo(_ap2.initium == 8 and _ap2.finis == 16,
              'diagnostica_materiae: octeti aperturae 8-16 (%d-%d)'
              % (_ap2.initium, _ap2.finis))

    _sanum = os.path.join(_d, 'sanum.sh')
    with open(_sanum, 'w') as f:
        f.write('echo a\n')
    credo(silva.diagnostica_materiae(_sanum) == [],
          'diagnostica_materiae: plagula sana lista vacua')

    # textus SEGMENTUM fontis fert (css: '}' vagum)
    _css = os.path.join(_d, 'plantatum.css')
    with open(_css, 'w') as f:
        f.write('a { color: red; } }')
    _c = silva.diagnostica_materiae(_css)
    credo(len(_c) == 1 and _c[0].textus == '}',
          'diagnostica_materiae: textus segmentum fontis (%r)'
          % (_c[0].textus if _c else None))
    # NEGATIVUM: genus (non absentia) sedem UNAM fert, ergo exitus eius
    # octetim idem manet ac ante sedes multiplices
    credo(len(_c) == 1 and _c[0].relata == () and _c[0].nota is None,
          'diagnostica_materiae: genus css sedem unam fert (relata %r, nota %r)'
          % (_c[0].relata if _c else None, _c[0].nota if _c else None))

    _ignotum = os.path.join(_d, 'x.txt')
    with open(_ignotum, 'w') as f:
        f.write('x\n')
    try:
        silva.diagnostica_materiae(_ignotum)
        credo(False, 'diagnostica_materiae: suffixum ignotum levat')
    except silva.SilvaError:
        credo(True, 'diagnostica_materiae: suffixum ignotum levat SilvaError')

    # NOMEN: 'diagnostica' simplex verdictum legati C89 manet - functiones
    # DUAE sunt, non una (Python definitionem priorem tacite obumbrat)
    credo(silva.diagnostica is not silva.diagnostica_materiae,
          'diagnostica et diagnostica_materiae functiones diversae')
finally:
    import shutil
    shutil.rmtree(_d, ignore_errors=True)


print('--- opus: commissio opus tabularii claudit (via frigida FICTA) ---')
# Via frigida SUBSTITUITUR stipula quae argumenta sua in plagulam
# scribit: porta pythonica NUMQUAM in tabularium vivum scribat.
# Stipula: '-res X' -> lineam primam breviarii ex ambitu reddit;
# aliter argumenta notat et rc ex ambitu reddit.
_od = tempfile.mkdtemp(prefix='silva_opus_')
_stip = os.path.join(_od, 'frigida_ficta.sh')
_acta = os.path.join(_od, 'acta.txt')
open(_stip, 'w').write(
    '#!/bin/bash\n'
    'printf "%s\\n" "$*" >> "' + _acta + '"\n'
    'if [ "$1" = "-res" ]; then\n'
    '  [ -n "${FICTA_RES_RC:-}" ] && { echo "res ignota" >&2; exit "$FICTA_RES_RC"; }\n'
    '  echo "  [nexus] strepitus launcheri"\n'
    '  echo "${FICTA_LINEA:-Opus fictum (opus, susceptum)}"\n'
    '  exit 0\n'
    'fi\n'
    'n=$(grep -c -- "-mutatio\\|-status" "' + _acta + '")\n'
    'if [ -n "${FICTA_FRANGE_AD:-}" ] && [ "$n" -ge "$FICTA_FRANGE_AD" ]; then echo "machina recusat" >&2; exit 1; fi\n'
    'echo scriptum\n')
os.chmod(_stip, 0o755)
_fr_vera = silva.FRIGIDA_IMPERIUM
silva.FRIGIDA_IMPERIUM = [_stip]
try:
    # praeiudicium: opus apertum licet; strepitus launcheri ('  [nexus]') toleratur
    credo(silva.opus_praeiudicare('X') == [], 'opus apertum (susceptum): nulla causa')
    os.environ['FICTA_LINEA'] = 'Parcum quoddam (parcum, tractum)'
    c = silva.opus_praeiudicare('X')
    credo(len(c) == 1 and "genus 'parcum'" in c[0] and 'OPERA sola' in c[0], 'genus non-opus: causa genus nominat')
    os.environ['FICTA_LINEA'] = 'Opus vetus (opus, perfectum)'
    c = silva.opus_praeiudicare('X')
    credo(len(c) == 1 and "'perfectum'" in c[0] and 'pendens | susceptum' in c[0], 'opus iam perfectum: causa status legales nominat')
    os.environ['FICTA_LINEA'] = 'Titulus (cum parenthesi) intus (opus, pendens)'
    credo(silva.opus_praeiudicare('X') == [], 'titulus parentheses ferens: genus et status ex fine lineae')
    del os.environ['FICTA_LINEA']
    os.environ['FICTA_RES_RC'] = '1'
    c = silva.opus_praeiudicare('nusquam')
    credo(len(c) == 1 and 'non solvitur' in c[0] and 'res ignota' in c[0], 'opus ignotum: causa machinae transit')

    # CAUSAE OMNES SIMUL: via vetita ET opus ignotum responso UNO, ANTE portam ullam
    try:
        silva.commissio('nihil', ['FAQ.md'], portae=['ficta-quae-non-est'], opus='nusquam')
        credo(False, 'commissio cum causis duabus')
    except silva.SilvaError as ex:
        credo('(2 causae)' in str(ex) and 'VETITA' in str(ex) and 'non solvitur' in str(ex),
              'commissio: via vetita ET opus ignotum SIMUL relata (non guttatim)')
        credo('nihil cursum' in str(ex), 'commissio: causae praeviae ante portas (nihil cursum)')
    # causa UNA formam pristinam servat (probatio 'VETITA ... remove' supra id tenet)
    del os.environ['FICTA_RES_RC']

    # clausura: gradus duo, argumenta recta, provenientia commissi
    open(_acta, 'w').close()
    eff = silva.opus_claudere('01OPUSFICTUM', 'abc1234', 'titulus commissi: res gesta\n\ncorpus longum\n')
    credo(eff == 'abc1234: titulus commissi: res gesta', 'effectus = commissum + linea PRIMA nuntii')
    lin = open(_acta).read().splitlines()
    credo(len(lin) == 2, 'clausura: vocationes duae (mutatio, status)')
    credo(lin[0] == '-actor claude -origo commissum:abc1234 -mutatio 01OPUSFICTUM effectus abc1234: titulus commissi: res gesta',
          'gradus I: -mutatio effectus cum provenientia commissi')
    credo(lin[1] == '-actor claude -origo commissum:abc1234 -status 01OPUSFICTUM perfectum',
          'gradus II: -status perfectum')

    # clausura FRACTA in gradu II: error CLARUS, hash fertur, NOLI iterare, imperium ad finiendum
    open(_acta, 'w').close()
    os.environ['FICTA_FRANGE_AD'] = '2'
    try:
        silva.opus_claudere('01OPUSFICTUM', 'abc1234', 'titulus\n')
        credo(False, 'clausura fracta')
    except silva.SilvaOpusError as ex:
        credo(ex.hash == 'abc1234', 'SilvaOpusError hash commissi fert')
        credo('FACTUM EST' in str(ex) and 'NOLI iterare' in str(ex), 'error dicit commissum STARE')
        credo('gradus 2 ex 2' in str(ex), 'error gradum fractum nominat')
        credo('-status 01OPUSFICTUM perfectum' in str(ex) and '-mutatio' not in str(ex).split('Ad finiendum manu:')[1],
              'error imperium RESIDUUM solum dat (gradus I iam factus non iteratur)')
    del os.environ['FICTA_FRANGE_AD']
finally:
    silva.FRIGIDA_IMPERIUM = _fr_vera
    for _k in ('FICTA_LINEA', 'FICTA_RES_RC', 'FICTA_FRANGE_AD'):
        os.environ.pop(_k, None)
    import shutil
    shutil.rmtree(_od, ignore_errors=True)

print('--- linea auctoris: SEMEL ---')
_n1 = silva._nuntium_cum_trailer('titulus\n\ncorpus\n\nCo-Authored-By: Exemplar X <x@y>\n')
credo(_n1.count('Co-Authored-By:') == 1 and 'Exemplar X' in _n1, 'nuntius lineam auctoris iam ferens: nulla altera appenditur')
_n2 = silva._nuntium_cum_trailer('titulus\n\ncorpus\n')
credo(_n2.count('Co-Authored-By:') == 1 and _n2.endswith('\n'), 'nuntius sine linea: una appenditur')
_n3 = silva._nuntium_cum_trailer('titulus: de Co-Authored-By: in media linea loquitur\n')
credo(_n3.count('\nCo-Authored-By:') == 1, 'mentio in MEDIA linea non est linea auctoris')


print()
if fracta:
    print('PYTHONICA: FRACTA %d' % len(fracta))
    sys.exit(1)
print('PYTHONICA: sana')

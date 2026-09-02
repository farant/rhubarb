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
diff = e.applicare()
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
e.applicare()
x2 = silva.extenta(via)
credo([t.titulus for t in x2 if t.definitio] == ['a', 'c', 'b'],
      'c post a, ante b')
e = silva.Editio(via)
e.inserere_ante('a', '/* d */\nvacuum\nd (vacuum)\n{\n    redde;\n}\n')
e.applicare()
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

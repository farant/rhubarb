"""pagina_examinis.py - folium codicum examinis ex fonte silvae.

Usus: python3 pagina_examinis.py <radix arboris>

Codices ex silva/fontes/silva_c89_semantica.h (enumeratio ExamenCodex,
ordine: numerus = index) et causae + severitates ex tabula _codices[] in
silva_c89_semantica.c - FONS UNICUS silvae, numquam copia. Glossae
Anglicae manu scriptae in docs/instrumenta/examen_glossae.tsv (per
NOMEN codicis, stabile). Codex sine glossa 'gloss needed' notatur, non
omittitur; numerus codicum et tabulae discrepans = RECUSATIO (nihil
scribitur).
"""
import html, os, re, sys
from collections import Counter

radix = sys.argv[1]
NOTA = '<!-- GENERATUM: docs/instrumenta/pagina_examinis.py - noli manu mutare -->'
h = open(os.path.join(radix, 'silva/fontes/silva_c89_semantica.h')).read()
c = open(os.path.join(radix, 'silva/fontes/silva_c89_semantica.c')).read()

a = h.index('EXAMEN_CODEX_REDECLARATIO_GENERIS = 0')
blk = h[a:h.index('} ExamenCodex;', a)]
nomina, dies, ultimus, pos = [], [], '', 0
for m in re.finditer(r'(EXAMEN_CODEX_[A-Z0-9_]+)', blk):
    d = re.findall(r'20\d\d-\d\d-\d\d', blk[pos:m.start()])
    pos = m.end()
    nomina.append(m.group(1)[len('EXAMEN_CODEX_'):])
    dies.append(d[-1] if d else '')
if nomina and nomina[-1] == 'NUMERUS':
    nomina.pop(); dies.pop()

t = c[c.index('_codices[] = {'):]
t = t[:t.index('\n};')]
tabula = []
for m in re.finditer(r'\{((?:\s*"(?:[^"\\]|\\.)*")+)\s*,\s*(EXAMEN_[A-Z]+)\s*\}', t):
    tabula.append((''.join(re.findall(r'"((?:[^"\\]|\\.)*)"', m.group(1))), m.group(2)[len('EXAMEN_'):]))
if len(tabula) != len(nomina):
    sys.exit('pagina_examinis: %d codices in capite, %d in tabula - recuso' % (len(nomina), len(tabula)))

glossae = {}
for linea in open(os.path.join(radix, 'docs/instrumenta/examen_glossae.tsv')):
    if linea.startswith('#') or '\t' not in linea:
        continue
    k, v = linea.rstrip('\n').split('\t', 1)
    glossae[k] = v

GRADUS = [
    ('VIOLATIO', 'violatio', 'A breach of C89 as the standard defines it: the compiler would reject it, or the program is not valid C89. Any violatio makes the verdict REICE.'),
    ('SUSPECTUM', 'suspectum', 'Legal C89, but suspicious: the kind of thing a compiler warns about (an implicit declaration, a path with no return, a variable read before it is written).'),
    ('DOMESTICUM', 'domesticum', "A house rule: legal and often unremarkable elsewhere, but against this house's discipline (sign conversions the -Werror flags would reject, chorda treated as NUL-terminated, Latin and identity rules, contracts, portability). They count against the house's own code; a stricter mode that makes them block outright is a named future step."),
    ('INFRA', 'infra', 'A gap in the tools, never a verdict: something examen cannot yet analyse (a missing system lexicon, a parked feature such as bit-fields). Verdicts exclude these rows as provisional.'),
]
numeri = Counter(g for _, g in tabula)
sine = [nomina[i] for i in range(len(nomina)) if nomina[i] not in glossae]

def linea(i, cum_ancora):
    causa, gradus = tabula[i]
    gl = glossae.get(nomina[i])
    return '<tr%s><td>%d</td><td><code>%s</code></td><td><span class="gradus-%s">%s</span></td><td>%s<br><span class="nota">%s%s</span></td></tr>' % (
        (' id="c%d"' % i) if cum_ancora else '', i, nomina[i], gradus.lower(), gradus.lower(),
        html.escape(gl) if gl else '<em>gloss needed</em>',
        html.escape(causa), (' · ' + dies[i]) if dies[i] else '')

# ordo numeri (ancorae #cN hic solum) et ordo gradus (greges cum titulo)
lineae_numeri = [linea(i, True) for i in range(len(tabula))]
lineae_gradus = []
for k, g, _ in GRADUS:
    membra = [i for i in range(len(tabula)) if tabula[i][1] == k]
    lineae_gradus.append('<tr class="grex-gradus"><td colspan="4"><span class="gradus-%s">%s</span> <span class="nota">%d codes</span></td></tr>' % (g, g, len(membra)))
    lineae_gradus += [linea(i, False) for i in membra]

def tabula_html(clavis, lineae_t):
    caput_num = '#' + (' &#9650;' if clavis == 'numerus' else '')
    caput_gr = 'Severity' + (' &#9650;' if clavis == 'gradus' else '')
    return '<table class="codices ordo-%s">\n<thead><tr><th><label for="ordo-numerus" class="ordinator">%s</label></th><th>Code</th><th><label for="ordo-gradus" class="ordinator">%s</label></th><th>What it catches</th></tr></thead>\n<tbody>\n%s\n</tbody>\n</table>' % (
        clavis, caput_num, caput_gr, '\n'.join(lineae_t))

tabulae = ('<input type="radio" name="ordo-codicum" id="ordo-numerus" class="ordo" checked>\n'
           '<input type="radio" name="ordo-codicum" id="ordo-gradus" class="ordo">\n\n'
           + tabula_html('numerus', lineae_numeri) + '\n\n' + tabula_html('gradus', lineae_gradus))

gradus_html = '\n'.join('<tr><td><strong>%s</strong></td><td>%d</td><td>%s</td></tr>' % (g, numeri[k], html.escape(d)) for k, g, d in GRADUS)

open(os.path.join(radix, 'docs/examen.html'), 'w').write('''<!doctype html>
%(nota)s
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Examen</title>
<link rel="stylesheet" href="stilus.css">
</head>
<body>

<p class="nota"><a href="index.html">Rhubarb</a></p>
<h1>Examen</h1>
<p class="subtitulus">Every rule the house's C89 judge checks: %(n)d codes</p>

<p><code>examen</code> is silva's judge of a single C file: it parses the
file with silva, analyses its types and its flow, and gives a verdict,
<strong>ACCIPE</strong> (accept), <strong>REICE</strong> (reject),
<strong>INFRA</strong> (the tools could not judge) or
<strong>RECUSO</strong> (it refuses: the file outran silva's
limits, macros were left unexpanded, and any verdict would be fiction;
a measured case once produced 293 phantom unknown types under a false
REICE). It runs after every edit
(the editor hook), on every commit (the pre-commit hook, on the .c files
being committed) and in the language server's diagnostics, so most of
its rules are met long before the compiler sees the file. Each finding
carries a numbered code. The codes are stable: they are never reordered
or renumbered, new ones are only appended, so a number seen in a log, a
decree or a pinned count always means the same rule.</p>

<h2>The four severities</h2>
<table>
<thead><tr><th>Severity</th><th>Codes</th><th>Meaning</th></tr></thead>
<tbody>
%(gradus)s
</tbody>
</table>

<h2>Suppressing a finding: TOLERA</h2>
<p>A deliberate exception is written next to the code it excuses, as a
comment on the same line or the line before:
<code>/* TOLERA CODE_NAME: reason */</code>. The reason is mandatory. A
TOLERA never silences anything the compiler itself would reject, and a
TOLERA that absorbs nothing, gives no reason or names an unknown code is
itself a finding (code 56, TOLERA_IRRITUM), because suppressions rot and
one that excuses nothing is a signal, not noise.</p>

<h2>All codes</h2>
<p class="nota">English gloss first; below it, silva's own Latin cause and,
where its group's comment gives one, the date the code was added. Groups
of codes share a long comment in
<code>silva/fontes/silva_c89_semantica.h</code> that explains the
calibration against clang and the decisions behind each rule.</p>
<p class="nota">Click <em>#</em> to order by number, or <em>Severity</em>
to see the codes clustered by kind.</p>

%(tabulae)s

<p class="nota">Generated from silva's single source (the ExamenCodex
enumeration and the _codices[] table in
<code>silva/fontes/silva_c89_semantica.{h,c}</code>) joined with
hand-written English glosses in
<code>docs/instrumenta/examen_glossae.tsv</code>; regenerate with
<code>./docs/instrumenta/regenerare.sh</code>. A new code appears here
marked "gloss needed" until its gloss is written.%(sine)s</p>

</body>
</html>
''' % {'nota': NOTA, 'n': len(tabula), 'gradus': gradus_html, 'tabulae': tabulae,
       'sine': (' Missing now: ' + ', '.join(sine) + '.') if sine else ''})
print('examen: %d codices (%s), sine glossa %d' % (len(tabula), ', '.join('%s %d' % (k.lower(), numeri[k]) for k, _, _ in GRADUS), len(sine)))

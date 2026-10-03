"""pagina_visionis.py - folium visionum ex scheda recensionis.

Usus: python3 pagina_visionis.py <scheda.md> <titulus> <fons> <provenientia>

fons: 'nomen.md' (in project-specs, nexus fit) aut textus (e.g. res tabularii).

Scheda = project-specs/visiones-extractae/<fons>.md: una sententia per
lineam ('- [ ] X12 textus (Lnn)  {GENUS > X3, R1}'), '[+]' = electa a
Frano. Folium in stdout: tres visus sine JavaScript (ut scripta / per
gradus / per regulas), inter quos radio + :checked commutat.
"""
import html, re, sys

via_schedae, titulus, fons, provenientia = sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4]

# gradus taxonomiae (docs/taxonomiae.html): clavis in scheda, nomen in folio, definitio brevis
GRADUS = [
    ('TELOS', 'Telos', 'the end; answers "why?" for everything'),
    ('PRINCIPIUM', 'Principium', 'prime condition; if ignored, things fail'),
    ('PRINCIPIUM+intervention, split', 'Principium fused with an intervention', 'a condition with its cure attached: to be split'),
    ('OCCASIO', 'Occasio', 'opportunity: situational; ignoring it costs a gain, not a failure'),
    ('INDICIUM', 'Indicium', 'clue: a trend or sign that suggests a direction; dated'),
    ('REGULA', 'Regula', 'commitment, kept or broken'),
    ('VISIO-OPERIS', 'Project vision', 'one thing that should exist'),
    ('DECRETUM', 'Load-bearing decision', 'within a project; expensive to reverse'),
    ('SUPELLEX', 'Furniture (not vision)', 'cheap to change'),
    ('nulla:ratio', 'Fits no tier: reason', 'a diagnosis, argument or analogy'),
    ('nulla:exemplum', 'Fits no tier: precedent', 'prior art cited'),
    ('nulla:quaestio', 'Fits no tier: open question', 'left open by the text'),
]
NOMINA_GRADUUM = {clavis: nomen for clavis, nomen, _ in GRADUS}

# regulae totius operis (docs/index.html); pagina = folium proprium, si est
REGULAE = [
    ('R1', '1.0 and done software', None),
    ('R2', 'C89 is our programming language', None),
    ('R3', 'Write our own libraries instead of using 3rd party dependencies', '../regulae/sine-dependentiis.html'),
]


def textus_reddere(t):
    t = html.escape(t, quote=False)
    t = re.sub(r'\*\*(.+?)\*\*', r'<strong>\1</strong>', t)
    t = re.sub(r'`([^`]+)`', r'<code>\1</code>', t)
    t = re.sub(r'\((L[\d\-, L]+|c|n\d+)\)\s*$', r'<span class="fons">\1</span>', t)
    return t


# --- scheda legenda -------------------------------------------------------
sententiae = []
sectio = None
patres = {}
commentarium = []
in_commentario = False
for linea in open(via_schedae):
    linea = linea.rstrip('\n')
    caput = re.match(r'^## (.*)$', linea)
    if caput:
        in_commentario = 'commentary' in caput.group(1)
        sectio = caput.group(1)
        continue
    if in_commentario:
        if linea.startswith('- '):
            commentarium.append(linea[2:])
        elif linea.strip() and commentarium:
            commentarium[-1] += ' ' + linea.strip()
        continue
    m = re.match(r'^( *)- \[(.)\] ([A-Z]+[\d.]+) (.*?)\s+\{([^{}]+?)(?: > ([A-Z0-9., ]+))?\}$', linea)
    if not m:
        continue
    gradus_profunditatis = len(m.group(1)) // 2
    patres[gradus_profunditatis] = m.group(3)
    pro = [x.strip() for x in m.group(6).split(',')] if m.group(6) else []
    sententiae.append(dict(
        sectio=sectio, profunditas=gradus_profunditatis, id=m.group(3), textus=m.group(4),
        gradus=m.group(5), pater=patres.get(gradus_profunditatis - 1) if gradus_profunditatis else None,
        pro=pro, electa=m.group(2) == '+'))

ignoti = [s['id'] for s in sententiae if s['gradus'] not in NOMINA_GRADUUM]
if ignoti:
    sys.exit('gradus ignoti in: ' + ', '.join(ignoti))


def sententia_reddere(s, cum_patre=False):
    pater = '<span class="pater">sub %s</span>' % s['pater'] if cum_patre and s['pater'] else ''
    pro = '<span class="pater">for %s</span>' % ', '.join(s['pro']) if s['pro'] else ''
    classis = ' class="electum"' if s['electa'] else ''
    return '<li%s><span class="id">%s</span>%s %s%s<span class="genus g-%s">%s</span></li>' % (
        classis, s['id'], textus_reddere(s['textus']), pater, pro,
        re.sub(r'[^a-z]', '', s['gradus'].lower()), NOMINA_GRADUUM[s['gradus']])


# --- visus I: ut scripta ----------------------------------------------------
partes = ['<div class="arbor">']
sectio_currens = None
profunditas = -1
for s in sententiae:
    if s['sectio'] != sectio_currens:
        while profunditas >= 0:
            partes.append('</ul>')
            profunditas -= 1
        partes.append('<h3>%s</h3>' % html.escape(s['sectio']))
        sectio_currens = s['sectio']
    while profunditas < s['profunditas']:
        partes.append('<ul>')
        profunditas += 1
    while profunditas > s['profunditas']:
        partes.append('</ul>')
        profunditas -= 1
    partes.append(sententia_reddere(s))
while profunditas >= 0:
    partes.append('</ul>')
    profunditas -= 1
partes.append('</div>')

# --- visus II: per gradus ---------------------------------------------------
partes.append('<div class="genera">')
for clavis, nomen, definitio in GRADUS:
    membra = [s for s in sententiae if s['gradus'] == clavis]
    partes.append('<h3>%s <span class="numerus">%d</span></h3><p class="nota">%s</p>' % (nomen, len(membra), definitio))
    if membra:
        partes.append('<ul>' + ''.join(sententia_reddere(s, True) for s in membra) + '</ul>')
partes.append('</div>')

# --- visus III: per regulas -------------------------------------------------
partes.append('<div class="regulae">')
partes.append('<p class="nota">Items that bear on a project-wide regula (a second axis: each keeps its tier).</p>')
for clavis, nomen, pagina in REGULAE:
    membra = [s for s in sententiae if clavis in s['pro']]
    nomen_html = '<a href="%s">%s</a>' % (pagina, html.escape(nomen)) if pagina else html.escape(nomen)
    partes.append('<h3><span class="id">%s</span>%s <span class="numerus">%d</span></h3>' % (clavis, nomen_html, len(membra)))
    if membra:
        partes.append('<ul>' + ''.join(sententia_reddere(s, True) for s in membra) + '</ul>')
    else:
        partes.append('<p class="nota">none in this source</p>')
partes.append('</div>')

commentarium_html = ''.join('<li>%s</li>' % textus_reddere(c) for c in commentarium)
electae = sum(s['electa'] for s in sententiae)
print('''<!doctype html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>%(t)s - Visiones</title>
<link rel="stylesheet" href="../stilus.css">
</head>
<body>

<p class="nota"><a href="../index.html">Rhubarb</a> · Visiones</p>
<h1>%(t)s</h1>
<p class="subtitulus">Visiones extractae ex %(fons_html)s</p>

<p>Extracted 2026-10-02 and classified into the <a href="../taxonomiae.html">vision
taxonomy</a>. %(prov)s %(n)d claims%(sel)s. <span class="fons">Lnn</span> is
the source line; "for Xn" marks evidence for another item or a regula.</p>

<input type="radio" name="visus" id="visus-arbor" class="visus" checked>
<label for="visus-arbor" class="visus">As written</label>
<input type="radio" name="visus" id="visus-genera" class="visus">
<label for="visus-genera" class="visus">By tier</label>
<input type="radio" name="visus" id="visus-regulae" class="visus">
<label for="visus-regulae" class="visus">By regula</label>

%(corpus)s

<details>
<summary>Since then: Claude's commentary, not in the source</summary>
<ul>%(comm)s</ul>
</details>

</body>
</html>''' % {'t': titulus, 'f': fons, 'n': len(sententiae), 'corpus': '\n'.join(partes),
             'comm': commentarium_html, 'prov': provenientia,
             'fons_html': ('<a href="../../project-specs/%s">%s</a>' % (fons, fons)) if fons.endswith('.md') else html.escape(fons),
             'sel': (', %d selected (highlighted)' % electae) if electae else ', not yet filtered'})

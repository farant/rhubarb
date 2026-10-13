"""pagina_bibliothecae.py - folia bibliothecarum ex inventario tabularii.

Usus: python3 pagina_bibliothecae.py <inventarium.tsv> <radix arboris> [greges.tsv]

Inventarium = 'bibliothecae (include)' in tabulario, exportatum per
'./gesta/frigida.sh -inventarium' (lineae: ordo, lens, genus, valor).
Scribit docs/bibliothecae/index.html (semper), greges.html (si greges.tsv
datur: inventarium 'greges bibliothecarum', lens 'grex' in ordinibus) et
folium breve pro quaque bibliotheca cum descriptione. Folium SINE nota GENERATUM (manu
scriptum, e.g. json.html) numquam rescribitur.
"""
import html, os, re, sys

via_tsv, radix = sys.argv[1], sys.argv[2]
via_greges = sys.argv[3] if len(sys.argv) > 3 else None
NOTA_GENERATUM = '<!-- GENERATUM: docs/instrumenta/pagina_bibliothecae.py - noli manu mutare -->'
dir_folii = os.path.join(radix, 'docs', 'bibliothecae')

EFFUGIA = {'\\\\': '\\', '\\t': '\t', '\\n': ' '}


def effugia_solvere(textus):
    """Forma machinae inventarii (gesta: _inventarii_effugere) soluta:
    '\\\\' -> '\\', '\\t' -> tabulatio, '\\n' -> spatium (linea nova in
    pagina spatium est). Uno transitu - duo 'replace' seriati '\\\\t'
    (obliqua effugita + 't') in obliquam et tabulationem verterent."""
    return re.sub(r'\\[\\tn]', lambda m: EFFUGIA[m.group(0)], textus)


cellae = {}
for linea in open(via_tsv):
    partes = linea.rstrip('\n').split('\t')
    if len(partes) == 4:
        cellae.setdefault(partes[0], {})[partes[1]] = effugia_solvere(partes[3])


ROMANI = {'I': 1, 'V': 5, 'X': 10, 'L': 50, 'C': 100}


# capita quorum facultates folium proprium habent (pagina_fabricae.py ...)
FOLIA_FACULTATUM = {
    'fabrica': ('fabrica_facultates.html', 'fabrica: the build layer, with an inventory of its features'),
    'stml': ('stml_facultates.html', 'STML: a tour of the syntax, with an inventory of its features'),
}


def numerus_romanus(r):
    n = 0
    for i, c in enumerate(r):
        v = ROMANI.get(c, 0)
        n += -v if i + 1 < len(r) and ROMANI.get(r[i + 1], 0) > v else v
    return n


greges = {}
if via_greges:
    for linea in open(via_greges):
        partes = linea.rstrip('\n').split('\t')
        if len(partes) == 4:
            greges.setdefault(partes[0], {})[partes[1]] = effugia_solvere(partes[3])


def sententia_prima(textus, maxima=170):
    """Prima sententia descriptionis (ad '. ' cuius sequens maiuscula est)."""
    m = re.search(r'\.\s+(?=[A-Z])', textus)
    prima = textus[:m.start() + 1] if m else textus
    return prima if len(prima) <= maxima else prima[:maxima].rsplit(' ', 1)[0] + ' …'


GLOSSA_MAXIMA = 220
ABBREVIATIONES = ('e.g', 'i.e', 'cf', 'etc', 'vs', 'par', 'ca', 'sc', 'viz')
GLOSSAE = {}


def lineae_commentarii(via):
    """Lineae logicae commentarii QUI CAPUT APERIT (sine '/*', ' *', '*/');
    [] si plagula a commentario non incipit."""
    try:
        crudae = open(os.path.join(radix, via)).read().split('\n')[:60]
    except OSError:
        return []
    if not crudae or not crudae[0].startswith('/*'):
        return []
    lineae = []
    for i, cruda in enumerate(crudae):
        textus = cruda[2:] if i == 0 else re.sub(r'^\s*\*(?!/) ?', '', cruda)
        finis = '*/' in textus
        lineae.append(textus.split('*/')[0].strip())
        if finis:
            break
    return lineae


def finis_sententiae(textus):
    """Index post punctum primum quod sententiam claudit (extra parentheses,
    ante spatium aut finem, non post abbreviationem); -1 si nullum."""
    profunditas = 0
    for i, c in enumerate(textus):
        if c in '([':
            profunditas += 1
        elif c in ')]':
            profunditas = max(0, profunditas - 1)
        elif c == '.' and profunditas == 0 and (i + 1 == len(textus) or textus[i + 1] == ' '):
            verbum = textus[:i].rsplit(' ', 1)[-1].lower()
            if verbum in ABBREVIATIONES or len(verbum) == 1:
                continue
            return i + 1
    return -1


def linea_perfecta(textus):
    """Linea glossae quae per se stat: puncto clauditur, aut parenthesi
    clausa cum omnibus parenthesibus paribus."""
    if textus.endswith('.'):
        return True
    return textus.endswith(')') and textus.count('(') == textus.count(')')


def glossa_capitis(via):
    """Glossa capitis: sententia prima commentarii aperientis.

    '/* nomen.h - glossa' (in linea prima aut, post '/*' nudum, secunda).
    Linea quae per se stat tota redditur, ut olim; linea IMPERFECTA (glossa
    in lineas sequentes fluit) ad finem sententiae suae continuatur - aut,
    si sententiam perfectam iam continet, ad eam praeciditur. Caput
    GENERATUM (sine 'nomen.h -'): linea GENERATUM ipsa glossa est."""
    if via in GLOSSAE:
        return GLOSSAE[via]
    lineae = lineae_commentarii(via)
    while lineae and not lineae[0]:
        lineae = lineae[1:]
    glossa = ''
    if lineae:
        m = re.match(r'\S+\.h\s*-+\s*(.*)$', lineae[0])
        if m:
            glossa = m.group(1)
        elif 'GENERATUM' in lineae[0]:
            glossa = lineae[0]
    if glossa and not linea_perfecta(glossa) and len(lineae) > 1 and lineae[1]:
        finis = finis_sententiae(glossa)
        for sequens in lineae[1:]:
            if finis >= 0 or not sequens:
                break
            glossa += ('' if glossa.endswith('/') else ' ') + sequens
            finis = finis_sententiae(glossa)
        if finis >= 0:
            glossa = glossa[:finis]
    if len(glossa) > GLOSSA_MAXIMA:
        glossa = glossa[:GLOSSA_MAXIMA].rsplit(' ', 1)[0] + ' …'
    GLOSSAE[via] = glossa
    return glossa


def plagulae_reddere(textus, gradus):
    """'lib/a.c (15) lib/b.c (8)' -> nexus ad fontes, numeri lenes."""
    if textus.startswith('('):
        return '<span class="nota">%s</span>' % html.escape(textus)
    partes = []
    for m in re.finditer(r'(\S+)(?: \((\d+)\))?', textus):
        nexus = '<a href="%s%s"><code>%s</code></a>' % (gradus, m.group(1), html.escape(m.group(1)))
        if m.group(2):
            nexus += ' <span class="nota">(%s functions)</span>' % m.group(2)
        partes.append(nexus)
    return ', '.join(partes)


def manu_scriptum(via):
    if not os.path.exists(via):
        return False
    return NOTA_GENERATUM not in open(via).read()


ordines = sorted(o for o in cellae if o.startswith('include/'))
descriptae = 0
lineae_indicis = []
for via in ordines:
    c = cellae[via]
    nomen = via[len('include/'):-2]
    glossa = glossa_capitis(via)
    mutatum = c.get('mutatum', '')
    via_folii = os.path.join(dir_folii, nomen + '.html')
    habet_folium = 'descriptio' in c or manu_scriptum(via_folii)
    if 'descriptio' in c:
        descriptae += 1
        if not manu_scriptum(via_folii):
            open(via_folii, 'w').write('''<!doctype html>
%(nota)s
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>%(nomen)s</title>
<link rel="stylesheet" href="../stilus.css">
</head>
<body>

<p class="nota"><a href="../index.html">Rhubarb</a> · <a href="index.html">Bibliothecae</a></p>
<h1>%(nomen)s</h1>
<p class="subtitulus">%(glossa)s</p>

<p>%(descriptio)s</p>

<table>
<tbody>
<tr><td>Header</td><td><a href="../../%(via)s"><code>%(via)s</code></a></td></tr>
<tr><td>Implementation</td><td>%(plagulae)s</td></tr>
<tr><td>Last change</td><td>%(mutatum)s</td></tr>%(grex)s%(facultates)s
</tbody>
</table>
<p class="nota">A stub: one paragraph so far. Generated from the ledger
inventory <em>bibliothecae (include)</em>; edit the inventory, not this
page.</p>

</body>
</html>
''' % {'nota': NOTA_GENERATUM, 'nomen': html.escape(nomen), 'glossa': html.escape(glossa),
       'descriptio': html.escape(c['descriptio']), 'via': via,
       'plagulae': plagulae_reddere(c.get('plagulae', ''), '../../'), 'mutatum': html.escape(mutatum),
       'grex': ('\n<tr><td>Group</td><td><a href="greges.html#%s">%s</a></td></tr>' % (c['grex'], html.escape(greges.get(c['grex'], {}).get('titulus', c['grex'])))) if c.get('grex') else '',
       'facultates': ('\n<tr><td>Features</td><td><a href="%s">%s</a></td></tr>' % FOLIA_FACULTATUM[nomen]) if nomen in FOLIA_FACULTATUM else ''})
    titulus = ('<a href="%s.html">%s</a>' % (nomen, html.escape(nomen))) if habet_folium else html.escape(nomen)
    lineae_indicis.append((nomen, mutatum.split(' ')[0], '<tr%s><td>%s</td><td>%s</td><td class="nota">%s</td><td>%s</td></tr>' % (
        ' class="descripta"' if habet_folium else '', titulus,
        html.escape(glossa) if glossa else '<span class="nota">— (no gloss in the header)</span>',
        html.escape(mutatum.split(' ')[0]), plagulae_reddere(c.get('plagulae', ''), '../../'))))

# quattuor ordines tabulae, sine JavaScript: radio + :checked; caput quodque
# label est quod ad ordinem proximum ducit (iterum pressum = ordo inversus)
ORDINES = [
    ('nomen-asc',    lambda r: (r[0],),       False, 'nomen-desc',   'mutatum-desc'),
    ('nomen-desc',   lambda r: (r[0],),       True,  'nomen-asc',    'mutatum-desc'),
    ('mutatum-desc', lambda r: (r[1], r[0]),  True,  'nomen-asc',    'mutatum-asc'),
    ('mutatum-asc',  lambda r: (r[1], r[0]),  False, 'nomen-asc',    'mutatum-desc'),
]
SIGNA = {'nomen-asc': (' ▲', ''), 'nomen-desc': (' ▼', ''), 'mutatum-desc': ('', ' ▼'), 'mutatum-asc': ('', ' ▲')}
radios = []
tabulae = []
for clavis, ordo, inversus, ad_nomen, ad_mutatum in ORDINES:
    radios.append('<input type="radio" name="ordo" id="ordo-%s" class="ordo"%s>' % (clavis, ' checked' if clavis == 'nomen-asc' else ''))
    signa = SIGNA[clavis]
    lineae = [r[2] for r in sorted(lineae_indicis, key=ordo, reverse=inversus)]
    tabulae.append('''<table class="bibliothecae ordo-%s">
<thead><tr><th><label for="ordo-%s" class="ordinator">Library%s</label></th><th>Gloss</th><th><label for="ordo-%s" class="ordinator">Last change%s</label></th><th>Files</th></tr></thead>
<tbody>
%s
</tbody>
</table>''' % (clavis, ad_nomen, signa[0], ad_mutatum, signa[1], '\n'.join(lineae)))
corpus_tabularum = '\n'.join(radios) + '\n\n' + '\n\n'.join(tabulae)

open(os.path.join(dir_folii, 'index.html'), 'w').write('''<!doctype html>
%(nota)s
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Bibliothecae</title>
<link rel="stylesheet" href="../stilus.css">
</head>
<body>

<p class="nota"><a href="../index.html">Rhubarb</a></p>
<h1>Bibliothecae</h1>
<p class="subtitulus">The libraries in include/: %(n)d headers, %(d)d described</p>

<p>One row per header in <code>include/</code> (the subsystems with folders
of their own, such as toml, crusta, silva and tessera, are not listed
here). The gloss is the first sentence of the header's opening comment,
and %(sine)d headers have none, most of them opening straight into their
include guard; a linked name has a page. Last change and implementation files are derived (git, and the
symbol index: a file is listed when it defines a function the header
declares). Every header has a one-paragraph description, written in
batches from 2026-10-02 and kept current after each merge of main. Source: the ledger
inventory <em>bibliothecae (include)</em>.</p>

<p class="nota">Click <em>Library</em> or <em>Last change</em> to sort; click again to reverse. %(ad_greges)s</p>

%(lineae)s

</body>
</html>
''' % {'nota': NOTA_GENERATUM, 'n': len(ordines), 'd': descriptae, 'lineae': corpus_tabularum,
   'sine': sum(1 for v in ordines if not glossa_capitis(v)),
   'ad_greges': ('Or see them <a href="greges.html">grouped by area</a>. ' if greges else '') + 'The four apps built from them have <a href="applicationes.html">a page of their own</a>, so do <a href="provinciae.html">the fifteen sub-projects</a> with folders of their own, and <a href="instrumenta.html">the tools</a> that operate them.'})
print('bibliothecae: %d, descriptae %d' % (len(ordines), descriptae))


# greges.html: bibliothecae per gregem, ordine lentis 'ordo', cum introductione
if greges:
    ordo_gregum = sorted(greges, key=lambda g: numerus_romanus(greges[g].get('ordo', '')))
    membra = {}
    for via in ordines:
        membra.setdefault(cellae[via].get('grex', ''), []).append(via)
    sectiones = []
    tabula_contentorum = []
    for g in ordo_gregum:
        gr = greges[g]
        vv = membra.get(g, [])
        descriptae_g = sum(1 for v in vv if 'descriptio' in cellae[v])
        tabula_contentorum.append('<li><a href="#%s">%s</a> <span class="nota">(%d)</span></li>' % (g, html.escape(gr.get('titulus', g)), len(vv)))
        lineae = []
        for v in vv:
            nomen = v[len('include/'):-2]
            c = cellae[v]
            via_folii = os.path.join(dir_folii, nomen + '.html')
            habet = 'descriptio' in c or manu_scriptum(via_folii)
            titulus = ('<a href="%s.html">%s</a>' % (nomen, nomen)) if habet else nomen
            if 'descriptio' in c:
                summa = html.escape(sententia_prima(c['descriptio']))
            elif glossa_capitis(v):
                summa = '<span class="nota">%s</span>' % html.escape(glossa_capitis(v))
            else:
                summa = '<span class="nota">—</span>'
            lineae.append('<tr%s><td>%s</td><td>%s</td></tr>' % (' class="descripta"' if habet else '', titulus, summa))
        sectiones.append('''<h2 id="%(g)s">%(ordo)s. %(titulus)s</h2>
<p class="nota">%(n)d libraries, %(d)d described</p>
<p>%(intro)s</p>
<table class="bibliothecae">
<thead><tr><th>Library</th><th>In one sentence <span class="nota">(or the header's own gloss)</span></th></tr></thead>
<tbody>
%(lineae)s
</tbody>
</table>''' % {'g': g, 'ordo': html.escape(gr.get('ordo', '')), 'titulus': html.escape(gr.get('titulus', g)),
              'n': len(vv), 'd': descriptae_g, 'intro': html.escape(gr.get('introductio', '')), 'lineae': '\n'.join(lineae)})
    sine_grege = membra.get('', [])
    open(os.path.join(dir_folii, 'greges.html'), 'w').write('''<!doctype html>
%(nota)s
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Bibliothecae per greges</title>
<link rel="stylesheet" href="../stilus.css">
</head>
<body>

<p class="nota"><a href="../index.html">Rhubarb</a> · <a href="index.html">Bibliothecae</a></p>
<h1>Bibliothecae per greges</h1>
<p class="subtitulus">The %(n)d libraries in include/, in %(k)d groups</p>

<p>The same libraries as the <a href="index.html">table</a>, clustered by
area, each group with a paragraph on what it is and how its libraries
relate. The grouping is a first cut (Claude, 2026-10-03), meant to be
revised; it lives in the ledger (the lens <em>grex</em> on
<em>bibliothecae (include)</em>, and the inventory <em>greges
bibliothecarum</em>), not in this page.%(sine)s</p>

<ul class="greges">
%(toc)s
</ul>

%(sectiones)s

</body>
</html>
''' % {'nota': NOTA_GENERATUM, 'n': len(ordines), 'k': len(ordo_gregum), 'toc': '\n'.join(tabula_contentorum),
       'sectiones': '\n\n'.join(sectiones),
       'sine': (' %d not yet grouped.' % len(sine_grege)) if sine_grege else ''})
    print('greges: %d, sine grege %d' % (len(ordo_gregum), len(sine_grege)))

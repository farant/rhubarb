"""pagina_briar.py - folium briar: facultates ex inventario 'briar facultates'.

Usus: python3 pagina_briar.py <inventarium.tsv> <radix arboris>

Inventarium (tabularium, exportatum per ./gesta/frigida.sh -inventarium
'briar facultates' EX ARBORE PRINCIPALI): ordo = facultas (slug), lentes
area, status (aedificatum/partim/dilatum), versio (v1-v4, inedita, -),
descriptio, fons. Exempla (salve, fragmenta) ex fixis briar ipsis
leguntur, numquam copiantur. Scribit docs/bibliothecae/briar.html.
Area aut status ignotus = RECUSATIO (nihil scribitur).
"""
import html, os, re, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import facultates

via_tsv, radix = sys.argv[1], sys.argv[2]
NOTA = '<!-- GENERATUM: docs/instrumenta/pagina_briar.py - noli manu mutare -->'

AREAE = [
    ('forma', 'The format'),
    ('regiones', 'Regions and elements'),
    ('contextus', 'Fragments (literate weaving)'),
    ('bibliotheca', 'Sharing code between thistles'),
    ('c', 'C in regions'),
    ('fabrica', 'Building, caching, running'),
    ('inspicere', 'Inspecting a build'),
    ('documentatio', 'Documentation for script writers'),
    ('facies', 'The literate page (-html)'),
    ('visio', 'The viewer (-visio) and Visio'),
    ('app', 'App bundles (-app)'),
    ('diagnostica', 'Diagnostics'),
    ('futura', 'Named futures'),
]
VERSIONES = ['v1', 'v2', 'v3', 'v4', 'inedita', '—']
TITULI_VERSIONUM = {'v1': 'v1 (2026-09-24)', 'v2': 'v2 (2026-09-26)', 'v3': 'v3 (2026-09-28)',
                    'v4': 'v4 (2026-09-29)', 'inedita': 'unreleased (built since v4)',
                    '—': 'no version (deferred, or partial without a release)'}

ordines, cellae = facultates.legere(via_tsv)
facultates.iudicare('pagina_briar', ordines, cellae, dict(AREAE), VERSIONES)
textus = facultates.codex_involvere(
    r'(#!/usr/bin/env briar|#!|&lt;&lt;#\w+&gt;&gt;|&lt;[^&\s][^<>]*?&gt;|(?<![\w/.-])--?[a-z]*\b(?!-)|[\w./~-]+\.thistle\b|~/\S+/)')
tabulae, status_html, index_arearum, numeri = facultates.tabulae(
    ordines, cellae, AREAE, VERSIONES, TITULI_VERSIONUM, textus)

FIXA = os.path.join(radix, 'briar/probationes/fixa/thistle')
salve = open(os.path.join(FIXA, 'salve.thistle')).read()
frag = open(os.path.join(FIXA, 'fragmenta.thistle')).read()
frag = frag[:frag.index('redde summa;')] + 'redde summa;\n}\n</c>'
frag = frag[frag.index('<c! id="incrementum">'):]
frag = re.sub(r'\n<c! id="capita">.*?\n<c!>\n<<#capita>>\n\n', '\n<c!>\n', frag, flags=re.S)

open(os.path.join(radix, 'docs/bibliothecae/briar.html'), 'w').write('''<!doctype html>
%(nota)s
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Briar</title>
<link rel="stylesheet" href="../stilus.css">
</head>
<body>

<p class="nota"><a href="../index.html">Rhubarb</a> · <a href="index.html">Bibliothecae</a> · <a href="provinciae.html#briar">Provinciae</a></p>
<h1>Briar</h1>
<p class="subtitulus">Literate C89 scripts, and everything a <code>.thistle</code> can do: %(n)d features</p>

<p>briar runs <code>.thistle</code> files: a C89 program written as a
document. Prose is Markdown, configuration is STML tags, the code sits in
raw <code>&lt;c!&gt;</code> regions, and a <code>#!</code> first line makes
the file executable. <code>./x.thistle</code> builds once, into a cache
keyed by the file's content, and from then on simply runs, so a C89 program
can be written and handed around like a script. The whole house library
(<code>include/</code> and <code>lib/</code>) is embedded in the briar
binary and available to every script, with no <code>#include</code> needed
for house headers. It was Fran's idea of 2026-09-04; outside testers (the
lapide project) have used it since, and their feedback shaped versions 2
to 4.</p>

<h2>A thistle</h2>
<p>The smallest complete example, from briar's own test fixtures: one
region is the program, the other its test (run with
<code>./salve.thistle -probatio</code>).</p>
<pre><code>%(salve)s</code></pre>

<p>Regions can be split into named <em>fragments</em> and woven back
together, literate programming in Knuth's sense: a line
<code>&lt;&lt;#name&gt;&gt;</code> inside any C region stands for the
fragment of that name, indented like the reference. An excerpt from the
fragments fixture:</p>
<pre><code>%(frag)s</code></pre>

<h2>The laws of the format</h2>
<ul>
<li><strong>Column 0 is the only place briar looks.</strong> A region or
element opens only when a line begins with <code>&lt;</code> and a name.
Everything else is prose, which needs no escaping.</li>
<li><strong>A raw region ends only at a column-0 closing tag</strong>, so a
C string containing <code>&lt;/c&gt;</code> cannot end it early.</li>
<li><strong>Every file is a tree.</strong> Even a broken file parses (an
unterminated region runs to the end, with a named fault), and writing the
tree back gives the source byte for byte.</li>
<li><strong>One grammar for attributes</strong>: STML's. HTML in prose is
text, never rendered; HTML lives in regions.</li>
</ul>

<h2>The command line</h2>
<table>
<thead><tr><th>Command</th><th>Does</th></tr></thead>
<tbody>
<tr><td><code>./x.thistle [args]</code></td><td>build if needed, then become the program (same process)</td></tr>
<tr><td><code>briar -probatio x.thistle</code></td><td>build and run the file's test region</td></tr>
<tr><td><code>briar -struere [-iterum]</code></td><td>build without running; <code>-iterum</code> forces a rebuild</td></tr>
<tr><td><code>briar -arbor</code> / <code>-partes</code></td><td>print the parse tree / the closure (headers, library members, unused fragments)</td></tr>
<tr><td><code>briar -amalgama</code></td><td>write one self-contained <code>.c</code> that clang compiles alone</td></tr>
<tr><td><code>briar -html</code> / <code>-visio</code></td><td>write the literate page / open it in a window</td></tr>
<tr><td><code>briar -app [-icon x.png]</code></td><td>make a macOS <code>.app</code> bundle beside the thistle</td></tr>
<tr><td><code>briar -bibliothecae</code>, <code>-bibliotheca x [-fons | -functiones]</code></td><td>list the libraries / read one library's header, sources or signatures</td></tr>
<tr><td><code>briar -dialectus</code></td><td>the dialect card: integer types, clang flags, the latina.h words, the C89 traps</td></tr>
<tr><td><code>briar -versio</code> / <code>-mutationes</code></td><td>the version / the changelog (<code>briar/MUTATIONES.md</code>, embedded)</td></tr>
</tbody>
</table>

<h2>Feature inventory</h2>
<p>Each row is one thing a script writer can rely on (or should know is
missing), with the version that brought it and where it is specified or
tested: "spec §N" is <code>project-specs/briar-spec.md</code>,
"MUTATIONES" the changelog, "fumus" a stage of the end-to-end smoke test
(<code>tools/briar_fumus.sh</code>), "gate" a suite of
<code>briar/compile_probationes.sh</code>.</p>
<table>
<thead><tr><th>Status</th><th></th><th>Rows</th><th>Meaning</th></tr></thead>
<tbody>
%(status)s
</tbody>
</table>
<p class="nota">Areas: %(index_arearum)s</p>
<p class="nota">Click <em>Feature</em> to group by area, <em>Status</em>
to see what is built and what is not, <em>Since</em> to read it as a
history.</p>

%(tabulae)s

<p class="nota">Generated from the ledger inventory "briar facultates"
(written 2026-10-03 from the spec v1.11, <code>briar/CLAUDE.md</code> and
<code>briar/MUTATIONES.md</code>) and briar's own fixtures; regenerate with
<code>./docs/instrumenta/regenerare.sh</code>. The authority is the spec
and the changelog: when they disagree with a row here, the row is
wrong.</p>

</body>
</html>
''' % {'nota': NOTA, 'n': len(ordines), 'salve': html.escape(salve.rstrip('\n'), quote=False),
       'frag': html.escape(frag, quote=False), 'status': status_html,
       'index_arearum': index_arearum, 'tabulae': tabulae})
print(facultates.compendium('briar', ordines, numeri))

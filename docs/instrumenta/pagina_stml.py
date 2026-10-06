"""pagina_stml.py - folium STML: itinerarium syntaxis + facultates ex inventario 'stml facultates'.

Usus: python3 pagina_stml.py <inventarium.tsv> <radix arboris>

Exempla itinerarii hic scripta sunt et AD TEMPUS GENERATIONIS per
instrumenta vera currunt (stml formare/expandere/vertere, canon_examen):
quod pagina ostendit est quod instrumentum hodie reddit, numquam copia
manu facta. Exitus exspectatus discrepans = RECUSATIO (nihil scribitur).
Exempla in directorio temporario scribuntur, numquam in arbore (vitia
deliberata plagulas .stml arboris non fiunt).

Instrumenta: STML (ordinarie <radix>/bin/stml, deinde ~/.bin/stml) et
CANON_EXAMEN (ordinarie <radix>/bin/canon_examen, deinde
$TABULARIUM_RADIX/bin/canon_examen). Scribit
docs/bibliothecae/stml_facultates.html.
"""
import html, os, re, subprocess, sys, tempfile
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import facultates

via_tsv, radix = sys.argv[1], sys.argv[2]
NOTA = '<!-- GENERATUM: docs/instrumenta/pagina_stml.py - noli manu mutare -->'


def instrumentum(env, *viae):
    if os.environ.get(env):
        return os.path.abspath(os.environ[env])
    for v in viae:
        if v and os.access(v, os.X_OK):
            return os.path.abspath(v)
    sys.exit('pagina_stml: instrumentum %s non inventum (%s) - recuso' % (env, ', '.join(v for v in viae if v)))


STML = instrumentum('STML', os.path.join(radix, 'bin/stml'), os.path.expanduser('~/.bin/stml'))
CANON = instrumentum('CANON_EXAMEN', os.path.join(radix, 'bin/canon_examen'),
                     os.path.join(os.environ.get('TABULARIUM_RADIX', ''), 'bin/canon_examen')
                     if os.environ.get('TABULARIUM_RADIX') else '')

AREAE = [
    ('documentum', 'The document: elements and attributes'),
    ('textus', 'Text and whitespace'),
    ('capturae', 'Captures'),
    ('fragmenta', 'Fragments and names'),
    ('scriptor', 'Writing it back'),
    ('templa', 'Templates'),
    ('distributio', 'Lists (DISTRIBUTIO)'),
    ('exemplaria', 'Patterns and questions'),
    ('html', 'HTML emission'),
    ('canon', 'Canon (schemas)'),
    ('selectio', 'Selectors'),
    ('instrumenta', 'Tools and API'),
    ('futura', 'Reserved and refused forms'),
]

ordines, cellae = facultates.legere(via_tsv)
menses = sorted({c['versio'] for c in cellae.values() if re.match(r'\d{4}-\d\d$', c['versio'])})
VERSIONES = menses + ['—']
TITULI_VERSIONUM = dict((m, m) for m in menses)
TITULI_VERSIONUM['—'] = 'undated (reserved, refused, or no single commit introduced it)'
facultates.iudicare('pagina_stml', ordines, cellae, dict(AREAE), VERSIONES)
textus = facultates.codex_involvere(
    r'(&lt;&lt;#@?\w*[^<>]*?&gt;&gt;|&lt;[^\s<>][^<>]*?&gt;|&amp;@[\w.!]+;|&amp;[a-z]+;|\$\w+'
    r'|(?:bin|lib|include|tools)/[\w./]+|\b[\w-]+\.(?:stml|canon|registrum)\b|\bstml_[a-z_]+'
    r'|\b[A-Z]{2,}(?:_[A-Z]+)*\b(?![a-z])|\b\w+="[^"]*"|stml (?:formare|expandere|vertere)'
    r'|(?<![\w/.-])-[a-z]+\b(?!-))')
tabulae, status_html, index_arearum, numeri = facultates.tabulae(
    ordines, cellae, AREAE, VERSIONES, TITULI_VERSIONUM, textus)

# ---------------------------------------------------------------------------
# exempla: (via, textus) plagulae; cursus: (argv, exitus exspectatus)

EXEMPLA = {}

EXEMPLA['liber'] = ({'liber.stml': '''<liber lingua=la publicus>
  <titulus>De Agri Cultura</titulus>
  <auctor>Cato</auctor>
  <pagina numerus="1">Est interdum praestare mercaturis rem quaerere,
    &lt;nisi&gt; tam periculosum siet.</pagina>
  <nota>brevis</>
  <vacuum></vacuum>
</liber>
'''}, [(['stml', 'formare', 'liber.stml'], 0)])

EXEMPLA['textus'] = ({'textus.stml': '''<exempla>
  <prosa>Haec linea
    et haec una sententia sunt.</prosa>
  <versus\\>
    Arma virumque cano,
      Troiae qui primus ab oris
  </versus>
  <codex!>if (a < b && c) { x = "<b>"; }</codex>
  <scriptum!>raw text may hold <\\/scriptum> itself</scriptum>
</exempla>
'''}, [(['stml', 'formare', 'textus.stml'], 0), (['stml', 'vertere', 'textus.stml'], 0)])

EXEMPLA['capturae'] = ({'capturae.stml': '''<capturae>
  <sectio (>
  <titulus>Prima</titulus>
  <i>p</i><) nota>
  <i>a</i><= et =><i>b</i>
  <linea! (>  raw to the end of the line, <not> a tag
</capturae>
'''}, [(['stml', 'vertere', 'capturae.stml'], 0)])

EXEMPLA['attributa'] = ({'nexus.stml': '''<p>
  <a><@href=>https://exemplum.org/?q="verbum"</>nexus</a>
</p>
'''}, [(['stml', 'vertere', 'nexus.stml'], 0)])

EXEMPLA['fragmenta'] = ({'fragmenta.stml': '''<pagina>
  <#salutatio>
    <p>Salve, lector.</p>
  </#>
  <caput><<#salutatio>></caput>
  <canis nomen="&laika;" genus=".canis"/>
</pagina>
'''}, [(['stml', 'formare', 'fragmenta.stml'], 0), (['stml', 'vertere', 'fragmenta.stml'], 2)])

EXEMPLA['templum'] = ({'chartae.stml': '''<#@charta nomen="@nomen" gradus="@gradus?" corpus="@corpus">
  <div class="charta">
    <h2>&@nomen;</h2>
    <COMMUTATIO de="&@gradus;">
      <CASUS est="alta"><p class="monitum">gradus altus</p></CASUS>
      <CASUS non-nihil><p>gradus: &@gradus;</p></CASUS>
      <CASUS nihil><p>sine gradu</p></CASUS>
    </COMMUTATIO>
    &@corpus;
  </div>
</#>
<paginae>
  <<#@charta nomen="silva" gradus="alta">><@corpus=><p>Parsator C89.</p></>
  <<#@charta nomen="materia">><@corpus=><p>Substratum arborum.</p></>
</paginae>
'''}, [(['stml', 'vertere', 'chartae.stml'], 0)])

EXEMPLA['distributio'] = ({'index.stml': '''<ul>
  <li class="provincia">
    <>silva</>
    <><@class=>provincia nova</>briar</>
    <><@class=/>materia</>
  </li>
</ul>
'''}, [(['stml', 'vertere', 'index.stml'], 0)])

EXEMPLA['quaestio'] = ({'bibliotheca.stml': '''<bibliotheca>
  <liber id="cato"><titulus>De Agri Cultura</titulus><recensio>bona</recensio></liber>
  <liber id="varro"><titulus>Res Rusticae</titulus></liber>
  <liber id="plinius"><titulus>Naturalis Historia</titulus></liber>
</bibliotheca>
<CATENA output="$sine">
  <(><EXEMPLAR><liber id="$id"><titulus>$t</titulus></liber></EXEMPLAR>
  <(><SINE><recensio/></SINE>
</CATENA>
<ul>
  <PER congruentia="$sine"><li>&@t; (&@id;)</li></PER>
</ul>
'''}, [(['stml', 'vertere', 'bibliotheca.stml'], 0)])

EXEMPLA['refutatio'] = ({'ordo.stml': '''<pagina>
  <<#@charta nomen="silva">>
</pagina>
<#@charta nomen="@nomen"><h2>&@nomen;</h2></#>
'''}, [(['stml', 'vertere', 'ordo.stml'], 2)])

EXEMPLA['canon'] = ({'libri.canon': '''<canon dialectus="libri" versio="1">
  <elementum nomen="libri" radix="verum">
    <liberum nomen="liber" minimum="1"/>
  </elementum>
  <elementum nomen="liber">
    <attributum nomen="id" genus="nomen" necessarium="verum"/>
    <attributum nomen="annus" genus="numerus"/>
    <liberum nomen="titulus" minimum="1" maximum="1"/>
  </elementum>
  <elementum nomen="titulus" textus="verum"/>
  <unicitas nomen="libri" attributum="id" super="liber"/>
</canon>
''', 'libri.stml': '''<libri>
  <liber id="cato" annus="-160"><titulus>De Agri Cultura</titulus></liber>
  <liber id="cato" annus="circa 37"><titulus>Res Rusticae</titulus><auctor>Varro</auctor></liber>
  <liber><titulus>Naturalis Historia</titulus><titulus>bis</titulus></liber>
</libri>
'''}, [(['canon_examen', '-canon', 'libri.canon', 'libri.stml'], 1)])

INSTRUMENTA = {'stml': STML, 'canon_examen': CANON}


def currere(clavis):
    plagulae, cursus = EXEMPLA[clavis]
    d = tempfile.mkdtemp(prefix='stml_exemplum_')
    for via, t in plagulae.items():
        open(os.path.join(d, via), 'w').write(t)
    partes = []
    for via, t in plagulae.items():
        partes.append('<p class="nota">%s</p>\n<pre><code>%s</code></pre>' % (via, html.escape(t.rstrip('\n'), quote=False)))
    for argv, exspectatus in cursus:
        r = subprocess.run([INSTRUMENTA[argv[0]]] + argv[1:], cwd=d, capture_output=True, text=True)
        if r.returncode != exspectatus:
            sys.exit('pagina_stml: exemplum %s: %s exitus %d, exspectatus %d - recuso\n%s%s' % (
                clavis, ' '.join(argv), r.returncode, exspectatus, r.stdout, r.stderr))
        if not (r.stdout + r.stderr).strip():
            sys.exit('pagina_stml: exemplum %s: %s nihil effudit - recuso' % (clavis, ' '.join(argv)))
        effusio = (r.stdout + r.stderr).rstrip('\n')
        effusio = effusio.replace(d + '/', '')
        partes.append('<p class="nota">$ %s%s</p>\n<pre class="effusio"><code>%s</code></pre>' % (
            html.escape(' '.join(argv)), '' if r.returncode == 0 else ' <span class="nota">(exit %d)</span>' % r.returncode,
            html.escape(effusio, quote=False)))
    for via in plagulae:
        os.remove(os.path.join(d, via))
    os.rmdir(d)
    return '\n'.join(partes)


ex = dict((k, currere(k)) for k in EXEMPLA)


def provenientia(via):
    r = subprocess.run([via, '-provenientia'], capture_output=True, text=True)
    m = re.search(r'^commissum (.*)$', r.stdout, re.M)
    return '%s (built from %s)' % (via.replace(os.path.expanduser('~'), '~'), m.group(1)) if m else via

testes = '; '.join(provenientia(v) for v in (STML, CANON))

open(os.path.join(radix, 'docs/bibliothecae/stml_facultates.html'), 'w').write('''<!doctype html>
%(nota)s
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>STML</title>
<link rel="stylesheet" href="../stilus.css">
</head>
<body>

<p class="nota"><a href="../index.html">Rhubarb</a> · <a href="index.html">Bibliothecae</a> · <a href="stml.html">stml.h</a></p>
<h1>STML</h1>
<p class="subtitulus">The house's one data syntax: a tour of the syntax, and an inventory of %(n)d features</p>

<p>STML looks like XML and is the format of almost everything the house
writes down that is not C: build declarations, schemas, the knowledge
base's genera, the trees silva and materia project from source code,
the HTML template behind the Markdown pages, the elements in a briar
script. It is one syntax carrying three layers, each a pure function from
a tree to a tree:</p>
<table>
<thead><tr><th>Layer</th><th>Does</th><th>Library</th><th>Verb</th></tr></thead>
<tbody>
<tr><td>document</td><td>text &#8644; tree, byte-faithful</td><td><code>lib/stml.c</code></td><td><code>stml formare</code></td></tr>
<tr><td>expansion</td><td>templates filled, questions answered, lists distributed</td><td><code>lib/stml_macros.c</code></td><td><code>stml expandere</code></td></tr>
<tr><td>emission</td><td>tree &#8594; HTML</td><td><code>lib/stml_html.c</code></td><td><code>stml vertere</code></td></tr>
</tbody>
</table>
<p>Beside them, <strong>canon</strong> judges a document against a schema
that is itself written in STML, and <strong>selectio</strong> matches
CSS-style selectors. Five laws shape everything below: files are the truth
and every other view is a projection; there is no expression language;
every failed reference is loud; writing without the pretty-printer gives
back the input byte for byte; and forms that are reserved but unbuilt are
written down anyway.</p>

<p class="nota">Every example on this page was run through the real
tools when the page was generated; what you see under each <code>$</code>
line is their actual output. Longer treatments:
<code>docs/stml-tutorial.md</code> (teaches, chapter by chapter) and
<code>docs/stml-reference.md</code> (states every rule and refusal).</p>

<h2>1. A first document</h2>
<p>Elements open and close like HTML. <code>&lt;/&gt;</code> closes
whatever is open. Attribute values are double-quoted or, for simple words,
bare; a name alone is a boolean. In text, <code>&lt;</code> must be
written <code>&amp;lt;</code>. The formatter rewrites the file into the
house layout: an element with a single child takes the <em>capture</em>
form <code>&lt;t(&gt;</code> (section 3), prose is re-wrapped, an empty
pair becomes <code>&lt;t/&gt;</code>. Both spellings are the same tree.</p>
%(liber)s

<h2>2. Four kinds of text</h2>
<p>Whitespace that contains a newline is <em>layout</em>; whitespace on
one line is <em>content</em>. So in a plain element, prose can be broken
across lines freely: it reads as one line. <code>&lt;t\\&gt;</code> keeps
the line breaks and removes the common indentation (poems, code examples).
<code>&lt;t!&gt;</code> is raw: nothing inside is a tag or an entity, so
code needs no escaping. A raw element can even contain its own closing
tag, written <code>&lt;\\/t&gt;</code>; the reader removes one backslash,
the writer adds it back.</p>
%(textus)s

<h2>3. Captures: nesting without closing tags</h2>
<p><code>&lt;t (&gt;</code> takes the next sibling as its child;
<code>(( </code> and <code>(((</code> take two or three.
<code>&lt;) t&gt;</code> takes the previous sibling, <code>&lt;= t
=&gt;</code> one on each side, and <code>&lt;t! (&gt;</code> the rest of
the line as raw text. They are written glued: a same-line space is
content, and would be the sibling captured. HTML emission shows the
resulting nesting with real close tags:</p>
%(capturae)s

<h2>4. Attributes as elements</h2>
<p>An attribute value cannot contain <code>"</code>, and cannot sensibly
hold a paragraph. <code>&lt;@name=&gt;…&lt;/&gt;</code> spells an
attribute as a child element, allowed only as the first children of its
element (or as a template's argument, section 6). <code>&lt;@name=/&gt;</code>
is a <em>tombstone</em>: explicitly absent (section 7 uses one).</p>
%(attributa)s

<h2>5. Fragments, and five kinds of name</h2>
<p><code>&lt;#id&gt;…&lt;/#&gt;</code> is a fragment: children grouped with
no element of their own. <code>&lt;&lt;#id&gt;&gt;</code> is a
transclusion, a window onto that fragment (aliasing, not copying). The
library keeps the window and resolves nothing; each consumer decides what
it means, which is why <code>vertere</code> refuses one by name. Each kind
of name has its own sigil:</p>
<table>
<thead><tr><th>Sigil</th><th>Names</th><th>Example</th></tr></thead>
<tbody>
<tr><td><code>#x</code></td><td>something in this document</td><td><code>&lt;#intro&gt;</code>, <code>&lt;&lt;#intro&gt;&gt;</code></td></tr>
<tr><td><code>&amp;x;</code></td><td>an individual in the world</td><td><code>nomen="&amp;laika;"</code></td></tr>
<tr><td><code>.x</code></td><td>a kind</td><td><code>genus=".canis"</code>, <code>&lt;.species&gt;</code></td></tr>
<tr><td><code>&amp;@x;</code></td><td>a template slot</td><td>section 6</td></tr>
<tr><td><code>$x</code></td><td>a pattern capture</td><td>section 8</td></tr>
</tbody>
</table>
%(fragmenta)s

<h2>6. Templates</h2>
<p><code>&lt;#@name slot="@slot"&gt;</code> defines a template; slots are
declared on the opening tag (<code>@slot?</code> is optional) and used as
<code>&amp;@slot;</code>. <code>&lt;&lt;#@name a="x"&gt;&gt;</code> calls
it; block arguments follow the call as attribute-elements and can hold
whole subtrees. <code>COMMUTATIO</code> chooses between arms; an optional
slot may only be used inside an arm that proves it present. There is no
expression language: arms compare literals, presence, tag names or
patterns, nothing else.</p>
%(templum)s

<h2>7. Lists without repeating yourself</h2>
<p>An element whose children are anonymous fragments
<code>&lt;&gt;…&lt;/&gt;</code> dissolves into one copy per item, each
inheriting the wrapper's attributes. An item's own attribute wins; a
tombstone removes the inherited one.</p>
%(distributio)s

<h2>8. Asking a document questions</h2>
<p><code>EXEMPLAR</code> is a pattern: an element written as you expect
to find it, with <code>$x</code> where a value should be captured. Its
answer is a set of rows. <code>CATENA</code> threads stages,
<code>SINE</code> keeps the rows inside which something is <em>absent</em>,
and <code>PER</code> writes its body once per row. Here: the books
without a review.</p>
<p><strong>Where the input comes from.</strong> A command names no
target. An <code>EXEMPLAR</code> (or the first link of a
<code>CATENA</code>) searches <em>everything above it in the
document</em>, at every depth, so a <code>&lt;liber&gt;</code> anywhere
above would match, not only those in the element just before it; anything
below the command is invisible to it. Each later link of a
<code>CATENA</code> takes the previous link's rows as its input and
searches inside them: here <code>SINE</code> looks inside each
<code>&lt;liber&gt;</code> the first link found. To narrow the scope, add a
first stage that matches the container (<code>&lt;EXEMPLAR
ancorata&gt;&lt;bibliotheca/&gt;&lt;/EXEMPLAR&gt;</code>), or start from an
earlier result with <code>de="$x"</code>. <code>output="$x"</code> names
the result, and something below (here <code>PER</code>) must consume it.
Because a command sees only what is above it, a document is processed in
one pass, top to bottom.</p>
%(quaestio)s
<p class="nota">Also: <code>modus</code> (how many answers are expected),
<code>&lt;*/&gt;</code> and <code>&lt;**&gt;</code> wildcards,
<code>cursus="fratrum"</code> for runs of siblings,
<code>DIRIBITIO</code> to dispatch by condition, <code>TRANSPARENTIA</code>
to ignore wrappers, and <code>&lt;INDAGO/&gt;</code> to trace every stage.
See the inventory below.</p>

<h2>9. Refusals are loud</h2>
<p>Nothing fails quietly. A template is visible only below its
definition (so expansion always terminates), and calling one too early is
an error with a name, a line and the template, never an empty result.
The expansion layer has 31 named errors.</p>
%(refutatio)s

<h2>10. Contracts: canon</h2>
<p>A canon is a schema written in STML: which elements exist, their
attributes and value kinds, which children and how many, which keys must
be unique or must refer to something. <code>canones.registrum</code> maps
the house's dialects to their canons, and <code>canon.canon</code> describes
the canon language itself. There is deliberately no expression language
here either; rules that need one live in the reading program and its
tests.</p>
%(canon)s

<h2>Tools</h2>
<table>
<thead><tr><th>Command</th><th>Does</th></tr></thead>
<tbody>
<tr><td><code>stml formare x.stml [-probare]</code></td><td>format into the house layout, no expansion; <code>-probare</code> only checks (exit 1 if it would change)</td></tr>
<tr><td><code>stml expandere x.stml</code></td><td>expand templates, answer questions, distribute lists; write STML</td></tr>
<tr><td><code>stml vertere x.stml</code></td><td>the whole chain, then HTML</td></tr>
<tr><td><code>bin/canon_examen x.stml</code></td><td>judge by the registry (<code>-canon X</code> explicit, <code>-machina</code> TSV)</td></tr>
<tr><td><code>bin/canon_coquere</code></td><td>generate typed C readers from a canon</td></tr>
<tr><td><code>silex iudicare x.stml</code></td><td>judge from outside the repository with the bundled canons</td></tr>
</tbody>
</table>
<p class="nota">Exit codes: 0 done or conforming, 1 divergent or faults found,
2 refused (with <code>file:line</code> on stderr) or nothing judged.</p>

<h2>Feature inventory</h2>
<p>Each row is one construct or rule, with the month it first appeared in
the C library (found with <code>git log -S</code> on a token that
introduced it; STML began as TypeScript before the C library of
2025-11, so the date is the C port's or later) and the section of
<code>docs/stml-reference.md</code> that states it.</p>
<table>
<thead><tr><th>Status</th><th></th><th>Rows</th><th>Meaning</th></tr></thead>
<tbody>
%(status)s
</tbody>
</table>
<p class="nota">Areas: %(index_arearum)s</p>
<p class="nota">Click <em>Feature</em> to group by area, <em>Status</em>
to see what is built and what is not, <em>Since</em> to read it month by
month.</p>

%(tabulae)s

<p class="nota">Generated from the ledger inventory "stml facultates"
(written 2026-10-03 from <code>docs/stml-reference.md</code>, the STML
specs and the code) with examples run through %(testes)s; regenerate with
<code>./docs/instrumenta/regenerare.sh</code>. The authority is the code
and the reference: when they disagree with a row here, the row is
wrong.</p>

</body>
</html>
''' % dict(ex, nota=NOTA, n=len(ordines), status=status_html, index_arearum=index_arearum,
           tabulae=tabulae, testes=html.escape(testes)))
print(facultates.compendium('stml', ordines, numeri) + '; exempla %d currunt' % len(EXEMPLA))

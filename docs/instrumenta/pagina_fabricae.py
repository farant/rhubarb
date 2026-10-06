"""pagina_fabricae.py - folium fabricae: facultates ex inventario 'fabrica facultates'.

Usus: python3 pagina_fabricae.py <inventarium.tsv> <radix arboris>

Inventarium (tabularium, exportatum per ./gesta/frigida.sh -inventarium
'fabrica facultates' EX ARBORE PRINCIPALI): ordo = facultas (slug),
lentes area, status (aedificatum/partim/dilatum), versio (segmentum: 1a,
1b, 2, post-2, -), descriptio, fons. Exempla declarationum (actio manus,
compositum briar, actio cum lectionibus) ex aedificatio.stml ipsis
leguntur, numquam copiantur; numeri actionum et exituum ex declarationibus
numerantur. Scribit docs/bibliothecae/fabrica_facultates.html. Area, status aut
versio ignota = RECUSATIO (nihil scribitur).
"""
import html, os, re, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import facultates

via_tsv, radix = sys.argv[1], sys.argv[2]
NOTA = '<!-- GENERATUM: docs/instrumenta/pagina_fabricae.py - noli manu mutare -->'

AREAE = [
    ('modelum', 'The model'),
    ('declarationes', 'Declarations'),
    ('ingressus', 'Inputs and digests'),
    ('iudex', 'The judge (iudicare)'),
    ('lectiones', 'Observed reads'),
    ('sanatio', 'Healing (sanare)'),
    ('provenientia', 'Installed binaries'),
    ('thesaurus', 'The object store'),
    ('superficies', 'Where it shows up'),
    ('migratio', 'Migrating the runners'),
    ('limites', 'Known limits and named futures'),
]
VERSIONES = ['1a', '1b', '2', 'post-2', '—']
TITULI_VERSIONUM = {'1a': 'slice 1a: the judge (2026-09-30)',
                    '1b': 'slice 1b: healing (2026-10-01)',
                    '2': 'slice 2: observed reads and the object store (2026-10-03)',
                    'post-2': 'after slice 2, on main (in progress)',
                    '—': 'no slice yet (deferred, or a limit without a release)'}

ordines, cellae = facultates.legere(via_tsv)
facultates.iudicare('pagina_fabricae', ordines, cellae, dict(AREAE), VERSIONES)
textus = facultates.codex_involvere(
    r'(bin/fabrica (?:iudicare|sanare|purgare|digestum)\b|silva\.commissio|sine_fabrica=\'&lt;causa&gt;\''
    r'|(?:bin|tools|lib|include|build)/[\w./<>&;-]*[\w>]|~/\.bin|[\w.-]+\.(?:stml|canon|db|sh)\b'
    r'|&lt;[a-z][^<>]*?&gt;|\$\(bin/fabrica quaere [^)]*\)|(?<![\w/.-])-[a-z]+\b(?!-)'
    r'|\b[A-Z]+_[A-Z_]+\b|&amp;@\w+;)')
tabulae, status_html, index_arearum, numeri = facultates.tabulae(
    ordines, cellae, AREAE, VERSIONES, TITULI_VERSIONUM, textus)

# exempla et numeri ex declarationibus ipsis
SUBSYSTEMATA = ['.', 'silva', 'tessera', 'officina']
declarationes = {s: open(os.path.join(radix, s, 'aedificatio.stml')).read() for s in SUBSYSTEMATA}
n_actionum = sum(len(re.findall(r'^\s*<actio ', d, re.M)) for d in declarationes.values())
n_exituum = sum(len(re.findall(r'^\s*<exitus ', d, re.M)) for d in declarationes.values())
n_compositorum = sum(len(re.findall(r'^\s*<compositum ', d, re.M)) for d in declarationes.values())


def excerpere(textus_decl, initium, finis):
    a = textus_decl.index(initium)
    a = textus_decl.rindex('\n', 0, a) + 1
    b = textus_decl.index(finis, a) + len(finis)
    return textus_decl[a:b]


ex_manus = excerpere(declarationes['.'], '<actio titulus="manus"', '</actio>')
ex_briar = excerpere(declarationes['.'], '<compositum titulus="briar"', '</compositum>')
ex_lectiones = excerpere(declarationes['tessera'], '<actio titulus="fragmentum_tessera_folium"', '</actio>')


def codex(t):
    return html.escape(re.sub(r'^  ', '', t, flags=re.M), quote=False)


open(os.path.join(radix, 'docs/bibliothecae/fabrica_facultates.html'), 'w').write('''<!doctype html>
%(nota)s
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Fabrica</title>
<link rel="stylesheet" href="../stilus.css">
</head>
<body>

<p class="nota"><a href="../index.html">Rhubarb</a> · <a href="index.html">Bibliothecae</a> · <a href="instrumenta.html">Instrumenta</a></p>
<h1>Fabrica</h1>
<p class="subtitulus">The house build layer, and everything it can do: %(n)d features</p>

<p>fabrica answers one question about every derived file in the house:
<em>was this made from today's inputs?</em> It was built because of a
shape of failure that kept recurring: a successful build, a stale
artifact, and no error. <code>bin/manus</code> went six weeks out of
date before anyone noticed, the house had at least six private notions
of "stale" (mtimes, existence checks, per-stage comparisons), and the
rebake after a <code>lib/</code> change was a ritual done by hand. The
answer was one concept: an artifact that knows what it was built from,
judged by the digests of its inputs, never by timestamps.</p>

<p>fabrica is a control plane for <em>derived</em> state. Sources,
commits and the ledger belong to people; fabrica declares what should be
derived from them, observes what is (<code>iudicare</code>), and
reconciles the difference (<code>sanare</code>). It sits above
<a href="aedilis.html">aedilis</a>, which stays the pure dependency
oracle, and it never commits anything.</p>

<h2>Declarations</h2>
<p>The root <code>fabrica.stml</code> names four subsystems; each declares
its actions in an <code>aedificatio.stml</code>, judged by the canon
<code>aedificatio.canon</code>. Today: %(actiones)d actions with
%(exitus)d outputs and %(composita)d composites. An installed binary: what
builds it, what it may write (its footprint), what it reads, and how its
output is judged (<code>relatio</code>: the binary reports the digest it
was built from):</p>
<pre><code>%(manus)s</code></pre>

<p>A generator whose inputs are <em>observed</em> rather than declared
(<code>lectiones="verum"</code>): fabrica keys it by what it actually read
on its last good run. The few inputs declared by hand are what tracing
cannot see:</p>
<pre><code>%(lectiones)s</code></pre>

<p>A composite names parts made by different actions, so a person can
still say "briar": <code>bin/fabrica iudicare briar</code> gives one
verdict (the worst of its parts) and <code>sanare briar</code> heals
whichever parts are stale.</p>
<pre><code>%(briar)s</code></pre>

<h2>How it knows an output is current</h2>
<table>
<thead><tr><th>Strategy</th><th>How</th><th>For</th></tr></thead>
<tbody>
<tr><td><code>regeneratio</code></td><td>rerun the producer in scratch, compare bytes exactly</td><td>committed generated files (amalgams, tables, snippets, capsules)</td></tr>
<tr><td><code>relatio</code></td><td>the binary reports its input digest (<code>-provenientia</code>)</td><td>linked binaries, which are not byte-reproducible</td></tr>
<tr><td><code>memoria</code></td><td>a record says these exact inputs were already verified</td><td>actions whose inputs are provably complete, or traced</td></tr>
<tr><td><code>ignota</code></td><td>never judged; realized first, as a precondition</td><td>things that cannot be known (an object store run blind)</td></tr>
</tbody>
</table>
<p>Verdicts: <strong>RECENS</strong> (current), <strong>STALUM</strong>
(stale, naming the input), <strong>IGNOTUM</strong> (cannot know, with
the reason; never rounded to current) and <strong>NON_IUDICATUM</strong>
(skipped by choice in the quick judge).</p>

<h2>The command line</h2>
<table>
<thead><tr><th>Command</th><th>Does</th></tr></thead>
<tbody>
<tr><td><code>bin/fabrica iudicare [x…]</code></td><td>the quick judge (~1.4 s): which artifacts are stale or unknown, and why</td></tr>
<tr><td><code>bin/fabrica iudicare -plenus [-audit] [-omnia]</code></td><td>the full judge: regenerate in scratch and compare; audit memo hits; list the current too</td></tr>
<tr><td><code>bin/fabrica iudicare -plenus -tacta VIA…</code></td><td>judge only the generated artifacts these paths touch (what a commit runs)</td></tr>
<tr><td><code>bin/fabrica sanare [-siccum] [x…]</code></td><td>heal: run what is stale in dependency order and re-judge; <code>-siccum</code> shows the plan; <code>sanare installata</code> is the rebake</td></tr>
<tr><td><code>bin/fabrica digestum TITULUS</code></td><td>an action's input-set digest (64 hex)</td></tr>
<tr><td><code>bin/fabrica purgare [-verificare]</code></td><td>trim the object store; verify every blob</td></tr>
<tr><td><code>bin/compilator</code></td><td>compile one source through the object store (a hit runs no clang)</td></tr>
<tr><td><code>x -provenientia</code></td><td>any installed house binary: the digest it was built from</td></tr>
</tbody>
</table>
<p class="nota">Exit codes everywhere: 0 all current, 1 something stale or
unknown, 2 nothing judged (absent or broken declarations).</p>

<h2>Feature inventory</h2>
<p>Each row is one thing fabrica does (or is known not to do yet), with
the slice that brought it and where it is specified or measured:
"spec v1/v2" are <code>project-specs/fabrica-spec-v1.md</code> and
<code>-v2.md</code> (slice 1a), "spec 1b" and "spec 2" the later slices;
T-numbers are plan tasks; ids like …W0ZBW are ledger records.</p>
<table>
<thead><tr><th>Status</th><th></th><th>Rows</th><th>Meaning</th></tr></thead>
<tbody>
%(status)s
</tbody>
</table>
<p class="nota">Areas: %(index_arearum)s</p>
<p class="nota">Click <em>Feature</em> to group by area, <em>Status</em>
to see what is built and what is not, <em>Since</em> to read it slice by
slice.</p>

%(tabulae)s

<p class="nota">Generated from the ledger inventory "fabrica facultates"
(written 2026-10-03 from the four fabrica specs and their "As built"
sections, <code>tools/fabrica.c</code>, <code>include/fabrica.h</code>
and the batch job …W0ZBW) and from the declarations themselves;
regenerate with <code>./docs/instrumenta/regenerare.sh</code>. The
authority is the specs' "As built" sections and the code: when they
disagree with a row here, the row is wrong.</p>

</body>
</html>
''' % {'nota': NOTA, 'n': len(ordines), 'actiones': n_actionum, 'exitus': n_exituum,
       'composita': n_compositorum, 'manus': codex(ex_manus), 'lectiones': codex(ex_lectiones),
       'briar': codex(ex_briar), 'status': status_html, 'index_arearum': index_arearum,
       'tabulae': tabulae})
print(facultates.compendium('fabrica', ordines, numeri) + '; declarationes: %d actiones, %d exitus, %d composita'
      % (n_actionum, n_exituum, n_compositorum))

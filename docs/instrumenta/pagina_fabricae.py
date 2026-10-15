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
    ('probationes', 'Tests and gates'),
    ('superficies', 'Where it shows up'),
    ('migratio', 'Migrating the runners'),
    ('limites', 'Known limits and named futures'),
]
VERSIONES = ['1a', '1b', '2', 'post-2', '3', '5', '6', '7', 'portae', 'circulus', 'gradus', '—']
TITULI_VERSIONUM = {'1a': 'slice 1a: the judge (2026-09-30)',
                    '1b': 'slice 1b: healing (2026-10-01)',
                    '2': 'slice 2: observed reads and the object store (2026-10-03)',
                    'post-2': 'after slice 2: the first runners on the compiler (2026-10-03)',
                    '3': 'slice 3: gates judged by what they read (2026-10-03)',
                    '5': 'slice 5: the gate rollout (2026-10-06/07)',
                    '6': 'slice 6: steps, members and areas (2026-10-07/08)',
                    '7': 'slice 7: the root suite as a composite (2026-10-09)',
                    'portae': 'portae declaratae: owed gates computed in C (2026-10-12)',
                    'circulus': 'circulus commissionis: the commit loop (2026-10-12)',
                    'gradus': 'cursores ad gradus: suites as composites (2026-10-13/14)',
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

# exempla et numeri ex declarationibus ipsis; subsystemata ex fabrica.stml
SUBSYSTEMATA = re.findall(r'<subsystema via="([^"]+)"/>', open(os.path.join(radix, 'fabrica.stml')).read())
if '.' not in SUBSYSTEMATA or 'html' not in SUBSYSTEMATA:
    sys.exit('pagina_fabricae: fabrica.stml sine subsystematibus notis (%r) - recuso' % SUBSYSTEMATA)
declarationes = {s: open(os.path.join(radix, s, 'aedificatio.stml')).read() for s in SUBSYSTEMATA}
n_actionum = sum(len(re.findall(r'^\s*<actio ', d, re.M)) for d in declarationes.values())
n_exituum = sum(len(re.findall(r'^\s*<exitus ', d, re.M)) for d in declarationes.values())
n_compositorum = sum(len(re.findall(r'^\s*<compositum ', d, re.M)) for d in declarationes.values())

# suitae ut gradus: actio iudicium cum <probationes_c exemplar>; membra = plagulae exemplaris
import glob
suitae = []
for s in SUBSYSTEMATA:
    for t, ex in re.findall(r'<actio titulus="([^"]+)" genus="iudicium">\s*<probationes_c exemplar="([^"]+)"',
                            declarationes[s]):
        suitae.append((t, ex, len(glob.glob(os.path.join(radix, ex)))))
if not any(t == 'probationes_html' for t, _, _ in suitae):
    sys.exit('pagina_fabricae: probationes_html non inventa in declarationibus - recuso')
n_membrorum = sum(n for _, _, n in suitae)
lineae_suitarum = '\n'.join(
    '<tr><td><code>%s</code></td><td><code>%s</code></td><td>%d</td></tr>' % (t, html.escape(ex), n)
    for t, ex, n in suitae)

# ubi quidque habitat: viae non sub build/ EXSISTERE debent (aliter recuso -
# tabula de locis quae locum absentem nominat mendax est)
LOCI = [
    ('Which subsystems fabrica reads', ['fabrica.stml'], 'every <code>bin/fabrica</code> command', ''),
    ('A subsystem\'s actions, composites and test suites', ['<sub>/aedificatio.stml', 'html/aedificatio.stml'],
     '<code>bin/fabrica</code>', ''),
    ('The grammar of those files', ['aedificatio.canon'], 'the declaration reader (an unknown element is refused)', ''),
    ('Include roots and compiler flags', ['aedilis.stml', 'tools/vexilla.sh'],
     '<code>bin/aedilis --radices</code>, <code>vexilla_radices</code>, <code>bin/compilator</code>', ''),
    ('Hand-written <code>-I</code> lists still owed', ['tools/radices_manu.tsv'],
     'gate <code>vexilla</code> VI (a two-way ratchet: only shrinks)', ''),
    ('Which gates a change owes', ['the <code># &lt;porta …/&gt;</code> line in each runner', 'lib/portae.c'],
     '<code>bin/fabrica portae debitae|tactus</code>, <code>silva.commissio</code>',
     'for a converted suite the <code>tegit</code> globs restate, by hand, what its members\' closures already say'),
    ('How a gate is run', ['pythonica/silva.py'],
     '<code>silva.porta</code>, <code>commissio</code>: <code>PORTAE</code> (runner + fallback signature), '
     '<code>PORTAE_GRADUUM</code> (gate &rarr; composite)',
     'the composite\'s name is repeated in <code>&lt;sub&gt;/aedificatio.stml</code>, the subsystem in <code>fabrica.stml</code>'),
    ('A suite\'s human entry point', ['<sub>/compile_probationes.sh', 'html/compile_probationes.sh'],
     'people, <code>tools/frigida_probare.sh</code>', ''),
    ('Verdicts, read traces, particles, runs', ['build/fabrica.db'],
     '<code>bin/fabrica iudicare|probare|causae</code>', ''),
    ('A member\'s area: binary, <code>credo.tsv</code>, <code>verdictum.txt</code>, link log',
     ['build/fabrica/area/<composite>/<member>/'], '<code>bin/fabrica</code>, <code>silva._binarium_probationis</code>', ''),
    ('An action\'s last log', ['build/fabrica/acta/<titulus>.log'], 'signature readers, people', ''),
    ('A gate\'s verdict text', ['build/fabrica/verdicta/<porta>.txt'], 'judged and healed as an artifact: <code>bin/fabrica sanare -machina &lt;path&gt;</code>', ''),
    ('Compiled objects (the store)', ['build/aedilis/obiecta', 'tools/cursor_communis.sh'],
     '<code>bin/compilator</code>, <code>cursor_instrumentum_struere</code>', ''),
    ('Per-section test results', ['credo.tsv in the member\'s area (<code>CREDO_VERDICTA</code>)'],
     '<code>-machina</code> <code>SECTIO</code> records', ''),
    ('Timing and history of every run', ['the mensura volume'], '<code>bin/mensor quaestio</code>', ''),
    ('How to convert a runner', ['ledger batch job …W0ZBW (rubric v3)', 'tools/gradus_oraculum.sh'],
     'whoever converts a suite: the checklist names nine things the oracle cannot see', ''),
]


def locus(v):
    if ' ' in v:  # prosa, iam HTML
        return v
    if not v.startswith(('build/', '<')) and not os.path.exists(os.path.join(radix, v)):
        sys.exit('pagina_fabricae: locus %s non exsistit - recuso' % v)
    return '<code>%s</code>' % html.escape(v)


lineae_locorum = '\n'.join(
    '<tr><td>%s</td><td>%s</td><td>%s</td><td>%s</td></tr>' % (
        res, '<br>'.join(locus(v) for v in viae), lector, ('<strong>also:</strong> ' + bis) if bis else '')
    for res, viae, lector, bis in LOCI)
n_bis = sum(1 for l in LOCI if l[3])

VERBA = [
    ('actio', 'action', 'a command with declared inputs (<code>ingressus</code>), outputs (<code>exitus</code>) and a footprint (<code>vestigium</code>)'),
    ('artificium', 'artifact', 'anything fabrica judges: a file, an installed binary, a verdict'),
    ('compositum', 'composite', 'a name for several artifacts judged as one (the worst verdict wins)'),
    ('iudicium', 'judgment action', 'an action whose output is a verdict, not a file: every gate and every test'),
    ('gradus', 'step', 'one declaration that expands into members: <code>&lt;probationes_c&gt;</code> makes one per test file'),
    ('membrum', 'member', 'one test: its own action, its own area, its own key'),
    ('area', 'area', 'a member\'s private directory under <code>build/fabrica/area/</code>; writing outside it is refused'),
    ('porta', 'gate', 'a check a commit may owe (<em>debita</em> when a change owes it)'),
    ('verdictum', 'verdict', 'what the judge says of an artifact (RECENS, STALUM, IGNOTUM); a check\'s own result is a pass or FRACTUM'),
    ('lectiones', 'observed reads', 'what a run actually read; <code>lectiones="verum"</code> keys an action by them'),
    ('particulae', 'particles', 'the seals of a pass\'s declared inputs, kept so the judge can name what changed'),
    ('servata', 'kept', 'a fresh member whose pass stands: not re-run, and said so'),
    ('debitum_scripturae', 'write debt', 'a named permission for one member to write outside its area (golden rewrites)'),
    ('ambitus variabilis', 'environment variable', 'one a member may read; its value is part of the key'),
    ('praecondicio', 'precondition', 'an action that must be current first'),
    ('iudicare · sanare · probare', 'judge · heal · test', 'say what is stale · make it current · run a test composite'),
    ('thesaurus', 'store', 'the content-addressed object store under <code>build/aedilis/obiecta</code>'),
    ('cursor · ianua', 'runner · entry point', 'the script a person runs; a converted runner is a thin entry point'),
    ('expeditio', 'batch job', 'a ledger job over an inventory, with a versioned rubric (…W0ZBW converts runners)'),
]
lineae_verborum = '\n'.join('<tr><td><code>%s</code></td><td>%s</td><td>%s</td></tr>' % v for v in VERBA)


def excerpere(textus_decl, initium, finis):
    a = textus_decl.index(initium)
    a = textus_decl.rindex('\n', 0, a) + 1
    b = textus_decl.index(finis, a) + len(finis)
    return textus_decl[a:b]


ex_manus = excerpere(declarationes['.'], '<actio titulus="manus"', '</actio>')
ex_briar = excerpere(declarationes['.'], '<compositum titulus="briar"', '</compositum>')
ex_lectiones = excerpere(declarationes['tessera'], '<actio titulus="fragmentum_tessera_folium"', '</actio>')
ex_html = excerpere(declarationes['html'], '<actio titulus="probationes_html"', '</actio>')


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

<p>Since slice 3 it also judges <em>checks</em>: a gate or a test is an
action whose output is a verdict, keyed by what it read, so an unchanged
set of reads means the pass stands across edits and commits. Since slice
6 a test suite is a declaration, not a script: each test file is a
member with its own key, and only the stale ones run.</p>

<h2>Where each fact lives</h2>
<p>The question a newcomer asks first. One row per fact; a row with an <strong>also</strong> note is a
fact with two homes, where the copies can drift (today %(n_bis)d). Paths
outside <code>build/</code> are checked when the page is generated: a
row naming a file that is gone refuses to generate.</p>
<table>
<thead><tr><th>Fact</th><th>Home</th><th>Read or written by</th><th></th></tr></thead>
<tbody>
%(loci)s
</tbody>
</table>

<h2>Words</h2>
<p>The vocabulary is Latin and dense; each word is exact, and most of
them name one element of a declaration or one verb of the tool.</p>
<table>
<thead><tr><th>Word</th><th>English</th><th>Means</th></tr></thead>
<tbody>
%(verba)s
</tbody>
</table>

<h2>Declarations</h2>
<p>The root <code>fabrica.stml</code> names %(n_subsys)d subsystems; each declares
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

<h2>Tests and gates on fabrica</h2>
<p>A converted test suite is one declaration. html's, in full:</p>
<pre><code>%(suita_html)s</code></pre>
<p><code>&lt;probationes_c&gt;</code> is a <em>step</em>: it expands into
one <em>member</em> per matching file, each a synthetic
<code>iudicium</code> action keyed by its source closure (from aedilis),
the house binaries it runs, what it read on its last pass, and the
values of its declared environment variables. Anything else in the
environment is not passed in; a write outside the member's area is
refused unless a <code>&lt;debitum_scripturae&gt;</code> names it (here:
the two golden files the rewrite modes regenerate).</p>
<p><code>./html/compile_probationes.sh</code> is a thin entry point (log,
lock) to <code>bin/fabrica probare probationes_html</code>. Stale
members compile through <code>bin/compilator</code>, link and run in
their areas, printing their output; fresh ones are kept and said so
(<code>servatae K/M</code>, with the judge's reason). Inside a member,
credo runs its sections and their results come back as
<code>SECTIO</code> records. <code>-vis</code> runs everything; a word
after the composite runs only the members whose name contains it. The
gate is the same call with <code>-machina</code>: pythonica reads the
<code>PROBARE</code> record (a pass only when every member passes), not
the printed text.</p>
<p>Suites declared this way today (%(n_suitarum)d, %(n_membrorum)d members):</p>
<table>
<thead><tr><th>Composite</th><th>Members from</th><th>Members</th></tr></thead>
<tbody>
%(suitae)s
</tbody>
</table>
<p class="nota">The other suites (briar, crusta, gesta, ludus_tessera,
officina, oratio, tessera) still run their own scripts; the ones that
build the tools their tests run wait on a way to declare such a tool
(<code>classis-b</code> below). Converting a suite follows the rubric of
batch job …W0ZBW, and <code>tools/gradus_oraculum.sh &lt;sub&gt;</code>
must agree (the old runner at a commit against <code>probare
-vis</code>).</p>

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
<tr><td><code>bin/fabrica iudicare [x…]</code></td><td>the quick judge: which artifacts are stale or unknown, and why</td></tr>
<tr><td><code>bin/fabrica probare COMPOSITUM [filtrum] [-vis]</code></td><td>run a test composite: stale members run, fresh ones are kept; <code>-vis</code> runs all</td></tr>
<tr><td><code>bin/fabrica portae debitae|tactus -portae TSV [viae]</code></td><td>which gates these paths owe, and every reason why</td></tr>
<tr><td><code>bin/fabrica causae [titulus]</code></td><td>runs counted per reason: why a verdict keeps re-running</td></tr>
<tr><td><code>bin/fabrica census</code></td><td>input and step kinds, and the write debts with whether each is in use</td></tr>
<tr><td><code>… -machina</code></td><td>on iudicare, sanare, probare: TSV records (first field = type), the only form programs read</td></tr>
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
and the batch job …W0ZBW; slices 3-7, portae declaratae, circulus
commissionis and cursores ad gradus added 2026-10-14 from their specs'
"As built" sections) and from the declarations themselves;
regenerate with <code>./docs/instrumenta/regenerare.sh</code>. The
authority is the specs' "As built" sections and the code: when they
disagree with a row here, the row is wrong.</p>

</body>
</html>
''' % {'nota': NOTA, 'n': len(ordines), 'actiones': n_actionum, 'exitus': n_exituum,
       'composita': n_compositorum, 'manus': codex(ex_manus), 'lectiones': codex(ex_lectiones),
       'briar': codex(ex_briar), 'status': status_html, 'index_arearum': index_arearum,
       'tabulae': tabulae, 'n_subsys': len(SUBSYSTEMATA), 'n_bis': n_bis, 'loci': lineae_locorum,
       'verba': lineae_verborum, 'suita_html': codex(ex_html), 'suitae': lineae_suitarum,
       'n_suitarum': len(suitae), 'n_membrorum': n_membrorum})
print(facultates.compendium('fabrica', ordines, numeri) + '; declarationes: %d actiones, %d exitus, %d composita'
      % (n_actionum, n_exituum, n_compositorum))

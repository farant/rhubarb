# fabrica spec 6 - the tool: chassis, step kinds, the toml pilot

Interview: `project-specs/fabrica-6-interview.md` (Q1-Q15 DECIDED, every
recommendation (a), Fran 2026-10-07). Vision:
`project-specs/fabrica-visio.md` (RESERVATUM; ledger …F9ESW3). Builds on
slice 5 (`fabrica-spec-5.md` §XI) and the pythonica speed work.

## 0. Data (2026-10-07)

- `FabricaGenus` (`include/fabrica.h:415`): `titulus`, `sigillare`,
  `enumerare`, `locare`, determinism flag; 12 input kinds as static
  tables in `lib/fabrica.c` (6,931 lines; tools/fabrica.c 3,442). No
  per-kind conformance fixture exists.
- aedilis has a library core: `lib/aedilis.c`, `aedilis_derivare`
  (closure of a scope, `include/aedilis.h:166`). The store is a library:
  `lib/thesaurus.c`. compilator does NOT: its key and store logic lives
  in `tools/compilator.c` (776 lines: `_clavem_capitis`,
  `_obumbrationem_addere`, `_clavem_plenam`, `_compilare`, ...).
- toml: runner `toml/compile_probationes.sh` (145 lines, 13 tests, 0
  unresolved effectus sites); verdict `porta_toml`, reuse 68% -> 78%
  over 150 commits (spec 5 T4). It reads `build/toml_corpus.lst` and
  `toml/build/aurum_silvestre.txt` (two generator actions).

## I. Framing

A step knows its reads, writes and naming by construction; a gate is a
composite of step verdicts over a shared, content-keyed pool. This
slice builds the chassis that makes every extension prove those
properties, the first step kind that real suites need
(`probationes_c`), and proves both on toml against today's runner,
before any other runner moves.

## II. The chassis

1. **Registries**, one per extension point, compiled in (C vtables,
   no dynamic loading): input kinds (today's `FabricaGenus`), step
   kinds (new `FabricaGradus`), evidence strategies (today's
   provenientia values), queries, commands. Slice 6 builds the
   registries for input and step kinds; the other three are named and
   registered as they exist (no behaviour change).
2. **Contract** (vision §IV): named particles; declared reads (static,
   or DYNAMIC = traced); declared writes; declared determinism; the
   ability to refuse with a reason; a cost class; and a **conformance
   fixture** (mandatory).
3. **Conformance fixture**: per entry, a small declared world in a
   temporary root where (a) a change to the entry's input class flips
   the judgment and names the particle, (b) an unrelated change leaves
   RECENS, (c) writes stay inside declared places. One chassis test
   (`probatio_fabrica_chassis`) runs every registered entry's fixture;
   an entry without one fails registration (the test enumerates the
   registry, not a hand list).
4. **Census**: `bin/fabrica census` lists entries by properties met
   (static/dynamic reads, deterministic/volatile, particles/coarse,
   cost class). An addition that cannot meet a property is classified,
   not rejected - the census is the design signal (Fran).

## III. Inputs on two axes

Declaration gains `res` (what: plagula, plagulae, directorium,
instrumentum, ambitus, repositorium) and `clavis` (how keyed: contentum,
exstantia, nomina, clausura, provenientia, identitas); file sets name
their `fons` (arbor | repositorium). Today's `genus` values remain as
ALIASES mapping to a pair (fasciculus = plagula/contentum;
instrumentum_domus = instrumentum/provenientia; identitas_clang =
instrumentum/identitas; plagulae = plagulae/contentum; ...) - no
existing declaration changes. New: `res="repositorium"` with `clavis`
commissum (HEAD), index, or a tracked set (`fons="repositorium"`).

## IV. Step kinds - `FabricaGradus` and `probationes_c`

1. `FabricaGradus` vtable: `titulus`; `membra` (expand a declared set
   to members, stable ids `<action>/<member>`); `lectiones` (static
   reads, or DYNAMIC); `scripturae` (its area only); `agere` (do the
   step in its area with the base environment); `iudicare` (verdict
   particle); properties for the census; the conformance fixture.
2. `probationes_c exemplar="..." [praeter="..."]`: per member - LINK
   (closure by `aedilis_derivare`, objects through the compilator
   library and store, flags from `aedilis.stml` only) then RUN (exit
   code; area = cwd scratch; base env). Link is STATIC (its key = the
   closure + flags + toolchain identity); run is DYNAMIC (keyed by its
   read-ledger trace, per member). A member annotated `facultas`
   (fenestra | rete | repositorium) is linked and judged but not run
   headless.
3. No templating. Naming by convention: the binary of member
   `toml/probationes/probatio_x.c` lives in its area as `probatio_x`.
4. `mandatum` remains the escape hatch, unchanged.

## V. Gates: composites, ordering, sharing

- A gate is a `compositum` (existing primitive) of step verdicts:
  RECENS iff all parts RECENS; otherwise the non-RECENS parts are named.
  The verdict text is derived from the parts (`<porta>: N/N`), no
  signal grep.
- `<post actio="..."/>` on an action = produce that action first
  (ordering only); a dynamic step's key comes from its trace.
- Steps are keyed by content, so two gates declaring the same member
  share its result (radix and the aedilis gate later).

## VI. Places and environment

Each step owns `build/fabrica/area/<action>/<member>/` (created and
emptied by fabrica; reads inside = products, spec 5 rule). Base
environment provided by the tool: `PATH` (fixed base), `HOME`,
`RHUBARB_RADIX` = root of the tree being built; nothing else inherited
unless declared (`<ambitus>`). A write outside declared places or a
read of an undeclared variable = refusal naming it.

## VII. Libraries

compilator's key and store logic moves to `lib/compilator.c` +
`include/compilator.h` (API approved by Fran before code);
`tools/compilator.c` becomes a thin CLI over it; `compilator-fumus`
stays green unchanged (the oracle for the extraction). fabrica links
`lib/aedilis.c` and `lib/compilator.c`; effectus stays a process.

## VIII. Bootstrap

The judges (`fabrica`, `compilator`, `aedilis`) form a stage judged
first; if any is stale, every other judgment refuses with "iudex X non
recens - sana X prius" instead of judging with a stale judge.

## IX. The pilot (toml)

1. `probationes_toml` in toml/aedificatio.stml: `post` toml_corpus and
   toml_aurum_silvestre; `probationes_c exemplar="toml/probationes/probatio_*.c"`.
   `porta_toml` (the runner verdict) stays.
2. ORACLE: both on one tree must agree per test (pass/fail per member,
   counts); plants: a broken test -> both red naming it; a broken
   library source -> both red.
3. A/B: per-step reuse vs runner-verdict reuse over the same 150
   commits (`tools/reusus_retro.sh` extended to step traces).
4. Switch: PORTAE['toml'] judged through the composite; the runner
   script deleted only after the switch has stood (as generata, T7c).

## X. Policies (Q15)

One flag source (`aedilis.stml`; `tools/vexilla.sh` derived from it or
retired when its last consumer goes); passes cached, failures re-run
with the cause recorded; stable step ids; deleted members reported as
orphans; base environment from the tool.

## XI. Not in this slice (recorded direction)

Volumes and the filum backend (next slice, vision §V staged order);
snapshot builds as default; PORTAE and inventory lenses as derived
views; other step kinds (`probationes_py`, `generare_et_conferre`,
`registra_cocta`, `capsulae`, `regio_generata`, `differentia`,
whole-set checks, `installare`); the source-annotation vocabulary
beyond `facultas` (`instrumentum`, `post`, `ambitus` in sources);
closures replacing hand lists in other runners (with each runner's
migration).

## XII. Done means

Registries for input and step kinds with a conformance fixture for
every entry (12 input kinds + `probationes_c`) and `bin/fabrica census`;
two-axis declarations with aliases and `repositorium`; bootstrap stage;
`lib/compilator.c`; `probationes_c` with areas and base env; toml
expressed as steps, agreeing with its runner per test, A/B measured,
switched; plants for every rule.

## AUDIENDA

- Whether `lib/aedilis.c` links into fabrica without the tools/aedilis.c
  CLI layer (believed yes: aedilis_derivare is the library entry).
- The size of the compilator extraction (776 lines; `principale` and
  argv parsing stay in tools/).
- Per-step reuse is unknown until the A/B; the switch waits on it.
- The step dialect canon (attributes, kinds) is drafted in this slice;
  names may change at review.

## XIII. As built

**T1 (2026-10-07): chassis for input kinds.** The registry was already
there (`_genera[]`, 12 static tables) but private and unchecked.
Public now: `fabrica_genera_numerus`, `fabrica_genus_obtinere`,
`fabrica_genus_loci`; census fields on `FabricaGenus`:
`particulae_nominatae`, `sumptus` (`FabricaSumptus`: vilis / medius /
carus). Conformance fixtures live in probatio_fabrica.c (its fake world
- not a separate probatio_fabrica_chassis file, which would have to
duplicate it): a table keyed by title; the chassis section enumerates
the REGISTRY, so a kind without a fixture is red by name. Each fixture:
a small world; a change of the kind's input class must change the
named particle; an unrelated change must leave the particles equal.
All 12 pass. Plants: `plagulae` sealed by the names-only sealer ->
"particulam src/a.c non mutat"; a fixture renamed away -> "genus
'manifesta' sine fixo conformitatis".
Fake world fix on the way: `_directorium_ponere` APPENDED a second
record for a directory already listed, so a re-listing was invisible -
it now replaces, as a real directory would.

`bin/fabrica census` (TSV per kind: particles, reproducible, cost,
places, locates; fumus case XXXIII). FIRST FINDING of the census, by
design: five kinds declare NO places (`loci:nulli` -
`_nihil_enumerare`): instrumentum, radices, identitas_clang,
instrumentum_domus, effectus. They are invisible to reverse-dependency
queries (`iudicare -tacta`, debts): a change to a file an effectus
script reads, or to an include root's names, cannot be traced back to
what depends on it that way. For external tools and toolchain identity
that is right; for effectus, radices and instrumentum_domus it is a
gap to close (their places are knowable: the effectus key's octeti and
nomina lines, the roots' directories, the binary's closure).

**T2 (2026-10-07): inputs on two axes + `repositorium`.** An ingressus
is declared either by `genus` (alias, unchanged) or by `res` + `clavis`;
a pair table maps ten pairs to kinds (`fabrica_genus_ex_pari`, inverse
`fabrica_genus_par`). Both at once -> refusal "genus et res/clavis
simul"; a pair with no kind -> refusal naming the pair (the design
signal). configuratio, binarium and radices have no pair (census:
`par:nullum`). canon: `genus` optional, `res`/`clavis` choices.
New kind `repositorium` (pair repositorium/commissum): seals HEAD's
sha as particle `repositorium:commissum` through a new sutura hook,
implemented in tools/fabrica.c with lib/git (`git_aperire`,
`git_ref_resolvere`) - a library, not a `git` process. Places: none,
deliberately (a commit is not a path; census `loci:nulli`, now 6).
Deferred (no consumer in the pilot): `clavis="index"` and
`fons="repositorium"` file sets - lib/git reads refs, objects and
trees but not the index.
Tests: equivalence of every pair with its alias; both-forms and
unknown-pair refusals; the `repositorium` conformance fixture (13
kinds); fumus XXXIV in a real temporary git repository: a remembered
generator with the repository input is RECENS by memory, and after an
empty new commit the memory no longer matches. Plants: a pair mapped
to the wrong kind -> equivalence red; HEAD ignored in the seal ->
the fixture red by name.

**T3 (2026-10-07): the judges' stage.** Actions may declare
`iudex="verum"` (canon attribute; `FabricaActio.iudex`); the root marks
fabrica, compilator and aedilis. `iudicare`: the marked actions are
judged FIRST; if one is not RECENS its own sentence is printed first
(counted, named under SANATIO) and every other selected artifact gets
`NON IUDICATUM <via> - iudex <X> non recens - sana <X> prius` without
being judged; exit 1. `sanare`: stale judges are healed in a first pass
(`stadium iudicum primum`), the rest after; bin/fabrica itself keeps
its existing refusal (it does not rebuild itself). The data-driven
marker replaces the hard-coded "bin/fabrica first" special case on the
judging side.
commissio: `_fabricam_exigere` refuses when the stage refused. Without
it a stale (untracked) bin/compilator would have made every touched
artifact NON IUDICATUM, and the rule "untracked stale artifacts do not
block" would have let the commit through with NOTHING judged - the
stage now enforces "heal installed binaries before committing" instead
of memory doing it.
Tests: fumus XXXV (stale judge -> others refused, exit 1) and XXXVI
(sanare heals the judge first, even when not selected); pythonica
(refusal lines block the commit). Plants: the stage off in iudicare ->
XXXV red; off in sanare -> XXXVI red; the commissio check off ->
pythonica red.
Found while planting: a plant restored with `cp -p` keeps the backup's
OLD mtime; fabrica_struere's mtime-judged struere.sh then keeps the
object compiled from the plant, and the provenance relation (a digest
of SOURCES) would still call that binary RECENS. Restore with plain cp
or touch (MEMORY). One more argument for content-addressed caching only
(vision §II.6).

**T4 (2026-10-07): compilator as a library.** `include/compilator.h`
(API approved by Fran): `compilator_aperire(piscina, radix_thesauri,
clang)`, `compilator_cacheabile(argv, n)`, `compilator_clavem(c, argv,
n, &clavis)` (the head key WITHOUT compiling - for T6's link step) and
`compilator_compilare(c, argv, n)` returning `CompilatorResultus
{codex, ex_thesauro, effusio, erratum}`. Fran chose clang's output as
STRINGS over stdout/stderr passthrough: a step can keep a failing
compile's diagnostics as its verdict cause; the CLI prints them.
`codex -1` = not cacheable or clang identity unknown. `lib/compilator.c`
(837 lines) is the old tool's logic unchanged (head key, full key with
per-name shadowing, depfile index, temp + rename into the store); the
only behaviour change is collecting output instead of writing it.
`tools/compilator.c` is now a 98-line CLI: provenance answer,
`FABRICA_THESAURUS`, exec clang itself when the call is not cacheable
or `codex < 0`, else print and return `codex`. Lint: `cacheabile`
admitted as a form of the permitted `cache` (glossary).
Oracle: `compilator_fumus.sh` stayed green - and so did the PLANT
(the source's bytes dropped from the head key): no case changed a
source without changing a header, and the source sits only in the head
key. New case XI (source edited -> miss, new object); fumus is XI/XI,
the plant is red on XI. Byte comparison over the 58 commands of the
toml closure (fresh store miss, then hit), new vs old binary: identical
except `toml_scalaris.o`, which also differs between two runs of the
OLD binary.
Finding (determinism, vision §II.10): Apple clang 16.0.0
(clang-1600.0.26.4) at `-O2` emits `toml_scalaris.o` with different
lane/register choices in one vectorized digit loop (`mul.4s` by 10,
`uzp1`/`uzp2` swapped) - semantically equal, bytes not. It depends on
process layout: stable 6/6 in one shell, a different object under
`env -i` and DIFFERENT AGAIN on each `env -i` run, flips with
environment size. A hermetic environment does NOT make it
deterministic. Consequence for keys: an object is never an input by
its bytes when a cheaper key exists (we key on sources - correct); a
determinism check over compiled objects would flag this file, so the
check must compare twice-built outputs per kind, and clang objects
need the rule "equal sources -> equal by declaration", not by bytes.

**T5 (2026-10-07): step kinds - `FabricaGradus`, areas, base env.**
A step's MEMBERS become synthetic `iudicium` actions:
`fabrica_gradus_explicare` keeps every action and appends, after each
action carrying a `gradus`, one action per member - title
`<action>/<member>`, `lectiones`, exitus `build/fabrica/area/<action>/
<member>/verdictum.txt` (strategy `verdictum`), vestigium = the area
(ARBOR), static inputs from the kind's `ingressus`, mandatum = (kind,
member source) in the memory key. The existing judge then does the
rest: key = static inputs + read trace, a trace is stored only after a
pass (passes cached, failures re-run), reads inside the area are
products. `_actionem_agere` branches to `_membrum_agere` for members:
old verdict deleted, `sutura->area_parare` (created and EMPTIED),
`fabrica_ambitum_basis` (PATH fixed to the system dirs, HOME,
TMPDIR=<area>tmp, RHUBARB_RADIX, then declared names - fabrica's own
environment never passes through), snapshot, kind `agere`
(`sutura->in_area_currere`, exact env), then refusals: write outside
the area (`scripsit extra vestigium: X`), environment read of a name
neither base nor declared (`ambitus non declaratus: X`, from the E lines
of the member's ledger); a pass -> fabrica writes `<id>: transiit`.
`fabrica_areas_orphanas` reports areas of vanished actions or members
(never deletes). Registry `fabrica_graduum_numerus/_obtinere/_invenire`
is EMPTY until T6; the chassis loop over it is in place.
Differences from the approved draft: `FabricaMembrum.titulus` (the
draft said `nomen` - a latina.h macro, examen refused it); sutura
`area_parare` added (the draft folded emptying into the contract);
`in_area_currere` also takes `acta_via`; `fabrica_acta_via` public (a
kind names the log the machine snapshots).
Tests (probatio_fabrica, GRADUS I-X + CHASSIS GRADUUM) with a toy kind
that lives in the test, not the registry: ids and order, base env
(fabrica's OMNIA=1 never leaks, declared CREDO_FILTRUM does), both
members pass in their areas, RECENS without a run, a static-input edit
re-runs one member, a dynamic read edit is named and re-runs, a failure
leaves no verdict and runs again, write outside and undeclared env
refused by name, orphans (`probationes_t/c/`, `vetus_actio/`),
duplicate member refused with its sedes. Plants: outside-write check
off, env refusal off, failure stored as pass, orphan check blind - each
red at its assertion. (A fifth plant first ran a STALE binary: `VERUM
|| a && b` failed -Werror and the old object ran - checked by deleting
the binary before the rerun.)
Found: `fabrica_suturam_parare` zeroed members one by one and the list
already lacked `particulas_*`, `verdictum_ponere`, `repositorium`; the
new test section shifted the stack and a garbage hook was called (Bus
error). It now memsets the whole struct first.
For T6: the real sutura hooks - `processus` has no cwd / exact-env
path, so `in_area_currere` needs a small `processus` API addition (to
Fran first); bin/fabrica calling `fabrica_gradus_explicare` and
printing orphan areas; the step element in aedificatio.stml + canon.

**T6a (2026-10-07): `processus_exsequi_cum`.** The real
`in_area_currere` needs a child with its own cwd and an EXACT
environment; `processus` (fork + execvp) inherited both. Added (API
approved by Fran): `ProcessusOptiones {directorium, ambitus}` and
`processus_exsequi_cum` - in the child after fork: `chdir` (failure
goes through the exec-error pipe, so it is `PROCESSUS_ERROR_EXEC` with
errno: nothing ran), then `environ` = the given vector, then `execvp`
(the PATH search uses the GIVEN environment). The parent never changes;
`processus_incipere` is now `_incipere_cum(.., NIHIL, ..)` - one path.
Tests probatio_processus XXII-XXVI (cwd honoured, `/usr/bin/env`
prints exactly the vector and the parent's environment is untouched,
PATH search through the given PATH and its failure, missing directory
= named EXEC error with no output, NIHIL options = processus_exsequi).
Plants: chdir skipped, environ not swapped, chdir failure swallowed -
each red. Glossary: `environ` (POSIX name).

**T6b (2026-10-07): the aedilis extractor as a library + `facultas`.**
`aedilis_derivare` takes its extractor through a seam; the real one
(silva for .c/.h, `clang -MM` for .m, per-run memo, content-store
records across runs) lived in tools/aedilis.c. Moved VERBATIM into
`lib/aedilis_silva.c` + `include/aedilis_silva.h` (API approved by
Fran): `aedilis_silva_creare(piscina, configuratio, radix_thesauri,
praefixum, memoria_oraculi)` and `aedilis_silva_extrahere` (an
`AedilisExtractor`); the two state structs merged into `AedilisSilva`.
One addition beyond the approved header: `aedilis_silva_oraculum` (the
`-MM` oracle on any source - `bin/aedilis --differentia` needs it).
tools/aedilis.c keeps only the CLI (1903 -> ~1100 lines); its source
list regenerated (`aedilis_silva`). Oracle: old vs new bin/aedilis
byte-identical on `--partes` for all 307 test closures (root, toml,
silva), `--corpus silva/fontes`, `--nexus-purus`, `--thesaurus` cold
and warm (8 runs), `--differentia`.
Asked about conflatio (Fran): it consumes a closure (its driver calls
`aedilis --partes`), so it does not replace the extractor; it could
replace per-object compile + link with ONE conflated translation unit
per member (key = sha of the text) but refuses vendor and .m sources
and recompiles the whole closure on any change - a possible later
variant of `probationes_c`, not T6.
`facultas` (Fran's choice of spelling): `<aedilis facultas="X"/>`,
X in fenestra | rete | repositorium, recorded in
`AedilisFructus.facultates` for the SCOPE only; in any other file of
the closure or an unknown value -> named refusal (a header cannot
declare it for all includers: many tests include fenestra.h and draw
headless). Tests probatio_aedilis 'facultatem' (fixtures fenestralis,
facultas_ignota, includens_facultatem + facultas_caput.h); plants:
scope check off, value check off, value not recorded - each red (the
third first crashed reading element 0 unguarded; guarded).
Decided for T6c (Fran): a test runs with cwd = tree root (tests are
root-relative), its binary, objects and TMPDIR in the member's area;
objects compiled into the area through the compilator store.

**T6c-1 (2026-10-07): `probationes_c` in the machine.** API approved by
Fran: `FabricaClausuraC {fontes, capita, vexilla_nexus, facultates}`
and two sutura hooks, `clausura_c` (closure of a test) and `compilare`
(source -> object through the store); the kind stays PURE in
lib/fabrica.c, the tool implements the hooks (T6c-2). Registered: the
step registry now holds `probationes_c`.
- membra: `exemplar="dir/forma"` - the .c files of dir matching the
  glob (fabrica's `_globus_congruit`), `praeter` excludes by name;
  member title = name without `.c`.
- ingressus (static key): closure sources + headers, `aedilis.stml`
  (the one flag source), `identitas_clang`. A closure that cannot be
  derived does NOT refuse the declarations: the member keys on its
  source alone and fails in agere with aedilis's cause (failures
  re-run).
- agere: compile each closure source into `<area>obiecta/<path__>.o`,
  link `<area><member>` (link flags split into argv words), and unless
  the scope declares a `facultas`, run it (cwd = tree root); the run's
  trace is the dynamic key. Facultas -> linked only, verdict
  `<id>: transiit (nexus solum: facultas fenestra)` (a kind may give a
  deterministic verdict note via `cauda` on a pass).
- Declarations: an action child named after a registered kind
  (`<probationes_c exemplar= praeter=/>`; attributes copied generically)
  plus `<ambitus variabilis="X"/>`; refused: a step outside `iudicium`,
  two steps; a step action needs no ingressus/exitus/lectiones. Canon:
  `probationes_c`, `ambitus`; ingressus/exitus lose `minimum="1"` (the
  reader keeps the rule for actions without a step).
- `in_area_currere` contract: cwd = tree root (Fran, T6b), the ledger
  created EMPTY before the run.
Found and fixed: an EMPTY trace could never be stored - an empty write
is a deletion in the trace store (spec 3 T5b), so a member that reads
nothing (link-only facultas member, a test without reads) re-ran every
time. A pass with an empty trace now stores one sentinel read (`X .`,
the root exists).
Tests (probatio_fabrica PROBATIONES_C I-X, fake closure table, fake
compiler, fake runner counting links and runs): declaration and its two
refusals; members a b c (zeta excluded, notes not matched); first heal
3 links / 2 runs / 5 compiles, framework flag split; nothing changed ->
nothing; library source of a -> a only; header of b -> b only; test c
(facultas) -> relinked, not run; run-time read -> a and b, not c;
failing run -> FRACTUM, runs again; compile error -> FRACTUM naming the
source and clang's message. Plants (5, all red): closure ignored,
facultas ignored, empty-trace sentinel off, step-outside-iudicium
refusal off, praeter ignored.

**T6c-2 (2026-10-07): the real hooks in bin/fabrica.** tools/fabrica.c
implements the four step hooks: `area_parare` (area deleted, `<area>tmp/`
created), `in_area_currere` (`processus_exsequi_cum` with the EXACT
vector `ambitus + FABRICA_LECTIONES=<absolute ledger>`, cwd = tree
root, ledger created empty, acta like every other action),
`clausura_c` (`aedilis_derivare` + `aedilis_silva` sharing the store
build/aedilis/obiecta; key prefix = bin/fabrica's own bytes + aedilis.stml;
the test file FIRST - aedilis's `obiecta` does not list the scope,
its struere.sh compiles it separately) and `compilare` (argv exactly as
aedilis's struere.sh: vendor rule flags alone, else house flags + `-I`
roots + per-source rule; through `compilator_compilare` - argv WITHOUT
`clang`, which the compilator would count as a second source). Set up
lazily (temporary fumus roots have an empty aedilis.stml). Declarations
are expanded while being read (`_declarationes_colligere`, the
enumerating sutura has `clausura_c`); `iudicare` prints step-area
orphans; bin/fabrica now links the silva amalgam (1.9 -> 3.3 MB).
Smoke (tools/fabrica_fumus.sh, now XXXVIII/XXXVIII), a real temporary
tree: XXXVII - three members (one `facultas="fenestra"`), first heal
passes all three (the facultas verdict says linked only), all RECENS
after, a library edit re-runs ONLY the member whose closure holds it;
member b exits 5 if it sees `FUMUS_ALIENUM` from fabrica's own
environment (it does not). XXXVIII - a failing test is FRACTUM with
its exit code; a vanished member's area is ORPHANUM. Plants (3, red):
environment inherited (b sees the variable), scope not first (b links
nothing), closure hook missing while reading declarations (the library
edit re-runs nothing).

**T7 (2026-10-07): gates as composites + `post`.** API approved by Fran.
- `FabricaActio.post` (`<post actio="X"/>`, canon element `post`):
  ORDER only - checked in `_pendet` beside `praecondicio`, so ordering,
  the heal's scope (healing a member first heals a stale producer) and
  "dependency broken -> OMISSUM" all follow; never a key. Members
  inherit their parent's `post`. A member whose run READS the product
  is keyed on it through its own trace (the product has a declared
  owner); a member that does not read it is untouched. `post` naming no
  action is refused with `fabrica_praecondiciones_probare`.
- `fabrica_gradus_composita`: every step action becomes a composite of
  the same title whose parts are its members; bin/fabrica adds them to
  the declared composites (a title clash is refused), so `iudicare X`
  and `sanare X` work by the step action's title.
- `fabrica_compositum_verdictum`: `<title>: N/M`, and if N < M
  ` - non recentia: a, b, c +K` (member verdicts named by their id).
  bin/fabrica prints it as a `VERDICTUM` line after `COMPOSITUM` - the
  line T10 judges instead of grepping a runner.
Tests (probatio_fabrica 'post et composita'): post parsed, unknown post
refused; producer ordered before members though declared after them;
healing only the member verdicts heals the producer FIRST; product
changed -> the reading member STALUM naming it, the other RECENS;
one composite with both parts; verdict `1/2 - non recentia: <id>` and
`2/2`. Fumus XXXIX (XXXIX/XXXIX) on a real tree: a generator writes
build/corpus.lst, test a fails without it; `sanare probationes_t` heals
g before a; `VERDICTUM probationes_t: 2/2`; after b's source changes
`1/2 - non recentia: probationes_t/probatio_b`. Plants (6, all red):
post ignored in `_pendet`; post as a key (producer outputs as member
inputs - the reading-only member goes stale too); verdict counting
stale parts as recent; members not inheriting post; step composites not
added (sanare by title refused); VERDICTUM line not printed.

**T8 (2026-10-08): toml as steps - the oracle phase.**
`probationes_toml` in toml/aedificatio.stml beside `porta_toml`:
`probationes_c exemplar="toml/probationes/probatio_*.c"`, `post` on
`toml_corpus` and `toml_aurum_silvestre`, and three declared variables
the env refusal found on the first run (`COMPUTUS_SCRIBERE`,
`ORACULUM_OMNIA`, `ORACULUM_EXEMPLUM` - optional switches the tests
read). Oracle tool `tools/toml_gradus_oraculum.sh [-machina]`: runner
and step version on the SAME tree, pass/fail per test, exit 0 only on
full agreement (it names every DISSONAT test).
Findings, in order:
1. A failing member stopped its siblings: the wave heal's "after a
   fracture nothing new starts" applied to step members, so one failure
   left 8 tests OMISSUM and the verdict blind. Members are independent:
   a member's FRACTUM no longer stops the wave (dependents are still
   OMISSUM through `_ante_agere`). Test: PROBATIONES_C XI (with
   `agere_simul`); plant (rule restored for members) red.
2. THE oracle finding: runner 13/13, steps 12/13 -
   `probatio_toml_totalitas` SIGSEGV at the pinned "emission 40 000
   deep survives". The step binary crashed with the full shell
   environment too: FLAGS. The house had two flag sources -
   tools/vexilla.sh (`-O2 -g`, every runner) and aedilis.stml (no
   optimization, bin/ tools and the aedilis gate); materia's recursive
   emission (park ...FAD8) overflows the -O0 stack at 40 000. Decided
   (Fran 2026-10-08): `-O2 -g` into aedilis.stml - the vexilla lists are
   now IDENTICAL (vexilla.sh derivable later). Everything aedilis-built
   rebuilt once (17 installed binaries healed; bin/fabrica in 9 s, the
   -O2 objects were already in the store from the runners).
3. After it: `oraculum toml: congruunt 13/13`, VERDICTUM 13/13, steps
   28 s of runs (cold) vs the runner's ~25 s.
Plants (both sides red, the SAME tests named, oracle agrees 13/13): a
failing assertion in probatio_toml_lector -> both `12/13`, lector named;
toml_scalaris boolean decoding inverted -> both `9/13`, api,
differentia, oraculum, scalaris named. (The oracle's own first draft
had a BSD-sed bug - no `\|` alternation in basic regex - and reported
DISSONAT absens/fracta: a real mismatch report, fixed with `sed -E`.)

**T9 (2026-10-08): A/B - step reuse vs runner reuse.** Tools:
`tools/reusus_retro.sh` gains `-fons` (a step member's static key =
its source + aedilis closure `--partes` O/C/V + aedilis.stml) and
resolves trace reads under build/ to their producer's declared inputs
(one level, as declared build/ inputs already were);
`tools/reusus_gradus_ab.sh GRADUS PORTA DIR [-n N]` replays both over the
same commits (today's traces, `git diff` per commit) with times from the
last SANATUM run in `cursus`.
Over the last 150 commits (HEAD 34762c46):

| | reused | test runs | time |
|---|---|---|---|
| runner `porta_toml` | 129/150 (86%) | 273 | ~723 s |
| steps, whole composite | 143/150 (95%) | 87 | ~188 s |
| steps, per member | 1863/1950 (95%) | | |

Test runs -68%. Where it comes from: the runner's 21 invalidating
commits = 7 shared with the steps + 14 runner-only, all RUNNER
MACHINERY - provenance of the house binaries its script calls (bin/fabrica
via `include/fabrica.h`, lib/fabrica.c, ...; bin/mensor via aedilis.stml,
processus.h), which its effectus key carries and a step member's key
does not (fabrica itself is the judges' stage). The steps were never
invalidated alone (0 step-only commits). On the 7 shared commits nearly
every member re-ran (87 runs ~ 12.4 of 13 per commit): toml's tests share
one closure core - member causes: aedilis.stml 39 (3 commits x 13, one
of them T8's own flag change), include/lectiones.h 20, piscina.h 16,
xar.h 12 - so PER-TEST granularity gains little on this suite; the gain
is the narrower key. Suites with disjoint closures (root, silva) are
where granularity itself should show.
Approximations (both sides alike): today's traces replayed over history;
clang identity omitted. TIME is overstated for the steps: the runner's
34.4 s is its whole run (closures, compile orchestration, run), a
member's time is its run only (compile and link through the store are
not counted) - the test-run count (-68%) is the fair number. The
slice-5 figure (78% over 150 commits ending at a0ccbd5f) is a different
window.

**T10 (2026-10-08): the switch.** `pythonica/silva.py`:
`PORTAE_GRADUUM = {'toml': 'probationes_toml'}` and `_porta_per_gradum`
- `porta('toml')` (no filter, live tree) runs `bin/fabrica sanare
probationes_toml` (stale producers through `post`, stale members) and
reads `VERDICTUM probationes_toml: N/M`; sane ONLY if N == M; compendium
`probationes_toml: 13/13 [membra cursa K/13, cetera reusa]`; failing
members become fracturae with their cause. Fallbacks: sanare exit 2 or
no VERDICTUM -> the runner's verdict path, then the raw runner; a filter
or a snapshot clone -> the raw runner. bin/fabrica `sanare` now prints
the VERDICTUM line for every composite named on its command line, from
the heal itself (a part is recent unless its action broke, was omitted,
went discordant or - dry run - is still to do), so the gate needs ONE
call. Trace reads (`L`) use the per-run seal memo.
Measured on the live tree: full reuse ~3.5 s (0/13 members run); the
first call after the switch re-ran the 7 members the T8 plants had left
without verdicts. Tests: pythonica 'porta gradus' (6 cases: pass, full
reuse, failing member named, exit 2 -> runner, VERDICTUM missing ->
iudicare, filter -> runner; the real mapping declared); fumus XXXIX
checks sanare's own VERDICTUM. Plants: sanity ignoring N == M -> red;
porta() not consulting the step path -> red.
The runner stays (manual tool, oracle `tools/toml_gradus_oraculum.sh`,
filtered and snapshot runs) until the switch has stood; its deletion is
desideratum ...MX8N (with the 'suitae probationum' inventory lens, which
reads the runner).

## Slice 6 as built - summary

The tool now has: registries with conformance fixtures for input kinds
(13) and step kinds (`probationes_c`), and a census; inputs on two axes
with aliases and `repositorium`; the judges' stage; compilator and the
aedilis extractor as libraries linked into bin/fabrica; `FabricaGradus`
(members = synthetic `iudicium` actions in their own areas, exact base
environment, undeclared-variable and outside-write refusals, orphan
areas); `probationes_c` end to end; composites with `post` and the
VERDICTUM line; toml judged through its steps (oracle 13/13, A/B over
150 commits: 273 -> 87 test runs; the gain is the narrower key, not
per-test granularity, on this suite). One flag source (aedilis.stml =
vexilla.sh). Not done (recorded): volumes / filum backend (next slice),
other step kinds, source annotations beyond `facultas`, snapshot builds,
PORTAE as a derived view, other runners (root and silva are where
per-test granularity should show), deleting the toml runner (...MX8N).

## Housekeeping after slice 6

H1 (machine output) - `bin/fabrica iudicare|sanare -machina` prints TSV
records; the first field is the type: IUDICIUM (status, artifact,
cause), COMPOSITUM, VERDICTUM (title, N, M, the non-fresh members),
SANANDA, ORPHANUM, BINARIA, PRAECONDICIO, SANATIO (event, title, ms,
cause), AGITUR, UNDA, NOTA, SUMMA. Tabs and newlines inside a field
become a space and ` | `. Errors (exit 2) stay on stderr. The human form
is unchanged. Consumers moved off the human lines: silva.py
(`_porta_per_gradum`, `_porta_per_fabricam`, `_stala_celeria`,
`_fabricam_exigere` via `_machina_legere`/`_machina_humana`),
`tools/generata_iudicare.sh`, `tools/toml_gradus_oraculum.sh`. The
pythonica fakes now speak TSV. Fumus XL is the contract: every line has
a known type, and the VERDICTUM counts are checked before and after a
heal. Plants: VERDICTUM N miscounted -> red; one human line leaking
into machine mode -> red ("lineae ignotae 1").
H2-H4 (file splits, no behaviour change) follow.

H2 (library split) - lib/fabrica.c (8,515 lines) is now seven files, the
code moved as whole ranges in its original order: fabrica.c (core:
helpers, sutura, manifest, judgment - action seal, read traces, store,
regeneration, strategies, fabrica_iudicare; ~1,830), fabrica_genera.c
(input kinds; ~1,680), fabrica_declarationes.c (reader, subsystems,
composites, preconditions; ~1,510), fabrica_ordo.c (dependencies,
order, write traces, waves; ~670), fabrica_sanare.c (~1,890),
fabrica_gradus.c (areas, environment, step registry, expansion,
orphans, step composites; ~620), fabrica_probationes_c.c (~360).
include/fabrica.h names the six extra bodies with `<aedilis corpus>`;
the 24 helpers that cross files are declared in the private
include/fabrica_interna.h with the prefix `fabricae_` (genitive -
distinct from the API, and no clash in the link-everything suite).
Behaviour unchanged: census, `iudicare -plenus -omnia -machina` and
`sanare -siccum -machina` byte-identical before/after (RECENS causes
normalised - memo and audit state, not code); fumus XL/XL; root suite
green. Plants in fabrica_sanare.c: the wave path's dependency check ->
fumus red, the serial path's -> root suite red.

H3 (test harness split) - probationes/probatio_fabrica.c (7,178 lines,
one `principale` of 38 blocks) is now six suites that mirror the
library files - probatio_fabrica_{genera,iudicium,declarationes,ordo,
sanare,gradus}.c - over a shared fake world,
probationes/fabrica_mundus_fictus.{h,c}: the in-memory disk, fake
sutura, fake actions, runs and traces, and the five knob variables
(51 names, prefix `mundi_`). Helpers that only one suite uses stay
static in it. Every section that changes a knob now puts it back
(transitum left `effectus_effusio` set - reset added before the split),
so the suites do not depend on order. Oracle: the same 37 sections with
the same assertion count each (772 in total). Plants: a fake-world fault
(deleting does nothing) -> iudicium and ordo red; the serial-path
library fault -> only sanare red, so a red suite now names the area.

H4 (the tool split, option A) - tools/fabrica.c (4,550 lines) keeps the
commands only (~2,200: declarations, orphans, iudicare, sanare,
digestum, purgare, causae, principale). The real world moved into
three files: tools/fabrica_sutura.c (disk, processes, act and act in
parallel, tree snapshot; ~1,340), tools/fabrica_memoria.c
(build/fabrica.db: verifications, read traces, particles, verdicts,
runs; ~670) and tools/fabrica_ansae.c (step hooks; ~410). They share
tools/fabrica_sutura.h, which names the three bodies with
`<aedilis corpus>`; the 21 shared names use the prefix `suturae_`.
Before the cut, five blocks that each assigned the real hooks one by
one became helpers: suturae_legentem_parare (read and list),
suturae_currentem_addere (run and ask), suturae_agentem_addere (act,
snapshot, parallel choice, extra directories) and
suturae_memoriam_nectere (the memory hooks). Iudicare now also wires
`cursum_legere`, which only fabrica_sanare reads - no change in
behaviour. Oracle as in H2: identical, fumus XL/XL. Plants: no `agere`
in the acting helper -> XVI/XVII red; area preparation failing ->
XXXVII red. Option B (the real sutura as a library) is desideratum
...DCF4G1; note that the memory code says sqlite must stay out of lib/.

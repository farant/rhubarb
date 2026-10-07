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

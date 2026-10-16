# unitates codicis - what a piece of code IS (units, roles, provenance, custody, language) (spec, DRAFT)

*Born 2026-10-15 in quinta, from Fran: "maybe it would be helpful to
start to have a semantic distinction between different sets of code ...
libraries / modules ... apps that are consumers ... cli tools ...
vendored code". It continues desideratum …MJ2 (2026-10-08, "libraries
vs applications", child of the conventions-as-data vision …R43654),
which already asked for a kind registry in STML and said: extract
conventions from real repetition, not ahead of it.*

*Status: DRAFT for Fran's review. Nothing built, nothing moved. Fran
10-15: reorganizing directories is welcome once the properties and
their members feel solid - not before.*

*Discipline note: the PULL rule. A property, kind or role enters the
registry when a CONSUMER reads it (a lint scope, a gate derivation, a
docs rule). The census lists candidates; it does not oblige us to
declare them.*

## 0. Data - census of 2026-10-15 (dcf80273, 10 090 tracked files)

Read-only script (scratchpad `census.py`, ordered path rules + markers);
full report noted on …MJ2. What changed the design:

- **Generated is not a place.** 101 files carry `GENERATUM` on line 1;
  98 of them are OUTSIDE `amalgama/` - capsules in `apps/*/assets`,
  `*_fontes_generata.sh`, `include/planta_lectio.h`, knotapel
  `demo-snapshot.c`. Provenance is its own property (§II), and the
  marker plus fabrica's action is already its truth.
- **Subsystems are containers.** `silva/` holds library 66, tool 69,
  test 63, fixture 103, script 57 files; every subsystem with a
  `compile_probationes.sh` mixes roles. A subsystem is a UNIT that
  contains files of many roles - not a role.
- **"What is not ours" is answered nine times, eight ways.**

  | site | scratch | build | fixa | amalgama | archivum | knotapel | vendor | oracula | other |
  |---|---|---|---|---|---|---|---|---|---|
  | tools/unci-git/lint_latinus.sh:12-13 | x | x | x | x | x | x | x | x | |
  | pythonica/silva.py:2908 `_EXCLUSA_LINTRI` | x | x | x | x | x | x | x | x | |
  | tools/formare_viae.sh:20-22 (commit) | x | x | x | x | x | x | - | x | |
  | .claude/hooks/formator-custos.sh:20 | x | x | x | x | - | - | - | - | |
  | .claude/hooks/examen-custos.sh:21 | x | x | x | x | - | - | - | - | |
  | .claude/hooks/excubitor-custos.sh:19 | x | x | x | - | - | - | - | - | |
  | .claude/hooks/canon-custos.sh:35 | x | x | - | - | - | - | - | - | probationes/ |
  | .claude/hooks/censor-custos.sh:21 | x | x | - | - | - | - | - | - | censoris/ |
  | .claude/hooks/natura-custos.sh:45 | x | x | - | - | - | - | - | - | |

  Plus per-file lists: `silva/probationes/fixa/examinis/exclusiones.txt`,
  `.../formatoris/exclusiones.txt`, `canalis_debita.tsv` (222 lines).
  Consequence today: an EDIT in vendor/ oracula/ knotapel/ archivum/ is
  judged by the formator and examen hooks; the COMMIT skips those
  directories.
- **Roles the first draft missed:** assets (JS/CSS baked into C
  capsules: `lib/mensa_assets`, `lib/speculum_assets`,
  `tools/silex_assets`, `apps/*/assets`); a home-grown tool in another
  language (`tools/vox`, `tools/tts.ts` - TypeScript); fixtures outside
  `probationes/` (`tools/specimina_anthropic/*.json`, captured API
  replies); natura's data (`natura/*.genera`, `*.census`).
- `principale()` in 918 files: test 573, research 168, tool 140, app 12,
  fixture 17 (sample programs as test input) - "has an entry point" is
  not a kind either.

## 0b. Census U1 under the model (2026-10-15, scratchpad `census2.py`)

- **Units found:** 54 directory units (14 subsystems, 14
  `*/instrumenta`, 10 apps, 5 `tools/<dir>`, docs 3, foreign 2,
  research 2, archive 1, data 1); 322 library stems in
  `include/`+`lib/`; 57 `tools/*.c` with an entry point (+6 helpers,
  all claimed by prefix: `fabrica_{ansae,litterae,memoria,portae,
  sutura}` -> fabrica, `natura_canones_emissio` -> natura_canones).
- **Library families:** 40 stems are prefixes of others (77 members).
  19 members are BACKENDS of one library (`_macos`, `_posix`,
  `_interna`, `_impl`, `_tabula`: `energia_macos`/`energia_posix`) -
  a suffix rule handles them. 58 look like CHILD libraries
  (`fabrica_sanare`, `arbor_lexema`, `imago_png`, `norma_inferre`,
  `vicus_quaestio`) - the real decision: one family unit, or separate
  libraries grouped under a parent?
- **Root tests:** 295 of 335 match a library stem. Of the 40 left, 20
  match an APP name (`probatio_pictor_*` 17, `scriba_*` 2,
  `calendarium` 1) -> stem matching must include app/tool unit names;
  the other 20 are legacy or cross-cutting names (`combinado`,
  `compound`, `iteratio`, `libro`/`libri`, `lapifex_*`, `navigator`,
  `notarius`) - per-file decisions or renames.
- **No unit (793 files):** `probationes/` 588 (mostly `fixa/` - fixtures
  follow their test, and fabrica's READ LOGS already know which test
  reads which fixture: use them, do not guess), `tools/` 84 (scripts:
  `*_fumus.sh` -> the tool's tests, `*_struere.sh` -> its build), root
  59 (`compile_tests.sh`, `aedificatio.stml`, docs, media) -> **the
  repository root is itself a unit** (fabrica.stml already has
  `<subsystema via="."/>`), `.claude/` 18 (hooks: tools, audience
  machina).

## I. The model: units, then files

**Unit** (unitas) = a registered, named thing that claims paths and
nests: `silva` (subsystem) contains `silva/instrumenta/arbor` (tool);
`apps/vicus` (app); `xar` (library); `knotapel` (research);
`vendor` (foreign). A file belongs to exactly ONE unit - the innermost
whose paths contain it. "This README is docs, and it belongs to vicus"
falls out of nesting.

Unit attributes:
- **genus** (kind): bibliotheca (library), applicatio (app),
  instrumentum (tool), subsystema (subsystem - a container of the
  others), investigatio (research/demos - knotapel), alienum (foreign:
  vendor, oracula glue). Open set (§III).
- **cura** (custody): domestica (ours, judged), aliena (foreign, never
  judged by house rules), exemplar/investigatio (ours, relaxed),
  archivata (frozen). Inherited by every file of the unit. Custody is
  what the nine exclusion lists of §0 are all asking.
- **usus** (audience, tools only, MANY allowed; §VI⅞b): homo (tools Fran
  chooses to run - vox, quaestio), agens (LLM/developer-facing -
  legati, natura, silva.py, mensor quaestio), machina
  (build/validation - fabrica, aedilis, examen, formator, lint, gate
  runners). mensor is agens + machina.

**File** properties, relative to its unit:
- **munus** (role): fons (source), caput (header), probatio (test),
  fixtura (fixture), documentum (docs), assets (non-C baked into
  capsules), declaratio (stml/canon/toml the build reads), scriptum
  (glue/runner). Mostly derived from path conventions INSIDE the unit.
- **provenientia** (provenance): manu / generatum. Truth = the
  `GENERATUM` marker + fabrica's declared action (generata gate T7b
  already enforces "every GENERATUM file has a generator").
- **lingua** (language): c89, objc, c99-glue (oracula), sh, python, js,
  ts, stml, md, ... Derived from the extension; exceptions declared
  (oracula's .c is C99 glue - the C89 formatter must not read it).

## II. Where the truth lives

- **Units are declared next to the code they own** (lesson of Bazel
  `BUILD`, Cargo workspaces, Go modules, CODEOWNERS): a small
  declaration per unit. Candidates: (a) a `<unitas>` element in the
  existing per-subsystem `aedificatio.stml` (fabrica.stml already lists
  the 10 subsystems), or (b) a separate `unitas.stml` per unit
  directory. (a) reuses a file fabrica already reads; (b) covers units
  with no build declarations (vendor/, knotapel/, docs/).
- **The kind registry is global** (the 10-08 "registrum generum") -
  and may not need a new format: natura (`bin/natura`, 35 models, 460
  genera; Porphyrian tree, `project-specs/arbor-porphyriana-spec.md`)
  is the house taxonomy layer, where kinds carry properties and species
  are kinds themselves (instrumentum -> species by audience). A natura
  model "codex" could BE the registry (§VI Q8):
  for each kind, role and audience - meaning, which roles a unit of
  that kind may contain, default custody, which rules attach. Adding a
  kind = adding an entry; C dispatches on data.
- **Provenance and language are mostly inferred** (marker, extension);
  declarations only state the exceptions.
- **Read in C** (lib, consumed by fabrica, silva/examen, the hooks
  through a CLI): one answer to "what is this file", never a tool's own
  regex. pythonica stays a thin wrapper (it calls the CLI).

## III. Extensibility

Kinds, roles, custody values and audiences are open sets declared in
the registry. A new tool audience, a "research" kind, a "data" role:
one registry entry, no code change in the consumers that dispatch on
attributes ("is custody domestica?", "does role probatio carry the
credo rule?"). Unknown values in a unit declaration are REFUSED with
the list of known ones (a silent default is a dead gate).

## IV. First consumer (pilot): custody replaces the exclusion lists

The nine sites of §0 all ask "is this file ours to judge?". Pilot =
the Latin lint (`lint_latinus.sh` + `silva.py` `_EXCLUSA_LINTRI`), then
the commit formatter, then the six hooks, one commit each, each with a
test that the new answer agrees with the old list on every tracked file
- and that names every file where they DISAGREE (the disagreements of
§0 are real decisions, made explicitly, not inherited by accident).

## V. Plan (proposed)

- **U1** census v2 under this model: assign every tracked file a unit
  (innermost) and the four properties from path rules + markers; report
  files with no unit, units that would need declaring, and ambiguous
  roles. Read-only.
- **U2** Fran decides kinds/roles/audiences and the open questions
  (§VI).
- **U3** registry STML + unit declarations for the units U1 found;
  C reader (lib) with tests; CLI `bin/<?> quid VIA` ("what is this
  file": unit, kind, role, provenance, custody, language).
- **U4** pilot consumer (§IV), then the other exclusion sites.
- **U5** later consumers by PULL: gate derivation (a library change
  owes its consumers; an app is a leaf), docs expectations per kind,
  merge-time "which installed tools changed", the …MJ2 app layout.
- **U5b** interrogationes pilot (§VII): xar + piscina, dead-answer
  check through silva, then the pool query.
- **U6** reorganization (moves, `genus__` prefixes) only when U1-U4
  have held - Fran 10-15.

## VI½. Q1 answered (Fran 10-15): three mechanisms, each where it fits

| unit shape | mechanism |
|---|---|
| directory (app, subsystem, vendor, research) | `unitas.stml` in the directory |
| single file (tool, script) | in-file annotation, like `<porta>`/`<cursus>` |
| library in `include/`+`lib/` | stem convention (`xar` claims `include/xar.h`, `lib/xar.c`, `lib/xar.worklog.md`, `probationes/probatio_xar.c`) + one short families list (`fabrica fabrica_*`, `arbor2_*`) |

Never declared: dependencies (aedilis closure), most roles (path
conventions), language (extension), provenance (marker + fabrica).
Declared minimum: titulus, genus, claimed paths (default: own dir),
custody (default from kind), tool audience, exceptions, one-line
descriptio; foreign code adds origo (source, pinned version, license).
`aedificatio.stml` stays about BUILDING; `apps/*/organum.stml` is the
app's RUNTIME manifest - neither absorbs unit identity; units point at
their actions. Q3 follows: per-library units come free from the stem.

## VI¾. Q8 answered (Fran 10-15): natura holds the KINDS - three layers

1. **Ontology = natura.** What a library/app/tool/research/foreign unit
   IS. Builds on genera that already exist: `/elementa_programmatis/
   bibliotheca` ("collection published for reuse"), `/elementa_
   programmatis/programma` ("collection with its own entry point" - the
   library/program line), `/fabricatio/adscitio` (foreign work adopted
   into a project = vendored), `/fabricatio/probatio` (species unitatis/
   integrationis/regressionis/fumi), `/plagula_computatralis` (file
   language/format - its header already names "repository files marked
   with their kind" as its purpose), `/documentatio` (manuale, exemplum,
   dictionarium; defectus_documenti: deceptio, silentium, ambiguitas).
   Missing (to add): `programma` -> species applicatio, instrumentum;
   subsystema; investigatio; custody; unitas + "role within a unit".
   `bin/natura quaere` ~40 ms cold.
2. **Policy = a small repo-local file.** What rhubarb DOES with each
   kind (custody aliena -> lint/formator skip; which roles a kind may
   contain), referring to natura kinds by address. Kept out of natura:
   natura is a REUSABLE semantic library (multilingual glosses, no
   rhubarb rules) - the same split as canon rules naming genera.
3. **Instances = the unit declarations of §VI½**, e.g. `<unitas
   titulus="vicus" genus="/elementa_programmatis/applicatio">`. Units
   are individuals of natura kinds, declared in the repo, not in natura.

Fran: natura's types are a work in progress and this is their first
real practical consumer - where a kind is missing or does not match
reality, CHANGE natura (through natura_examen's contract), do not work
around it. Smaller calls (proposed): tool audience = a multi-valued
PROPERTY, not species/etiam=; natura's defectus_documenti species become
the interrogationes audit categories (silentium = uncovered API,
deceptio = dead answer, ambiguitas = two units answering one question);
extend elementa_programmatis rather than start a parallel model.

## VI⅞. Q2 and Q4 answered (Fran 10-15: nesting as a general affordance; details to Claude)

- **Q2.** Nesting is GENERAL: any unit may contain units (apps too -
  …MJ2's `apps/APP/SECTIO/`). `subsystema` is a KIND whose policy says
  "contains other units and owns the shared files that bind them"
  (`aedificatio.stml`, `compile_probationes.sh`, README) - a kind so
  policy can attach ("a subsystem has a suite entry point").
- **Q4.** Tests and fixtures are ROLES inside the unit they test, not
  units: "which tests cover xar" = a role query within `xar`. The root
  `probationes/` is a LOCATION; `probationes/probatio_xar.c` belongs to
  `xar` by the stem convention. Fallbacks: a test matching no unit's
  stem (integration, e.g. `probatio_vicus_quaestio_interna`) belongs to
  the innermost enclosing unit, else the root suite's unit; fixtures
  follow their test (`md/probationes/fixa/computus/` -> the owner of
  `probatio_md_computus`).

## VI⅞b. Q5, Q6, Q7 answered (Fran 10-15)

- **Q5 custody** = how strictly house rules govern the code; four
  values: domestica (ours, all rules), remissa (ours, loose - house
  style never blocks: knotapel, whose C99 plants unci fumus XX already
  excludes), aliena (foreign, never judged: vendor/, oracula/),
  archivata (frozen, no rules, no gates: archivum/). "exemplar" is a
  KIND if ever needed, not a custody. Kind and custody stay separate
  because units are PROMOTED: knotapel is research (default remissa),
  yet math foundations already graduated from it into house libraries
  - promotion changes custody, not necessarily kind.
- **Q6 tool audience** = homo / agens / machina (multi-valued
  property). Reuses the house caller vocabularies: tabularium `actor`
  fran/claude/machina, mensor `vocator` homo/agens/commissio/uncus/
  fabrica/frigida (the last four = machine callers). Free audit:
  declared audience vs OBSERVED callers in the run record.
- **Q7 CLI** = `bin/fabrica quid VIA` (unit, kind, role, provenance,
  custody, language), beside the wished `bin/fabrica cur VIA`
  (…F89); fabrica already loads the declarations and owns gate
  derivation (the second consumer). Not natura (reusable ontology,
  no rhubarb policy - §VI¾). Agents: the same answer as a legati MCP
  tool `unitas {via}`.

## VI⅞c. Tests are several facets; library families (Fran 10-15)

How a test RUNS is orthogonal properties, not one kind:

| facet | values | example today |
|---|---|---|
| role | probatio (asserts) / **exploratio** (demo, exploration: runs and shows, judges nothing) | `probatio_pictor_toy`; some of the 20 legacy names of §0b |
| species | unitatis, integrationis, regressionis, fumi (natura `/fabricatio/probatio` already) | fumus scripts = fumi |
| **interactio** | automatica / **manualis** (needs Fran: watch, click, decide) | `icones_fumus -agere`, `fasciculum_fumus -agere` |
| **occupatio** | nihil / **fenestra** (opens windows) / **focus** (native events take the machine) | laboratorium, apps/forum, apps/pictor fumus (`manus_clavem` steals focus) |

Consumers: `fusio_probare`'s "manu debita" list says WHY each is manual;
`occupatio="focus"` never runs detached / asks first (today only a
memory note, laboratorium-gui-test-focus); `manualis` is queued for when
Fran is present instead of silently skipped. Generalizes the existing
`<porta ... manu causa="...">` annotation (aequivalentia). exploratio
files are research-role: never graded as tests.

**Library families (Fran: good):** children are SEPARATE libraries
nested under a family unit (`imago` family: `imago`, `imago_png`,
`imago_opus`, ...) - "who uses imago_png" stays a query, "the imago
family" too. BACKENDS (`_macos`, `_posix`, `_interna`, `_impl`,
`_tabula`; 19 of 77 members) belong to their single library by a
suffix rule, not a decision.

## VI⅞d. Lifecycle split from custody (Fran 10-15)

Fran: a flag for units SUPERSEDED by later generations. "Archived" is
where a unit is in its LIFE, not how strictly it is governed - so:
- **cura** (custody): domestica / remissa / aliena (archivata moves);
- **vita** (lifecycle): viva (active) / **vetusta** (legacy: superseded,
  still present, maybe still runs, not extended - names its
  `successor`) / archivata (frozen, out of use).
The successor link lets tools point forward (arbor v1/v2 -> silva,
lapifex-era tests). §0b: `combinado` is an APP (Fran), not a test.

## VI⅞e. U3 step 1 done: natura additions (2026-10-15)

METAMODULUS read in full. Its RESIDENCE RULE (§1.4: "the library holds
what is timelessly describable ... YOUR individuals live in application
volumina and CITE the library") is the Q8 split; its PIEPER rule ("every
property must be true of the kind") moved CUSTODY out of natura entirely
(house policy; foreignness is already `/fabricatio/adscitio`, "first-
party elsewhere, adopted here"). Already present and REUSED: `programma`
species `applicatio`, `instrumentum_lineae`, `daemon`;
`instrumentum_aedificationis`; `/necessitudines/substituit`,
`continet`, `exercet`, `orta_ex`; `/laboratorium/experimentum`.

Added (natura_examen SANUM, VULNERA 0, MONITA 0; 468 -> 471 genera):
- `elementa_programmatis` v9: **`opus_programmatis`** (general kind -
  METAMODULUS §9 "several specialised, no general"), machine `vita`
  (viva -> vetusta per supersessio -> archivata), relations continet /
  substituit / orta_ex; `bibliotheca` and `programma` re-parented under
  it (addresses unchanged - §4b); **`subsystema`** (parts: build
  declarations, suite entry point); **`opus_explorationis`** (research
  work; exercet `/laboratorium/experimentum`); `programma` property
  **`usus`** (homo/agens/machina, multiplex - the artefact's final
  cause).
- `fabricatio` v12: `probatio` properties **`interactio`**
  (automatica/manualis) and **`occupatio`** (nihil/fenestra/focus).
- NAME COLLISION caught by the canon monolith: `investigatio` is
  `/iudicium/investigatio` (inquiry into the causes of an event - a
  different kind); the research kind is `opus_explorationis`. Read
  "investigatio" in §I/§VI⅞b above as this kind.

## VI⅞f. U3 step 2a: policy file + first declarations (2026-10-15)

Fran approved: policy in its own root file (not fabrica.stml, which is
silex's repository marker); `unitas.stml` files found through git's
file list (tracked + new), never a directory walk (a walk needs its
own "skip build/" list - a tenth exclusion list); the PULL scope for
declarations.

- `unitates.stml` (canon `unitates.canon`): custody, lifecycle, the 5
  kinds the declarations cite (natura addresses) with default custody,
  ORDERED role rules (directory segment and/or extensions, first match
  wins - no globs, no expression language), language by extension,
  judges (`<iudex>`), library stem convention + backend suffixes,
  families.
- **The nine lists are three properties, not one.** The lint's list
  decomposes: vendor/oracula = custody aliena, knotapel = remissa,
  archivum = lifecycle archivata, fixa = role fixtura, amalgama =
  provenance generatum, build/scratchpad = untracked. So a judge
  declares every list it judges (`curae vitae munera provenientiae
  linguae`, all required - absent never means "all"), and each
  exclusion site becomes one `<iudex>` line. Only `lint_latinus` is
  declared now; the others arrive with their consumer (U4).
- 29 `unitas.stml` (canon `unitas.canon`): root `rhubarb`, 13
  subsystems, 11 apps, vendor (with `<origo>`: sqlite 3.53.3, stb_image
  2.30), oracula (subsystem, `cura="aliena"`), knotapel
  (opus_explorationis), archivum (`vita="archivata"`).
- **Found while declaring:**
  - `saltuarius` calls itself "the first true APPLICATION in the
    monorepo" - declared as an app with its own suite (nesting is
    general, an app may have a runner), not a subsystem.
  - Four apps share a name with a library stem (`nomenclator`,
    `quaestio`, `terminale`, `vicus`), so unit identity = kind +
    title. Whether an app CONTAINS its same-named library is a U5
    (…MJ2 app layout) question.
  - Tool units are NOT derived from `tools/*.c`: a prefix rule
    misreads `auscultator_terminalis` and `briar_spectator` (tools of
    their own) as helpers, and `principale` detection is text
    guessing (`natura_quaesitor.c` spells it `main`). Q1 already says
    single-file tools are declared by in-file annotation; they wait
    for their consumer (the `usus` audit). Until then their files
    fall to the root unit.
  - `officina/amalgama/officina.h` (and silva's vanilla `silva.h`) is
    HAND-WRITTEN inside `amalgama/`: the lint skips it by directory
    today, the new model would judge it (manu). The first real
    disagreement for the U4 agreement test to name.
- The canon hook runs from the session's project directory (main
  checkout), whose registry lacks the new dialects until merge - in
  quinta, judge with `bin/canon_examen` directly.

**U3 step 2b (2026-10-15): reader library `unitates`** (`include/
unitates.h`, `lib/unitates.c`, `probationes/probatio_unitates.c`, 8
credo sections, 5 plants red). `unitates_legere` (policy + declarations
through a sutura, every refusal at once), `unitates_quid` (innermost
unit + custody, lifecycle, role, provenance, language, each with the
rule that decided it), `unitates_iudicat` (judge -> IUDICAT /
NON_IUDICAT with the first excluding property / IUDEX_IGNOTUS). The
policy gained `<usus>` (audience values are an open set like the
others). Inheritance and lookup rules: `lib/unitates.worklog.md`.
Next: `bin/fabrica quid VIA` (disk + git sutura) with a fumus over the
REAL tree (no refusals; spot answers), then the lint pilot.

**U3 step 2c (2026-10-15): `bin/fabrica quid` + gate `unitates`.**
`bin/fabrica quid [-machina] VIA...` (every field with its cause),
`-iudex TITULUS VIA...` (IUDICAT / NON_IUDICAT + cause; unknown judge
= exit 2), `-omnes` (every unit). Sutura = `git ls-files -co
--exclude-standard` + filum (`tools/fabrica_unitates.c`). Gate
`unitates` (`tools/unitates_fumus.sh`, praeiudicium, ~1 s): canon over
policy + every declaration (they had NO gate before - INTECTA), load
without refusal, 10 known answers, the lint judge on one file per
directory the lint excludes by name. Born red by 3 plants (policy
custody, unknown value, unknown attribute).

First contact with the real tree: 329 units (29 directory + 300
libraries; 7 families nest 34 children), load 0.39 s, no refusal;
judging all 2107 tracked .c/.h/.m files 0.39 s.

**Lint preview (old name list vs `iudex lint_latinus`)** - the U4
decision list, measured:
- Found a RULE BUG first: test-support headers (`probationes/*.h`, 15
  files: `pictor_commune.h`, `fabrica_mundus_fictus.h`, ...) fell to
  the fixture rule - the test rule covered only `.c .m`. Now `.c .m .h`.
- 43 files the lint judges TODAY that the model skips - all GENERATED
  outside `amalgama/` (37: `silva/fontes` tables, `lib/*_lectio.c`,
  `*_tabula.c`, `instrumenta/principalia/*_generata.h`; 6 asset
  capsules). Open question for U4: should the Latin lint judge
  generated code? For: a sh/python generator's emitted names are
  checked nowhere else. Against: the fix is in the generator, and
  amalgama is skipped today for the same reason.
- 3 files the model judges that the lint skips today: the hand-written
  vanilla headers inside `amalgama/` (`silva.h`, `officina.h`,
  `tessera.h`).
- Tool helpers are ALREADY declared: `tools/fabrica_sutura.h` names
  its bodies with `<aedilis corpus="tools/fabrica_portae.c"/>` - the
  "tool owns these files" fact Q1 meant to annotate exists as data;
  the tool-unit step should read it, not add a second declaration.

## VI⅞g. U4 step 1: the Latin lint asks the judge (2026-10-15)

Fran: generated code stays judged (a sh/python generator's emitted
names have no other check); fixtures are skipped (they are inputs -
JSON, PNG, expected output, foreign-style C samples); the 3
hand-written amalgama headers are judged.

**A tenth list, and the lint was split against itself.** §0 counted
the hook's directory list; the lint's real SCOPE was elsewhere -
`ORATIO_VOCABULA_EXCLUSA` (knotapel vendor archivum oracula) applied to
the nexus index. fixa/ (135 indexed files) and amalgama/ were judged
on every run but never triggered the lint: an unknown word committed
in a fixture passed, and the next lib/ commit was refused for it.

Now ONE answer, `<iudex titulus="lint_latinus">` (domestica, viva
vetusta, fons caput probatio, manu generatum, c89 objc), read by:
- `oratio/instrumenta/vocabula.c` - tracked list through
  `unitates_viae_iudicatae`; missing list or empty result = exit 2
  (an empty filter means "all paths" downstream);
- the oratio pin test - the same filter, so pin and lint cannot drift;
- the commit trigger - `lint_latinus.sh` and `silva.lint_latinus_
  praevium` ask `./oratio/vocabula.sh -iudicandae VIA...` (no regex
  left; `bin/fabrica` is not used because bin/ is not committed).

Measured: tolerated copy -44 lines (fixture-only words + 15 already
stale), +0; NOVA 0. Plant: fixtura added to the judge's roles ->
`NOVA 29`, exit 1; restored -> NOVA 0, evanida 0. Remaining exclusion
sites of §0: commit formatter, the six hooks, `ORATIO_PROSA_EXCLUSA`
(prose lint), examen/formator per-file lists - one commit each.

## VI⅞h. U4 step 2: the commit formatter (2026-10-15)

`tools/formare_viae.sh` (silva.commissio + the pre-commit hook) asks
`<iudex titulus="formator_commissionis">` (domestica, viva vetusta, fons
caput probatio, MANU, c89). Measured against the old rules over 1699
files: identical except six -
- vendor/ (3 files) was formatted on CHANGED LINES ("terra non
  evoluta", 2026-09-01). Fran: never touch vendor (e.g. a worktree
  building an amalgam out of bearssl - not ours to lint or format).
  Now: custody aliena, skipped.
- the 3 hand-written vanilla headers in `amalgama/` (590/412/205
  divergences, never formatted). Fran: amalgams stay unformatted; if
  ever formatted, that is a STAGE OF AMALGAM GENERATION. Modelled as a
  ROLE, not a language: `<munus titulus="amalgama" segmentum=
  "amalgama"/>` (the shippable product, generated .c and hand headers
  alike). The formatter's judge omits it; the lint's judge lists it
  (lint scope unchanged: NOVA 0, evanida 0).

**One filter for shell consumers:** `tools/iudex_unitatum.sh [-causae]
JUDGE [VIA... | stdin]` - self-building (crusta/facies.sh pattern:
aedilis closure + bin/compilator; no bin/fabrica, which a fresh clone
lacks), ~0.6 s warm. `-causae` names every rejected path with its
cause on stderr (the hook's XIX stage asserts the GENERATUM file is
NAMED as skipped - a silent skip would have been a regression). The
lint trigger moved onto it too; vocabula's lint-only `-iudicandae`
(U4 step 1) is gone. `bin/fabrica quid` remains the explaining CLI for
people and agents.

Plant: formator judge admits generated -> unci XIX red; restored green.
Gate `unitates` V covers the filter. Found on the way: unci XXII
`UNCUS_CRIBRUM: fixa praeterita` - the hook's examen filter is another
exclusion site (next).

## VI⅞i. U4 step 3: the pre-commit hook (2026-10-15)

`tools/unci-git/pre-commit` had FIVE copies of the directory list, and
they disagreed with each other: examen's lacked vendor (stb_image.h
survived only through the per-file "known noise" list), diagnostics
excluded only the ROOT `probationes/fixa/` (crusta's fixa scripts were
diagnosed), and the `UNCUS_CRIBRUM` copies repeated both. Now three
judges, one filter (`_cribrum JUDGE` -> `tools/iudex_unitatum.sh`; a
broken judge OBSTRUCTS):
- `examen_commissionis` (domestica, viva vetusta, fons probatio ASSETS,
  manu generatum, c89 objc) - vs today: only vendor/sqlite3.c leaves
  (Fran: vendor never touched); generated asset capsules stay judged.
- `diagnostica_materiae` (.sh .css; fons probatio scriptum declaratio,
  manu generatum) - vs today: 12 oracle-input scripts under
  `crusta/probationes/fixa/` leave (fixtures are inputs, now for EVERY
  fixa/, not only the root one).
- `formator_commissionis` also feeds the hook's `-delta` warning.

Two RULE BUGS found by the comparison (mine): `.css` had no role (3
stylesheets -> no role, would have left diagnostics) - now `fons`;
shell tests in `probationes/` (`probatio_icones_iconutil.sh`, ...)
fell to the fixture catch-all, the same bug as the test headers -
the test rule is now `.c .m .h .sh`. Plants: fixtures into
diagnostics -> unci XIV red; remissa into examen -> unci XX red.

## VI⅞j. U4 step 4: the post-edit hooks (2026-10-15)

Four `.claude/hooks/*-custos.sh` now source `iudex_custodum.sh`
(`custos_admittere CUSTOS JUDGE "$FILE" || exit 0`): formator ->
`formator_commissionis`, examen -> `examen_commissionis`, censor -> new
`censor_latinae` (domestica, fons caput probatio assets, c89), canon ->
new `canon_custos` (declaratio, stml). The helper judges in the FILE'S
OWN tree (its policy, its binary - ~30 ms when the built binary is
newer than its sources, else the self-building wrapper); outside any
repository (scratchpad) or git-ignored (build/) = not judged; a tree
without the policy or a broken judge = ONE notice with the judge's
error ("custos tacet"), never a silent pass. formator/examen keep their
old "own checkout only" restriction BEFORE asking the judge (else every
edit in an unmerged worktree would print a notice).

Measured vs the old case-lists (the edit hooks judged MORE than the
commit did, §0): censor 2101 -> 1734 (knotapel 172, fixtures 172,
vendor/oracula 6, archivum 5, amalgama 6 leave - your decisions;
assets kept); canon 138 -> 134 at first - and that exposed a MODEL GAP:
`vendor/unitas.stml`, `knotapel/unitas.stml`, ... inherited the custody
of the unit they declare. **A unit's `unitas.stml` belongs to the
CONTAINING unit** (the container's record of its child, like a Bazel
BUILD file) - root's stays the root's. After the rule: canon identical.

Left as they are, on purpose: `excubitor-custos` (reports who includes
an edited header - a FACT of the include graph, not a house rule; its
fixa/ skip is "fixtures are not in the graph") and `natura-custos`
(.genera only; skips scratchpad/build, which the helper's own
repository/ignore checks would also skip). Gate `unitates` VI drives
the helper and two real hooks (silent on vendor and on the censor's
regression corpus - loud without the filter). Plants: declaration rule
removed -> unit test red; fixtures into censor_latinae -> VI red. The
hooks run from the session's project dir: effective after merge.

## VI⅞k. Provenance made true: the marker position (2026-10-15, Fran: option A)

Found while measuring the prose lint: the house rule "`GENERATUM` on
line 1" was false for ~300 generated files (267 docs pages and 36
natura canons with the marker on line 2 because line 1 must be
`<!doctype`/`<?xml`; 12 cooked grammar registries, silva_latina_datum.h,
silva/c89.canon on line 3; natura/cocta/glossae.html with NO marker).
Every reader used line 1, so the commit formatter treated the 13 C
ones as hand-written (latent: their output happened to be conformant).
Nothing checked the other direction (generator output carries the
marker). A "first N lines" window was rejected: include/norma_canon.h
names GENERATUM in prose on line 2 and is hand-written (Go's lesson:
an exact form or position, never "the word near the top").

**Rule (lib/unitates.c `_provenientia_legere`): `GENERATUM` on line 1,
or on line 2 when line 1 is a declaration the format requires
(`<?xml`, `<!doctype`/`<!DOCTYPE`).** Generators fixed: materia
coctor (12 registries, its test re-pinned), silva latina_datum,
silva canon coquere (c89.canon), natura_glossae (glossae.html).
Splices - a generated REGION in a hand-written file (latina.h
numerals, silva.h / hospes.c public surface; fabrica declares the file
as its own action's input) - carry no header marker: `bin/fabrica
iudicare -machina` now prints a 5th field `splicita` on those IUDICIUM
lines only (other lines byte-identical).

Gate `generata` now checks BOTH directions through `bin/fabrica quid`:
marked -> has a fabrica action ("GENERATUM sine iudicio", as before
but with the real rule) and fabrica output -> marked unless splicita
("SINE SIGNO", new). Result: 143 judged, 0 stale, 0/0. The real rule
exposed 268 generated docs pages (docs/*.html, Python generator
`docs/instrumenta/regenerare.sh`) that NO fabrica action judges -
stale pages possible unnoticed; exception with cause in the gate +
park …PT44JPJA. Plant: splicita disabled -> exactly latina.h, silva.h,
hospes.c "SINE SIGNO". Left: md/CENSUS.md (marker on line 3, no fabrica
action - its input is every *.md) - with the prose lint step; the
edit-guard hook generata-custos.sh keeps its stricter "lines 1-3 + the
do-not-edit phrase" rule (a warning, and it already caught these).

## VI⅞l. U4 step 5: the prose lint; CENSUS.md retired (2026-10-15)

`ORATIO_PROSA_EXCLUSA` (vendor/ archivum/ + two generated files by
name) -> judge `prosa_anglica` (domestica REMISSA - knotapel is
English by decision and stays judged -, viva vetusta, documentum,
MANU, md), read by `vocabula -prosa` and the oratio corpus test.
Measured 1490 -> 1463: the 27 leaving are markdown FIXTURES
(commonmark samples, dependency-treebank, toml-test) - Fran's
"fixtures are inputs" applied to prose. `gesta/annales/tabula.md` was
already untracked (ledger left the repo 10-08): a dead entry, gone.

`md/CENSUS.md` (Fran: on-demand report, nothing depended on it): no
longer committed; `./md/census.sh` prints the report, `-scribere`
removed; the report text says "Relatio", not GENERATUM. It was the
last generated file whose marker sat on line 3 with no fabrica action
(input = every *.md). Gate `unitates` V checks the prose judge
(knotapel judged, a markdown fixture not).

## VI⅞m. U5a: the unit census (2026-10-15)

`bin/fabrica quid -census [-machina]` (lib `unitates_census`): every
enumerated file counted into its innermost unit by role; policy
`<exspectatio genus=... munus=X minimum=N | attributum="usus"
descriptio=.../>` judged per kind (archived units never); plus ORPHAN
root tests (`probationes/probatio_*.c` that fall to the root by the
directory fallback) and UNDECLARED directories (source/header/script/
test files that fall to the root; include/ lib/ probationes/ excluded).
Report, not gate (0.3 s); gate `unitates` VII keeps "every app declares
its audience" (true today) as a ratchet. Plants: archived judged ->
red; orphan dropped -> red; an app without usus -> VII red.

**First real-tree census:** 330 units, 137 with gaps (189):
- 60 libraries without a test - mixed: real gaps, generated tables
  tested through their parent (norma_canon, organum_canon,
  organa_infixa), header-only (latina), font data (fons_6x8).
- 129 of 300 libraries without a worklog/doc - CLAUDE.md says worklogs
  are made "as needed", so this expectation may be too strict: Fran's
  call whether it stays (it is one line).
- 0 apps without an audience.
- 20 orphan tests (U1 counted 40; app names now claim pictor/scriba
  tests). Some have an owner the prefix rule cannot see because the
  library name is LONGER than the test's (probatio_arbor2_lapifex ->
  arbor2_lapifex_adaptator).
- 25 undeclared code directories: `tools` itself (c89 67, sh 113 - the
  single-file tools waiting for tool units, item #3), asset directories
  named `*_assets` (the `assets` role rule matches the exact segment
  `assets` only: lib/mensa_assets, lib/speculum_assets,
  tools/silex_assets, probationes/*_assets), .claude/hooks, pythonica,
  docs/instrumenta, tools/vox (TypeScript), tools/linux,
  tools/unci-git, vim, scripts, episodes, lint, instrumenta,
  book_assets, reference.

**U5a follow-up (2026-10-15, Fran):** the "library has a worklog/doc"
expectation is DROPPED (worklogs are made as needed; many libraries
predate them) - documentation returns with U5b's typed questions.
Ownership fixed instead: a stem claims `lib/S.<any>.md` (worklog AND
phase-log - 7 phase-logs used to fall to the root), `docs/bibliothecae/
S.html` (`<bibliothecae paginae=...>`; those pages are RENDERED from
the ledger's library inventory + the header's first sentence, 4 are
fully hand-written - see park …PT44JPJA note …8T3RF) and `lib/S_assets/`
(`bona="_assets"`). Role rules gained `segmenti_suffixum`: `assets` now
matches `assets` OR `*_assets` (14 capsule directories: html/js/css +
toml manifest + generated .c/.h; Fran preferred the suffix over an
`asset__` filename prefix). `.html` joins the documentation role.
Census after: 60 gaps (all "library without a test"), 20 orphans, 16
undeclared dirs (was 137/20/25). Side effect: test asset capsules are
role assets, so the Latin lint no longer judges them (one tolerated
word, `templates`, left the pinned copy). A bug of mine caught by the
unit test and gate III: the `S_assets` branch preceded the plain
`lib/S.c` branch in an else-if chain and swallowed every lib/ file.
Ledger: `tasks/` folder vision filed …HMFZB2KA (doc-drift linter ->
task; overlaps the ledger's expeditio - decide the home first).

## VII. Interrogationes - documentation as typed questions (Fran 10-15)

A unit exposes what it can do by DECLARING QUESTIONS it answers
("jeopardy style"): "how do I ...?" -> "xar_addere: segments never
move". Why: question-shaped entries are self-contained and identical
in shape, so they pool across hundreds of units by concatenation (API
docs and READMEs do not); they are indexed by the question a reader
HAS (THE BENCH in MEMORY.md - pain -> command - is the most-used doc
an agent has, for this reason); and they are auditable units.

Shape (sketch, details as we go):

    <interrogatio genus="quomodo">
      <quaestio(>how do I keep pointers stable while an array grows?
      <responsum symbolum="xar_addere" (>segments never move; ...
    </interrogatio>

TYPED (Fran: the natural entry point for auditing pain points and
limitations later): quomodo (how do I - answer = function, command,
recipe), possumne (can I - yes/no + limits), cur (why does - decision,
worklog), quid (what is - glossary); open set via the registry (§III):
limitatio, dolor (known pain) are expected later.

Audits made possible by ANCHORED answers (symbol, command, path):
- dead answer: silva resolves `symbolum=`; renamed/deleted -> red;
- API coverage: every public function of `include/X.h` answers at
  least one question (uncovered = a to-do, or not really public);
- overlap: two units answering one question = duplicate function or a
  merge candidate;
- reverse lookup: the questions naming `xar_addere` ARE its docs;
- per-type counts per kind ("every library has >= 1 quomodo").

Pool = a query, not a file: a legati/natura tool ("quaestio: how do I
..." with FTS, like the ledger's). Open: questions in English or Latin;
next to the function (annotation, fresher) or in the unit's file
(easier to audit). Pilot when U3 lands: ~5 questions each for xar and
piscina + the dead-answer check. (`FAQ.md` was an experiment, deleted
in main 6061a54c - not a seed corpus.)

**U5b slice 1 (2026-10-16, Fran):** the open questions are answered:
one file per unit, `lib/S.interrogationes.stml` (not header
annotations: a recipe spanning three functions has no home there, and
an anchor implied by position can never go dead, so the audit weakens);
questions and answers in ENGLISH (the reader types the question that
way), markup and identifiers Latin. Dialect `interrogationes.canon`
(registered by root `<interrogationes>`): `interrogatio genus=` is an
`electio` in the canon (quomodo possumne cur quid - a new type = one
`<optio>`), `<quaestio! (>` = one raw line, `<responsum symbolum= via=
imperium=>` = text; "at least one anchor" is the gate's job (slice 3),
the canon cannot say co-occurrence. The stem rule now claims
`lib/S.<any>.stml` as well as `.md` (role declaratio, so canon_custos
judges it); `lib/S.stml` with no middle segment stays with the root.
Pilot drafts: 5 questions each for xar and piscina, every answer read
against lib/xar.c and lib/piscina.c - Fran edits them. VETITAE lost
`FAQ.md`. Next: slice 2 (reader + census count), slice 3 (dead-answer
gate).

**U5b slice 2 (2026-10-16, Fran: reader in lib/unitates):** role
`interrogationes` in the policy (`extensiones=".interrogationes.stml"`,
before `declaratio` - suffix match, no C rule code; canon_custos judges
it). `unitates_interrogationes(u, piscina)` -> entries {unit, path,
line, type, question, answer, symbols, paths, command} + per-file
errors (foreign root, `unitas=` not the owner, unknown element or
attribute, question/answer missing, twice or empty, answer without an
anchor, illegible STML); never refuses the whole read. Text is the
flow reading (`stml_textus_fluxus`), so the hand form and the
formatter's capture form read identically (pinned). CLI `bin/fabrica
quid -interrogationes [-machina] [UNITAS...]`: entries, per-type counts
per unit and in total, errors; exit 1 on any error, 2 on an unknown
unit. The census counts interrogationes FILES per unit (role count, no
read); the expectation "library answers >= 1 question" is NOT added
yet (it would add ~250 gaps beside the 61 test gaps - Fran's call).
Gate `unitates` stage VIII: canon on every `*.interrogationes.stml`,
reader 0 errors, no empty question/answer, and a bad file in a
throwaway git tree gives exit 1 with both errors NAMED (born red by a
plant: an anchorless answer in lib/xar.interrogationes.stml). Anchors
are still not resolved - slice 3.

**U5b slice 3 (2026-10-16): gate `interrogationes`** (`tools/
interrogationes_fumus.sh`, praeiudicium, ~8 s; Fran: the census
expectation waits until more libraries have questions). Dead answer =
red, NAMED as `path:line kind value - reason`:
- `symbolum`: one `silva/nexus.sh -renovare`, then ONE pass over
  build/nexus.tsv for every anchored symbol (per-symbol nexus.sh calls
  would cost ~1.75 s each). Alive only if a `sedes` lies in a file whose
  role is caput/fons AND provenance manu - asked of `bin/fabrica quid`,
  no path list. A copy in a fixture or the amalgam keeps a deleted
  symbol forever and would hide the death ("sedes solum extra fontem
  manu scriptum" names those copies).
- `via`: exists in the tree. `imperium`: first word with `/` =
  executable file under the root; bare word = on PATH.
Also red: any reader error (exit 1 of `quid -interrogationes`); exit 2
(cannot judge) when bin/fabrica is missing, the nexus table is broken,
or there are no questions at all (a gate that judges nothing is dead).
`tegit` includes `include/*`: a header rename owes the gate. Born red
by five plants (renamed symbol, fixture-only symbol, dead path, missing
script, command not on PATH), each naming its own cause. Gotcha: the
gate-declaration reader (lib/portae.c via portae-declaratae) sees only
TRACKED files - a new, untracked gate script reads as "sine
annotatione" until `git add -N`.

**U5b slice 4 (2026-10-16, Fran: plus a generated docs/faq.html linked
from docs/index.html):** all in `bin/fabrica quid -interrogationes`
(tools/fabrica_interrogationes.c, an aedilis corpus of bin/fabrica):
- `-quaere "TEXT"`: word overlap, question 3 / symbol 2 / answer 1 per
  query word (counted once, best place); English stop words dropped;
  identifiers also split on `_`; match = equal, or a common prefix of
  >= 4 that covers the shorter word but its last 2 letters
  (pointer/pointers, compare/comparing). Exit 1 = nothing matched, 2 =
  only stop words. Honest overlap, not semantics: "allocation fails"
  does not find "fatal".
- `-symbolum X`: the reverse lookup (the questions naming X ARE its docs).
- `-html VIA`: one deterministic page; refuses (exit 1, writes nothing)
  while any question file has an error. Fabrica action
  `interrogationes_pagina` (aedificatio.stml, `lectiones="verum"`):
  inputs = lib/*.interrogationes.stml, the NAMES of include/ (stems =
  owners), unitates.stml, the script, bin/fabrica by provenance + its
  manifest; what filum reads is supplied by the read book. Correctness
  does not hang on the key: generata regenerates under -plenus and
  compares bytes (plant: an edited question -> `STALUM docs/faq.html`).
  docs/index.html links it under Bibliothecae.
- API coverage = `tools/interrogationes_fumus.sh -tegmen [include/X.h]`
  (needs the nexus table, so it lives beside the gate): today piscina
  11/17, xar 8/33 functions anchored, the rest named.
Gate unitates VIII also pins -quaere, -symbolum, -html (line-2 marker,
HTML escaping) and the refusal on a bad file, in a throwaway git tree.
U5b is complete; open: more libraries' questions (then the census
expectation), and whether legati should answer the same query.

## VIII. Open questions (for Fran)

1. ~~Unit declarations~~ - answered §VI½ (three mechanisms).
2. ~~Subsystem~~ - answered §VI⅞ (a kind; nesting general).
3. One library = one unit (`xar`), or `include/`+`lib/` = one big
   library unit with files as members? Per-library units make
   "consumers of xar" a real query; one big unit is simpler.
4. ~~Tests and fixtures~~ - answered §VI⅞ (roles in the tested unit).
5. ~~Custody values~~ - answered §VI⅞b.
6. ~~Tool audience names~~ - answered §VI⅞b (homo / agens / machina).
7. ~~CLI~~ - answered §VI⅞b (`bin/fabrica quid VIA` + legati `unitas`).
8. ~~Kind registry as natura~~ - answered §VI¾ (three layers).

## AUDIENDA (not verified)

- The census rules are first-match path heuristics; role counts are
  approximate (e.g. every `*.md` in `lib/` counted as docs - true for
  worklogs, not checked one by one).
- Whether the hooks' narrower exclusions are deliberate (e.g. formator
  on oracula/) or drift was not asked of their authors' commits.
- natura was read at the level of `include/natura.h` (loader, genus
  lookup, apparatus with inheritance) and the genus lists of six code
  models; METAMODULUS contract rules and how a NEW model is admitted
  were not - read `natura/METAMODULUS.md` before adding genera (U3).

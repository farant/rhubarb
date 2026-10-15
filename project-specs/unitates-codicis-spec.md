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
piscina + the dead-answer check. `FAQ.md` is Fran's (never edited by
agents) - a seed corpus only if Fran moves entries over.

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

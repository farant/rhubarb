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
- **usus** (audience, tools only, MANY allowed): homo (tools Fran
  chooses to run - vox, quaestio), agens (LLM/developer-facing -
  legati, natura, silva.py, mensor quaestio), aedificatio
  (build/validation - fabrica, aedilis, examen, formator, lint, gate
  runners). mensor is agens + aedificatio.

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
- **U6** reorganization (moves, `genus__` prefixes) only when U1-U4
  have held - Fran 10-15.

## VI. Open questions (for Fran)

1. Unit declarations: inside `aedificatio.stml` (a) or `unitas.stml`
   per unit (b) or both (b for units without builds)?
2. Is "subsystem" a kind, or just a unit that contains other units?
   (silva is both a library family and a set of tools.)
3. One library = one unit (`xar`), or `include/`+`lib/` = one big
   library unit with files as members? Per-library units make
   "consumers of xar" a real query; one big unit is simpler.
4. Tests and fixtures of a unit: role inside the unit (proposed), or
   their own units (`probationes/` is shared by all root libraries)?
5. Custody values: are exemplar and investigatio the same thing?
6. Tool audience names: homo / agens / aedificatio - or different?
7. Name of the CLI (`natura quid`? `fabrica quid`? its own binary?).
8. The kind registry as a natura model (kinds = genera, tool audiences
   = species, rules as properties) instead of a new STML format?
   Proposed yes, pending a read of natura's model/property machinery -
   one taxonomy layer, not two.

## AUDIENDA (not verified)

- The census rules are first-match path heuristics; role counts are
  approximate (e.g. every `*.md` in `lib/` counted as docs - true for
  worklogs, not checked one by one).
- Whether the hooks' narrower exclusions are deliberate (e.g. formator
  on oracula/) or drift was not asked of their authors' commits.
- natura's model machinery (properties, state machines, how a model
  is consumed from C) was NOT read for this draft - only its memory
  note. Q8 depends on it; read `project-specs/genera-spec*.md` and
  `arbor-porphyriana-spec.md` before U3.

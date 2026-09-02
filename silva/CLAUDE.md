# silva — orientation

*You are reading this because you touched a file in silva/. This file is the
canonical orientation for silva work: the vision pins, the culture, and the
re-orientation protocol. It exists because vision drift across compaction
sessions is the project's biggest process risk — technical decisions made on
smoothed-over assumptions quietly shave the maximalist scope. That shaving is
how the three previous parsers died.*

## ⚠ GELATIO — furca materiae in cursu (2026-08-27)

**Silva ad EMENDATIONES SOLAS gelata est** dum substratum generale
(`materia`) ex ea furcatur. Consilium: `project-specs/materia-spec.md`;
decretum `01M12FJR` in tabulario.

Quid hoc significat si silvam nunc tangis:

- **Emendatio vitii**: fac, sed nota quod in materiam REPLICANDA est
  cum furca vivat. Si replicatio onerosa fit, furca nimis diu currit —
  scopum SECA, noli extendere.
- **Opus novum in nucleo** (nodus, token, arbor, scribere,
  arbor_aequalitas, quaestio, quaestiones, commissio): **noli**, nisi
  Fran aliter dicat. Hi moduli in materiam migrant; opus hic factum bis
  fiet.
- **Frons C89** (lexema, expandere, conditio, semantica, formator,
  fluxus, tabulae_c89) **et instrumentarium LR** (glr, generare,
  coquere): manent, opus licet.

Quid IAM factum est: `silva_tabulae.h` scissum (M4, `6b5ca0a3` et
`4f1584e2`) — typi registri in `silva_registrum.h`, quod NIHIL includit
praeter `latina.h`. Phasis 0 peracta: chirurgia classificata.

Gelatio finit cum phasis V peracta sit (silva materiam consumens,
portis omnibus viridibus). Si hoc caput adhuc gelationem dicit et
mensis abiit, ROGA — furca superstes esse debet, non pendere.

## Quid sit

Silva is the 4th-generation C89 parsing substrate of the rhubarb monorepo,
after arbor v1 (recursive descent), arbor2 (hand-written GLR), and lapifex
(generated LALR). Deliverable: a single-file amalgamated `silva.c` + vanilla-C89
`silva.h` (SQLite style — develop modular, ship generated) consumed by
**../solarium**, a 3D IDE where code is a database + a 3D structural
representation + literate programming. Speculum (a structured-thinking C89
superset, future STML syntax) sits above; silva stays speculum-unaware but is
its parsing substrate.

## PROTOCOLLUM — re-orientation (session start / post-compaction)

Do these IN ORDER before writing code:

1. **This file** (auto-loaded — done).
2. **silva/phase-log.md** — read the TAIL first: current phase INTENTIO/RELATIO
   and the latest complexity entries. It is the state re-entry point.
3. **The spec section for the phase you're touching** — silva-spec-v2.md
   (§8 token/lexer/expander, §9 grammar/annotations/nodes, §10 conditionals,
   §11 amalgamation + M1 obligations). Compaction summaries preserve RULES but
   lose the WHY; the spec sections carry the why, and the why is what shapes
   structs correctly. **Never implement a chunk from a compaction summary
   alone.**
4. **Any .worklog.md** next to files you're editing.
5. **If the maximalist rule ever feels negotiable, vague, or inconvenient**:
   re-read project-specs/silva-brainstorm.md (5 minutes). For the deep feature
   vision: arbor_ce_lxxxix-brainstorm.md and arbor-v2-all-brainstorms.md.
   These transcripts carry the motivating energy that compression strips.
6. **Warm the bench (~2 min).** Run each read-tier instrument once —
   seeing real output builds the reach that reading a list doesn't.
   Aim at your actual task; known-good fallbacks:
   `./silva/nexus.sh silva_c89_parsare` · `./silva/emitte.sh
   lib/piscina.c piscina_generare_dynamicum` · `./silva/origo.sh
   lib/piscina.c 239:5` ('if' <- si). **Include ONE mcp legati call**,
   aimed at your actual task — the fori ab_lecto principle applied
   here: discovery, not discipline (Fran 2026-07-29). Prefer `corpus`
   on a **TYPE** (struct/typedef) over a function: function-source is
   what you'd assume anyway; whole-struct-bodies-with-field-comments
   is the capability the training prior doesn't predict (it cracked
   the 2026-07-30 span bug in two calls). Known-good fallbacks when
   no task target exists yet:
   `corpus chorda` (the house type in four lines — mensura + datum,
   the NOT-null-terminated reminder for free; for rich field
   comments try `corpus SilvaMacroVista`) ·
   `vocantes _tolera_absorbere` (a 14-caller emission-gate map in one
   call — name-domain blast radius) ·
   `symbolum similitudo_optima` (the full card: signature, comment,
   per-file usage LINES).
   A miss is not a failure, it is the second lesson: unknown titles
   answer with simillima (decurtata — the guess shrinks from the
   right until candidates appear; family names may beat the best
   cousin), and `./silva/nexus.sh -similis <word>` is the wider net.
   The exact-extents anecdote stands: a 110-line grep+Read window
   once truncated a function at line 8487 of 8558 and silently
   dropped its header comment.
   Symptom table (dolor → mandatum) in MEMORY.md; censor runs itself
   (amalgamare porta 6 + post-edit hook); interpretare is
   officina-side.

Before compaction: append a phase-log addendum with anything in-flight.
When narrowing scope: log it in the phase-log with the deferred use case
NAMED and the path back preserved (see VISIO last pin).

**At every PHASE boundary (not chunk): re-read the WHOLE phase-log and
audit it** — every complexity either resolved or parked WITH A NAMED
LANDING SPOT (a phase that owns it). The tail-read catches state; only
the full re-read catches drift between documents (the 2026-07-02 audit
found four such drifts — sketch-vs-spec, entry-vs-follow-through — and
zero code bugs; that is the class this practice exists for). Phase-log
sections are APPENDED, never inserted (chronology is the re-entry
reader's map).

Build/test: `./silva/compile_probationes.sh [filter]`. Tables:
`./silva/generare.sh` regenerates fontes/silva_tabulae_*.{h,c} from
grammatica/*.stml — run it after ANY grammar edit (probatio_silva_tabulae
breaks loudly if you forget; generated files are committed). Amalgam:
`./silva/amalgamare.sh` regenerates the COMMITTED deliverable
silva/amalgama/silva.c and runs its verification gates (standalone
full-severity compile, hospes host-pollution + equivalence,
nm-intersection 0) — run it after ANY fontes edit; the suite does NOT
catch a stale amalgam (silva.h is hand-written; new public API =
extend it). Manifest lists are GENERATED since 2026-07-21: fontes
lists aedilis-derived (fontes_generata.h; policy = fontes_politica.sh
+ tools/amalgama_fontes_generare.sh), excludenda COMPILER-HARVESTED
(excludenda_generata.h; tools/amalgama_excludenda_generare.sh). New
use of an excluded vendored function → re-run the harvest, NEVER
hand-edit the generated headers (the compile clamat either way).
Hand surface = policy file + rename tables + silva.h, nothing else.
Dev-time mains live in instrumenta/principalia/ (outside the test
link glob).
The read bench (instrumenta prima, 2026-07-10): ./silva/censor.sh
(latina lint; also in amalgamare porta 6 + post-edit hook),
./silva/emitte.sh <file> <symbol> (byte-exact definition),
./silva/origo.sh <file> <linea>[:<col>] (expansion story),
./silva/computus.sh <fons.c> [-nudum] [-machina] [-iter N] (imago
memoriae et temporis parseris: lexemata, dati/commissa/otiosa, alvei,
allocationes, ms lexandi/parsandi + HOROLOGIA PHASIUM lex/expansio/
glr/commissio - piscina propria I MiB; porta = probatio_silva_computus,
aurum fixa/computus/basis.tsv; RP §11),
./silva/sample.sh <fons.c> [-iter N] [-secunda S] (profilum 'sample':
tabula foliorum + build/sample/<n>.folded pro flamma - UBI tempus it),
./silva/aequivalentia.sh -basis | -conferre (documenta arboris
octetim contra basim, CAPITA ad basim in disco restituta dum
curritur - porta cuiusque optimizationis; RP 5.1),
./silva/arbor.sh <fons.c> [-nudum] [-tacitus] (documentum STML
canonicum in stdout; compendium in stderr numerat lexemata /
expansa / ambigua / ERRORES. Cave: -nudum super codicem
latinizatum ZERO ambigua reddit quia PARSURA FRANGITUR - XLV
nodi ERROR - non quia resoluta sit),
./silva/nexus.sh <symbol> (who-uses/which-file; -similis fuzzy;
-renovare rebuilds build/nexus.tsv),
./silva/examen.sh <file> [-posix|-nudum] [-machina] (C89 judgment:
verdict + positioned diagnostics naming the offending SYMBOL and,
for included-file rows, the inclusion chain "(per: root > ...)";
M4a + 2026-07-27; also pushed post-edit via examen-custos hook.
Pins = EXPECTED-red files with a named cause per row, format
via<TAB>causa, in fixa/examinis/exclusiones.txt - NOT lexicon
gaps: those get healed via systema_posix.h sectiones, auspex_posix
gate). Differential bar: ./silva/examen_vectis.sh [-corpus]
(oracle leak sweep + pinned invalid corpus + pin diff).
Classification bar: ./silva/ambigua.sh <viae> [-machina] [-loca]
[-pinna silva/probationes/fixa/ambiguorum/pinna.tsv] — retained-
AMBIGUUS census under the max-knowledge lexicon context (baseline
226 pinned 2026-08-19; dominant residual = `(name) * x` inside
macro bodies, local names unknown to the oracle; above-pin =
REGRESSUS, exit 1). Officina-side sibling:
./officina/interpretare.sh (run a corpus function in machinula).
Conventions: root CLAUDE.md + the conventions recorded in phase-log
complexities (i32 is UNSIGNED — s32/s64 for anything signed; never
`nomen`, `registrum`, `magnitudo`, `exire`, or `structura` as
identifiers — all are latina macros (typedef/register/sizeof/exit/
struct; general rule after six firings: EVERY word #defined in
latina.h is reserved, check it before coining; exire silently
renamed a struct FIELD in modular TUs — tessera Phase C); use titulus/tabularium; chorda not
null-terminated; Xar element pointers ARE stable across appends —
segmented, sine reallocatio — only swap-remove/sort/truncate disturb
them, see the 2026-07-02 Correctio).

## VISIO — what silva is FOR

Each pin is a feature commitment paired with the design consequence that
protects it. Smoothing a "quirk" means visibly deleting a feature — don't.

- **Code is a database.** The ur-motivation, older than solarium: "the code
  itself is a data format we can work with in a flexible way." Every struct
  field is a future query column. Information that exists during parsing
  should survive into queryable form, not be consumed and discarded.
- **Layers are the product, not plumbing.** Expansion generations, conditional
  arms, files of a project — a forest (hence the name). Queries traverse
  layers ("does layer 3 of 5 use this macro?"). Provenance chains (origo) are
  source-maps for C: they exist so solarium can SHOW the expansion story.
  Provenance is a product feature, never debug metadata, never optional.
- **Wildcard conditional queries are the default.** "If you give it a file it
  searches the whole file" — including arms the compiler never took. ALL
  conditional arms retained forever (partitioned, not duplicated); the
  evaluated default track is just one configuration among many; named config
  presets + per-query atom assignments select others.
- **Byte-exact roundtrip is the ruthless oracle.** code→parse→emit == code,
  AND code→STML→load→emit == code. Trivia single-owner invariant; whitespace
  is style, comments are content. Byte-conservative everywhere: unknown bytes
  lex (OCTETUS_IGNOTUS), unmatched directives pass through, nothing is ever
  silently dropped.
- **Comments are content with structure.** They ride as trivia for roundtrip,
  but the vision (arbor-v2 brainstorm; arbor2-comment-spec.md) commits to
  comments as queryable entities: own-line / end-of-line / freestanding
  metadata, auto-attachment heuristics (end-of-line → preceding node; tight
  above → following node; blank-line-isolated → freestanding), extensible
  doc-comment patterns (/* ==== */ banners). The trivia representation must
  not foreclose the comment query layer. WATCH THIS ONE — it is the most
  easily smoothed-over pin.
- **Transformation, not just reading.** CSS-selector queries (Latin kebab-case
  tags derived from node genera, semantic pseudo-selectors like :calls(),
  $name captures) plus a pattern→template transform DSL with <%append> /
  <%prepend> / <%replace> mutation tags. The formatter (Fran's style,
  refining over time) rides on this. Trees must support clone/mutate/re-emit.
- **Robustness bar is tree-sitter.** Any bytes in → tree + diagnostics out,
  never crash. Snippets parse with API-injected context (macros/typedefs —
  this is speculum's entry point). Missing definitions → AMBIGUUS nodes with
  a canonical spine; when the oracle later learns the answer,
  re-canonicalize IN PLACE, no reparse. Editor-facing modes may retain
  failed alternatives.
- **Lint substrate.** Nodes tagged per standard (C89/C99/extension) so
  "find all C99 usages" is a query; Dependable C (dependablec.org) annotation
  pass is planned. The parser annotates; the lint layer reports.
- **Project scale.** Full project index, header graph (+ "you might need to
  include X" suggestions), per-file identifier tables for targeted reparse,
  unity-build / translation-unit composition ambitions. API offers BOTH tree
  mode and streaming/callback mode. Contexts are long-lived and reusable
  (two-arena memory: per-parse scratch + caller-owned results).
- **Nothing assumes analysis-only.** A runtime C compiler / REPL inside
  solarium is a live long-term possibility. No representation may be a
  dead end for execution.
- **The amalgamation IS the deliverable.** silva.h is vanilla C89 — the host
  must never see latina's defines (si/per/character stay internal).
  Prefix-renamed vendored libs (silva_piscina_*...), zero host collisions.
- **Node identity contract (simulation ⑤).** Node CONTENTS are always
  constructed through the checked ponere path (S32); node IDENTITY may be
  re-bound by the engine (transmutatio) until commit; after commit, trees are
  immutable except the sanctioned re-canonicalization API. Structural genera
  (ambiguus/error/conditionalis) are generator-REQUIRED — the engine's
  fabrica ambigui always exists; there is no degraded no-packing mode.
- **Why maximalism is law.** All three predecessors died from early
  data-model decisions that foreclosed features — s64-only reduction values,
  the Lexema*/Token* split that let provenance die at the parser boundary,
  a conflict-free grammar that left the fork machinery dormant. NOT from
  missing features. Every one of those decisions looked reasonable at the
  time. Therefore: when narrowing anything, name the use case being deferred
  and the path back, in the phase-log, so it stays a decision instead of
  becoming an assumption.

## CULTURA — how this team works

- **Complete first, optimize after — but measure from day one.** No
  performance budgets in 1.0, but the harness records parse time + peak
  arena bytes over the corpus so the cost curve is visible history.
- **Carry the tests.** Each generation must pass its predecessors' suites
  (arbor2 expander 134 + lexer 41 + lapifex integration 48 + roundtrip 62 =
  the compatibility bar; v1 praeparator too). Fresh implementations are
  welcome; fresh test bars are not. Old generations stay frozen as reference
  until parity, then engines are deleted — but each v1 DOWNSTREAM suite
  (quaestio/formator/typus/index — the only corpora for silva's post-1.0
  milestones) is deleted only after its bar is harvested at its milestone
  (spec-v2 §4.1). The shared roundtrip corpus (probationes/fixa/roundtrip/)
  is already silva's fidelity bar (probatio_silva_fidelitas).
- **One unit-tested module at a time**, composable, interface first (Eskil:
  APIs are expensive to refactor, implementations cheap). Walking skeleton
  before module polish — integration walls are found under load, not by
  finishing modules in isolation (lapifex M3 is the cautionary tale).
- **Price narrowings at the seam, not from memory.** "Defer to v2" /
  "syntactic for now" appearing in an INTENTIO before the exact
  integration point has been READ is a trigger, not a decision: the
  house builds callback seams (aux/config structs — canonicum,
  aestimator, parametrum_constans/accumulat, expressio_acies,
  resolutor, praecommissio) precisely where extensions hook, and
  memory-priced estimates systematically overestimate. Ten minutes
  at the seam has twice turned a planned compromise into the full
  feature (contractus 77 dataflow; chunk C's never-built callback
  seam). Complements the VISIO narrowing rule: before NAMING a
  deferred use case, check whether the substrate already dissolved
  the deferral.
- **Simulate before building — and mid-phase, against shipped code.**
  Pretend-implementations surface interaction bugs no per-feature review
  finds. Complexities are recorded in the four-part schema
  (discovered-while / consists-in / consequences / handled-by) and survive
  as files. Simulatio ⑤ proved the technique works mid-phase against real
  code: it REVERSED an escalation decision with evidence, and its design
  shipped with zero new complexities. When a design tradeoff is contested,
  simulate it before debating it.
- **Raw transcripts precede specs.** Brainstorm Q&A is preserved verbatim
  before any spec is written — decisions keep their reasoning attached.
- **Latin in the code** — identifiers, comments, messages, and the query
  surface (Latin-as-filter is philosophy, not decoration). English is fine
  in meta-documents (phase-log, worklogs, this file).
- **Fran understands all the code.** Explain as you build; he is the RAG
  with the whole system in his head. Teaching is part of the job, not
  overhead.
- **Cathedral time.** This is infrastructure intended to outlast its
  builders; "reasonable for now" is how the last three attempts died.
  When a decision feels obviously fine, check it against VISIO — the
  failure mode always feels reasonable from inside.

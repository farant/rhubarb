# officina M4a — EXAMEN (lamina exsecutionis) — spec

*2026-07-11. Gradus 4/4 of the M4a on-ramp. Inputs, read in order on
re-entry: officina-m4a-fundamenta.md (grounding, 13 facts) →
officina-simulatio-6.md (C1-C11, no reversal) →
officina-m4a-interrogatio.md (15 DECISUS, raw Q&A) → this spec.
Codebase-grounded via the fundamenta explorers; line cites there.
Never implement a chunk from a compaction summary alone.*

## I. Quid sit

**Examen** is the enforcement layer: C89 judgment BEFORE lowering.
Silva-side (M0 precedent — books in silva/phase-log.md, pointer
entries in officina/phase-log.md); its machinery lives inside
semantica (the single-home rule for C89 semantics); "examen" names
the layer's vocabulary, driver, and bars. Consumers: the M4b REPL
(judgment makes the R honest), silva-lsp's diagnostics lane, fresh-
code checking, and demissio's retirement of deferred-to-runtime
judgment.

Reframe from the explorers (hold onto this): semantica already
DETECTS (~44 diagnostic sites, ~40 messages) and already RECOVERS
(parser ERROR nodes + TYPUS_C89_ERROR poison-without-cascade). M4a
completes and productionizes: severity + positions + policy + the
compatibility relation + the missing analyses + the invalid corpus +
the differential bar.

## II. DECISUS (15, sealed at interrogatio)

1. Name: **examen**.
2. violatio tier = **C89-standard-strict** (K&R defs + implicit calls
   are NOT violations; they land in the house tier).
3. Tiers: **violatio / suspectum / domesticum**.
4. Lvalue word: **locus** (_est_locus; locus mutabilis).
5. Diagnosticum v2 = **{severitas, codex, positio materializata,
   socius}** added to {nodus, causa}; byte ranges deferred-named.
6. v0 scope = **core as simmed**: relation + three-way seat + locus +
   arity/return/controlling-scalar. **Post-interview fold (same day,
   Fran-approved): + redeclaration type-compat + operator-site
   pointer-target compat** — both are one-seat consumers of the
   chunk-B relation, both REPL-frequency-real, and the operator sites
   remove a differential-bar inconsistency (clang rejects them; an
   unwired examen would ACCIPE). Deferred-named: switch case-set,
   initializer validation, member legality, cast constraints,
   incomplete-object declarations, enum representability — the
   complete deferral picture lives in **project-specs/c89-lacunae.md**
   (the standing completeness ledger).
7. Posture: **always-on** in the analysis walk (M3 lineae precedent);
   sweep cost measured-and-recorded at seal.
8. Ambiguity: **provisional mark** on verdicts under retained-AMBIGUUS
   canonicals; harness/driver exclude provisional from hard verdicts.
9. Oracle string **SEALED** (§V) + **leak sweep** over C99/GNU
   extension groups before blessing the invalid corpus; clang version
   recorded in every run's output.
10. Secondary bar = **recorded measurement** (semantica-diagnosed
    sistere census delta at seal; no gate).
11. Invalid corpus: **silva/probationes/fixa/examinis/**, **EXSPECTA**
    markers, include-free, grep/sed extractor, bless-from-oracle.
12. Standing gate: **percursus gains a verdict column**; clang side of
    the differential runs at milestones/on-demand.
13. Driver: **censor pattern** — silva/instrumenta/principalia/
    examen.c + silva/examen.sh.
14. Machine lane: **-machina TSV** (via, linea, columna, severitas,
    codex, provisionale, causa).
15. Driver lexicon: **ISO default / -posix / -nudum**.

## III. Vocabulary (ceremony — do not coin casually)

- severitas: `EXAMEN_VIOLATIO` / `EXAMEN_SUSPECTUM` /
  `EXAMEN_DOMESTICUM` (public enum).
- codex: stable per-message id enum (`EXAMEN_CODEX_*`); the
  infrastructure class is `EXAMEN_CODEX_LEXICON_DEEST` (never a
  judgment; harness treats as infra).
- locus = lvalue; locus mutabilis = modifiable lvalue.
- provisionale = the retained-ambiguus bit on a diagnostic.
- The verdict of a TU: ACCIPE / REICE (derived: any non-provisional
  VIOLATIO ⇒ REICE).

## IV. Chunks (each independently sealable; Fran commits between)

### Chunk A — diagnosticum v2 (the struct and the plumbing)
- Extend SemanticaDiagnosticum: {nodus, causa} + {codex, severitas,
  positio materializata {via, linea, columna}, socius (one related
  node, may be NIHIL), provisionale}.
- Shared resolver `silva_c89_diagnosticum_positio` — the censor walk
  done once (token → silva_token_radix → linea:columna + via via
  silva_fons_via), with macro-expansion context available (origo
  chain). Compile-time diagnostics keep line:col (do NOT inherit the
  runtime line-only coarsening).
- Sweep all ~44 addere sites: each gains codex + severitas (call-site
  constants; the lexicon-gap sites get LEXICON_DEEST per sim C1).
- Fix the 5 NIHIL-node sites (node in scope one frame up, else
  fallback token).
- Fold the stale h:26-28 "initiatores non probantur" comment
  (completion shipped 2026-07-10; validation is §VII park).
- Amalgam: new public rows mirrored + hospes calls (standing rule);
  VERIFICATUM.
**Bar A: existing silva suite green; amalgam gates; a new probatio
covering resolver positions (incl. a macro-expansion case) and the
retrofitted fields on known-firing diagnostics.**

### Chunk B — relatio compatibilitatis (the pure function)
- `_typi_compatibiles(a, b)` per C89 6.1.2.6 over interned TypusC89*:
  identity fast path (interning); pointee recursion with
  qualifier SET-INCLUSION (target pointee quals ⊇ source's); array
  element + size sentinel (-1 = incomplete, compatible); function
  types: return compat + prototyped/K&R reconciliation
  (est_prototypata/est_variadica flags exist); tag/enum nominal by
  declarans (NO member-wise walk in v0 — sim C2: structural compat is
  a cross-TU rule; M4a judges one TU).
- The decision procedure `_assignatio_iudicium(finis, valoris, nodus)`
  → {LICET, LICET_CONVERSIO, MONE_* (suspectum/domesticum),
  VIOLATIO_*}: arithmetic↔arithmetic always LICET_CONVERSIO;
  null-pointer constant → any pointer; void* ↔ object pointer both
  directions; compatible-pointee; qualifier-discard = VIOLATIO;
  incompatible-pointee = VIOLATIO; int↔pointer = VIOLATIO;
  aggregate-identity = LICET (no conversion).
- TRAP pinned in probatio: char* vs const char* are SEPARATELY
  interned — the qualified pairs MUST go through the relation, never
  `==`. Qual discipline split (sim C3): relation owns POINTEE quals;
  locus owns TOP-LEVEL quals.
**Bar B: table-driven probatio (~40+ pairs) covering every branch
above incl. the qualifier traps, K&R reconciliation, array sentinel,
void*/null cases. Pure function — the most unit-testable module in
the layer.**

### Chunk C — sedes (wiring judgment into the walk, always-on)
- `_ad_finem_annotare` becomes three-way (sim C4): existing
  arith/pointer branches → aggregate-IDENTITY branch (LICET, no
  annotation — legal struct assignment must keep passing) → residue
  VIOLATIO. Monstrator branch SPLIT: legal subset (null constant,
  void* either way, compatible pointee) keeps annotating; illegal
  subset diagnoses + REFRAINS from annotating (retire the fabricated
  conversus; demissio reads absent = no conversion — verified).
- `_est_locus(nodus)` — compute-on-demand recursive form-walk (sim
  C6: NO storage, SemanticaTypatio untouched — it is demissio's
  frozen contract). Consumers: ASSIGNATIO left (locus mutabilis:
  locus ∧ ¬const-top-level ∧ ¬array ∧ complete ∧ ¬function), `&`
  operand (locus or function designator), ++/-- (locus mutabilis).
- Cheap table wins wired: call arity at VOCATIO (exemption set per
  sim C7: unprototyped, est_implicitum synthesis, variadic checks
  fixed-part minimum only); return void/non-void vs reditus_currens;
  controlling-expression scalar.
- Relation consumers beyond the seat (post-interview fold):
  (a) redeclaration type-compat at the symbol-registration site —
  today only genus mismatch fires ("redeclaratio generis diversi
  eodem scopo"); the relation upgrades it to full type
  compatibility, socius = the prior declaration; (b) pointer-target
  compatibility at operator sites — ==/!=/relationals, pointer
  subtraction, ?: branches (today class-only: two DIFFERENT object
  pointers pass silently). ~4 call sites, same relation.
- est_implicitum materialization: post-walk symbol sweep converts the
  flag into a SUSPECTUM diagnostic at declarans (the out-of-band
  channel joins the unified stream; fundamenta fact 6).
- Provisional gating: any diagnostic on a node under a retained-
  AMBIGUUS canonical gets provisionale=VERUM (extend the
  _intra_ambiguum machinery; no new mechanism).
- Recovery rule (one line): never diagnose a node whose type is
  already TYPUS_C89_ERROR.
**Bar C (three-sided): (1) fusor circuli BYTE-IDEM before/after —
proves the subtractive fix invisible on valid code (sim C5, zero new
machinery); (2) percursus sweep: diagnostics delta vs baseline
consists ONLY of new-judgment codices, and the violatio tier on the
real corpus is EMPTY once the 245-residuum mapping lands (map the
M0b worklog classification → tiers; any violatio survivor = a
severity mis-assignment or a REAL house bug — both are finds);
(3) sweep cost measured and recorded (freeze precedent).**

### Chunk D — effusor + vectis differentialis (the product surface)
- Driver: silva/instrumenta/principalia/examen.c + silva/examen.sh.
  `./silva/examen.sh <file.c> [-posix|-nudum] [-machina]` — parse,
  analyse (two-pass closure as percursus does), judge, print verdict
  (ACCIPE/REICE/INFRA) + diagnostics with via:linea:columna,
  severitas, codex, provisional dimming. -machina = TSV rows.
  ISO systema seed by default.
- Invalid corpus: silva/probationes/fixa/examinis/*.c — include-free;
  EXSPECTA markers, one-line rigid grammar in trailing comments:
  `/* EXSPECTA linea:codex */`; grep/sed extractor (never silva);
  pins blessed from oracle output. Seed from the explorer fixture
  SETS (recreate deliberately — 21 constraint + 11 C99 + 6 severity
  probes + the sim's aggregate/locus/arity cases).
- Oracle leak sweep FIRST (DECISUS 9): fixture sweep over C99/GNU
  extension warning groups hunting designator-style -pedantic-errors
  leaks; findings become oracle-string amendments before any blessing.
- Differential harness: examen side = percursus + verdict column
  (standing); clang side = the sealed oracle string forked per file
  (milestone/on-demand), version recorded, compare (verdict, linea,
  codex↔warning-group) — columns non-comparable across versions.
- System-header parity DODGED not solved (sim C10): invalid corpus is
  include-free; real-corpus side reuses the enumerated include set.
**Bar D = THE M4a BAR: (1) real corpus 100% ACCIPE (zero
non-provisional violatio) on the examen side AND clang-oracle exit 0
agreement; (2) invalid corpus: every fixture REICE with pins matched
at (linea, codex); (3) leak sweep completed before blessing;
(4) percursus verdict column standing; (5) deferred constraint
classes get EXCLUSIO rows in the harness (stdout-bar precedent) —
every place examen would ACCIPE what clang REICEs is a NAMED blind
spot with its cause and ledger pointer, never a silent one. Seal =
Fran's commit of the RELATIO.**

## V. The sealed oracle string

    clang -std=c89 -pedantic-errors \
          -Wno-long-long -Wno-overlength-strings \
          -Werror=c99-designator \
          -Wno-error=implicit-function-declaration \
          -Wno-strict-prototypes \
          -ferror-limit=0 -fno-caret-diagnostics -fsyntax-only

Exit 0 = ACCIPE, 1 = REICE. Amendable ONLY by leak-sweep findings
(recorded in the harness worklog) or a clang version change (version
pinned in every run's output; the accept/reject line MOVED at clang
15/16 — implicit-function-declaration default-error). Demotion
mechanics: default-errors demote with -Wno-error=X; pedantic-promoted
diagnostics need full -Wno-X.

## VI. Measure-first unknowns (record at the named chunk)

- Chunk C: always-on judgment cost on the percursus sweep (expect
  noise — pointer compares + shallow recursion over 1.5M nodes).
- Chunk C: how many of the 245 residuum map violatio (expect 0; any
  nonzero = finds, handle before bar C closes).
- Chunk D: forked clang wall time over the corpus (serial measured
  23-41ms/file → ~20s; confirm the few-seconds forked estimate).
- Chunk C: does enum already classify arithmetic in the relation's
  eyes (expected: yes via _est_arithmeticum — verify, else enum↔int
  compat is a relation case).

## VII. Parks (named, with owners/pull-conditions)

*The C89-completeness parks below are also rows in
**project-specs/c89-lacunae.md** — the standing whole-toolchain
completeness ledger. Update the ledger when a park opens or closes.*

- Switch case-set analysis (duplicate cases, default count) — pull:
  REPL user reports or corpus need; aestimator ready.
- Initializer validation (excess/shape/static-const) — pull: first
  real miss; completion already shipped, validation is distinct.
- Struct member legality (bitfield widths…) — pull: bitfield unpark.
- Cast constraints (both sides scalar, 6.3.4) — decidable from
  tables; casts keep the "trust me" posture in v0.
- Object-of-incomplete-type declarations (`struct S x;` never
  completed) — access side already enforced; declaration side isn't.
- Enum int-representability + rare corners — with the ledger.
- Byte ranges + extent-API promotion — pull: silva-lsp
  publishDiagnostics (the named consumer).
- Cross-TU composite/structural compatibility — M4b/link-time
  question, not a TU judgment.
- -domus driver mode (fail on domesticum for house code) — pull:
  first daily-driver use as a pre-commit check.
- REPL/M4b inheritance: judgment-before-lowering + vindex_onerator +
  the incremental world (implicit-then-real declaration
  reconciliation across turns — named in fundamenta fact 6).
- Sibling-leak fixture sweep results feed the oracle worklog even
  when clean (record the swept group list).

## VIII. Books discipline

Silva-side books: INTENTIO/RELATIO per chunk in silva/phase-log.md;
officina/phase-log.md gets pointer entries at milestone boundaries
(M0 precedent). Worklogs at the moment of finds. Instrument debrief
in every RELATIO (adhibita/fructus/asperitates/desiderata). At seal:
full phase-log re-read + audit; spec-v2 §XI restated with actuals;
memory updated. Do named folds IN CHUNK (M3 audit lesson).

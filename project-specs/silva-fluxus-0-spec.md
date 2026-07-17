# Fluxus-0 spec — CFG + attingibilitas, codices 63–67 (2026-07-17)

**THE RE-ENTRY DOCUMENT for the fluxus arc.** Authorities, in
order: this spec → simulatio XI (silva-simulatio-11.md, the
codebase contact + register) → the FLUXUS-0 INTENTIO (silva/
phase-log.md tail) → interview record (silva-fluxus-0-interview.md).
Never implement from a compaction summary; start here.

## Quid

`silva_c89_fluxus.{h,c}`: statement-granularity control-flow graph
per function definition, built on the silva tree, silva-side
(dependency law: officina consumes silva, never the reverse). Five
examen codices consume it: 63 SEMITA_SINE_REDDITU (SUSPECTUM,
-Wreturn-type parity), 64 CASUS_LAPSUS (DOMESTICUM, TOLERA-bilis),
65 SENTENTIA_INATTINGIBILIS (DOMESTICUM, stretch), 66
FRANGE_EXTRA_CONTEXTUM + 67 SALTA_AD_TITULUM_IGNOTUM (VIOLATIO,
verdict-flipping — closes a verified pre-existing parity hole:
clang errors, examen said ACCIPE). Codex 62 stays DORMANT (re-arm
is stage 2 def-use, not this arc).

## Oracle pins (calibrated live, Apple clang 16, specimens in sim)

- House flags blind to fall-through + unreachable; -Wreturn-type in
  -Wall (parity; corpus census MUST be 0 — it compiles -Werror).
- -Wreturn-type FOLDS constants: while(1), for(;;), while(2-1),
  goto-self-loop all silent; while(x) fires. Folding is
  parity-REQUIRED ⇒ aestimator seam.
- -Wimplicit-fallthrough: fires at RECEIVING label, path-based
  (`si (x) frange;` end reachable ⇒ fires), previous group
  non-empty; label stacking silent; comments never suppress.
- -Wunreachable-code: post-jump fires; return-after-infinite-loop
  only under -aggressive (DEFERRED by name).
- frange-outside-context / salta-to-unknown-label: clang ERRORS.

## Module architecture

- **Dependencies**: nodus, tabulae_c89, piscina, xar. NOT semantica.
- **Auxilia seam** (XI-1): builder takes
  `FluxusAuxilia { canonicum fn, aestimator fn, contextus }` —
  canonicalization is caller POLICY (_canonicum's own comment),
  folding is semantica's _constans_probare via thin wrapper. NIHIL
  members = identity / nothing-constant. Unit tests: toy literal
  evaluator (~10 lines).
- **Storage** (XI-5): FluxusBlocus BY VALUE in Xar (stable element
  pointers — xar is segmented); FluxusMargo by value in per-block
  Xar; edges hold FluxusBlocus*; caller's piscina; zero per-block
  mallocs. Never swap-remove/sort these xars.
- **Structs**: FluxusFunctionis { definitio, bloci, introitus,
  exitus (synthetic), … }; FluxusBlocus { index, sententiae (Xar of
  constans SilvaNodus*), margines, attingibilis, titulus_casus
  (heading casus/ordinarius/titulatum node or NIHIL — tier firing
  position) }; FluxusMargo { genus, destinatio, origo nodus }.
- **Edge genera**: SEQUENS, VERUS, FALSUS, CASUS, ORDINARIUS,
  RUPTURA, PERSECUTIO, SALTUS, REDDITUS, CADIT (fall-off-end →
  exitus), LAPSUS (group-boundary sequential edge, tagged AT
  CONSTRUCTION — codex 64 is a filter, not a search).
  (Uppercase ≠ latina macros: VERUS/FALSUS fine, never bare
  VERUM/FALSUM as identifiers.)
- **API stance**: internal v1 — amalgam manifest rows + semantica
  include; silva.h gets ONLY codex enum rows. Path back: solarium
  query surface exports a wrapped view later.

## Builder walk (per statement genus; canonicalize at every dispatch)

- Blocks are flow-runs, not scopes; nested corpus recurses into the
  SAME block. Two passes: ① collect titulatum labels (function-
  scoped, chorda mensura+memcmp), pre-create their blocks; ② walk.
- si: close block, VERUS→consequens, FALSUS→alioquin-or-join;
  constant conditions fold to single edge (uniform with loops).
- dum: head block; VERUS→body, FALSUS→after (OMITTED when constant
  true); body-end→head; ruptura→after, persecutio→head.
- fac_dum: body first; body-end→conditio block; VERUS→body,
  FALSUS→after; persecutio→CONDITIO (C semantics), ruptura→after.
- per: initium in current; conditio head (ABSENT = constant-true);
  corpus; passus block; body-end→passus→head; persecutio→PASSUS,
  ruptura→after.
- commutatio: dispatch block; FLAT scan of corpus lista (Duff
  pre-rejected by grammar — XI-2): pre-label elementa open a block
  with NO inbound edge (naturally unreachable, matches clang); each
  casus/ordinarius group opens a label-headed block with
  CASUS/ORDINARIUS edge from dispatch; open previous-group end ⇒
  LAPSUS edge. NO ordinarius ⇒ dispatch→after edge (no-match path —
  REQUIRED for 63 parity). Stacking = sibling groups, first
  sententiae [0] (verified live; genera-c89.md correct, GLR worklog
  note stale).
- titulatum: SEQUENS into pre-created block, which becomes current.
- salta: SALTUS edge to label block (unknown label ⇒ codex 67 at
  the semantica layer; builder drops edge). Close block.
- frange/perge: RUPTURA/PERSECUTIO to context-stack target (one
  stack of Frangibile; commutatio pushes ruptura-only, perge
  searches past it). Empty stack ⇒ codex 66 (semantica layer),
  drop edge, never crash. Close block.
- redde: REDDITUS→exitus. sententia_vacua: append. declaratio:
  append. CONDITIONALIS: descend RAMUS_SUMPTUS transparently (taken
  track pin; tree keeps all arms, CFG is a view). AMBIGUUS:
  canonicum callback, still-ambiguous ⇒ opaque append. ERROR:
  opaque append, flow passes through (conservative).
- Function end: open block → CADIT edge → exitus. Attingibilitas =
  one DFS from introitus.

## Codex specs

- **63 SEMITA_SINE_REDDITU (SUSPECTUM)**: reditus_currens non-void
  && non-error && exists CADIT edge with attingibilis source. Fires
  at the definitio node. Gets right, with zero special cases: goto
  self-loops, dum(VERUM)-only-exit functions, every-arm-returns
  classifier switches (dominant corpus idiom — XI-6). noreturn gap:
  MEASURE FIRST via differentia (decision Q1); options named in
  XI-3, no design until the count exists.
- **64 CASUS_LAPSUS (DOMESTICUM, TOLERA-bilis)**: LAPSUS edge with
  attingibilis source && previous group sententiae non-empty ⇒ fire
  at receiving label. TOLERA-only suppression (decision Q2).
  Expected census: ONE (arbor_syntaxis.c:3491→3504, guarded frange,
  frozen) — stands as standing census.
- **65 SENTENTIA_INATTINGIBILIS (DOMESTICUM, stretch)**:
  !attingibilis && non-empty block ⇒ fire at first statement. Plain
  -Wunreachable-code semantics only.
- **66 FRANGE_EXTRA_CONTEXTUM / 67 SALTA_AD_TITULUM_IGNOTUM
  (VIOLATIO, verdict-flipping)**: parity contract — clang errors.
  Covers perge too (66).

## Integration (chunk 2)

Build in _definitionem_ambulare AFTER _corpus_ambulare (aestimator
has typations). sem grows `Xar* fluxus_functionum` — retain all
(decision Q7). Tiers are post-build passes where diagnostics
already accumulate. Differentia oracle invocation gains
-Wimplicit-fallthrough / -Wunreachable-code for 64/65 buckets.

## Chunk plan (each ends green: suites + vectis + amalgamare)

1. fluxus.{h,c} + probatio_silva_fluxus (toy auxilia; assert block
   counts / edge genera / attingibilitas / cadit over the sim's
   battery incl. the two named specimens: silva_lexema.c
   _aspicere_eff, tabula_characterum.c:903).
2. Semantica integration + 63 + 66 + 67 + THE DIFFERENTIA SWEEP
   (the CFG's acceptance test; noreturn count falls out).
3. 64 + census + adjudication (expect one frozen site).
4. (stretch) 65.

## Deferred by name (paths back)

- noreturn knowledge — until differentia count (house-list option
  shaped in XI-3).
- -Wunreachable-code-return variant — same CFG, extra query.
- Expression-level subdivision — MC/DC horizon + codex 62 stage 2;
  block API must not foreclose (blocks reference expression nodes
  intact).
- Conditional-arm parameterization — when config-preset queries
  land.
- Duff admission — grammar work; change-seat = fluxus group scan
  (nota 01KXRF67RF…).
- Public export — solarium query surface era.
- Comment-idiom suppression — REJECTED (decision Q2), not deferred.

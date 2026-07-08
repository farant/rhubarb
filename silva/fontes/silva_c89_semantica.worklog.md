# silva_c89_semantica worklog

## 2026-07-08 — Chunk A ships (typus + forma + aestimator)

Notes that don't belong in the header comment:

- **long long accepted as extension, deliberately.** C89 has no long
  long, but the corpus REQUIRES it (latina.h s64 = signed long long,
  ~624 uses, -Wno-long-long repo-wide). PRIMITIVUM_LONGUS_LONGUS[_
  INSIGNATUM] exist; the multiset mapper takes longi==2. When
  standard-tagging lands (recensio #5), these should carry EXTENSIO.
- **long double = 8 on Apple arm64** (AAPCS64 Darwin: long double is
  double). Linux arm64 would be 16 (quad) — if silva ever runs
  elsewhere, the forma table is the port point. Haruspex will verify.
- **sizeof(expr) in constant expressions = named park (M0b).** Needs
  the expression's TYPE, which is M0b's expression typing. Corpus
  array sizes are literals post-expansion (Roman numeral macros), so
  zero corpus load. Diagnostic fires if hit.
- **Interning is a linear scan** (sem->derivati, structural compare
  at creation). Complete-first rule; per-file type counts are small.
  If percursus -semantica shows cost, switch to a hashed key. The
  CONTRACT (pointer equality = identity) is unaffected.
- **Flat-table tag redefinition (Chunk A limitation):** two same-name
  structs in different scopes both register in sem->tags; second
  definition creates a NEW type and re-registers (last wins). Correct
  per-scope resolution arrives with scopus (Chunk B). Legal C that
  hits this: rare in corpus (block-scope struct defs are the
  const-strip unions, all ANONYMOUS — untagged, never registered).
- **Enum tags share the tag namespace with struct/union tags** (one
  C89 tag namespace) — deliberately one sem->tags table.
- **\x and \NNN character escapes = FALSUM (park).** Corpus uses
  none in constant-expression positions.
- Probatio landmine that bit: hand-counted fixture lengths. Use
  strlen. (The trailing NUL parsed as OCTETUS_IGNOTUS -> ERROR node
  — which is silva behaving exactly as designed.)

## 2026-07-08 — Chunk B ships (scopus + ambulatio integra)

- **B bar AMENDED honestly**: INTENTIO said "diagnostics 0" over the
  corpus; the sweep gives 818 in 524 files — ALL of class "typus
  nominatus ignotus" (probe-verified on filum.c/piscina.c): plain
  `size_t n;` declarations commit as DECLARATIO with an
  oracle-unknown type name (only cast/sizeof positions fork into
  ambigui — census). This is PRECISELY the systema gap; Chunk C's
  bar inherits "semantica diagnostica -> ~0". Amendment named here
  and in the phase-log.
- Corpus sweep: 729 files, zero crashes, 234,427 symbola,
  semantica cost invisible (~108s total vs ~112s baseline run).
- Scope model: params scope (prof 1) + body corpus scope (prof 2)
  nested — redeclaring a param in the body block is legal-shadowing
  by this model (lint's call, not ours).
- Same-scope redeclaration: same genus = lenient overwrite
  (tentative definitions); different genus = diagnostic + overwrite.
- Labels namespace: not implemented (nothing consumes it; lint owns
  goto/label legality) — park named.
- Block-scope struct tag redefinition now CORRECT via scoped tag
  tables (Chunk A's flat-table limitation closed).

## 2026-07-08 — Chunk C ships (systema + oraculum trivalens + clausura)

- **THE NUMBERS**: ambigui at commit 15,261 -> 3,401 (preload +
  trivalent precommit registration); post-closure residuum 411
  indecisa = 2.7% of the original mass (97.3% resolved). Semantica
  diagnostica 818 -> 110 in 26 files.
- **Residuum classified** (the bar's clause):
  (a) nested cast/paren CHAINS incl. GLR fork-as-reading packing —
      delineare.c 34 (s32 — known type!), scrutinium family ~85
      (values/i/magnitudo — known non-types!), elementa,
      pinacotheca_visus, nuntium... The already-named nested-cast
      resolver park, now BETTER CHARACTERIZED: per-fork examination
      defeats itself on packed fork-chains where forks appear AS
      readings of other forks. Minimal repro = the probatio's
      "catenae nidificatae" fixture (SCR_ELEM macro shape from
      scrutinium.h). Landing spot: post-M0a resolver look (chain-
      aware joint resolution, or resolve-inner-first fixpoint).
  (b) POSIX names (close, socket, ioctl, readdir families) —
      reactor 18, tcp_posix 11, iter_directoria, tessera_pons.
      Systema is ISO C89 by DECISUS; a systema_posix.h supplement
      is a named on-pull item (officina Wave-1b/3 timing).
  (c) diagnostics-only classes: ~55 roundtrip FIXTURE files with
      DELIBERATELY unknown types (test_two_unknown_ret.c...);
      ~15 praebenda basename-collision victims (silva_coquere's
      SilvaGen* — percursus 5-collision first-wins limitation, a
      harness item, not semantica).
- **Contagion bug found+fixed during classification**: the trivalent
  kill leaked through NESTED ambigui (outer reading absorbed
  non_typus from an inner ALTERNATIVE it doesn't choose -> both
  outer readings killed -> wrongful retention). Fix: _nodum_examinare
  special-cases AMBIGUUS - nominatus/ignotus still merge as ANY
  (old semantics), non_typus merges only if ALL inner readings carry
  it (fork wholly impossible). Proven by the minimal repro: 2 of 3
  forks rescued; the third is the deep-chain park.
- **decisum != versum**: recanonicare counts FLIPS only; resolutions
  agreeing with the canonical guess don't flip. The closure metric
  is silva_c89_ambigua_indecisa_numerare (sweep: 1,939 flips + 1,051
  confirmations + 411 indecisa of 3,401).
- **Stale-object trap, TWICE**: two build systems write different
  object names into silva/build (suite: <nomen>.o; percursus.sh:
  fons_<nomen>.o). Probes linked fons_* objects that predated fontes
  edits — a fix "didn't work" because it was never in the binary.
  When probing with hand-linked binaries after fontes edits: rm the
  relevant silva/build objects first, or rebuild via BOTH scripts.

## 2026-07-08 — M0b Chunk A ships (typatio: tabula + descensus + primaria)

- **Numbers**: suite 206 asserts (was 146); sweep 731 plagulae,
  **899,518 typationes, 0 ruinae**; walls unmoved (errores 0,
  fideles 730/731, ambigui 3,411, clausura 1,939/414 — the lexicon
  attach perturbed NOTHING on the parse side).
- **THE FIND — libc value-macros are invisible, and expression
  typing is what exposes them.** First sweep: diagnostics 110 →
  18,293 in 381 files. Cause: `NIHIL` expands to `NULL`, and NULL
  is a <stddef.h> macro silva never sees — it lexes as a plain
  IDENTIFIER, resolves against nothing, and fires "identificator
  ignotus" in nearly every guard expression in the monorepo (EOF,
  INT_MAX, SEEK_* likewise). M0a never saw this because
  DECLARATIONS never contain NULL — only expressions do. This is
  the dark twin of simulatio-2's "pleasant surprise": implicit
  extern-int self-heals unknown CALLEES, but value MACROS don't
  self-heal.
- **Fix — the lexicon channel earns its keep**: systema_c89.h
  already carried the macro surface (NULL/EOF/limits/SEEK_*); it
  reached only the SYSTEMA parse, not user parses. percursus now
  attaches systema as a LEXICON to the context under -semantica
  (the channel that discards everything but #defines — empirically
  "inert for typedefs" in M0a, which is exactly the property we
  want here). Diagnostics 18,293 → **1,026 in 52 plagulis** (-94%).
  Baseline (flags off) stays pristine — attach is -semantica-only.
- **THE SECOND FIND — a REAL LATENT BUG in latina.h, caught by
  typing on day one.** Post-lexicon residue (1,026) was dominated
  by `pl`/`f`/`FILUM` unknowns. Root cause: latina.h:402 read
  `#define FILE FILUM` — REVERSED vs every other rename (imprimere
  printf, liberare free). It only ever compiled by accident: with
  latina included before <stdio.h>, the macro rewrites stdio's OWN
  `typedef ... FILE;` into defining FILUM — so the monorepo's file
  type was literally NAMED FILUM inside the invisible system
  header, and include order was secretly load-bearing. Silva could
  never see that typedef → `FILUM* f;` retained decl/expr forks
  with canonical = MULTIPLICATION → typing walked `FILUM * f` as
  an expression → both identifiers "ignoti". Fran confirmed bug;
  fix = flip to `#define FILUM FILE` (order-independent, matches
  the house pattern). Root suite 93/94 after flip (the 1 = the
  known flaky probatio_tcp), silva suite 29/29, VERIFICATUM.
- **The flip improved M0a's closure numbers RETROACTIVELY**:
  ambigui retained 3,411 → 3,354, indecisa 414 → **355** — ~59 of
  what M0a classified as nested-chain-park residue were actually
  FILUM-bug forks. The park is smaller than its ledger said.
- Post-flip sweep: diagnostica **245 in 28 plagulis**; typationes
  900,320; symbola +203 (the f/pl file-handle variables now
  register). Residuum classified (diagnosticator cause census):
  (a) **POSIX (~120)** — tcp_posix 36, tessera_pons 25+25,
      iter_directoria/reactor/imago/filum; lexemes AF_INET,
      SOL_SOCKET, TCSAFLUSH, SIGTSTP, EINTR, DT_DIR, DIR, ssize_t,
      S_I*USR. The named systema_posix on-pull park — NOW KNOWN to
      need value-MACROS via the lexicon channel as well as
      prototypes via channel B (same two-channel shape as ISO).
      Includes the silva instrumenta's own dirent/mkdir uses
      (percursus/haruspex/infidelis, 13 total).
  (b) **latina-less standalone files (~112)** — roundtrip fixtures
      (62: deliberately-unknown types Color/MyType/i8-without-
      latina/redde-as-type), genera_biblia 20, knotapel demo_107
      17, silva probatio fixture content 12. Deliberate or
      out-of-world; existing M0a class extended to expressions.
  (c) **small tail (5)** — typedef-in-expression 4 (re-examine at
      Chunk D's post-closure re-analysis; suspected C1 canonical
      artifacts) + 1 mensura-non-constans.
- Amalgam gates caught two expected consequences: (1)
  tabula_dispersa_numerus had to leave the amalgamator EXCLUDENDA
  (first real caller — "the compile clamat" exactly as documented);
  (2) hospes's semantica fixture used an UNDECLARED x in
  `(foo)(x);` — M0b typing now correctly diagnoses it; fixture
  gained `static int x;` (the ==0 diagnostics assert stays
  meaningful).
- Instrument note: scratchpad "diagnosticator" (percursus copy +
  per-diagnosticum causa/lexeme/line printing) did the
  classification; throwaway, not committed. If cause-level
  diagnostics are wanted permanently, that's a percursus flag on
  pull.
- **LP64 limits correction**: systema's LONG_MIN/LONG_MAX/ULONG_MAX
  carried ILP32 values (long=32bit) — wrong for arm64 LP64. Types
  were always right (suffixes), values were not. Fixed.
- **Table simplification vs INTENTIO**: no custom friatio/comparatio
  needed — the default chorda FNV-1a hashes mensura bytes (binary-
  safe, chorda is not NUL-terminated by design), so pointer keys
  are just 8 pointer bytes wrapped in a chorda via the ordinary
  creare_chorda. Key bytes live IN the entry (SemanticaTypatio
  .clavis_octeti) — no cast-qual contortions.
- **Behavior change (aestimator)**: _folium_character_aestimare
  now decodes through the shared _fugam_decodere — \x/\NNN
  UNPARKED (both char literals and string lengths); multi-char
  literals ('ab') now FALSUM (previously returned first char —
  impl-defined anyway); L'...'/L"..." = named-park diagnostic.
- Integer literals beyond s64 wrap in _folium_integer_legere
  (ULONG_MAX's 18446744073709551615UL types correctly via suffix;
  the VALUE wraps to -1 — harmless for typing, aestimator caveat).
- Typing dispatcher policy: B/C genera (binarium/vocatio/accessus/
  congeries...) DESCEND into children and return NIHIL silently —
  primaries inside operators get typed in A, the operators
  themselves in B/C. vocatio's functio locus deliberately skipped
  (C89 implicit-int callee rule = Chunk C).

## 2026-07-08 — Chunk D ships (haruspex + index + publica)

- **HARUSPEX VERDICT: 177 TU, 4,801 assertiones, 0 dissentientes.**
  clang consents to every sizeof/alignment/offsetof/enum-value
  semantica computed across include/, lib/, silva/tessera/
  saltuarius fontes. Design: each TU #includes the ORIGINAL file
  (textual inclusion works for .c too!) + appended compile-time
  asserts; clang -fsyntax-only = the verdict; .c-internal static
  types verified in-TU. Alignment asserted via the classic
  { char c; T t; } offsetof trick (C89 has no alignof).
- Provenance partition holds: systema types skipped by transitive
  by-value dependency check (corpus fact: 0 types embed systema
  types by value — praetermissa_systemate 0 in the full run, 1
  only when haruspex accidentally swept systema itself, now
  excluded: systema is silva-parsed, never clang-compiled).
- 34 praetermissa = function/incomplete typedefs (legitimately
  formless). Enum CONSTANT VALUES asserted too — the aestimator is
  clang-verified, not just the layout.
- TU naming traps found live: binary-vs-directory name collision
  (build/haruspex -> exta/ — the entrails, naturally); quoted
  #include resolves TU-relative (-I repo root); TU files must end
  .c regardless of source extension.
- Publica: manifest + CADENDA (TypusC89 full-def owned by silva.h;
  SilvaSemantica opaque, SemanticaScopus fully internal) + silva.h
  M0a section (vanilla; i32=unsigned int EXACTLY or the amalgam TU
  sees conflicting declarations) + hospes semantica block: all 11
  new public functions CALLED with content asserts incl. the
  closure fixture through the public API. VERIFICATUM first try,
  hospes 28/28.

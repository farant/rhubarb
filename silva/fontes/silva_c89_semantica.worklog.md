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

## 2026-07-08 — M0b Chunk B ships (UAC + operatores + auspex)

- **AUSPEX VERDICT: 2,147 expressiones, 3,753 assertiones, 0
  dissentientes.** Every primitive pair × every operator, size AND
  signedness, clang-certified — the same expression text flows
  through OUR pipeline (parse → typare) and through clang
  (compile-time asserts: sizeof in array bound + the signedness
  trick `(expr)*0 - 1 > 0`, an integral constant expression). TU
  compiled as C99 deliberately: the long-long C99-rank choice
  (DECISUS 4) is itself what gets certified. Gaps recorded: double
  vs long double (identical on Darwin), pointer-expression
  signedness (not constant expressions — sizes only), ternary
  composite pointer TYPES (sizes equal — probatio fixtures carry
  those instead).
- Suite 239 asserts (was 206). Sweep: **1,086,949 typationes**
  (+186k = the operators), diagnostica 265 in 30 plagulis.
  **Delta vs Chunk A fully classified: ZERO operator-class false
  positives over a million typed expressions.** +19 = typus
  nominatus ignotus for POSIX types in CAST position — a new
  sighting path (conversio typing calls typus_ex_specie now), same
  systema_posix park. +1 = a REAL BUG (below).
- **INTENTIO amendment (named): conversio (cast) typing pulled
  into B** — the chunk map had assigned it to neither B nor C;
  auspex's cast-literal probes required it, and it belongs with
  conversions anyway (it IS the conversion operator). Typus =
  species sine qualibus; internum's conversus = the cast target.
- **THIRD real bug found by typing — uuid.c:44 `(insignatus i8)`**:
  expands to `unsigned i8`, illegal (i8 is already a typedef of
  unsigned char). Never caught because it sits in the #else (Linux)
  arm and Apple compiles the __APPLE__ arm. Silva does NOT define
  __APPLE__, takes the #else arm, and type-checked code that no
  compiler on this machine has ever compiled. Fixed: `(i8)`.
  NOTE for config-query milestone: silva's taken-arm evaluation
  models a platform-neutral config — here a feature (free checking
  of the arm Apple never sees), but multi-config semantics remain
  that milestone's problem.
- UAC engine: C99 integer conversion ranks; LP64 representability
  = strict-size-dominance (long swallows unsigned int; long long
  does NOT swallow unsigned long → unsigned long long). Pinned:
  ul+ll → ull, i+l → long w/ operand conversus, float ladder
  (fp+i → float per C89), shifts = promoted LEFT (non-UAC),
  ptr−ptr → long (systema ptrdiff_t), null-constant → pointer
  (incl. `(void*)0` form), composite void* with qual merging,
  compound assignment implied-op conversions, return conversion
  via sem->reditus_currens (save/restore around bodies).
- Operand-class diagnostics are deliberately narrow (wrong CLASS
  only — poison absorbs silently, deep compatibility = lint).
  The zero-false-positive sweep validates the narrowness.

## 2026-07-08 — M0b Chunk C ships (postfixa + exparcata + est_implicitum)

- **COVERAGE DEBUT: 99.61%** (1,497,967 / 1,503,793 expression
  nodes typed, measured POST-closure against the final tree —
  deliberately, so the 5,826-node gap IS the canonical-relative
  flip class that Chunk D's two-pass closes; percursus now prints
  per-file `[expr coopertura N/M]` + the summary %).
- Suite 275 asserts; typationes 1,610,983 (+524k); VERIFICATUM.
- Shipped: vocatio (reditus; prototyped arg conversions via
  _ad_finem_annotare; variadic-extra + unprototyped default
  promotions incl. float→double and decay; callee designator decay
  annotated; arity NOT diagnosed — enforcement layer's); subscriptio
  (incl. commutative 1[arr]); accessus (one genus, ./-> via
  tok_operator; member = bare-token chorda compare over membra;
  BASIS QUALS PROPAGATE to member type — const struct → const
  member, pinned; incomplete tag = poison + diagnostic); magnitudo
  pair typed as size_t (unsigned long, systema).
- **est_implicitum shipped (INTENTIO amendment via the
  enforcement-layer question)**: unknown DIRECT-identifier callees
  synthesize `extern int f()` (non-prototyped) registered in the
  CURRENT scope with est_implicitum=VERUM and declarans = THE CALL
  SITE (folium-identificator, not a declaratio) — the datum that
  would be unrecoverable post-hoc. Field mirrored into silva.h's
  SemanticaSymbolum (vanilla int) + hospes reads it. Pinned:
  implicit vs declared, call-site declarans, non-prototypata type.
- **Retroactive improvement #2: indecisa 355 → 305** — implicit
  callees flow into the oracle as non-types via augere, killing
  more declaration readings. The nested-chain park ledger shrinks
  again without touching the resolver.
- **magnitudo-expressionis EXPARCATUM**: aestimator computes
  sizeof(expr) through the typing engine (aestimator↔forma↔typatio
  three-way recursion); sizeof(arr) = 16 pinned (UNdecayed by
  construction of naturalis).
- **Congeries context typing (DECISUS 6)**: type flows DOWN from
  the declarator; direct correspondence typed (nested congeries
  recurse; struct members positional; union first member; scalar
  braces legal); element conversions annotated ('a'→char member
  pinned); chorda-into-char-array recognized as legal.
  **THE ELISION MEASUREMENT: ZERO instances in the whole corpus**
  — generated tables are fully braced, as predicted. The M1-parked
  full elision walk costs nothing today.
- **Noise class found + silenced precisely**: typing the callee
  position unlocked "typedef in expressione/vocatum" × 2,155 —
  ALL inside retained-AMBIGUUS canonicals (the nested-chain park
  files: arbor2_glr_tabula 1,916, delineare 34, scrutinium
  family...). Real code cannot contain typedef-as-expression (it
  wouldn't compile); inside a retained fork it's pure canonical
  noise owned by closure/park machinery. Fix: _intra_ambiguum
  (pater chain, fixed at commit) guards ONLY the two typedef
  diagnostics — poison still applied, coverage unaffected,
  "identificator ignotus" left ungated (it found three real bugs).
  Post-guard arithmetic closes exactly: 265 − 4 + 151 = 412.
- Diagnostics 412 in 32 plagulis, all classified: ~154 ignotus
  (POSIX macros/fixtures) + 105 typus nominatus (POSIX types) +
  151 accessus structurae incompletae (struct stat/sockaddr/
  termios... — the systema_posix park's third face: types,
  macros, AND tag layouts) + small tail.

## 2026-07-08 — M0b Chunk D ships (bis-analysis + publica) — M0b CLOSED

- **COOPERTURA 100.00% (1,503,915/1,503,915).** The two-pass
  pipeline (sem₁ feeds closure, annotations discarded →
  recanonicare → sem₂ authoritative) closed the 5,826-node
  canonical-relative gap to zero. Second analysis cost: invisible
  (113s sweep, unchanged). Diagnostics 412 → 399 — the C-RELATIO's
  "re-examine at D" tail (typedef-in-expression C1 artifacts)
  evaporated with the flips, exactly as predicted.
- Publica: three silva.h rows (typus/conversio_expressionis,
  typationes_numerus — vanilla const-correct; canonical-relative
  contract stated in the section comment) + hospes content asserts
  through the public surface. VERIFICATUM 28/28.
- Officina spec-v2 §XII.8 RESOLVED: §VI rewritten (trivalent,
  M0a/M0b both shipped w/ certified numbers), §XI M0 bar restated,
  §XII.8 struck through with M1 named next.
- **M0b milestone summary**: 4 chunks, one day; probatio 146 → 275
  asserts; 1.6M typationes; auspex 3,753/0; THREE real bugs found
  (latina FILE/FILUM reversal, uuid insignatus-i8, value-macro
  invisibility); TWO retroactive M0a improvements (indecisa 414 →
  305 — a quarter of the nested-chain ledger was latent bugs and
  missing knowledge, not resolver limitation); elision measured
  ZERO. Demissio's input contract FROZEN.

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

## 2026-07-09 — M1a Chunk A (officina export package)

- **Nexus symbolorum**: use-site→symbol parallel table, the exact
  SemanticaTypatio pattern (pointer-bytes key embedded in the entry,
  creare_chorda FNV). Recorded at the ONLY two resolution sites:
  FOLIUM_IDENTIFICATOR expression case + the vocatio callee path
  (which bypasses the identifier case and does its own invenire —
  implicit synthesized callees get bound there too, so
  symbolum_nodi(callee)->est_implicitum works). Public rows
  silva_c89_symbolum_nodi (canonical-aware, canonical-RELATIVE like
  typatio) + nexus_numerus.
- **chorda_decodere**: public escape-decoding of folium-chorda into
  a caller piscina (upper-bound alloc = source length; single pass;
  fragments concatenated; embedded NUL fine — chorda is binary).
  Reuses _fugam_decodere via same-TU placement. NO trailing NUL by
  design: acies.numerus = octeti+1 already carries it; demissio
  zero-fills the datum image from the type.
- **FINDING — the L park is PRE-PARSER for strings**: `int* w =
  L"ab";` does not parse (numerus_errorum 1). The defensive
  datum[0]=='L' checks in _chordae_litteralis_octeti/_folium_
  character_aestimare are unreachable for STRING literals today
  (kept — cheap armor if the lexer learns L later). Probatio uses
  \q invalid-escape as the reachable decodere-FALSUM path instead.
- constans_aestimare + responsa_vacare needed NO fontes changes —
  both existed with prototypes; the gap was amalgam silva.h rows
  only (audit items (b)+(d) were cheaper than the sim priced).
- Gates: suite 308/308 (+22), silva 29/29, amalgam VERIFICATUM
  hospes 28/28 (all 5 rows called with content asserts), percursus
  UNCHANGED baselines (0 errores; coverage 100.00%; 399 diagnostica
  in 31 plagulis; 305 indecisa) — nexus recording is invisible to
  every existing gate.

## 2026-07-10 — COMPLETIO ACIEI PER INITIATOREM (C89 3.5.7)

Pulled by officina M2c: the "magnitudo inaestimabilis" sistere
class (arbor2_glr/scribere/compound/utf8) + capsula's wrong-exit
("initiator staticus inaestimabilis" — capsula_assets.mensura =
sizeof of a completed array → 0 → aperire NIHIL) + flatura's
"forma elementi ignota" ALL traced to one unimplemented clause:
"at the end of its initializer list, the array no longer has
incomplete type." Semantica typed symbols from declarator alone;
congeries typing flowed types DOWN but never the count UP.

Fix at the single declaration chokepoint
(silva_c89_declarationem_tractare, both file- and block-scope
route through it): declared type = incomplete ACIES + initiator
present → construct COMPLETED acies (typi shared — new node via
silva_c89_typus_acies, never mutate). Count: congeries top-level
NODE elements (elision measured ZERO in corpus — fully braced,
count == array count); string literal → decoded octeti + 1
(chorda_decodere). extern/tentative without initiator stay
incomplete — correct per the norm. Typedef guarded (cannot have
initializers anyway).

Gates: suite 29/29; percursus 100.00% coverage, 0 errores,
indecisa 9 (unchanged); diagnostica 455→463, classified: +7
accessus-incompletae (typing progresses deeper into POSIX files),
+1 inside the stb_image vendored park; **auspex 3,753/0 — clang
certifies every completed sizeof**; amalgama VERIFICATUM 28/28.
Officina: fusor sistere 2,659→2,604 in 68→53 plagulis
(magnitudo 94→29 instr; initiator staticus GONE); cursor Wave-0
**47→52/73** (arbor2_glr 101M instructions PASSES, arbor2_scribere,
compound, utf8, capsula incl. gzip inflation, flatura).

Note for the future standards-audit layer: this construct is
conforming C89 (ancient element-count idiom); the C99 cousins a
lint pass must distinguish: flexible array members, VLAs,
designated initializers. The :norma pseudo + standard-tag pin
(quaestio QC) is the named home for that query.

## 2026-07-11 — examen chunk A: the self-referential corpus lesson

While proving the "diagnostics firing unchanged" gate (percursus
pre/post diff via `git stash push` of just the two edited files), the
counts refused to match: baseline 619/46 vs post 588/45, plus ~470
extra symbols/typations post-change. Cause: **the percursus corpus
includes your own diff** — the edited semantica.{c,h} are corpus
members (bigger file = more nodes), and the NEW untracked
probatio_silva_examen.c is swept too: under the STASHED (old) header
its EXAMEN_CODEX_* references are unknown identifiers → 31
"identificator ignotus" diagnostics that vanish under the new header.
Per-file diff (`grep "diagnostica" | sort` both runs, diff) showed
the entire delta was that one self-referential file — every
pre-existing corpus file fired identically. Method note for future
gate-proofs: always diff PER-FILE, never totals; and expect your own
new test files to diagnose under the stashed baseline.

## 2026-07-13 — anonymous-tag baptism + renderer totality (legatus shakedown find #1)

Hover on `legatus_currere` returned a bare name: one parameter of
type `constans LegatusConfiguratio*` — a TAGLESS struct typedef —
made `silva_c89_typum_scribere` return irreddibilis, killing the
whole signature. Three changes:

1. **Baptism** (`_typedef_baptizare`): a typedef donates its name to
   an anonymous struct/union/enum tag at registration time (clang's
   pattern; first typedef wins, named tags untouched, qualificatus
   wrappers pierced). Called from BOTH registration paths — the
   organic `declarationem_tractare` est_typedef branch AND the
   API-injection `silva_c89_typedef_registrare` (which, note, has
   ZERO organic callers — nexus proved the two paths never meet).
   Safety argument: tag identity is NOMINAL (by declarans, not by
   titulus — header contract), and grep shows titulus is read only
   by the renderer and officina_indicium. examen diagnostics never
   print it → VECTIS TENET confirmed no differential drift.
2. **Renderer arms**: ACIES was falling through to ordinarius
   (`T[n]` / `T[]` now); anonymous aggregates that baptism can't
   reach (no typedef at all) render `structura <anonyma>` instead
   of failing; ENUMERATUS now appends its tag when present.
   The renderer can no longer fail on any valid type — only on
   TYPUS_C89_ERROR or buffer exhaustion.
3. **Side-find**: silva/compile_probationes.sh had been BROKEN since
   legatus chunk C — its instrumenta/*.c glob swept the amalgam-world
   nexus_ordines.c into the fontes world (silva.h not found). Nobody
   noticed because the officina runner was the daily driver. Fixed
   with an explicit documented skip. Law reaffirmed: one source
   cannot speak both spellings; flat instrumenta/*.c is presumed
   fontes-world by the runner.

Free consequence: officina_indicium writes tag.titulus when present,
so typedef'd-anonymous structs now carry real names into vindex too.
Bars: semantica 332/332 (new PROBARE block: baptism, no-overwrite,
function signature, arrays, placeholder), hospes 31/31, officina
12/12 suites (indicium pins unmoved), examen_vectis TENET.

## 2026-07-14 — macro domesticum in capite alieno (codex 54)

Implemented from the phase-log INTENTIO (capture-then-implement,
third proof: zero design reversals). Three implementation finds the
hot context did NOT have:

1. **Fons vias are BASENAMES** ("sqlite3.h", never "vendor/sqlite3.h")
— praebere is basename-keyed in BOTH loaders (examen.c AND
praeparator.c), and percursus row attribution ("viae sine '/'
omissae" in fundere) DEPENDS on it — do not "fix". The INTENTIO's
via rule therefore runs on the INCLUSION GRAPH's written directive
paths (exp->inclusiones[].via, "../vendor/sqlite3.h" as typed), not
on fons vias. Alienity propagates transitively (included-by-alien =
alien) in a SINGLE forward pass — parent edges are appended before
child edges (_includendum_processare writes, then recurses).

2. **ISO macros needed excluding** — the INTENTIO's literal rule
("definition fons non-alien") would fire on NULL/EOF/... (systema
lexicon = non-alien) used anywhere in vendor text, i.e. on every stb
TU regardless of include order. New SilvaFons.est_lexicon (set at
the ONE site lexicon fontes are born, _contextum_applicare in
silva_parsare.c); lexicon + synthetic fontes never "domestic" —
vendor code uses standard macros by right.

3. **cum_latina worlds = NAMED LIMIT** — where latina is concatenated
into the systema lexicon text (sessio/colloquium/vindex via
praeparator cum_latina), latina macros are est_lexicon -> check
silent there. The carrying channels (examen.sh, legatus push, MCP
diagnostica, percursus) all true-include latina (legatus PINS
cum_latina=0, legatus.c:~4332) -> latina.h = real praebere'd fons ->
domestic -> check live. Path back if a cum_latina judge ever needs
it: name-test against silva_latina_datum's lexeme set.

Rule detail: only the OUTERMOST expansion of each origo chain is
judged (invocation whose radix is FONS = the name physically written
in the vendor file); macros inside domestic macro bodies are the
definer's business. Argument-substituted tokens self-exclude (their
corpus token sits at the use site = alien). Dedup per (macro name,
alien fons). Message built in sem->piscina (causa contract relaxed,
comments updated in both header spellings); position 1:1 of the
judged file (include directive line is not in the vista).

Corpus: ZERO findings (imago/scrinium order correctly — prediction
held). Exclusiones re-pin 39->40 was phase-A baseline drift
(scrinium.c + probatio_officina_legatus.c = POSIX-lexicon class;
genera_biblia.c HEALED by smart-quotes fix), not this check.
scrinium.c judge cost 0.23s user = baseline-identical (fast path
exits at the edge scan for vendor-free TUs).

## 2026-07-16 — conversio signi (gradus DOMESTICUM)

The i32-unsigned wall gets a judgment tier (desideratum 01KXPXBQG7:
three -Wsign-conversion compiler errors AFTER examen said ACCIPE in
the QB arc alone). Finds at build time:

- SEAT REFINEMENT: the desideratum sketched a post-pass over the
  typationes table; code contact found `_conversionem_annotare` is a
  17-line funnel ALL ~35 conversion-recording sites flow through,
  with {nodus, naturalis, finis} in hand, parsura_currens set
  (positio materializes), and walk-order determinism (a hash-table
  post-pass would emit in nondeterministic order). One additive call
  in the types-differ branch; zero typing surgery.
- ENUM CALIBRATION FIND: clang gives non-negative enums UNSIGNED
  compatible types — `unsigned u = e;` is SILENT under clang
  -Wsign-conversion while our enum-as-int model would fire. C89
  enum type is impl-defined, so v1 EXCLUDES enumeratus entirely
  (named limit in _primitivum_integrale's comment). Calibrate
  against the live oracle BEFORE pinning asserts — this would have
  been a corpus false-positive class discovered the slow way.
- SILENT PROBE: the constant-fits suppression calls
  silva_c89_constans_aestimare on candidate nodes; its
  magnitudo-typi path reaches formam_computare which CAN emit
  diagnostics (incomplete types etc.). Guard = snapshot
  xar_numerus(diagnostica) before the probe, xar_truncare after if
  it grew. Total-count assert in the probatio pins "no leaked
  probe noise" on the common path.
- Differential quality: the conversio_signi.domesticum fixture
  agrees with clang line-AND-COLUMN exactly (13:12/23:13/30:13/
  36:12) — the radix-token positio materialization is byte-precise
  on this class. Vectis ②b also pins count parity (pins == oracle
  warnings) per fixture.
- addere refactor: silva_c89_diagnosticum_addere_cum_socio body
  became interior _diagnosticum_addere_plenum(+causa_structa);
  structured causa (both types named via silva_c89_typum_scribere,
  piscina sprintf) rides the STANDARD positio/provisionale path —
  the MACRO_DOMESTICUM manual-positio pattern was not repeated.

### Addendum, same day — the named tail arrived on schedule

First corpus contact: 217 firings, every one a false positive by
construction (whole repo compiles under -Wsign-conversion -Werror).
The INTENTIO's "per-intervallum = corpus-contact-only" clause was
the right call — the tail was real and had exact shape:

- 145 = vendored stb_image.h. The naive fons-via check NEVER fired
  because fons viae are BASENAMES (praebere) — the exact trap the
  MACRO_DOMESTICUM comment documents. Fix: factored its per-fons
  alienity table into _alienitates_computare (shared), cached on
  sem per-parsura (alieni_parsura key), queried by radix
  fons_index. DOMESTICUM tiers judge domestic code only.
- ~53 = clang's IntRange analysis we lacked. Built
  _intervallum_expressionis {latitudo, non_negativum}: constants
  exact (silent probe), ternary = arm union, comparisons/logical =
  {1,nonneg}, bit/arithmetic operators by widths, fallback = TYPE
  range from the M0b typationes table (the table paid off — every
  subexpression already carries its type).
- CALIBRATION LAW (fired three times today): clang's suppressions
  are HEURISTIC, not mathematical — subtraction keeps nonneg when
  both operands nonneg (u8-u8, (1<<n)-1, prod-128 all silent) even
  though 5-10<0; compound-assign LHS conversions (s += u) are
  silent while RHS (u += s) fires; non-negative enums are typed
  unsigned. Every predicate decision MUST be pinned against the
  live oracle first — two of three guesses were wrong.
- One DELIBERATE test revision: probatio_silva_semantica's M0b UAC
  fixture contains a true sign conversion (ul + ll) — its
  zero-diagnostics pin became a one-DOMESTICUM-row pin (clang
  agrees the row is real).
- Stale-binary strike (bench lesson N+1): ran silva/build/examen
  directly after editing fontes — only examen.sh relinks it;
  first "alien guard doesn't work" was a stale binary.

## 2026-07-17 — TOLERA (gradus severi) built; corpus law strikes again

- Self-referential corpus law, FOURTH strike (fragor/invalidum/
  scriptum before): the TOLERA machinery's own doc comments
  ("TOLERA irrita...", "TOLERA linea eadem...") parsed as directives
  with unknown codices. Grammar fix: the codex word must be
  non-empty uppercase — prose about TOLERA continues in lowercase,
  directives don't. Pinned in the unit suite.
- The severa fire path inherits ALL parity suppressions (cast/
  comparison/compound/alien) by construction — it sits after them
  in the same judgment. TOLERA lookup = lazy per-parsura comment
  scan over token trivia (spatia_ante/post), same caching shape as
  the alienity table.
- Adjudication of the 17 measured sites found TWO REAL BUGS
  (delineare dead clamp, color rounding drift) and 5 legitimate
  TOLERA sites (guarded digit arithmetic, mask construction). The
  hit rate argument for strict tiers made itself.

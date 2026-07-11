# OFFICINA M4a — FUNDAMENTA (grounding synthesis)

2026-07-11. Synthesis of four parallel grounding explorers run before
simulatio-6 and the M4a interview, per the ORDO M4 DECISUS addendum
(officina/phase-log.md) and spec-v2 §XI. Explorers: I taxonomia
constrictionum (C89 constraint classes vs what semantica holds),
II caecitates annotatoris (blind spots, WITH experiments — 16 invalid
fixtures through a purpose-built typing dumper), III diagnostica et
recuperatio (existing machinery + recovery posture), IV calibratio
oraculi (clang-16 experiments, working POCs). This document is what
survives; the full reports lived in-session.

Line cites: `h:` = silva/fontes/silva_c89_semantica.h,
`c:` = silva/fontes/silva_c89_semantica.c.

## I. Load-bearing facts

**1. Semantica already judges — M4a productionizes an existing signal.**
(All four explorers converged on this independently; it reframes the
milestone.) ~44 `silva_c89_diagnosticum_addere` call sites fire ~40
distinct violation messages today: undeclared value-identifiers
(c:3597), non-function callees (c:4458), ALL operator operand-class
violations (10 diagnostics, c:3848–3988), member-access errors
(c:4600/4638/4606), incomplete-type access, invalid array sizes,
non-constant enum values, and more. What's missing is NOT detection —
it is: severity, materialized positions, policy, the compatibility
relation, and an invalid corpus exercising paths the clang-prevalidated
real corpus never hit. Framing M4a as greenfield would overscope it.

**2. `_ad_finem_annotare` (c:3338) is THE seat.** Single chokepoint for
the entire target-context family — called from exactly 5 sites: scalar
init (c:1638), aggregate element (c:1949), assignment `=` (c:4167),
call argument (c:4492), `redde` (c:4843). When operand classes don't
match it currently falls off the end silently — no `else` (c:3350-3364).
Teaching this ONE function to return a verdict wires assignment +
init + argument + return + element compatibility all at once.

**3. Worse than silent — it FABRICATES.** The monstrator branch
(c:3355) accepts ANY pointer source and writes a bogus `conversus`
into demissio's frozen contract. Observed (explorer II, t/04 and
t/15): `int** pp = p;` (an `int*`) fabricates `conversus=int**`;
`int* p = &c;` (a `char*`) fabricates `conversus=int*`. The M4a change
at this seat is partly SUBTRACTIVE: reject AND refrain from
annotating. Any design that only appends diagnostics misses this.

**4. Recovery is INHERITED, not built.** Two working layers already:
- Parser never fails on malformed input. The segment-cutting loop
  (silva_parsare.c:246-573) wraps unparseable segments in ERROR nodes
  carrying their raw tokens (_nodum_erroris_facere,
  silva_parsare.c:110-139; invariant at :52 "numquam arborem falsam")
  and ALWAYS commits a full walkable tree. The only NIHIL-tree paths
  are infrastructure failures (null args, alloc failure), never input.
  `parsura->numerus_errorum` is public (amalgam silva.h:569).
- Semantica poison discipline holds: singleton `TYPUS_C89_ERROR`
  (sem->typus_erroris, c:303-313) absorbs without cascading —
  `_conversionem_annotare` returns early on poisoned operands with NO
  new diagnostic (c:3253-3269); header contract at h:21-23. Confirmed
  experimentally (t/14: bogus inner assign flows up, no cascade).
The enforcement layer's recovery rule is one line: never re-diagnose a
node whose type is already TYPUS_C89_ERROR.

**5. The diagnosticum struct is the gap, not the messages.**
`SemanticaDiagnosticum = {const SilvaNodus* nodus; const char* causa}`
(h:196-199) — no severity, no stable code, no materialized position
(5 of ~44 sites pass a NIHIL node = no position at all:
c:1143,1157,2154,2190,2195), no related-site list ("prius hic
declaratum"), no ranges. Position plumbing EXISTS (token origo chain →
silva_token_radix → root linea/columna + silva_fons_via; censor's
_flagrationem_referre is the proven walk, censor.c:194-228; origo.c's
_catenam_imprimere:41-103 reconstructs macro-expansion context) but
every consumer re-walks it by hand — a shared resolver is a named
deliverable. Node byte-extent for LSP ranges exists but is static
(_extentum_valoris, silva_parsare.c:639-726) — needs promotion.
Compile-time diagnostics keep line:col (machinula's line-only
coarsening is a runtime artifact of the distilled table — don't
inherit it).

**6. est_implicitum is an OUT-OF-BAND channel — and the severity
template.** It lives on SemanticaSymbolum (h:172, set only at c:4398),
NOT in the diagnostica array; an enforcement walk that only reads the
diagnostics list misses the canonical legal-but-suspect case. The
existing asymmetry is already correct C89 (confirmed t/07): unknown
callee = legal implicit-int, flagged; unknown value-identifier =
diagnosed error. That asymmetry IS the violation-vs-suspect split the
interview formalizes. (M4b note: implicit-then-later-real-declaration
needs reconciliation across REPL turns.)

**7. Judgment must gate on ambiguity — canonical-of-fork = the
EXPRESSION reading.** The typing engine canonicalizes first
(_canonicum at c:3512) and only ever sees the canonical reading. On an
UNRESOLVED decl/expr fork (`Foo * bar;` with unknown Foo), the
canonical is the expression reading: judged as a multiplication of two
unknown ids → 2 spurious "identificator ignotus" (observed, t/12) —
a valid declaration rejected. Directly threatens "real corpus 100%
accepted." Two rules: (a) load systema/oracle knowledge BEFORE judging
(M0b's lexicon lesson restated: diagnostics 18,293→245 when the
lexicon attached); (b) enforce hard only on fully-resolved trees —
verdicts under a retained-AMBIGUUS canonical are provisional or
suppressed. `_intra_ambiguum` (c:2980) is the existing gate to extend.
Also: enforcement judges the post-closure sem2 (the two-pass closure
lives in the HARNESS, percursus.c:558-598, not in analysare).

**8. The clang oracle needs calibration in BOTH directions**
(clang-16 = Apple clang 16.0.0 / clang-1600.0.26.4, arm64-darwin23.6).
Defaults falsely REJECT legal C89 (implicit function declarations —
self-promoted to default-error in clang 15/16) and falsely ACCEPT real
constraint violations with exit 0 (incompatible pointer types,
const-discard, ptr==int compare — warnings by default).
-pedantic-errors closes the lenient side but adds false rejections
(K&R defs) and LEAKS designated initializers (classified as GNU
extension; even -Weverything misses it — only -Werror=c99-designator
catches it). Working oracle string (tested):

    clang -std=c89 -pedantic-errors \
          -Wno-long-long -Wno-overlength-strings \
          -Werror=c99-designator \
          -Wno-error=implicit-function-declaration \
          -Wno-strict-prototypes \
          -ferror-limit=0 -fno-caret-diagnostics -fsyntax-only

Exit 0 = ACCEPT, 1 = REJECT. House-dialect flags are load-bearing: the
real corpus has 7 long-long files — strict C89 rejects lib/json.c.
Both sides of the differential must run identical dialect flags.
Demotion mechanics are two-tier: default-errors demote with
-Wno-error=X, but -pedantic-errors-promoted diagnostics need full
-Wno-X (-Wno-error= does NOT outrank -pedantic-errors). Version
recording is MANDATORY per run — the accept/reject line moves across
clang versions.

**9. Severity taxonomy maps onto invocation tiers nearly for free:**
- violatio            → the -pedantic-errors line (+ designator plug + dialect)
- licitum-sed-suspectum → the DELTA of -pedantic -Wall -Wextra -Werror
                          minus -pedantic-errors (assign-in-condition,
                          fall-off-end-of-non-void, unused, …)
- lint-domus          → beyond clang; house flags (-Wconversion,
                          -Wsign-conversion, -Wcast-qual) supply raw
                          material; latina/censor class lives here too
-pedantic-errors vs -pedantic -Werror diverge on EXACTLY the
suspect-tier cases — the two constructions ARE the two tiers.

**10. The differential can NOT be diagnostic-count-based.** Many
current diagnostics are systema-lexicon gaps (unknown POSIX
types/macros) — missing knowledge, not rejection. The harness needs a
causa→severity mapping (a classification pass over all ~40 existing
messages) and lexicon loading before any count is meaningful. Pinning:
compare (verdict, line, warning-group) — NOT columns; clang anchors at
non-obvious spots (the declarator, not the initializer — observed) and
columns drift across versions. Bless pins from oracle output
(captare.sh precedent). Runtime measured: 23-41ms per file
-fsyntax-only → ~20s serial over ~622 TUs, a few seconds fork-per-file
(cursor pattern) — feasible as a STANDING gate, not just a milestone
bar.

**11. What genuinely must be BUILT (the complete new-machinery list):**
- The compatibility relation `_typi_compatibiles(a,b)` per C89 6.1.2.6
  (~150 lines): pointer compat (recurse pointee + assignment target-qual
  ⊇ source-qual), array compat (element + size/-1 sentinel), function
  composite types (prototyped/K&R reconciliation — flags exist:
  est_prototypata/est_variadica h:118-123), tag/enum nominal-by-declarans,
  void* rules. ~80% of INPUTS exist: _qualibus_exutum c:2966,
  _quales_typi c:2998, _est_arithmeticum c:3068, _ut_monstrator c:3274,
  _est_constans_nulla c:3296, _lapsus c:3047, _promotum c:3014, the
  certified UAC engine _commune_arithmeticum c:3179. TRAP: interning
  makes identity a free pointer compare (_derivata_paria c:343) but
  char* and const char* are SEPARATELY interned — shortcutting
  compatibility with == gets qualifier rules subtly wrong.
- A severity-bearing decision procedure on top of the relation
  (LICET / MONE_* / ERROR) — where the taxonomy becomes code.
- Lvalue classification — the ONE genuinely new per-node analysis.
  C89 lvalue-ness is an expression-FORM property, orthogonal to type;
  semantica classifies nothing today (grep: two designator comments).
  Blocks `5=x`, assign-to-array, assign-to-const, `&`/++ enforcement.
  ASSIGNATIO case (c:4130) never inspects its left node; result strips
  quals (c:4157) — const-assignment passes silently (t/09).
- Cheap unwired table wins: call arity (VOCATIO c:4352; the explicit
  park comment "aritas non diagnosticatur - laminae exsecutionis" at
  c:4531), return void/non-void (reditus_currens h:243), controlling-
  expression-scalar (predicate exists, unwired).
- Later/optional classes needing new walk state: switch case-set
  (duplicate cases, default count), initializer validation
  (excess/shape/static-const — completion shipped 2026-07-10;
  VALIDATION distinct and unbuilt), struct member legality.
- Richer diagnosticum struct + shared position resolver + extent API
  (each new public row = amalgam export + hospes call, standing rule).
- A committed single-file driver (parse + analyse + dump diagnostics
  w/ positions + verdicts). NONE EXISTS — percursus is corpus-only,
  counts-only; the M0b classification used an UNCOMMITTED throwaway;
  explorer II had to build its own (~230 lines, public API sufficed).
  The differential harness needs it anyway → critical path.
- Judgment hooks INLINE in the walk, not a second pass: the operand
  pairing (e.g. function type + arg list for arity) is only available
  during the walk; the flat typationes table doesn't store structure.

**12. A corpus-native SECONDARY bar is available.** demissio currently
defers judgment to runtime SISTERE ops with interned cause strings
(officina_demissio.c:645-651, 940-957) — the "semantica-diagnosed"
class (126 corpus cases) is exactly nodes semantica flagged and
demissio parked. Judgment-before-lowering should drive
sistere-for-semantic-reasons toward zero on well-typed input — a
measurable complement to the differential bar. (This is the posture
officina/CLAUDE.md names: "the sistere op is that layer's named future
runtime seat.")

**13. REPL-reality ranking splits cleanly** (payoff is front-loaded):
top live-user errors are (a) already enforced — undeclared id, member
access, operator misuse; (b) cheap table wins — arg count, return
mismatch; (c) the compatibility relation — arg/assignment type
mismatch; (d) lvalue — the one new analysis. Implicit-call wants a
WARNING in a REPL (very common interactively, legal C89).

## II. Existing surface inventory (what M4a consumes/extends)

| Component | State | M4a role |
|---|---|---|
| SemanticaDiagnosticum {nodus,causa} + addere (c:66-84) + numerus/per_indicem (amalgam) | 44 sites, ~40 messages | THE struct to extend (severity, code, position, related) |
| est_implicitum (h:172, c:4398) | flag on symbol, out-of-band | suspect-tier template; walk must scan symbols |
| TYPUS_C89_ERROR poison (c:303-313, h:21-23) | working, no-cascade | inherited recovery; rule: never re-diagnose poisoned |
| ERROR/AMBIGUUS genera (amalgam :776-777) + numerus_errorum (:569) | parser recovery complete | tree always walkable; judge post-closure sem2 |
| _intra_ambiguum (c:2980) | suppresses typedef-in-expr diags in retained forks | the gate to extend for provisional verdicts |
| _ad_finem_annotare (c:3338; 5 call sites) | silent fall-through + fabricating ptr branch | THE verdict seat (subtractive fix included) |
| Type predicates + UAC (c:2966-3296, 3179) | certified (auspex 3,753/0) | compatibility relation inputs (~80% done) |
| censor walk (censor.c:194-228) + 3-level exit codes (:206-268, 818-822) | shipped, FP 0.00 | reference reject-with-positions consumer; de-facto severity scheme |
| origo chain walk (origo.c:41-103) | shipped | macro-expansion context for diagnostics |
| _extentum_valoris (silva_parsare.c:639-726) | static/unexposed | promote for LSP ranges |
| haruspex/auspex harness pattern | shipped, incremental cache | "compilatio est iudicium" extended to "same judgments"; enumerate includes verbatim |
| percursus two-pass closure (percursus.c:558-598) | harness-side | enforcement judges sem2 the same way |

## III. Open questions → the interview

1. **NAME ceremony.** Candidate: examen (cave iudicium ~ indicium).
2. **Violatio tier: C89-standard-strict or house-strict?** They differ
   exactly on K&R prototypes + implicit function calls. Explorer IV
   recommendation: standard-strict for the tier; house strictness
   routes to lint-domus (corpus passes both; keeps tiers honest —
   auspex precedent for deliberate, documented flag deviations).
3. **Seal the oracle invocation string** (fact 8) — and where does the
   fixture sweep for sibling GNU-extension leaks live?
4. **Severity policy:** adopt the 3-tier ↔ invocation-tier mapping
   (fact 9)? Classify all ~40 existing messages into tiers (the
   causa→severity mapping is REQUIRED for the differential anyway).
5. **Diagnosticum v2 shape:** which fields v0 — severity, stable code,
   materialized {via,linea,columna}, related-site list, byte range?
   Shared resolver API name? NIHIL-node sites get fallback tokens?
6. **v0 scope:** compatibility relation + arity + return + lvalue as
   the core? switch case-set / initializer validation / member
   legality named-deferred or in?
7. **Ambiguity policy:** suppress vs mark-provisional on
   retained-ambiguus canonicals? (Fact 7 — the 100%-accept bar hinges
   on it.)
8. **Fabricated-conversus retirement** (fact 3): rejecting means NOT
   annotating — confirm demissio/percursus consumers tolerate absent
   conversus on rejected nodes (poison instead?).
9. **Books silva-side** per M0 precedent (spec amendment already says
   so) — confirm + phase-log pointer discipline.
10. **Single-file driver:** name/home (examen CLI? silva/instrumenta?);
    output format (human + machine-readable for the harness?).
11. **Secondary bar adoption** (fact 12): sistere semantica-diagnosed
    → 0 as a formal M4a gate, or a recorded measurement?
12. **Invalid-corpus format:** EXSPECTA markers pinning
    (line, warning-group); home (silva/probationes/fixa? own dir?);
    bless-from-oracle workflow (captare precedent).
13. **Standing gate posture:** differential as amalgamare/percursus
    gate (measured feasible: seconds forked) or milestone-only?

## IV. Corrections to the pre-exploration framing

1. **"Semantica contains ZERO judgment" is FALSE** — the load-bearing
   correction, found independently by explorers I, II, III. It
   annotates AND diagnoses (~40 messages); it never rejects, has no
   severity, and its diagnostics lack positions. M4a = complete +
   productionize, not create.
2. **"Recovery mode" is not a build item** — both layers already
   recover (fact 4). The interview question shrinks to the ambiguity
   policy (fact 7).
3. **The spec's bar oracle "-std=c89 -pedantic-errors" is
   under-specified in both directions** (fact 8) — the interview seals
   the full string.
4. **Blind spots are CONCENTRATED, not scattered** — one chokepoint
   function + two node cases (assignment lvalue, call arity), not an
   audit of every expression family.
5. Stale header comment: h:26-28 still lists "initiatores non
   probantur (M0b)" — array-completion-by-initializer SHIPPED
   2026-07-10; initializer VALIDATION remains unbuilt. Fold on next
   semantica touch.
6. Diagnostic-site count is ~42-44 (grep variants across explorers);
   treat as ~44, exact count at classification time (question 4).

## V. Explorer artifacts

Scratch (session scratchpad, not committed, will vanish): explorer II's
dump.c (~230-line single-file typing/diagnostic dumper — public API
sufficed; rebuild as the committed driver) + t/01-t/16 invalid
fixtures; explorer IV's casus/ (21 constraint fixtures), c99/ (11
C99-feature fixtures), lenient/ (6 default-severity probes), pinning/
(working extractor POC: EXSPECTA markers → PIN-MATCH/MISMATCH verdict —
it caught its own mis-pinned column, proving the line+group rule).
These fixture SETS are the seed of the synthetic invalid corpus —
recreate deliberately in-repo at spec time, don't rescue the scratch.

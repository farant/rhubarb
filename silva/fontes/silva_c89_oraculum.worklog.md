# silva_c89_oraculum worklog

## 2026-07-10 — SANATIO CATENARUM: the M0a nested-cast park falls
## (dual trivalent kill + winner-only flag flow)

Pulled by officina M2c: 19 Wave-0 suites halted at runtime on
sistere "vocatio sine typo functionis" — all tracing to ONE parse
shape: `(typename)(parenthesized-operand)` packed as a CALL and
retained. This was M0a's named resolver park ("nested cast/paren
chains + GLR fork-as-reading packing"), landing spot "chain-aware
joint resolution, or resolve-inner-first fixpoint". Both candidate
designs from that ledger note shipped, as two complementary rules:

**1. The DUAL trivalent kill** (`typus_ut_expr_adest`). The shipped
trivalent rule kills readings that use a known NON-type as a type.
The missing symmetric rule: a reading that uses a known TYPE as a
plain identifier-expression (FOLIUM_IDENTIFICATOR) is impossible —
C89 cannot parse an unshadowed typedef name in expression position.
Shadowing protection is inherited for free: a shadowing declaration
registers the name as non-type, and the rule requires
`typum_novit && !non_typum_novit` (exact mirror of the first rule's
"typus vincit" conservatism). Namespace safety is by construction:
member names (accessus_tok_titulus), labels (titulatum_tok_titulus),
goto targets (salta_tok_destinatio) are TOKEN slots, not folium
nodes — the examination never sees them as identifiers.

WHY the old rules missed this shape: `(u32)(sizeof(catena))` — the
INNER fork (sizeof-type vs sizeof-expr) contains the known typedef
`catena` and sits inside BOTH outer readings. nominatus flows up
from ANY inner reading, so cast AND call readings both became
"typo-positive" — the uniqueness rule (c) broke on a tie. Plain
`(T)(x)` always resolved (no inner nominatus); every corpus site
was nested (`(i32)((i8)buf[i] & 0xFF)`, `(i32)(magnitudo(chorda))`).

**2. Winner-only flag flow through decided inner forks.** In
`_nodum_examinare`'s AMBIGUUS case: the inner fork is now resolved
on the spot (recursive `_ambiguum_examinare_prof`, depth-capped;
stored verdicts consulted first) — if decided, flags flow from the
WINNING reading alone (a decided fork is an ordinary node). Only
undecided forks keep the conservative merge (nominatus/ignotus from
ANY reading; poisons only if ALL readings carry them — the Chunk C
contagion fix, now extended to the new poison flag).

**Emergent bonus — transmutation at parse time**: with typedefs
visible in walk order (file-scope or preloaded contextus — i.e. the
whole corpus), the chains now decide at the PRECOMMIT walk, so the
engine TRANSMUTES them into definite nodes: no ambiguus is retained
at all. The corpus sites vanish from retention rather than being
closure-decided.

**The dead-fork finding** (catenae nidificatae repro): the SCR_ELEM
fixture's residuum-of-1 turned out to be a fork whose readings are
ALL impossible (it lives inside a reading its parent already
rejected). It cannot be decided (no valid winner) and sits OFF the
canonical spine — invisible to typing/demissio. Honest retention.
The probatio now documents this: live forks all decide; the dead
fork stays. NAMED possible follow-up (no pull yet): a
reachability-aware indecisa metric that skips off-spine forks.

**Numbers** (percursus full corpus): indecisa 305 → **9**; retained
ambigui ~3,354 → **1,046**; coverage 100.00% HELD; errores 0;
fideles 759/760 (the 1 = the pre-existing labeled pasta deferral,
unchanged); diagnostica 399 → 455 — classified: known POSIX/fixture
classes (accessus structurae incompletae 151→187, POSIX
types/macros, latina-less fixtures) un-gated by forks becoming
decided; NO new cause names. **auspex: 3,753 assertions, clang
consentit, 0 dissent** — the certification that no fork was decided
wrongly. Suite 29/29 (+ new fixture pinning the corpus shape:
ambigui ZEPHYRUM retained). Amalgam VERIFICATUM, hospes 28/28.

**Downstream (officina)**: fusor — sistere "vocatio sine typo
functionis" collapsed to 1 instruction corpus-wide; total sistere
2,912 → 2,659 in 105 → 68 plagulae; circuli 578/578 byte-idem.
Cursor Wave-0: **31 → 45/73 praeteritae**. New classes surfaced by
suites advancing past the old wall: forma localis ignota (9 suites),
nuntium EXITUS 1 (first wrong-exit — runs to completion, fails an
assert: investigate), lapifex TEMPUS pending long-boundary run.

## 2026-08-19 — lexicon typedefs preloaded into the oracle (01M0D4QV5S part 1)

The lexicon path in the expansion layer processes DIRECTIVES ONLY
("Reliqua lexici abiciuntur"), so latina.h's typedefs (i8/i32/s64/
b32...) never reached the oracle: `i8 * t;` stayed retained-AMBIGUUS
where clang sees a declaration — every cum-contextu consumer diverged
from the compiler on latina-typedef disambiguation (the frozen
`i8 * novum_datum;` damage class the formator hit in nuntium.c).

Fix in silva_c89_parsare_cum_contextu, using the seam the API comment
already blessed ("oraculum datum typos plagulae ACCIPIT - praeonera
typos externos ante"): each lexicon plagula is parsed STANDALONE with
the shared oracle before the main parse. Typedefs register through
the ordinary precommit walk (titles are COPIED into the oracle's
piscina; the lexicon tree is discarded); later lexica see earlier
lexica's types (shared-oracle cascade); preloaded = visible
everywhere because visibility is walk ORDER, not cross-file situs
(the 2026-07-06 sanatio). The expansion layer keeps its
directives-only contract — this lives entirely in the c89 layer.

Notes:
- Grammar pre-check: `typedef unsigned long long` parses (i64/s64
  register fine).
- Per-parse cost: one standalone parse of each lexicon per
  cum-contextu call (latina.h ≈ mostly directives, tiny GLR load).
  Caching door: preload once per long-lived oracle, not per call —
  park until mensor says it matters.
- The old c89 probatio vector `"Meus m;" + lexicon typedef` passed
  BEFORE this fix only because root-level `Meus m;` cannot fork —
  it proved the parse, not registration. The new tests assert
  typum_novit directly + the forking shape `i8 * t;`.
- memoriae_index: `typedef size_t memoriae_index;` — size_t is a
  SYSTEM typedef silva never sees; whether it registers is not
  pinned here. The named door: system typedefs in the systema
  lexicon files (now that lexicon content flows, adding typedef
  lines there heals FILE/size_t/... for free).

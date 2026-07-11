# OFFICINA M4b — FUNDAMENTA (grounding synthesis)

2026-07-11. Synthesis of four parallel grounding explorers run before
simulatio-7 and the M4b interview, per the ORDO M4b addendum
(officina/phase-log.md) and spec-v2 §XI. Explorers: I modus initus
incrementalis (what silva provides for fragments/turns — WITH
experiments: bare fragments fed to examen), II mundus vivus (the
linked world's growth/persistence — WITH two compiled probes vs the
officina amalgam: probe_mundus, probe_regio), III reconciliatio
symbolorum (implicit→real across turns — WITH a two-turn systema
driver vs the silva amalgam proving cross-turn reconciliation), IV
superficies producti (read loop / printing / #! — WITH live shebang
experiments through examen). This document is what survives; the
full reports lived in-session.

Line cites: `sem.c/.h` = silva/fontes/silva_c89_semantica.{c,h};
`conexio.c` / `regio.c` / `machinula.c/.h` / `demissio.c` =
officina/fontes/officina_*; `onerator` =
officina/instrumenta/vindex_onerator.c; `examen.c` =
silva/instrumenta/principalia/examen.c; `c89.stml` =
silva/grammatica/c89.stml.

## I. Load-bearing facts

**1. The fragment door is ALREADY OPEN — a REPL turn parses with no
wrapping.** Grammar start symbol `elementa` (c89.stml:1216) is a list
of `elementum` = sententia | declaratio | definitio-functionis
(c89.stml:156-171), with the intent stated in the grammar itself:
"Liberalitas: sententiae in summo gradu licitae (norma tree-sitter;
conformitas = lint)" (:163-166). Verified live: `int x = 5;`,
`if (x) f();`, and a function definition all parse as complete
top-level input. `corpus` (brace block) uses the SAME rule as the
segment root — a turn and a function body are the same grammatical
object (:240-247). The ONLY gap vs "any snippet": a bare expression
needs its `;` (sententia-expressionis, :783-786) — append one and
pull the expression back out via the existing accessor
(silva_c89_sententia_expressionis_expressio, silva.h:900). Do NOT
wrap turns in a synthetic function by default: wrapping introduces a
scope boundary and forfeits free file-scope accumulation (fact 2).
Caveat: `return 5;` / `break;` at top level parse AND pass the judge
silently — statement-context legality is unchecked; REPL policy
needed (interview).

**2. The systema channel IS the cross-turn reconciliation mechanism —
proven by experiment, not just read.** (Explorers I and III converged
independently; this is the milestone's reframe.)
`silva_c89_semantica_analysare_cum_systemate` walks a systema parsura
first, registering ALL symbols — functions, variables, typedefs —
marked ex_systemate, then walks the current parsura against them
(sem.c:2293-2340). Explorer III's two-turn driver: turn 2 calling
turn 1's function and using turn 1's struct → analysed alone: 1
violatio + 1 suspectum; analysed with turn 1 as systema: **0
diagnostics, symbol visible ex_systemate**. "Turn N sees turns
1..N-1" = feed accumulated prior turns as systema. What's missing is
PLUMBING: the API takes a single systema parsura; prior trees must
stay alive (the onerator destroys the silva arena per TU,
onerator:305, but the systema walk re-walks the tree, sem.c:2314);
and typedef knowledge must ALSO reach the parser via a second,
separate feed (fact 4).

**3. Two incrementality axes — do not conflate.** recanonicare +
bis-analysis (sem1 → oraculum_augere → responsa_vacare →
silva_recanonicare → sem2; recipe examen.c:329-356, onerator:279-289)
is STRICTLY intra-parse: it re-decides retained-AMBIGUUS forks of one
already-parsed buffer in place, oracle fresh per TU. It is NOT the
vehicle for absorbing a new turn — adding source is always a new
parse. The REPL uses both, separately: bis-analysis inside each turn,
systema chaining between turns.

**4. Parsing and judging need prior context from two different
feeds.** Judgment: systema channel (fact 2). Parsing: the oraculum —
the typedef-vs-identifier disambiguator, trivalent, positional
(situs-0 = globally visible, silva.h:426-431; augere silva.h:
1301-1304). Prior turns' typedef names must be preseeded situs-0
before parsing turn N, else `myint z;` misparses regardless of what
the systema knows. SilvaContextus itself is a const receipt — parse
cannot mutate it; one context (lexicon = latina.h + systema headers,
praebenda = repo header sweep, examen.c:124-170) serves the whole
session (silva.h:985; contextus.h:68-79).

**5. Implicit→real is TWO independent reconciliations — the briefing
merged them.** (i) JUDGMENT-side: systema-feed resolves it (fact 2);
the historical SUSPECTUM "vocatio implicita" emitted in turn 1 was
correct at emission and is never retroactively cleared (verified:
still flagged when defined later in the same buffer). (ii)
EXECUTION-side: world-link binds the reference to the real
definition. Neither needs the other; conflating them over-scopes the
semantica work.

**6. Undefined symbols are ALREADY graceful end-to-end at every
layer.** semantica: synthesizes `extern int f()` at the call site,
est_implicitum, SUSPECTUM, verdict stays ACCIPE (sem.c:5278-5300,
2319-2337). demissio: lowers to a DIRECT external symbol operand
`$f`, not a sistere (demissio.c:2298-2311). conexio: undefined →
DECIPULA (lazy named trap), link SUCCEEDS (conexio.c:338-354).
machinula: executing it → clean distinct halt MACHINULA_DECIPULA
"decipula vocata: f" (machinula.c:2723-2736); never called → nothing.
A REPL turn calling a not-yet-defined function runs today and halts
politely at the call.

**7. THE core design problem: a growing world and a state-preserving
world CONFLICT under the current design.** (Explorer II's central
find.) Three facts chain: (i) conexio is one-shot — modulum_addere
refuses once linked (conexio.c:159), nectere refuses re-linking
(:258, latch :401); (ii) the regio is ONE 64GB mmap at a FIXED base
(0x300000000000) that returns NIHIL if occupied — old and new worlds
cannot coexist; destroy first (regio.c:23-26, 80-91; probe-verified);
(iii) even reusing a regio, nectere ALWAYS re-copies each datum's
INITIAL image over freshly bump-allocated sedes (conexio.c:304-313) —
mutated globals are orphaned. Net: any turn that adds a symbol
currently loses global/heap continuity. Solving (or explicitly
deferring) this is M4b's first design decision.

**8. Redefinition WITHOUT new symbols is a cheap slot swap — the
frozen planae help rather than hurt.** Calls never bake a body
pointer: resolution is descriptor-at-fixed-address → index → BOTH
functiones_nexae[index] and planae[index] in parallel
(machinula.c:2600-2705; indirect calls hold the descriptor ADDRESS,
:2622-2631). Swap functiones_nexae[j].functio + re-freeze planae[j]
(machinula-owned, already mutable for breakpoints) and every caller
redirects automatically. Required cleanup: that function's
breakpoints (keyed (functio_index, flat instr), machinula.c:70-74)
and its indicium entries (keyed by MedullaFunctio*,
officina_indicium.c:37) go stale. New symbols in the body → forces
relink (new module needs a translation array, sealed after nectere).

**9. VM persistence across currere calls is real — and now
probe-verified, but NOT regression-tested.** machinula_aperire's
reset list is narrow: frames, stack cursor, halt state
(machinula.c:1954-1962). Globals, heap (malloc bridge), open FILE*
handles, breakpoints, counters all PERSIST. Probe: global
7→42→43→44 across calls; malloc'd block survives. exit() = halt flag
BENE + codex, no longjmp, no poison — the next currere ran fine
(probe: pre-exit global intact). Halts (SISTERE/DECIPULA/VITIUM)
equally non-fatal (probatio_officina_machinula.c:206-218). TEST GAP:
no committed test writes a global in call N and reads it in N+1 —
"globalia persistunt" was correct DESIGN, structurally guaranteed,
behaviorally unproven until the probe. M4b adds that regression test.

**10. The cursor's world-caching does NOT transfer to the REPL.**
What cursor caches = lowered MedullaModulus structs in the persistent
officina piscina; it RE-LINKS per suite INSIDE A fork
(cursor.c:907, 942-970) — the fork exit is what frees the fixed-base
regio. A single-process REPL cannot fork per turn without losing VM
state. Rebuild economics remain favorable though: on relink the
expensive half (parse → semantica → demissio) survives as moduli;
only link + freeze re-run.

**11. Rebinding is already REPL-lenient at silva level; the friction
is at the linker.** Same-type redeclaration in one buffer is silent
(verified: `int x=5; int x=6;` → ACCIPE); type-changing redecl
diagnoses REDECLARATIO_* — but the incompat check is guarded by
`!vetus->ex_systemate` (sem.c:449), so systema-fed prior bindings can
be freely rebound at judgment level. At link level two modules
defining x = hard "definitio duplex" fracture (conexio.c:209-217):
execution rebinding requires the loader to EXCLUDE the shadowed
module (latest-definition-wins module selection), not just add the
new one.

**12. The tag seam bites exactly ONE case: REDEFINE-with-body + value
crossing.** Struct tags from different parsuras are nominally
DISTINCT (identity = declarans node, sem.h:130; the ex_systemate
redecl clemency does NOT cover tags — separate axis,
_tag_registrare sem.c:979). Experiment: turn 2 REFERENCING turn 1's
struct P → clean; turn 2 REDEFINING struct P with a body, then
passing its value to turn 1's function → MONSTRATORES_INCOMPATIBILES,
REICE. Consequence: a growing-single-TU world model dodges the seam
entirely; a per-turn-parsura model inherits it (re-pasting a struct
def = spurious rejection). This sharpens the world-model choice into
a concrete tradeoff.

**13. The #! line must be actively blanked — and the fix is one line
of loader code, verified.** An unknown directive (`#!...`, also
`#frobnicate`) becomes a syntax-error node → REICE ("pass-through" =
byte retention for roundtrip, NOT error-free; `#pragma` IS
accepted). Verified fix: overwrite the first line with spaces but
KEEP the `\n` before parsing — clean parse AND line numbers stay true
to the user's file (error on physical line 2 reports :2). No silva
change needed; silva has zero shebang awareness (`!` lexes only as
EXCLAMATIO/NON_AEQUALIS, silva_lexema.c:744,768). argv: no channel to
principale exists — machinula_currere zero-fills params
(machinula.h:69-73); v0 choice = drop argv or extend interpretare's
literal-baking wrapper trick (datum blocks + locus,
interpretare.c:408-435) to synthesize an argv array.

**14. Read loop: tessera has NO line editor and REQUIRES a TTY.**
tessera_pons_posix_creare returns NIHIL if stdin is not a terminal
(tessera/fontes/tessera_pons_posix.h:9-11); the event layer delivers
keystrokes, not lines. The only line editing in the repo is vindex's
~30 hand-rolled lines (append/backspace/Enter/Esc; NO in-line cursor,
NO history — Up/Down scroll the source pane; vindex.c:62-64,
1198-1230). Bare fgets keeps the piped/non-TTY path the #! runner
needs anyway. No command-history or dotfile-persistence precedent
exists anywhere in the repo.

**15. Value printing: interpretare is a PARTIAL precedent — scalars
solved, memory unformatted.** It generates a $main medulla-text
wrapper printing the return value (interpretare.c:643-762): void →
"(vacuum)", F32/F64 → %g, S8-S64 → %lld, everything else INCLUDING
pointers → %llu (raw address — medulla has no pointer type,
officina_medulla.h:43-56). Aggregates refused by design (%reditus
convention detected, interpretare.c:606-629 — guard born from a real
corpus crash). Clean stream split to inherit: values → stdout,
receipts/errors → stderr (:889,898). Net-new for a REPL: chorda
contents / char* dereferencing (VM region is host-addressable, so
mechanically possible), `value : type` display (nearly free —
typus_reditus + medulla_typum_titulus already exist, :688).

**16. Incomplete vs wrong: examen cannot distinguish — but the
signals exist one layer down.** All syntax errors reach examen as
"nodi erroris (syntaxis) N" at position 0:0 (examen.c:363-376);
positioned diagnostics are SEMANTIC-only (via/linea/columna,
examen.c:428-431). The REPL reads the tree directly: (i) lexer
brace-balance via paired genera SILVA_LEX_{PAREN,QUADRA,BRACE}_
{APERTA,CLAUSA} (silva_lexema.c:777-782) — the cheap "still typing?"
heuristic; (ii) error-node-at-EOF via numerus_errorum (silva.h:562)
+ silva_c89_error_tokens span (silva.h:951) — sole error ending at
EOF ⇒ incomplete, mid-stream ⇒ wrong; (iii) parse-probing every
partial turn is safe (never-crash bar). Unterminated /* has its own
lexeme genus (COMMENTUM_CLAUSUM absent, silva_lexema.c:470,940).

**17. Session memory grows monotonically without a plan.** Every
parse allocates tree+tokens+semantica into a piscina, nothing freed
until destruere (examen uses one 256MB arena, examen.c:211); every
demittere+relink appends moduli/conexio/machinula/planae into the
officina piscina (onerator:308-314). The two-arena VISIO pin applies:
a persistent world arena (committed turns' trees — which must stay
alive for the systema walk, fact 2 — plus moduli) and a per-turn
scratch arena reset after eval (rejected/superseded turns).

## II. Existing surface inventory (what M4b consumes/extends)

| Component | State | M4b role |
|---|---|---|
| Grammar liberal top-level (c89.stml:156-171) | shipped, verified | turns parse directly, no wrapper |
| analysare_cum_systemate (sem.c:2293-2340) | shipped; single-systema API | THE cross-turn judgment channel; extend to chained/merged priors |
| oraculum augere + situs-0 (silva.h:426-431,1301-1304) | shipped, per-TU fresh | cross-turn typedef preseed (parse-side feed) |
| SilvaContextus (contextus.h:68-79; const at parse) | shipped | one per session; lexicon+praebenda channels |
| examen.c recipe (:293-356) | shipped M4a | the REPL's parse+judge template (bis-analysis per turn) |
| examen verdict + positioned semantic diagnostics | shipped M4a | judgment before lowering = REICE before demissio, free |
| vindex_onerator (onerator:239-365) | shipped M3/M4a, "REPL M4 mox" comment | the loader to extend; NOTE: destroys silva arena per TU (:305) — must retain trees |
| conexio one-shot link + decipulae (conexio.c:159,258,338-354,401) | sealed-by-design | the seal is the growth constraint (fact 7); decipulae = graceful undefined |
| regio fixed-base single-instance (regio.c:23-26,80-91) | by design, probe-verified | one live world; reset story = interview |
| nectere initial-image copy (conexio.c:304-313) | by design | the state-wipe on relink (fact 7iii) |
| machinula planae freeze + descriptor indirection (machinula.c:1657-1779,2600-2705) | shipped M3 | slot-swap redefinition path (fact 8) |
| machinula_aperire narrow reset + currere iterable (machinula.c:1954-1962; machinula.h:69-73) | shipped M2, probe-verified | THE loop primitive; add write-N/read-N+1 regression test |
| interpretare wrapper generator + printer (interpretare.c:408-435,643-762) | shipped M2 | scalar printing + literal-arg baking (argv trick) |
| vindex hand line editor (vindex.c:1198-1230) | shipped M3 | raw-mode template IF wanted; else fgets |
| lexer paired genera + error-token spans (silva_lexema.c:777-782; silva.h:951) | shipped | incompleteness detection signals |
| decipula halt (machinula.c:2723-2736; machinula.h:45) | shipped | polite not-yet-defined runtime UX |

## III. Open questions → the interview

**World model (the big fork):**
1. Turn accumulation: GROWING SYNTHETIC TU (concat + reparse all —
   simple, provably correct, one declarans/tag = no seam; but
   re-judges history incl. historical implicit warnings, O(n) per
   turn) vs PER-TURN PARSURA chained via systema (incremental,
   warning-stable; needs the single-systema limit lifted — list API,
   concatenated prior source, or persistent symbol table; inherits
   the tag seam, fact 12) vs hybrid?
2. Execution world growth: rebuild-per-symbol-adding-turn (globals
   reset — acceptable v0, documented?) vs "keep existing globalia"
   nectere mode vs incremental nectere (append descriptors, never
   touch prior sedes) vs regio checkpoint/reset? Is slot-swap (fact
   8) the happy path with relink as fallback?
3. Is descriptor address stability a CONTRACT across relinks?
   (Indirect calls in live data hold descriptor addresses.)
4. Rebinding policy: latest-def-wins module exclusion at the loader
   (fact 11) — confirm; and what are value semantics of re-running
   an initializer (`int x = 5;` entered twice)?

**Turn semantics:**
5. Bare expression: auto-append `;` + unwrap sententia-expressionis,
   or require termination?
6. Top-level `return`/`break`/`case`: reject (REPL's own lint),
   accept as no-op, or wrap-on-demand?
7. Tag redefinition across turns (if per-turn model): reject with a
   clear message, or auto-unify same-text tags (new mechanism)?
8. Implicit-call ergonomics: SUSPECTUM warning per emission stands
   (correct-at-emission), or suppress when a later turn defines it?

**Session state:**
9. Arena plan: persistent world arena + per-turn scratch (fact 17) —
   what exactly graduates from scratch to world on a committed turn
   (tree? modulus? both)?
10. Oracle lifetime: one session oraculum grown situs-0 per turn, or
    rebuilt from the world each turn?
11. The globals-persistence regression test (fact 9) — in the M4b
    bar?

**Surface:**
12. Read loop v0: bare fgets (works piped, cooked-mode editing only)
    vs tessera raw + hand editor? History in scope or deferred
    (saltuarius precedent)? If persistent: invent ~/.rhubarb
    convention?
13. Incompleteness rule v0: brace/paren balance (cheap, lexer) vs
    error-node-at-EOF (richer, tree API)?
14. Printing depth v0: interpretare scalars verbatim, or + chorda/
    char* contents + `value : type` (the REPL's signature feature)?
15. #! runner: strictly batch (parse+judge+run whole file — nearly
    done today) or sharing the REPL world model? argv: drop (argc=0)
    or synthesize the argv-array wrapper?
16. Indicium per turn: full rewrite (current API, onerator:356) or
    per-function incremental update (redefinition wants it)?
17. Turn verdict UX: REICE → show positioned semantic diagnostics
    (exists) + tree-derived syntax spans (fact 16); does a REICE'd
    turn leave ANY trace in the session (history yes, world no)?

## IV. Corrections to the pre-exploration framing

1. **"Snippets parse with API-injected context" is NOT future
   speculum work** — the fragment door is already open in the
   shipped grammar (fact 1); the C89 REPL need not wait for any
   parser feature. Only true gap: the mandatory `;`.
2. **"Implicit→real reconciliation needs building" is WRONG as
   stated** — judgment-side reconciliation exists TODAY (the systema
   channel, experiment-proven, fact 2); execution-side is the
   world-link. Two independent reconciliations; recanonicare is
   NEITHER (intra-parse only, fact 3).
3. **"cursor caches worlds" is imprecise in a way that matters** —
   it caches lowered moduli and re-links inside a FORK; the pattern
   does not transfer to a single-process stateful REPL (fact 10).
4. **"GLOBALIA PERSISTUNT (probatum?)" — design yes, test no.**
   Probe-confirmed now; regression test is an M4b deliverable
   (fact 9).
5. **"Frozen planae[] — quid mutatur?" answered, and it's BETTER
   than feared:** descriptor indirection makes same-symbol
   redefinition a slot swap, not a rebuild (fact 8). The frozen
   architecture is redefinition-FRIENDLY.
6. **The briefing's unnamed elephant, now named:** world growth vs
   state continuity conflict (fact 7) — THE M4b design problem. No
   prior document states it.
7. **"#! passes through" is FALSE for parsing** — unknown directive
   = error node = REICE; the blank-first-line fix is verified
   (fact 13).
8. **interpretare is a partial printing precedent only** (fact 15);
   **syntax errors carry no positions through examen** — semantic
   diagnostics do; the REPL reads error-node spans from the tree
   (fact 16).

## V. Explorer artifacts

Scratch (session scratchpad, ephemeral, will vanish): explorer I's
fragment probes (bare declaration/statement/expression files through
examen); explorer II's probe_mundus.c + probe_regio.c (globals/heap/
exit persistence; fixed-base regio coexistence — compile vs
officina/amalgama/officina.c); explorer III's repl_probe.c (the
two-turn systema driver — TEST A/B), seam_probe.c (tag seam),
impl_then_def.c trio (warning-not-retroactively-cleared) — compile
vs silva/amalgama/silva.c; explorer IV's shebang fixtures
(sheb/stripped/control). The probe TECHNIQUES (two-turn systema
driver; write-N/read-N+1 persistence probe) are the seeds of M4b's
committed tests — recreate deliberately in-repo at spec time, don't
rescue the scratch.

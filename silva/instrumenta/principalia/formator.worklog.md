# formator worklog

## 2026-08-19 — G1 birth: lint mode, stream tranche (R5/R6/R12/R14/R16)

Contract: project-specs/formator-regulae.md. Split per differre
precedent: machine in instrumenta/silva_formator.{h,c} (linkable into
probationes via the instrumenta glob, portable to silex later), CLI
here. Exit contract matches censor: 0 conformis | 1 divergentiae |
2 usus/illegibilis.

Design decisions:

- **Raw stream, not the parse stream.** `parsura->lexemata` is the
  EXPANDED stream — trivia at macro sites are not source-faithful
  there. `silva_lexare_cruda` gives real tokens + trivia interleaved
  in source order; the whole tranche rides it.
- **Genus semantics = free correctness.** Tabs are their own trivia
  genus (SILVA_LEX_TABULAE), so tabs inside string literals are
  invisible to R5 with zero code. Banner runs are only scanned inside
  COMMENTUM tokens, so `====` in a string literal never fires R14.
  Both negatives are pinned in the probatio.
- **R6 matches valor, not genus, for Latin keywords.** The raw lexer
  only lexes ENGLISH keywords ("verba clausa Anglica solum" —
  latina.h flows through the expander), so `si`/`dum`/`per`/
  `commutatio` arrive as IDENTIFICATOR. English genera (IF/WHILE/
  FOR/SWITCH) are also checked for vendored/generated code. `fac`
  excluded (no paren follows it).
- **R5 is tabs-only in this tranche.** A naive indent%4 check would
  FALSE-FIRE on the two-column alignment rule (R7): aligned
  declaration runs legitimately start at non-multiple-of-4 columns
  (`      memoriae_index  capacitas;`). Depth-correct indent checking
  must be R7-aware → tranche 2 (structural).
- **Comment-interior trailing whitespace is not detected.** It lives
  inside the COMMENTUM token's bytes, which the stream walk doesn't
  open (and which the FIX tier may never touch — Contractus). Known
  gap, recorded, revisit with R14's promotion decision at G3.
- Final-newline checks (exactly one) are raw-byte checks under the
  spatia-caudae rule name.

Found en route: **the whole silva suite had been link-broken since
the 2026-08-18 differre extraction** — the instrumenta/*.c glob
links silva_differre.c into every probatio, but its lib deps
(differentia, sigillum) were never added to compile_probationes.sh's
hand-maintained RADIX_FONTES. First suite run since then was
formator's probatio. Fixed by adding both.

First light (verbatim-verified against the hand archaeology):
piscina.c → 45 divergences: 24 vexillum-59 rows (12 banners × 2
rules — piscina is the 59-width nonconformist), 6 trailing-space
lines (archaeology had found 4 of them by eye), 14 lines > 72, and
1 file-ending blank line (never noticed by hand). Zero false fires:
no tabs, no R6. chorda.c → 132 tab rows (the mid-file tab switch at
chorda_concatenare), 5 trailing, 42 long. Probatio 33/33.

Named doors: tranche 2 = structural rules (R1/R2/R3/R4/R7/R8/R9/
R13 need the tree; selector engine silva_quaestio may carry several
— "definitio-functionis" one selector each). R15 include-order =
lint-forever candidate. Census over lib/ + include/ = the G1
acceptance payoff.

## 2026-08-19 — exclusiones + R16 confirmed at 72

Pins file silva/probationes/fixa/formatoris/exclusiones.txt
(examen-pins pattern, via<TAB>causa, named cause per row; leading
"./" on input viae normalized away). Exemption always announced on
stderr — silent gate ≡ dead gate. Born with 20 rows: biblia_dr.c
(data) + arbor/arbor2/lapifex frozen generations (Fran 2026-08-19).
Threshold experiment from census data (no re-run needed — inventum
column carries actual lengths): >72 = 6356, >79 = 2854, >80 = 2556.
60% reduction available at 80, but kept 72: the norm is the target,
not the count, and tranche-2 structural fixes (params one-per-line)
will shrink old long lines anyway. Census with exclusions live:
14286 in 244/291 files (tabs 7772 / long 5225 / banners 1237 /
trailing 51 / keyword 1). Hook note: mid-edit examen flagged
"variabilis inutilis" on the exclusiones declaration — intermediate
state between the declaration edit and the use edits; rebuild under
-Werror (unused = error) is the authority and passed.

## 2026-08-19 — tranche 2a: structural rules (R1/R2/R3/R4/R8)

Tree pass added to the machine: silva_contextus + latinam_addere +
silva_c89_parsare_cum_contextu per lint; parse failure => stream
rules only (fragments stay lintable, lint never crashes). LAW OF
POSITIONS: tree tokens are EXPANDED and carry def-site lexical
fields (a `principale` definition's name token is `main` pointing
into latina.h) — every direct token position check goes through
silva_token_radix; node spans use extensionem_lineis (resolves
internally). Pinned by the principale probatio case. alioquin-si
chains thread the chain-head node (caput) so else-if braces measure
against the chain start, not the inner si column. R8 exempts a sole
bare-void parameter (detected structurally: 1 specifier ==
TYPUS_PRIMITIVUS with 1 VOID token, no declarator). R3 applies to
CORPUS only — struct/union/enum braces stay on the declaration line
by design. Skipped with cause: FAC_DUM corpus, bare nested blocks,
prototypes (R1/R2/R8 on DECLARATIO = tranche 2b).

Doors: per-file "arbor absens" loud marker when parse fails (today
it silently skips structural rules; coverage argument for now =
examen parses this whole corpus daily with pinned exclusions, so
house files parse); prototype checks; R7/R9/R13 alignment tranche.

Census (tree live): 19549 in 259/291 — parametra-singula 2284
(the abandoned fiddly rule), spatium-definitionis 2029 (2000 def /
29 call), bracchia-allman 829, custos-una-linea 114,
typus-in-linea-sua 7 (ALL seven = one cluster of flat one-line
predicates in lib/json.c:1021-1051). Probatio 52/52; suite 40/40;
piscina.c still exactly 45 (zero structural — the exemplar is
conformant under its own executable rules).

## 2026-08-19 — tranche 2b: alignment rules (R7/R9/R13-partial)

R7 columnae-binae in lint form: per run, with cb = one-past-type-end
— common cb across members, gap = 1 + max stars, first star at
exactly cb, names at exactly cb + gap. Runs: struct/union member
lists, leading local-declaration blocks, multi-line param lists
(single-line param form is R8's finding, R7 stays silent). Exempt
per member WITH CAUSE: function-pointer declarators, post-star
qualifiers, multi-declarator lines, multi-line specs. R9: clusters
of simple single-line assignment statements (blank line or any
other statement breaks); `=` at max(lhs_end)+1; single-statement
clusters silent (spacing there = R10's future job). R13 partial:
2-blanks-before-banner / 1-after (banner = comment containing an
=-run >= 10) + 3+ blank collapse; between-functions and
after-locals policy = doors (comment interplay needs design).

TWO CALIBRATION FINDINGS from piscina first light (45 -> 71, all
new fires verified real):
1. **Minimal vs uniform padding**: the piscina->primus stanzas are
   uniformly aligned but ONE column wider than minimal (op at
   max_cb+2); the alveus stanzas use minimal (max_cb+1). Rule as
   coded = minimal. Fran must decree before fix tier.
2. **Cluster aggressiveness**: the rule as decreed aligns ANY
   adjacent simple assignments (`alveus->offset = ZEPHYRUM;` +
   `alveus = alveus->sequens;` must co-align) — the hand never did
   this in loop bodies. May want narrowing (e.g. only stanzas of
   >= 3, or same-LHS-prefix). Census: 4308 fires — review sample
   before -scribere.

**R7's big limitation, named**: the lint parse has NO include
resolution — unknown typedefs (memoriae_index, Alveus, chorda...)
make members ambiguous and runs exempt-skip. piscina.c shows ZERO
columnae-binae despite being the alignment exemplar. Door:
praebere-based header resolution (legati-style prepared contextus)
= the single highest-value lint upgrade. Even blind, corpus-wide
R7 = 3863 fires (known-type runs only).

Census 2b: 28503 total (columnae 3863 / aequatio 4308 / intervalla
783 on top of 2a's numbers). Probatio 67/67.

## 2026-08-19 — praebere upgrade: include resolution (R7 sees everything)

formator_lint gained a contextus param (NIHIL = internal
latina-only; probatio path). CLI builds ONE long-lived contextus:
latinam_addere + praebere every *.h in include/, silva/fontes/,
silva/instrumenta/ (basename as via — matches #include "x.h"
spelling; contextus copies content, reused across all parses).
CONSEQUENCE HANDLED: the tree now carries header-fons nodes —
span checks self-filter via extensionem_lineis(fons_princeps),
and all direct token checks go through _principalis() (fons_index
== fons_princeps) so header content is never flagged under the
linted file's name. Differential probatio: a praebere'd fictional
typedef makes a broken run fire 2 columnae divergences that were
exempt-invisible without it. Also learned the hard way (again):
hardcoded string-literal lengths lie — 121 vs the real 122
truncated the final newline and fired linea-nova-deest; strlen.

VALIDATION: piscina.c = ZERO columnae fires with full resolution —
the alignment exemplar passes its own executable rule completely
(71 total unchanged, all real drift). Census: columnae 3863 ->
6212 (+2349 newly visible), total 34584. Cost: 35s / 291 files
(~120ms each with full header expansion) — RP not needed for lint
tempo.

## 2026-08-19 — lint tail: R10/R11/R15/R13-inter — LINT TIER COMPLETE

R10 operatores: tree checks on BINARIUM (one space both sides,
same-line guards so author breaks don't false-fire), ACCESSUS /
CONVERSIO / UNARIUM tight; ASSIGNATIO excluded (R9's domain);
comma = stream check (no space before, space-or-newline after;
extra spaces after tolerated — table alignment). R11: statement
spans collected in tree walk (SENTENTIA/DECLARATIO/REDDE + si/dum/
per condition regions); continuation lines must start >= ca+4
(>= not ==: exact-column would war with R7's right-aligned params;
strictness is a G2 decision); operator-leading enforced by flagging
binary ops that END a line inside a span (STAR/AMPERSAND excluded —
unary-ambiguous at stream level; '=' excluded — house allows it at
line end). R15: stream walk of #include lines — postulata first,
house before system. R13-inter: adjacent top-level definitions need
exactly 1 blank; commentarium_ducens folds doc comments into the
function's start; banner-in-gap pairs SKIP (banner spacing is R14/
R13-banner's business).

Two false-fire classes caught and fixed at first light: (1) struct/
union/enum-defining and congeries-initialized DECLARATIOs are
block-shaped, not wrapped statements — exempted from R11 spans
('};' at column 1 was firing); (2) banner-separated functions —
commentarium_ducens only sees tightly-above comments, so the
banner-in-gap check scans the source REGION between functions.

piscina.c: 71 unchanged — ZERO fires from all four new rules (its
operators/continuations/includes/function-spacing are clean); all
positives pinned by deliberate-violation probatio cases (91/91).
Census FINAL LINT: 42555 — operatores 4466, continuatio 3341,
intervalla 940, ordo-inclusionum SEVEN (imago.c = vendored stb,
selectio.c, capsula_caudae.h). 16/16 rules live. Next: G2
-scribere.

## 2026-08-19 — G2a: the emendatio engine (-scribere) + stream fixes

THE ARCHITECTURAL DECISION: detectors ARE the fixers. Each detector
that fires records the whitespace edit curing it (`_addere` then
`_emendare`: up to 2 spans in ORIGINAL coordinates + insert text,
FormatorEmendatio[II] on the divergence). One analysis pass = report
AND fix; the two modes physically cannot drift. Rejected
alternative: a separate fix engine keyed off (regula, nuntius)
strings — stringly, and drift-prone. Tree-transform machinery (the
VISIO's clone/mutate/emit) stays the G3+ road for banner rewriting;
for whitespace-only v1, byte splicing with a whitespace-invariant
applier is strictly safer.

The applier is dumb and paranoid BY DESIGN (Contractus enforced at
the lowest layer):
- deletes only bytes in {space, tab, CR, NL}; violation = the whole
  file REFUSED loudly ("vitium detectoris") — never partial-applied
- inserts only spaces/newlines
- newline-bearing edits touching a preprocessor line are dropped
  (directive guard: '#' lines + backslash-continuations; splitting
  a #define would change semantics). Untested path in G2a (no
  stream rule can reach it) — G2b OBLIGATION: probatio case via
  R3/R4 on a statement-shaped macro body.
- overlapping spans: sorted descending (offset_a, offset_b), first
  accepted wins, rest dropped — the NEXT ITERATION regenerates them
  against fresh geometry. This is the composition mechanism: rules
  compose through the fixpoint loop, not through inter-rule
  choreography (R8 splits params; next round R7 aligns them).
- descending application = every span's original coordinates stay
  valid simultaneously; no reindexing.

formator_scribere: work on a piscina copy (fons untouched), loop
lint→flatten→apply to fixpoint (ITERATIONES_MAXIMAE 12), token-
series gate vs the ORIGINAL every round (non-whitespace tokens
compared by genus+valor — COMMENTS INCLUDED, so comment bytes are
gate-checked too). Refusals return the untouched original with a
named querela. applicatae==0 with pending edits (all directive-
dropped) = honest convergence: residuals stay reported by lint.

DECREE (Fran-visible default): tabula = IV spatia.

Wired in G2a (stream tier): R5, R12 ×3, R6 ×2, R10-comma ×2,
R13-collapse. The collapse edit spans from the 4th newline token of
the run to the last (keeps 2 blanks); interleaved trailing spaces
on blank lines are separate R12 edits — overlap-drop + iteration
handles the collision (verified in the EOF case where R13-collapse
and R12-final-blanks overlap: one applies, the other converges next
round).

First light, real files (scratch copies): chorda.c (the tab
monster) + piscina.c fixed in ONE pass each; re-run = no-op
(idempotence on real files); differre = 8 + 7 MUTATA all
[cosmetica] — THE DOGFOOD GATE PASSES ON REAL FILES; examen ACCIPE
both. 3 tab-lines survive in chorda.c = comment-INTERIOR bytes
(inside the COMMENTUM token) — lint never flagged them, fix never
touches them; lint↔fix agree on the Contractus boundary. Probatio
117/117 (portae section: differre-cosmetica assert via
silva_differre_classificare_textus — the differre MACHINE is
C-callable from the probatio, no shelling out; fidelitas
lexare→emittere byte-exact; idempotence). Suite 40/40.

Gotchas hit: credo has no credo_piscina() (pass piscina to
helpers); unsigned-subtract-then-compare flagged by examen in the
directive-continuation scan (restructured to compare-first);
qsort needs stdlib.h.

Doors: G2b = tree-tier emendationes (R1/R2/R3/R4/R8 + R13-inter +
banner spacing) + the directive-guard probatio case; G2c =
alignment emendationes (R7/R9/R10-tree/R11 incl. the two-span
operator-move fix). Corpus rollout AFTER G2c, supervised: run
-scribere over lib/+include/+silva/ with differre + examen + full
suite as the acceptance stack, library by library.

## 2026-08-19 — G2b: tree-tier emendationes (R1/R2/R3/R4/R8 + R13-inter + vexilla)

Contract refinement forced by R4: emendationes now carry `tolerans`.
JOIN spans (R4 body-join, R2 gaps, R13-inter blank-trims) can
legitimately contain a comment or backslash-continuation — those
drop QUIETLY as unfixable (divergence stays reported). Strict spans
(padding, line-leading indentation, token-own bytes) keep the loud
whole-file refusal, which stays reserved for actual detector bugs.
Two wrappers (_emendare / _emendare_tolerans) over _emendare_ipsum;
G2a call sites untouched.

THE FINDING OF THE DAY — radix is USE-SITE: my directive-guard
fixture (R4 on a statement-shaped `#define` body) came back
CONFORMIS from the real CLI, and the reason is architectural:
`silva_token_radix` follows the INVOCATIO chain, so every token of
a macro-expanded statement resolves to the invocation line. The
expanded tree collapses to a single source point; multi-line macro
bodies can never fire R4/R3. (The token's OWN linea/columna fields
are def-site — that's the latina.h trap already known — but radix,
which all position checks go through, is use-site.) Pinned as its
own probatio case. The directive guard is instead reachable — and
now pinned — via banner-post spacing trying to insert '\n' at
column 1 of an '#include' line: dropped, file intact, residual
divergence. Guard is deliberately conservative (drops even
line-start inserts on directive lines, which would actually be
safe); recorded as acceptable coverage loss.

Fix mechanics worth remembering:
- R3 own-line move = pure INSERT of '\n' + (owner_col - 1) spaces
  before the brace; the orphaned space before it becomes trailing
  and R12 cleans it NEXT round. Same lazy-composition trick
  everywhere: no fix cleans up after another fix.
- R8 splits ALL params of a one-line list in a single round
  (insert-only edits never overlap); provisional indent IV until
  G2c's R7 alignment.
- R13-inter excess-blanks and R13-collapse can both fire on the
  same gap (3+ blanks between functions) — overlap-drop plus
  iteration converges: collapse wins round 1 (higher offset,
  descending order), inter trims to one blank round 2.
- Banner ante/post inserts guard on nl_cursus > 0 (banner sharing
  a line with code = unfixable, no emendatio).

json.c first light: 213 emendationes / 3 iterations; R1 7->0
(the flat predicates at 1021-1051 now wear full house form),
R8 96->0, R2 69->0; longitudo 66->63 as a side effect of param
splits. differre: 69 MUTATA all [cosmetica]. examen ACCIPE. Rerun
stable. Probatio 144/144; suite 40/40.

Doors: freestanding non-banner comments between functions make
R13-inter's blank count include comment lines (lint-level, G1
behavior) — the tolerans trim just refuses there, so it is safe
but unexamined; revisit with the comment query layer. G2c next:
R7 columns / R9 clusters / R10-tree spacing / R11 continuation
(incl. the two-span operator move).

## 2026-08-19 — G2c: alignment emendationes — FIX TIER COMPLETE

R7 (three fixes per member: type-shift is an insert at the
specifier start — R7Membrum gained `ca`; star pull-left; title
push-pull GUARDED on the star being correctly placed, else the
delete span would contain the star and trip the strict guard —
iteration handles it after the star moves), R9 push-pull to max+2,
R10 tree gaps (all tolerans), R11 under-indent (line-start insert)
+ the operator-move: TWO strict spans from one divergence (insert
newline+indent before the op; join op with next line's first
token), withheld as a pair when a comment or '\' continuation
obstructs — the applier has no transactions, so the DETECTOR is
the transaction boundary.

TWO G1-ERA LATENT BUGS, both exposed the moment lint's claims
became physical edits:

1. **The parametra LISTA carries comma TOKENS.** _valor_nodus on
   the comma gives NIHIL and _parametra_ordinem_censere treated
   that as fatal — R7-params has NEVER judged a multi-param
   function since 2b (R8's loop always skipped non-nodes, which is
   why R8 worked). Symptom: identical member shapes fired as a
   struct but were CONFORMIS as params. Census +2229 columnae.
   Lesson: silva grammar lists interleave separator tokens with
   nodes — every lista consumer must skip VALOR_TOKEN elements.

2. **The R11/R1 prototype war.** A two-line prototype (specifiers
   line, name at column 1) is a DECLARATIO → R11 span → its
   R1-correct shape counted as an under-indented continuation.
   Lint had silently miscounted this for the whole census era
   (~2110 rows); the FIX physically indented json.c's
   _scribere_valor prototype by 4, and THE DIFFERRE GATE CAUGHT
   IT — the reshaped prototype broke unit pairing (ADDITA+REMOTA
   of the same titulus instead of MUTATA). Function-declarator
   declarations now join forma_bloccalis in the R11 exemption.
   The dogfood twist paid for itself: a gate of a different KIND
   caught what token-series + whitespace-only + examen all passed.

Census at final G2c semantics: 42,674 (continuatio 3341→1231,
columnae 6212→8441 — the two corrections visible as numbers).

First light: piscina.c 35/2 — the R9 calibration points from 2b
resolved exactly as decreed (alveus stanzas minimal→max+2, the
loop-body pair alveus->offset/alveus co-aligned aggressively);
json.c 535/4; chorda.c 478/3. Residuals on all three = ONLY
longitudo 119 (total unchanged — no net new long lines from
alignment) + vexillum 56. differre 17/42/72 all [cosmetica],
examen ACCIPE ×3, rerun stable ×3. Probatio 167/167 (six new
sections incl. the two-span move pinned at 3 iterations and the
R8→R7 composition reproducing the _allocare_interna exemplar
form byte-for-byte). Suite 40/40.

Doors: R7 has no 72-exception (R9 does) — watch rollout for
alignment pushing lines long (three-file trial: zero net). Debug
note for next time: the machine has no stdio — printf debugging
needs a temporary include.

FIX TIER COMPLETE. Next: supervised corpus rollout
(library-by-library, differre + examen + suite as acceptance),
then G3 doors (banners, R14 promotion, prototype R1/R8 coverage,
comment-interior whitespace).

## 2026-08-19 — R17 catena-logica (padded-paren chains + comparison runs)

Born from Fran reviewing batch one: the R11 operator-move had
flipped json.c's trailing-&& chains to leading-at-ca+4, and the
convergence turned out to be exact — the padded-paren form Fran
chose is the machine's existing output PLUS three spaces after
'(' (for si, paren+1 == ca+4 precisely). The rejected alternative
(operand-aligned, operator at paren-2) would have contradicted
R11's minimum and needed a carve-out.

Shape: multiline &&/|| chain directly under a si/dum condition
paren → first operand paren+4, leading operators paren+1,
two-char-comparison branches align ops at max(lhs cb) + I (ONE
space after the longest LHS — Fran's exemplar; deliberately
tighter than R9's + II). Claims mechanism: every participating
comparison registers its operator position in ambitus->catena_ops
and R10-ante DEFERS (same '=' division as R9/R10; without it the
aligned form would flag "spatium unicum ante"). Claim happens even
when C is silent — already-aligned ops need the deferral too.
Order matters: _catenam_censere runs BEFORE the walk descends into
the condition (si: top of _si_censere; dum: case block before
liberi recursion), so claims precede R10's checks.

Scope decisions: si + dum only. per DEFERRED WITH CAUSE (its paren
wraps the init clause first — paren+4 anchors nothing sensible);
parenless chains (redde) stay on R11 ca+4; single-line conditions
untouched (no rewrap); nested parens anchor recursively (excluded
from the outer spine walk). Mixed chains: padding + operator
placement apply, only comparison branches align. First-branch
participation lags one round (it sits at paren+1 until A pads it
to paren+4) — fixpoint absorbs it, exemplar converges in 3.

Applied to the rolled-out trio: chorda +1 (a substituere guard),
json +11 (the aspicere chains — now byte-identical to Fran's
exemplar), piscina zero. differre 1+2 MUTATA all [cosmetica],
examen ACCIPE, suites green. Probatio 177/177 (target-form
self-conformance pinned — proves the R10 deferral; negatives:
single-line tight, multiline non-chain unpadded).

## 2026-08-19 — evolutio II + the AMBIGUUS oscillation

Batch two (10 foundation files) produced the first real
non-convergence refusals: tabula_dispersa + iter_directoria,
"punctum fixum intra XII iterationes non attinctum". Per-iteration
tracing showed a clean 2-cycle in 'operatores': insert-a-space
rounds alternating with delete-a-space rounds at the same sites.
Minimal repro: `(i32)-I`.

ROOT CAUSE: the construct parses as an AMBIGUUS node (conversio
vs binarium readings — the oracle does not decree, even though
i32 is a lexicon typedef; lexicon-type visibility for ambiguity
resolution is a SILVA-SIDE question, named below). The formator's
generic liberi recursion descended into ALL interpretationes —
walking BOTH readings and enforcing both spacings: the binary arm
wants `(i32) - I`, the cast arm wants `(i32)-I`, each round the
arm disagreeing with the current text fires. Perfect oscillation;
the 12-round breaker caught it and refused honestly (files left
untouched — the refusal machinery paying for itself).

FIX: _nodum_percurrere cuts at SILVA_C89_GENUS_AMBIGUUS — a
region the tree itself cannot decide is a region whose spacing
cannot be judged. Conservative coverage loss, pinned in probatio
(`(ignotum_genus)-I` stable + untouched). Convergence restored:
tabula_dispersa 131 em/4 it, iter_directoria 220/3.

DOORS (silva-side): (1) why does the oracle keep cast-vs-binary
ambiguous when the type IS a lexicon typedef? Resolving would
upgrade formator coverage in those regions (currently unjudged).
(2) differre desideratum: a DIRECTIVE unit appearing twice (both
arms of #ifdef) whose only change is indentation classifies as
ADDITA+REMOTA instead of MUTATA[cosmetica] — filum.c's
<sys/stat.h> tab→space case; byte diff is pure whitespace, token
gate proved identity, but the differre gate needs a manual glance
when directives move columns.

Batch two results: xar 836/4, filum 807/4, nuntium 598/3,
chorda_aedificator 441/3, iter_directoria 220/3, tabula_dispersa
131/4, similitudo 91/4, tempus 45/2, credo 30/3, internamentum
18/3. differre all [cosmetica] (filum's directive rows explained
above); examen ACCIPE ×10; root suite 136/137 (planta_lectio
pre-existing); silva 40/40. Probatio 179/179.

## 2026-08-19 — lexicon-type visibility: the root of the ambiguus class

Fran spotted `i8 * novum_datum;` in nuntium.c — frozen damage from
the PRE-cut era: `typus * titulus;` in statement position is the
classic declaration-vs-multiplication ambiguity, and before the
AMBIGUUS cut the walk enforced both readings (R10's binarium
spacing on the multiplication arm inserted the spaces; the
converged hybrid satisfied both arms). Post-cut, lint went blind
to the site and froze it. Damage census: 41 sites across 6 files
(chorda 14, filum 11, xar 7, chorda_aedificator 4, json 3,
nuntium 1) — ALL i8*/FILUM* i.e. LEXICON-typed.

ROOT CAUSE (two mechanisms conspiring): (1) silva_parsare's
lexicon path processes DIRECTIVES ONLY ("Reliqua lexici
abiciuntur" — documented), so latina.h's typedef lines never
reach the tree and never register with the oracle; (2) the disk
include/latina.h IS praebere'd by the CLI, but the lexicon's
'#define LATINA_H' survives (it IS a directive), so the include
guard suppresses the real body. json's own JsonValor* locals
aligned fine all along because FILE-LOCAL/header typedefs
register normally — only latina primitives were invisible.

FIX (formator-side, faithful): formator_latinam_praebere — the
CLI praeberet latina.h with the guard NEUTRALIZED (the #ifndef/
#define pair and the LAST #endif are BLANKED, not deleted — line
positions and provenance preserved). The transitive
#include "latina.h" in every house file then parses the real
typedefs; identical macro redefinitions are legal C89 and silva
tolerates them. Differential probatio pin: a guarded mini-latina
praebere'd through the helper makes 'typus * x;' fire TWO
columnae-binae (star + title) where the suppressed form yields
ZERO (ambiguus-skipped).

Repair pass over all 13 formatted files: chorda 48, json 17,
xar 13, chorda_aedificator 7, nuntium 3, filum 2 emendationes —
the nuntium glitch site now reads 'i32  nova_capacitas; /
 i8* novum_datum;' (the exemplar two-column form). REMAINING
CLASS: FILUM = #define FILUM FILE — a SYSTEM typedef (stdio.h is
never parsed), genuinely unresolvable → those sites stay
ambiguus-skipped; the 10 frozen filum.c sites hand-repaired
(tight star restored), stable under re-scribere. DOOR: system
typedef visibility (FILE/DIR...) — a praebere'd systema-mini or
examen's systema_posix.h sections could feed the oracle; needs
its own design pass. The SILVA-side door stands too: lexicon
content flowing (typedefs, not just directives) would fix this
at the true root for every silva consumer, not just the
formator.

Gates: examen ACCIPE x13; differre all [cosmetica] (filum's
directive rows = the known classification quirk); root suite
136/137 (planta_lectio pre-existing); silva 40/40. Probatio
185/185.

## 2026-08-19 — R9 refined: equal-width clusters + cluster-level 72 guard

Fran found nuntium 647-654 ragged: valor-accumulator lines mixing
one and two spaces before the operators. Archaeology: the hand's
original was UNIFORMLY single-spaced; R9's per-member 72-exception
aligned the short lines to max+2 and silently skipped the long
ones — the exception destroyed the very uniformity the rule
exists for. Two refinements, both from Fran's principles:

1. Equal-width LHS → operator at max+1 (single space). The double
   space marks padding; identical widths have none. Corollary:
   equal-width clusters insert nothing and can't overflow.
2. The 72 guard is now CLUSTER-level: any member would overflow →
   the WHOLE cluster drops to minimal (own cb+1 each). No partial
   alignment, ever. The overflow predicate uses
   (fines - op) + recta - 1, which is invariant to current
   spacing — an earlier draft that measured current line length
   would have oscillated (align→overflow→unalign→fits→align).

Probatio 189/189 (uniform tighten, cluster-fall at the exact
72 boundary, and the R10-interplay expectation updated — a/d are
equal-width so they tightened to single space). Refinement pass
over the 13: nine files touched (piscina 6 / chorda 15 / json 5 /
xar 16 / credo 10 / tabula_dispersa 11 / chorda_aedificator 11 /
nuntium 21 / tempus 34 emendationes); the nuntium block is now
byte-identical to the hand's original. Gates: examen ACCIPE x13,
differre cosmetica-only (filum's known directive rows), root
136/137 (pre-existing), silva 40/40.

## 2026-08-19 — R9 covers declaration initializers (the G1 door closes)

Fran found nuntium 770-771: `s64 shifted = ...` / `s64 mask = ...`
with unaligned initializer `=`s — and correctly guessed the rule
gap. R9 clustered assignment STATEMENTS only; R7 governed the
type/name columns of those declarations (both s64, so nothing
moved); the initializer operators were governed by NOTHING (R10
excludes '='). This was the named G1 door "decl-with-init `=`
alignment in R7 runs".

Closed: DECLARATIO elements with a single DECLARATOR_INITIATUS
(single-line, operator on the declarator's line) now feed the R9
cluster — LHS cb = one-past the DECLARATOR (array suffixes ride,
as in R7), so the equal-width and cluster-fall refinements apply
unchanged. Feeding happens in the R7 leading-run branch (which
`perge`d past the R9 section entirely — the aeq append/flush
logic is now the `_aeq_pascere` helper both paths share).
Decisions: R7-EXEMPT decl-inits still feed (the '=' can align
even when the columns can't); uninitialized declarations BREAK
the cluster (conservative; door if corpus says otherwise);
multi-declarator lines break; decl-init + adjacent assignment
statement co-cluster (the aggressive decree crosses the
declaration/statement boundary — pinned).

Composition note: R7 moves names, which changes declarator cb
values — the fixpoint loop absorbs the interaction (R7 settles,
then '='s align; no choreography).

nuntium 770-771 now reads `shifted  = / mask     =` — exactly
Fran's sketch. Six files took small passes (chorda 8, credo 9,
json 7, similitudo 6, nuntium 4, internamentum 2). Probatio
193/193; examen ACCIPE; differre cosmetica-only; root 136/137
(pre-existing); silva 40/40.

## 2026-08-19 — evolutio III: lib/ wholesale + five engine hardenings

Batch three (all of lib/) opened with 98 written / 23 refused.
The refusals decomposed into FIVE distinct defect classes, each
now fixed and (where synthesizable) pinned:

1. **Inline block-shaped specifiers poisoned column runs** —
   `unio { ... } u;` in a locals block: the locals run collected
   it with cb = one-past '}' and shifted neighbors toward it
   forever, while _membra_censere "aligned" its INTERNAL members
   sitting on ONE line, growing the target each round (the
   js_lexema chase, +7/round). Fixes: STRUCTURA/UNIO/ENUMERATIO
   specifiers exempt from _membrum_colligere; same-line members
   NEVER co-align (guards in _membra_censere, _corpus_interius,
   _parametra_ordinem, _aeq_pascere — R7/R9 are cross-line rules).
2. **Macro-expanded LHS extents lie** (coloratio 999:
   `coloratio_index(...) = X` — radix collapses to the invocation,
   measured cb lands mid-call) — R9's pull-delete reached real
   code and the STRICT guard refused the file (correctly!).
   All four alignment pulls (R9, R7 stella, R7 titulus, R17-C)
   are now tolerans: a lying extent or an interposed comment
   drops the edit quietly, divergence stays.
3. **Macro-expanded OPERATORS produce garbage geometry** (flatura:
   FLATURA_SCRIBERE_BITS(...) internals firing R10 with -21
   gaps) — operator extraction for R9/R10/R17 switched from
   _valor_radix to _token_fons: only SOURCE-level operators are
   judged. Latina keywords are unaffected (their checks anchor on
   keyword/paren tokens via radix, which is honest 1:1 there).
4. **Self-chasing title columns** (flatura 1811: 245→267→289…)
   — a member whose measured type extent crossed its own title
   (macro lie) made cb_max grow with every title pad. Sanity
   invariant in _membrum_colligere: the type must END before the
   title starts (titulus->columna >= cb), else exempt.
5. **The banner-banner one-byte war** (persistentia: header
   comment with a 50-run IS a banner; post-rule wanted 1 blank,
   the next banner's ante-rule wanted 2 — eternal insert/delete
   of one newline). Decree: when the next token is itself a
   banner, the ANTE rule owns the gap.

After hardening: **122 files written, ZERO refusals, second
sweep fully stable**. Gates: differre cosmetica-only on 109;
13 files show ADDITA/REMOTA pairing quirks — ALL verified pure
whitespace by the independent oracle (whitespace-stripped
sha both sides identical). imago.c examen REICE is PRE-EXISTING
(vendored stb; red on HEAD too). ROLLOUT CHOREOGRAPHY LESSON:
mtime-gated tool binaries (natura_glossae/canones) go STALUS
when their sources are formatted — ./tools/natura_struere.sh is
part of the rollout dance; expect the same for other
self-gating tools. Root suite 136/137 after rebuild
(planta_lectio pre-existing); silva 40/40. Probatio 198/198.

Doors: node-level source-purity query (silva-side: "is this
subtree macro-free?") would replace the per-rule guards with one
clean cut, same shape as the AMBIGUUS cut; differre directive/
unit pairing on whitespace-only reshapes (13-file quirk class).

## 2026-08-19 — evolutio IV: include/ (143 capita, zero recusationes)

The header batch ran clean on the first pass: 143 written, zero
refusals, second sweep stable — the five hardenings held on
prototype-heavy terrain with nothing new to fix. latina.h PINNED
in exclusiones (row 21): its macro table is tab-STOP aligned by
hand; naive tab→4sp would shred the expansion column, no rule
exists to re-align macro tables, and the lexicon datum is
generated from it. Ten files show differre pairing quirks — all
examen-ACCIPE and all verified whitespace-pure by the stripped-sha
oracle (prototype reshapes break pairing keys more often than
function bodies; the filed differre desideratum covers it).

Choreography executed: ./silva/amalgamare.sh (ALSO retired the
batch-three debt — the amalgam vendors lib sources and had been
stale since lib/ was formatted; VERIFICATUM standalone + hospes
37/37 + nm 0), ./tools/natura_struere.sh, both suites (root
136/137 planta_lectio pre-existing; silva 40/40).

Formatted so far: lib/ complete (batches 1-3) + include/
complete (batch 4). Remaining terrain: silva/fontes +
silva/instrumenta, apps/, tools' C sources — per decree.

## 2026-08-19 — prototype coverage (the 2a door closes; Fran's arx_caeli read)

Fran read arx_caeli.h and asked for right-aligned parameter types
on declarations — the tranche-2a door (R1/R2/R8/R7-params were
definition-only while the DECREED rules always said "definitions
AND prototypes"). Closed by refactoring: _functionis_caput_censere
(R1/R2/R8) + _parametra_ordinem_functionis (R7-params) are now
cores over the FUNCTIONIS declarator node; _definitionem_censere
is a thin wrapper; ROOT-level DECLARATIOs with a true prototype
shape get the same checks. "True prototype" = single declarator
resolving through _declarator_functionis whose INTERNUM is
DECLARATOR_TITULUS — function-POINTER declarators have internum
PARENTHESIS and stay exempt (R7's standing rule). Prototypes are
checked ONLY from the root walk, so local prototypes inside
function bodies are untouched (their R1 "column 1" demand would
be wrong there).

Sweep: 177 files retouched (headers + the .c files' interior
forward declarations), zero refusals, stable, purity oracle
clean, examen ACCIPE (imago pre-existing). arx_caeli.h now:
`               ArcCaeli* arc,` / `    FunctioLinkCallback  callback,`
— the _allocare_interna form on declarations. Amalgam
re-verified; natura rebuilt; root 136/137; silva 40/40.
Probatio 207/207 (arx_caeli-shape bytes derived and pinned;
single-line prototype split; fn-pointer-typedef negative).

## 2026-08-19 — formator-custos: the report-only post-edit hook

PostToolUse (Write|Edit) on .c/.h: lints the written file and
pushes FIXABLE divergences (count + first 6 rows) with the
one-command nudge "./silva/formator.sh <via> -scribere". Silence
discipline: lint-forever rules (longitudo/vexilla/ordo) are
FILTERED — piscina.c carries 14 long lines forever and nagging
each edit would teach ignoring. Pinned exclusions are silent via
the CLI itself (exempta → stderr, no rows). Warm cost ~0.6s
(formator.sh's mtime .o cache). PLANTED FAULT at birth verified
three ways: fires on unformatted terrain (silva_lexema.c, 270
fixable), silent on formatted-with-lint-residue (piscina), silent
on pinned (biblia_dr).

## 2026-08-19 — geometria_fida adoption: the R2-call false-positive class

Silva shipped the provenance queries (5b3ddab: est_fons_purus +
geometria_fida, born from the evolutio-III door). The very probatio
testing them provided the live case: being full of CREDO_* macro
invocations, it drew 46 FALSE spatium-definitionis rows the moment the
post-edit hook linted it. Mechanism: a call INSIDE a macro body has all
its tokens collapsed to the invocation site — the callee "ends" at
invocation-column+len while the paren claims the invocation column
itself, so the measured gap goes negative (-15). The evolutio-III
hardening had assumed parens were 1:1-honest via radix — true for
latina keyword parens, false for parens inside macro bodies; lib/ and
include/ never fired it loudly because their residual rows were
silently unfixable (tolerans dropped the edits).

Fix in _vocationem_censere: apertum through _token_fons (a call paren
must be SOURCE to judge call spacing) + the callee subtree gated by
silva_nodus_geometria_fida — the first bespoke-guard replacement by
the silva query, exactly as the door promised. TDD: probatio section
"vocatio in corpore macri" pinned first (macro-body call = 0 rows,
honest spaced call in the same fixture still fires — no over-cut).

Gates: probatio 210/210; silva 41/41; corpus census 6,499 -> 6,491
with longitudo/vexillum byte-identical — EIGHT of the 102 "honest
unfixable" residuals across lib/+include were actually this false
class (zero spatium-definitionis rows remain). Lint-only rule change:
removes judgments, cannot create edits — formatted corpus fixpoint
undisturbed by construction.

Remaining adoption door: the other _valor_radix anchor sites (statement
keywords/braces) survived the whole lib/include corpus clean; if the
probationes rollout (CREDO-dense terrain) fires new classes, the cheap
general design is a per-parse LYING-SITE set (one linear pass over
parsura->lexemata marking sites claimed by >=2 distinct tokens; then
per-token honesty is O(1)) rather than per-node subtree walks.

## 2026-08-19 — CLI: lint mode gets the per-file opus piscina (silent batch death)

Exposed by the lexicon-typedef preload, but pre-existing: lint mode
parsed every file into the LONG-LIVED piscina (context + praebere'd
headers + all parses, never reset) while only -scribere mode had a
per-file opus piscina. At ~230 corpus files the arena exhausted and
the process died SILENTLY mid-batch. Diagnostic signature worth
remembering: the census dropped ONLY in lint-forever classes
(longitudo -135, vexillum -93, fixable classes identical) and the
per-file diff showed every vanished file was alphabetically AFTER a
point (include/t*-z*) — a census drop in lint-forever classes means
files dropped out of judgment, not improvement. Fix: both modes now
run inside a per-file opus piscina (file text included); the
long-lived piscina serves the context alone. Corpus census restored
byte-identical (6,491 / 246 files).

Also confirmed: the lexicon preload produced ZERO new divergences on
the formatted corpus — the rollout had already run with types visible
via formator_latinam_praebere, which the root fix now makes redundant
(retirement is the follow-up).

## 2026-08-19 — formator_latinam_praebere RETIRED (the bandage comes off)

With lexicon typedefs preloaded by silva itself (94899b2), the
guard-blanking praebere existed only to do worse what the root fix
does everywhere: ~70 lines of machinery (_lineam_vacuare,
_linea_congruit, the guard scanner) deleted; the CLI praebet
latina.h like any other header (its body correctly guard-suppressed
under #include — types come from the lexicon). The "latina sine
custode" differential test became "typus lexici (radix sanata)":
a plain-C typedef added via silva_contextus_lexicon_addere, same
two columnae-binae assertions. Gates: probatio 210/210, silva
41/41, corpus census 6,491 byte-identical.

## 2026-09-01 — `-scribere -intra <functio>`: scope by NAME, resolved on every parse

Roadmap item I of desideratum 01M1FMEKZG. Yesterday's emulation
(scratch copy, `-scribere` twice, splice the new functions back by
name) is now the tool. The design decision that mattered: the scope
lives in the MACHINE (`formator_lint_intra` / `formator_scribere_intra`
+ `FormatorIntra`), not in the wrapper or the CLI, because the
fixpoint loop reparses on every iteration and line numbers shift as
emendations land — a line range goes stale after round one, a
function title does not. Extents are recomputed from each parse.

Extent = [previous root node's last line + 1, this node's last line]:
the leading comment and the blank lines ABOVE a function belong to
it, which is exactly R13's own attribution (`la_effectiva`). Root
prototypes of the same title are in scope too. A divergence survives
only if its site AND every emendation line sit inside one extent;
straddlers drop (conservative: half an edit outside the scope is
never applied). Unknown name = machine-side refusal in scribere_intra
(`functio intra ignota`; the CLI names the function, exit 2);
lint_intra reports through `inventae`. A parse failure finds nothing
and therefore refuses — a scoped write on a fragment never degrades
into a raw-stream sweep of the whole file.

Gate: probatio section "ambitum nominatum" (24 assertions: scope b
leaves a byte-identical incl. its prototype; scope a leaves the
blanks above b alone; both names == unscoped write; lint rows all
inside the extents; unknown name refuses with the original text;
inventae NIHIL refuses equally). Planted fault (containment always
VERUM) → 5 red. The feature then formatted itself: 8 names in
silva_formator.c, 103 emendationes, `-delta` +0 new / −64 old
residuals inside the two renamed bodies, every hunk inside a named
function; header and CLI likewise (+0).

Found on the way:
- R13 `intervalla` counts DIRECTIVE lines as blank lines between
  functions: a `#define` block between two functions reads "N pro 1",
  and its tolerant emendation drops silently — a lint-forever false
  positive. Workaround today: keep the define block where the
  baseline had it. Real fix is a door: skip directive lines (or treat
  a directive block as a separator, like a banner).
- `-delta` site listing: identical-content keys (banner lines,
  `FormatorScriptum s;`) list EVERY site in the file for one new
  row. The multiset count is right; the sites are not. Door: mark
  shared keys ("clavis communis ×N") or list only the surplus.
- Idempotence (roadmap item IV) MEASURED: full `-scribere` on
  silva_formator.c three times — 417 emendationes on run one,
  checksum identical after runs two and three. The note behind item
  IV was editing between passes. Retired.
- A scope smaller than a function (a section inside a probatio's
  principale) still needs the scratch-copy splice. Door: block or
  section scope.

## 2026-09-01 — Custodia LXXII glomerum + fractura linearum (R16 becomes FIX)

Roadmap item II of 01M1FMEKZG, guard first. Reproduced yesterday's
"-scribere CREATES long lines" on scratch copies: legatus.c 0 → 19
(R7 pushed struct members whose TRAILING COMMENTS then crossed 72),
semantica.c 17 → 80 (R17 part A's three spaces and part C's
comparison alignment on lines carrying several operands). Both
guards were measuring the wrong thing: R17 C used the ramus extent,
R7 had nothing. Fix: a per-line length table on the walk context
(`_longitudines_metiri`, `_longitudo_lineae`; R16 now reads it too),
and the guard measures the WHOLE line plus the shift. R7: cluster
falls, one LINT-only row names the widest member ("glomus non
ordinatum: ordinatio LXXII excederet"). R17 A: skipped when it
would overflow (the wrapper breaks the line first, A applies next
iteration). R17 C: restructured into collect / decide / emit, with
R9's cluster-level `cadit` → every participant at its own cb + 1.
The predicate is spacing-independent (shift = target − present), so
it cannot oscillate. Real files after: legatus 0 → 0, semantica
17 → 14 (survivors: comment tails, `casus X: redde ...` with no
candidate, an identifier wider than the remaining width).

Wrapper: the `longitudo-lxxii` divergence carries its own emendation
when a break exists (`_fracturas_censere`, after the tree walk) —
detector IS the fixer, and the message becomes "linea nimis longa -
frangibilis" so the post-edit hook can nudge it while plain residuals
stay silent. Candidates come from the RAW token stream (macro-honest,
positions never lie): a SPATIA after a comma, after `=`, or before a
binary operator whose previous token ends an operand (so unary `-`
never qualifies). Choice: logical operators first, at the outermost
paren depth on the line, rightmost that fits; otherwise the rightmost
candidate that fits; nothing fits → leftmost (the tail is judged
again next iteration). No break on directive lines, `\`-continued
lines, lines without a candidate (a lone string literal), or when
only a comment tail crosses 72. Continuation indent: first line of
the statement → anchor + 4 (the innermost registered statement,
single-line statements now registered too, `ambitus->sententiae`);
a continuation line keeps its own indent unless the break operator
binds tighter than the operator leading the line (a `==` inside an
`||` operand steps in by 4). Argument tails stay at one level across
several breaks; `+` chains stay level. R11/R17 finish the layout on
the next iteration — composition through iteration, as designed.

Gates: 17 measured fixtures pinned in two probatio sections (guard:
struct with comment tails, chain first line exactly 72, cluster
overflow at 70 vs. its non-overflowing twin at 55; wrapper: call
args, chain, declaration init, depth preference, double break,
nothing-fits, or-chain operand, plus-chain, and the four no-break
cases). Planted faults: wrapper disarmed → 20 red; R7 guard → 4;
R17 C guard → 2. differre on both real-file writes: 0 substantiva,
0 documentaria, 141 / 313 cosmetica.

The feature caught its author: `-delta` on the machine file flagged
my new candidate struct — two 74-column comment tails and the guard
row on its own cluster. Comments moved above the struct.

Doors: tectum = anchor + 40 under deep indentation (STML's rule,
`gradus * II + XL`, floor 72) — Fran wants fixtures at deep indent
first, then the rule; `casus X:` label as a break candidate; `?:`
as candidates; a scope smaller than a function for the probatio
sections (scratch splice again).

## 2026-09-01 — Fractura yields to any rule inserting a newline on the same line

Fran asked to see a real before/after and the prototype at
semantica.c:4861 came back with a BLANK LINE between its parameters.
Cause: R8 (parametra-singula) and the wrapper both inserted a newline
at the same point in the same iteration; the applier drops
OVERLAPPING spans, but a zero-width insertion at the end of the
deleted space is ADJACENT, so both applied. Fix: `_fracturas_censere`
yields on a line where any other divergence's emendation inserts a
newline (R8, R1) — that rule acts first, the line is judged again
next iteration. Fixture `s` (root prototype > 72 with two params)
pinned; plant (yield disabled) → 1 red.

Process lesson, the expensive kind: the first attempt to land this
fix FAILED its anchor assertion (the scoped write had realigned the
function) inside a long shell script, the generator then ran the
CLI against the UNFIXED binary and pinned the defective output, and
the probatio went green. A pin generated from the implementation
proves nothing until the implementation has been seen to change
the pin — here the fix turned the pinned fixture red first, which is
the only reason the mistake was caught. Long scripts get `set -e`;
regenerated pins get eyeballed.

## 2026-09-01 — Anchor names in alignment messages (roadmap item III)

The desideratum's complaint: "columna typorum dextra non ordinata
24/25" gives a column and no rule, and I derived the rule wrong three
times before measuring it. Now the walk context carries the source
and per-line start offsets (alongside the length table), and the
four alignment rules format their message through `_nuntius_ancorae`:
`<basis> (ancora '<textus>' l.<linea><cauda>)`, where the anchor is
the member everything else aligns to (first member with cb ==
cb_maxima for R7/R9/R17 C; the statement's first token for R11) and
the tail is the rule itself (`+ II`, `+ I (aequilata)`, `: typus
latissimus`, `+ I + stellae N`, `, minimum (LXXII)` when the cluster
fell to the 72 guard). Text is a view into the source, truncated at
32 bytes. R11 takes its anchor from the raw token stream since it
runs without the ambitus.

Nothing downstream keys on message text: `-delta` keys are rule +
line content, the hook filters by rule (plus the `frangibilis`
marker, which is unchanged). Real sample from semantica.c:
`columna titulorum non ordinata (ancora 'constans SilvaNodus'
l.4210 + I + stellae 1)`. Gate: probatio section with one assertion
per rule; plant (formatter returns the bare basis) → 4 red.

## 2026-09-01 — Format at commit: `-lineae` + the pre-commit writes

Fran's step one of "formatting should not need attention": the
pre-commit now formats the functions whose lines changed and re-stages.
Pieces: `formator_extenta` (public; root definitions and prototypes
with the same extent rule `-intra` uses, so the two can never
disagree), CLI `-lineae a-b` (repeatable; resolved to function names
by extent intersection BEFORE the fixpoint loop, since lines shift and
names do not; ranges outside every function warn and are left alone;
no function at all → file untouched, exit 0), and the hook: staged
diff hunks → `-lineae`, new files → whole write, worktree ≠ index →
warn, never format (a partial commit must not drag unstaged edits into
the index). Ordering in the hook: format → examen → delta warning, so
the examen judges what is committed.

The commit that introduced it was its own first run: four files
formatted inside their changed functions, examen ACCIPE, tree clean
afterwards (the re-add leaves worktree == index).

Gates: probatio section for `formator_extenta` (four extents, broken
parse → empty not NIHIL); intra smoke V/VI (`-lineae` ≡ `-intra`;
struct lines warn); hook smoke VI–VIII on a tracked fixture
(`tools/unci-git/fumus_formae.c`, restored by checkout): appended
mis-formatted function is formatted in worktree AND index; partial
commit untouched with warning; new file formatted whole.

Fixture lesson: my first "outside any function" case used the blank
line between a prototype and the next definition — which by the
extent rule belongs to that definition. Structs between functions are
the genuine outside. The rule is right; the fixture was wrong.

Door: after the rollout (step two) every baseline is zero and the hook
can write whole files, which also covers structs and tables. Generated
files need exclusions first (`capsula_*`, tabulae, amalgams) or the
hook formats what the generator will overwrite.

## 2026-09-01 — Rollout (step two): the whole tree outside lib/include

Fran's step two. `tools/forma_evolutio.sh <dir> [-scribere]`: tracked
.c/.h under the directory minus fixa/vendor/build/amalgama/scratchpad
and `capsula_*` bundles, headers of every directory inside passed as
`-capita`, one formator invocation, then differre against HEAD per
changed file — the second oracle of a different kind. Generated files
(tabulae, latina_datum, *_generata.h, lexicon_c89, two generated
probationes, amalgama_ligator) went into the formator exclusions with
their generator named, because formatting what a generator overwrites
is churn. Commits are whitespace-only and bypass the hook (the oracle
is differre, not the examen); their hashes go to
`.git-blame-ignore-revs` and `git config blame.ignoreRevsFile` is set.

Order and verdicts: tools 26 (compile_tools.sh is STALE — it needs a
tool name and a dozen tools fail on the pre-format commit too, so the
gate was "no clang error in the formatted build"), css 11 (8/8),
materia 18 (6/6), gesta 13 (4/4), tessera 17 (5/5), saltuarius 42
(13/13), officina 45 + machinula (13/13), apps 3 (tabs → spaces; GUI
smokes not run), lint/instrumenta 2, silva 127 (suite pending),
probationes (pending). knotapel skipped: research demos in English C,
not house code. vendor skipped.

What the rollout found, in order:
- `-capita`: outside silva the CLI had only the silva header dirs, so
  every non-silva type was unknown and R7 silently skipped those
  declarations. The CLI now provides each input file's own directory
  plus explicit `-capita`.
- Oscillation, officina_machinula.c: two multi-line macros expand to
  function definitions; R1/R2/R8/R13 judged the expanded tree and
  aimed edits at the invocation line forever. The INDAGO trace
  (`FORMATOR_INDAGO=1`, per-iteration armed divergences) read it off
  in one run. Fix: definitions and root prototypes whose geometry is
  not faithful are not judged — the same `silva_nodus_geometria_fida`
  gate R2-vocatio already used. Fixture pinned, plant red.
- The differre counter in the rollout script matched the WORD
  "substantiva" in diff bodies (silva_differre.c contains
  `redde "substantiva"`); the labels are bracketed. Count labels.
- Amalgams drift silently: amalgamata_probare only compiles the
  committed artifact. After formatting officina/tessera/silva fontes,
  each needed `amalgama_excludenda_generare` (manifests were stale
  anyway) then `amalgamare.sh`. Regenerated and committed.
- zsh does not word-split `$var` in `for` — `${=var}`. Bit twice.
- The probationes pass exceeds the 10-minute foreground limit;
  background it.

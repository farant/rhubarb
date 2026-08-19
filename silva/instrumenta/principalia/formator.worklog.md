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

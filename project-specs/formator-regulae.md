# Formator — Regulae Formandi (v1)

Status: DECRETAE 2026-08-19 (Fran + Claude, conversatio directa).
Derived by archaeology from the hand-formatted stratum (lib/piscina.c,
lib/chorda.c early sections) cross-checked against recent practice
(lib/js_lexema.c) and a codebase-wide banner survey. Exemplars below are
verbatim corpus bytes unless marked otherwise.

This document is the contract for the **formator** (autoformatter riding
on silva) and its lint mode. It precedes the RP performance work by
explicit decision (parcum 01M0BGJXEM): the formatter exercises trivia
mutation + emission — the exact surface the performance changes would
touch — before any representation changes land.

---

## Contractus — what the formator NEVER does (v1)

The v1 fix operation is a **pure whitespace transform between an
unchanged token stream**:

- never adds, removes, or reorders a token
- never rewrites a byte inside a comment
- never changes brace structure (braceless stays braceless)
- never introduces or removes line breaks *inside* a statement
  (no rewrapping — the author's break points stand; only their
  indentation is normalized)

Consequence: every output byte is forced by a rule, and the gates below
can be maximally strict.

## Gradus — staging (lint first)

- **G0** — this document (rules with stable names, exemplars, tiers).
- **G1** — formator LINT mode: per-rule detectors over the silva tree,
  positioned self-naming divergences (`via:linea:columna regula
  exspectatum/inventum`), `-machina` TSV twin, house exit codes
  (0 = conformis, 1 = divergentiae, 2 = NIHIL IUDICATUM). Then: the
  corpus census — run over lib/ + include/ and record which rules fire
  where. The census is the acceptance test for the rules themselves.
- **G2** — `-scribere`: the fix operation for FIX-tier rules, gated by
  the full porta stack below.
- **G3+** — named deferrals, each a future explicit decision:
  banner rewriting (requires relaxing the differre gate to allow
  `documentaria`), statement rewrapping, include reordering.

## Portae — verification stack (G2 gates)

1. **Series lexematum** — token stream of output identical to input,
   trivia excluded. The strongest form of "only trivia changed."
2. **Differre** — `./silva/differre.sh vetus novum` reports ONLY
   `MUTATA [cosmetica]` rows. Zero substantiva, zero documentaria,
   zero ADDITA/REMOTA/MOTA. (The dogfood twist: the formatter
   exercises differre's classifier corpus-wide; differre gates the
   formatter.)
3. **Fidelitas** — silva roundtrip on the output: parse(output) then
   emit == output, byte-exact.
4. **Idempotentia** — format(format(x)) == format(x).
5. **Lint nulla** — the linter reports zero divergences on formatter
   output (for FIX-tier rules). The two modes verify each other.
6. **Compilatio** — examen ACCIPE on every formatted file; full test
   suite green from formatted sources.

---

## Regulae

Each rule: stable Latin name (lint output is self-naming), tier
(**FIX** = lint + rewrite in G2; **LINT** = report-only in v1),
statement, exemplar, edges.

### R1 `typus-in-linea-sua` — FIX

Storage class + return type on their own line; function name at column
0; in definitions AND prototypes (headers included).

```c
interior memoriae_index
_proxima_ordinatio (
```

### R2 `spatium-definitionis` — FIX

One space between function name and `(` in definitions and prototypes;
NO space at call sites.

```c
piscina_destruere (        /* definitio */
liberare(alveus);          /* vocatio   */
```

### R3 `bracchia-allman` — FIX

`{` on its own line for function bodies and all control blocks, at the
indent of its opener. (Moving a brace between lines is trivia-only.)

### R4 `custos-una-linea` — FIX

A braceless control body sits on the same line as its condition.

```c
si (!piscina) redde NIHIL;
```

Edge: joining is mechanical (trivia-only). If the joined line exceeds
R16's limit, R16 reports it — the join still happens (R16 is LINT).

### R5 `indentatio-quaterna` — FIX

4 spaces per level. Tabs forbidden everywhere (chorda.c switches to
tabs mid-file at `chorda_concatenare`; piscina.h is tab-indented —
both are drift).

### R6 `spatium-post-claves` — FIX

One space after control keywords before `(`: `si (`, `dum (`, `per (`,
`commutatio (`, and after `fac`.

### R7 `columnae-binae` — FIX

The two-column declaration rule (Fran's formulation, verified to
predict the hand-formatted corpus byte-for-byte). Applies to struct
members, local declaration blocks, and parameter lists.

Within one run: a RIGHT-aligned type column and a LEFT-aligned
identifier column. Gap between them = **1 + the run's maximum star
count** (no pointers in the run: 1 space; any single-star pointer:
2 spaces). Stars sit in the gap immediately after their own type text
(because types are right-aligned, this is the leftmost gap column) —
the star is NOT part of the type column.

```c
nomen structura Alveus {
              vacuum* buffer;
      memoriae_index  capacitas;
      memoriae_index  offset;
    structura Alveus* sequens;
} Alveus;
```

Edges:
- Qualifiers (`constans` etc.) are part of the type text and
  right-align with it: `constans character* piscinae_titulum`.
- `**` generalizes via the gap formula (gap 3, both stars packed after
  the type).
- Array suffixes ride with the identifier in the left column
  (`acervus[64]` is just a wide identifier) — they participate.
- FUNCTION POINTERS are exempt (identifier embedded mid-type breaks
  the model): plain single-space form, and they do not count toward a
  run's column widths.
- Run boundaries: blank line or any non-declaration line ends a run;
  each run aligns independently.

### R8 `parametra-singula` — FIX

Parameters one per line, ALWAYS — including single-parameter functions.
Open paren stays on the name line; closing paren on the last parameter
line. R7 alignment applies across the parameter run.

```c
_allocare_interna (
               Piscina* piscina,
        memoriae_index  mensura,
        memoriae_index  ordinatio,
                   b32  fatalis)
```

### R9 `aequatio-assignationum` — FIX

`=` alignment per cluster. A cluster is a contiguous run of simple
single-line assignment statements; a blank line or any other
statement breaks the cluster; each cluster aligns independently.
Mechanism: LHS stays LEFT-aligned at the statement indent; padding
goes before the `=`; the operator sits at **max(LHS end) + 2** —
TWO spaces after the longest LHS (`xyz  = valor`, decreed
2026-08-19: the corpus contained both one- and two-space stanzas;
Fran chose two).

```c
    piscina->primus                = alveus_primus;
    piscina->mensura_alvei_initia  = mensura_alvei_initia;
```

Clusters are AGGRESSIVE by decree: any adjacent simple assignments
co-align, including mixed member/local stanzas — odd-looking
results feed the post-lint heuristic refinement phase rather than
pre-emptive exceptions. (chorda.c:35-36 right-aligns bare LHS
names instead — decreed drift; statements keep a uniform left
edge.)

Two refinements (decreed 2026-08-19, from nuntium first light):

- **Equal-width clusters sit at max+1** — when every LHS has the
  same width there is no padding anywhere, so the double space
  marks nothing; single space (the hand's `valor = / valor |=`
  form). Corollary: an equal-width cluster inserts no padding and
  can never be pushed past 72 by alignment.
- **The 72 guard is CLUSTER-level**: if aligning would push ANY
  member past 72, the WHOLE cluster falls back to minimal (each
  operator at its own LHS end + 1). Uniformity is the invariant
  R9 exists for; the earlier per-member exception broke it —
  short members aligned, long ones stayed, producing the ragged
  mix Fran found at nuntium 647-654. The overflow predicate is
  computed independently of present spacing, so it cannot
  oscillate across fixpoint iterations.

### R10 `operatores` — FIX

Binary operators spaced; `->` and `.` tight; casts tight
(`(i8*)piscina_allocare(...)`); unary operators tight; comma = no
space before, one space after. (Unary-vs-binary `-`/`*`/`&` is decided
by the parse — silva knows.)

### R11 `continuatio` — FIX

Author line breaks inside statements stand (Contractus); their
continuation lines are re-indented to statement indent + 4, and a
break adjacent to a binary operator places the operator at the START
of the continuation line.

```c
    redde c == ' ' || c == '\t' || c == '\n' || c == '\r'
        || c == '\f' || c == '\v';
```

### R12 `spatia-caudae` — FIX

No trailing whitespace on any line; file ends with exactly one
newline.

### R13 `intervalla` — FIX

Blank-line policy: 2 blank lines before a banner, 1 after it; 1 blank
line between functions; runs of 3+ blank lines collapse to the local
maximum the policy allows; exactly 1 blank line between a function's
local-declaration block and its first statement (when both exist).

### R14 `vexillum-quinquaginta` — LINT (v1)

Section banners use 50 `=` characters:

```c
/* ==================================================
 * Titulus
 * ================================================== */
```

Survey 2026-08-19: banners appear in 142/154 lib/*.c and 137/157
include/*.h; width 50 dominates (3105 occurrences vs 694 for 72;
piscina.c's 59 is the outlier at 114). LINT tier because fixing
rewrites comment bytes (Contractus); promoting to FIX is the G3
decision that relaxes the differre gate to allow `documentaria`.

### R15 `ordo-inclusionum` — LINT (permanent?)

Include order: `postulata_posix.h` first when present (codex 85), then
own header, then house headers, then system headers. LINT tier
because reordering is token reordering — likely never a formator fix;
fixes stay manual (or a separate consented tool).

### R16 `longitudo-lxxii` — LINT (v1)

Line length limit 72 (DECRETA 2026-08-19; observed practice ranged
64-80, recent files hover 66-72). Report only; the formator never
rewraps (Contractus).

### R17 `catena-logica` — FIX (decreta 2026-08-19)

Multiline `&&`/`||` chains in `si`/`dum` conditions take the
padded-paren form: first operand at paren+4 (three spaces after
`(`), each line-leading chain operator at paren+1, and — the
comparison-run extension — branches that are two-character
comparisons (`==` `!=` `<=` `>=`) align their operators at
**max(LHS end) + 1** (ONE space after the longest left side; note
the deliberate contrast with R9's max+2).

```c
        si (   _lex_aspicere(lex, I)   == 'a'
            && _lex_aspicere(lex, II)  == 'l'
            && _lex_aspicere(lex, III) == 's'
            && _lex_aspicere(lex, IV)  == 'e')
```

Edges:
- Triggers ONLY on already-multiline chains (single-line conditions
  stay tight — no rewrap, Contractus).
- The paren-interior padding is R17's territory: no future
  tight-paren rule may claim that gap.
- R10-ante DEFERS to R17 for claimed comparison operators (same
  division as `=` between R10 and R9); R10-post (one space after
  the operator) still applies. Alignment inherits R9's 72-exception.
- Mixed chains: non-comparison branches (boolean calls, `<`/`>`
  single-char comparisons) share the padding but don't participate
  in alignment.
- Nested parenthesized chains anchor on their own paren
  (recursion); parenless chains (`redde a || b`) stay on R11's
  ca+4. `per` deferred WITH CAUSE: its paren wraps the init clause
  first, so the paren+4 anchor doesn't fit — chains there keep
  R11's minimum until a sensible anchor is decreed.
- Chosen over the operand-aligned hanging form (`&&` at paren−2)
  because it is strictly compatible with R11's ≥ ca+4 minimum for
  every keyword and anchors on a single token.

---

## Quaestiones apertae

- Header-specific rules beyond R1/R7/R8 applying to prototypes
  (doc-comment shapes like piscina.h's notatio section are observed
  but not decreed — out of v1 scope).

## Status G1 (2026-08-19)

CONFIRMED home: `silva/formator.sh` → `instrumenta/principalia/
formator.c` (CLI) + `instrumenta/silva_formator.{h,c}` (machine,
probatio-linkable — the differre split). Stream tranche shipped:
R5 (tabs only — depth checking deferred WITH CAUSE: naive %4
false-fires on R7 alignment), R6, R12, R14, R16. Structural
tranche (tree-riding rules) next. Probatio 33/33; negatives pin
the genus semantics (tabs in string literals and `=`-runs in
strings are immune by construction). First light on piscina.c: 45
divergences, superset of the hand archaeology, zero false fires.

## Status G2a (2026-08-19) — the emendatio engine + stream fixes

`-scribere` is live. Architecture: **the detectors are the fixers** —
each lint detector, when it fires, also records the machine-readable
edit that cures it (up to two whitespace spans in ORIGINAL
coordinates + insert text, via `_addere` then `_emendare`). One
analysis pass produces both report and fix; the modes cannot drift.
The applier is deliberately dumb and paranoid: it may only delete
whitespace bytes, only insert spaces/newlines, refuses newline edits
touching preprocessor lines (directive guard incl. `\`-continuations),
drops overlapping spans, and applies descending by offset so all
original coordinates stay valid. `formator_scribere` loops lint→apply
to a fixpoint (max 12): **rules compose through iteration, not
choreography** — G2b's R8 will split params, the next round's R7
aligns them. Gates in the machine: token-series identity vs the
ORIGINAL every round (comments included — bytes must match), fixpoint
required, refusal returns the untouched original with a named
querela. External gates in the probatio: differre classifies fix as
`cosmetica` (the dogfood assert), lexare→emittere roundtrips the
output byte-exact, idempotence.

DECREE G2a: **tabula = IV spatia** (pure-tab indent in the corpus is
one level per tab). Comment-INTERIOR tabs survive (inside COMMENTUM
bytes — Contractus), same class as comment-interior trailing space;
G3's banner decision owns them.

Wired so far (stream tier): R5 tabs, R12 (trailing + final-newline +
trailing-blank-lines), R6, R10-comma, R13-collapse (3+ blanks → 2).
Real-file first light: chorda.c + piscina.c fixed in one pass,
differre = 8 + 7 MUTATA ALL `[cosmetica]`, examen ACCIPE both,
second `-scribere` run a no-op (idempotence on real files). Probatio
117/117; suite 40/40. Unwired fixes (still lint-only): R13 banner
spacing/inter-functions, R1/R2/R3/R4/R8 (G2b), R7/R9/R10-tree/R11
(G2c).

## Status G2b (2026-08-19) — tree-tier fixes

Wired: R1 (name to own line / de-indent), R2 definition + call gaps,
R3 both braces both cases (move to own line at owner column / re-column),
R4 join, R8 param split (provisional indent 4 — G2c's R7 aligns),
R13-inter (insert blank when glued / trim to one), banner ante/post
spacing. New contract nuance: emendationes carry a `tolerans` flag —
JOIN spans (R4, R2 gaps, R13-inter trims) may legitimately hit a
comment or `\` continuation and then drop quietly as unfixable;
strict spans (padding, indentation, token-own bytes) still refuse the
whole file loudly on non-whitespace (detector bug).

FINDING pinned in probatio: `silva_token_radix` follows the
INVOCATION chain — a macro-expanded statement's whole tree collapses
to the use-site line, so R4/R3 legitimately never fire inside
`#define` bodies (the multi-line guard-macro fixture is CONFORMIS).
The directive guard is instead exercised by banner-post spacing
wanting to insert `\n` at column 1 of an `#include` line — dropped
(conservatively: even line-start inserts), file intact, divergence
residual.

Real-file first light: json.c — 213 emendationes, 3 iterations;
typus-in-linea-sua 7→0, parametra-singula 96→0,
spatium-definitionis 69→0, longitudo 66→63 (param splits shorten
lines); differre 69 MUTATA ALL [cosmetica]; examen ACCIPE; rerun
stable. Probatio 144/144; suite 40/40. Remaining: G2c alignment
(R7/R9/R10-tree/R11), then the supervised corpus rollout.

## Status G2c (2026-08-19) — alignment fixes: FIX TIER COMPLETE

Wired: R7 all three column adjustments (type-shift right / star into
gap / title push-pull, with the title fix withheld while the star is
misplaced — next round handles it), R9 `=` push-pull to max+2 (72-
exception inherited from lint), R10 tree gaps (binarium/accessus/
conversio/unarium — all tolerans, comments can sit in expression
gaps), R11 under-indent + the operator-move (TWO spans emitted as
one transaction, withheld entirely when a comment or continuation
obstructs either side).

TWO G1-ERA LATENT BUGS exposed when fixes made lint physical:
(1) the parametra LISTA carries comma TOKENS between parametrum
nodes — `_parametra_ordinem_censere` treated a non-node element as
fatal, so R7-params never judged ANY multi-param function (census
+2229 columnae now visible); (2) two-line PROTOTYPES are DECLARATIOs
so R11 counted their R1-correct name-at-column-1 line as an
under-indented continuation — the fix physically indented a json.c
prototype and differre could no longer pair the unit (ADDITA+REMOTA
instead of MUTATA: the dogfood gate caught it). Function-declarator
declarations now exempt from R11 spans (census −2110 continuatio).
Census at G2c semantics: 42,674.

Real-file first light: piscina.c 35 em/2 it (the alveus stanzas
moved to max+2, the loop-body cluster co-aligned — the decrees
executing on the exemplar); json.c 535/4; chorda.c 478/3. Residual
lint on all three: ONLY longitudo (119, count unchanged — alignment
pushed no net new lines past 72) + vexillum (56). differre:
17/42/72 MUTATA ALL [cosmetica]; examen ACCIPE ×3; rerun stable ×3.
Probatio 167/167; suite 40/40. THE FIX TIER IS COMPLETE — next: the
supervised corpus rollout, then G3 doors.

## Status EVOLUTIONIS (2026-08-19) — lib/ + include/ COMPLETE

The rollout is done for the library tier: all of lib/ (batches
1-3) and all of include/ (batch 4) are in house form, PROTOTYPES
INCLUDED (the 2a door closed: R1/R2/R8/R7-params now judge
root-level declarations; true prototype = function declarator
whose internum is TITULUS; fn-pointer declarators exempt; local
prototypes untouched). Rules added/refined during rollout by
Fran's corpus reads: R17 catena-logica; R9 equal-width→max+1,
cluster-level 72 guard, declaration initializers join clusters.
Engine hardenings (see formator.worklog.md 2026-08-19, evolutio
III): block-shaped specifiers exempt from runs; same-line members
never co-align; alignment pulls tolerans; ONLY source-level
operators judged (_token_fons); title-before-cb sanity;
banner-banner gap owned by ante; AMBIGUUS subtrees unjudged;
latina.h praebere'd guard-blanked (lexicon typedefs visible to
the oracle — silva door 01M0D4QV5S for the true root fix).

CENSUS: 42,674 pre-fix → **6,499 post-rollout**, of which 6,397
(98%) is the lint-forever tier (longitudo 5,160 + vexillum
1,237). The 102 fixable-rule residuals are honest unfixables:
tolerans-dropped spans (comments/macro geometry), directive-
guarded edits, mid-line banners. Exclusiones: 21 rows (latina.h
newest — tab-stop macro table, lexicon datum generated from it).

ROLLOUT CHOREOGRAPHY (for the remaining terrain — silva/fontes +
instrumenta, apps/, tools/, probationes/): after formatting,
run ./silva/amalgamare.sh (the amalgam vendors lib sources) and
./tools/natura_struere.sh (mtime-gated tools go STALUS); expect
other self-gating tools to complain loudly. Purity oracle for
differre pairing quirks (01M0D4RN3B): whitespace-stripped sha
compare of both sides.

EXCLUSIONES (decretae 2026-08-19): pins file
`silva/probationes/fixa/formatoris/exclusiones.txt`, format
`via<TAB>causa` (the examen-pins pattern — every row carries a
named cause). Exemption is NEVER silent: the CLI announces
`formator: exempta <via> (<causa>)` on stderr. Exempted at birth:
lib/biblia_dr.c (data, not code) + the three frozen predecessor
generations (arbor ×6, arbor2 ×8, lapifex ×5 — deleted after
parity harvest per silva culture). R16 stays 72: raising to 80
would cut fires 60% (6356→2556 measured), but the count is not
the target — the norm is, and recent practice sits under 72.
Census baseline with exclusions live: 14286 divergences in 244 of
291 judged files (tabs 7772 / long 5225 / banners 1237 / trailing
51 / keyword-spacing 1).

## Status ADOPTIONIS PROVENIENTIAE (2026-08-19)

Silva quaestiones provenientiae praebet (5b3dab~: silva_nodus_
est_fons_purus severa + silva_nodus_geometria_fida — sedes radicum
distinctae; expansio 1:1 fida, 1:N collabitur). Adoptio prima:
R2-vocatio (_vocationem_censere) — apertum per _token_fons (paren
fontis sola iudicatur) + subarbor functionis per geometria_fida.
Classis falsorum interfecta: vocatio in corpore macri (hiatus
negativi; XLVI ordines in probatione ipsa quaestionum, VIII
residua "infixabilia" trans lib/+include — census 6499→6491,
spatium-definitionis residuum NULLUM). Porta corporis formati
intacta per constructionem (iudicia sola remota). Ostium: sedes
_valor_radix reliquae (ancorae sententiarum) mundae trans lib/
include; si evolutio probationum (terra CREDO-densa) classes novas
accendit, consilium generale = tabula SEDIUM MENTIENTIUM per
parsuram (transitus linearis unus; honestas per lexema O(1)).

## Lex basis mensurabilis: `-delta` (2026-09-01)

`./silva/formator.sh <via> -delta [ref]` (ref = HEAD) reddit
divergentias NOVAS contra versionem ref: clavis = regula + contentum
lineae, numquam numerus lineae (labitur editione). Effusum: summa
`basis N -> nunc M (+a novae, -b evanidae)` et sedes novae nominatae;
`-machina` = TSV + ordo `DELTA`. Exitus 0 = nihil novum, 1 = novum.
`-contra <basis.c>` = basis explicita ex disco. Plagula extra ref =
basis vacua ("basis absens" - omnia nova; `-scribere` eam).

Cur per contentum: tally classium permutationem celat (+I -I = zephyrum
mendax); numeri linearum in campis inventum/exspectatum regulae
`parametra-singula` comparationem per positionem polluunt.

Uncus pre-commit (tools/unci-git) `-delta` super plagulas .c/.h
commissionis currit et MONET (+N, sedes usque ad XII) - numquam obstat:
lex basis iudicium hominis manet. Porta natalis:
`silva/formator_delta_fumus.sh`.

## Ambitus nominatus: `-scribere -intra <functio>` (2026-09-01)

`./silva/formator.sh <via> [-scribere] -intra <functio> [-intra ...]`:
lint et scriptura intra extenta functionum nominatarum SOLA. Extentum =
a linea post nodum radicis priorem (commentarium ducens et intervalla
supra ad functionem infra pertinent - eadem attributio quam R13 facit)
ad lineam ultimam definitionis; prototypum radicis eiusdem tituli
quoque. NOMINA, non lineae: passus punctum-fixum quaque iteratione
reparsat, lineae labuntur, titulus manet - ergo in machina
(`formator_lint_intra` / `formator_scribere_intra`, `FormatorIntra`),
non in involucro. Divergentia extra extentum TOTA (sedes + emendationes
omnes) cadit; transgrediens cadit (conservative). Functio ignota =
recusatio clamosa (exitus 2; machina ipsa recusat, CLI nomen dat);
parsura fracta = nihil inventum = recusatio.

Porta natalis: `probatio_silva_formator` sectio "ambitum nominatum"
(vitium plantatum in `_intra_continetur` → V rubrae). Usus cum lege
basis: plagula communis baseline magno - `-scribere -intra` functiones
novas, `-delta` postea zephyrum novum. Quod NON attingit: ambitus
minor functione (sectio intra principale probationis) - copia scratch
+ insertio manet (porta: ambitus corporis/sectionis).

## Custodia LXXII glomerum + R16 FIX `fractura` (2026-09-01)

**Custodia.** Ordinatio (R7 columnae, R9 aequatio, R17 A/C catena)
numquam lineam ullam ultra LXXII trudit. Linea TOTA metitur (tabula
longitudinum in ambitu; commentarium caudae inclusum, quod extensio
nodi non videt), tractio = columna recta - columna praesens. Glomus
totum cadit (uniformitas, ut R9): R7 nihil ordinat et ordinem LINT
unum in membro latissimo dat (`glomus non ordinatum: ordinatio LXXII
excederet`); R17 C omnes participes ad cb suum + I; R17 A omittitur
(fractura lineam prius scindit). Praedicatum a spatiis praesentibus
independens - non oscillat.

**Fractura.** `longitudo-lxxii` emendationem fert ubi candidatus
exstat (nuntius `linea nimis longa - frangibilis`; uncus post
editionem eam nunc monet, residuum planum tacet). Candidati ex
lexematis CRUDIS: SPATIA post virgulam, post `=`, ante operatorem
binarium cuius lexema prius operandum finit (`-` unarium exclusum).
Electio: logici (&&/||) profunditate minima, dextimus cadens; aliter
dextimus cadens; nullo cadente sinistimus (cauda iteratione sequenti).
Nulla fractura: directivae, continuationes `\`, litterae solae, cauda
commentarii sola transgrediens. Indentatio: linea prima sententiae
(sententia intima registrata, uni-lineares quoque) -> ca + IV;
continuata -> gradus suus, nisi candidatus artius ligat quam operator
lineam ducens (+ IV). R11/R17 iteratione sequenti perficiunt.
Tectum LXXII planum; porta: sub indentatione gravi tectum = ca + XL
(regula STML) - fixa primum.

Portae natales: probatio sectiones "custodiam LXXII glomerum" et
"fracturam linearum" (XVII fixa mensurata; vitia plantata XX/IV/II
rubrae). Oraculum alterum: differre super legatus.c et semantica.c
scriptas = cosmetica omnia.

## Ancorae in nuntiis ordinationis (2026-09-01)

Ordines R7 (columnae-binae), R9 (aequatio-assignationum), R17 C
(catena-logica comparatio) et R11 (continuatio) ancoram nominant et
regulam ipsam ferunt: `<basis> (ancora '<textus>' l.<linea><cauda>)`.
Ancora = membrum ad quod ceterae ordinantur: R7 typus latissimus
(`: typus latissimus`, `: stella ad oram typi latissimi`, `+ I +
stellae N`); R9 sinistrum longissimum (`+ II`, `+ I (aequilata)`,
`, minimum (LXXII)`); R17 C sinistrum longissimum catenae (`+ I`,
`, minimum (LXXII)`); R11 lexema primum sententiae ancorantis (`+
IV`). Textus ad XXXII octetos decurtatur. Clavis `-delta` (regula +
contentum lineae) nuntium non tangit; uncus regulam solam legit.
Porta: probatio sectio "ancoras in nuntiis" (IV regulae; vitium
plantatum IV rubrae).

## Scriptura in commissione: `-lineae` + uncus (2026-09-01)

`formator -lineae a-b` (repetibile): functiones radicis quarum
extentum lineas tangit (`formator_extenta` - idem ambitus quem
`-intra`) nominantur et ut `-intra` iudicantur; nomina ANTE passum
punctum-fixum resolvuntur. Lineae extra functiones (structurae,
tabulae) monentur, non formantur; nihil intra functiones = plagula
intacta, exitus 0.

Uncus pre-commit (tools/unci-git): plagulae .c/.h commissioni traditae
-> hunks `git diff --cached -U0` -> `-scribere -lineae` -> index
repositus. Plagula nova = scriptura tota. Arbor operis ab indice
differens (commissio partialis) NON formatur, monetur. Ordo: scriptura,
examen, monitum `-delta`. Evasio `--no-verify`. Porta:
`tools/unci-git/fumus.sh` VI-VIII (fixum tractum fumus_formae.c).
Porta post evolutionem totam: scriptura tota (structurae inclusae).

## Evolutio totius terrae (2026-09-01)

`./tools/forma_evolutio.sh <dir> [-scribere]`: census aut scriptura
tota directorii (plagulae tractae .c/.h sine fixa/vendor/build/
amalgama/scratchpad/capsula_*; capita directoriorum interiorum per
`-capita`), oraculum alterum `differre` per plagulam mutatam
(`[substantiva]`/`[documentaria]` = FRACTUM). Generata in exclusionibus
(generator nominatus). Commissiones formae solae `--no-verify`
(oraculum = differre), hashes in `.git-blame-ignore-revs`. Post formam
fontium amalgamata REGENERANDA (excludenda primum) - amalgamata_probare
drift non videt. Terra evoluta 2026-09-01: tools, css, materia, gesta,
tessera, saltuarius, officina, apps, lint, instrumenta, silva,
probationes; omissa: knotapel (demonstrationes Anglicae), vendor.
Definitiones ex macro expansae (geometria infida) non iudicantur.

## -extenta, columna emendationum, differre -verdictum, pythonica (2026-09-01)

`formator -extenta`: TSV `via titulus linea_a linea_nodi linea_b genus`
(extenta radicis; linea_nodi = linea prima nodi, sine commentario
ducente). `-machina` columna VIII = numerus emendationum (0 = quod
formator sanare nequit; uncus post-editionem ea sola ostendit).
`differre -verdictum [cosmetica|documentaria]`: exitus 0 si paria omnia
intra gradum, 1 aliter, ordo `VERDICTUM <gradus> cos doc sub mota add
rem`; MOTA/ADDITA/REMOTA semper substantiva. Uncus pre-commit: scriptura
TOTA praeter knotapel/vendor. `pythonica/silva.py`: facies Pythonica
(extenta, Editio omnia-aut-nihil cum ancoris tolerantibus, substituere/
inserere nomine, verdictum, differre) - porta `pythonica/probare.sh`.
R13: acervus directivarum inter functiones = separator (nullus ordo).

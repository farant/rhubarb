# differre.c — worklog

## 2026-08-17 — natus (differentia semantica unitatum)

Third granularity of the differentia core in one day: the SAME
Myers-over-interned-identity that diffs lines diffs UNITS — lex
both versions, partition bytes by unit, sigillum each span, intern
the hashes, differentia_seriei over units. Silex diffs lines;
differre diffs the units you think in.

**MOTA semantics finding (measured at birth):** LCS anchoring
means "moved" is RELATIVE. A function that changed position while
its neighbors also changed stays IDEM in the LCS (it IS the
anchor); only units that fall out of the LCS with an equal-hash
partner report MOTA. Measured both ways: clean swap among
unchanged neighbors → "MOTA (contentum idem, sede nova)", zero
line churn (a line diff would show ±N noise); position change
amid changed neighbors → anchored, neighbors report. This is
correct, not a limitation — who moved is genuinely relative.

**Partition rule:** unit k's byte span = [prev end, own start),
own start = first token's leading trivia (attachment rules decide
comment ownership); unit 0 absorbs the file header, last unit
absorbs the tail. Every byte belongs to exactly one unit — the
differentia partition invariant philosophy at unit scale.

**Classification:** substantiva (code token sequences differ) /
documentaria (code identical, comment trivia differ) / cosmetica
(tokens and comments identical — whitespace only). Decidable
because trivia are first-class tokens.

**Prototype and definition are separate units** (top-level
semicolon vs brace rule) — a signature change shows twice, once
per unit. Honest; a symbol-level grouping pass is a named door.

**Known edges (at birth, per debrief practice):**
- Duplicate-content units: first-unmatched-wins in MOTA/MUTATA
  maps; a second identical pair may mis-pair. Rare, noted.
- Directive units are line-naive: #if/#endif are separate untitled
  units; conditional-ARM-aware diff (silva retains untaken arms —
  nothing else on earth diffs those) is the big named door.
- File-header comment edits attribute to the first unit (MUTATA
  documentaria on it) — honest but slightly odd.
- Titulus matching ignores genus: a typedef replacing a function
  of the same name reports MUTATA. Acceptable; genus is in the
  output either way.
- The -machina TSV is the composition surface: changed-symbol
  names pipe into nexus.sh / legati vocantes for blast radius —
  deliberately NOT built in.

Factoring: unit scanner extracted from the amalgamator into
instrumenta/silva_unitates.{h,c} (PORTA: amalgam byte-identical,
suite 39/39). silva_unitates_scandere = the enumeration wrapper;
directive titles for define/undef/ifdef/ifndef/include.

## 2026-08-18 — modus -commissum (commissum totum in symbolis)

`differre -commissum [ref | vetus novus]` = "what did this commit
do, in symbols" — git log --stat at unit granularity. Zero refs =
HEAD vs parent; one = that commit vs its first parent; two =
tree vs tree across any range. Built on git_arbores_differre:
unchanged subtrees are never opened, so the whole-repo walk costs
roughly what the changed files cost.

The refactor that paid for it: the pairing block and the emission
loop moved out of principale into _paria_computare /
_paria_emittere unchanged (same rows byte-for-byte in single-file
mode, verified against the pre-refactor output). Emission gained
two knobs: via_machina (prefix a path column — commissum -machina
only) and corpora (unified bodies under MUTATA — ON for
single-file modes, OFF for -commissum; the --stat spirit is rows,
you drill into one file with `-git <via> <vetus> <novus>`).

Per-file handling: .c/.h → full unit pairing; anything else → one
line-diff summary row (genus "plagula" in TSV); NUL byte on
either side → "(binaria)", no counts. Line totals are per-unit
sums, deliberately NOT git's whole-file counts (MOTA counts 0,
boundaries differ) — same numbers as single-file differre, which
is the consistency that matters.

First light: 0f7ad83 renders as 6 files / 17 unit pairs with the
sha-breve arc named function by function; file list matches
`git show --name-status` exactly. The prototype+definition twin
rows (parked door) show up as expected and read fine in practice.

## 2026-08-18 — modus -historia (symbolum per commissa)

`differre -historia <via> <titulus> [ref]` — when did symbol X
change, and HOW. First-parent walk from ref; two-stage screen:
(1) file blob sha per commit via git_sha_per_viam — unchanged
file = zero parsing, zero content reads; (2) where the file
changed, parse both sides and compare the sigillum of the
CONCATENATED spans of every unit bearing the title — prototype +
definition travel as one identity, so a signature change is one
event, not two. Events: MUTATA (classified substantiva/
documentaria/cosmetica via flat token/comment compare of the
concatenated spans), ADDITA (birth — walk continues afterward, so
removed-then-readded ghosts surface as REMOTA deeper down),
REMOTA. Bodies = unified diff of the spans; -summa = event lines;
-machina = sha/dies/status/classificatio/+N/-M/subiectum TSV.

Memory stays flat across the walk: each parse lives in a
throwaway piscina destroyed after span extraction; only the
extracted spans + sigilla survive into the walk arena. Ten months
of lib/piscina.c history walks in ~4s wall.

Two compile-time landmines worth recording:
- `brevis` as a variable name = latina macro for `short` →
  `Piscina* short;` syntax error. censor.sh named the culprit
  immediately. (Firing #7 of the every-latina-word-is-reserved
  rule.)
- `"????-??-??"` as a date sentinel: `??-` is a TRIGRAPH (→ `~`)
  and -Wtrigraphs -Werror rejects it. Any `??` run in a string
  literal is suspect in C89. silva's examen does NOT model
  trigraphs — clang caught what the house parser passed.

First light: git_ref_resolvere = born 4aafde9, +6 at 0f7ad83
(short-sha hook), a930731 correctly SKIPPED (file changed, symbol
didn't — the stage-2 screen proving itself).
`imprimere` in latina.h: never substantively changed since
2025-10-21 — both events [cosmetica] (a drifting blank line). The
classifier telling true stories about the project's whole life.

## 2026-08-19 — pairing survives whitespace-only reshapes (01M0D4RN3B closed)

The engine's first dedicated probatio (probatio_silva_differre.c) was
born for this fix. Two defects, both found by reading the pairing
loop against the reproduced filum.c case:

1. **First-ordinal-only tables**: sigilla/tituli_remotarum stored only
   the FIRST remota ordinal per key ("si !continet → inserere"), so
   the second identical `#include` in #ifdef arms could never find
   its partner — the first was claimed, the second unreachable →
   false ADDITA+REMOTA. Fix: table values are now Xar lists of
   ordinals; lookup takes the first UNCLAIMED (_ordinalis_liber).
2. **No whitespace-blind pairing key**: unit identity was a sha of
   RAW span bytes; whitespace change killed both IDEM and MOTA, and
   title-pairing was the only rescue — so TITLE-LESS units (#endif,
   #else) could never re-pair, and title collisions across #ifdef
   arms could cross-pair into false "substantiva". Fix: a second
   NORMALIZED seal per unit (sha over the token series, genus+valor,
   whitespace excluded) — identitates_normatae on the latus — pairs
   between MOTA and title: MOTA (raw) > MUTATA (normata) > MUTATA
   (titulus) > ADDITA.

The normalized seal IS the "whitespace-stripped sha" interim oracle
from the rollout, promoted into the engine — the pairing key now has
exactly cosmetica-semantics, so the classification downstream agrees
with the pairing by construction.

METAMORPHIC PROPERTY, pinned in probatio section V and swept over
the real store: for file F and whitespace-only transform W (the
formator), differre(F, W(F)) = ADDITA 0, REMOTA 0, MUTATA all
cosmetica. Sweep: 59 unformatted-terrain files transformed (silva/
fontes + instrumenta + tools + 2 probationes), ZERO violations; the
sweep's awk verified against a planted fault (counts 2 on a real
ADDITA diff — a zero you haven't tested is not a zero). Controls:
true ADDITA/REMOTA/substantiva/documentaria all preserved (section
IV). Suite 42/42; silex rebuilt (it bundles this engine).

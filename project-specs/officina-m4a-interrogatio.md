# officina M4a — interrogatio (raw Q&A)

*2026-07-11. Gradus 3/4 of the M4a on-ramp. 15 questions from
officina-m4a-fundamenta.md §III as modified by simulatio-6 (Q8
dissolved pre-interview; vocabulary ceremony added). All 15 answered
with the recommended option — recorded verbatim below. DECISUS.*

## Identity + policy core

**Q1 — Layer name (module prefix, driver, how we say it forever).**
Candidates: examen (addendum's floated name), arbiter, iudicium
(one letter-swap from indicium — real collision risk), sententia.
**A: examen.** Examination/weighing/scrutiny; distinct from every
shipped name; examen.sh reads naturally.

**Q2 — The violatio-tier line: C89-standard-strict or house-strict?**
They differ on exactly two constructs: K&R/unprototyped definitions
and implicit function calls — both legal C89, both forbidden by house
daily flags. **A: Standard-strict.** violatio = exactly C89 constraint
violations; K&R + implicit calls land in the house tier; the driver
can still fail house code on house-tier findings via a flag — policy
composes instead of being baked into the tier.

**Q3 — Severity tier vocabulary (ceremony).**
**A: violatio / suspectum / domesticum.** 'suspectum' continues
censor's existing middle-class usage; 'domesticum' says exactly what
the third tier is (ours, not the standard's).

**Q4 — Lvalue vocabulary (ceremony).**
**A: locus.** _est_locus(nodus); locus mutabilis = modifiable lvalue.
Pairs with valor as lvalue/rvalue; short, greppable, unclaimed.
(sedes rejected — it's the phase-log's prose workhorse; claiming it in
code muddles the meta-language.)

## Machinery shape

**Q5 — Diagnosticum v2 fields for v0.**
**A: severitas + codex + positio materializata + socius.** Byte
ranges deferred until the LSP consumer pulls (extent-API promotion =
named park). Related-site (socius) costs one field now vs a migration
later.

**Q6 — v0 judgment scope.**
**A: Core as simmed.** Compatibility relation + three-way seat +
locus (lvalue/const/modifiability) + cheap table wins (arity, return
void/non-void, controlling-expr scalar). Named-deferred with pull
conditions: switch case-set, initializer validation, struct member
legality.

**Q7 — Enforcement posture: always-on or opt-in flag?**
**A: Always-on** (M3 lineae precedent: provenance/judgment is a
product feature). One semantica, one behavior; severity field makes
downstream filtering trivial; sweep cost measured and recorded at
seal.

**Q8 — Ambiguity policy for verdicts under retained-AMBIGUUS
canonicals.**
**A: Provisional mark.** Verdicts under retained ambigui carry a
provisional bit; harness/driver exclude them from hard accept/reject;
UIs may show them dimmed. Nothing silently lost. (Suppress-entirely
rejected: hides findings with no trace. Judge-anyway rejected: fires
spurious violations, fights the accept bar.)

## Bars + harness posture

**Q9 — Seal the oracle invocation string?**
`clang -std=c89 -pedantic-errors -Wno-long-long
-Wno-overlength-strings -Werror=c99-designator
-Wno-error=implicit-function-declaration -Wno-strict-prototypes
-ferror-limit=0 -fno-caret-diagnostics -fsyntax-only`
**A: Seal it + leak sweep.** Adopt the string, plus one fixture sweep
over C99/GNU-extension warning groups hunting sibling
designator-style leaks BEFORE the invalid corpus is blessed. Version
recording per run mandatory regardless.

**Q10 — Secondary bar (semantica-diagnosed SISTERE class → 0):
formal gate or recorded measurement?**
**A: Recorded measurement.** Fusor census already reports the class;
record the delta at seal. The 126 include cases v0's scope may not
cover — gating at 0 would force scope creep to pass the gate.

**Q11 — Invalid corpus home + marker word.**
**A: silva/probationes/fixa/examinis/ + EXSPECTA markers.** Books
silva-side per M0 precedent; fixtures with silva's other fixa;
include-free by rule; pins at (linea, warning-group/codex); grep/sed
extractor (never silva); blessed from oracle output (captare
precedent).

**Q12 — Standing-gate posture for the differential.**
**A: Percursus gains a verdict column** (as stdout-diff joined
cursor). The clang side of the differential runs at milestones /
on-demand — examen-side regressions are what daily work can break.

## Driver

**Q13 — Driver home and shape.**
**A: censor pattern** — silva/instrumenta/principalia/examen.c +
silva/examen.sh. Usage: ./silva/examen.sh <file.c> — parse, analyse,
judge, print verdict + diagnostics with positions. Joins the bench.
This is ALSO the committed single-file diagnostic dumper that never
existed (M0b used an uncommitted throwaway).

**Q14 — Machine output lane.**
**A: -machina TSV flag.** One row per diagnostic: via, linea,
columna, severitas, codex, provisional-bit, causa. nexus.tsv
precedent; harness consumes directly.

**Q15 — Driver default lexicon.**
**A: ISO systema seed by default; -posix adds the POSIX layer
(vindex parity); -nudum = no lexicon** (for testing the LEXICON_DEEST
machinery itself). Judgment without a lexicon judges a guess — the
default protects the common case.

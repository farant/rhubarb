# OFFICINA M4b — INTERROGATIO (raw Q&A record)

2026-07-11. The gradus-III interview, following the proven arc:
exploratores (officina-m4b-fundamenta.md) → simulatio-7
(officina-simulatio-7.md) → THIS → spec. Fifteen questions, four
rounds; ALL recommendations accepted. Decisions that preceded the
interview (sealed by the design conversation + sim, recorded here
for completeness): the REPLAY-DOCUMENT world model (document =
truth, world = cache of replay(document)); the GREEN INVARIANT
(document always replays green; acceptance = whole-document
verdict); ENGINE/SURFACE split (sessio library + surfaces; workbench
+ vindex-eval = named pulls); JUDGE-THE-WRAPPED-TEXT (statement
turns wrapped for judgment+lowering; statement-context legality
free); JUDGE-BEFORE-TEARDOWN ordering; always-reparse (no systema
chaining, no oracle carry-forward, no tag seam in v0).

## Round 1 — turn semantics / per-turn UX

**Q1 (C1). Repair policy when appending `;` makes invalid input
parse clean?**
A: **Repair + echo.** Accept any single-elementum repair but echo
the repaired line (`… x + 2;`) — DWIM without silence; the echo is
the honesty mechanism.

**Q2. Declaration-turn echo?**
A: **Terse `x : s32`.** Name : type from semantica's typing —
confirms what the judge understood (typedefs resolved); passive
type-teaching suits the REPL-as-teacher role.

**Q3. Value-printing depth v0?**
A: **Scalars + char\* preview.** All scalars as `value : type`
(host-side, semantica type names); char* prints address + bounded
NUL-string preview from VM memory; aggregates refused by name.
chorda display = NAMED FAST-FOLLOW (aggregate return — %reditus
path).

**Q4. Changed-history surfacing?**
A: **Quiet notice.** One line `turnus 3: effusio mutata` (with
`:effusio 3` to inspect old vs new). Honest counterpart of the
green invariant without spam.

## Round 2 — world lifecycle / execution policy

**Q5. Fast path in v0 or always-rebuild?**
A: **Always-rebuild v0.** One code path; C9's dirty-world machinery
disappears; correctness trivially auditable. Fast path = FIRST
named optimization, added from measurement.

**Q6. Startup world?**
A: **-plagulae list** (interpretare/vindex precedent); no flag =
empty world (builtins only). Whole-monorepo prelower = named
fast-follow on the cursor moduli cache.

**Q7. Refusal families in session mode (multi-select)?**
A: **All three: file WRITES, tempus/fortuna, stdin reads.** The
refusal mechanism ships as the seam; recording-on-first-contact
(acta-visio) is the eventual unlock for all three.

**Q8. Bless the C11 reversal (umbra out of v0)?**
A: **Blessed: seam v0, umbra pull.** v0 refuses writes by name; the
umbra + :committe ceremony = FIRST named post-v0 pull, spec'd as an
acta-seam consumer. Cost measured: stdio semantics over buffers is
the real build.

## Round 3 — artifact / runner / bar

**Q9 (C4). Save dialect default?**
A: **Liberal default + -strictum flag.** `:serva` writes the
document as typed (perfect round-trip with :aperi and #!);
`:serva -strictum` emits wrapped + synthesized principale = strict
C89 clang-compilable export.

**Q10 (C12). Script rule?**
A: **Bless both modes.** principale-if-present, else the top-level
statement schedule in document order — C scripts with top-level
code become a real product. :aperi (load + continue) falls out.

**Q11. History posture v0?**
A: **Document IS history.** `:monstra` shows numbered turns; no
line-recall, no history file, no ~/.rhubarb convention yet. The
raw-mode editor with recall = the workbench pull.

**Q12. The M4b bar?**
A: **Transcript golden + script + persistence.** (1) golden-
transcript suite through the ENGINE API — scripted input, full
output byte-compared, including a redefinition/changed-history
scenario and each refusal; (2) a #! script driving ≥2 monorepo libs
runs green; (3) the globals write-N/read-N+1 regression test
(mundus's gap) ships. All existing suites stay green.

## Round 4 — ceremony / runner identity

**Q13. Engine + binary names?**
A: **sessio + colloquium.** Engine = sessio (officina/instrumenta/
library, UI-free). Binary = colloquium (a conversation — what a
REPL is).

**Q14. Trace name?**
A: **acta.** The Acta Diurna resonance — the record of what the
program did in public. Overlay stays umbra when it arrives.

**Q15a. Wrapper prefix?**
A: **turnus_N + collision check.** No reserved-prefix violation;
engine scans document symbols before generating and shifts on
collision.

**Q15b. Which binary owns #!?**
A: **colloquium batch mode.** `#!/usr/bin/env colloquium` — same
binary, engine batch path; REFUSALS OFF in script mode (no replay →
no green invariant → real writes/time/stdin), JUDGMENT ON (REICE
before demissio — a script that doesn't judge green doesn't run).
cursor stays the corpus sweep tool.

## Spec-time decisions delegated (flagged for review at spec)

- #! argv v0: DROP (argc=0/argv=NULL, machinula zero-fill); the
  argv-array wrapper synthesis (interpretare literal-baking
  extended) = named park. Follows the minimal/refusal posture.
- stderr: captured per turn via ansae[2], displayed after stdout,
  uncolored v0.
- Books: officina-side (the REPL is an officina product; silva
  pointer entries only if silva fontes are touched — expected: NOT).

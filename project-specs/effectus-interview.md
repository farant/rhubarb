# effectus — interview raw (fabrica slice 4, crusta pilot) (2026-10-05)

Feature: an EFFECT SUMMARY of shell scripts - every place a script reads,
writes, executes, sources, lists or tests a file or reads an environment
variable, each with its evaluated path value - built on crusta and emitted
as STML so the house exemplar lint can match it. Fran's framing: a test
pilot for effect analysis in C and other execution languages; the
vocabulary should be language-neutral, bash implemented first.

Context carried into the interview (sessions 2026-10-03/05):
- fabrica slice 3 (spec 3 §XIV): gates as `iudicium` actions keyed on the
  read ledger's trace + `crusta/fontationes.sh` (scripts sourced/executed).
  The remaining blind spot: bash reading files directly (`$(cat f)`,
  `< f`, `[ -f x ]`, globs) - only the audit can catch it.
- Desideratum …Q4C7D (lint 'lectiones bash', rules grown from real
  examples, false positives pinned as negative fixtures, one inventory row
  per site).
- Capability check (2026-10-05): crusta's exemplar lint expresses the seed
  patterns STRUCTURALLY (redirectio + operator, pars-substitutio with
  `cat`, git, python3, `[ ... ] ||`) - a scratch rule matched exactly; but
  text matches by EQUALITY only, and many paths are variables only
  fontationes' evaluator resolves. First real examples, 9 hits in toml's
  15-script chain: 4 closure lists written earlier in the same run
  (cursor_communis.sh), 4 lock bookkeeping files (sera.sh), 1 legacy mtime
  stamp (vexilla.sh, build/vexilla.sigillum + touch include/latina.h).
- Proposal accepted in conversation: an analyzer computes value questions
  as attributes (sub_build, resolutio, scripta_in_ambitu) so rules stay
  equality-only (no expression language); the same vocabulary for the
  runtime trace makes static-vs-dynamic comparison set arithmetic.

## Round 1 — shape

**Q1. How wide?**
Options: bash summary + lint / plus a second verdict gate (aedilis or
fabrica) / schema-first cross-language.
**A: Bash summary + lint.** crusta effect summary (sites, evaluator,
command table, STML), exemplar lint rules over it, back-filled over toml's
runner chain; fontationes becomes a client. Vocabulary named
language-neutral, only bash implemented.

**Q2. How is the analyzer's recall measured?**
Options: dynamic spike first / fixtures only / fixtures now, dynamic later.
**A: Dynamic spike first.** Before building: prove a
DYLD_INSERT_LIBRARIES interposer logs open/stat made by Homebrew bash
(redirections, source, test -f, globs). If it works it is the oracle for
the slice; if not, we learn that up front.

**Q3. What consumes the summary in v1?**
Options: lint + fabrica key / lint only.
**A: Lint + fabrica key.** Exemplar rules match it (blocking in verdict
chains, warning elsewhere), and the verdict key digests resolved bash reads
automatically - fontationes' job, generalized.

**Q4. Flow sensitivity?**
Options: flow-insensitive / control-flow graph now.
**A: Flow-insensitive.** Facts per process scope ("something in this
scope writes a path covering this read"), no ordering; a CFG later if
examples demand it.

## Round 2 — vocabulary, commands, unknowns, scope

**Q5. Where does the language-neutral vocabulary live?**
Options: STML dialect + canon / informal, crusta-owned.
**A: STML dialect + canon.** An 'effectus' dialect written down once
(project-specs/effectus-spec.md) and pinned by a canon like the house's
other STML dialects, so a crusta (bash) and later silva (C) emitter are
judged against the same rules.

**Q6. How is the external-command table seeded?**
Options: measure the house corpus / six seed patterns only.
**A: Measure the house corpus.** Count which external commands appear in
tracked .sh files, seed the table with the top ones; unknown commands are
explicit 'effect unknown' sites that the examples loop grows from.

**Q7. An unresolvable read in a verdict chain?**
Options: IGNOTUM unless excused / warn, trust the audit.
**A: IGNOTUM unless excused.** Strict, like fontationes: not reusable
until fixed or excused in place with a <tolera> giving the reason (which
becomes the inventory row).

**Q8. Which scripts are analyzed in v1?**
Options: all house .sh as data / verdict chains only.
**A: All house .sh as data.** The summary is computed for every tracked
.sh once (a census: sites by kind, resolution and class - the example
corpus), blocking only in verdict chains, warnings elsewhere.

## Round 3 — excuses, inventory, fabrica

**Q9. Excuse granularity?**
Options: grammar scope / per site only / per file allowlist.
**A: Grammar scope.** The existing <tolera> mechanism: a comment
annotation covers the node it is attached to - one line, one command, or a
whole function/conditional (materia_annotationes scopes by owning node).
sera.sh's lock reads = one excuse on the function, not four.

**Q10. Where does the per-site inventory live?**
Options: census file + ledger rows for decisions / every site in the
ledger.
**A: Census file + ledger rows for decisions.** The full census (every
site, every script) is a generated file under build/ (regenerable, cheap
to diff); the tabularium inventory holds only rows that carry a DECISION -
excused sites, false-positive classes, rules and their fixtures.

**Q11. How does fabrica consume it?**
Options: new genus, retire fontationes / keep both.
**A: New genus 'effectus', retire fontationes.** Digests every resolved
read/source/exec in the summary (files by content, house binaries by
provenance), unresolved = IGNOTUM unless excused; porta_toml switches to
it; genus 'fontationes' removed once nothing declares it.

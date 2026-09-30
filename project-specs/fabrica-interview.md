# fabrica — interview raw (2026-09-29)

Feature: the house build layer (ledger parcum 01KZYN4VPZ, regio
'fabrica - makefile replacement'). Prompted by a day of tool-debt
work that measured the cost side of the build: 15 `build/`
directories, 1,737 objects, 5.5 GB; `piscina.o` and `chorda.o` each
compiled 24 times; `frigida_probare.sh` 14 suites ~14 min serial;
`generata` 381 s with no stage timings; `porta_vetustatis` run twice
(31 s of silva's 42 s); at least six bespoke notions of staleness
(mtime `newest_header`, existence, none — `bin/manus` stale six weeks
—, per-stage content compare, natura exit 3, content-hash keys and
receipts); stale installed binaries (`bin/manus`, the spectator); a
manual rebake ritual run ~15 times in one session. The park's bell
rule ("a seventh bespoke staleness check → start the fabrica") had
rung: `generata` stage VI, built the same day, was the seventh.

Pre-interview agreement (conversation):
- Inputs are MODULAR: whole files first; later granular views via
  silva/stml query selectors.
- Actions are MODULAR, with cross-cutting concerns (timing, logs,
  exit contract, deadlines, locks, output verification, receipts,
  scheduling) handled by the build layer as an ENVELOPE.
- A makefile-like DECLARATION file; source comments FEED it: an
  input is in essence a LIVE QUERY, and comments opt things into it.
- Claude's edges: granularity is per-CONSUMER (the amalgam reads
  comments, compilation may not; a too-narrow view = silent stale
  build; bytes default, narrow is opt-in; digest cost << action
  cost).

## Round 1 — structure

**Q1. Relation to the gate layer (portae, receipts, portae_debitae)?**
A: **Gates become actions.** A gate is an action whose output is a
verdict; receipts become fabrica records; "owed gates" becomes "stale
actions downstream of the diff". (Sequenced later — see Q20.)

**Q2. What is a recipe (Canon forbids an expression language)?**
A: **Typed action kinds.** A small registry of kinds in C (compile,
link, run a test, run a script, generate, install …) with named
parameters; declarations name a kind and fill parameters, no
templating.

**Q3. Provenance of committed generated files on a fresh clone?**
A: "**Self-describing or regenerate-and-compare, maybe this can be
modular too.**" → provenance is itself modular, chosen per artifact.

**Q4. First shippable slice?**
A: **Judge only** — a read-only report: stale or current, why, and
the healing command. Builds nothing.

## Round 2 — queries and trust

**Q5. Who owns the opt-in decision?**
A: **Pull: the declaration selects.** Comments state PROPERTIES; the
declaration's query picks by property; new matching files join
automatically; sources never name actions.

**Q6. An opt-in property no query selects (orphan, e.g. a typo)?**
A: **Warn.**

**Q7. Guard against an action reading something it didn't declare?**
A: "**We might be able to parse some of this stuff using crusta,
etc.**" → reads are DERIVED statically (crusta for shell, silva for
C, aedilis closures), not traced at runtime.

**Q8. Where do records live?**
A: **sqlite in `build/`** (vendor/sqlite; local, disposable,
queryable; history → timing trends).

## Round 3 — derivation, scheduling, scope

**Q9. Reads that can't be resolved statically (loops, globs, runtime
paths)?**
A: "**Lean to refuse, name the line** — but we can see how well the
idea works in practice and adapt."

**Q10. How does the planner know two actions may run at once?**
A: **Derived from outputs** — conflict iff declared outputs or
scratch directories overlap; parallel by default up to a process
limit (processes, never threads).

**Q11. Artifacts outside the repo (~/.bin/briar, spectator, the
briar cache)?**
A: **Yes, as installs** — registered artifacts with an install
action; the rebake ritual becomes one command.

**Q12. Division of work with aedilis?**
A: **Fabrica atop aedilis.** aedilis stays the dependency oracle
(closures, manifests, pure engine); fabrica is a new library above
it: declarations, queries, planner, registry, envelope.

## Round 4 — declarations, migration

**Q13. Where do declarations live?**
A: **Per subsystem** (`silva/fabrica.stml`, `briar/fabrica.stml` …)
next to the code; a root file lists the subsystems.

**Q14. Which document do selector queries run over?**
A: "**It could be conceptually clean to have certain modules have
their own build/creation dependencies — but we'll see where we are
using the CSS selectors in practice.**" → OPEN; indices are
module-owned artifacts; decided by the first real queries.

**Q15. Today's bespoke staleness checks once the fabrica judges the
same artifacts?**
A: **Oracle, then delete** — each old check becomes an oracle (a
gate asserts agreement); delete once they agree on real history.

**Q16. How is healing expressed?**
A: **An ordered plan** — all stale artifacts, dependency order,
printed as commands.

## Round 5 — cold start and self-report (after an explanation)

Fran asked what the empty-records problem was, and whether the
dependency graph didn't already answer it. Explained: the graph says
what an artifact SHOULD be built from; whether the file on disk WAS
built from today's inputs is history, which needs a record (make
uses mtimes as that record; we use digests, so the record is
explicit). A builder with empty records rebuilds everything (fine);
a JUDGE-only slice with empty records can't call anything current —
on day one, after deleting `build/`, and on every fresh clone.

**Q17. An artifact with no record?**
A: **Use provenance** — committed files: regenerate and compare;
installed binaries: ask the binary; neither = "stale: no
provenance", never assumed fine.

**Q18. Should installable binaries carry their own record?**
A: **Yes, one convention** — each installable tool answers one
standard flag with its input digest, written in at build time.

**Q19. Where should the judge surface?**
A: "**Any time that information is needed/appropriate**" (first
asked before the explanation; Claude to propose concrete places).

**Q20. Name — 'fabrica' is also briar's partition module?**
A: **Keep fabrica.** `briar_fabrica` keeps its name; the prefix
disambiguates.

## Round 6 — scope of slice 1

**Q21. Families registered in slice 1?**
A: **Installed binaries, committed amalgams, generated sources.**
(Canones/natura later.)

**Q22. Vocabulary of opt-in comments?**
A: "**Not sure — we'll have a better idea once we see what
necessitates the feature. We don't have to take advantage of this if
it's not necessary.**" → DEFERRED; queries over opt-ins are used only
where they earn their place.

**Q23. briar's own build/cache vs the fabrica?**
A: **Converge later** — briar out of slice 1, but its needs shape the
library API.

**Q24. Cleanup of `build/` (5.5 GB)?**
A: **Report orphans** — outputs no declaration produces, with sizes;
deleting stays manual.

## Round 7 — cost and sequencing

**Q25. Judge cost budget?**
A: **Two modes** — quick (self-reports, digests, records; no
regeneration; under ~2 s) for session start and hooks; full
(regenerate and compare) for commissio and on demand.

**Q26. Running the healing plan?**
A: **Slice 1b** — judge first (1a), then a thin executor that runs
the plan inside the envelope (timing, logs, exit contract).

**Q27. After slice 1?**
A: **Test runners** (old runner as byte-for-byte oracle), then the
shared object cache.

**Q28. The self-report flag's name?**
A: **`-provenientia`** (matches runae's PROVENIENTIA files).

## Round 8 — scripts and GUI gates

**Q29. Existing build scripts use loops and variable paths; with
"refuse unresolvable reads", wrapping them as-is would be refused
everywhere. How does slice 1 handle them?**
A: "**Maybe a standard way of querying the files (with comment
attributes potentially) — building up the list and passing it to the
next stage of the script; crusta lint could enforce, and fabrica
would derive the list using the same method.** Open to whatever is
most practical; a concrete example would help."

Claude's concrete example (from the real scripts):
- `tools/manus_struere.sh` already delegates to an aedilis-GENERATED
  `build/aedilis/manus_instrumentum/struere.sh` whose closure is in
  the aedilis manifest — no crusta needed for that tier. That script
  shares `build/aedilis/obiecta/` but judges staleness by mtime.
- Launcher snippets (`tools/*_fontes_generata.sh`) are COMMITTED
  query results (aedilis computed, frozen, `generata` III checks).
- `silva/amalgamare.sh` hand list + `"$RADIX_DIR/lib/$f.c"` loop →
  idiom: `FONTES="$(bin/fabrica quaere '<literal query>')"`; crusta
  checks the argument is a literal and that every read path comes
  from such a variable or is literal; the fabrica evaluates the SAME
  literal query, so script and judge cannot disagree.
- Cost: scripts calling `bin/fabrica` need it built first → one small
  bootstrap script (Go's make.bash pattern).

**Q30. Is the query idiom practical?**
A: **Yes, three tiers** — aedilis manifests for generated builds; the
literal-query idiom (lint-enforced) for hand scripts; explicit
declaration as the migration fallback; a bootstrap script builds
`bin/fabrica` first.

**Q31. The seed cache `build/aedilis/obiecta/` (mtime)?**
A: **Yes, digests there** — it becomes the fabrica's object store.

**Q32. GUI gates (-agere, pictor/forum fumus, the icon check)?**
A: **Exclusive kind** — a `fenestra` kind never runs alongside
another of its kind, and never unless explicitly asked.

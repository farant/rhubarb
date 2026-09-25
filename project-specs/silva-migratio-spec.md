# Silva → materia (phase 5) — state of play and migration process

2026-09-24 · planning pass (Fran: "materia has proven itself; not *if*
we migrate silva, just *how*"). Supersedes the phase-5/6 text of
`materia-spec.md` §10, which was written against a design that was
never built (`MateriaContextus`; phase-log note D). Measurements below
were taken today; nothing here is migrated yet. **Decisions for Fran are
marked ◆.**

## 1. Why now (the reordering case)

Decree **01M2KC03NG** (2026-09-15) set the order HTML → JS → silva. JS
was placed before silva as "the one-way lexer pipe's real test"
(regex-vs-division and template-literal nesting need parser→lexer
feedback). **That test was paid on 2026-09-16 by crusta**, on a harder
language: decree 01M2NJ1JR7, "the lector is a function of (mode,
position); the builder owns the mode stack" — 19 lexer modes, heredoc
bodies, backticks by depth, `$((` tried then rewound, all gates green
through materia. JS keeps its own consumers (a pure-C89 `js_syntaxis`
gate, the laboratorium) but no longer gates silva.

The spec's own promotion trigger for migrating early (§10, "Trigger that
promotes B": *the freeze gets expensive — work genuinely required inside
silva's core*) is met by work queued today:

- lapide bugs/009 (`va_arg(va, character*)` rejected) and bugs/010
  (`__attribute__((…))` rejected) — parser gaps a frozen silva cannot fix;
- located C syntax errors (briar bugs/001; examen and legati also print
  `nodi erroris (syntaxis) 1` with no location) — needs the parser to
  record where it died;
- the `latina.h` description line — blocked by silva's embedded copy
  (`silva_latina_datum.c`, pinned byte-equal by probatio_silva_contextus);
- format contracts / nominal typechecking will want frontend work.

◆ **D1. New decree: silva (phase 5) before JS.**

## 2. State of play (measured 2026-09-24)

**Substrate modules** — the six with a materia counterpart; phase 5
deletes silva's copies:

| silva | lines | materia | lines |
|---|---:|---|---:|
| silva_arbor | 7,063 | materia_arbor | 3,965 |
| silva_arbor_aequalitas | 1,023 | materia_arbor_aequalitas | 628 |
| silva_nodus | 990 | materia_nodus | 727 |
| silva_annotationes | 914 | materia_annotationes | 410 |
| silva_scribere | 850 | materia_scribere | 568 |
| silva_token | 428 | materia_token | 190 |

materia's are roughly half the size because the C89 specifics moved out
into registered frontend hooks. `silva_quaestio` (1,881) has no
counterpart — open item MG3. Everything else in `silva/fontes` is the
**C89 frontend** and stays: lexema, expandere (3,246), glr, parsare,
commissio, conditio, contextus, tabulae (c89 17,869 · imparilis ·
sceleti), c89_semantica (17,471), c89_fluxus(_datorum), c89_oraculum,
latina_datum, quaestiones.

**materia gained since the fork** (silva lacks all of it): lexicon,
coctor (registrum coctum), sedes, diagnostica (declared shapes +
`emissa`), pictor, exemplaria, excusatio; REFERENTIA loci; visio
projections; the STML clausura escape ladder. Migration hands all of it
to C89 for free.

**The shim is the migrated frontend's prototype.**
`materia/instrumenta/shim_c89.c` (1,462 lines) converts a silva tree to
materia types, emits through `materia_scribere`, and compares against
silva byte for byte, through STML twice, and by the tree comparator.
Today: **400/400, fractae 0, 15 s** (392 at the HTML boundary). Its
token tail ("cauda lexematis = datum frontis C89") is, in its own words,
"the very shape the C89 frontend of phase 5 will have".

**Freeze replay debt: small.** 10 commits touched `silva/fontes` since
2026-08-27; only two touched substrate modules — a whole-tree format
pass (cosmetic) and `1373a3e8` (refactor, judged no-replay in the
phase-log). One latent silva bug is queued for the phase-5 replay: CR
representation (found by the CSS client, fixed in materia via the `cr`
attribute, B6).

**Consumer surface: small.** Outside silva/materia, substrate API use
is ~24 distinct functions — `silva_token_radix` 14, `silva_nodus_
extensionem` 10, `silva_scribere_nodum`/`_fontem` 10,
`silva_nodus_extensionem_lineis` 4, `silva_arbor_*` a handful,
`silva_quaestio_*` 5 — in officina (12 files: legatus, compendium,
praeparator…), briar (4), saltuarius (5). A few walk node fields
directly (briar's `_errorem_quaerere`: `->loci`, `numerus_locorum`;
compendium: `->pater`). silva's own instruments (examen, formator,
nexus, renominare, selecta, censor…) are internal and migrate with it.

**Acceptance bars (§7 M3) — re-measured 2026-09-24 (plan T1), one
runner `./silva/m3_probare.sh` (gate `silva-m3`, ~65 s):**

| bar | source | 2026-08 | 2026-09-24 | pin |
|---|---|---:|---:|---|
| subtree round trip | `probatio_silva_arbor_circuitus` (M1, oracles A and B) | 281 | 281 | fixed |
| plain-C | `probatio_silva_arbor_plagula` (M2, plain corpus) | 78 | 78 | fixed |
| latinized | `probatio_silva_arbor_plagula` (`lib/*.c` + closures) | 154 | 182 | live ≥ 182 |
| hospes | `instrumenta/principalia/hospes.c` over the COMMITTED amalgam | 39 | 40 | fixed |
| adversarial | `probatio_silva_arbor_parsura` T7b | 24 | 24 | fixed |
| haruspex | `silva/haruspex.sh` (TUs vs clang) | 243 | 279 | live ≥ 279 |

Fixed = frozen corpus, exact count (a move is a commit with a cause).
Live = corpus grows with the tree: all must pass AND the count must
not fall below the floor; a rise is a note, not a failure. Hospes
judges the committed amalgam, so it moves only when `amalgamare.sh`
regenerates it (silva/CLAUDE.md: after ANY fontes edit).

## 3. The key risk the old plan does not name: the oracle disappears

Today the shim's oracle is **silva itself** — materia's output is judged
against the live silva emitter. The migration *replaces* that emitter.
Mid-migration, "compare against silva" compares the new code to itself.

So before the first substrate switch, **freeze the oracle — as a
program, not a snapshot** (Fran, 2026-09-24: "we should be able to make
an amalgam snapshot of silva"). `silva/amalgama/silva.c` (90,292 lines)
+ `silva.h` has ONLY system includes; measured today, copied out of the
tree it compiles alone under the house flags (3.6 MB object). So:

- **`oraculum_silvae`**, a standalone tool built from
  `git show <pin>:silva/amalgama/silva.{c,h}` extracted into `build/`
  (the pinned commit IS the oracle — no 90k-line copy committed). Given
  a file it prints what the gate compares: byte round trip, STML
  projection, tree-comparator digest, semantic verdict.
- **A separate process**, so the frozen silva's symbols never meet the
  live binary's (which links materia + the new frontend mid-migration).
- **Stronger than a static snapshot**: it judges ANY input — the shim
  corpus (400), the M3 corpora, new files, fuzz cases (crusta
  totalitas style) — on demand. Same pattern as crusta's fixed external
  oracle (`/opt/homebrew/bin/bash` 5.2).
- **Intended divergences are NAMED**, never counted away: the CR fix
  (step 2), later bugs/009-010 (step 5). A named-exceptions list like
  crusta's oracle ("every dispar named, the naming pinned"), so a fix
  that breaks something else cannot hide behind an unchanged total.

Every migration step is judged against the oracle; the live corpus keeps
invariants (byte round trip) only — house doctrine: *a snapshot needs a
frozen input*.

## 4. Proposed process

◆ **D2. Incremental behind a facade (recommended) vs. big bang
(§10's 5.1: delete, include materia, regenerate).** Big bang breaks
every consumer in one commit and has no intermediate green state; the
house gates want every commit green. Incremental: silva's public names
become a facade over materia's types (`nomen MateriaNodus SilvaNodus;`,
thin wrappers or macros for the ~24 external functions), then modules
switch one at a time, each step fully gated.

**Step 0 — measure and freeze (no silva change).**
- Re-measure every M3 bar with today's suites; name each bar's script.
- Build `oraculum_silvae` from the amalgam at a pinned commit (§3) and a
  gate that diffs live silva against it over the shim + M3 corpora —
  born red by a plant in the live emitter.
- Replay inventory as a tabularium inventory (rows = silva commits since
  the fork + the CR bug; lens "replicatum").
- Consumer inventory (rows = files using the facade surface; lens
  "per facadem transit").

**Step 1 — promote the shim into the C89 frontend.** `shim_c89.c`'s
conversion and hooks become `silva/fontes/silva_frons.c` (name ◆): the
registered frontend hooks (origin write/read, sections `<fontes>` /
`<regio-*>`, extent lookup, reinserenda collector). The shim gate keeps
running unchanged (it now exercises production code).

**Step 2 — switch modules, one per commit, leaves first.**
token (C89 data moves into the frontend tail) → nodus → scribere →
arbor + aequalitas (the big one: 7,063 → 3,965 + frontend sections) →
annotationes. Each commit: silva suite + shim + snapshot comparison +
consumer suites (officina/legatus, briar, saltuarius) + the M3 bars it
touches. The CR latent bug is fixed at the scribere step, on purpose,
with its own test.

**Step 3 — quaestio (MG3).** ◆ **D3.** Options: (a) port
`silva_quaestio` onto `MateriaNodus` and keep it in silva (a query
language over C89 trees); (b) move it into materia as a generic tree
query (M8 "thin" argues against); (c) replace with `selectio` (CSS
selectors over STML) through the projection — cheapest API, but a
projection per query costs. Recommend (a): smallest change, MG3 decided
by what C actually needs, revisit when a second client wants queries.

**Step 4 — delete and seal.** silva's substrate copies deleted, amalgam
regenerated (`silva.h` = frontend + facade), **M3 acceptance** against
the re-measured bars and the frozen snapshot. The freeze ends here.

**Step 5 (5.x) — the payoff: C89 in the diagnostics pipeline.**
- A C89 registry *declaration* (`registrum.stml`, like crusta's) so
  `registrum coctum` and declared shapes apply; `lexicon_c89` (today
  generated for the shim from silva sources) finds its real home.
- The GLR records the token where all heads died (+ expected kinds if
  cheap) and emits it through `emissa`: primary sedes "hic exspectatur",
  relata "hic coepit" at the unit start — the two-location shape clang
  prints. examen, legati and briar get located syntax errors at once,
  printed by `lib/excerptum`.
- bugs/009 (`va_arg` with a pointer type) and bugs/010
  (`__attribute__`) become ordinary frontend fixes.

**Step 6 — cleanup** as §10 (LR toolkit's home, MAP, census, MEMORY).

## 5. Meanwhile (does not wait for phase 5)

briar relays clang's diagnostics when silva fails, and names "clang
accepts it" as a silva limitation — clang-quality errors for thistle
authors now, and the only honest classification of bugs/009-010 until
step 5. When step 5 lands briar switches to the native path and keeps
clang only as the second opinion.

## 6. Decisions — DECIDED by Fran, 2026-09-24

| ◆ | decision | decided |
|---|---|---|
| D1 | silva before JS (new decree superseding the order in 01M2KC03NG) | **yes** |
| D2 | incremental facade vs big bang | **incremental** |
| D3 | quaestio's home (MG3) | **port onto MateriaNodus, stays in silva** |
| D4 | frontend module name | **`silva_frons`** |
| D5 | is step 5 (diagnostics) inside phase 5 or its own arc | **inside, as 5.x** |

## 7. Next

On Fran's decisions: a task plan (`silva-migratio-plan.md`) with one
task per step-0 item and one per module switch, each with its gate; a
park in the tabularium under Materia Parsers → c89 parser.

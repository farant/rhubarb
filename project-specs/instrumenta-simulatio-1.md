# Instrumenta prima — simulatio 1

*2026-07-10, during the M2-seal celer blessing run. Simulated
implementation of the three bootstrap tools named in
silva-instrumenta-visio.md §XIII (latina lint, nexus/emitte CLI,
interpretare), walked against the real codebase as known at 75/80.
Purpose per house method: surface the complications BEFORE building.
Findings C1-C5; the sim killed one cost estimate and re-derived one
architectural pressure independently.*

## Scope simulated

Three CLI tools (NOT MCP — the CLI-before-MCP correction stands; a C89
JSON-RPC server is a project, a CLI tool is an afternoon, and Claude
calls CLI via Bash today with zero plumbing):

1. Latina-landmine lint (silva-side instrumentum)
2. `nexus <symbolum>` (all use-sites) + `emitte <plagula> <symbolum>`
   (byte-exact definition) (silva-side)
3. `interpretare <functio> [argumenta]` (officina-side)

## C1 — the latina lint is an ORIGO query, not a symbol-table query
## (the sim's biggest kill)

The pre-sim estimate ("pure query over shipped symbol tables, an
afternoon") is WRONG, because of expansion order: the corpus is written
WITH latina.h, so by the time silva's tree exists, `nomen` in source has
already expanded to `typedef`. The tree never contains the landmine
identifier — it contains the expansion. The `exire` firing is the
instructive case: a struct field named `exire` becomes a field silently
named `exit` — VALID code, no diagnostic anywhere, only the origo chain
knows.

So the real query is: **name-binding tokens (declarator names, member
names, parameter names, labels) whose origo shows they were produced by
expanding a latina.h macro.** Legitimate latina usage (si/redde/per as
KEYWORDS) expands in keyword positions; the landmine is latina expansion
landing in a NAME position.

Consequences:
- SPIKE FIRST (half day): verify per-token origo exposes macro identity
  + definition header ("this token came from expanding macro X defined
  in latina.h") through public silva.h rows. Expansion layers exist and
  vindex's macro-aware stepping is a pin, so the data is there; whether
  the QUERY surface is public determines whether the lint needs a new
  row (ceremony: row + hospes call, same change).
- The define list itself: v0 text-scans latina.h for `#define (\w+)` —
  40 lines, no new surface, honest.
- Revised estimate: 1-2 days after the spike, not an afternoon.
- Output discipline: percursus-style classified rows; the historical
  firings (nomen/exire/magnitudo/registrum/structura...) become the
  regression fixtures — the lint must find ALL PAST FIRINGS if run
  against pre-fix commits (a beautiful validation corpus we already
  own: git history has every landmine as a before/after pair).

## C2 — cold start is the real enemy of the read tier

`nexus` as naively simulated: parse corpus (~103s arbor + semantica) per
CLI call. A 100-second find-references is NOT a grep replacement (grep:
50ms). This is the sim's second finding and it re-derives the acta
pressure independently: the read tier wants persistent derived state.

Honest v0 answers, in house style (boring, works today):
- **emitte / typus: single-TU mode** — parse ONE file + its includes
  (~200ms-2s). "Show me this function byte-exact" doesn't need the
  world.
- **nexus: sweep-then-grep** — a percursus-shaped `nexus_percursus`
  dumps EVERY symbol's use-sites to build/nexus.tsv ONCE (one corpus
  parse); the `nexus` CLI greps the tsv (ms). Regenerate when stale
  (mtime check + honest warning). This is a derived-fact cache — acta's
  first table wearing a .tsv costume, DELIBERATELY disposable so the
  schema ceremony stays unconstrained. The dogfooding experience feeds
  the ceremony.
- NAMED NON-GOAL: no daemon/server mode in v0. That answer properly
  belongs to M4 (REPL) and acta.

## C3 — interpretare is a wrapper-module generator (no new machinula
## surface at all)

The clean shape found by simulation: interpretare does NOT need arg
marshalling in machinula. It generates a tiny .medulla WRAPPER MODULE
(the inline-module pattern from probatio_officina_machinula, proven
today by the tempus and ansae tests): datum rows for string literals,
`functio $main` that calls the target with the literal args and
printf's the result by declared return type. Then: cursor's world-link
+ machinula_currere("main"). Everything exists; the tool is a text
generator plus cursor guts.

- Scalars + string literals v0; aggregates = named deferral (print hex
  via a legere loop if wanted later).
- Return-type-aware printf: signature from the conexio (interpreted
  functions carry parametra/reditus).
- The wrapper module is TEXT — trivially inspectable/debuggable, and
  every interpretare invocation is reproducible by keeping the wrapper.

## C4 — interpretare's cold start: manual scoping v0

Whole-world link = ~2 min cold start (arbor dominates), killing the
quick-hypothesis loop the tool exists for. Auto dependency-closure
(lower target TU, chase undefined symbols to their providers) is real
design work — v1, and it informs M4. v0: `-plagulae <list>` manual
scoping (user names 3-5 TUs; typical closure per spec-v2 measurements:
piscina=4, stml=10) → seconds. Fallback: no list = whole world, honest
2-min warning. Plus: today's sputnik hunt would have been
`interpretare -plagulae sputnik_*,chorda,piscina,internamentum ...` —
the manual list is not a real burden for the person who just read the
failing suite.

## C5 — placement, naming, gates

- Homes: silva/instrumenta/principalia/ (lint, nexus_percursus, emitte)
  + officina/instrumenta/principalia/ (interpretare). Shell wrappers per
  percursus.sh/fusor.sh precedent.
- NAMES ARE FRAN'S CEREMONY. Working names used here: lint =
  "custos_latinae"?, nexus CLI = "nexus", emitte = "emitte",
  interpretare = "interpretare" (the visio doc's proposals). Not sealed.
- Gates per tool: lint = the git-history firing corpus (C1); nexus/emitte
  = spot-check vs grep on 10 symbols + byte-diff emitte output against
  the source extent; interpretare = re-run today's sputnik repro + the
  42-fixtures as smoke.
- Books: each tool gets a worklog entry; one phase-log addendum for the
  phase; the sim doc (this file) updated with RELATIO when done.

## Revised budget

| item | estimate |
|---|---|
| Origo-provenance spike | 0.5 d |
| Latina lint | 1-2 d (spike-dependent) |
| nexus_percursus + nexus CLI | 1 d |
| emitte (single-TU) | 0.5 d |
| interpretare v0 (wrapper-gen + -plagulae) | 1-1.5 d |
| **Total** | **~4-5 d** |

Cut line if sprawl: lint + emitte ship first (highest pain-per-day);
nexus sweep second; interpretare third (it's also the most fun, which
is exactly why it goes last).

## Open questions for Fran

1. Names for the four binaries (ceremony).
2. Does the lint run standing in compile gates (amalgamare/suite) or
   on-demand only? (Recommendation: on-demand until false-positive rate
   measured on one full corpus run, then standing.)
3. Is the .tsv derived-cache acceptable as EXPLICITLY disposable
   pre-acta scaffolding, or would you rather wait for the acta ceremony?
   (Recommendation: disposable scaffolding, in build/, never committed.)
4. interpretare v0 scalars-only — acceptable? (Aggregates print as hex
   dump later.)

## ADDENDUM — simulate-work pass on Day 1 (the lint), same evening

Ran the house technique (pretend-implement with five-minute ticks, real
research allowed) on the lint's first day. Two findings that the
analytical pass above MISSED:

**A1 — the spike is already answered: origo surface is public and
sufficient.** SilvaOrigo is embedded in every token ("numquam absens");
the EXPANSIO arm carries nomen_macro directly. Check =
`origo.genus == SILVA_ORIGO_EXPANSIO && nomen_macro ∈ latina-set`. No
new silva.h row. Half-day spike deleted; lint estimate drops to ~1 day.

**A2 — position discrimination is the load-bearing wall (not the origo
check).** EVERY keyword in the corpus is a latina expansion (si, redde,
per...) — flagging by expansion alone flags every line. The tool's real
work is the NAME-BINDING SLOT INVENTORY: declarator names, member
names, parameter names, enum constants, labels, typedef names. Hand
listing = the AEDIFICATA lesson waiting to re-fire. DESIGN DECISION OF
THE DAY: derive the inventory mechanically from the generated accessor
table (the generator already knows which genera carry tok_titulus-shaped
slots) — coverage becomes drift-impossible.

Also surfaced: per-file latina.h-inclusion awareness (knotapel English-C
files can't fire; the expander knows the include set), and the
validation harness sharpened — ten historical firings × `git show`
pre-fix blob × single-TU lint run × assert-fires-at-recorded-line = the
tool's credo suite, entirely from history we already own.

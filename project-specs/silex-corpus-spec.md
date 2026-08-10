# Silex corpus — the embedded library registry (spec)

2026-08-10. Approved design from brainstorm. Closes the heart of
desideratum 01KZ6CSBQN using vocabularium's six measured gaps as the
requirements list, and completes deferred vision 01KZ4Y9A8T ("single
binary carrying all library sources — capsula, deferred").

**One sentence:** silex carries the rhubarb library corpus inside
itself as a capsula, so `novum`/`renovare`/`partes` work anywhere;
rebuilding silex (which already installs to `~/.bin`) IS the
library-update workflow.

## Rulings from the interview

| Fork | Ruling |
|---|---|
| Scope | Full «unplug the fabrica» criterion: corpus capsula + renovare re-rooted + `silex partes`. novum `-vitrea` and generated-script tiering stay OUT (next pull). |
| Corpus contents | Lib tier only: `lib/*.c` + `lib/*.m` + `include/*.h` + `vendor/*` + `tools/capsula_generare.c` (~24MB raw; 145 sources + 143 headers measured 2026-08-10). No subsystem amalgams. |
| Resolution order | `-f` > ascent-from-cwd > embedded corpus. **SILEX_FABRICA is RETIRED** — branch deleted from the chain, export removed from `~/.zshenv`. The shipped path must be the dogfooded path. |
| Dirty-tree discipline | Corpus stamped with fabrica git commit + build date; dirty tree adds a `SORDIDUM` marker. Build and install proceed; provenance stays honest. |

## Why (the measured motivation)

- Vocabularium (first vitrea scaffold, 2026-08-09) measured six gaps,
  filed on 01KZ6CSBQN. The sharpest: **renovare never re-roots the
  closure from project fontes** — `lib/silex.c:1659` says it in one
  comment: "semina = capita vendicata". New project code can never
  pull ADDENDA.
- Every fabrica-reach during that build was a missing verb: the
  closure oracle demanded `cd rhubarb && bin/aedilis ... --partes`
  (plus a probe file written INTO the fabrica), vendoring was
  hand-`cp` of 13 pairs across two rounds.
- Posture already shipped (6e4d446): silex installs to `~/.bin` via
  struere; cwd verbs (status/condere/historia) proven
  fabrica-independent. This spec finishes the other half.

## Design

### 1. The corpus capsula

`tools/silex_struere.sh` gains a corpus step before compiling silex:

1. **Generate** `build/corpus_silicis.toml` from the tree — never
   hand-listed: all `lib/*.c`, `lib/*.m`, `include/*.h`, `vendor/*`,
   `tools/capsula_generare.c`. Capsula keys = fabrica-relative paths
   (`lib/chorda.c`) so the corpus is a drop-in mirror of disk layout.
2. **Stamp**: one synthetic entry `corpus.versio` containing
   `commit=<git rev-parse HEAD --short>`, `dies=<ISO date>`, and
   `SORDIDUM` if `git status --porcelain` is non-empty (measured at
   struere time, in the fabrica).
3. **Embed**: run `capsula_generare` (compression ON for the corpus
   if the flag is functional — sqlite3.c dominates the 24MB; if
   compression is not usable, accept the size), compile the generated
   pair into silex as a second embed beside `capsula_silex_frons`.

`silex -versio` prints the silex version AND the corpus stamp.
`novum` output names the stamp ("vendicatae ex silice <commit>").

### 2. The lector seam — SilexFons

Exactly one function reads library files today:
`_plagulam_e_fabrica_colligere` (lib/silex.c:183 — `filum_existit` +
`filum_legere_totum` against `fabrica + "/" + pars + titulus`). It
becomes `_plagulam_e_fonte_colligere` over a small seam:

```
SilexFons — genus DISCUS (fabrica cstr) | CORPUS (Capsula*)
  fons_existit(fons, via_relativa)          -> b32
  fons_legere(fons, via_relativa, piscina)  -> chorda (+inventum)
  fons_titulus(fons)                        -> for messages/stamps
```

`silex_clausuram_colligere` takes `SilexFons*` instead of the fabrica
path. Fons resolution (shared by novum/renovare/partes):
`-f <via>` > ascent from cwd (`silex_fabricam_invenire`, unchanged) >
embedded corpus. The `getenv("SILEX_FABRICA")` branch
(tools/silex.c:480) is DELETED; the export is removed from
`~/.zshenv`; all usage text updated. A fons is always resolvable now
(the corpus is compiled in), so the "fabrica ignota" refusal paths
become dead code and are removed — after this spec no verb needs a
disk fabrica at all.

### 3. Closure rules complete (.m + vendor)

Two additions inside `silex_clausuram_colligere`, serving all verbs
and both fons genera identically:

- **Platform twin**: at the existing `.h → .c` twin step
  (lib/silex.c:281), when `lib/X.c` does not exist in the fons, try
  `lib/X_macos.m`. (Convention source: aedilis. Only `_macos` in v1 —
  the only platform that exists.)
- **Vendor pair**: an include of `vendor/Y.h` (any prefix — scrinium
  writes `../vendor/sqlite3.h`) collects `vendor/Y.h` and, if
  present, `vendor/Y.c`, with origo `vendicata:vendor/Y.h` etc.
  No struct change: the `vendor/` prefix in `SilexRes.via` IS the
  marker — callers that need the vendor tier test the via prefix.

NOT in scope: teaching novum to emit tiered aedificare/probare
scripts (vendor rule, flag battery, per-target lists). That is the
`-vitrea` pull; this spec only guarantees the closure WALK knows the
truth so renovare can vendor the files and partes can report them.

### 4. Renovare re-rooted

Seed set changes from "capita vendicata" (lib/silex.c:1659) to the
UNION of:

- includes scanned (`_inclusiones_scrutari`, existing) from the
  project's **authored** manifest plagulae — every manifest via NOT
  under `lib/`, `include/`, `vendor/` and not a generated
  `capsula_*` pair;
- the currently-vendored `include/` set (continuity — nothing that
  worked before stops being tracked).

Everything downstream (three sigilla, RENOVANDA/ADDENDA/VULNUS/
CONFLICTUS/DERELICTA classes, one-transaction apply, hold-back
semantics) is UNCHANGED. `missum-novum` reads through the resolved
fons — i.e., by default "what this silex ships." The renovare actum
gains the corpus stamp in its datum.

**Pre-fix oracle (law):** the vocabularium scenario — authored file
gains `#include "volumen.h"`, renovare says "omnia recentia" — must
be reproduced as a failing test against current code BEFORE the fix,
in the fabrica ficta harness.

### 5. `silex partes` — the closure oracle as a verb

`silex partes [plagula ...]` from a project directory:

- Seeds: the named files; default = all authored fontes (same
  authored-set rule as renovare).
- Walks the closure through the resolved fons and prints the aedilis-
  style classification (O source / C header / V vendor / .m platform)
  PLUS an **ABSENT** column: entries missing from the project's own
  tree — exactly what `renovare -scribere` would vendor. Oracle and
  dry-run in one verb.
- `-machina` for TSV (house convention; exit 2 = nothing ran).

This retires both fabrica-reaches from the vocabularium session: the
`bin/aedilis` runs and the probe-file-in-fabrica trick.

### 6. Error handling

- Corpus embed missing/corrupt (should be impossible post-struere):
  loud refusal naming the three resolution sources tried.
- `-f` pointing at a non-fabrica (no `include/`): existing refusal
  stands (lib/silex.c:230).
- A `.h` with neither `.c` nor `_macos.m` twin: header-only lib,
  normal, no warning (current behavior).
- Vendor include with no vendor source in fons: warning, not death
  (matches existing citata-sine-fonte posture).

## Testing

In `probationes/probatio_silex.c` (existing fabrica-ficta harness):

1. **Differential fons gate (strongest):** build a tiny hand-made
   `CapsulaEmbed` mirroring a fabrica-ficta subset; closure walk over
   DISCUS fons vs CORPUS fons must be identical (count, viae, origo,
   contenta).
2. **Platform twin rule:** ficta gains `include/x.h` +
   `lib/x_macos.m` (no `lib/x.c`) → closure contains the `.m`.
3. **Vendor pair rule:** ficta source includes `../vendor/y.h` →
   closure contains `vendor/y.h` + `vendor/y.c`, marked vendor.
4. **Renovare re-root (pre-fix oracle FIRST):** authored file in a
   ficta project gains a new include whose lib is not vendored;
   current code reports no ADDENDA (test must FAIL before the fix,
   verified); after: ADDENDA names the pair.
5. **Partes:** on a ficta project, output lists the closure with the
   ABSENT column matching what renovare would add; `-machina` TSV
   parses; exit codes honest.
6. **Stamp:** corpus stamp entry present and printed by `-versio`
   (struere-level smoke, not unit).

Suite runs via `./compile_tests.sh silex`.

## Acceptance criterion (files on 01KZ6CSBQN when met)

Rename the rhubarb directory. From a fresh terminal:
`silex novum probandum -d ~/Documents/projects/silicetum` → add an
include to its fontes → `silex partes` names the need → `silex
renovare -scribere` vendors it → `./aedificare.sh && ./probare.sh`
green → `silex condere`. An entire project session, in-project,
silex verbs only. (Vitrea scaffolds still need the hand tiers in
their build scripts — that is the deferred `-vitrea` pull, not this
criterion.)

## Out of scope

`novum -vitrea` seed and tiered script generation; per-target source
lists in generated scripts; flag-battery-as-data; multi-platform
(`_linux.m`?) twins; corpus subsetting/curation; capsula compression
work beyond flipping the existing flag; any aedilis code changes.

## Workflow after this ships

- Update libraries everywhere: `cd rhubarb && ./tools/silex_struere.sh`
  (rebuild + reinstall), then `silex renovare` per project.
- Dev on live sources: work inside the rhubarb tree (ascent) or pass
  `-f`.
- `~/.zshenv` loses `SILEX_FABRICA` as part of this work.

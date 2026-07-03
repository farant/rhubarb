# saltuarius — orientation

*You are reading this because you touched a file in saltuarius/.
This is the canonical orientation: vision pins, protocol,
build/test. The silva/CLAUDE.md discipline applies here wholesale;
this file carries only what is saltuarius's own.*

## Quid sit

Saltuarius is the forest warden: a TUI code explorer —
ranger-style miller columns outside, a silva-powered source view
inside (lexeme coloring, layer flips, origo chains, conditional
regions). The first true APPLICATION in the monorepo and the
real-host proof for both amalgams. The money shot: cursor on a
token → origo popup (macro def ← include path ← layer) → Enter
jumps there. Go-to-definition for the preprocessor.

Design record: project-specs/saltuarius-interview.md (24
decisions) → saltuarius-spec.md (v1 intent) →
**saltuarius-spec-v2.md (codebase-grounded — the implementation
truth; §2 is the silva API-pressure ledger, §10 the resolved
decisions)**.

## PROTOCOLLUM

Same as silva's (silva/CLAUDE.md): phase-log.md tail first at
session start / post-compaction; INTENTIO before a phase, RELATIO
after; complexities in the four-part schema; never implement from
a compaction summary alone.

## Build/test

- `./saltuarius/compile_probationes.sh [filter]` — the suite.
  Headless: tessera memoria pons + fixture listings; no tty, no
  real filesystem in view-model tests.
- `./saltuarius/saltuarius.sh [radix]` — build + run the app on a
  real terminal (defaults to the rhubarb root).

## VISIO — pins with their protecting consequences

- **The amalgam boundary is the API.** Include silva/amalgama/
  silva.h and tessera/amalgama/tessera.h ONLY — never either
  project's fontes/ (compile flags don't even offer them). Where
  the amalgam surface falls short, that gap is a deliverable
  API-pressure finding (spec-v2 §2 ledger), fixed in silva/tessera
  through their own gates — never worked around by reaching in.
- **Monorepo citizen for its own state**: piscina, chorda, xar,
  tabula_dispersa, filum, via, iter_directoria, utf8 are ordinary
  dependencies.
- **View-models never touch the filesystem.** The disk edge lives
  in the thin main + nexus: production fills tabularium from
  iter_directoria; probationes fill it from literal fixture
  arrays. Injection IS the test seam — no filesystem vtable.
- **Goldens assert CELLS, not bytes.** TesseraOpus is transparent
  and cellulam_legere reads the back buffer — render, then assert
  content/styles WITHOUT tessera_praesentare. Byte-level goldens
  live in tessera's suite, not here.
- **Every file always opens** (fines degrade, never break): worst
  case plain text + a truthful status badge. Browsing never dies
  because parsing did.
- **Frame discipline** (tessera's rules, inherited): repaint the
  whole logical screen each frame (the diff is cheap); app-side
  chorda_aedificators pre-size + reset per frame, NEVER finire;
  never index opus->tergum directly (fixed 512 stride); draw only
  within tessera_latitudo()/altitudo().
- **Forbidden key bindings**: Ctrl+I (≡Tab), Ctrl+M / Ctrl+J
  (≡Enter), Ctrl+H (≡Backspace) — classic-lossy xterm aliasing.
  gg is app-side pending-key state.
- **Parse memory is real**: 84–242 MB peak arena PER ROOT
  (measured). LRU cache N=8, one piscina per root, evict =
  destruere. Parse-on-open shows a transient status message; no
  async machinery.
- **Public API names ≤ 31 chars** (C89 significance).
- Latin in the code; English fine in meta-documents. `nomen`,
  `registrum`, `magnitudo`, and `exire` are latina macros
  (typedef/register/sizeof/exit) — never identifiers, INCLUDING
  struct field names (exire silently renamed a field in every
  modular TU; only amalgamation exposed it). i32 is UNSIGNED —
  s32 for anything carrying -1.

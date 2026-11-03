# OFFICINA BIBLIOTHECAE — libraries found by #include

**STATUS: IMPLETUM 2026-11-02** (secunda) — T1 7174cda5 (colloquium
links again + launcher gate), T2 280e4f78 (bibliothecae resolver),
T3 c5388bf2 (sessio #include turns load libraries), T4 (interpretare
derives by default). Acceptance VI.1-6 all held; see officina/
phase-log.md "OFFICINA BIBLIOTHECAE: RELATIO".

*Designed with Fran 2026-11-02 (brainstorm in chat, secunda, after
chronica C6). Fran's direction: build up evergreen toolchain pieces
first (interpreter gaps, then aarch64), and try things like the vicus
IDE (ledger desideratum 01M6F0SY) once the pieces exist. This is the
first piece. Latin names below are proposals until each step's header
text is approved.*

## I. Intent

The interpreter (machinula, through sessio / interpretare /
colloquium) runs house libraries correctly once their `.c` files are
loaded: `./officina/interpretare.sh -derivare
piscina_generare_dynamicum '"x"' 1024` -> BENE, 65 instructions
(measured 2026-11-02). But today the person running code must NAME
those files: `-plagulae a,b,c` on the command line, or a hand-made
list in C. Miss one and the run stops with
`decipula vocata: piscina_generare_dynamicum` (ledger desideratum
01KYB9JM, hit twice). The #! fixture shows the cost exactly:
`officina/probationes/fixa/sessio_scripta/salutatio.scriptum`
includes three headers (piscina, chorda, filum), and its test
(`probatio_officina_sessio.c:850`) lists FIVE files by hand -
precisely what `bin/aedilis lib/filum.c --enumerare` prints.

**The rule this spec establishes: `#include` is the declaration.**
Wherever the interpreter runs C, the libraries it needs are the
closure of the code's own `#include "x.h"` lines, found by aedilis
(the house build truth), never a hand list.

Why now: every later piece leans on it - briar running thistles
interpreted (instant reruns; next project), the vicus IDE (a program
page whose app re-runs on every edit), vindex on library code.

## II. Goals

1. **One shared C function** that turns a set of include directives
   (or a source text / file) into the ordered list of `.c` files the
   interpreter must load, by calling `aedilis_derivare` (C API,
   include/aedilis.h:199; extractor from include/aedilis_silva.h).
   Consumers: sessio, interpretare; later briar and the IDE.
2. **The REPL (sessio/colloquium).** A turn `#include "chorda.h"` -
   at the start or in the middle of a session - loads lib/chorda.c
   and its closure; the next turn can call chorda functions. No
   `-plagulae` needed.
3. **#! scripts.** `salutatio.scriptum` runs with no plagulae list;
   the D2 test drops its hand list of five.
4. **interpretare.** Without `-plagulae`, the target function's own
   closure is derived (today's experimental `-derivare`, done in C
   rather than awk, becomes the default). Today the default is "all
   of lib/" with a warning.
5. **colloquium builds again** (ledger parcum 01M4XR3Q: link fails on
   `_nexus_ordines_capitis_gradus` since 8bbf97f7, confirmed
   2026-11-02) - and a gate runs the launcher, so it cannot rot
   silently again (nothing ran it; that is why it broke unnoticed).

## III. Non-goals

- briar running `.thistle` files interpreted (next project; builds on
  this).
- The aarch64 native backend (M5, parcum 01KXJ2NV; after this).
- Missing system-call bridges in machinula (open/write/close) and the
  `magnitudo` of a static array under FABRICA_LECTIONES (parcum
  01M4XR3N). A library that loads fine but calls an unbridged system
  function still traps at call time, as today.
- Unloading libraries when an include turn is edited away.
- The corpus sweep (`cursor.sh`) and its world-link: unchanged.

## IV. Anti-goals (failure even if it "works")

- A second closure engine or a hard-coded header -> file table.
  aedilis (with aedilis.stml: irregularia, corpus/obiectum
  annotations) is the one truth; the interpreter asks it.
- Silent partial loads. If the closure holds something the
  interpreter cannot run, say so by name before running, never trap
  later with a mystery.
- Losing `-plagulae`: it stays, as an explicit override/addition.

## V. Decided (Fran, or Claude's call marked)

- **aedilis, not silex**, is the engine (Claude's call). aedilis is
  the house build truth and already powers `interpretare -derivare`;
  silex scans textually from an embedded corpus and serves briar
  outside the repo - briar's interpreted mode may need silex later,
  which is why the shared function should hide the engine behind its
  own interface.
- **Variant `posix`** (Claude's call). aedilis.stml prefers
  `macos posix` (aedilis.stml:62), which picks `x_macos.m` first;
  the interpreter cannot run Objective-C, so derivations ask for
  `posix`.
- **Mac-only headers are caught by deriving twice** (Claude's call,
  after the builder check). Under `posix`, aedilis's probe
  (`_obiectum_probare`, lib/aedilis.c:602: `X_<var>.m`, `X_<var>.c`,
  `X.c`) finds NOTHING for a header like fenestra.h and silently
  adds no object - so a posix-only derivation would load fenestra.h
  and trap later, the anti-goal. Rule: derive once with `posix` (the
  load set) and once with the configured preference (`macos posix`,
  detection only). Every `.m` in the second whose header
  (`AedilisObiectum.caput`) has no object in the first is refused,
  naming the `.m` and that header. aedilis itself is NOT changed.
  Cost: two derivations, ~0.2 s cold.
- **What loads is a deny-list** (Claude's call): every `.c` file
  aedilis resolves loads, except: `.m` (above), vendor sources,
  objects marked absens, and files over interpretare's existing
  4 MB limit (`mensura_maxima`, interpretare.c:45 - e.g. the silva
  amalgam behind `#include "silva.h"`). Each exclusion is a named
  refusal of the whole turn/run, never a silent skip. (Today's
  `-derivare` keeps only `^lib/.*\.c$` and drops the rest
  silently; that filter goes.)
- **interpretare: function -> file** (Claude's call). The target
  function's home is looked up in the symbol index
  (`build/nexus.tsv`, as `-derivare` does today), restricted to
  `include/` and `lib/` sedes; the derivation scope is then the
  HEADER itself, so aedilis's `corpus` annotations decide the
  implementation (calendarium_liturgicum.h -> lib/sanctorale.c), not
  a name guess. Ambiguous -> refused, candidates named. Index
  missing or the function absent from it -> today's behaviour (all
  of lib/, with its existing warning). A fresh clone therefore
  still works, just slowly, until `./silva/nexus.sh -renovare`.
- **Loaded libraries stay loaded** for the life of a session (sessio
  already keeps library modules in their own pool,
  `piscina_bibliothecarum`, apart from each generation's world -
  sessio.c:1597). A library already loaded is not loaded twice.
- **System headers** (`<stdio.h>` etc.) load nothing: machinula's
  builtins cover libc, as today.

## VI. Acceptance (the bar)

1. Golden sessio transcript: session with no plagulae; turn
   `#include "chorda.h"`; turn creating a Piscina and a chorda
   (`chorda_ex_literis("ave", p)`); a turn printing it with
   `printf("%.*s", ...)` shows `ave`, and a turn `c.mensura` shows 3
   (sessio does not print aggregates - sessio.c:2231 - and that stays
   out of scope). Same with the include arriving after other turns.
2. D2: `salutatio.scriptum` runs with an empty plagulae list, exit 0,
   both lines printed, the file written and re-read.
3. `interpretare piscina_generare_dynamicum '"x"' 1024` with no flags
   loads exactly piscina.c (the "mundus: 1 moduli" line) and returns
   BENE.
4. Refusals: `#include "fenestra.h"` is refused, naming
   `lib/fenestra_macos.m` and `include/fenestra.h`;
   `#include "silva.h"` is refused naming the amalgam and the size
   limit.
5. A gate builds `officina/colloquium.sh` and runs one batch script
   through it (exit 0).
6. Each new assertion is born red by a COMPILING plant (house rule):
   e.g. derivation skipped -> D2 traps; refusal check removed -> the
   `.m` test goes red (not merely a different error).

## VII. Left to the builder

- Where the shared function lives and its exact name/signature
  (officina/instrumenta beside sessio/praeparator seems natural).
  Header text goes to Fran for approval first, as always. Linking:
  the silva amalgam is already linked by the launchers and
  compile_probationes.sh; lib/aedilis.c and lib/aedilis_silva.c are
  NOT (builder check), and bring stml, via, filum, thesaurus,
  sigillum, internamentum, selectio, iter_directoria with them -
  add them to all three link lines.
- How to feed a REPL document to aedilis, which derives from a scope
  FILE path: the extractor is injected (`AedilisExtractor`), so a
  wrapper extractor can answer the scope path from memory (the
  session's include lines) and defer to the silva extractor for real
  files; or a temp file. Either is fine if no stray files are left.
- Whether to use aedilis's cross-run cache (`aedilis_thesauri_
  praefixum`). Measured cold: 0.1 s for filum's closure - fine
  without it.
- How colloquium.sh gets nexus_ordines (silva/instrumenta/
  nexus_ordines.c, like silva_lexicon.c beside it). Note the
  generated list `colloquium_fontes_generata.sh` covers lib/ only,
  so switching to it alone would not fix this.
- Load order of the closure (the interpreter links modules together,
  so order should not matter; verify).

## AUDIENDA (not verified)

*Answers (2026-11-02, after T4):* (1) VERIFIED - a library demitted
mid-session links cleanly; every turn rebuilds the generation's link
over all library modules ("#include medio"). (2) Held for every
closure exercised (piscina, chorda, filum, fasti: all lowered); not
swept corpus-wide. (3) Unchanged and harmless: praeparator walks the
header as before; the resolver only adds .c modules.

- That loading a library module MID-session links cleanly with
  generations already built (the module pool is separate, but the
  per-generation link was only ever exercised with libraries present
  from the start).
- That every lib/*.c in a typical closure lowers through demissio
  (fusor says 571 TUs lower; closures were not checked one by one).
- How praeparator treats `#include` of a header it already walked
  from the system/latina context.

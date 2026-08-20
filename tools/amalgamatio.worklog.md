# amalgamatio — worklog

## 2026-08-20 — the day the amalgamation machinery got exercised

Started as one item on the arbor plan (T1: a freshness gate, because
`amalgamare.sh` rebuilds from committed manifests and nothing checked
them). Finished having found that **two of three projects could not
amalgamate at all**, and had not been able to for months.

### What was broken, in the order it surfaced

**1. The excludenda generator, dead for silva.** Its amalgamator build
recipe is a duplicate of the one in `amalgamare.sh`. When
`silva_unitates.c` was extracted (fed3e78) only silva's copy grew the
new source, so the generator failed to link: `Undefined symbols:
_silva_lineam_finire`. Nobody knew, because you only run it when
adding a module or changing which vendored functions get called —
rare events.

**2. It destroyed the committed manifest when it failed.** The fixpoint
writes `excludenda_generata.h` on every iteration (the amalgamator
`#include`s it and is rebuilt each round) — and once BEFORE the loop
with EMPTY lists, since that is the fixpoint's starting point. So a
first-round failure left the file gutted: 153 entries gone, silently.
Measured the hard way — it happened to me. Now snapshots up front and
restores through a `trap`; only success keeps the new file.

**3. Manifests stale in all three projects.** `postulata_posix.h`
entered silva's closure through `silva_c89_semantica` at 7ba8d84 and
never reached any manifest. The gate caught silva on its first run and
tessera + officina the moment they were probed.

**4. tessera and officina could not amalgamate.** Same `silva_unitates`
divergence, two more copies. `AMALGAMA HORUM DUORUM PROIECTORUM
CONFICI NON POTERAT` — and it took building the gate to find out,
because building the gate meant *running* them.

**5. haruspex red, and dark.** Its exclusion for `systema_c89.h` was
written as one NAME when the reasoning covered a CLASS (lexicon files
carry synthetic types and cannot compile standalone). `systema_posix.h`
became the second lexicon; the condition never grew. Now green: 243
TUs, 7,503 shape assertions against clang.

### The root cause, stated once

**Single cases that should have been classes.** One hardcoded filename
in haruspex. One build recipe copied four times. One manifest entry
nobody re-derived. Each was fine until the second member of its class
appeared — and every failure was silent, because the tools that would
have complained were the tools nobody ran.

### Two drift modes, and they are NOT alike

Worth keeping straight, because it decides where gates are worth paying
for:

- **fontes drift (a file missing from the manifest) is SILENT.** The
  file is never read, so nothing references it, so nothing errors.
  Measured: with a planted module and a stale manifest, the amalgamator
  succeeded, the module was absent (grep = 0), and the output compiled
  clean under the full flag set.
- **excludenda drift (a pruned function gaining a caller) is LOUD.**
  The amalgam references an undefined symbol and the standalone compile
  fails. This is what bit T0: a new static helper in lib/stml.c called
  `chorda_aedificator_spectare`, pruned as dead since nothing had ever
  called it.

The exploratory pass had reported both as silent. Only measurement
separated them. The root suite's manifest turns out to be the loud kind
too (`SOURCE_FILES` feeds clang directly) — its own header says so:
*nexus fractus clamat*.

### Gate placement — a flaw in my own first design

Gate 0 was placed before any building, reasoning that failing fast
beats discarding work. But the excludenda probe LINKS an amalgamator,
which needs warm objects — so on a cold tree the gate breaks itself.
Silva hid this because its objects were already warm. tessera and
officina, whose `mech_silva_unitates.o` had never existed, exposed it.
The gate now runs after objects and before amalgamation, keeping nearly
all the fail-fast value.

### Costs, measured

- committed amalgams compile: **0.9s** (all three)
- excludenda probe: **7s**; fontes probe: a few seconds
- full regeneration, warm objects: **silva 53s, officina 16s,
  tessera 10s → 79s**
- root manifest probe: 36s
- haruspex: 121s

Which is why the tiers ended up where they did: the 0.9s artifact check
runs in the ordinary suite; the regeneration gates ride `amalgamare.sh`;
the 36s and 121s checks want a deliberate slow tier that does not exist
yet.

### Still open

- `--excludenda <path>` on the amalgamator, so the generator physically
  cannot write the committed file (today's trap is a mitigation; the
  class survives).
- 11 of 19 generated files do not declare their regeneration command —
  finish that and a `recens.sh` sweep can verify all of them by reading
  their own banners.
- Nothing checks whether a committed amalgam is stale relative to its
  SOURCES. The gates check manifests; the suite checks the artifact
  compiles. "Is the artifact current?" is still unasked, and is the
  cheap check that would tell you when the 79s is owed.
- haruspex runs nowhere.
- The amalgamator build recipe is shared for silva's three sources and
  the two-file mechanism, but tessera/officina still name their own
  `amalgamator.c` inline. One source each, so drift risk is low.

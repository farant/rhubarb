# officina_conexio worklog

## 2026-07-10 — natus + primus contactus mundi (M2a)

Model per sim-4 C1: GLOBAL symbol table (tabula_dispersa over
length-aware interned names; values = index+1 as pointer — dodges
any xar-element-stability question) + one s32 translation array per
module. Moduli never rewritten. nectere = two phases: (I) every
symbol gets a sedes — FUNCTIO → 16B descriptor in globalia
(address IS the fn-ptr value; signum INTERPRETATUM=1/DECIPULA=2,
never 0 so zeroed memory is never a valid descriptor), DATUM →
placed+copied per ordinatio, known extern cells (stdin/stdout/
stderr ansae 0/1/2 + errno 0), else DECIPULA; (II) relocations
patched ADDITIVELY via memcpy (no alignment assumption — ordinatio-1
data can hold slots).

FIRST WORLD-LINK CONTACT (the corpus-contact law, 4th data point) —
three real finds in one afternoon:
1. **Demissio stem-mangling gap**: anonymous data ($chorda_0 in
   EVERY module) + file-scope statics (_est_spatium in json.c AND
   lapifex_lexere.c) collided across modules. M1b couldn't see it —
   single-TU lowering can't collide. The sealed vocabulary already
   had the answer ($<stem>.<name>); demissio just never needed the
   file-stem half until a linker existed. Fixed at ONE chokepoint
   (_titulum_symboli — definitions AND references through the same
   path, or the link breaks silently).
2. **REAL CORPUS BUG**: include/coloratio.h (ColoratioIndex,
   StatusLinea) + include/biblia.h (LiberIndex) had bare
   `enumeratio {...} Name;` — MISSING `nomen`. Not typedefs:
   tentative definitions of anonymous-enum GLOBALS in every
   including TU. Native linking tolerates via common merging;
   honest strict linking caught it. All three names appear only in
   comments (intent obvious); `nomen` added. FILE-FILUM class find.
3. **State-corruption on duplicate**: first version returned early
   on duplicate definition leaving moduli/translationes desynced →
   downstream segfault. Now: collect ALL duplicates (census value —
   the whole collision wall in one run), state stays coherent,
   nectere refuses while numerus_fracturarum > 0.

Also re-confirmed from MEMORY.md, the hard way: STALE-OBJECT TRAP —
ran build/cursor directly after fontes edits; the fix "didn't work"
because it was never in the binary. cursor.sh relinks; use it.

Decipulae census note (M2a run): 164 named. Classes: real libc
(~60, the Wave 0/1/2 builtin list materializing), platform tier
(fenestra_*/tls_* — Tier-2 exclusion holding), biblia_* (cascade
from the one over-tectum TU), and ~70 BARE LOCAL NAMES (st, tv,
hints, stb-image internals) — the linker face of the POSIX/stb
sistere park (identifiers whose declarations live in regions
semantica couldn't type; demissio's global fallback interned them).
Named M2c classification row; harmless at link (fault-if-called).

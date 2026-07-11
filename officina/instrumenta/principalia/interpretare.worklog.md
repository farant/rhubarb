# interpretare — worklog

## 2026-07-10 — natus (instrumenta prima, tool 5 of 5)

The quick-hypothesis loop: `interpretare [-plagulae a,b,c]
<functio> [args...]` — lower the scoped lib/ TUs, generate a tiny
.medulla WRAPPER module (the inline-module pattern from
probatio_officina_machinula), link, run in machinula, print the
result by declared return type. The wrapper is TEXT (-textus dumps
it) — inspectable and reproducible, per sim-1 C3. No new machinula
surface at all.

**Bar met (known-answer smoke, all correct):**
- fasti_est_bissextilis 2024→1, 1900→0 (b32 path)
- fasti_dies_in_mense 2 2024→29, 2 1900→28 (s32 path)
- unda_sinus 0.25 1 1→1, 0.125 1 2→1.41421 (f32 args + return;
  CLI float literals get ".0" appended for the lector; args
  evaluated by PARAM type — the M2c lesson honored)
- ~2s per run with -plagulae scoping (vs the sputnik hunt's ~10min
  of scratch-file + fusor + read loop — the sim's promise held)
- halt report free on non-BENE, confirmed in the wild (instruction
  count + causa printed on the aggregate wild-write before the
  guard existed)

**Two real finds on birth day:**
1. **dilatare, not extendere** — f32→f64 widening is `dilatare`
   in the sealed vocabulary (extendere = INTEGER widening). The
   generator first emitted extendere.f32.f64; the LECTOR ACCEPTED
   IT LENIENTLY and preserved raw bits → f32 bits printed as f64
   denormal (5.26354e-315 = 0x3f800000 exactly). Fixed. NOTE for
   medulla_textus: the lector does not validate suffix pairs
   against the op family — a stricter check is a cheap future
   hardening (would have caught this at parse).
2. **Aggregate returns detected and REFUSED honestly** — the M1 C7
   convention (synthetic %reditus first param) is visible in the
   medulla signature; without the guard, calling fasti_computus
   (→Dies) wild-wrote through a garbage address. v0 refuses with
   the named deferral spelled out. Hex-dump support = the sealed
   deferral's other half, on pull.

**Composition proven during its own testing (organic, not
validation):** nexus found unda_sinus's real home (tempus.c, not
unda*), emitte showed the 3-param signature that explained a
wrong-looking 0, and interpretare's own not-found error suggests
the exact nexus command. The bench answers questions about itself.

**Rough edges (noted at the reach):**
- String-arg path proven via the $forma datum mechanism (identical
  code path); a user-facing char*-taking corpus function is rare
  in a chorda-house — untested live, note only.
- Runs in-process (no fork): a wild deref in the target segfaults
  the tool. Acceptable for a single-shot diagnosis instrument;
  cursor's fork armor exists where sweeps must survive.
- -plagulae filters lib/ only; probationes companions and fixture
  .c files unreachable (scalaria.c's pecunia_exemplum) — named,
  add probationes/ filtering if a real reach asks.

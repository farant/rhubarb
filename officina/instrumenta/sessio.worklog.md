# sessio.worklog.md

## 2026-07-11 — chunk B: the canalis-macrorum find (B7 hunt)

Symptom: `p = piscina_generare_dynamicum(...)` turn halted SISTERE
"forma localis ignota" at piscina_generare_dynamicum+0, while
`./officina/interpretare.sh -plagulae piscina ...` ran the same
function green. Hunt: latina-lexicon bisects were CONFOUNDED (Latin
turns need latina; removing it broke everything else). The
discriminating instrument was a diagnostics dump inside
_bibliothecam_demittere: 68 diagnostics, all `typus nominatus
ignotus` in piscina.h — memoriae_index unknown.

THE LESSON (three-part):
1. **The contextus lexicon is a MACRO channel only** (examen.c's own
   comment says "canalis macrorum"). Typedefs in systema_c89.h reach
   parses via systema_parsura (analysare_cum_systemate harvest) +
   oraculum preseed — NOT via the lexicon layer.
2. **A lexicon that defines an include GUARD suppresses the real
   include.** My latina-as-lexicon defined LATINA_H → piscina.c's own
   `#include "latina.h"` no-opped → latina's TYPEDEFS (memoriae_index,
   the i8..s64 family) vanished from the parse entirely. Macros
   present, typedefs gone — the worst half-alive state.
3. **The concatenation pattern (ISO+POSIX) was the tell.** Every
   working loader concatenates extra system text into ONE lexicon +
   ONE systema parse instead of adding a second lexicon. latina.h now
   concatenates after systema (order matters: size_t before
   memoriae_index) — its typedefs enter the systema parsura and the
   oracle like size_t's do. piscina.c: 68 diagnostics → 0.

Also this session: probatio cfg struct-growth trap (new
SessioConfiguratio fields uninitialized in an existing test →
garbage plagulae_numerus → segfault in creare; memset the cfg —
struct growth needs zero-init discipline at every consumer).

Interpretare's role: the known-good differential. When two pipelines
disagree on the same input, instrument the DELTA, not the symptom.

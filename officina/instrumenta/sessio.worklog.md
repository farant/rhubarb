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

## 2026-11-02 — officina-bibliothecae T3: #include turns load libraries

A directive turn now runs `bibliothecae_ex_textu` (bibliothecae.c,
aedilis closure) before anything else and demits every returned path
not yet in `oneratae` (TabulaDispersa of radix-relative paths). cfg
`plagulae` are recorded there too, so `-plagulae lib/piscina.c` +
`#include "piscina.h"` loads piscina.c once (B7 guards this: the
"oneratae ignored" plant broke B7, D2 and "clausurae communes").

**AUDIENDA §1 answered: a library demitted mid-session links fine.**
Modules live in `piscina_bibliothecarum`; every turn rebuilds the
generation (`conexio` over all library modules + the document), so
a module added by turn N is simply in the link from turn N on.
"#include medio" (x defined, then chorda.h) proves it.

Resolver is created lazily on the first directive turn (sessions
that never include never read aedilis.stml). Refusal or a failed
demission -> REIECTUM with the cause as the diagnostic (fenestra.h
names lib/fenestra_macos.m). A failure halfway leaves the earlier
paths demitted and recorded - modules never unload, and a retry
loads nothing twice.

Cost: unmeasurable at script scale - salutatio.scriptum (3 includes,
5 libs) runs in ~0.8 s with or without -plagulae.

Duplicate-load test uses `filum_existit("/")`, not a write: outside
script mode the machinula refuses writes (MACHINULA_RECUSARE_SCRIPTURAS).

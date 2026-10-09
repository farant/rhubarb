# norma_ad_c worklog

## 2026-10-08 - the emitter, bin/norma, oracle III (norma-plan-3 A5)

- **Shape of generated C (R7).** One `Norma* nodi[N]` per named norma,
  decimal literals, statements in walk order; the ONLY generated
  identifiers are `<praefixum><titulus>`, `<praefixum>gignens_<g>` and
  `_struere_<titulus>` (the identifier lint has no GENERATUM exemption).
  A reference `<ad>` is `nodi[k] = _struere_usus(piscina, facta)`; `facta`
  memoizes per public call, so a norma referenced twice is ONE node - the
  DAG survives into C (pointer equality asserted; plant b dropped the
  memo -> red).
- **Generator prototypes live in the generated HEADER** (`JsonValor*
  <praefixum>gignens_<g>(Sors*, Piscina*, vacuum*)`), so the consumer's
  definition has a prototype (-Wmissing-prototypes) and a missing one is
  a LINK error.
- **Literals.** Strings escape `" \ ?` (trigraphs) and controls (octal);
  > 509 bytes refused (C89 limit, -Woverlength-strings). s64 as
  `(s64)NL`, LONG_MIN spelled `(-9223372036854775807L - 1L)`; f64 as the
  shortest `%.*g` that round-trips, `.0` appended if it looks integral;
  non-finite refused. Lengths `(i32)NUL`.
- **bin/norma** (tools/norma.c): `c` (generator) and `iudicare` (judge a
  JSON file against a named norma; 0 valid, 1 invalid, 2 usage/load).
  Built by aedilis (`tools/norma_struere.sh`, R8); the generator action
  rebuilds it first - fabrica's judge accepted that (normae_fixae RECENS
  under `iudicare -plenus -omnia`), AUDIENDA closed.
- **Oracle III**: `probatio_norma_stml.c` `#include`s the generated
  `fixa/norma/normae_fixae.c` (aedilis did not object - AUDIENDA closed)
  and asserts loader vs generated C: same export, same judgments on
  values generated from both, same float bounds bit for bit, shared node
  shared, and the committed files == what today's emitter writes.
- Bit me: a plan buffer `b[128]` that clang's fortify check proves always
  overflows (format ~150 bytes) -> 256. A test local named `registrum`
  (latina.h `register`) -> `gignentes`. zsh does not word-split `$REGEN`
  - the first emitter plants silently skipped regeneration and were
  caught only by the freshness assertion; redone with `${=REGEN}`.
- examen (commit judge) took no declarations from the quoted `#include "fixa/norma/normae_fixae.c"` (4 'monstrator et integer mixti' at the fixa_norma_* calls); an explicit `#include "fixa/norma/normae_fixae.h"` before it -> ACCIPE.

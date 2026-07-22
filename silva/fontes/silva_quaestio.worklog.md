# silva_quaestio worklog

## 2026-07-22 — QC pseudo-classes (QA+QB -> QC)

Full pseudo-class layer landed in one pass; notes for future
editors:

- **`registrum` is a latina macro (register).** The registry
  parameter is named `registro` (ablative) everywhere — the type
  name `SilvaQuaestioPseudoRegistrum` is safe (single token), the
  bare nominative is not. silva_commissio.h:166 hit the same trap
  earlier.
- **Top-level nodes match BOTH :primus and :ultimus.** The parse
  radix is a naked LISTA (silva_parsare.c:244), so top-level nodes
  have pater == NIHIL, _listam_continentem returns NIHIL, and the
  only-child DECISUS fires. Pinned in the bar. Meaningful
  first/last queries scope under a parent (corpus > x:primus).
- **One subtree walker serves six pseudos** (:habet + the four
  semantic ones + nothing else walks): _liberos_quaerere/
  _valorem_quaerere, boolean early-exit, mirrors _nodum_visitare's
  ambiguus rule. If the visitare descent rule ever changes, change
  the walker in the same commit.
- **:habet ceiling** = `limes` param threaded through
  _catenae_congruit; limes itself is EXCLUDED (strict descendants,
  CSS relative-selector parity). :non deliberately passes NIHIL
  (absolute semantics — negation of plain congruit).
- **Registration is compile-time-before, not patch-after.** v1's
  registrare-onto-compiled-query shape is impossible under the
  unknown-name=fractura decretum; the registry must exist before
  compilare_cum_registro. Functio/datum are COPIED into the pars —
  later registry mutation does not affect compiled queries.
- Nested selector slices are NUL-terminated copies in the piscina
  fed back through _compilare_cum recursively (depth cap XVI,
  causa "pseudo nidificatio nimis profunda").

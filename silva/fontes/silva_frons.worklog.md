# silva_frons — worklog

## 2026-09-25 — born: promoted out of the shim (silva-migratio-plan T6b)

**What moved.** The production half of `materia/instrumenta/shim_c89.c`
(~1,050 lines: the token tail `SilvaFronsCauda`, conversion of registry,
tokens, values and nodes, the origin hooks, the arbor frontend hooks and
the `FRONS_C89` table) became `silva/fontes/silva_frons.{h,c}`. Ported
verbatim by script, then three deliberate changes:
- the global `SHIM` became the opaque context `SilvaFrons`, passed
  explicitly through conversion and through the hooks' `datum`
  (`uncus.datum = frons`; only `_extentum_quaerere` needs it);
- `FRONS_C89` initialises all NINE fields (`nodum_ornare = NIHIL`): the
  shim had compiled under narrowed flags precisely because this table
  missed a field — silva/fontes compiles under the house flags;
- `_radix_silvae` (defined, never called) was not promoted.
The shim is now a 372-line driver (was 1,462) and compiles under the
HOUSE flags too — its narrowed-flag remission lost its reason and was
removed.

**API** (approved by Fran, two small additions): `creare`,
`valorem_convertere`, `nodum_convertere`, `scripturam_parare`,
`arborem_parare`, `lexemata_numerus`, plus `silva_frons_causa` (the shim
reported the lexicon's refusal by name) and `silva_frons_nodus_radicis`
(the commit's root value is a VALUE; shim and writer need the node).

**Traps hit.** `registrum` is a latina.h macro (`register`) — the
context field is `tabularium`. The porting script missed calls whose `(`
ended a line; the compiler named all three.

**What guards the macro attribute.** Plant: drop the `macro` attribute
from written origins → shim `-stml` STML 383/401 (18 dispar), exit 1.
But the double round trip and the tree comparator stayed green —
writer and reader agree with each other either way. Only the TEXT
comparison against silva sees it. When the shim retires (T13), the
oracle's `stml` column (whole-file writer, pinned silva) is what
guards such attributes.

**Build.** silva/fontes now includes materia headers, so every site
that compiles `silva/fontes/*.c` (silva runner, 20 tools, silex) gained
`-I materia/fontes` and a header guard over it; `lustrum.sh` has no
header guard at all (pre-existing, noted). The amalgam EXCLUDES
`silva_frons.c` and `silva_lexicon_c89.c` (`fontes_politica.sh`
`radices()`) until the seal: `amalgamare.sh` regenerated it
byte-identical. The tools' generated dependency lists did not change.

**The lexicon** moved to `silva/fontes/silva_lexicon_c89.{h,c}`
(generator `silva/instrumenta/lexicon_c89_generare.sh`; content
identical but for comments; symbols `LEXICON_C89*` unchanged). materia's
lexicon probatio compiles it through a TEST-ONLY include list.

**Numbers.** Silva 54/54; shim `-stml` 402 + 401/401/401 (the corpus is
`silva/fontes/*.c` among others — it gained exactly these two files,
both pass); oracle clean; materia 14/14; all 20 tools build and carry
`silva_frons_creare`.

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

## 2026-09-25 — T10b: hooks over silva's own tokens; the read-side extent bug

**Second hook set, no conversion.** `silva_frons_arborem_silvae_parare`
builds a MateriaArborConsilium whose origin hook, frontend table and
token form all work on silva's OWN tail (SilvaOrigo, standard,
continuations) through the accessors. SilvaToken is a MateriaToken
(T7b) and SilvaNodus a MateriaNodus (T8), so there is nothing to
convert. The table lives inside SilvaFrons (not static) because the
hooks need a context: extents READ from a document are registered in
`frons->extenta_lecta`, so a subtree read no longer needs an expansion
(silva's old reader refused 101 nodes with "extentum sine expansione").
The old converted-tail hooks stay until T10c retires conversion.

Ported semantics, deliberately identical to silva_arbor.c: extent lookup
by identity only; `longitudo = valor` on every read token (the old reader
created each token with silva_token_ex_fonte; materia creates trivia with
no hook, so `perficere` walks the tree, trivia, origin chains and read
extents; `silva_frons_longitudines_figere` is public for T10c's laminas);
the def-site is a synthetic token built from the def-f/l/c reference.

**The bug the all-nodes shim found (fixed in the old hooks too).** When
tokens were read back, the extent landed in the DERIVED token's tail, but
the extent hook searched `frons->lexemata`, which only CONVERSION fills.
So the extent of a function-like invocation was never found on the read
side, the cursor never walked `(3)\n`, and the token after the invocation
lost `initium_lineae` (its byte offsets were off too; the structural
comparator doesn't look at them). 20 nodes in lib/arbor2_glr_tabula.c,
invisible while the shim checked only each file's FIRST node. Fix: the
reader also records the extent on the invocation's own tail, and the hook
checks the radix first. Pinned by the shim's inline case "invocatio ante
lineam" (red without the fix).

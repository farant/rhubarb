# norma_stml worklog

## 2026-10-08 - canon embedded, dialect names without a sigil (norma-plan-3 A2)

**Embedding (plan R1).** `norma.canon` (root, beside aedilis.canon) is
turned into `lib/norma_canon.c` by `tools/norma_canon_infigere.sh` (od +
awk, GENERATUM on line 1, fabrica action `norma_canon`). Not capsula:
capsula writes its header beside its TOML (outside include/, so the
aedilis include/X.h -> lib/X.c rule cannot find it) and its runtime pulls
flatura/via/iter_directoria for one ~6.5 KB file. The test asserts the
embedded bytes equal the file on disk, so a canon edit without
regeneration is red at once (plant a).

**Names: plain `nomen`, no `#`.** The spec sketched `<norma
titulus="#usus">` / `<ad norma="#usus"/>` with canon genera
identitas/referentia. Canon refused them: the decree of 2026-08-10
(lib/canon.c, CANON_GENUS_IDENTITAS case) gave `#` back to STML document
fragments and made `&name;` the sign of a WORLD individual (library-wide,
wikilink sense). A norma name is a key INSIDE one document - exactly
what `<citatio>` (xs:key/keyref) checks, verbatim, for any genus. So:
`titulus` and `norma=` are genus `nomen`; `<citatio
nomen="normae-citatae" attributum="norma" ad="norma/titulus"
super="ad"/>` catches unresolved references (plant b removed it ->
CITATIO_IRRITA assertion red).

**Canon reports one defect twice** - `<nemo/>` inside `<norma>` is both
LIBERUM_ILLICITUM (in the parent) and ELEMENTUM_IGNOTUM. Tests assert
"genus present" (`_canon_habet`), not "exactly one".

**`intra=` scoping is per NEAREST instance** - measured: key `id` in an
outer and an inner objectum = 0 vitia; `id` twice in one objectum =
NOMEN_BIS. So canon, not only the core's SCHEMA_PRAVA, names duplicate
keys and variants with a line.

**Inline capture strings in C tests** are easy to get wrong (an
implicit capture plus a stray `</>` = parse failure at 3:1); fixtures in
C strings are written explicit-close.

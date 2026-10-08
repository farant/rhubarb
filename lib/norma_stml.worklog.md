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

## 2026-10-08 - the reader (norma-plan-3 A3)

Canon judges first (vocabulary, cardinality per name, genera, citatio,
unicitas); the reader only what canon cannot say: exactly ONE type child
(canon counts per element name, not per group of names), bounds in
pairs (R3) that parse (`strtol`/`strtod`, finite only) and are not
inverted, `electio=` vs `<licitum>`, reference cycles, generators
against a registry. Every class has a fixture; a bitmask over
`NormaStmlCausa` asserts all eleven fired.

- Captures are ordinary children after parsing (measured while
  planning), so the reader never looks at `captio_numerus`.
- Named normae resolve lazily with a three-state memo (0 / building /
  built) and a stack (`catena`) so a cycle names its chain:
  "circulus: a -> b -> a". A cycle is reported once: the second root of
  the cycle is already 'built' (with NIHIL) when the top loop reaches it.
- Column = byte offset from the previous newline (`positus_initium`);
  line from the parser.
- s64 range taken as LONG_MIN..LONG_MAX because `strtol` returns long;
  equal on macOS LP64 (the only target, CLAUDE.md).
- Bit me: a parameter named `ordinarius` - latina.h `default`. Renamed
  `praestitutum`. (The plan text had it; examen would have caught it at
  commit, clang caught it first.)
- Plant notes: a plant must COMPILE under -Werror - deleting the only
  call of a static function is "unused function", not a red test. The
  cycle plant keeps `_circulus` referenced behind `si (FALSUM)`.

## 2026-10-08 - the writer; oracles I, II, V (norma-plan-3 A4)

- **Canonical form = the formatter's form, byte for byte.** The writer
  builds an STML tree and calls `stml_scribere(radix, VERUM)`; measured:
  the pretty writer chooses capture form (`<campus titulus="id" (>
  <textus/>`) on API-built trees exactly as `stml formare` does on parsed
  ones. ONE difference found: the document-level rewrite (formare,
  `_formatum`) ends at `</normae>` with NO trailing newline; the plan's
  writer appended one. Dropped - the writer matches the formatter, and
  `stml formare -probare` passes on writer output. The committed
  fixture is the writer's output (R5): `cmp` against `stml formare` of it
  = identical.
- Attribute values are written RAW by STML (`_attributa_scribere`, no
  escaping): keys/tags/forms containing `" & < >` or a newline are
  refused with "STML ferre nequit"; enum values with them go to
  `<licitum>` children (text is escaped). A value equal to `true` is
  printed bare (`titulus`) and reads back as `true` - round trip intact.
- Floats: shortest `%.*g` (1..17) that `strtod` returns exactly - `0.1`,
  `0.3`, `-1e-300` print as written and survive two cycles.
- `<variatio>` never gets `<ad>`: canon demands an `<objectum>` child
  there, so a named objectum used as a variant is written inline (DAG
  sharing lost at that one spot, meaning unchanged).
- Refusals with named reasons: key canon/STML cannot carry, generator
  function without a name, SCHEMA_PRAVA node, a named norma containing
  itself ("circulus"), an unnamed builder cycle (depth > CXXVIII).
- Oracle II: the two builder schemas of probatio_norma_gignere (copied,
  generator NAMED) survive writer -> reader with identical JSON Schema
  export and identical judgments on values generated from BOTH sides
  (TYPICA, FINES, INVALIDA; L seeds).
- Plants: no `requiritur="falsum"` -> I, II red; shared nodes inlined ->
  I + pointer equality red; reader ignoring `modus` -> II red.

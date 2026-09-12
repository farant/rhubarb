
## 2026-09-11 — the value model and the writer (plan 1, task IV)

First two thirds of `plist`: the typed value, the access surface, and
the writer. The reader is task V.

**The writer owns no escaping.** It builds an `XmlNodus` tree and calls
`xml_scribere`, so `<`, `>`, `&` and `"` are escaped by the one layer
that already knows how (`lib/xml.c:564-615`). Only the prolog and the
DOCTYPE line are prepended by hand. Writing it the other way — emitting
tags directly — would have meant a second escaper, which is exactly the
"two greens hiding a broken contract" shape.

**A dict is an ordered `Xar` of pairs.** `plist_dictio_ponere` replaces
an existing key IN PLACE and appends otherwise, so editing a bundle's
`Info.plist` never reshuffles it and two writes of one value are
byte-identical. That determinism is pinned directly (write twice,
compare).

**`integer` is a latina.h macro.** The struct field `s64 integer;`
expanded to `s64 int;` and clang said "cannot combine with previous
type-name declaration specifier" — the field is `integrum` now, which
also matches its constructor `plist_integrum_creare`. Worth remembering
for any future value model: `integer`, `nomen`, `casus`, `magnitudo`
and friends are all taken.

**C0 control characters are refused on write** (`PLIST_ERROR_SIGNUM`),
in keys as well as values. XML 1.0 admits only tab, newline and return
below 0x20, and our escape layer cannot express the rest, so emitting
one raw would produce invalid XML. Refusing beats writing a file that
`plutil` will reject later — and the read side of the same gap (numeric
character references) stays parked in `plist-spec.md` §9.

**`plist_aequalis` is public API, not test scaffolding.** `fasciculum`
needs "does this bundle's plist already say what I want?" so it can
leave a file alone rather than rewrite it.

Calibration: making `ponere` always append reddened the count/order
assertion at `probatio_plist.c:60`; blinding the C0 refusal reddened
the status assertion at `:174`. Both green after revert.

Numbers: 38 assertions, whole root suite 162 -> 163 tests. The new
library entered `compile_tests_fontes_generata.sh` (177 -> 178 C
sources) via `./tools/compile_tests_fontes_generare.sh`, which reads
`lib/`, so it had to run AFTER `lib/plist.c` existed, not before.

## 2026-09-11 — the reader (plan 1, task V)

`plist_legere` walks the same `XmlNodus` tree the writer builds, so the
two halves share one notion of what a plist is. 94 assertions now.

**Refusals name a PLACE, not a line.** `XmlNodus` carries no position
(`include/xml.h:68`), so every refusal carries a `semita` built on the
way down: `plist/dict/key[3]`. Ten named statuses, each asserted
individually — the house rule is that a refusal test proves its own
cause, never merely that something was refused.

**Two robustness cases went in BEFORE the implementation**, because a
real `Info.plist` has both and neither is in the spec's example:

- **Apple wraps `<data>` across lines**, indented with tabs. The reader
  strips whitespace before decoding (`_album_tollere`). Without this,
  every base64 payload Apple ever wrote would be refused.
- **`<data></data>` is legal and means zero bytes.** That short-circuits
  rather than asking the decoder what it thinks of empty input — the
  decoder's answer for an empty string is not a documented contract, and
  a guess there would have turned a legal plist into `PLIST_ERROR_BASE64`.

**`nomen` is `typedef`.** The helper's parameter `constans character*
nomen` expanded to `constans character* typedef`, and clang said
"invalid parameter name: 'typedef' is a keyword". CLAUDE.md warns about
this exact word and suggests the substitute I used, `titulus`. Second
instance of the class today after `integer` -> `int` in task IV, and
both were downstream-noisy: three "incompatible class assignment"
violations at `:857/:895/:927` were only the call sites seeing an
implicit `int`-returning helper. **When examen reports assignment
violations at CALL SITES, look for a broken DEFINITION first.**

Calibration: accepting multiple root children reddened `:319`;
accepting non-whitespace text inside a container reddened `:324`. Both
green after revert.

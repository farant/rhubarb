# plist — spec v1 (property lists: read, write, compare)

*Born 2026-09-11. Came out of a question about packaging a binary as a
double-clickable `.app`, which decomposed into three libraries —
**plist** → **fasciculum** (bundle layout) → **icones** (`.icns` from
an image) — plus one gap in `imago_opus`. This spec covers the first
only; the other two get their own. Fran's decisions from that
conversation are marked DECISUS; everything else is PROPOSITUM and was
agreed in conversation unless marked OPEN. Every "exists" claim cites
the header or file:line it rests on. English prose, Latin identifiers,
as in briar-spec.md.*

*Discipline note: plists appear in `Info.plist`, entitlements,
LaunchAgents, Services declarations and `defaults` files. This library
serves the first consumer (fasciculum) but is designed as the general
layer, because the second consumer is not hypothetical. It does NOT
grow features on speculation — rows in §9 graduate on PULL.*

Status vocabulary: **DECISUS** (Fran decided) · **PROPOSITUM**
(proposed, agreed in conversation) · **OPEN** (undecided) ·
**PARCATUM** (deliberately deferred, §9) · **AUDIENDUM** (not
verified; §10).

## 1. Purpose and scope

An XML property list is a small typed tree. This library reads one
into a value, lets a caller inspect and change it, writes it back, and
compares two of them. It owns the plist *grammar* (which elements are
legal where, what their text means) and nothing else: the XML layer
below it already exists, and the filesystem is one thin function.

In scope: the eight XML plist types, read and write, value comparison,
and a named refusal for everything it will not do.

Out of scope: binary plists (`bplist00`), DTD validation, plist
`version` other than `1.0`, and Apple's tab indentation. See §9.

Consumers: **fasciculum** (generating and reading `Info.plist`),
later entitlements and LaunchAgent writers. No consumer is built in
this spec — the layer stands on its own first (DECISUS).

## 2. Decisions (DECISUS, 2026-09-11)

| # | Decision |
|---|---|
| D1 | plist is its OWN library over `xml.h`, not helpers inside the bundle library. |
| D2 | It READS as well as writes: load, inspect, modify, save. |
| D3 | Purpose is local tools now; notarization is out of scope everywhere in this arc. |
| D4 | Sequencing: plist → fasciculum → icones, each with its own spec, bottom-up. |
| D5 | Missing general-purpose bits may be fixed in their HOME library rather than worked around here (§6). |

## 3. The value model

```c
nomen enumeratio {
    PLIST_DICTIO, PLIST_LISTA, PLIST_CHORDA, PLIST_VERITAS,
    PLIST_INTEGER, PLIST_REALIS, PLIST_DIES, PLIST_DATA
} PlistGenus;

nomen structura PlistValor PlistValor;
nomen structura { chorda clavis; PlistValor* valor; } PlistPar;
```

| genus | XML element | payload |
|---|---|---|
| `PLIST_DICTIO` | `<dict>` | `Xar*` of `PlistPar`, ORDER PRESERVED |
| `PLIST_LISTA` | `<array>` | `Xar*` of `PlistValor*` |
| `PLIST_CHORDA` | `<string>` | `chorda` |
| `PLIST_VERITAS` | `<true/>` `<false/>` | `b32` |
| `PLIST_INTEGER` | `<integer>` | `s64` |
| `PLIST_REALIS` | `<real>` | `f64` |
| `PLIST_DIES` | `<date>` | `DiesHora` (UTC), via `fasti` |
| `PLIST_DATA` | `<data>` | `chorda` of DECODED bytes |

Rules:

- **Arena-allocated, never freed.** Every constructor takes a
  `Piscina*`, as everywhere else in the house.
- **Strings are `chorda`** (`include/chorda.h:12`), so a value may
  carry bytes without a terminator.
- **A dict is an ordered list of pairs, not a hash.** Order is
  preserved so that read → write is stable and a diff of two
  `Info.plist` files stays legible; a hash would trade exactly that
  for a lookup nobody needs at this size. `plist_dictio_ponere`
  replaces in place when the key exists and appends otherwise, so
  editing a bundle's plist never reshuffles it.
- **`<data>` holds decoded bytes**, encoded on write. We own base64
  already: `base64_codificare` / `base64_decodificare`
  (`include/base64.h:43,62`).
- **`<date>` is typed**, not lexical, because D5 lets the missing
  conversion land in `fasti` where it belongs (§6.5). Dates are
  normalized to UTC; a plist written by us always ends in `Z`.

Surface (sketch, PROPOSITUM):

```c
/* one constructor per genus, all eight */
PlistValor* plist_dictio_creare   (Piscina*);
PlistValor* plist_lista_creare    (Piscina*);
PlistValor* plist_chordam_creare  (chorda, Piscina*);
PlistValor* plist_veritatem_creare (b32, Piscina*);
PlistValor* plist_integrum_creare (s64, Piscina*);
PlistValor* plist_realem_creare   (f64, Piscina*);
PlistValor* plist_diem_creare     (DiesHora, Piscina*);
PlistValor* plist_data_creare     (chorda octeti, Piscina*);

b32          plist_dictio_ponere (PlistValor* d, chorda clavis,
                                  PlistValor* valor, Piscina*);
b32          plist_lista_addere  (PlistValor* lista, PlistValor* valor,
                                  Piscina*);
PlistValor*  plist_dictio_capere (PlistValor* d, constans character* clavis);
i32          plist_numerus       (PlistValor* d_aut_lista);
PlistValor*  plist_ad_indicem    (PlistValor* lista, i32 index);
b32          plist_aequalis      (PlistValor* a, PlistValor* b);
```

Two notes on that surface, so neither reads as an accident:

- **`ponere` takes a `chorda` key, `capere` takes C literals.** Writing
  a key usually means passing data through (`chorda` from a parse or a
  build); reading one is almost always a literal at the call site
  (`plist_dictio_capere(d, "CFBundleIdentifier")`). A `_chorda` twin of
  `capere` graduates on PULL, not before.
- **`plist_numerus` counts pairs for a dict and elements for a list**,
  and refuses (returns 0) for a scalar genus.

## 4. Reading

```c
nomen structura {
    PlistStatus  status;
    PlistValor*  radix;    /* NIHIL nisi PLIST_SUCCESSUS */
    chorda       semita;   /* "plist/dict/key[3]" */
    chorda       causa;
} PlistResultus;

PlistResultus plist_legere (chorda textus, Piscina*,
                            InternamentumChorda*);
PlistResultus plist_legere_plagulam (constans character* via, Piscina*,
                                     InternamentumChorda*);
```

Shape mirrors `xml_legere` (`include/xml.h:97`), including the
interner the caller owns.

**Errors name an element path, not a line.** `XmlNodus`
(`include/xml.h:68`) carries genus, titulus, valor, attributa, liberi
and parens — no position. So a refusal reads
`plist/dict/key[3]: clavis sine valore`. This is the honest limit of
the substrate and one more reason for the materia-based xml
(desideratum 01M299JPS0): positions and trivia would come with it.

| status | meaning |
|---|---|
| `PLIST_SUCCESSUS` | tree built |
| `PLIST_ERROR_XML` | the XML layer refused; carries its `XmlStatus` |
| `PLIST_ERROR_NON_PLIST` | root element is not `<plist>` |
| `PLIST_ERROR_VERSIO` | `version=` present and not `1.0` |
| `PLIST_ERROR_BINARIUM` | `bplist00` magic — a different format (§9) |
| `PLIST_ERROR_STRUCTURA` | key without value, value without key, unknown element (named) |
| `PLIST_ERROR_BASE64` | `<data>` is not valid base64 |
| `PLIST_ERROR_NUMERUS` | integer overflow, real malformed, trailing junk |
| `PLIST_ERROR_DIES` | `<date>` is not an ISO-8601 UTC stamp |
| `PLIST_ERROR_SIGNUM` | a `<string>` to be written holds a C0 control character (§6.7) |
| `PLIST_ERROR_MEMORIA` | allocation |

Policies:

- **Whitespace-only text between elements is ignored** (plists are
  indented); non-whitespace text inside a container is
  `PLIST_ERROR_STRUCTURA`, never silently dropped.
- **One guard at the chorda→C-string boundary.** Numbers and dates
  reach terminator-based parsing, and a `chorda` may legally contain
  NUL; the refusal lives at that boundary once, not at each call site
  (the recurring chorda-NUL hazard — §6.3 fixes its root).
- **Exactly one child of `<plist>`.** Zero children, or several, is
  `PLIST_ERROR_STRUCTURA`: the format carries one root value.
- **Named entities need no work; numeric ones do.** `lib/xml.c:898-950`
  decodes `&lt; &gt; &amp; &quot; &apos;`, and
  `probationes/probatio_xml.c:414` pins a round trip through four of
  them. But `:951-957` copies any UNRECOGNIZED entity through
  literally, so `&#65;` survives as five bytes — see §6.7.

## 5. Writing

```c
chorda plist_scribere (PlistValor* radix, Piscina*);            /* purum */
b32    plist_scribere_plagulam (PlistValor*, constans character* via,
                                Piscina*);
```

- **Pure core, thin I/O twin** — the `imago_png_codificare` /
  `imago_png_scribere` split, so the whole writer unit-tests with no
  filesystem.
- **Emits** the XML declaration, Apple's DOCTYPE line, `<plist
  version="1.0">`, then the tree through `xml_scribere`
  (`include/xml.h:266`), which escapes `<`, `>`, `&` in text and `"`
  in attributes (`lib/xml.c:564-615`).
- **Deterministic**: dict order is preserved, so one value always
  writes one byte sequence. Pinned by writing twice in the suite.
- **Indentation is ours, not Apple's.** Apple indents with tabs;
  whitespace is insignificant and `plutil` accepts either, so we take
  `xml_scribere`'s and document the deviation rather than add an
  indentation policy to `xml.h`.
- **`<data>` emits one base64 line.** Apple wraps at a fixed width;
  both are valid.
- **`plist_aequalis` is API, not just test scaffolding** —
  fasciculum needs "does this bundle's plist already say what I want?"
  so it can leave files alone.

## 6. Home-library fixes (D5)

Each is a gap in a library plist sits on, each lands there with its
own test, and each is useful beyond plist.

### 6.1 `xml.h` — skip `<!DOCTYPE …>`

`lib/xml.c:1172` and `:1459` skip an XML declaration (`<?xml …?>`) but
there is no DOCTYPE branch, so a real `Info.plist` — which always
carries one — would fail with `XML_ERROR_SYNTAXIS`. Fix: skip a
doctype declaration before the root element, and **refuse an internal
subset (`[…]`) by name** rather than mis-skipping to the first `>`.

### 6.2 `chorda.h` — an exact `f64` form

`chorda_ex_f64` (`lib/chorda.c:1688-1728`) builds `"%.*f"` into a
132-byte buffer. That cannot round-trip a double: `1e300` needs three
hundred digits, `1e-20` vanishes at any sane precision. Add
`chorda_ex_f64_exacta` (`%.17g`) for `<real>` and anyone else who
needs a number to survive a round trip.

### 6.3 `chorda.h` — refuse embedded NUL, and refuse overflow

`chorda_ut_f64` (`lib/chorda.c:1613-1647`) copies `mensura` bytes,
appends a terminator, then accepts when `*terminus == '\0'` — which is
**true at an embedded NUL**. So `1\0garbage` parses as `1`.
`chorda_ut_s32` (`:998-1032`) has the identical shape, so this is a
FAMILY defect, verified on both, and the fix belongs to the family
rather than to plist.

The same reading turned up a second one: `chorda_ut_s32` takes
`strtol`'s `signatus longus` and **casts it to `s32` with no range
check**, so `"99999999999"` returns VERUM carrying a truncated value.
`<integer>` needs that refused, so the family fix covers overflow too
(`ERANGE` plus the target type's own bounds), and `chorda_ut_s64`
(§6.4) is written with it from the start.

### 6.4 `chorda.h` — the `s64` pair

`include/chorda.h:241-268` offers `chorda_ut_s32`, `ut_i32`, `ut_f64`,
`chorda_ex_s32`, `chorda_ex_f64` — no `s64` in either direction. Plist
`<integer>` is signed 64-bit, and s32 would truncate silently. Add
`chorda_ut_s64` / `chorda_ex_s64`. The header already deals in 64-bit
values (`chorda_ex_bytes_legibilis` takes an `i64`, `:352`).

### 6.5 `fasti.h` — the ISO-8601 datetime pair

`fasti_parsare_iso` (`include/fasti.h:417`) takes `YYYY-MM-DD` only;
plist `<date>` is a full stamp (`2026-09-11T02:41:32Z`). Add
`fasti_ad_iso(DiesHora, Piscina*) → chorda` and
`fasti_ex_iso(chorda, DiesHora*) → b32`, named to match the existing
`fasti_ad_unix` / `fasti_ex_unix` (`:341,346`) and `fasti_ad_iulianum`
/ `fasti_ex_iuliano` (`:326,331`) pairs. Offsets (`+02:00`) are
converted to UTC on read; writing always emits `Z`.

### 6.6 Not needed after all

Escaping on write is present for `<`, `>`, `&` and `"`
(`lib/xml.c:564-615`); named-entity decoding on read is present (§4).
Base64 is present (§3).

### 6.7 Control characters and numeric references

`lib/xml.c:951-957` copies an unrecognized entity through verbatim, so
`&#65;` and `&#x41;` are not decoded. Apple's writers use numeric
references for control characters, so a plist carrying one reads back
wrong — silently, since nothing refuses it. The two halves are decided
differently:

- **Reading** numeric references belongs in `xml.h` and is PARCATUM
  until a real plist needs it. Pull: a G5 fixture containing one.
- **Writing**: plist REFUSES a `<string>` holding a C0 control
  character (`PLIST_ERROR_SIGNUM`), because our escape layer cannot
  express one and emitting it raw produces invalid XML. A refusal now
  beats a corrupt file later; the refusal relaxes the day `xml.h` can
  escape them.

## 7. Probationes and gates

`probationes/probatio_plist.c`, wired by
`./tools/compile_tests_fontes_generare.sh`. Home fixes are tested in
the files that already exist: `probatio_chorda.c`, `probatio_xml.c`,
`probatio_fasti.c`. **Every gate is born red by a planted fault**, and
a refusal test asserts its SPECIFIC status and path — not merely that
something was refused.

| # | Gate |
|---|---|
| G1 | Value model: dict order preserved; `ponere` replaces in place, never duplicates a key. |
| G2 | Reader: one case per genus, nested containers; each refusal in §4's table by status AND path. |
| G3 | Writer: golden bytes for a canonical plist; written twice, identical (determinism); escaping; `<data>`; reals `1e300`, `1e-20`, `0.1` read back exactly. |
| G4 | Round trip ours: value → write → read → `plist_aequalis`. |
| G5 | Round trip theirs: an Apple-written `Info.plist` → read → write → compare. Fixture = the 636-byte `demo_85.dSYM/Contents/Info.plist`, copied into `probationes/fixa/plist/` so the test does not reach into `knotapel/`. It carries a real DOCTYPE — the reason §6.1 exists. |
| G6 | Foreign oracle: `probationes/probatio_plist_plutil.sh`, modeled on `probatio_qr_gyrus.sh`. CONTROL FIRST (lint Apple's fixture, proving `/usr/bin/plutil` behaves), then ours, then `plutil -convert xml1` on both and diff. |
| G7 | NUL and overflow guards: `chorda_ut_f64("1\0garbage")` refuses, `chorda_ut_s32("99999999999")` refuses rather than truncating; plist refuses an `<integer>` carrying an embedded NUL, and a `<string>` carrying a C0 control character on write. |

**Oracle calibration, stated before it bites:** `plutil` accepts a
plist with no DOCTYPE and ignores indentation, so its silence vouches
for neither. G3 asserts the prolog and DOCTYPE lines directly. (The
imago lesson: ask what an oracle forgives before treating its silence
as proof.)

## 8. Phase plan (test-first; each phase ends green)

| Phase | Work | Why here |
|---|---|---|
| P1 | `chorda`: `s64` pair, `ex_f64_exacta`, NUL + overflow refusal across `ut_*` (§6.2-6.4) + tests | plist's own tests depend on these; each useful alone |
| P2 | `xml`: DOCTYPE skip + internal-subset refusal (§6.1) + tests | G5 cannot run without it |
| P3 | `fasti`: `ad_iso` / `ex_iso` (§6.5) + tests | `<date>` types on it |
| P4 | plist value model + pure writer + G1, G3 | writing is the half with no unknowns |
| P5 | plist reader + refusals + G2, G4, G5, G7 | reading is where the fixtures bite |
| P6 | G6, the `plutil` gate with its control | last, because it judges finished output |
| P7 | `lib/plist.worklog.md`, dated note in `lib/chorda.worklog.md`, as-built notes here | before the commit, per house habit |

## 9. Named deferrals (PARCATUM)

| Row | Pull |
|---|---|
| Binary plists (`bplist00`) | A consumer that must read one. It is a separate format and a library-sized job; refused by name today. |
| `version` other than `1.0` | Apple has not shipped one. |
| DTD validation | Nothing needs it; the DOCTYPE is emitted and skipped, never checked. |
| Apple's tab indentation | A diff against Apple-written files that someone actually minds. |
| Dates outside UTC | A consumer needing to preserve an original offset byte-for-byte. |
| plist over materia | Lands when xml becomes a materia client (desideratum 01M299JPS0) — the API is designed so the xml seam stays narrow. |

## 10. Risks, notes, AUDIENDA

*Revised and closed items: see §11, as built 2026-09-11.*

- **VERIFIED 2026-09-11** (was AUDIENDUM): numeric character
  references are NOT decoded — `lib/xml.c:951-957` copies any
  unrecognized entity through verbatim. Became §6.7.
- **VERIFIED 2026-09-11** (was AUDIENDUM): `chorda_ut_s32`
  (`lib/chorda.c:998-1032`) shares `ut_f64`'s NUL flaw AND casts
  `longus` to `s32` unchecked. Became §6.3.
- **AUDIENDUM:** `chorda_ut_i32` was still not read; the same shape is
  expected, and §6.3's family fix assumes it.
- **AUDIENDUM:** whether `xml_legere` keeps whitespace-only text nodes
  at all. §4's policy assumes it does and ignores them; if it drops
  them, one rule in the reader becomes dead code.
- **AUDIENDUM:** `plutil -convert xml1 -o -` behaviour was not run;
  G6's diff step may need a normalization pass on both sides.
- **AUDIENDUM:** whether `xml.h` handles CDATA and processing
  instructions inside the tree. Plists contain neither, so this only
  matters for a plist someone hand-edited oddly.
- **Risk, low:** `XmlNodus` has no position, so every plist error
  message is a path. If that proves too coarse in practice, the answer
  is the materia rebase, not a hand-rolled position in plist.
- **Risk, low:** the round-trip gates prove value identity, not byte
  identity. Byte identity against Apple's own writer is explicitly not
  a goal (indentation, base64 wrapping).

## Cross-references

- `include/xml.h`, `lib/xml.c` — the layer below; desideratum
  01M299JPS0 (materia client).
- `include/chorda.h`, `include/base64.h`, `include/fasti.h` — the
  libraries §6 touches.
- `probationes/probatio_qr_gyrus.sh` — the foreign-oracle gate
  pattern G6 copies.
- Next specs in this arc: `fasciculum` (bundle layout: value → pure
  render → thin writer → reader), `icones` (`.icns` and `.iconset`,
  with an area/box resampler landing in `imago_opus`).


## 11. As built (2026-09-11, plan 1 tasks I–VII)

Everything in §1–§9 was implemented. This section records where the
built thing differs from the design, and closes what §10 left open.
Commits: `698f7475` (chorda), `351ab229` (xml), `64bd6dc9` (fasti),
`fcd52773` (plist value + writer), `a92b9ef7` (reader), `67a385bc`
(oracle).

### Amendments to the design

1. **`plist_scribere` takes a `PlistStatus*`** (§5 showed it without
   one). §6.7's control-character refusal needs to name a cause, and an
   empty `chorda` cannot. Signature: `chorda plist_scribere(PlistValor*,
   PlistStatus*, Piscina*)`; `plist_scribere_plagulam` likewise.
2. **The xml status is `XML_ERROR_PARS_INTERNA`, not
   `XML_ERROR_SUBSETUM_INTERNUM`** (§6.1 guessed the latter). The Latin
   identifier lint refused the first commit over the coined word
   `subsetum`; with three sites the lint's own rule says rename, and
   *pars interna* says what the XML spec calls the internal subset in
   real Latin. Value VIII, appended.
3. **The value field is `integrum`, not `integer`** — `integer` is a
   `latina.h` macro, so `s64 integer;` expanded to `s64 int;`. Same
   class bit twice: a helper parameter named `nomen` expanded to
   `typedef`. Both are named in `lib/plist.worklog.md`.
4. **Two reader contracts the design did not state**, both required by
   real Apple files and both now pinned: `<data>` text is stripped of
   whitespace before base64 decoding (Apple wraps it across
   tab-indented lines), and `<data></data>` means zero bytes without
   consulting the decoder about empty input.
5. **G6 runs after the test loop, not in the runner's preflight.** The
   gate executes `build/probationes/probatio_plist`, so registering it
   beside the `qr_gyrus` gate inside `run_speculum()` made it run before
   its own prerequisite existed — it exited 2 and aborted the suite.
   `qr_gyrus` survives there because its binary is built by other
   means. Exit 2 in the new position is a named skip, not a failure.

### §10 items now settled

- **`chorda_ut_i32` — CLOSED.** It shares `ut_f64`'s NUL flaw *and*
  accepted `-1` as 4294967295 via `strtoul`. Both fixed in task I, with
  the whole `chorda_ut_*` family guarded by one helper.
- **Whitespace-only text nodes — CLOSED.** `xml_legere` does keep them;
  the reader skips them and refuses non-whitespace text inside a
  container. Proven by a planted fault (`probatio_plist.c:324`).
- **`plutil -convert xml1` — CLOSED.** Used by G6 to compare canonical
  forms; behaves as assumed.
- **A third `chorda` defect found while implementing task I**, not
  predicted here: `chorda_ex_f64` used `snprintf`'s would-have-written
  length as a `memcpy` length against a 132-byte buffer, so
  `chorda_ex_f64(1e300, 6, …)` read ~175 bytes past it. Guarded.
- **CDATA and processing instructions — STILL AUDIENDUM.** Plists
  contain neither; untested.
- **Byte identity against Apple — CONFIRMED IMPOSSIBLE, as designed.**
  Apple's dSYM files carry the older `-//Apple Computer//DTD PLIST
  1.0//EN` public identifier while we emit `-//Apple//…`. G5 compares
  values and G6 compares `plutil` canonical forms, which is why neither
  is affected.

### Numbers

`probatio_plist` 111 assertions; `chorda` 271, `xml` 95, `fasti` 179.
Whole root suite 162 → 163 tests, all green. Every gate was seen red by
a planted fault before being trusted.

# psalmodia_tabula worklog

## 2026-11-10 — R2: reading, canonical dump, totality

Plan: project-specs/psalmodia-plan-1.md R2. Header approved in R1.

**Measured.** The table is 53 794 bytes, 1 160 lines; one full read
(STML parse + walk) takes under 1 ms (`clock()` prints 0 ms in
`_tabulam_veram_probare`). No performance concern.

**Design decisions.**
- The reader collects every error instead of stopping at the first
  (contrast `instrumentarium_legere` in carmen.c): a slip list should
  be complete. Errors are `linea N: causa`, in walk order = file order.
- Arrays are sized by counting an element's children before filling
  (`_liberos_numerare`), not grown with xar as the plan sketched; only
  the error list grows (xar, copied into the piscina at the end).
  Simpler, and exact sizes.
- A `<tonus>` missing modus/clavis/tenor, or with an unknown modus,
  is reported and SKIPPED (its children are not walked: no clef, no
  meaning). Same for an `<exemplum>` missing pars/textus/notae or with
  an unknown pars. Clausulae and loci are never skipped; an unknown
  element inside them is reported and the rest read.
- Leaves (initium, flexa, praeparatio, accentus, nota) take their
  trimmed text; an element nested in a leaf is reported (otherwise
  `stml_textus_valor` would silently swallow its text).
- Booleans accept exactly `verum`/`falsum`; `casus` only
  `monosyllaba`; `positio` only `altera`; anything else is a vitium
  naming the allowed values.

**STML facts relied on.**
- `stml_legere` sets `elementum_radix` only on success: a file STML
  cannot parse (truncated, unclosed, empty) gives ONE vitium at STML's
  error line and no tones. STML is all-or-nothing; the "good tones
  survive" promise holds for semantic errors, not syntax errors.
- Attribute values are raw (entities decoded only in text) and
  interned in the piscina passed in — so the source buffer can be
  freed after reading, as the header promises.
- An attribute value may span lines (`notae="f  gh h\n   g ..."`):
  STML keeps the newline inside the value; tokens split on any ASCII
  whitespace.

**Plants (silva.planta, radix gate, filtrum psalmodia), both RED then
restored byte-identical (backup in build/plantae_psalmodia/):**
- (a) unknown-attribute check silenced → the `tonica` assertion in
  `_totalitas_probare` red. That assertion was ADDED for this plant:
  the plan's fixtures never exercised an unknown attribute.
- (b) tokens keep one trailing byte → `_tabulam_veram_probare` red
  (tone 1 intonation "f").

**Lint lesson.** `booleum`/`booleus`/`booleanum` are all unknown to
the lexicon (the house's `boolean` hits are tolerated English in
json/plist). Use `veritatis`/`veritatem`: `_veritatem_legere`,
`_campus_veritatis`. `silva/renominare.sh` refuses untracked files
(git is its undo), so renaming inside a brand-new file is a
whole-word replace.

## 2026-11-11 — R3: checks (gabc notes, ending letters, clashes)

**Measured (plan step 1).** `gabc_legere` on `"(c4) a(gvFED)"` →
success, 4 notes, last gradus -6 (D); `"(c4) a(hixih)"` → last -2
(A); `"(f3) a(e)"` → 0 (C). gradus is NEGATIVE below do (under c4
h = -2, g = -3; under c3 g = -1 = si), so the letter needs a true
modulo (`_littera` adds 7 until non-negative; C89 `%` on negatives
is implementation-defined). Every headerless string carries one
MONITUM (`separator '%%' deest`) — `gabc_successus` ignores monita.

**gabc success is not enough.** Three strings gabc accepts cleanly
would have slipped through the plan's check:
- `a()` — success, zero notes;
- `a(h)x)` — success, the `)` just opens a third syllable;
- `a(c3h)` — success, `c3` is read as a clef change mid-notes.
So a note string counts as readable only if: no gabc VITIUM, exactly
two syllables (clef + notes), at least one note, and the note
syllable holds only NEUMA / ALTERATIO / SPATIUM elements. A clef is
checked alone (`"(" clavis ")"`: one syllable, one CLAVIS element);
a bad clef is ONE vitium and the strings under it are not checked
(else `clavis="c5"` would bury its own line under dozens).
Also learned: `h_q` is legal gabc (episema + quadratum) — `q` alone
is the error ("signum notae sine nota").

**Design.**
- Each check builds its string in a scratch piscina (`Lector.probatio`,
  16 KB dynamic) that is emptied before every `gabc_legere` and
  destroyed at the end; the table's piscina only gains the vitia.
  Consequence: a returned `GabcCantus*` dies at the next check, so
  `_clausulam_probare` copies the last gradus out before checking
  the superveniens.
- Per-tone checks run at the end of `_tonum_legere` (the initium and
  flexa lines are only known there — the structs keep no line for
  them); the cross-tone clash pass runs after all tones; then the
  vitia are stably sorted by line, so the slip list reads like a
  compiler's.
- Example tokens are checked under the tone's clef even when they
  name a terminatio with its own clef: only validity is asked, and
  positions do not depend on the clef.
- Added beyond the plan (in the header's PROBATIONES): the same
  mediatio (monosyllaba, ad_libitum) twice in one tone — otherwise
  R4's `psalmodia_mediationem_eligere` would silently take the first.

**Real table: 0 vitia** — the transcription has no slips the checks
can see. To prove the checks fire on the real file, a scratch copy
with D2→E2, g2→g, `gh(`, `h@@` gave exactly the expected six lines
(incl. two knock-on "exemplum differentiam ... non habet").

**Plants (silva.planta, radix gate), both RED, restored
byte-identical (backup build/plantae_psalmodia/*.r3.ante, log r3.log):**
- (a) letter check under c4 always → red; planta names the first
  fracture (real table, nine false letter vitia in c3/f3 tones); a
  hand re-run listed all: real table, forma canonica (tone 2 f3 D on
  f), and both f3 fixtures in `_lapsus_probare`.
- (b) clash pass per tone, not per modus → the 4 / 4-altera fixture
  red.

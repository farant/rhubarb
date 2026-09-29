# runae — phase log

*The text stack's Unicode core, first brick: width + graphemes. Plan:
`project-specs/unicode-width-graphemes-plan.md` (D1–D8 approved by Fran
2026-09-28). Narrative of building here (INTENTIO before a task,
FACTUM/RELATIO after); find-time notes in `lib/runae.worklog.md`.*

## U1 — CORPUS (2026-09-28)

### INTENTIO

Test DATA before any table. `tools/runae_corpus.c` parses Lapide's
`01_Preliminares{,_<lingua>}.html` with the HOUSE html parser
(`html_arbor_parsare`, materia) and walks the tree: every `<p>` in
document order; its text and entity nodes decoded exactly as
`html_coctum` does (`entitates_html_decoquere`), descendants included
(`<b>`, `<a>`); `<br>` → newline; runs of ASCII whitespace collapsed to
one space (U+3000 and other non-ASCII spaces kept); paragraphs trimmed
and separated by a blank line. Whole paragraphs until the sample
reaches 4 KiB. One file per language:
`probationes/fixa/runae/corpus/<lingua>.txt`, 35 languages (Lapide's
unsuffixed files are English, `_lt` is Latin → `la.txt`). The tool
builds against `html/build/` objects like `html/arbor.sh`.

Gate: `probationes/probatio_runae_corpus.c` (red first: files absent):
for each of the 35 languages the file exists, is ≥ 4 KiB, is valid
UTF-8 end to end (`utf8_decodere` never −1), and is in the right
SCRIPT: at least 100 codepoints in that language's main block
(Devanagari for hi, Thai for th, Hangul for ko, Hebrew for he, …;
Latin letters for the Latin-script languages). The script check is
what catches an extractor that mixes files up or loses the text. A
plant: a sample file swapped between two languages → red by name.

**U1 FACTUM (corpus).**
- `tools/runae_corpus.c` + `.sh` extract 35 samples (4,108–6,169 bytes)
  through the house HTML parser.
- An independent Python `html.parser` extraction matched all 35 byte
  for byte.
- `probatio_runae_corpus.c` was red first (35 files absent by name),
  then green.
- Plant: hi and th samples swapped → both named (0 runes in their
  script), restored.
- Provenance in `probationes/fixa/runae/corpus/PROVENIENTIA.md`.

## U2 — UCD + GENERATOR + WIDTH (2026-09-28)

### INTENTIO

**The width rule, read from Ghostty's source, not paraphrased.** Ghostty
1.3.2-dev (vendored in opentui's `zig-deps.tar.gz`) takes its width from
uucode: `src/build/uucode_config.zig` `WidthComponent` over uucode's
`components.zig` `Wcwidth`. As one rule over UCD properties:

1. standalone width: 0 for gc Cc, Cs, Zl, Zp; 1 for U+00AD; 0 for
   Default_Ignorable_Code_Point; 2 for U+2E3A; 3 for U+2E3B (clamped to
   2 below); 2 for East_Asian_Width W or F; 2 for
   Grapheme_Cluster_Break Regional_Indicator; otherwise 1. (U+20E3 is
   standalone 2, but it's Me, so rule 3 zeroes it.)
2. zero-in-grapheme: standalone 0, or Emoji_Modifier, or gc Mn or Me, or
   GCB V, T or Prepend.
3. width = 0 if zero-in-grapheme AND NOT Emoji_Modifier AND GCB is not
   Prepend; else min(2, standalone).

Consequences worth stating: Mn/Me are 0; **Mc (spacing marks, e.g.
Devanagari ि U+093F) are 1**; Hangul medial/final jamo are 0; skin-tone
modifiers are 2 (they show in isolation); Prepend characters keep width
1. My research note and plan D5 paraphrased this loosely ("combining
marks Mn/Mc/Me = 0", "emoji presentation = 2"): wrong on Mc, and emoji
presentation isn't a separate rule (those characters are EAW W). The
CODE is the reference; the corrections go into the research note.

Invalid runes (negative, > U+10FFFF) → 1 (they render as U+FFFD, and
Ghostty returns 1 past the max). Surrogates → 0 (Cs).

Inputs (fetched once 2026-09-28, SHA-256 in PROVENIENTIA):
EastAsianWidth, DerivedGeneralCategory, DerivedCoreProperties (DI, and
InCB for U4), GraphemeBreakProperty, emoji-data; GraphemeBreakTest is for
U4. Each file's `@missing` line sets the default before its data lines.

Build: `include/runae.h` (`runae_latitudo`, `RUNAE_VERSIO`),
`lib/runae.c`, the generated `lib/runae_tabulae.c` + header (a 2-stage
table: 4,352 block indices + deduplicated 256-byte blocks; the value byte
keeps bits 0–1 for width, and the rest is left for U4's break classes).
The generator is `tools/runae_generare.c` with a `-probare` wrapper.

Red first: header + a stub `runae_latitudo` that returns 1 (today's
assumption), then `probatio_runae.c`: about 40 hand cases derived from
the rule (not from the table), the ledger's two measured cases (`広` = 2,
U+0301 = 0), a totality sweep over 0..U+10FFFF plus invalid values, and
every corpus rune getting a width. Then the generator and table turn it
green. Plants: a rule dropped in the generator → named cases red; a
hand-edited table → `-probare` red.

**U2 FACTUM (UCD + generator + width).**
- The six Unicode 15.1.0 files and the licence were fetched once and
  pinned (SHA-256 in `probationes/fixa/unicode/15.1.0/PROVENIENTIA.md`).
- `tools/runae_generare.c` + `.sh` (`-probare`, whitespace-insensitive)
  → `lib/runae_tabulae.c` (104 unique blocks, ~35 KB); `include/runae.h`
  (`runae_latitudo`, `RUNAE_VERSIO`), `include/runae_tabulae.h`,
  `lib/runae.c` (two array loads).
- Red first: stub = 1 everywhere; 41 of 57 hand cases failed by name (the other 16 expect 1).
- Green: all 57 cases, totality over 0..U+10FFFF (8,254 / 923,318 /
  182,540 at widths 0/1/2, none out of range), and corpus expectations
  per script.
- Plants: Mn/Me not zeroed → the six mark cases; the DI rule removed →
  the ZWSP family and the Hangul filler; a hand-edited table →
  `-probare` stale. The generata gate gained section VI.
- `compile_tests_fontes_generata.sh` regenerated (+runae.c,
  +runae_tabulae.c).
- The research note and plan D5 were corrected against Ghostty's code.

## U3 — WIDTH ORACLE (2026-09-28)

### INTENTIO

An independent derivation of the same table: `tools/runae_oraculum.c`
opens ICU4C 74.2 (Homebrew, Unicode 15.1) with `dlopen` and looks up
`u_charType`, `u_hasBinaryProperty`, `u_getIntPropertyValue`,
`u_getUnicodeVersion` by STRING name (no ICU headers, which are C99/C++
and fail the house flags; no English identifiers in the source; enum
values hand-copied from `unicode/uchar.h` 74.2 with the source name in
a comment). It applies the SAME Ghostty rule to ICU's properties
instead of our UCD files, and refuses to run unless ICU's Unicode
version equals `RUNAE_VERSIO`.

Three modes:
- default: every codepoint vs `runae_latitudo`; zero differences
  expected (a difference = a parser bug in our generator, a data
  difference, or a misread enum).
- `-aurum`: writes ICU's answer as contiguous ranges to
  `probationes/fixa/runae/aurum_icu.txt` (committed, like toml's
  tomllib aurum), and `probatio_runae.c` gains a PERMANENT check that
  the table equals the aurum on every codepoint, with no ICU needed at
  test time.
- `-opentui`: OpenTUI's `unicode-width-map.zon` (3,909 entries) against
  our widths; every difference is classified (Mc, controls, DI, RI, …)
  and recorded as a named POLICY row (their policy, not a standard).
  Dev-time only, since opentui isn't in the tree.

Red first for the permanent check: the aurum gate is written against an
empty aurum file (it must fail: no coverage). Plants: a misread enum
(W ↔ Na) in the oracle → differences named; a hand-edited aurum line →
the gate names the range.

**U3 FACTUM (oracle).**
- `tools/runae_oraculum.c` + `.sh` (ICU4C 74.2 via dlopen by name):
  0 differences over 1,114,112 codepoints.
- The aurum (`probationes/fixa/runae/aurum_icu.txt`, 951 ranges) is now a
  permanent check in `probatio_runae.c`: red first (empty aurum:
  coverage fails), then green.
- Plants: a misread EAW enum in the oracle → 182,519 differences; an
  aurum range's width edited → named per codepoint with its range; an
  aurum line deleted → "non contiguum" named.
- OpenTUI: 29 differences = 15 version (Unicode 17 made them W) + 14
  policy (their hand-widened emoji-capable symbols), none a bug.
- examen lexicon gained `dlfcn.h` (auspex-certified, planted);
  `examen-corpus` holds.

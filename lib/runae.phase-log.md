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

## U4 — GRAPHEME BREAKS + CLUSTER WIDTH (2026-09-28)

### INTENTIO

**Data (15.1, measured before designing):** every
Extended_Pictographic codepoint is GCB Other (3,537); InCB Consonant ⊂
GCB Other (240); InCB Extend ⊂ GCB Extend ∪ {ZWJ} (883 + 1); InCB Linker
⊂ GCB Extend (6); ZWJ is InCB Extend; the 5 emoji modifiers are GCB
Extend with InCB None; the 371 emoji-variation bases are GCB Other. So
ONE class enum carries the whole break alphabet (19 values: Other, CR,
LF, Control, 4 kinds of Extend (plain, InCB Extend, InCB Linker, emoji
modifier), ZWJ, RI, Prepend, SpacingMark, the 5 Hangul syllable
classes, Extended_Pictographic, InCB Consonant), 5 bits. The value byte
= width (bits 0–1) + class (bits 2–6) + emoji-variation base (bit 7).
The generator ASSERTS each containment above, so a future Unicode that
breaks one fails loudly. zero-in-grapheme is derived, not stored: width
0, or Prepend, or emoji modifier (exactly Ghostty's definition, U2). One
more pinned file: `emoji-variation-sequences.txt` (same D3 fetch).
`@missing` becomes per property: a default must precede ITS OWN
property's data (the InCB default sits mid-file, after DI's data).

**API** (`include/runae.h`, names from modules/001, unsealed):
`RunaeRuptura` (caller-held state: odd-RI parity, ExtPict Extend*,
…ZWJ, InCB consonant, …linker), `runae_rupturam_initiare`,
`runae_rumpitur(prior, runa, &ruptura)` = pure UAX #29 15.1 (GB3–GB13
incl. GB9c; an invalid rune breaks on both sides, like Control), and
`runae_graphema_proximum(initium, finis, &latitudo)`, which segments by
`runae_rumpitur` and applies Ghostty's width effects (`graphemeWidth`,
read from `../ghostty/src/unicode/grapheme.zig`): start at the first
rune's width; VS16/VS15 set 2/1 only after a valid variation base (else
no effect); any following rune that isn't zero-in-grapheme makes it 2.
One deliberate difference: Ghostty restores the break state after an
invalid selector (its cells don't store it); runae's segmentation stays
pure UAX #29 (edge: RI + invalid VS + RI).

**Oracles:** (1) `GraphemeBreakTest.txt` 15.1, every line, every
boundary, in the new `probatio_runae_graphemata.c`; (2) Ghostty's own
cluster-width test cases, ported; (3) ICU's character break iterator
(`ubrk_*` + `utext_openUTF8` via dlopen) over all 35 corpus samples,
every boundary byte offset compared, and the result committed as
`probationes/fixa/runae/aurum_graphemata.txt` (per language: cluster
count + an FNV-1a hash of the boundary offsets), checked permanently.

Red first: API stubs (every rune its own cluster, width = the rune's).
Plants: GB9c removed, GB11 removed, the RI parity inverted, the VS base
check dropped.

**U4 FACTUM (graphemes).**
- `runae_rumpitur` (UAX #29 15.1, GB3–GB13 incl. GB9c; caller-held
  `RunaeRuptura`) and `runae_graphema_proximum` (Ghostty cluster width).
- Table byte = width + a 5-bit break class (19 classes) + the
  variation-base bit; 120 blocks. Containments asserted in the generator;
  `@missing` guarded per property.
- A seventh pinned file: emoji-variation-sequences.txt.
- Red first: stubs → 768 of 1,187 conformance lines, the Ghostty width
  cases and the empty aurum failed.
- Green:
  - GraphemeBreakTest 1,187/1,187.
  - 20 Ghostty cluster-width cases.
  - ICU `ubrk` (via dlopen, `utext_openUTF8` byte offsets) = runae on
    every boundary of all 35 samples (0 differences), committed as
    `aurum_graphemata.txt` (count + FNV-1a per language) and checked
    permanently.
- Plants caught by name: GB9c (7 lines + bn/gu/hi/ml), GB11 (4 lines),
  RI parity (6 lines), VS base (3 width cases).
- The scripted edits of this task went through pythonica's Editio.

## U5 — tessera wide cells (2026-09-28)

Narrative in `tessera/phase-log.md` ("WIDE CELLS (runae U5)").

## U5b — tessera grapheme cells (2026-09-28)

Narrative in `tessera/phase-log.md` ("GRAPHEME CELLS (runae U5b)"); D7 = (a)
per-opus cluster table (Fran). Commit 5ed13286.

## U5c — width policy from the environment (2026-09-28)

Narrative in `tessera/phase-log.md` ("WIDTH POLICY FROM THE ENVIRONMENT").
`RunaePolitica` / `runae_graphema_ex_politica` live here: SIMPLEX = the
ZWJ rule only (the spacing-mark rule was refuted by Fran's second
Terminal.app look). Commit e4b08570.

**State before compaction (2026-09-28):** U1–U5c done on rhubarb-secunda
(9 commits ahead of main, none merged). NEXT U6: saltuarius column math
and `lib/excerptum`'s caret move from rune counts to `runae` widths (the
ledger's measured cases 広 / e+U+0301 become tests), then U7 (the Lapide
viewer), U8 (RELATIO, ledger …VRTHANR → impletum, merge to main).

## U6a — the diagnostics caret measures columns (2026-09-28)

**INTENTIO (Fran approved the U6 split: U6a excerptum, U6b saltuarius +
`tessera_graphema_ponere`).** The caret in `lib/excerptum` counted UTF-8
characters; the ledger's measured cases (…VRTHANR) become tests.

**Red first:** 7 new cases (two measured cases on the single-location
path, `a広b` tildes, a wide first cluster, an empty span on a wide
cluster, a start inside a cluster, and the measured `a { 広: red } }`
through `excerptum_scribere_multa` — the path the product calls).
6 red; the empty span was already right (kept as a regression guard).

**Green:** `_signum_scribere` walks clusters via
`runae_graphema_proximum` (default policy: diagnostics have no terminal
handle). 79/79 after a control-byte case was added (see plants).
Details and rules: `lib/excerptum.worklog.md` (2026-09-28).

**Plants (compiling), caught by name:** spaces per cluster not width;
tildes per cluster; start-inside-cluster ignored; control width 0 —
this last one was GREEN at first (no case), so `a\x01b` was added.

**Blast radius:** excerptum is in the silva amalgam and in toml, css,
silva, materia and crusta probationes lists and `tools/diagnostica.sh`
(the pre-commit hook's own tool); 22 silva launcher snippets
regenerated. The amalgam leaked `RUNAE_GRADUS_*` as global data, which
silva's nm gate cannot see (it compares only against silva's own six
dependency objects) → exact renames `SILVA_RUNAE_GRADUS_*`; quaestio
…8RWX1 filed for the gate. officina's amalgam does not carry excerptum
(its manifest "staleness" was only missing objects in this worktree).

## U6b — text-level measurement in runae (2026-09-29)

**INTENTIO (Fran, 2026-09-29).** Fran asked whether U6's saltuarius work
was reusable. The model-side math (a span's width; column → the start of
the unit covering it) has four consumers — excerptum, saltuarius, the
U7 viewer, tessera — so it moved into runae instead of
`saltuarius_liber.c`. U6 re-split: U6b runae, U6c
`tessera_graphema_ponere`, U6d saltuarius. Names sealed:
`runae_latitudo_textus`, `runae_columnam_quaerere`. Decision (a): the
text-level functions count what the house DRAWS.

**The unit rule (UNITAS PINGENDA, runae.h):** a C0 or DEL byte = one
byte, 1 column (every house painter draws a substitute: tessera `?`,
excerptum/saltuarius ` `); an invalid byte = one byte, 1 column
(tessera draws `?` per byte — `utf8_decodere` alone would swallow a
whole overlong/surrogate sequence as one); otherwise a grapheme under the
given policy. C1 stays 0 (tessera draws nothing for it).
`runae_latitudo` for a single rune is unchanged (Cc = 0, Unicode's
truth). A zero-width unit covers no column, so `columnam_quaerere` skips
it; a column inside a wide unit snaps to its start; past the end →
`finis` and the total width.

**Red first:** stubs; 14 width cases and 12 column cases (`a広b` both
halves, a zero-width skip, the control rule, the family under both
policies) red by name.

**Green:** 238/238 in probatio_runae_graphemata, including a corpus
property on all 35 languages × both policies: an INDEPENDENT walker in
the test restates the unit rule; every 16th visible unit, queried at its
LAST column, must map back to its own start and start column, and the
total width must equal the walked sum.

**Plants (compiling), caught by name:** control rule off (4 cases + the
corpus, via newlines); invalid-per-byte off (the overlong case); wide
snap off (3 cases + the corpus); policy ignored (the SIMPLEX family). The
policy plant first failed to COMPILE (unused parameter) and was redone.

**Amalgams:** the new functions are unused in both, so they are trimmed:
tessera's exclusions regenerated as-is; silva needed `runae` added to its
trimmed bases in `fontes_politica.sh` (the exclusions generator refused
"nomen inclassificabile" until then). The manifest must be regenerated
BEFORE and AFTER the exclusions (fontes → excludenda → fontes →
amalgamare). Both amalgam `.c` files are byte-identical to before;
VERIFICATUM + idempotent.

## U6c — tessera places one unit (2026-09-29)

Narrative in `tessera/phase-log.md` ("PLACE ONE UNIT"):
`tessera_graphema_ponere`, `_octetos_scribere` rebuilt on it, and a
cross-check of tessera's unit rule against `runae_latitudo_textus` over
the whole corpus.

## U6d — saltuarius measures and paints by columns (2026-09-29)

Narrative in `saltuarius/phase-log.md` ("WIDE AND COMBINED
CHARACTERS"). The model's cursor is a screen column (runae's text-level
functions); painting goes one unit at a time through
`tessera_graphema_ponere`. Fran's look in Ghostty: right; one Ghostty
artefact (pre-base vowel sign under a block cursor) recorded as the
terminal's. The ledger item …VRTHANR is now satisfied in all three named
consumers (excerptum, tessera, saltuarius); it closes in U8.

## U7a — the unit walk becomes public (2026-09-29)

**INTENTIO (Fran approved).** folium's wrapper must walk text one unit at
a time (end + width). The rule lived privately in `_unitas_pingenda`;
without it public, folium would be the THIRD re-implementation (tessera
keeps its copy on purpose — the corpus cross-check guards it; saltuarius
pre-measured with `runae_graphema_ex_politica` + its own control check,
which measures an overlong sequence as 1 column where tessera draws 3
`?`). So: `runae_unitas_proxima(initium, finis, politica, &latitudo)`,
same shape as `runae_graphema_ex_politica`; `runae_latitudo_textus` and
`runae_columnam_quaerere` are rebuilt on it (no behaviour change);
saltuarius's pre-measure uses it (the malformed-input case fixed).

**Red first** (stub): 9 unit cases + the corpus property, which now
compares every step of runae's walk with the test's independent walker
(35 languages × 2 policies). **Green** 258/258. **Plants (compiling),
caught by name:** control rule off (2 cases + 76 corpus steps),
invalid-per-byte off (the overlong case), policy ignored (the SIMPLEX
family; the first attempt did not apply — the formatter had wrapped the
line — and was redone). Amalgams: the function is trimmed as unused in
both; `.c` byte-identical, VERIFICATUM + idempotent.

## U7b — folium, the Lapide corpus viewer (2026-09-29)

**INTENTIO (Fran approved).** A tessera instrumentum next to spectaculum.
Name: `lector` was proposed, then changed to **`folium`** (a leaf of a
book) because tessera already has `TesseraLector` (the input reader).
Layout, keeping tessera's pin (grid + input forever; layout is a future
separate library):
- `tessera/instrumenta/folium/folium_pagina.{h,c}` — the PURE part:
  `folium_involvere` (wrap to a width → `FoliumLinea {initium, finis,
  latitudo}` + statistics) and `folium_paginam_pingere` (a page into a
  TesseraOpus; controls → space). Linked by tessera's test runner, not by
  the library or the amalgam. `aedilis.stml` gained the root.
- `tessera/instrumenta/principalia/folium.c` + `tessera/folium.sh`
  (generated source list): loads the 35 corpus samples (or given
  files), pages (space/j/k/PgUp/PgDn, g/G), languages (]/[ and arrows),
  re-wraps on resize, status line with language, page, units, columns,
  wide and zero-width counts, policy.

**Wrapping, deliberately naive:** paragraphs are the file's lines (\r
trimmed); break after a space, or between units either of which is
wide BY NATURE (first rune width 2: CJK, emoji); otherwise hard-break
before the unit that would overflow (Thai, long words); a unit wider
than the line sits alone; blank paragraphs kept.

**The golden caught a real bug on first look.** The first rule was
"either unit is wide" by CLUSTER width — but under Ghostty's rule a
spacing mark (Mc) makes `लि` 2 wide, so Hindi words broke in the middle
("कोर्नेलि|यूस"). Fixed to the first rune's own width; a case pins it.

**Tests (probatio_tessera_folium):** 9 small cases (space, CJK, Thai,
over-wide unit, blank paragraph, CRLF, space-then-wide, Mc cluster, break
after wide) + zero width refused; INVARIANTS over 35 languages × 2
policies × widths 40 and 17, computed independently from runae
(lossless reassembly, true width ≤ limit unless a lone unit, unit
boundaries, every break legal, statistics = sums): 140 wraps, 0
failures; drawing: every row read back == its line (10/10 for each
golden); a tab draws as a space; goldens `probationes/fixa/folium/
{ja,hi,yo}.40x10.txt` (page 1, written with FOLIUM_AURUM_SCRIBERE=1 and
checked by eye). Red first (stubs); plants (compiling), all caught by
name: no space break, cluster-width "wide" (case + corpus legality +
hi golden), no break after wide, no hard break (Thai + corpus width),
CR kept, controls not spaced. Two plants first failed to apply / tested
the wrong thing and were redone.

**Terminal look (Fran, 2026-09-29): all 35 languages looked good; no
problems seen.** Findings table — what is ours and what is the
terminal's:

| Area | Seen | Whose | Note |
|---|---|---|---|
| CJK (ja, zh, ko) | right | — | wide cells + containment |
| Indic (hi, bn, gu, ml, ta) in Ghostty | right | — | Mc = 2 agrees with Ghostty |
| Indic in Terminal.app | narrower conjuncts (U5c look) | terminal | renders rows as shaped text |
| Pre-base vowel sign under a block cursor | vanishes (U6d look, saltuarius) | terminal | Ghostty recolours glyphs by anchor cell |
| RTL (ar, fa, he) | not flagged | ours, by design | no bidi: words are drawn in memory order; the bidi brick is not built |
| Thai line breaks | not flagged | ours, by design | naive hard break mid-word; real breaking needs a dictionary |
| Japanese line starts (・、。) | not flagged | ours, by design | no kinsoku rules |
| ZWJ families in Terminal.app | three emoji | terminal (modelled) | SIMPLEX policy (U5c) |

The "not flagged" rows are properties of the naive wrapper that a
reader who does not read those scripts would not catch by eye; they are
listed so the table does not claim more than was checked.

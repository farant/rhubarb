# runae — worklog

## 2026-09-28: U1, the Lapide corpus

- Lapide's language suffixes: the unsuffixed page is ENGLISH and `_lt` is
  LATIN (by the pages' own `hreflang`). The plan's first draft had both
  wrong; fixed there.
- The house HTML parser (materia's html client) extracted all 35
  languages, and an independent Python stdlib extraction under the same
  normalization matched byte for byte. The walk is simple because
  Lapide's pages are clean: no content out of place (`sedes`), no
  adoption agency. A messier page would need html_coctum's ordering
  logic, so don't reuse the walk on arbitrary HTML.
- Useful for later tasks: `ar` is fully vowel-marked (dense combining
  marks, width 0), `yo` stacks combining tone marks on dotted letters,
  `ja` uses U+3000 ideographic spaces (kept, not collapsed), `th` has no
  spaces between words.
- The tool builds against `html/build/*.o` (like `html/arbor.sh`); those
  objects have no `filum`/`utf8`, so it uses stdio directly.
- The script gate proves the script, not the language: Latin-script
  languages (21 of them) and ar/fa can't be told apart by it.
- examen: `<br>` inside a C comment is read as an annotation tag; I wrote
  "(br)" instead.

## 2026-09-28: U2, the width table

- The width rule is Ghostty's, read from its code (the vendored tarball
  in opentui: `ghostty/src/build/uucode_config.zig` + `uucode-ghostty/
  src/components.zig` `Wcwidth`), not from my research note, which had
  it slightly wrong (Mc isn't 0; emoji presentation isn't its own
  rule). Next time, read the code before stating a rule.
- `DerivedCoreProperties.txt` has a SECOND `@missing` line mid-file (for
  InCB, line 12607), after DI's data. A blanket "@missing must come before
  data" guard rejected the file; the guard now fails only when the late
  default touches flags we actually read. U4 reads InCB and must apply
  that default BEFORE InCB's data lines, i.e. per property, not per
  file.
- EastAsianWidth 15.1 lists the default-W unassigned ranges (plane 2/3,
  CJK blocks) explicitly; its only `@missing` is `N` for everything.
- The table: 104 unique 256-byte blocks (26,624 bytes) + 4,352 u16 block
  indices = ~35 KB of data; the generated source is 76 KB.
- `-probare` ignores whitespace: the commit-time formator may reflow the
  committed generated file (the same reason silva's lexicon check does;
  the tessera amalgam was bitten by exactly this in 1.2 T5). Plant:
  whitespace-only change → still fresh; a value change → stale.
- `tools/generata_probare.sh` section VI now runs `-probare`. In the
  secunda worktree the gate as a whole still reports the silva/officina
  manifests stale (a worktree-only false alarm), so I ran section VI by
  hand for the commit.
- Corpus measurements: ar has 928 zero-width runes out of 2,343 (full
  vowel marking); hi 310, th 328, yo 164; ja/zh have almost no
  width-1 runes, ko 489 (its spaces and punctuation).
- The Latin lint blocked the first commit on 6 new words. `graphema`
  (a house term like `lexema`, heavily used from U4 on) and `ignorabilis`
  (classical) went into `oratio/glossarium.stml`. `gc` became
  `V_CATEGORIA_NULLA`, `iamo` became `V_MEDIAE_FINALES` (Hangul medial and
  final letters), and `RUNAE_UNICODUM_VERSIO` became `RUNAE_VERSIO`.

## 2026-09-28: U3, the ICU oracle

- ICU is opened with `dlopen` and its functions looked up by STRING name
  (`u_charType_74` …). That keeps ICU's C99/C++ headers and English
  identifiers out of house code. The function-pointer cast goes through
  `memcpy` (ISO C forbids converting an object pointer to a function
  pointer; -pedantic would reject it). Enum values are hand-copied from
  `unicode/uchar.h` 74.2 with the ICU name in a comment; a misread enum
  is exactly what the W↔Na plant simulated (182,519 differences, named).
- examen didn't know `dlfcn.h` (the first house use; only vendored
  sqlite had it). I healed the lexicon the house way: a `dlfcn.h` section
  in `silva/fontes/systema_posix.h`, and RTLD_* values certified in
  `officina/auspex_posix.sh` (a plant with RTLD_NOW 0x3 → "TU nostrum
  DISSENTIT"). `examen-corpus` still holds.
- Result: 0 differences over all 1,114,112 codepoints between the
  UCD-generated table and ICU's properties under the same rule. It's
  committed as `probationes/fixa/runae/aurum_icu.txt` (951 ranges, 13
  KB), and `probatio_runae.c` checks every codepoint against it
  permanently, with no ICU needed. The aurum also guards coverage
  (contiguous ranges up to U+10FFFF).
- OpenTUI's map (3,907 entries) differs on 29 codepoints, all "we say 1,
  they say 2":
  - 15 are VERSION differences: OpenTUI's data is Unicode 17.0, where the
    I Ching trigrams U+2630–2637 and mono/digrams U+268A–268F became W,
    and U+1F6D8 (LANDSLIDE) is new and W. They agree after a version
    bump.
  - 14 are POLICY: OpenTUI hand-widens text-default emoji-capable
    symbols (‼ ⁉ ☢ ☣ ⚛ ⛑, heart ornaments U+2760–2767), which are N/A
    even in 17.0 (`utf8.zig` `eawToWidth`, the hand ranges after the W/F
    test). Ghostty and runae keep them 1; VS16 widens them per cluster
    (U4).
  - Zero differences on marks, controls or default-ignorables: OpenTUI's
    "Mc = 0" never shows up because its map has no Mc entries.
- Python's unicodedata (Unicode 15.0) was not run: ICU matched exactly,
  and a 15.0 source would only add version noise.
- (U3, late) The Latin lint also refused `opentui` and `regionalis`
  (identifiers); renamed `_mappam_alienam_conferre`, `RUPTURA_REGIONIS`.

## 2026-09-28: U4, grapheme breaks + cluster width

- **Editio (pythonica) is the edit tool** (Fran reminded me mid-task).
  My raw Python string replaces broke on the commit-time formator's
  realignment. Editio's `replace` is whitespace-tolerant, all-or-nothing,
  and judges with examen. Two Editio lessons:
  - An anchor inside a comment needs `tolerans='verba'` (a comment is ONE
    token to it). It refused cleanly and wrote nothing.
  - `inserere_ante(<first function>)` inserts at the top of the FILE:
    the first definition's extent includes the file's leading comment
    and `#include` lines. Anchor on a later line instead (I used
    `replace` on the `#define` after the includes).
- Slip: I ran `scribe F < F` once (only meant to judge). It survived
  (`<` reads the whole input before scribe writes; the old disaster was a
  pipe), but to judge an already-edited file, run `silva/examen.sh F`.
- Measured before designing: in 15.1 every containment the one-byte
  layout needs holds (ExtPict ⊂ Other, InCB Consonant ⊂ Other, InCB
  Extend ⊂ Extend ∪ ZWJ, Linker ⊂ Extend, ZWJ is InCB Extend, modifiers
  are Extend with InCB None, variation bases ⊂ Other). The generator
  asserts all of them per codepoint.
- Break state is updated from PRIOR on each call (prior is consumed
  exactly once per pair). That's why Ghostty's "restore the state after
  an ignored VS" can't simply be copied: runae's segmentation stays pure
  UAX #29, and only the WIDTH follows Ghostty (`ultima` = Ghostty's
  `prev`).
- Results: GraphemeBreakTest 15.1, all 1,187 lines, zero failures on the
  first run; Ghostty's 20 cluster-width cases; ICU's break iterator over
  all 35 samples, zero boundary differences in ~114k clusters.
- GB9c's corpus footprint: removing it breaks exactly bn, gu, hi, ml,
  the scripts whose virama is InCB=Linker in 15.1. Tamil's virama is not
  a Linker, so ta is untouched. The corpus is sharp enough to see that.
- The table grew from 104 to 120 unique blocks (~40 KB); widths still
  match ICU on every codepoint (the aurum_icu check).

## 2026-09-28: U5c, width policy

- SIMPLEX was derived from three Terminal.app observations (हि = 1 cell,
  the ZWJ family unjoined, ❤️ = 2), not from documentation. It is a
  HYPOTHESIS about Terminal.app until the look confirms it; flags and
  skin tones are its first untested predictions (both 2).
- The red run showed the policy is minimal: of seven SIMPLEX cases, only
  the two that differ from Ghostty failed before the implementation.
- Editio refused, rightly, a deletion that changed the code's shape;
  `tolerans='spatia'` is the documented verbatim escape for a deliberate
  removal. It also reported `sana: False` when my stub put a statement
  before declarations (C89 violation), so its verdict is worth reading
  every time.
- tessera doesn't auto-detect: a `getenv` inside `tessera_aperire` would
  make the test suite depend on the terminal it runs in. Apps opt in
  with `tessera_politicam_ponere(opus, tessera_politica_ambitus())`.
- CORRECTION after the second Terminal.app look: its हि is 2 cells (like
  Ghostty); my "Mc = 0" reading of the U5b screenshot was wrong. The
  shift there came from the conjunct न्दी. Squeezing हि to 1 made the ि
  disappear (our next cluster was drawn over it), which is how the
  mistake became visible. SIMPLEX keeps only the ZWJ rule. Lesson: one
  screenshot row with two clusters can't say WHICH cluster disagreed;
  isolate the clusters (one per row) when measuring a terminal.

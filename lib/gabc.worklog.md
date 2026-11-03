# gabc.c worklog

## 2026-11-03 — T2, reader core

Plan: `project-specs/gabc-plan.md` T2; spec `project-specs/gabc-spec.md`.

**Shape.** Hand-written scanner over the bytes, three stages:
(1) header lines up to the first line that is exactly `%%` (found by a
pre-scan; none = whole input is body + monitum `GABC_SINE_CAPITE`);
(2) body: text up to `(`, notes up to `)`, one syllable per pair, the
syllable's elements collected in one scratch `Xar` that is copied out
and emptied per syllable; (3) `gradus` resolved in a separate pass, walking
elements in source order with the clef in force. Line/column come from
a table of line starts (binary search), so diagnostics never need a
running line counter.

**Grammar = Gregorio's lexer** (`gabc-notes-determination.l`, copy in the
session scratchpad `gregorio_docs/notes.l`). Things that are easy to get
wrong:
- an accidental is the pitch letter FOLLOWED by the sign: `ix` = flat
  sign at position i (the letter is not a note). Same for custos `g+`.
- `[cf]b?[1-5]` is a clef even mid-group; `cb` without a digit is two
  notes. Clef letters are lower case only.
- tabs/CR/LF inside parentheses are ignored and do NOT split a neume;
  a space does.
- `!` alone (before a letter) is a zero-width break; `!/...` and `! `
  are unbreakable spaces. `//[` is `/` then `/[n]`.
- `[...]` content can escape `]` with `$`.
- `'0` `'1` `.0` `.1` `o0` `o1` `r0`-`r8` `_0`-`_5`* carry digits; an
  upper-case pitch takes a direction digit 0-2.

**Deliberate T2 gap (G5 debt).** The scanner consumes every construct
Gregorio knows, so the Salve reads with zero diagnostics, but T2 only
RECORDS notes, neumes, clefs, bars and accidentals. Modifiers (T3) and
spaces, breaks, custos, brackets, `{` `}`, nabc (T4) are consumed and
dropped for now. The functions `_modificatoris_longitudo` and
`_spatii_longitudo` and the `[`/`z`/`|` branches of `_notas_legere`
are where T3/T4 start recording.

**Choices (not from Gregorio):**
- notes before any clef: c3 (old `initial-key` default). No diagnostic
  yet — T4 may want one (no genus exists for it).
- `verbum_incipit`: text present AND (first text OR any whitespace since
  the previous text-bearing syllable). Textless syllables are FALSE but
  do not swallow the whitespace, so `ve(gh) (hg)re(g)` makes `re` a word
  start, while `ve(gh)(hg)re(g)` does not.
- bar adjacent to a note in the same group (before or after, no space)
  → monitum. `(g.,)` also warns; fine for a monitum.
- `%` in header lines = comment to end of line (so `name: x; % note`
  still ends at `;`). In the body, `%` is a comment only between
  syllables; inside syllable text it is literal for now.
- unclosed `(` → VITIUM, notes read to end of input.
- `|` toggles nabc mode (models `nabc-lines: 1`); T4 revisits.

**Measured, not computed:** Salve = 141 syllables, 145 notes (shell
`grep`/`tr` over the body). The reader agrees.

**Plant:** clef honoured only in syllable 0 → `_claves_probare`
FRACTUM; restored, green.

**Debug aid:** a ~20-line driver that prints `gabc_tabulam_scribere`
for a file links against `build/gabc.o piscina.o chorda.o
chorda_aedificator.o xar.o filum.o lectiones.o` (session scratchpad
`gabc_tabula.c`). Useful until `auditio -gabc` exists (T8).

## 2026-11-03 — T3, shapes, rhythm signs, accidentals

**Modifiers** apply to the last note of the OPEN neume (`neuma >= 0`);
the last note globally is that note, so no extra index is kept. A
modifier after a space/clef/bar is a VITIUM, not silently attached to
some earlier note.

**Shorthand expansion** (`gvv`, `gsss`, `GVV`) is done by the modifier
itself: a `v`/`V`/`s` on a note that already carries that shape clones
the note (same positio, directio, inclinatum bit) into the open neume.
This also accepts `gv.v`, which Gregorio's lexer would not treat as a
bivirga — harmless, and simpler than a lookahead pattern.

**Accidental scope** lives in the resolution pass, not the lexer: a
15-slot table (one per positio, sentinel `ALTERATIO_NULLA` = no sign),
cleared at a syllable with `verbum_incipit`, a bar, and a clef. The
clef's b-flat (`cb3`) is a fallback used only when the slot is empty,
so a natural `y` on si overrides it within scope. Ficta notes
(`r6`-`r8`) keep the alteratio the lexer gave them (`perge` inside the
switch continues the note loop — correct, the switch is not a loop).

**Measured:** Salve `'` 39, `.` 14, `_` 8, `w` 2 (shell) = reader.

**Plants:** bar not clearing scope → alteration test red; `..` as one
mora → rhythm test red.

## 2026-11-03 — T4, typesetting content, opaque marks, malformed input, totality

**Grammar sources, fetched this session** (scratchpad `gregorio_score/`):
`gabc-score-determination.l` and `.y` (syllable text), `gregoriotex-symbols.tex`
(the `<sp>` table, `\gresetspecial` lines 218-245). Facts that are easy to
get wrong:
- In syllable text `$` escapes any next byte; `%` starts a comment (to end of
  line, newlines eaten) even mid-text; a newline plus following whitespace is
  ONE space; `~` is the lyric tie (rendered U+203F, Gregorio's default
  `\greundertie`).
- `<v>` and `<alt>` are raw until their closing tag, which may lie past a `(`
  (`<v>\foo(x)</v>` is one verbatim run). `<sp>` content cannot contain `<`
  `{` `}`.
- Styles `i b ul c sc tt` STAY OPEN across syllables (Gregorio's `styles`
  bitmask, re-inserted at each syllable); `e` (elision) is not carried. We
  close `e` at the syllable end with a vitium; a carried style still open at
  EOF is a vitium located at its opening tag.
- An unknown `<tag>` is literal text in Gregorio; we keep it as text and add
  a monitum only when it has tag shape (`<` `/`? letters `>`).
- Notes: a lone `]` ends an `@[` autofusion group (T2 flagged it as an
  error); `{` `}` are TeX boxes; `<nlba>` may appear inside notes; `$)` does
  not close a notes group (score lexer `(\$.|[^|\)])+`).
- `!/0` and `!/!` lex as `/0` and `/!` (the `!` is absorbed); `!/` `!//`
  `! ` `!/[n]` are the non-breaking variants. The approved header has no NB
  flag, so the non-breaking forms are stored as two elements, `nullum` then
  the space — mirrors the source tokens, nothing lost. Flagged to Fran.
- Pitch ceiling = Gregorio `highest_pitch`; in our positions that is
  `2*lines+4` (m on 4 lines, p on 5) → monitum `GABC_POSITIO_EXTRA` for
  notes, custodes, accidentals. `claves_mutatae.gabc` gained
  `staff-lines:5;` because its `n` is legitimately above a 4-line staff.

**`<sp>` → UTF-8 choices**: R/ ℟, V/ ℣, 'ae ǽ, 'oe œ́ (œ + U+0301), ae æ, oe œ,
`*` ✶ (StarSix glyph), `+` † (the glyph is "Dagger", not the Maltese cross),
ut ‿, ot ⁀, dt ⁐ (U+2050 is the over-plus-under tie shape), A/ Ⱥ (no barred-A
in Unicode; the stroke variant is the nearest). Unknown codes: raw text +
monitum.

**Text runs.** One run per (genus, stilus); a run's chorda points into the
source when its bytes are contiguous there, otherwise it is copied into the
piscina (sp output, `$` escapes, newline→space, braces removed). Translation
runs are closed at `]` so `[a][b]` stays two runs; `[/]` is its own
TRANSLATIO run "/". `vocalis_initium/finis` count bytes of the joined
cantatus text. `verbum_incipit` now means "some cantatus byte" (markup-only
or translation-only syllables are textless).

**`<pr:N>` value** has no field in the approved header: the PROTRUSIO bit is
set on the following text and the value gets a monitum ("valor protrusionis
non servatur") — not silent. Expect ~0 occurrences in GregoBase; revisit if
T9 finds any.

**Quadratic guard**: an unclosed `<v>`/`<alt>` searches to EOF; a cache
("no closing tag at or after offset X") makes repeated failures O(1), so a
file of many unclosed `<v>` stays linear.

**Dump**: new lines `textus` (only for "ornate" syllables: >1 run, non-cantatus
genus, or any stilus), `vocalis`, `signa`; old expected dumps changed only by
the new `spatium` lines.

**Measured (shell)**: Salve `[ll:1]` 3, `[oh:h]` 6, `!` 2, `/` 1, `//` 1, `{ia}` 1
(the syllable's whole text is `{ia}`) = reader (9 marks, 4 spaces, 1 vocalis).

**Totality**: 68,806 cases (every prefix + 2,000 seeded single-byte mutations
per fixture, half from a gabc-significant alphabet), each in an exact-size
malloc so ASan sees overreads. Born GREEN on the T3 reader (it was already
total), so its plant is a contract break: stray `]` in notes sets `deficit`
→ NIHIL → fracti > 0. Whole suite under `tools/sonda.sh` (ASan+UBSan): clean.

**Plants**: `[oh:` dropped → `_signa_opaca_probare` red; length cut at first
NUL → `_vitia_formae_probare` red; stray `]` → NIHIL → `_totalitatem_probare`
red. All restored byte-identical (cmp).

**Bug caught by the formae dump**: the new `casus '<'` (for `<nlba>` in notes)
shadowed `<` as the augmented-ascending modifier; a non-nlba `<` now falls to
`_modificatorem_legere`.

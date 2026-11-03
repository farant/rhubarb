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

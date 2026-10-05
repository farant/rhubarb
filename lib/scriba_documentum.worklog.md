# scriba_documentum worklog

## 2026-10-05 — S0: the text document on historia

The second `historia` client: a fixed sheet (`TabulaCharacterum`)
whose truth is a volumen of `mutatio` acts. Projection memory = cells
(W×H) then the s32 sticky indentations at a 4-aligned offset, one
block from `piscina_allocare_ordinatum` (plain `piscina_allocare`
aligns to 1); the TabulaCharacterum's two pointers point into it.

**An act is an effect, computed generically.** Trim trailing BLANK
lines from both sheets, take the common prefix and suffix, record
`<mutatio linea=p deletae=d>` + the new lines; applying splices the
trimmed lines and pads with blank lines to the height. Exact for ANY
pair of sheets by construction - so the plan's open question "what does
vim do on a full sheet" stopped mattering: whatever vim does, the
before/after diff records it. (Read anyway: `tabula_inserere_lineam`
refuses only when the last line has NON-whitespace content; a last line
of whitespace-but-not-blank is pushed off and lost - vector V.)

**What "blank" is - the header was wrong.** `tabula_characterum.h`
says `tabula_initiare` sets cells to '\0'; the code fills SPACES ("so
the cursor can roam") and indentation -1, and insert/delete line write
the same. So blank = all ' ' + indentation -1. '\0' still appears
(`tabula_trahere_sinistram` writes it into the last column; vim.c:877)
and is a different byte, so it is content: the codec keeps it (`\0`).
My first test built sheets with `tabula_ex_literis_cum_dimensionibus`,
which pads with spaces AND sets indentation 0 - every line was then
"content" and every diff ran to the bottom. The test now builds from
`tabula_initiare` + raw cells. (Not fixing the header comment here:
tabula_characterum.h is widely included and a comment edit owes the
whole gate closure; worth doing alongside a real change to it.)

**Line codec in an ATTRIBUTE, not text.** STML keeps attribute values
raw both ways; text has trivia rules, and leading/trailing spaces are
content here. Escapes: `\\ \0 \t \1 \q \xHH` (other bytes < 0x20 or
>= 0x7F); trailing spaces dropped (the reader pads with spaces).
`<`, `&`, `>` pass raw in the attribute and round-trip (vector VII).

**Validation before mutation.** `scriba_mutatio_applicare` decodes all
new lines into scratch first; a bad act (range past the content, too
wide, bad escape, would overflow the sheet, wrong root) returns FALSUM
with the sheet untouched (vector VIII).

Tests: 77 asserts; every act is checked twice (applied == post AND the
recomputed diff is empty). Plants, all compiling: indentation ignored
in line equality; indentation not written; any whitespace treated as
blank; `deletae` from the wrong side - all caught. (My first
indentation plant `la == la` died on -Wtautological-compare: exit
codes read, redone.)

Formatter: two `operatores` divergences at `tabula_cellula(...) != ' '`
are a formatter false positive (silva formator worklog, same day).

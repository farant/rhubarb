# aemulator - worklog

## 2026-10-06 — A0: the oracle before the code

**The text dump is Ghostty's `plainString`, exactly** (formatter.zig
plain emit, `unwrap=false`, `trim=false`): a never-written cell
(codepoint 0) becomes a space only if real text follows it on the row;
a WRITTEN space is text and is kept even at the end; spacer cells
(wide tail/head) are skipped; a row with no text accumulates and is
emitted as newlines only if a later row has text (trailing blank rows
vanish); rows joined by `\n`. "A\tB" -> `A       B` (tab moves, writes
nothing); erase-right of "Hello World" from col 6 -> `Hello`. Matching
it lets vectors copy Ghostty's expected strings verbatim.

**Conversion rules** used for Terminal.zig's API-call tests (written in
the vector file's header): print(c) -> c, setCursorPos(r,c) -> CSI r;c H
(0 acts as 1), cursorUp(n) -> CSI n A (B C D likewise), CR/LF/BS
literal (LF is LF only - no implied CR without LNM), eraseLine
.right/.left -> CSI K / CSI 1K, eraseDisplay .below/.above -> CSI J /
CSI 1J, setAttribute -> SGR. Tests that need origin mode, scroll
regions, reflow (resize) or mode 2027 were cut or trimmed to phase A;
the trimmed one says so.

**Erased cells keep the current background** (Ghostty "eraseLine right
preserves background sgr"): the vector asserts the erased cell's style
as canonical SGR `\x1B[0;48;2;255;0;0m` - in Ghostty it is a bg-only
cell content, in ours it will be the cell's style. Canonical SGR forms
confirmed against probatio_stilus_terminalis: `\033[0m`, palette
colours always `38;5;n` / `48;5;n`.

**STML attribute values keep backslashes literally** (only entities are
decoded - probed with `stml formare`), so `\x1B[` survives and the test
decodes it; `<`, `&`, `"` are kept out of vectors anyway.

**`casus` is the latina macro for `case`** - the C side says
`exemplum`; the STML element may keep the name `casus` (a string).

Measured: 172 of Terminal.zig's 428 tests check only externally
visible state (keyword heuristic; plan AUDIENDA).

## 2026-10-06 — A1: the skeleton (API approved unchanged)

**A test-harness false alarm worth remembering:** "resize within
capacity allocates nothing" failed at first - the CREDO assertions
interleaved between the measurement and the check write their records
into the SAME piscina (`credo_aperire(piscina)`). Measure, do the work
into plain variables, read the usage again, THEN assert. (Section IX's
steady-state loop had no CREDO inside and was green all along - I
first misread the failing line as IX.)

**Ghostty print rules reproduced** (Terminal.print, no 2027, no
margins): pending wrap is set whenever the write reaches the last
column (even with DECAWM off - the mode is consulted when the NEXT
print wraps); a wide char at the last column writes a spacer-head
there, marks the row soft-wrapped and continues at x=0 of the next
row; writing over a wide head clears its tail, writing over a tail
clears its head.

**UTF-8 across `aemulator_scribere` calls:** the tokenizer returns an
IMPRIMERE run up to the end of the buffer, so a rune can be split; up
to 3 trailing bytes are carried. A sequence (ESC...) arriving while a
rune is pending turns it into U+FFFD, like a terminal that sees a
broken byte stream.

**Accidental pass:** "cursorLeft no wrap" (CSI 10 D at column 1) is
green in A1 because ignoring the CSI and clamping at 0 both leave the
text "A\nB". A2 must keep it green for the right reason.

**Purity:** aemulator.c is pure (house headers + <string.h>); the link
closure is not - piscina.c and stilus_terminalis.c (via
chorda_aedificator) pull stdio/stdlib. Exact list for the wasm door.

## 2026-10-06 — A2: the loopback surface

All 24 A1 debts passed on the first A2 run - the debt mechanism then
FAILED the suite once per paid debt ("DEBITUM SOLUTUM - promove")
until the `debitum` attributes were removed. That is the point of it.

**Style table collection - a plant that survived:** "collection does
not renumber cells" passed the first test because compaction keeps
order: the only styled cell that outlived a collection sat at an index
nothing below it freed, so its number never changed. The test now
interns a style that dies (red, index 1) before one that stays on
screen (green, index 2) and forces a collection: green must move to 1,
and only real renumbering keeps that cell green.

**A plant that did not compile proves nothing:** P6 first failed with
"unused parameter" - recorded as SUPERSTES by the runner but it was a
build failure. Replanted in a compiling form (erase with the DEFAULT
style's background) and caught.

**Resize with two screens:** each screen keeps ITS cursor row visible;
the primary's cursor while you are on the alternate screen is where it
was at entry, so shrinking below it scrolls the primary's top rows off
(into scrollback in Ghostty; dropped here until phase C). My first test
expected the primary untouched with its cursor below the new height -
the core was right, the test was wrong.

**ESC 7/8 came with 1049:** the saved-cursor machinery 1049 needs IS
DECSC/DECRC, so they were implemented in A2 (planned for B) with
Ghostty's "cursor save and restore" vector.

## 2026-10-06 — A3: the loopback

The first run failed everywhere - in the HARNESS: tessera's front
buffer is indexed with a fixed stride (`TESSERA_LATITUDO_MAXIMA`, 512),
not the current width (tessera_opus.c `_index`). Reading
`frons[y * latitudo + x]` compared the wrong cells.

With that fixed, hand frames, 60 random frames and the whole vicus
session matched; the only remaining mismatch was a REAL tessera bug:
a full repaint (resize) cleared with `ESC[2J` under the previous
frame's pen - BCE painted every cell with the old background while
tessera skipped blank cells. Fixed in tessera (pen reset before the
clear) with its own regression test; see tessera/phase-log.md. This
is what features/009 promised: tessera's byte goldens could never see
it, a semantic comparison saw it at once.

Comparison rules: blank = no bytes or one space on both sides (tessera
writes spaces, never-written emulator cells are empty); colours PLENI
(CCLVI would quantize RGB in the bytes but not in tessera's buffer);
multi-codepoint graphemes excluded (emulator v2). Every session must
leave `aemulator_ignota` at 0 - tessera emits nothing the emulator
does not understand.

C1 question closed: same behaviour as Ghostty (raw C1 byte -> U+FFFD,
UTF-8-encoded C1 -> ignored).

Plant P8 ("harness skips the style comparison") survived by
construction - removing a check can only show when the compared
things differ; P2 (SGR ignored) is the plant that proves the check
works.

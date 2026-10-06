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

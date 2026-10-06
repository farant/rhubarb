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

# series_terminalis worklog

## 2026-10-07 — high bytes vanished in the SS state (found by aemulator D3)

`ESC N` / `ESC O` enter our own SS state (a divergence: parameters
then one final byte, so `ESC O 2 P` is one lexeme for SS3 keys). The
state had rules for C0, digits, `;`, DEL and 0x20..0x7E, and NONE for
0x80..0xFF - the transition table's default is "ignore, stay". So
`ESC N` + a UTF-8 rune lost every byte of the rune, and the next ASCII
byte became the SS final. aemulator D3 saw it as a single shift that
skipped the emoji and landed on the following `q`; in input mode
`ESC O é` would have dropped the é the same way.

Fix: the escape state's existing "ESC + high byte" rule (FUGA, byte NOT
consumed) now covers the SS state too: `ESC N é` -> FUGA (`ESC N`) +
IMPRIMERE (é). Three red tests in probatio_series_terminalis (write
mode N and O-with-parameter, input mode O). The interpreter already
treats a non-lone FUGA as nothing, so input now keeps the é; the
emulator reads FUGA with introducer N/O as a single shift.

Consumers: aemulator, interpres (via rivus), and the tessera amalgam
(regenerated; `generata` stage VII judges it).

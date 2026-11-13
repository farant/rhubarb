# codificator_a64 worklog

## 2026-11-03 — born (officina M5 T3)

Encoder for the A64 subset officina's native backend needs (64
formae). Spec G22-23: lib/ library, pinned clang golden, re-pin
script. Everything here is checked against
probationes/fixa/codificator_a64/aurum.tsv; when adding a forma, add
semen rows (valid edges + "!" refusals) and run
tools/codificator_a64_oraculum.sh (needs Xcode clang; gates don't).

clang invocation that works on clang 21: `clang -cc1as -triple
arm64-apple-macosx -filetype asm -show-encoding -o - file.s`. Without
`-filetype asm -o -` it prints nothing. One `; encoding: [b0,b1,b2,b3]`
per instruction, little-endian; some lines carry an extra comment line
(`; =16773120`) with no encoding - parse only encoding lines. clang
prints aliases (cset, ubfx, mov) for the raw forms we feed it; we
compare words, never text.

Field overloads (documented in the header): UBFM/SBFM immr/imms in
immediatum/translatio; FMOV_GEN direction in translatio; conversion
FP width in translatio; FMOV_IMM takes imm8 (call
codificator_a64_fmov_immediatum first); pair Rt2 in rm. latitudo is
bits for register forms and BYTES for memory forms (plan's choice).

bitmask: rotation search instead of LLVM's shifted-mask formulas - my
first draft of the wrapped-run branch (ones crossing the element
boundary) went wrong in the arithmetic; the search handles wrapped
and unwrapped runs identically. The suite decodes all 8192 triples
with its own DecodeBitMasks, so the encoder is checked against an
independent inverse, not against itself.

Formatter note: a brace-less nested `per (...) per (...) per (...) {`
is mangled by formator (operands split across lines). Use one loop
or braces.

# numerus_romanus worklog

## 2026-09-26 — scribere + exprimere (Fran: fill latina.h 0-3999; canonical form for all n)

Added the writer side. `numerus_romanus_scribere(n)` = canonical
subtractive form for 1-3999; 0 and >3999 return an empty chorda (0 has
no numeral; past MMMCMXCIX the Romans used the vinculum, which an
identifier can't carry). `numerus_romanus_exprimere(n)` = a C89
expression in latina.h vocabulary for ANY n — what the planned
formatter rewrite (desideratum ...QCHQ) will emit for digit literals:

- 0 -> `ZEPHYRUM`; <= 3999 -> the numeral itself.
- exact multiple of 1024 that is NOT a round thousand -> `k * MXXIV`
  (4096 -> `IV * MXXIV`, 1 MiB -> `MXXIV * MXXIV`): keeps binary intent.
- otherwise thousands, vinculum-style: `IV * M`, `V * M + CCLXXX`,
  `IV * M * M + D * M`. Top group stays whole when < 4000 (`MD * M`).
- a term whose value exceeds int gets `(i64)` on its first factor —
  a decimal literal silently becomes long, but a product of int macros
  overflows (UB). A binary factor that is a sum gets parentheses.

Tie-break decided in-session: 128000 is both `CXXV * MXXIV` and
`CXXVIII * M`; round thousands stay thousands. So 4500*1024 = 4608000
renders decimal — my own first test expected the binary form and was
wrong; the rule was right.

The reader is unchanged and stays strict (three-repeat cap, MMMM
rejected) — with latina.h capped at 3999 the two agree by construction.

**Test design:** the test carries a tiny evaluator (expr/term/factor,
numerals via `legere`) that computes each expression with C int
semantics and flags any product/sum over 0x7FFFFFFF without `(i64)`.
Property: eval(exprimere(n)) == n with no overflow for all n in
0-70000, every k*1024 and k*10^6 for k <= 4096, and 10k `sors`
values below 2^40. Plants (tie-break, cast, parens) each red.

**Bit me:** `per (k = numerus - I; k >= ZEPHYRUM; k--)` with k `i32`
— house i32 is UNSIGNED, the loop never ends, segfault (exit 139, no
output: stdout buffered). s32. Found via `lldb --batch -o run -o bt`.
Also: the test first used `casus` as a variable (latina `case`) — the
new bugs/018 hint named it on the spot.

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

## 2026-09-27 — latina.h numerals generated: ZEPHYRUM..MMMCMXCIX, MMMM/MMMMXCVI retired (step 2)

`tools/latina_numeri.{c,sh}` rewrites the section between
`/* NUMERI ROMANI - GENERATUM` and `/* finis numerorum generatorum */`
from `numerus_romanus_scribere` (default = compare, rc 1 stale;
`-scribere` = write). Gate: `generata` section V (plant: one wrong
value -> red, byte offset named).

**Bootstrap loop:** the generator and numerus_romanus.c are themselves
written in latina numerals, so emptying the block first made the
generator uncompilable. Bootstrapped by wrapping the OLD hand block in
the markers and letting the tool replace it. If the block is ever
hand-broken: `git checkout include/latina.h`, then `-scribere`.

**Measured before committing to it:** 3753 new names vs the tree with
comments/strings stripped = zero code collisions (flatura.h's CCLVIII
was already an undefined-but-unexpanded macro); vs 3643 macOS SDK
headers = only dns_util.h (union members MD, MX), never included —
hence "system headers before latina.h" in the section comment. Cost:
clang +0.6 ms/TU; examen over 60 lib files unchanged (8.9 s, 1.6 GB
peak). Real cost is source size: latina.h 13 KB -> 121 KB, baked
silva_latina_datum.c 59 KB -> 567 KB, silva amalgam +0.6 MB; officina
and tessera amalgams carry latina.h inline too (all regenerated).

**Migration** (scratch tokenizer, code tokens only): 179 sites / 81
files; `MMMMXCVI` -> `IV * MXXIV`, `MMMM` -> `IV * M`; parenthesized
when the value is a #define body (8: HTTP_*, CATENA_OPTIMA, ...) or
follows / % ~ ! sizeof (none did). `tools/mensor.c` MORA_AMPLA was
ALREADY an unparenthesized product (`MMMMXCVI * M`) — now
`(IV * MXXIV * M)`. Frozen fixtures (probationes/fixa/roundtrip/
latina.h) untouched; archived lapifex tests migrated so they still
compile. Breaks lapide's loca.thistle (3 uses) — MUTATIONES line
marked FRANGIT.

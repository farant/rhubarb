# silva_lexema.worklog.md

## 2026-10-15 - benchmark + keyword lookup (strlen per identifier)

New tool `./silva/lexema_mensura.sh [-n N] [-machina] [FILE...]`: bytes,
tokens and throughput of `silva_lexare_cruda` (raw) and `silva_lexare`
(raw + trivia attachment), CPU time (clock), best of N with a fresh
pool per repetition (allocation counted). Default inputs latina.h and
silva_c89_semantica.c; `lib/*.c` gives a corpus SUMMA. Use -n 40 on a
loaded machine - -n 5 on a 119 KB file swung +-25%.

Why: profiling `examen-corpus` (percursus) showed 59% of samples under
`silva_includendum_praebere` (headers re-supplied per TU; latina.h,
121 KB / 4045 #defines, in all 2042 corpus files), the raw lexer ~19%
self. A sample of the benchmark itself put `strlen` at 18%:
`_quaerere_verbum_clausum` ran for EVERY identifier and called strlen
on each of the 32 keyword literals per comparison.
Fix: keyword lengths computed once (lazy static table, no hand-typed
lengths); pre-reject identifiers outside 2..8 letters or not starting
'a'..'w' (Roman numerals, long Latin words, capitals) before the scan.
Measured at -n 40 against HEAD: raw 77.6 -> 111 MB/s (+43%), with
trivia 58 -> 75 MB/s (+30%), 17.8 -> 25.5 M tokens/s.
Test gap found by the first plant (bound `> VII` stayed GREEN - no test
lexed the 8-letter keywords): new block in probatio_silva_lexema lexes
all 32 keywords + 15 near misses; both plants (`> VII`, first letter
`> 'v'`) now red.
Tried and REVERTED: backslash fast paths in _sumere / _aspicere_eff /
_finis_eff (a splice always starts with '\'): no measurable gain - the
per-character cost is the call and the line/column bookkeeping, not the
splice check.
Remaining profile (keyword fix in): per-character loop ~45%, Xar growth
~15%, allocation + zeroing ~18%, token construction ~12% - no single
hotspot. Compiler: house flags are -O2 -g; -O3 alone +0%, -O2 -flto
+24% raw / +18% attached (cross-file inlining of xar/piscina/materia
helpers) - a build-system decision (link cost x 276 root tests; ThinLTO
the candidate), not taken.

## 2026-11-03 — silva budget step 3: character classes, bulk runs, index-range trivia

**Profile (percursus parse-only, after step 2).** Lexing was 19.7% of the run (`silva_lexare`), mostly header lexing during directive processing. Inside it: `silva_lexare_cruda`'s own per-character code ~1,500 samples, token construction ~800, `_sumere` ~720. Trivia attachment (`silva_spatia_attachere`) was another 6.5% of the run, ~80% of it Xar appends and allocation.

**Changes (all internal; no API change):**
1. **Trivia attachment.** Pending trivia between two real tokens is always CONTIGUOUS in the raw stream, so the pending Xar (appended per trivia token, and a NEW Xar created after every token that had trivia) is replaced by the index range `cruda[ab, i)`. The six-way kind test is a switch (`_est_trivia`).
2. **Trivia quick reject.** Trivia can only start at `\\` (splice), space, tab, `/`, `\n`, `\r`. Any other raw char returns NIHIL at once; before, every real token paid ~6 effective reads (each a splice check with bounds) first.
3. **Character classes + bulk runs.** `_classes[256]` is built at first use from the existing predicates (`_est_corpus_identificatoris`, `_est_cifra`, …), so table and predicates can't disagree. `_cursum_sumere` consumes a run of raw bytes of one class in a tight loop (line/column as in `_progredi_crudum`). NO class contains `\\`, so a run stops wherever a splice could begin, and the effective path takes one step there: without a splice the effective char IS the raw char. Used for identifiers, digit runs (decimal/hex/fraction/exponent), spaces, tabs, string/char literal bodies, and block/line comment bodies. `_sumere_dum` = run, then (only if the stop byte is `\\`) one effective step, repeat.
4. **Punctuation.** If none of the next three raw bytes is `\\`, c/c2/c3 are read raw and the n chars are advanced directly; the ~40-way if-chain is a switch on the first char. `_lexema_proximum` reads its two lookahead chars raw under the same condition.

**Bench.**

| | before | after |
|---|---|---|
| lexer micro-bench (lexema_mensura -n 3, lib/*.c): instructions | 26.7 G | 16.2 G (-39%) |
| … stage by stage | | trivia range 25.5, quick reject 25.2, runs 18.7, punctuation 16.8, raw lookahead 16.2 |
| percursus parse-only: instructions | 306.1 G | 262.0 G (-14.4%) |
| percursus parse-only: cycles | 81.1 G | 71.3 G (load ~5 vs ~138!) |
| percursus -semantica: instructions | 713.1 G | 660.7 G (-7.3%) |
| computus usus per file | 39.6 MB (stml.c) | 36.7 MB |

The machine had load 90–145 during most of this work: 8 orphaned `yes` processes (parent launchd, started 16:01, not from this session). Instruction counts carry the comparison. The -semantica peak RSS read 2.27 GB at g2 but 3.2 GB here. Re-measured with the HEAD lexer swapped in at the same moment: 3.23 GB RSS / 3.85 GB footprint vs 3.17–3.31 / 3.80 with the new lexer, so the jump is environmental, not this change.

**Equivalence.**
- A new token-level differential (scratch `lexdiff`: raw AND attached streams, every field incl. scissurae and trivia lists, hashed per file) old vs new over all 3,275 tracked .c/.h plus 6,000 fuzz inputs. The fuzz inputs are random concatenations biased to splices (`\\\n`, `\\\r\n`), CR/CRLF, unterminated strings and comments, NUL/high bytes, and every punctuation char. Result: 9,275/9,275 identical.
- A planted off-by-one in the trivia split changed 5,122 of the outputs, so the differential has teeth.
- aequivalentia -conferre: 482/483 trees byte-identical (silva_lexema.c itself changed).

**Plants** (7). The differential caught all 7, but the unit tests caught only 2 at first: B (identifier ignores splice) and E (trivia rejects splice). Missed: A (string class contains `\\`), C (run doesn't count newlines), D (punctuation advances one column), F (lookahead checks only the first raw byte for `\\`), G (punctuation checks only the first of three bytes). New section "cursus celeres" in probatio_silva_lexema.c: escaped quote and splice inside a string, line/column after a multi-line block comment, columns after `<<=` and `->`, `.`+splice+`5`, and a splice inside `<<=`. It passes on the old lexer too, and all 7 plants are now red.

**Amalgam trap.** The first suite run went 16/57: `excludenda_silva` refused "definitio duplicata '_classis' (lib/runae.c …)". The amalgam is ONE translation unit, so a new `interior` helper must not reuse a static name from any other amalgamated file. Renamed to `_classis_octeti`, and checked every other new name first (`grep -w` over lib/, silva/fontes/, materia/fontes/). Worth checking BEFORE the suite whenever a silva source gains static helpers.

**Computus golden.** Memory columns only (usus/apex/otiosa/allocationes and *_lexandi; e.g. arrays.c usus 310,728 -> 294,504: no pending-trivia Xars). Token counts unchanged. Regenerated in the same commit.

**What's left in the lexer:** token construction (`silva_token_ex_fonte` -> `materia_token_creare`/`_cauda`, piscina allocation) is now the largest piece. It sits behind the materia token API, outside steps 1–3. The -flto note above (+24% raw) still applies.

## 2026-11-06 — SILVA_TOKEN_COMMENTUM_LEXATUM (budget fusionis item 5b)

`silva_spatia_attachere` now marks the token that receives a comment trivium (bit IV of `vexilla`): it tracks the first and last comment index in each pending trivia run, then the prior token gets the bit if the first comment falls before the newline split and the next token if the last one falls at or after it. Lexer-only signal: the trivia setters don't maintain it, so only raw lexer tokens may be trusted (the expander reads it on captured directive lines). Details and measurements in silva_annotationes.worklog.md (item 5b). Test: probatio_silva_lexema "vexillum commenti lexati".

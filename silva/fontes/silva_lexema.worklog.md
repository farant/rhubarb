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


## 2026-07-11 — claudere-then-query trap (M4b chunk A find)

`credo_claudere` RESETS state ("conditio solum reficitur",
credo.c) — calling `credo_omnia_praeterierunt()` AFTER it always
returns VERUM (zero failures visible). A new probatio that ended
`credo_claudere(); redde credo_omnia_praeterierunt() ? 0 : 1;`
exited 0 despite a failing assert — caught only by a deliberate
harness-teeth test (broke an expectation on purpose; suite stayed
green; investigated). Correct order (probatio_piscina is the
canonical template): QUERY first, then compendium, then claudere:

    praeteritus = credo_omnia_praeterierunt();
    credo_imprimere_compendium();
    credo_claudere();
    redde praeteritus ? ZEPHYRUM : I;

Lesson doubled: (1) query-before-claudere; (2) the teeth test
(deliberate break → expect FRACTAE) is cheap and catches exactly
this class — a suite that cannot fail.

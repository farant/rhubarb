# dispositio — phase log

Pure layout in cells (track b, B2, of ludus_tessera). Plan:
`project-specs/dispositio-plan.md`. Oracle: Clay (`../clay` @ e6cc369).

## D0 — the oracle (2026-10-03)

**`tools/dispositio_oraculum.sh`** lays out an STML tree with Clay and
prints one rectangle per node in preorder (`nodus i x y w h`, Clay's
floats as `%g`). The tree format is the one dispositio's tests will
read: `<dispositio latitudo altitudo>` (the surface) holding `<nodus>`
with `directio`, per-axis `apta[:min:max] | crescens[:min:max] |
fixa:n | pars:centesimae`, `spatium="s d sup inf"`, `intervallum`,
`allineatio_x/_y`, `praecidere_x/_y`. Clay's implicit root (a row
the size of the surface) is part of the contract.

**Where the code lives (Fran): a new `oracula/` directory** - glue
around foreign reference implementations used as oracles. The C99
wrapper (`oracula/clay/clay_vinculum.c`, which includes clay.h and
calls `Clay_*`) cannot pass examen (C89: REICE, 16 violations) or the
Latin lint (`clay`, `allineatio` new) - measured before the change.
`oracula` was added BY NAME, with a comment, to every name-based list:
the pre-commit examen selector AND its formatter-warning twin (the
pattern occurs twice in pre-commit - my first edit's assertion caught
that), the lint (shell hook + `silva.py`'s twin), `tools/formare_viae.sh`,
`silva/identitates.sh`, examen's census prune. The plain-C header the
Latin driver and the C99 glue share must avoid every latina macro
(`interior` is `static` - padding became `spatium_*`), and `clay.h`
never shares a TU with latina.h (its identifiers would be rewritten).

**The exclusion I first missed was the deepest one.** The vocabulary
lint counts words from EVERY TRACKED .c/.h (`git ls-files` in
oratio/vocabula.sh), filtered by `ORATIO_VOCABULA_EXCLUSA`
(knotapel/vendor/archivum) - not only the committed paths. My check
"lint on lib/dispensator.c: NOVA 0" ran while oracula/ was untracked,
so it proved nothing; the first commit attempt (blocked by an unrelated
`-nt` diagnostic) left the files STAGED, and the second attempt's
`unci` gate section XVIII (lint of lib/piscina.c expects "nihil novi")
reported clay/allineatio. Every future commit would have failed the
same way. `oracula/` added to ORATIO_VOCABULA_EXCLUSA (not to the
prose list: no prose there). Lesson: a check run before the state
changes (untracked → tracked) verifies the wrong world.

**Pinned:** the script refuses with exit 2, by name, without the clone
or at another commit (`CLAY_DIR` overrides the path - both reds run).
`-probare` compares fixtures in `oracula/clay/probationes/`: a
hand-worked column (padding, gap, fixed, grow, fixed-height bottom row:
Clay agreed with the hand table exactly) and three grow children in 80
cells. A wrong expectation reports FRACTUM, exit 1.

**The float question (plan AUDIENDA), answered:** integer inputs stay
integral wherever no division occurs; a grow split that does not
divide comes back fractional (80/3 → 26.6667 ×3 at 0, 26.6667,
53.3333). That fixture records Clay's truth; dispositio will give
27, 27, 26 (remainder to the earliest - the named divergence), and
D2's comparison rule is defined against exactly this case.

**Found for D1:** `allineatio` is not in the lexicon (the lint flagged
it); dispositio lives in lib/ and must use a known word or add a
glossary entry.

Owed in main: the `pythonica` gate (silva.py edited; red in secunda by
construction).

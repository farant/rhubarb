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

## D1 — fit, fixed, percent, positions (2026-10-03)

`include/dispositio.h` + `lib/dispositio.c`: nodes added under a parent
(-1 = Clay's implicit root: a FIXED row the size of the surface),
`dispositio_computare(d, w, h, mensor, ctx)` (mensor ignored until D3 -
in the signature now so D3 does not change the API), `dispositio_fines`
in cells. Three passes, an integer translation of Clay e6cc369 read line
by line: (1) fit, leaves up - reverse index order works because a
parent is always added before its children; (2) per axis, x then y,
root down - PERCENT of (parent − padding − gaps along the axis), and
across the axis FIT/GROW clamped to the inner size, floored at their
content minimum (a 50-wide child inside a 30-wide parent stays 50 when
its own minimum is 50 - Clay's semantics, pinned by d1_minmax); (3)
positions - along the axis the free space by alignment, floored at 0;
across, `(inner − child) × {0, ½, 1}` NOT floored (a centred child
wider than a clipping parent goes negative).

**Integer rules (named divergences):** centring = floor of the half,
with a real floor for negatives (`-3/2 → -2`; C89's division is
implementation-defined there - `modulus.h` warns the same); PERCENT =
floor(avail × centesimae / 100). Comparison rule against Clay:
ours == floor(Clay + 0.001) for every value (the epsilon absorbs
`0.7f` = 0.69999…). **Kept from Clay:** an empty node gets no padding
across its axis (empty padded row "1 2 3 4" → 3 × 0) - arguably a quirk,
mirrored for oracle agreement, pinned by d1_vacuum, to revisit if it
ever bites.

**Tests:** `probationes/probatio_dispositio.c` (20 asserts) reads the
D1 fixtures and Clay's committed answers from oracula/clay/probationes
(no clone needed at test time; `tools/dispositio_oraculum.sh -probare`
re-checks the answers against live Clay: 9/9). Seven fixtures (row with
padding/gap, alignment centre/end on both axes with odd remainders,
fit container, percent along/across incl. 0.7, min/max clamps, clipping
with a negative centre, empty padded nodes), every value also checked
by hand before trusting Clay. Red: a stub (each mismatch printed with
ours and Clay's). Plants, all caught by name: gap counted n times
(d1_apta), padding one-sided (3 fixtures), centre by truncation
(d1_praecidere −1 vs −2), percent without gaps (d1_pars 11 vs 9), cross
offset floored at 0 (d1_praecidere). Three of my plant runs LOOKED
uncaught - my grep anchored `^  FRACTA` and the failure lines began
with credo's progress dots; the exit codes (1) said otherwise. The
exit-code rule from B1 is what kept that from being a false report.

`allineatio` got a glossary entry (Fran: glossary entries are always
fine when needed).

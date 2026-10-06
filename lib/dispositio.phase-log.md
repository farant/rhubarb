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

## D2 — grow and shrink (2026-10-03)

Along the axis, after percent: room > 0 with GROW children → Clay's
equalising (the smallest grow child first, up to the next smallest,
then evenly, each capped at its max); room < 0 → FIT and GROW children
shrink largest first, floored at their content minimum - unless the
parent clips that axis (then nothing is compressed). Members are kept
in tree order in a scratch Xar on the Dispositio; a capped or floored
member is marked -1, not removed, so the order survives.

**The remainder rule changed from the plan, for a reason.** The plan
said "leftover cells to the earliest children". Worked through, that
drifts edges: with k equal children and r leftover cells, every edge
after the first r children sits up to ~k/4 cells right of the exact
split - visible misalignment in a wide equal split. The rule now is
**every edge is the floor of the exact edge**: in an even split member
j gains floor((j+1)S/k) − floor(jS/k), and in a shrink loses
ceil((j+1)D/k) − ceil(jD/k) (so its edges still floor). Sizes differ by
at most a cell, leftovers spread evenly. And it gave ONE comparison
rule for everything: every edge (x, x+w, y, y+h) == floor(Clay edge +
0.02). The D1 fixtures (centre 12.5, percent 10.5 and 5.28, a −1.5
cross offset) all satisfy it unchanged.

**Why 0.02 now, not 0.001:** Clay's grow/shrink loop divides each step
by the number of ALL resizable children, not by the size of the
smallest/largest group, so it approaches the equal split geometrically
and stops at an epsilon of 0.01 - a value can read 5.99 where the exact
answer is 6. Its limit is the proper equalisation, which is what we
compute exactly in integers.

**Fixtures** (Clay's answers, every one also worked by hand):
`columna` and `tres_crescentes` from D0; `d2_inaequales` (0 and 20 grow
in 60 beside a fixed 10: the smaller catches up, then 5 each → 25/25);
`d2_maximum` (four grows, two capped: 10, 26.5, 25, 26.5 → ours 10,
26, 25, 27 - every edge the floor of Clay's); `d2_contractio` (clipping
FIT children 15 and 10 into 17 with a fixed 2: 15→10, then 7.5/7.5 →
ours 7/8); `d2_praecidere` (a clipping parent: no compression);
`d2_minimum` (min 7 stops the shrink, the rest goes to the other: 7,
5). 27 asserts; oracle 14/14.

**Plants, all caught by name (exit codes read):** grow ignoring the
equalisation (d2_inaequales), the plan's remainder-to-earliest rule
(d2_maximum, tres_crescentes), shrink inside a clipping parent
(d2_praecidere), shrink ignoring the minimum (d2_minimum), grow
ignoring max (d2_maximum).

## D3 — text (2026-10-03)

**Clay's model, read before writing:** text is not a node property in
Clay but a TEXT CHILD sized {unwrapped width, line height} whose
minimum width is the LONGEST WORD - Clay prepares for word wrapping
even when a text does not wrap, so a parent may shrink below the full
text. dispositio mirrors that: a node with `textus` gets a VIRTUAL
first child, fixed (measured width × 1), placed before the real
children with the gap after it; its minimum is the widest
space-separated word; with no measurer (`mensor` NIHIL) text is
ignored. Multiple lines ('\n') and wrapping are not in v1.

**One measurer on both sides.** The oracle glue gained a measure hook
(`vinculum_mensorem_ponere`): the Latin driver hands Clay a C function
that measures with `runae_latitudo_textus` under GRAPHEMATUM - the same
function the test's `DispositioMensor` calls. So "中文 ok" is 7 cells
for both (each CJK rune 2), not 9 (bytes).

**Fixtures** (predicted by hand, Clay agreed on every value):
`d3_textus` - "salve" with side padding (7 × 1); a column holding
"中文 ok" and a 3×2 child with gap 1 (7 × 4, child at y+2); a row
holding "ab" and a 4×3 child with gap 2 (8 × 3, child at x+4).
`d3_contractio` - "aa bbbb" (7 wide, minimum 4) beside a fixed 6 in a
12-wide row shrinks to 6: possible only because of the longest-word
minimum. 32 asserts (incl. "no measurer → ignored"); oracle 16/16.

Plants, all caught by name with clean compiles: bytes instead of the
measurer (d3_textus), the minimum = whole text (d3_contractio), no gap
after the text (d3_textus children), text not counted as a child in
the fit pass (d3_textus).

## D4 — RELATIO (2026-10-03)

**`dispositio` v1 is done:** a pure layout library in CELLS - Clay's
model (direction; per axis FIT / GROW / FIXED / PERCENT with min/max;
padding; gap; alignment; clip) and Clay's three passes, translated to
integers; text through a caller's measurer. 32 asserts over 16
fixtures; the oracle re-checks every committed answer against live
Clay (`tools/dispositio_oraculum.sh -probare`: 16/16, Clay e6cc369).

**Named differences from Clay** (all in include/dispositio.h):
1. Integers in cells - cell alignment by construction (track b's
   reason for the library).
2. Centre = floor of the half, a true floor for negatives (−3/2 → −2).
3. PERCENT = floor(avail × centesimae / 100).
4. Grow/shrink split: EVERY EDGE IS THE FLOOR OF THE EXACT EDGE - the
   plan's "remainder to the earliest" drifted edges up to ~k/4 cells
   and became a plant.
One comparison rule covers all of them: every edge == floor(Clay edge
+ 0.02).

**Kept from Clay on purpose:** an empty node gets no padding across
its axis (d1_vacuum); the text minimum is the longest word (d3).

**What the work found, by weight:**
1. **The oracle needed its own house category** (`oracula/`, Fran):
   C99 glue with foreign identifiers cannot live under the house's C89
   judge or Latin lint. Excluding it by name took SEVEN places, and the
   deepest one (the vocabulary counts words of every TRACKED file) was
   only found when a gate ran with the files staged - a check run
   before the state change verifies the wrong world.
2. **Clay's equalising converges, it does not divide** (the divisor is
   every resizable child), so its values stop at ±0.01 of the exact
   answer - the reason for 0.02, and the reason ours (exact integers)
   is the cleaner reading of the same rule.
3. **Read before deciding.** The remainder rule, the text model
   (a child with a longest-word minimum) and the empty-node quirk all
   came from reading Clay line by line; each would have been guessed
   wrong.
4. **Process:** two plants looked uncaught (an anchored grep missed
   lines starting with credo's dots) and one plant did not compile
   (B1) - reading each plant's EXIT CODE is now the rule.

**Not in v1:** multi-line text and wrapping; floating/absolute
positioning; aspect ratio; scroll offsets. Each waits for a screen that
needs it.

**Next:** track (b) B3 - pictor laid out with dispositio, responsive in
both targets.

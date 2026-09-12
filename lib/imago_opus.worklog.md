
## 2026-09-12 — the area resampler, and this library's first automated gate

Built for `icones` (`project-specs/icones-spec.md` §5,
`icones-plan-1.md` task 1), but the resampler is general and belongs
here rather than in a bundle-icon library — the same argument that put
`mkdir -p` in `filum` instead of inside `fasciculum`.

**This library had NO automated test before today.**
`probationes/probatio_imago.c` opens a `fenestra`, presents pixels and
calls `fenestra_destruere`; it contains **zero `CREDO_` assertions**. So
`IMAGO_SCALA_PROXIMUS` and `IMAGO_SCALA_BILINEARIS` — which `specimen`,
`pictor` and every thumbnail path rely on — had never been pinned by
anything. `probationes/probatio_imago_opus.c` now pins both BEFORE the
new mode, so the new mode's gate is not the only thing standing there.
26 assertions.

That absence also explains a claim I had to retract mid-design. A
29-day-old memory note said the resampler "can perturb pixels even at
1:1", and I repeated it three times before checking. **Measured, bilinear
at exactly 1:1 is arithmetically exact**: `scale = ((fons-1)·FP_ONE) /
(dest-1)` equals `FP_ONE` when the dimensions match, so the fraction is
zero and every pixel passes through untouched. The note was plausible
folklore precisely because nothing was ever testing the thing it
described.

**The real hazard is one layer up, and it survived into the spec.** The
danger is not unit scale but *a path that believes it is at unit scale*:
`imago_extrahere_et_scalare` and `imago_scalare_ad_limites` derive output
dimensions from MAX bounds preserving aspect
(`lib/imago_opus.c:263-271`), truncating the ratio and then the product
in 16-bit fixed point — so asking 512 from a 512-wide crop can yield
511, and then every pixel IS interpolated. A caller who needs exact
dimensions must compute them itself. `SCALE_FP_SHIFT` is 16, not the 12
I first wrote; and there are THREE zero-dimension guards (`:173`,
`:246`, `:320`), not one. Both citations were wrong in the first draft of
the spec because they came from a `sed` range offset rather than the
file.

**`IMAGO_SCALA_AREA`: why bilinear is not merely lower quality.**
Bilinear samples a 2×2 neighbourhood regardless of scale factor.
Downscaling 1024→16 is 64×, so each output pixel covers 4,096 source
pixels and bilinear reads **four** of them — 99.9% of the image is
discarded. At icon ratios that is the difference between a small picture
and aliasing noise. Appended after the existing two (enums append, never
reorder), so no existing caller changed.

**Alpha weighting is correctness, not polish.** `imago_typus.h` states
the buffer is RGBA8 **not premultiplied**, so averaging RGB while
ignoring alpha lets invisible pixels contribute colour — the classic
halo, and icons are mostly transparent edges. The accumulation
premultiplies and divides back out: `out_a = Σ(a·w)/Σw`, `out_rgb =
Σ(rgb·a·w)/Σ(a·w)`, round-half-up.

The gate for it separates on **every channel**, which is what makes it
loud: one opaque red pixel beside one fully-transparent green pixel,
2:1. Correct is `(255, 0, 0, 128)`; the naive average is
`(127, 127, 127, 127)`. A naive implementation cannot squeak past.

**Fractional coverage, because the happy path hides its absence.** From
a 1024px source every icon size is an exact power-of-two ratio, so
integer binning would have passed every test I would naturally have
written. The pinned case is therefore a 3→2 ratio, where every footprint
edge is partial.

**Both pinned constants came from an oracle of a different kind**, not
from my own arithmetic checking itself: Python's exact rationals give
spans `[0, 3/2)` and `[3/2, 3)` at weight `3/2`, hence **40** and
**200**; and the alpha case **128 / 255 / 0**. Same discipline
`imago_png` adopted after discovering stb_image validates neither
Adler-32 nor chunk CRCs, so 54 assertions stayed green on a corrupted
file.

**A prediction stated before the run, and kept.** `_scalare_area` has no
1:1 short-circuit, and I2 c asserts exactness at 1:1 anyway — because a
single-pixel footprint takes full weight and round-half-up does not move
the value. Writing the prediction into the test's comment meant a
failure would have named my reasoning as wrong rather than tempting me
to paper over it with a guard. It passed.

**THE PLANT MISSED ITS TARGET, AND THAT WAS THE FINDING.** I planted the
alpha weight away in the RED accumulator (`* a` → `* 255`) expecting I2
a — the halo gate — to redden. It went red at I2 c instead, and I2 a's
four assertions stayed GREEN.

The reason is arithmetic, not luck. I2 a's fixture was opaque red beside
**transparent green**, so the only pixel carrying red already had alpha
255: substituting the constant 255 for it changes nothing, and the plant
was **invisible by construction**. It surfaced in I2 c only because that
block's middle pixel has alpha 200.

Two consequences, both worth keeping:

- **I2 a was testing alpha weighting on ONE channel.** Its red and blue
  assertions asserted nothing against that fixture — they read like
  coverage and were decoration. Re-planting on `summa_g` reddened it
  immediately (`:144`), confirming the gate is live but narrow. The
  fixture is now opaque red beside **transparent WHITE**, so the
  transparent pixel carries colour in every channel and blinding ANY of
  the three accumulators falsifies it.
- **I2 c was guarding alpha weighting by accident.** It was written to
  pin 1:1 exactness and reddened under BOTH plants, because alpha 200
  makes the weighting observable there. Unplanned double duty is not a
  virtue; it meant a real hole in I2 a was masked by a green suite.

The general shape, which is the channel-wise form of a lesson this arc
already paid for: **a fixture where the varied property is constant on
some axis makes assertions along that axis unfalsifiable.** Planting on
one accumulator and watching a DIFFERENT block fail is the cheapest
possible way to discover it — and "the plant went red" would have been a
satisfying and misleading place to stop.

**`s64` throughout, and a wrap point corrected.** A 64×64 footprint sums
to ~266M (inside `s32`) but a 256× reduction passes 4 billion, so
accumulators are `s64`. Offsets are `s64` too, because the existing
paths compute `src_y * fons_lat * IV` in `s32`. The spec's §11 says that
wraps above 8192² sources — **wrong**: 8192² × 4 = 268M, comfortably
inside `s32`, and the real wrap is near **23170²**. Found only when
writing the arithmetic out, which is the argument for plans carrying
real code. The as-built corrects it.

**The fixture fix is demonstrated, not asserted.** After widening I2 a's
transparent pixel from green to WHITE, the *identical* red-channel plant
that had been completely invisible now reddens `probatio_imago_opus.c:152`.
Same plant, same code, different fixture, opposite outcome — which is the
only kind of evidence that a test-strengthening actually strengthened
anything rather than just keeping the suite green.

Final calibration, three plants each hitting a DISTINCT target:
red-channel alpha weight → `:152`; green-channel alpha weight → `:144`;
fractional coverage (`pondus` forced constant) → `:186` and `:188`, the
40/200 pins, **with I2 a and I2 c both surviving**. That disjointness is
the point: I2 a tests alpha weighting, I2 b tests coverage, and neither
is quietly standing in for the other.

**Two refusals from `silva.planta` before that third plant landed, each
naming its own rule — worth recording because I had both rules written
in my own plan's Global Constraints and still tripped them.**

- **`Editio` refuses a FLAT replacement over a MULTI-LINE anchor.**
  Collapsing a two-line expression (15 tokens) into one line (5 tokens)
  raises *"novus planus lexematibus imparibus: forma perderetur"* — the
  editor protects the file's line structure rather than silently
  reflowing it. Give `novus` its own newline, or pass
  `tolerans='spatia'` for verbatim writing.
- **`planta`'s PRE-FLIGHT rejects a plant that breaks `-Werror`**
  (*"planta non compilat (praevolatus, nihil cursum)"*), so nothing ran
  and the tree stayed clean. My second attempt kept `pondus_y`
  referenced but dropped `x_ab`/`x_ad`, and `-Wunused-variable` killed
  it. A plant must satisfy BOTH guards at once: preserve line shape AND
  keep every variable used. The working form multiplies the real term by
  ZEPHYRUM and adds the constant, so all three variables stay live and
  the result is fixed.

Related staleness to expect: **`build/portae/radix.planta_rubra.acta` is
not cleared by a refusal or by a green run**, so reading it after either
shows the PREVIOUS plant's reds. Both times above, the three stale lines
looked like a result.

**Two notes for whoever executes a plan in this house next.**
`./silva/scribe.sh` returns **rc=1 with `examen: REICE` on a TDD red
step** and keeps the file ("plagula scripta manet, emenda") — it flagged
the undeclared `IMAGO_SCALA_AREA` before clang did. That is the tool
working, not the write failing. And **touching `include/latina.h` forces
a cold rebuild of the whole tree** (by design — `tools/vexilla.sh` uses
exactly that mechanism), so the excubitor then prints a 15-row staleness
table that pushes the suite's verdict off the end of `tail`. Grep the log
for `Totalis`; do not tail it.

**Why this commit shows 790 changed lines in `latina.h` for a one-line
addition.** I passed `include/latina.h` to `./silva/formator.sh
-scribere`, and the formator re-tabulated the whole file's numeral
alignment. Verified whitespace-only before believing it: `git show -w`
reduces the entire diff to the single added `#define MXXIV 1024`, and a
value census across the file counts **345 numerals before, 346 after,
with `MXXIV 1024` the only difference** — no existing macro's value
moved. The reformat is noise in the history, not a change in the
foundation header, and it rides in a commit about a resampler because
that is where it happened. Lesson worth the line: **do not hand
`latina.h` to the formatter as a matter of routine** — it is the
most-included header in the tree, a whole-file restyle buries the real
change, and the pre-commit formats whole files anyway.

**A fixture hazard that will bite the next person to edit `latina.h`.**
`probationes/probatio_git.c` compares `include/latina.h` **as committed at
HEAD** against the same file **on disk** — `git_massam_per_viam(repositorium,
caput, "include/latina.h", …)` versus `filum_legere_totum("include/latina.h",
…)`, at lines 225-240 and again at 288-297. So adding one line to that
header turns FOUR git assertions red until the change is committed, and
they fail on `massa.mensura == discus.mensura`, which points at git's blob
reading rather than at whoever edited the header.

The whole suite went 164/164 green to 164/165 the moment I touched
`latina.h`, and I was one step from hunting a deflate bug in a library I
had never opened. Any suite that pins a LIVE repository file against HEAD
behaves this way, and `latina.h` is the most-edited header in the tree —
so this will recur. Two notes on method: the diagnosis came from reading
what the assertions actually compare (ten lines) rather than from
bisecting; and I deliberately did NOT `git stash` to test the hypothesis,
because this tree also held a colleague's uncommitted work and a bare
stash would have swept it up with mine.

`MXXIV` (1024) was missing from `latina.h`, which has `DXII` 512 and
`MMMMXCVI` 4096 — added, since a numeral table spanning 4096 without
1024 is simply incomplete, and 1024 is the largest icon rendering.

## 2026-09-12 — `imago_extrahere_et_scalare` silently ran BILINEAR for `AREA`

**The defect.** Task 1 added `IMAGO_SCALA_AREA` to `imago_scalare`'s
`commutatio` and missed the second entry point. `imago_extrahere_et_scalare`
dispatched as `si (modus == IMAGO_SCALA_PROXIMUS) { … } alioquin
{ bilinear }`, so an `AREA` request fell into the bilinear arm with no
error. `icones` asks for `AREA` through exactly that function, so **every
icon icones produced was bilinear** — the whole resampler arc bypassed.

**How it was found — not by any gate this library had.** I2 tested
`imago_scalare` directly, and icones' gates I1–I6 never look at pixel
values. It surfaced when building icones' quality gate (I7) against a
frozen `sips -z 16 16` oracle: a scratch probe compared `sips`' 16 px with
our area, bilinear and nearest results, and icones' actual 16 px matched
the BILINEAR row on all twelve numbers (mean delta, four threshold counts,
per-channel maxima) while differing from direct `imago_scalare` AREA by a
mean of ~100. Identical measurements across a dozen independent columns
are a fingerprint, not a coincidence.

**The general lesson: appending an enum value means finding EVERY place
that enum is dispatched.** A `commutatio` without `ordinarius` would at
least let `-Wswitch` speak; an `if` / `alioquin` chain never warns, and its
final `alioquin` silently captures every value added later. "Enums append,
never reorder" protects existing callers of the enum — it does nothing for
the dispatch sites. `nexus` / legati `vocantes` on the SIBLING values
(`IMAGO_SCALA_PROXIMUS`) is how to find them.

**The fix: copy the region, call the proven function.** The new `AREA` arm
copies the crop rectangle into a piscina buffer (offsets in `s64`) and
calls the unchanged `_scalare_area`. Giving `_scalare_area` crop
parameters would have meant editing the function I2's exact constants pin;
copying adds no arithmetic that could be wrong. Cost: one crop-sized
allocation, `AREA` only.

**Gate I2 d — equivalence, with a fixture that can fail.** A 6×2 source,
crop `[1, 5) × [0, 2)` to 2×1: `extrahere(AREA)` must equal
`imago_scalare(AREA)` over a hand-copied crop, byte for byte, and is pinned
to hand-computed reds **98** ((20+30+180+160)/4 = 97.5, round half up) and
**88**. Both ratios (4→2, 2→1) are exact, so `extrahere`'s fit-within
truncation cannot move the dimensions. The block ALSO asserts that
bilinear gives something DIFFERENT on this fixture — if area and bilinear
agreed here, the equivalence would prove nothing (the fixture lesson of
I2 a, again).

- **Red before the fix, as predicted by hand:** the pins received **20 and
  50** — bilinear's corner-aligned samples of the crop's first row — plus
  the equivalence. 3 reds; the "bilinear differs" assertion green.
- **Plant:** `si (modus == IMAGO_SCALA_AREA)` → `IMAGO_SCALA_BILINEARIS`
  (equal tokens): exactly the same 3 reds, nothing else. 37 assertions.

**Still true and now named:** `extrahere`'s fit-within dimensions truncate
(e.g. a 1000 px crop asked for 16 yields 15), which is its documented
contract for its other caller (`lib/importatio_visus.c`, `PROXIMUS`, a
zoomed preview where one pixel does not matter). icones stops using this
function in its next commit and crops to an exact square instead.

**Left alone deliberately:** `-vitia` reports four banner lines of 60 `=`
at `:11`, `:13`, `:156`, `:158` (house width 50). `git blame`: `e288f0c59`,
2026-01-03 — eight months older than this change, which touches only the
new arm. Reformatting unrelated lines would widen a one-defect commit.

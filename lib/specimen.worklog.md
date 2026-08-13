# specimen worklog

## 2026-08-13 — birth (the regression layer)

Sits on imago_collatio + imago_png + manus. Fran chose the three
policies: goldens committed to git, element crop as the default scope
with whole-window available, and acceptance by renaming a file.

### The honest framing this was built under

Visual regression is the most machine-dependent test there is. Snapshot
output depends on OS version, **display scale factor** (a Retina
machine produces literally double-size images → instant
`MENSURAE_DISCREPANT`), installed fonts, and rasterizer version. And we
measured earlier the same day that a 0.3px text shift leaves 211
false-positive pixels *after* antialias suppression.

So the risk was never "can't build it". It was building a suite that
cries wolf until everyone ignores it — which by this house's standard is
worse than nothing, because it manufactures confidence. Two mitigations
are baked in rather than optional:

- **Element crop, not window.** Less area is less noise, and an
  unrelated change elsewhere can't break your button test.
- **Missing golden FAILS.** A test that quietly writes its own baseline
  on first run then passes forever has never compared anything. That is
  a lying-green generator, and it is the single most tempting shortcut
  in this whole layer because it feels friendly.

### Structure: the pure core

    specimen_iudicare(Imago* captura, titulus, regula, piscina)   /* PURE */
    specimen_manus  (Manus*, titulus, selector, regula, piscina)  /* capture+crop */

Same split that worked for manus: the judging core takes an
already-captured image, so the whole decision table is unit-testable
with synthetic images and no window. Only capture-and-crop needs a live
app, and that stays smoke-only.

### Decisions worth remembering

**Own crop, not `imago_extrahere_et_scalare`.** That one always runs the
resampler, and a bilinear pass even at 1:1 can perturb pixels. In golden
comparison that is precisely forbidden, so `_secare` is a plain
row-wise memcpy.

**Scale is derived, not assumed.** `getBoundingClientRect` returns CSS
pixels; the snapshot may be 2× on Retina. The factor comes from
`image_width / window.innerWidth` rather than `devicePixelRatio`, so
mixed-DPI and zoomed cases fall out correctly instead of being
special-cased wrongly.

**`MENSURAE_DISCREPANT` is its own verdict** and sets
`collatio_facta = FALSUM`. Different-sized images were never compared,
so reporting a diff count would be inventing one. Folding it into
DISCREPAT would cost hours hunting a regression that is actually just a
different monitor.

**`SPECIMEN_SCRINIUM_INACCESSUM`, not `_ABEST`.** Without POSIX `stat`
we cannot distinguish "directory missing" from "not writable", so the
name admits both rather than claiming the one we can't prove.

**Titulus is validated** — it becomes a path, so `/` and `..` are
refused rather than allowed to escape the golden directory.

**A passing run deletes stale artifacts.** Without it, a test you fixed
leaves `.novum.png` and `.differentia.png` in `git status` forever, and
the review queue stops meaning anything. The `.novum`/`.differentia`
files are deliberately NOT gitignored: while they exist they *are* your
list of failures awaiting eyes.

### Verification

35 assertions, all synthetic. Both critical gates proven by poison:

| poison | fracti | what it proves |
|---|---|---|
| missing golden auto-blesses + returns CONGRUIT | 3 | line 157 `CREDO_FALSUM(_exstat(golden))` catches auto-bless *independently of the verdict* |
| skip stale-artifact cleanup on success | 2 | lines 286-287 |

Note the second one: the verdict stayed CONGRUIT and the counts stayed
correct. Only the side-effect assertions caught it. Same asymmetry as
the `pixela_ferenda` poison — bugs that leave the *answer* right while
degrading everything around it are invisible to result-only tests.

### Not covered

`specimen_manus` has no automatic test — it needs a live window, so it
belongs in the by-hand smoke path alongside the rest of manus. Faking a
Manus would prove the fake works.

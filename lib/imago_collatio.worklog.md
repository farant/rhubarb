# imago_collatio worklog

## 2026-08-13 — birth

Built as step one of visual regression, deliberately *before* anything
that knows what a screenshot is. Fran's call, and it was the right one:
a diff that only ever answers "did this test pass" ends up with the
test's tolerances baked into the library's API, which is the one part
you can't refactor later.

### Why the boundary is where it is

`Imago` in → numbers + `Imago` out. **No filesystem, ever.** That falls
out of two facts discovered during the survey:

1. `Imago` already existed (Jan 2026) — RGBA8, arena-allocated. The
   input type was already sitting there.
2. There is **no PNG encoder in C** in this repo. Every PNG we write
   goes through AppKit (`NSBitmapImageFileTypePNG`, `vitrea_macos.m:871`
   and `tools/vitrea_spica.m:387`). Decoding is vendored stb_image
   (`vendor/stb_image.h`).

So a diff library that wanted to *write* its output would have had to
either grow an encoder or reach into the ObjC layer. Returning the
overlay as another `Imago` dodges both: whoever wants bytes on disk
hands it to the path that already exists.

The happy consequence is that the whole test suite is synthetic. No
window, no window server, no golden files, no `-vivum`. A test that
never touches the environment cannot go green *because of* the
environment — which is the failure mode that bit us repeatedly in the
manus work.

### The metric, and why the antialias clause is subtle

Per-pixel distance = **max absolute difference across R,G,B,A**. Chosen
over Euclidean/perceptual because `color.h` has only
`color_ad_cinereum` — Lab/CIEDE2000 would have been new work for a
benefit that tolerance-plus-antialiasing already covers.

Tolerance is **exceeded, not equalled**: `tolerantia = V` forgives a
delta of exactly 5. Section III of the probatio pins this both ways
(V congruunt / IV discrepant) because it is exactly the kind of
off-by-one that flips silently under a refactor.

Antialias detection is Kotsarenko & Ramos, the algorithm `pixelmatch`
uses. Two conditions, and the second is the one that's easy to get
wrong:

- the pixel has a **brighter and a darker** neighbour (it sits on an
  edge, not in a flat field), and
- the extreme neighbour has ≥3 identical siblings **in both images**.

That last clause is load-bearing. Check only the image you're scanning
and a *real* change that happens to land near an edge gets classified
as antialiasing and silently dropped. The whole point of the feature is
to suppress noise; if it also suppresses signal it is worse than not
having it, because it manufactures confidence.

Edges of the image count as one phantom identical sibling — a pixel in
the border row has fewer neighbours and would otherwise be judged
unfairly.

`delta_maximum` is deliberately **unfiltered**: max over every pixel,
counted or spurned or under tolerance. It is a measurement, not a
verdict, so it can tell you how much tolerance headroom is left before
the suite starts screaming. Section III asserts it survives a
CONGRUUNT verdict for exactly this reason.

### Verification: both poles, both directions

57 assertions. The two that carry the weight are a matched pair:

- **§VIII** — one pair of images, two rules, opposite verdicts
  (0 diversa / 8 leniata vs 8 diversa / 0 leniata). A feature you can't
  switch off hasn't been shown to be on.
- **§IX** — one black pixel in a flat white field, detection *enabled*,
  must still be reported.

Proven by planting faults rather than trusting green:

| poison | fracti | where |
|---|---|---|
| `_leniatum` → always FALSUM | 3 | §VIII 332–334 |
| `_leniatum` → always VERUM  | 5 | §IX 367–371 |

Reverted, back to 57/57. Note the expected values (8 leniata,
delta_maximum 127, arca 6×6) were derived by hand-walking the algorithm
*before* the first run — a passing number you predicted is worth much
more than one you read off and enshrined.

### Trap for later

`./compile_tests.sh imago_collatio` rebuilds the whole object world on a
cold tree and blows the 2-minute foreground cap. Background it. Also:
the background-task notification reports the exit of the *whole*
command, so a trailing `echo` launders a red suite into "exit code 0".
Append `EXIT=$?` to the output file and read it from there.

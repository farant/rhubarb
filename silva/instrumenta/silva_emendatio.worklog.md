# silva_emendatio worklog

## 2026-10-26 extracted from emendare.c for transformare T5

emendare.c held the only write guard (85 must vanish, no code grows, no
REICE). transformare -scribere needs the same rules with a different
target ("code 97 loses exactly N", N = edits applied), so the guard
moved here and emendare calls it with EMENDATIO_AD_NIHIL. The caller
passes its own ante-judgment (both callers already need it: emendare for
the include anchor, transformare for the plan).

Code 97 fires once per call site (checked on lib/specimen.c: 3 calls, 3
diagnostics), so "exactly N" is sound. A site absorbed by a <tolera>
would break it: the rewrite would not lower the count and the tolera
would go IRRITUM (another code grows) - refused, by design.

clang check: no stdin in processus, so the text goes to a temp dir
($TMPDIR/transformare_custodia_<k>, k counted up until mkdir succeeds -
mkdir is atomic, concurrent runs get distinct dirs; no pid needed) with
`-iquote <dir of the original>` so the file's own "..." includes resolve.
clang runs on the ORIGINAL first: a file that does not compile
standalone (missing -D, platform header) cannot be verified and is
refused, never written. Caught at once on a scratch fixture without a
prototype (-Wmissing-prototypes).

Test flags: a bare -std=c89 -pedantic -Werror rejects latina.h itself
(`long long` for i64) - the house adds -Wno-long-long. The tool takes
the real flags from transformare.sh (VEXILLA_C89 + derived -I roots
after `--`).

Plants: scope count, growth, post re-judge, clang exit code - all red.
REICE check NOT plantable: any VIOLATIO it would see also shows as a
grown code first. Kept as in emendare (belt and braces), noted here.
Planting the post re-judge goes red through undefined state (post
fields unset), not through the syntax assertion - still red.

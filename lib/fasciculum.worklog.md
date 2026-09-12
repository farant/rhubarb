
## 2026-09-11 — task 2: the merge was implemented twice, and the plan predicted its own blind spot

**The `plista_extra` merge had two mechanisms doing one job.** The plan
(`fasciculum-plan-1.md` task 2 step 4) had `_clavem_ponere` skip a
generated key when `plista_extra` already carried it, *and* a loop
afterwards that re-applied every `plista_extra` pair through
`plist_dictio_ponere` — which `include/plist.h:118` documents as
replacing an existing key IN PLACE. Either half alone makes the caller
win (D8). The skip only changed key *ordering*.

The tell was in the plan itself: step 6 warned that its second plant
"needs the loop removed as well, or it stays green — if it does, that is
the plan telling you the merge is asserted in only one direction; add the
missing assertion rather than weakening the plant." That diagnosis was
wrong in an instructive way. Nothing was under-asserted; the
implementation was redundant, and a gate cannot go red when two
independent mechanisms both enforce the thing you blinded.

**The loop is the load-bearing half, so the skip is the one that goes.**
Without the loop, a caller's *new* keys — `NSAppleScriptEnabled` in H2,
document types and Services declarations in real use — would never reach
the dict at all, because the generated-key list is fixed. Without the
skip, nothing is lost: generated keys go in first, the caller's pairs
replace them in place, order stays the generated order with extras
appended. One way to express the rule, and a plant on the loop now
reddens H2.

The general shape, worth keeping: **when a planted fault will not go red,
suspect redundant implementation before suspecting a missing
assertion.** Two code paths enforcing one rule look like defensive
programming and behave like a disabled gate.

**H1's entry count: the spec says four, the design yields two.**
`fasciculum-spec.md` §8 lists H1 as "four entries"; §3/§4 produce two for
a minimal bundle (`Contents/Info.plist` + `Contents/MacOS/<exe>`) and
three with an icon. Four was presumably counted when `PkgInfo` was still
in scope — §10 later defers it ("modern macOS ignores it"). The test
asserts two. Flagged for the spec as-built rather than silently
reconciled.

**What H1's purity assertion actually proves, which is less than it
looks.** `reddere` takes no root path, so it *cannot* create a bundle
even if it wanted to; `CREDO_FALSUM(filum_directorium_existit(
"Contents"))` only shows nothing relative appeared in the working
directory. The property the spec really claims — reads no file
CONTENTS — is not observable from outside the function, and no assertion
in H1 tests it. Recorded so nobody later mistakes a green H1 for proof of
purity. What does hold it honest: `pars->fons` is asserted to be
pointer-identical to the caller's path, so a 20 MB binary provably never
passed through the arena.

**Ten puts, one invariant.** The plan ignored every
`plist_dictio_ponere` return value, which loses a key silently if the
arena ever fails. Checking all ten would be ten blocks saying the same
thing. Instead the required keys are confirmed present once, after the
dict is built — any failed put lands there regardless of which one it
was.

**Refusals name the key, not the empty value.** The plan set
`sedes_vitii` to `f->identitas` when refusing for a missing identity —
i.e. to the empty chorda it had just rejected. It now names
`"CFBundleIdentifier"` / `"CFBundleExecutable"`, which is what §3 means
by a refusal that names its subject, and what H3 will assert in task 4.
`status` and `sedes_vitii` are also validated before being written
through; the plan wrote `*status` before checking the pointer.

**Seam worth naming (task 2): plist has no dict-iteration accessor.** The merge
loop reaches into `PlistValor.liberi` with `xar_obtinere` because there
is no `plist_dictio_par(dictio, i)`. The struct is public in `plist.h`,
so this is legal rather than a hack, but one library walking another's
`Xar` is the kind of coupling that ages badly — a candidate desideratum
for plist, not a blocker here.

## 2026-09-11 (task 4) — two defects in one comparison, and why only one test caught one

`fasciculum_legere` (the inverse of §4) and `fasciculum_aequalis`, gates
H3 and H5; 76 → 132 assertions.

**`fasciculum_aequalis` shipped two defects into its first green build,
and the second is the instructive one.**

1. `_nomina_aequalia` compared FULL PATHS while its own name and comment
   said it compared names. `/tmp/probatio_fasciculum_binarium` never
   equals `<area>/Contents/MacOS/probatio_fasciculum_binarium`, so H5's
   round trip went red immediately. `via_nomen` was not available as a
   fix: it allocates, and `aequalis` takes NO piscina — the same
   no-allocation constraint I had already solved for the default
   literals with `chorda_aequalis_literis` and then failed to carry
   across. A `strrchr`-based basename helper needs no arena at all.
2. **`titulus` was never compared.** I wrote a comment explaining that
   titulus must be compared AFTER the names because its default derives
   from the executable's basename — and then did not write the
   comparison. Two bundles differing only in `CFBundleName` compared
   equal.

**No test caught the second, and the reason generalises.** H5's
dissimilitude case varied exactly one field, `identitas` — which was one
of the fields I did compare. **A test that varies one field proves that
one field is compared.** It says nothing about the others, and a green
result from it feels like evidence about the whole comparison. The block
now varies every compared field in turn (identitas, titulus, versio,
versio_aedificationis, sine_scandali, executable name, icon name,
plista_extra), one `CREDO_FALSUM` each. That widening is also what made
the plan's named plant meaningful: blinding the `plist_aequalis`
delegation reddens exactly the `plista_extra` case and leaves the other
seven green, which is the evidence that each case exercises a different
branch instead of all funnelling through one.

**Effective equality, decided rather than inherited.** `reddere`
defaults `versio` to "1.0" and `versio_aedificationis` to "1" and writes
them into the plist, so a minimal value written and read back differs
from itself in those fields. A literal comparison would report
"different" for a bundle byte-identical to what we would write — which
defeats the documented purpose of the function ("ut scriptor fasciculum
iam rectum non rescribat"). So `aequalis` compares EFFECTIVE values: an
empty field equals its default. The defaults live in one place
(`FASCICULUM_*_ORDINARIA`) used by both `reddere` and `aequalis`, so the
property holds by construction rather than by two sites agreeing. NIHIL
`plista_extra` equals an empty dict for the same reason — `plist_numerus`
tolerates NIHIL while `plist_aequalis` returns FALSUM for it, so the
zero-pair case is settled before delegating.

**A spec requirement the approved API cannot carry.** §6 says
`FASCICULUM_ERROR_PLISTA` "carries plist's own status and semita rather
than flattening them", but the four signatures §3 fixes have nowhere to
put a `PlistStatus`. What travels is `Contents/Info.plist` joined with
plist's element path, so the refusal names both the file and the
offending element; the numeric plist status is genuinely lost. Named in
the code, in H3's comment and here rather than quietly satisfied by a
weaker reading.

**`<string.h>` and a rule about include hygiene.** Task 2 deliberately
dropped it because `reddere` used nothing from it. Correct then; wrong
the moment the reader arrived, and the build failed with `rc=2` —
NOTHING RAN — on implicit `strcmp` and `memset` under `-Werror`.
"Include only what you use" has to be re-asked when a file grows a
second function, not decided once at birth.

**Asymmetry recorded, not accidental:** `legere` confirms the executable
named by `CFBundleExecutable` exists, and does NOT require the icon to
exist. §4 says to confirm the executable and is silent on the icon, and
§11's AUDIENDUM already records that we never validate an `.icns`. A
bundle with a missing executable is the failure that cannot be
diagnosed from the outside; a missing icon renders blank.

Also fixed in passing: `_clavis_levata` used a `character` loop counter
with a `(memoriae_index)` cast on the array index — the cast only
existed because the type was wrong. It is an `i32` now and the cast is
gone.

## 2026-09-11 (task 3) — a plant that stayed green found an unasserted branch

`fasciculum_scribere`: per plan entry, create the parent, write bytes
(GENITUM) or copy the source (COPIATUM), then set 0755 where
`exsecutabile_fiat`. Gates H4, H6 and a new H6b; 68 → 76 assertions.

**The plan's writer could not have worked**, and the reason was one
call: it created parents with `filum_directorium_creare_si_necesse`,
which is a single `mkdir`. `Contents/MacOS` is two levels below a virgin
scratch root, so it fails ENOENT. Fixed in the layer that owned the gap
(`filum_directorium_creare_cum_parentibus`, commit `35e57a23`) rather
than by walking the path here — see `lib/filum.worklog.md` for that
finding, including the discovery that `filum_delere` is `remove()` and
so already removes empty directories.

**A PLANT THAT STAYS GREEN NAMES A COVERAGE HOLE, NOT A BAD PLANT.**
Blinding the GENITUM failure path (`redde VERUM` instead of refusing
when `filum_scribere` fails) left the gate green, and `silva.planta`
refused it: `porta cum planta VIRIDIS - porta muta`. The cause was not
the plant. H6 makes `Contents/MacOS` unwritable, so `Contents` itself is
open, the `Info.plist` write genuinely SUCCEEDS, and only the copy
fails — H6 exercises the COPIATUM branch and nothing in the suite ever
made a generated write fail. The GENITUM refusal was unasserted and
would have shipped that way.

The fix was the missing assertion, not a weaker plant (the rule plan
task 2 step 6 states, applied where it actually bit). **H6b**: a second
scratch area where `Contents` EXISTS but is 0500 — so no directory needs
creating and the only possible refusal is the generated write failing.
Re-planted, it now reddens exactly three assertions: the `CREDO_FALSUM`
on the call, the specific `FASCICULUM_ERROR_SCRIPTIO`, and
`sedes_vitii` naming `Contents/Info.plist`. H6's COPIATUM assertions
stayed green throughout, which is the evidence that the two branches are
asserted INDEPENDENTLY rather than one covering for the other.

Worth keeping as a shape: **two failure branches of one function look
like one path from outside it.** Both refuse with the same status and
the same kind of `sedes_vitii`, so a test that reaches either one
appears to cover "the write failed". Only a plant per branch tells you
which one you actually reach.

Two smaller facts about the tooling, both nearly cost me an hour:

- **`silva.planta` reverts the file even when it RAISES** on a green
  gate. Verified rather than assumed — `FASCICULUM_ERROR_SCRIPTIO` back
  to two occurrences, both `redde VERUM;` accounted for as function
  tails. Committing a planted "half-written bundles report success"
  would have been the worst defect this arc could ship.
- **`build/portae/radix.planta_rubra.acta` is STALE after a green
  plant.** A green run does not overwrite it, so reading it then shows
  the PREVIOUS plant's reds. I read plant 1's three reds while
  investigating plant 2's silence and briefly took them for plant 2's.

**Gate corrections to the plan's H4/H6.** H4 now writes a bundle WITH an
icon, covering spec §8's "icon placed" where that row lives instead of
leaning on H5 in task 4, and pins the three-entry plan. It also asserts
the mode is SELECTIVE — the plist and the icon are NOT executable —
which plant 1 confirmed by reddening only the executable's assertion out
of three. Both H4 and H6 tear down deepest-first at BOTH ends, which
only became possible once `filum_delere` was known to remove
directories: with a fixed `/tmp` area and no way to remove one, "the
writer created these directories" is true on a virgin machine and passes
for the wrong reason ever after. H6 also restores 0500 → 0755 BEFORE its
teardown as well as after, because a run that died holding 0500 makes
the deletes themselves fail and poisons the next run.

**`sedes_vitii` carries the RELATIVE path, not the absolute one.** The
plan passed the joined absolute `directorium` for
`FASCICULUM_ERROR_DIRECTORIUM`. H6 only asserts that `sedes` CONTAINS
`"Contents/MacOS"`, which an absolute path satisfies too, so the
distinction would have gone untested either way — but a refusal should
name the path inside the bundle, consistently with every other refusal
here, rather than leaking the scratch root it happened to be written
under. All three failure modes now use the entry's own `semita`.

**Glossary: `contents` and `macos` as `ignotum-permissum`.** Apple's
bundle directories are fixed foreign names; Latinizing them would record
a fiction and renaming the identifiers would stop the test reading
against the spec's layout. Same exit as `plist` and `nanoid`. Noted in
passing: the sibling buffer `via_res` escaped the lint only because
`res` happens to be a Latin word.

### Later the same day — the calibration, and three tool lessons

**Both plants landed where they should.** Plant 1 (`exsecutabile_fiat`
VERUM → FALSUM) reddened exactly one assertion, H1's
`CREDO_VERUM(pars->exsecutabile_fiat)` — H2 never reads that field, so
the tightness confirms the coupling was read correctly. Plant 2 (the
merge loop blinded) reddened THREE: `v->textus == "BNDL"` (the caller
overriding a generated key), `CREDO_NON_NIHIL(v)` and
`v != NIHIL && v->veritas` (the caller's own new key). So H2 asserts
both halves of D8, and the plan's prediction that this plant might stay
green was a symptom of the redundant implementation, not of a missing
assertion.

**An assertion that crashes is worse than one that fails.** Plant 2
would have segfaulted H2 before it could report: `CREDO_NON_NIHIL(v)`
records its failure and CONTINUES, so the next line's `v->veritas`
dereferenced NIHIL and truncated the rest of the compendium. Folding the
NIHIL test into the assertion itself (`v != NIHIL && v->veritas`) is not
a conditional wrapped AROUND an assertion — it always executes and
always reports — and the plant then printed a clean red for the very
line that would have crashed. Only the key a plant actually removes
needed this; the other derefs were left alone rather than gold-plated
on speculation.

**`vocabula -nova` cannot see untracked files, and said NOVA 0 anyway.**
The identifier lint counts git-TRACKED `.c`/`.h` only. All three files
here were new, so the first run reported NOVA 0 — which meant "no words
examined", not "no new words". `git add -N` (intent-to-add, which
`git ls-files` reports) made them visible and the true answer appeared:
NOVA 3, rc=1 — exactly what would have blocked the commit, as it blocked
three of six commits in plist's arc. Check this lint with the files in
the index or the check is theatre.

**`CONFORMIS` from the formator is not a claim that the file is right.**
Rewrapping an over-long comment via
`silva.Editio.replace(..., tolerans='verba')` produced a `/*` opener at
16 columns against continuation lines at 9: the prose-tolerant mode
anchors on the first prose WORD, so the leading whitespace included in
the replacement was ADDED to the indentation already there. The formatter
reported CONFORMIS anyway — it judges line length and banners, not a
comment's opening column. Fixed with `tolerans=False` (exact octets).
Two rules worth keeping: with `tolerans='verba'` never put leading
whitespace in the replacement, and a clean formatter report means no
rule fired, not that the result reads correctly.

**Two glossary entries, on two different precedents.** The lint named
`plista`, `exsecutabilis` and `exsecutabile` — all real Latin, so exit
(a), an entry each. `plist` was already in the glossary from yesterday's
arc as `ignotum-permissum`, and the reflex was to file `plista` beside
it. The house actually distinguishes a BARE borrowing (`plist`, `json`,
`nanoid` → `ignotum-permissum`) from one given a Latin shape so it can
decline: `ranga`, English "range" in the first declension, is
`latina/substantivum` with a full paradigm. `plista` is the second kind.
`exsecutabilis`/`exsecutabile` are one adjective in two forms rather
than two lemmas, and `attingibilis` is the same `-bilis, -e` formation
to copy. Recorded in the entry instead of harmonised away: `filum` calls
this notion `exsequibile` in its comments while the identifiers say
`exsecutabile` — two words for one thing, noted on purpose. Judged by
`canon_examen` (VITIA 0) and the glossarium gate; identifier unknowns
2950 → 2947.

# herbarium worklog

## 2026-10-08 - first implementation (vates-plan-2 T2)

Spec: project-specs/herbarium-spec.md; header approved by Fran (7b5ca52b).

**Decisions inside the header's freedom**
- A variant is new when its MASKED body differs from every stored
  variant of the same kind: digit runs -> `#`, runs of `[A-Za-z0-9_-]`
  of length >= 12 containing a digit -> `@` (ids, request ids). So
  "limit 50 reached" and "limit 99 reached" are ONE variant; "slow down
  please" is a second one.
- Default header allowlist: content-type, retry-after, request-id,
  x-request-id (compared lowercased via chorda_minuscula), plus the
  caller's `capita_admissa`. set-cookie and everything else: dropped.
- Request headers and request body are NEVER written; only method,
  host, path and the caller-named top-level body fields
  (`campi_petitionis`, e.g. "model") go into `summarium`.
- Each press uses its own temporary piscina (destroyed at the end) - a
  long-lived herbarium must not grow its arena per sighting.
- Specimen files are pretty JSON (people read them at promotion);
  the sighting index is compact JSONL appended with
  `filum_appendere_firmiter` (T1) - counts and last-seen are derived
  from it, no file is ever rewritten.
- `herbarium_vectura(NIHIL, x)` returns `x`; `herbarium_premere(NIHIL, ...)`
  is a no-op. Capture failure (unwritable directory) is reported ONCE on
  stderr and never touches the response.

**Plants** (probatio_herbarium, 50 + 2 assertions): header always
admitted -> SECRETUM_COOKIE red; press below threshold -> counts red;
index append skipped -> sightings red; replay status+1 -> red.
**Variants by raw body first stayed GREEN**: with a variant limit of II
both raw and masked comparison keep TWO files, so a file count cannot
tell which bodies were kept. Added assertions that variant II is "slow
down please" and "limit 99" is absent; the plant is now red. Lesson
(again): a count-only assertion forgives the wrong contents.

**Lint renames**: in_cifris->in_numeris, _sceleton->_sceletum_scribere
(the approved header's `sceleti` proves the oracle knows sceletus),
summ->summarium_petitionis, probatio_sceleton->probatio_sceleti,
probatio_redditio->probatio_reddere.

## 2026-10-09 - the guard before use, herbarium side (herbarium-spec-2 H1)

- **Judge.** `HerbariumOptiones.iudex` is called in `_exsequi_capiens`
  for successful responses BELOW `status_minimus`, after the inner
  transport returns and BEFORE the wrapper returns - a flagged response
  is on disk before the consumer sees a byte. Responses at or above the
  threshold are pressed as "status" exactly as before and never reach the
  judge (asserted: call count II of III). `iudex` NIHIL = old behaviour.
  herbarium stays generic: it knows no JSON and no norma; the consumer
  brings the judge. The test checks the specimen count INSIDE the request
  loop, right after the first (flagged) response returns.
- **Default location** `herbarium_sedes_ordinaria(hospes)`: an explicit
  `$RHUBARB_HERBARIUM` must name an existing directory (refused with a
  named cause otherwise - a typo never silently captures into $HOME);
  else `$HOME/.rhubarb/herbarium`; then `/<hospes>`. Empty host, '/' or
  '..' refused. Not created here (`herbarium_aperire` creates with
  parents). Pattern taken from main's gesta/fontes/annales_sedes.c.
- Tests save and restore `HOME` and `RHUBARB_HERBARIUM` around the
  location cases (setenv/unsetenv, POSIX via postulata_posix.h).
- Plants: judge never called -> four assertions red; missing env
  directory falling through to $HOME -> two red.

## 2026-10-09 - kind key: order-independent, arrays as a union of shapes

Found by the deliberate shape capture (tools/specimina_colligere): the
skeleton summarized an array by its FIRST element only, so every 200
reply whose `content` began with a plain text block collapsed into one
kind - and with variantes_maximae III the citations and max_tokens replies
were counted but not stored. Fran: "arrays keyed by the union type of
their elements ... not order dependent ... if one of the elements is a
shape we've never seen before that's a legitimate outlier."

Now `_sceletum` returns the skeleton as a string: object keys sorted
(`{a,b}` == `{b,a}`), arrays = the distinct element skeletons, sorted,
joined by '|' (`[{id:s,input:{},type:s}|{text:s,type:s}]`). Element order
and repetition do not change the kind; a new element shape anywhere does.
Kinds of piles captured before this change no longer line up with new
captures (file names are hashes of the key) - nothing depends on that.

The capture tool keeps variantes_maximae XXXII (deliberate capture wants
every example); the always-on outlier capture keeps III (a disk bound per
kind; the number of kinds is unbounded and every sighting is counted).
Plants (red): first element only; object keys unsorted.

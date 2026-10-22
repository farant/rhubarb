# mixtura worklog

## 2026-10-20 — S2: the offline mixer (sexta)

Spec `project-specs/sonus-spec.md` §IV, §VI. Fran approved the header
("yup! that looks great"): two halves (Mixtura = audio side, Rector =
main side), flat fixed-size commands, frame-stamped commands, ramps in
frames, buses named `grex` with `SUMMA` as master, reports as the only
way back.

**Render = segments between command times.** `mixtura_reddere` applies
every command whose `momentum <= now`, finds the next pending command
inside the block, and renders voices frame by frame up to it. A command
at frame f therefore acts BEFORE frame f is rendered - sample-accurate.
Pending commands are kept sorted by insertion that is STABLE (same
momentum = arrival order; the suite pins "the later master-gain wins").
A command in the past acts at the start of the next segment and counts
as late (`iussa_tarda`), measured against the block start.

**Danger noted while planting:** applying only `momentum < now` (not
`<=`) would leave a command whose time equals `now` pending forever and
render zero-length segments in a loop. The plant list deliberately
avoided that mutation; the `<=` is load-bearing.

**Start gain:** `clivus > 0` on a start = fade in from silence; 0 = full
at once (keeps the transient - a percussive buffer must not be
softened). Stop with `clivus 0` ends at once (FINIS_IUSSU); with a ramp it
fades and reports when the ramp reaches 0.

**Pan = constant power** (cos/sin of (p+1)pi/4): centre = 0.70710677
per side, so a centred mono voice is -3 dB per channel. Pan gains are
recomputed only while the pan ramp moves.

**Mute keeps time**: a muted bus's voices still advance (the suite
starts a 100-frame buffer under a muted bus, unmutes at 50, hears
[50, 100), and gets VOX_FINITA at 100). Master mute = `SUMMA.mutus` =
spec §X.3's master mute.

**Voices may not target SUMMA** - it is the sum of the buses, not a
bus a voice plays into; a start naming it is `irrita`.

**Rector**: free slot first; else steal lowest priority, oldest on a
tie, only if its priority <= the new one. Generation 0 is reserved for
"no handle" (wraps skip it). A SURREPTA report for the old handle does
not free the slot the new handle now holds (generation mismatch).

**Census on request** (`CENSUM_PETERE` -> report CENSUS): the main side
never reads mixer state, so even counters come through the report
queue. `apex` (peak of the master) resets at each census.

**Sonitus voice in the mixer == sonitus_reddere x pan**, byte for byte
(the suite compares exactly: gains of 1.0 multiply exactly).

**Plants 2026-10-20 (13 via silva.planta, all red then green, source
byte-identical).** Two lessons:
- *First green, then a survivor.* The suite passed on the first
  implementation run. Plant 1 ("commands one frame early",
  `<= now + 1`) SURVIVED: segments always begin AT a command's moment,
  so an early-by-one command only shows when two commands sit on
  ADJACENT frames. Added a test (hard-left voice at 300, hard-right at
  301: frame 300 must be left only) - plant red.
- *Stable insertion was untested* until the plant list was written; the
  "later same-moment command wins" test went in BEFORE the batch and
  caught "insertio instabilis" (`>` -> `>=`).
- One plant first failed to COMPILE (unused `momentum` under -Werror);
  planta's syntax pre-check refused it without touching the tree; rerun
  with `(vacuum)momentum;`.
Red list: early commands, late not counted, stale handle accepted,
linear pan, gain without ramp, bus mute ignored, master mute ignored,
silent steal, priority ignored, rector ignores generation, loop
ignored, lost reports not counted, unstable insertion.
- Lint before commit: `MIXTURA_FINIS_SISTITA` -> `MIXTURA_FINIS_IUSSU`
  ("by command", ablative of iussum): *sistita* is not Latin (sisto's
  participle is *status*). The only rename after header approval.

## 2026-10-22 - S5a: voice kind TIBIAE

Third voice kind: MIXTURA_IUSSUM_TIBIAM (tibia params + note, gate open),
NOTAM_PONERE (note with `clivus` frames of glide), PORTAM_CLAUDERE
(release; the voice ends with FINIS_NATURALIS when the envelope rests).
New enum values appended AFTER CENSUM_PETERE so existing values keep
their numbers. NOTAM_PONERE / PORTAM_CLAUDERE on a stale handle or a non-
tibia voice count as `iussa_irrita` (`_tibia_ansae`). SISTERE and
VOCEM_PONERE work on tibia voices unchanged. The suite checks the mixer
output bit-equals a direct tibia render x MEDIUM, including a pitch SET
at frame 8000 (commands act BEFORE their frame is rendered). Plants 5/5.

## 2026-10-22 - S5b: rector vivit / dimittere / occupatae

For the shared host (concentus): `vivit` = slot occupied with the same
generation; `dimittere` frees a slot whose start was refused (only if the
handle is current - a stale handle must not kill the slot's new voice);
`occupatae` counts occupied slots (the host's quiet test).

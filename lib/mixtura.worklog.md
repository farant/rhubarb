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

## 2026-10-24 - reverb send bus, missio, cauda

Voices x missio (slid like gain/pan) are summed mono into one shared SonusReverberatio,
whose stereo output is added to the master sum BEFORE the master gain, so master mute
silences the tail but a group mute does not (the tail lives in the shared room). The
return level (lucrum_reverberationis, set by REVERBERATIONEM_PONERE with a slide)
defaults to 0, and at 0 the reverb isn't processed at all. So every existing render is
byte-identical, and the state freezes until the level comes back.

mixtura_cauda_vivit: quadra_tacita counts samples whose wet output (after the return
level) stays under 1.6e-5 (-96 dB); alive while that count is under 100 ms. It starts
at the limit (not alive). concentus_agere counts a live tail as alive, which is what
makes the auditio WAV export, auscultatio and live output keep the tail and then close
on their own.

mixtura_iussum_parare didn't zero tibia/nota (older) and now zeroes the reverb form
explicitly (aula 0).

## 2026-10-24 - master limiter (headroom, desideratum ...WH53)

A zero-latency, stereo-linked peak limiter after the master gain, before output. It
covers everything on the mixer (songs, theremin, notification sounds, WAV export, live
output). The peak a = max(|l|,|r|). If a x current gain > ceiling, the gain drops AT
that sample (no lookahead, so no delay, and no sample moves), then recovers with a
120 ms time constant. Both channels get the same gain. The output is clamped to the
ceiling as well, against rounding a hair above it.

Default ceiling 1.0 (0 dBFS), not the 0.97 first proposed: full scale is legal (three
existing tests produce exactly 1.0, e.g. a unit voice panned hard left). Below the
ceiling the gain is exactly 1, so every render that never went over is byte-identical.
MIXTURA_IUSSUM_LIMITATOREM_PONERE (lucrum = ceiling, 0 = off).

BUG FOUND BY THE RECOVERY TEST: storing the GAIN and recovering with g = 1 - (1-g)*c
STALLS in f32 near 1. When (1-g)(1-c) < half an ulp of 1.0 (~3e-8), rounding returns
the same g forever, about 0.99983 here (the mix left ~0.0015 dB quiet for good). The
state is now the reduction x = 1 - g, decayed x *= c, snapped to 0 under 1e-7. A
shrinking small number never stalls.

Census: apex = mix peak BEFORE the limiter (the truth: "this would have clipped"),
limitata = samples where the limiter actually cut the gain (overs; recovery samples NOT
counted - the first version counted them and reported 374794 for a song with 27 overs),
reductio = lowest gain since the previous census. The "fragor" preset peaks above 1.0;
probatio_mixtura determinismus switches the limiter off (its apex == output-peak check
is about determinism).

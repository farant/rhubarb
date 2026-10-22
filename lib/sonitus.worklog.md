# sonitus worklog

## 2026-10-20 — S1b: the sfxr voice on sonus (sexta)

Spec `project-specs/sonus-spec.md` (S1). Fran approved the header shape
("that looks good!"): one voice path, all-f32 knobs + an offsetof
schema, solarium's 20 knobs with Latin names, solarium's 8 presets
ported byte-for-byte with Latin names (blip punctum, step passus, whoosh
susurrus, thump fragor, wind ventus, laser radius, crackle crepitus,
water rivus). Added after approval: `SonitusVox.finis` (counted end).

**The voice IS the renderer.** `sonitus_reddere` = prepare a voice, pull
until `finita`. The suite pins that a hand-pulled voice equals
`sonitus_reddere` sample for sample, so S2's mixer (which will pull the
same struct inside the callback) cannot drift from the offline path.

**End is COUNTED, not detected.** `finita` = `exemplum >= finis` with
`finis = (impetus + mora + remissio) x fs`, the same function
`sonitus_longitudo` uses. Waiting for the envelope to reach QUIESCIT
would let the float release slope add or drop a sample, and then
`sonitus_longitudo` would lie about the buffer size.

**Seconds -> samples must ROUND.** First green run had one red:
`mora 0.01` gave 479 samples. `0.01f` is 0.00999999977, so
`(i32)(0.01f * 48000)` truncates to 479. Same lesson as sonus's phase
increment; `_exempla` rounds now.

**Seed streams - what the plant taught.** Variation rolls use
`sors_seminare(semen, series I)`; the noise oscillator owns its OWN
`Sors` inside `SonusUnda`, seeded series 0. I first wrote that the
disjoint series is why variation cannot shift the noise - wrong: the
separate Sors instance is why. The probe plant (variation on series 0)
stayed GREEN, correctly: the only effect of sharing the series is that
the three rolls equal the first three noise values (a correlation, not
an audible change). Kept series I for decorrelation; no test pins it.
Three rolls are ALWAYS drawn (solarium's rule) so the stream position
never depends on `varietas`.

**Plants 2026-10-20:** 11 via silva.planta, 10 red then green (punch,
jump, slide sign, variation ignored, end +1, seam weights, schema
offset, preset lookup, missing clamp, truncated seconds); 1 probe
survived as explained above. Source byte-identical after.

**Envelope mapping.** sfxr's attack/sustain/punch/decay = sonus ADSR with
lapsus 0, tenor 1, gate closed at `finis_morae`; punch is a separate
gain `1 + ictus x (1 - progress through mora)`. The suite checks the
three points (0.75 / 0.625 / 0.5 for ictus 0.5, amplitudo 0.5).

**Pitch numbers are analytic**: slide +1 oct/s from 440 for 1 s =
440/ln 2 x (2 - 1) = 634.78 cycles; -1 oct/s = 317.39; jump x2 at 0.5 s
= 220 + 440; vibrato 1 semitone at 5 Hz averages ~440.4.

**Known deviation from solarium**: noise RNG is sors (PCG32) not the LCG,
and phase is the 32-bit integer accumulator - presets sound the same
family but are not sample-identical to solarium's WAVs.

## 2026-10-22 - preset `tonus` (ours, not solarium's)

None of the eight solarium presets can carry a melody: `punctum` jumps
up a fifth after 50 ms (`saltus 1.5`), `radius` sweeps down (`clivus -9`),
the rest are noise. `tonus` is the house voice for `cantus` (S4): 50%
pulse, 3 ms attack (no click), 60 ms release, and FIXED pitch (no slide,
jump, vibrato or `varietas`), so the note that sounds is the note that was
written. Its `mora` 0.10 only matters when it plays alone; `cantus` with
`tenere` overwrites it with the written length. The suite asserts the
fixed-pitch fields, not the timbre values, which stay free to retune
by ear. Fran chose a preset here over a colour local to `auditio`, so
`auditio`, vicus and a jingle Claude writes can all say `@tonus`.

# cantor worklog

## 2026-10-22 - S6c: the sequencer

Plays a Carmen through the shared host (concentus), reading the LIVE
model every pulse - nothing precompiled, so an edit while playing is heard
on the next pass (section vivum). Position is in ticks; frames come from
the song's tempo map anchored where playback started:
F(p) = basis_quadrum + Q(p) - Q(basis_pulsus).

Each pulse schedules the notes whose frame falls before
concentus_initium + CANTOR_PROSPECTUS (250 ms): TIBIAM at the exact start
frame, PORTAM_CLAUDERE at the end frame (the mixer queues future commands);
an '@preset' note is a one-shot SONITUM. Gain = note x track; mute skips;
any solo silences the rest. Priority II (jingles are I).

Loop: when scheduling reaches the region end, basis_quadrum moves to
F(finis) and basis_pulsus back to the region start - the second pass's
frames come from the same map, exactly one region later (section regio).
CANTOR_ITERATIONES (XVI) caps wraps per pulse: a degenerate region (one
64th at 400 bpm = 450 frames) would otherwise wrap ~27 times per 250 ms;
the rest continues on the next pulse with no gap (section brevis).

Stop / seek: commands already sent cannot be withdrawn, so the cantor
remembers its voices (handle, start, end): a sounding voice gets SISTERE
now with a 5 ms fade; a not-yet-started one gets SISTERE AT ITS OWN START
FRAME with clivus 0 (queue insertion is stable: TIBIAM then SISTERE at the
same frame -> the voice ends before rendering a sample).

Plants: one plant ("basis not advanced") made cantor_agere loop forever -
a test binary hung 51 minutes before I noticed. That is what pulled the
wrap cap in; with it the same plant goes red in seconds. A hand-written
watchdog to kill hung tests misparsed ps etime ("00:00" read as hours) and
killed everything at once - stopped; that run's results were discarded.
Two plants survived the first tests: stop sending SISTERE at "now" for
future voices (the test sampled only the LAST block - now a peak over the
whole window) and never forgetting finished voices (the effect appears
only after > CANTOR_VOCES notes - the short-loop test now runs 3 s, ~320
notes). Final: 16/16 red.

## 2026-10-22 - S6d step 1: public default resolver

cantor_tibia_ordinaria / cantor_sonitus_ordinarius are the former
interior _tibia / _sonitus bodies, made public with the CantorResolutor
field signatures so a user-patch resolver (vicus, later) can fall back to
them for names it does not know. cantor_creare now FILLS any NIHIL field
with them, so the call sites call c->resolutor.X directly (no fallback
branch). Section 'ordinarius': known / unknown / prefix chord without a
terminator ("quadransX" mensura 8) / empty / 200-byte name; a resolver
built from the two functions plays like NIHIL. Plants 4/4 red (prefix
ignored, tibia field not filled, sonitus returns NIHIL, call site
bypasses the resolver).

## 2026-10-24 - room on start, pan and send per note

cantor_incipere always sends REVERBERATIONEM_PONERE with the song's form and return
level, 0 when the song has none. On the shared mixer that silences the previous song's
room instead of letting it linger. Each note carries pars->positio and
pars->reverberatio (missio).

## 2026-10-24 - dynamica: note-on gain + streamed voice-gain ramps (desideratum ...16EVW)

Note-on lucrum = nota x pars x carmen_intensio(at onset). Held tibia voices follow the
dynamics with MIXTURA_IUSSUM_VOCEM_PONERE: one segment per eighth (CANTOR_GRADUS_INTENSIONIS
= 480 pulsus) and at every dynamics start/end, each a linear clivus (frames between the
two points) to the value just before the next point; a <dynamica> STEP starting at the
point first jumps with CANTOR_EVANESCENTIA. Positio and missio of the part are re-sent in
every VOCEM_PONERE (the command ramps all three - leaving them 0 would pan/dry the voice).

Why STREAMED (only points inside the prospectus window each tick, after the window's
notes so new voices get theirs): the mixer queue is bounded (auscultatio forma.iussa =
256). Pre-scheduling every segment of a long held chord at note-on would overflow it and
drop commands silently (census iussa_amissa).

First run bug: "is this a step?" was decided by comparing the gain one pulse earlier -
inside a smooth ramp that differs by ~1e-4, so every eighth emitted an extra jump. A step
is structural: a <dynamica> without usque starts exactly here (_gradus_ad).

CantorVox gained sequitur/pars/basis/pulsus_initium/pulsus_finis/transitus; transitus =
basis_quadrum of the pass that started the voice - after a loop wrap, a voice still
ringing from the previous pass must not take points of the new pass. Sonitus (one-shot
percussion) voices do not follow.

End to end (probatio_auscultatio 'dynamica'): a held c1 pp -> ff rendered: end/start peak
ratio > 2.5x that of the same note without dynamics. Demo waltz: per-bar rms -24.5 dBFS
(bars 1-8) rising to -17.9 (bar 13), falling after.

Plants 18/18 red; one first attempt did not compile (stubbing the streaming call left the helper unused - rewritten as an empty window). Three tests were added BEFORE plants after reading the plant list: two steps at one position refused; VOCEM_PONERE keeps positio/missio; loop wrap guard (transitus).

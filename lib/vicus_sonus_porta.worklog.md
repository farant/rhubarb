# vicus_sonus_porta worklog

## 2026-10-22 - the real sound port (sonus S4 step 4b)

Thin glue: `cantuale_ordinarium` + `tintinnabulum_creare` whose
`TintinnabuliExitus` wraps auditus; time = `fenestra_tempus_ms`.

The one non-obvious part: `auditus_aperire` allocates its two rings
(iussa, nuntii; LXIV each - >= one jingle's 32 notes plus SISTERE for all,
and >= 2 x voices) from the piscina it is given, and tintinnabulum CLOSES
the device after TINTINNABULUM_QUIES (30 s) and reopens on the next
jingle. Passing vicus's long-lived piscina would grow it on every reopen,
for as long as vicus runs. So `exitum_aperire` creates a small dynamic
piscina per open and `exitum_claudere` destroys it after
`auditus_claudere` has waited out the audio thread.

No unit test (it would play through the speakers during gates); the
logic above it is covered by probatio_tintinnabulum (offline mixtura)
and probatio_vicus_sonus (fake port). Live check = Fran's ears.

## 2026-10-22 - S5b: the port builds a concentus

The port now creates one concentus (PORTA_VOCES = XLVIII: a full jingle
plus room for live voices) over the auditus adapter and the tintinnabulum
on top; `agere` = concentus_agere then tintinnabulum_agere (messages first,
then waiting jingles). Ring capacities went 64 -> 128: auditus wants
nuntii >= 2 x voices, and the voices grew to 48 (powers of two required).

## 2026-10-22 - S5c: live voices (theremin)

`vocem_incipere(tibia, vibratus, nota, lucrum)`: tibia preset by name
(unknown -> aether; vibratus off zeroes the preset's depth), captured at
priority II so jingles (I) never steal it, MUSICA bus. The mixer handle is
packed into the i64 (sedes << 32 | generatio; generatio 0 = "no voice").
`vocem_ponere` sends NOTAM_PONERE + VOCEM_PONERE with the same glide in
frames. Up to IV live voices are remembered; `tacere` (master mute) closes
their gates first, then the tintinnabulum's quiesce closes the device
after the releases. PortaSoni is now memset - piscina_allocare memory is
not zeroed, and garbage handles could have read as live. No unit test
(real CoreAudio); Fran's ears.

## 2026-11-04 — gabc part 2 P5: carmen in the port; CoreAudio split out

Four OPTIONAL slots in VicusPortaSoni (carmen_incipere / quaerere /
sistere / pulsus); old fakes leave them NIHIL and
vicus_applicatio_soni_portam_ponere keeps validating only the old ones.
The Cantor is created lazily on the port's concentus at the first
carmen_incipere; porta_agere calls cantor_agere after concentus_agere;
tacere (mute) stops it. `carmen_incipere` is also the G18 swap: cantor
holds only the carmen pointer and its voices store part indices, so the
old carmen may be freed the moment it returns (probatio_cantor
"permutatio": old carmen poisoned + piscina destroyed, ASan via
tools/sonda.sh clean).

**Own "playing" flag.** cantor_sonat stays VERUM for a moment after
cantor_sistere while voices fade, so carmen_pulsus = -1 unless the port's
flag (set by incipere, cleared by sistere/tacere) AND cantor_sonat
(natural end) both hold.

**Split.** Testing the real port on a fake exitus pulled
concentus_auditus -> auditus_macos.m into the test archive, and the test
link carries no AudioToolbox — EVERY test stopped linking. So this file
now builds the port over any ConcentusExitus
(vicus_sonus_porta_cum_exitu) and knows no CoreAudio; the CoreAudio
constructor `vicus_sonus_porta()` moved to lib/vicus_sonus_auditus.c
(include/vicus_sonus_auditus.h), which only apps/vicus/vicus.c and
vicus_terminalis.c include. Forma constants VICUS_SONUS_VOCES/ANULI are
shared through the header. `Carmen` got a struct tag so vicus_sonus.h
can name `structura Carmen` without including carmen.h.

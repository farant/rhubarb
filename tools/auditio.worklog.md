# auditio worklog

## 2026-10-22 - S6d step 3: -carmen / -carmina

Songs play through concentus + cantor exactly as vicus will drive them:
a 10 ms pulse, concentus_agere then cantor_agere, done when the cantor no
longer sounds AND concentus_agere reports no live voice (release tails
included - the natural end fades out, no cut). Live: the exitus is
concentus_auditus (step 2). WAV: an in-file exitus keeps the mixtura from
aperire, momentum = mixtura_momentum, each pulse renders 480 frames -
same horizon logic with simulated time; Korobeiniki renders 26.8 s in
0.08 s, and two renders are byte-identical. Buffer = song + 2 s tail.

-iterare (live only; refused with -via, exit 2): Ctrl-C sets a
sig_atomic_t flag, the loop calls cantor_sistere (5 ms fade) and keeps
pulsing until the voices finish - no click (Fran: no artifacts).
-pars P sets solus on that track. Instruments are checked up front with
cantor_tibia_ordinaria so an unknown one is an error, not silence.

The bar trace ("transitus N  mensura m/M", one line per change, readable
by agents) printed "17/16" at the very end: cantor_pulsus at the end
equals the song length = start of a bar that does not exist - clamped.

Fran listened 10-22: "it sounds very good! bass/melody is excellent and
tempo and notes sound great too"; loop works, no artifacts, the normal
end fades out.

## 2026-10-22 - M1/M4: -partitura, -mensurae, -liber

-partitura prints partitura.h text (no sound); there -pars filters the
score instead of soloing playback (_ex_carminibus takes 'solum').
-liber reads an outside book with filum_legere_totum (never fopen,
examen 96) and judges it like the embedded one; the error line now
carries song / track / clip / offset (offset was missing before - an
agent writing a book needs it). Validated combinations exit 2.

## 2026-10-23 - M2b: -censura, -regulae

-censura prints one line per finding, then "censura: N inventa
(regulae: ...)"; exit 0 clean, 3 findings (1 errors, 2 usage kept).
-regulae "+a,-b" edits the defaults, "a,b" (no signs) means exactly
these; an unknown rule is a usage error (exit 2) naming the five.

## 2026-10-23 - M3: -auscultare; WAV via auscultatio

-via now renders through auscultatio_reddere (the in-file exitus is
gone); _carmen_sonare is live-only. -auscultare prints the whole-song
line (length, peak, clipping, RMS, dBFS), one line per track rendered
solo, one per bar, TACITA where notes should sound; exit 3 on clipping
or a TACITA bar.

## 2026-10-23 - -recensio [-json], -partitura -harmonia

-recensio prints recensio_carminis_textus (or -json: the pretty JSON for
MCP); -regulae / -partitura / -harmonia feed the report; exit 3 when
monita > 0. -partitura -harmonia adds the chord row (harmonia.h) to the
plain score too.

## 2026-10-23 - -carmina spells ranges like recensio (M5a)

`-carmina` now uses partitura_ambitus / partitura_notam_scribere / partitura_clavis_littera; auditio's own `_nota_titulus` (sharps only) and `_clavis_titulus` (a copy of partitura's key table) are gone. Visible changes: stella's melody `Bb4..D6` (was `A#4..D6`); note counts exclude '@' sounds (no book song has any); an empty part prints `-` (was `-..-`). `_carmen_monstrare` takes the caller's piscina for the range string.

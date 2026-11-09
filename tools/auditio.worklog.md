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

## 2026-10-23 - unknown -regulae name lists rules from censura (M5b)

The 'regula ignota' message hard-coded the rule names (it would have missed 'tertia'); it now loops censura_titulus over the enum.

## 2026-10-23 - house tool declaration (M5c, wish …PQ3XN)

auditio is now declared like praeco: answers `-provenientia` (provenientia_auditio, plagula build/fabrica/provenientia/auditio.c via the aedilis obiectum annotation), built by `tools/auditio_struere.sh` (praeco_struere.sh with names swapped: aedilis -> provenientia_scribere -> struere -> rm+cp into bin/auditio), actions `auditio` + `institutio_auditio` in aedificatio.stml, both in the `installata` composite. In sexta only bin/auditio is built (`bin/fabrica sanare bin/auditio`, RECENS); `~/.bin/auditio` is judged 'artificium absens' until main's `sanare installata` after the merge - the same state ~/.bin/praeco is in from any worktree. Use from then on: `auditio -carmen T -recensio [-partitura -harmonia] [-json]`, `-liber x.stml` for a scratch song file.

## 2026-10-23 - E3: -mensura / -ponere / -substituere [-scribere] (wish …NJ7K)

Thin layer over emendatio.h. `-mensura P M` prints the bar text and its START state
(o l v @ - `l` written with partitura_valorem_scribere, MML lengths are always plain
values): the korobeiniki trap (bar 1 contains 'o5 l8', so new text starts in o4) is
visible before anyone writes. `-ponere P M:T NOTA` / `-substituere P M MML` print
vetus / novum (the exact text swapped) and ante / post (the part's partitura line for
that bar, last line of a one-bar one-part partitura_scribere). DRY RUN by default;
`-scribere` needs `-liber` (the embedded book has no file to write; for carmina.stml the
tool reminds to run tools/carmina_infigere.sh). Exit 0 / 1 refused (reason + frustum +
offset) / 2 usage. `_vitium_carminis` shared with _ex_carminibus, and it fflush(stdout)s
first - piped, stderr otherwise overtook the header line.

Verified on a scratch copy: substituere bassus 2 -> ponere 2:3 A2 -> substituere back
-> file identical to carmina.stml byte for byte. No fumus gate exists for auditio; the
logic and its plants live in probatio_emendatio / probatio_cantus.

## 2026-10-23 - -partitura -explicatio (N1)

Line from explicatio.h instead of the per-beat harmonia line (PartituraForma has ONE extra
line slot, so -harmonia and -explicatio are exclusive - usage error with both). The
Explicatio is computed once in _partituram_scribere and passed as additae_ctx.

## 2026-10-23 - -partitura -functio (#3 F2)

Lines stack now (PartituraAddita list): -harmonia -explicatio -functio in that order, any
combination (the old "harmonia OR explicatio" usage error is gone). One Explicatio is
computed for both explicatio and functio lines.

## 2026-10-23 - -copiare (palette #4)

`-copiare P M[-N] [Q:]K [+-g]`: the interval is an optional 4th argument recognised by a
leading digit or sign+digit (so `-1` is the interval, not a flag); `-semitonia` and
`-repetere R` are only legal with -copiare. Output: header (source -> target, interval,
repetitions) then 'ante' / 'post' partitura line per destination bar; dry run unless
-scribere (with -liber).

## 2026-10-23 - book instruments in auditio

_instrumenta_probare accepts carmen_instrumentum before the built-ins; -carmina prints
"instrumenta: cembalo (chorda), ..." under the parts. Playback / WAV / -auscultare need
nothing: cantor resolves book instruments itself.

## 2026-10-24 - '-carmina' names genus partiales

The `instrumenta:` line in -carmina had its own chorda/modulatio/unda ternary, so a book
instrument with genus="partiales" printed "(unda)". The demo book showed it, but no test
covers it. Fixed inline. The genus-word table now exists twice (lib/carmen.c GENERA,
here) - a tibia_generis_titulus() in tibia.h would make one home (proposal to Fran,
header change).

## 2026-10-24 - genus formantes in -carmina

One more branch in the instrumenta line (TIBIA_FORMANTES -> "formantes"). The duplicated genus-word table is still pending Fran (see the entry above).

## 2026-10-24 - -instrumenta; -carmina leaves the instrumentarium out

-instrumenta prints the built-in presets (genus, censura range table) and the house
instrumentarium (genus, its own ambitus). -carmina's instrumenta line only lists song and
book instruments now (numerus_instrumentorum - numerus_instrumentarii). Without that,
every song would print the five house instruments. Genus words come from
tibia_generis_titulus.

## 2026-10-24 - -auscultare names the limiter

auscultatio_reddere_censum; the summary line adds ", limitator N quadra (-x.x dB; mixtura +y.y dBFS)" when the master limiter engaged. The un-hand-balanced vespera demo: mix +2.1 dBFS, 27 overs, WAV at full scale exactly, no clipping.

## 2026-11-02 — `-musicxml -sonare` and the file-name title

To hear a converted score you had to pass `-carmen <title>`, and you
had to know the title first. A score with no work or movement title
came out as "musicxml" (the converter's fallback; the library never
sees the path). Now:

- When `-titulus` is not given and the score has neither a work nor a
  movement title, auditio passes the file name without its directory
  or last extension as `forma.titulus`
  (`musica/fontes/nata_lux_80317.mxl` → `nata_lux_80317`). The
  converter normalizes it like any other title. A score that has a
  title keeps it (the Brahms lied stays `9_lieder_and_songs_op_32`).
- `-sonare` turns off "convert only" and plays the converted song,
  like `-carmen`. Exit 2 without `-musicxml`, or with `-via` or an
  analysis mode (`-censura`, `-partitura`, `-recensio`, `-auscultare`,
  `-tonus`).

Testing note: a stub `afplay` on PATH only catches `-via`-less renders
of SOUNDS (`auditio radius`). Playing a SONG (`-carmen`, `-sonare`)
goes through cantor straight to the audio device, so a stub does not
silence it. A silent check for the song path still needs a way to
stop playback (none yet).

## 2026-11-04 — `-gabc` (gabc plan T8) and the first auditio smoke test

`-gabc VIA [-do P] [-strictus] [-titulus T]` behaves exactly like
`-musicxml`: alone it prints the book on stdout and the converter's report
on stderr (exit 3 with approximations, 1 on a parse error or a strict
refusal); with `-sonare`, `-carmen`, `-partitura` … the converted book is
the source. `-do` takes a note name through `partitura_notam_legere`
("F4", "Bb3") and sets `do_fixum`; without it the highest note lands on
F4. A chant with no `name:` header (and no `-titulus`) takes the file name.
`-gabc` with `-musicxml` → exit 2; `-do` without `-gabc` → exit 2.

Shared, not copied: the tail both converters had (report to stderr, wrap in
`<carmina>`, re-read, title, exit code) is `_conversionem_perficere`;
`_musicxml_convertere` now calls it too. The validation checks use one
`conversum` (= the -musicxml or -gabc path).

No suite covered `-musicxml` (it was checked by hand, see 2026-11-02), so
there was nothing to mirror: `tools/auditio_fumus.sh` (gate
`auditio-fumus`, `instrumentum="bin/auditio"`) is new. 17 checks: book
re-read through `-liber … -carmina`, `-partitura` title line, usage exits,
`-do B3`/`Bb3` first note, approximation exit 3, strict exit 1 with empty
stdout, `-titulus`, file-name title, parse error, and `-musicxml nata_lux`
through the shared tail. It does not play sound (no way to silence cantor).

Plant by hand (planta could not name the gate while the script was
untracked): `-do` ignored → the two `-do` checks FRACTUM; restored with
cp + touch, rebuilt, green.

**Same day, follow-up:** the gate was declared in the script but not
registered: commissio and planta only know gates listed in `PORTAE`
(`pythonica/silva.py`), so the T8 commit never ran it (I ran it by hand).
Now registered. Proving it through planta exposed two traps: (1) the gate
used whatever `bin/auditio` was there, so a source plant stayed green (mute
gate) -> the script now builds from source first (~7 s); (2) after planta's
restore the planted object survived, because aedilis' generated builder
judges by `-nt` and /bin/bash 3.2 compares whole seconds (restore landed in
the same second as the planted compile) -> the script removes the objects of
the files it covers before building. planta: red on the two `-do` checks,
green after restore. Ledger parcum 01M5XD21FEJW523NBK03VWJKVB (ictus).

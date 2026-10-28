# harmonia worklog

## 2026-10-23 - chord line (ledger wish ...7CVF, from composing 'vesperae')

Names what SOUNDS: the 12-bit pitch-class set at a tick (tracks not muted,
solo respected, no percussion) is rotated to each candidate root and
matched EXACTLY against 15 templates; the bass root wins when it fits
(Csus2 vs Gsus4, aug, dim7), else the lowest class. Sixths left out (Fran
10-23): C E G A is always Am7 (Am7/C with C in the bass). Opt-in row.

Layering: harmonia needs partitura's spelling and the score needs the
chord row - a library cycle. Instead partitura got a generic extra-row
hook (PartituraForma.additae_titulus / addita / additae_ctx) and
partitura_valorem_scribere went public so the row uses the score's own
value grammar (D:2 G:2); harmonia_lineam_scribere fits the hook.

Two notation findings while using it: power chords first printed as
'A5' / 'D5/A', which read exactly like NOTE names in the same grammar -
all dyads now in parentheses: A(5), D(3)/F#, Em(3). And my own test
voicing "C/E" (E5 G4 E3) was really {E, G}: the tool said Em(3), the test
was wrong - same lesson as the parallel fifths, the tool reads what is
there. Two-voice songs (vesperae, Korobeiniki) mostly give dyads; implied
harmony is a different, opinionated problem - not attempted.

Plants: two cases added beforehand because I could see they would survive
- out-of-template sets listed "from the bass up" coincide with plain order
when the bass is C (test with bass E), and in 6/8 sampling every eighth
vs every dotted quarter is hidden by merging unless a passing note sits
mid-beat (F5 on the second eighth).

## 2026-10-23 - harmonia_sine_tertia (M5b)

Pure predicate for censura 'tertia' (and later the DAW pane): classes match {0}, {0,7}, {0,7,10} or {0,7,11} from some root. Each shape has a unique root (no rotation of one matches another), so no bass tie-break is needed. sus2/sus4 deliberately NOT matched - the chord name already says the third was replaced. Dropped a `classes & _bit(r)` root-presence check: every shape contains 0, so an absent root can never match - a plant there was an equivalent mutant. The `& 0xFFF` mask is NOT redundant (F + bit 13 rotated by 5 gives {0,8}, a false negative) and has a test.

## 2026-10-23 - interior `_tempus` removed

Uses carmen_tempus_mensurae (carmen.h) - the rule is shared with censura and emendatio.

## 2026-10-24 - chord names spelled as the composer wrote (desideratum ...FP9CJ)

_classem_scribere used to spell a pitch class with NO letter, so for out-of-scale classes
partitura_orthographia fell back to "flat key -> flat": C#+E in D minor printed Dbm(3),
though the cembalo wrote c#. Harmonia now carries litterae[XII] (the written letter per
class, first one wins; ex_classibus clears it). harmonia_ad_pulsum and explicatio's
windows record the letters of the notes they used (harmonia_litteram_notare), and
_classem_scribere passes the letter into partitura_orthographia, which honours a
written letter. A written d- stays Db.

Test lesson: C# in D minor is ALSO the leading tone, so the new partitura fallback alone
already spells it right - dropping the letters would still pass the C#m test. The
G#+B in D minor case (G# is not the leading tone; the key rule says Ab) is the one that
proves the letters arrive. The explicatio fixture for implied Bb7 in Bb major wrote its
bass g# (copied from the A minor case); with letters winning it read Bb7/G#, so the
fixture now writes a- and keeps "Bb7/Ab".

Six house songs: analysis output (-harmonia -explicatio -functio) byte-identical before
and after - none had a wrong spelling (completorium's Dbm bar was written out in the bass
rewrite).

## 2026-10-26 - pure chord half moved to acordum (Q7G5 step 1)

The chord table, acordum_ex_classibus and acordum_sine_tertia now live in lib/acordum.c
(include/acordum.h), and the types are renamed Harmonia -> Acordum, HarmoniaGenus ->
AcordumGenus, HARMONIA_* -> ACORDUM_*. Reason: the accompaniment generator (comitatus)
expands while the book is READ, inside carmen.c - it needs a chord-name parser, and
harmonia depends on carmen (ad_pulsum, orthography by key). The pure half had to sit
below carmen. harmonia keeps "what sounds in a song": ad_pulsum, scribere (suffix via
acordum_suffixum), lineam_scribere, litteram_notare.

Renames went through silva/build/renominare directly: the renominare.sh wrapper refuses
files with uncommitted changes (git = undo), and a run of 21 renames over the same 9 files
dirties them after the first. Backups were in build/plantae_s1/orig. Identifiers inside
CREDO_* macro arguments and in comments are "manuales" - swapped by hand with word-bounded
perl (BSD sed has no \b). explicatio.c had its own local struct named Acordum (window
chord: classes + septima); it became AcordumFenestrae first, or the rename collides.

The test sections exemplaria / radices / sine_tertia moved to probatio_acordum.c with
their helpers; probatio_harmonia keeps nomina, sonantes, linea, orthographia (4/4).

## 2026-10-27 - step 2c: the 'acorda' line (declared harmony as a lead sheet)

D03TB part 2. harmonia_acorda_lineam_scribere prints the declared chords (Carmen.acorda) per bar
as written ("C#m:2 B7:2" - author's spelling, "-:2 A:2" when a declared span starts mid-bar,
'r' for a declared rest, '-' outside any span), next to the literal 'harmonia' line, so a
disagreement between what was declared and what sounds is visible at a glance (proelium bar
16: acorda "B7sus4:2 B7:2", harmonia "E(5)/B:2 B(3):2"). An explicit nullum branch was redundant
- a declared 'r' is an empty Acordum and harmonia_scribere already writes "r" (plant survived,
branch removed). auditio: `-partitura -acorda` (line order acorda, harmonia, explicatio,
functio = PARTITURA_ADDITAE IV, all four fit). Plants 4/4.

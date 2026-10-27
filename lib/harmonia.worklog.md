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

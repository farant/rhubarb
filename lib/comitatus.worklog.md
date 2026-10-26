# comitatus worklog

## 2026-10-26 - born (Q7G5 step 2): figures of accompaniment

Pure library below carmen: reads a chord progression (comitatus_acorda_legere, bars
given by the caller as start pulses), reads a figure (comitatus_figuram_legere), and
realizes notes (comitatus_componere). carmen.c turns <acorda>/<figura>/<comitatus> into
a generated CarmenFrustum. Header text approved by Fran; one correction told to Fran
before writing: 'Q' is the chord's OWN fifth (dim: diminished, aug: augmented), root + 7
only when the chord has no fifth - otherwise Bdim's alternating bass lands on F#.

Design notes:
- the note-length grammar is cantus's (divisor of the whole 1..64, up to two dots with
  divisibility), copied as _valorem_legere because cantus keeps it interior.
- voicing: close position, every inversion x every octave whose used voices fit the
  range; cost = sum of |moves| of the voices the figure uses (common count with the
  previous voicing), first chord = mean nearest the range centre; ties go lower. A range
  >= 12 always admits a voicing: start on the first chord tone at or above the floor -
  the k tones from there span 12 - (gap below that tone), so the top stays under
  floor + 12.
- the voicing changes ONLY when the chord changes (content compare: genus, radix,
  bassus, classes) - repeated "C | C" bars keep one voicing and alternate B/Q.
- figure bar index restarts when the chord at a bar start differs from the chord
  sounding just before it (pulse - 1), so "C:2 G:2 | G" puts Q on bar 2.
- 'Q' must differ from the bass note (C/G: other octave); if the range has no other
  octave it falls back to the bass itself (documented in the header).

Hand-computed waltz ("C | C | G7 | C", 55..76 / 36..57): C 60 64 67 over C3, Q = G2,
G7 -> D F G (62 65 67, cost 3 beats B D F at 5), C back to 60 64 67. All 9 sections
passed on the first run; 10 plants red (cost ignores previous, ties upper, figure never
restarts, Q not avoiding bass, roll without stagger, remainder not to last, overflowing
values accepted, gain not reset per figure bar, Q = root+7 always, letters not from the
chord).

Lint renames before commit: comitatus_realizare -> comitatus_componere (realizare is not
Latin; componere = put together), local fm -> mensura_figurae, inversio -> prima (real
Latin but unknown to the oracle - a gloss would move oratio's pins, a rename is free),
test helpers _valsa / VALSA_FIGURA / _sectio_valsa -> _liber_ternarius / FIGURA_TERNARIA /
_sectio_saltatio. Glossary: acordorum (gen. pl.) added to the existing acordum entry.
Locals need renominare -intra <function>; names inside CREDO_* arguments are "manuales"
(perl \b swap).

Observed in an auditio run (scratch waltz C C G7 G7 Dm7 G7 C C): with V = 3 voices on a
four-note chord the voicing uses the three LOWEST tones of the chosen close position, so
minimal motion from C E G picks D F G for G7 - the leading tone B is dropped. Standard
practice omits the FIFTH first. Raised with Fran as a follow-up (header says voices
1..min(V, k) of the close position).

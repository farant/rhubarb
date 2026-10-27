# acordum worklog

## 2026-10-26 - born from harmonia; acordum_legere (Q7G5 step 1)

What a chord IS, with no Carmen: the 15-entry exemplar table (moved verbatim from
harmonia.c), naming from pitch classes (acordum_ex_classibus, acordum_sine_tertia) and
the new direction, name -> classes (acordum_legere). One table serves both directions,
so new vocabulary (sixths, ninths - item 43HA) is added once.

acordum_legere decisions (header text approved by Fran):
- suffix = LONGEST match over the table, ties to table order. "Cmaj7" must not read as
  "m" + "aj7"; "7" reads the FIRST seventh exemplar (four classes), not the fifthless one.
- slash bass may lie outside the classes ("C/D" = passing D, explicatio's own spelling);
  classes stay the exemplar only, so a generated upper voicing never contains it.
- letters by letter-steps per interval (2:+1, 3/4:+2, 5:+3, 6/7/8:+4, 9/10/11:+6): Bbm7b5
  spells Fb as 'f', Bdim7 spells Ab, Caug spells G#. Stored lowercase like
  CarmenNota.littera.
- root letter must be uppercase: 'b' is the flat sign, so "Bb" vs "b..." stays
  unambiguous; no suffix starts with 'b' or '#'.
- on FALSUM the output is untouched (the struct is built locally, copied at the end).

Lint trap: the first draft named the parameter `nomen` (= typedef macro); examen REICE.
It is `titulus`.

Circuitus test: every named genus x every root, spelled with acordum_suffixum, must read
back to the same genus/root AND acordum_ex_classibus of the read classes (bass = root)
must agree - this ties the two directions of the table together.

## 2026-10-26 - chord properties live in the table (43HA step 1)

AcordumProprietates per genus: suffixum, numeralis (functio quality), tertia, quinta,
septima (intervals, -1 none), gradus (explicatio naming rank). The SUFFIXA array folded
into it - one row per genus, CLASSIS_UNA/IGNOTA/NULLA included; out-of-range genus ->
the NULLA row. Callers that used ENUM ORDER (explicatio _gradus_generis `g <
SUSPENSA_IV`, functio `g >= SEPTIMA && g <= DIMINUTA_SEPTIMA`, `genus <=
DIMINUTA_SEPTIMA` for inversion figures) and the per-genus switches (explicatio
_septima, functio quality strings, the six-genus lowercase list, comitatus _quintam)
now read the row. Equivalences used: lowercase numeral <=> tertia == III; inversion
figure <=> gradus == III; seventh figure <=> septima >= 0.

Proof of no behavior change: auditio -partitura -harmonia -explicatio -functio for all
seven house songs byte-identical before/after. New test 'proprietates' ties every row
to its exemplar: the third/fifth/seventh interval of each row must be a class of the
chord read from "C" + its suffix. functio's explicit role lists (dominant forms,
cadence-final forms, picardy) stay code - step 2 extends them for the new genera.

## 2026-10-26 - 43HA step 2: sixths, ninths, 7sus4, 7#5

New rows: 6, m6, add9, 9 (and 9 without fifth), 7b9 (and without fifth), maj9, m9,
7sus4, 7#5. Exemplar rows now carry their own LETTER STEPS (`litterae[VI]`) - the old
shared GRADUS_LITTERARUM table could not spell a sixth (+5) next to a seventh (+6), or
a ninth (+1). Bbm6 -> G, C7b9 -> Db, F#7#5 -> C##.

`radix_basso` rows match only when root == bass: 6 = m7 and m6 = m7b5 as pitch sets, so
C E G A is C6 over C and Am7 over A/E/G. 7sus4 joined them during implementation
(deviation from the approved header): without it G C D F over C read "G7sus4/C" and
absorbed a neighbour F and passing D (explicatio test 'C:1 (F5 vic., D5 tr.)').

`additae` (PROPRIETATES): interval bits of tones that are ADDED (sixth, ninth, flat
ninth; and - second deviation - the fourth of 7sus4 and the raised fifth of 7#5). Used by
explicatio's rule (see its worklog). Test 'proprietates' asserts additae is a subset of
the row's classes.

Plants 11/11 red (in three files); the 9-row additae plant survived until explicatio got a
passing-D-over-C7 case, and the 7#5 plant's first anchor ended inside a comment (planta
refuses - anchor before the comment). PROPRIETATES comments moved to their own lines
(new rows ran past 72).

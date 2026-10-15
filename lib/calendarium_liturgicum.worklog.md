# calendarium_liturgicum worklog

## 2026-09-12 — Ordinary Time after Pentecost was always "week I"

Found by building `project-specs/exempla/kalendarium.thistle`, which puts
the calendar on screen. Its sample answer for 14 September 2026 said
`"hebdomada": 1` and psalter week 1. That Monday is in week XXIV.

**Measured before touching anything**, eleven dates through the app's own
method: before Lent the weeks were right (12 Jan I, 10 Feb V, 17 Feb VI).
Every date after Pentecost read "Hebdomada I" — 25 May, 14 June,
12 and 13 September, Christ the King on 22 November, 9 June 2025,
17 May 2027 — and so did the psalter week, which derives from it.

**Cause: two errors, and a clamp that hid both.** In
`calendarium_tempus_info`, case `TEMPUS_PER_ANNUM_II`:

1. It counted back from `calendarium_anchorae(cal, dies.annus + I)`
   — the first Sunday of Advent of the NEXT year, about 440 days away.
   The result went far below zero.
2. Even with the right year, `XXXIV - (days + VI) / VII` is one short:
   13 September would have been XXIII, Christ the King XXXIII.
3. `si (info.hebdomada < I) info.hebdomada = I;` turned the impossible
   negative into a plausible "I". A clamp that converts a value which
   cannot happen into one that looks normal is an opt-out of its own
   error report; without it the page would have shown nothing, or
   nonsense, on the first day after Pentecost.

The two comments above the old code said it plainly: "Hebdomada post
Pentecosten est ~34" and "Calculus complexus - simplificamus".

**The rule.** After Pentecost the weeks are counted BACKWARDS: the week
that begins with Christ the King (the Sunday before Advent I) is XXXIV.
So for a date whose week begins on Sunday S,
`hebdomada = XXXV - (Advent I of the same year - S) / VII`. Advent of the
SAME civil year: Ordinary Time after Pentecost always falls before it.
No clamp.

**Why nothing caught it.** The suite asked which SEASON a date is in,
never which week. 124 assertions, zero on `hebdomada`.

**Gates (124 → 164 assertions).**

- Anchors 2026 (Easter, Ascension, Sacred Heart, Baptism, Christ the
  King, Advent I) and Advent 2023, the edge where Christmas Eve is itself
  a Sunday and so is the fourth Sunday of Advent.
- Weeks across every season, each date's answer known without this
  library: Ordinary Time I (12 and 18 January, 17 February 2026), Lent I
  and Easter II with their titles, Advent IV 2025 and Advent I 2026, the
  last Saturday before Advent (XXXIV), the Monday after Pentecost in
  three years (2024 VII, 2025 X, 2026 VIII), 12 and 13 September 2026
  (XXIII and XXIV, with titles "Sabbatum, Hebdomada XXIII" and "Dominica
  XXIV per Annum"), Christ the King 2025 and 2026 (XXXIV). Psalter weeks
  at 17 February (II), 25 May (IV), 13 September (IV), 22 November (II).

Before the fix: **13 reds, predicted 13** — every Ordinary-Time-II
assertion except the season check for 28 November, which was already
right.

**Calibration, three plants, all predicted and matched:**

| Plant | Predicted | Observed |
|---|---|---|
| P1 Advent of the next year again | 13 | 13 |
| P2 `XXXIV` for `XXXV` (one short) | 13 | 13 |
| P3 no step back to the week's Sunday | 7 | 7 — weekdays only; every Sunday stays green |

P3 is the informative one: the Sunday assertions alone would pass a
formula that forgets weekdays belong to the week their Sunday began.

**Known and not fixed here** (the example shows or may show them):

- The Sunday and weekday reading cycles are computed from the civil
  year, so from Advent I to 31 December they are one year behind; the
  suite's own Advent I 2024 assertion expects Year B where Year C began.
  Its own commit, next.
- `computare_anchorae` fixes Epiphany on 6 January, but when that day is
  a Sunday it moves the Baptism of the Lord to Monday 7 January — the
  rule for countries that transfer Epiphany to Sunday, applied to a
  calendar that does not. Universally, Baptism would be Sunday 13
  January (2030, 2036).
- The title of a day never names a movable solemnity: Christ the King
  reads "Dominica XXXIV per Annum", Palm Sunday "Dominica VI
  Quadragesimae". Only a saint's feast replaces the title.
- Christmas season weeks are all I.

## 2026-09-12 — The reading cycles followed the civil year

`calendarium_obtinere_diem` passed `dies.annus` to
`calendarium_cyclus_dominicalis` and `calendarium_cyclus_quotidianus`.
The liturgical year begins on the First Sunday of Advent and bears the
number of the civil year in which it ends, so from Advent I to 31
December every date got the previous year's cycles: Advent I 2024 read
Sunday Year B where Year C began that day, and weekday Year II where
Year I began.

**The suite held the error as a fact.** Its Advent I 2024 check
asserted `CYCLUS_B`. A test written from the same assumption as the
code agrees with the code. It now asserts C, with a comment saying what
it used to say and why it changed.

**Fix.** `annus_liturgicus` is the civil year, plus one from Advent I of
that year onward; both cycle functions receive it. They still take a
year, and the header now says it is the liturgical year.

**Gates (164 → 173).** Advent I 2024: C and I (the corrected assertion
and its weekday twin). 28 November 2026, the last day of liturgical
2026: A and II — the positive twin on the near side of the boundary.
29 November 2026: B and I. 31 December 2024: C and I. 6 January 2025:
C and I. Born red: 6, predicted 6.

| Plant | Predicted | Observed |
|---|---|---|
| CP1 no `+ I` | 6 | 6 |
| CP2 `>` for `>=` (Advent I itself left in the old year) | 4 | 4 — only the two Advent I days; 31 December stays green |

## 2026-10-15 - liturgia-plan L0: guards, sections, F9

Plan: `project-specs/liturgia-plan.md` (from the 10-13 review
`project-specs/liturgia-recensio/RECENSIO.md`; US calendar). L0 first
converts `probatio_calendarium.c` to credo II sections (16) and adds
guards that pass today: Easter vs the published table 2000-2040, an
independent computus (Oudin 1940, `_pascha_alterum`) 1583-4099 with
0 mismatches, the extremes (1818/2285 Mar 22, 1943/2038 Apr 25,
1981/2076 Apr 19), anchor relations for 2008/2011/2038/2285, and the
review's "pass (guard)" rows as a `CasusDiei` table (principal gradus,
colour, short title for saints; temporal titles unchecked because L2
renames them).

F9: `calendarium_finis_temporis(TEMPUS_PER_ANNUM_II, N)` used
`calendarium_anchorae(cal, N + 1)`. The season API takes the
LITURGICAL year N; Ordinary Time II of year N ends the day before
Advent I of CIVIL year N (that Sunday starts year N + 1). The new
`limites_temporum` section checks finis(t) + 1 == initium(t + 1) for
every season of 2025/2026/2038, including PA_II(N) -> Advent(N + 1);
only that last transition was wrong (2026: 2027-11-27 instead of
2026-11-28). Plant `annus + I` back -> red.

## 2026-10-15 - liturgia-plan L1: precedence by OrdoTabulae

The old model had a private `PRAECED_*` scale that only decided whether
a sanctoral entry was INCLUDED, and then chose the principal by raw
`gradus` - where `GRADUS_DOMINICA` (V) > `GRADUS_SOLLEMNITAS` (IV), so
every Sunday swallowed every solemnity (RECENSIO F1), All Souls fell
through an unreachable branch (F2), and memorials beat privileged
weekdays because those were graded FERIA (F7, F13).

New: public `OrdoTabulae` (UNLY 59 rows 1-13, smaller = stronger) on
every `Celebratio` and every `SanctoraleDatum` (227 entries, scripted:
rule from gradus + exceptions Christmas/Epiphany II, four feasts of the
Lord V, Guadalupe VIII; All Souls gets `GRADUS_DEFUNCTORUM` and III).
`gradus` is now only the display kind. One day = temporal entry
(always first) + one winning solemnity/feast (ordo strictly smaller
than the temporal; ties go to the temporal) + memorials ONLY while the
temporal stays principal: on rank IX days with `commemoratio`, on rank
XIII days in full (`ad_libitum` for optional ones). Principal = smallest
ordo among entries neither ad_libitum nor commemorated; colour = the
principal's. Temporal ranks: Easter Sunday, Good Friday, Holy Saturday
I (UNLY 19: the Triduum closes with Easter Vespers); Holy Thursday as
a DAY II (Holy Week Mon-Thu - the Triduum starts in the evening).

Tests: sanctorale ordo/gradus consistency, temporal ranks, the 4a FAIL
rows, commemoration/ad libitum flags, and a 2000-2040 property sweep
(principal never NIHIL/ad lib/commemorated, minimal, colour; Nov 2 =
All Souls; no memorial principal on rank IX; a solemnity loses only to
rank I-II). Seven plants red (principal direction, commemoratio, octave
not privileged, Holy Thursday in the Triduum, ad libitum, Easter rank,
winner without rank). The sweep uses `piscina_vacare` per day on its
own pool - obtinere_diem allocates ~1 KB per call.

Not done (PULL): two obligatory memorials on one day -> both optional.
No fixed date has two (checked); the first case is the Immaculate
Heart (movable, L2), so the rule lands with it. `chorda_ex_buffer`
takes a non-const buffer, so pointing chordae at the static sanctorale
strings needs a const cast (-Wcast-qual): titles are still copied (L5).

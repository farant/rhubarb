# liturgia — plan for fixing calendarium_liturgicum (US calendar)

*Written 2026-10-15 in secunda (at 49bd1cf2), from Fran's answers to
the open questions of `liturgia-recensio/RECENSIO.md` (the review of
2026-10-13; ledger park …KC11SH). The question: the library under the
calendar app swallows solemnities on Sundays, never shows All Souls,
has no movable feasts and no transfers - fix it, as the US proper
calendar.*

*Discipline: phases land one at a time, each red-first with plants
(`silva.planta`), each committed with its gates. PULL rule - nothing
built ahead of the phase that needs it. Header text shown to Fran
before coding. Finding numbers (F1..F15) and test tables (4a..4f) refer
to RECENSIO.md.*

## I. Decisions (Fran, 2026-10-15)

1. **The calendar is the US proper calendar.** `calendarium_creare`
   IS the US calendar; no region parameter yet (other calendars are a
   later design question, 10-13). Data is shaped so a region layer can
   come later.
2. **Ascension and Corpus Christi on Sunday** (most US provinces);
   **Epiphany** = the Sunday Jan 2-8. No province flag for now.
3. **Precedence by Table rank**: new `OrdoTabulae` (UNLY 59 rows 1-13)
   on every `Celebratio`; `gradus` stays the KIND for display.
4. **Optional memorials never become the principal** and never set
   `color_diei`; they are listed with `ad_libitum` (as printed ordos).
5. **Commemorations stay in `celebrationes`** with a `commemoratio`
   flag (memorials on Lent weekdays, Dec 17-24, Christmas octave),
   never the principal.
6. **Colours**: Holy Saturday violet (the Vigil belongs to Easter);
   All Souls violet; Palm Sunday red; Holy Thursday white.
7. **Holy days of obligation** become a function of (calendar, date)
   with the US abrogation rules (Jan 1, Aug 15, Nov 1 on Sat/Mon;
   Dec 8 transferred carries none). May slip to L5.
8. **Domain 1970..9999** for the liturgy, refused outside; `fasti`
   computus stays 1583+.
9. **`calendarium_invenire_festum`** gets implemented after L2.
10. **New proper titles in English** like the sanctorale ("Ascension
    of the Lord"); generic Sunday/weekday titles unchanged for now.

## II. Phases

| Phase | Content | Findings |
|---|---|---|
| **L0** | Guards that pass today: Easter vs published table 2000-2040, independent computus (Oudin) 1583-4099, extremes, anchors, the "pass (guard)" rows of 4a-4c. Fix `finis_temporis(PA_II)` (season symmetry test red first). | F9, 4d |
| **L1** | Precedence model (API): `OrdoTabulae`, `ordo`/`commemoratio`/`ad_libitum` on `Celebratio`; principal = smallest ordo; "feast of the Lord" kind in the sanctorale; All Souls ranked with solemnities. | F1 F2 F3 F7 F13 |
| **L2** | Proprium de tempore table (movable celebrations with title, rank, colour) + US temporale (Epiphany Sunday, Ascension/Corpus Christi Sunday, Baptism rule). | F5 F6 F8 F11, F10 temporale |
| **L3** | Per-year transfer pass stored with the anchors (Joseph, Annunciation, Immaculate Conception, John Baptist vs Sacred Heart, impeded solemnities in general). | F4 |
| **L4** | Sanctorale data audit: US fixes, 2018-2021 universal additions, "current as of" date, comment/data mismatches. | F10 data |
| **L5** | Psalter week after Ash Wednesday; `nomen_celebrationum` on `obtinere_diem`; robustness (domain, invalid dates, NULL, anchors by value, fewer allocations); `invenire_festum`; obligation function if not done; property sweep 1970-2100 as a test. | F12 F14 F15, 4f |

Consumers to keep working: `lib/kalendarium.c` (colour, principal
title, season name), `lib/schirmata.c` (`calendarium_nomen_celebrationum`),
`lib/calendario_visus.c` (`dies_obligationis`).

## III. Status

- L0 DONE 2026-10-15: suite in credo II sections (16), guards
  (computus table + Oudin 1583-4099, extremes, anchor relations,
  4a-4c guard rows), F9 fixed (season symmetry red first, plant).
- L1 - next: header text for OrdoTabulae first.

## IV. AUDIENDA

- Psalter weeks in the Christmas season and the Easter octave: no
  authoritative source checked; the library returns 1 (RECENSIO Q6).
  Not asserted until a source is chosen.
- Rows in RECENSIO are verified against the probe at 6ed11bd6; the
  library has not changed since (checked 2026-10-15), but every row is
  re-checked when it becomes a test.

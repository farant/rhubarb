
## 2026-09-11 — the ISO pair: fasti could format a date but not an instant

`fasti_parsare_iso` took `YYYY-MM-DD` only, so nothing in fasti could
read or write a full timestamp. plist `<date>` is an ISO-8601 UTC stamp
(`2026-09-11T02:41:32Z`), so the conversion landed here rather than
inside plist — a date parser has no business living in a property-list
library (`project-specs/plist-spec.md` §6.5).

Named `fasti_ad_iso` / `fasti_ex_iso` to sit beside the two pairs that
already exist: `fasti_ad_unix` / `fasti_ex_unix` and `fasti_ad_iulianum`
/ `fasti_ex_iuliano`. Same type in, same direction convention.

**Reuse rather than reimplementation.** `fasti_ex_iso` hands the first
ten bytes to `fasti_parsare_iso` (via `chorda_sectio`) and the three
time fields to the existing `_parsare_duo_digiti`, then validates with
`fasti_dies_valida` + `fasti_hora_valida`. The only new arithmetic is
the zone.

**The sign convention, stated once.** `+02:00` means the local clock runs
AHEAD of UTC, so UTC = local − offset. The sign is therefore inverted
exactly once, at parse time, and the offset is then added. Both
directions are pinned, each with a day rollover: `04:41:32+02:00` →
`02:41:32` same day; `00:30:00+02:00` → previous day 22:30;
`23:30:00-02:00` → next day 01:30.

**Two refusals rather than two silent truncations:**

- **Fractional seconds** (`…:32.5Z`) are refused in v1. Nothing needs
  them, and a silently truncated instant is worse than a named refusal.
- **Years outside 0..9999** are refused by `fasti_ad_iso`, because the
  four-digit field cannot express them. Returning an empty chorda beats
  emitting a stamp that does not mean what it says.

`fasti_ad_iso` writes its twenty digits straight into the arena
allocation — no stack buffer and no `memcpy`, so the function adds no
dependency to this file.

Calibration: two planted faults, each of which had to keep the code
compiling (the first attempt left `offsetum_negans` set-but-unused and
`silva.planta`'s pre-flight refused it — a plant that breaks the build
proves nothing). Inverting the sign condition reddened the `+02:00`
assertion at `probatio_fasti.c:792`; accepting an unknown zone as UTC
reddened the fractional-seconds refusal at `:827`. Both green after the
reverts.

Note for readers of the diff: this was `lib/fasti.c`'s first pass under
the formatter, so the commit carries `cosmetica` rewraps of pre-existing
lines beside the two added functions.

## 2026-09-12 — Latin date forms: genitive month, Roman numerals

Found through `project-specs/exempla/kalendarium.thistle`:
`FASTI_FORMA_LITURGICA` wrote "Feria II, 14 September 2026" and
`FASTI_FORMA_LATINA_LONGA` "14 September 2026". The header promised
"Dominica, 15 Martii 2024" and "XV Martii MMXXIV". The `.c` admitted
"simplificatum: 15 Martii 2024" for the long form, but even the
simplification was not what it said: `NOMINA_MENSIUM_LATINA` is
nominative (Martius, September), and a Latin date takes the month in
the genitive — *XV Martii*, the fifteenth (day) of March. No code in the
tree used either form; `probatio_fasti.c` tested only the short one.

The header's own example was wrong too: 15 March 2024 was a Friday,
"Feria VI", not "Dominica".

**Changes.** `NOMINA_MENSIUM_LATINA_GENETIVO` (Ianuarii … Decembris;
Aprilis and the -ber months are third declension, -is). Both forms use
it. `_romanum_scribere` writes I..MMMCMXCIX; outside that range (year 0,
negative years, 4000 and up) it writes digits, because a Roman numeral
cannot express them and a wrong numeral is worse than a foreign one.
The long form writes day and year with it; the liturgical form keeps
digits, as its header example always showed. `fasti_nomen_mensis` is
unchanged: naming a month, the nominative is right.

**Gates (179 → 199).** Both forms on 15 March 2024; the first of every
month of 2026 in the long form (all twelve genitives); 31 December 1999
(CM, XC, IX); 24 August 3999 (IV, and the top of the range); 19 July
3888 (the longest numeral below 4000); 9 January 4000 and 14 April of
year 0 (digits); 14 September 2026 in the liturgical form. Born red:
20, predicted 20.

| Plant | Predicted | Observed |
|---|---|---|
| FP1 liturgical form back to the nominative | 2 | 2 |
| FP2 "DCCCC" for "CM" | 2 | 2 (1999, 3999) |
| FP3 3999 excluded from the range | 1 | 1 |
| FP4 year 0 admitted | 1 | 1 — nothing written where the year should be |

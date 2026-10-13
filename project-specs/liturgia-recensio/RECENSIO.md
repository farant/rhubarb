> **Status (Fran, 2026-10-13):** the calendar is the **US** proper
> calendar. Supporting other calendars (General Roman, other
> conferences) is a later design question, not now. This review is the
> basis of a future project, likely next after the calendar widget
> (agenda-plan A2-A6). Ledger: parcum …KC11SH. Probe sources are kept
> here as `.c.txt` (English identifiers; not part of the C tree) -
> rebuild with `build.sh.txt` against the repo sources.

# Review: calendarium_liturgicum (Ordinary Form calendar)

Date: 2026-10-13. Tree: `rhubarb-secunda` at 6ed11bd6 (working tree, read-only).
Files reviewed: `include/calendarium_liturgicum.h`, `lib/calendarium_liturgicum.c`,
`lib/sanctorale.c`, `include/fasti.h`, `lib/fasti.c`, `probationes/probatio_calendarium.c`,
`lib/calendarium_liturgicum.worklog.md`, `lib/fasti.worklog.md`.

Every "ACTUAL" below is the library's own output. It comes from the probes in this
directory, compiled with plain clang against the repo sources:

- `probe.c`: one-line dumps of selected days, an Easter cross-check, anchors, the season-boundary API and robustness checks. Its output is in `probe_output.txt`. `./probe dump Y M D` prints one day and `./probe year Y` prints a whole year.
- `scan.c`: lists whole bug classes over a range of years. The 2020-2040 output is in `scan_output.txt`. `./scan Y0 Y1` runs another range.
- `build.sh`: rebuilds the probe.

Notation: in the probe output, `gr` is the `GradusCelebrationis` of `celebratio_principalis`.
"Table n" means row n of the UNLY Table of Liturgical Days (UNLY 59).

---

## 1. Summary

### What is right

- **Easter computus is correct.** Its results match the published table for every year from 2000 to 2040 (0 mismatches out of 41). They also match an independent algorithm (Oudin 1940) for every year from 1583 to 4099 (0 mismatches out of 2517). The extreme dates are right: 1818 and 2285 give Mar 22; 1943 and 2038 give Apr 25; 1981 and 2076 give Apr 19.
- **Anchors derived from Easter are correct** in every checked year: Ash Wednesday, Palm Sunday, the Triduum, Ascension (Thursday), Pentecost, Trinity, Corpus Christi (Thursday), Sacred Heart, Advent I (Nov 27 to Dec 3), and Christ the King.
- **The season of each day is correct.** So are Advent and Lent week numbers, the Easter week numbers, and the backward count of Ordinary Time after Pentecost (it matches 2008 week 6 and 2038 week 11).
- **The Sunday (A/B/C) and weekday (I/II) cycles change on Advent I**, which is correct (fixed 2026-09-12).
- Gaudete and Laetare are rose. Pentecost is red.
- These precedence cases behave correctly:
  - Solemnities and feasts on ordinary weekdays win.
  - Feasts beat Lent weekdays and Advent weekdays (St Andrew 2026-11-30, Chair of Peter 2027-02-22).
  - Advent, Lent and Easter Sundays exclude sanctoral celebrations.
  - Ash Wednesday, Holy Week and the Easter octave exclude everything.
  - Saints' feasts on Ordinary Time Sundays are correctly suppressed (St Luke 2026-10-18).

### The most important problems

1. **Solemnities that fall on a Sunday of Ordinary Time or Christmas are swallowed by the Sunday.** This is the reported Nov 1 2026 bug. The class has 21 dates in 2020-2040 and 132 in 1970-2100, and it includes Christmas Day (2022, 2033, 2039), Mary Mother of God, Epiphany, All Saints, the Assumption, Peter & Paul and the Nativity of John the Baptist. The cause: the principal celebration is chosen by `GradusCelebrationis`, where `GRADUS_DOMINICA` (V) is greater than `GRADUS_SOLLEMNITAS` (IV). That happens even though `comparare_praecedentiam` already decided that the solemnity wins.
2. **All Souls (Nov 2) never appears, in any year**, whether Nov 2 is a weekday or a Sunday. An ordering bug in `comparare_praecedentiam` drops it.
3. **Feasts of the Lord on Ordinary Time Sundays are dropped.** These are the Presentation, Transfiguration, Exaltation of the Cross and Lateran (12 dates in 2020-2040). The data model cannot tell a feast of the Lord from a saint's feast.
4. **No transfers.** An impeded solemnity simply disappears: St Joseph on a Lent Sunday or in Holy Week, the Annunciation in Holy Week or the Easter octave or on Palm Sunday, the Immaculate Conception on an Advent Sunday.
5. **Movable celebrations never reach `calendarium_obtinere_diem`.** The anchors are computed but never turned into celebrations: Ascension, Trinity, Corpus Christi, Sacred Heart, Christ the King, Holy Family, Baptism of the Lord (by name), Mary Mother of the Church, Immaculate Heart, and the names of Pentecost, Palm Sunday, Easter and the Triduum. Effects:
   - Ascension 2026 shows St Matthias in red.
   - Christ the King and Trinity are green.
   - In 2022 and 2033 John the Baptist stays on June 24, colliding with the Sacred Heart.
6. **Memorials become the principal celebration in privileged seasons**: Lent weekdays, Dec 17-24 and the Christmas octave. The day then takes the saint's colour: Perpetua & Felicity in Lent comes out red, not violet.
7. **Other wrong colours:**
   - Palm Sunday is violet; it should be red.
   - Holy Thursday is red; it should be white.
   - Holy Saturday is red.
   - Christ the King, Trinity, Corpus Christi and Sacred Heart are green; they should be white.
   - All Souls is green; it should be violet or black.
8. **Wrong Baptism of the Lord when Jan 6 is a Sunday** (2008, 2013, 2019, 2030, 2036). The library uses Monday Jan 7, but the answer is Sunday Jan 13 under both the General Roman and the US calendar. This also shifts every weekday number of Ordinary Time I up by one, until Ash Wednesday.
9. **`calendarium_finis_temporis(TEMPUS_PER_ANNUM_II, 2026)` returns 2027-11-27** instead of 2026-11-28. This is the same "next year's Advent" bug that was fixed in `calendarium_tempus_info` on 2026-09-12.
10. **The calendar is a mix.** `sanctorale.c` is mostly the US proper calendar (with US errors), while the temporale is universal (Epiphany fixed on Jan 6, Ascension on Thursday). Neither calendar is implemented faithfully.

The existing suite (`probatio_calendarium.c`) has no test where a solemnity or feast of the Lord falls on a Sunday. It has no transfer test and no test of a movable feast's title or colour. Its All Souls test asserts only `numerus_celebrationum >= 1`, which the temporal entry alone always satisfies, so it passes while All Souls is missing.

---

## 2. Findings (ranked by severity)

### F1. CRITICAL: A Sunday outranks every solemnity when the principal is chosen

**Rule.** UNLY 5: a Sunday "gives way only to solemnities and feasts of the Lord". Only the Sundays of Advent, Lent and Easter take precedence over solemnities. Table: solemnities are rank 3; Sundays of Christmas and Ordinary Time are rank 6.

**Cause.**

- `include/calendarium_liturgicum.h:48-56` orders `GRADUS_SOLLEMNITAS = IV` below `GRADUS_DOMINICA = V`.
- `creare_celebrationem_temporale` gives every Sunday `GRADUS_DOMINICA`, whatever the season (`lib/calendarium_liturgicum.c:1183-1185`).
- The filter is actually right: `comparare_praecedentiam` returns `I` ("solemnity wins") for an Ordinary Time Sunday (`:1094-1101`).
- But its result is only used to include or exclude the sanctoral entry (`:1329`). The principal is then chosen by the raw `gradus` (`:1351-1357`), so the Sunday wins.

**Every case 2020-2040** (`scan_output.txt`, class C1; 132 cases 1970-2100):

| Date | Expected (principal, colour) | ACTUAL |
|---|---|---|
| 2020-11-01, 2026-11-01, 2037-11-01 | All Saints, SOLLEMNITAS, white | Dominica XXXI per Annum, DOMINICA, green |
| 2021-08-15, 2027-08-15, 2032-08-15, 2038-08-15 | Assumption, white | Dominica XX per Annum, green |
| 2025-06-29, 2031-06-29, 2036-06-29 | Ss Peter & Paul, red | Dominica XIII per Annum, green |
| 2029-06-24, 2035-06-24, 2040-06-24 | Nativity of St John Baptist, white | Dominica XII per Annum, green |
| 2022-12-25, 2033-12-25, 2039-12-25 | Nativity of the Lord | "Dominica - Tempus Nativitatis" (white, but wrong celebration) |
| 2023-01-01, 2034-01-01, 2040-01-01 | Mary, Mother of God | "Dominica - Tempus Nativitatis" |
| 2030-01-06, 2036-01-06 (and 2019-01-06) | Epiphany | "Dominica - Tempus Nativitatis" |

`titulus_diei` is wrong on the same dates. That matters to `lib/kalendarium.c:345`, which paints the day cell with `color_diei`: Nov 1 2026 is drawn green.

**Fix (minimal patch).** Pick the principal by precedence, not by `gradus`:

1. Compute a `GradusPraecedentiae` for each celebration: `praeced_temp` for the temporal entry, `obtinere_praeced_sanctorale()` for each sanctoral entry.
2. Store it next to the celebration (a parallel local array is enough).
3. Choose `principalis` by the maximum precedence; on ties, prefer the temporal entry.
4. Keep `gradus` as a descriptive "kind" for display.

With the existing `PRAECED_*` values this already does the right thing: an Ordinary Time Sunday is 5, a solemnity is 7, and an Advent, Lent or Easter Sunday is 8 (but those exclude the solemnity anyway). The real fix is the model change in section 3.

### F2. CRITICAL: All Souls is never celebrated

**Rule.** Table 3 lists the Commemoration of All the Faithful Departed together with the solemnities, so it outranks an Ordinary Time Sunday.

**Cause.** In `obtinere_praeced_temporale`, Nov 2 itself is mapped to `PRAECED_DEFUNCTORUM` (VI) (`:1034-1038`). Inside `comparare_praecedentiam`, the check `si (praeced_temp >= PRAECED_DOMINICA) redde -I;` (`:1105`) fires before the special case at `:1111`, because DEFUNCTORUM (6) ≥ DOMINICA (5). When Nov 2 is a Sunday, the temporal value is `PRAECED_DOMINICA`, and the same line drops it again. The special case is unreachable.

| Date | Expected | ACTUAL |
|---|---|---|
| 2026-11-02 Mon | All Souls, violet (or black) | Feria II, Hebdomada XXXI, FERIA, green |
| 2025-11-02 Sun | All Souls, violet | Dominica XXXI per Annum, green |
| 2014-11-02 Sun | All Souls | Dominica XXXI per Annum, green |
| 2024-11-02 Sat | All Souls | Sabbatum, Hebdomada XXX, green |

The existing test at `probatio_calendarium.c:688-693` only asserts `numerus_celebrationum >= I`, so it passes.

**Fix.**

- The temporal side should not know about Nov 2. Delete `:1034-1038`.
- In the sanctoral mapping, rank All Souls with the solemnities: `PRAECED_SOLLEMNITAS`, or a dedicated value at the same level. Then the existing solemnity branch handles it, and it beats an Ordinary Time Sunday.
- Give it its own kind rather than reusing `GRADUS_DIES_PECULIARIS`, which is also used for Ash Wednesday, Holy Week and the Easter octave.

### F3. HIGH: Feasts of the Lord on Ordinary Time Sundays are dropped

**Rule.** UNLY 5 and Table 5 (feasts of the Lord) versus Table 6 (Ordinary Time Sundays). The Lateran dedication is treated as a feast of the Lord in this respect: it replaced the Sunday in 2008, 2014 and 2025.

**Cause.** `SanctoraleDatum` has only `gradus`, so the Presentation, Transfiguration, Exaltation and Lateran are plain `GRADUS_FESTUM`. Then `:1105` lets every Sunday beat every feast.

**Every case 2020-2040:**

| Feast | Dates on a Sunday |
|---|---|
| Presentation | 2020-02-02, 2025-02-02, 2031-02-02 |
| Transfiguration | 2023-08-06, 2028-08-06, 2034-08-06 |
| Exaltation of the Cross | 2025-09-14, 2031-09-14, 2036-09-14 |
| Dedication of the Lateran | 2025-11-09, 2031-11-09, 2036-11-09 |

For example, 2025-09-14 ACTUAL: "Dominica XXIV per Annum", DOMINICA, green. Expected: Exaltation of the Holy Cross, FESTUM, red. On 2025-11-09, the Lateran (white) is replaced by "Dominica XXXII per Annum" in green.

**Fix.** Add a kind to the sanctorale data (for example `b32 festum_domini`, or a separate rank `GRADUS_FESTUM_DOMINI`). In precedence, feast of the Lord (Table 5) > Ordinary Time / Christmas Sunday (Table 6) > other feasts (Table 7).

### F4. HIGH: No transfer step; impeded solemnities vanish

**Rules.**

- UNLY 5: a solemnity on an Advent, Lent or Easter Sunday moves to the following Monday. The exception is Palm Sunday or Easter Sunday.
- UNLY 60: an impeded solemnity moves to the nearest day not listed in Table 1-8. The Annunciation in Holy Week or the Easter octave goes to the Monday after Easter II.
- CDW 2008: St Joseph in Holy Week is anticipated to the Saturday before Palm Sunday.

**Cause.** `:1329` simply leaves out any sanctoral entry with `comp < 0`. Nothing is queued or moved.

| Year | Impeded on | Expected new date | ACTUAL on the impeded date | ACTUAL on the expected date |
|---|---|---|---|---|
| 2023 | Mar 19, Lent Sunday | Mon Mar 20, St Joseph | Dominica IV Quadragesimae (rose) | Feria II, violet |
| 2028 | Mar 19, Lent Sunday | Mon Mar 20 | Dominica III Quadragesimae | Feria II |
| 2034 | Mar 19, Lent Sunday (Laetare) | Mon Mar 20 | Dominica IV Quadragesimae (rose) | Feria II, Hebdomada IV |
| 2008 | Mar 19, Holy Wednesday | Sat Mar 15 | Feria IV, Hebd. VI | Sabbatum, violet |
| 2035 | Mar 19, Holy Monday | Sat Mar 17 | Feria II, Hebd. VI | **St Patrick (MEM_OPT, white)** |
| 2024 | Mar 25, Holy Monday | Mon Apr 8, Annunciation | Feria II, Hebd. VI | Feria II, Hebd. II, white |
| 2008 | Mar 25, Easter Tuesday | Mon Mar 31 | Feria III, Hebd. I | Feria II, Hebd. II |
| 2027 | Mar 25, Holy Thursday | Mon Apr 5 | (Triduum) | St Vincent Ferrer (MEM_OPT) |
| 2029 | Mar 25, Palm Sunday | Mon Apr 9 | Dominica VI Quadragesimae | Feria II, Hebd. II |
| 2032 | Mar 25, Holy Thursday | Mon Apr 5 | (Triduum) | St Vincent Ferrer (MEM_OPT) |
| 2035 | Mar 25, Easter Sunday | Mon Apr 2 | Dominica I Paschae | St Francis of Paola (MEM_OPT) |
| 2040 | Mar 25, Palm Sunday | Mon Apr 9 | (dropped) | Feria II, white |
| 2024 | Dec 8, Advent II Sunday | Mon Dec 9, Immaculate Conception | Dominica II Adventus | St Juan Diego (MEM_OPT, white) |
| 2030 | Dec 8, Advent II Sunday | Mon Dec 9 | Dominica II Adventus | St Juan Diego |

**Fix.** Add a per-year transfer pass, computed once with the anchors:

1. Walk the fixed solemnities and the movable ones (after F5).
2. For each, find the date and test it against the impeding days, using the same precedence function as day lookup.
3. When impeded, apply the rule for that celebration:
   - Joseph in Holy Week → the Saturday before Palm Sunday.
   - Annunciation in Holy Week or the octave, or on Palm/Easter Sunday → the Monday after Easter II.
   - Otherwise → the next day not in Table 1-8.
4. Store the result in a small array inside `AnchoresMobiles`: `{Dies dies; s32 index_sanctoralis;}`. Three to five entries a year is enough.

`calendarium_obtinere_diem` then adds "celebrations transferred onto this day" next to `sanctorale_obtinere`. The same mechanism also handles the Sacred Heart and John the Baptist collision (F5).

### F5. HIGH: Movable celebrations are absent from day lookup

**Rules.**

- UNLY 6: Holy Family, Baptism, Trinity and Christ the King are permanently assigned to Sundays.
- UNLY 25: Ascension. UNLY 7: transfer of Epiphany, Ascension and Corpus Christi to a Sunday.
- Table 2-3 ranks Ascension and Pentecost and the solemnities of the Lord.
- Mary, Mother of the Church: CDW decree of 2018.

**Cause.** `computare_anchorae` computes `ascensio`, `ss_trinitas`, `corpus_christi`, `cor_iesu` and `christus_rex`, but `calendarium_obtinere_diem` never reads them. The only celebrations it builds are the generic temporal day and the fixed `SANCTORALE[]`. `formare_titulum` knows only "Dominica N <tempus>" and "Feria N, Hebdomada N".

| Date | Expected | ACTUAL |
|---|---|---|
| 2026-05-14 Thu | Ascension, SOLLEMNITAS, white; St Matthias omitted that year | **St Matthias, FESTUM, red** |
| 2026-05-24 | Pentecost, red | "Dominica VIII Paschae", DOMINICA, red (colour right, title wrong) |
| 2026-05-25 Mon | Mary, Mother of the Church, MEMORIA, white | St Bede (MEM_OPT), white |
| 2026-05-31 Sun | Most Holy Trinity, SOLLEMNITAS, white | Dominica IX per Annum, green |
| 2026-06-04 Thu | Corpus Christi (universal) | Feria V, Hebd. IX, green |
| 2026-06-12 Fri | Sacred Heart, SOLLEMNITAS, white | Feria VI, Hebd. X, green |
| 2026-06-13 Sat | Immaculate Heart (MEMORIA) coinciding with St Anthony (MEMORIA) | St Anthony only |
| 2022-06-24 / 2033-06-24 | Sacred Heart; John the Baptist moved to Jun 23 (as CDW did in 2022) | John the Baptist on Jun 24; Jun 23 is a green feria |
| 2038-06-24 Thu | Corpus Christi (universal Thursday) clashes with John the Baptist; under the US calendar Corpus Christi is Sun Jun 27 and there is no clash | John the Baptist, with no awareness of the clash |
| 2026-11-22 / 2025-11-23 | Christ the King, SOLLEMNITAS, white | Dominica XXXIV per Annum, green |
| 2025-12-28 Sun | Holy Family, FESTUM, white | "Dominica - Tempus Nativitatis" |
| 2027-12-26 Sun | Holy Family (St Stephen omitted; that part is correct) | "Dominica - Tempus Nativitatis" |
| 2022-12-30 Fri | Holy Family (Christmas on a Sunday, so no Sunday in the octave) | Feria VI, Hebdomada I |
| 2026-01-11 Sun | Baptism of the Lord, FESTUM, white | "Dominica - Tempus Nativitatis" |

**Fix.** Add a "proprium de tempore" table. Each entry holds an anchor (Easter, Advent I, Christmas, Epiphany), an offset, a title, a rank, a kind and a colour. `obtinere_diem` adds the matching entry as a celebration with `GENUS_MOBILE`; that enum value already exists and is unused. Easter Sunday, the Triduum days, Palm Sunday and Ash Wednesday belong in the same table, so they get proper names and the right colours (F6).

### F6. HIGH: Wrong colours on specific days

**Rule.** GIRM 346:

- Red on Palm Sunday and Good Friday.
- White for the Mass of the Lord's Supper and for solemnities of the Lord other than those of the Passion.
- Violet or black for All Souls.

| Date | Expected | ACTUAL | Cause |
|---|---|---|---|
| 2026-03-29 Palm Sunday | red | violet | `calendarium_color_temporis` Lent branch `:519-530`, which has no Palm Sunday case |
| 2026-04-02 Holy Thursday | white (Chrism Mass and Lord's Supper) | red | `:532-533`: the whole Triduum is red |
| 2026-04-04 Holy Saturday | no Mass by day; the Vigil is white (policy question, see section 5) | red | `:532-533` |
| Christ the King, Trinity, Corpus Christi, Sacred Heart | white | green | F5 |
| All Souls | violet or black | green | F2 |
| Lent and Dec 17-24 weekdays with a memorial | violet | the saint's colour | F7 |

`COLLIT_NIGER` exists but is never produced.

### F7. HIGH: Memorials become the principal in privileged seasons and take over the day's colour

**Rules.**

- Table 9: weekdays of Lent, Dec 17-24 and the Christmas octave outrank memorials (Table 10-12).
- UNLY 14: obligatory memorials on Lent weekdays can be celebrated only as optional memorials.
- GIRM 355a-b: in those periods the saint is at most a commemoration (the collect); the Mass and colour are of the day.

**Cause.**

- `comparare_praecedentiam` `:1127-1140` returns 0 ("permitted") for memorials on privileged weekdays.
- The principal loop `:1351-1357` then prefers `GRADUS_MEMORIA` (2) or `GRADUS_MEMORIA_AD_LIBITUM` (1) to the temporal `GRADUS_FERIA` (0), because privileged weekdays are graded `GRADUS_FERIA` (`:1196-1198`).
- Christmas octave weekdays are not privileged at all: they fall through to `PRAECED_FERIA` at `:1047`.
- Also, the comment at `:1028`/`:1040` treats Advent weekdays before Dec 17 as "privileged". That only gives the right result by accident: memorials are fully celebrated then.

**2026 instances** (254 in 2020-2040; 1591 in 1970-2100):

| Date | ACTUAL principal | Expected |
|---|---|---|
| 2025-03-07 (Lent Fri) | Ss Perpetua & Felicity, MEMORIA, **red** | Lent weekday, violet; commemoration of Perpetua & Felicity |
| 2026-02-23 (Lent Mon) | St Polycarp, MEMORIA, red | Lent weekday, violet |
| 2026-03-17 | St Patrick, MEM_OPT, white | Lent weekday, violet |
| 2026-02-21 (Sat after Ash Wednesday) | St Peter Damian, white | violet |
| 2026-12-21, 2026-12-23 | St Peter Canisius / St John of Kanty, white | Dec 21 / Dec 23 privileged weekday, violet |
| 2026-12-29, 2026-12-31 | St Thomas Becket (red) / St Sylvester (white) | 5th / 7th day in the octave of Christmas, white |

**Fix.** Give the temporal entry its real Table rank: rank 9 for Lent weekdays, Dec 17-24 and the octave days. Return memorials on those days in `celebrationes`, flagged `commemoratio = VERUM` and never eligible as principal. `color_diei` stays the season's colour.

### F8. MEDIUM: Wrong Baptism of the Lord when Jan 6 is a Sunday; Ordinary Time I weekdays one week too high

**Rule.**

- UNLY 38: the Baptism is the Sunday after Jan 6.
- With Epiphany transferred to Sunday (UNLY 7; the US practice), the Baptism moves to Monday only when Epiphany is Jan 7 or 8.
- When Jan 6 is itself a Sunday, the Baptism is Sunday Jan 13 under both calendars.

**Cause.** `lib/calendarium_liturgicum.c:133-137`. The worklog already flags this.

| Date | Expected | ACTUAL |
|---|---|---|
| 2019-01-07 Mon | Monday after Epiphany (Christmas season) | still Christmas season; library Baptism = this day |
| 2019-01-13 Sun | Baptism of the Lord, white, Christmas season | "Dominica I per Annum", green, PA_I |
| 2019-01-14 Mon | Ordinary Time week 1 | week **2** (psalter 2) |
| 2019-01-19 Sat | week 1 | week 2 |
| 2019-01-21 Mon | week 2 | week **3** |
| 2008-01-13 Sun | Baptism of the Lord | "Dominica I per Annum" |

Affected years: 2008, 2013, 2019, 2030, 2036, 2041, 2047. Sundays are numbered correctly by accident (6 days / 7 rounds down); every weekday is off by one until Ash Wednesday.

**Fix.** Universal calendar: `baptisma = epiphania + (VII - dies_hebdomadae)` in all cases, which gives +7 when Jan 6 is a Sunday. For a US mode, first compute Epiphany as the Sunday from Jan 2 to Jan 8, then put the Baptism on Monday if Epiphany is Jan 7 or 8, otherwise on the next Sunday.

### F9. MEDIUM: `calendarium_finis_temporis(TEMPUS_PER_ANNUM_II, annus)` is a year late

**Cause.** `:281-283` uses `calendarium_anchorae(cal, annus + I)`. Ordinary Time II of year N ends the day before Advent I of the same civil year N, which is the bug class fixed in `tempus_info` on 2026-09-12.

| Call | Expected | ACTUAL |
|---|---|---|
| `finis_temporis(PA_II, 2026)` | 2026-11-28 | **2027-11-27** |

(`initium` for the same season gives 2026-05-25, which is correct.)

**Fix.** Use `annus` (and add a test that pairs `initium` and `finis` for every season).

### F10. MEDIUM: Which calendar? It is a mix, with errors in both directions

The header (`:14-17`) and `sanctorale.c:1-6` say "Calendarium Romanum Generale (1969)", "with US holy days".

The data is mostly the **US proper calendar**:

- US memorials: Seton (Jan 4), Neumann (Jan 5), Kateri (Jul 14), Cabrini (Nov 13), Brebeuf & Jogues (Oct 19).
- Our Lady of Guadalupe is a feast (Dec 12).
- Peter Claver is an obligatory memorial.
- Damien, Serra, Isidore the Farmer, Durocher, Duchesne, Miguel Pro.
- Camillus is moved to Jul 18 and Elizabeth of Portugal to Jul 5.
- There is a "July 4 Independence Day" entry.
- `dies_obligationis` is commented "US Holy Day of Obligation", and the test asserts Peter & Paul is "not an obligation in the US".

The temporale, however, is universal: Epiphany is fixed on Jan 6, and Ascension and Corpus Christi are Thursdays. So **for US users**:

- Epiphany is wrong in 6 years out of 7 (e.g. 2026: US Sunday Jan 4; library Tue Jan 6).
- Ascension is on Thursday in most US provinces where it should be the 7th Sunday of Easter (Boston, Hartford, New York, Newark, Philadelphia and Omaha keep Thursday).
- Corpus Christi is on Thursday where it should be Sunday.

**US data errors** (judged against the US calendar, which the data clearly intends):

- Oct 19: St Paul of the Cross belongs on **Oct 20** in the US. Probe 2026-10-19 lists both Brebeuf/Jogues and Paul of the Cross.
- Jan 22: the US Day of Prayer for the Legal Protection of Unborn Children is missing. St Vincent moves to **Jan 23** in the US, together with St Marianne Cope, who is missing.
- Missing US optional memorials: St Katharine Drexel (Mar 3), St André Bessette (Jan 6, a weekday in US years), St Teresa of Calcutta (Sep 5).
- "Independence Day" (Jul 4) is a Mass formulary in the US Missal, not a calendar memorial. Recommend removing it or flagging it as a votive or optional proper Mass.

**Missing universal additions since 2018** (needed under either calendar):

- Mary, Mother of the Church (Monday after Pentecost, MEMORIA, 2018).
- St Paul VI (May 29, opt., 2019).
- St Gregory of Narek (Feb 27), St John of Avila (May 10) and St Hildegard (Sep 17) (opt., 2021).
- Jul 29 renamed and extended to "Ss Martha, Mary and Lazarus" (2021).

**Comment/data mismatches (the data is right):**

- Comments call these optional while their rank is MEMORIA: Polycarp (`sanctorale.c:193`), Peter Claver (`:791`; obligatory in the US) and Padre Pio (`:842`).
- Comments say "Memoria (US)" while their rank is optional: Isidore the Farmer (`:371`) and Serra (`:519`).

### F11. LOW-MEDIUM: Temporal titles and ranks are generic or wrong on named days

From probe section K:

| Date | ACTUAL title / gradus | Expected |
|---|---|---|
| Easter 2026-04-05 | "Dominica I Paschae", DOMINICA | Dominica Paschae in Resurrectione Domini (highest rank) |
| Pentecost | "Dominica VIII Paschae" | Dominica Pentecostes |
| Palm Sunday | "Dominica VI Quadragesimae" | Dominica in Palmis de Passione Domini |
| Holy Mon-Wed | "Feria II, Hebdomada VI" | Feria II Hebdomadae Sanctae |
| Triduum | "Feria V/VI, Sabbatum, Hebdomada I" | Feria V in Cena Domini / Feria VI in Passione Domini / Sabbatum Sanctum |
| Ash Wednesday | "Feria IV, Hebdomada I" | Feria IV Cinerum |
| Thu-Sat after Ash Wednesday | "Hebdomada I" (the same label as the real Lent week I) | "post Cineres" |
| Christmas Sundays | "Dominica - Tempus Nativitatis" | Holy Family / Dominica II post Nativitatem / Baptism |

Easter Sunday and Divine Mercy Sunday are reported as `GRADUS_DOMINICA` (5), *below* Easter Monday (`GRADUS_DIES_PECULIARIS`, 6). Precedence is not affected, because the octave check runs first, but the exposed rank is misleading.

### F12. LOW-MEDIUM: Psalter week wrong after Ash Wednesday

The Liturgy of the Hours uses psalter week IV from Ash Wednesday to the following Saturday. **ACTUAL** for 2026-02-18 to 02-21: `ps1`. The cause is the Lent hebdomada of 1 at `:441-445`, which feeds `calendarium_hebdomada_psalterii`.

The Christmas season is always week 1 / psalter 1 (worklog: "Christmas season weeks are all I"). The real psalter weeks there need an authoritative ordo; see Open questions.

### F13. LOW: An optional memorial becomes "principal" and recolours the day

On a weekday with only optional memorials, the first optional memorial becomes `celebratio_principalis`, and its colour becomes `color_diei`. Example: 2026-01-20 gives St Fabian, red, where the day is green.

This happens on 1474 days in 2020-2040. An optional memorial is the celebrant's choice; ordos print the weekday colour, with the option noted.

**Proposal.** The principal stays the weekday; the optional memorials are listed with `ad_libitum`. Alternatively, expose both `color_diei` (the weekday's) and the colours of the options.

### F14. LOW: `calendarium_nomen_celebrationum` ignores precedence

`lib/schirmata.c:1147` (the status bar) uses it. It concatenates `fasti_nomen_festivitatis`, which knows only Easter, Pentecost, Good Friday, Ash Wednesday and Palm Sunday, with the raw sanctorale. ACTUAL:

- 2029-03-25 → "Palm Sunday, The Annunciation of the Lord". The Annunciation is transferred that year.
- 2026-05-14 → "Saint Matthias the Apostle". It is Ascension.

**Fix.** Build it from `calendarium_obtinere_diem`: the principal plus any non-suppressed optional memorials.

### F15. LOW: Robustness

- **Year range.** No validation. `fasti_computus(1000)` returns Mar 30, a Gregorian rule applied to a Julian-era year. `fasti_computus(-5)` returns "Mar 4" of year -5, from negative `%`. `calendarium_cyclus_dominicalis(-1)` returns C. The Ordinary Form calendar is only meaningful from Advent 1969, and the Gregorian computus only from 1583. Recommend a documented domain (e.g. 1970..9999) and a refusal (`NIHIL` / `FALSUM`) outside it.
- **Invalid dates.** `calendarium_obtinere_diem(2026-02-30)` silently returns data. The weekday comes from JDN normalisation (Mar 2), while the sanctorale lookup uses Feb 30. Validate with `fasti_dies_valida`.
- **NULL handling.** `cal == NIHIL` crashes everywhere. `calendarium_anchorae` returns `NIHIL` on allocation failure, and every caller dereferences it unchecked (e.g. `calendarium_tempus:304/320`).
- **Anchor cache.** Correctness is fine: a 26-distinct-year sweep, then a descending scan from 2100 to 2000, always returned the right year. But every miss allocates a fresh `AnchoresMobiles` (about 200 B) from `cal->piscina` and never reuses the evicted slot, so memory grows without bound over a long sweep. Policy: "evict the minimum year". During a descending scan, each miss evicts the entry just inserted. That is harmless but useless as caching.
  - Recommendation: return anchors **by value**, e.g. `b32 calendarium_anchorae(cal, annus, AnchoresMobiles* exitus)`. Callers then never hold pointers into the cache, and the cache can overwrite slots in place. Today in-place overwrite would be unsafe, because callers keep the pointer across a second `calendarium_anchorae` call (`calendarium_tempus_info:407/416`).
- **Allocations per `calendarium_obtinere_diem` call.** Into the caller's pool: one `InformatioDiei`; two copies of the temporal title (`:1266`, and `:1174` via `creare_celebrationem_temporale`); a temporal `Celebratio` that is then copied into the array and abandoned; the array; and for each sanctoral entry a `Celebratio` plus two `chorda_ex_literis` copies of static strings. That is about 10 allocations, roughly 0.5-1 KB per call. `kalendarium.c` calls it once per day cell per draw into `md->piscina`, so this is fine if that pool is reset every frame and a slow leak otherwise. Cheap wins:
  - Build the celebrations directly in the array.
  - Point the `chorda` at the static sanctorale strings instead of copying them.
  - Compute `tempus_info` once (today `calendarium_tempus` runs about 8 times per call).
- **`calendarium_invenire_festum` is a public stub.** It returns 0-0-0 (ACTUAL for "Easter", 2026). It should be removed from the header or implemented. Once F5 exists, it is a simple lookup in the movable table plus the sanctorale.

---

## 3. Precedence model proposal

The structural problem is that one enum, `GradusCelebrationis`, does three jobs at once: what kind of celebration it is (solemnity, feast, memorial, Sunday, weekday), how much precedence it has, and what to display. A Sunday is not a single rank: an Advent Sunday is Table 2 and an Ordinary Time Sunday is Table 6. A feast is two ranks (Lord = 5, saint = 7). All Souls shares its value with Ash Wednesday. Keep the first job and the third, and add an explicit precedence rank:

```c
/* Ordo Tabulae Dierum Liturgicorum (UNLY 59): MINOR numerus = maior praecedentia */
nomen enumeratio {
    ORDO_TRIDUUM                  = I,     /* 1 */
    ORDO_PRIVILEGIATUS            = II,    /* 2: Nativitas, Epiphania, Ascensio, Pentecoste,
                                              Dom. Adv/Quad/Pasch, Cineres, Hebd. Sancta II-V,
                                              Octava Paschae */
    ORDO_SOLLEMNITAS_GENERALIS    = III,   /* 3: + Commemoratio Defunctorum */
    ORDO_SOLLEMNITAS_PROPRIA      = IV,    /* 4 */
    ORDO_FESTUM_DOMINI            = V,     /* 5 */
    ORDO_DOMINICA_COMMUNIS        = VI,    /* 6: Dom. Nativitatis et per annum */
    ORDO_FESTUM_GENERALE          = VII,   /* 7 */
    ORDO_FESTUM_PROPRIUM          = VIII,  /* 8 */
    ORDO_FERIA_PRIVILEGIATA       = IX,    /* 9: 17-24 Dec, Octava Nat., feriae Quad. */
    ORDO_MEMORIA_GENERALIS        = X,     /* 10 */
    ORDO_MEMORIA_PROPRIA          = XI,    /* 11 */
    ORDO_MEMORIA_AD_LIBITUM       = XII,   /* 12 */
    ORDO_FERIA                    = XIII   /* 13 */
} OrdoTabulae;
```

A `Celebratio` then carries:

- `gradus`: the kind, for display (unchanged).
- `ordo`: its Table row.
- `b32 commemoratio`: set for a memorial on a rank-9 day.
- `b32 ad_libitum`: set for an optional memorial.
- `b32 translata`: set for a transferred solemnity, with the original date.

**Algorithm (per day):**

1. **Candidates.** One temporal day: the season weekday or Sunday, or a movable proper from the *proprium de tempore* table (F5), which gives Easter, Pentecost, Trinity, Christ the King, Holy Family, Baptism, Ascension, Corpus Christi, Sacred Heart and so on. Add the sanctorale entries for the date and any **transferred** solemnities that land on this date (from the per-year transfer table).
2. **Temporal `ordo` comes from season and weekday:**
   - Sunday of Advent, Lent or Easter → 2. Sunday of Christmas or Ordinary Time → 6.
   - Ash Wednesday, Holy Mon-Thu and the Easter octave → 2. The Triduum → 1.
   - Dec 17-24, Christmas octave weekdays and Lent weekdays → 9. Other weekdays → 13.
3. **Principal** = the candidate with the smallest `ordo`. Ties: the temporal entry wins over Table 6; otherwise the rule-specific tie-breaks (UNLY 60).
4. **Losers:**
   - A solemnity whose `ordo` is ≤ 4 and which is beaten by a rank 1-2 day is **transferred**. This is decided in the per-year pass, not here, so the day view stays a pure lookup.
   - A memorial on a rank-9 day becomes a `commemoratio`.
   - Feasts and memorials beaten otherwise are omitted.
   - Two obligatory memorials on one day (e.g. Immaculate Heart with another) both become optional (CDW 2018 practice).
5. **`color_diei`** is the principal's colour, with these exceptions:
   - Optional memorials never set it.
   - Palm Sunday and Good Friday are red; Holy Thursday is white.
   - Gaudete and Laetare are rose.
6. **The per-year transfer pass** belongs with the anchors (`computare_anchorae`). It runs the same precedence function over the impeded solemnities and records `{dies_nova, celebratio}`. It holds 3-6 entries a year, and its rules are the explicit ones listed in F4.

**Calendar selection.** Make the calendar a parameter of `calendarium_creare`:

```c
nomen enumeratio { CALENDARIUM_GENERALE = ZEPHYRUM, CALENDARIUM_USA = I } RegioCalendarii;
CalendariumLiturgicum* calendarium_creare_regionis(Piscina* piscina, RegioCalendarii regio);
```

The region drives three things:

- Epiphany: Jan 6, or the Sunday from Jan 2 to Jan 8.
- Ascension and Corpus Christi: Thursday or Sunday.
- The sanctorale: a universal table plus a region overlay with add, move and re-rank entries. This overlay also expresses Camillus moving to Jul 18, Paul of the Cross to Oct 20, Vincent to Jan 23, and so on, instead of hard-coding them in the base table.

`dies_obligationis` should become a *function* of (calendar, date), not a static flag. In the US, Jan 1, Aug 15 and Nov 1 lose the obligation on a Saturday or Monday, and Dec 8 moved to Dec 9 carries none.

This keeps the public API mostly stable: `InformatioDiei`, `celebratio_principalis` and `color_diei` keep their meaning, and the changes are added fields plus one new constructor. That respects the "get the API right early" principle: the field to add now is `ordo` (with `commemoratio` and `ad_libitum`), before more consumers grow around `gradus`.

---

## 4. Proposed tests (for `probationes/probatio_calendarium.c`)

Make these table-driven, so one row covers each case. A sketch in house style (careful: `nomen` is a macro, so the field is named `titulus`):

```c
interior constans structura {
    s32 annus, mensis, dies;
    constans character* titulus_brevis;   /* principalis */
    GradusCelebrationis gradus;
    ColorLiturgicus color;
} CASUS_DIERUM[] = {
    { MM + XXVI, FASTI_NOVEMBER, I, "All Saints", GRADUS_SOLLEMNITAS, COLLIT_ALBUS },
    ...
};
/* per (i...) { info = calendarium_obtinere_diem(...);
 *   CREDO_CHORDA_AEQUALIS_LITERIS(info->celebratio_principalis->titulus_brevis, c->titulus_brevis);
 *   CREDO_AEQUALIS_S32(info->celebratio_principalis->gradus, c->gradus);
 *   CREDO_AEQUALIS_S32(info->color_diei, c->color); } */
```

Every row below was checked against the probe: **all of the "fails today" rows fail today**, and the "passes today" rows are guards.

### 4a. Precedence (catch whole classes)

| Date | Expected principal | Rank / colour | Rule | Today |
|---|---|---|---|---|
| 2026-11-01 Sun | All Saints | SOLLEMNITAS / white | UNLY 5, Table 3 > 6 | FAIL (Dominica XXXI, green) |
| 2025-06-29 Sun | Ss Peter & Paul | SOLLEMNITAS / red | same | FAIL |
| 2022-12-25 Sun | Christmas | SOLLEMNITAS / white | Table 2 | FAIL |
| 2023-01-01 Sun | Mary, Mother of God | SOLLEMNITAS / white | Table 3 | FAIL |
| 2025-09-14 Sun | Exaltation of Holy Cross | FESTUM(Domini) / red | Table 5 > 6 | FAIL |
| 2025-11-09 Sun | Lateran Basilica | FESTUM(Domini) / white | Table 5 > 6 | FAIL |
| 2026-11-02 Mon | All Souls | (defunctorum) / violet | Table 3 | FAIL |
| 2025-11-02 Sun | All Souls | / violet | Table 3 > 6 | FAIL |
| 2024-12-08 Sun | Dominica II Adventus | DOMINICA / violet | UNLY 5 | pass (guard) |
| 2027-12-12 Sun | Dominica III Adventus (Guadalupe omitted) | DOMINICA / rose | Table 2 > 8 | pass (guard) |
| 2026-10-18 Sun | Dominica XXIX per Annum (St Luke omitted) | DOMINICA / green | Table 6 > 7 | pass (guard against over-correcting F3) |
| 2026-02-22 Sun | Dominica I Quadragesimae (Chair omitted) | DOMINICA / violet | Table 2 > 7 | pass (guard) |
| 2027-02-22 Mon | Chair of St Peter | FESTUM / white | Table 7 > 9 | pass (guard) |
| 2026-11-30 Mon | St Andrew | FESTUM / red | Table 7 > 13 | pass (guard) |
| 2025-03-07 Fri | Lent weekday; Perpetua & Felicity = commemoration | FERIA / violet | UNLY 14, Table 9 > 10 | FAIL (red memorial) |
| 2026-12-21 Mon | Dec 21 weekday; Canisius = commemoration | / violet | Table 9 > 12 | FAIL |
| 2026-12-29 Tue | 5th day in the Christmas octave | / white | Table 9 > 12 | FAIL (Becket red) |
| 2025-12-03 Wed | St Francis Xavier | MEMORIA / white | Table 10 > 13 | pass (guard: Advent before Dec 17 keeps memorials) |
| 2026-01-20 Tue | weekday (Fabian/Sebastian listed ad libitum) | FERIA / green | Table 12 option | FAIL (if F13 adopted) |
| 2026-03-30 Mon | Holy Monday | DIES_PEC / violet | Table 2 | pass (guard) |

### 4b. Transfers

| Date | Expected | Rule | Today |
|---|---|---|---|
| 2023-03-20 Mon | St Joseph (transferred) | UNLY 5 | FAIL |
| 2023-03-19 Sun | Dominica IV Quadragesimae, rose | UNLY 5 | pass (guard) |
| 2008-03-15 Sat | St Joseph (anticipated) | CDW 2008 | FAIL |
| 2035-03-17 Sat | St Joseph (anticipated; not St Patrick) | CDW 2008 | FAIL |
| 2024-04-08 Mon | Annunciation | UNLY 60 | FAIL |
| 2029-04-09 Mon | Annunciation (from Palm Sunday) | UNLY 5, 60 | FAIL |
| 2035-04-02 Mon | Annunciation (from Easter Sunday) | UNLY 60 | FAIL |
| 2008-03-31 Mon | Annunciation (from Easter Tuesday) | UNLY 60 | FAIL |
| 2024-12-09 Mon | Immaculate Conception | UNLY 5 | FAIL |
| 2022-06-23 Thu | Nativity of St John Baptist (anticipated for Sacred Heart) | UNLY 60 | FAIL |
| 2022-06-24 Fri | Sacred Heart | Table 3 | FAIL |

### 4c. Movable celebrations, titles, colours

| Date | Expected | Today |
|---|---|---|
| 2026-05-14 Thu | Ascension, SOLLEMNITAS, white (Matthias omitted) | FAIL (Matthias red) |
| 2026-05-24 Sun | Pentecost, red | colour pass; title FAIL |
| 2026-05-25 Mon | Mary, Mother of the Church, MEMORIA, white | FAIL |
| 2026-05-31 Sun | Most Holy Trinity, white | FAIL (green) |
| 2026-06-04 Thu | Corpus Christi (universal), white | FAIL |
| 2026-06-12 Fri | Sacred Heart, white | FAIL |
| 2026-11-22 Sun | Christ the King, white | FAIL (green) |
| 2025-12-28 Sun | Holy Family, white | FAIL (title) |
| 2022-12-30 Fri | Holy Family | FAIL |
| 2026-01-11 Sun | Baptism of the Lord, white | FAIL (title) |
| 2019-01-13 Sun | Baptism of the Lord, Christmas season | FAIL (Dominica I per Annum, green) |
| 2019-01-14 Mon | Ordinary Time week **1**, psalter 1 | FAIL (2) |
| 2026-03-29 Sun | Palm Sunday, **red** | FAIL (violet) |
| 2026-04-02 Thu | Holy Thursday, **white** | FAIL (red) |
| 2026-04-03 Fri | Good Friday, red | pass (guard) |
| 2026-12-13 Sun | Gaudete, rose | pass (guard) |
| 2026-03-15 Sun | Laetare, rose | pass (guard) |
| 2026-02-19 Thu | psalter week **4** | FAIL (1) |

### 4d. Anchors and Easter cross-check

- **Easter 2000-2040 against the published table** (keep this as a literal table in the test; the values are in `probe.c`, `EASTER_TABLE`). Passes today. It is a guard for any future change to `fasti_computus`.
- **Independent cross-check over 1583-4099** against a second algorithm written in the test (Oudin; code in `probe.c:oudin`). Passes today, 0 mismatches. It is cheap: 2517 iterations.
- **Extremes:** 1818-03-22, 2285-03-22, 1943-04-25, 2038-04-25, and the 1981/2076 Apr 19 "Easter Sunday not Apr 26" correction.
- **For each of 2008, 2011, 2038, 2285, check:**
  - Ash Wednesday = Easter − 46, a Wednesday.
  - Pentecost = Easter + 49.
  - Advent I is in Nov 27..Dec 3 and is a Sunday.
  - Christ the King = Advent I − 7.
- **Baptism of the Lord:** 2019 → Jan 13, 2030 → Jan 13, 2024 → Jan 7. The first two fail today.
- **Season API symmetry:** for every season of 2026, `finis(t)` + 1 day == `initium(t+1)`, and `finis(PA_II, 2026)` = 2026-11-28. Fails today (F9).

### 4e. Cycles, weeks and boundaries (extend the existing ones)

| Check | Expected | Today |
|---|---|---|
| Monday after Pentecost, early-Easter year (2008-05-12) | week VI | pass |
| Monday after Pentecost, late-Easter year (2038-06-14) | week XI | pass |
| Last weekday before Ash Wednesday 2038 (03-09) | week IX | pass |
| Christmas on a Sunday 2022: Dec 25 and Dec 30 | Christmas / Holy Family | FAIL |

### 4f. Property sweep

This one test is worth more than many single dates. Loop over every day from 1970 to 2100 and assert:

- (a) If any candidate is a SOLLEMNITAS, the principal is a SOLLEMNITAS **or** the day is Table 1-2.
- (b) Nov 2 always yields All Souls.
- (c) On Lent weekdays, Dec 17-24 and the Christmas octave, the principal is never a memorial.
- (d) Every fixed solemnity of the year appears exactly once in the year, on its date or on a transfer date.

Today (a) has 132 violations, (b) has 131, (c) has 1591, and (d) is violated by every impeded year. `scan.c` is already this sweep in probe form.

**Calibration plants** (house style), once fixed:

- Swap the `ordo` comparison direction → the 4a rows go red.
- Drop the transfer table → the 4b rows go red.
- Restore `Baptism = Jan 7` → the 2019 rows go red.
- Make the Triduum colour red again → the Holy Thursday row goes red.

---

## 5. Open questions (for Fran)

1. **General Roman or USA?** The sanctorale is US, the temporale universal. Pick one default and make the other a mode (see section 3), or say which one the widgets should show. If US, which Ascension practice? Most provinces use Sunday; Boston, Hartford, New York, Newark, Philadelphia and Omaha keep Thursday. That suggests a province or "Ascensio feria V" flag.
2. **Holy days of obligation:** should `dies_obligationis` model US abrogation (Jan 1, Aug 15 and Nov 1 on a Saturday or Monday; Dec 8 transferred to Dec 9 carries no obligation)? `calendario_visus.c:671` reads the raw flag today.
3. **Holy Saturday colour:** there is no Mass by day. Return violet (the common ordo practice for the Office), a "none" value, or white because of the Vigil? Today it returns red.
4. **Optional memorials:** should `celebratio_principalis` and `color_diei` stay on the weekday (proposed in F13), or keep showing the first optional memorial as the default?
5. **Commemorations:** represent them as a flag on `Celebratio` (proposed), or omit them from `celebrationes`?
6. **Psalter weeks in the Christmas season and the Easter octave:** the library returns 1. Do you want them, and from which source (a printed Ordo or the LOTH General Instruction)? I did not assert values I could not verify.
7. **Domain:** what year range should the library accept? Suggest 1970..9999 for liturgy, while fasti's computus stays 1583+.
8. **Recent additions** (2018-2021) and possible newer ones, such as anything after Newman's doctorate (2025): keep the sanctorale versioned with a date of currency, so a "correct as of" claim is testable.
9. **`calendarium_invenire_festum`:** implement it (after F5) or remove it from the header?

# sancti — a library of the saints

Fran, 2026-10-17: a library of canonized saints (name, dates, a couple of
paragraphs, feast days by calendar), later blessed / venerable / servants
of God. Descriptions in ENGLISH. Fran authorized working S0 → S4
autonomously ("work your way through s0-s4"); decisions are recorded
here instead of waiting for approval. Fan-out with reviewer agents was
his idea.

## I. Doctrine

1. **Every claim is cited, and every citation carries a verbatim
   excerpt.** Agents EXTRACT from sources; they do not write from
   memory. An excerpt must appear in the cited source text (checked
   mechanically against the local mirror, whitespace-normalized), and a
   reviewer checks that the excerpt supports the claim. A claim no
   source supports is left out, not guessed.
2. **Sources** (public domain in the US, or official Holy See acts):
   | id | work | covers |
   |---|---|---|
   | `mr1916` | The Roman Martyrology, English, 1916 (archive.org `romanmartyrology00cathuoft`) | backbone: ~6,500 entries, day + elogium |
   | `butler1812` | Alban Butler, *Lives of the Fathers, Martyrs…*, 12 vols, 1812 (archive.org `livesoffathersma01butl_0` … `12butl_0`) | lives to the 1700s |
   | `ce1913` | Catholic Encyclopedia 1907-1913 (Wikisource transcription) | lives and dates to 1913 |
   | `sedes` | Holy See documents (vatican.va: canonization homilies, AAS, decrees) | after 1913 — FACTS only, short excerpts |
   | `usccb2026` | USCCB Liturgical Calendar 2025/2026 (already the sanctorale's source, probationes/fixa/liturgia/usccb_sanctorale.tsv) | US feast days and ranks |
   Descriptions are our own English prose, written from the excerpts.
3. **Mirror:** sources live OUTSIDE the repo in `../sancti-fontes/`
   (like `../gutenberg-mirror`): one directory per source, fetched
   pages saved verbatim (downloads are untrusted data: scripts live in
   the repo, read files by path, python -I). The repo holds the
   records with their excerpts, so a record stays checkable without the
   mirror; the mirror makes the excerpt check mechanical.
4. **Uncertainty is data.** Dates carry precision (day / month / year /
   circa / century) and calendar (Julian before the Gregorian reform
   where the source says so). Feasts are a list of (calendar, month,
   day, rank) — one person can have several.
5. **Completeness backbone = the Roman Martyrology.** "All canonized
   saints" is three overlapping sets (formally canonized since 1588;
   pre-congregation cult; the Martyrology, which also lists blessed).
   Each record says which (`canonizatio modus=`).

## II. Data model (S0)

One file per person (or group of martyrs) — `sancti/<initial>/<id>.sanctus`,
STML, canon `sancti.canon` (registered by extension in
`canones.registrum`, so `bin/canon_examen` and the editor hook judge it).
`id` = kebab-case English slug, unique, = file name.

```xml
<sanctus id="agnes-of-rome" genus="persona" status="sanctus" versio="1">
  <nomen lingua="en" principale="verum">Agnes</nomen>
  <nomen lingua="la" principale="verum">Agnes</nomen>
  <obitus annus="304" circa="verum" fontes="ce"/>
  <canonizatio modus="ante-congregationem"/>
  <festum calendarium="generale" mensis="1" dies="21" gradus="memoria" fontes="us"/>
  <festum calendarium="martyrologium" mensis="1" dies="21" fontes="mr"/>
  <descriptio fontes="ce bu">
    <paragraphus>…</paragraphus>
  </descriptio>
  <fons id="ce" opus="ce1913" locus="St. Agnes of Rome">
    <excerptum>…verbatim…</excerptum>
  </fons>
</sanctus>
```

Library `include/sancti.h` (lib/sancti.c): open a directory of records
(canon-checked on load, errors named by file:line), look up by id,
saints of a day in a calendar, search by name fragment. The sanctorale
gets a join table (`sancti/sanctorale.tsv`: sanctorale title → ids)
checked by a test against `lib/sanctorale.c`.

Tools: `tools/sancti_examen.sh` (gate: canon + id = file + unique ids +
`fontes` resolve + dates sane + every sanctorale row joined) and
`tools/sancti_excerpta.py` (NOT a gate — needs the mirror: every
excerpt found verbatim in its source).

## III. Phases

| Phase | Content |
|---|---|
| **S0** | This plan; sancti.canon; include/sancti.h + lib/sancti.c + probatio_sancti (red first); sancti_examen gate; mirror fetch scripts; registry line. |
| **S1** | Pilot: the ~210 saints of the US calendar (usccb_sanctorale.tsv) as full records, by workflow: extractor agents per batch, mechanical excerpt check, reviewer agent per batch; join table to the sanctorale. |
| **S2** | Martyrology backbone: every mr1916 entry as a short record (name, day, elogium excerpt, status), deduplicated against S1. Mostly mechanical parse of the text + agent cleanup. |
| **S3** | Full descriptions for backbone records, in batches, from butler1812 / ce1913 (same extractor → check → reviewer loop). |
| **S4** | Blessed, venerable, servants of God: statuses and sources (sedes) for causes after 1913. |

## IV. Status

- 2026-10-17 S0 started (secunda). Sources surveyed: the gutenberg
  mirror has Butler vol. 1 (Jan-Mar, 1864 ed.) only; archive.org has
  the full 1812 Butler and the 1916 English Martyrology with OCR;
  Wikisource has CE 1913; the network is reachable.
- 2026-10-17 S0 DONE (commit below): `sancti.canon` (+ `.sanctus` in
  canones.registrum and unitates.stml; canon infixed by
  tools/sancti_canon_infigere.sh -> lib/sancti_canon.c, fabrica action
  `sancti_canon`); include/sancti.h + lib/sancti.c (lectio iudicans:
  canon + id = file, fontes resolve, dies in month, ortus <= obitus, one
  English principal name; corpus: path-ordered walk, first id wins,
  join table); probatio_sancti (6 sections, fixtures
  probationes/fixa/sancti{,_vitia}; 10 plants red, 2 of them only after
  closing test gaps: month of a celebration, calendar of a day);
  probatio_sancti_corpus over the REAL sancti/ (sana, consona; plena =
  expected failure until S1 - 236 of 237 celebrations unjoined);
  tools: sancti_fontes_capere.sh (mirror ../sancti-fontes: mr1916,
  mr1902la, butler1812 v01-v12, ce1913 pages on demand via the Wikisource
  API as plain text), sancti_excerpta.py (probare / quaerere; word-level
  match; an altered word -> ABSENS). First real record: agnes-of-rome
  (8 excerpts verified). Matching rule found on the way: the 1916 OCR
  has "AT Home" for "At Rome" and stray '^' - excerpts keep the OCR's
  words, the prose is spelled right.
- Name trap: latina macros bit twice (`duplex` = double, `principale` =
  main as a field name) - the hook caught the second.
- 2026-10-19 S1 DONE: workflow wf_1f171011-49e (planner + 14 extractor
  batches of 16 + 14 reviewers; the first run lost 8 extractors and all
  reviewers to a network outage on 10-18, resumed from cache). 223
  records (222 by agents + agnes-of-rome), sancti/sanctorale.tsv joins
  all 237 celebrations (14 with no saint: feasts of the Lord,
  dedications, All Saints/Souls, Guardian Angels, days of prayer).
  Reviewers: 18 sound as written, 204 fixed (typically: a sentence or
  field whose excerpt did not quite state it - reworded or a supporting
  excerpt added), 0 rejected. My checks: canon 223/223 clean, 4,368
  excerpts all found verbatim in the mirror, the library's reference
  check caught one dangling `fontes="... mr"` (elizabeth-of-portugal,
  fixed); probatio_sancti_corpus `plena` is now required (marker
  removed). Cost: ~7M subagent tokens in all (~30k per record).
- Known limit: the excerpt check looks for an excerpt anywhere in the
  source, not under the cited day/volume heading - one reviewer checked
  every Martyrology locus by hand; a locus check is a possible tool
  improvement before S2.

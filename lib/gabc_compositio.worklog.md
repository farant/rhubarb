# gabc_compositio worklog

## 2026-11-04 — P3: first implementation (gabc part 2)

**Shape of the code.** One `Compositor` walks the syllables. Each
syllable is first composed into FRUSTA (chunks): runs of elements up to a
breakable spatium (` `, `/0`, `/`, `//`, `/[n]`) or a FRACTIO; `!` and
`/!` only widen the gap inside a chunk. Chunk coordinates: x from the
chunk start, y from the system's bottom line (= 0, up is negative,
`y(p) = -(p-3)*s/2`). Then the chunks are placed into the current
system; a full or broken system is FINISHED: custos, justification,
hyphens, text row, staff lines, page. Note boxes ride along as internal
picta (genus PICTUM_CAPSA) so every move applies to them too; they are
filtered into `notae[]` at the end.

**Fallback for figures gabc_figura does not name: greedy segmentation.**
`gabc_figura` only names a neume when the WHOLE run matches one table
row, so `ghf'fd`, `jjj`, `ef'gh...` come back IGNOTA. Instead of the
header's "each head side by side" fallback, the run is cut into the
longest known figure from the left (sub-ranges passed to gabc_figura as
a copied element), single notes drawn alone; s/II between segments at
the same pitch, s/IV otherwise. `ghffd` = torculus + clivis — what
Gregorio draws. Never drops a note. (Ruling in the plan ledger.)

**Two cursors, not one.** The header said one cursor (notes and text
both >= max(notes end + s, text end + gap)). That over-spaces a long
text under a single note. Notes and text each have their own cursor;
the syllable origin is the smallest shift that satisfies both. A
syllable without text pushes the text cursor to its notes' end, so text
never slides under a bar or clef. (Ruling.)

**Bars carry no margins of their own.** The header's "s*3/4 before and
after a bar" doubled up with the inter-syllable gap s (bars are almost
always their own syllable `(;)`), so bars are k wide (finalis 2k + s/2)
and rely on the normal gaps. (Ruling.)

**Text row is per system**: max(s below the bottom line, lowest note
pictum + s/4). On the single-line Salve the clivis stem in `rí(dc)`
(down to the middle of the space under c) is the lowest thing, so the
row is 5 px lower than a first-three-syllables hand estimate gave — the
test comment shows the derivation.

**Custos reserve** (s/2 + custos width) is always kept at the line end,
even when no note follows; lines are at most 8 px under-filled at
spatium VIII before justification hides it.

**Not in v1:** hyphen at a line end when a word continues on the next
system; second clef (`@c4`) is drawn but not repeated at line starts;
Dominican bars drawn as minor; text styles other than rubric colour
(one fixed font); translations and verbatim text.

**Tests** use inline gabc strings and RELATIVE neume dumps (pieces and
rectangles of one syllable relative to its first piece), so hand
expectations stay short. Hand-derived: 16 neume dumps, 9 sign dumps,
bar heights, vowel centring (7 cases), word/syllable gaps, hyphen x,
58 puncta on a full first line, z/Z/z- custos, ledger lines, clef
change, cb3, custos before clef change, clicks, Salve first system.
Pinned: `probationes/fixa/gabc/compositio/salve.compositio` (546 lines;
`GABC_COMPOSITIO_SCRIBERE=1` rewrites it) and corpus totals (82 chants
at VI, VIII, XXIV: 473 pages, 109925 picta, nothing out of bounds).

**Plants** (silva.planta, radix/gabc_compositio, all red then green):
custos computed through the old clef; vowel centred on the last note
(caught ONLY by the pinned Salve dump — the centring tests use single
notes); greedy fill one syllable too many; pes connecting line on the
left.

## 2026-11-04 — P4 changes from looking at real renders

Two rules changed after neumae -scala 2 renders (Salve, Dies irae,
Ingrediente): hyphen between syllables of one word now at a gap of half
a cell (a few px gap without hyphen read as two words); ligature stems
(clivis, porrectus, ancus) end s/2 + s/4 below the lower note instead
of using the virga's rule (which ran a whole space below the head).
Rubric runs may carry `. , :` ("℣."), but need at least one rubric sign.
GabcCompositio carries `forma`. Salve dump and corpus totals re-pinned
(471 pages, 111800 picta).

## 2026-11-06 — final review: split syllable text, clef-only first line

Two defects found by the whole-branch review (Review Focus 3), both in
`syllabam_ponere`:

- **Text of a syllable broken mid-melisma landed on the LAST line.** The
  text was emitted after the frusta loop, and `frangere()` inside the
  loop had already started a new system, so `co->systema` was the last
  continuation line while x was computed on the first. Spec G7: text
  stays with the first part. Now `textum_syllabae_ponere` runs right
  after the first frustum is placed (before any mid-syllable break);
  `supra_emittere` moved with it (same bug). After a mid-syllable break
  `systema_incipere` resets `textus_finis`, so the next syllable's text
  starts fresh on the continuation line.
- **Clef-only syllable made the first line "full".** `(c4)` places a
  frustum, which set `plenum`, so a too-wide first syllable broke BEFORE
  itself and left line 1 with only a clef and a custos. New
  `co->cantatum` (notes, alteration, bar or text — not a bare clef or
  custos; `Frustum.cantatum` from `frusta_aedificare`) guards the
  "move the whole syllable to the next line" check. `plenum` still
  decides whether a system is emitted and the mid-syllable breaks.

Salve dump changed in order only (text now follows its first frustum,
same coordinates). Corpus: 111800 -> 111789 picta, pages unchanged —
all at spatium 24, 8 chants; e.g. 03895 p2: "ló" now under its
melisma's first notes on line 2, and the stray hyphen it used to make
with "rum" on line 3 is gone. No corpus chant hit the clef-only case.
Test: `_latitudinem_probare` (melisma first syllable starts on system
0 with its text; "Al le lu(40)ia": text on its first note's system).

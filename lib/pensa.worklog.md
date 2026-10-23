# pensa worklog

## 2026-10-23 - K2: outline, items for a day, ticks

**Outline rule** (revised with Fran during K2; spec §IV.1). Text is split
into lines; blank lines separate SECTIONS (a page break of the book is a
blank line, see `scriba_liber_textum_legere`). Each section is dedented
on its own (minimum indentation of its lines; tab = 4 columns), so a
whole section may sit indented and only relative indentation matters.
Inside a section a top-level line WITH more-indented lines under it is a
schedule heading (parsed by recursus) and every deeper line is its item
(any depth flattens to items). A top-level line WITHOUT children is an
every-day item and is NEVER parsed as a schedule - "weekends" alone is an
item called "weekends". Consecutive childless lines share one implicit
every-day group (`schedula` empty, canonical "every day"); a heading ends
it, so the next childless line opens a new one and groups keep text
order. Test `a / mondays / x / b` pins the reset inside ONE section -
every earlier case crossed a section boundary, which resets it anyway,
so the reset plant stayed green until that case existed.

A heading that does not parse keeps its items and its `vitium`; it
contributes nothing to any day (the read view will show the error under
it). Corollary that bit a test: in `"  a\nb\n  c"` the "b" has a child,
so it is a heading, and "b" is not a schedule - an error group, by design.

**Items for a day**: groups in order whose recursus contains the day,
items deduplicated by text (the same item in "every day" and "weekends"
shows once on a Saturday).

**Ticks are plain acts**, genus `pensa`, no historia/base machinery:
`<pensum actio="factum|infectum" dies="YYYY-MM-DD">item</pensum>`. An
untick is just another act; the last act per (day, item) wins.
`PensaDocumentum` keeps a hash `ISO + "\x1f" + item` -> tick state and the
seq of the last act read, and reads new acts BEFORE EVERY QUERY, so other
documents, panes and processes on the same volume are seen without any
notification. `pensa_ponere` appends nothing when the state would not
change (the act count is asserted).

**Item text is text CONTENT, not an attribute** (K1 lesson: STML
attribute values are raw, `&quot;`/`&amp;` are not decoded there; text
content is escaped on write and unescaped on read). Test item
`R&D "notes" <x>` round-trips. Item text is trimmed on write and on
query. The trim on READ (around `stml_textus_internus`) is a guard only:
its plant stayed green because our writer never pads the text.

## 2026-10-23 — K3 Task 1: scratch pool (scheda)

The calendar recomposes and redraws every frame, so one long-lived
`PensaDocumentum` gets `pensa_factum` many times per second. Before this
change every query allocated its key (ISO date + "\x1f" + item) and every
`acta_legere` its act list in `doc->piscina`: 100 query pairs grew the
pool 14.7 KB -> 199.5 KB. Now a per-document `scheda` (dynamic pool,
vacated at the top of every `acta_legere`) holds the act list, the parsed
STML, query keys and the act `pensa_ponere` builds. Only a key the table
has never seen is transcribed into `doc->piscina` (`tabula_dispersa`
holds keys BY REFERENCE and keeps the old key on update), so a toggled
item costs nothing after its first tick.

Two things the memory test had to step around, both not pensa's:
- `volumen_actum_super_appendere` allocates `genus_ch` with
  `chorda_ex_literis(genus, volumen->piscina)` on EVERY append (8 B for
  "pensa"). User-paced, so harmless here, but a volume that appends in a
  loop grows its pool. The test gives the volume its own pool.
- The first "infectum" act interns the word once (shared interner): one
  warm-up pair before measuring.

# paginatio.worklog.md

## 2026-08-01 — birth: page designators are not integers

Fran, on the reading-notes editor: *"some books have page numbers
that start as roman numerals and then are normal integers starting
from 1 later... right now page number has to be an integer so it
doesn't work. I would probably just create a page number field type
if possible."*

He's right that this is a type, not a formatting problem. A real
book carries TWO page series — front matter `i…xlii`, body `1…380` —
and `xii` and `12` are DIFFERENT PAGES of the same book. An integer
field cannot express that, so notes on front matter were simply
unrecordable.

### Why a library and not a regex in the app

The strict roman parser already existed, in `capitula.c`, written
for chapter inscriptions. A second consumer arrived, so it was
promoted to `numerus_romanus` — its own library, because a roman
numeral is neither a chapter nor a page. `paginatio` sits on top and
owns only the page-designator grammar.

The migration carried its tests with it (`probatio_paginatio.c`),
which is the point: if promotion had quietly loosened the strict
subtractive rule, those cases would have caught it. Both suites were
run before and after.

### The property that made this safe on live data

The `pagina` type accepts **an integer as well as a string**. That
makes it a strict SUPERSET of `numerus`, which means retyping
`adnotatio.pagina` from `numerus` to `pagina` cannot orphan the
annotations already written with integer pages. Measured after the
change on Fran's store: `insalubres 0`, with 16 annotations already
in place.

This is why the additive-alignment helper grew an explicit
`retypatio: true` flag rather than silently applying type changes.
The flag is the caller ASSERTING that the new type contains the old
one; without it, `aequare` still refuses to change a type, which is
the genus layer's own law.

### Sorting is the whole reason the type exists

`paginatio_clavis` maps a designator to one integer: roman into a
low band, arabic into a high one. So front matter sorts before body,
each numerically, and every sorter — store, webview, anything later
— compares the same number and cannot disagree. Absent/invalid sorts
LAST so an unpaginated note doesn't interrupt paginated ones.

Strictness follows the inscriptio law: `12a`, `p. 12`, `-5`, `0` are
all refused. A lenient reader would place a page at the wrong point
in the order, and a wrong order attaches a note to the wrong place —
silently.

### The twin

Sorting happens client-side, so the JS carries a twin of this
library. C is the oracle; the cases are duplicated verbatim between
`probatio_paginatio.c` and `probatio_fori.js` because a twin without
shared cases diverges silently — and divergence here means a page in
the wrong order, which nobody notices until the note index looks
absurd.

### Harness note (second time today)

The fake DOM's `textContent` returned only explicitly-set text, not
the concatenation of children — so a node built from spans read as
`""` and two assertions went red while the code painted correctly.
Fixed the harness to concatenate, as a real DOM does.

Also caught a VACUOUS assertion by calibrating: "false booleans are
omitted" passed even with the skip removed, because no real species
declares a boolean field. Replaced with a direct test against a
synthetic species — which then failed correctly under the same
calibration.

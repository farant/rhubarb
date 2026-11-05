# chronica_nomina worklog

## 2026-10-30 - C5a declaration scanner (Opus 5.5)

`<person>` / `<place>` declarations (Fran: "another way to declare
people"; for now they only declare existence). The tag REGION is found
by hand, then only that region goes through STML (stml_legere +
stml_distribuere), so prose around it (`a < b`, `AT&T`) is never
parsed. Fran's nested forms come from STML DISTRIBUTIO (reference 18)
for free: `<person><>John</><>Greg</></>` and
`<person><(>John<(>Greg</>` both become two person elements; wrapper
attributes carry down and an item's own wins.

**Region finder (regionem_finire):** a depth count over tags. An open
tag ('<' + letter, '#', '>', '(', '@', '.') counts +1, '</...>' counts
-1, a self-closing tag ('/' before '>') counts 0, and a capture ('('
before '>', e.g. `<(>`, `<# (>`) counts 0. Comments are skipped, and
quotes are respected when finding '>'. A '<' followed by a space or a
digit is prose, not a tag. A capture at depth 0 (`<person (>`) is
refused as "forma non admissa", since inside running prose it is
unclear where it would end.

**Signedness bug the compiler caught:** claudens() first returned -1
through an unsigned i32, so `k < 0` would never have been true and an
unclosed tag would run off. Positions in the finder are s32 now.

**Item positions:** after distribution STML keeps each item's `linea`
(1 = first line of the region) but not its byte offset (positus).
So the declaration line = region line + linea - 1, and the column =
the first non-blank on that line, or the tag's own column when the
item shares the opening line. Jump-home from "Greg" lands on Greg's
line.

**Gathering (chronica_fontes):** each book's projection
(EventaLibri) now also holds declarations, so editing a page refreshes
both events and declarations, and codificare adds "D ..." lines. The
table gains `declarationes` (copied like res) plus `personae` / `loca`
(ChronicaEntitas). These are built by collecting names from event
people= / place= (eventa 1 each) and from declarations (declarationes
1 each), stable-sorting by bytes, then merging equal neighbours and
summing the counts.

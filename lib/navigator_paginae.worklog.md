# navigator_paginae worklog

## 2026-10-27 - N3.1b link index (Fran's idea)

Second flow `nexus` in NavigatorTextus: "Links" + one PARAGRAPHUS per
distinct link target (dedup on URL without #anchor, first appearance
wins, the note keeps the first URL WITH its anchor). Same-host
`/wiki/T` -> decoded title with '_' -> space; anything else
"host: decoded path" (other Wikipedia languages included - their titles
are still recognisable in the path). The pane paginates the two flows
separately and concatenates the page lists (`paginae_articuli` marks
the boundary; drawing picks the text by page index), so the index
always starts on a fresh page and j/k flows into it.
Rhubarb: 136 targets = 6 pages at 500x600 with paragraph spacing -
denser (single spacing, or two columns) is an obvious later tweak.

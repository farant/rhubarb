
## 2026-10-27 - page-break rules (widows, orphans, titles)

Fran (navigator, applies to EPUBs too): no lone first line of a
paragraph at a page bottom, no lone last line at a page top, titles
never split, a title never stranded at the bottom - and a run of
titles ("Part II" + "Chapter 3") moves together.

lector_paginare is now two passes: measure every clausula into lines
(wrapping depends only on width; `segmentum_involvere` appends to an
Xar instead of placing), then place block by block with look-ahead.
`capiunt` simulates lineam_ponere's arithmetic (gap before the first
line unless the page is empty; an empty page always takes one line).
Paragraph/verse: split at k only if k >= 2 and r - k >= 2, so blocks
of <= 3 lines never split. Title: the GROUP = consecutive titles +
`servandae` lines of the next block (2, or all if <= 3 - must match
what the paragraph rule will actually keep, else the paragraph jumps
and strands the title anyway). Group doesn't fit here but would on a
fresh page -> new page; fits nowhere -> at least keep the title whole;
title taller than a page -> flows (lineam_ponere breaks).

Escape hatch for tiny pages: on an empty page that can't satisfy the
rules, place as many lines as fit (the 4 px test still gives one line
per page, no hang). Existing lector/navigator suites needed no
changes: none pinned page counts that the rules moved. Saved reading
positions are byte offsets, so they map onto the new pages.

# exemplaria worklog

## 2026-10-09 - born for the focus indicator

Moved verbatim out of lib/delineare.c (the legacy painter keeps using
it through `exemplar_obtinere`). Bit order: byte = row, 0x80 = x 0
(the legacy painter's `I << (VII - px)`). `exemplar_punctum` tiles by
`& VII`, so negative coordinates wrap correctly (-1 = column 7). The id
is `i32` (unsigned), so "negative id" is not a case. Gotcha when adding
a library that the legacy painter uses: three build lists are written
or generated outside the root suite - ludus_tessera and tessera
compile_probationes.sh (by hand) and tessera/musivum_fontes_generata.sh
(regenerate with the command in its header).

# Fixa md — fontes externi

| plagula | origo | versio | octeti | exempla |
|---|---|---|---|---|
| `commonmark/spec.txt` | https://spec.commonmark.org/0.31.2/spec.txt | CommonMark 0.31.2 | 204,857 | 652 |
| `gfm/spec.txt` | https://raw.githubusercontent.com/github/cmark-gfm/master/test/spec.txt | GFM 0.29 (2019-04-06) | 216,680 | 672 |

Vendored 2026-09-03 (A6). Byte counts and example counts are PINNED in
`probatio_md_exempla`; a re-vendor is a named change to that gate. The
example fence form (32 backticks + ` example`, `.` separator, tabs as
`→`) is read by `md/fontes/md_exempla.c` — one reader for both files.
GFM sections judged by the html oracle (B3): Tables, Task list items,
Strikethrough, Autolinks (extension); "Disallowed Raw HTML" is not in
the dialect (spec §0).

## Fixa md (snapshots, 2026-09-03, A8c)

`md/spica.md` (project-specs/md-arbor-spica.md), `md/episodium.md`
(episodes/1x01-piscina-capacitas-nova.md), `md/piscina.md`
(docs/piscina.md), `md/sabaw.md` (SABAW2.md) — copies frozen for the
computus golden (`computus/basis.tsv`) and the totality gate, so that
the pinned numbers never move because a living document was edited.

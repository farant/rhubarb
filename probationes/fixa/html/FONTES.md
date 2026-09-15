# Fixa html — fontes externi

| directorium | origo | versio | plagulae | octeti | exempla |
|---|---|---|---|---|---|
| `html5lib/` | html5lib-tests `tree-construction/*.dat`, copied from the LOCAL Go module cache `golang.org/x/net@v0.30.0/html/testdata/webkit/` (no network) | x/net v0.30.0 snapshot, 2024-10 | 54 | 428,448 | 1,708 (184 `#document-fragment`, 8 `#script-on`, 27 `#script-off`) |

Vendored 2026-09-15 (oracle task O1; Fran chose the local copy over an
upstream fetch). Licence: `html5lib/README` verbatim — the WebKit BSD
notice; the files reached Go through WebKit's LayoutTests. The
`scripted/` subdirectory (3 files that need script execution) was NOT
copied. File count, byte count and case counts are PINNED in
`probatio_html_exempla`; a re-vendor from upstream
(github.com/html5lib/html5lib-tests, MIT, a few cases newer) is a
named change to that gate.

Reader: `html/fontes/html_exempla.c`, one reader for all 54 files,
mirroring html5lib's own `support.py` TestData: a line whose stripped
form starts with `#` is a section heading; `#data` opens a case; every
section loses ONE trailing newline; the last section before the next
`#data` loses one more (the blank separator line). So an input that
ends in a blank line is an input that ends in `\n` (6 such cases).

Three files (`domjs-unsafe`, `plain-text-unsafe`,
`pending-spec-changes-plain-text-unsafe`) contain NUL bytes. `grep`
without `-a` treats them as binary and counts ZERO cases in them: the
first tally of this corpus said 1,625 for that reason. The reader
works on bytes and sees all 83.

Expected-tree format (`#document`): lines `| ` + two spaces per depth
level; `<tag>` lowercase (foreign: `<svg svg>`, `<math math>`);
attributes alphabetical on their own lines as `name="value"` with the
value DECODED and unquoted; text `"..."` with entities decoded;
comments `<!-- x -->`; `<!DOCTYPE html>`; template children under a
`content` pseudo-node. The expected trees SYNTHESIZE `html`, `head`,
`body` (the simple builder never does — spec §11 H4), and their values
are COOKED (ours are byte-faithful): the oracle gate derives a cooked
view and unwraps (ledger 01M2KPNTT0).

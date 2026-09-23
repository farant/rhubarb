# internamentum.worklog.md

## 2026-09-22 — "" is REFUSED, and that is a contract

The json audit found `{"":1}` planted a NIHIL key because
`chorda_internare` returns NIHIL for the empty string (the hash table
refuses zero-length keys). First fix attempted HERE: a canonical ""
kept outside the table. The radix gate refused it — three suites red:

- `lib/actio.c:117` and `lib/insula.c:527` intern a name and treat NIHIL
  as "empty name refused" (commented as such).
- STML: a tag `< >` parses to a node whose `titulus` is NIHIL — the
  modelled "empty title" state (`lib/stml.c` ~2357, CUSTOS NIHIL
  01M171YAEP); `stml_strictum`/`stml_html` judge it, ~177 sites check
  `titulus == NIHIL` across lib/silva/materia. probatio_stml_html fell
  first; the other suites never ran.

So "empty → NIHIL" was a load-bearing contract that the header never
stated. Reverted. The contract is now WRITTEN in internamentum.h (with
its consumers named) and PINNED by a test block in
probatio_internamentum.c (both entry points, datum NIHIL and non-NIHIL,
est_internata, numerus). Plant: making `chorda_internare_ex_literis("")`
return non-NIHIL turned the pin red at :287, green on revert.

json keeps its own canonical "" instead (lib/json.worklog.md).

Lesson: an "obvious" fix to a shared library's edge case is a
contract change until the consumers say otherwise — grep for code that
USES the edge as a signal, not just code that trips on it.

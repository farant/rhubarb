# html_arbor — worklog

## 2026-09-15 — H6: the substrate limit the plan predicted, found in its true shape

The plan named one place this client could pay a substrate change: a
text token containing the literal closing tag of its own lexeme
element, which the STML raw form cannot carry. It fired, but not where
I expected.

- `<p>textus cum </lex-textus> intra se</p>` passes. The HTML lexer
  reads `</lex-textus>` as an END TAG, so the string never sits inside
  one `textus` token; it becomes an `elementum-malum` and round-trips.
  Bytes and tree both identical.
- `<script>var t = '</lex-textus-crudus>';</script>` is REFUSED by
  `materia_arbor_scribere_nodum`: the raw-text token is one token by
  the lexer's contract, its value contains `</lex-textus-crudus>`, and
  `_valorem_crudum_notare` refuses ("valor sequentiam claudentem
  fert"). Same for `<!-- </lex-commentarium> -->` inside a comment
  token.

Why materia insists on raw: values are "octeti codicis, non prosa" —
a pretty-printer re-fills plain text, so code bytes must be raw. When
raw is impossible the writer refuses the whole document rather than
falling back to escaped text. md met the same wall and split its token
values at `</md-` by hand; html cannot split a token (one owner per
byte, and the lexer's raw-text token is one token).

Pinned in `probatio_html_stml` as the CURRENT behaviour (the refusal
and its exact cause asserted), so that a materia fallback flips the
pin red and the two cases move into the round-trip list. Filed as a
materia question in the ledger; Fran's decision. Reachable only by
adversarial input (a script or comment containing the projection's own
closing tag), never by an ordinary page — the fourteen-file corpus
passed both cycles and the comparator.

**Measured on the side, worth keeping:** the STML projection of the
corpus is 15× the source (162,775 → 2,384,563 bytes). The 200-deep
nesting fixture is 287× (3,475 → 996,225): pretty indentation grows
with depth, so a deep tree's projection is quadratic in depth. H9's
computus will pin the counts; this is the number oratio's "projection
cost" conversation was about, seen from a different client.

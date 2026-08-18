# html_lexema worklog

## 2026-08-18 — initial implementation (design decisions)

Shipped in one pass, 261/261 first run. The interesting decisions,
recorded because they are invisible in the code unless you know the
HTML5 spec's corners:

**Mode machine, not token-local dispatch.** Unlike CSS, HTML cannot be
lexed one token at a time from byte class alone: after `<div` you are
in attribute territory, after `<script ...>` everything is raw text
until `</script`. The lexer carries `modus` (data / intra-tag /
crudus) plus the pending tag name across tokens. Totality argument is
the same as css_lexema's: every path through `_lexema_consumere`
advances the cursor by ≥1 byte, so tokens tile the source and
emission == concatenation.

**Order of checks in `_intra_tag_consumere` is load-bearing.** The
post-`=` value check sits BEFORE the `/>` check but AFTER bare `>`.
This is what makes the two famous gotchas come out browser-correct:
`<a href=/>` → value `/`, tag ends normally (NOT self-closing);
`<a href=x/>` → value `x/` (slash is a legal unquoted-value char).
Swap the order and both silently become self-closing tags.

**`</>` emits PRAVUM where the spec emits NOTHING.** The spec's
end-tag-open state on `>` says "parse error, switch to data,
emit nothing" — a token stream with a hole in it. Byte fidelity
forbids holes, so `</>` (and `</3>`, `<?...>`, `<!x>`) become
COMMENTARIUM_PRAVUM: loud, named, span-exact. Clean house files must
carry zero of them (corpus gate counts PRAVUM alongside IMPERFECTA).

**Script double-escape machinery deliberately skipped.** The spec's
"script data double escaped" states (the legacy
`<script><!-- document.write('<script>..</script>') --></script>`
dance) are not implemented — `</script` always closes. This is the
"tractable half" boundary from the spec doc. If a real file ever
trips it, the corpus gate will show a premature close (imperfecta
downstream), which is the signal to revisit.

**Doctype = one token to the first `>` IS the spec rule.** I expected
this to be a simplification, but the spec's abrupt-doctype rules end
the doctype at `>` even inside quoted public/system identifiers. One
rule, spec-conformant.

**REFERENTIA is strict-form only.** `&name;` / `&#dd;` / `&#xhh;`
with a 64-byte lookahead cap (longest real named ref is 33). No
semicolon → plain text, no token. Refs are only split out in DATA
mode; attribute values and RCDATA keep them verbatim inside the
larger token (cooking is the arbor's job, per the trio contract).

**RCDATA lexed identically to RAWTEXT.** title/textarea vs
script/style differ only in cooking (char refs), not in boundaries,
so one `MODUS_CRUDUS` serves all four. The distinction is recorded
here for the future html_arbor.

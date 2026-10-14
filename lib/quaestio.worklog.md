# quaestio.worklog.md

## 2026-10-13 - origin: $question in scriba (Fran)

Fran: "$question<>...</>" in a scriba page asks Claude and writes the
answer into the page's free space. quaestio is the ONLY place that
talks to vates for it, so vicus never links vates/http (the terminal
twin and ludus_tessera stay free of the TLS stack - checked in the
aedilis manifests: vicus_terminalis has none, the window app has
http.c, vates.c, quaestio.c).

- Request: model QUAESTIO_EXEMPLAR (claude-opus-5-5, Fran's choice for
  v1; later maybe $question<opus>/<haiku>), system prompt states the
  character budget and the page width and asks for plain ASCII with no
  Markdown; max_tokens = characters / 3 + 64 (tokens ~ chars/4, margin
  for long words). One user message = the body verbatim.
- Response: all TEXT blocks concatenated, then reduced to ASCII (scriba
  cells are single bytes): common Unicode punctuation mapped (curly
  quotes, dashes, ellipsis, nbsp, bullet), other multi-byte -> '?',
  '\r' dropped, other control bytes -> space. Empty text -> error
  ("recusatio" if the stop reason was refusal).
- GOTCHA: VatesResponsum.bloci holds VatesBlocus STRUCTS, not
  pointers - the header comment said "VatesBlocus*" (fixed in
  vates.h). Dereferencing as a pointer gave empty text silently.
- Errors keep vates' description plus the provider's message
  ("... : Overloaded").
- Tests use the fake provider with sine_herbario = VERUM and
  conatus_maximi = 1 (no writes outside the build, no retry waits).
  Real questions from the window app keep vates' default herbarium
  capture (~/.rhubarb/herbarium), as agreed.
- No prices: vates prices only from a caller table; none for Opus 5.5
  yet, so the status line shows tokens only.

## 2026-10-13 - Markdown flattened with the house parser (Fran: ragged lines, flat code)

Live test: the answer came back as Markdown with its own line breaks
(and hard-wrapped at the model's guess of a width), and our wrap broke
it again -> "start of line / end / next line"; code lost all
indentation because the old layout skipped runs of spaces and ad_ascii
turned tabs into one space. "No Markdown" in the system prompt was not
enough. Now: ad_ascii keeps '\t'; markdown_planare parses with
md_arbor_parsare (first user of the md parser outside md/; the root
suite gained md_arbor, md_inlinea, md_lexema, md_lexicon, md_decoctum,
md_registrum_coctum) and emits LOGICAL LINES:
- paragraph = one line: soft/hard breaks -> space, emphasis/strong
  delimiters and link syntax dropped (text kept), code spans keep their
  backticks, escapes lose the backslash; whitespace collapsed.
- heading = "# " + text (one '#' whatever the level - Fran).
- the paragraph's own PREFIX tokens carry list/quote nesting: indent
  -> spaces, bullet -> "- ", "N. " and "> " kept. So nested lists and
  quotes come out right without walking containers specially.
- fenced/indented code: one line per code line, leading '\t' sentinel,
  only MD_LEX_INDENTATIO_CODICIS (the code's OWN indentation, not the
  container's) expanded to spaces at 4-column stops, then the content.
- tables, html, rules: raw token text line by line, verbatim.
- blank lines only where the source had a LINEA_VACUA (tight lists stay
  tight).
The exact output for a sample answer is pinned in probatio_quaestio
"markdown" (hand-written expectation, matched first run).

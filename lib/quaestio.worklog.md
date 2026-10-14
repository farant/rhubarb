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

## 2026-10-14 — one planning path, line budget, stream, frames (quaestio-rivus T5)

- `quaestio_textum_planare` (= markdown_planare(ad_ascii(x))) is public:
  the non-streamed result, the streamed result and (T8) vicus' per-pulse
  re-layout all go through it, so the finished page cannot differ by
  path. `quaestio_rogare` / `_rivum` share `petitionem_struere` and
  `responsum_reddere`.
- Budget in LINES (Fran 2026-10-14: a paragraph's ragged last line and
  the blank line after it eat whole lines, so a character budget
  overshoots by ~40% on paragraphy answers): the prompt says "The page
  has L lines of W characters" plus the rules that eat space, and
  allows light Markdown (we reflow it since 4731ac17; the old "no
  Markdown" was ignored anyway). `max_tokens = L*W/3 + 64`, a backstop.
  API change: `characteres` -> `lineae`; the one caller
  (apps/vicus/vicus.c) converts its old character budget back to lines
  INTERIM until T8 replaces the port.
- `QuaestioResponsum.causa_finis` (raw stop reason) so the helper's F
  frame can say `max_tokens`.
- Frames `T <n>\n<bytes>`, `E <n>\n<bytes>`, `U <in> <out>\n`,
  `F <causa>\n`: reader returns PLURA for a partial frame, PRAVA for
  malformed headers (digits only). PLANT LESSON: the first plant (accept
  a T frame before all its bytes arrived) stayed GREEN - the test's
  prefix shares its buffer with the whole stream, so the over-read found
  the right bytes. Added "position never passes the prefix end"; then
  red. A split test over one shared buffer cannot see over-reads by
  content - assert positions.
- `quaestio_adiutor_currere`: T per delta immediately, then U + F, or
  E + `F error`; the F frame is always last (plant: F dropped -> red).

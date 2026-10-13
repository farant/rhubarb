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

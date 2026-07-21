# stml.c worklog

## 2026-07-20 — raw line capture `<tag! (>` (reader side)

Completed a feature the codebase had already half-built: a literal
`/* TODO: Implement line capture for raw tags */` sat in
`_tok_legere_tag`, the tokenizer already emitted CRUDUS tokens with
`captio_numerus > 0`, the raw-mode guard already excluded them from
scan-until-closing-tag, the parser's crudus branch already set both
flags and the CAPTIO_ANTE writer already emitted `<tag! (>` with
unescaped raw children. Even the token fields for the design
(`habet_captus`, `captus_contentus`) existed, initialized everywhere
and read nowhere. Surfaced by the aedilis Phase-0 stml specimens.

**Semantics (Fran, 2026-07-20)**: captures the rest of the line RAW
(no tags, no entities) up to but not including `\n`; trailing `\r`
trimmed; leading/trailing spaces of the content kept (raw is raw);
`((>` records captio_numerus=2 but still captures ONE line — the
multi-line + dedent question is deliberately deferred.

**The four changes**:
1. Tokenizer (`_tok_legere_tag`, the TODO branch): reads the
   post-`>` remainder of the line into `token.captus_contentus`
   (the pre-sketched fields), leaving `\n` to the normal stream.
2. Parser (`_parser_legere_elementum_crudus`): captured form
   attaches the text child from `captus_contentus` directly
   (interned raw — no unescape, no normalization) and returns.
   THE ACTUAL BUG lived here: the branch unconditionally consumed
   the next CLAUDERE token (correct for `<tag!>...</tag>`, wrong
   for captures which own no closing tag) — it devoured the
   PARENT's closing tag, and the parent then hit EOF →
   TAG_NON_CLAUSUM. That single consumption was why the whole
   feature appeared unimplemented.
3. Capture pass (`_processare_captiones`): ANTE branch now skips
   `crudus` nodes — their capture was satisfied at parse time;
   grabbing following siblings would double-capture.
4. Writer (CAPTIO_ANTE branch): in NON-pretty mode appends `\n`
   after the captured content — without it the following sibling
   is devoured on re-read. In pretty mode the parent's separator
   newline already serves (probatio pins both postures
   byte-stable).

Also corrected the stale stml.h header claim "No entity
references": text is escape/decode SYMMETRIC; attribute values are
raw both ways (`"` unrepresentable there); raw tags verbatim.

Tests: probatio_stml gained 7 blocks (asperities verbatim, the
devour regression pair, entities-intact, empty capture, `((>`
notation, dual-posture round-trip, constructor path — the aedilis
manifest lane). All green first run.

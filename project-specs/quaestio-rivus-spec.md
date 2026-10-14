# quaestio per rivum - streaming answers for $question (spec)

*Fran 2026-10-13, after $question shipped (4731ac17, ff 55cc51af):
"the blocking response + whatever library upgrades we would need to do
streaming responses would be worth thinking about". Question that
prompted it: scriba freezes for the whole request; the answer appears
all at once.*

Goal: **the answer grows on the page as it arrives**, scriba never
blocks, Esc cancels, and the finished page is byte-for-byte what the
non-streamed path produces today. The library work is reusable: the
streaming core does not care which transport feeds it.

Decisions (Fran, 2026-10-13/14):
- **Route A now, route B later.** A: a helper process `bin/quaestio`
  does a plain blocking streamed request; vicus pumps it each frame via
  `processus`. B (later, not in this spec): in-process on `reactor.h`
  with non-blocking DNS (`DNSServiceGetAddrInfo`, ledger decision
  …CYT9A8), connect and TLS.
- **The core is incremental "bytes in, events out" parsers**, so B
  reuses it unchanged (not a callback inside the blocking read loop,
  which would only serve A).
- **Page locked while streaming, Esc cancels**, the page painted in
  the desk color (`COLOR_SUPERFICIES`) as the lock signal.
- **Esc / error mid-stream = answered**, with a last line
  `[cancelled]` / `[error: <reason>]` in the page text; undo removes the
  whole answer in one step.
- **Budget in lines, not characters** (Fran: a paragraph's ragged last
  line and the blank line after it eat whole lines). The prompt states
  the page shape; the fit itself is measured on our side.
- **Light Markdown allowed in the prompt** (we reflow it since
  4731ac17; the old "no Markdown" instruction is ignored anyway).

## I. What exists (measured 2026-10-14, quarta 312fc0e3)

- `http_exsequi` (`lib/http.c:753`) reads until the connection closes,
  THEN finds the headers, THEN decodes chunked (`_decodificare_chunked`,
  `lib/http.c:190`). Nothing is visible before the end.
- Incremental parsing exists for REQUESTS only: `http_parser_adicere`
  (`include/http.h:377`, used by hospitium).
- Transport seam: `HttpVectura` (`include/http.h:219-222`) returns a
  whole `HttpResultus`. vates (`VatesOptiones.vectura`,
  `include/vates.h:252`), the fake provider, herbarium capture
  (`herbarium_vectura`) and replay (`herbarium_reddens`,
  `include/herbarium.h:17-22`) all sit on it.
- `vates_mittere` (`include/vates.h:293`) is one blocking call with
  retries. No SSE or `text/event-stream` code anywhere in the tree.
- `processus` has a non-blocking path (`include/processus.h:144-246`):
  `processus_incipere`, `processus_pulsare` (select with zero timeout),
  `processus_effusio_hactenus` (stdout SO FAR, readable while the child
  runs), `processus_abrumpere`. No threads by design.
- scriba runs a pulse every frame: `scribam_pulsare`
  (`lib/vicus_applicatio.c:702`) -> `scriba_reficere`
  (`lib/scriba_applicatio.c:245`) -> `scriba_iussum_dilatum_currere`
  (`lib/scriba_actiones.c:1254`), which today does stage I (paint the
  "asking..." message) and stage II (run the verb, `effectum_applicare`
  `lib/scriba_actiones.c:785`, one `scriba_documentum_committere`).
- The $question verb (`lib/vicus_applicatio.c:440-500`): budget
  `characteres = (l * w - 11) * 85 / 100`, port call
  `VicusPortaQuaestionis` (`include/vicus_applicatio.h:43`, blocking),
  fit `responsum_aptare(textus, w, l)` (`lib/vicus_applicatio.c:364`,
  already fits by LINES at sentence or logical-line end).
- Markdown planning and ASCII mapping live inside `quaestio_rogare`
  (`lib/quaestio.c`); the prompt (`lib/quaestio.c:641-644`) says "no
  Markdown ... at most N characters ... W columns wide";
  `max_tokens = N/3 + 64` (`lib/quaestio.c:640`).
- Page paint: `scriba_figura_folii` fills `COLOR_BACKGROUND`
  (`lib/scriba_figurae.c:182`), border `COLOR_BORDER` (medium-dark
  gray); the desk around it is `COLOR_SUPERFICIES` (medium gray,
  `lib/thema.c:86`).

## II. Layers

Each layer is a library with its own tests; none knows about scriba.

### II.1 `http_lector` - incremental HTTP response parser (new)

Bytes in, events out, no I/O. Events: status + headers ready; body
bytes (decoded); end. Handles `Content-Length`, `Transfer-Encoding:
chunked` (a chunk header, CRLF or chunk boundary may fall at ANY byte
split) and read-until-close (end only at the caller's EOF signal).
Limits like `http_parser_creare_cum_limitibus` (header size, count).
Malformed input = a named error state, never a crash.

Oracle: for every response in the test set, feeding it whole, byte by
byte, and at every 2-way split gives the same headers and the same body
as `http_exsequi`'s existing parse of the same bytes.

### II.2 `http_exsequi_rivus` + streaming transport seam

The existing connect / TLS / send code with the read loop feeding
`http_lector`; each body piece goes to a sink
`(vacuum* ctx, constans i8* octeti, i32 n)`; returns the final status
like `http_exsequi` (headers, status, error) with an empty body.
A streaming seam beside `HttpVectura` (same idea: function + datum),
so vates, the fake provider, herbarium capture and replay can stand in.
Herbarium capture records the raw streamed body (SSE text) as the
response body; replay feeds it back through the sink in pieces.

### II.3 `sse` - Server-Sent Events parser (new)

Incremental, per the WHATWG event-stream rules we need: `event:`,
`data:` (several lines joined with `\n`), `id:` and `retry:` parsed
and ignored, `:` comment lines ignored, a blank line dispatches,
CRLF / LF / CR line ends, a line split across feeds. Output: (event
name, data) pairs.

### II.4 vates streaming: `vates_mittere_rivum`

Same request as `vates_mittere` plus `"stream": true`; decodes
Anthropic's events: `message_start` (id, input usage),
`content_block_start` / `content_block_delta` (`text_delta`,
`thinking_delta`, `input_json_delta`) / `content_block_stop`,
`message_delta` (stop reason, output usage), `message_stop`, `ping`,
`error`. A callback receives text deltas (thinking deltas flagged
separately); the return value is the same assembled `VatesResponsum`
`vates_mittere` gives (blocks, stop reason, usage), so callers can
switch without losing anything. Retries only BEFORE the first delta
(after it, a failure is reported, not retried - the caller has already
shown text). The fake provider scripts a stream as a list of text
pieces (emitted as real SSE through the seam, so the decoder is
exercised). norma (schema drift) skips streamed responses in v1 -
follow-up, not silent: the capture is marked as streamed.

### II.5 `quaestio`: one planning pipeline, line budget, streaming

- `quaestio_textum_planare(crudum)` becomes public: ASCII mapping +
  Markdown -> logical lines (today's internal code, unchanged
  behavior). The non-streamed `quaestio_rogare` and the stream use the
  same function, so the final text is identical.
- Prompt (both paths): plain ASCII; light Markdown allowed (headings,
  lists, code blocks), it will be reflowed; "The page has L lines of W
  characters. Every paragraph starts on a new line, a blank line
  between paragraphs uses a whole line, and a short last line still
  uses the whole line, so prefer fewer, fuller paragraphs. Be direct."
  W = text width after margins (`w - 2 * QUAESTIO_MARGO`).
  `max_tokens = L * W / 3 + 64` (backstop only).
- `quaestio_rogare_rivum(vates, exemplar, quaestio, lineae, latitudo,
  callback, ctx, piscina)`: streams raw deltas to the callback.
- The API's parameters change from characters to lines in both
  functions (callers: the vicus port only).

### II.6 helper `bin/quaestio` + frame protocol

- `apps/quaestio/quaestio.c` (built by aedilis like vicus): reads
  `$HOME/.rhubarb/anthropic.clavis` itself (never argv, never stdout),
  arguments `-exemplar X -lineae L -latitudo W -quaestio <text>`, calls
  `quaestio_rogare_rivum`, writes FRAMES to stdout, exits 0 after the
  final frame.
- Frames (text, length-prefixed, readable in a log):
  `T <n>\n<n bytes>` text delta - `U <input> <output>\n` usage -
  `E <n>\n<n bytes>` error - `F <stop reason>\n` end.
  A frame reader (`quaestio_tabulas_legere`, incremental over
  `processus_effusio_hactenus` with a consumed offset) and writer live
  in the library; `main` only wires them to stdout. A helper that dies
  without `F` = error "helper exited without answer".
- vicus finds the helper next to its own executable (the tree's
  `bin/`, or `~/.bin/` once installed: `bin/quaestio` joins the
  installed set at merge time, main only).

### II.7 iussum: a command effect can be RUNNING

`IussumEffectus` gains `IussumOpus* opus` (NIHIL = synchronous, today).
`IussumOpus`: `ctx`, `pulsare(ctx, piscina) -> {status: CURRIT |
FINITUM, textus: whole text so far (already laid out), nuntius}`,
`abrumpere(ctx, causa) -> final textus`. The verb owns layout and
budget; scriba owns lock, snapshot, re-render, Esc and commit. Any
long-running command can use it; $question is the first.

### II.8 vicus $question verb + port

- The port becomes start / pump / cancel
  (`VicusPortaQuaestionis` replaced by a small struct of functions);
  `apps/vicus/vicus.c` implements it with `processus` + the frame
  reader; tests use a scripted in-memory fake (no process, no
  network). The terminal twin still has no port (same message).
- The verb's `IussumOpus` keeps the raw text received so far; each
  pulse with new text re-runs planning -> `responsum_disponere` ->
  fit to L lines. If the laid-out text no longer fits, it stops the
  helper (`abrumpere`) and finishes with the existing sentence-end
  truncation (`responsum_aptare`, extended to report "did not fit").
- Status line: `question: asking claude-opus-5-5...` (stage I, as now)
  -> `question: streaming... (Esc cancels)` -> `question: <usage>` /
  `question: cancelled` / `question: <error>`.

## III. The page while streaming (scriba)

1. Stage II starts the verb; if it returns an `opus`, scriba enters
   RUNNING: snapshot of the working page (`laboris`) taken, page locked.
2. Each pulse: `opus->pulsare`; if the text changed, the working page
   = snapshot + `effectum_applicare(text so far)`; nothing committed.
   The command head stays as typed until the end.
3. FINITUM: head -> `<question answered>`, one
   `scriba_documentum_committere` (one undo step), status as above.
4. Locked: every key except Esc is ignored, clicks on the page are
   ignored; only this scriba pane (other vicus panes keep working). The
   page fill is `COLOR_SUPERFICIES` instead of `COLOR_BACKGROUND`.
5. Esc: `opus->abrumpere` -> text so far + `[cancelled]`, committed as
   answered. Error mid-stream: same with `[error: <reason>]`. Error
   before any text: nothing inserted, status only (as today).
6. Hard limit: the helper is started with a 180 s `processus` timeout;
   a timeout is an error.

## IV. Tests and plants

- `http_lector`: split oracle (II.1) over Content-Length, chunked
  (incl. chunk extensions, trailers, 0-chunk), close-delimited,
  malformed (bad chunk size, header overflow); plant: chunk boundary
  off by one -> red.
- `sse`: spec cases (multi-line data, comments, CR/LF/CRLF, split
  everywhere); plant: blank line not dispatching -> red.
- vates: fake stream -> deltas in order + assembled response equals
  the non-streamed fake response for the same text; error event; retry
  before first delta only; herbarium capture + replay of a stream.
- quaestio: planning pipeline unchanged (existing markdown section
  green); prompt text pinned; line budget numbers pinned.
- frames: writer -> reader round trip at every split; truncated stream
  -> error.
- scriba/vicus (fake port): stream of N pieces over N pulses -> page
  grows, input ignored, page color tag, final page == non-streamed
  result for the same text, one undo; Esc mid-stream -> `[cancelled]`;
  overflow stops the fake; error mid-stream; error before text.
- One live check by Fran in vicus (key needed), recorded in the
  worklog.

## V. Not in this spec

- Route B (in-process reactor transport, DNS, non-blocking connect /
  TLS, hospitium deferred responses).
- norma for streamed responses (II.4).
- `$question<opus>` / `<haiku>` model selection; cost display.
- Editing elsewhere on the page while streaming.

## AUDIENDA (not verified)

- That SecureTransport's blocking `tls_recipere` returns partial data
  as soon as a TLS record arrives (expected; if it buffers, A still
  works but in bigger pieces - measure in the live check).
- Anthropic's exact current event set for opus-5-5 streams (taken from
  the API docs; the first real capture is the check, norma later).
- That `processus_effusio_hactenus` keeps up with a fast stream without
  pipe back-pressure stalls (it drains per pulse; 60 pulses/s × 64 KB
  pipe buffer should be ample - measure).
- That Markdown reflow of a PARTIAL answer never needs more lines than
  the final one at the same prefix (the overflow stop assumes monotone
  growth; an open code fence closing later could in principle shrink
  it - accepted: we stop early at worst).

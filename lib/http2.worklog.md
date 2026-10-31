# http2 worklog

## 2026-10-31 - T5: client connection (http2-plan T5)

Sans-IO connection: `http2_recipere` (octets in), `http2_mittenda`/`_consumere` (octets out), `http2_eventum_capere` (events out). Client role only; `HTTP2_SERVUS` returns NIHIL (spec D1).

- **Memory for a long-lived object** (piscina never frees): fixed input buffer (9 + our max frame), fixed 256 KiB output buffer with compaction, fixed header-block buffer (our CONTINUATION octet cap), one small dynamic piscina per stream destroyed at close, and a scratch piscina for pending events that is `piscina_vacare`d when `http2_eventum_capere` finds the queue empty. Events are deep-copied into the caller's piscina, so the caller must drain events before relying on more `recipere` calls reusing scratch - documented in the header.
- **Header blocks are always HPACK-decoded**, even for a stream we already closed or reset: skipping a block desynchronises the dynamic table for every later response.
- **One send path ("antlia", `_mittere_pendentia`)**: pending bodies first, then queued requests in stream-id order while active < peer MAX_CONCURRENT_STREAMS. Called after receive, consume, cancel and new requests. T6 adds window limits inside `_data_mittere`. Requests stay queued (raw headers copied) until sent, because HPACK encoding must happen in wire order.
- A whole header block is only sent when the output buffer can take all its HEADERS + CONTINUATION frames at once - nothing may interleave inside a block.
- **Error mapping**: codec stream errors -> RST_STREAM + ABROGATUS event; connection errors -> GOAWAY (last stream 0, we never accept peer streams) + ERROR event, connection dead (recipere/petere refuse). CONTINUATION count/octet caps -> ENHANCE_YOUR_CALM (the 2024 CONTINUATION-flood class). Frames on streams we never opened (even ids, or >= next id) -> PROTOCOL_ERROR; frames on streams we closed are ignored (after decoding headers).
- GOAWAY: streams above last-stream-id get a DISCESSUS event each (safe to retry) and are freed; one DISCESSUS with stream 0 carries code, last id and debug data; new requests are refused, lower streams finish normally.
- Mutation check: disabling the "nothing between HEADERS and CONTINUATION" rule makes "continuatio interrupta" fail in four places.
- Dropped the plan's `corpus_sequitur` flag (no streaming uploads needed yet).

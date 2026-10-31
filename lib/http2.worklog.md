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

## 2026-10-31 - T6: flow control

- **Send side:** `_data_mittere` takes min(remaining body, peer max frame, free output, connection window, stream window); windows are s64 so a SETTINGS decrease can drive a stream negative (6.9.2) - sending then simply waits for WINDOW_UPDATEs. INITIAL_WINDOW_SIZE changes move every live stream (queued ones too: their window is fixed at `http2_petere` from the peer's value then in force) by the delta; a result above 2^31-1 is a connection FLOW_CONTROL_ERROR. WINDOW_UPDATE overflow is a stream error on a stream, a connection error on stream 0; WINDOW_UPDATE on an idle stream is PROTOCOL_ERROR.
- **Receive side:** every DATA frame is charged by its *whole* payload, padding included - hence `Http2Forma.longitudo` (added to the T4 codec). Connection window is charged even for frames on streams we closed. Exceeding a stream window -> RST_STREAM FLOW_CONTROL_ERROR (no DATA event for that frame); exceeding the connection window -> GOAWAY. Top-up rule (C4): when a window is at or below half its target, one WINDOW_UPDATE restores it (stream target = our INITIAL_WINDOW_SIZE, 1 MiB; connection 16 MiB). No stream update after END_STREAM.
- The 3 MiB test plays a strict server that only sends what its view of our windows allows and replenishes from the WINDOW_UPDATEs it reads - without top-ups it stalls at exactly 1 MiB (seen red).
- **Concurrency needed no new code:** the T5 pump already queued requests above MAX_CONCURRENT_STREAMS and released them in id order as streams closed; the T6 test passed before any T6 change.

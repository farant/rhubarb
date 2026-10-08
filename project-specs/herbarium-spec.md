# herbarium spec - pressed specimens of what an API really sends

Born 2026-10-07 (Fran, during the vates spec review: "an http client
that is used for interacting with apis ... automatically logs unique
error messages ... so you can have more robust mock responses from the
api over time"). Placement decided by Fran: its own small library now,
generic over any HTTP API; vates is the first consumer. Ledger parcum
…SQM6.

*The question: how do test mocks come to contain the errors and
oddities an API actually produces, instead of the ones somebody
imagined while reading its documentation?*

## 0. Data (2026-10-07)

- `HttpPetitio` is opaque with no read accessors (`include/http.h:59`);
  its fields - method, schema, hospes, portus, via, corpus, capita,
  tempus_ms - live in `lib/http.c:28-47`. Capture and replay both need
  to read a request, so accessors are owed (§V).
- There is no transport type: callers call `http_exsequi` directly
  (`http.h:189`). A seam is owed (§V).
- Hashing: `sigillum` = SHA-256 (`include/sigillum.h:1`,
  `sigillum_computare`, `sigillum_hex`).
- Name: `specimen` is taken (visual regression, `include/specimen.h`);
  herbarium = the collection where pressed specimens are kept.

## I. Framing

Two halves over ONE transport seam:

- **Capture**: wraps any transport. A response that is unexpected
  (configurable) is pressed the first time its KIND is seen; later
  sightings are counted, not stored again.
- **Replay**: a transport that serves stored specimens, so tests run
  against real responses offline.

Between them, a deliberate **promotion** step: captured specimens live
outside any repository; the ones worth keeping are reviewed and copied
into committed fixtures. Two reasons: error bodies can quote the
request back (anything the user typed), so nothing enters history
unread; and a test that reads an uncommitted input breaks fresh clones
(house lesson, `frigida_probare`).

The payoff lives in each consumer: one test enumerates every committed
specimen, replays it, and asserts the consumer turns it into a NAMED
outcome. A specimen with no named handling turns that test red - the API
taught us something, and the suite says so.

## II. What is captured

- **Predicate** (options): status >= a threshold (default 400), plus
  explicit captures by the consumer (`herbarium_premere`) with a
  cause string - this is how NOVELTY is pressed: vates calls it when it
  meets a block type, `stop_reason` or field it did not model
  (OPACUM fallback), so silent API shape changes are captured even on
  HTTP 200.
- **Kind key** (what "unique" means): a function `(petitio, responsum,
  piscina) -> chorda`. Default: status + the JSON skeleton of the body
  (keys and value genera, recursively, values dropped; non-JSON bodies
  -> status + content-type + length bucket). Consumers supply better
  keys; vates' Anthropic key = status + `error.type` + `error.message`
  with digits and id-like runs masked.
- **Variants per key**: keep the first N bodies (default III) whose
  masked text differs - different wordings of one error kind.

## III. Storage (capture directory, given by the caller)

- `specimina/<sigillum of key, hex>-<n>.json`, written ONCE per
  variant: key, cause, status, response headers (ALLOWLIST only,
  default `content-type`, `retry-after`, `request-id`-like, and any the
  caller adds; never `set-cookie`), body verbatim, first-seen time,
  and a request summary (method, host, path, plus fields the caller
  names - for vates: model). NEVER request headers (they carry keys),
  NEVER the request body by default.
- `index.jsonl`: one line per SIGHTING (time, key hash, status, cause),
  write + fsync, append-only. Counts and last-seen are derived by
  reading it, so no file is ever rewritten.
- Promotion v1 is a reviewed `cp` into the consumer's committed fixture
  directory (vates: `probationes/fixa/vates/herbarium/`). A CLI comes
  only when a second consumer wants one (PULL rule).

## IV. API shape (header to be reviewed before implementation)

```c
Herbarium*    herbarium_aperire (Piscina*, constans HerbariumOptiones*);
HttpVectura   herbarium_vectura (Herbarium*, HttpVectura involuta);  /* capturing wrapper */
vacuum        herbarium_premere (Herbarium*, HttpPetitio*, HttpResponsum*,
                                 chorda causa);                      /* explicit / novelty */
Xar*          herbarium_enumerare (Piscina*, constans character* directorium);
                                 /* -> HerbariumSpecimen*, sorted by key */
HttpVectura   herbarium_reddens  (Piscina*, Xar* specimina);         /* replay transport */
```

`HttpVectura` is a struct of a function pointer and its `datum`, so a
wrapper can hold the transport it wraps. Capture never changes what
the caller receives: the wrapped response passes through untouched, and
a capture failure (disk full, unwritable directory) is reported once
on stderr and never fails the request.

## V. House additions owed (home library: http, own tests + worklog)

1. **`HttpVectura`** in `http.h`: `{ HttpResultus (*exsequi)(HttpPetitio*,
   Piscina*, vacuum* datum); vacuum* datum; }` and
   `http_vectura_ordinaria()` = the real `http_exsequi`. vates' seam
   (vates spec §IV) becomes this type; there is no `VatesVectura`.
2. **Read accessors on `HttpPetitio`**: method, url parts (schema,
   hospes, portus, via), body, header count + header i, `tempus_ms`.
   Read-only; the struct stays opaque.

## VI. Testing

- Capture: same kind twice -> one specimen, two sightings; a new
  wording -> variant 2; variant limit honoured; allowlist drops other
  headers; request headers and body never written; capture failure
  leaves the response intact.
- Default key: two bodies differing only in values share a key; a new
  field changes it.
- Replay: enumerate -> serve in order -> the consumer sees status,
  headers and body byte for byte.
- Each behaviour born red by a plant.

## VII. Not in this slice

A promotion CLI; capture of request bodies (opt-in only, if ever);
sharing specimens between machines; capturing streams (comes with SSE).

## AUDIENDA

- Masking "id-like runs" for the Anthropic key is a heuristic; the
  first live weeks will show whether it over- or under-splits kinds.
  The variant limit bounds the damage of under-masking.

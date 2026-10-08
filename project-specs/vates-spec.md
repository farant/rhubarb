# vates spec - the provider-neutral LLM library

Born 2026-10-07 from the optchat brainstorm (ledger parcum …SQM6; Fran's
decisions recorded there and in memory `optchat-project`). First
consumer: `../optchat`, a C89 build of Taelin's OptChat
(gist.github.com/VictorTaelin/91837951a5ce5b38f341ec1ba1df6449). Second
named consumer: arca (parcum …MMF58F), which supplies keys later.

*The question: how does house C89 talk to a hosted language model, so
that switching model or provider is configuration, and usage/cost is
accounted in ONE place for every caller?*

## 0. Data (2026-10-07)

- **HTTP client exists** (`include/http.h:142-206`): `http_petitio_creare`,
  `_caput_addere`, `_corpus_ponere_chorda`, `_tempus_ponere`,
  `http_exsequi` -> `HttpResultus {successus, responsum, error,
  error_descriptio}`; `http_responsum_caput` reads one header
  (`http.h:217`). Chunked bodies are decoded (`lib/http.c:175-260`) but
  the whole body is buffered: no incremental read, no SSE.
- **Every request is `Connection: close`** (`lib/http.c:455`): one TLS
  handshake per call.
- **GAP: the timeout never reaches TLS.** The https branch calls
  `tls_connectere(hospes, portus, piscina)` with default options
  (`lib/http.c:801`); the default is 30 s (`lib/tls_macos.m:190`); the
  caller's `tempus_ms` is applied only on the plain-TCP branch
  (`lib/http.c:819`), as `http.h:155-158` admits ("Semita http plana
  sola (v1)"). A non-streaming model call sends no byte until it is
  finished, so every call longer than ~30 s would end
  `HTTP_ERROR_TIMEOUT`. Fix owed (§VII).
- **TLS** is SecureTransport (`lib/tls_macos.m:1-10`), deprecated by
  Apple, TLS <= 1.2. Behind `tls.h`, which already names itself the
  platform veil (`tls.h:3-4`); ledger notes name BearSSL as the
  "Linux-day" vendored option (natura worklog; forum vendoredlibrary).
- **JSON** (`include/json.h`): parse + build; objects are an `Xar` of
  pairs (`json.h:89`), so `json_scribere` emits keys in insertion order
  - deterministic bytes, which prompt caching needs (a prefix match).
  Integers are s64 (`json.h:85`).
- **Async seams exist elsewhere**: `processus_incipere/pulsare/metere`
  (`include/processus.h:193-236`), `reactor` (poll loop,
  `include/reactor.h:1-13`). No threads, by design.
- **API facts** (claude-api skill reference, cached 2026-10-06): one
  endpoint `POST /v1/messages`; headers `x-api-key`,
  `anthropic-version: 2023-06-01`, `content-type`; at most 4
  `cache_control` breakpoints; top-level `cache_control` = automatic
  tail breakpoint; usage fields `input_tokens`,
  `cache_creation_input_tokens` (split 5m/1h in `cache_creation`),
  `cache_read_input_tokens`, `output_tokens`; Opus 5.5 / Sonnet 5.5
  reject disabled thinking and forced `tool_choice`; thinking blocks
  must be passed back unchanged (preserved thinking); `stop_reason`
  includes `refusal` with `stop_details`.

## I. Framing

vates owns: a neutral request/response model, ONE blocking call, retries,
and usage accounting. It does not own: an agent loop, tools' behaviour,
conversation storage, streaming (reserved, §X), or where keys come from.

Three principles:

1. **Nothing is dropped.** Any response block vates does not understand
   (thinking, redacted thinking, server-tool blocks, block types not
   yet invented) is kept as an OPACUM block holding the provider's JSON
   and re-emitted verbatim when the caller appends it to the next
   request. Forward compatibility and preserved thinking both fall out.
2. **Deterministic request bytes.** Same neutral request -> same JSON
   bytes, always. No timestamps, no map-ordered keys. (Caching is a
   byte-prefix match.)
3. **The key comes from the caller.** vates never reads the environment
   or a file for secrets; `vates_anthropic_aperire` takes the key as
   bytes and never logs, echoes or stores it in error text. Arca plugs
   in above vates without an API change.

## II. The neutral model

- **Block** (`VatesBlocus`): genus TEXTUS | INSTRUMENTUM_PETITUM
  (tool_use: id, titulus, input as `JsonValor*`) | INSTRUMENTI_EFFECTUS
  (tool_result: id, content text, `b32 erratum`) | OPACUM (`JsonValor*`
  carried verbatim). Every block has `b32 signum_thesauri` = a cache
  breakpoint after this block.
- **Message**: munus USOR | ASSISTENS, plus an ordered list of blocks.
  A response's blocks are the same type, so appending a response to the
  next request is one call.
- **Request** (`VatesPetitio`): exemplar (model id, chorda), system
  blocks, tools (`VatesInstrumentum {titulus, descriptio, schema
  JsonValor*}`), messages, max_tokens, effort (chorda, provider's
  vocabulary), thinking display, `b32 thesaurus_cauda` (the automatic
  tail breakpoint), and two escape hatches for experiments: extra body
  fields (`clavis -> JsonValor*`, merged last) and extra headers (e.g.
  `anthropic-beta`). The escape hatches are how provider features
  nobody has modelled yet (fallbacks, betas) get used without changing
  vates.
- **Response** (`VatesResponsum`): id, blocks, causa_finis (enum
  FINIS | INSTRUMENTUM | MAXIMUM | RECUSATIO | PAUSA | ALIA, plus the
  raw string), `VatesUsus`, error (below).

The caller counts breakpoints; vates refuses a request with more than
the provider's maximum (4 for Anthropic) with a named error, before
sending.

## III. The call

```c
VatesResponsum* vates_mittere (Vates* v, VatesPetitio* p,
                               chorda propositum, Piscina* piscina);
```

- Blocking. `propositum` is the accounting tag ("magister",
  "compactor", ...); it never reaches the provider.
- **Errors** (`VatesError`): OK | RETE (connection/TLS) | TEMPUS |
  STATUS (HTTP status + provider error type + message from the error
  JSON) | PARSE | LIMES (local refusal, e.g. too many breakpoints).
  A refusal is NOT an error: it is `causa_finis == RECUSATIO` on a
  successful response.
- **Retries**: on RETE, 408, 409, 429, 5xx, 529 - up to N attempts
  (default 3), fixed wait (default 2 s) or `retry-after` if present
  (capped). TEMPUS retries at most once (a 10-minute call is expensive
  to repeat). 4xx otherwise return at once. Every attempt is one ledger
  line (§V), so retries are visible in the accounting.
- **Timeout**: per call, default 10 minutes (needs §VII).

## IV. Backends

```c
Vates* vates_anthropic_aperire (Piscina*, chorda clavis, VatesOptiones*);
Vates* vates_fictus_aperire    (Piscina*);   /* scripted, no network */
```

- A backend is a table of functions (C vtable, compiled in, the
  `FabricaGenus` pattern): build the provider body from a
  `VatesPetitio`, send, parse the provider response into a
  `VatesResponsum` + `VatesUsus`.
- **Transport seam**: the Anthropic backend sends through a
  `VatesVectura` function pointer (url, headers, body -> status,
  headers, body). Default = `http_exsequi`. vates' own tests inject a
  scripted transport and assert the exact request bytes and the parse
  of recorded responses.
- **fictus**: a backend for CONSUMERS' tests. The test pushes scripted
  neutral responses (text, tool calls, usage, stop reasons, errors);
  fictus records every request it received so a test can assert what
  was sent (e.g. where breakpoints fell). optchat's whole pipeline runs
  under credo on fictus, free and offline.
- OpenAI is the named second backend (not in this slice). The model is
  shaped with it in view: encrypted reasoning items ride as OPACUM;
  its breakpoint mechanism differs, so `signum_thesauri` is a request,
  which each backend maps to what its provider supports.

## V. Accounting

- `VatesUsus`: input, cache_lectum, cache_scriptum_5m,
  cache_scriptum_1h, output (all s64 tokens); `mora_ms`; `pretium`
  in micro-dollars (s64), or -1 when no price is known.
- **Prices are data, given by the caller**: a table of `VatesPretium
  {exemplar, input, output, cache_lectum, cache_5m, cache_1h}` in
  micro-dollars per million tokens. vates computes cost; it never
  embeds a price list (prices change; a stale table in code lies).
- **Ledger** (`rationarium`): optional; when the caller opens one
  (`vates_rationarium_aperire(v, via)`), every attempt appends one JSON
  line - tempus, provisor, exemplar, propositum, attempt number, http
  status, causa_finis, usus, pretium, mora_ms, response id - with
  write + fsync. Never the key, never request or response bodies.
  "What did the compactor cost this week" becomes a query over a file.

## VI. Keys

Caller-supplied bytes (§I.3). optchat v1 reads `~/.rhubarb/anthropic.clavis`
(0600, outside every repository, written by Fran, never by Claude) and
passes it in. Arca later replaces the file read; vates does not change.

## VII. House fixes owed (home libraries, own tests + worklog notes)

1. **http -> tls timeout**: the https branch uses
   `tls_connectere_cum_optionibus` with the caller's `tempus_ms` (and
   the default when 0). Test: a local plain listener that accepts and
   stays silent cannot exercise TLS, so the test asserts the plumbing
   (options reach `tls_connectere_cum_optionibus`) and a smoke probe
   proves it against httpbin `/delay/n` over https. Worklog:
   `lib/http.worklog.md`.

Nothing else is owed by the design. Anything the smoke probes (§IX Q1)
uncover is added here before it is fixed.

## VIII. Testing

- **credo, offline**: request building (golden JSON bytes per
  feature: system + tools + breakpoints + OPACUM round-trip + extra
  fields), response parsing (recorded real responses as fixtures:
  text, tool_use, thinking, refusal, max_tokens, error bodies), retry
  policy over the scripted transport (429 with retry-after, then 200;
  529 x N; 400 no retry), price arithmetic, ledger lines, breakpoint
  limit refusal, fictus behaviour.
- **Local scripted server** (plain HTTP on port 0, from the house
  `tcp` + `http_parser`): proves the real `http_exsequi` path against
  exact bytes, including malformed and truncated responses.
- **Smoke probes, never gates** (network, sometimes money): named
  tools under `tools/`, run by hand.
  - howsmyssl.com/a/check - which TLS version and cipher we negotiate.
  - badssl.com (expired., wrong.host., self-signed., untrusted-root.)
    - that we REFUSE what we should.
  - httpbin.org (/status/429, /delay/n, /stream/n, /drip, /anything)
    - timeouts, chunked, echo of our exact headers.
  - api.anthropic.com `GET /v1/models` - free; proves TLS + headers +
    key + JSON end to end.
  - one tiny `messages` call, then the same prefix twice: proves
    `cache_read_input_tokens > 0`. Costs cents.

## IX. Build order (worktree `../rhubarb-quarta`)

- **Q1** smoke probe: GET /v1/models + howsmyssl + badssl through the
  house stack, BEFORE the API hardens. A TLS failure here makes
  Network.framework / BearSSL behind `tls.h` the first task instead.
- **Q2** `include/vates.h` for Fran's review (the API is the thing to
  get right; implementation can improve later). No implementation
  before approval.
- **Q3** §VII fix; neutral model + Anthropic body building + response
  parsing + fictus + transport seam, test-first.
- **Q4** retries, accounting, ledger; live smoke (one call, then cache
  proof).
- **Merge** quarta -> main by the tertia procedure.

## X. Not in this slice (recorded direction)

- **Streaming / async**: `vates_incipere` + `vates_pulsare` +
  `vates_metere`, shaped like `processus_*`, so a poll loop can drive
  several calls (optchat's compactor `JOBS > 1`) without threads.
  Needs incremental reads in `http` and an SSE library (generic,
  rhubarb). Fran wants it; v1 does not need it.
- OpenAI backend; connection reuse (keep-alive); Batches;
  count_tokens; the Models API as a library call.
- Platform TLS swap (Network.framework or BearSSL) - only if Q1 says so.

## XI. Done means

- `vates.h` approved by Fran before implementation.
- Offline suite green, every behaviour born red by a plant (house
  rule).
- §VII fix in with its test and worklog note.
- Live: one real call through vates; the second identical-prefix call
  shows `cache_read_input_tokens > 0`; both ledger lines present with
  correct cost.
- optchat can be built on it via silex with no vates change.

## AUDIENDA

- **BearSSL and TLS 1.3**: I believe BearSSL 0.6 (its last release)
  stops at TLS 1.2, which contradicts ledger nota seq 360 ("TLS 1.3 ...
  officium BearSSL"). Unverified here; check bearssl.org before any
  decision rests on it, and correct the ledger then.
- **Does `api.anthropic.com` accept TLS 1.2 from SecureTransport?** Q1
  answers it.
- **`PostToolUse` redaction** (arca's Claude-side layer, not vates):
  whether a Claude Code hook can redact tool output before the model
  sees it - unverified.
- **OPACUM re-serialization**: vates re-emits a parsed `JsonValor`, not
  the provider's original bytes. String escapes may differ
  (`é` vs raw UTF-8). Believed harmless (thinking signatures
  cover content, not JSON escaping) - the live smoke should include a
  thinking block round-trip to confirm.
- Prices and model facts are from the skill's cache dated 2026-10-06;
  the caller's price table is the source of truth.

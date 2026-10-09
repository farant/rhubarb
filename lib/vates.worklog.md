# vates worklog

## 2026-10-08 - part A: neutral model + Anthropic request body (vates-plan-2 T3)

Spec: project-specs/vates-spec.md (incl. §I.4 "config speaks the
provider's language, code none"); header approved by Fran (7b5ca52b).

**Render rules** (`vates_anthropic_corpus`, pure, deterministic):
- body key order fixed: model, max_tokens, thinking (only when showing
  summaries: adaptive + display summarized), output_config (only with
  effort), cache_control (tail breakpoint), system, tools, messages,
  then extras TAGGED "anthropic" (last; other providers' extras skipped).
- a block whose `crudum` came from provider "anthropic" is emitted
  VERBATIM (unknown fields survive); with `signum_thesauri` a shallow
  copy + cache_control is emitted for text/tool_use/tool_result. The
  crudum itself is never mutated. OPACUM never gets cache_control.
- a block whose crudum is from another provider: rebuilt from neutral
  fields if text/tool_use/tool_result, OMITTED if OPACUM.
- a message left with no blocks is omitted (an empty content array is a
  400 at the API).
- json_scribere emits raw UTF-8 (no \u escapes): the golden with
  macrons, a cross and a non-BMP letter matched byte for byte.

**Plants** (4/4 red): signum ignored, foreign OPACUM emitted, empty
message kept, extras emitted regardless of provider.

**Rulings**: `_responsum_vacuum` and `_signa_numerare` moved out of part A
(unused until T4/T5 -> -Werror unused-function); `_blocos_anthropic` ->
`_contentum_anthropic` (lint: "blocos" unknown).

## 2026-10-08 - part B: reading Anthropic responses (vates-plan-2 T4)

`_legere(status, corpus, piscina, &novitas)` (pure; `vates_anthropic_legere`
is the public face that ignores novitas):
- EVERY content block keeps `crudum` + `provisor = "anthropic"`; genus:
  text -> TEXTUS (textus filled), tool_use -> PETITUM (id, name, input),
  everything else -> OPACUM. Unknown types (not thinking /
  redacted_thinking) are NOVITAS "blocus ignotus: <type>" - kept, never
  dropped.
- known keys: top level {id,type,role,model,content,stop_reason,
  stop_sequence,stop_details,usage}; text {type,text,citations};
  tool_use {type,id,name,input}. Any other key = NOVITAS (the block stays
  usable; crudum preserves the key for re-emission). Only the FIRST
  novelty per response is reported (one press per response).
- stop_reason map end_turn/stop_sequence/tool_use/max_tokens/refusal/
  pause_turn; anything else -> ALIA + NOVITAS. refusal category from
  stop_details.category.
- usage: cache_creation.ephemeral_{5m,1h} when present, else all of
  cache_creation_input_tokens counts as 5m.
- status != 200 -> STATUS (error.type / error.message, or the first
  CCLVI bytes of a non-JSON body); 200 + unparseable -> PARSE.

Round trips proven in the tests: a thinking block with its signature +
tool_use come back byte-exact in the next request; a text block with
`citations` keeps them (via crudum). Plants 4/4 red: unknown block
dropped, cache_creation ignored, error.message in error_genus, crudum
ignored for TEXTUS. `_responsum_vacuum` re-inserted here (moved out of
part A in T3).

## 2026-10-08 - part C: providers, vates_mittere, ledger, fictus (vates-plan-2 T5)

- **Provider table** compiled in: {"anthropic", vates_anthropic_aperire},
  {"fictus", ...}. `vates_aperire` by name; unknown -> NIHIL + stderr
  listing the known names (seen in the test: "provisor ignotus 'openai'
  - noti: anthropic fictus").
- **The key**: copied ONCE into the Vates (NUL-terminated for the header).
  http_petitio_caput_addere necessarily copies it into the call's piscina
  too (http's API). Proven absent from the ledger, herbarium index,
  every specimen file and the error text (probatio_clavis_non_effunditur:
  a distinctive key through a scripted vectura, every written file read
  back). Plant (key as a ledger field) red.
- **Retry**: RETE always; TEMPUS on the first attempt only (a 10-minute
  call is expensive to repeat); HTTP 408/409/429/>=500. Wait =
  retry-after seconds if present else mora_iterandi_ms, ALWAYS capped by
  mora_iterandi_maxima_ms (a hostile retry-after 3600 with a 5 ms cap:
  the whole test runs in 0.3 s; a cap 1000x too large is red).
- **Usage** on the returned response = sum over attempts; each attempt is
  its own ledger line (write + fsync); price -1 when the model is not in
  the caller's table. Worked example 1000/2000/300/0/50 tokens at
  $4/$0.20/$5/$8/$20 per MTok = 6900 micro-dollars (asserted).
- **Breakpoints**: > IV -> LIMES before anything is sent (fictus saw 0
  requests).
- **fictus** = the Anthropic backend over a scripted vectura: replies are
  rendered as real Anthropic JSON and parsed by the real parser; the sent
  body equals `vates_anthropic_corpus` byte for byte.
- **Novelty -> herbarium**: an unknown block on HTTP 200 is pressed with
  causa "blocus ignotus: server_tool_use" (test; plant red).
- Masking for the Anthropic kind key is a copy of herbarium's (the
  approved header does not export it). `piscina == NIHIL` -> NIHIL, the
  one exception to "never NIHIL".
- Lint renames: hr->resultus_http, ho->optiones_herbarii,
  in_cifris->in_numeris; glossary entry herbarium gained the form
  "herbarii".

## 2026-10-08 - committed specimens + the sweep (vates-plan-2 T6)

`probationes/fixa/vates/herbarium/specimina/` holds eight hand-made seeds
in herbarium's exact format (semen_<kind>-1.json): 529 overloaded, 429
rate_limit (with retry-after), 400 invalid_request, 401 authentication,
500 api_error, 200 refusal (category cyber), 200 max_tokens, 200 with an
unknown block (server_tool_use). `probatio_specimina_commissa` replays
EVERY committed specimen through `herbarium_reddens` +
`vates_anthropic_legere` and demands a NAMED outcome: non-200 ->
VATES_ERROR_STATUS with a non-empty error_genus; 200 -> success with a
causa_finis other than ALIA. Presence is asserted (> 0), not a fixed
count - the set is meant to grow.

**Promotion (how the set grows):** live capture lands in the caller's
herbarium directory (optchat: ../optchat-acta/herbarium, tools/vates_fumus:
build/vates_fumus_herbarium). Read the specimen (error bodies can quote
the request back), then `cp` it here unchanged. If the sweep turns red,
the API sent something vates cannot name yet: teach the parser, then the
suite is green again. Plant: a seed with stop_reason "novum_finis" -> red
at :733 (ALIA = unnamed), removed.

## 2026-10-08 - first live run (vates-plan-2 T7, tools/vates_fumus)

claude-sonnet-5-5, 10:27Z, system prompt 3851 bytes (1839 tokens):

| call | what | in | cache read | cache write | out | udollaria | ms |
|---|---|---|---|---|---|---|---|
| A | first call, system marked | 18 | 0 | 1839 | 6 | 4693 | 1975 |
| B | identical prefix | 18 | **1839** | 0 | 6 | **463** | 1437 |
| C | thinking, effort high | 34 | 0 | 0 | 138 | 1448 | 3695 |
| D | C's thinking block re-sent + new turn | 196 | 0 | 0 | 58 | 972 | 3227 |

Total 7576 udollaria ($0.0076). Cache read ~10x cheaper than the write
call. D proves OPACUM re-emission: the API accepted the thinking
signature from a re-serialized JsonValor (vates-spec AUDIENDA "OPACUM
re-serialization": RESOLVED - harmless). Ledger: 4 lines = 4 attempts.
Key-leak grep (Fran): 0 in stdout, ledger, herbarium (4 files).

**herbarium caught the API's real shape on the first live call:** every
200 carries top-level `container` and `diagnostics` (unknown to
`_claves_summae`, novelty "campus ignotus: container" - `diagnostics`
hidden because only the first novelty is reported) and usage extras
`output_tokens_details`, `service_tier`, `inference_geo` (usage keys are
not novelty-checked). -> T7b.

## 2026-10-08 - the live shape taught (vates-plan-2 T7b, Fran's ruling)

`_claves_summae` gains `container` and `diagnostics` (both `null` in an
ordinary call: no code-execution container, no cache diagnostics asked).
Test `probatio_forma_viva` feeds the EXACT live body from the T7 run
through fictus + herbarium and demands zero specimens (was one). Plant:
dropping only "diagnostics" is red - the first-novelty-only rule had
hidden it behind "container", the test pins both.

Known but unmodelled (recorded, not parsed): `usage.output_tokens_details`
(seen: `{"thinking_tokens":0}` - thinking is ALREADY inside output_tokens;
a future VatesUsus field could show the thinking share of a turn's cost),
`usage.service_tier` ("standard"), `usage.inference_geo` ("global").
Usage keys are not novelty-checked, so these never pressed.

Promoted (Fran read it first, approved unchanged): the live text response
as `probationes/fixa/vates/herbarium/specimina/vivum_textus_sonnet_5_5-1.json`
- the sweep now covers the API's real shape, not only hand-made seeds.

## 2026-10-08 - novelty = a declared NOTANDUM schema (norma-plan-2 N5)

The hand-kept key lists (`_claves_summae`, `_claves_textus`,
`_claves_petiti`, `_in_indice`, `_claves_probare`) are gone. `_legere` now
judges every 200 body against `_forma_responsi` - a norma schema of the
Anthropic message in NOTANDUM mode: top level (id, type, role, model,
content, stop_reason, stop_sequence, stop_details, usage, container,
diagnostics), usage (incl. cache_creation, output_tokens_details,
service_tier, inference_geo), and content blocks as a discriminated union
on "type" (text {text, citations?}, tool_use {id, name, input}, thinking
{thinking, signature}, redacted_thinking {data}).

Novelty = ALL of norma's notes (and issues), joined with "; ":
- unknown block -> "blocus ignotus: <type>" (same text as before; the T5
  test still sees exactly "blocus ignotus: server_tool_use");
- undeclared key -> "campus ignotus: <path>" (now WITH the path, and
  every one, not just the first);
- a schema issue (e.g. a required field missing) -> "forma fracta: <path>";
- unknown stop_reason still from `_causa_finis`, first in the line.

Regression oracle: probatio_vates UNCHANGED, 106/106. Plants: drop
"diagnostics" from the schema -> live-shape test red; content union
APERTUM -> novelty test red. Live (N5.4, claude-sonnet-5-5, $0.0078):
all checks pass and herbarium recorded 0 new sightings for ordinary
calls (4 before, 4 after).

## 2026-10-08 - response schema moved to `.norma` (norma-plan-3 A6)

The Anthropic response schema (N5's `_forma_responsi`, ~58 lines of
builder calls) now lives in `lib/vates_responsum.norma` - three named
normae `responsum`, `usus`, `blocus` - and reaches the binary as
GENERATED C (`include/vates_responsum_norma.h` + `lib/vates_responsum_norma.c`,
fabrica action `vates_responsum`, `bin/norma c`). No STML parser is linked
into vates; `_novitates_ex_norma` calls `vates_norma_responsum(p)`.

How the file was made: a throwaway program ran a copy of the OLD builder
(handing back its `usus` and `blocus` nodes) through `norma_stml_scribere`
- equivalence by construction, then proven: `probatio_forma_ex_norma`
keeps a verbatim copy of the old builder as an ORACLE ONLY and asserts the
generated schema has the same JSON Schema export and the same judgments
on values generated from both (TYPICA/FINES/INVALIDA, L seeds), that the
file is in formatter form, and that the reader loads it without notes.

To change the schema now: edit `lib/vates_responsum.norma`, run
`./tools/norma_c_regenerare.sh lib/vates_responsum.norma vates_norma_ include/vates_responsum_norma.h lib/vates_responsum_norma.c`
(fabrica's judge prints that line when the C is stale), and update the
oracle copy in the test if the change is deliberate.

Plants: `stop_sequence` made required + regenerated -> export and
judgment oracle red (the committed specimens all carry the field, so the
specimen test stays green - it is not this change's witness); one byte of
the generated C edited -> fabrica STALUM with the healing command.

## 2026-10-09 - the guard before use, vates side (herbarium-spec-2 H2)

Fran's floor: "is this an outlier payload that I need to save as a
specimen before I do anything else". Measured before: a 200 whose body
was not JSON (or whose root was not an object) was NEVER saved -
`_legere` returned VATES_ERROR_PARSE before computing novelty, and the
press came after `_legere`; statuses 201..399 were neither parsed nor
saved. Plant b re-creates exactly that (late press back, judge off): the
schema-note case stays green, the non-JSON / array / 204 / 302 cases go
red.

Now:
- `_iudex_anthropic` runs INSIDE herbarium's transport wrapper, before
  vates sees the response: status != 200 -> "status inexspectatus: N";
  not JSON -> "corpus non JSON"; root not object -> "radix non objectum";
  else `_novitates_corporis` (unknown stop_reason + schema notes/errors,
  i.e. the old novelty, unchanged in wording). Uses only status,
  json_legere, norma_iudicare. The late press in `vates_mittere` and the
  schema call in `_legere` are gone - one capture point.
- Capture is ON by default: `herbarium_via` NIHIL = the default location
  (`herbarium_sedes_ordinaria`) + host; the Anthropic host is derived
  from VATES_URL_ANTHROPIC ("api.anthropic.com"); the fictus provider
  uses host "fictus" so fake responses never join the real pile.
  `sine_herbario` switches it off. An unresolvable default (e.g.
  $RHUBARB_HERBARIUM naming a missing directory) prints one stderr line
  and the call proceeds without capture.
- Test isolation: `principale` points $RHUBARB_HERBARIUM at
  /tmp/probatio_vates_sedes_<pid> before any Vates exists and removes
  the piles at the end; ~/.rhubarb/herbarium verified untouched.
- Ruling: the spec's "judge without vates_mittere" test would need the
  judge public; it stays static. Order is proven by H1's in-loop count
  (wrapper presses before return) plus the non-JSON case here (saved
  although vates' parse exits early).
- The judge parses the body once more than before (json_legere in the
  judge, again in `_legere`) - cost accepted, noted.

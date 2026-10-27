# communio phase 2 (server act store + protocol) - Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.
> **House rule (Fran):** executed INLINE, one task per turn, Fran approves each before the next.

**Goal:** a volume on the Mac sends its acts and blobs to the household server over HTTP and receives every other device's acts of its space; a second volume follows a picture drawn in the first through the server, live (long-poll), with nothing duplicated or lost across a server outage. Proven in-process with a real `hospitium` and `http_vocare` on one reactor, then once against the VPS.

**Architecture:** the server is just another volume (phase 1 proved the relay A -> B -> C keeps identity and writer numbering): a library `communio_servus` keeps a device registry and mounts the protocol on a `Hospitium`, importing pushed acts into its own volume and serving them to the other devices of the same space. A library `communio_cliens` is a reactor-driven state machine (push, then long-poll pull, repeat) keeping its cursors in the local volume's `praesentia`. A small codec `communio_forma` is the one place acts become JSON. exemplar mounts the server on its existing hospitium with its own `communio.volumen`; a `communio` command-line tool drives the client on the Mac.

**Tech Stack:** C89 in Latin (latina.h), `volumen`/`scrinium` (SQLite), `hospitium` (server, deferred responses), `http_vocatio` (reactor client), `json`, `sigillum` (SHA-256), `moneta` (random), credo v2 suites, `silva.commissio`.

**Spec:** `project-specs/communio-spec.md` (APPROBATUM 2026-10-26) - D1, D2, D3, D4, D5; modules II.3 (protocol + client), II.4 (server act store), II.5 (spaces, minimal), II.2 (device registry; sign-in by email is phase 3), II.9 (live documents through the client). Phase 1: `project-specs/communio-plan-1.md`, spec section VI.

## Calls made in this plan (the spec's "left to the builder", V)

- **C1 Server store = its own volume, `<data>/communio.volumen`,** opened by exemplar next to `exemplar.volumen`. Imported acts never mix with exemplar's notes and users, so exemplar's own readers (`_replicare`, which reads every act) need no change and phase 4 backs up a pure act store. The device registry tables live in that volume too; the person check goes through exemplar's `Cives*`.
- **C2 Device credential = a token the DEVICE mints.** `communio tessera` writes 32 random bytes (base64url, 43 characters) to a 0600 file on the device and prints only the token's SHA-256 hex. Fran registers that digest through the admin channel (`communio_apparatum_addere`). The secret never travels and never appears in a chat, a log or the ledger, like an SSH public key. Phase 3's magic link registers the same digest by email instead; the table and the check do not change.
- **C3 A device belongs to one space** (spec V: all pictures -> "familia"). The device record names its space; a pull returns the acts of every OTHER device registered in that space (revoked devices included: revoking stops syncing, it does not hide what was shared). Membership (`communio_membra`) is checked when a device is registered.
- **C4 The replica binds on first contact.** The device record starts without a replica; the first `POST /communio/apparatus {"replica":...}` binds it; the same replica again is a no-op, another one is refused (409). A device can push only acts of its bound replica (D3 credit, D4 single writer). Adoption (D3b) is phase 3.
- **C5 Wire format: HTTP + JSON.** An act is an object of strings (`hora`, `basis_hora` as decimal strings - they are packed 64-bit clocks; `datum` as a string, byte-exact). Endpoints under `/communio/`. Cursors: upload = the device's own local `seq`; download = the server's local `seq` (its arrival order).
- **C6 Blobs travel before the acts that name them** (phase 1 rule `VOLUMEN_MASSA_DEEST`): the push asks which hashes the server lacks, then PUTs them; a pull lists the referenced hashes the server holds and the client fetches the missing ones before importing. Any registered device may fetch any blob by hash in milestone 1 (one space; per-space blob access is revisited with a second space).
- **C7 HTTP stays in one place in the client** (one static function sends a request) so phase 5 can put the iOS URLSession host service (D11) behind it; no transport interface is built now.

## Global Constraints

- C89, house flags (-pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Wcast-qual ...); Latin identifiers; `i32`/`i64` UNSIGNED; Roman-numeral macros; `nomen` is a macro; chorda not NUL-terminated.
- Acts are immutable and keep their identity `(hora, replica)`, `machina`, `momentum`, `genus`, `datum` byte-for-byte, and `seq_originis` (writer numbering) on every hop.
- Derived data never syncs: `communio_actum_derivatum` (the phase 1 rule) decides, in one place.
- Network-boundary validation (deferred minor M4) on both sides: replica = 26 Crockford base32 characters; `hora` > 0; `seq_originis` > 0; `genus` non-empty; a `plagula-*` act whose `via` starts with `aliena/` is refused.
- Secrets: the token is never logged, never put in an act, a volume, an error message, a fixture or a commit; the server stores only its SHA-256 hex and compares in constant time; the client reads the token file with `filum_*` under `<tolera codex="CANALIS_PRAETERITUS">` (never `lectiones`) and refuses a file whose mode is not 0600.
- Limits: `COMMUNIO_ACTA_MAXIMA` CC (200) acts per push or pull; push body at most DXII KB (512 KB, under hospitium's 1 MB); long-poll wait `mora` at most XXV s (under hospitium's 30 s deferred-response limit); client retry after an error XXX s.
- Server writes are the single writer (exemplar's `sera`); every multi-row write inside `volumen_transactionem_incipere/committere`.
- No UI changes, no iOS/Android code, no email in this phase. Deploying to the VPS (Task 8) happens only from main, on Fran's word.

## Review Focus

1. A push interrupted halfway (connection drops after the server committed some acts): the retry re-sends them, the server answers "already present", the cursor moves on - no duplicates -> T4 "iterum post abruptionem", T6 "servus cadit".
2. A device pushing acts that are not its own (another replica, or a forged identity): refused, nothing written, the device's cursor unchanged -> T4 "replica aliena recusatur".
3. A picture whose stroke act arrives before its image blob (or a blob PUT whose bytes do not match the hash): never imported half; the act waits for the blob; a bad blob is refused -> T4 "massa prius", "massa falsa".
4. A long-poll waiter whose client went away, or two waiters on one push: no response to a dead connection, both live waiters answered, no timer left behind -> T5 "cliens abiit", "duo exspectantes".
5. An importing volume's own projections and documents: foreign acts of the same genus or path never change the local conspectus or a local document, and out-of-order arrival falls back to a full projection -> T1.

---

### Task 1: importing volumes stay correct (carried from phase 1)

**Files:**
- Modify: `lib/conspectus.c` (~1100-1130, `conspectus_renovare`), `lib/historia.c` (`historia_sequi`), `lib/conspectus.worklog.md`, `lib/historia.worklog.md`
- Test: `probationes/probatio_communio.c` (new sections), `probationes/probatio_conspectus.c` if a conspectus fixture is simpler there

**Interfaces:**
- Consumes: phase 1 (`volumen_acta_propria_legere`, `volumen_replica`, `volumen_seq_ultimum`, `communio_transferre`).
- Produces: no new API. Rule (recorded in `include/volumen.h`'s header comment): **a reader that derives local state from acts reads only its own replica** (`volumen_acta_propria_legere` / `_generum_replicae_legere`), falling back to the unfiltered reader only when the replica is empty.

- [ ] **Step 1: failing tests** - "conspectus proprius": volume A has a projection over a document path; B (another replica) writes acts of the same genus and a `plagula-condita` at the same `via`; transfer B -> A; `conspectus_renovare` on A leaves A's projection unchanged (no invalidation, same value) and A's cursor reaches `volumen_seq_ultimum(A)` (the cheap "nothing new" check short-circuits on the next call). "sequi extra ordinem" (M1): B follows A's foreign picture; import A's acts into B in an order where a later stroke arrives before an earlier one (call `volumen_actum_importare` directly); `pictor_documentum_sequi` -> state equals a fresh `pictor_documentum_aperire_alienum`.
- [ ] **Step 2:** `./compile_tests.sh communio` (and `conspectus` if used) -> FAIL on both.
- [ ] **Step 3:** conspectus reads own acts when the replica is non-empty and sets `r->cursor = volumen_seq_ultimum` after the pass; `historia_sequi` takes the incremental path only if `vivi_prior + new live acts == vivi after recomputing`, otherwise projects from scratch.
- [ ] **Step 4:** PASS; `./compile_tests.sh volumen historia pictor conspectus communio` green.
- [ ] **Step 5:** commit (this plan file goes in this commit).

### Task 2: `communio_forma` - acts as JSON, validated

**Files:**
- Create: `include/communio_forma.h`, `lib/communio_forma.c` (via `./silva/scribe.sh`)
- Modify: `include/communio.h`, `lib/communio.c` (export the derived-act rule), `include/volumen.h`, `lib/volumen.c` (replica-set reader)
- Test: create `probationes/probatio_communio_forma.c`; `probationes/probatio_volumen.c`

**Interfaces:**
- Consumes: `VolumenActum`, `VolumenIdentitas` (phase 1).
- Produces:
  - `b32 communio_actum_derivatum(constans VolumenActum* actum);` (moved out of `communio.c`'s static `derivatum`; `communio_transferre` uses it).
  - `#define COMMUNIO_ACTA_MAXIMA CC`
  - `JsonValor* communio_actum_scribere(constans VolumenActum* actum, Piscina* piscina);` - object with string fields `hora, replica, machina, momentum, genus, datum, basis_hora, basis_replica, basis_machina` and integer `seq_originis`; basis fields omitted when the act has no basis.
  - `b32 communio_actum_legere(constans JsonValor* obiectum, VolumenActum* exitus, chorda* causa, Piscina* piscina);` - fills the act (`seq` = 0); FALSUM + short Latin cause on a missing/malformed field.
  - `b32 communio_actum_validum(constans VolumenActum* actum, chorda* causa);` - the M4 rules of Global Constraints.
  - `chorda communio_acta_scribere(Xar* acta /* VolumenActum */, Piscina*);` -> `{"acta":[...]}` and `Xar* communio_acta_legere(chorda corpus, chorda* causa, Piscina*);` (NIHIL on a bad document or any invalid act).
  - `Xar* communio_massas_nominatas(constans VolumenActum* actum, Piscina*);` - the 64-lowercase-hex runs of `datum` (the phase 1 scan, moved here and shared by `communio_transferre`, the server and the client).
  - `Xar* volumen_acta_replicarum_legere(Volumen* volumen, s64 post_seq, chorda* replicae, i32 numerus, i32 limes, Piscina* piscina);` - acts of any listed replica after local seq, by seq, at most `limes`.

- [ ] **Step 1: failing tests** - "iter et reditus": an act with a basis, a datum containing quotes, backslashes, newlines and `é`, `hora` near 2^62 -> scribere -> json_scribere -> json_legere -> legere: every field equal byte-for-byte (`chorda_aequalis`), `seq_originis` kept; an act without basis round-trips with empty basis. "validum": replica of 25 characters, lowercase replica, `I`/`L`/`O`/`U` in a replica, `hora` 0, `seq_originis` 0, empty genus, `plagula-condita` with `via` `aliena/X/y` -> each FALSUM with a cause; the good act VERUM. "corpus malum": not JSON, `acta` missing, one bad act among good ones -> NIHIL. "massae nominatae": a datum with two sigilla and a 65-hex run -> exactly the two. Volumen suite "replicae selectae": three replicas' acts imported into one volume -> reader with two of them returns only theirs, in seq order, `limes` respected.
- [ ] **Step 2:** FAIL (no header). **Step 3:** implement; `communio_transferre` switches to the shared rule and scan (its suite stays green). **Step 4:** `./compile_tests.sh communio volumen` green. **Step 5:** commit.

### Task 3: `communio_servus` - the device registry

**Files:**
- Create: `include/communio_servus.h`, `lib/communio_servus.c`, `lib/communio_servus.worklog.md`
- Test: create `probationes/probatio_communio_servus.c`

**Interfaces:**
- Consumes: `Cives*` (`cives_per_id`, `CIVIS_ACTIVUS`), `sigillum`, `moneta`.
- Produces:
  - Tables (CREATE IF NOT EXISTS, in the server volume): `communio_membra (spatium TEXT, civis TEXT, PRIMARY KEY (spatium, civis))`; `communio_apparatus (id TEXT PRIMARY KEY /* ULID */, civis TEXT, spatium TEXT, titulus TEXT, digestum TEXT UNIQUE /* sha256 hex */, replica TEXT UNIQUE, creatus_ms INTEGER, usus_ultimus_ms INTEGER, revocatus INTEGER DEFAULT 0)`. Registry changes are also appended as acts `communio.apparatus-additus {id, civis, spatium, titulus}`, `communio.apparatus-revocatus {id}`, `communio.membrum-additum {spatium, civis}` (never the digest).
  - `nomen structura { chorda id; chorda civis; chorda spatium; chorda titulus; chorda replica; b32 revocatus; } CommunioApparatus;`
  - `CommunioServus* communio_servum_creare(Piscina*, Volumen* volumen, Cives* cives, s64 (*nunc_ms)(vacuum));`
  - `b32 communio_membrum_addere(CommunioServus*, chorda spatium, chorda civis);`
  - `b32 communio_apparatum_addere(CommunioServus*, chorda civis, chorda spatium, chorda titulus, chorda digestum, character* id_exitus /* >= 27 */);` - FALSUM if the person is not active, not a member of the space, or the digest is not 64 lowercase hex or already used.
  - `b32 communio_apparatum_revocare(CommunioServus*, chorda id);`
  - `Xar* communio_apparatus_enumerare(CommunioServus*, Piscina*);`
  - `b32 communio_apparatum_verificare(CommunioServus*, chorda tessera, CommunioApparatus* exitus, Piscina*);` - token of exactly 43 characters -> digest lookup, constant-time compare, not revoked, person `CIVIS_ACTIVUS`; updates `usus_ultimus_ms`.
  - `s32 communio_replicam_ligare(CommunioServus*, chorda id, chorda replica);` -> 0 bound now or already this replica, 1 bound to another replica, 2 replica taken by another device, -1 invalid.
  - `b32 communio_tesseram_facere(character exitus[XLIV], character digestum[LXV]);` - client side helper used by the tool (T7) and the tests: mints a token and its digest.

- [ ] **Step 1: failing tests** - "apparatus": member + device by digest -> verificare with the token VERUM (fields filled); wrong token, 42-character token, empty -> FALSUM; "revocatus": after revoke -> FALSUM; "persona suspensa": `cives_statum_ponere` suspended -> FALSUM; "non membrum": adding a device for a person outside the space -> FALSUM; "digestum iteratum" -> FALSUM; "ligare": 0, 0 again, 1 for another replica, 2 when a second device claims the first's replica; "secretum nusquam": after all of the above, no act `datum` and no table cell contains the token text (scan `volumen_acta_legere` + the registry rows).
- [ ] **Step 2:** FAIL. **Step 3:** implement (pattern: `lib/admissio.c` `_digestum`, `_aequale_constans`, `admissio_verificator`). **Step 4:** PASS. **Step 5:** commit.

### Task 4: the protocol on `hospitium` - push, blobs, pull

**Files:**
- Modify: `include/communio_servus.h`, `lib/communio_servus.c`
- Test: `probationes/probatio_communio_servus.c` (HTTP sections; helper: one `Reactor`, `hospitium` with `cfg.reactor` and `portus` 0, requests via `http_vocare` to `http://127.0.0.1:<portus>`, loop `reactor_poll` until `http_vocatio_finita` - patterns from `probatio_hospitium_dilatum.c` and `probatio_http_vocatio.c`)

**Interfaces:**
- Consumes: T2 codec and validation, T3 registry.
- Produces: `b32 communio_servum_montare(CommunioServus*, Hospitium*);` registering, all requiring `Authorization: Bearer <tessera>` (401 `{"error":"..."}` otherwise):
  - `POST /communio/apparatus` `{"replica":R}` -> 200 `{"apparatus":id,"spatium":S}`; 409 on `communio_replicam_ligare` 1 or 2; 400 invalid.
  - `POST /communio/massae/desunt` `{"massae":[hex...]}` -> 200 `{"desunt":[hex...]}`.
  - `PUT /communio/massae/:sigillum` raw body (or the spooled `corpus_via` above 1 MB, via `volumen_massam_condere_ex_filo`) -> 201; 400 if the bytes' SHA-256 is not `:sigillum`.
  - `GET /communio/massae/:sigillum` -> 200 streamed (`colloquium_respondere_fontem`), 404 absent.
  - `POST /communio/acta` `{"acta":[...]}` (at most `COMMUNIO_ACTA_MAXIMA`) -> imports in order inside one transaction per act; stops at the first refusal; 200 `{"accepta":n,"refusum":null}` or `{"accepta":n,"refusum":{"index":i,"causa":"hora-futura|conflictus|massa-deest|invalidum"}}`; 403 (nothing imported) if any act's replica is not the device's bound replica or the device has none.
  - `GET /communio/apparatus` -> 200 `{"apparatus":[{"replica":R,"titulus":T,"civis":C,"persona":P}...]}` for the caller's space (bound devices only; `persona` = the person's `titulus`; no digests) - answers "who and which device" for every received act (spec III.6; the gallery's "by Amanda" in phase 5).
  - `GET /communio/acta?post=N` -> 200 `{"acta":[...],"cursor":M,"plura":b,"massae":[hex...]}`: acts (server seq > N) of every other bound replica of the caller's space, at most `COMMUNIO_ACTA_MAXIMA`, `cursor` = server seq of the last returned (N if none), `massae` = hashes the acts name that the server holds.

- [ ] **Step 1: failing tests** - "sine tessera" 401 on every route; "revocatus" 401; "ligare" 200 then 409; "auctores": `GET /communio/apparatus` from Y lists X's replica with X's person and device titles, no `digestum` key; "replica aliena recusatur": device X pushes an act of Y's replica -> 403, server act count unchanged; "acta et tractio": X binds, pushes 3 acts -> `accepta` 3; device Y (same space) pulls `post=0` -> 3 acts, identity/datum/`seq_originis` equal to X's; X pulls -> 0 (own excluded); device Z in another space -> 0; "iterum post abruptionem": the same 3 pushed again -> `accepta` 3, server count unchanged; "massa prius": a `plagula-condita` whose blob the server lacks -> `refusum.causa` `massa-deest`; `desunt` lists it; PUT; push again -> accepted; Y's pull lists the hash in `massae` and GET returns the bytes; "massa falsa": PUT with bytes not matching -> 400, `volumen_massam_habet` FALSUM; "paginae": 250 acts -> pull returns 200 with `plura` true, then 50 with `plura` false; "invalidum": an act with a `via` under `aliena/` -> `refusum.causa` `invalidum`.
- [ ] **Step 2:** FAIL. **Step 3:** implement. **Step 4:** PASS + `./compile_tests.sh 'communio|hospitium'` green. **Step 5:** commit.

### Task 5: live pull (long-poll)

**Files:**
- Modify: `lib/communio_servus.c`, `include/communio_servus.h`
- Test: `probationes/probatio_communio_servus.c`

**Interfaces:**
- Consumes: T4 pull; `colloquium_differre`, `dilatum_colloquium`, `reactor_timer_adicere`/`_cancellare`.
- Produces: `GET /communio/acta?post=N&mora=S` (S seconds, capped at XXV): if there is nothing to return, the request waits; a successful push into the space answers every live waiter of that space (except the pusher's own replica) with the same body a fresh pull would give; at S seconds an empty 200 (`acta` [], `cursor` N). Waiters whose `dilatum_colloquium` is NIHIL are dropped. `communio_servum_destruere(CommunioServus*)` cancels every timer.

- [ ] **Step 1: failing tests** - "exspectat": Y long-polls `mora=5`; pump 100 ms -> not finished; X pushes -> Y finishes with X's act; "tempus": `mora=1`, nothing pushed -> empty 200 after ~1 s; "duo exspectantes": Y and W (same space) wait, X pushes -> both answered; "cliens abiit": Y waits then aborts (`http_vocationem_abrumpere`); X pushes -> no crash, server still answers a new pull; after `communio_servum_destruere` and `hospitium_destruere`, `reactor_numerus_timer(r) == 0`.
- [ ] **Step 2:** FAIL. **Step 3:** implement. **Step 4:** PASS. **Step 5:** commit.

### Task 6: `communio_cliens` - the device side

**Files:**
- Create: `include/communio_cliens.h`, `lib/communio_cliens.c`, `lib/communio_cliens.worklog.md`
- Test: `probationes/probatio_communio.c` (end-to-end sections: two device volumes + server in one process)

**Interfaces:**
- Consumes: T2 codec, T4/T5 endpoints, `http_vocare`, `volumen_praesens_ponere/legere`, phase 1 import and `pictor_documentum_sequi`.
- Produces:
  - `nomen vacuum (*CommunioAdvenerunt)(vacuum* ctx, i32 numerus);` - called after acts were imported (the app follows its open documents there).
  - `nomen structura { s64 ascensus; s64 descensus; i32 missa; i32 recepta; b32 connexus; chorda error; } CommunioStatus;`
  - `CommunioCliens* communio_clientem_creare(Piscina*, Reactor*, Volumen* volumen, constans character* basis /* "https://host" */, chorda tessera, CommunioAdvenerunt advenerunt, vacuum* ctx);`
  - `vacuum communio_cliens_incipere(CommunioCliens*);` - cycle: bind replica (once) -> push own syncable acts after `ascensus` (batches of at most `COMMUNIO_ACTA_MAXIMA` and DXII KB; missing blobs first) -> pull `post=descensus&mora=XXV` (fetch missing blobs, import in one transaction per act, advance `descensus`) -> repeat; on any error, record it and retry after XXX s.
  - `vacuum communio_cliens_sistere(CommunioCliens*);` (aborts the open request, cancels timers), `CommunioStatus communio_cliens_status(constans CommunioCliens*);`, `b32 communio_cliens_otiosus(constans CommunioCliens*);` (nothing to push and the last pull returned empty - for the CLI and tests), and an option `mora` 0 for one-shot runs: `vacuum communio_cliens_moram_ponere(CommunioCliens*, i32 secundae);`.
  - Cursors in `praesentia`: `communio/<basis>/ascensus`, `communio/<basis>/descensus` (decimal text), read at create, written after each accepted batch.

- [ ] **Step 1: failing tests** - "per servum": volumes A and B (two devices of one space), server in process; A draws a picture (strokes, an image layer, undo, redo - reuse `operationes`); run both clients until `otiosus`; B's `pictor_documenta_aliena_enumerare` lists it and `pictor_documentum_aperire_alienum` composite sigillum equals A's. "vivum": B opens the foreign picture and its client runs with `mora` XXV; A draws 2 strokes, its client pushes; B's `advenerunt` fires and `pictor_documentum_sequi` makes it equal A again - without B issuing a new request (the waiting pull answered). "servus cadit": stop the server (`hospitium_destruere`) mid-run after A's first batch, A draws more, restart a server on the same volume and registry and the same port (if rebinding the port fails in the test, the test may re-point the clients - a ruling, not a skipped check), both clients reach `otiosus`: B has every act exactly once (count by replica), A's `ascensus` equals its last own seq. "cursores manent": destroy and recreate B's client -> it pulls nothing old (descensus read back). "tessera mala": a client with a wrong token -> `status.error` set, nothing imported, no crash.
- [ ] **Step 2:** FAIL. **Step 3:** implement. **Step 4:** PASS; `./compile_tests.sh communio volumen historia pictor` green; print the time for a 500-stroke picture end to end. **Step 5:** commit.

### Task 7: exemplar mounts the server; the `communio` tool

**Files:**
- Modify: `apps/exemplar/exemplar_nucleus.c`, `apps/exemplar/exemplar_internum.h` (`communio.volumen`, `CommunioServus*`), `apps/exemplar/exemplar_administratio.c` (tools), `apps/exemplar/exemplar.worklog.md`
- Create: `tools/communio.c` (via `./silva/scribe.sh`) and its build action like the other in-tree tools
- Test: `probationes/probatio_exemplar_applicatio.c` (new section); a small test for the tool's token file

**Interfaces:**
- Consumes: T3-T6.
- Produces:
  - exemplar opens `<data>/communio.volumen` (`durabilitas_plena`), creates the server with `ianua`'s `Cives*`, mounts it (no nginx change: `/communio/` already proxies through `location /`), destroys it before the hospitium on close.
  - Admin tools (scope `administratio`): `communio_membrum_addere {spatium, civis}`, `communio_apparatum_addere {civis, spatium, titulus, digestum}` -> `{id}`, `communio_apparatum_revocare {id}`, `communio_apparatus_enumerare {}` -> rows without digests. Tool count in `mc.numerus` updated.
  - `bin/communio tessera -via FILE` - mints a token into FILE (created 0600, refuses to overwrite), prints only the digest. `bin/communio replica -volumen V` - prints the volume's replica. `bin/communio currere -volumen V -servus URL -tessera FILE [-semel]` - runs the client (one-shot with `-semel`: until `otiosus`), printing the status line per cycle (never the token).

- [ ] **Step 1: failing tests** - exemplar section "communio": admin `communio_membrum_addere` + `communio_apparatum_addere` over the channel (`_administrare`), then a raw HTTP pull with the bearer token -> 200; without -> 401; `communio_apparatus_enumerare` output contains no digest. Tool: "tessera" writes a 0600 file of 43 characters and prints a 64-hex digest equal to SHA-256 of the file; a second run on the same path refuses; `currere` refuses a token file with mode 0644.
- [ ] **Step 2:** FAIL. **Step 3:** implement. **Step 4:** PASS + `./compile_tests.sh 'exemplar|communio'` green; exemplar's existing suite unchanged. **Step 5:** commit.

### Task 8: live (after the merge, on Fran's word)

Not code. Run only from main after `fusio_probare` and the fast-forward, with Fran present.

- [ ] Deploy exemplar (`tools/dimittere.sh exemplar`); `/salus` answers.
- [ ] On the Mac: `bin/communio tessera -via ~/.rhubarb/communio/macintosh.clavis` (and a second for a test volume); Fran registers both digests through the admin channel (member "familia" first).
- [ ] `bin/communio currere -semel` from pictor's volume, then from a scratch volume: the scratch volume lists Fran's pictures under `aliena/` and one opens with the same composite sigillum (checked with a small `-imago` render).
- [ ] Spec gets section VII "phase 2 built"; ledger and memory updated.

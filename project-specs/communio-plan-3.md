# communio phase 3 (sign-in by email) - Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.
> **House rule (Fran):** executed INLINE, one task per turn, Fran approves each before the next.

**Goal:** a new device signs itself in: `communio inscribere -email <addr>` mints the device's token, the server emails a link and a short code to that address if (and only if) it is on the allowlist, and confirming either one registers the device for the address's person - no admin step per device. The allowlist (email -> person, admin channel) is the root of access; email goes through a swappable provider (Resend first) on the server.

**Architecture:** a new library `epistula` is the house email API (a message, a provider table, a fake provider for tests, Resend over `http_vocare`). `communio_servus` gains the allowlist and sign-in requests (`communio_permissi`, `communio_aditus`) plus three unauthenticated routes, throttled through exemplar's existing `custodia_frenum`. exemplar reads the provider key from its data directory, wires the email sender and custodia into the communio server, and gains admin tools for the allowlist. The `communio` tool gains `inscribere`. A small `tools/secretum_mittere.sh` copies a local key file to the server without the key ever appearing anywhere.

**Tech Stack:** C89 in Latin (latina.h), `communio_servus` / `communio_cliens` (phase 2), `hospitium`, `http_vocatio` (BearSSL, system trust store on Linux), `json`, `sigillum`, `moneta`, `custodia` (frenum), `cives`, bash for the secret tool, credo v2 suites, `silva.commissio`.

**Spec:** `project-specs/communio-spec.md` - D6 (magic link to an allowlisted email; code fallback; the device gets its own credential), D7 (email platform API, swappable providers, Resend first, `Idempotency-Key`), D3 (one person per device), "Resolved with Fran (2026-10-27, before phase 3)" (allowlist = email -> person, admin may create the person; sender `communio@golden.earth`; local keys copied up by a tool), module II.6, II.7. Phases 1-2: spec sections VI, VII.

## Calls made in this plan

- **C1 Adoption (D3b) is NOT in this plan.** "Restore my old device" means a fresh volume taking over an old replica id and receiving that replica's acts as its OWN, with references in the writer's numbering - a volumen-level design (replica takeover, own-act import, seq vs seq_originis for own documents) that deserves its own plan. Phase 3 delivers sign-in; adoption becomes plan 3b. Cost if Fran disagrees: one more task family here.
- **C2 Sign-in flow.** `POST /communio/intrare {"epistula","digestum","titulus"}` (no token) -> always `202 {"aditus": <ULID>}`. Only for an allowlisted, active address is a request stored and a mail sent; otherwise a random id is returned and nothing is stored or sent - the answer is identical, so the endpoint reveals nothing. The mail carries a LINK (`<basis>/communio/confirmare?aditus=<id>&signum=<32 random bytes, base64url>`) and a CODE (8 Crockford base32 characters, shown `XXXX-XXXX`). Opening the link (GET, a small HTML page) or `POST /communio/confirmare {"aditus","codex"}` registers the device digest given at `intrare` for the address's person in the entry's space. Both secrets are stored only as SHA-256; a request lives XV minutes, is single-use, and allows V code attempts. The device then simply uses its token (the CLI waits until it works).
- **C3 Throttling** reuses exemplar's `custodia_frenum` (already guards password sign-in): keys `communio-ip:<addr>` and `communio-epistula:<addr>` on `intrare`, `communio-aditus:<id>` on code attempts. The communio server receives the Custodia through a setter; without one (tests that do not care) it does not throttle.
- **C4 Allowlist storage**: `communio_permissi (epistula TEXT PRIMARY KEY /* lowercased, trimmed */, civis TEXT, spatium TEXT, creatus_ms, revocatus)` in `communio.volumen`. The admin tool `communio_epistulam_permittere {epistula, nomen, titulus, spatium}` creates the person (in exemplar.volumen, through cives) when the handle does not exist, adds the space membership and the entry. Two volumes, so not one transaction: person first (re-running the tool is idempotent), then entry. Revoking an entry stops new sign-ins; existing devices are revoked separately (listed together by `communio_permissos_enumerare`).
- **C5 Email text is English** (the recipients read it; the code stays Latin): subject "Sign in to golden.earth", text + minimal HTML with the link, the code, the device title and "if you did not ask for this, ignore this email". `Idempotency-Key` = the request id, so a retried send never mails twice.
- **C6 `epistula` follows the vates pattern**: a provider table `{titulus, constructor}` with "resend" and "fictus" (records messages in memory for tests); the Resend provider takes an optional base URL so tests point it at a local hospitium.
- **C7 `secretum_mittere.sh`** follows `dimittere.sh` conventions (`-siccum` default prints, `-agere` acts; ssh alias), streams the key through ssh stdin into `sudo install -o <app> -g <app> -m 0600 /dev/stdin /var/lib/<app>/<name>`, refuses a local file looser than 0600 and an existing remote file unless `-renovare`, and confirms with `sha256sum` on both ends printing only `congruit` / `differt`. Tested with stub `ssh`/`sudo` on PATH and a scratch root.
- **C8 DNS**: oceanus learns MX priority (DigitalOcean requires it; Resend asks for MX on its `send` subdomain); the records themselves (values from Resend's dashboard - not secrets) are set at the live step.

## Global Constraints

- C89, house flags; Latin identifiers; `i32`/`i64` UNSIGNED; Roman numerals; `nomen` is a macro; chorda not NUL-terminated; new C files via `./silva/scribe.sh`; raw file IO only under `<tolera codex="CANALIS_PRAETERITUS">` for secrets, otherwise `filum_*`/`lectiones_*` (examen 97).
- Secrets: the Resend key, device tokens, sign-in codes and link tokens never appear in an act, a volume row (only SHA-256), an error, a log line, argv, a temp file, a commit, or Claude's context. Email addresses are personal data: stored only on the server, never in the repository (tests use `@example.org`).
- An unknown or revoked address gets exactly the same answer as an allowlisted one at `intrare` (status, body shape, no extra work visible).
- Sign-in request: XV minutes, single use, V code attempts; code 8 Crockford base32 characters; link token 32 random bytes.
- Server memory: every new route resets the server's scratch pool like the phase-2 routes (`communio_servi_usus` stays flat); the `probationes_veneni` gate keeps covering `probatio_communio_servus`.
- The email send is asynchronous (reactor); a failed send is logged (without the code) and the request stays valid until expiry.

## Review Focus

1. Enumeration: an attacker sends `intrare` for many addresses - the replies for allowlisted and unknown addresses are indistinguishable, and throttling caps the mail volume per address and per IP -> T4 "nihil prodit", "frenum".
2. Code guessing: wrong codes, an expired request, a used request, more than V attempts - all refused, nothing registered, the right code no longer works after V failures -> T3 "codex", T4 "conatus".
3. A link opened twice, or link and code both used: the device is registered once; the second confirmation is a harmless "already done" -> T3 "semel".
4. An address revoked (or its person suspended) between `intrare` and confirmation: confirmation fails, no device -> T3 "revocatus medio".
5. The provider down or the key missing: `intrare` still answers 202, the failure is logged without secrets, nothing crashes; exemplar starts without the key and says so -> T2 "provisor deficit", T5 "sine clave".

---

### Task 1: `tools/secretum_mittere.sh` - a local key to the server, unseen

**Files:**
- Create: `tools/secretum_mittere.sh`, `probationes/secretum_mittere_probatio.sh` (stub `ssh`/`sudo`/`sha256sum` on PATH, scratch root), entry in the shell-test runner the repo uses for `tools/*.sh` (follow `tools/dimittere.sh`'s test if one exists, else `compile_tests.sh`'s shell hook)

**Interfaces:**
- Produces: `tools/secretum_mittere.sh [-siccum|-agere] [-renovare] <clavis-localis> <app> <alias-ssh> [nomen-remotum]` (default remote name = basename of the local file). Exit 0 + `congruit`; 1 on refusal or `differt`.

- [ ] **Step 1: failing tests** - local file 0644 -> refused, nothing sent; `-siccum` prints the commands and never runs ssh; `-agere` with the stub writes `<root>/var/lib/<app>/<name>` mode 0600 owned per the stub's record, output contains `congruit` and NOT the key bytes nor their hash; second run without `-renovare` -> refused (remote exists); with `-renovare` -> replaced; a stub that corrupts the copy -> `differt`, exit 1; the key never appears in the recorded argv of any stub.
- [ ] **Step 2:** run -> FAIL (no script). **Step 3:** implement. **Step 4:** PASS. **Step 5:** commit (this plan and the spec's phase-3 decisions go in this commit).

### Task 2: `epistula` - the house email API

**Files:**
- Create: `include/epistula.h`, `lib/epistula.c`, `lib/epistula.worklog.md`, `probationes/probatio_epistula.c`

**Interfaces:**
- Produces:
  - `nomen structura { chorda a; chorda ad; chorda titulus; chorda textus; chorda html; chorda clavis_idempotentiae; } EpistulaNuntius;`
  - `nomen vacuum (*EpistulaFinis)(vacuum* ctx, b32 successus, chorda causa);`
  - `nomen structura Epistularius Epistularius;`
  - `Epistularius* epistularium_aperire(Piscina*, Reactor*, constans character* provisor /* "resend" | "fictus" */, chorda clavis, constans character* basis /* NIHIL = provider default */);` - NIHIL + names the known providers on stderr if unknown.
  - `b32 epistulam_mittere(Epistularius*, constans EpistulaNuntius*, EpistulaFinis, vacuum* ctx);` - asynchronous; FALSUM only for a malformed message (missing a/ad/titulus/textus).
  - `vacuum epistularium_claudere(Epistularius*);` (aborts sends in flight, zeroes the key copy)
  - fake provider: `i32 epistularius_fictus_numerus(constans Epistularius*);` and `b32 epistularius_fictus_nuntius(constans Epistularius*, i32 index, EpistulaNuntius* exitus);`
  - `chorda epistula_resend_corpus(constans EpistulaNuntius*, Piscina*);` (pure; `{"from","to":[...],"subject","text","html"}`)
- Resend: `POST <basis or https://api.resend.com>/emails`, headers `Authorization: Bearer <key>`, `Content-Type: application/json`, `Idempotency-Key`; success = 200; any other status -> `successus` FALSUM with cause `"resend: HTTP <n>"` (never the key).

- [ ] **Step 1: failing tests** - "corpus": JSON fields exact, quotes/newlines escaped, `html` omitted when empty; "fictus": two sends recorded in order with all fields; "resend per hospitium": a local hospitium route `/emails` records the request - method POST, bearer equals the test key, `Idempotency-Key` present, body = `epistula_resend_corpus`; 200 -> finis(VERUM); "provisor deficit": route answers 500 -> finis(FALSUM, "resend: HTTP 500"), cause does not contain the key; unknown provider -> NIHIL; malformed message -> FALSUM, no request.
- [ ] **Step 2:** FAIL. **Step 3:** implement (pattern: `lib/vates.c` provider table; HTTP like `lib/communio_cliens.c` `_mittere`, finish handled on the next reactor turn). **Step 4:** PASS; regenerate test sources. **Step 5:** commit.

### Task 3: allowlist and sign-in requests (no HTTP)

**Files:**
- Modify: `include/communio_servus.h`, `lib/communio_servus.c`, `lib/communio_servus.worklog.md`
- Test: `probationes/probatio_communio_servus.c`

**Interfaces:**
- Consumes: phase-2 registry (`communio_apparatum_addere`, membership), `cives`.
- Produces:
  - Tables `communio_permissi` (C4) and `communio_aditus (id TEXT PRIMARY KEY, epistula TEXT, civis TEXT, spatium TEXT, digestum TEXT /* device token digest */, titulus TEXT, codex TEXT /* sha256 */, signum TEXT /* sha256 */, creatus_ms, expirat_ms, conatus INTEGER DEFAULT 0, confirmatus INTEGER DEFAULT 0, apparatus TEXT)`; acts `communio.permissus {epistula_digestum, civis, spatium}`, `communio.permissio-revocata {epistula_digestum}`, `communio.aditus-confirmatus {id, apparatus}`. Acts carry only the address's SHA-256 (the act log is backed up and kept forever); the clear address lives only in its `communio_permissi` row, which is needed to send mail.
  - `b32 communio_epistulam_permittere(CommunioServus*, chorda epistula, chorda civis, chorda spatium);` (normalises: trim, lowercase; requires active member), `b32 communio_epistulam_revocare(CommunioServus*, chorda epistula);`, `Xar* communio_permissos_enumerare(CommunioServus*, Piscina*);` (`CommunioPermissus { chorda epistula; chorda civis; chorda spatium; b32 revocatus; }`).
  - `#define COMMUNIO_ADITUS_VITA_MS (XV * LX * M)`, `#define COMMUNIO_ADITUS_CONATUS V`, `#define COMMUNIO_CODEX_MENSURA VIII`
  - `nomen structura { character id[MONETA_ULID_MENSURA]; character codex[X]; character signum[XLIV]; b32 permissus; chorda epistula; } CommunioAditus;` (codex/signum in clear only in this struct, for the mail; zeroed by the caller after sending)
  - `b32 communio_aditum_petere(CommunioServus*, chorda epistula, chorda digestum, chorda titulus, CommunioAditus* exitus);` - always fills `id`; `permissus` VERUM (and request stored, codex/signum filled) only for an allowlisted, unrevoked, active address and a valid unused digest.
  - `s32 communio_aditum_confirmare(CommunioServus*, chorda id, chorda codex, chorda signum, character* apparatus_exitus);` - exactly one of codex/signum non-empty; 0 = registered now, 1 = already confirmed (idempotent, same device id), -1 = refused (unknown, expired, wrong secret, too many attempts, address revoked, person inactive). Code compare: constant time, case-insensitive, the dash ignored.

- [ ] **Step 1: failing tests** - "permittere": entry stored lowercased/trimmed, listing shows it, unknown person -> FALSUM; "nihil prodit (petere)": unknown address and allowlisted address both return an id of 26 characters, only the allowlisted one sets `permissus` and stores a row (count rows); "codex": right code (with dash, lower case) -> 0, device verifies with its token; wrong code V times -> -1 each and then the right code -> -1; "signum": the link token alone -> 0; "semel": confirm twice -> 1 with the same apparatus id, one device row; "expiratus": clock past XV minutes (inject `nunc_ms`) -> -1; "revocatus medio": entry revoked after `petere` -> -1; person suspended -> -1; "secretum nusquam": no act or row contains the code, the link token or an address in clear except `communio_permissi.epistula`; `communio_servi_usus` flat over 100 `petere`.
- [ ] **Step 2:** FAIL. **Step 3:** implement. **Step 4:** PASS + venenum gate green. **Step 5:** commit.

### Task 4: the sign-in routes

**Files:**
- Modify: `include/communio_servus.h`, `lib/communio_servus.c`
- Test: `probationes/probatio_communio_servus.c` (HTTP sections; fake email provider; Custodia from `custodia_creare` with a small `frenum_conatus`)

**Interfaces:**
- Consumes: T2 `epistula`, T3 requests.
- Produces:
  - `vacuum communio_servum_epistularium_ponere(CommunioServus*, Epistularius*, constans character* a /* "communio@golden.earth" */, constans character* basis /* public URL for links */);`
  - `vacuum communio_servum_custodiam_ponere(CommunioServus*, Custodia*);`
  - Routes (mounted by `communio_servum_montare`, WITHOUT the bearer check): `POST /communio/intrare` (C2; 400 on a malformed body; 429 when throttled - same for known and unknown addresses), `POST /communio/confirmare {"aditus","codex"}` -> 200 `{"apparatus"}` / 403 `{"error":"aditus"}`, `GET /communio/confirmare?aditus=&signum=` -> 200 small HTML page "Signed in: <device title>. You can close this page." or 403 page; mail (C5) sent through the Epistularius only when `permissus`, codex/signum zeroed right after the message is handed over.

- [ ] **Step 1: failing tests** - "intrare": allowlisted address -> 202, fake provider has 1 message to that address whose text contains the code and a link with the request id; unknown address -> 202 with the same body keys, provider has 0 new messages; "confirmare codex": code from the recorded mail -> 200, then a bearer pull with the device token -> 200; "confirmare signum": GET with the link from the mail -> 200 HTML, device works; "conatus": V wrong codes then the right one -> 403; "frenum": repeated `intrare` for one address beyond the limit -> 429 for it, a different address from another IP still 202; "sine epistulario": no provider set -> `intrare` still 202, a stderr line notes the unsent mail (no code in it), no crash, no device registered.
- [ ] **Step 2:** FAIL. **Step 3:** implement. **Step 4:** PASS + venenum green + `communio_servi_usus` flat across 50 intrare/confirmare. **Step 5:** commit.

### Task 5: exemplar - key, sender, custodia, admin tools

**Files:**
- Modify: `apps/exemplar/exemplar_nucleus.c`, `apps/exemplar/exemplar_internum.h`, `apps/exemplar/exemplar_administratio.c/.h`, `apps/exemplar/exemplar.worklog.md`
- Test: `probationes/probatio_exemplar_applicatio.c`

**Interfaces:**
- Consumes: T2-T4.
- Produces: exemplar reads `<data>/resend.clavis` (0600, oceanus-style reader under CANALIS_PRAETERITUS; missing -> one stderr line "exemplar: resend.clavis deest - epistulae non mittuntur" and sign-in mails are skipped), opens `epistularium_aperire(..., "resend", clavis, NIHIL)` (key buffer zeroed), sets it on the communio server with sender `communio@golden.earth` and `e->basis`, sets `e->custodia`; closes the Epistularius before the hospitium. Admin tools (now XIV): `communio_epistulam_permittere {epistula, nomen, titulus, spatium}` (creates the person if the handle is free: `cives_creare` in an exemplar.volumen transaction, then membership + entry), `communio_epistulam_revocare {epistula}`, `communio_permissos_enumerare {}`. Test hook: `ExemplarConfiguratio` gains `provisor_epistulae` (NIHIL = "resend"; tests pass "fictus").

- [ ] **Step 1: failing tests** - exemplar section "inscriptio": admin `communio_epistulam_permittere` for a new handle -> person created (cives_enumerare shows it), entry listed; raw HTTP `POST /communio/intrare` -> 202; the fake provider recorded the mail; confirm with its code -> 200; bearer pull with the device token -> 200. "sine clave": a data dir without resend.clavis starts, prints the warning, `intrare` -> 202.
- [ ] **Step 2:** FAIL. **Step 3:** implement. **Step 4:** PASS + exemplar suite green. **Step 5:** commit.

### Task 6: the device side - `communio inscribere`

**Files:**
- Modify: `include/communio_cliens.h`, `lib/communio_cliens.c`, `tools/communio.c`
- Test: `probationes/probatio_communio.c`

**Interfaces:**
- Produces:
  - `b32 communio_inscriptionem_petere(Reactor*, constans character* basis, chorda epistula, constans character* via_tesserae, chorda titulus, character aditus_exitus[MONETA_ULID_MENSURA], Piscina*);` - mints the token file (`communio_tesseram_scribere`, refuses an existing file), POSTs `intrare` with its digest, returns the request id; runs the reactor until done (one-shot helper for the tool and the tests).
  - `s32 communio_inscriptionem_confirmare(Reactor*, constans character* basis, chorda aditus, chorda codex, character apparatus_exitus[MONETA_ULID_MENSURA], Piscina*);` (0 ok, -1 refused, -2 network)
  - `b32 communio_tessera_valet(Reactor*, constans character* basis, constans character* via_tesserae, Piscina*);` (one bearer request: does the token work yet?)
  - CLI: `communio inscribere -servus URL -email ADDR -tessera FILE -titulus T` -> prints "Epistula missa ad ADDR (si permissa est). Codicem scribe aut nexum aperi." then reads a code line from stdin while polling `communio_tessera_valet` every II s (whichever comes first); prints "Apparatus inscriptus: <id>"; exit 1 after XV minutes.
- [ ] **Step 1: failing tests** (in-process server + fake provider) - "inscribere per codicem": petere -> code taken from the fake mail -> confirmare 0 -> the token works -> a client syncs a volume; "inscribere per nexum": petere -> GET link via the test's HTTP helper -> `communio_tessera_valet` VERUM; "epistula ignota": petere succeeds (202), no mail, token never valid; "tessera exsistens": petere with an existing token file -> FALSUM, file untouched.
- [ ] **Step 2:** FAIL. **Step 3:** implement. **Step 4:** PASS; tool smoke by hand (prompt, code typed). **Step 5:** commit.

### Task 7: oceanus - MX records

**Files:**
- Modify: `include/oceanus.h`, `lib/oceanus.c`, `tools/oceanus.c`
- Test: `probationes/probatio_oceanus.c`

**Interfaces:**
- Produces: `OceanusRegistrum` gains `i32 prioritas;` (0 = not sent); `oceanus_assecurare_prioritate(o, dominium, genus, titulus, data, ttl, prioritas)`; CLI `assecurare ... [ttl] [prioritas]`; the JSON body carries `"priority"` for MX; IDEM also compares priority.
- [ ] **Step 1: failing tests** (existing fake-DO server in probatio_oceanus) - MX create sends `priority`; same MX twice -> IDEM; changed priority -> MUTATUM; a ~420-character TXT value round-trips and is IDEM on the second run.
- [ ] **Step 2:** FAIL. **Step 3:** implement. **Step 4:** PASS. **Step 5:** commit.

### Task 8: live (after the merge, on Fran's word)

- [ ] Fran adds golden.earth (sending subdomain as Resend proposes) in Resend's dashboard and gives Claude the DNS record values (public, not secrets).
- [ ] `oceanus assecurare` the records (TXT SPF/DKIM, MX); Resend shows the domain verified.
- [ ] `tools/secretum_mittere.sh -agere resend.clavis exemplar smaragda` -> `congruit`; check `/etc/ssl/certs/ca-certificates.crt` exists on smaragda.
- [ ] Deploy exemplar (backup first); `/mcp` reconnect; `communio_epistulam_permittere` for Fran's address (`fran`, "familia") and Amanda's (`amanda`, "Amanda", "familia").
- [ ] Fran runs `communio inscribere` on the Mac (new token file), receives the mail, confirms by code; then by link with a second scratch device; the scratch device is revoked.
- [ ] Spec section VIII "phase 3 built"; ledger and memory.

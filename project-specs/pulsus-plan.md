# pulsus (house notifications: tokens, quiet-period batches, APNs from the server) - Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.
> **House rule (Fran):** executed INLINE, one task per turn, Fran approves each before the next.

**Goal:** a notifications capability on the communio server - devices register APNs tokens, apps report events under a key, the server waits out a per-sender quiet period and sends one notification to the rest of the family over our HTTP/2 - with pictor on iPhone (phase 1) and a signed `pictor.app` on the Mac (phase 2) as its first client.

**Architecture:** a pure payload/validation module shared by server and clients (`pulsus_forma`), a server library (`pulsus`) beside `communio_servus` in the same volume with an injected clock and an injected sender, a non-blocking APNs sender on the server's reactor (`apns_mittens`, on `http2_nexus`), exemplar wiring and admin tools, a Linux build of the HTTP/2 stack; on the client side a small `communio_cliens` addition (register, queued events, "acts accepted" hook), shell hooks in `TestaAmbitus` (permission, token, tap, background grace), and pictor's events / Notify now / transient viewer group. Phase 2 adds signing to fasciculum and app-wide data for the Mac.

**Tech Stack:** C89 in Latin (latina.h), `volumen`/`scrinium` (sqlite), `reactor` timers, `hospitium` routes, `communio_servus`, `cives`, `json`, `http2_nexus`, `apns`, `tessera_es256`, `veredarius`, `testa` shells (`lib/testa_ios.m`, macOS testa), `fasciculum`, credo v2 suites, `silva.commissio`; Go oracle `tools/oracula/http2_servus.go` as child process only; `tools/linux/aedificare.sh` / `tools/linux/probatio.sh` (docker/OrbStack) for Linux.

**Spec:** `project-specs/pulsus-spec.md` (PROPOSITUM 2026-11-02, approved by Fran the same day, commit 4f29cdb3): D1-D8, III.1-III.5, IV, V.

## Calls made in this plan

- **C1 Files and types.** `include/pulsus_forma.h` + `lib/pulsus_forma.c` (pure), `include/pulsus.h` + `lib/pulsus.c` (server), `include/apns_mittens.h` + `lib/apns_mittens.c` (server sender). Types `PulsusEventus`, `PulsusTactus`, `Pulsus`, `PulsusMissio`, `PulsusExitus`, `ApnsMittens`. Tables `pulsus_inscriptiones`, `pulsus_pendentes`.
- **C2 Item id = `<replica>:<spatium>`** - a shared picture is (origin replica, space) on every device (`pictor_communia_index(p, replica, spatium)`, `include/pictor_documentum.h:427`). This settles spec AUDIENDA 3.
- **C3 No DELETE route.** `veredarius` has GET/POST/PUT only (`include/veredarius.h`); an inscription dies by Apple's 410/BadDeviceToken or by device revocation (rows of revoked devices are filtered at send time and purged in the same pass). Re-registering replaces.
- **C4 Time is pulled, not pushed, in the core.** `pulsus_maturos_mittere(p)` sends every batch whose `ultimus_ms + mora_ms <= nunc_ms`; the reactor timer only calls it (one timer, re-armed to the earliest due batch). Tests drive the fake clock and call it directly.
- **C5 Retries live in the sender, the verdict in pulsus.** `apns_mittens` retries 429 / 5xx / `retentabilis` / transport loss at 1 s, 5 s, 30 s (then gives up), re-signs once on `ExpiredProviderToken`/`InvalidProviderToken`; it reports one final `PulsusExitus` per message. pulsus deletes the inscription on `PULSUS_SIGNUM_MORTUUM`, logs on `PULSUS_DEFECTUS`.
- **C6 Provider token** re-signed when older than XL minutes (inside spec's 20-50).
- **C7 Server config** `<data>/apns.stml`: `<apns clavis_via="apns.clavis" clavis_id= turma= thema="earth.golden.pictor" hospes="api.push.apple.com" hospes_sandbox="api.sandbox.push.apple.com"/>`; `clavis_via` relative to the data directory; `ApnsConfiguratio` gains optional `hospes_sandbox` (empty = no sandbox sends).
- **C8 Ambitus at build time:** define `PULSUS_AMBITUS` = `"sandbox"` from `tools/ios/app.sh` scopes tabula/simulator and from the phase-2 Mac build, `"productio"` from scope divulgatio; the shell passes it in `TestaAmbitus.pulsus_ambitus`. Entitlements need no script change: `app.sh` already copies them from the profile (`tools/ios/app.sh:244-251`).
- **C9 Admin tools** follow the table at `apps/exemplar/exemplar_administratio.c:822`: `pulsus_inscriptiones_enumerare`, `pulsus_probare {civis}`.
- **C10 Limits:** tessera hex II..CCLVI chars, even; thema/clavis I..CXXVIII of `[A-Za-z0-9._-]`; templates <= CCLVI octets, no control chars, `{persona}`/`{numerus}` only; res <= C per request, item id <= CCLVI, batch <= M items (extra ignored, counted); mora 0..MMMDC s; payload <= MMMMXCVI octets (build `MMMM + XCVI`, no numeral beyond MMMCMXCIX).

## Global Constraints

- C89, house flags; Latin identifiers; `i32`/`i64` UNSIGNED, `s64` signed; Roman numerals (products beyond MMMCMXCIX); `nomen`/`casus`/`magnitudo`/`interior` never identifiers; chorda not NUL-terminated; new C files via `./silva/scribe.sh`; examen 97 (raw IO only via `filum_*`/`lectiones_*`); Latin lint blocks new words (rename or glossary).
- `pulsus_forma` and `pulsus` core: no sockets; clock injected (`s64 (*nunc_ms)(vacuum)`, NIHIL = real); sender injected.
- Secrets: the APNs key is never read by Claude, printed, logged, put in argv, an error or the repo; the provider token is never logged; device tokens logged only as an VIII-char prefix. Key reaches the server only on Fran's go.
- Recipients: active members of the sender's space, other than the sender PERSON, non-revoked devices, matching thema (spec D5 + calls).
- Go only as a child-process oracle (no go.mod); a suite needing it prints `NIHIL CURSUM` when Go is absent (as `probatio_http2_nexus`).
- Apple side: create, never revoke or delete; each step on Fran's go. TestFlight from main only; ff main only on Fran's word; exemplar deploy from worktree under `worktree-deploys-exemplar` conditions.
- Fran approves each task.

## Review Focus

1. A batch is due while the APNs connection is down or Apple answers 5xx for minutes: the notification is retried within its bounds, then dropped and logged - the server never blocks, never sends twice for a 200 -> T4 "iteratio finita".
2. The server restarts (deploy) between an event and its due time, or after the due time: the batch is sent exactly once after start-up -> T2 "restitutio".
3. A device re-registers with a new token (reinstall, iOS rotation) or two devices of one person register: old token replaced per device, both devices of a recipient get it, neither of the sender's -> T2 "inscriptio renovata".
4. A tap arrives for pictures not synced yet, partly deleted, or a payload truncated (`plures`): synced first, unknown skipped, grid on none -> T10 "grex incompletus".
5. Hostile or broken input on the routes (oversize templates, control chars, non-hex token, 10 000 item ids, unknown thema): 400 with nothing stored, never memory growth -> T3 "bombae".

---

### Task 1: `pulsus_forma` - events, payload, tap, templates (pure)

**Files:**
- Create: `include/pulsus_forma.h`, `lib/pulsus_forma.c`, `probationes/probatio_pulsus_forma.c`, `lib/pulsus.worklog.md`

**Interfaces:**
- Produces:
  - `nomen structura { chorda clavis; chorda thema; Xar* res; i32 mora_s; chorda unus; chorda plures; b32 statim; } PulsusEventus;` (res = Xar of chorda)
  - `b32 pulsus_eventum_legere(chorda json, Piscina* p, PulsusEventus* e, chorda* causa);` - C10 limits; FALSUM + causa names the field.
  - `chorda pulsus_eventum_scribere(constans PulsusEventus* e, Piscina* p);` (client side)
  - `b32 pulsus_inscriptionem_legere(chorda json, Piscina* p, chorda* tessera, chorda* ambitus, chorda* thema, chorda* causa);`
  - `chorda pulsus_textum_facere(chorda forma, chorda persona, i32 numerus, Piscina* p);` - replaces `{persona}`, `{numerus}` (decimal), nothing else.
  - `chorda pulsus_corpus_facere(chorda titulus, chorda textus, chorda clavis, chorda a, constans Xar* res, Piscina* p);` - payload of spec III.1 (`sound "default"`, `thread-id` = clavis); drops trailing ids and sets `"plures":true` until <= MMMMXCVI octets.
  - `nomen structura { chorda clavis; chorda a; Xar* res; b32 plures; } PulsusTactus;` `b32 pulsus_tactum_legere(chorda json, Piscina* p, PulsusTactus* t);` - reads the `pulsus` object of a delivered payload.

- [ ] **Step 1: failing tests** - "eventus": pictor's event round-trips scribere->legere; each C10 limit rejected with its field in causa (mora 3601, template with `\x01`, `{nomen}` placeholder, 101 ids, thema `a b`). "textus": `"{persona} shared {numerus} pictures"`, Fran, 3 -> `Fran shared 3 pictures`; `"{persona} shared a picture"` -> `Fran shared a picture`. "corpus": 3 ids -> exact JSON of spec III.1; 400 ids of 40 chars -> length <= 4096, `"plures":true`, ids a prefix in order. "tactus": corpus_facere output -> tactum_legere gives the same clavis/a/res/plures; a payload without `pulsus` -> FALSUM.
- [ ] **Step 2:** `./compile_tests.sh pulsus_forma` -> FAIL (no header). **Step 3:** implement (house `json`). **Step 4:** PASS; `./tools/compile_tests_fontes_generare.sh`. **Step 5:** commit via `silva.commissio` (this plan goes in this commit).

### Task 2: `pulsus` core - inscriptions, batches, recipients, restitution

**Files:**
- Create: `include/pulsus.h`, `lib/pulsus.c`, `probationes/probatio_pulsus.c`
- Modify: `lib/pulsus.worklog.md`

**Interfaces:**
- Consumes: T1; `communio_apparatus_enumerare` (`include/communio_servus.h`), `cives_per_id` (`include/cives.h`).
- Produces:
  - `nomen enumeratio { PULSUS_TRADITUM = 0, PULSUS_SIGNUM_MORTUUM, PULSUS_DEFECTUS } PulsusExitus;`
  - `nomen structura { chorda signum; chorda ambitus; chorda thema; chorda corpus; chorda apparatus; } PulsusMissio;`
  - `nomen vacuum (*PulsusFinis)(vacuum* ctx, constans PulsusMissio* missio, PulsusExitus exitus, chorda ratio);` - the sender hands back its own copy of the message (T2 ruling: pulsus needs to know which token finished without a per-message allocation). A `PulsusMissio` is read only inside `mittere`; the sender copies what it keeps.
  - `nomen structura { chorda apparatus; chorda thema; chorda tessera; chorda ambitus; s64 creatus_ms; s64 usus_ultimus_ms; } PulsusInscriptio;` (enumerare rows; tessera shortened)
  - `nomen b32 (*PulsusMittere)(vacuum* sui, constans PulsusMissio* m, PulsusFinis finis, vacuum* ctx);`
  - `Pulsus* pulsus_creare(Piscina* p, Volumen* v, CommunioServus* servus, Cives* cives, s64 (*nunc_ms)(vacuum));` - tables created if absent.
  - `vacuum pulsus_mittentem_ponere(Pulsus* p, PulsusMittere m, vacuum* sui);` (NIHIL = log "pulsus: apns absens - non mittitur")
  - `b32 pulsus_inscribere(Pulsus* p, chorda apparatus, chorda tessera, chorda ambitus, chorda thema);` - replaces the row of (apparatus, thema).
  - `b32 pulsus_eventum_addere(Pulsus* p, chorda apparatus, constans PulsusEventus* e);` - sender = the device's civis and spatium; statim -> sends now.
  - `i32 pulsus_maturos_mittere(Pulsus* p);` - returns batches sent (C4).
  - `s64 pulsus_proximum_ms(constans Pulsus* p);` - earliest due time, -1 if none.
  - `Xar* pulsus_inscriptiones_enumerare(Pulsus* p, Piscina* q);` (rows; tessera shortened to VIII chars)
  - `b32 pulsus_probare(Pulsus* p, chorda civis);` - fixed test notification to that person's devices.
  - `vacuum pulsus_destruere(Pulsus* p);`

- [ ] **Step 1: failing tests** (fake clock, fake sender recording `PulsusMissio`s; servus with persons Fran, Amanda, Petrus(other space); devices Fran-iPhone, Fran-Mac, Amanda-iPhone, Amanda-iPad, Amanda-old(revoked)) - "quies": events at t=0, 120 s, 240 s with mora 300 -> nothing at 539 s, one notification at 540 s to Amanda-iPhone and Amanda-iPad only, body `Fran shared 3 pictures`, ids in first-seen order, duplicates once. "statim": event with statim -> sent at once, row gone; statim with no ids and no pending batch -> nothing sent. "per mittentem": Fran and Amanda events interleaved -> two notifications, each to the other's devices. "per clavem": two clavis for Fran -> two notifications. "sine fine": events every 200 s for an hour -> nothing until 300 s after the last (no max wait). "restitutio" (Review Focus 2): event, destroy, create a new Pulsus on the same volume -> `pulsus_proximum_ms` = original due; clock past due -> sent once; third instance sends nothing. "inscriptio renovata" (Review Focus 3): Amanda-iPhone re-registers a new token -> only the new one used; Fran's devices never receive. "mortuum": finis `PULSUS_SIGNUM_MORTUUM` -> row deleted, next batch skips it. "revocatus": revoked device's row purged at send. "probare": Amanda -> both her devices, fixed text.
- [ ] **Step 2:** `./compile_tests.sh pulsus` -> FAIL. **Step 3:** implement. **Step 4:** PASS (and `./compile_tests.sh communio` still green). **Step 5:** commit.

### Task 3: routes and exemplar wiring

**Files:**
- Modify: `include/pulsus.h`, `lib/pulsus.c`, `apps/exemplar/exemplar_nucleus.c`, `apps/exemplar/exemplar_internum.h`, `apps/exemplar/exemplar_administratio.c`, `probationes/probatio_pulsus.c`, `probationes/probatio_exemplar.c`

**Interfaces:**
- Consumes: T2; `communio_apparatum_verificare`, `hospitium_praebere`, `hospitium_reactor`, `reactor_timer_adicere`/`_cancellare`.
- Produces:
  - `b32 pulsus_montare(Pulsus* p, Hospitium* h);` - `PUT /communio/pulsus/inscriptio` -> 204, `POST /communio/pulsus/eventus` -> 202; Bearer via `communio_apparatum_verificare` (401); 400 on any `pulsus_*_legere` failure, nothing stored; one reactor timer re-armed to `pulsus_proximum_ms` after every change and every fire.
  - exemplar: `Pulsus` created after the communio server, mounted after it, destroyed before it; admin tools C9.

- [ ] **Step 1: failing tests** - probatio_pulsus "viae": over a real hospitium on 127.0.0.1 (as probatio_communio_servus does): no Bearer -> 401; good PUT -> 204 and row present; good POST with mora 1 -> after the reactor runs ~1.2 s the fake sender saw one message. "bombae" (Review Focus 5): template 300 octets, `\x07` in template, token `zz`, 10 000 ids, body 2 MB -> 400 each, tables unchanged, `communio_servi_usus`-style footprint stable across 1000 bad requests. probatio_exemplar "pulsus": exemplar data dir without apns files starts, logs `pulsus: apns absens`, routes answer; admin `pulsus_inscriptiones_enumerare` lists a registered device with token shortened.
- [ ] **Step 2:** run both -> FAIL. **Step 3:** implement. **Step 4:** PASS. **Step 5:** commit.

### Task 4: `apns_mittens` - non-blocking sender on the reactor

**Files:**
- Create: `include/apns_mittens.h`, `lib/apns_mittens.c`, `probationes/probatio_apns_mittens.c`
- Modify: `include/apns.h` / `lib/apns.c` (`hospes_sandbox`, C7), `tools/oracula/http2_servus.go` (`/3/device/` answers by token prefix: `c8` 200, `c410` 410 Unregistered, `c429` 429 twice then 200, `c403` 403 ExpiredProviderToken once then 200, `c500` always 500), `tools/apns.c` (blocking path on top of the sender), `lib/apns.worklog.md`

**Interfaces:**
- Consumes: `http2_nexus_*`, `tessera_apns_facere`, T2's `PulsusMissio`/`PulsusFinis`/`PulsusExitus`.
- Produces:
  - `ApnsMittens* apns_mittentem_aperire(Reactor* r, constans ApnsConfiguratio* c, constans i8* clavis_der, i32 mensura, s64 (*nunc_ms)(vacuum), Piscina* p);` - copies the key into its own memory (zeroed on close); one `Http2Nexus` per ambitus, opened lazily, reopened after death.
  - `b32 apns_mittens_mittere(vacuum* sui, constans PulsusMissio* m, PulsusFinis finis, vacuum* ctx);` - matches `PulsusMittere`; headers spec III.1; retries and re-sign per C5/C6.
  - `vacuum apns_mittentem_claudere(ApnsMittens* m);` - pending messages finish `PULSUS_DEFECTUS` before return.
  - `vacuum apns_mittentis_moras_ponere(ApnsMittens* m, constans i32* moras_ms, i32 n);` (tests: short backoff)
  - `vacuum apns_mittentis_vecturam_ponere(ApnsMittens* m, b32 (*petere)(vacuum* sui, constans HpackCaput* capita, i32 n, chorda corpus, Http2NexusFinis finis, vacuum* ctx), vacuum* sui);` - test seam replacing the `http2_nexus_petere` call (NIHIL = real nexus).

- [ ] **Step 1: failing tests** - unit (always run, through the transport seam): token reused at 39 min, re-signed at 40 min; ExpiredProviderToken -> one re-sign + retry; 429,429,200 -> TRADITUM after 2 retries; 500 x4 -> DEFECTUS after 3 retries ("iteratio finita", Review Focus 1); 410 and 400 BadDeviceToken -> SIGNUM_MORTUUM, no retry; 200 never retried; claudere with 2 pending -> 2 DEFECTUS. Oracle (NIHIL CURSUM without Go): the five token prefixes against `http2_servus.go` with short moras give TRADITUM, MORTUUM, TRADITUM, TRADITUM, DEFECTUS; payload and `apns-topic`/`apns-push-type`/`apns-expiration` seen by the server. `./compile_tests.sh apns` (old suite) stays green.
- [ ] **Step 2:** run -> FAIL. **Step 3:** implement; rebuild `tools/apns.c` via `bin/aedilis tools/apns.c && bash build/aedilis/apns/struere.sh`. **Step 4:** PASS. **Step 5:** commit.

### Task 5: exemplar sends - config, key, sender wired

**Files:**
- Modify: `apps/exemplar/exemplar_nucleus.c`, `probationes/probatio_exemplar.c`, `apps/exemplar/exemplar.worklog.md`

**Interfaces:**
- Consumes: T3, T4, `apns_configurationem_legere`, `tessera_clavem_legere`.
- Produces: at start-up exemplar reads `<data>/apns.stml` + `<data>/apns.clavis` (0600 check by `tessera_clavem_legere`), opens `apns_mittens` and sets it on pulsus; either missing or invalid -> pulsus stays without sender and the log line says which file (never content).

- [ ] **Step 1: failing tests** - probatio_exemplar "apns": data dir with apns.stml pointing at the Go oracle (`hospes 127.0.0.1`, `portus`, `ancora`) and a test key from `tessera_es256`'s seeded generator (T10 of http2) written 0600 -> `pulsus_probare` delivers to the oracle (NIHIL CURSUM without Go); key 0644 -> log names `apns.clavis` and `0600`, no send; stml missing -> `apns absens`.
- [ ] **Step 2-4:** FAIL -> implement -> PASS. **Step 5:** commit.

### Task 6: Linux build of the stack

**Files:**
- Modify: whatever Linux exposes in `lib/hpack.c`, `lib/http2*.c`, `lib/apns*.c`, `lib/tessera_es256.c`, `lib/pulsus*.c`, `lib/tls13*.c` (portability only), their worklogs

**Interfaces:** none new.

- [ ] **Step 1:** `./tools/linux/probatio.sh -musl hpack http2_forma http2 tessera_es256 pulsus_forma pulsus apns_mittens` -> record what fails (compile or run). Expected: some failures (first Linux compile).
- [ ] **Step 2:** fix each at its cause (no `#ifdef` unless the platform truly differs; record each in the file's worklog). **Step 3:** rerun -> all green (Go-oracle cases may print NIHIL CURSUM in the container - acceptable, they ran on the Mac). **Step 4:** `./tools/linux/aedificare.sh apps/exemplar/exemplar.c` -> exit 0, `build/linux/exemplar` produced. **Step 5:** commit.

### Task 7: `communio_cliens` - register, queued events, "acts accepted" hook

**Files:**
- Modify: `include/communio_cliens.h`, `lib/communio_cliens.c`, `probationes/probatio_communio.c`

**Interfaces:**
- Consumes: T1 (`pulsus_eventum_scribere`), `veredarius`.
- Produces:
  - `nomen vacuum (*CommunioMissa)(vacuum* ctx, constans chorda* spatia, i32 numerus);` `vacuum communio_cliens_missa_ponere(CommunioCliens* c, CommunioMissa f, vacuum* ctx);` - after each POST /communio/acta, the distinct spatia of own acts the server accepted.
  - `vacuum communio_cliens_pulsum_inscribere(CommunioCliens* c, chorda tessera, chorda ambitus, chorda thema);` - PUT now and again after each (re)start until 204.
  - `vacuum communio_cliens_pulsum_nuntiare(CommunioCliens* c, constans PulsusEventus* e);` - appended to a queue table `communio_pulsus_cauda` in the client volume, posted in order, deleted on 202, kept on failure (retried with the client's existing backoff and on next start); 400 -> deleted and logged.

- [ ] **Step 1: failing tests** (fake veredarius, as the suite's existing cases) - "missa": 3 own acts in two spatia accepted -> hook once with the 2 spatia; refused acts not reported. "inscriptio": PUT body exact; 500 then 204 -> two PUTs, none after. "cauda": event while veredarius fails -> row kept; client destroyed and recreated on the same volume -> posted on start, row gone; two events keep order; 400 -> row deleted.
- [ ] **Step 2-4:** FAIL -> implement -> PASS. **Step 5:** commit.

### Task 8: shell hooks - permission, token, tap, background grace (iOS)

**Files:**
- Modify: `include/testa_cursus.h`, `lib/testa_ios.m`, `lib/testa.c` / `lib/testa_android.c` (fields left NIHIL), `tools/ios/app.sh` (C8 define), `probationes/probatio_testa.c`

**Interfaces:**
- Produces in `TestaAmbitus`:
  - `nomen vacuum (*TestaPulsusSignum)(vacuum* ctx, chorda tessera_hex, chorda causa);` (empty tessera + causa = refused/failed)
  - `nomen vacuum (*TestaPulsusTactus)(vacuum* ctx, chorda corpus_json);`
  - `nomen b32 (*TestaPulsumPetere)(vacuum* sui, TestaPulsusSignum signum, TestaPulsusTactus tactus, vacuum* ctx);` fields `pulsum_petere`, `pulsus_ctx`, `constans character* pulsus_ambitus` - NIHIL where unsupported.
  - iOS behaviour: `pulsum_petere` asks alert+sound, registers, delivers the token (hex lowercase) on every launch iOS gives one; the UNUserNotificationCenter delegate is set in `didFinishLaunching` so a cold-launch tap is held and delivered on the first `pulsum_petere`; `willPresent` -> no banner (app in foreground); `sceneDidEnterBackground` -> `beginBackgroundTask`, reactor timers keep running, task ended after XXV s or on expiry.

- [ ] **Step 1: failing test** - probatio_testa "pulsus": `testa_signum_hex(constans i8* octeti, i32 n, Piscina*)` (new pure helper in `lib/testa_cursus.c`, used by the .m) gives lowercase hex for a 32-octet token; macOS/Android ambitus have `pulsum_petere == NIHIL`. **Step 2:** FAIL. **Step 3:** implement (the .m paths are checked on the device in T11). **Step 4:** PASS; `./tools/ios/app.sh apps/pictor/pictor.c -simulator` builds. **Step 5:** commit.

### Task 9: pictor - ask, report, Notify now

**Files:**
- Modify: `lib/pictor_communio.c`, `include/pictor_communio.h`, `lib/pictor_componentia.c` (⋯ entry), `lib/pictor_actiones.c`, `apps/pictor/pictor.c`, `probationes/probatio_pictor_communio.c`, `probationes/probatio_pictor_componentia.c`

**Interfaces:**
- Consumes: T7, T8.
- Produces: `vacuum pictor_communio_pulsum_parare(PictorCommunio* pc, constans TestaAmbitus* a);` - after sign-in succeeds or at start when already signed in: `pulsum_petere`; token -> `communio_cliens_pulsum_inscribere(.., thema "earth.golden.pictor")`. Missa hook -> event `clavis "pictor.picturae"`, res `<own replica>:<spatium>` per spatium, mora 300, templates `"{persona} shared a picture"` / `"{persona} shared {numerus} pictures"`. Action `"pulsus.statim"` ("Notify family now", shown only when signed in) -> statim event with the open picture's id, or with no ids when nothing is open (the server then flushes Fran's pending batch, or does nothing).

- [ ] **Step 1: failing tests** - "pulsus eventus": fake client; acts accepted for 2 spatia -> one queued event with 2 ids `<replica>:<sp>`; not signed in -> nothing, `pulsum_petere` never called. "pulsus statim": menu lists "Notify family now" only when signed in; action with an open picture -> statim event with its id. **Step 2-4:** FAIL -> implement -> PASS. **Step 5:** commit.

### Task 10: tap -> transient group in the viewer

**Files:**
- Modify: `include/pictor_documentum.h`, `lib/pictor_documentum.c`, `lib/pictor_actiones.c`, `lib/pictor_communio.c`, `probationes/probatio_pictor_communia.c`

**Interfaces:**
- Consumes: T1 `pulsus_tactum_legere`, T8 tactus hook.
- Produces: `b32 pictor_communia_gregem_ponere(PictorCommunia* p, constans Xar* res);` - group = ids resolved via `pictor_communia_index` in given order, unknown skipped; FALSUM if none resolve. Viewer swipe / ← → loop over the group while set; × clears it and shows the Shared grid page 1. Tap handler: not synced -> `communio_cliens_excitare`, grid "loading" line, retry the group after the next `advenerunt`; still nothing (or `plures`) -> Shared grid.

- [ ] **Step 1: failing tests** - "grex": ids [B, X(unknown), A] -> viewer on B, next A, next B (loop), X skipped; × -> grid page 1, group cleared. "grex incompletus" (Review Focus 4): all unknown -> FALSUM, grid shown; `plures` -> grid; ids arriving after one `advenerunt` -> group opens then. **Step 2-4:** FAIL -> implement -> PASS. **Step 5:** commit.

### Task 11: phase 1 live - Apple side, server, Fran's phone (with Fran)

**Files:** none in the repo beyond a spec STATUS line / worklogs.

- [ ] **Step 1 (Fran's go):** enable PUSH_NOTIFICATIONS on `earth.golden.pictor` (`nundinae_apple -capacitas pictor PUSH_NOTIFICATIONS`), create the explicit iOS development profile (Fran's and Amanda's phones) and regenerate the App Store profile. Expected: `apns -praeparare` reports PRAESENS.
- [ ] **Step 2 (Fran's go):** install `apns.clavis` (0600, owner exemplar) and `apns.stml` in the server data dir over ssh; deploy exemplar (worktree-deploy conditions; volume backup first). Expected: log without `apns absens`.
- [ ] **Step 3:** `./tools/ios/app.sh apps/pictor/pictor.c -tabula -currere` on Fran's phone (sandbox); allow notifications; `pulsus_inscriptiones_enumerare` shows it; `pulsus_probare` Fran -> banner on the locked phone; tapping it opens pictor. (Fran's own shares never notify Fran - the real recipient check is Amanda's phone in Step 5.)
- [ ] **Step 4:** merge to main (fusio_probare, ff on Fran's word). **Step 5 (main, Fran's go):** TestFlight build via `tools/ios/divulgare.sh pictor`; Amanda updates and allows notifications; Fran shares 3 pictures from his phone and stops -> ~5 min later Amanda's locked phone shows `Fran shared 3 pictures`; tap -> the three in the viewer group (T10). Record apns-ids in the spec.

### Task 12: phase 2 - signed `pictor.app`, app-wide data

**Files:**
- Modify: `include/fasciculum.h`, `lib/fasciculum.c`, `probationes/probatio_fasciculum.c`, `apps/pictor/pictor.c`, `lib/pictor_applicatio.c`; Create: `apps/pictor/app.sh` (builds `build/pictor.app` via fasciculum)

**Interfaces:**
- Produces: fasciculum option `{ identitas_signandi, profilum_via, iura_via }` (empty = ad-hoc, as today) -> `codesign --sign <identitas> --entitlements <iura>` with `embedded.provisionprofile` copied in; pictor: app-wide data dir `~/Library/Application Support/pictor/` holding `ultimum.stml` (`<ultimum via="/abs/path/pictor.volumen"/>`); resolution order `-volumen` > `ultimum.stml` > `pictor.volumen` in cwd; the path written on each open; nothing moved, `.tessera` untouched.

- [ ] **Step 1: failing tests** - fasciculum "signatio": with the option set, the planned codesign argv and the copied profile path are as specified (planned commands are data - as the existing fumus does - no real signing in tests); empty option -> today's ad-hoc output byte-identical. pictor "sedes": temp HOME -> first run with `-volumen /tmp/x/p.volumen` writes `ultimum.stml`; second run without args opens it; cwd volume used only when neither. **Step 2-4:** FAIL -> implement -> PASS. **Step 5:** commit.

### Task 13: phase 2 - Mac push hooks and live check (with Fran)

**Files:**
- Modify: `lib/testa.c` (macOS shell; and `lib/fenestra_macos.m` where the app delegate lives), `apps/pictor/app.sh`

- [ ] **Step 1:** the T8 hooks on macOS (UNUserNotificationCenter, `registerForRemoteNotifications` on NSApplication, tap -> tactus; foreground suppressed); `pulsus_ambitus "sandbox"`. Test: probatio_testa "pulsus" now expects `pulsum_petere != NIHIL` on macOS when built as an app bundle (flag from fasciculum build), NIHIL for `bin/pictor`.
- [ ] **Step 2 (Fran's go):** App ID covers macOS (or a Mac App ID + thema - then add it to `apns.stml`), register Fran's Mac, create the macOS development profile with push.
- [ ] **Step 3:** `apps/pictor/app.sh` -> `pictor.app` opens the last volume, still signed in; `pulsus_probare` Fran -> Mac banner. **Step 4:** Amanda shares from her phone -> `Amanda shared N pictures` on the Mac; click -> the group. Record in the spec. **Step 5:** commit.

### Task 14: final review and close

- [ ] **Step 1:** `review-package` over the branch; fable reviewer with spec, plan, Review Focus, ledger rulings. **Step 2:** fix Critical/Important with RED->GREEN; minors to the tabularium. **Step 3:** spec STATUS IMPLETUM + section "as built"; merge main in, fusio_probare, ff on Fran's word; delete the plan workspace.

# communio phase 5 (iPhone + pictor) - Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.
> **House rule (Fran):** executed INLINE, one task per turn, Fran approves each before the next.

**Goal:** pictor syncs by itself on the iPhone and the Mac: a welcome screen and in-app sign-in by email, a "Shared pictures" gallery of every other household device's pictures (newest first), a read-only viewer that swipes and loops and offers "Edit a copy", and credits ("by Amanda · iPhone") - with the iPhone speaking HTTPS only through Apple's URLSession and Amanda on her own phone through TestFlight.

**Architecture:** a small house interface `veredarius` (one HTTP request, a finish callback, abort) replaces the client's two direct `http_vocare` users; the Mac shell and tools back it with the house client, the iOS shell with URLSession, Android with nothing. The shell also hands the app a reactor it keeps running (timers on iOS, sockets + timers on the Mac). Sign-in becomes a non-blocking state object (`CommunioInscriptio`) whose pending state lives in the volume. pictor gains `pictor_communio` (client + device list + credits), a second gallery over the foreign pictures phase 1 already stores, a viewer dialog, "edit a copy", and the welcome / sign-in dialogs; the iOS shell gains URLSession, reactor timers and the system keyboard.

**Tech Stack:** C89 in Latin (latina.h), Objective-C (MRC) only inside `lib/testa_ios.m`, `communio_cliens` / `communio_servus` (phases 2-3, unchanged on the server), `http_vocatio` (Mac/tools only), `reactor`, `volumen` (praesentia), pictor compositio (`pictor_componentia`, `pictor_actiones`, organum.stml), `elenchus`, credo v2 suites, `silva.commissio`, `tools/ios/app.sh`, `tools/ios/divulgare.sh` (main only).

**Spec:** `project-specs/communio-spec.md` section IX (phase 5 design, APPROBATUM 2026-10-27), with D4 (single-writer, edit a copy), D9/D12 (ULIDs), D10, D11 (URLSession), section VIII (what phase 3 built). Gallery: `project-specs/pictor-pinacotheca-spec.md` D2-D9. Shell rules: `include/testa.h` REGULAE 1-3.

## Calls made in this plan

- **C1 The shell provides network and loop.** `TestaAmbitus` gains `Reactor* reactor` (shell-owned and shell-pumped) and `Veredarius* veredarius` (NIHIL = this platform has no network yet). macOS: house client on that reactor; iOS: URLSession, reactor holds timers only; Android: both NIHIL. It also gains `constans character* machina` ("Mac", "iPhone", "Android") - the device title sent at sign-in (spec IX: titled by kind). pictor and `communio_cliens` include only `veredarius.h`, never `http_vocatio.h`, so the iOS and Android builds link no TLS (spec D11: no bundled cryptography).
- **C2 One sign-in object.** `CommunioInscriptio` replaces the three blocking helpers of plan-3 T6 (`communio_inscriptionem_petere/_confirmare`, `communio_tessera_valet`); the `communio inscribere` command is rewritten on top of it, so the app and the command share one path. While waiting it checks the token every II s (link clicked), accepts a code at any time, and gives up after XV minutes (the server's limit).
- **C3 Pending sign-in survives the app being killed** (testa REGULA 3; the person leaves pictor for Mail): the object writes `{aditus, incepta_ms}` (never the email address) into the volume's praesentia `communio/inscriptio` and pictor resumes it at start while the token file exists and XV minutes have not passed.
- **C4 Token file = the volume path + `.tessera`** (`pictor.volumen.tessera`, 0600, via `communio_tesseram_scribere/_legere`). A pictor run from a worktree with its own volume can never pick up the Mac's real token. `.gitignore` gains `/pictor.volumen.tessera`. On 401 (device revoked) pictor stops syncing and DELETES the token file, so "Sign in..." works again (plan-3 deferred minor 5).
- **C5 Server address** `https://exemplar.golden.earth`, overridable with `-servus <url>` (tests use a local hospitium); `-fumus` never syncs. The server needs NO change in this phase (device list `GET /communio/apparatus` already returns replica, title, person) - no deploy.
- **C6 New pictures get a ULID spatium** (`moneta_ulid`, 26 characters) instead of `pictura-<n>`; existing pictures keep their names (D9).
- **C7 Shared list order** = the writer's time (`hora`) of each picture's latest act, newest first; ties by replica, then spatium. The viewer remembers the picture's IDENTITY (`replica:spatium`), not its index, so arrivals never jump it.
- **C8 Edit a copy** = a new own picture (C6) at this device's canvas size whose single layer is the foreign composite fitted in, through the same path Import image uses (glutinatio confirmed at once); the copy's `documentum` plagula records `origo="<replica>:<spatium>"`.
- **C9 Text field is pictor's own** (spec IX my call): text in a transient insula field, `EVENTUS_TEXTUS` appends, `RETRORSUM` deletes, `REDITUS` submits. The shell service `claviaturam_ponere` shows the system keyboard (email keyboard / code keyboard with one-time-code autofill) or hides it; NIHIL on the Mac (physical keyboard).
- **C10 Copy (English, exact):** menu "My pictures…", "Shared pictures…", "New picture", "Import image…", "Sign in…"; welcome "Sign in…", "Draw without signing in"; sign-in "Email address", "Send", "Check your email", "Tap the link we sent, or type the code here.", "Code", "Cancel", "That code isn't right.", "This sign-in expired. Start again.", "Can't reach the server. Try again.", "Signed in as %s"; gallery "No shared pictures yet.", "Sign in to see shared pictures."; viewer "by %s · %s", "Edit a copy". (Typographic … and ·, as the gallery already uses.)

## Global Constraints

- C89, house flags; Latin identifiers; `i32`/`i64` UNSIGNED; Roman numerals; `nomen`/`longus` are macros; chorda not NUL-terminated; new C files via `./silva/scribe.sh`; examen 97 (raw IO only via `filum_*`/`lectiones_*`; secrets under `<tolera codex="CANALIS_PRAETERITUS">` at open sites only).
- Secrets: the device token never appears in a log line, an error, argv, a volume row, an act or Claude's context; the sign-in code is never stored; the email address is never stored on the device (only typed into the field) and never in the repository (tests use `@example.org`).
- testa REGULAE: no platform call in shared material (services come from the shell); nothing blocks and no private loop; state that matters is in the insula or the volume.
- iOS bundles no cryptography (no BearSSL in the iOS binary - check its symbols); Android links no networking (`android-struere` gate stays green); TestFlight publishing only from main's checkout.
- Shared pictures are read-only everywhere: no path from the viewer appends an act to a foreign document.
- One HTTP path in the client: every request goes through `Veredarius` (a test fails if `communio_cliens.c` references `http_vocare`).
- Fran approves each task; never ff main or publish without his word; on Apple's side create what is missing, never revoke or delete.

## Review Focus

1. The app is killed (or suspended for long) while the person is in Mail: reopening pictor resumes the sign-in and completes it when the link was clicked -> T2 "resumptio", T7 "resumptio applicationis".
2. The phone sleeps / the network drops during the long-poll: on resume the client re-polls at once, nothing is imported twice, the grid catches up -> T5 "resumptio".
3. New pictures and strokes arrive while the Shared grid or the viewer is open: the viewer stays on the same picture and swipe order stays consistent -> T6 "vivus".
4. The device is revoked (401): pictor stops syncing without a retry storm, deletes the token, and "Sign in…" comes back -> T5 "revocatus".
5. Typed input: a code pasted as "abcd-efgh" or with spaces, a very long address, non-ASCII - normalised or refused with the right message, never a crash -> T7 "codex normalis".

---

### Task 1: `veredarius` - one HTTP request interface; the client moves onto it

**Files:**
- Create: `include/veredarius.h`, `lib/veredarius_domesticus.c` (house implementation over `http_vocatio`), `probationes/probatio_veredarius.c`
- Modify: `include/communio_cliens.h`, `lib/communio_cliens.c`, `lib/communio_cliens.worklog.md`, `tools/communio.c`, `probationes/probatio_communio.c`

**Interfaces:**
- Produces:
  - `nomen structura { HttpMethodus methodus; constans character* url; constans character* auctoritas; /* "Bearer …" aut NIHIL */ chorda corpus; constans character* mimen; i32 tempus_ms; } VeredariusPetitio;`
  - `nomen structura { b32 successus; i32 status; chorda corpus; chorda causa; } VeredariusResponsum;` (corpus in the caller's piscina; causa never contains the URL query or auctoritas)
  - `nomen vacuum (*VeredariusFinis)(vacuum* ctx, constans VeredariusResponsum* r);`
  - `nomen structura Veredarius { b32 (*petere)(vacuum* sui, constans VeredariusPetitio* p, Piscina* piscina, VeredariusFinis finis, vacuum* ctx, i32* signum); vacuum (*abrumpere)(vacuum* sui, i32 signum); vacuum* sui; } Veredarius;` - contract: `finis` is never called inside `petere`, never after `abrumpere`, exactly once otherwise.
  - `Veredarius* veredarium_domesticum_creare(Piscina*, Reactor*);`
  - `communio_clientem_creare(Piscina*, Reactor*, Veredarius*, Volumen*, basis, tessera, advenerunt, ctx)` (Veredarius added; Reactor stays for timers).

- [ ] **Step 1: failing tests** - "domestica" (local hospitium): GET and POST with body + bearer arrive intact, status and body returned, finis on a later reactor turn; "abrumpere": finis never called; "tempus": a route that never answers -> successus FALSUM after tempus_ms, causa without the URL; "communio per veredarium fictum": a fake Veredarius in the test records every request the client makes during a push/pull cycle against canned answers - all requests went through it; "nullus http_vocare": the test reads `lib/communio_cliens.c` and fails if it contains `http_vocare` or `http_vocatio.h`; the existing communio suite (21) passes on the house Veredarius.
- [ ] **Step 2:** run -> FAIL. **Step 3:** implement (`_mittere` and `_vocare_semel` become one `_petere` on `Veredarius`; finish handling stays "next reactor turn"). **Step 4:** PASS; regenerate test sources. **Step 5:** commit (this plan goes in this commit).

### Task 2: `CommunioInscriptio` - sign-in that never blocks

**Files:**
- Modify: `include/communio_cliens.h`, `lib/communio_cliens.c`, `tools/communio.c`, `probationes/probatio_communio.c`, `probationes/probatio_communio_servus.c` (only if a shared fixture moves)

**Interfaces:**
- Consumes: T1 `Veredarius`.
- Produces:
  - `nomen enumeratio { INSCRIPTIO_PETIT, INSCRIPTIO_EXSPECTAT, INSCRIPTIO_CONFIRMAT, INSCRIPTIO_INSCRIPTA, INSCRIPTIO_CODEX_FALSUS, INSCRIPTIO_EXPIRATA, INSCRIPTIO_RETE } InscriptioGradus;` (CODEX_FALSUS still accepts codes; EXPIRATA, RETE and INSCRIPTA are final)
  - `nomen vacuum (*CommunioInscriptioMutata)(vacuum* ctx, InscriptioGradus gradus);`
  - `CommunioInscriptio* communio_inscriptionem_incipere(Piscina*, Reactor*, Veredarius*, Volumen*, constans character* basis, chorda epistula, constans character* via_tesserae, chorda titulus, CommunioInscriptioMutata, vacuum* ctx);` (mints the token file; writes praesentia `communio/inscriptio`)
  - `CommunioInscriptio* communio_inscriptionem_resumere(Piscina*, Reactor*, Veredarius*, Volumen*, constans character* basis, constans character* via_tesserae, CommunioInscriptioMutata, vacuum* ctx);` - NIHIL if nothing pending, the token file is gone, or XV minutes passed (then the praesentia entry and the token file are removed).
  - `b32 communio_inscriptioni_codicem_dare(CommunioInscriptio*, chorda codex);` (normalised: upper-case, `-` and spaces removed, VIII Crockford characters, else FALSUM with no request)
  - `InscriptioGradus communio_inscriptionis_gradus(constans CommunioInscriptio*);`
  - `vacuum communio_inscriptionem_abicere(CommunioInscriptio*);` (deletes the token file and the praesentia entry unless INSCRIPTA)
  - removed: `communio_inscriptionem_petere`, `communio_inscriptionem_confirmare`, `communio_tessera_valet`.

- [ ] **Step 1: failing tests** (in-process server with the `fictus` mail provider, as plan-3) - "per codicem": incipere -> EXSPECTAT; code from the fake mail -> INSCRIPTA, token works; "per nexum": link confirmed through the server's POST -> INSCRIPTA within III checks; "codex falsus": wrong code -> CODEX_FALSUS, then the right one -> INSCRIPTA; "codex normalis": `"abcd-efgh"`, `" ABCD EFGH "` accepted, `"ABC"` and `"ABCDEFG!"` refused without a request; "expirata": clock past XV min -> EXPIRATA, token file gone; "rete": server down -> RETE; "resumptio": incipere, drop the object, resumere from the same volume -> EXSPECTAT, link -> INSCRIPTA; praesentia never contains the address; "abicere": token file and entry removed; "nihil obstruit": no call blocks (each returns before the server answers); the `communio inscribere` CLI tests still pass on the new object.
- [ ] **Step 2:** FAIL. **Step 3:** implement. **Step 4:** PASS. **Step 5:** commit.

### Task 3: global picture ids, the Shared list and its thumbnails

**Files:**
- Modify: `include/pictor_documentum.h`, `lib/pictor_documentum.c`, `lib/pictor_actiones.c` (`spatium_novum`), `lib/pictor_documentum.worklog.md`
- Test: `probationes/probatio_pictor_communia.c` (new; two volumes joined by `communio_transferre`, as phase-1 tests do)

**Interfaces:**
- Produces:
  - `PictorPicturaAliena` gains `s64 hora;` (writer's time of the picture's latest act).
  - `Xar* pictor_aliena_recentia(Volumen*, Piscina*);` (of `PictorPicturaAliena`, order C7)
  - `b32 pictor_pinacem_alienum_facere(Volumen*, InternamentumChorda*, chorda replica, constans character* spatium, i32 latitudo, i32 altitudo, Piscina*, Imago* exitus);` (same drawing as `pictor_pinacem_facere`, one shared helper)
  - new pictures: spatium = `moneta_ulid` (C6).

- [ ] **Step 1: failing tests** - "ulid": New picture's spatium is 26 Crockford characters, two in a row differ, old `pictura-3` still opens; "ordo": B draws pictures p, q, then strokes p again; after transfer A's list = [p, q]; equal times order by replica then spatium; own pictures never appear; "pinax alienus": thumbnail pixels equal B's own `pictor_pinacem_facere` of the same picture; unknown picture -> FALSUM.
- [ ] **Step 2:** FAIL. **Step 3:** implement. **Step 4:** PASS. **Step 5:** commit.

### Task 4: Edit a copy

**Files:**
- Modify: `include/pictor_applicatio.h`, `lib/pictor_applicatio.c`, `include/pictor_documentum.h`, `lib/pictor_documentum.c`
- Test: `probationes/probatio_pictor_communia.c`

**Interfaces:**
- Consumes: T3.
- Produces:
  - `b32 pictor_exemplum_facere(PictorMontatio* m, chorda replica, constans character* spatium_alienum, character spatium_novum[XXVIII]);` - creates, fills (C8), records the origin, and shows the copy.
  - `b32 pictor_originem_legere(Volumen*, constans character* spatium, Piscina*, chorda* replica, chorda* spatium_alienum);`

- [ ] **Step 1: failing tests** - "exemplum": copy of B's picture on A -> new own picture at A's canvas size, its composite equals the foreign composite fitted (`pictor_imaginem_aptare`), it is in A's own recent list first, `pictor_originem_legere` returns (B's replica, spatium); drawing on the copy appends acts; the foreign original gained no act; "dimensiones": a foreign picture larger and smaller than A's canvas both fit; unknown foreign picture -> FALSUM, nothing created.
- [ ] **Step 2:** FAIL. **Step 3:** implement. **Step 4:** PASS. **Step 5:** commit.

### Task 5: sync inside pictor (Mac first)

**Files:**
- Create: `include/pictor_communio.h`, `lib/pictor_communio.c`, `lib/pictor_communio.worklog.md`
- Modify: `include/testa_cursus.h` (`TestaAmbitus.reactor`, `.veredarius`, `.machina`), `lib/testa.c` (macOS: create both, pump the reactor in the window loop - its sockets wake the wait, its timers bound it), `lib/testa_android.c` (both NIHIL), `apps/pictor/pictor.c` (token path C4, `-servus`, start sync, resume on `EVENTUS_RESUMPTIO`), `.gitignore`
- Test: `probationes/probatio_pictor_communio.c` (new)

**Interfaces:**
- Consumes: T1, T2, T3.
- Produces:
  - `nomen vacuum (*PictorCommunioMutata)(vacuum* ctx);` (pictures arrived, device list or signed-in state changed)
  - `PictorCommunio* pictor_communionem_aperire(Piscina*, Reactor*, Veredarius*, Volumen*, constans character* via_tesserae, constans character* basis, PictorCommunioMutata, vacuum* ctx);` - NIHIL when `veredarius` is NIHIL.
  - `b32 pictor_communio_inscripta(constans PictorCommunio*);` (a valid token file exists and the server has not refused it)
  - `vacuum pictor_communio_incipere(PictorCommunio*);` / `vacuum pictor_communio_excitare(PictorCommunio*);` (resume)
  - `chorda pictor_communio_auctor(constans PictorCommunio*, chorda replica, Piscina*);` -> `"Amanda · iPhone"` from the cached device list (praesentia `communio/apparatus`), `""` if unknown.
  - `chorda pictor_communio_persona(constans PictorCommunio*, Piscina*);` -> this device's person ("Amanda") for "Signed in as".

- [ ] **Step 1: failing tests** (in-process server, two pictor volumes, house Veredarius) - "advenit": B draws, A's mutata fires within the reactor turns and `pictor_aliena_recentia` on A lists it; "auctor": after start the device list is fetched and cached, `auctor(B) == "Amanda · iPhone"` (names from the test fixture), still answered after a restart with the server down; "sine tessera": no token file -> no request at all; "sine rete": aperire(veredarius NIHIL) -> NIHIL; "resumptio": stop the server mid long-poll, restart it, `excitare` -> new acts arrive once (import counts unchanged for old ones); "revocatus": server revokes A -> A stops after one 401, token file deleted, `inscripta` FALSUM, mutata fired, no further requests over the next minute of reactor time; "fumus": `-fumus` makes no request.
- [ ] **Step 2:** FAIL. **Step 3:** implement. **Step 4:** PASS; `apps/pictor/fumus.sh` still FUMUS PLENUS; by hand: two Mac pictor windows on two scratch volumes against a local exemplar (`-servus http://127.0.0.1:<port>`, devices registered by the admin tools) - a picture drawn in one appears in the other's Shared list within a minute with no input events (the shell's reactor pumping works while idle). **Step 5:** commit.

### Task 6: menu, Shared pictures and the viewer

**Files:**
- Modify: `include/pictor_componentia.h`, `lib/pictor_componentia.c`, `include/pictor_actiones.h`, `lib/pictor_actiones.c`, `lib/pictor_figurae.c`, `include/pictor_documentum.h` / `lib/pictor_documentum.c` (Shared gallery state beside `PictorPinacotheca`), `apps/pictor/organum.stml` (actions `communia.agere`, `spectator.agere`; transient fields `pagina_communium`, `spectata`)
- Test: `probationes/probatio_pictor_communia.c` (compositio part), `probationes/probatio_pictor_pinacotheca.c` (menu text)

**Interfaces:**
- Consumes: T3, T4, T5.
- Produces:
  - `pictor_elenchum_parare(constans PictorElenchiCondiciones* c, ElenchusRes res[V], Elenchus* e);` with `nomen structura { b32 importare; b32 communio; b32 inscripta; } PictorElenchiCondiciones;` - order and visibility per spec IX.3, copy C10.
  - palette `"communia"` (grid, same geometry and paging as the gallery) and `"spectator"`; field `spectata` = `"<replica>:<spatium>"`.

- [ ] **Step 1: failing tests** - "menu": items for each condition (Mac signed out: My pictures, Shared pictures, New picture, Sign in; iPhone signed in: + Import image, no Sign in; Android: My pictures, New picture); "communia": grid lists the foreign pictures newest first, 12 per page, ‹ › paging, empty texts (C10) when nothing / not signed in; "spectator": tap -> viewer shows that picture with "by Amanda · iPhone"; drag left past the threshold -> next, from the last -> first; drag right from the first -> last; ← → keys the same; a short drag or vertical drag changes nothing; × and a tap outside -> grid on the same page; "lectio sola": pen/eraser/line actions in the viewer append nothing to any document; "Edit a copy" -> viewer closes, the copy is the open picture (T4); "vivus": a new foreign picture arrives while viewing p -> still p, swipe order includes the new one; strokes on p arrive -> image refreshes; screenshots of grid and viewer as PNG when `PINACOTHECA_IMAGINES` is set.
- [ ] **Step 2:** FAIL. **Step 3:** implement (swipe from `EVENTUS_TRACTUS_FINIT`: |dx| ≥ a quarter of the viewer width and |dx| > |dy|). **Step 4:** PASS; existing gallery suite still green. **Step 5:** commit.

### Task 7: welcome and sign-in screens

**Files:**
- Modify: `lib/pictor_componentia.c`, `lib/pictor_actiones.c`, `lib/pictor_figurae.c`, `apps/pictor/organum.stml` (actions `salutatio.agere`, `inscriptio.agere`; transient field `inscriptio_textus`), `apps/pictor/pictor.c` (show welcome at start, resume a pending sign-in), `include/testa_cursus.h` (`TestaClaviaturamPonere claviaturam_ponere; vacuum* claviatura_ctx;` with `TESTA_CLAVIATURA_NULLA | _EPISTULA | _CODEX`)
- Test: `probationes/probatio_pictor_inscriptio.c` (new)

**Interfaces:**
- Consumes: T2, T5, T6.
- Produces: praesentia `pictor/salutatio` (`"visa"` once either welcome choice is taken); dialogs `"salutatio"` and `"inscriptio"`.

- [ ] **Step 1: failing tests** (in-process server, fake mail) - "salutatio": first start with a Veredarius -> welcome; "Draw without signing in" -> canvas, menu has "Sign in…", second start -> no welcome; no Veredarius -> never shown; "via codicis": Sign in… -> type the address (EVENTUS_TEXTUS), Send -> "Check your email" + the keyboard service asked for `_CODEX`; type the code -> "Signed in as Amanda", sync started, menu without "Sign in…", keyboard hidden; "via nexus": confirm through the link instead -> same end without typing; "codex falsus": "That code isn't right."; "expirata" and "rete" messages (C10); "codex normalis": pasted `abcd-efgh` works; RETRORSUM deletes, REDITUS submits, text longer than CCLIV bytes is cut, non-ASCII in the code field is refused with the wrong-code message; "Cancel" -> token file gone; "resumptio applicationis": start a sign-in, tear the app down, build it again on the same volume -> "Check your email" again, link -> signed in; the address never appears in the volume; screenshots (welcome, both sign-in steps) as PNG when `PINACOTHECA_IMAGINES` is set.
- [ ] **Step 2:** FAIL. **Step 3:** implement. **Step 4:** PASS. **Step 5:** commit.

### Task 8: iOS shell - URLSession and reactor timers

**Files:**
- Modify: `lib/testa_ios.m`, `lib/testa_initus.c` + header (pure frame scheduling input), `lib/testa_ios.worklog.md`
- Test: `probationes/probatio_testa_initus.c`

**Interfaces:**
- Consumes: T1 `Veredarius`, T5 `TestaAmbitus.reactor/.veredarius`.
- Produces: iOS `a->veredarius` over `NSURLSession` (delegate queue = main queue; timeout = `tempus_ms` + XV s; no caching; status + body copied into the piscina; failures -> causa `"rete: <domain> <code>"`); `a->reactor` with timers only, polled with timeout 0 on every frame and on a timer set to the reactor's next deadline; idle with no deadline = no frames.

- [ ] **Step 1: failing tests** (pure, on the Mac) - "mora reactoris": scheduling input with a reactor deadline of CCC ms and nothing else due -> POST CCC; deadline 0 -> STATIM; no deadline, idle -> no frame; the existing frame-scheduling cases unchanged.
- [ ] **Step 2:** FAIL. **Step 3:** implement. **Step 4:** PASS; `bin/fabrica probare ios-struere` green; the iOS binary contains no `br_` symbols (`nm`); Simulator run with `-servus https://exemplar.golden.earth` shows the welcome screen, and a debug-only check `GET /communio/apparatus` without a token logs status 401 (harmless). **Step 5:** commit.

### Task 9: iOS shell - the system keyboard

**Files:**
- Modify: `lib/testa_ios.m` (`TestaVisus` adopts `UIKeyInput` + `UITextInputTraits`), `lib/testa_initus.c` + header (pure text -> events), `lib/testa_ios.worklog.md`
- Test: `probationes/probatio_testa_initus.c`

**Interfaces:**
- Consumes: T7 `claviaturam_ponere` and its kinds.
- Produces: `i32 testa_textum_eventus(constans character* utf8, s64 tempus, Eventus* eventus, i32 maximus);` - "\n" -> `CLAVIS_DEPRESSUS REDITUS`, other text -> one `EVENTUS_TEXTUS`; `deleteBackward` -> `RETRORSUM`. `_EPISTULA` = email keyboard, `textContentType` email, no autocorrect/autocapitalize; `_CODEX` = ASCII keyboard, all-caps, `oneTimeCode`; `_NULLA` resigns first responder.

- [ ] **Step 1: failing tests** (pure) - "textus": `"a"` -> one TEXTUS; `"ab\n"` -> TEXTUS "ab" + REDITUS; `""` -> none; UTF-8 multi-byte passes intact; count capped at `maximus`.
- [ ] **Step 2:** FAIL. **Step 3:** implement. **Step 4:** PASS; `ios-struere` green; Simulator: Sign in… brings up the email keyboard, typing shows in the field, Send switches to the code keyboard. **Step 5:** commit.

### Task 10: live - main, TestFlight, three devices (with Fran)

- [ ] **Step 1:** final whole-branch review (fable), fixes per executing-plans; merge main in; `fusio_probare`; on Fran's word ff main; heal main.
- [ ] **Step 2:** server sanity: `communio_apparatus_enumerare` - the test devices of phases 2-3 pushed nothing a person would see; nothing reset.
- [ ] **Step 3:** Mac: `mv ~/.rhubarb/communio/macintosh.clavis <main>/pictor.volumen.tessera` (never read; mode stays 0600); Mac pictor from main syncs as "Macintosh".
- [ ] **Step 4:** on Fran's word publish from main (`tools/ios/divulgare.sh pictor`); `nundinae_apple -testatores pictor` shows Amanda in Familia (re-invite only if needed, address typed by Fran).
- [ ] **Step 5:** Fran's iPhone: the update keeps My pictures; welcome -> sign in by link -> "Signed in as Fran". Amanda's iPhone: install, sign in. Spec IX "Done" 1-4 checked on the devices; a later build installed over a signed-in app keeps the sign-in.
- [ ] **Step 6:** spec section X "phase 5 built"; tabularium + memory; delete this plan's workspace.

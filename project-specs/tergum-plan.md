# tergum (pictor syncs in the background: pictures arrive with the notification, uploads finish after close) - Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.
> **House rule (Fran):** executed INLINE, one task per turn, Fran approves each before the next.

**Goal:** when the server sends a pulsus notification it also wakes pictor on the phone for a few seconds, and pictor downloads everything new before the person taps. A picture drawn and closed at once still reaches the server.

**Architecture:**
- **Server:** one key added to the payload (`pulsus_forma`).
- **Client library:** two small additions to `communio_cliens`, a "caught up" counter and the mora in its status, plus a public default mora.
- **Build script:** `app.sh` adds a background mode to every app whose profile has push.
- **iOS shell (testa):** a remote-notification handler that opens a wake-up window, with one completion and a backstop deadline.
- **pictor:** in that window, pictor sets the mora to 0, restarts the pulls, waits until a pull comes back empty, then finishes and restores the mora.
- **Layer #1:** a device check first, on build 10.

**Tech Stack:** C89 in Latin (latina.h), `communio_cliens`, `pictor_communio`, `reactor`, `json`, `testa` iOS shell (`lib/testa_ios.m`, Objective-C, UIKit), `tools/ios/app.sh` + `communia.sh` (bash, plutil/PlistBuddy), credo v2 suites, `silva.commissio`, `tools/dimittere.sh` for exemplar.

**Spec:** `project-specs/tergum-spec.md` (APPROBATUM 2026-11-04, commit 4a6f7760): II, III.1-III.5, IV, V.

## Calls made in this plan

- **C1 Names.**
  - `CommunioStatus` gains `i32 tractiones_vacuae;` and `i32 mora;`.
  - `#define COMMUNIO_MORA_ORDINARIA XXV` in `include/communio_cliens.h` (the private `MORA_ORDINARIA` goes).
  - `testa_cursus.h` gains `TestaExcitatioExitus` (`TESTA_EXCITATIO_NOVA = 0`, `TESTA_EXCITATIO_NIHIL`, `TESTA_EXCITATIO_DEFECTUS`), `TestaExcitatioFinire`, `TestaPulsusExcitatio` and `TestaExcitationemPonere`, and a `TestaAmbitus.excitationem_ponere` field (sui = `pulsus_ctx`).
  - pictor gains `pictor_communio_excitationis_terminum_ponere`.
  - `communia.sh` gains `ios_modos_tergi_ponere`.
- **C2 Where "empty pull" is counted.** In `_importare`
  (`lib/communio_cliens.c:672-686`), the branch that sets
  `s->otiosa = VERUM`. That branch means the server returned no acts
  and did not say `plura`. A pull that returns only acts already present
  loops at once and is not counted; the next, empty pull is.
- **C3 A third client addition beyond the spec's two:** `mora` in
  `CommunioStatus`. Spec V requires tests to check the mora **value**
  after the window, and the client has no getter.
- **C4 Two deadlines.**
  - **pictor's own:** `EXCITATIO_TERMINUS_MS` = XX s, settable for
    tests. pictor always ends its own window, finishes "defectus" and
    restores the mora itself.
  - **testa's backstop:** XXV s. It only calls the system completion
    handler, and a late finish is ignored.
  - So the mora is never left at 0, even when the backstop fires.
- **C5 Several notifications close together share one window.** Up to
  `EXCITATIO_MAXIMA` (IV) pending finishes.
  - A later arrival notes the counter again and calls renovare
    again: its pictures reached the server before its push, so any
    pull started after it sees them.
  - All the pending finishes complete together, with the same result.
  - A fifth arrival finishes "nihil" at once; the open window fetches
    its pictures anyway.
- **C6 Result mapping in testa.**
  - NOVA → `UIBackgroundFetchResultNewData`.
  - NIHIL → `NoData`.
  - DEFECTUS → `Failed`.
  - The backstop → `Failed`.
  - No callback registered, or no reactor → `NoData` at once.
- **C7 testa's Objective-C code has no unit test.** It needs UIKit. It
  is checked by the ios-struere gate (it compiles and links) and on
  devices in T7. The logic that can be tested lives in pictor (T6).
- **C8 The plist decision is a shell function,
  `ios_modos_tergi_ponere PROFILUM_PLIST INFO_PLIST`.**
  - `app.sh` calls it right after writing Info.plist (line 186), for
    tabula (`$IOS_PROFILUM_PLIST`) and divulgatio
    (`$D/profilum.plist`).
  - The simulator has no profile and does not call it.
  - The gate tests the function on fixture plists, with no signing.
- **C9 The payload change only takes effect once exemplar is
  redeployed** (T7, on Fran's go, with volume backups). Phones on build
  10 ignore `content-available` because they have no handler, and
  behave as today.
- **C10 Layer #1 is checked first (T1), on build 10, before any
  code.** If it fails, the fix is diagnosed and inserted as a ruled
  task before T2.

## Global Constraints

- **House C rules:**
  - C89 with the house flags; Latin identifiers.
  - `i32`/`i64` are UNSIGNED, `s64` signed.
  - Roman numerals for constants.
  - `nomen`/`casus`/`magnitudo`/`interior` are never identifiers.
  - chorda is not NUL-terminated.
  - examen 97.
  - The Latin lint blocks new words: rename, or add a glossary entry.
- No existing signature changes (spec III.2, III.3).
  `TestaPulsumPetere`, the tap path and registration stay as they are.
- On macOS and Android the new `TestaAmbitus` fields stay NIHIL (filled
  with ZEPHYRUM). An app that registers nothing gets "no data".
- The mora after any window is the mora pictor set, otherwise
  `COMMUNIO_MORA_ORDINARIA`. **Never 0.**
- **Secrets:**
  - device tokens only as an VIII-char prefix in logs;
  - no APNs key or provider token in logs, argv or errors.
- **Fran's go is needed for:** each Apple step, TestFlight (from main
  only), ff main, and the exemplar deploy (backups first; a worktree
  deploy only under `worktree-deploys-exemplar` conditions).
- Fran approves each task.

## Review Focus

1. **A second notification arrives while a window is open** (Fran
   shares, then shares again a minute later, and the phone wakes for
   both). Expected: every system completion is called exactly once,
   and the second batch is fetched too. → T6 "duae excitationes".
2. **The network is down or the server unreachable during the
   window.** Expected: pictor finishes "defectus" at its deadline, the
   mora goes back to XXV, and the phone never stays pulling every
   second. → T6 "terminus".
3. **The notification arrives while pictor is open and already
   synced.** Expected: one quick pull, finish "nihil", nothing imported
   twice, nothing changes on screen. → T6 "nihil novi".
4. **The notification is for an edit to a picture already on the
   phone.** Expected: the edit is fetched before the tap. → T6
   "emendatio".
5. **The notification arrives before pictor has a running client**
   (not signed in yet, or the device was revoked). Expected: finish at
   once with no crash, and no request is sent. → T6 "sine cliente".

---

### Task 1: Layer #1 - uploads after close, checked on build 10 (no code)

**Files:**
- Modify: `project-specs/tergum-spec.md` (VIII "As run", started here)

**Interfaces:** none.

- [ ] **Step 1: confirm the server log can answer.**
  Run `ssh smaragda 'systemctl cat exemplar'` and confirm `exemplar`
  runs with `-acta`, which turns on the access log
  (`lib/hospitium.c:505`, lines `[hospitium] POST /communio/acta 200
  ...`).
  - Expected: `-acta` is present.
  - If it is absent: stop and ask Fran before changing the service.
- [ ] **Step 2: Fran's part, on build 10.**
  1. Open pictor and draw one stroke.
  2. Swipe to the home screen within II s.
  3. Lock the phone.
  4. Say the time.
- [ ] **Step 3: read the log.**
  `ssh smaragda 'sudo journalctl -u exemplar --since "<time - 1 min>"'`.
  - Expected: `POST /communio/acta 200` and
    `POST /communio/pulsus/eventus 200` within XXV s of the swipe.
  - Fran's other devices may add lines; only POSTs count, and only
    the phone that just drew sends them.
- [ ] **Step 4: record the result.**
  - **Pass:** spec VIII gets the lines (times, routes) and layer #1
    is closed with no code. Commit the spec via `silva.commissio`,
    and put this plan in the same commit.
  - **Fail:** stop. Use superpowers:systematic-debugging with the
    candidates in spec III.5. The fix becomes a ruled task before T2.

### Task 2: payload gains `content-available`

**Files:**
- Modify: `lib/pulsus_forma.c:452-458` (inside `aps`, after `thread-id`), `probationes/probatio_pulsus_forma.c:223` ("corpus")
- Modify: `lib/pulsus.worklog.md`

**Interfaces:**
- Produces: the payload `{"aps":{"alert":{...},"sound":"default","thread-id":<clavis>,"content-available":1},"pulsus":{...}}`.

- [ ] **Step 1: failing test.** In "corpus":
  - the exact JSON for 3 ids becomes
    `...,"thread-id":"<clavis>","content-available":1},"pulsus":...`;
  - for 400 ids, the output still has `"content-available":1` and is
    still <= 4096 octets with `"plures":true`.
- [ ] **Step 2:** `./compile_tests.sh pulsus_forma` → FAIL on the
  exact JSON.
- [ ] **Step 3:** add
  `json_objectum_ponere(systema, "content-available", json_integer_creare(piscina, I))`.
- [ ] **Step 4:** `./compile_tests.sh pulsus_forma` passes, and so do
  `pulsus` (server payloads) and `apns`.
- [ ] **Step 5:** commit via `silva.commissio`.

### Task 3: `communio_cliens` - caught-up counter, mora in status, public default

**Files:**
- Modify: `include/communio_cliens.h:48-57` (CommunioStatus), `lib/communio_cliens.c:30` (MORA_ORDINARIA → header), `lib/communio_cliens.c:672-686` (count), `lib/communio_cliens.c:1372-1391` (status)
- Test: `probationes/probatio_communio.c`, new section "tractiones vacuae (tergum)"
- Modify: `lib/communio_cliens.worklog.md`

**Interfaces:**
- Produces:
  - `#define COMMUNIO_MORA_ORDINARIA XXV`
  - `CommunioStatus.tractiones_vacuae` (i32): DESCENSUS pulls that
    succeeded with no acts and no `plura` (C2).
  - `CommunioStatus.mora` (i32): the client's current mora, in seconds.

- [ ] **Step 1: failing test** "tractiones vacuae (tergum)". Use two
  replicas against the in-process server, as "transferre" does.
  - A new client's `status.mora == COMMUNIO_MORA_ORDINARIA`.
  - After `communio_cliens_moram_ponere(c, 0)`, `status.mora == 0`.
  - With mora 0 and nothing on the server, `tractiones_vacuae` rises
    above its start within a few loops.
  - **Caught up means complete:**
    1. Replica A sends D strokes, enough that B's pull says `plura` at
       least once.
    2. B notes `v0 = tractiones_vacuae` and calls renovare.
    3. Run until `tractiones_vacuae > v0`.
    4. At that moment B's `recepta` equals the full count.

    Check the moment in a loop that tests the counter after every
    reactor pass, not after idling.
- [ ] **Step 2:** `./compile_tests.sh communio` → FAIL (no member
  `tractiones_vacuae`).
- [ ] **Step 3:** implement C1 and C2. The status copies
  `cliens->mora`.
- [ ] **Step 4:** `./compile_tests.sh communio` passes, and so do
  `pictor_communio`, `pictor_inscriptio` and `exemplar` (a server
  suite that uses the client).
- [ ] **Step 5:** commit.

### Task 4: `app.sh` - background mode for apps with push

**Files:**
- Modify: `tools/ios/communia.sh` (new function), `tools/ios/app.sh:186` (call it for tabula and divulgatio), `tools/ios_struere_porta.sh` (test)

**Interfaces:**
- Produces: `ios_modos_tergi_ponere PROFILUM_PLIST INFO_PLIST`.
  - If the profile's `Entitlements.aps-environment` exists, it adds
    `UIBackgroundModes` (array) holding `remote-notification` to
    INFO_PLIST.
  - Exit 0 whether or not it added the key; exit 1 if PlistBuddy
    fails.

- [ ] **Step 1: failing test** in `tools/ios_struere_porta.sh`,
  before the build. Use two fixture profile plists written to a temp
  directory, one with `Entitlements.aps-environment = production` and
  one without, and a copy of the template-rendered Info.plist.
  - With aps: `plutil -extract UIBackgroundModes.0 raw` prints
    `remote-notification`.
  - Without aps: no `UIBackgroundModes` key.
  - Run twice on the same plist: still one entry (idempotent).
- [ ] **Step 2:** `./tools/ios_struere_porta.sh` → FRACTA (function
  missing).
- [ ] **Step 3:** implement the function and call it from `app.sh`
  for both signing scopes (C8).
- [ ] **Step 4:** the gate is viridis.
  `./tools/ios/app.sh apps/pictor/pictor.c -tabula -servare`, or the
  divulgatio build without sending:
  `plutil -p build/ios/.../Info.plist | grep -A2 UIBackgroundModes`
  shows `remote-notification`.
- [ ] **Step 5:** commit.

### Task 5: testa - notification wake-up window (iOS shell)

**Files:**
- Modify: `include/testa_cursus.h` (types + field, by `pulsum_petere`), `lib/testa_ios.m` (TestaDelegatus method, window, backstop; registration in the ambitus setup beside `_pulsum_petere`)

**Interfaces:**
- Consumes: T4 (the background mode is needed on the device, not to
  compile).
- Produces (`include/testa_cursus.h`):
  - `nomen enumeratio { TESTA_EXCITATIO_NOVA = 0, TESTA_EXCITATIO_NIHIL, TESTA_EXCITATIO_DEFECTUS } TestaExcitatioExitus;`
  - `nomen vacuum (*TestaExcitatioFinire)(vacuum* sui, TestaExcitatioExitus exitus);` - the first call per window wins; later calls do nothing.
  - `nomen vacuum (*TestaPulsusExcitatio)(vacuum* ctx, chorda corpus_json, TestaExcitatioFinire finire, vacuum* finire_sui);` - corpus is `userInfo` as JSON and is valid only inside the call.
  - `nomen vacuum (*TestaExcitationemPonere)(vacuum* sui, TestaPulsusExcitatio excitatio, vacuum* ctx);`
  - `TestaAmbitus.excitationem_ponere` (sui = `pulsus_ctx`); NIHIL on macOS and Android.

- [ ] **Step 1: the failing check is the build.** Add a use of
  `a->excitationem_ponere` in pictor behind a NIHIL check, the way T6
  will call it: `excitationem_ponere(a->pulsus_ctx, fn, pc)`. Then
  `./compile_tests.sh pictor_communio` → FAIL (no member).
- [ ] **Step 2: types and field.** Add them to `testa_cursus.h`. The
  macOS testa and the Android testa already fill the ambitus with
  ZEPHYRUM; confirm that by reading where each sets up its ambitus.
- [ ] **Step 3: iOS.**
  - `application:didReceiveRemoteNotification:fetchCompletionHandler:`
    serialises `userInfo` the way the tap path does
    (`lib/testa_ios.m:1544-1560`).
  - It opens a window: it keeps the completion handler, polls the
    reactor at `GRATIA_PULSUS_S` (sharing the grace poller if one is
    running), and arms the XXV s backstop.
  - It calls the registered `excitatio`. The finish function maps per
    C6 and calls the completion handler once. The backstop calls it
    with Failed.
  - With no callback or no reactor, it answers `NoData` at once.
  - Several windows may be open at once (C5 lives in pictor). testa
    keeps one handler per window, in a small fixed array of IV.
    Overflow answers `NoData` at once.
- [ ] **Step 4:** `./compile_tests.sh pictor_communio` and
  `./tools/ios_struere_porta.sh` both pass. Run the macOS pictor build
  as well: `./compile_tests.sh pictor` passes.
- [ ] **Step 5:** commit. The worklog `lib/testa_ios.worklog.md` notes
  that this has no unit test (C7) and is checked on devices in T7.

### Task 6: pictor - fetch in the window

**Files:**
- Modify: `lib/pictor_communio.c` (registration in `pictor_communio_pulsum_parare`, window state, deadline), `include/pictor_communio.h` (terminus setter)
- Test: `probationes/probatio_pictor_communio.c`, new section "excitatio (tergum)"
- Modify: `lib/pictor_communio.worklog.md`

**Interfaces:**
- Consumes: T3 (`tractiones_vacuae`, `recepta`, `mora`, `COMMUNIO_MORA_ORDINARIA`), T5 (types, `excitationem_ponere`), `pulsus_tactum_legere`, `pulsus_corpus_facere`.
- Produces:
  - `vacuum pictor_communio_excitationis_terminum_ponere(PictorCommunio* pc, i32 ms);` (default `EXCITATIO_TERMINUS_MS` = XX s; tests use a few hundred ms).

- [ ] **Step 1: failing tests** "excitatio (tergum)".
  - **The fake ambitus.** Extend `ambitum_fictum`: a fake
    `excitationem_ponere` captures `(fn, ctx)`, and a fake `finire`
    counts calls and keeps the last result.
  - **The payload.** Each case delivers
    `pulsus_corpus_facere(... clavis "pictor" ...)` to the captured
    `fn`.
  - **The cases.** Each expects `finire` called exactly once, and
    afterwards `communio_cliens_status(...).mora ==
    COMMUNIO_MORA_ORDINARIA`:
    - "nova": B draws a new picture and A receives the push → finish
      NOVA, and A has the picture (`pictor_communia_index`) before any
      tap.
    - "emendatio" (Review Focus 4): the picture is already on A, B adds
      strokes → NOVA, and A's copy has the new strokes.
    - "nihil novi" (Review Focus 3): everything already synced → NIHIL
      after one pull, and `recepta` is unchanged.
    - "duae excitationes" (Review Focus 1):
      1. Deliver push 1 for picture P1.
      2. Before the window ends, B draws P2 and push 2 is delivered.
      3. Both finishes are called exactly once, and both pictures are
         present.
    - "terminus" (Review Focus 2): stop the server (or use a
      veredarius that never answers), set the terminus to CCC ms →
      DEFECTUS; the mora is back to XXV, not 0.
    - "sine cliente" (Review Focus 5): pulsum_parare has run and the
      client is then refused → DEFECTUS at once, with no request.
      Bad JSON or a wrong clavis → NIHIL at once.
    - "mora posita": with `pictor_communio_moram_ponere(pc, 0)` set
      before, the mora after the window is 0 again. This is the
      `pc->mora_posita` branch of spec III.3.
- [ ] **Step 2:** `./compile_tests.sh pictor_communio` → FAIL.
- [ ] **Step 3: implement spec III.4 with C4 and C5.**
  - Register in `pictor_communio_pulsum_parare` when
    `a->excitationem_ponere` is set.
  - Window state on `PictorCommunio`: the noted
    `tractiones_vacuae` and `recepta`, up to IV pending
    `(finire, finire_sui)`, and the deadline timer.
  - Watch with a reactor timer (QUAESTIO-sized, ~C ms) that reads
    `communio_cliens_status`.
  - Restore the mora on every exit: `pc->mora` if `pc->mora_posita`,
    else `COMMUNIO_MORA_ORDINARIA`.
- [ ] **Step 4:** `./compile_tests.sh pictor_communio` passes, and so
  do `pictor`, `pictor_inscriptio`, `communio` and
  `./tools/ios_struere_porta.sh`.
- [ ] **Step 5:** commit.

### Task 7: rollout and device checks

**Files:**
- Modify: `project-specs/tergum-spec.md` (VIII "As run"), `.superpowers/sdd/tergum-plan/progress.md`

**Interfaces:** none.

- [ ] **Step 1: merge main into quarta** using the merge procedure
  (rebuild tools, regenerate and cmp the test index, run the suites),
  then `fusio_probare` green.
- [ ] **Step 2 ★ ff main** (Fran's word; no worktree mid-merge, main
  clean).
- [ ] **Step 3 ★ deploy exemplar from main** (Fran's go).
  1. Stop the service, copy `exemplar.volumen` and `communio.volumen`
     to `*.ante-tergum`, start it.
  2. Run `tools/dimittere.sh -siccum exemplar smaragda`, then
     `-agere`.
  3. Salus must be green.
  4. Check `pulsus_probare amanda`: 200, and the banner shows as
     before.
- [ ] **Step 4 ★ TestFlight build 11** (Fran's go). Run
  `./tools/ios/divulgare.sh pictor -siccum` first. That is now a true
  dry run (a464c061). Then run it with `-notae`.
- [ ] **Step 5: device checks** (spec V).
  - **(a) Prefetch.** This is the finish line:
    1. Amanda's phone is locked, with pictor in the background and
       not force-quit.
    2. Fran draws two pictures and closes pictor.
    3. When the push goes out, the access log shows a
       `GET /communio/acta` burst **before** Amanda touches the
       phone.
    4. Her tap opens the pictures with no "Loading".
  - **(b)** Layer #1 again, on build 11.
  - **(c) Foreground.** With pictor open, a push changes nothing
    visible and imports nothing twice.
- [ ] **Step 6:** fill in spec VIII and answer the AUDIENDA items the
  checks settled. Commit, then run `task-done` for T7.

# tergum - pictor syncs in the background: pictures arrive with the notification, uploads finish after close (spec)

**STATUS: APPROBATUM 2026-11-04** (quarta; talked through with Fran
the same afternoon, while pulsus T11 waited on a main merge; Fran: "that
looks good to me". A builder check afterwards led to the refinements
in II "My calls": the TestFlight plist path, "done" = caught up so
edits are fetched, the two client additions, and restoring the mora).

*Fran 2026-11-04, after the first real batch on Amanda's phone: "when i
looked at her phone some of the drawings on my phone were not showing
up in her shared gallery, was that to be exxpected?" - then "would one
of the next things for us to work on maybe be something like
background sync? what is involved in that?" On the three layers I laid
out (prefetch on the notification, check that uploads finish after
close, true background sync later): "that makes sense to me, lets take
that approach".*

Goal: **the notification and the pictures arrive together.** Today
pictor only syncs while it is open on both ends. A notification tells
Amanda that Fran shared pictures, but the pictures are still on the
server until she opens pictor, and the tap shows "Loading new
pictures..." while they download. When the server sends the
notification it should also wake pictor for a few seconds so it can
download the pictures. On the other side, a picture drawn and then
closed straight away should still reach the server.

**Finish line.** Amanda's phone is locked and pictor is in the
background (not force-quit). Fran draws two pictures and closes pictor
within two seconds of the last stroke. A few minutes later Amanda's
phone shows the notification. The server log shows her device fetching
those pictures **before** she touches the phone. When she taps, the
two pictures are already there, with no "Loading".

Ledger: desideratum 01M6KRHKCN83R5F5GWZ3MW5XAX.

## I. What exists (main e0821a6d, 2026-11-04)

**Uploads after close.** These are probably already handled, but nobody has checked.
- `sceneDidEnterBackground` calls `_gratiam_incipere`
  (`lib/testa_ios.m:1633`). It asks iOS for background time
  (`beginBackgroundTaskWithName:@"pulsus"`) and polls the reactor every
  `GRATIA_PULSUS_S` (0.1 s) for `GRATIA_S` (XXV s,
  `lib/testa_ios.m:1450`). Uploads and pulsus events in flight
  should finish inside that window.

**The notification.**
- `pulsus_forma.c:440-478` builds `{"aps":{"alert":{title,body},
  "sound":"default","thread-id":clavis},"pulsus":{clavis,res[],a,
  plures?}}`.
- `apns_mittens.c:444-447` sends it with `apns-push-type: alert` and
  `apns-priority: 10`.
- There is no `content-available` key, so iOS never wakes pictor for a push.

**The phone side.**
- `TestaPulsusDelegatus` handles `willPresentNotification`, which
  returns None, and the tap (`didReceiveNotificationResponse` → app
  callback `TestaPulsusTactus`).
- `TestaDelegatus` (`lib/testa_ios.m:1715`) has no
  `application:didReceiveRemoteNotification:fetchCompletionHandler:`.
- `tools/ios/Info.forma.plist` has no `UIBackgroundModes`.

**The tap.** `pictor_communio.c:420-480` reads the `pulsus` object
(`pulsus_tactum_legere`). It opens the batch if those pictures are
already present. If not, it shows "Loading" and keeps retrying for up
to `TACTUS_MORA_MS` (15 s).

**The sync client.** `communio_cliens` (`include/communio_cliens.h`)
has two independent paths, plus the pulsus path:
- ASCENSUS sends local work.
- DESCENSUS pulls with a **long-poll** (`mora`, XXV s by default).

`communio_cliens_otiosus` (`lib/communio_cliens.c:1394`) is true only
when all three paths are idle **and none has a request in flight**.
With a 25 s long-poll a request is almost always in flight, so on a
real phone `otiosus` is practically never true. The tests reach it
only because they set mora 0 (`probatio_communio.c:935`, 970, 1009,
and others).
- `communio_cliens_moram_ponere` can change the mora while the client
  runs.
- `communio_cliens_renovare` (`lib/communio_cliens.c:1287`) is
  sistere followed by incipere. It aborts and restarts **all three**
  paths, uploads and pulsus events included. The cursors make this
  safe: nothing is lost or sent twice. pictor calls it on resume
  (`pictor_communio.c:792`).

**No "caught up" signal.**
- `CommunioAdvenerunt` fires only when something was imported.
- `CommunioStatus` (`include/communio_cliens.h:48`) has no field that
  says a pull came back empty.
- The default mora, `MORA_ORDINARIA` (XXV), is private to
  `lib/communio_cliens.c:30`.
- pictor holds the mora only when a test has set it (`pc->mora_posita`,
  `pictor_communio.c:766`). In production `pc->mora` is 0.

**Where the background-mode key would go.** `app.sh` writes
Info.plist from the template at line 186, before either signing
branch:
- The device build (tabula) reads `aps-environment` from the profile
  (line 274).
- The TestFlight build (divulgatio) copies the profile's whole
  Entitlements dict (line 255) and never reads the key on its own.

## II. Decisions (Fran 2026-11-04)

1. **Three layers, in order:**
   - **#2:** download the pictures when the notification arrives.
   - **#1:** check that uploads finish after close. This is
     probably a test, not new code.
   - **#3:** true background sync without a notification (silent
     pushes to every device, `BGAppRefreshTask`). #3 is **later** and
     is not in this spec.
2. Finish pulsus T11 (TestFlight build 10) first. **Done 11-04.**
3. The order of this work relative to pulsus T12-T14 (fasciculum
   pictor.app, Mac push, final review) is still Fran's call.

**My calls (Claude), for Fran to check:**
- **One push, not two.** The existing alert push gains
  `"content-available":1`. Apple accepts that combination on an alert
  push with priority 10. A separate silent push would be throttled
  harder and would double what the server sends.
- **The background-mode key is added only to apps that can receive
  pushes.** The rule is: if the signing profile's Entitlements contain
  `aps-environment`, Info.plist gets
  `UIBackgroundModes = [remote-notification]`.
  - The check happens once, where Info.plist is written, so the
    device build and the TestFlight build both get the key. Apps
    without push stay unchanged.
  - The template is shared, so this goes through `app.sh` rather than
    `Info.forma.plist`.
- **"Done" means caught up: a pull came back empty.** It does not mean
  the ids in the push are present, for two reasons:
  - The sender pushes for **edits** too (`_missa` →
    `pictor_spatia_actorum`), so an id Amanda already has may carry
    new strokes, and the push has no version number.
  - A `plures` push names no ids at all.

  Waiting for one empty pull covers new pictures, edits and `plures`
  with one rule. During the window the mora drops to 0, so pulls come
  straight back.
- **The client gains two small things, both additions.**
  - A `tractiones_vacuae` counter in `CommunioStatus`: the number of
    pulls that imported nothing and said nothing more is waiting.
  - A public `COMMUNIO_MORA_ORDINARIA` (XXV) in place of the private
    `MORA_ORDINARIA`.

  No existing signature changes.
- **We only handle the case where pictor is suspended.**
  - When iOS has suspended pictor, its reactor and sync client still
    exist, and the window simply runs them.
  - When the system has ended pictor and relaunches it in the
    background, the handler answers "no data" at once. The tap then
    works as it does today. Whether testa can start that way is
    unverified (AUDIENDA).
  - When the user force-quits, iOS delivers nothing, by design.

## III. Design

### III.1 Server: one key in the payload

`pulsus_forma.c` adds `"content-available":1` to the `aps` object.
Everything else stays the same:
- headers (`alert`, priority 10);
- the alert text;
- `thread-id`;
- the `pulsus` object.

The Mac (pulsus T13) ignores the key.

### III.2 iOS shell (testa)

- **Background mode.** Where it writes Info.plist (line 186),
  `app.sh` adds `UIBackgroundModes` (`remote-notification`) when the
  chosen profile's Entitlements contain `aps-environment`. That check
  covers both the tabula and the divulgatio branch. The simulator
  build has no profile and gets no key.
- **Delegate method.** `TestaDelegatus` implements
  `application:didReceiveRemoteNotification:fetchCompletionHandler:`.
  It serialises `userInfo` to JSON, the same way the tap path does.
  - If the app has registered an arrival callback, testa starts a
    **window**.
  - If not (no callback yet, or no reactor), it calls the
    completion handler at once with `UIBackgroundFetchResultNoData`.
- **The window.**
  - testa asks iOS for background time.
  - It polls the reactor at `GRATIA_PULSUS_S`, as `_gratiam_incipere`
    does. One poller serves both: if a grace window is already
    running, the arrival window shares it.
  - It hands the JSON to the app's callback.
  - It arms a **deadline** of XXV s. iOS allows about 30 s, and the
    deadline must fire before iOS gives up.
- **Completion, exactly once.** The app finishes through a function
  testa gives it: done with "new" (pictures imported), done with
  "none", or failed. Whichever comes first, the app's finish or the
  deadline, calls the system completion handler. Later calls do
  nothing.
- **API.** One new callback type and one new finish function on
  `TestaAmbitus`, set beside `pulsum_petere`. The builder chooses the
  exact shape (III.4 says what pictor needs).
  - **Fixed:** the existing `TestaPulsumPetere` signature does not
    change; pictor's tap and registration code stays untouched.
  - **Fixed:** on macOS and Android the new fields are NIHIL.
  - **Fixed:** an app that sets nothing gets "no data" every time.

### III.3 communio client: two additions

- **`tractiones_vacuae`** (i32) in `CommunioStatus`. It counts the
  DESCENSUS pulls that succeeded, imported nothing, and did not say
  `plura`. It goes up once per such pull, whatever the mora.
- **`COMMUNIO_MORA_ORDINARIA`** (XXV) in `communio_cliens.h`. It
  replaces the private `MORA_ORDINARIA`, and the client keeps using it
  as its default.

The window calls:
1. `communio_cliens_moram_ponere(c, 0)`.
2. `communio_cliens_renovare(c)`. This aborts the stale long-poll
   and starts a pull at once. As I. notes, it also restarts uploads
   and pulsus events, which the cursors make safe.
3. When the window ends, whichever way it ends, the mora goes back to
   what pictor set (`pc->mora` when `pc->mora_posita`), otherwise to
   `COMMUNIO_MORA_ORDINARIA`. **Never** `pc->mora` alone: in
   production that is 0, and it would leave the phone pulling every
   second for good.

If the builder finds that the mora can't be changed mid-pull without
losing or doubling a batch, that is a bug in the client: fix it there,
with a test in `probatio_communio.c`.

### III.4 pictor

`pictor_communio_pulsum_parare` also registers the arrival callback.
On arrival:
1. Read the `pulsus` object with `pulsus_tactum_legere`, the same
   reader the tap uses. A wrong key or bad JSON means finish "none".
2. If the client is refused (401) or not running, finish "failed".
3. Note `recepta` and `tractiones_vacuae` from the client status.
   Drop the mora to 0 and call renovare.
4. Watch the status. A short reactor timer or a callback both work;
   the builder chooses. When `tractiones_vacuae` passes the noted
   value, the phone is caught up:
   - finish "new" if `recepta` went up;
   - otherwise finish "none".
5. If the client is refused during the window, finish "failed".
6. Restore the mora (III.3) in all cases, the deadline included.

This always does at least one pull, even when pictor was open and
already synced. That pull comes straight back, so the cost is one
request per notification.

Nothing is drawn and no document opens during the window. When Amanda
taps, `_pulsus_tactus` finds the pictures present and opens the batch
at once.

### III.5 Uploads after close (layer #1)

This is a test first. On a device:
1. Draw.
2. Swipe to the home screen within two seconds of the last stroke.
3. Lock the phone.

The server log must show the acta POST, and the pulsus event, within
XXV s of the swipe.

- **If it passes:** record it in VIII, with no code change.
- **If it fails:** find out why before writing code. Candidates:
  - URLSession completions may not reach the reactor while pictor is
    in the background.
  - The window may end too early.
  - ASCENSUS may wait for its one-second tick.

A possible later improvement, ending the window early once uploads
are idle, is left to the builder (VI) and is not required.

## IV. Apple-side steps

**None.**
- Background modes are not an App ID capability.
- Push is already on (pulsus T11).
- The profiles do not change.

TestFlight build 11 is published on Fran's go, as always.

## V. Tests and oracles

**Automated:**
- **Payload.** `probatio_pulsus`: the built payload's `aps` contains
  `content-available` = 1, and its other keys are unchanged.
- **Plist.** `app.sh`: the generated Info.plist has
  `UIBackgroundModes` with `remote-notification` exactly when the
  profile's Entitlements have `aps-environment`. The check covers the
  device (tabula) path **and** the TestFlight (divulgatio) path. If
  the decision is a small function or step that can run without
  signing, test it directly. Otherwise check the built plist in
  whichever existing check already inspects it.
- **pictor arrival.** Use a fake `TestaAmbitus` against the
  in-process communio server, as the pictor/communio tests already
  do. Each case expects the finish to be called **once**:
  - a new picture → imported → finish "new";
  - **an edit** to a picture already present → the edit is imported
    → finish "new";
  - nothing new → one empty pull → finish "none";
  - client refused → finish "failed";
  - in every case the mora **value** afterwards is
    `COMMUNIO_MORA_ORDINARIA`, or what pictor set. It is never 0.
- **communio.** `tractiones_vacuae` goes up on an empty pull, and not
  on a pull that imports something or says `plura`. If III.3 turns up
  a bug in changing the mora mid-pull, a test for that too.
- **Build.** The ios-struere gate compiles the new delegate method.
- **Deadline.** testa's deadline logic gets a test if it can be
  pulled into a pure function. If not, it is checked on the device.

**On devices (Fran's go, as in pulsus T11):**
- **(a) Prefetch.** This is the finish line above. The oracle is the
  server log, which must show Amanda's device pulling after the push
  and before the tap.
- **(b) Uploads after close.** The procedure in III.5.
- **(c) Foreground.** With pictor open, a push changes nothing
  visible and causes no double import.

## VI. Not in this spec

- Layer #3: background sync without a notification. Later, as a
  design question.
- The Mac in the background (a helper process; pulsus T13 territory).
- Doing real work after the system has ended pictor (III.2 answers
  "no data").
- Ending the grace window early when uploads are idle.

## VII. Left to the builder

- The exact `TestaAmbitus` field names and the finish function's
  shape.
- Whether the arrival window and the grace window share one timer or
  use two.
- How pictor watches `tractiones_vacuae`: a reactor timer or a
  callback.

## VIII. As run

- **Layer #1, 2026-11-05 01:59 UTC (Fran's phone, TestFlight 10).**
  Exemplar access log (`-acta` is on):
  - 01:59:24 cold open: `GET /communio/acta`, `GET`/`POST
    /communio/apparatus`, `PUT /communio/pulsus/inscriptio 204`.
  - 01:59:37 `POST /communio/acta 200` and `POST
    /communio/pulsus/eventus 202`, in the same second. 202 is the
    route's normal answer (`lib/pulsus.c:1094`); the plan's "200" was
    a typo.
  - 01:59:49 `GET /communio/acta 200`, about 12 s after the upload,
    while the phone was on the home screen or locked.
  - **Passed, with no code change.** One caveat: ASCENSUS ticks every
    1 s, so a stroke drawn 2 s before the swipe may upload while
    pictor is still in front. The test shows the edit is not lost
    and the reactor keeps running after close (the 01:59:49 pull).
    It does not prove an upload that *started* in the background.
    The stricter check is airplane mode on, draw, swipe home, then
    airplane mode off and lock; the upload can then only happen in
    the background.

## AUDIENDA (not verified while writing)

- **Alert plus content-available.** I believe iOS wakes the app for
  an alert push carrying `content-available` and still shows the
  banner, and that throttling is milder than for pure background
  pushes. This comes from memory, not from a test.
- **Relaunch after the system ends pictor.** Does a background launch
  for a remote notification connect a scene? Does testa then have its
  reactor and sync client? The design assumes no and answers "no
  data".
- **URLSession in the background.** Do URLSession completions on iOS
  (the veredarius) make progress while pictor is in the background and
  testa polls the reactor? Device test (b) answers this for uploads,
  and (a) for downloads.
- **Delivery conditions.** Low Power Mode, poor signal, and a phone
  that has been idle for a long time may delay or drop the wake.
  Prefetch is best effort. The tap path stays as the fallback.

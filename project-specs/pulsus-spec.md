# pulsus - notifications for our apps: device tokens, quiet-period batches, APNs from the server (spec)

**STATUS: PROPOSITUM 2026-11-02** (quarta; brainstorm with Fran the
same afternoon, right after http2 phase 1 reached main as 424b4a6b).

*Fran 2026-11-02, on what should happen: "i think we'll want a manual
notification trigger or maybe some kind of debounced but like with a
5-10 timeout or something like that (not sure how tractable that is if
the app / phone is asleep or something like that, i guess we could have
some kind of dead man's switch type thing on the server? but however we
build should be a generic module/capability that pictor can use more so
than baked into pictor if that makes sense". Then: "yes, 5-10 minutes
of inactivity. for now it would go to everyone (if they have
notifications turned on? ...) and we'll figure out more granular stuff
later"; "yes, timer is per sender"; on the tap: "either gallery or we
might have a transient group of 'updated pictures' that the
notification kind of deep links to that you can swipe through?"; on
the Mac: "i think we would want to set up macos notifications too" -
"yeah, we can probably make into an app, we already have pretty much
everything for the briar -app option".*

Goal: **a notifications capability of the house**, living on the
communio server, that any of our apps can use: a device registers its
push address once; an app reports "something happened" under a key;
the server waits for a quiet period per sender and key, then sends ONE
notification to the rest of the family through Apple's push service
(APNs), over our own HTTP/2 client. pictor is its first user, not its
home.

**Finish line, phase 1:** Fran draws or edits three pictures on his
iPhone and stops. About five minutes later Amanda's iPhone (TestFlight
build, locked) shows "Fran shared 3 pictures". Tapping it opens pictor
on the first of those three; she swipes through just those three; ×
returns her to the Shared grid.

**Finish line, phase 2:** Amanda shares pictures from her iPhone and
"Amanda shared N pictures" arrives on Fran's Mac, where pictor now runs
as a signed `pictor.app`; clicking it opens the same viewer. (Fran's
own pictures never notify his Mac - D5. Fran alone can check delivery
first with `pulsus-probare`, III.1.)


## I. What exists (main 424b4a6b, 2026-11-02)

- **Our HTTP/2 client and APNs sender (Mac only so far).** `hpack`,
  `http2_forma`, `http2` (sans-IO), `http2_nexus` (reactor glue) and
  `lib/apns.c` / `include/apns.h`, plus the `apns` CLI
  (`tools/apns.c`): ES256 provider token, `POST /3/device/<token>`,
  verified live against `api.sandbox.push.apple.com` (http2-spec,
  FINIS LINE). The stack sits on `reactor.c` (poll(), portable) and
  `tls.h`, whose house default is our own TLS 1.3 (`lib/tls13.c`, ALPN
  in `lib/tls13.c` / `lib/tls13_nuntii.c`); none of it has been
  compiled on Linux yet.
- **The communio server** (`include/communio_servus.h`,
  `lib/communio_servus.c`, mounted by exemplar on the VPS "smaragda"):
  spaces, members (`communio_membra`), devices (`communio_apparatus`,
  Bearer token -> device -> person), acts, blobs, long-poll with timers
  on the server's reactor. It already runs an outbound service client
  on that reactor: the email sender (`epistularium_aperire`), configured
  from `<data>/resend.clavis` and simply off ("epistulae non
  mittuntur") when the file is missing
  (`apps/exemplar/exemplar_nucleus.c:1215-1243`). The push sender
  follows the same pattern.
- **pictor on iPhone and Mac** syncs through communio (communio-spec
  IX/X): My pictures, Shared pictures (grid of other devices'
  pictures, newest first), a full-screen read-only viewer with swipe /
  ← → that loops through the grid's order
  (`lib/pictor_actiones.c:1822`, `pictor_communia_agere`). Sync runs
  only while the app is open. On iOS, networking is a shell host
  service over URLSession (no crypto in the app, export answer
  "exempt"); the shell is `lib/testa_ios.m` (UIApplicationDelegate at
  :1466). The Mac window shell is `lib/fenestra_macos.m`.
- **Mac pictor is a bare program today** (`bin/pictor`, ad-hoc
  signed, identifier "pictor", no bundle). `lib/fasciculum.c` (briar
  `-app`, FASCICULUS sealed 2026-09-14) already builds real `.app`
  bundles: Info.plist, identity, icon via iconutil, replace-own-bundle
  rule.
- **Apple side.** App ID `earth.golden.pictor` (team Q5WC9FQ3J2),
  PUSH_NOTIFICATIONS **off** (`apns -praeparare` reports ABSENS).
  APNs key `LA8Z282UYC` (`~/.rhubarb/ios/AuthKey_LA8Z282UYC.p8`,
  SECRET) works against the sandbox. Development builds are signed
  with the wildcard profile "iOS Team Provisioning Profile: *";
  TestFlight builds with the distribution profile via
  `tools/ios/divulgare.sh` (main only).


## II. Decisions (Fran 2026-11-02)

- **D1. Generic capability, server-side.** The notification logic
  lives on the communio server as a module any app uses; pictor only
  registers, reports, and handles taps.
- **D2. Quiet period, on the server.** A per-sender timer restarts
  with every new event; when 5-10 minutes pass with nothing new, one
  notification goes out. The server does the waiting ("dead man's
  switch"), so neither phone needs to be awake. pictor uses **5
  minutes**.
- **D3. Manual trigger.** "Send now" skips the wait.
- **D4. Timer is per sender** (and per app key - see III.1).
- **D5. Recipients: everyone in the family except the sender**, on
  every device where notifications are allowed. Finer choice
  (per-picture recipients) comes later. Turning notifications on/off
  is the operating system's job for now (iOS Settings); no in-app
  setting.
- **D6. Tap opens a transient group:** the pictures from that
  notification, in the existing viewer, swipe through them, × back to
  the Shared grid. Nothing new to manage afterwards.
- **D7. The Mac gets real push too**, as a second phase, by making
  Mac pictor a real signed app (building on fasciculum), not by
  in-app polling.
- **D8. Phones first, Mac second.**

**My calls (Claude, stated during the brainstorm, Fran agreed "everything
else looks good"):** "except the sender" means the PERSON (Fran's own
Mac and phone are never notified about Fran's pictures); text like
"Fran shared 3 pictures"; 5 minutes default with each app choosing its
own; two people sharing at once get separate notifications; a token
Apple rejects is dropped; a picture deleted before the tap is skipped,
and if none remain the Shared grid opens.


## III. Design

### III.1 The server capability (`pulsus`)

A new library on the server side, `lib/pulsus.c` / `include/pulsus.h`
(name: my call - "pulsus", a knock; `nuntium` is taken by our protobuf
and `admonitiones` is vicus's in-app list), created alongside the
communio server and sharing its volume, reactor and clock injection
(`nunc_ms`, as `communio_servum_creare` takes).

**Tables (in the server volume):**

- `pulsus_inscriptiones`: apparatus (device id), tessera (APNs device
  token, hex), ambitus (`productio` | `sandbox`), thema (bundle id,
  e.g. `earth.golden.pictor`), creatus_ms, usus_ultimus_ms. One row per
  device and thema; re-registering replaces. A revoked device's rows
  are never used (and are deleted on revocation).
- `pulsus_pendentes`: one row per open batch - civis (sender), spatium,
  clavis (app key, e.g. `pictor.picturae`), thema, res (ordered, distinct
  item ids), textus templates, primus_ms, ultimus_ms, mora_ms. Persisted
  so that a deploy or restart in the middle of a quiet period does not
  lose the notification: on start-up every row's timer is re-armed with
  its remaining time (fires at once if already due).

**Batching.** Key = (sender person, space, clavis). An event adds its
item ids (distinct, in order of first appearance) and restarts the
timer at `mora`. When the timer fires, the batch is sent and its row
deleted. **No maximum wait** (my call): someone drawing for an hour
keeps restarting the timer, and the family hears about it five minutes
after they stop, not mid-drawing. "Send now" fires the batch
immediately (adding its items first).

**Text.** The app supplies two templates, one for one item and one for
several, with `{persona}` (the sender's display name from `cives`) and
`{numerus}` placeholders; pictor's: `"{persona} shared a picture"` /
`"{persona} shared {numerus} pictures"`. The notification title is the
app's name ("pictor"). Templates are plain text, bounded (CCLVI octets
each), no control characters.

**Routes** (under the existing `/communio/` prefix, Bearer token as
everywhere there; 401 otherwise):

```
PUT    /communio/pulsus/inscriptio {"tessera":hex, "ambitus":"productio"|"sandbox",
                                    "thema":"earth.golden.pictor"} -> 204
DELETE /communio/pulsus/inscriptio?thema=...                       -> 204
POST   /communio/pulsus/eventus {"clavis":"pictor.picturae", "thema":...,
                                 "res":["<id>",...], "mora":300,
                                 "unus":"...", "plures":"...",
                                 "statim":false}                   -> 202
```

Validation: tessera = even-length hex, bounded; ambitus one of two
words; thema/clavis bounded identifiers; res at most C per request and
M per batch (excess ignored, counted); mora 0..3600 s (0 = statim).
Bad input -> 400, nothing stored. Item ids are opaque to the server.

**Delivery.** For each active member of the space other than the
sender person, for each non-revoked device of theirs with an
inscription for that thema: one APNs request with

```
{"aps":{"alert":{"title":"pictor","body":"Fran shared 3 pictures"},
        "sound":"default","thread-id":"<clavis>"},
 "pulsus":{"clavis":"pictor.picturae","res":["<id>",...],"a":"<civis>"}}
```

(sound: my call - Fran did not choose; "default" is the usual tone.)
Item ids are included only while the payload stays under APNs' 4096
octets; beyond that `res` is truncated and `"plures":true` set (the
app then opens the Shared grid). Headers: `apns-push-type: alert`,
`apns-topic: <thema>`, `apns-priority: 10`, `apns-expiration`: a day.

Outcome handling: 200 -> `usus_ultimus_ms`; 410 `Unregistered` or 400
`BadDeviceToken` -> delete that inscription; 403 provider-token errors
-> refresh the token once, then log and stop; 429 / 5xx / connection
loss -> retry with backoff (a few tries, bounded total), then log and
drop. Never retry forever, never block the server. Logs name the
person, device title and a short token prefix, never the provider
token.

**Admin.** `exemplar -administrare` gains `pulsus-inscriptiones`
(list, tokens shortened) and `pulsus-probare <civis>` (send a fixed
test notification to that person's devices) - the finish line is
checked with the second before pictor is involved (my call).

### III.2 APNs on the server (Linux)

- **Config** mirrors the email sender: `<data>/apns.stml` (non-secret:
  clavis_id, turma, thema list, hospes for each ambitus) and
  `<data>/apns.clavis` (the .p8 key, mode 0600, owner exemplar).
  Either missing -> pulsus still accepts registrations and batches but
  logs "pulsus: apns absens - non mittitur" instead of sending.
- **Environments.** TestFlight builds receive production tokens
  (`api.push.apple.com`); development installs (`tools/ios/app.sh`)
  receive sandbox tokens (`api.sandbox.push.apple.com`). The app
  declares its ambitus at registration from how it was built (a build
  define set by `divulgare.sh` vs `app.sh`); the server keeps one
  HTTP/2 connection per ambitus, opened lazily and kept (Apple asks
  for long-lived connections).
- **A non-blocking sender.** Today `apns_mittere` runs its own
  reactor until answered and sends a fixed body - right for the CLI,
  wrong for the server. pulsus needs a sender on exemplar's reactor
  (like the email sender): requests queued, answers through a
  callback, connections kept; the CLI keeps its blocking path on top
  of it.
- **Provider token** reused for 20-50 minutes, then re-signed (Apple
  refuses tokens older than an hour and throttles re-signing more
  often than every 20 minutes).
- **Linux build.** The first time `hpack`/`http2*`/`apns`/`tls13`
  compile and run on Linux. Whatever the Mac-only phase left
  Mac-specific (e.g. the SecureTransport comment in `http2_nexus.c`,
  address resolution, any `__APPLE__` paths) is fixed here; the house
  TLS 1.3 backend is used.
- **The key reaches the server only on Fran's go** (copy over ssh,
  install 0600 as exemplar; never through the repo, argv, logs or
  lectiones).

### III.3 The phone (iOS shell + communio client)

- **Shell (`lib/testa_ios.m`), general for every phone app:**
  ask permission (alert + sound) through UNUserNotificationCenter;
  `registerForRemoteNotifications`; deliver the token (hex) and any
  registration failure to the app as host events (the `TestaAmbitus`
  pattern); deliver a tapped notification's `pulsus` payload to the
  app on warm start AND cold launch (the shell is scene-based: the
  notification-center delegate's `didReceiveNotificationResponse`,
  set before launch finishes, covers both). While the app is in the
  foreground, notifications are not shown (my call - live sync already
  refreshes the screen).
- **Entitlement.** The app is signed with `aps-environment`
  (development for `app.sh`, production for `divulgare.sh`). The
  wildcard development profile cannot carry push, so development
  builds need an explicit profile for `earth.golden.pictor` with push
  (see IV).
- **A short grace period when backgrounded (my call, builder
  check).** Locking the phone right after the last stroke must not
  lose the notification. When the app leaves the foreground the shell
  asks iOS for a background task (`beginBackgroundTask`, ~30 s) so
  in-flight uploads and the event post finish; an event still unsent
  when the time runs out is kept in the volume and posted on the next
  launch (the server merges it into the batch or opens a new one).
  This is a send-only grace, not background sync (VI stands).
- **Communio client.** A small `communio_cliens` addition: register
  (PUT inscriptio, retried on failure, repeated whenever iOS hands a
  new token) and post events. These are plain requests through the
  existing platform HTTP interface (veredarius), so the URLSession
  path and the Mac's house client both work.

### III.4 pictor as the first client

- **When to ask.** Right after sign-in succeeds, and on the first
  launch of the update for a device that is already signed in (my
  call - the moment the question makes sense). Never before sign-in.
- **Events.** When pictor's own acts for a picture (new picture, new
  strokes, Edit a copy) are accepted by the server, it posts an event
  `clavis "pictor.picturae"`, `res [<picture id>]`, `mora 300`. The id
  is the picture's synced identity - the same on every device, the one
  the Shared grid already resolves (builder confirms which; AUDIENDA).
- **Notify now.** A ⋯ menu entry "Notify family now" (signed in only)
  posts `statim:true` with the open picture if one is open, else the
  pending batch alone; nothing pending and nothing open -> entry is
  disabled.
- **Tap -> transient group.** The viewer gains a mode whose order is
  the given id list instead of the grid's; swipe / ← → loop through
  just those; × returns to the Shared grid (page 1). Pictures not yet
  synced: pictor syncs first, showing the grid's "loading" line, then
  opens; ids still unknown after sync are skipped; none left (or
  `plures`) -> the Shared grid.

### III.5 Phase 2: the Mac

- **`pictor.app`.** Built through fasciculum with identity
  `earth.golden.pictor` and the pictor icon.
- **Where its data lives (Fran 2026-11-02).** Two kinds: APP-WIDE
  data (the push registration, the last-opened document) goes in
  `~/Library/Application Support/pictor/`; the volume is a DOCUMENT
  and stays where it is. Today `bin/pictor` finds `pictor.volumen`
  relative to the current directory (`apps/pictor/pictor.c:81`), which
  an app launched from the Dock or a notification cannot; so the app
  remembers the last-opened volume by absolute path (first run: the
  existing main-checkout volume, given once). Nothing is moved, the
  `.tessera` beside it is untouched, the Mac stays signed in, and
  `bin/pictor` from the repo keeps working. A notification tap opens
  the remembered document. *Fran: "long term we want our volumen
  based things to be genuinely document based where i could have
  multiple pictor volumen on the computer (potentially even as .pictor
  files ...)"* - that is its own project (tabularium desideratum
  "pictor documenta"); phase 2 must not block it.
- **Signing.** Apple Development certificate + a macOS development
  provisioning profile for `earth.golden.pictor` with push
  (`com.apple.developer.aps-environment`), Fran's Mac registered as a
  device. Sandbox ambitus. fasciculum gains "sign with identity +
  embed profile + entitlements" as a general option (briar `-app` can
  use it later).
- **Shell (`lib/fenestra_macos.m`):** the same three pieces as iOS
  (permission, token, tap) behind the same host events, so pictor's
  code is shared.
- Distributing the Mac app to others (Developer ID, notarization) is
  out of scope.


## IV. Apple-side steps (each on Fran's go; create, never revoke)

1. Switch PUSH_NOTIFICATIONS on for App ID `earth.golden.pictor`
   (`apns -praeparare` already inspects; enabling invalidates existing
   profiles).
2. Regenerate the App Store distribution profile used by
   `divulgare.sh`; create an explicit iOS development profile for
   `earth.golden.pictor` (push) covering Fran's and Amanda's phones.
3. Copy the APNs key to the server (III.2).
4. Phase 2: make sure the App ID covers macOS, register Fran's Mac,
   create the macOS development profile.
5. A new TestFlight build (main only) carrying the entitlement.


## V. Tests and oracles

- **pulsus, sans network:** a fake sender (function pointer) and fake
  clock: events restart the timer; per-sender and per-key separation;
  statim; no-max-wait; distinct item order; restart re-arms from the
  table (due rows fire at once); recipients exclude the sender person,
  revoked devices, other spaces and other themata; 410/BadDeviceToken
  delete the inscription; retries are bounded; payload truncation at
  4096 with `plures`; route validation (400s, 401).
- **Against the Go oracle:** the T11 `/3/device/` route in
  `tools/oracula/http2_servus.go` extended to answer 200 / 410 / 429 by
  token, so pulsus -> apns -> http2 is exercised end to end on the Mac
  and on Linux (Go stays a fixture recorder; recordings checked in, no
  Go needed to run the tests).
- **Linux:** the http2 and apns suites run green on the VPS build (or
  a Linux build host) before deploy.
- **Shell and pictor:** the transient-group viewer mode (order,
  looping, skipping unknown ids, falling back to the grid) in pictor's
  existing probationes; host-event parsing of the payload.
- **Live finish lines** (sections above), after `pulsus-probare`
  succeeds on each device.


## VI. Not in this spec

Per-picture or per-person recipients; in-app mute; badges and unread
counts; notification grouping beyond `thread-id`; background sync
(the app still syncs only when open); Android push (FCM); Mac
distribution beyond Fran's Mac; replacing nginx; the server role of
our HTTP/2.


## VII. Left to the builder

Exact Latin names below `pulsus` (tables, routes, functions); table
layout details; the backoff numbers; how the build define reaches the
app; how the viewer's group mode is represented in pictor's state;
whether the admin verbs go through the existing administrare command
table; how fasciculum's signing option is spelled.


## AUDIENDA (not verified while writing)

- That key `LA8Z282UYC` works against the **production** APNs host
  (Apple can restrict a key to one environment); only the sandbox was
  tried.
  **Answered 2026-11-04: it does not** - see VIII.
- That TestFlight builds really carry `aps-environment production`
  once the regenerated distribution profile is used.
  **Answered 2026-11-03: yes** (build 9, checked in the signed app).
- Which identity pictor's synced pictures carry that every device
  shares (the Shared grid's `communia.pinax.<k>` is a local index).
- That the existing App ID can be extended to macOS, or whether the
  Mac needs its own App ID (and then its own topic).
- That nothing in `http2_nexus.c` / `apns.c` / `tls13.c` is
  Mac-specific beyond the comment noted; Linux has not compiled them.
  **Answered: compiled for Linux (T6) and running on the droplet
  since 2026-11-03.**
- That URLSession-based networking on iOS needs no change for these
  two small requests.
- iOS behavior when permission is denied and later granted in
  Settings (the app re-registers on each launch; untested).


## VIII. As run (T11, 2026-11-03 to 11-04)

- **Apple (step 1, 11-03):** PUSH_NOTIFICATIONS switched on for
  `earth.golden.pictor`; development profile "rhubarb pictor
  Development 2026-11-03" (Fran's three devices, `aps-environment
  development`) made by the new `nundinae_apple -evolutio`; App Store
  profile "... 2026-11-03" (`production`). The old App Store profile
  went INVALID and was left in place.
- **Droplet (step 2, 11-03):** main merged into quarta (ab12a59d) and
  main fast-forwarded to it first, so the server runs main. Volumes
  backed up as `*.volumen.ante-pulsus` with exemplar stopped (the
  write-ahead logs held a week of data; a plain copy of the volume
  file would have lost it). `apns.clavis` by `secretum_mittere.sh`,
  `apns.stml` naming both hosts. Log: `pulsus: apns paratus -
  api.push.apple.com, sandbox api.sandbox.push.apple.com`.
- **Sandbox (step 3, 11-03):** development build on Fran's phone over
  Wi-Fi; registration arrived (sandbox); `pulsus_probare fran` ->
  Apple 200, banner on the lock screen, tap opened pictor.
- **Production (step 5, 11-03/04):** TestFlight pictor 1.0 (9) from
  main ab12a59d. Amanda's phone registered (production), but both
  sends failed with **`BadEnvironmentKeyInToken`**: key LA8Z282UYC is
  sandbox-only. Fran created key **4M5T3MMW2J (Sandbox &
  Production)**; swapped on the droplet (`secretum_mittere.sh
  -renovare`, `clavis_id` in `apns.stml`, restart). Then
  `pulsus_probare amanda` -> 200 and a banner; a real drawing session
  (uploads 15:36:39-15:39:04 UTC) produced one batch sent at
  15:44:04 UTC, 5 minutes after the last event, accepted by Apple.
- **Not recorded: Apple's `apns-id`s.** The server logs success or
  failure only. Times stand in for them (ruling); logging the ids
  (not secret) is a small follow-up.
- **Bug found live:** "Notify family now" sent dead stack bytes as the
  picture id (server 400, event dropped) - historia kept the
  caller's `spatium` pointer and "New picture" builds it in a stack
  buffer. Fixed at the cause in fdaadd4d (historia copies it); to
  reach phones in TestFlight build 10.
- **Lesson for the next app/key:** check a new APNs key's environment
  in the portal before shipping; the error only shows on the first
  production send.


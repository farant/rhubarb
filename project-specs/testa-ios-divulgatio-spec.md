# Publishing house iOS apps to TestFlight (spec C2)

**Status: IMPLETUM 2026-10-25** - designed in conversation with Fran
(2026-10-24), builder-checked, approved ("that looks good for me"); plan
`project-specs/testa-ios-divulgatio-plan.md`; built in T1-T5; pictor 1.0
(5) published to TestFlight, Fran's wife invited. Section VI records
what building decided and measured. Worktree quarta (= main
bd438fd7 when written). Follows C1 (`project-specs/testa-ios-spec.md`,
pictor on Fran's iPhone) and C1b (photo import), both IN MAIN. Ledger:
mobile vision desideratum ...577GAS.

*Fran 2026-10-24: "ideally it would be kind of like the merge behavior.
i'm on my laptop connected to the internet, i tell you it's ready to
publish, we publish the app and the new version shows up in test flight";
Fran's wife "is already a member of the App Store Connect team"; "my
understanding is that the api cannot create the initial record for the
app"; prunifex "was just set up to deploy through pushing to github ...
linked in app store connect web app" (not the model); icon: "probably a
pictor illustration itself would work well", actool "(a) is okay for
now"; What to Test notes: "something you can draft unless i tell you to
include something specific"; "probably only what is main would be good";
after the builder check: "(a) for one sounds good, and part of this is
that we are also just making general purpose libraries that could be
used to implement workflows (ours being the first and only for now) so i
think it makes sense to support things if the api supports it. i can
make sure she can see all apps in App Store Connect".*

Goal: **Fran says "publish pictor"; one house command builds pictor from
main for real iPhones, signs it for distribution, uploads it through the
App Store Connect API, and makes it available to the internal TestFlight
group Fran's wife is in - minutes later it is in her TestFlight.** On the
way the house gets its own App Store Connect API client (C89, our TLS),
reusable for every later house iOS app ("vicus mobile").

Decisions (Fran, 2026-10-24, in conversation, unless marked "my call"):

- **D1 Publishing = merging:** a deliberate act from the laptop, one
  command, after Fran says so. TestFlight is the family's channel
  (C1 D2: phone install from the Mac = "commit", TestFlight = "merge").
- **D2 Internal testers only:** Fran's wife is on the App Store Connect
  team, so builds reach her without Apple's beta review. External
  testers are out of scope.
- **D3 Our own client, Apple's web API:** a house C89 library talks to
  the App Store Connect API over our TLS - no Xcode project, no
  XcodeGen, no GitHub/Xcode Cloud workflow (prunifex is not the model),
  no fastlane. Since 2025 the API has public **Build Uploads**
  (`buildUploads`, `buildUploadFiles`: create, reserve, PUT parts to
  given URLs, commit, poll), so altool/Transporter are not needed.
- **D4 The app record is created by Fran once, in the website** (the
  API cannot create apps). Everything else is the tool's.
- **D5 The icon is a pictor drawing:** Fran draws pictor's icon in
  pictor as pixel art (my call: square, e.g. 128 x 128), committed next
  to the app; the build scales it by an integer factor (nearest
  neighbour) to the 1024 x 1024 App Store icon.
- **D6 Apple's asset compiler for the icon, for now (Fran: "(a) is okay
  for now"):** App Store uploads require the icon inside a compiled asset
  catalog (`Assets.car`) named by `CFBundleIconName`. The build writes a
  tiny `.xcassets` folder (PNG + `Contents.json`) and runs `xcrun actool`
  - a command-line compiler shipped with Xcode, like `clang`/`codesign`.
  A house `.car` writer is parked (undocumented format).
- **D7 "What to Test" is drafted by Claude** from the commits since the
  last publish (Fran may dictate additions); the tool takes the text as
  input.
- **D8 Only main is published:** the command refuses unless it runs in
  main's checkout, on branch main, with a clean tree; the build records
  the commit it came from (e.g. in its Info.plist, `RhubarbCommissum`).
- **D9 Encryption declaration from the build (my call, Fran agreed it
  is a good idea in conversation):** `ITSAppUsesNonExemptEncryption` is
  derived from the app's aedilis closure: no TLS / BearSSL / cipher
  objects -> `false`; any present -> the tool REFUSES and names them
  (a networked app needs its export answer decided by Fran first). pictor
  contains only SHA-256 hashing (integrity, not encryption) -> `false`.
- **D12 Admin API key; a general-purpose library (Fran):** the team key
  has role **Admin**, so the tool can create the distribution
  certificate and App Store profile itself (the anti-goals - create,
  never revoke or delete - cap what it does with that power). The
  library is a general App Store Connect client, not a pictor-publish
  script: its shape (token, JSON:API requests, pagination, errors,
  resource helpers) supports what the API supports, our TestFlight
  workflow being its first consumer. C2 implements the resources the
  workflow needs, on a generic core where more are cheap to add.
- **D11 Last published commit lives at Apple (my call, after the
  builder check):** the "What to Test" text ends with a line
  `commit <hash>`; the next publish reads the latest build's text through
  the API to find where its commit list starts. No local state file, no
  git tag (main's checkout stays untouched).
- **D10 Build numbers automatic, version by hand (my call):**
  `CFBundleVersion` = one more than the highest build of that app known
  to App Store Connect (read through the API); `CFBundleShortVersionString`
  stays "1.0" until Fran changes it (a per-app file).

Anti-goals (my calls, Fran: "that looks great"):

- The API key (`.p8`) and the distribution private key are never read
  into a transcript, printed, logged, copied into the repo or `build/`.
  The tool checks they exist and are mode 0600, nothing more (house rule
  for secrets: never route a secret through `lectiones`).
- Nothing on Apple's side is ever revoked or deleted (certificates,
  profiles, bundle IDs, apps, testers). Create what is missing, reuse
  what exists. prunifex and every other team resource stay untouched.
- No silent half-publish: every Apple refusal (upload rejected,
  processing failed, validation error) is reported with Apple's own
  reason, and the command exits non-zero. A failed publish can be re-run.


## I. What exists (2026-10-24)

| Fact | Where |
|---|---|
| iOS build for the phone (closure via aedilis `--varians ios,posix`, `iphoneos` SDK, Info.plist template, signing, devicectl) | `tools/ios/app.sh`, `tools/ios/communia.sh`, `tools/ios/Info.forma.plist` |
| Development signing chosen by data (profile `<team>.*`, identity by certificate SHA-1); team `Q5WC9FQ3J2` | `tools/ios/communia.sh` |
| ECDSA P-256 raw signing (ES256 = r ‖ s, 64 bytes), PKCS#8 private key decoding (`br_skey_decoder`), SHA-256, MD5 (Build Uploads' whole-file checksum is MD5), HMAC, RSA key generation (`br_rsa_keygen`), RSA PKCS#1 signing, PKCS#8 RSA key encoding (`br_encode_rsa_pkcs8_der`) | vendored BearSSL `vendor/bearssl_amalgama.{c,h}` (behind `tls_bearssl`) |
| HTTP client, JSON, base64, DER reading, signed S3 uploads (PUT of parts to URLs) | `include/http_vocatio.h`, `json.h`, `base64.h`, `fiducia_der.h`, `horreum*.h` + `signatura_aws.h` |
| TLS 1.3 own client (house default) with BearSSL 1.2 fallback | `include/tls.h`, `tls13*.h` |
| Icon PNG writing (macOS `.icns` / `.iconset` from one image); it only scales DOWN (larger sizes go to `omissa`) and not nearest-neighbour - the 128 -> 1024 upscale needs a small helper of its own | `include/icones.h:100`, `lib/icones.c:576, 603` |
| `app.sh -tabula` is development-only: it needs a connected phone first, accepts only development profiles (`get-task-allow`, UDID listed) and writes `get-task-allow` true; `Info.forma.plist` hard-codes version 1.0 / build 1 and has none of Xcode's `DT*` toolchain keys | `tools/ios/app.sh:68-72, 135-136`, `tools/ios/communia.sh:136-141`, `tools/ios/Info.forma.plist:23-26` |
| Build Uploads facts: `app` is a REQUIRED relationship of the BuildUpload (no wrong-app guessing); whole-file checksum MD5, per-part MD5 or SHA-256; each upload operation names its own method, offset/length and headers; IPA `uti` = `com.apple.ipa`; beta groups have `hasAccessToAllBuilds` | Apple reference pages (builder check 2026-10-24) |
| Xcode 27.1 (iOS 27.1 SDK) on macOS 27 - meets TestFlight's SDK floor | measured 2026-10-24 |
| Keychain: "Apple Development: Francis Arant (44TZ4R8793)" only; no "Apple Distribution" identity | measured 2026-10-23 |


## II. Design

### II.1 The API client library (`lib/nundinae_apple.c`, `include/nundinae_apple.h`)

Name (my call, refined with Fran 2026-10-24): **nundinae** (Latin
"market") is a FAMILY of store libraries - this spec builds Apple's App
Store Connect client `nundinae_apple`; Amazon Appstore / Google Play
siblings (`nundinae_amazon`, `nundinae_google`) may follow (Fran: "we
will probably have a similar library for ... amazon fire store / normal
android store"). What they share (credentials handling, the "last
published" record, the publish command's shape) moves into a common
`nundinae` core when a second store exists - not before.


- **Credentials:** a non-secret config `~/.rhubarb/ios/nundinae_apple.stml`
  (key ID, issuer ID, team ID, path of the `.p8`), the `.p8` itself at
  `~/.rhubarb/ios/AuthKey_<KEYID>.p8`, mode 0600, placed by Fran. The
  library reads the key into memory only to sign (never logs it, never
  through `lectiones`).
- **Token:** ES256 JWT (header `alg ES256, kid, typ JWT`; claims `iss`,
  `iat`, `exp` <= 20 min, `aud appstoreconnect-v1`), base64url, signed
  with the PKCS#8 P-256 key; cached and renewed before expiry.
- **Requests:** JSON:API over `https://api.appstoreconnect.apple.com/v1`
  with our HTTP client; pagination followed; Apple's `errors[]` turned
  into one named failure (status, code, title, detail).
- **Uploads:** follow each returned operation exactly (method, URL,
  offset/length, headers); MD5 of the whole file for the commit.
- **Resources C2 needs:** bundle IDs (find/create `earth.golden.<app>`),
  certificates (find/create `DISTRIBUTION` from a CSR), profiles
  (find/create `IOS_APP_STORE` for bundle ID + certificate, download),
  apps (find by bundle ID - created by Fran), builds (list, highest
  version, processing state), build uploads + upload files (create,
  reserve, PUT parts to the given URLs with the given headers, commit,
  poll state), beta groups (find/create the internal group; add build),
  beta build localizations ("What to Test" text), beta testers (ensure
  Fran's wife is in the group - by email Fran gives once).
- Pure parts (JWT assembly, JSON:API parsing, error extraction,
  pagination links, CSR DER) are host-tested against fixtures; network
  calls are exercised against Fran's account by hand / a smoke step.

### II.2 Distribution identity (first publish, once per team)

The tool generates an RSA 2048 key, builds a PKCS#10 CSR (DER), asks the
API for an `DISTRIBUTION` certificate, then imports key + certificate
into Fran's login keychain (`security import`) so `codesign` can use it.
The private key's backup goes to `~/.rhubarb/ios/distributio.clavis`
(mode 0600, PKCS#8) - a secret like the API key. If a usable "Apple
Distribution" identity is already in the keychain and on the team, it is
reused. (Apple limits distribution certificates per team; never revoke -
D anti-goals.) **At the cap (my call):** the tool stops and lists the
team's distribution certificates (name, expiry, who made them); Fran
decides in the website - the tool never revokes. **Keychain access (my
call):** the key goes into the LOGIN keychain; on the first distribution
signing macOS asks once whether `codesign` may use it - Fran clicks
"Always Allow" (first publish is attended). No extra keychain, no
keychain password stored anywhere.

### II.3 The publish command

`tools/ios/divulgare.sh <app>` (name my call, Latin "make public";
wraps a C tool `bin/nundinae_apple` for the API work), run by Claude when Fran
says "publish <app>":

1. **Refuse unless** main's checkout, branch main, clean tree (D8);
   config + key present and 0600; Xcode present.
2. **Encryption check** (D9) from the aedilis closure.
3. **Account preparation** (idempotent): bundle ID, distribution
   identity (II.2), App Store profile, app record found (else exit 2
   telling Fran to create it in the website with the exact bundle ID
   and name), internal group with the tester.
4. **Build:** a DISTRIBUTION branch of `app.sh` (no phone, no
   development profile, no `get-task-allow`) for `iphoneos` with: the
   Xcode toolchain keys (`DTPlatformName`, `DTSDKName`, `DTXcode`,
   `DTXcodeBuild`, `DTPlatformBuild`, `DTPlatformVersion`, `DTCompiler`,
   `DTSDKBuild`, `BuildMachineOSBuild`) read from the installed Xcode,
   build number (D10),
   version, commit (D8), `CFBundleIconName` + `Assets.car` from the icon
   (D5, D6), `ITSAppUsesNonExemptEncryption` (D9); embedded App Store
   profile; distribution signing with entitlements from that profile
   (no `get-task-allow`); package `Payload/<app>.app` as `<app>.ipa`.
5. **Upload** through Build Uploads; wait for Apple's acceptance, then
   for the build's processing to finish (polling, with a ceiling and the
   last state named on timeout).
6. **TestFlight:** set the "What to Test" text (D7, ending with the
   commit line of D11), add the build to the internal group (or the
   group has `hasAccessToAllBuilds` - builder's choice).
7. Print: app, version, build number, commit, group, and where it is.

### II.4 The icon (D5, D6)

`apps/<app>/icon.png` (pixel art; my call: square, side dividing 1024)
-> integer nearest-neighbour scale to 1024 x 1024 (a small helper -
`icones` only scales down), no alpha (App Store icons must be opaque) -> `AppIcon`
appiconset with a single 1024 universal entry -> `xcrun actool
--compile` (iOS, minimum 17.0) -> `Assets.car` + Info.plist partial
merged into the app's Info.plist. The phone installs of `app.sh -tabula`
get the same icon (no more blank icon). Until Fran draws it, a
placeholder (my call: a plain framed "P" in the pictor palette).

### II.5 One-time setup Fran does in the website (documented in the spec's VI when built)

1. Users and Access -> Integrations -> App Store Connect API: create a
   **team key** with role **Admin** (D12); download the `.p8` once; put it at
   the path above, `chmod 600`; give Claude the key ID and issuer ID (not
   secret).
2. After the tool has registered `earth.golden.pictor`: Apps -> + -> New
   App (iOS, name "pictor" or as Fran likes, bundle ID
   `earth.golden.pictor`, SKU free text).
3. Make sure Fran's wife's App Store Connect user has **access to all
   apps** (Users and Access), so a new app like pictor is visible to her
   as an internal tester (Fran 2026-10-24: will check).
4. Nothing else: the group, the tester, certificate and profile are the
   tool's.


## III. Tests

1. **Pure (host, credo):** JWT header/claims bytes and base64url; ES256
   signature verifies against the public key (BearSSL verify) for a
   test key generated in the test; JSON:API parsing of recorded
   fixtures (lists, pagination `links.next`, `errors[]`); CSR DER parses
   back (subject, public key) and its signature verifies; build-number
   rule; encryption-check rule (closure with / without tls objects);
   Info.plist keys added.
2. **Icon:** 128 x 128 fixture -> 1024 x 1024 PNG, exact nearest
   neighbour, opaque; `actool` produces `Assets.car` (smoke, needs
   Xcode).
3. **Refusals:** not main / dirty tree / key mode not 0600 / config
   missing / app record missing -> exit 2 naming the cause, before any
   network write.
4. **Live, by hand (Fran's account):** read-only calls first (list apps,
   builds); then the first real publish of pictor; Fran's wife sees the
   build in TestFlight.
5. **Gate:** no network in gates; the pure suites run in the normal
   gates; the build-with-icon step joins `ios-struere` (simulator, no
   signing).


## IV. Not in this spec

App Store release and review, external testers / beta review,
screenshots and store metadata, our own `Assets.car` writer, Android
store distribution, notarized macOS apps, the pinacotheca.


## V. Order of work (the plan will follow approval)

1. API client core: config, ES256 JWT, a read call (list apps) against
   Fran's account.
2. Distribution identity + App Store profile (II.2).
3. Icon in the build (II.4) - also for `-tabula` installs.
4. Packaging + Build Uploads + processing wait.
5. TestFlight: group, tester, What to Test; `divulgare.sh` end to end;
   first publish of pictor.


## AUDIENDA (not verified when written)

- Field-level request bodies of `buildUploads` / `buildUploadFiles`
  (read the reference pages first; checksum = MD5, operations
  self-describing - see I).
- Whether `actool` alone (no Xcode project) satisfies the upload's icon
  validation (`CFBundleIconName` + 1024 entry) - first upload settles it.
- The current US export rule for apps bundling standard encryption
  (only matters for a future networked app; D9 refuses until decided).


## VI. Built (2026-10-24 .. 25)

Commits in quarta: T1 c6fa6616 (client core, `-status`), T2 ba0cffd7
(account preparation), T3 db1096e5 (icon), T4 3bf0e836 (distribution
build, Build Uploads), T5 38ab4dcf (TestFlight, `divulgare.sh`), close
(this commit: R8 switch removed, this section).

**Publishing now:** Fran says "publish <app>"; in main's checkout Claude
runs `tools/ios/divulgare.sh <app> -siccum` (version, last published
commit read from Apple, commits since), drafts the notes, then
`tools/ios/divulgare.sh <app> -notae <file>`. Diagnostics:
`bin/nundinae_apple -status` (agreements), `-testatores <app>`,
`-invitare <app> <email>`, `-quaerere </v1/...>` (GET only). The CLI is
built through aedilis (`build/aedilis/nundinae_apple/nundinae_apple`).

**Rulings made while building**

- **Agreements as a control-plane check (T1):** every request fails with
  `FORBIDDEN.REQUIRED_AGREEMENTS_MISSING_OR_EXPIRED` until the account
  holder accepts Apple's agreements; `-status` exits 3 naming it (Fran:
  "a great kind of control plane function").
- **Tester fallback (T2):** relating an existing team member to the
  group gave 409 "cannot be assigned"; creating the tester with the
  group relationship works.
- **Icon (T3):** App Store icons may not carry alpha - an RGB PNG writer
  was added; actool nests `CFBundleIconName` only inside
  `CFBundleIcons`, so the build also sets it at the top level.
- **Encryption check (T4):** prefixes `tls`, `bearssl`, `aes_`,
  `aesctr`, `chacha`, `crypt` on the base names of linked objects (`O`,
  `V` in `aedilis --partes`); not plain `aes`, which catches Latin
  words (aestimatio). Pictor: 70 objects, none.
- **Upload commit checksum (T4):** `sourceFileChecksums` = `file` only,
  MD5 hex UPPERCASE (as the `entityTag` Apple shows after the PUT);
  lowercase plus a composite -> 409 ATTRIBUTE.INVALID. Apple fills the
  composite itself.
- **Signing (T4):** entitlements = the App Store profile's minus
  `get-task-allow` and the wildcard `keychain-access-groups`.
- **Invitations (T5, Fran):** after the build joins Familia, testers
  still `NOT_INVITED` get `POST /v1/betaTesterInvitations`; `REVOKED`
  is never re-invited. On the first publish Apple had already invited
  Fran's wife when the build joined the group (state INVITED), so no
  second email went out.
- **Version (T5):** `apps/<app>/versio` (one line) else 1.0; the build
  number is automatic (D10).

**Measured / checked:** suite `probatio_nundinae_apple` 12 sections
green (plants red: broad `aes`, locale check removed); every
`divulgare.sh` refusal shown before any network call (checkout,
branch, dirty tree, key not 0600, usage); build 4 (test upload) and
build 5 (first publish) processed VALID in about 9 and 12 minutes,
`usesNonExemptEncryption` false (no export question in App Store
Connect); ipa 1 MiB; `-ultimum` reads back the published commit;
builds 1-3 are dead uploads (a stale binary sent the rejected checksum
three times - `compile_tools.sh` links old `build/*.o`).

**AUDIENDA answered:** request bodies read from Apple's DocC JSON
(`developer.apple.com/tutorials/data/documentation/appstoreconnectapi/
<page>.json`); actool alone satisfies the icon validation (builds 4
and 5 VALID); the export rule remains open - D9 still refuses any app
that links TLS until it is decided.

# Publishing to TestFlight (C2) Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:executing-plans (Fran chose inline, one task per turn, Fran approves each). Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** `tools/ios/divulgare.sh pictor`, run from main's checkout when Fran says "publish pictor", builds, signs, uploads and hands the build to the internal TestFlight group Fran's wife is in.

**Architecture:** a general App Store Connect client `nundinae_apple` (C89: ES256 token, JSON:API requests over the house blocking HTTP client, resource helpers) with a CLI `bin/nundinae_apple`; `tools/ios/app.sh` gains a DISTRIBUTION build (icon via `actool`, toolchain keys, App Store profile, IPA); `tools/ios/divulgare.sh` orchestrates. Pure parts host-tested; the account parts proved live with Fran's key.

**Tech Stack:** C89 in Latin (latina.h), credo v2, vendored BearSSL (ES256, MD5, RSA keygen/sign, PKCS#8), `http_exsequi`, `json`, `base64url_codificare`, aedilis, `xcrun actool` / `codesign` / `security`.

**Spec:** `project-specs/testa-ios-divulgatio-spec.md` (approved 2026-10-24).

## Global Constraints

- C89 + house flags; Latin identifiers; latina.h macros never identifiers; Latin lint blocks commits (`apple` is a proper name: glossary `ignotum-permissum`, like `android`, `ios`).
- Secrets: `~/.rhubarb/ios/AuthKey_<KEYID>.p8` and `~/.rhubarb/ios/distributio.clavis` never printed, logged, copied into the repo or `build/`, never through `lectiones` (raw read with `<tolera codex="CANALIS_PRAETERITUS">`, precedent `lib/horreum.c:737`); mode must be 0600 or refuse. Claude never reads them.
- Apple side: create what is missing, reuse what exists; NEVER revoke or delete anything.
- Every Apple refusal reported with status + code + title + detail; non-zero exit; re-runnable.
- Publish only from main's checkout, branch main, clean tree (D8). No network in gates.
- Admin team key (D12); BuildUpload has its `app` relationship; whole-file checksum MD5; follow each upload operation's method/URL/offset/length/headers.
- Version "1.0" until Fran changes it (`apps/<app>/versio`); build number = max known + 1 (D10); "What to Test" ends with `commit <hash>` (D11).

## Review Focus

1. A long upload outlives the 20-minute token - expected: the token is renewed before expiry, no 401 mid-upload. Pinned in T1 (renewal rule).
2. A re-run after a failed upload - expected: the new build number is above every number Apple has seen, including uploads that never processed. Pinned in T4 (number rule over builds AND buildUploads).
3. Apple answers 429 / 5xx or a non-JSON body (proxy HTML) - expected: GETs retried with backoff (III, ceiling), writes never blindly retried; the failure names status + the first CC bytes, no crash. Pinned in T1.
4. A failing request's message - expected: method, URL path, status, Apple's error; never the token or a header value. Pinned in T1.
5. An app whose build contains TLS/BearSSL - expected: publish refuses before building, naming the objects. Pinned in T4.

## Rulings (made while planning)

- **R1 one library, one CLI:** `include/nundinae_apple.h` + `lib/nundinae_apple.c` (core + resources; split into `nundinae_apple_*.c` files only if it passes ~1500 lines); CLI `tools/nundinae_apple.c` -> `bin/nundinae_apple` (verbs below).
- **R2 config** `~/.rhubarb/ios/nundinae_apple.stml`: `<nundinae_apple clavis_id="..." emissor="..." turma="Q5WC9FQ3J2" via_clavis="~/.rhubarb/ios/AuthKey_<id>.p8"/>` (non-secret; Claude writes it from the IDs Fran gives).
- **R3 token renewal:** reuse while `exp - now > CXX` s, else make a new one (duration MCC s).
- **R4 retries:** GET only, on 429 / 500-504 / transport failure, III attempts, waits I, IV, XVI s (honour `Retry-After` if present, capped at LX s).
- **R5 CLI verbs** (all print one fact per line, exit 0 / 1 failure / 2 refusal): `-applicationes`; `-parare <app> -tester <email>`; `-proxima <app>`; `-mittere <app> <ipa> <versio> <aedificatio>`; `-ultimum <app>`; `-distribuere <app> <aedificatio> -notae <file>`; `-cryptographia` (stdin: aedilis `--partes`).
- **R6 icon:** `apps/<app>/icon.png`, square, side dividing 1024 (128 recommended), nearest-neighbour integer upscale, composited opaque over white (my call; pixel art keeps its colours, transparency becomes white).
- **R8 development exemption (Fran 2026-10-24: "we can loosen the publish from main rule while we are developing the library"):** until C2 closes, `divulgare.sh` accepts a non-main checkout when `DIVULGATIO_EXTRA_MAIN=1` is set - clean tree still required, the branch name printed and recorded in the notes' commit line (`commit <hash> (rhubarb-quarta)`). The closure task (T5 step 4) removes the switch.
- **R7 profile store:** downloaded App Store profiles at `~/.rhubarb/ios/profila/<bundleid>.mobileprovision` (not secret).

## File map

| File | Task | Role |
|---|---|---|
| `include/nundinae_apple.h`, `lib/nundinae_apple.c`, `probationes/probatio_nundinae_apple.c`, `probationes/fixa/nundinae_apple/*.json` | 1-5 | client + fixtures |
| `tools/nundinae_apple.c` | 1-5 | CLI |
| `oratio/glossarium.stml` | 1 | `apple` |
| `include/icones.h`, `lib/icones.c`, `probationes/probatio_icones.c` | 3 | `icones_amplificare` |
| `apps/pictor/icon.png`, `apps/pictor/versio` | 3, 4 | placeholder icon, "1.0" |
| `tools/ios/app.sh`, `tools/ios/Info.forma.plist` | 3, 4 | icon, distribution build, IPA |
| `tools/ios/divulgare.sh` | 5 | publish |
| `project-specs/testa-ios-divulgatio-spec.md` | 5 | IMPLETUM + VI |

---

### Task 1: client core - token, requests, errors, a read call

**Interfaces (produces):**
- `b32 nundinae_apple_configurationem_legere(constans character* via, NundinaeAppleConfiguratio* c, chorda* causa, Piscina* p)` (fields `clavis_id`, `emissor`, `turma`, `via_clavis`, chorda).
- `chorda nundinae_apple_tesseram_facere(constans i8* clavis_pkcs8_der, i32 mensura, chorda clavis_id, chorda emissor, s64 nunc_s, i32 duratio_s, Piscina* p)` - pure; empty chorda on failure.
- `b32 nundinae_apple_aperire(NundinaeApple* n, constans NundinaeAppleConfiguratio* c, chorda* causa, Piscina* p)` - checks the key is 0600, reads it (raw, tolerated), keeps DER in memory only.
- `NundinaeAppleResponsum nundinae_apple_vocare(NundinaeApple* n, HttpMethodus m, constans character* via, chorda corpus_json, Piscina* p)` - `{b32 successus; i32 status; JsonValor* corpus; chorda causa;}`; R3, R4.
- `chorda nundinae_apple_causa_ex(i32 status, chorda corpus, Piscina* p)`, `chorda nundinae_apple_proxima(constans JsonValor* r)` (links.next or empty), `b32 nundinae_apple_iterandum(HttpMethodus m, i32 status, b32 transportus_fractus)`, `b32 nundinae_apple_renovandum(s64 exspirat_s, s64 nunc_s)` - pure.
- CLI `-applicationes`: `<id>\t<bundleId>\t<name>` per app (paginated).

- [ ] **Step 1: Failing tests** `probatio_nundinae_apple.c` sections:
  - `"tessera"`: a P-256 key made in the test (`br_ec_keygen` with a fixed HMAC-DRBG seed, `br_encode_ec_pkcs8_der`); token = three base64url parts, no '='; header JSON = `{"alg":"ES256","kid":"TEST123","typ":"JWT"}`; claims `iss`="emissor-test", `iat`=1792800000, `exp`=1792801200, `aud`="appstoreconnect-v1"; signature part decodes to 64 bytes and verifies (`br_ecdsa_i31_vrfy_raw`) over `header.claims`; a truncated key -> empty chorda.
  - `"renovatio"`: (exp 1000, now 870) -> FALSUM; (1000, 881) -> VERUM.
  - `"causa"`: fixture `errores.json` (status "409", code "ENTITY_ERROR.ATTRIBUTE.INVALID", title, detail) -> contains all four; HTML body with status 502 -> "502" + first CC bytes; a body holding `Bearer eyJ...` never echoed beyond CC bytes and the token string given to the request never appears in any causa (Review Focus 4).
  - `"iteratio"`: GET 429/500/503/transport -> VERUM; POST/PATCH/DELETE any -> FALSUM; GET 404 -> FALSUM.
  - `"paginae"`: fixture with `links.next` -> that URL; without -> empty.
  - `"configuratio"`: fixture STML -> fields; missing attribute -> FALSUM naming it.
  - `"clavis"`: a temp file mode 0644 -> `aperire` FALSUM naming "0600"; mode 0600 with a test key -> VERUM.
  Run `./compile_tests.sh probatio_nundinae_apple` -> link fails.
- [ ] **Step 2: Implement** (BearSSL included as `lib/tls13_clavis.c` does; `apple` glossary entry; CLI with `-applicationes`); green.
- [ ] **Step 3: Live (Fran):** create the Admin team key (spec II.5.1), place it at the path, `chmod 600`, give Claude the key ID + issuer ID; Claude writes the config; `bin/nundinae_apple -applicationes` lists the team's apps (prunifex among them). Nothing written to Apple.
- [ ] **Step 4: Commit.**

### Task 2: account preparation - bundle ID, distribution identity, profile, group, tester

**Interfaces (produces):**
- `chorda nundinae_apple_petitionem_facere(constans br_rsa_private_key* sk, constans br_rsa_public_key* pk, chorda nomen_commune, Piscina* p)` - PKCS#10 DER, SHA-256 with RSA, subject `CN=<nomen_commune>`; pure.
- Resource helpers (each find-then-create, never delete): `nundinae_apple_signum_fasciculi(n, chorda bundle_id, chorda nomen, chorda* id)`, `nundinae_apple_certificatum_distributionis(n, chorda* id, chorda* der)`, `nundinae_apple_profilum(n, chorda id_signi, chorda id_certificati, chorda* contentum)`, `nundinae_apple_applicatio(n, chorda bundle_id, chorda* id)` (find only; absent -> FALSUM + causa telling Fran to create it in the website with the exact bundle ID), `nundinae_apple_grex(n, chorda id_app, chorda nomen, chorda* id)` (internal), `nundinae_apple_tester(n, chorda id_grex, chorda email)`.
- CLI `-parare <app> -tester <email>`: runs them in order; distribution identity per spec II.2 (RSA 2048, CSR, create, `security import` key + cert into the login keychain with `-T /usr/bin/codesign`, backup `~/.rhubarb/ios/distributio.clavis` 0600; reuse an identity already in keychain AND on the team; at Apple's cap -> stop listing the certificates); profile saved per R7.

- [ ] **Step 1: Failing tests** sections `"petitio"` (CSR from a test RSA key: outer SEQUENCE, version 0, subject CN bytes, SubjectPublicKeyInfo modulus = the key's, signature verifies with `br_rsa_i31_pkcs1_vrfy` over CertificationRequestInfo with SHA-256 OID) and `"responsa"` (fixtures: certificates list with 2 DISTRIBUTION + 1 DEVELOPMENT -> the 2 named; profiles list -> profile for a bundle ID found; app list without the bundle ID -> causa contains the bundle ID and "website"). Run -> red.
- [ ] **Step 2: Implement**; green.
- [ ] **Step 3: Live (Fran present):** `-parare pictor -tester <wife's email>` -> bundle ID registered; Fran creates the pictor app record (spec II.5.2) when told; re-run -> identity created (macOS key-access dialog: "Always Allow" if shown), profile saved, group "Familia" + tester. Re-run again -> every step "reused", nothing created.
- [ ] **Step 4: Commit.**

### Task 3: the icon in every iOS build

**Interfaces (produces):** `IconesStatus icones_amplificare(constans Imago* fons, i32 latus, Imago* exitus, Piscina* p)` - integer nearest-neighbour to `latus`, opaque over white (R6); `ICONES_ERROR_LATERA` if the side does not divide `latus` or the source is not square. `app.sh` (all paths): when `apps/<app>/icon.png` exists -> 1024 PNG -> `AppIcon.appiconset` (one universal 1024 entry) -> `xcrun actool --compile <app.app> --platform <sdk> --minimum-deployment-target 17.0 --app-icon AppIcon --output-partial-info-plist <p>` -> its keys merged into Info.plist.

- [ ] **Step 1: Failing test** `probatio_icones.c` section `"amplificare"`: 128x128 fixture with a known pattern and a transparent pixel -> 1024x1024; `out(x,y) = src(x/8,y/8)` for samples incl. corners; the transparent source pixel -> opaque white; 100x100 -> `ICONES_ERROR_LATERA`; 128x64 -> `ICONES_ERROR_LATERA`. Red -> implement -> green.
- [ ] **Step 2: Placeholder** `apps/pictor/icon.png` 128x128: a framed "P" in pictor's palette (made by a throwaway spike under `build/spica/`, the PNG committed; Fran replaces it by drawing in pictor).
- [ ] **Step 3: app.sh** icon path; `./tools/ios_struere_porta.sh` viridis; `Assets.car` in the simulator `.app`; `plutil -extract CFBundleIconName` = AppIcon; simulator screenshot shows the icon on the home screen (open it).
- [ ] **Step 4: Phone (Fran unlocks):** `-tabula -currere` -> pictor's home-screen icon is the placeholder. Commit.

### Task 4: distribution build, upload, processing

**Interfaces (produces):**
- `b32 nundinae_apple_cryptographia(constans chorda* obiecta, i32 n, Xar* nominata)` - VERUM if any path matches `tls`, `bearssl`, `cryptum`, `aes`, `chacha` (base name), names collected; pure.
- `i32 nundinae_apple_versio_maxima(constans JsonValor* aedificationes, constans JsonValor* missiones)` - max integer `version` over builds and `cfBundleVersion` over buildUploads (non-numeric ignored, none -> 0); pure.
- `b32 nundinae_apple_operationes_legere(constans JsonValor* r, Xar* operationes)` - `{methodus, url, offset, longitudo, capita}` per `uploadOperations`; pure.
- `b32 nundinae_apple_mittere(n, chorda id_app, constans character* via_ipa, chorda versio, i32 aedificatio, chorda* id_aedificationis, chorda* causa, Piscina* p)` - BuildUpload (app relationship, version, build, platform IOS) -> BuildUploadFile (`com.apple.ipa`, name, size) -> operations sent exactly as given -> commit with MD5 -> poll the upload state (every XV s, ceiling XXX min, last state named) -> wait for the build's processing (VALID; INVALID/FAILED named).
- `app.sh <app> -divulgatio -profilum <p> -identitas <sha1> -versio <v> -aedificatio <n> -commissum <hash>`: no phone; `Info.forma.plist` gains `@VERSIO@`, `@AEDIFICATIO@`, `RhubarbCommissum`, `ITSAppUsesNonExemptEncryption` false, and the DT* toolchain keys (spec II.3 step 4, values from `xcodebuild -version`, `xcrun --sdk iphoneos --show-sdk-version/--show-sdk-build-version`, `sw_vers -buildVersion`); entitlements = the App Store profile's `Entitlements` with `<team>.<bundleid>`; refuses (exit 2) when `bin/nundinae_apple -cryptographia` finds crypto; `Payload/<app>.app` -> `build/ios/divulgatio/<app>/<app>.ipa`. CLI `-proxima`, `-mittere`.

- [ ] **Step 1: Failing tests** sections `"cryptographia"` (pictor-like list -> FALSUM; list with `lib/tls13.c`, `vendor/bearssl_amalgama.c` -> VERUM naming both), `"versio"` (builds "1","12","3","beta" + uploads "13" -> 13), `"operationes"` (fixture with II operations -> methods, offsets, lengths, a header pair). Red -> implement -> green.
- [ ] **Step 2: Distribution build** of pictor offline (needs T2's identity + profile): `codesign -d --entitlements :-` shows no `get-task-allow`; Info.plist has the DT* keys, version, build, commit, `CFBundleIconName`; `unzip -l` shows `Payload/pictor.app/`.
- [ ] **Step 3: Live (Fran's OK):** `-mittere pictor <ipa> 1.0 <proxima>` -> accepted, processing VALID (build visible in App Store Connect, TestFlight tab, not yet in the group). If Apple's validation names a problem (icon, keys), fix and re-run with the next number (Review Focus 2).
- [ ] **Step 4: Commit.**

### Task 5: TestFlight distribution and the publish command

**Interfaces (produces):** `chorda nundinae_apple_commissum_ultimum(chorda notae)` (hash from the last `commit <hash>` line, empty if none; pure); `-ultimum <app>` prints the last published commit; `-distribuere <app> <aedificatio> -notae <file>` (beta build localization en-US `whatsNew`, build added to group "Familia"); `tools/ios/divulgare.sh <app> [-siccum | -notae <file>]`.

- [ ] **Step 1: Failing test** `"commissum"`: notes ending `commit 0e4434b7` -> "0e4434b7"; two commit lines -> the last; none -> empty. Red -> green.
- [ ] **Step 2: divulgare.sh:** refusals before any network (not main's checkout / branch / dirty tree / config / key not 0600 -> exit 2 naming it - tested by running it from quarta and with a dirty tree); `-siccum` prints app, last published commit (`-ultimum`) and `git log --oneline <ultimum>..HEAD` for Claude to draft notes; `-notae <file>` runs: `-parare` (reuse), `-proxima`, `app.sh -divulgatio`, `-mittere`, `-distribuere` with the notes + `commit <HEAD>` appended; prints app, version, build, commit, group.
- [ ] **Step 3: First publish (Fran says "publish pictor"):** from quarta under R8 (`DIVULGATIO_EXTRA_MAIN=1`), after showing the refusal without it. Claude drafts notes from `-siccum`; publish; Fran's wife sees pictor in TestFlight.
- [ ] **Step 4: Close:** remove the R8 switch; spec IMPLETUM + section VI (rulings, measurements, AUDIENDA answered); worklogs; memory `ios-toolchain` line; ledger note under ...577GAS; commit.

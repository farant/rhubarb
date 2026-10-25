# nundinae_apple worklog

## 2026-10-24 - first live contact with App Store Connect (C2 T1-T2)

- **Agreements block everything:** with a valid token every call answered
  `403 FORBIDDEN.REQUIRED_AGREEMENTS_MISSING_OR_EXPIRED` until the account
  holder accepted Apple's updated agreement; acceptance took a few minutes
  to reach the API. `-status` (cheapest GET) names it and exits 3; every
  verb prints the same hint (Fran: "control plane").
- **Tester into a new internal group:** Fran's wife already existed as a
  beta tester (`inviteType EMAIL`, from Prunifex's internal group "golden
  earth"). Adding that tester record to pictor's new internal group with
  `POST /v1/betaGroups/{id}/relationships/betaTesters` answered `409
  STATE_ERROR "Tester(s) cannot be assigned"`. `POST /v1/betaTesters`
  with `email` + the `betaGroups` relationship worked (she shows once in
  the group). `nundinae_apple_tester` tries the relationship first and
  falls back to that on 409. Her user: role ADMIN, allAppsVisible.
- **Distribution identity:** RSA 2048 + PKCS#10 (CN "rhubarb nundinae");
  Apple's certificate comes back with displayName = the account name
  ("Francis Arant"). Key as PKCS#1 PEM ("BEGIN RSA PRIVATE KEY") imports
  with `security import <file> -k login.keychain-db -t priv -f openssl
  -T /usr/bin/codesign`; the certificate (DER) with plain `security
  import`; the pair shows in `find-identity -v -p codesigning` by SHA-1.
  No keychain dialog appeared during import (codesign may still ask once
  at first signing).
- Profile names must be unique per team: "rhubarb <app> App Store
  <certificate serial>".
- `-quaerere </v1/...>` (GET only, pretty JSON) was the tool that found
  the tester problem - keep it as the first diagnostic.

## 2026-10-24 - C2 T4: Build Uploads live

- **Commit checksum:** `PATCH /v1/buildUploadFiles/{id}` with
  `sourceFileChecksums {file:{MD5, lowercase hex}, composite:{same}}`
  -> 409 ENTITY_ERROR.ATTRIBUTE.INVALID ("... is not a valid value for
  the attribute 'sourceFileChecksums'"). Accepted: `file` only, hex
  UPPERCASE (matches the `entityTag` Apple shows on the upload operation
  after the PUT, e.g. `40196EC3...`). Apple fills `composite` itself.
  Both changes went in together, so which one mattered is not proven;
  the uploaded-file record (`-quaerere /v1/buildUploadFiles/<id>`) shows
  the entityTag = the MD5 of what arrived - a good check that the bytes
  are intact before blaming the checksum.
- **Stale binary trap (cost build numbers 2 and 3):** `./compile_tools.sh
  nundinae_apple` links whatever `build/*.o` exists - it does NOT
  recompile a changed lib/ file. The library edits never reached
  bin/nundinae_apple; tries 2 and 3 sent try 1's body. Check:
  `strings <bin> | grep <a string you removed>`. app.sh now builds the
  CLI via `bin/aedilis tools/nundinae_apple.c` + its struere.sh (mtime
  tracked, ~1 s when unchanged, installs nothing).
- **Build numbers:** failed uploads (buildUploads that never finished)
  still consume their cfBundleVersion; `-proxima` counts them (Review
  Focus 2 confirmed live: 1 failed -> proxima 2). Builds 1-3 of pictor
  1.0 are dead uploads; 4 was the first committed one.
- **cryptographia prefixes:** plan said "aes"; narrowed to `aes_` /
  `aesctr` because Latin words start with "aes" (aestimatio, aestas) -
  a false hit would block a publish. Test pins `lib/aestimatio.c` clean.
  Only `O` and `V` lines of `aedilis --partes` are judged (headers don't
  link).
- **Xar is segmented**: never pass `xar_obtinere(x, 0)` as a C array
  (SIGBUS in `-cryptographia` at 73 entries) - copy to a flat array.
- **Distribution signing:** entitlements = the App Store profile's
  minus `get-task-allow` and minus `keychain-access-groups` (wildcard
  `TEAM.*`, unused): application-identifier, team-identifier,
  beta-reports-active. DT* keys from xcodebuild/xcrun/sw_vers;
  DTXcode for 27.1 = "2710".

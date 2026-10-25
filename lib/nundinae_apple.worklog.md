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

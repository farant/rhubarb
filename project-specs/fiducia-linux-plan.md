# fiducia on Linux - implementation plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use
> superpowers:executing-plans (Fran's choice for this repo: INLINE, one
> task per turn, Fran approves each before the next). Steps use
> checkbox (`- [ ]`) syntax for tracking.

**Goal:** outgoing HTTPS from Linux - a real certificate verifier
behind the unchanged `fiducia.h`, plus a house blocklist (OneCRL +
hand list, compiled in) on both platforms.

**Architecture:** a small portable DER reader for certificate parts
(`fiducia_der`), a portable blocklist check over a generated table
(`fiducia.c` + `fiducia_diffisa`), and `fiducia_posix.c` rewritten on
BearSSL `br_x509_minimal` with the distribution CA bundle. Both TLS
stacks keep calling `fiducia_iudicare` exactly as today.

**Tech stack:** C89 in Latin (latina.h), credo v2 tests, BearSSL
(vendored amalgam), json.h + stml.h for the generator, fabrica
generator action + generata gate, Docker images for Linux.

**Spec:** `project-specs/fiducia-linux-spec.md` (approved by Fran
2026-10-22). Read it with this plan; it is the authority.

## Global constraints

- House C89 flags (`-std=c89 -pedantic -Wall -Wextra -Werror
  -Wconversion -Wsign-conversion -Wcast-qual -Wstrict-prototypes
  -Wmissing-prototypes -Wwrite-strings`); everything in Latin; latina.h
  macros and every Roman numeral 0-3999 are forbidden identifiers.
- `i8`..`i64` UNSIGNED, `s32`/`s64` signed; `chorda` NOT NUL-terminated.
- Files only through `filum_*` (examen code 96). POSIX `.c` files
  include `postulata_posix.h` FIRST.
- No logging; reasons are returned chordae: "certificatum reiectum:
  <ratio>" with chain position. No secret anywhere (certificates are
  public).
- Trust store = known distribution paths only (spec II.2); NO
  `SSL_CERT_FILE`, no other override; `ancora_via` = that DER only.
- Blocklist compiled in, generated, byte-for-byte reproducible;
  freshness REPORTED, never a gate.
- Judge new/changed C files one at a time with
  `./silva/examen.sh VIA -machina` (the hook's form). Unknown system
  names -> `silva/fontes/systema_posix.h` + `officina/auspex_posix.sh`
  + glossary `ignotum-permissum` (precedent cf2e80be).
- Linux runs by hand: `./tools/linux/probatio.sh -musl|-glibc <suites>`.
- Commit via `silva.commissio(msg, viae)` from a detached python
  script; never install from the worktree; main is fast-forwarded only
  on Fran's word.

## Rulings made while planning (facts checked 2026-10-22)

- **OneCRL format verified** (changeset
  `firefox.settings.services.mozilla.com/v1/buckets/security-state/collections/onecrl/changeset?_expected=0`,
  826 KB, timestamp 1792622041776): 1907 records - 1898
  `issuerName`+`serialNumber`, 9 `subject`+`pubKeyHash`; 645 have
  `enabled: false` (skipped). All values base64: `issuerName` /
  `subject` = DER Name, `serialNumber` = the INTEGER's content octets
  (leading 0x00 kept), `pubKeyHash` = SHA-256 of the DER
  SubjectPublicKeyInfo. Spec AUDIENDA item 1 answered.
- **The eMudhra TLS intermediates are already in OneCRL** (bug
  2083382): six issuer+serial records 2026-10-17 under "emSign Root
  TLS CA - G3", and six subject+pubKeyHash records 2026-10-21 ("EM
  DV/OV/EV TLS CA - G3B/G3C") - the latter catch the Sectigo
  cross-signs. The two code-signing CAs are not (OneCRL is TLS-only;
  irrelevant to TLS). So the hand list is born EMPTY (mechanism
  tested with a fixture list); the built-in table's eMudhra coverage
  comes from the snapshot. Spec AUDIENDA item 2 answered.
- **Entry kinds follow Mozilla's semantics exactly:** `EMITTENS_SERIES`
  (issuer DER + serial octets) and `SUBIECTUM_CLAVIS` (subject DER AND
  SPKI SHA-256 must both match). The spec's "key only" kind is not
  needed: a cross-sign keeps subject and key, so subject+key catches
  it. Hand list uses the same two kinds.
- **BearSSL falls back to the subject CN when a leaf has no SAN**
  (`vendor/bearssl_amalgama.h:8955-8958`). Policy is SAN only, so
  fiducia_posix refuses a leaf without a SAN extension itself
  ("nomen hospitis: SAN deest"). Spec AUDIENDA item 3 answered.
- **A portable DER reader is its own unit** (`fiducia_der`): the
  blocklist needs issuer/serial/subject/SPKI, which BearSSL's decoder
  does not expose, and the IP SAN check needs the extensions. One
  reader, tested directly, used by both.
- **Pinned snapshot committed raw** at
  `probationes/fixa/fiducia/onecrl/2026-10-21/changeset.json` +
  `SHA256SUMS` (provenance over size; public data). Fetched again in
  T2 with the same URL and checked against what this ruling recorded.

## Rulings made while executing

- **T1: `geminus` initially had its own subject (CN=geminus)** - fixed
  in T2 to CN=localhost (same subject AND key, new serial), the true
  cross-sign stand-in for Mozilla's subject+key entries.
- **T1: probatio_fiducia had never compiled under gcc**
  (-Wformat-overflow on 512-byte path buffers) - widened in T1.
- **T2: real eMudhra certificates added as fixtures** (crt.sh, public
  CA certificates; NOTICE) - the only way to test that
  `fiducia_iudicare` itself consults the built-in table; replaces the
  plan's T3 `diffisa_vera` idea.
- **T2: generator rule tests are `-fumus`, run by hand** (pecunia
  precedent), not a credo section - the generator is a build-time
  tool; the generata gate guards its output.
- **T2: no `lib/fiducia.c`** - silex resolves a header X.h to
  `lib/X.c` BEFORE `X_macos.m` (probatio_silex "novum -vitrea" went red
  at the first commit attempt: the vitrea project lost
  fiducia_macos.m). So ONE implementation per header:
  `fiducia_diffisa.h` -> `lib/fiducia_diffisa.c` (the check),
  `fiducia_diffisa_tabula.h` -> `lib/fiducia_diffisa_tabula.c`
  (generated), `fiducia.h` -> the platform files. Stale objects of the
  old names deleted from build/ (99) before the commit.

- **T3: no bundle mtime -> content-keyed cache.** filum_status lacks
  mtime; the cache key is path + size + SHA-256 (re-read and hashed per
  call, ~2 ms warm vs ~28 ms full parse, measured on both libcs).
- **T3: the bundle diagnostic is `fiducia_fasciculum_describere(via,
  &ancorae, &omissae, &via_usa, &causa, piscina)`** (plan named it
  without `causa`); macOS returns FALSUM "solum POSIX".
- **T3: `fiducia_nomen_commune` added to fiducia_der** for the leaf
  summary's CN (needed a DN walk; one DER reader).
- **T3: TLS test suites made Linux-capable** (buffers, openssl path,
  SecureTransport leg macOS-only) - outside the plan's file list, needed
  for its "probatio_tls_servus under Linux" step.

- **T4: the live client is `tools/fiducia_vivum.c`** with +/-/~
  expectations per host and exit code; R2 is checked by horreum (real
  account host), cloudflare.com stands in for its chain in the public
  list, the bare r2 host is reported only (its certificate expired
  2026-09-09). Live checks stay by hand (network), not gates.

## Review focus

1. **Serial encoding edge cases** - serials with a leading 0x00 (high
   bit set), 20-octet serials, and a one-octet serial: OneCRL's base64
   octets must equal the certificate's INTEGER content octets exactly,
   or a blocked certificate passes. -> T1 `partes` (fixture certs with
   chosen serials) and T2 `diffisa_series`.
2. **A chain where the blocked certificate is NOT the leaf** (blocked
   intermediate, clean leaf; blocked root sent by the server; blocked
   root only in the bundle). -> T2 `diffisa_media`, T3 `ancora_diffisa`
   (anchor dropped at load).
3. **IP literal hosts** - IPv4 and IPv6 literal, a leaf with only DNS
   SANs, a leaf with no SAN but CN = the IP. -> T1 `san_ip`, T3
   `hospes_ip`.
4. **Bundle file oddities** - CRLF line endings, a non-certificate PEM
   block (e.g. `TRUSTED CERTIFICATE`), one corrupt base64 block in the
   middle: skipped and counted, the rest still trusted. -> T3
   `fasciculus_mixtus` (via the internal path seam, see T3).
5. **Truncated / hostile DER** - lengths running past the buffer,
   indefinite lengths, a SAN extension that is not a SEQUENCE: refused
   with a named reason, never read out of bounds. -> T1 `der_malum`.

---

### Task 1: `fiducia_der` - certificate parts and SAN addresses

**Files:**
- Create: `include/fiducia_der.h`, `lib/fiducia_der.c`,
  `lib/fiducia_der.worklog.md`, `probationes/probatio_fiducia_der.c`
- Modify: `probationes/fixa/tls/generare.sh` (new fixtures),
  `compile_tests_fontes_generata.sh` (regenerated)

**Interfaces - Produces:**
```c
/* fiducia_der.h - partes certificati X.509 ex DER (pura, sine
 * allocatione: sectiones in octetos datos). Solum quae fiducia
 * poscit: emittens, series, subiectum, SPKI, nomina alterna. */
/* <aedilis corpus="lib/fiducia_der.c"/> */
nomen structura {
    constans i8* octeti;
            i32  mensura;
} FiduciaSectio;                    /* sectio intra DER datum */

nomen structura {
    FiduciaSectio  emittens;        /* Name DER (cum tag et longitudine) */
    FiduciaSectio  series;          /* INTEGER contentum (0x00 servatum) */
    FiduciaSectio  subiectum;       /* Name DER */
    FiduciaSectio  clavis_publica;  /* SubjectPublicKeyInfo DER */
    FiduciaSectio  extensiones;     /* [3] contentum; mensura 0 si nullae */
              b32  habet_san;       /* extensio 2.5.29.17 adest */
} FiduciaPartes;

/* VERUM si DER certificatum bene formatum (longitudines intra
 * fines, tag expectati); FALSUM + *causa nominata aliter */
b32 fiducia_partes_legere (constans i8* der, i32 mensura,
    FiduciaPartes* partes, constans character** causa);

/* iPAddress SAN [7]: VERUM si octeti (IV aut XVI) inter nomina
 * alterna folii sunt */
b32 fiducia_san_ip_continet (constans FiduciaPartes* partes,
    constans i8* ip, i32 mensura);
```

- [ ] **Step 1: fixtures.** Extend `generare.sh`: default `OSSL` to
  `command -v openssl` when `/opt/homebrew/bin/openssl` is absent (for
  Linux images); add `serie_alta` (serial `0x80 01`, so DER keeps a
  leading 0x00), `serie_longa` (20-octet serial), `ip_solum`
  (`IP:127.0.0.1,IP:::1`, no DNS), `sine_san` (CN=localhost, no SAN
  extension), `geminus` (a SECOND certificate re-using
  `localhost.key` - the cross-sign stand-in), `sha1` (leaf signed with
  `-md sha1`), `rsa_parva` (RSA-1024 leaf). Each also as `.der`.
- [ ] **Step 2: failing tests** `probatio_fiducia_der.c` (credo v2,
  suite setup runs `generare.sh` into `credo_area` like
  `probatio_fiducia.c`):
  - `partes`: localhost.der parses; `subiectum` contains "localhost";
    `series` of `serie_alta` == `00 80 01`, of `serie_longa` has 20
    octets; `clavis_publica` of `localhost` and `geminus` are
    byte-equal while their serials differ.
  - `san_ip`: localhost contains 127.0.0.1 (IV octets); `ip_solum`
    contains `::1` (XVI octets); alienus contains no IP;
    `habet_san` false for `sine_san`.
  - `der_malum`: truncated DER (every prefix length of localhost.der
    from 0 to mensura-1) -> FALSUM with a cause, never a crash; a
    length byte claiming past the end -> FALSUM; indefinite length
    (0x80) -> FALSUM.
  Run: `./compile_tests.sh probatio_fiducia_der.c`. Expected: fails
  to link (`fiducia_partes_legere` undefined).
- [ ] **Step 3: implement** `lib/fiducia_der.c`: a TLV reader
  (`_tlv(octeti, mensura, &tag, &contentum, &reliquum)`: definite
  short/long form up to IV length octets, every length checked
  against the remaining buffer), then the fixed walk Certificate ->
  tbsCertificate -> [0] version? -> serial -> sigAlg -> issuer ->
  validity -> subject -> SPKI -> [1]/[2]? -> [3] extensions; SAN =
  extension whose OID is `55 1D 11`, value OCTET STRING containing a
  SEQUENCE of GeneralName; `[7]` primitive (tag 0x87) = IP octets.
- [ ] **Step 4: green** - the suite passes on macOS; also
  `./tools/linux/probatio.sh -musl fiducia_der` and `-glibc` (pure
  code; the images need `openssl` for fixtures - Dockerfiles gain
  `openssl` (both) in this task: `apk add ... openssl`,
  `apt-get install ... openssl`; rebuild with the command in each
  Dockerfile's header).
- [ ] **Step 5: plant** - accept indefinite length -> `der_malum`
  red; restore -> green.
- [ ] **Step 6: worklog + commit** (`fiducia_der: partes certificati
  et nomina alterna IP ex DER (fiducia-linux T1)`); viae include the
  Dockerfiles and the regenerated sources list.

### Task 2: the house blocklist (portable, both platforms)

**Files:**
- Create: `include/fiducia_diffisa.h`, `lib/fiducia.c`,
  `tools/fiducia_diffisa_generare.c`,
  `tools/fiducia_diffisa_generare.sh`, `lib/fiducia_diffisa.c`
  (GENERATED), `probationes/fixa/fiducia/onecrl/2026-10-21/changeset.json`,
  `.../SHA256SUMS`, `probationes/fixa/fiducia/diffisa.stml` (empty
  list + format comment), `probationes/fixa/fiducia/diffisa_probatio.stml`
  (fixture hand list), `probationes/probatio_fiducia_diffisa.c`,
  `lib/fiducia.worklog.md`
- Modify: `include/fiducia.h` (`<aedilis corpus="lib/fiducia.c"/>`,
  comment), `lib/fiducia_macos.m` (blocklist first),
  `lib/fiducia_posix.c` (blocklist first, still refusing otherwise),
  `aedificatio.stml` (generator actio, modelled on `pecunia_monetae`),
  `compile_tests_fontes_generata.sh`

**Interfaces - Consumes:** `fiducia_partes_legere` (T1),
`sigillum_computare` (SHA-256), `json_*`, `stml` reader.
**Produces:**
```c
/* fiducia_diffisa.h - indicium diffisarum (OneCRL + manu) */
/* <aedilis corpus="lib/fiducia.c"/> */
nomen enumeratio {
    FIDUCIA_DIFFISA_EMITTENS_SERIES = I,   /* emittens DER + series */
    FIDUCIA_DIFFISA_SUBIECTUM_CLAVIS       /* subiectum DER + SHA-256 SPKI */
} FiduciaDiffisaGenus;

nomen structura {
     FiduciaDiffisaGenus  genus;
            constans i8* primum;      /* emittens aut subiectum (DER) */
                    i32  primum_mensura;
            constans i8* secundum;    /* series aut SHA-256 (XXXII) */
                    i32  secundum_mensura;
     constans character* causa;       /* "bug 2083382 (OneCRL)" */
} FiduciaDiffisa;

extern constans FiduciaDiffisa  fiducia_diffisae[];
extern constans i32             fiducia_diffisae_numerus;
extern constans character*      fiducia_diffisae_tempus;  /* "2026-10-21" */

/* index primae congruentis in tabula, aut -1; *positio = locus in
 * catena. Catena mala (non DER) -> -1, iudicium ulterius id nominat */
s32 fiducia_diffisam_invenire (constans FiduciaCertificatum* catena,
    i32 numerus, constans FiduciaDiffisa* tabula, i32 tabula_numerus,
    i32* positio);

/* VERUM si nulla diffisa; FALSUM + *causa "certificatum reiectum:
 * clavis diffisa (catena [k]): <causa>". tabula NIHIL = ordinaria
 * (fiducia_diffisae) - probationes tabulam suam dant */
b32 fiducia_diffisas_iudicare (constans FiduciaCertificatum* catena,
    i32 numerus, constans FiduciaDiffisa* tabula, i32 tabula_numerus,
    chorda* causa, Piscina* piscina);
```

- [ ] **Step 1: pin the snapshot.** Fetch the changeset URL above into
  `probationes/fixa/fiducia/onecrl/2026-10-21/changeset.json`, write
  `SHA256SUMS`, and record the record counts (1907 / enabled 1262) in
  the worklog. If the fetched file differs from the ruling's
  timestamp, date the directory by the new timestamp instead and say
  so.
- [ ] **Step 2: failing tests** `probatio_fiducia_diffisa.c` (fixtures
  from T1's generare.sh):
  - `subiectum_clavis`: a test table with one SUBIECTUM_CLAVIS entry
    built from `localhost` (subject DER + SHA-256 of its SPKI) blocks
    `localhost` AND `geminus` (same key, other serial) at position 0;
    `alienus` passes.
  - `emittens_series`: an entry from `serie_alta`'s issuer + serial
    (`00 80 01`) blocks `serie_alta` only; an entry with the serial
    WITHOUT the leading 0x00 blocks nothing (exact octets).
  - `diffisa_media`: chain [alienus, localhost] with the localhost
    entry -> found at position 1.
  - `causa`: `fiducia_diffisas_iudicare(..., NIHIL, 0, ...)` (built-in
    table) on a clean chain -> VERUM; with the localhost test table on
    [alienus, localhost] -> FALSUM, cause contains "clavis diffisa",
    "catena [1]" and the entry's causa text.
  - `tabula_vera`: the built-in table has >= 1200 entries; contains
    the six "EM ... TLS CA - G3B/G3C" SUBIECTUM_CLAVIS entries (match
    by the subject DER bytes taken from the snapshot in the test
    itself through json.h, not retyped); `fiducia_diffisae_tempus` ==
    "2026-10-21".
  - `generator_manu`: the generator run on a fixture hand list
    (`diffisa_probatio.stml`, two entries) + an empty snapshot produces
    exactly those two entries, sorted.
  Run: `./compile_tests.sh probatio_fiducia_diffisa.c`. Expected:
  link failure.
- [ ] **Step 3: generator.** `tools/fiducia_diffisa_generare.c` reads
  the snapshot (json.h: `changes[]`, skip `enabled == false`, base64
  decode the four fields) and the hand list (stml: `<diffisa
  genus="emittens_series|subiectum_clavis" primum="base64"
  secundum="base64" causa="..." dies="YYYY-MM-DD" fons="URL"/>`),
  verifies `SHA256SUMS`, sorts by (genus, primum, secundum), and
  writes `lib/fiducia_diffisa.c`: line 1 `GENERATUM`, NAMED static
  byte arrays (uuidtext memory: big tables in named arrays), the
  `fiducia_diffisae[]` table, count, snapshot date. Causa per entry:
  "bug <n> (OneCRL)" from `details.bug`, or the hand entry's text.
  The `.sh` wrapper builds it with aedilis and runs it; actio in
  `aedificatio.stml` declares every input (snapshot, SHA256SUMS,
  hand list, generator sources, header) and the output.
- [ ] **Step 4: lib/fiducia.c** - `fiducia_diffisam_invenire`: for each
  chain certificate, `fiducia_partes_legere`; compare issuer+serial
  or subject+SHA-256(SPKI) byte-wise against the table (linear scan:
  ~1262 entries x chain length, microseconds; sort makes a later
  binary search possible, not needed now). `fiducia_diffisas_iudicare`
  wraps it (tabula NIHIL -> the built-in table) and formats the
  cause.
- [ ] **Step 5: callers.** `fiducia_macos.m` and `fiducia_posix.c`
  call `fiducia_diffisas_iudicare` FIRST and return its refusal.
- [ ] **Step 6: green** on macOS (`probatio_fiducia_diffisa`,
  `probatio_fiducia`, `probatio_tls_servus`) and Linux musl + glibc
  (`fiducia_diffisa`, `fiducia_der`); generata gate
  (`tools/generata_iudicare.sh`) passes; regenerating twice gives the
  same bytes.
- [ ] **Step 7: plants** - generator not skipping disabled records ->
  `tabula_vera` count check red (assert also `< 1300`); comparison of
  serial ignoring the leading 0x00 -> `emittens_series` red.
- [ ] **Step 8: worklog + commit** (`fiducia: indicium diffisarum
  domus - OneCRL fixum et manu, in ambabus platformis
  (fiducia-linux T2)`).

### Task 3: `fiducia_posix.c` - the Linux verifier

**Files:**
- Modify: `lib/fiducia_posix.c` (rewritten), `include/fiducia.h`
  (comment: Linux real), `tools/linux/Dockerfile.musl`
  (`ca-certificates`), `probationes/probatio_fiducia.c` (new
  sections), `lib/fiducia_posix.worklog.md` (create)
- Possibly: `silva/fontes/systema_posix.h`, `officina/auspex_posix.sh`,
  `oratio/glossarium.stml` (lexicon gaps, cf2e80be precedent)

**Interfaces - Consumes:** T1 partes/SAN, T2 blocklist, BearSSL
`br_pem_decoder_*`, `br_x509_decoder_*`, `br_x509_minimal_*`,
`br_sha256/384/512_vtable`, `br_rsa_i31_pkcs1_vrfy`,
`br_ecdsa_i31_vrfy_asn1`, `br_ec_all_m31`. **Produces:** the unchanged
`fiducia_iudicare` / `fiducia_folium_describere`, plus ONE diagnostic
in `fiducia.h` (prototype required by -Wmissing-prototypes):
`b32 fiducia_fasciculum_describere(constans character* via, i32*
ancorae, i32* omissae, chorda* via_usa, Piscina* piscina)` - via NIHIL
= the system bundle (path found, anchors loaded, blocks skipped);
via given = that file into a scratch store (tests, Review focus 4).
macOS implements it as FALSUM with cause "solum POSIX" (SecTrust has
no bundle file). It also serves T4's freshness/diagnostic report
together with `fiducia_diffisae_tempus`.

- [ ] **Step 1: failing tests** (new sections in `probatio_fiducia.c`,
  both platforms unless noted):
  - `sha1`, `rsa_parva`: refused with the test CA as anchor.
  - `sine_san`: CN=localhost, no SAN -> refused for host "localhost".
  - `hospes_ip`: `ip_solum` accepted for "127.0.0.1" and "::1";
    `alienus` (DNS only) refused for "127.0.0.1".
  - `ancora_diffisa` (Linux): a bundle file (scratch) containing the
    test CA plus a blocked anchor -> the blocked one is dropped at
    load (`omissae` counts it), via `fiducia_fasciculum_describere`.
  - `fasciculus_mixtus` (Linux): CRLF bundle, a `TRUSTED CERTIFICATE`
    block, one corrupt block -> `ancorae` = the good ones,
    `omissae` = 2 (same diagnostic).
  - existing `sine_ancora` on Linux now really consults the system
    bundle (Ubuntu/Alpine) and still refuses the test-CA leaf.
  Run on Linux: `./tools/linux/probatio.sh -musl fiducia`. Expected:
  every section red (placeholder refuses all, and the seam is
  undefined -> link failure first).
- [ ] **Step 2: implement.** Bundle discovery over the spec's path
  list (`filum_existit` + regular file); PEM decode
  (`br_pem_decoder`, only `CERTIFICATE` blocks), each through
  `br_x509_decoder` into `br_x509_trust_anchor` (DN + key copied into
  the store's piscina; CA flag from the decoder); blocked anchors
  dropped (`fiducia_diffisam_invenire` on the single DER); cache keyed
  by path + size + mtime (`filum_status`). `ancora_via` -> a
  one-anchor store, bundle untouched. Judging: T2 blocklist ->
  `br_x509_minimal_init` with SHA-256/384/512 only, `set_rsa`
  (i31), `set_ecdsa` (all curves m31), `set_minrsa(CCLVI)` ->
  start_chain(host or NIHIL for an IP literal) -> each cert
  start/append/end -> end_chain; non-zero -> mapped Latin reason with
  chain position; then SAN-only rule (`habet_san`) and, for IP hosts,
  `fiducia_san_ip_continet` (inet_pton -> IV/XVI octets).
  `fiducia_folium_describere` via `br_x509_decoder` (days since year 0
  - 719528 -> Unix days) and the CN via a name element (`03 55 04 03`)
  on `br_x509_minimal`'s DN extraction.
- [ ] **Step 3: green** on Linux musl + glibc: `fiducia`,
  `fiducia_der`, `fiducia_diffisa`, `tls_servus`; macOS unchanged
  green (`probatio_fiducia`, `probatio_tls_servus`). Examen per file
  (hook form). Measure the bundle parse time on both images
  (worklog; spec AUDIENDA "parse cost").
- [ ] **Step 4: plants** - register SHA-1 -> `sha1` red; drop the
  SAN-only rule -> `sine_san` red; skip the IP SAN check -> `hospes_ip`
  red; keep blocked anchors -> `ancora_diffisa` red. Restore each.
- [ ] **Step 5: worklog + commit** (`fiducia_posix: iudicium catenae
  sub Linux - fasciculus distributionis, BearSSL, SAN solum, IP
  (fiducia-linux T3)`).

### Task 4: live checks from Linux, horreum on Linux, docs

**Files:**
- Modify: `project-specs/fiducia-linux-spec.md` (status IMPLETUM,
  AUDIENDA answers), `lib/fiducia_posix.worklog.md`, memory topic
- Create: `tools/fiducia_vivum.c` (built with `bin/aedilis`) and
  `tools/linux/fiducia_vivum.sh` (by-hand live check, not a gate)

- [ ] **Step 1:** `tools/fiducia_vivum.c`: for each `host[:port]`
  argument, `tls_connectere` (default options: system trust) and print
  one line `host<TAB>bene|REIECTUM<TAB>tls_error_descriptio`; plus
  `fiducia_fasciculum_describere(NIHIL, ...)` and
  `fiducia_diffisae_tempus` as a header line. `tools/linux/fiducia_vivum.sh`
  compiles it inside each image (like probatio.sh) and runs it, with
  network, against: DO
  (`nyc3.digitaloceanspaces.com:443`), R2
  (`<account>.r2.cloudflarestorage.com` - account id passed as an
  ARGUMENT by Fran, never committed), `expired.badssl.com`,
  `wrong.host.badssl.com`, `untrusted-root.badssl.com`,
  `sha1-intermediate.badssl.com`. Expected: first two accepted, the
  four badssl hosts refused with the matching reason.
- [ ] **Step 2:** horreum from Linux, read-only: build `bin/horreum`
  inside the glibc image and run `inspicere` of a missing key against
  DO and R2 with Fran's profiles mounted READ-ONLY (Fran runs the
  `docker run` line - the profiles never leave Fran's machine
  otherwise). Expected: 404 "NoSuchKey"-class answers, i.e. TLS and
  signing both worked.
- [ ] **Step 3:** spec status IMPLETUM + AUDIENDA answered (paths on
  the droplet's distribution checked by Fran's `cat /etc/os-release`
  on the droplet or from canalis notes); ledger desideratum
  ...GCMMBX impletum; park for CT (spec IV) filed; memory updated.
- [ ] **Step 4: commit** (`fiducia-linux: vivum sub Linux probatum,
  specificatio impleta (fiducia-linux T4)`).

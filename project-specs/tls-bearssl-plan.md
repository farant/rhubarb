# tls bearssl plan - BearSSL's TLS 1.2 engine behind tls.h (step 0 of …BWQS8H)

> **For agentic workers:** execution mode is fixed by Fran's standing rule:
> INLINE, one task per turn, Fran approves each task before the next; no
> subagents. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** the unchanged `tls.h` API runs on BearSSL's TLS 1.2 client engine
by default, with SecureTransport still selectable at runtime
(`TlsOptiones.vectura`, `RHUBARB_TLS=securetransport`), trust decided by the
platform (`fiducia`), and every offline TLS test passing against BOTH
backends.

**Architecture:** BearSSL is vendored byte-identical at a pinned commit
under `vendor/bearssl/` and turned into ONE generated amalgamation pair
`vendor/bearssl.c` + `vendor/bearssl.h` (the `vendor/sqlite3.c` precedent),
so every existing build path takes it unchanged. `lib/tls.c` becomes a
dispatcher over two internal backends: `lib/tls_securetransport_macos.m`
(today's `tls_macos.m`, renamed, same behaviour) and `lib/tls_bearssl.c`
(engine pump + an `br_x509_class` validator that hands the chain to
`fiducia`). `lib/fiducia_macos.m` holds the SecTrust code both backends use.

**Tech stack:** C89 + latina.h house code; BearSSL master 7bea48e (C99,
compiled relaxed as vendor); Security.framework (SecTrust) for trust;
openssl `s_server` as the offline oracle; credo v2 suites.

**Spec:** `project-specs/tls-bearssl-spec.md` (quarta b0dd8b59). Read it
with this plan; this plan argues from it and records where it departs.

## Global constraints

- House code: C89 via latina.h, compiled with `-std=c89 -pedantic -Wall
  -Wextra -Werror -Wconversion -Wsign-conversion -Wcast-qual
  -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings`
  (`tools/vexilla.sh`). Identifiers, comments, messages in Latin. Never a
  latina.h macro as an identifier (`nomen`, `per`, `casus`, `duplex`, every
  Roman numeral...). `i32`/`i64` are UNSIGNED, `s32`/`s64` signed. `chorda`
  is not NUL-terminated.
- **Nothing above `tls.h` changes** (http, http_vocatio, vates, vicus).
- **Security floor, exactly today's:** TLS 1.2 only; the six suites
  ECDHE-{ECDSA,RSA} x {AES-128-GCM-SHA256, AES-256-GCM-SHA384,
  CHACHA20-POLY1305-SHA256}; real certificate validation with system trust;
  `verificare_certificatum = FALSUM` = key decoded, trust skipped;
  `ancora_via` = that DER CA is the ONLY trust anchor.
- `vendor/bearssl/` is byte-identical to upstream `git archive 7bea48e`;
  never edited by hand. `vendor/bearssl.{c,h}` are GENERATUM (line 1),
  produced only by `tools/bearssl_amalgamare.sh`.
- `bearssl.h` is included BEFORE `latina.h` (Roman-numeral macros collide
  with vendor parameter names - the sqlite rule, `lib/scrinium.c:3-6`), and
  after `postulata_posix.h` in POSIX files.
- Network tests carry `/* <aedilis facultas="rete"/> */`; gates only link
  them. Offline TLS tests use `probationes/servus_tls.h` (openssl s_server)
  with certificates generated at test time
  (`probationes/fixa/tls/generare.sh`).
- Never read or print `~/.rhubarb/anthropic.clavis`; Fran runs live checks.
  No installs to `~/.bin` from this worktree. No bare `git stash`.
- Commits via `silva.commissio` from a detached python launcher
  (double-fork + setsid, `python3 -u`, log ends with `DETACH rc=`, Monitor
  on the log), worktree guard
  (`os.chdir(Q); sys.path.insert(0, Q+'/pythonica');
  assert silva.__file__.startswith(Q+'/')`). Before it: `./silva/formator.sh
  -scribere` on touched C, `-vitia` parity with HEAD copies, Latin lint
  `VOCABULA_VIAE_ADDITAE="..." ./oratio/vocabula.sh -nova`, and
  `bin/fabrica sanare bin/briar bin/briar-spectator` when lib/ changed.
  While gates run, the tree is frozen.
- Every task proves its tests with plants via `silva.planta(via, vetus,
  novus, 'radix', filtrum=..., tolerans='spatia')`: the plant must compile,
  the named test must go red, the revert green. A muted plant = a test gap,
  fixed before commit.
- New lib file -> `./tools/compile_tests_fontes_generare.sh`; new root test
  binaries need nothing else (fabrica composite finds them).

## Rulings made while planning (spec deviations, ledgered here)

- **R1 - amalgamation, not a directory vendor form (spec II.2).** The
  spec let the plan choose between an aedilis directory form and a
  generated `<fons>` list after reading aedilis. Reading it showed FIVE
  independent consumers of vendor sources, all built around one header +
  one source (`lib/aedilis.c:714-800` X.h->X.c; `tools/fabrica_ansae.c:
  199-266` per-`fons` flags; `compile_tests.sh:45-56` VENDOR_SOURCES;
  `ludus_tessera/compile_probationes.sh:234-244`; `lib/silex.c:520-795`
  `vendor/<basis>` closure copy). A directory form changes all five. A
  generated amalgamation changes none of them. A naive unity build fails
  (static names collide across alternative implementations: `api_mul`,
  `P256_P`, `t0_codeblock`, typedef `p256_jacobian`...). The generator
  therefore asks clang for each file's own file-scope names
  (`-ast-dump=json`) and textual `#define`s, renames names defined in
  more than one file (`#define n n__<file>` ... `#undef n`), and undefines
  each file's macros after it. **Measured 2026-10-15 on 7bea48e:** 294
  sources, 435 renames, 461 macro undefs, compiles with no warnings (502
  KB object); upstream `test_crypto all` (88 algorithm tests, 45 s) and
  `test_x509` pass against the amalgamation. Cost if wrong: one generator;
  the directory form remains possible later.
- **R2 - C89 boundary = `inline` -> `__attribute__((unused))` (spec II.3,
  mitigation 3 variant).** The generated header opens with
  `#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L` /
  `#define inline __attribute__((unused))` and undefines it at the end.
  Measured: a house file including it passes every house flag, and examen
  gives ACCIPE with zero findings, once examen knows the `<stdint.h>`
  exact-width types (R3). `__inline__` was tried first: clang accepts it,
  but silva reads it as a type name (74 findings). silva already treats
  `__attribute__` as an empty internal macro (`silva_expandere.c:2415`),
  and `unused` silences the unused-static warning. `static` without
  `inline` means the same in C. No adapter is needed (mitigation 4 not
  taken).
- **R3 - stdint exact-width types join examen's lexicon.** Added to
  `silva/fontes/systema_posix.h` (semper section, beside `intptr_t`), with
  `officina/auspex_posix.sh` size assertions, a plant, and glossary entries
  if the Latin lint flags them. 64-bit as `long`/`unsigned long` (LP64,
  the `intptr_t` precedent - no `long long` in C89).
- **R4 - the whole tracked upstream tree.** Vendored as is via
  `git archive` (466 files, 4.1 MB), including `T0Comp.exe` (upstream's
  prebuilt T0 compiler; its source `T0/` comes too; we never run it) and
  `test/` (the KAT oracle the gate runs). The root `.gitignore` `build/`
  rule swallows `vendor/bearssl/build/.do_not_remove`: it is added with
  `git add -f` so a fresh clone hashes equal.
- **R5 - `vendor/` leaves the pre-commit examen and the formatter.**
  Already excluded from the examen corpus walk (`silva/examen.sh:145`), the
  Latin lint (`tools/unci-git/lint_latinus.sh:41`, `pythonica/silva.py:
  2669`), identitates and oratio. Still judged by the pre-commit examen
  list (`tools/unci-git/pre-commit:106-107`), and formatted on changed lines
  by `tools/formare_viae.sh:45`. A new upstream file is all changed lines,
  so it would be rewritten, and 294 C99 files would be REICE'd. Vendor
  code is foreign by definition. Pinned by a new hook smoke step (like XX
  for knotapel).
- **R6 - `fiducia` has its own summary type (spec II.4).** The spec's
  `fiducia_folium_describere` returned `TlsCertificatum`. `fiducia.h`
  including `tls.h` would pull the whole TLS closure into every fiducia
  user, including its test. So `FiduciaFolium` carries the same four
  fields and the TLS backends copy them across.
- **R7 - `RHUBARB_TLS` is read per connection, not once (spec II.5).** It
  costs one `getenv` per connection, keeps no hidden state, and lets
  tests flip it with `setenv`. `tls_vecturam_eligere` is public so the
  choice is testable without a connection.
- **R8 - backend file names follow aedilis's variant rule.** aedilis
  resolves `X.h` to `lib/X_macos.m` BEFORE `lib/X.c`
  (`lib/aedilis.c:553-575`). If `lib/tls.c` and `lib/tls_macos.m` both
  existed, `tls.h` would keep resolving to SecureTransport. So
  `tls_macos.m` becomes `lib/tls_securetransport_macos.m` behind
  `include/tls_securetransport.h`, and BearSSL is `include/tls_bearssl.h` +
  `lib/tls_bearssl.c`. Each backend has its own result type with the same
  fields as `TlsResultus`. There is no vtable: two backends, one `switch`.
- **R9 - blocking mode uses the socket's own timeouts, not `poll()`
  (spec II.6).** `tcp_connectere_cum_optionibus` already arms the socket
  timeouts, and SecureTransport's blocking path relies on them (read
  callback seeing `TCP_ITERUM`, `tls_macos.m:554-562`). So the BearSSL pump
  is ONE function for both modes, and "would block" in blocking mode means
  a timeout.
- **R10 - matrix = suite run twice.** `probatio_tls_servus` and
  `probatio_tls_gradus` run their whole section table once with
  `RHUBARB_TLS=bearssl` and once with `=securetransport` (the same
  assertions), and `tools/tls_effluvia.sh` then covers both backends with
  no change. The vates/vicus offline suites never touch TLS (plain
  `servus_scriptus`), so they have no matrix to add. Their network files
  are run by hand under both values.
- **R11 - pinned commit 7bea48e** (master, 2026-04-06, "Fixed bug in
  handling incoming records with invalid length (impacted CBC enc...)",
  `git describe` v0.6-39-g7bea48e): last release v0.6 is 2018; master
  carries the later fixes (RSA PSS 2022, P-256 m64 carry 2020,
  record-length 2026).
- **R12 - an RSA fixture.** The spec lists RSA leaves as AUDIENDA.
  `generare.sh` gains `localhost_rsa` (RSA-2048 leaf from the same EC CA),
  which drives ECDHE_RSA, `rsavrfy` and the decoder's RSA path in an
  offline test.

### Rulings made while executing

- **R13 (T1) - generated pair is `vendor/bearssl_amalgama.{c,h}`, not
  `vendor/bearssl.{c,h}`.** Every later mention of `vendor/bearssl.c`,
  `vendor/bearssl.h` and `#include "bearssl.h"` in this plan means the
  amalgama names. Why: twelve silva tools collect every `.h` in the tree
  and resolve a quoted include by BASENAME, and the first one met in the
  directory walk wins (`silva_expandere.c:1210`). examen reached
  `vendor/bearssl/inc/bearssl.h` first, bypassed the C89 shim and
  REICE'd. A unique basename works in all twelve; the walk is ledger park
  …C4Q5.
- **R14 (T1) - header macros that a source redefines identically stay
  defined.** `ARSH` in `ec_c25519_m15/m31` and `ec_p256_m31` duplicates
  `inner.h:1030/1033` (legal C). The generator leaves such macros out of
  its post-file `#undef`s, and fails on a DIFFERENT redefinition or an
  `#undef` of a header macro. 455 undefs, not 461.
- **R15 (T1) - `FABRICA_SCRIPTURA`**: the generator writes under it when
  set (fabrica's scratch root), as `tools/runae_generare.sh` does.
- **R16 (T1) - plant preflight include roots.** `silva.syntaxis` (the
  `silva.planta` preflight) used a hand `-I` list without `vendor/`. It
  now appends `bin/aedilis --radices`, keeping the old order first.
- **R17 (merge 85baa08f) - examen code 96 CANALIS_PRAETERITUS**, which
  arrived from main after planning: raw `fopen`/`access`/`stat` outside
  filum is a violation. In T2-T5, `fiducia_macos.m`'s anchor read uses
  `lectiones_fopen`, `tls_bearssl.c`'s anchor check uses `filum_existit`,
  and test files use `filum_scribere`/`filum_existit`/`lectiones_fopen`
  (same shapes as the plan's code).
- **R18 (T1) - glossary / lint.** Eight `ignotum-permissum` entries
  (`int8` ... `uint64`). Test identifiers are renamed instead of
  glossaried: `_sectio_semen`, `generator`.
- **R19 (T1) - the hook gate step is XXIII**, not XXI: main added XXI and
  XXII.
- **R20 (T2) - anchor check at setup = READABLE only** (`filum_existit`,
  examen code 96). A readable non-DER anchor is rejected by fiducia at
  server-auth with its named reason, not at setup (same error category).
  Fixtures: `GENUS_CLAVIS=rsa hospes ...` selects the RSA leaf (`hospes`'
  3rd/4th args are positional and optional); `ignotus.der` too.
- **R21 (T2) - fiducia reads the anchor with `filum_legere_totum`**: no
  16 KB stack buffer, and fabrica sees the read.
- **R22 (T3) - `RHUBARB_TLS` via `lectiones_ambitus`**, not raw `getenv`:
  the house's tracked getenv, so fabrica sees the dependency. Consequence:
  every root test whose closure includes tls.c reads it, so it is declared
  `<ambitus variabilis="RHUBARB_TLS"/>` on `probationes_radicis`
  (aedificatio.stml); its value is part of each verdict's key. Without
  that, the radix gate refused: "ambitus non declaratus: RHUBARB_TLS".
- **R23 (T3) - backend type spelled `TlsSecuretransport` /
  `TlsSecuretransportResultus`** (function prefix `tls_securetransport_`
  unchanged). CamelCase `SecureTransport` splits into the real English word
  "transport", which may never be glossaried. One proper-noun word stays,
  and it and `bearssl` are `ignotum-permissum`. Every later
  `TlsSecureTransport` in this plan means `TlsSecuretransport`; T4 names its
  type `TlsBearssl`.
- **R24 (T4) - plant set revised.** The plan's "CLAUSA -> LEGERE" plant and
  its "pendens - 1" plant were mute by analysis: mid-handshake EOF never
  reaches CLAUSA, and the gradus test asserts only the response. The
  replacements: EOF treated as a clean close, `br_ssl_engine_flush`
  dropped, `recvapp_ack` dropped. The "clausa a pare" section now has the
  server READ the ClientHello before closing (clean FIN, not RST) and
  asserts the EOF branch's own text. Details in lib/tls_bearssl.worklog.md.
- **R25 (T4) - `Validator` is `Iudex`** (field `iudex`, vtable
  `IUDICIS_CLASSIS`): "validator" is a real English word (lint).
- **R26 (T6) - the floor is enforced twice in BearSSL** (versions 1.2..1.2
  plus an AEAD-only TLS 1.2 suite list), so a version-only plant is mute
  against a 1.1 server that offers CBC. The `tls11` section's proving
  plant swaps the config for `br_ssl_client_init_full` (TLS 1.0-1.2, all
  suites). The 1.1 server needs `-cipher DEFAULT:@SECLEVEL=0` (OpenSSL 3.3
  refuses 1.1 at the default level). `servus_tls.h` gains
  `servum_tls_incipere_cum(..., additamenta, ...)`, and the old entry
  point wraps it.
- **R27 (T6) - section function `_sectio_versio_vetus`** (title "tls11"):
  `tls11` is a new word to the lint.
- **R28 (T6) - spec status and ledger closed AFTER Fran's live check**,
  in a separate small commit, so that check's result is recorded with
  them.
- **R29 (merge) - examen-corpus walks `vendor/` too** (`silva/percursus.sh`
  has its own tree walk; R5 covered only the hook and the formatter).
  `fusio_probare` refused: about 300 upstream C99 files were REICE but not
  pinned. Fix: percursus skips directories directly under `vendor/`
  (upstream trees, guarded byte for byte by gate `bearssl`), while files in
  `vendor/` itself are still walked. `vendor/bearssl_amalgama.c` is pinned in
  `exclusiones.txt`. The generated header's shim line carries
  `<tolera codex="EXTENSIO_COMPILATORIS">`, so the header is ACCIPE on its own.
  Side effect: `lib/imago.c` (stb_image) is ACCIPE now thanks to R3's stdint
  lexicon, so its pin was deleted (the ratchet shrinks).
- **R30 (merge) - includers write `#include "../vendor/bearssl_amalgama.h"`**
  (the sqlite precedent, `lib/scrinium.c`), not the bare name. silex finds a
  vendored header by `vendor/` in the include text (`_vendor_basim`);
  without it, a scaffolded project copied `lib/tls_bearssl.c` but not the
  vendor pair, and gate `silex-semen` failed to build ('bearssl_amalgama.h'
  file not found). A hand run of that gate needs `bin/fabrica sanare
  bin/silex` first, because silex scaffolds from its baked-in corpus.

## Review focus

Inputs no task's tests reach that are most likely to bite, each with the
behaviour a reasonable person expects:

1. **IP-literal hosts** (`https://127.0.0.1:port`): BearSSL sends the
   literal as SNI and SecTrust matches the cert's IP SAN; the connection
   succeeds the same as with SecureTransport, not a name-mismatch error.
2. **Peer FIN without close_notify after the handshake**: `tls_recipere`
   returns 0 (EOF), the same as SecureTransport's `errSSLClosedNoNotify`
   handling. During the handshake it is a named error, never a hang.
3. **Write backpressure with large bodies** (a request bigger than the
   socket buffer on a non-blocking socket): `tls_mittere` returns
   `TLS_ITERUM` until the accepted bytes' records are on the wire, then
   their count. Nothing is lost or sent twice.
4. **Oversized chains** (>16 certificates, or one >16 KB): the handshake
   fails with the named reason "catena certificatorum nimis magna". No
   overflow, no crash.
5. **Trust evaluation stalling the reactor**: `SecTrustEvaluateWithError`
   may fetch intermediates or revocation data. It blocks inside
   `tls_gradus` for that time, exactly as SecureTransport did inside
   `SSLHandshake`. Not a regression, but the first place to look if the
   vicus UI stutters on connect.

---

### Task 1: vendor + amalgamation + build + C89 boundary + KAT (spec II.1-II.3)

**Files:**
- Create: `vendor/bearssl/` (upstream tree, `git archive 7bea48e`),
  `vendor/bearssl/VENDOR.md`
- Create: `tools/bearssl_amalgamare.py`, `tools/bearssl_amalgamare.sh`
- Create (generated): `vendor/bearssl.c`, `vendor/bearssl.h`
- Create: `tools/bearssl_probare.sh` (gate `bearssl`)
- Create: `probationes/probatio_bearssl.c`
- Modify: `aedificatio.stml` (generator action), `aedilis.stml:82-93`
  (vendor fons), `compile_tests.sh:45-47` (VENDOR_SOURCES),
  `pythonica/silva.py:1526` (PORTAE), `tools/unci-git/pre-commit:106-107`,
  `tools/formare_viae.sh:45`, `tools/unci-git/fumus.sh` (new step XXI),
  `silva/fontes/systema_posix.h` (stdint), `officina/auspex_posix.sh`
  (asserts), `silva/fontes/systema_posix.worklog.md`

**Interfaces:**
- Produces: `#include "bearssl.h"` usable from house C89 code (after
  `postulata_posix.h`, before `latina.h`); `vendor/bearssl.c` linked
  automatically into any closure that includes it; gate `bearssl`
  (`BEARSSL: sana|FRACTA`).

- [ ] **Step 1: Vendor the tree**

```bash
cd /Users/francisarant/Documents/projects/rhubarb-quarta
test -d ../bearssl/.git && git -C ../bearssl rev-parse HEAD   # expect 7bea48e5e850ab4cafbe68d3765cdaba13a86d6f
mkdir -p vendor/bearssl
git -C ../bearssl archive 7bea48e5e850ab4cafbe68d3765cdaba13a86d6f | tar -x -C vendor/bearssl
find vendor/bearssl -type f | wc -l                       # expect 466
git check-ignore vendor/bearssl/build/.do_not_remove      # prints a rule: must be force-added
```

Expected: 466 files; the check-ignore line prints `.gitignore:3:build/`.

- [ ] **Step 2: Write `vendor/bearssl/VENDOR.md`**

The checksum line is computed by the same function the gate uses
(step 9). Write the file, then fill `summa` with the output of
`tools/bearssl_probare.sh -summa`:

```markdown
# BearSSL - vendored

- Fons: https://www.bearssl.org/git/BearSSL (clone ../bearssl)
- Commissio: 7bea48e5e850ab4cafbe68d3765cdaba13a86d6f (master, 2026-04-06,
  v0.6-39-g7bea48e) - "Fixed bug in handling incoming records with invalid
  length"
- Licentia: MIT (LICENSE.txt), Thomas Pornin
- Arbor: `git archive` totus, octetim idem, numquam manu mutatus.
  Renovatio = directorium totum substituere, haec plagula renovanda,
  `tools/bearssl_amalgamare.sh`, porta `bearssl`.
- Amalgama: vendor/bearssl.c + vendor/bearssl.h (GENERATUM,
  tools/bearssl_amalgamare.py); compilata laxe (aedilis.stml <vendor>,
  compile_tests.sh VENDOR_FLAGS).
- Summa: SHA-256 super lineas `shasum -a 256` omnium fasciculorum
  (praeter hanc), ordine LC_ALL=C.

summa: <tools/bearssl_probare.sh -summa>
```

- [ ] **Step 3: Write the generator `tools/bearssl_amalgamare.py`**

```python
#!/usr/bin/env python3
"""bearssl_amalgamare.py - vendor/bearssl.c + vendor/bearssl.h GENERATA
ex arbore upstream vendor/bearssl/ (tls-bearssl-plan T1, regula R1/R2).

CUR AMALGAMA (ut vendor/sqlite3.c): viae aedificationis domus (aedilis
X.h -> X.c, fabrica, compile_tests.sh VENDOR_SOURCES, ludus_tessera,
silex vendor/<basis>) par UNUM caput + UNUM fontem tractant.

COLLISIONES: fontes upstream nomina gradus plagulae (static, typedef,
tag, enumeratio) inter se iterant et macra propria definiunt. ORACULUM
clang: -ast-dump=json per fontem -> nomina in fonte IPSO definita;
'#define' textualia fontis -> macra eius. Nomen in pluribus fontibus ->
'#define n n__<fons>' ante fontem, '#undef' post; macra fontis post eum
'#undef'. Probatio: tools/bearssl_probare.sh (KAT upstream).

C89 (caput): 'static inline' -> sub C89 'inline' fit
'__attribute__((unused))' (silva __attribute__ ut vacuum legit; 'unused'
monitum functionis non adhibitae tacet). Fons ipse C99: nihil mutatur.

Usus: tools/bearssl_amalgamare.py [radix]
"""
import concurrent.futures
import json
import os
import re
import subprocess
import sys

RADIX = os.path.abspath(sys.argv[1] if len(sys.argv) > 1 else '.')
ARBOR = os.path.join(RADIX, 'vendor', 'bearssl')
FONS_EXITUS = os.path.join(RADIX, 'vendor', 'bearssl.c')
CAPUT_EXITUS = os.path.join(RADIX, 'vendor', 'bearssl.h')
CLANG = ['clang', '-std=c99', '-I' + os.path.join(ARBOR, 'inc'),
         '-I' + os.path.join(ARBOR, 'src')]
DEFINE = re.compile(r'^[ \t]*#[ \t]*define[ \t]+([A-Za-z_][A-Za-z_0-9]*)',
                    re.M)
INCLUDE_INTERIUS = re.compile(r'^[ \t]*#[ \t]*include[ \t]+"inner\.h"'
                              r'[^\n]*\n', re.M)
INCLUDE_CITATUM = re.compile(r'^[ \t]*#[ \t]*include[ \t]+"([^"]+)"', re.M)
INCLUDE_CAPITIS = re.compile(r'^#include "(bearssl[a-z0-9_]*\.h)"\n',
                             re.M)
PRAEFATIO = ('/* {} - GENERATUM a tools/bearssl_amalgamare.py ex '
             'vendor/bearssl (BearSSL 7bea48e, MIT) - NE MANU EDITES */\n')


def fracta(nuntius):
    sys.stderr.write('bearssl_amalgamare: FRACTA: ' + nuntius + '\n')
    sys.exit(1)


def legere(via):
    with open(via, encoding='utf-8') as f:
        return f.read()


def nomina_interiora(via):
    """nomina gradus plagulae in fonte IPSO definita (clang)."""
    r = subprocess.run(CLANG + ['-fsyntax-only', '-Xclang',
                                '-ast-dump=json', via],
                       capture_output=True, text=True)
    if r.returncode != 0:
        fracta('clang ' + via + ': ' + r.stderr[:400])
    radix = json.loads(r.stdout)
    vera = os.path.realpath(via)
    currens = None
    nomina = set()
    for d in radix.get('inner', []):
        loc = d.get('loc', {})
        # ordo: loc, spellingLoc, expansionLoc - expansio vincit
        for k in (loc, loc.get('spellingLoc', {}),
                  loc.get('expansionLoc', {})):
            if 'file' in k:
                currens = k['file']
        inclusum = ('includedFrom' in loc
                    or 'includedFrom' in loc.get('expansionLoc', {}))
        if inclusum or currens is None \
                or os.path.realpath(currens) != vera:
            continue
        genus = d.get('kind')
        if genus in ('FunctionDecl', 'VarDecl') \
                and d.get('storageClass') == 'static':
            nomina.add(d['name'])
        if genus in ('TypedefDecl', 'RecordDecl', 'EnumDecl') \
                and d.get('name'):
            nomina.add(d['name'])
        if genus == 'EnumDecl':
            for e in d.get('inner', []):
                if e.get('kind') == 'EnumConstantDecl':
                    nomina.add(e['name'])
    return nomina


def caput_planum():
    """inc/bearssl.h cum capitibus suis inlineatis (semel quodque)."""
    visa = set()

    def explicare(titulus):
        if titulus in visa:
            return ''
        visa.add(titulus)
        return INCLUDE_CAPITIS.sub(
            lambda m: explicare(m.group(1)),
            legere(os.path.join(ARBOR, 'inc', titulus)))
    textus = explicare('bearssl.h')
    if '#include "' in textus:
        fracta('caput: include citatum residuum')
    return textus, visa


def main():
    fontes = sorted(
        os.path.relpath(os.path.join(d, f), os.path.join(ARBOR, 'src'))
        for d, _, fs in os.walk(os.path.join(ARBOR, 'src'))
        for f in fs if f.endswith('.c'))
    if not fontes:
        fracta('vendor/bearssl/src vacuum')
    viae = [os.path.join(ARBOR, 'src', s) for s in fontes]
    with concurrent.futures.ThreadPoolExecutor(os.cpu_count() or 4) as ex:
        nomina = dict(zip(fontes, ex.map(nomina_interiora, viae)))
    textus = {s: legere(v) for s, v in zip(fontes, viae)}
    macra = {s: set(DEFINE.findall(textus[s])) for s in fontes}

    caput, capita = caput_planum()
    interius = legere(os.path.join(ARBOR, 'src', 'inner.h'))
    configuratio = legere(os.path.join(ARBOR, 'src', 'config.h'))
    macra_capitum = set(DEFINE.findall(caput)) \
        | set(DEFINE.findall(interius)) | set(DEFINE.findall(configuratio))
    for s in fontes:
        for m in INCLUDE_CITATUM.findall(textus[s]):
            if m != 'inner.h':
                fracta(s + ': include citatum ignotum "' + m + '"')
        communia = macra[s] & macra_capitum
        if communia:
            fracta(s + ': macra capitum redefinit: '
                   + ' '.join(sorted(communia)))

    numerus = {}
    for s in fontes:
        for n in nomina[s] | macra[s]:
            numerus[n] = numerus.get(n, 0) + 1

    # caput: C89 limen + capita planata
    with open(CAPUT_EXITUS, 'w', encoding='utf-8', newline='\n') as o:
        o.write(PRAEFATIO.format('bearssl.h'))
        o.write('/* capita: ' + ' '.join(sorted(capita)) + ' */\n')
        o.write('#if !defined(__STDC_VERSION__) '
                '|| __STDC_VERSION__ < 199901L\n'
                '#define inline __attribute__((unused))\n'
                '#define BEARSSL_INLINE_C89\n#endif\n')
        o.write(caput)
        o.write('#ifdef BEARSSL_INLINE_C89\n#undef inline\n'
                '#undef BEARSSL_INLINE_C89\n#endif\n')

    # fons: inner.h (config.h inlineatum, bearssl.h = caput generatum
    # eodem directorio) + fontes renominati
    interius = re.sub(r'^#include "config\.h"\n',
                      lambda m: configuratio, interius, flags=re.M)
    renominata = 0
    with open(FONS_EXITUS, 'w', encoding='utf-8', newline='\n') as o:
        o.write(PRAEFATIO.format('bearssl.c'))
        o.write('#line 1 "vendor/bearssl/src/inner.h"\n')
        o.write(interius)
        for s in fontes:
            basis = os.path.basename(s)[:-2]
            rn = sorted(n for n in nomina[s] if numerus[n] > 1)
            renominata += len(rn)
            for n in rn:
                o.write('#define %s %s__%s\n' % (n, n, basis))
            o.write('#line 1 "vendor/bearssl/src/%s"\n' % s)
            o.write(INCLUDE_INTERIUS.sub('', textus[s]))
            if not textus[s].endswith('\n'):
                o.write('\n')
            for n in rn:
                o.write('#undef %s\n' % n)
            for n in sorted(macra[s]):
                o.write('#undef %s\n' % n)
    sys.stderr.write('bearssl_amalgamare: fontes %d, renominata %d, '
                     'macra %d\n' % (len(fontes), renominata,
                                     sum(len(v) for v in macra.values())))


main()
```

And the wrapper the fabrica action runs, `tools/bearssl_amalgamare.sh`:

```bash
#!/bin/bash
# tools/bearssl_amalgamare.sh - vendor/bearssl.{c,h} GENERATA ex
# vendor/bearssl (actio fabricae 'bearssl_amalgama'; python declaratus ut
# instrumentum, ut entitates_html_generare.sh)
set -euo pipefail
cd "$(dirname "$0")/.."
exec /opt/homebrew/bin/python3 -I tools/bearssl_amalgamare.py "$PWD"
```

- [ ] **Step 4: Run the generator; prove determinism and compile**

```bash
chmod +x tools/bearssl_amalgamare.py tools/bearssl_amalgamare.sh
./tools/bearssl_amalgamare.sh
cp vendor/bearssl.c $S/bearssl.c.1; cp vendor/bearssl.h $S/bearssl.h.1
./tools/bearssl_amalgamare.sh
cmp vendor/bearssl.c $S/bearssl.c.1 && cmp vendor/bearssl.h $S/bearssl.h.1 && echo IDEM
head -1 vendor/bearssl.c vendor/bearssl.h | grep -c GENERATUM          # 2
clang -O2 -c vendor/bearssl.c -o $S/bearssl.o && echo FONS_OK
```

Expected: `fontes 294, renominata 435, macra 461` (± if clang's AST
changes), `IDEM`, `2`, `FONS_OK` with no warnings. (`$S` = scratchpad.)

- [ ] **Step 5: Write the failing KAT smoke test
  `probationes/probatio_bearssl.c`**

```c
/* probatio_bearssl.c - BearSSL vendicatum (vendor/bearssl.c amalgama)
 * nectitur et currit: SHA-256 FIPS 180-2 "abc", X25519 RFC 7748 §6.1
 * (clavis publica Alicae), entropia systematis (pactio sine ea
 * recusatur - tls-bearssl-spec II.6). KAT plenus upstream: porta
 * 'bearssl' (tools/bearssl_probare.sh). */
#include "bearssl.h"
#include "latina.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior vacuum
_hex (
    constans character* hex,
         unsigned char* exitus,
                   i32  mensura)
{
    i32 i;

    per (i = ZEPHYRUM; i < mensura; i++)
    {
        unsigned int octetus;

        (vacuum)sscanf(hex + i * II, "%2x", &octetus);
        exitus[i] = (unsigned char)octetus;
    }
}

interior vacuum
_sectio_sha256 (
    CredoContextus* c)
{
    br_sha256_context  ctx;
        unsigned char  h[XXXII];
        unsigned char  exspectatum[XXXII];

    (vacuum)c;
    _hex("ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad",
        exspectatum, XXXII);
    br_sha256_init(&ctx);
    br_sha256_update(&ctx, "abc", III);
    br_sha256_out(&ctx, h);
    CREDO_VERUM(memcmp(h, exspectatum, XXXII) == ZEPHYRUM);
}

interior vacuum
_sectio_x25519 (
    CredoContextus* c)
{
    constans br_ec_impl*  ec = br_ec_get_default();
         unsigned char  privata[XXXII];
         unsigned char  publica[XXXII];
         unsigned char  exspectata[XXXII];
                size_t  n;

    (vacuum)c;
    _hex("77076d0a7318a57d3c16c17251b26645df4c2f87ebc0992ab177fba51db92c2a",
        privata, XXXII);
    _hex("8520f0098930a754748b7ddcb43ef75a0dbf3a0d26381af4eba4a98eaa9b4e6a",
        exspectata, XXXII);
    CREDO_NECESSE_NON_NIHIL(ec);
    n = ec->mulgen(publica, privata, XXXII, BR_EC_curve25519);
    CREDO_AEQUALIS_I32((i32)n, XXXII);
    CREDO_VERUM(memcmp(publica, exspectata, XXXII) == ZEPHYRUM);
}

interior vacuum
_sectio_entropia (
    CredoContextus* c)
{
         constans character* titulus = NIHIL;
                br_prng_seeder  semen = br_prng_seeder_system(&titulus);
     br_hmac_drbg_context  drbg;

    (vacuum)c;
    CREDO_NECESSE(semen != NIHIL);
    CREDO_VERUM(titulus != NIHIL && strlen(titulus) > ZEPHYRUM);
    br_hmac_drbg_init(&drbg, &br_sha256_vtable, "probatio", VIII);
    CREDO_VERUM(semen(&drbg.vtable) != ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "sha256", _sectio_sha256, NIHIL, NIHIL, NIHIL, NIHIL },
    { "x25519", _sectio_x25519, NIHIL, NIHIL, NIHIL, NIHIL },
    { "entropia", _sectio_entropia, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("bearssl", SECTIONES);
}
```

- [ ] **Step 6: Run it - expect FAIL (vendor source not linked yet)**

Run: `./compile_tests.sh probatio_bearssl`
Expected: link error, undefined `_br_sha256_init` & co (VENDOR_SOURCES
does not have it yet), or examen REICE on the unknown stdint types.

- [ ] **Step 7: Wire the build**

`aedilis.stml`, inside `<vendor>` after the sqlite `<fons>`:

```xml
    <!-- BearSSL amalgama (tls-bearssl-plan T1, R1): GENERATUM ex
         vendor/bearssl; C99 laxe, ut sqlite -->
    <fons via="vendor/bearssl.c">
      <vexillum! (>-O2
    </fons>
```

`compile_tests.sh` VENDOR_SOURCES:

```bash
declare -a VENDOR_SOURCES=(
    "vendor/sqlite3.c"
    "vendor/bearssl.c"
)
```

`aedificatio.stml`, after `entitates_html`:

```xml
  <!-- BearSSL amalgama (tls-bearssl-plan T1, R1): oraculum clang
       (nomina interiora per -ast-dump=json), ergo identitas clang et
       python ingressus -->
  <actio titulus="bearssl_amalgama" genus="generator">
    <mandatum>
      <verbum! (>./tools/bearssl_amalgamare.sh
    </mandatum>
    <ingressus genus="fasciculus" via="tools/bearssl_amalgamare.sh"/>
    <ingressus genus="fasciculus" via="tools/bearssl_amalgamare.py"/>
    <ingressus genus="plagulae" via="vendor/bearssl/src" suffixa=".c .h"/>
    <ingressus genus="plagulae" via="vendor/bearssl/inc" suffixa=".h"/>
    <ingressus genus="instrumentum" via="/opt/homebrew/bin/python3"/>
    <ingressus genus="identitas_clang" via="clang"/>
    <exitus via="vendor/bearssl.c" provenientia="regeneratio"/>
    <exitus via="vendor/bearssl.h" provenientia="regeneratio"/>
  </actio>
```

Then: `bin/fabrica iudicare -plenus -tacta vendor/bearssl.c vendor/bearssl.h`
Expected: the new action judged, artifacts equal (no regeneration diff).

- [ ] **Step 8: examen lexicon - stdint (R3)**

First, `grep -n 'int8_t\|int16_t\|int32_t\|int64_t' silva/fontes/systema_*.h`
must print nothing (no earlier definitions). Then in
`silva/fontes/systema_posix.h`, `<sectio semper="verum"/>`, right after
`typedef long intptr_t;`:

```c
/* stdint.h - typi latitudinis exactae (C99; vendor/bearssl.h, tls-bearssl
 * T1). LP64: 64 bit ut long (praecedens intptr_t; C89 'long long' non
 * novit) */
typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long               int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long      uint64_t;
```

In `officina/auspex_posix.sh` `emittere_asserta()` (the harness includes
the real headers - add `#include <stdint.h>` there if absent):

```bash
    echo "IUNGO_ASSERTA(sizeof(int8_t) == 1 && sizeof(uint8_t) == 1);"
    echo "IUNGO_ASSERTA(sizeof(int16_t) == 2 && sizeof(uint16_t) == 2);"
    echo "IUNGO_ASSERTA(sizeof(int32_t) == 4 && sizeof(uint32_t) == 4);"
    echo "IUNGO_ASSERTA(sizeof(int64_t) == 8 && sizeof(uint64_t) == 8);"
    echo "IUNGO_ASSERTA((uint32_t)-1 > 0 && (int32_t)-1 < 0);"
```

Plant (compiles): `typedef unsigned long      uint64_t;` ->
`typedef unsigned int       uint64_t;` → `./officina/auspex_posix.sh`
DISSENTIT; revert green. Worklog entry in
`silva/fontes/systema_posix.worklog.md`. Latin lint: if `uint8`/`int16`...
are flagged, add `ignotum-permissum` glossary entries in
`oratio/glossarium.stml` (pattern: `einprogress`, line 994).

- [ ] **Step 9: Write the gate `tools/bearssl_probare.sh`**

```bash
#!/bin/bash
# tools/bearssl_probare.sh - porta 'bearssl' (tls-bearssl-plan T1):
#   I   integritas: summa arboris vendor/bearssl == VENDOR.md (editio
#       fortuita intra vendor/bearssl capitur)
#   II  KAT upstream contra AMALGAMAM: test_crypto all (LXXXVIII
#       algorithmi) + test_x509 (corpus catenarum) - amalgama renominata
#       eadem facit ac fontes separati
# Generatio ipsa (amalgama == generator hodie) = porta 'generata'.
# <porta titulus="bearssl" tegit="vendor/bearssl/* vendor/bearssl.c vendor/bearssl.h tools/bearssl_amalgamare.py tools/bearssl_amalgamare.sh"/>
# Usus: ./tools/bearssl_probare.sh [-summa]
set -u
cd "$(dirname "$0")/.."
summa_arboris() {
    (cd vendor/bearssl && find . -type f ! -path ./VENDOR.md -print0 \
        | LC_ALL=C sort -z | xargs -0 shasum -a 256) \
        | shasum -a 256 | cut -d' ' -f1
}
if [ "${1:-}" = "-summa" ]; then summa_arboris; exit 0; fi
fracta=0
scripta=$(sed -n 's/^summa: \([0-9a-f]\{64\}\)$/\1/p' vendor/bearssl/VENDOR.md)
vera=$(summa_arboris)
if [ -n "$scripta" ] && [ "$scripta" = "$vera" ]; then
    echo "  I   integritas arboris OK"
else
    echo "  I   FRACTUM: summa $vera != VENDOR.md '${scripta}'"; fracta=1
fi
B=build/bearssl_probare
mkdir -p "$B"
if clang -std=c99 -O2 -c vendor/bearssl.c -o "$B/bearssl.o" 2> "$B/fons.err" \
 && clang -std=c99 -O2 -Ivendor/bearssl/inc -Ivendor/bearssl/src \
        -o "$B/test_crypto" vendor/bearssl/test/test_crypto.c "$B/bearssl.o" 2> "$B/crypto.err" \
 && clang -std=c99 -O2 -Ivendor/bearssl/inc -Ivendor/bearssl/src \
        -o "$B/test_x509" vendor/bearssl/test/test_x509.c "$B/bearssl.o" 2> "$B/x509.err"; then
    if "$B/test_crypto" all > "$B/crypto.log" 2>&1; then
        echo "  II  test_crypto all OK ($(grep -c 'done' "$B/crypto.log") probationes)"
    else
        echo "  II  FRACTUM: test_crypto (vide $B/crypto.log)"; tail -5 "$B/crypto.log"; fracta=1
    fi
    if (cd vendor/bearssl && "../../$B/test_x509") > "$B/x509.log" 2>&1; then
        echo "  II  test_x509 OK"
    else
        echo "  II  FRACTUM: test_x509 (vide $B/x509.log)"; tail -5 "$B/x509.log"; fracta=1
    fi
else
    echo "  II  FRACTUM: aedificatio"; cat "$B"/*.err | head -20; fracta=1
fi
if [ "$fracta" -eq 0 ]; then echo "BEARSSL: sana"; exit 0; fi
echo "BEARSSL: FRACTA"; exit 1
```

`pythonica/silva.py` PORTAE (beside `'pythonica'`):

```python
    # tls-bearssl-plan T1: integritas vendor/bearssl + KAT upstream
    'bearssl': (['./tools/bearssl_probare.sh'], r'BEARSSL: (sana|FRACTA)'),
```

Then `git add -N tools/bearssl_probare.sh` (gate `portae-declaratae`
reads annotations of tracked runners), fill VENDOR.md's `summa:` from
`./tools/bearssl_probare.sh -summa`, and run the gate:
Expected: `I ... OK`, `II test_crypto all OK (88 probationes)`,
`II test_x509 OK`, `BEARSSL: sana` (~70 s).

- [ ] **Step 10: Hook exclusion for `vendor/` (R5), pinned by a smoke step**

`tools/unci-git/pre-commit:106-107` regex gains `vendor`:

```bash
        | grep -vE '(^|/)(scratchpad|build|fixa|amalgama|archivum|oracula|knotapel|vendor)/')"
```

`tools/formare_viae.sh:45` likewise
(`...|archivum|oracula|knotapel|vendor)/'`), and its header comment
(`:21-27`) loses the "vendor/: intra lineas mutatas" rule in favour of
"vendor/ numquam (alienum; octetim upstream - tls-bearssl R5)".

`tools/unci-git/fumus.sh`: a step XXI modelled exactly on step XX
(`:211-240`, knotapel), with a staged `vendor/fumus_vendor/proba.c` that
is badly formatted C99 (`// commentum` + `for (int i = 0; ...)`). After
the hook, the index and tree copies are byte-equal to the original, and
the hook output names no examen verdict for it. Header list line:
`#   XXI vendor/ EXCLUSA TOTA (tls-bearssl R5)`.
Plant: remove `|vendor` from the pre-commit regex → step XXI FRACTUM;
revert green.

- [ ] **Step 11: Run the test - expect PASS**

Run: `./compile_tests.sh probatio_bearssl`
Expected: 3/3 sections pass; examen on `probationes/probatio_bearssl.c`
ACCIPE (`./silva/examen.sh probationes/probatio_bearssl.c -machina`).

- [ ] **Step 12: Plants**

- `tools/bearssl_amalgamare.py`: `rn = sorted(n for n in nomina[s] if numerus[n] > 1)`
  -> `rn = []` → regenerate → `clang -c vendor/bearssl.c` fails
  (`redefinition of 'api_mul'`); the `generata` judgement and
  `bearssl` gate both red. Revert, regenerate, `IDEM`.
- One byte appended to `vendor/bearssl/README.txt` → gate `I FRACTUM`.
  Revert (`git checkout`), green.
- `_sectio_sha256`: `"abc", III` -> `"abd", III` → section red. Revert.

- [ ] **Step 13: Commit**

Stage: `vendor/bearssl` (and `git add -f
vendor/bearssl/build/.do_not_remove`), `vendor/bearssl.c`,
`vendor/bearssl.h`, the tools, the test, the stml/sh/py edits, lexicon,
worklog. Commit message:
`bearssl T1: vendor/bearssl (7bea48e) + amalgama generata + porta bearssl + stdint in lexico + vendor/ extra uncum`.
Expected owed gates include `bearssl`, `generata`, `radix`, `examen`,
`unci`, `auspex`. All green.

---

### Task 2: `fiducia` - platform trust layer (spec II.4)

**Files:**
- Create: `include/fiducia.h`, `lib/fiducia_macos.m`,
  `probationes/probatio_fiducia.c`
- Modify: `probationes/fixa/tls/generare.sh` (DER of each host cert + RSA
  leaf), `lib/tls_macos.m` (anchor path and certificate details go through
  fiducia), `aedilis.stml` (nexus rule `fiducia_macos`),
  `probationes/probatio_tls_servus.c` (section "certificatum"),
  `lib/tls_macos.worklog.md`

**Interfaces:**
- Produces:

```c
nomen structura {
    constans i8* octeti;    /* DER */
            i32  mensura;
} FiduciaCertificatum;

nomen structura {
       b32 valida;
       s64 non_ante;
       s64 non_post;
    chorda subiectum;
} FiduciaFolium;

b32 fiducia_iudicare (constans FiduciaCertificatum* catena, i32 numerus,
    constans character* hospes, constans character* ancora_via,
    chorda* causa, Piscina* piscina);
b32 fiducia_folium_describere (constans FiduciaCertificatum* folium,
    FiduciaFolium* exitus, Piscina* piscina);
```

- [ ] **Step 1: Fixtures gain DER + an RSA leaf (R12)**

In `probationes/fixa/tls/generare.sh`, inside `hospes ()` after the PEM is
signed:

```bash
    "$OSSL" x509 -in "$DEST/$1.pem" -outform DER -out "$DEST/$1.der"
```

and a key-type parameter. `clavis ()` gets an RSA twin:

```bash
clavis_rsa () { "$OSSL" genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:2048 -out "$1" 2>/dev/null; }
```

`hospes` takes an optional 5th argument `rsa` that selects `clavis_rsa`
for the leaf key; then one more invocation:
`hospes localhost_rsa "DNS:localhost,IP:127.0.0.1" "" "" rsa`. Header
comment gains `localhost_rsa.{pem,key,der}  ut localhost, clavis RSA-2048
(ECDHE_RSA)` and `*.der  folia DER (fiducia)`. Run it into `$S/certa` and
`ls` must show `localhost.der alienus.der expiratus.der ignotus.der
localhost_rsa.der`.

- [ ] **Step 2: Write the failing test `probationes/probatio_fiducia.c`**

```c
/* probatio_fiducia.c - fiducia_iudicare / fiducia_folium_describere
 * contra certificata probationis IN TEMPORE generata
 * (probationes/fixa/tls/generare.sh): ancora sola, hospes alienus,
 * expiratus, CA ignota, repositorium systematis (CA probationis
 * ignorat), non-DER, ancora absens; summarium folii. Sine rete. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "processus.h"
#include "fiducia.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

nomen structura {
    character fixa[DXII];
    character ancora[DXII];
} Locus;

interior vacuum
_suitam_parare (
    CredoContextus* c)
{
                  Locus* l = (Locus*)piscina_allocare(c->piscina,
                      magnitudo(Locus));
    constans character* area = credo_area(c);
    constans character* argumenta[III];
      ProcessusResultus  pr;

    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_NECESSE_NON_NIHIL(area);
    sprintf(l->fixa, "%s/certa", area);
    sprintf(l->ancora, "%s/ca.der", l->fixa);
    argumenta[ZEPHYRUM] = "probationes/fixa/tls/generare.sh";
    argumenta[I]        = l->fixa;
    argumenta[II]       = NIHIL;
    pr = processus_exsequi(argumenta, XXX * M, c->piscina);
    CREDO_NECESSE(pr.successus && pr.codex_exitus == ZEPHYRUM);
    c->datum = l;
}

/* <titulus>.der -> FiduciaCertificatum in piscina */
interior FiduciaCertificatum
_legere (
    CredoContextus* c,
    constans character* titulus)
{
    constans Locus* l = (constans Locus*)c->suita;
    FiduciaCertificatum fc;
              character via[DXII];
                   FILE* f;
                     i8* capsa = (i8*)piscina_allocare(c->piscina,
                         XVI * MXXIV);
                  size_t n = ZEPHYRUM;

    sprintf(via, "%s/%s.der", l->fixa, titulus);
    f = fopen(via, "rb");
    si (f != NIHIL && capsa != NIHIL)
    {
        n = fread(capsa, I, XVI * MXXIV, f);
        (vacuum)fclose(f);
    }
    fc.octeti  = capsa;
    fc.mensura = (i32)n;
    redde fc;
}

/* iudicium unius folii; causa in *causa */
interior b32
_iudicare (
    CredoContextus* c,
    constans character* titulus,
    constans character* hospes,
                   b32  cum_ancora,
                chorda* causa)
{
    constans Locus* l = (constans Locus*)c->suita;
    FiduciaCertificatum fc = _legere(c, titulus);

    CREDO_NECESSE(fc.mensura > ZEPHYRUM);
    redde fiducia_iudicare(&fc, I, hospes, cum_ancora ? l->ancora : NIHIL,
        causa, c->piscina);
}

interior b32
_continet (
    chorda c,
    constans character* fragmentum)
{
    i32 k = (i32)strlen(fragmentum);
    i32 i;

    per (i = ZEPHYRUM; i + k <= c.mensura; i++)
    {
        si (memcmp(c.datum + i, fragmentum, k) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
_sectio_ancora (
    CredoContextus* c)
{
    chorda causa;

    causa.datum = NIHIL; causa.mensura = ZEPHYRUM;
    CREDO_VERUM(_iudicare(c, "localhost", "localhost", VERUM, &causa));
    CREDO_VERUM(_iudicare(c, "localhost_rsa", "localhost", VERUM, &causa));
    CREDO_VERUM(_iudicare(c, "localhost", "127.0.0.1", VERUM, &causa));
}

interior vacuum
_reiectum (
    CredoContextus* c,
    constans character* titulus,
                   b32  cum_ancora)
{
    chorda causa;

    causa.datum = NIHIL; causa.mensura = ZEPHYRUM;
    CREDO_FALSUM(_iudicare(c, titulus, "localhost", cum_ancora, &causa));
    CREDO_VERUM(_continet(causa, "certificatum reiectum"));
}

interior vacuum _sectio_alienus (CredoContextus* c) { _reiectum(c, "alienus", VERUM); }
interior vacuum _sectio_expiratus (CredoContextus* c) { _reiectum(c, "expiratus", VERUM); }
interior vacuum _sectio_ignotus (CredoContextus* c) { _reiectum(c, "ignotus", VERUM); }
interior vacuum _sectio_sine_ancora (CredoContextus* c) { _reiectum(c, "localhost", FALSUM); }

interior vacuum
_sectio_non_der (
    CredoContextus* c)
{
    FiduciaCertificatum fc;
    chorda causa;

    fc.octeti  = (constans i8*)"non certificatum";
    fc.mensura = (i32)strlen("non certificatum");
    causa.datum = NIHIL; causa.mensura = ZEPHYRUM;
    CREDO_FALSUM(fiducia_iudicare(&fc, I, "localhost", NIHIL, &causa,
        c->piscina));
    CREDO_VERUM(_continet(causa, "DER"));
}

interior vacuum
_sectio_ancora_absens (
    CredoContextus* c)
{
    FiduciaCertificatum fc = _legere(c, "localhost");
    chorda causa;

    causa.datum = NIHIL; causa.mensura = ZEPHYRUM;
    CREDO_FALSUM(fiducia_iudicare(&fc, I, "localhost",
        "/nusquam/ancora.der", &causa, c->piscina));
    CREDO_VERUM(_continet(causa, "ancora legi non potest"));
}

/* folium: subiectum CN, validitas CCCLXV dierum (± diem) */
interior vacuum
_sectio_folium (
    CredoContextus* c)
{
    FiduciaCertificatum fc = _legere(c, "localhost");
          FiduciaFolium ff;
                    s64 dies;

    CREDO_NECESSE(fiducia_folium_describere(&fc, &ff, c->piscina));
    CREDO_VERUM(ff.valida);
    CREDO_CHORDA_AEQUALIS_LITERIS(ff.subiectum, "localhost");
    dies = (ff.non_post - ff.non_ante) / ((s64)LXXXVI * M + CD);
    CREDO_VERUM(dies >= CCCLXIV && dies <= CCCLXVI);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "ancora", _sectio_ancora, NIHIL, NIHIL, NIHIL, NIHIL },
    { "alienus", _sectio_alienus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "expiratus", _sectio_expiratus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ignotus", _sectio_ignotus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sine ancora", _sectio_sine_ancora, NIHIL, NIHIL, NIHIL, NIHIL },
    { "non der", _sectio_non_der, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ancora absens", _sectio_ancora_absens, NIHIL, NIHIL, NIHIL, NIHIL },
    { "folium", _sectio_folium, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("fiducia", SECTIONES, _suitam_parare,
        NIHIL);
}
```

(86400 s = `(s64)LXXXVI * M + CD`: Roman numerals 0-3999 are macros,
larger values are products. The formatter splits the one-line sections.)

- [ ] **Step 3: Run it - expect FAIL** (`fiducia.h` missing)

Run: `./compile_tests.sh probatio_fiducia`
Expected: compile error `'fiducia.h' file not found`.

- [ ] **Step 4: Write `include/fiducia.h`** (as in Interfaces, with this
  header comment):

```c
/* fiducia.h - iudicium fiduciae certificatorum per PLATFORMAM
 * (tls-bearssl-spec II.4)
 *
 * Catena (DER, folium primum) contra repositorium systematis aut ancoram
 * SOLAM, politia SSL cum nomine hospitis (catena, tempus, nomen,
 * revocatio - quidquid systema honorat). Implementatio:
 * fiducia_macos.m (SecTrustEvaluateWithError); postea fiducia_windows.c
 * (CertGetCertificateChain), fiducia_linux.c (br_x509_minimal +
 * fasciculus distributionis).
 *
 * NOTA: iudicium OBSTRUERE potest (intermedia per AIA, revocatio) - ut
 * SecureTransport intus faciebat. FiduciaFolium = TlsCertificatum sine
 * tls.h (regula R6). */
```

with `fiducia_iudicare` documented as "VERUM si fide digna pro hospite;
ancora_via NIHIL = repositorium systematis, aliter DER quod SOLUM fide
dignum est; FALSUM -> *causa nominata ('certificatum reiectum: <ratio
systematis>', 'certificatum catenae non est DER', 'ancora legi non
potest', ...)" and `fiducia_folium_describere` as "folium DER -> tempora
(epocha unix) et subiectum; FALSUM si non DER (exitus valida FALSUM)".

- [ ] **Step 5: Write `lib/fiducia_macos.m`**

```objc
/* fiducia_macos.m - fiducia.h per Security.framework. Capita systematis
 * ANTE latina.h. Ex tls_macos.m translata (tls-bearssl T2):
 * _ancoram_legere, iudicium SecTrust cum politia SSL expressa,
 * _tempus_ex_valore, _proprietatem_capere. */
#include <Security/Security.h>
#include <CoreFoundation/CoreFoundation.h>
#include <stdio.h>
#include <string.h>

#include "latina.h"
#include "fiducia.h"

interior SecCertificateRef
_certificatum_ex_der (
    constans i8* octeti,
           i32  mensura)
{
            CFDataRef data;
    SecCertificateRef cert;

    si (octeti == NIHIL || mensura == ZEPHYRUM)
    {
        redde NULL;
    }
    data = CFDataCreate(NULL, (constans UInt8*)octeti, (CFIndex)mensura);
    si (data == NULL)
    {
        redde NULL;
    }
    cert = SecCertificateCreateWithData(NULL, data);
    CFRelease(data);
    redde cert;
}

/* ancora DER ex via; NULL + *causa si nequit */
interior SecCertificateRef
_ancoram_legere (
     constans character*  via,
    constans character** causa)
{
                 FILE* f;
         unsigned char capsa[XVI * MXXIV];
                size_t n;
    SecCertificateRef cert;

    f = fopen(via, "rb");
    si (!f)
    {
        *causa = "ancora legi non potest";
        redde NULL;
    }
    n = fread(capsa, 1, sizeof(capsa), f);
    fclose(f);
    cert = _certificatum_ex_der((constans i8*)capsa, (i32)n);
    si (!cert)
    {
        *causa = "ancora non est certificatum DER";
    }
    redde cert;
}

/* "<praefixum>: <descriptio CFError>" in piscina */
interior chorda
_causa (
    constans character* praefixum,
             CFErrorRef erratum,
               Piscina* piscina)
{
      character nuntius[CCLVI];
    CFStringRef descriptio = erratum ? CFErrorCopyDescription(erratum)
                                     : NULL;

    strcpy(nuntius, praefixum);
    si (descriptio)
    {
        strcat(nuntius, ": ");
        CFStringGetCString(descriptio, nuntius + strlen(nuntius),
            (CFIndex)(sizeof(nuntius) - strlen(nuntius)),
            kCFStringEncodingUTF8);
        CFRelease(descriptio);
    }
    redde chorda_ex_literis(nuntius, piscina);
}

b32
fiducia_iudicare (
    constans FiduciaCertificatum* catena,
                             i32  numerus,
             constans character* hospes,
             constans character* ancora_via,
                          chorda* causa,
                         Piscina* piscina)
{
    CFMutableArrayRef  certa = NULL;
          CFStringRef  hospes_cf = NULL;
         SecPolicyRef  politia = NULL;
          SecTrustRef  fiducia = NULL;
           CFErrorRef  erratum = NULL;
  constans character*  defectus = NIHIL;
                  b32  bene = FALSUM;
                  i32  i;

    si (causa == NIHIL || piscina == NIHIL)
    {
        redde FALSUM;
    }
    si (catena == NIHIL || numerus == ZEPHYRUM || hospes == NIHIL)
    {
        *causa = chorda_ex_literis("fiducia: argumenta invalida", piscina);
        redde FALSUM;
    }
    certa     = CFArrayCreateMutable(NULL, (CFIndex)numerus,
        &kCFTypeArrayCallBacks);
    hospes_cf = CFStringCreateWithCString(NULL, hospes,
        kCFStringEncodingUTF8);
    si (certa == NULL || hospes_cf == NULL)
    {
        defectus = "fiducia: allocatio CoreFoundation fallita";
    }
    per (i = ZEPHYRUM; defectus == NIHIL && i < numerus; i++)
    {
        SecCertificateRef c = _certificatum_ex_der(catena[i].octeti,
            catena[i].mensura);

        si (c == NULL)
        {
            defectus = "certificatum catenae non est DER";
            frange;
        }
        CFArrayAppendValue(certa, c);
        CFRelease(c);
    }
    si (defectus == NIHIL)
    {
        politia = SecPolicyCreateSSL(true, hospes_cf);
        si (SecTrustCreateWithCertificates(certa, politia, &fiducia)
            != errSecSuccess)
        {
            defectus = "SecTrustCreateWithCertificates fallita";
        }
    }
    si (defectus == NIHIL && ancora_via != NIHIL)
    {
        SecCertificateRef ancora = _ancoram_legere(ancora_via, &defectus);

        si (ancora != NULL)
        {
            constans void* valores[I];
                CFArrayRef ancorae;

            valores[ZEPHYRUM] = ancora;
            ancorae = CFArrayCreate(NULL, valores, 1,
                &kCFTypeArrayCallBacks);
            CFRelease(ancora);
            si (ancorae == NULL)
            {
                defectus = "ancora: CFArrayCreate fallita";
            }
            alioquin
            {
                SecTrustSetAnchorCertificates(fiducia, ancorae);
                SecTrustSetAnchorCertificatesOnly(fiducia, true);
                CFRelease(ancorae);
            }
        }
    }
    si (defectus != NIHIL)
    {
        *causa = chorda_ex_literis(defectus, piscina);
    }
    alioquin
    {
        bene = SecTrustEvaluateWithError(fiducia, &erratum) ? VERUM
                                                             : FALSUM;
        si (!bene)
        {
            *causa = _causa("certificatum reiectum", erratum, piscina);
        }
    }
    si (erratum)   { CFRelease(erratum); }
    si (fiducia)   { CFRelease(fiducia); }
    si (politia)   { CFRelease(politia); }
    si (hospes_cf) { CFRelease(hospes_cf); }
    si (certa)     { CFRelease(certa); }
    redde bene;
}
```

followed by `_tempus_ex_valore` and `_proprietatem_capere` moved verbatim
from `tls_macos.m:971-1020`, and:

```objc
b32
fiducia_folium_describere (
    constans FiduciaCertificatum* folium,
                   FiduciaFolium* exitus,
                         Piscina* piscina)
{
    SecCertificateRef  cert;
      CFDictionaryRef  valores = NULL;
           CFArrayRef  claves;
            CFTypeRef  oids[II];
          CFStringRef  summarium;

    si (exitus == NIHIL)
    {
        redde FALSUM;
    }
    memset(exitus, ZEPHYRUM, magnitudo(*exitus));
    cert = folium ? _certificatum_ex_der(folium->octeti, folium->mensura)
                  : NULL;
    si (cert == NULL)
    {
        redde FALSUM;
    }
    oids[ZEPHYRUM] = kSecOIDX509V1ValidityNotBefore;
    oids[I]        = kSecOIDX509V1ValidityNotAfter;
    claves = CFArrayCreate(NULL, oids, II, &kCFTypeArrayCallBacks);
    si (claves != NULL)
    {
        valores = SecCertificateCopyValues(cert, claves, NULL);
        CFRelease(claves);
    }
    si (valores != NULL)
    {
        exitus->non_ante = _tempus_ex_valore(_proprietatem_capere(valores,
            kSecOIDX509V1ValidityNotBefore));
        exitus->non_post = _tempus_ex_valore(_proprietatem_capere(valores,
            kSecOIDX509V1ValidityNotAfter));
        CFRelease(valores);
    }
    summarium = SecCertificateCopySubjectSummary(cert);
    si (summarium != NULL)
    {
        character capsa[CCLVI];

        si (CFStringGetCString(summarium, capsa, (CFIndex)CCLVI,
                kCFStringEncodingUTF8))
        {
            exitus->subiectum = chorda_ex_literis(capsa, piscina);
        }
        CFRelease(summarium);
    }
    CFRelease(cert);
    exitus->valida = (exitus->non_post > ZEPHYRUM);
    redde exitus->valida;
}
```

`aedilis.stml` `<nexus>`:

```xml
    <regula obiectum="fiducia_macos">
      <vexillum! (>-framework Security
      <vexillum! (>-framework CoreFoundation
    </regula>
```

Then `./tools/compile_tests_fontes_generare.sh`.

- [ ] **Step 6: Run the test - expect PASS**

Run: `./compile_tests.sh probatio_fiducia`
Expected: 8/8. `./silva/censor.sh lib/fiducia_macos.m` prints no
`[CENSURA]` rows.

- [ ] **Step 7: SecureTransport uses fiducia (behaviour unchanged)**

In `lib/tls_macos.m`:
- Struct: drop `CFArrayRef ancorae; CFStringRef hospes_cf;`; add
  `constans character* hospes; constans character* ancora_via;` (piscina
  copies: the caller's option strings need not outlive setup).
- `_contextum_parare`: replace the `_ancoram_legere` block
  (`:429-439`) with copies:

```objc
    conn->hospes = _copiare(hospes, conn->piscina);
    si (optiones && optiones->ancora_via)
    {
        conn->ancora_via = _copiare(optiones->ancora_via, conn->piscina);
    }
```

  where

```objc
/* chorda C in piscina (optiones vocantis non vivunt ultra pactionem) */
interior constans character*
_copiare (
    constans character* s,
               Piscina* piscina)
{
    size_t     n = strlen(s) + 1;
    character* d = (character*)piscina_allocare(piscina, (i64)n);

    si (d)
    {
        memcpy(d, s, n);
    }
    redde d;
}
```

  The condition `conn->ancorae` at `:566` and `:689` becomes
  `conn->ancora_via`.
- A shared helper replaces `_ancoram_legere` and `_fiduciam_iudicare`
  (the anchor must still be readable at setup: keep the old early failure
  by `access(optiones->ancora_via, R_OK)` -> "ancora legi non potest"):

```objc
/* catena hospitis (DER, folium primum) in piscina; numerus in *numerus;
 * NIHIL si nulla */
interior FiduciaCertificatum*
_catenam_capere (
    TlsConnexio* conn,
            i32* numerus)
{
           SecTrustRef  fiducia = NULL;
              CFIndex  n;
              CFIndex  k;
  FiduciaCertificatum*  catena;

    *numerus = ZEPHYRUM;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    si (SSLCopyPeerTrust(conn->ssl_context, &fiducia) != noErr
        || fiducia == NULL)
    {
        redde NIHIL;
    }
    n = SecTrustGetCertificateCount(fiducia);
    catena = n > 0 ? (FiduciaCertificatum*)piscina_allocare(conn->piscina,
        (i64)((size_t)n * magnitudo(FiduciaCertificatum))) : NIHIL;
    per (k = 0; catena && k < n; k++)
    {
        CFDataRef data = SecCertificateCopyData(
            SecTrustGetCertificateAtIndex(fiducia, k));
        i8* octeti = data ? (i8*)piscina_allocare(conn->piscina,
            (i64)CFDataGetLength(data)) : NIHIL;

        si (octeti == NIHIL)
        {
            si (data) { CFRelease(data); }
            catena = NIHIL;
            frange;
        }
        memcpy(octeti, CFDataGetBytePtr(data),
            (size_t)CFDataGetLength(data));
        catena[k].octeti  = octeti;
        catena[k].mensura = (i32)CFDataGetLength(data);
        CFRelease(data);
    }
#pragma clang diagnostic pop
    CFRelease(fiducia);
    si (catena)
    {
        *numerus = (i32)n;
    }
    redde catena;
}

/* post errSSLServerAuthCompleted cum ancora: fiducia_iudicare */
interior b32
_fiduciam_iudicare (
    TlsConnexio* conn)
{
                    i32  numerus;
    FiduciaCertificatum* catena = _catenam_capere(conn, &numerus);

    si (catena == NIHIL)
    {
        conn->erratum = chorda_ex_literis("certificatum hospitis deest",
            conn->piscina);
        redde FALSUM;
    }
    redde fiducia_iudicare(catena, numerus, conn->hospes, conn->ancora_via,
        &conn->erratum, conn->piscina);
}
```

- `tls_certificatum_obtinere` body: `_catenam_capere` ->
  `fiducia_folium_describere(&catena[0], &ff, piscina)` -> copy
  `valida/non_ante/non_post/subiectum` into `exitus`. Delete
  `_tempus_ex_valore`, `_proprietatem_capere` (moved to fiducia).
- `tls_claudere`: drop the `ancorae` / `hospes_cf` releases.
- `#include "fiducia.h"` after `tls.h`.

- [ ] **Step 8: New offline section for certificate details**

`probationes/probatio_tls_servus.c`, before `SECTIONES`:

```c
/* certificatum hospitis: summarium folii ex connexione aperta */
interior vacuum
_sectio_certificatum (
    CredoContextus* c)
{
    constans Locus* l = (constans Locus*)c->suita;
         ServusTls  s;
       TlsOptiones  opt = tls_optiones_default();
       TlsResultus  tr;
   TlsCertificatum  cert;
               s64  dies;

    opt.ancora_via = l->ancora;
    opt.timeout_ms = V * M;
    CREDO_NECESSE(servum_tls_incipere(&s, l->dir, l->fixa, "localhost",
        c->piscina));
    tr = tls_connectere_cum_optionibus("localhost", s.portus, &opt,
        c->piscina);
    CREDO_NECESSE(tr.successus);
    CREDO_VERUM(tls_certificatum_obtinere(tr.connexio, &cert));
    CREDO_VERUM(cert.valida);
    CREDO_CHORDA_AEQUALIS_LITERIS(cert.subiectum, "localhost");
    dies = (cert.non_post - cert.non_ante) / ((s64)LXXXVI * M + CD);
    CREDO_VERUM(dies >= CCCLXIV && dies <= CCCLXVI);
    tls_claudere(tr.connexio);
    servum_tls_claudere(&s);
}
```

and `{ "certificatum", _sectio_certificatum, NIHIL, NIHIL, NIHIL, NIHIL },`
after "obstruens". Also an RSA leg in `_sectio_parvum`'s twin:
`_sectio_rsa` = `_sectio_parvum` with certificate `"localhost_rsa"`
(row `"rsa"`).

Run: `./compile_tests.sh probatio_tls_servus probatio_tls_gradus
probatio_fiducia` → all green (9 + 3 + 8 sections).

- [ ] **Step 9: Plants**

- `bene = SecTrustEvaluateWithError(fiducia, &erratum) ? VERUM : FALSUM;`
  -> `bene = VERUM;` (compiles; `erratum` still released) →
  fiducia alienus/expiratus/ignotus/sine ancora red AND tls_servus
  alienus/expiratus/ignotus red (SecureTransport goes through fiducia
  for the anchor path). Revert green.
- `fiducia_folium_describere`: swap the two `_proprietatem_capere` OIDs →
  "folium" red (negative days) and tls_servus "certificatum" red.
- `_catenam_capere`: `catena[k].mensura = (i32)CFDataGetLength(data);` ->
  `... - 1;` → the truncated DER is "non DER", so accepted handshakes are
  rejected: tls_servus parvum, magnum, certificatum, rsa red (the reject
  sections stay green - they are rejected either way). Revert green.
- Known mute: `SecTrustSetAnchorCertificatesOnly(fiducia, true)` ->
  `false` changes nothing offline. The system store knows neither the test
  CA nor the rejected certs, so this is the limit of an offline test.
  Record it in the worklog; the network `-badssl` run in T6 is its only
  check.

- [ ] **Step 10: Commit**

`fiducia T2: fiducia.h + fiducia_macos.m (SecTrust); SecureTransport ancoram et certificatum per fiduciam; fixa DER + folium RSA`.
Worklog note in `lib/tls_macos.worklog.md` (anchor path moved; the
SecTrust policy is still explicit). Expected owed gates: radix, examen,
silex (closure change: Security now via fiducia_macos too), briar-fumus.

---

### Task 3: dispatcher - `lib/tls.c` with SecureTransport behind it (spec II.5)

**Files:**
- Rename: `lib/tls_macos.m` -> `lib/tls_securetransport_macos.m`
  (`git mv`), `lib/tls_macos.worklog.md` ->
  `lib/tls_securetransport_macos.worklog.md`
- Create: `include/tls_securetransport.h`, `lib/tls.c`,
  `probationes/probatio_tls_vectura.c`
- Modify: `include/tls.h`, `aedilis.stml` (nexus rule rename),
  `probationes/probatio_silex.c:797-880`, `lib/silex.c:2404,2524`
  (comments), `tools/silex_semen_fumus.sh:15` (comment),
  `probationes/fixa/stml_aurea/fontes/aedilis.stml.fons:44`? (ONLY if that
  golden is a copy of the live aedilis.stml that a gate compares; check
  with `git grep -n aedilis.stml.fons`; else leave it)

**Interfaces:**
- Consumes: the backend's old public functions (now
  `tls_securetransport_*`).
- Produces (public, `tls.h`):

```c
nomen enumeratio {
    TLS_VECTURA_ORDINARIA = 0,     /* RHUBARB_TLS aut ordinaria domus */
    TLS_VECTURA_BEARSSL,
    TLS_VECTURA_SECURETRANSPORT
} TlsVectura;
/* TlsOptiones: ultimum membrum */  TlsVectura vectura;
TlsVectura tls_vecturam_eligere (constans TlsOptiones* optiones);
TlsVectura tls_vectura (TlsConnexio* connexio);
```

- Produces (internal, `tls_securetransport.h`): `TlsSecureTransport`,
  `TlsSecureTransportResultus {b32 successus; TlsSecureTransport*
  connexio; TlsError error; chorda error_descriptio;}`, and
  `tls_securetransport_{connectere,incipere,gradus,gradus_error,mittere,
  recipere,claudere,est_valida,certificatum}` with the tls.h contracts.

- [ ] **Step 1: Write the failing test `probationes/probatio_tls_vectura.c`**

```c
/* probatio_tls_vectura.c - electio vecturae TLS (tls-bearssl-spec II.5,
 * R7): optiones explicitae vincunt; ORDINARIA -> RHUBARB_TLS
 * ('bearssl' | 'securetransport'), aliter ordinaria domus; connexio
 * vecturam suam meminit. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "processus.h"
#include "tls.h"
#include "servus_tls.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* ordinaria domus: T3 SECURETRANSPORT, T6 BEARSSL */
#define VECTURA_DOMUS TLS_VECTURA_SECURETRANSPORT

nomen structura {
    character dir[DXII];
    character fixa[DXII];
    character ancora[DXII];
} Locus;

interior vacuum
_suitam_parare (
    CredoContextus* c)
{
                  Locus* l = (Locus*)piscina_allocare(c->piscina,
                      magnitudo(Locus));
    constans character* area = credo_area(c);
    constans character* argumenta[III];
      ProcessusResultus  pr;

    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_NECESSE_NON_NIHIL(area);
    strcpy(l->dir, area);
    sprintf(l->fixa, "%s/certa", area);
    sprintf(l->ancora, "%s/ca.der", l->fixa);
    argumenta[ZEPHYRUM] = "probationes/fixa/tls/generare.sh";
    argumenta[I]        = l->fixa;
    argumenta[II]       = NIHIL;
    pr = processus_exsequi(argumenta, XXX * M, c->piscina);
    CREDO_NECESSE(pr.successus && pr.codex_exitus == ZEPHYRUM);
    c->datum = l;
}

/* ambitus mundus post omnem sectionem */
interior vacuum
_purgare (
    CredoContextus* c)
{
    (vacuum)c;
    (vacuum)unsetenv("RHUBARB_TLS");
}

interior vacuum
_sectio_ordinaria (
    CredoContextus* c)
{
    TlsOptiones opt = tls_optiones_default();

    (vacuum)c;
    CREDO_AEQUALIS_I32((i32)opt.vectura, (i32)TLS_VECTURA_ORDINARIA);
    (vacuum)unsetenv("RHUBARB_TLS");
    CREDO_AEQUALIS_I32((i32)tls_vecturam_eligere(&opt), (i32)VECTURA_DOMUS);
    CREDO_AEQUALIS_I32((i32)tls_vecturam_eligere(NIHIL),
        (i32)VECTURA_DOMUS);
}

interior vacuum
_sectio_ambitus (
    CredoContextus* c)
{
    TlsOptiones opt = tls_optiones_default();

    (vacuum)c;
    (vacuum)setenv("RHUBARB_TLS", "securetransport", I);
    CREDO_AEQUALIS_I32((i32)tls_vecturam_eligere(&opt),
        (i32)TLS_VECTURA_SECURETRANSPORT);
    (vacuum)setenv("RHUBARB_TLS", "bearssl", I);
    CREDO_AEQUALIS_I32((i32)tls_vecturam_eligere(&opt),
        (i32)TLS_VECTURA_BEARSSL);
    (vacuum)setenv("RHUBARB_TLS", "nugae", I);
    CREDO_AEQUALIS_I32((i32)tls_vecturam_eligere(&opt), (i32)VECTURA_DOMUS);
    /* explicita vincit ambitum */
    (vacuum)setenv("RHUBARB_TLS", "bearssl", I);
    opt.vectura = TLS_VECTURA_SECURETRANSPORT;
    CREDO_AEQUALIS_I32((i32)tls_vecturam_eligere(&opt),
        (i32)TLS_VECTURA_SECURETRANSPORT);
}

interior vacuum
_sectio_meminit (
    CredoContextus* c)
{
    constans Locus* l = (constans Locus*)c->suita;
         ServusTls  s;
       TlsOptiones  opt = tls_optiones_default();
       TlsResultus  tr;

    CREDO_AEQUALIS_I32((i32)tls_vectura(NIHIL), (i32)TLS_VECTURA_ORDINARIA);
    opt.ancora_via = l->ancora;
    opt.timeout_ms = V * M;
    opt.vectura    = TLS_VECTURA_SECURETRANSPORT;
    CREDO_NECESSE(servum_tls_incipere(&s, l->dir, l->fixa, "localhost",
        c->piscina));
    tr = tls_connectere_cum_optionibus("localhost", s.portus, &opt,
        c->piscina);
    CREDO_NECESSE(tr.successus);
    CREDO_AEQUALIS_I32((i32)tls_vectura(tr.connexio),
        (i32)TLS_VECTURA_SECURETRANSPORT);
    tls_claudere(tr.connexio);
    servum_tls_claudere(&s);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "ordinaria", _sectio_ordinaria, NIHIL, _purgare, NIHIL, NIHIL },
    { "ambitus", _sectio_ambitus, NIHIL, _purgare, NIHIL, NIHIL },
    { "meminit", _sectio_meminit, NIHIL, _purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("tls_vectura", SECTIONES,
        _suitam_parare, NIHIL);
}
```

- [ ] **Step 2: Run it - expect FAIL** (`TLS_VECTURA_ORDINARIA` undeclared)

- [ ] **Step 3: `include/tls.h`**

- Header comment line 4: `Implementatio: tls.c (vectura per
  connexionem: tls_bearssl.c | tls_securetransport_macos.m)`; line 28:
  `Opaque handle - tls.c`.
- Before `TlsOptiones`, the `TlsVectura` enum with this comment:
  `/* vectura (implementatio) - tls-bearssl-spec II.5. ORDINARIA:
  RHUBARB_TLS ('bearssl' | 'securetransport') si positum, aliter
  ordinaria domus. Vocantes qui optiones memset() faciunt ORDINARIAM
  habent. */`
- `TlsOptiones` gains, last: `TlsVectura vectura; /* ORDINARIA =
  ambitus/domus */`.
- After `tls_optiones_default`:

```c
/* vectura quam optiones (NIHIL licet) et ambitus RHUBARB_TLS eligunt -
 * numquam ORDINARIA. Per connexionem legitur (setenv vim habet). */
TlsVectura
tls_vecturam_eligere (
    constans TlsOptiones* optiones);

/* vectura connexionis; ORDINARIA si connexio NIHIL */
TlsVectura
tls_vectura (
    TlsConnexio* connexio);
```

- [ ] **Step 4: Move the backend**

```bash
git mv lib/tls_macos.m lib/tls_securetransport_macos.m
git mv lib/tls_macos.worklog.md lib/tls_securetransport_macos.worklog.md
```

Exact word-boundary renames inside `lib/tls_securetransport_macos.m`
ONLY, by a checked python step (each pattern's count printed and
compared to a fresh grep; no other file touched):

```python
import re
via = 'lib/tls_securetransport_macos.m'
s = open(via).read()
paria = [  # ordo: longiora primum
    (r'\btls_connectere_cum_optionibus\b', 'tls_securetransport_connectere'),
    (r'\btls_certificatum_obtinere\b', 'tls_securetransport_certificatum'),
    (r'\btls_gradus_error\b', 'tls_securetransport_gradus_error'),
    (r'\btls_gradus\b', 'tls_securetransport_gradus'),
    (r'\btls_incipere\b', 'tls_securetransport_incipere'),
    (r'\btls_mittere\b', 'tls_securetransport_mittere'),
    (r'\btls_recipere\b', 'tls_securetransport_recipere'),
    (r'\btls_claudere\b', 'tls_securetransport_claudere'),
    (r'\btls_est_valida\b', 'tls_securetransport_est_valida'),
    (r'\bTlsResultus\b', 'TlsSecureTransportResultus'),
    (r'\bTlsConnexio\b', 'TlsSecureTransport'),
    (r'#include "tls\.h"', '#include "tls_securetransport.h"'),
]
for vetus, novus in paria:
    s, n = re.subn(vetus, novus, s)
    print(n, vetus)
open(via, 'w').write(s)
```

Then delete from that file, by hand: `tls_optiones_default`,
`tls_connectere` (no-options form), `tls_mittere_omnia`
(`tls_securetransport_mittere_omnia` after the rename),
`tls_error_descriptio` - they move to `tls.c` (step 6). `tls_mittere_omnia`
called `tls_mittere` (now `tls_securetransport_mittere`): it moves to
tls.c as a dispatcher over `tls_mittere`. Update the file header comment:
`tls_securetransport_macos.m - VECTURA SecureTransport (olim
tls_macos.m; tls-bearssl T3): mores idem; solum tls.c vocat`.

- [ ] **Step 5: Write `include/tls_securetransport.h`**

```c
/* tls_securetransport.h - VECTURA SecureTransport, INTERNA: solum
 * lib/tls.c includit (tls-bearssl-spec II.5, regula R8). Implementatio:
 * tls_securetransport_macos.m. Contractus quisque = functio tls.h
 * eiusdem nominis sine 'securetransport'. */
#ifndef TLS_SECURETRANSPORT_H
#define TLS_SECURETRANSPORT_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "tcp.h"
#include "tls.h"

nomen structura TlsSecureTransport TlsSecureTransport;

nomen structura {
                   b32  successus;
    TlsSecureTransport* connexio;
              TlsError  error;
                chorda  error_descriptio;
} TlsSecureTransportResultus;

TlsSecureTransportResultus
tls_securetransport_connectere (
      constans character* hospes,
                     i32  portus,
    constans TlsOptiones* optiones,
                 Piscina* piscina);

TlsSecureTransportResultus
tls_securetransport_incipere (
             TcpConnexio* tcp,
      constans character* hospes,
    constans TlsOptiones* optiones,
                 Piscina* piscina);

TlsGradus
tls_securetransport_gradus (
    TlsSecureTransport* connexio);

chorda
tls_securetransport_gradus_error (
    TlsSecureTransport* connexio);

s32
tls_securetransport_mittere (
    TlsSecureTransport* connexio,
          constans i8* data,
                  i32  mensura);

s32
tls_securetransport_recipere (
    TlsSecureTransport* connexio,
                   i8* buffer,
                  i32  capacitas);

vacuum
tls_securetransport_claudere (
    TlsSecureTransport* connexio);

b32
tls_securetransport_est_valida (
    TlsSecureTransport* connexio);

b32
tls_securetransport_certificatum (
    TlsSecureTransport* connexio,
       TlsCertificatum* exitus);

#endif /* TLS_SECURETRANSPORT_H */
```

- [ ] **Step 6: Write `lib/tls.c`**

```c
/* tls.c - tls.h: DISPATCHER vecturarum (tls-bearssl-spec II.5). Vectura
 * per connexionem eligitur (TlsOptiones.vectura; ORDINARIA ->
 * RHUBARB_TLS; aliter ordinaria domus) et in connexione servatur.
 * Vecturae: tls_securetransport_macos.m; tls_bearssl.c (T4). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "tcp.h"
#include "tls.h"
#include "tls_securetransport.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* ordinaria domus (T6: TLS_VECTURA_BEARSSL) */
#define TLS_VECTURA_DOMUS TLS_VECTURA_SECURETRANSPORT

structura TlsConnexio {
            TlsVectura  vectura;
    TlsSecureTransport* securetransport;
};

TlsOptiones
tls_optiones_default (vacuum)
{
    TlsOptiones opt;

    memset(&opt, ZEPHYRUM, magnitudo(opt));
    opt.timeout_ms              = XXX * M;
    opt.verificare_certificatum = VERUM;
    opt.ancora_via              = NIHIL;
    opt.vectura                 = TLS_VECTURA_ORDINARIA;
    redde opt;
}

TlsVectura
tls_vecturam_eligere (
    constans TlsOptiones* optiones)
{
    constans character* ambitus;

    si (optiones != NIHIL && optiones->vectura != TLS_VECTURA_ORDINARIA)
    {
        redde optiones->vectura;
    }
    ambitus = getenv("RHUBARB_TLS");
    si (ambitus != NIHIL && strcmp(ambitus, "bearssl") == ZEPHYRUM)
    {
        redde TLS_VECTURA_BEARSSL;
    }
    si (ambitus != NIHIL && strcmp(ambitus, "securetransport") == ZEPHYRUM)
    {
        redde TLS_VECTURA_SECURETRANSPORT;
    }
    redde TLS_VECTURA_DOMUS;
}

interior TlsResultus
_error (
              TlsError  error,
    constans character* nuntius,
              Piscina* piscina)
{
    TlsResultus r;

    r.successus        = FALSUM;
    r.connexio         = NIHIL;
    r.error            = error;
    r.error_descriptio = chorda_ex_literis(nuntius, piscina);
    redde r;
}

/* resultus vecturae SecureTransport -> TlsResultus */
interior TlsResultus
_involvere_securetransport (
    TlsSecureTransportResultus  st,
                      Piscina* piscina)
{
    TlsResultus r;

    r.successus        = st.successus;
    r.error            = st.error;
    r.error_descriptio = st.error_descriptio;
    r.connexio         = NIHIL;
    si (st.successus)
    {
        r.connexio = (TlsConnexio*)piscina_allocare(piscina,
            (i64)magnitudo(TlsConnexio));
        si (r.connexio == NIHIL)
        {
            tls_securetransport_claudere(st.connexio);
            redde _error(TLS_ERROR_TCP, "Allocatio fallita", piscina);
        }
        memset(r.connexio, ZEPHYRUM, magnitudo(TlsConnexio));
        r.connexio->vectura         = TLS_VECTURA_SECURETRANSPORT;
        r.connexio->securetransport = st.connexio;
    }
    redde r;
}

TlsResultus
tls_connectere (
    constans character* hospes,
                   i32  portus,
               Piscina* piscina)
{
    TlsOptiones opt = tls_optiones_default();

    redde tls_connectere_cum_optionibus(hospes, portus, &opt, piscina);
}

TlsResultus
tls_connectere_cum_optionibus (
      constans character* hospes,
                     i32  portus,
    constans TlsOptiones* optiones,
                 Piscina* piscina)
{
    commutatio (tls_vecturam_eligere(optiones))
    {
        casus TLS_VECTURA_SECURETRANSPORT:
            redde _involvere_securetransport(tls_securetransport_connectere(
                hospes, portus, optiones, piscina), piscina);
        ordinarius:
            redde _error(TLS_ERROR_HANDSHAKE, "vectura bearssl nondum",
                piscina);
    }
}

TlsResultus
tls_incipere (
             TcpConnexio* tcp,
      constans character* hospes,
    constans TlsOptiones* optiones,
                 Piscina* piscina)
{
    commutatio (tls_vecturam_eligere(optiones))
    {
        casus TLS_VECTURA_SECURETRANSPORT:
            redde _involvere_securetransport(tls_securetransport_incipere(
                tcp, hospes, optiones, piscina), piscina);
        ordinarius:
            si (tcp)
            {
                tcp_claudere(tcp);
            }
            redde _error(TLS_ERROR_HANDSHAKE, "vectura bearssl nondum",
                piscina);
    }
}

TlsVectura
tls_vectura (
    TlsConnexio* connexio)
{
    redde connexio ? connexio->vectura : TLS_VECTURA_ORDINARIA;
}

TlsGradus
tls_gradus (
    TlsConnexio* connexio)
{
    si (connexio == NIHIL)
    {
        redde TLS_GRADUS_ERROR;
    }
    redde tls_securetransport_gradus(connexio->securetransport);
}

chorda
tls_gradus_error (
    TlsConnexio* connexio)
{
    chorda vacua;

    si (connexio == NIHIL)
    {
        vacua.datum   = NIHIL;
        vacua.mensura = ZEPHYRUM;
        redde vacua;
    }
    redde tls_securetransport_gradus_error(connexio->securetransport);
}

s32
tls_mittere (
    TlsConnexio* connexio,
    constans i8* data,
            i32  mensura)
{
    si (connexio == NIHIL)
    {
        redde -I;
    }
    redde tls_securetransport_mittere(connexio->securetransport, data,
        mensura);
}

b32
tls_mittere_omnia (
    TlsConnexio* connexio,
    constans i8* data,
            i32  mensura)
{
    i32 totalis = ZEPHYRUM;

    si (connexio == NIHIL || data == NIHIL || mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    dum (totalis < mensura)
    {
        s32 n = tls_mittere(connexio, data + totalis, mensura - totalis);

        si (n < ZEPHYRUM && n != TLS_ITERUM)
        {
            redde FALSUM;
        }
        si (n <= ZEPHYRUM)
        {
            (vacuum)usleep(M);   /* 1 ms, ut olim */
            perge;
        }
        totalis += (i32)n;
    }
    redde VERUM;
}

s32
tls_recipere (
    TlsConnexio* connexio,
             i8* buffer,
            i32  capacitas)
{
    si (connexio == NIHIL)
    {
        redde -I;
    }
    redde tls_securetransport_recipere(connexio->securetransport, buffer,
        capacitas);
}

vacuum
tls_claudere (
    TlsConnexio* connexio)
{
    si (connexio == NIHIL)
    {
        redde;
    }
    tls_securetransport_claudere(connexio->securetransport);
}

b32
tls_est_valida (
    TlsConnexio* connexio)
{
    redde connexio != NIHIL
        && tls_securetransport_est_valida(connexio->securetransport);
}

b32
tls_certificatum_obtinere (
        TlsConnexio* connexio,
    TlsCertificatum* exitus)
{
    si (connexio == NIHIL)
    {
        si (exitus)
        {
            memset(exitus, ZEPHYRUM, magnitudo(*exitus));
        }
        redde FALSUM;
    }
    redde tls_securetransport_certificatum(connexio->securetransport,
        exitus);
}
```

plus `tls_error_descriptio` moved verbatim from the old file. NOTE the old
`tls_mittere_omnia` refused `mensura <= 0`, retried on `n == 0` and
failed on any negative. The dispatcher version also retries on
`TLS_ITERUM`; for SecureTransport's blocking path `tls_mittere` never
returns `TLS_ITERUM`, so behaviour is unchanged. `usleep` needs
`postulata_posix.h` first if examen asks (POSIX in a .c file) - add it as
line 1 if so.

`aedilis.stml`: `<regula obiectum="tls_macos">` ->
`<regula obiectum="tls_securetransport_macos">`. Then
`./tools/compile_tests_fontes_generare.sh`.

- [ ] **Step 7: References to `tls_macos.m`**

`git grep -n tls_macos -- ':!*.md' ':!docs'` and fix each:
`probationes/probatio_silex.c:797-880` (the vitreum closure now holds
`lib/tls.c`, `lib/tls_securetransport_macos.m`, `lib/fiducia_macos.m`;
the assertion at `:805` names `tls_securetransport_macos.m`; the comment at
`:879` says Security comes via those), `lib/silex.c:2404,2524` and
`tools/silex_semen_fumus.sh:15` comments, `include/tls.h:4,28` (step 3).
The golden `probationes/fixa/stml_aurea/fontes/aedilis.stml.fons` is a
parser fixture, frozen input: leave it unless a gate compares it to the
live file (check `git grep -n 'aedilis.stml.fons'`).

- [ ] **Step 8: Run - expect PASS**

`./compile_tests.sh probatio_tls_vectura probatio_tls_servus
probatio_tls_gradus probatio_fiducia probatio_silex` → all green;
`./ludus_tessera/compile_probationes.sh` green (vicus links via http).

- [ ] **Step 9: Plants**

- `tls_vecturam_eligere`: `ambitus = getenv("RHUBARB_TLS");` ->
  `ambitus = NIHIL;` → "ambitus" red. Revert.
- `tls_vectura`: `connexio->vectura` -> `TLS_VECTURA_ORDINARIA` →
  "meminit" red. Revert.
- `_involvere_securetransport`: drop
  `r.connexio->securetransport = st.connexio;` (memset leaves NIHIL) →
  tls_servus parvum/obstruens red (no crash: every backend function
  refuses a NIHIL handle - if one crashes instead, that is a finding to
  fix in the backend). Revert.

- [ ] **Step 10: Commit**

`tls T3: lib/tls.c vectura per connexionem (TlsOptiones.vectura, RHUBARB_TLS); tls_macos.m -> tls_securetransport_macos.m (mores idem)`.
Expected owed gates: radix, silex, briar-fumus, ludus_tessera, examen.

---

### Task 4: `lib/tls_bearssl.c` - stepped path + validator (spec II.6)

**Files:**
- Create: `include/tls_bearssl.h`, `lib/tls_bearssl.c`,
  `lib/tls_bearssl.worklog.md`, `probationes/probatio_tls_bearssl.c`
- Modify: `lib/tls.c` (BearSSL branches)

**Interfaces:**
- Consumes: `bearssl.h` (T1), `fiducia_iudicare` / `FiduciaCertificatum`
  (T2), dispatcher shape (T3).
- Produces (internal, `tls_bearssl.h`): `TlsBearssl`, `TlsBearsslResultus
  {b32 successus; TlsBearssl* connexio; TlsError error; chorda
  error_descriptio;}`, `tls_bearssl_{incipere,gradus,gradus_error,mittere,
  recipere,claudere,est_valida}` (T4), `tls_bearssl_{connectere,
  certificatum}` (T5) - same contracts as tls.h.

- [ ] **Step 1: Write the failing test `probationes/probatio_tls_bearssl.c`**

Suite fixture as in `probatio_tls_servus.c:60-101` (certs + `parvum.txt`)
plus `setenv("RHUBARB_TLS", "bearssl", 1)` in `_suitam_parare` so
`http_vocare` goes through BearSSL. The sections:

```c
/* pactio gradatim per http_vocatio (RHUBARB_TLS=bearssl): 200 + corpus */
interior vacuum _sectio_parvum (CredoContextus* c);      /* = tls_servus parvum */
/* ECDHE_RSA: folium RSA-2048 (R12) */
interior vacuum _sectio_rsa (CredoContextus* c);         /* parvum, "localhost_rsa" */
/* reiecta: causa fiduciae nominata ('certificatum reiectum') */
interior vacuum _sectio_alienus (CredoContextus* c);     /* _reiectum alienus */
interior vacuum _sectio_ignotus (CredoContextus* c);     /* _reiectum ignotus */
interior vacuum _sectio_sine_ancora (CredoContextus* c); /* _reiectum localhost, sine ancora */
```

(`_petere`, `_reiectum`, `_rivus`, `_finis`, `_ms_nunc` copied from
`probatio_tls_servus.c:43-253`; `_reiectum` additionally asserts
`_continet(res.error_descriptio, "certificatum reiectum")`.) And the
direct API sections, full code:

```c
/* fossa non-blocans: tcp_connectere + O_NONBLOCK */
interior TcpConnexio*
_tcp_non_obstruens (
         i32  portus,
    Piscina* p)
{
    TcpResultus r = tcp_connectere("127.0.0.1", portus, p);
        integer f;

    si (!r.successus)
    {
        redde NIHIL;
    }
    f = (integer)tcp_obtinere_fd(r.connexio);
    (vacuum)fcntl(f, F_SETFL, fcntl(f, F_GETFL, ZEPHYRUM) | O_NONBLOCK);
    redde r.connexio;
}

/* exspectare fossam (POLLIN/POLLOUT) usque ad CC ms */
interior vacuum
_exspectare (
    TlsConnexio* t,
    TcpConnexio* tcp,
            b32  scribere)
{
    structura pollfd pf;

    (vacuum)t;
    pf.fd      = (integer)tcp_obtinere_fd(tcp);
    pf.events  = (brevis)(scribere ? POLLOUT : POLLIN);
    pf.revents = ZEPHYRUM;
    (vacuum)poll(&pf, I, CC);
}

/* pactio gradatim directa: tls_gradus usque ad PERFECTUS; GET; lectio
 * usque ad 0; vectura BEARSSL memorata */
interior vacuum
_sectio_gradus (
    CredoContextus* c)
{
    constans Locus* l = (constans Locus*)c->suita;
         ServusTls  s;
       TlsOptiones  opt = tls_optiones_default();
       TlsResultus  tr;
      TcpConnexio* tcp;
         TlsGradus  g = TLS_GRADUS_LEGERE;
         character  petitio[] = "GET /parvum.txt HTTP/1.0\r\n\r\n";
         character  capsa[IV * MXXIV];
               i32  missa = ZEPHYRUM;
               i32  lecta = ZEPHYRUM;
               s32  n;
               i32  k;

    opt.vectura    = TLS_VECTURA_BEARSSL;
    opt.ancora_via = l->ancora;
    CREDO_NECESSE(servum_tls_incipere(&s, l->dir, l->fixa, "localhost",
        c->piscina));
    tcp = _tcp_non_obstruens(s.portus, c->piscina);
    CREDO_NECESSE_NON_NIHIL(tcp);
    tr = tls_incipere(tcp, "localhost", &opt, c->piscina);
    CREDO_NECESSE(tr.successus);
    CREDO_AEQUALIS_I32((i32)tls_vectura(tr.connexio),
        (i32)TLS_VECTURA_BEARSSL);
    per (k = ZEPHYRUM; k < D && g != TLS_GRADUS_PERFECTUS
         && g != TLS_GRADUS_ERROR; k++)
    {
        g = tls_gradus(tr.connexio);
        si (g == TLS_GRADUS_LEGERE || g == TLS_GRADUS_SCRIBERE)
        {
            _exspectare(tr.connexio, tcp, g == TLS_GRADUS_SCRIBERE);
        }
    }
    CREDO_NECESSE(g == TLS_GRADUS_PERFECTUS);
    per (k = ZEPHYRUM; k < D && missa < (i32)strlen(petitio); k++)
    {
        n = tls_mittere(tr.connexio, (constans i8*)petitio + missa,
            (i32)strlen(petitio) - missa);
        si (n == TLS_ITERUM)
        {
            _exspectare(tr.connexio, tcp, VERUM);
            perge;
        }
        CREDO_NECESSE(n > ZEPHYRUM);
        missa += (i32)n;
    }
    per (k = ZEPHYRUM; k < D; k++)
    {
        n = tls_recipere(tr.connexio, (i8*)capsa + lecta,
            (i32)magnitudo(capsa) - lecta - I);
        si (n == TLS_ITERUM)
        {
            _exspectare(tr.connexio, tcp, FALSUM);
            perge;
        }
        si (n <= ZEPHYRUM)
        {
            frange;
        }
        lecta += (i32)n;
    }
    CREDO_AEQUALIS_I32((i32)n, ZEPHYRUM);    /* finis mundus */
    capsa[lecta] = '\0';
    CREDO_VERUM(strstr(capsa, "salve per tls\n") != NIHIL);
    tls_claudere(tr.connexio);
    CREDO_FALSUM(tls_est_valida(tr.connexio));
    tls_claudere(tr.connexio);               /* iterum: tutum */
    servum_tls_claudere(&s);
}
```

```c
/* verificatio omissa: CA ignota sine ancora, tamen PERFECTUS */
interior vacuum
_sectio_omissa (
    CredoContextus* c)
{
    constans Locus* l = (constans Locus*)c->suita;
         ServusTls  s;
       TlsOptiones  opt = tls_optiones_default();
       TlsResultus  tr;
      TcpConnexio* tcp;
         TlsGradus  g = TLS_GRADUS_LEGERE;
               i32  k;

    opt.vectura                 = TLS_VECTURA_BEARSSL;
    opt.verificare_certificatum = FALSUM;
    CREDO_NECESSE(servum_tls_incipere(&s, l->dir, l->fixa, "ignotus",
        c->piscina));
    tcp = _tcp_non_obstruens(s.portus, c->piscina);
    CREDO_NECESSE_NON_NIHIL(tcp);
    tr = tls_incipere(tcp, "localhost", &opt, c->piscina);
    CREDO_NECESSE(tr.successus);
    per (k = ZEPHYRUM; k < D && g != TLS_GRADUS_PERFECTUS
         && g != TLS_GRADUS_ERROR; k++)
    {
        g = tls_gradus(tr.connexio);
        si (g == TLS_GRADUS_LEGERE || g == TLS_GRADUS_SCRIBERE)
        {
            _exspectare(tr.connexio, tcp, g == TLS_GRADUS_SCRIBERE);
        }
    }
    CREDO_AEQUALIS_I32((i32)g, (i32)TLS_GRADUS_PERFECTUS);
    tls_claudere(tr.connexio);
    servum_tls_claudere(&s);
}

/* par in pactione claudit: ERROR nominatum, numquam pendet */
interior vacuum
_sectio_clausa_a_pare (
    CredoContextus* c)
{
    TcpServusResultus  sr = tcp_servus_creare_cum_optionibus("127.0.0.1",
        ZEPHYRUM, NIHIL, c->piscina);
          TcpResultus  acceptum;
          TlsOptiones  opt = tls_optiones_default();
          TlsResultus  tr;
         TcpConnexio* tcp;
            TlsGradus  g = TLS_GRADUS_LEGERE;
                  i32  k;

    CREDO_NECESSE(sr.successus);
    CREDO_NECESSE(tcp_servus_auscultare(sr.servus, IV) == TCP_OK);
    tcp = _tcp_non_obstruens(tcp_servus_obtinere_portum(sr.servus),
        c->piscina);
    CREDO_NECESSE_NON_NIHIL(tcp);
    acceptum = tcp_servus_accipere(sr.servus, c->piscina);
    CREDO_NECESSE(acceptum.successus);
    tcp_claudere(acceptum.connexio);       /* par abit */
    opt.vectura = TLS_VECTURA_BEARSSL;
    tr = tls_incipere(tcp, "localhost", &opt, c->piscina);
    CREDO_NECESSE(tr.successus);
    per (k = ZEPHYRUM; k < D && g != TLS_GRADUS_PERFECTUS
         && g != TLS_GRADUS_ERROR; k++)
    {
        g = tls_gradus(tr.connexio);
        si (g == TLS_GRADUS_LEGERE || g == TLS_GRADUS_SCRIBERE)
        {
            _exspectare(tr.connexio, tcp, g == TLS_GRADUS_SCRIBERE);
        }
    }
    CREDO_AEQUALIS_I32((i32)g, (i32)TLS_GRADUS_ERROR);
    CREDO_VERUM(tls_gradus_error(tr.connexio).mensura > ZEPHYRUM);
    tls_claudere(tr.connexio);
    tcp_servus_claudere(sr.servus);
}
```

(If `tcp_servus_creare_cum_optionibus` refuses NIHIL options, pass
`tcp_servus_optiones_default()` by address as `servus_tls.h:36` does;
`tcp_servus_accipere` may need the accept retry helper from
`probatio_http_vocatio.c` (`_accipere`, poll + retry) - reuse it.)

Includes: `postulata_posix.h`, latina, piscina, chorda,
chorda_aedificator, credo, reactor, http, http_vocatio, tls, tcp,
servus_tls.h, `<fcntl.h> <poll.h> <stdio.h> <stdlib.h> <string.h>
<unistd.h> <sys/time.h>`. Table: parvum, rsa, alienus, ignotus,
sine ancora, gradus, omissa, clausa a pare. principale:
`credo_suitam_currere_cum("tls_bearssl", SECTIONES, _suitam_parare,
_suitam_purgare)` where `_suitam_purgare` unsets `RHUBARB_TLS`.

- [ ] **Step 2: Run - expect FAIL**

Expected: every section fails at `tr.successus` / IRRITA with
"vectura bearssl nondum" (T3's dispatcher branch).

- [ ] **Step 3: Write `include/tls_bearssl.h`**

Same shape as `tls_securetransport.h` with `TlsBearssl`,
`TlsBearsslResultus` and `tls_bearssl_` names (header comment: "VECTURA
BearSSL, INTERNA: solum lib/tls.c includit; machina TLS 1.2 BearSSL
(vendor/bearssl.c), fiducia per fiducia.h"), including the T5 pair now
(`tls_bearssl_connectere`, `tls_bearssl_certificatum`) so the header is
written once; T5 implements them.

- [ ] **Step 4: Write `lib/tls_bearssl.c`**

```c
/* tls_bearssl.c - VECTURA BearSSL (tls-bearssl-spec II.6)
 *
 * Machina BearSSL (vendor/bearssl.c) est machina status pura: quattuor
 * capsae (sendrec, recvrec, sendapp, recvapp). _antlia eam per
 * br_ssl_engine_current_state agit super tcp_mittere/tcp_recipere -
 * EADEM pro modo gradatim (fossa non-blocans: 'exspecta' = LEGERE/
 * SCRIBERE) et obstruente (fossa cum moris tcp: 'exspecta' = tempus
 * excessum, regula R9).
 *
 * Fiducia: validator br_x509_class proprius catenam DER colligit,
 * clavem folii per br_x509_decoder legit (get_pkey), in end_chain
 * fiducia_iudicare vocat (platforma iudicat; regula decisionis A).
 * verificare_certificatum FALSUM: clavis legitur, fiducia omittitur.
 *
 * Limen: TLS 1.2 solum; sex cifrae (ECDHE-ECDSA/RSA x AES-GCM /
 * ChaCha20-Poly1305) - idem ac SecureTransport (_cifra_admissa).
 * Nihil extra piscinam dimittendum (classis R5 hic nequit). */
#include "postulata_posix.h"
#include "bearssl.h"    /* ANTE latina.h: numeri Romani nomina
                         * parametrorum vendoris frangerent */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "tcp.h"
#include "tls.h"
#include "fiducia.h"
#include "tls_bearssl.h"
#include <stdio.h>
#include <string.h>

#define CATENA_MAXIMA         XVI
#define CERTIFICATUM_MAXIMUM  (XVI * MXXIV)

/* validator: vtable PRIMUM (BearSSL &validator.vtable tradit) */
nomen structura {
    constans br_x509_class* vtable;
               TlsBearssl* connexio;
} Validator;

nomen enumeratio {
    ANTLIA_PARATA = 0,   /* status quaesitus adest */
    ANTLIA_LEGERE,       /* fossa legibilis exspectanda (obstruens:
                          * tempus excessum) */
    ANTLIA_SCRIBERE,     /* fossa scribibilis exspectanda */
    ANTLIA_CLAUSA,       /* clausura munda (close_notify, aut EOF post
                          * pactionem - ut SecureTransport) */
    ANTLIA_ERROR         /* connexio->erratum nominat */
} Antlia;

structura TlsBearssl {
     br_ssl_client_context  cc;
                 Validator  validator;
   br_x509_decoder_context  decoder;     /* clavis folii (get_pkey) */
             unsigned char* capsa;       /* BR_SSL_BUFSIZE_BIDI */
               TcpConnexio* tcp;
                   Piscina* piscina;
        constans character* hospes;
        constans character* ancora_via;  /* NIHIL = repositorium */
                       b32  verificare;
                       b32  pacta;       /* pactio perfecta */
                       b32  clausa;
                       b32  reiecta;     /* fiducia catenam reiecit */
       FiduciaCertificatum  catena[CATENA_MAXIMA];
                       i32  numerus;     /* certificata perfecta */
                        i8* scribendum;  /* certificatum currens */
                       i32  exspectata;  /* longitudo eius */
                       i32  scripta;
                       b32  nimis;       /* catena/certificatum nimium */
                    size_t  pendens;     /* mittere: octeti accepti,
                                          * recorda nondum missa */
                    chorda  erratum;
};

/* ---- validator (br_x509_class) ---- */
/* (BearSSL's parameter name 'server_name' -> 'titulus_servi': never
 * 'nomen*' - nomen is the typedef macro) */

interior vacuum
_v_initium_catenae (
    constans br_x509_class** ctx,
        constans character* titulus_servi)
{
    TlsBearssl* c = ((Validator*)(vacuum*)ctx)->connexio;

    (vacuum)titulus_servi;
    c->numerus = ZEPHYRUM;
    c->nimis   = FALSUM;
    c->reiecta = FALSUM;
}

interior vacuum
_v_initium_certificati (
    constans br_x509_class** ctx,
                   uint32_t  longitudo)
{
    TlsBearssl* c = ((Validator*)(vacuum*)ctx)->connexio;

    c->scribendum = NIHIL;
    si (c->nimis || c->numerus >= CATENA_MAXIMA
        || longitudo > CERTIFICATUM_MAXIMUM || longitudo == ZEPHYRUM)
    {
        c->nimis = VERUM;
        redde;
    }
    c->scribendum = (i8*)piscina_allocare(c->piscina, (i64)longitudo);
    c->exspectata = (i32)longitudo;
    c->scripta    = ZEPHYRUM;
    si (c->scribendum == NIHIL)
    {
        c->nimis = VERUM;
        redde;
    }
    si (c->numerus == ZEPHYRUM)
    {
        br_x509_decoder_init(&c->decoder, NIHIL, NIHIL);
    }
}

interior vacuum
_v_appendere (
    constans br_x509_class** ctx,
     constans unsigned char* octeti,
                     size_t  mensura)
{
    TlsBearssl* c = ((Validator*)(vacuum*)ctx)->connexio;
        size_t  capax;

    si (c->nimis || c->scribendum == NIHIL)
    {
        redde;
    }
    capax = (size_t)(c->exspectata - c->scripta);
    si (mensura > capax)
    {
        c->nimis = VERUM;
        redde;
    }
    memcpy(c->scribendum + c->scripta, octeti, mensura);
    c->scripta += (i32)mensura;
    si (c->numerus == ZEPHYRUM)
    {
        br_x509_decoder_push(&c->decoder, octeti, mensura);
    }
}

interior vacuum
_v_finis_certificati (
    constans br_x509_class** ctx)
{
    TlsBearssl* c = ((Validator*)(vacuum*)ctx)->connexio;

    si (c->nimis || c->scribendum == NIHIL
        || c->scripta != c->exspectata)
    {
        c->nimis = VERUM;
        redde;
    }
    c->catena[c->numerus].octeti  = c->scribendum;
    c->catena[c->numerus].mensura = c->scripta;
    c->numerus++;
    c->scribendum = NIHIL;
}

interior unsigned
_v_finis_catenae (
    constans br_x509_class** ctx)
{
    TlsBearssl* c = ((Validator*)(vacuum*)ctx)->connexio;

    si (c->nimis || c->numerus == ZEPHYRUM)
    {
        c->erratum = chorda_ex_literis(
            "catena certificatorum nimis magna aut vacua", c->piscina);
        c->reiecta = VERUM;
        redde BR_ERR_X509_LIMIT_EXCEEDED;
    }
    si (br_x509_decoder_get_pkey(&c->decoder) == NIHIL)
    {
        c->erratum = chorda_ex_literis("clavis folii legi non potest",
            c->piscina);
        c->reiecta = VERUM;
        redde BR_ERR_X509_UNSUPPORTED;
    }
    si (c->verificare && !fiducia_iudicare(c->catena, c->numerus,
            c->hospes, c->ancora_via, &c->erratum, c->piscina))
    {
        c->reiecta = VERUM;
        redde BR_ERR_X509_NOT_TRUSTED;
    }
    redde ZEPHYRUM;
}

interior constans br_x509_pkey*
_v_clavis (
    constans br_x509_class* constans* ctx,
                          unsigned* usus)
{
    TlsBearssl* c = ((constans Validator*)(constans vacuum*)ctx)->connexio;

    si (usus != NIHIL)
    {
        *usus = BR_KEYTYPE_KEYX | BR_KEYTYPE_SIGN;
    }
    redde br_x509_decoder_get_pkey(&c->decoder);
}

hic_manens constans br_x509_class VALIDATOR_CLASSIS = {
    magnitudo(Validator),
    _v_initium_catenae,
    _v_initium_certificati,
    _v_appendere,
    _v_finis_certificati,
    _v_finis_catenae,
    _v_clavis
};
```

(The casts through `vacuum*` keep `-Wcast-qual` quiet: the pointee of
`const br_x509_class **` is a non-const pointer. `get_pkey`'s
`const * const *` stays const all the way. If clang still objects,
the executor rules on the minimal cast and ledgers it.)

```c
/* ---- nomina errorum ---- */

interior constans character*
_erratum_machinae (
    integer  codex)
{
    hic_manens character capsa[XCVI];

    commutatio (codex)
    {
        casus BR_ERR_BAD_VERSION:
        casus BR_ERR_UNSUPPORTED_VERSION:
            redde "versio TLS non admissa (solum TLS 1.2)";
        casus BR_ERR_BAD_CIPHER_SUITE:
            redde "cifra non admissa";
        casus BR_ERR_BAD_SIGNATURE:
            redde "subscriptio hospitis falsa";
        casus BR_ERR_BAD_MAC:
            redde "MAC recordi falsum";
        casus BR_ERR_NO_RANDOM:
            redde "entropia systematis deest";
        casus BR_ERR_IO:
            redde "I/O fossae fallita";
        ordinarius:
            frange;
    }
    si (codex >= BR_ERR_SEND_FATAL_ALERT)
    {
        sprintf(capsa, "nuntius fatalis missus: %d",
            codex - BR_ERR_SEND_FATAL_ALERT);
    }
    alioquin si (codex >= BR_ERR_RECV_FATAL_ALERT)
    {
        sprintf(capsa, "nuntius fatalis a pare: %d",
            codex - BR_ERR_RECV_FATAL_ALERT);
    }
    alioquin
    {
        sprintf(capsa, "BearSSL error %d", codex);
    }
    redde capsa;
}
```

`hic_manens character capsa[]` is file-lifetime scratch. It must be
copied at once: every caller does `chorda_ex_literis(_erratum_machinae(e),
piscina)` immediately (single-threaded house, R2 of http-vocatio).

```c
/* ---- antlia ---- */

interior vacuum
_errorem_ponere (
          TlsBearssl* c,
    constans character* nuntius)
{
    si (c->erratum.mensura == ZEPHYRUM)
    {
        c->erratum = chorda_ex_literis(nuntius, c->piscina);
    }
}

/* machinam agere donec status 'sitis' adest, aut exspectandum, aut
 * clausa/error. Recorda mittenda SEMPER primum (BearSSL doctrina). */
interior Antlia
_antlia (
    TlsBearssl* c,
      unsigned  sitis)
{
    br_ssl_engine_context* e = &c->cc.eng;

    per (;;)
    {
        unsigned status = br_ssl_engine_current_state(e);

        si (status & BR_SSL_CLOSED)
        {
            integer err = br_ssl_engine_last_error(e);

            si (err == BR_ERR_OK)
            {
                redde ANTLIA_CLAUSA;
            }
            si (!c->reiecta)
            {
                _errorem_ponere(c, _erratum_machinae(err));
            }
            redde ANTLIA_ERROR;
        }
        si (status & BR_SSL_SENDREC)
        {
                   size_t  n;
            unsigned char* b = br_ssl_engine_sendrec_buf(e, &n);
                      s32  m = tcp_mittere(c->tcp, (constans i8*)b, (i32)n);

            si (m < ZEPHYRUM)
            {
                _errorem_ponere(c, "scriptio fossae fallita");
                redde ANTLIA_ERROR;
            }
            si (m == ZEPHYRUM)
            {
                redde ANTLIA_SCRIBERE;
            }
            br_ssl_engine_sendrec_ack(e, (size_t)m);
            perge;
        }
        si (status & sitis)
        {
            redde ANTLIA_PARATA;
        }
        si (status & BR_SSL_RECVREC)
        {
                   size_t  n;
            unsigned char* b = br_ssl_engine_recvrec_buf(e, &n);
                      s32  m = tcp_recipere(c->tcp, (i8*)b, (i32)n);

            si (m == TCP_ITERUM)
            {
                redde ANTLIA_LEGERE;
            }
            si (m == ZEPHYRUM)
            {
                /* EOF sine close_notify: post pactionem finis (ut
                 * errSSLClosedNoNotify SecureTransport); in pactione
                 * error */
                si (c->pacta)
                {
                    redde ANTLIA_CLAUSA;
                }
                _errorem_ponere(c, "connexio a pare clausa in pactione");
                redde ANTLIA_ERROR;
            }
            si (m < ZEPHYRUM)
            {
                _errorem_ponere(c, "lectio fossae fallita");
                redde ANTLIA_ERROR;
            }
            br_ssl_engine_recvrec_ack(e, (size_t)m);
            perge;
        }
        _errorem_ponere(c, "machina BearSSL immota");
        redde ANTLIA_ERROR;
    }
}
```

(`per (;;)` is the house's infinite loop spelling; if examen asks for
`dum (VERUM)`, use that.)

```c
/* ---- praeparatio ---- */

interior constans character*
_copiare (
    constans character* s,
               Piscina* piscina)
{
    size_t     n = strlen(s) + I;
    character* d = (character*)piscina_allocare(piscina, (i64)n);

    si (d != NIHIL)
    {
        memcpy(d, s, n);
    }
    redde d;
}

/* NIHIL si bene; aliter causa */
interior constans character*
_parare (
             TlsBearssl* c,
    constans character* hospes,
  constans TlsOptiones* optiones)
{
    hic_manens constans uint16_t CIFRAE[] = {
        BR_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256,
        BR_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256,
        BR_TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384,
        BR_TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384,
        BR_TLS_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256,
        BR_TLS_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256
    };
    br_ssl_engine_context* e = &c->cc.eng;

    c->hospes     = _copiare(hospes, c->piscina);
    c->ancora_via = (optiones && optiones->ancora_via)
        ? _copiare(optiones->ancora_via, c->piscina) : NIHIL;
    c->verificare = optiones ? optiones->verificare_certificatum : VERUM;
    c->capsa      = (unsigned char*)piscina_allocare(c->piscina,
        (i64)BR_SSL_BUFSIZE_BIDI);
    si (c->hospes == NIHIL || c->capsa == NIHIL
        || (optiones && optiones->ancora_via && c->ancora_via == NIHIL))
    {
        redde "Allocatio fallita";
    }
    si (c->ancora_via != NIHIL && access(c->ancora_via, R_OK) != ZEPHYRUM)
    {
        redde "ancora legi non potest";
    }
    br_ssl_client_zero(&c->cc);
    br_ssl_engine_set_versions(e, BR_TLS12, BR_TLS12);
    br_ssl_engine_set_suites(e, CIFRAE,
        magnitudo(CIFRAE) / magnitudo(CIFRAE[ZEPHYRUM]));
    br_ssl_engine_set_hash(e, br_sha256_ID, &br_sha256_vtable);
    br_ssl_engine_set_hash(e, br_sha384_ID, &br_sha384_vtable);
    br_ssl_engine_set_prf_sha256(e, &br_tls12_sha256_prf);
    br_ssl_engine_set_prf_sha384(e, &br_tls12_sha384_prf);
    br_ssl_engine_set_default_ec(e);
    br_ssl_engine_set_default_ecdsa(e);
    br_ssl_engine_set_default_rsavrfy(e);
    br_ssl_engine_set_default_aes_gcm(e);
    br_ssl_engine_set_default_chapol(e);
    c->validator.vtable   = &VALIDATOR_CLASSIS;
    c->validator.connexio = c;
    br_ssl_engine_set_x509(e, &c->validator.vtable);
    br_ssl_engine_set_buffer(e, c->capsa, BR_SSL_BUFSIZE_BIDI, I);
    si (!br_ssl_client_reset(&c->cc, c->hospes, ZEPHYRUM))
    {
        redde _erratum_machinae(br_ssl_engine_last_error(e));
    }
    redde NIHIL;
}

interior TlsBearsslResultus
_error (
              TlsError  error,
    constans character* nuntius,
              Piscina* piscina)
{
    TlsBearsslResultus r;

    r.successus        = FALSUM;
    r.connexio         = NIHIL;
    r.error            = error;
    r.error_descriptio = chorda_ex_literis(nuntius, piscina);
    redde r;
}

/* connexio nova super tcp (possessa: clauditur si defectus) */
interior TlsBearsslResultus
_creare (
            TcpConnexio* tcp,
     constans character* hospes,
   constans TlsOptiones* optiones,
                Piscina* piscina)
{
    TlsBearsslResultus  r;
            TlsBearssl* c;
   constans character* causa;

    si (tcp == NIHIL || hospes == NIHIL || piscina == NIHIL)
    {
        si (tcp)
        {
            tcp_claudere(tcp);
        }
        redde _error(TLS_ERROR_TCP, "Argumenta invalida", piscina);
    }
    c = (TlsBearssl*)piscina_allocare(piscina,
        (i64)magnitudo(TlsBearssl));
    si (c == NIHIL)
    {
        tcp_claudere(tcp);
        redde _error(TLS_ERROR_TCP, "Allocatio fallita", piscina);
    }
    memset(c, ZEPHYRUM, magnitudo(TlsBearssl));
    c->tcp     = tcp;
    c->piscina = piscina;
    causa      = _parare(c, hospes, optiones);
    si (causa != NIHIL)
    {
        tcp_claudere(tcp);
        redde _error(TLS_ERROR_HANDSHAKE, causa, piscina);
    }
    r.successus              = VERUM;
    r.connexio               = c;
    r.error                  = TLS_OK;
    r.error_descriptio.datum = NIHIL;
    r.error_descriptio.mensura = ZEPHYRUM;
    redde r;
}

/* ---- publica (tls_bearssl.h) ---- */

TlsBearsslResultus
tls_bearssl_incipere (
             TcpConnexio* tcp,
      constans character* hospes,
    constans TlsOptiones* optiones,
                 Piscina* piscina)
{
    redde _creare(tcp, hospes, optiones, piscina);
}

TlsGradus
tls_bearssl_gradus (
    TlsBearssl* c)
{
    si (c == NIHIL || c->clausa)
    {
        redde TLS_GRADUS_ERROR;
    }
    commutatio (_antlia(c, BR_SSL_SENDAPP))
    {
        casus ANTLIA_PARATA:
            c->pacta = VERUM;
            redde TLS_GRADUS_PERFECTUS;
        casus ANTLIA_LEGERE:
            redde TLS_GRADUS_LEGERE;
        casus ANTLIA_SCRIBERE:
            redde TLS_GRADUS_SCRIBERE;
        casus ANTLIA_CLAUSA:
            _errorem_ponere(c, "connexio clausa in pactione");
            redde TLS_GRADUS_ERROR;
        ordinarius:
            redde TLS_GRADUS_ERROR;
    }
}

chorda
tls_bearssl_gradus_error (
    TlsBearssl* c)
{
    chorda vacua;

    vacua.datum   = NIHIL;
    vacua.mensura = ZEPHYRUM;
    redde c ? c->erratum : vacua;
}

/* mittere: octeti in sendapp, flush, recorda effunduntur. Contractus
 * tls.h: TLS_ITERUM = idem iterum vocandum; octeti iam accepti (pendens)
 * tunc redduntur cum recorda eorum missa sunt (ut scriptum_pendens
 * SecureTransport) */
s32
tls_bearssl_mittere (
    TlsBearssl* c,
  constans i8* data,
          i32  mensura)
{
    br_ssl_engine_context* e;
                   Antlia  a;

    si (c == NIHIL || c->clausa)
    {
        redde -I;
    }
    si (data == NIHIL || mensura == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    e = &c->cc.eng;
    si (c->pendens == ZEPHYRUM)
    {
        a = _antlia(c, BR_SSL_SENDAPP);
        si (a == ANTLIA_LEGERE || a == ANTLIA_SCRIBERE)
        {
            redde TLS_ITERUM;
        }
        si (a != ANTLIA_PARATA)
        {
            redde -I;
        }
        {
                   size_t  capax;
            unsigned char* b = br_ssl_engine_sendapp_buf(e, &capax);
                   size_t  n = (size_t)mensura < capax ? (size_t)mensura
                                                       : capax;

            memcpy(b, data, n);
            br_ssl_engine_sendapp_ack(e, n);
            br_ssl_engine_flush(e, ZEPHYRUM);
            c->pendens = n;
        }
    }
    a = _antlia(c, BR_SSL_SENDAPP);   /* recorda effundere */
    si (a == ANTLIA_SCRIBERE || a == ANTLIA_LEGERE)
    {
        redde TLS_ITERUM;
    }
    si (a != ANTLIA_PARATA)
    {
        redde -I;
    }
    {
        size_t n = c->pendens;

        c->pendens = ZEPHYRUM;
        redde (s32)n;
    }
}
```

NOTE on `_antlia(c, BR_SSL_SENDAPP)` after a flush: SENDREC is set while
records are pending, so the loop writes them before it reports SENDAPP.
PARATA therefore means "nothing left to send". In gradatim, LEGERE can
come back only when the engine wants to READ before it can send (TLS 1.2
renegotiation is refused by BearSSL, so that is rare); the caller then
retries.

```c
s32
tls_bearssl_recipere (
    TlsBearssl* c,
           i8* buffer,
          i32  capacitas)
{
    br_ssl_engine_context* e;

    si (c == NIHIL || c->clausa)
    {
        redde -I;
    }
    si (buffer == NIHIL || capacitas == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    e = &c->cc.eng;
    commutatio (_antlia(c, BR_SSL_RECVAPP))
    {
        casus ANTLIA_PARATA:
        {
                   size_t  n;
            unsigned char* b = br_ssl_engine_recvapp_buf(e, &n);

            si (n > (size_t)capacitas)
            {
                n = (size_t)capacitas;
            }
            memcpy(buffer, b, n);
            br_ssl_engine_recvapp_ack(e, n);
            redde (s32)n;
        }
        casus ANTLIA_LEGERE:
        casus ANTLIA_SCRIBERE:
            redde TLS_ITERUM;   /* obstruens: tempus excessum (TCP_ITERUM) */
        casus ANTLIA_CLAUSA:
            c->clausa = VERUM;
            redde ZEPHYRUM;
        ordinarius:
            redde -I;
    }
}

vacuum
tls_bearssl_claudere (
    TlsBearssl* c)
{
    si (c == NIHIL)
    {
        redde;
    }
    si (!c->clausa && c->capsa != NIHIL && c->tcp != NIHIL)
    {
        br_ssl_engine_context* e = &c->cc.eng;
                          i32  k;

        /* close_notify, conatu uno: recorda pendentia semel mittuntur */
        br_ssl_engine_close(e);
        per (k = ZEPHYRUM; k < VIII
             && (br_ssl_engine_current_state(e) & BR_SSL_SENDREC); k++)
        {
                   size_t  n;
            unsigned char* b = br_ssl_engine_sendrec_buf(e, &n);
                      s32  m = tcp_mittere(c->tcp, (constans i8*)b,
                          (i32)n);

            si (m <= ZEPHYRUM)
            {
                frange;
            }
            br_ssl_engine_sendrec_ack(e, (size_t)m);
        }
    }
    si (c->tcp != NIHIL)
    {
        tcp_claudere(c->tcp);
        c->tcp = NIHIL;
    }
    c->clausa = VERUM;
}

b32
tls_bearssl_est_valida (
    TlsBearssl* c)
{
    redde c != NIHIL && !c->clausa && c->tcp != NIHIL
        && tcp_est_valida(c->tcp);
}
```

- [ ] **Step 5: Dispatcher branches in `lib/tls.c`**

`#include "tls_bearssl.h"`; the struct gains `TlsBearssl* bearssl;`;
`_involvere_bearssl` mirrors `_involvere_securetransport`; every
function switches on `connexio->vectura`:

```c
TlsGradus
tls_gradus (
    TlsConnexio* connexio)
{
    si (connexio == NIHIL)
    {
        redde TLS_GRADUS_ERROR;
    }
    si (connexio->vectura == TLS_VECTURA_BEARSSL)
    {
        redde tls_bearssl_gradus(connexio->bearssl);
    }
    redde tls_securetransport_gradus(connexio->securetransport);
}
```

(same pattern for gradus_error, mittere, recipere, claudere, est_valida;
`tls_incipere`'s `ordinarius` branch becomes
`casus TLS_VECTURA_BEARSSL: redde _involvere_bearssl(tls_bearssl_incipere(
tcp, hospes, optiones, piscina), piscina);`). The blocking
`tls_connectere_cum_optionibus` keeps "vectura bearssl nondum" until T5,
and certificatum returns FALSUM for BearSSL until T5.

- [ ] **Step 6: Run - expect PASS**

`./compile_tests.sh probatio_tls_bearssl` → 8/8. Also re-run
`probatio_tls_servus probatio_tls_gradus probatio_tls_vectura
probatio_fiducia` (unchanged: SecureTransport still the default).
Examen on `lib/tls_bearssl.c` ACCIPE (this is the real proof of R2/R3 on
a full backend).

- [ ] **Step 7: Plants** (each must turn the named sections red)

- `_parare`: `br_ssl_engine_set_versions(e, BR_TLS12, BR_TLS12);` ->
  `br_ssl_engine_set_versions(e, BR_TLS10, BR_TLS12);` - mute against
  s_server (which picks 1.2 anyway). MUTE BY DESIGN for an offline TLS
  1.2 server: the floor is proved by `tools/rete_fumus.c -badssl`
  (T6, network) and by T6's s_server run with `-tls1_1` (step 3 there).
  Record as such.
- `_v_finis_catenae`: `si (c->verificare && !fiducia_iudicare(` ->
  `si (FALSUM && !fiducia_iudicare(` → alienus, ignotus, sine ancora red.
- `_v_appendere`: drop the `br_x509_decoder_push` call → every handshake
  fails ("clavis folii legi non potest") → parvum, rsa, gradus, omissa red.
- `tls_bearssl_gradus`: the `casus ANTLIA_CLAUSA:` branch returns
  `TLS_GRADUS_LEGERE` instead of ERROR → "clausa a pare" runs its D
  bounded iterations and ends LEGERE ≠ ERROR → red, and never hangs.
  (Planting `si (c->pacta)` -> `si (VERUM)` in `_antlia` instead would be
  mute: gradus maps CLAUSA to ERROR as well.)
- `tls_bearssl_mittere`: `c->pendens = n;` -> `c->pendens = n - I;` →
  "gradus" red (request never fully counted).

- [ ] **Step 8: Commit**

`tls_bearssl T4: vectura BearSSL gradatim (antlia, validator br_x509_class -> fiducia), tls.c rami bearssl`.
Worklog `lib/tls_bearssl.worklog.md`: the pump order, why one pump serves
both modes, the BearSSL record-length fix in 7bea48e, the mute plants and
why.

---

### Task 5: blocking path + certificate details (spec II.6)

**Files:**
- Modify: `lib/tls_bearssl.c`, `lib/tls.c`,
  `probationes/probatio_tls_bearssl.c`

**Interfaces:**
- Produces: `tls_bearssl_connectere` (blocking, socket timeouts - R9),
  `tls_bearssl_certificatum` (leaf from the collected chain ->
  `fiducia_folium_describere`).

- [ ] **Step 1: Failing sections in `probatio_tls_bearssl.c`**

```c
/* obstruens: ancora -> successus; alienus -> TLS_ERROR_CERTIFICATUM;
 * certificatum folii legitur */
interior vacuum
_sectio_obstruens (
    CredoContextus* c)
{
    constans Locus* l = (constans Locus*)c->suita;
         ServusTls  s;
       TlsOptiones  opt = tls_optiones_default();
       TlsResultus  tr;
   TlsCertificatum  cert;
               s64  dies;

    opt.vectura    = TLS_VECTURA_BEARSSL;
    opt.ancora_via = l->ancora;
    opt.timeout_ms = V * M;
    CREDO_NECESSE(servum_tls_incipere(&s, l->dir, l->fixa, "localhost",
        c->piscina));
    tr = tls_connectere_cum_optionibus("localhost", s.portus, &opt,
        c->piscina);
    CREDO_NECESSE(tr.successus);
    CREDO_AEQUALIS_I32((i32)tls_vectura(tr.connexio),
        (i32)TLS_VECTURA_BEARSSL);
    CREDO_VERUM(tls_certificatum_obtinere(tr.connexio, &cert));
    CREDO_CHORDA_AEQUALIS_LITERIS(cert.subiectum, "localhost");
    dies = (cert.non_post - cert.non_ante) / ((s64)LXXXVI * M + CD);
    CREDO_VERUM(dies >= CCCLXIV && dies <= CCCLXVI);
    CREDO_VERUM(tls_mittere_omnia(tr.connexio,
        (constans i8*)"GET /parvum.txt HTTP/1.0\r\n\r\n", XXVIII));
    tls_claudere(tr.connexio);
    servum_tls_claudere(&s);
    CREDO_NECESSE(servum_tls_incipere(&s, l->dir, l->fixa, "alienus",
        c->piscina));
    tr = tls_connectere_cum_optionibus("localhost", s.portus, &opt,
        c->piscina);
    CREDO_FALSUM(tr.successus);
    CREDO_AEQUALIS_I32((i32)tr.error, (i32)TLS_ERROR_CERTIFICATUM);
    servum_tls_claudere(&s);
}

/* tempus: servus auscultat sed numquam respondet (backlog accipit
 * connexionem) -> TLS_ERROR_TCP intra ~moram, numquam pendet */
interior vacuum
_sectio_tempus (
    CredoContextus* c)
{
    TcpServusOptiones  so = tcp_servus_optiones_default();
    TcpServusResultus  sr = tcp_servus_creare_cum_optionibus("127.0.0.1",
        ZEPHYRUM, &so, c->piscina);
          TlsOptiones  opt = tls_optiones_default();
          TlsResultus  tr;
                  s64  initium;

    CREDO_NECESSE(sr.successus);
    CREDO_NECESSE(tcp_servus_auscultare(sr.servus, IV) == TCP_OK);
    opt.vectura    = TLS_VECTURA_BEARSSL;
    opt.timeout_ms = D;
    initium = _ms_nunc();
    tr = tls_connectere_cum_optionibus("localhost",
        tcp_servus_obtinere_portum(sr.servus), &opt, c->piscina);
    CREDO_FALSUM(tr.successus);
    CREDO_AEQUALIS_I32((i32)tr.error, (i32)TLS_ERROR_TCP);
    CREDO_VERUM(_ms_nunc() - initium < (s64)III * M);
    tcp_servus_claudere(sr.servus);
}
```

(Connecting to "localhost" may try ::1 first; the house test server binds
127.0.0.1 - if the connect to ::1 is refused quickly that is fine; if it
is slow, connect to "127.0.0.1" and note it.) Rows: `obstruens`,
`tempus`.

- [ ] **Step 2: Run - expect FAIL** ("vectura bearssl nondum").

- [ ] **Step 3: Implement**

`lib/tls_bearssl.c`:

```c
TlsBearsslResultus
tls_bearssl_connectere (
      constans character* hospes,
                     i32  portus,
    constans TlsOptiones* optiones,
                 Piscina* piscina)
{
         TcpOptiones  tcp_opt = tcp_optiones_default();
         TcpResultus  tcp_res;
  TlsBearsslResultus  r;

    si (hospes == NIHIL || piscina == NIHIL)
    {
        redde _error(TLS_ERROR_TCP, "Argumenta invalida", piscina);
    }
    si (optiones && optiones->timeout_ms > ZEPHYRUM)
    {
        tcp_opt.timeout_ms = optiones->timeout_ms;
    }
    tcp_res = tcp_connectere_cum_optionibus(hospes, portus, &tcp_opt,
        piscina);
    si (!tcp_res.successus)
    {
        r.successus        = FALSUM;
        r.connexio         = NIHIL;
        r.error            = TLS_ERROR_TCP;
        r.error_descriptio = tcp_res.error_descriptio;
        redde r;
    }
    r = _creare(tcp_res.connexio, hospes, optiones, piscina);
    si (!r.successus)
    {
        redde r;
    }
    commutatio (_antlia(r.connexio, BR_SSL_SENDAPP))
    {
        casus ANTLIA_PARATA:
            r.connexio->pacta = VERUM;
            redde r;
        casus ANTLIA_LEGERE:
        casus ANTLIA_SCRIBERE:
            tls_bearssl_claudere(r.connexio);
            redde _error(TLS_ERROR_TCP, "Tempus handshake excessum",
                piscina);
        ordinarius:
        {
            b32    reiecta = r.connexio->reiecta;
            chorda causa   = r.connexio->erratum;

            tls_bearssl_claudere(r.connexio);
            r.successus        = FALSUM;
            r.connexio         = NIHIL;
            r.error            = reiecta ? TLS_ERROR_CERTIFICATUM
                                         : TLS_ERROR_HANDSHAKE;
            r.error_descriptio = causa.mensura > ZEPHYRUM ? causa
                : chorda_ex_literis("pactio fallita", piscina);
            redde r;
        }
    }
}

b32
tls_bearssl_certificatum (
         TlsBearssl* c,
    TlsCertificatum* exitus)
{
    FiduciaFolium ff;

    si (exitus == NIHIL)
    {
        redde FALSUM;
    }
    memset(exitus, ZEPHYRUM, magnitudo(*exitus));
    si (c == NIHIL || c->clausa || c->numerus == ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (!fiducia_folium_describere(&c->catena[ZEPHYRUM], &ff, c->piscina))
    {
        redde FALSUM;
    }
    exitus->valida    = ff.valida;
    exitus->non_ante  = ff.non_ante;
    exitus->non_post  = ff.non_post;
    exitus->subiectum = ff.subiectum;
    redde exitus->valida;
}
```

`lib/tls.c`: `tls_connectere_cum_optionibus`'s BEARSSL branch calls
`_involvere_bearssl(tls_bearssl_connectere(...))`;
`tls_certificatum_obtinere` dispatches on vectura. Note that the
dispatcher's `tls_mittere_omnia` loop serves both backends. BearSSL's
blocking `tls_mittere` returns `TLS_ITERUM` only on a socket timeout; the
loop then retries every 1 ms, as SecureTransport's `n == 0` path did.

- [ ] **Step 4: Run - expect PASS**: `./compile_tests.sh
  probatio_tls_bearssl` → 10/10, plus the T4 set unchanged.

- [ ] **Step 5: Plants**

- `tls_bearssl_connectere`: `r.error = reiecta ? TLS_ERROR_CERTIFICATUM :
  TLS_ERROR_HANDSHAKE;` -> `r.error = TLS_ERROR_HANDSHAKE;` →
  "obstruens" red.
- `casus ANTLIA_LEGERE:` branch returning success instead
  (`r.connexio->pacta = VERUM; redde r;` copied into it) → "tempus" red.
- `tls_bearssl_certificatum`: drop `exitus->subiectum = ff.subiectum;` →
  "obstruens" red. (Indexing the last chain element instead of the leaf
  would be mute: s_server sends the leaf only, so the chain has one
  certificate. Record it in the worklog as needing an intermediate-CA
  fixture.)

- [ ] **Step 6: Commit**

`tls_bearssl T5: via obstruens (moris fossae, R9) + certificatum folii per fiduciam`.

---

### Task 6: matrix, leaks, BearSSL default, live check (spec III)

**Files:**
- Modify: `lib/tls.c` (`TLS_VECTURA_DOMUS` -> BEARSSL),
  `probationes/probatio_tls_vectura.c` (`VECTURA_DOMUS`),
  `probationes/probatio_tls_servus.c`, `probationes/probatio_tls_gradus.c`
  (principale runs both), `include/tls.h` (comment),
  `lib/tls_bearssl.worklog.md`, `project-specs/tls-bearssl-spec.md`
  (status line)

- [ ] **Step 1: Matrix (R10) - both suites run under both backends**

`probationes/probatio_tls_servus.c` principale:

```c
/* matrix (tls-bearssl R10): sectiones EAEDEM sub vectura utraque -
 * differentia = inventum */
s32
principale (vacuum)
{
    s32 bearssl;
    s32 securetransport;

    (vacuum)setenv("RHUBARB_TLS", "bearssl", I);
    bearssl = credo_suitam_currere_cum("tls_servus [bearssl]", SECTIONES,
        _suitam_parare, NIHIL);
    (vacuum)setenv("RHUBARB_TLS", "securetransport", I);
    securetransport = credo_suitam_currere_cum(
        "tls_servus [securetransport]", SECTIONES, _suitam_parare, NIHIL);
    (vacuum)unsetenv("RHUBARB_TLS");
    redde bearssl != ZEPHYRUM ? bearssl : securetransport;
}
```

Same in `probatio_tls_gradus.c` with `credo_suitam_currere`. The
`_sectio_obstruens` / `_sectio_certificatum` calls take
`tls_optiones_default()` (vectura ORDINARIA -> env) so they follow the
matrix too. Run both: every section passes twice. If a section passes
on one backend and fails on the other, STOP: that difference is a
finding (diagnose it; never weaken the assertion).

- [ ] **Step 2: Flip the default**

`lib/tls.c`: `#define TLS_VECTURA_DOMUS TLS_VECTURA_BEARSSL`;
`probatio_tls_vectura.c`: `#define VECTURA_DOMUS TLS_VECTURA_BEARSSL`.
Run `probatio_tls_vectura` - it must go red BEFORE the lib.c edit and
green after it (edit the test first).

- [ ] **Step 3: The TLS 1.2 floor against a 1.1-only server (closes the
  mute version plant from T4)**

`probationes/servus_tls.h` gets an optional extra argument string
(`servum_tls_incipere_cum(&s, dir, fixa, cert, "-tls1_1", p)` with the
old function calling it with `""`). New section in
`probatio_tls_servus.c` (so it runs in the matrix): `"tls11"` - server
`-tls1_1`, `_reiectum`-style assert that the connection fails with
`HTTP_ERROR_CONNEXIO` and a non-empty description (BearSSL: "versio TLS
non admissa"; SecureTransport: its OSStatus text). Re-run T4's version
plant: now red under bearssl. Revert.

(If Homebrew openssl refuses `-tls1_1` at its default security level,
add `-cipher 'DEFAULT:@SECLEVEL=0'`. If that still fails, the section
is skipped with a NOTA reason via `exspectata_fracta`, and the floor
rests on the network check in step 5.)

- [ ] **Step 4: Leaks, both backends**

`./tools/tls_effluvia.sh -aedificare` - the matrix makes each test binary
run both backends under `leaks --atExit`. Expected: zero non-tolerated
ROOT LEAK / ROOT CYCLE. A BearSSL leak would be OUR bug (no
CoreFoundation objects in tls_bearssl.c except via fiducia - check
fiducia's releases first).

- [ ] **Step 5: Network checks (by hand; not gates)**

```bash
./compile_tests.sh probatio_tls probatio_http_vocatio_rete   # builds (rete)
./build/probationes/probatio_tls                               # BearSSL default
RHUBARB_TLS=securetransport ./build/probationes/probatio_tls
./build/probationes/probatio_http_vocatio_rete
RHUBARB_TLS=securetransport ./build/probationes/probatio_http_vocatio_rete
bin/rete_fumus -tls ; bin/rete_fumus -badssl                    # or the built tools/rete_fumus
RHUBARB_TLS=securetransport bin/rete_fumus -badssl
```

Expected: identical verdict tables. `-badssl`: expired / wrong.host /
self-signed / untrusted-root / tls-v1-0 / tls-v1-1 / rc4 / 3des / null
REJECTED and the good hosts accepted under both. `-tls` (howsmyssl):
TLS 1.2, no insecure suites. A row that differs is a finding.

- [ ] **Step 6: Fran's live check**

Ask Fran to run vicus with `$question` (route B → http_vocatio → tls →
BearSSL by default) and, once, with `RHUBARB_TLS=securetransport`. Never
read the key; Fran reports.

- [ ] **Step 7: Close out**

- `include/tls.h` comment: implementation line names the dispatcher and
  "BearSSL ordinaria (RHUBARB_TLS=securetransport: vectura vetus,
  delenda postea)".
- Spec status line: "IMPLETUM (T1-T6)", with the commit range.
- Ledger: the wish …BWQS8H gets a note "step 0 done (commits ...)"; a
  new park "SecureTransport delenda" with the condition (a period with
  both, no differences).
- Worklog `lib/tls_bearssl.worklog.md`: matrix result, leaks result,
  network tables, what Fran saw.

- [ ] **Step 8: Commit**

`tls T6: BearSSL ordinaria; matrix vecturarum (tls_servus, tls_gradus), solum TLS 1.2 contra servum 1.1, effluvia utriusque`.
Owed gates as commissio computes them. Do NOT merge to main without
Fran's go.

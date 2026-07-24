# villa — spec v2, THE BUILD BLUEPRINT (2026-07-24)

Supersedes villa-spec.md for implementation (v1 remains the intent
record; raw Q&A in villa-interview.md; board desideratum
01KY0P5GG9). Produced from three explorer reports — transport,
scaffolding, world+gates. Line numbers as surveyed 2026-07-24;
re-verify before editing, but the seams are named precisely enough
to survive drift.

## 0. What exploration changed

1. **The house has NO process-spawn capability.** Not partial —
   none. Zero `exec*`, zero `popen`, zero `posix_spawn`; every
   shell-out is `system()`; the only two `fork()` sites never exec.
   `systema_posix.h` is **silva's parser lexicon** (a hand-written
   miniature header so `examen` can judge fork/pipe code without
   false positives), NOT a platform library. villa builds the spawn
   layer. This is v2's biggest cost line.
2. **TLS exposes no certificate at all** — connect/send/recv/close
   and a verify toggle. Cheap to add (SecureTransport already holds
   the trust object), and it forces a design change: **the external
   probe talks to `tls_connectere` directly, not `http_exsequi`.**
3. **CORRECTION to my earlier claim:** the http drain-loop defect is
   NOT a villa prerequisite. Because the probe goes direct to TLS
   (cert expiry is unreachable through http anyway), villa never
   calls `http_exsequi`. The bug is real, confirmed, and filed
   (desideratum **01KYANH7AN**) — but it is independent work.
4. **The shared client is free and gets a gate.** `lib/aedilis.c:479`
   maps `include/X.h → lib/X.c` automatically (verified), so
   extraction costs zero build config — and `probationes/` is swept
   by `compile_tests.sh` while **`apps/` is swept by nothing**.
5. **`apps/forum/fumus.sh` is referenced by no runner.** A villa
   fumus of the same shape would pass once and rot silently. The
   gate design changes accordingly (§IX).
6. **Reserved datum keys** would have silently broken the servus
   schema: `genus` and `titulus` are *stripped* from the datum merge
   and `status` projects to the lifecycle column.
7. **Actions must be `nota`, not `mutatio`** — `mutatio` is judged
   against the genus campi and would emit a custody note on every
   single restart.

## I. Prerequisites (build before villa; each ships with its own gate)

### P1 — `processus`: spawn with capture (NEW)

`include/processus.h` + `lib/processus_posix.c`, mirroring the
existing `tcp.h`/`tcp_posix.c` split. Implementation shape: the
`probationes/probatio_http.c:79-155` pipe+fork+waitpid pattern, plus
`execvp` and a `select()` drain over two pipes.

```c
nomen enumeratio {
    PROCESSUS_OK = 0,
    PROCESSUS_ERROR_GENERARE,   /* fork/pipe fallita */
    PROCESSUS_ERROR_TEMPUS,     /* mora excessa - occisus */
    PROCESSUS_ERROR_IO
} ProcessusError;

nomen structura {
    b32            successus;
    i32            codex_exitus;    /* WEXITSTATUS */
    i32            signum;          /* 0 nisi signo occisus */
    chorda         effusio;         /* stdout */
    chorda         erratum;         /* stderr - SEPARATIM */
    i32            mora_ms;
    ProcessusError error;
    chorda         error_descriptio;
} ProcessusResultus;

/* argumenta = vector NIHIL-terminatus; argumenta[0] = binarium.
 * NULLA concha - nulla citatio necessaria. */
ProcessusResultus processus_exsequi(
    constans character* constans* argumenta,
    i32 mora_maxima_ms, Piscina* piscina);
```

**Design calls that matter:**
- **argv vector, never `/bin/sh`.** This deletes the entire shell-
  quoting class — which is load-bearing precisely because of the
  arbitrary-command feature, the one place a quoting bug is a
  user-visible foot-gun. It also makes the command builder produce
  a vector, so its gate is a structural comparison rather than
  string diffing.
- **stderr separate.** ssh's diagnostics (BatchMode auth refusal,
  ConnectTimeout) land on stderr; villa must show them without
  polluting the remote command's stdout.
- **v1 is a strict SUBSET of the streaming version.** Collect-with-
  deadline over `select()` *is* the streaming loop with the callback
  elided, so the later `processus_incipere`/`_legere`/`_finire`
  triple (for log follow) is a new entry point over the same
  internals, not a rewrite. Do not build streaming now; do not
  design it out either.

Gate: `probationes/probatio_processus.c` — exit codes, stdout/stderr
separation, deadline kill, argv with spaces/quotes surviving intact
(`/bin/echo` and a small stub script suffice; no network).

### P2 — TLS peer certificate accessor (small addition)

`lib/tls_macos.m` already holds `SSLContextRef`. Path:
`SSLCopyPeerTrust` → `SecTrustGetCertificateAtIndex(0)` →
`SecCertificateCopyValues(kSecOIDX509V1ValidityNotAfter)` → CFDate →
epoch. Captured right after handshake while the context is live.
Add to `include/tls.h`:

```c
nomen structura {
    b32    valida;
    s64    non_ante;      /* epocha */
    s64    non_post;      /* epocha - dies reliqui hinc */
    chorda subiectum;
} TlsCertificatum;

b32 tls_certificatum_obtinere(TlsConnexio*, TlsCertificatum* exitus);
```

No vendored backend, Security.framework already linked. Gate: joins
the `-reticularis` set (needs a real handshake).

### P3 — `cliens_tabularii`: extract the daemon client (REFACTOR)

`include/cliens_tabularii.h` + `lib/cliens_tabularii.c`. ~337 lines
lifted from `apps/forum/forum.c`: `_ch`, `_chorda_est`,
`_conectere_aut_gignere` (:91-128), `_daemon_petere` (:132-198),
`_textus_ex_responso` (:203-244), `_litterae_ex` (:271-287),
`_res_id_ex_textu` (:290-323), `_instrumentum_vocare` (:327-366),
`_res_capere` (:369-394), `_daemon_transmittere` (:739-791).

```c
nomen structura {
    i32                 portus;
    s64                 petitio_index;
    constans character* actor;        /* "fran" */
    constans character* launcher;     /* "./gesta/tabulariumd.sh" */
    constans character* praefixum;    /* "[villa]" pro stderr */
} ClientTabularii;
```

- App coupling was only ever two fields (port, actor). The
  hardcoded `actor:"fran"` and the 4-name allowlist become
  parameters.
- **PRESERVE the `hic_manens b32 genitus` fast-fail**
  (`forum.c:102-107`): spawn the launcher once, then fail fast, so a
  dead daemon can't freeze the UI's poll behind a 5s blocking
  handler. This is a fixed bug — do not re-derive it.
- **Order: build villa against the extracted lib, convert forum
  afterward**, so forum stays green throughout.

Gate: `probationes/probatio_cliens_tabularii.c` — drives a real
ephemeral `tabulariumd` on `-portus 0` (the `probatio_fori.c:104-128`
pattern). This is coverage the client has never had.

## II. File plan

| path | what | new? |
|---|---|---|
| `include/processus.h` / `lib/processus_posix.c` | spawn+capture | **new (P1)** |
| `include/tls.h` / `lib/tls_macos.m` | cert accessor | edit (P2) |
| `include/cliens_tabularii.h` / `lib/cliens_tabularii.c` | daemon client | **new (P3)** |
| `include/villa_lectores.h` / `lib/villa_lectores.c` | **pure** parsers + ssh command builder | **new** |
| `apps/villa/villa.c` | app: main, handlers, probe scheduling | **new** |
| `apps/villa/assets/index.html` | console UI | **new** |
| `apps/villa/assets/villa.toml` + generated `capsula_villa.{h,c}` | embed | **new** |
| `apps/villa/villa.sh`, `apps/villa/fumus.sh` | launcher, smoke | **new** |
| `probationes/probatio_villa_lectores.c` | parser gates | **new** |
| `probationes/probatio_villa_agens.c` | seam+event gate (§IX) | **new** |
| `probationes/fixa/villa/*.txt` + `ssh_stipes.sh` | fixtures + stub ssh | **new** |

**Why parsers live in `lib/` and not `apps/villa/`:** root
`probationes/` is the only directory a bare `./compile_tests.sh`
sweeps, and a test there must link the parsers. `include/villa_
lectores.h` also gets the automatic `include/→lib/` resolution. The
parsers must NOT live inside the app's `principale` translation unit.

## III. Open questions — resolved

- **Q1 (spawn):** nothing exists; build P1. Incremental reads NOT in
  v1, but the v1 signature is a subset of the streaming one.
- **Q2 (cert expiry):** not reachable today; add P2. **Reject the
  v1 fallback of reading cert files over ssh** — it re-couples the
  external probe to ssh (destroying the "works when ssh is down"
  point) and reports what the box has on disk rather than what the
  world is served, which is exactly the misconfiguration case the
  probe exists to catch.
- **Q3 (http HTTPS):** the v1 inference was wrong. `probatio_http.c`
  runs against a local fork fixture over **plain http**; the
  httpbin URLs are parser-only asserts that never open a socket. The
  HTTPS success path has no live coverage. Moot for villa — the
  probe goes direct to TLS (§VI).
- **Q4 (transmittere reuse):** share via P3, parameterized.
- **Q5 (ControlPath):** `$HOME/.rhubarb/cm/%C`, mode 0700 (§V).
- **Q6 (genera collision):** app-layer discipline (§IV.2); machine
  hardening parked separately.
- **Q7 (fixtures):** capture twice, pin only if byte-identical;
  sanitize by hand; commit the sanitized file as truth (§IX.4).

## IV. The world

### 1. Seeded genera (exact JSON)

**RESERVED KEYS — never use as a campus clavis:** `genus`, `titulus`,
`corpus`, `tags`, `ancorae`, `signatura`, `status`, `notae`.
`genus`/`titulus` are stripped from the datum merge
(`tabularium.c:2694-2698`); `status` projects to the res lifecycle
column. Hence `nomen_servi`, not `titulus`.

```js
// servus
{ clavis: 'servus', titulus_monstrans: 'Servus',
  campi: [
    { clavis:'nomen_servi', monstrans:'Nomen',  typus:'textus'  },
    { clavis:'hospes',      monstrans:'Hospes', typus:'textus'  },
    { clavis:'usor',        monstrans:'Usor',   typus:'textus'  },
    { clavis:'portus',      monstrans:'Portus', typus:'numerus' },
    { clavis:'clavis_ssh',  monstrans:'Clavis', typus:'textus'  }],
  campus_tituli: 'nomen_servi',
  ordo_ordinarius: { campus:'nomen_servi', directio:'ascendens' } }

// servitium
{ clavis: 'servitium', titulus_monstrans: 'Servitium',
  campi: [
    { clavis:'nomen_servitii', monstrans:'Nomen',  typus:'textus' },
    { clavis:'unitas',         monstrans:'Unitas', typus:'textus' },
    { clavis:'servus', monstrans:'Servus', typus:'relatio',
      ad:'servus', cardinalitas:'unum' }],
  campus_tituli: 'nomen_servitii',
  ordo_ordinarius: { campus:'nomen_servitii', directio:'ascendens' } }
```

No default-value mechanism exists — port 22 defaulting is villa's UI
job. `commentarium` is a SYSTEM genus; villa does **not** seed it.

### 2. Collision discipline (the machine will not save us)

`_definitionem_proicere` (`gesta.c:1134-1179`) guards **only**
`usor=0`. A clavis already owned by another *user* genus is silently
`INSERT OR REPLACE`d — the other app's schema vanishes. Write-time
judgment does emit `"clavis generis iam occupata"`, but validation
**judges without blocking** and `addere`'s response is success-shaped,
so nothing surfaces it.

Three lines of discipline:
1. Check `legere('definitio')` for the clavis before seeding (the
   `scholia_genera_curare` pattern, `index.html:1803-1837`).
2. **Also inspect the `addere` response for `iam occupata`** and
   refuse loudly. This single `indexOf` converts a silent schema
   clobber into a visible error.
3. Never seed a clavis in the system-14.

`servus`/`servitium` are free today (verified against the live
registry: 14 system genera, 9 user genera, no conflict).

### 3. Actions as events — `nota`, with `origo`

**Use `actus:'nota'`, never `mutatio`.** `nota` falls through
`_eventum_validare` unjudged (`gesta.c:2391`); a `mutatio` carrying
action fields would be judged against `servus`'s campi and emit
`"clavis extra campos"` custody notes on **every restart** — unless
the schema were polluted with log fields, which inverts the model
(the schema describes the server, not the log).

`origo` is its own column in `tessellae` (not inside datum) and is
the correct provenance slot — the same one `seminatio` and
`gesta-custodia` use, and `acta`/`quaerere` can filter on it.

```js
instrumentum('gerere', { res: servus_id, actus: 'nota', origo: 'villa',
  textus: 'systemctl restart nginx — exitus 0 (2.1s)' });
instrumentum('gerere', { res: servus_id, actus: 'nota', origo: 'villa',
  textus: 'systemctl restart nginx — FRACTUM, exitus 5\n'
        + 'Failed to restart nginx.service: Unit not found.' });
```

If structured querying is later wanted, the answer is a separate
`actio_villae` genus with real campi — not `mutatio` on `servus`.

### 4. Comments

Use the **one-call** form (`addere {genus:'commentarium', corpus,
ad: servus_id}`), not forum's older two-call `_mittere`
(`forum.c:434-493`) which predates the `ad` param. Read via
`legere('commentarium')` filtered on `respondet_ad`. Title derives
from corpus automatically.

## V. Transport

```
ssh -o ControlMaster=auto -o ControlPath=<dir>/%C \
    -o ControlPersist=300 -o ConnectTimeout=5 -o BatchMode=yes \
    [-p port] [-i keyfile] user@host '<script>'
```

**ControlPath = `$HOME/.rhubarb/cm/%C`, mode 0700, created at
startup.** Measured: 72 bytes (31 to spare under macOS's 103-byte
usable `sun_path`). Rejected alternatives:
- `build/villa/%C` = **99 bytes** — one repo rename from silent
  failure, AND `build/` is actively `rm -rf`'d by
  `speculum_generare.sh:150` and the fumus scripts. A control socket
  deleted under ssh silently degrades every later command to a full
  handshake, with no error.
- `%C` (40-char hash) over `%r@%h:%p` because the latter is
  unbounded — a long FQDN or IPv6 literal blows the cap
  unpredictably.

This establishes the house's first machine-local runtime directory
outside the repo. The vault (v1 §IX) will want the same place.

**Gate the length in the pure command builder**: compute the
expanded path and **refuse loudly above ~100 bytes**. An oversize
ControlPath otherwise manifests only as unexplained latency, since
ssh reports the bind failure only under `-v`.

**The ssh binary is a seam, read once:**
```c
constans character* e = getenv("VILLA_SSH");
via_ssh = (e != NIHIL) ? e : "ssh";
```
Stored in the config struct at load. Config path and test path are
**the same seam, not two** — following the blessed `RHUBARB_RADIX`
idiom (the only env-var redirect precedent in the house).

`BatchMode=yes` is non-negotiable (no interactive prompt can hang
the UI). `known_hosts` trust is a precondition; villa refuses with a
named cause rather than inventing a TOFU flow.

## VI. External probe — direct to TLS

`tls_connectere` + a hand-written one-line GET (the
`probatio_tls.c:111-154` shape), **not** `http_exsequi`. Three
reasons: cert expiry is unreachable through http (which closes the
connection before returning); http has no caller-settable deadline;
and http's HTTPS success path has never run live. villa needs only
status line + elapsed ms + cert notAfter for one fixed GET per site.

**Neither probe may run on the UI thread**: there is no connect
timeout (blocking `connect()`, `tcp_posix.c:196`) and no DNS timeout
(unbounded `getaddrinfo`, `:180`). Probe work happens in the app's
per-tick slot with its own budget, never inline in an internuntius
handler.

## VII. Scaffolding checklist (ordered — 2 and 3 precede 4)

0. **Prereq tools**: `bin/capsula_generare` is built only by
   `compile_tests.sh:434`; `bin/aedilis` by `tools/aedilis_struere.sh`.
   Add a guard to villa.sh: `[ -x bin/aedilis ] || ./tools/aedilis_struere.sh || exit 1`.
1. **`aedilis.stml` `<inclusa>`** += `apps/villa` and `apps/villa/assets`
   (both — the assets entry is needed for the capsula header).
2. **`apps/villa/assets/villa.toml`**: `villa_files = ["index.html"]`,
   **`villa_compress = true`** (forum left it false and pays a
   committed 640 KB `capsula_forum.c` on every UI edit).
3. **`./bin/capsula_generare apps/villa/assets/villa.toml`** — and
   unlike forum, **put this in villa.sh**. forum.sh omits it, which
   is why editing index.html and relaunching shows stale UI.
4. **`tools/speculum_generare.sh villa apps/villa/villa.c apps/villa/villa.sh`**
5. **`apps/villa/villa.sh`** — forum's, plus step 3, keeping
   **`rm -f bin/villa` before `cp`** (overwriting a signed Mach-O in
   place invalidates the ad-hoc signature → SIGKILL).
6. **`apps/villa/fumus.sh`** — forum's shape, scratch
   `build/fumus_villae`, plus `VILLA_SSH=<stub>` exported.
7. **Not touched**: `compile_tests.sh` (auto-globs `probationes/`),
   `.gitignore`, no `*_fontes_generata.sh`. `MAP.txt` has zero
   `apps/` references — a pre-existing gap worth one line for both
   apps at some point.

**Generated `capsula_villa.{h,c}` are COMMITTED** (they live in
`assets/` to escape the `build/` ignore).

## VIII. App structure and UI

**Two-arena discipline** (`forum.c:823-826`): `piscina` (app
lifetime) and `piscina_vocationis` (reset every tick, at
`:976`/`:1053`). **Probe snapshots must NOT live in
`piscina_vocationis`** — they survive ticks. Put them in `piscina`
or a dedicated third arena.

Boilerplate to copy: the fenestra→capsula→vitrea→internuntius chain
(`forum.c:892-927`), the speculum extern annotation (`:39-43`), the
drain loop (`:964-1054`) — with villa's probe scheduling in the
app-specific per-tick slot. Cleanup destroys only vitrea + fenestra
+ piscinae; capsula/inx/speculum are arena-owned.

**Load-bearing UI conventions:**
- Diff-poll: serialize per view into `ultima`, re-render only on
  change, `anquirere_nunc()` after every write.
- `arca_activa()` is **opt-in** via `data-inter="verum"` — mark only
  inputs inside re-rendered containers (villa: the command box, log
  count). Whole-view poll exemption for the add-host form and
  command composer.
- Error surface: one `#nuntius-erroris` line, `culpam_ostendere`,
  and **clear-on-success** in every poll branch. villa needs a
  *separate* per-server staleness element — the transient error line
  must not clear the "ultima probatio 14 min" marker.
- Build the **nested** `vertere` / `villa_vertere` two-level switch
  from the start (list → detail → logs); forum had to retrofit it
  three times.
- Theme block verbatim; use the `body.className = 'latus'` widening
  trick for the services/sites tables.
- Fumus hook: `fumus_modus` → choreography → `fumus_perfectus`, with
  the step budget (`FUMUS_GRESSUS_MAXIMI` × 200ms). Raise it —
  villa's chain (add → discover → probe → render → restart → assert)
  is longer than forum's.

## IX. Gates

### 1. Parser gates — `probationes/probatio_villa_lectores.c`

Pure functions against committed fixtures: `systemctl is-active` /
`show`, `nginx -T` (server_name → proxy_pass → cert path),
`df -P`, `free -m`, journalctl framing, **and the ssh command
builder** (flags, port, key, ControlPath length refusal, argv
element boundaries).

**Fixture trap**: `_plagula_litterae` returns `""` for a missing
file, so a typo'd path yields empty input and a parser returning
"0 services" **passes**. Every fixture test opens with
`CREDO_VERUM(strlen(fx) > 0)`.

### 2. Seam + event gate — `probationes/probatio_villa_agens.c`

**This is the change forced by exploration.** `apps/forum/fumus.sh`
is referenced by no runner, no Makefile, no hook — a villa fumus of
the same shape would pass the day it was written and rot in silence
(exactly what `officina/captare.sh:57-62` warns about: a gate that
cannot go green gets ignored, and the instrument quietly rots).

So the *headless* half of the bar moves into the swept suite: a C
test that sets `VILLA_SSH` to the stub, runs the probe path
end-to-end, spins a real ephemeral `tabulariumd` on `-portus 0` (the
`probatio_fori.c:104-128` pattern proves this works from a C test),
performs a restart action, and asserts the `nota` event with
`origo:"villa"` landed in the annales. Only WebView *rendering*
assertions stay in the unswept `fumus.sh`.

### 3. Stub ssh — `probationes/fixa/villa/ssh_stipes.sh`

No precedent exists for injecting a fake binary; `VILLA_SSH` follows
the `RHUBARB_RADIX` idiom. The stub ignores ssh flags, inspects the
tail of argv (the remote script), and `cat`s the matching fixture.
Because it is `exec`'d directly, no `PATH` manipulation is needed
and a real `ssh` can never be shadowed.

### 4. Fixture capture discipline (from `officina/captare.sh`)

Capture from the real droplet, **run each capture twice, pin only if
byte-identical**; anything nondeterministic gets named as an
explicit exclusion rather than quietly making the gate flaky —
`nginx -T`, `df`, and `journalctl` all embed volatile content.
Sanitize **by hand** and commit the sanitized file as the truth
(never a script that re-sanitizes at test time), with a README
naming what was redacted.

**Capture more shapes than seem necessary while the box is in front
of you**: an inactive unit, a failed unit, a server block with no
`proxy_pass`, a full disk, an unresolvable cert path. The parsers
are pure functions and the fixtures are the entire specification of
what they must survive.

### 5. Lived bar

Point villa at the real droplet: smaragda and nginx as tracked
services; real sites with upstreams and cert days; tail a real log;
restart something harmless; find the action in the ledger; write an
operational note on the server and read it back over MCP.

## X. Build order

- **V0 — `processus`** (P1) + its gate. Nothing else can proceed.
- **V1 — `tls` cert accessor** (P2) + `-reticularis` gate.
- **V2 — `cliens_tabularii`** (P3) + its gate (first-ever coverage
  for that code). villa builds against it; forum converts after.
- **V3 — `villa_lectores`**: ssh command builder + parsers, driven
  entirely by fixtures. Parser gate green before any app exists.
- **V4 — the app**: scaffolding (§VII), seeded genera, probe
  scheduling, console UI, actions + event writing.
- **V5 — gates**: `probatio_villa_agens` (swept) + `fumus.sh`
  (rendering).
- **V6 — lived bar** against the droplet, with fixture capture in
  the same sitting.

## XI. Gotchas ledger

- `systema_posix.h` is silva's lexicon, not a library.
- argv vector, never `/bin/sh` (quoting class deleted at the root).
- ControlPath ≤ ~100 bytes, gated with a loud refusal; `%C` not
  `%r@%h:%p`; never under `build/` (it gets `rm -rf`'d).
- `BatchMode=yes` always; `known_hosts` trust is a precondition.
- Reserved datum keys — `titulus`/`genus` are stripped, `status`
  projects to the lifecycle column.
- Actions are `nota` + `origo:"villa"`, never `mutatio`.
- Genus collision silently clobbers user genera — check the clavis
  AND the response for `iam occupata`.
- `_plagula_litterae` returns `""` on missing file — assert
  non-empty first.
- Probe snapshots must not live in the per-tick arena.
- `capsula_generare` after every `index.html` edit — put it in the
  launcher.
- `rm -f` before `cp` for the binary (macOS ad-hoc signature).
- No runner sees `apps/` — anything that must not rot goes under
  root `probationes/`.
- Independent, already filed: http drain loop reports truncated
  bodies as success (**01KYANH7AN**).

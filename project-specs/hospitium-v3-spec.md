# hospitium v3 - web apps on the droplet: a small app with login, end to end (spec)

*Fran 2026-10-16, right after TLS 1.3 reached main (ff e1a26869): "being
able to write and deploy web apps to a droplet (maybe behind nginx for
now) ... cookies, file uploads ... mcp servers on droplet, possibly
channel type things, api servers ... gaps in general capabilities".
Successor of the "hospitium HTTP gaps" item named before the TLS arcs;
first step toward parcum ...AAAA (Forum online, Amanda + MCP OAuth).*

Goal: **the house can build a web app in C89 and run it on Fran's droplet
behind nginx.** The proving app is small and complete: register, log in,
keep notes with attached files, download them, log out. It is deployed to
`smaragda` as `https://exemplar.golden.earth`, with its DNS record made
by a house DigitalOcean client. Everything in between becomes reusable:
the handler model, request helpers, security, storage, the Linux build
and the deploy.

Decisions (Fran, 2026-10-16, in conversation):
- **First target = a small web app with login**, built for this arc
  (`apps/exemplar/`). Forum online is its own later arc (headless split,
  where truth lives).
- **Approach C:** fill the request/response gaps, and settle the handler
  API now with two narrow extensions (deferred responses, streamed
  bodies) instead of retrofitting them when MCP/SSE arrive.
- **Single process, single loop:** cooperative CPU jobs live in
  `reactor.h`; hospitium can share the app's reactor. Threads only if
  unsliceable CPU work appears later (a worker pool posting completions
  to the reactor - a future wish, not this arc).
- **Passwords: our own argon2id** (RFC 9106) on our own BLAKE2b (RFC
  7693), RFC vectors as byte-exact oracles. No new vendored library.
- **Sessions: opaque random tokens** in HttpOnly/Secure/SameSite
  cookies, stored hashed.
- **Storage: one volumen** (SQLite via scrinium, already vendored):
  events in `acta`, uploaded files as `massae`. Fran pointed at volumen
  and scrinium; they replace a hand-made JSONL log.
- **Deployment: Fran's droplet `smaragda`; Claude may deploy over ssh,
  with Fran's approval before each push.** Brownfield: the box already
  runs smaragda + nginx + three services; we add our own user, unit and
  one nginx site, never touch the others (villa's rule).
- **DigitalOcean API client in this arc, DNS records only**; token in
  `~/.rhubarb/digitalocean.clavis` (0600), a custom-scoped DNS-only
  token Fran creates; vault later.
- **Hostname `exemplar.golden.earth`.**

## I. What exists (main d2ccd129, 2026-10-16)

- **hospitium** (`include/hospitium.h`, `lib/hospitium.c`, 1315 lines):
  single-thread HTTP/1.1 on `reactor.h` (poll), keep-alive without
  pipelining, guards 400/411/413/414/501, router with `:params`, 405 +
  Allow, HEAD, directory serving with traversal defense, ACAO knob,
  access log, counters; 102 tests. **Handlers must answer synchronously,
  in one buffered response, before returning** - the colloquium is a
  stack local in `_tractatorem_vocare`. Request bodies buffered whole
  (`petitio_maxima` default 1 MB). Files served by `filum_legere_totum`
  (whole file in memory). Hospitium owns a private `Reactor`.
- **http** (`lib/http.c`): server parser (`HttpPetitioServeri`: method,
  uri, via, raw `quaestio`, header array, buffered body, keep-alive,
  chunked), client (`http_exsequi`, incremental response reader,
  redirects), serializer. No cookies, no multipart, no form or query
  parsing, no request-header lookup in the public API.
- **url.h**: `url_decodificare` (handles `+`), `url_params_parse`.
- **json.h**: reader and writer (`json_scribere`, builders).
- **reactor.h**: fds (poll), timers (`reactor_poll` shortens its wait to
  the next timer); no cooperative jobs, no signals.
- **processus.h**: async children (`processus_incipere/pulsare`).
- **scrinium** (SQLite wrapper: prepare/bind/step, transactions,
  migrations, ULID; WAL, busy_timeout, `synchronous=NORMAL`) and
  **volumen** (`acta` append-only events, `massae` SHA-256 blobs with
  dedup, transactions, `volumen_acta_legere`, `volumen_temporarium`).
  Blobs bound whole (`scrinium_ligare_massam`), no incremental blob I/O.
- **moneta**: `moneta_octeti_fortuiti` (/dev/urandom, NO fallback -
  refuses), ULID, nanoid. **sors** is seeded/deterministic, never for
  secrets.
- **BearSSL** vendored: SHA-256, HMAC (and the TLS client; outbound
  HTTPS from macOS works through tls13 + fiducia_macos).
- **Linux**: `tools/linux/` (Dockerfile.glibc ubuntu 22.04 + gcc,
  Dockerfile.musl alpine 3.20 + gcc, `probatio.sh` runs pure-C suites,
  `porta_linux.sh` compiles lib/ under both, `-fumus` built and ran
  tabulariumd). macOS-only pieces: `nomina_macos.m` (DNS) and
  `fiducia_macos.m` (trust) - NOT needed by the proving app (no outbound
  calls on the droplet).
- **villa**: ssh alias `smaragda`, brownfield doctrine.
- **The droplet** (read-only check 2026-10-16): x86_64, **1 CPU, 960 MB
  RAM (~420 MB available)**, Ubuntu 25.04, glibc 2.41, systemd 257,
  nginx 1.26.3 (sites: catholicbookwishlist, default, kisci-events,
  smaragda.golden.earth), certbot present, ports in use 22/80/443 and
  3005-3007 (bound on ALL interfaces - not ours; noted for villa), user
  `fran` with passwordless sudo.

## II. Design

### II.1 Handler API (hospitium)

**Deferred response.**
```c
vacuum colloquium_differre (HospitiumColloquium* colloquium);
b32    colloquium_valet    (HospitiumColloquium* colloquium);
```
- The colloquium lives in the connection piscina (not the stack). A
  handler that defers leaves its connection in a new state EXSPECTANS:
  no further reading, no idle timeout; a deadline
  `responsum_maximum_ms` (config, default 30 s) -> 503 + close.
- Each connection slot carries a **generation**; the colloquium records
  it. A late `colloquium_respondere*` on a closed or reused slot is a
  no-op; `colloquium_valet` tells the app.
- Existing handlers are unchanged (deferral is opt-in).

**Cooperative jobs and a shared reactor.**
```c
/* reactor.h */
nomen b32 (*ReactorLabor)(vacuum* datum);   /* one step ~1-2 ms; VERUM = done */
b32 reactor_laborem_adicere (Reactor*, ReactorLabor, vacuum* datum);
/* hospitium.h */
HospitiumConfiguratio.reactor;   /* NIHIL = private (as today) */
Reactor* hospitium_reactor (Hospitium*);
```
- Each `reactor_poll` cycle runs one step of every pending job; while
  any job is pending the poll wait is 0.
- One process, one loop: server, outbound `http_vocatio`, timers,
  `processus_pulsare`, sliced CPU work.

**Streamed request bodies.**
- Config `corpus_in_memoria_maximum` (default 1 MB), `petitio_maxima`
  (app-chosen, e.g. 64 MB), `directorium_temporarium`.
- Parser hook: after the headers, hospitium takes the body bytes
  (Content-Length or chunked) itself. A non-multipart body above the
  memory limit streams to a temp file -> `petitio->corpus_via`.
- Multipart bodies stream into the multipart parser (II.2).
- Temp files are deleted at the end of the request.

**Body sources for responses.**
```c
nomen structura {
    i64 mensura;                                  /* Content-Length */
    s32 (*legere)(vacuum* datum, i8* capsa, i32 n);  /* >0, 0 = finis, <0 error */
    vacuum (*claudere)(vacuum* datum);
    vacuum* datum;
} HospitiumFons;
vacuum colloquium_respondere_fontem (col, i32 status,
    constans character* mimen, HospitiumFons fons);
vacuum colloquium_respondere_filum (col, i32 status,
    constans character* mimen, constans character* via);  /* fons = file */
```
- Headers with the real Content-Length, then the body pulled in chunks
  as the socket drains (the write tail). Directory serving switches to
  the file source. Volumen blobs are the second source (II.4). Ranges
  out of scope.

**Clean shutdown.**
```c
b32 reactor_signum_adicere (Reactor*, integer signum,
    vacuum (*tractator)(vacuum* datum), vacuum* datum);   /* self-pipe */
vacuum hospitium_sistere_molliter (Hospitium*, i32 mora_ms);
```
- SIGTERM: stop accepting, finish in-flight requests, deferred
  responses and jobs up to `mora_ms`, then close; the app then closes
  its volumen (checkpoint) and exits 0. The signal handler only writes
  a byte to a pipe.

### II.2 Request and response helpers

```c
chorda colloquium_quaestio   (col, "q");          /* first value, decoded */
UrlParams colloquium_quaestiones (col);
chorda colloquium_caput      (col, "Accept");     /* case-insensitive */
chorda colloquium_forma      (col, "titulus");    /* urlencoded OR multipart */
chorda colloquium_crustulum  (col, "sid");
vacuum colloquium_crustulum_ponere (col, "sid", valor,
    constans HospitiumCrustulumOptiones* o);
chorda colloquium_cliens     (col);   /* client IP (trusted XFF last hop, else peer) */
chorda colloquium_schema     (col);   /* "https" behind trusted nginx, else "http" */
vacuum colloquium_redirigere (col, i32 status, constans character* locus);
i32    colloquium_fila       (col, HospitiumFilum** fila);
```
- Decoding is lazy, cached, in the request piscina.
- **Multipart** (`multipart/form-data`): a new incremental parser
  library, fed by hospitium as bytes arrive. Fields stay in memory
  (limit per field); each file part streams to its own temp file while
  SHA-256 runs over it. Limits: parts, field size, header size per
  part; boundary validated; the client filename is reported, never a
  path.
- **Cookies** (RFC 6265): parse `Cookie`; `Set-Cookie` with `Max-Age`,
  `Path`, `Domain`, `HttpOnly`, `Secure`, `SameSite` (Strict/Lax/None);
  names and values validated against RFC 6265's character set (refused,
  not sent). Defaults: HttpOnly, Secure when `colloquium_schema` is
  https, SameSite=Lax, Path=/. Response-header cap 16 -> 32.
- **Behind nginx**: `HospitiumConfiguratio.procurator_fidus` - trust
  `X-Forwarded-For` / `-Proto` only when the peer is loopback.
- JSON responses via the existing `json.h` writer (a thin
  `colloquium_respondere_json` only if the plan finds it worth it).

### II.3 Security

- **BLAKE2b** (new library, RFC 7693): unkeyed and keyed, variable
  output length.
- **argon2id** (new library, RFC 9106), incremental:
  ```c
  Argon2idOpus* argon2id_incipere (constans Argon2idParametri*,
      constans i8* tessera, i32 tn, constans i8* sal, i32 sn, Piscina*);
  b32  argon2id_gradus   (Argon2idOpus*, i32 caudices_maximi);
  vacuum argon2id_digestum (Argon2idOpus*, i8* exitus, i32 n);
  ```
  Lanes computed one after another (the RFC vector has p=4; production
  p=1). Production: m = 19 MiB, t = 2, p = 1 (OWASP's first choice).
  **At most 2 hashes at once** (the droplet has ~420 MB free); more wait
  in the job queue. Stored as a PHC string
  (`$argon2id$v=19$m=19456,t=2,p=1$salt$hash`); constant-time compare;
  an unknown user still costs one full hash against a fixed dummy
  (no timing oracle on account existence); old parameters keep
  verifying and are re-hashed at the next successful login.
- **Randomness**: `moneta_octeti_fortuiti` (no fallback) for salts,
  tokens and the server secret.
- **custodia** (new library, app-agnostic): sessions + CSRF + throttling.
  - Session token = 32 random bytes, base64url, cookie `sid`
    (HttpOnly, Secure, SameSite=Lax). The store keeps SHA-256(token) ->
    {user, created, expires, last seen}. New token at login (fixation),
    deleted at logout, 30-day absolute lifetime, idle limit.
  - CSRF: hidden field `csrf = HMAC-SHA256(server_secret, session
    hash)`, checked on every unsafe method, together with `Origin`
    matching the site.
  - Server secret: 32 random bytes generated at first start,
    `data/secretum`, mode 0600 (outside the volumen, so a database copy
    does not carry it).
  - Throttling: in-memory counters per client IP and per account; after
    N failures, delayed answers (deferred, never blocking), then refusal
    for a window.
  - Passwords: at least 8 characters, at most 1024 bytes (hashing-DoS
    guard), no composition rules (NIST).

### II.4 Storage (volumen)

- The app's state is **one volumen**: `volumen_actum_appendere` for
  every change (`usor.creatus`, `tessera.mutata`, `sessio.creata`,
  `sessio.deleta`, `nota.scripta`, `nota.deleta`, `filum.receptum`); at
  start `volumen_acta_legere(vol, 0)` rebuilds the in-memory state
  (`tabula_dispersa`). Multi-step changes in one transaction.
- Uploaded files are **massae** (SHA-256 dedup built in); the
  `filum.receptum` event records owner, original name, MIME, size,
  hash.
- **Additions to scrinium/volumen:**
  - an opening option for `synchronous=FULL` ("saved" survives power
    loss), default unchanged for existing users;
  - incremental blob I/O on `sqlite3_blob_open/read/write`:
    `volumen_massam_condere_ex_filo` (stream a temp file into a
    zeroblob, hash, dedup) and a chunked blob reader = the second
    `HospitiumFons`.
- Downloads: `Content-Disposition: attachment` (sanitized filename),
  `X-Content-Type-Options: nosniff` - uploads are never rendered inline
  as our own pages.
- Backup = SQLite online backup / `VACUUM INTO` (not `cp` under WAL).
- Tests use `volumen_temporarium`.

### II.5 Linux and deployment

- **Build**: `tools/linux/aedificare.sh <app.c>` - closure from
  `bin/aedilis --partes`, compiled in the **musl** container with
  `-static --platform linux/amd64` -> one self-contained binary.
- **Tests on Linux**: the new libraries and the hospitium suites under
  `tools/linux/probatio.sh` (glibc and musl).
- **On the droplet**:
  - user `exemplar`, binary `/opt/exemplar/exemplar`, data
    `/var/lib/exemplar`;
  - systemd unit: `ExecStart=/opt/exemplar/exemplar --data
    /var/lib/exemplar --portus 3010`, `Restart=on-failure`,
    `NoNewPrivileges`, `ProtectSystem=strict`,
    `ReadWritePaths=/var/lib/exemplar`, `PrivateTmp`, `ProtectHome`;
    logs to journald;
  - nginx site `exemplar.golden.earth`: `proxy_pass
    http://127.0.0.1:3010`, `Host`, `X-Forwarded-For`,
    `X-Forwarded-Proto`, `client_max_body_size 64m`; certbot for TLS;
  - the app listens on **127.0.0.1:3010** only; `/salus` health check.
- **First-time setup** `tools/praeparare_hospitem.sh`: user, dirs, unit,
  site, `nginx -t` before reload, certbot. Brownfield: touches only its
  own files.
- **Deploy** `tools/dimittere.sh <app> <ssh-alias>`: build -> rsync to
  `.new` -> keep `.prior` -> atomic mv -> `systemctl restart` -> `/salus`
  over ssh; failure -> rollback to `.prior`, named. A **dry-run mode
  prints every remote command**; Claude shows it and pushes only after
  Fran's OK.

### II.6 DigitalOcean DNS client

- New library (working name `oceanus`) on the house HTTPS client (tls13
  + fiducia_macos - runs on the Mac, where deploys start):
  `GET/POST/PUT/DELETE /v2/domains/{domain}/records[/{id}]`, JSON via
  `json.h`, paging handled.
- Token: `~/.rhubarb/digitalocean.clavis`, mode 0600, read by the
  library alone with a tolerated raw read (never `lectiones`, never
  printed, never in errors or logs); refused if the mode is wider than
  0600. Fran creates a custom-scoped token (domain records only).
- Used by `praeparare_hospitem.sh`: ensure `A exemplar ->
  165.227.210.190` (idempotent: create if missing, update if
  different, report).

### II.7 The proving app (`apps/exemplar/`)

- `exemplar --data <dir> --portus <n> [--hospes 127.0.0.1]
  [--invitatio <code>]`.
- Pages: register (only with the invite code), log in, log out, "my
  notes" (list; add with an optional attachment; delete), download.
  Server-rendered HTML, no JavaScript required, one CSS file embedded in
  the binary.
- Exercises: deferred response + argon2id job, multipart upload into the
  volumen, streamed download, cookies/sessions, CSRF + Origin,
  throttling, `X-Forwarded-*`, SIGTERM drain, `/salus`.

## III. Tests

1. **RFC oracles**: RFC 7693 Appendix A ("abc") and the Appendix E
   self-test grand digest; RFC 9106 §5.3 argon2id vector byte for byte;
   the incremental API equals the one-shot result at every slice size.
2. **Multipart**: one body split at every byte position gives identical
   fields/files; guard-region truncation sweep (an over-read = SIGSEGV,
   the tls13 R11 lesson); limits (parts, field size, header size),
   missing/bad boundary, hostile filenames.
3. **hospitium**: deferred response (answer later; timeout -> 503; late
   answer after close = no-op via the generation guard); jobs interleave
   with I/O; streamed upload above the memory limit; source-streamed
   download larger than any buffer; cookies (parse, attributes,
   refusal of bad values); forwarded headers trusted only from
   loopback; SIGTERM drain finishes an in-flight request.
4. **curl as a differential oracle** (wish ...R7HKS8): `curl -F`
   multipart, cookie jar round trip, chunked uploads.
5. **scrinium/volumen**: `synchronous=FULL` option; streamed blob in and
   out equals the bytes; dedup.
6. **custodia**: session lifecycle, fixation (new token at login), CSRF
   accept/refuse, Origin mismatch, throttling windows, dummy hash for
   unknown users.
7. **oceanus**: against a local hospitium fake of the DO API (canned
   JSON shaped from DO's documentation); token-file mode check; the
   token never appears in any output. Live check by Fran.
8. **exemplar end to end** against a scratch volumen.
9. **Linux**: all pure-C suites above under glibc and musl.
10. **Live**: deploy to smaragda; register, upload, download, restart
    (drain), `/salus`; Fran's check from his phone.
11. **Plants** per task via silva.planta.

## IV. Not in this spec

SSE / streamed responses and WebSockets (the deferred colloquium leaves
room for them); MCP and OAuth; Forum online; outbound HTTPS **from** Linux
(`fiducia_linux`, a POSIX resolver); a threading API; TOTP, passkeys,
email verification or reset; range requests; compression; HTTP/2; a
query engine; volumen compaction or replication; DigitalOcean droplets
and firewalls (closing the exposed 3005-3007 ports is villa's or a later
DO-client step); CI; multiple droplets.

## AUDIENDA (not verified)

- That `lectiones_fopen` (used by `moneta` for /dev/urandom) never
  records file CONTENT at an app's runtime - the house rule says secrets
  must never pass through lectiones.
- That SQLite and BearSSL build cleanly under musl `-static` amd64 in
  the alpine container (the existing gate compiles lib/ but not as a
  static app binary).
- That DigitalOcean's custom-scoped tokens can be limited to domain
  records, and the exact scope names.
- That `sqlite3_blob_*` incremental I/O is enabled in the vendored
  build (no `SQLITE_OMIT_INCRBLOB`).
- Whether hospitium's parser can hand over body bytes after the headers
  without a deeper rework (II.1 streamed bodies).
- argon2id wall time at m=19 MiB, t=2 on the droplet's single CPU
  (target: under ~150 ms per hash including slicing overhead).

Task order (the plan refines it):
1. reactor: jobs + signals;
2. hospitium: deferred response, generation guard, shared reactor,
   body sources, graceful stop;
3. hospitium: streamed request bodies + multipart library;
4. request helpers (query/form/headers/cookies/forwarded/redirect);
5. BLAKE2b + argon2id;
6. scrinium/volumen: FULL option + streamed blobs;
7. custodia (sessions, CSRF, throttling);
8. exemplar app (offline, end to end);
9. Linux build + tests under glibc/musl;
10. oceanus (DO DNS) + first-time setup + deploy scripts;
11. live deploy to smaragda + Fran's check.

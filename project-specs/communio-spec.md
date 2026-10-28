# communio: shared data across devices and people (milestone 1: pictor)

**Status: APPROBATUM 2026-10-26** (Fran: "yes ... we can continue with
the implementation plan") - designed in conversation with Fran
(2026-10-26), grounded by a three-part recon of the volume layer, the
server/identity side and the app side (scratchpad sync_recon_map.md;
key findings in section I). Builds on `project-specs/genera-datorum.md`
§VIII-b/VIII-c (the target sync design) and
`project-specs/compages-rationes-tutorial.md` (server as single writer,
personae, gap G7/G8). First consumer: pictor's gallery
(`project-specs/pictor-pinacotheca-spec.md`, in main 5863a8e9).

*Fran 2026-10-26: "having data sync to the vps so that data on computer
+ devices will be shared (and backed up to s3). this will also involve
tenancy because some things will be shared, some will be per user and
you will want to figure out how to credit changes to user ... identify
what general purpose missing pieces we might have that could be built
up as modules"; on pictor: "everyone sees everyone's drawings ...
drawings that were created on-device in one section and read only
versions of drawings created on other devices in another section";
"drawings for now can be per-device because dimensions will be
different on different devices anyway ... 'edit a copy'"; on the long
run: "kind of like notion almost where you have users with ownership
over data" - widgets (shopping list) on desktop, mobile and MCP,
notifications early, chat later; "a lot of this might be an expansion
of the volumen layer"; on sign-in: "magic links to an email address,
that way the admin can just say 'this email address should have
access'"; email "should be kind of a 'platform' api where the provider
can be switched out"; Resend to start; "just me and amanda to start".*

Goal (milestone 1): **pictor pictures made on Fran's Mac and iPhone and
on Amanda's iPhone reach every other device of the household through
the VPS, read-only there, each change credited to a person and a
device; the server's data is backed up to S3; a new device signs in
with a magic link sent to an allowlisted email address.**

The milestone is a proof of the general layer, not a pictor feature:
every piece below is built as a reusable module, and pictor is its
first client.

## Decisions

- **D1 Local-first, server as meeting point (genera-datorum VIII-b).**
  Each device keeps working offline on its own volume; sync is a
  background exchange. The VPS stores every device's acts and serves
  them to the others; S3 holds the backup. No device ever needs another
  device to be online.
- **D2 The unit of sync is the act (journal), never a projection.**
  Acts are immutable and carry `(hora, machina)`; they keep that
  identity when copied to the server and to other devices. Blobs
  (massae) travel by their SHA-256. Derived data (projections,
  plagulae, checkpoints) never syncs - each device rebuilds (VIII-b).
- **D2b Sync identity is per volume, not per machine (my call, builder
  check).** Today `(hora, machina)` is minted per volume, and every
  volume on a Mac shares the host UUID - two volumes (standalone pictor,
  vicus's pane, tests) can mint the same identity, and an idempotent
  import would silently drop a real act. Each volume gets its own
  random id at creation (a "replica" id); synced acts are identified by
  `(hora, replica)`; `machina` stays on the act for the record.
- **D3 Every act is credited to a person and a device - one person per
  device (Fran).** A device (replica) belongs to exactly one person, so
  "who" is answered by the server's device -> person record, not stored
  on each act: no new act column, no rewriting history, and acts made
  before sign-in are credited the moment the device registers. Shared
  family devices (the kids' tablets) are a later question.
- **D3b A lost phone or reinstall gets its pictures back editable
  (Fran).** Device identity is issued by the server and can be ADOPTED:
  signing in on a new install offers "restore <old device>"; the new
  install takes over that replica's identity, pulls its acts back and
  keeps editing them. The server is therefore a backup of each device,
  not only a meeting point. This is the general device model for every
  future app, not a pictor special case.
- **D4 Pictor pictures are single-writer (Fran).** A picture can only
  be changed on the device that made it (dimensions differ per device).
  Other devices receive read-only copies that keep following the
  original. "Edit a copy" makes a new local picture from a received one
  (fitted to this device like an imported image) that remembers which
  picture it started from. No merge rules are needed for milestone 1.
- **D5 Spaces (tenancy) - milestone 1 has one: the household.** Data
  lives in a space; a space has members (people) and each member sees
  everything in it. Milestone 1: one space "familia" with Fran and
  Amanda; all pictor pictures live there. Per-person spaces and shared
  spaces with other members are the general shape (Notion-like
  ownership) but only "familia" is exercised now.
- **D6 Sign-in = magic link to an allowlisted email (Fran).** An admin
  (Fran, through the existing admin channel) adds an email address to
  the allowlist. Signing in: enter the email -> receive a link (and a
  short code as fallback) -> the device is registered and gets its own
  device credential, so it never needs the email again until revoked.
  On a phone the link opens the app when installed (our domain linked
  to the app); the code is typed otherwise. Passkeys come later as a
  faster way back in (ledger …G8JN); the email allowlist stays the root
  of access. Guests (read-only web pages for allowlisted emails, e.g.
  grandparents) are a later milestone, enabled by the same allowlist.
- **D7 Email is a platform API with swappable providers (Fran).**
  Callers ask "send this message to this address"; a provider behind it
  does the HTTP. Milestone 1 provider: **Resend** (free: 3,000/month,
  100/day; `POST https://api.resend.com/emails`, bearer key, JSON
  from/to/subject/html/text, `Idempotency-Key` header so a retried
  send never emails twice; DNS records added at golden.earth's current
  DNS host, DigitalOcean). Cloudflare Email Service is a possible later
  provider (needs Cloudflare DNS and Workers Paid for arbitrary
  recipients; sending in public beta since 2026-04).
- **D8 The act log is the truth; S3 holds it as immutable, encrypted
  segments (Fran 2026-10-26).** The log of acts has two complete homes:
  each `.volumen` file (the log plus everything rebuildable from it - a
  single portable document per device, backable by any means) and S3
  (sealed segments "acts of replica R, from N to M", plus blobs). Either
  rebuilds the other; nothing in S3 is ever overwritten. Milestone 1
  backs the server up this way from the start (not file copies), plus
  an occasional full snapshot (itself immutable) so a restore = latest
  snapshot + later segments. Bucket: **Cloudflare R2** (Fran: free for
  10 GB-month and 1M writes, free egress; a provider other than the
  VPS's DigitalOcean, so one account problem cannot take server and
  backups together). horreum already speaks R2 (create-only uploads with
  `If-None-Match: *`).
- **D8b Encryption: a trusted server, encrypted everywhere else (Fran).**
  The server can read data (needed for MCP/Claude, notifications, later
  guest pages); everything that leaves it is encrypted with keys the
  storage provider never sees. Envelope keys: each segment/snapshot has
  its own random key, stored wrapped by a master key (a secret file on
  the server, 0600, plus a copy in Fran's password manager); rotating
  the master re-wraps only the small key records, the data stays
  immutable. Blob object names are keyed hashes (genera-datorum VIII-b)
  so the provider cannot see which files are equal. End-to-end
  encryption (the server cannot read) is a later per-space option (e.g.
  a private diary), never the default.
- **D8c Deletes are soft; real deletion is crypto-shredding (Fran).**
  Deleting is an act: the thing leaves every view and stays restorable
  from the log. Where data must truly be gone, its segment keys are
  destroyed and the ciphertext becomes unreadable.
- **D9 Picture identity becomes global.** `pictura-<n>` collides across
  devices; new pictures get globally unique spatia (ULID, D3 of
  genera-datorum). Existing local pictures keep working; when shared
  they are addressed by device + spatium.
- **D10 Devices in milestone 1: Fran's Mac, Fran's iPhone, Amanda's
  iPhone** (my call - the Android tablets follow once the iPhone path
  works; they need the INTERNET permission and a CA trust store).
- **D12 Every entity has a ULID; names are fields (Fran 2026-10-27).**
  Every "business entity" (picture, calendar item, contact, user,
  volume, later shopping-list items, messages, ...) gets a ULID minted
  where it is created - unique without coordination (offline devices),
  time-ordered (index-friendly, genera-datorum VIII-c). Titles, display
  names, handles and paths are ordinary fields that may change or
  repeat; links, shares, "edit a copy" origins and MCP references
  always point at the ULID. Already true for agenda, personae, cives and
  (T1) volume replicas; pictor pictures move to ULIDs with D9. Fran:
  once data in volumen is categorized (the category registry,
  genera-datorum D6), "has a ULID" is enforced by the category for
  entity kinds rather than remembered by each app.
- **D11 HTTPS on iOS through Apple's networking (Fran).** Apps call one
  house HTTP interface; on iOS it is backed by URLSession through a host
  service (the `TestaAmbitus` pattern, like the photo picker), so the
  app bundles no cryptography of its own, keeps the plain "exempt"
  export answer, and leaves certificate trust to the OS. Mac and VPS
  keep the house TLS; Android gets the same platform treatment later.

## Long horizon (Fran 2026-10-26)

*"long term the idea is to have a kind of 'private federated internet'
solution / internet alternative in a sense based around people having
private internets that can be federated potentially but i think
everything we are building here is good building blocks for however
that later stuff might work."* (See also `project-specs/brighton-visio.md`
on federated links.) Nothing in milestone 1 federates, but its choices
keep that door open: a household server is a node of a private
internet; spaces, members and replica ids say whose data it is and where
it came from wherever it travels; the immutable act log is what nodes
would exchange - federation = a space shared between two households'
servers, the same mechanism one level up.

## I. What exists (recon 2026-10-26)

| Fact | Where |
|---|---|
| Act = `seq` (local arrival), `momentum`, `genus`, `datum` (JSON), `identitas {hora, machina}`, `basis {hora, machina}`; UNIQUE (hora, machina); hybrid clock computed in the INSERT | `include/volumen.h:300-314`, `lib/volumen.c:36-84, 507-557` |
| `machina_identitas`: macOS host UUID; per-install file on iOS/Android; /etc/machine-id on Linux; appends refused without it | `include/machina.h:119-130` |
| "acts after seq N" (`volumen_acta_legere`, per-genus variant), latest seq, `VACUUM INTO` snapshot | `include/volumen.h:84, 316-339` |
| Forks recorded via `basis` ("two acts of the same base, across machines too"); historia keeps documents per spatium in one volume | `include/volumen.h:148-156`, `include/historia.h:28-37` |
| Projections catch up by seq cursor (ready to follow incoming acts); open historia documents fold once at open (do NOT follow) | `project-specs/proiectiones.md:228-238, 279-286` |
| Users: `cives` (ULID, handle, status), invitations, passwords (argon2id), web sessions (ianua); OAuth 2.1 server (admissio: DCR, PKCE, token -> civis/cliens/scopi); MCP over HTTP with per-user tools (exemplar notes credited civis + via + cliens) | `include/cives.h`, `include/ianua.h`, `include/admissio.h`, `apps/exemplar/exemplar_nucleus.c:506-508` |
| Admin channel: AF_UNIX MCP (invitations, cives) reached by ssh | `include/canalis.h`, `apps/exemplar/exemplar_administratio.h` |
| VPS: exemplar (one process, one reactor, one writer) behind nginx/TLS on smaragda; deploy `tools/dimittere.sh`; setup `tools/praeparare_hospitem.sh` (DNS via oceanus/DigitalOcean) | `apps/exemplar/exemplar.service`, `apps/exemplar/exemplar.nginx` |
| HTTP server: routing, multipart, streamed bodies, deferred responses up to 30 s (long-poll possible); no SSE/WebSocket | `include/hospitium.h:84-85, 372-387` |
| Clients (desktop): own TLS 1.3 + BearSSL fallback; blocking `http`, reactor `http_vocatio`; SSE parser | `include/tls.h`, `include/http.h`, `include/http_vocatio.h` |
| S3: horreum (put/get/list/multipart/conditional create; DO + R2 tested); nothing backed up yet | `include/horreum.h`, `project-specs/horreum-spec.md:32-43, 409-420` |
| Mobile shells: no networking, no INTERNET permission on Android, no background modes; iOS publish refuses TLS in the closure until Fran answers the export question | `lib/testa_ios.m`, `tools/android/AndroidManifest.forma.xml`, `project-specs/testa-ios-divulgatio-spec.md:64-71` |
| In-app notifications (admonitiones) fed by a local log; no OS notifications, no push | `include/admonitiones.h`, `project-specs/colloquia-plan.md:45-46` |

## II. Missing pieces = the modules this milestone builds

Each is general; pictor is only the first user.

1. **Act import (volumen).** Store a foreign act keeping its `(hora,
   machina)` (and actor), with its own local `seq`; HLC receive rule
   (push the local clock past it) with a future-skew bound (VIII-c);
   idempotent (same identity twice = no-op).
2. **Replica identity and device registry.** A random replica id per
   volume (D2b); the server's device -> person record (D3); adoption of
   an old device's identity on a new install (D3b).
3. **Sync protocol + client.** Device -> server: my acts after my
   upload cursor, plus missing blobs by hash. Server -> device: acts
   of my spaces after my download cursor; long-poll for "something
   new". Cursors persisted per remote (outbox state).
4. **Server act store.** On the VPS: acts per space from every device,
   credited and ordered; serves the protocol; the single writer.
5. **Spaces and membership.** Which space a piece of data belongs to;
   members; the server only serves a member the spaces they belong to.
6. **Devices and magic-link sign-in.** Email allowlist (admin), magic
   link + code, device registration with its own credential, revoke.
   Extends ianua/admissio rather than replacing them.
7. **Email platform API + Resend provider** (D7).
8. **Platform HTTP.** One house HTTP interface; iOS backed by
   URLSession as a host service (D11); the app-link from our domain into
   the iOS app (D6).
9. **Live documents.** An open document (and the pictor gallery) sees
   acts that arrive while it is open.
10. **Log segments to S3** (D8, D8b): sealed, encrypted, immutable
   segments + snapshots in R2, envelope keys, keyed blob names, a tried
   restore; crypto-shredding (D8c) designed in, exercised once.
11. **pictor as the first client:** global picture ids (D9), the
   gallery's "this device" and "other devices" sections (read-only,
   credited "by Amanda" etc.), "edit a copy" (D4).

## III. What "done" looks like

1. Fran draws on his iPhone; within a minute (both online) the picture
   appears in the "other devices" section on Fran's Mac and Amanda's
   iPhone, credited to Fran, read-only; further strokes keep arriving.
2. With a device offline, drawing continues; when it reconnects,
   everything it missed arrives and everything it drew goes out - no
   duplicates, nothing lost.
3. "Edit a copy" on a received picture makes a new local picture that
   records its origin.
4. A new device signs in only via a magic link to an allowlisted
   address; an address not on the list gets nothing (and learns
   nothing); a revoked device stops syncing.
5. The server's data is in S3; a restore onto a fresh server is done
   once and its pictures match.
6. Every act on every device can answer "who and which device".

## IV. Non-goals (milestone 1)

Notifications and push; chat; job queues and task inboxes (ledger
…S7); widgets over MCP; guests and private web pages; passkeys;
per-person spaces beyond "familia"; multi-writer merge rules (pictor is
single-writer); Android tablets (follow-up, D10); client-side
encryption of synced data (VIII-b target - see questions); vicus as a
sync client.

## V. Left to the builder

- Wire format and endpoints of the sync protocol (HTTP + JSON is the
  house default; batch sizes; long-poll vs periodic poll timing).
- Where the server act store lives (inside exemplar or a sibling
  service on the same box) - as long as it is one writer and backed up.
- Exact form of the device credential (opaque token stored hashed, like
  ianua sessions, is the obvious one) and of the magic link (single-use,
  short-lived, hashed at rest).
- How pictor's volume maps to spaces (all pictures -> "familia").
- Segment size/cadence and snapshot cadence (D8); the AEAD cipher for
  segments (house ChaCha20-Poly1305 or AES-GCM from the TLS stack).

## Resolved with Fran (2026-10-26)

One person per device (D3); lost phone/reinstall restores editable
pictures (D3b); HTTPS on iOS via Apple's networking - keeps the exempt
export answer (D11); backups in Cloudflare R2 (D8); encrypted with
envelope keys, trusted-server model (D8b); soft deletes, crypto-shredding
for real deletes (D8c).

## Resolved with Fran (2026-10-27, before phase 3)

- **Allowlist = email -> person (Fran: "yes").** An entry pairs an
  address with a person (civis); the admin tool creates the person in
  the same step when the handle does not exist yet (Amanda has no
  exemplar account). Milestone 1 allowlist: Fran's address -> `fran`,
  Amanda's address -> `amanda` (the addresses themselves are entered
  on the server through the admin channel only, never written into the
  repository). The same allowlist later carries guests (D6).
- **Sender: `communio@golden.earth`.**
- **Server secrets are kept locally and copied up by a tool (Fran's
  idea).** Keys live where the others do (`~/.rhubarb/<provider>.clavis`,
  0600; `resend.clavis` created by Fran 2026-10-27); a small tool streams
  one to `/var/lib/<app>/<name>` on the host (owner = the app's user,
  0600) through ssh stdin - never in argv, a temp file or any output;
  confirms by comparing hashes on both ends and printing only
  match/mismatch; refuses to overwrite unless asked to rotate. General
  for every later server key (R2, push).

## AUDIENDA (not verified while writing)

- iOS app-links (associated domains) for a self-signed-up developer
  account and our own domain - setup steps not tried.
- Resend's DNS verification records at DigitalOcean (standard SPF/DKIM;
  not tried).
- URLSession as a C-callable host service: completion handlers on the
  main thread fit the shell's single-thread rule (blocks already compile
  under the house ObjC flags, testa-ios-imago-spec I).
- Battery/background limits: milestone 1 syncs while the app is open
  (no background sync); acceptable for a proof?

## VI. Phase 1 built (2026-10-27, quarta)

Plan `project-specs/communio-plan-1.md`, tasks T1-T5: replica identity
(0a61c01d), act import with writer numbering and the clock bound
(b44ff2af), `communio_transferre` moving acts + blobs between volumes
(94c80240), foreign documents read-only (dc10992a), following
(this commit). Proven in-process with two volumes on the Mac: a picture
drawn in A opens in B with identical pixels (strokes, image layer, undo,
redo, branch), keeps following, B's own pictures untouched. Findings
that change later phases: references inside a document are in the
WRITER's numbering (`seq_originis`); foreign documents never use
checkpoints; local listings must skip `aliena/`; a manifest change
needs its blob first (`VOLUMEN_MASSA_DEEST`). Next: phase 2 (server act
store + protocol).


## VII. Phase 2 built (2026-10-27, quarta -> main f7fcba7f)

Plan `project-specs/communio-plan-2.md`, tasks T1-T7 + final review
(fable: 0 critical, 6 important, all fixed). The household server is
exemplar with a second volume `<data>/communio.volumen` (imported acts
never mix with notes and users); routes under `/communio/` (nginx
`location /` already forwards them); every route requires a device
token. Devices MINT their own token (`communio tessera`), the admin
registers only its SHA-256 over the admin channel (handle + space +
title + digest) - no secret ever crosses the channel or a chat. The
server announces its volume's replica: a replaced or restored server
makes clients reset their cursors and re-sync (deleting
`communio.volumen` is a safe reset). One blob limit on both sides
(16 MB). Client: two lanes (push, long-poll pull) on the reactor,
cursors in the volume's praesentia, all HTTP in one function (phase 5
swaps in URLSession).

Live (Task 8, 2026-10-27, with Fran): deployed (backup
`exemplar.volumen.ante-communio`); Fran in "familia"; device
"Macintosh" (main checkout's `pictor.volumen`: one picture, 11 strokes)
pushed 13 acts over HTTPS; a scratch device pulled them and the
picture's composite fingerprint matched exactly (02aad7f4...); the
scratch device was revoked and its token then got 401. Not yet: iPhone
and Amanda (phase 5), email sign-in (phase 3), backups (phase 4),
background sync. Deferred minors: see the plan-2 ledger note on desideratum
...GRTKH (lane quarantine is the one to watch).

## VIII. Phase 3 built (2026-10-27, quarta -> main d6884bbf)

Plan `project-specs/communio-plan-3.md`, tasks T1-T7 + final review
(fable: 1 critical, 4 important, all fixed in d4dbe8bb). A device signs
in by email: the device mints its token as before and sends only the
digest with `POST /communio/intrare`; the server mails a one-time link
plus an 8-character code (15 minutes, 5 tries) from
`communio@golden.earth` via Resend (`lib/epistula`, with a fake provider
for tests); `communio inscribere` takes the code from stdin or notices
the link was clicked, whichever comes first. Only allowlisted addresses
get mail (allowlist = email -> person, admin tools
`communio_epistulam_permittere/_revocare`, `communio_permissos_enumerare`),
but every request answers 202 after the same 250 ms floor, so the
answer never tells whether an address is known. Mail budgets are counted
in the database (3 per address per hour, 30 per server per day), not in
an evictable memory table. The link opens a page with a button; only the
POST registers the device (mail scanners prefetch links). Server keys
live locally in `~/.rhubarb/<name>.clavis` and go up with
`tools/secretum_mittere.sh` (ssh stdin, hash compare, no overwrite
unless asked to rotate). `oceanus` learned MX priority.

Live (Task 8, 2026-10-27, with Fran): golden.earth verified at Resend
(DKIM, `send`/`rsend` CNAMEs, DMARC p=none via oceanus; the AUDIENDA
item on Resend DNS is settled); `resend.clavis` copied up (match);
deployed d4dbe8bb from quarta under the worktree-deploy rule (backups
`*.ante-epistulae`); both addresses allowlisted (`amanda` created); Fran
signed a device in by clicking the link, and it was then revoked (the
hand-registered "Macintosh" stays). Finding: DigitalOcean stores CNAME
targets without the trailing dot, so oceanus reports them as changed on
every run (ledgered). Deferred minors: the plan-3 ledger's list, filed
in tabularium. Next: phase 5 (iPhone + pictor), phase 4 (backups),
3b (device adoption).

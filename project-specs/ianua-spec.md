# ianua - house accounts as general components (spec)

**Status: SCRIPTUM 2026-10-20** - approved by Fran; plan
`project-specs/ianua-plan.md` (T1-T6). The four AUDIENDA items are
settled there: concurrent CLI write -> P1 (single-writer lock, stop /
invite / start), admissio re-keying -> P8, note authors -> P7, frenum
keys -> P6.
Ledger: desideratum ...MK0 (`01M5CZKPHXDYCB0VQ8W315ZMK0`, piece #1 of
its seven components); design question ...XWG (domus vs per-app) stays
open and is NOT decided here - this spec builds what both answers need.

*Fran 2026-10-20, after mcp-remotus went live on exemplar: "having a
single identity that could be re-used against each new app we make
would be pretty useful", then: "the safe route is to implement as many
things as general purpose components as possible that lets the
implementation be a thinner layer while giving us the flexibility to
try different things". Piece #1 chosen first because it is extraction
from running code, is needed under either identity model, and fixes a
live defect (section I).*

Goal: **accounts stop living inside each app.** Three data libraries -
`cives` (who someone is), `credentialia` (how they prove it),
`invitationes` (who may join) - plus `ianua`, the door an app mounts on
hospitium (sign up, sign in, sign out, change password, invite).
exemplar becomes a thin consumer; the budgeting app (sub-project C)
composes the same pieces. Switching later between per-app accounts and
one house identity (`domus`) rewrites only the composition, never the
data machinery.

Decisions (Fran, 2026-10-20, in conversation):
- **Piece #1 first** of the ledger's list (#2 sealing + key store /
  crypto-shredding, #3 admissio userinfo + OAuth client, #4 sealed
  volumen, #5 passkeys come later, each pulled by a consumer).
- **Approach A:** three pure data libraries + one thin flow library
  (not one `rationes` library - it would trap invitations and
  credentials inside the HTTP flow; not data libraries alone - every
  app would re-copy the security-sensitive flow, which is how exemplar
  grew ~600 lines of it).
- **Audience (b):** apps will admit invited friends and family, so
  invitations are per-person codes (single use, expiry, who invited
  whom, revocable) - not one shared secret.
- **Who may invite:** any active member, by default; an app tightens it
  through the `potest_invitare` callback. No roles in this spec.
- **First person:** a CLI command on the app binary prints a
  single-use code; the shared `cfg.invitatio` secret is retired.

## I. What exists (quarta 026140d2, 2026-10-20)

- **exemplar's account layer** is `apps/exemplar/exemplar_nucleus.c`
  ~170-1000 of 1719 lines:
  - users keyed by **handle** (`_usor_ponere` :180, `tabula_dispersa`
    by `nomen_usoris`) - no stable id;
  - state **replayed from the whole event log at boot** (`_replicare`
    :254), including `usor.creatus` / `tessera.mutata` events that
    carry the **argon2id PHC hash** (:620-622, :713-715). **Defect:**
    the log is append-only, so every hash ever set lives forever;
  - invitation = **one shared secret** from config, compared in
    constant time (`_inscribere_post` :656-658);
  - async hashing plumbing - `Opus` slots, deferral
    (`colloquium_differre`), the rate-limit delay timer
    (`custodia_frenum` + `reactor_timer_adicere` :870-904), rehash on
    login (`_renovatio_facta` :699) - ~250 lines every app would copy;
  - sessions replayed from `sessio.creata` / `sessio.deleta` (hash of
    the token only - correct, :772).
- **`lib/custodia`** (750 lines): argon2id hash/verify on a worker,
  session tokens + their sigillum, CSRF, Origin check, `frenum`. Unknown
  handle = full dummy hash (`custodia_tesseram_verificare`, salt
  `custodia:fictum`, lib/custodia.c:403-414) - timing already equal.
- **`lib/admissio`** keeps state in `scrinium` tables + events without
  secrets - the pattern this spec adopts. Its grants and tokens store
  the identity `intratus` returned in `usor TEXT` columns
  (lib/admissio.c:72, :79): today a handle.
- **`lib/scrinium`**: transactions (`scrinium_incipere` /
  `committere` / `revolvere`), append-only migrations
  (`scrinium_migrare`), `scrinium_ulid`; opens with `journal_mode =
  WAL` and `busy_timeout = 5000` (lib/scrinium.c:61-62).
  `volumen_scrinium` exposes the volumen's own store, so events and
  table writes can share one transaction.
- **Name taken:** `personae` is the contacts library (nomenclator,
  lib/personae.c 1360 lines). Prefixes `cives_`, `credentialia_`,
  `invitationes_`, `ianua_` are free (grep of lib include apps tools
  probationes, 2026-10-20).

## II. Design

Common to the three data libraries:
- each opens its tables in the app's volumen via `scrinium_migrare`
  (table names prefixed by the library);
- every write runs **inside a transaction the caller opens**, so one
  user action can span libraries atomically;
- each writes its own events with `volumen_actum_appendere`; **no event
  ever carries a secret** (no PHC, no invitation code, no session
  token - hashes of tokens only, as today);
- no HTTP, no reactor dependency except `credentialia` (it owns the
  async hashing).

### II.1 `cives` - who someone is

Table `cives`: `id` TEXT PK (ULID, stable forever), `nomen_usoris`
TEXT UNIQUE (sign-in handle, 1-32 of `a-z0-9._-`, the rule exemplar
has), `titulus` TEXT (display name, free text), `status` (activus /
suspensus / deletus), `creatus_ms`.

API (shape; exact signatures in the plan): `cives_aperire(Volumen*)`,
`cives_creare(c, nomen_usoris, titulus, &id)`, `cives_per_id`,
`cives_per_nomen`, `cives_nomen_mutare` (handle change keeps the id),
`cives_statum_ponere`, `cives_delere` (tombstone: status deletus; the
handle stays reserved and is never reissued, so old log lines that name
it never point at a new person).

Events: `civis.creatus` {id, nomen_usoris}, `civis.renominatus`,
`civis.status`, `civis.deletus`. True erasure is piece #2's job.

### II.2 `credentialia` - how someone proves it

Table `credentialia`: `id`, `civis` (ULID), `genus` ("tessera" now;
passkeys later as another genus), `phc` TEXT, `creatum_ms`,
`ultimus_usus_ms`. A person may hold several.

Owns, moved from exemplar: the work slots, deferral, the rate-limit
delay (`frenum` keyed per IP+handle and per handle), the unknown-handle
path (custodia's dummy hash), rehash on login when parameters changed.

**Hashing is split from writing:**
- `credentialia_digerere(cr, tessera, reditus, datum)` hashes on the
  worker and hands back the PHC;
- `credentialia_inserere(cr, civis, genus, phc)` writes it inside the
  caller's transaction;
- `credentialia_verificare(cr, nomen_usoris, tessera, clavis_paris,
  reditus, datum)` - lookup, frenum, verify, rehash; the callback gets
  the civis id or failure.

The split lets sign-up hash first, then commit person + credential +
invitation together.

Events: `credentiale.positum` {civis, genus}, `credentiale.remotum` -
person and kind only.

### II.3 `invitationes` - who may join

Code: 128 random bits as base64url (22 characters; `base64url_scribere`
from mcp-remotus T2), shown **once**; only SHA-256 of it is stored.

Table `invitationes`: `id` (ULID), `sigillum` (hash, UNIQUE), `auctor`
(civis ULID; empty for the CLI bootstrap), `nota` (e.g. "pro Amanda"),
`creata_ms`, `expirat_ms`, `usus_max` (default 1), `usus_numerus`,
`usa_a` (civis who used it), `revocata_ms`.

API: `invitationes_creare` (returns the code once), `inspicere` (is it
valid, without using it - the sign-up page), `consumere(inv, codex,
civis)` inside the transaction with a guarded update (`usus_numerus <
usus_max`, not expired, not revoked) so two racing sign-ups cannot both
take a single-use code, `revocare`, `enumerare` (by auctor).

Events: `invitatio.creata` {id, auctor, expirat}, `invitatio.usa` {id,
civis}, `invitatio.revocata` - never the code.

### II.4 `ianua` - the door

Mounted on hospitium like `admissio_montare`; holds the security
plumbing in one place: Origin, CSRF, frenum delay, constant time, safe
`redire` (local paths only - exemplar's `_redire_tutum`), deferral, log
before responding.

**Sessions move here.** Table: token *hash*, civis, created, expires,
last seen. Events `sessio.creata` / `sessio.deleta` with the hash only.
**Changing a password ends every other session of that person.**

Routes (under a configurable prefix):
- `GET/POST /intrare` - sign in; `?redire=` kept, local only;
- `GET/POST /inscribere` - accepts `?invitatio=` so an invitation is a
  link; hashes, then **one transaction**: consume invitation, create
  person, insert credential, events; then the app's
  `post_inscriptionem` hook;
- `POST /exire` - sign out;
- `GET/POST /tessera` - change password (current one required);
- `GET/POST /invitationes` - the signed-in person's invitations:
  create (note + expiry), see the code and link **once**, see who used
  each, revoke.

`IanuaOptiones` (the policy layer): prefix, session lifetime, default
landing path, invitation required (b32), `pagina` frame callback (the
app wraps the default forms in its own title and style), 
`post_inscriptionem(civis)`, `potest_invitare(civis)` (default: any
active person).

API for app handlers: `ianua_intratus(ia, col, &civis, &csrf)`
(replaces `exemplar_intratus_usor`), and an adapter with
`AdmissioIntratus`'s signature so OAuth consent sees the same person.

**First person:** `<app> -invitare "<nota>"` opens the volumen, issues
a single-use code (auctor empty), prints it and the link, exits. On
smaragda: `sudo -u exemplar exemplar -invitare ...` while the server
runs (WAL + busy_timeout make the concurrent write safe in principle;
section AUDIENDA).

Not in this spec (YAGNI): a device/session list page, email, password
reset by mail, roles. A forgotten password = another member issues a
reset invitation (later), or the CLI.

### II.5 exemplar's cutover and migration

- exemplar mounts `ianua`; its account routes, `Usor`/`Sessio` tables,
  `Opus` slots and `_replicare` account branches go away; notes stay.
- **Migration, once, in one transaction**, recorded via
  `scrinium_migrare`: replay old `usor.creatus` / `tessera.mutata` into
  `cives` (new ULID each) + `credentialia` (latest PHC); re-key note
  authors (`nota.scripta` replay maps handle -> ULID; new notes store
  the ULID) and admissio's `usor` columns in grants and tokens from
  handle to ULID; old `sessio.*` dropped (everyone signs in once).
- **What it cannot fix:** the PHC hashes already in the append-only log
  stay there. After the deploy, Fran and Amanda change their passwords
  so the logged hashes belong to dead passwords. Rewriting the log is
  refused: it would set the precedent that the log is editable.

### II.6 Order of work (shape for the plan)

1. `cives`
2. `credentialia` (the async machinery moves out of exemplar)
3. `invitationes`
4. `ianua` (sessions, routes, options, CLI bootstrap, admissio adapter)
5. exemplar cutover + migration
6. deploy to smaragda + live checks

Inline, one task per turn, Fran approves each (as mcp-remotus).

## III. Tests

Each library its own credo v2 suite; existing behaviour **moves** out
of exemplar's suite rather than being rewritten.

- **cives:** handle rules; rename keeps the ULID; tombstone keeps the
  handle reserved; reopen the volumen **twice**.
- **credentialia:** digerere -> inserere -> verificare; unknown handle
  takes the same path; frenum delay deferred, never blocking; rehash on
  login; full slots refuse "occupatum"; **no event contains
  `$argon2`** - planted red by writing the PHC into an event.
- **invitationes:** single use; expiry; revocation; two racing
  `consumere` - one wins; the code appears in no table and no event
  (scan both for the plaintext).
- **ianua** (in-process hospitium, like exemplar's suite now): sign-up
  with code commits all three; a failure mid-transaction leaves none of
  them; sign-in / sign-out / redire; Origin and CSRF refusals; password
  change ends other sessions; invitation page shows the code once; the
  admissio adapter; the CLI bootstrap against a closed and an open
  volumen.
- **exemplar:** app sections (notes, MCP tools, `/applicationes`) keep
  passing through ianua; **migration test** builds a pre-migration
  volumen with the old event shapes, migrates, checks both people sign
  in and still see their notes and grants, then migrates **again** -
  no change.
- Linux build (musl + glibc via OrbStack) before calling any task done
  (gcc catches sign-conversion clang misses).

## IV. Not in this spec

- **Piece #2** sealing + key store (crypto-shredding) - pulled when an
  app promises real deletion; the caveats are in ledger ...MK0.
- **Piece #3** admissio userinfo / introspection + an OAuth *client* -
  only under domus (...XWG).
- **Piece #4** per-person sealed volumen; **piece #5** passkeys
  (WebAuthn: CBOR, COSE, P-256).
- Roles, email, reset by mail, a device/session list page.

## AUDIENDA (not verified)

- **Concurrent CLI write** while the server runs: WAL +
  `busy_timeout = 5000` are set, but whether volumen's event sequence
  and any in-memory state of the running server stay consistent with
  an outside writer is not checked. Fallback if not: a start-up flag
  that prints a code when `cives` is empty.
- **admissio re-keying:** the `usor` columns are known (lib/admissio.c
  :72, :79); whether admissio events or `/applicationes` rendering also
  carry the handle in a way the migration must touch is not yet read.
- **Note authors in the log:** old `nota.scripta` events carry the
  handle; the plan must choose between mapping at replay time (log
  untouched) and a notes table - not decided here.
- Whether `custodia`'s frenum keys (`par:`, `usor:` strings built in
  exemplar :865-869) move into `credentialia` unchanged or become a
  documented key scheme.

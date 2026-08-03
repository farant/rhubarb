# RATIONES — the hello-world that limit-tests the framework

STATUS: aspirational draft (2026-08-04). Written in present tense AS IF the
framework were finished — README-driven development. Every chapter carries a
status box: EXSTAT (works today) / DIMIDIUM (partial, seam named) / DEEST
(missing, gap named). Code is illustrative — existing-library signatures are
approximated, invented APIs are proposals with unsealed names. The purpose of
this document is to be red-teamed: by Fran at the prose level, by
context-free agents at the walkthrough level.

The app: **rationes** — a shopping-list + monthly-budgeting app. Documents
are files. A document opens locally or from a server. It runs as a desktop
app, a web app, a daemon, and an MCP server — one binary, different flags.

Framework working name: **compages** (a joining-together; NOMEN NONDUM
SIGILLATUM).

---

## 0. What you need

- the rhubarb checkout, `./compile_tests.sh` green
- for chapters 8+: a VPS you can ssh into, nginx + letsencrypt already
  configured, a domain pointed at it

What you do NOT need: any dependency. Everything below is house C89.

---

## 1. Scaffold

```
$ bin/aedilis novum rationes --vitrea
  apps/rationes/
    rationes.sh            launcher (generated, aedilis-maintained)
    fontes/rationes.c      principale — you own main()
    assets/index.html      the vitrea page
    assets/rationes.js
    probationes/fumus.sh   smoke test
$ ./apps/rationes/rationes.sh
```

A window opens with a hello page. The scaffold is ~60 lines of C: open a
fenestra, mount vitrea over it, serve `assets/` via capsula, drain events.
You own `main()` — compages never calls you; you call it.

> **STATUS**
> - vitrea + fenestra + capsula assets: **EXSTAT** (forum, villa, demos)
> - `include/X.h → lib/X.c` zero-config resolution: **EXSTAT** (aedilis)
> - `aedilis novum` scaffold command: **DEEST** — today you copy an existing
>   app by hand. Gap G1.

---

## 2. The document is a file, the file is a database

Rationes is document-based. A document is a **volumen**: a single sqlite
file that contains everything — the event log, the entity projections, the
images, the users. Copy the file, you copied the document. Back it up with
cp. This is the scrinium discipline (WAL, single writer, append-only
migrations) wrapped in a document lifecycle:

```c
Volumen* vol = volumen_aperire(piscina, "~/Documenta/domus.rationes");
/* creates the file if absent: migrations run, event log initialized */
```

Inside every volumen, truth is the **acta** table — an append-only event
log, exactly the gesta shape but per-file instead of per-repo. Entities are
a rebuildable projection of the acta. `volumen_reficere()` drops projections
and replays the log; corruption of a projection is never data loss.

> **STATUS**
> - scrinium (sqlite wrapper, migrations, ULID, WAL): **EXSTAT**
> - event log + projection discipline: **EXSTAT** as architecture (gesta),
>   but gesta is repo-global and JSONL-truth; the per-file, sqlite-truth
>   variant is **DEEST**. Gap G2 — `volumen.h`, the keystone.
> - Note: this is also the forum's own migration path (diurnum 2026-08-03:
>   "migrate the data model to event sourced sqlite").

---

## 3. Entities: define genera, get behavior

You declare the schema as genera — the same runtime entity-type machinery
the forum runs on, as a linkable library:

```c
volumen_genus_serere(vol,
    "res_emenda",                      /* shopping-list item */
    "titulus:textus:necessarium",
    "pretium_aestimatum:numerus",
    "emptum:veritas",
    "imago:imago",                     /* see ch. 6 */
    NIHIL);

volumen_genus_serere(vol,
    "sumptus",                         /* an expense */
    "dies:dies:necessarium",
    "summa:numerus:necessarium",
    "categoria:electio(cibus,domus,vestis,cetera)",
    "apocha:imago",                    /* the receipt photo */
    NIHIL);
```

Every write is an event in the acta; custodia judges rows against the genus
definition (soft-flags, never blocks — definitions judge, they don't
constrain). Queries go through the projection: plain SQL via scrinium, FTS
included.

> **STATUS**
> - genera semantics (runtime types, custodia, field palette, FTS):
>   **EXSTAT** — but embedded in tabularium.c (7,530 lines, app not
>   library). Extraction is Gap G3 — the ActiveRecord moment. Three pulls
>   already recorded (storage direction, this tutorial, VPS split).
> - field palette today: textus/area/dies/annus/numerus/veritas/electio/
>   relatio — no imago/blobum. Gap G6.

---

## 4. The generated UI (the heavy-library moment)

You have not written any UI code yet. Ask for it:

```c
vitrea_genera_praebere(vitrea, vol);
```

The app now shows a tab per genus: list view, create/edit forms derived
from the field palette, delete with the genus-appropriate shape. This is
the forum's FileMaker layer as a library call — schema in, working CRUD
app out.

**The escape hatch (the taste question):** generated views are the floor,
not the ceiling. Your own assets/rationes.js coexists with the generated
tabs; when a generated view is 90% right, you *replace that view* with your
own page that calls the same internuntius endpoints the generated one used.
Generated UI is never edited in place — it is either accepted or replaced
per-view. (The silva regeneration story — provenance-marked generated code
with hand-written islands — applies to generated C, ch. 10 note.)

The budget view is your first custom page: a monthly rollup,

```sql
SELECT categoria, SUM(summa) FROM sumptus
  WHERE dies BETWEEN :initium AND :finis GROUP BY categoria;
```

rendered against a per-category budget the user sets. Projection queries
are just SQL — no query DSL to learn.

> **STATUS**
> - forum proves the pattern end-to-end (runtime genus → UI): **EXSTAT**
>   as precedent, **DEEST** as a library call (`vitrea_genera_praebere`).
>   Gap G4 — falls out of G3 + the forum's existing JS, mostly extraction.
> - internuntius (JSON-RPC over missor seam): **EXSTAT**

---

## 5. It's a shopping list

Chapters 1–4 already gave you the app: add `res_emenda` items on your
phone-sized window, check them off in the store, snap the receipt into a
`sumptus` when you pay. The tutorial's midpoint claim: **a CRUD document
app is ~100 lines of your own code.** If the walkthrough finds otherwise,
that variance is the polish backlog.

---

## 6. Images

`imago` is a field genus: the picker accepts a file or a paste, the bytes
land in the volumen as a blob (flatura-compressed, sigillum-addressed so
identical images store once), the generated views thumbnail it. sqlite
holds blobs happily at this scale; the document stays one file.

> **STATUS**
> - flatura (compression): **EXSTAT**; sigillum (content address): **EXSTAT**
> - blob column discipline + imago field genus + upload path through
>   vitrea/internuntius: **DEEST**. Gap G6. (Amusing precedent: the OLD
>   entitas lineage already had TYPUS_BLOBUM "auto-compressed with
>   flatura" — the idea is ten months ahead of the plumbing.)

---

## 7. Multi-user

Every event in the acta already carries an actor. Multi-user means actors
become **personae** — a seeded genus in every volumen:

```c
volumen_personam_serere(vol, "fran", PERSONA_DOMINUS);
volumen_personam_serere(vol, "amanda", PERSONA_SCRIPTOR);
```

Locally this is attribution (who added the milk). Remotely (ch. 8) it is
authorization: the server maps a login to a persona and stamps every event
it accepts. Tabs/views can be shared or per-persona (the forum's
shared-vs-instanced split, decided per genus).

> **STATUS**
> - actor-on-every-event: **EXSTAT** (gesta/tabularium law from birth)
> - personae as identity + roles + login mapping: **DEEST**. Gap G7.
> - vitrea servus auth (token→cookie, birth-gate): **EXSTAT** for
>   single-secret; multi-credential is part of G7.

---

## 8. Remote documents

The File → Open dialog accepts a URL:

```c
Volumen* vol = volumen_aperire_remotum(piscina,
    "https://rationes.example.com/domus.rationes",
    credentiale);
```

Truth lives on the server. The client holds a replica projection and an
outbox: reads are local and instant; writes are events sent to the server,
which is the **single writer** appending to the acta (the scrinium law,
now load-bearing for sync). The append-only log is what makes this
tractable — replication is "ship me acta after seq N," a paginated pull,
not a merge algorithm.

**Decision recorded, v1:** offline writes queue in the outbox and apply
when reachable; conflicts resolve last-writer-wins at the field level with
the losing event preserved in the acta (nothing is ever lost, the log is
the log). No CRDTs in v1 — the single-writer server IS the ordering.

> **STATUS**
> - the protocol shape (MCP-over-TCP per-request, start-if-absent):
>   **DIMIDIUM** — cliens_tabularii is the embryo, but it is
>   tabularium-specific and has no replica/outbox concept
> - volumen replication (acta-after-seq pull, outbox, replay): **DEEST**.
>   Gap G8 — the largest genuinely new design in this document.

---

## 9. Deploy

You have a VPS with nginx + letsencrypt and a domain. Then:

```
$ ./apps/rationes/rationes.sh deducere fran@vps.example.com \
      --dominium rationes.example.com
  [porta]    linux/amd64 binary built in container, gate green
  [vectura]  binary + assets shipped (scp)
  [systemd]  rationes.service installed, running
  [nginx]    site config written, reloaded; TLS via existing certbot
  https://rationes.example.com is live
```

The web app at that domain is the same vitrea app over vitrea servus — the
transport shim means the assets and JS are byte-identical to the desktop
app. Your phone gets the QR code.

> **STATUS**
> - Linux build proven + gated: **EXSTAT** (porta_linux; arm64 lanes —
>   amd64 is one --platform flag, Gap G5a)
> - vitrea servus (serve any vitrea app unmodified, auth, QR): **EXSTAT**
> - ssh orchestration precedent: **EXSTAT** (villa drives remote boxes)
> - the `deducere` command (build+ship+systemd+nginx as one verb):
>   **DEEST**. Gap G5.
> - `audire` over SSE for live updates through nginx: **DEEST** (known
>   vitrea servus gap, already ledgered)

---

## 10. One binary, four postures

```
$ rationes                          # desktop: fenestra + vitrea
$ rationes -servire 8080            # web app (vitrea servus)
$ rationes -daemon                  # headless: volumen host, TCP
$ rationes -mcp                     # MCP server over stdio
```

The daemon posture is what runs on the VPS: it owns the volumen (single
writer) and serves both the web app and replication. The MCP posture
exposes the same operations as tools — census, quaerere, addere on your
own genera — so a local Claude can work your shopping list.

Note on generated C: the scaffold and any generated glue carry silva
provenance marks (STML in comments), so `aedilis novum --renovare` can
re-derive its portions of rationes.c while preserving your islands —
regeneration as AST-level merge, not overwrite. (The biblia_dr.c wound is
the negative-space proof this is needed.)

> **STATUS**
> - one-binary-many-postures precedent: **EXSTAT** (forum -servire;
>   tabulariumd daemon; MCP over tabellarius)
> - doing it as a framework convention a scaffold emits: **DEEST**
>   (folds into G1/G3)
> - provenance-marked regenerable codegen: **DEEST** — the silva arc.
>   Gap G9 (long-line, not blocking).

---

## 11. OAuth and mobile Claude

The daemon grows one more flag:

```
$ rationes -daemon -mcp-publicus
```

which serves MCP over HTTPS behind OAuth — the flow mobile Claude
(claude.ai custom connectors) requires: authorization endpoint, token
exchange, per-persona scopes. Secrets (signing keys, client registry) come
from arca, never from files in the repo.

Now "add milk to the list" works from the phone in a Claude conversation,
against the same volumen your kitchen laptop has open.

> **STATUS**
> - everything here: **DEEST**. Gap G10 — OAuth server library
>   (auctoritas.h?, name unsealed) + arca dependency (arca itself is
>   design-banked, unbuilt). This chapter is the aspirational summit, and
>   it is also the stated driver (financial/budgeting MCP, diurnum
>   2026-08-03).

---

## THE GAP HARVEST

| # | Gap | Size | Depends on |
|---|-----|------|-----------|
| G1 | `aedilis novum` scaffold | S — template + generation, aedilis owns it | — |
| G2 | `volumen.h` — document = sqlite file, acta-in-file, projections | M — new lib, but scrinium+gesta are the parts bin | — |
| G3 | genera extraction from tabularium.c into a library | L — the keystone; 3 pulls recorded | G2 (or lands beside it) |
| G4 | `vitrea_genera_praebere` — generated CRUD UI as a call | M — extraction from forum JS + internuntius | G3 |
| G5 | `deducere` deploy verb (+ G5a amd64 lane) | M — composition of porta/villa/scp/systemd/nginx | — |
| G6 | imago/blobum field genus + upload path | S–M | G3 |
| G7 | personae: identity, roles, login → persona mapping | M | G3 |
| G8 | volumen replication: single-writer server, acta-after-seq pull, outbox | L — the big new design | G2 |
| G9 | provenance-marked regenerable codegen (silva) | L, long-line | silva annotationes |
| G10 | OAuth MCP (auctoritas) + arca | L; arca first | arca |

Reading of the table: **G2+G3 unlock more than half the tutorial** (ch. 2,
3, 4, 6, 7 all lean on them), match the forum's own announced migration,
and were already named the keystone before this document existed. G1 is
cheap and makes every later loop-iteration faster to red-team. G8 and G10
are the genuinely new designs and deserve their own interview/spec cycles
when their turn comes.

Loop protocol reminder: improvements to this document and to the libraries
come from walkthrough stumbles, not from speculation. Next crank: red-team
chapters 1–5 against today's reality with a context-free agent once G1–G3
exist even in embryo.

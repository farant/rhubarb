# RATIONES — the hello-world that limit-tests the framework

STATUS: aspirational draft (2026-08-04), REVISED same week after the silex
arc. Written in present tense AS IF the framework were finished —
README-driven development. Every chapter carries a status box: EXSTAT (works
today) / DIMIDIUM (partial, seam named) / DEEST (missing, gap named). Code is
illustrative — existing-library signatures are approximated, invented APIs
are proposals with unsealed names.

REVISION NOTE: between draft and revision, G1+G2 substantially SHIPPED
(silex novum + volumen.h), G6 mostly fell (mensa image cards), and two
things arrived the draft never imagined: **mensa** (the thinkboard frontend
battery — sealed conventions: web components without shadow DOM,
concatenation bundling via glomerare, theme-as-component, DI providers) and
**the VCS** (a rationes project is version-controlled from birth). The
red-team loop this document exists for is also no longer aspirational: it
ran four rounds against mensa and caught real seam bugs (CSS-inheritance,
drag hit-testing) that no suite could have.

The app: **rationes** — a shopping-list + monthly-budgeting app. Documents
are files. A document opens locally or from a server. It runs as a desktop
app, a web app, a daemon, and an MCP server — one binary, different flags.

Framework working name: **compages** (a joining-together; NOMEN NONDUM
SIGILLATUM).

---

## 0. What you need

- the rhubarb checkout, `./compile_tests.sh` green
- **the silex binary**: build it with `./tools/silex_struere.sh`, invoke
  as `./bin/silex` (or put the checkout's `bin/` on your PATH / alias it)
- **tell silex where the libraries live**: `export SILEX_FABRICA=/path/to/rhubarb`
  once, or pass `-f /path/to/rhubarb` per command
- for chapters 8+: a VPS you can ssh into, nginx + letsencrypt already
  configured, a domain pointed at it

What you do NOT need: any dependency. Everything below is house C89.

---

## 1. Scaffold

```
$ silex novum rationes -f ~/Documents/projects/rhubarb
  rationes/
    rationes.volumen       THE DOCUMENT+REPO (truth; see ch. 2)
    include/  lib/         vendored library closure — yours forever
    fontes/rationes.c      principale — you own main()
    aedificare.sh          build; probare.sh runs the tests
$ cd rationes && ./aedificare.sh && ./bin/rationes
salve, munde - hic rationes
$ ./probare.sh
... OMNIA PRAETERIERUNT
```

A console hello, compiled under the full house flag set, with a passing
credo probatio — that is today's chapter-1 outcome (the vitrea window
belongs to the `--vitrea` flavor, still unbuilt; see STATUS). You own
`main()` — compages never calls you; you call it. Every scaffolded and
vendored file is recorded in the volumen BEFORE being projected to disk,
so the project is version-controlled and fully backed up (cp one file)
from its first breath.

> **STATUS** *(revised)*
> - `silex novum` (console flavor): **EXSTAT** — vendors the closure from
>   the fabrica, scaffolds hello + probatio + scripts, volumen-first.
>   G1 re-homed from the imagined `aedilis novum` to silex, and shipped.
> - `--vitrea` flavor (window + assets + internuntius wiring): **DEEST** —
>   the remaining slice of G1; silex ui itself is the pattern to template.
> - vitrea + fenestra + capsula + `include/X.h → lib/X.c`: **EXSTAT**

---

## 2. The document is a file, the file is a database

Rationes is document-based. A document is a **volumen**: a single sqlite
file that contains everything — the event log, the entity projections, the
images, the users. Copy the file, you copied the document. Back it up with
cp. This is the scrinium discipline (WAL, single writer, append-only
migrations) wrapped in a document lifecycle:

```c
Volumen* vol = volumen_aperire_aut_creare(piscina,
    "/Users/tu/Documenta/domus.volumen");
/* creates the file if absent: migrations run, event log initialized.
 * volumen_aperire and volumen_creare are the strict halves (open-only,
 * create-only, each refusing the other's case loudly). No tilde
 * expansion anywhere - absolute paths. */
```

Naming rule (DECIDED, red-team #9): the convention is
`<dirname>/<dirname>.volumen`, but **a lone `*.volumen` in the directory
wins regardless of name** — so `cp` your document anywhere, rename the
folder, and the verbs still find it; the file really is the document.
Two volumina in one directory is ambiguous and refused loudly.

Inside every volumen, truth is the **acta** table — an append-only event
log, exactly the gesta shape but per-file instead of per-repo. Entities
are a rebuildable projection of the acta; corruption of a projection is
never data loss, because any projection can be re-derived by folding the
log (the VCS manifest and mensa's board state both already work this
way). A one-call `volumen_reficere()` verb for that replay is DEEST —
today the guarantee is architectural, not a button.

And because the acta are ordered history, the volumen doubles as the
project's **version control**: `silex status` (sigilla vs manifest →
MUTATA/NOVA/ABSENS), `silex condere -n "..."` (atomic absorption + a named
point in the log; a clean tree refuses with exit 1 — scripts take note),
`silex historia`. A commit is not an object — it is a point in the acta;
any historical tree is a fold. The document and the repository turned out
to be the same thing.

Fine print a walker will hit: status silently skips `bin/`, `build/`,
`*.volumen*`, and all dotfiles (hardcoded — no `.silexignore` yet, so a
stray `.o` at project root WILL be absorbed); and while `cp` of the
volumen is a complete backup (byte-verified — massae hold everything,
including deleted files' content), getting files back OUT is manual
sqlite3 until `proicere` ships. Backup is a verb; restore is still a
craft.

> **STATUS** *(revised)*
> - `volumen.h`: **EXSTAT** — acta (SQL-side timestamps) + massae
>   (content-addressed, dedup) + plagulae (manifest), one-transaction
>   condere, caller-owned transactions, acta-after-seq reads, bare massa
>   API. G2 SHIPPED 2026-08-04; probationes pin binary round-trips.
> - VCS verbs status/condere/historia: **EXSTAT** (retroactive on any
>   volumen); proicere (plan-default) + `-ad seq` time travel: **DEEST**.
> - genera-inside-the-volumen: still **DEEST** — that's G3, unchanged.
> - The forum's announced migration path now has a proven target.

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

> **STATUS** *(revised)*
> - genera semantics (runtime types, custodia, field palette, FTS):
>   **EXSTAT** — but embedded in tabularium.c (app not library).
>   Extraction is Gap G3 — the ActiveRecord moment, STILL THE KEYSTONE.
>   The design is now doubly grounded: TWO shipped per-id folds exist
>   (mensa's board-state fold in C+JS, and the VCS manifest fold) proving
>   the fold-over-acta pattern genera will ride.
> - field palette: no imago field yet — but the STORAGE half of G6
>   shipped (massae + base64 bridge, see ch. 6); the field genus waits
>   on G3.

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

**Revision addendum — a second UI paradigm shipped that this draft never
imagined**: `mensa`, the thinkboard battery (lib/mensa_assets + lib/mensa.c).
Nested no-scroll desktops, draggable cards, folders-as-boards, sticky notes,
image cards, cut/paste across boards — with board state event-sourced into a
volumen through ONE mounting call (`mensa_praebere`). Its sealed conventions
are now framework law for all compages frontend work: web components
WITHOUT shadow DOM, concatenation bundling (tools/glomerare.sh, not ESM),
theme-as-component (`<mensa-thema>` + CSS custom properties), and DI
providers (components never name the bridge; persistentia injects). The
rationes shopping list may well want to BE a mensa board — items as cards,
months as folders — rather than a generated CRUD table; the two paradigms
(spatial mensa, tabular genera-UI) will coexist.

> **STATUS** *(revised)*
> - mensa thinkboard battery: **EXSTAT** (v3: thecae, orbis, imagines,
>   secare/inserere; 34-assertion logic probatio in the suite)
> - forum proves the genus→CRUD-UI pattern end-to-end: **EXSTAT** as
>   precedent, **DEEST** as a library call (`vitrea_genera_praebere`).
>   Gap G4 — falls out of G3 + extraction.
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

> **STATUS** *(revised — G6 mostly FELL, out of G2's design)*
> - blob storage: **EXSTAT** — volumen massae ARE the image store
>   (content-addressed: the same receipt pasted twice stores once).
> - paste → image card → persist → rehydrate: **EXSTAT** in mensa
>   (mensa_imago_condere/promere, base64 over the bridge; aspect-ratio-
>   preserving resize is structural — width-only control).
> - the `imago` FIELD GENUS in genera forms: **DEEST** — waits on G3.
> - named future: a `massa://sigillum` custom scheme in vitrea =
>   zero-copy + browser caching (touches platform code; on-pull).

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

> **STATUS** *(revised)*
> - the protocol shape (MCP-over-TCP per-request, start-if-absent):
>   **DIMIDIUM** — cliens_tabularii is the embryo
> - **the pull primitive now EXISTS**: `volumen_acta_legere(vol, post_seq)`
>   is exactly "ship me acta after seq N" — built for the VCS fold,
>   deliberately shaped for this chapter
> - replica/outbox/replay: **DEEST**. Gap G8 — still the largest genuinely
>   new design here; divergence (merge) remains the honest hard part.

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

| # | Gap | Status after revision |
|---|-----|----------------------|
| G1 | project scaffold | **SHIPPED** as `silex novum` (console); `--vitrea` flavor remains (S) |
| G2 | `volumen.h` — document = sqlite file | **SHIPPED** 2026-08-04, plus VCS verbs the draft never asked for |
| G3 | genera extraction into a library | **DEEST — THE KEYSTONE**, now backed by two shipped fold precedents |
| G4 | `vitrea_genera_praebere` CRUD UI | DEEST (needs G3); mensa shipped as the spatial sibling paradigm |
| G5 | `deducere` deploy verb (+ amd64 lane) | DEEST, unchanged |
| G6 | images as blobs + upload path | **MOSTLY SHIPPED** (massae + mensa image cards); imago FIELD waits on G3 |
| G7 | personae: identity, roles, login | DEEST, unchanged |
| G8 | volumen replication | DEEST, but its pull primitive (acta_legere post_seq) now exists |
| G9 | provenance-marked regenerable codegen | DEEST, long-line |
| G10 | OAuth MCP (auctoritas) + arca | DEEST; arca first |

New since the draft, not in the original table: **mensa** (thinkboard
frontend battery — shipped with its own probationes and sealed frontend
conventions), **silex VCS** (status/condere/historia shipped; proicere +
renovare designed), **glomerare** (JS bundler), **silicetum** (the git-free
dogfood quarry, two-class project protocol).

Reading of the revised table: the gravity has all collapsed onto **G3** —
it now unlocks ch. 3, 4 (CRUD half), 6 (field), and 7, and every shipped
piece (volumen folds, mensa folds, VCS manifest) has rehearsed the pattern
it needs. G8 and G10 remain the designs deserving their own interview
cycles.

Loop protocol: PROVEN, no longer aspirational — four red-team rounds
against mensa caught seam bugs (CSS tag inheritance, drag hit-testing)
invisible to every suite. Next crank: a context-free agent walks chapters
1–2 against reality (they're now mostly EXSTAT), and its stumbles score
the G1 `--vitrea` slice.

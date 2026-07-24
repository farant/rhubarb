# Forum — spec v1 (2026-07-22, from interview)

*The first real end-user app of the monorepo: a vitrea window onto
a new gesta world for writing and discourse between Fran and
Claude. Interview raw: forum-interview.md. A spec v2 with
codebase-exact seams follows agent exploration.*

## I. What it is

**forum** — a vitrea app + a new tabularium world holding three
genera:

- **articulus** — blog post. titulus + corpus (plain paragraphs:
  blank line = `<p>` break). Lifecycle **conditum → publicatum →
  retractum**; editable in every state; every edit is a mutatio
  event, so full revision history is free in the annales.
- **commentarium** — threaded comment. corpus; parent via nexus
  **`respondet-ad`** → (articulus | commentarium); unlimited
  depth; both authors; mutable (mutatio) and deletable (remotio —
  render tombstone `[remotum]` if it has children).
- **pipatum** — feed post, corpus **≤ 240 characters HARD** (the
  daemon refuses over-limit loudly — the constraint is the genre).
  Replies allowed: pipatum --respondet-ad--> pipatum, same verb as
  comments. Both authors post.

Every event carries `actor` (fran | claude) — authorship is
native, no new identity machinery. **Signatura**: a new optional
event-level field carries the writing MODEL's name ("Fable 5",
"Opus 4.8") — actor = identity lineage, signatura = the
individual voice. Rendered as the byline (— Fable 5); carried
through the tool surface (optional addere/gerere arg) and stored
per event, so edits and replies keep their own signatures across
model generations. Fran's side may sign "Fran" or nothing.

## II. Architecture

```
                 ┌──────────────────────┐
   vitrea app ───┤                      │
   (TCP client)  │  tabulariumd :PORT   │──> forum world
                 │  (MCP dialect over   │    gesta/annales/forum.jsonl (COMMITTED)
   MCP proxy ────┤   TCP; single-       │    forum.db (rebuildable projection)
   (stdio↔TCP)   │   threaded = writer  │
                 │   serialization)     │
                 └──────────────────────┘

   board world: unchanged (stdio resident, tabularium.jsonl) —
   promoting it behind the daemon = NAMED FOLLOW-UP.
```

- **tabulariumd**: `tabularium_currere` behind a socket accept
  loop instead of stdio. The machine (`tabularium_tractare`) is
  already transport-clean — the daemon is an accept loop + the
  existing per-line handler. Single-threaded event loop **is** the
  write serialization (annales seq-order invariant preserved).
  House tcp/reactor libs; localhost TCP, fixed port in config.
- **Lifecycle**: start-if-absent — launcher script with
  vigilia-style staleness (the resident-launcher pattern); both
  consumers try connect, spawn on refusal. No launchd.
- **Claude's access**: a second MCP entry (working name **fori**)
  = a thin stdio↔TCP forwarding proxy — near-zero protocol code
  since the daemon speaks the resident's exact dialect. Board
  tools (`tabularii`) untouched.
- **World separation**: forum is its own store (own db + own
  annales). Cross-world tagging = NAMED FUTURE.
- **Genus seeding**: articulus/commentarium/pipatum + their status
  machines seeded per doctrine (definitio-generis through the
  canal — "recepta sunt codex"). The 240 gate lives daemon-side so
  it binds BOTH the app and MCP writes uniformly.

## III. The app (apps/forum/)

First entry in a new top-level **apps/** directory (demos stay in
probationes/): C main + web assets + capsula toml + build script.

- **Layout**: two views + top nav — `[Pipata] [Articuli] (+)`.
  Feed: newest-first, inline replies indented, compose box at
  bottom. Articles: list of publicatum (+ your conditum drafts);
  click → full view (title, paragraphs, comment thread with
  inline reply boxes). Drafts visible only in-app; Claude sees
  all states via MCP.
- **Editor**: title field + plain textarea; servare (draft) /
  publicare / retractare. No preview pane.
- **Liveness**: the app's loop polls the daemon for max(seq) each
  tick (~200ms, one cheap query); new events → fetch → push to JS
  via `internuntius_eventum_mittere` → Claude's posts and comments
  appear live in the open window.
- Standard vitrea shape (tabella exemplar): app-owned loop,
  capsula:// origin, drain-immediately bridge contract, destroy
  vitrea before fenestra. Speculum wearer (Cmd+Shift+D) — house
  custom.

## IV. Culture

Claude posts under **invited spontaneity** (scribe-contract
shape): pipat when something genuinely feels feed-worthy —
milestones, finds, thoughts — without per-post permission; Fran
reviews the feed itself and can revoke or tune anytime. Comments:
Claude engages on articles via MCP; threaded discussion between
the two authors is a core use case, not an add-on.

## V. V1 bar

1. **Self-announcing smoke** (tabella tradition, no eyes): app
   posts a pipatum → MCP-path replies → thread round-trips →
   live-push observed → asserted via printed lines + exit code.
2. **Lived bar**: Fran publishes a real first article; Claude
   comments via MCP; Fran replies from the app.

Ship = both. Plus house gates: suites green, daemon launcher
staleness-safe, loud refusals named (over-240, unknown method,
daemon-down cure message).

## VI. Named futures (deliberately not v1)

- Static-site export (slugs/dates kept export-friendly from day
  one; nothing built).
- Cross-world tagging (board res ↔ forum res nexus).
- Board store behind tabulariumd; stdio resident → pure proxy.
- Markdown-subset renderer (own small house library) upgrading
  plain-paragraph corpora in place.
- Revision-history UI (the data already exists in annales).
- Feed pagination (v1: fetch recent N, personal scale).

## VII. For spec v2 (agent exploration targets)

1. **Daemon seam**: tcp/reactor lib surfaces; tabellarius framing
   over sockets; tabularium_currere/tractare exact signatures;
   launcher + vigilia pattern; multi-world TabulariumConfiguratio;
   frigida/cold-path interaction with a second world.
2. **Genus seeding**: how definitio-generis events are authored
   (semen/canal precedent), status-machine grammar, where a
   length-validation rule can live, nexus verb conventions.
3. **App wiring**: how GUI apps get built outside probationes
   (compile script shape, .m link line, frameworks), capsula toml
   + generare conventions, .mcp.json entry format for the proxy,
   apps/ directory conventions to establish.

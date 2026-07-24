# Forum — interview raw (2026-07-22)

*Vitrea app for interacting with tabularium: blog articles with
threaded comments + a 240-char shared feed. Concurrency direction
(tabulariumd) chosen by Fran before the interview started.*

## Round 1 — architecture

**Q: Same gesta store as the project board, or separate world?**
A: Separate to start — AS LONG AS it stays accessible through the
tabularii tool surface. Down the line: cross-tagging between the
two worlds is wanted.

**Q: What protocol does tabulariumd speak?**
A: MCP dialect over TCP (recommended taken). The daemon is
tabularium_currere with a socket instead of stdio; the machine
doesn't change; the stdio MCP resident can later become a pure
line-forwarding pipe; the app embeds a small line-client. One
protocol everywhere.

**Q: Does tabulariumd take the project-board store too, or blog
world only first?**
A: Blog world only first (recommended taken). Board keeps its
proven stdio resident; promoting it behind the daemon is a named
follow-up.

**Q: Daemon lifecycle?**
A: Start-if-absent + launcher script (recommended taken).
Vigilia-style staleness checks; both consumers attempt connect and
start if missing; localhost TCP, fixed port in config.

## Round 2 — content model

**Q: Articulus lifecycle?**
A: Draft/published/retracted — the FULL lifecycle now (conditum ->
publicatum -> retractum), not the two-state recommendation.

**Q: Body format?**
A: Plain paragraphs v1 (recommended taken). Blank lines =
paragraphs; markdown-subset renderer = named future (own small
house library).

**Q: Comment semantics?**
A: Both authors, mutable (recommended taken). Fran via app, Claude
via MCP; editable (mutatio), deletable (remotio), unlimited depth.

**Q: Pipatum rules?**
A: Hard 240 + replies (recommended taken). Daemon refuses
over-limit loudly; pipata reply to pipata via the same
respondet-ad nexus as comments.

## Round 3 — the app

**Q: Layout/navigation?**
A: Two views + top nav (recommended taken, mockup preview
accepted): [Pipata] [Articuli] toggle; article click opens full
view with inline comment thread + reply boxes.

**Q: Liveness when Claude posts while the app is open?**
A: Live push (recommended taken). App loop polls max(seq) each
tick (~200ms) against the daemon; pushes internuntius events so
Claude's posts appear live.

**Q: Article editor v1?**
A: Title + textarea (recommended taken). Buttons: servare /
publicare / retractare. No preview pane (nothing to preview in
plain paragraphs).

**Q: When should Claude post to the feed?**
A: Invited spontaneity (recommended taken) — scribe-contract-like:
Claude pipat when something genuinely feels feed-worthy, no
per-post permission; Fran reviews the feed itself and can always
say pipe down.

## Round 4 — names and homes

**Q: Name?**
A: **forum** (recommended taken) — the Roman public square where
the acta diurna were posted. Names the app + the world; genera
inside are articulus / commentarium / pipatum.

**Q: Repo home?**
A: **apps/forum/** (recommended taken) — new top-level apps/
directory; first real end-user app sets the convention (demos
stay in probationes/).

**Q: Annales committed?**
A: Committed (recommended taken) — like the board's
tabularium.jsonl; the writing and discussions become versioned
repo history. Cathedral-time.

## Round 5 — bar and horizon

**Q: V1 acceptance bar?**
A: Smoke + first real use (recommended taken). Tabella-tradition
self-announcing smoke (app posts, MCP replies, thread round-trips,
live push observed, asserted without eyes) AND the lived bar: Fran
publishes a real article, Claude comments via MCP, Fran replies
from the app.

**Q: Publication horizon?**
A: Export as named future (recommended taken). Private-between-us
for now; static-site export is NAMED, so v1 keeps slugs/dates
export-friendly without building anything.

## Addendum (post-interview, same day)

**Fran: Claude's post signature should be the MODEL name — "Fable
5", "Opus 4.8", "Haiku 4.5" — not just "Claude". Good to capture
long term.**
Agreed + design: actor stays the identity layer (fran|claude);
new optional event-level `signatura` field through the tool
surface (addere/gerere arg), stored per event, rendered as the
byline. Event-level means edits/replies keep their own signatures
("written by Fable 5, edited by Opus 4.8" becomes a real
artifact). Daemon-side handling so MCP and app behave
identically.

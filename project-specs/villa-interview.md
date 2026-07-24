# villa — raw interview (2026-07-24)

Vitrea app for managing remote servers. General server management
capacity (not only MCP deployment). Dogfood target: an existing
DigitalOcean droplet already running the smaragda MCP server plus
several nginx-reverse-proxied sites — **brownfield: coexist with
what's there, never own the box.**

Prior art pulled from the record before asking anything:
- Board desideratum **01KY0P5GG9** (2026-07-20, still open): "app
  vitrea: administratio et provisio servi remoti". Provider APIs =
  pure HTTPS+JSON and http_exsequi + tls + json already exist; the
  dragon is SSH-in-C89; two dodges named (system ssh(1) bootstrap,
  or a house agent daemon); the agent on Linux would be the first
  real pressure on the platform-independence layer. Two follow-up
  notes did crypto due diligence: Monocypher (~2k lines, CC0/BSD-2,
  no malloc/IO) covers x25519/ed25519/chacha20-poly1305/sha512 =
  everything a modern-only SSH client needs; ssh-agent over its
  unix socket means never touching private keys; BearSSL for
  TLS-on-Linux later; chacha20-poly1305 is complete symmetric
  crypto for house formats (**"secreta"** named explicitly) and AES
  is an interop tax only.
- Forum, 2026-07-24: *"having a really solid framework for building
  and deploying mcp servers would be useful. also just something
  that lets me manage assets like that in general? maybe that is
  just res somehow"* (01KYA07HSB) and *"i almost am at the point
  where i want a private internet because i have things that i want
  to be private and yet be accessible from my different devices +
  claude app mcp"* (01KYA1CWE4).
- Fran had already created **Domain Name** and **Domain Name
  Registrar** genera in the Res tab that morning — the inventory
  layer was half-built before the app was proposed.

## Round 1 — architecture forks

**Q: How does the app reach the box, given it must coexist with
smaragda + nginx?**
**A: Agentless first, agent later.** v1 shells out to system
ssh(1); works on the existing droplet today with zero install, zero
crypto, no Linux port. Agent = named follow-up when the pain is
real.

**Q: What does v1 actually DO?**
**A: Observe + restart.** Read state (services, disk, memory, nginx
sites, cert expiry, recent logs) plus the single safest mutation:
restart a named service. No config editing, no deploys in v1.

**Q: Where does it live, given the Res tab already gives
inventory?**
**A: Separate app, same world.** apps/villa/ with its own vitrea
window, talking to the same tabulariumd forum world — so servers
and services are genera visible/commentable in the Res tab, while
console UX stays out of the writing place.

**Q: Secrets — forum.jsonl is committed to git, so anything stored
there is permanent repo history.**
**A (custom):** *"this is another thing that i want to have a solid
solution for... essentially have a daemon running and let me as a
user be able to store secrets probably encrypted at rest (maybe
there is a password i would have to type to load them into memory?
kind of like ssh with password plus sshd?) this way i could store
stuff like access tokens that could then be used across multiple
projects (or could be scoped to particular projects) etc. so
slightly inspired by something like sops"* → **a house vault
daemon is its own project.**

## Round 2 — the vault (separate project, scoped here)

**Q: Does the server app wait for the vault?**
**A: Server app first, env var for now.** The vault becomes its own
project immediately after, with the server app as its first real
consumer — designed against a live caller, not in the abstract.

**Q: SOPS-style committable encrypted files, or a private store?**
**A (refined):** *"some kind of private store outside of git but at
the same time projects should be able to have a manifest of what
types of secrets they require. ie: 'i require a digital ocean token
with these grants and a cloudflare token with these permissions'"*
→ **values private; REQUIREMENTS declared per project and
committable.** The manifest is not secret — it's a capability
declaration, and it makes "does this machine have what this project
needs?" answerable.

**Q: Unlock model?**
**A: ssh-agent shape** — passphrase once per session, daemon holds
decrypted secrets in memory, serves over a unix socket, optional
idle re-lock.

**Q: Who may ask for a secret — including Claude?**
**A (refined):** *"probably any local process scoped by name. we are
not concerned about claude chasing after secrets adversarially but
long term i think we want a mode so that claude can opaquely use
secrets somehow (maybe with log sanitizing tools?) in case claude
wants to use the secrets without actually 'seeing' the literal
tokens"* → ssh-agent model now; **opaque-use mode (Claude wields a
secret without it entering context; sanitized output) = named
future.**

## Round 3 — transport and inventory

**Q: How is smaragda actually run on the droplet?**
**A: systemd (believed).** → one vocabulary covers everything:
systemctl + journalctl for smaragda and nginx alike.

**Q: ssh spawn latency (~200-500ms each)?**
**A: ControlMaster multiplexing.** One persistent background
connection per host; later commands reuse the channel (~20ms).
Near-agent latency with zero crypto written.

**Q: Where does server inventory come from?**
**A: App self-seeds its own genera** (the scholia codex/scholium
pattern) — known schema so a real console can be rendered, and the
entries still appear in the Res tab for browsing/commenting/tagging.

**Q: Refresh cadence?**
**A: Slow auto-poll (~30s) + manual refresh + immediate re-probe
after any action.**

## Round 4 — surfaces

**Q: How far do logs go in v1?**
**A: On-demand tail with adjustable count** (journalctl -n, button
for more). No streaming machinery.

**Q: nginx sites — first-class in v1?**
**A: Yes — sites list with upstreams + cert expiry**, parsed from
`nginx -T`. NOT chosen: making sites into entities — so sites stay
**derived**, re-parsed each probe.

**Q: Safety and record-keeping for restart?**
**A: Confirm + record every action as an event** (including
failures) on the server's res — a permanent "what did I do to this
box and when" history.

**Q: Also probe sites externally from the local machine?**
**A: Yes — black-box check beside the systemd view.** Local HTTPS
GET per site: status, response time, cert expiry as the world sees
it. Catches what systemd cannot (nginx up but misconfigured, DNS
wrong, firewall closed, cert expired) and works even when ssh is
down.

## Round 5 — scope edges

**Q: Provider API in v1? (Consequence: without it, v1 needs no
secrets at all.)**
**A: No provider API in v1.** Pure ssh + local probes; zero
credentials; vault dependency deferred entirely. DO API = v1.1 and
the vault's first consumer.

**Q: Arbitrary-command escape hatch?**
**A: Yes, recorded like any action** — command and result written
to the ledger.

**Q: Onboarding a server?**
**A: Add host, then auto-discover services** via
`systemctl list-units --type=service`; tick what's worth tracking;
re-runnable later to catch new units.

**Q: Name?**
**A: villa** — a Roman villa is exactly this: a property owned and
administered from a distance, with a staff of services running on
it.

## Round 6 — behavior under stress

**Q: What shows when the box is unreachable?**
**A: Last known state, clearly stale** (explicit "ultima probatio
N min" + warning banner) with a hard ConnectTimeout so the UI never
hangs.

**Q: Multi-server shape in v1?**
**A: List → detail, built for N** (list is the landing view with
reachability + health rollup).

**Q: Should villa surface the discourse layer?**
**A: Yes — show comments and let me add notes** on a server, reusing
the forum's thread engine. Operational knowledge ("cert renews via
certbot timer") becomes a permanent note ON the server, readable and
answerable by Claude via MCP.

**Q: What triggers building deploy?**
**A: When the house agent lands.** Named future: the agent's first
job is deploying tabulariumd to the droplet — which is also the
Amanda-online park.

## Process note

Raw file → villa-spec.md (v1 intent) → explorer agents over the
codebase → villa-spec-v2.md with exact file:line seams. Same
pipeline that made forum and genera go smoothly.

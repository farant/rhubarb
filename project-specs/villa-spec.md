# villa — spec v1 (2026-07-24)

A vitrea app for administering remote servers: observe, restart,
read logs, watch nginx sites — over plain `ssh(1)`, against boxes
that already exist and already run other people's software.

Raw interview: villa-interview.md. Board: desideratum 01KY0P5GG9.
This is the intent document; spec v2 (post-exploration) carries the
exact file:line seams.

## I. Vision

A Roman villa is a property you own and administer from a distance,
with a staff running it in your absence. That is the whole design
brief: the box is *not* ours to reshape — the droplet already runs
smaragda and nginx, put there by other means — so villa **observes,
nudges, and remembers**, and never assumes ownership.

The strategic payoff is convergence. Three standing wants collapse
into one primitive once a server is manageable: a framework for
deploying MCP servers, an inventory of assets ("maybe that is just
res somehow" — it is), and a private-but-reachable internet for
Fran's own devices and the Claude app. v1 does not build those. It
builds the floor they all stand on, and it does it without a single
credential.

**The ledger is the differentiator.** Anyone can wrap ssh in a UI.
villa writes what you *did* — every restart, every ad-hoc command,
every failure — as permanent events on the server's res, in a world
Claude can read. Six months from now the answer to "why is this box
configured like this" lives beside the box.

## II. The line that shapes everything: telemetry is not history

**Probe results are NEVER written to the ledger.** Service states,
disk usage, response times, uptime — all ephemeral, held in app
memory, lost on quit, refreshed on launch. A 30-second poll writing
events would produce ~2,900 events/day of noise and drown the
annales.

**Actions ARE written.** Restart, arbitrary command, adding a
server, discovering services — each becomes an event carrying its
outcome (including failure). The annales stay a record of intent
and consequence, exactly as the board is a record of decisions
rather than metrics.

Consequence: staleness is a first-class UI concept, since there is
no durable state to fall back on (§VII).

**Refinement (2026-07-24, parcum 01KYAMZVFN):** sampling is not
history, but a *crossing* is. Metrics belong in a sibling store
(`metrum`, RRD-shaped, fixed-size forever) rather than in gesta —
lossy-by-design vs lossless-by-design are different storage
problems. The two join at thresholds: memory sitting at 40% is
metrum's business and produces no events; memory *crossing* 90% is
a thing that happened and belongs in gesta. A threshold check is
the transducer between them, which is also where alerting will
eventually live without inventing a third system. villa v1 still
discards its numbers — but its parsers already emit exactly the
(name, value) pairs metrum will want, so wiring it later is small.
Do NOT build a seam for it now.

## III. Architecture

### Transport: agentless, multiplexed

Every remote operation is `ssh(1)` invoked as a child process:

```
ssh -o ControlMaster=auto \
    -o ControlPath=<runtime-dir>/cm-%r@%h:%p \
    -o ControlPersist=300 \
    -o ConnectTimeout=5 \
    -o BatchMode=yes \
    [-p port] [-i keyfile] user@host '<script>'
```

- **ControlMaster/ControlPersist**: first command opens a master
  connection; subsequent commands reuse the channel (~20ms instead
  of ~300ms). Near-agent latency, zero cryptography written.
- **BatchMode=yes** is non-negotiable: it guarantees ssh never
  prompts interactively, which would hang the UI forever.
- **ConnectTimeout** bounds the worst case; the UI stays responsive
  by construction.
- **Authentication is borrowed, never handled.** Keys come from the
  user's existing ssh-agent / `~/.ssh`. villa stores a *key alias*
  at most, never key material. **v1 handles zero secrets.**
- **known_hosts is a precondition**: the host must already be
  trusted (BatchMode means no TOFU prompt). v1 refuses with a clear
  message rather than inventing a trust UI.

### Testability: the ssh binary is a seam

The path to `ssh` is configurable (env var / config). Tests point it
at a **stub script emitting canned fixtures**, so the entire suite
runs offline with no network and no live box — the same discipline
that made the hospitium suite offline. This is why the code must
separate three concerns cleanly:

1. **build a command** (pure string construction — testable)
2. **run it** (the only impure part — one function)
3. **parse the output** (pure — testable against captured real
   output fixtures)

### Data: two seeded genera, everything else derived

The app self-seeds its genera on first run (the scholia
codex/scholium pattern), so it knows its own schema:

- **`servus`** — titulus, hospes (host), usor (ssh user), portus
  (default 22), clavis (key alias/IdentityFile path, optional).
- **`servitium`** — titulus (display name), unitas (systemd unit),
  relatio → servus (cardinalitas unum).

**Derived, never stored**: nginx sites (re-parsed from `nginx -T`
each probe), all probe results, external check results. Deriving
sites means the app can never disagree with the box about what it
serves — the same reasoning that made tags a projection.

### World

Same forum world via tabulariumd (per-request TCP client, the
forum's proven pattern) — so servers and services appear in the Res
tab, carry tags, and accept comments. The `transmittere` passthrough
handler already shipped in the forum gives arbitrary tool access
with `actor` injection; villa needs the same (or a shared copy).

## IV. What v1 does

### Probe (one batched script per server per refresh)

A single composite shell script per refresh — one spawn regardless
of how many services are tracked:

- `uptime`, `df -P /`, `free -m` (or /proc fallbacks)
- `systemctl is-active <unit>` (or one `systemctl show` per tracked
  unit for state + since)
- `nginx -T` when the sites panel is relevant (heavier; may refresh
  on a slower cadence than services)

Parsed into an in-memory snapshot with a timestamp.

### External black-box probe (no ssh)

For each site discovered from nginx: an HTTPS GET from the local
machine using the house http+tls client — status code, response
time, and TLS certificate expiry **as the world sees it**. This is
deliberately a second, independent perspective: it catches nginx
running but misconfigured, DNS wrong, firewall closed, or an
expired cert — and it keeps working when ssh is down.

*(Open: whether the house TLS layer exposes peer certificate expiry.
If not, fall back to reading cert files over ssh. v2 prices this.)*

### Actions (confirmed, recorded)

- **restart a service** — `systemctl restart <unit>`, confirmation
  step, immediate re-probe, event written with outcome.
- **arbitrary command** — free-text, run over the same channel,
  output displayed, command + exit status + output head recorded.
  The escape hatch is honest about what the app is.
- Both write a nota/mutatio event on the servus res; failures are
  recorded with the same weight as successes.

### Logs

On-demand `journalctl -u <unit> -n <count> --no-pager`, count
selector (100 / 500 / 2000), refresh button. No streaming in v1.

### Onboarding

Add-host form (host, user, port, key alias) → immediate reachability
check → `systemctl list-units --type=service --state=running` →
checkbox list of discovered units → ticked ones become `servitium`
entries. Re-runnable later to catch new units. Roughly a minute from
nothing to a fully modeled droplet.

## V. UI (vitrea + internuntius, forum conventions)

- **List view** (landing): one row per servus — name, reachability
  dot, service health rollup ("3/3 activa"), last-probe age. Built
  for N from the start.
- **Detail view**: header (host, uptime, disk, memory, **staleness
  marker**), services table (unit, state, since, [restart] [logs]),
  sites table (domain, upstream, external status + ms, cert days
  remaining), command box, and the **commentaria thread** on this
  server with a compose box — reusing the forum's thread engine, so
  "cert renews via certbot timer" becomes a permanent note on the
  box that Claude can read and answer over MCP.
- **Logs panel**: monospace, count selector, refresh.
- Warm-dark theme, 22px, speculum wearer (Cmd+Shift+D), rm-before-cp
  in the launcher — all app conventions inherited.

## VI. Degradation

- Hard `ConnectTimeout`; probes run without blocking the UI.
- On failure: **keep showing the last successful snapshot**, marked
  explicitly ("ultima probatio 14 min") with a warning banner. The
  state you most want during an outage is what was true before it.
- External probes continue independently — a server can be
  ssh-unreachable while its sites are demonstrably fine, and villa
  should show exactly that.

## VII. The bar

1. **Parser gates** (offline, fixture-based): systemctl output,
   `nginx -T` (sites → upstreams → cert paths), `df`/`free`/uptime,
   `journalctl` framing, and the ssh command builder (flags, port,
   key, quoting).
2. **Fumus with a stub ssh**: add server → discover services →
   probe → render list and detail in the DOM → restart → assert the
   action event landed in the annales. Entirely offline.
3. **Lived bar**: point villa at the real droplet; see smaragda and
   nginx as tracked services; see the actual sites with upstreams
   and cert days; tail a real log; restart something harmless; find
   the action recorded in the ledger afterward; write one operational
   note on the server and read it back over MCP.

## VIII. Out of scope (named, not vague)

- **House agent daemon** — the named follow-up; first Linux port of
  the platform layer; unlocks streaming, push, supervised deploys.
- **Deploy** — waits for the agent. Its first job: deploy
  tabulariumd to the droplet, which is also the Amanda-online park
  (01KY6449PF).
- **Provider API (DO)** — v1.1; droplet metadata, out-of-band
  reboot, snapshots. The vault's first consumer.
- **Vault daemon** — its own project (§IX).
- **metrum** — time-series sibling store (parcum 01KYAMZVFN);
  villa is its intended first producer, and sparklines beside
  services/sites are the first payoff. Alerting arrives with it,
  as threshold-crossings written into gesta.
- Config editing (nginx), cert management, provisioning new
  droplets, multi-user.

## IX. The vault, scoped but not built here

Separate project, interviewed alongside villa because it surfaced
here. Recorded so the design isn't lost:

- **Private store outside git**, encrypted at rest. Crypto already
  scouted in July: Monocypher's XChaCha20-Poly1305 + Argon2.
- **Projects declare requirements in a committable manifest** ("this
  project needs a DigitalOcean token with these grants, a Cloudflare
  token with these permissions"). Requirements are public; values
  never are. Makes "does this machine have what this project needs?"
  answerable, and new-machine setup tractable.
- **ssh-agent unlock shape**: passphrase once per session, decrypted
  secrets held in memory, served over a unix socket, optional idle
  re-lock.
- **Access**: any local process, scoped by name.
- **Named future — opaque use**: a mode where Claude can *wield* a
  secret without seeing it (substitution at the last moment plus
  output sanitizing), so an agent can deploy without the token ever
  entering its context.

## X. Open questions for v2 exploration

1. Process spawn with output capture in the house: what exists
   (systema_posix process coverage), and does anything support
   incremental reads (needed later for streaming logs)?
2. Does the house TLS client expose peer certificate expiry? If not,
   the cert-days column comes from the box instead of the world.
3. Does the http client do HTTPS GET against arbitrary external
   hosts cleanly today (the offline-suite exclusions suggest yes but
   under `-reticularis` gating)?
4. Reuse vs copy of the forum's `transmittere` passthrough handler
   and its per-request daemon client — shared library, or duplicated
   into apps/villa?
5. Runtime dir convention for the ControlPath socket (must be short:
   unix socket paths cap near 104 bytes on macOS — a long path
   silently breaks multiplexing).
6. Genera self-seeding across two apps: forum seeds codex/scholium,
   villa seeds servus/servitium — any collision risk in the shared
   world, and who owns a genus if both would seed it?
7. Fixture capture: real `nginx -T` and `systemctl` output from the
   droplet, sanitized, committed as test fixtures.

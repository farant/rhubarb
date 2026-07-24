# fixa villae — captured 2026-07-24

Real output from a live Debian droplet (nginx reverse proxy + several
systemd-managed app services), captured by `captare.sh` and sanitized
by hand before committing.

## Discipline (officina/captare.sh pattern)

Every command was run **twice** and pinned only if the two runs were
byte-identical. Results:

| fixture | verdict |
|---|---|
| `nginx_T.txt` | PIN — identical across runs |
| `systemctl_show_activae.txt` | PIN |
| `systemctl_show_ignota.txt` | PIN |
| `systemctl_show_inactiva.txt` | PIN |
| `list_units.txt` | PIN |
| `df.txt` | **VARIANS** — block counts drift |
| `uptime.txt` | **VARIANS** — obviously |
| `meminfo.txt` | **VARIANS** — MemAvailable/Cached drift |

The three VARIANS files are single-capture snapshots. Their **format**
is what the parsers are pinned against; their **values** are a moment
in time and no test should assert a specific number from them. The
twice-capture is what proved the variance is confined to values (the
diffs are numeric only — column layout identical), which is exactly
why the discipline exists rather than assuming.

## Sanitization

Committed files have real domains, app-service names, and internal
localhost ports replaced with `exemplum`-family equivalents. **Every
structural feature is preserved byte-for-byte otherwise.** The mapping
lived only in the shell that produced these files — deliberately not
committed, since a committed mapping would defeat the sanitization.

Raw captures stay in `cruda/` (gitignored) as a local reference for
debugging against the live box.

## What these fixtures actually specify

Written down because the parsers' correctness is defined by surviving
these shapes, and several were surprises worth naming:

**`systemctl show`**
- Properties come back in **systemd's order, not the order requested**
  (`MainPID`, `NRestarts`, `Id`, …). Parsers must be key-driven; a
  positional parser would be wrong.
- Multiple units in one call are separated by **blank lines**.
- Empty values are normal: `UnitFileState=` and
  `ActiveEnterTimestamp=` appear bare on an unknown unit.
- Unknown unit ⇒ `LoadState=not-found`, `ActiveState=inactive`,
  `SubState=dead`, and `Description` echoes the requested name.
- `ActiveEnterTimestamp` is a **human string** (`Fri 2025-09-12
  19:42:15 UTC`), not epoch. v1 displays it verbatim; if a "running
  for N days" figure is ever wanted, ask for
  `ActiveEnterTimestampMonotonic` instead of parsing this.

**`nginx -T`**
- **Tabs and spaces are mixed within one file.** Any indentation
  assumption is wrong.
- Certbot produces **paired** server blocks per site: an HTTPS block
  holding `proxy_pass` + `ssl_certificate`, and a companion HTTP block
  that only does `return 301`. A parser that assumes one block per
  site double-counts.
- `proxy_pass` lives **inside `location`**, one brace deeper than
  `server_name` — so a flat "directives of this block" scan misses it.
- Not every server block has an upstream: there is a `server_name _`
  default with a static `root`, and the redirect blocks have neither
  `proxy_pass` nor `ssl_certificate`.
- `listen` appears as `80`, `[::]:80`, `443 ssl`, `[::]:443 ssl
  ipv6only=on`, and `80 default_server`.
- Directives carry trailing comments (`# managed by Certbot`).
- Multiple names on one `server_name` line (apex + `www`).

**`list_units`**
- Has a **legend footer** after a blank line. Parsers must stop at the
  blank line or they will read prose as units.
- `DESCRIPTION` is the last column and contains spaces — split on the
  first four fields only.

## Re-capturing

`./probationes/fixa/villa/captare.sh <ssh-alias>` — read-only, needs
passwordless sudo for `nginx -T`. Sanitize by hand afterward; never
re-sanitize at test time.

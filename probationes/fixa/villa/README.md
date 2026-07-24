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

## The failure shapes (added 2026-07-24, after V3 shipped)

The original set covered only healthy output. For a tool whose entire
purpose is showing you when something is broken, that meant the
parsers had never seen the case they exist for — and would have met
it first at the worst possible moment.

Captured by deliberately failing a throwaway systemd unit
(`captare_fracta.sh`, which **mutates the server** and is separate
from `captare.sh` for exactly that reason):

| fixture | what it pins |
|---|---|
| `systemctl_show_fracta.txt` | `ActiveState=failed`, `Result=exit-code`, `ExecMainStatus=1`, `UnitFileState=static` |
| `systemctl_show_medio_ignota.txt` | an unknown unit **between** two healthy ones |
| `list_units_fractae.txt` | a failed unit in list form |
| `list_units_vacuae.txt` | header + blank + `0 loaded units listed.` and no rows |
| `journalctl_vacua.txt` | the multi-line `Hint:` preamble before `-- No entries --` |

**Two traps this exposed**, both now asserted in §XVII of the test so
that a comment isn't the only thing guarding them:

1. **`ActiveEnterTimestamp=` is empty for a failed unit, an inactive
   unit, *and* an unknown unit alike.** An empty timestamp therefore
   says nothing about which case you are in — using it as a proxy for
   "not found" would be wrong.
2. **`Result=success` is returned for units that are dead and for
   units that do not exist.** Displaying it unconditionally would put
   "Result: success" next to a service that isn't running. The value
   is stored verbatim (the reader must not lie about what systemd
   said), but the derived `fracta` flag carries the judgment, and
   `causa_finis` means nothing unless `fracta` is true.

`Result` and `ExecMainStatus` were added to the property list at the
same time; the healthy fixtures were re-captured so the whole set
shares one property list.

## Re-capturing

`./probationes/fixa/villa/captare.sh <ssh-alias>` — **read-only**,
needs passwordless sudo for `nginx -T`. That promise is why the
failure capture lives elsewhere.

`./probationes/fixa/villa/captare_fracta.sh <ssh-alias>` —
**mutates the server**: creates a unit that fails, captures, removes
it. Cleanup runs from a `trap ... EXIT` so a mid-capture failure
still tears the unit down; verify afterward with

```
ssh <alias> 'systemctl list-units --state=failed --plain --no-legend'
```

Sanitize by hand afterward; never re-sanitize at test time.

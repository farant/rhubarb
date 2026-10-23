# nomina_posix.worklog.md

## 2026-10-22 - born (park ...NPMN7; Fran approved the forked-child design)

POSIX has no non-blocking resolver: `getaddrinfo` blocks, and
`getaddrinfo_a` is glibc-only and uses threads (the house has none). So a
forked CHILD calls `getaddrinfo` - through the system resolver
(nsswitch, /etc/hosts, systemd-resolved), the same principle as the
macOS file: our answers match every other program's - and writes ONE
fixed-size `NominaResponsum` down a pipe (~2.2 KB < PIPE_BUF, so the
write is atomic), then `_exit`. The read end goes into the caller's
reactor; on a full message the child is reaped (it exits right after
the write, so the blocking `waitpid` is momentary) and `finis` runs on a
0 ms timer, never inside a callback or inside `incipere`.

- **The port goes in as the service string** (`AI_NUMERICSERV`):
  getaddrinfo hands back sockaddrs with the port already set, so this
  `.c` file (judged by examen, unlike the `.m`) never touches
  `sockaddr_in6`.
- **Literal IPs never fork:** `AI_NUMERICHOST` is answered in-process
  (it cannot block).
- **Errors:** `EAI_NONAME` (and `EAI_NODATA` where defined) -> "nomen
  ignotum", the same text as macOS; anything else "DNS erratum: " +
  `gai_strerror`. A child that dies before a full message ->
  "resolutio fallita (processus filius sine responso)".
- **Abort** kills (SIGKILL) and reaps the child: no zombie. No timeout
  of its own, as on macOS (the caller's connect deadline covers it).
- `nomina_familias_alternare` moved to the portable `lib/nomina.c`
  (header `<aedilis corpus>`; the platform file is added by aedilis
  convention, as pseudoterminale does). On macOS the generated source
  list still picks `nomina_macos.m`, never this file.

Tests: the existing `probatio_nomina` sections run unchanged under
Linux musl + glibc; new `abruptum_filius` (abort x III while the child
runs -> nothing in the reactor, `waitpid(-1, WNOHANG) == -1`) and the
localhost section now also asserts no child is left. Plant (abort
without reaping) -> red on Linux, restored -> green. `http_vocatio`
(383) and `horreum_vocatio` (122) now link and pass under Linux - the
point of the park. No gate runs suites under Linux (the Linux gate only
compiles lib/*.c); `tools/linux/probatio.sh` is by hand.

One flake on the way, in the TEST: the first `abruptum_filius` polled
once between incipere and abort; a child answering from /etc/hosts can
be read AND the 0 ms finish timer fire in that single poll, so `finis`
legitimately came before the abort. Seen once in ~30 runs; the test
no longer polls there. 16 clean runs after the fix.

Commit hook refused the first attempt: examen's lexicon had no
AI_NUMERICHOST / AI_NUMERICSERV / EAI_NONAME (healed in
silva/fontes/systema_posix.h, see its worklog).

Still missing for real HTTPS from Linux: `fiducia_posix.c` refuses every
chain (fiducia_linux not written) - next.

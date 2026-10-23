# canalis worklog

## 2026-10-21 - T1: the channel (canalis-spec II.1)

`include/canalis.h` + `lib/canalis_posix.c`: AF_UNIX listen / accept /
connect, peer uid, all connections handed back as ordinary `TcpConnexio*`
through `tcp_ex_fossa`.

- **The directory is the wall.** Between `bind` and `chmod 0600` the
  socket carries umask bits; nobody else can reach it because the parent
  is 0700 (created so, or refused when it has any group/other bit - never
  tightened silently). Plant without the chmod: the socket came out 0755
  under umask 022 (`modi` red).
- **Stale socket removal relies on the caller's single-writer lock**
  (header precondition): `audire` deletes a SOCKET at the path, refuses
  anything else (`alienum` pins that a regular file survives).
- **Accepted fds:** BSD/macOS inherit O_NONBLOCK from the listening
  socket, Linux does not - `accipere` sets it explicitly. Every fd gets
  FD_CLOEXEC so children never inherit the channel.
- **Peer uid:** `getpeereid` (macOS) / `SO_PEERCRED` with `struct ucred`
  (Linux; needs `_GNU_SOURCE`, defined under `__linux__` right after
  `postulata_posix.h` - plan P1, probe compiled on all three targets
  first). Unreadable credentials = refused. The cross-uid refusal itself
  cannot be tested without root; `par` proves the uid is really read
  (plant "uid 0 without reading" -> red) and the comparison is one line.
- **Trap:** `filum_existit` is `fopen(via, "r")` - a socket NEVER
  "exists" by it. My first `claudere` / `directorium_apertum` assertions
  used it and were vacuous (would pass with no delete at all); the suite
  now uses `_adest` (`filum_status`), and the `vetus` section caught it
  by failing the opposite way.
- **Tool gap closed, not routed around:** examen REICE'd the file
  (sockaddr_un, AF_UNIX, ECONNABORTED, getpeereid, geteuid, socketpair,
  S_ISSOCK unknown) while clang was happy - the lexicon-gap pattern of
  systema_posix.worklog.md. Added the Darwin forms there; auspex asserts
  them against the real headers.
- Linux musl + glibc: canalis 45/45, tcp 24/24.

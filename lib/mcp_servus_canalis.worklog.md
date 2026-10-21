# mcp_servus_canalis worklog

## 2026-10-21 - T2: MCP over the channel (canalis-spec II.2)

Server on the reactor (accept -> peer check -> slot of VIII; newline
lines -> `mcp_servus_tractare`; one reply line into a per-connection
write queue, EAGAIN -> write interest), plus two blocking clients:
`pons` (stdio bridge, raw fds + poll) and `vocare` (one tool call).

- **Half-close is not close.** `pons` does `shutdown(SHUT_WR)` at stdin
  EOF and then waits for replies, so EOF on read only stops reading; the
  connection closes once the write queue is empty AND no deferred reply
  is pending. If nothing is left to read or write but a deferred reply is
  pending, the fd leaves the reactor (a permanent POLLHUP would otherwise
  spin the loop); the late reply is written directly and EPIPE closes.
  REACTOR_ERROR closes at once.
- **Generation number** guards deferred replies: a connection closed with
  a reply pending (here: over-long line) frees its slot, a new client
  takes it, and the stale reply must not reach the newcomer. `fuga` could
  NOT pin this (the fleeing client keeps its slot until its reply comes),
  so section `locus_iteratus` forces close-with-pending + reuse; plant
  "generation check removed" -> red there.
- **Darwin SIGPIPE despite SO_NOSIGPIPE** (found here, fixed in tcp):
  a socket ACCEPTED after the peer already closed raises signal 13 on
  send with SO_NOSIGPIPE set; MSG_NOSIGNAL prevents it. Probes: socketpair
  never, accept-then-close never, close-then-accept always. `fuga` and
  `linea_imperfecta` hit it (client writes and closes before the server
  accepts). See tcp_posix.worklog.md.
- **Test lessons:** `_paratus`'s probe connection still held slot 0 when
  `nonus` opened its eight, so the EIGHTH was refused - the rule was
  right, the test raced; it now waits 100 ms. A plant that makes `pons`
  spin hung the suite in a blocking `waitpid` (killed by hand after 19
  min) - both pons sections now wait with a deadline
  (`_exspectare_filium`) and the plant goes red alone in ~2 s. Server
  death is diagnosed by `_vivus` (prints signal / exit code) - that is
  how signal 13 was seen at all.
- `initiatum` belongs to the whole McpServus, not to a connection: after
  the first initialize, other connections are not forced to initialize
  (our clients always do). Noted in the header.
- Linux musl + glibc: 87/87.

## 2026-10-21 - CORRECTION: half-close did not work (whole-branch review)

The T2 entry above claimed half-close semantics ("EOF stops reading only;
what is owed is delivered"). It was false: `_legere` read with
`tcp_recipere`, which on EOF sets `connexio->clausa`, after which
`tcp_mittere` returns -1 without calling send(). So after a client's
SHUT_WR (exactly what `pons` does at stdin EOF) a deferred reply was
dropped and a reply larger than one socket buffer was cut at ~8 KiB -
and pons still exited 0. The `pons` section passed only because its
reply was small and synchronous (written before the EOF was read). The
reviewer found it by reading tcp_posix.c and confirmed it with a probe.

Fix: `_legere` reads with raw `recv` on the fd (EOF -> finis_lectionis
only; the TcpConnexio stays open for writing). Pinned by `pons_dilatum`
(tarde after stdin closed) and `pons_magnum` (CC KB reply after stdin
closed) - both red before, green after; plant "back to tcp_recipere" ->
red. Lesson: a half-close test needs a reply that is still OWED when the
EOF arrives (deferred or larger than the socket buffer), not one already
written.

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

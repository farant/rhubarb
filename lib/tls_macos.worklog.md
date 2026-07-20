# tls_macos.m worklog

## 2026-07-17 — sentinel surgery + dead partial-read check

Part of the VANA adjudication (quaestio 01KXQ9NF8J class 1):
tls_mittere/tls_recipere returned i32 (unsigned house type) with a
documented -1 sentinel — every `n < 0` check on them was dead.
Signatures now s32 (matching the documented design), internal
(i32)-1/(i32)processed casts became -1/(s32)processed, and the two
SecureTransport callbacks' `i32 n` locals became s32 (their n<0
guards on tcp_recipere/tcp_mittere were dead the same way).

SECOND BUG found during the same read (_tls_read_callback): the
partial-read check ran AFTER `*dataLength = (size_t)n;` — i.e. it
compared n against itself, always false, so partial reads never
returned errSSLWouldBlock (the SecureTransport contract). Fixed by
capturing `petitum = *dataLength` at entry and comparing n against
that. The check was INTENDED (it existed!) but the ordering killed
it — same disease as the whole arc: designed right, implemented
dead.

NOTE: .m files are invisible to examen/percursus (walks .c/.h only)
— the VANA census could never have found this file. Grep is the
only meter here; remember that when auditing platform layers.

## 2026-07-20 — read callback remapped onto the TCP_ITERUM contract

tcp_recipere's 0-return used to mean both EOF and would-block, and
the read callback mapped 0 → errSSLWouldBlock — so a peer that
closed mid-read looked like "no data yet" and SecureTransport
would spin/wait instead of reporting closure. With TCP_ITERUM (-2)
the mapping is now honest: ITERUM → errSSLWouldBlock, <0 →
errSSLClosedAbort, 0 → errSSLClosedGraceful. Order matters: the
ITERUM check must precede n < 0 because the sentinel itself is
negative.

CAVEAT: this path is validated only by the live -reticularis TLS
run — compile is green but a manual re-smoke is still owed before
trusting it.

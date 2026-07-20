# tcp_posix.c worklog

## 2026-07-20 — hospitium Phase 0: recv contract + SIGPIPE + accepted-fd options

The recv return contract had an EOF/would-block collision: both
returned 0, so no caller could tell "peer closed" from "nothing
yet". New contract: >0 data, 0 EOF, TCP_ITERUM (-2) would-block,
-1 error. tcp_mittere deliberately KEEPS 0 = would-block (no EOF
collision exists on the send side); the asymmetry is documented in
tcp.h. Same disambiguation on accept: EAGAIN now returns
TCP_ERROR_ITERUM (appended to the enum — never mid-enum) instead
of a fake TCP_ERROR_IO. TCP_ITERUM (s32 sentinel) and
TCP_ERROR_ITERUM (TcpError) are deliberately distinct channels.

_ponere_nosigpipe applies SO_NOSIGPIPE unconditionally on both
connect and accepted fds. It lives OUTSIDE _applicare_optiones on
purpose: that helper early-returns on !opt, and NOSIGPIPE must
never be skippable — a server that writes to a reset connection
without it dies of SIGPIPE. Accepted fds also get TCP_NODELAY
(via _applicare_optiones with a local {0, VERUM, FALSUM}).

Caller audit before the change (exhaustive): tcp_recipere ←
tls_macos.m read callback (the ONLY ripple — see its worklog),
http.c recv loop (branches only on n>0, needed zero edits), tests.
Only one switch on TcpError exists (tcp_error_descriptio).

Also the 01KXJ2QJ8X flake: probatio_accipere_connexio had carried
"Blocking mode evitat race condition" since forever, but
multi_connexiones and integratio_http never got that fix, and
ponere_non_blocans dialed httpbin.org live. All three fixed
(blocking servus / loopback rewrite); suite ran 25/25 green.

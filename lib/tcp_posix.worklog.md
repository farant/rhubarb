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

## 2026-07-28 - retemptatio EINTR (hygiene POSIX)

connect/recv/send nunc EINTR retemptant (commissum 30190d6).
connect subtilis: post EINTR connexio in fundo pergit - retemptatio
quae EISCONN reddit SUCCESSUS est, non error. Sine hac hygiene
signum quodlibet (debugger, profilator, SIGCHLD) syscallum
interrumpit et 'Interrupted system call' ut errorem connexionis
falso refert - sub lldb mensuratum antequam sanatum. EISCONN 56
in lexicon per auspex_posix (TU ambo consentiunt). Fortasse
fluctuationem probatio_tcp_servus veterem sanat (01KXJ2QJ8X
observandum).

## 2026-08-03 - sonda glibc prima (Linux arc): duo inventa, CONSULTO non sanata

Sonda Docker (ubuntu 22.04, gcc 11.4, glibc 2.35, -std=c89 + vexilla
domus): plagula nuda 20 errores dat, quorum 18 macrone probationis
proprietatum sanantur (_DEFAULT_SOURCE aut _XOPEN_SOURCE=600 - radix
una getaddrinfo/addrinfo celata, cascata ~15 errorum ex UNA radice).
Duo residua = inventa vera:

1. **SO_NOSIGPIPE (linea 105) Darwin-solum est.** Nullus macro sanat.
   Linux aequivalens = MSG_NOSIGNAL per send, aut SIGPIPE ignorare.
   Vernaculum in plagula 'posix' nominata - specimen primum stratae
   diagnosticae portabilitatis.

2. **usleep (linea 329) XSI obsoletum est: MENSURATUM sanari sub
   _XOPEN_SOURCE=600 et _DEFAULT_SOURCE, NON sub 700, NON sub
   _POSIX_C_SOURCE=200112L** (XPG7 usleep sustulit). Ergo 700 NON
   superset 600 ad usum domus. Via moderna = nanosleep.

AMBO relicta ut sunt: Fran decrevit stratum diagnosticum (codex
examinis) ante emendationes aedificari - specimina fracta ei opus
sunt. Sanatio post codicem probatum.

## 2026-08-03 (sero) - specimen retiratum: SO_NOSIGPIPE portabiliter solutum

Fumus tabulariumd sub Linux specimen coegit: clausura daemonis hanc
plagulam fert, error electus nexum verum obstruxit. Solutio plena
(non tantum bracchium): Darwin SO_NOSIGPIPE in bracchio __APPLE__
(codex 86 quiescit ut calibratio praedixit), Linux MSG_NOSIGNAL in
missione ipsa (tcp_scribere). Pinnae portae remotae; glibc 133/136.
Codex 86 vigilat pro Darwinismis FUTURIS - specimen primum suum
munus implevit.

## 2026-10-14 - non-blocking connect + the fd leak in tcp_claudere (http-vocatio T3)

**The leak.** `tcp_recipere` sets `clausa` when `recv` returns 0 (the
peer closed). `tcp_claudere` then skipped `close()` because its guard
was `fd >= 0 && !clausa` - so every connection the SERVER closed first
leaked its fd. That is every `http_exsequi` (it reads until close) and
every TLS connection closed by the peer. Short-lived tools never
noticed; a long-running vicus asking questions over route B would
have run out of fds. `probatio_tcp_incipere` section `clausura paris`
showed it live (the fd still open after `tcp_claudere`, `fcntl
F_GETFD` != -1) before the fix. Now `tcp_claudere` always closes a
non-negative fd and is idempotent. Possibly related: the open ledger
question "probatio_tcp_servus fluctuans" - not checked.

**Non-blocking connect** for the reactor-driven client:
- `tcp_incipere(addressus, mensura, piscina)` - raw sockaddr bytes with
  the port already set (nomina fills it, plan R3), so this file never
  needs `sockaddr_in6`. Socket `O_NONBLOCK`, `SO_NOSIGPIPE`,
  `TCP_NODELAY`, and deliberately NO `SO_RCVTIMEO/SO_SNDTIMEO`: the
  caller's reactor timers own time. `connect` returning `EINPROGRESS`
  (or `EINTR`, which on a non-blocking socket also means "continues
  asynchronously") is success-pending.
- `tcp_incipere_perficere` - call ONLY after the fd is writable:
  `getsockopt(SO_ERROR)` 0 = connected, else a named error. Called
  before writability it would report 0 falsely.
- `tcp_ex_fossa` wraps an existing fd (tests over pipes/socketpairs).

Lexicon: examen REICE'd this file for `EINPROGRESS` and `SO_ERROR`
(the predicted gap); both added to `silva/fontes/systema_posix.h` with
Darwin values and asserted in auspex (plant `EINPROGRESS 37` ->
DISSENTIT).

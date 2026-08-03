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

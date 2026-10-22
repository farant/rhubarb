# systema_posix.h worklog

## 2026-09-14 — ENOENT, lstat, rmdir, symlink

Found by `filum_arborem_delere` (`lib/filum.c`, briar plan 7 Task 2),
the first committed C code to use any of the four. The examen refused
`lib/filum.c`: `identificator ignotus in expressione: ENOENT` (a
violation, so REICE) plus `vocatio implicita` for `lstat` and `rmdir`;
the probatio drew the same for `symlink`. clang accepted all of it. The
lexicon was behind the real headers; the code was right.

Added in the sections the judged file's own `#include` lines already
evoke: `lstat` beside `stat` (sys/stat.h), `rmdir` and `symlink` beside
`unlink` (unistd.h), `ENOENT 2` at the head of the errno.h section
(Darwin value). The prototypes follow the real ones.

**Certification.** `officina/auspex_posix.sh` gained
`IUNGO_ASSERTA(ENOENT == 2)`; both TUs CONSENTIT. Plant: `ENOENT 3` in
this file → TU nostrum DISSENTIT (`assertum_161`, array of negative
size), restored → CONSENTIT. The gate certifies sizes, offsets and macro
VALUES only. Function prototypes have never been certified (`unlink`,
`opendir`, `snprintf` are the precedent), so the three new functions
stand on the real prototypes, as their neighbours do.

**No rebuild needed:** examen reads this file at runtime
(`silva/instrumenta/principalia/examen.c:311`). After the edit all
three filum files judge ACCIPE with 0 suspecta.

## 2026-10-13 - clock_gettime + getrusage (analytica T2)

tools/mensor.c `cursus currere` (the run chassis) needs a monotonic
duration and the child's rusage. examen REICE'd it at commit time
(CLOCK_MONOTONIC / RUSAGE_CHILDREN unknown, `struct rusage` incomplete)
while clang was happy - the lexicon gap pattern of 09-14. Added
`clockid_t`, CLOCK_REALTIME/CLOCK_MONOTONIC and `clock_gettime` to the
time.h sectio, and a new `sys/resource.h` sectio (poscit sys/time.h for
struct timeval) with the full Darwin `struct rusage` (144 bytes: two
timevals then 14 longs), RUSAGE_SELF/RUSAGE_CHILDREN, `getrusage`.
auspex_posix asserts the macro values, sizeof(clockid_t), sizeof(struct
rusage) and four field offsets; clang agreed on the first run. Plant
CLOCK_MONOTONIC 5 -> TU nostrum DISSENTIT; restored, certified. Note
for readers: Darwin ru_maxrss is BYTES (Linux KiB). Six names tolerated
in the glossary (clockid, gettime, realtime, monotonic, rusage,
getrusage). The same session also moved mensor's two sleeps from usleep
(code 87) to nanosleep.

## 2026-10-14 - EINPROGRESS + SO_ERROR (http-vocatio T3)

`lib/tcp_posix.c` gained a non-blocking connect (`tcp_incipere`,
`tcp_incipere_perficere`): examen REICE'd it for `EINPROGRESS` and
`SO_ERROR` while clang was happy - the lexicon-gap pattern again.
Added `EINPROGRESS 36` to the errno.h sectio and `SO_ERROR 0x1007` to
the sys/socket.h sectio (Darwin values). `officina/auspex_posix.sh`
asserts both; clang agreed on the first run. Plant `EINPROGRESS 37` ->
TU nostrum DISSENTIT (assertum_174), restored -> CONSENTIT.

## 2026-10-15 - AF_INET6 (http-vocatio staggered connects)

`probationes/probatio_nomina.c` (section `alternare`) names AF_INET6;
examen REICE'd it (identificator ignotus). Added `AF_INET6 30` (Darwin;
Linux is 10) next to AF_INET; auspex asserts it - CONSENTIT, plant 31
-> DISSENTIT, restored.

## 2026-10-15 - F_OK X_OK W_OK R_OK (access modes)

The offline TLS server test uses `access(..., R_OK/X_OK)`; examen
REICE'd them (unknown). Added beside the `access` prototype in the
unistd.h sectio (Darwin = the common POSIX values 0/1/2/4); auspex
asserts all four - CONSENTIT, plant R_OK 5 -> DISSENTIT, restored.

## 2026-10-15 - stdint exact-width types (tls-bearssl T1, R3)

`vendor/bearssl_amalgama.h` (BearSSL's public headers, generated) uses
`uint8_t`..`uint64_t` / `int8_t`..`int64_t` everywhere; examen knew only
`intptr_t`, so a house file including it got ~220 "typus nominatus
ignotus" + "specificatores mixti" findings. Added the eight exact-width
types in the semper sectio beside `intptr_t`; 64-bit spelled `long` /
`unsigned long` (LP64, no `long long` in C89 - same choice as intptr_t).
auspex asserts size AND signedness of all eight (real TU includes
<stdint.h>); CONSENTIT both TUs; hand plant uint64_t -> unsigned int ->
TU nostrum DISSENTIT, restored. (silva.planta could not run this plant:
its preflight compiles the header ALONE and systema_posix.h needs the
systema_c89.h prelude - expected, not a bug.)

## 2026-10-21 - AF_UNIX surface (canalis T1)

`lib/canalis_posix.c` compiled clean under clang but examen REICE'd it:
`struct sockaddr_un`, `AF_UNIX`, `ECONNABORTED`, `getpeereid`, `geteuid`,
`socketpair`, `S_ISSOCK` unknown. Added (Darwin forms): S_IFSOCK 0140000 +
S_ISSOCK in sys/stat.h; ECONNABORTED 53 in errno.h; AF_UNIX 1 and
socketpair in sys/socket.h; a new `sys/un.h` sectio with `sockaddr_un
{sun_len, sun_family, sun_path[104]}`; geteuid + getpeereid in the
unistd.h identity sectio. auspex asserts values, sizeof 106 and offsets
(real TU now includes <sys/un.h>) - CONSENTIT both TUs; hand plant
sun_path[103] -> TU nostrum DISSENTIT (assertum_185), restored ->
CERTIFICATAE. The Linux-only branch (`struct ucred`, SO_PEERCRED) is not
modelled: examen judges the Darwin preprocessing.

## 2026-10-21 - shutdown/SHUT_* and MSG_NOSIGNAL (canalis T2)

`mcp_canalis_pons` half-closes with `shutdown(s, SHUT_WR)`; added
`shutdown` + SHUT_RD/WR/RDWR (0/1/2) to the sys/socket.h sectio. And
`tcp_mittere` now sends with MSG_NOSIGNAL on Darwin as well (SO_NOSIGPIPE
is not honoured on a socket accepted after its peer closed): added
`MSG_NOSIGNAL 0x80000` (Darwin SDK value). auspex asserts both - planted
SHUT_WR 2 -> DISSENTIT, restored -> CERTIFICATAE.

## 2026-10-22 - netdb: AI_NUMERICHOST, AI_NUMERICSERV, EAI_NONAME (nomina_posix.c)

The commit hook's examen refused the new `lib/nomina_posix.c` (unknown
macro = violation): the netdb sectio had the addrinfo shape and the
three functions but no flags or error codes. Added the three names with
Darwin values (0x4, 0x1000, 8 - Linux differs: 0x400 and -2, which is
why the lexicon is a Darwin certificate, not a Linux one; the Docker
compile is Linux's judge). auspex asserts them - planted EAI_NONAME 9 ->
DISSENTIT, restored -> CERTIFICATAE. EAI_NODATA deliberately NOT added:
nomina_posix.c uses it only under `#ifdef`, an untaken arm stays quiet.
Glossary: numerichost, numericserv, eai, noname as ignotum-permissum.
Lesson: judge with `./silva/examen.sh VIA -machina` ONE file at a time
(as the hook does) - a multi-file call accepted the file the hook
refused.

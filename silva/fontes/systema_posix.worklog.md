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

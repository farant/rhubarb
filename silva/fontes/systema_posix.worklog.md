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

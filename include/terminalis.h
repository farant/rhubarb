/* terminalis.h - Terminalis POSIX: modus crudus, lectio cum mora,
 * scriptura, amplitudo (eventus B3b-iii)
 *
 * Stratum platformae SUB rivus_terminalis (qui PURUS manet): fossa 0
 * legitur, 1 scribitur. Modus crudus ut pons tesserae: ISIG solum pro
 * SUSP - Ctrl-Z vere suspendit (terminalis restituitur, post SIGCONT
 * redintratur), Ctrl-C et Ctrl-\ claves ordinariae sunt.
 *
 * Octeti modorum a vocante dantur (rivus_modos_intrare, et
 * rivus_modos_exeundi SINE exitu) et COPIANTUR: tractatores signorum
 * (fatalia, TSTP, CONT) et atexit terminalem restituunt - write +
 * tcsetattr solum (async-signal-tuta).
 *
 * Status STATICUS: terminalis unus per processum.
 */

#ifndef TERMINALIS_H
#define TERMINALIS_H

/* <aedilis nexus="purus"/> - clausura sine regula nexus (nulla
 * framework; eventus A1b: bin/aedilis --nexus-purus) */

#include "latina.h"

#define TERMINALIS_MODI_MAXIMI CCLVI   /* octeti intrandi/exeundi */

nomen structura {
    s32 columnae;
    s32 lineae;
    s32 latitudo;   /* pixela areae (ws_xpixel); 0 = ignota */
    s32 altitudo;
} TerminalisAmplitudo;

/* VERUM si fossae 0 et 1 terminales sunt (isatty). */
b32
terminalis_adest (vacuum);

/* Modus crudus + octeti intrandi scripti; exeundi servantur pro
 * exitu et signis. FALSUM si non terminalis, iam intratum, aut octeti
 * > TERMINALIS_MODI_MAXIMI. */
b32
terminalis_intrare (
    constans i8* intrandi,
            i32  mensura_intrandi,
    constans i8* exeundi,
            i32  mensura_exeundi);

/* Octeti exeundi scripti, modus pristinus restitutus. Sine intrato:
 * VERUM, nihil agitur. */
b32
terminalis_exire (vacuum);

/* Legere usque ad capacitatem. mora_ms < 0: sine fine. Redde > 0
 * octeti lecti; 0 = mora exhausta aut signum (WINCH, CONT); -1 =
 * terminalis abiit aut error. */
s32
terminalis_legere (
     i8* buffer,
    i32  capacitas,
    s32  mora_ms);

/* Omnes octetos scribere (EINTR iteratur). */
b32
terminalis_scribere (
    constans i8* octeti,
            i32  mensura);

/* Columnae, lineae, pixela (0 si terminalis ea non narrat - e.g.
 * Terminal.app). Refugium 80x24 si ioctl deficit. */
b32
terminalis_amplitudo (
    TerminalisAmplitudo* amplitudo);

/* VERUM SEMEL post SIGCONT (vocans EVENTUS_RESUMPTIO fingat). */
b32
terminalis_resumptum (vacuum);

/* VERUM SEMEL post SIGWINCH (vocans amplitudinem rogat). */
b32
terminalis_amplitudo_mutata (vacuum);

#endif /* TERMINALIS_H */

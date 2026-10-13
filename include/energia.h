/* energia.h - status pilae systematis post stratum suggestus: macOS
 * per IOKit (lib/energia_macos.m), ceteri nullam pilam
 * (lib/energia_posix.c) - variantes aedilis (macos posix). */

#ifndef ENERGIA_H
#define ENERGIA_H

#include "latina.h"

nomen structura {
    b32 adest;         /* pila exstat (FALSUM: machina mensae) */
    s32 centesimae;    /* 0-100 */
    b32 oneratur;      /* charging */
} EnergiaStatus;

/* status nunc; FALSUM (adest FALSUM) si nulla pila aut lectio
 * deficit */
b32
energia_status (
    EnergiaStatus* status);

#endif /* ENERGIA_H */

/* fenestra_tempus_posix.c - horologium monotonum POSIX (Linux)
 *
 * Variantes 'posix' fenestra_tempus_macos.c (mach_absolute_time):
 * clock_gettime(CLOCK_MONOTONIC) in nanosecundis, frequentia 1e9;
 * dormire per nanosleep (usleep obsoletum, codex 87). Natum ad portam
 * Linux hospitii v3 (T9): tempus.c, reactor, hospitium hoc egent. */
#include "postulata_posix.h"
#include "fenestra_tempus.h"

#include <time.h>

i64
fenestra_tempus_obtinere_pulsus (
    vacuum)
{
    structura timespec t;

    si (clock_gettime(CLOCK_MONOTONIC, &t) != 0)
    {
        redde 0;
    }
    redde (i64)t.tv_sec * 1000000000ULL + (i64)t.tv_nsec;
}

f64
fenestra_tempus_obtinere_frequentiam (
    vacuum)
{
    redde 1e9;
}

s64
fenestra_tempus_ms (
    vacuum)
{
    redde (s64)(fenestra_tempus_obtinere_pulsus() / 1000000ULL);
}

vacuum
fenestra_dormire (
    i32 microsecundae)
{
    structura timespec t;

    si (microsecundae == 0)
    {
        redde;
    }
    t.tv_sec   = (time_t)(microsecundae / 1000000U);
    t.tv_nsec  = (longus)(microsecundae % 1000000U) * 1000L;
    (vacuum)nanosleep(&t, NIHIL);
}

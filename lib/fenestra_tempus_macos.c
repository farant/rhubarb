/* fenestra_tempus_macos.c - Vide fenestra_tempus.h (horologium mach,
 * dormire POSIX). Ex fenestra_macos.m verbatim motae (eventus A3a). */
#include "postulata_posix.h"
#include "fenestra_tempus.h"
#include <mach/mach_time.h>
#include <unistd.h>

i64
fenestra_tempus_obtinere_pulsus (
    vacuum)
{
    redde (i64)mach_absolute_time();
}

s64
fenestra_tempus_ms (
    vacuum)
{
    f64 pulsus;
    f64 frequentia;   /* pulsus per secundum (f64) */

    pulsus      = (f64)fenestra_tempus_obtinere_pulsus();
    frequentia  = fenestra_tempus_obtinere_frequentiam();
    si (frequentia <= 0.0)
    { redde ZEPHYRUM;
    }
    redde (s64)((pulsus * 1000.0) / frequentia);
}

f64
fenestra_tempus_obtinere_frequentiam (
    vacuum)
{
    mach_timebase_info_data_t informatio;

    mach_timebase_info(&informatio);
    redde 1e9 * (f64)informatio.denom / (f64)informatio.numer;
}

vacuum
fenestra_dormire (
    i32 microsecundae)
{
    si (microsecundae > ZEPHYRUM)
    {
        usleep((unsigned int)microsecundae);
    }
}

/* hora_hybrida.c - hora hybrida (vide include/hora_hybrida.h) */

#include "postulata_posix.h"

#include "hora_hybrida.h"

#include <sys/time.h>

/* numerator maximus: XVI bita inferiora omnia posita */
#define NUMERATOR_MAXIMUS ((s64)LXV * (s64)M + (s64)DXXXV)

s64
hora_hybrida_componere (
    Momentum momentum,
         i32 numerator)
{
    redde (momentum << HORA_HYBRIDA_NUMERATOR_BITA)
        | ((s64)numerator & NUMERATOR_MAXIMUS);
}

Momentum
hora_hybrida_momentum (
    s64 signum)
{
    redde signum >> HORA_HYBRIDA_NUMERATOR_BITA;
}

i32
hora_hybrida_numerator (
    s64 signum)
{
    redde (i32)(signum & NUMERATOR_MAXIMUS);
}

/* numerator plenus in millisecundam proximam transit (ultimum + I
 * portat) - signum tamen numquam minuitur */
s64
hora_hybrida_proxima (
         s64 ultimum,
    Momentum physicum)
{
    s64 physica;

    physica = hora_hybrida_componere(physicum, ZEPHYRUM);
    redde physica > ultimum ? physica : ultimum + I;
}

Momentum
hora_hybrida_physica (
    vacuum)
{
    structura timeval tv;

    si (gettimeofday(&tv, NIHIL) != ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    redde (Momentum)tv.tv_sec * M + (Momentum)(tv.tv_usec / M);
}

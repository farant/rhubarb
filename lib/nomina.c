/* nomina.c - pars portabilis resolutionis nominum (nomina.h):
 * ordo familiarum RFC 8305 par. 4. Resolutores per platformam in
 * nomina_macos.m et nomina_posix.c (per conventionem aedilis). */
#include "nomina.h"

vacuum
nomina_familias_alternare (
    NominaAddressus* addressus,
                i32  numerus)
{
    NominaAddressus prima[NOMINA_ADDRESSUS_MAXIMI];
    NominaAddressus altera[NOMINA_ADDRESSUS_MAXIMI];
                i32 np = ZEPHYRUM;
                i32 na = ZEPHYRUM;
                i32 ip = ZEPHYRUM;
                i32 ia = ZEPHYRUM;
                i32 k;

    si (!addressus || numerus <= I || numerus > NOMINA_ADDRESSUS_MAXIMI)
    {
        redde;
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        si (addressus[k].familia == addressus[ZEPHYRUM].familia)
        {
            prima[np++] = addressus[k];
        }
        alioquin
        {
            altera[na++] = addressus[k];
        }
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        si ((k % II == ZEPHYRUM && ip < np) || ia >= na)
        {
            addressus[k] = prima[ip++];
        }
        alioquin
        {
            addressus[k] = altera[ia++];
        }
    }
}

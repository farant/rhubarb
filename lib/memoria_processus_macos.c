/* memoria_processus_macos.c - vestigium per proc_pid_rusage
 * (RUSAGE_INFO_V4: ri_phys_footprint, ri_lifetime_max_phys_footprint,
 * ri_resident_size). Vide memoria_processus.h. */

#include "postulata_posix.h"
#include "memoria_processus.h"
#include <string.h>
#include <unistd.h>
#include <libproc.h>
#include <sys/resource.h>

b32
memoria_processus_legere (
                  s32  pid,
     MemoriaProcessus* exitus)
{
    structura rusage_info_v4 usus;
                       s32 quis;

    si (exitus == NIHIL)
    {
        redde FALSUM;
    }
    memset(exitus, ZEPHYRUM, magnitudo(MemoriaProcessus));
    si (pid < ZEPHYRUM)
    {
        redde FALSUM;
    }
    quis = pid == ZEPHYRUM ? (s32)getpid() : pid;
    si (proc_pid_rusage(quis, RUSAGE_INFO_V4, (rusage_info_t*)&usus)
        != ZEPHYRUM)
    {
        redde FALSUM;
    }
    exitus->vestigium       = (i64)usus.ri_phys_footprint;
    exitus->vestigium_apex  = (i64)usus.ri_lifetime_max_phys_footprint;
    exitus->residens        = (i64)usus.ri_resident_size;
    redde VERUM;
}

/* memoria_processus_posix.c - vestigium per /proc/<pid>/status (Linux)
 *
 * Variantes 'posix' memoria_processus_macos.c (proc_pid_rusage). Linux
 * memoriam compressam non habet (zram/zswap exceptis): vestigium =
 * VmRSS + VmSwap (proximum ri_phys_footprint: paginae tentae, sive
 * residentes sive eiectae), apex = VmHWM (apex RSS - permutatio non
 * includitur: apex vestigii verus deest), residens = VmRSS. Valores in
 * kB (MXXIV octeti). Systema sine /proc (BSD): FALSUM, ut caput
 * permittit - credo memoriam tum ZEPHYRUM notat.
 *
 * LECTIO per filum_octeti_* (rivus usque ad finem): /proc mensuram
 * ZEPHYRUM per stat() nuntiat, ergo filum_legere_totum chordam vacuam
 * SINE errore redderet. Sub FABRICA_LECTIONES lectio libro notatur
 * (fabrica in vasis Linux non currit; ianua T3, 2026-10-20). */

#include "postulata_posix.h"
#include "memoria_processus.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"

#include <stdio.h>
#include <string.h>

#define STATUS_MAXIMUS (XVI * MXXIV)

/* valor kB lineae "<clavis>:\t   123 kB" in octetis; -I si deest */
interior s64
_valor (
    constans character* textus,
    constans character* clavis)
{
     constans character* a = strstr(textus, clavis);
                    s64  n = ZEPHYRUM;

    /* clavis initio lineae solum */
    dum (a && a != textus && a[-I] != '\n')
    {
        a = strstr(a + I, clavis);
    }
    si (!a)
    {
        redde -I;
    }
    a += strlen(clavis);
    dum (*a == ' ' || *a == '\t')
    {
        a++;
    }
    si (*a < '0' || *a > '9')
    {
        redde -I;
    }
    dum (*a >= '0' && *a <= '9')
    {
        n = n * X + (s64)(*a - '0');
        a++;
    }
    redde n * (s64)MXXIV;
}

b32
memoria_processus_legere (
                  s32  pid,
     MemoriaProcessus* exitus)
{
                 Piscina* p;
    FilumOctetorumLector* l;
               character  via[LXIV];
               character* textus;
                     i32  n = ZEPHYRUM;
                     s32  k;
                     s64  residentes;
                     s64  apex;
                     s64  permutata;

    si (exitus == NIHIL)
    {
        redde FALSUM;
    }
    memset(exitus, ZEPHYRUM, magnitudo(MemoriaProcessus));
    si (pid < ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (pid == ZEPHYRUM)
    {
        strcpy(via, "/proc/self/status");
    }
    alioquin
    {
        sprintf(via, "/proc/%ld/status", (longus)pid);
    }
    p = piscina_generare_dynamicum("memoria_processus", STATUS_MAXIMUS
        + CCLVI);
    si (!p)
    {
        redde FALSUM;
    }
    l       = filum_octeti_aperire(via, p);
    textus  = (character*)piscina_allocare(p, STATUS_MAXIMUS + I);
    si (!l || !textus)
    {
        filum_octeti_claudere(l);
        piscina_destruere(p);
        redde FALSUM;
    }
    dum (   n < STATUS_MAXIMUS && (k = filum_octeti_legere(l,
        (i8*)textus + n, (i32)(STATUS_MAXIMUS - n))) > ZEPHYRUM)
    {
        n += (i32)k;
    }
    filum_octeti_claudere(l);
    textus[n]   = '\0';
    residentes  = _valor(textus, "VmRSS:");
    apex        = _valor(textus, "VmHWM:");
    permutata   = _valor(textus, "VmSwap:");
    piscina_destruere(p);
    /* filius exitus (zombie): status sine Vm* lineis */
    si (residentes < ZEPHYRUM || apex < ZEPHYRUM)
    {
        redde FALSUM;
    }
    exitus->residens        = (i64)residentes;
    exitus->vestigium       = (i64)(residentes + (permutata
        > ZEPHYRUM ? permutata
        : ZEPHYRUM));
    exitus->vestigium_apex  = (i64)(apex
        > residentes ? apex : residentes);
    si (exitus->vestigium_apex < exitus->vestigium)
    {
        exitus->vestigium_apex = exitus->vestigium;
    }
    redde VERUM;
}

/* probatio_circulus.c - Probationes anuli SPSC (include/circulus.h) et
 * ordinis memoriae (include/atomus.h)
 *
 * Sectio ultima 'duo processus': scriptor et lector in PROCESSIBUS
 * duobus (fork) super memoriam communem (mmap MAP_SHARED | MAP_ANON) -
 * concurrentia vera in nucleis diversis SINE filis (domus filis caret,
 * processus.h). Scriptor quaterniones {k, k, k, k} mittit; lector
 * ordinem (nullus omissus, nullus iteratus) et integritatem (nullus
 * semiscriptus) iudicat. Gyri vacui terminantur (CIRCULUS_GYRI): anulus
 * fractus probationem FRANGIT, numquam suspendit. Vide
 * lib/circulus.worklog.md.
 */
#include "postulata_posix.h"
#include "latina.h"
#include "credo.h"
#include "atomus.h"
#include "circulus.h"
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

#define CIRCULUS_ELEMENTA (CC * M)            /* CC milia quaterniones */
#define CIRCULUS_GYRI     (0x10000000U)       /* gyri vacui maximi */

nomen structura {
    i32 a;
    i32 b;
    i32 c;
    i32 d;
} Quaternio;


    /* ==================================================
     * ATOMUS: valor publicatus legitur
     * ================================================== */

interior vacuum
_sectio_atomus (
    CredoContextus* c)
{
    volatilis i32 locus = ZEPHYRUM;

    (vacuum)c;
    atomus_publicare(&locus, VII);
    CREDO_AEQUALIS_I32 (atomus_legere(&locus), VII);
    atomus_publicare(&locus, 0xFFFFFFFFU);
    CREDO_AEQUALIS_I32 (atomus_legere(&locus), 0xFFFFFFFFU);
}


    /* ==================================================
     * PARARE: refutationes
     * ================================================== */

interior vacuum
_sectio_parare (
    CredoContextus* c)
{
    Circulus anellus;
          i8 memoria[LXIV];

    (vacuum)c;
    CREDO_VERUM (!circulus_parare(&anellus, memoria, ZEPHYRUM, IV));
    CREDO_VERUM (!circulus_parare(&anellus, memoria, III, IV));
    CREDO_VERUM (!circulus_parare(&anellus, memoria, XII, IV));
    CREDO_VERUM (!circulus_parare(&anellus, memoria, VIII, ZEPHYRUM));
    CREDO_VERUM (!circulus_parare(&anellus, NIHIL, VIII, IV));
    CREDO_VERUM (circulus_parare(&anellus, memoria, XVI, IV));
    CREDO_VERUM (circulus_parare(&anellus, memoria, I, IV));
    CREDO_AEQUALIS_I32 (circulus_numerus(&anellus), ZEPHYRUM);
}


    /* ==================================================
     * ORDO: FIFO, plenus, vacuus
     * ================================================== */

interior vacuum
_sectio_ordo (
    CredoContextus* c)
{
    Circulus anellus;
         i32 memoria[VIII];
         i32 k;
         i32 x;
         b32 ordo = VERUM;

    (vacuum)c;
    CREDO_VERUM (circulus_parare(&anellus, memoria, VIII,
        magnitudo(i32)));
    CREDO_VERUM (!circulus_legere(&anellus, &x));
    per (k = ZEPHYRUM; k < VIII; k++)
    {
        x = k + C;
        CREDO_VERUM (circulus_scribere(&anellus, &x));
    }
    CREDO_AEQUALIS_I32 (circulus_numerus(&anellus), VIII);
    x = DCCCLXXXVIII;
    CREDO_VERUM (!circulus_scribere(&anellus, &x));
    CREDO_AEQUALIS_I32 (circulus_numerus(&anellus), VIII);
    per (k = ZEPHYRUM; k < VIII; k++)
    {
        si (!circulus_legere(&anellus, &x) || x != k + C)
        {
            ordo = FALSUM;
        }
    }
    CREDO_VERUM (ordo);
    CREDO_VERUM (!circulus_legere(&anellus, &x));
    CREDO_AEQUALIS_I32 (circulus_numerus(&anellus), ZEPHYRUM);
}


    /* ==================================================
     * CIRCUMVOLUTIO: indices trans 2^32
     * ================================================== */

interior vacuum
_sectio_revolutio (
    CredoContextus* c)
{
    Circulus anellus;
         i32 memoria[VIII];
         i32 k;
         i32 x;
         b32 bene = VERUM;

    (vacuum)c;
    CREDO_VERUM (circulus_parare(&anellus, memoria, VIII,
        magnitudo(i32)));
    /* indices prope finem spatii XXXII bitorum */
    anellus.caput = 0xFFFFFFF0U;
    anellus.cauda = 0xFFFFFFF0U;
    per (k = ZEPHYRUM; k < LXIV; k++)
    {
        x = k;
        si (   !circulus_scribere(&anellus, &x)
            || circulus_numerus(&anellus) != I)
        {
            bene = FALSUM;
        }
        x = 0xDEADU;
        si (!circulus_legere(&anellus, &x) || x != k)
        {
            bene = FALSUM;
        }
    }
    CREDO_VERUM (bene);
    /* plenus trans circumvolutionem */
    anellus.caput = 0xFFFFFFFCU;
    anellus.cauda = 0xFFFFFFFCU;
    per (k = ZEPHYRUM; k < VIII; k++)
    {
        x = k;
        CREDO_VERUM (circulus_scribere(&anellus, &x));
    }
    CREDO_AEQUALIS_I32 (circulus_numerus(&anellus), VIII);
    CREDO_VERUM (!circulus_scribere(&anellus, &x));
    CREDO_VERUM (circulus_legere(&anellus, &x) && x == ZEPHYRUM);
}


    /* ==================================================
     * MENSURA: elementa III octetorum, vicini intacti
     * ================================================== */

interior vacuum
_sectio_mensura (
    CredoContextus* c)
{
    Circulus anellus;
          i8 memoria[IV * III + IV];
          i8 elementum[III];
          i8 lectum[III];
         i32 k;

    (vacuum)c;
    per (k = ZEPHYRUM; k < IV * III + IV; k++)
    {
        memoria[k] = 0xEEU;
    }
    CREDO_VERUM (circulus_parare(&anellus, memoria, IV, III));
    per (k = ZEPHYRUM; k < V; k++)
    {
        elementum[ZEPHYRUM]  = (i8)(k * III);
        elementum[I]         = (i8)(k * III + I);
        elementum[II]        = (i8)(k * III + II);
        si (k == IV)
        {
            CREDO_VERUM (!circulus_scribere(&anellus, elementum));
        }
        alioquin
        {
            CREDO_VERUM (circulus_scribere(&anellus, elementum));
        }
    }
    /* custos post IV x III octetos intactus */
    CREDO_AEQUALIS_I32 ((i32)memoria[XII], 0xEEU);
    CREDO_AEQUALIS_I32 ((i32)memoria[XV], 0xEEU);
    CREDO_VERUM (circulus_legere(&anellus, lectum));
    CREDO_VERUM (lectum[ZEPHYRUM] == 0 && lectum[I] == 1
        && lectum[II] == 2);
    CREDO_VERUM (circulus_legere(&anellus, lectum));
    CREDO_VERUM (lectum[ZEPHYRUM] == 3 && lectum[I] == 4
        && lectum[II] == 5);
}


    /* ==================================================
     * DUO PROCESSUS: scriptor in filio, lector in parente
     * ================================================== */

interior vacuum
_scriptor (
    Circulus* anellus)
{
    i32 k;

    per (k = ZEPHYRUM; k < CIRCULUS_ELEMENTA; k++)
    {
        Quaternio q;
              i32 gyri = ZEPHYRUM;

        q.a = k;
        q.b = k;
        q.c = k;
        q.d = k;
        dum (!circulus_scribere(anellus, &q))
        {
            gyri++;
            si (gyri > CIRCULUS_GYRI)
            {
                _exit(II);
            }
        }
    }
    _exit(ZEPHYRUM);
}

interior vacuum
_sectio_duo_processus (
    CredoContextus* c)
{
    memoriae_index mensura = magnitudo(Circulus)
        + LXIV * magnitudo(Quaternio);
             vacuum* regio;
           Circulus* anellus;
              pid_t  filius;
                i32  exspectatum  = ZEPHYRUM;
                i32  gyri         = ZEPHYRUM;
                b32  integra      = VERUM;
                int  status       = ZEPHYRUM;

    (vacuum)c;
    regio = mmap(NIHIL, mensura, PROT_READ | PROT_WRITE,
                 MAP_SHARED | MAP_ANON, -I, ZEPHYRUM);
    CREDO_VERUM (regio != MAP_FAILED);
    si (regio == MAP_FAILED)
    {
        redde;
    }
    anellus = (Circulus*)regio;
    CREDO_VERUM (circulus_parare(anellus, (i8*)regio
        + magnitudo(Circulus),
                                 LXIV, magnitudo(Quaternio)));
    filius = fork();
    CREDO_VERUM (filius >= ZEPHYRUM);
    si (filius < ZEPHYRUM)
    {
        (vacuum)munmap(regio, mensura);
        redde;
    }
    si (filius == ZEPHYRUM)
    {
        _scriptor(anellus);
    }
    dum (exspectatum < CIRCULUS_ELEMENTA)
    {
        Quaternio q;

        si (circulus_legere(anellus, &q))
        {
            si (   q.a != exspectatum || q.b != exspectatum
                || q.c != exspectatum || q.d != exspectatum)
            {
                integra = FALSUM;
            }
            exspectatum++;
            gyri = ZEPHYRUM;
        }
        alioquin
        {
            gyri++;
            si (gyri > CIRCULUS_GYRI)
            {
                frange;
            }
        }
    }
    (vacuum)waitpid(filius, &status, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (exspectatum, CIRCULUS_ELEMENTA);
    CREDO_VERUM (integra);
    CREDO_VERUM (WIFEXITED(status) && WEXITSTATUS(status) == ZEPHYRUM);
    (vacuum)munmap(regio, mensura);
}


hic_manens constans CredoSectio SECTIONES[] = {
    { "atomus", _sectio_atomus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "parare", _sectio_parare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ordo", _sectio_ordo, NIHIL, NIHIL, NIHIL, NIHIL },
    { "revolutio", _sectio_revolutio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "mensura", _sectio_mensura, NIHIL, NIHIL, NIHIL, NIHIL },
    { "duo processus", _sectio_duo_processus, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("circulus", SECTIONES);
}

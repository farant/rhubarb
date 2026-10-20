/* sonda r2: terminale_applicatio_aedificare FALSUM -> pt clausus?
 * (caput: "FALSUM si pars deficit (pt tunc clausus)"). Piscina certae
 * magnitudinis per magnitudines; infans in processu filio (piscina_allocare
 * deficiens forsitan abortit). */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "pseudoterminale.h"
#include "terminale.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

hic_manens s32 clausurae = ZEPHYRUM;
hic_manens vacuum (*claudere_verum)(vacuum*) = NIHIL;

interior vacuum
claudere_numerans (vacuum* datum)
{
    clausurae++;
    claudere_verum(datum);
}

/* exitus: 0 VERUM, 1 FALSUM et clausus, 2 FALSUM NON clausus */
interior s32
probare (memoriae_index mensura)
{
                Piscina* pin;
                Piscina* p;
    InternamentumChorda* in;
        Pseudoterminale* pt;
    TerminaleApplicatio  app;

    pin = piscina_generare_dynamicum("sonda", 65536);
    in  = internamentum_creare(pin);
    pt  = pseudoterminale_memoriae_creare(pin, (constans i8*)"hi", II,
        ZEPHYRUM);
    claudere_verum = pt->claudere;
    pt->claudere   = claudere_numerans;
    p = piscina_generare_certae_magnitudinis("sonda_certa", mensura);
    si (terminale_applicatio_aedificare(&app, p, in, pt, 480, 192))
    {
        redde ZEPHYRUM;
    }
    si (clausurae == ZEPHYRUM || !getenv("TACITE")) imprimere("  mensura %lu: FALSUM, clausurae pt = %d, hospes %s\n",
        (unsigned long)mensura, (int)clausurae,
        app.hospes ? "CREATUS" : "nullus");
    redde clausurae >= I ? I : II;
}

s32
principale (s32 argc, character** argv)
{
    memoriae_index m;
              s32 status;
              s32 res;
              pid_t fil;
              s32 vidi_non_clausum;

    vidi_non_clausum = ZEPHYRUM;
    memoriae_index finis;
    memoriae_index gradus;
    m = argc > III ? (memoriae_index)atol(argv[I]) : 256u;
    finis = argc > III ? (memoriae_index)atol(argv[II]) : 4u * 1024u * 1024u;
    gradus = argc > III ? (memoriae_index)atol(argv[III]) : ZEPHYRUM;
    dum (m <= finis)
    {
        fflush(stdout);
        fil = fork();
        si (fil == ZEPHYRUM)
        {
            exit((int)probare(m));
        }
        (vacuum)waitpid(fil, &status, ZEPHYRUM);
        si (WIFEXITED(status))
        {
            res = WEXITSTATUS(status);
            si (res == II) { vidi_non_clausum++; }
            si (res == ZEPHYRUM)
            {
                imprimere("mensura %lu: VERUM (finis scrutationis)\n",
                    (unsigned long)m);
                frange;
            }
        }
        alioquin
        {
            imprimere("  mensura %lu: signum %d\n", (unsigned long)m,
                (int)WTERMSIG(status));
        }
        m = gradus ? m + gradus : m + m / 8u;
    }
    imprimere("casus FALSUM sine clausura pt: %d\n", (int)vidi_non_clausum);
    redde ZEPHYRUM;
}

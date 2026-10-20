/* sonda r1: scrutinium terminalis post terminale_claudere et
 * piscinam hospitis destructam manet? (mstats: octeti in usu) */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "pseudoterminale.h"
#include "terminale.h"
#include <stdio.h>
#include <malloc/malloc.h>

hic_manens b32 purgare = FALSUM;

/* layout privatum TerminaleContextus: app, scrutinium (lib/terminale.c:64) */
nomen structura { TerminaleApplicatio* app; Piscina* scrutinium; } CaputContextus;

interior b32
cyclus (vacuum)
{
                Piscina* p;
    InternamentumChorda* in;
        Pseudoterminale* pt;
    TerminaleApplicatio  app;
                    b32  ok;

    p  = piscina_generare_dynamicum("sonda", 65536);
    in = internamentum_creare(p);
    pt = pseudoterminale_memoriae_creare(p, (constans i8*)"hi", II, ZEPHYRUM);
    ok = terminale_applicatio_aedificare(&app, p, in, pt, 480, 192);
    si (ok)
    {
        terminale_claudere(&app);
        si (purgare)
        {
            piscina_destruere(((CaputContextus*)app.contextus)->scrutinium);
        }
    }
    piscina_destruere(p);
    redde ok;
}

s32
principale (vacuum)
{
    struct mstats a;
    struct mstats b;
    struct mstats c;
              s32 i;
              s32 bona;

    bona = ZEPHYRUM;
    (vacuum)cyclus();
    (vacuum)cyclus();
    a = mstats();

    i = ZEPHYRUM;
    dum (i < 50) { bona += cyclus() ? I : ZEPHYRUM; i++; }
    b = mstats();
    i = ZEPHYRUM;
    dum (i < 50) { bona += cyclus() ? I : ZEPHYRUM; i++; }
    c = mstats();
    imprimere("cycli boni: %d/100\n", (int)bona);
    imprimere("bytes_used ante: %lu, post 50: %lu, post 100: %lu\n",
        (unsigned long)a.bytes_used, (unsigned long)b.bytes_used,
        (unsigned long)c.bytes_used);
    imprimere("incrementum per cyclum: %ld\n",
        (long)((long)c.bytes_used - (long)a.bytes_used) / 100);
    si (!purgare)
    {
        purgare = VERUM;
        imprimere("--- iterum, scrutinio manu destructo ---\n");
        a = mstats();
        i = ZEPHYRUM;
        dum (i < 100) { (vacuum)cyclus(); i++; }
        c = mstats();
        imprimere("incrementum per cyclum (scrutinio destructo): %ld\n",
            (long)((long)c.bytes_used - (long)a.bytes_used) / 100);
    }
    redde ZEPHYRUM;
}

/* sonda: _obiecti_nomen non est iniectivum - eadem basis x.c in
 * directoriis "lib/a__b" et "lib/a/b" idem obiectum lib__a__b__x.o */
#include "latina.h"
#include "aedilis.h"
#include "piscina.h"
#include "xar.h"
#include "chorda.h"
#include <stdio.h>
#include <string.h>

interior chorda
_ch (constans character* s)
{
    chorda c;
    c.datum   = (i8*)(constans vacuum*)s;
    c.mensura = (i32)strlen(s);
    redde c;
}

integer
principale (vacuum)
{
    Piscina*             piscina;
    AedilisFructus       fructus;
    AedilisConfiguratio  configuratio;
    AedilisObiectum*     ob;
    chorda               scriptum;
    i32                  i;
    i32                  initium;

    piscina = piscina_generare_dynamicum("sonda", 65536);
    memset(&fructus, 0, sizeof(fructus));
    memset(&configuratio, 0, sizeof(configuratio));
    configuratio.radix     = _ch(".");
    configuratio.inclusa   = xar_creare(piscina, sizeof(chorda));
    configuratio.vexilla   = xar_creare(piscina, sizeof(chorda));
    configuratio.variantes = xar_creare(piscina, sizeof(chorda));
    fructus.scopus      = _ch("probationes/sonda.c");
    fructus.varians     = _ch("macos");
    fructus.obiecta     = xar_creare(piscina, sizeof(AedilisObiectum));
    fructus.capita      = xar_creare(piscina, sizeof(AedilisCaput));
    fructus.systemata   = xar_creare(piscina, sizeof(chorda));
    fructus.inresolutae = xar_creare(piscina, sizeof(chorda));
    fructus.vendores    = xar_creare(piscina, sizeof(AedilisVendor));
    fructus.vexilla_annotata = xar_creare(piscina, sizeof(chorda));
    fructus.pura        = xar_creare(piscina, sizeof(chorda));
    fructus.facultates  = xar_creare(piscina, sizeof(chorda));
    fructus.instrumenta = xar_creare(piscina, sizeof(chorda));

    ob = (AedilisObiectum*)xar_addere(fructus.obiecta);
    memset(ob, 0, sizeof(*ob));
    ob->via = _ch("lib/a__b/x.c");
    ob = (AedilisObiectum*)xar_addere(fructus.obiecta);
    memset(ob, 0, sizeof(*ob));
    ob->via = _ch("lib/a/b/x.c");

    scriptum = aedilis_scriptum_scribere(&fructus, &configuratio,
        piscina, FALSUM, NIHIL);
    initium = 0;
    per (i = 0; i < scriptum.mensura; i++)
    {
        si (scriptum.datum[i] == (i8)'\n')
        {
            si (i - initium > 9
                && memcmp(scriptum.datum + initium, "compilare", 9) == 0)
            {
                imprimere("%.*s\n", (integer)(i - initium),
                    (constans character*)(scriptum.datum + initium));
            }
            initium = i + 1;
        }
    }
    redde 0;
}

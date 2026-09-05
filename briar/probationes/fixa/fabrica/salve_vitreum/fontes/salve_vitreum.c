/* salve_vitreum.c - a briar genitum ex briar/probationes/fixa/thistle/salve_vitreum.thistle: fenestra vitrea, methodi regionum praebitae.
 * Gyrus atrii hic (vide include/atrium.h); status usoris NIHIL (plan 2). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "json.h"
#include "atrium.h"
#include "internuntius.h"
#include "capsula_salve_vitreum.h"
#include "salve_vitreum_regiones.h"
#include <stdio.h>
#include <string.h>

s32
principale (integer argc, character** argv)
{
    Piscina*            piscina;
    AtriumConfiguratio  figura;
    Atrium*             atrium;
    chorda              causa;

    piscina = piscina_generare_dynamicum("salve_vitreum", 16777216);
    si (piscina == NIHIL)
    {
        redde I;
    }
    memset(&figura, 0, magnitudo(figura));
    figura.titulus  = "salve";
    figura.latitudo = 640;
    figura.altitudo = 400;
    figura.capsula  = &capsula_salve_vitreum;
    atrium_vexilla_legere(&figura, argc, argv);

    atrium = atrium_creare(piscina, &figura, &causa);
    si (atrium == NIHIL)
    {
        imprimere("FRACTA: %.*s\n", (integer)causa.mensura,
            (constans character*)causa.datum);
        redde I;
    }
    (vacuum)internuntius_praebere(atrium_internuntius(atrium),
        "salve", salve, NIHIL);

    imprimere("[salve_vitreum] fenestra aperta\n");
    si (atrium_portus(atrium) != ZEPHYRUM)
    {
        imprimere("[salve_vitreum] imperium: http://127.0.0.1:%d/imperium\n",
            (integer)atrium_portus(atrium));
    }
    fflush(stdout);
    atrium_monstrare(atrium);
    dum (atrium_currendum(atrium))
    {
        (vacuum)atrium_gressus(atrium);
    }
    atrium_destruere(atrium);
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}

/* probatio_officina_lanceatores.c - lanceatores officinae VERE
 * currere (colloquium.sh, interpretare.sh): aedificatio + cursus
 * per processum, effusio capta. Nulla porta olim lanceatorem
 * colloquii currebat - ideo nexus fractus (nexus_ordines, parcum
 * 01M4XR3Q) a 8bbf97f7 usque ad 2026-11-02 latuit.
 *
 * Probat: scriptum #! (fixum salutatio.scriptum) per colloquium.sh
 * - exitus 0, ambae lineae effusionis. */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "processus.h"
#include "credo.h"

#include <stdlib.h>
#include <string.h>

/* aedificatio frigida lanceatoris (omnia obiecta) tempus eget */
#define MORA_AEDIFICATIONIS 600000

interior constans character*
_radix (vacuum)
{
    constans character* radix = getenv("RHUBARB_RADIX");

    redde radix != NIHIL ? radix : ".";
}

interior vacuum
_sectio_colloquium (
    CredoContextus* c)
{
               Piscina* piscina = c->piscina;
     ProcessusOptiones  optiones;
     ProcessusResultus  r;
    constans character* argumenta[] = {
        "officina/colloquium.sh",
        "officina/probationes/fixa/sessio_scripta/salutatio.scriptum",
        "-posix",   /* bibliothecae ex #include scripti (T3) */
        NIHIL
    };

    memset(&optiones, ZEPHYRUM, magnitudo(ProcessusOptiones));
    optiones.directorium = _radix();
    r = processus_exsequi_cum(argumenta, &optiones, MORA_AEDIFICATIONIS,
        piscina);
    CREDO_VERUM(r.successus);
    CREDO_AEQUALIS_I32(r.codex_exitus, ZEPHYRUM);
    CREDO_CHORDA_CONTINET(r.effusio,
        chorda_ex_literis("ave ex scripto (14 octeti)", piscina));
    CREDO_CHORDA_CONTINET(r.effusio,
        chorda_ex_literis("relectum: scriptum scripsit", piscina));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "colloquium scriptum", _sectio_colloquium, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("officina_lanceatores", SECTIONES);
}

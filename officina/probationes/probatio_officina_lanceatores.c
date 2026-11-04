/* probatio_officina_lanceatores.c - lanceatores officinae VERE
 * currere (colloquium.sh, interpretare.sh): aedificatio + cursus
 * per processum, effusio capta. Nulla porta olim lanceatorem
 * colloquii currebat - ideo nexus fractus (nexus_ordines, parcum
 * 01M4XR3Q) a 8bbf97f7 usque ad 2026-11-02 latuit.
 *
 * Probat: scriptum #! (fixum salutatio.scriptum) per colloquium.sh
 * - exitus 0, ambae lineae effusionis; interpretare.sh sine
 * -plagulae - mundus ex indice + aedile (officina-bibliothecae T4). */

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

/* interpretare sine -plagulae: domus functionis ex indice
 * (build/nexus.tsv), clausura per aedilem - piscina.h solum
 * piscina.c onerat (olim: lib/ totus, ~30 s) */
interior vacuum
_sectio_interpretare (
    CredoContextus* c)
{
               Piscina* piscina = c->piscina;
     ProcessusOptiones  optiones;
     ProcessusResultus  r;
    constans character* argumenta[] = {
        "officina/interpretare.sh",
        "piscina_generare_dynamicum",
        "\"x\"",
        "1024",
        NIHIL
    };

    memset(&optiones, ZEPHYRUM, magnitudo(ProcessusOptiones));
    optiones.directorium = _radix();
    r = processus_exsequi_cum(argumenta, &optiones, MORA_AEDIFICATIONIS,
        piscina);
    CREDO_VERUM(r.successus);
    CREDO_AEQUALIS_I32(r.codex_exitus, ZEPHYRUM);
    CREDO_CHORDA_CONTINET(r.erratum,
        chorda_ex_literis("mundus: 1 moduli", piscina));
    CREDO_CHORDA_CONTINET(r.erratum,
        chorda_ex_literis("[BENE", piscina));
}

/* fasti_est_bissextilis: caput fasti.h -> fasti.c + clausura; sine
 * monito mundi totius */
interior vacuum
_sectio_fasti (
    CredoContextus* c)
{
               Piscina* piscina = c->piscina;
     ProcessusOptiones  optiones;
     ProcessusResultus  r;
    constans character* argumenta[] = {
        "officina/interpretare.sh",
        "fasti_est_bissextilis",
        "2024",
        NIHIL
    };

    memset(&optiones, ZEPHYRUM, magnitudo(ProcessusOptiones));
    optiones.directorium = _radix();
    r = processus_exsequi_cum(argumenta, &optiones, MORA_AEDIFICATIONIS,
        piscina);
    CREDO_VERUM(r.successus);
    CREDO_AEQUALIS_I32(r.codex_exitus, ZEPHYRUM);
    CREDO_VERUM(chorda_aequalis_literis(r.effusio, "1\n"));
    CREDO_FALSUM(chorda_continet(r.erratum,
        chorda_ex_literis("mundus totus", piscina)));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "colloquium scriptum", _sectio_colloquium, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "interpretare sine plagulis", _sectio_interpretare, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "interpretare fasti", _sectio_fasti, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("officina_lanceatores", SECTIONES);
}

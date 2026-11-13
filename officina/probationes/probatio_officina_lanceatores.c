/* probatio_officina_lanceatores.c - lanceatores officinae VERE
 * currere (colloquium.sh, interpretare.sh, cursor.sh, vindex.sh):
 * aedificatio + cursus
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

/* M5 T0: cursor mundum per bibliothecae_mundum_totum nectit (una
 * implementatio per caput) - olim testa.c + testa_android.c ambo
 * in mundo, omnis suites NEXUS FRACTUS (testa_principale; parcum
 * 01M6FZMA) */
interior vacuum
_sectio_cursor_sola (
    CredoContextus* c)
{
               Piscina* piscina = c->piscina;
     ProcessusOptiones  optiones;
     ProcessusResultus  r;
    constans character* argumenta[] = {
        "officina/cursor.sh",
        "-sola",
        "probatio_piscina",
        NIHIL
    };

    memset(&optiones, ZEPHYRUM, magnitudo(ProcessusOptiones));
    optiones.directorium = _radix();
    r = processus_exsequi_cum(argumenta, &optiones, MORA_AEDIFICATIONIS,
        piscina);
    /* nexus + exsecutio solum: suites credo in machinula nondum
     * currunt (getpid, setjmp, fork... - parcum "credo non
     * interpretabilis"), ergo exitus cursoris I et PRAETERIIT
     * nondum exspectantur */
    CREDO_VERUM(r.successus);
    CREDO_FALSUM(chorda_continet(r.erratum,
        chorda_ex_literis("NEXUS FRACTUS", piscina)));
    CREDO_CHORDA_CONTINET(r.erratum,
        chorda_ex_literis("0 nexus fracti / 1 suites", piscina));
    CREDO_CHORDA_CONTINET(r.erratum,
        chorda_ex_literis("instructiones exsecutae", piscina));
}

/* M5 T0: enumeratio sine tecto - olim CCLVI suites tacite
 * (parcum 01M6FZMG); probatio_xar alphabetice post sectionem */
interior vacuum
_sectio_cursor_tectum (
    CredoContextus* c)
{
               Piscina* piscina = c->piscina;
     ProcessusOptiones  optiones;
     ProcessusResultus  r;
                   i32  lineae = ZEPHYRUM;
                   i32  k;
    constans character* argumenta[] = {
        "officina/cursor.sh",
        "-enumerare",
        NIHIL
    };

    memset(&optiones, ZEPHYRUM, magnitudo(ProcessusOptiones));
    optiones.directorium = _radix();
    r = processus_exsequi_cum(argumenta, &optiones, MORA_AEDIFICATIONIS,
        piscina);
    CREDO_VERUM(r.successus);
    CREDO_AEQUALIS_I32(r.codex_exitus, ZEPHYRUM);
    CREDO_CHORDA_CONTINET(r.effusio,
        chorda_ex_literis("probationes/probatio_xar.c", piscina));
    per (k = ZEPHYRUM; k < r.effusio.mensura; k++)
    {
        si (r.effusio.datum[k] == '\n')
        {
            lineae++;
        }
    }
    CREDO_VERUM(lineae >= CD);
}

/* M5 T0: functio in indice ignota (index absens) -> mundus totus
 * per bibliothecas, non lib/ totus (parcum 01M6FG1G) */
interior vacuum
_sectio_interpretare_totus (
    CredoContextus* c)
{
               Piscina* piscina = c->piscina;
     ProcessusOptiones  optiones;
     ProcessusResultus  r;
    constans character* argumenta[] = {
        "officina/interpretare.sh",
        "-index",
        "officina/probationes/fixa/bibliothecae/absens.tsv",
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
    CREDO_CHORDA_CONTINET(r.erratum,
        chorda_ex_literis("mundus totus", piscina));
    CREDO_FALSUM(chorda_continet(r.erratum,
        chorda_ex_literis("NEXUS FRACTUS", piscina)));
}

/* vindex.sh: lanceator debuggeris ligatur et currit. A 8bbf97f7
 * usque ad 2026-11-12 non ligabatur (nexus_ordines deerat - idem
 * vitium ac colloquii); nulla porta eum currebat. -imago: punctum
 * in credo_suitam_currere, cursus ad pausam, effigies una. */
interior vacuum
_sectio_vindex (
    CredoContextus* c)
{
               Piscina* piscina = c->piscina;
     ProcessusOptiones  optiones;
     ProcessusResultus  r;
    constans character* argumenta[] = {
        "officina/vindex.sh",
        "probationes/probatio_chorda.c",
        "-plagulae", "chorda,credo",
        "-imago", "credo_suitam_currere",
        NIHIL
    };

    memset(&optiones, ZEPHYRUM, magnitudo(ProcessusOptiones));
    optiones.directorium = _radix();
    r = processus_exsequi_cum(argumenta, &optiones, MORA_AEDIFICATIONIS,
        piscina);
    CREDO_VERUM(r.successus);
    CREDO_AEQUALIS_I32(r.codex_exitus, ZEPHYRUM);
    CREDO_CHORDA_CONTINET(r.effusio,
        chorda_ex_literis("VINDEX  lib/credo.c  [PAUSA]", piscina));
    CREDO_CHORDA_CONTINET(r.effusio,
        chorda_ex_literis("g gradus", piscina));
}

/* interpretare -nativum (M5 T7): scopus per dorsum compilatur ante
 * cursum - fructus et exitus idem; scopus quem dorsum recusat
 * interpretatus currit, causa nominata, exitus immutatus */
interior vacuum
_sectio_interpretare_nativum (
    CredoContextus* c)
{
               Piscina* piscina = c->piscina;
     ProcessusOptiones  optiones;
     ProcessusResultus  r;
    constans character* argumenta[] = {
        "officina/interpretare.sh",
        "-nativum",
        "fasti_est_bissextilis",
        "2024",
        NIHIL
    };
    /* malloc per pontem et memoria (M5 T8): functio vera lib/ */
    constans character* cum_acervo[] = {
        "officina/interpretare.sh",
        "-nativum",
        "piscina_generare_dynamicum",
        "\"x\"",
        "1024",
        NIHIL
    };
    /* fluitantia nondum (Opus IX): recusata, interpretata currit */
    constans character* recusanda[] = {
        "officina/interpretare.sh",
        "-nativum",
        "color_srgb_ad_linearem",
        "0.5",
        NIHIL
    };

    memset(&optiones, ZEPHYRUM, magnitudo(ProcessusOptiones));
    optiones.directorium = _radix();
    r = processus_exsequi_cum(argumenta, &optiones, MORA_AEDIFICATIONIS,
        piscina);
    CREDO_VERUM(r.successus);
    CREDO_AEQUALIS_I32(r.codex_exitus, ZEPHYRUM);
    CREDO_VERUM(chorda_aequalis_literis(r.effusio, "1\n"));
    CREDO_CHORDA_CONTINET(r.erratum, chorda_ex_literis(
        "nativum: fasti_est_bissextilis compilata", piscina));
    CREDO_CHORDA_CONTINET(r.erratum,
        chorda_ex_literis("[BENE", piscina));

    r = processus_exsequi_cum(cum_acervo, &optiones,
        MORA_AEDIFICATIONIS, piscina);
    CREDO_VERUM(r.successus);
    CREDO_AEQUALIS_I32(r.codex_exitus, ZEPHYRUM);
    CREDO_CHORDA_CONTINET(r.erratum, chorda_ex_literis(
        "nativum: piscina_generare_dynamicum compilata", piscina));
    CREDO_CHORDA_CONTINET(r.erratum,
        chorda_ex_literis("[BENE", piscina));

    r = processus_exsequi_cum(recusanda, &optiones, MORA_AEDIFICATIONIS,
        piscina);
    CREDO_VERUM(r.successus);
    CREDO_AEQUALIS_I32(r.codex_exitus, ZEPHYRUM);
    CREDO_VERUM(chorda_aequalis_literis(r.effusio, "0.214041\n"));
    CREDO_CHORDA_CONTINET(r.erratum, chorda_ex_literis(
        "nativum recusatum: ", piscina));
    CREDO_CHORDA_CONTINET(r.erratum,
        chorda_ex_literis("[BENE", piscina));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "colloquium scriptum", _sectio_colloquium, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "interpretare sine plagulis", _sectio_interpretare, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "interpretare fasti", _sectio_fasti, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "cursor sola", _sectio_cursor_sola, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "cursor sine tecto", _sectio_cursor_tectum, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "interpretare mundus totus", _sectio_interpretare_totus, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "vindex imago", _sectio_vindex, NIHIL, NIHIL, NIHIL, NIHIL },
    { "interpretare -nativum", _sectio_interpretare_nativum, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("officina_lanceatores", SECTIONES);
}

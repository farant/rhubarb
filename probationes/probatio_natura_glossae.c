/* probatio_natura_glossae.c - census glossarum (spec glossae par. 5)
 *
 * Quinque sectiones: corpus plenum (copertura tota, porta viret),
 * corpus lacunosum (CARENS et GEMINA nominatim, porta rubet),
 * -linguae (electio postulata verdictum mutat), exitus II (nihil
 * cursum numquam sanitas tacita), pagina html (encyclopaedia et
 * lacunae fusae, evasio probata).
 *
 * Instrumentum per processum exsecutum (ut probatio_natura_canones):
 * effusio (stdout) capta, assertio in lineis TSV modi -machina fit.
 */
#include "postulata_posix.h"
#include "latina.h"
#include "credo.h"
#include "chorda.h"
#include "piscina.h"
#include "filum.h"
#include "processus.h"
#include <stdio.h>
#include <sys/stat.h>

/* ==================================================
 * Custodes fontium - binarium fonti dissimile probationem de
 * codice qui iam non exsistit facit (mos probatio_natura_canones)
 * ================================================== */

staticus constans character* NG_FONTES[] = {
    "tools/natura_glossae.c",
    "lib/natura.c",
    "include/natura.h",
    "lib/stml.c",
    "include/stml.h",
    NIHIL
};

interior constans character* _fons_absens(vacuum);
interior constans character* _fons_recentior_binario(
                                 constans character* binarium);
interior ProcessusResultus _censum_currere(
                               constans character* radix,
                               constans character* arg1,
                               constans character* arg2,
                               constans character* arg3,
                               Piscina* piscina);
interior b32 _continet_literis(chorda effusio,
                               constans character* literae,
                               Piscina* piscina);

interior constans character*
_fons_absens(
    vacuum)
{
    structura stat  f;
    i32             i;

    per (i = ZEPHYRUM; NG_FONTES[i] != NIHIL; i++)
    {
        si (stat(NG_FONTES[i], &f) != ZEPHYRUM)
        {
            redde NG_FONTES[i];
        }
    }
    redde NIHIL;
}

interior constans character*
_fons_recentior_binario(
    constans character*  binarium)
{
    structura stat  b;
    i32             i;

    si (stat(binarium, &b) != ZEPHYRUM)
    {
        redde NIHIL;
    }

    per (i = ZEPHYRUM; NG_FONTES[i] != NIHIL; i++)
    {
        structura stat f;

        si (stat(NG_FONTES[i], &f) != ZEPHYRUM)
        {
            perge;   /* absentiam _fons_absens sola nuntiat */
        }
        si (f.st_mtime > b.st_mtime)
        {
            redde NG_FONTES[i];
        }
    }
    redde NIHIL;
}

/* bin/natura_glossae super corpore dato currere; effusio (stdout)
 * capta - assertio in lineis TSV fit */
interior ProcessusResultus
_censum_currere(
    constans character*  radix,
    constans character*  arg1,
    constans character*  arg2,
    constans character*  arg3,
    Piscina*             piscina)
{
    constans character* argumenta[VIII];
    i32                 n;

    n = ZEPHYRUM;
    argumenta[n++] = "bin/natura_glossae";
    argumenta[n++] = "-radix";
    argumenta[n++] = radix;
    si (arg1) { argumenta[n++] = arg1; }
    si (arg2) { argumenta[n++] = arg2; }
    si (arg3) { argumenta[n++] = arg3; }
    argumenta[n] = NIHIL;

    redde processus_exsequi(argumenta, 60000, piscina);
}

interior b32
_continet_literis(
    chorda               effusio,
    constans character*  literae,
    Piscina*             piscina)
{
    redde chorda_continet(effusio,
                          chorda_ex_literis(literae, piscina));
}

s32 principale (vacuum)
{
         b32  praeteritus;
    Piscina*  piscina;

    piscina = piscina_generare_dynamicum("probatio_natura_glossae",
                                         8388608);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ========================================================
     * 0. instrumentum paratur: absens -> struere semel;
     *    stalum -> RECUSARE (mos probatio_natura_canones)
     * ======================================================== */

    {
        imprimere("\n--- 0. instrumentum ---\n");

        si (!filum_existit("bin/natura_glossae"))
        {
            constans character* struere[II];
            ProcessusResultus   rs;

            imprimere("  (bin/natura_glossae abest - struo semel)\n");
            struere[ZEPHYRUM] = "./tools/natura_struere.sh";
            struere[I]        = NIHIL;
            rs = processus_exsequi(struere, 300000, piscina);
            si (!rs.successus || rs.codex_exitus != ZEPHYRUM)
            {
                imprimere("  structor fefellit (codex %u)\n",
                          rs.codex_exitus);
            }
        }
        CREDO_VERUM (filum_existit("bin/natura_glossae"));

        {
            constans character* absens;
            constans character* stalum;

            absens = _fons_absens();
            si (absens)
            {
                imprimere("  custos '%s' ABEST - index custodum "
                          "fontem nominat qui non est\n", absens);
            }
            CREDO_NIHIL (absens);

            stalum = _fons_recentior_binario("bin/natura_glossae");
            si (stalum)
            {
                imprimere("  bin/natura_glossae STALUS ('%s' "
                          "recentior) - strue primum: "
                          "./tools/natura_struere.sh\n", stalum);
            }
            CREDO_NIHIL (stalum);
        }
    }


    /* ========================================================
     * I. corpus plenum: copertura tota, porta viret
     * ======================================================== */

    {
        ProcessusResultus r;

        imprimere("\n--- I. corpus plenum ---\n");

        r = _censum_currere("probationes/exempla/gl_plenum",
                            "-machina", NIHIL, NIHIL, piscina);
        CREDO_VERUM (r.successus);
        CREDO_AEQUALIS_I32 ((i32)r.codex_exitus, ZEPHYRUM);
        CREDO_VERUM (_continet_literis(r.effusio,
            "SUMMA\tla\t2\t2", piscina));
        CREDO_VERUM (_continet_literis(r.effusio,
            "SUMMA\ten\t2\t2", piscina));
        CREDO_VERUM (_continet_literis(r.effusio,
            "SUMMA\tfr\t2\t2", piscina));
        CREDO_VERUM (!_continet_literis(r.effusio, "CARENS",
                                        piscina));
        CREDO_VERUM (!_continet_literis(r.effusio, "GEMINA",
                                        piscina));

        r = _censum_currere("probationes/exempla/gl_plenum",
                            "-porta", NIHIL, NIHIL, piscina);
        CREDO_VERUM (r.successus);
        CREDO_AEQUALIS_I32 ((i32)r.codex_exitus, ZEPHYRUM);
    }


    /* ========================================================
     * II. corpus lacunosum: carentia et gemina NOMINATIM -
     *     relatio index operum est, porta sola obstat
     * ======================================================== */

    {
        ProcessusResultus r;

        imprimere("\n--- II. corpus lacunosum ---\n");

        r = _censum_currere("probationes/exempla/gl_lacunosum",
                            "-machina", NIHIL, NIHIL, piscina);
        CREDO_VERUM (r.successus);
        /* relatio numquam obstat - modus -porta solus */
        CREDO_AEQUALIS_I32 ((i32)r.codex_exitus, ZEPHYRUM);
        CREDO_VERUM (_continet_literis(r.effusio,
            "CARENS\tprobatio\tgamma\tfr", piscina));
        CREDO_VERUM (_continet_literis(r.effusio,
            "CARENS\tprobatio\tdelta\tla", piscina));
        CREDO_VERUM (_continet_literis(r.effusio,
            "GEMINA\tprobatio\tdelta\ten", piscina));
        CREDO_VERUM (_continet_literis(r.effusio,
            "SUMMA\tfr\t1\t2", piscina));

        r = _censum_currere("probationes/exempla/gl_lacunosum",
                            "-porta", NIHIL, NIHIL, piscina);
        CREDO_VERUM (r.successus);
        CREDO_AEQUALIS_I32 ((i32)r.codex_exitus, I);
    }


    /* ========================================================
     * III. -linguae: electio postulata verdictum mutat
     * ======================================================== */

    {
        ProcessusResultus r;

        imprimere("\n--- III. -linguae ---\n");

        /* fr sola postulata: gamma caret -> porta rubet */
        r = _censum_currere("probationes/exempla/gl_lacunosum",
                            "-porta", "-linguae", "fr", piscina);
        CREDO_VERUM (r.successus);
        CREDO_AEQUALIS_I32 ((i32)r.codex_exitus, I);

        /* en sola in pleno: porta viret */
        r = _censum_currere("probationes/exempla/gl_plenum",
                            "-porta", "-linguae", "en", piscina);
        CREDO_VERUM (r.successus);
        CREDO_AEQUALIS_I32 ((i32)r.codex_exitus, ZEPHYRUM);
    }


    /* ========================================================
     * IV. exitus II - NIHIL CURSUM numquam sanitas tacita
     * ======================================================== */

    {
        ProcessusResultus r;

        imprimere("\n--- IV. exitus II ---\n");

        r = _censum_currere("probationes/exempla/NON_EST",
                            NIHIL, NIHIL, NIHIL, piscina);
        CREDO_VERUM (r.successus);
        CREDO_AEQUALIS_I32 ((i32)r.codex_exitus, II);

        r = _censum_currere("probationes/exempla/gl_plenum",
                            "-ignotum", NIHIL, NIHIL, piscina);
        CREDO_VERUM (r.successus);
        CREDO_AEQUALIS_I32 ((i32)r.codex_exitus, II);
    }


    /* ========================================================
     * V. pagina html: encyclopaedia et lacunae fusae
     * ======================================================== */

    {
        ProcessusResultus r;
        chorda            pagina;

        imprimere("\n--- V. pagina html ---\n");

        r = _censum_currere("probationes/exempla/gl_lacunosum",
                            "-html", "build/probatio_glossae.html",
                            NIHIL, piscina);
        CREDO_VERUM (r.successus);
        CREDO_AEQUALIS_I32 ((i32)r.codex_exitus, ZEPHYRUM);

        pagina = filum_legere_totum("build/probatio_glossae.html",
                                    piscina);
        CREDO_MAIOR_I32 (pagina.mensura, (i32)ZEPHYRUM);
        CREDO_VERUM (_continet_literis(pagina, "<!DOCTYPE html>",
                                       piscina));
        /* glossa vera transit */
        CREDO_VERUM (_continet_literis(pagina,
            "Gap kind: French missing", piscina));
        /* lacuna VISIBILIS - pagina index operum est */
        CREDO_VERUM (_continet_literis(pagina, "deest", piscina));
        /* evasio: '<' fixturae iterum evasum exit */
        CREDO_VERUM (_continet_literis(pagina,
            "&lt;markup&gt;", piscina));

        /* artefactum verum commissum: sententia stabilis plantae */
        pagina = filum_legere_totum("natura/cocta/glossae.html",
                                    piscina);
        CREDO_MAIOR_I32 (pagina.mensura, (i32)ZEPHYRUM);
        CREDO_VERUM (_continet_literis(pagina, "photosynthesis",
                                       piscina));

        /* syntaxis: exemplum canonicum tagi e canone cocto -
         * attributum identitatis, optiones electionis, liberum
         * nidificatum (historia -> eventum) */
        CREDO_VERUM (_continet_literis(pagina,
            "&lt;planta nomen=\"#nomen\"", piscina));
        CREDO_VERUM (_continet_literis(pagina,
            "herba|frutex|arbor|scandens", piscina));
        CREDO_VERUM (_continet_literis(pagina, "&lt;eventum",
                                       piscina));
    }


    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}

/* probatio_processus.c - Probationes bibliothecae processuum.
 *
 * OFFLINE integre: /bin/echo, /bin/sh, /bin/cat et stipes localis
 * sola adhibentur - nulla rete, nullus daemon, nullum binarium
 * generatum.
 *
 * Probat inter alia duas res quas system() praestare NON potest et
 * propter quas haec bibliotheca exsistit:
 *   - stdout et stderr SEPARATIM captos
 *   - argumenta cum spatiis/virgulis/lineis novis INTACTA (nulla
 *     concha ea interpretatur)
 */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "processus.h"
#include <stdio.h>
#include <string.h>

/* chordam ut litterae NUL-terminatas in piscina (pro strstr/strcmp) */
interior constans character*
_litterae_ex (Piscina* piscina, chorda c)
{
    character* l = (character*)piscina_allocare(piscina,
        (memoriae_index)(c.mensura + I));

    si (l == NIHIL)
    {
        redde "";
    }
    si (c.mensura > ZEPHYRUM)
    {
        memcpy(l, c.datum, (memoriae_index)c.mensura);
    }
    l[c.mensura] = '\0';
    redde l;
}

s32 principale (vacuum)
{
    Piscina* piscina;
    b32 praeteritus;

    piscina = piscina_generare_dynamicum("probatio_processus",
        8388608);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);

    /* I. exsecutio simplex: effusio capta, codex zerus */
    {
        constans character* argumenta[IV];
        ProcessusResultus r;

        imprimere("\n--- I. exsecutio simplex ---\n");
        argumenta[0] = "/bin/echo";
        argumenta[I] = "salve";
        argumenta[II] = NIHIL;
        r = processus_exsequi(argumenta, 5000, piscina);

        CREDO_VERUM (r.successus);
        CREDO_AEQUALIS_I32 (r.codex_exitus, ZEPHYRUM);
        CREDO_VERUM (r.error == PROCESSUS_OK);
        CREDO_VERUM (strcmp(_litterae_ex(piscina, r.effusio),
            "salve\n") == ZEPHYRUM);
        CREDO_AEQUALIS_I32 (r.erratum.mensura, ZEPHYRUM);
    }

    /* II. codex exitus non-zerus = SUCCESSUS (processus cucurrit
     * et respondit; successus significat generationem, non
     * felicitatem imperii) */
    {
        constans character* argumenta[IV];
        ProcessusResultus r;

        imprimere("\n--- II. codex exitus non-zerus ---\n");
        argumenta[0] = "/bin/sh";
        argumenta[I] = "-c";
        argumenta[II] = "exit 5";
        argumenta[III] = NIHIL;
        r = processus_exsequi(argumenta, 5000, piscina);

        CREDO_VERUM (r.successus);
        CREDO_AEQUALIS_I32 (r.codex_exitus, V);
        CREDO_VERUM (r.error == PROCESSUS_OK);
    }

    /* III. stdout et stderr SEPARATIM (quod system() nequit) */
    {
        constans character* argumenta[IV];
        ProcessusResultus r;
        constans character* ef;
        constans character* er;

        imprimere("\n--- III. effusio et erratum separata ---\n");
        argumenta[0] = "/bin/sh";
        argumenta[I] = "-c";
        argumenta[II] = "echo ad-effusionem; echo ad-erratum 1>&2";
        argumenta[III] = NIHIL;
        r = processus_exsequi(argumenta, 5000, piscina);

        CREDO_VERUM (r.successus);
        ef = _litterae_ex(piscina, r.effusio);
        er = _litterae_ex(piscina, r.erratum);
        CREDO_VERUM (strstr(ef, "ad-effusionem") != NIHIL);
        CREDO_VERUM (strstr(ef, "ad-erratum") == NIHIL);
        CREDO_VERUM (strstr(er, "ad-erratum") != NIHIL);
        CREDO_VERUM (strstr(er, "ad-effusionem") == NIHIL);
    }

    /* IV. NULLA CONCHA: argumentum cum spatiis, virgulis, linea
     * nova INTACTUM transit. Hoc est ratio propter quam vector
     * adhibetur - sub /bin/sh haec omnia frangerentur aut
     * imperium alienum pararent. */
    {
        constans character* argumenta[IV];
        ProcessusResultus r;
        constans character* ef;

        imprimere("\n--- IV. argumenta sine concha ---\n");
        argumenta[0] = "/bin/echo";
        argumenta[I] = "spatium 'virgula' \"duplex\" ; rm -rf /\n"
            "linea nova";
        argumenta[II] = NIHIL;
        r = processus_exsequi(argumenta, 5000, piscina);

        CREDO_VERUM (r.successus);
        ef = _litterae_ex(piscina, r.effusio);
        CREDO_VERUM (strstr(ef, "'virgula'") != NIHIL);
        CREDO_VERUM (strstr(ef, "\"duplex\"") != NIHIL);
        CREDO_VERUM (strstr(ef, "; rm -rf /") != NIHIL);
        CREDO_VERUM (strstr(ef, "linea nova") != NIHIL);
    }

    /* V. terminus temporis: processus occisus, error nominatus */
    {
        constans character* argumenta[IV];
        ProcessusResultus r;

        imprimere("\n--- V. mora excessa ---\n");
        argumenta[0] = "/bin/sh";
        argumenta[I] = "-c";
        argumenta[II] = "sleep 10";
        argumenta[III] = NIHIL;
        r = processus_exsequi(argumenta, 300, piscina);

        CREDO_FALSUM (r.successus);
        CREDO_VERUM (r.error == PROCESSUS_ERROR_TEMPUS);
        /* occisus intra tempus rationabile, non post X secunda */
        CREDO_VERUM (r.mora_ms < 3000);
    }

    /* VI. binarium absens: EXEC distinctum a codice 127.
     * MOMENTI PRO VILLA: ssh ipse 127 reddit cum imperium REMOTUM
     * deest - si eos confunderemus, "ssh non installatum" et
     * "systemctl in capsa remota abest" idem viderentur. */
    {
        constans character* argumenta[III];
        ProcessusResultus r;

        imprimere("\n--- VI. exec fallitum vs codex CXXVII ---\n");
        argumenta[0] = "/bin/nullum-tale-binarium-est";
        argumenta[I] = NIHIL;
        r = processus_exsequi(argumenta, 5000, piscina);

        CREDO_FALSUM (r.successus);
        CREDO_VERUM (r.error == PROCESSUS_ERROR_EXEC);

        /* contra: processus VERUS qui 127 reddit */
        {
            constans character* arg2[IV];
            ProcessusResultus r2;

            arg2[0] = "/bin/sh";
            arg2[I] = "-c";
            arg2[II] = "exit 127";
            arg2[III] = NIHIL;
            r2 = processus_exsequi(arg2, 5000, piscina);

            CREDO_VERUM (r2.successus);
            CREDO_AEQUALIS_I32 (r2.codex_exitus, CXXVII);
            CREDO_VERUM (r2.error == PROCESSUS_OK);
        }
    }

    /* VII. effusio maior quam frustum unum (sacculus crescens) */
    {
        constans character* argumenta[IV];
        ProcessusResultus r;

        imprimere("\n--- VII. effusio magna ---\n");
        argumenta[0] = "/bin/sh";
        argumenta[I] = "-c";
        argumenta[II] = "i=0; while [ $i -lt 3000 ]; do "
            "echo linea-longa-ad-sacculum-implendum; i=$((i+1)); done";
        argumenta[III] = NIHIL;
        r = processus_exsequi(argumenta, 10000, piscina);

        CREDO_VERUM (r.successus);
        CREDO_AEQUALIS_I32 (r.codex_exitus, ZEPHYRUM);
        /* 3000 x XXXI octeti > IV KiB frustum multiplex */
        CREDO_VERUM (r.effusio.mensura > 90000);
    }

    /* VIII. argumenta invalida recusantur clare */
    {
        constans character* argumenta[II];
        ProcessusResultus r;

        imprimere("\n--- VIII. argumenta invalida ---\n");
        argumenta[0] = NIHIL;
        r = processus_exsequi(argumenta, 5000, piscina);
        CREDO_FALSUM (r.successus);
        CREDO_VERUM (r.error == PROCESSUS_ERROR_ARGUMENTA);

        r = processus_exsequi(NIHIL, 5000, piscina);
        CREDO_FALSUM (r.successus);
        CREDO_VERUM (r.error == PROCESSUS_ERROR_ARGUMENTA);
    }

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    redde I;
}

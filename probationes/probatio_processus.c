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
_litterae_ex (
    Piscina* piscina,
     chorda  c)
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
        b32  praeteritus;

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
         ProcessusResultus  r;

        imprimere("\n--- I. exsecutio simplex ---\n");
        argumenta[0]   = "/bin/echo";
        argumenta[I]   = "salve";
        argumenta[II]  = NIHIL;
        r              = processus_exsequi(argumenta, 5000, piscina);

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
         ProcessusResultus  r;

        imprimere("\n--- II. codex exitus non-zerus ---\n");
        argumenta[0]    = "/bin/sh";
        argumenta[I]    = "-c";
        argumenta[II]   = "exit 5";
        argumenta[III]  = NIHIL;
        r               = processus_exsequi(argumenta, 5000, piscina);

        CREDO_VERUM (r.successus);
        CREDO_AEQUALIS_I32 (r.codex_exitus, V);
        CREDO_VERUM (r.error == PROCESSUS_OK);
    }

    /* III. stdout et stderr SEPARATIM (quod system() nequit) */
    {
        constans character* argumenta[IV];
         ProcessusResultus  r;
        constans character* ef;
        constans character* er;

        imprimere("\n--- III. effusio et erratum separata ---\n");
        argumenta[0]    = "/bin/sh";
        argumenta[I]    = "-c";
        argumenta[II]   = "echo ad-effusionem; echo ad-erratum 1>&2";
        argumenta[III]  = NIHIL;
        r               = processus_exsequi(argumenta, 5000, piscina);

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
         ProcessusResultus  r;
        constans character* ef;

        imprimere("\n--- IV. argumenta sine concha ---\n");
        argumenta[0] = "/bin/echo";
        argumenta[I] = "spatium 'virgula' \"duplex\" ; rm -rf /\n"
            "linea nova";
        argumenta[II]  = NIHIL;
        r              = processus_exsequi(argumenta, 5000, piscina);

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
         ProcessusResultus  r;

        imprimere("\n--- V. mora excessa ---\n");
        argumenta[0]    = "/bin/sh";
        argumenta[I]    = "-c";
        argumenta[II]   = "sleep 10";
        argumenta[III]  = NIHIL;
        r               = processus_exsequi(argumenta, 300, piscina);

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
         ProcessusResultus  r;

        imprimere("\n--- VI. exec fallitum vs codex CXXVII ---\n");
        argumenta[0]  = "/bin/nullum-tale-binarium-est";
        argumenta[I]  = NIHIL;
        r             = processus_exsequi(argumenta, 5000, piscina);

        CREDO_FALSUM (r.successus);
        CREDO_VERUM (r.error == PROCESSUS_ERROR_EXEC);

        /* contra: processus VERUS qui 127 reddit */
        {
            constans character* arg2[IV];
             ProcessusResultus  r2;

            arg2[0]    = "/bin/sh";
            arg2[I]    = "-c";
            arg2[II]   = "exit 127";
            arg2[III]  = NIHIL;
            r2         = processus_exsequi(arg2, 5000, piscina);

            CREDO_VERUM (r2.successus);
            CREDO_AEQUALIS_I32 (r2.codex_exitus, CXXVII);
            CREDO_VERUM (r2.error == PROCESSUS_OK);
        }
    }

    /* VII. effusio maior quam frustum unum (sacculus crescens) */
    {
        constans character* argumenta[IV];
         ProcessusResultus  r;

        imprimere("\n--- VII. effusio magna ---\n");
        argumenta[0] = "/bin/sh";
        argumenta[I] = "-c";
        argumenta[II] = "i=0; while [ $i -lt 3000 ]; do "
            "echo linea-longa-ad-sacculum-implendum; i=$((i+1)); done";
        argumenta[III]  = NIHIL;
        r               = processus_exsequi(argumenta, 10000, piscina);

        CREDO_VERUM (r.successus);
        CREDO_AEQUALIS_I32 (r.codex_exitus, ZEPHYRUM);
        /* 3000 x XXXI octeti > IV KiB frustum multiplex */
        CREDO_VERUM (r.effusio.mensura > 90000);
    }

    /* VIII. argumenta invalida recusantur clare */
    {
        constans character* argumenta[II];
         ProcessusResultus  r;

        imprimere("\n--- VIII. argumenta invalida ---\n");
        argumenta[0]  = NIHIL;
        r             = processus_exsequi(argumenta, 5000, piscina);
        CREDO_FALSUM (r.successus);
        CREDO_VERUM (r.error == PROCESSUS_ERROR_ARGUMENTA);

        r = processus_exsequi(NIHIL, 5000, piscina);
        CREDO_FALSUM (r.successus);
        CREDO_VERUM (r.error == PROCESSUS_ERROR_ARGUMENTA);
    }


    /* ==============================================================
     * IX-XVI. SEMITA INCREMENTALIS
     *
     * Ratio exsistendi: domus filis CARET, ergo facies vitreae
     * filum UNUM habet. processus_exsequi cum mora V secundorum
     * fenestram per V secunda congelaret. Probationes sequentes id
     * quod maxime importat figunt: PULSUM NON OBSTARE.
     * ============================================================== */

    /* IX. AEQUIVALENTIA: semita incrementalis idem quod obstans.
     * Haec probatio adfirmationem 'duae semitae, ansa una' custodit
     * - si ansae dissidere inciperent, hic primum clamaret. */
    {
        constans character* argumenta[IV];
         ProcessusResultus  obstans;
         ProcessusResultus  incrementalis;
                 Processus* p;

        imprimere("\n--- IX. aequivalentia semitarum ---\n");
        argumenta[0] = "/bin/sh";
        argumenta[I] = "-c";
        argumenta[II] = "echo ad-effusionem; echo ad-erratum 1>&2;"
            " exit 3";
        argumenta[III] = NIHIL;

        obstans = processus_exsequi(argumenta, 5000, piscina);

        p = processus_incipere(argumenta, 5000, piscina);
        CREDO_NON_NIHIL (p);
        dum (processus_pulsare(p) == PROCESSUS_CURRIT)
        {
            /* nihil - pulsamus donec paratus */
        }
        incrementalis = processus_metere(p);

        CREDO_VERUM (obstans.successus);
        CREDO_VERUM (incrementalis.successus);
        CREDO_AEQUALIS_I32 (incrementalis.codex_exitus,
            obstans.codex_exitus);
        CREDO_AEQUALIS_I32 (incrementalis.codex_exitus, III);
        CREDO_VERUM (strcmp(_litterae_ex(piscina,
            incrementalis.effusio),
            _litterae_ex(piscina, obstans.effusio)) == ZEPHYRUM);
        CREDO_VERUM (strcmp(_litterae_ex(piscina,
            incrementalis.erratum),
            _litterae_ex(piscina, obstans.erratum)) == ZEPHYRUM);
    }

    /* X. PULSUS NON OBSTAT - probatio quae totam rem portat.
     *
     * Infans per ~CCC ms dormit et NIHIL effundit. Si pulsus
     * obstaret, ansa SEMEL curreret. Numerus pulsuum ergo mensura
     * DIRECTA est: milia = non obstat, unum = obstat.
     *
     * Nullum horologium hic adhibetur consulto - numerus pulsuum
     * per se probat, et adsertiones temporis in machinis oneratis
     * fluctuant. */
    {
        constans character* argumenta[IV];
                 Processus* p;
         ProcessusResultus  r;
                       i32  pulsus = ZEPHYRUM;

        imprimere("\n--- X. pulsus non obstat ---\n");
        argumenta[0]    = "/bin/sh";
        argumenta[I]    = "-c";
        argumenta[II]   = "sleep 0.3";
        argumenta[III]  = NIHIL;

        p = processus_incipere(argumenta, 10000, piscina);
        CREDO_NON_NIHIL (p);
        dum (processus_pulsare(p) == PROCESSUS_CURRIT)
        {
            pulsus++;
            si (pulsus > 5000000)
            {
                frange;   /* custos contra ansam aeternam */
            }
        }
        r = processus_metere(p);

        CREDO_VERUM (r.successus);
        CREDO_AEQUALIS_I32 (r.codex_exitus, ZEPHYRUM);
        /* si pulsus obstaret, hic I esset */
        CREDO_VERUM (pulsus > 1000);
        /* et infans vere per ~CCC ms cucurrit */
        CREDO_VERUM (r.mora_ms > 100);
    }

    /* XI. terminus custoditur in modo PULSANTE quoque - vocator
     * tardus processum aeternum parere non debet */
    {
        constans character* argumenta[IV];
                 Processus* p;
         ProcessusResultus  r;
                       i32  pulsus = ZEPHYRUM;

        imprimere("\n--- XI. terminus in modo pulsante ---\n");
        argumenta[0]    = "/bin/sh";
        argumenta[I]    = "-c";
        argumenta[II]   = "sleep 10";
        argumenta[III]  = NIHIL;

        p = processus_incipere(argumenta, 300, piscina);
        CREDO_NON_NIHIL (p);
        dum (processus_pulsare(p) == PROCESSUS_CURRIT)
        {
            pulsus++;
            si (pulsus > 5000000)
            {
                frange;
            }
        }
        r = processus_metere(p);

        CREDO_FALSUM (r.successus);
        CREDO_VERUM (r.error == PROCESSUS_ERROR_TEMPUS);
        CREDO_VERUM (r.mora_ms < 3000);
    }

    /* XII. abrumpere: pyxis 'siste' faciei */
    {
        constans character* argumenta[IV];
                 Processus* p;
         ProcessusResultus  r;
                       i32  i;

        imprimere("\n--- XII. abrumpere ---\n");
        argumenta[0]    = "/bin/sh";
        argumenta[I]    = "-c";
        argumenta[II]   = "sleep 10";
        argumenta[III]  = NIHIL;

        p = processus_incipere(argumenta, 30000, piscina);
        CREDO_NON_NIHIL (p);
        per (i = ZEPHYRUM; i < X; i++)
        {
            (vacuum)processus_pulsare(p);
        }
        processus_abrumpere(p);
        r = processus_metere(p);

        CREDO_FALSUM (r.successus);
        CREDO_VERUM (r.error == PROCESSUS_ERROR_TEMPUS);
        /* mora X secundorum NON exspectata est */
        CREDO_VERUM (r.mora_ms < 3000);
        /* iam perfectus: pulsus posterior PARATUS statim */
        CREDO_VERUM (processus_pulsare(p) == PROCESSUS_PARATUS);
    }

    /* XIII. manubrium FRACTUM: error ante generationem manubrium
     * VALIDUM reddit quod statim paratus est - vocator semitam
     * UNAM tractat, non duas */
    {
        constans character* argumenta[II];
                 Processus* p;
         ProcessusResultus  r;

        imprimere("\n--- XIII. manubrium fractum ---\n");
        argumenta[0] = NIHIL;

        p = processus_incipere(argumenta, 5000, piscina);
        CREDO_NON_NIHIL (p);
        CREDO_VERUM (processus_pulsare(p) == PROCESSUS_PARATUS);
        r = processus_metere(p);
        CREDO_FALSUM (r.successus);
        CREDO_VERUM (r.error == PROCESSUS_ERROR_ARGUMENTA);

        /* piscina NIHIL est unicus casus qui NIHIL reddit */
        CREDO_NIHIL (processus_incipere(argumenta, 5000, NIHIL));
    }

    /* XIV. metere sine pulsando (commoditas) + idempotentia */
    {
        constans character* argumenta[IV];
                 Processus* p;
         ProcessusResultus  prima;
         ProcessusResultus  secunda;

        imprimere("\n--- XIV. metere sine pulsu, bis ---\n");
        argumenta[0]   = "/bin/echo";
        argumenta[I]   = "salve";
        argumenta[II]  = NIHIL;

        p = processus_incipere(argumenta, 5000, piscina);
        CREDO_NON_NIHIL (p);
        /* nullus pulsus - metere obstat et perficit */
        prima = processus_metere(p);
        CREDO_VERUM (prima.successus);
        CREDO_VERUM (strcmp(_litterae_ex(piscina, prima.effusio),
            "salve\n") == ZEPHYRUM);

        /* idempotens: idem resultum, nulla messis dupla */
        secunda = processus_metere(p);
        CREDO_VERUM (secunda.successus);
        CREDO_AEQUALIS_I32 (secunda.codex_exitus, prima.codex_exitus);
        CREDO_VERUM (strcmp(_litterae_ex(piscina, secunda.effusio),
            "salve\n") == ZEPHYRUM);
    }

    /* XV. effusio MAGNA per pulsus - sacculus per multos pulsus
     * crescit, nihil amittitur */
    {
        constans character* argumenta[IV];
                 Processus* p;
         ProcessusResultus  r;
                       i32  pulsus = ZEPHYRUM;

        imprimere("\n--- XV. effusio magna per pulsus ---\n");
        argumenta[0] = "/bin/sh";
        argumenta[I] = "-c";
        argumenta[II] = "i=0; while [ $i -lt 3000 ]; do "
            "echo linea-longa-ad-sacculum-implendum; i=$((i+1)); done";
        argumenta[III] = NIHIL;

        p = processus_incipere(argumenta, 10000, piscina);
        CREDO_NON_NIHIL (p);
        dum (processus_pulsare(p) == PROCESSUS_CURRIT)
        {
            pulsus++;
            si (pulsus > 5000000)
            {
                frange;
            }
        }
        r = processus_metere(p);

        CREDO_VERUM (r.successus);
        CREDO_AEQUALIS_I32 (r.codex_exitus, ZEPHYRUM);
        CREDO_VERUM (r.effusio.mensura > 90000);
    }

    /* XVI. exec fallitum per semitam incrementalem - distinctio
     * EXEC/CXXVII quam villa postulat per hanc semitam quoque
     * servatur */
    {
        constans character* argumenta[III];
                 Processus* p;
         ProcessusResultus  r;

        imprimere("\n--- XVI. exec fallitum incrementaliter ---\n");
        argumenta[0] = "/bin/nullum-tale-binarium-est";
        argumenta[I] = NIHIL;

        p = processus_incipere(argumenta, 5000, piscina);
        CREDO_NON_NIHIL (p);
        dum (processus_pulsare(p) == PROCESSUS_CURRIT)
        {
            /* nihil */
        }
        r = processus_metere(p);
        CREDO_FALSUM (r.successus);
        CREDO_VERUM (r.error == PROCESSUS_ERROR_EXEC);
    }


    /* XVII. NEPOS FISTULAM TENENS - infans abit, nepos vivit.
     *
     * CASUS VERUS QUI HOC REVELAVIT: ssh cum 'ControlPersist'
     * magistrum in tergo relinquit qui stdout/stderr NOSTRA
     * hereditat. Imperium nostrum finitur, infans abit, sed EOF
     * numquam venit quia magister finem scribendi TENET. Villa
     * contra servum verum in aeternum pendebat dum stipes (qui
     * magistrum non habet) feliciter transibat.
     *
     * Hic idem forma minima: 'echo; sleep 30 &' - concha effusionem
     * scribit, nepotem generat qui fistulam hereditat, et statim
     * abit. Ante emendationem haec probatio XXX secunda penderet.
     * ============================================================ */

    {
        constans character* argumenta[IV];
         ProcessusResultus  r;

        imprimere("\n--- XVII. nepos fistulam tenens ---\n");
        argumenta[0]    = "/bin/sh";
        argumenta[I]    = "-c";
        argumenta[II]   = "echo salve; sleep 30 &";
        argumenta[III]  = NIHIL;
        r               = processus_exsequi(argumenta, 10000, piscina);

        CREDO_VERUM (r.successus);
        CREDO_AEQUALIS_I32 (r.codex_exitus, ZEPHYRUM);
        /* effusio ANTE exitum scripta capitur - messis reliquias
         * haurit, non solum EOF exspectat */
        CREDO_VERUM (strstr(_litterae_ex(piscina, r.effusio),
            "salve") != NIHIL);
        /* ET NON EXSPECTAVIMUS nepotem: XXX secunda vs < III */
        CREDO_VERUM (r.mora_ms < 3000);
        CREDO_VERUM (r.error == PROCESSUS_OK);
    }

    /* XVIII. idem per semitam INCREMENTALEM */
    {
        constans character* argumenta[IV];
                 Processus* p;
         ProcessusResultus  r;
                       i32  pulsus = ZEPHYRUM;

        imprimere("\n--- XVIII. nepos, semita incrementalis ---\n");
        argumenta[0]    = "/bin/sh";
        argumenta[I]    = "-c";
        argumenta[II]   = "echo salve; sleep 30 &";
        argumenta[III]  = NIHIL;

        p = processus_incipere(argumenta, 10000, piscina);
        CREDO_NON_NIHIL (p);
        dum (processus_pulsare(p) == PROCESSUS_CURRIT)
        {
            pulsus++;
            si (pulsus > 5000000)
            {
                frange;
            }
        }
        r = processus_metere(p);
        CREDO_VERUM (r.successus);
        CREDO_VERUM (strstr(_litterae_ex(piscina, r.effusio),
            "salve") != NIHIL);
        CREDO_VERUM (r.mora_ms < 3000);
    }

    /* XIX. transformare: SOLUM semitae defectus hic probantur -
     * exsecutio felix hunc ipsum processum substitueret (semita
     * felix in probatio_officina_renovatio integre agitur) */
    {
        constans character* argumenta[II];

        imprimere("\n--- XIX. transformare (defectus soli) ---\n");
        argumenta[0] = "/bin/nullum-tale-binarium-est";
        argumenta[I] = NIHIL;
        CREDO_FALSUM (processus_transformare(argumenta));

        argumenta[0] = NIHIL;
        CREDO_FALSUM (processus_transformare(argumenta));
        CREDO_FALSUM (processus_transformare(NIHIL));
    }


    /* XX. TERMINUS SINE FISTULIS - infans fistulas claudit sed
     * vivere pergit. Imago speculi XVII: illic fistulae apertae
     * infante mortuo, hic fistulae CLAUSAE infante VIVO.
     *
     * MENSURATUM 2026-07-30 ante emendationem: 'exec >&- 2>&-;
     * sleep 3' sub termino 500 ms moram TOTAM (3009 ms) vixit et
     * SUCCESSUS rediit - custodia termini in _ansam_pulsare habitat
     * quae fistulis clausis ante eam redit, et waitpid obstans
     * finalis terminum nullum habebat.
     * ============================================================ */

    {
        constans character* argumenta[IV];
         ProcessusResultus  r;

        imprimere("\n--- XX. terminus sine fistulis ---\n");
        argumenta[0]    = "/bin/sh";
        argumenta[I]    = "-c";
        argumenta[II]   = "exec >&- 2>&-; sleep 3";
        argumenta[III]  = NIHIL;
        r               = processus_exsequi(argumenta, 500, piscina);

        CREDO_FALSUM (r.successus);
        CREDO_VERUM (r.error == PROCESSUS_ERROR_TEMPUS);
        /* terminus rexit: nec ante eum finitum (>= 400) nec mora
         * infantis tota exspectata (< 2500) */
        CREDO_VERUM (r.mora_ms >= 400);
        CREDO_VERUM (r.mora_ms < 2500);
    }

    /* XXI. idem per semitam INCREMENTALEM - sine emendatione
     * pulsans CURRIT in aeternum acciperet (custodia termini
     * fistulis clausis numquam attingebatur) */
    {
        constans character* argumenta[IV];
                 Processus* p;
         ProcessusResultus  r;
                       i32  pulsus = ZEPHYRUM;

        imprimere("\n--- XXI. terminus sine fistulis,"
            " incrementalis ---\n");
        argumenta[0]    = "/bin/sh";
        argumenta[I]    = "-c";
        argumenta[II]   = "exec >&- 2>&-; sleep 3";
        argumenta[III]  = NIHIL;

        p = processus_incipere(argumenta, 500, piscina);
        CREDO_NON_NIHIL (p);
        dum (processus_pulsare(p) == PROCESSUS_CURRIT)
        {
            pulsus++;
            si (pulsus > 50000000)
            {
                frange;
            }
        }
        r = processus_metere(p);
        CREDO_FALSUM (r.successus);
        CREDO_VERUM (r.error == PROCESSUS_ERROR_TEMPUS);
        CREDO_VERUM (r.mora_ms < 2500);
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

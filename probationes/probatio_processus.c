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
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

/* I. exsecutio simplex: effusio capta, codex zerus */
interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* argumenta[IV];
         ProcessusResultus  r;

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
}

/* II. codex exitus non-zerus = SUCCESSUS (processus cucurrit
 * et respondit; successus significat generationem, non
 * felicitatem imperii) */
interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* argumenta[IV];
         ProcessusResultus  r;

        argumenta[0]    = "/bin/sh";
        argumenta[I]    = "-c";
        argumenta[II]   = "exit 5";
        argumenta[III]  = NIHIL;
        r               = processus_exsequi(argumenta, 5000, piscina);

        CREDO_VERUM (r.successus);
        CREDO_AEQUALIS_I32 (r.codex_exitus, V);
        CREDO_VERUM (r.error == PROCESSUS_OK);
    }
}

/* III. stdout et stderr SEPARATIM (quod system() nequit) */
interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* argumenta[IV];
         ProcessusResultus  r;
        constans character* ef;
        constans character* er;

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
}

/* IV. NULLA CONCHA: argumentum cum spatiis, virgulis, linea
 * nova INTACTUM transit. Hoc est ratio propter quam vector
 * adhibetur - sub /bin/sh haec omnia frangerentur aut
 * imperium alienum pararent. */
interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* argumenta[IV];
         ProcessusResultus  r;
        constans character* ef;

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
}

/* V. terminus temporis: processus occisus, error nominatus */
interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* argumenta[IV];
         ProcessusResultus  r;

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
}

/* VI. binarium absens: EXEC distinctum a codice 127.
 * MOMENTI PRO VILLA: ssh ipse 127 reddit cum imperium REMOTUM
 * deest - si eos confunderemus, "ssh non installatum" et
 * "systemctl in capsa remota abest" idem viderentur. */
interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* argumenta[III];
         ProcessusResultus  r;

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
}

/* VII. effusio maior quam frustum unum (sacculus crescens) */
interior vacuum
_sectio_septima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* argumenta[IV];
         ProcessusResultus  r;

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
}

/* VIII. argumenta invalida recusantur clare */
interior vacuum
_sectio_octava (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* argumenta[II];
         ProcessusResultus  r;

        argumenta[0]  = NIHIL;
        r             = processus_exsequi(argumenta, 5000, piscina);
        CREDO_FALSUM (r.successus);
        CREDO_VERUM (r.error == PROCESSUS_ERROR_ARGUMENTA);

        r = processus_exsequi(NIHIL, 5000, piscina);
        CREDO_FALSUM (r.successus);
        CREDO_VERUM (r.error == PROCESSUS_ERROR_ARGUMENTA);
    }
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
interior vacuum
_sectio_nona (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* argumenta[IV];
         ProcessusResultus  obstans;
         ProcessusResultus  incrementalis;
                 Processus* p;

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
interior vacuum
_sectio_decima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* argumenta[IV];
                 Processus* p;
         ProcessusResultus  r;
                       i32  pulsus = ZEPHYRUM;

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
}

/* XI. terminus custoditur in modo PULSANTE quoque - vocator
 * tardus processum aeternum parere non debet */
interior vacuum
_sectio_decima_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* argumenta[IV];
                 Processus* p;
         ProcessusResultus  r;
                       i32  pulsus = ZEPHYRUM;

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
}

/* XII. abrumpere: pyxis 'siste' faciei */
interior vacuum
_sectio_decima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* argumenta[IV];
                 Processus* p;
         ProcessusResultus  r;
                       i32  i;

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
}

/* XIII. manubrium FRACTUM: error ante generationem manubrium
 * VALIDUM reddit quod statim paratus est - vocator semitam
 * UNAM tractat, non duas */
interior vacuum
_sectio_decima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* argumenta[II];
                 Processus* p;
         ProcessusResultus  r;

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
}

/* XIV. metere sine pulsando (commoditas) + idempotentia */
interior vacuum
_sectio_decima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* argumenta[IV];
                 Processus* p;
         ProcessusResultus  prima;
         ProcessusResultus  secunda;

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
}

/* XV. effusio MAGNA per pulsus - sacculus per multos pulsus
 * crescit, nihil amittitur */
interior vacuum
_sectio_decima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* argumenta[IV];
                 Processus* p;
         ProcessusResultus  r;
                       i32  pulsus = ZEPHYRUM;

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
}

/* XVI. exec fallitum per semitam incrementalem - distinctio
 * EXEC/CXXVII quam villa postulat per hanc semitam quoque
 * servatur */
interior vacuum
_sectio_decima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* argumenta[III];
                 Processus* p;
         ProcessusResultus  r;

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

interior vacuum
_sectio_decima_septima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* argumenta[IV];
         ProcessusResultus  r;

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
}

/* XVIII. idem per semitam INCREMENTALEM */
interior vacuum
_sectio_decima_octava (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* argumenta[IV];
                 Processus* p;
         ProcessusResultus  r;
                       i32  pulsus = ZEPHYRUM;

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
}

/* XIX. transformare: SOLUM semitae defectus hic probantur -
 * exsecutio felix hunc ipsum processum substitueret (semita
 * felix in probatio_officina_renovatio integre agitur) */
interior vacuum
_sectio_decima_nona (
    CredoContextus* c)
{
    (vacuum)c;
    {
        constans character* argumenta[II];

        argumenta[0] = "/bin/nullum-tale-binarium-est";
        argumenta[I] = NIHIL;
        CREDO_FALSUM (processus_transformare(argumenta));

        argumenta[0] = NIHIL;
        CREDO_FALSUM (processus_transformare(argumenta));
        CREDO_FALSUM (processus_transformare(NIHIL));
    }
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

interior vacuum
_sectio_vicesima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* argumenta[IV];
         ProcessusResultus  r;

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
}

/* XXI. idem per semitam INCREMENTALEM - sine emendatione
 * pulsans CURRIT in aeternum acciperet (custodia termini
 * fistulis clausis numquam attingebatur) */
interior vacuum
_sectio_vicesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* argumenta[IV];
                 Processus* p;
         ProcessusResultus  r;
                       i32  pulsus = ZEPHYRUM;

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
}

/* XXII. OPTIONES (fabrica-6 T6): directorium infantis */
interior vacuum
_sectio_vicesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
         constans character* argumenta[II];
          ProcessusOptiones  optiones;
          ProcessusResultus  r;

        argumenta[0] = "/bin/pwd";
        argumenta[I] = NIHIL;
        optiones.directorium = "/usr/bin";
        optiones.ambitus = NIHIL;
        r = processus_exsequi_cum(argumenta, &optiones, 5000, piscina);
        CREDO_VERUM (r.successus);
        CREDO_VERUM (strcmp(_litterae_ex(piscina, r.effusio),
            "/usr/bin\n") == ZEPHYRUM);
    }
}

/* XXIII. ambitus EXACTUS: nihil hereditatur, parens intactus */
interior vacuum
_sectio_vicesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
         constans character* argumenta[II];
         constans character* ambitus[III];
          ProcessusOptiones  optiones;
          ProcessusResultus  r;

        argumenta[0] = "/usr/bin/env";
        argumenta[I] = NIHIL;
        ambitus[0] = "A=1";
        ambitus[I] = "PATH=/usr/bin:/bin";
        ambitus[II] = NIHIL;
        optiones.directorium = NIHIL;
        optiones.ambitus = ambitus;
        r = processus_exsequi_cum(argumenta, &optiones, 5000, piscina);
        CREDO_VERUM (r.successus);
        CREDO_VERUM (strcmp(_litterae_ex(piscina, r.effusio),
            "A=1\nPATH=/usr/bin:/bin\n") == ZEPHYRUM);
        CREDO_NON_NIHIL (getenv("PATH"));
        CREDO_NIHIL (getenv("A"));
    }
}

/* XXIV. quaestio PATH per ambitum datum */
interior vacuum
_sectio_vicesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
         constans character* argumenta[II];
         constans character* ambitus[II];
          ProcessusOptiones  optiones;
          ProcessusResultus  r;

        argumenta[0] = "env";
        argumenta[I] = NIHIL;
        ambitus[0] = "PATH=/usr/bin";
        ambitus[I] = NIHIL;
        optiones.directorium = NIHIL;
        optiones.ambitus = ambitus;
        r = processus_exsequi_cum(argumenta, &optiones, 5000, piscina);
        CREDO_VERUM (r.successus);
        CREDO_VERUM (strcmp(_litterae_ex(piscina, r.effusio),
            "PATH=/usr/bin\n") == ZEPHYRUM);
        ambitus[0] = "PATH=/nusquam_directorium";
        r = processus_exsequi_cum(argumenta, &optiones, 5000, piscina);
        CREDO_FALSUM (r.successus);
        CREDO_VERUM (r.error == PROCESSUS_ERROR_EXEC);
    }
}

/* XXV. directorium absens: error EXEC nominatus, nihil cucurrit */
interior vacuum
_sectio_vicesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
         constans character* argumenta[II];
          ProcessusOptiones  optiones;
          ProcessusResultus  r;

        argumenta[0] = "/bin/pwd";
        argumenta[I] = NIHIL;
        optiones.directorium = "/nusquam_directorium";
        optiones.ambitus = NIHIL;
        r = processus_exsequi_cum(argumenta, &optiones, 5000, piscina);
        CREDO_FALSUM (r.successus);
        CREDO_VERUM (r.error == PROCESSUS_ERROR_EXEC);
        CREDO_VERUM (strstr(_litterae_ex(piscina, r.error_descriptio),
            "No such file") != NIHIL);
        CREDO_AEQUALIS_I32 (r.effusio.mensura, ZEPHYRUM);
    }
}

/* XXVI. optiones NIHIL = processus_exsequi */
interior vacuum
_sectio_vicesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* argumenta[III];
         ProcessusResultus  r;

        argumenta[0] = "/bin/echo";
        argumenta[I] = "salve";
        argumenta[II] = NIHIL;
        r = processus_exsequi_cum(argumenta, NIHIL, 5000, piscina);
        CREDO_VERUM (r.successus);
        CREDO_VERUM (strcmp(_litterae_ex(piscina, r.effusio),
            "salve\n") == ZEPHYRUM);
    }
}

/* XXVII. aetas 'ps -o etime=' ([[dd-]hh:]mm:ss) - colloquia T8 */
interior vacuum
_sectio_vicesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
         s64 secundae;

        CREDO_VERUM (processus_aetatem_legere(chorda_ex_literis("05:07",
            piscina), &secundae));
        CREDO_AEQUALIS_S64 (secundae, (s64)CCCVII);
        CREDO_VERUM (processus_aetatem_legere(chorda_ex_literis(
            "1:02:03", piscina), &secundae));
        CREDO_AEQUALIS_S64 (secundae, (s64)MMMDCCXXIII);
        CREDO_VERUM (processus_aetatem_legere(chorda_ex_literis(
            "2-01:02:03", piscina), &secundae));
        /* II dies (LXXXVI CD s quisque) + 01:02:03 */
        CREDO_AEQUALIS_S64 (secundae, (s64)II * ((s64)LXXXVI * (s64)M
            + (s64)CD) + (s64)MMMDCCXXIII);
        CREDO_VERUM (processus_aetatem_legere(chorda_ex_literis("00:00",
            piscina), &secundae));
        CREDO_AEQUALIS_S64 (secundae, (s64)ZEPHYRUM);
        /* deformes */
        CREDO_FALSUM (processus_aetatem_legere(chorda_ex_literis("",
            piscina), &secundae));
        CREDO_FALSUM (processus_aetatem_legere(chorda_ex_literis("abc",
            piscina), &secundae));
        CREDO_FALSUM (processus_aetatem_legere(chorda_ex_literis("05",
            piscina), &secundae));
        CREDO_FALSUM (processus_aetatem_legere(chorda_ex_literis("05:",
            piscina), &secundae));
        CREDO_FALSUM (processus_aetatem_legere(chorda_ex_literis(":07",
            piscina), &secundae));
        CREDO_FALSUM (processus_aetatem_legere(chorda_ex_literis(
            "1:2:3:4", piscina), &secundae));
        CREDO_FALSUM (processus_aetatem_legere(chorda_ex_literis("5-",
            piscina), &secundae));
        CREDO_FALSUM (processus_aetatem_legere(chorda_ex_literis(
            "05:7x", piscina), &secundae));
        CREDO_FALSUM (processus_aetatem_legere(chorda_ex_literis(
            " 05:07", piscina), &secundae));
        /* dies sine horis: ps semper dd-hh:mm:ss */
        CREDO_FALSUM (processus_aetatem_legere(chorda_ex_literis(
            "2-01:02", piscina), &secundae));
    }
}

/* XXVIII. vivus: processus ipse (parens conchae) exstat et nuper
 * coepit; concha messa non exstat; pid nullus/negativus FALSUM */
interior vacuum
_sectio_vicesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* argumenta[IV];
         ProcessusResultus  r;
                       s32  ipse;
                       s32  mortuus;
                       s64  initium;
                       s64  nunc;

        argumenta[0]    = "/bin/sh";
        argumenta[I]    = "-c";
        argumenta[II]   = "echo $PPID";
        argumenta[III]  = NIHIL;
        r               = processus_exsequi(argumenta, 5000, piscina);
        CREDO_NECESSE (r.successus && r.codex_exitus == ZEPHYRUM);
        ipse           = (s32)atoi(_litterae_ex(piscina, r.effusio));
        argumenta[II]  = "echo $$";
        r              = processus_exsequi(argumenta, 5000, piscina);
        CREDO_NECESSE (r.successus && r.codex_exitus == ZEPHYRUM);
        mortuus        = (s32)atoi(_litterae_ex(piscina, r.effusio));
        CREDO_VERUM (ipse > ZEPHYRUM && mortuus > ZEPHYRUM);

        nunc     = (s64)time(NIHIL) * (s64)M;
        initium  = (s64)ZEPHYRUM;
        CREDO_VERUM (processus_vivus(ipse, &initium, piscina));
        /* probatio modo coepit: initium intra decem minuta ante nunc */
        CREDO_VERUM (initium >= nunc - (s64)DC * (s64)M);
        CREDO_VERUM (initium <= nunc + (s64)II * (s64)M);

        /* aetas vere subtrahitur: launchd (pid I) ab initio machinae */
        CREDO_VERUM (processus_vivus((s32)I, &initium, piscina));
        CREDO_VERUM (initium > (s64)ZEPHYRUM);
        CREDO_VERUM (initium < nunc - (s64)LX * (s64)M);

        initium = (s64)I;
        CREDO_FALSUM (processus_vivus(mortuus, &initium, piscina));
        CREDO_AEQUALIS_S64 (initium, (s64)ZEPHYRUM);
        CREDO_FALSUM (processus_vivus((s32)ZEPHYRUM, &initium,
            piscina));
        CREDO_FALSUM (processus_vivus(-(s32)I, &initium, piscina));
        CREDO_FALSUM (processus_vivus(ipse, &initium, NIHIL));
        /* initium_out NIHIL licet */
        CREDO_VERUM (processus_vivus(ipse, NIHIL, piscina));
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "I. exsecutio simplex", _sectio_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "II. codex exitus non-zerus", _sectio_secunda, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "III. effusio et erratum separata", _sectio_tertia, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "IV. argumenta sine concha", _sectio_quarta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "V. mora excessa", _sectio_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "VI. exec fallitum vs codex CXXVII", _sectio_sexta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "VII. effusio magna", _sectio_septima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "VIII. argumenta invalida", _sectio_octava, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "IX. aequivalentia semitarum", _sectio_nona, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "X. pulsus non obstat", _sectio_decima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "XI. terminus in modo pulsante", _sectio_decima_prima, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "XII. abrumpere", _sectio_decima_secunda, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "XIII. manubrium fractum", _sectio_decima_tertia, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "XIV. metere sine pulsu, bis", _sectio_decima_quarta, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "XV. effusio magna per pulsus", _sectio_decima_quinta, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "XVI. exec fallitum incrementaliter", _sectio_decima_sexta, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "XVII. nepos fistulam tenens", _sectio_decima_septima, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "XVIII. nepos, semita incrementalis", _sectio_decima_octava,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "XIX. transformare (defectus soli)", _sectio_decima_nona, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "XX. terminus sine fistulis", _sectio_vicesima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "XXI. terminus sine fistulis, incrementalis",
        _sectio_vicesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "XXII. directorium infantis", _sectio_vicesima_secunda, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "XXIII. ambitus exactus", _sectio_vicesima_tertia, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "XXIV. PATH ambitus dati", _sectio_vicesima_quarta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "XXV. directorium absens", _sectio_vicesima_quinta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "XXVI. optiones NIHIL", _sectio_vicesima_sexta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "XXVII. aetas ex etime", _sectio_vicesima_septima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "XXVIII. vivus", _sectio_vicesima_octava, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("processus", SECTIONES);
}

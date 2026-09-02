/* probatio_villa_agens.c - Porta agentis villae.
 *
 * OFFLINE integre: ssh VERUS numquam vocatur. VILLA_SSH ad
 * ssh_stipes.sh ponitur, quem agens per execvp vocat - ergo nulla
 * manipulatio PATH, et ssh verus obumbrari non potest.
 *
 * CUR HAEC PORTA IN probationes/ RADICIS VIVIT: apps/ a nullo
 * cursore verritur. apps/forum/fumus.sh id ipsum passum est -
 * porta quae viridis fieri non potest neglegitur, et instrumentum
 * tacite putrescit. Ergo dimidium SINE FACIE agentis huc migravit;
 * sola redditio WebView in fumus non-verso manet.
 */

#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "xar.h"
#include "villa_agens.h"
#include "villa_lectores.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "json.h"
#include "filum.h"

#define STIPES "probationes/fixa/villa/ssh_stipes.sh"

interior chorda*
_addere_chordam (
                   Xar* x,
               Piscina* p,
    constans character* s)
{
    chorda* e = (chorda*)xar_addere(x);

    si (e != NIHIL)
    {
        *e = chorda_ex_literis(s, p);
    }
    redde e;
}

/* probationem ad finem pulsare (porta contra ansam aeternam) */
interior i32
_ad_finem_pulsare (
    VillaAgens* a)
{
    i32 pulsus = ZEPHYRUM;

    dum (villa_probationes_currentes(a) > ZEPHYRUM)
    {
        (vacuum)villa_agens_pulsare(a);
        pulsus++;
        si (pulsus > 2000000)
        {
            frange;
        }
    }
    redde pulsus;
}

s32 principale (vacuum)
{
              Piscina* piscina;
    VillaConfiguratio  cfg;
                  b32  praeteritus;
               chorda  causa;

    piscina = piscina_generare_dynamicum("probatio_villa_agens",
        16777216);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ==============================================================
	 * I. CONFIGURATIO - sutura ambientis, directorium 0700
	 * ============================================================== */

    {
        imprimere("\n--- I. configuratio ---\n");
        causa = chorda_ex_literis("", piscina);

        setenv("VILLA_SSH", STIPES, I);
        CREDO_VERUM (villa_configurationem_parare(&cfg, piscina,
            &causa));
        /* SUTURA UNA: ambiens semel lectus, in configuratione
		 * habitans - semita probationis et semita vera eandem
		 * premunt */
        CREDO_NON_NIHIL (cfg.via_ssh);
        si (cfg.via_ssh != NIHIL)
        {
            CREDO_VERUM (strcmp(cfg.via_ssh, STIPES) == ZEPHYRUM);
        }
        CREDO_CHORDA_NON_VACUA (cfg.via_moderandi);
        CREDO_CHORDA_CONTINET (cfg.via_moderandi,
            chorda_ex_literis(".rhubarb/cm", piscina));
        /* terminus viae moderandi: expansa sub C octetis manere
		 * debet, alioquin ssh multiplicationem tacite deserit */
        CREDO_VERUM (cfg.via_moderandi.mensura + I
            + (i32)VILLA_MODERANDI_SIGILLUM
            <= (i32)VILLA_MODERANDI_TERMINUS);
    }


    /* ==============================================================
	 * II. CUSTOS NOMINUM - vector iniectionis quem concha REMOTA
	 * aperit (latus locale vectorem argumentorum habet, sed nomina
	 * unitatum in scriptum NOSTRUM interpolantur)
	 * ============================================================== */

    {
        imprimere("\n--- II. custos nominum unitatum ---\n");

        /* formae VERAE systemd */
        CREDO_VERUM (villa_unitas_valida(
            chorda_ex_literis("nginx.service", piscina)));
        CREDO_VERUM (villa_unitas_valida(
            chorda_ex_literis("getty@tty1.service", piscina)));
        CREDO_VERUM (villa_unitas_valida(
            chorda_ex_literis("systemd-udevd.service", piscina)));
        CREDO_VERUM (villa_unitas_valida(
            chorda_ex_literis("user@1000.service", piscina)));

        /* quidquid conchae significat RECUSATUR */
        CREDO_FALSUM (villa_unitas_valida(
            chorda_ex_literis("x; curl malum|sh", piscina)));
        CREDO_FALSUM (villa_unitas_valida(
            chorda_ex_literis("a b", piscina)));
        CREDO_FALSUM (villa_unitas_valida(
            chorda_ex_literis("$(rm -rf /)", piscina)));
        CREDO_FALSUM (villa_unitas_valida(
            chorda_ex_literis("`id`", piscina)));
        CREDO_FALSUM (villa_unitas_valida(
            chorda_ex_literis("a\nb", piscina)));
        CREDO_FALSUM (villa_unitas_valida(
            chorda_ex_literis("a&b", piscina)));
        CREDO_FALSUM (villa_unitas_valida(
            chorda_ex_literis("", piscina)));

        /* TERMINUS LONGITUDINIS ambobus lateribus (porta uno
		 * latere sola probata in aeternum uno errare potest):
		 * CCLV licitum, CCLVI recusatum - terminus systemd
		 * ipsius, et sine eo nomen longum capacitatem scripti
		 * (CXXVIII per unitatem) sprintf superscriberet */
        {
            chorda longum;

            longum.datum = (i8*)piscina_allocare(piscina,
                (memoriae_index)CCLVI);
            CREDO_NON_NIHIL (longum.datum);
            si (longum.datum != NIHIL)
            {
                memset(longum.datum, 'a', (memoriae_index)CCLVI);
                longum.mensura = (i32)CCLV;
                CREDO_VERUM (villa_unitas_valida(longum));
                longum.mensura = (i32)CCLVI;
                CREDO_FALSUM (villa_unitas_valida(longum));
            }
        }
    }


    /* ==============================================================
	 * III. SCRIPTUM PROBATIONIS - nomen pravum NUMQUAM intrat
	 * ============================================================== */

    {
           Xar* u       = xar_creare(piscina, (i32)magnitudo(chorda));
           i32  omissa  = ZEPHYRUM;
        chorda  s;

        imprimere("\n--- III. scriptum probationis ---\n");
        _addere_chordam(u, piscina, "nginx.service");
        _addere_chordam(u, piscina, "x; curl malum|sh");
        _addere_chordam(u, piscina, "getty@tty1.service");

        s = villa_scriptum_probationis(u, &omissa, piscina);

        /* OMISSIO NOMINATA, non tacita: vocator scire debet se
		 * minus probasse quam petivit */
        CREDO_AEQUALIS_I32 (omissa, (i32)I);
        CREDO_CHORDA_CONTINET (s,
            chorda_ex_literis("nginx.service", piscina));
        CREDO_CHORDA_CONTINET (s,
            chorda_ex_literis("getty@tty1.service", piscina));
        /* NOMEN PRAVUM ABEST - adsertio quae totum custodem portat.
		 *
		 * CAUTIO ADSERTIONIS: 'nullum ";" adest' adserere FALSUM
		 * est - scriptum ipsum semicola in definitione functionis
		 * suae fert ('s(){ ...; shift; ...; }'). Adsertio nimis
		 * lata custodem non probat sed generatorem accusat. Ergo
		 * nomen pravum IPSUM quaerimus, per partes suas. */
        CREDO_FALSUM (chorda_continet(s,
            chorda_ex_literis("curl malum", piscina)));
        CREDO_FALSUM (chorda_continet(s,
            chorda_ex_literis("x; curl", piscina)));
        CREDO_FALSUM (chorda_continet(s,
            chorda_ex_literis("|sh", piscina)));

        /* sectiones omnes emittuntur */
        CREDO_CHORDA_CONTINET (s,
            chorda_ex_literis("s unitates ", piscina));
        CREDO_CHORDA_CONTINET (s,
            chorda_ex_literis("s nginx ", piscina));
        CREDO_CHORDA_CONTINET (s,
            chorda_ex_literis("s memoria ", piscina));

        /* nulla unitas valida -> 'true', NON 'systemctl show' sine
		 * argumentis (quod OMNES unitates capsae redderet) */
        {
            Xar* solum_prava = xar_creare(piscina,
                (i32)magnitudo(chorda));
            chorda s2;

            _addere_chordam(solum_prava, piscina, "a;b");
            s2 = villa_scriptum_probationis(solum_prava, &omissa,
                piscina);
            CREDO_AEQUALIS_I32 (omissa, (i32)I);
            CREDO_CHORDA_CONTINET (s2,
                chorda_ex_literis("s servitia true", piscina));
        }
    }


    /* ==============================================================
	 * IV. PROBATIO PLENA per stipitem - catena tota
	 * ============================================================== */

    {
        VillaAgens* a = villa_agens_creare(&cfg, NIHIL, piscina);
        Xar*        u = xar_creare(piscina, (i32)magnitudo(chorda));
        constans ProbatioServi* p;

        imprimere("\n--- IV. probatio plena ---\n");
        CREDO_NON_NIHIL (a);
        _addere_chordam(u, piscina, "nginx.service");

        setenv("VILLA_STIPES_MODUS", "bene", I);
        CREDO_VERUM (villa_probationem_incipere(a,
            chorda_ex_literis("s1", piscina),
            chorda_ex_literis("gemma", piscina), u, &causa));
        /* NON OBSTAT: probatio in cursu est statim post initium */
        CREDO_AEQUALIS_I32 (villa_probationes_currentes(a), (i32)I);
        CREDO_VERUM (_ad_finem_pulsare(a) > ZEPHYRUM);

        p = villa_probatio_ultima(a, chorda_ex_literis("s1",
            piscina));
        CREDO_NON_NIHIL (p);
        si (p != NIHIL)
        {
            CREDO_VERUM (p->perfecta);
            CREDO_VERUM (p->felix);
            /* nulla querela = omnes sectiones advenerunt */
            CREDO_CHORDA_VACUA (p->querelae);

            /* lectiones parsatae, non solum receptae */
            CREDO_NON_NIHIL (p->unitates);
            CREDO_AEQUALIS_I32 (xar_numerus(p->unitates), (i32)XXV);
            CREDO_NON_NIHIL (p->servitia);
            CREDO_AEQUALIS_I32 (xar_numerus(p->servitia), (i32)III);
            CREDO_NON_NIHIL (p->situs);
            CREDO_AEQUALIS_I32 (xar_numerus(p->situs), (i32)IV);
            CREDO_CHORDA_AEQUALIS_LITERIS (p->discus.systema,
                "/dev/vda1");
            CREDO_VERUM (p->memoria.summa_kb > ZEPHYRUM);
            CREDO_VERUM (p->tempus_activum > ZEPHYRUM);
        }
    }


    /* ==============================================================
	 * V. DEFECTUS PARTIALIS - sectio una cadit, ceterae valent.
	 * Probatio partim felix TACITE dimidiata pessimum genus est.
	 * ============================================================== */

    {
        VillaAgens* a = villa_agens_creare(&cfg, NIHIL, piscina);
        Xar*        u = xar_creare(piscina, (i32)magnitudo(chorda));
        constans ProbatioServi* p;

        imprimere("\n--- V. defectus partialis ---\n");
        _addere_chordam(u, piscina, "nginx.service");
        setenv("VILLA_STIPES_MODUS", "nginx_fractum", I);
        CREDO_VERUM (villa_probationem_incipere(a,
            chorda_ex_literis("s1", piscina),
            chorda_ex_literis("gemma", piscina), u, &causa));
        (vacuum)_ad_finem_pulsare(a);

        p = villa_probatio_ultima(a, chorda_ex_literis("s1",
            piscina));
        CREDO_NON_NIHIL (p);
        si (p != NIHIL)
        {
            /* ssh ipse SUCCESSIT (codex 0) - defectus intra
			 * sectionem est, quod aliud est */
            CREDO_VERUM (p->felix);
            /* sed querela nginx NOMINATUR */
            CREDO_CHORDA_CONTINET (p->querelae,
                chorda_ex_literis("nginx", piscina));
            CREDO_NIHIL (p->situs);
            /* ceterae sectiones INTACTAE - defectus unius alias
			 * non corrumpit */
            CREDO_NON_NIHIL (p->unitates);
            CREDO_CHORDA_AEQUALIS_LITERIS (p->discus.systema,
                "/dev/vda1");
        }
    }

    /* VI. sectio ABSENS + effusio ABSCISSA */
    {
        VillaAgens* a = villa_agens_creare(&cfg, NIHIL, piscina);
        Xar*        u = xar_creare(piscina, (i32)magnitudo(chorda));
        constans ProbatioServi* p;

        imprimere("\n--- VI. sectio absens, effusio abscissa ---\n");
        _addere_chordam(u, piscina, "nginx.service");

        setenv("VILLA_STIPES_MODUS", "sectio_absens", I);
        CREDO_VERUM (villa_probationem_incipere(a,
            chorda_ex_literis("s1", piscina),
            chorda_ex_literis("gemma", piscina), u, &causa));
        (vacuum)_ad_finem_pulsare(a);
        p = villa_probatio_ultima(a, chorda_ex_literis("s1",
            piscina));
        CREDO_NON_NIHIL (p);
        si (p != NIHIL)
        {
            CREDO_CHORDA_CONTINET (p->querelae,
                chorda_ex_literis("memoria", piscina));
            CREDO_NON_NIHIL (p->unitates);
        }

        setenv("VILLA_STIPES_MODUS", "abscissum", I);
        CREDO_VERUM (villa_probationem_incipere(a,
            chorda_ex_literis("s2", piscina),
            chorda_ex_literis("gemma", piscina), u, &causa));
        (vacuum)_ad_finem_pulsare(a);
        p = villa_probatio_ultima(a, chorda_ex_literis("s2",
            piscina));
        CREDO_NON_NIHIL (p);
        si (p != NIHIL)
        {
            /* sectio non clausa: contentum eius NON FIDENDUM,
			 * ergo querela */
            CREDO_CHORDA_NON_VACUA (p->querelae);
        }
    }

    /* VII. ssh IPSE cadit - causa NOMINATA, non 'defecit' nudum */
    {
        VillaAgens* a = villa_agens_creare(&cfg, NIHIL, piscina);
        Xar*        u = xar_creare(piscina, (i32)magnitudo(chorda));
        constans ProbatioServi* p;

        imprimere("\n--- VII. ssh ipse cadit ---\n");
        _addere_chordam(u, piscina, "nginx.service");

        setenv("VILLA_STIPES_MODUS", "hospes_ignotus", I);
        CREDO_VERUM (villa_probationem_incipere(a,
            chorda_ex_literis("s1", piscina),
            chorda_ex_literis("gemma", piscina), u, &causa));
        (vacuum)_ad_finem_pulsare(a);
        p = villa_probatio_ultima(a, chorda_ex_literis("s1",
            piscina));
        CREDO_NON_NIHIL (p);
        si (p != NIHIL)
        {
            CREDO_VERUM (p->perfecta);
            CREDO_FALSUM (p->felix);
            CREDO_VERUM (p->causa.genus
                == VILLA_EXITUS_HOSPES_IGNOTUS);
            CREDO_VERUM (p->causa.ssh_ipse);
        }

        setenv("VILLA_STIPES_MODUS", "permissio", I);
        CREDO_VERUM (villa_probationem_incipere(a,
            chorda_ex_literis("s2", piscina),
            chorda_ex_literis("gemma", piscina), u, &causa));
        (vacuum)_ad_finem_pulsare(a);
        p = villa_probatio_ultima(a, chorda_ex_literis("s2",
            piscina));
        CREDO_NON_NIHIL (p);
        si (p != NIHIL)
        {
            CREDO_VERUM (p->causa.genus == VILLA_EXITUS_PERMISSIO);
        }
    }

    /* VIII. ARENAE GEMINAE: photographia ultima VIVA manet dum
	 * probatio nova currit. Sine hoc facies id quod eodem tictu
	 * legit deleret - insectum quod solum sub onere appareret. */
    {
        VillaAgens* a = villa_agens_creare(&cfg, NIHIL, piscina);
        Xar*        u = xar_creare(piscina, (i32)magnitudo(chorda));
        constans ProbatioServi* prior;
        chorda      clavis = chorda_ex_literis("s1", piscina);

        imprimere("\n--- VIII. arenae geminae ---\n");
        _addere_chordam(u, piscina, "nginx.service");
        setenv("VILLA_STIPES_MODUS", "bene", I);

        CREDO_VERUM (villa_probationem_incipere(a, clavis,
            chorda_ex_literis("gemma", piscina), u, &causa));
        (vacuum)_ad_finem_pulsare(a);
        prior = villa_probatio_ultima(a, clavis);
        CREDO_NON_NIHIL (prior);

        /* probatio SECUNDA incipit: prior adhuc legibilis esse
		 * debet dum haec currit */
        CREDO_VERUM (villa_probationem_incipere(a, clavis,
            chorda_ex_literis("gemma", piscina), u, &causa));
        si (prior != NIHIL)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (prior->discus.systema,
                "/dev/vda1");
            CREDO_NON_NIHIL (prior->unitates);
            CREDO_AEQUALIS_I32 (xar_numerus(prior->unitates),
                (i32)XXV);
        }
        (vacuum)_ad_finem_pulsare(a);

        /* probatio dupla eiusdem servi RECUSATUR dum currit */
        CREDO_VERUM (villa_probationem_incipere(a, clavis,
            chorda_ex_literis("gemma", piscina), u, &causa));
        CREDO_FALSUM (villa_probationem_incipere(a, clavis,
            chorda_ex_literis("gemma", piscina), u, &causa));
        CREDO_CHORDA_CONTINET (causa,
            chorda_ex_literis("iam currit", piscina));
        (vacuum)_ad_finem_pulsare(a);
    }

    /* IX. ACTIO - imperium curritur, causa discernitur.
	 * (Scriptura eventus sine daemone probari non potest; hic
	 * cliens NIHIL est, ergo semita actionis sola custoditur -
	 * eventus in porta vivi V6 custodietur.) */
    {
        VillaAgens* a = villa_agens_creare(&cfg, NIHIL,
            piscina);
        ResultusActionis r;

        imprimere("\n--- IX. actio ---\n");
        setenv("VILLA_STIPES_MODUS", "imperium", I);
        r = villa_actionem_agere(a,
            chorda_ex_literis("gemma", piscina),
            chorda_ex_literis("", piscina),
            chorda_ex_literis("systemctl restart nginx", piscina),
            piscina);
        CREDO_VERUM (r.successus);
        CREDO_VERUM (r.causa.genus == VILLA_EXITUS_SUCCESSUS);
        CREDO_CHORDA_CONTINET (r.effusio,
            chorda_ex_literis("servitium restitutum", piscina));
        /* cliens NIHIL -> nullus eventus, sed actio tamen currit */
        CREDO_FALSUM (r.eventus_scriptus);

        setenv("VILLA_STIPES_MODUS", "imperium_malum", I);
        r = villa_actionem_agere(a,
            chorda_ex_literis("gemma", piscina),
            chorda_ex_literis("", piscina),
            chorda_ex_literis("systemctl restart nulla", piscina),
            piscina);
        CREDO_FALSUM (r.successus);
        CREDO_VERUM (r.causa.genus == VILLA_EXITUS_IMPERIUM_FRACTUM);
        CREDO_AEQUALIS_I32 (r.causa.codex, (i32)III);
        CREDO_CHORDA_CONTINET (r.erratum,
            chorda_ex_literis("Unit not found", piscina));
    }


    /* ==============================================================
	 * X. EVENTUS - actio in ANNALES scribitur
	 *
	 * Daemon EPHEMERUS (-portus 0, scrinium/annales in build/):
	 * tabularium VERUM Franis numquam tangitur. Haec est adsertio
	 * quam specificatio §IX.2 nominatim petivit, et sine ea
	 * 'actiones eventus pariunt' spes esset, non factum.
	 * ============================================================== */

    {
        character imperium[1024];
          integer portus_int = ZEPHYRUM;
              i32 k;

        imprimere("\n--- X. eventus in annalibus ---\n");
        /* glob, non nomina tria: sqlite '-wal' et '-shm' comites
		 * relinquit quos lista explicita praeterit (id ipsum hic
		 * accidit - porta sordes suas in build/ reliquit) */
        (vacuum)system("rm -f build/probatio_villa.*");
        CREDO_VERUM (system("./gesta/tabulariumd.sh -struere"
            " > /dev/null 2>&1") == ZEPHYRUM);
        CREDO_VERUM (system("gesta/build/tabulariumd -portus 0"
            " -scrinium build/probatio_villa.db"
            " -annales build/probatio_villa.jsonl"
            " > build/probatio_villa.portus 2> /dev/null &")
            == ZEPHYRUM);
        per (k = ZEPHYRUM; k < XXV; k++)
        {
            FILE* pf = fopen("build/probatio_villa.portus", "r");

            si (pf != NIHIL)
            {
                si (   fscanf(pf, "portus %d", &portus_int) == I
                    && portus_int > 0)
                {
                    fclose(pf);
                    frange;
                }
                fclose(pf);
            }
            (vacuum)system("sleep 0.2");
        }
        CREDO_VERUM (portus_int > 0);

        si (portus_int > 0)
        {
             ClientTabularii  cliens;
                  VillaAgens* a;
            ResultusActionis  r;
                      chorda  culpa = chorda_ex_literis("",
                          piscina);
                      chorda  res_id;
                   JsonValor* args;
                      chorda  resp;

            cliens_tabularii_incipere(&cliens, (i32)portus_int);
            cliens.praefixum  = "[probatio]";
            cliens.actor      = "villa";
            /* launcher VACUUS: daemon iam currit, et start-if-absent
			 * hic daemonem portus ALTERIUS excitaret */
            cliens.launcher  = NIHIL;
            cliens.genitus   = VERUM;

            a = villa_agens_creare(&cfg, &cliens, piscina);
            CREDO_NON_NIHIL (a);

            /* rem servi creare ut eventum ferat */
            args = json_objectum_creare(piscina);
            json_objectum_ponere(args, "genus",
                json_chorda_creare(piscina,
                    chorda_ex_literis("nota", piscina)));
            json_objectum_ponere(args, "titulus",
                json_chorda_creare(piscina,
                    chorda_ex_literis("Servus probationis", piscina)));
            resp = cliens_tabularii_vocare(&cliens, piscina,
                "addere", args, &culpa);
            res_id = cliens_tabularii_res_id(resp, piscina);
            CREDO_CHORDA_NON_VACUA (res_id);

            setenv("VILLA_STIPES_MODUS", "imperium", I);
            r = villa_actionem_agere(a,
                chorda_ex_literis("gemma", piscina), res_id,
                chorda_ex_literis("systemctl restart nginx",
                    piscina), piscina);
            CREDO_VERUM (r.successus);
            CREDO_VERUM (r.eventus_scriptus);

            /* EVENTUS VERE ADEST: rem relegimus et notam quaerimus */
            {
                JsonValor* q = json_objectum_creare(piscina);
                   chorda  lectio;

                json_objectum_ponere(q, "res",
                    json_chorda_creare(piscina, res_id));
                lectio = cliens_tabularii_vocare(&cliens, piscina,
                    "res", q, &culpa);
                CREDO_CHORDA_CONTINET (lectio,
                    chorda_ex_literis("systemctl restart nginx",
                        piscina));

                /* ORIGO in ANNALIBUS quaerenda, non in effusione
				 * 'res': origo columna PROPRIA tessellae est, non
				 * pars dati, et compendium 'res' eam non reddit.
				 * Adsertio contra annales praestat quia id probat
				 * quod vere durat - eventum in actis DURABILIBUS
				 * cum provenientia recta, non textum in summario. */
                {
                    chorda annales = filum_legere_totum(
                        "build/probatio_villa.jsonl", piscina);

                    CREDO_VERUM (annales.mensura > ZEPHYRUM);
                    CREDO_CHORDA_CONTINET (annales,
                        chorda_ex_literis("\"origo\":\"villa\"",
                            piscina));
                    CREDO_CHORDA_CONTINET (annales,
                        chorda_ex_literis(
                            "\"genus_eventus\":\"nota\"", piscina));
                    /* NUMQUAM 'mutatio': ea contra campos generis
					 * iudicaretur et notam custodiae in OMNI
					 * restitutione pareret */
                    CREDO_FALSUM (chorda_continet(annales,
                        chorda_ex_literis(
                            "\"genus_eventus\":\"mutatio\"",
                            piscina)));
                }
            }

            /* actio FRACTA quoque eventum parit - defectus
			 * silentium non est */
            setenv("VILLA_STIPES_MODUS", "imperium_malum", I);
            r = villa_actionem_agere(a,
                chorda_ex_literis("gemma", piscina), res_id,
                chorda_ex_literis("systemctl restart nulla",
                    piscina), piscina);
            CREDO_FALSUM (r.successus);
            CREDO_VERUM (r.eventus_scriptus);
            {
                JsonValor* q = json_objectum_creare(piscina);
                   chorda  lectio;

                json_objectum_ponere(q, "res",
                    json_chorda_creare(piscina, res_id));
                lectio = cliens_tabularii_vocare(&cliens, piscina,
                    "res", q, &culpa);
                CREDO_CHORDA_CONTINET (lectio,
                    chorda_ex_literis("imperium fractum", piscina));
            }
        }
        sprintf(imperium, "pkill -f 'tabulariumd -portus 0"
            " -scrinium build/probatio_villa.db' > /dev/null 2>&1");
        (vacuum)system(imperium);
        (vacuum)system("rm -f build/probatio_villa.*");
    }


    /* ==============================================================
	 * XI. IUDICIUM PUNCTI - regulae tripartitae (decisae 2026-07-30)
	 * ============================================================== */

    {
        ProbatioServi p;

        imprimere("\n--- XI. iudicium puncti ---\n");

        CREDO_VERUM (villa_iudicare(NIHIL) == VILLA_IUDICIUM_IGNOTUS);

        memset(&p, 0, magnitudo(ProbatioServi));
        CREDO_VERUM (villa_iudicare(&p) == VILLA_IUDICIUM_IGNOTUS);

        p.perfecta  = VERUM;
        p.felix     = FALSUM;
        CREDO_VERUM (villa_iudicare(&p) == VILLA_IUDICIUM_FRACTUS);

        p.felix = VERUM;
        CREDO_VERUM (villa_iudicare(&p) == VILLA_IUDICIUM_SANUS);

        /* querelae -> flavum (sectio lapsa, non servus mortuus) */
        p.querelae = chorda_ex_literis("memoria", piscina);
        CREDO_VERUM (villa_iudicare(&p) == VILLA_IUDICIUM_LANGUIDUS);
        p.querelae = chorda_ex_literis("", piscina);

        /* discus: terminus XC ambobus lateribus */
        p.discus.capacitas = 89;
        CREDO_VERUM (villa_iudicare(&p) == VILLA_IUDICIUM_SANUS);
        p.discus.capacitas = (i32)XC;
        CREDO_VERUM (villa_iudicare(&p) == VILLA_IUDICIUM_LANGUIDUS);
        p.discus.capacitas = ZEPHYRUM;

        /* servitium observatum non-sanum = RUBRUM, etiam probatione
		 * felici - tres viae (fracta / non currit / non inventa) */
        {
            StatusServitii* s;

            p.servitia = xar_creare(piscina,
                (i32)magnitudo(StatusServitii));
            s = (StatusServitii*)xar_addere(p.servitia);
            CREDO_NON_NIHIL (s);
            si (s != NIHIL)
            {
                memset(s, 0, magnitudo(StatusServitii));
                s->inventa  = VERUM;
                s->currit   = VERUM;
                CREDO_VERUM (villa_iudicare(&p)
                    == VILLA_IUDICIUM_SANUS);

                s->fracta = VERUM;
                CREDO_VERUM (villa_iudicare(&p)
                    == VILLA_IUDICIUM_FRACTUS);
                s->fracta = FALSUM;

                s->currit = FALSUM;
                CREDO_VERUM (villa_iudicare(&p)
                    == VILLA_IUDICIUM_FRACTUS);
                s->currit = VERUM;

                s->inventa = FALSUM;
                CREDO_VERUM (villa_iudicare(&p)
                    == VILLA_IUDICIUM_FRACTUS);
                s->inventa = VERUM;

                /* rubrum vincit flavum */
                s->fracta   = VERUM;
                p.querelae  = chorda_ex_literis("nginx", piscina);
                CREDO_VERUM (villa_iudicare(&p)
                    == VILLA_IUDICIUM_FRACTUS);
            }
        }

        CREDO_VERUM (strcmp(villa_iudicium_nomen(
            VILLA_IUDICIUM_FRACTUS), "fractus") == ZEPHYRUM);
    }


    /* ==============================================================
	 * XII. ACTIO INCREMENTALIS - sedes altera, arena gemina.
	 * Semita faciei: incipere statim redit, pulsus perficit,
	 * fructus ultimus vivus manet dum actio nova currit.
	 * ============================================================== */

    {
        VillaAgens* a = villa_agens_creare(&cfg, NIHIL,
            piscina);
        chorda      clavis     = chorda_ex_literis("s1", piscina);
        chorda      gemma      = chorda_ex_literis("gemma", piscina);
        chorda      vacua_res  = chorda_ex_literis("", piscina);
        constans ResultusActionis* r;
        constans ResultusActionis* prior;
        i32         gradus;

        imprimere("\n--- XII. actio incrementalis ---\n");
        CREDO_NON_NIHIL (a);

        CREDO_FALSUM (villa_actio_currit(a, clavis));
        CREDO_NIHIL (villa_actio_ultima(a, clavis));

        setenv("VILLA_STIPES_MODUS", "imperium", I);
        CREDO_VERUM (villa_actionem_incipere(a, clavis, gemma,
            vacua_res, chorda_ex_literis("systemctl restart nginx",
                piscina), &causa));
        /* NON OBSTAT: actio statim in cursu */
        CREDO_VERUM (villa_actio_currit(a, clavis));

        /* actio dupla eiusdem servi RECUSATUR dum currit */
        CREDO_FALSUM (villa_actionem_incipere(a, clavis, gemma,
            vacua_res, chorda_ex_literis("uptime", piscina),
            &causa));
        CREDO_CHORDA_CONTINET (causa,
            chorda_ex_literis("iam currit", piscina));

        gradus = ZEPHYRUM;
        dum (villa_actio_currit(a, clavis) && gradus < 2000000)
        {
            (vacuum)villa_agens_pulsare(a);
            gradus++;
        }
        CREDO_FALSUM (villa_actio_currit(a, clavis));

        r = villa_actio_ultima(a, clavis);
        CREDO_NON_NIHIL (r);
        si (r != NIHIL)
        {
            CREDO_VERUM (r->successus);
            CREDO_VERUM (r->causa.genus == VILLA_EXITUS_SUCCESSUS);
            CREDO_CHORDA_CONTINET (r->effusio,
                chorda_ex_literis("servitium restitutum", piscina));
            /* cliens NIHIL -> nullus eventus */
            CREDO_FALSUM (r->eventus_scriptus);
        }

        /* ARENA GEMINA: fructus prior legibilis manet dum actio
		 * nova currit (idem quod §VIII probationibus probat) */
        prior = r;
        setenv("VILLA_STIPES_MODUS", "imperium_malum", I);
        CREDO_VERUM (villa_actionem_incipere(a, clavis, gemma,
            vacua_res, chorda_ex_literis("systemctl restart nulla",
                piscina), &causa));
        si (prior != NIHIL)
        {
            CREDO_CHORDA_CONTINET (prior->effusio,
                chorda_ex_literis("servitium restitutum", piscina));
        }
        gradus = ZEPHYRUM;
        dum (villa_actio_currit(a, clavis) && gradus < 2000000)
        {
            (vacuum)villa_agens_pulsare(a);
            gradus++;
        }
        r = villa_actio_ultima(a, clavis);
        CREDO_NON_NIHIL (r);
        si (r != NIHIL)
        {
            CREDO_FALSUM (r->successus);
            CREDO_VERUM (r->causa.genus
                == VILLA_EXITUS_IMPERIUM_FRACTUM);
        }

        /* abrumpere: actio finitur, fructus adest; iterum vocata
		 * sine actione currente nihil agit */
        setenv("VILLA_STIPES_MODUS", "imperium", I);
        CREDO_VERUM (villa_actionem_incipere(a, clavis, gemma,
            vacua_res, chorda_ex_literis("uptime", piscina),
            &causa));
        villa_actionem_abrumpere(a, clavis);
        CREDO_FALSUM (villa_actio_currit(a, clavis));
        CREDO_NON_NIHIL (villa_actio_ultima(a, clavis));
        villa_actionem_abrumpere(a, clavis);

        /* probatio et actio SIMUL in eadem sede - arenae separatae,
		 * pulsus unus ambas provehit */
        {
            Xar* u = xar_creare(piscina, (i32)magnitudo(chorda));
            constans ProbatioServi* pr;

            _addere_chordam(u, piscina, "nginx.service");
            setenv("VILLA_STIPES_MODUS", "bene", I);
            CREDO_VERUM (villa_probationem_incipere(a, clavis,
                gemma, u, &causa));
            setenv("VILLA_STIPES_MODUS", "imperium", I);
            CREDO_VERUM (villa_actionem_incipere(a, clavis, gemma,
                vacua_res, chorda_ex_literis("uptime", piscina),
                &causa));
            gradus = ZEPHYRUM;
            dum (   (villa_probationes_currentes(a) > ZEPHYRUM
                    || villa_actio_currit(a, clavis))
                 && gradus < 2000000)
            {
                (vacuum)villa_agens_pulsare(a);
                gradus++;
            }
            pr = villa_probatio_ultima(a, clavis);
            CREDO_NON_NIHIL (pr);
            si (pr != NIHIL)
            {
                CREDO_VERUM (pr->felix);
            }
            r = villa_actio_ultima(a, clavis);
            CREDO_NON_NIHIL (r);
            si (r != NIHIL)
            {
                CREDO_VERUM (r->successus);
            }
        }

        /* enumeratio sediumm (fundamentum villa_status faciei) */
        CREDO_AEQUALIS_I32 (villa_sedes_numerus(a), (i32)I);
        CREDO_CHORDA_AEQUALIS_LITERIS (villa_sedes_clavis(a,
            ZEPHYRUM), "s1");
        CREDO_AEQUALIS_I32 (villa_sedes_clavis(a, (i32)V).mensura,
            ZEPHYRUM);
        CREDO_FALSUM (villa_probatio_currit(a, clavis));
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

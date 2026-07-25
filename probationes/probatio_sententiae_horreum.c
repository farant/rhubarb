/* ====================================================================
 * PROBATIO HORREI SENTENTIARUM (S2)
 *
 * Lector purus in probatio_sententiae.c custoditur. HAEC porta aliud
 * probat: quod textus per CONDITORIUM iter faciens IDEM manet.
 *
 * Quare id refert: sigillum super textum computatur. Si vel unus
 * octetus per codificationem JSON, per SQLite, per filum mutatur,
 * OMNE sigillum mutatur et OMNIS ancora frangitur. Experimentum in
 * conditorio scaenico id NON probavit - solum quod insertio non
 * erravit.
 *
 * ------------------------------------------------------------------
 * CAVE: SECTIONES HIC INDEPENDENTES NON SUNT
 *
 * Omnes DAEMONEM UNUM communem habent, ergo quaelibet sectio quae
 * conditorium mutat sectiones POSTERIORES afficit. Cellae uncinatae
 * independentes VIDENTUR et non sunt.
 *
 * Instantia concreta: sectio V genus CONSULTO collidit, quod registrum
 * rapit, quod iudicium camporum EXSTINGUIT. Ergo sectio IV (quae
 * iudicium vivere probat) ANTE sectionem V currere DEBET, et sectio VI
 * definitionem restituit ne quid postea addatur in terram mortuam.
 *
 * Ordine inverso sectio IV TACITE frangitur - quod primo cursu huius
 * portae accidit, et symptoma DUABUS sectionibus a causa distabat.
 *
 * SECTIONEM NOVAM ADDENS: pone quid conditorium iam passum sit eo loco
 * quo eam inseris. Si genus, campos, aut definitiones tangit, sedem
 * eius contra sectiones IV-VI examina - non ad finem adice sperans.
 * ------------------------------------------------------------------
 *
 * Daemon EPHEMERUS (-portus 0, scrinium et annales in build/):
 * tabularium VERUM Franis numquam tangitur.
 * ==================================================================== */

#include "latina.h"
#include "credo.h"
#include "sententiae.h"
#include "cliens_tabularii.h"
#include "filum.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include "json.h"
#include "sigillum.h"

#include <stdio.h>
#include <stdlib.h>   /* system */
#include <string.h>

#define FX_LIBER "probationes/fixa/sententiae/de_imagine.md"

interior chorda
_ch (Piscina* piscina, constans character* litterae)
{
    redde chorda_ex_literis(litterae, piscina);
}

interior JsonValor*
_campus (Piscina* piscina, constans character* clavis,
         constans character* typus)
{
    JsonValor* c = json_objectum_creare(piscina);

    json_objectum_ponere(c, "clavis", json_chorda_creare(piscina,
        _ch(piscina, clavis)));
    json_objectum_ponere(c, "typus", json_chorda_creare(piscina,
        _ch(piscina, typus)));
    redde c;
}

/* Definitionem generis 'liber' mittere. cum_campis=FALSUM definitionem
 * VENENATAM parit - eandem clavem sine campis, qua collisio probatur. */
interior chorda
_liber_definire (ClientTabularii* cliens, Piscina* piscina,
                 constans character* titulus, b32 cum_campis)
{
    JsonValor* def   = json_objectum_creare(piscina);
    JsonValor* args  = json_objectum_creare(piscina);
    chorda     culpa = chorda_ex_literis("", piscina);

    json_objectum_ponere(def, "clavis", json_chorda_creare(piscina,
        _ch(piscina, "liber")));
    json_objectum_ponere(def, "titulus_monstrans",
        json_chorda_creare(piscina, _ch(piscina, titulus)));

    si (cum_campis)
    {
        JsonValor* campi = json_tabulatum_creare(piscina);

        json_tabulatum_addere(campi, _campus(piscina, "siglum", "textus"));
        json_tabulatum_addere(campi, _campus(piscina, "slug", "textus"));
        /* 'status_libri', NON 'status': 'status' clavis RESERVATA est -
         * iudicium camporum eam praeterit ET columnam vitae proicit,
         * ergo campus ita nominatus taciturnus et fallax simul esset */
        json_tabulatum_addere(campi, _campus(piscina, "status_libri",
            "textus"));
        json_tabulatum_addere(campi, _campus(piscina, "fons", "area"));
        json_objectum_ponere(def, "campi", campi);
    }

    json_objectum_ponere(args, "genus", json_chorda_creare(piscina,
        _ch(piscina, "definitio")));
    json_objectum_ponere(args, "titulus", json_chorda_creare(piscina,
        _ch(piscina, titulus)));
    /* datum CHORDA esse debet quae JSON continet, non obiectum */
    json_objectum_ponere(args, "datum", json_chorda_creare(piscina,
        json_scribere(def, piscina)));

    redde cliens_tabularii_vocare(cliens, piscina, "addere", args, &culpa);
}

/* CUSTOS COLLISIONIS - tabulam generum LEGIT.
 *
 * Nota bene quid hic NON fiat: responsum 'addere' non inspicitur.
 * Violatio 'clavis generis iam occupata' in NOTIS entis sepelitur,
 * non in responso - ergo custos qui responsum inspicit collisionem
 * numquam videt. Sectio V id probat. */
interior b32
_genus_adest (ClientTabularii* cliens, Piscina* piscina,
              constans character* clavis)
{
    chorda     culpa = chorda_ex_literis("", piscina);
    JsonValor* entia;
    i32        k;

    entia = cliens_tabularii_legere(cliens, piscina,
        _ch(piscina, "definitio"), (i32)C, &culpa);
    si (entia == NIHIL || !json_est_tabulatum(entia)) { redde FALSUM; }

    per (k = ZEPHYRUM; k < json_tabulatum_numerus(entia); k++)
    {
        JsonValor* e = json_tabulatum_obtinere(entia, (i32)k);
        JsonValor* d;
        JsonValor* c;

        si (e == NIHIL) { perge; }
        d = json_objectum_capere(e, "datum");
        si (d == NIHIL) { perge; }
        c = json_objectum_capere(d, "clavis");
        si (c == NIHIL || !json_est_chorda(c)) { perge; }
        si (chorda_aequalis_literis(json_ad_chorda(c), clavis))
        { redde VERUM; }
    }
    redde FALSUM;
}

/* Campum mutare, deinde rem BREVITER relegere. */
interior chorda
_mutare_et_legere (ClientTabularii* cliens, Piscina* piscina, chorda res_id,
                   constans character* clavis, constans character* valor)
{
    JsonValor* args  = json_objectum_creare(piscina);
    JsonValor* q     = json_objectum_creare(piscina);
    chorda     culpa = chorda_ex_literis("", piscina);

    json_objectum_ponere(args, "res", json_chorda_creare(piscina, res_id));
    json_objectum_ponere(args, "actus", json_chorda_creare(piscina,
        _ch(piscina, "mutatio")));
    json_objectum_ponere(args, "clavis", json_chorda_creare(piscina,
        _ch(piscina, clavis)));
    json_objectum_ponere(args, "valor", json_chorda_creare(piscina,
        _ch(piscina, valor)));
    (vacuum)cliens_tabularii_vocare(cliens, piscina, "gerere", args, &culpa);

    json_objectum_ponere(q, "res", json_chorda_creare(piscina, res_id));
    json_objectum_ponere(q, "breviter", json_chorda_creare(piscina,
        _ch(piscina, "verum")));
    redde cliens_tabularii_vocare(cliens, piscina, "res", q, &culpa);
}

s32
principale (vacuum)
{
    Piscina* piscina;
    b32      praeteritus;
    integer  portus_int = ZEPHYRUM;
    i32      k;

    piscina = piscina_generare_dynamicum("probatio_sent_horreum", 33554432);
    si (!piscina) { imprimere("FRACTA: piscina\n"); redde I; }
    credo_aperire(piscina);

    imprimere("\n--- I. daemon ephemerus ---\n");
    {
        (vacuum)system("rm -f build/probatio_sent.*");
        CREDO_VERUM (system("./gesta/tabulariumd.sh -struere"
            " > /dev/null 2>&1") == ZEPHYRUM);
        CREDO_VERUM (system("gesta/build/tabulariumd -portus 0"
            " -scrinium build/probatio_sent.db"
            " -annales build/probatio_sent.jsonl"
            " > build/probatio_sent.portus 2> /dev/null &") == ZEPHYRUM);

        per (k = ZEPHYRUM; k < XXV; k++)
        {
            FILE* pf = fopen("build/probatio_sent.portus", "r");

            si (pf != NIHIL)
            {
                si (fscanf(pf, "portus %d", &portus_int) == I &&
                    portus_int > 0)
                { fclose(pf); frange; }
                fclose(pf);
            }
            (vacuum)system("sleep 0.2");
        }
        CREDO_VERUM (portus_int > 0);
    }

    si (portus_int > 0)
    {
        ClientTabularii cliens;
        chorda          culpa = chorda_ex_literis("", piscina);
        chorda          fons_originalis;
        chorda          res_id = chorda_ex_literis("", piscina);
        Liber           ante;

        cliens_tabularii_incipere(&cliens, (i32)portus_int);
        cliens.praefixum = "[probatio]";
        cliens.actor     = "sententiae";
        cliens.launcher  = NIHIL;   /* daemon iam currit */
        cliens.genitus   = VERUM;

        fons_originalis = filum_legere_totum(FX_LIBER, piscina);
        si (fons_originalis.mensura == ZEPHYRUM)
        { imprimere("  FIXUM ABEST: %s\n", FX_LIBER); }
        CREDO_VERUM (fons_originalis.mensura > ZEPHYRUM);

        ante = sententiae_legere(fons_originalis, piscina);
        CREDO_VERUM (ante.successus);
        CREDO_AEQUALIS_I32 (xar_numerus(ante.sententiae), (i32)273);

        imprimere("\n--- II. genus 'liber': custos tabulam GENERA legit ---\n");
        {
            chorda resp;

            CREDO_FALSUM (_genus_adest(&cliens, piscina, "liber"));
            resp = _liber_definire(&cliens, piscina, "Liber", VERUM);
            CREDO_CHORDA_CONTINET (resp, _ch(piscina, "creata"));
            CREDO_VERUM (_genus_adest(&cliens, piscina, "liber"));
        }

        imprimere("\n--- III. iter per conditorium: OCTETUS PRO OCTETO ---\n");
        {
            JsonValor* datum = json_objectum_creare(piscina);
            JsonValor* args  = json_objectum_creare(piscina);
            chorda     resp;

            json_objectum_ponere(datum, "siglum", json_chorda_creare(piscina,
                _ch(piscina, "DI")));
            json_objectum_ponere(datum, "slug", json_chorda_creare(piscina,
                _ch(piscina, "de-imagine")));
            json_objectum_ponere(datum, "status_libri",
                json_chorda_creare(piscina, _ch(piscina, "DRAFT")));
            json_objectum_ponere(datum, "fons", json_chorda_creare(piscina,
                fons_originalis));

            json_objectum_ponere(args, "genus", json_chorda_creare(piscina,
                _ch(piscina, "liber")));
            json_objectum_ponere(args, "titulus", json_chorda_creare(piscina,
                _ch(piscina, "De Imagine")));
            json_objectum_ponere(args, "datum", json_chorda_creare(piscina,
                json_scribere(datum, piscina)));

            resp = cliens_tabularii_vocare(&cliens, piscina, "addere",
                args, &culpa);
            res_id = cliens_tabularii_res_id(resp, piscina);
            CREDO_CHORDA_NON_VACUA (res_id);
        }

        {
            JsonValor* entia;
            chorda     fons_reditus = chorda_ex_literis("", piscina);
            Liber      post;

            entia = cliens_tabularii_legere(&cliens, piscina,
                _ch(piscina, "liber"), (i32)X, &culpa);
            CREDO_NON_NIHIL (entia);
            si (entia != NIHIL && json_est_tabulatum(entia))
            {
                JsonValor* e;

                CREDO_AEQUALIS_I32 (json_tabulatum_numerus(entia), (i32)1);
                e = json_tabulatum_obtinere(entia, (i32)ZEPHYRUM);
                CREDO_NON_NIHIL (e);
                si (e != NIHIL)
                {
                    JsonValor* d = json_objectum_capere(e, "datum");

                    CREDO_NON_NIHIL (d);
                    si (d != NIHIL)
                    {
                        JsonValor* f = json_objectum_capere(d, "fons");

                        CREDO_NON_NIHIL (f);
                        si (f != NIHIL && json_est_chorda(f))
                        { fons_reditus = json_ad_chorda(f); }
                    }
                }
            }

            /* ADSERTIO PRAECIPUA I: octetus pro octeto. */
            CREDO_AEQUALIS_I32 (fons_reditus.mensura,
                                fons_originalis.mensura);
            CREDO_CHORDA_AEQUALIS (fons_reditus, fons_originalis);

            /* ADSERTIO PRAECIPUA II: et ergo NULLA ancora frangitur.
             * Adsertio octetorum sola dicit textum parem esse; haec
             * dicit quod REFERT - sigilla omnia congruere, ergo
             * commentaria omnia iter superstitura. */
            post = sententiae_legere(fons_reditus, piscina);
            CREDO_VERUM (post.successus);
            CREDO_AEQUALIS_I32 (xar_numerus(post.sententiae), (i32)273);

            {
                i32 congruentia = ZEPHYRUM;

                per (k = ZEPHYRUM; k < xar_numerus(ante.sententiae); k++)
                {
                    Sententia* a = (Sententia*)xar_obtinere(ante.sententiae,
                        (i32)k);
                    Sententia* b = (Sententia*)xar_obtinere(post.sententiae,
                        (i32)k);

                    si (a == NIHIL || b == NIHIL) { perge; }
                    si (chorda_aequalis(a->locus, b->locus) &&
                        sigillum_aequale(&a->sigillum, &b->sigillum))
                    { congruentia++; }
                }
                CREDO_AEQUALIS_I32 (congruentia, (i32)273);
            }
        }

        /* CAVE ORDINIS: haec sectio ANTE sectionem V currere DEBET.
         * Sectio V genus consulto collidit et iudicium camporum
         * EXSTINGUIT; post eam quaelibet adsertio hic TACITE
         * praeteriret (campus ignotus notam non pareret, et adsertio
         * negativa laeta esset ob causam prorsus falsam).
         *
         * Sectionem novam quae iudicium spectat inter IV et V NE
         * INSERAS nisi cur constet. Vide caput plagulae. */
        imprimere("\n--- IV. iudicium camporum vivit ---\n");
        {
            chorda lectio;

            /* Villa 'nota' adhibuit quia campum declaratum non habebat.
             * Hic 'fons' declaratus est, ergo mutatio MUNDA est - et
             * mutatio RECTA est, quia nota campum non ponit. */
            lectio = _mutare_et_legere(&cliens, piscina, res_id, "fons",
                "# Breve\n\n```\n1. Nova sententia.\n```\n");
            CREDO_CHORDA_NON_VACUA (lectio);
            CREDO_FALSUM (chorda_continet(lectio,
                _ch(piscina, "violatio camporum")));

            /* et campus IGNOTUS notam custodiae PARIT - ergo silentium
             * supra iudicium est, non taciturnitas */
            lectio = _mutare_et_legere(&cliens, piscina, res_id,
                "campus_ignotus", "quidlibet");
            CREDO_CHORDA_CONTINET (lectio,
                _ch(piscina, "violatio camporum"));
        }

        imprimere("\n--- V. collisio: responsum MUNDUM, iudicium DELETUM ---\n");
        {
            chorda resp;
            chorda lectio;

            /* Doctrina dicebat: 'responsum addere pro iam occupata
             * inspice'. Non operatur - et pretium non est nuntius
             * amissus sed SCHEMA RAPTUM. */
            resp = _liber_definire(&cliens, piscina, "Liber Alter", FALSUM);

            /* (a) responsum successus MUNDUS est */
            CREDO_CHORDA_CONTINET (resp, _ch(piscina, "creata"));
            CREDO_FALSUM (chorda_continet(resp,
                _ch(piscina, "iam occupata")));

            /* (b) et IUDICIUM NUNC TACET. Definitio collidens sine
             * campis registrum rapuit; genus sine campis omnino non
             * iudicatur. Ergo campus ignotus qui modo notam peperit
             * nunc TACITE transit.
             *
             * Hoc primo conatu huius portae ACCIDIT: sectio collisionis
             * ante sectionem iudicii posita erat, et sectionem iudicii
             * tacite fregit. Nunc adseritur, non patitur. */
            lectio = _mutare_et_legere(&cliens, piscina, res_id,
                "alius_campus_ignotus", "quidlibet");
            CREDO_CHORDA_NON_VACUA (lectio);
            CREDO_FALSUM (chorda_continet(lectio,
                _ch(piscina, "alius_campus_ignotus")));
        }

        imprimere("\n--- VI. recuperatio: definitio recta iudicium reddit ---\n");
        {
            chorda lectio;

            (vacuum)_liber_definire(&cliens, piscina, "Liber", VERUM);

            lectio = _mutare_et_legere(&cliens, piscina, res_id,
                "tertius_campus_ignotus", "quidlibet");
            CREDO_CHORDA_CONTINET (lectio,
                _ch(piscina, "tertius_campus_ignotus"));
        }

        imprimere("\n--- VII. 'breviter' corpus NON effundit ---\n");
        {
            JsonValor* q = json_objectum_creare(piscina);
            chorda     brevis_lectio;

            json_objectum_ponere(q, "res", json_chorda_creare(piscina,
                res_id));
            json_objectum_ponere(q, "breviter", json_chorda_creare(piscina,
                _ch(piscina, "verum")));
            brevis_lectio = cliens_tabularii_vocare(&cliens, piscina, "res",
                q, &culpa);

            /* 'res' plenum datum crudum verbatim imprimit - in libro
             * XLV KB in responsum funderet. Porta ne quis eam semitam
             * in app aut in usu meo post compactionem adhibeat. */
            CREDO_MINOR_S32 ((s32)brevis_lectio.mensura, (s32)4096);
        }

        imprimere("\n--- VIII. annales: mutatio scripta ---\n");
        {
            chorda annales = filum_legere_totum(
                "build/probatio_sent.jsonl", piscina);

            CREDO_VERUM (annales.mensura > ZEPHYRUM);
            CREDO_CHORDA_CONTINET (annales,
                _ch(piscina, "\"genus_eventus\":\"mutatio\""));
            CREDO_CHORDA_CONTINET (annales,
                _ch(piscina, "\"clavis\":\"fons\""));
        }
    }

    {
        character imperium[1024];

        sprintf(imperium, "pkill -f 'tabulariumd -portus 0"
            " -scrinium build/probatio_sent.db' > /dev/null 2>&1");
        (vacuum)system(imperium);
        (vacuum)system("rm -f build/probatio_sent.*");
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    si (praeteritus) { redde ZEPHYRUM; }
    redde I;
}

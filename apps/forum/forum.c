/* apps/forum/forum.c - forum: fenestra vitreae in mundum scriptorum
 * (F2 sceletum + F3 articuli/fila - app prima in apps/).
 *
 * Exemplar tabellae (ansa app-possessa, capsula, internuntius) +
 * cliens daemonis tabulariumd: connexio TCP PER-PETITIONEM
 * (contractus v1 spec-v2 par I), initialize non necessarium (daemon
 * se praeinitiat), start-if-absent ut fori (defectus CITUS post
 * generationem primam - anquisitio IS tractatorem ne congelet).
 *
 * Tractatores IS (F3 generales):
 *   res_legere {genus} -> tabulatum structuratum (daemon "legere")
 *   mittere {genus, corpus, ad?} -> addere + nexus respondet-ad
 *   articulum_servare {res_id?, titulus, corpus} -> addere conditum
 *     aut mutatio datum-mersione (mersio superficialis probata)
 *   status_ponere {res_id, novus} -> gerere status
 *   mutare {res_id, corpus} -> editio (mutatio clavis)
 *   delere {res_id} -> tumulus (remotio corporis)
 * Actor semper "fran" - app instrumentum Franis est; Claudius per
 * MCP scribit (fori) cum signatura.
 *
 * Vexillum -fumus: pipata + articuli leguntur ANTE fenestram,
 * numeri impressi, exitus - porta scriptabilis sine oculis. */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "json.h"
#include "tcp.h"
#include "fenestra.h"
#include "capsula.h"
#include "vitrea.h"
#include "internuntius.h"
#include "speculum.h"
#include "cliens_tabularii.h"
#include "capsula_forum.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* capsula fontium speculi (build/speculum/forum/) - externus
 * directus, mos consumptoris speculi (caput generatum non
 * includitur: silva ".." non resolvit, symbolum contractus est) */
/* <aedilis obiectum="build/speculum/forum/capsula_speculi_forum.c"/> */
externus constans CapsulaEmbed capsula_speculi_forum;

#define FORUM_PORTUS_ORDINARIUS 8753
/* tempus excedens fumi pleni: CL gressus x CC ms = XXX s
 * (chorographia generum G2 gyros addidit - ansa laxata) */
#define FUMUS_GRESSUS_MAXIMI 150

nomen structura {
    ClientTabularii cliens;   /* portus, index, actor, launcher */
    /* fumus plenus (-fumus-plenus): fenestra vera, IS pipat, C
     * respondet via daemonis (MCP simulata), IS filium videt */
    b32 fumus_plenus;
    b32 fumus_perfectus_est;
    b32 fumus_responsum_missum;
    character fumus_pipatum_id[LXIV];
    i32 fumus_pipatum_mensura;
} ForumStatus;

/* litterae -> chorda (sine copia; unio contra cast-qual) */
interior chorda
_ch_forum (constans character* litterae)
{
    chorda c;
    unio { constans character* l; i8* m; } u;

    u.l = litterae;
    c.datum = u.m;
    c.mensura = (i32)strlen(litterae);
    redde c;
}

/* praefixum XL codicillorum ut titulus (limes UTF-8 servatus) */
interior chorda
_titulus_ex_corpore (chorda corpus)
{
    i32 codicilli = ZEPHYRUM;
    i32 i = ZEPHYRUM;
    chorda t;

    dum (i < corpus.mensura && codicilli < XL)
    {
        i++;
        dum (i < corpus.mensura
            && ((insignatus character)corpus.datum[i] & 0xC0u)
                == 0x80u)
        {
            i++;
        }
        codicilli++;
    }
    t.datum = corpus.datum;
    t.mensura = i;
    redde t;
}

/* ==================================================
 * tractatores internuntii
 * ================================================== */

interior JsonValor*
_res_legere (JsonValor* argumenta, Piscina* piscina,
    vacuum* datum, chorda* culpa)
{
    ForumStatus* forum = (ForumStatus*)datum;
    chorda genus;
    JsonValor* fructus;
    JsonValor* res;

    genus.mensura = ZEPHYRUM;
    genus.datum = NIHIL;
    si (argumenta != NIHIL)
    {
        genus = json_ad_chorda(json_objectum_capere(argumenta,
            "genus"));
    }
    si (genus.mensura == ZEPHYRUM)
    {
        *culpa = _ch_forum("genus requiritur");
        redde NIHIL;
    }
    res = cliens_tabularii_legere(&forum->cliens, piscina, genus,
        (i32)CC, culpa);
    si (res == NIHIL)
    {
        redde NIHIL;
    }
    fructus = json_objectum_creare(piscina);
    json_objectum_ponere(fructus, "res", res);
    redde fructus;
}

/* mittere {genus, corpus, ad?}: addere (actor fran) + nexus
 * respondet-ad si ad datur. Fructus {bene, res_id}. */
interior JsonValor*
_mittere (JsonValor* argumenta, Piscina* piscina, vacuum* datum,
    chorda* culpa)
{
    ForumStatus* forum = (ForumStatus*)datum;
    chorda genus, corpus, ad;
    JsonValor* arg_obj;
    chorda textus;
    chorda novum_id;
    JsonValor* fructus;

    genus.mensura = ZEPHYRUM; genus.datum = NIHIL;
    corpus.mensura = ZEPHYRUM; corpus.datum = NIHIL;
    ad.mensura = ZEPHYRUM; ad.datum = NIHIL;
    si (argumenta != NIHIL)
    {
        genus = json_ad_chorda(json_objectum_capere(argumenta,
            "genus"));
        corpus = json_ad_chorda(json_objectum_capere(argumenta,
            "corpus"));
        ad = json_ad_chorda(json_objectum_capere(argumenta, "ad"));
    }
    si (genus.mensura == ZEPHYRUM || corpus.mensura == ZEPHYRUM)
    {
        *culpa = _ch_forum("genus et corpus requiruntur");
        redde NIHIL;
    }
    arg_obj = json_objectum_creare(piscina);
    json_objectum_ponere(arg_obj, "genus",
        json_chorda_creare(piscina, genus));
    json_objectum_ponere(arg_obj, "titulus",
        json_chorda_creare(piscina, _titulus_ex_corpore(corpus)));
    json_objectum_ponere(arg_obj, "corpus",
        json_chorda_creare(piscina, corpus));
    json_objectum_ponere(arg_obj, "actor",
        json_chorda_creare_literis(piscina, "fran"));
    textus = cliens_tabularii_vocare(&forum->cliens, piscina, "addere",
        arg_obj, culpa);
    si (textus.mensura == ZEPHYRUM)
    {
        redde NIHIL;
    }
    novum_id = cliens_tabularii_res_id(textus, piscina);
    si (ad.mensura > ZEPHYRUM && novum_id.mensura > ZEPHYRUM)
    {
        JsonValor* nexus_obj = json_objectum_creare(piscina);
        chorda nexus_textus;

        json_objectum_ponere(nexus_obj, "res",
            json_chorda_creare(piscina, novum_id));
        json_objectum_ponere(nexus_obj, "actus",
            json_chorda_creare_literis(piscina, "nexus"));
        json_objectum_ponere(nexus_obj, "verbum",
            json_chorda_creare_literis(piscina, "respondet-ad"));
        json_objectum_ponere(nexus_obj, "alterum",
            json_chorda_creare(piscina, ad));
        json_objectum_ponere(nexus_obj, "actor",
            json_chorda_creare_literis(piscina, "fran"));
        nexus_textus = cliens_tabularii_vocare(&forum->cliens, piscina,
            "gerere", nexus_obj, culpa);
        si (nexus_textus.mensura == ZEPHYRUM)
        {
            /* res creata sed filum fractum - culpa nominat */
            *culpa = _ch_forum("res creata sed nexus fili fractus");
            redde NIHIL;
        }
    }
    /* fumus plenus: pipatum IS-latum notatur - C ei respondebit */
    si (forum->fumus_plenus && ad.mensura == ZEPHYRUM
        && novum_id.mensura > ZEPHYRUM
        && novum_id.mensura < (i32)LXIV)
    {
        memcpy(forum->fumus_pipatum_id, novum_id.datum,
            (memoriae_index)novum_id.mensura);
        forum->fumus_pipatum_mensura = novum_id.mensura;
    }
    imprimere("[forum] missum (%.*s): %.*s\n",
        (int)genus.mensura, (constans character*)genus.datum,
        (int)(corpus.mensura > XL ? XL : corpus.mensura),
        (constans character*)corpus.datum);
    fflush(stdout);
    fructus = json_objectum_creare(piscina);
    json_objectum_ponere(fructus, "bene",
        json_boolean_creare(piscina, VERUM));
    json_objectum_ponere(fructus, "res_id",
        json_chorda_creare(piscina, novum_id));
    redde fructus;
}

/* articulum_servare {res_id?, titulus, corpus}: novus = addere
 * conditum; exsistens = mutatio datum-mersione (superficialis -
 * signatura/tags supersunt) */
interior JsonValor*
_articulum_servare (JsonValor* argumenta, Piscina* piscina,
    vacuum* datum, chorda* culpa)
{
    ForumStatus* forum = (ForumStatus*)datum;
    chorda res_id, titulus, corpus;
    chorda textus;
    chorda novum_id;
    JsonValor* fructus;

    res_id.mensura = ZEPHYRUM; res_id.datum = NIHIL;
    titulus.mensura = ZEPHYRUM; titulus.datum = NIHIL;
    corpus.mensura = ZEPHYRUM; corpus.datum = NIHIL;
    si (argumenta != NIHIL)
    {
        res_id = json_ad_chorda(json_objectum_capere(argumenta,
            "res_id"));
        titulus = json_ad_chorda(json_objectum_capere(argumenta,
            "titulus"));
        corpus = json_ad_chorda(json_objectum_capere(argumenta,
            "corpus"));
    }
    si (titulus.mensura == ZEPHYRUM)
    {
        *culpa = _ch_forum("titulus requiritur");
        redde NIHIL;
    }
    si (res_id.mensura == ZEPHYRUM)
    {
        JsonValor* arg_obj = json_objectum_creare(piscina);

        json_objectum_ponere(arg_obj, "genus",
            json_chorda_creare_literis(piscina, "articulus"));
        json_objectum_ponere(arg_obj, "titulus",
            json_chorda_creare(piscina, titulus));
        si (corpus.mensura > ZEPHYRUM)
        {
            json_objectum_ponere(arg_obj, "corpus",
                json_chorda_creare(piscina, corpus));
        }
        json_objectum_ponere(arg_obj, "actor",
            json_chorda_creare_literis(piscina, "fran"));
        textus = cliens_tabularii_vocare(&forum->cliens, piscina, "addere",
            arg_obj, culpa);
        si (textus.mensura == ZEPHYRUM)
        {
            redde NIHIL;
        }
        novum_id = cliens_tabularii_res_id(textus, piscina);
    }
    alioquin
    {
        JsonValor* arg_obj = json_objectum_creare(piscina);
        JsonValor* mersio = json_objectum_creare(piscina);

        json_objectum_ponere(mersio, "titulus",
            json_chorda_creare(piscina, titulus));
        json_objectum_ponere(mersio, "corpus",
            json_chorda_creare(piscina, corpus));
        json_objectum_ponere(arg_obj, "res",
            json_chorda_creare(piscina, res_id));
        json_objectum_ponere(arg_obj, "actus",
            json_chorda_creare_literis(piscina, "mutatio"));
        json_objectum_ponere(arg_obj, "datum",
            json_chorda_creare(piscina,
                json_scribere(mersio, piscina)));
        json_objectum_ponere(arg_obj, "actor",
            json_chorda_creare_literis(piscina, "fran"));
        textus = cliens_tabularii_vocare(&forum->cliens, piscina, "gerere",
            arg_obj, culpa);
        si (textus.mensura == ZEPHYRUM)
        {
            redde NIHIL;
        }
        novum_id = res_id;
    }
    imprimere("[forum] articulus servatus: %.*s\n",
        (int)titulus.mensura, (constans character*)titulus.datum);
    fflush(stdout);
    fructus = json_objectum_creare(piscina);
    json_objectum_ponere(fructus, "bene",
        json_boolean_creare(piscina, VERUM));
    json_objectum_ponere(fructus, "res_id",
        json_chorda_creare(piscina, novum_id));
    redde fructus;
}

/* actus unus gerere super rem: adiutor communis status_ponere/
 * mutare/delere */
interior JsonValor*
_gerere_simplex (ForumStatus* forum, Piscina* piscina,
    JsonValor* arg_obj, chorda* culpa)
{
    chorda textus = cliens_tabularii_vocare(&forum->cliens, piscina, "gerere",
        arg_obj, culpa);
    JsonValor* fructus;

    si (textus.mensura == ZEPHYRUM)
    {
        redde NIHIL;
    }
    fructus = json_objectum_creare(piscina);
    json_objectum_ponere(fructus, "bene",
        json_boolean_creare(piscina, VERUM));
    redde fructus;
}

interior JsonValor*
_status_ponere (JsonValor* argumenta, Piscina* piscina,
    vacuum* datum, chorda* culpa)
{
    ForumStatus* forum = (ForumStatus*)datum;
    chorda res_id, novus;
    JsonValor* arg_obj;

    res_id.mensura = ZEPHYRUM; res_id.datum = NIHIL;
    novus.mensura = ZEPHYRUM; novus.datum = NIHIL;
    si (argumenta != NIHIL)
    {
        res_id = json_ad_chorda(json_objectum_capere(argumenta,
            "res_id"));
        novus = json_ad_chorda(json_objectum_capere(argumenta,
            "novus"));
    }
    si (res_id.mensura == ZEPHYRUM || novus.mensura == ZEPHYRUM)
    {
        *culpa = _ch_forum("res_id et novus requiruntur");
        redde NIHIL;
    }
    arg_obj = json_objectum_creare(piscina);
    json_objectum_ponere(arg_obj, "res",
        json_chorda_creare(piscina, res_id));
    json_objectum_ponere(arg_obj, "actus",
        json_chorda_creare_literis(piscina, "status"));
    json_objectum_ponere(arg_obj, "novus",
        json_chorda_creare(piscina, novus));
    json_objectum_ponere(arg_obj, "actor",
        json_chorda_creare_literis(piscina, "fran"));
    redde _gerere_simplex(forum, piscina, arg_obj, culpa);
}

interior JsonValor*
_mutare (JsonValor* argumenta, Piscina* piscina, vacuum* datum,
    chorda* culpa)
{
    ForumStatus* forum = (ForumStatus*)datum;
    chorda res_id, corpus;
    JsonValor* arg_obj;

    res_id.mensura = ZEPHYRUM; res_id.datum = NIHIL;
    corpus.mensura = ZEPHYRUM; corpus.datum = NIHIL;
    si (argumenta != NIHIL)
    {
        res_id = json_ad_chorda(json_objectum_capere(argumenta,
            "res_id"));
        corpus = json_ad_chorda(json_objectum_capere(argumenta,
            "corpus"));
    }
    si (res_id.mensura == ZEPHYRUM || corpus.mensura == ZEPHYRUM)
    {
        *culpa = _ch_forum("res_id et corpus requiruntur");
        redde NIHIL;
    }
    arg_obj = json_objectum_creare(piscina);
    json_objectum_ponere(arg_obj, "res",
        json_chorda_creare(piscina, res_id));
    json_objectum_ponere(arg_obj, "actus",
        json_chorda_creare_literis(piscina, "mutatio"));
    json_objectum_ponere(arg_obj, "clavis",
        json_chorda_creare_literis(piscina, "corpus"));
    json_objectum_ponere(arg_obj, "valor",
        json_chorda_creare(piscina, corpus));
    json_objectum_ponere(arg_obj, "actor",
        json_chorda_creare_literis(piscina, "fran"));
    redde _gerere_simplex(forum, piscina, arg_obj, culpa);
}

interior JsonValor*
_delere (JsonValor* argumenta, Piscina* piscina, vacuum* datum,
    chorda* culpa)
{
    ForumStatus* forum = (ForumStatus*)datum;
    chorda res_id;
    JsonValor* arg_obj;

    res_id.mensura = ZEPHYRUM; res_id.datum = NIHIL;
    si (argumenta != NIHIL)
    {
        res_id = json_ad_chorda(json_objectum_capere(argumenta,
            "res_id"));
    }
    si (res_id.mensura == ZEPHYRUM)
    {
        *culpa = _ch_forum("res_id requiritur");
        redde NIHIL;
    }
    arg_obj = json_objectum_creare(piscina);
    json_objectum_ponere(arg_obj, "res",
        json_chorda_creare(piscina, res_id));
    json_objectum_ponere(arg_obj, "actus",
        json_chorda_creare_literis(piscina, "remotio"));
    json_objectum_ponere(arg_obj, "clavis",
        json_chorda_creare_literis(piscina, "corpus"));
    json_objectum_ponere(arg_obj, "actor",
        json_chorda_creare_literis(piscina, "fran"));
    redde _gerere_simplex(forum, piscina, arg_obj, culpa);
}

/* fumus_modus {} -> {plenus} - IS choreographiam fumi rogaverit */
interior JsonValor*
_fumus_modus (JsonValor* argumenta, Piscina* piscina,
    vacuum* datum, chorda* culpa)
{
    ForumStatus* forum = (ForumStatus*)datum;
    JsonValor* fructus = json_objectum_creare(piscina);

    (vacuum)argumenta;
    (vacuum)culpa;
    json_objectum_ponere(fructus, "plenus",
        json_boolean_creare(piscina, forum->fumus_plenus));
    redde fructus;
}

/* fumus_perfectus {} - IS filium vidit: gyrus totus clausus */
interior JsonValor*
_fumus_perfectus (JsonValor* argumenta, Piscina* piscina,
    vacuum* datum, chorda* culpa)
{
    ForumStatus* forum = (ForumStatus*)datum;

    (vacuum)argumenta;
    (vacuum)culpa;
    forum->fumus_perfectus_est = VERUM;
    redde json_objectum_creare(piscina);
}

s32 principale (integer argc, character** argv)
{
    Piscina* piscina = piscina_generare_dynamicum("forum",
        16777216);
    Piscina* piscina_vocationis = piscina_generare_dynamicum(
        "forum_vocationes", 8388608);
    ForumStatus forum;
    b32 fumus = FALSUM;
    integer k;
    FenestraConfiguratio figura_fenestrae;
    VitreaConfiguratio figura_vitreae;
    Fenestra* fenestra;
    Capsula* capsula;
    Vitrea* vitrea;
    Internuntius* inx;

    si (piscina == NIHIL || piscina_vocationis == NIHIL)
    {
        redde I;
    }
    cliens_tabularii_incipere(&forum.cliens,
        (i32)FORUM_PORTUS_ORDINARIUS);
    forum.cliens.praefixum = "[forum]";
    forum.fumus_plenus = FALSUM;
    forum.fumus_perfectus_est = FALSUM;
    forum.fumus_responsum_missum = FALSUM;
    forum.fumus_pipatum_mensura = ZEPHYRUM;
    per (k = I; k < argc; k++)
    {
        si (strcmp(argv[k], "-portus") == ZEPHYRUM && k + I < argc)
        {
            forum.cliens.portus = (i32)atoi(argv[k + I]);
            k++;
        }
        alioquin si (strcmp(argv[k], "-fumus") == ZEPHYRUM)
        {
            fumus = VERUM;
        }
        alioquin si (strcmp(argv[k], "-fumus-plenus") == ZEPHYRUM)
        {
            forum.fumus_plenus = VERUM;
        }
    }
    si (fumus)
    {
        /* porta sine oculis: lectiones contra daemonem verum ante
         * fenestram ullam */
        chorda culpa;
        JsonValor* pipata;
        JsonValor* articuli;

        culpa.mensura = ZEPHYRUM;
        culpa.datum = NIHIL;
        pipata = cliens_tabularii_legere(&forum.cliens, piscina,
            _ch_forum("pipatum"), (i32)CC, &culpa);
        articuli = cliens_tabularii_legere(&forum.cliens, piscina,
            _ch_forum("articulus"), (i32)CC, &culpa);
        si (pipata == NIHIL || articuli == NIHIL)
        {
            imprimere("[forum] fumus FRACTUS: %.*s\n",
                (int)culpa.mensura,
                (constans character*)culpa.datum);
            redde I;
        }
        imprimere("[forum] fumus: %d pipata, %d articuli\n",
            (int)json_tabulatum_numerus(pipata),
            (int)json_tabulatum_numerus(articuli));
        piscina_destruere(piscina_vocationis);
        piscina_destruere(piscina);
        redde ZEPHYRUM;
    }

    figura_fenestrae.titulus = "forum";
    figura_fenestrae.x = CC;
    figura_fenestrae.y = CC;
    figura_fenestrae.latitudo = 1000;
    figura_fenestrae.altitudo = 900;
    figura_fenestrae.vexilla = FENESTRA_CLAUDIBILIS
        | FENESTRA_MUTABILIS | FENESTRA_CENTRATA;
    fenestra = fenestra_creare(piscina, &figura_fenestrae);
    si (fenestra == NIHIL)
    {
        imprimere("FRACTA: fenestra\n");
        redde I;
    }
    capsula = capsula_aperire(&capsula_forum, piscina);
    si (capsula == NIHIL)
    {
        imprimere("FRACTA: capsula\n");
        redde I;
    }
    figura_vitreae.origo = VITREA_ORIGO_CAPSULA;
    figura_vitreae.capsula = capsula;
    figura_vitreae.via_initialis = "index.html";
    figura_vitreae.url = NIHIL;
    figura_vitreae.inspectabilis = VERUM;
    vitrea = vitrea_creare(piscina, fenestra, &figura_vitreae);
    si (vitrea == NIHIL)
    {
        imprimere("FRACTA: vitrea\n");
        redde I;
    }
    inx = internuntius_creare(piscina, vitrea_missor, vitrea);
    si (inx == NIHIL)
    {
        imprimere("FRACTA: internuntius\n");
        redde I;
    }
    (vacuum)internuntius_praebere(inx, "res_legere",
        _res_legere, &forum);
    (vacuum)internuntius_praebere(inx, "mittere",
        _mittere, &forum);
    (vacuum)internuntius_praebere(inx, "articulum_servare",
        _articulum_servare, &forum);
    (vacuum)internuntius_praebere(inx, "status_ponere",
        _status_ponere, &forum);
    (vacuum)internuntius_praebere(inx, "mutare",
        _mutare, &forum);
    (vacuum)internuntius_praebere(inx, "delere",
        _delere, &forum);
    (vacuum)internuntius_praebere(inx, "transmittere",
        cliens_tabularii_transmittere, &forum.cliens);
    (vacuum)internuntius_praebere(inx, "fumus_modus",
        _fumus_modus, &forum);
    (vacuum)internuntius_praebere(inx, "fumus_perfectus",
        _fumus_perfectus, &forum);

    /* modus-debug se-fontis: Cmd+Shift+D (mos domus) */
    {
        Speculum* speculum = speculum_creare(piscina,
            &capsula_speculi_forum, inx, vitrea_aestimator,
            vitrea);
        i32 gressus_fumi = ZEPHYRUM;

        si (speculum == NIHIL)
        {
            imprimere("FRACTA: speculum\n");
            redde I;
        }

        imprimere("[forum] fenestra aperta (daemon portus %d)\n",
            (int)forum.cliens.portus);
        fflush(stdout);

        dum (!fenestra_debet_claudere(fenestra))
        {
            Eventus eventus;
            chorda nuntium;
            VitreaNuntiusGenus genus;
            PiscinaNotatio nota;

            fenestra_expectare_eventus(fenestra, CC);
            dum (fenestra_obtinere_eventus(fenestra, &eventus))
            {
                (vacuum)speculum_tangere(speculum, &eventus);
            }
            nota = piscina_notare(piscina_vocationis);
            dum (vitrea_obtinere_nuntium(vitrea, &nuntium, &genus))
            {
                si (genus == VITREA_NUNTIUS_PONS)
                {
                    internuntius_tractare(inx, nuntium,
                        piscina_vocationis);
                }
                alioquin
                {
                    vitrea_recargare(vitrea);
                }
            }
            /* fumus plenus: C respondet pipato IS-lato via
             * daemonis (via MCP simulata), deinde IS filium
             * videre debet et fumus_perfectus vocare */
            si (forum.fumus_plenus)
            {
                si (forum.fumus_pipatum_mensura > ZEPHYRUM
                    && !forum.fumus_responsum_missum)
                {
                    JsonValor* arg_obj =
                        json_objectum_creare(piscina_vocationis);
                    chorda ad_id;
                    chorda culpa_f;
                    chorda textus_f;

                    ad_id.datum =
                        (i8*)forum.fumus_pipatum_id;
                    ad_id.mensura = forum.fumus_pipatum_mensura;
                    culpa_f.mensura = ZEPHYRUM;
                    culpa_f.datum = NIHIL;
                    json_objectum_ponere(arg_obj, "genus",
                        json_chorda_creare_literis(
                            piscina_vocationis, "pipatum"));
                    json_objectum_ponere(arg_obj, "corpus",
                        json_chorda_creare_literis(
                            piscina_vocationis,
                            "[fumus] responsum trans daemonem"));
                    json_objectum_ponere(arg_obj, "ad",
                        json_chorda_creare(piscina_vocationis,
                            ad_id));
                    json_objectum_ponere(arg_obj, "actor",
                        json_chorda_creare_literis(
                            piscina_vocationis, "claude"));
                    json_objectum_ponere(arg_obj, "signatura",
                        json_chorda_creare_literis(
                            piscina_vocationis, "Fumus"));
                    textus_f = cliens_tabularii_vocare(&forum.cliens,
                        piscina_vocationis, "addere", arg_obj,
                        &culpa_f);
                    forum.fumus_responsum_missum = VERUM;
                    si (textus_f.mensura == ZEPHYRUM)
                    {
                        imprimere("[forum] FUMUS FRACTUS:"
                            " responsum non missum\n");
                        redde I;
                    }
                    imprimere("[forum] fumus: responsum missum\n");
                    fflush(stdout);
                }
                si (forum.fumus_perfectus_est)
                {
                    imprimere("FUMUS PLENUS: pipatum IS ->"
                        " responsum C via daemonis -> filium IS"
                        " vidit\n");
                    fflush(stdout);
                    frange;
                }
                gressus_fumi++;
                si (gressus_fumi > (i32)FUMUS_GRESSUS_MAXIMI)
                {
                    imprimere("[forum] FUMUS FRACTUS: tempus"
                        " excessum\n");
                    redde I;
                }
            }
            piscina_reficere(piscina_vocationis, nota);
        }
    }

    imprimere("[forum] finis\n");
    vitrea_destruere(vitrea);
    fenestra_destruere(fenestra);
    piscina_destruere(piscina_vocationis);
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}

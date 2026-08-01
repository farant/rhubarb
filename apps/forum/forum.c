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
#include "vitrea_servus.h"
#include "internuntius.h"
#include "speculum.h"
#include "cliens_tabularii.h"
#include "sententiae.h"
#include "capitula.h"
#include "xar.h"
#include "capsula_forum.h"
#include "moneta.h"
#include "qr.h"
#include "rete.h"
#include "processus.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>   /* mkdir + chmod - app macOS/POSIX sola */

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
    /* servus ad telephonum: PROCESSUS ALTER (ansae eventuum duae
     * numquam intertexuntur). NIHIL = non currit. Pulsatur in ansa
     * app - processus_exsequi fenestram congelaret. */
    Processus* servus_proc;
    i32        servus_portus;
    Piscina*   piscina_diuturna;   /* processui et tesserae */
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
    /* 'sine_campis' + 'res' TRANSMITTUNTUR (01KYCMQMED): index
     * librorum campum 'fons' tacet (XLV KB per documentum saturaret
     * pyxidem clientis CCLVI KB ad V documenta), apertio unum ens
     * dato pleno petit. Absentia utriusque = mos vetus exacte. */
    {
        JsonValor* arg = json_objectum_creare(piscina);
        chorda sine_campis;
        chorda res_unum;

        json_objectum_ponere(arg, "genus",
            json_chorda_creare(piscina, genus));
        json_objectum_ponere(arg, "quantum",
            json_integer_creare(piscina, (s64)CC));
        sine_campis = json_ad_chorda(json_objectum_capere(argumenta,
            "sine_campis"));
        si (sine_campis.mensura > ZEPHYRUM)
        {
            json_objectum_ponere(arg, "sine_campis",
                json_chorda_creare(piscina, sine_campis));
        }
        res_unum = json_ad_chorda(json_objectum_capere(argumenta,
            "res"));
        si (res_unum.mensura > ZEPHYRUM)
        {
            json_objectum_ponere(arg, "res",
                json_chorda_creare(piscina, res_unum));
        }
        res = cliens_tabularii_legere_cum(&forum->cliens, piscina,
            arg, culpa);
    }
    si (res == NIHIL)
    {
        redde NIHIL;
    }
    fructus = json_objectum_creare(piscina);
    json_objectum_ponere(fructus, "res", res);
    redde fructus;
}

/* sententias_parsare {fons} -> structura libri.
 *
 * FUNCTIO PURA: textus intrat, structura exit. Nihil scribitur, nihil
 * legitur - resolutio §II.3 (resolutio unius viae) hic in signatura
 * ipsa apparet.
 *
 * TEXTUM SENTENTIARUM NON REDDIT, CONSULTO. Inscriptio VERBATIM est:
 * consumptor ex 'fons' ipso reddit, non ex arbore. Textum addere
 * responsum duplicaret (XLV KB iterum) pro nullo consumptore. Quod
 * reddimus est id quod JS computare NEQUIT: loci, lineae, gradus,
 * SIGILLA (SHA-256 super formam normatam - nulla via in velamine).
 *
 * CAVE: tractator in FILO INTERFACIEI currit et nulla fila in domo
 * sunt. Documentum unum per vocationem, numquam bibliothecam. */
interior JsonValor*
_sententias_parsare (JsonValor* argumenta, Piscina* piscina,
    vacuum* datum, chorda* culpa)
{
    chorda     fons;
    Liber      liber;
    JsonValor* fructus;
    JsonValor* puncta;
    JsonValor* partes;
    JsonValor* gradus;
    JsonValor* anomaliae;
    i32        k;

    (vacuum)datum;

    fons.mensura = ZEPHYRUM;
    fons.datum   = NIHIL;
    si (argumenta != NIHIL)
    {
        fons = json_ad_chorda(json_objectum_capere(argumenta, "fons"));
    }
    si (fons.mensura == ZEPHYRUM)
    {
        *culpa = _ch_forum("fons requiritur");
        redde NIHIL;
    }

    liber = sententiae_legere(fons, piscina);

    fructus = json_objectum_creare(piscina);
    json_objectum_ponere(fructus, "successus",
        json_boolean_creare(piscina, liber.successus));
    json_objectum_ponere(fructus, "culpa", json_chorda_creare_literis(
        piscina, sententiae_culpae_nomen(liber.culpa)));
    json_objectum_ponere(fructus, "linea_culpae",
        json_integer_creare(piscina, (s64)liber.linea_culpae));
    json_objectum_ponere(fructus, "causa",
        json_chorda_creare(piscina, liber.causa));
    json_objectum_ponere(fructus, "titulus",
        json_chorda_creare(piscina, liber.titulus));
    json_objectum_ponere(fructus, "siglum",
        json_chorda_creare(piscina, liber.siglum));
    json_objectum_ponere(fructus, "status_libri",
        json_chorda_creare(piscina, liber.status));

    /* culpa structurae: arbor vacua est, ergo cetera omittimus */
    si (!liber.successus)
    {
        redde fructus;
    }

    gradus = json_tabulatum_creare(piscina);
    per (k = ZEPHYRUM; k < xar_numerus(liber.vocabularium); k++)
    {
        Gradus*    g = (Gradus*)xar_obtinere(liber.vocabularium, (i32)k);
        JsonValor* o;

        si (g == NIHIL) { perge; }
        o = json_objectum_creare(piscina);
        json_objectum_ponere(o, "vocabulum",
            json_chorda_creare(piscina, g->vocabulum));
        json_objectum_ponere(o, "fert_onus",
            json_boolean_creare(piscina, g->fert_onus));
        json_tabulatum_addere(gradus, o);
    }
    json_objectum_ponere(fructus, "gradus", gradus);

    partes = json_tabulatum_creare(piscina);
    per (k = ZEPHYRUM; k < xar_numerus(liber.partes); k++)
    {
        Pars*      p = (Pars*)xar_obtinere(liber.partes, (i32)k);
        JsonValor* o;

        si (p == NIHIL) { perge; }
        o = json_objectum_creare(piscina);
        json_objectum_ponere(o, "titulus",
            json_chorda_creare(piscina, p->titulus));
        json_objectum_ponere(o, "prima",
            json_integer_creare(piscina, (s64)p->prima));
        json_objectum_ponere(o, "numerus",
            json_integer_creare(piscina, (s64)p->numerus));
        json_tabulatum_addere(partes, o);
    }
    json_objectum_ponere(fructus, "partes", partes);

    puncta = json_tabulatum_creare(piscina);
    per (k = ZEPHYRUM; k < xar_numerus(liber.sententiae); k++)
    {
        Sententia* s = (Sententia*)xar_obtinere(liber.sententiae, (i32)k);
        JsonValor* o;
        character  hex[SIGILLUM_HEX_MENSURA];

        si (s == NIHIL) { perge; }
        sigillum_hex(&s->sigillum, hex);

        o = json_objectum_creare(piscina);
        json_objectum_ponere(o, "locus",
            json_chorda_creare(piscina, s->locus));
        json_objectum_ponere(o, "profunditas",
            json_integer_creare(piscina, (s64)s->profunditas));
        json_objectum_ponere(o, "linea",
            json_integer_creare(piscina, (s64)s->linea));
        json_objectum_ponere(o, "pars",
            json_integer_creare(piscina, (s64)s->pars));
        json_objectum_ponere(o, "gradus",
            json_integer_creare(piscina, (s64)s->gradus));
        json_objectum_ponere(o, "onus",
            json_chorda_creare(piscina, s->onus_gradus));
        json_objectum_ponere(o, "retractum",
            json_boolean_creare(piscina, s->retractum));
        json_objectum_ponere(o, "inresolutum",
            json_boolean_creare(piscina, s->inresolutum));
        /* _literis, NON _ch_forum: json_chorda_creare chordam SINE
         * copia octetorum servat (datum.chorda_valor = valor), et 'hex'
         * ACERVI est - post iterationem pendulus fieret. Consecutio non
         * erat ruina sed RESPONSUM CORRUPTUM: series JSON octetos
         * receptos legit, velamen nihil parsare potuit, promissum
         * numquam solvit. Ergo PENDENTIA, non reiectio - et catch
         * numquam flagravit. */
        json_objectum_ponere(o, "sigillum",
            json_chorda_creare_literis(piscina, hex));
        json_tabulatum_addere(puncta, o);
    }
    json_objectum_ponere(fructus, "puncta", puncta);

    anomaliae = json_tabulatum_creare(piscina);
    per (k = ZEPHYRUM; k < xar_numerus(liber.anomaliae); k++)
    {
        Anomalia*  a = (Anomalia*)xar_obtinere(liber.anomaliae, (i32)k);
        JsonValor* o;

        si (a == NIHIL) { perge; }
        o = json_objectum_creare(piscina);
        json_objectum_ponere(o, "genus", json_chorda_creare_literis(
            piscina, sententiae_anomaliae_nomen(a->genus)));
        json_objectum_ponere(o, "linea",
            json_integer_creare(piscina, (s64)a->linea));
        json_objectum_ponere(o, "textus",
            json_chorda_creare(piscina, a->textus));
        json_objectum_ponere(o, "causa",
            json_chorda_creare(piscina, a->causa));
        json_tabulatum_addere(anomaliae, o);
    }
    json_objectum_ponere(fructus, "anomaliae", anomaliae);

    redde fructus;
}

/* capitula_parsare {fons} -> {capitula: [{inscriptio, titulus, gradus,
 * ordo, linea}]}.
 *
 * FUNCTIO PURA, ut soror sententiarum: textus intrat, structura exit.
 * Scriptio entium velaminis est - hic nihil in conditorium it.
 *
 * NULLA culpa redditur quia nulla esse potest (vide capitula.h §I).
 * Consumptor tamen praevisionem monstrare DEBET ante scriptionem: XL
 * capitula ex lectione tacita nata XL emendationes poscunt. */
interior JsonValor*
_capitula_parsare (JsonValor* argumenta, Piscina* piscina,
    vacuum* datum, chorda* culpa)
{
    chorda     fons;
    IndexLibri idx;
    JsonValor* fructus;
    JsonValor* tabulatum;
    i32        k;

    (vacuum)datum;

    fons.mensura = ZEPHYRUM;
    fons.datum   = NIHIL;
    si (argumenta != NIHIL)
    {
        fons = json_ad_chorda(json_objectum_capere(argumenta, "fons"));
    }
    si (fons.mensura == ZEPHYRUM)
    {
        *culpa = _ch_forum("fons requiritur");
        redde NIHIL;
    }

    idx = capitula_legere(fons, piscina);

    fructus   = json_objectum_creare(piscina);
    tabulatum = json_tabulatum_creare(piscina);

    per (k = ZEPHYRUM; k < xar_numerus(idx.capitula); k++)
    {
        Capitulum* c = (Capitulum*)xar_obtinere(idx.capitula, (i32)k);
        JsonValor* o;

        si (c == NIHIL) { perge; }
        o = json_objectum_creare(piscina);
        json_objectum_ponere(o, "inscriptio",
            json_chorda_creare(piscina, c->inscriptio));
        json_objectum_ponere(o, "titulus",
            json_chorda_creare(piscina, c->titulus));
        json_objectum_ponere(o, "gradus",
            json_integer_creare(piscina, (s64)c->gradus));
        json_objectum_ponere(o, "ordo",
            json_integer_creare(piscina, (s64)c->ordo));
        json_objectum_ponere(o, "linea",
            json_integer_creare(piscina, (s64)c->linea));
        json_tabulatum_addere(tabulatum, o);
    }

    json_objectum_ponere(fructus, "capitula", tabulatum);
    json_objectum_ponere(fructus, "gradus_maximus",
        json_integer_creare(piscina, (s64)idx.gradus_maximus));
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

/* ==================================================
 * IMAGO TERGALIS - anteponimentum machinae localis in
 * privata/forum/ (numquam commissum; res mundi NON est, ergo nec
 * daemon nec annales - plagula una, superscripta in mutatione)
 * ================================================== */

#define FORUM_TERGALE_VIA "privata/forum/tergale.imago"
#define FORUM_TERGALE_MENSURA_MAXIMA (8 * 1024 * 1024)

/* tergale_ponere {datum} -> {bene} - datum = URL datorum integra
 * (textus "data:..." ut est; app formam non interpretatur) */
interior JsonValor*
_tergale_ponere (JsonValor* argumenta, Piscina* piscina,
    vacuum* datum, chorda* culpa)
{
    JsonValor* fructus = json_objectum_creare(piscina);
    chorda imago = json_ad_chorda(json_objectum_capere(argumenta,
        "datum"));
    b32 bene = FALSUM;

    (vacuum)datum;
    (vacuum)culpa;
    si (imago.mensura > (i32)V
        && imago.mensura <= (i32)FORUM_TERGALE_MENSURA_MAXIMA
        && memcmp(imago.datum, "data:", (size_t)V) == ZEPHYRUM)
    {
        FILE* filum;

        (vacuum)mkdir("privata", 0755);
        (vacuum)mkdir("privata/forum", 0755);
        filum = fopen(FORUM_TERGALE_VIA, "wb");
        si (filum != NIHIL)
        {
            si (fwrite(imago.datum, I,
                    (memoriae_index)imago.mensura, filum)
                == (memoriae_index)imago.mensura)
            {
                bene = VERUM;
            }
            (vacuum)fclose(filum);
        }
    }
    json_objectum_ponere(fructus, "bene",
        json_boolean_creare(piscina, bene));
    redde fructus;
}

/* tergale_capere {} -> {datum} - chorda vacua si absens */
interior JsonValor*
_tergale_capere (JsonValor* argumenta, Piscina* piscina,
    vacuum* datum, chorda* culpa)
{
    JsonValor* fructus = json_objectum_creare(piscina);
    FILE* filum = fopen(FORUM_TERGALE_VIA, "rb");
    chorda imago;

    (vacuum)argumenta;
    (vacuum)datum;
    (vacuum)culpa;
    imago.mensura = ZEPHYRUM;
    imago.datum = NIHIL;
    si (filum != NIHIL)
    {
        long mensura_l;

        fseek(filum, 0L, SEEK_END);
        mensura_l = ftell(filum);
        fseek(filum, 0L, SEEK_SET);
        si (mensura_l > 0L
            && mensura_l <= (long)FORUM_TERGALE_MENSURA_MAXIMA)
        {
            imago.datum = (i8*)piscina_allocare(piscina,
                (memoriae_index)mensura_l);
            si (imago.datum != NIHIL
                && fread(imago.datum, I,
                       (memoriae_index)mensura_l, filum)
                    == (memoriae_index)mensura_l)
            {
                imago.mensura = (i32)mensura_l;
            }
            alioquin
            {
                imago.datum = NIHIL;
            }
        }
        (vacuum)fclose(filum);
    }
    si (imago.mensura > ZEPHYRUM)
    {
        json_objectum_ponere(fructus, "datum",
            json_chorda_creare(piscina, imago));
    }
    alioquin
    {
        json_objectum_ponere(fructus, "datum",
            json_chorda_creare_literis(piscina, ""));
    }
    redde fructus;
}

/* tergale_delere {} -> {bene} */
interior JsonValor*
_tergale_delere (JsonValor* argumenta, Piscina* piscina,
    vacuum* datum, chorda* culpa)
{
    JsonValor* fructus = json_objectum_creare(piscina);

    (vacuum)argumenta;
    (vacuum)datum;
    (vacuum)culpa;
    (vacuum)remove(FORUM_TERGALE_VIA);
    json_objectum_ponere(fructus, "bene",
        json_boolean_creare(piscina, VERUM));
    redde fructus;
}

/* ==================================================
 * servus ad telephonum (QR + processus alter)
 * ================================================== */

/* infra definita (tessera ante servum legi debet) */
interior constans character* _tesseram_parare (Piscina* piscina);

/* QR ex URL in JSON: amplitudo + ordines ut chordae '0'/'1'.
 * Velamen matricem pingit; C eam computat. */
interior JsonValor*
_qr_ad_json (constans character* url, Piscina* piscina)
{
    QR         qr;
    JsonValor* obiectum;
    JsonValor* ordines;
    chorda     datum;
    s32        y, x;

    datum = chorda_ex_literis(url, piscina);
    /* gradus M: aequilibrium inter densitatem et tolerantiam -
     * camera telephoni velamen oblique legit */
    qr = qr_generare(datum, QR_ECC_M, piscina);
    si (!qr.successus)
    {
        redde NIHIL;
    }
    obiectum = json_objectum_creare(piscina);
    json_objectum_ponere(obiectum, "amplitudo",
        json_integer_creare(piscina, (s64)qr.amplitudo));
    json_objectum_ponere(obiectum, "versio",
        json_integer_creare(piscina, (s64)qr.versio));

    ordines = json_tabulatum_creare(piscina);
    per (y = ZEPHYRUM; y < (s32)qr.amplitudo; y++)
    {
        character linea[128];

        per (x = ZEPHYRUM; x < (s32)qr.amplitudo; x++)
        {
            linea[x] = (qr_modulus(&qr, x, y) == I) ? '1' : '0';
        }
        linea[qr.amplitudo] = '\0';
        json_tabulatum_addere(ordines,
            json_chorda_creare_literis(piscina, linea));
    }
    json_objectum_ponere(obiectum, "ordines", ordines);
    redde obiectum;
}

/* status servi + candidati cum QR suo quisque.
 * QR unum non damus quia addressum unum non est (vide rete.h):
 * si duae interfacies supersunt, homo oculis eligit. */
interior JsonValor*
_servus_status (JsonValor* argumenta, Piscina* piscina,
    vacuum* datum, chorda* culpa)
{
    ForumStatus* f = (ForumStatus*)datum;
    JsonValor*   fructus;
    JsonValor*   candidati;
    b32          currit;

    (vacuum)argumenta;
    (vacuum)culpa;

    currit = (f->servus_proc != NIHIL);
    fructus = json_objectum_creare(piscina);
    json_objectum_ponere(fructus, "currens",
        json_boolean_creare(piscina, currit));
    json_objectum_ponere(fructus, "portus",
        json_integer_creare(piscina, (s64)f->servus_portus));

    candidati = json_tabulatum_creare(piscina);
    si (currit)
    {
        constans character* tessera = _tesseram_parare(piscina);
        ReteInterfacies     tabulatum[VIII];
        i32                 numerus;
        i32                 i;

        numerus = rete_addressus_locales(tabulatum, VIII);
        per (i = ZEPHYRUM; i < numerus && tessera != NIHIL; i++)
        {
            character  url[512];
            JsonValor* ordo = json_objectum_creare(piscina);
            JsonValor* qr;

            sprintf(url, "http://%s:%d/?clavis=%s",
                tabulatum[i].addressum, (int)f->servus_portus,
                tessera);
            json_objectum_ponere(ordo, "interfacies",
                json_chorda_creare_literis(piscina,
                    tabulatum[i].titulus));
            json_objectum_ponere(ordo, "addressum",
                json_chorda_creare_literis(piscina,
                    tabulatum[i].addressum));
            json_objectum_ponere(ordo, "url",
                json_chorda_creare_literis(piscina, url));
            qr = _qr_ad_json(url, piscina);
            si (qr != NIHIL)
            {
                json_objectum_ponere(ordo, "qr", qr);
            }
            json_tabulatum_addere(candidati, ordo);
        }
    }
    json_objectum_ponere(fructus, "candidati", candidati);
    redde fructus;
}

interior JsonValor*
_servus_incipere (JsonValor* argumenta, Piscina* piscina,
    vacuum* datum, chorda* culpa)
{
    ForumStatus*        f = (ForumStatus*)datum;
    constans character* argv[VII];
    character           portus_txt[XVI];
    i32                 portus = 8790;

    si (f->servus_proc != NIHIL)
    {
        *culpa = _ch_forum("servus iam currit");
        redde NIHIL;
    }
    si (argumenta != NIHIL)
    {
        JsonValor* p = json_objectum_capere(argumenta, "portus");

        si (p != NIHIL && json_est_integer(p))
        {
            portus = (i32)json_ad_integer(p);
        }
    }
    si (_tesseram_parare(piscina) == NIHIL)
    {
        *culpa = _ch_forum("tessera parari non potuit");
        redde NIHIL;
    }
    sprintf(portus_txt, "%d", (int)portus);

    /* PROCESSUS ALTER, non filum: ansa vitreae et ansa hospitii
     * numquam intertexuntur. mora ZEPHYRUM = infinitus (servus
     * currere debet donec sistatur). */
    argv[ZEPHYRUM] = "bin/forum";
    argv[I]        = "-servire";
    argv[II]       = portus_txt;
    argv[III]      = "-hospes";
    argv[IV]       = "0.0.0.0";
    argv[V]        = NIHIL;

    f->servus_proc = processus_incipere(argv, ZEPHYRUM,
        f->piscina_diuturna);
    si (f->servus_proc == NIHIL)
    {
        *culpa = _ch_forum("processus incipi non potuit");
        redde NIHIL;
    }
    f->servus_portus = portus;
    /* status verum per _servus_status petatur: hic solum
     * confirmamus incepisse. Pagina statum interrogat postquam
     * servus ligare potuit. */
    {
        JsonValor* fructus = json_objectum_creare(piscina);

        json_objectum_ponere(fructus, "currens",
            json_boolean_creare(piscina, VERUM));
        json_objectum_ponere(fructus, "portus",
            json_integer_creare(piscina, (s64)portus));
        redde fructus;
    }
}

interior JsonValor*
_servus_sistere (JsonValor* argumenta, Piscina* piscina,
    vacuum* datum, chorda* culpa)
{
    ForumStatus* f = (ForumStatus*)datum;
    JsonValor*   fructus = json_objectum_creare(piscina);

    (vacuum)argumenta;
    (vacuum)culpa;

    si (f->servus_proc != NIHIL)
    {
        processus_abrumpere(f->servus_proc);
        f->servus_proc = NIHIL;
        f->servus_portus = ZEPHYRUM;
    }
    json_objectum_ponere(fructus, "currens",
        json_boolean_creare(piscina, FALSUM));
    redde fructus;
}

/* Tesseram legere aut gignere. Lima EXTRA arborem git, modo 0600.
 * Secretum in repositorio numquam sedeat; haec lima est sutura per
 * quam arca (01KYAMMMF58F) postea succedet.
 * Redde NIHIL si gignere non potuit (fons fortuitorum defecit) -
 * vocans REFUSAT potius quam clavem divinabilem adhibeat. */
interior constans character*
_tesseram_parare (Piscina* piscina)
{
    constans character* domus = getenv("HOME");
    character  via[1024];
    character* tessera;
    FILE*      f;
    i8         octeti[32];
    i32        i;

    si (domus == NIHIL)
    {
        redde NIHIL;
    }
    sprintf(via, "%s/.rhubarb", domus);
    (vacuum)mkdir(via, 0700);
    sprintf(via, "%s/.rhubarb/forum.tessera", domus);

    tessera = (character*)piscina_allocare(piscina, 65);
    si (tessera == NIHIL)
    {
        redde NIHIL;
    }

    f = fopen(via, "r");
    si (f != NIHIL)
    {
        si (fgets(tessera, 65, f) != NIHIL)
        {
            fclose(f);
            tessera[strcspn(tessera, "\r\n")] = '\0';
            si (strlen(tessera) >= XXXII)
            {
                redde tessera;
            }
        }
        alioquin
        {
            fclose(f);
        }
    }

    /* gignere: LXIV characteres hex = CCLVI bits */
    si (!moneta_octeti_fortuiti(octeti, XXXII))
    {
        imprimere("[forum] tessera gigni NON potuit"
            " (/dev/urandom deest) - servus recusat\n");
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < XXXII; i++)
    {
        sprintf(tessera + i * II, "%02x",
            (unsigned int)(i8)octeti[i]);
    }
    tessera[64] = '\0';

    f = fopen(via, "w");
    si (f == NIHIL)
    {
        redde NIHIL;
    }
    fprintf(f, "%s\n", tessera);
    fclose(f);
    (vacuum)chmod(via, 0600);
    imprimere("[forum] tessera nova scripta: %s\n", via);
    redde tessera;
}

/* PRAEBITOR: registratio methodorum fori - SEMEL scripta, ab
 * utroque transportu vocata (fenestra vitreae, servus HTTP).
 * Methodus hic addita in telephono statim adest quia locus alter
 * ubi obliviscaris non est.
 *
 * Modus PUBLICUS harnesium fumi excludit: methodi illae fenestram
 * et probationem regunt, et super socket nihil quaerunt. */
interior vacuum
_methodos_praebere (Internuntius* inx, InternuntiusModus modus,
    vacuum* datum)
{
    ForumStatus* f = (ForumStatus*)datum;

    (vacuum)internuntius_praebere(inx, "res_legere",
        _res_legere, f);
    (vacuum)internuntius_praebere(inx, "mittere",
        _mittere, f);
    (vacuum)internuntius_praebere(inx, "articulum_servare",
        _articulum_servare, f);
    (vacuum)internuntius_praebere(inx, "status_ponere",
        _status_ponere, f);
    (vacuum)internuntius_praebere(inx, "mutare",
        _mutare, f);
    (vacuum)internuntius_praebere(inx, "delere",
        _delere, f);
    (vacuum)internuntius_praebere(inx, "transmittere",
        cliens_tabularii_transmittere, &f->cliens);
    (vacuum)internuntius_praebere(inx, "sententias_parsare",
        _sententias_parsare, f);
    (vacuum)internuntius_praebere(inx, "capitula_parsare",
        _capitula_parsare, f);
    (vacuum)internuntius_praebere(inx, "tergale_ponere",
        _tergale_ponere, f);
    (vacuum)internuntius_praebere(inx, "tergale_capere",
        _tergale_capere, f);
    (vacuum)internuntius_praebere(inx, "tergale_delere",
        _tergale_delere, f);

    si (modus == INTERNUNTIUS_MODUS_LOCALIS)
    {
        (vacuum)internuntius_praebere(inx, "fumus_modus",
            _fumus_modus, f);
        (vacuum)internuntius_praebere(inx, "fumus_perfectus",
            _fumus_perfectus, f);
        /* servus ad telephonum: FENESTRAE SOLIUS. Servus qui se
         * ipsum gignere posset furcam infinitam pareret, et
         * telephonum servum alium incipere nihil quaerit. */
        (vacuum)internuntius_praebere(inx, "servus_status",
            _servus_status, f);
        (vacuum)internuntius_praebere(inx, "servus_incipere",
            _servus_incipere, f);
        (vacuum)internuntius_praebere(inx, "servus_sistere",
            _servus_sistere, f);
    }
}

s32 principale (integer argc, character** argv)
{
    Piscina* piscina = piscina_generare_dynamicum("forum",
        16777216);
    Piscina* piscina_vocationis = piscina_generare_dynamicum(
        "forum_vocationes", 8388608);
    ForumStatus forum;
    b32 fumus = FALSUM;
    i32 portus_servi = ZEPHYRUM;        /* 0 = modus fenestrae */
    constans character* hospes_servi = NIHIL;  /* NIHIL = loopback */
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
    forum.servus_proc = NIHIL;
    forum.servus_portus = ZEPHYRUM;
    forum.piscina_diuturna = piscina;
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
        alioquin si (strcmp(argv[k], "-servire") == ZEPHYRUM
            && k + I < argc)
        {
            portus_servi = (i32)atoi(argv[k + I]);
            k++;
        }
        alioquin si (strcmp(argv[k], "-hospes") == ZEPHYRUM
            && k + I < argc)
        {
            /* expositio in reticulum ACTUS EXPLICITUS: sine hoc
             * servus loopback solum audit */
            hospes_servi = argv[k + I];
            k++;
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

    /* MODUS SERVI: transportus alter, applicatio eadem. Fenestra
     * nulla, WebKit nullum - ergo haec semita in Linux quoque
     * vivit. Processus alter de industria (ansae eventuum duae
     * numquam intertexuntur). */
    si (portus_servi > ZEPHYRUM)
    {
        VitreaServusConfiguratio figura_servi;
        VitreaServus* servus;
        Capsula* capsula_servi = capsula_aperire(&capsula_forum,
            piscina);

        si (capsula_servi == NIHIL)
        {
            imprimere("FRACTA: capsula\n");
            redde I;
        }
        memset(&figura_servi, ZEPHYRUM, magnitudo(figura_servi));
        figura_servi.capsula         = capsula_servi;
        figura_servi.via_initialis   = "index.html";
        figura_servi.praebitor       = _methodos_praebere;
        figura_servi.praebitor_datum = &forum;
        figura_servi.hospes          = hospes_servi;
        /* tessera SEMPER paratur, etiam in loopback: ergo QR idem
         * est utrovis modo, et transitus ad -hospes nihil mutat */
        figura_servi.tessera         = _tesseram_parare(piscina);
        si (hospes_servi != NIHIL && figura_servi.tessera == NIHIL)
        {
            imprimere("FRACTA: tessera necessaria ad expositionem\n");
            redde I;
        }
        figura_servi.portus          = portus_servi;
        figura_servi.acta_accessus   = VERUM;

        servus = vitrea_servus_creare(piscina, &figura_servi);
        si (servus == NIHIL)
        {
            imprimere("FRACTA: servus (portus %d occupatus?)\n",
                (int)portus_servi);
            redde I;
        }
        imprimere("[forum] servus http://%s:%d/ (daemon portus %d)\n",
            hospes_servi ? hospes_servi : "127.0.0.1",
            (int)vitrea_servus_portus(servus),
            (int)forum.cliens.portus);
        fflush(stdout);
        vitrea_servus_currere(servus);
        vitrea_servus_destruere(servus);
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
    _methodos_praebere(inx, INTERNUNTIUS_MODUS_LOCALIS, &forum);

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
            /* servum pulsare: NUMQUAM OBSTAT (select mora zephyri).
             * Servus vivus CURRIT manet; si obiit (portus occupatus,
             * ruina), PARATUS fit et manubrium purgamus, ne facies
             * de servo mortuo mentiatur. */
            si (forum.servus_proc != NIHIL
                && processus_pulsare(forum.servus_proc)
                    == PROCESSUS_PARATUS)
            {
                (vacuum)processus_metere(forum.servus_proc);
                forum.servus_proc = NIHIL;
                forum.servus_portus = ZEPHYRUM;
                imprimere("[forum] servus ad telephonum cessavit\n");
                fflush(stdout);
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

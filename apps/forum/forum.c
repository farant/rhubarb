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

#include "postulata_posix.h"
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
#include "stml.h"
#include "filum.h"
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
     * dato pleno petit. Absentia utriusque = mos vetus exacte.
     *
     * CAVE TRANSMISSIONEM: hic argumenta NOMINATIM eliguntur, non
     * transfunduntur. Argumentum novum quod hic non additur TACITE
     * cadit - velamen rectum, daemon rectus, filtrum nullum. Quod
     * peius est quam error: cum consumptor selectionem clientis
     * latere DEPOSUERIT (capitula), lectio non-filtrata omnia
     * reddit et ut datum verum legitur. */
    {
        JsonValor* arg = json_objectum_creare(piscina);
        chorda sine_campis;
        chorda res_unum;
        chorda nexus_verbum;
        chorda nexus_ad;

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
        nexus_verbum = json_ad_chorda(json_objectum_capere(argumenta,
            "nexus_verbum"));
        si (nexus_verbum.mensura > ZEPHYRUM)
        {
            json_objectum_ponere(arg, "nexus_verbum",
                json_chorda_creare(piscina, nexus_verbum));
        }
        nexus_ad = json_ad_chorda(json_objectum_capere(argumenta,
            "nexus_ad"));
        si (nexus_ad.mensura > ZEPHYRUM)
        {
            json_objectum_ponere(arg, "nexus_ad",
                json_chorda_creare(piscina, nexus_ad));
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

/* ==================================================
 * CHARTA - censum legere: proiectio graphi censūs pro tabula
 * infinita (spec: project-specs/charta-spec.md). LEGENDI SOLUM -
 * census veritas manet, haec via numquam scribit.
 * ================================================== */

#define CHARTA_CENSUS_VIA "rhubarb.census"
#define CHARTA_SEMINA_VIA "natura/cocta/semina.census"
#define CHARTA_CANON_VIA  "natura/cocta/individua.canon"

/* nodus natus, cum ansis ad quas postea additur */
nomen structura {
    chorda     clavis;     /* citatio verbatim: '&x;' */
    chorda     genus;
    chorda     stml;       /* fons verbatim (emissio byte-fidelis) */
    JsonValor* obiectum;
    JsonValor* notae;
} ChartaNodus;

/* semen externum: citatio -> genus + fons (e semina.census) */
nomen structura {
    chorda clavis;
    chorda genus;
    chorda stml;
} ChartaSemen;

/* citatio-ne? '&x;' (individuum), '.x' (genus), '#x' (locus
 * documenti) - sigilla librarii. Ceterum valor verbatim est. */
interior b32
_charta_citatio_est (chorda valor)
{
    si (valor.mensura < II)
    {
        redde FALSUM;
    }
    si (valor.datum[ZEPHYRUM] == '&')
    {
        redde valor.datum[valor.mensura - I] == ';';
    }
    redde valor.datum[ZEPHYRUM] == '.'
        || valor.datum[ZEPHYRUM] == '#';
}

/* signum unius limae: mtime.magnitudo.inodus - inodus servationem
 * atomicam (lima nova eadem secunda) capit, quam mtime solum
 * perderet */
interior vacuum
_charta_signum_limae (constans character* via, character* exitus)
{
    structura stat status;

    si (stat(via, &status) == ZEPHYRUM)
    {
        sprintf(exitus, "%ld.%ld.%lu",
            (longus)status.st_mtime,
            (longus)status.st_size,
            (insignatus longus)status.st_ino);
    }
    alioquin
    {
        sprintf(exitus, "x");
    }
}

/* signum trium limarum in exitus (>= 160 octeti). FALSUM = census
 * ipse abest - sine eo nihil pingendum est. */
interior b32
_charta_signum_componere (character* exitus)
{
    structura stat probatio;
    character  s1[48];
    character  s2[48];
    character  s3[48];

    si (stat(CHARTA_CENSUS_VIA, &probatio) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    _charta_signum_limae(CHARTA_CENSUS_VIA, s1);
    _charta_signum_limae(CHARTA_SEMINA_VIA, s2);
    _charta_signum_limae(CHARTA_CANON_VIA, s3);
    sprintf(exitus, "%s-%s-%s", s1, s2, s3);
    redde VERUM;
}

/* vitium proiectionis: responsum plenum, NON culpa RPC - cliens
 * inter tubulum fractum et censum non-parsabilem (servatio media)
 * distinguere debet: graphum bonum ultimum retinet, vexillum
 * monstrat, tela numquam vacua fit (spec par. VIII) */
interior JsonValor*
_charta_vitium (Piscina* piscina, constans character* textus)
{
    JsonValor* fructus = json_objectum_creare(piscina);

    json_objectum_ponere(fructus, "vitium",
        json_chorda_creare_literis(piscina, textus));
    redde fructus;
}

/* radix elementaris documenti (nodum documenti transit) */
interior StmlNodus*
_charta_elementum_radicis (StmlNodus* documentum)
{
    i32 i;
    i32 numerus;

    si (documentum == NIHIL)
    {
        redde NIHIL;
    }
    si (documentum->genus == STML_NODUS_ELEMENTUM)
    {
        redde documentum;
    }
    numerus = stml_numerus_liberorum(documentum);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        StmlNodus* liber = stml_liberum_ad_indicem(documentum, i);

        si (liber != NIHIL && liber->genus == STML_NODUS_ELEMENTUM)
        {
            redde liber;
        }
    }
    redde NIHIL;
}

/* semina externa: citatio -> genus. Defectus NON letalis - monitum
 * apponitur et stipites 'ignotum' manent (numquam tacite). */
interior Xar*
_charta_semina_legere (Piscina* piscina, JsonValor* monita)
{
    Xar*         semina;
    chorda       fons;
    StmlResultus resultus;
    StmlNodus*   radix;
    i32          i;
    i32          numerus;

    semina = xar_creare(piscina, magnitudo(ChartaSemen));
    fons = filum_legere_totum(CHARTA_SEMINA_VIA, piscina);
    si (fons.mensura == ZEPHYRUM)
    {
        json_tabulatum_addere(monita, json_chorda_creare_literis(
            piscina, "semina non lecta - externi 'ignotum' erunt"));
        redde semina;
    }
    resultus = stml_legere(fons, piscina,
        internamentum_creare(piscina));
    si (!resultus.successus || resultus.radix == NIHIL)
    {
        json_tabulatum_addere(monita, json_chorda_creare_literis(
            piscina, "semina non parsata - externi 'ignotum' erunt"));
        redde semina;
    }
    radix = _charta_elementum_radicis(resultus.radix);
    numerus = radix ? stml_numerus_liberorum(radix) : ZEPHYRUM;
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        StmlNodus* liber = stml_liberum_ad_indicem(radix, i);
        chorda*    clavis_attr;

        si (liber == NIHIL || liber->genus != STML_NODUS_ELEMENTUM
            || liber->titulus == NIHIL)
        {
            perge;
        }
        clavis_attr = stml_attributum_capere(liber, "nomen");
        si (clavis_attr != NIHIL)
        {
            ChartaSemen* semen = (ChartaSemen*)xar_addere(semina);

            semen->clavis = *clavis_attr;
            semen->genus  = *liber->titulus;
            /* fons verbatim - tabella chartae eum stipiti externo
             * monstrat ('unde hoc venit' sine semina aperiendis) */
            semen->stml   = stml_scribere(liber, piscina, FALSUM);
        }
    }
    redde semina;
}

interior ChartaSemen*
_charta_semen_invenire (Xar* semina, chorda clavis)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(semina); i++)
    {
        ChartaSemen* semen = (ChartaSemen*)xar_obtinere(semina, i);

        si (semen != NIHIL && chorda_aequalis(semen->clavis, clavis))
        {
            redde semen;
        }
    }
    redde NIHIL;
}

interior ChartaNodus*
_charta_nodum_invenire (Xar* tabula, chorda clavis)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(tabula); i++)
    {
        ChartaNodus* nodus = (ChartaNodus*)xar_obtinere(tabula, i);

        si (nodus != NIHIL && chorda_aequalis(nodus->clavis, clavis))
        {
            redde nodus;
        }
    }
    redde NIHIL;
}

/* CAVE: index in tabulam redditus tantum valet dum xar_addere in
 * eandem tabulam non iterum vocatur (tabula crescens migrat) */
interior ChartaNodus*
_charta_nodum_creare (Xar* tabula, JsonValor* nodi, chorda clavis,
    chorda genus, b32 alienus, Piscina* piscina)
{
    ChartaNodus* nodus = (ChartaNodus*)xar_addere(tabula);

    nodus->clavis   = clavis;
    nodus->genus    = genus;
    nodus->stml.mensura = ZEPHYRUM;
    nodus->stml.datum   = NIHIL;
    nodus->obiectum = json_objectum_creare(piscina);
    nodus->notae    = json_tabulatum_creare(piscina);
    json_objectum_ponere(nodus->obiectum, "genus",
        json_chorda_creare(piscina, genus));
    json_objectum_ponere(nodus->obiectum, "externus",
        json_boolean_creare(piscina, alienus));
    json_objectum_ponere(nodus->obiectum, "notae", nodus->notae);
    json_objectum_ponere_chorda(nodi, clavis, nodus->obiectum);
    redde nodus;
}

interior vacuum
_charta_aristam_addere (JsonValor* aristae, Xar* scopi,
    chorda a, chorda ad, chorda familia, Piscina* piscina)
{
    JsonValor* arista = json_objectum_creare(piscina);
    chorda*    scopus = (chorda*)xar_addere(scopi);

    *scopus = ad;
    json_objectum_ponere(arista, "a",
        json_chorda_creare(piscina, a));
    json_objectum_ponere(arista, "ad",
        json_chorda_creare(piscina, ad));
    json_objectum_ponere(arista, "familia",
        json_chorda_creare(piscina, familia));
    json_tabulatum_addere(aristae, arista);
}

/* entitas una censūs -> nodus + aristae. Elementum '%' est
 * augmentatio librarii: clavis destinata nomen est, contentum
 * additivum (notae, aristae) nodo externo apponitur.
 * Attributa: valor-citatio -> arista singularis; ceterum verbatim.
 * Liberi: nota/glossa -> prosa; ad= -> arista; ceterum numeratur
 * (praetermissa - nihil tacite cadit). */
interior vacuum
_charta_entitatem_legere (StmlNodus* elementum, Xar* tabula,
    Xar* semina, JsonValor* nodi, JsonValor* aristae, Xar* scopi,
    i32* praetermissa, Piscina* piscina)
{
    chorda       clavis;
    ChartaNodus* nodus;
    JsonValor*   attributa = NIHIL;
    i32          i;
    i32          numerus;

    si (elementum == NIHIL
        || elementum->genus != STML_NODUS_ELEMENTUM
        || elementum->titulus == NIHIL)
    {
        redde;   /* commenta, textus albus - non numerantur */
    }

    si (chorda_aequalis_literis(*elementum->titulus, "%"))
    {
        si (elementum->augmentum_clavis == NIHIL)
        {
            (*praetermissa)++;
            redde;
        }
        clavis = *elementum->augmentum_clavis;
        nodus = _charta_nodum_invenire(tabula, clavis);
        si (nodus == NIHIL)
        {
            ChartaSemen* semen = _charta_semen_invenire(semina,
                clavis);
            chorda genus;

            si (semen != NIHIL)
            {
                genus = semen->genus;
            }
            alioquin
            {
                genus = chorda_ex_literis("ignotum", piscina);
            }
            nodus = _charta_nodum_creare(tabula, nodi, clavis,
                genus, VERUM, piscina);
            si (semen != NIHIL)
            {
                nodus->stml = semen->stml;
            }
        }
    }
    alioquin
    {
        chorda* clavis_attr = stml_attributum_capere(elementum,
            "nomen");

        si (clavis_attr == NIHIL)
        {
            (*praetermissa)++;
            redde;
        }
        clavis = *clavis_attr;
        nodus = _charta_nodum_invenire(tabula, clavis);
        si (nodus == NIHIL)
        {
            nodus = _charta_nodum_creare(tabula, nodi, clavis,
                *elementum->titulus, FALSUM, piscina);
        }
    }

    /* fons verbatim apponitur (emissio byte-fidelis, non-pulchra):
     * entitas + augmentationes eiusdem clavis omnes colliguntur */
    {
        chorda emissio = stml_scribere(elementum, piscina, FALSUM);

        si (nodus->stml.mensura == ZEPHYRUM)
        {
            nodus->stml = emissio;
        }
        alioquin
        {
            nodus->stml = chorda_concatenare(
                chorda_concatenare(nodus->stml, _ch_forum("\n\n"),
                    piscina),
                emissio, piscina);
        }
    }

    numerus = elementum->attributa
        ? xar_numerus(elementum->attributa) : ZEPHYRUM;
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        StmlAttributum* attr = (StmlAttributum*)xar_obtinere(
            elementum->attributa, i);

        si (attr == NIHIL || attr->titulus == NIHIL
            || attr->valor == NIHIL
            || chorda_aequalis_literis(*attr->titulus, "nomen"))
        {
            perge;
        }
        si (_charta_citatio_est(*attr->valor))
        {
            _charta_aristam_addere(aristae, scopi, clavis,
                *attr->valor, *attr->titulus, piscina);
        }
        alioquin
        {
            si (attributa == NIHIL)
            {
                attributa = json_objectum_creare(piscina);
                json_objectum_ponere(nodus->obiectum, "attributa",
                    attributa);
            }
            json_objectum_ponere_chorda(attributa, *attr->titulus,
                json_chorda_creare(piscina, *attr->valor));
        }
    }

    numerus = stml_numerus_liberorum(elementum);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        StmlNodus* liber = stml_liberum_ad_indicem(elementum, i);
        chorda*    ad;

        si (liber == NIHIL || liber->genus != STML_NODUS_ELEMENTUM
            || liber->titulus == NIHIL)
        {
            perge;
        }
        si (chorda_aequalis_literis(*liber->titulus, "nota"))
        {
            json_tabulatum_addere(nodus->notae,
                json_chorda_creare(piscina,
                    stml_textus_normalizatus(liber, piscina)));
            perge;
        }
        si (chorda_aequalis_literis(*liber->titulus, "glossa"))
        {
            chorda* lingua = stml_attributum_capere(liber, "lingua");

            si ((lingua == NIHIL
                    || chorda_aequalis_literis(*lingua, "en"))
                && json_objectum_capere(nodus->obiectum, "glossa")
                    == NIHIL)
            {
                json_objectum_ponere(nodus->obiectum, "glossa",
                    json_chorda_creare(piscina,
                        stml_textus_normalizatus(liber, piscina)));
            }
            perge;
        }
        ad = stml_attributum_capere(liber, "ad");
        si (ad != NIHIL)
        {
            _charta_aristam_addere(aristae, scopi, clavis, *ad,
                *liber->titulus, piscina);
            perge;
        }
        (*praetermissa)++;
    }
}

/* glossa Anglica generis e canone monolitho. Octeti in piscina
 * canonis vivunt - vocans transcribat antequam piscina pereat. */
interior chorda
_charta_glossam_generis (StmlNodus* radix_canonis, chorda genus,
    Piscina* piscina_canonis)
{
    chorda vacua;
    i32    i;
    i32    numerus = stml_numerus_liberorum(radix_canonis);

    vacua.mensura = ZEPHYRUM;
    vacua.datum   = NIHIL;
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        StmlNodus* liber = stml_liberum_ad_indicem(radix_canonis, i);
        chorda*    clavis_attr;
        i32        j;
        i32        n;

        si (liber == NIHIL || liber->genus != STML_NODUS_ELEMENTUM
            || liber->titulus == NIHIL
            || !chorda_aequalis_literis(*liber->titulus, "elementum"))
        {
            perge;
        }
        clavis_attr = stml_attributum_capere(liber, "nomen");
        si (clavis_attr == NIHIL
            || !chorda_aequalis(*clavis_attr, genus))
        {
            perge;
        }
        n = stml_numerus_liberorum(liber);
        per (j = ZEPHYRUM; j < n; j++)
        {
            StmlNodus* g = stml_liberum_ad_indicem(liber, j);
            chorda*    lingua;

            si (g == NIHIL || g->genus != STML_NODUS_ELEMENTUM
                || g->titulus == NIHIL
                || !chorda_aequalis_literis(*g->titulus, "glossa"))
            {
                perge;
            }
            lingua = stml_attributum_capere(g, "lingua");
            si (lingua != NIHIL
                && chorda_aequalis_literis(*lingua, "en"))
            {
                redde stml_textus_normalizatus(g, piscina_canonis);
            }
        }
        redde vacua;   /* elementum inventum, glossa Anglica abest */
    }
    redde vacua;
}

/* glossae generum e canone monolitho (846 KB) apponere. Piscina
 * PROPRIA effimera - piscinam vocationis (VIII MB) arbor canonis
 * saturaret (OOM plagularum immanium notum, 01KYQ4H5ZG). Parsatio
 * sola in reaedificatione plena currit - interrogationes signo
 * breviantur. Numerus appositarum redditur - porta fumi >0 poscit. */
interior i32
_charta_glossas_apponere (Xar* tabula, JsonValor* monita,
    Piscina* piscina)
{
    Piscina*     piscina_canonis;
    chorda       fons;
    StmlResultus resultus;
    StmlNodus*   radix;
    i32          appositae = ZEPHYRUM;
    i32          i;

    piscina_canonis = piscina_generare_dynamicum("charta_canon",
        8388608);
    si (piscina_canonis == NIHIL)
    {
        json_tabulatum_addere(monita, json_chorda_creare_literis(
            piscina,
            "piscina canonis deest - glossae generum absunt"));
        redde ZEPHYRUM;
    }
    resultus.successus = FALSUM;
    resultus.radix = NIHIL;
    fons = filum_legere_totum(CHARTA_CANON_VIA, piscina_canonis);
    si (fons.mensura > ZEPHYRUM)
    {
        resultus = stml_legere(fons, piscina_canonis,
            internamentum_creare(piscina_canonis));
    }
    si (!resultus.successus || resultus.radix == NIHIL)
    {
        json_tabulatum_addere(monita, json_chorda_creare_literis(
            piscina, "canon non lectus - glossae generum absunt"));
        piscina_destruere(piscina_canonis);
        redde ZEPHYRUM;
    }
    radix = _charta_elementum_radicis(resultus.radix);
    per (i = ZEPHYRUM; radix != NIHIL && i < xar_numerus(tabula); i++)
    {
        ChartaNodus* nodus = (ChartaNodus*)xar_obtinere(tabula, i);
        chorda       glossa;

        si (nodus == NIHIL)
        {
            perge;
        }
        glossa = _charta_glossam_generis(radix, nodus->genus,
            piscina_canonis);
        si (glossa.mensura > ZEPHYRUM)
        {
            /* transcribere: octeti piscinae canonis mox pereunt */
            json_objectum_ponere(nodus->obiectum, "glossa_generis",
                json_chorda_creare(piscina,
                    chorda_transcribere(glossa, piscina)));
            appositae++;
        }
    }
    piscina_destruere(piscina_canonis);
    redde appositae;
}

/* censum_legere {signum?} -> graphum censūs, aut {mutatum:falsum}
 * (signum aequale - tres stat, nulla parsatio), aut {vitium}
 * (census abest/non parsatur - cliens graphum ultimum retinet).
 * Involucrum scaenae: {scaena, signum, nodi, aristae, ...} -
 * porta effugii nidificationis (spec par. III). */
interior JsonValor*
_censum_legere (JsonValor* argumenta, Piscina* piscina,
    vacuum* datum, chorda* culpa)
{
    character    signum_novum[192];
    chorda       signum_vetus;
    chorda       fons;
    StmlResultus resultus;
    StmlNodus*   radix;
    JsonValor*   fructus;
    JsonValor*   nodi;
    JsonValor*   aristae;
    JsonValor*   monita;
    Xar*         tabula;
    Xar*         semina;
    Xar*         scopi;
    i32          praetermissa = ZEPHYRUM;
    i32          glossae_generum;
    i32          i;
    i32          numerus;

    (vacuum)datum;
    (vacuum)culpa;   /* vitium in fructu, non culpa RPC (spec II.6) */

    si (!_charta_signum_componere(signum_novum))
    {
        redde _charta_vitium(piscina,
            "rhubarb.census abest (stat)");
    }
    signum_vetus.mensura = ZEPHYRUM;
    signum_vetus.datum   = NIHIL;
    si (argumenta != NIHIL)
    {
        signum_vetus = json_ad_chorda(json_objectum_capere(
            argumenta, "signum"));
    }
    si (signum_vetus.mensura > ZEPHYRUM
        && chorda_aequalis_literis(signum_vetus, signum_novum))
    {
        fructus = json_objectum_creare(piscina);
        json_objectum_ponere(fructus, "mutatum",
            json_boolean_creare(piscina, FALSUM));
        redde fructus;
    }

    fons = filum_legere_totum(CHARTA_CENSUS_VIA, piscina);
    si (fons.mensura == ZEPHYRUM)
    {
        redde _charta_vitium(piscina, "rhubarb.census legi nequit");
    }
    resultus = stml_legere(fons, piscina,
        internamentum_creare(piscina));
    si (!resultus.successus || resultus.radix == NIHIL)
    {
        redde _charta_vitium(piscina,
            "census non parsatur (servatio media?)");
    }
    radix = _charta_elementum_radicis(resultus.radix);
    si (radix == NIHIL)
    {
        redde _charta_vitium(piscina, "census sine radice");
    }

    fructus = json_objectum_creare(piscina);
    nodi    = json_objectum_creare(piscina);
    aristae = json_tabulatum_creare(piscina);
    monita  = json_tabulatum_creare(piscina);
    tabula  = xar_creare(piscina, magnitudo(ChartaNodus));
    scopi   = xar_creare(piscina, magnitudo(chorda));
    semina  = _charta_semina_legere(piscina, monita);

    numerus = stml_numerus_liberorum(radix);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        _charta_entitatem_legere(stml_liberum_ad_indicem(radix, i),
            tabula, semina, nodi, aristae, scopi, &praetermissa,
            piscina);
    }

    /* stipites: scopi extra censum minti (semina aut ignoti) -
     * arista sine nodo utroque telae nihil diceret */
    per (i = ZEPHYRUM; i < xar_numerus(scopi); i++)
    {
        chorda*      scopus = (chorda*)xar_obtinere(scopi, i);
        ChartaSemen* semen;
        ChartaNodus* stipes;
        chorda       genus;
        chorda       clavis;

        si (scopus == NIHIL)
        {
            perge;
        }
        clavis = *scopus;
        si (_charta_nodum_invenire(tabula, clavis) != NIHIL)
        {
            perge;
        }
        semen = _charta_semen_invenire(semina, clavis);
        si (semen != NIHIL)
        {
            genus = semen->genus;
        }
        alioquin si (clavis.datum[ZEPHYRUM] == '.')
        {
            genus = chorda_ex_literis("genus", piscina);
        }
        alioquin si (clavis.datum[ZEPHYRUM] == '#')
        {
            genus = chorda_ex_literis("locus", piscina);
        }
        alioquin
        {
            genus = chorda_ex_literis("ignotum", piscina);
        }
        stipes = _charta_nodum_creare(tabula, nodi, clavis, genus,
            VERUM, piscina);
        si (semen != NIHIL)
        {
            stipes->stml = semen->stml;
        }
    }

    /* fons verbatim in datum (post ambulationem totam: entitas
     * cum augmentationibus suis iam collecta est) */
    per (i = ZEPHYRUM; i < xar_numerus(tabula); i++)
    {
        ChartaNodus* nodus = (ChartaNodus*)xar_obtinere(tabula, i);

        si (nodus != NIHIL && nodus->stml.mensura > ZEPHYRUM)
        {
            json_objectum_ponere(nodus->obiectum, "stml",
                json_chorda_creare(piscina, nodus->stml));
        }
    }

    glossae_generum = _charta_glossas_apponere(tabula, monita,
        piscina);

    json_objectum_ponere(fructus, "scaena",
        json_chorda_creare_literis(piscina, "census"));
    json_objectum_ponere(fructus, "signum",
        json_chorda_creare_literis(piscina, signum_novum));
    json_objectum_ponere(fructus, "nodi", nodi);
    json_objectum_ponere(fructus, "aristae", aristae);
    json_objectum_ponere(fructus, "glossae_generum",
        json_integer_creare(piscina, (s64)glossae_generum));
    json_objectum_ponere(fructus, "praetermissa",
        json_integer_creare(piscina, (s64)praetermissa));
    json_objectum_ponere(fructus, "monita", monita);
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
    (vacuum)internuntius_praebere(inx, "censum_legere",
        _censum_legere, f);
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
        /* charta: graphum censūs contra limas VERAS (lex
         * conditorii veri - fixturae assumpta communia habent) */
        {
            chorda     culpa_ch;
            JsonValor* graphum;
            JsonValor* nodi_f;
            JsonValor* aristae_f;
            JsonValor* iterum;
            JsonValor* arg_signum;

            culpa_ch.mensura = ZEPHYRUM;
            culpa_ch.datum   = NIHIL;
            graphum = _censum_legere(NIHIL, piscina, &forum,
                &culpa_ch);
            si (graphum == NIHIL
                || json_objectum_capere(graphum, "vitium") != NIHIL)
            {
                imprimere("[forum] fumus charta FRACTUS: vitium\n");
                redde I;
            }
            nodi_f    = json_objectum_capere(graphum, "nodi");
            aristae_f = json_objectum_capere(graphum, "aristae");
            si (json_objectum_numerus(nodi_f) == ZEPHYRUM
                || json_tabulatum_numerus(aristae_f) == ZEPHYRUM
                || json_ad_integer(json_objectum_capere(graphum,
                       "glossae_generum")) == (s64)ZEPHYRUM)
            {
                imprimere("[forum] fumus charta FRACTUS: graphum"
                    " vacuum aut glossae absunt\n");
                redde I;
            }
            /* signum stabile: limae eaedem -> mutatum falsum */
            arg_signum = json_objectum_creare(piscina);
            json_objectum_ponere(arg_signum, "signum",
                json_objectum_capere(graphum, "signum"));
            iterum = _censum_legere(arg_signum, piscina, &forum,
                &culpa_ch);
            si (iterum == NIHIL
                || json_objectum_capere(iterum, "mutatum") == NIHIL
                || json_ad_boolean(json_objectum_capere(iterum,
                       "mutatum")))
            {
                imprimere("[forum] fumus charta FRACTUS: signum"
                    " instabile\n");
                redde I;
            }
            imprimere("[forum] fumus charta: %d nodi, %d aristae,"
                " %d glossae generum, %d praetermissa\n",
                (int)json_objectum_numerus(nodi_f),
                (int)json_tabulatum_numerus(aristae_f),
                (int)json_ad_integer(json_objectum_capere(graphum,
                    "glossae_generum")),
                (int)json_ad_integer(json_objectum_capere(graphum,
                    "praetermissa")));
            /* semita glossae ENTIS contra conditorium verum
             * (nuntius, non porta - glossae veniunt et eunt) */
            {
                JsonValor* piscina_n = json_objectum_capere(nodi_f,
                    "&piscina;");
                chorda     fons_stml;

                imprimere("[forum] fumus charta: glossa entis"
                    " piscinae %s\n",
                    (piscina_n != NIHIL
                     && json_objectum_capere(piscina_n, "glossa")
                        != NIHIL) ? "ADEST" : "abest");
                /* fons verbatim PORTA est: ens censu mintum sine
                 * stml = structura fracta, non contentum mobile */
                fons_stml = json_ad_chorda(piscina_n != NIHIL
                    ? json_objectum_capere(piscina_n, "stml")
                    : NIHIL);
                si (fons_stml.mensura == ZEPHYRUM)
                {
                    imprimere("[forum] fumus charta FRACTUS:"
                        " fons stml abest\n");
                    redde I;
                }
            }
        }
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

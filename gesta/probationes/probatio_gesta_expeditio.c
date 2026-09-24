/* probatio_gesta_expeditio.c - expeditiones: plicatura et validatio
 * (expeditio v1 T1; spec project-specs/expeditio-spec.md)
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "json.h"
#include "credo.h"
#include "gesta_expeditio.h"
#include <stdio.h>
#include <string.h>

/* obiectum JSON ex litteris (NIHIL si fractum) */
interior JsonValor*
_json (
    constans character* litterae,
               Piscina* pn)
{
    JsonResultus r = json_legere_literis(litterae, pn);

    redde r.successus ? r.radix : NIHIL;
}

/* eventum plicare (actor 'claude', creatum datum) */
interior b32
_plicare (
             JsonValor* status,
    constans character* genus,
    constans character* datum,
    constans character* creatum,
               Piscina* pn)
{
    redde gesta_expeditio_applicare(status, chorda_ex_literis(genus,
        pn),
        _json(datum, pn), chorda_ex_literis("claude", pn),
        chorda_ex_literis(creatum, pn), pn);
}

/* causae validationis (NIHIL = licet) */
interior constans character*
_validare (
             JsonValor* status,
    constans character* genus,
    constans character* datum,
               Piscina* pn)
{
    redde gesta_expeditio_validare(status, chorda_ex_literis(genus, pn),
        _json(datum, pn), pn);
}

/* numerus causarum ("  - " per lineam) */
interior i32
_causae (
    constans character* textus)
{
    constans character* p = textus;
                   i32  n = ZEPHYRUM;

    si (p == NIHIL)
    {
        redde ZEPHYRUM;
    }
    dum ((p = strstr(p, "  - ")) != NIHIL)
    {
        n++;
        p += IV;
    }
    redde n;
}

/* gradus ordinis (NIHIL si nullus) */
interior JsonValor*
_gradus (
             JsonValor* status,
    constans character* ordo)
{
    JsonValor* g = json_objectum_capere(status, "gradus");

    redde g != NIHIL ? json_objectum_capere(g, ordo) : NIHIL;
}

/* campus chordae gradus ordinis */
interior chorda
_campus_gradus (
             JsonValor* status,
    constans character* ordo,
    constans character* clavis)
{
    redde json_ad_chorda(json_objectum_capere(_gradus(status, ordo),
        clavis));
}

/* versio rubricae in gradu ordinis (0 si nulla) */
interior s64
_versio_gradus (
             JsonValor* status,
    constans character* ordo)
{
    JsonValor* v = json_objectum_capere(_gradus(status, ordo),
        "versio");

    redde v != NIHIL && json_est_integer(v) ? json_ad_integer(v)
        : (s64)ZEPHYRUM;
}

s32 principale (vacuum)
{
      Piscina* pn;
    JsonValor* status;

    pn = piscina_generare_dynamicum("probatio_gesta_expeditio", 65536);
    si (!pn)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(pn);
    status = json_objectum_creare(pn);


    /* ==================================================
     * GENERA EVENTUUM
     * ================================================== */

    imprimere("\n--- Probans genera eventuum ---\n");
    CREDO_VERUM (gesta_expeditio_eventus_est(
        chorda_ex_literis("expeditio-photographia", pn)));
    CREDO_VERUM (gesta_expeditio_eventus_est(
        chorda_ex_literis("rubrica-mutata", pn)));
    CREDO_VERUM (gesta_expeditio_eventus_est(
        chorda_ex_literis("gradus-positus", pn)));
    CREDO_VERUM (gesta_expeditio_eventus_est(
        chorda_ex_literis("gradus-promotus", pn)));
    CREDO_VERUM (gesta_expeditio_eventus_est(
        chorda_ex_literis("ordines-additi", pn)));
    CREDO_FALSUM (gesta_expeditio_eventus_est(
        chorda_ex_literis("cella-posita", pn)));


    /* ==================================================
     * PHOTOGRAPHIA: ordines, rubrica v1, filtrum, implenda
     * ================================================== */

    {
        JsonValor* ordines;
        JsonValor* rubrica;

        imprimere("\n--- Probans photographiam ---\n");
        CREDO_NIHIL (_validare(status, "expeditio-photographia",
            "{\"inventarium\":\"01INVENTARIUMFICTUM\",\"ordines\":[\""
            "a.sh\",\"b.sh\",\"c.sh\"],\"rubrica\":\"sors_lcg_classic"
            "us si casus fixi\",\"filtrum\":{\"lens\":\"migratum\",\""
            "op\":\"!=\",\"valor\":\"ita\"},\"implenda\":{\"lens\":\""
            "migratum\",\"genus\":\"ita-non\",\"valor\":\"ita\"}}",
            pn));
        CREDO_VERUM (_plicare(status, "expeditio-photographia",
            "{\"inventarium\":\"01INVENTARIUMFICTUM\",\"ordines\":[\""
            "a.sh\",\"b.sh\",\"c.sh\"],\"rubrica\":\"sors_lcg_classic"
            "us si casus fixi\",\"filtrum\":{\"lens\":\"migratum\",\""
            "op\":\"!=\",\"valor\":\"ita\"},\"implenda\":{\"lens\":\""
            "migratum\",\"genus\":\"ita-non\",\"valor\":\"ita\"}}",
            "2026-09-24T10:00:00Z", pn));
        ordines = json_objectum_capere(status, "ordines");
        CREDO_AEQUALIS_I32 (json_tabulatum_numerus(ordines), III);
        CREDO_CHORDA_AEQUALIS_LITERIS (json_ad_chorda(
            json_objectum_capere(json_tabulatum_obtinere(ordines, II),
                "clavis")), "c.sh");
        CREDO_CHORDA_AEQUALIS_LITERIS (json_ad_chorda(
            json_objectum_capere(json_tabulatum_obtinere(ordines,
                ZEPHYRUM), "additus")), "2026-09-24T10:00:00Z");
        rubrica = json_objectum_capere(status, "rubrica");
        CREDO_AEQUALIS_S64 (json_ad_integer(json_objectum_capere(
            rubrica, "versio")), (s64)I);
        CREDO_CHORDA_AEQUALIS_LITERIS (json_ad_chorda(
            json_objectum_capere(rubrica, "textus")),
            "sors_lcg_classicus si casus fixi");
        CREDO_CHORDA_AEQUALIS_LITERIS (json_ad_chorda(
            json_objectum_capere(json_objectum_capere(status,
                "implenda"), "valor")), "ita");
        CREDO_CHORDA_AEQUALIS_LITERIS (json_ad_chorda(
            json_objectum_capere(json_objectum_capere(status,
                "filtrum"), "op")), "!=");
        CREDO_CHORDA_AEQUALIS_LITERIS (json_ad_chorda(
            json_objectum_capere(status, "inventarium")),
            "01INVENTARIUMFICTUM");
        /* photographia secunda: plicatura praetermittit, validatio
         * recusat */
        CREDO_FALSUM (_plicare(status, "expeditio-photographia",
            "{\"inventarium\":\"01INVENTARIUMFICTUM\",\"ordines\":[\""
            "x.sh\"],\"rubrica\":\"sors_lcg_classicus si casus fixi\""
            ",\"filtrum\":{\"lens\":\"migratum\",\"op\":\"!=\",\"valo"
            "r\":\"ita\"},\"implenda\":{\"lens\":\"migratum\",\"genus"
            "\":\"ita-non\",\"valor\":\"ita\"}}",
            "2026-09-24T10:01:00Z", pn));
        CREDO_AEQUALIS_I32 (json_tabulatum_numerus(json_objectum_capere(
            status, "ordines")), III);
        CREDO_AEQUALIS_I32 (_causae(_validare(status,
            "expeditio-photographia",
            "{\"inventarium\":\"01INVENTARIUMFICTUM\",\"ordines\":[\""
            "x.sh\"],\"rubrica\":\"sors_lcg_classicus si casus fixi\""
            ",\"filtrum\":{\"lens\":\"migratum\",\"op\":\"!=\",\"valo"
            "r\":\"ita\"},\"implenda\":{\"lens\":\"migratum\",\"genus"
            "\":\"ita-non\",\"valor\":\"ita\"}}", pn)), I);
    }


    /* ==================================================
     * GRADUS: versio rubricae CURRENS in quoque gradu
     * ================================================== */

    {
        imprimere("\n--- Probans gradus et versiones rubricae ---\n");
        CREDO_VERUM (_plicare(status, "gradus-positus",
            "{\"ordines\":[\"a.sh\",\"b.sh\"],\"status\":\"factum\","
            "\"per\":\"abc1234\"}",
            "2026-09-24T11:00:00Z", pn));
        CREDO_CHORDA_AEQUALIS_LITERIS (_campus_gradus(status, "a.sh",
            "status"), "factum");
        CREDO_CHORDA_AEQUALIS_LITERIS (_campus_gradus(status, "b.sh",
            "per"), "abc1234");
        CREDO_CHORDA_AEQUALIS_LITERIS (_campus_gradus(status, "a.sh",
            "creatum"), "2026-09-24T11:00:00Z");
        CREDO_CHORDA_AEQUALIS_LITERIS (_campus_gradus(status, "a.sh",
            "actor"), "claude");
        CREDO_AEQUALIS_S64 (_versio_gradus(status, "a.sh"), (s64)I);
        CREDO_VERUM (_plicare(status, "rubrica-mutata",
            "{\"textus\":\"sors_lcg_classicus si casus fixi; aliter s"
            "ors_intra\"}", "2026-09-24T12:00:00Z", pn));
        CREDO_AEQUALIS_S64 (json_ad_integer(json_objectum_capere(
            json_objectum_capere(status, "rubrica"), "versio")),
            (s64)II);
        CREDO_VERUM (_plicare(status, "gradus-positus",
            "{\"ordines\":[\"c.sh\"],\"status\":\"factum\"}",
            "2026-09-24T12:30:00Z", pn));
        CREDO_AEQUALIS_S64 (_versio_gradus(status, "c.sh"), (s64)II);
        CREDO_AEQUALIS_S64 (_versio_gradus(status, "a.sh"), (s64)I);
        /* reaperire */
        CREDO_VERUM (_plicare(status, "gradus-positus",
            "{\"ordines\":[\"a.sh\"],\"status\":\"apertum\"}", "2026-"
            "09-24T13:00:00Z", pn));
        CREDO_CHORDA_AEQUALIS_LITERIS (_campus_gradus(status, "a.sh",
            "status"), "apertum");
        /* plicatura defensiva: ordo ignotus praetermittitur */
        (vacuum)_plicare(status, "gradus-positus",
            "{\"ordines\":[\"z.sh\"],\"status\":\"factum\"}", "2026-0"
            "9-24T13:10:00Z", pn);
        CREDO_NIHIL (_gradus(status, "z.sh"));
    }


    /* ==================================================
     * INCREMENTUM ET PROMOTIO
     * ================================================== */

    {
        imprimere("\n--- Probans incrementum et promotionem ---\n");
        CREDO_NIHIL (_validare(status, "ordines-additi",
            "{\"ordines\":[\"d.sh\"]}", pn));
        CREDO_VERUM (_plicare(status, "ordines-additi",
            "{\"ordines\":[\"d.sh\"]}", "2026-09-24T14:00:00Z", pn));
        CREDO_AEQUALIS_I32 (json_tabulatum_numerus(json_objectum_capere(
            status, "ordines")), IV);
        CREDO_AEQUALIS_I32 (_causae(_validare(status, "ordines-additi",
            "{\"ordines\":[\"a.sh\"]}", pn)), I);
        CREDO_NIHIL (_validare(status, "gradus-promotus",
            "{\"ordo\":\"d.sh\",\"opus\":\"01OPUSFICTUM\"}", pn));
        CREDO_VERUM (_plicare(status, "gradus-promotus",
            "{\"ordo\":\"d.sh\",\"opus\":\"01OPUSFICTUM\"}", "2026-09"
            "-24T14:10:00Z", pn));
        CREDO_CHORDA_AEQUALIS_LITERIS (_campus_gradus(status, "d.sh",
            "status"), "promotum");
        CREDO_CHORDA_AEQUALIS_LITERIS (_campus_gradus(status, "d.sh",
            "opus"), "01OPUSFICTUM");
        /* gradus factus post promotionem: opus servatur */
        CREDO_VERUM (_plicare(status, "gradus-positus",
            "{\"ordines\":[\"d.sh\"],\"status\":\"factum\",\"per\":\""
            "def5678\"}", "2026-09-24T15:00:00Z", pn));
        CREDO_CHORDA_AEQUALIS_LITERIS (_campus_gradus(status, "d.sh",
            "status"), "factum");
        CREDO_CHORDA_AEQUALIS_LITERIS (_campus_gradus(status, "d.sh",
            "opus"), "01OPUSFICTUM");
        /* promotio ordinis iam facti recusatur */
        CREDO_AEQUALIS_I32 (_causae(_validare(status, "gradus-promotus",
            "{\"ordo\":\"b.sh\",\"opus\":\"01OPUSALTERUM\"}", pn)), I);
    }


    /* ==================================================
     * VALIDATIO: causae omnes simul
     * ================================================== */

    {
        constans character* c;

        imprimere("\n--- Probans validationem ---\n");
        /* omissum sine nota */
        c = _validare(status, "gradus-positus",
            "{\"ordines\":[\"c.sh\"],\"status\":\"omissum\"}", pn);
        CREDO_AEQUALIS_I32 (_causae(c), I);
        CREDO_VERUM (c != NIHIL && strstr(c, "nota") != NIHIL);
        CREDO_NIHIL (_validare(status, "gradus-positus",
            "{\"ordines\":[\"c.sh\"],\"status\":\"omissum\",\"nota\":"
            "\"sine generatore\"}", pn));
        /* ordines duo ignoti + status pravus = tres causae */
        c = _validare(status, "gradus-positus",
            "{\"ordines\":[\"q.sh\",\"r.sh\"],\"status\":\"forsitan\""
            "}", pn);
        CREDO_AEQUALIS_I32 (_causae(c), III);
        CREDO_AEQUALIS_I32 (_causae(_validare(status, "rubrica-mutata",
            "{\"textus\":\"\"}", pn)), I);
        CREDO_AEQUALIS_I32 (_causae(_validare(status, "gradus-positus",
            "{\"ordines\":[],\"status\":\"factum\"}", pn)), I);
        CREDO_AEQUALIS_I32 (_causae(_validare(status, "mutatio", "{}",
            pn)), I);
    }

    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}

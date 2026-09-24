/* probatio_gesta_inventarium.c - inventaria: plicatura et validatio
 * (inventarium v1 T1; spec project-specs/inventarium-spec.md)
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "json.h"
#include "credo.h"
#include "gesta_inventarium.h"
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

/* eventum plicare (actor 'claude', creatum fixum) */
interior b32
_plicare (
             JsonValor* status,
    constans character* genus,
    constans character* datum,
               Piscina* pn)
{
    redde gesta_inventarium_applicare(status, chorda_ex_literis(genus,
        pn),
        _json(datum, pn), chorda_ex_literis("claude", pn),
        chorda_ex_literis("2026-09-23T12:00:00Z", pn), pn);
}

/* causae validationis (NIHIL = licet) */
interior constans character*
_validare (
             JsonValor* status,
    constans character* genus,
    constans character* datum,
               Piscina* pn)
{
    redde gesta_inventarium_validare(status, chorda_ex_literis(genus,
        pn),
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

s32 principale (vacuum)
{
      Piscina* pn;
    JsonValor* status;

    pn = piscina_generare_dynamicum("probatio_gesta_inventarium",
        65536);
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
    CREDO_VERUM (gesta_inventarium_eventus_est(
        chorda_ex_literis("ordo-additus", pn)));
    CREDO_VERUM (gesta_inventarium_eventus_est(
        chorda_ex_literis("cella-posita", pn)));
    CREDO_FALSUM (gesta_inventarium_eventus_est(
        chorda_ex_literis("mutatio", pn)));


    /* ==================================================
     * PLICATURA: ordines, lentes, cellae
     * ================================================== */

    {
        JsonValor* ordines;
        JsonValor* cella;

        imprimere("\n--- Probans plicaturam ---\n");
        CREDO_VERUM (_plicare(status, "ordo-additus",
            "{\"ordines\":[\"a.sh\",\"b.sh\"]}", pn));
        /* duplicatum praetermittitur (defensiva), novum additur */
        CREDO_VERUM (_plicare(status, "ordo-additus",
            "{\"ordines\":[\"a.sh\",\"c.sh\"]}", pn));
        ordines = json_objectum_capere(status, "ordines");
        CREDO_AEQUALIS_I32 (json_tabulatum_numerus(ordines), III);
        CREDO_CHORDA_AEQUALIS_LITERIS (json_ad_chorda(
            json_objectum_capere(json_tabulatum_obtinere(ordines, II),
                "clavis")), "c.sh");
        CREDO_CHORDA_AEQUALIS_LITERIS (json_ad_chorda(
            json_objectum_capere(json_tabulatum_obtinere(ordines,
            ZEPHYRUM),
                "actor")), "claude");

        CREDO_VERUM (_plicare(status, "lens-addita",
            "{\"nomen\":\"in PORTAE\",\"genus_valoris\":\"ita-non\"}",
            pn));
        CREDO_VERUM (_plicare(status, "lens-addita",
            "{\"nomen\":\"tegit\",\"genus_valoris\":\"textus\"}", pn));
        CREDO_FALSUM (_plicare(status, "lens-addita",
            "{\"nomen\":\"tegit\",\"genus_valoris\":\"textus\"}", pn));

        CREDO_VERUM (_plicare(status, "cella-posita",
            "{\"fons\":\"derivatum\",\"per\":\"opus X\",\"cellae\":["
            "{\"ordo\":\"a.sh\",\"lens\":\"in PORTAE\",\"valor\":"
            "{\"genus\":\"ita-non\",\"valor\":\"ita\"}},"
            "{\"ordo\":\"nullus.sh\",\"lens\":\"tegit\",\"valor\":"
            "{\"genus\":\"textus\",\"valor\":\"x\"}}]}", pn));
        cella = json_objectum_capere(json_objectum_capere(
            json_objectum_capere(status, "cellae"), "a.sh"),
            "in PORTAE");
        CREDO_NON_NIHIL (cella);
        CREDO_CHORDA_AEQUALIS_LITERIS (json_ad_chorda(
            json_objectum_capere(cella, "valor")), "ita");
        CREDO_CHORDA_AEQUALIS_LITERIS (json_ad_chorda(
            json_objectum_capere(cella, "fons")), "derivatum");
        CREDO_CHORDA_AEQUALIS_LITERIS (json_ad_chorda(
            json_objectum_capere(cella, "per")), "opus X");
        CREDO_CHORDA_AEQUALIS_LITERIS (json_ad_chorda(
            json_objectum_capere(cella, "creatum")),
            "2026-09-23T12:00:00Z");
        /* cella ordinis absentis praetermissa */
        CREDO_NIHIL (json_objectum_capere(json_objectum_capere(status,
            "cellae"), "nullus.sh"));
        /* superscriptio: valor posterior vincit */
        CREDO_VERUM (_plicare(status, "cella-posita",
            "{\"fons\":\"manu\",\"cellae\":[{\"ordo\":\"a.sh\","
            "\"lens\":\"in PORTAE\",\"valor\":{\"genus\":\"ita-non\","
            "\"valor\":\"non\"}}]}", pn));
        cella = json_objectum_capere(json_objectum_capere(
            json_objectum_capere(status, "cellae"), "a.sh"),
            "in PORTAE");
        CREDO_CHORDA_AEQUALIS_LITERIS (json_ad_chorda(
            json_objectum_capere(cella, "valor")), "non");
        CREDO_NIHIL (json_objectum_capere(cella, "per"));
    }


    /* ==================================================
     * REMOTIO: ordo et cellae eius abeunt
     * ================================================== */

    {
        imprimere("\n--- Probans remotionem ---\n");
        CREDO_VERUM (_plicare(status, "ordo-remotus",
            "{\"ordines\":[\"a.sh\"],\"causa\":\"deletum\"}", pn));
        CREDO_AEQUALIS_I32 (json_tabulatum_numerus(
            json_objectum_capere(status, "ordines")), II);
        CREDO_NIHIL (json_objectum_capere(json_objectum_capere(status,
            "cellae"), "a.sh"));
    }


    /* ==================================================
     * VALIDATIO: causae omnes simul
     * ================================================== */

    {
        constans character* c;

        imprimere("\n--- Probans validationem ---\n");
        CREDO_NIHIL (_validare(status, "ordo-additus",
            "{\"ordines\":[\"d.sh\"]}", pn));
        c = _validare(status, "ordo-additus",
            "{\"ordines\":[\"b.sh\",\"x.sh\",\"x.sh\"]}", pn);
        CREDO_AEQUALIS_I32 (_causae(c), II);
        CREDO_VERUM (c != NIHIL
            && strstr(c, "'b.sh' iam adest") != NIHIL);
        c = _validare(status, "ordo-remotus",
            "{\"ordines\":[\"z.sh\"]}", pn);
        CREDO_AEQUALIS_I32 (_causae(c), II);
        c = _validare(status, "lens-addita",
            "{\"nomen\":\"tegit\",\"genus_valoris\":\"numerus\"}", pn);
        CREDO_AEQUALIS_I32 (_causae(c), II);
        CREDO_NIHIL (_validare(status, "cella-posita",
            "{\"fons\":\"manu\",\"cellae\":[{\"ordo\":\"b.sh\","
            "\"lens\":\"tegit\",\"valor\":{\"genus\":\"textus\","
            "\"valor\":\"radix\"}}]}", pn));
        /* fons malus + ordo absens + lens absens + ita-non malum */
        c = _validare(status, "cella-posita",
            "{\"fons\":\"coniectura\",\"cellae\":["
            "{\"ordo\":\"q.sh\",\"lens\":\"nulla\",\"valor\":"
            "{\"genus\":\"textus\",\"valor\":\"x\"}},"
            "{\"ordo\":\"b.sh\",\"lens\":\"in PORTAE\",\"valor\":"
            "{\"genus\":\"ita-non\",\"valor\":\"fortasse\"}}]}", pn);
        CREDO_AEQUALIS_I32 (_causae(c), IV);
        /* genus valoris lenti non congruens */
        c = _validare(status, "cella-posita",
            "{\"fons\":\"manu\",\"cellae\":[{\"ordo\":\"b.sh\","
            "\"lens\":\"tegit\",\"valor\":{\"genus\":\"ita-non\","
            "\"valor\":\"ita\"}}]}", pn);
        CREDO_AEQUALIS_I32 (_causae(c), I);
        c = _validare(status, "mutatio", "{}", pn);
        CREDO_AEQUALIS_I32 (_causae(c), I);
    }


    /* ==================================================
     * NON APPLICABILE: valor expressus in OMNI lente (decretum
     * ...SD7JR, optio 1) - genus tertium, causa optionalis in valore
     * ================================================== */

    {
        constans character* c;
                 JsonValor* cella;

        imprimere("\n--- Probans non applicabile ---\n");
        /* in lente ita-non ET in lente textus: genus lentis non
         * obstat */
        CREDO_NIHIL (_validare(status, "cella-posita",
            "{\"fons\":\"manu\",\"cellae\":[{\"ordo\":\"b.sh\","
            "\"lens\":\"in PORTAE\",\"valor\":{\"genus\":"
            "\"non-applicabile\"}},{\"ordo\":\"c.sh\",\"lens\":"
            "\"tegit\",\"valor\":{\"genus\":\"non-applicabile\","
            "\"valor\":\"GUI solum\"}}]}", pn));
        /* causa non chorda recusatur */
        c = _validare(status, "cella-posita",
            "{\"fons\":\"manu\",\"cellae\":[{\"ordo\":\"b.sh\","
            "\"lens\":\"tegit\",\"valor\":{\"genus\":"
            "\"non-applicabile\",\"valor\":7}}]}", pn);
        CREDO_AEQUALIS_I32 (_causae(c), I);
        CREDO_VERUM (c != NIHIL
            && strstr(c, "non-applicabile") != NIHIL);
        /* plicatura: genus et causa servantur */
        CREDO_VERUM (_plicare(status, "cella-posita",
            "{\"fons\":\"manu\",\"cellae\":[{\"ordo\":\"c.sh\","
            "\"lens\":\"tegit\",\"valor\":{\"genus\":"
            "\"non-applicabile\",\"valor\":\"GUI solum\"}}]}", pn));
        cella = json_objectum_capere(json_objectum_capere(
            json_objectum_capere(status, "cellae"), "c.sh"), "tegit");
        CREDO_CHORDA_AEQUALIS_LITERIS (json_ad_chorda(
            json_objectum_capere(cella, "genus")), "non-applicabile");
        CREDO_CHORDA_AEQUALIS_LITERIS (json_ad_chorda(
            json_objectum_capere(cella, "valor")), "GUI solum");
    }

    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}

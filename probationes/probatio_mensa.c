/* probatio_mensa.c - Probationes Mensae (plica actorum -> status) */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "volumen.h"
#include "mensa.h"
#include "json.h"
#include "credo.h"

#include <stdio.h>

#define VIA_PROBATIONIS "build/probatio_mensa.db"

interior s64
_actum (Volumen* vol, constans character* genus,
    constans character* datum, Piscina* piscina);

interior s64
_actum (Volumen* vol, constans character* genus,
    constans character* datum, Piscina* piscina)
{
    redde volumen_actum_appendere(vol, genus,
        chorda_ex_literis(datum, piscina));
}

s32 principale (vacuum)
{
    b32      praeteritus;
    Piscina* piscina;
    Volumen* vol;

    piscina = piscina_generare_dynamicum("probatio_mensa", 262144);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    si (filum_existit(VIA_PROBATIONIS))
    {
        filum_delere(VIA_PROBATIONIS);
    }
    vol = volumen_creare(piscina, VIA_PROBATIONIS);

    /* ========================================================
     * PROBARE: volumen_acta_legere - post_seq exclusivum
     * ======================================================== */

    {
        Xar*          acta;
        VolumenActum* a;

        imprimere("\n--- Probans volumen_acta_legere ---\n");

        _actum(vol, "probatio", "{\"n\":1}", piscina);
        _actum(vol, "probatio", "{\"n\":2}", piscina);

        acta = volumen_acta_legere(vol, 0, piscina);
        CREDO_NON_NIHIL(acta);
        /* volumen-creatum + duo probationis */
        CREDO_AEQUALIS_I32((i32)xar_numerus(acta), (i32)3);

        acta = volumen_acta_legere(vol, 2, piscina);
        CREDO_AEQUALIS_I32((i32)xar_numerus(acta), (i32)1);
        a = (VolumenActum*)xar_obtinere(acta, 0);
        CREDO_AEQUALIS_S64(a->seq, (s64)3);
        CREDO_CHORDA_AEQUALIS_LITERIS(a->genus, "probatio");
        CREDO_CHORDA_AEQUALIS_LITERIS(a->datum, "{\"n\":2}");
        CREDO_VERUM(a->momentum.mensura > ZEPHYRUM);
    }

    /* ========================================================
     * PROBARE: plica - creatum + fusio + deletum + aliena
     * ======================================================== */

    {
        chorda       status;
        JsonResultus lectum;
        JsonValor*   elementa;
        JsonValor*   elem;

        imprimere("\n--- Probans plicam mensae ---\n");

        _actum(vol, "mensa:creatum",
            "{\"id\":\"scida-a\",\"genus_elementi\":\"scida\","
            "\"x\":10,\"y\":20}", piscina);
        _actum(vol, "mensa:positum",
            "{\"id\":\"scida-a\",\"x\":55,\"y\":60}", piscina);
        _actum(vol, "mensa:creatum",
            "{\"id\":\"scidula-b\",\"genus_elementi\":\"scidula\","
            "\"x\":5,\"y\":5,\"textus\":\"salve\"}", piscina);
        _actum(vol, "mensa:textus",
            "{\"id\":\"scidula-b\",\"textus\":\"emendatum\"}",
            piscina);
        _actum(vol, "mensa:creatum",
            "{\"id\":\"scida-c\",\"x\":1,\"y\":1}", piscina);
        _actum(vol, "mensa:deletum", "{\"id\":\"scida-c\"}",
            piscina);
        /* aliena + corrupta: plica toleret */
        _actum(vol, "probatio", "{\"id\":\"scida-a\",\"x\":999}",
            piscina);
        _actum(vol, "mensa:positum", "non json", piscina);
        _actum(vol, "mensa:positum", "{\"sine_id\":1}", piscina);

        status = mensa_statum_fingere(vol, piscina);
        CREDO_VERUM(status.mensura > ZEPHYRUM);

        lectum = json_legere(status, piscina);
        CREDO_VERUM(lectum.successus);
        elementa = json_objectum_capere(lectum.radix, "elementa");
        CREDO_NON_NIHIL(elementa);

        /* scida-a: positio ULTIMA vincit, genus_elementi manet */
        elem = json_objectum_capere(elementa, "scida-a");
        CREDO_NON_NIHIL(elem);
        CREDO_AEQUALIS_S64(json_ad_integer(
            json_objectum_capere(elem, "x")), (s64)55);
        CREDO_AEQUALIS_S64(json_ad_integer(
            json_objectum_capere(elem, "y")), (s64)60);
        CREDO_CHORDA_AEQUALIS_LITERIS(json_ad_chorda(
            json_objectum_capere(elem, "genus_elementi")), "scida");

        /* scidula-b: textus superscriptus */
        elem = json_objectum_capere(elementa, "scidula-b");
        CREDO_NON_NIHIL(elem);
        CREDO_CHORDA_AEQUALIS_LITERIS(json_ad_chorda(
            json_objectum_capere(elem, "textus")), "emendatum");

        /* scida-c: deleta - abest */
        elem = json_objectum_capere(elementa, "scida-c");
        CREDO_NIHIL(elem);
    }

    /* ========================================================
     * PROBARE: resurrectio post deletum
     * ======================================================== */

    {
        chorda       status;
        JsonResultus lectum;
        JsonValor*   elem;

        imprimere("\n--- Probans resurrectionem ---\n");

        _actum(vol, "mensa:creatum",
            "{\"id\":\"scida-c\",\"x\":7,\"y\":8}", piscina);
        status = mensa_statum_fingere(vol, piscina);
        lectum = json_legere(status, piscina);
        CREDO_VERUM(lectum.successus);
        elem = json_objectum_capere(
            json_objectum_capere(lectum.radix, "elementa"),
            "scida-c");
        CREDO_NON_NIHIL(elem);
        CREDO_AEQUALIS_S64(json_ad_integer(
            json_objectum_capere(elem, "x")), (s64)7);
    }

    volumen_claudere(vol);

    /* ========================================================
     * Compendium
     * ======================================================== */

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}

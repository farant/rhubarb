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

/* volumen in area sectionis (credo_area): olim build/probatio_mensa.db
 * extra vestigium membri (-journal per VFS vestigii notatum) */
interior constans character*
_via_probationis (
    CredoContextus* c);

interior constans character*
_via_probationis (
    CredoContextus* c)
{
    redde chorda_ut_cstr(chorda_concatenare(chorda_ex_literis(
        credo_area(c), c->piscina), chorda_ex_literis(
        "/probatio_mensa.db", c->piscina), c->piscina), c->piscina);
}

interior s64
_actum (
               Volumen* vol,
    constans character* genus,
    constans character* datum,
               Piscina* piscina);

interior s64
_actum (
               Volumen* vol,
    constans character* genus,
    constans character* datum,
               Piscina* piscina)
{
    redde volumen_actum_appendere(vol, genus,
        chorda_ex_literis(datum, piscina));
}


/* Volumen recens in area sua: quaeque sectio suum aperit et
 * claudit (sectiones ordine liberae, credo v2) */


/* ========================================================
 * PROBARE: volumen_acta_legere - post_seq exclusivum
 * ======================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    Volumen* vol;

    piscina  = c->piscina;
    vol      = volumen_creare(piscina, _via_probationis(c));

    {
                 Xar* acta;
        VolumenActum* a;

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

    volumen_claudere(vol);
}


/* ========================================================
 * PROBARE: plica - creatum + fusio + deletum + aliena
 * ======================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    Volumen* vol;

    piscina  = c->piscina;
    vol      = volumen_creare(piscina, _via_probationis(c));

    {
              chorda  status;
        JsonResultus  lectum;
           JsonValor* elementa;
           JsonValor* elem;

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

    volumen_claudere(vol);
}


/* ========================================================
 * PROBARE: resurrectio post deletum
 * ======================================================== */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    Volumen* vol;

    piscina  = c->piscina;
    vol      = volumen_creare(piscina, _via_probationis(c));

    {
              chorda  status;
        JsonResultus  lectum;
           JsonValor* elem;

        /* scida-c creata et deleta (ut in plica supra) - volumen
         * recens, ergo deletio hic iteratur */
        _actum(vol, "mensa:creatum",
            "{\"id\":\"scida-c\",\"x\":1,\"y\":1}", piscina);
        _actum(vol, "mensa:deletum", "{\"id\":\"scida-c\"}",
            piscina);

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
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "volumen_acta_legere", _sectio_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "plicam mensae", _sectio_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "resurrectionem", _sectio_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("mensa", SECTIONES);
}

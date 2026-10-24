/* probatio_carmen.c - Probationes carminis (include/carmen.h)
 *
 * legere: mappae (tempus, metrum cum mutatione, clavis), partes,
 * frusta per mensuram (metro mutato) et pulsus, notae in pulsibus intra
 * frustum cum orthographia, longitudo carminis. vitia: carmen, pars,
 * frustum, offset et causa. ordinarium: liber infixus, korobeiniki.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "carmen.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define CARMEN_PROBAE \
    "<carmina><carmen titulus=\"proba\" tempus=\"90\" metrum=\"4/4\" " \
    "clavis=\"d maior\">" \
    "<metrum mensura=\"3\" valor=\"3/4\"/>" \
    "<tempus mensura=\"2\" valor=\"180\"/>" \
    "<clavis mensura=\"3\" valor=\"bb minor\"/>" \
    "<pars titulus=\"a\" instrumentum=\"quadrans\" lucrum=\"0.5\">" \
    "<frustum mensura=\"1\"><textus!>l4 c# d- e r</textus></frustum>" \
    "<frustum mensura=\"4\"><textus!>l4 g a b</textus></frustum>" \
    "</pars>" \
    "<pars titulus=\"b\" instrumentum=\"triangulum\" mutus solus>" \
    "<frustum pulsus=\"480\"><textus!>l8 c</textus></frustum>" \
    "</pars></carmen></carmina>"

interior chorda
_ch (
        CredoContextus* c,
    constans character* litterae)
{
    redde chorda_ex_literis(litterae, c->piscina);
}

interior b32
_legere (
        CredoContextus* c,
    constans character* stml,
               Carmina* liber)
{
    CarminisVitium vitium;
               b32 bene;

    bene = carmina_legere(_ch(c, stml), c->piscina, liber, &vitium);
    si (!bene)
    {
        imprimere("  liber: '%.*s' '%.*s' %d %d: %s\n",
            (integer)vitium.carmen.mensura,
            (constans character*)vitium.carmen.datum,
            (integer)vitium.pars.mensura,
            (constans character*)vitium.pars.datum,
            (integer)vitium.frustum, (integer)vitium.offset,
            vitium.causa ? vitium.causa : "(nulla)");
    }
    redde bene;
}

/* liber unius carminis "x" cum parte "a" et corpore dato */
interior constans character*
_carmen (
        CredoContextus* c,
    constans character* attributa,
    constans character* corpus)
{
    memoriae_index  n = strlen(attributa) + strlen(corpus) + CXXVIII;
         character* b = (character*)piscina_allocare(c->piscina, n);

    sprintf(b, "<carmina><carmen titulus=\"x\"%s>%s</carmen></carmina>",
        attributa, corpus);
    redde b;
}

/* FALSUM, et carmen, pars, frustum, offset dati */
interior vacuum
_vitium (
        CredoContextus* c,
    constans character* stml,
    constans character* carmen,
    constans character* pars,
                    s32  frustum,
                    s32  offset)
{
           Carmina liber;
    CarminisVitium vitium;
               b32 bene;

    memset(&vitium, ZEPHYRUM, magnitudo(vitium));
    vitium.offset = CMXCIX;
    bene = carmina_legere(_ch(c, stml), c->piscina, &liber, &vitium);
    CREDO_FALSUM (bene);
    CREDO_CHORDA_AEQUALIS_LITERIS (vitium.carmen, carmen);
    CREDO_CHORDA_AEQUALIS_LITERIS (vitium.pars, pars);
    CREDO_AEQUALIS_S32 ((s32)vitium.frustum, frustum);
    CREDO_AEQUALIS_S32 ((s32)vitium.offset, offset);
    CREDO_VERUM (vitium.causa != NIHIL);
    CREDO_AEQUALIS_I32 (liber.numerus, ZEPHYRUM);
    si (bene || (s32)vitium.offset != offset)
    {
        imprimere("  '%s': offset %d (%s)\n", stml,
            (integer)vitium.offset,
            vitium.causa ? vitium.causa : "-");
    }
}


    /* ==================================================
     * LEGERE: mappae, partes, frusta, notae
     * ================================================== */

interior vacuum
_sectio_legere (
    CredoContextus* c)
{
                Carmina  liber;
        constans Carmen* k;
    constans CarmenPars* a;
    constans CarmenPars* b;

    CREDO_NECESSE (_legere(c, CARMEN_PROBAE, &liber));
    CREDO_NECESSE (liber.numerus == I);
    k = carmina_invenire(&liber, _ch(c, "proba"));
    CREDO_NECESSE (k == &liber.carmina[ZEPHYRUM]);
    CREDO_VERUM (carmina_invenire(&liber, _ch(c, "nulla")) == NIHIL);

    CREDO_GRADUS ("mappae");
    CREDO_NECESSE (k->numerus_temporum == II);
    CREDO_AEQUALIS_S64 (k->tempora[ZEPHYRUM].pulsus, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (k->tempora[ZEPHYRUM].tempus, XC);
    CREDO_AEQUALIS_S64 (k->tempora[I].pulsus, (s64)MMMDCCCXL);
    CREDO_AEQUALIS_I32 (k->tempora[I].tempus, CLXXX);
    /* IV quartae ad XC = 2.667 s = CXXVIII milia quadra */
    CREDO_AEQUALIS_S64 (k->tempora[I].quadrum, (s64)(CXXVIII * M));
    CREDO_NECESSE (k->numerus_metrorum == II);
    CREDO_AEQUALIS_I32 (k->metra[ZEPHYRUM].numerator, IV);
    CREDO_AEQUALIS_I32 (k->metra[ZEPHYRUM].denominator, IV);
    CREDO_AEQUALIS_S64 (k->metra[I].pulsus, (s64)MMMDCCCXL * II);
    CREDO_AEQUALIS_I32 (k->metra[I].numerator, III);
    CREDO_NECESSE (k->numerus_clavium == II);
    CREDO_AEQUALIS_S32 (k->claves[ZEPHYRUM].quintae, II);
    CREDO_FALSUM (k->claves[ZEPHYRUM].minor);
    CREDO_AEQUALIS_S32 (k->claves[I].quintae, -V);
    CREDO_VERUM (k->claves[I].minor);
    CREDO_AEQUALIS_S64 (k->claves[I].pulsus, (s64)MMMDCCCXL * II);

    CREDO_GRADUS ("mensurae ex metro");
    CREDO_AEQUALIS_S64 (carmen_pulsus_mensurae(k, I), ZEPHYRUM);
    CREDO_AEQUALIS_S64 (carmen_pulsus_mensurae(k, II), (s64)MMMDCCCXL);
    CREDO_AEQUALIS_S64 (carmen_pulsus_mensurae(k, III),
                        (s64)MMMDCCCXL * II);
    /* post 3/4: mensura = MMDCCCLXXX */
    CREDO_AEQUALIS_S64 (carmen_pulsus_mensurae(k, IV),
                        (s64)MMMDCCCXL * II + MMDCCCLXXX);
    CREDO_AEQUALIS_S64 (carmen_pulsus_mensurae(k, V),
                        (s64)MMMDCCCXL * II + (s64)MMDCCCLXXX * II);

    CREDO_GRADUS ("partes et frusta");
    CREDO_NECESSE (k->numerus_partium == II);
    a = &k->partes[ZEPHYRUM];
    b = &k->partes[I];
    CREDO_CHORDA_AEQUALIS_LITERIS (a->titulus, "a");
    CREDO_CHORDA_AEQUALIS_LITERIS (a->instrumentum, "quadrans");
    CREDO_VERUM (a->lucrum == 0.5f);
    CREDO_FALSUM (a->mutus);
    CREDO_FALSUM (a->solus);
    CREDO_NECESSE (a->numerus_frustorum == II);
    CREDO_AEQUALIS_S64 (a->frusta[ZEPHYRUM].pulsus, ZEPHYRUM);
    CREDO_AEQUALIS_S64 (a->frusta[ZEPHYRUM].longitudo, (s64)MMMDCCCXL);
    CREDO_NECESSE (a->frusta[ZEPHYRUM].numerus == III);
    CREDO_AEQUALIS_S64 (a->frusta[ZEPHYRUM].notae[I].pulsus, (s64)CMLX);
    CREDO_AEQUALIS_S64 (a->frusta[ZEPHYRUM].notae[II].longitudo,
                        (s64)CMLX);
    CREDO_AEQUALIS_I32 (a->frusta[ZEPHYRUM].notae[ZEPHYRUM].nota, LXI);
    CREDO_VERUM (a->frusta[ZEPHYRUM].notae[ZEPHYRUM].littera == 'c');
    CREDO_VERUM (a->frusta[ZEPHYRUM].notae[I].littera == 'd');
    CREDO_AEQUALIS_I32 (a->frusta[ZEPHYRUM].notae[I].nota, LXI);
    CREDO_CHORDA_AEQUALIS_LITERIS (a->frusta[ZEPHYRUM].textus,
                                   "l4 c# d- e r");
    /* frustum mensurae IV (post metrum 3/4) */
    CREDO_AEQUALIS_S64 (a->frusta[I].pulsus,
                        (s64)MMMDCCCXL * II + MMDCCCLXXX);
    CREDO_AEQUALIS_S64 (a->frusta[I].longitudo, (s64)MMDCCCLXXX);
    CREDO_CHORDA_AEQUALIS_LITERIS (b->instrumentum, "triangulum");
    CREDO_VERUM (b->lucrum == 1.0f);
    CREDO_VERUM (b->mutus);
    CREDO_VERUM (b->solus);
    CREDO_NECESSE (b->numerus_frustorum == I);
    CREDO_AEQUALIS_S64 (b->frusta[ZEPHYRUM].pulsus, (s64)CDLXXX);
    CREDO_AEQUALIS_S64 (b->frusta[ZEPHYRUM].longitudo, (s64)CDLXXX);

    CREDO_GRADUS ("longitudo carminis: finis frusti ultimi");
    CREDO_AEQUALIS_S64 (k->pulsus, (s64)MMMDCCCXL * II
        + (s64)MMDCCCLXXX * II);

    CREDO_GRADUS ("ordinaria sine mappis: CXX, 4/4, C maior");
    CREDO_NECESSE (_legere(c, _carmen(c, "", "<pars titulus=\"a\" "
        "instrumentum=\"aether\"><frustum mensura=\"2\"><textus!>c"
        "</textus></frustum></pars>"), &liber));
    k = &liber.carmina[ZEPHYRUM];
    CREDO_AEQUALIS_I32 (k->numerus_temporum, I);
    CREDO_AEQUALIS_I32 (k->tempora[ZEPHYRUM].tempus, CXX);
    CREDO_AEQUALIS_I32 (k->metra[ZEPHYRUM].numerator, IV);
    CREDO_AEQUALIS_S32 (k->claves[ZEPHYRUM].quintae, ZEPHYRUM);
    CREDO_AEQUALIS_S64 (k->partes[ZEPHYRUM].frusta[ZEPHYRUM].pulsus,
                        (s64)MMMDCCCXL);

    CREDO_GRADUS ("clavis acuta, tempus mensura I, '@' cum t");
    CREDO_NECESSE (_legere(c, _carmen(c, " clavis=\"f# minor\"",
        "<tempus mensura=\"1\" valor=\"100\"/>"
        "<pars titulus=\"a\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>@punctum c</textus></frustum></pars>"
        "<pars titulus=\"b\" instrumentum=\"aether\" mutus/>"),
        &liber));
    k = &liber.carmina[ZEPHYRUM];
    CREDO_AEQUALIS_S32 (k->claves[ZEPHYRUM].quintae, III);
    CREDO_VERUM (k->claves[ZEPHYRUM].minor);
    CREDO_AEQUALIS_I32 (k->numerus_temporum, I);
    CREDO_AEQUALIS_I32 (k->tempora[ZEPHYRUM].tempus, C);
    {
        constans CarmenFrustum* f = k->partes[ZEPHYRUM].frusta;

        CREDO_CHORDA_AEQUALIS_LITERIS (f->notae[ZEPHYRUM].praeparatum,
                                       "punctum");
    }
    /* mutus solus, sine solo */
    CREDO_VERUM (k->partes[I].mutus);
    CREDO_FALSUM (k->partes[I].solus);
}


    /* ==================================================
     * VITIA: carmen, pars, frustum, offset
     * ================================================== */

interior vacuum
_sectio_vitia (
    CredoContextus* c)
{
    constans character* pars_bona = "<pars titulus=\"a\" instrumentum="
        "\"aether\"><frustum mensura=\"1\"><textus!>c</textus>"
        "</frustum></pars>";

    /* liber ipse */
    _vitium(c, "<liber/>", "", "", -I, ZEPHYRUM);
    _vitium(c, "<carmina><carmen tempus=\"90\"/></carmina>", "", "",
            -I, ZEPHYRUM);
    _vitium(c, "<carmina><carmen titulus=\"x\"/><carmen titulus=\"x\"/>"
            "</carmina>", "x", "", -I, ZEPHYRUM);
    _vitium(c, "<carmina><cantio/></carmina>", "", "", -I, ZEPHYRUM);

    /* carmen: attributa et mappae */
    _vitium(c, _carmen(c, " metrum=\"4/3\"", pars_bona), "x", "", -I,
            ZEPHYRUM);
    _vitium(c, _carmen(c, " metrum=\"0/4\"", pars_bona), "x", "", -I,
            ZEPHYRUM);
    _vitium(c, _carmen(c, " metrum=\"4\"", pars_bona), "x", "", -I,
            ZEPHYRUM);
    _vitium(c, _carmen(c, " clavis=\"h maior\"", pars_bona), "x", "",
            -I, ZEPHYRUM);
    _vitium(c, _carmen(c, " clavis=\"a dur\"", pars_bona), "x", "", -I,
            ZEPHYRUM);
    _vitium(c, _carmen(c, " tempus=\"10\"", pars_bona), "x", "", -I,
            ZEPHYRUM);
    _vitium(c, _carmen(c, " tempus=\"lente\"", pars_bona), "x", "", -I,
            ZEPHYRUM);
    _vitium(c, _carmen(c, "", "<nota/>"), "x", "", -I, ZEPHYRUM);
    _vitium(c, _carmen(c, "", "<tempus mensura=\"2\"/>"), "x", "", -I,
            ZEPHYRUM);

    /* pars */
    _vitium(c, _carmen(c, "", "<pars titulus=\"a\"><frustum mensura="
            "\"1\"><textus!>c</textus></frustum></pars>"), "x", "a", -I,
            ZEPHYRUM);
    _vitium(c, _carmen(c, "", "<pars titulus=\"a\" instrumentum="
            "\"aether\" lucrum=\"2\"/>"), "x", "a", -I, ZEPHYRUM);
    _vitium(c, _carmen(c, "", "<pars titulus=\"a\" instrumentum=\"q\"/>"
            "<pars titulus=\"a\" instrumentum=\"q\"/>"), "x", "a", -I,
            ZEPHYRUM);

    /* frustum: positio, textus MML, 't', tegimen */
    _vitium(c, _carmen(c, "", "<pars titulus=\"a\" instrumentum=\"q\">"
            "<frustum><textus!>c</textus></frustum></pars>"), "x", "a",
            ZEPHYRUM, ZEPHYRUM);
    _vitium(c, _carmen(c, "", "<pars titulus=\"a\" instrumentum=\"q\">"
            "<frustum mensura=\"1\" pulsus=\"0\"><textus!>c</textus>"
            "</frustum></pars>"), "x", "a", ZEPHYRUM, ZEPHYRUM);
    _vitium(c, _carmen(c, "", "<pars titulus=\"a\" instrumentum=\"q\">"
            "<frustum mensura=\"0\"><textus!>c</textus></frustum>"
            "</pars>"), "x", "a", ZEPHYRUM, ZEPHYRUM);
    _vitium(c, _carmen(c, "", "<pars titulus=\"a\" instrumentum=\"q\">"
            "<frustum mensura=\"1\"><textus!>c</textus></frustum>"
            "<frustum mensura=\"2\"><textus!>c l3 d</textus></frustum>"
            "</pars>"), "x", "a", I, III);
    _vitium(c, _carmen(c, "", "<pars titulus=\"a\" instrumentum=\"q\">"
            "<frustum mensura=\"1\"><textus!>c t90 d</textus>"
            "</frustum></pars>"), "x", "a", ZEPHYRUM, II);
    _vitium(c, _carmen(c, "", "<pars titulus=\"a\" instrumentum=\"q\">"
            "<frustum mensura=\"1\"><textus!>c1</textus></frustum>"
            "<frustum pulsus=\"1920\"><textus!>c</textus></frustum>"
            "</pars>"), "x", "a", I, ZEPHYRUM);
    _vitium(c, _carmen(c, "", "<pars titulus=\"a\" instrumentum=\"q\">"
            "<frustum mensura=\"1\"></frustum></pars>"), "x", "a",
            ZEPHYRUM, ZEPHYRUM);
}


    /* ==================================================
     * ORDINARIUM: liber infixus
     * ================================================== */

interior vacuum
_sectio_ordinarium (
    CredoContextus* c)
{
            Carmina  liber;
     CarminisVitium  vitium;
    constans Carmen* k;
                i32  i;

    CREDO_NECESSE (carmina_ordinaria(c->piscina, &liber, &vitium));
    k = carmina_invenire(&liber, _ch(c, "korobeiniki"));
    CREDO_NECESSE (k != NIHIL);
    CREDO_AEQUALIS_I32 (k->tempora[ZEPHYRUM].tempus, CXLIV);
    CREDO_AEQUALIS_S32 (k->claves[ZEPHYRUM].quintae, ZEPHYRUM);
    CREDO_VERUM (k->claves[ZEPHYRUM].minor);
    CREDO_NECESSE (k->numerus_partium == II);
    /* XVI mensurae 4/4: partes ambae aequae, carmen iterat ad finem */
    CREDO_AEQUALIS_S64 (k->pulsus, (s64)MMMDCCCXL * XVI);
    per (i = ZEPHYRUM; i < k->numerus_partium; i++)
    {
        CREDO_AEQUALIS_I32 (k->partes[i].numerus_frustorum, I);
        CREDO_AEQUALIS_S64 (k->partes[i].frusta[ZEPHYRUM].longitudo,
                            (s64)MMMDCCCXL * XVI);
    }
    CREDO_CHORDA_AEQUALIS_LITERIS (k->partes[ZEPHYRUM].instrumentum,
                                   "quadrans");
    CREDO_CHORDA_AEQUALIS_LITERIS (k->partes[I].instrumentum,
                                   "triangulum");
    CREDO_AEQUALIS_I32 (k->partes[I].frusta[ZEPHYRUM].numerus, CXXVIII);
}

/* lineae scriptae frusti: pulsus intra frustum, offset in textu eius */
interior vacuum
_sectio_lineae (
    CredoContextus* c)
{
                   Carmina  liber;
    constans CarmenFrustum* f;

    CREDO_NECESSE (_legere(c,
        "<carmina><carmen titulus=\"l\">"
        "<pars titulus=\"m\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\"><textus!>c d e f</textus></frustum>"
        "<frustum mensura=\"2\"><textus!>g a | b c</textus></frustum>"
        "</pars></carmen></carmina>", &liber));
    f = liber.carmina[ZEPHYRUM].partes[ZEPHYRUM].frusta;
    CREDO_AEQUALIS_I32 (f[ZEPHYRUM].numerus_linearum, ZEPHYRUM);
    CREDO_VERUM (f[ZEPHYRUM].lineae == NIHIL);
    CREDO_NECESSE (f[I].numerus_linearum == I);
    CREDO_AEQUALIS_S64 (f[I].lineae[ZEPHYRUM].pulsus, (s64)MCMXX);
    CREDO_AEQUALIS_I32 (f[I].lineae[ZEPHYRUM].offset, IV);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "legere", _sectio_legere, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vitia", _sectio_vitia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "lineae", _sectio_lineae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ordinarium", _sectio_ordinarium, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("carmen", SECTIONES);
}

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

/* tempus metri ad pulsum: 1/denominator; 6/8 9/8 12/8 tres octavae;
 * 3/8 et 6/4 non composita (regula: denominator VIII, numerator > III
 * multiplex III); mutatio metri in initio mensurae */
interior vacuum
_sectio_tempus (
    CredoContextus* c)
{
    hic_manens constans s64 SPERATA[IX] = {
        960, 960, 1440, 1440, 1440, 480, 1920, 960, 1920
    };
            Carmina  liber;
    constans Carmen* k;
                i32  m;

    CREDO_NECESSE (_legere(c,
        "<carmina><carmen titulus=\"t\" metrum=\"4/4\">"
        "<metrum mensura=\"2\" valor=\"3/4\"/>"
        "<metrum mensura=\"3\" valor=\"6/8\"/>"
        "<metrum mensura=\"4\" valor=\"9/8\"/>"
        "<metrum mensura=\"5\" valor=\"12/8\"/>"
        "<metrum mensura=\"6\" valor=\"3/8\"/>"
        "<metrum mensura=\"7\" valor=\"2/2\"/>"
        "<metrum mensura=\"8\" valor=\"6/4\"/>"
        "<metrum mensura=\"9\" valor=\"3/2\"/>"
        "<pars titulus=\"p\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>c4</textus></frustum></pars>"
        "</carmen>"
        "<carmen titulus=\"u\"><pars titulus=\"p\" instrumentum="
        "\"aether\"><frustum mensura=\"1\"><textus!>c4</textus>"
        "</frustum></pars></carmen></carmina>", &liber));
    k = &liber.carmina[ZEPHYRUM];
    per (m = I; m <= IX; m++)
    {
        s64 initium = carmen_pulsus_mensurae(k, (i32)m);

        CREDO_AEQUALIS_S64 (carmen_tempus_mensurae(k, initium),
                            SPERATA[m - I]);
        CREDO_AEQUALIS_S64 (carmen_tempus_mensurae(k, initium + I),
                            SPERATA[m - I]);
        si (m > I)
        {
            /* pulsus ultimus mensurae prioris: metrum prius */
            CREDO_AEQUALIS_S64 (carmen_tempus_mensurae(k, initium - I),
                                SPERATA[m - II]);
        }
    }
    /* sine metro: 4/4 ordinarium */
    CREDO_AEQUALIS_S64 (carmen_tempus_mensurae(&liber.carmina[I],
                                               ZEPHYRUM), CMLX);
}

/* instrumenta libri et carminis: basis praeparata et libri (catena),
 * ordinaria sine basi, praeparatum redefinitum, carminis proprium
 * vincit et aliis invisibile */
hic_manens constans character LIBER_INSTRUMENTORUM[] =
    "<carmina>"
    "<instrumentum titulus=\"cembalo\" basis=\"chorda\""
    " claritas=\"0.9\""
    " decrementum=\"0.8\" ambitus=\"41..89\"/>"
    "<instrumentum titulus=\"cembalo_mollis\" basis=\"cembalo\""
    " claritas=\"0.3\"/>"
    "<instrumentum titulus=\"sinus_purus\"/>"
    "<instrumentum titulus=\"aether\" basis=\"quadrans\""
    " amplitudo=\"0.2\"/>"
    "<instrumentum titulus=\"serra_fm\" genus=\"modulatio\""
    " forma=\"serra\" ratio=\"2\" index=\"1.5\" impetus=\"0.01\"/>"
    "<carmen titulus=\"a\">"
    "<instrumentum titulus=\"campana_mollis\" basis=\"campana\""
    " index=\"3\"/>"
    "<instrumentum titulus=\"cembalo\" basis=\"chorda\""
    " claritas=\"0.1\"/>"
    "<instrumentum titulus=\"cembalo_cantus\" basis=\"cembalo_mollis\""
    " decrementum=\"2\"/>"
    "<pars titulus=\"p\" instrumentum=\"cembalo\"><frustum "
    "mensura=\"1\"><textus!>c4</textus></frustum></pars></carmen>"
    "<carmen titulus=\"b\"><pars titulus=\"p\" "
    "instrumentum=\"cembalo\">"
    "<frustum mensura=\"1\"><textus!>c4</textus></frustum></pars>"
    "</carmen></carmina>";

interior vacuum
_sectio_instrumenta (
    CredoContextus* c)
{
                        Carmina  liber;
                constans Carmen* a;
                constans Carmen* b;
    constans CarmenInstrumentum* x;

    CREDO_NECESSE (_legere(c, LIBER_INSTRUMENTORUM, &liber));
    a = carmina_invenire(&liber, _ch(c, "a"));
    b = carmina_invenire(&liber, _ch(c, "b"));
    CREDO_NECESSE (a != NIHIL && b != NIHIL);
    CREDO_AEQUALIS_I32 (a->numerus_instrumentorum, VIII);
    CREDO_AEQUALIS_I32 (b->numerus_instrumentorum, V);
    /* basis praeparata, campi mutati, ambitus */
    x = carmen_instrumentum(b, _ch(c, "cembalo"));
    CREDO_NECESSE (x != NIHIL);
    CREDO_VERUM (x->parametra.genus == 1.0f);
    CREDO_VERUM (x->parametra.claritas == 0.9f);
    CREDO_VERUM (x->parametra.decrementum == 0.8f);
    CREDO_VERUM (   x->parametra.amplitudo
                 == tibia_praeparatum("chorda")->amplitudo);
    CREDO_AEQUALIS_I32 (x->infima, XLI);
    CREDO_AEQUALIS_I32 (x->summa, LXXXIX);
    /* catena: basis libri supra definita, ambitus hereditarius */
    x = carmen_instrumentum(b, _ch(c, "cembalo_mollis"));
    CREDO_NECESSE (x != NIHIL);
    CREDO_VERUM (x->parametra.claritas == 0.3f);
    CREDO_VERUM (x->parametra.decrementum == 0.8f);
    CREDO_AEQUALIS_I32 (x->infima, XLI);
    /* sine basi: ordinaria */
    x = carmen_instrumentum(b, _ch(c, "sinus_purus"));
    CREDO_NECESSE (x != NIHIL);
    CREDO_VERUM (   x->parametra.genus == 0.0f
                 && x->parametra.forma == (f32)SONUS_UNDA_SINUS
                 && x->parametra.officium == 0.5f
                 && x->parametra.involucrum.tenor == 1.0f
                 && x->parametra.amplitudo == 0.5f
                 && x->parametra.involucrum.impetus == 0.0f);
    CREDO_AEQUALIS_I32 (x->infima, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (x->summa, CXXVII);
    /* verba generis et formae */
    x = carmen_instrumentum(b, _ch(c, "serra_fm"));
    CREDO_NECESSE (x != NIHIL);
    CREDO_VERUM (   x->parametra.genus == 2.0f
                 && x->parametra.forma == (f32)SONUS_UNDA_SERRA
                 && x->parametra.ratio == 2.0f
                 && x->parametra.index == 1.5f
                 && x->parametra.involucrum.impetus == 0.01f);
    /* praeparatum redefinitum */
    x = carmen_instrumentum(b, _ch(c, "aether"));
    CREDO_NECESSE (x != NIHIL);
    CREDO_VERUM (   x->parametra.forma == 0.0f
                 && x->parametra.amplitudo == 0.2f);
    /* carminis proprium vincit; alteri invisibile; praeparata non
     * hic */
    x = carmen_instrumentum(a, _ch(c, "cembalo"));
    CREDO_NECESSE (x != NIHIL);
    CREDO_VERUM (x->parametra.claritas == 0.1f);
    CREDO_AEQUALIS_I32 (x->summa, CXXVII);
    x = carmen_instrumentum(a, _ch(c, "campana_mollis"));
    CREDO_NECESSE (x != NIHIL);
    CREDO_VERUM (   x->parametra.genus == 2.0f
                 && x->parametra.index == 3.0f
                 && x->parametra.ratio == 1.4f);
    /* carminis basis ex libro */
    x = carmen_instrumentum(a, _ch(c, "cembalo_cantus"));
    CREDO_NECESSE (x != NIHIL);
    CREDO_VERUM (   x->parametra.claritas == 0.3f
                 && x->parametra.decrementum == 2.0f);
    CREDO_AEQUALIS_I32 (x->infima, XLI);
    CREDO_VERUM (carmen_instrumentum(b, _ch(c, "campana_mollis"))
        == NIHIL);
    CREDO_VERUM (carmen_instrumentum(b, _ch(c, "chorda")) == NIHIL);
}

/* vitia instrumentorum: causa nominata */
interior b32
_instrumentum_recusat (
        CredoContextus* c,
    constans character* instrumentum,
    constans character* causa)
{
           Carmina  liber;
    CarminisVitium  v;
         character* s = (character*)piscina_allocare(c->piscina, M);

    sprintf(s, "<carmina>%s<carmen titulus=\"x\"><pars titulus=\"p\" "
            "instrumentum=\"aether\"><frustum mensura=\"1\"><textus!>c4"
            "</textus></frustum></pars></carmen></carmina>",
            instrumentum);
    memset(&v, ZEPHYRUM, magnitudo(v));
    si (carmina_legere(_ch(c, s), c->piscina, &liber, &v))
    {
        imprimere("  acceptum: %s\n", instrumentum);
        redde FALSUM;
    }
    si (v.causa == NIHIL || strcmp(v.causa, causa) != ZEPHYRUM)
    {
        imprimere("  %s: causa '%s'\n", instrumentum,
                  v.causa ? v.causa : "-");
        redde FALSUM;
    }
    redde VERUM;
}

interior vacuum
_sectio_instrumenta_vitia (
    CredoContextus* c)
{
    CREDO_VERUM (_instrumentum_recusat(c,
        "<instrumentum titulus=\"x\" sonus=\"1\"/>",
        "instrumentum: attributum ignotum"));
    CREDO_VERUM (_instrumentum_recusat(c,
        "<instrumentum titulus=\"x\" claritas=\"1.5\"/>",
        "instrumentum: valor malus aut extra fines"));
    CREDO_VERUM (_instrumentum_recusat(c,
        "<instrumentum titulus=\"x\" impetus=\"-1\"/>",
        "instrumentum: valor malus aut extra fines"));
    CREDO_VERUM (_instrumentum_recusat(c,
        "<instrumentum titulus=\"x\" amplitudo=\"multa\"/>",
        "instrumentum: valor malus aut extra fines"));
    CREDO_VERUM (_instrumentum_recusat(c,
        "<instrumentum titulus=\"x\" genus=\"piano\"/>",
        "instrumentum: valor malus aut extra fines"));
    CREDO_VERUM (_instrumentum_recusat(c,
        "<instrumentum titulus=\"x\" ambitus=\"50..40\"/>",
        "instrumentum: valor malus aut extra fines"));
    CREDO_VERUM (_instrumentum_recusat(c,
        "<instrumentum titulus=\"x\" claritas=\"\"/>",
        "instrumentum: valor malus aut extra fines"));
    CREDO_VERUM (_instrumentum_recusat(c,
        "<instrumentum titulus=\"x\" ambitus=\"\"/>",
        "instrumentum: valor malus aut extra fines"));
    CREDO_VERUM (_instrumentum_recusat(c,
        "<instrumentum titulus=\"x\" basis=\"nullum\"/>",
        "instrumentum: basis ignota"));
    /* basis infra definita: ignota */
    CREDO_VERUM (_instrumentum_recusat(c,
        "<instrumentum titulus=\"x\" basis=\"y\"/>"
        "<instrumentum titulus=\"y\"/>",
        "instrumentum: basis ignota"));
    CREDO_VERUM (_instrumentum_recusat(c,
        "<instrumentum titulus=\"x\"/><instrumentum titulus=\"x\"/>",
        "instrumentum iteratum"));
    CREDO_VERUM (_instrumentum_recusat(c,
        "<instrumentum titulus=\"X Y\"/>",
        "instrumentum sine titulo [a-z0-9_]+"));
}

/* partiales: genus verbum, "r:a:d ..." in ordinem, basis hereditaria
 * (marimba), attributum omnes substituit (reliqui nulli) */
hic_manens constans character LIBER_PARTIALIUM[] =
    "<carmina>"
    "<instrumentum titulus=\"organum_plenum\" genus=\"partiales\""
    " partiales=\"1:1:0  2:0.5:0 4:0.25:1.5\"/>"
    "<instrumentum titulus=\"marimba_mollis\" basis=\"marimba\""
    " amplitudo=\"0.3\"/>"
    "<instrumentum titulus=\"organum_tenue\" basis=\"organum\""
    " partiales=\"1:1:0\"/>"
    "<carmen titulus=\"a\"><pars titulus=\"p\" "
    "instrumentum=\"organum_plenum\">"
    "<frustum mensura=\"1\"><textus!>c4</textus></frustum></pars>"
    "</carmen></carmina>";

interior vacuum
_sectio_partiales (
    CredoContextus* c)
{
                        Carmina  liber;
                constans Carmen* a;
    constans CarmenInstrumentum* x;
        constans TibiaParametra* m = tibia_praeparatum("marimba");
                            i32  k;

    CREDO_NECESSE (_legere(c, LIBER_PARTIALIUM, &liber));
    a = carmina_invenire(&liber, _ch(c, "a"));
    CREDO_NECESSE (a != NIHIL && m != NIHIL);
    x = carmen_instrumentum(a, _ch(c, "organum_plenum"));
    CREDO_NECESSE (x != NIHIL);
    CREDO_VERUM (x->parametra.genus == (f32)TIBIA_PARTIALES);
    CREDO_VERUM (   x->parametra.partiales[0].ratio == 1.0f
                 && x->parametra.partiales[0].amplitudo == 1.0f
                 && x->parametra.partiales[1].ratio == 2.0f
                 && x->parametra.partiales[1].amplitudo == 0.5f
                 && x->parametra.partiales[2].ratio == 4.0f
                 && x->parametra.partiales[2].amplitudo == 0.25f
                 && x->parametra.partiales[2].decrementum == 1.5f
                 && x->parametra.partiales[3].ratio == 0.0f);
    x = carmen_instrumentum(a, _ch(c, "marimba_mollis"));
    CREDO_NECESSE (x != NIHIL);
    CREDO_VERUM (x->parametra.amplitudo == 0.3f);
    per (k = ZEPHYRUM; k < TIBIA_PARTIALES_NUMERUS; k++)
    {
        CREDO_VERUM (   x->parametra.partiales[k].ratio
                        == m->partiales[k].ratio
                     && x->parametra.partiales[k].decrementum
                        == m->partiales[k].decrementum);
    }
    x = carmen_instrumentum(a, _ch(c, "organum_tenue"));
    CREDO_NECESSE (x != NIHIL);
    CREDO_VERUM (   x->parametra.genus == (f32)TIBIA_PARTIALES
                 && x->parametra.partiales[0].ratio == 1.0f
                 && x->parametra.partiales[1].ratio == 0.0f
                 && x->parametra.partiales[1].amplitudo == 0.0f);
}

interior vacuum
_sectio_partiales_vitia (
    CredoContextus* c)
{
    constans character* causa =
        "instrumentum: valor malus aut extra fines";

    CREDO_VERUM (_instrumentum_recusat(c,
        "<instrumentum titulus=\"x\" partiales=\"0:1:0\"/>", causa));
    CREDO_VERUM (_instrumentum_recusat(c,
        "<instrumentum titulus=\"x\" partiales=\"33:1:0\"/>", causa));
    CREDO_VERUM (_instrumentum_recusat(c,
        "<instrumentum titulus=\"x\" partiales=\"1:2:0\"/>", causa));
    CREDO_VERUM (_instrumentum_recusat(c,
        "<instrumentum titulus=\"x\" partiales=\"1:1:61\"/>", causa));
    CREDO_VERUM (_instrumentum_recusat(c,
        "<instrumentum titulus=\"x\" partiales=\"1:1\"/>", causa));
    CREDO_VERUM (_instrumentum_recusat(c,
        "<instrumentum titulus=\"x\" partiales=\"1:1:0:2\"/>", causa));
    CREDO_VERUM (_instrumentum_recusat(c,
        "<instrumentum titulus=\"x\" partiales=\"1:x:0\"/>", causa));
    CREDO_VERUM (_instrumentum_recusat(c,
        "<instrumentum titulus=\"x\" partiales=\"\"/>", causa));
    CREDO_VERUM (_instrumentum_recusat(c,
        "<instrumentum titulus=\"x\" partiales=\"   \"/>", causa));
    CREDO_VERUM (_instrumentum_recusat(c,
        "<instrumentum titulus=\"x\" partiales=\"1:1:0 2:1:0 3:1:0"
        " 4:1:0 5:1:0 6:1:0 7:1:0 8:1:0 9:1:0\"/>", causa));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "legere", _sectio_legere, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vitia", _sectio_vitia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "lineae", _sectio_lineae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ordinarium", _sectio_ordinarium, NIHIL, NIHIL, NIHIL, NIHIL },
    { "tempus", _sectio_tempus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "instrumenta", _sectio_instrumenta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "instrumenta_vitia", _sectio_instrumenta_vitia, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "partiales", _sectio_partiales, NIHIL, NIHIL, NIHIL, NIHIL },
    { "partiales_vitia", _sectio_partiales_vitia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("carmen", SECTIONES);
}

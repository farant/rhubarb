/* probatio_auscultatio.c - Probationes auscultationis
 * (include/auscultatio.h)
 *
 * metiri: exempla ficta, numeri exacti (mensurae ex mappa temporis et
 * metri). reddere / partem / tacitae: sonus verus, proprietates
 * (deterministicus, quadrum 0 = pulsus 0, longitudo, silentium).
 * Ad CXX quartae: mensura 4/4 = 2 s = XCVI milia quadra.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "carmen.h"
#include "cantor.h"
#include "auscultatio.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

#define MENSURA_ORDINARIA (XCVI * M)

interior Carmen*
_carmen (
        CredoContextus* c,
    constans character* stml)
{
           Carmina* liber;
    CarminisVitium  vitium;

    liber = (Carmina*)piscina_allocare(c->piscina, magnitudo(Carmina));
    si (!carmina_legere(chorda_ex_literis(stml, c->piscina), c->piscina,
                        liber, &vitium) || liber->numerus < I)
    {
        imprimere("  carmen: %s (offset %u)\n",
                  vitium.causa ? vitium.causa : "-", vitium.offset);
        redde NIHIL;
    }
    redde &liber->carmina[ZEPHYRUM];
}

interior b32
_prope (
    f64 a,
    f64 b)
{
    si (fabs(a - b) <= 1e-6)
    {
        redde VERUM;
    }
    imprimere("  %.9f pro %.9f\n", a, b);
    redde FALSUM;
}

/* |exemplum| maximum in quadris [ab, ad) */
interior f32
_apex (
    constans f32* x,
             i32  ab,
             i32  ad)
{
    f32 m = 0.0f;
    i32 i;

    per (i = II * ab; i < II * ad; i++)
    {
        f32 v = x[i] < 0.0f ? -x[i] : x[i];

        m = v > m ? v : m;
    }
    redde m;
}

interior f32*
_exempla (
    CredoContextus* c,
               i32  quadra,
               f32  valor)
{
    f32* x = (f32*)piscina_allocare(c->piscina,
        (memoriae_index)quadra * II * magnitudo(f32));
    i32 i;

    per (i = ZEPHYRUM; x != NIHIL && i < II * quadra; i++)
    {
        x[i] = valor;
    }
    redde x;
}


    /* ==================================================
     * METIRI: apex, tonsura, rms, mensurae (exempla ficta)
     * ================================================== */

interior vacuum
_sectio_metiri (
    CredoContextus* c)
{
    constans Carmen* k = _carmen(c,
        "<carmina><carmen titulus=\"m\">"
        "<pars titulus=\"a\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\">"
        "<textus!>o4 c1 c1</textus></frustum></pars></carmen>"
        "</carmina>");
    AuscultatioSummarium  s;
                     f32* x = _exempla(c, II * MENSURA_ORDINARIA, 0.5f);
                     f64  summa;
                     i32  i;

    CREDO_NECESSE (k != NIHIL && x != NIHIL);
    /* mensura 2: silentium praeter +1.25 (dextra) et -1.0 (sinistra) */
    per (i = II * MENSURA_ORDINARIA; i < IV * MENSURA_ORDINARIA; i++)
    {
        x[i] = 0.0f;
    }
    x[II * MENSURA_ORDINARIA + DCCCLXXXI]  = 1.25f;
    x[II * MENSURA_ORDINARIA + MMM]        = -1.0f;
    CREDO_NECESSE (auscultatio_metiri(k, x, II * MENSURA_ORDINARIA,
        c->piscina,
                                      &s));
    CREDO_AEQUALIS_I32 (s.numerus, II);
    CREDO_VERUM (_prope((f64)s.apex, 1.25));
    CREDO_AEQUALIS_I32 (s.tonsa, II);
    CREDO_VERUM (_prope((f64)s.mensurae[ZEPHYRUM], 0.5));
    summa = 1.25 * 1.25 + 1.0;
    CREDO_VERUM (_prope((f64)s.mensurae[I],
        sqrt(summa / (f64)(II * MENSURA_ORDINARIA))));
    CREDO_VERUM (_prope((f64)s.rms,
        sqrt((0.25 * (f64)(II * MENSURA_ORDINARIA) + summa)
             / (f64)(IV * MENSURA_ORDINARIA))));
    /* exempla breviora: mensura ultra = 0 */
    CREDO_NECESSE (auscultatio_metiri(k, x, MENSURA_ORDINARIA,
        c->piscina,
        &s));
    CREDO_VERUM (_prope((f64)s.mensurae[I], 0.0));
    CREDO_AEQUALIS_I32 (s.tonsa, ZEPHYRUM);
}

/* mensurae ex mappis: mensura 2 = 3/4 ad LX = 3 s */
interior vacuum
_sectio_mappae (
    CredoContextus* c)
{
    constans Carmen* k = _carmen(c,
        "<carmina><carmen titulus=\"t\">"
        "<metrum mensura=\"2\" valor=\"3/4\"/>"
        "<tempus mensura=\"2\" valor=\"60\"/>"
        "<pars titulus=\"a\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\">"
        "<textus!>o4 c1 | c2. | c2.</textus></frustum></pars></carmen>"
        "</carmina>");
     AuscultatioSummarium  s;
                      i32  quadra  = MENSURA_ORDINARIA + II * CXLIV * M;
                      f32* x       = _exempla(c, quadra, 0.1f);
                      i32  i;

    CREDO_NECESSE (k != NIHIL && x != NIHIL);
    per (i = II * MENSURA_ORDINARIA; i < II * quadra; i++)
    {
        x[i] = i < II * (MENSURA_ORDINARIA + CXLIV * M) ? 0.2f : 0.3f;
    }
    CREDO_NECESSE (auscultatio_metiri(k, x, quadra, c->piscina, &s));
    CREDO_AEQUALIS_I32 (s.numerus, III);
    CREDO_VERUM (_prope((f64)s.mensurae[ZEPHYRUM], 0.1));
    CREDO_VERUM (_prope((f64)s.mensurae[I], 0.2));
    /* mensura post mutationem: initium ex mappa (non pulsus x XXV) */
    CREDO_VERUM (_prope((f64)s.mensurae[II], 0.3));
}


    /* ==================================================
     * REDDERE: deterministicum, quadrum 0 = pulsus 0, longitudo, pausae
     * ================================================== */

interior vacuum
_sectio_reddere (
    CredoContextus* c)
{
    constans Carmen* k = _carmen(c,
        "<carmina><carmen titulus=\"r\">"
        "<pars titulus=\"a\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\">"
        "<textus!>o4 r4 c4 r2 | r1 | r1 | c1</textus></frustum></pars>"
        "</carmen></carmina>");
    constans Carmen* pausis = _carmen(c,
        "<carmina><carmen titulus=\"f\">"
        "<pars titulus=\"a\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 c4 r2. | r1 | r1</textus></frustum>"
        "</pars></carmen></carmina>");
     AuscultatioSummarium  s;
                      f32* x;
                      f32* y;
                      i32  n;
                      i32  m;

    CREDO_NECESSE (k != NIHIL && pausis != NIHIL);
    /* carmen pausis finitum: totum redditur, etiam sine voce viva */
    CREDO_NECESSE (auscultatio_reddere(pausis, NIHIL, c->piscina, &x,
        &n));
    CREDO_VERUM (n >= III * MENSURA_ORDINARIA);
    CREDO_NECESSE (auscultatio_reddere(k, NIHIL, c->piscina, &x, &n));
    CREDO_NECESSE (auscultatio_reddere(k, NIHIL, c->piscina, &y, &m));
    CREDO_AEQUALIS_I32 (n, m);
    CREDO_VERUM (memcmp(x, y, (size_t)n * II * magnitudo(f32))
                 == ZEPHYRUM);
    /* totum carmen et cauda (ad II s) */
    CREDO_VERUM (n >= IV * MENSURA_ORDINARIA);
    CREDO_VERUM (n <= IV * MENSURA_ORDINARIA + AUSCULTATIO_CAUDA);
    /* quadrum 0 = pulsus 0: quarta prima tacet, secunda sonat */
    CREDO_VERUM (_apex(x, ZEPHYRUM, XXIV * M) < 1e-6f);
    CREDO_VERUM (_apex(x, XXIV * M, XLVIII * M) > 0.01f);
    /* exacte: nota ad quadrum XXIV milia (pulsus CMLX) incipit */
    CREDO_VERUM (_apex(x, XXIV * M, XXIV * M + CDLXXX) > 1e-5f);
    CREDO_NECESSE (auscultatio_metiri(k, x, n, c->piscina, &s));
    CREDO_AEQUALIS_I32 (s.numerus, IV);
    CREDO_VERUM (s.mensurae[ZEPHYRUM] > AUSCULTATIO_TACITUM);
    CREDO_VERUM (s.mensurae[II] < AUSCULTATIO_TACITUM);
    CREDO_VERUM (s.mensurae[III] > AUSCULTATIO_TACITUM);
    CREDO_AEQUALIS_I32 (s.tonsa, ZEPHYRUM);
}


    /* ==================================================
     * PARTEM: pars sola - sonat, minor quam totum; index extra
     * ================================================== */

interior vacuum
_sectio_partem (
    CredoContextus* c)
{
    constans Carmen* k = _carmen(c,
        "<carmina><carmen titulus=\"p\">"
        "<pars titulus=\"melodia\" instrumentum=\"quadrans\">"
        "<frustum mensura=\"1\"><textus!>o5 e d c d</textus></frustum>"
        "</pars><pars titulus=\"bassus\" instrumentum=\"triangulum\">"
        "<frustum mensura=\"1\"><textus!>o2 a1</textus></frustum>"
        "</pars>"
        "</carmen></carmina>");
     AuscultatioSummarium  totum;
     AuscultatioSummarium  melodia;
     AuscultatioSummarium  bassus;
                      f32* x;
                      i32  n;

    CREDO_NECESSE (k != NIHIL);
    CREDO_NECESSE (auscultatio_reddere(k, NIHIL, c->piscina, &x, &n));
    CREDO_NECESSE (auscultatio_metiri(k, x, n, c->piscina, &totum));
    CREDO_NECESSE (auscultatio_partem(k, ZEPHYRUM, NIHIL, c->piscina,
                                      &melodia));
    CREDO_NECESSE (auscultatio_partem(k, I, NIHIL, c->piscina,
        &bassus));
    CREDO_VERUM (melodia.rms > AUSCULTATIO_TACITUM);
    CREDO_VERUM (bassus.rms > AUSCULTATIO_TACITUM);
    CREDO_VERUM (melodia.rms < totum.rms);
    CREDO_VERUM (bassus.rms < totum.rms);
    CREDO_VERUM (melodia.rms != bassus.rms);
    CREDO_FALSUM (auscultatio_partem(k, II, NIHIL, c->piscina,
        &bassus));
}


    /* ==================================================
     * TACITAE: notae sine sono (instrumentum NIHIL); mutus, pausa non
     * ================================================== */

interior vacuum
_sectio_tacitae (
    CredoContextus* c)
{
    Carmen* k = (Carmen*)(vacuum*)_carmen(c,
        "<carmina><carmen titulus=\"s\">"
        "<pars titulus=\"a\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\">"
        "<textus!>o4 c1</textus></frustum></pars>"
        "<pars titulus=\"b\" instrumentum=\"nullum\"><frustum "
        "mensura=\"3\">"
        "<textus!>o4 c1</textus></frustum></pars>"
        "<pars titulus=\"c\" instrumentum=\"aether\" mutus>"
        "<frustum mensura=\"4\"><textus!>o4 c1</textus></frustum>"
        "</pars>"
        "</carmen></carmina>");
     AuscultatioSummarium  s;
                      f32* x;
                      i32  n;
                      i32  mensurae[IV];

    CREDO_NECESSE (k != NIHIL);
    CREDO_NECESSE (auscultatio_reddere(k, NIHIL, c->piscina, &x, &n));
    CREDO_NECESSE (auscultatio_metiri(k, x, n, c->piscina, &s));
    CREDO_AEQUALIS_I32 (s.numerus, IV);
    /* mensura 3 tantum: 'b' sine instrumento; 2 pausa, 4 muta */
    CREDO_NECESSE (auscultatio_tacitae(k, &s, mensurae, IV) == I);
    CREDO_AEQUALIS_I32 (mensurae[ZEPHYRUM], III);
    CREDO_AEQUALIS_I32 (auscultatio_tacitae(k, &s, mensurae, ZEPHYRUM),
        I);
    /* pars 'a' sola: 'b' sonare non debet - nulla tacita */
    k->partes[ZEPHYRUM].solus = VERUM;
    CREDO_AEQUALIS_I32 (auscultatio_tacitae(k, &s, mensurae, IV),
        ZEPHYRUM);
}

/* cauda reverberationis ultra AUSCULTATIO_CAUDA redditur (capacitas
 * crescit dum vivit), deinde tacet; sine aula longitudo eadem */
interior vacuum
_sectio_cauda_aulae (
    CredoContextus* c)
{
    constans Carmen* k = _carmen(c,
        "<carmina><carmen titulus=\"aula\" tempus=\"240\">"
        "<reverberatio aula=\"0.85\" mollities=\"0.4\" lucrum=\"1\"/>"
        "<pars titulus=\"a\" instrumentum=\"triangulum\""
        " reverberatio=\"1\"><frustum mensura=\"1\"><textus!>"
        "c4 r2.</textus></frustum></pars></carmen></carmina>");
    constans Carmen* sicca = _carmen(c,
        "<carmina><carmen titulus=\"sicca\" tempus=\"240\">"
        "<pars titulus=\"a\" instrumentum=\"triangulum\">"
        "<frustum mensura=\"1\"><textus!>c4 r2.</textus></frustum>"
        "</pars></carmen></carmina>");
    f32* x;
    f32* y;
    i32  n;
    i32  m;
    i32  i;
    f32  apex = 0.0f;

    CREDO_NECESSE (k != NIHIL && sicca != NIHIL);
    CREDO_NECESSE (auscultatio_reddere(k, NIHIL, c->piscina, &x, &n));
    CREDO_NECESSE (auscultatio_reddere(sicca, NIHIL, c->piscina, &y,
                                       &m));
    /* carmen 1 s; sine aula cauda brevis (remissio) */
    CREDO_VERUM (m < SONUS_FREQUENTIA + AUSCULTATIO_CAUDA);
    CREDO_VERUM (n > II * SONUS_FREQUENTIA + AUSCULTATIO_CAUDA);
    per (i = n - SONUS_FREQUENTIA / C; i < n; i++)
    {
        f32 l = x[II * i] < 0.0f ? -x[II * i] : x[II * i];

        apex = l > apex ? l : apex;
    }
    CREDO_VERUM (apex < 1.0e-4f);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "metiri", _sectio_metiri, NIHIL, NIHIL, NIHIL, NIHIL },
    { "mappae", _sectio_mappae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "reddere", _sectio_reddere, NIHIL, NIHIL, NIHIL, NIHIL },
    { "partem", _sectio_partem, NIHIL, NIHIL, NIHIL, NIHIL },
    { "tacitae", _sectio_tacitae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "cauda_aulae", _sectio_cauda_aulae, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("auscultatio", SECTIONES);
}

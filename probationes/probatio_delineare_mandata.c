/* probatio_delineare_mandata.c - mandata -> pixela (gradus VII):
 * asserta pixelorum sine exemplari, deinde specimen */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
#include "color.h"
#include "thema.h"
#include "fenestra.h"
#include "mandatum.h"
#include "delineare.h"
#include "delineare_mandata.h"
#include "exemplaria.h"
#include "imago_typus.h"
#include "specimen.h"
#include "glyphae_ductae.h"
#include "fons.h"
#include "tabula_pixelorum.h"
#include "credo.h"
#include <stdio.h>

/* Fons imaginum probationis: "quadrum" = 4x4, ruber cum angulo
 * transparente */
interior constans Imago*
fons_probationis (
    chorda  provenientia,
    vacuum* ctx)
{
    si (chorda_aequalis_literis(provenientia, "quadrum"))
    {
        redde (constans Imago*)ctx;
    }
    redde NIHIL;
}

interior ColorMandati
rgba (
    i8 r,
    i8 g,
    i8 b)
{
    ColorMandati c;

    c.genus = COLOR_MANDATI_RGBA;
    c.valor = color_ad_pixelum(color_ex_rgba(r, g, b, (i8)CCLV));
    redde c;
}

/* Textum per delineare_mandata in (X, V) pingere et cum mensura
 * moduli nativi conferre: 'extra' = pixela atramenti extra capsulam
 * mensam; 'ultima_columna' / 'ultima_linea' = atramentum in cellula
 * ultima capsulae (capsula ARTA, non solum continens). */
interior vacuum
_textum_conferre (
                 Piscina* piscina,
     InternamentumChorda* intern,
         TabulaPixelorum* u,
      constans character* cstr,
                     s32* extra,
                     b32* ultima_columna,
                     b32* ultima_linea)
{
    Mandata* m;
    Modulus  modulus;
        s32  lat;
        s32  alt;
        s32  x;
        s32  y;
        i32  niger;
        i32  pixelum;

    niger = color_ad_pixelum(color_ex_rgb((i8)ZEPHYRUM, (i8)ZEPHYRUM,
        (i8)ZEPHYRUM));
    tabula_pixelorum_vacare(u, niger);
    m = mandata_creare(piscina, intern);
    mandata_textus(m, X, V, chorda_ex_literis(cstr, piscina), ZEPHYRUM,
        rgba((i8)CCLV, (i8)CCLV, (i8)CCLV));
    delineare_mandata(m, u, NIHIL, NIHIL);
    modulus = delineare_mandata_modulus(u);
    modulus_textum_metiri(&modulus, delineare_mandata_mensor(),
        chorda_ex_literis(cstr, piscina), &lat, &alt);
    *extra           = ZEPHYRUM;
    *ultima_columna  = FALSUM;
    *ultima_linea    = FALSUM;
    per (y = ZEPHYRUM; y < (s32)u->altitudo; y++)
    {
        per (x = ZEPHYRUM; x < (s32)u->latitudo; x++)
        {
            pixelum = tabula_pixelorum_obtinere_pixelum(u, (i32)x,
                (i32)y);
            si (pixelum == niger)
            {
                perge;
            }
            si (x < X || x >= X + lat || y < V || y >= V + alt)
            {
                (*extra)++;
                perge;
            }
            si (x >= X + lat - modulus.cellula_latitudo)
            {
                *ultima_columna = VERUM;
            }
            si (y >= V + alt - modulus.cellula_altitudo)
            {
                *ultima_linea = VERUM;
            }
        }
    }
}

/* Fixura suitae: thema semel initiatur (ut olim ante omnia) */
interior vacuum
_suitam_parare (
    CredoContextus* c)
{
    (vacuum)c;
    thema_initiare();
}

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
                 Piscina* piscina;
         TabulaPixelorum* t;

    piscina  = c->piscina;
    t        = tabula_pixelorum_creare_nuda(piscina, LXIV, XLVIII);
    CREDO_NON_NIHIL(t);
    CREDO_AEQUALIS_I32(t->latitudo, LXIV);
    CREDO_AEQUALIS_I32(t->altitudo, XLVIII);
    tabula_pixelorum_vacare(t, color_ad_pixelum(color_ex_rgb(
        (i8)ZEPHYRUM, (i8)ZEPHYRUM, (i8)ZEPHYRUM)));
}

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
                 Piscina* piscina;
     InternamentumChorda* intern;
         TabulaPixelorum* t;
                 Mandata* m;
                   Fines  f;
                 Punctum  a;
                 Punctum  b;
                   Imago  quadrum;
                     i32  coetus;
                     i32  i;
                  chorda  provenientia;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    t        = tabula_pixelorum_creare_nuda(piscina, LXIV, XLVIII);
    tabula_pixelorum_vacare(t, color_ad_pixelum(color_ex_rgb(
        (i8)ZEPHYRUM, (i8)ZEPHYRUM, (i8)ZEPHYRUM)));

    /* quadrum 4x4 ruber, pixelum (0,0) transparens */
    quadrum.latitudo  = IV;
    quadrum.altitudo  = IV;
    quadrum.pixela    = (i8*)piscina_allocare(piscina, LXIV);
    per (i = ZEPHYRUM; i < XVI; i++)
    {
        quadrum.pixela[i * IV]        = (i8)CCLV;
        quadrum.pixela[i * IV + I]    = ZEPHYRUM;
        quadrum.pixela[i * IV + II]   = ZEPHYRUM;
        quadrum.pixela[i * IV + III]  = (i8)CCLV;
    }
    quadrum.pixela[III] = ZEPHYRUM;   /* alpha (0,0) = 0 */

    /* Mandata: rectangulum, linea, imago scalata, textus in coetu
     * translato */
    m    = mandata_creare(piscina, intern);
    f.x  = II; f.y = II; f.latitudo = X; f.altitudo = VI;
    mandata_rectangulum(m, f, rgba((i8)ZEPHYRUM, (i8)CCLV,
        (i8)ZEPHYRUM),
                        VERUM);
    a.x = ZEPHYRUM; a.y = XX; b.x = XX; b.y = XX;
    mandata_linea(m, a, b, I, rgba((i8)ZEPHYRUM, (i8)ZEPHYRUM,
        (i8)CCLV));
    /* coetus: origo (30,10), scala II, sectio ad (30,10,20,20) */
    f.x = XXX; f.y = X; f.latitudo = XX; f.altitudo = XX;
    provenientia = chorda_ex_literis("coetus_a", piscina);
    coetus = mandata_coetus_incipere(m, f, VERUM, ZEPHYRUM, ZEPHYRUM,
        II,
                                     provenientia);
    f.x = ZEPHYRUM; f.y = ZEPHYRUM; f.latitudo = IV; f.altitudo = IV;
    mandata_imago(m, chorda_ex_literis("quadrum", piscina), f);
    /* rectangulum extra sectionem: (15,15,10,10) locale -> schirmo
     * (60,40)..(80,60), praecisum ad 50,30 */
    f.x = XV; f.y = XV; f.latitudo = X; f.altitudo = X;
    mandata_rectangulum(m, f, rgba((i8)CCLV, (i8)CCLV, (i8)ZEPHYRUM),
                        VERUM);
    mandata_coetus_finire(m, coetus);
    mandata_textus(m, II, XXX, chorda_ex_literis("Ok", piscina),
        ZEPHYRUM,
                   rgba((i8)CCLV, (i8)CCLV, (i8)CCLV));

    delineare_mandata(m, t, fons_probationis, &quadrum);

    /* rectangulum plenum viride: intra (5,5), extra (13,5) */
    CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(t, V, V),
        color_ad_pixelum(color_ex_rgb((i8)ZEPHYRUM, (i8)CCLV,
        (i8)ZEPHYRUM)));
    CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(t, XIII, V),
        color_ad_pixelum(color_ex_rgb((i8)ZEPHYRUM, (i8)ZEPHYRUM,
        (i8)ZEPHYRUM)));
    /* linea caerulea per (10,20) */
    CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(t, X, XX),
        color_ad_pixelum(color_ex_rgb((i8)ZEPHYRUM, (i8)ZEPHYRUM,
        (i8)CCLV)));
    /* imago scalata II: pixelum (1,1) imaginis -> schirmo
     * (32..33, 12..13) ruber; pixelum (0,0) transparens -> schirmo
     * (30,10) manet niger */
    CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(t, XXXIII,
        XIII),
        color_ad_pixelum(color_ex_rgb((i8)CCLV, (i8)ZEPHYRUM,
        (i8)ZEPHYRUM)));
    CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(t, XXX, X),
        color_ad_pixelum(color_ex_rgb((i8)ZEPHYRUM, (i8)ZEPHYRUM,
        (i8)ZEPHYRUM)));
        /* sectio: flavum schirmo (60..80, 40..60) praecisum ad (50,30):
     * INTRA rectangulum flavum nihil flavum manet */
    CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(t, LXII, XLII),
        color_ad_pixelum(color_ex_rgb((i8)ZEPHYRUM, (i8)ZEPHYRUM,
        (i8)ZEPHYRUM)));
    CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(t, LXIII,
        XLVII),
        color_ad_pixelum(color_ex_rgb((i8)ZEPHYRUM, (i8)ZEPHYRUM,
        (i8)ZEPHYRUM)));
    /* textus: aliquid album in linea 30..37 */
    {
        b32 album;
        i32 x;
        album = FALSUM;
        per (x = II; x < XIV; x++)
        {
            i32 y;
            per (y = XXX; y < XXXVIII; y++)
            {
                si (tabula_pixelorum_obtinere_pixelum(t, x, y)
                    == color_ad_pixelum(color_ex_rgb((i8)CCLV, (i8)CCLV,
                                                     (i8)CCLV)))
                {
                    album = VERUM;
                }
            }
        }
        CREDO_VERUM(album);
    }
}

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
                 Piscina* piscina;
     InternamentumChorda* intern;
         TabulaPixelorum* t;
         TabulaPixelorum* u;
                 Modulus  modulus;
           ModulusMensor  mensor;
                     s32  extra;
                     b32  ultima_columna;
                     b32  ultima_linea;
                     i32  i;
      constans character* textus_probandi[VII];

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    t        = tabula_pixelorum_creare_nuda(piscina, LXIV, XLVIII);
    modulus  = delineare_mandata_modulus(t);
    CREDO_AEQUALIS_S32(modulus.cellula_latitudo, VI);
    CREDO_AEQUALIS_S32(modulus.cellula_altitudo, VIII);
    CREDO_AEQUALIS_S32(modulus.extensio_latitudo, LXIV);
    CREDO_AEQUALIS_S32(modulus.extensio_altitudo, XLVIII);
    mensor = delineare_mandata_mensor();
    CREDO_VERUM(mensor.genus == MODULUS_MENSOR_FONTIS);
    /* proprietas: textus pictus intra mensuram iacet, et arte */
    u = tabula_pixelorum_creare_nuda(piscina, CXXVIII, XL);
    CREDO_NON_NIHIL(u);
    textus_probandi[ZEPHYRUM]  = "Ok";
    textus_probandi[I]         = "Hg|";
    textus_probandi[II]        = "\xE4\xB8\xAD";        /* 中 -> TOFU */
    textus_probandi[III]       = "e\xCC\x81";            /* e + U+0301 */
    textus_probandi[IV]        = "a\nlonger";
    textus_probandi[V]         = "\xFF";
    textus_probandi[VI]        = "\xC3\xA9t\xC3\xA9";  /* été (Latin-1) */
    per (i = ZEPHYRUM; i < VII; i++)
    {
        _textum_conferre(piscina, intern, u, textus_probandi[i], &extra,
            &ultima_columna, &ultima_linea);
        si (extra != ZEPHYRUM || !ultima_columna || !ultima_linea)
        {
            imprimere("  textus %u: extra %d, ultima columna %d, ultima "
                "linea %d\n", (insignatus integer)i, (int)extra,
                (int)ultima_columna, (int)ultima_linea);
        }
        CREDO_AEQUALIS_S32(extra, ZEPHYRUM);
        CREDO_VERUM(ultima_columna);
        CREDO_VERUM(ultima_linea);
    }

    /* omnis glyphus fontis (ASCII imprimibile, Latin-1) intra
     * cellulam suam: progressus VI veritas fontis, non spes. EXCEPTIONES
     * NOMINATAE (inventae 2026-10-01, data HP 100LX originalia - et in
     * fons_6x8.h.backup): '>' et '}' apicem in columna VI ponunt (bitum
     * 0x02), pixelum unum in cellulam proximam. Glyphus alius qui
     * effluit, aut exceptio emendata, hanc assertionem frangit. */
    {
        character unus[III];
              s32 effluentes;
              b32 exceptio;
              i32 r;

        effluentes = ZEPHYRUM;
        per (r = 0x21; r <= 0xFF; r++)
        {
            si (r >= 0x7F && r < 0xA1)
            {
                perge;
            }
            si (r < 0x80)
            {
                unus[ZEPHYRUM]  = (character)r;
                unus[I]         = '\0';
            }
            alioquin
            {
                unus[ZEPHYRUM]  = (character)(0xC0 | (r >> VI));
                unus[I]         = (character)(0x80 | (r & 0x3F));
                unus[II]        = '\0';
            }
            _textum_conferre(piscina, intern, u, unus, &extra,
                &ultima_columna, &ultima_linea);
            exceptio = (b32)(r == 0x3E || r == 0x7D);
            si ((extra != ZEPHYRUM) != exceptio)
            {
                imprimere("  U+%04X: %d pixela extra cellulam\n",
                    (insignatus integer)r, (int)extra);
                effluentes++;
            }
        }
        CREDO_AEQUALIS_S32(effluentes, ZEPHYRUM);
    }
}

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
                 Piscina* piscina;
     InternamentumChorda* intern;
         TabulaPixelorum* t;
                 Mandata* m;
                   Fines  f;
                 Punctum  a;
                 Punctum  b;
                   Imago  quadrum;
                     i32  coetus;
                     i32  i;
                  chorda  provenientia;
                   Imago  captura;
         SpecimenFructus  sf;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    t        = tabula_pixelorum_creare_nuda(piscina, LXIV, XLVIII);
    tabula_pixelorum_vacare(t, color_ad_pixelum(color_ex_rgb(
        (i8)ZEPHYRUM, (i8)ZEPHYRUM, (i8)ZEPHYRUM)));

    /* quadrum 4x4 ruber, pixelum (0,0) transparens */
    quadrum.latitudo  = IV;
    quadrum.altitudo  = IV;
    quadrum.pixela    = (i8*)piscina_allocare(piscina, LXIV);
    per (i = ZEPHYRUM; i < XVI; i++)
    {
        quadrum.pixela[i * IV]        = (i8)CCLV;
        quadrum.pixela[i * IV + I]    = ZEPHYRUM;
        quadrum.pixela[i * IV + II]   = ZEPHYRUM;
        quadrum.pixela[i * IV + III]  = (i8)CCLV;
    }
    quadrum.pixela[III] = ZEPHYRUM;   /* alpha (0,0) = 0 */

    /* Mandata: rectangulum, linea, imago scalata, textus in coetu
     * translato */
    m    = mandata_creare(piscina, intern);
    f.x  = II; f.y = II; f.latitudo = X; f.altitudo = VI;
    mandata_rectangulum(m, f, rgba((i8)ZEPHYRUM, (i8)CCLV,
        (i8)ZEPHYRUM),
                        VERUM);
    a.x = ZEPHYRUM; a.y = XX; b.x = XX; b.y = XX;
    mandata_linea(m, a, b, I, rgba((i8)ZEPHYRUM, (i8)ZEPHYRUM,
        (i8)CCLV));
    /* coetus: origo (30,10), scala II, sectio ad (30,10,20,20) */
    f.x = XXX; f.y = X; f.latitudo = XX; f.altitudo = XX;
    provenientia = chorda_ex_literis("coetus_a", piscina);
    coetus = mandata_coetus_incipere(m, f, VERUM, ZEPHYRUM, ZEPHYRUM,
        II,
                                     provenientia);
    f.x = ZEPHYRUM; f.y = ZEPHYRUM; f.latitudo = IV; f.altitudo = IV;
    mandata_imago(m, chorda_ex_literis("quadrum", piscina), f);
    /* rectangulum extra sectionem: (15,15,10,10) locale -> schirmo
     * (60,40)..(80,60), praecisum ad 50,30 */
    f.x = XV; f.y = XV; f.latitudo = X; f.altitudo = X;
    mandata_rectangulum(m, f, rgba((i8)CCLV, (i8)CCLV, (i8)ZEPHYRUM),
                        VERUM);
    mandata_coetus_finire(m, coetus);
    mandata_textus(m, II, XXX, chorda_ex_literis("Ok", piscina),
        ZEPHYRUM,
                   rgba((i8)CCLV, (i8)CCLV, (i8)CCLV));

    delineare_mandata(m, t, fons_probationis, &quadrum);

    captura  = imago_ex_tabula(t);
    CREDO_AEQUALIS_I32(captura.latitudo, LXIV);
    sf = specimen_iudicare(&captura, "mandata_prima",
        specimen_regula_solita("probationes/pictor/specimina"),
        piscina);
    si (sf.sententia != SPECIMEN_CONGRUIT)
    {
        imprimere("SPECIMEN %s: %.*s\n",
                  specimen_sententia_nomen(sf.sententia),
                  (int)sf.causa.mensura, sf.causa.datum);
    }
    CREDO_VERUM(sf.sententia == SPECIMEN_CONGRUIT);
}

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        /* rectangulum plenum solidum per ordines impletur (terminale
         * park 011): pixela visibilia = rectangulum ^ tabula ^
         * praecisio - margo quisque praecisionis */
        TabulaPixelorum* tp = tabula_pixelorum_creare_nuda(piscina, XX,
            XX);
     ContextusDelineandi* cp = delineare_creare_contextum(piscina, tp);
                     i32  niger;
                     i32  ruber;

        niger = color_ad_pixelum(color_ex_rgb((i8)ZEPHYRUM,
            (i8)ZEPHYRUM, (i8)ZEPHYRUM));
        ruber = color_ad_pixelum(color_ex_rgb((i8)CCLV, (i8)ZEPHYRUM,
            (i8)ZEPHYRUM));
        tabula_pixelorum_vacare(tp, niger);
        delineare_ponere_praecisionem(cp, V, V, VI, VI);
        delineare_rectangulum_plenum(cp, ZEPHYRUM, ZEPHYRUM, XX, XX,
            color_ex_rgb((i8)CCLV, (i8)ZEPHYRUM, (i8)ZEPHYRUM));
        CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(tp, V, V),
            ruber);
        CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(tp, X, X),
            ruber);
        CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(tp, IV, V),
            niger);
        CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(tp, V, IV),
            niger);
        CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(tp, XI, X),
            niger);
        CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(tp, X, XI),
            niger);
    }
}

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        TabulaPixelorum* tn = tabula_pixelorum_creare_nuda(piscina, XX,
            XX);
                 Mandata* mn;
                   Fines  fn;
                 Punctum  pa;
                 Punctum  pb;
                 Punctum  pp[IV];
                     i32  fundus;
                     i32  rubeus;
                     i32  albus;
          memoriae_index  fracti_ante;
                     s32  x;
                     s32  y;
                     b32  lucet;

        fundus = color_ad_pixelum(color_ex_rgb((i8)ZEPHYRUM,
            (i8)ZEPHYRUM,
            (i8)ZEPHYRUM));
        rubeus  = rgba((i8)CCLV, (i8)ZEPHYRUM, (i8)ZEPHYRUM).valor;
        albus   = rgba((i8)CCLV, (i8)CCLV, (i8)CCLV).valor;

        /* I rectangulum plenum (-5,-5,10,10): pars (0..4, 0..4) */
        tabula_pixelorum_vacare(tn, fundus);
        mn    = mandata_creare(piscina, intern);
        fn.x  = -V; fn.y = -V; fn.latitudo = X; fn.altitudo = X;
        mandata_rectangulum(mn, fn, rgba((i8)CCLV, (i8)ZEPHYRUM,
            (i8)ZEPHYRUM), VERUM);
        delineare_mandata(mn, tn, NIHIL, NIHIL);
        CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(tn,
            ZEPHYRUM,
            ZEPHYRUM), rubeus);
        CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(tn, IV,
            IV),
            rubeus);
        CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(tn, V, V),
            fundus);

        /* II margo (-5,2,10,5): superior y=2 et inferior y=6 (x 0..4),
         * dextra x=4 (y 3..5); intus fundus */
        tabula_pixelorum_vacare(tn, fundus);
        mn    = mandata_creare(piscina, intern);
        fn.x  = -V; fn.y = II; fn.latitudo = X; fn.altitudo = V;
        mandata_rectangulum(mn, fn, rgba((i8)CCLV, (i8)ZEPHYRUM,
            (i8)ZEPHYRUM), FALSUM);
        delineare_mandata(mn, tn, NIHIL, NIHIL);
        CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(tn,
            ZEPHYRUM,
            II), rubeus);
        CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(tn, II,
            VI),
            rubeus);
        CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(tn, IV,
            IV),
            rubeus);
        CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(tn, II,
            IV),
            fundus);

        /* III linea (-5,10)->(10,10): NON PENDET, deinde pixela
         * 0..10 */
        tabula_pixelorum_vacare(tn, fundus);
        mn    = mandata_creare(piscina, intern);
        pa.x  = -V; pa.y = X; pb.x = X; pb.y = X;
        mandata_linea(mn, pa, pb, I, rgba((i8)CCLV, (i8)ZEPHYRUM,
            (i8)ZEPHYRUM));
        fracti_ante = credo_numerus_fracti();
        CREDO_NON_PENDET(delineare_mandata(mn, tn, NIHIL, NIHIL), MM);
        si (credo_numerus_fracti() == fracti_ante)
        {
            delineare_mandata(mn, tn, NIHIL, NIHIL);
            CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(tn,
                ZEPHYRUM, X), rubeus);
            CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(tn, X,
                X), rubeus);
            CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(tn, XI,
                X), fundus);
        }

        /* IV polygonum plenum cum verticibus negativis */
        tabula_pixelorum_vacare(tn, fundus);
        mn       = mandata_creare(piscina, intern);
        pp[0].x  = -V; pp[0].y = -V;
        pp[1].x = X;  pp[1].y = -V;
        pp[2].x = X;  pp[2].y = X;
        pp[3].x = -V; pp[3].y = X;
        mandata_polygonum(mn, pp, IV, rgba((i8)CCLV, (i8)ZEPHYRUM,
            (i8)ZEPHYRUM), VERUM);
        fracti_ante = credo_numerus_fracti();
        CREDO_NON_PENDET(delineare_mandata(mn, tn, NIHIL, NIHIL), MM);
        si (credo_numerus_fracti() == fracti_ante)
        {
            delineare_mandata(mn, tn, NIHIL, NIHIL);
            CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(tn,
                ZEPHYRUM, ZEPHYRUM), rubeus);
            CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(tn, V,
                V),
                rubeus);
            CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(tn, XV,
                XV), fundus);
        }

        /* V textus "M" ad x = -3: columnae visibiles pinguntur (pixela
         * per additionem cum involutione et finium probationem - hodie
         * iam rectum; hic FIXUM, non rubrum) */
        tabula_pixelorum_vacare(tn, fundus);
        mn = mandata_creare(piscina, intern);
        mandata_textus(mn, -III, ZEPHYRUM, chorda_ex_literis("M",
            piscina),
            ZEPHYRUM, rgba((i8)CCLV, (i8)CCLV, (i8)CCLV));
        delineare_mandata(mn, tn, NIHIL, NIHIL);
        lucet = FALSUM;
        per (y = ZEPHYRUM; y < VIII; y++)
        {
            per (x = ZEPHYRUM; x < III; x++)
            {
                si (tabula_pixelorum_obtinere_pixelum(tn, (i32)x,
                    (i32)y)
                    == albus)
                {
                    lucet = VERUM;
                }
            }
        }
        CREDO_VERUM(lucet);
    }
}

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        TabulaPixelorum* te = tabula_pixelorum_creare_nuda(piscina, XX,
            XX);
                 Mandata* me;
                   Fines  fe;
                   Fines  fc;
                     i32  fundus;
                     i32  rubeus;
                     s32  x;
                     s32  y;
                     i32  errores;
                     i32  picti;

        fundus = color_ad_pixelum(color_ex_rgb((i8)ZEPHYRUM,
            (i8)ZEPHYRUM, (i8)ZEPHYRUM));
        rubeus = rgba((i8)CCLV, (i8)ZEPHYRUM, (i8)ZEPHYRUM).valor;
        tabula_pixelorum_vacare(te, fundus);
        me = mandata_creare(piscina, intern);
        /* coetus translatus (III, II): rectangulum locale (-VI, I, XII,
         * X) = tabula (-III..8, 3..12) - praecisum ad x 0 */
        fc.x = ZEPHYRUM; fc.y = ZEPHYRUM; fc.latitudo  = XX;
        fc.altitudo                                    = XX;
        {
            i32 c;

            c = mandata_coetus_incipere(me, fc, FALSUM, III, II, I,
                chorda_ex_literis("coetus", piscina));
            fe.x = -VI; fe.y = I; fe.latitudo = XII; fe.altitudo = X;
            mandata_rectangulum_exemplar(me, fe, rgba((i8)CCLV,
                (i8)ZEPHYRUM, (i8)ZEPHYRUM),
                EXEMPLAR_PUNCTA_DUPLICIA_DISPERSA);
            mandata_coetus_finire(me, c);
        }
        delineare_mandata(me, te, NIHIL, NIHIL);
        errores  = ZEPHYRUM;
        picti    = ZEPHYRUM;
        per (y = ZEPHYRUM; y < XX; y++)
        {
            per (x = ZEPHYRUM; x < XX; x++)
            {
                b32 intus;
                b32 debet;

                intus = x < IX && y >= III && y < XIII;
                debet = intus && exemplar_punctum(
                    EXEMPLAR_PUNCTA_DUPLICIA_DISPERSA, x, y);
                si (tabula_pixelorum_obtinere_pixelum(te, (i32)x,
                    (i32)y)
                    != (debet ? rubeus : fundus))
                {
                    errores++;
                }
                si (debet)
                {
                    picti++;
                }
            }
        }
        CREDO_AEQUALIS_I32(errores, ZEPHYRUM);
        CREDO_VERUM(picti > ZEPHYRUM);
        /* id invalidum: nihil */
        tabula_pixelorum_vacare(te, fundus);
        me = mandata_creare(piscina, intern);
        mandata_rectangulum_exemplar(me, fc, rgba((i8)CCLV,
            (i8)ZEPHYRUM,
            (i8)ZEPHYRUM), CC);
        delineare_mandata(me, te, NIHIL, NIHIL);
        CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(te,
            ZEPHYRUM,
            ZEPHYRUM), fundus);
    }
}

/* glyphae ductae (terminale: capsae, quadra, braille, symbola) etiam in
 * textu mandatorum et tabulae: pixela ex larva glyphae_ductae, non
 * TOFU. Plenum = color litterae, vacuum = fundus, partiale = mixtum */
interior b32
larvam_congruere (
    TabulaPixelorum* t,
                i32  x,
                i32  y,
                s32  runa,
                i32  cw,
                i32  ch,
                i32  scala,
                i32  littera,
                i32  fundus)
{
     i8 larva[CXXVIII];
    i32 px;
    i32 py;
    i32 p;

    si (!glyphae_ductae_pingere(runa, cw, ch, larva))
    {
        redde FALSUM;
    }
    per (py = ZEPHYRUM; py < ch; py++)
    {
        per (px = ZEPHYRUM; px < cw; px++)
        {
            p = tabula_pixelorum_obtinere_pixelum(t, x + px * scala,
                y + py * scala);
            si (larva[py * cw + px] == (i8)0xFF && p != littera)
            {
                redde FALSUM;
            }
            si (larva[py * cw + px] == ZEPHYRUM && p != fundus)
            {
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
                 Piscina* piscina;
     InternamentumChorda* intern;
         TabulaPixelorum* t;
                 Mandata* m;
                     i32  albus;
                     i32  niger;
                     i32  p;
                     i32  mixta;
                     i32  px;
                     i32  py;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    t        = tabula_pixelorum_creare_nuda(piscina, LXIV, XLVIII);
    niger    = color_ad_pixelum(color_ex_rgb((i8)ZEPHYRUM, (i8)ZEPHYRUM,
        (i8)ZEPHYRUM));
    albus    = color_ad_pixelum(color_ex_rgb((i8)CCLV, (i8)CCLV,
        (i8)CCLV));
    tabula_pixelorum_vacare(t, niger);
    m = mandata_creare(piscina, intern);
    /* U+2713 check mark, U+2591 umbra levis */
    mandata_textus(m, ZEPHYRUM, ZEPHYRUM,
        chorda_ex_literis("\xE2\x9C\x93",
        piscina), FONS_PARVUS, rgba((i8)CCLV, (i8)CCLV, (i8)CCLV));
    mandata_textus(m, ZEPHYRUM, XVI, chorda_ex_literis("\xE2\x9C\x93",
        piscina), FONS_TITULI, rgba((i8)CCLV, (i8)CCLV, (i8)CCLV));
    mandata_textus(m, XX, ZEPHYRUM, chorda_ex_literis("\xE2\x96\x91",
        piscina), FONS_PARVUS, rgba((i8)CCLV, (i8)CCLV, (i8)CCLV));
    delineare_mandata(m, t, NIHIL, NIHIL);

    CREDO_GRADUS("check mark VI x VIII ex larva");
    CREDO_VERUM(larvam_congruere(t, ZEPHYRUM, ZEPHYRUM, 0x2713, VI,
        VIII,
        I, albus, niger));

    CREDO_GRADUS("check mark VIII x XVI (fons titulorum) ex larva");
    CREDO_VERUM(larvam_congruere(t, ZEPHYRUM, XVI, 0x2713, VIII, XVI, I,
        albus, niger));

    CREDO_GRADUS("umbra: pixela mixta (nec fundus nec littera)");
    mixta = ZEPHYRUM;
    per (py = ZEPHYRUM; py < VIII; py++)
    {
        per (px = ZEPHYRUM; px < VI; px++)
        {
            p = tabula_pixelorum_obtinere_pixelum(t, XX + px, py);
            si (p != albus && p != niger)
            {
                mixta++;
            }
        }
    }
    CREDO_VERUM(mixta > ZEPHYRUM);

    CREDO_GRADUS("chorda scalata II: larva in quadris II x II");
    tabula_pixelorum_vacare(t, niger);
    tabula_pixelorum_pingere_chordam_scalatam(t, ZEPHYRUM, ZEPHYRUM,
        chorda_ex_literis("\xE2\x9C\x93", piscina), albus, II);
    CREDO_VERUM(larvam_congruere(t, ZEPHYRUM, ZEPHYRUM, 0x2713, VI,
        VIII,
        II, albus, niger));
    CREDO_VERUM(larvam_congruere(t, I, I, 0x2713, VI, VIII, II, albus,
        niger));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "Tabula nuda", _sectio_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Asserta pixelorum (sine exemplari)", _sectio_secunda, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "Modulus nativus (tessellatio T2)", _sectio_tertia, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "Specimen (gradus VII)", _sectio_quarta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "praecisio, via celeris (park 011)", _sectio_quinta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "013: origo NEGATIVA - primitiva praeciduntur, non evanescunt",
        _sectio_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "exemplar (indicium foci): tabulae alligatum", _sectio_septima,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "glyphae ductae (non TOFU)", _sectio_octava, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("delineare_mandata", SECTIONES,
        _suitam_parare, NIHIL);
}

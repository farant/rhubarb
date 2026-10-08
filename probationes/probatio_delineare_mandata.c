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
#include "imago_typus.h"
#include "specimen.h"
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

s32 principale (vacuum)
{
                 Piscina* piscina;
     InternamentumChorda* intern;
         TabulaPixelorum* t;
                 Mandata* m;
                   Fines  f;
                 Punctum  a;
                 Punctum  b;
                   Imago  quadrum;
                   Imago  captura;
                     i32  coetus;
                     i32  i;
         SpecimenFructus  sf;
                  chorda  provenientia;
         TabulaPixelorum* u;
                 Modulus  modulus;
           ModulusMensor  mensor;
                     s32  extra;
                     b32  ultima_columna;
                     b32  ultima_linea;
                     i32  c;
      constans character* textus_probandi[VII];

    piscina = piscina_generare_dynamicum("probatio_delineare_mandata",
        XVI * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();

    imprimere("\n--- Tabula nuda ---\n");
    t = tabula_pixelorum_creare_nuda(piscina, LXIV, XLVIII);
    CREDO_NON_NIHIL(t);
    CREDO_AEQUALIS_I32(t->latitudo, LXIV);
    CREDO_AEQUALIS_I32(t->altitudo, XLVIII);
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

    imprimere("\n--- Mandata: rectangulum, linea, imago scalata, textus"
              " in coetu translato ---\n");
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

    imprimere("\n--- Asserta pixelorum (sine exemplari) ---\n");
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

    imprimere("\n--- Modulus nativus (tessellatio T2) ---\n");
    modulus = delineare_mandata_modulus(t);
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
    per (c = ZEPHYRUM; c < VII; c++)
    {
        _textum_conferre(piscina, intern, u, textus_probandi[c], &extra,
            &ultima_columna, &ultima_linea);
        si (extra != ZEPHYRUM || !ultima_columna || !ultima_linea)
        {
            imprimere("  textus %u: extra %d, ultima columna %d, ultima "
                "linea %d\n", (insignatus integer)c, (int)extra,
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

    imprimere("\n--- Specimen (gradus VII) ---\n");
    captura = imago_ex_tabula(t);
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

    imprimere("\n--- praecisio, via celeris (park 011) ---\n");
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

    imprimere("\n--- 013: origo NEGATIVA - primitiva praeciduntur, non"
              " evanescunt ---\n");
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

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}

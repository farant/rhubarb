/* probatio_figura.c - registrum figurarum, pingere (gradus VI) */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
#include "thema.h"
#include "componens.h"
#include "mandatum.h"
#include "figura.h"
#include "exemplaria.h"
#include "credo.h"
#include <stdio.h>

/* Figura probationis: rectangulum plenum coloris thematis + titulus */
interior vacuum
figura_bottonis_probationis (
    constans Componens* c,
               Mandata* m,
                   i32  thema,
                vacuum* ctx)
{
           Fines f;
    ColorMandati color;

    (vacuum)thema;
    (*(i32*)ctx)++;
    f.x          = ZEPHYRUM;
    f.y          = ZEPHYRUM;
    f.latitudo   = c->fines.latitudo;
    f.altitudo   = c->fines.altitudo;
    color.genus  = COLOR_MANDATI_THEMA;
    color.valor  = (i32)COLOR_ACCENT_PRIMARY;
    mandata_rectangulum(m, f, color, VERUM);
    color.valor = (i32)COLOR_TEXT;
    mandata_textus(m, ZEPHYRUM, ZEPHYRUM, c->titulus, ZEPHYRUM, color);
}

interior Componens*
nodus (
                Piscina* p,
    InternamentumChorda* in,
     constans character* id,
                 Partes  partes,
                    s32  x,
                    s32  y,
                    s32  w,
                    s32  h)
{
     Componens* c;
         Fines  f;

    c           = componens_creare(p, in, id, partes);
    f.x         = x;
    f.y         = y;
    f.latitudo  = w;
    f.altitudo  = h;
    componens_ponere_fines(c, f);
    redde c;
}

/* linea verticalis ad x (margo segmenti) ? */
interior b32
linea_ad_x_adest (
     Mandata* v,
         s32  x)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(v->elementa); i++)
    {
        constans Mandatum* y;

        y = (constans Mandatum*)xar_obtinere(v->elementa, i);
        si (   y->genus == MANDATUM_LINEA && y->puncta[ZEPHYRUM].x == x
            && y->puncta[I].x == x)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;
        FiguraRegistrum* reg;
              Componens* radix;
              Componens* pannus;
              Componens* b1;
                Mandata* m;
                Mandata* m2;
               Mandatum* x;
               FiguraFn  fn;
                 vacuum* ctx;
                    i32  vocationes;

    piscina     = c->piscina;
    intern      = internamentum_creare(piscina);
    vocationes  = ZEPHYRUM;

    reg = figura_registrum_creare(piscina);
    CREDO_NON_NIHIL(reg);
    CREDO_VERUM(figura_registrare(reg, PARTES_BOTTONE, ZEPHYRUM,
                                  figura_bottonis_probationis,
                                  &vocationes));
    /* duplex (partes, thema) recusatur */
    CREDO_FALSUM(figura_registrare(reg, PARTES_BOTTONE, ZEPHYRUM,
                                   figura_bottonis_probationis,
                                   &vocationes));
    /* idem partes, thema aliud: licet */
    CREDO_VERUM(figura_registrare(reg, PARTES_BOTTONE, I, figura_finium,
                                  NIHIL));
        CREDO_FALSUM(figura_registrare(reg, PARTES_CAMPUS, ZEPHYRUM,
            NIHIL,
                                   NIHIL));
    /* pannus (dialogus) figuram ET liberos habet: ordo z probatur */
    CREDO_VERUM(figura_registrare(reg, PARTES_DIALOGUS, ZEPHYRUM,
                                  figura_finium, NIHIL));
    fn   = NIHIL;
    ctx  = NIHIL;
    CREDO_VERUM(figura_invenire(reg, PARTES_BOTTONE, ZEPHYRUM, &fn,
        &ctx));
    CREDO_VERUM(fn == figura_bottonis_probationis);
    CREDO_VERUM(ctx == (vacuum*)&vocationes);
    CREDO_FALSUM(figura_invenire(reg, PARTES_TABULA, ZEPHYRUM, &fn,
        &ctx));

    /* radix (0,0,200,100)
     *   pannus (70,0,130,100) translatio (10,50) scala II sectio
     *     b1 (5,5,40,20) bottone "Ok" */
    radix  = nodus(piscina, intern, "radix", PARTES_NULLUM,
                   ZEPHYRUM, ZEPHYRUM, CC, C);
        pannus = nodus(piscina, intern, "pannus", PARTES_DIALOGUS,
                   LXX, ZEPHYRUM, CXXX, C);
    componens_ponere_transformatio(pannus, X, L, II);
    componens_ponere_sectio(pannus, VERUM);
    b1 = nodus(piscina, intern, "b1", PARTES_BOTTONE, V, V, XL, XX);
    componens_ponere_titulum(b1, "Ok");
    componens_addere_liberum(radix, pannus);
    componens_addere_liberum(pannus, b1);

    CREDO_GRADUS("Pingere: coetus per componens, figura ante liberos");
    m = mandata_creare(piscina, intern);
    pingere(radix, reg, ZEPHYRUM, m);
    CREDO_AEQUALIS_I32(vocationes, I);
        /* coetus radix, coetus pannus, RECT panni (figura ANTE
         * liberos), coetus b1, rect b1, textus = VI */
    CREDO_AEQUALIS_I32(mandata_numerus(m), VI);
    x = mandata_obtinere(m, ZEPHYRUM);
    CREDO_VERUM(x->genus == MANDATUM_COETUS);
    CREDO_CHORDA_AEQUALIS_LITERIS(x->provenientia, "radix");
    CREDO_AEQUALIS_I32(x->magnitudo_arboris, VI);
    x = mandata_obtinere(m, I);
    CREDO_VERUM(x->genus == MANDATUM_COETUS);
    CREDO_CHORDA_AEQUALIS_LITERIS(x->provenientia, "pannus");
    CREDO_AEQUALIS_S32(x->fines.x, LXX);
    CREDO_AEQUALIS_S32(x->translatio.x, X);
    CREDO_AEQUALIS_S32(x->translatio.y, L);
    CREDO_AEQUALIS_I32(x->scala, II);
        CREDO_VERUM(x->sectio);
    CREDO_AEQUALIS_I32(x->magnitudo_arboris, V);
    x = mandata_obtinere(m, II);
    CREDO_VERUM(x->genus == MANDATUM_RECTANGULUM);   /* fines panni */
    CREDO_FALSUM(x->impletum);
    CREDO_AEQUALIS_S32(x->fines.latitudo, CXXX);
    x = mandata_obtinere(m, III);
    CREDO_VERUM(x->genus == MANDATUM_COETUS);
    CREDO_CHORDA_AEQUALIS_LITERIS(x->provenientia, "b1");
    CREDO_AEQUALIS_I32(x->magnitudo_arboris, III);
    x = mandata_obtinere(m, IV);
    CREDO_VERUM(x->genus == MANDATUM_RECTANGULUM);
    CREDO_VERUM(x->impletum);
    /* in spatio PROPRIO: origo (0,0), non (5,5) */
    CREDO_AEQUALIS_S32(x->fines.x, ZEPHYRUM);
    CREDO_AEQUALIS_S32(x->fines.latitudo, XL);
    CREDO_VERUM(x->color.genus == COLOR_MANDATI_THEMA);
        x = mandata_obtinere(m, V);
    CREDO_VERUM(x->genus == MANDATUM_TEXTUS);
    CREDO_CHORDA_AEQUALIS_LITERIS(x->textus, "Ok");

    CREDO_GRADUS("Thema alterum: figura alia");
    m = mandata_creare(piscina, intern);
    pingere(radix, reg, I, m);
    CREDO_AEQUALIS_I32(vocationes, I);              /* non vocata */
    /* figura_finium: rectangulum vacuum unum */
    CREDO_AEQUALIS_I32(mandata_numerus(m), IV);
    x = mandata_obtinere(m, III);
    CREDO_VERUM(x->genus == MANDATUM_RECTANGULUM);
    CREDO_FALSUM(x->impletum);

    CREDO_GRADUS("Partes sine figura: coetus solus");
    m = mandata_creare(piscina, intern);
    pingere(radix, reg, II, m);
    CREDO_AEQUALIS_I32(mandata_numerus(m), III);

    CREDO_GRADUS("Puritas: bis idem");
    m   = mandata_creare(piscina, intern);
    m2  = mandata_creare(piscina, intern);
    pingere(radix, reg, ZEPHYRUM, m);
    pingere(radix, reg, ZEPHYRUM, m2);
    CREDO_VERUM(mandata_aequalia(m, m2));
    CREDO_AEQUALIS_I32(vocationes, III);
}

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
         FiguraRegistrum* hospes;
         FiguraRegistrum* app;
                     i32  signum;
                FiguraFn  f;
                  vacuum* c;

        hospes  = figura_registrum_creare(piscina);
        app     = figura_registrum_creare(piscina);
        CREDO_VERUM(figura_registrare(hospes, PARTES_INDEX, ZEPHYRUM,
            figura_finium, NIHIL));
        CREDO_VERUM(figura_registrare(app, PARTES_CAMPUS, ZEPHYRUM,
            figura_finium, &signum));
        CREDO_VERUM(figura_registrum_miscere(hospes, app));
        CREDO_VERUM(figura_invenire(hospes, PARTES_CAMPUS, ZEPHYRUM, &f,
            &c));
        CREDO_VERUM(c == &signum);
        CREDO_FALSUM(figura_registrum_miscere(hospes, app));
        figura_registrum_vacare(hospes);
        CREDO_FALSUM(figura_invenire(hospes, PARTES_INDEX, ZEPHYRUM, &f,
            &c));
        CREDO_VERUM(figura_registrare(hospes, PARTES_INDEX, ZEPHYRUM,
            figura_finium, NIHIL));
    }
}

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);

    /* figura_mensam_pingere: superficies per fines, exemplar foci
     * supra solum si focatum */
    {
                   Mandata* mandata_mensae;
         constans Mandatum* y;
                     Fines  f;

        f.x             = II;
        f.y             = III;
        f.latitudo      = XL;
        f.altitudo      = XXX;
        mandata_mensae  = mandata_creare(piscina, intern);
        figura_mensam_pingere(mandata_mensae, f, FALSUM);
        CREDO_AEQUALIS_I32(xar_numerus(mandata_mensae->elementa), I);
        y = (constans Mandatum*)xar_obtinere(mandata_mensae->elementa,
            ZEPHYRUM);
        CREDO_VERUM(y->genus == MANDATUM_RECTANGULUM && y->impletum);
        CREDO_VERUM(y->fines.x == II && y->fines.latitudo == XL);
        CREDO_VERUM(   y->color.genus == COLOR_MANDATI_THEMA
                    && y->color.valor == (i32)COLOR_SUPERFICIES);
        mandata_mensae = mandata_creare(piscina, intern);
        figura_mensam_pingere(mandata_mensae, f, VERUM);
        CREDO_AEQUALIS_I32(xar_numerus(mandata_mensae->elementa), II);
        y = (constans Mandatum*)xar_obtinere(mandata_mensae->elementa,
            I);
        CREDO_VERUM(y->exemplar == FIGURA_EXEMPLAR_FOCI);
        CREDO_VERUM(   y->color.genus == COLOR_MANDATI_THEMA
                    && y->color.valor == (i32)COLOR_BORDER);
        CREDO_VERUM(y->fines.y == III && y->fines.altitudo == XXX);
    }
}

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
    (vacuum)c;
    /* figura_color_thematis: genus thematis, valor = color datus */
    {
        ColorMandati cm;

        cm = figura_color_thematis(COLOR_ACCENT_PRIMARY);
        CREDO_VERUM(cm.genus == COLOR_MANDATI_THEMA);
        CREDO_AEQUALIS_I32((i32)cm.valor, (i32)COLOR_ACCENT_PRIMARY);
    }
}

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);

    /* instrumenta visus (V1): globulus, sagitta, segmentum, capsa,
     * ordo electus, fascia, umbra, campus, textus audax */
    {
                    Mandata* v;
          constans Mandatum* y;
                      Fines  f;
                     chorda  t;

        f.x         = X;
        f.y         = XX;
        f.latitudo  = XL;
        f.altitudo  = XVI;
        t           = chorda_ex_literis("Today", piscina);
        /* globulus planus: fundus, margo, textus centratus */
        v = mandata_creare(piscina, intern);
        figura_globulum_pingere(v, f, t, ZEPHYRUM);
        CREDO_AEQUALIS_I32(xar_numerus(v->elementa), III);
        y = (constans Mandatum*)xar_obtinere(v->elementa, ZEPHYRUM);
        CREDO_VERUM(y->impletum
            && y->color.valor == (i32)COLOR_BACKGROUND);
        y = (constans Mandatum*)xar_obtinere(v->elementa, I);
        CREDO_VERUM(!y->impletum && y->color.valor == (i32)COLOR_TEXT);
        y = (constans Mandatum*)xar_obtinere(v->elementa, II);
        CREDO_VERUM(y->genus == MANDATUM_TEXTUS);
        CREDO_AEQUALIS_S32(y->fines.x, X + (XL - V * VI) / II);
        CREDO_VERUM(chorda_aequalis_literis(y->textus, "Today"));
        /* electum cum umbra: umbra primum (intra fines), corpus II
         * minus, fundus selectionis */
        v = mandata_creare(piscina, intern);
        figura_globulum_pingere(v, f, t, FIGURA_ELECTUM | FIGURA_UMBRA);
        y = (constans Mandatum*)xar_obtinere(v->elementa, ZEPHYRUM);
        CREDO_VERUM(y->impletum && y->color.valor == (i32)COLOR_TEXT);
        CREDO_AEQUALIS_S32(y->fines.x + y->fines.latitudo, X + XL);
        CREDO_AEQUALIS_S32(y->fines.y + y->fines.altitudo, XX + XVI);
        y = (constans Mandatum*)xar_obtinere(v->elementa, I);
        CREDO_VERUM(y->color.valor == (i32)COLOR_SELECTION);
        CREDO_AEQUALIS_S32(y->fines.latitudo, XL - II);
        /* obscurum: textus obscurus */
        v = mandata_creare(piscina, intern);
        figura_globulum_pingere(v, f, t, FIGURA_OBSCURUM);
        y = (constans Mandatum*)xar_obtinere(v->elementa, II);
        CREDO_VERUM(y->color.valor == (i32)COLOR_TEXT_DIM);
        /* sagitta: polygonum plenum */
        v = mandata_creare(piscina, intern);
        figura_sagittam_pingere(v, f, I, ZEPHYRUM);
        y = (constans Mandatum*)xar_obtinere(v->elementa,
            xar_numerus(v->elementa) - I);
        CREDO_VERUM(y->genus == MANDATUM_POLYGONUM && y->impletum);
        /* segmentum ultimum: margo dexter adest, medium sine eo */
        v = mandata_creare(piscina, intern);
        figura_segmentum_pingere(v, f, t, FIGURA_SEGMENTUM_MEDIUM,
            ZEPHYRUM);
        CREDO_FALSUM(linea_ad_x_adest(v, X + XL - I));
        v = mandata_creare(piscina, intern);
        figura_segmentum_pingere(v, f, t, FIGURA_SEGMENTUM_ULTIMUM,
            ZEPHYRUM);
        CREDO_VERUM(linea_ad_x_adest(v, X + XL - I));
        CREDO_VERUM(linea_ad_x_adest(v, X));
        /* capsa: vacua II elementa, facta + II lineae */
        v = mandata_creare(piscina, intern);
        figura_capsam_pingere(v, X, XX, FALSUM);
        CREDO_AEQUALIS_I32(xar_numerus(v->elementa), II);
        v = mandata_creare(piscina, intern);
        figura_capsam_pingere(v, X, XX, VERUM);
        CREDO_AEQUALIS_I32(xar_numerus(v->elementa), IV);
        /* ordo electus: fundus selectionis, fascia accentus */
        v = mandata_creare(piscina, intern);
        figura_ordinem_electum_pingere(v, f);
        y = (constans Mandatum*)xar_obtinere(v->elementa, ZEPHYRUM);
        CREDO_VERUM(y->color.valor == (i32)COLOR_SELECTION);
        y = (constans Mandatum*)xar_obtinere(v->elementa, I);
        CREDO_VERUM(   y->color.valor == (i32)COLOR_ACCENT_PRIMARY
                    && y->fines.latitudo == III);
        /* fascia: fundus et exemplar */
        v = mandata_creare(piscina, intern);
        figura_fasciam_pingere(v, f);
        y = (constans Mandatum*)xar_obtinere(v->elementa, I);
        CREDO_VERUM(y->exemplar == EXEMPLAR_CINEREUM_LEVE);
        /* umbra: II infra-dextrorsum */
        v = mandata_creare(piscina, intern);
        figura_umbram_pingere(v, f);
        y = (constans Mandatum*)xar_obtinere(v->elementa, ZEPHYRUM);
        CREDO_VERUM(y->fines.x == X + II && y->fines.y == XX + II);
        /* campus: superficies, margo */
        v = mandata_creare(piscina, intern);
        figura_campum_pingere(v, f);
        y = (constans Mandatum*)xar_obtinere(v->elementa, ZEPHYRUM);
        CREDO_VERUM(y->color.valor == (i32)COLOR_SUPERFICIES);
        /* textus audax: bis, +1 px */
        v = mandata_creare(piscina, intern);
        figura_textum_audacem(v, X, XX, t, figura_color_thematis(
            COLOR_TEXT));
        CREDO_AEQUALIS_I32(xar_numerus(v->elementa), II);
        y = (constans Mandatum*)xar_obtinere(v->elementa, I);
        CREDO_AEQUALIS_S32(y->fines.x, X + I);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "Registrum", _sectio_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "T2a: vacare et miscere (vicus)", _sectio_secunda, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "figura_mensam_pingere", _sectio_tertia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "figura_color_thematis", _sectio_quarta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "instrumenta visus (V1)", _sectio_quinta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("figura", SECTIONES);
}

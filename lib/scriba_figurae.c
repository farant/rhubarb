/* scriba_figurae.c - figurae scribae */

#include "scriba_figurae.h"
#include "thema.h"
#include "exemplaria.h"
#include "chronica_eventus.h"
#include "chronica_nomina.h"

#include <string.h>


/* ==================================================
 * Auxilia
 * ================================================== */

interior Fines
fines (
    s32 x,
    s32 y,
    s32 w,
    s32 h)
{
    Fines f;

    f.x         = x;
    f.y         = y;
    f.latitudo  = w;
    f.altitudo  = h;
    redde f;
}

/* cellula visibilis: tab, continuatio, '\0', regimen -> spatium */
interior character
visibilis (
    character c)
{
    i8 o;

    o = (i8)c;
    redde (o >= XXXII && o < CXXVII) ? c : ' ';
}

/* S3b: signa iussorum notorum lineae l iterum pinguntur colore
 * accentus (iidem glyphi super textum iam pictum) */
interior vacuum
iussa_colorare (
                        Mandata* m,
     constans TabulaCharacterum* t,
                IussumRegistrum* iussa,
                            s32  l,
                         chorda  linea,
                            s32  cw,
                            s32  ch)
{
    Iussum i;
    chorda signum;
       s32 a;

    a = ZEPHYRUM;
    dum (iussum_proximum(t, l, a, iussum_registrum_notum, iussa,
             m->piscina, &i))
    {
        si (i.initium >= (s32)linea.mensura)
        {
            frange;
        }
        signum.datum    = linea.datum + i.initium;
        signum.mensura  = (i32)((i.finis < (s32)linea.mensura
                                 ? i.finis : (s32)linea.mensura)
                                - i.initium);
        mandata_textus(m, i.initium * cw, l * ch, signum, ZEPHYRUM,
            figura_color_thematis(COLOR_ACCENT_PRIMARY));
        a = i.finis;
    }
}

/* S3d: nexus ('#verbum') lineae l colore accentus secundi */
interior vacuum
nexus_colorare (
                        Mandata* m,
     constans TabulaCharacterum* t,
                            s32  l,
                         chorda  linea,
                            s32  cw,
                            s32  ch)
{
    Iussum i;
    chorda signum;
       s32 a;

    a = ZEPHYRUM;
    dum (iussum_nexus_proximus(t, l, a, m->piscina, &i))
    {
        si (i.initium >= (s32)linea.mensura)
        {
            frange;
        }
        signum.datum    = linea.datum + i.initium;
        signum.mensura  = (i32)((i.finis < (s32)linea.mensura
                                 ? i.finis : (s32)linea.mensura)
                                - i.initium);
        mandata_textus(m, i.initium * cw, l * ch, signum, ZEPHYRUM,
            figura_color_thematis(COLOR_ACCENT_SECONDARY));
        a = i.finis;
    }
}


/* chronica C6: segmentum [(la, ca), (lb, cb)) colore dato, per
 * lineas; lineae selectae [sel_a, sel_b] non */
interior vacuum
segmentum_colorare (
                        Mandata* m,
     constans TabulaCharacterum* t,
                            s32  la,
                            s32  ca,
                            s32  lb,
                            s32  cb,
                            s32  sel_a,
                            s32  sel_b,
                            s32  cw,
                            s32  ch,
                     ColorThema  color)
{
    s32 l;

    per (l = la; l <= lb && l < (s32)t->altitudo; l++)
    {
           s32 a = l == la ? ca : ZEPHYRUM;
           s32 b = l == lb ? cb : (s32)t->latitudo;
           s32 k;
        chorda s;

        si (l >= sel_a && l <= sel_b)
        {
            perge;
        }
        si (b > (s32)t->latitudo)
        {
            b = (s32)t->latitudo;
        }
        si (a >= b)
        {
            perge;
        }
        s.datum = (i8*)piscina_allocare(m->piscina,
            (memoriae_index)(b - a));
        s.mensura = (i32)(b - a);
        per (k = a; k < b; k++)
        {
            s.datum[k - a] = (i8)visibilis(tabula_cellula(t, (i32)l,
                (i32)k));
        }
        mandata_textus(m, a * cw, l * ch, s, ZEPHYRUM,
            figura_color_thematis(color));
    }
}

/* cellula (l, c) ut octetus crudus ('\0', tab, continuatio: spatium) */
interior character
cellula_cruda (
    constans TabulaCharacterum* t,
                           s32  l,
                           s32  c)
{
    character x = tabula_cellula(t, (i32)l, (i32)c);

    redde (x == '\0' || x == '\t' || x == TAB_CONTINUATIO) ? ' ' : x;
}

/* chronica C6: tags <event>, <person>, <place> folii totius - eventus
 * bonus tags accentu, declaratio bona regio accentu, vitium errore */
interior vacuum
chronicam_colorare (
                        Mandata* m,
     constans TabulaCharacterum* t,
                            s32  sel_a,
                            s32  sel_b,
                            s32  cw,
                            s32  ch)
{
    Xar* lineae;
    Xar* eventa;
    Xar* declarationes;
    Xar* vitia;
    i32  i;
    s32  l;

    lineae         = xar_creare(m->piscina, (i32)magnitudo(chorda));
    eventa         = xar_creare(m->piscina,
        (i32)magnitudo(ChronicaEventus));
    declarationes  = xar_creare(m->piscina,
        (i32)magnitudo(ChronicaDeclaratio));
    vitia          = xar_creare(m->piscina,
        (i32)magnitudo(ChronicaVitium));
    si (!lineae || !eventa || !declarationes || !vitia)
    {
        redde;
    }
    per (l = ZEPHYRUM; l < (s32)t->altitudo; l++)
    {
           s32 finis = (s32)t->latitudo;
           s32 k;
        chorda s;

        dum (finis > ZEPHYRUM && cellula_cruda(t, l, finis - I) == ' ')
        {
            finis--;
        }
        s.datum    = (i8*)piscina_allocare(m->piscina,
            (memoriae_index)finis + I);
        s.mensura  = (i32)finis;
        per (k = ZEPHYRUM; k < finis; k++)
        {
            s.datum[k] = (i8)cellula_cruda(t, l, k);
        }
        *(chorda*)xar_addere(lineae) = s;
    }
    (vacuum)chronica_eventa_legere(lineae, m->piscina, eventa, vitia);
    (vacuum)chronica_nomina_legere(lineae, m->piscina, m->intern,
        declarationes, vitia);
    /* eventus bonus: '<event ...>' (ad '>' primum extra quotas) et
     * clausura ('<' ultimum ante finem) */
    per (i = ZEPHYRUM; i < xar_numerus(eventa); i++)
    {
        constans ChronicaEventus* e = (constans ChronicaEventus*)
            xar_obtinere(eventa, i);
        s32 la  = (s32)e->linea;
        s32 ca  = (s32)e->columna;
        s32 lb  = la;
        s32 cb  = ca;
        b32 q   = FALSUM;

        dum (lb < (s32)t->altitudo)
        {
            constans chorda* s = (constans chorda*)xar_obtinere(lineae,
                (i32)lb);

            si (cb >= (s32)s->mensura)
            {
                lb++;
                cb = ZEPHYRUM;
                perge;
            }
            si (s->datum[cb] == '"')
            {
                q = !q;
            }
            alioquin si (s->datum[cb] == '>' && !q)
            {
                frange;
            }
            cb++;
        }
        segmentum_colorare(m, t, la, ca, lb, cb + I, sel_a, sel_b, cw,
            ch, COLOR_ACCENT_SECONDARY);
        la = (s32)e->linea_finis;
        ca = (s32)e->columna_finis;
        dum (ca > ZEPHYRUM)
        {
            constans chorda* s = (constans chorda*)xar_obtinere(lineae,
                (i32)la);

            ca--;
            si (ca < (s32)s->mensura && s->datum[ca] == '<')
            {
                frange;
            }
        }
        segmentum_colorare(m, t, la, ca, (s32)e->linea_finis,
            (s32)e->columna_finis, sel_a, sel_b, cw, ch,
            COLOR_ACCENT_SECONDARY);
    }
    per (i = ZEPHYRUM; i < xar_numerus(declarationes); i++)
    {
        constans ChronicaDeclaratio* d = (constans ChronicaDeclaratio*)
            xar_obtinere(declarationes, i);

        segmentum_colorare(m, t, (s32)d->regio_linea,
            (s32)d->regio_columna, (s32)d->regio_linea_finis,
            (s32)d->regio_columna_finis, sel_a, sel_b, cw, ch,
            COLOR_ACCENT_SECONDARY);
    }
    per (i = ZEPHYRUM; i < xar_numerus(vitia); i++)
    {
        constans ChronicaVitium* v = (constans ChronicaVitium*)
            xar_obtinere(vitia, i);

        segmentum_colorare(m, t, (s32)v->linea, (s32)v->columna,
            (s32)v->linea_finis, (s32)v->columna_finis, sel_a, sel_b,
            cw, ch, COLOR_ERROR);
    }
}


/* ==================================================
 * Figurae
 * ================================================== */

/* <purus/> mensa: fines toti colore COLOR_SUPERFICIES */
vacuum
scriba_figura_mensae (
    constans Componens* c,
               Mandata* m,
                   i32  thema,
                vacuum* ctx)
{
    (vacuum)thema;
    (vacuum)ctx;
    /* indicium foci (Franus): puncta levia super superficiem */
    figura_mensam_pingere(m, fines(ZEPHYRUM, ZEPHYRUM,
        c->fines.latitudo,
        c->fines.altitudo), chorda_aequalis_literis(c->titulus,
        "focatum"));
}

/* <purus/> folium: charta, margo, selectio, cursor, textus */
vacuum
scriba_figura_folii (
    constans Componens* c,
               Mandata* m,
                   i32  thema,
                vacuum* ctx)
{
                 ScribaFigurae* sf;
    constans TabulaCharacterum* t;
                           s32  cw;
                           s32  ch;
                           s32  l;
                           s32  col;
                           s32  finis;
                           s32  sel_a;
                           s32  sel_b;
                    ColorThema  color_cursoris;
                        chorda  linea;
                     character  sub;
                       Punctum  cursor;

    (vacuum)thema;
    sf = (ScribaFigurae*)ctx;
    si (!sf || !sf->sa)
    {
        redde;
    }
    t = &sf->sa->laboris;
    si (t->latitudo == ZEPHYRUM || t->altitudo == ZEPHYRUM)
    {
        redde;
    }
    cw = c->fines.latitudo / (s32)t->latitudo;
    ch = c->fines.altitudo / (s32)t->altitudo;
    /* opus currens (quaestio-rivus III): pagina clausa colore mensae */
    mandata_rectangulum(m, fines(ZEPHYRUM, ZEPHYRUM, c->fines.latitudo,
        c->fines.altitudo), figura_color_thematis(sf->sa->opus
        ? COLOR_SUPERFICIES : COLOR_BACKGROUND), VERUM);
    /* margo in cellulis MARGINIS (cellula tota extra folium):
     * tessellatio rectangulum ad oras cellularum PROXIMAS rotundat et
     * marginem in anulo interiore pingit - margo I pixeli extra
     * chartam in cellulas folii cadebat (Franus: scribendo
     * delebatur) */
    mandata_rectangulum(m, fines(-cw, -ch,
        c->fines.latitudo + II * cw, c->fines.altitudo + II * ch),
        figura_color_thematis(COLOR_BORDER), FALSUM);

    /* selectio (visualis): lineae inter ancoram et cursorem */
    sel_a = -I;
    sel_b = -II;
    si (c->numerus_punctorum >= II)
    {
        sel_a = c->puncta[I].y < c->puncta[ZEPHYRUM].y
              ? c->puncta[I].y : c->puncta[ZEPHYRUM].y;
        sel_b = c->puncta[I].y < c->puncta[ZEPHYRUM].y
              ? c->puncta[ZEPHYRUM].y : c->puncta[I].y;
        mandata_rectangulum(m, fines(ZEPHYRUM, sel_a * ch,
            c->fines.latitudo, (sel_b - sel_a + I) * ch),
            figura_color_thematis(COLOR_SELECTION), VERUM);
    }

    /* cursor: quadratum plenum sub textu, colore STATUS modi sui (ut
     * verbum in linea status) - COLOR_CURSOR idem aurum est ac
     * COLOR_STATUS_INSERT in themate: modi aliter pares essent */
    color_cursoris = chorda_aequalis_literis(c->titulus, "inserere")
                   ? COLOR_STATUS_INSERT
                   : chorda_aequalis_literis(c->titulus, "visualis")
                   ? COLOR_STATUS_VISUAL : COLOR_STATUS_NORMAL;
    cursor.x = -I;
    cursor.y = -I;
    si (c->numerus_punctorum >= I)
    {
        cursor = c->puncta[ZEPHYRUM];
        mandata_rectangulum(m, fines(cursor.x * cw, cursor.y * ch, cw,
            ch),
            figura_color_thematis(color_cursoris),
            VERUM);
    }

    /* textus: linea una per mandatum, spatiis finalibus omissis */
    per (l = ZEPHYRUM; l < (s32)t->altitudo; l++)
    {
        finis = (s32)t->latitudo;
        dum (   finis > ZEPHYRUM
             && visibilis(tabula_cellula(t, (i32)l, (i32)(finis - I)))
                == ' ')
        {
            finis--;
        }
        si (finis == ZEPHYRUM)
        {
            perge;
        }
        linea.datum = (i8*)piscina_allocare(m->piscina,
            (memoriae_index)finis);
        linea.mensura = (i32)finis;
        per (col = ZEPHYRUM; col < finis; col++)
        {
            linea.datum[col] = (i8)visibilis(tabula_cellula(t, (i32)l,
                (i32)col));
        }
        mandata_textus(m, ZEPHYRUM, l * ch, linea, ZEPHYRUM,
            figura_color_thematis((l >= sel_a
                && l <= sel_b) ? COLOR_BACKGROUND
                                                      : COLOR_TEXT));
        /* S3b: iussa nota colore accentus super textum (lineae
         * selectae non: textus ibi colore chartae) */
        si (   sf->sa->iussa && !(l >= sel_a && l <= sel_b)
            && memchr(linea.datum, '$', (size_t)finis))
        {
            iussa_colorare(m, t, sf->sa->iussa, l, linea, cw, ch);
        }
        /* S3d: nexus (solum cum libro paginarum) */
        si (   sf->sa->liber && !(l >= sel_a && l <= sel_b)
            && memchr(linea.datum, '#', (size_t)finis))
        {
            nexus_colorare(m, t, l, linea, cw, ch);
        }
    }

    /* chronica C6: tags eventuum et nominum (solum cum libro) */
    si (sf->sa->liber)
    {
        chronicam_colorare(m, t, sel_a, sel_b, cw, ch);
    }

    /* character sub cursore colore chartae */
    si (   cursor.x >= ZEPHYRUM && cursor.y >= ZEPHYRUM
        && cursor.x < (s32)t->latitudo && cursor.y < (s32)t->altitudo)
    {
        sub = visibilis(tabula_cellula(t, (i32)cursor.y,
            (i32)cursor.x));
        si (sub != ' ')
        {
            linea.datum = (i8*)piscina_allocare(m->piscina, I);
            linea.datum[ZEPHYRUM] = (i8)sub;
            linea.mensura = I;
            mandata_textus(m, cursor.x * cw, cursor.y * ch, linea,
                           ZEPHYRUM,
                           figura_color_thematis(COLOR_BACKGROUND));
        }
    }
}

/* <purus/> status: "MODUS l:c" - modus colore suo, positio textu */
vacuum
scriba_figura_status (
    constans Componens* c,
               Mandata* m,
                   i32  thema,
                vacuum* ctx)
{
    chorda modus;
    chorda reliquum;
       s32 y;
       i32 i;

    (vacuum)thema;
    (vacuum)ctx;
    mandata_rectangulum(m, fines(ZEPHYRUM, ZEPHYRUM, c->fines.latitudo,
        c->fines.altitudo), figura_color_thematis(COLOR_BACKGROUND),
        VERUM);
    y = (c->fines.altitudo > VIII) ? (c->fines.altitudo - VIII) / II
                                   : ZEPHYRUM;
    modus = c->titulus;
    per (i = ZEPHYRUM; i < c->titulus.mensura; i++)
    {
        si (c->titulus.datum[i] == ' ')
        {
            frange;
        }
    }
    modus.mensura     = i;
    reliquum.datum    = c->titulus.datum + i;
    reliquum.mensura  = c->titulus.mensura - i;
    mandata_textus(m, II, y, modus, ZEPHYRUM,
        figura_color_thematis(chorda_aequalis_literis(modus, "INSERERE")
                       ? COLOR_STATUS_INSERT
                       : chorda_aequalis_literis(modus, "VISUALIS")
                       ? COLOR_STATUS_VISUAL : COLOR_STATUS_NORMAL));
    si (reliquum.mensura > ZEPHYRUM)
    {
        mandata_textus(m, II + (s32)modus.mensura * VI, y, reliquum,
                       ZEPHYRUM, figura_color_thematis(COLOR_TEXT));
    }
}

/* <purus/> index paginae: "pagina i/n" textu, centratus in altitudine
 * ut status */
vacuum
scriba_figura_paginae (
    constans Componens* c,
               Mandata* m,
                   i32  thema,
                vacuum* ctx)
{
    s32 y;

    (vacuum)thema;
    (vacuum)ctx;
    y = (c->fines.altitudo > VIII) ? (c->fines.altitudo - VIII) / II
                                   : ZEPHYRUM;
    mandata_textus(m, ZEPHYRUM, y, c->titulus, ZEPHYRUM,
                   figura_color_thematis(COLOR_TEXT));
}

/* <purus/> nuntius: titulus colore erroris, centratus ut status */
vacuum
scriba_figura_nuntii (
    constans Componens* c,
               Mandata* m,
                   i32  thema,
                vacuum* ctx)
{
    s32 y;

    (vacuum)thema;
    (vacuum)ctx;
    y = (c->fines.altitudo > VIII) ? (c->fines.altitudo - VIII) / II
                                   : ZEPHYRUM;
    mandata_textus(m, ZEPHYRUM, y, c->titulus, ZEPHYRUM,
                   figura_color_thematis(COLOR_ERROR));
}

/* comites P3: vectis libri comitis - fascia coloris accentus, titulus
 * colore fundi (distat a libro communi) */
interior vacuum
scriba_figura_vectis (
    constans Componens* c,
               Mandata* m,
                   i32  thema,
                vacuum* ctx)
{
    Fines f;
      s32 y;

    (vacuum)thema;
    (vacuum)ctx;
    f.x         = ZEPHYRUM;
    f.y         = ZEPHYRUM;
    f.latitudo  = c->fines.latitudo;
    f.altitudo  = c->fines.altitudo;
    mandata_rectangulum(m, f,
        figura_color_thematis(COLOR_ACCENT_PRIMARY),
        VERUM);
    y = (c->fines.altitudo > VIII) ? (c->fines.altitudo - VIII) / II
                                   : ZEPHYRUM;
    mandata_textus(m, VI, y, c->titulus, ZEPHYRUM,
                   figura_color_thematis(COLOR_BACKGROUND));
}

vacuum
scriba_figurae_registrare (
    FiguraRegistrum* reg,
                i32  thema,
      ScribaFigurae* ctx)
{
    si (!reg || !ctx)
    {
        redde;
    }
    figura_registrare(reg, PARTES_PROSPECTUS, thema,
        scriba_figura_mensae,
                      ctx);
    figura_registrare(reg, PARTES_CAMPUS, thema, scriba_figura_folii,
        ctx);
    figura_registrare(reg, PARTES_TITULUS, thema, scriba_figura_status,
                      ctx);
    figura_registrare(reg, PARTES_INDEX, thema, scriba_figura_paginae,
                      ctx);
    figura_registrare(reg, PARTES_DIALOGUS, thema, scriba_figura_nuntii,
                      ctx);
    figura_registrare(reg, PARTES_ITEM, thema, scriba_figura_vectis,
        ctx);
}

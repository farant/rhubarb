/* pictor_figurae.c - figurae pictoris */

#include "pictor_figurae.h"
#include "thema.h"
#include "exemplaria.h"

#include <stdio.h>

/* indicium foci: exemplar marginis lateris focati (vicus-latera) */
#define EXEMPLAR_FOCI EXEMPLAR_PUNCTA_DUPLICIA_DISPERSA

interior ColorMandati
color_thematis (
    ColorThema c)
{
    ColorMandati cm;

    cm.genus = COLOR_MANDATI_THEMA;
    cm.valor = (i32)c;
    redde cm;
}

/* <purus/> Prospectus (013): mensa - fines toti colore
 * COLOR_SUPERFICIES; pagina (liber) supra pingitur */
vacuum
figura_prospectus (
    constans Componens* c,
               Mandata* m,
                   i32  thema,
                vacuum* ctx)
{
    Fines f;

    (vacuum)thema;
    (vacuum)ctx;
    f.x         = ZEPHYRUM;
    f.y         = ZEPHYRUM;
    f.latitudo  = c->fines.latitudo;
    f.altitudo  = c->fines.altitudo;
    mandata_rectangulum(m, f, color_thematis(COLOR_SUPERFICIES), VERUM);
    /* indicium foci (Franus): puncta levia super superficiem */
    si (chorda_aequalis_literis(c->titulus, "focatum"))
    {
        mandata_rectangulum_exemplar(m, f, color_thematis(COLOR_BORDER),
            EXEMPLAR_FOCI);
    }
}

/* <purus/> */
vacuum
figura_tabulae (
    constans Componens* c,
               Mandata* m,
                   i32  thema,
                vacuum* ctx)
{
    PictorFigurae* pf;
            Fines  f;
              i32  i;

    (vacuum)thema;
    pf = (PictorFigurae*)ctx;
    si (!pf || !pf->doc)
    {
        redde;
    }
    f.x         = ZEPHYRUM;
    f.y         = ZEPHYRUM;
    f.latitudo  = c->fines.latitudo;
    f.altitudo  = c->fines.altitudo;
    mandata_imago(m, pictor_documentum_sigillum_hex(pf->doc,
        m->piscina), f);
    /* margo paginae (013): in cellulis MARGINIS (cellula tota extra
     * documentum) - tessellatio rectangulum ad oras cellularum
     * proximas rotundat et marginem in anulo interiore pingit: margo I
     * pixeli extra documentum in cellulas documenti cadebat (scriba,
     * Franus 2026-10-05). Sine cellula (0): I pixelum, ut olim. */
    {
        Fines margo;
          s32 mx;
          s32 my;

        mx = pf->cellula_latitudo > ZEPHYRUM ? (s32)pf->cellula_latitudo
                                             : I;
        my = pf->cellula_altitudo > ZEPHYRUM ? (s32)pf->cellula_altitudo
                                             : I;
        margo.x         = -mx;
        margo.y         = -my;
        margo.latitudo  = f.latitudo + II * mx;
        margo.altitudo  = f.altitudo + II * my;
        mandata_rectangulum(m, margo, color_thematis(COLOR_BORDER),
                            FALSUM);
    }
    /* aspergillum (actio): guttae puncti cuiusque ex semine et radio
     * in titulo - eaedem ac in actu (pictor_gutta), sine morae
     * guttis (eae solutione apparent) */
    si (   c->numerus_punctorum > ZEPHYRUM
        && chorda_aequalis_literis(c->actio, "aspergillum.ictus"))
    {
      longus semen;
     integer radius;
         s32 dx;
         s32 dy;
         i32 k;
       Fines g;

        semen   = ZEPHYRUM;
        radius  = ZEPHYRUM;
        (vacuum)sscanf(chorda_ut_cstr(c->titulus, m->piscina), "%ld %d",
            &semen, &radius);
        g.latitudo = I;
        g.altitudo = I;
        per (i = ZEPHYRUM; i < c->numerus_punctorum; i++)
        {
            per (k = ZEPHYRUM; k < PICTOR_GUTTAE_PUNCTO; k++)
            {
                pictor_gutta((s64)semen, i, k, (s32)radius, &dx, &dy);
                g.x = c->puncta[i].x + dx;
                g.y = c->puncta[i].y + dy;
                mandata_rectangulum(m, g,
                    color_thematis(COLOR_ACCENT_PRIMARY), VERUM);
            }
        }
    }
    alioquin
    {
        per (i = I; i < c->numerus_punctorum; i++)
        {
            mandata_linea(m, c->puncta[i - I], c->puncta[i], I,
                          color_thematis(COLOR_ACCENT_PRIMARY));
        }
    }
    si (c->numerus_punctorum > ZEPHYRUM)
    {
        f.x         = c->puncta[c->numerus_punctorum - I].x - I;
        f.y         = c->puncta[c->numerus_punctorum - I].y - I;
        f.latitudo  = III;
        f.altitudo  = III;
        mandata_rectangulum(m, f, color_thematis(COLOR_CURSOR), FALSUM);
    }
}

/* <purus/> */
vacuum
figura_tituli (
    constans Componens* c,
               Mandata* m,
                   i32  thema,
                vacuum* ctx)
{
    Fines f;

    (vacuum)thema;
    (vacuum)ctx;
    f.x         = ZEPHYRUM;
    f.y         = ZEPHYRUM;
    f.latitudo  = c->fines.latitudo;
    f.altitudo  = c->fines.altitudo;
    mandata_rectangulum(m, f, color_thematis(COLOR_BACKGROUND), VERUM);
    /* 013 B3: linea status una cellula (VIII) - textus (fons 6x8)
     * verticaliter centratus, numquam infra fines (olim II fixum in
     * linea XII: in VIII pars inferior praecidebatur) */
    mandata_textus(m, II, (f.altitudo > VIII) ? (f.altitudo - VIII) / II
                                              : ZEPHYRUM,
                   c->titulus, ZEPHYRUM, color_thematis(COLOR_TEXT));
}

constans Imago*
pictor_imago_fons (
    chorda  provenientia,
    vacuum* ctx)
{
    PictorFigurae* pf;

    pf = (PictorFigurae*)ctx;
    si (!pf || !pf->doc)
    {
        redde NIHIL;
    }
    si (chorda_aequalis(provenientia,
            pictor_documentum_sigillum_hex(pf->doc, pf->doc->piscina)))
    {
        redde pictor_documentum_proiectio(pf->doc);
    }
    redde NIHIL;
}

vacuum
pictor_figurae_registrare (
    FiguraRegistrum* reg,
                i32  thema,
      PictorFigurae* ctx)
{
    si (!reg || !ctx)
    {
        redde;
    }
    figura_registrare(reg, PARTES_TABULA, thema, figura_tabulae, ctx);
    figura_registrare(reg, PARTES_TITULUS, thema, figura_tituli, ctx);
    figura_registrare(reg, PARTES_PROSPECTUS, thema, figura_prospectus,
                      ctx);
}

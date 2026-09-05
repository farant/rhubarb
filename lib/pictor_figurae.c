/* pictor_figurae.c - figurae pictoris */

#include "pictor_figurae.h"
#include "thema.h"

interior ColorMandati
color_thematis (
    ColorThema c)
{
    ColorMandati cm;

    cm.genus = COLOR_MANDATI_THEMA;
    cm.valor = (i32)c;
    redde cm;
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
    per (i = I; i < c->numerus_punctorum; i++)
    {
        mandata_linea(m, c->puncta[i - I], c->puncta[i], I,
                      color_thematis(COLOR_ACCENT_PRIMARY));
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
    mandata_textus(m, II, II, c->titulus, ZEPHYRUM,
                   color_thematis(COLOR_TEXT));
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
}

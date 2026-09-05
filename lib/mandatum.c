/* mandatum.c - Mandata delineandi: sex primitiva et index eorum */

#include "mandatum.h"
#include "stml.h"

#include <string.h>


/* ==================================================
 * TABULAE
 * ================================================== */

hic_manens constans character* tituli_generum[MANDATUM_GENUS_NUMERUS] =
    {
    "coetus", "rectangulum", "linea", "polygonum", "imago", "textus"
};

hic_manens constans character* tituli_colorum[III] = {
    "thema", "index", "rgba"
};


/* ==================================================
 * GEOMETRIA
 * ================================================== */

b32
fines_continet (
      Fines f,
    Punctum p)
{
    redde (p.x >= f.x && p.y >= f.y
        && p.x < f.x + f.latitudo && p.y < f.y + f.altitudo);
}


/* ==================================================
 * INDEX
 * ================================================== */

Mandata*
mandata_creare (
                Piscina* piscina,
    InternamentumChorda* intern)
{
    Mandata* m;

    m = (Mandata*)piscina_allocare(piscina, magnitudo(Mandata));
    si (!m)
    { redde NIHIL;
    }
    m->elementa  = xar_creare(piscina, (i32)magnitudo(Mandatum));
    m->piscina   = piscina;
    m->intern    = intern;
    redde m;
}

vacuum
mandata_vacare (
    Mandata* m)
{
    xar_vacare(m->elementa);
}

i32
mandata_numerus (
    constans Mandata* m)
{
    redde xar_numerus(m->elementa);
}

Mandatum*
mandata_obtinere (
    constans Mandata* m,
                 i32  index)
{
    redde (Mandatum*)xar_obtinere(m->elementa, index);
}


/* ==================================================
 * EMISSIO
 * ================================================== */

interior Mandatum*
mandatum_novum (
          Mandata* m,
    MandatumGenus  genus)
{
    Mandatum* md;

    md = (Mandatum*)xar_addere(m->elementa);
    memset(md, ZEPHYRUM, magnitudo(Mandatum));
    md->genus              = genus;
    md->scala              = I;
    md->magnitudo_arboris  = I;
    redde md;
}

interior Punctum*
puncta_copiare (
              Piscina* piscina,
     constans Punctum* puncta,
                  i32  numerus)
{
    Punctum* copia;
        i32  i;

    copia = (Punctum*)piscina_allocare(piscina,
        magnitudo(Punctum) * (memoriae_index)numerus);
    per (i = ZEPHYRUM; i < numerus; i++)
    { copia[i] = puncta[i];
    }
    redde copia;
}

vacuum
mandata_rectangulum (
         Mandata* m,
           Fines  fines,
    ColorMandati  color,
             b32  impletum)
{
    Mandatum* md;

    md            = mandatum_novum(m, MANDATUM_RECTANGULUM);
    md->fines     = fines;
    md->color     = color;
    md->impletum  = impletum;
}

vacuum
mandata_linea (
         Mandata* m,
         Punctum  a,
         Punctum  b,
             i32  crassitudo,
    ColorMandati  color)
{
    Mandatum* md;
     Punctum  ab[II];

    ab[0] = a; ab[1] = b;
    md                     = mandatum_novum(m, MANDATUM_LINEA);
    md->puncta             = puncta_copiare(m->piscina, ab, II);
    md->numerus_punctorum  = II;
    md->crassitudo         = crassitudo;
    md->color              = color;
}

vacuum
mandata_polygonum (
              Mandata* m,
     constans Punctum* puncta,
                  i32  numerus,
         ColorMandati  color,
                  b32  impletum)
{
    Mandatum* md;

    md = mandatum_novum(m, MANDATUM_POLYGONUM);
    md->puncta = puncta_copiare(m->piscina, puncta, numerus);
    md->numerus_punctorum = numerus;
    md->color = color;
    md->impletum = impletum;
}

vacuum
mandata_imago (
     Mandata* m,
      chorda  fons,
       Fines  fines)
{
    Mandatum* md;

    md          = mandatum_novum(m, MANDATUM_IMAGO);
    md->textus  = fons;
    md->fines   = fines;
}

vacuum
mandata_textus (
         Mandata* m,
             i32  x,
             i32  y,
          chorda  textus,
             i32  fons,
    ColorMandati  color)
{
    Mandatum* md;

    md           = mandatum_novum(m, MANDATUM_TEXTUS);
    md->fines.x  = x;
    md->fines.y  = y;
    md->textus   = textus;
    md->fons     = fons;
    md->color    = color;
}

i32
mandata_coetus_incipere (
     Mandata* m,
       Fines  fines,
         b32  sectio,
         i32  translatio_x,
         i32  translatio_y,
         i32  scala,
      chorda  provenientia)
{
    Mandatum* md;
         i32  index;

    index             = xar_numerus(m->elementa);
    md                = mandatum_novum(m, MANDATUM_COETUS);
    md->fines         = fines;
    md->sectio        = sectio;
    md->translatio.x  = translatio_x;
    md->translatio.y  = translatio_y;
    md->scala         = scala < I ? I : scala;
    md->provenientia  = provenientia;
    redde index;
}

vacuum
mandata_coetus_finire (
     Mandata* m,
         i32  index)
{
    Mandatum* md;

    md                     = mandata_obtinere(m, index);
    md->magnitudo_arboris  = xar_numerus(m->elementa) - index;
}


/* ==================================================
 * SERIALIZATIO
 * ================================================== */

interior constans character*
numerus_ut_literae (
     Piscina* piscina,
         i32  n)
{
    redde chorda_ut_cstr(chorda_ex_s32((s32)n, piscina), piscina);
}

interior vacuum
attributum_numericum (
              StmlNodus* nodus,
       constans Mandata* m,
     constans character* titulus,
                    i32  n)
{
    stml_attributum_addere(nodus, m->piscina, m->intern, titulus,
                           numerus_ut_literae(m->piscina, n));
}

interior vacuum
puncta_scribere (
            StmlNodus* nodus,
     constans Mandata* m,
    constans Mandatum* md)
{
    chorda s;
    chorda spatium;
    chorda comma;
       i32 i;

    s        = chorda_ex_literis("", m->piscina);
    spatium  = chorda_ex_literis(" ", m->piscina);
    comma    = chorda_ex_literis(",", m->piscina);
    per (i = ZEPHYRUM; i < md->numerus_punctorum; i++)
    {
        si (i > ZEPHYRUM)
        { s = chorda_concatenare(s, spatium, m->piscina);
        }
        s = chorda_concatenare(s, chorda_ex_s32((s32)md->puncta[i].x,
            m->piscina), m->piscina);
        s = chorda_concatenare(s, comma, m->piscina);
        s = chorda_concatenare(s, chorda_ex_s32((s32)md->puncta[i].y,
            m->piscina), m->piscina);
    }
    stml_textum_addere(nodus, m->piscina, m->intern, chorda_ut_cstr(s,
        m->piscina));
}

/* Scribere mandatum ad nodum; redde numerum mandatorum consumptorum. */
interior i32
mandatum_scribere (
            StmlNodus* parens,
     constans Mandata* m,
                  i32  index)
{
              Mandatum* md;
             StmlNodus* nodus;
    constans character* titulus_attributi;
                   i32  consumpta;
                   i32  i;

    md = mandata_obtinere(m, index);
    nodus = stml_elementum_creare(m->piscina, m->intern,
        tituli_generum[md->genus]);

    si (   md->genus != MANDATUM_TEXTUS && md->genus != MANDATUM_LINEA
        && md->genus != MANDATUM_POLYGONUM)
    {
        attributum_numericum(nodus, m, "x", md->fines.x);
        attributum_numericum(nodus, m, "y", md->fines.y);
        attributum_numericum(nodus, m, "latitudo", md->fines.latitudo);
        attributum_numericum(nodus, m, "altitudo", md->fines.altitudo);
    }
    si (md->genus == MANDATUM_TEXTUS)
    {
        attributum_numericum(nodus, m, "x", md->fines.x);
        attributum_numericum(nodus, m, "y", md->fines.y);
        attributum_numericum(nodus, m, "fons", md->fons);
    }
    si (md->genus != MANDATUM_COETUS && md->genus != MANDATUM_IMAGO)
    {
        stml_attributum_addere(nodus, m->piscina, m->intern,
            "color_genus",
                               tituli_colorum[md->color.genus]);
        attributum_numericum(nodus, m, "color", md->color.valor);
    }
    si (   md->genus == MANDATUM_RECTANGULUM
        || md->genus == MANDATUM_POLYGONUM)
    {
        stml_attributum_addere(nodus, m->piscina, m->intern, "impletum",
                               md->impletum ? "verum" : "falsum");
    }
    si (md->genus == MANDATUM_LINEA)
    {
        attributum_numericum(nodus, m, "crassitudo", md->crassitudo);
    }
    si (md->genus == MANDATUM_LINEA || md->genus == MANDATUM_POLYGONUM)
    {
        puncta_scribere(nodus, m, md);
    }
    si (md->genus == MANDATUM_TEXTUS || md->genus == MANDATUM_IMAGO)
    {
        titulus_attributi = (md->genus
            == MANDATUM_TEXTUS) ? "textus" : "fons";
        stml_attributum_addere(nodus, m->piscina, m->intern,
            titulus_attributi,
                               chorda_ut_cstr(md->textus, m->piscina));
    }
    si (md->genus == MANDATUM_COETUS)
    {
        stml_attributum_addere(nodus, m->piscina, m->intern, "sectio",
                               md->sectio ? "verum" : "falsum");
        attributum_numericum(nodus, m, "translatio_x",
            md->translatio.x);
        attributum_numericum(nodus, m, "translatio_y",
            md->translatio.y);
        attributum_numericum(nodus, m, "scala", md->scala);
        si (md->provenientia.mensura > ZEPHYRUM)
        {
            stml_attributum_addere(nodus, m->piscina, m->intern,
                "provenientia",
                                   chorda_ut_cstr(md->provenientia,
                                   m->piscina));
        }
    }
    stml_liberum_addere(parens, nodus);

    consumpta = I;
    si (md->genus == MANDATUM_COETUS)
    {
        i = index + I;
        dum (i < index + md->magnitudo_arboris)
        {
            i += mandatum_scribere(nodus, m, i);
        }
        consumpta = md->magnitudo_arboris;
    }
    redde consumpta;
}

chorda
mandata_scribere_stml (
    constans Mandata* m,
             Piscina* piscina,
                 b32  pulchrum)
{
    StmlNodus* radix;
          i32  i;
          i32  n;

    radix  = stml_elementum_creare(piscina, m->intern, "mandata");
    n      = mandata_numerus(m);
    i      = ZEPHYRUM;
    dum (i < n)
    { i += mandatum_scribere(radix, m, i);
    }
    redde stml_scribere(radix, piscina, pulchrum);
}

interior i32
attributum_i32 (
             StmlNodus* nodus,
    constans character* titulus)
{
    chorda* c;
       i32  v;

    c = stml_attributum_capere(nodus, titulus);
    v = ZEPHYRUM;
    si (c)
    { chorda_ut_i32(*c, &v);
    }
    redde v;
}

interior b32
attributum_b32 (
             StmlNodus* nodus,
    constans character* titulus)
{
    chorda* c;

    c = stml_attributum_capere(nodus, titulus);
    redde (c && chorda_aequalis_literis(*c, "verum"));
}

interior chorda
attributum_chorda (
             StmlNodus* nodus,
    constans character* titulus,
               Piscina* piscina)
{
    chorda* c;

    c = stml_attributum_capere(nodus, titulus);
    si (c)
    { redde *c;
    }
    redde chorda_ex_literis("", piscina);
}

interior vacuum
puncta_legere (
    StmlNodus* nodus,
     Mandatum* md,
      Piscina* piscina)
{
                   chorda textus;
    chorda_fissio_fructus paria;
    chorda_fissio_fructus xy;
                      i32 i;
                      i32 v;

    textus                 = stml_textus_valor(nodus, piscina);
    paria                  = chorda_fissio(textus, ' ', piscina);
    md->numerus_punctorum  = paria.numerus;
    md->puncta = (Punctum*)piscina_allocare(piscina,
        magnitudo(Punctum) * (memoriae_index)paria.numerus);
    per (i = ZEPHYRUM; i < paria.numerus; i++)
    {
        xy  = chorda_fissio(paria.elementa[i], ',', piscina);
        v   = ZEPHYRUM; si (xy.numerus > ZEPHYRUM)
                        { chorda_ut_i32(xy.elementa[0], &v);
                        }
        md->puncta[i].x  = v;
        v                = ZEPHYRUM; si (xy.numerus > I)
                                     { chorda_ut_i32(xy.elementa[1],
                                           &v);
                                     }
        md->puncta[i].y = v;
    }
}

interior vacuum
mandatum_legere (
    StmlNodus* nodus,
      Mandata* m)
{
     Mandatum* md;
          i32  index;
          i32  g;
    StmlNodus* liberum;
       chorda* cg;

    index  = xar_numerus(m->elementa);
    g      = ZEPHYRUM;
    dum (   g < MANDATUM_GENUS_NUMERUS
         && !chorda_aequalis_literis(*nodus->titulus,
         tituli_generum[g]))
    { g++;
    }
    si (g == MANDATUM_GENUS_NUMERUS)
    { redde;
    }

    md                  = mandatum_novum(m, (MandatumGenus)g);
    md->fines.x         = attributum_i32(nodus, "x");
    md->fines.y         = attributum_i32(nodus, "y");
    md->fines.latitudo  = attributum_i32(nodus, "latitudo");
    md->fines.altitudo  = attributum_i32(nodus, "altitudo");
    cg                  = stml_attributum_capere(nodus, "color_genus");
    md->color.genus     = COLOR_MANDATI_THEMA;
    si (cg && chorda_aequalis_literis(*cg, "index"))
    { md->color.genus = COLOR_MANDATI_INDEX;
    }
    si (cg && chorda_aequalis_literis(*cg, "rgba"))
    { md->color.genus = COLOR_MANDATI_RGBA;
    }
    md->color.valor   = attributum_i32(nodus, "color");
    md->impletum      = attributum_b32(nodus, "impletum");
    md->crassitudo    = attributum_i32(nodus, "crassitudo");
    md->fons          = attributum_i32(nodus, "fons");
    md->sectio        = attributum_b32(nodus, "sectio");
    md->translatio.x  = attributum_i32(nodus, "translatio_x");
    md->translatio.y  = attributum_i32(nodus, "translatio_y");
    md->scala         = attributum_i32(nodus, "scala");
    si (md->scala < I)
    { md->scala = I;
    }
    md->textus = attributum_chorda(nodus, g
        == MANDATUM_IMAGO ? "fons" : "textus", m->piscina);
    md->provenientia = attributum_chorda(nodus, "provenientia",
        m->piscina);

    si (g == MANDATUM_LINEA || g == MANDATUM_POLYGONUM)
    { puncta_legere(nodus, md, m->piscina);
    }

    si (g == MANDATUM_COETUS)
    {
        liberum = stml_primus_liberum(nodus);
        dum (liberum)
        {
            si (liberum->genus == STML_NODUS_ELEMENTUM)
            { mandatum_legere(liberum, m);
            }
            liberum = stml_frater_proximus(liberum);
        }
        /* md pointer potest mutari post additiones: relegere */
        md                     = mandata_obtinere(m, index);
        md->magnitudo_arboris  = xar_numerus(m->elementa) - index;
    }
}

Mandata*
mandata_legere_stml (
     constans character* cstr,
                Piscina* piscina,
    InternamentumChorda* intern)
{
    StmlResultus  res;
         Mandata* m;
       StmlNodus* liberum;

    res = stml_legere_ex_literis(cstr, piscina, intern);
    si (!res.successus || !res.elementum_radix)
    { redde NIHIL;
    }
    m        = mandata_creare(piscina, intern);
    liberum  = stml_primus_liberum(res.elementum_radix);
    dum (liberum)
    {
        si (liberum->genus == STML_NODUS_ELEMENTUM)
        { mandatum_legere(liberum, m);
        }
        liberum = stml_frater_proximus(liberum);
    }
    redde m;
}


/* ==================================================
 * COMPARATIO
 * ================================================== */

b32
mandata_aequalia (
    constans Mandata* a,
    constans Mandata* b)
{
         i32  n;
         i32  i;
         i32  k;
    Mandatum* x;
    Mandatum* y;

    n = mandata_numerus(a);
    si (n != mandata_numerus(b))
    { redde FALSUM;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        x = mandata_obtinere(a, i);
        y = mandata_obtinere(b, i);
        si (x->genus != y->genus)
        { redde FALSUM;
        }
        si (memcmp(&x->fines, &y->fines, magnitudo(Fines)))
        { redde FALSUM;
        }
        si (   x->color.genus != y->color.genus
            || x->color.valor != y->color.valor)
        { redde FALSUM;
        }
        si (   x->impletum   != y->impletum
            || x->crassitudo != y->crassitudo)
        { redde FALSUM;
        }
        si (   x->fons  != y->fons || x->sectio != y->sectio
            || x->scala != y->scala)
        { redde FALSUM;
        }
        si (   x->translatio.x != y->translatio.x
            || x->translatio.y != y->translatio.y)
        { redde FALSUM;
        }
        si (x->magnitudo_arboris != y->magnitudo_arboris)
        { redde FALSUM;
        }
        si (x->numerus_punctorum != y->numerus_punctorum)
        { redde FALSUM;
        }
        per (k = ZEPHYRUM; k < x->numerus_punctorum; k++)
        {
            si (   x->puncta[k].x != y->puncta[k].x
                || x->puncta[k].y != y->puncta[k].y)
            { redde FALSUM;
            }
        }
        si (!chorda_aequalis(x->textus, y->textus))
        { redde FALSUM;
        }
        si (!chorda_aequalis(x->provenientia, y->provenientia))
        { redde FALSUM;
        }
    }
    redde VERUM;
}

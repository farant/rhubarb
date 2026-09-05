/* componens.c - Arbor componentium: nodus logicus */

#include "componens.h"
#include "stml.h"

#include <string.h>


hic_manens constans character*
    tituli_partium[PARTES_NUMERUS] = {
    "nullum", "bottone", "campus", "optio", "index", "item",
    "dialogus", "titulus", "imago", "tabula"
};

hic_manens constans character*
    tituli_praedicatorum[PRAEDICATUM_NUMERUS] =
    {
    "rectangulum", "polygonum", "masca", "proprius"
};

/* Chorda vacua sine allocatione: internamentum chordam vacuam RECUSAT
 * (redde NIHIL, internamentum.c:129) - vacuitas valor est, non
 * entrium canonicum. Datum non-constans ut -Wcast-qual taceat; numquam
 * scribitur. */
hic_manens i8 datum_vacuum[I] = { ZEPHYRUM };

interior chorda
internare_aut_vacua (
    InternamentumChorda* intern,
     constans character* litterae)
{
    chorda  vacua;
    chorda* canonica;

    si (litterae == NIHIL || litterae[0] == '\0')
    {
        vacua.mensura  = ZEPHYRUM;
        vacua.datum    = datum_vacuum;
        redde vacua;
    }
    canonica = chorda_internare_ex_literis(intern, litterae);
    si (canonica == NIHIL)
    {
        vacua.mensura  = ZEPHYRUM;
        vacua.datum    = datum_vacuum;
        redde vacua;
    }
    redde *canonica;
}

constans character*
partes_titulus (
    Partes p)
{
    si ((i32)p >= PARTES_NUMERUS)
    { redde "nullum";
    }
    redde tituli_partium[p];
}

Partes
partes_ex_titulo (
    constans character* titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < PARTES_NUMERUS; i++)
    {
        si (strcmp(titulus, tituli_partium[i]) == ZEPHYRUM)
        { redde (Partes)i;
        }
    }
    redde PARTES_NULLUM;
}


/* ==================================================
 * AEDIFICATIO
 * ================================================== */

Componens*
componens_creare (
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* id,
                 Partes  partes)
{
    Componens* c;

    c = (Componens*)piscina_allocare(piscina, magnitudo(Componens));
    si (!c)
    { redde NIHIL;
    }
    memset(c, ZEPHYRUM, magnitudo(Componens));
    c->intern   = intern;
    c->id       = internare_aut_vacua(intern, id);
    c->actio    = internare_aut_vacua(intern, "");
    c->titulus  = internare_aut_vacua(intern, "");
    c->partes   = partes;
    c->scala    = I;
    c->liberi   = xar_creare(piscina, (i32)magnitudo(Componens*));
    redde c;
}

vacuum
componens_addere_liberum (
    Componens* parens,
    Componens* liberum)
{
    Componens** sedes;

    sedes            = (Componens**)xar_addere(parens->liberi);
    *sedes           = liberum;
    liberum->parens  = parens;
}

vacuum
componens_ponere_fines (
    Componens* c,
        Fines  fines)
{ c->fines = fines;
}

vacuum
componens_ponere_praedicatum (
      Componens* c,
    Praedicatum  p)
{ c->praedicatum = p;
}

vacuum
componens_ponere_focusabilis (
    Componens* c,
          b32  focusabilis)
{ c->focusabilis = focusabilis;
}

vacuum
componens_ponere_sectio (
    Componens* c,
          b32  sectio)
{ c->sectio = sectio;
}

vacuum
componens_ponere_actio (
             Componens* c,
    constans character* actio)
{
    c->actio = internare_aut_vacua(c->intern, actio);
}

vacuum
componens_ponere_titulum (
             Componens* c,
    constans character* titulus)
{
    c->titulus = internare_aut_vacua(c->intern, titulus);
}

vacuum
componens_ponere_transformatio (
     Componens* c,
           s32  translatio_x,
           s32  translatio_y,
           i32  scala)
{
    c->translatio.x  = translatio_x;
    c->translatio.y  = translatio_y;
    c->scala         = scala < I ? I : scala;
}

i32
componens_numerus_liberorum (
    constans Componens* c)
{
    redde xar_numerus(c->liberi);
}

Componens*
componens_liberum (
    constans Componens* c,
                   i32  index)
{
    redde *(Componens**)xar_obtinere(c->liberi, index);
}

Componens*
componens_invenire_per_id (
     Componens* radix,
        chorda  id)
{
          i32  i;
          i32  n;
    Componens* inventus;

    si (chorda_aequalis(radix->id, id))
    { redde radix;
    }
    n = componens_numerus_liberorum(radix);
    per (i = ZEPHYRUM; i < n; i++)
    {
        inventus = componens_invenire_per_id(componens_liberum(radix,
            i), id);
        si (inventus)
        { redde inventus;
        }
    }
    redde NIHIL;
}


/* ==================================================
 * SERIALIZATIO
 * ================================================== */

interior vacuum
attributum_numericum (
              StmlNodus* nodus,
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* titulus,
                    i32  n)
{
    stml_attributum_addere(nodus, piscina, intern, titulus,
                           chorda_ut_cstr(chorda_ex_s32((s32)n,
                           piscina), piscina));
}

interior vacuum
attributum_signatum (
              StmlNodus* nodus,
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* titulus,
                    s32  valor)
{
    stml_attributum_addere(nodus, piscina, intern, titulus,
        chorda_ut_cstr(chorda_ex_s32(valor, piscina), piscina));
}

interior vacuum
attributum_chordae (
              StmlNodus* nodus,
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* titulus,
                 chorda  valor)
{
    stml_attributum_addere(nodus, piscina, intern, titulus,
                           chorda_ut_cstr(valor, piscina));
}

interior StmlNodus*
componens_ad_nodum (
              Componens* c,
                Piscina* piscina,
    InternamentumChorda* intern)
{
    StmlNodus* nodus;
          i32  i;
          i32  n;
       chorda  s;
       chorda  spatium;
       chorda  comma;

    nodus = stml_elementum_creare(piscina, intern, "componens");
    attributum_chordae(nodus, piscina, intern, "id", c->id);
    stml_attributum_addere(nodus, piscina, intern, "partes",
        partes_titulus(c->partes));
    si (c->actio.mensura > ZEPHYRUM)
    { attributum_chordae(nodus, piscina, intern, "actio", c->actio);
    }
    si (c->titulus.mensura > ZEPHYRUM)
    { attributum_chordae(nodus, piscina, intern, "titulus", c->titulus);
    }
    attributum_signatum(nodus, piscina, intern, "x", c->fines.x);
    attributum_signatum(nodus, piscina, intern, "y", c->fines.y);
    attributum_signatum(nodus, piscina, intern, "latitudo",
        c->fines.latitudo);
    attributum_signatum(nodus, piscina, intern, "altitudo",
        c->fines.altitudo);
    stml_attributum_addere(nodus, piscina, intern, "praedicatum",
        tituli_praedicatorum[c->praedicatum]);
    stml_attributum_addere(nodus, piscina, intern, "focusabilis",
        c->focusabilis ? "verum" : "falsum");
    stml_attributum_addere(nodus, piscina, intern, "sectio",
        c->sectio ? "verum" : "falsum");
    attributum_signatum(nodus, piscina, intern, "translatio_x",
        c->translatio.x);
    attributum_signatum(nodus, piscina, intern, "translatio_y",
        c->translatio.y);
    attributum_numericum(nodus, piscina, intern, "scala", c->scala);

    si (c->numerus_punctorum > ZEPHYRUM)
    {
        s        = chorda_ex_literis("", piscina);
        spatium  = chorda_ex_literis(" ", piscina);
        comma    = chorda_ex_literis(",", piscina);
        per (i = ZEPHYRUM; i < c->numerus_punctorum; i++)
        {
            si (i > ZEPHYRUM)
            { s = chorda_concatenare(s, spatium, piscina);
            }
            s = chorda_concatenare(s, chorda_ex_s32((s32)c->puncta[i].x,
                piscina), piscina);
            s = chorda_concatenare(s, comma, piscina);
            s = chorda_concatenare(s, chorda_ex_s32((s32)c->puncta[i].y,
                piscina), piscina);
        }
        attributum_chordae(nodus, piscina, intern, "puncta", s);
    }

    n = componens_numerus_liberorum(c);
    per (i = ZEPHYRUM; i < n; i++)
    {
        stml_liberum_addere(nodus,
            componens_ad_nodum(componens_liberum(c, i), piscina,
            intern));
    }
    redde nodus;
}

chorda
componens_scribere_stml (
              Componens* radix,
                Piscina* piscina,
    InternamentumChorda* intern,
                    b32  pulchrum)
{
    redde stml_scribere(componens_ad_nodum(radix, piscina, intern),
        piscina, pulchrum);
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

interior s32
attributum_s32 (
             StmlNodus* nodus,
    constans character* titulus)
{
    chorda* c;
       s32  v;

    c = stml_attributum_capere(nodus, titulus);
    v = ZEPHYRUM;
    si (c)
    { chorda_ut_s32(*c, &v);
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

interior Componens*
nodus_ad_componens (
              StmlNodus* nodus,
                Piscina* piscina,
    InternamentumChorda* intern)
{
                Componens* c;
                   chorda* a;
                StmlNodus* liberum;
    chorda_fissio_fructus  paria;
    chorda_fissio_fructus  xy;
                      i32  i;
                      s32  v;
                      i32  p;

    a = stml_attributum_capere(nodus, "id");
    c = componens_creare(piscina, intern, a ? chorda_ut_cstr(*a,
        piscina) : "", PARTES_NULLUM);
    a = stml_attributum_capere(nodus, "partes");
    si (a)
    { c->partes = partes_ex_titulo(chorda_ut_cstr(*a, piscina));
    }
    a = stml_attributum_capere(nodus, "actio");
    si (a)
    { componens_ponere_actio(c, chorda_ut_cstr(*a, piscina));
    }
    a = stml_attributum_capere(nodus, "titulus");
    si (a)
    { componens_ponere_titulum(c, chorda_ut_cstr(*a, piscina));
    }
    c->fines.x         = attributum_s32(nodus, "x");
    c->fines.y         = attributum_s32(nodus, "y");
    c->fines.latitudo  = attributum_s32(nodus, "latitudo");
    c->fines.altitudo  = attributum_s32(nodus, "altitudo");
    a                  = stml_attributum_capere(nodus, "praedicatum");
    c->praedicatum     = PRAEDICATUM_RECTANGULUM;
    si (a)
    {
        per (p = ZEPHYRUM; p < PRAEDICATUM_NUMERUS; p++)
        {
            si (chorda_aequalis_literis(*a, tituli_praedicatorum[p]))
            { c->praedicatum = (Praedicatum)p;
            }
        }
    }
    c->focusabilis   = attributum_b32(nodus, "focusabilis");
    c->sectio        = attributum_b32(nodus, "sectio");
    c->translatio.x  = attributum_s32(nodus, "translatio_x");
    c->translatio.y  = attributum_s32(nodus, "translatio_y");
    c->scala         = attributum_i32(nodus, "scala");
    si (c->scala < I)
    { c->scala = I;
    }

    a = stml_attributum_capere(nodus, "puncta");
    si (a && a->mensura > ZEPHYRUM)
    {
        paria                 = chorda_fissio(*a, ' ', piscina);
        c->numerus_punctorum  = paria.numerus;
        c->puncta = (Punctum*)piscina_allocare(piscina,
            magnitudo(Punctum) * (memoriae_index)paria.numerus);
        per (i = ZEPHYRUM; i < paria.numerus; i++)
        {
            xy  = chorda_fissio(paria.elementa[i], ',', piscina);
            v   = ZEPHYRUM; si (xy.numerus > ZEPHYRUM)
                            { chorda_ut_s32(xy.elementa[0], &v);
                            }
            c->puncta[i].x  = v;
            v               = ZEPHYRUM; si (xy.numerus > I)
                                        { chorda_ut_s32(xy.elementa[1],
                                              &v);
                                        }
            c->puncta[i].y = v;
        }
    }

    liberum = stml_primus_liberum(nodus);
    dum (liberum)
    {
        si (liberum->genus == STML_NODUS_ELEMENTUM)
        {
            componens_addere_liberum(c, nodus_ad_componens(liberum,
                piscina, intern));
        }
        liberum = stml_frater_proximus(liberum);
    }
    redde c;
}

Componens*
componens_legere_stml (
     constans character* cstr,
                Piscina* piscina,
    InternamentumChorda* intern)
{
    StmlResultus res;

    res = stml_legere_ex_literis(cstr, piscina, intern);
    si (!res.successus || !res.elementum_radix)
    { redde NIHIL;
    }
    redde nodus_ad_componens(res.elementum_radix, piscina, intern);
}


/* ==================================================
 * COMPARATIO
 * ================================================== */

b32
componens_aequalis (
    constans Componens* a,
    constans Componens* b)
{
    i32 i;
    i32 n;

    si (!chorda_aequalis(a->id, b->id))
    { redde FALSUM;
    }
    si (!chorda_aequalis(a->actio, b->actio))
    { redde FALSUM;
    }
    si (!chorda_aequalis(a->titulus, b->titulus))
    { redde FALSUM;
    }
    si (a->partes != b->partes)
    { redde FALSUM;
    }
    si (memcmp(&a->fines, &b->fines, magnitudo(Fines)))
    { redde FALSUM;
    }
    si (a->praedicatum != b->praedicatum)
    { redde FALSUM;
    }
    si (a->focusabilis != b->focusabilis || a->sectio != b->sectio)
    { redde FALSUM;
    }
    si (   a->translatio.x != b->translatio.x
        || a->translatio.y != b->translatio.y || a->scala != b->scala)
    { redde FALSUM;
    }
    si (a->numerus_punctorum != b->numerus_punctorum)
    { redde FALSUM;
    }
    per (i = ZEPHYRUM; i < a->numerus_punctorum; i++)
    {
        si (   a->puncta[i].x != b->puncta[i].x
            || a->puncta[i].y != b->puncta[i].y)
        { redde FALSUM;
        }
    }
    n = componens_numerus_liberorum(a);
    si (n != componens_numerus_liberorum(b))
    { redde FALSUM;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        si (!componens_aequalis(componens_liberum(a, i),
            componens_liberum(b, i)))
        { redde FALSUM;
        }
    }
    redde VERUM;
}

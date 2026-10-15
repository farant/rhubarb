/* probatio_dithering_exemplaria.c - modus dithering per exemplaria
 *
 * Imagines syntheticae solae (nulla plagula fixa): planae, gradientes,
 * striae. Quae probantur: determinatio, praesupputationes (census numquam
 * excedit), situla pictographica (lateres in rubro plano, album planum
 * manet), colores activi, magnitudines quaevis, aspersio et hachurae
 * per optiones exstinguibiles, errores nominati.
 */
#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "dithering.h"
#include "dithering_exemplaria.h"
#include <math.h>
#include <string.h>


/* ============================================================
 * Imagines syntheticae
 * ============================================================ */

/* imago plana coloris palettae */
interior i8*
_imago_plana (
    Piscina* piscina,
        s32  lat,
        s32  alt,
        s32  index)
{
    i8* rgba = (i8*)piscina_allocare(piscina,
        (memoriae_index)(lat * alt * IV));
    s32 p;

    per (p = 0; p < lat * alt; p++)
    {
        rgba[p * IV + 0] = AQUINAS_PALETTE[index][0];
        rgba[p * IV + 1] = AQUINAS_PALETTE[index][1];
        rgba[p * IV + 2] = AQUINAS_PALETTE[index][2];
        rgba[p * IV + 3] = (i8)CCLV;
    }
    redde rgba;
}

/* scaena mixta: caelum gradiens supra, striae diagonales (sinus) in
 * medio, maculae planae et strepitus infra - omnes classes adsunt */
interior i8*
_imago_scaena (
    Piscina* piscina,
        s32  lat,
        s32  alt)
{
    i8* rgba = (i8*)piscina_allocare(piscina,
        (memoriae_index)(lat * alt * IV));
    s32 x, y;

    per (y = 0; y < alt; y++)
    {
        per (x = 0; x < lat; x++)
        {
            s32 p = y * lat + x;
            f64 r, g, b;

            si (y < alt / III)
            {
                f64 t  = (f64)y / (f64)(alt / III);
                r      = 0.55 + 0.35 * t; g = 0.70 + 0.20 * t; b = 0.95;
            }
            alioquin si (y < 2 * alt / III)
            {
                f64 s  = 0.5 + 0.25 * sin((f64)(x + y) * 0.7);
                r      = s; g = 0.8 * s; b = 0.4 * s;
            }
            alioquin
            {
                i32 h = (i32)(x * 7919 + y * 104729) * 2654435761u;
                f64 n = (f64)((h >> 16) & 0xFF) / 255.0;
                b32 macula = ((x / XVI) + (y / XVI)) % II == 0;
                r = macula ? 0.25 : 0.20 + 0.3 * n;
                g = macula ? 0.45 : 0.35 + 0.3 * n;
                b = macula ? 0.15 : 0.10 + 0.2 * n;
            }
            rgba[p * IV + 0] = (i8)(s32)(r * 255.0);
            rgba[p * IV + 1] = (i8)(s32)(g * 255.0);
            rgba[p * IV + 2] = (i8)(s32)(b * 255.0);
            rgba[p * IV + 3] = (i8)CCLV;
        }
    }
    redde rgba;
}

/* gradiens tenuis eiusdem coloris (regio una, umbra amissa magna) */
interior i8*
_imago_gradiens_rubra (
    Piscina* piscina,
        s32  lat,
        s32  alt)
{
    i8* rgba = (i8*)piscina_allocare(piscina,
        (memoriae_index)(lat * alt * IV));
    s32 x, y;

    per (y = 0; y < alt; y++)
    {
        per (x = 0; x < lat; x++)
        {
            s32 p             = y * lat + x;
            f64 t             = (f64)x / (f64)lat;
            rgba[p * IV + 0]  = (i8)(s32)(110.0 + 40.0 * t);
            rgba[p * IV + 1]  = (i8)(s32)(26.0 + 10.0 * t);
            rgba[p * IV + 2]  = (i8)(s32)(26.0 + 10.0 * t);
            rgba[p * IV + 3]  = (i8)CCLV;
        }
    }
    redde rgba;
}

/* omnes indices in colores activos cadunt */
interior b32
_omnes_activi (
    constans DitheringFructus* f,
                 constans b32* activi)
{
    s32 p;
    per (p = 0; p < (s32)(f->latitudo * f->altitudo); p++)
    {
        s32 i = (s32)f->indices[p];
        si (i < 0 || i >= XVI || !activi[i]) redde FALSUM;
    }
    redde VERUM;
}


/* ============================================================
 * Sectiones
 * ============================================================ */

interior vacuum
_sectio_optiones (
    CredoContextus* c)
{
    DitheringExemplariaOptiones o;

    dithering_exemplaria_optiones_ordinariae(&o);
    CREDO_F64_PROXIMUS(o.praesupputatio_pictographica, 0.30, 1e-12);
    CREDO_F64_PROXIMUS(o.praesupputatio_hachurarum, 0.15, 1e-12);
    CREDO_F64_PROXIMUS(o.pressio_aspersionis, 2.0, 1e-12);
    CREDO_F64_PROXIMUS(o.tectum_aspersionis, 0.50, 1e-12);
    CREDO_AEQUALIS_I32((i32)(o.materia[VI]
        == DITHERING_PICTOGRAMMA_LATERES), I);
    CREDO_AEQUALIS_I32((i32)(o.materia[V]
        == DITHERING_PICTOGRAMMA_NULLUM), I);
    CREDO_AEQUALIS_I32((i32)(o.materia[0]
        == DITHERING_PICTOGRAMMA_NULLUM), I);
    CREDO_AEQUALIS_I32((i32)(o.materia[XIII]
        == DITHERING_PICTOGRAMMA_SQUAMAE), I);
    (vacuum)c;
}

interior vacuum
_sectio_determinatum (
    CredoContextus* c)
{
    b32  activi[XVI];
     i8* rgba = _imago_scaena(c->piscina, XCVI, LXXII);
    DitheringFructus a, b;

    dithering_praeparare_omnes(activi);
    a = dithering_exemplaria_colorum(rgba, XCVI, LXXII, activi, NIHIL,
        NIHIL, c->piscina);
    b = dithering_exemplaria_colorum(rgba, XCVI, LXXII, activi, NIHIL,
        NIHIL, c->piscina);
    CREDO_NECESSE(a.successus && b.successus);
    CREDO_AEQUALIS_I32((i32)a.latitudo, XCVI);
    CREDO_AEQUALIS_I32((i32)a.altitudo, LXXII);
    CREDO_VERUM(memcmp(a.indices, b.indices,
        (memoriae_index)(XCVI * LXXII)) == 0);
    CREDO_VERUM(_omnes_activi(&a, activi));
}

interior vacuum
_sectio_praesupputationes (
    CredoContextus* c)
{
    b32 activi[XVI];
    i8* rgba = _imago_scaena(c->piscina, CXXVIII, XCVI);
    DitheringExemplariaOptiones o;
    DitheringExemplariaCensus k;
    DitheringFructus f;

    dithering_praeparare_omnes(activi);
    dithering_exemplaria_optiones_ordinariae(&o);
    f = dithering_exemplaria_colorum(rgba, CXXVIII, XCVI, activi, &o,
        &k, c->piscina);
    CREDO_NECESSE(f.successus);
    CREDO_VERUM(k.tegmen_pictographicum
        <= o.praesupputatio_pictographica + 1e-12);
    CREDO_VERUM(k.tegmen_hachurarum <= o.praesupputatio_hachurarum
        + 1e-12);
    CREDO_VERUM(k.regiones_pictographicae > 0);   /* scaena maculas planas habet */

    o.praesupputatio_pictographica  = 0.0;
    o.praesupputatio_hachurarum     = 0.0;
    f = dithering_exemplaria_colorum(rgba, CXXVIII, XCVI, activi, &o,
        &k, c->piscina);
    CREDO_NECESSE(f.successus);
    CREDO_F64_PROXIMUS(k.tegmen_pictographicum, 0.0, 1e-12);
    CREDO_F64_PROXIMUS(k.tegmen_hachurarum, 0.0, 1e-12);
    CREDO_AEQUALIS_I32(k.regiones_pictographicae, 0);
    CREDO_AEQUALIS_I32(k.regiones_hachuratae, 0);
}

/* rubrum obscurum planum, praesupputatio plena: regio una implenda
 * lateribus; par colorum colorem regionis (VI) continet */
interior vacuum
_sectio_lateres (
    CredoContextus* c)
{
    hic_manens constans character* LATERES[VIII] = {
        "########", "#.......", "#.......", "#.......",
        "########", "....#...", "....#...", "....#..."
    };
    b32 activi[XVI];
    i8* rgba = _imago_plana(c->piscina, LXIV, LXIV, VI);
    DitheringExemplariaOptiones o;
    DitheringExemplariaCensus k;
    DitheringFructus f;
    s32 x, y, alter = -1, congrua = 0;

    dithering_praeparare_omnes(activi);
    dithering_exemplaria_optiones_ordinariae(&o);
    o.praesupputatio_pictographica = 1.0;
    f = dithering_exemplaria_colorum(rgba, LXIV, LXIV, activi, &o, &k,
        c->piscina);
    CREDO_NECESSE(f.successus);
    CREDO_AEQUALIS_I32(k.regiones_pictographicae, I);

    /* color alter = qui in pixelis laterum (positum positus) praevalet */
    per (y = 0; y < LXIV && alter < 0; y++) per (x = 0; x < LXIV
                                                && alter
                                                    < 0; x++) si (LATERES[y
                                                                  % VIII][x
                                                                  % VIII]
                                                                  == '#'
                                                                  && f.indices[y * LXIV
                                                                                            + x]
                                                                                            != VI) alter =
                                                                                                                              (s32)f.indices[y * LXIV
                                                                                                            + x];
    CREDO_NECESSE(alter >= 0);

    per (y = 0; y < LXIV; y++)
    {
        per (x = 0; x < LXIV; x++)
        {
            b32 positum  = LATERES[y % VIII][x % VIII] == '#';
            s32 i        = (s32)f.indices[y * LXIV + x];
            si (   (positum && i == alter)
                || (!positum && i == VI)) congrua++;
        }
    }
    /* residuum Atkinson pauca mutare potest; exemplar praevalet */
    CREDO_VERUM(congrua >= (LXIV * LXIV * 95) / 100);
}

interior vacuum
_sectio_album (
    CredoContextus* c)
{
    b32 activi[XVI];
    i8* rgba = _imago_plana(c->piscina, XLVIII, XL, V);
    DitheringExemplariaOptiones o;
    DitheringExemplariaCensus k;
    DitheringFructus f;
    s32 p;

    dithering_praeparare_omnes(activi);
    dithering_exemplaria_optiones_ordinariae(&o);
    o.praesupputatio_pictographica = 1.0;
    f = dithering_exemplaria_colorum(rgba, XLVIII, XL, activi, &o, &k,
        c->piscina);
    CREDO_NECESSE(f.successus);
    CREDO_AEQUALIS_I32(k.regiones_pictographicae, 0);
    per (p = 0; p < XLVIII * XL; p++)
    {
        si (f.indices[p] != V)
        {
            CREDO_AEQUALIS_I32((i32)f.indices[p], V);
            frange;
        }
    }
}

interior vacuum
_sectio_activi (
    CredoContextus* c)
{
                 b32  activi[XVI];
                  i8* rgba = _imago_scaena(c->piscina, LXXX, LX);
    DitheringFructus  f;
                 s32  i;

    dithering_praeparare_griseum(activi);
    f = dithering_exemplaria_colorum(rgba, LXXX, LX, activi, NIHIL,
        NIHIL, c->piscina);
    CREDO_NECESSE(f.successus);
    CREDO_VERUM(_omnes_activi(&f, activi));

    dithering_praeparare_monochromaticus(activi);
    f = dithering_exemplaria_colorum(rgba, LXXX, LX, activi, NIHIL,
        NIHIL, c->piscina);
    CREDO_NECESSE(f.successus);
    CREDO_VERUM(_omnes_activi(&f, activi));

    /* color unus: omnia illo colore */
    per (i = 0; i < XVI; i++) activi[i] = i == XII;
    f = dithering_exemplaria_colorum(rgba, LXXX, LX, activi, NIHIL,
        NIHIL, c->piscina);
    CREDO_NECESSE(f.successus);
    CREDO_VERUM(_omnes_activi(&f, activi));
}

interior vacuum
_sectio_magnitudines (
    CredoContextus* c)
{
    hic_manens constans s32 MAGNITUDINES[V][II] = {
        {XXXVII, XXIII}, {I, I}, {I, XL}, {XL, I}, {VII, IX}
    };
    b32 activi[XVI];
    s32 m;

    dithering_praeparare_omnes(activi);
    per (m = 0; m < V; m++)
    {
        s32 lat = MAGNITUDINES[m][0], alt = MAGNITUDINES[m][1];
                      i8* rgba = _imago_scaena(c->piscina, lat, alt);
        DitheringFructus  f = dithering_exemplaria_colorum(rgba,
            (i32)lat, (i32)alt,
                                                          activi, NIHIL,
                                                          NIHIL,
                                                          c->piscina);
        CREDO_NECESSE(f.successus);
        CREDO_VERUM(_omnes_activi(&f, activi));
    }
}

/* gradiens rubrum: regio pictographica cuius umbra amissa aspersionem
 * vocat; pressio 0 = nulla aspersio, imago diversa */
interior vacuum
_sectio_aspersio (
    CredoContextus* c)
{
    b32 activi[XVI];
    i8* rgba = _imago_gradiens_rubra(c->piscina, XCVI, LXIV);
    DitheringExemplariaOptiones o;
    DitheringExemplariaCensus k0, k2;
    DitheringFructus f0, f2;

    dithering_praeparare_omnes(activi);
    dithering_exemplaria_optiones_ordinariae(&o);
    o.praesupputatio_pictographica = 1.0;
    f2 = dithering_exemplaria_colorum(rgba, XCVI, LXIV, activi, &o, &k2,
        c->piscina);
    o.pressio_aspersionis = 0.0;
    f0 = dithering_exemplaria_colorum(rgba, XCVI, LXIV, activi, &o, &k0,
        c->piscina);
    CREDO_NECESSE(f0.successus && f2.successus);
    CREDO_VERUM(k2.regiones_pictographicae > 0);
    CREDO_VERUM(k2.tegmen_aspersionis > 0.0);
    CREDO_F64_PROXIMUS(k0.tegmen_aspersionis, 0.0, 1e-12);
    CREDO_VERUM(memcmp(f0.indices, f2.indices,
        (memoriae_index)(XCVI * LXIV)) != 0);
}

/* striae diagonales sinusoidales in tono medio = campus cohaerens.
 * Fascia striarum tertiam imaginis partem tegit: regio maior quam
 * praesupputatio omittitur (non partim hachuratur), ergo 0.50 hic. */
interior vacuum
_sectio_hachurae (
    CredoContextus* c)
{
    b32 activi[XVI];
    i8* rgba = _imago_scaena(c->piscina, CXXVIII, CXXVIII);
    DitheringExemplariaOptiones o;
    DitheringExemplariaCensus k;
    DitheringFructus f;

    dithering_praeparare_omnes(activi);
    dithering_exemplaria_optiones_ordinariae(&o);
    o.praesupputatio_hachurarum = 0.50;
    f = dithering_exemplaria_colorum(rgba, CXXVIII, CXXVIII, activi, &o,
        &k, c->piscina);
    CREDO_NECESSE(f.successus);
    CREDO_VERUM(k.regiones_hachuratae > 0);
    CREDO_VERUM(k.tegmen_hachurarum > 0.0);
}

interior vacuum
_sectio_errores (
    CredoContextus* c)
{
                 b32  activi[XVI];
                  i8* rgba = _imago_plana(c->piscina, VIII, VIII, III);
    DitheringFructus  f;
                 s32  i;

    dithering_praeparare_omnes(activi);
    f = dithering_exemplaria_colorum(NIHIL, VIII, VIII, activi, NIHIL,
        NIHIL, c->piscina);
    CREDO_FALSUM(f.successus);
    f = dithering_exemplaria_colorum(rgba, 0, VIII, activi, NIHIL,
        NIHIL, c->piscina);
    CREDO_FALSUM(f.successus);
    f = dithering_exemplaria_colorum(rgba, VIII, VIII, activi, NIHIL,
        NIHIL, NIHIL);
    CREDO_FALSUM(f.successus);
    per (i = 0; i < XVI; i++) activi[i] = FALSUM;
    f = dithering_exemplaria_colorum(rgba, VIII, VIII, activi, NIHIL,
        NIHIL, c->piscina);
    CREDO_FALSUM(f.successus);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "optiones ordinariae", _sectio_optiones, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "determinatum", _sectio_determinatum, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "praesupputationes", _sectio_praesupputationes, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "lateres in rubro plano", _sectio_lateres, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "album planum manet", _sectio_album, NIHIL, NIHIL, NIHIL, NIHIL },
    { "colores activi", _sectio_activi, NIHIL, NIHIL, NIHIL, NIHIL },
    { "magnitudines quaevis", _sectio_magnitudines, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "aspersio", _sectio_aspersio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "hachurae", _sectio_hachurae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "errores", _sectio_errores, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("dithering_exemplaria", SECTIONES);
}

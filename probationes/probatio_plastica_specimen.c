/* probatio_plastica_specimen.c - mensurae rei et pagina aspectuum
 * (plastica_specimen.h)
 *
 * metiri: fines, triangula, partes, invisae, vestigium (centrum
 * cellae, summum vincit, res tenuis cellas 0 dat). pagina: mensura,
 * charta, quattuor aspectus scala una, ordines dithering colores
 * palettae solos tenent, ordo partium colorem proprium cuique parti
 * dat, obex invisus non pingitur, determinata.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "imago_typus.h"
#include "stereometria.h"
#include "statua.h"
#include "dithering.h"
#include "plastica.h"
#include "plastica_specimen.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define CELLA PLASTICA_SPECIMEN_CELLA

enumeratio { COLUMNA_FRONS, COLUMNA_LATUS, COLUMNA_DESUPER,
             COLUMNA_QUARTAE };
enumeratio { ORDO_CRUDUS, ORDO_EXEMPLARIA, ORDO_ATKINSON,
             ORDO_PARTES };

/* pixela cellae quae praedicato satisfaciunt: numerus, fines,
 * centrum */
nomen structura {
    i32 pixela;
    s32 sinistrum;
    s32 dextrum;
    s32 summum;
    s32 infimum;
    f64 medium_x;
    f64 medium_y;
} Census;

nomen b32 (*Praedicatum) (constans i8* pixelum);

interior Statua*
_statua (
        CredoContextus* c,
    constans character* literae)
{
    PlasticaFructus f = plastica_legere(
        chorda_ex_literis(literae, c->piscina), c->piscina);

    si (!f.successus)
    {
        printf("descriptio recusata (linea %u): %.*s\n",
            (insignatus)f.linea, (integer)f.error.mensura,
            (constans character*)f.error.datum);
    }
    CREDO_NECESSE (f.successus);
    redde f.statua;
}

interior constans i8*
_pixelum (
    constans Imago* imago,
               i32  columna,
               i32  ordo,
               i32  x,
               i32  y)
{
    redde imago->pixela + ((memoriae_index)(ordo * CELLA + y)
        * imago->latitudo + columna * CELLA + x) * IV;
}

interior b32
_est_charta (
    constans i8* p)
{
    redde p[ZEPHYRUM] == 255 && p[I] == 255 && p[II] == 255;
}

interior b32
_est_res (
    constans i8* p)
{
    redde !_est_charta(p);
}

interior b32
_est_rubrum (
    constans i8* p)
{
    redde p[ZEPHYRUM] > XX && p[ZEPHYRUM] > II * p[I]
        && p[ZEPHYRUM] > II * p[II];
}

interior b32
_est_viride (
    constans i8* p)
{
    redde p[I] > XX && p[I] > II * p[ZEPHYRUM] && p[I] > II * p[II];
}

interior b32
_est_caeruleum (
    constans i8* p)
{
    redde p[II] > XX && p[II] > II * p[ZEPHYRUM] && p[II] > II * p[I];
}

/* aurantium: r > g > b, g non parvum */
interior b32
_est_aurantium (
    constans i8* p)
{
    redde p[ZEPHYRUM] > XX && p[ZEPHYRUM] > p[I] && p[I] > p[II]
        && III * p[I] > p[ZEPHYRUM] && II * p[II] < p[I];
}

interior b32
_est_palettae (
    constans i8* p)
{
    i32 k;

    per (k = ZEPHYRUM; k < XVI; k++)
    {
        si (   p[ZEPHYRUM] == AQUINAS_PALETTE[k][ZEPHYRUM]
            && p[I]        == AQUINAS_PALETTE[k][I]
            && p[II]       == AQUINAS_PALETTE[k][II])
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior Census
_census (
    constans Imago* imago,
               i32  columna,
               i32  ordo,
       Praedicatum  praedicatum)
{
    Census r;
       i32 x;
       i32 y;

    memset(&r, ZEPHYRUM, magnitudo(r));
    r.sinistrum  = (s32)CELLA;
    r.summum     = (s32)CELLA;
    r.dextrum    = -I;
    r.infimum    = -I;
    per (y = ZEPHYRUM; y < CELLA; y++)
    {
        per (x = ZEPHYRUM; x < CELLA; x++)
        {
            si (!praedicatum(_pixelum(imago, columna, ordo, x, y)))
            {
                perge;
            }
            r.pixela++;
            r.medium_x   += (f64)x;
            r.medium_y   += (f64)y;
            r.sinistrum  = (s32)x < r.sinistrum ? (s32)x : r.sinistrum;
            r.dextrum    = (s32)x > r.dextrum ? (s32)x : r.dextrum;
            r.summum     = (s32)y < r.summum ? (s32)y : r.summum;
            r.infimum    = (s32)y;
        }
    }
    si (r.pixela)
    {
        r.medium_x /= (f64)r.pixela;
        r.medium_y /= (f64)r.pixela;
    }
    redde r;
}

interior s32
_latitudo (
    Census r)
{
    redde r.dextrum - r.sinistrum + I;
}

interior s32
_altitudo (
    Census r)
{
    redde r.infimum - r.summum + I;
}

/* ---------- metiri ---------- */

interior vacuum
_sectio_mensurae_capsae (
    CredoContextus* c)
{
    PlasticaMensurae m = plastica_metiri(_statua(c,
        "<res titulus=\"a\">\n"
        "<capsa magnitudo=\"2 1 2\" signum=\"solidum\"/>\n"
        "</res>\n"), c->piscina);

    CREDO_F64_PROXIMUS ((f64)m.minimum.x, -1.0, 1.0e-5);
    CREDO_F64_PROXIMUS ((f64)m.minimum.y, 0.0, 1.0e-5);
    CREDO_F64_PROXIMUS ((f64)m.minimum.z, -1.0, 1.0e-5);
    CREDO_F64_PROXIMUS ((f64)m.maximum.x, 1.0, 1.0e-5);
    CREDO_F64_PROXIMUS ((f64)m.maximum.y, 1.0, 1.0e-5);
    CREDO_F64_PROXIMUS ((f64)m.maximum.z, 1.0, 1.0e-5);
    CREDO_AEQUALIS_I32 (m.triangulorum, XII);
    CREDO_AEQUALIS_I32 (m.partium, I);
    CREDO_AEQUALIS_I32 (m.invisae, ZEPHYRUM);
    /* II unitates = VIII cellae per latus */
    CREDO_AEQUALIS_I32 (m.cellae_solidae, LXIV);
    CREDO_AEQUALIS_I32 (m.cellae_gressibiles, ZEPHYRUM);
}

/* res quae supra humum pendet aut sub eam mergitur: licet, numeri
 * punctum infimum monstrant (plan, review focus 3) */
interior vacuum
_sectio_infimum (
    CredoContextus* c)
{
    PlasticaMensurae m = plastica_metiri(_statua(c,
        "<res titulus=\"a\">\n"
        "<capsa magnitudo=\"1 1 1\" locus=\"0 0.5 0\"/>\n"
        "</res>\n"), c->piscina);

    CREDO_F64_PROXIMUS ((f64)m.minimum.y, 0.5, 1.0e-5);
    m = plastica_metiri(_statua(c,
        "<res titulus=\"a\">\n"
        "<capsa magnitudo=\"1 1 1\" locus=\"3 -0.25 0\"/>\n"
        "</res>\n"), c->piscina);
    CREDO_F64_PROXIMUS ((f64)m.minimum.y, -0.25, 1.0e-5);
    CREDO_F64_PROXIMUS ((f64)m.minimum.x, 2.5, 1.0e-5);
    CREDO_F64_PROXIMUS ((f64)m.maximum.x, 3.5, 1.0e-5);
    /* sine signo: vestigium nullum */
    CREDO_AEQUALIS_I32 (m.cellae_solidae, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (m.cellae_gressibiles, ZEPHYRUM);
}

/* palus tenuis inter centra cellarum: 0 cellae (spec 14a). Idem
 * palus super centrum cellae: I - craticula rei propria */
interior vacuum
_sectio_vestigium_tenue (
    CredoContextus* c)
{
    PlasticaMensurae m = plastica_metiri(_statua(c,
        "<res titulus=\"a\">\n"
        "<capsa magnitudo=\"0.1 1 0.1\" signum=\"solidum\"/>\n"
        "</res>\n"), c->piscina);

    CREDO_AEQUALIS_I32 (m.partium, I);
    CREDO_AEQUALIS_I32 (m.cellae_solidae, ZEPHYRUM);
    m = plastica_metiri(_statua(c,
        "<res titulus=\"a\">\n"
        "<capsa magnitudo=\"0.1 1 0.1\" locus=\"0.125 0 -0.125\""
        " signum=\"solidum\"/>\n"
        "</res>\n"), c->piscina);
    CREDO_AEQUALIS_I32 (m.cellae_solidae, I);
    /* dimidia unitas lata, in craticula: II x II */
    m = plastica_metiri(_statua(c,
        "<res titulus=\"a\">\n"
        "<capsa magnitudo=\"0.5 1 0.5\" signum=\"solidum\"/>\n"
        "</res>\n"), c->piscina);
    CREDO_AEQUALIS_I32 (m.cellae_solidae, IV);
}

/* summum vincit; pars sine signo praeteritur; latera erecta nihil
 * tegunt; cuneus (facies obliqua) tegit */
interior vacuum
_sectio_vestigium_summum (
    CredoContextus* c)
{
    PlasticaMensurae m = plastica_metiri(_statua(c,
        "<res titulus=\"a\">\n"
        "<capsa magnitudo=\"2 1 2\" signum=\"solidum\"/>\n"
        "<capsa magnitudo=\"1 0.2 1\" locus=\"0 1 0\""
        " signum=\"gressibile\"/>\n"
        "</res>\n"), c->piscina);

    CREDO_AEQUALIS_I32 (m.partium, II);
    CREDO_AEQUALIS_I32 (m.triangulorum, XXIV);
    CREDO_AEQUALIS_I32 (m.cellae_gressibiles, XVI);
    CREDO_AEQUALIS_I32 (m.cellae_solidae, XLVIII);
    /* ornamentum super solidum: solidum manet */
    m = plastica_metiri(_statua(c,
        "<res titulus=\"a\">\n"
        "<capsa magnitudo=\"2 1 2\" signum=\"solidum\"/>\n"
        "<capsa magnitudo=\"1 0.2 1\" locus=\"0 1 0\"/>\n"
        "</res>\n"), c->piscina);
    CREDO_AEQUALIS_I32 (m.cellae_solidae, LXIV);
    CREDO_AEQUALIS_I32 (m.cellae_gressibiles, ZEPHYRUM);
    /* gressibile infra solidum: solidum vincit ubi tegit */
    m = plastica_metiri(_statua(c,
        "<res titulus=\"a\">\n"
        "<capsa magnitudo=\"2 0.2 2\" signum=\"gressibile\"/>\n"
        "<capsa magnitudo=\"1 1 1\" signum=\"solidum\"/>\n"
        "</res>\n"), c->piscina);
    CREDO_AEQUALIS_I32 (m.cellae_solidae, XVI);
    CREDO_AEQUALIS_I32 (m.cellae_gressibiles, XLVIII);
    /* cuneus: facies obliqua totum vestigium tegit */
    m = plastica_metiri(_statua(c,
        "<res titulus=\"a\">\n"
        "<cuneus magnitudo=\"1 1 1\" signum=\"gressibile\"/>\n"
        "</res>\n"), c->piscina);
    CREDO_AEQUALIS_I32 (m.cellae_gressibiles, XVI);
}

interior vacuum
_sectio_invisae (
    CredoContextus* c)
{
    PlasticaMensurae m = plastica_metiri(_statua(c,
        "<res titulus=\"a\">\n"
        "<capsa magnitudo=\"0.1 1 0.1\" color=\"#804020\"/>\n"
        "<capsa magnitudo=\"0.5 1 0.5\" signum=\"solidum\""
        " invisibilis=\"verum\"/>\n"
        "</res>\n"), c->piscina);

    CREDO_AEQUALIS_I32 (m.partium, II);
    CREDO_AEQUALIS_I32 (m.invisae, I);
    CREDO_AEQUALIS_I32 (m.cellae_solidae, IV);
    /* fines obicem invisum quoque tenent */
    CREDO_F64_PROXIMUS ((f64)m.maximum.x, 0.25, 1.0e-5);
}

interior vacuum
_sectio_metiri_nihil (
    CredoContextus* c)
{
    PlasticaMensurae m = plastica_metiri(NIHIL, c->piscina);
    PlasticaMensurae z;

    memset(&z, ZEPHYRUM, magnitudo(z));
    CREDO_AEQUALIS_I32 (m.triangulorum, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (m.partium, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (m.invisae, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (m.cellae_solidae, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (m.cellae_gressibiles, ZEPHYRUM);
    CREDO_F64_PROXIMUS ((f64)m.minimum.x, 0.0, 0.0);
    CREDO_F64_PROXIMUS ((f64)m.maximum.y, 0.0, 0.0);
    /* piscina NIHIL: omnia 0 */
    m = plastica_metiri(_statua(c,
        "<res titulus=\"a\">\n"
        "<capsa magnitudo=\"2 1 2\" signum=\"solidum\"/>\n"
        "</res>\n"), NIHIL);
    CREDO_AEQUALIS_I32 (m.triangulorum, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (m.partium, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (m.cellae_solidae, ZEPHYRUM);
}

/* ---------- colores ---------- */

interior vacuum
_sectio_colores (
    CredoContextus* c)
{
    i32 j;
    i32 k;

    (vacuum)c;
    CREDO_AEQUALIS_I32 (PLASTICA_SPECIMEN_COLORES, XII);
    CREDO_LITERAE_AEQUALES (plastica_specimen_color(ZEPHYRUM), "red");
    CREDO_LITERAE_AEQUALES (plastica_specimen_color(I), "orange");
    per (k = ZEPHYRUM; k < PLASTICA_SPECIMEN_COLORES; k++)
    {
        CREDO_NECESSE_NON_NIHIL (plastica_specimen_color(k));
        CREDO_VERUM (strlen(plastica_specimen_color(k)) > II);
        per (j = ZEPHYRUM; j < k; j++)
        {
            CREDO_VERUM (strcmp(plastica_specimen_color(j),
                plastica_specimen_color(k)) != ZEPHYRUM);
        }
        CREDO_LITERAE_AEQUALES (
            plastica_specimen_color(k + PLASTICA_SPECIMEN_COLORES),
            plastica_specimen_color(k));
    }
}

/* ---------- pagina ---------- */

interior vacuum
_sectio_pagina_argumenta (
    CredoContextus* c)
{
    Statua* s = _statua(c,
        "<res titulus=\"a\">\n"
        "<capsa magnitudo=\"2 1 2\" color=\"#ff0000\"/>\n"
        "</res>\n");
    Statua vacua;
     Imago imago;

    CREDO_AEQUALIS_I32 (PLASTICA_SPECIMEN_LATITUDO, 640);
    CREDO_AEQUALIS_I32 (PLASTICA_SPECIMEN_ALTITUDO, 640);
    CREDO_FALSUM (plastica_specimen_pingere(NIHIL, &imago,
        c->piscina));
    CREDO_FALSUM (plastica_specimen_pingere(s, NIHIL, c->piscina));
    CREDO_FALSUM (plastica_specimen_pingere(s, &imago, NIHIL));
    memset(&vacua, ZEPHYRUM, magnitudo(vacua));
    CREDO_VERUM (statua_finire(&vacua, c->piscina));
    CREDO_FALSUM (plastica_specimen_pingere(&vacua, &imago,
        c->piscina));
    memset(&imago, ZEPHYRUM, magnitudo(imago));
    CREDO_VERUM (plastica_specimen_pingere(s, &imago, c->piscina));
    CREDO_NECESSE_NON_NIHIL (imago.pixela);
    CREDO_AEQUALIS_I32 (imago.latitudo, PLASTICA_SPECIMEN_LATITUDO);
    CREDO_AEQUALIS_I32 (imago.altitudo, PLASTICA_SPECIMEN_ALTITUDO);
}

/* capsa rubra: cella cruda non vacua in omni aspectu; margo cellae
 * charta; ordines dithering colores palettae solos tenent et chartam
 * extra rem non tangunt; ordo partium idem tegit ac crudus; omnia
 * pixela opaca; pagina iterata eadem octetim */
interior vacuum
_sectio_pagina_modi (
    CredoContextus* c)
{
    Statua* s = _statua(c,
        "<res titulus=\"a\">\n"
        "<capsa magnitudo=\"2 1 2\" color=\"#ff0000\"/>\n"
        "</res>\n");
     Imago imago;
     Imago altera;
       i32 columna;
       i32 ordo;
       i32 x;
       i32 y;
       i32 mala;

    CREDO_NECESSE (plastica_specimen_pingere(s, &imago, c->piscina));
    CREDO_NECESSE (plastica_specimen_pingere(s, &altera, c->piscina));
    CREDO_OCTETI_AEQUALES (imago.pixela, altera.pixela,
        (memoriae_index)imago.latitudo * imago.altitudo * IV);
    mala = ZEPHYRUM;
    per (y = ZEPHYRUM; y < (i32)imago.altitudo; y++)
    {
        per (x = ZEPHYRUM; x < (i32)imago.latitudo; x++)
        {
            mala += (i32)(imago.pixela[((memoriae_index)y
                * imago.latitudo + x) * IV + III] != 255);
        }
    }
    CREDO_AEQUALIS_I32 (mala, ZEPHYRUM);
    per (columna = ZEPHYRUM; columna < IV; columna++)
    {
        Census crudus = _census(&imago, columna, ORDO_CRUDUS,
            _est_res);
        Census rubra  = _census(&imago, columna, ORDO_CRUDUS,
            _est_rubrum);
        Census partes = _census(&imago, columna, ORDO_PARTES,
            _est_res);

        CREDO_MAIOR_I32 (crudus.pixela, M);
        /* crudum: colores veri (rubrum illuminatum rubrum manet) */
        CREDO_AEQUALIS_I32 (rubra.pixela, crudus.pixela);
        CREDO_AEQUALIS_I32 (partes.pixela, crudus.pixela);
        /* margo: res cellam non tangit */
        CREDO_MAIOR_S32 (crudus.sinistrum, II);
        CREDO_MAIOR_S32 (crudus.summum, II);
        CREDO_MINOR_S32 (crudus.dextrum, (s32)CELLA - III);
        CREDO_MINOR_S32 (crudus.infimum, (s32)CELLA - III);
        per (ordo = ORDO_EXEMPLARIA; ordo <= ORDO_ATKINSON; ordo++)
        {
            i32 extra   = ZEPHYRUM;   /* charta tacta */
            i32 aliena  = ZEPHYRUM;   /* color extra palettam */
            i32 tecta   = ZEPHYRUM;

            per (y = ZEPHYRUM; y < CELLA; y++)
            {
                per (x = ZEPHYRUM; x < CELLA; x++)
                {
                    constans i8* p = _pixelum(&imago, columna, ordo,
                        x, y);
                    b32 res = _est_res(_pixelum(&imago, columna,
                        ORDO_CRUDUS, x, y));

                    si (!res)
                    {
                        extra += (i32)!_est_charta(p);
                        perge;
                    }
                    tecta++;
                    aliena += (i32)!_est_palettae(p);
                }
            }
            CREDO_AEQUALIS_I32 (extra, ZEPHYRUM);
            CREDO_AEQUALIS_I32 (aliena, ZEPHYRUM);
            CREDO_AEQUALIS_I32 (tecta, crudus.pixela);
        }
    }
}

/* scala una: capsa II x I x II. frons et latus: II lata, I alta;
 * desuper: quadratum II; tribus quartis: latior quam frons */
interior vacuum
_sectio_pagina_scala (
    CredoContextus* c)
{
    Statua* s = _statua(c,
        "<res titulus=\"a\">\n"
        "<capsa magnitudo=\"2 1 2\" color=\"#ff0000\"/>\n"
        "</res>\n");
     Imago imago;
    Census frons;
    Census latus;
    Census desuper;
    Census quartae;

    CREDO_NECESSE (plastica_specimen_pingere(s, &imago, c->piscina));
    frons = _census(&imago, COLUMNA_FRONS, ORDO_CRUDUS, _est_res);
    latus = _census(&imago, COLUMNA_LATUS, ORDO_CRUDUS, _est_res);
    desuper  = _census(&imago, COLUMNA_DESUPER, ORDO_CRUDUS,
        _est_res);
    quartae  = _census(&imago, COLUMNA_QUARTAE, ORDO_CRUDUS,
        _est_res);
    /* sphaera radii 1.5 cellam CLX implet: plus quam XL pixela per
     * unitatem */
    CREDO_MAIOR_S32 (_latitudo(frons), LXXX);
    CREDO_MINOR_S32 (_latitudo(frons), CVII);
    CREDO_MAIOR_S32 (_latitudo(frons) + III, II * _altitudo(frons));
    CREDO_MINOR_S32 (_latitudo(frons) - III, II * _altitudo(frons));
    CREDO_MAIOR_S32 (_latitudo(latus) + II, _latitudo(frons));
    CREDO_MINOR_S32 (_latitudo(latus) - II, _latitudo(frons));
    CREDO_MAIOR_S32 (_altitudo(latus) + II, _altitudo(frons));
    CREDO_MINOR_S32 (_altitudo(latus) - II, _altitudo(frons));
    CREDO_MAIOR_S32 (_latitudo(desuper) + II, _latitudo(frons));
    CREDO_MINOR_S32 (_latitudo(desuper) - II, _latitudo(frons));
    CREDO_MAIOR_S32 (_altitudo(desuper) + II, _latitudo(frons));
    CREDO_MINOR_S32 (_altitudo(desuper) - II, _latitudo(frons));
    /* XLV gradus: diagonalis, radix II latior */
    CREDO_MAIOR_S32 (_latitudo(quartae) * X, _latitudo(frons) * XIII);
    CREDO_MINOR_S32 (_latitudo(quartae) * X, _latitudo(frons) * XV);
    /* res in media cella sedet */
    CREDO_MAIOR_S32 (frons.sinistrum + frons.dextrum, (s32)CELLA - V);
    CREDO_MINOR_S32 (frons.sinistrum + frons.dextrum, (s32)CELLA + V);
    CREDO_MAIOR_S32 (frons.summum + frons.infimum, (s32)CELLA - V);
    CREDO_MINOR_S32 (frons.summum + frons.infimum, (s32)CELLA + V);
}

/* quid quisque aspectus videt: tabula rubra ad +z (frons rei),
 * caerulea ad -z (tergum), viridis ad -x */
interior vacuum
_sectio_pagina_aspectus (
    CredoContextus* c)
{
    Statua* s = _statua(c,
        "<res titulus=\"a\">\n"
        "<capsa magnitudo=\"1 1 0.2\" locus=\"0 0 0.9\""
        " color=\"#ff0000\"/>\n"
        "<capsa magnitudo=\"1 1 0.2\" locus=\"0 0 -0.9\""
        " color=\"#0000ff\"/>\n"
        "<capsa magnitudo=\"0.2 1 1\" locus=\"-0.9 0 0\""
        " color=\"#00ff00\"/>\n"
        "</res>\n");
     Imago imago;
    Census rubra;
    Census caerulea;
    Census viridis;

    CREDO_NECESSE (plastica_specimen_pingere(s, &imago, c->piscina));
    /* frons: oculus ad +z stat; rubra caeruleam totam occultat;
     * viridis ad sinistram */
    rubra     = _census(&imago, COLUMNA_FRONS, ORDO_CRUDUS,
        _est_rubrum);
    caerulea  = _census(&imago, COLUMNA_FRONS, ORDO_CRUDUS,
        _est_caeruleum);
    viridis   = _census(&imago, COLUMNA_FRONS, ORDO_CRUDUS,
        _est_viride);
    CREDO_MAIOR_I32 (rubra.pixela, C);
    CREDO_AEQUALIS_I32 (caerulea.pixela, ZEPHYRUM);
    CREDO_MAIOR_I32 (viridis.pixela, X);
    CREDO_VERUM (viridis.medium_x < rubra.medium_x);
    /* latus (gyrus XC): frons rei ad dextram versa - oculus latus
     * -x videt: viridis plena, rubra ad dextram, caerulea ad
     * sinistram */
    rubra     = _census(&imago, COLUMNA_LATUS, ORDO_CRUDUS,
        _est_rubrum);
    caerulea  = _census(&imago, COLUMNA_LATUS, ORDO_CRUDUS,
        _est_caeruleum);
    viridis   = _census(&imago, COLUMNA_LATUS, ORDO_CRUDUS,
        _est_viride);
    CREDO_MAIOR_I32 (viridis.pixela, M);
    CREDO_MAIOR_I32 (rubra.pixela, X);
    CREDO_MAIOR_I32 (caerulea.pixela, X);
    CREDO_VERUM (rubra.medium_x > viridis.medium_x);
    CREDO_VERUM (caerulea.medium_x < viridis.medium_x);
    /* desuper: charta ut tabula - +x ad dextram, frons (+z) infra */
    rubra     = _census(&imago, COLUMNA_DESUPER, ORDO_CRUDUS,
        _est_rubrum);
    caerulea  = _census(&imago, COLUMNA_DESUPER, ORDO_CRUDUS,
        _est_caeruleum);
    viridis   = _census(&imago, COLUMNA_DESUPER, ORDO_CRUDUS,
        _est_viride);
    CREDO_MAIOR_I32 (rubra.pixela, C);
    CREDO_MAIOR_I32 (caerulea.pixela, C);
    CREDO_MAIOR_I32 (viridis.pixela, C);
    CREDO_VERUM (rubra.medium_y > caerulea.medium_y + 20.0);
    CREDO_VERUM (viridis.medium_x < rubra.medium_x - 10.0);
    /* tribus quartis: desuper et a fronte simul - omnes tres */
    rubra     = _census(&imago, COLUMNA_QUARTAE, ORDO_CRUDUS,
        _est_rubrum);
    caerulea  = _census(&imago, COLUMNA_QUARTAE, ORDO_CRUDUS,
        _est_caeruleum);
    viridis   = _census(&imago, COLUMNA_QUARTAE, ORDO_CRUDUS,
        _est_viride);
    CREDO_MAIOR_I32 (rubra.pixela, C);
    CREDO_MAIOR_I32 (caerulea.pixela, X);
    CREDO_MAIOR_I32 (viridis.pixela, C);
    CREDO_VERUM (rubra.medium_y > caerulea.medium_y);
}

/* ordo partium: capsae duae griseae -> rubra et aurantia; obex
 * invisus nec in crudo nec in partibus pingitur, sed numerum partis
 * suum tenet (pars tertia colorem tertium acciperet) */
interior vacuum
_sectio_pagina_partes (
    CredoContextus* c)
{
    Statua* s = _statua(c,
        "<res titulus=\"a\">\n"
        "<capsa magnitudo=\"1 1 1\" locus=\"-0.6 0 0\""
        " color=\"#808080\"/>\n"
        "<capsa magnitudo=\"3 1.5 3\" signum=\"solidum\""
        " invisibilis=\"verum\"/>\n"
        "<capsa magnitudo=\"1 1 1\" locus=\"0.6 0 0\""
        " color=\"#808080\"/>\n"
        "</res>\n");
     Imago imago;
    Census crudus;
    Census partes;
       i32 x;
       i32 y;
       i32 sinistra_mala   = ZEPHYRUM;
       i32 dextra_mala     = ZEPHYRUM;
       i32 cruda_colorata  = ZEPHYRUM;

    CREDO_NECESSE (plastica_specimen_pingere(s, &imago, c->piscina));
    crudus = _census(&imago, COLUMNA_FRONS, ORDO_CRUDUS, _est_res);
    partes = _census(&imago, COLUMNA_FRONS, ORDO_PARTES, _est_res);
    CREDO_MAIOR_I32 (crudus.pixela, D);
    CREDO_AEQUALIS_I32 (partes.pixela, crudus.pixela);
    /* obex III latus non pingitur: res visa II.2 lata, I alta */
    CREDO_MAIOR_S32 (_latitudo(crudus), II * _altitudo(crudus));
    CREDO_MINOR_S32 (_latitudo(crudus) * X, XXIV * _altitudo(crudus));
    per (y = ZEPHYRUM; y < CELLA; y++)
    {
        per (x = ZEPHYRUM; x < CELLA; x++)
        {
            constans i8* p = _pixelum(&imago, COLUMNA_FRONS,
                ORDO_PARTES, x, y);
            constans i8* q = _pixelum(&imago, COLUMNA_FRONS,
                ORDO_CRUDUS, x, y);

            si (_est_charta(p))
            {
                perge;
            }
            cruda_colorata += (i32)(q[ZEPHYRUM] != q[I]
                || q[I] != q[II]);
            si (x < CELLA / II)
            {
                sinistra_mala += (i32)!_est_rubrum(p);
            }
            alioquin
            {
                /* pars 0 rubra, pars I (obex) aurantia sed invisa,
                 * pars II flava */
                dextra_mala += (i32)!(p[ZEPHYRUM] > XX
                    && II * p[I] > p[ZEPHYRUM]
                    && II * p[II] < p[I]);
            }
        }
    }
    CREDO_AEQUALIS_I32 (cruda_colorata, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (sinistra_mala, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (dextra_mala, ZEPHYRUM);
    CREDO_VERUM (!_est_aurantium(_pixelum(&imago, COLUMNA_FRONS,
        ORDO_PARTES, (i32)partes.sinistrum + V,
        (i32)(partes.summum + partes.infimum) / II)));
    CREDO_LITERAE_AEQUALES (plastica_specimen_color(II), "yellow");
}

/* res tota invisa: pagina charta sola, VERUM */
interior vacuum
_sectio_pagina_invisa (
    CredoContextus* c)
{
    Statua* s = _statua(c,
        "<res titulus=\"a\">\n"
        "<capsa magnitudo=\"1 1 1\" signum=\"solidum\""
        " invisibilis=\"verum\"/>\n"
        "</res>\n");
     Imago imago;
       i32 columna;
       i32 ordo;

    CREDO_NECESSE (plastica_specimen_pingere(s, &imago, c->piscina));
    per (ordo = ZEPHYRUM; ordo < IV; ordo++)
    {
        per (columna = ZEPHYRUM; columna < IV; columna++)
        {
            CREDO_AEQUALIS_I32 (_census(&imago, columna, ordo,
                _est_res).pixela, ZEPHYRUM);
        }
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "mensurae_capsae", _sectio_mensurae_capsae, NIHIL, NIHIL,
      NIHIL, NIHIL },
    { "infimum", _sectio_infimum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vestigium_tenue", _sectio_vestigium_tenue, NIHIL, NIHIL,
      NIHIL, NIHIL },
    { "vestigium_summum", _sectio_vestigium_summum, NIHIL, NIHIL,
      NIHIL, NIHIL },
    { "invisae", _sectio_invisae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "metiri_nihil", _sectio_metiri_nihil, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "colores", _sectio_colores, NIHIL, NIHIL, NIHIL, NIHIL },
    { "pagina_argumenta", _sectio_pagina_argumenta, NIHIL, NIHIL,
      NIHIL, NIHIL },
    { "pagina_modi", _sectio_pagina_modi, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "pagina_scala", _sectio_pagina_scala, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "pagina_aspectus", _sectio_pagina_aspectus, NIHIL, NIHIL,
      NIHIL, NIHIL },
    { "pagina_partes", _sectio_pagina_partes, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "pagina_invisa", _sectio_pagina_invisa, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("plastica_specimen", SECTIONES);
}

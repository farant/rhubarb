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
#include "plastica_motus.h"
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

/* ---------- linea motus (plastica-motus-spec IV.6) ---------- */

#define CELLA_MOTUS PLASTICA_MOTUS_CELLA

/* capsa caerulea I x II x I in articulo uno: salit (circulus, 0.5
 * sursum in medio) et mergit (0.25 deorsum, tenetur) */
#define SALTATOR_CORPUS \
    "<figura titulus=\"saltator\">\n" \
    "<articulus titulus=\"corpus\">\n" \
    "<capsa magnitudo=\"1 2 1\" color=\"#2040c0\"/>\n" \
    "</articulus>\n" \
    "<positura titulus=\"alta\">" \
    "<art titulus=\"corpus\" motus=\"0 0.5 0\"/></positura>\n" \
    "<positura titulus=\"ima\">" \
    "<art titulus=\"corpus\" motus=\"0 -0.25 0\"/></positura>\n"

hic_manens constans character* SALTATOR =
    SALTATOR_CORPUS
    "<cyclus titulus=\"salit\" duratio=\"1\" circulus=\"verum\">\n"
    "<clavis tempus=\"0\" positura=\"quies\"/>\n"
    "<clavis tempus=\"0.5\" positura=\"alta\"/>\n"
    "</cyclus>\n"
    "<cyclus titulus=\"mergit\" duratio=\"1\">\n"
    "<clavis tempus=\"0\" positura=\"quies\"/>\n"
    "<clavis tempus=\"0.5\" positura=\"ima\"/>\n"
    "</cyclus>\n"
    "</figura>";

interior PlasticaFiguraFructus
_figura (
        CredoContextus* c,
    constans character* literae)
{
    PlasticaFiguraFructus f = plastica_figura_legere(
        chorda_ex_literis(literae, c->piscina), ZEPHYRUM, c->piscina);

    si (!f.successus)
    {
        printf("figura recusata (linea %u): %.*s\n",
            (insignatus)f.linea, (integer)f.error.mensura,
            (constans character*)f.error.datum);
    }
    CREDO_NECESSE (f.successus);
    redde f;
}

interior constans i8*
_pixelum_motus (
    constans Imago* imago,
               i32  columna,
               i32  ordo,
               i32  x,
               i32  y)
{
    redde imago->pixela
        + ((memoriae_index)(ordo * CELLA_MOTUS + y) * imago->latitudo
            + columna * CELLA_MOTUS + x) * IV;
}

interior Census
_census_motus (
    constans Imago* imago,
               i32  columna,
               i32  ordo,
       Praedicatum  praedicatum)
{
    Census r;
       i32 x;
       i32 y;

    memset(&r, ZEPHYRUM, magnitudo(r));
    r.sinistrum  = (s32)CELLA_MOTUS;
    r.summum     = (s32)CELLA_MOTUS;
    r.dextrum    = -I;
    r.infimum    = -I;
    per (y = ZEPHYRUM; y < CELLA_MOTUS; y++)
    {
        per (x = ZEPHYRUM; x < CELLA_MOTUS; x++)
        {
            si (!praedicatum(_pixelum_motus(imago, columna, ordo, x,
                y)))
            {
                perge;
            }
            r.pixela++;
            r.sinistrum  = (s32)x < r.sinistrum ? (s32)x : r.sinistrum;
            r.dextrum    = (s32)x > r.dextrum ? (s32)x : r.dextrum;
            r.summum     = (s32)y < r.summum ? (s32)y : r.summum;
            r.infimum    = (s32)y;
        }
    }
    redde r;
}

/* capsa caerulea illuminata */
interior b32
_est_capsa_caerulea (
    constans i8* p)
{
    redde p[II] > p[ZEPHYRUM] + XX && p[II] > p[I] + XX;
}

/* linea humi */
interior b32
_est_cinereum (
    constans i8* p)
{
    redde p[ZEPHYRUM] == p[I] && p[I] == p[II] && p[ZEPHYRUM] > L
        && p[ZEPHYRUM] < CCL;
}

/* signum cardinis */
interior b32
_est_signum_rubrum (
    constans i8* p)
{
    redde p[ZEPHYRUM] > CC && p[I] < LX && p[II] < LX;
}

interior b32
_non_album (
    constans i8* p)
{
    redde p[ZEPHYRUM] != 255 || p[I] != 255 || p[II] != 255;
}

#define PROXIMUS_S32(valor, speratus) \
    fac { \
        CREDO_VERUM ((valor) >= (s32)(speratus) - I); \
        CREDO_VERUM ((valor) <= (s32)(speratus) + I); \
    } dum (FALSUM)

interior vacuum
_sectio_motus_numeri (
    CredoContextus* c)
{
    PlasticaFiguraFructus f = _figura(c, SALTATOR);
    PlasticaMensuraeMotus m;
                   Statua vacua;

    /* salit: ab humo ad 0.5 et redit */
    memset(&m, 0x7F, magnitudo(m));
    CREDO_VERUM (plastica_metiri_motum(f.statua, ZEPHYRUM, &m,
        c->piscina));
    CREDO_F64_PROXIMUS ((f64)m.infimum_minimum, 0.0, 1.0e-4);
    CREDO_F64_PROXIMUS ((f64)m.infimum_maximum, 0.5, 1.0e-4);
    CREDO_VERUM (m.hiatus_suturae < 1.0e-3f);
    /* mergit: 0.25 sub humum et ibi manet */
    CREDO_VERUM (plastica_metiri_motum(f.statua, I, &m, c->piscina));
    CREDO_F64_PROXIMUS ((f64)m.infimum_minimum, -0.25, 1.0e-4);
    CREDO_F64_PROXIMUS ((f64)m.infimum_maximum, 0.0, 1.0e-4);
    CREDO_F64_PROXIMUS ((f64)m.hiatus_suturae, 0.25, 1.0e-3);
    /* argumenta mala: FALSUM, mensurae 0 */
    memset(&m, 0x7F, magnitudo(m));
    CREDO_FALSUM (plastica_metiri_motum(f.statua, II, &m, c->piscina));
    CREDO_F64_PROXIMUS ((f64)m.infimum_minimum, 0.0, 1.0e-9);
    CREDO_F64_PROXIMUS ((f64)m.infimum_maximum, 0.0, 1.0e-9);
    CREDO_F64_PROXIMUS ((f64)m.hiatus_suturae, 0.0, 1.0e-9);
    CREDO_FALSUM (plastica_metiri_motum(NIHIL, ZEPHYRUM, &m,
        c->piscina));
    CREDO_FALSUM (plastica_metiri_motum(f.statua, ZEPHYRUM, NIHIL,
        c->piscina));
    CREDO_FALSUM (plastica_metiri_motum(f.statua, ZEPHYRUM, &m,
        NIHIL));
    vacua           = *f.statua;
    vacua.verticum  = ZEPHYRUM;
    CREDO_FALSUM (plastica_metiri_motum(&vacua, ZEPHYRUM, &m,
        c->piscina));

    /* apex inter quadra picturae: exempla XXXII eum capiunt (clavis
     * ad 9/32) */
    f = _figura(c,
        SALTATOR_CORPUS
        "<cyclus titulus=\"citus\" duratio=\"1\" circulus=\"verum\">\n"
        "<clavis tempus=\"0\" positura=\"quies\"/>\n"
        "<clavis tempus=\"0.28125\" positura=\"alta\"/>\n"
        "<clavis tempus=\"0.3125\" positura=\"quies\"/>\n"
        "</cyclus>\n"
        "</figura>");
    CREDO_VERUM (plastica_metiri_motum(f.statua, ZEPHYRUM, &m,
        c->piscina));
    CREDO_F64_PROXIMUS ((f64)m.infimum_maximum, 0.5, 1.0e-4);
    CREDO_F64_PROXIMUS ((f64)m.infimum_minimum, 0.0, 1.0e-4);

    /* semper supra humum, semper infra: minimum et maximum ex
     * exemplo primo incipiunt, non ex 0; hiatus in x solo et in z
     * solo */
    f = _figura(c,
        SALTATOR_CORPUS
        "<positura titulus=\"dextra\">"
        "<art titulus=\"corpus\" motus=\"0.3 0 0\"/></positura>\n"
        "<positura titulus=\"retro\">"
        "<art titulus=\"corpus\" motus=\"0 0 0.4\"/></positura>\n"
        "<cyclus titulus=\"pendet\" duratio=\"1\">\n"
        "<clavis tempus=\"0\" positura=\"alta\"/>\n"
        "</cyclus>\n"
        "<cyclus titulus=\"iacet\" duratio=\"1\">\n"
        "<clavis tempus=\"0\" positura=\"ima\"/>\n"
        "</cyclus>\n"
        "<cyclus titulus=\"labitur\" duratio=\"1\">\n"
        "<clavis tempus=\"0\" positura=\"quies\"/>\n"
        "<clavis tempus=\"0.5\" positura=\"dextra\"/>\n"
        "</cyclus>\n"
        "<cyclus titulus=\"recedit\" duratio=\"1\">\n"
        "<clavis tempus=\"0\" positura=\"quies\"/>\n"
        "<clavis tempus=\"0.5\" positura=\"retro\"/>\n"
        "</cyclus>\n"
        "</figura>");
    CREDO_VERUM (plastica_metiri_motum(f.statua, ZEPHYRUM, &m,
        c->piscina));
    CREDO_F64_PROXIMUS ((f64)m.infimum_minimum, 0.5, 1.0e-4);
    CREDO_F64_PROXIMUS ((f64)m.infimum_maximum, 0.5, 1.0e-4);
    CREDO_VERUM (plastica_metiri_motum(f.statua, I, &m, c->piscina));
    CREDO_F64_PROXIMUS ((f64)m.infimum_minimum, -0.25, 1.0e-4);
    CREDO_F64_PROXIMUS ((f64)m.infimum_maximum, -0.25, 1.0e-4);
    CREDO_VERUM (plastica_metiri_motum(f.statua, II, &m, c->piscina));
    CREDO_F64_PROXIMUS ((f64)m.hiatus_suturae, 0.3, 1.0e-3);
    CREDO_VERUM (plastica_metiri_motum(f.statua, III, &m, c->piscina));
    CREDO_F64_PROXIMUS ((f64)m.hiatus_suturae, 0.4, 1.0e-3);

    /* hiatus = vertex qui MAXIME distat: pars una stat, altera it */
    f = _figura(c,
        "<figura titulus=\"a\">\n"
        "<capsa magnitudo=\"1 1 1\" locus=\"3 0 0\"/>\n"
        "<articulus titulus=\"corpus\">\n"
        "<capsa magnitudo=\"1 1 1\"/>\n"
        "</articulus>\n"
        "<positura titulus=\"mota\">"
        "<art titulus=\"corpus\" motus=\"0 0 0.7\"/></positura>\n"
        "<cyclus titulus=\"it\" duratio=\"1\">\n"
        "<clavis tempus=\"0\" positura=\"quies\"/>\n"
        "<clavis tempus=\"0.5\" positura=\"mota\"/>\n"
        "</cyclus>\n"
        "</figura>");
    CREDO_VERUM (plastica_metiri_motum(f.statua, ZEPHYRUM, &m,
        c->piscina));
    CREDO_F64_PROXIMUS ((f64)m.hiatus_suturae, 0.7, 1.0e-3);

    /* hiatus per VERTICES: articulus in loco vertitur (cardo non
     * movetur), angulus capsae 90 gradus it: chorda I */
    f = _figura(c,
        "<figura titulus=\"a\">\n"
        "<articulus titulus=\"rota\">\n"
        "<capsa magnitudo=\"1 1 1\"/>\n"
        "</articulus>\n"
        "<positura titulus=\"versa\">"
        "<art titulus=\"rota\" rotatio=\"0 90 0\"/></positura>\n"
        "<cyclus titulus=\"vertit\" duratio=\"1\">\n"
        "<clavis tempus=\"0\" positura=\"quies\"/>\n"
        "<clavis tempus=\"0.5\" positura=\"versa\"/>\n"
        "</cyclus>\n"
        "</figura>");
    CREDO_VERUM (plastica_metiri_motum(f.statua, ZEPHYRUM, &m,
        c->piscina));
    CREDO_F64_PROXIMUS ((f64)m.hiatus_suturae, 1.0, 1.0e-3);
    CREDO_F64_PROXIMUS ((f64)m.infimum_minimum, 0.0, 1.0e-4);
    CREDO_F64_PROXIMUS ((f64)m.infimum_maximum, 0.0, 1.0e-4);
}

interior vacuum
_sectio_motus_pictura (
    CredoContextus* c)
{
    PlasticaFiguraFructus f = _figura(c, SALTATOR);
                    Imago imago;
                   Census r;
                      i32 k;
                      i32 x;
                      i32 y;

    CREDO_NECESSE (plastica_specimen_motus_pingere(f.statua,
        f.articuli, f.articulorum, &imago, c->piscina));
    /* VIII cellae latae, duo ordines per cyclum */
    CREDO_AEQUALIS_I32 (imago.latitudo, PLASTICA_MOTUS_LATITUDO);
    CREDO_AEQUALIS_I32 (imago.latitudo, VIII * CXX);
    CREDO_AEQUALIS_I32 (imago.altitudo, IV * CXX);
    CREDO_NECESSE (imago.pixela != NIHIL);

    /* a latere, quies: capsa I x II unitates = XL x LXXX pixela,
     * pes in linea humi (C), in media cella */
    r = _census_motus(&imago, ZEPHYRUM, ZEPHYRUM, _est_capsa_caerulea);
    PROXIMUS_S32 (r.summum, XX);
    PROXIMUS_S32 (r.infimum, XCIX);
    PROXIMUS_S32 (r.sinistrum, XL);
    PROXIMUS_S32 (r.dextrum, LXXIX);
    /* quadrum k ad tempus k / VIII: 0.25 -> X pixela, 0.5 -> XX,
     * 0.75 -> X iterum */
    r = _census_motus(&imago, II, ZEPHYRUM, _est_capsa_caerulea);
    PROXIMUS_S32 (r.summum, X);
    PROXIMUS_S32 (r.infimum, LXXXIX);
    r = _census_motus(&imago, IV, ZEPHYRUM, _est_capsa_caerulea);
    PROXIMUS_S32 (r.summum, ZEPHYRUM);
    PROXIMUS_S32 (r.infimum, LXXIX);
    r = _census_motus(&imago, VI, ZEPHYRUM, _est_capsa_caerulea);
    PROXIMUS_S32 (r.summum, X);
    r = _census_motus(&imago, VII, ZEPHYRUM, _est_capsa_caerulea);
    PROXIMUS_S32 (r.summum, XV);
    /* linea humi cinerea per cellam totam in ordine laterali, ubi
     * figura eam non tegit; nusquam alibi */
    per (k = ZEPHYRUM; k < VIII; k++)
    {
        r = _census_motus(&imago, k, ZEPHYRUM, _est_cinereum);
        CREDO_AEQUALIS_S32 (r.summum, (s32)PLASTICA_MOTUS_HUMUS);
        CREDO_AEQUALIS_S32 (r.infimum, (s32)PLASTICA_MOTUS_HUMUS);
        CREDO_AEQUALIS_S32 (r.sinistrum, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (r.dextrum, (s32)CELLA_MOTUS - I);
        r = _census_motus(&imago, k, I, _est_cinereum);
        CREDO_AEQUALIS_I32 (r.pixela, ZEPHYRUM);
    }
    /* cardo articuli ruber: in quiete ad (LX, C), in apice XX supra;
     * III x III */
    r = _census_motus(&imago, ZEPHYRUM, ZEPHYRUM, _est_signum_rubrum);
    CREDO_AEQUALIS_I32 (r.pixela, IX);
    CREDO_AEQUALIS_S32 (r.sinistrum, LIX);
    CREDO_AEQUALIS_S32 (r.dextrum, LXI);
    CREDO_AEQUALIS_S32 (r.summum, XCIX);
    CREDO_AEQUALIS_S32 (r.infimum, CI);
    r = _census_motus(&imago, IV, ZEPHYRUM, _est_signum_rubrum);
    CREDO_AEQUALIS_I32 (r.pixela, IX);
    CREDO_AEQUALIS_S32 (r.summum, LXXIX);
    CREDO_AEQUALIS_S32 (r.sinistrum, LIX);

    /* tribus quartis: dithering - colores palettae soli, nullum
     * signum; figura adest et in apice altius stat (XX pixela
     * cos 35 = XVI) */
    per (k = ZEPHYRUM; k < VIII; k++)
    {
        r = _census_motus(&imago, k, I, _non_album);
        CREDO_VERUM (r.pixela > D);
        per (y = ZEPHYRUM; y < CELLA_MOTUS; y++)
        {
            per (x = ZEPHYRUM; x < CELLA_MOTUS; x++)
            {
                constans i8* p = _pixelum_motus(&imago, k, I, x, y);

                si (_non_album(p) && !_est_palettae(p))
                {
                    CREDO_VERUM (_est_palettae(p));
                    redde;
                }
            }
        }
    }
    r = _census_motus(&imago, ZEPHYRUM, I, _non_album);
    y = (i32)r.summum;
    x = (i32)r.infimum;
    r = _census_motus(&imago, IV, I, _non_album);
    PROXIMUS_S32 (r.summum, (s32)y - XVI);
    PROXIMUS_S32 (r.infimum, (s32)x - XVI);
    /* punctum humi sub medio figurae ad (LX, C): basis capsae
     * (rhombus) circa id iacet */
    r = _census_motus(&imago, ZEPHYRUM, I, _non_album);
    CREDO_VERUM (r.infimum > (s32)PLASTICA_MOTUS_HUMUS);
    CREDO_VERUM (r.infimum < (s32)PLASTICA_MOTUS_HUMUS + XX);
    PROXIMUS_S32 ((r.sinistrum + r.dextrum) / II, LX);

    /* cyclus alter in ordinibus II et III: mergit X pixela ad
     * quadrum IV et ibi manet; lineam humi tegit */
    r = _census_motus(&imago, ZEPHYRUM, II, _est_capsa_caerulea);
    PROXIMUS_S32 (r.summum, XX);
    r = _census_motus(&imago, IV, II, _est_capsa_caerulea);
    PROXIMUS_S32 (r.summum, XXX);
    PROXIMUS_S32 (r.infimum, CIX);
    r = _census_motus(&imago, VII, II, _est_capsa_caerulea);
    PROXIMUS_S32 (r.summum, XXX);
    CREDO_VERUM (_est_capsa_caerulea(_pixelum_motus(&imago, IV, II, L,
        PLASTICA_MOTUS_HUMUS)));
    CREDO_VERUM (_est_cinereum(_pixelum_motus(&imago, IV, II, V,
        PLASTICA_MOTUS_HUMUS)));
    r = _census_motus(&imago, IV, III, _non_album);
    CREDO_VERUM (r.pixela > D);
    /* opaca tota */
    CREDO_AEQUALIS_I32 (imago.pixela[III], 255);
}

interior vacuum
_sectio_motus_scala (
    CredoContextus* c)
{
    PlasticaFiguraFructus f;
                    Imago imago;
                   Census r;

    /* alta IV unitates: semper LXXX pixela alta, latitudo sequitur */
    f = _figura(c,
        "<figura titulus=\"a\">\n"
        "<capsa magnitudo=\"1 4 1\" color=\"#2040c0\"/>\n"
        "<cyclus titulus=\"stat\" duratio=\"1\">\n"
        "<clavis tempus=\"0\" positura=\"quies\"/>\n"
        "</cyclus>\n"
        "</figura>");
    CREDO_NECESSE (plastica_specimen_motus_pingere(f.statua, NIHIL,
        ZEPHYRUM, &imago, c->piscina));
    CREDO_AEQUALIS_I32 (imago.altitudo, II * CXX);
    r = _census_motus(&imago, III, ZEPHYRUM, _est_capsa_caerulea);
    PROXIMUS_S32 (r.summum, XX);
    PROXIMUS_S32 (r.infimum, XCIX);
    PROXIMUS_S32 (r.sinistrum, L);
    PROXIMUS_S32 (r.dextrum, LXIX);
    /* sine articulis signum nullum */
    r = _census_motus(&imago, ZEPHYRUM, ZEPHYRUM, _est_signum_rubrum);
    CREDO_AEQUALIS_I32 (r.pixela, ZEPHYRUM);

    /* lata VI unitates (in x): III unitates latitudinem regunt,
     * scala dimidia - XL pixela alta; a latere z (I unitas) XX
     * pixela */
    f = _figura(c,
        "<figura titulus=\"a\">\n"
        "<capsa magnitudo=\"6 2 1\" color=\"#2040c0\"/>\n"
        "<cyclus titulus=\"stat\" duratio=\"1\">\n"
        "<clavis tempus=\"0\" positura=\"quies\"/>\n"
        "</cyclus>\n"
        "</figura>");
    CREDO_NECESSE (plastica_specimen_motus_pingere(f.statua, NIHIL,
        ZEPHYRUM, &imago, c->piscina));
    r = _census_motus(&imago, ZEPHYRUM, ZEPHYRUM, _est_capsa_caerulea);
    PROXIMUS_S32 (r.summum, LX);
    PROXIMUS_S32 (r.infimum, XCIX);
    PROXIMUS_S32 (r.sinistrum, L);
    PROXIMUS_S32 (r.dextrum, LXIX);
    /* lata in z quoque regit */
    f = _figura(c,
        "<figura titulus=\"a\">\n"
        "<capsa magnitudo=\"1 2 6\" color=\"#2040c0\"/>\n"
        "<cyclus titulus=\"stat\" duratio=\"1\">\n"
        "<clavis tempus=\"0\" positura=\"quies\"/>\n"
        "</cyclus>\n"
        "</figura>");
    CREDO_NECESSE (plastica_specimen_motus_pingere(f.statua, NIHIL,
        ZEPHYRUM, &imago, c->piscina));
    r = _census_motus(&imago, ZEPHYRUM, ZEPHYRUM, _est_capsa_caerulea);
    PROXIMUS_S32 (r.summum, LX);
    PROXIMUS_S32 (r.sinistrum, ZEPHYRUM);
    PROXIMUS_S32 (r.dextrum, CXIX);

    /* figura extra originem: medium quietis in media cella; humus
     * tamen y = 0 (capsa I supra humum pendet) */
    f = _figura(c,
        "<figura titulus=\"a\">\n"
        "<capsa magnitudo=\"1 2 1\" locus=\"4 1 3\""
        " color=\"#2040c0\"/>\n"
        "<cyclus titulus=\"stat\" duratio=\"1\">\n"
        "<clavis tempus=\"0\" positura=\"quies\"/>\n"
        "</cyclus>\n"
        "</figura>");
    CREDO_NECESSE (plastica_specimen_motus_pingere(f.statua, NIHIL,
        ZEPHYRUM, &imago, c->piscina));
    r = _census_motus(&imago, ZEPHYRUM, ZEPHYRUM, _est_capsa_caerulea);
    PROXIMUS_S32 (r.sinistrum, XL);
    PROXIMUS_S32 (r.dextrum, LXXIX);
    PROXIMUS_S32 (r.summum, ZEPHYRUM);
    PROXIMUS_S32 (r.infimum, LIX);
    r = _census_motus(&imago, ZEPHYRUM, I, _non_album);
    CREDO_VERUM (r.pixela > D);
    PROXIMUS_S32 ((r.sinistrum + r.dextrum) / II, LX);
}

interior vacuum
_sectio_motus_fines (
    CredoContextus* c)
{
    PlasticaFiguraFructus f = _figura(c, SALTATOR);
    PlasticaFiguraFructus g;
                    Imago prima;
                    Imago altera;
                   Statua vacua;
                   Census r;
                      i32 k;
                      i32 n;

    /* argumenta mala */
    CREDO_FALSUM (plastica_specimen_motus_pingere(NIHIL, NIHIL,
        ZEPHYRUM, &prima, c->piscina));
    CREDO_FALSUM (plastica_specimen_motus_pingere(f.statua, NIHIL,
        ZEPHYRUM, NIHIL, c->piscina));
    CREDO_FALSUM (plastica_specimen_motus_pingere(f.statua, NIHIL,
        ZEPHYRUM, &prima, NIHIL));
    /* sine cyclis: FALSUM */
    g = _figura(c,
        "<figura titulus=\"a\"><capsa magnitudo=\"1 1 1\"/></figura>");
    CREDO_FALSUM (plastica_specimen_motus_pingere(g.statua, NIHIL,
        ZEPHYRUM, &prima, c->piscina));

    /* determinata */
    CREDO_NECESSE (plastica_specimen_motus_pingere(f.statua,
        f.articuli, f.articulorum, &prima, c->piscina));
    CREDO_NECESSE (plastica_specimen_motus_pingere(f.statua,
        f.articuli, f.articulorum, &altera, c->piscina));
    CREDO_AEQUALIS_I32 ((i32)memcmp(prima.pixela, altera.pixela,
        (memoriae_index)prima.latitudo * prima.altitudo * IV),
        ZEPHYRUM);

    /* pars quae cellam excedit praeciditur: cyclus alter IV unitates
     * sursum et deorsum volat; ordines cycli primi intacti manent
     * et cellae vicinae nihil accipiunt */
    g = _figura(c,
        SALTATOR_CORPUS
        "<positura titulus=\"summa\">"
        "<art titulus=\"corpus\" motus=\"0 4 0\"/></positura>\n"
        "<positura titulus=\"infima\">"
        "<art titulus=\"corpus\" motus=\"0 -4 0\"/></positura>\n"
        "<positura titulus=\"dextra\">"
        "<art titulus=\"corpus\" motus=\"0 0 -1.5\"/></positura>\n"
        "<cyclus titulus=\"salit\" duratio=\"1\" circulus=\"verum\">\n"
        "<clavis tempus=\"0\" positura=\"quies\"/>\n"
        "<clavis tempus=\"0.5\" positura=\"alta\"/>\n"
        "</cyclus>\n"
        "<cyclus titulus=\"volat\" duratio=\"1\">\n"
        "<clavis tempus=\"0\" positura=\"quies\"/>\n"
        "<clavis tempus=\"0.25\" positura=\"summa\"/>\n"
        "<clavis tempus=\"0.5\" positura=\"infima\"/>\n"
        "<clavis tempus=\"0.75\" positura=\"dextra\"/>\n"
        "</cyclus>\n"
        "<cyclus titulus=\"mergit\" duratio=\"1\">\n"
        "<clavis tempus=\"0\" positura=\"quies\"/>\n"
        "<clavis tempus=\"0.5\" positura=\"ima\"/>\n"
        "</cyclus>\n"
        "<positura titulus=\"laeva\">"
        "<art titulus=\"corpus\" motus=\"0 0 1.5\"/></positura>\n"
        "<cyclus titulus=\"latera\" duratio=\"1\">\n"
        "<clavis tempus=\"0\" positura=\"dextra\"/>\n"
        "<clavis tempus=\"0.5\" positura=\"laeva\"/>\n"
        "</cyclus>\n"
        "</figura>");
    CREDO_NECESSE (plastica_specimen_motus_pingere(g.statua,
        g.articuli, g.articulorum, &altera, c->piscina));
    CREDO_AEQUALIS_I32 (altera.altitudo, VIII * CXX);
    CREDO_AEQUALIS_I32 ((i32)memcmp(prima.pixela, altera.pixela,
        (memoriae_index)prima.latitudo * II * CXX * IV), ZEPHYRUM);
    CREDO_AEQUALIS_I32 ((i32)memcmp(
        prima.pixela + (memoriae_index)prima.latitudo * II * CXX * IV,
        altera.pixela
            + (memoriae_index)altera.latitudo * IV * CXX * IV,
        (memoriae_index)prima.latitudo * II * CXX * IV), ZEPHYRUM);
    /* summa et infima: figura tota extra cellam, signum quoque */
    r = _census_motus(&altera, II, II, _est_capsa_caerulea);
    CREDO_AEQUALIS_I32 (r.pixela, ZEPHYRUM);
    r = _census_motus(&altera, II, II, _est_signum_rubrum);
    CREDO_AEQUALIS_I32 (r.pixela, ZEPHYRUM);
    r = _census_motus(&altera, IV, II, _est_capsa_caerulea);
    CREDO_AEQUALIS_I32 (r.pixela, ZEPHYRUM);
    /* dextra: II unitates = LXXX pixela ad latus - partim extra;
     * cella sequens nihil caerulei accipit praeter figuram suam */
    r = _census_motus(&altera, VI, II, _est_capsa_caerulea);
    CREDO_VERUM (r.pixela > ZEPHYRUM);
    CREDO_VERUM (r.sinistrum == ZEPHYRUM
        || r.dextrum == (s32)CELLA_MOTUS - I);
    r = _census_motus(&altera, VI, II, _est_signum_rubrum);
    CREDO_VERUM (r.pixela < IX);
    CREDO_VERUM (r.pixela > ZEPHYRUM);
    /* signum praecisum in cellam vicinam non fluit: quadrum VII
     * (positura eadem tenta) idem habet, quadrum V (figura infra
     * cellam) nihil, quadrum 0 signum suum integrum */
    k = r.pixela;
    r = _census_motus(&altera, VII, II, _est_signum_rubrum);
    CREDO_AEQUALIS_I32 (r.pixela, k);
    r = _census_motus(&altera, V, II, _est_signum_rubrum);
    CREDO_AEQUALIS_I32 (r.pixela, ZEPHYRUM);
    r = _census_motus(&altera, ZEPHYRUM, II, _est_signum_rubrum);
    CREDO_AEQUALIS_I32 (r.pixela, IX);
    /* cyclus 'latera': signum ad marginem unum (quadrum 0) et ad
     * alterum (quadra IV .. VII, positura tenta): utrumque
     * praecisum, nihil in vicinam fluit - quadra tenta paria,
     * quadrum I (figura intra cellam) signum integrum */
    r = _census_motus(&altera, ZEPHYRUM, VI, _est_signum_rubrum);
    CREDO_VERUM (r.pixela > ZEPHYRUM && r.pixela < IX);
    n = r.pixela;
    r = _census_motus(&altera, IV, VI, _est_signum_rubrum);
    CREDO_VERUM (r.pixela > ZEPHYRUM && r.pixela < IX);
    CREDO_AEQUALIS_I32 (r.pixela + n, IX);
    k = r.pixela;
    r = _census_motus(&altera, V, VI, _est_signum_rubrum);
    CREDO_AEQUALIS_I32 (r.pixela, k);
    r = _census_motus(&altera, VI, VI, _est_signum_rubrum);
    CREDO_AEQUALIS_I32 (r.pixela, k);
    r = _census_motus(&altera, VII, VI, _est_signum_rubrum);
    CREDO_AEQUALIS_I32 (r.pixela, k);
    r = _census_motus(&altera, I, VI, _est_signum_rubrum);
    CREDO_AEQUALIS_I32 (r.pixela, IX);
    /* statua sine verticibus */
    vacua           = *f.statua;
    vacua.verticum  = ZEPHYRUM;
    CREDO_FALSUM (plastica_specimen_motus_pingere(&vacua, NIHIL,
        ZEPHYRUM, &prima, c->piscina));
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
    { "motus_numeri", _sectio_motus_numeri, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "motus_pictura", _sectio_motus_pictura, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "motus_scala", _sectio_motus_scala, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "motus_fines", _sectio_motus_fines, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("plastica_specimen", SECTIONES);
}

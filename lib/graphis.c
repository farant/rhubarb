/*
 * graphis.c - descriptio formarum planarum in picturam (graphis.h)
 *
 * Arbor STML semel ambulatur: forma quaeque probatur et statim in
 * tabulas (fons, larva, domini) pingitur. Recusatio prima manet et
 * omnia abicit - pictura dimidia numquam redditur. Post formas
 * regiones numerantur, deinde fons totus semel per Atkinson transit.
 *
 * Arithmetica tota integra est (s32, et s64 ubi producta crescunt):
 * polygonum per lineas scandit cum sectionibus rationalibus, ovum
 * per inaequalitatem ellipsis, linea per quadrata posita.
 */

#include "graphis.h"
#include "stml.h"
#include "internamentum.h"
#include "xar.h"
#include "exemplaria.h"
#include "dithering.h"
#include <string.h>

#define LAT                 ((s32)GRAPHIS_LATITUDO)
#define ALT                 ((s32)GRAPHIS_ALTITUDO)
#define ERRORIS_MENSURA     CCLVI
#define PUNCTA_MAXIMA       CCLVI
#define COORDINATA_MAXIMA   (IV * MXXIV)
#define CRASSITUDO_MAXIMA   XVI
#define GREGES_MAXIMI       XVI
#define TITULUS_MAXIMUS     LXIV
#define VERBA_MAXIMA        IV
#define VIS_PLENA           CCLVI
#define VIS_ORDINARIA_MILLE 600
#define VIS_MINIMA_MILLE    400

hic_manens constans character* constans EXEMPLARIA[EXEMPLAR_NUMERUS] =
{
    "solidus", "puncta_dispersa", "puncta_regularia", "tessellatum",
    "lineolae_horizontales", "lineae_verticales", "diagonalis",
    "verticalis_dispersus", "dispersus", "puncta_diagonalia",
    "latericium", "latericium_offset", "cinereum_leve",
    "diagonalis_dispersus", "rhombus_dispersus", "textum", "mixtus",
    "angulus", "latericium_complexus", "albus", "rarissimus",
    "puncta_duplicia_dispersa", "puncta_duplicia",
    "horizontalis_dispersus", "lineae_horizontales",
    "diagonalis_anterior", "horizontalis_latus",
    "diagonalis_posterior", "dispersus_mixtus", "margo_sinister",
    "arcus", "linea_rhombus", "anguli_parvi", "diagonalis_undulatus",
    "crux", "mixtus_gravis", "capsae_complexae", "rhombus_lineatus"
};

/* quomodo forma pingitur */
nomen structura {
    b32 impletur;         /* colores dati */
    b32 duo;              /* colores duo (non tegumentum) */
    b32 tegumentum;       /* secundus '-' */
     i8 primus[III];
     i8 secundus[III];
     i8 planus[III];      /* mixtura secundum densitatem */
    i32 exemplar;
    i32 vis;              /* 0 .. VIS_PLENA */
    b32 margo;
    s32 crassitudo;
     i8 color_marginis[III];
    i32 dominus;          /* 0 = sine titulo */
    b32 lucens;           /* lucem dat nec accipit */
} Proprietates;

nomen structura {
      Piscina* piscina;
           i8* fons;
           i8* larva;
           i8* domini;
           i8* lucentes;
   GraphisLux  lux;
 GraphisLumen  lumina[GRAPHIS_LUMINA_MAXIMA];
       chorda  tituli[GRAPHIS_TITULI_MAXIMI];
          i32  lineae[GRAPHIS_TITULI_MAXIMI];
          i32  titulorum;
          i32  picta;      /* pixela formae currentis intra picturam */
          b32  fractum;
          i32  linea;
    character  error[ERRORIS_MENSURA];
} StatusPicturae;


/* ==================================================
 * Recusatio
 * ================================================== */

interior vacuum
_appone (
               StatusPicturae* p,
                          i32* longitudo,
           constans character* fons,
                          i32  mensura)
{
    i32 k;

    per (k = ZEPHYRUM;
         k < mensura && *longitudo + I < ERRORIS_MENSURA; k++)
    {
        character littera = fons[k];

        /* nuntius lineam novam numquam fert */
        si (littera == '\n' || littera == '\r' || littera == '\t')
        {
            littera = ' ';
        }
        p->error[(*longitudo)++] = littera;
    }
    p->error[*longitudo] = '\0';
}

interior vacuum
_appone_literas (
               StatusPicturae* p,
                          i32* longitudo,
           constans character* fons)
{
    si (fons)
    {
        _appone(p, longitudo, fons, (i32)strlen(fons));
    }
}

interior vacuum
_appone_chordam (
            StatusPicturae* p,
                       i32* longitudo,
           constans chorda* fons)
{
    si (fons && fons->datum)
    {
        _appone(p, longitudo, (constans character*)fons->datum,
            fons->mensura > LXIV ? LXIV : fons->mensura);
    }
}

/* nuntius = ante + medium + post; prima recusatio manet */
interior b32
_recusare (
               StatusPicturae* p,
                          i32  linea,
           constans character* ante,
              constans chorda* medium,
           constans character* post)
{
    i32 longitudo = ZEPHYRUM;

    si (p->fractum)
    {
        redde FALSUM;
    }
    p->fractum  = VERUM;
    p->linea    = linea;
    _appone_literas(p, &longitudo, ante);
    _appone_chordam(p, &longitudo, medium);
    _appone_literas(p, &longitudo, post);
    redde FALSUM;
}

/* "<elementum> attribute 'titulus' quid" */
interior b32
_recusare_attributum (
               StatusPicturae* p,
           constans StmlNodus* nodus,
           constans character* titulus,
           constans character* quid)
{
    i32 longitudo = ZEPHYRUM;

    si (p->fractum)
    {
        redde FALSUM;
    }
    p->fractum  = VERUM;
    p->linea    = nodus->linea;
    _appone_literas(p, &longitudo, "<");
    _appone_chordam(p, &longitudo, nodus->titulus);
    _appone_literas(p, &longitudo, "> attribute '");
    _appone_literas(p, &longitudo, titulus);
    _appone_literas(p, &longitudo, "' ");
    _appone_literas(p, &longitudo, quid);
    redde FALSUM;
}

/* "<elementum> quid" */
interior b32
_recusare_elementum (
               StatusPicturae* p,
           constans StmlNodus* nodus,
           constans character* quid)
{
    i32 longitudo = ZEPHYRUM;

    si (p->fractum)
    {
        redde FALSUM;
    }
    p->fractum  = VERUM;
    p->linea    = nodus->linea;
    _appone_literas(p, &longitudo, "<");
    _appone_chordam(p, &longitudo, nodus->titulus);
    _appone_literas(p, &longitudo, "> ");
    _appone_literas(p, &longitudo, quid);
    redde FALSUM;
}


/* ==================================================
 * Valores attributorum
 * ================================================== */

interior b32
_spatium (
    i8 littera)
{
    redde littera == ' ' || littera == '\t' || littera == '\n'
        || littera == '\r';
}

/* verba spatiis separata; FALSUM si plura quam 'maximum' */
interior b32
_verba (
    constans chorda* valor,
             chorda* exitus,
                i32  maximum,
                i32* numerus)
{
    i32 k = ZEPHYRUM;

    *numerus = ZEPHYRUM;
    dum (VERUM)
    {
        i32 initium;

        dum (k < valor->mensura && _spatium(valor->datum[k]))
        {
            k++;
        }
        si (k >= valor->mensura)
        {
            frange;
        }
        initium = k;
        dum (k < valor->mensura && !_spatium(valor->datum[k]))
        {
            k++;
        }
        si (*numerus >= maximum)
        {
            redde FALSUM;
        }
        exitus[(*numerus)++] = chorda_sectio(*valor, initium, k);
    }
    redde VERUM;
}

/* integer intra fines coordinatarum */
interior b32
_coordinata (
    chorda  verbum,
       s32* exitus)
{
    s32 x;

    si (   !chorda_ut_s32(verbum, &x) || x > (s32)COORDINATA_MAXIMA
        || x < -(s32)COORDINATA_MAXIMA)
    {
        redde FALSUM;
    }
    *exitus = x;
    redde VERUM;
}

/* 'numerus' coordinatae, nec plus nec minus */
interior b32
_coordinatae (
    constans chorda* valor,
                s32* exitus,
                i32  numerus)
{
    chorda verba[VERBA_MAXIMA];
       i32 n;
       i32 k;

    si (   numerus > VERBA_MAXIMA
        || !_verba(valor, verba, numerus, &n) || n != numerus)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        si (!_coordinata(verba[k], &exitus[k]))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* paria "x,y x,y ..."; *numerus paria lecta */
interior b32
_paria (
    constans chorda* valor,
                s32* x,
                s32* y,
                i32* numerus)
{
    i32 k = ZEPHYRUM;

    *numerus = ZEPHYRUM;
    dum (VERUM)
    {
        i32 initium;
        i32 comma;

        dum (k < valor->mensura && _spatium(valor->datum[k]))
        {
            k++;
        }
        si (k >= valor->mensura)
        {
            frange;
        }
        initium  = k;
        comma    = valor->mensura;
        dum (k < valor->mensura && !_spatium(valor->datum[k]))
        {
            si (valor->datum[k] == ',' && comma == valor->mensura)
            {
                comma = k;
            }
            k++;
        }
        si (   *numerus >= PUNCTA_MAXIMA || comma == valor->mensura
            || !_coordinata(chorda_sectio(*valor, initium, comma),
                &x[*numerus])
            || !_coordinata(chorda_sectio(*valor, comma + I, k),
                &y[*numerus]))
        {
            redde FALSUM;
        }
        (*numerus)++;
    }
    redde VERUM;
}

interior s32
_cifra_hex (
    i8 littera)
{
    si (littera >= '0' && littera <= '9')
    {
        redde (s32)littera - '0';
    }
    si (littera >= 'a' && littera <= 'f')
    {
        redde (s32)littera - 'a' + X;
    }
    si (littera >= 'A' && littera <= 'F')
    {
        redde (s32)littera - 'A' + X;
    }
    redde -I;
}

/* color: index palettae 0 .. 15 aut #rrggbb */
interior b32
_color (
    chorda  verbum,
        i8* rgb)
{
    s32 index;
    i32 k;

    si (verbum.mensura > ZEPHYRUM && verbum.datum[ZEPHYRUM] == '#')
    {
        si (verbum.mensura != VII)
        {
            redde FALSUM;
        }
        per (k = ZEPHYRUM; k < III; k++)
        {
            s32 cifra_maior = _cifra_hex(verbum.datum[I + k * II]);
            s32 cifra_minor = _cifra_hex(verbum.datum[II + k * II]);

            si (cifra_maior < ZEPHYRUM || cifra_minor < ZEPHYRUM)
            {
                redde FALSUM;
            }
            rgb[k] = (i8)(cifra_maior * XVI + cifra_minor);
        }
        redde VERUM;
    }
    si (   !chorda_ut_s32(verbum, &index) || index < ZEPHYRUM
        || index >= (s32)AQUINAS_COLORUM_NUMERUS)
    {
        redde FALSUM;
    }
    memcpy(rgb, AQUINAS_PALETTE[index], III);
    redde VERUM;
}

/* "0", "1", "0.45": millesimae 0 .. maximum; tres loci summum */
interior b32
_millesimae (
    chorda  verbum,
       i32  maximum,
       i32* exitus)
{
    i32 k = ZEPHYRUM;
    i32 integra;
    i32 pars = ZEPHYRUM;
    i32 loci = ZEPHYRUM;

    si (   verbum.mensura < I
        || verbum.datum[ZEPHYRUM] < '0'
        || verbum.datum[ZEPHYRUM] > '9')
    {
        redde FALSUM;
    }
    integra  = (i32)(verbum.datum[ZEPHYRUM] - '0');
    k        = I;
    si (k < verbum.mensura)
    {
        si (verbum.datum[k] != '.' || k + I >= verbum.mensura)
        {
            redde FALSUM;
        }
        per (k = II; k < verbum.mensura; k++)
        {
            i8 littera = verbum.datum[k];

            si (littera < '0' || littera > '9' || loci >= III)
            {
                redde FALSUM;
            }
            pars = pars * X + (i32)(littera - '0');
            loci++;
        }
    }
    dum (loci < III)
    {
        pars *= X;
        loci++;
    }
    si (integra * M + pars > maximum)
    {
        redde FALSUM;
    }
    *exitus = integra * M + pars;
    redde VERUM;
}

interior b32
_titulus_est (
       constans chorda* titulus,
    constans character* literae)
{
    redde titulus && chorda_aequalis_literis(*titulus, literae);
}

/* omne attributum nodi in indice 'licita' (NIHIL terminato) est? */
interior b32
_attributa_licita (
                         StatusPicturae* p,
              constans StmlNodus* nodus,
    constans character* constans* licita)
{
    i32 numerus = nodus->attributa ? xar_numerus(nodus->attributa)
        : ZEPHYRUM;
    i32 k;
    i32 j;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        constans StmlAttributum* a =
            (constans StmlAttributum*)xar_obtinere(nodus->attributa,
                k);
                            b32 notum = FALSUM;

        per (j = ZEPHYRUM; a && licita[j]; j++)
        {
            si (_titulus_est(a->titulus, licita[j]))
            {
                notum = VERUM;
                frange;
            }
        }
        si (!notum)
        {
            redde _recusare(p, nodus->linea, "unknown attribute '",
                a ? a->titulus : NIHIL, "'");
        }
    }
    redde VERUM;
}

/* attributum in tago ipso scriptum est? (valor vacuus a lectore
 * ut absens redditur: hic distinguitur) */
interior b32
_attributum_scriptum (
    constans StmlNodus* nodus,
    constans character* titulus)
{
    i32 numerus = nodus->attributa ? xar_numerus(nodus->attributa)
        : ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        constans StmlAttributum* a =
            (constans StmlAttributum*)xar_obtinere(nodus->attributa,
                k);

        si (a && _titulus_est(a->titulus, titulus))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* nodus liberum elementum habet? (forma liberos non fert) */
interior b32
_habet_elementa (
    constans StmlNodus* nodus)
{
    i32 numerus = nodus->liberi ? xar_numerus(nodus->liberi)
        : ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        StmlNodus** locus =
            (StmlNodus**)xar_obtinere(nodus->liberi, k);

        si (   locus && *locus
            && (*locus)->genus == STML_NODUS_ELEMENTUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}


/* ==================================================
 * Proprietates formae
 * ================================================== */

interior i32
_densitas (
    i32 exemplar)
{
    constans i8* octeti  = exemplar_obtinere(exemplar);
            i32  n       = ZEPHYRUM;
            i32  k;
            i32  b;

    per (k = ZEPHYRUM; octeti && k < VIII; k++)
    {
        per (b = ZEPHYRUM; b < VIII; b++)
        {
            n += (i32)(octeti[k] >> b) & I;
        }
    }
    redde n;
}

interior b32
_colores_legere (
               StatusPicturae* p,
           constans StmlNodus* nodus,
                 Proprietates* q)
{
    constans chorda* valor = stml_attributum_capere(nodus, "colores");
             chorda  verba[II];
                i32  n;

    si (!valor)
    {
        redde VERUM;
    }
    si (   !_verba(valor, verba, II, &n) || n < I
        || !_color(verba[ZEPHYRUM], q->primus))
    {
        redde _recusare_attributum(p, nodus, "colores",
            "needs one or two colours (0 to 15 or #rrggbb)");
    }
    q->impletur = VERUM;
    si (n == II)
    {
        si (chorda_aequalis_literis(verba[I], "-"))
        {
            q->tegumentum = VERUM;
        }
        alioquin si (_color(verba[I], q->secundus))
        {
            q->duo = VERUM;
        }
        alioquin
        {
            redde _recusare_attributum(p, nodus, "colores",
                "needs one or two colours (0 to 15 or #rrggbb)");
        }
    }
    redde VERUM;
}

interior b32
_exemplar_legere (
               StatusPicturae* p,
           constans StmlNodus* nodus,
                 Proprietates* q,
                          b32* datum)
{
    constans chorda* valor = stml_attributum_capere(nodus,
        "exemplar");
                i32 k;

    *datum       = valor != NIHIL;
    q->exemplar  = EXEMPLAR_SOLIDUS;
    si (!valor)
    {
        redde VERUM;
    }
    per (k = ZEPHYRUM; k < EXEMPLAR_NUMERUS; k++)
    {
        si (chorda_aequalis_literis(*valor, EXEMPLARIA[k]))
        {
            q->exemplar = k;
            redde VERUM;
        }
    }
    redde _recusare_attributum(p, nodus, "exemplar",
        "is not a known pattern name");
}

interior b32
_vis_legere (
               StatusPicturae* p,
           constans StmlNodus* nodus,
                 Proprietates* q,
                          b32  exemplar_datum)
{
    constans chorda* valor = stml_attributum_capere(nodus, "vis");
             chorda  verba[I];
                i32  n;
                i32  mille = (q->duo && exemplar_datum)
                    ? VIS_ORDINARIA_MILLE : ZEPHYRUM;

    si (valor)
    {
        si (q->tegumentum)
        {
            redde _recusare_attributum(p, nodus, "vis",
                "has no meaning on an overlay (second colour '-')");
        }
        si (   !_verba(valor, verba, I, &n) || n != I
            || !_millesimae(verba[ZEPHYRUM], M, &mille)
            || (mille > ZEPHYRUM && mille < VIS_MINIMA_MILLE))
        {
            redde _recusare_attributum(p, nodus, "vis",
                "must be 0, or from 0.4 to 1 (three decimals at"
                " most)");
        }
    }
    q->vis = (mille * VIS_PLENA + D) / M;
    redde VERUM;
}

interior b32
_stilum_legere (
               StatusPicturae* p,
           constans StmlNodus* nodus,
                 Proprietates* q,
                          b32  requisitus)
{
    constans chorda* valor = stml_attributum_capere(nodus, "stilus");
             chorda  verba[II];
                i32  n;
                s32  crassitudo;

    si (!valor)
    {
        redde requisitus
            ? _recusare_attributum(p, nodus, "stilus", "is required")
            : VERUM;
    }
    si (   !_verba(valor, verba, II, &n) || n != II
        || !chorda_ut_s32(verba[ZEPHYRUM], &crassitudo)
        || crassitudo < I || crassitudo > (s32)CRASSITUDO_MAXIMA
        || !_color(verba[I], q->color_marginis))
    {
        redde _recusare_attributum(p, nodus, "stilus",
            "needs a width from 1 to 16 and a colour");
    }
    q->margo       = VERUM;
    q->crassitudo  = crassitudo;
    redde VERUM;
}

interior b32
_titulum_legere (
               StatusPicturae* p,
           constans StmlNodus* nodus,
                 Proprietates* q)
{
    constans chorda* valor = stml_attributum_capere(nodus, "titulus");
                i32  k;

    si (!valor && !_attributum_scriptum(nodus, "titulus"))
    {
        redde VERUM;
    }
    si (   !valor || valor->mensura < I
        || valor->mensura > TITULUS_MAXIMUS)
    {
        redde _recusare_attributum(p, nodus, "titulus",
            "must have from 1 to 64 characters");
    }
    per (k = ZEPHYRUM; k < p->titulorum; k++)
    {
        si (chorda_aequalis(p->tituli[k], *valor))
        {
            redde _recusare(p, nodus->linea, "the title '", valor,
                "' is used twice");
        }
    }
    si (p->titulorum >= GRAPHIS_TITULI_MAXIMI)
    {
        redde _recusare(p, nodus->linea,
            "more than 255 titled shapes", NIHIL, NIHIL);
    }
    p->tituli[p->titulorum] = *valor;
    p->lineae[p->titulorum] = nodus->linea;
    p->titulorum++;
    q->dominus = p->titulorum;
    redde VERUM;
}

/* lucens="verum": forma lucem dat nec accipit */
interior b32
_lucens_legere (
        StatusPicturae* p,
    constans StmlNodus* nodus,
          Proprietates* q)
{
    constans chorda* valor = stml_attributum_capere(nodus, "lucens");

    si (!valor)
    {
        redde VERUM;
    }
    si (!chorda_aequalis_literis(*valor, "verum"))
    {
        redde _recusare_attributum(p, nodus, "lucens",
            "can only be \"verum\"");
    }
    q->lucens = VERUM;
    redde VERUM;
}

/* proprietates formae impletae (non lineae) */
interior b32
_proprietates (
               StatusPicturae* p,
           constans StmlNodus* nodus,
                 Proprietates* q)
{
    b32 exemplar_datum;
    i32 densitas;
    i32 k;

    memset(q, ZEPHYRUM, magnitudo(*q));
    si (   !_colores_legere(p, nodus, q)
        || !_exemplar_legere(p, nodus, q, &exemplar_datum)
        || !_vis_legere(p, nodus, q, exemplar_datum)
        || !_stilum_legere(p, nodus, q, FALSUM)
        || !_lucens_legere(p, nodus, q))
    {
        redde FALSUM;
    }
    si (!q->impletur && !q->margo)
    {
        redde _recusare_elementum(p, nodus,
            "needs colores or stilus");
    }
    densitas = _densitas(q->exemplar);
    per (k = ZEPHYRUM; k < III; k++)
    {
        q->planus[k] = q->duo
            ? (i8)(((i32)q->primus[k] * densitas
                + (i32)q->secundus[k] * (LXIV - densitas) + XXXII)
                / LXIV)
            : q->primus[k];
    }
    redde _titulum_legere(p, nodus, q);
}


/* ==================================================
 * Pixela
 * ================================================== */

interior vacuum
_pixelum_impletionis (
                  StatusPicturae* p,
    constans Proprietates* q,
                      s32  x,
                      s32  y)
{
    i32  locus;
     i8* pixelum;
    b32  bitus;
    i32  k;

    si (x < ZEPHYRUM || y < ZEPHYRUM || x >= LAT || y >= ALT)
    {
        redde;
    }
    p->picta++;
    locus    = (i32)(y * LAT + x);
    pixelum  = p->fons + locus * IV;
    bitus    = exemplar_punctum(q->exemplar, x, y);
    si (q->tegumentum)
    {
        si (!bitus)
        {
            redde;
        }
        memcpy(pixelum, q->primus, III);
        p->larva[locus]     = 255;
        p->lucentes[locus]  = (i8)q->lucens;
        /* tegumentum sine titulo regionem subiectam non rumpit */
        si (!q->dominus)
        {
            redde;
        }
    }
    alioquin si (!q->duo)
    {
        memcpy(pixelum, q->primus, III);
        p->larva[locus] = ZEPHYRUM;
    }
    alioquin
    {
        per (k = ZEPHYRUM; k < III; k++)
        {
            i32 positus = bitus ? q->primus[k] : q->secundus[k];

            pixelum[k] = (i8)(((i32)q->planus[k] * (VIS_PLENA - q->vis)
                + positus * q->vis + CXXVIII) >> VIII);
        }
        p->larva[locus] = (i8)(q->vis > 255 ? 255 : q->vis);
    }
    p->lucentes[locus]  = (i8)q->lucens;
    p->domini[locus]    = (i8)q->dominus;
}

interior vacuum
_pixelum_marginis (
                  StatusPicturae* p,
    constans Proprietates* q,
                      s32  x,
                      s32  y)
{
    i32 locus;

    si (x < ZEPHYRUM || y < ZEPHYRUM || x >= LAT || y >= ALT)
    {
        redde;
    }
    p->picta++;
    locus = (i32)(y * LAT + x);
    memcpy(p->fons + locus * IV, q->color_marginis, III);
    p->larva[locus]     = ZEPHYRUM;
    p->lucentes[locus]  = (i8)q->lucens;
    p->domini[locus]    = (i8)q->dominus;
}

/* pavimentum a / b (b > 0), etiam pro negativis (C89 divisionem
 * negativorum non definit) */
interior s64
_pavimentum (
    s64 a,
    s64 b)
{
    redde a >= ZEPHYRUM ? a / b : -((-a + b - I) / b);
}

/* tectum a / b (b > 0) */
interior s64
_tectum (
    s64 a,
    s64 b)
{
    redde -_pavimentum(-a, b);
}

/* segmentum: quadrata lateris 'crassitudo' per iter posita */
interior vacuum
_segmentum (
                  StatusPicturae* p,
    constans Proprietates* q,
                      s32  x_a,
                      s32  y_a,
                      s32  x_b,
                      s32  y_b)
{
    s32 motus_x  = x_b - x_a;
    s32 motus_y  = y_b - y_a;
    s32 gradus   = motus_x < ZEPHYRUM ? -motus_x : motus_x;
    s32 altus    = motus_y < ZEPHYRUM ? -motus_y : motus_y;
    s32 ante     = (q->crassitudo - I) / II;
    s32 k;
    s32 u;
    s32 v;

    si (altus > gradus)
    {
        gradus = altus;
    }
    per (k = ZEPHYRUM; k <= gradus; k++)
    {
        s32 x = x_a;
        s32 y = y_a;

        si (gradus > ZEPHYRUM)
        {
            x += (s32)_pavimentum((s64)II * motus_x * k + gradus,
                (s64)II * gradus);
            y += (s32)_pavimentum((s64)II * motus_y * k + gradus,
                (s64)II * gradus);
        }
        /* quadratum totum extra picturam: nihil */
        si (   x + q->crassitudo < ZEPHYRUM || x - q->crassitudo > LAT
            || y + q->crassitudo < ZEPHYRUM
            || y - q->crassitudo > ALT)
        {
            perge;
        }
        per (v = ZEPHYRUM; v < q->crassitudo; v++)
        {
            per (u = ZEPHYRUM; u < q->crassitudo; u++)
            {
                _pixelum_marginis(p, q, x - ante + u, y - ante + v);
            }
        }
    }
}

/* polygonum per lineas: pixelum intra si centrum eius intra est.
 * Sectio lineae cum latere: x duplicatum = numerator / denominator */
interior vacuum
_polygonum_implere (
                  StatusPicturae* p,
    constans Proprietates* q,
             constans s32* x,
             constans s32* y,
                      i32  numerus)
{
    s64 numeratores[PUNCTA_MAXIMA];
    s64 denominatores[PUNCTA_MAXIMA];
    s32 summus   = y[ZEPHYRUM];
    s32 infimus  = y[ZEPHYRUM];
    s32 ordo;
    i32 k;

    per (k = I; k < numerus; k++)
    {
        si (y[k] < summus)
        {
            summus = y[k];
        }
        si (y[k] > infimus)
        {
            infimus = y[k];
        }
    }
    si (summus < ZEPHYRUM)
    {
        summus = ZEPHYRUM;
    }
    si (infimus > ALT - I)
    {
        infimus = ALT - I;
    }
    per (ordo = summus; ordo <= infimus; ordo++)
    {
        s32 medium     = II * ordo + I;    /* y centri, duplicatum */
        i32 sectiones  = ZEPHYRUM;
        i32 j;

        per (k = ZEPHYRUM; k < numerus; k++)
        {
            i32 sequens  = (k + I) % numerus;
            s32 y_a      = y[k];
            s32 y_b      = y[sequens];
            s64 numerator;
            s64 denominator;

            si ((II * y_a < medium) == (II * y_b < medium))
            {
                perge;
            }
            denominator = (s64)(y_b - y_a);
            numerator   = (s64)II * x[k] * denominator
                + (s64)(medium - II * y_a) * (x[sequens] - x[k]);
            si (denominator < ZEPHYRUM)
            {
                denominator  = -denominator;
                numerator    = -numerator;
            }
            /* insertio ordinata: a/b < c/d <=> a*d < c*b */
            j = sectiones;
            dum (   j > ZEPHYRUM
                 && numerator * denominatores[j - I]
                    < numeratores[j - I] * denominator)
            {
                numeratores[j]    = numeratores[j - I];
                denominatores[j]  = denominatores[j - I];
                j--;
            }
            numeratores[j]    = numerator;
            denominatores[j]  = denominator;
            sectiones++;
        }
        per (k = ZEPHYRUM; k + I < sectiones; k += II)
        {
            /* centrum pixeli (2x + 1) inter sectiones */
            s64 primus  = _tectum(numeratores[k] - denominatores[k],
                II * denominatores[k]);
            s64 ultimus = _pavimentum(
                numeratores[k + I] - denominatores[k + I],
                II * denominatores[k + I]);
            s32 columna;

            si (primus < ZEPHYRUM)
            {
                primus = ZEPHYRUM;
            }
            si (ultimus > LAT - I)
            {
                ultimus = LAT - I;
            }
            per (columna = (s32)primus; columna <= (s32)ultimus;
                 columna++)
            {
                _pixelum_impletionis(p, q, columna, ordo);
            }
        }
    }
}

/* centrum pixeli intra ellipsim rectangulo (x, y, lat, alt)
 * inscriptam? */
interior b32
_in_ovo (
    s32 columna,
    s32 ordo,
    s32 x,
    s32 y,
    s32 lat,
    s32 alt)
{
    s64 a = (s64)II * (columna - x) + I - lat;
    s64 b = (s64)II * (ordo - y) + I - alt;
    s64 l = (s64)lat * lat;
    s64 h = (s64)alt * alt;

    redde a * a * h + b * b * l <= l * h;
}

interior vacuum
_ovum_pingere (
                  StatusPicturae* p,
    constans Proprietates* q,
                      s32  x,
                      s32  y,
                      s32  lat,
                      s32  alt)
{
    s32 summus     = y < ZEPHYRUM ? ZEPHYRUM : y;
    s32 infimus    = y + alt - I > ALT - I ? ALT - I : y + alt - I;
    s32 sinister   = x < ZEPHYRUM ? ZEPHYRUM : x;
    s32 dexter     = x + lat - I > LAT - I ? LAT - I : x + lat - I;
    s32 intus_lat  = lat - II * q->crassitudo;
    s32 intus_alt  = alt - II * q->crassitudo;
    s32 columna;
    s32 ordo;

    per (ordo = summus; ordo <= infimus; ordo++)
    {
        per (columna = sinister; columna <= dexter; columna++)
        {
            si (!_in_ovo(columna, ordo, x, y, lat, alt))
            {
                perge;
            }
            si (q->impletur)
            {
                _pixelum_impletionis(p, q, columna, ordo);
            }
            /* margo: intra ovum, extra ovum crassitudine minutum */
            si (   q->margo
                && (   intus_lat < I || intus_alt < I
                    || !_in_ovo(columna, ordo, x + q->crassitudo,
                        y + q->crassitudo, intus_lat, intus_alt)))
            {
                _pixelum_marginis(p, q, columna, ordo);
            }
        }
    }
}


/* ==================================================
 * Formae
 * ================================================== */

hic_manens constans character* constans LICITA_FINIUM[] = {
    "fines", "colores", "exemplar", "vis", "stilus", "titulus",
    "lucens", NIHIL
};
hic_manens constans character* constans LICITA_PUNCTORUM[] = {
    "puncta", "colores", "exemplar", "vis", "stilus", "titulus",
    "lucens", NIHIL
};
hic_manens constans character* constans LICITA_LINEAE[] = {
    "puncta", "stilus", "titulus", "lucens", NIHIL
};
hic_manens constans character* constans LICITA_GREGIS[] = {
    "locus", NIHIL
};
hic_manens constans character* constans LICITA_RADICIS[] = {
    "titulus", "latitudo", "altitudo", "lux", "tinctura", "umbra",
    NIHIL
};
hic_manens constans character* constans LICITA_LUMINIS[] = {
    "locus", "radii", "area", "mollities", "color", "vis", NIHIL
};

/* forma nihil intra picturam pinxit? */
interior b32
_picta_probare (
               StatusPicturae* p,
           constans StmlNodus* nodus)
{
    redde p->picta > ZEPHYRUM ? VERUM
        : _recusare_elementum(p, nodus,
            "paints no pixel inside the picture (it lies outside,"
            " or is too thin to cover a pixel centre)");
}

/* rectangulum et ovum */
interior b32
_forma_finium (
               StatusPicturae* p,
    constans StmlNodus* nodus,
                   s32  motus_x,
                   s32  motus_y,
                   b32  ovum)
{
     constans chorda* valor;
        Proprietates  q;
                 s32  f[IV];
                 s32  columna;
                 s32  ordo;

    si (!_attributa_licita(p, nodus, LICITA_FINIUM))
    {
        redde FALSUM;
    }
    valor = stml_attributum_capere(nodus, "fines");
    si (!valor)
    {
        redde _recusare_attributum(p, nodus, "fines", "is required");
    }
    si (!_coordinatae(valor, f, IV))
    {
        redde _recusare_attributum(p, nodus, "fines",
            "needs 4 integers from -4096 to 4096 (x y width height)");
    }
    si (f[II] < I || f[III] < I)
    {
        redde _recusare_attributum(p, nodus, "fines",
            "size must be positive");
    }
    si (!_proprietates(p, nodus, &q))
    {
        redde FALSUM;
    }
    f[ZEPHYRUM]  += motus_x;
    f[I]         += motus_y;
    p->picta     = ZEPHYRUM;
    si (ovum)
    {
        _ovum_pingere(p, &q, f[ZEPHYRUM], f[I], f[II], f[III]);
        redde _picta_probare(p, nodus);
    }
    si (q.impletur)
    {
        s32 summus   = f[I] < ZEPHYRUM ? ZEPHYRUM : f[I];
        s32 infimus  = f[I] + f[III] > ALT ? ALT : f[I] + f[III];
        s32 sinister = f[ZEPHYRUM] < ZEPHYRUM ? ZEPHYRUM
            : f[ZEPHYRUM];
        s32 dexter   = f[ZEPHYRUM] + f[II] > LAT ? LAT
            : f[ZEPHYRUM] + f[II];

        per (ordo = summus; ordo < infimus; ordo++)
        {
            per (columna = sinister; columna < dexter; columna++)
            {
                _pixelum_impletionis(p, &q, columna, ordo);
            }
        }
    }
    si (q.margo)
    {
        s32 dexter   = f[ZEPHYRUM] + f[II] - I;
        s32 infimus  = f[I] + f[III] - I;

        _segmentum(p, &q, f[ZEPHYRUM], f[I], dexter, f[I]);
        _segmentum(p, &q, dexter, f[I], dexter, infimus);
        _segmentum(p, &q, dexter, infimus, f[ZEPHYRUM], infimus);
        _segmentum(p, &q, f[ZEPHYRUM], infimus, f[ZEPHYRUM], f[I]);
    }
    redde _picta_probare(p, nodus);
}

/* polygonum et linea */
interior b32
_forma_punctorum (
               StatusPicturae* p,
           constans StmlNodus* nodus,
                          s32  motus_x,
                          s32  motus_y,
                          b32  linea)
{
     constans chorda* valor;
        Proprietates  q;
                 s32  x[PUNCTA_MAXIMA];
                 s32  y[PUNCTA_MAXIMA];
                 i32  numerus;
                 i32  k;

    si (!_attributa_licita(p, nodus,
        linea ? LICITA_LINEAE : LICITA_PUNCTORUM))
    {
        redde FALSUM;
    }
    valor = stml_attributum_capere(nodus, "puncta");
    si (!valor)
    {
        redde _recusare_attributum(p, nodus, "puncta", "is required");
    }
    si (   !_paria(valor, x, y, &numerus)
        || numerus < (linea ? II : III))
    {
        redde _recusare_attributum(p, nodus, "puncta", linea
            ? "needs 2 to 256 points \"x,y\" of integers from -4096"
              " to 4096"
            : "needs 3 to 256 points \"x,y\" of integers from -4096"
              " to 4096");
    }
    si (linea)
    {
        memset(&q, ZEPHYRUM, magnitudo(q));
        si (   !_stilum_legere(p, nodus, &q, VERUM)
            || !_lucens_legere(p, nodus, &q)
            || !_titulum_legere(p, nodus, &q))
        {
            redde FALSUM;
        }
    }
    alioquin si (!_proprietates(p, nodus, &q))
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        x[k] += motus_x;
        y[k] += motus_y;
    }
    p->picta = ZEPHYRUM;
    si (q.impletur)
    {
        _polygonum_implere(p, &q, x, y, numerus);
    }
    si (q.margo)
    {
        per (k = ZEPHYRUM; k + I < numerus; k++)
        {
            _segmentum(p, &q, x[k], y[k], x[k + I], y[k + I]);
        }
        si (!linea)
        {
            _segmentum(p, &q, x[numerus - I], y[numerus - I],
                x[ZEPHYRUM], y[ZEPHYRUM]);
        }
    }
    redde _picta_probare(p, nodus);
}

/* vis lucis: millesimae 0 .. maximum in partes GRAPHIS_LUX_UNA;
 * absens: praesumpta */
interior b32
_vis_lucis (
        StatusPicturae* p,
    constans StmlNodus* nodus,
    constans character* titulus,
                   i32  maximum,
                   i32  praesumpta,
    constans character* quid,
                   i32* exitus)
{
    constans chorda* valor = stml_attributum_capere(nodus, titulus);
             chorda  verba[I];
                i32  n;
                i32  mille;

    *exitus = praesumpta;
    si (!valor)
    {
        redde VERUM;
    }
    si (   !_verba(valor, verba, I, &n) || n != I
        || !_millesimae(verba[ZEPHYRUM], maximum, &mille))
    {
        redde _recusare_attributum(p, nodus, titulus, quid);
    }
    *exitus = (mille * GRAPHIS_LUX_UNA + D) / M;
    redde VERUM;
}

/* color unus; absens: albus */
interior b32
_color_lucis (
        StatusPicturae* p,
    constans StmlNodus* nodus,
    constans character* titulus,
                    i8* rgb)
{
    constans chorda* valor = stml_attributum_capere(nodus, titulus);
             chorda  verba[I];
                i32  n;

    memset(rgb, 255, III);
    si (!valor)
    {
        redde VERUM;
    }
    si (   !_verba(valor, verba, I, &n) || n != I
        || !_color(verba[ZEPHYRUM], rgb))
    {
        redde _recusare_attributum(p, nodus, titulus,
            "needs one colour (0 to 15 or #rrggbb)");
    }
    redde VERUM;
}

/* elementum lumen: lumen puncti (locus, radii) aut areae (area,
 * mollities) */
interior b32
_lumen_legere (
        StatusPicturae* p,
    constans StmlNodus* nodus)
{
    constans chorda* locus  = stml_attributum_capere(nodus, "locus");
    constans chorda* radii  = stml_attributum_capere(nodus, "radii");
    constans chorda* area   = stml_attributum_capere(nodus, "area");
    constans chorda* mollis = stml_attributum_capere(nodus,
        "mollities");
       GraphisLumen* l;
                s32  duo[II];

    si (!_attributa_licita(p, nodus, LICITA_LUMINIS))
    {
        redde FALSUM;
    }
    si ((locus != NIHIL) == (area != NIHIL))
    {
        redde _recusare_elementum(p, nodus,
            "needs locus or area (one of them, not both)");
    }
    si (p->lux.luminum >= GRAPHIS_LUMINA_MAXIMA)
    {
        redde _recusare_elementum(p, nodus,
            "is one light too many (16 at most)");
    }
    l = &p->lumina[p->lux.luminum];
    memset(l, ZEPHYRUM, magnitudo(*l));
    si (locus)
    {
        si (mollis)
        {
            redde _recusare_attributum(p, nodus, "mollities",
                "belongs to an area light");
        }
        si (!_coordinatae(locus, duo, II))
        {
            redde _recusare_attributum(p, nodus, "locus",
                "needs 2 integers from -4096 to 4096");
        }
        l->punctum  = VERUM;
        l->x        = duo[ZEPHYRUM];
        l->y        = duo[I];
        si (!radii)
        {
            redde _recusare_attributum(p, nodus, "radii",
                "is required");
        }
        si (   !_coordinatae(radii, duo, II) || duo[ZEPHYRUM] < I
            || duo[I] < I)
        {
            redde _recusare_attributum(p, nodus, "radii",
                "needs 2 integers from 1 to 4096");
        }
        l->radius_x = duo[ZEPHYRUM];
        l->radius_y = duo[I];
    }
    alioquin
    {
        s32 x[PUNCTA_MAXIMA];
        s32 y[PUNCTA_MAXIMA];
        i32 numerus;

        si (radii)
        {
            redde _recusare_attributum(p, nodus, "radii",
                "belongs to a point light");
        }
        si (   !_paria(area, x, y, &numerus) || numerus < III
            || numerus > GRAPHIS_AREA_PUNCTA)
        {
            redde _recusare_attributum(p, nodus, "area",
                "needs 3 to 64 points \"x,y\" of integers from -4096"
                " to 4096");
        }
        l->punctorum  = numerus;
        l->area_x     = (s32*)piscina_allocare(p->piscina,
            (memoriae_index)(numerus * magnitudo(s32)));
        l->area_y     = (s32*)piscina_allocare(p->piscina,
            (memoriae_index)(numerus * magnitudo(s32)));
        si (!l->area_x || !l->area_y)
        {
            redde _recusare(p, ZEPHYRUM, "out of memory", NIHIL,
                NIHIL);
        }
        memcpy(l->area_x, x, numerus * magnitudo(s32));
        memcpy(l->area_y, y, numerus * magnitudo(s32));
        si (mollis)
        {
            s32 m;

            si (   !_coordinatae(mollis, &m, I) || m < ZEPHYRUM
                || m > (s32)GRAPHIS_MOLLITIES_MAXIMA)
            {
                redde _recusare_attributum(p, nodus, "mollities",
                    "needs an integer from 0 to 16");
            }
            l->mollities = (i32)m;
        }
    }
    si (   !_color_lucis(p, nodus, "color", l->color)
        || !_vis_lucis(p, nodus, "vis", II * M, GRAPHIS_LUX_UNA,
            "must be from 0 to 2 (three decimals at most)", &l->vis))
    {
        redde FALSUM;
    }
    p->lux.luminum++;
    redde VERUM;
}

/* liberi radicis aut gregis */
interior b32
_liberi (
               StatusPicturae* p,
           constans StmlNodus* nodus,
                          s32  motus_x,
                          s32  motus_y,
                          i32  altitudo)
{
    i32 numerus = nodus->liberi ? xar_numerus(nodus->liberi)
        : ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
                StmlNodus** locus =
                    (StmlNodus**)xar_obtinere(nodus->liberi, k);
        constans StmlNodus* liber = locus ? *locus : NIHIL;
                       b32  bene;

        si (!liber || liber->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        si (_titulus_est(liber->titulus, "grex"))
        {
            constans chorda* valor;
                        s32  locus_gregis[II];

            si (!_attributa_licita(p, liber, LICITA_GREGIS))
            {
                redde FALSUM;
            }
            locus_gregis[ZEPHYRUM] = ZEPHYRUM;
            locus_gregis[I] = ZEPHYRUM;
            valor = stml_attributum_capere(liber, "locus");
            si (valor && !_coordinatae(valor, locus_gregis, II))
            {
                redde _recusare_attributum(p, liber, "locus",
                    "needs 2 integers from -4096 to 4096");
            }
            si (altitudo >= GREGES_MAXIMI)
            {
                redde _recusare_elementum(p, liber,
                    "is nested more than 16 deep");
            }
            bene = _liberi(p, liber, motus_x + locus_gregis[ZEPHYRUM],
                motus_y + locus_gregis[I], altitudo + I);
        }
        alioquin si (   _titulus_est(liber->titulus, "rectangulum")
                     || _titulus_est(liber->titulus, "ovum")
                     || _titulus_est(liber->titulus, "polygonum")
                     || _titulus_est(liber->titulus, "linea"))
        {
            si (_habet_elementa(liber))
            {
                redde _recusare_elementum(p, liber,
                    "takes no children");
            }
            si (   _titulus_est(liber->titulus, "rectangulum")
                || _titulus_est(liber->titulus, "ovum"))
            {
                bene = _forma_finium(p, liber, motus_x, motus_y,
                    _titulus_est(liber->titulus, "ovum"));
            }
            alioquin
            {
                bene = _forma_punctorum(p, liber, motus_x, motus_y,
                    _titulus_est(liber->titulus, "linea"));
            }
        }
        alioquin si (_titulus_est(liber->titulus, "lumen"))
        {
            si (altitudo > ZEPHYRUM)
            {
                redde _recusare_elementum(p, liber,
                    "must be a direct child of <pictura>");
            }
            si (_habet_elementa(liber))
            {
                redde _recusare_elementum(p, liber,
                    "takes no children");
            }
            bene = _lumen_legere(p, liber);
        }
        alioquin
        {
            redde _recusare_elementum(p, liber,
                "is an unknown element");
        }
        si (!bene)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* attributum radicis, si adest, numerum datum dicere debet */
interior b32
_magnitudo_radicis (
               StatusPicturae* p,
           constans StmlNodus* radix,
           constans character* titulus,
                          s32  speratus,
           constans character* quid)
{
    constans chorda* valor = stml_attributum_capere(radix, titulus);
                s32  x;

    si (valor && (!chorda_ut_s32(*valor, &x) || x != speratus))
    {
        redde _recusare_attributum(p, radix, titulus, quid);
    }
    redde VERUM;
}

/* regio quaeque satis pixelorum possidet? */
interior b32
_regiones_probare (
    StatusPicturae* p)
{
    i32 numeri[GRAPHIS_TITULI_MAXIMI + I];
    i32 k;

    memset(numeri, ZEPHYRUM, magnitudo(numeri));
    per (k = ZEPHYRUM; k < (i32)(LAT * ALT); k++)
    {
        numeri[p->domini[k]]++;
    }
    per (k = ZEPHYRUM; k < p->titulorum; k++)
    {
        si (numeri[k + I] < GRAPHIS_REGIO_MINIMA)
        {
            redde _recusare(p, p->lineae[k], "the region '",
                &p->tituli[k],
                "' owns fewer than 64 pixels after all shapes are"
                " painted");
        }
    }
    redde VERUM;
}


/* ==================================================
 * Publica
 * ================================================== */

interior GraphisFructus
_fructus_recusatus (
    constans StatusPicturae* p,
                    Piscina* piscina)
{
    GraphisFructus f;

    memset(&f, ZEPHYRUM, magnitudo(f));
    f.error = chorda_ex_literis(p->error, piscina);
    f.linea = p->linea;
    redde f;
}

GraphisFructus
graphis_legere (
     chorda  textus,
    Piscina* piscina)
{
             GraphisFructus  f;
             StatusPicturae  p;
               StmlResultus  r;
           DitheringFructus  d;
              GraphisTabula* tabula;
               GraphisLumen* lumina;
                         i8* illustratus;
                        b32  activi[XVI];
                        i32  pixela = (i32)(LAT * ALT);
                        i32  k;

    memset(&f, ZEPHYRUM, magnitudo(f));
    memset(&p, ZEPHYRUM, magnitudo(p));
    si (!piscina)
    {
        redde f;
    }
    p.piscina = piscina;
    r = stml_legere(textus, piscina, internamentum_creare(piscina));
    si (!r.successus || !r.elementum_radix)
    {
        chorda causa = r.error;

        (vacuum)_recusare(&p,
            r.successus ? ZEPHYRUM : r.linea_erroris,
            "the description is not valid STML: ",
            causa.mensura ? &causa : NIHIL,
            causa.mensura ? NIHIL : "no root element");
        redde _fructus_recusatus(&p, piscina);
    }
    si (!_titulus_est(r.elementum_radix->titulus, "pictura"))
    {
        (vacuum)_recusare(&p, r.elementum_radix->linea,
            "the root element must be <pictura>", NIHIL, NIHIL);
        redde _fructus_recusatus(&p, piscina);
    }
    p.fons   = (i8*)piscina_allocare(piscina,
        (memoriae_index)(pixela * IV));
    p.larva   = (i8*)piscina_allocare(piscina, (memoriae_index)pixela);
    p.domini  = (i8*)piscina_allocare(piscina, (memoriae_index)pixela);
    p.lucentes = (i8*)piscina_allocare(piscina,
        (memoriae_index)pixela);
    tabula   = (GraphisTabula*)piscina_allocare(piscina,
        magnitudo(GraphisTabula));
    si (!p.fons || !p.larva || !p.domini || !p.lucentes || !tabula)
    {
        (vacuum)_recusare(&p, ZEPHYRUM, "out of memory", NIHIL,
            NIHIL);
        redde _fructus_recusatus(&p, piscina);
    }
    memset(p.larva, ZEPHYRUM, pixela);
    memset(p.domini, ZEPHYRUM, pixela);
    memset(p.lucentes, ZEPHYRUM, pixela);
    p.lux.lumina = p.lumina;
    per (k = ZEPHYRUM; k < pixela; k++)
    {
        memcpy(p.fons + k * IV, AQUINAS_PALETTE[GRAPHIS_COLOR_FUNDI],
            III);
        p.fons[k * IV + III] = 255;
    }
    si (   !_attributa_licita(&p, r.elementum_radix, LICITA_RADICIS)
        || !_magnitudo_radicis(&p, r.elementum_radix, "latitudo",
            LAT, "must be 360 (the only picture size)")
        || !_magnitudo_radicis(&p, r.elementum_radix, "altitudo",
            ALT, "must be 240 (the only picture size)")
        || !_vis_lucis(&p, r.elementum_radix, "lux", M,
            GRAPHIS_LUX_UNA,
            "must be from 0 to 1 (three decimals at most)",
            &p.lux.lux)
        || !_vis_lucis(&p, r.elementum_radix, "umbra", M, ZEPHYRUM,
            "must be from 0 to 1 (three decimals at most)",
            &p.lux.umbra)
        || !_color_lucis(&p, r.elementum_radix, "tinctura",
            p.lux.tinctura)
        || !_liberi(&p, r.elementum_radix, ZEPHYRUM, ZEPHYRUM,
            ZEPHYRUM)
        || !_regiones_probare(&p))
    {
        redde _fructus_recusatus(&p, piscina);
    }
    illustratus = graphis_illustrare(p.fons, p.lucentes, (i32)LAT,
        (i32)ALT, &p.lux, piscina);
    lumina      = (GraphisLumen*)piscina_allocare(piscina,
        (memoriae_index)((p.lux.luminum ? p.lux.luminum : I)
            * magnitudo(GraphisLumen)));
    si (!illustratus || !lumina)
    {
        (vacuum)_recusare(&p, ZEPHYRUM, "out of memory", NIHIL,
            NIHIL);
        redde _fructus_recusatus(&p, piscina);
    }
    memcpy(lumina, p.lumina, p.lux.luminum * magnitudo(GraphisLumen));
    dithering_praeparare_omnes(activi);
    d = dithering_atkinson_colorum(illustratus, (i32)LAT, (i32)ALT,
        activi, piscina);
    memset(tabula, ZEPHYRUM, magnitudo(*tabula));
    tabula->lucentes    = p.lucentes;
    tabula->lux         = p.lux;
    tabula->lux.lumina  = lumina;
    tabula->fons        = p.fons;
    tabula->larva       = p.larva;
    tabula->domini      = p.domini;
    tabula->titulorum   = p.titulorum;
    tabula->tituli    = (chorda*)piscina_allocare(piscina,
        (memoriae_index)((p.titulorum ? p.titulorum : I)
            * magnitudo(chorda)));
    si (!d.successus || !d.indices || !tabula->tituli)
    {
        (vacuum)_recusare(&p, ZEPHYRUM, "out of memory", NIHIL,
            NIHIL);
        redde _fructus_recusatus(&p, piscina);
    }
    per (k = ZEPHYRUM; k < p.titulorum; k++)
    {
        tabula->tituli[k] = chorda_transcribere(p.tituli[k], piscina);
    }
    f.successus    = VERUM;
    f.tabula       = tabula;
    f.indices      = d.indices;
    f.illustratus  = illustratus;
    redde f;
}

constans character*
graphis_exemplar_titulus (
    i32 id)
{
    redde id < EXEMPLAR_NUMERUS ? EXEMPLARIA[id] : NIHIL;
}

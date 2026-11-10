/* probatio_gabc_pictura.c - pagina compositionis gabc in mandata (gabc
 * pars II, P4)
 *
 * Forma ordinaria (spatium VIII): caput VIII x VII -> VII rectangula
 * unius ordinis VIII lata; lineae systematis COLOR_TEXT_DIM; syllaba
 * illuminata COLOR_ACCENT_PRIMARY; rubrica rubrum palettae RGBA;
 * initialis ut quadrata scalae (nullus textus); notae compositae
 * demptae. Imago Salve sine fenestra in
 * build/gabc_salve_pagina_1.png (oculis inspicienda). */
#include "latina.h"
#include "credo.h"
#include "gabc.h"
#include "gabc_compositio.h"
#include "gabc_pictura.h"
#include "glyphae_ductae.h"
#include "mandatum.h"
#include "delineare_mandata.h"
#include "imago_png.h"
#include "internamentum.h"
#include "thema.h"
#include "color.h"
#include "figura.h"
#include "filum.h"
#include "piscina.h"
#include "tempus.h"
#include <stdio.h>
#include <string.h>

#define VIA_SALVE   "musica/fontes/salve_2435.gabc"
#define VIA_IMAGO   "build/gabc_salve_pagina_1.png"
#define SINE_INITIALI "initial-style:0;\n%%\n"
#define MARGO_TABULAE XVI


/* ==================================================
 * Instrumenta
 * ================================================== */

interior GabcCompositio*
_componere (
          CredoContextus* c,
      constans character* literae)
{
    GabcCompositionisForma  forma;
                GabcCantus* cantus;

    cantus = gabc_legere(chorda_ex_literis(literae, c->piscina),
        c->piscina);
    gabc_compositionis_formam_ordinariam(&forma);
    redde gabc_componere(cantus, &forma, c->piscina);
}

interior Mandata*
_pingere (
             CredoContextus* c,
    constans GabcCompositio* comp,
                        s32  illuminata)
{
    GabcPicturaStatus  status;
              Mandata* md;

    thema_initiare();
    md = mandata_creare(c->piscina, internamentum_creare(c->piscina));
    status.syllaba_illuminata = illuminata;
    status.nota_illuminata = -I;
    si (!gabc_paginam_pingere(comp, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM,
        &status,
            md))
    {
        redde NIHIL;
    }
    redde md;
}

interior constans GabcPictum*
_pictum (
     constans GabcCompositio* comp,
              GabcPictiGenus  genus,
                         s32  syllaba)
{
    i32 i;

    per (i = ZEPHYRUM; i < comp->numerus_pictorum; i++)
    {
        si (   comp->picta[i].genus == genus
            && (syllaba < -I || comp->picta[i].syllaba == syllaba))
        {
            redde &comp->picta[i];
        }
    }
    redde NIHIL;
}

/* rectangulum impletum (x, y, l, a) colore thematis aut RGBA? */
interior b32
_rectangulum_est (
     constans Mandata* md,
                  s32  x,
                  s32  y,
                  s32  latitudo,
                  s32  altitudo,
    ColorMandatiGenus  genus,
                  i32  valor)
{
         i32  i;
    Mandatum* m;

    per (i = ZEPHYRUM; i < mandata_numerus(md); i++)
    {
        m = mandata_obtinere(md, i);
        si (   m->genus          == MANDATUM_RECTANGULUM && m->impletum
            && m->fines.x        == x && m->fines.y == y
            && m->fines.latitudo == latitudo
            && m->fines.altitudo == altitudo && m->color.genus == genus
            && m->color.valor    == valor)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* mandatum textus cum litteris datis; NIHIL si nullum */
interior Mandatum*
_textus (
      constans Mandata* md,
    constans character* literae)
{
         i32  i;
         i32  n;
    Mandatum* m;

    n = (i32)strlen(literae);
    per (i = ZEPHYRUM; i < mandata_numerus(md); i++)
    {
        m = mandata_obtinere(md, i);
        si (   m->genus == MANDATUM_TEXTUS && m->textus.mensura == n
            && memcmp(m->textus.datum, literae, (size_t)n) == ZEPHYRUM)
        {
            redde m;
        }
    }
    redde NIHIL;
}


/* ==================================================
 * I: mandata
 * ================================================== */

interior vacuum
_mandata_probare (
    CredoContextus* c)
{
         GabcCompositio* comp;
    constans GabcPictum* p;
                Mandata* md;
               Mandatum* t;
                    s32  r;
                    i32  i;
                    i32  lineae;
                    i32  partiales;

    comp = _componere(c, SINE_INITIALI "(c4) a(g)");
    CREDO_NON_NIHIL(comp);
    md = _pingere(c, comp, -I);
    CREDO_NON_NIHIL(md);
    p = _pictum(comp, GABC_PICTUM_PARS, I);
    per (r = ZEPHYRUM; r < VII; r++)
    {
        CREDO_VERUM(_rectangulum_est(md, p->x, p->y - III + r, VIII, I,
            COLOR_MANDATI_THEMA, (i32)COLOR_TEXT));
    }
    lineae = ZEPHYRUM;
    per (i = ZEPHYRUM; i < mandata_numerus(md); i++)
    {
        Mandatum* m = mandata_obtinere(md, i);

        si (   m->genus          == MANDATUM_RECTANGULUM
            && m->color.genus    == COLOR_MANDATI_THEMA
            && m->color.valor    == (i32)COLOR_TEXT_DIM
            && m->fines.latitudo == CMXLIV)
        {
            lineae++;
        }
    }
    CREDO_AEQUALIS_I32(lineae, IV);
    t = _textus(md, "a");
    CREDO_NON_NIHIL(t);
    CREDO_AEQUALIS_I32((i32)t->color.genus, (i32)COLOR_MANDATI_THEMA);
    CREDO_AEQUALIS_I32(t->color.valor, (i32)COLOR_TEXT);

    /* illuminata: notae et textus syllabae; lineae non */
    md = _pingere(c, comp, I);
    per (r = ZEPHYRUM; r < VII; r++)
    {
        CREDO_VERUM(_rectangulum_est(md, p->x, p->y - III + r, VIII, I,
            COLOR_MANDATI_THEMA, (i32)COLOR_ACCENT_PRIMARY));
    }
    t = _textus(md, "a");
    CREDO_AEQUALIS_I32(t->color.valor, (i32)COLOR_ACCENT_PRIMARY);
    lineae = ZEPHYRUM;
    per (i = ZEPHYRUM; i < mandata_numerus(md); i++)
    {
        Mandatum* m = mandata_obtinere(md, i);

        si (   m->genus          == MANDATUM_RECTANGULUM
            && m->color.valor    == (i32)COLOR_TEXT_DIM
            && m->fines.latitudo == CMXLIV)
        {
            lineae++;
        }
    }
    CREDO_AEQUALIS_I32(lineae, IV);

    /* rubrica: rubrum palettae ut RGBA */
    comp  = _componere(c, SINE_INITIALI "(c4) a(g) *()");
    md    = _pingere(c, comp, -I);
    t     = _textus(md, "*");
    CREDO_NON_NIHIL(t);
    CREDO_AEQUALIS_I32((i32)t->color.genus, (i32)COLOR_MANDATI_RGBA);
    CREDO_AEQUALIS_I32(t->color.valor,
        color_ad_pixelum(color_ex_palette(PALETTE_DARK_RED)));

    /* porrectus: margines obliqui ut RGBA mixti */
    comp       = _componere(c, SINE_INITIALI "(c4) a(hgh)");
    md         = _pingere(c, comp, -I);
    partiales  = ZEPHYRUM;
    per (i = ZEPHYRUM; i < mandata_numerus(md); i++)
    {
        si (mandata_obtinere(md, i)->color.genus == COLOR_MANDATI_RGBA)
        {
            partiales++;
        }
    }
    CREDO_MAIOR_I32(partiales, ZEPHYRUM);

    /* pagina extra: FALSUM, mandata intacta */
    md = mandata_creare(c->piscina, internamentum_creare(c->piscina));
    CREDO_FALSUM(gabc_paginam_pingere(comp, I, ZEPHYRUM, ZEPHYRUM,
        NIHIL,
        md));
    CREDO_AEQUALIS_I32(mandata_numerus(md), ZEPHYRUM);
}


/* ==================================================
 * II: initialis
 * ================================================== */

/* rectangula intra capsam initialis: omnia altitudine scalae */
interior vacuum
_initialem_credere (
          CredoContextus* c,
      constans character* literae,
                     i32  scala)
{
         GabcCompositio* comp;
    constans GabcPictum* p;
                Mandata* md;
               Mandatum* m;
                    i32  i;
                    i32  intra;
                    i32  malae;

    comp  = _componere(c, literae);
    p     = _pictum(comp, GABC_PICTUM_INITIALIS, -II);
    CREDO_NON_NIHIL(p);
    CREDO_AEQUALIS_I32(p->fons, scala);
    md     = _pingere(c, comp, -I);
    intra  = ZEPHYRUM;
    malae  = ZEPHYRUM;
    per (i = ZEPHYRUM; i < mandata_numerus(md); i++)
    {
        m = mandata_obtinere(md, i);
        si (   m->genus   == MANDATUM_RECTANGULUM && m->fines.x >= p->x
            && m->fines.x < p->x + (s32)p->latitudo
            && m->fines.y >= p->y
            && m->fines.y < p->y + (s32)p->altitudo)
        {
            intra++;
            si (   m->fines.altitudo                != (s32)scala
                || m->fines.latitudo % (s32)scala   != ZEPHYRUM
                || (m->fines.y - p->y) % (s32)scala != ZEPHYRUM)
            {
                malae++;
            }
        }
    }
    CREDO_MAIOR_I32(intra, V);
    CREDO_AEQUALIS_I32(malae, ZEPHYRUM);
    CREDO_NIHIL(_textus(md, "S"));
}

interior vacuum
_initialem_probare (
    CredoContextus* c)
{
    /* initial-style I: XL / XVI = II; II: (II * XL + VIII) / XVI = V */
    _initialem_credere(c, "(c4) Sal(g)ve(h)", II);
    _initialem_credere(c,
        "initial-style:2;\n%%\n(c4) Sal(g)ve(h) (z) a(g)", V);
}


/* ==================================================
 * III: textus
 * ================================================== */

interior vacuum
_textum_probare (
    CredoContextus* c)
{
    GabcCompositio* comp;
           Mandata* md;

    /* e + acutus compositus: "et" pingitur */
    comp  = _componere(c, SINE_INITIALI "(c4) e\xCC\x81t(g)");
    md    = _pingere(c, comp, -I);
    CREDO_NON_NIHIL(_textus(md, "et"));
    /* œ Œ ǽ a glyphis ductis, non TOFU */
    CREDO_VERUM(glyphae_ductae_est(0x0153));
    CREDO_VERUM(glyphae_ductae_est(0x0152));
    CREDO_VERUM(glyphae_ductae_est(0x01FD));
    comp  = _componere(c, SINE_INITIALI "(c4) c\xC5\x93(g)");
    md    = _pingere(c, comp, -I);
    CREDO_NON_NIHIL(_textus(md, "c\xC5\x93"));
}


/* ==================================================
 * IV: imago sine fenestra
 * ================================================== */

interior vacuum
_imaginem_probare (
    CredoContextus* c)
{
    GabcCompositionisForma  forma;
                GabcCantus* cantus;
            GabcCompositio* comp;
           TabulaPixelorum* tp;
                   Mandata* md;
                     Imago  imago;
                PngFructus  png;
                     Fines  f;
       constans GabcPictum* p;
                       i32  latitudo;
                       i32  altitudo;
                       i32  x;
                       i32  y;
                       i32  vacuum_pixelum;
                       i32  extra;
                       i32  lineae;
                       i32  i;
                       f64  initium;

    thema_initiare();
    cantus = gabc_legere(filum_legere_totum(VIA_SALVE, c->piscina),
        c->piscina);
    gabc_compositionis_formam_ordinariam(&forma);
    comp = gabc_componere(cantus, &forma, c->piscina);
    CREDO_NON_NIHIL(comp);
    latitudo = forma.latitudo + II * MARGO_TABULAE;
    altitudo = forma.altitudo + II * MARGO_TABULAE;
    tp        = tabula_pixelorum_creare_nuda(c->piscina, latitudo,
                    altitudo);
    CREDO_NON_NIHIL(tp);
    vacuum_pixelum = tabula_pixelorum_obtinere_pixelum(tp, ZEPHYRUM,
        ZEPHYRUM);
    md = mandata_creare(c->piscina, internamentum_creare(c->piscina));
    f.x = MARGO_TABULAE;
    f.y = MARGO_TABULAE;
    f.latitudo = (s32)forma.latitudo;
    f.altitudo = (s32)forma.altitudo;
    mandata_rectangulum(md, f, figura_color_thematis(COLOR_BACKGROUND),
        VERUM);
    CREDO_VERUM(gabc_paginam_pingere(comp, ZEPHYRUM, MARGO_TABULAE,
        MARGO_TABULAE, NIHIL, md));
    imprimere("  Salve pagina I: %u mandata\n",
        (unsigned)mandata_numerus(md));
    tempus_initiare();
    initium = tempus_nunc();
    delineare_mandata(md, tp, NIHIL, NIHIL);
    imprimere("  delineare_mandata: %.2f ms\n",
        (tempus_nunc() - initium) * 1000.0);
    imago  = imago_ex_tabula(tp);
    png    = imago_png_scribere(&imago, VIA_IMAGO, c->piscina);
    CREDO_VERUM(png.successus);

    /* atramentum in omni linea systematis (ordo lineae, x + X) */
    lineae = ZEPHYRUM;
    per (i = ZEPHYRUM; i < comp->numerus_pictorum; i++)
    {
        p = &comp->picta[i];
        si (   p->genus    == GABC_PICTUM_RECTANGULUM
            && p->tinctura == GABC_TINCTURA_SYSTEMA && p->syllaba == -I)
        {
            lineae++;
            CREDO_VERUM(tabula_pixelorum_obtinere_pixelum(tp,
                (i32)(MARGO_TABULAE + p->x + X),
                (i32)(MARGO_TABULAE + p->y))
                != tabula_pixelorum_obtinere_pixelum(tp,
                       (i32)(MARGO_TABULAE + p->x + X),
                       (i32)(MARGO_TABULAE + p->y - II)));
        }
    }
    CREDO_MAIOR_I32(lineae, VIII);

    /* nihil extra paginam */
    extra = ZEPHYRUM;
    per (y = ZEPHYRUM; y < altitudo; y++)
    {
        per (x = ZEPHYRUM; x < latitudo; x++)
        {
            si (   (x < MARGO_TABULAE || y < MARGO_TABULAE
                 || x >= MARGO_TABULAE + forma.latitudo
                 || y >= MARGO_TABULAE + forma.altitudo)
                && tabula_pixelorum_obtinere_pixelum(tp, x, y)
                   != vacuum_pixelum)
            {
                extra++;
            }
        }
    }
    CREDO_AEQUALIS_I32(extra, ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "I: mandata", _mandata_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "II: initialis", _initialem_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "III: textus", _textum_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "IV: imago sine fenestra", _imaginem_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("gabc_pictura", SECTIONES);
}

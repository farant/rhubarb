/* probatio_pictor_glutinatio.c - glutinatio pendens in documento
 * (Franus 2026-10-13)
 *
 * parare: imago aptata et BIS ditherata (A Atkinson = idem ac
 * pictor_imaginem_aptare; B exemplaria, diversum), acta intacta,
 * praevisio RGBA = pixela strati futuri, sigilla diversa. confirmare:
 * stratum novum modi electi, revocatio una, reapertio eadem. abicere:
 * nihil in actis. parare iterum: prior abicitur; fons vacua: nihil
 * mutatur.
 */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "thema.h"
#include "color.h"
#include "volumen.h"
#include "credo.h"
#include "imago_typus.h"
#include "tabula_pixelorum.h"
#include "pictor_documentum.h"
#include <string.h>

#define LATUS_DOC CDLXXX

/* imago C x L: dimidium sinistrum rubrum, dextrum caeruleum, quadrans
 * infimus dexter perspicuus (ut probatio_pictor_imago) */
interior Imago
_imago_ficta (
    Piscina* p)
{
    Imago im;
      i32 x;
      i32 y;

    im.latitudo = C;
    im.altitudo = L;
    im.pixela = (i8*)piscina_allocare(p, (memoriae_index)(C * L * IV));
    per (y = ZEPHYRUM; y < L; y++)
    {
        per (x = ZEPHYRUM; x < C; x++)
        {
            i8* q        = im.pixela + (y * C + x) * IV;
            q[ZEPHYRUM]  = x < L ? (i8)CCLV : ZEPHYRUM;
            q[I]         = x < L ? ZEPHYRUM : (i8)C;
            q[II]        = x < L ? ZEPHYRUM : (i8)CCLV;
            q[III]       = (x >= L && y >= XXV) ? ZEPHYRUM : (i8)CCLV;
        }
    }
    redde im;
}

/* volumen sectionis: parare aperit, purgare (SEMPER) claudit - volumen
 * temporarium relictum portas alias rubras facit */
hic_manens Volumen* volumen_sectionis = NIHIL;

interior vacuum
_parare (
    CredoContextus* c)
{
    volumen_sectionis = volumen_temporarium(c->piscina,
        "probatio_pictor_glutinatio");
}

interior vacuum
_purgare (
    CredoContextus* c)
{
    (vacuum)c;
    si (volumen_sectionis)
    {
        volumen_claudere(volumen_sectionis);
        volumen_sectionis = NIHIL;
    }
}

interior PictorDocumentum*
_documentum (
    CredoContextus* c,
          Volumen** vol)
{
    *vol = volumen_sectionis;
    CREDO_NECESSE_NON_NIHIL(*vol);
    redde pictor_documentum_creare(c->piscina,
        internamentum_creare(c->piscina),
        *vol, "", LATUS_DOC, LATUS_DOC, LXIV);
}

/* pixela strati (index) in rectangulo glutinationis = praevisio modi? */
interior b32
_stratum_praevisioni_aequale (
    constans PictorDocumentum* doc,
                          i32  index,
               constans Imago* praevisio,
                          s32  x0,
                          s32  y0)
{
    constans PictorStratum* s = pictor_documentum_stratum(doc, index);
              constans i32* pv =
                  (constans i32*)(constans vacuum*)praevisio->pixela;
    i32 x;
    i32 y;

    si (!s || !s->pixela) redde FALSUM;
    per (y = ZEPHYRUM; y < praevisio->altitudo; y++)
    {
        per (x = ZEPHYRUM; x < praevisio->latitudo; x++)
        {
            si ((i32)tabula_pixelorum_obtinere_pixelum(s->pixela,
                    (i32)x0 + x, (i32)y0 + y)
                != pv[y * praevisio->latitudo + x])
            {
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}

interior vacuum
_sectio_parare (
    CredoContextus* c)
{
                      Volumen* vol;
             PictorDocumentum* doc  = _documentum(c, &vol);
                        Imago  im   = _imago_ficta(c->piscina);
                           i8* atkinson;
                          s32  x;
                          s32  y;
                          i32  w;
                          i32  h;
                          s64  finis;
                          i32  k;
                          i32  mala;
    constans PictorGlutinatio* g;

    CREDO_NECESSE_NON_NIHIL(doc);
    finis = pictor_documentum_finis(doc);
    CREDO_FALSUM(doc->glutinatio.pendens);
    CREDO_NECESSE(pictor_documentum_glutinationem_parare(doc, &im));
    g = &doc->glutinatio;
    CREDO_VERUM(g->pendens);
    /* acta intacta: nihil in cauda, strata eadem */
    CREDO_VERUM(pictor_documentum_finis(doc) == finis);
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_stratorum(doc), I);

    /* geometria et A = pictor_imaginem_aptare (Atkinson) */
    atkinson = pictor_imaginem_aptare(&im, LATUS_DOC, LATUS_DOC,
        c->piscina, &x, &y, &w, &h);
    CREDO_NECESSE_NON_NIHIL(atkinson);
    CREDO_AEQUALIS_S32(g->x, x);
    CREDO_AEQUALIS_S32(g->y, y);
    CREDO_AEQUALIS_I32(g->latitudo, w);
    CREDO_AEQUALIS_I32(g->altitudo, h);
    CREDO_VERUM(memcmp(g->indices[PICTOR_GLUTINATIO_ATKINSON], atkinson,
        (memoriae_index)(w * h)) == ZEPHYRUM);
    CREDO_VERUM(memcmp(g->indices[PICTOR_GLUTINATIO_EXEMPLARIA],
        atkinson,
        (memoriae_index)(w * h)) != ZEPHYRUM);

    /* B: indices palettae aut 0xFF, perspicuitas eadem ac A */
    mala = ZEPHYRUM;
    per (k = ZEPHYRUM; k < w * h; k++)
    {
        i8 a = g->indices[PICTOR_GLUTINATIO_ATKINSON][k];
        i8 b = g->indices[PICTOR_GLUTINATIO_EXEMPLARIA][k];
        si (   (b >= XVI && b != (i8)CCLV)
            || ((a == (i8)CCLV) != (b == (i8)CCLV)))
        {
            mala++;
        }
    }
    CREDO_AEQUALIS_I32(mala, ZEPHYRUM);

    /* praevisio: dimensiones, perspicuum alpha 0, sigilla diversa */
    CREDO_AEQUALIS_I32(g->praevisio[PICTOR_GLUTINATIO_EXEMPLARIA].latitudo,
        w);
    CREDO_AEQUALIS_I32(g->praevisio[PICTOR_GLUTINATIO_EXEMPLARIA].altitudo,
        h);
    mala = ZEPHYRUM;
    per (k = ZEPHYRUM; k < w * h; k++)
    {
        i8 alpha =
            g->praevisio[PICTOR_GLUTINATIO_EXEMPLARIA].pixela[k * IV
                + III];
        si ((g->indices[PICTOR_GLUTINATIO_EXEMPLARIA][k] == (i8)CCLV)
            != (alpha == ZEPHYRUM))
        {
            mala++;
        }
    }
    CREDO_AEQUALIS_I32(mala, ZEPHYRUM);
    CREDO_VERUM(strlen(g->sigillum[PICTOR_GLUTINATIO_ATKINSON])
        == SIGILLUM_HEX_MENSURA - I);
    CREDO_VERUM(strcmp(g->sigillum[PICTOR_GLUTINATIO_ATKINSON],
        g->sigillum[PICTOR_GLUTINATIO_EXEMPLARIA]) != ZEPHYRUM);
}

interior vacuum
_sectio_confirmare (
    CredoContextus* c)
{
             Volumen* vol;
    PictorDocumentum* doc  = _documentum(c, &vol);
               Imago  im   = _imago_ficta(c->piscina);
               Imago  praevisio;
                 s32  x0;
                 s32  y0;
                 s32  id;
    PictorDocumentum* alter;

    CREDO_NECESSE_NON_NIHIL(doc);
    /* sine pendente: nihil */
    CREDO_AEQUALIS_S32(pictor_documentum_glutinationem_confirmare(doc,
        PICTOR_GLUTINATIO_EXEMPLARIA, I), ZEPHYRUM);
    CREDO_NECESSE(pictor_documentum_glutinationem_parare(doc, &im));
    /* modus extra: nihil, pendens manet */
    CREDO_AEQUALIS_S32(pictor_documentum_glutinationem_confirmare(doc,
        PICTOR_GLUTINATIO_MODI, I), ZEPHYRUM);
    CREDO_VERUM(doc->glutinatio.pendens);

    praevisio = doc->glutinatio.praevisio[PICTOR_GLUTINATIO_EXEMPLARIA];
    x0 = doc->glutinatio.x;
    y0 = doc->glutinatio.y;
    id = pictor_documentum_glutinationem_confirmare(doc,
        PICTOR_GLUTINATIO_EXEMPLARIA, I);
    CREDO_AEQUALIS_S32(id, II);
    CREDO_FALSUM(doc->glutinatio.pendens);
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_stratorum(doc), II);
    /* stratum = praevisio B, pixelum pro pixelo */
    CREDO_VERUM(_stratum_praevisioni_aequale(doc, I, &praevisio, x0,
        y0));

    /* reapertio: eadem */
    alter = pictor_documentum_aperire(c->piscina,
        internamentum_creare(c->piscina), vol, "");
    CREDO_NECESSE_NON_NIHIL(alter);
    CREDO_VERUM(memcmp(alter->tabula->pixela, doc->tabula->pixela,
        (memoriae_index)(doc->latitudo * doc->altitudo) * magnitudo(i32))
            == ZEPHYRUM);

    /* revocatio una: stratum tollitur */
    CREDO_VERUM(pictor_documentum_revocare(doc));
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_stratorum(doc), I);
}

interior vacuum
_sectio_confirmare_atkinson (
    CredoContextus* c)
{
             Volumen* vol;
    PictorDocumentum* doc  = _documentum(c, &vol);
               Imago  im   = _imago_ficta(c->piscina);
               Imago  praevisio;
                 s32  x0;
                 s32  y0;

    CREDO_NECESSE_NON_NIHIL(doc);
    CREDO_NECESSE(pictor_documentum_glutinationem_parare(doc, &im));
    praevisio  = doc->glutinatio.praevisio[PICTOR_GLUTINATIO_ATKINSON];
    x0         = doc->glutinatio.x;
    y0         = doc->glutinatio.y;
    CREDO_AEQUALIS_S32(pictor_documentum_glutinationem_confirmare(doc,
        PICTOR_GLUTINATIO_ATKINSON, I), II);
    CREDO_VERUM(_stratum_praevisioni_aequale(doc, I, &praevisio, x0,
        y0));
}

interior vacuum
_sectio_abicere (
    CredoContextus* c)
{
             Volumen* vol;
    PictorDocumentum* doc  = _documentum(c, &vol);
               Imago  im   = _imago_ficta(c->piscina);
                 s64  finis;

    CREDO_NECESSE_NON_NIHIL(doc);
    finis = pictor_documentum_finis(doc);
    CREDO_NECESSE(pictor_documentum_glutinationem_parare(doc, &im));
    pictor_documentum_glutinationem_abicere(doc);
    CREDO_FALSUM(doc->glutinatio.pendens);
    CREDO_VERUM(pictor_documentum_finis(doc) == finis);
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_stratorum(doc), I);
    CREDO_AEQUALIS_S32(pictor_documentum_glutinationem_confirmare(doc,
        PICTOR_GLUTINATIO_ATKINSON, I), ZEPHYRUM);
    /* abicere sine pendente: nihil */
    pictor_documentum_glutinationem_abicere(doc);
    CREDO_FALSUM(doc->glutinatio.pendens);
}

interior vacuum
_sectio_iterum (
    CredoContextus* c)
{
             Volumen* vol;
    PictorDocumentum* doc  = _documentum(c, &vol);
               Imago  im   = _imago_ficta(c->piscina);
               Imago  alta;
               Imago  vacua;

    CREDO_NECESSE_NON_NIHIL(doc);
    CREDO_NECESSE(pictor_documentum_glutinationem_parare(doc, &im));
    CREDO_AEQUALIS_I32(doc->glutinatio.latitudo, LATUS_DOC);

    /* imago alta (L x C): altitudo plena - prior abicitur */
    alta.latitudo  = L;
    alta.altitudo  = C;
    alta.pixela    = im.pixela;   /* eadem octeti, forma alia */
    CREDO_NECESSE(pictor_documentum_glutinationem_parare(doc, &alta));
    CREDO_AEQUALIS_I32(doc->glutinatio.altitudo, LATUS_DOC);
    CREDO_AEQUALIS_I32(doc->glutinatio.latitudo, LATUS_DOC * L / C);

    /* fons vacua: FALSUM, pendens prior manet intactus */
    vacua.latitudo  = ZEPHYRUM;
    vacua.altitudo  = ZEPHYRUM;
    vacua.pixela    = NIHIL;
    CREDO_FALSUM(pictor_documentum_glutinationem_parare(doc, &vacua));
    CREDO_VERUM(doc->glutinatio.pendens);
    CREDO_AEQUALIS_I32(doc->glutinatio.altitudo, LATUS_DOC);
    CREDO_FALSUM(pictor_documentum_glutinationem_parare(NIHIL, &im));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "parare", _sectio_parare, _parare, _purgare, NIHIL, NIHIL },
    { "confirmare exemplaria", _sectio_confirmare, _parare, _purgare,
        NIHIL, NIHIL },
    { "confirmare atkinson", _sectio_confirmare_atkinson, _parare,
        _purgare, NIHIL, NIHIL },
    { "abicere", _sectio_abicere, _parare, _purgare, NIHIL, NIHIL },
    { "parare iterum", _sectio_iterum, _parare, _purgare, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    thema_initiare();
    redde credo_suitam_currere("pictor_glutinatio", SECTIONES);
}

/* probatio_pictor_glutinatio.c - glutinatio pendens in documento
 * (Franus 2026-10-13)
 *
 * parare: imago aptata et BIS ditherata (A Atkinson = idem ac
 * pictor_imaginem_aptare; B exemplaria, diversum), acta intacta,
 * praevisio RGBA = pixela strati futuri, sigilla diversa. confirmare:
 * stratum novum modi electi, revocatio una, reapertio eadem. abicere:
 * nihil in actis. parare iterum: prior abicitur; fons vacua: nihil
 * mutatur. radere (Franus 2026-10-13): larva "alterum hic" - discus
 * per segmentum alterum modum in praevisione UTRAQUE revelat (A/B
 * mutato larva manet: vice versa), restituere reponit, acta intacta,
 * extra glutinationem nihil; confirmare per pixelum miscet.
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
#include "insula.h"
#include "motus.h"
#include "componens.h"
#include "mandatum.h"
#include "figura.h"
#include "delineare_mandata.h"
#include "pictor_componentia.h"
#include "pictor_figurae.h"
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

/* compositio + figurae + rasterizatio: praevisio modi electi pingitur
 * ad locum suum, fons imaginum eam reddit, linea status A / B / OK / X */
interior vacuum
_sectio_figura (
    CredoContextus* c)
{
                 Volumen* vol;
        PictorDocumentum* doc  = _documentum(c, &vol);
                   Imago  im   = _imago_ficta(c->piscina);
     InternamentumChorda* in   = internamentum_creare(c->piscina);
      InsulaRepositorium* repo;
                   Motus  motus;
        PictorCompositio  cfg;
           PictorFigurae  pf;
         FiguraRegistrum* reg;
                 Mandata* m;
               Componens* arbor;
         TabulaPixelorum* t;
                     i32  n;
                     i32  i;
                     i32  imagines      = ZEPHYRUM;
                     i32  praevisiones  = ZEPHYRUM;
                     i32  litterae      = ZEPHYRUM;
                     i32  k;
                     s32  px = -I;
                     s32  py = -I;

    CREDO_NECESSE_NON_NIHIL(doc);
    CREDO_NECESSE(pictor_documentum_glutinationem_parare(doc, &im));
    repo = insula_repositorium_creare(c->piscina, in,
        "<pictor latitudo=\"480\" altitudo=\"480\""
        " instrumentum=\"penicillus\" zoom=\"1\" glutinatio=\"B\"/>");
    motus_initiare(&motus, c->piscina);
    memset(&cfg, ZEPHYRUM, magnitudo(cfg));
    cfg.fenestra_latitudo = D;
    cfg.fenestra_altitudo = D + LX;
    cfg.cellula_latitudo = VI;
    cfg.cellula_altitudo = VIII;
    cfg.status_lineae = IV;
    cfg.doc = doc;
    arbor = pictor_componere(repo, &motus, c->piscina, in, &cfg);
    CREDO_NECESSE_NON_NIHIL(arbor);

    pf.doc               = doc;
    pf.cellula_latitudo  = VI;
    pf.cellula_altitudo  = VIII;
    reg                  = figura_registrum_creare(c->piscina);
    pictor_figurae_registrare(reg, ZEPHYRUM, &pf);
    m = mandata_creare(c->piscina, in);
    pingere(arbor, reg, ZEPHYRUM, m);
    n = mandata_numerus(m);
    per (i = ZEPHYRUM; i < n; i++)
    {
        Mandatum* x = mandata_obtinere(m, i);
        si (x->genus == MANDATUM_IMAGO)
        {
            imagines++;
            si (   x->textus.mensura > XI
                && memcmp(x->textus.datum, "glutinatio:", XI)
                    == ZEPHYRUM)
            {
                praevisiones++;
                CREDO_VERUM(pictor_imago_fons(x->textus, &pf)
                    == &doc->glutinatio.praevisio[PICTOR_GLUTINATIO_EXEMPLARIA]);
                CREDO_AEQUALIS_S32(x->fines.latitudo,
                    (s32)doc->glutinatio.latitudo);
                CREDO_AEQUALIS_S32(x->fines.altitudo,
                    (s32)doc->glutinatio.altitudo);
            }
        }
        si (   x->genus == MANDATUM_TEXTUS
            && (chorda_aequalis_literis(x->textus, "A")
                || chorda_aequalis_literis(x->textus, "B")
                || chorda_aequalis_literis(x->textus, "OK")
                || chorda_aequalis_literis(x->textus, "X")))
        {
            litterae++;
        }
    }
    CREDO_AEQUALIS_I32(imagines, II);
    CREDO_AEQUALIS_I32(praevisiones, I);
    CREDO_AEQUALIS_I32(litterae, IV);

    /* rasterizatio: pixelum non perspicuum praevisionis in tabula
     * (ad originem tabulae VI, VIII) colorem praevisionis habet */
    per (k = ZEPHYRUM; k
        < doc->glutinatio.latitudo * doc->glutinatio.altitudo; k++)
    {
        si (doc->glutinatio.indices[PICTOR_GLUTINATIO_EXEMPLARIA][k]
            < XVI)
        {
            px = (s32)(k % doc->glutinatio.latitudo);
            py = (s32)(k / doc->glutinatio.latitudo);
            frange;
        }
    }
    CREDO_NECESSE(px >= ZEPHYRUM);
    t = tabula_pixelorum_creare_nuda(c->piscina, D, D + LX);
    tabula_pixelorum_vacare(t, ZEPHYRUM);
    delineare_mandata(m, t, pictor_imago_fons, &pf);
    CREDO_AEQUALIS_I32((i32)tabula_pixelorum_obtinere_pixelum(t,
        (i32)(VI + doc->glutinatio.x + px), (i32)(VIII
            + doc->glutinatio.y + py)),
        ((constans i32*)(constans vacuum*)
            doc->glutinatio.praevisio[PICTOR_GLUTINATIO_EXEMPLARIA].pixela)
            [py * (s32)doc->glutinatio.latitudo + px]);
    (vacuum)vol;
}

/* pixelum praevisionis ex indice (0xFF -> 0 perspicuum) */
interior i32
_pixelum_indicis (
    i8 index)
{
    redde index >= XVI ? ZEPHYRUM
        : (i32)color_ad_pixelum(color_ex_palette((i32)index));
}

interior i32
_praevisio_pixelum (
    constans PictorGlutinatio* g,
                          i32  modus,
                          i32  k)
{
    redde ((constans i32*)(constans vacuum*)g->praevisio[modus].pixela)
        [k];
}

/* index k glutinationis (non perspicuum) ubi A et B different, longe
 * a marginibus (discus XI totus intus) */
interior s32
_k_diversum (
    constans PictorGlutinatio* g)
{
    i32 k;
    i32 x;
    i32 y;

    per (k = ZEPHYRUM; k < g->latitudo * g->altitudo; k++)
    {
        x = k % g->latitudo;
        y = k / g->latitudo;
        si (   x >= XX && y >= XX && x + XX < g->latitudo
            && y + XX < g->altitudo
            && g->indices[PICTOR_GLUTINATIO_ATKINSON][k] < XVI
            && g->indices[PICTOR_GLUTINATIO_ATKINSON][k]
                != g->indices[PICTOR_GLUTINATIO_EXEMPLARIA][k])
        {
            redde (s32)k;
        }
    }
    redde -I;
}

interior vacuum
_sectio_radere (
    CredoContextus* c)
{
             Volumen* vol;
    PictorDocumentum* doc  = _documentum(c, &vol);
               Imago  im   = _imago_ficta(c->piscina);
    PictorGlutinatio* g;
                 s32  k;
                 s32  cx;
                 s32  cy;
                 s32  dx;
                 s32  dy;
                 s64  finis;
                 i32  revelata;
                 i32  mala;
                 i32  j;

    CREDO_NECESSE_NON_NIHIL(doc);
    /* sine pendente: nihil */
    CREDO_FALSUM(pictor_documentum_glutinationem_radere(doc, C, C, C, C,
        V, FALSUM));
    CREDO_NECESSE(pictor_documentum_glutinationem_parare(doc, &im));
    g      = &doc->glutinatio;
    finis  = pictor_documentum_finis(doc);
    CREDO_NECESSE_NON_NIHIL(g->larva);
    k = _k_diversum(g);
    CREDO_NECESSE(k >= ZEPHYRUM);
    cx = g->x + k % (s32)g->latitudo;
    cy = g->y + k / (s32)g->latitudo;

    /* discus XI in puncto uno: centrum revelat alterum in UTRAQUE */
    CREDO_VERUM(pictor_documentum_glutinationem_radere(doc, cx, cy, cx,
        cy, XI, FALSUM));
    CREDO_AEQUALIS_I32((i32)g->larva[k], I);
    CREDO_AEQUALIS_I32(_praevisio_pixelum(g,
        PICTOR_GLUTINATIO_EXEMPLARIA,
        (i32)k),
        _pixelum_indicis(g->indices[PICTOR_GLUTINATIO_ATKINSON][k]));
    CREDO_AEQUALIS_I32(_praevisio_pixelum(g, PICTOR_GLUTINATIO_ATKINSON,
        (i32)k),
        _pixelum_indicis(g->indices[PICTOR_GLUTINATIO_EXEMPLARIA][k]));
    /* acta intacta */
    CREDO_VERUM(pictor_documentum_finis(doc) == finis);
    /* forma: pixela larvae = discus XI exacte (pictor_disci_pixelum) */
    revelata  = ZEPHYRUM;
    mala      = ZEPHYRUM;
    per (j = ZEPHYRUM; j < g->latitudo * g->altitudo; j++)
    {
        dx = (s32)(j % g->latitudo) + g->x - (cx - XI / II);
        dy = (s32)(j / g->latitudo) + g->y - (cy - XI / II);
        si (g->larva[j])
        {
            revelata++;
        }
        si ((g->larva[j] != ZEPHYRUM) != pictor_disci_pixelum(XI, dx,
            dy))
        {
            mala++;
        }
        /* praevisio B ubique = larva ? A : B */
        si (_praevisio_pixelum(g, PICTOR_GLUTINATIO_EXEMPLARIA, j)
            != _pixelum_indicis(g->larva[j]
                ? g->indices[PICTOR_GLUTINATIO_ATKINSON][j]
                : g->indices[PICTOR_GLUTINATIO_EXEMPLARIA][j]))
        {
            mala++;
        }
    }
    CREDO_AEQUALIS_I32(mala, ZEPHYRUM);
    CREDO_VERUM(revelata > LXXX && revelata < CXXI);
    /* iterum idem: nihil novi */
    CREDO_FALSUM(pictor_documentum_glutinationem_radere(doc, cx, cy, cx,
        cy, XI, FALSUM));

    /* restituere: reponit */
    CREDO_VERUM(pictor_documentum_glutinationem_radere(doc, cx, cy, cx,
        cy, XI, VERUM));
    mala = ZEPHYRUM;
    per (j = ZEPHYRUM; j < g->latitudo * g->altitudo; j++)
    {
        si (   g->larva[j]
            || _praevisio_pixelum(g, PICTOR_GLUTINATIO_EXEMPLARIA, j)
                != _pixelum_indicis(
                    g->indices[PICTOR_GLUTINATIO_EXEMPLARIA][j]))
        {
            mala++;
        }
    }
    CREDO_AEQUALIS_I32(mala, ZEPHYRUM);

    /* segmentum: discus I per lineam - pixela omnia lineae */
    CREDO_VERUM(pictor_documentum_glutinationem_radere(doc, cx - X, cy,
        cx + X, cy, I, FALSUM));
    revelata = ZEPHYRUM;
    per (j = ZEPHYRUM; j < g->latitudo * g->altitudo; j++)
    {
        si (g->larva[j])
        {
            revelata++;
            CREDO_AEQUALIS_S32((s32)(j / g->latitudo) + g->y, cy);
        }
    }
    CREDO_AEQUALIS_I32(revelata, XXI);
    CREDO_VERUM(pictor_documentum_finis(doc) == finis);
}

/* extra glutinationem nihil; discus in margine praeciditur */
interior vacuum
_sectio_radere_margines (
    CredoContextus* c)
{
             Volumen* vol;
    PictorDocumentum* doc  = _documentum(c, &vol);
               Imago  im   = _imago_ficta(c->piscina);
    PictorGlutinatio* g;
                 i32  revelata;
                 i32  j;

    CREDO_NECESSE_NON_NIHIL(doc);
    CREDO_NECESSE(pictor_documentum_glutinationem_parare(doc, &im));
    g = &doc->glutinatio;
    /* imago C x L in CDLXXX: y = CXX, supra eam nihil */
    CREDO_NECESSE(g->y > XX);
    CREDO_FALSUM(pictor_documentum_glutinationem_radere(doc, C, V, CC,
        V,
        IX, FALSUM));
    CREDO_FALSUM(pictor_documentum_glutinationem_radere(doc, -L, -L, -L,
        -L, IX, FALSUM));
    /* centrum in margine summo: dimidium disci intus */
    CREDO_VERUM(pictor_documentum_glutinationem_radere(doc, C, g->y, C,
        g->y, IX, FALSUM));
    revelata = ZEPHYRUM;
    per (j = ZEPHYRUM; j < g->latitudo * g->altitudo; j++)
    {
        si (g->larva[j])
        {
            revelata++;
            CREDO_VERUM((s32)(j / g->latitudo) <= IV);
        }
    }
    CREDO_VERUM(revelata > XX && revelata < L);
}

/* confirmare per pixelum: larva ? alter : electus - pro B et A */
interior vacuum
_sectio_confirmare_larva (
    CredoContextus* c)
{
    i32 modus;

    per (modus = ZEPHYRUM; modus < (i32)PICTOR_GLUTINATIO_MODI; modus++)
    {
                       Volumen* vol;
              PictorDocumentum* doc;
                         Imago  im   = _imago_ficta(c->piscina);
              PictorGlutinatio  g;
                           s32  k;
                           s32  cx;
                           s32  cy;
        constans PictorStratum* s;
                           i32  j;
                           i32  mala;

        _purgare(c);
        _parare(c);
        doc = _documentum(c, &vol);
        CREDO_NECESSE_NON_NIHIL(doc);
        CREDO_NECESSE(pictor_documentum_glutinationem_parare(doc, &im));
        k = _k_diversum(&doc->glutinatio);
        CREDO_NECESSE(k >= ZEPHYRUM);
        cx = doc->glutinatio.x + k % (s32)doc->glutinatio.latitudo;
        cy = doc->glutinatio.y + k / (s32)doc->glutinatio.latitudo;
        CREDO_VERUM(pictor_documentum_glutinationem_radere(doc, cx - XX,
            cy, cx + XX, cy + V, VII, FALSUM));
        g = doc->glutinatio;   /* indices et larva in piscina manent */
        CREDO_AEQUALIS_S32(pictor_documentum_glutinationem_confirmare(doc,
            (PictorGlutinatioModus)modus, I), II);
        s = pictor_documentum_stratum(doc, I);
        CREDO_NECESSE_NON_NIHIL(s);
        mala = ZEPHYRUM;
        per (j = ZEPHYRUM; j < g.latitudo * g.altitudo; j++)
        {
            i8 index = g.larva[j]
                ? g.indices[(PICTOR_GLUTINATIO_MODI - I)
                    - (i32)modus][j]
                : g.indices[modus][j];
            si ((i32)tabula_pixelorum_obtinere_pixelum(s->pixela,
                    (i32)(g.x + (s32)(j % g.latitudo)),
                    (i32)(g.y + (s32)(j / g.latitudo)))
                != _pixelum_indicis(index))
            {
                mala++;
            }
        }
        CREDO_AEQUALIS_I32(mala, ZEPHYRUM);
        CREDO_VERUM(g.larva[k] != ZEPHYRUM);
    }
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
    { "praevisio in figura", _sectio_figura, _parare, _purgare, NIHIL,
        NIHIL },
    { "radere", _sectio_radere, _parare, _purgare, NIHIL, NIHIL },
    { "radere margines", _sectio_radere_margines, _parare, _purgare,
        NIHIL, NIHIL },
    { "confirmare larva", _sectio_confirmare_larva, _parare, _purgare,
        NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    thema_initiare();
    redde credo_suitam_currere("pictor_glutinatio", SECTIONES);
}

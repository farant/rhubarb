/* lector_paginae.c - caput in paginas (include/lector_paginae.h) */
#include "lector_paginae.h"
#include "fons.h"
#include "utf8.h"

#define INTERVALLUM  VIII   /* px inter clausulas */
#define INDENTATIO   II     /* cellulae continuationis versus */
/* px vacui post lineam textus (FONS_PARVUS): spatium duplex (Franus
 * 2026-10-24) */
#define INTERVALLUM_LINEARUM  VIII

/* dispositor: paginae, pagina currens (index), y in ea */
nomen structura {
         Xar* paginae;
         i32  currens;
         s32  y;
         s32  altitudo;
     Piscina* piscina;
} Dispositor;

interior b32
paginam_novam (
    Dispositor* d)
{
    LectorPagina* g;

    g = (LectorPagina*)xar_addere(d->paginae);
    si (!g)
    {
        redde FALSUM;
    }
    g->lineae   = xar_creare(d->piscina, (i32)magnitudo(LectorLinea));
    g->initium  = ZEPHYRUM;
    d->currens  = xar_numerus(d->paginae) - I;
    d->y        = ZEPHYRUM;
    redde g->lineae != NIHIL;
}

/* lineam in pagina currente ponere; non capit (et pagina non vacua)
 * -> pagina nova. Intervallum solum si pagina iam lineas habet. */
interior b32
lineam_ponere (
    Dispositor* d,
           i32  initium,
           i32  finis,
           i32  fons,
           s32  x,
           s32  intervallum)
{
    LectorPagina* g;
     LectorLinea* l;
             s32  altitudo_lineae;
             s32  passus;

    /* altitudo visibilis capere debet; passus (cum linea vacua spatii
     * duplicis) solum ad lineam sequentem - vacua post ultimam paginae
     * non requiritur */
    altitudo_lineae  = fons_altitudo(fons);
    passus           = altitudo_lineae + (fons == FONS_PARVUS
        ? INTERVALLUM_LINEARUM : ZEPHYRUM);
    g                = (LectorPagina*)xar_obtinere(d->paginae,
        d->currens);
    si (xar_numerus(g->lineae) == ZEPHYRUM)
    {
        intervallum = ZEPHYRUM;
    }
    alioquin si (d->y + intervallum + altitudo_lineae > d->altitudo)
    {
        si (!paginam_novam(d))
        {
            redde FALSUM;
        }
        g            = (LectorPagina*)xar_obtinere(d->paginae,
            d->currens);
        intervallum  = ZEPHYRUM;
    }
    d->y += intervallum;
    si (xar_numerus(g->lineae) == ZEPHYRUM)
    {
        g->initium = initium;
    }
    l = (LectorLinea*)xar_addere(g->lineae);
    si (!l)
    {
        redde FALSUM;
    }
    l->initium  = initium;
    l->finis    = finis;
    l->fons     = fons;
    l->x        = x;
    l->y        = d->y;
    d->y        += passus;
    redde VERUM;
}

/* octetus post runam ad i (semper progreditur) */
interior i32
runam_praeterire (
    chorda textus,
       i32 i,
       i32 finis)
{
    constans i8* q;

    q = textus.datum + i;
    (vacuum)utf8_decodere(&q, textus.datum + finis);
    si (q <= textus.datum + i)
    {
        redde i + I;
    }
    redde (i32)(q - textus.datum);
}

/* segmentum [a, b) involvere: verba ad spatia ASCII, verbum longum ad
 * runam secatur; continuatio indentata (cellulae indentationis);
 * *intervallum ante lineam primam, deinde nihil */
interior b32
segmentum_involvere (
    Dispositor* d,
        chorda  textus,
           i32  a,
           i32  b,
           i32  fons,
           s32  cellulae,
           s32  indentatio,
           s32* intervallum)
{
    i32 positio;
    i32 finis;
    i32 linea_finis;
    i32 ultimum;
    s32 spatium;
    s32 n;
    s32 k;
    b32 prima;

    positio  = a;
    prima    = VERUM;
    dum (VERUM)
    {
        dum (positio < b && textus.datum[positio] == ' ')
        {
            positio++;
        }
        si (positio >= b)
        {
            frange;
        }
        n = cellulae - (prima ? ZEPHYRUM : indentatio);
        si (n < I)
        {
            n = I;
        }
        finis    = positio;
        k        = ZEPHYRUM;
        spatium  = -I;
        dum (finis < b && k < n)
        {
            si (textus.datum[finis] == ' ')
            {
                spatium = (s32)finis;
            }
            finis = runam_praeterire(textus, finis, b);
            k++;
        }
        si (finis >= b || textus.datum[finis] == ' ')
        {
            linea_finis = finis >= b ? b : finis;
        }
        alioquin si (spatium > (s32)positio)
        {
            linea_finis = (i32)spatium;
        }
        alioquin
        {
            linea_finis = finis;   /* verbum linea longius */
        }
        ultimum = linea_finis;
        dum (ultimum > positio && textus.datum[ultimum - I] == ' ')
        {
            ultimum--;
        }
        si (!lineam_ponere(d, positio, ultimum, fons, prima ? ZEPHYRUM
                : indentatio * fons_latitudo(fons), *intervallum))
        {
            redde FALSUM;
        }
        *intervallum  = ZEPHYRUM;
        prima         = FALSUM;
        positio       = linea_finis;
    }
    redde VERUM;
}

Xar*
lector_paginare (
    constans CodexCaput* caput,
                    i32  latitudo,
                    i32  altitudo,
                Piscina* piscina)
{
                Dispositor  d;
    constans CodexClausula* b;
                       i32  i;
                       i32  a;
                       i32  j;
                       i32  fons;
                       s32  cellulae;
                       s32  intervallum;

    si (!caput || !piscina)
    {
        redde NIHIL;
    }
    d.paginae   = xar_creare(piscina, (i32)magnitudo(LectorPagina));
    d.altitudo  = (s32)altitudo;
    d.piscina   = piscina;
    si (!d.paginae || !paginam_novam(&d))
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; caput->clausulae
        && i < xar_numerus(caput->clausulae); i++)
    {
        b = (constans CodexClausula*)xar_obtinere(caput->clausulae, i);
        si (b->finis > caput->textus.mensura || b->initium > b->finis)
        {
            perge;
        }
        fons = b->genus == CODEX_TITULUS ? FONS_TITULI : FONS_PARVUS;
        cellulae = (s32)latitudo / fons_latitudo(fons);
        si (cellulae < I)
        {
            cellulae = I;
        }
        intervallum = INTERVALLUM;
        si (b->genus == CODEX_PARAGRAPHUS)
        {
            si (!segmentum_involvere(&d, caput->textus, b->initium,
                    b->finis, fons, cellulae, ZEPHYRUM, &intervallum))
            {
                redde NIHIL;
            }
            perge;
        }
        a = b->initium;
        per (j = b->initium; j <= b->finis; j++)
        {
            si (j == b->finis || caput->textus.datum[j] == '\n')
            {
                si (!segmentum_involvere(&d, caput->textus, a, j, fons,
                        cellulae, b->genus == CODEX_VERSUS ? INDENTATIO
                        : ZEPHYRUM, &intervallum))
                {
                    redde NIHIL;
                }
                a = j + I;
            }
        }
    }
    redde d.paginae;
}

i32
lector_pagina_positionis (
    Xar* paginae,
    i32  positio)
{
    i32 k;
    i32 i;

    k = ZEPHYRUM;
    per (i = I; paginae && i < xar_numerus(paginae); i++)
    {
        si (((constans LectorPagina*)xar_obtinere(paginae,
                i))->initium > positio)
        {
            frange;
        }
        k = i;
    }
    redde k;
}

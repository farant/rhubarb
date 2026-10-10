/* iussum.c - iussa in textu (vicus-latera S3a). Ratio in capite. */

#include "iussum.h"
#include "xar.h"

#include <string.h>


/* ==================================================
 * Auxilia
 * ================================================== */

/* cellula (coordinatae signatae, iam intra tabulam probatae) */
interior character
cellula (
    constans TabulaCharacterum* t,
                           s32  linea,
                           s32  columna)
{
    redde t->cellulae[(i32)linea * t->latitudo + (i32)columna];
}

interior b32
verbi_character (
    character c)
{
    redde (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
        || (c >= '0' && c <= '9') || c == '_';
}

interior b32
verbi_initium (
    character c)
{
    redde c >= 'a' && c <= 'z';
}

/* S3e: '-' intra verbum (non in fine - vide legere) */
interior b32
verbi_sequens (
    character c)
{
    redde (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_'
        || c == '-';
}

/* cellula vacua ('\0') ut spatium */
interior b32
spatium_est (
    character c)
{
    redde c == ' ' || c == '\0' || c == '\t';
}

/* cellulae [a, b) lineae, spatiis extremis demptis, in piscinam */
interior chorda
segmentum (
    constans TabulaCharacterum* t,
                           s32  linea,
                           s32  a,
                           s32  b,
                       Piscina* piscina)
{
    chorda c;

    dum (a < b && spatium_est(cellula(t, linea, a)))
    {
        a++;
    }
    dum (b > a && spatium_est(cellula(t, linea, b - I)))
    {
        b--;
    }
    c.mensura  = (i32)(b - a);
    c.datum    = NIHIL;
    si (c.mensura > ZEPHYRUM)
    {
        c.datum = (i8*)piscina_allocare(piscina,
            (memoriae_index)c.mensura);
        memcpy(c.datum, t->cellulae + (i32)linea * t->latitudo + (i32)a,
            (memoriae_index)c.mensura);
    }
    redde c;
}

/* iussum cuius '$' in columna c legere; FALSUM si non iussum */
interior b32
legere (
    constans TabulaCharacterum* t,
                           s32  linea,
                           s32  c,
                   IussumNotum  notum,
                        vacuum* ctx,
                       Piscina* piscina,
                        Iussum* exitus)
{
    s32 latitudo;
    s32 j;
    s32 k;
    s32 a;
    i32 n;
    i32 i;

    latitudo = (s32)t->latitudo;
    si (cellula(t, linea, c) != '$')
    {
        redde FALSUM;
    }
    si (c > ZEPHYRUM && verbi_character(cellula(t, linea, c - I)))
    {
        redde FALSUM;
    }
    j = c + I;
    si (j >= latitudo || !verbi_initium(cellula(t, linea, j)))
    {
        redde FALSUM;
    }
    dum (j < latitudo && verbi_sequens(cellula(t, linea, j)))
    {
        j++;
    }
    /* '-' finale verbum non est: "$dies-" = $dies et '-' */
    dum (cellula(t, linea, j - I) == '-')
    {
        j--;
    }
    exitus->linea    = linea;
    exitus->initium  = c;
    exitus->finis    = j;
    exitus->verbum = segmentum(t, linea, c + I, j,
        piscina);
    exitus->numerus_argumentorum  = ZEPHYRUM;
    exitus->argumenta             = NIHIL;
    si (notum && !notum(exitus->verbum, ctx))
    {
        redde FALSUM;
    }
    si (j >= latitudo || cellula(t, linea, j) != '(')
    {
        redde VERUM;
    }
    /* argumenta: ')' prima in eadem linea, aliter non iussum */
    k = j + I;
    dum (k < latitudo && cellula(t, linea, k) != ')')
    {
        k++;
    }
    si (k >= latitudo)
    {
        redde FALSUM;
    }
    exitus->finis = k + I;
    si (segmentum(t, linea, j + I, k, piscina).mensura == ZEPHYRUM)
    {
        redde VERUM;
    }
    n = I;
    per (a = j + I; a < k; a++)
    {
        si (cellula(t, linea, a) == ',')
        {
            n++;
        }
    }
    exitus->argumenta = (chorda*)piscina_allocare(piscina,
        (memoriae_index)n * magnitudo(chorda));
    exitus->numerus_argumentorum  = n;
    a                             = j + I;
    per (i = ZEPHYRUM; i < n; i++)
    {
        s32 b;

        b = a;
        dum (b < k && cellula(t, linea, b) != ',')
        {
            b++;
        }
        exitus->argumenta[i]  = segmentum(t, linea, a, b, piscina);
        a                     = b + I;
    }
    redde VERUM;
}


/* ==================================================
 * Interfacies
 * ================================================== */

b32
iussum_proximum (
    constans TabulaCharacterum* t,
                           s32  linea,
                           s32  a_columna,
                   IussumNotum  notum,
                        vacuum* ctx,
                       Piscina* piscina,
                        Iussum* exitus)
{
    s32 c;

    si (   !t || !piscina || !exitus || linea < ZEPHYRUM
        || linea >= (s32)t->altitudo)
    {
        redde FALSUM;
    }
    per (c = a_columna < ZEPHYRUM ? ZEPHYRUM : a_columna;
         c < (s32)t->latitudo; c++)
    {
        si (legere(t, linea, c, notum, ctx, piscina, exitus))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

b32
iussum_ad_locum (
    constans TabulaCharacterum* t,
                           s32  linea,
                           s32  columna,
                   IussumNotum  notum,
                        vacuum* ctx,
                       Piscina* piscina,
                        Iussum* exitus)
{
    s32 a;

    si (!t || columna < ZEPHYRUM || columna >= (s32)t->latitudo)
    {
        redde FALSUM;
    }
    a = ZEPHYRUM;
    dum (iussum_proximum(t, linea, a, notum, ctx, piscina, exitus))
    {
        si (exitus->initium > columna)
        {
            redde FALSUM;
        }
        si (columna < exitus->finis)
        {
            redde VERUM;
        }
        a = exitus->finis;
    }
    redde FALSUM;
}


/* ==================================================
 * Nexus (S3d)
 * ================================================== */

interior b32
nexus_character (
    character c)
{
    redde (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_'
        || c == '-';
}

/* nexus cuius '#' in columna c legere */
interior b32
nexum_legere (
    constans TabulaCharacterum* t,
                           s32  linea,
                           s32  c,
                       Piscina* piscina,
                        Iussum* exitus)
{
    s32 j;

    si (cellula(t, linea, c) != '#')
    {
        redde FALSUM;
    }
    si (c > ZEPHYRUM && verbi_character(cellula(t, linea, c - I)))
    {
        redde FALSUM;
    }
    j = c + I;
    dum (j < (s32)t->latitudo && nexus_character(cellula(t, linea, j)))
    {
        j++;
    }
    si (j == c + I)
    {
        redde FALSUM;
    }
    exitus->linea    = linea;
    exitus->initium  = c;
    exitus->finis    = j;
    exitus->verbum = segmentum(t, linea, c + I, j,
        piscina);
    exitus->numerus_argumentorum  = ZEPHYRUM;
    exitus->argumenta             = NIHIL;
    redde VERUM;
}

b32
iussum_nexus_proximus (
    constans TabulaCharacterum* t,
                           s32  linea,
                           s32  a_columna,
                       Piscina* piscina,
                        Iussum* exitus)
{
    s32 c;

    si (   !t || !piscina || !exitus || linea < ZEPHYRUM
        || linea >= (s32)t->altitudo)
    {
        redde FALSUM;
    }
    per (c = a_columna < ZEPHYRUM ? ZEPHYRUM : a_columna;
         c < (s32)t->latitudo; c++)
    {
        si (nexum_legere(t, linea, c, piscina, exitus))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

b32
iussum_nexus_ad_locum (
    constans TabulaCharacterum* t,
                           s32  linea,
                           s32  columna,
                       Piscina* piscina,
                        Iussum* exitus)
{
    s32 a;

    si (!t || columna < ZEPHYRUM || columna >= (s32)t->latitudo)
    {
        redde FALSUM;
    }
    a = ZEPHYRUM;
    dum (iussum_nexus_proximus(t, linea, a, piscina, exitus))
    {
        si (exitus->initium > columna)
        {
            redde FALSUM;
        }
        si (columna < exitus->finis)
        {
            redde VERUM;
        }
        a = exitus->finis;
    }
    redde FALSUM;
}


/* ==================================================
 * Registrum
 * ================================================== */

nomen structura {
           chorda  verbum;
              b32  consumit;
    IussumFunctio  functio;
           vacuum* ctx;
} VerbumRegistratum;

structura IussumRegistrum {
    Piscina* piscina;
        Xar* verba;   /* VerbumRegistratum */
};

interior VerbumRegistratum*
invenire (
    constans IussumRegistrum* r,
                      chorda  verbum)
{
    VerbumRegistratum* v;
                  i32  i;

    si (!r)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < xar_numerus(r->verba); i++)
    {
        v = (VerbumRegistratum*)xar_obtinere(r->verba, i);
        si (chorda_aequalis(v->verbum, verbum))
        {
            redde v;
        }
    }
    redde NIHIL;
}

IussumRegistrum*
iussum_registrum_creare (
    Piscina* piscina)
{
    IussumRegistrum* r;

    si (!piscina)
    {
        redde NIHIL;
    }
    r = (IussumRegistrum*)piscina_allocare(piscina,
        magnitudo(IussumRegistrum));
    r->piscina = piscina;
    r->verba = xar_creare(piscina,
        (i32)magnitudo(VerbumRegistratum));
    redde r;
}

b32
iussum_registrare (
        IussumRegistrum* r,
     constans character* verbum,
                    b32  consumit,
          IussumFunctio  functio,
                 vacuum* ctx)
{
    VerbumRegistratum* v;
               chorda  c;
                  i32  i;

    si (!r || !verbum || !functio)
    {
        redde FALSUM;
    }
    c = chorda_ex_literis(verbum, r->piscina);
    /* forma eadem ac in textu: '[a-z][a-z0-9_-]*', '-' non finale */
    si (   c.mensura              == ZEPHYRUM
        || !verbi_initium((character)c.datum[ZEPHYRUM])
        || c.datum[c.mensura - I] == '-')
    {
        redde FALSUM;
    }
    per (i = I; i < c.mensura; i++)
    {
        si (!verbi_sequens((character)c.datum[i]))
        {
            redde FALSUM;
        }
    }
    v = invenire(r, c);
    si (!v)
    {
        v = (VerbumRegistratum*)xar_addere(r->verba);
        si (!v)
        {
            redde FALSUM;
        }
    }
    v->verbum    = c;
    v->consumit  = consumit;
    v->functio   = functio;
    v->ctx       = ctx;
    redde VERUM;
}

b32
iussum_registrum_notum (
    chorda  verbum,
    vacuum* ctx)
{
    redde invenire((constans IussumRegistrum*)ctx, verbum) != NIHIL;
}

b32
iussum_consumit (
    constans IussumRegistrum* r,
                      chorda  verbum)
{
    VerbumRegistratum* v;

    v = invenire(r, verbum);
    redde v ? v->consumit : FALSUM;
}

b32
iussum_currere (
       IussumRegistrum* r,
       constans Iussum* iussum,
               Piscina* piscina,
        IussumEffectus* effectus)
{
    VerbumRegistratum* v;

    si (!iussum || !piscina || !effectus)
    {
        redde FALSUM;
    }
    memset(effectus, ZEPHYRUM, magnitudo(IussumEffectus));
    v = invenire(r, iussum->verbum);
    si (!v)
    {
        redde FALSUM;
    }
    redde v->functio(iussum, v->ctx, piscina, effectus);
}

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

/* corpus: '</>' primum a (linea, columna) - eadem linea aut
 * sequentibus; VERUM et locus '<' */
interior b32
clausuram_invenire (
    constans TabulaCharacterum* t,
                           s32  linea,
                           s32  columna,
                           s32* linea_inventa,
                           s32* columna_inventa)
{
    s32 l;
    s32 c;

    per (l = linea; l < (s32)t->altitudo; l++)
    {
        per (c = (l == linea ? columna : ZEPHYRUM);
             c + II < (s32)t->latitudo; c++)
        {
            si (   cellula(t, l, c)      == '<'
                && cellula(t, l, c + I)  == '/'
                && cellula(t, l, c + II) == '>')
            {
                *linea_inventa    = l;
                *columna_inventa  = c;
                redde VERUM;
            }
        }
    }
    redde FALSUM;
}

/* corpus a (la, ca) ad (lb, cb) exclusum: lineae verbatim, spatiis
 * finalibus demptis, '\n' iunctae; spatia extrema totius dempta;
 * cellula vacua ('\0') = spatium */
interior chorda
corpus_legere (
    constans TabulaCharacterum* t,
                           s32  la,
                           s32  ca,
                           s32  lb,
                           s32  cb,
                       Piscina* piscina)
{
    chorda  c;
        i8* q;
       s32  l;
       s32  n;
       s32  a;
       s32  b;
       s32  k;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    q = (i8*)piscina_allocare(piscina, (memoriae_index)((lb - la + I)
        * ((s32)t->latitudo + I)));
    si (!q)
    {
        redde c;
    }
    n = ZEPHYRUM;
    per (l = la; l <= lb; l++)
    {
        a = l == la ? ca : ZEPHYRUM;
        b = l == lb ? cb : (s32)t->latitudo;
        dum (b > a && spatium_est(cellula(t, l, b - I)))
        {
            b--;
        }
        si (l > la)
        {
            q[n++] = (i8)'\n';
        }
        per (k = a; k < b; k++)
        {
            character x  = cellula(t, l, k);
            q[n++]       = (i8)(x == '\0' ? ' ' : x);
        }
    }
    a = ZEPHYRUM;
    dum (a < n && (spatium_est((character)q[a]) || q[a] == (i8)'\n'))
    {
        a++;
    }
    dum (   n > a
         && (spatium_est((character)q[n - I]) || q[n - I] == (i8)'\n'))
    {
        n--;
    }
    c.datum    = n > a ? q + a : NIHIL;
    c.mensura  = (i32)(n - a);
    redde c;
}

interior b32
legere (
    constans TabulaCharacterum* t,
                           s32  linea,
                           s32  c,
                   IussumNotum  notum,
                        vacuum* ctx,
                       Piscina* piscina,
                        Iussum* exitus,
                           b32  plenum,
                           b32  intra_probare);

/* '$' in (linea, c) intra corpus iussi prioris? Ab initio tabulae
 * percurritur; corpora transiliuntur (ergo '$' intra corpus numquam
 * corpus aperit - nulla nidificatio) */
interior b32
intra_corpus (
    constans TabulaCharacterum* t,
                           s32  linea,
                           s32  c,
                   IussumNotum  notum,
                        vacuum* ctx,
                       Piscina* piscina)
{
    Iussum x;
       s32 l;
       s32 k;

    l = ZEPHYRUM;
    k = ZEPHYRUM;
    dum (l < linea || (l == linea && k < c))
    {
        si (   cellula(t, l, k) == '$'
            && legere(t, l, k, notum, ctx, piscina, &x, FALSUM, FALSUM)
            && x.corpus)
        {
            si (   x.linea_clausurae > linea
                || (x.linea_clausurae == linea && x.post_clausuram > c))
            {
                redde VERUM;
            }
            l = x.linea_clausurae;
            k = x.post_clausuram;
        }
        alioquin
        {
            k++;
        }
        si (k >= (s32)t->latitudo)
        {
            l++;
            k = ZEPHYRUM;
        }
    }
    redde FALSUM;
}

/* iussum cuius '$' in columna c legere; FALSUM si non iussum.
 * plenum FALSUM: corpus non copiatur (percursio intra_corpus);
 * intra_probare: '$' intra corpus prius non est iussum */
interior b32
legere (
    constans TabulaCharacterum* t,
                           s32  linea,
                           s32  c,
                   IussumNotum  notum,
                        vacuum* ctx,
                       Piscina* piscina,
                        Iussum* exitus,
                           b32  plenum,
                           b32  intra_probare)
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
    exitus->corpus                = FALSUM;
    exitus->linea_clausurae       = linea;
    exitus->post_clausuram        = j;
    si (notum && !notum(exitus->verbum, ctx))
    {
        redde FALSUM;
    }
    si (intra_probare && intra_corpus(t, linea, c, notum, ctx, piscina))
    {
        redde FALSUM;
    }
    /* corpus: '<>' statim post verbum, '</>' primum - aliter prosa */
    si (   j + I < latitudo && cellula(t, linea, j) == '<'
        && cellula(t, linea, j + I) == '>')
    {
        s32 lc;
        s32 cc;

        si (!clausuram_invenire(t, linea, j + II, &lc, &cc))
        {
            redde FALSUM;
        }
        exitus->corpus           = VERUM;
        exitus->finis            = j + II;
        exitus->linea_clausurae  = lc;
        exitus->post_clausuram   = cc + III;
        si (plenum)
        {
            chorda corpus = corpus_legere(t, linea, j + II, lc, cc,
                piscina);
            si (corpus.mensura > ZEPHYRUM)
            {
                exitus->argumenta = (chorda*)piscina_allocare(piscina,
                    magnitudo(chorda));
                exitus->argumenta[ZEPHYRUM]   = corpus;
                exitus->numerus_argumentorum  = I;
            }
        }
        redde VERUM;
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
        si (legere(t, linea, c, notum, ctx, piscina, exitus, VERUM,
            VERUM))
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
    exitus->corpus                = FALSUM;
    exitus->linea_clausurae       = linea;
    exitus->post_clausuram        = j;
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

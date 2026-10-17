/* dependentia.c - Vide dependentia.h. */

#include "dependentia.h"
#include <string.h>

/* Structura arboris ad mensuras: filii ordine positionis, tempora
 * ingressus et egressus percursus (dominatio in O(1)), fines proventus.
 * Indices 1..n (elementum 0 non adhibetur). */
nomen structura {
               i32  numerus;
    constans   i32* capita;
               i32  radix;
               i32* filius_primus;   /* 0 = nullus */
               i32* frater;          /* proximus ordine positionis, 0 = nullus */
               i32* ingressus;
               i32* egressus;
} Structura;

interior vacuum*
_allocare (
    Piscina* piscina,
        i32  numerus,
        i32  magnitudo_elementi)
{
    memoriae_index mensura = (memoriae_index)numerus
        * (memoriae_index)magnitudo_elementi;
           vacuum* p;

    si (mensura == ZEPHYRUM)
    {
        mensura = I;
    }
    p = piscina_conari_allocare(piscina, mensura);
    si (p != NIHIL)
    {
        memset(p, ZEPHYRUM, (size_t)mensura);
    }
    redde p;
}

DependentiaStatus
dependentia_validare (
    constans DependentiaArbor* arbor)
{
    i32 n;
    i32 k;
    i32 radices = ZEPHYRUM;

    si (   arbor         == NIHIL || arbor->numerus == ZEPHYRUM
        || arbor->capita == NIHIL)
    {
        redde DEPENDENTIA_VACUA;
    }
    n = arbor->numerus;
    per (k = I; k <= n; k++)
    {
        i32 h = arbor->capita[k - I];

        si (h > n || h == k)
        {
            redde DEPENDENTIA_CAPUT_EXTRA;
        }
        si (h == ZEPHYRUM)
        {
            radices = radices + I;
        }
    }
    si (radices == ZEPHYRUM)
    {
        redde DEPENDENTIA_RADIX_NULLA;
    }
    si (radices > I)
    {
        redde DEPENDENTIA_RADICES_PLURES;
    }
    /* radix una: cyclus si via capitum post n passus radicem non
     * attingit */
    per (k = I; k <= n; k++)
    {
        i32 v       = k;
        i32 passus  = ZEPHYRUM;

        dum (v != ZEPHYRUM && passus <= n)
        {
            v       = arbor->capita[v - I];
            passus  = passus + I;
        }
        si (v != ZEPHYRUM)
        {
            redde DEPENDENTIA_CYCLUS;
        }
    }
    redde DEPENDENTIA_SANA;
}

/* Structuram parare (arbor sana requiritur). */
interior b32
_structura (
                      Piscina* piscina,
    constans DependentiaArbor* arbor,
                    Structura* s)
{
    i32  n = arbor->numerus;
    i32  k;
    i32  tempus = ZEPHYRUM;
    i32  culmen = ZEPHYRUM;
    i32* acervus;
    i32* cursor;

    s->numerus  = n;
    s->capita   = arbor->capita;
    s->radix    = ZEPHYRUM;
    s->filius_primus = (i32*)_allocare(piscina, n + I,
        (i32)magnitudo(i32));
    s->frater = (i32*)_allocare(piscina, n + I,
        (i32)magnitudo(i32));
    s->ingressus = (i32*)_allocare(piscina, n + I,
        (i32)magnitudo(i32));
    s->egressus = (i32*)_allocare(piscina, n + I,
        (i32)magnitudo(i32));
    acervus = (i32*)_allocare(piscina, n + I,
        (i32)magnitudo(i32));
    cursor = (i32*)_allocare(piscina, n + I,
        (i32)magnitudo(i32));
    si (   s->filius_primus == NIHIL || s->frater == NIHIL
        || s->ingressus     == NIHIL || s->egressus == NIHIL
        || acervus          == NIHIL || cursor == NIHIL)
    {
        redde FALSUM;
    }
    /* a dextra ad sinistram in caput indicis: filii ordine positionis */
    per (k = n; k >= I; k--)
    {
        i32 h = arbor->capita[k - I];

        si (h == ZEPHYRUM)
        {
            s->radix = k;
        }
        alioquin
        {
            s->frater[k]         = s->filius_primus[h];
            s->filius_primus[h]  = k;
        }
    }
    /* percursus sine recursione */
    acervus[culmen]         = s->radix;
    culmen                  = culmen + I;
    s->ingressus[s->radix]  = tempus;
    tempus                  = tempus + I;
    cursor[s->radix]        = s->filius_primus[s->radix];
    dum (culmen > ZEPHYRUM)
    {
        i32 u = acervus[culmen - I];
        i32 c = cursor[u];

        si (c != ZEPHYRUM)
        {
            cursor[u]        = s->frater[c];
            s->ingressus[c]  = tempus;
            tempus           = tempus + I;
            cursor[c]        = s->filius_primus[c];
            acervus[culmen]  = c;
            culmen           = culmen + I;
        }
        alioquin
        {
            s->egressus[u]  = tempus;
            tempus          = tempus + I;
            culmen          = culmen - I;
        }
    }
    redde VERUM;
}

/* u dominat v (u == v incluso) */
interior b32
_dominat (
    constans Structura* s,
                   i32  u,
                   i32  v)
{
    redde (b32)(s->ingressus[u] <= s->ingressus[v]
        && s->egressus[v] <= s->egressus[u]);
}

b32
dependentia_omittere (
                      Piscina* piscina,
    constans DependentiaArbor* arbor,
                 constans b32* omittere,
             DependentiaArbor* exitus,
                          i32* contractiones)
{
    i32  n;
    i32  k;
    i32  m       = ZEPHYRUM;
    i32  mutata  = ZEPHYRUM;
    i32* novus;
    i32* capita;

    si (   dependentia_validare(arbor) != DEPENDENTIA_SANA
        || omittere                    == NIHIL || exitus == NIHIL)
    {
        redde FALSUM;
    }
    n      = arbor->numerus;
    novus  = (i32*)_allocare(piscina, n + I, (i32)magnitudo(i32));
    si (novus == NIHIL)
    {
        redde FALSUM;
    }
    per (k = I; k <= n; k++)
    {
        si (!omittere[k - I])
        {
            m         = m + I;
            novus[k]  = m;
        }
        alioquin si (arbor->capita[k - I] == ZEPHYRUM)
        {
            redde FALSUM;   /* radix omissa */
        }
    }
    capita = (i32*)_allocare(piscina, m, (i32)magnitudo(i32));
    si (capita == NIHIL)
    {
        redde FALSUM;
    }
    per (k = I; k <= n; k++)
    {
        i32 h        = arbor->capita[k - I];
        b32 mutatum  = FALSUM;

        si (omittere[k - I])
        {
            perge;
        }
        /* radix non omissa: via capitum omissorum in verbo servato
         * desinit */
        dum (h != ZEPHYRUM && omittere[h - I])
        {
            h        = arbor->capita[h - I];
            mutatum  = VERUM;
        }
        capita[novus[k] - I] = h == ZEPHYRUM ? ZEPHYRUM : novus[h];
        si (mutatum)
        {
            mutata = mutata + I;
        }
    }
    exitus->numerus  = m;
    exitus->capita   = capita;
    si (contractiones != NIHIL)
    {
        *contractiones = mutata;
    }
    redde VERUM;
}

b32
dependentia_cruces (
                      Piscina* piscina,
    constans DependentiaArbor* arbor,
            DependentiaCruces* exitus)
{
    i32  n;
    i32  k;
    i32  i;
    i32  j;
    i32  m      = ZEPHYRUM;
    i32  radix  = ZEPHYRUM;
    i32* sinister;
    i32* dexter;
    b32* sectus;

    si (   dependentia_validare(arbor) != DEPENDENTIA_SANA
        || exitus                      == NIHIL)
    {
        redde FALSUM;
    }
    n         = arbor->numerus;
    sinister  = (i32*)_allocare(piscina, n, (i32)magnitudo(i32));
    dexter    = (i32*)_allocare(piscina, n, (i32)magnitudo(i32));
    sectus    = (b32*)_allocare(piscina, n, (i32)magnitudo(b32));
    si (sinister == NIHIL || dexter == NIHIL || sectus == NIHIL)
    {
        redde FALSUM;
    }
    per (k = I; k <= n; k++)
    {
        i32 h = arbor->capita[k - I];

        si (h == ZEPHYRUM)
        {
            radix = k;
            perge;
        }
        sinister[m]  = h < k ? h : k;
        dexter[m]    = h < k ? k : h;
        m            = m + I;
    }
    memset(exitus, ZEPHYRUM, magnitudo(*exitus));
    per (i = ZEPHYRUM; i < m; i++)
    {
        i32 a = sinister[i];
        i32 b = dexter[i];

        /* arcus radicis (0, radix): 0 < a < radix < b */
        si (a < radix && radix < b)
        {
            exitus->cruces_radicis = exitus->cruces_radicis + I;
        }
        per (j = i + I; j < m; j++)
        {
            i32 c = sinister[j];
            i32 e = dexter[j];

            si (a == c || a == e || b == c || b == e)
            {
                perge;   /* verbum commune: numquam crux */
            }
            exitus->paria_disiuncta = exitus->paria_disiuncta + I;
            si ((a < c && c < b && b < e) || (c < a && a < e && e < b))
            {
                exitus->cruces  = exitus->cruces + I;
                sectus[i]       = VERUM;
                sectus[j]       = VERUM;
            }
        }
    }
    per (i = ZEPHYRUM; i < m; i++)
    {
        si (sectus[i])
        {
            exitus->arcus_secti = exitus->arcus_secti + I;
        }
    }
    exitus->planaris    = (b32)(exitus->cruces == ZEPHYRUM);
    exitus->proiectiva  = (b32)(exitus->planaris
        && exitus->cruces_radicis == ZEPHYRUM);
    redde VERUM;
}

/* proventus duorum fratrum alternant? (positiones a minimo ad maximum
 * percurruntur; series notarum A/B ceteris omissis alternat si mutationes
 * >= III) */
interior b32
_alternant (
    constans Structura* s,
                   i32  a,
                   i32  b,
                   i32  initium,
                   i32  finis)
{
    i32 p;
    i32 ultima      = ZEPHYRUM;   /* 0 nulla, I a, II b */
    i32 mutationes  = ZEPHYRUM;

    per (p = initium; p <= finis; p++)
    {
        i32 nota = ZEPHYRUM;

        si (_dominat(s, a, p))
        {
            nota = I;
        }
        alioquin si (_dominat(s, b, p))
        {
            nota = II;
        }
        si (nota == ZEPHYRUM)
        {
            perge;
        }
        si (ultima != ZEPHYRUM && nota != ultima)
        {
            mutationes = mutationes + I;
            si (mutationes >= III)
            {
                redde VERUM;
            }
        }
        ultima = nota;
    }
    redde FALSUM;
}

b32
dependentia_mensurare (
                      Piscina* piscina,
    constans DependentiaArbor* arbor,
                          i32* lacunae,
           DependentiaMensura* exitus)
{
    Structura  s;
          i32  n;
          i32  u;
          i32  p;
          i32* minimum;
          i32* maximum;

    si (   dependentia_validare(arbor) != DEPENDENTIA_SANA
        || exitus                      == NIHIL)
    {
        redde FALSUM;
    }
    si (!_structura(piscina, arbor, &s))
    {
        redde FALSUM;
    }
    n        = arbor->numerus;
    minimum  = (i32*)_allocare(piscina, n + I, (i32)magnitudo(i32));
    maximum  = (i32*)_allocare(piscina, n + I, (i32)magnitudo(i32));
    si (minimum == NIHIL || maximum == NIHIL)
    {
        redde FALSUM;
    }
    memset(exitus, ZEPHYRUM, magnitudo(*exitus));
    exitus->bene_nidificata = VERUM;

    /* lacunae (Def. 6-7): cursus verborum dominatorum - I */
    per (u = I; u <= n; u++)
    {
        i32 cursus  = ZEPHYRUM;
        b32 intus   = FALSUM;

        per (p = I; p <= n; p++)
        {
            b32 d = _dominat(&s, u, p);

            si (d)
            {
                si (minimum[u] == ZEPHYRUM)
                {
                    minimum[u] = p;
                }
                maximum[u] = p;
                si (!intus)
                {
                    cursus = cursus + I;
                }
            }
            intus = d;
        }
        si (lacunae != NIHIL)
        {
            lacunae[u - I] = cursus - I;
        }
        si (cursus - I > exitus->gradus_lacunarum)
        {
            exitus->gradus_lacunarum = cursus - I;
        }
    }
    exitus->proiectiva = (b32)(exitus->gradus_lacunarum == ZEPHYRUM);

    /* nidificatio (Def. 8): subarbores disiunctae alternantes habent
     * antecessores fratres (filios antecessoris communis infimi) qui
     * etiam alternant, ergo paria fratrum sufficiunt */
    per (u = I; u <= n && exitus->bene_nidificata; u++)
    {
        i32 a;

        per (a = s.filius_primus[u]; a != ZEPHYRUM
            && exitus->bene_nidificata; a = s.frater[a])
        {
            i32 b;

            per (b = s.frater[a]; b != ZEPHYRUM; b = s.frater[b])
            {
                i32 initium;
                i32 finis;

                si (maximum[a] < minimum[b] || maximum[b] < minimum[a])
                {
                    perge;
                }
                initium  = minimum[a] < minimum[b] ? minimum[a]
                    : minimum[b];
                finis    = maximum[a] > maximum[b] ? maximum[a]
                    : maximum[b];
                si (_alternant(&s, a, b, initium, finis))
                {
                    exitus->bene_nidificata = FALSUM;
                    frange;
                }
            }
        }
    }

    /* gradus arcuum (Def. 9): in intervallo arcus radix componentis est
     * verbum cuius caput extra intervallum iacet (subgraphum arboris
     * inductum); numerantur radices a capite arcus non dominatae */
    per (u = I; u <= n; u++)
    {
        i32 h = arbor->capita[u - I];
        i32 initium;
        i32 finis;
        i32 gradus = ZEPHYRUM;

        si (h == ZEPHYRUM)
        {
            perge;
        }
        initium  = h < u ? h : u;
        finis    = h < u ? u : h;
        per (p = initium; p <= finis; p++)
        {
            i32 c = arbor->capita[p - I];

            si (   (c == ZEPHYRUM || c < initium || c > finis)
                && !_dominat(&s, h, p))
            {
                gradus = gradus + I;
            }
        }
        si (gradus > exitus->gradus_arcuum)
        {
            exitus->gradus_arcuum = gradus;
        }
    }
    redde VERUM;
}

s32
dependentia_nucleus_planaris (
                      Piscina* piscina,
    constans DependentiaArbor* arbor)
{
    i32  n;
    i32  k;
    i32  longitudo;
    i32* initia;      /* CSR: dextri arcuum a sinistro i */
    i32* dextri;
    s32* f;           /* f[(i - 1) n + (j - 1)] = optimum intra [i, j] */

    si (dependentia_validare(arbor) != DEPENDENTIA_SANA)
    {
        redde (s32)-I;
    }
    n       = arbor->numerus;
    initia  = (i32*)_allocare(piscina, n + II, (i32)magnitudo(i32));
    dextri  = (i32*)_allocare(piscina, n, (i32)magnitudo(i32));
    f       = (s32*)piscina_conari_allocare(piscina, (memoriae_index)n
        * (memoriae_index)n * (memoriae_index)magnitudo(s32));
    si (initia == NIHIL || dextri == NIHIL || f == NIHIL)
    {
        redde (s32)-I;
    }
    memset(f, ZEPHYRUM, (size_t)n * (size_t)n * magnitudo(s32));
    /* numerare per sinistrum, deinde collocare */
    per (k = I; k <= n; k++)
    {
        i32 h = arbor->capita[k - I];

        si (h != ZEPHYRUM)
        {
            i32 a = h < k ? h : k;

            initia[a + I] = initia[a + I] + I;
        }
    }
    per (k = I; k <= n; k++)
    {
        initia[k + I] = initia[k + I] + initia[k];
    }
    {
        i32* positio = (i32*)_allocare(piscina, n + I,
            (i32)magnitudo(i32));

        si (positio == NIHIL)
        {
            redde (s32)-I;
        }
        per (k = I; k <= n; k++)
        {
            positio[k] = initia[k];
        }
        per (k = I; k <= n; k++)
        {
            i32 h = arbor->capita[k - I];

            si (h != ZEPHYRUM)
            {
                i32 a = h < k ? h : k;
                i32 b = h < k ? k : h;

                dextri[positio[a]]  = b;
                positio[a]          = positio[a] + I;
            }
        }
    }
    /* f(i, j) = h(i, j) + [arcus (i, j)]; h(i, j) = max(f(i + 1, j),
     * max per arcus (i, k), i < k < j: f(i, k) + f(k, j)). Arcus (i, k)
     * longissimus ex i electus nullum arcum intra [i, k] secat; arcus
     * electi aut intra [i, k] aut intra [k, j] iacent. */
    per (longitudo = I; longitudo < n; longitudo++)
    {
        i32 i;

        per (i = I; i + longitudo <= n; i++)
        {
            i32 j        = i + longitudo;
            /* f(i + 1, j): i + 1 <= j semper */
            s32 optimum  = f[(i32)(i * n + (j - I))];
            i32 q;
            b32 arcus    = FALSUM;

            per (q = initia[i]; q < initia[i + I]; q++)
            {
                i32 kk = dextri[q];

                si (kk == j)
                {
                    arcus = VERUM;
                }
                alioquin si (kk < j)
                {
                    s32 v = f[(i32)((i - I) * n + (kk - I))]
                        + f[(i32)((kk - I) * n + (j - I))];

                    si (v > optimum)
                    {
                        optimum = v;
                    }
                }
            }
            f[(i32)((i - I) * n + (j - I))] = optimum
                + (arcus ? I : ZEPHYRUM);
        }
    }
    redde f[(i32)(n - I)];   /* f(1, n) */
}

/* radix collectionis disiunctae (compressio viae per dimidium) */
interior i32
_radix (
    i32* parens,
    i32  x)
{
    dum (parens[x] != x)
    {
        parens[x]  = parens[parens[x]];
        x          = parens[x];
    }
    redde x;
}

s32
dependentia_componentes (
                      Piscina* piscina,
    constans DependentiaArbor* arbor,
                          s32* componens)
{
    i32  n;
    i32  k;
    i32  q;
    s32  numerus = ZEPHYRUM;
    i32* parens;
    s32* nota;

    si (   dependentia_validare(arbor) != DEPENDENTIA_SANA
        || componens                   == NIHIL)
    {
        redde (s32)-I;
    }
    n       = arbor->numerus;
    parens  = (i32*)_allocare(piscina, n + I, (i32)magnitudo(i32));
    nota    = (s32*)_allocare(piscina, n + I, (i32)magnitudo(s32));
    si (parens == NIHIL || nota == NIHIL)
    {
        redde (s32)-I;
    }
    per (k = I; k <= n; k++)
    {
        parens[k]  = k;
        nota[k]    = (s32)-I;
    }
    /* arcus nominatur verbo dependenti */
    per (k = I; k <= n; k++)
    {
        i32 h = arbor->capita[k - I];
        i32 a;
        i32 b;

        si (h == ZEPHYRUM)
        {
            perge;
        }
        a = h < k ? h : k;
        b = h < k ? k : h;
        per (q = k + I; q <= n; q++)
        {
            i32 g = arbor->capita[q - I];
            i32 c;
            i32 e;

            si (g == ZEPHYRUM)
            {
                perge;
            }
            c = g < q ? g : q;
            e = g < q ? q : g;
            si ((a < c && c < b && b < e) || (c < a && a < e && e < b))
            {
                i32 ra = _radix(parens, k);
                i32 rb = _radix(parens, q);

                si (ra != rb)
                {
                    parens[rb] = ra;
                }
            }
        }
    }
    per (k = I; k <= n; k++)
    {
        i32 r;

        si (arbor->capita[k - I] == ZEPHYRUM)
        {
            componens[k - I] = (s32)-I;
            perge;
        }
        r = _radix(parens, k);
        si (nota[r] < ZEPHYRUM)
        {
            nota[r] = numerus;
            numerus = numerus + I;
        }
        componens[k - I] = nota[r];
    }
    redde numerus;
}

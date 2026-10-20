/* conllu.c - Vide conllu.h. */

#include "conllu.h"
#include <string.h>

interior chorda
_chorda (
     i8* datum,
    i32  mensura)
{
    chorda c;

    c.datum    = datum;
    c.mensura  = mensura;
    redde c;
}

interior vacuum
_vitium (
          ConlluVitium* v,
                   i32  linea,
    constans character* causa)
{
    si (v != NIHIL)
    {
        v->linea = linea;
        v->causa = causa;
    }
}

interior b32
_incipit (
                chorda  c,
    constans character* literae)
{
    i32 l = (i32)strlen(literae);

    redde (b32)(c.mensura >= l
        && memcmp(c.datum, literae, (size_t)l) == ZEPHYRUM);
}

/* numerus decimalis totus; FALSUM si non totus aut vacuus */
interior b32
_numerus (
    chorda  c,
       i32* exitus)
{
    i32 v = ZEPHYRUM;
    i32 i;

    si (c.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < c.mensura; i++)
    {
        si (c.datum[i] < '0' || c.datum[i] > '9')
        {
            redde FALSUM;
        }
        v = v * (i32)X + (i32)(c.datum[i] - '0');
    }
    *exitus = v;
    redde VERUM;
}

/* HEAD: numerus decimalis (IX cifrae ad summum); -1 aliter */
interior s32
_caput (
    chorda c)
{
    i32 v;

    si (c.mensura > (i32)IX || !_numerus(c, &v))
    {
        redde (s32)-I;
    }
    redde (s32)v;
}

b32
conllu_spatium_post (
    chorda misc)
{
                   i32  i;
    constans character* signum  = "SpaceAfter=No";
                   i32  l       = (i32)strlen(signum);

    per (i = ZEPHYRUM; i + l <= misc.mensura; i++)
    {
        si (   memcmp(misc.datum + i, signum, (size_t)l) == ZEPHYRUM
            && (i == ZEPHYRUM || misc.datum[i - I] == '|')
            && (i + l == misc.mensura || misc.datum[i + l] == '|'))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* linea lexematis in X campos scindere; FALSUM si numerus campi non X */
interior b32
_campi (
    chorda linea,
    chorda campi[10])
{
    i32 i = ZEPHYRUM;
    i32 k = ZEPHYRUM;

    dum (k < (i32)X)
    {
        i32 a = i;

        dum (i < linea.mensura && linea.datum[i] != '\t')
        {
            i = i + I;
        }
        campi[k]  = _chorda(linea.datum + a, i - a);
        k         = k + I;
        si (i >= linea.mensura)
        {
            frange;
        }
        i = i + I;   /* tabulatio */
    }
    redde (b32)(k == (i32)X && i >= linea.mensura);
}

Xar*
conllu_legere (
         Piscina* piscina,
          chorda  fons,
    ConlluVitium* vitium)
{
    Xar* sententiae = xar_creare(piscina,
        (i32)magnitudo(ConlluSententia));
          ConlluSententia* s       = NIHIL;
                      i32  cursor  = ZEPHYRUM;
                      i32  linea   = ZEPHYRUM;
    /* T38 a: '# newdoc id' ultimum visum - documentum sententiarum
     * sequentium (vacuum ante primum) */
                   chorda documentum = _chorda(NIHIL, ZEPHYRUM);

    si (sententiae == NIHIL)
    {
        _vitium(vitium, ZEPHYRUM, "memoria");
        redde NIHIL;
    }
    dum (cursor < fons.mensura)
    {
           i32 a = cursor;
           i32 b = a;
        chorda l;

        dum (b < fons.mensura && fons.datum[b] != '\n')
        {
            b = b + I;
        }
        cursor  = b + I;
        linea   = linea + I;
        si (b > a && fons.datum[b - I] == '\r')
        {
            b = b - I;
        }
        l = _chorda(fons.datum + a, b - a);
        si (l.mensura == ZEPHYRUM)
        {
            /* linea vacua: sententia clauditur */
            si (s != NIHIL && xar_numerus(s->lexemata) == ZEPHYRUM)
            {
                xar_removere_ultimum(sententiae);
            }
            s = NIHIL;
            perge;
        }
        si (s == NIHIL)
        {
            s = (ConlluSententia*)xar_addere(sententiae);
            si (s == NIHIL)
            {
                _vitium(vitium, linea, "memoria");
                redde NIHIL;
            }
            memset(s, ZEPHYRUM, magnitudo(*s));
            s->lexemata = xar_creare(piscina,
                (i32)magnitudo(ConlluLexema));
            s->linea       = linea;
            s->documentum  = documentum;   /* T38 a */
            si (s->lexemata == NIHIL)
            {
                _vitium(vitium, linea, "memoria");
                redde NIHIL;
            }
        }
        si (l.datum[ZEPHYRUM] == '#')
        {
            si (_incipit(l, "# text = "))
            {
                s->textus = _chorda(l.datum + (i32)IX, l.mensura
                    - (i32)IX);
            }
            alioquin si (_incipit(l, "# sent_id = "))
            {
                s->id = _chorda(l.datum + (i32)XII, l.mensura
                    - (i32)XII);
            }
            alioquin si (_incipit(l, "# newdoc id = "))
            {
                /* T38 a: documentum novum - haec sententia et sequentes */
                documentum    = _chorda(l.datum + (i32)XIV, l.mensura
                    - (i32)XIV);
                s->documentum = documentum;
            }
            perge;
        }
        {
                        chorda  campi[10];
                  ConlluLexema* t;
                           i32  k;
                           i32  na;
                           i32  nb;

            si (!_campi(l, campi))
            {
                _vitium(vitium, linea, "campi non X");
                redde NIHIL;
            }
            /* ID: a | a-b | a.b (vacuus: omittitur) */
            k = ZEPHYRUM;
            dum (   k < campi[ZEPHYRUM].mensura
                 && campi[ZEPHYRUM].datum[k] != '-'
                 && campi[ZEPHYRUM].datum[k] != '.')
            {
                k = k + I;
            }
            si (!_numerus(_chorda(campi[ZEPHYRUM].datum, k), &na))
            {
                _vitium(vitium, linea, "ID non numerus");
                redde NIHIL;
            }
            si (   k < campi[ZEPHYRUM].mensura
                && campi[ZEPHYRUM].datum[k] == '.')
            {
                perge;   /* nodus vacuus */
            }
            nb = na;
            si (k < campi[ZEPHYRUM].mensura)
            {
                si (   !_numerus(_chorda(campi[ZEPHYRUM].datum + k + I,
                        campi[ZEPHYRUM].mensura - k - I), &nb)
                    || nb < na)
                {
                    _vitium(vitium, linea, "ranga mala");
                    redde NIHIL;
                }
            }
            t = (ConlluLexema*)xar_addere(s->lexemata);
            si (t == NIHIL)
            {
                _vitium(vitium, linea, "memoria");
                redde NIHIL;
            }
            t->a             = na;
            t->b             = nb;
            t->ranga         = (b32)(nb != na);
            t->forma         = campi[I];
            t->lemma         = campi[II];
            t->upos          = campi[III];
            t->xpos          = campi[IV];
            t->feats         = campi[V];
            t->head          = campi[VI];
            t->deprel        = campi[VII];
            t->deps          = campi[VIII];
            t->misc          = campi[IX];
            t->spatium_post  = conllu_spatium_post(campi[IX]);
            t->linea         = linea;
            t->caput         = _caput(campi[VI]);
        }
    }
    si (s != NIHIL && xar_numerus(s->lexemata) == ZEPHYRUM)
    {
        xar_removere_ultimum(sententiae);
    }
    redde sententiae;
}

chorda
conllu_textus (
                     Piscina* piscina,
    constans ConlluSententia* sententia)
{
       i32 n        = xar_numerus(sententia->lexemata);
       i32 mensura  = ZEPHYRUM;
       i32 k;
       i32 intra_rangam = ZEPHYRUM;
    chorda c;

    si (sententia->textus.mensura > ZEPHYRUM)
    {
        c.datum = (i8*)piscina_allocare(piscina,
            (memoriae_index)sententia->textus.mensura + I);
        si (c.datum == NIHIL)
        {
            redde _chorda(NIHIL, ZEPHYRUM);
        }
        memcpy(c.datum, sententia->textus.datum,
            (size_t)sententia->textus.mensura);
        c.mensura           = sententia->textus.mensura;
        c.datum[c.mensura]  = ZEPHYRUM;
        redde c;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        constans ConlluLexema* t =
            (constans ConlluLexema*)xar_obtinere(sententia->lexemata,
            k);

        mensura = mensura + t->forma.mensura + I;
    }
    c.datum = (i8*)piscina_allocare(piscina, (memoriae_index)mensura
        + I);
    si (c.datum == NIHIL)
    {
        redde _chorda(NIHIL, ZEPHYRUM);
    }
    c.mensura = ZEPHYRUM;
    per (k = ZEPHYRUM; k < n; k++)
    {
        constans ConlluLexema* t =
            (constans ConlluLexema*)xar_obtinere(sententia->lexemata,
            k);

        si (intra_rangam > ZEPHYRUM)
        {
            intra_rangam = intra_rangam - I;   /* verbum rangae: superficies iam scripta */
            perge;
        }
        si (t->ranga)
        {
            intra_rangam = t->b - t->a + I;
        }
        memcpy(c.datum + c.mensura, t->forma.datum,
            (size_t)t->forma.mensura);
        c.mensura = c.mensura + t->forma.mensura;
        si (t->spatium_post && k + I < n)
        {
            c.datum[c.mensura]  = ' ';
            c.mensura           = c.mensura + I;
        }
    }
    c.datum[c.mensura] = ZEPHYRUM;
    redde c;
}

chorda
conllu_proprietas (
                chorda  feats,
    constans character* clavis)
{
    i32 l = (i32)strlen(clavis);
    i32 i = ZEPHYRUM;

    /* paria 'Clavis=Valor' lineis '|' divisa */
    dum (i < feats.mensura)
    {
        i32 a = i;

        dum (i < feats.mensura && feats.datum[i] != '|')
        {
            i = i + I;
        }
        si (   i - a > l
            && memcmp(feats.datum + a, clavis, (size_t)l) == ZEPHYRUM
            && feats.datum[a + l]                         == '=')
        {
            redde _chorda(feats.datum + a + l + I, i - a - l - I);
        }
        i = i + I;   /* '|' */
    }
    redde _chorda(NIHIL, ZEPHYRUM);
}

b32
conllu_arbor (
                     Piscina* piscina,
    constans ConlluSententia* sententia,
                 ConlluArbor* exitus)
{
                      i32   n        = ZEPHYRUM;
                      i32   numerus  = xar_numerus(sententia->lexemata);
                      i32   k;
                      i32*  capita;
    constans ConlluLexema** verba;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        constans ConlluLexema* t =
            (constans ConlluLexema*)xar_obtinere(sententia->lexemata,
            k);

        si (!t->ranga)
        {
            n = n + I;
        }
    }
    capita = (i32*)piscina_conari_allocare(piscina,
        (memoriae_index)(n > ZEPHYRUM ? n : I) * magnitudo(i32));
    verba = (constans ConlluLexema**)piscina_conari_allocare(piscina,
        (memoriae_index)(n > ZEPHYRUM ? n : I)
        * magnitudo(constans ConlluLexema*));
    si (capita == NIHIL || verba == NIHIL)
    {
        redde FALSUM;
    }
    n = ZEPHYRUM;
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        constans ConlluLexema* t =
            (constans ConlluLexema*)xar_obtinere(sententia->lexemata,
            k);

        si (t->ranga)
        {
            perge;
        }
        si (t->a != n + I)
        {
            redde FALSUM;   /* ID non consecutivi */
        }
        verba[n]  = t;
        n         = n + I;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        /* caput non numeratum: extra 0..n, ut validatio id reiciat */
        capita[k] = verba[k]->caput < ZEPHYRUM ? n + I
            : (i32)verba[k]->caput;
    }
    exitus->arbor.numerus  = n;
    exitus->arbor.capita   = capita;
    exitus->verba          = verba;
    redde VERUM;
}

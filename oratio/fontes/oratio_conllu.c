/* oratio_conllu.c - Vide oratio_conllu.h. */

#include "oratio_conllu.h"
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
    OratioVocabulariumVitium* v,
                         i32  linea,
          constans character* causa)
{
    si (v != NIHIL)
    {
        v->plagula  = "conllu";
        v->linea    = linea;
        v->offset   = (s32)-I;
        v->causa    = causa;
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

b32
oratio_conllu_spatium_post (
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
oratio_conllu_legere (
                     Piscina* piscina,
                      chorda  fons,
    OratioVocabulariumVitium* vitium)
{
    Xar* sententiae = xar_creare(piscina,
        (i32)magnitudo(OratioConlluSententia));
    OratioConlluSententia* s       = NIHIL;
                      i32  cursor  = ZEPHYRUM;
                      i32  linea   = ZEPHYRUM;

    si (sententiae == NIHIL)
    {
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
            s = (OratioConlluSententia*)xar_addere(sententiae);
            si (s == NIHIL)
            {
                redde NIHIL;
            }
            memset(s, ZEPHYRUM, magnitudo(*s));
            s->lexemata = xar_creare(piscina,
                (i32)magnitudo(OratioConlluLexema));
            s->linea    = linea;
            si (s->lexemata == NIHIL)
            {
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
            perge;
        }
        {
                        chorda  campi[10];
            OratioConlluLexema* t;
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
            t = (OratioConlluLexema*)xar_addere(s->lexemata);
            si (t == NIHIL)
            {
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
            t->spatium_post  = oratio_conllu_spatium_post(campi[IX]);
            t->linea         = linea;
        }
    }
    si (s != NIHIL && xar_numerus(s->lexemata) == ZEPHYRUM)
    {
        xar_removere_ultimum(sententiae);
    }
    redde sententiae;
}

chorda
oratio_conllu_textus (
                           Piscina* piscina,
    constans OratioConlluSententia* sententia)
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
        constans OratioConlluLexema* t =
            (constans OratioConlluLexema*)xar_obtinere(sententia->lexemata,
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
        constans OratioConlluLexema* t =
            (constans OratioConlluLexema*)xar_obtinere(sententia->lexemata,
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

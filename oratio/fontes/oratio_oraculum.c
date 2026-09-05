/* oratio_oraculum.c - Vide oratio_oraculum.h. */

#include "oratio_oraculum.h"
#include "oratio_arbor.h"
#include "oratio_partes.h"
#include "materia_nodus.h"
#include "materia_token.h"
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

interior chorda
_copia (
    Piscina* piscina,
     chorda  c)
{
    chorda d;

    d.datum = (i8*)piscina_allocare(piscina, (memoriae_index)c.mensura
        + I);
    si (d.datum == NIHIL)
    {
        redde _chorda(NIHIL, ZEPHYRUM);
    }
    si (c.mensura > ZEPHYRUM)
    {
        memcpy(d.datum, c.datum, (size_t)c.mensura);
    }
    d.datum[c.mensura]  = ZEPHYRUM;
    d.mensura           = c.mensura;
    redde d;
}

interior b32
_aequalis (
    chorda a,
    chorda b)
{
    redde (b32)(a.mensura == b.mensura
        && (a.mensura == ZEPHYRUM
            || memcmp(a.datum, b.datum, (size_t)a.mensura)
                == ZEPHYRUM));
}

interior b32
_literis (
                chorda  c,
    constans character* literae)
{
    redde (b32)(c.mensura == (i32)strlen(literae)
        && (c.mensura == ZEPHYRUM
            || memcmp(c.datum, literae, (size_t)c.mensura)
                == ZEPHYRUM));
}

hic_manens constans character* constans UPOS[] = {
    "NOUN", "PROPN", "VERB", "AUX", "ADJ", "ADV", "PRON", "DET", "ADP",
    "NUM", "CCONJ", "SCONJ", "PART", "INTJ", "SYM", "PUNCT", "X"
};

OratioClassis
oratio_oraculum_classis_ex_upos (
    chorda upos)
{
    i32 i;

    per (i = ZEPHYRUM; i < (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM; i++)
    {
        si (_literis(upos, UPOS[i]))
        {
            redde (OratioClassis)i;
        }
    }
    redde ORATIO_CLASSIS_NUMERUS_CLASSIUM;
}

vacuum
oratio_oraculum_census_vacare (
    OratioOraculumCensus* census)
{
    memset(census, ZEPHYRUM, magnitudo(*census));
}


/* ==================================================
 * Elementa arboris cum extentibus et classibus
 * ================================================== */

#define CLASSES_MAXIMAE 17
#define LEMMATA_MAXIMA  64

nomen structura {
    s32 a;                             /* extentum octetorum in textu */
    s32 b;
    chorda classes;                    /* lexema derivatum (vocabulum) aut titulus */
    i32 numerus_classium;
    OratioClassis classis[CLASSES_MAXIMAE];
    i32 numerus_lemmatum;
    chorda lemmata[LEMMATA_MAXIMA];    /* plicata */
    b32 ignotum;
} Elementum;

/* extentum lexematum listae loci */
interior vacuum
_extentum_listae (
    constans MateriaValor* v,
                      s32* a,
                      s32* b)
{
    i32 n;
    i32 k;

    si (v->genus == MATERIA_VALOR_TOKEN && v->datum.token != NIHIL)
    {
        constans MateriaToken* t = v->datum.token;

        si (t->byte_offset >= ZEPHYRUM)
        {
            si (*a < ZEPHYRUM || t->byte_offset < *a)
            {
                *a = t->byte_offset;
            }
            si (t->byte_offset + (s32)t->valor.mensura > *b)
            {
                *b = t->byte_offset + (s32)t->valor.mensura;
            }
        }
        redde;
    }
    si (v->genus != MATERIA_VALOR_LISTA)
    {
        redde;
    }
    n = materia_valor_lista_numerus(*v);
    per (k = ZEPHYRUM; k < n; k++)
    {
        constans MateriaValor* e = materia_valor_lista_obtinere(*v, k);

        si (e != NIHIL && e->genus == MATERIA_VALOR_TOKEN)
        {
            _extentum_listae(e, a, b);
        }
    }
}

interior b32
_elementum_addere (
                          Piscina* piscina,
    constans OratioVocabulariumLa* voc,
            constans MateriaNodus* n,
                              Xar* elementa)
{
    Elementum* e = (Elementum*)xar_addere(elementa);
          i32  k;

    si (e == NIHIL)
    {
        redde FALSUM;
    }
    memset(e, ZEPHYRUM, magnitudo(*e));
    e->a = (s32)-I;
    e->b = ZEPHYRUM;
    si (n->genus == (s32)ORATIO_GENUS_VOCABULUM)
    {
        constans MateriaValor* classes =
            &n->loci[ORATIO_VOCABULUM_CLASSES];
        constans MateriaValor* analyses =
            &n->loci[ORATIO_VOCABULUM_ANALYSES];

        _extentum_listae(&n->loci[ORATIO_VOCABULUM_PARTES], &e->a,
            &e->b);
        si (   classes->genus       == MATERIA_VALOR_TOKEN
            && classes->datum.token != NIHIL)
        {
            chorda c = classes->datum.token->valor;
               i32 i = ZEPHYRUM;

            e->classes = c;
            dum (   i < c.mensura
                 && e->numerus_classium < (i32)CLASSES_MAXIMAE)
            {
                          i32 a = i;
                OratioClassis cl;

                dum (i < c.mensura && c.datum[i] != ' ')
                {
                    i = i + I;
                }
                cl =
                    oratio_classis_ex_titulo((constans character*)c.datum
                        + a,
                    i - a);
                si ((i32)cl < (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM)
                {
                    e->classis[e->numerus_classium] = cl;
                    e->numerus_classium = e->numerus_classium + I;
                }
                i = i + I;
            }
        }
        e->ignotum = (b32)(e->numerus_classium == I
            && e->classis[ZEPHYRUM] == ORATIO_CLASSIS_IGNOTUM);
        si (analyses->genus == MATERIA_VALOR_LISTA)
        {
            i32 m = materia_valor_lista_numerus(*analyses);

            per (k = ZEPHYRUM; k < m
                && e->numerus_lemmatum < (i32)LEMMATA_MAXIMA;
                 k++)
            {
                constans MateriaNodus* an =
                    materia_valor_lista_obtinere(
                    *analyses, k)->datum.nodus;
                constans MateriaValor* l =
                    &an->loci[ORATIO_ANALYSIS_LEMMA];

                si (   l->genus       == MATERIA_VALOR_TOKEN
                    && l->datum.token != NIHIL)
                {
                    chorda plicata =
                        oratio_vocabularium_la_plicare(piscina,
                        l->datum.token->valor);
                    i32 j;
                    b32 nova = VERUM;

                    per (j = ZEPHYRUM; j < e->numerus_lemmatum; j++)
                    {
                        si (_aequalis(e->lemmata[j], plicata))
                        {
                            nova = FALSUM;
                            frange;
                        }
                    }
                    si (nova)
                    {
                        e->lemmata[e->numerus_lemmatum] = plicata;
                        e->numerus_lemmatum = e->numerus_lemmatum + I;
                    }
                }
            }
        }
        (vacuum)voc;
        redde VERUM;
    }
    si (n->genus == (s32)ORATIO_GENUS_INTERPUNCTIO)
    {
        _extentum_listae(&n->loci[ORATIO_INTERPUNCTIO_SIGNUM], &e->a,
            &e->b);
        e->classis[ZEPHYRUM]  = ORATIO_CLASSIS_INTERPUNCTIO;
        e->numerus_classium   = I;
        e->classes = _copia(piscina, _chorda(NIHIL,
            ZEPHYRUM));
        redde VERUM;
    }
    si (n->genus == (s32)ORATIO_GENUS_NUMERUS)
    {
        _extentum_listae(&n->loci[ORATIO_NUMERUS_CRUDUM], &e->a, &e->b);
        e->classis[ZEPHYRUM]  = ORATIO_CLASSIS_NUMERALE;
        e->numerus_classium   = I;
        redde VERUM;
    }
    xar_removere_ultimum(elementa);
    redde VERUM;
}

interior b32
_elementa_colligere (
                          Piscina* piscina,
    constans OratioVocabulariumLa* voc,
            constans MateriaNodus* n,
                              Xar* elementa)
{
    i32 i;

    si (n == NIHIL)
    {
        redde VERUM;
    }
    si (   n->genus == (s32)ORATIO_GENUS_VOCABULUM
        || n->genus == (s32)ORATIO_GENUS_INTERPUNCTIO
        || n->genus == (s32)ORATIO_GENUS_NUMERUS)
    {
        redde _elementum_addere(piscina, voc, n, elementa);
    }
    per (i = ZEPHYRUM; i < n->numerus_locorum; i++)
    {
        constans MateriaValor* v = &n->loci[i];

        si (v->genus == MATERIA_VALOR_NODUS)
        {
            si (!_elementa_colligere(piscina, voc, v->datum.nodus,
                elementa))
            {
                redde FALSUM;
            }
        }
        alioquin si (v->genus == MATERIA_VALOR_LISTA)
        {
            i32 m = materia_valor_lista_numerus(*v);
            i32 j;

            per (j = ZEPHYRUM; j < m; j++)
            {
                constans MateriaValor* e =
                    materia_valor_lista_obtinere(*v, j);

                si (   e != NIHIL && e->genus == MATERIA_VALOR_NODUS
                    && !_elementa_colligere(piscina, voc,
                    e->datum.nodus,
                        elementa))
                {
                    redde FALSUM;
                }
            }
        }
    }
    redde VERUM;
}


/* ==================================================
 * Iudicium
 * ================================================== */

interior vacuum
_exemplum (
    OratioOraculumClassis* c,
                  Piscina* piscina,
                   chorda  forma,
                   chorda  classes,
                   chorda  lemma)
{
    OratioOraculumExemplum* e;

    si (c->numerus_exemplorum >= (i32)ORATIO_ORACULUM_EXEMPLA)
    {
        redde;
    }
    e         = &c->exempla[c->numerus_exemplorum];
    e->forma  = _copia(piscina, forma);
    e->classes  = _copia(piscina, classes.mensura > ZEPHYRUM ? classes
        : _chorda(NIHIL, ZEPHYRUM));
    e->lemma               = _copia(piscina, lemma);
    c->numerus_exemplorum  = c->numerus_exemplorum + I;
}

/* formam in textu a cursore quaerere (spatia praeterita); -I si absens */
interior s32
_formam_invenire (
    chorda textus,
       i32 cursor,
    chorda forma)
{
    i32 i = cursor;

    si (forma.mensura == ZEPHYRUM)
    {
        redde (s32)-I;
    }
    dum (i + forma.mensura <= textus.mensura)
    {
        si (memcmp(textus.datum + i, forma.datum, (size_t)forma.mensura)
            == ZEPHYRUM)
        {
            redde (s32)i;
        }
        i = i + I;
    }
    redde (s32)-I;
}

/* verbum aureum unum contra classes elementorum [e0, e1) iudicare */
interior vacuum
_verbum_iudicare (
                        Piscina* piscina,
           OratioOraculumCensus* census,
                            Xar* elementa,
                            i32  e0,
                            i32  e1,
    constans OratioConlluLexema* verbum,
                            b32  primum)
{
    OratioClassis aurea = oratio_oraculum_classis_ex_upos(verbum->upos);
    OratioOraculumClassis* c;
    b32 tectum    = FALSUM;
    b32 primaria  = FALSUM;
    b32 lemma     = FALSUM;
    b32 ignotum   = VERUM;
    chorda classes;
    chorda lemma_plicatum;
    i32 k;

    c                = &census->classes[aurea];
    census->verba    = census->verba + I;
    c->verba         = c->verba + I;
    classes.datum    = NIHIL;
    classes.mensura  = ZEPHYRUM;
    lemma_plicatum = oratio_vocabularium_la_plicare(piscina,
        verbum->lemma);
    per (k = e0; k < e1; k++)
    {
        constans Elementum* e =
            (constans Elementum*)xar_obtinere(elementa, k);
                       i32 j;

        si (k == e0)
        {
            classes = e->classes;
        }
        si (!e->ignotum)
        {
            ignotum = FALSUM;
        }
        per (j = ZEPHYRUM; j < e->numerus_classium; j++)
        {
            si (e->classis[j] == aurea)
            {
                tectum = VERUM;
                si (j == ZEPHYRUM && k == e0 && primum)
                {
                    primaria = VERUM;
                }
            }
        }
        per (j = ZEPHYRUM; j < e->numerus_lemmatum; j++)
        {
            si (_aequalis(e->lemmata[j], lemma_plicatum))
            {
                lemma = VERUM;
            }
        }
    }
    si (e1 <= e0)
    {
        ignotum = FALSUM;
    }
    si (tectum)
    {
        census->tecta  = census->tecta + I;
        c->tecta       = c->tecta + I;
    }
    alioquin
    {
        _exemplum(c, piscina, verbum->forma, classes, verbum->lemma);
    }
    si (primaria)
    {
        census->primaria  = census->primaria + I;
        c->primaria       = c->primaria + I;
    }
    si (lemma)
    {
        census->lemmata  = census->lemmata + I;
        c->lemmata       = c->lemmata + I;
    }
    si (ignotum)
    {
        census->ignota  = census->ignota + I;
        c->ignota       = c->ignota + I;
    }
}

interior b32
_sententiam_iudicare (
                           Piscina* piscina,
     constans OratioVocabulariumLa* voc,
    constans OratioConlluSententia* s,
              OratioOraculumCensus* census)
{
    Piscina* scratch = piscina_generare_dynamicum("oraculum_sententia",
        16777216);
    chorda textus;
    MateriaNodus* doc;
    Xar* elementa;
    i32 cursor = ZEPHYRUM;
    i32 k;
    i32 n;
    i32 e_proximum = ZEPHYRUM;

    si (scratch == NIHIL)
    {
        redde FALSUM;
    }
    census->sententiae  = census->sententiae + I;
    textus              = oratio_conllu_textus(scratch, s);
    doc = textus.datum != NIHIL
        ? oratio_arbor_parsare(scratch,
        (constans character*)textus.datum,
            textus.mensura)
        : NIHIL;
    elementa = xar_creare(scratch, (i32)magnitudo(Elementum));
    si (   doc == NIHIL || elementa == NIHIL
        || !oratio_partes_annotare(scratch, voc, doc, NIHIL)
        || !_elementa_colligere(scratch, voc, doc, elementa))
    {
        census->sententiae_fractae = census->sententiae_fractae + I;
        piscina_destruere(scratch);
        redde VERUM;
    }
    n = xar_numerus(s->lexemata);
    per (k = ZEPHYRUM; k < n; k++)
    {
        constans OratioConlluLexema* t =
            (constans OratioConlluLexema*)xar_obtinere(s->lexemata, k);
        s32 ga;
        s32 gb;
        i32 e0;
        i32 e1;
        i32 verba_rangae = t->ranga ? t->b - t->a + I : ZEPHYRUM;
        i32 w;

        si (t->ranga)
        {
            census->rangae = census->rangae + I;
        }
        ga = _formam_invenire(textus, cursor, t->forma);
        si (ga < ZEPHYRUM)
        {
            /* forma in textu absens: verba inalignata, cursor manet */
            per (w = ZEPHYRUM; w <= verba_rangae; w++)
            {
                constans OratioConlluLexema* v =
                    (constans OratioConlluLexema*)
                    xar_obtinere(s->lexemata, k + (t->ranga ? w
                        + I : ZEPHYRUM));

                si (!t->ranga && w > ZEPHYRUM)
                {
                    frange;
                }
                si (t->ranga && w == verba_rangae)
                {
                    frange;
                }
                si (k + (t->ranga ? w + I : ZEPHYRUM) < n)
                {
                    OratioClassis aurea =
                        oratio_oraculum_classis_ex_upos(
                        v->upos);

                    census->verba       = census->verba + I;
                    census->inalignata  = census->inalignata + I;
                    census->classes[aurea].verba =
                        census->classes[aurea].verba + I;
                    census->classes[aurea].inalignata =
                        census->classes[aurea].inalignata + I;
                }
            }
            k = k + verba_rangae;
            perge;
        }
        gb      = ga + (s32)t->forma.mensura;
        cursor  = (i32)gb;
        /* elementa quae extentum [ga, gb) tangunt */
        dum (   e_proximum < xar_numerus(elementa)
             && ((constans Elementum*)xar_obtinere(elementa,
            e_proximum))->b
                <= ga)
        {
            e_proximum = e_proximum + I;
        }
        e0 = e_proximum;
        e1 = e0;
        dum (   e1 < xar_numerus(elementa)
             && ((constans Elementum*)xar_obtinere(elementa, e1))->a
                < gb)
        {
            e1 = e1 + I;
        }
        si (t->ranga)
        {
            per (w = ZEPHYRUM; w < verba_rangae && k + I + w < n; w++)
            {
                _verbum_iudicare(piscina, census, elementa, e0, e1,
                    (constans OratioConlluLexema*)xar_obtinere(s->lexemata,
                        k + I + w), (b32)(w == ZEPHYRUM));
            }
            k = k + verba_rangae;
        }
        alioquin
        {
            _verbum_iudicare(piscina, census, elementa, e0, e1, t,
                VERUM);
        }
    }
    piscina_destruere(scratch);
    redde VERUM;
}

b32
oratio_oraculum_iudicare (
                          Piscina* piscina,
    constans OratioVocabulariumLa* voc,
                              Xar* sententiae,
             OratioOraculumCensus* census)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(sententiae); i++)
    {
        si (!_sententiam_iudicare(piscina, voc,
                (constans OratioConlluSententia*)xar_obtinere(sententiae,
                i),
                census))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

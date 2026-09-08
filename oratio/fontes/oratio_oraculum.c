/* oratio_oraculum.c - Vide oratio_oraculum.h. */

#include "oratio_oraculum.h"
#include "oratio_lexicon.h"
#include "oratio_stml.h"
#include "materia_lexicon.h"
#include "internamentum.h"
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
    census->lingua_documenti = (s32)-I;
}

/* discrepantiam notare (T19a): clavis 'aurea/nostra/forma plicata' in
 * indice; cella nova in Xar (cellae xar numquam moventur - monstrator
 * in indice stabilis) aut numerus cellae inventae auctus. Forma
 * monstrosa (> CC octeti) non tabulatur - numquam ruit. */
interior vacuum
_discrepantiam_notare (
                 Piscina* piscina,
    OratioOraculumCensus* census,
           OratioClassis  aurea,
                  chorda  forma,
           OratioClassis  nostra)
{
                     character*  clavis_datum;
                        chorda   plicata;
                        chorda   clavis;
                        vacuum*  valor;
    OratioOraculumDiscrepantia*  d;
    OratioOraculumDiscrepantia** cella;
                           i32   praefixum;

    si (census->discrepantiae == NIHIL)
    {
        census->discrepantiae = xar_creare(piscina,
            (i32)magnitudo(OratioOraculumDiscrepantia*));
        census->discrepantiae_index = tabula_dispersa_creare_chorda(
            piscina, (i32)1024);
        si (   census->discrepantiae       == NIHIL
            || census->discrepantiae_index == NIHIL)
        {
            census->discrepantiae = NIHIL;
            redde;
        }
    }
    plicata = oratio_vocabularium_la_plicare(piscina, forma);
    /* clavis binaria in piscina (forma quaelibet - URL aurea PROPN
     * CCXL octetorum lex summae bis fregit sub tabula stationaria):
     * octetus classis aureae, octetus nostrae, forma; chorda mensuram
     * fert, octeti nulli licent; sine stdio */
    praefixum    = (i32)II;
    clavis_datum = (character*)piscina_allocare(piscina,
        (memoriae_index)(plicata.mensura + praefixum));
    si (plicata.datum == NIHIL || clavis_datum == NIHIL)
    {
        redde;
    }
    clavis_datum[ZEPHYRUM]  = (character)(i32)aurea;
    clavis_datum[I]         = (character)(i32)nostra;
    memcpy(clavis_datum + praefixum, plicata.datum,
        (size_t)plicata.mensura);
    clavis.datum    = (i8*)clavis_datum;
    clavis.mensura  = (i32)praefixum + plicata.mensura;
    si (tabula_dispersa_invenire(census->discrepantiae_index, clavis,
            &valor))
    {
        d           = (OratioOraculumDiscrepantia*)valor;
        d->numerus  = d->numerus + I;
        redde;
    }
    d = (OratioOraculumDiscrepantia*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(*d));
    cella = (OratioOraculumDiscrepantia**)xar_addere(
        census->discrepantiae);
    si (d == NIHIL || cella == NIHIL)
    {
        redde;
    }
    d->aurea    = aurea;
    d->nostra   = nostra;
    d->forma    = plicata;
    d->numerus  = I;
    *cella      = d;
    (vacuum)tabula_dispersa_inserere(census->discrepantiae_index,
        clavis, d);
}

/* ordo: numerus maior prior, deinde forma octetim, deinde classis */
interior s32
_discrepantias_comparare (
    constans vacuum* a,
    constans vacuum* b)
{
    constans OratioOraculumDiscrepantia* x =
        *(OratioOraculumDiscrepantia* constans*)a;
    constans OratioOraculumDiscrepantia* y =
        *(OratioOraculumDiscrepantia* constans*)b;
    i32 minima;
    s32 c;

    si (x->numerus != y->numerus)
    {
        redde x->numerus > y->numerus ? (s32)-I : (s32)I;
    }
    minima = x->forma.mensura < y->forma.mensura ? x->forma.mensura
        : y->forma.mensura;
    c = (s32)memcmp(x->forma.datum, y->forma.datum, (size_t)minima);
    si (c != ZEPHYRUM)
    {
        redde c < ZEPHYRUM ? (s32)-I : (s32)I;
    }
    si (x->forma.mensura != y->forma.mensura)
    {
        redde x->forma.mensura < y->forma.mensura ? (s32)-I : (s32)I;
    }
    si ((i32)x->nostra != (i32)y->nostra)
    {
        redde (i32)x->nostra < (i32)y->nostra ? (s32)-I : (s32)I;
    }
    redde ZEPHYRUM;
}

Xar*
oratio_oraculum_discrepantiae (
                          Piscina* piscina,
    constans OratioOraculumCensus* census,
                    OratioClassis  aurea)
{
    Xar* exitus;
    i32  i;
    i32  n;

    exitus = xar_creare(piscina,
        (i32)magnitudo(OratioOraculumDiscrepantia*));
    si (exitus == NIHIL)
    {
        redde NIHIL;
    }
    si (census->discrepantiae == NIHIL)
    {
        redde exitus;
    }
    n = xar_numerus(census->discrepantiae);
    per (i = ZEPHYRUM; i < n; i++)
    {
        OratioOraculumDiscrepantia* d =
            *(OratioOraculumDiscrepantia**)xar_obtinere(
                census->discrepantiae, i);
        OratioOraculumDiscrepantia** cella;

        si (d == NIHIL || d->aurea != aurea)
        {
            perge;
        }
        cella = (OratioOraculumDiscrepantia**)xar_addere(exitus);
        si (cella == NIHIL)
        {
            redde NIHIL;
        }
        *cella = d;
    }
    xar_ordinare(exitus, _discrepantias_comparare);
    redde exitus;
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
        s32 decisio;                       /* OratioDecisio; -I = nemo (T19g) */
    i32 numerus_analysium;             /* lectiones (candidata) */
    chorda auctor;                     /* titulus regulae decidentis (T19g) */
} Elementum;

constans character* constans ORATIO_ORACULUM_TITULI_PARTITIONIS[] = {
    "praelatio", "impletio", "umbra", "una", "aperta", "nulla", "ranga"
};

/* auctorem notare (T19g): clavis = titulus (copia in piscina iudicii
 * ut clavis stabilis sit - titulus elementi in piscina sententiae
 * vivit); cella nova aut numeri aucti. Ut _discrepantiam_notare. */
interior vacuum
_auctorem_notare (
                 Piscina* piscina,
    OratioOraculumCensus* census,
                  chorda  titulus,
                     b32  primaria)
{
                   vacuum*  valor;
     OratioOraculumAuctor*  a;
     OratioOraculumAuctor** cella;
                   chorda   clavis;

    si (census->auctores == NIHIL)
    {
        census->auctores = xar_creare(piscina,
            (i32)magnitudo(OratioOraculumAuctor*));
        census->auctores_index = tabula_dispersa_creare_chorda(piscina,
            (i32)256);
        si (   census->auctores       == NIHIL
            || census->auctores_index == NIHIL)
        {
            census->auctores = NIHIL;
            redde;
        }
    }
    si (tabula_dispersa_invenire(census->auctores_index, titulus,
        &valor))
    {
        a         = (OratioOraculumAuctor*)valor;
        a->verba  = a->verba + I;
        si (primaria)
        {
            a->primaria = a->primaria + I;
        }
        redde;
    }
    clavis = _copia(piscina, titulus);
    a      = (OratioOraculumAuctor*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(*a));
    cella  = (OratioOraculumAuctor**)xar_addere(census->auctores);
    si (clavis.datum == NIHIL || a == NIHIL || cella == NIHIL)
    {
        redde;
    }
    a->titulus   = clavis;
    a->verba     = I;
    a->primaria  = primaria ? I : ZEPHYRUM;
    *cella       = a;
    (vacuum)tabula_dispersa_inserere(census->auctores_index, clavis, a);
}

interior s32
_auctores_comparare (
    constans vacuum* p,
    constans vacuum* q)
{
    constans OratioOraculumAuctor* x =
        *(constans OratioOraculumAuctor* constans*)p;
    constans OratioOraculumAuctor* y =
        *(constans OratioOraculumAuctor* constans*)q;

    si (x->verba != y->verba)
    {
        redde x->verba > y->verba ? (s32)-I : (s32)I;
    }
    redde chorda_comparare(x->titulus, y->titulus);
}

Xar*
oratio_oraculum_auctores (
                          Piscina* piscina,
    constans OratioOraculumCensus* census)
{
    Xar* exitus = xar_creare(piscina,
        (i32)magnitudo(OratioOraculumAuctor*));
    i32 i;

    si (exitus == NIHIL)
    {
        redde NIHIL;
    }
    si (census->auctores == NIHIL)
    {
        redde exitus;
    }
    per (i = ZEPHYRUM; i < xar_numerus(census->auctores); i++)
    {
        OratioOraculumAuctor** cella =
            (OratioOraculumAuctor**)xar_addere(exitus);

        si (cella == NIHIL)
        {
            redde NIHIL;
        }
        *cella = *(OratioOraculumAuctor**)xar_obtinere(census->auctores,
            i);
    }
    xar_ordinare(exitus, _auctores_comparare);
    redde exitus;
}

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
    e->a        = (s32)-I;
    e->b        = ZEPHYRUM;
    e->decisio  = (s32)-I;
    si (n->genus == (s32)ORATIO_GENUS_VOCABULUM)
    {
        constans MateriaValor* classes =
            &n->loci[ORATIO_VOCABULUM_CLASSES];
        constans MateriaValor* analyses =
            &n->loci[ORATIO_VOCABULUM_ANALYSES];
        constans MateriaValor* decisio =
            &n->loci[ORATIO_VOCABULUM_DECISIO];

                /* T19g: decisio, auctor et numerus lectionum pro partitione */
        si (decisio->genus == MATERIA_VALOR_INDEX)
        {
            constans MateriaValor* auctor =
                &n->loci[ORATIO_VOCABULUM_AUCTOR];

            e->decisio = decisio->datum.index;
            si (   auctor->genus       == MATERIA_VALOR_TOKEN
                && auctor->datum.token != NIHIL)
            {
                e->auctor = auctor->datum.token->valor;
            }
        }

        si (analyses->genus == MATERIA_VALOR_LISTA)
        {
            e->numerus_analysium =
                materia_valor_lista_numerus(*analyses);
        }


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
        /* signum = interpunctio aut symbolum (UD PUNCT / SYM: '-'
         * utrumque, '$' '/' ':-)' SYM): candidata ambo, interpunctio
         * prima (T16) */
        hic_manens character signa[] = "interpunctio symbolum";

        _extentum_listae(&n->loci[ORATIO_INTERPUNCTIO_SIGNUM], &e->a,
            &e->b);

                e->classis[ZEPHYRUM]  = ORATIO_CLASSIS_INTERPUNCTIO;
        e->classis[I]                 = ORATIO_CLASSIS_SYMBOLUM;
        e->numerus_classium           = II;
        e->numerus_analysium          = II;   /* candidata duo, nemo decidit */

        e->classes = _copia(piscina, _chorda((i8*)signa,
            (i32)(magnitudo(signa) - I)));
        redde VERUM;
    }
    si (n->genus == (s32)ORATIO_GENUS_NUMERUS)
    {
        _extentum_listae(&n->loci[ORATIO_NUMERUS_CRUDUM], &e->a, &e->b);
                e->classis[ZEPHYRUM]  = ORATIO_CLASSIS_NUMERALE;
        e->numerus_classium           = I;
        e->numerus_analysium          = I;

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
            OratioClassis  aurea;
    OratioOraculumClassis* c;
                      b32  tectum    = FALSUM;
                      b32  primaria  = FALSUM;
                      b32  lemma     = FALSUM;
                      b32  ignotum   = VERUM;
                   chorda  classes;
                   chorda  lemma_plicatum;
                      i32  k;

    aurea            = oratio_oraculum_classis_ex_upos(verbum->upos);
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
        /* T19g: partitio decisionum - verbum aureum QUODQUE per genus
     * decisionis elementi primi sui (nulla sine elemento, ranga = verbum
     * rangae non primum); auctor per regulam. Lex summae in porta:
     * partitio tota == verba - inalignata. */
    {
        constans Elementum* e = e1 > e0
            ? (constans Elementum*)xar_obtinere(elementa, e0) : NIHIL;
                       i32 p;

        si (e == NIHIL)
        {
            p = ORATIO_ORACULUM_PARTITIO_NULLA;
        }
        alioquin si (!primum)
        {
            p = ORATIO_ORACULUM_PARTITIO_RANGA;
        }
        alioquin si (   e->decisio >= ZEPHYRUM
                     && e->decisio < (s32)ORATIO_DECISIO_NUMERUS)
        {
            p = (i32)e->decisio;
            si (e->auctor.mensura > ZEPHYRUM)
            {
                _auctorem_notare(piscina, census, e->auctor, primaria);
            }
        }
        alioquin si (e->numerus_analysium == I)
        {
            p = ORATIO_ORACULUM_PARTITIO_UNA;
        }
        alioquin si (e->numerus_analysium > I)
        {
            p = ORATIO_ORACULUM_PARTITIO_APERTA;
        }
        alioquin
        {
            p = ORATIO_ORACULUM_PARTITIO_NULLA;
        }
        census->partitio_verba[p] = census->partitio_verba[p] + I;
        si (primaria)
        {
            census->partitio_primaria[p] =
                census->partitio_primaria[p] + I;
        }
    }


    /* T19a: tectum sed non primum = discrepantia (classis nostra prima
     * elementi primi); lex summae: tecta - primaria per classem */
    si (tectum && !primaria && e1 > e0)
    {
        constans Elementum* primum_elementum =
            (constans Elementum*)xar_obtinere(elementa, e0);

        _discrepantiam_notare(piscina, census, aurea, verbum->forma,
            primum_elementum->numerus_classium > ZEPHYRUM
                ? primum_elementum->classis[ZEPHYRUM]
                : ORATIO_CLASSIS_NUMERUS_CLASSIUM);
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

/* resolutio optionalis (T17): programma + regulae + intern + ratum */
nomen structura {
       constans OratioProgramma* programma;
                            s32  regulae_numerus;
            InternamentumChorda* intern;
   constans MateriaLexiconRatum* ratum;
             constans character* lingua_documenti;   /* NIHIL = census
                                                  * sententiae solius */
} Resolutio;

interior b32
_sententiam_iudicare (
                           Piscina* piscina,
        constans OratioVocabularia* vocabularia,
    constans OratioConlluSententia* s,
              OratioOraculumCensus* census,
                constans Resolutio* resolutio)
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
    i32 lingua_index;
    OratioPartesCensus census_partium;

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
            || !oratio_partes_annotare(scratch, vocabularia, doc,
            &census_partium))
        {
        census->sententiae_fractae = census->sententiae_fractae + I;
        piscina_destruere(scratch);
        redde VERUM;
        }
    /* census linguae sententiae (T18): relatus per thesaurum - regula
     * linguae solum sententias Anglicas iudicatas tangit */
        lingua_index = oratio_resolutio_lingua_censu_index(
            census_partium.vocabula_linguarum);
                census->sententiae_linguae[lingua_index] =
                    census->sententiae_linguae[lingua_index] + I;
        si (   (   resolutio != NIHIL && resolutio->programma != NIHIL
            && !oratio_resolutio_applicare(scratch, resolutio->intern,
                resolutio->ratum, resolutio->programma,
                resolutio->regulae_numerus,
                resolutio->lingua_documenti != NIHIL
                    ? resolutio->lingua_documenti
                    : ORATIO_TITULI_LINGUARUM[lingua_index], doc,
                NIHIL))
            || !_elementa_colligere(scratch, vocabularia->la, doc,
            elementa))
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
       constans OratioVocabularia* vocabularia,
                              Xar* sententiae,
             OratioOraculumCensus* census)
{
    redde oratio_oraculum_iudicare_resolutum(piscina, vocabularia,
        NIHIL,
        (s32)-I, sententiae, census);
}

/* census linguae DOCUMENTI (T18): thesaurus totus annotatur semel ante
 * iudicium, suffragia vocabulorum summantur, lingua = plurima (Latina
 * in aequalitate). Sententia sola fallit (EWT: CXXXIV sententiae 'I
 * have.', nomina sola, lineae sine verbis Latinae iudicatae); documentum
 * totum non fallit - et ita instrumenta (arbor, verba) censent. Pretium:
 * parsura + annotatio bis per sententiam (~I s per thesaurum). */
interior i32
_linguam_documenti_censere (
                       Piscina* piscina,
    constans OratioVocabularia* vocabularia,
                           Xar* sententiae,
          OratioOraculumCensus* census)
{
    i32 i;
    i32 k;

    (vacuum)piscina;
    per (i = ZEPHYRUM; i < xar_numerus(sententiae); i++)
    {
        constans OratioConlluSententia* s =
            (constans OratioConlluSententia*)xar_obtinere(sententiae,
            i);
        Piscina* scratch = piscina_generare_dynamicum(
            "oraculum_census_linguae", 16777216);
        chorda textus;
        MateriaNodus* doc;
        OratioPartesCensus census_partium;

        si (scratch == NIHIL)
        {
            redde ZEPHYRUM;
        }
        textus = oratio_conllu_textus(scratch, s);
        doc = textus.datum != NIHIL
            ? oratio_arbor_parsare(scratch,
            (constans character*)textus.datum, textus.mensura)
            : NIHIL;
        si (   doc != NIHIL
            && oratio_partes_annotare(scratch, vocabularia, doc,
                &census_partium))
        {
            per (k = ZEPHYRUM; k < (i32)ORATIO_LINGUA_NUMERUS; k++)
            {
                census->suffragia_linguarum[k] =
                    census->suffragia_linguarum[k]
                    + census_partium.vocabula_linguarum[k];
            }
        }
        piscina_destruere(scratch);
    }
    redde oratio_resolutio_lingua_censu_index(
        census->suffragia_linguarum);
}

b32
oratio_oraculum_iudicare_resolutum (
                          Piscina* piscina,
       constans OratioVocabularia* vocabularia,
         constans OratioProgramma* programma,
                              s32  regulae_numerus,
                              Xar* sententiae,
             OratioOraculumCensus* census)
{
              Resolutio resolutio;
    MateriaLexiconRatum ratum;
     MateriaLexIudicium iudicium;
                    i32 i;

    resolutio.programma         = programma;
    resolutio.regulae_numerus   = regulae_numerus;
    resolutio.intern            = NIHIL;
    resolutio.ratum             = NIHIL;
    resolutio.lingua_documenti  = NIHIL;
    si (programma != NIHIL)
    {
        resolutio.intern = internamentum_creare(piscina);
        si (   resolutio.intern == NIHIL
            || !materia_lexicon_ratum_facere(&ratum, &ORATIO_LEXICON,
                &iudicium))
        {
            redde FALSUM;
        }
        resolutio.ratum = &ratum;
        census->lingua_documenti = (s32)_linguam_documenti_censere(
            piscina, vocabularia, sententiae, census);
        resolutio.lingua_documenti =
            ORATIO_TITULI_LINGUARUM[(i32)census->lingua_documenti];
    }
    per (i = ZEPHYRUM; i < xar_numerus(sententiae); i++)
    {
        si (!_sententiam_iudicare(piscina, vocabularia,
                (constans OratioConlluSententia*)xar_obtinere(sententiae,
                i),
                census, programma != NIHIL ? &resolutio : NIHIL))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

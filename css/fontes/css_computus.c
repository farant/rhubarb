/* css_computus.c - imago memoriae et temporis parseris CSS (vide .h) */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "css_arbor.h"
#include "css_registrum.h"
#include "css_lexicon.h"
#include "materia_nodus.h"
#include "materia_scribere.h"
#include "materia_arbor.h"
#include "materia_lexicon.h"
#include "css_computus.h"
#include <string.h>
#include <time.h>

interior duplex
_ms (
    clock_t a,
    clock_t b)
{
    redde (duplex)(b - a) * 1000.0 / (duplex)CLOCKS_PER_SEC;
}

/* nodos et lexemata arboris numerare: loci NODUS/TOKEN et elementa
 * listarum (idem mos ac probatio_css_corpus) */
interior vacuum
_numerare (
    constans MateriaNodus* nodus,
                      i32* nodi,
                      i32* lexemata)
{
    i32 i;

    si (nodus == NIHIL)
    {
        redde;
    }
    (*nodi)++;
    per (i = ZEPHYRUM; i < nodus->numerus_locorum; i++)
    {
        constans MateriaValor* v = &nodus->loci[i];

        si (v->genus == MATERIA_VALOR_NODUS)
        {
            _numerare(v->datum.nodus, nodi, lexemata);
        }
        alioquin si (v->genus == MATERIA_VALOR_TOKEN)
        {
            (*lexemata)++;
        }
        alioquin si (v->genus == MATERIA_VALOR_LISTA)
        {
            i32 n = materia_valor_lista_numerus(*v);
            i32 j;

            per (j = ZEPHYRUM; j < n; j++)
            {
                constans MateriaValor* e =
                    materia_valor_lista_obtinere(*v, j);

                si (e == NIHIL)
                {
                    perge;
                }
                si (e->genus == MATERIA_VALOR_NODUS)
                {
                    _numerare(e->datum.nodus, nodi, lexemata);
                }
                alioquin si (e->genus == MATERIA_VALOR_TOKEN)
                {
                    (*lexemata)++;
                }
            }
        }
    }
}

b32
css_computus_metiri (
    constans character* fons,
                   i32  mensura,
           CssComputus* exitus)
{
         Piscina* piscina;
    MateriaNodus* radix;
         clock_t  t0;
         clock_t  t1;
             b32  octeti_idem;
             b32  arbor_idem;

    si (exitus == NIHIL)
    {
        redde FALSUM;
    }
    memset(exitus, 0, magnitudo(*exitus));
    si (fons == NIHIL)
    {
        redde FALSUM;
    }
    exitus->octeti_fontis = mensura;

    /* I. parsura - piscina propria; numeri memoriae parsurae SOLIUS */
    piscina = piscina_generare_dynamicum("css_computus_parsare",
        CSS_COMPUTUS_ALVEUS_INITIUM);
    si (piscina == NIHIL)
    {
        redde FALSUM;
    }
    t0                    = clock();
    radix                 = css_arbor_parsare(piscina, fons, mensura);
    t1                    = clock();
    exitus->ms_parsandi   = _ms(t0, t1);
    exitus->usus          = piscina_summa_usus(piscina);
    exitus->otiosa        = piscina_summa_inutilis_allocatus(piscina);
    exitus->commissa      = exitus->usus + exitus->otiosa;
    exitus->apex          = piscina_summa_apex_usus(piscina);
    exitus->alvei         = piscina_numerus_alveorum(piscina);
    exitus->allocationes  = piscina_numerus_allocationum(piscina);
    si (radix == NIHIL)
    {
        piscina_destruere(piscina);
        redde FALSUM;
    }
    _numerare(radix, &exitus->nodi, &exitus->lexemata);

    /* II. emissio, STML bis, comparator - piscina altera */
    octeti_idem  = FALSUM;
    arbor_idem   = FALSUM;
    {
                           Piscina* altera;
         MateriaScripturaConsilium  cs;
                  MateriaScriptura  emissa;
               MateriaLexiconRatum  ratum;
                MateriaLexIudicium  iudicium;
             MateriaArborConsilium  consilium;
             MateriaArborScriptura  s;
                     MateriaNodus*  lecta;
                MateriaArborVitium  vitium;
           MateriaArborDifferentia  d;

        altera = piscina_generare_dynamicum("css_computus_arbor",
            CSS_COMPUTUS_ALVEUS_INITIUM);
        si (altera == NIHIL)
        {
            piscina_destruere(piscina);
            redde FALSUM;
        }
        materia_scriptura_consilium_nudum(&cs, &CSS_REGISTRUM);
        t0 = clock();
        emissa = materia_scribere_nodum(altera, radix, &cs);
        t1 = clock();
        exitus->ms_emittendi = _ms(t0, t1);
        octeti_idem = emissa.successus
                   && emissa.textus.mensura == mensura
                   && (   mensura == ZEPHYRUM
                       || memcmp(emissa.textus.datum, fons,
                              (memoriae_index)mensura) == ZEPHYRUM);
        si (materia_lexicon_ratum_facere(&ratum, &CSS_LEXICON,
            &iudicium))
        {
            materia_arbor_consilium_nudum(&consilium, &CSS_REGISTRUM,
                &ratum, "css");
            t0 = clock();
            s = materia_arbor_scribere_nodum(altera, radix,
                &consilium);
            t1                          = clock();
            exitus->ms_arbor_scribendi  = _ms(t0, t1);
            si (s.successus)
            {
                exitus->octeti_stml  = s.textus.mensura;
                t0                   = clock();
                lecta = materia_arbor_legere(altera, NIHIL, s.textus,
                    &consilium, &vitium);
                t1                        = clock();
                exitus->ms_arbor_legendi  = _ms(t0, t1);
                si (lecta != NIHIL)
                {
                    t0 = clock();
                    arbor_idem = materia_arbor_aequalis(radix, lecta,
                        MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &d);
                    t1                     = clock();
                    exitus->ms_comparandi  = _ms(t0, t1);
                }
            }
        }
        exitus->allocationes_arboris =
            piscina_numerus_allocationum(altera);
        piscina_destruere(altera);
    }
    exitus->successus = octeti_idem && arbor_idem;
    piscina_destruere(piscina);
    redde exitus->successus;
}

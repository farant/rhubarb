/* oratio_computus.c - vide oratio_computus.h (gemellus md_computus.c; cliens quartus) */

#include "oratio_computus.h"
#include "oratio_arbor.h"
#include "oratio_stml.h"
#include "oratio_registrum.h"
#include "oratio_lexicon.h"
#include "materia_nodus.h"
#include "materia_scribere.h"
#include "materia_arbor.h"
#include "materia_lexicon.h"
#include <string.h>
#include <time.h>

interior duplex
_ms (
    clock_t a,
    clock_t b)
{
    redde (duplex)(b - a) * 1000.0 / (duplex)CLOCKS_PER_SEC;
}

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
oratio_computus_metiri (
    constans character* fons,
                   i32  mensura,
            OratioComputus* exitus)
{
         Piscina* piscina;
    MateriaNodus* radix;
         clock_t  t0;
         clock_t  t1;
             b32  octeti_idem = FALSUM;
             b32  arbor_idem  = FALSUM;

    si (exitus == NIHIL)
    {
        redde FALSUM;
    }
    memset(exitus, ZEPHYRUM, magnitudo(*exitus));
    si (fons == NIHIL)
    {
        redde FALSUM;
    }
    exitus->octeti_fontis = mensura;
    piscina = piscina_generare_dynamicum("oratio_computus_parsare",
        ORATIO_COMPUTUS_ALVEUS_INITIUM);
    si (piscina == NIHIL)
    {
        redde FALSUM;
    }
    t0 = clock();
    radix = oratio_arbor_parsare(piscina, fons,
        mensura);
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
    {
                          Piscina* altera;
        MateriaScripturaConsilium  cs;
                 MateriaScriptura  emissa;
              MateriaLexiconRatum  ratum;
               MateriaLexIudicium  iudicium;
            MateriaArborConsilium  consilium;
            MateriaArborScriptura  s;
                    MateriaNodus* lecta;
               MateriaArborVitium vitium;
          MateriaArborDifferentia d;

        altera = piscina_generare_dynamicum("oratio_computus_arbor",
            ORATIO_COMPUTUS_ALVEUS_INITIUM);
        si (altera == NIHIL)
        {
            piscina_destruere(piscina);
            redde FALSUM;
        }
        materia_scriptura_consilium_nudum(&cs, &ORATIO_REGISTRUM);
        cs.fons_index = ORATIO_FONS_PLAGULAE;
        t0 = clock();
        emissa = materia_scribere_nodum(altera, radix, &cs);
        t1 = clock();
        exitus->ms_emittendi = _ms(t0, t1);
        octeti_idem = (b32)(emissa.successus
            && emissa.textus.mensura == mensura
            && (mensura == ZEPHYRUM
                || memcmp(emissa.textus.datum, fons,
                (memoriae_index)mensura) == ZEPHYRUM));
        si (materia_lexicon_ratum_facere(&ratum, &ORATIO_LEXICON,
            &iudicium))
        {
            oratio_stml_consilium(&consilium, &ratum);
            t0 = clock();
            s = materia_arbor_scribere_nodum(altera, radix, &consilium);
            t1 = clock();
            exitus->ms_arbor_scribendi = _ms(t0, t1);
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
    exitus->successus = (b32)(octeti_idem && arbor_idem);
    piscina_destruere(piscina);
    redde exitus->successus;
}

/* toml_computus.c - imago memoriae et temporis clientis toml (vide .h)
 *
 * Numeratio nodorum ITERATIVA (Q11a: instrumentum super plagulam
 * profundam non ruat) - idem mos numerandi ac crusta_computus: loci
 * NODUS/TOKEN et elementa listarum; trivia non numerantur.
 */
#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "toml_arbor.h"
#include "toml_coctum.h"
#include "toml_lexicon.h"
#include "toml_registrum.h"
#include "materia_nodus.h"
#include "materia_scribere.h"
#include "materia_arbor.h"
#include "materia_lexicon.h"
#include "toml_computus.h"
#include <string.h>
#include <time.h>

interior f64
_ms (
    clock_t a,
    clock_t b)
{
    redde (f64)(b - a) * 1000.0 / (f64)CLOCKS_PER_SEC;
}

/* nodos et lexemata numerare, acervo explicito */
interior vacuum
_numerare (
                  Piscina* piscina,
    constans MateriaNodus* radix,
                      i32* nodi,
                      i32* lexemata)
{
    Xar* acervus = xar_creare(piscina,
                       (i32)magnitudo(constans MateriaNodus*));
    constans MateriaNodus** locus;

    si (acervus == NIHIL || radix == NIHIL)
    {
        redde;
    }
    locus = (constans MateriaNodus**)xar_addere(acervus);
    si (locus == NIHIL)
    {
        redde;
    }
    *locus = radix;
    dum (xar_numerus(acervus) > ZEPHYRUM)
    {
        constans MateriaNodus* nodus = *(constans MateriaNodus**)
            xar_obtinere(acervus, xar_numerus(acervus) - I);
                          i32 i;

        xar_removere_ultimum(acervus);
        (*nodi)++;
        per (i = ZEPHYRUM; i < nodus->numerus_locorum; i++)
        {
            constans MateriaValor* v = &nodus->loci[i];

            si (v->genus == MATERIA_VALOR_NODUS)
            {
                locus = (constans MateriaNodus**)xar_addere(acervus);
                si (locus != NIHIL)
                {
                    *locus = v->datum.nodus;
                }
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
                        locus = (constans MateriaNodus**)xar_addere(
                            acervus);
                        si (locus != NIHIL)
                        {
                            *locus = e->datum.nodus;
                        }
                    }
                    alioquin si (e->genus == MATERIA_VALOR_TOKEN)
                    {
                        (*lexemata)++;
                    }
                }
            }
        }
    }
}

b32
toml_computus_metiri (
    constans character* fons,
                   i32  mensura,
          TomlComputus* exitus)
{
         Piscina* piscina;
    MateriaNodus* radix;
      TomlParsura r;
         clock_t  t0;
         clock_t  t1;
             b32  octeti_idem;
             b32  arbor_idem;

    si (exitus == NIHIL)
    {
        redde FALSUM;
    }
    memset(exitus, ZEPHYRUM, magnitudo(*exitus));
    si (fons == NIHIL && mensura > ZEPHYRUM)
    {
        redde FALSUM;
    }
    exitus->octeti_fontis = mensura;

    /* I. parsura - piscina propria; numeri memoriae parsurae SOLIUS */
    piscina = piscina_generare_dynamicum("toml_computus_parsare",
        TOML_COMPUTUS_ALVEUS_INITIUM);
    si (piscina == NIHIL)
    {
        redde FALSUM;
    }
    t0                    = clock();
    radix                 = toml_arbor_parsare(piscina, fons,
        (s32)mensura, &r);
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

    /* II. numeratio, emissio, STML bis, comparator, coctio - piscina
     * altera */
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
                        TomlCoctum  coctum;

        altera = piscina_generare_dynamicum("toml_computus_arbor",
            TOML_COMPUTUS_ALVEUS_INITIUM);
        si (altera == NIHIL)
        {
            piscina_destruere(piscina);
            redde FALSUM;
        }
        _numerare(altera, radix, &exitus->nodi, &exitus->lexemata);
        materia_scriptura_consilium_nudum(&cs, &TOML_REGISTRUM);
        t0 = clock();
        emissa = materia_scribere_nodum(altera, radix, &cs);
        t1 = clock();
        exitus->ms_emittendi = _ms(t0, t1);
        octeti_idem = emissa.successus
                   && emissa.textus.mensura == (i32)mensura
                   && (   mensura == ZEPHYRUM
                       || memcmp(emissa.textus.datum, fons,
                              (memoriae_index)mensura) == ZEPHYRUM);
        si (materia_lexicon_ratum_facere(&ratum, &TOML_LEXICON,
            &iudicium))
        {
            materia_arbor_consilium_nudum(&consilium, &TOML_REGISTRUM,
                &ratum, "toml");
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
        t0                   = clock();
        coctum               = toml_coquere(altera, radix, &r);
        t1                   = clock();
        exitus->ms_coquendi  = _ms(t0, t1);
        exitus->diagnostica = coctum.diagnostica != NIHIL
            ? xar_numerus(coctum.diagnostica) : ZEPHYRUM;
        exitus->allocationes_arboris =
            piscina_numerus_allocationum(altera);
        piscina_destruere(altera);
    }
    exitus->successus = octeti_idem && arbor_idem;
    piscina_destruere(piscina);
    redde exitus->successus;
}

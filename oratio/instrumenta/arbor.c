/* arbor.c - Proiectio STML plagulae orationis (oratio/arbor.sh)
 *
 * Usus: arbor <plagula.txt> [-tacitus] [-partes]
 * Legit plagulam, parsat (oratio_arbor), scribit documentum STML
 * (materia_arbor_scribere_nodum cum consilio oratio) in stdout.
 * -partes (T12): tabula Latina la.bin et glossarium ex RHUBARB_RADIX
 * onerantur et arbor annotatur (analyses, classes, linguae) ante
 * scripturam; cum -tacitus census annotationis. -tacitus: numerum
 * octetorum solum. Exitus: 0 sanum, 1 fractum, 2 usus/plagula/tabula
 * absens.
 */

#include "latina.h"
#include "oratio_arbor.h"
#include "oratio_stml.h"
#include "oratio_lexicon.h"
#include "oratio_partes.h"
#include "oratio_vocabularia.h"
#include "oratio_resolutio.h"
#include "internamentum.h"
#include <stdlib.h>
#include "materia_arbor.h"
#include "materia_lexicon.h"
#include "materia_nodus.h"
#include "piscina.h"
#include <stdio.h>
#include <string.h>

interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
         FILE* f;
        longus longitudo;
    character* memoria;
        size_t lecti;

    f = fopen(via, "rb");
    si (f == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(f);
        redde NIHIL;
    }
    longitudo = ftell(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    rewind(f);
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    si (lecti != (size_t)longitudo)
    {
        redde NIHIL;
    }
    *mensura = (i32)longitudo;
    redde memoria;
}

integer
principale (
      integer   argc,
    character** argv)
{
                 Piscina* piscina;
               character* textus;
                     i32  mensura = ZEPHYRUM;
            MateriaNodus* radix;
      MateriaLexiconRatum ratum;
       MateriaLexIudicium iudicium;
    MateriaArborConsilium consilium;
    MateriaArborScriptura s;
                                          b32 tacitus = FALSUM;
                     b32  partes  = FALSUM;
                     b32  crudus  = FALSUM;
                  integer i;
      constans character* via = NIHIL;

    per (i = I; i < argc; i++)
    {
        si (strcmp(argv[i], "-tacitus") == ZEPHYRUM)
        {
            tacitus = VERUM;
        }
        alioquin si (strcmp(argv[i], "-partes") == ZEPHYRUM)
        {
            partes = VERUM;   /* T12: analyses (tabula Latina + glossarium) */
        }
        alioquin si (strcmp(argv[i], "-crudus") == ZEPHYRUM)
        {
            crudus = VERUM;   /* T17: ordo fontis, sine resolutione */
        }
        alioquin
        {
            via = argv[i];
        }
    }
    si (via == NIHIL)
    {
        fprintf(stderr,
            "usus: arbor <plagula.txt> [-tacitus] [-partes]"
            " [-crudus]\n");
        redde II;
    }
    piscina = piscina_generare_dynamicum("oratio_arbor_instrumentum",
        4194304);
    textus = _plagulam_legere(piscina, via, &mensura);
    si (textus == NIHIL)
    {
        fprintf(stderr, "arbor: plagula absens: %s\n", via);
        redde II;
    }
    si (!materia_lexicon_ratum_facere(&ratum, &ORATIO_LEXICON,
        &iudicium))
    {
        fprintf(stderr, "arbor: lexicon recusatum\n");
        redde I;
    }
    oratio_stml_consilium(&consilium, &ratum);
        radix = oratio_arbor_parsare(piscina, textus, mensura);
    si (radix == NIHIL)
    {
        fprintf(stderr, "arbor: parsura fracta\n");
        redde I;
    }
    si (partes)
    {
        /* T12/T16: vocabularia (la.bin + glossarium + Moby) ex radice,
         * annotatio */
               OratioVocabularia vocabularia;
        OratioVocabulariumVitium vitium;
              OratioPartesCensus census;

        si (!oratio_vocabularia_onerare(piscina,
            getenv("RHUBARB_RADIX"),
                &vocabularia, &vitium))
        {
            fprintf(stderr,
                "arbor: vocabularia non onerata: %s:%d %s\n",
                vitium.plagula ? vitium.plagula : "?",
                (integer)vitium.linea,
                vitium.causa ? vitium.causa : "-");
            redde II;
        }
        si (!oratio_partes_annotare(piscina, &vocabularia, radix,
            &census))
        {
            fprintf(stderr, "arbor: annotatio fracta\n");
            redde I;
        }
        /* T17: resolutio (programma absens = nihil) */
        si (!crudus)
        {
            InternamentumChorda* intern = internamentum_creare(piscina);
                OratioProgramma* programma = intern == NIHIL ? NIHIL
                    : oratio_resolutio_programma_onerare(piscina,
                    intern,
                    getenv("RHUBARB_RADIX"), &vitium);

            si (   programma == NIHIL && vitium.causa != NIHIL
                && strcmp(vitium.causa, "plagula absens") != ZEPHYRUM)
            {
                fprintf(stderr,
                    "arbor: programma resolutionis: %s:%d %s\n",
                    vitium.plagula ? vitium.plagula : "?",
                    (integer)vitium.linea, vitium.causa);
                redde II;
            }
            si (   programma != NIHIL
                && !oratio_resolutio_applicare(piscina, intern, &ratum,
                    programma, (s32)-I,
                    oratio_resolutio_lingua_censu(census.linguae),
                    radix,
                    NIHIL))
            {
                fprintf(stderr, "arbor: resolutio fracta\n");
                redde I;
            }
        }
        si (tacitus)
        {
            imprimere("vocabula %d  analyses %d  ignota %d"
                "  latina %d  anglica %d\n",
                (integer)census.vocabula, (integer)census.analyses,
                (integer)census.ignota,
                (integer)census.linguae[ORATIO_LINGUA_LATINA],
                (integer)census.linguae[ORATIO_LINGUA_ANGLICA]);
        }
    }
    s = materia_arbor_scribere_nodum(piscina, radix, &consilium);
    si (!s.successus)
    {
        fprintf(stderr, "arbor: scriptura fracta: %s\n",
            s.causa ? s.causa : "-");
        redde I;
    }
    si (tacitus)
    {
        imprimere("%d octeti STML\n", (integer)s.textus.mensura);
    }
    alioquin
    {
        fwrite(s.textus.datum, I, (size_t)s.textus.mensura, stdout);
    }
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}

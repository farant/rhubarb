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
#include "oratio_vocabularium.h"
#include "oratio_vocabularium_la.h"
#include "oratio_glossarium.h"
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
                                          b32  tacitus = FALSUM;
                     b32  partes  = FALSUM;
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
        alioquin
        {
            via = argv[i];
        }
    }
    si (via == NIHIL)
    {
        fprintf(stderr,
            "usus: arbor <plagula.txt> [-tacitus] [-partes]\n");
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
        /* T12: tabula Latina (la.bin) + glossarium ex radice, annotatio */
        constans character* radix_viae = getenv("RHUBARB_RADIX");
                 character  via_tabulae[1024];
                 character* octeti;
                       i32  mensura_tabulae = ZEPHYRUM;
                    chorda  tabula;
    OratioVocabulariumLa* voc;
 OratioVocabulariumVitium vitium;
       OratioPartesCensus census;

        si (radix_viae == NIHIL)
        {
            radix_viae = ".";
        }
        sprintf(via_tabulae, "%s/oratio/vocabularium/la.bin",
            radix_viae);
        octeti = _plagulam_legere(piscina, via_tabulae,
            &mensura_tabulae);
        si (octeti == NIHIL)
        {
            fprintf(stderr, "arbor: tabula absens: %s\n", via_tabulae);
            redde II;
        }
        tabula.datum = (i8*)octeti;
        tabula.mensura = mensura_tabulae;
        voc = oratio_vocabularium_la_onerare(piscina, tabula, &vitium);
        si (voc == NIHIL)
        {
            fprintf(stderr, "arbor: onus tabulae fractum: %s\n",
                vitium.causa ? vitium.causa : "-");
            redde II;
        }
        sprintf(via_tabulae, "%s/oratio/glossarium.stml", radix_viae);
        octeti = _plagulam_legere(piscina, via_tabulae,
            &mensura_tabulae);
        si (octeti != NIHIL)
        {
            chorda fons_glossarii;
            OratioGlossarium* gl;

            fons_glossarii.datum    = (i8*)octeti;
            fons_glossarii.mensura  = mensura_tabulae;
            gl = oratio_glossarium_legere(piscina, fons_glossarii,
                &vitium);
            si (gl == NIHIL)
            {
                fprintf(stderr,
                    "arbor: glossarium non legitur: %s:%d %s\n",
                    vitium.plagula ? vitium.plagula : "?",
                    (integer)vitium.linea,
                    vitium.causa ? vitium.causa : "-");
                redde II;
            }
            oratio_vocabularium_la_glossarium_ponere(voc, gl);
        }
        si (!oratio_partes_annotare(piscina, voc, radix, &census))
        {
            fprintf(stderr, "arbor: annotatio fracta\n");
            redde I;
        }
        si (tacitus)
        {
            imprimere("vocabula %d  analyses %d  ignota %d\n",
                (integer)census.vocabula, (integer)census.analyses,
                (integer)census.ignota);
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

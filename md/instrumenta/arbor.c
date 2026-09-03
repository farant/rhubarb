/* arbor.c - Proiectio STML plagulae markdown (md/arbor.sh)
 *
 * Usus: arbor <plagula.md> [-tacitus]
 * Legit plagulam, parsat (md_arbor), scribit documentum STML
 * (materia_arbor_scribere_nodum cum consilio md) in stdout.
 * -tacitus: numerum octetorum solum. Exitus: 0 sanum, 1 fractum,
 * 2 usus/plagula absens.
 */

#include "latina.h"
#include "md_arbor.h"
#include "md_stml.h"
#include "md_lexicon.h"
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
                  integer i;
      constans character* via = NIHIL;

    per (i = I; i < argc; i++)
    {
        si (strcmp(argv[i], "-tacitus") == ZEPHYRUM)
        {
            tacitus = VERUM;
        }
        alioquin
        {
            via = argv[i];
        }
    }
    si (via == NIHIL)
    {
        fprintf(stderr, "usus: arbor <plagula.md> [-tacitus]\n");
        redde II;
    }
    piscina = piscina_generare_dynamicum("md_arbor_instrumentum",
        4194304);
    textus = _plagulam_legere(piscina, via, &mensura);
    si (textus == NIHIL)
    {
        fprintf(stderr, "arbor: plagula absens: %s\n", via);
        redde II;
    }
    si (!materia_lexicon_ratum_facere(&ratum, &MD_LEXICON, &iudicium))
    {
        fprintf(stderr, "arbor: lexicon recusatum\n");
        redde I;
    }
    md_stml_consilium(&consilium, &ratum);
    radix = md_arbor_parsare(piscina, textus, mensura);
    si (radix == NIHIL)
    {
        fprintf(stderr, "arbor: parsura fracta\n");
        redde I;
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

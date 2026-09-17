/* arbor.c - Proiectio STML plagulae HTML (html/arbor.sh)
 *
 * Usus: arbor <plagula.html> [-tacitus] [-sedes]
 * Legit plagulam, parsat (html_arbor), scribit documentum STML
 * (materia_arbor_scribere_nodum cum consilio html) in stdout.
 * -tacitus: numerum octetorum solum. Exitus: 0 sanum, 1 fractum,
 * 2 usus/plagula absens.
 * -sedes: VISIO sedium (sedes="L:C-L:C" octeti="B-B" in elementis;
 * lector eam recusat) - materia-sedes-spec par. III.
 */

#include "latina.h"
#include "html_arbor.h"
#include "html_lexicon.h"
#include "html_registrum.h"
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
      MateriaLexiconRatum  ratum;
       MateriaLexIudicium  iudicium;
    MateriaArborConsilium  consilium;
    MateriaArborScriptura  s;
                      b32  tacitus  = FALSUM;
                      b32  sedes    = FALSUM;
                  integer  i;
       constans character* via = NIHIL;

    per (i = I; i < argc; i++)
    {
        si (strcmp(argv[i], "-tacitus") == ZEPHYRUM)
        {
            tacitus = VERUM;
        }
        alioquin si (strcmp(argv[i], "-sedes") == ZEPHYRUM)
        {
            sedes = VERUM;   /* visio sedium (materia-sedes) */
        }
        alioquin
        {
            via = argv[i];
        }
    }
    si (via == NIHIL)
    {
        fprintf(stderr,
            "usus: arbor <plagula.html> [-tacitus] [-sedes]\n");
        redde II;
    }
    piscina = piscina_generare_dynamicum("html_arbor_instrumentum",
        4194304);
    textus = _plagulam_legere(piscina, via, &mensura);
    si (textus == NIHIL)
    {
        fprintf(stderr, "arbor: plagula absens: %s\n", via);
        redde II;
    }
    si (!materia_lexicon_ratum_facere(&ratum, &HTML_LEXICON, &iudicium))
    {
        fprintf(stderr, "arbor: lexicon recusatum\n");
        redde I;
    }
    materia_arbor_consilium_nudum(&consilium, &HTML_REGISTRUM, &ratum,
        "html");
    radix = html_arbor_parsare(piscina, textus, mensura);
    si (radix == NIHIL)
    {
        fprintf(stderr, "arbor: parsura fracta\n");
        redde I;
    }
    consilium.sedes_scribere = sedes;
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

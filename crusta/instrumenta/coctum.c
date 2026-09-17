/* coctum.c - Forma normalis plagulae crustae (crusta/coctum.sh)
 *
 * Usus: coctum <plagula.sh>
 * Legit plagulam, parsat (crusta_arbor), imprimit visionem coctam
 * (crusta_coctum: forma 'declare -f' bash 5.2.15, modus functionis
 * gradu zephyro) in stdout. Parsura non sana in stderr nominatur (mala,
 * clausurae absentes) sed forma tamen imprimitur. Exitus: 0 sanum, 1
 * fractum, 2 usus/plagula absens.
 */

#include "latina.h"
#include "crusta_arbor.h"
#include "crusta_coctum.h"
#include "crusta_registrum.h"
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
    CrustaParsura  relatio;
           chorda  coctum;

    si (argc != II)
    {
        fprintf(stderr, "usus: coctum <plagula.sh>\n");
        redde II;
    }
    piscina = piscina_generare_dynamicum("crusta_coctum_instrumentum",
        4194304);
    textus = _plagulam_legere(piscina, argv[I], &mensura);
    si (textus == NIHIL)
    {
        fprintf(stderr, "coctum: plagula absens: %s\n", argv[I]);
        redde II;
    }
    radix = crusta_arbor_parsare(piscina, textus, mensura, &CRUSTA_BASH,
        &relatio);
    si (radix == NIHIL)
    {
        fprintf(stderr, "coctum: parsura fracta\n");
        redde I;
    }
    si (!relatio.sana)
    {
        fprintf(stderr, "coctum: parsura non sana (mala %d, clausurae "
            "absentes %d)\n", (integer)relatio.mala,
            (integer)relatio.clausurae_absentes);
    }
    coctum = crusta_coctum_scribere(piscina, radix);
    si (coctum.datum == NIHIL)
    {
        fprintf(stderr, "coctum: memoria deficit\n");
        redde I;
    }
    fwrite(coctum.datum, I, (size_t)coctum.mensura, stdout);
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}

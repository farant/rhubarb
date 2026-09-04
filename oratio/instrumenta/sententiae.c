/* sententiae.c - sententiae orationis (oratio/sententiae.sh)
 *
 * Usus: sententiae <plagula.txt> [-machina]
 * Una sententia per lineam: '<initium>\t<finis>\t<linea>\t<textus>'
 * (lineae novae intra sententiam spatia fiunt); -machina addit caput
 * '#'. Exitus: 0 sententiae, 1 nullae, 2 usus/plagula/parsura.
 */

#include "latina.h"
#include "oratio_arbor.h"
#include "oratio_sententiae.h"
#include "oratio_forma.h"
#include "piscina.h"
#include "xar.h"
#include <stdio.h>
#include <string.h>

interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
          FILE* f = fopen(via, "rb");
        longus  longitudo;
     character* memoria;

    si (f == NIHIL)
    {
        redde NIHIL;
    }
    fseek(f, 0L, SEEK_END);
    longitudo = ftell(f);
    rewind(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    si (fread(memoria, I, (size_t)longitudo, f) != (size_t)longitudo)
    {
        fclose(f);
        redde NIHIL;
    }
    fclose(f);
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
    Xar* xs;
    b32 machina = FALSUM;
    constans character* via = NIHIL;
    integer i;
    i32 n;

    per (i = I; i < argc; i++)
    {
        si (strcmp(argv[i], "-machina") == ZEPHYRUM)
        {
            machina = VERUM;
        }
        alioquin
        {
            via = argv[i];
        }
    }
    si (via == NIHIL)
    {
        fprintf(stderr, "usus: sententiae <plagula.txt> [-machina]\n");
        redde II;
    }
    piscina =
        piscina_generare_dynamicum("oratio_sententiae_instrumentum",
        33554432);
    textus = _plagulam_legere(piscina, via, &mensura);
    si (textus == NIHIL)
    {
        fprintf(stderr, "sententiae: plagula absens: %s\n", via);
        redde II;
    }
    radix = oratio_arbor_parsare(piscina, textus, mensura);
    xs = radix != NIHIL ? oratio_sententiae_extenta(piscina,
        radix) : NIHIL;
    si (xs == NIHIL)
    {
        fprintf(stderr, "sententiae: parsura fracta\n");
        redde II;
    }
    si (machina)
    {
        imprimere("# initium\tfinis\tlinea\tforma\ttextus\n");
    }
    n = xar_numerus(xs);
    per (i = ZEPHYRUM; i < (integer)n; i++)
    {
        constans OratioSententia* s =
            (constans OratioSententia*)xar_obtinere(xs,
            (i32)i);
        s32 k;

        imprimere("%d\t%d\t%d\t%s\t", (integer)s->initium,
            (integer)s->finis,
            (integer)s->linea,
            oratio_forma_titulus(s->forma));
        per (k = s->initium; k < s->finis; k++)
        {
            character c = textus[k];

            putchar((c == '\n' || c == '\r' || c == '\t') ? ' ' : c);
        }
        putchar('\n');
    }
    piscina_destruere(piscina);
    redde n > ZEPHYRUM ? ZEPHYRUM : I;
}

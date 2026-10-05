/* effectus.c - Instrumentum summarii effectuum (crusta/effectus.sh)
 *
 * Usus:  effectus <scriptum> [-radix DIR]
 * Effusio: summarium STML dialecti 'effectus' (effectus.canon) in
 * stdout. Radix ordinaria = directorium operis (portae a radice
 * currunt); tabula mandatorum ex radix/crusta/effectus_mandata.stml.
 *
 * Exitus: 0 sanum | 2 usus / scriptum absens / tabula illegibilis. */

#include "postulata_posix.h"

#include "latina.h"
#include "crusta_effectus.h"
#include "internamentum.h"
#include "piscina.h"
#include "stml.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define VIA_MAXIMA (IV * MXXIV)

integer
principale (
      integer   argc,
    character** argv)
{
                 Piscina* piscina;
     InternamentumChorda* intern;
               StmlNodus* summarium;
                  chorda  textus;
               character  radix[VIA_MAXIMA];
      constans character* scriptum  = NIHIL;
      constans character* causa     = NIHIL;
                 integer  i;
                     i32  k;

    radix[ZEPHYRUM] = '\0';
    per (i = I; i < argc; i++)
    {
        si (strcmp(argv[i], "-radix") == ZEPHYRUM && i + I < argc)
        {
            i++;
            si (strlen(argv[i]) >= magnitudo(radix))
            {
                fprintf(stderr, "effectus: radix nimis longa\n");
                redde II;
            }
            strcpy(radix, argv[i]);
        }
        alioquin si (scriptum == NIHIL)
        {
            scriptum = argv[i];
        }
        alioquin
        {
            scriptum = NIHIL;
            frange;
        }
    }
    si (scriptum == NIHIL)
    {
        fprintf(stderr, "usus: effectus <scriptum> [-radix DIR]\n");
        redde II;
    }
    si (   radix[ZEPHYRUM]                 == '\0'
        && getcwd(radix, magnitudo(radix)) == NIHIL)
    {
        fprintf(stderr, "effectus: directorium operis ignotum\n");
        redde II;
    }
    k = (i32)strlen(radix);
    dum (k > I && radix[k - I] == '/')
    {
        radix[k - I] = '\0';
        k--;
    }
    piscina = piscina_generare_dynamicum("crusta_effectus",
        (memoriae_index)XVI * M * M);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "effectus: memoria deficit\n");
        redde II;
    }
    intern = internamentum_creare(piscina);
    summarium = crusta_effectus_derivare(piscina, intern, radix,
        scriptum, NIHIL, &causa);
    si (summarium == NIHIL)
    {
        fprintf(stderr, "effectus: %s: %s\n", scriptum,
            causa != NIHIL ? causa : "memoria deficit");
        piscina_destruere(piscina);
        redde II;
    }
    textus = stml_scribere(summarium, piscina, VERUM);
    fwrite(textus.datum, I, (size_t)textus.mensura, stdout);
    imprimere("\n");
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}

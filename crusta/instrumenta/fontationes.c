/* fontationes.c - Instrumentum fontationum crustae (crusta/fontationes.sh)
 *
 * Usus:  fontationes <scriptum> [-radix DIR]
 * Effusio: linea una per fontationem, tabulis separata:
 *   <genus>\t<via>\t<plagula>:<linea>\t<fontatum|exsecutum>
 * (ordo per genus, deinde viam; crusta_fontationes.h genera nominat).
 * Radix ordinaria = directorium operis (portae a radice currunt).
 *
 * Exitus: 0 omnia resoluta | 1 irresolutum ullum (clavis iudicii
 * IGNOTUM, spec 3 par. III.2) | 2 usus / scriptum absens. */

#include "postulata_posix.h"

#include "latina.h"
#include "crusta_fontationes.h"
#include "piscina.h"
#include "xar.h"
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
                     Xar* fontationes;
               character  radix[VIA_MAXIMA];
      constans character* scriptum  = NIHIL;
      constans character* causa     = NIHIL;
                 integer  i;
                     i32  k;
                     b32  irresolutum = FALSUM;

    radix[ZEPHYRUM] = '\0';
    per (i = I; i < argc; i++)
    {
        si (strcmp(argv[i], "-radix") == ZEPHYRUM && i + I < argc)
        {
            i++;
            si (strlen(argv[i]) >= magnitudo(radix))
            {
                fprintf(stderr, "fontationes: radix nimis longa\n");
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
        fprintf(stderr, "usus: fontationes <scriptum> [-radix DIR]\n");
        redde II;
    }
    si (   radix[ZEPHYRUM]                 == '\0'
        && getcwd(radix, magnitudo(radix)) == NIHIL)
    {
        fprintf(stderr, "fontationes: directorium operis ignotum\n");
        redde II;
    }
    k = (i32)strlen(radix);
    dum (k > I && radix[k - I] == '/')
    {
        radix[k - I] = '\0';
        k--;
    }
    piscina = piscina_generare_dynamicum("crusta_fontationes", 4194304);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "fontationes: memoria deficit\n");
        redde II;
    }
    fontationes = crusta_fontationes_derivare(piscina, radix, scriptum,
        &causa);
    si (fontationes == NIHIL)
    {
        fprintf(stderr, "fontationes: %s: %s\n", scriptum,
            causa != NIHIL ? causa : "memoria deficit");
        piscina_destruere(piscina);
        redde II;
    }
    per (k = ZEPHYRUM; k < xar_numerus(fontationes); k++)
    {
        CrustaFontatio* f = (CrustaFontatio*)xar_obtinere(fontationes,
            k);

        imprimere("%s\t%s\t%s:%u\t%s\n",
            crusta_fontatio_genus_titulus(f->genus),
            f->via, f->plagula, (insignatus integer)f->linea,
            f->fontatum ? "fontatum" : "exsecutum");
        si (f->genus == CRUSTA_FONTATIO_IRRESOLUTUM)
        {
            irresolutum = VERUM;
        }
    }
    piscina_destruere(piscina);
    redde irresolutum ? I : ZEPHYRUM;
}

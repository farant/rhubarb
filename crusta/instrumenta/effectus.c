/* effectus.c - Instrumentum summarii effectuum (crusta/effectus.sh)
 *
 * Usus:  effectus <scriptum> [-radix DIR] [-tabula PLAGULA]
 *        effectus -observata <scriptum> <liber> [-radix DIR]
 *        effectus -comparare <scriptum> <liber> [-radix DIR]
 *                 [-ante_scripta PLAGULA]
 * Effusio: summarium STML dialecti 'effectus' (effectus.canon);
 * -observata: summarium liberi oraculi (interpositio_macos.c);
 * -comparare: situs observati quos summarium staticum NON tegit,
 * linea una quisque ('elementum\tvia\tmandatum'). Radix ordinaria =
 * directorium operis; tabula mandatorum ex
 * radix/crusta/effectus_mandata.stml.
 *
 * Exitus: 0 sanum (comparare: omnia tecta) | 1 non tecta |
 *         2 usus / scriptum absens / tabula illegibilis. */

#include "postulata_posix.h"

#include "latina.h"
#include "crusta_effectus.h"
#include "filum.h"
#include "internamentum.h"
#include "piscina.h"
#include "stml.h"
#include "xar.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define VIA_MAXIMA (IV * MXXIV)

interior vacuum
_usus (vacuum)
{
    fprintf(stderr, "usus: effectus <scriptum> [-radix DIR]\n"
        "       effectus -observata <scriptum> <liber> [-radix DIR]\n"
        "       effectus -comparare <scriptum> <liber> [-radix DIR] "
        "[-ante_scripta PLAGULA]\n");
}

interior constans character*
_attributum (
               Piscina* piscina,
             StmlNodus* n,
    constans character* titulus)
{
    chorda* v = stml_attributum_capere(n, titulus);

    redde v == NIHIL ? "" : chorda_ut_cstr(*v, piscina);
}

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
      constans character* modus        = NIHIL;
      constans character* scriptum     = NIHIL;
      constans character* liber        = NIHIL;
      constans character* ante_via     = NIHIL;
      constans character* via_tabulae  = NIHIL;
               StmlNodus* tabula       = NIHIL;
      constans character* causa        = NIHIL;
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
        alioquin si (   strcmp(argv[i], "-ante_scripta") == ZEPHYRUM
                     && i + I < argc)
        {
            ante_via = argv[++i];
        }
        alioquin si (   strcmp(argv[i], "-tabula") == ZEPHYRUM
                     && i + I < argc)
        {
            via_tabulae = argv[++i];
        }
        alioquin si (   modus == NIHIL && scriptum == NIHIL
                     && (strcmp(argv[i], "-observata") == ZEPHYRUM
                         || strcmp(argv[i], "-comparare") == ZEPHYRUM))
        {
            modus = argv[i];
        }
        alioquin si (scriptum == NIHIL)
        {
            scriptum = argv[i];
        }
        alioquin si (modus != NIHIL && liber == NIHIL)
        {
            liber = argv[i];
        }
        alioquin
        {
            _usus();
            redde II;
        }
    }
    si (scriptum == NIHIL || (modus != NIHIL && liber == NIHIL))
    {
        _usus();
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
    si (via_tabulae != NIHIL)
    {
        StmlResultus r = stml_legere(filum_legere_totum(via_tabulae,
            piscina), piscina, intern);

        si (!r.successus)
        {
            fprintf(stderr, "effectus: tabula '%s' illegibilis\n",
                via_tabulae);
            piscina_destruere(piscina);
            redde II;
        }
        tabula = r.elementum_radix;
    }

    si (modus == NIHIL)
    {
        summarium = crusta_effectus_derivare(piscina, intern, radix,
            scriptum, tabula, &causa);
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

    /* oraculum: liber -> observata; comparare: contra staticum */
    {
         chorda  datum = filum_legere_totum(liber, piscina);
            Xar* ante = xar_creare(piscina, (i32)magnitudo(character*));
      StmlNodus* observatum;
      StmlNodus* staticum;
            Xar* non_tecta;
            Xar* explicata;

        si (datum.datum == NIHIL)
        {
            fprintf(stderr, "effectus: liber '%s' illegibilis\n",
                liber);
            piscina_destruere(piscina);
            redde II;
        }
        observatum = crusta_effectus_observata(piscina, intern, radix,
            scriptum, datum, tabula, ante, &causa);
        si (observatum == NIHIL)
        {
            fprintf(stderr, "effectus: observata: %s\n",
                causa != NIHIL ? causa : "memoria deficit");
            piscina_destruere(piscina);
            redde II;
        }
        si (ante_via != NIHIL)
        {
            ChordaAedificator* ca = chorda_aedificator_creare(piscina,
                (memoriae_index)MXXIV);

            per (k = ZEPHYRUM; k < xar_numerus(ante); k++)
            {
                chorda_aedificator_appendere_literis(ca,
                    *(character**)xar_obtinere(ante, k));
                chorda_aedificator_appendere_character(ca, '\n');
            }
            (vacuum)filum_scribere(ante_via,
                chorda_aedificator_finire(ca));
        }
        si (strcmp(modus, "-observata") == ZEPHYRUM)
        {
            textus = stml_scribere(observatum, piscina, VERUM);
            fwrite(textus.datum, I, (size_t)textus.mensura, stdout);
            imprimere("\n");
            piscina_destruere(piscina);
            redde ZEPHYRUM;
        }
        staticum = crusta_effectus_derivare(piscina, intern, radix,
            scriptum, tabula, &causa);
        si (staticum == NIHIL)
        {
            fprintf(stderr, "effectus: %s: %s\n", scriptum,
                causa != NIHIL ? causa : "memoria deficit");
            piscina_destruere(piscina);
            redde II;
        }
        explicata = xar_creare(piscina, (i32)magnitudo(StmlNodus*));
        non_tecta = crusta_effectus_non_tecta(piscina, staticum,
            observatum, explicata);
        per (k = ZEPHYRUM; k < xar_numerus(explicata); k++)
        {
            StmlNodus* o = *(StmlNodus**)xar_obtinere(explicata, k);

            imprimere("ignotum\t%.*s\t%s\t%s\n",
                (integer)o->titulus->mensura,
                (constans character*)o->titulus->datum,
                _attributum(piscina, o, "via"),
                _attributum(piscina, o, "mandatum"));
        }
        per (k = ZEPHYRUM; non_tecta && k < xar_numerus(non_tecta); k++)
        {
            StmlNodus* o = *(StmlNodus**)xar_obtinere(non_tecta, k);

            imprimere("%.*s\t%s\t%s\n", (integer)o->titulus->mensura,
                (constans character*)o->titulus->datum,
                _attributum(piscina, o, "via"),
                _attributum(piscina, o, "mandatum"));
        }
        imprimere("effectus: observata non tecta %u, per ignota "
            "explicata %u, ante scripta %u\n",
            non_tecta ? (insignatus integer)xar_numerus(non_tecta) : 0U,
            (insignatus integer)xar_numerus(explicata),
            (insignatus integer)xar_numerus(ante));
        k = non_tecta != NIHIL && xar_numerus(non_tecta) == ZEPHYRUM
            ? ZEPHYRUM : I;
        piscina_destruere(piscina);
        redde (integer)k;
    }
}

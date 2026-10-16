/* iudex_unitatum.c - FILTRUM IUDICIS pro scriptis crustae: quas viarum
 * iudex (unitates.stml <iudex>) iudicat? (spec unitates-codicis U4)
 *
 * Usus (per tools/iudex_unitatum.sh, ex radice repositorii):
 *   iudex_unitatum TITULUS VIA...   viae iudicatae, linea una per viam
 *   iudex_unitatum TITULUS          viae ex stdin (linea una per viam)
 *   -causae (ante TITULUM): via NON iudicata in stderr cum causa
 *     ('via: non iudicatur - cura aliena (vendor/unitas.stml) ...')
 *
 * Consumptores crustae (formator commissionis, lint Latinus, unci)
 * hoc rogant, numquam indicem suum directoriorum. Responsum cum CAUSIS
 * (cur via non iudicatur): bin/fabrica quid -iudex TITULUS VIA.
 *
 * Exitus: 0 responsum (lista vacua licet) | 2 usus, iudex ignotus aut
 * RECUSATIO politicae/declarationum (causae in stderr). */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "unitates.h"
#include "unitates_arboris.h"

#include <stdio.h>
#include <string.h>

/* via iudicata -> stdout; non iudicata -> stderr cum causa si
 * 'causae'; *ignotus = iudex ignotus */
interior vacuum
_iudicare (
     constans Unitates* u,
                chorda  iudex,
    constans character* via,
                   b32  causae,
               Piscina* piscina,
                   b32* ignotus)
{
              chorda causa;
    UnitatesIudicium j = unitates_iudicat(u, iudex,
        chorda_ex_literis(via,
        piscina), piscina, &causa);

    si (j == UNITATES_IUDEX_IGNOTUS)
    {
        *ignotus = VERUM;
    }
    alioquin si (j == UNITATES_IUDICAT)
    {
        printf("%s\n", via);
    }
    alioquin si (causae)
    {
        fprintf(stderr, "%s: non iudicatur - %.*s\n", via,
            (integer)causa.mensura, (constans character*)causa.datum);
    }
}

integer
principale (
      integer   argc,
    character** argv)
{
     Piscina* piscina;
    Unitates* u;
      chorda  causa;
      chorda  iudex;
         b32  ignotus  = FALSUM;
         b32  causae   = FALSUM;
     integer  primus   = I;
     integer  i;

    si (argc > I && strcmp(argv[I], "-causae") == ZEPHYRUM)
    {
        causae = VERUM;
        primus = II;
    }
    si (argc <= primus)
    {
        fprintf(stderr, "usus: iudex_unitatum [-causae] TITULUS "
            "[VIA...]  (sine viis: stdin)\n");
        redde II;
    }
    piscina = piscina_generare_dynamicum("iudex_unitatum", 16777216);
    u = unitates_legere(unitates_sutura_arboris(NIHIL, piscina),
        piscina,
        &causa, NIHIL);
    si (u == NIHIL)
    {
        fprintf(stderr, "iudex_unitatum: RECUSATIO unitatum "
            "(unitates.stml):\n%.*s\n", (integer)causa.mensura,
            (constans character*)causa.datum);
        redde II;
    }
    iudex = chorda_ex_literis(argv[primus], piscina);
    si (argc > primus + I)
    {
        per (i = primus + I; i < argc && !ignotus; i++)
        {
            _iudicare(u, iudex, argv[i], causae, piscina, &ignotus);
        }
    }
    alioquin
    {
        character linea[MXXIV];

        dum (!ignotus && fgets(linea, (integer)magnitudo(linea), stdin)
                != NIHIL)
        {
            size_t l = strlen(linea);

            dum (   l > ZEPHYRUM
                 && (linea[l - I] == '\n' || linea[l - I] == '\r'))
            {
                linea[--l] = '\0';
            }
            si (l > ZEPHYRUM)
            {
                _iudicare(u, iudex, linea, causae, piscina, &ignotus);
            }
        }
    }
    si (ignotus)
    {
        fprintf(stderr, "iudex_unitatum: iudex '%s' ignotus in "
            "unitates.stml (bin/fabrica quid -iudex X VIA nominat "
            "notos)\n", argv[primus]);
        redde II;
    }
    fflush(stdout);
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}

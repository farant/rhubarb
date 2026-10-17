/* fabrica_consilium.c - bin/fabrica consilium: CONSILIUM IUDICATUM
 * (spec project-specs/consilium-spec.md, gradus S3b; nucleus
 * lib/consilium.c)
 *
 * Usus: bin/fabrica consilium [-machina] VIA
 *   VIA = documentum consilium.canon (gradus, effectus, regulae).
 *   Effusio: undae (ordo), arcus cum causis, cyclus nominatus,
 *   recusationes, conflictus, iudicia (non LICET); -machina: TSV
 *   (contractus include/consilium.h) + 'SUMMA gradus N undae N
 *   conflictus N vetita N roganda N cyclus N recusationes N'.
 * Exitus: 0 consilium exsequi licet · 1 non sine iudicio (conflictus,
 *   cyclus, recusatio, VETITUM aut ROGANDUM) · 2 usus aut documentum
 *   illegibile (causae omnes in stderr).
 */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "filum.h"
#include "consilium.h"
#include "fabrica_sutura.h"

#include <stdio.h>
#include <string.h>

s32
suturae_consilium (
          s32   argc,
    character** argv,
      Piscina*  piscina)
{
    constans character* via      = NIHIL;
                   b32  machina  = FALSUM;
             Consilium* c;
                   Xar* regulae  = NIHIL;
                   Xar* conflictus;
                   Xar* iudicia;
         ConsiliumOrdo* ordo;
                chorda  causa;
                chorda  effusio;
                   i32  vetita   = ZEPHYRUM;
                   i32  roganda  = ZEPHYRUM;
                   s32  i;

    per (i = II; i < argc; i++)
    {
        si (strcmp(argv[i], "-machina") == ZEPHYRUM)
        {
            machina = VERUM;
        }
        alioquin si (argv[i][ZEPHYRUM] != '-' && via == NIHIL)
        {
            via = argv[i];
        }
        alioquin
        {
            via = NIHIL;
            frange;
        }
    }
    si (via == NIHIL)
    {
        fprintf(stderr, "usus: bin/fabrica consilium [-machina] VIA\n");
        redde II;
    }
    si (!filum_existit(via))
    {
        fprintf(stderr, "consilium: documentum absens: %s\n", via);
        redde II;
    }
    c = consilium_legere(filum_legere_totum(via, piscina), piscina,
        &regulae, &causa);
    si (c == NIHIL)
    {
        fprintf(stderr, "consilium: RECUSATIO %s:\n%.*s\n", via,
            (integer)causa.mensura, (constans character*)causa.datum);
        redde II;
    }
    ordo        = consilium_ordinare(c, piscina);
    conflictus  = consilium_conflictus(c, ordo, piscina);
    iudicia     = consilium_iudicare(c, regulae, piscina);
    per (i = ZEPHYRUM; i < (s32)xar_numerus(iudicia); i++)
    {
        constans ConsiliumIudicium* j = (constans ConsiliumIudicium*)
            xar_obtinere(iudicia, (i32)i);

        si (j->sententia == CONSILIUM_VETITUM)
        {
            vetita++;
        }
        alioquin si (j->sententia == CONSILIUM_ROGANDUM)
        {
            roganda++;
        }
    }
    effusio = consilium_scribere(c, ordo, conflictus, iudicia, machina,
        piscina);
    fwrite(effusio.datum, I, (size_t)effusio.mensura, stdout);
    printf(machina
        ? "SUMMA\tgradus\t%ld\tundae\t%ld\tconflictus\t%ld\tvetita\t%ld"
          "\troganda\t%ld\tcyclus\t%ld\trecusationes\t%ld\n"
        : "summa: %ld gradus, %ld undae, %ld conflictus, %ld vetita,"
          " %ld roganda, cyclus %ld, %ld recusationes\n",
        (longus)xar_numerus(consilium_gradus(c)),
        (longus)xar_numerus(ordo->undae),
        (longus)xar_numerus(conflictus),
        (longus)vetita, (longus)roganda,
        (longus)xar_numerus(ordo->cyclus),
        (longus)xar_numerus(ordo->recusationes));
    redde (   xar_numerus(conflictus) > ZEPHYRUM || vetita > ZEPHYRUM
           || roganda > ZEPHYRUM || xar_numerus(ordo->cyclus) > ZEPHYRUM
           || xar_numerus(ordo->recusationes)
               > ZEPHYRUM) ? I : ZEPHYRUM;
}

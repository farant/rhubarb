/* extenta.c - extenta structuralia markdown per selectorem (md/extenta.sh)
 *
 * Usus: extenta <plagula.md> '<selector>'
 * Effusum TSV (linea titularis '#' praefixa):
 *   via  index  tag  b-initium  b-finis  linea  columna  linea-finis
 * Exitus: 0 congruentia cum octetis, 1 nulla, 2 fractura (usus, plagula
 * absens, parsura, selector - causa in stderr). Gemellum selecta.sh
 * pro markdown; Prosa pythonicae hoc legit.
 */

#include "latina.h"
#include "md_extenta.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
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
    InternamentumChorda* intern;
              character* textus;
                    i32  mensura = ZEPHYRUM;
      MdExtentaResultus  r;
                    i32  i;
                    i32  n;

    si (argc != III)
    {
        fprintf(stderr, "usus: extenta <plagula.md> '<selector>'\n");
        redde II;
    }
    piscina = piscina_generare_dynamicum("md_extenta_instrumentum",
        16777216);
    intern = internamentum_creare(piscina);
    textus = _plagulam_legere(piscina, argv[I], &mensura);
    si (textus == NIHIL)
    {
        fprintf(stderr, "extenta: plagula absens: %s\n", argv[I]);
        redde II;
    }
    r = md_extenta_quaerere(piscina, intern, textus, mensura, argv[II]);
    si (!r.successus)
    {
        fprintf(stderr, "extenta: %.*s\n", (integer)r.causa.mensura,
            (constans character*)r.causa.datum);
        piscina_destruere(piscina);
        redde II;
    }
    imprimere("# via\tindex\ttag\tb-initium\tb-finis\tlinea\tcolumna\t"
              "linea-finis\n");
    n = xar_numerus(r.extenta);
    per (i = ZEPHYRUM; i < n; i++)
    {
        constans MdExtentum* x = (constans MdExtentum*)xar_obtinere(
            r.extenta, i);

        imprimere("%s\t%d\t%s\t%d\t%d\t%d\t%d\t%d\n", argv[I],
            (integer)i,
            x->tag, (integer)x->initium, (integer)x->finis,
            (integer)x->linea, (integer)x->columna,
            (integer)x->linea_finis);
    }
    piscina_destruere(piscina);
    redde n > ZEPHYRUM ? ZEPHYRUM : I;
}

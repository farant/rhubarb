/* probatio_exemplaria.c - exemplaria classica Mac 8x8 (vicus-latera,
 * indicium foci)
 *
 * I: obtinere - fines indicis. II: ordo bitorum (0x80 = x 0) per
 * SOLIDUS, ALBUS, TESSELLATUM, CINEREUM_LEVE. III: tegendo - modulo
 * VIII, negativa quoque. IV: id invalidum FALSUM. */
#include "latina.h"
#include "piscina.h"
#include "exemplaria.h"
#include "credo.h"
#include <stdio.h>

s32 principale (vacuum)
{
    Piscina* piscina;
        s32  x;
        s32  y;
        i32  n;

    piscina = piscina_generare_dynamicum("probatio_exemplaria", M * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);

    imprimere("\n--- I: obtinere ---\n");
    CREDO_NON_NIHIL(exemplar_obtinere(EXEMPLAR_SOLIDUS));
    CREDO_NON_NIHIL(exemplar_obtinere(EXEMPLAR_NUMERUS - I));
    CREDO_NIHIL(exemplar_obtinere(EXEMPLAR_NUMERUS));
    CREDO_AEQUALIS_I32((i32)(insignatus character)
        exemplar_obtinere(EXEMPLAR_TESSELLATUM)[ZEPHYRUM], 0xAA);

    imprimere("\n--- II: ordo bitorum ---\n");
    n = ZEPHYRUM;
    per (y = ZEPHYRUM; y < VIII; y++)
    {
        per (x = ZEPHYRUM; x < VIII; x++)
        {
            si (exemplar_punctum(EXEMPLAR_SOLIDUS, x, y))
            {
                n++;
            }
            CREDO_FALSUM(exemplar_punctum(EXEMPLAR_ALBUS, x, y));
        }
    }
    CREDO_AEQUALIS_I32(n, LXIV);
    /* 0xAA / 0x55: x 0 in linea 0, x 1 in linea 1 */
    CREDO_VERUM(exemplar_punctum(EXEMPLAR_TESSELLATUM, ZEPHYRUM,
        ZEPHYRUM));
    CREDO_FALSUM(exemplar_punctum(EXEMPLAR_TESSELLATUM, I, ZEPHYRUM));
    CREDO_FALSUM(exemplar_punctum(EXEMPLAR_TESSELLATUM, ZEPHYRUM, I));
    CREDO_VERUM(exemplar_punctum(EXEMPLAR_TESSELLATUM, I, I));
    /* CINEREUM_LEVE: punctum unum, (0, 0) */
    n = ZEPHYRUM;
    per (y = ZEPHYRUM; y < VIII; y++)
    {
        per (x = ZEPHYRUM; x < VIII; x++)
        {
            si (exemplar_punctum(EXEMPLAR_CINEREUM_LEVE, x, y))
            {
                n++;
            }
        }
    }
    CREDO_AEQUALIS_I32(n, I);
    CREDO_VERUM(exemplar_punctum(EXEMPLAR_CINEREUM_LEVE, ZEPHYRUM,
        ZEPHYRUM));

    imprimere("\n--- III: tegendo ---\n");
    CREDO_VERUM(exemplar_punctum(EXEMPLAR_CINEREUM_LEVE, VIII, XVI));
    CREDO_VERUM(exemplar_punctum(EXEMPLAR_CINEREUM_LEVE, -VIII, -VIII));
    CREDO_FALSUM(exemplar_punctum(EXEMPLAR_CINEREUM_LEVE, -I,
        ZEPHYRUM));
    /* -1 = columna VII */
    CREDO_VERUM(exemplar_punctum(EXEMPLAR_LINEAE_VERTICALES, -II,
        ZEPHYRUM));
    CREDO_FALSUM(exemplar_punctum(EXEMPLAR_LINEAE_VERTICALES, -I,
        ZEPHYRUM));

    imprimere("\n--- IV: id invalidum ---\n");
    CREDO_FALSUM(exemplar_punctum(EXEMPLAR_NUMERUS, ZEPHYRUM,
        ZEPHYRUM));

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}

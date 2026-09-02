/* probatio_moneta.c - ULID + fons fortuitorum
 *
 * moneta nullam probationem habuit usque ad 2026-08-01, cum
 * moneta_octeti_fortuiti addita est (fons secretorum). Functio
 * quae claves parit sine probatione manere non debet.
 */

#include "latina.h"
#include "moneta.h"
#include "credo.h"
#include "piscina.h"

#include <stdio.h>
#include <string.h>

s32
principale (vacuum)
{
    Piscina* piscina;
        b32  praeteritus;

    piscina = piscina_generare_dynamicum("probatio_moneta", 65536);
    si (piscina == NIHIL)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);

    /* ---- ULID: forma et unicitas ---- */
    {
        character a[MONETA_ULID_MENSURA];
        character b[MONETA_ULID_MENSURA];

        imprimere("\n--- Probans moneta_ulid ---\n");
        moneta_ulid(a);
        moneta_ulid(b);

        CREDO_AEQUALIS_I32((i32)strlen(a), XXVI);
        CREDO_AEQUALIS_I32((i32)strlen(b), XXVI);
        /* monotonia intra ms eundem: a <= b semper */
        CREDO_VERUM(strcmp(a, b) <= ZEPHYRUM);
        CREDO_VERUM(strcmp(a, b) != ZEPHYRUM);
    }

    /* ---- ULID fortuitum: sine monotonia, pars fortuita differt ---- */
    {
        character a[MONETA_ULID_MENSURA];
        character b[MONETA_ULID_MENSURA];

        imprimere("\n--- Probans moneta_ulid_fortuita ---\n");
        moneta_ulid_fortuita(a);
        moneta_ulid_fortuita(b);

        CREDO_AEQUALIS_I32((i32)strlen(a), XXVI);
        CREDO_VERUM(strcmp(a, b) != ZEPHYRUM);
        /* praefixum temporis (X characteres) idem intra ms eundem,
         * cauda fortuita differt - id ipsum est cur haec varians
         * exsistit (praefixa brevia discernenda) */
        CREDO_VERUM(strncmp(a + X, b + X, XVI) != ZEPHYRUM);
    }

    /* ---- octeti fortuiti: fons secretorum ---- */
    {
         i8 a[32];
         i8 b[32];
         i8 sentinella[8];
        i32 i;
        i32 differentiae = ZEPHYRUM;

        imprimere("\n--- Probans moneta_octeti_fortuiti ---\n");

        memset(a, ZEPHYRUM, magnitudo(a));
        memset(b, ZEPHYRUM, magnitudo(b));
        CREDO_VERUM(moneta_octeti_fortuiti(a, XXXII));
        CREDO_VERUM(moneta_octeti_fortuiti(b, XXXII));

        /* duae vocationes differant (probabilitas collisionis
         * 2^-256 - si haec cadit, fons fractus est, non fortuna) */
        per (i = ZEPHYRUM; i < XXXII; i++)
        {
            si (a[i] != b[i])
            {
                differentiae++;
            }
        }
        CREDO_VERUM(differentiae > ZEPHYRUM);

        /* non omnes zephyri (fons mutus id daret) */
        differentiae = ZEPHYRUM;
        per (i = ZEPHYRUM; i < XXXII; i++)
        {
            si (a[i] != ZEPHYRUM)
            {
                differentiae++;
            }
        }
        CREDO_VERUM(differentiae > ZEPHYRUM);

        /* argumenta mala: FALSUM, effusio INTACTA */
        memset(sentinella, 0x5A, magnitudo(sentinella));
        CREDO_FALSUM(moneta_octeti_fortuiti(NIHIL, VIII));
        CREDO_FALSUM(moneta_octeti_fortuiti(sentinella, ZEPHYRUM));
        per (i = ZEPHYRUM; i < VIII; i++)
        {
            CREDO_AEQUALIS_I32((i32)(i8)sentinella[i], 0x5A);
        }
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(piscina);

    redde praeteritus ? ZEPHYRUM : I;
}

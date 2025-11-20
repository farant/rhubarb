/* probatio_uuid.c - Probationes UUID */
#include "latina.h"
#include "uuid.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>  /* memcmp */
#include <unistd.h>  /* usleep */

s32 principale(vacuum)
{
    Piscina* piscina;
    b32      praeteritus;

    /* Aperire piscinam et credonem */
    piscina = piscina_generare_dynamicum("probatio_uuid", CDLVI);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    /* ==================================================
     * Probare uuidv7_creare
     * ================================================== */

    {
        chorda uuid;

        imprimere("\n--- Probans uuidv7_creare ---\n");

        uuid = uuidv7_creare(piscina);
        CREDO_NON_NIHIL(uuid.datum);
        CREDO_AEQUALIS_I32(uuid.mensura, UUID_MENSURA_CHORDAE);

        /* Imprimere pro inspectione */
        imprimere("UUID generatus: %.*s\n", uuid.mensura, uuid.datum);
    }

    /* ==================================================
     * Probare uuid_est_validus
     * ================================================== */

    {
        chorda uuid;

        imprimere("\n--- Probans uuid_est_validus ---\n");

        uuid = uuidv7_creare(piscina);
        CREDO_VERUM(uuid_est_validus(uuid));

        /* UUID validus manualiter */
        uuid = chorda_ex_literis("01234567-89ab-7cde-8f01-23456789abcd", piscina);
        CREDO_VERUM(uuid_est_validus(uuid));

        /* UUID invalidus - mensura incorrecta */
        uuid = chorda_ex_literis("01234567", piscina);
        CREDO_FALSUM(uuid_est_validus(uuid));

        /* UUID invalidus - dashes in locis incorrectis */
        uuid = chorda_ex_literis("0123456789ab7cde8f0123456789abcd--", piscina);
        CREDO_FALSUM(uuid_est_validus(uuid));

        /* UUID invalidus - characteres non-hex */
        uuid = chorda_ex_literis("0123456z-89ab-7cde-8f01-23456789abcd", piscina);
        CREDO_FALSUM(uuid_est_validus(uuid));
    }

    /* ==================================================
     * Probare uuid_est_v7
     * ================================================== */

    {
        chorda uuid;

        imprimere("\n--- Probans uuid_est_v7 ---\n");

        uuid = uuidv7_creare(piscina);
        CREDO_VERUM(uuid_est_v7(uuid));

        /* UUIDv4 (simulatus) */
        uuid = chorda_ex_literis("01234567-89ab-4cde-8f01-23456789abcd", piscina);
        CREDO_FALSUM(uuid_est_v7(uuid));

        /* UUIDv7 validus */
        uuid = chorda_ex_literis("01234567-89ab-7cde-8f01-23456789abcd", piscina);
        CREDO_VERUM(uuid_est_v7(uuid));
    }

    /* ==================================================
     * Probare uuid_ex_chorda et uuid_ad_chordam
     * ================================================== */

    {
        chorda  uuid_orig;
        chorda  uuid_conv;
        i8      bytes[XVI];

        imprimere("\n--- Probans conversio UUID ---\n");

        uuid_orig = uuidv7_creare(piscina);

        /* Convertere ad bytes */
        CREDO_VERUM(uuid_ex_chorda(uuid_orig, bytes));

        /* Convertere retro ad chordam */
        uuid_conv = uuid_ad_chordam(bytes, piscina);
        CREDO_NON_NIHIL(uuid_conv.datum);
        CREDO_AEQUALIS_I32(uuid_conv.mensura, UUID_MENSURA_CHORDAE);

        /* Debent esse aequales */
        CREDO_CHORDA_AEQUALIS(uuid_orig, uuid_conv);
    }

    /* ==================================================
     * Probare unicitas UUIDs
     * ================================================== */

    {
        chorda uuid1;
        chorda uuid2;
        chorda uuid3;

        imprimere("\n--- Probans unicitas ---\n");

        /* Generare tres UUIDs */
        uuid1 = uuidv7_creare(piscina);
        uuid2 = uuidv7_creare(piscina);
        uuid3 = uuidv7_creare(piscina);

        /* Debent esse omnes diversi */
        CREDO_FALSUM(chorda_aequalis(uuid1, uuid2));
        CREDO_FALSUM(chorda_aequalis(uuid2, uuid3));
        CREDO_FALSUM(chorda_aequalis(uuid1, uuid3));

        imprimere("UUID 1: %.*s\n", uuid1.mensura, uuid1.datum);
        imprimere("UUID 2: %.*s\n", uuid2.mensura, uuid2.datum);
        imprimere("UUID 3: %.*s\n", uuid3.mensura, uuid3.datum);
    }

    /* ==================================================
     * Probare ordinatio temporalis
     * ================================================== */

    {
        chorda uuid1;
        chorda uuid2;
        s32    cmp;

        imprimere("\n--- Probans ordinatio temporalis ---\n");

        /* Generare primum UUID */
        uuid1 = uuidv7_creare(piscina);

        /* Exspectare paulo (assecurare tempus diversum) */
        usleep(II * M);  /* 2 milliseconds */

        /* Generare secundum UUID */
        uuid2 = uuidv7_creare(piscina);

        /* UUID2 debet esse maior quam UUID1 (quia posterior in tempore) */
        cmp = (s32)memcmp(uuid1.datum, uuid2.datum, VIII);  /* Compare primi 8 chars */

        imprimere("UUID 1: %.*s\n", uuid1.mensura, uuid1.datum);
        imprimere("UUID 2: %.*s\n", uuid2.mensura, uuid2.datum);

        /* Secundus debet esse maior vel aequalis */
        /* (aequalis si generati in eodem millisecondo) */
        CREDO_VERUM(cmp <= ZEPHYRUM);
    }

    /* ==================================================
     * Probare generatio multiplex
     * ================================================== */

    {
        chorda uuids[XX];
        i32    i;
        i32    j;

        imprimere("\n--- Probans generatio multiplex ---\n");

        /* Generare XX UUIDs */
        per (i = ZEPHYRUM; i < XX; i++)
        {
            uuids[i] = uuidv7_creare(piscina);
            CREDO_NON_NIHIL(uuids[i].datum);
            CREDO_VERUM(uuid_est_validus(uuids[i]));
            CREDO_VERUM(uuid_est_v7(uuids[i]));
        }

        /* Verificare omnes sunt unici */
        per (i = ZEPHYRUM; i < XX; i++)
        {
            per (j = i + I; j < XX; j++)
            {
                CREDO_FALSUM(chorda_aequalis(uuids[i], uuids[j]));
            }
        }

        imprimere("Generati XX UUIDs unici:\n");
        per (i = ZEPHYRUM; i < X; i++)
        {
            imprimere("  %.*s\n", uuids[i].mensura, uuids[i].datum);
        }
        imprimere("  ...\n");
    }

    /* ==================================================
     * Probare conversio cum UUID cognito
     * ================================================== */

    {
        chorda  uuid_str;
        i8      bytes[XVI];
        chorda  uuid_retro;

        imprimere("\n--- Probans conversio cum UUID cognito ---\n");

        /* UUID cognitus */
        uuid_str = chorda_ex_literis("01234567-89ab-7cde-8f01-23456789abcd", piscina);

        /* Convertere ad bytes */
        CREDO_VERUM(uuid_ex_chorda(uuid_str, bytes));

        /* Verificare aliqua bytes */
        CREDO_AEQUALIS_I32((i32)bytes[ZEPHYRUM], 0x01);
        CREDO_AEQUALIS_I32((i32)bytes[I], 0x23);
        CREDO_AEQUALIS_I32((i32)bytes[II], 0x45);
        CREDO_AEQUALIS_I32((i32)bytes[III], 0x67);

        /* Convertere retro */
        uuid_retro = uuid_ad_chordam(bytes, piscina);
        CREDO_CHORDA_AEQUALIS(uuid_str, uuid_retro);
    }

    /* ==================================================
     * Probare version bits
     * ================================================== */

    {
        chorda  uuid;
        i8      bytes[XVI];

        imprimere("\n--- Probans version bits ---\n");

        uuid = uuidv7_creare(piscina);
        CREDO_VERUM(uuid_ex_chorda(uuid, bytes));

        /* Byte 6 debet habere version bits (alto nibble = 0x7) */
        CREDO_AEQUALIS_I32(((i32)bytes[VI] >> IV) & 0x0F, 0x07);

        /* Byte 8 debet habere variant bits (duo alti bits = 10) */
        CREDO_AEQUALIS_I32(((i32)bytes[VIII] >> VI) & 0x03, 0x02);
    }

    /* ==================================================
     * Compendium
     * ================================================== */

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();

    /* Purificatio */
    credo_claudere();
    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}

/* probatio_sigillum.c - Probationes Sigilli (SHA-256)
 * Vectores NIST FIPS 180-4 / vectores probationis notissimi. */
#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "sigillum.h"
#include <stdio.h>
#include <string.h>

interior vacuum
_proba_vector (constans character* titulus, constans character* nuntius,
    constans character* speratum)
{
    Sigillum s = sigillum_computare(nuntius, strlen(nuntius));
    character hex[SIGILLUM_HEX_MENSURA];

    sigillum_hex(&s, hex);
    imprimere("  %s\n", titulus);
    CREDO_VERUM (strcmp(hex, speratum) == ZEPHYRUM);
}

s32 principale (vacuum)
{
    b32 praeteritus;
    Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_sigillum", 4096);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    /* ========================================================
     * PROBARE: vectores NIST
     * ======================================================== */
    {
        imprimere("\n--- Probans vectores NIST ---\n");

        _proba_vector("vacuum", "",
            "e3b0c44298fc1c149afbf4c8996fb924"
            "27ae41e4649b934ca495991b7852b855");
        _proba_vector("abc", "abc",
            "ba7816bf8f01cfea414140de5dae2223"
            "b00361a396177a9cb410ff61f20015ad");
        _proba_vector("bloccus duplex",
            "abcdbcdecdefdefgefghfghighijhijk"
            "ijkljklmklmnlmnomnopnopq",
            "248d6a61d20638b8e5c026930c3e6039"
            "a33ce45964ff2167f6ecedd419db06c1");
    }

    /* ========================================================
     * PROBARE: farcimen circa limites bloccorum (55/56/64 octeti -
     * anguli farciminis: 55 = longitudo intra bloccum, 56 = bloccus
     * farciminis additur, 64 = bloccus exactus)
     * ======================================================== */
    {
        character nuntius[65];
        Sigillum uno;
        Sigillum partim;
        SigillumContextus ctx;
        integer k;

        imprimere("\n--- Probans limites bloccorum ---\n");

        per (k = ZEPHYRUM; k < 64; k++)
        {
            nuntius[k] = 'a';
        }
        nuntius[64] = '\0';

        per (k = 55; k <= 64; k++)
        {
            uno = sigillum_computare(nuntius, (memoriae_index)k);
            sigillum_incipere(&ctx);
            sigillum_addere(&ctx, nuntius, (memoriae_index)(k / 2));
            sigillum_addere(&ctx, nuntius + (k / 2),
                (memoriae_index)(k - k / 2));
            partim = sigillum_finire(&ctx);
            CREDO_VERUM (sigillum_aequale(&uno, &partim));
        }
    }

    /* ========================================================
     * PROBARE: nuntius longus ("a" x 1,000,000 - NIST)
     * ======================================================== */
    {
        character mille_a[1000];
        SigillumContextus ctx;
        Sigillum s;
        character hex[SIGILLUM_HEX_MENSURA];
        integer k;

        imprimere("\n--- Probans nuntium longum ---\n");

        per (k = ZEPHYRUM; k < 1000; k++)
        {
            mille_a[k] = 'a';
        }
        sigillum_incipere(&ctx);
        per (k = ZEPHYRUM; k < 1000; k++)
        {
            sigillum_addere(&ctx, mille_a, 1000);
        }
        s = sigillum_finire(&ctx);
        sigillum_hex(&s, hex);
        CREDO_VERUM (strcmp(hex,
            "cdc76e5c9914fb9281a1c7e284d73e67"
            "f1809a48a497200e046d39ccc7112cd0") == ZEPHYRUM);
    }

    /* ========================================================
     * PROBARE: aequalitas + identitas contenti
     * ======================================================== */
    {
        Sigillum a = sigillum_computare("idem", 4);
        Sigillum b = sigillum_computare("idem", 4);
        Sigillum c = sigillum_computare("Idem", 4);

        imprimere("\n--- Probans aequalitatem ---\n");

        CREDO_VERUM (sigillum_aequale(&a, &b));
        CREDO_VERUM (!sigillum_aequale(&a, &c));
    }

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}

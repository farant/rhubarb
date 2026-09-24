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

interior vacuum
_proba_hmac (
    constans character* titulus,
    constans character* clavis,
                   i32  mensura_clavis,
    constans character* datum,
                   i32  mensura_datorum,
    constans character* speratum)
{
     Sigillum s = sigillum_hmac(clavis, (memoriae_index)mensura_clavis,
         datum, (memoriae_index)mensura_datorum);
    character hex[SIGILLUM_HEX_MENSURA];

    sigillum_hex(&s, hex);
    imprimere("  %s\n", titulus);
    CREDO_VERUM (strcmp(hex, speratum) == ZEPHYRUM);
}

s32 principale (vacuum)
{
        b32  praeteritus;
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


    /* ==================================================
     * PROBARE: HMAC-SHA256 - speranda GENERATA per Python hmac
     * (RFC 4231 casus + margines clavis); lapide feature-requests/005
     * ================================================== */

    {
        imprimere("\n--- Probans HMAC-SHA256 ---\n");
        _proba_hmac("RFC 4231 casus 1",
            "\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b"
            "\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b", 20,
            "\x48\x69\x20\x54\x68\x65\x72\x65", 8,
            "b0344c61d8db38535ca8afceaf0bf12b"
            "881dc200c9833da726e9376c2e32cff7");
        _proba_hmac("RFC 4231 casus 2",
            "\x4a\x65\x66\x65", 4,
            "\x77\x68\x61\x74\x20\x64\x6f\x20\x79\x61\x20\x77"
            "\x61\x6e\x74\x20\x66\x6f\x72\x20\x6e\x6f\x74\x68"
            "\x69\x6e\x67\x3f", 28,
            "5bdcc146bf60754e6a042426089575c7"
            "5a003f089d2739839dec58b964ec3843");
        _proba_hmac("RFC 4231 casus 3",
            "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
            "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa", 20,
            "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd"
            "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd"
            "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd"
            "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd"
            "\xdd\xdd", 50,
            "773ea91e36800e46854db8ebd09181a7"
            "2959098b3ef8c122d9635514ced565fe");
        _proba_hmac("RFC 4231 casus 6: clavis longior bloco",
            "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
            "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
            "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
            "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
            "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
            "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
            "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
            "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
            "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
            "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
            "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa", 131,
            "\x54\x65\x73\x74\x20\x55\x73\x69\x6e\x67\x20\x4c"
            "\x61\x72\x67\x65\x72\x20\x54\x68\x61\x6e\x20\x42"
            "\x6c\x6f\x63\x6b\x2d\x53\x69\x7a\x65\x20\x4b\x65"
            "\x79\x20\x2d\x20\x48\x61\x73\x68\x20\x4b\x65\x79"
            "\x20\x46\x69\x72\x73\x74", 54,
            "60e431591ee0b67f0d8a26aacbf5b77f"
            "8e0bc6213728c5140546040f0ee37f54");
        _proba_hmac("clavis vacua, datum vacuum",
            "", 0,
            "", 0,
            "b613679a0814d9ec772f95d778c35fc5"
            "ff1697c493715653c6c712144292c5ad");
        _proba_hmac("clavis LXIV octetorum exacte",
            "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b"
            "\x0c\x0d\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17"
            "\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f\x20\x21\x22\x23"
            "\x24\x25\x26\x27\x28\x29\x2a\x2b\x2c\x2d\x2e\x2f"
            "\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3a\x3b"
            "\x3c\x3d\x3e\x3f", 64,
            "\x61\x62\x63", 3,
            "6ab541b4869dca71c4ca11d8bb1b0253"
            "3b789a557583161429292c7404bc21f6");
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

/* probatio_base64.c - Probationes pro Base64 bibliotheca
 *
 * Probationes encoding/decoding Base64 secundum RFC 4648.
 */

#include "base64.h"
#include "credo.h"
#include "piscina.h"
#include "chorda.h"

#include <stdio.h>
#include <string.h>


/* ========================================================================
 * PROBATIONES - LONGITUDO CODIFICATA
 * ======================================================================== */

interior vacuum
probatio_longitudo_codificata(Piscina* piscina)
{
    (vacuum)piscina;

    printf("--- Probans longitudo codificata ---\n");

    /* 0 bytes -> 0 characteres */
    CREDO_AEQUALIS_I32(base64_longitudo_codificata(0), 0);

    /* 1 byte -> 4 characteres (cum II padding) */
    CREDO_AEQUALIS_I32(base64_longitudo_codificata(I), IV);

    /* 2 bytes -> 4 characteres (cum I padding) */
    CREDO_AEQUALIS_I32(base64_longitudo_codificata(II), IV);

    /* 3 bytes -> 4 characteres (nullum padding) */
    CREDO_AEQUALIS_I32(base64_longitudo_codificata(III), IV);

    /* 4 bytes -> 8 characteres */
    CREDO_AEQUALIS_I32(base64_longitudo_codificata(IV), VIII);

    /* 6 bytes -> 8 characteres */
    CREDO_AEQUALIS_I32(base64_longitudo_codificata(VI), VIII);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - RFC 4648 TEST VECTORS
 * ======================================================================== */

interior vacuum
probatio_rfc4648_vectors(Piscina* piscina)
{
    chorda codificata;

    printf("--- Probans RFC 4648 test vectors ---\n");

    /* "" -> "" */
    codificata = base64_codificare((constans i8*)"", 0, piscina);
    CREDO_AEQUALIS_I32(codificata.mensura, 0);

    /* "f" -> "Zg==" */
    codificata = base64_codificare((constans i8*)"f", I, piscina);
    CREDO_AEQUALIS_I32(codificata.mensura, IV);
    CREDO_VERUM(memcmp(codificata.datum, "Zg==", IV) == 0);

    /* "fo" -> "Zm8=" */
    codificata = base64_codificare((constans i8*)"fo", II, piscina);
    CREDO_AEQUALIS_I32(codificata.mensura, IV);
    CREDO_VERUM(memcmp(codificata.datum, "Zm8=", IV) == 0);

    /* "foo" -> "Zm9v" */
    codificata = base64_codificare((constans i8*)"foo", III, piscina);
    CREDO_AEQUALIS_I32(codificata.mensura, IV);
    CREDO_VERUM(memcmp(codificata.datum, "Zm9v", IV) == 0);

    /* "foob" -> "Zm9vYg==" */
    codificata = base64_codificare((constans i8*)"foob", IV, piscina);
    CREDO_AEQUALIS_I32(codificata.mensura, VIII);
    CREDO_VERUM(memcmp(codificata.datum, "Zm9vYg==", VIII) == 0);

    /* "fooba" -> "Zm9vYmE=" */
    codificata = base64_codificare((constans i8*)"fooba", V, piscina);
    CREDO_AEQUALIS_I32(codificata.mensura, VIII);
    CREDO_VERUM(memcmp(codificata.datum, "Zm9vYmE=", VIII) == 0);

    /* "foobar" -> "Zm9vYmFy" */
    codificata = base64_codificare((constans i8*)"foobar", VI, piscina);
    CREDO_AEQUALIS_I32(codificata.mensura, VIII);
    CREDO_VERUM(memcmp(codificata.datum, "Zm9vYmFy", VIII) == 0);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - DECODIFICATIO
 * ======================================================================== */

interior vacuum
probatio_decodificatio(Piscina* piscina)
{
    chorda        input;
    Base64Fructus fructus;

    printf("--- Probans decodificatio ---\n");

    /* "Zg==" -> "f" */
    input = chorda_ex_literis("Zg==", piscina);
    fructus = base64_decodificare(input, piscina);
    CREDO_NON_NIHIL(fructus.datum);
    CREDO_AEQUALIS_I32(fructus.mensura, I);
    CREDO_VERUM(fructus.datum[0] == 'f');

    /* "Zm8=" -> "fo" */
    input = chorda_ex_literis("Zm8=", piscina);
    fructus = base64_decodificare(input, piscina);
    CREDO_NON_NIHIL(fructus.datum);
    CREDO_AEQUALIS_I32(fructus.mensura, II);
    CREDO_VERUM(memcmp(fructus.datum, "fo", II) == 0);

    /* "Zm9v" -> "foo" */
    input = chorda_ex_literis("Zm9v", piscina);
    fructus = base64_decodificare(input, piscina);
    CREDO_NON_NIHIL(fructus.datum);
    CREDO_AEQUALIS_I32(fructus.mensura, III);
    CREDO_VERUM(memcmp(fructus.datum, "foo", III) == 0);

    /* "Zm9vYmFy" -> "foobar" */
    input = chorda_ex_literis("Zm9vYmFy", piscina);
    fructus = base64_decodificare(input, piscina);
    CREDO_NON_NIHIL(fructus.datum);
    CREDO_AEQUALIS_I32(fructus.mensura, VI);
    CREDO_VERUM(memcmp(fructus.datum, "foobar", VI) == 0);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - ROUND TRIP
 * ======================================================================== */

interior vacuum
probatio_round_trip(Piscina* piscina)
{
    constans i8*  originale;
    i32           mensura;
    chorda        codificata;
    Base64Fructus decodificata;

    printf("--- Probans round trip ---\n");

    /* Textus simplex */
    originale = (constans i8*)"Hello, World!";
    mensura = XIII;
    codificata = base64_codificare(originale, mensura, piscina);
    CREDO_NON_NIHIL(codificata.datum);

    decodificata = base64_decodificare(codificata, piscina);
    CREDO_NON_NIHIL(decodificata.datum);
    CREDO_AEQUALIS_I32(decodificata.mensura, mensura);
    CREDO_VERUM(memcmp(decodificata.datum, originale, (size_t)mensura) == 0);

    printf("  'Hello, World!' -> '%.*s' -> '%.*s'\n",
           codificata.mensura, codificata.datum,
           decodificata.mensura, decodificata.datum);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - BINARY DATA
 * ======================================================================== */

interior vacuum
probatio_binary_data(Piscina* piscina)
{
    i8            data[VIII];
    chorda        codificata;
    Base64Fructus decodificata;
    i32           i;

    printf("--- Probans binary data (cum nulls) ---\n");

    /* Data cum null bytes */
    data[0] = 0x00;
    data[I] = 0x01;
    data[II] = 0x02;
    data[III] = 0x00;
    data[IV] = (i8)0xFF;
    data[V] = (i8)0xFE;
    data[VI] = 0x00;
    data[VII] = 0x7F;

    codificata = base64_codificare(data, VIII, piscina);
    CREDO_NON_NIHIL(codificata.datum);

    decodificata = base64_decodificare(codificata, piscina);
    CREDO_NON_NIHIL(decodificata.datum);
    CREDO_AEQUALIS_I32(decodificata.mensura, VIII);

    per (i = 0; i < VIII; i++)
    {
        CREDO_VERUM(decodificata.datum[i] == data[i]);
    }

    printf("  Binary data cum nulls: OK\n");

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - NULLUM ARGUMENTA
 * ======================================================================== */

interior vacuum
probatio_nullum_argumenta(Piscina* piscina)
{
    chorda        codificata;
    chorda        input;
    Base64Fructus fructus;

    printf("--- Probans nullum argumenta ---\n");

    /* Codificare cum NIHIL datum */
    codificata = base64_codificare(NIHIL, X, piscina);
    CREDO_NIHIL(codificata.datum);
    CREDO_AEQUALIS_I32(codificata.mensura, 0);

    /* Codificare cum mensura 0 */
    codificata = base64_codificare((constans i8*)"test", 0, piscina);
    CREDO_AEQUALIS_I32(codificata.mensura, 0);

    /* Codificare cum NIHIL piscina */
    codificata = base64_codificare((constans i8*)"test", IV, NIHIL);
    CREDO_NIHIL(codificata.datum);

    /* Decodificare cum NIHIL datum */
    input.datum = NIHIL;
    input.mensura = IV;
    fructus = base64_decodificare(input, piscina);
    CREDO_NIHIL(fructus.datum);

    /* Decodificare cum mensura 0 */
    input = chorda_ex_literis("Zg==", piscina);
    input.mensura = 0;
    fructus = base64_decodificare(input, piscina);
    CREDO_NIHIL(fructus.datum);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - LONGITUDO DECODIFICATA
 * ======================================================================== */

interior vacuum
probatio_longitudo_decodificata(Piscina* piscina)
{
    chorda input;

    printf("--- Probans longitudo decodificata ---\n");

    /* "Zg==" -> 1 byte */
    input = chorda_ex_literis("Zg==", piscina);
    CREDO_AEQUALIS_I32(base64_longitudo_decodificata(input), I);

    /* "Zm8=" -> 2 bytes */
    input = chorda_ex_literis("Zm8=", piscina);
    CREDO_AEQUALIS_I32(base64_longitudo_decodificata(input), II);

    /* "Zm9v" -> 3 bytes */
    input = chorda_ex_literis("Zm9v", piscina);
    CREDO_AEQUALIS_I32(base64_longitudo_decodificata(input), III);

    /* "Zm9vYmFy" -> 6 bytes */
    input = chorda_ex_literis("Zm9vYmFy", piscina);
    CREDO_AEQUALIS_I32(base64_longitudo_decodificata(input), VI);

    printf("\n");
}


/* ========================================================================
 * PRINCIPALE
 * ======================================================================== */

integer
principale(vacuum)
{
    Piscina* piscina;
    b32 successus;

    printf("\n");
    printf("========================================\n");
    printf("PROBATIONES BASE64\n");
    printf("========================================\n");
    printf("\n");

    piscina = piscina_generare_dynamicum("probatio_base64", LXIV * M);
    credo_aperire(piscina);

    probatio_longitudo_codificata(piscina);
    probatio_rfc4648_vectors(piscina);
    probatio_decodificatio(piscina);
    probatio_round_trip(piscina);
    probatio_binary_data(piscina);
    probatio_nullum_argumenta(piscina);
    probatio_longitudo_decodificata(piscina);

    credo_imprimere_compendium();

    printf("========================================\n");
    printf("\n");

    successus = credo_omnia_praeterierunt();

    credo_claudere();
    piscina_destruere(piscina);

    redde successus ? 0 : I;
}

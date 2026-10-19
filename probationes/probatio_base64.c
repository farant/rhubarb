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
_sectio_prima (
    CredoContextus* c)
{
    (vacuum)c;

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
}


/* ========================================================================
 * PROBATIONES - RFC 4648 TEST VECTORS
 * ======================================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
     chorda  codificata;

    piscina = c->piscina;

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
}


/* ========================================================================
 * PROBATIONES - DECODIFICATIO
 * ======================================================================== */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
          Piscina* piscina;
           chorda  input;
    Base64Fructus  fructus;

    piscina = c->piscina;

    /* "Zg==" -> "f" */
    input    = chorda_ex_literis("Zg==", piscina);
    fructus  = base64_decodificare(input, piscina);
    CREDO_NON_NIHIL(fructus.datum);
    CREDO_AEQUALIS_I32(fructus.mensura, I);
    CREDO_VERUM(fructus.datum[0] == 'f');

    /* "Zm8=" -> "fo" */
    input    = chorda_ex_literis("Zm8=", piscina);
    fructus  = base64_decodificare(input, piscina);
    CREDO_NON_NIHIL(fructus.datum);
    CREDO_AEQUALIS_I32(fructus.mensura, II);
    CREDO_VERUM(memcmp(fructus.datum, "fo", II) == 0);

    /* "Zm9v" -> "foo" */
    input    = chorda_ex_literis("Zm9v", piscina);
    fructus  = base64_decodificare(input, piscina);
    CREDO_NON_NIHIL(fructus.datum);
    CREDO_AEQUALIS_I32(fructus.mensura, III);
    CREDO_VERUM(memcmp(fructus.datum, "foo", III) == 0);

    /* "Zm9vYmFy" -> "foobar" */
    input    = chorda_ex_literis("Zm9vYmFy", piscina);
    fructus  = base64_decodificare(input, piscina);
    CREDO_NON_NIHIL(fructus.datum);
    CREDO_AEQUALIS_I32(fructus.mensura, VI);
    CREDO_VERUM(memcmp(fructus.datum, "foobar", VI) == 0);
}


/* ========================================================================
 * PROBATIONES - ROUND TRIP
 * ======================================================================== */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
          Piscina* piscina;
      constans i8* originale;
              i32  mensura;
           chorda  codificata;
    Base64Fructus  decodificata;

    piscina = c->piscina;

    /* Textus simplex */
    originale   = (constans i8*)"Hello, World!";
    mensura     = XIII;
    codificata  = base64_codificare(originale, mensura, piscina);
    CREDO_NON_NIHIL(codificata.datum);

    decodificata = base64_decodificare(codificata, piscina);
    CREDO_NON_NIHIL(decodificata.datum);
    CREDO_AEQUALIS_I32(decodificata.mensura, mensura);
    CREDO_VERUM(memcmp(decodificata.datum, originale, (size_t)mensura)
        == 0);

    printf("  'Hello, World!' -> '%.*s' -> '%.*s'\n",
           codificata.mensura, codificata.datum,
           decodificata.mensura, decodificata.datum);
}


/* ========================================================================
 * PROBATIONES - BINARY DATA
 * ======================================================================== */

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
          Piscina* piscina;
               i8  data[VIII];
           chorda  codificata;
    Base64Fructus  decodificata;
              i32  i;

    piscina = c->piscina;

    /* Data cum null bytes */
    data[0]    = 0x00;
    data[I]    = 0x01;
    data[II]   = 0x02;
    data[III]  = 0x00;
    data[IV]   = (i8)0xFF;
    data[V]    = (i8)0xFE;
    data[VI]   = 0x00;
    data[VII]  = 0x7F;

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
}


/* ========================================================================
 * PROBATIONES - NULLUM ARGUMENTA
 * ======================================================================== */

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
          Piscina* piscina;
           chorda  codificata;
           chorda  input;
    Base64Fructus  fructus;

    piscina = c->piscina;

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
    input.datum    = NIHIL;
    input.mensura  = IV;
    fructus        = base64_decodificare(input, piscina);
    CREDO_NIHIL(fructus.datum);

    /* Decodificare cum mensura 0 */
    input          = chorda_ex_literis("Zg==", piscina);
    input.mensura  = 0;
    fructus        = base64_decodificare(input, piscina);
    CREDO_NIHIL(fructus.datum);
}


/* ========================================================================
 * PROBATIONES - LONGITUDO DECODIFICATA
 * ======================================================================== */

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
    Piscina* piscina;
     chorda  input;

    piscina = c->piscina;

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
}


/* ========================================================================
 * PRINCIPALE
 * ======================================================================== */

hic_manens constans CredoSectio SECTIONES[] = {
    { "longitudo codificata", _sectio_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "RFC 4648 test vectors", _sectio_secunda, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "decodificatio", _sectio_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "round trip", _sectio_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "binary data (cum nulls)", _sectio_quinta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "nullum argumenta", _sectio_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "longitudo decodificata", _sectio_septima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("base64", SECTIONES);
}

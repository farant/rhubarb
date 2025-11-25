/* probatio_friatio.c - Probationes Friationis (Hash Functions) */
#include "latina.h"
#include "friatio.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

s32 principale(vacuum)
{
    Piscina* piscina;
    b32      praeteritus;

    /* Aperire piscinam et credonem */
    piscina = piscina_generare_dynamicum("probatio_friatio", MMMMXCVI);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    /* ==================================================
     * Probare FNV-1a basicum
     * ================================================== */

    imprimere("\n--- Probans friatio_fnv1a ---\n");

    {
        chorda ch;
        i32    h1, h2;

        /* Hash chorda vacua */
        ch = chorda_ex_literis("", piscina);
        h1 = friatio_fnv1a(ch);
        CREDO_VERUM(h1 != ZEPHYRUM);
        imprimere("  Hash vacua: 0x%08X\n", h1);

        /* Hash simplex */
        ch = chorda_ex_literis("hello", piscina);
        h1 = friatio_fnv1a(ch);
        CREDO_VERUM(h1 != ZEPHYRUM);
        imprimere("  Hash 'hello': 0x%08X\n", h1);

        /* Determinismus - idem input = idem output */
        h2 = friatio_fnv1a(ch);
        CREDO_AEQUALIS_I32(h1, h2);

        /* Chordae differentes = hashes differentes */
        ch = chorda_ex_literis("world", piscina);
        h2 = friatio_fnv1a(ch);
        CREDO_INAEQUALITAS_I32(h1, h2);
        imprimere("  Hash 'world': 0x%08X\n", h2);
    }

    /* ==================================================
     * Probare FNV-1a literis
     * ================================================== */

    imprimere("\n--- Probans friatio_fnv1a_literis ---\n");

    {
        chorda ch;
        i32    h1, h2;

        h1 = friatio_fnv1a_literis("test", IV);
        CREDO_VERUM(h1 != ZEPHYRUM);

        /* Debet aequalis esse ad chorda versio */
        ch = chorda_ex_literis("test", piscina);
        h2 = friatio_fnv1a(ch);
        CREDO_AEQUALIS_I32(h1, h2);

        imprimere("  Hash 'test' (literis): 0x%08X\n", h1);
    }

    /* ==================================================
     * Probare FNV-1a valores cognitos
     * ================================================== */

    imprimere("\n--- Probans FNV-1a valores cognitos ---\n");

    {
        chorda ch;
        i32    h;

        /* FNV-1a of "" = 0x811c9dc5 (2166136261) */
        ch = chorda_ex_literis("", piscina);
        h = friatio_fnv1a(ch);
        CREDO_AEQUALIS_I32(h, (i32)0x811c9dc5);
        imprimere("  '' -> 0x%08X (expected 0x811c9dc5)\n", h);

        /* "a" */
        ch = chorda_ex_literis("a", piscina);
        h = friatio_fnv1a(ch);
        CREDO_AEQUALIS_I32(h, (i32)0xe40c292c);
        imprimere("  'a' -> 0x%08X (expected 0xe40c292c)\n", h);

        /* "foobar" */
        ch = chorda_ex_literis("foobar", piscina);
        h = friatio_fnv1a(ch);
        CREDO_AEQUALIS_I32(h, (i32)0xbf9cf968);
        imprimere("  'foobar' -> 0x%08X (expected 0xbf9cf968)\n", h);
    }

    /* ==================================================
     * Probare DJB2 basicum
     * ================================================== */

    imprimere("\n--- Probans friatio_djb2 ---\n");

    {
        chorda ch;
        i32    h1, h2;

        /* Hash simplex */
        ch = chorda_ex_literis("hello", piscina);
        h1 = friatio_djb2(ch);
        CREDO_VERUM(h1 != ZEPHYRUM);
        imprimere("  Hash 'hello': 0x%08X\n", h1);

        /* Determinismus */
        h2 = friatio_djb2(ch);
        CREDO_AEQUALIS_I32(h1, h2);

        /* Chordae differentes */
        ch = chorda_ex_literis("world", piscina);
        h2 = friatio_djb2(ch);
        CREDO_INAEQUALITAS_I32(h1, h2);
        imprimere("  Hash 'world': 0x%08X\n", h2);
    }

    /* ==================================================
     * Probare DJB2 literis
     * ================================================== */

    imprimere("\n--- Probans friatio_djb2_literis ---\n");

    {
        chorda ch;
        i32    h1, h2;

        h1 = friatio_djb2_literis("test", IV);
        CREDO_VERUM(h1 != ZEPHYRUM);

        /* Debet aequalis esse ad chorda versio */
        ch = chorda_ex_literis("test", piscina);
        h2 = friatio_djb2(ch);
        CREDO_AEQUALIS_I32(h1, h2);

        imprimere("  Hash 'test' (literis): 0x%08X\n", h1);
    }

    /* ==================================================
     * Probare DJB2 valores cognitos
     * ================================================== */

    imprimere("\n--- Probans DJB2 valores cognitos ---\n");

    {
        chorda ch;
        i32    h;

        /* DJB2 of empty string = 5381 (init value) */
        ch = chorda_ex_literis("", piscina);
        h = friatio_djb2(ch);
        CREDO_AEQUALIS_I32(h, (i32)5381);
        imprimere("  '' -> %u (expected 5381)\n", h);

        /* Known value for "hello" */
        ch = chorda_ex_literis("hello", piscina);
        h = friatio_djb2(ch);
        CREDO_AEQUALIS_I32(h, (i32)0x0F923099);
        imprimere("  'hello' -> 0x%08X\n", h);
    }

    /* ==================================================
     * Probare SHA-1 basicum
     * ================================================== */

    imprimere("\n--- Probans sha1_friare ---\n");

    {
        i8  digest[SHA1_DIGEST_MENSURA];
        i32 i;

        /* Hash empty string */
        sha1_friare((constans i8*)"", ZEPHYRUM, digest);

        imprimere("  SHA1('') = ");
        per (i = ZEPHYRUM; i < SHA1_DIGEST_MENSURA; i++)
        {
            imprimere("%02x", (i8)digest[i] & 0xFF);
        }
        imprimere("\n");

        /* Known value: SHA1("") = da39a3ee5e6b4b0d3255bfef95601890afd80709 */
        CREDO_AEQUALIS_S32((i8)digest[ZEPHYRUM] & 0xFF, 0xda);
        CREDO_AEQUALIS_S32((i8)digest[I] & 0xFF, 0x39);
        CREDO_AEQUALIS_S32((i8)digest[II] & 0xFF, 0xa3);
        CREDO_AEQUALIS_S32((i8)digest[III] & 0xFF, 0xee);
    }

    /* ==================================================
     * Probare SHA-1 test vectors
     * ================================================== */

    imprimere("\n--- Probans SHA-1 test vectors ---\n");

    {
        i8  digest[SHA1_DIGEST_MENSURA];
        i32 i;

        /* Test vector 1: "abc" */
        /* Expected: a9993e364706816aba3e25717850c26c9cd0d89d */
        sha1_friare((constans i8*)"abc", III, digest);

        imprimere("  SHA1('abc') = ");
        per (i = ZEPHYRUM; i < SHA1_DIGEST_MENSURA; i++)
        {
            imprimere("%02x", (i8)digest[i] & 0xFF);
        }
        imprimere("\n");

        CREDO_AEQUALIS_S32((i8)digest[ZEPHYRUM] & 0xFF, 0xa9);
        CREDO_AEQUALIS_S32((i8)digest[I] & 0xFF, 0x99);
        CREDO_AEQUALIS_S32((i8)digest[II] & 0xFF, 0x3e);
        CREDO_AEQUALIS_S32((i8)digest[III] & 0xFF, 0x36);
        CREDO_AEQUALIS_S32((i8)digest[XIX] & 0xFF, 0x9d);
    }

    {
        constans character* test2 = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
        i32 len2 = (i32)strlen(test2);
        i8  digest[SHA1_DIGEST_MENSURA];
        i32 i;

        /* Expected: 84983e441c3bd26ebaae4aa1f95129e5e54670f1 */
        sha1_friare((constans i8*)test2, len2, digest);

        imprimere("  SHA1('abcdbcde...nopq') = ");
        per (i = ZEPHYRUM; i < SHA1_DIGEST_MENSURA; i++)
        {
            imprimere("%02x", (i8)digest[i] & 0xFF);
        }
        imprimere("\n");

        CREDO_AEQUALIS_S32((i8)digest[ZEPHYRUM] & 0xFF, 0x84);
        CREDO_AEQUALIS_S32((i8)digest[I] & 0xFF, 0x98);
        CREDO_AEQUALIS_S32((i8)digest[II] & 0xFF, 0x3e);
        CREDO_AEQUALIS_S32((i8)digest[III] & 0xFF, 0x44);
    }

    /* ==================================================
     * Probare SHA-1 incrementale
     * ================================================== */

    imprimere("\n--- Probans SHA-1 incrementale ---\n");

    {
        SHA1Contextus ctx;
        i8  digest1[SHA1_DIGEST_MENSURA];
        i8  digest2[SHA1_DIGEST_MENSURA];
        i32 i;

        /* Hash "hello world" in uno passu */
        sha1_friare((constans i8*)"hello world", XI, digest1);

        /* Hash "hello world" in duobus passibus */
        sha1_initiare(&ctx);
        sha1_addere(&ctx, (constans i8*)"hello ", VI);
        sha1_addere(&ctx, (constans i8*)"world", V);
        sha1_finire(&ctx, digest2);

        /* Debent esse aequales */
        per (i = ZEPHYRUM; i < SHA1_DIGEST_MENSURA; i++)
        {
            CREDO_AEQUALIS_S32((i8)digest1[i] & 0xFF, (i8)digest2[i] & 0xFF);
        }

        imprimere("  Incrementale = uno passu: ");
        per (i = ZEPHYRUM; i < SHA1_DIGEST_MENSURA; i++)
        {
            imprimere("%02x", (i8)digest1[i] & 0xFF);
        }
        imprimere("\n");
    }

    /* ==================================================
     * Probare sha1_friare_chorda
     * ================================================== */

    imprimere("\n--- Probans sha1_friare_chorda ---\n");

    {
        chorda ch;
        i8     digest1[SHA1_DIGEST_MENSURA];
        i8     digest2[SHA1_DIGEST_MENSURA];
        i32    i;

        ch = chorda_ex_literis("test string", piscina);

        sha1_friare_chorda(ch, digest1);
        sha1_friare((constans i8*)"test string", XI, digest2);

        /* Debent esse aequales */
        per (i = ZEPHYRUM; i < SHA1_DIGEST_MENSURA; i++)
        {
            CREDO_AEQUALIS_S32((i8)digest1[i] & 0xFF, (i8)digest2[i] & 0xFF);
        }

        imprimere("  SHA1 chorda = SHA1 literis: VERUM\n");
    }

    /* ==================================================
     * Probare comparatio FNV-1a vs DJB2
     * ================================================== */

    imprimere("\n--- Probans comparatio FNV-1a vs DJB2 ---\n");

    {
        chorda ch;
        i32    fnv, djb;

        ch = chorda_ex_literis("same input", piscina);
        fnv = friatio_fnv1a(ch);
        djb = friatio_djb2(ch);

        /* Debent esse differentes (different algorithms) */
        CREDO_INAEQUALITAS_I32(fnv, djb);

        imprimere("  'same input':\n");
        imprimere("    FNV-1a: 0x%08X\n", fnv);
        imprimere("    DJB2:   0x%08X\n", djb);
    }

    /* ==================================================
     * Probare SHA-256 basicum
     * ================================================== */

    imprimere("\n--- Probans sha256_friare ---\n");

    {
        i8  digest[SHA256_DIGEST_MENSURA];
        i32 i;

        /* Hash empty string */
        sha256_friare((constans i8*)"", ZEPHYRUM, digest);

        imprimere("  SHA256('') = ");
        per (i = ZEPHYRUM; i < SHA256_DIGEST_MENSURA; i++)
        {
            imprimere("%02x", (i8)digest[i] & 0xFF);
        }
        imprimere("\n");

        /* Known value: SHA256("") = e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855 */
        CREDO_AEQUALIS_S32((i8)digest[ZEPHYRUM] & 0xFF, 0xe3);
        CREDO_AEQUALIS_S32((i8)digest[I] & 0xFF, 0xb0);
        CREDO_AEQUALIS_S32((i8)digest[II] & 0xFF, 0xc4);
        CREDO_AEQUALIS_S32((i8)digest[III] & 0xFF, 0x42);
    }

    /* ==================================================
     * Probare SHA-256 test vectors
     * ================================================== */

    imprimere("\n--- Probans SHA-256 test vectors ---\n");

    {
        i8  digest[SHA256_DIGEST_MENSURA];
        i32 i;

        /* Test vector 1: "abc" */
        /* Expected: ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad */
        sha256_friare((constans i8*)"abc", III, digest);

        imprimere("  SHA256('abc') = ");
        per (i = ZEPHYRUM; i < SHA256_DIGEST_MENSURA; i++)
        {
            imprimere("%02x", (i8)digest[i] & 0xFF);
        }
        imprimere("\n");

        CREDO_AEQUALIS_S32((i8)digest[ZEPHYRUM] & 0xFF, 0xba);
        CREDO_AEQUALIS_S32((i8)digest[I] & 0xFF, 0x78);
        CREDO_AEQUALIS_S32((i8)digest[II] & 0xFF, 0x16);
        CREDO_AEQUALIS_S32((i8)digest[III] & 0xFF, 0xbf);
        CREDO_AEQUALIS_S32((i8)digest[XXXI] & 0xFF, 0xad);
    }

    {
        constans character* test2 = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
        i32 len2 = (i32)strlen(test2);
        i8  digest[SHA256_DIGEST_MENSURA];
        i32 i;

        /* Expected: 248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1 */
        sha256_friare((constans i8*)test2, len2, digest);

        imprimere("  SHA256('abcdbcde...nopq') = ");
        per (i = ZEPHYRUM; i < SHA256_DIGEST_MENSURA; i++)
        {
            imprimere("%02x", (i8)digest[i] & 0xFF);
        }
        imprimere("\n");

        CREDO_AEQUALIS_S32((i8)digest[ZEPHYRUM] & 0xFF, 0x24);
        CREDO_AEQUALIS_S32((i8)digest[I] & 0xFF, 0x8d);
        CREDO_AEQUALIS_S32((i8)digest[II] & 0xFF, 0x6a);
        CREDO_AEQUALIS_S32((i8)digest[III] & 0xFF, 0x61);
    }

    /* ==================================================
     * Probare SHA-256 incrementale
     * ================================================== */

    imprimere("\n--- Probans SHA-256 incrementale ---\n");

    {
        SHA256Contextus ctx;
        i8  digest1[SHA256_DIGEST_MENSURA];
        i8  digest2[SHA256_DIGEST_MENSURA];
        i32 i;

        /* Hash "hello world" in uno passu */
        sha256_friare((constans i8*)"hello world", XI, digest1);

        /* Hash "hello world" in duobus passibus */
        sha256_initiare(&ctx);
        sha256_addere(&ctx, (constans i8*)"hello ", VI);
        sha256_addere(&ctx, (constans i8*)"world", V);
        sha256_finire(&ctx, digest2);

        /* Debent esse aequales */
        per (i = ZEPHYRUM; i < SHA256_DIGEST_MENSURA; i++)
        {
            CREDO_AEQUALIS_S32((i8)digest1[i] & 0xFF, (i8)digest2[i] & 0xFF);
        }

        imprimere("  Incrementale = uno passu: ");
        per (i = ZEPHYRUM; i < SHA256_DIGEST_MENSURA; i++)
        {
            imprimere("%02x", (i8)digest1[i] & 0xFF);
        }
        imprimere("\n");
    }

    /* ==================================================
     * Probare sha256_friare_chorda
     * ================================================== */

    imprimere("\n--- Probans sha256_friare_chorda ---\n");

    {
        chorda ch;
        i8     digest1[SHA256_DIGEST_MENSURA];
        i8     digest2[SHA256_DIGEST_MENSURA];
        i32    i;

        ch = chorda_ex_literis("test string", piscina);

        sha256_friare_chorda(ch, digest1);
        sha256_friare((constans i8*)"test string", XI, digest2);

        /* Debent esse aequales */
        per (i = ZEPHYRUM; i < SHA256_DIGEST_MENSURA; i++)
        {
            CREDO_AEQUALIS_S32((i8)digest1[i] & 0xFF, (i8)digest2[i] & 0xFF);
        }

        imprimere("  SHA256 chorda = SHA256 literis: VERUM\n");
    }

    /* ==================================================
     * Probare CRC32 basicum
     * ================================================== */

    imprimere("\n--- Probans crc32_calculare ---\n");

    {
        i32 crc;

        /* Hash empty data */
        crc = crc32_calculare((constans i8*)"", ZEPHYRUM);
        imprimere("  CRC32('') = 0x%08X\n", crc);
        CREDO_AEQUALIS_I32(crc, (i32)0x00000000);

        /* Known test vector: "123456789" -> 0xCBF43926 */
        crc = crc32_calculare((constans i8*)"123456789", IX);
        imprimere("  CRC32('123456789') = 0x%08X (expected 0xCBF43926)\n", crc);
        CREDO_AEQUALIS_I32(crc, (i32)0xCBF43926);
    }

    /* ==================================================
     * Probare CRC32 test vectors
     * ================================================== */

    imprimere("\n--- Probans CRC32 test vectors ---\n");

    {
        i32 crc;

        /* Test vector: "hello" */
        crc = crc32_calculare((constans i8*)"hello", V);
        imprimere("  CRC32('hello') = 0x%08X\n", crc);
        CREDO_VERUM(crc != ZEPHYRUM);

        /* Test vector: "The quick brown fox jumps over the lazy dog" */
        crc = crc32_calculare(
            (constans i8*)"The quick brown fox jumps over the lazy dog",
            XLIII);
        imprimere("  CRC32('The quick brown...') = 0x%08X (expected 0x414FA339)\n", crc);
        CREDO_AEQUALIS_I32(crc, (i32)0x414FA339);
    }

    /* ==================================================
     * Probare CRC32 incrementale
     * ================================================== */

    imprimere("\n--- Probans CRC32 incrementale ---\n");

    {
        i32 crc1, crc2;

        /* CRC32 "hello world" in uno passu */
        crc1 = crc32_calculare((constans i8*)"hello world", XI);

        /* CRC32 "hello world" in duobus passibus */
        crc2 = crc32_initiare();
        crc2 = crc32_addere(crc2, (constans i8*)"hello ", VI);
        crc2 = crc32_addere(crc2, (constans i8*)"world", V);
        crc2 = crc32_finire(crc2);

        /* Debent esse aequales */
        CREDO_AEQUALIS_I32(crc1, crc2);

        imprimere("  Incrementale = uno passu: 0x%08X\n", crc1);
    }

    /* ==================================================
     * Probare crc32_calculare_chorda
     * ================================================== */

    imprimere("\n--- Probans crc32_calculare_chorda ---\n");

    {
        chorda ch;
        i32    crc1, crc2;

        ch = chorda_ex_literis("test string", piscina);

        crc1 = crc32_calculare_chorda(ch);
        crc2 = crc32_calculare((constans i8*)"test string", XI);

        /* Debent esse aequales */
        CREDO_AEQUALIS_I32(crc1, crc2);

        imprimere("  CRC32 chorda = CRC32 literis: VERUM (0x%08X)\n", crc1);
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

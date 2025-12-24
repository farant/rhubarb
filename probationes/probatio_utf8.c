/* probatio_utf8.c - Probationes UTF-8 decodendi */
#include "latina.h"
#include "utf8.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>

s32 principale(vacuum)
{
    Piscina* piscina;
    b32 praeteritus;

    piscina = piscina_generare_dynamicum("test_utf8", MMMMXCVI);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ==================================================
     * Probare utf8_longitudo_byte
     * ================================================== */

    {
        imprimere("\n--- Probans utf8_longitudo_byte ---\n");

        /* ASCII (0xxxxxxx) -> 1 byte */
        CREDO_AEQUALIS_S32(utf8_longitudo_byte(0x00), I);
        CREDO_AEQUALIS_S32(utf8_longitudo_byte(0x41), I);  /* 'A' */
        CREDO_AEQUALIS_S32(utf8_longitudo_byte(0x7F), I);

        /* 2 bytes (110xxxxx) */
        CREDO_AEQUALIS_S32(utf8_longitudo_byte(0xC2), II);  /* U+0080+ */
        CREDO_AEQUALIS_S32(utf8_longitudo_byte(0xDF), II);

        /* 3 bytes (1110xxxx) */
        CREDO_AEQUALIS_S32(utf8_longitudo_byte(0xE0), III);
        CREDO_AEQUALIS_S32(utf8_longitudo_byte(0xEF), III);

        /* 4 bytes (11110xxx) */
        CREDO_AEQUALIS_S32(utf8_longitudo_byte(0xF0), IV);
        CREDO_AEQUALIS_S32(utf8_longitudo_byte(0xF4), IV);

        /* Continuatio (10xxxxxx) -> 0 (invalidum ut principalis) */
        CREDO_AEQUALIS_S32(utf8_longitudo_byte(0x80), ZEPHYRUM);
        CREDO_AEQUALIS_S32(utf8_longitudo_byte(0xBF), ZEPHYRUM);

        /* Invalidum (11111xxx) -> 0 */
        CREDO_AEQUALIS_S32(utf8_longitudo_byte(0xF8), ZEPHYRUM);
        CREDO_AEQUALIS_S32(utf8_longitudo_byte(0xFF), ZEPHYRUM);
    }


    /* ==================================================
     * Probare utf8_est_continuatio
     * ================================================== */

    {
        imprimere("\n--- Probans utf8_est_continuatio ---\n");

        /* Continuationes (10xxxxxx) */
        CREDO_VERUM(utf8_est_continuatio(0x80));
        CREDO_VERUM(utf8_est_continuatio(0xBF));
        CREDO_VERUM(utf8_est_continuatio(0xA0));

        /* Non-continuationes */
        CREDO_FALSUM(utf8_est_continuatio(0x00));
        CREDO_FALSUM(utf8_est_continuatio(0x7F));
        CREDO_FALSUM(utf8_est_continuatio(0xC0));
        CREDO_FALSUM(utf8_est_continuatio(0xE0));
        CREDO_FALSUM(utf8_est_continuatio(0xF0));
    }


    /* ==================================================
     * Probare utf8_decodere - ASCII
     * ================================================== */

    {
        constans i8 ascii[] = { 'H', 'e', 'l', 'l', 'o' };
        constans i8* ptr = ascii;
        constans i8* finis = ascii + V;
        s32 runa;

        imprimere("\n--- Probans utf8_decodere ASCII ---\n");

        runa = utf8_decodere(&ptr, finis);
        CREDO_AEQUALIS_S32(runa, 'H');
        CREDO_VERUM(ptr == ascii + I);

        runa = utf8_decodere(&ptr, finis);
        CREDO_AEQUALIS_S32(runa, 'e');

        runa = utf8_decodere(&ptr, finis);
        CREDO_AEQUALIS_S32(runa, 'l');

        runa = utf8_decodere(&ptr, finis);
        CREDO_AEQUALIS_S32(runa, 'l');

        runa = utf8_decodere(&ptr, finis);
        CREDO_AEQUALIS_S32(runa, 'o');
        CREDO_VERUM(ptr == finis);
    }


    /* ==================================================
     * Probare utf8_decodere - 2 bytes (Latin-1 supplement)
     * ================================================== */

    {
        /* U+00E9 = e-acutum = C3 A9 */
        constans i8 e_acutum[] = { 0xC3, 0xA9 };
        constans i8* ptr = e_acutum;
        constans i8* finis = e_acutum + II;
        s32 runa;

        imprimere("\n--- Probans utf8_decodere 2-byte ---\n");

        runa = utf8_decodere(&ptr, finis);
        CREDO_AEQUALIS_S32(runa, 0x00E9);  /* e-acutum */
        CREDO_VERUM(ptr == finis);
    }

    {
        /* U+00F1 = n-tilde = C3 B1 */
        constans i8 n_tilde[] = { 0xC3, 0xB1 };
        constans i8* ptr = n_tilde;
        constans i8* finis = n_tilde + II;
        s32 runa;

        runa = utf8_decodere(&ptr, finis);
        CREDO_AEQUALIS_S32(runa, 0x00F1);  /* n-tilde */
    }

    {
        /* U+00FC = u-umlaut = C3 BC */
        constans i8 u_umlaut[] = { 0xC3, 0xBC };
        constans i8* ptr = u_umlaut;
        constans i8* finis = u_umlaut + II;
        s32 runa;

        runa = utf8_decodere(&ptr, finis);
        CREDO_AEQUALIS_S32(runa, 0x00FC);  /* u-umlaut */
    }


    /* ==================================================
     * Probare utf8_decodere - 3 bytes
     * ================================================== */

    {
        /* U+03C0 = pi = CE A0 */
        constans i8 pi[] = { 0xCE, 0xA0 };
        constans i8* ptr = pi;
        constans i8* finis = pi + II;
        s32 runa;

        imprimere("\n--- Probans utf8_decodere 3-byte ---\n");

        runa = utf8_decodere(&ptr, finis);
        CREDO_AEQUALIS_S32(runa, 0x03A0);  /* Pi maiusculum */
    }

    {
        /* U+2019 = right single quote = E2 80 99 */
        constans i8 rsquote[] = { 0xE2, 0x80, 0x99 };
        constans i8* ptr = rsquote;
        constans i8* finis = rsquote + III;
        s32 runa;

        runa = utf8_decodere(&ptr, finis);
        CREDO_AEQUALIS_S32(runa, 0x2019);  /* right single quote */
    }

    {
        /* U+201C = left double quote = E2 80 9C */
        constans i8 ldquote[] = { 0xE2, 0x80, 0x9C };
        constans i8* ptr = ldquote;
        constans i8* finis = ldquote + III;
        s32 runa;

        runa = utf8_decodere(&ptr, finis);
        CREDO_AEQUALIS_S32(runa, 0x201C);  /* left double quote */
    }

    {
        /* U+2014 = em dash = E2 80 94 */
        constans i8 emdash[] = { 0xE2, 0x80, 0x94 };
        constans i8* ptr = emdash;
        constans i8* finis = emdash + III;
        s32 runa;

        runa = utf8_decodere(&ptr, finis);
        CREDO_AEQUALIS_S32(runa, 0x2014);  /* em dash */
    }


    /* ==================================================
     * Probare utf8_decodere - 4 bytes
     * ================================================== */

    {
        /* U+1F600 = grinning face = F0 9F 98 80 */
        constans i8 emoji[] = { 0xF0, 0x9F, 0x98, 0x80 };
        constans i8* ptr = emoji;
        constans i8* finis = emoji + IV;
        s32 runa;

        imprimere("\n--- Probans utf8_decodere 4-byte ---\n");

        runa = utf8_decodere(&ptr, finis);
        CREDO_AEQUALIS_S32(runa, 0x1F600);  /* grinning face */
        CREDO_VERUM(ptr == finis);
    }


    /* ==================================================
     * Probare utf8_decodere - Sequentiae invalidae
     * ================================================== */

    {
        /* Continuatio orphana */
        constans i8 orphana[] = { 0x80 };
        constans i8* ptr = orphana;
        constans i8* finis = orphana + I;
        s32 runa;

        imprimere("\n--- Probans utf8_decodere invalidae ---\n");

        runa = utf8_decodere(&ptr, finis);
        CREDO_AEQUALIS_S32(runa, -I);
        CREDO_VERUM(ptr == finis);  /* Debet promovere */
    }

    {
        /* Sequentia truncata (2-byte incompleta) */
        constans i8 truncata[] = { 0xC3 };
        constans i8* ptr = truncata;
        constans i8* finis = truncata + I;
        s32 runa;

        runa = utf8_decodere(&ptr, finis);
        CREDO_AEQUALIS_S32(runa, -I);
    }

    {
        /* Sequentia truncata (3-byte incompleta) */
        constans i8 truncata3[] = { 0xE2, 0x80 };
        constans i8* ptr = truncata3;
        constans i8* finis = truncata3 + II;
        s32 runa;

        runa = utf8_decodere(&ptr, finis);
        CREDO_AEQUALIS_S32(runa, -I);
    }

    {
        /* Overlong encoding (2-byte pro ASCII) */
        constans i8 overlong[] = { 0xC0, 0x80 };  /* Overlong NUL */
        constans i8* ptr = overlong;
        constans i8* finis = overlong + II;
        s32 runa;

        runa = utf8_decodere(&ptr, finis);
        CREDO_AEQUALIS_S32(runa, -I);
    }

    {
        /* Byte invalidus 0xFF */
        constans i8 invalidus[] = { 0xFF };
        constans i8* ptr = invalidus;
        constans i8* finis = invalidus + I;
        s32 runa;

        runa = utf8_decodere(&ptr, finis);
        CREDO_AEQUALIS_S32(runa, -I);
    }


    /* ==================================================
     * Probare utf8_decodere - Buffer vacuus
     * ================================================== */

    {
        constans i8 datum[] = { 'x' };
        constans i8* ptr = datum;
        constans i8* finis = datum;  /* ptr == finis */
        s32 runa;

        imprimere("\n--- Probans utf8_decodere buffer vacuus ---\n");

        runa = utf8_decodere(&ptr, finis);
        CREDO_AEQUALIS_S32(runa, -I);
    }


    /* ==================================================
     * Probare utf8_numerare_runas
     * ================================================== */

    {
        /* ASCII solum */
        constans i8 ascii[] = { 'H', 'e', 'l', 'l', 'o' };
        s32 numerus;

        imprimere("\n--- Probans utf8_numerare_runas ---\n");

        numerus = utf8_numerare_runas(ascii, V);
        CREDO_AEQUALIS_S32(numerus, V);
    }

    {
        /* Mixta: "Hé" = H + e-acutum (2 bytes) */
        constans i8 mixta[] = { 'H', 0xC3, 0xA9 };  /* H + e-acutum */
        s32 numerus;

        numerus = utf8_numerare_runas(mixta, III);
        CREDO_AEQUALIS_S32(numerus, II);  /* 2 runae */
    }

    {
        /* "café" = c + a + f + e-acutum */
        constans i8 cafe[] = { 'c', 'a', 'f', 0xC3, 0xA9 };
        s32 numerus;

        numerus = utf8_numerare_runas(cafe, V);
        CREDO_AEQUALIS_S32(numerus, IV);  /* 4 runae */
    }

    {
        /* Buffer vacuus */
        s32 numerus;

        numerus = utf8_numerare_runas(NIHIL, ZEPHYRUM);
        CREDO_AEQUALIS_S32(numerus, ZEPHYRUM);
    }


    /* ==================================================
     * Probare utf8_proxima_runa
     * ================================================== */

    {
        constans i8 datum[] = { 'A', 0xC3, 0xA9, 'B' };  /* A + e-acutum + B */
        constans i8* ptr = datum;
        constans i8* finis = datum + IV;

        imprimere("\n--- Probans utf8_proxima_runa ---\n");

        /* A -> e-acutum */
        ptr = utf8_proxima_runa(ptr, finis);
        CREDO_VERUM(ptr == datum + I);

        /* e-acutum -> B */
        ptr = utf8_proxima_runa(ptr, finis);
        CREDO_VERUM(ptr == datum + III);

        /* B -> finis */
        ptr = utf8_proxima_runa(ptr, finis);
        CREDO_VERUM(ptr == finis);
    }


    /* ==================================================
     * Probare utf8_prior_runa
     * ================================================== */

    {
        constans i8 datum[] = { 'A', 0xC3, 0xA9, 'B' };  /* A + e-acutum + B */
        constans i8* ptr = datum + IV;  /* Ad finem */
        constans i8* initium = datum;

        imprimere("\n--- Probans utf8_prior_runa ---\n");

        /* finis -> B */
        ptr = utf8_prior_runa(ptr, initium);
        CREDO_VERUM(ptr == datum + III);

        /* B -> e-acutum */
        ptr = utf8_prior_runa(ptr, initium);
        CREDO_VERUM(ptr == datum + I);

        /* e-acutum -> A */
        ptr = utf8_prior_runa(ptr, initium);
        CREDO_VERUM(ptr == datum);

        /* A -> initium (non mutat) */
        ptr = utf8_prior_runa(ptr, initium);
        CREDO_VERUM(ptr == initium);
    }


    /* ==================================================
     * Probare textum realem Gutenberg
     * ================================================== */

    {
        /* Textus typicus cum smart quotes et dashes */
        /* "Hello," he said—"how are you?" */
        constans i8 gutenberg[] = {
            0xE2, 0x80, 0x9C,  /* U+201C left double quote */
            'H', 'e', 'l', 'l', 'o', ',',
            0xE2, 0x80, 0x9D,  /* U+201D right double quote */
            ' ', 'h', 'e', ' ', 's', 'a', 'i', 'd',
            0xE2, 0x80, 0x94,  /* U+2014 em dash */
            0xE2, 0x80, 0x9C,  /* U+201C left double quote */
            'h', 'o', 'w', ' ', 'a', 'r', 'e', ' ', 'y', 'o', 'u', '?',
            0xE2, 0x80, 0x9D   /* U+201D right double quote */
        };
        s32 numerus;
        constans i8* ptr;
        constans i8* finis;
        s32 runa;

        imprimere("\n--- Probans textum Gutenberg ---\n");

        /* Numerare runas */
        numerus = utf8_numerare_runas(gutenberg, (s32)magnitudo(gutenberg));
        /* Exspectamus: " H e l l o , " _ h e _ s a i d — " h o w _ a r e _ y o u ? "
         * = 1 + 6 + 1 + 8 + 1 + 1 + 12 + 1 = 31 runae */
        CREDO_AEQUALIS_S32(numerus, XXXI);

        /* Verificare primas runas */
        ptr = gutenberg;
        finis = gutenberg + (s32)magnitudo(gutenberg);

        runa = utf8_decodere(&ptr, finis);
        CREDO_AEQUALIS_S32(runa, 0x201C);  /* left double quote */

        runa = utf8_decodere(&ptr, finis);
        CREDO_AEQUALIS_S32(runa, 'H');
    }


    /* ==================================================
     * Compendium
     * ================================================== */

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();

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

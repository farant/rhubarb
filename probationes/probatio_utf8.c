/* probatio_utf8.c - Probationes UTF-8 decodendi */
#include "latina.h"
#include "utf8.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>


/* ==================================================
 * Probare utf8_longitudo_byte
 * ================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    (vacuum)c;

    {
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
}


/* ==================================================
 * Probare utf8_est_continuatio
 * ================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    (vacuum)c;

    {
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
}


/* ==================================================
 * Probare utf8_decodere - ASCII
 * ================================================== */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    (vacuum)c;

    {
        constans i8  ascii[]  = { 'H', 'e', 'l', 'l', 'o' };
        constans i8* ptr      = ascii;
        constans i8* finis    = ascii + V;
                s32  runa;

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
}


/* ==================================================
 * Probare utf8_decodere - 2 bytes (Latin-1 supplement)
 * ================================================== */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
    (vacuum)c;

    {
        /* U+00E9 = e-acutum = C3 A9 */
        constans i8  e_acutum[]  = { 0xC3, 0xA9 };
        constans i8* ptr         = e_acutum;
        constans i8* finis       = e_acutum + II;
                s32  runa;

        runa = utf8_decodere(&ptr, finis);
        CREDO_AEQUALIS_S32(runa, 0x00E9);  /* e-acutum */
        CREDO_VERUM(ptr == finis);
    }

    {
        /* U+00F1 = n-tilde = C3 B1 */
        constans i8  n_tilde[]  = { 0xC3, 0xB1 };
        constans i8* ptr        = n_tilde;
        constans i8* finis      = n_tilde + II;
                s32  runa;

        runa = utf8_decodere(&ptr, finis);
        CREDO_AEQUALIS_S32(runa, 0x00F1);  /* n-tilde */
    }

    {
        /* U+00FC = u-umlaut = C3 BC */
        constans i8  u_umlaut[]  = { 0xC3, 0xBC };
        constans i8* ptr         = u_umlaut;
        constans i8* finis       = u_umlaut + II;
                s32  runa;

        runa = utf8_decodere(&ptr, finis);
        CREDO_AEQUALIS_S32(runa, 0x00FC);  /* u-umlaut */
    }
}


/* ==================================================
 * Probare utf8_decodere - 3 bytes
 * ================================================== */

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
    (vacuum)c;

    {
        /* U+03C0 = pi = CE A0 */
        constans i8  pi[]   = { 0xCE, 0xA0 };
        constans i8* ptr    = pi;
        constans i8* finis  = pi + II;
                s32  runa;

        runa = utf8_decodere(&ptr, finis);
        CREDO_AEQUALIS_S32(runa, 0x03A0);  /* Pi maiusculum */
    }

    {
        /* U+2019 = right single quote = E2 80 99 */
        constans i8  rsquote[]  = { 0xE2, 0x80, 0x99 };
        constans i8* ptr        = rsquote;
        constans i8* finis      = rsquote + III;
                s32  runa;

        runa = utf8_decodere(&ptr, finis);
        CREDO_AEQUALIS_S32(runa, 0x2019);  /* right single quote */
    }

    {
        /* U+201C = left double quote = E2 80 9C */
        constans i8  ldquote[]  = { 0xE2, 0x80, 0x9C };
        constans i8* ptr        = ldquote;
        constans i8* finis      = ldquote + III;
                s32  runa;

        runa = utf8_decodere(&ptr, finis);
        CREDO_AEQUALIS_S32(runa, 0x201C);  /* left double quote */
    }

    {
        /* U+2014 = em dash = E2 80 94 */
        constans i8  emdash[]  = { 0xE2, 0x80, 0x94 };
        constans i8* ptr       = emdash;
        constans i8* finis     = emdash + III;
                s32  runa;

        runa = utf8_decodere(&ptr, finis);
        CREDO_AEQUALIS_S32(runa, 0x2014);  /* em dash */
    }
}


/* ==================================================
 * Probare utf8_decodere - 4 bytes
 * ================================================== */

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
    (vacuum)c;

    {
        /* U+1F600 = grinning face = F0 9F 98 80 */
        constans i8  emoji[]  = { 0xF0, 0x9F, 0x98, 0x80 };
        constans i8* ptr      = emoji;
        constans i8* finis    = emoji + IV;
                s32  runa;

        runa = utf8_decodere(&ptr, finis);
        CREDO_AEQUALIS_S32(runa, 0x1F600);  /* grinning face */
        CREDO_VERUM(ptr == finis);
    }
}


/* ==================================================
 * Probare utf8_decodere - Sequentiae invalidae
 * ================================================== */

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
    (vacuum)c;

    {
        /* Continuatio orphana */
        constans i8  orphana[]  = { 0x80 };
        constans i8* ptr        = orphana;
        constans i8* finis      = orphana + I;
                s32  runa;

        runa = utf8_decodere(&ptr, finis);
        CREDO_AEQUALIS_S32(runa, -I);
        CREDO_VERUM(ptr == finis);  /* Debet promovere */
    }

    {
        /* Sequentia truncata (2-byte incompleta) */
        constans i8  truncata[]  = { 0xC3 };
        constans i8* ptr         = truncata;
        constans i8* finis       = truncata + I;
                s32  runa;

        runa = utf8_decodere(&ptr, finis);
        CREDO_AEQUALIS_S32(runa, -I);
    }

    {
        /* Sequentia truncata (3-byte incompleta) */
        constans i8  truncata3[]  = { 0xE2, 0x80 };
        constans i8* ptr          = truncata3;
        constans i8* finis        = truncata3 + II;
                s32  runa;

        runa = utf8_decodere(&ptr, finis);
        CREDO_AEQUALIS_S32(runa, -I);
    }

    {
        /* Overlong encoding (2-byte pro ASCII) */
        constans i8  overlong[]  = { 0xC0, 0x80 };  /* Overlong NUL */
        constans i8* ptr         = overlong;
        constans i8* finis       = overlong + II;
                s32  runa;

        runa = utf8_decodere(&ptr, finis);
        CREDO_AEQUALIS_S32(runa, -I);
    }

    {
        /* Byte invalidus 0xFF */
        constans i8  invalidus[]  = { 0xFF };
        constans i8* ptr          = invalidus;
        constans i8* finis        = invalidus + I;
                s32  runa;

        runa = utf8_decodere(&ptr, finis);
        CREDO_AEQUALIS_S32(runa, -I);
    }
}


/* ==================================================
 * Probare utf8_decodere - Buffer vacuus
 * ================================================== */

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
    (vacuum)c;

    {
        constans i8  datum[]  = { 'x' };
        constans i8* ptr      = datum;
        constans i8* finis    = datum;  /* ptr == finis */
                s32  runa;

        runa = utf8_decodere(&ptr, finis);
        CREDO_AEQUALIS_S32(runa, -I);
    }
}


/* ==================================================
 * Probare utf8_numerare_runas
 * ================================================== */

interior vacuum
_sectio_nona (
    CredoContextus* c)
{
    (vacuum)c;

    {
        /* ASCII solum */
        constans i8 ascii[] = { 'H', 'e', 'l', 'l', 'o' };
                s32 numerus;

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
}


/* ==================================================
 * Probare utf8_proxima_runa
 * ================================================== */

interior vacuum
_sectio_decima (
    CredoContextus* c)
{
    (vacuum)c;

    {
        constans i8 datum[]  = { 'A', 0xC3, 0xA9, 'B' };  /* A + e-acutum + B */
        constans i8* ptr     = datum;
        constans i8* finis   = datum + IV;

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
}


/* ==================================================
 * Probare utf8_prior_runa
 * ================================================== */

interior vacuum
_sectio_decima_prima (
    CredoContextus* c)
{
    (vacuum)c;

    {
        constans i8 datum[]   = { 'A', 0xC3, 0xA9, 'B' };  /* A + e-acutum + B */
        constans i8* ptr      = datum + IV;  /* Ad finem */
        constans i8* initium  = datum;

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
}


/* ==================================================
 * Probare textum realem Gutenberg
 * ================================================== */

interior vacuum
_sectio_decima_secunda (
    CredoContextus* c)
{
    (vacuum)c;

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
                s32  numerus;
        constans i8* ptr;
        constans i8* finis;
                s32  runa;

        /* Numerare runas */
        numerus = utf8_numerare_runas(gutenberg,
            (s32)magnitudo(gutenberg));
        /* Exspectamus: " H e l l o , " _ h e _ s a i d — " h o w _ a r e _ y o u ? "
         * = 1 + 6 + 1 + 8 + 1 + 1 + 12 + 1 = 31 runae */
        CREDO_AEQUALIS_S32(numerus, XXXI);

        /* Verificare primas runas */
        ptr    = gutenberg;
        finis  = gutenberg + (s32)magnitudo(gutenberg);

        runa = utf8_decodere(&ptr, finis);
        CREDO_AEQUALIS_S32(runa, 0x201C);  /* left double quote */

        runa = utf8_decodere(&ptr, finis);
        CREDO_AEQUALIS_S32(runa, 'H');
    }
}


/* ==================================================
 * utf8_codere - par decodere (circuitus integer)
 * ================================================== */

interior vacuum
_sectio_decima_tertia (
    CredoContextus* c)
{
    (vacuum)c;

    {
                 i8  buffer[4];
        constans i8* ptr;

        /* 1 byte: ASCII */
        CREDO_AEQUALIS_S32(utf8_codere('A', buffer), 1);
        CREDO_AEQUALIS_S32((s32)buffer[0], (s32)'A');

        /* 2 bytes: pi 0x3C0 -> CF 80 */
        CREDO_AEQUALIS_S32(utf8_codere(0x3C0, buffer), 2);
        CREDO_AEQUALIS_S32((s32)buffer[0], 0xCF);
        CREDO_AEQUALIS_S32((s32)buffer[1], 0x80);

        /* 3 bytes: quota sinistra 0x201C -> E2 80 9C */
        CREDO_AEQUALIS_S32(utf8_codere(0x201C, buffer), 3);
        CREDO_AEQUALIS_S32((s32)buffer[0], 0xE2);
        CREDO_AEQUALIS_S32((s32)buffer[1], 0x80);
        CREDO_AEQUALIS_S32((s32)buffer[2], 0x9C);

        /* 4 bytes: 0x1F600 */
        CREDO_AEQUALIS_S32(utf8_codere(0x1F600, buffer), 4);
        CREDO_AEQUALIS_S32((s32)buffer[0], 0xF0);

        /* circuitus: codere -> decodere idem reddit */
        (vacuum)utf8_codere(0x2192, buffer);   /* sagitta -> */
        ptr = buffer;
        CREDO_AEQUALIS_S32(utf8_decodere(&ptr, buffer + 4), 0x2192);

        /* invalida: surrogata, ultra, negativa */
        CREDO_AEQUALIS_S32(utf8_codere(0xD800, buffer), 0);
        CREDO_AEQUALIS_S32(utf8_codere(0xDFFF, buffer), 0);
        CREDO_AEQUALIS_S32(utf8_codere(0x110000, buffer), 0);
        CREDO_AEQUALIS_S32(utf8_codere(-1, buffer), 0);
        CREDO_AEQUALIS_S32(utf8_codere('A', NIHIL), 0);

        /* limites formarum */
        CREDO_AEQUALIS_S32(utf8_codere(0x7F, buffer), 1);
        CREDO_AEQUALIS_S32(utf8_codere(0x80, buffer), 2);
        CREDO_AEQUALIS_S32(utf8_codere(0x7FF, buffer), 2);
        CREDO_AEQUALIS_S32(utf8_codere(0x800, buffer), 3);
        CREDO_AEQUALIS_S32(utf8_codere(0xFFFF, buffer), 3);
        CREDO_AEQUALIS_S32(utf8_codere(0x10000, buffer), 4);
        CREDO_AEQUALIS_S32(utf8_codere(0x10FFFF, buffer), 4);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "utf8_longitudo_byte", _sectio_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "utf8_est_continuatio", _sectio_secunda, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "utf8_decodere ASCII", _sectio_tertia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "utf8_decodere 2-byte", _sectio_quarta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "utf8_decodere 3-byte", _sectio_quinta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "utf8_decodere 4-byte", _sectio_sexta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "utf8_decodere invalidae", _sectio_septima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "utf8_decodere buffer vacuus", _sectio_octava, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "utf8_numerare_runas", _sectio_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "utf8_proxima_runa", _sectio_decima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "utf8_prior_runa", _sectio_decima_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "textum Gutenberg", _sectio_decima_secunda, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "codere", _sectio_decima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("utf8", SECTIONES);
}

/* probatio_chorda_aedificator.c - Probationes ChordaAedificator */
#include "postulata_posix.h"
#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "chorda_aedificator.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>


    /* ==================================================
	 * Probare chorda_aedificator_creare
	 * ================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        ChordaAedificator* aedificator;

        aedificator = chorda_aedificator_creare(piscina, CXXVIII);
        CREDO_NON_NIHIL(aedificator);

        CREDO_AEQUALIS_I32((i32)chorda_aedificator_longitudo(aedificator),
            ZEPHYRUM);
        CREDO_AEQUALIS_I32(chorda_aedificator_indentatio_gradus(aedificator),
            ZEPHYRUM);

        chorda_aedificator_destruere(aedificator);
    }
}


    /* ==================================================
	 * Probare chorda_aedificator_appendere_character
	 * ================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        ChordaAedificator* aedificator;
                   chorda  result;

        aedificator = chorda_aedificator_creare(piscina, CXXVIII);
        CREDO_VERUM(chorda_aedificator_appendere_character(aedificator,
            'H'));
        CREDO_VERUM(chorda_aedificator_appendere_character(aedificator,
            'i'));

        CREDO_AEQUALIS_I32((i32)chorda_aedificator_longitudo(aedificator),
            II);

        result = chorda_aedificator_finire(aedificator);
        CREDO_AEQUALIS_I32(result.mensura, II);
    }
}


    /* ==================================================
	 * Probare chorda_aedificator_appendere_literis
	 * ================================================== */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        ChordaAedificator* aedificator;
                   chorda  result, speratus;

        aedificator = chorda_aedificator_creare(piscina, CXXVIII);
        CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator,
            "salve"));
        CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator,
            " "));
        CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator,
            "munde"));

        result    = chorda_aedificator_finire(aedificator);
        speratus  = chorda_ex_literis("salve munde", piscina);

        CREDO_VERUM(chorda_aequalis(result, speratus));
    }
}


    /* ==================================================
	 * Probare chorda_aedificator_appendere_chorda
	 * ================================================== */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        ChordaAedificator* aedificator;
                   chorda  s1, s2, result, speratus;

        aedificator  = chorda_aedificator_creare(piscina, CXXVIII);
        s1           = chorda_ex_literis("prefix", piscina);
        s2           = chorda_ex_literis("suffix", piscina);

        CREDO_VERUM(chorda_aedificator_appendere_chorda(aedificator,
            s1));
        CREDO_VERUM(chorda_aedificator_appendere_chorda(aedificator,
            s2));

        result    = chorda_aedificator_finire(aedificator);
        speratus  = chorda_ex_literis("prefixsuffix", piscina);

        CREDO_VERUM(chorda_aequalis(result, speratus));
    }
}


    /* ==================================================
	 * Probare chorda_aedificator_appendere_s32
	 * ================================================== */

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        ChordaAedificator* aedificator;
                   chorda  result, speratus;

        aedificator = chorda_aedificator_creare(piscina, CXXVIII);
        CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator,
            "answer="));
        CREDO_VERUM(chorda_aedificator_appendere_s32(aedificator,
            XLII));

        result    = chorda_aedificator_finire(aedificator);
        speratus  = chorda_ex_literis("answer=42", piscina);

        CREDO_VERUM(chorda_aequalis(result, speratus));
    }
}


    /* ==================================================
	 * Probare chorda_aedificator_appendere_i32
	 * ================================================== */

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        ChordaAedificator* aedificator;
                   chorda  result, speratus;

        aedificator = chorda_aedificator_creare(piscina, CXXVIII);
        CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator,
            "count="));
        CREDO_VERUM(chorda_aedificator_appendere_i32(aedificator,
            CCLVI));

        result    = chorda_aedificator_finire(aedificator);
        speratus  = chorda_ex_literis("count=256", piscina);

        CREDO_VERUM(chorda_aequalis(result, speratus));
    }
}


    /* ==================================================
	 * Probare chorda_aedificator_appendere_f64
	 * ================================================== */

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        ChordaAedificator* aedificator;
                   chorda  result, speratus;

        aedificator = chorda_aedificator_creare(piscina, CXXVIII);
        CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator,
            "pi="));
        CREDO_VERUM(chorda_aedificator_appendere_f64(aedificator,
            3.14159,
            II));

        result    = chorda_aedificator_finire(aedificator);
        speratus  = chorda_ex_literis("pi=3.14", piscina);

        CREDO_CHORDA_AEQUALIS(result, speratus);
    }
}


    /* ==================================================
	 * Probare chorda_aedificator_appendere_lineam_novam
	 * ================================================== */

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        ChordaAedificator* aedificator;
                   chorda  result, speratus;

        aedificator = chorda_aedificator_creare(piscina, CXXVIII);
        CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator,
            "primus"));
        CREDO_VERUM(chorda_aedificator_appendere_lineam_novam(aedificator));
        CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator,
            "secundus"));

        result    = chorda_aedificator_finire(aedificator);
        speratus  = chorda_ex_literis("primus\nsecundus", piscina);

        CREDO_CHORDA_AEQUALIS(result, speratus);
    }
}


    /* ==================================================
	 * Probare chorda_aedificator_appendere_indentation
	 * ================================================== */

interior vacuum
_sectio_nona (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        ChordaAedificator* aedificator;
                   chorda  result, speratus;

        aedificator = chorda_aedificator_creare(piscina, CXXVIII);
        /* Gradus 0: nullum indentationem */
        CREDO_VERUM(chorda_aedificator_appendere_indentationem(aedificator,
            ZEPHYRUM));
        CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator,
            "no_indent"));

        CREDO_VERUM(chorda_aedificator_appendere_lineam_novam(aedificator));
        /* Gradus 1: duo spatia */
        CREDO_VERUM(chorda_aedificator_appendere_indentationem(aedificator,
            I));
        CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator,
            "indented"));

        result    = chorda_aedificator_finire(aedificator);
        speratus  = chorda_ex_literis("no_indent\n  indented", piscina);

        CREDO_CHORDA_AEQUALIS(result, speratus);
    }
}


    /* ==================================================
	 * Probare chorda_aedificator_push/pop_indentation
	 * ================================================== */

interior vacuum
_sectio_decima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        ChordaAedificator* aedificator;
                      i32  level1, level2, level3;

        aedificator = chorda_aedificator_creare(piscina, CXXVIII);

        level1 = chorda_aedificator_indentatio_gradus(aedificator);
        CREDO_AEQUALIS_I32(level1, ZEPHYRUM);

        chorda_aedificator_push_indentationem(aedificator);
        level2 = chorda_aedificator_indentatio_gradus(aedificator);
        CREDO_AEQUALIS_I32(level2, I);

        chorda_aedificator_push_indentationem(aedificator);
        level3 = chorda_aedificator_indentatio_gradus(aedificator);
        CREDO_AEQUALIS_I32(level3, II);

        chorda_aedificator_pop_indentationem(aedificator);
        level2 = chorda_aedificator_indentatio_gradus(aedificator);
        CREDO_AEQUALIS_I32(level2, I);

        chorda_aedificator_pop_indentationem(aedificator);
        level1 = chorda_aedificator_indentatio_gradus(aedificator);
        CREDO_AEQUALIS_I32(level1, ZEPHYRUM);

        chorda_aedificator_destruere(aedificator);
    }
}


    /* ==================================================
	 * Probare chorda_aedificator_spectare
	 * ================================================== */

interior vacuum
_sectio_decima_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        ChordaAedificator* aedificator;
                   chorda  view;

        aedificator = chorda_aedificator_creare(piscina, CXXVIII);
        CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator,
            "test"));

        view = chorda_aedificator_spectare(aedificator);
        CREDO_AEQUALIS_I32((i32)view.mensura, IV);

        /* Spectare non debet finire, aedificator adhuc validus */
        CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator,
            "123"));
        CREDO_AEQUALIS_I32((i32)chorda_aedificator_longitudo(aedificator),
            VII);

        chorda_aedificator_destruere(aedificator);
    }
}


    /* ==================================================
	 * Probare chorda_aedificator_reset
	 * ================================================== */

interior vacuum
_sectio_decima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        ChordaAedificator* aedificator;
                   chorda  result1, result2, speratus2;

        aedificator = chorda_aedificator_creare(piscina, CXXVIII);

        CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator,
            "first_string"));
        CREDO_AEQUALIS_I32((i32)chorda_aedificator_longitudo(aedificator),
            XII);

        result1 = chorda_aedificator_finire(aedificator);
        CREDO_AEQUALIS_I32(result1.mensura, XII);

        /* Non potest reficere post finire, sed potest creare aedificatorem novum */
        aedificator = chorda_aedificator_creare(piscina, CXXVIII);
        CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator,
            "second"));
        CREDO_AEQUALIS_I32((i32)chorda_aedificator_longitudo(aedificator),
            VI);

        result2    = chorda_aedificator_finire(aedificator);
        speratus2  = chorda_ex_literis("second", piscina);
        CREDO_VERUM(chorda_aequalis(result2, speratus2));
    }
}


    /* ==================================================
	 * Probare chorda_aedificator_appendere_evasus_json
	 * ================================================== */

interior vacuum
_sectio_decima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        ChordaAedificator* aedificator;
                   chorda  input, result, speratus;

        aedificator = chorda_aedificator_creare(piscina, CXXVIII);
        CREDO_VERUM(chorda_aedificator_appendere_character(aedificator,
            '"'));

        input = chorda_ex_literis("hello\"world", piscina);
        CREDO_VERUM(chorda_aedificator_appendere_evasus_json(aedificator,
            input));

        CREDO_VERUM(chorda_aedificator_appendere_character(aedificator,
            '"'));

        result    = chorda_aedificator_finire(aedificator);
        speratus  = chorda_ex_literis("\"hello\\\"world\"", piscina);

        CREDO_CHORDA_AEQUALIS(result, speratus);
    }
}


    /* ==================================================
	 * Probare chorda_aedificator_appendere_literis_evasus_json
	 * ================================================== */

interior vacuum
_sectio_decima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        ChordaAedificator* aedificator;
                   chorda  result, speratus;

        aedificator = chorda_aedificator_creare(piscina, CXXVIII);
        CREDO_VERUM(chorda_aedificator_appendere_character(aedificator,
            '"'));

        CREDO_VERUM(chorda_aedificator_appendere_literis_evasus_json(
            aedificator, "test\\value"));

        CREDO_VERUM(chorda_aedificator_appendere_character(aedificator,
            '"'));

        result    = chorda_aedificator_finire(aedificator);
        speratus  = chorda_ex_literis("\"test\\\\value\"", piscina);

        CREDO_CHORDA_AEQUALIS(result, speratus);
    }
}


    /* ==================================================
	 * Probare capacity growth
	 * ================================================== */

interior vacuum
_sectio_decima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        ChordaAedificator* aedificator;
                   chorda  result;
                      i32  i;
                character  buffer[VII];

        aedificator = chorda_aedificator_creare(piscina, XVI);  /* Start small */

        /* Appendere satis ut crescentiam cogat */
        per (i = ZEPHYRUM; i < XX; i++)
        {
            snprintf(buffer, VII, "k%d,", i);
            CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator,
                buffer));
        }

        result = chorda_aedificator_finire(aedificator);
        /* Debet accumulavisse omnes introitus (XX × ~3-4 bytes) */
        CREDO_VERUM(result.mensura > XXXII);
    }
}


    /* ==================================================
	 * Probare combined operations (JSON-like structure)
	 * ================================================== */

interior vacuum
_sectio_decima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        ChordaAedificator* aedificator;
                   chorda  result;

        aedificator = chorda_aedificator_creare(piscina, CXXVIII);

        CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator,
            "{"));
        CREDO_VERUM(chorda_aedificator_appendere_lineam_novam(aedificator));

        chorda_aedificator_push_indentationem(aedificator);
        CREDO_VERUM(chorda_aedificator_appendere_indentationem(aedificator,
            chorda_aedificator_indentatio_gradus(aedificator)));
        CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator,
            "\"name\":"));
        CREDO_VERUM(chorda_aedificator_appendere_character(aedificator,
            ' '));
        CREDO_VERUM(chorda_aedificator_appendere_character(aedificator,
            '"'));
        CREDO_VERUM(chorda_aedificator_appendere_literis_evasus_json(
            aedificator, "test"));
        CREDO_VERUM(chorda_aedificator_appendere_character(aedificator,
            '"'));
        CREDO_VERUM(chorda_aedificator_appendere_character(aedificator,
            ','));
        CREDO_VERUM(chorda_aedificator_appendere_lineam_novam(aedificator));

        CREDO_VERUM(chorda_aedificator_appendere_indentationem(aedificator,
            chorda_aedificator_indentatio_gradus(aedificator)));
        CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator,
            "\"value\":"));
        CREDO_VERUM(chorda_aedificator_appendere_character(aedificator,
            ' '));
        CREDO_VERUM(chorda_aedificator_appendere_s32(aedificator,
            XLII));
        CREDO_VERUM(chorda_aedificator_appendere_lineam_novam(aedificator));

        chorda_aedificator_pop_indentationem(aedificator);
        CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator,
            "}"));

        result = chorda_aedificator_finire(aedificator);
        /* Obiectum JSON simplex: ~40 bytes */
        CREDO_VERUM(result.mensura > XXX);
    }
}


    /* ==================================================
	 * Probare NIHIL input handling
	 * ================================================== */

interior vacuum
_sectio_decima_septima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        ChordaAedificator* aedificator;

        aedificator = chorda_aedificator_creare(piscina, CXXVIII);

        /* appendere_literis cum cstr NIHIL debet esse tutum */
        CREDO_FALSUM(chorda_aedificator_appendere_literis(aedificator,
            NIHIL));

        chorda_aedificator_destruere(aedificator);
    }
}


    /* ==================================================
	 * Probare large strings
	 * ================================================== */

interior vacuum
_sectio_decima_octava (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        ChordaAedificator* aedificator;
                   chorda  result;
                      i32  i;

        aedificator = chorda_aedificator_creare(piscina, CXXVIII);

        /* Appendere CCLVI tempora ad aedificandam chordam magnam */
        per (i = ZEPHYRUM; i < CCLVI; i++)
        {
            CREDO_VERUM(chorda_aedificator_appendere_character(aedificator,
                'x'));
        }

        result = chorda_aedificator_finire(aedificator);
        CREDO_AEQUALIS_I32((i32)result.mensura, CCLVI);
    }
}


    /* ==================================================
	 * Probare lineam novam contentum verificatio
	 * ================================================== */

interior vacuum
_sectio_decima_nona (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        ChordaAedificator* aedificator;
                   chorda  result, speratus;

        aedificator = chorda_aedificator_creare(piscina, CXXVIII);
        CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator,
            "linea1"));
        CREDO_VERUM(chorda_aedificator_appendere_lineam_novam(aedificator));
        CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator,
            "linea2"));

        result    = chorda_aedificator_finire(aedificator);
        speratus  = chorda_ex_literis("linea1\nlinea2", piscina);

        CREDO_CHORDA_AEQUALIS(result, speratus);
    }
}


    /* ==================================================
	 * Probare numeri negativi et zephyrum
	 * ================================================== */

interior vacuum
_sectio_vicesima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        ChordaAedificator* aedificator;
                   chorda  result, speratus;

        aedificator = chorda_aedificator_creare(piscina, CXXVIII);
        CREDO_VERUM(chorda_aedificator_appendere_s32(aedificator,
            -XLII));
        CREDO_VERUM(chorda_aedificator_appendere_character(aedificator,
            ','));
        CREDO_VERUM(chorda_aedificator_appendere_s32(aedificator,
            ZEPHYRUM));
        CREDO_VERUM(chorda_aedificator_appendere_character(aedificator,
            ','));
        CREDO_VERUM(chorda_aedificator_appendere_s32(aedificator,
            XLII));

        result    = chorda_aedificator_finire(aedificator);
        speratus  = chorda_ex_literis("-42,0,42", piscina);

        CREDO_CHORDA_AEQUALIS(result, speratus);
    }
}


    /* ==================================================
	 * Probare omnes evasus JSON
	 * ================================================== */

interior vacuum
_sectio_vicesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        ChordaAedificator* aedificator;
                   chorda  input, result, speratus;

        aedificator = chorda_aedificator_creare(piscina, CXXVIII);
        CREDO_VERUM(chorda_aedificator_appendere_character(aedificator,
            '"'));

        input = chorda_ex_literis("test\n\r\t\"\\end", piscina);
        CREDO_VERUM(chorda_aedificator_appendere_evasus_json(aedificator,
            input));

        CREDO_VERUM(chorda_aedificator_appendere_character(aedificator,
            '"'));

        result = chorda_aedificator_finire(aedificator);
        speratus = chorda_ex_literis("\"test\\n\\r\\t\\\"\\\\end\"",
            piscina);

        CREDO_CHORDA_AEQUALIS(result, speratus);
    }
}


        /* ==================================================
     * Probare effugium JSON OMNIUM CCLVI octetorum (2026-09-22) -
     * regula JSON per se (non ctype): < 0x20 et DEL -> \u00xx (\n
     * \r \t formae breves), '"' et '\\' praefixo, ceteri (etiam
     * >= 0x80, octeti UTF-8) crudi. Olim iscntrl((signed char)c):
     * UB pro octetis >= 0x80 et a locale pendens.
     * ================================================== */

interior vacuum
_sectio_vicesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
         constans character* hex = "0123456789abcdef";
                        i32  b;
                        i32  discordes = ZEPHYRUM;

        per (b = ZEPHYRUM; b < CCLVI; b++)
        {
            ChordaAedificator* aedificator;
                           i8  octetus;
                       chorda  input, result;
                    character speratus[VIII];
                          i32 n = ZEPHYRUM;

            octetus        = (i8)b;
            input.datum    = &octetus;
            input.mensura  = I;

            si (b == '"' || b == '\\')
            {
                speratus[n++] = '\\';
                speratus[n++] = (character)b;
            }
            alioquin si (b == '\n' || b == '\r' || b == '\t')
            {
                speratus[n++] = '\\';
                speratus[n++] = (b == '\n') ? 'n' : (b == '\r') ? 'r'
                                                              : 't';
            }
            alioquin si (b < 0x20 || b == 0x7F)
            {
                speratus[n++] = '\\';
                speratus[n++] = 'u';
                speratus[n++] = '0';
                speratus[n++] = '0';
                speratus[n++] = hex[(b >> IV) & 0xF];
                speratus[n++] = hex[b & 0xF];
            }
            alioquin
            {
                speratus[n++] = (character)b;
            }

            aedificator = chorda_aedificator_creare(piscina, XVI);
            CREDO_VERUM(chorda_aedificator_appendere_evasus_json(
                aedificator, input));
            result = chorda_aedificator_finire(aedificator);
            si (   result.mensura != n
                || memcmp(result.datum, speratus,
                          (memoriae_index)n) != ZEPHYRUM)
            {
                imprimere("  DISCORS octetus 0x%02x: [%.*s]\n",
                    (insignatus integer)b, (integer)result.mensura,
                    (character*)result.datum);
                discordes++;
            }
        }
        /* numerus discordium UNUS assertus - singuli supra nominati */
        CREDO_AEQUALIS_I32(discordes, ZEPHYRUM);
    }
}


    /* ==================================================
     * Chorda VACUA: successus, nihil additum (olim FALSUM - effugium
     * nullorum octetorum 'defectus' habebatur)
     * ================================================== */

interior vacuum
_sectio_vicesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        ChordaAedificator* aedificator;
                   chorda  vacua_sine_datis, vacua_cum_datis, result;

        vacua_sine_datis.datum    = NIHIL;
        vacua_sine_datis.mensura  = ZEPHYRUM;
        vacua_cum_datis           = chorda_ex_literis("x", piscina);
        vacua_cum_datis.mensura   = ZEPHYRUM;

        aedificator = chorda_aedificator_creare(piscina, XVI);
        CREDO_VERUM(chorda_aedificator_appendere_character(aedificator,
            '['));
        CREDO_VERUM(chorda_aedificator_appendere_evasus_json(aedificator,
            vacua_sine_datis));
        CREDO_VERUM(chorda_aedificator_appendere_evasus_json(aedificator,
            vacua_cum_datis));
        CREDO_VERUM(chorda_aedificator_appendere_literis_evasus_json(
            aedificator, ""));
        CREDO_VERUM(chorda_aedificator_appendere_character(aedificator,
            ']'));
        result = chorda_aedificator_finire(aedificator);
        CREDO_CHORDA_AEQUALIS(result, chorda_ex_literis("[]", piscina));

        /* aedificator NIHIL manet FALSUM */
        CREDO_FALSUM(chorda_aedificator_appendere_evasus_json(NIHIL,
            vacua_cum_datis));
    }
}


    /* ==================================================
	 * Probare reset verum
	 * ================================================== */

interior vacuum
_sectio_vicesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        ChordaAedificator* aedificator;
                   chorda  result, speratus;

        aedificator = chorda_aedificator_creare(piscina, CXXVIII);
        CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator,
            "first"));
        CREDO_AEQUALIS_I32((i32)chorda_aedificator_longitudo(aedificator),
            V);

        chorda_aedificator_reset(aedificator);
        CREDO_AEQUALIS_I32((i32)chorda_aedificator_longitudo(aedificator),
            ZEPHYRUM);

        CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator,
            "second"));
        result    = chorda_aedificator_finire(aedificator);
        speratus  = chorda_ex_literis("second", piscina);

        CREDO_CHORDA_AEQUALIS(result, speratus);

        chorda_aedificator_destruere(aedificator);
    }
}


    /* ==================================================
	 * Probare spectare multiplex
	 * ================================================== */

interior vacuum
_sectio_vicesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        ChordaAedificator* aedificator;
                   chorda  view1, view2, view3;

        aedificator = chorda_aedificator_creare(piscina, CXXVIII);

        view1 = chorda_aedificator_spectare(aedificator);
        CREDO_AEQUALIS_I32((i32)view1.mensura, ZEPHYRUM);

        CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator,
            "a"));
        view2 = chorda_aedificator_spectare(aedificator);
        CREDO_AEQUALIS_I32((i32)view2.mensura, I);

        CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator,
            "bc"));
        view3 = chorda_aedificator_spectare(aedificator);
        CREDO_AEQUALIS_I32((i32)view3.mensura, III);

        chorda_aedificator_destruere(aedificator);
    }
}


    /* ==================================================
	 * Probare indentatio cum verificatione
	 * ================================================== */

interior vacuum
_sectio_vicesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        ChordaAedificator* aedificator;
                   chorda  result, speratus;

        aedificator = chorda_aedificator_creare(piscina, CXXVIII);

        CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator,
            "a"));
        CREDO_VERUM(chorda_aedificator_appendere_lineam_novam(aedificator));
        CREDO_VERUM(chorda_aedificator_appendere_indentationem(aedificator,
            II));
        CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator,
            "b"));

        result    = chorda_aedificator_finire(aedificator);
        speratus  = chorda_ex_literis("a\n    b", piscina);  /* IV spatia pro gradus II */

        CREDO_CHORDA_AEQUALIS(result, speratus);
    }
}


    /* ==================================================
	 * Probare f64 decimales varii
	 * ================================================== */

interior vacuum
_sectio_vicesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        ChordaAedificator* aedificator;
                   chorda  result, speratus;

        aedificator = chorda_aedificator_creare(piscina, CXXVIII);
        CREDO_VERUM(chorda_aedificator_appendere_f64(aedificator,
            3.14159,
            ZEPHYRUM));
        CREDO_VERUM(chorda_aedificator_appendere_character(aedificator,
            ','));
        CREDO_VERUM(chorda_aedificator_appendere_f64(aedificator, -2.5,
            I));
        CREDO_VERUM(chorda_aedificator_appendere_character(aedificator,
            ','));
        CREDO_VERUM(chorda_aedificator_appendere_f64(aedificator,
            1.23456789,
            V));

        result    = chorda_aedificator_finire(aedificator);
        speratus  = chorda_ex_literis("3,-2.5,1.23457", piscina);

        CREDO_CHORDA_AEQUALIS(result, speratus);
    }
}


    /* ==================================================
	 * Probare contentum post crescentiam
	 * ================================================== */

interior vacuum
_sectio_vicesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        ChordaAedificator* aedificator;
                   chorda  result, speratus;
                      i32 i;

        aedificator = chorda_aedificator_creare(piscina, VIII);  /* Capacitas parva */

        /* Aedificare formam notam quae crescentias multiplas coget */
        per (i = ZEPHYRUM; i < X; i++)
        {
            CREDO_VERUM(chorda_aedificator_appendere_character(aedificator,
                'x'));
        }

        result    = chorda_aedificator_finire(aedificator);
        speratus  = chorda_ex_literis("xxxxxxxxxx", piscina);

        CREDO_CHORDA_AEQUALIS(result, speratus);
    }
}


    /* ==================================================
	 * Probare indentatio limites
	 * ================================================== */

interior vacuum
_sectio_vicesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        ChordaAedificator* aedificator;
                      i32  level;

        aedificator = chorda_aedificator_creare(piscina, CXXVIII);

        /* Pop ad gradum zero tutum */
        chorda_aedificator_pop_indentationem(aedificator);
        level = chorda_aedificator_indentatio_gradus(aedificator);
        CREDO_AEQUALIS_I32(level, ZEPHYRUM);

        /* Push ad maximum M */
        chorda_aedificator_push_indentationem(aedificator);
        CREDO_AEQUALIS_I32(chorda_aedificator_indentatio_gradus(aedificator),
            I);

        chorda_aedificator_destruere(aedificator);
    }
}


    /* ==================================================
	 * Probare chorda continet
	 * ================================================== */

interior vacuum
_sectio_tricesima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        ChordaAedificator* aedificator;
                   chorda  result, needle;

        aedificator = chorda_aedificator_creare(piscina, CXXVIII);
        CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator,
            "Hello "));
        CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator,
            "beautiful "));
        CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator,
            "world"));

        result = chorda_aedificator_finire(aedificator);
        needle = chorda_ex_literis("beautiful", piscina);

        CREDO_CHORDA_CONTINET(result, needle);
    }
}


    /* ==================================================
	 * Probare chorda incipit et terminatur
	 * ================================================== */

interior vacuum
_sectio_tricesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        ChordaAedificator* aedificator;
                   chorda  result, prefixum, suffixum;

        aedificator = chorda_aedificator_creare(piscina, CXXVIII);
        CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator,
            "prefix_middle_suffix"));

        result    = chorda_aedificator_finire(aedificator);
        prefixum  = chorda_ex_literis("prefix", piscina);
        suffixum  = chorda_ex_literis("suffix", piscina);

        CREDO_CHORDA_INCIPIT(result, prefixum);
        CREDO_CHORDA_TERMINATUR(result, suffixum);
    }
}


    /* ==================================================
	 * Probare comparationes integrae
	 * ================================================== */

interior vacuum
_sectio_tricesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
          ChordaAedificator* aedificator;
             memoriae_index  longitudo;

        aedificator = chorda_aedificator_creare(piscina, CXXVIII);
        CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator,
            "test"));

        longitudo = chorda_aedificator_longitudo(aedificator);

        CREDO_MAIOR_I32((i32)longitudo, III);
        CREDO_MINOR_I32((i32)longitudo, V);
        CREDO_MAIOR_AUT_AEQUALIS_I32((i32)longitudo, IV);
        CREDO_MINOR_AUT_AEQUALIS_I32((i32)longitudo, IV);

        chorda_aedificator_destruere(aedificator);
    }
}


    /* ==================================================
	 * Probare appendere_repetita
	 * ================================================== */

interior vacuum
_sectio_tricesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        ChordaAedificator* aedificator;
                    chorda result, speratus;

        /* Repetitio simplex */
        aedificator = chorda_aedificator_creare(piscina, CXXVIII);
        CREDO_VERUM(chorda_aedificator_appendere_repetita(aedificator,
            'x',
            V));
        result    = chorda_aedificator_finire(aedificator);
        speratus  = chorda_ex_literis("xxxxx", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* Repetitio zero */
        aedificator = chorda_aedificator_creare(piscina, CXXVIII);
        CREDO_VERUM(chorda_aedificator_appendere_repetita(aedificator,
            'x',
            ZEPHYRUM));
        result = chorda_aedificator_finire(aedificator);
        CREDO_AEQUALIS_I32(result.mensura, ZEPHYRUM);

        /* Repetitio cum aliis characteribus */
        aedificator = chorda_aedificator_creare(piscina, CXXVIII);
        CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator,
            "hello"));
        CREDO_VERUM(chorda_aedificator_appendere_repetita(aedificator,
            ' ',
            III));
        CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator,
            "world"));
        result    = chorda_aedificator_finire(aedificator);
        speratus  = chorda_ex_literis("hello   world", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* Indentatio */
        aedificator = chorda_aedificator_creare(piscina, CXXVIII);
        CREDO_VERUM(chorda_aedificator_appendere_repetita(aedificator,
            ' ',
            IV));
        CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator,
            "indented"));
        result    = chorda_aedificator_finire(aedificator);
        speratus  = chorda_ex_literis("    indented", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);
    }
}


    /* ==================================================
	 * Probare appendere_hex_i32
	 * ================================================== */

interior vacuum
_sectio_tricesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        ChordaAedificator* aedificator;
                    chorda result, speratus;

        /* Numerus simplex */
        aedificator = chorda_aedificator_creare(piscina, CXXVIII);
        CREDO_VERUM(chorda_aedificator_appendere_hex_i32(aedificator,
            XV));
        result    = chorda_aedificator_finire(aedificator);
        speratus  = chorda_ex_literis("f", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* Numerus maior */
        aedificator = chorda_aedificator_creare(piscina, CXXVIII);
        CREDO_VERUM(chorda_aedificator_appendere_hex_i32(aedificator,
            CCLV));
        result    = chorda_aedificator_finire(aedificator);
        speratus  = chorda_ex_literis("ff", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* Zero */
        aedificator = chorda_aedificator_creare(piscina, CXXVIII);
        CREDO_VERUM(chorda_aedificator_appendere_hex_i32(aedificator,
            ZEPHYRUM));
        result    = chorda_aedificator_finire(aedificator);
        speratus  = chorda_ex_literis("0", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* Cum prefixo */
        aedificator = chorda_aedificator_creare(piscina, CXXVIII);
        CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator,
            "0x"));
        CREDO_VERUM(chorda_aedificator_appendere_hex_i32(aedificator,
            XLIX));
        result    = chorda_aedificator_finire(aedificator);
        speratus  = chorda_ex_literis("0x31", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);
    }
}

/* AMPLIATIO (2026-10-21): crescentia per piscina_conari_ampliare -
 * buffera vetera non relinquuntur, et appendix sui buffer motum
 * superstat. Piscinae propriae: mensura ex statu piscinae pendet. */
interior vacuum
_sectio_tricesima_quinta (
    CredoContextus* c)
{
    (vacuum)c;
    {
                   Piscina* p;
         ChordaAedificator* a;
                    chorda  s;
                       i32  i;
                       b32  recte;

        /* I. C milia characterum ab capacitate XVI: capacitas finalis
         * CXXXI milia (II ad XVII); olim summa duplicationum omnium
         * relinquebatur (~CCLXII milia) */
        p = piscina_generare_dynamicum("ampliatio", IV * MXXIV);
        CREDO_NON_NIHIL (p);
        a = chorda_aedificator_creare(p, XVI);
        per (i = ZEPHYRUM; i < C * M; i++)
        {
            (vacuum)chorda_aedificator_appendere_character(a,
                (character)('a' + (i % XXVI)));
        }
        s = chorda_aedificator_spectare(a);
        CREDO_AEQUALIS_I32 ((i32)s.mensura, C * M);
        recte = VERUM;
        per (i = ZEPHYRUM; i < s.mensura; i++)
        {
            si (s.datum[i] != (i8)('a' + (i % XXVI)))
            {
                recte = FALSUM;
            }
        }
        CREDO_VERUM (recte);
        CREDO_VERUM (piscina_summa_usus(p) < (memoriae_index)(CL * M));
        piscina_destruere(p);

        /* II. appendix sui XII vicibus, crescentia in alveo solo (via
         * II - motus possibilis): "abcde" x II ad XII. Sine
         * re-derivatione fontis lectio ex buffer liberato. */
        p = piscina_generare_dynamicum("ampliatio_sui", LXIV);
        CREDO_NON_NIHIL (p);
        a = chorda_aedificator_creare(p, VIII);
        (vacuum)chorda_aedificator_appendere_literis(a, "abcde");
        per (i = ZEPHYRUM; i < XII; i++)
        {
            s = chorda_aedificator_spectare(a);
            CREDO_VERUM (chorda_aedificator_appendere_chorda(a, s));
        }
        s = chorda_aedificator_spectare(a);
        CREDO_AEQUALIS_I32 ((i32)s.mensura, XX * MXXIV);
        recte = VERUM;
        per (i = ZEPHYRUM; i < s.mensura; i++)
        {
            si (s.datum[i] != (i8)("abcde"[i % V]))
            {
                recte = FALSUM;
            }
        }
        CREDO_VERUM (recte);
        piscina_destruere(p);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "chorda_aedificator_creare", _sectio_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "chorda_aedificator_appendere_character", _sectio_secunda, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "chorda_aedificator_appendere_literis", _sectio_tertia, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "chorda_aedificator_appendere_chorda", _sectio_quarta, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "chorda_aedificator_appendere_s32", _sectio_quinta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "chorda_aedificator_appendere_i32", _sectio_sexta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "chorda_aedificator_appendere_f64", _sectio_septima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "chorda_aedificator_appendere_lineam_novam", _sectio_octava,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "chorda_aedificator_appendere_indentation", _sectio_nona, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "chorda_aedificator_push/pop_indentation", _sectio_decima, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "chorda_aedificator_spectare", _sectio_decima_prima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "chorda_aedificator_reset", _sectio_decima_secunda, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "chorda_aedificator_appendere_evasus_json", _sectio_decima_tertia,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "chorda_aedificator_appendere_literis_evasus_json",
        _sectio_decima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "capacity_crescentia", _sectio_decima_quinta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "combined_operations (JSON pattern)", _sectio_decima_sexta, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "tutelae initus NIHIL", _sectio_decima_septima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "large_strings", _sectio_decima_octava, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "lineam_novam_contentum", _sectio_decima_nona, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "numeri_negativi_et_zephyrum", _sectio_vicesima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "omnes_evasus_json", _sectio_vicesima_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "effugium JSON octetorum CCLVI", _sectio_vicesima_secunda, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "effugium JSON chordae vacuae", _sectio_vicesima_tertia, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "reset_verum", _sectio_vicesima_quarta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "spectare_multiplex", _sectio_vicesima_quinta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "indentatio_cum_verificatione", _sectio_vicesima_sexta, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "f64_decimales_varii", _sectio_vicesima_septima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "contentum_post_crescentiam", _sectio_vicesima_octava, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "indentatio_limites", _sectio_vicesima_nona, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "chorda_continet", _sectio_tricesima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "chorda_incipit_et_terminatur", _sectio_tricesima_prima, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "comparationes_integrae", _sectio_tricesima_secunda, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "appendere_repetita", _sectio_tricesima_tertia, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "appendere_hex_i32", _sectio_tricesima_quarta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "ampliatio: sine bufferis relictis, appendix sui",
        _sectio_tricesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("chorda_aedificator", SECTIONES);
}

/* probatio_chorda.c - Probationes Chordae */
#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

s32 principale (vacuum) 
{
    Piscina* piscina;
        b32  praeteritus;


    /* Aperire credo et piscina */
    piscina = piscina_generare_dynamicum("test_chorda", 4096);
    si (!piscina) 
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ================================================== 
     * Probare chorda_ex_literis
     * ================================================== */

    {
        chorda s1, s2, s3;

        imprimere("\n--- Probans chorda_ex_literis ---\n");

        s1 = chorda_ex_literis("salve", piscina);
        CREDO_AEQUALIS_S32((s32) s1.mensura, V);

        s2 = chorda_ex_literis("", piscina);
        CREDO_AEQUALIS_S32((s32) s2.mensura, ZEPHYRUM);

        s3 = chorda_ex_literis(NIHIL, piscina);
        CREDO_AEQUALIS_S32((s32) s3.mensura, ZEPHYRUM);
    }


    /* =================================================
     * Probare chorda_aequalis
     * ================================================== */

    {
        chorda a, b, c;

        imprimere("\n--- Probans chorda_aequalis ---\n");

        a = chorda_ex_literis("probatio", piscina);
        b = chorda_ex_literis("probatio", piscina);
        c = chorda_ex_literis("altera", piscina);

        CREDO_CHORDA_AEQUALIS(a, b);
        CREDO_FALSUM(chorda_aequalis(a, c));
    }


    /* =================================================
     * Probare chorda_transcribere
     * ================================================== */

    {
        chorda originalis, transcriptus;

        imprimere("\n--- Probans chorda_transcribere ---\n");

        originalis   = chorda_ex_literis("transcriptio", piscina);
        transcriptus = chorda_transcribere(originalis, piscina);

        CREDO_AEQUALIS_I32(transcriptus.mensura, originalis.mensura);
        CREDO_CHORDA_AEQUALIS(transcriptus, originalis);
    }


    /* =================================================
     * Probare chorda_continet
     * ================================================== */

    {
        chorda fenum, acus_inventa, acus_non_inventa;

        imprimere("\n--- Probans chorda_continet ---\n");

        fenum            = chorda_ex_literis("salve munde", piscina);
        acus_inventa     = chorda_ex_literis("munde",       piscina);
        acus_non_inventa = chorda_ex_literis("xyz",         piscina);

        CREDO_CHORDA_CONTINET(fenum, acus_inventa);
        CREDO_FALSUM(chorda_continet(fenum, acus_non_inventa));
    }


    /* =================================================
     * Probare chorda_incipit
     * ================================================== */

    {
        chorda ch, praefixum, non_praefixum;

        imprimere("\n--- Probans chorda_incipit ---\n");

        ch            = chorda_ex_literis("praefixum_suffixum", piscina);
        praefixum     = chorda_ex_literis("praefixum",          piscina);
        non_praefixum = chorda_ex_literis("suffixum",           piscina);

        CREDO_CHORDA_INCIPIT(ch, praefixum);
        CREDO_FALSUM(chorda_incipit(ch, non_praefixum));
    }
 
    /* =================================================
     * Probare chorda_terminatur
     * ================================================== */

    {
        chorda ch2, suffixum, non_suffixum;

        imprimere("\n--- Probans chorda_terminatur ---\n");

        ch2          = chorda_ex_literis("praefixum_suffixum", piscina);
        suffixum     = chorda_ex_literis("suffixum",           piscina);
        non_suffixum = chorda_ex_literis("praefixum",          piscina);

        CREDO_CHORDA_TERMINATUR(ch2, suffixum);
        CREDO_FALSUM(chorda_terminatur(ch2, non_suffixum));
    }


    /* =================================================
     * Probare chorda_sectio
     * ================================================== */

    {
        chorda plenus, sectio, speratus;

        imprimere("\n--- Probans chorda_sectio ---\n");

        plenus = chorda_ex_literis("abcdef", piscina);
        sectio = chorda_sectio(plenus, II, V);

        CREDO_AEQUALIS_I32(sectio.mensura, III);

        speratus = chorda_ex_literis("cde", piscina);
        CREDO_CHORDA_AEQUALIS(sectio, speratus);
    }

    /* =================================================
     * Probare chorda_ex_buffer
     * ================================================== */

    {
            i8  buffer[X] = "testdata";
        chorda  from_buffer = chorda_ex_buffer(buffer, VIII);

        imprimere("\n--- Probans chorda_ex_buffer ---\n");
        
        CREDO_AEQUALIS_I32 (from_buffer.mensura, VIII);
        CREDO_NON_NIHIL    (from_buffer.datum);
    }

    /* =================================================
     * Probare chorda_aequalis_case_insensitivus
     * ================================================== */
    
    {
        chorda lower, upper, mixed1, mixed2, different;

        imprimere("\n--- Probans chorda_aequalis_case_insensitivus ---\n");

        lower     = chorda_ex_literis("salve", piscina);
        upper     = chorda_ex_literis("SALVE", piscina);
        mixed1    = chorda_ex_literis("SaLvE", piscina);
        mixed2    = chorda_ex_literis("sAlVe", piscina);
        different = chorda_ex_literis("vale", piscina);

        CREDO_VERUM  (chorda_aequalis_case_insensitivus(lower, upper));
        CREDO_VERUM  (chorda_aequalis_case_insensitivus(mixed1, mixed2));
        CREDO_VERUM  (chorda_aequalis_case_insensitivus(lower, mixed1));
        CREDO_FALSUM (chorda_aequalis_case_insensitivus(lower, different));
    }

    /* =================================================
     * Probare chorda_comparare
     * ================================================== */

    {
        chorda a, b, c, d;

        imprimere("\n--- Probans chorda_comparare ---\n");

        a = chorda_ex_literis("abc", piscina);
        b = chorda_ex_literis("abc", piscina);
        c = chorda_ex_literis("abd", piscina);
        d = chorda_ex_literis("ab",  piscina);

        CREDO_AEQUALIS_S32(chorda_comparare(a, b), ZEPHYRUM);
        CREDO_MINOR_S32(chorda_comparare(a, c), ZEPHYRUM);    /* a < c */
        CREDO_MAIOR_S32(chorda_comparare(c, a), ZEPHYRUM);    /* c > a */
        CREDO_MAIOR_S32(chorda_comparare(a, d), ZEPHYRUM);    /* a > d (longior) */
    }

    /* =================================================
     * Probare chorda_numerare_occurrentia
     * ================================================== */

    {
        chorda fenum, acus1, acus2, acus3, empty;

        imprimere("\n--- Probans chorda_numerare_occurrentia ---\n");

        fenum = chorda_ex_literis("abcabcabc", piscina);
        acus1 = chorda_ex_literis("abc",       piscina);
        acus2 = chorda_ex_literis("ab",        piscina);
        acus3 = chorda_ex_literis("xyz",       piscina);
        empty = chorda_ex_literis("",          piscina);

        CREDO_AEQUALIS_I32 (chorda_numerare_occurrentia(fenum, acus1), III);
        CREDO_AEQUALIS_I32 (chorda_numerare_occurrentia(fenum, acus2), III);
        CREDO_AEQUALIS_I32 (chorda_numerare_occurrentia(fenum, acus3), ZEPHYRUM);
        CREDO_AEQUALIS_I32 (chorda_numerare_occurrentia(fenum, empty), ZEPHYRUM);
    }

    /* =================================================
     * Probare chorda_minuscula
     * ================================================== */

    {
        chorda original, lowered, expected;

        imprimere("\n--- Probans chorda_minuscula ---\n");

        original = chorda_ex_literis("SaLvE MuNdE", piscina);
        lowered  = chorda_minuscula(original, piscina);
        expected = chorda_ex_literis("salve munde", piscina);

        CREDO_AEQUALIS_I32(lowered.mensura, original.mensura);
        CREDO_CHORDA_AEQUALIS(lowered, expected);
    }


    /* =================================================
     * Probare chorda_maiuscula
     * ================================================== */

    {
        chorda original, uppercased, expected;

        imprimere("\n--- Probans chorda_maiuscula ---\n");

        original   = chorda_ex_literis("SaLvE MuNdE", piscina);
        uppercased = chorda_maiuscula(original, piscina);
        expected   = chorda_ex_literis("SALVE MUNDE", piscina);

        CREDO_AEQUALIS_I32(uppercased.mensura, original.mensura);
        CREDO_CHORDA_AEQUALIS(uppercased, expected);
    }

    /* =================================================
    * Probare chorda_ut_cstr
    * ================================================== */

    {
                    chorda  s;
                 character* cstr_result;
        constans character* expected_cstr = "teststring";

        imprimere("\n--- Probans chorda_ut_cstr ---\n");

        s           = chorda_ex_literis(expected_cstr, piscina);
        cstr_result = chorda_ut_cstr(s, piscina);

        CREDO_NON_NIHIL    (cstr_result);
        CREDO_AEQUALIS_S32 (strcmp(cstr_result, expected_cstr), ZEPHYRUM);
    }

    /* =================================================
    * Probare chorda_ut_s32
    * ================================================== */

    {
        chorda num_string, invalid_string;
           s32 result;
           b32 success;

        imprimere("\n--- Probans chorda_ut_s32 ---\n");

        num_string     = chorda_ex_literis("12345", piscina);
        invalid_string = chorda_ex_literis("notanumber", piscina);

        success = chorda_ut_s32(num_string, &result);
        CREDO_VERUM        (success);
        CREDO_AEQUALIS_S32 (result, 12345);

        success = chorda_ut_s32(invalid_string, &result);
        CREDO_FALSUM (success);
    }

    /* =================================================
    * Probare chorda_ut_i32
    * ================================================== */

    {
        chorda num_string, invalid_string;
           i32 result;
           b32 success;

        imprimere("\n--- Probans chorda_ut_i32 ---\n");

        num_string     = chorda_ex_literis("9876", piscina);
        invalid_string = chorda_ex_literis("notanumber", piscina);

        success = chorda_ut_i32(num_string, &result);
        CREDO_VERUM        (success);
        CREDO_AEQUALIS_I32 (result, 9876);

        success = chorda_ut_i32(invalid_string, &result);
        CREDO_FALSUM (success);
    }

    /* =================================================
     * Probare chorda_concatenare
     * ================================================== */

    {
        chorda s1, s2, concatenatus, speratus;

        imprimere("\n--- Probans chorda_concatenare ---\n");

        s1 = chorda_ex_literis("salve", piscina);
        s2 = chorda_ex_literis("munde", piscina);
        concatenatus = chorda_concatenare(s1, s2, piscina);
        speratus = chorda_ex_literis("salvemunde", piscina);

        CREDO_AEQUALIS_I32(concatenatus.mensura, X);
        CREDO_CHORDA_AEQUALIS(concatenatus, speratus);
    }

    /* =================================================
     * Probare chorda_praecidi_laterale
     * ================================================== */

    {
        chorda original, praecisus, speratus;

        imprimere("\n--- Probans chorda_praecidi_laterale ---\n");

        original = chorda_ex_literis("  salve munde  ", piscina);
        praecisus = chorda_praecidi_laterale(original, piscina);
        speratus = chorda_ex_literis("salve munde", piscina);

        CREDO_CHORDA_AEQUALIS(praecisus, speratus);

        /* Probare cum solis spatia */
        original = chorda_ex_literis("   ", piscina);
        praecisus = chorda_praecidi_laterale(original, piscina);
        CREDO_AEQUALIS_I32(praecisus.mensura, ZEPHYRUM);
    }

    /* =================================================
     * Probare chorda_fissio
     * ================================================== */

    {
        chorda original;
        chorda_fissio_fructus fructus;
        chorda speratus1, speratus2, speratus3;

        imprimere("\n--- Probans chorda_fissio ---\n");

        original = chorda_ex_literis("alice,bob,charlie", piscina);
        fructus = chorda_fissio(original, ',', piscina);

        CREDO_AEQUALIS_I32 (fructus.numerus, III);

        speratus1 = chorda_ex_literis("alice", piscina);
        speratus2 = chorda_ex_literis("bob", piscina);
        speratus3 = chorda_ex_literis("charlie", piscina);

        CREDO_CHORDA_AEQUALIS(fructus.elementa[ZEPHYRUM], speratus1);
        CREDO_CHORDA_AEQUALIS(fructus.elementa[I], speratus2);
        CREDO_CHORDA_AEQUALIS(fructus.elementa[II], speratus3);

        /* Probare cum delimitore finale */
        original = chorda_ex_literis("a,b,", piscina);
        fructus = chorda_fissio(original, ',', piscina);
        CREDO_AEQUALIS_I32 (fructus.numerus, III);

        /* Probare sine delimitore */
        original = chorda_ex_literis("nodelim", piscina);
        fructus = chorda_fissio(original, ',', piscina);
        CREDO_AEQUALIS_I32 (fructus.numerus, I);
    }

    /* =================================================
     * Probare chorda_fissio cum multa elementa (>16)
     * Probat crescentiam array - debet exponere defectum
     * ================================================== */

    {
        chorda original;
        chorda_fissio_fructus fructus;
        chorda speratus;
        i32 i;

        imprimere("\n--- Probans chorda_fissio cum multa elementa ---\n");

        /* Crea chordam cum 20 elementa - initial capacitas XVI
         * Crescentia necessaria ad 16->32 */
        original = chorda_ex_literis("a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t", piscina);
        fructus = chorda_fissio(original, ',', piscina);

        /* Debet habere XX elementa */
        CREDO_AEQUALIS_I32 (fructus.numerus, XX);

        /* Probare primum elementum (ante crescentiam) */
        speratus = chorda_ex_literis("a", piscina);
        CREDO_CHORDA_AEQUALIS(fructus.elementa[ZEPHYRUM], speratus);

        /* Probare decimum sextum elementum (ubi crescentia occurrit) */
        speratus = chorda_ex_literis("p", piscina);
        CREDO_CHORDA_AEQUALIS(fructus.elementa[XV], speratus);

        /* Probare decimum septimum elementum (post crescentiam) */
        speratus = chorda_ex_literis("q", piscina);
        CREDO_CHORDA_AEQUALIS(fructus.elementa[XVI], speratus);

        /* Probare ultimum elementum */
        speratus = chorda_ex_literis("t", piscina);
        CREDO_CHORDA_AEQUALIS(fructus.elementa[XIX], speratus);

        /* Probare omnia elementa systematice */
        per (i = ZEPHYRUM; i < fructus.numerus; i++)
        {
            /* Omnia elementa debent habere mensuram > 0 */
            CREDO_VERUM (fructus.elementa[i].mensura > ZEPHYRUM);
            CREDO_NON_NIHIL (fructus.elementa[i].datum);
        }
    }

    /* =================================================
     * Probare chorda_friare
     * ================================================== */

    {
        chorda s1, s2, s3;
           i32 friatum1, friatum2, friatum3;

        imprimere("\n--- Probans chorda_friare ---\n");

        s1 = chorda_ex_literis("same", piscina);
        s2 = chorda_ex_literis("same", piscina);
        s3 = chorda_ex_literis("different", piscina);

        friatum1 = chorda_friare(s1);
        friatum2 = chorda_friare(s2);
        friatum3 = chorda_friare(s3);

        CREDO_AEQUALIS_I32     (friatum1, friatum2);
        CREDO_INAEQUALITAS_I32 (friatum1, friatum3);
        CREDO_INAEQUALITAS_I32 (friatum1, ZEPHYRUM);
    }


    /* =================================================
     * Probare chorda_ut_s32 cum numeri negativi
     * ================================================== */

    {
        chorda neg_string, zero_string;
           s32 result;
           b32 success;

        imprimere("\n--- Probans chorda_ut_s32_numeri_negativi ---\n");

        neg_string  = chorda_ex_literis("-12345", piscina);
        zero_string = chorda_ex_literis("0", piscina);

        success = chorda_ut_s32(neg_string, &result);
        CREDO_VERUM(success);
        CREDO_AEQUALIS_S32(result, -12345);

        success = chorda_ut_s32(zero_string, &result);
        CREDO_VERUM(success);
        CREDO_AEQUALIS_S32(result, ZEPHYRUM);
    }


    /* =================================================
     * Probare chorda_sectio limites
     * ================================================== */

    {
        chorda original, sectio, speratus;

        imprimere("\n--- Probans chorda_sectio_limites ---\n");

        original = chorda_ex_literis("abcdef", piscina);

        /* Sectio ab initio */
        sectio = chorda_sectio(original, ZEPHYRUM, III);
        speratus = chorda_ex_literis("abc", piscina);
        CREDO_CHORDA_AEQUALIS(sectio, speratus);

        /* Sectio ad finem */
        sectio = chorda_sectio(original, III, VI);
        speratus = chorda_ex_literis("def", piscina);
        CREDO_CHORDA_AEQUALIS(sectio, speratus);

        /* Sectio totum */
        sectio = chorda_sectio(original, ZEPHYRUM, VI);
        CREDO_CHORDA_AEQUALIS(sectio, original);
    }


    /* =================================================
     * Probare chorda_concatenare cum chordis vacuis
     * ================================================== */

    {
        chorda s1, s2, empty, result;

        imprimere("\n--- Probans chorda_concatenare_vacuus ---\n");

        s1    = chorda_ex_literis("hello", piscina);
        s2    = chorda_ex_literis("world", piscina);
        empty = chorda_ex_literis("", piscina);

        /* Concatenare cum vacuo secundo - debet habere s1 */
        result = chorda_concatenare(s1, empty, piscina);
        CREDO_AEQUALIS_I32(result.mensura, s1.mensura);
        CREDO_CHORDA_AEQUALIS(result, s1);

        /* Concatenare cum vacuo primo - debet habere s2 */
        result = chorda_concatenare(empty, s2, piscina);
        CREDO_AEQUALIS_I32(result.mensura, s2.mensura);
        CREDO_CHORDA_AEQUALIS(result, s2);

        /* Concatenare duo vacua */
        result = chorda_concatenare(empty, empty, piscina);
        CREDO_AEQUALIS_I32(result.mensura, ZEPHYRUM);
    }


    /* =================================================
     * Probare chorda_invenire
     * ================================================== */

    {
        chorda fenum, acus, non_inventa;
        chorda result;

        imprimere("\n--- Probans chorda_invenire ---\n");

        fenum       = chorda_ex_literis("salve munde orbis", piscina);
        acus        = chorda_ex_literis("munde", piscina);
        non_inventa = chorda_ex_literis("xyz", piscina);

        /* Invenire acum quae exsistit */
        result = chorda_invenire(fenum, acus);
        CREDO_INAEQUALITAS_I32(result.mensura, ZEPHYRUM);
        CREDO_CHORDA_INCIPIT(result, acus);

        /* Invenire acum quae non exsistit */
        result = chorda_invenire(fenum, non_inventa);
        CREDO_AEQUALIS_I32(result.mensura, ZEPHYRUM);
        CREDO_NIHIL(result.datum);
    }


    /* =================================================
     * Probare chorda_praecidere
     * ================================================== */

    {
        chorda original, praecisus, speratus;

        imprimere("\n--- Probans chorda_praecidere ---\n");

        /* Praecidere cum spatia lateralia */
        original  = chorda_ex_literis("  test  ", piscina);
        praecisus = chorda_praecidere(original);
        speratus  = chorda_ex_literis("test", piscina);
        CREDO_CHORDA_AEQUALIS(praecisus, speratus);

        /* Praecidere cum tabs et spatia */
        original  = chorda_ex_literis("\t\n  test  \n\t", piscina);
        praecisus = chorda_praecidere(original);
        CREDO_CHORDA_AEQUALIS(praecisus, speratus);

        /* Praecidere sine spatia lateralia */
        original  = chorda_ex_literis("test", piscina);
        praecisus = chorda_praecidere(original);
        CREDO_CHORDA_AEQUALIS(praecisus, speratus);
    }


    /* =================================================
     * Compendium
     * ================================================== */

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();

    /* Destructio */
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

/* probatio_chorda.c - Probationes Chordae */
#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>


    /* ================================================== 
     * Probare chorda_ex_literis
     * ================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda s1, s2, s3;

        s1 = chorda_ex_literis("salve", piscina);
        CREDO_AEQUALIS_S32((s32) s1.mensura, V);

        s2 = chorda_ex_literis("", piscina);
        CREDO_AEQUALIS_S32((s32) s2.mensura, ZEPHYRUM);

        s3 = chorda_ex_literis(NIHIL, piscina);
        CREDO_AEQUALIS_S32((s32) s3.mensura, ZEPHYRUM);
    }
}


    /* =================================================
     * Probare chorda_aequalis
     * ================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda a, b, c;

        a = chorda_ex_literis("probatio", piscina);
        b = chorda_ex_literis("probatio", piscina);
        c = chorda_ex_literis("altera", piscina);

        CREDO_CHORDA_AEQUALIS(a, b);
        CREDO_FALSUM(chorda_aequalis(a, c));
    }
}


    /* =================================================
     * Probare chorda_transcribere
     * ================================================== */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda originalis, transcriptus;

        originalis    = chorda_ex_literis("transcriptio", piscina);
        transcriptus  = chorda_transcribere(originalis, piscina);

        CREDO_AEQUALIS_I32(transcriptus.mensura, originalis.mensura);
        CREDO_CHORDA_AEQUALIS(transcriptus, originalis);
    }
}


    /* =================================================
     * Probare chorda_continet
     * ================================================== */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda fenum, acus_inventa, acus_non_inventa;

        fenum             = chorda_ex_literis("salve munde", piscina);
        acus_inventa      = chorda_ex_literis("munde",       piscina);
        acus_non_inventa  = chorda_ex_literis("xyz",         piscina);

        CREDO_CHORDA_CONTINET(fenum, acus_inventa);
        CREDO_FALSUM(chorda_continet(fenum, acus_non_inventa));
    }
}


    /* =================================================
     * Probare chorda_incipit
     * ================================================== */

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda ch, praefixum, non_praefixum;

        ch = chorda_ex_literis("praefixum_suffixum",
            piscina);
        praefixum = chorda_ex_literis("praefixum",
            piscina);
        non_praefixum = chorda_ex_literis("suffixum",
            piscina);

        CREDO_CHORDA_INCIPIT(ch, praefixum);
        CREDO_FALSUM(chorda_incipit(ch, non_praefixum));
    }
}


    /* =================================================
     * Probare chorda_terminatur
     * ================================================== */

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda ch2, suffixum, non_suffixum;

        ch2 = chorda_ex_literis("praefixum_suffixum", piscina);
        suffixum = chorda_ex_literis("suffixum",           piscina);
        non_suffixum = chorda_ex_literis("praefixum",          piscina);

        CREDO_CHORDA_TERMINATUR(ch2, suffixum);
        CREDO_FALSUM(chorda_terminatur(ch2, non_suffixum));
    }
}


    /* =================================================
     * Probare chorda_sectio
     * ================================================== */

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda plenus, sectio, speratus;

        plenus = chorda_ex_literis("abcdef", piscina);
        sectio = chorda_sectio(plenus, II, V);

        CREDO_AEQUALIS_I32(sectio.mensura, III);

        speratus = chorda_ex_literis("cde", piscina);
        CREDO_CHORDA_AEQUALIS(sectio, speratus);
    }
}


    /* =================================================
     * Probare chorda_ex_buffer
     * ================================================== */

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
    (vacuum)c;
    {
            i8 buffer[X]    = "testdata";
        chorda from_buffer  = chorda_ex_buffer(buffer, VIII);

        CREDO_AEQUALIS_I32 (from_buffer.mensura, VIII);
        CREDO_NON_NIHIL    (from_buffer.datum);
    }
}


    /* =================================================
     * Probare chorda_aequalis_case_insensitivus
     * ================================================== */

interior vacuum
_sectio_nona (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda lower, upper, mixed1, mixed2, different;

        lower      = chorda_ex_literis("salve", piscina);
        upper      = chorda_ex_literis("SALVE", piscina);
        mixed1     = chorda_ex_literis("SaLvE", piscina);
        mixed2     = chorda_ex_literis("sAlVe", piscina);
        different  = chorda_ex_literis("vale", piscina);

        CREDO_VERUM  (chorda_aequalis_case_insensitivus(lower, upper));
        CREDO_VERUM  (chorda_aequalis_case_insensitivus(mixed1,
            mixed2));
        CREDO_VERUM  (chorda_aequalis_case_insensitivus(lower, mixed1));
        CREDO_FALSUM (chorda_aequalis_case_insensitivus(lower,
            different));
    }
}


    /* =================================================
     * Probare chorda_comparare
     * ================================================== */

interior vacuum
_sectio_decima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda a, b, c, d;

        a = chorda_ex_literis("abc", piscina);
        b = chorda_ex_literis("abc", piscina);
        c = chorda_ex_literis("abd", piscina);
        d = chorda_ex_literis("ab",  piscina);

        CREDO_AEQUALIS_S32(chorda_comparare(a, b), ZEPHYRUM);
        CREDO_MINOR_S32(chorda_comparare(a, c), ZEPHYRUM);    /* a < c */
        CREDO_MAIOR_S32(chorda_comparare(c, a), ZEPHYRUM);    /* c > a */
        CREDO_MAIOR_S32(chorda_comparare(a, d), ZEPHYRUM);    /* a > d (longior) */
    }
}


    /* =================================================
     * Probare chorda_numerare_occurrentia
     * ================================================== */

interior vacuum
_sectio_decima_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda fenum, acus1, acus2, acus3, empty;

        fenum = chorda_ex_literis("abcabcabc", piscina);
        acus1 = chorda_ex_literis("abc",       piscina);
        acus2 = chorda_ex_literis("ab",        piscina);
        acus3 = chorda_ex_literis("xyz",       piscina);
        empty = chorda_ex_literis("",          piscina);

        CREDO_AEQUALIS_I32 (chorda_numerare_occurrentia(fenum, acus1),
            III);
        CREDO_AEQUALIS_I32 (chorda_numerare_occurrentia(fenum, acus2),
            III);
        CREDO_AEQUALIS_I32 (chorda_numerare_occurrentia(fenum, acus3),
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (chorda_numerare_occurrentia(fenum, empty),
            ZEPHYRUM);
    }
}


    /* =================================================
     * Probare chorda_minuscula
     * ================================================== */

interior vacuum
_sectio_decima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda original, lowered, expected;

        original  = chorda_ex_literis("SaLvE MuNdE", piscina);
        lowered   = chorda_minuscula(original, piscina);
        expected  = chorda_ex_literis("salve munde", piscina);

        CREDO_AEQUALIS_I32(lowered.mensura, original.mensura);
        CREDO_CHORDA_AEQUALIS(lowered, expected);
    }
}


    /* =================================================
     * Probare chorda_maiuscula
     * ================================================== */

interior vacuum
_sectio_decima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda original, uppercased, expected;

        original    = chorda_ex_literis("SaLvE MuNdE", piscina);
        uppercased  = chorda_maiuscula(original, piscina);
        expected    = chorda_ex_literis("SALVE MUNDE", piscina);

        CREDO_AEQUALIS_I32(uppercased.mensura, original.mensura);
        CREDO_CHORDA_AEQUALIS(uppercased, expected);
    }
}


    /* =================================================
    * Probare chorda_ut_cstr
    * ================================================== */

interior vacuum
_sectio_decima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
                    chorda  s;
                 character* cstr_result;
        constans character* expected_cstr = "teststring";

        s            = chorda_ex_literis(expected_cstr, piscina);
        cstr_result  = chorda_ut_cstr(s, piscina);

        CREDO_NON_NIHIL    (cstr_result);
        CREDO_AEQUALIS_S32 (strcmp(cstr_result, expected_cstr),
            ZEPHYRUM);
    }
}


    /* =================================================
    * Probare chorda_ut_s32
    * ================================================== */

interior vacuum
_sectio_decima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda num_string, invalid_string;
           s32 result;
           b32 success;

        num_string      = chorda_ex_literis("12345", piscina);
        invalid_string  = chorda_ex_literis("notanumber", piscina);

        success = chorda_ut_s32(num_string, &result);
        CREDO_VERUM        (success);
        CREDO_AEQUALIS_S32 (result, 12345);

        success = chorda_ut_s32(invalid_string, &result);
        CREDO_FALSUM (success);
    }
}


    /* =================================================
    * Probare chorda_ut_i32
    * ================================================== */

interior vacuum
_sectio_decima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda num_string, invalid_string;
           i32 result;
           b32 success;

        num_string      = chorda_ex_literis("9876", piscina);
        invalid_string  = chorda_ex_literis("notanumber", piscina);

        success = chorda_ut_i32(num_string, &result);
        CREDO_VERUM        (success);
        CREDO_AEQUALIS_I32 (result, 9876);

        success = chorda_ut_i32(invalid_string, &result);
        CREDO_FALSUM (success);
    }
}


    /* =================================================
     * Probare chorda_concatenare
     * ================================================== */

interior vacuum
_sectio_decima_septima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda s1, s2, concatenatus, speratus;

        s1            = chorda_ex_literis("salve", piscina);
        s2            = chorda_ex_literis("munde", piscina);
        concatenatus  = chorda_concatenare(s1, s2, piscina);
        speratus      = chorda_ex_literis("salvemunde", piscina);

        CREDO_AEQUALIS_I32(concatenatus.mensura, X);
        CREDO_CHORDA_AEQUALIS(concatenatus, speratus);
    }
}


    /* =================================================
     * Probare chorda_praecidi_laterale
     * ================================================== */

interior vacuum
_sectio_decima_octava (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda original, praecisus, speratus;

        original   = chorda_ex_literis("  salve munde  ", piscina);
        praecisus  = chorda_praecidi_laterale(original, piscina);
        speratus   = chorda_ex_literis("salve munde", piscina);

        CREDO_CHORDA_AEQUALIS(praecisus, speratus);

        /* Probare cum solis spatia */
        original   = chorda_ex_literis("   ", piscina);
        praecisus  = chorda_praecidi_laterale(original, piscina);
        CREDO_AEQUALIS_I32(praecisus.mensura, ZEPHYRUM);
    }
}


    /* =================================================
     * Probare chorda_fissio
     * ================================================== */

interior vacuum
_sectio_decima_nona (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
                       chorda original;
        chorda_fissio_fructus fructus;
        chorda speratus1, speratus2, speratus3;

        original  = chorda_ex_literis("alice,bob,charlie", piscina);
        fructus   = chorda_fissio(original, ',', piscina);

        CREDO_AEQUALIS_I32 (fructus.numerus, III);

        speratus1 = chorda_ex_literis("alice", piscina);
        speratus2 = chorda_ex_literis("bob", piscina);
        speratus3 = chorda_ex_literis("charlie", piscina);

        CREDO_CHORDA_AEQUALIS(fructus.elementa[ZEPHYRUM], speratus1);
        CREDO_CHORDA_AEQUALIS(fructus.elementa[I], speratus2);
        CREDO_CHORDA_AEQUALIS(fructus.elementa[II], speratus3);

        /* Probare cum delimitore finale */
        original  = chorda_ex_literis("a,b,", piscina);
        fructus   = chorda_fissio(original, ',', piscina);
        CREDO_AEQUALIS_I32 (fructus.numerus, III);

        /* Probare sine delimitore */
        original  = chorda_ex_literis("nodelim", piscina);
        fructus   = chorda_fissio(original, ',', piscina);
        CREDO_AEQUALIS_I32 (fructus.numerus, I);
    }
}


    /* =================================================
     * Probare chorda_fissio cum multa elementa (>16)
     * Probat crescentiam array - debet exponere defectum
     * ================================================== */

interior vacuum
_sectio_vicesima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
                       chorda original;
        chorda_fissio_fructus fructus;
                       chorda speratus;
                          i32 i;

        /* Crea chordam cum 20 elementa - initial capacitas XVI
         * Crescentia necessaria ad 16->32 */
        original =
            chorda_ex_literis("a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t",
            piscina);
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
}


    /* =================================================
     * Probare chorda_friare
     * ================================================== */

interior vacuum
_sectio_vicesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda s1, s2, s3;
           i32 friatum1, friatum2, friatum3;

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
}


    /* =================================================
     * Probare chorda_ut_s32 cum numeri negativi
     * ================================================== */

interior vacuum
_sectio_vicesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda neg_string, zero_string;
           s32 result;
           b32 success;

        neg_string   = chorda_ex_literis("-12345", piscina);
        zero_string  = chorda_ex_literis("0", piscina);

        success = chorda_ut_s32(neg_string, &result);
        CREDO_VERUM(success);
        CREDO_AEQUALIS_S32(result, -12345);

        success = chorda_ut_s32(zero_string, &result);
        CREDO_VERUM(success);
        CREDO_AEQUALIS_S32(result, ZEPHYRUM);
    }
}


    /* =================================================
     * Probare chorda_sectio limites
     * ================================================== */

interior vacuum
_sectio_vicesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda original, sectio, speratus;

        original = chorda_ex_literis("abcdef", piscina);

        /* Sectio ab initio */
        sectio    = chorda_sectio(original, ZEPHYRUM, III);
        speratus  = chorda_ex_literis("abc", piscina);
        CREDO_CHORDA_AEQUALIS(sectio, speratus);

        /* Sectio ad finem */
        sectio    = chorda_sectio(original, III, VI);
        speratus  = chorda_ex_literis("def", piscina);
        CREDO_CHORDA_AEQUALIS(sectio, speratus);

        /* Sectio totum */
        sectio = chorda_sectio(original, ZEPHYRUM, VI);
        CREDO_CHORDA_AEQUALIS(sectio, original);
    }
}


    /* =================================================
     * Probare chorda_concatenare cum chordis vacuis
     * ================================================== */

interior vacuum
_sectio_vicesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda s1, s2, empty, result;

        s1     = chorda_ex_literis("hello", piscina);
        s2     = chorda_ex_literis("world", piscina);
        empty  = chorda_ex_literis("", piscina);

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
}


    /* =================================================
     * Probare chorda_invenire
     * ================================================== */

interior vacuum
_sectio_vicesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda fenum, acus, non_inventa;
        chorda result;

        fenum        = chorda_ex_literis("salve munde orbis", piscina);
        acus         = chorda_ex_literis("munde", piscina);
        non_inventa  = chorda_ex_literis("xyz", piscina);

        /* Invenire acum quae exsistit */
        result = chorda_invenire(fenum, acus);
        CREDO_INAEQUALITAS_I32(result.mensura, ZEPHYRUM);
        CREDO_CHORDA_INCIPIT(result, acus);

        /* Invenire acum quae non exsistit */
        result = chorda_invenire(fenum, non_inventa);
        CREDO_AEQUALIS_I32(result.mensura, ZEPHYRUM);
        CREDO_NIHIL(result.datum);
    }
}


    /* =================================================
     * Probare chorda_praecidere
     * ================================================== */

interior vacuum
_sectio_vicesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda original, praecisus, speratus;

        /* Praecidere cum spatia lateralia */
        original   = chorda_ex_literis("  test  ", piscina);
        praecisus  = chorda_praecidere(original);
        speratus   = chorda_ex_literis("test", piscina);
        CREDO_CHORDA_AEQUALIS(praecisus, speratus);

        /* Praecidere cum tabs et spatia */
        original   = chorda_ex_literis("\t\n  test  \n\t", piscina);
        praecisus  = chorda_praecidere(original);
        CREDO_CHORDA_AEQUALIS(praecisus, speratus);

        /* Praecidere sine spatia lateralia */
        original   = chorda_ex_literis("test", piscina);
        praecisus  = chorda_praecidere(original);
        CREDO_CHORDA_AEQUALIS(praecisus, speratus);
    }
}


    /* =================================================
     * Probare chorda_invenire_index
     * ================================================== */

interior vacuum
_sectio_vicesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda fenum, acus;
           s32 index;

        fenum  = chorda_ex_literis("salve munde orbis", piscina);
        acus   = chorda_ex_literis("munde", piscina);

        /* Invenire acum quae exsistit */
        index = chorda_invenire_index(fenum, acus);
        CREDO_AEQUALIS_S32(index, VI);

        /* Invenire acum ad initium */
        acus   = chorda_ex_literis("salve", piscina);
        index  = chorda_invenire_index(fenum, acus);
        CREDO_AEQUALIS_S32(index, ZEPHYRUM);

        /* Invenire acum quae non exsistit */
        acus   = chorda_ex_literis("xyz", piscina);
        index  = chorda_invenire_index(fenum, acus);
        CREDO_AEQUALIS_S32(index, -I);

        /* Invenire acum vacuam - datum est NIHIL ergo redde -1 */
        acus   = chorda_ex_literis("", piscina);
        index  = chorda_invenire_index(fenum, acus);
        CREDO_AEQUALIS_S32(index, -I);
    }
}


    /* =================================================
     * Probare chorda_invenire_ultimum
     * ================================================== */

interior vacuum
_sectio_vicesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda fenum, acus, result, speratus;

        fenum  = chorda_ex_literis("abc abc abc", piscina);
        acus   = chorda_ex_literis("abc", piscina);

        /* Invenire ultimam occurrentiam */
        result = chorda_invenire_ultimum(fenum, acus);
        CREDO_AEQUALIS_I32(result.mensura, III);
        /* Verificare quod result incipit ad positionem VIII */
        speratus = chorda_ex_literis("abc", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);
        /* Verificare quod datum indicat ad ultimam occurrentiam */
        CREDO_VERUM(result.datum == fenum.datum + VIII);

        /* Non inventus */
        acus    = chorda_ex_literis("xyz", piscina);
        result  = chorda_invenire_ultimum(fenum, acus);
        CREDO_AEQUALIS_I32(result.mensura, ZEPHYRUM);
        CREDO_NIHIL(result.datum);
    }
}


    /* =================================================
     * Probare chorda_invenire_ultimum_index
     * ================================================== */

interior vacuum
_sectio_vicesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda fenum, acus;
           s32 index;

        fenum  = chorda_ex_literis("abc abc abc", piscina);
        acus   = chorda_ex_literis("abc", piscina);

        /* Invenire ultimam positionem */
        index = chorda_invenire_ultimum_index(fenum, acus);
        CREDO_AEQUALIS_S32(index, VIII);

        /* Prima positio */
        fenum = chorda_ex_literis("abcdef", piscina);
        index = chorda_invenire_ultimum_index(fenum, acus);
        CREDO_AEQUALIS_S32(index, ZEPHYRUM);

        /* Non inventus */
        acus   = chorda_ex_literis("xyz", piscina);
        index  = chorda_invenire_ultimum_index(fenum, acus);
        CREDO_AEQUALIS_S32(index, -I);
    }
}


    /* =================================================
     * Probare chorda_praecidere_sinistram
     * ================================================== */

interior vacuum
_sectio_tricesima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda original, result, speratus;

        /* Praecidere spatia a sinistra */
        original  = chorda_ex_literis("   test  ", piscina);
        result    = chorda_praecidere_sinistram(original);
        speratus  = chorda_ex_literis("test  ", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* Sine spatiis sinistris */
        original  = chorda_ex_literis("test  ", piscina);
        result    = chorda_praecidere_sinistram(original);
        CREDO_CHORDA_AEQUALIS(result, original);

        /* Omnia spatia */
        original  = chorda_ex_literis("   ", piscina);
        result    = chorda_praecidere_sinistram(original);
        CREDO_AEQUALIS_I32(result.mensura, ZEPHYRUM);
    }
}


    /* =================================================
     * Probare chorda_praecidere_dextram
     * ================================================== */

interior vacuum
_sectio_tricesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda original, result, speratus;

        /* Praecidere spatia a dextra */
        original  = chorda_ex_literis("  test   ", piscina);
        result    = chorda_praecidere_dextram(original);
        speratus  = chorda_ex_literis("  test", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* Sine spatiis dextris */
        original  = chorda_ex_literis("  test", piscina);
        result    = chorda_praecidere_dextram(original);
        CREDO_CHORDA_AEQUALIS(result, original);

        /* Omnia spatia */
        original  = chorda_ex_literis("   ", piscina);
        result    = chorda_praecidere_dextram(original);
        CREDO_AEQUALIS_I32(result.mensura, ZEPHYRUM);
    }
}


    /* =================================================
     * Probare chorda_substituere
     * ================================================== */

interior vacuum
_sectio_tricesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda original, antiquum, novum, result, speratus;

        /* Substitutio simplex */
        original  = chorda_ex_literis("salve munde", piscina);
        antiquum  = chorda_ex_literis("munde", piscina);
        novum     = chorda_ex_literis("orbis", piscina);
        result = chorda_substituere(original, antiquum, novum,
            piscina);
        speratus = chorda_ex_literis("salve orbis", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* Substitutio multiplex */
        original  = chorda_ex_literis("abc abc abc", piscina);
        antiquum  = chorda_ex_literis("abc", piscina);
        novum     = chorda_ex_literis("xyz", piscina);
        result = chorda_substituere(original, antiquum, novum,
            piscina);
        speratus = chorda_ex_literis("xyz xyz xyz", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* Substitutio cum mensura maiore */
        original  = chorda_ex_literis("a b c", piscina);
        antiquum  = chorda_ex_literis(" ", piscina);
        novum     = chorda_ex_literis("---", piscina);
        result = chorda_substituere(original, antiquum, novum,
            piscina);
        speratus = chorda_ex_literis("a---b---c", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* Substitutio cum mensura minore */
        original  = chorda_ex_literis("hello world", piscina);
        antiquum  = chorda_ex_literis("world", piscina);
        novum     = chorda_ex_literis("!", piscina);
        result = chorda_substituere(original, antiquum, novum,
            piscina);
        speratus = chorda_ex_literis("hello !", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* Non inventus - redde originalem */
        original  = chorda_ex_literis("test", piscina);
        antiquum  = chorda_ex_literis("xyz", piscina);
        novum     = chorda_ex_literis("abc", piscina);
        result = chorda_substituere(original, antiquum, novum,
            piscina);
        CREDO_CHORDA_AEQUALIS(result, original);
    }
}


    /* =================================================
     * Probare chorda_invertere
     * ================================================== */

interior vacuum
_sectio_tricesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda original, result, speratus;

        /* Invertere chordam simplicem */
        original  = chorda_ex_literis("abcdef", piscina);
        result    = chorda_invertere(original, piscina);
        speratus  = chorda_ex_literis("fedcba", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* Invertere chordam imparem */
        original  = chorda_ex_literis("abc", piscina);
        result    = chorda_invertere(original, piscina);
        speratus  = chorda_ex_literis("cba", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* Invertere chordam unius characteris */
        original  = chorda_ex_literis("x", piscina);
        result    = chorda_invertere(original, piscina);
        CREDO_CHORDA_AEQUALIS(result, original);

        /* Invertere chordam vacuam */
        original  = chorda_ex_literis("", piscina);
        result    = chorda_invertere(original, piscina);
        CREDO_AEQUALIS_I32(result.mensura, ZEPHYRUM);
    }
}


    /* =================================================
     * Probare chorda_duplicare
     * ================================================== */

interior vacuum
_sectio_tricesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda original, result, speratus;

        /* Duplicare ter */
        original  = chorda_ex_literis("ab", piscina);
        result    = chorda_duplicare(original, III, piscina);
        speratus  = chorda_ex_literis("ababab", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* Duplicare semel (copia) */
        original  = chorda_ex_literis("test", piscina);
        result    = chorda_duplicare(original, I, piscina);
        CREDO_CHORDA_AEQUALIS(result, original);

        /* Duplicare nullis vicibus */
        result = chorda_duplicare(original, ZEPHYRUM, piscina);
        CREDO_AEQUALIS_I32(result.mensura, ZEPHYRUM);

        /* Duplicare chordam vacuam */
        original  = chorda_ex_literis("", piscina);
        result    = chorda_duplicare(original, V, piscina);
        CREDO_AEQUALIS_I32(result.mensura, ZEPHYRUM);
    }
}


    /* =================================================
     * Probare chorda_pascalis
     * ================================================== */

interior vacuum
_sectio_tricesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda original, result, speratus;

        /* Spatia verba separant */
        original  = chorda_ex_literis("hello world", piscina);
        result    = chorda_pascalis(original, piscina);
        speratus  = chorda_ex_literis("HelloWorld", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* Underscore separatores */
        original  = chorda_ex_literis("hello_world", piscina);
        result    = chorda_pascalis(original, piscina);
        speratus  = chorda_ex_literis("HelloWorld", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* Hyphen separatores */
        original  = chorda_ex_literis("hello-world", piscina);
        result    = chorda_pascalis(original, piscina);
        speratus  = chorda_ex_literis("HelloWorld", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* camelCase input */
        original  = chorda_ex_literis("helloWorld", piscina);
        result    = chorda_pascalis(original, piscina);
        speratus  = chorda_ex_literis("HelloWorld", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* Iam PascalCase */
        original  = chorda_ex_literis("HelloWorld", piscina);
        result    = chorda_pascalis(original, piscina);
        speratus  = chorda_ex_literis("HelloWorld", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* SCREAMING_CASE */
        original  = chorda_ex_literis("HELLO_WORLD", piscina);
        result    = chorda_pascalis(original, piscina);
        speratus  = chorda_ex_literis("HelloWorld", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* Cum numeris */
        original  = chorda_ex_literis("test123abc", piscina);
        result    = chorda_pascalis(original, piscina);
        speratus  = chorda_ex_literis("Test123Abc", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* Chorda vacua */
        original  = chorda_ex_literis("", piscina);
        result    = chorda_pascalis(original, piscina);
        CREDO_AEQUALIS_I32(result.mensura, ZEPHYRUM);

        /* Unus character */
        original  = chorda_ex_literis("a", piscina);
        result    = chorda_pascalis(original, piscina);
        speratus  = chorda_ex_literis("A", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* Cum interpunctione */
        original  = chorda_ex_literis("Hello, World!", piscina);
        result    = chorda_pascalis(original, piscina);
        speratus  = chorda_ex_literis("HelloWorld", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* getElementById */
        original  = chorda_ex_literis("getElementById", piscina);
        result    = chorda_pascalis(original, piscina);
        speratus  = chorda_ex_literis("GetElementById", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);
    }
}


    /* =================================================
     * Probare chorda_camelus
     * ================================================== */

interior vacuum
_sectio_tricesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda original, result, speratus;

        /* Spatia verba separant */
        original  = chorda_ex_literis("hello world", piscina);
        result    = chorda_camelus(original, piscina);
        speratus  = chorda_ex_literis("helloWorld", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* PascalCase ad camelCase */
        original  = chorda_ex_literis("HelloWorld", piscina);
        result    = chorda_camelus(original, piscina);
        speratus  = chorda_ex_literis("helloWorld", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* SCREAMING_CASE */
        original  = chorda_ex_literis("HELLO_WORLD", piscina);
        result    = chorda_camelus(original, piscina);
        speratus  = chorda_ex_literis("helloWorld", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* Iam camelCase */
        original  = chorda_ex_literis("helloWorld", piscina);
        result    = chorda_camelus(original, piscina);
        speratus  = chorda_ex_literis("helloWorld", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* Unus character */
        original  = chorda_ex_literis("A", piscina);
        result    = chorda_camelus(original, piscina);
        speratus  = chorda_ex_literis("a", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);
    }
}


    /* =================================================
     * Probare chorda_serpens
     * ================================================== */

interior vacuum
_sectio_tricesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda original, result, speratus;

        /* PascalCase ad snake_case */
        original  = chorda_ex_literis("HelloWorld", piscina);
        result    = chorda_serpens(original, piscina);
        speratus  = chorda_ex_literis("hello_world", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* camelCase ad snake_case */
        original  = chorda_ex_literis("getElementById", piscina);
        result    = chorda_serpens(original, piscina);
        speratus  = chorda_ex_literis("get_element_by_id", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* Iam snake_case */
        original  = chorda_ex_literis("already_snake", piscina);
        result    = chorda_serpens(original, piscina);
        speratus  = chorda_ex_literis("already_snake", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* Spatia */
        original  = chorda_ex_literis("hello world", piscina);
        result    = chorda_serpens(original, piscina);
        speratus  = chorda_ex_literis("hello_world", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* XMLParser - acronym */
        original  = chorda_ex_literis("XMLParser", piscina);
        result    = chorda_serpens(original, piscina);
        speratus  = chorda_ex_literis("xml_parser", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* Unus character */
        original  = chorda_ex_literis("A", piscina);
        result    = chorda_serpens(original, piscina);
        speratus  = chorda_ex_literis("a", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);
    }
}


    /* =================================================
     * Probare chorda_kebab
     * ================================================== */

interior vacuum
_sectio_tricesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda original, result, speratus;

        /* PascalCase ad kebab-case */
        original  = chorda_ex_literis("HelloWorld", piscina);
        result    = chorda_kebab(original, piscina);
        speratus  = chorda_ex_literis("hello-world", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* camelCase ad kebab-case */
        original  = chorda_ex_literis("getElementById", piscina);
        result    = chorda_kebab(original, piscina);
        speratus  = chorda_ex_literis("get-element-by-id", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* Spatia */
        original  = chorda_ex_literis("hello world", piscina);
        result    = chorda_kebab(original, piscina);
        speratus  = chorda_ex_literis("hello-world", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* snake_case ad kebab-case */
        original  = chorda_ex_literis("hello_world", piscina);
        result    = chorda_kebab(original, piscina);
        speratus  = chorda_ex_literis("hello-world", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);
    }
}


    /* =================================================
     * Probare chorda_pascalis_serpens
     * ================================================== */

interior vacuum
_sectio_tricesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda original, result, speratus;

        /* Spatia */
        original  = chorda_ex_literis("hello world", piscina);
        result    = chorda_pascalis_serpens(original, piscina);
        speratus  = chorda_ex_literis("Hello_World", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* camelCase ad Pascal_Snake */
        original  = chorda_ex_literis("getElementById", piscina);
        result    = chorda_pascalis_serpens(original, piscina);
        speratus  = chorda_ex_literis("Get_Element_By_Id", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* PascalCase ad Pascal_Snake */
        original  = chorda_ex_literis("HelloWorld", piscina);
        result    = chorda_pascalis_serpens(original, piscina);
        speratus  = chorda_ex_literis("Hello_World", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* Unus character */
        original  = chorda_ex_literis("a", piscina);
        result    = chorda_pascalis_serpens(original, piscina);
        speratus  = chorda_ex_literis("A", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);
    }
}


    /* =================================================
     * Probare conversiones casus - casus limites
     * ================================================== */

interior vacuum
_sectio_quadragesima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda original, result, speratus;

        /* Omnes maiusculae "ABC" */
        original  = chorda_ex_literis("ABC", piscina);
        result    = chorda_serpens(original, piscina);
        speratus  = chorda_ex_literis("abc", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* Omnes minusculae "abc" */
        original  = chorda_ex_literis("abc", piscina);
        result    = chorda_pascalis(original, piscina);
        speratus  = chorda_ex_literis("Abc", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* Numeri solum "123" */
        original  = chorda_ex_literis("123", piscina);
        result    = chorda_pascalis(original, piscina);
        speratus  = chorda_ex_literis("123", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* Interpunctio solum - debet reddere vacuum */
        original  = chorda_ex_literis("!!!", piscina);
        result    = chorda_pascalis(original, piscina);
        CREDO_AEQUALIS_I32(result.mensura, ZEPHYRUM);

        /* Mixta "Hello, World! 123" */
        original  = chorda_ex_literis("Hello, World! 123", piscina);
        result    = chorda_kebab(original, piscina);
        speratus  = chorda_ex_literis("hello-world-123", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* Underscores duplices */
        original  = chorda_ex_literis("hello__world", piscina);
        result    = chorda_pascalis(original, piscina);
        speratus  = chorda_ex_literis("HelloWorld", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* HTTPSConnection - acronym in medio */
        original  = chorda_ex_literis("HTTPSConnection", piscina);
        result    = chorda_serpens(original, piscina);
        speratus  = chorda_ex_literis("https_connection", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);
    }
}


    /* =================================================
     * Probatio chorda_ut_f64
     * ================================================== */

interior vacuum
_sectio_quadragesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda s;
           f64 valor;
           b32 successus;

        /* Numerus integer */
        s          = chorda_ex_literis("42", piscina);
        successus  = chorda_ut_f64(s, &valor);
        CREDO_VERUM(successus);
        CREDO_VERUM(valor > 41.9 && valor < 42.1);

        /* Numerus decimalis */
        s          = chorda_ex_literis("3.14159", piscina);
        successus  = chorda_ut_f64(s, &valor);
        CREDO_VERUM(successus);
        CREDO_VERUM(valor > 3.14 && valor < 3.15);

        /* Numerus negativus */
        s          = chorda_ex_literis("-123.45", piscina);
        successus  = chorda_ut_f64(s, &valor);
        CREDO_VERUM(successus);
        CREDO_VERUM(valor < -123.4 && valor > -123.5);

        /* Chorda invalida */
        s          = chorda_ex_literis("abc", piscina);
        successus  = chorda_ut_f64(s, &valor);
        CREDO_FALSUM(successus);

        /* Chorda mixta invalida */
        s          = chorda_ex_literis("42abc", piscina);
        successus  = chorda_ut_f64(s, &valor);
        CREDO_FALSUM(successus);

        /* Chorda vacua */
        s.datum    = NIHIL;
        s.mensura  = ZEPHYRUM;
        successus  = chorda_ut_f64(s, &valor);
        CREDO_FALSUM(successus);
    }
}


    /* =================================================
     * Probatio chorda_ex_s32
     * ================================================== */

interior vacuum
_sectio_quadragesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda result, speratus;

        /* Numerus positivus */
        result    = chorda_ex_s32(42, piscina);
        speratus  = chorda_ex_literis("42", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* Numerus negativus */
        result    = chorda_ex_s32(-123, piscina);
        speratus  = chorda_ex_literis("-123", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* Zero */
        result    = chorda_ex_s32(ZEPHYRUM, piscina);
        speratus  = chorda_ex_literis("0", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);
    }
}


    /* =================================================
     * Probatio chorda_ex_f64
     * ================================================== */

interior vacuum
_sectio_quadragesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda result, speratus;

        /* Numerus cum 2 decimales */
        result    = chorda_ex_f64(3.14159, II, piscina);
        speratus  = chorda_ex_literis("3.14", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* Numerus cum 0 decimales */
        result    = chorda_ex_f64(42.9, ZEPHYRUM, piscina);
        speratus  = chorda_ex_literis("43", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* Numerus negativus */
        result    = chorda_ex_f64(-1.5, I, piscina);
        speratus  = chorda_ex_literis("-1.5", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);
    }
}


    /* =================================================
     * Probatio chorda_character_ad
     * ================================================== */

interior vacuum
_sectio_quadragesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda s, result, speratus;

        s = chorda_ex_literis("hello", piscina);

        /* Primus character */
        result    = chorda_character_ad(s, ZEPHYRUM, piscina);
        speratus  = chorda_ex_literis("h", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* Medius character */
        result    = chorda_character_ad(s, II, piscina);
        speratus  = chorda_ex_literis("l", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* Ultimus character */
        result    = chorda_character_ad(s, IV, piscina);
        speratus  = chorda_ex_literis("o", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* Index extra limites */
        result = chorda_character_ad(s, X, piscina);
        CREDO_AEQUALIS_I32(result.mensura, ZEPHYRUM);

        /* Index negativus - skipped (i32 is unsigned) */
    }
}


    /* =================================================
     * Probatio chorda_vacua
     * ================================================== */

interior vacuum
_sectio_quadragesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda s;

        /* Chorda vacua (NIHIL) */
        s.datum    = NIHIL;
        s.mensura  = ZEPHYRUM;
        CREDO_VERUM(chorda_vacua(s));

        /* Chorda vacua (mensura zero) */
        {
            i8 temp_buffer[]  = "test";
            s.datum           = temp_buffer;
            s.mensura         = ZEPHYRUM;
            CREDO_VERUM(chorda_vacua(s));
        }

        /* Chorda non vacua */
        s = chorda_ex_literis("hello", piscina);
        CREDO_FALSUM(chorda_vacua(s));
    }
}


    /* =================================================
     * Probatio chorda_fissio_chorda
     * ================================================== */

interior vacuum
_sectio_quadragesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda s, delim;
        chorda_fissio_fructus fructus;

        /* Delimitator simplex */
        s        = chorda_ex_literis("a::b::c", piscina);
        delim    = chorda_ex_literis("::", piscina);
        fructus  = chorda_fissio_chorda(s, delim, piscina);
        CREDO_AEQUALIS_I32(fructus.numerus, III);
        CREDO_CHORDA_AEQUALIS(fructus.elementa[ZEPHYRUM],
            chorda_ex_literis("a", piscina));
        CREDO_CHORDA_AEQUALIS(fructus.elementa[I],
            chorda_ex_literis("b", piscina));
        CREDO_CHORDA_AEQUALIS(fructus.elementa[II],
            chorda_ex_literis("c", piscina));

        /* Delimitator in initio */
        s        = chorda_ex_literis("::a::b", piscina);
        fructus  = chorda_fissio_chorda(s, delim, piscina);
        CREDO_AEQUALIS_I32(fructus.numerus, III);
        CREDO_AEQUALIS_I32(fructus.elementa[ZEPHYRUM].mensura,
            ZEPHYRUM);

        /* Delimitator in fine */
        s        = chorda_ex_literis("a::b::", piscina);
        fructus  = chorda_fissio_chorda(s, delim, piscina);
        CREDO_AEQUALIS_I32(fructus.numerus, III);
        CREDO_AEQUALIS_I32(fructus.elementa[II].mensura, ZEPHYRUM);

        /* Sine delimitatore */
        s        = chorda_ex_literis("abc", piscina);
        fructus  = chorda_fissio_chorda(s, delim, piscina);
        CREDO_AEQUALIS_I32(fructus.numerus, I);
        CREDO_CHORDA_AEQUALIS(fructus.elementa[ZEPHYRUM],
            chorda_ex_literis("abc", piscina));

        /* Delimitator vacuus */
        delim.datum    = NIHIL;
        delim.mensura  = ZEPHYRUM;
        fructus        = chorda_fissio_chorda(s, delim, piscina);
        CREDO_AEQUALIS_I32(fructus.numerus, I);
    }
}


    /* =================================================
     * Probatio chorda_iungere
     * ================================================== */

interior vacuum
_sectio_quadragesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda elementa[IV];
        chorda separator, result, speratus;

        /* Iunctio simplex */
        elementa[ZEPHYRUM] = chorda_ex_literis("a", piscina);
        elementa[I] = chorda_ex_literis("b", piscina);
        elementa[II] = chorda_ex_literis("c", piscina);
        separator = chorda_ex_literis(",", piscina);
        result = chorda_iungere(elementa, III, separator, piscina);
        speratus = chorda_ex_literis("a,b,c", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* Iunctio cum separatore longiori */
        separator  = chorda_ex_literis(" - ", piscina);
        result     = chorda_iungere(elementa, III, separator, piscina);
        speratus   = chorda_ex_literis("a - b - c", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* Iunctio sine separatore */
        separator.datum = NIHIL;
        separator.mensura = ZEPHYRUM;
        result = chorda_iungere(elementa, III, separator, piscina);
        speratus = chorda_ex_literis("abc", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* Unum elementum */
        separator  = chorda_ex_literis(",", piscina);
        result     = chorda_iungere(elementa, I, separator, piscina);
        speratus   = chorda_ex_literis("a", piscina);
        CREDO_CHORDA_AEQUALIS(result, speratus);

        /* Zero elementa */
        result = chorda_iungere(elementa, ZEPHYRUM, separator, piscina);
        CREDO_AEQUALIS_I32(result.mensura, ZEPHYRUM);
    }
}


interior vacuum
_sectio_quadragesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
                    chorda  s;
                       s64  v;
                        i8  cum_nullo[III];
                       f64  r;
                       i32  i;
        constans character* bona[V];
                       s64  exspectata[V];

        bona[0]        = "0";
        exspectata[0]  = 0;
        bona[1]        = "-1";
        exspectata[1]  = -1;
        bona[2]        = "9223372036854775807";
        exspectata[2]  = 9223372036854775807LL;
        bona[3]        = "-9223372036854775808";
        exspectata[3]  = -9223372036854775807LL - 1LL;
        bona[4]        = "0000000000000000000012";
        exspectata[4]  = 12;

        per (i = ZEPHYRUM; i < V; i++)
        {
            CREDO_VERUM(chorda_ut_s64(
                chorda_ex_literis(bona[i], piscina), &v));
            CREDO_AEQUALIS_S64(v, exspectata[i]);
        }

        /* iter reditus: ex_s64 -> ut_s64 idem */
        per (i = ZEPHYRUM; i < V; i++)
        {
            CREDO_VERUM(chorda_ut_s64(
                chorda_ex_s64(exspectata[i], piscina), &v));
            CREDO_AEQUALIS_S64(v, exspectata[i]);
        }
        CREDO_CHORDA_AEQUALIS_LITERIS(chorda_ex_s64(0, piscina), "0");
        CREDO_CHORDA_AEQUALIS_LITERIS(chorda_ex_s64(-1, piscina), "-1");
        CREDO_CHORDA_AEQUALIS_LITERIS(
            chorda_ex_s64(-9223372036854775807LL - 1LL, piscina),
            "-9223372036854775808");

        /* superfluitas, sordes, vacuum */
        CREDO_FALSUM(chorda_ut_s64(
            chorda_ex_literis("9223372036854775808", piscina), &v));
        CREDO_FALSUM(chorda_ut_s64(
            chorda_ex_literis("99999999999999999999", piscina), &v));
        CREDO_FALSUM(chorda_ut_s64(
            chorda_ex_literis("12x", piscina), &v));
        CREDO_FALSUM(chorda_ut_s64(
            chorda_ex_literis("-", piscina), &v));

        /* NULLUM insertum: familia tota recusat */
        cum_nullo[0]  = (i8)'1';
        cum_nullo[1]  = (i8)ZEPHYRUM;
        cum_nullo[2]  = (i8)'2';
        s.datum       = cum_nullo;
        s.mensura     = III;
        CREDO_FALSUM(chorda_ut_s64(s, &v));
        {
            s32 a;
            i32 b;

            CREDO_FALSUM(chorda_ut_s32(s, &a));
            CREDO_FALSUM(chorda_ut_i32(s, &b));
            CREDO_FALSUM(chorda_ut_f64(s, &r));
        }
    }
}

    /* ---- superfluitas s32 / i32: truncatio tacita recusata ---- */

interior vacuum
_sectio_quadragesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        s32 a;
        i32 b;

        CREDO_VERUM(chorda_ut_s32(
            chorda_ex_literis("2147483647", piscina), &a));
        CREDO_AEQUALIS_S32(a, 2147483647L);
        CREDO_VERUM(chorda_ut_s32(
            chorda_ex_literis("-2147483648", piscina), &a));
        CREDO_AEQUALIS_S32(a, -2147483647L - 1L);
        CREDO_FALSUM(chorda_ut_s32(
            chorda_ex_literis("2147483648", piscina), &a));
        CREDO_FALSUM(chorda_ut_s32(
            chorda_ex_literis("99999999999", piscina), &a));

        CREDO_VERUM(chorda_ut_i32(
            chorda_ex_literis("4294967295", piscina), &b));
        CREDO_FALSUM(chorda_ut_i32(
            chorda_ex_literis("4294967296", piscina), &b));
        /* strtoul '-1' circumvolvit - nunc recusatur */
        CREDO_FALSUM(chorda_ut_i32(
            chorda_ex_literis("-1", piscina), &b));
    }
}

    /* ---- f64 exacta: iter reditus duplicis ---- */

interior vacuum
_sectio_quinquagesima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        f64 valores[III];
        f64 reditus;
        i32 i;

        valores[0] = 1e300;
        valores[1] = 1e-20;
        valores[2] = 0.1;

        per (i = ZEPHYRUM; i < III; i++)
        {
            CREDO_VERUM(chorda_ut_f64(
                chorda_ex_f64_exacta(valores[i], piscina), &reditus));
            CREDO_VERUM(reditus == valores[i]);
        }
        /* forma vetus 1e300 non capit: buffer CXXXII octetorum, sed
         * snprintf longitudinem VERAM (CCCVII) reddit - memcpy sine
         * custodia trans buffer legebat. Nunc chorda VACUA. */
        CREDO_CHORDA_VACUA(chorda_ex_f64(1e300, VI, piscina));
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "chorda_ex_literis", _sectio_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "chorda_aequalis", _sectio_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "chorda_transcribere", _sectio_tertia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "chorda_continet", _sectio_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "chorda_incipit", _sectio_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "chorda_terminatur", _sectio_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "chorda_sectio", _sectio_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "chorda_ex_buffer", _sectio_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "chorda_aequalis_case_insensitivus", _sectio_nona, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "chorda_comparare", _sectio_decima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "chorda_numerare_occurrentia", _sectio_decima_prima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "chorda_minuscula", _sectio_decima_secunda, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "chorda_maiuscula", _sectio_decima_tertia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "chorda_ut_cstr", _sectio_decima_quarta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "chorda_ut_s32", _sectio_decima_quinta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "chorda_ut_i32", _sectio_decima_sexta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "chorda_concatenare", _sectio_decima_septima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "chorda_praecidi_laterale", _sectio_decima_octava, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "chorda_fissio", _sectio_decima_nona, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "chorda_fissio cum multa elementa", _sectio_vicesima, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "chorda_friare", _sectio_vicesima_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "chorda_ut_s32_numeri_negativi", _sectio_vicesima_secunda, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "chorda_sectio_limites", _sectio_vicesima_tertia, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "chorda_concatenare_vacuus", _sectio_vicesima_quarta, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "chorda_invenire", _sectio_vicesima_quinta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "chorda_praecidere", _sectio_vicesima_sexta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "chorda_invenire_index", _sectio_vicesima_septima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "chorda_invenire_ultimum", _sectio_vicesima_octava, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "chorda_invenire_ultimum_index", _sectio_vicesima_nona, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "chorda_praecidere_sinistram", _sectio_tricesima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "chorda_praecidere_dextram", _sectio_tricesima_prima, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "chorda_substituere", _sectio_tricesima_secunda, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "chorda_invertere", _sectio_tricesima_tertia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "chorda_duplicare", _sectio_tricesima_quarta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "chorda_pascalis", _sectio_tricesima_quinta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "chorda_camelus", _sectio_tricesima_sexta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "chorda_serpens", _sectio_tricesima_septima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "chorda_kebab", _sectio_tricesima_octava, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "chorda_pascalis_serpens", _sectio_tricesima_nona, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "conversiones_casus_limites", _sectio_quadragesima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "chorda_ut_f64", _sectio_quadragesima_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "chorda_ex_s32", _sectio_quadragesima_secunda, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "chorda_ex_f64", _sectio_quadragesima_tertia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "chorda_character_ad", _sectio_quadragesima_quarta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "chorda_vacua", _sectio_quadragesima_quinta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "chorda_fissio_chorda", _sectio_quadragesima_sexta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "chorda_iungere", _sectio_quadragesima_septima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "chorda_ut_s64 / ex_s64", _sectio_quadragesima_octava, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "limites s32 / i32", _sectio_quadragesima_nona, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "chorda_ex_f64_exacta", _sectio_quinquagesima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("chorda", SECTIONES);
}

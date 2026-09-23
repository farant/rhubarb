/* probatio_json.c - Probationes pro bibliotheca JSON
 */

#include "json.h"
#include "credo.h"
#include "piscina.h"
#include <stdio.h>
#include <string.h>
#include <math.h>


/* ========================================================================
 * PROBATIONES - PRIMITIVA
 * ======================================================================== */

interior vacuum
probatio_nullum(Piscina* piscina)
{
    JsonResultus res;

    imprimere("--- Probans nullum ---\n");

    res = json_legere_literis("null", piscina);
    CREDO_VERUM(res.successus);
    CREDO_VERUM(res.radix != NIHIL);
    CREDO_VERUM(json_est_nullum(res.radix));
    CREDO_VERUM(json_genus(res.radix) == JSON_NULLUM);
}

interior vacuum
probatio_boolean(Piscina* piscina)
{
    JsonResultus res;

    imprimere("--- Probans boolean ---\n");

    res = json_legere_literis("true", piscina);
    CREDO_VERUM(res.successus);
    CREDO_VERUM(json_est_boolean(res.radix));
    CREDO_VERUM(json_ad_boolean(res.radix) == VERUM);

    res = json_legere_literis("false", piscina);
    CREDO_VERUM(res.successus);
    CREDO_VERUM(json_est_boolean(res.radix));
    CREDO_VERUM(json_ad_boolean(res.radix) == FALSUM);
}

interior vacuum
probatio_integer(Piscina* piscina)
{
    JsonResultus res;

    imprimere("--- Probans integer ---\n");

    res = json_legere_literis("42", piscina);
    CREDO_VERUM(res.successus);
    CREDO_VERUM(json_est_integer(res.radix));
    CREDO_VERUM(json_ad_integer(res.radix) == 42);

    res = json_legere_literis("-123", piscina);
    CREDO_VERUM(res.successus);
    CREDO_VERUM(json_ad_integer(res.radix) == -123);

    res = json_legere_literis("0", piscina);
    CREDO_VERUM(res.successus);
    CREDO_VERUM(json_ad_integer(res.radix) == 0);

    /* Large numbers */
    res = json_legere_literis("9223372036854775807", piscina);
    CREDO_VERUM(res.successus);
    CREDO_VERUM(json_est_integer(res.radix));
}

interior vacuum
probatio_fluitans(Piscina* piscina)
{
    JsonResultus res;
             f64 val;

    imprimere("--- Probans fluitans ---\n");

    res = json_legere_literis("3.14", piscina);
    CREDO_VERUM(res.successus);
    CREDO_VERUM(json_est_fluitans(res.radix));
    val = json_ad_fluitans(res.radix);
    CREDO_VERUM(fabs(val - 3.14) < 0.001);

    res = json_legere_literis("-0.5", piscina);
    CREDO_VERUM(res.successus);
    val = json_ad_fluitans(res.radix);
    CREDO_VERUM(fabs(val - (-0.5)) < 0.001);

    res = json_legere_literis("1e10", piscina);
    CREDO_VERUM(res.successus);
    CREDO_VERUM(json_est_fluitans(res.radix));

    res = json_legere_literis("1.5E-3", piscina);
    CREDO_VERUM(res.successus);
    val = json_ad_fluitans(res.radix);
    CREDO_VERUM(fabs(val - 0.0015) < 0.0001);
}

interior vacuum
probatio_chorda(Piscina* piscina)
{
    JsonResultus res;
          chorda val;

    imprimere("--- Probans chorda ---\n");

    res = json_legere_literis("\"hello\"", piscina);
    CREDO_VERUM(res.successus);
    CREDO_VERUM(json_est_chorda(res.radix));
    val = json_ad_chorda(res.radix);
    CREDO_VERUM(val.mensura == 5);
    CREDO_VERUM(memcmp(val.datum, "hello", 5) == 0);

    /* Empty string */
    res = json_legere_literis("\"\"", piscina);
    CREDO_VERUM(res.successus);
    val = json_ad_chorda(res.radix);
    CREDO_VERUM(val.mensura == 0);
}


/* ========================================================================
 * PROBATIONES - ESCAPE SEQUENCES
 * ======================================================================== */

interior vacuum
probatio_escapes(Piscina* piscina)
{
    JsonResultus res;
          chorda val;

    imprimere("--- Probans escapes ---\n");

    /* Quote escape */
    res = json_legere_literis("\"hello\\\"world\"", piscina);
    CREDO_VERUM(res.successus);
    val = json_ad_chorda(res.radix);
    CREDO_VERUM(val.mensura == 11);
    CREDO_VERUM(memcmp(val.datum, "hello\"world", 11) == 0);

    /* Backslash */
    res = json_legere_literis("\"a\\\\b\"", piscina);
    CREDO_VERUM(res.successus);
    val = json_ad_chorda(res.radix);
    CREDO_VERUM(val.mensura == 3);
    CREDO_VERUM(memcmp(val.datum, "a\\b", 3) == 0);

    /* Newline and tab */
    res = json_legere_literis("\"line1\\nline2\\ttab\"", piscina);
    CREDO_VERUM(res.successus);
    val = json_ad_chorda(res.radix);
    CREDO_VERUM(val.datum[5] == '\n');
    CREDO_VERUM(val.datum[11] == '\t');

    /* Unicode escape - simple ASCII */
    res = json_legere_literis("\"\\u0041\"", piscina);
    CREDO_VERUM(res.successus);
    val = json_ad_chorda(res.radix);
    CREDO_VERUM(val.mensura == 1);
    CREDO_VERUM(val.datum[0] == 'A');

    /* Unicode escape - multibyte */
    res = json_legere_literis("\"\\u00E9\"", piscina);  /* e-acute */
    CREDO_VERUM(res.successus);
    val = json_ad_chorda(res.radix);
    CREDO_VERUM(val.mensura == 2);  /* UTF-8 two bytes */
}


/* ========================================================================
 * PROBATIONES - TABULATUM (ARRAY)
 * ======================================================================== */

interior vacuum
probatio_tabulatum(Piscina* piscina)
{
    JsonResultus  res;
       JsonValor* elem;

    imprimere("--- Probans tabulatum ---\n");

    /* Empty array */
    res = json_legere_literis("[]", piscina);
    CREDO_VERUM(res.successus);
    CREDO_VERUM(json_est_tabulatum(res.radix));
    CREDO_VERUM(json_tabulatum_numerus(res.radix) == 0);

    /* Simple array */
    res = json_legere_literis("[1, 2, 3]", piscina);
    CREDO_VERUM(res.successus);
    CREDO_VERUM(json_tabulatum_numerus(res.radix) == 3);
    elem = json_tabulatum_obtinere(res.radix, 0);
    CREDO_VERUM(json_ad_integer(elem) == 1);
    elem = json_tabulatum_obtinere(res.radix, 2);
    CREDO_VERUM(json_ad_integer(elem) == 3);

    /* Mixed types */
    res = json_legere_literis("[1, \"two\", true, null]", piscina);
    CREDO_VERUM(res.successus);
    CREDO_VERUM(json_tabulatum_numerus(res.radix) == 4);
    CREDO_VERUM(json_est_integer(json_tabulatum_obtinere(res.radix,
        0)));
    CREDO_VERUM(json_est_chorda(json_tabulatum_obtinere(res.radix, 1)));
    CREDO_VERUM(json_est_boolean(json_tabulatum_obtinere(res.radix,
        2)));
    CREDO_VERUM(json_est_nullum(json_tabulatum_obtinere(res.radix, 3)));

    /* Nested arrays */
    res = json_legere_literis("[[1, 2], [3, 4]]", piscina);
    CREDO_VERUM(res.successus);
    CREDO_VERUM(json_tabulatum_numerus(res.radix) == 2);
    elem = json_tabulatum_obtinere(res.radix, 0);
    CREDO_VERUM(json_est_tabulatum(elem));
    CREDO_VERUM(json_tabulatum_numerus(elem) == 2);
}


/* ========================================================================
 * PROBATIONES - OBJECTUM
 * ======================================================================== */

interior vacuum
probatio_objectum(Piscina* piscina)
{
    JsonResultus  res;
       JsonValor* val;
          chorda  str_val;

    imprimere("--- Probans objectum ---\n");

    /* Empty object */
    res = json_legere_literis("{}", piscina);
    CREDO_VERUM(res.successus);
    CREDO_VERUM(json_est_objectum(res.radix));
    CREDO_VERUM(json_objectum_numerus(res.radix) == 0);

    /* Simple object */
    res = json_legere_literis("{\"name\": \"John\", \"age\": 30}",
        piscina);
    CREDO_VERUM(res.successus);
    CREDO_VERUM(json_objectum_numerus(res.radix) == 2);

    val = json_objectum_capere(res.radix, "name");
    CREDO_VERUM(val != NIHIL);
    CREDO_VERUM(json_est_chorda(val));
    str_val = json_ad_chorda(val);
    CREDO_VERUM(str_val.mensura == 4);
    CREDO_VERUM(memcmp(str_val.datum, "John", 4) == 0);

    val = json_objectum_capere(res.radix, "age");
    CREDO_VERUM(val != NIHIL);
    CREDO_VERUM(json_ad_integer(val) == 30);

    CREDO_VERUM(json_objectum_habet(res.radix, "name"));
    CREDO_VERUM(!json_objectum_habet(res.radix, "unknown"));

    /* Nested object */
    res = json_legere_literis("{\"person\": {\"name\": \"Alice\"}}",
        piscina);
    CREDO_VERUM(res.successus);
    val = json_objectum_capere(res.radix, "person");
    CREDO_VERUM(json_est_objectum(val));
    val = json_objectum_capere(val, "name");
    CREDO_VERUM(json_est_chorda(val));
}


/* ========================================================================
 * PROBATIONES - WHITESPACE
 * ======================================================================== */

interior vacuum
probatio_whitespace(Piscina* piscina)
{
    JsonResultus res;

    imprimere("--- Probans whitespace ---\n");

    res = json_legere_literis("  {  \"key\"  :  \"value\"  }  ",
        piscina);
    CREDO_VERUM(res.successus);
    CREDO_VERUM(json_objectum_habet(res.radix, "key"));

    res = json_legere_literis("[\n  1,\n  2,\n  3\n]", piscina);
    CREDO_VERUM(res.successus);
    CREDO_VERUM(json_tabulatum_numerus(res.radix) == 3);

    res = json_legere_literis("{\t\"a\"\t:\t1\t}", piscina);
    CREDO_VERUM(res.successus);
}


/* ========================================================================
 * PROBATIONES - ERRORES
 * ======================================================================== */

interior vacuum
probatio_errores(Piscina* piscina)
{
    JsonResultus res;

    imprimere("--- Probans errores ---\n");

    /* Unterminated string */
    res = json_legere_literis("\"hello", piscina);
    CREDO_VERUM(!res.successus);

    /* Invalid token */
    res = json_legere_literis("undefined", piscina);
    CREDO_VERUM(!res.successus);

    /* Missing colon */
    res = json_legere_literis("{\"key\" \"value\"}", piscina);
    CREDO_VERUM(!res.successus);

    /* Trailing comma (strict JSON) */
    res = json_legere_literis("[1, 2, 3,]", piscina);
    CREDO_VERUM(!res.successus);

    /* Missing value */
    res = json_legere_literis("{\"key\":}", piscina);
    CREDO_VERUM(!res.successus);
}


/* ========================================================================
 * PROBATIONES - BUILDER
 * ======================================================================== */

interior vacuum
probatio_builder(Piscina* piscina)
{
    JsonValor* obj;
    JsonValor* arr;
    JsonValor* nested;
       chorda  output;

    imprimere("--- Probans builder ---\n");

    /* Build simple object */
    obj = json_objectum_creare(piscina);
    CREDO_VERUM(obj != NIHIL);

    json_objectum_ponere(obj, "name",
        json_chorda_creare_literis(piscina, "Test"));
    json_objectum_ponere(obj, "value", json_integer_creare(piscina,
        42));
    json_objectum_ponere(obj, "active", json_boolean_creare(piscina,
        VERUM));

    CREDO_VERUM(json_objectum_numerus(obj) == 3);
    CREDO_VERUM(json_ad_integer(json_objectum_capere(obj, "value"))
        == 42);

    /* Build array */
    arr = json_tabulatum_creare(piscina);
    json_tabulatum_addere(arr, json_integer_creare(piscina, 1));
    json_tabulatum_addere(arr, json_integer_creare(piscina, 2));
    json_tabulatum_addere(arr, json_integer_creare(piscina, 3));

    CREDO_VERUM(json_tabulatum_numerus(arr) == 3);

    /* Nested structure */
    nested = json_objectum_creare(piscina);
    json_objectum_ponere(nested, "items", arr);
    json_objectum_ponere(nested, "count", json_integer_creare(piscina,
        3));

    output = json_scribere(nested, piscina);
    CREDO_VERUM(output.mensura > 0);
    imprimere("  Output: %.*s\n", output.mensura, output.datum);
}


/* ========================================================================
 * PROBATIONES - SERIALIZATION
 * ======================================================================== */

interior vacuum
probatio_serialization(Piscina* piscina)
{
    JsonValor* val;
       chorda  output;

    imprimere("--- Probans serialization ---\n");

    /* Null */
    val     = json_nullum_creare(piscina);
    output  = json_scribere(val, piscina);
    CREDO_VERUM(output.mensura == 4);
    CREDO_VERUM(memcmp(output.datum, "null", 4) == 0);

    /* Boolean */
    val     = json_boolean_creare(piscina, VERUM);
    output  = json_scribere(val, piscina);
    CREDO_VERUM(memcmp(output.datum, "true", 4) == 0);

    /* Integer */
    val     = json_integer_creare(piscina, -123);
    output  = json_scribere(val, piscina);
    CREDO_VERUM(memcmp(output.datum, "-123", 4) == 0);

    /* String with escapes */
    val     = json_chorda_creare_literis(piscina, "hello\nworld");
    output  = json_scribere(val, piscina);
    imprimere("  Escaped: %.*s\n", output.mensura, output.datum);
    /* Should contain \n escape */
}


/* ========================================================================
 * PROBATIONES - ROUND TRIP
 * ======================================================================== */

interior vacuum
probatio_round_trip(Piscina* piscina)
{
    constans character* original =
        "{\"name\":\"Test\",\"values\":[1,2,3],\"nested\":{\"a\":true}}";
          JsonResultus res1;
          JsonResultus res2;
                chorda serialized;

    imprimere("--- Probans round trip ---\n");

    /* Parse original */
    res1 = json_legere_literis(original, piscina);
    CREDO_VERUM(res1.successus);

    /* Serialize */
    serialized = json_scribere(res1.radix, piscina);
    imprimere("  Original:   %s\n", original);
    imprimere("  Serialized: %.*s\n", serialized.mensura,
        serialized.datum);

    /* Parse again */
    res2 = json_legere(serialized, piscina);
    CREDO_VERUM(res2.successus);

    /* Compare structure */
    CREDO_VERUM(json_objectum_numerus(res2.radix)
        == json_objectum_numerus(res1.radix));
    CREDO_VERUM(json_objectum_habet(res2.radix, "name"));
    CREDO_VERUM(json_objectum_habet(res2.radix, "values"));
    CREDO_VERUM(json_objectum_habet(res2.radix, "nested"));
}


/* ========================================================================
 * PROBATIONES - PRETTY PRINT
 * ======================================================================== */

interior vacuum
probatio_pretty_print(Piscina* piscina)
{
    JsonValor* obj;
    JsonValor* arr;
       chorda  output;

    imprimere("--- Probans pretty print ---\n");

    obj = json_objectum_creare(piscina);
    json_objectum_ponere(obj, "name",
        json_chorda_creare_literis(piscina, "Test"));

    arr = json_tabulatum_creare(piscina);
    json_tabulatum_addere(arr, json_integer_creare(piscina, 1));
    json_tabulatum_addere(arr, json_integer_creare(piscina, 2));
    json_objectum_ponere(obj, "items", arr);

    output = json_scribere_pulchrum(obj, piscina);
    imprimere("%.*s\n", output.mensura, output.datum);

    /* Should have newlines */
    {
        i32 i;
        b32 has_newline = FALSUM;
        per (i = 0; i < output.mensura; i++)
        {
            si (output.datum[i] == '\n')
            {
                has_newline = VERUM;
                frange;
            }
        }
        CREDO_VERUM(has_newline);
    }
}


/* ========================================================================
 * PROBATIONES - ITERATOR
 * ======================================================================== */

interior vacuum
probatio_iterator(Piscina* piscina)
{
            JsonResultus  res;
    JsonObjectumIterator  iter;
                  chorda  clavis;
               JsonValor* valor;
                     i32  count = 0;

    imprimere("--- Probans iterator ---\n");

    res = json_legere_literis("{\"a\": 1, \"b\": 2, \"c\": 3}",
        piscina);
    CREDO_VERUM(res.successus);

    iter = json_objectum_iterator(res.radix);

    dum (json_objectum_iterator_proxima(&iter, &clavis, &valor))
    {
        imprimere("  Key: %.*s = %lld\n",
            clavis.mensura, clavis.datum,
            (long long)json_ad_integer(valor));
        count++;
    }

    CREDO_VERUM(count == 3);
}


/* ========================================================================
 * PROBATIONES - CLAVIS VACUA (2026-09-22)
 *
 * {"":1} clavem NIHIL ponebat (internamentum "" recusabat): capere
 * casu NIHIL == NIHIL inveniebat, scribere et iterator ruebant.
 * ======================================================================== */

interior vacuum
probatio_clavis_vacua(Piscina* piscina)
{
            JsonResultus  res;
    JsonObjectumIterator  iter;
                  chorda  clavis;
                  chorda  vacua;
               JsonValor* valor;
               JsonValor* obj;
                  chorda  output;

    imprimere("--- Probans clavem vacuam ---\n");

    vacua.datum    = NIHIL;
    vacua.mensura  = 0;

    res = json_legere_literis("{\"\":1}", piscina);
    CREDO_VERUM(res.successus);
    CREDO_AEQUALIS_I32(json_objectum_numerus(res.radix), I);
    CREDO_NON_NIHIL(json_objectum_par_obtinere(res.radix, 0)->clavis);
    CREDO_AEQUALIS_S64(json_ad_integer(json_objectum_capere(res.radix,
        "")), 1);
    CREDO_AEQUALIS_S64(json_ad_integer(json_objectum_capere_chorda(
        res.radix, vacua)), 1);

    /* ruinae priores: scribere et iterator clavem NIHIL legebant */
    CREDO_NON_RUIT(json_scribere(res.radix, piscina));
    output = json_scribere(res.radix, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(output, "{\"\":1}");

    iter = json_objectum_iterator(res.radix);
    CREDO_VERUM(json_objectum_iterator_proxima(&iter, &clavis, &valor));
    CREDO_AEQUALIS_I32(clavis.mensura, 0);
    CREDO_AEQUALIS_S64(json_ad_integer(valor), 1);

    /* clavis vacua a "a" distinguitur, etiam imbricata */
    res = json_legere_literis("{\"\":1,\"a\":{\"\":[]}}", piscina);
    CREDO_VERUM(res.successus);
    CREDO_AEQUALIS_I32(json_objectum_numerus(res.radix), II);
    CREDO_AEQUALIS_S64(json_ad_integer(json_objectum_capere(res.radix,
        "")), 1);
    CREDO_VERUM(json_est_tabulatum(json_objectum_capere(
        json_objectum_capere(res.radix, "a"), "")));
    output = json_scribere(res.radix, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(output, "{\"\":1,\"a\":{\"\":[]}}");

    /* aedificator: ponere "" bis = una clavis, rescripta */
    obj = json_objectum_creare(piscina);
    json_objectum_ponere(obj, "", json_integer_creare(piscina, 1));
    json_objectum_ponere_chorda(obj, vacua, json_integer_creare(piscina,
        2));
    CREDO_AEQUALIS_I32(json_objectum_numerus(obj), I);
    output = json_scribere(obj, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(output, "{\"\":2}");
}


/* ========================================================================
 * PROBATIONES - PROFUNDITAS (2026-09-22)
 *
 * Recursio sine limite: II000000 '[' acervum exhauriebat (SIGSEGV) -
 * cliens quivis residentem MCP necare poterat.
 * ======================================================================== */

/* n aperientes, "0", n claudentes; objecta ut {"a": ... } */
interior chorda
_nidum_facere (
     Piscina* piscina,
         i32  n,
         b32  objecta)
{
    ChordaAedificator* aed;
                  i32  i;

    aed = chorda_aedificator_creare(piscina, n * VI + XVI);
    per (i = 0; i < n; i++)
    {
        chorda_aedificator_appendere_literis(aed,
            objecta ? "{\"a\":" : "[");
    }
    chorda_aedificator_appendere_character(aed, '0');
    per (i = 0; i < n; i++)
    {
        chorda_aedificator_appendere_character(aed,
            objecta ? '}' : ']');
    }
    redde chorda_aedificator_finire(aed);
}

interior vacuum
probatio_profunditas(Piscina* piscina)
{
    JsonResultus res;
          chorda textus;

    imprimere("--- Probans profunditatem ---\n");

    /* ad limitem ipsum: licet */
    res = json_legere(_nidum_facere(piscina, JSON_PROFUNDITAS_MAXIMA,
        FALSUM), piscina);
    CREDO_VERUM(res.successus);
    res = json_legere(_nidum_facere(piscina, JSON_PROFUNDITAS_MAXIMA,
        VERUM), piscina);
    CREDO_VERUM(res.successus);

    /* uno ultra: recusatur ad aperientem excedentem */
    res = json_legere(_nidum_facere(piscina, JSON_PROFUNDITAS_MAXIMA
        + I,
        FALSUM), piscina);
    CREDO_FALSUM(res.successus);
    CREDO_CHORDA_CONTINET(res.error, chorda_ex_literis("profunda",
        piscina));
    CREDO_AEQUALIS_I32(res.linea, I);
    CREDO_AEQUALIS_I32(res.columna, JSON_PROFUNDITAS_MAXIMA + I);

    res = json_legere(_nidum_facere(piscina, JSON_PROFUNDITAS_MAXIMA
        + I,
        VERUM), piscina);
    CREDO_FALSUM(res.successus);
    CREDO_CHORDA_CONTINET(res.error, chorda_ex_literis("profunda",
        piscina));

    /* impetus: II000000 '[' sine claudentibus - non ruit */
    {
        i8* data;
        data = (i8*)piscina_allocare(piscina, 2000000);
        memset(data, '[', 2000000);
        textus.datum    = data;
        textus.mensura  = 2000000;
    }
    CREDO_NON_RUIT(json_legere(textus, piscina));
    res = json_legere(textus, piscina);
    CREDO_FALSUM(res.successus);
    CREDO_CHORDA_CONTINET(res.error, chorda_ex_literis("profunda",
        piscina));
}


/* ========================================================================
 * PROBATIONES - CAUDA (2026-09-22)
 *
 * json_legere FINEM post valorem radicis non probabat: '{"a":1}
 * garbage', '1 2', '{"a":1}}' successus erant, et '01' ut 0 legebatur.
 * gesta_annales_verificare lineam quamque sic legit - duo eventus in
 * linea una conglutinati ut unus transibant.
 * ======================================================================== */

/* textum refutatum esse et locum caudae nominari */
interior vacuum
_cauda_refutata (
               Piscina* piscina,
    constans character* textus,
                    i32  linea,
                    i32  columna)
{
    JsonResultus res;

    /* nomen casus in effusu - FRACTA infra lineas adiutoris solum
     * nominant */
    imprimere("  cauda refutanda: %s\n", textus);
    res = json_legere_literis(textus, piscina);
    CREDO_FALSUM(res.successus);
    CREDO_NIHIL(res.radix);
    CREDO_CHORDA_CONTINET(res.error, chorda_ex_literis("post valorem",
        piscina));
    CREDO_AEQUALIS_I32(res.linea, linea);
    CREDO_AEQUALIS_I32(res.columna, columna);
}

interior vacuum
probatio_cauda(Piscina* piscina)
{
    JsonResultus res;
          chorda cum_nullo;

    imprimere("--- Probans caudam ---\n");

    _cauda_refutata(piscina, "{\"a\":1} garbage", I, IX);
    _cauda_refutata(piscina, "1 2", I, III);
    _cauda_refutata(piscina, "{\"a\":1}}", I, VIII);
    _cauda_refutata(piscina, "[1]]", I, IV);
    _cauda_refutata(piscina, "\"a\" \"b\"", I, V);
    _cauda_refutata(piscina, "null null", I, VI);
    /* duo eventus annalium sine '\n' inter eos */
    _cauda_refutata(piscina, "{\"seq\":1}{\"seq\":2}", I, X);
    /* cifra praefixa: '0' valor est, '1' cauda */
    _cauda_refutata(piscina, "01", I, II);
    _cauda_refutata(piscina, "-01", I, III);
    _cauda_refutata(piscina, "00", I, II);
    /* verbum clavis cum cauda litterarum */
    _cauda_refutata(piscina, "truex", I, V);
    /* error lexematis post valorem (olim tacitus) */
    _cauda_refutata(piscina, "[1] @", I, V);
    /* locus per lineas */
    _cauda_refutata(piscina, "{}\n\n  x", III, III);

    /* octetus NUL post valorem: non spatium, refutatur */
    cum_nullo            = chorda_ex_literis("{}?", piscina);
    cum_nullo.datum[II]  = (i8)'\0';
    res                  = json_legere(cum_nullo, piscina);
    CREDO_FALSUM(res.successus);

    /* spatium album (quattuor genera) ante et post: licet */
    res = json_legere_literis(" \t\r\n{\"a\":1} \t\r\n", piscina);
    CREDO_VERUM(res.successus);
    CREDO_AEQUALIS_S64(json_ad_integer(json_objectum_capere(res.radix,
        "a")), 1);
    res = json_legere_literis("0", piscina);
    CREDO_VERUM(res.successus);
    res = json_legere_literis("-0.5e1\n", piscina);
    CREDO_VERUM(res.successus);
}


/* ========================================================================
 * PRINCIPALE
 * ======================================================================== */

int
main (void)
{
    Piscina* piscina;

    imprimere("\n========================================\n");
    imprimere("PROBATIONES JSON\n");
    imprimere("========================================\n\n");

    piscina = piscina_generare_dynamicum("probatio_json", CDLVI);
    credo_aperire(piscina);

    probatio_nullum(piscina);
    probatio_boolean(piscina);
    probatio_integer(piscina);
    probatio_fluitans(piscina);
    probatio_chorda(piscina);
    probatio_escapes(piscina);
    probatio_tabulatum(piscina);
    probatio_objectum(piscina);
    probatio_whitespace(piscina);
    probatio_errores(piscina);
    probatio_builder(piscina);
    probatio_serialization(piscina);
    probatio_round_trip(piscina);
    probatio_pretty_print(piscina);
    probatio_iterator(piscina);
    probatio_clavis_vacua(piscina);
    probatio_profunditas(piscina);
    probatio_cauda(piscina);

    credo_imprimere_compendium();

    {
        b32 omnia = credo_omnia_praeterierunt();
        piscina_destruere(piscina);
        imprimere("========================================\n\n");
        return omnia ? 0 : 1;
    }
}

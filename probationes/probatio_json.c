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
    JsonResultus res;
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
    CREDO_VERUM(json_est_integer(json_tabulatum_obtinere(res.radix, 0)));
    CREDO_VERUM(json_est_chorda(json_tabulatum_obtinere(res.radix, 1)));
    CREDO_VERUM(json_est_boolean(json_tabulatum_obtinere(res.radix, 2)));
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
    JsonResultus res;
    JsonValor* val;
    chorda str_val;

    imprimere("--- Probans objectum ---\n");

    /* Empty object */
    res = json_legere_literis("{}", piscina);
    CREDO_VERUM(res.successus);
    CREDO_VERUM(json_est_objectum(res.radix));
    CREDO_VERUM(json_objectum_numerus(res.radix) == 0);

    /* Simple object */
    res = json_legere_literis("{\"name\": \"John\", \"age\": 30}", piscina);
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
    res = json_legere_literis("{\"person\": {\"name\": \"Alice\"}}", piscina);
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

    res = json_legere_literis("  {  \"key\"  :  \"value\"  }  ", piscina);
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
    chorda output;

    imprimere("--- Probans builder ---\n");

    /* Build simple object */
    obj = json_objectum_creare(piscina);
    CREDO_VERUM(obj != NIHIL);

    json_objectum_ponere(obj, "name", json_chorda_creare_literis(piscina, "Test"));
    json_objectum_ponere(obj, "value", json_integer_creare(piscina, 42));
    json_objectum_ponere(obj, "active", json_boolean_creare(piscina, VERUM));

    CREDO_VERUM(json_objectum_numerus(obj) == 3);
    CREDO_VERUM(json_ad_integer(json_objectum_capere(obj, "value")) == 42);

    /* Build array */
    arr = json_tabulatum_creare(piscina);
    json_tabulatum_addere(arr, json_integer_creare(piscina, 1));
    json_tabulatum_addere(arr, json_integer_creare(piscina, 2));
    json_tabulatum_addere(arr, json_integer_creare(piscina, 3));

    CREDO_VERUM(json_tabulatum_numerus(arr) == 3);

    /* Nested structure */
    nested = json_objectum_creare(piscina);
    json_objectum_ponere(nested, "items", arr);
    json_objectum_ponere(nested, "count", json_integer_creare(piscina, 3));

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
    chorda output;

    imprimere("--- Probans serialization ---\n");

    /* Null */
    val = json_nullum_creare(piscina);
    output = json_scribere(val, piscina);
    CREDO_VERUM(output.mensura == 4);
    CREDO_VERUM(memcmp(output.datum, "null", 4) == 0);

    /* Boolean */
    val = json_boolean_creare(piscina, VERUM);
    output = json_scribere(val, piscina);
    CREDO_VERUM(memcmp(output.datum, "true", 4) == 0);

    /* Integer */
    val = json_integer_creare(piscina, -123);
    output = json_scribere(val, piscina);
    CREDO_VERUM(memcmp(output.datum, "-123", 4) == 0);

    /* String with escapes */
    val = json_chorda_creare_literis(piscina, "hello\nworld");
    output = json_scribere(val, piscina);
    imprimere("  Escaped: %.*s\n", output.mensura, output.datum);
    /* Should contain \n escape */
}


/* ========================================================================
 * PROBATIONES - ROUND TRIP
 * ======================================================================== */

interior vacuum
probatio_round_trip(Piscina* piscina)
{
    constans character* original = "{\"name\":\"Test\",\"values\":[1,2,3],\"nested\":{\"a\":true}}";
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
    imprimere("  Serialized: %.*s\n", serialized.mensura, serialized.datum);

    /* Parse again */
    res2 = json_legere(serialized, piscina);
    CREDO_VERUM(res2.successus);

    /* Compare structure */
    CREDO_VERUM(json_objectum_numerus(res2.radix) == json_objectum_numerus(res1.radix));
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
    chorda output;

    imprimere("--- Probans pretty print ---\n");

    obj = json_objectum_creare(piscina);
    json_objectum_ponere(obj, "name", json_chorda_creare_literis(piscina, "Test"));

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
    JsonResultus res;
    JsonObjectumIterator iter;
    chorda clavis;
    JsonValor* valor;
    i32 count = 0;

    imprimere("--- Probans iterator ---\n");

    res = json_legere_literis("{\"a\": 1, \"b\": 2, \"c\": 3}", piscina);
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
 * PRINCIPALE
 * ======================================================================== */

int
main(void)
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

    credo_imprimere_compendium();

    {
        b32 omnia = credo_omnia_praeterierunt();
        piscina_destruere(piscina);
        imprimere("========================================\n\n");
        return omnia ? 0 : 1;
    }
}

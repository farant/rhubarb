/* sonda: lexer bounds - each input in an EXACT-size malloc buffer (no NUL) */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "json.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Piscina* pisc;

static void casus_(const char* titulus, const char* bytes, size_t n)
{
    chorda in;
    JsonResultus r;
    i8* buf = NULL;
    if (n > 0) { buf = (i8*)malloc(n); memcpy(buf, bytes, n); }
    else { buf = (i8*)malloc(1); }
    in.datum = buf; in.mensura = (i32)n;
    r = json_legere(in, pisc);
    printf("%-22s succ=%d radix=%s L%u C%u err=[%.*s]\n", titulus, (int)r.successus,
        r.radix ? "ptr" : "NIHIL", (unsigned)r.linea, (unsigned)r.columna,
        (int)r.error.mensura, r.error.datum ? (const char*)r.error.datum : "");
    free(buf);
}
#define K(t, s) casus_(t, s, sizeof(s) - 1)

int main(void)
{
    pisc = piscina_generare_dynamicum("sonda", 65536);
    casus_("vacuum", "", 0);
    K("spatia", "  \t\r\n ");
    K("tru", "tru");
    K("nul", "nul");
    K("fals", "fals");
    K("True", "True");
    K("truex", "truex");
    K("true", "true");
    K("false", "false");
    K("null", "null");
    K("1", "1");
    K("-", "-");
    K("-0", "-0");
    K("1.", "1.");
    K(".5", ".5");
    K("1e", "1e");
    K("1e+", "1e+");
    K("+1", "+1");
    K("0x1", "0x1");
    K("NaN", "NaN");
    K("Infinity", "Infinity");
    K("-Infinity", "-Infinity");
    K("str_open", "\"abc");
    K("str_esc_end", "\"abc\\");
    K("u12_end", "\"\\u12");
    K("uD800_end", "\"\\uD800");
    K("uD800_bs_end", "\"\\uD800\\");
    K("uD800_bsu_end", "\"\\uD800\\u");
    K("uD800_bsuDC_end", "\"\\uD800\\uDC");
    K("uD800A", "\"\\uD800A\"");
    K("trunc_utf8_end", "\"\xC3");
    K("trunc3_end", "\"\xE2\x82");
    K("arr_open", "[1,2");
    K("obj_open", "{\"a\"");
    K("obj_colon_end", "{\"a\":");
    K("obj_comma_end", "{\"a\":1,");
    K("arr_comma_end", "[1,");
    K("BOM", "\xEF\xBB\xBF{}");
    K("1 2", "1 2");
    K("{}}", "{}}");
    K("01", "01");
    casus_("1NUL", "1\0", 2);
    casus_("NUL1", "\0" "1", 2);
    K("[1,]", "[1,]");
    K("{a,}", "{\"a\":1,}");
    K("nocolon", "{\"a\" 1}");
    K("nocomma", "[1 2]");
    K("nocomma_obj", "{\"a\":1 \"b\":2}");
    K("comment", "[1/*x*/]");
    K("linecomment", "// x\n1");
    K("squote", "'a'");
    K("barekey", "{a:1}");
    K("emptycomma", "[,]");
    K("objcomma", "{,}");
    K("u_lower", "\"\\u00e9\"");
    K("esc_x", "\"\\x41\"");
    K("esc_U", "\"\\U0041\"");
    return 0;
}

/* sonda: error line/column base and counting */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "json.h"
#include <stdio.h>
#include <string.h>

static Piscina* pisc;
static void p_(const char* t, const char* s)
{
    JsonResultus r = json_legere_literis(s, pisc);
    printf("%-22s succ=%d L%u C%u [%.*s]\n", t, (int)r.successus, (unsigned)r.linea, (unsigned)r.columna,
        (int)r.error.mensura, r.error.datum ? (const char*)r.error.datum : "");
}
int main(void)
{
    char deep[600]; int i;
    pisc = piscina_generare_dynamicum("sonda", 65536);
    p_("first_char", "x");
    p_("crlf", "[1,\r\n x]");
    p_("lf", "[1,\n x]");
    p_("lone_cr", "[1,\r x]");
    p_("multibyte_line", "[\"\xC3\xA9\xC3\xA9\", x]");
    p_("escape_counts", "[\"\\u00e9\", x]");
    p_("err_in_key", "{\"ab\\q\":1}");
    p_("dup_key_line2", "{\"a\":1,\n  \"a\":2}");
    p_("f64_in_obj", "{\"a\":\n 1e999}");
    p_("s64_in_arr", "[1,\n  -9223372036854775809]");
    p_("utf8_after_mb", "\"\xC3\xA9\xFF\"");
    for (i = 0; i < 257; i++) deep[i] = (i % 2) ? '{' : '[';
    deep[257] = 0;
    p_("mixed_257_bare", deep);
    /* valid mixed nest of 256: [{"a":[{"a":... */
    {
        char buf[4000]; size_t o = 0; int d;
        for (d = 0; d < 256; d++) o += (size_t)sprintf(buf + o, "%s", (d % 2) ? "{\"a\":" : "[");
        o += (size_t)sprintf(buf + o, "0");
        for (d = 255; d >= 0; d--) buf[o++] = (d % 2) ? '}' : ']';
        buf[o] = 0;
        p_("mixed_256_valid", buf);
        o = 0;
        for (d = 0; d < 257; d++) o += (size_t)sprintf(buf + o, "%s", (d % 2) ? "{\"a\":" : "[");
        o += (size_t)sprintf(buf + o, "0");
        for (d = 256; d >= 0; d--) buf[o++] = (d % 2) ? '}' : ']';
        buf[o] = 0;
        p_("mixed_257", buf);
    }
    return 0;
}

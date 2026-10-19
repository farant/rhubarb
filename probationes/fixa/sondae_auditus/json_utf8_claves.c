/* sonda: raw bytes inside KEYS, exact-size buffers */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "json.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Piscina* pisc;
static void c_(const char* t, const char* b, size_t n)
{
    i8* buf = (i8*)malloc(n); chorda in; JsonResultus r;
    memcpy(buf, b, n); in.datum = buf; in.mensura = (i32)n;
    r = json_legere(in, pisc);
    printf("%-16s succ=%d L%u C%u [%.*s]\n", t, (int)r.successus, (unsigned)r.linea, (unsigned)r.columna,
        (int)r.error.mensura, r.error.datum ? (const char*)r.error.datum : "");
    free(buf);
}
#define K(t, s) c_(t, s, sizeof(s) - 1)
int main(void)
{
    pisc = piscina_generare_dynamicum("sonda", 65536);
    K("key_tab", "{\"a\tb\":1}");
    c_("key_nul", "{\"a\0b\":1}", 9);
    K("key_del", "{\"a\x7f\":1}");
    K("key_overlong", "{\"\xC0\xAF\":1}");
    K("key_surr", "{\"\xED\xA0\x80\":1}");
    K("key_gt10ffff", "{\"\xF4\x90\x80\x80\":1}");
    K("key_orphan", "{\"\x80\":1}");
    K("key_f8", "{\"\xF8\x88\x80\x80\x80\":1}");
    K("key_trunc_end", "{\"\xE2\x82");
    K("key_ok_emoji", "{\"\xF0\x9F\x98\x80\":1}");
    K("key_uD800", "{\"\\uD800\":1}");
    K("key_uDC00", "{\"\\uDC00\":1}");
    K("key_pair", "{\"\\uD83D\\uDE00\":1,\"\xF0\x9F\x98\x80\":2}");
    K("val_overlong3", "\"\xE0\x80\xAF\"");
    K("val_overlong4", "\"\xF0\x80\x80\xAF\"");
    K("val_10ffff", "\"\xF4\x8F\xBF\xBF\"");
    K("val_fffe", "\"\xEF\xBF\xBE\"");
    return 0;
}

/* sonda: json_via index wrap and segment forms */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "json.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Piscina* pisc;
static JsonValor* radix;

static void via_(const char* v)
{
    JsonValor* x = json_via(radix, v);
    chorda w;
    if (!x) { printf("VIA %-26s -> NIHIL\n", v ? v : "(NIHIL)"); return; }
    w = json_scribere(x, pisc);
    printf("VIA %-26s -> %.*s\n", v, (int)w.mensura, (const char*)w.datum);
}

int main(void)
{
    JsonResultus r;
    char* longa; int i;
    pisc = piscina_generare_dynamicum("sonda", 65536);
    r = json_legere_literis("{\"a\":[\"zero\"],\"b\":[\"b0\",\"b1\"],\"c\":[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21],"
        "\"o\":{\"0\":\"clavis0\",\"01\":\"clavis01\",\"\":\"vacua\",\"x.y\":\"punctum\"},\"s\":5,\"n\":null,\"e\":[]}", pisc);
    printf("parse succ=%d\n", (int)r.successus);
    radix = r.radix;
    via_("a.0"); via_("a.1");
    via_("a.4294967296"); via_("a.8589934592"); via_("b.4294967297"); via_("a.42949672960");
    via_("a.18446744073709551616"); via_("c.4294967306"); via_("c.21"); via_("c.22");
    via_("e.0"); via_("e.4294967296");
    via_("b.00"); via_("b.01"); via_("b.001"); via_("b.-1"); via_("b.+1"); via_("b. 1"); via_("b.1a"); via_("b.0x1");
    via_("o.0"); via_("o.01"); via_("o."); via_("o.x.y"); via_("s.x"); via_("n.x"); via_("s");
    via_(""); via_("."); via_(".a"); via_("a."); via_("a..0");
    via_(NULL);
    /* very long path: 100000 segments */
    longa = (char*)malloc(300000);
    longa[0] = 0;
    for (i = 0; i < 100000; i++) strcat(longa + (i ? (i*2-1) : 0), i ? ".a" : "a");
    {
        JsonValor* x = json_via(radix, longa);
        printf("VIA long(100000 seg) -> %s\n", x ? "ptr" : "NIHIL");
    }
    free(longa);
    return 0;
}

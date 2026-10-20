/* sonda: nidificatio profunda - lector (argv[1]=N), scriptor/aequalis (argv[2]=1) */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "plist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 1 << 20);
    InternamentumChorda* intern = internamentum_creare(p);
    unsigned long n = argc > 1 ? strtoul(argv[1], NULL, 10) : 1000;
    int modus = argc > 2 ? atoi(argv[2]) : 0;
    if (modus == 0) {
        const char* h = "<plist version=\"1.0\">";
        unsigned long len = strlen(h) + n * 15 + 16, k, pos = 0;
        char* buf = (char*)malloc(len);
        chorda c; PlistResultus r;
        memcpy(buf, h, strlen(h)); pos = strlen(h);
        for (k = 0; k < n; k++) { memcpy(buf + pos, "<array>", 7); pos += 7; }
        for (k = 0; k < n; k++) { memcpy(buf + pos, "</array>", 8); pos += 8; }
        memcpy(buf + pos, "</plist>", 8); pos += 8;
        c.datum = (i8*)buf; c.mensura = (i32)pos;
        r = plist_legere(c, p, intern);
        printf("legere n=%lu status=%d radix=%s\n", n, (int)r.status, r.radix ? "non-NIHIL" : "NIHIL");
    } else {
        /* aedificare n listas nidificatas, deinde scribere et conferre */
        PlistValor* radix = plist_lista_creare(p);
        PlistValor* cur = radix; PlistValor* b = plist_lista_creare(p); PlistValor* curb = b;
        unsigned long k; PlistStatus st; chorda o;
        for (k = 0; k < n; k++) {
            PlistValor* x = plist_lista_creare(p); PlistValor* y = plist_lista_creare(p);
            plist_lista_addere(cur, x, p); cur = x;
            plist_lista_addere(curb, y, p); curb = y;
        }
        printf("aequalis n=%lu -> ", n); fflush(stdout);
        printf("%d\n", (int)plist_aequalis(radix, b)); fflush(stdout);
        printf("scribere n=%lu -> ", n); fflush(stdout);
        o = plist_scribere(radix, &st, p);
        printf("status=%d mensura=%u\n", (int)st, (unsigned)o.mensura);
    }
    return 0;
}

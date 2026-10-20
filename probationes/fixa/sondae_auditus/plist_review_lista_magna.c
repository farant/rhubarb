/* sonda: dictio magna - tempus lectionis (ponere linearis per clavem) */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "plist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char** argv)
{
    unsigned long n = argc > 1 ? strtoul(argv[1], NULL, 10) : 1000, k, pos = 0;
    Piscina* p = piscina_generare_dynamicum("sonda", 1 << 20);
    InternamentumChorda* intern = internamentum_creare(p);
    char* buf = (char*)malloc(n * 48 + 64);
    chorda c; PlistResultus r; clock_t t0;
    pos += (unsigned long)sprintf(buf + pos, "<plist version=\"1.0\"><array>");
    for (k = 0; k < n; k++) pos += (unsigned long)sprintf(buf + pos, "<string>k%08lu</string><true/>", k);
    pos += (unsigned long)sprintf(buf + pos, "</array></plist>");
    c.datum = (i8*)buf; c.mensura = (i32)pos;
    t0 = clock();
    r = plist_legere(c, p, intern);
    printf("n=%lu bytes=%lu status=%d numerus=%u secundae=%.2f\n", n, pos, (int)r.status,
        r.radix ? (unsigned)plist_numerus(r.radix) : 0u, (double)(clock() - t0) / CLOCKS_PER_SEC);
    return 0;
}

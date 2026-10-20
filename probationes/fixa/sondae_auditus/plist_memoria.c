/* sonda: piscina certae magnitudinis exhausta - argv[1] modus (0 legere, 1 scribere), argv[2] octeti */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "plist.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    int modus = argc > 1 ? atoi(argv[1]) : 0;
    unsigned long n = argc > 2 ? strtoul(argv[2], NULL, 10) : 4096;
    Piscina* magna = piscina_generare_dynamicum("magna", 65536);
    Piscina* parva = piscina_generare_certae_magnitudinis("parva", (memoriae_index)n);
    InternamentumChorda* intern = internamentum_creare(magna);
    const char* t = "<plist version=\"1.0\"><dict><key>alpha</key><string>aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa</string>"
        "<key>beta</key><array><integer>1</integer><integer>2</integer><real>0.5</real></array></dict></plist>";
    chorda c = chorda_ex_literis(t, magna);
    if (modus == 0) {
        PlistResultus r;
        printf("legere parva=%lu ...\n", n); fflush(stdout);
        r = plist_legere(c, parva, intern);
        printf("status=%d radix=%s\n", (int)r.status, r.radix ? "non-NIHIL" : "NIHIL");
    } else {
        PlistResultus r = plist_legere(c, magna, intern);
        PlistStatus st; chorda o;
        printf("scribere parva=%lu ...\n", n); fflush(stdout);
        o = plist_scribere(r.radix, &st, parva);
        printf("status=%d mensura=%u\n", (int)st, (unsigned)o.mensura);
    }
    return 0;
}

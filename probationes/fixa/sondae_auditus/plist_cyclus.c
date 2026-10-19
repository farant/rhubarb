/* sonda: graphum cyclicum - argv[1]: 0 scribere(d in d), 1 aequalis(d,d), 2 aequalis(l,l) lista in se */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "plist.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    int modus = argc > 1 ? atoi(argv[1]) : 0;
    PlistValor* d = plist_dictio_creare(p);
    PlistValor* l = plist_lista_creare(p);
    PlistStatus st;
    printf("ponere(d,d)=%d addere(l,l)=%d\n",
        (int)plist_dictio_ponere(d, chorda_ex_literis("ego", p), d, p),
        (int)plist_lista_addere(l, l, p));
    fflush(stdout);
    if (modus == 0) { chorda o = plist_scribere(d, &st, p); printf("scribere status=%d mensura=%u\n", (int)st, (unsigned)o.mensura); }
    if (modus == 1) printf("aequalis(d,d)=%d\n", (int)plist_aequalis(d, d));
    if (modus == 2) printf("aequalis(l,l)=%d\n", (int)plist_aequalis(l, l));
    return 0;
}

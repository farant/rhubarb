/* sonda: plagulae - scribere_plagulam / legere_plagulam */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "plist.h"
#include <stdio.h>

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    InternamentumChorda* intern = internamentum_creare(p);
    PlistValor* d = plist_dictio_creare(p);
    PlistStatus st;
    b32 ok;
    PlistResultus r;
    FILE* f;
    plist_dictio_ponere(d, chorda_ex_literis("k", p), plist_integrum_creare(1, p), p);

    st = (PlistStatus)99; ok = plist_scribere_plagulam(d, "build/sonda/plist_rt/non_est_dir/x.plist", &st, p);
    printf("scribere_plagulam(dir absens): redde=%d status=%d\n", (int)ok, (int)st);
    st = (PlistStatus)99; ok = plist_scribere_plagulam(d, "build/sonda/plist_rt", &st, p);
    printf("scribere_plagulam(directorium): redde=%d status=%d\n", (int)ok, (int)st);
    st = (PlistStatus)99; ok = plist_scribere_plagulam(d, NIHIL, &st, p);
    printf("scribere_plagulam(NIHIL): redde=%d status=%d\n", (int)ok, (int)st);
    st = (PlistStatus)99; ok = plist_scribere_plagulam(d, "build/sonda/plist_rt/ok.plist", &st, p);
    printf("scribere_plagulam(ok): redde=%d status=%d\n", (int)ok, (int)st);

    r = plist_legere_plagulam("build/sonda/plist_rt/ok.plist", p, intern);
    printf("legere_plagulam(ok): status=%d aequalis=%d\n", (int)r.status, r.radix ? (int)plist_aequalis(d, r.radix) : -1);
    r = plist_legere_plagulam("build/sonda/plist_rt/absens.plist", p, intern);
    printf("legere_plagulam(absens): status=%d causa=%.*s\n", (int)r.status, (int)r.causa.mensura, (const char*)r.causa.datum);
    r = plist_legere_plagulam("build/sonda/plist_rt", p, intern);
    printf("legere_plagulam(directorium): status=%d causa=%.*s\n", (int)r.status, (int)r.causa.mensura, (const char*)r.causa.datum);
    f = fopen("build/sonda/plist_rt/vacua.plist", "wb"); if (f) fclose(f);
    r = plist_legere_plagulam("build/sonda/plist_rt/vacua.plist", p, intern);
    printf("legere_plagulam(vacua): status=%d causa=%.*s\n", (int)r.status, (int)r.causa.mensura, (const char*)r.causa.datum);
    r = plist_legere_plagulam(NIHIL, p, intern);
    printf("legere_plagulam(NIHIL): status=%d\n", (int)r.status);
    r = plist_legere(chorda_ex_literis("<plist version=\"1.0\"><string>a</string></plist>", p), p, NIHIL);
    printf("legere(intern NIHIL): status=%d\n", (int)r.status);
    return 0;
}

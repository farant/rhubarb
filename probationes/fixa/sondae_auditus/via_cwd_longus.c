#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "via.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    char orig[4096]; char seg[201]; int depth; chorda c; char* big; size_t cap = 16384;
    if (!getcwd(orig, sizeof orig)) return 3;
    memset(seg, 'd', 200); seg[200] = 0;
    if (chdir("build/sonda/via_cwd_longus") != 0) return 4;
    for (depth = 0; depth < 25; depth++) {
        mkdir(seg, 0755);
        if (chdir(seg) != 0) { perror("chdir"); return 5; }
    }
    big = (char*)malloc(cap);
    errno = 0;
    printf("getcwd(16384)=%s len=%lu\n", getcwd(big, cap) ? "ok" : "fail", getcwd(big, cap) ? (unsigned long)strlen(big) : 0UL);
    printf("getcwd(4096) errno=%d (ERANGE=%d)\n", getcwd(orig + 0, 0) ? 0 : 0, ERANGE);
    {
        char small[4096]; char* g = getcwd(small, sizeof small);
        printf("getcwd(4096 buf): %s errno=%d\n", g ? "ok" : "NULL", g ? 0 : errno);
    }
    c = via_directorium_currens(p);
    printf("directorium_currens mensura=%u datum=%p\n", (unsigned)c.mensura, (void*)c.datum);
    c = via_absoluta(chorda_ex_literis("sub/../file", p), p);
    printf("absoluta('sub/../file') -> mensura=%u abs=%d text='%.*s'\n", (unsigned)c.mensura,
           (int)via_est_absoluta(c), (int)(c.mensura > 40 ? 40 : c.mensura), (const char*)c.datum);
    /* cleanup */
    for (depth = 0; depth < 25; depth++) { chdir(".."); rmdir(seg); }
    free(big);
    piscina_destruere(p);
    return 0;
}

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "via.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

static void scr(const char* t, chorda c)
{
    unsigned int k;
    printf("%s -> ", t);
    if (!c.datum) { printf("<NIHIL m=%u>", (unsigned)c.mensura); }
    else { printf("'"); for (k = 0; k < c.mensura; k++) putchar(c.datum[k]); printf("'"); }
    printf(" abs=%d\n", (int)via_est_absoluta(c));
}
int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    char orig[4096];
    if (!getcwd(orig, sizeof orig)) return 3;
    scr("absoluta('')", via_absoluta(chorda_ex_literis("", p), p));
    scr("absoluta('../../../../../../../../../../..')", via_absoluta(chorda_ex_literis("../../../../../../../../../../..", p), p));
    scr("absoluta('/a/./b/..')", via_absoluta(chorda_ex_literis("/a/./b/..", p), p));
    if (chdir("/") != 0) return 4;
    scr("[cwd=/] absoluta('foo')", via_absoluta(chorda_ex_literis("foo", p), p));
    scr("[cwd=/] absoluta('')", via_absoluta(chorda_ex_literis("", p), p));
    if (chdir(orig) != 0) return 5;
    /* cwd deleted */
    rmdir("build/sonda/via_absoluta_cwd/morituri");
    if (mkdir("build/sonda/via_absoluta_cwd/morituri", 0755) != 0) { perror("mkdir"); return 6; }
    if (chdir("build/sonda/via_absoluta_cwd/morituri") != 0) return 7;
    if (rmdir(orig[0] ? "../morituri" : "") != 0) { perror("rmdir"); }
    {
        char tmp[4096]; char* g = getcwd(tmp, sizeof tmp);
        printf("[cwd deleted] getcwd=%s errno=%d\n", g ? g : "(NULL)", g ? 0 : errno);
    }
    scr("[cwd deleted] directorium_currens()", via_directorium_currens(p));
    scr("[cwd deleted] absoluta('foo/../bar')", via_absoluta(chorda_ex_literis("foo/../bar", p), p));
    scr("[cwd deleted] absoluta('x')", via_absoluta(chorda_ex_literis("x", p), p));
    chdir(orig);
    piscina_destruere(p);
    return 0;
}

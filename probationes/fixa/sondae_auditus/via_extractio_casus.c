#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "via.h"
#include <stdio.h>

static void scr(chorda c)
{
    unsigned int k;
    if (!c.datum) { printf("<NIHIL>"); return; }
    for (k = 0; k < c.mensura; k++) putchar(c.datum[k]);
}
int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    const char* tab_in[] = {
        "", "a", "/", ".", "..", "///", "/foo/bar/", "foo/", "/foo/", "foo//bar", "//foo", "/a//",
        "a/..", "a/.", "foo//", "a/", "foo", "dir/foo.txt", "a.b/c", ".bashrc", ".bashrc.bak",
        "..foo", "...", "foo.", "/a/b.c/", "archive.tar.gz", "/x.d/", "..txt", "a/.b.c", "x/..", NULL };
    int k;
    for (k = 0; tab_in[k]; k++) {
        chorda in = chorda_ex_literis(tab_in[k], p);
        printf("%s\t", tab_in[k]);
        scr(via_directorium(in, p)); printf("\t");
        scr(via_nomen(in, p)); printf("\t");
        scr(via_nomen_radix(in, p)); printf("\t");
        scr(via_extensio(in, p)); printf("\n");
    }
    /* est_absoluta / est_relativa */
    {
        const char* ab[] = { "", "/", "//", "C:\\x", "\\\\srv\\share", "a", NULL };
        chorda e; 
        for (k = 0; ab[k]; k++) {
            chorda in = chorda_ex_literis(ab[k], p);
            printf("ABS %s\t%d\t%d\n", ab[k], (int)via_est_absoluta(in), (int)via_est_relativa(in));
        }
        e.datum = NIHIL; e.mensura = 0;
        printf("ABS NIHIL\t%d\t%d\n", (int)via_est_absoluta(e), (int)via_est_relativa(e));
        e.datum = (i8*)"\0/x"; e.mensura = 3;
        printf("ABS \\0/x\t%d\t%d\n", (int)via_est_absoluta(e), (int)via_est_relativa(e));
    }
    piscina_destruere(p);
    return 0;
}

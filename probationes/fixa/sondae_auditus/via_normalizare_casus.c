#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "via.h"
#include <stdio.h>
#include <string.h>

static void scr(chorda c)
{
    unsigned int k;
    if (!c.datum && c.mensura == 0) { printf("<NIHIL>"); return; }
    for (k = 0; k < c.mensura; k++) {
        unsigned char ch = (unsigned char)c.datum[k];
        if (ch == 0) printf("\\0"); else putchar(ch);
    }
}
int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    const char* tab_in[] = {
        "", ".", "./", "./.", "foo/..", "a/b/../..", "/..", "/../foo", "/a/../../b",
        "../foo", "../..", "../../a", "a/../../b", "../../..", "a/../..", "x/../../..",
        "/", "//", "///foo", "//foo", "foo/", "foo/bar/", "foo/.",
        "...", ".hidden", "..foo", "foo..", "a/.../b", "a\\b", "foo//bar", "/foo/../bar",
        "foo/./bar", "a/./../b", "./../a", NULL };
    int k;
    for (k = 0; tab_in[k]; k++) {
        chorda in = chorda_ex_literis(tab_in[k], p);
        chorda o1 = via_normalizare(in, p);
        chorda o2 = via_normalizare(o1, p);
        printf("%s\t", tab_in[k]); scr(o1); printf("\t"); scr(o2); printf("\n");
    }
    /* embedded NUL */
    {
        chorda in; chorda o;
        in.datum = (i8*)"a\0b/../c"; in.mensura = 8;
        o = via_normalizare(in, p);
        printf("NUL:a\\0b/../c\t"); scr(o); printf("\n");
        in.datum = (i8*)"x\0y/z"; in.mensura = 5;
        o = via_normalizare(in, p);
        printf("NUL:x\\0y/z\t"); scr(o); printf("\n");
    }
    piscina_destruere(p);
    return 0;
}

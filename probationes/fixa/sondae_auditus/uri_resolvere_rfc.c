#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "uri.h"
#include <stdio.h>
#include <string.h>

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    /* RFC 3986 5.4.1 + 5.4.2, base http://a/b/c/d;p?q */
    const char* t[][2] = {
        {"g:h","g:h"},{"g","http://a/b/c/g"},{"./g","http://a/b/c/g"},{"g/","http://a/b/c/g/"},
        {"/g","http://a/g"},{"//g","http://g"},{"?y","http://a/b/c/d;p?y"},{"g?y","http://a/b/c/g?y"},
        {"#s","http://a/b/c/d;p?q#s"},{"g#s","http://a/b/c/g#s"},{"g?y#s","http://a/b/c/g?y#s"},
        {";x","http://a/b/c/;x"},{"g;x","http://a/b/c/g;x"},{"g;x?y#s","http://a/b/c/g;x?y#s"},
        {"","http://a/b/c/d;p?q"},{".","http://a/b/c/"},{"./","http://a/b/c/"},{"..","http://a/b/"},
        {"../","http://a/b/"},{"../g","http://a/b/g"},{"../..","http://a/"},{"../../","http://a/"},
        {"../../g","http://a/g"},
        {"../../../g","http://a/g"},{"../../../../g","http://a/g"},{"/./g","http://a/g"},{"/../g","http://a/g"},
        {"g.","http://a/b/c/g."},{".g","http://a/b/c/.g"},{"g..","http://a/b/c/g.."},{"..g","http://a/b/c/..g"},
        {"./../g","http://a/b/g"},{"./g/.","http://a/b/c/g/"},{"g/./h","http://a/b/c/g/h"},
        {"g/../h","http://a/b/c/h"},{"g;x=1/./y","http://a/b/c/g;x=1/y"},{"g;x=1/../y","http://a/b/c/y"},
        {"g?y/./x","http://a/b/c/g?y/./x"},{"g?y/../x","http://a/b/c/g?y/../x"},
        {"g#s/./x","http://a/b/c/g#s/./x"},{"g#s/../x","http://a/b/c/g#s/../x"},{"http:g","http:g"},
        {"https://x/y/../z","https://x/z"},{"https://x","https://x"},{"//other:81/x","http://other:81/x"},
        {"g//h","http://a/b/c/g//h"},{"/a//b/","http://a/a//b/"},
        {NIHIL,NIHIL}};
    Uri* base = uri_creare("http://a/b/c/d;p?q", p);
    int i, bad = 0;
    for (i = 0; t[i][0]; i++) {
        chorda rel; Uri* r; chorda s;
        rel.datum = (i8*)t[i][0]; rel.mensura = (i32)strlen(t[i][0]);
        r = uri_resolvere(base, rel, p);
        if (!r) { printf("%-14s exp=%-26s got=NIHIL  MALUM\n", t[i][0], t[i][1]); bad++; continue; }
        s = uri_construere(r, p);
        { int ok = (s.mensura == strlen(t[i][1]) && memcmp(s.datum, t[i][1], s.mensura) == 0);
          printf("%-14s exp=%-26s got=%.*s %s\n", t[i][0], t[i][1], (int)s.mensura, (const char*)s.datum, ok ? "" : "MALUM");
          if (!ok) bad++; }
    }
    printf("MALA: %d / %d\n", bad, i);
    /* base with port + userinfo inherited by '//g' ? */
    {
        Uri* b2 = uri_creare("http://u@a:81/b/c", p);
        chorda rel; Uri* r;
        rel.datum = (i8*)"//g/x"; rel.mensura = 5;
        r = uri_resolvere(b2, rel, p);
        printf("base u@a:81 + //g/x -> h=%.*s port=%u ui=%.*s via=%.*s (exp h=g port=0 ui='' via=/x)\n",
            (int)r->hospes.mensura, (const char*)r->hospes.datum, (unsigned)r->portus,
            (int)r->userinfo.mensura, (const char*)r->userinfo.datum, (int)r->via.mensura, (const char*)r->via.datum);
        r = uri_resolvere(b2, rel, p);
    }
    piscina_destruere(p);
    return 0;
}

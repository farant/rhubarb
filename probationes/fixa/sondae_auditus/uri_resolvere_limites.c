#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "uri.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void show(const char* tag, Uri* r, Piscina* p)
{
    chorda s;
    if (!r) { printf("%s -> NIHIL\n", tag); return; }
    s = uri_construere(r, p);
    printf("%s -> '%.*s' abs=%u schlen=%u hlen=%u\n", tag, (int)s.mensura, (const char*)s.datum,
        (unsigned)r->est_absoluta, (unsigned)r->schema.mensura, (unsigned)r->hospes.mensura);
}

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    Uri* base = uri_creare("http://a/b/c/d;p?q", p);
    chorda rel;
    char* heap;
    /* non-NUL slice on heap, exact size: ASan sees overread */
    heap = (char*)malloc(5);
    memcpy(heap, "g?y#sXXXX", 5);
    rel.datum = (i8*)heap; rel.mensura = 3;  /* "g?y" */
    show("slice 'g?y' of malloc(5)", uri_resolvere(base, rel, p), p);
    rel.mensura = 5;
    show("exact malloc(5) 'g?y#s' no NUL", uri_resolvere(base, rel, p), p);
    free(heap);
    /* embedded NUL */
    rel.datum = (i8*)"a\0b"; rel.mensura = 3;
    show("embedded NUL 'a\\0b' mensura 3", uri_resolvere(base, rel, p), p);
    rel.datum = NIHIL; rel.mensura = 0;
    show("{NIHIL,0}", uri_resolvere(base, rel, p), p);
    rel.datum = (i8*)"g"; rel.mensura = 1;
    show("NIHIL base", uri_resolvere(NIHIL, rel, p), p);
    show("relative base '/x/y' + g", uri_resolvere(uri_creare("/x/y", p), rel, p), p);
    show("base mailto:a@b + g", uri_resolvere(uri_creare("mailto:a@b", p), rel, p), p);
    show("base file:///x/y + g", uri_resolvere(uri_creare("file:///x/y", p), rel, p), p);
    show("base http://a (empty path) + g", uri_resolvere(uri_creare("http://a", p), rel, p), p);
    show("base http://a?q + ?z", uri_resolvere(uri_creare("http://a?q", p), (rel.datum=(i8*)"?z", rel.mensura=2, rel), p), p);
    rel.datum = (i8*)"http://[::1"; rel.mensura = 11;
    show("malformed rel 'http://[::1'", uri_resolvere(base, rel, p), p);
    /* long base path (> 128 aedificator initial) */
    {
        char buf[600]; int i; Uri* r;
        strcpy(buf, "http://h/");
        for (i = 0; i < 100; i++) strcat(buf, "seg/");
        strcat(buf, "f");
        rel.datum = (i8*)"zz"; rel.mensura = 2;
        r = uri_resolvere(uri_creare(buf, p), rel, p);
        printf("long base via len=%u last=%.*s\n", (unsigned)r->via.mensura, 8, (const char*)r->via.datum + r->via.mensura - 8);
    }
    /* chained: base = previous result */
    {
        Uri* r1; Uri* r2;
        rel.datum = (i8*)"x/y"; rel.mensura = 3;
        r1 = uri_resolvere(base, rel, p);
        rel.datum = (i8*)"../z"; rel.mensura = 4;
        r2 = uri_resolvere(r1, rel, p);
        show("chained (base+x/y)+../z exp http://a/b/c/z", r2, p);
        printf("base->hospes shared with r1? %s\n", base->hospes.datum == r1->hospes.datum ? "yes" : "no");
    }
    piscina_destruere(p);
    return 0;
}

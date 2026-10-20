#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "uri.h"
#include <stdio.h>

static void pc(const char* t, chorda c)
{
    printf(" %s=", t);
    if (c.datum == NIHIL) printf("<nil>");
    else printf("'%.*s'", (int)c.mensura, (const char*)c.datum);
}

static void dump(const char* s, Piscina* p)
{
    Uri u;
    b32 ok = uri_parse(s, &u, p);
    printf("[%s] ok=%u abs=%u", s, (unsigned)ok, (unsigned)u.est_absoluta);
    pc("sch", u.schema); pc("ui", u.userinfo); pc("h", u.hospes);
    printf(" port=%u", (unsigned)u.portus);
    pc("via", u.via); pc("q", u.quaestio); pc("f", u.fragmentum);
    printf("\n");
}

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    const char* casus_[] = {
        "", "/", "?", "#", ":", "@", "[", "a",
        "mailto:a@b.com", "urn:isbn:0451450523", "news:comp.lang.c", "http:",
        "localhost:8080", "a:b", "./a:b",
        "1http://h", "ht tp://h", "+x://h", "://h", "HTTP://h",
        "file:///etc/passwd", "http:///p", "http://", "http://:80/", "http://user@/",
        "http://a@b@c/", "http://h/p@q", "http://h?x=@y", "http://u:p:q@h/",
        "http://h:/", "http://h:0/", "http://h:65535/", "http://h:65536/",
        "http://h:4294967296/", "http://h:4294967376/", "http://h:99999999999999999999/",
        "http://h:-1/", "http://h:+80/", "http://h:0080/", "http://h:8a/", "http://h: 80/",
        "http://[::1]/", "http://[::1]:8080/", "http://[::1/", "http://[::1]x/",
        "http://[::1]:8a/", "http://[fe80::1%25en0]/", "http://[v1.x]/", "http://::1/",
        "http://h/%zz", "http://h/%", "http://h/%00x", "http://h/a%2Fb",
        "http://h/?", "http://h/", "http://h/#", "http://h",
        "http://h/p#a?b", "http://h/p?a#b#c", "http://h/p?a?b", "/p#f?q",
        "//host/p", "///p", "////",
        "/proxy/http://evil.com/x", "a/b://c", "//h/x://y", "page.html?u=http://x",
        "http://h/\x7f\x01 \t\n", "http://h/\xc3\x28",
        NIHIL };
    int i;
    for (i = 0; casus_[i]; i++) dump(casus_[i], p);
    piscina_destruere(p);
    return 0;
}

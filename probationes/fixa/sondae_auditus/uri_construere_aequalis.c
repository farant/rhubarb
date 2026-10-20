#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "uri.h"
#include <stdio.h>
#include <string.h>

static chorda ch(const char* s) { chorda c; c.datum = (i8*)s; c.mensura = s ? (i32)strlen(s) : 0; if (!s) c.datum = NIHIL; return c; }

static void rt(const char* s, Piscina* p)
{
    Uri* u = uri_creare(s, p); chorda o; Uri* u2;
    o = uri_construere(u, p);
    { char buf[256]; memcpy(buf, o.datum, o.mensura); buf[o.mensura] = 0;
      u2 = uri_creare(buf, p);
      printf("rt [%s] -> [%s] %s reparse_aequalis=%u\n", s, buf,
        (strlen(s) == o.mensura && memcmp(s, o.datum, o.mensura) == 0) ? "SAME" : "DIFF",
        (unsigned)uri_aequalis(u, u2)); }
}

static void bu(const char* tag, Uri* u, Piscina* p)
{
    chorda o = uri_construere(u, p);
    printf("build %-40s -> '%.*s'\n", tag, (int)o.mensura, o.datum ? (const char*)o.datum : "");
}

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    Uri u;
    const char* rts[] = {"https://user:pass@example.com:8080/path?query=1#section", "//host/p", "//host",
        "http://h/?", "http://h/#", "http://h", "mailto:a@b.com", "file:///etc/passwd", "http://[::1]:8080/",
        "?y", "#f", "http://h:0/", "a:b", "http://h/a b", NIHIL};
    int i;
    for (i = 0; rts[i]; i++) rt(rts[i], p);

    memset(&u, 0, sizeof u); bu("zeroed", &u, p);
    { chorda o = uri_construere(NIHIL, p); printf("build NIHIL -> mensura %u datum %p\n", (unsigned)o.mensura, (void*)o.datum); }
    memset(&u, 0, sizeof u); u.schema = ch("http"); u.hospes = ch("h"); u.via = ch("a"); bu("http + h + via 'a'", &u, p);
    memset(&u, 0, sizeof u); u.via = ch("//evil/x"); bu("no auth, via '//evil/x'", &u, p);
    memset(&u, 0, sizeof u); u.via = ch("a:b/c"); bu("no scheme, via 'a:b/c'", &u, p);
    memset(&u, 0, sizeof u); u.schema = ch("https"); u.hospes = ch("h"); u.portus = 443; bu("https:443", &u, p);
    memset(&u, 0, sizeof u); u.schema = ch("http"); u.hospes = ch("h"); u.portus = 70000; bu("portus 70000", &u, p);
    memset(&u, 0, sizeof u); u.schema = ch("http"); u.portus = 80; u.via = ch("/x"); bu("portus no hospes", &u, p);
    memset(&u, 0, sizeof u); u.schema = ch("http"); u.hospes = ch("::1"); u.portus = 80; bu("hospes '::1' unbracketed", &u, p);
    memset(&u, 0, sizeof u); u.schema = ch("http"); u.hospes = ch("h"); u.via = ch("/a?b#c"); u.quaestio = ch("x#y"); u.userinfo = ch("a@b/c"); bu("raw ? # @ / in fields", &u, p);
    memset(&u, 0, sizeof u); u.schema = ch("mailto"); u.via = ch("a@b.com"); u.est_absoluta = VERUM; bu("mailto via a@b.com", &u, p);
    memset(&u, 0, sizeof u); u.est_absoluta = VERUM; u.hospes = ch("h"); bu("est_absoluta VERUM, schema empty", &u, p);
    memset(&u, 0, sizeof u); u.portus = 4294967295u; u.hospes = ch("h"); bu("portus 2^32-1", &u, p);

    /* aequalis */
    {
        const char* pr[][2] = {
            {"http://alice@h/p", "http://bob@h/p"},
            {"HTTP://Example.COM/p", "http://example.com/p"},
            {"http://h/%2f", "http://h/%2F"},
            {"http://h/%41", "http://h/A"},
            {"http://h/a/../b", "http://h/b"},
            {"http://h/", "http://h:80/"},
            {"http://h", "http://h/"},
            {"http://h/p#a", "http://h/p#b"},
            {"http://h:0/", "http://h/"},
            {"//h/p", "x://h/p"},
            {NIHIL, NIHIL}};
        for (i = 0; pr[i][0]; i++) {
            Uri* a = uri_creare(pr[i][0], p); Uri* b = uri_creare(pr[i][1], p);
            printf("aeq [%s] [%s] = %u / sym %u\n", pr[i][0], pr[i][1], (unsigned)uri_aequalis(a, b), (unsigned)uri_aequalis(b, a));
        }
        { Uri* a = uri_creare("http://h/", p);
          printf("aeq NIHIL,NIHIL=%u NIHIL,a=%u a,a=%u\n", (unsigned)uri_aequalis(NIHIL, NIHIL), (unsigned)uri_aequalis(NIHIL, a), (unsigned)uri_aequalis(a, a)); }
        { Uri a2, b2; memset(&a2,0,sizeof a2); memset(&b2,0,sizeof b2); a2.hospes = ch("h"); b2.hospes = ch("h"); b2.est_absoluta = VERUM;
          printf("aeq est_absoluta differs = %u; uri_est_absoluta(b2 w/o schema)=%u; NIHIL=%u\n", (unsigned)uri_aequalis(&a2,&b2), (unsigned)uri_est_absoluta(&b2), (unsigned)uri_est_absoluta(NIHIL)); }
    }
    /* portus_default */
    {
        const char* s[] = {"https","http","ftp","HTTPS","Http","httpsx","htt","ftps","ws","wss","ssh","gopher",NIHIL};
        chorda c;
        for (i = 0; s[i]; i++) printf("default(%s)=%u\n", s[i], (unsigned)uri_portus_default(ch(s[i])));
        c.datum = (i8*)"httpsXYZ"; c.mensura = 5; printf("default(slice https of httpsXYZ)=%u\n", (unsigned)uri_portus_default(c));
        c.datum = (i8*)"http\0s"; c.mensura = 6; printf("default(http\\0s,6)=%u\n", (unsigned)uri_portus_default(c));
        c.datum = (i8*)"http\0"; c.mensura = 5; printf("default(http\\0,5)=%u\n", (unsigned)uri_portus_default(c));
        c.datum = NIHIL; c.mensura = 0; printf("default({NIHIL,0})=%u\n", (unsigned)uri_portus_default(c));
        c.datum = NIHIL; c.mensura = 5; printf("default({NIHIL,5})=%u\n", (unsigned)uri_portus_default(c));
    }
    piscina_destruere(p);
    return 0;
}

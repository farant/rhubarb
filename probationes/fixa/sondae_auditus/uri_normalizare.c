#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "uri.h"
#include <stdio.h>
#include <string.h>

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    /* RFC 5.2.4 remove_dot_segments expected */
    const char* t[][2] = {
        {"/../a","/a"},{"/a/../../b","/b"},{"../a","a"},{"a/../..",""},{"..",""},{".",""},
        {"/a/.","/a/"},{"/a/..","/"},{"/a/b/","/a/b/"},{"/a//b","/a//b"},{"//","//"},
        {"/a/%2e%2e/b","/b"},{"/%2E/","/"},{"/a/..b","/a/..b"},{"/.../","/.../"},{"/a./","/a./"},
        {"/","/"},{"",""},{"/a/b/c/./../../g","/a/g"},{"mid/content=5/../6","mid/6"},
        {NIHIL,NIHIL}};
    int i;
    for (i = 0; t[i][0]; i++) {
        Uri u; chorda before, r; int ok;
        memset(&u, 0, sizeof u);
        u.via.datum = (i8*)t[i][0]; u.via.mensura = (i32)strlen(t[i][0]);
        if (u.via.mensura == 0) u.via.datum = NIHIL;
        before = u.via;
        r = uri_via_normalizare(&u, p);
        ok = (r.mensura == strlen(t[i][1]) && (r.mensura == 0 || memcmp(r.datum, t[i][1], r.mensura) == 0));
        printf("%-22s exp=%-10s got=%s%.*s%s %s%s\n", t[i][0], t[i][1], r.datum ? "'" : "<nil>",
            (int)r.mensura, r.datum ? (const char*)r.datum : "", r.datum ? "'" : "", ok ? "" : "MALUM",
            (u.via.datum != before.datum || u.via.mensura != before.mensura) ? " (uri->via MUTATA)" : "");
    }
    { chorda r = uri_via_normalizare(NIHIL, p); printf("NIHIL uri -> mensura %u\n", (unsigned)r.mensura); }
    piscina_destruere(p);
    return 0;
}

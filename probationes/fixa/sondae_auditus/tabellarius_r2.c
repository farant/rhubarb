/* sonda r2: Content-Length valor pravus -> 0 / praefixum numericum */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "tabellarius.h"
#include <stdio.h>
#include <string.h>

interior vacuum
_proba (constans character* titulus, constans character* fluxus_textus)
{
    Piscina* piscina = piscina_generare_dynamicum("sonda", 65536);
    FILE* f = tmpfile();
    chorda c;
    b32 finitus;
    integer k;

    fputs(fluxus_textus, f);
    rewind(f);
    printf("== %s\n", titulus);
    per (k = 0; k < 3; k++)
    {
        c = tabellarius_epistulam_legere(f, piscina, &finitus);
        printf("  vocatio %d: mensura=%u finitus=%d datum=%s corpus=[%.*s]\n",
            k, (unsigned)c.mensura, (int)finitus,
            c.datum ? "non-NIHIL" : "NIHIL",
            (int)c.mensura, c.datum ? (constans character*)c.datum : "");
        si (finitus) frange;
    }
    fclose(f);
    piscina_destruere(piscina);
}

integer
principale (vacuum)
{
    _proba("abc", "Content-Length: abc\r\n\r\n{\"jsonrpc\":\"2.0\",\"method\":\"m\"}");
    _proba("abc + sequens", "Content-Length: abc\r\n\r\n{\"a\":1}Content-Length: 2\r\n\r\n{}");
    _proba("praefixum 2xyz", "Content-Length: 2xyz\r\n\r\n{\"a\":1}");
    _proba("vacuum", "Content-Length:\r\n\r\n{}");
    _proba("legitimum 0", "Content-Length: 0\r\n\r\n");
    redde 0;
}

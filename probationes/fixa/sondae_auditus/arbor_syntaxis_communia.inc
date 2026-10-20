#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "arbor_syntaxis.h"
#include <stdio.h>
#include <string.h>

interior ArborSyntaxisResultus
_sonda_parsere(Piscina* p, InternamentumChorda* intern, constans character* fons)
{
    ArborSyntaxis* syn = arbor_syntaxis_creare(p, intern);
    redde arbor_syntaxis_parsere_fontem(syn, fons, (i32)strlen(fons), "sonda_via.c");
}

interior vacuum
_sonda_errores(ArborSyntaxisResultus res)
{
    i32 i, n;
    n = res.errores ? xar_numerus(res.errores) : ZEPHYRUM;
    imprimere("  successus=%d errores=%u\n", (integer)res.successus, (insignatus integer)n);
    per (i = ZEPHYRUM; i < n; i++)
    {
        ArborError* e = *(ArborError**)xar_obtinere(res.errores, i);
        imprimere("   err %u:%u '%.*s' via_file.mensura=%u\n", (insignatus integer)e->linea,
            (insignatus integer)e->columna, (integer)e->nuntius.mensura, (constans character*)e->nuntius.datum,
            (insignatus integer)e->via_file.mensura);
    }
}

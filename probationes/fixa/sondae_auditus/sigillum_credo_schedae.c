/* schedae credo pro inventis sigilli - debent HODIE frangi */
#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "sigillum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

nomen structura {
    SigillumContextus contextus;
                   i8 custos[256];
} SigillumInvolucrum;

interior vacuum
_finire_bis (vacuum)
{
    SigillumContextus ctx;
             Sigillum primum;
             Sigillum secundum;

    sigillum_incipere(&ctx);
    sigillum_addere(&ctx, "abc", III);
    primum  = sigillum_finire(&ctx);
    secundum = sigillum_finire(&ctx);
    (vacuum)primum;
    (vacuum)secundum;
    si (ctx.massa_mensura > LXIII)
    {
        abort();
    }
}

s32 principale (vacuum)
{
    Piscina* piscina;
    piscina = piscina_generare_dynamicum("sonda", 65536);
    credo_aperire(piscina);

    /* A: finire bis non ruit */
    {
        imprimere("\n--- Probans finire bis ---\n");
        CREDO_NON_RUIT(_finire_bis());
    }

    /* B: addere post finire intra contextum manet */
    {
        SigillumInvolucrum inv;
                       i8 zeta[CC];
                  integer k;
                      b32 integer_custos = VERUM;

        imprimere("\n--- Probans addere post finire ---\n");
        memset(inv.custos, 0xA5, magnitudo(inv.custos));
        memset(zeta, 'Z', magnitudo(zeta));
        sigillum_incipere(&inv.contextus);
        sigillum_addere(&inv.contextus, "abc", III);
        (vacuum)sigillum_finire(&inv.contextus);
        sigillum_addere(&inv.contextus, zeta, (memoriae_index)CC);
        CREDO_MINOR_AUT_AEQUALIS_I32(inv.contextus.massa_mensura, LXIII);
        per (k = ZEPHYRUM; k < 256; k++)
        {
            si (inv.custos[k] != 0xA5) { integer_custos = FALSUM; }
        }
        CREDO_VERUM(integer_custos);
    }

    /* C: invariantia camporum post finire */
    {
        SigillumContextus ctx;

        imprimere("\n--- Probans invariantia post finire ---\n");
        sigillum_incipere(&ctx);
        sigillum_addere(&ctx, "abc", III);
        (vacuum)sigillum_finire(&ctx);
        CREDO_MINOR_AUT_AEQUALIS_I32(ctx.massa_mensura, LXIII);
        CREDO_VERUM(ctx.totalis == (i64)III || ctx.totalis == ZEPHYRUM);
    }

    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}

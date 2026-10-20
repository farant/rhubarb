#include "latina.h"
#include "piscina.h"
#include "registrum_commandi.h"
#include <stdio.h>
#include <string.h>

interior b32 mandatum_deficiens(ContextusCommandi* ctx) { (vacuum)ctx; redde FALSUM; }

integer principale(vacuum)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    RegistrumCommandi* reg = registrum_commandi_creare(p);
    ContextusCommandi ctx;
    b32 r_inventus, r_absens, habet;
    memset(&ctx, 0, magnitudo(ctx));
    registrum_commandi_registrare(reg, "deficit", mandatum_deficiens, NIHIL);
    habet = registrum_commandi_habet(reg, "deficit");
    r_inventus = registrum_commandi_executare(reg, "deficit", &ctx);
    r_absens = registrum_commandi_executare(reg, "absens", &ctx);
    printf("habet=%u executare(inventus, functio FALSUM)=%u executare(absens)=%u\n",
        (unsigned)habet, (unsigned)r_inventus, (unsigned)r_absens);
    redde 0;
}

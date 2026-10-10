/* probatio_energia.c - status pilae (S5), fumus in machina vera
 *
 * Machina sine pila (mensae) FALSUM reddit et adest FALSUM; cum pila
 * VERUM, adest VERUM, centesimae 0-100. Lectio iterata idem (intra
 * horam unam, centesimae vix mutantur) - non mentitur. Valor ipse
 * oculis Frani confertur (lectio IOKit). */
#include "latina.h"
#include "piscina.h"
#include "energia.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

s32 principale (vacuum)
{
          Piscina* piscina;
    EnergiaStatus  s;
    EnergiaStatus  t;
              b32  r;

    piscina = piscina_generare_dynamicum("probatio_energia", XVI * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);

    imprimere("\n--- I: status sanus ---\n");
    memset(&s, (integer)VII, magnitudo(s));
    r = energia_status(&s);
    CREDO_VERUM(r == s.adest);
    si (r)
    {
        imprimere("  pila: %d%%%s\n", (integer)s.centesimae,
            s.oneratur ? " (oneratur)" : "");
        CREDO_VERUM(s.centesimae >= ZEPHYRUM && s.centesimae <= C);
    }
    alioquin
    {
        imprimere("  nulla pila\n");
        CREDO_FALSUM(s.oneratur);
    }

    imprimere("\n--- II: lectio iterata ---\n");
    CREDO_VERUM(energia_status(&t) == r);
    si (r)
    {
        CREDO_VERUM(   t.centesimae - s.centesimae <= II
                    && s.centesimae - t.centesimae <= II);
    }
    CREDO_FALSUM(energia_status(NIHIL));

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}

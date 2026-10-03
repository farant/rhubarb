/* probatio_copia_terminalis.c - OSC 52 (eventus B3b-ii): octeti
 * exacti, UTF-8, textus vacuus. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "copia_terminalis.h"
#include "credo.h"
#include <stdio.h>

s32 principale (vacuum)
{
    Piscina* piscina;
     chorda  c;

    piscina = piscina_generare_dynamicum("probatio_copia_terminalis",
        M * LXIV);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);

    imprimere("\n--- I. OSC 52: octeti exacti ---\n");
    c = copia_terminalis_componere(piscina, (constans i8*)"hello", V);
    CREDO_VERUM (chorda_aequalis_literis(c,
        "\033]52;c;aGVsbG8=\033\\"));
    /* UTF-8: e acutum (C3 A9) */
    c = copia_terminalis_componere(piscina, (constans i8*)"\303\251",
        II);
    CREDO_VERUM (chorda_aequalis_literis(c, "\033]52;c;w6k=\033\\"));

    imprimere("\n--- II. textus vacuus: series sine onere ---\n");
    c = copia_terminalis_componere(piscina, (constans i8*)"", ZEPHYRUM);
    CREDO_VERUM (chorda_aequalis_literis(c, "\033]52;c;\033\\"));

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}

/* copia_terminalis.c - Vide copia_terminalis.h */

#include "copia_terminalis.h"
#include "base64.h"
#include <string.h>

#define PRAEFIXUM_COPIAE "\033]52;c;"
#define PRAEFIXI_LONGITUDO ((i32)(magnitudo(PRAEFIXUM_COPIAE) - I))

chorda
copia_terminalis_componere (
        Piscina* piscina,
    constans i8* textus,
            i32  mensura)
{
    chorda b = base64_codificare(textus, mensura, piscina);
    chorda c;
       i32 n = PRAEFIXI_LONGITUDO + b.mensura;

    c.mensura = ZEPHYRUM;
    c.datum = (i8*)piscina_allocare(piscina, (memoriae_index)(n
        + II));
    si (c.datum == NIHIL || (b.datum == NIHIL && mensura > ZEPHYRUM))
    {
        redde c;
    }
    memcpy(c.datum, PRAEFIXUM_COPIAE,
        (memoriae_index)PRAEFIXI_LONGITUDO);
    si (b.mensura > ZEPHYRUM)
    {
        memcpy(c.datum + PRAEFIXI_LONGITUDO, b.datum,
            (memoriae_index)b.mensura);
    }
    /* ST (ESC \) terminat */
    c.datum[n]      = (i8)0x1B;
    c.datum[n + I]  = '\\';
    c.mensura       = n + II;
    redde c;
}

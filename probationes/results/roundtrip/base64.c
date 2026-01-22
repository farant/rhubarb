/* base64.c - Base64 Codificatio et Decodificatio
 *
 * Implementatio RFC 4648 Base64.
 */

#include "base64.h"
#include "piscina.h"

#include <string.h>
character*
alphabetum ;
/* ========================================================================
 * TABULAE
 * ======================================================================== */s8 tabula_decodificationis[CXXVIII] ;


/* ========================================================================
 * FUNCTIONES - UTILITAS
 * ======================================================================== */

i32
base64_longitudo_codificata(i32 mensura_originalis)
{
    /* Formula: 4 * ceil(n / 3) */
    si (mensura_originalis <= 0)
    {
        redde 0;
    }

    redde ((mensura_originalis + II) / III) * IV;
}
/* ========================================================================
 * FUNCTIONES - UTILITAS
 * ======================================================================== */
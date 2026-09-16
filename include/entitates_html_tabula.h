/* entitates_html_tabula.h - Declaratio tabulae GENERATAE
 * (lib/entitates_html_tabula.c; tools/entitates_html_generare.sh).
 * Caput proprium ut clausura inclusionum (aedilis) fontem generatum
 * trahat; facies publica in entitates_html.h. */

#ifndef ENTITATES_HTML_TABULA_H
#define ENTITATES_HTML_TABULA_H

#include "latina.h"

nomen structura {
    /* 'amp;' aut forma vetus 'amp' (sine ';'); NUL terminatus */
    constans character* titulus;
    /* UTF-8, NUL terminati */
    constans character* octeti;
    /* octeti UTF-8, I..VI */
    i32 mensura;
} EntitasHtml;

/* GENERATA: ordine octetorum titulorum (strcmp) */
externus constans EntitasHtml ENTITATES_HTML[];
externus constans i32 ENTITATES_HTML_NUMERUS;

#endif /* ENTITATES_HTML_TABULA_H */

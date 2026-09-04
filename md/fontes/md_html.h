/* md_html.h - markdown -> html per COMPOSITIONEM STML (B2)
 *
 * Fons markdown parsatur (md_arbor), in STML proicitur
 * (materia_arbor_scribere_nodum cum consilio md), cum programmate
 * templorum (md/html/md-html.stml - introitus '#@md-nodus', dispensator
 * sui vocans, B1) in documentum UNUM componitur:
 *
 *   <radix> programma proiectio
 *     <EXEMPLAR modus="unum" output="$doc"><documentum $d/></EXEMPLAR>
 *     <html-fragmentum><PER congruentia="$doc">
 *       <<#@md-nodus>><@n=>&@d;</></PER></html-fragmentum>
 *   </radix>
 *
 * expanditur et distribuitur (catena stml), fragmentum in HTML SINE
 * doctype vertitur (stml_html_vertere_liberos). Nihil partiale: in
 * vitio html vacua est et causa nominatur. Documentum compositum
 * redditur pro inspectione (stml expandere manu).
 */

#ifndef MD_HTML_H
#define MD_HTML_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "internamentum.h"

nomen structura {
       b32 successus;
    chorda html;     /* fragmentum HTML (sine doctype) */
    chorda causa;    /* vitium nominatum (aut vacua) */
    chorda stml;     /* documentum compositum (aut vacua) */
} MdHtmlResultus;

/* programma = textus templorum (md/html/md-html.stml lectus a vocante) */
MdHtmlResultus
md_html_reddere (
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* fons,
                    i32  mensura,
                 chorda  programma);

#endif /* MD_HTML_H */

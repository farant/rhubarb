/* navigator_paginae.h - articulus in paginas (navigator N3.1).
 * Articulus (articulus.h) in CodexCaput UNUM vertitur (contractus
 * codex.h: clausulae "\n\n" iunctae, offsets in textu) quem
 * lector_paginare (lector_paginae.h) paginat - articulus totus fluit,
 * sectiones paginas novas NON incipiunt:
 *   titulus articuli, tituli sectionum, TITULUS -> CODEX_TITULUS
 *   PARAGRAPHUS, CITATIO                       -> CODEX_PARAGRAPHUS
 *   INDEX gradu g -> CODEX_PARAGRAPHUS, 2(g-1) spatia + "• "
 *   FIGURA        -> omittitur (imagines N5)
 *   adiuncta      -> NON in columna principali (columna lateralis N3.5)
 * Notae (nexus, emphasis, fortis) in offsets textus capitis
 * translatae (N4 nexus in lineis invenit). Pura. */

#ifndef NAVIGATOR_PAGINAE_H
#define NAVIGATOR_PAGINAE_H

/* <aedilis corpus="lib/navigator_paginae.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "codex.h"
#include "articulus.h"

/* COLUMNA LATERALIS (N3.5; spec V.13-14, V.18): contentum ancillare
 * paginae affixum - nunc adiuncta (infobox) sectionis. Columna = caput
 * suum (titulus adiuncti, tituli interni, linea per par "Clavis:
 * valor", paragraphi; nexus servati) et ancora = offset clausulae
 * primae sectionis in caput.textus articuli (pagina possessoris = quae
 * eum continet). Columna una per sectionem cum adiunctis. Instrumentum
 * generale: tabulae, figurae postea. */
nomen structura {
           i32  ancora;
    CodexCaput  caput;
           Xar* notae;          /* ArticulusNota in caput.textus */
} NavigatorColumna;

nomen structura {
    CodexCaput caput;          /* articulus: textus + clausulae
                                 * (lector_paginare) */
          Xar* notae;           /* ArticulusNota, offsets in
                                 * caput.textus, ordine textus */
    /* INDEX NEXUUM (spec V.19): fluxus SECUNDUS, in paginis novis post
     * articulum - titulus "Links", deinde linea per scopum distinctum
     * (URL sine #ancora), ordine primae apparitionis. Linea = titulus
     * scopi: in hospite articuli /wiki/<T> -> T decodicatus, '_' ->
     * spatium ("Petiole (botany)"); aliter "<hospes>: <via
     * decodicata>". Linea tota nexus est (nota NEXUS, scopus = URL
     * prima apparitionis). Nullis nexibus: nexus.textus vacuus. */
    CodexCaput  nexus;
           Xar* notae_nexuum;    /* ArticulusNota in nexus.textus */
           Xar* columnae;        /* NavigatorColumna, ordine textus */
} NavigatorTextus;

/* NIHIL solum memoria deficiente */
NavigatorTextus*
navigator_textum_facere (
    constans Articulus* articulus,
               Piscina* piscina);

#endif /* NAVIGATOR_PAGINAE_H */

/* entitates_html.h - Referentiae characterum nominatae HTML5
 *
 * Tabula MMCCXXXI titulorum (WHATWG entities.json per html.entities
 * Pythonis) in lib/entitates_html_tabula.c GENERATA
 * (tools/entitates_html_generare.sh; -probare = recens?): 'amp;' cum
 * ';' et CVI formae VETERES sine ';' ('amp', 'copy', 'not' ...),
 * octeti UTF-8 (puncta I aut II, <= VI octeti), ordine octetorum
 * titulorum. Quaestio binaria exacta et decoctor HTML5 plenus
 * (numericae cum aut sine ';', C1 -> Windows-1252, invalidae U+FFFD;
 * nominatae congruentia LONGISSIMA, veteres inclusae; exceptio
 * attributi). Consumentes: md_decoctum (CommonMark: titulus cum ';'
 * exactus, per quaerere) et html_coctum (decoquere). Natum O2b-4
 * (2026-09-15) ex oraculo html5lib et politura md 01M1NBEVM0.
 */

#ifndef ENTITATES_HTML_H
#define ENTITATES_HTML_H

#include "latina.h"
#include "entitates_html_tabula.h"

/* Titulus exactus [mensura octeti] ('amp;' aut 'amp') -> entitas aut
 * NIHIL. Quaestio binaria. */
constans EntitasHtml*
entitates_html_quaerere (
    constans character* titulus,
                   i32  mensura);

/* Referentiam characteris HTML5 ab 'ab' (fons[ab] == '&') decoquere:
 * numerica '&#65' '&#x41' cum aut sine ';' (nulla digitus = nulla
 * referentia; ZEPHYRUM, > 0x10FFFF, surrogata -> U+FFFD; C1 0x80-0x9F
 * -> Windows-1252 ubi tabula dat), aut nominata: congruentia
 * LONGISSIMA in tabula (cum ';' primum, deinde formae veteres sine
 * ';' a longiore). in_attributo: forma vetus ante '=' aut litteram/
 * digitum NON decoquitur (spec, 'historical reasons'). Octeti UTF-8
 * in exitus (capacitas >= VIII), *longitudo, *post (index post
 * referentiam). FALSUM = nulla referentia ('&' litteralis manet). */
b32
entitates_html_decoquere (
    constans character* fons,
                   s32  ab,
                   s32  ad,
                   b32  in_attributo,
             character* exitus,
                   i32* longitudo,
                   s32* post);

#endif /* ENTITATES_HTML_H */

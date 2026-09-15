/* html_exempla.h - Lector plagularum .dat html5lib (tree-construction)
 *
 * Forma (html5lib-tests, support.py TestData - hic speculata): linea
 * cuius forma detonsa '#' incipit = titulus sectionis; '#data'
 * exemplum novum aperit. Sectio quaeque lineas suas cum '\n' cumulat,
 * deinde '\n' UNUM caudale perdit; sectio ULTIMA exempli (ante '#data'
 * proximum) ALTERUM prius perdit (linea vacua separans). Ergo initus
 * in lineam vacuam desinens = initus in '\n' desinens. Sectiones:
 * data, errors, new-errors (ignorata), document-fragment (linea
 * sequens = contextus, e.g. 'td' aut 'svg desc'), script-on/off,
 * document (arbor exspectata lineis '| '). Octeti crudi: plagulae
 * 'unsafe' NUL ferunt - lector chordis vivit, non literis.
 * Manifestum fixurae: HTML_EXEMPLA_PLAGULAE (probationes/fixa/html/
 * FONTES.md); numeri in probatio_html_exempla pinnati.
 */

#ifndef HTML_EXEMPLA_H
#define HTML_EXEMPLA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"

/* '#script-on' / '#script-off'; sine sectione script-off valet */
enumeratio {
    HTML_EXEMPLUM_SCRIPTUM_INCERTUM = 0,
    HTML_EXEMPLUM_SCRIPTUM_CLAUSUM,
    HTML_EXEMPLUM_SCRIPTUM_APERTUM
};

nomen structura {
    /* I-basatus, ordine plagulae */
    i32 numerus;
    /* linea tituli '#data', I-basata */
    i32 linea;
    /* initus: octeti crudi, copia in piscina */
    chorda datum;
    /* sectio '#errors' (ignoranda ab oraculo, servata) */
    chorda errores;
    /* '#document-fragment' adest */
    b32 fragmentum;
    /* titulus elementi contextus fragmenti ('td', 'svg desc'), vacua
     * si documentum */
    chorda contextus;
    /* HTML_EXEMPLUM_SCRIPTUM_* */
    i32 scriptum;
    /* arbor exspectata: lineae '| ...' '\n' separatae, sine ultimo */
    chorda documentum;
} HtmlExemplum;

/* Viae plagularum .dat relativae radici (RHUBARB_RADIX), NIHIL
 * terminatae - manifestum fixurae vendendae. */
externus constans character* constans HTML_EXEMPLA_PLAGULAE[];

/* Xar de HtmlExemplum (per valorem). NIHIL = memoria deficit. Tituli
 * ante '#data' primum ignorantur; exemplum sine '#document' manet
 * (documentum vacua) - porta id numerat. */
Xar*
html_exempla_legere (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura);

#endif /* HTML_EXEMPLA_H */

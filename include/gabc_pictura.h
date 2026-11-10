/* gabc_pictura.h - pagina compositionis gabc in mandata delineandi
 *
 * Spec: project-specs/gabc-spec.md §V, G11, G14. Planum:
 * project-specs/gabc-plan-2.md P4. Tabula antiphonale (vicus) et
 * instrumentum neumae (PNG sine fenestra) eadem via pingunt.
 *
 * COLORES (thema): atramentum COLOR_TEXT, lineae systematis et additae
 * COLOR_TEXT_DIM, rubricae rubrum palettae (PALETTE_DARK_RED ut RGBA -
 * COLOR_MANDATI_INDEX indices colorationis legit, non palettam),
 * syllaba aut nota illuminata COLOR_ACCENT_PRIMARY (atramentum et
 * rubrica, non lineae systematis).
 *
 * PARTES: larva neumae_ductae in cursus rectangulorum unius ordinis
 * vertitur (ut terminale glyphas ductas): opacitas plena = color
 * thematis vivus; partialis = RGBA mixtus cum COLOR_BACKGROUND in
 * pingendo (Mandata misturam non habent). Fundum non pingitur - vocans
 * id pingit.
 *
 * TEXTUS: notae compositae (U+0300-036F) demuntur ante mandatum (fons
 * cellulam propriam eis daret; compositio eas non numerat). Runae quas
 * glyphae_ductae pingunt (œ Œ ǽ ...) per tabulam pixelorum eo cadunt.
 *
 * INITIALIS: littera fontis VIII x XVI (CP437) pixelo quoque ut
 * quadrato scala x scala, cursus ordinis coniuncti; nullum mandatum
 * textus.
 */
#ifndef GABC_PICTURA_H
#define GABC_PICTURA_H

/* <aedilis corpus="lib/gabc_pictura.c"/> */

#include "latina.h"
#include "gabc_compositio.h"
#include "mandatum.h"

nomen structura {
    s32 syllaba_illuminata;      /* -1 nulla */
    s32 nota_illuminata;         /* -1 nulla */
} GabcPicturaStatus;

/* paginam compositionis ad (x, y) in mandata; status NIHIL = nihil
 * illuminatum. FALSUM si pagina extra aut argumentum NIHIL (mandata
 * tunc intacta). */
b32
gabc_paginam_pingere (
       constans GabcCompositio* compositio,
                           i32  pagina,
                           s32  x,
                           s32  y,
    constans GabcPicturaStatus* status,
                       Mandata* mandata);

#endif /* GABC_PICTURA_H */

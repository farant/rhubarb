/* saltuarius_proba.h - Adiutores aureorum cellularum (spec-v2
 * par 7)
 *
 * Aurei asserunt CELLAS, non octetos: opus pingitur, cratis tergi
 * per tessera_cellulam_legere legitur - tessera_praesentare NON
 * requiritur. Aurei octetorum in suite TESSERAE manent.
 */

#ifndef SALTUARIUS_PROBA_H
#define SALTUARIUS_PROBA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "tessera.h"

/* Ordinem cellularum in chordam legere: signum 0 -> ' ', alia
 * octeti UTF-8 compacti (LSB primus). Spatia caudae praecisa. */
chorda
saltuarius_proba_linea (
        Piscina* piscina,
    TesseraOpus* opus,
            s32  y,
            s32  x_initium,
            i32  mensura);

/* Prima columna in ordine y ubi textus incipit; -1 si absens.
 * Comparatio runa-per-runam (textus UTF-8 licet). */
s32
saltuarius_proba_quaerere (
           TesseraOpus* opus,
                   s32  y,
    constans character* textus);

/* Cellam directe legere (asserta stilorum:
 * PROBA_CELLA(o,x,y).ornamenta & TESSERA_ORNAMENTUM_INVERSUM) */
#define PROBA_CELLA(opus, x, y) tessera_cellulam_legere((opus), (x), (y))

#endif /* SALTUARIUS_PROBA_H */

/* crusta_exempla.h - Lector casuum crustae (forma Oils 'spec/')
 *
 * Forma (plagulae 'spec' Oils, x.test.sh, hic speculata ut plagula
 * Oils immutata legatur): linea '#### titulus' casum aperit; lineae
 * ante '####' primum praefatio sunt (ignorantur); DATUM = octeti
 * omnes a linea
 * post titulum usque ad initium lineae primae '## ' aut '####'
 * proximi (aut EOF); '## NOMEN: textus' cum textu non vacuo post
 * colon = sectio unius lineae; '## NOMEN:' solum sectionem plurium
 * linearum aperit quae ad '## END' (exclusum) desinit - intra eam
 * omnis linea contentum est. Lector octetis vivit: casus CRLF '\r'
 * suum servant (titulus et textus unius lineae detonsi, datum crudum).
 * Chordae copiae in piscina sunt (fons constans manet).
 *
 * Manifestum casuum FreeBSD vendatorum (probationes/fixa/crusta/
 * FONTES.md): CRUSTA_FREEBSD_PLAGULAE - numeri in portis pinnati.
 */

#ifndef CRUSTA_EXEMPLA_H
#define CRUSTA_EXEMPLA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"

nomen structura {
    chorda titulus;
    chorda textus;
} CrustaSectio;

nomen structura {
    /* I-basatus, ordine plagulae */
    i32 numerus;
    /* linea '####', I-basata */
    i32 linea;
    /* post '#### ', detonsus */
    chorda titulus;
    /* octeti inter lineam tituli et sectionem primam / casum
     * proximum */
    chorda datum;
    /* CrustaSectio per valorem */
    Xar* sectiones;
} CrustaExemplum;

/* Viae plagularum FreeBSD bin/sh/tests/parser relativae radici
 * (RHUBARB_RADIX), NIHIL terminatae. */
externus constans character* constans CRUSTA_FREEBSD_PLAGULAE[];

/* Xar de CrustaExemplum (per valorem). NIHIL = memoria deficit. */
Xar*
crusta_exempla_legere (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura);

/* Textus sectionis primae tituli dati; datum NIHIL si absens. */
chorda
crusta_exemplum_sectio (
    constans CrustaExemplum* exemplum,
         constans character* titulus);

#endif /* CRUSTA_EXEMPLA_H */

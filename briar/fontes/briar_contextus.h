/* briar_contextus.h - Fragmenta et transclusio (spec par. 3.4, v1.6):
 * regio '<c! id="x">' FRAGMENTUM est (numquam radix, nusquam sola
 * compilatur); linea '<<#x>>' intra regionem C quamlibet (probatione
 * inclusa) transclusio est - fragmentum ibi contexitur, lineae eius
 * praefixo albo lineae transclusionis praepositae (regula noweb),
 * profunditate prima (fragmenta fragmenta citant). Sigilla STML
 * propria: '<#id>' fragmentum, '<<#id>>' transclusio = ALIAS
 * (decretum 2026-08-26): res una, fenestrae multae - textus contextus
 * est quod compilator videt. briar consumptor C primus qui '<<#id>>'
 * in textum resolvit (stml-visio par. 2).
 *
 * Gradus inter nexum et silvam: radix cum '<<#x>>' C non est, silva
 * eam recusaret. Fructus in regione: contextus (textus contextus) +
 * lineae (tabula linearum: linea .thistle cuiusque lineae contextus,
 * radicis aut fragmenti) + est_fragmentum. Radix sine transclusione
 * tabulam identitatis fert - forma una consumptoribus. Fragmentum:
 * contextus = contentum, lineae NIHIL.
 *
 * Recusationes (causa + linea_erroris in regione, ut silva): definitio
 * in fragmento (id invalidum, methodus/munus, id iteratum), textura in
 * RADICE (fragmentum non definitum, circulus, transclusio malformata).
 * Fragmentum non adhibitum vitium non est (-partes id nominat).
 *
 * Reservata (par. 9): transclusio in medio lineae, fragmenta templi
 * '<#@x>', continuatio 'pars=', genera cruda alia, lexemata arboris
 * cum dialecto briar-c89.
 */

#ifndef BRIAR_CONTEXTUS_H
#define BRIAR_CONTEXTUS_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "briar_nexus.h"

nomen structura {
            chorda  id;      /* sine '#' */
     BriarNexusRes* regio;
               Xar* usus;    /* i32: lineae .thistle transclusionum,
                              * ordine texturae, sine geminis */
} BriarFragmentum;

/* lector lineae: I = transclusio (id et praefixum albus positi),
 * ZEPHYRUM = linea ordinaria (etiam '<<#x>>' in medio: octeti manent),
 * -I = malformata ('<<#' post album initio, forma alia) */
s32
briar_contextus_lineam_legere (
    chorda  linea,
    chorda* id,
    chorda* praefixum);

/* regiones C classificat (id= -> est_fragmentum), radices contexit,
 * recusationes in regione ponit; *fragmenta (si non NIHIL) = Xar de
 * BriarFragmentum ordine documenti (etiam sine fragmentis: vacuum);
 * redde numerum radicum contextarum, -I si memoria deficit */
s32
briar_contexere (
    Piscina*  piscina,
        Xar*  nexus,
        Xar** fragmenta);

#endif /* BRIAR_CONTEXTUS_H */

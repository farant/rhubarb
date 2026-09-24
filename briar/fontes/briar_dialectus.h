/* briar_dialectus.h - charta dialecti (briar -dialectus): quae
 * scriptor plagulae .thistle scire debet ANTE errorem compilationis.
 *
 * Partes DERIVATAE ne putrescant: typi integrorum et tabula verborum
 * ex textu latina.h ipso (eius quod corpus fert), vexilla ex chorda
 * quam briar clang tradit. Pars MANU scripta: laquei C89 (lingua
 * congelata est - non putrescunt).
 */
#ifndef BRIAR_DIALECTUS_H
#define BRIAR_DIALECTUS_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"

/* Charta tota ut textus (lineae '\n'). latina = textus include/
 * latina.h; vexilla_plana / vexilla_vitrea = briar_fabrica_vexilla
 * pro forma plana / vitrea. Chorda vacua si piscina deficit. */
chorda
briar_dialectus_charta (
                 chorda  latina,
     constans character* vexilla_plana,
     constans character* vexilla_vitrea,
                Piscina* piscina);

#endif /* BRIAR_DIALECTUS_H */

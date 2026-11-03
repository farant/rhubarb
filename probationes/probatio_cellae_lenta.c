/* probatio_cellae_lenta.c - sectio lenta probationum cellarum:
 * activatio_iudica contra tabulas exactas D119 pro n = V, VI
 *
 * Olim sectio 'iudicium contra D119 lentum' in probatio_cellae.c; in
 * suite sua ut cum ceteris parallele currat (budget B1 D8: sub
 * veneno ~43% temporis cellarum). Tabulae et auxilia communia in
 * cellae_d119.h; n = III, IV manent in probatio_cellae.c.
 */
#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "quaternio.h"
#include "cellae.h"
#include "cellae_d119.h"

/* n = V, VI (CLXXVII milia copiarum per catalogum et functionem) */
interior vacuum
_probare_iudicium_d119_lentum (
    CredoContextus* c)
{
    Catalogus* zeta   = _zeta_octo_catalogus(c->piscina);
    Catalogus* aurea  = _aurea_prima(c->piscina);
    Activatio* act    = NIHIL;

    CREDO_NECESSE_NON_NIHIL (zeta);
    CREDO_NECESSE_NON_NIHIL (aurea);
    CREDO_NECESSE (activatio_creare(zeta, NIHIL, ZEPHYRUM,
        CELLAE_CELLA_D94, ZEPHYRUM, c->piscina, &act));
    _contra_d119(c, act, D119_ZETA, SCALA_D119, NIHIL, V, VI);
    CREDO_NECESSE (activatio_creare(aurea, NIHIL, ZEPHYRUM,
        CELLAE_CELLA_D94, ZEPHYRUM, c->piscina, &act));
    _contra_d119(c, act, D119_AUREA, SCALA_D119, NIHIL, V, VI);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "iudicium contra D119 lentum", _probare_iudicium_d119_lentum,
        NIHIL, NIHIL, "lentus", NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("cellae_lenta", SECTIONES);
}

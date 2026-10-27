/* admonitiones_volantes.h - admonitiones volantes (toasts;
 * colloquia-plan T4b): stratum vici (vicus_stratum_addere) supra
 * latera, ad dextrum sub linea tabularum, recentissima supra.
 *
 * VISIBILES (ex indice et tempore derivatae, nullum tempus in statu):
 * non lecta, non dimissa, aetate minor quam VIII s (NUNTIUS) aut XXX s
 * (ATTENTIO); III recentissimae talium. Ictus in corpore: porta
 * actionis (ut Enter centri) et lecta; ictus in 'x': dimissa (non
 * lecta - in centro manet). Claves nullae, focus numquam. Manifestum
 * apps/volantes/organum.stml (campus dimissae). */

#ifndef ADMONITIONES_VOLANTES_H
#define ADMONITIONES_VOLANTES_H

/* <aedilis corpus="lib/admonitiones_volantes.c"/> */

#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "insula.h"
#include "actio.h"
#include "figura.h"
#include "dispensator.h"
#include "organum_montatio.h"
#include "admonitiones.h"
/* AdmonitionisActio, AdmonitionumNunc */
#include "admonitiones_visus.h"

nomen structura {
             InsulaRamus  ramus;
          ActioRegistrum* actiones;
         FiguraRegistrum* figurae;
            Admonitiones* index;       /* ligamen (communis centro) */
    /* nuntii hospitis ("Sounds muted"; index proprius, NIHIL = nulli):
     * SOLI volant - nec centrum nec numerus non lectarum nec sonus eos
     * videt (indicem communem legunt). Ictus corporis legit sine
     * porta. Hospes post montationem ponit. */
            Admonitiones* transientes;
       AdmonitionisActio  agere;       /* porta (ut centrum) */
                  vacuum* agere_ctx;
        AdmonitionumNunc  nunc;        /* NIHIL = nulla volat */
                  vacuum* nunc_ctx;
                 Piscina* piscina;
     InternamentumChorda* intern;
                  vacuum* privatum;
} VolantiumMontatio;

/* montare in ramo <volantes id> (manifestum infixum); superficies tota
 * latitudo x altitudo (0 = ordinaria); systemata NIHIL licet */
b32
volantes_montare (
       VolantiumMontatio* m,
                 Piscina* piscina,
     InternamentumChorda* intern,
      InsulaRepositorium* repo,
      constans character* id,
            Admonitiones* index,
       AdmonitionisActio  agere,
                  vacuum* agere_ctx,
                     i32  latitudo,
                     i32  altitudo,
         StatusSystemata* systemata);

/* compositor (facies strati) - ctx = VolantiumMontatio*; radix 0 x 0
 * ad originem, liberi in coordinatis superficiei */
Componens*
volantes_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx);

/* pulsus: VERUM si copia visibilium mutata (nova advenit, exspiravit,
 * lecta) - quadrum pingendum */
b32
volantes_pulsare (
    VolantiumMontatio* m);

#endif /* ADMONITIONES_VOLANTES_H */

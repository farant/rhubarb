/* indagatio_visus.h - INDAGATIO in latere vici (genus "indagatio";
 * iussum $search, Cmd+Shift+F; project-specs/indagatio.md §VII, S3).
 * Manifestum apps/indagatio/organum.stml; index (Indagatio*) per
 * ligamen a parente (vicus unum per volumen tenet); status in ramo:
 * q, genus, pagina - semita "indagatio?q=..&genus=..&pagina=..".
 *
 * QUAESTIO in capite (membrum__prompta): scribenti fructus vivi ex
 * textu promptae; Enter q committit (semita, historia lateris); Esc
 * promptam claudit, q intacta.
 * CONSPECTUS (genus absens): grex quisque cum inventis primis et
 * "+ k more". GREX (genus datum): inventa generis unius, paginata ad
 * altitudinem lateris. INVENTUM: titulus et contextus, deinde linea
 * excerpti cum verbis quaesitis signatis.
 * Claves: j k (electum), Enter (inventum per portam aperire; in
 * "+ k more" grex aperitur), Tab (grex electi), [ ] (paginae gregis),
 * / (quaestio), Esc (grex -> conspectus -> q vacua -> latus tollitur
 * per vicum).
 */

#ifndef INDAGATIO_VISUS_H
#define INDAGATIO_VISUS_H

/* <aedilis corpus="lib/indagatio_visus.c"/> */

#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "insula.h"
#include "actio.h"
#include "figura.h"
#include "dispensator.h"
#include "organum_montatio.h"
#include "indagatio.h"

/* porta: semitam inventi aperire (vicus: in acervo, ut $via;
 * probationes: notatur). FALSUM = non aperta (latus manet). */
nomen b32 (*IndagatioAperire)(vacuum* ctx, chorda semita);

nomen structura {
             InsulaRamus  ramus;
          ActioRegistrum* actiones;
         FiguraRegistrum* figurae;
               Indagatio* index;        /* ligamen (a parente) */
        IndagatioAperire  aperire;      /* NIHIL: Enter nihil agit */
                  vacuum* aperire_ctx;
                 Piscina* piscina;
     InternamentumChorda* intern;
                  vacuum* figurae_ctx;  /* privatum */
} IndagatioMontatio;

/* montare in ramo <indagatio id="..."> (manifestum infixum);
 * superficies latitudo x altitudo (0 = ordinaria); systemata NIHIL
 * licet */
b32
indagatio_visus_montare (
       IndagatioMontatio* m,
                 Piscina* piscina,
     InternamentumChorda* intern,
      InsulaRepositorium* repo,
      constans character* id,
               Indagatio* index,
        IndagatioAperire  aperire,
                  vacuum* aperire_ctx,
                     i32  latitudo,
                     i32  altitudo,
         StatusSystemata* systemata);

/* compositor (dispensator; facies vici) - ctx = IndagatioMontatio* */
Componens*
indagatio_visus_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx);

/* argumentum lateris: URI semitae ("indagatio?q=rhubarb") aut verba
 * nuda (cognomen: -> indagatio?q=<verba>) aut vacuum (-> indagatio).
 * Semita mala: FALSUM, nihil mutatum. */
b32
indagatio_visus_argumentum_ponere (
       IndagatioMontatio* m,
      constans character* argumentum);

/* Cmd+Shift+F: prompta aperitur, q currens in ea (cursor in fine) */
b32
indagatio_visus_quaestionem_aperire (
    IndagatioMontatio* m);

#endif /* INDAGATIO_VISUS_H */

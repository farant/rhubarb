/* admonitiones_visus.h - centrum admonitionum (genus "admonitiones";
 * colloquia-plan T4a) et sequax libri praeconii.
 *
 * CENTRUM: manifestum apps/admonitiones/organum.stml; admonitiones per
 * ligamen (index sequacis, communis omnibus lateribus); status visus in
 * ramo (electa = id, pagina). Ordo: signum (ATTENTIO plenum, NUNTIUS
 * vacuum), titulus, aetas; corpus infra; non lectae colore accentus.
 * Claves: j k (electa), [ ] (paginae), x (lecta), X (omnes lectae:
 * campus radicis admonitiones_lectae_ad), Enter aut ictus duplex
 * (porta actionis; lecta fit), r (T4e: lectae ostenduntur - ordinarie
 * celantur; lecta celata electam ad vicinam movet), Esc (T4d: latus
 * tollitur, per vicum).
 *
 * SEQUAX: index ex cauda libri, deinde per praeconium_legere_ab in
 * pulsu; tituli per sessionem in memoria (transcriptum, aliter nomen
 * directorii). Hospes (vicus) eum in systemate suo pulsat. */

#ifndef ADMONITIONES_VISUS_H
#define ADMONITIONES_VISUS_H

/* <aedilis corpus="lib/admonitiones_visus.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "insula.h"
#include "actio.h"
#include "figura.h"
#include "dispensator.h"
#include "organum_montatio.h"
#include "admonitiones.h"

/* porta actionis (vicus: T4a multiplexor eligit; T4c latus
 * terminalis); VERUM si acta */
nomen b32 (*AdmonitionisActio)(vacuum* ctx, constans Admonitio* x);

/* porta temporis: ms ab epocha (aetas ordinum) */
nomen s64 (*AdmonitionumNunc)(vacuum* ctx);

nomen structura {
             InsulaRamus  ramus;
          ActioRegistrum* actiones;
         FiguraRegistrum* figurae;
            Admonitiones* index;       /* ligamen (a sequace) */
       AdmonitionisActio  agere;       /* porta */
                  vacuum* agere_ctx;
        AdmonitionumNunc  nunc;        /* post montationem; NIHIL =
                                        * sine aetate */
                  vacuum* nunc_ctx;
                 Piscina* piscina;
     InternamentumChorda* intern;
                  vacuum* figurae_ctx; /* privatum */
} AdmonitionumMontatio;

/* montare in ramo <admonitiones id> (manifestum infixum); superficies
 * latitudo x altitudo (0 = ordinaria); systemata NIHIL licet */
b32
admonitiones_montare (
    AdmonitionumMontatio* m,
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

/* compositor (dispensator; facies vici) - ctx =
 * AdmonitionumMontatio* */
Componens*
admonitiones_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx);

/* omnes non recentiores quam tempus (ms) lectae notantur (X centri;
 * vicus in initio ex campo radicis); numerus notatarum */
i32
admonitiones_lectas_notare_ad (
    Admonitiones* a,
             s64  tempus);

/* sequax: index (hospitis, iam communis centro) ex cauda libri
 * (octeti_initiales) impletur, deinde per praeconium_legere_ab; tituli
 * per sessionem in memoria */
nomen structura {
           Admonitiones* index;
     constans character* via;
                    s64  positio;
                Piscina* piscina;     /* diuturna */
                Piscina* scheda;      /* per pulsum vacatur */
                    Xar* tituli;      /* sessio -> titulus */
} AdmonitionumSequax;

b32
admonitiones_sequacem_initiare (
    AdmonitionumSequax* s,
               Piscina* piscina,
          Admonitiones* index,
    constans character* via,
                   i32  octeti_initiales);

/* pulsus: eventus novi -> admonitiones; numerus additarum (0 = nihil
 * novi); *recentissima = id novae recentissimae (pro admonitio_nova;
 * in piscina indicis) */
i32
admonitiones_sequi (
    AdmonitionumSequax* s,
                chorda* recentissima);

#endif /* ADMONITIONES_VISUS_H */

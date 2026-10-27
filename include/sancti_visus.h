/* sancti_visus.h - BIBLIOTHECA SANCTORUM in latere vici (genus
 * "sancti"; iussum $saints, $saints(<id>)). Manifestum
 * apps/sancti/organum.stml; corpus per ligamen (a parente: vicus
 * directorium sancti/ semel aperit - RHUBARB_SANCTI vincit; commune
 * omnibus lateribus); status visus in ramo.
 *
 * INDEX paginatus: linea per sanctum - titulus, festum primum
 * ("Oct 22"), aetas ("1920–2005"), munera. ORDO (clavis s cyclum
 * facit): calendarium (ordinarium; pagina diei hodierni aperitur,
 * electus primus a hodie), nomen (alphabeticus), aetas
 * (chronologicus per obitum, deinde ortum, deinde saeculum; ignoti
 * ultimi). Claves: j k (electus), [ ] (paginae), Enter (aperire),
 * / (quaestio in nominibus omnibus), t (ad hodie), s (ordo), Esc
 * (quaestio vacua, deinde latus tollitur per vicum).
 *
 * SANCTUS: caput (titulus, nomen latinum, ortus et obitus cum locis,
 * canonizatio, munera, festa), descriptio paginata, fontes in fine.
 * Claves: [ ] (paginae textus), j k (sanctus prior / proximus ordine
 * indicis), Esc (ad indicem). */

#ifndef SANCTI_VISUS_H
#define SANCTI_VISUS_H

/* <aedilis corpus="lib/sancti_visus.c"/> */

#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "insula.h"
#include "actio.h"
#include "figura.h"
#include "dispensator.h"
#include "organum_montatio.h"
#include "sancti.h"

/* porta hodie (vicus: horologium; probationes: dies fixa) */
nomen b32 (*SanctorumHodie)(vacuum* ctx, s32* mensis, s32* dies);

nomen structura {
             InsulaRamus  ramus;
          ActioRegistrum* actiones;
         FiguraRegistrum* figurae;
   constans SanctiCorpus* corpus;      /* ligamen (a parente) */
          SanctorumHodie  hodie;
                  vacuum* hodie_ctx;
                 Piscina* piscina;
     InternamentumChorda* intern;
                  vacuum* figurae_ctx; /* privatum */
} SanctorumMontatio;

/* montare in ramo <sancti id="..."> (manifestum infixum); superficies
 * latitudo x altitudo (0 = ordinaria); systemata NIHIL licet */
b32
sancti_visus_montare (
       SanctorumMontatio* m,
                 Piscina* piscina,
     InternamentumChorda* intern,
      InsulaRepositorium* repo,
      constans character* id,
   constans SanctiCorpus* corpus,
          SanctorumHodie  hodie,
                  vacuum* hodie_ctx,
                     i32  latitudo,
                     i32  altitudo,
         StatusSystemata* systemata);

/* compositor (dispensator; facies vici) - ctx = SanctorumMontatio* */
Componens*
sancti_visus_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx);

/* argumentum lateris: URI semitae ("sancti/sanctus/thomas-aquinas",
 * "sancti/index?quaestio=x") aut id sancti nudum (alias: -> sancti/
 * sanctus/<id>) aut vacuum (-> sancti/index). Semita applicatur
 * (semita.h); sanctus ignotus aut semita mala: FALSUM, nihil mutatum.
 * Sanctus apertus per argumentum: origo "argumentum" (Esc latus
 * reddit); electus = sanctus apertus. */
b32
sancti_visus_argumentum_ponere (
       SanctorumMontatio* m,
      constans character* argumentum);

/* comites P5 (facies.advocatum; ctx = SanctorumMontatio*): prompta
 * "quaestio" (quaestione currenti praeimpleta), ordo congruentiae -
 * ut '/' */
b32
sancti_visus_quaestionem_aperire (
    vacuum* ctx);

/* comites P1 (facies.locus_titulus; ctx = SanctorumMontatio*):
 * titulus sancti aperti, index "Saints" */
chorda
sancti_visus_locus_titulus (
     vacuum* ctx,
    Piscina* piscina);

#endif /* SANCTI_VISUS_H */

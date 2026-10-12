/* conservatio.h - SYSTEMA PERSISTENTIAE (status-plan S3): campi vita
 * persistens ramorum registratorum in volumine applicationis servantur
 * (plagula "status/<clavis>") et ad montationem restituuntur.
 *
 * Systema actarium repositorii possidet: post omnem scripturam per
 * portam successam, ramus quisque cuius pars PERSISTENS mutata est
 * scribitur (transientes numquam; mobiles semel per quietem, quia
 * motus insulam solum in quiete tangit).
 *
 * Forma plagulae:
 *   <status versio="1" genus="pictor">
 *     <durabilis .../><ephemera instrumentum="linea" zoom="2"/>
 *   </status>
 *
 * Restitutio = migratio (census XII): campi manifesto ignoti aut non
 * persistentes abiciuntur, absentes valorem initialem retinent;
 * scriptura per portam (canon iudicat) - refusa: status servatus
 * praeteritur (nuntius), montatio numquam frangitur. */

#ifndef CONSERVATIO_H
#define CONSERVATIO_H

/* <aedilis corpus="lib/conservatio.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "volumen.h"
#include "insula.h"
#include "organum.h"

nomen structura Conservatio Conservatio;

/* actarium repositorii ponit (NIHIL si actarius alius iam adest) */
Conservatio*
conservatio_creare (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     InsulaRepositorium* repo);

/* ramum registrare et statum servatum restituere. VOCANDUM post
 * insula_ramum_initiare et ANTE dominos (restitutio anonyma, ut
 * initiatio). clavis = id rami (aut genus pro radice). Redde numerum
 * camporum restitutorum quorum valor MUTATUS est (id idem non
 * numeratur). */
i32
conservatio_ramum_addere (
              Conservatio* conservatio,
     constans InsulaRamus* ramus,
         constans Organum* organum,
       constans character* clavis);

#endif /* CONSERVATIO_H */

/* speculum.h - Modus-debug se-fontis pro apps vitreis
 *
 * Binarium fontem proprium portat (capsula generata a
 * tools/speculum_generare.sh in build/speculum/<titulus>/);
 * speculum eum per pontem praebet (methodus "speculum_obtinere":
 * effusio una - proventus + fontes + documenta + exclusa) et
 * velamen JS in paginam evaluat cum Cmd+Shift+D premitur.
 *
 * Stipes evaluationis SE-SANANS est: si pagina recargata est
 * (window.speculum_praesens abest), tota iniectio iterum currit -
 * nulla memoria in latere C de statu paginae.
 *
 * USUS (in app vitrea):
 *   externus constans CapsulaEmbed capsula_speculi_<titulus>;
 *   ...
 *   spec = speculum_creare(piscina, &capsula_speculi_<titulus>,
 *       inx, vitrea_aestimator, vitrea);
 *   ...in effusione eventuum ansae:
 *   dum (fenestra_obtinere_eventus(fenestra, &eventus))
 *   {
 *       speculum_tangere(spec, &eventus);
 *   }
 *
 * Spec: project-specs/speculum-spec-v2.md; interrogatio:
 * project-specs/speculum-interview.md
 */

#ifndef SPECULUM_H
#define SPECULUM_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "capsula.h"
#include "internuntius.h"
#include "fenestra.h"

nomen structura Speculum Speculum;

/* seam aestimationis: speculum JS per hanc functionem in paginam
 * mittit. Vitrea praebet vitrea_aestimator (datum = Vitrea*) -
 * idem mos ac missor internuntii; probationes fictum dant. */
nomen vacuum (*SpeculumAestimator)(vacuum* datum, chorda js);

/* Creare: capsulam aperit, "speculum_obtinere" in internuntio
 * praebet, stipitem evaluationis semel struit (bona ex capsula:
 * lib/speculum_assets/speculum.{js,css}). Corpora fontium PIGRE
 * leguntur - primum obtinere ea in piscinam inflat et retinet.
 * Redde NIHIL si capsula fracta aut methodus iam praebita. */
Speculum*
speculum_creare (
                  Piscina* piscina,
    constans CapsulaEmbed* fontes,
             Internuntius* inx,
       SpeculumAestimator  aestimator,
                   vacuum* aestimator_datum);

/* In effusione eventuum ansae vocanda. Cmd+Shift+D deprimitur ->
 * stipitem se-sanantem per aestimatorem evaluat. Redde VERUM si
 * eventus consumptus est. */
b32
speculum_tangere (
            Speculum* speculum,
    constans Eventus* eventus);

#endif /* SPECULUM_H */

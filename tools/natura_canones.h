#ifndef NATURA_CANONES_H
#define NATURA_CANONES_H

/* tools/natura_canones.h - exemplar communis inter onerationem
 * et emissionem.
 *
 * natura_canones.c corpus onerat et exemplar elementi PLICAT;
 * natura_canones_emissio.c id in litteras vertit (TSV inspectionis,
 * canonem generatum). Sceletum hic manet quia utraque pars eo
 * eget et neutra eius domina est.
 */

#include "latina.h"
#include "natura.h"
#include "stml.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include <stdio.h>

nomen structura {
         chorda* titulus;     /* nomen naturae, snake_case */
         chorda* modulus;
    NaturaGenus* genus;       /* genus ipsum, aut genus rei continentis */
    NaturaGenus* genus_etiam; /* etiam= resolutum (res sola), vel NIHIL:
                                * apparatus AMBARUM catenarum debetur
                                * (natura.h par. NaturaRes) */
       StmlNodus* nodus;       /* nodus entis (genus aut res) */
             b32  est_res;     /* VERUM = species/individuum/cultivar */
} NcEns;


/* ==================================================
 * Exemplar elementi - apparatus PLICATUS
 *
 * natura_apparatus hereditatem iam solvit (catenam sub= trans
 * exemplaria ascendit); hic dispositio sola restat: quid
 * attributum fiat, quid liberum, quid electio.
 * ================================================== */

nomen enumeratio {
    NC_MEMBRUM_ATTRIBUTUM = I,   /* -> <attributum> */
    NC_MEMBRUM_LIBERUM    = II   /* -> <liberum> + <elementum intra=> */
} NcMembrumDiscrimen;

/* unde membrum venit. Discrimen (attributum/liberum) formam DICIT,
 * originem non: pars et proprietas multiplex ambae liberum fiunt,
 * sed pars nota= fert et proprietas valorem in textu. Emissor id
 * ex genere valoris coniectare posset ('nomen' => relatio), sed
 * coniectura ea nexus est qui tacite frangitur cum genus mutatur. */
nomen enumeratio {
    NC_ORIGO_PROPRIETAS = I,
    NC_ORIGO_PARS       = II,
    NC_ORIGO_RELATIO    = III,
    NC_ORIGO_MACHINA    = IV
} NcMembrumOrigo;

nomen structura {
     NcMembrumDiscrimen  discrimen;
         NcMembrumOrigo  origo;
                 chorda* titulus;       /* nomen naturae, snake */
     constans character* praefixum;     /* "status_" vel NIHIL */
     constans character* genus_valoris; /* "textus"/"nomen"/... */
                    Xar* optiones;      /* Xar de chorda* - electio */
                 chorda* praestitutum;  /* ordinarius=, vel NIHIL */
    /* FINES (spec fines) - verbatim e proprietate in canonem
     * coctum, numerus solus; NIHIL = absens */
                chorda* fractio;
                chorda* finis_minimus;
                chorda* finis_maximus;
    /* MONOLITHUS solum: index nominum entium (snake) quorum
     * identitates hoc membrum citare licet - petitum primum, tum
     * posteri eius omnes (catena sub= ET etiam=), quia claves per
     * titulum EXACTUM colliguntur et subgenus titulum alium fert;
     * NIHIL = nulla citatio. Canon per modulum id numquam ponit,
     * quia relatio ibi plagulas transit et documentum clavem non
     * tenet (spec par. 3.5).
     *
     * DECRETUM ipsius emissoris est ut hoc scribat, non ut id
     * COLLIGAT: quae relatio citari possit et quae clausura eius
     * sit natura_canones.c solus iudicat (petitum resolvi debet -
     * vide ibi). */
                   Xar* citatio_ad;   /* Xar de chorda* */
} NcMembrum;

nomen structura {
      NcEns* ens;
        Xar* membra;     /* Xar de NcMembrum */
        Xar* actiones;   /* Xar de chorda* - pro <eventum actio=> */
} NcElementum;


/* ==================================================
 * Emissio (natura_canones_emissio.c)
 * ================================================== */

/* nomen naturae (snake) -> nomen canonis (kebab) */
vacuum
_kebab_scribere (
               FILE* f,
    constans chorda* t);

/* exemplar unius elementi in TSV - modus -inspicere */
vacuum
_elementum_inspicere (
           FILE* f,
    NcElementum* el);

/* canonem unius plagulae emittere.
 *   elementa  - Xar de NcElementum* (ordo = ordo emissionis)
 *   dialectus - nomen dialecti (= modulus), pro <canon dialectus=>
 *   fons      - unde generatum, pro signo GENERATUM
 *   praefatio - commentarium ADDITUM post signum, vel NIHIL
 *
 * praefatio SEORSUM stat (non intra signum) CONSULTO: custos
 * 'head -3' legit et AMBAS locutiones ibi poscit, ergo prosa per
 * fons= immissa signum in lineam quartam pelleret et plagulam
 * tacite inermem relinqueret - id ipsum quod semel iam momordit.
 * Commentarium proprium post signum eum numquam movet.
 *
 * Redde FALSUM si quid emitti NON potuit: praestitutum generi
 * repugnans, quota in valore ATTRIBUTI (in textu elementi licet),
 * aut octetus nullus in nomine entis, membri, actionis. Tunc NIHIL
 * scriptum valet et vocans plagulam abicere debet - canon dimidius
 * mendacium est. */
b32
_canonem_emittere (
                  FILE* f,
                   Xar* elementa,
    constans character* dialectus,
    constans character* fons,
    constans character* praefatio,
                   Xar* entia,      /* NIHIL = sine clavibus
                                      * externis; monolithus entia
                                      * tradit (librarium W1) */
               Piscina* piscina);

/* census relationum - NUMERI IN CANONEM IPSUM SCRIBUNTUR.
 *
 * Non in commentario fixi: corpus crescit, et numerus fixus
 * mentiretur eo ipso momento quo quis exemplar addit. Canon
 * generatus de SE dicat quantum vere examinet. SEDES numerantur
 * (membra huius canonis), non declarationes generum. */
nomen structura {
    i32 omnes;
    i32 citatae;
    i32 apertae;
    i32 multiplices;
    i32 ignotae;
} NcCensusRelationum;

/* exemplar elementi ex ente aedificare (plicatio hereditate iam
 * soluta) - definitio in natura_canones.c; emissio (semina) per
 * individuum vocat ut membra pro appellationibus habeat */
NcElementum*
_elementum_aedificare (
     NaturaBibliotheca* bib,
                 NcEns* ens,
                   b32  monolithus,
    NcCensusRelationum* census,
               Piscina* piscina);

/* an nodus fontis individuum sit (spec census par. 1: praedicabile
 * non est, tag non gignit) - communis inter iudicium
 * (natura_canones.c) et semina (emissio) */
b32
_nodus_individuum_est (
    constans StmlNodus* n);

/* censum seminum scribere (spec census par. 2): individua
 * dictionarii ut DATA - instantiae tagorum generum suorum in
 * documento .census generato. Valores NODI PROPRII soli (hereditata
 * per ordinarius= tagi veniunt); relationes signum ferunt ('#' si
 * petitum individuum, '.' si genus/species); historia subarbor
 * transcribitur (textus normalizatus); nota curatorialis OMITTITUR.
 * Redde FALSUM ut _canonem_emittere: nihil scriptum valet. */
b32
_censum_seminum_scribere (
     NaturaBibliotheca* bib,
                   Xar* entia,
    constans character* via,
               Piscina* piscina);

#endif /* NATURA_CANONES_H */

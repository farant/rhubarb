/* chronica.h - CHRONICA in latere vici (genus "chronica"; spec
 * project-specs/chronica-spec.md C3). Manifestum
 * apps/chronica/organum.stml; fontes (chronica_fontes.h) a vico
 * dati, communes omnibus lateribus; status visus in ramo.
 *
 * VISUS ANNI: in summo omnis paginae annus magnus (FONS_TITULI):
 * "1446 BC", "AD 1274". Infra eventa anni ordine tabulae: descriptio
 * (paragraphus), deinde linea parva obscura - dies ("15 Jul"; mensis
 * solus "Jul"), genus, personae (", "), locus, " · " iuncta, vacua
 * omissa. Paginae per lector_paginare (fractiones domus: descriptio
 * longa in paginam sequentem fluit, numquam praecisa). Linea ima:
 * "< Timeline - AD 1274 - 1/2 >" (sagittae anni).
 * CLAVES: h / l = annus prior / proximus CUM EVENTIS; j / k = pagina
 * proxima / prior intra annum; sagittae idem (sinistra / dextra =
 * h / l, deorsum / sursum = j / k). In fine: nihil.
 * MUS: ictus in sagitta pedis sinistra / dextra = h / l; ictus in
 * dimidio superiore / inferiore paginae = k / j.
 * POSITIO: annus visus in praesentia voluminis ("chronica/annus")
 * quotiens mutatur; apertio sine argumento eum restituit; numquam
 * visus: annus cum eventis hodie proximus. Annus (restitutus, datus,
 * aut mutatione vacuus factus) sine eventis: proximus cum eventis
 * (aequalitas: prior), pagina 0. Pagina ultra finem: ultima.
 * MUTATIO: tabula in omni compositione legitur (chronica_tabula: sine
 * mutatione eadem, nihil reaedificatur).
 * VACUA: nulla eventa - "Write <event year=\"1274\">...</event> in a
 * note"; pes "Timeline".
 * LOCUS: "chronica/annus/1274", "chronica/annus/-1446"; titulus loci
 * "Timeline - AD 1274 - 1/2". Comes (Cmd+T) anni sui est.
 * (C4: electio et Enter domum; C5: visus personae et loci.) */

#ifndef CHRONICA_H
#define CHRONICA_H

/* <aedilis corpus="lib/chronica.c"/> */

#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "insula.h"
#include "actio.h"
#include "figura.h"
#include "dispensator.h"
#include "organum_montatio.h"
#include "volumen.h"
#include "fasti.h"
#include "chronica_fontes.h"

nomen structura {
             InsulaRamus  ramus;
          ActioRegistrum* actiones;
         FiguraRegistrum* figurae;
          ChronicaFontes* fontes;        /* a vico */
                 Volumen* volumen;       /* praesentia positionis */
                 Piscina* piscina;
     InternamentumChorda* intern;
                  vacuum* figurae_ctx;   /* privatum: cache paginarum */
    /* hodie (annus prope quem prima apertio); vicus: hodie_vici,
     * probationes fictum. NIHIL aut FALSUM = annus primus */
                     b32  (*hodie)(vacuum* ctx, Dies* hodie);
                  vacuum* hodie_ctx;
} ChronicaMontatio;

/* montare in ramo <chronica id="..."> (manifestum infixum);
 * superficies latitudo x altitudo (0 = ordinaria) */
b32
chronica_montare (
       ChronicaMontatio* m,
                Piscina* piscina,
    InternamentumChorda* intern,
     InsulaRepositorium* repo,
     constans character* id,
         ChronicaFontes* fontes,
                Volumen* volumen,
                    i32  latitudo,
                    i32  altitudo,
        StatusSystemata* systemata);

/* compositor (dispensator; facies vici) - ctx = ChronicaMontatio* */
Componens*
chronica_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx);

/* argumentum: "" = positio servata; "1274", "-1446" (annus; sine
 * eventis: proximus); semita "chronica/annus/<n>". Malum (0, non
 * numerus): FALSUM, nihil mutatum. */
b32
chronica_argumentum_ponere (
       ChronicaMontatio* m,
     constans character* argumentum);

/* facies.semitam_secuta (historia, $via, restitutio) */
b32
chronica_semitam_secuta (
    vacuum* ctx);

/* facies.locus */
chorda
chronica_locus (
     vacuum* ctx,
    Piscina* piscina);

/* facies.locus_titulus */
chorda
chronica_locus_titulus (
     vacuum* ctx,
    Piscina* piscina);

#endif /* CHRONICA_H */

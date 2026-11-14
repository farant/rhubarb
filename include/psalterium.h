/* psalterium.h - PSALTERIUM: latus vici (genus "psalterium") cum
 * psalmis I-CL in tonis psalmorum (spec project-specs/psalterium-
 * spec.md; planum project-specs/psalterium-plan.md T3-T7). Manifestum
 * apps/psalterium/organum.stml.
 *
 * DUO VISUS (campus "visus"):
 *   INDEX  - CL ordines "109  Dixit Dóminus Dómino meo" (numerus
 *            Vulgatae, incipit: psalterium_incipit), paginati (album;
 *            pagina electum sequitur, nullus campus paginae indicis).
 *   PSALMUS - psalmus electus aptatus (psalmodia_aptare, titulus
 *            "Psalmus N", Gloria Patri) et per gabc_visus pictus
 *            (paginae, spatium, sonus). Caput supra paginam
 *            (margo visus): "≡" (ad indicem), "‹" (prior), "›"
 *            (proximus), signum toni ("8 G"). Linea ima visus:
 *            "< ▶ Psalmus 109 · 8 G  2/5 >".
 *
 * CLAVES (tabula Praeceptorum per visum):
 *   index:   Up/Down et j/k electum movent; PageUp/PageDown paginam;
 *            Enter psalmum aperit; cifrae numerum psalmi eligunt
 *            ("1" "0" "9" -> 109; numerus ultra CL a cifra nova
 *            incipit; Backspace cifram tollit); Esc latus claudit
 *            (vici ordinarium, effugium_redit).
 *   psalmus: claves visus (Spatium, Left/Right, PageUp/PageDown, j/k,
 *            + = -) et t / T tonus proximus / prior (circulus D8,
 *            in fine redit), ] / [ psalmus proximus / prior (ultra CL
 *            aut ante I nihil), Esc ad indicem (psalmus electus).
 * MUS: index - ordo ictu aperitur, sagittae lineae imae paginam
 *      vertunt; psalmus - "≡" "‹" "›" et signum (tonus proximus)
 *      ictu, cetera ut in visu (sagittae, globulus, syllaba).
 *
 * TONUS (D6, Franus 2026-11-14): campus unus "signum" lateris, absens
 * aut ignotus = "8 G" (tacite); transit cum psalmus mutatur. CIRCULUS
 * (D8): terminationes omnes tabulae infixae ordine tabulae, praeter
 * ad_libitum et monosyllaba (aptator eas ipse eligit) - XXXIII hodie.
 * Tonus aut psalmus mutatus: textus novus visui SINE continuare
 * (sonus sistit); psalmus novus paginam 0, tonus novus paginam servat
 * (ultra finem coercita). Esc ad indicem sonum quoque sistit.
 * Admonitio aptatoris prima = vexillum visus; cantus tamen pingitur.
 *
 * SEMITA: campi "visus" et "psalmus" ligatio (psalterium/index,
 * psalterium/psalmus/109); cognomina "" -> index, numerus ->
 * psalmus/{}. Restitutio (initium, historia) per vicum, deinde
 * psalterium_semitam_secuta psalmum ex campis iterum aptat. Signum
 * non in semita (lateris est, non visus).
 *
 * ARGUMENTUM ($psalterium, spec III.3): "" (index), "109", "109,8 G"
 * (iussum argumenta commate iungit); URI generis ("psalterium/...")
 * directe ad semitam. Vitia: "psalterium: psalm number 1-150";
 * tonus ignotus: "psalterium: " + causa psalmodia_signum_invenire
 * (terminationes toni enumerat).
 *
 * MEMORIA: psalmus aptatus in piscina sua per mutationem (vetus post
 * novam destruitur); textus psalmorum ex psalterium_infixum; nihil
 * horum in statu.
 *
 * SONUS per GabcVisusPorta (hospes ponit; cantor unus vici).
 */
#ifndef PSALTERIUM_H
#define PSALTERIUM_H

/* <aedilis corpus="lib/psalterium.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "internamentum.h"
#include "insula.h"
#include "actio.h"
#include "figura.h"
#include "dispensator.h"
#include "organum_montatio.h"
#include "fasti.h"
#include "gabc_visus.h"
#include "psalmodia_tabula.h"

#define PSALTERIUM_PSALMI              CL
#define PSALTERIUM_SIGNUM_ORDINARIUM   "8 G"

nomen structura {
          InsulaRamus  ramus;
       ActioRegistrum* actiones;
      FiguraRegistrum* figurae;
       GabcVisusPorta  porta;       /* incipere NIHIL = mutum */
              Piscina* piscina;
  InternamentumChorda* intern;
                  i32  latitudo;    /* superficies montationis */
                  i32  altitudo;
               vacuum* figurae_ctx; /* privatum: tabula, incipit,
                                     * visus, psalmus aptatus */
} PsalteriumMontatio;


/* ==================================================
 * Auxilia pura (probationes, iussum vici)
 * ================================================== */

/* incipit psalmi ex textu toto plagulae (lineae '%' et vacuae
 * praetereuntur): versus primus usque ad '*' aut '†' primum, ',' ':'
 * ';' et spatia extrema dempta; "1. " initio omissum. Vacua si nullus
 * versus. */
chorda
psalterium_incipit (
      chorda  textus,
     Piscina* piscina);

/* terminationes circuli (D8) in tabula */
i32
psalterium_signa_numerus (
    constans PsalmodiaTabula* tabula);

/* signum terminationis index (ab 0) circuli: modus + " " +
 * differentia ("8 G"), aut modus solus si differentia vacua
 * ("peregrinus"); vacua extra fines */
chorda
psalterium_signum (
    constans PsalmodiaTabula* tabula,
                         i32  index,
                     Piscina* piscina);

/* index signi in circulo; -1 si ignotum aut extra circulum */
s32
psalterium_signi_index (
    constans PsalmodiaTabula* tabula,
                      chorda  signum);

/* argumentum $psalterium ("", "109", "109,8 G"; spatia circa partes
 * dempta) -> *psalmus (-1 = index) et *signum (vacuum = non datum,
 * in piscina scheda). FALSUM: causa (si non NIHIL) vitium scribit
 * (ARGUMENTUM supra). Tabula tonorum infixa in scheda legitur. */
b32
psalterium_argumentum_legere (
                chorda  argumentum,
               Piscina* scheda,
                   s32* psalmus,
                chorda* signum,
     ChordaAedificator* causa);


/* ==================================================
 * Latus
 * ================================================== */

/* montare in ramo <psalterium id> (manifestum infixum) cum argumento
 * (NIHIL aut "" = index aut status servatus); porta NIHIL = muta
 * (copiatur); systemata NIHIL licet */
b32
psalterium_montare (
          PsalteriumMontatio* m,
                     Piscina* piscina,
         InternamentumChorda* intern,
          InsulaRepositorium* repo,
          constans character* id,
          constans character* argumentum,
                         i32  latitudo,
                         i32  altitudo,
     constans GabcVisusPorta* porta,
             StatusSystemata* systemata);

/* compositor (dispensator; facies vici); ctx = PsalteriumMontatio* */
Componens*
psalterium_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx);

/* argumentum novum (vicus_acervo_mutare): ARGUMENTUM supra; FALSUM et
 * nihil mutatum si malum */
b32
psalterium_argumentum_ponere (
     PsalteriumMontatio* m,
     constans character* argumentum);

/* facies.semitam_secuta: post semitam a vico applicatam (initium,
 * historia) psalmum ex campis iterum aptat;
 * ctx = PsalteriumMontatio* */
b32
psalterium_semitam_secuta (
    vacuum* ctx);

/* pulsus (facies.pulsare involvit): sonus visus; VERUM si pingendum */
b32
psalterium_pulsare (
    vacuum* ctx);

/* facies.mora: dum sonat (gabc_visus_mora), aliter -1 */
Mora
psalterium_mora (
    vacuum* ctx);

/* facies.locus_titulus: "Psalterium" aut "Psalmus 109" */
chorda
psalterium_titulus (
     vacuum* ctx,
    Piscina* piscina);

#endif /* PSALTERIUM_H */

/* pictor_actiones.h - tractatores pictoris (P3: penicillus.ictus,
 * instrumentum.eligere)
 *
 * <tractator/>: nullum I/O; scripturae per portas solas (insulae:
 * mutare_*, documentum: pictor_documentum_actum - porta documenti).
 * Una actio per INTENTIONEM, phasis ex genere eventus (spec §5.2).
 */

#ifndef PICTOR_ACTIONES_H
#define PICTOR_ACTIONES_H

/* <aedilis corpus="lib/pictor_actiones.c"/> */

#include "latina.h"
#include "actio.h"
#include "pictor_documentum.h"
#include "xar.h"
#include "imago.h"

nomen structura {
    PictorDocumentum* doc;
         InsulaRamus  ramus;   /* R3: status pictoris; repo NIHIL =
                                * radix repositorii tractatori dati -
                                * structuram TOTAM nulla (memset) */
                 Xar* tempora; /* aspergillum: tempus (ms) cuiusque
                                * puncti pendentis; NIHIL donec
                                * primum */
                 s64 semen;   /* aspergillum: semen ictus currentis */
    /* L5: porta imaginis (Cmd+V) - NIHIL = nulla; applicationes
     * clipboard_capere_imaginem ponunt, probationes fictam */
    ImagoFructus (*imago_capere)(vacuum* ctx, Piscina* piscina);
          vacuum* imago_ctx;
} PictorActiones;

vacuum
pictor_actiones_registrare (
    ActioRegistrum* reg,
    PictorActiones* ctx);

/* <tractator/> */
b32
pictor_penicillus_ictus (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx);

/* <tractator/> aspergillum (MacPaint): ut penicillus capit et puncta
 * colligit; actum cum semine et tempore cuiusque puncti scribit
 * (punctum solutionis quoque: mora ante solutionem guttas addit) */
b32
pictor_aspergillum_ictus (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx);

/* <tractator/> strata (pictor-strata L3): ictus in palette stratorum -
 * 'stratum.<id>' eligit (ephemera stratum_activum), 'oculus.<id>'
 * visibilitatem in actis mutat, 'strata.novum' stratum novum supra
 * currens (fit currens), 'strata.deletum' currens delet (inferius fit
 * currens; ultimum numquam). Dominus 'stratum_activum'. Palette
 * aperta manet. */
b32
pictor_strata_agere (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx);

/* <tractator/> linea (Franus): ictus primus initium figit (captura
 * manet); motus sine botone praevisionem ad indicatorem movet; ictus
 * secundus segmentum scribit - cum Shift punctum eius initium novum
 * (series), aliter finitur. Esc aut ictus extra tabulam pendentem
 * abicit. Segmentum = ictus unus (revocatio per segmentum). */
b32
pictor_linea_ictus (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx);

/* <tractator/> spongia: ut penicillus capit et puncta colligit; actum
 * sine colore scribit (documentum colore fundi pingit) */
b32
pictor_spongia_ictus (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx);

/* <tractator/> quadratum lineae status ictum: palettam suam aperit
 * (ephemera 'palette' = instrumentum / color_primus / color_secundus
 * / exemplar / magnitudo);
 * eadem iterum: claudit */
b32
pictor_palettam_aperire (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx);

/* <tractator/> optio coloris ictum ('optio.color_primus.<n>',
 * 'optio.color_secundus.<n>'; n = -1 nullus): colorem ponit, palettam
 * claudit. Registratur ut 'color_primus.ponere' et
 * 'color_secundus.ponere' (domini harum attributorum) */
b32
pictor_colorem_ponere (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx);

/* <tractator/> optio exemplaris ictum ('optio.exemplar.<n>', n in
 * [0, EXEMPLAR_NUMERUS)): exemplar ponit, palettam claudit. Dominus
 * attributi 'exemplar' */
b32
pictor_exemplar_ponere (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx);

/* <tractator/> optio magnitudinis ictum ('optio.magnitudo.<n>'):
 * magnitudinem instrumenti currentis ponit - penicillus diametrum
 * (I II III IV VI VIII XII XVI XXXII LXIV), aspergillum multiplicem
 * radii (I II IV VIII XVI), linea latitudinem (I II IV VIII XVI);
 * spongia nihil - palettam claudit. Dominus
 * attributorum 'magnitudo_penicilli' et 'magnitudo_aspergilli' */
b32
pictor_magnitudinem_ponere (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx);

/* <tractator/> 'p' penicillus, 'a' aspergillum, 'e' spongia, 'l'
 * linea; Cmd+V: imago e porta in stratum novum supra currens (fit
 * currens); Cmd+Z revocat, Cmd+Shift+Z reficit (linea pendens:
 * abicitur solum); ictus
 * in optionem instrumenti ('optio.instrumentum.<nomen>') idem ponit et
 * palettam claudit; Esc palettam apertam claudit */
b32
pictor_instrumentum_eligere (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx);

#endif /* PICTOR_ACTIONES_H */

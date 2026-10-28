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
#include "claviatura.h"

nomen structura {
    PictorDocumentum* doc;
         InsulaRamus  ramus;   /* R3: status pictoris; repo NIHIL =
                                * radix repositorii tractatori dati -
                                * structuram TOTAM nulla (memset) */
                 Xar* tempora; /* aspergillum: tempus (ms) cuiusque
                                * puncti pendentis; NIHIL donec
                                * primum */
                 s64 semen;   /* aspergillum: semen ictus currentis */
             Punctum rasura;  /* glutinatio pendens: punctum ultimum
                               * tractus radentis (coordinatae
                               * tabulae) */
    /* L5: porta imaginis (Cmd+V) - NIHIL = nulla; applicationes
     * clipboard_capere_imaginem ponunt, probationes fictam */
    ImagoFructus (*imago_capere)(vacuum* ctx, Piscina* piscina);
          vacuum* imago_ctx;
    /* testa-ios C1b: electio imaginis (servitium hospitis, non
     * obstruens - imago POSTEA ut EVENTUS_DEPOSITIO venit); NIHIL =
     * nulla */
             b32 (*imaginem_eligere)(vacuum* ctx);
          vacuum* imaginem_eligere_ctx;
    /* pinacotheca T4: picturas mutare sine pictor_applicatio.h (id
     * hoc caput includit) - pictor_montare implet (PictorMontatio* in
     * 'montatio'); NIHIL (probationes sine montatione) = res menu
     * nihil agunt */
          vacuum* montatio;
             b32 (*picturam_condere)(vacuum* montatio,
                                     constans character* spatium);
             b32 (*picturam_ponere)(vacuum* montatio,
                                    constans character* spatium);
    /* pinacotheca T5: apertio eam parat (scheda, picturae) */
    PictorPinacotheca* pinacotheca;
    /* communio-plan-5 T6: Shared pictures et spectator - compositio
     * (structura PictorCompositio, pictor_componentia.h: condiciones
     * menu) legitur; exemplum per montationem (Edit a copy); origo
     * tractus imaginis (coordinatae locales) */
       PictorCommunia* communia;
    constans structura PictorCompositio* compositio;
             b32 (*exemplum_facere)(vacuum* montatio, chorda replica,
                                    constans character* spatium);
             Punctum origo_tractus;
    /* communio-plan-5 T7: inscriptio per hospitem (pictor_communio
     * ligat; NIHIL = nulla) et claviatura systematis (testa) */
             b32 (*inscriptionem_incipere)(vacuum* ctx,
                 chorda epistula);
             b32 (*codicem_dare)(vacuum* ctx, chorda codex);
          vacuum (*inscriptionem_abicere)(vacuum* ctx);
                vacuum* inscriptio_ctx;
TestaClaviaturamPonere  claviaturam_ponere;
                vacuum* claviatura_ctx;
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

/* <tractator/> glutinatio pendens: 'glutinatio.A' / 'glutinatio.B'
 * modum eligit, 'glutinatio.confirmare' stratum novum (fit currens),
 * 'glutinatio.abicere' abicit. Dominus 'glutinatio'. Tractus in
 * 'tabula' RADIT (Franus 2026-10-13): discus magnitudinis penicilli
 * per tractum modum alterum revelat, cum Optione reponit - in
 * documento extra acta, insula intacta */
b32
pictor_glutinatio_agere (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx);

/* <tractator/> strata (pictor-strata L3): ictus in palette stratorum -
 * 'stratum.<id>' eligit (campus stratum_activum), 'oculus.<id>'
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

/* <tractator/> quadratum imaginis ictum (testa-ios C1b): servitium
 * hospitis imaginem_eligere SEMEL vocat (non obstruens - photographia
 * electa postea ut EVENTUS_DEPOSITIO venit). Servitium NIHIL ->
 * FALSUM, nihil. Registratur ut 'imago.eligere'. */
b32
pictor_imaginem_eligere (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx);

/* <tractator/> plan-5 T6 Shared pictures: pinax spectatorem aperit,
 * prior/posterior paginam, X claudit. 'communia.agere'. */
b32
pictor_communia_agere (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx);

/* <tractator/> plan-5 T6 spectator: tractus (circulariter), ‹ ›, Edit a
 * copy, X / ictus extra imaginem ad rete. 'spectator.agere'. */
b32
pictor_spectatorem_agere (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx);

/* plan-5 T7: salutatio prima (communio praesto, nondum visa, non
 * inscriptus) aut inscriptio pendens -> palette "salutatio" /
 * "inscriptio". VERUM si posita. */
b32
pictor_salutationem_ponere_in (
    InsulaRepositorium* repo,
        PictorActiones* pa);

/* <tractator/> plan-5 T7 salutatio: Sign in… / Draw without signing in.
 * 'salutatio.agere'. */
b32
pictor_salutationem_agere (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx);

/* <tractator/> plan-5 T7 inscriptio: Send, Cancel, OK, campus.
 * 'inscriptio.agere'. */
b32
pictor_inscriptionem_agere (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx);

/* <tractator/> res menu ⋯ ictum ('elenchus.<k>', pinacotheca D2;
 * tabula pictor_elenchum_parare): "Open picture…" palettam in
 * "pinacotheca" mutat; "New picture" picturam novam "pictura-<n>" (n
 * = I + maximum exstans) condit et ostendit, menu claudit; "Import
 * image…" servitium imaginem_eligere vocat, menu claudit.
 * Registratur ut 'elenchus.agere'. */
b32
pictor_elenchum_agere (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx);

/* <tractator/> pinacotheca (spec D3, D7): 'pinax.<k>' picturam
 * tituli ("pinax:...:<spatium>") aperit et claudit;
 * 'pinacotheca.prior' / '.posterior' paginam mutant (intra limites);
 * 'pinacotheca.claudere' claudit, nihil aliud. Dominus
 * 'pagina_pinacothecae'. Registratur ut 'pinacotheca.agere'. */
b32
pictor_pinacothecam_agere (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx);

/* <tractator/> quadratum lineae status ictum: palettam suam aperit
 * (campus 'palette' = instrumentum / color_primus / color_secundus
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

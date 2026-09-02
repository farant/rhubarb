/* apparatus.h - clausura inclusionum et PORTA APPARATUS, communis
 *
 * CUR COMMUNIS (T7, 2026-08-22): porta apparatus BIS incendit, duabus
 * causis diversis, et utroque numerum peperit qui datum simulabat
 * (spec arbor-parsura par. 6.5). Porta talis exemplar SUUM in
 * probatione altera habere non debet: tertia occasio erraret
 * INDEPENDENTER, et duae portae eiusdem rei divergere possunt ubi una
 * non potest.
 *
 * Ergo hic unum exemplar vivit, et probationes ambae - circuitus
 * octetorum (probatio_silva_arbor_plagula) et iudicium canonis
 * (probatio_silva_canon_corpus) - id vocant.
 *
 * QUID PORTA CUSTODIAT: codex latinizatus clausuram inclusionum
 * POSCIT. Sine ea latina.h numquam videtur, 'si' identificator MANET,
 * arbor circuit OCTETIM EXACTA - et numerus purus NIHIL de codice
 * latinizato mensurat. Numerus confidenter falsus peior est quam
 * nullus.
 */

#ifndef SILVA_PROBATIONES_APPARATUS_H
#define SILVA_PROBATIONES_APPARATUS_H

#include "latina.h"
#include "piscina.h"
#include "silva_parsare.h"

#define CLAUSURA_MAXIMA  96
#define VIA_MAXIMA       512
#define CAUSAE_MAXIMAE   32

/* Causa cum numero suo - census loquitur causis, non summa sola
 * (summa immota manere potest dum causae subter omnino mutantur) */
nomen structura {
    constans character* causa;
                   i32  numerus;
} CausaNumerata;

/* Clausura inclusionum ex 'bin/aedilis fons.c --partes' (ordines
 * C = capita domestica). Silva plagulas ipsa numquam aperit -
 * consulto, substratum purum est - ergo harnesium eas PRAEBERE
 * debet. */
nomen structura {
    character series[CLAUSURA_MAXIMA][VIA_MAXIMA];
          i32 numerus;
          b32 latina_inest;   /* latina.h in clausura = latinizata */
          b32 truncata;       /* CLAUSURA_MAXIMA transgressa - CLAMANDUM */
} Clausura;

vacuum
apparatus_causam_notare (
      CausaNumerata* series,
                i32* numerus,
 constans character* causa);

b32
apparatus_desinit_in (
    constans character* via,
    constans character* cauda);

b32
apparatus_est_c (
    constans character* titulus);

b32
apparatus_est_c_vel_h (
    constans character* titulus);

i8*
apparatus_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura_exitus);

vacuum
apparatus_clausuram_petere (
    constans character* radix,
    constans character* via_relativa,
              Clausura* clausura);

/* Clausurae CORPORIS totius uno cursu aedilis (--corpus dir --partes):
 * extractor memor capita semel parsat trans scopos - per plagulam
 * popen CLVI vicibus XVI s per probationem consumebat (2026-09-02).
 * viae = character[VIA_MAXIMA] valore, clausurae = Clausura valore,
 * indice pari. Plagula in corpore absens = clausura VACUA (numerus 0,
 * latina_inest FALSUM) et FALSUM - porta apparatus eam clamat, nihil
 * tacite per plagulam suppletur. */
nomen structura {
    Xar* viae;
    Xar* clausurae;
} ClausuraeCorporis;

ClausuraeCorporis*
apparatus_clausuras_petere (
    constans character* radix,
    constans character* directorium,
               Piscina* piscina);

b32
apparatus_clausuram_ex_corpore (
    constans ClausuraeCorporis* corpus,
            constans character* via_relativa,
                      Clausura* clausura);

/* Plagulam parsare, clausura praebita si petitur.
 *
 * 'praebere' est SUTURA CALIBRATIONIS, consulto permanens: FALSUM
 * clausuram silvae NEGAT dum scientiam nostram de ea servat, ergo
 * porta apparatus incendere DEBET. Polaritas tuta est - positio
 * falsa portam CLAMARE facit, non tacere.
 *
 * fons_latina_exitus: index fontis latina.h, aut -1 si non praebita. */
SilvaParsura*
apparatus_parsare (
                Piscina* opus,
     constans character* via,
            constans i8* fons,
                    i32  mensura,
     constans character* radix,
      constans Clausura* clausura,
                    b32  praebere,
                    s32* fons_latina_exitus);

/* PORTA IPSA: an apparatus SANUS sit (aut non applicabilis).
 *
 * Numerat expansiones quarum DEFINITIO in latina.h iacet - non
 * expansiones omnes. Distinctio MENSURATA est: plagula quaeque suum
 * '#define' ferens (e.g. XAR_FACTOR_DUPLICANDI in lib/xar.c) lexemata
 * expansa parit SINE ulla clausura praebita, et LIX ex CLIV plagulis
 * portam naive sic transierunt dum latina.h numquam visa est.
 *
 * Reddit VERUM si sana (aut clausura latina.h non fert); expansa_exitus
 * numerum accipit (NIHIL licet). */
b32
apparatus_sanus (
    constans SilvaParsura* parsura,
        constans Clausura* clausura,
                      s32  fons_latina,
                      i32* expansa_exitus);

#endif /* SILVA_PROBATIONES_APPARATUS_H */

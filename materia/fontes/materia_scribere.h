/* materia_scribere.h - Emissio arborum ad octetos
 *
 * Ambulatio PURE STRUCTURALIS: loci ordine layout, numquam
 * positione dependens (nulla ordinatio per byte_offset, nulla
 * lectio vicinorum). Haec puritas subarbores intactas post
 * transformationes octetim exactas servat: subarbor mota octetos
 * suos ubicumque emittit.
 *
 * ============================================================
 * DUAE CORRECTIONES AD SPEC, UTRAQUE MENSURATA 2026-08-27
 * ============================================================
 *
 * I. AMBIGUUS OPTIONALIS EST. Spec par. 2.1 dicit quinque genera
 * structuralia obligatoria in silva_generare.c:1315 (instrumentario
 * LR) vivere, ergo 'HG2 evaporat' quia frontes manu scriptae
 * generatorem LR numquam vocant. VERUM DE GENERATORE, FALSUM DE
 * SCRIPTORE: silva_scribere.c:625 _scriptor_parare
 * _formam_ambigui_invenire REDDIT, et utraque via publica
 * (silva_scribere_valorem, _nodum) FALSUM ut fatale tractat -
 * 'tabularium sine forma ambigui'. Ergo registrum SINE genere
 * 'ambiguus' scriptorem silvae frangit, in modulo NUCLEI.
 *
 * Hic ambiguus OPTIONALIS est: absens = lingua ambiguitatem non
 * habet (descensus recursivus numquam eam parit), non tabulae
 * corruptae. Praesens sed CORRUPTUS (locis carens) adhuc RECUSAT -
 * distinctio inter 'non habeo' et 'fractum habeo' servanda est.
 *
 * II. REINSERENDA AB OMNI INGRESSU RAGGIUNGIBILIA (MG1 editio I).
 * silva_scribere.c:613 _scriptor_parare 'reinserenda = NIHIL'
 * DURATUM tenet, et solum silva_scribere_fontem ea implet; vias
 * subarborum praeterea fons_index = -I tradunt (:686), quod
 * praedicatum omissionis (:299) numquam ardere sinit. Ergo
 * mechanismus VIVUS sed INACCESSIBILIS erat ex via quam CSS et
 * HTML adhibent. Hic utrumque consilii campus est.
 */

#ifndef MATERIA_SCRIBERE_H
#define MATERIA_SCRIBERE_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "xar.h"
#include "materia_token.h"
#include "materia_nodus.h"
#include "materia_registrum.h"


/* ==================================================
 * Fructus
 * ================================================== */

nomen structura {
                      b32  successus;
                   chorda  textus;   /* octeti emissi; vacua in fractura */
       constans character* causa;    /* diagnostica statica; NIHIL si bene */
    constans MateriaNodus* sedes;    /* nodus fracturae; NIHIL licet */
} MateriaScriptura;


/* ==================================================
 * Reinserendum - lamina octetorum EXTRA ordinem arboris
 *
 * Generale (MG1): 'octeti quos nulla lexema arboris nominat, ad
 * offset datum fundendi'. C89 lineas directivas et ramos non
 * sumptos hac via reddit; alia lingua alia.
 * ================================================== */

nomen structura {
    s32  offset;
    Xar* lamina;    /* Xar de MateriaToken* */
} MateriaReinserendum;


/* ==================================================
 * Consilium scripturae
 * ================================================== */

nomen structura {
    constans MateriaRegistrumCoctum* tabularium;

    /* NIHIL = lingua derivationem non habet; omne lexema se ipsum
     * emittit. Recte pro CSS et HTML. */
    constans MateriaOrigoUncus* origo;

    /* Octeti unius lexematis. NIHIL = valor verbatim (+ trivia),
     * quod omni linguae sine laminis intra lexema rectum est.
     * C89 hic scissuras reinserit. */
    b32 (*valorem_emittere)(vacuum*, ChordaAedificator*,
                            constans MateriaToken*);
    vacuum* valorem_datum;

    /* Xar de MateriaReinserendum, per offset ORDINATUS. NIHIL
     * licet. Campus consilii est, non hardcoded NIHIL - vide
     * correctionem II supra. */
    Xar* reinserenda;

    /* Cuius plagulae octeti petuntur; -I = quaelibet */
    s32 fons_index;
} MateriaScripturaConsilium;

/* Consilium minimum: tabularium solum, cetera NIHIL/-I. */
vacuum
materia_scriptura_consilium_nudum (
          MateriaScripturaConsilium* consilium,
    constans MateriaRegistrumCoctum* tabularium);


/* ==================================================
 * Ingressus
 * ================================================== */

MateriaScriptura
materia_scribere_valorem (
                               Piscina* piscina,
                          MateriaValor  valor,
    constans MateriaScripturaConsilium* consilium);

MateriaScriptura
materia_scribere_nodum (
                               Piscina* piscina,
                 constans MateriaNodus* nodus,
    constans MateriaScripturaConsilium* consilium);

/* Comparator ad reinserenda ordinanda (xar_ordinare) - vocans ea
 * componit, ergo instrumentum ordinandi ei debetur. */
s32
materia_reinserenda_comparare (
    constans vacuum* a,
    constans vacuum* b);

#endif /* MATERIA_SCRIBERE_H */

/* silva_quaestiones.h - bibliotheca selectorum nominatorum
 * (QUAESTIONES NOMINATAE - parcum 01KXPV9FPK; INTENTIO in
 * silva/phase-log.md 2026-07-31)
 *
 * Plagula commissa silva/quaestiones.stml selectores NOMINATOS et
 * PARAMETRIZATOS fert; hic modulus eam legit, validat, et
 * selectores completos ad machinam quaestionis (silva_quaestio)
 * tradit. Lectio disci apud consumptores manet - fontes chordas
 * accipiunt.
 *
 * FORMA PLAGULAE:
 *   <quaestiones>
 *     <quaestio titulus="..." gradus="oculi|invarians">
 *       <selector>...</selector>
 *       <parametrum titulus="..."/>    (nulla aut plura)
 *       <causa>...</causa>
 *     </quaestio>
 *   </quaestiones>
 *
 * SUBSTITUTIO ($titulus in textu selectoris):
 *   - intra valores attributorum citatos: [locus="$param"]
 *   - argumentum pseudo INTEGRUM:        :vocat($param)
 * Extra hos contextus '$' = captura machinae ($nomen post tagum)
 * et numquam tangitur - CONTEXTUS discriminat, non nomen, ergo
 * collisio structuraliter impossibilis.
 *
 * VALIDATIO (in legere; plagula tota valida aut NIHIL + culpa
 * nominata): selector compilatur (specimen "x" pro parametris);
 * parametra declarata <-> $loci selectoris UTRIMQUE; causa et
 * gradus obligatoria; gradus invarians = zero parametra; tituli
 * unici.
 *
 * GRADUS INVARIANS: selector qui ZERO congruentias reddere debet -
 * bibliotheca fit custodia structuralis sine codice C novo (v0
 * OCULI PRIMUM: relatio sola, nulli unco inserta; promotio per
 * invariantem postquam tenuit).
 */

#ifndef SILVA_QUAESTIONES_H
#define SILVA_QUAESTIONES_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "silva_registrum.h"
#include "silva_quaestio.h"

/* ==================================================
 * Typi
 * ================================================== */

nomen enumeratio {
    SILVA_QUAESTIONES_OCULI = 0,  /* relatio - numquam porta */
    SILVA_QUAESTIONES_INVARIANS   /* zero congruentiae = TENET */
} SilvaQuaestionesGradus;

/* Quaestio nominata: unum entrium bibliothecae (lectum, validatum) */
nomen structura {
    chorda titulus;
    s32    gradus;      /* SilvaQuaestionesGradus */
    chorda selector;    /* textus crudus cum $parametris, trimmatus */
    Xar*   parametra;   /* chorda (valore) - tituli declarati */
    chorda causa;
} SilvaQuaestioNominata;

/* Argumentum pro parare: par titulus->valor */
nomen structura {
    chorda titulus;
    chorda valor;
} SilvaQuaestionesArgumentum;

/* Bibliotheca lecta. tabularium/registro in legere data hic
 * servantur - registro per compilationes posteriores (parare)
 * legitur, ergo bibliothecam supervivere debet. */
nomen structura {
    Xar*                                   nominatae; /* SilvaQuaestioNominata (valore) */
    constans SilvaRegistrumCoctum*         tabularium;
    constans SilvaQuaestioPseudoRegistrum* registro;
} SilvaQuaestiones;

/* ==================================================
 * API
 * ================================================== */

/* Bibliothecam ex chorda legere et TOTAM validare. NIHIL + *culpa_out
 * (chorda in piscinam formata, entrium nominans) in fractura
 * QUALIBET - plagula valida tota aut nihil. registro NIHIL =
 * pseudo-classes nativae solae. */
SilvaQuaestiones*
silva_quaestiones_legere (
    Piscina*                               piscina,
    constans SilvaRegistrumCoctum*         tabularium,
    constans SilvaQuaestioPseudoRegistrum* registro,
    chorda                                 fons,
    chorda*                                culpa_out);

/* Numerus quaestionum nominatarum */
i32
silva_quaestiones_numerus (constans SilvaQuaestiones* bibliotheca);

/* Quaestio nominata ad indicem; NIHIL = extra fines */
constans SilvaQuaestioNominata*
silva_quaestiones_ad_indicem (
    constans SilvaQuaestiones* bibliotheca,
    i32                        index);

/* Quaestio nominata per titulum; NIHIL = absens */
constans SilvaQuaestioNominata*
silva_quaestiones_invenire (
    constans SilvaQuaestiones* bibliotheca,
    constans character*        titulus);

/* Argumenta in selectorem texere et compilare. argumenta = series
 * plana (monstrator + numerus; NIHIL/0 = nulla) - series plana, non
 * Xar, ut hospites amalgamatis (quibus Xar legendus solum est) eam
 * aedificare possint. Fracturae nominatae: argumentum
 * absens/ignotum/iteratum, valor illicitus (characteres '"' '(' ')'
 * vetiti - textura selectoris integra manet), compilatio. Fructus
 * reusabilis trans arbores (contractus machinae). */
SilvaQuaestio*
silva_quaestiones_parare (
    Piscina*                              piscina,
    constans SilvaQuaestiones*            bibliotheca,
    constans SilvaQuaestioNominata*       nominata,
    constans SilvaQuaestionesArgumentum*  argumenta,
    i32                                   numerus_argumentorum,
    chorda*                               culpa_out);

#endif /* SILVA_QUAESTIONES_H */

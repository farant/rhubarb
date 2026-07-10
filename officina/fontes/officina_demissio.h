/* officina_demissio.h - Demissio: arbor typata -> medulla (M1b)
 *
 * UNICUM domicilium ubi semantica C89 explicita fit: conversiones
 * annotatae (M0b {naturalis, conversus} - contractus CONGELATUS)
 * operationes conversionum visibiles fiunt; arithmetica monstratorum
 * per formas scalatur; && / || / ternarius in fluxum linearizantur.
 * NULLA probatio typorum hic - demissio annotationibus OBOEDIT
 * (lamina exsecutionis futura iudicat); numquam ruit - inlowerabilia
 * -> sistere cum causa classificata (postura tree-sitter).
 *
 * Silva TRANS AMALGAMA SOLUM (silva.h - officina a silva pendet EX
 * ARCHITECTURA; dependentia externa unica documentata). Praesumit
 * sem POST-FLIP (typatio canonicae-relativa; fistula bis-analysans
 * VOCANTIS est - fusor percursum exscribit).
 *
 * NB sem NON-constans: forma pigra (formam_computare) et aestimator
 * tabulas per quaestionem mutant (INTENTIO const dicebat -
 * emendatio, vide worklog).
 */

#ifndef OFFICINA_DEMISSIO_H
#define OFFICINA_DEMISSIO_H

#include "officina_medulla.h"
#include "silva.h"

/* Modulus ex TU uno (conexio ad M2). titulus_moduli = via fontis
 * ("lib/chorda.c") - praefixum staticorum internorum ex eo. */
MedullaModulus* demissio_currere (Piscina* piscina,
    constans SilvaParsura* parsura, SilvaSemantica* sem,
    chorda titulus_moduli);

/* Distillatio linearum (M2a): origo -> (via, linea) DUM frons vivit
 * (parsura->expansio necessaria pro viis fontium; nulla via publica
 * post liberationem). Post hanc arbores liberari possunt - clavis
 * origo opaca manet. Consumptor: recordator volatus (M2b);
 * proto-indicium (M3). */
MedullaLineae* demissio_lineas_colligere (Piscina* piscina,
    constans MedullaModulus* modulus,
    constans SilvaParsura* parsura);

#endif /* OFFICINA_DEMISSIO_H */

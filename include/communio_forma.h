/* communio_forma.h - acta ut JSON pro protocollo communionis
 * (project-specs/communio-plan-2.md T2; spec C5)
 *
 * Locus UNUS ubi actum voluminis JSON fit et ex JSON redit: servus
 * (communio_servus) et cliens (communio_cliens) hunc solum adhibent.
 *
 * FORMA ACTUS: obiectum chordarum - hora, replica, machina, momentum,
 * genus, datum, et basis_hora, basis_replica, basis_machina (omissa si
 * actum basim non habet); seq_originis integer. hora et basis_hora
 * numeri decimales in chorda (horologia composita LXIV bitorum, ne
 * lector alius per duplicem fluitantem amittat); datum chorda ad
 * octetum servata. seq locale numquam transit (lector 0 ponit).
 *
 * VALIDATIO IN LIMINE RETIS (minus dilatum M4), utrimque:
 * replica = XXVI signa Crockford base32 maiuscula (sine I L O U);
 * hora > 0; seq_originis > 0; genus non vacuum; plagula-condita et
 * plagula-remota viam habent quae non sub "aliena/" cadit (spatium
 * manifesti alieni localiter reservatum).
 */

#ifndef COMMUNIO_FORMA_H
#define COMMUNIO_FORMA_H

/* <aedilis corpus="lib/communio_forma.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "json.h"
#include "volumen.h"

/* acta per petitionem (missio aut tractio) */
#define COMMUNIO_ACTA_MAXIMA CC

/* massa maxima communicabilis (recensio 5): HTTP_LECTOR_CORPUS_MAXIMUM
 * (http.h) - cliens massam maiorem trahere non potest, ergo servus eam
 * non accipit et cliens eam non mittit (error visibilis in apparatu
 * scribente, non silentium in omnibus aliis) */
#define COMMUNIO_MASSA_MAXIMA ((i32)XVI * (i32)M * (i32)M)

/* actum in obiectum JSON; NIHIL si memoria deficit */
JsonValor*
communio_actum_scribere (
    constans VolumenActum* actum,
                  Piscina* piscina);

/* obiectum JSON in actum (seq = 0); FALSUM et causa brevis si campus
 * deest aut malus est. Validationem non facit (communio_actum_validum).
 * chordae in memoriam obiecti JSON monstrant (eius piscina vivat). */
b32
communio_actum_legere (
       JsonValor* obiectum,
    VolumenActum* exitus,
          chorda* causa,
         Piscina* piscina);

/* replica: XXVI signa Crockford base32 maiuscula (sine I L O U) */
b32
communio_replica_valida (
    chorda replica);

/* leges limini (supra); FALSUM et causa si actum recusandum est */
b32
communio_actum_validum (
    constans VolumenActum* actum,
                   chorda* causa,
                  Piscina* piscina);

/* {"acta":[...]} ex Xar de VolumenActum */
chorda
communio_acta_scribere (
         Xar* acta,
     Piscina* piscina);

/* corpus {"acta":[...]} in Xar de VolumenActum: NIHIL (et causa) si
 * non JSON, sine "acta", plus quam COMMUNIO_ACTA_MAXIMA, aut actum
 * quodvis malum vel invalidum */
Xar*
communio_acta_legere (
      chorda  corpus,
      chorda* causa,
     Piscina* piscina);

/* massae quas actum nominat: quaeque series EXACTA LXIV characterum
 * hex minusculorum in datum (sigillum plagulae, <imago massa=...>) -
 * regula generalis sine forma applicationis; Xar de chorda (in datum
 * actus monstrant, non copiae). Par falsum massam extra mittit
 * tantum. */
Xar*
communio_massas_nominatas (
    constans VolumenActum* actum,
                  Piscina* piscina);

#endif /* COMMUNIO_FORMA_H */

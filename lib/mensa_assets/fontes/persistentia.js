/* persistentia.js - glutinum pontis: SOLUM hic internuntius
 * tangitur. Componenta 'mensa-actum' ebulliunt; nos ea ad
 * mensa_actum (latus C, lib/mensa.c) ferimus et in onere statum
 * plicatum plano damus (planum reddit - v2 tabulas nidificatas
 * novit).
 *
 * Sine ponte (internuntius abest): planum statu vacuo redditur -
 * componenta vivunt, nihil persistit. */

function mensaPersistentia(planum) {
  if (typeof internuntius === 'undefined') {
    planum.statumPonere({});
    return;
  }

  /* auscultator ANTE onerationem - nihil inter tempus perit */
  planum.addEventListener('mensa-actum', function (e) {
    internuntius.vocare('mensa_actum', e.detail)
      .catch(function (err) {
        console.log('mensa: actum periit: ' + err.message);
      });
  });

  /* praebitor imaginum (lex DI - planum pontem numquam nominat) */
  planum.imaginesPraebere({
    condere: function (b64) {
      return internuntius.vocare('mensa_imago_condere',
        { datum_b64: b64 })
        .then(function (r) { return r.sigillum; });
    },
    promere: function (sigillum) {
      return internuntius.vocare('mensa_imago_promere',
        { sigillum: sigillum })
        .then(function (r) { return r.datum_b64; });
    }
  });

  internuntius.vocare('mensa_status', {})
    .then(function (r) {
      planum.statumPonere(r.elementa || {});
    })
    .catch(function (err) {
      console.log('mensa: status non oneratus: ' + err.message);
      planum.statumPonere({});
    });
}

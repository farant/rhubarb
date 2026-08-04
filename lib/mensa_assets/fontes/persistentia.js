/* persistentia.js - glutinum pontis: SOLUM hic internuntius
 * tangitur. Componenta 'mensa-actum' ebulliunt; nos ea ad
 * mensa_actum (latus C, lib/mensa.c) ferimus et in onere
 * mensa_status plicatum applicamus.
 *
 * Usus (in pagina app):
 *   mensaPersistentia(document.querySelector('mensa-planum'));
 *
 * Sine ponte (internuntius abest): tacet - componenta vivunt,
 * nihil persistit. */

function mensaPersistentia(planum) {
  if (typeof internuntius === 'undefined') { return; }

  /* auscultator ANTE onerationem - nihil inter tempus perit */
  planum.addEventListener('mensa-actum', function(e) {
    internuntius.vocare('mensa_actum', e.detail)
      .catch(function(err) {
        console.log('mensa: actum periit: ' + err.message);
      });
  });

  internuntius.vocare('mensa_status', {})
    .then(function(r) {
      var id, datum, el;
      for (id in r.elementa) {
        datum = r.elementa[id];
        el = document.getElementById(id);
        if (!el && datum.genus_elementi === 'scidula') {
          el = planum.scidulamCreare(
            datum.x || 10, datum.y || 10, datum.textus || '',
            id, true);
        }
        if (!el) { continue; }
        if (el.ponePositum && typeof datum.x === 'number'
            && typeof datum.y === 'number') {
          el.ponePositum(datum.x, datum.y);
        }
        if (el.textumPonere && typeof datum.textus === 'string') {
          el.textumPonere(datum.textus);
        }
      }
    })
    .catch(function(err) {
      console.log('mensa: status non oneratus: ' + err.message);
    });
}

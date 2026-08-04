/* repositorium.js - <silex-repositorium>: speculum voluminis in
 * fenestra. Lectores puri soli (vcs_historia / vcs_plica /
 * vcs_plagula) - arborem laborantem numquam tangit, et iter
 * temporis GRATIS venit: conditio = punctum caudae, plica =
 * plicatura ad seq, ergo "conditionem aperire" = arborem illius
 * temporis vivam legere.
 *
 * App-proprium silicis (non pars pilae mensae). Vectis angularis
 * apparet solum si proiectum sub cwd invenitur. Overlay volvitur
 * INTRA columnas suas - lex "planum numquam volvitur" mensae
 * intacta manet (hoc non planum est). */

var SILEX_REPOSITORIUM_STILI =
  '.silex-repositorium-vectis {' +
  '  position: fixed; top: .7em; right: .9em; z-index: 1000000;' +
  '  background: var(--mensa-charta); color: var(--mensa-accentus);' +
  '  border: 1px solid var(--mensa-margo); border-radius: 999px;' +
  '  padding: .35em 1em; font: inherit; cursor: pointer;' +
  '  user-select: none; -webkit-user-select: none;' +
  '}' +
  '.silex-repositorium-vectis:hover {' +
  '  border-color: var(--mensa-accentus);' +
  '}' +
  'silex-repositorium {' +
  '  display: none; position: fixed; inset: 0; z-index: 1000001;' +
  '  background: var(--mensa-fundus); color: var(--mensa-textus);' +
  '  flex-direction: column; font: 13px/1.5 monospace;' +
  '}' +
  'silex-repositorium.apertum { display: flex; }' +
  'silex-repositorium .caput {' +
  '  display: flex; align-items: baseline; gap: 1em;' +
  '  padding: .8em 1.2em; border-bottom: 1px solid' +
  '  var(--mensa-margo);' +
  '  user-select: none; -webkit-user-select: none;' +
  '}' +
  'silex-repositorium .caput .titulus {' +
  '  color: var(--mensa-accentus); letter-spacing: .18em;' +
  '  text-transform: uppercase;' +
  '}' +
  'silex-repositorium .caput .proiectum {' +
  '  color: var(--mensa-textus-secundus); flex: 1;' +
  '  overflow: hidden; text-overflow: ellipsis;' +
  '  white-space: nowrap;' +
  '}' +
  'silex-repositorium .caput .claudere {' +
  '  background: none; border: none; color:' +
  '  var(--mensa-textus-secundus); font: inherit;' +
  '  cursor: pointer; font-size: 1.2em;' +
  '}' +
  'silex-repositorium .caput .claudere:hover {' +
  '  color: var(--mensa-accentus);' +
  '}' +
  'silex-repositorium .corpus {' +
  '  display: flex; flex: 1; min-height: 0;' +
  '}' +
  'silex-repositorium .columna {' +
  '  overflow-y: auto; border-right: 1px solid var(--mensa-margo);' +
  '  padding: .6em 0;' +
  '}' +
  'silex-repositorium .columna.conditiones { width: 24%; }' +
  'silex-repositorium .columna.plagulae { width: 26%; }' +
  'silex-repositorium .columna.contentum {' +
  '  flex: 1; border-right: none; padding: .9em 1.2em;' +
  '}' +
  'silex-repositorium .res {' +
  '  padding: .35em 1.1em; cursor: pointer;' +
  '  user-select: none; -webkit-user-select: none;' +
  '}' +
  'silex-repositorium .res:hover { background: var(--mensa-charta); }' +
  'silex-repositorium .res.electa {' +
  '  background: var(--mensa-charta);' +
  '  border-left: 3px solid var(--mensa-accentus);' +
  '  padding-left: calc(1.1em - 3px);' +
  '}' +
  'silex-repositorium .res .linea-prima { display: block; }' +
  'silex-repositorium .res .linea-secunda {' +
  '  display: block; color: var(--mensa-textus-secundus);' +
  '  font-size: .85em;' +
  '}' +
  'silex-repositorium pre {' +
  '  margin: 0; white-space: pre; font: inherit;' +
  '  user-select: text; -webkit-user-select: text;' +
  '}';

class SilexRepositorium extends HTMLElement {
  connectedCallback() {
    var ipse;
    if (this._paratum) { return; }
    this._paratum = true;
    ipse = this;
    this._seq = 0;
    this._plagula = null;

    this._caput = document.createElement('div');
    this._caput.className = 'caput';
    this._caput.innerHTML =
      '<span class="titulus">repositorium</span>' +
      '<span class="proiectum"></span>' +
      '<button class="claudere" title="claudere (Esc)">&times;' +
      '</button>';
    this.appendChild(this._caput);
    this._caput.querySelector('.claudere')
      .addEventListener('click', function () { ipse.claudere(); });

    this._corpus = document.createElement('div');
    this._corpus.className = 'corpus';
    this._conditiones = document.createElement('div');
    this._conditiones.className = 'columna conditiones';
    this._plagulae = document.createElement('div');
    this._plagulae.className = 'columna plagulae';
    this._contentum = document.createElement('div');
    this._contentum.className = 'columna contentum';
    this._corpus.appendChild(this._conditiones);
    this._corpus.appendChild(this._plagulae);
    this._corpus.appendChild(this._contentum);
    this.appendChild(this._corpus);

    document.addEventListener('keydown', function (e) {
      if (e.key === 'Escape' && ipse.classList.contains('apertum')) {
        ipse.claudere();
      }
    });
  }

  aperire() {
    this.classList.add('apertum');
    this._conditionesOnerare();
  }

  claudere() {
    this.classList.remove('apertum');
  }

  _conditionesOnerare() {
    var ipse = this;

    internuntius.vocare('vcs_historia', {})
      .then(function (r) {
        var index, ordo;

        ipse._caput.querySelector('.proiectum').textContent =
          r.proiectum;
        ipse._conditiones.innerHTML = '';
        /* arbor praesens primum, deinde conditiones recentissimae */
        ipse._conditionemAddere(0, 'arbor praesens', '(plica ' +
          'manifesti hodierna)');
        ordo = r.ordo.slice().reverse();
        for (index = 0; index < ordo.length; index = index + 1) {
          ipse._conditionemAddere(ordo[index].seq,
            '[' + ordo[index].seq + '] ' + ordo[index].nuntius,
            ordo[index].momentum + ' · '
              + ordo[index].tactae + ' plagulae');
        }
        ipse._conditionemEligere(0);
      })
      .catch(function (e) {
        ipse._conditiones.innerHTML = '';
        ipse._contentum.textContent = 'CULPA: ' + e.message;
      });
  }

  _conditionemAddere(seq, prima, secunda) {
    var ipse = this;
    var res = document.createElement('div');
    var l1 = document.createElement('span');
    var l2 = document.createElement('span');

    res.className = 'res';
    res.dataset.seq = String(seq);
    l1.className = 'linea-prima';
    l1.textContent = prima;
    l2.className = 'linea-secunda';
    l2.textContent = secunda;
    res.appendChild(l1);
    res.appendChild(l2);
    res.addEventListener('click', function () {
      ipse._conditionemEligere(seq);
    });
    this._conditiones.appendChild(res);
  }

  _eligere(columna, praedicatum) {
    var index;
    var res = columna.querySelectorAll('.res');

    for (index = 0; index < res.length; index = index + 1) {
      res[index].classList.toggle('electa', praedicatum(res[index]));
    }
  }

  _conditionemEligere(seq) {
    var ipse = this;

    this._seq = seq;
    this._eligere(this._conditiones, function (r) {
      return r.dataset.seq === String(seq);
    });
    this._contentum.textContent = '';
    internuntius.vocare('vcs_plica', { seq: seq })
      .then(function (r) {
        var index;

        ipse._plagulae.innerHTML = '';
        for (index = 0; index < r.ordo.length; index = index + 1) {
          ipse._plagulamAddere(r.ordo[index].via,
            r.ordo[index].origo);
        }
        /* plagula prior electa retinetur si adhuc exsistit */
        if (ipse._plagula) {
          var manet = false;

          for (index = 0; index < r.ordo.length;
              index = index + 1) {
            if (r.ordo[index].via === ipse._plagula) {
              manet = true;
              break;
            }
          }
          if (manet) { ipse._plagulamEligere(ipse._plagula); }
          else { ipse._plagula = null; }
        }
      })
      .catch(function (e) {
        ipse._plagulae.innerHTML = '';
        ipse._contentum.textContent = 'CULPA: ' + e.message;
      });
  }

  _plagulamAddere(via, origo) {
    var ipse = this;
    var res = document.createElement('div');
    var l1 = document.createElement('span');
    var l2 = document.createElement('span');

    res.className = 'res';
    res.dataset.via = via;
    l1.className = 'linea-prima';
    l1.textContent = via;
    l2.className = 'linea-secunda';
    l2.textContent = origo;
    res.appendChild(l1);
    res.appendChild(l2);
    res.addEventListener('click', function () {
      ipse._plagulamEligere(via);
    });
    this._plagulae.appendChild(res);
  }

  _plagulamEligere(via) {
    var ipse = this;

    this._plagula = via;
    this._eligere(this._plagulae, function (r) {
      return r.dataset.via === via;
    });
    internuntius.vocare('vcs_plagula',
      { seq: this._seq, plagula: via })
      .then(function (r) {
        var pre = document.createElement('pre');

        pre.textContent = r.contentum;
        ipse._contentum.innerHTML = '';
        ipse._contentum.appendChild(pre);
      })
      .catch(function (e) {
        ipse._contentum.textContent = 'CULPA: ' + e.message;
      });
  }
}

if (typeof customElements !== 'undefined') {
  customElements.define('silex-repositorium', SilexRepositorium);
}

/* initium: stilos inicere; vectis + overlay solum si proiectum
 * sub cwd invenitur (vcs_historia respondet) */
function silexRepositorium() {
  var stili, vectis, overlay;

  if (typeof internuntius === 'undefined') { return; }
  if (!document.getElementById('silex-repositorium-stili')) {
    stili = document.createElement('style');
    stili.id = 'silex-repositorium-stili';
    stili.textContent = SILEX_REPOSITORIUM_STILI;
    document.head.appendChild(stili);
  }
  internuntius.vocare('vcs_historia', {})
    .then(function () {
      overlay = document.createElement('silex-repositorium');
      document.body.appendChild(overlay);
      vectis = document.createElement('button');
      vectis.className = 'silex-repositorium-vectis';
      vectis.textContent = 'repositorium';
      vectis.addEventListener('click', function () {
        overlay.aperire();
      });
      document.body.appendChild(vectis);
    })
    .catch(function () {
      /* nullum proiectum sub cwd - vectis non apparet */
    });
}

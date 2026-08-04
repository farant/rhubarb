/* GLOMERATUM a tools/glomerare.sh - NOLI MANU EDERE;
   fontes veri infra nominantur. */

/* ================ ex lib/mensa_assets/fontes/thema.js ================ */
/* thema.js - <mensa-thema>: thema ut componentum.
 *
 * Stilos omnium componentorum mensae SEMEL in caput inicit
 * (proprietates CSS consuetae = superficies thematis); attributa
 * elementi proprietates superscribunt in subtree suo:
 *   <mensa-thema fundus="#101216" accentus="#e8a04c">
 * fit --mensa-fundus / --mensa-accentus in hoc elemento.
 * SINE shadow DOM consulto: thema globale, inspector verax,
 * harness probationis simplex. */

var MENSA_STILI =
  'mensa-thema {' +
  '  display: block; width: 100%; height: 100%;' +
  '  --mensa-fundus: #16181d;' +
  '  --mensa-charta: #101216;' +
  '  --mensa-margo: #2a2d35;' +
  '  --mensa-textus: #d8d4cc;' +
  '  --mensa-textus-secundus: #8a8578;' +
  '  --mensa-accentus: #e8a04c;' +
  '  --mensa-scidula-fundus: #3a3420;' +
  '  --mensa-scidula-margo: #5c5232;' +
  '  --mensa-umbra: 0 4px 16px rgba(0,0,0,.45);' +
  '  background: var(--mensa-fundus);' +
  '  color: var(--mensa-textus);' +
  '  font: 14px/1.5 monospace;' +
  '}' +
  'mensa-planum {' +
  '  display: block; position: relative;' +
  '  width: 100%; height: 100%;' +
  '  overflow: hidden;' +   /* LEX: planum numquam volvitur */
  '}' +
  'mensa-scida {' +
  '  display: block; position: absolute;' +
  '  background: var(--mensa-charta);' +
  '  border: 1px solid var(--mensa-margo);' +
  '  border-radius: 6px; padding: .9em 1.1em;' +
  '  box-shadow: var(--mensa-umbra);' +
  '  cursor: grab; user-select: none;' +
  '  -webkit-user-select: none;' +
  '}' +
  'mensa-scida.tractans { cursor: grabbing; opacity: .92; }' +
  'mensa-scida h2 {' +
  '  color: var(--mensa-textus-secundus); font-size: .85em;' +
  '  margin: 0 0 .5em 0; text-transform: uppercase;' +
  '  letter-spacing: .1em;' +
  '}' +
  'mensa-scidula {' +
  '  background: var(--mensa-scidula-fundus);' +
  '  border-color: var(--mensa-scidula-margo);' +
  '  min-width: 10em; min-height: 5em; padding: .5em .7em;' +
  '}' +
  'mensa-scidula .corpus {' +
  '  outline: none; min-height: 3.5em; cursor: text;' +
  '  user-select: text; -webkit-user-select: text;' +
  '  white-space: pre-wrap;' +
  '}' +
  'mensa-scidula .delere {' +
  '  position: absolute; top: .2em; right: .35em;' +
  '  background: none; border: none; cursor: pointer;' +
  '  color: var(--mensa-textus-secundus); font: inherit;' +
  '}' +
  'mensa-scidula .delere:hover { color: var(--mensa-accentus); }';

class MensaThema extends HTMLElement {
  connectedCallback() {
    var i, a;
    if (!document.getElementById('mensa-stili')) {
      var s = document.createElement('style');
      s.id = 'mensa-stili';
      s.textContent = MENSA_STILI;
      document.head.appendChild(s);
    }
    for (i = 0; i < this.attributes.length; i++) {
      a = this.attributes[i];
      this.style.setProperty('--mensa-' + a.name, a.value);
    }
  }
}

if (typeof customElements !== 'undefined') {
  customElements.define('mensa-thema', MensaThema);
}

/* ================ ex lib/mensa_assets/fontes/scida.js ================ */
/* scida.js - <mensa-scida>: charta tractabilis in plano.
 *
 * Positio in CENTESIMIS plani (attributa x/y; latitudo optionalis) -
 * responsio = %, non puncta. Tractus per eventa punctoris; in fine
 * tractus 'mensa-actum' {genus:'positum'} ebullit - persistentia.js
 * solum pontem tangit, componenta sine ponte usibilia manent. */

class MensaScida extends HTMLElement {
  connectedCallback() {
    if (this._paratum) { return; }
    this._paratum = true;
    this.ponePositum(
      parseFloat(this.getAttribute('x') || '10'),
      parseFloat(this.getAttribute('y') || '10'));
    if (this.getAttribute('latitudo')) {
      this.style.width = this.getAttribute('latitudo') + '%';
    }
    this.addEventListener('pointerdown',
      this._prehendere.bind(this));
  }

  ponePositum(x, y) {
    this._x = x;
    this._y = y;
    this.style.left = x + '%';
    this.style.top = y + '%';
  }

  _prehendere(e) {
    var mensura, initX, initY, origoX, origoY, ipse, movere, solvere;
    /* elementa activa tractum non incipiunt */
    if (e.target.closest(
        'button, input, textarea, select, a, [contenteditable]')) {
      return;
    }
    if (!this.parentElement) { return; }
    mensura = this.parentElement.getBoundingClientRect();
    if (mensura.width === 0 || mensura.height === 0) { return; }
    initX = e.clientX;
    initY = e.clientY;
    origoX = this._x;
    origoY = this._y;
    ipse = this;
    this.classList.add('tractans');
    this.setPointerCapture(e.pointerId);
    movere = function(ev) {
      var dx = (ev.clientX - initX) / mensura.width * 100;
      var dy = (ev.clientY - initY) / mensura.height * 100;
      ipse.ponePositum(
        Math.max(0, Math.min(97, origoX + dx)),
        Math.max(0, Math.min(95, origoY + dy)));
    };
    solvere = function() {
      ipse.classList.remove('tractans');
      ipse.removeEventListener('pointermove', movere);
      ipse.removeEventListener('pointerup', solvere);
      ipse.removeEventListener('pointercancel', solvere);
      ipse.actumMittere('positum', {
        x: Math.round(ipse._x * 100) / 100,
        y: Math.round(ipse._y * 100) / 100
      });
    };
    this.addEventListener('pointermove', movere);
    this.addEventListener('pointerup', solvere);
    this.addEventListener('pointercancel', solvere);
  }

  /* datum.id semper additum; ebullit ad planum/persistentiam */
  actumMittere(genus, datum) {
    datum.id = this.id;
    this.dispatchEvent(new CustomEvent('mensa-actum', {
      bubbles: true,
      detail: { genus: genus, datum: datum }
    }));
  }
}

if (typeof customElements !== 'undefined') {
  customElements.define('mensa-scida', MensaScida);
}

/* ================ ex lib/mensa_assets/fontes/scidula.js ================ */
/* scidula.js - <mensa-scidula>: scidula adhaesiva (stickies macOS).
 *
 * EST scida (classem extendit - tractus hereditatur); corpus
 * contenteditable + bulla deletionis. Textus mutatus post moram
 * (600ms) 'mensa-actum' {genus:'textus'} mittit; deletio
 * {genus:'deletum'} mittit et elementum removet. */

class MensaScidula extends MensaScida {
  connectedCallback() {
    var ipse, initium;
    if (this._paratum) { return; }
    super.connectedCallback();
    ipse = this;
    initium = this.getAttribute('textus') || this.textContent || '';
    this.textContent = '';

    this._delere = document.createElement('button');
    this._delere.className = 'delere';
    this._delere.textContent = '×';
    this._delere.addEventListener('click', function() {
      ipse.actumMittere('deletum', {});
      ipse.remove();
    });
    this.appendChild(this._delere);

    this._corpus = document.createElement('div');
    this._corpus.className = 'corpus';
    this._corpus.contentEditable = 'true';
    this._corpus.textContent = initium;
    this._corpus.addEventListener('input', function() {
      if (ipse._mora) { clearTimeout(ipse._mora); }
      ipse._mora = setTimeout(function() {
        ipse._mora = null;
        ipse.actumMittere('textus',
          { textus: ipse._corpus.textContent });
      }, 600);
    });
    this.appendChild(this._corpus);
  }

  /* rehydratio sine emissione (persistentia vocat) */
  textumPonere(textus) {
    if (this._corpus) { this._corpus.textContent = textus; }
  }
}

if (typeof customElements !== 'undefined') {
  customElements.define('mensa-scidula', MensaScidula);
}

/* ================ ex lib/mensa_assets/fontes/planum.js ================ */
/* planum.js - <mensa-planum>: superficies scriptorii.
 *
 * LEX: planum numquam volvitur (overflow hidden in stilo);
 * elementa in centesimis ponuntur. Duplex-click in plano ipso
 * (non in scida) scidulam novam creat eo loco. Scriptoria
 * nidificata (thecae) = iteratio posterior. */

class MensaPlanum extends HTMLElement {
  connectedCallback() {
    if (this._paratum) { return; }
    this._paratum = true;
    this.addEventListener('dblclick', this._duplex.bind(this));
  }

  _duplex(e) {
    var mensura, x, y, scidula;
    if (e.target !== this) { return; }
    mensura = this.getBoundingClientRect();
    x = (e.clientX - mensura.left) / mensura.width * 100;
    y = (e.clientY - mensura.top) / mensura.height * 100;
    scidula = this.scidulamCreare(
      Math.round(x * 100) / 100, Math.round(y * 100) / 100, '',
      'scidula-' + Date.now().toString(36), false);
    scidula.actumMittere('creatum', {
      genus_elementi: 'scidula',
      x: scidula._x, y: scidula._y, textus: ''
    });
  }

  /* et duplex-click et rehydratio persistentiae hac via - corpus
   * unum, deerrare non possunt. tacite = sine eventu (rehydratio). */
  scidulamCreare(x, y, textus, id, tacite) {
    var scidula = document.createElement('mensa-scidula');
    scidula.id = id;
    scidula.setAttribute('x', String(x));
    scidula.setAttribute('y', String(y));
    scidula.setAttribute('textus', textus);
    this.appendChild(scidula);
    void tacite;   /* creatio ipsa numquam emittit - vocator emittit */
    return scidula;
  }
}

if (typeof customElements !== 'undefined') {
  customElements.define('mensa-planum', MensaPlanum);
}

/* ================ ex lib/mensa_assets/fontes/persistentia.js ================ */
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

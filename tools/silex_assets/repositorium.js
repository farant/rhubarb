/* repositorium.js - chartae VCS in tabula mensae (v2: overlay
 * mortuum, vivant chartae).
 *
 * Tres chartae declaratae in index.html, singulae EST scida
 * (tractus/positio persistens/z-ordo hereditantur):
 *   <silex-conditiones>  cauda conditionum, paginata
 *   <silex-plagulae>     plicatura ad conditionem electam
 *   <silex-contentum>    plagula lecta, lineis paginata
 * Nexus in-loco: conditio electa -> plagulae; plagula electa ->
 * contentum (eventa in documento; extractio in chartas proprias =
 * futurum consulto dilatum).
 *
 * LEX MENSAE SERVATA: numquam volvitur - paginatur (MensaPaginator
 * e logica, sagittae in pede chartae). Ordines pointerdown sistunt:
 * charta capite/margine trahitur, ordo libere clicatur.
 *
 * CAVE SELECTORIBUS (lex v2.1 mensae): tags novae hereditatem CSS
 * classium JS non sequuntur - fundamentum chartae hic REPLICATUR
 * pro tags silicis.
 *
 * Lectores puri (vcs_historia/plica/plagula) per praebitorem
 * SILEX_VCS - chartae pontem numquam nominant (lex DI); silexVcs()
 * in index.html eum iniectat et onerationem incipit. */

var SILEX_VCS = null;

var SILEX_REPOSITORIUM_STILI =
  /* fundamentum chartae replicatum (vide CAVE supra) */
  'silex-conditiones, silex-plagulae, silex-contentum {' +
  '  display: block; position: absolute;' +
  '  background: var(--mensa-charta);' +
  '  border: 1px solid var(--mensa-margo);' +
  '  border-radius: 6px; padding: .9em 1.1em;' +
  '  box-shadow: var(--mensa-umbra);' +
  '  cursor: grab;' +
  '  user-select: none; -webkit-user-select: none;' +
  '}' +
  'silex-conditiones.tractans, silex-plagulae.tractans,' +
  'silex-contentum.tractans { cursor: grabbing; opacity: .92; }' +
  'silex-conditiones.electum, silex-plagulae.electum,' +
  'silex-contentum.electum {' +
  '  outline: 2px solid var(--mensa-accentus);' +
  '}' +
  'silex-conditiones h2, silex-plagulae h2, silex-contentum h2 {' +
  '  color: var(--mensa-textus-secundus); font-size: .85em;' +
  '  margin: 0 0 .5em 0; text-transform: uppercase;' +
  '  letter-spacing: .1em; white-space: nowrap;' +
  '  overflow: hidden; text-overflow: ellipsis;' +
  '}' +

  /* ordines listarum */
  '.silex-vcs-res {' +
  '  padding: .25em .5em; border-radius: 4px; cursor: pointer;' +
  '}' +
  '.silex-vcs-res:hover { background: var(--mensa-fundus); }' +
  '.silex-vcs-res.electa {' +
  '  background: var(--mensa-fundus);' +
  '  border-left: 3px solid var(--mensa-accentus);' +
  '  padding-left: calc(.5em - 3px);' +
  '}' +
  '.silex-vcs-res .linea-prima {' +
  '  display: block; white-space: nowrap; overflow: hidden;' +
  '  text-overflow: ellipsis;' +
  '}' +
  '.silex-vcs-res .linea-secunda {' +
  '  display: block; color: var(--mensa-textus-secundus);' +
  '  font-size: .8em; white-space: nowrap; overflow: hidden;' +
  '  text-overflow: ellipsis;' +
  '}' +

  /* pes: sagittae paginationis + status */
  '.silex-vcs-pes {' +
  '  display: flex; align-items: center; gap: .6em;' +
  '  margin-top: .5em; justify-content: center;' +
  '}' +
  '.silex-vcs-pes button {' +
  '  background: none; border: 1px solid var(--mensa-margo);' +
  '  border-radius: 4px; color: var(--mensa-textus);' +
  '  font: inherit; cursor: pointer; padding: 0 .6em;' +
  '}' +
  '.silex-vcs-pes button:hover {' +
  '  border-color: var(--mensa-accentus);' +
  '  color: var(--mensa-accentus);' +
  '}' +
  '.silex-vcs-pes button:disabled {' +
  '  opacity: .3; cursor: default;' +
  '}' +
  '.silex-vcs-pes .status {' +
  '  color: var(--mensa-textus-secundus); font-size: .8em;' +
  '}' +

  /* contentum: lineae praeformatae, minutae */
  'silex-contentum pre {' +
  '  margin: 0; font: .85em/1.45 monospace; white-space: pre;' +
  '  overflow: hidden;' +
  '  user-select: text; -webkit-user-select: text; cursor: text;' +
  '}' +
  'silex-contentum .vacuum, silex-plagulae .vacuum,' +
  'silex-conditiones .vacuum {' +
  '  color: var(--mensa-textus-secundus); font-style: italic;' +
  '}' +

  /* ansa magnitudinis (exemplar imaginis: latitudo sola) */
  '.silex-vcs-ansa {' +
  '  position: absolute; right: -2px; bottom: -2px;' +
  '  width: 14px; height: 14px; cursor: ew-resize;' +
  '  border-right: 3px solid var(--mensa-accentus);' +
  '  border-bottom: 3px solid var(--mensa-accentus);' +
  '  border-radius: 0 0 5px 0; opacity: 0;' +
  '}' +
  'silex-conditiones:hover .silex-vcs-ansa,' +
  'silex-plagulae:hover .silex-vcs-ansa,' +
  'silex-contentum:hover .silex-vcs-ansa { opacity: .85; }';

/* fundamentum commune chartarum VCS: caput + corpus + pes paginans
 * + ansa latitudinis. Subclasses: _quantum(), onerare-viam suam. */
class SilexChartaVcs extends MensaScida {
  connectedCallback() {
    var ipse;
    if (this._paratum) { return; }
    super.connectedCallback();
    ipse = this;
    this.ordo = [];
    this.paginator = new MensaPaginator(this._quantum());

    this._titulus = document.createElement('h2');
    this._titulus.textContent = this._titulusPrimus();
    this.appendChild(this._titulus);
    this._corpus = document.createElement('div');
    this.appendChild(this._corpus);

    this._pes = document.createElement('div');
    this._pes.className = 'silex-vcs-pes';
    this._ante = document.createElement('button');
    this._ante.textContent = '‹';
    this._ante.addEventListener('click', function () {
      if (ipse.paginator.ante()) { ipse._reddere(); }
    });
    this._status = document.createElement('span');
    this._status.className = 'status';
    this._post = document.createElement('button');
    this._post.textContent = '›';
    this._post.addEventListener('click', function () {
      if (ipse.paginator.post(ipse.ordo.length)) { ipse._reddere(); }
    });
    this._pes.appendChild(this._ante);
    this._pes.appendChild(this._status);
    this._pes.appendChild(this._post);
    this.appendChild(this._pes);

    this._ansa = document.createElement('div');
    this._ansa.className = 'silex-vcs-ansa';
    this._ansa.addEventListener('pointerdown',
      this._latitudinemPrehendere.bind(this));
    this.appendChild(this._ansa);
  }

  _titulusPrimus() { return 'vcs'; }
  _quantum() { return 8; }

  titulumPonere(textus) {
    if (this._titulus) { this._titulus.textContent = textus; }
  }

  nuntiumMonstrare(textus) {
    this.ordo = [];
    this._corpus.innerHTML = '';
    var d = document.createElement('div');
    d.className = 'vacuum';
    d.textContent = textus;
    this._corpus.appendChild(d);
    this._pes.style.display = 'none';
  }

  ordinemPonere(ordo) {
    this.ordo = ordo;
    this.paginator.pagina = 0;
    this._pes.style.display = ordo.length > this.paginator.quantum
      ? 'flex' : 'none';
    this._reddere();
  }

  _reddere() {
    var ipse = this;
    var fenestra = this.paginator.fenestra(this.ordo);
    var index;

    this._corpus.innerHTML = '';
    for (index = 0; index < fenestra.length; index = index + 1) {
      this._corpus.appendChild(this._resCreare(fenestra[index]));
    }
    this._status.textContent =
      this.paginator.statum(this.ordo.length);
    this._ante.disabled = this.paginator.pagina === 0;
    this._post.disabled = this.paginator.pagina + 1
      >= this.paginator.paginae(this.ordo.length);
  }

  /* ordo unius rei; pointerdown sistitur - charta capite trahitur,
   * ordo libere clicatur */
  _resCreare(res) {
    var ipse = this;
    var d = document.createElement('div');
    var l1 = document.createElement('span');
    var l2 = document.createElement('span');

    d.className = 'silex-vcs-res';
    l1.className = 'linea-prima';
    l1.textContent = this._lineaPrima(res);
    l2.className = 'linea-secunda';
    l2.textContent = this._lineaSecunda(res);
    d.appendChild(l1);
    d.appendChild(l2);
    d.addEventListener('pointerdown', function (e) {
      e.stopPropagation();
    });
    d.addEventListener('click', function () {
      ipse._electio(res, d);
    });
    return d;
  }

  _lineaPrima(res) { return String(res); }
  _lineaSecunda(res) { return ''; }
  _electio(res, elementum) {}

  _electamSignare(elementum) {
    var index;
    var omnes = this._corpus.querySelectorAll('.silex-vcs-res');

    for (index = 0; index < omnes.length; index = index + 1) {
      omnes[index].classList.toggle('electa',
        omnes[index] === elementum);
    }
  }

  /* latitudo sola (exemplar imaginis) - altitudo paginatione regitur */
  _latitudinemPrehendere(e) {
    var ipse = this;
    var planum_mensura, initX, origo_lat, movere, solvere;

    e.stopPropagation();
    if (!this.parentElement) { return; }
    planum_mensura = this.parentElement.getBoundingClientRect();
    if (planum_mensura.width === 0) { return; }
    initX = e.clientX;
    origo_lat = this.getBoundingClientRect().width
      / planum_mensura.width * 100;
    this._ansa.setPointerCapture(e.pointerId);

    movere = function (ev) {
      var lat = Math.max(12, Math.min(90,
        origo_lat + (ev.clientX - initX)
          / planum_mensura.width * 100));
      ipse.style.width = lat + '%';
    };
    solvere = function () {
      ipse._ansa.removeEventListener('pointermove', movere);
      ipse._ansa.removeEventListener('pointerup', solvere);
      ipse._ansa.removeEventListener('pointercancel', solvere);
      ipse.actumMittere('magnitudo', {
        latitudo: Math.round(ipse.getBoundingClientRect().width
          / planum_mensura.width * 10000) / 100
      });
    };
    this._ansa.addEventListener('pointermove', movere);
    this._ansa.addEventListener('pointerup', solvere);
    this._ansa.addEventListener('pointercancel', solvere);
  }

  latitudinemPonere(pct) {
    this.style.width = pct + '%';
  }
}

class SilexConditiones extends SilexChartaVcs {
  _titulusPrimus() { return 'conditiones'; }
  _quantum() { return 6; }

  onerare() {
    var ipse = this;

    if (!SILEX_VCS) { return; }
    SILEX_VCS.historia()
      .then(function (r) {
        var ordo = r.ordo.slice().reverse();

        ordo.unshift({ seq: 0, nuntius: 'arbor praesens',
          momentum: '(plica manifesti hodierna)', tactae: null });
        ipse.ordinemPonere(ordo);
        ipse._electio(ordo[0], ipse._corpus.firstChild);
      })
      .catch(function (e) {
        ipse.nuntiumMonstrare('proiectum sub cwd non inventum -'
          + ' aperi silex e directorio proiecti');
      });
  }

  _lineaPrima(c) {
    return c.seq === 0 ? c.nuntius : '[' + c.seq + '] ' + c.nuntius;
  }
  _lineaSecunda(c) {
    return c.tactae === null ? c.momentum
      : c.momentum + ' · ' + c.tactae + ' plagulae';
  }
  _electio(c, elementum) {
    this._electamSignare(elementum);
    document.dispatchEvent(new CustomEvent('vcs-conditio-electa', {
      detail: { seq: c.seq }
    }));
  }
}

class SilexPlagulae extends SilexChartaVcs {
  connectedCallback() {
    var ipse;
    if (this._paratum) { return; }
    super.connectedCallback();
    ipse = this;
    this.nuntiumMonstrare('elige conditionem');
    document.addEventListener('vcs-conditio-electa', function (e) {
      ipse._onerare(e.detail.seq);
    });
  }

  _titulusPrimus() { return 'plagulae'; }
  _quantum() { return 9; }

  _onerare(seq) {
    var ipse = this;

    if (!SILEX_VCS) { return; }
    this._seq = seq;
    this.titulumPonere(seq === 0 ? 'plagulae · praesens'
      : 'plagulae · seq ' + seq);
    SILEX_VCS.plica(seq)
      .then(function (r) { ipse.ordinemPonere(r.ordo); })
      .catch(function (e) {
        ipse.nuntiumMonstrare('CULPA: ' + e.message);
      });
  }

  _lineaPrima(p) { return p.via; }
  _lineaSecunda(p) { return p.origo; }
  _electio(p, elementum) {
    this._electamSignare(elementum);
    document.dispatchEvent(new CustomEvent('vcs-plagula-electa', {
      detail: { seq: this._seq, via: p.via }
    }));
  }
}

class SilexContentum extends SilexChartaVcs {
  connectedCallback() {
    var ipse;
    if (this._paratum) { return; }
    super.connectedCallback();
    ipse = this;
    this.nuntiumMonstrare('elige plagulam');
    document.addEventListener('vcs-plagula-electa', function (e) {
      ipse._onerare(e.detail.seq, e.detail.via);
    });
  }

  _titulusPrimus() { return 'contentum'; }
  _quantum() { return 24; }   /* lineae per paginam */

  _onerare(seq, via) {
    var ipse = this;

    if (!SILEX_VCS) { return; }
    this.titulumPonere(via + ' · '
      + (seq === 0 ? 'praesens' : 'seq ' + seq));
    SILEX_VCS.plagula(seq, via)
      .then(function (r) {
        ipse.ordinemPonere(r.contentum.split('\n'));
      })
      .catch(function (e) {
        ipse.nuntiumMonstrare('CULPA: ' + e.message);
      });
  }

  /* lineae non ordines clicabiles sunt - pre unum per paginam */
  _reddere() {
    var pre = document.createElement('pre');
    var fenestra = this.paginator.fenestra(this.ordo);

    this._corpus.innerHTML = '';
    pre.textContent = fenestra.join('\n');
    pre.addEventListener('pointerdown', function (e) {
      e.stopPropagation();   /* textus eligibilis, non tractus */
    });
    this._corpus.appendChild(pre);
    this._status.textContent =
      this.paginator.statum(this.ordo.length);
    this._ante.disabled = this.paginator.pagina === 0;
    this._post.disabled = this.paginator.pagina + 1
      >= this.paginator.paginae(this.ordo.length);
  }
}

if (typeof customElements !== 'undefined') {
  customElements.define('silex-conditiones', SilexConditiones);
  customElements.define('silex-plagulae', SilexPlagulae);
  customElements.define('silex-contentum', SilexContentum);
}

/* initium: stilos inicere, praebitorem ponere, onerationem incipere */
function silexVcs(praebitor) {
  var stili, conditiones;

  if (!document.getElementById('silex-repositorium-stili')) {
    stili = document.createElement('style');
    stili.id = 'silex-repositorium-stili';
    stili.textContent = SILEX_REPOSITORIUM_STILI;
    document.head.appendChild(stili);
  }
  SILEX_VCS = praebitor;
  conditiones = document.querySelector('silex-conditiones');
  if (conditiones && conditiones.onerare) { conditiones.onerare(); }
}

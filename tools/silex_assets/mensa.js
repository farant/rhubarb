/* GLOMERATUM a tools/glomerare.sh - NOLI MANU EDERE;
   fontes veri infra nominantur. */

/* ================ ex lib/mensa_assets/fontes/logica.js ================ */
/* logica.js - logica pura mensae: SINE DOM, sine temporibus veris -
 * probabilis synchronice (probatio_mensae.js). Planum haec adhibet;
 * hic nihil de elementis HTML scitur. */

/* fusio plana per-id - SPECULUM plicae C (lib/mensa.c _statum_valor).
 * Si formae deerrant, probationes ambae (C + JS) simul corrigendae. */
function mensaFundere(elementa, genus, datum) {
  var id, clavis, elem;
  if (!datum || !datum.id) { return elementa; }
  id = datum.id;
  if (genus === 'deletum') {
    delete elementa[id];
    return elementa;
  }
  elem = elementa[id];
  if (!elem) { elem = elementa[id] = {}; }
  for (clavis in datum) {
    if (clavis !== 'id') { elem[clavis] = datum[clavis]; }
  }
  return elementa;
}

/* quae elementa in tabula data reddenda - tabula absens = 'radix';
 * elementa interna (genus_elementi 'internum') numquam redduntur */
function mensaQuaeRedenda(elementa, tabula) {
  var id, ordo = [];
  for (id in elementa) {
    if (elementa[id].genus_elementi === 'internum') { continue; }
    if ((elementa[id].tabula || 'radix') === tabula) { ordo.push(id); }
  }
  return ordo;
}

/* quo retro e tabula currente: acervus navigationis vincit (margo
 * qua venisti - electio graph-compatibilis); acervo vacuo, sedes
 * thecae quae tabulam nominat (elementa[tabula].tabula); orphana ->
 * radix. In radice nullum retro (null). */
function mensaRetro(elementa, tabula, acervus) {
  if (tabula === 'radix') { return null; }
  if (acervus && acervus.length > 0) {
    return acervus[acervus.length - 1];
  }
  if (elementa[tabula]) { return elementa[tabula].tabula || 'radix'; }
  return 'radix';
}

/* ARBITER GESTUUM - machina statuum plani (tene-vs-trahe-vs-duplex).
 * Sine DOM: planum eventa punctoria in vocationes vertit et
 * actiones exsequitur. Fructus quisque = null aut {actio: ...}:
 *   tempus_incipe  - planum horologium tenendi incipiat (450ms)
 *   tempus_dele    - horologium deleat
 *   orbis_aperi    - menu radiale ad (x,y) aperiat
 *   duplex_planum  - creatio repetita ad (x,y)
 * Status 'orbis': duplex et sursum supprimuntur donec orbis_clausus. */
function MensaArbiter(limen) {
  this.limen = typeof limen === 'number' ? limen : 6;
  this.status = 'quies';
  this.x = 0;
  this.y = 0;
}
MensaArbiter.prototype.deorsum = function (x, y, in_plano) {
  if (this.status === 'orbis') { return null; }
  if (!in_plano) { this.status = 'quies'; return null; }
  this.status = 'tenens';
  this.x = x;
  this.y = y;
  return { actio: 'tempus_incipe' };
};
MensaArbiter.prototype.motus = function (x, y) {
  var dx, dy;
  if (this.status !== 'tenens') { return null; }
  dx = x - this.x;
  dy = y - this.y;
  if (dx * dx + dy * dy > this.limen * this.limen) {
    this.status = 'quies';
    return { actio: 'tempus_dele' };
  }
  return null;
};
MensaArbiter.prototype.sursum = function () {
  if (this.status === 'tenens') {
    this.status = 'quies';
    return { actio: 'tempus_dele' };
  }
  return null;
};
MensaArbiter.prototype.tempus_teneri = function () {
  if (this.status !== 'tenens') { return null; }
  this.status = 'orbis';
  return { actio: 'orbis_aperi', x: this.x, y: this.y };
};
MensaArbiter.prototype.orbis_clausus = function () {
  this.status = 'quies';
};
MensaArbiter.prototype.duplex = function (x, y, in_plano) {
  if (!in_plano || this.status === 'orbis') { return null; }
  return { actio: 'duplex_planum', x: x, y: y };
};

/* ================ ex lib/mensa_assets/fontes/thema.js ================ */
/* thema.js - <mensa-thema>: thema ut componentum.
 *
 * Stilos omnium componentorum mensae SEMEL in caput inicit
 * (proprietates CSS consuetae = superficies thematis); attributa
 * elementi proprietates superscribunt in subtree suo:
 *   <mensa-thema fundus="#101216" accentus="#e8a04c">
 * fit --mensa-fundus / --mensa-accentus in hoc elemento.
 * SINE shadow DOM consulto: thema globale, inspector verax,
 * harness probationis simplex.
 *
 * CAVE SELECTORIBUS: CSS hereditatem classium JS NON sequitur -
 * mensa-scidula est tag ALIUD quam mensa-scida, ergo fundamentum
 * chartarum selectoribus IUNCTIS datur (mensa-scida, mensa-scidula,
 * mensa-theca). Decipula mensurata: sine iunctione scidulae
 * position:absolute carebant - in fluxu sedebant, xy click
 * ignorabant, tractus nihil movebat (v2, manibus Franis capta).
 *
 * LEX SELECTIONIS: nihil eligibile nisi textus editionis - chartae,
 * thecae, orbis, tituli user-select:none; solum .editans .corpus
 * textum eligere sinit. */

var MENSA_STILI =
  'mensa-thema {' +
  '  display: block; width: 100%; height: 100%;' +
  '  --mensa-fundus: #16181d;' +
  '  --mensa-charta: #101216;' +
  '  --mensa-margo: #2a2d35;' +
  '  --mensa-textus: #d8d4cc;' +
  '  --mensa-textus-secundus: #8a8578;' +
  '  --mensa-accentus: #e8a04c;' +
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

  /* fundamentum chartarum COMMUNE (vide CAVE supra) */
  'mensa-scida, mensa-scidula, mensa-theca {' +
  '  display: block; position: absolute;' +
  '  background: var(--mensa-charta);' +
  '  border: 1px solid var(--mensa-margo);' +
  '  border-radius: 6px; padding: .9em 1.1em;' +
  '  box-shadow: var(--mensa-umbra);' +
  '  cursor: grab;' +
  '  user-select: none; -webkit-user-select: none;' +
  '}' +
  'mensa-scida.tractans, mensa-scidula.tractans,' +
  'mensa-theca.tractans { cursor: grabbing; opacity: .92; }' +
  'mensa-scida.electum, mensa-scidula.electum,' +
  'mensa-theca.electum { outline: 2px solid var(--mensa-accentus); }' +
  'mensa-scida.sectum, mensa-scidula.sectum,' +
  'mensa-theca.sectum { opacity: .45; outline-style: dashed; }' +
  'mensa-scida h2 {' +
  '  color: var(--mensa-textus-secundus); font-size: .85em;' +
  '  margin: 0 0 .5em 0; text-transform: uppercase;' +
  '  letter-spacing: .1em;' +
  '}' +

  /* scidula: charta eadem ac ceterae (petitio Franis) - mensurae
   * minimae solae propriae */
  'mensa-scidula { min-width: 11em; min-height: 4.5em; }' +
  'mensa-scidula .corpus {' +
  '  min-height: 3em; cursor: inherit; white-space: pre-wrap;' +
  '  outline: none;' +
  '}' +
  'mensa-scidula.editans { cursor: default; }' +
  'mensa-scidula.editans .corpus {' +
  '  cursor: text; outline: 1px dashed var(--mensa-accentus);' +
  '  user-select: text; -webkit-user-select: text;' +
  '}' +
  'mensa-scidula .delere {' +
  '  position: absolute; top: .2em; right: .35em;' +
  '  background: none; border: none; cursor: pointer;' +
  '  color: var(--mensa-textus-secundus); font: inherit;' +
  '  user-select: none; -webkit-user-select: none;' +
  '}' +
  'mensa-scidula .delere:hover { color: var(--mensa-accentus); }' +

  /* theca: icon plicae (fundamentum commune superscribit) */
  'mensa-theca {' +
  '  width: 7em; padding: .7em .5em .5em .5em;' +
  '  text-align: center; box-shadow: none;' +
  '}' +
  'mensa-theca .glyphus {' +
  '  width: 3.2em; height: 2.3em; margin: 0 auto;' +
  '  background: var(--mensa-margo);' +
  '  border-radius: 3px; position: relative;' +
  '}' +
  'mensa-theca .glyphus::before {' +
  '  content: ""; position: absolute; top: -.45em; left: 0;' +
  '  width: 1.4em; height: .5em;' +
  '  background: var(--mensa-margo);' +
  '  border-radius: 3px 3px 0 0;' +
  '}' +
  'mensa-theca:hover .glyphus,' +
  'mensa-theca:hover .glyphus::before {' +
  '  background: var(--mensa-accentus);' +
  '}' +
  'mensa-theca .titulus {' +
  '  margin-top: .4em; font-size: .85em; word-break: break-word;' +
  '}' +
  'mensa-theca .titulus input {' +
  '  background: var(--mensa-fundus); color: var(--mensa-textus);' +
  '  border: 1px solid var(--mensa-accentus); font: inherit;' +
  '  width: 95%; text-align: center;' +
  '}' +
  /* theca retro: margo qua venisti - obliqua et muta */
  'mensa-theca[retro] {' +
  '  font-style: italic; opacity: .7; cursor: pointer;' +
  '}' +
  'mensa-theca[retro] .glyphus,' +
  'mensa-theca[retro] .glyphus::before {' +
  '  background: var(--mensa-textus-secundus);' +
  '}' +

  /* orbis: menu radiale - numquam eligibile */
  'mensa-orbis {' +
  '  position: absolute; transform: translate(-50%, -50%);' +
  '  z-index: 99;' +
  '  user-select: none; -webkit-user-select: none;' +
  '}' +
  'mensa-orbis .petalum {' +
  '  position: absolute; transform: translate(-50%, -50%);' +
  '  background: var(--mensa-charta); color: var(--mensa-textus);' +
  '  border: 1px solid var(--mensa-accentus);' +
  '  border-radius: 999px; padding: .5em 1em; font: inherit;' +
  '  cursor: pointer; white-space: nowrap;' +
  '  box-shadow: var(--mensa-umbra);' +
  '  user-select: none; -webkit-user-select: none;' +
  '}' +
  'mensa-orbis .petalum:hover {' +
  '  background: var(--mensa-accentus); color: var(--mensa-fundus);' +
  '}' +
  'mensa-orbis .petalum-0 { left: -3.2em; top: 0; }' +
  'mensa-orbis .petalum-1 { left: 3.2em; top: 0; }';

class MensaThema extends HTMLElement {
  connectedCallback() {
    var i, a;
    if (!document.getElementById('mensa-stili')) {
      var s = document.createElement('style');
      s.id = 'mensa-stili';
      s.textContent = MENSA_STILI;
      document.head.appendChild(s);
    }
    for (i = 0; i < this.attributes.length; i = i + 1) {
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
 * Positio in CENTESIMIS plani (attributa x/y; latitudo optionalis).
 * Tractus incipit post limen motus (4 puncta) - sine motu, click =
 * selectio (planum.eligere). In fine tractus VERI: si punctor super
 * theca stat, 'collocatum' (motio in tabulam eius); alioquin
 * 'positum'. Attributum 'fixa' = numquam tractabilis (theca retro).
 * persistentia.js solum pontem tangit - componenta sine ponte
 * usibilia manent. */

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
    var mensura, initX, initY, origoX, origoY, ipse;
    var motum, movere, solvere;

    if (this.hasAttribute('fixa')) { return; }
    if (this.classList.contains('editans')) { return; }
    if (e.target.closest(
        'button, input, textarea, select, a, [contenteditable=true]')) {
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
    motum = false;
    this.setPointerCapture(e.pointerId);

    movere = function (ev) {
      var dx = ev.clientX - initX;
      var dy = ev.clientY - initY;

      if (!motum) {
        if (dx * dx + dy * dy < 16) { return; }   /* limen 4pt */
        motum = true;
        ipse.classList.add('tractans');
      }
      ipse.ponePositum(
        Math.max(0, Math.min(97, origoX + dx / mensura.width * 100)),
        Math.max(0, Math.min(95, origoY + dy / mensura.height * 100)));
    };
    solvere = function (ev) {
      var planum = ipse.closest('mensa-planum');
      var sub, theca;

      ipse.classList.remove('tractans');
      ipse.removeEventListener('pointermove', movere);
      ipse.removeEventListener('pointerup', solvere);
      ipse.removeEventListener('pointercancel', solvere);

      if (!motum) {
        /* click purus = selectio */
        if (planum) { planum.eligere(ipse); }
        return;
      }
      /* depositio super thecam = motio in tabulam eius (theca
       * retro = motio in tabulam PETITAM - sursum uno gradu) */
      sub = document.elementFromPoint(ev.clientX, ev.clientY);
      theca = sub ? sub.closest('mensa-theca') : null;
      if (theca && theca !== ipse) {
        ipse.actumMittere('collocatum', {
          tabula: theca.hasAttribute('retro')
            ? theca.getAttribute('petens') : theca.id
        });
        return;
      }
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
/* scidula.js - <mensa-scidula>: scidula adhaesiva.
 *
 * Obiectum quietum: NON semper editabilis (tractus liberatur) -
 * duplex-click editionem aperit, blur/Escape claudit (textus
 * mutatus emittitur in clausura). EST scida; deletio per bullam ×. */

class MensaScidula extends MensaScida {
  connectedCallback() {
    var ipse, initium;
    if (this._paratum) { return; }
    super.connectedCallback();
    ipse = this;
    initium = this.getAttribute('textus') || '';

    this._delere = document.createElement('button');
    this._delere.className = 'delere';
    this._delere.textContent = '×';
    this._delere.addEventListener('click', function () {
      ipse.actumMittere('deletum', {});
      ipse.remove();
    });
    this.appendChild(this._delere);

    this._corpus = document.createElement('div');
    this._corpus.className = 'corpus';
    this._corpus.contentEditable = 'false';
    this._corpus.textContent = initium;
    this._corpus.addEventListener('blur', function () {
      ipse.editionemClaudere();
    });
    this.appendChild(this._corpus);

    this.addEventListener('dblclick', function (e) {
      e.stopPropagation();
      ipse.editionemAperire();
    });
  }

  editionemAperire() {
    if (this.classList.contains('editans')) { return; }
    this._ante = this._corpus.textContent;
    this.classList.add('editans');
    this._corpus.contentEditable = 'true';
    this._corpus.focus();
  }

  editionemClaudere() {
    if (!this.classList.contains('editans')) { return; }
    this.classList.remove('editans');
    this._corpus.contentEditable = 'false';
    if (this._corpus.textContent !== this._ante) {
      this.actumMittere('textus',
        { textus: this._corpus.textContent });
    }
  }

  /* rehydratio sine emissione (planum vocat) */
  textumPonere(textus) {
    if (this._corpus && !this.classList.contains('editans')) {
      this._corpus.textContent = textus;
    }
  }
}

if (typeof customElements !== 'undefined') {
  customElements.define('mensa-scidula', MensaScidula);
}

/* ================ ex lib/mensa_assets/fontes/theca.js ================ */
/* theca.js - <mensa-theca>: plica in plano - EST scida (tractus
 * hereditatur) et IPSA TABULAM NOMINAT: id thecae = id tabulae
 * intra eam. Duplex-click intrat; res in eam tractae moventur
 * (scida.js probat casum in solvere).
 *
 * Theca 'retro' (synthetica, a plano nata, numquam condita):
 * attributa retro + fixa + petens=<tabula> - margo qua venisti,
 * non parens conditus (electio graph-compatibilis). */

class MensaTheca extends MensaScida {
  connectedCallback() {
    var ipse;
    if (this._paratum) { return; }
    super.connectedCallback();
    ipse = this;

    this._glyphus = document.createElement('div');
    this._glyphus.className = 'glyphus';
    this.appendChild(this._glyphus);

    this._titulus = document.createElement('div');
    this._titulus.className = 'titulus';
    this._titulus.textContent = this.getAttribute('titulus') || '';
    this.appendChild(this._titulus);

    this.addEventListener('dblclick', function (e) {
      var planum;
      if (ipse._nominans) { return; }
      e.stopPropagation();
      planum = ipse.closest('mensa-planum');
      if (!planum) { return; }
      if (ipse.hasAttribute('retro')) {
        planum.exire();
      } else {
        planum.intrare(ipse.id);
      }
    });
  }

  titulumPonere(titulus) {
    if (this._titulus) { this._titulus.textContent = titulus; }
  }

  /* editio nominis (post creationem vocatur - Enter/blur committit) */
  nominare() {
    var ipse = this;
    var arca = document.createElement('input');

    if (this._nominans) { return; }
    this._nominans = true;
    arca.value = this._titulus.textContent;
    this._titulus.textContent = '';
    this._titulus.appendChild(arca);
    arca.focus();
    arca.select();

    function committere() {
      var novum = arca.value || 'theca';
      ipse._nominans = false;
      ipse._titulus.textContent = novum;
      ipse.actumMittere('titulus', { titulus: novum });
    }
    arca.addEventListener('blur', committere);
    arca.addEventListener('keydown', function (e) {
      if (e.key === 'Enter') { arca.blur(); }
      e.stopPropagation();
    });
  }
}

if (typeof customElements !== 'undefined') {
  customElements.define('mensa-theca', MensaTheca);
}

/* ================ ex lib/mensa_assets/fontes/orbis.js ================ */
/* orbis.js - <mensa-orbis>: menu radiale (preme-et-tene in plano).
 * Petala duo pro nunc (theca / nota); planum aperit et claudit
 * (click-alibi et Escape ibi tractantur - orbis ipse solum petala
 * pingit et delectum refert). */

class MensaOrbis extends HTMLElement {
  /* x/y in centesimis plani; petala = [{genus, titulus}];
   * delectus(genus) vocatur ante clausuram */
  aperire(x, y, petala, delectus) {
    var ipse = this;
    var index;

    this.style.left = x + '%';
    this.style.top = y + '%';
    this.textContent = '';
    for (index = 0; index < petala.length; index = index + 1) {
      (function (petalum, situs) {
        var bulla = document.createElement('button');

        bulla.className = 'petalum petalum-' + situs;
        bulla.textContent = petalum.titulus;
        bulla.addEventListener('click', function (e) {
          e.stopPropagation();
          delectus(petalum.genus);
        });
        ipse.appendChild(bulla);
      })(petala[index], index);
    }
  }
}

if (typeof customElements !== 'undefined') {
  customElements.define('mensa-orbis', MensaOrbis);
}

/* ================ ex lib/mensa_assets/fontes/planum.js ================ */
/* planum.js - <mensa-planum>: superficies scriptoriorum NIDIFICATORUM.
 *
 * v2: planum tabulam currentem E STATU reddit (non iam "HTML
 * declaratum hydrata"). Registrum nodorum (declarati capiuntur in
 * ortu, creati e statu nascuntur); navigatio = re-redditio filtro
 * alio. LEX: numquam volvitur.
 *
 * Gestus (arbiter in logica.js, probatus sine DOM):
 *   preme-et-tene (450ms) in plano vacuo -> orbis (theca/nota)
 *   duplex-click in plano vacuo -> genus ULTIMUM creatum repetere
 *   click in scida -> selectio; ⌘X secat (intentio), ⌘V inserit
 *   (transactio - collocatum in tabulam currentem); Escape omnia
 *   claudit. Numquam insertum = numquam factum.
 *
 * Cache statūs: auscultator proprius 'mensa-actum' omnem eventum
 * in cache fundit (mensaFundere = speculum plicae C) - re-redditio
 * sine itinere ad pontem recta est. */

var MENSA_TEMPUS_TENENDI = 450;

class MensaPlanum extends HTMLElement {
  connectedCallback() {
    var ipse, index, pueri;
    if (this._paratum) { return; }
    this._paratum = true;
    ipse = this;

    this._elementa = {};
    this._tabula = 'radix';
    this._acervus = [];
    this._nodi = {};
    this._ultimum = 'nota';
    this._electum = null;
    this._sectum = null;
    this._orbis = null;
    this._retro = null;
    this._arbiter = new MensaArbiter(6);
    this._tempus = null;

    /* nodos declaratos capere (scidae app - radix implicita) */
    pueri = Array.prototype.slice.call(this.children);
    for (index = 0; index < pueri.length; index = index + 1) {
      if (pueri[index].id) {
        this._nodi[pueri[index].id] = pueri[index];
      }
    }

    /* cache semper recens: omnis actus (e scidis aut ex ipso) */
    this.addEventListener('mensa-actum', function (e) {
      mensaFundere(ipse._elementa, e.detail.genus, e.detail.datum);
      if (e.detail.genus === 'collocatum'
          || e.detail.genus === 'deletum') {
        ipse.reddere();
      }
    });

    this.addEventListener('pointerdown',
      this._deorsum.bind(this));
    this.addEventListener('pointermove', function (e) {
      ipse._agere(ipse._arbiter.motus(e.clientX, e.clientY));
    });
    this.addEventListener('pointerup', function () {
      ipse._agere(ipse._arbiter.sursum());
    });
    this.addEventListener('dblclick', this._duplex.bind(this));
    document.addEventListener('keydown', this._clavis.bind(this));

    this.reddere();
  }

  /* ---------- status + redditio ---------- */

  statumPonere(elementa) {
    var internum = elementa['internum-activa'];

    this._elementa = elementa || {};
    this._acervus = [];
    if (internum && internum.activa) {
      this._tabula = internum.activa;
    }
    this.reddere();
  }

  reddere() {
    var ipse = this;
    var id, ordo, index, node, datum;

    this._orbemClaudere();
    this.eligere(null);

    /* omnes nodos registri detachere */
    for (id in this._nodi) {
      node = this._nodi[id];
      if (node.parentElement === this) { this.removeChild(node); }
    }
    if (this._retro && this._retro.parentElement === this) {
      this.removeChild(this._retro);
      this._retro = null;
    }

    /* elementa tabulae currentis e statu */
    ordo = mensaQuaeRedenda(this._elementa, this._tabula);
    for (index = 0; index < ordo.length; index = index + 1) {
      id = ordo[index];
      datum = this._elementa[id];
      node = this._nodi[id] || this._nodumCreare(id, datum);
      if (!node) { continue; }
      this.appendChild(node);
      if (typeof datum.x === 'number' && typeof datum.y === 'number'
          && node.ponePositum) {
        node.ponePositum(datum.x, datum.y);
      }
      if (typeof datum.textus === 'string' && node.textumPonere) {
        node.textumPonere(datum.textus);
      }
      if (typeof datum.titulus === 'string' && node.titulumPonere) {
        node.titulumPonere(datum.titulus);
      }
    }

    /* declarati sine statu: in radice suo loco HTML manent */
    if (this._tabula === 'radix') {
      for (id in this._nodi) {
        if (!(id in this._elementa)
            && this._nodi[id].parentElement !== this) {
          this.appendChild(this._nodi[id]);
        }
      }
    }

    /* theca retro synthetica (numquam condita) */
    if (this._tabula !== 'radix') {
      var quo = mensaRetro(this._elementa, this._tabula,
        this._acervus);
      var retro = document.createElement('mensa-theca');

      retro.setAttribute('retro', '');
      retro.setAttribute('fixa', '');
      retro.setAttribute('petens', quo);
      retro.setAttribute('titulus', '↩ retro');
      retro.setAttribute('x', '2');
      retro.setAttribute('y', '3');
      this._retro = retro;
      this.appendChild(retro);
      void ipse;
    }
  }

  _nodumCreare(id, datum) {
    var node = null;

    if (datum.genus_elementi === 'scidula'
        || datum.genus_elementi === 'nota') {
      node = document.createElement('mensa-scidula');
      node.setAttribute('textus', datum.textus || '');
    } else if (datum.genus_elementi === 'theca') {
      node = document.createElement('mensa-theca');
      node.setAttribute('titulus', datum.titulus || 'theca');
    } else {
      return null;   /* genus ignotum aut scida declarata absens */
    }
    node.id = id;
    node.setAttribute('x',
      String(typeof datum.x === 'number' ? datum.x : 10));
    node.setAttribute('y',
      String(typeof datum.y === 'number' ? datum.y : 10));
    this._nodi[id] = node;
    return node;
  }

  /* ---------- navigatio ---------- */

  intrare(tabula) {
    this._acervus.push(this._tabula);
    this._tabula = tabula;
    this._visumMittere();
    this.reddere();
  }

  exire() {
    var quo = mensaRetro(this._elementa, this._tabula,
      this._acervus);

    if (quo === null) { return; }
    if (this._acervus.length > 0) { this._acervus.pop(); }
    this._tabula = quo;
    this._visumMittere();
    this.reddere();
  }

  _visumMittere() {
    this.actumMittere('visum', {
      id: 'internum-activa', genus_elementi: 'internum',
      activa: this._tabula
    });
  }

  /* ---------- selectio + secare/inserere ---------- */

  eligere(node) {
    if (this._electum && this._nodi[this._electum]) {
      this._nodi[this._electum].classList.remove('electum');
    }
    this._electum = node ? node.id : null;
    if (node) { node.classList.add('electum'); }
  }

  _clavis(e) {
    var editans = document.activeElement
      && (document.activeElement.isContentEditable
        || document.activeElement.tagName === 'INPUT');

    if (e.key === 'Escape') {
      this._orbemClaudere();
      if (editans) { document.activeElement.blur(); }
      if (this._sectum && this._nodi[this._sectum]) {
        this._nodi[this._sectum].classList.remove('sectum');
      }
      this._sectum = null;
      this.eligere(null);
      return;
    }
    if (editans || !e.metaKey) { return; }
    if (e.key === 'x' && this._electum) {
      if (this._sectum && this._nodi[this._sectum]) {
        this._nodi[this._sectum].classList.remove('sectum');
      }
      this._sectum = this._electum;
      this._nodi[this._sectum].classList.add('sectum');
      e.preventDefault();
    } else if (e.key === 'v' && this._sectum) {
      /* transactio: motio in tabulam currentem */
      if (this._nodi[this._sectum]) {
        this._nodi[this._sectum].classList.remove('sectum');
      }
      this.actumMittere('collocatum', {
        id: this._sectum, tabula: this._tabula, x: 42, y: 38
      });
      this._sectum = null;
      e.preventDefault();
    }
  }

  /* ---------- gestus plani ---------- */

  _deorsum(e) {
    var ipse = this;

    if (this._orbis && !this._orbis.contains(e.target)) {
      this._orbemClaudere();
      return;
    }
    if (e.target === this) { this.eligere(null); }
    this._agere(this._arbiter.deorsum(e.clientX, e.clientY,
      e.target === this));
    void ipse;
  }

  _duplex(e) {
    var f = this._arbiter.duplex(e.clientX, e.clientY,
      e.target === this);

    if (f && f.actio === 'duplex_planum') {
      this._creareAd(this._ultimum, e.clientX, e.clientY);
    }
  }

  _agere(f) {
    var ipse = this;

    if (!f) { return; }
    if (f.actio === 'tempus_incipe') {
      this._tempus = setTimeout(function () {
        ipse._tempus = null;
        ipse._agere(ipse._arbiter.tempus_teneri());
      }, MENSA_TEMPUS_TENENDI);
    } else if (f.actio === 'tempus_dele') {
      if (this._tempus) {
        clearTimeout(this._tempus);
        this._tempus = null;
      }
    } else if (f.actio === 'orbis_aperi') {
      this._orbemAperire(f.x, f.y);
    }
  }

  _orbemAperire(clientX, clientY) {
    var ipse = this;
    var mensura = this.getBoundingClientRect();
    var x = (clientX - mensura.left) / mensura.width * 100;
    var y = (clientY - mensura.top) / mensura.height * 100;

    this._orbis = document.createElement('mensa-orbis');
    this.appendChild(this._orbis);
    this._orbis.aperire(x, y,
      [{ genus: 'theca', titulus: 'theca' },
       { genus: 'nota', titulus: 'nota' }],
      function (genus) {
        ipse._orbemClaudere();
        ipse._creareAd(genus, clientX, clientY);
      });
  }

  _orbemClaudere() {
    if (this._orbis) {
      this.removeChild(this._orbis);
      this._orbis = null;
    }
    this._arbiter.orbis_clausus();
  }

  /* ---------- creatio ---------- */

  _creareAd(genus, clientX, clientY) {
    var mensura = this.getBoundingClientRect();
    var x = Math.round((clientX - mensura.left)
      / mensura.width * 10000) / 100;
    var y = Math.round((clientY - mensura.top)
      / mensura.height * 10000) / 100;
    var id, node;

    this._ultimum = genus;
    if (genus === 'theca') {
      id = 'theca-' + Date.now().toString(36);
      node = this._nodumCreare(id,
        { genus_elementi: 'theca', titulus: 'nova', x: x, y: y });
      this.appendChild(node);
      node.ponePositum(x, y);
      node.actumMittere('creatum', {
        genus_elementi: 'theca', titulus: 'nova',
        x: x, y: y, tabula: this._tabula
      });
      node.nominare();
    } else {
      id = 'scidula-' + Date.now().toString(36);
      node = this._nodumCreare(id,
        { genus_elementi: 'scidula', textus: '', x: x, y: y });
      this.appendChild(node);
      node.ponePositum(x, y);
      node.actumMittere('creatum', {
        genus_elementi: 'scidula', textus: '',
        x: x, y: y, tabula: this._tabula
      });
      node.editionemAperire();
    }
  }

  /* emissio ex ipso plano (visum, insertum) */
  actumMittere(genus, datum) {
    this.dispatchEvent(new CustomEvent('mensa-actum', {
      bubbles: true,
      detail: { genus: genus, datum: datum }
    }));
  }
}

if (typeof customElements !== 'undefined') {
  customElements.define('mensa-planum', MensaPlanum);
}

/* ================ ex lib/mensa_assets/fontes/persistentia.js ================ */
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

  internuntius.vocare('mensa_status', {})
    .then(function (r) {
      planum.statumPonere(r.elementa || {});
    })
    .catch(function (err) {
      console.log('mensa: status non oneratus: ' + err.message);
      planum.statumPonere({});
    });
}

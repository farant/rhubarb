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

/* titulus tabulae currentis = titulus thecae quae eam nominat;
 * radix sine titulo (null); theca sine titulo -> id ipsum */
function mensaTitulus(elementa, tabula) {
  if (tabula === 'radix') { return null; }
  if (elementa[tabula] && elementa[tabula].titulus) {
    return elementa[tabula].titulus;
  }
  return tabula;
}

/* ordo summus trans elementa omnia - basis levationis (novus
 * summus = summus + 1). Elementa sine ordine = 0; interna ordinem
 * numquam ferunt, ergo sponte innocua. */
function mensaOrdoSummus(elementa) {
  var id, summus = 0;
  for (id in elementa) {
    if (typeof elementa[id].ordo === 'number'
        && elementa[id].ordo > summus) {
      summus = elementa[id].ordo;
    }
  }
  return summus;
}

/* levatio chartae prehensae: fructus = ordo novus (summus + 1) aut
 * null si charta iam SOLA in summo stat - acta quieta: prehensio
 * sine effectu visibili eventum non scribit. Aequalitas in summo
 * (duae chartae eodem ordine) levationem meret. */
function mensaLevatio(elementa, id) {
  var alia, solus;
  var summus = mensaOrdoSummus(elementa);
  var elem = elementa[id];

  if (elem && elem.ordo === summus && summus > 0) {
    solus = true;
    for (alia in elementa) {
      if (alia !== id && elementa[alia].ordo === summus) {
        solus = false;
        break;
      }
    }
    if (solus) { return null; }
  }
  return summus + 1;
}

/* PAGINATOR - lex mensae: numquam volvitur, paginatur. Purum (sine
 * DOM): chartae fenestram rogant, sagittae ante/post vocant.
 * Clamping ubique - pagina extra fines numquam redditur; ordo
 * brevior facta paginam ultimam retinet quae exsistit. */
function MensaPaginator(quantum) {
  this.quantum = typeof quantum === 'number' && quantum > 0
    ? quantum : 10;
  this.pagina = 0;
}
MensaPaginator.prototype.paginae = function (numerus) {
  if (numerus <= 0) { return 1; }
  return Math.ceil(numerus / this.quantum);
};
MensaPaginator.prototype._artare = function (numerus) {
  var summa = this.paginae(numerus);
  if (this.pagina >= summa) { this.pagina = summa - 1; }
  if (this.pagina < 0) { this.pagina = 0; }
};
MensaPaginator.prototype.fenestra = function (ordo) {
  this._artare(ordo.length);
  return ordo.slice(this.pagina * this.quantum,
    (this.pagina + 1) * this.quantum);
};
MensaPaginator.prototype.ante = function () {
  if (this.pagina > 0) { this.pagina = this.pagina - 1; return true; }
  return false;
};
MensaPaginator.prototype.post = function (numerus) {
  if (this.pagina + 1 < this.paginae(numerus)) {
    this.pagina = this.pagina + 1;
    return true;
  }
  return false;
};
MensaPaginator.prototype.statum = function (numerus) {
  this._artare(numerus);
  return (this.pagina + 1) + '/' + this.paginae(numerus);
};

/* ---- paginatio linearum involutarum (visus plagularum) ----
 * Monospatium + fractio per characterem (break-all) faciunt ordines
 * involutos ARITHMETICAM puram: ceil(mensura/columnae) - nulla
 * mensuratio DOM, recomputatio inter tractum gratis. */

/* quot ordines visuales linea fontis occupat (vacua = 1) */
function mensaOrdinesLineae(mensura, columnae) {
  if (columnae < 1) { columnae = 1; }
  if (mensura <= 0) { return 1; }
  return Math.ceil(mensura / columnae);
}

/* partitio linearum in paginas: ordines = numerus ordinum visualium
 * per lineam; ordinesPaginae = quot ordines pagina capit. Fructus =
 * indices lineae PRIMAE cuiusque paginae. Linea una pagina maior
 * paginam propriam accipit (praecisa redditur - malum minus quam
 * volutio). */
function mensaPartitioLinearum(ordines, ordinesPaginae) {
  var initia = [0];
  var cumulus = 0;
  var index;

  if (ordinesPaginae < 1) { ordinesPaginae = 1; }
  for (index = 0; index < ordines.length; index = index + 1) {
    if (cumulus > 0 && cumulus + ordines[index] > ordinesPaginae) {
      initia.push(index);
      cumulus = 0;
    }
    cumulus = cumulus + ordines[index];
  }
  return initia;
}

/* qua pagina linea ancorae iacet (regula Franis: post reflexionem
 * salta ad paginam quae lineam summam tuam CONTINET - etiamsi
 * media in pagina iam iaceat) */
function mensaPaginaAncorae(initia, linea) {
  var index;

  for (index = initia.length - 1; index >= 0; index = index - 1) {
    if (initia[index] <= linea) { return index; }
  }
  return 0;
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
  'mensa-scida, mensa-scidula, mensa-theca, mensa-imago {' +
  '  display: block; position: absolute;' +
  '  background: var(--mensa-charta);' +
  '  border: 1px solid var(--mensa-margo);' +
  '  border-radius: 6px; padding: .9em 1.1em;' +
  '  box-shadow: var(--mensa-umbra);' +
  '  cursor: grab;' +
  '  user-select: none; -webkit-user-select: none;' +
  '}' +
  'mensa-scida.tractans, mensa-scidula.tractans, mensa-imago.tractans,' +
  'mensa-theca.tractans { cursor: grabbing; opacity: .92; }' +
  'mensa-scida.electum, mensa-scidula.electum, mensa-imago.electum,' +
  'mensa-theca.electum { outline: 2px solid var(--mensa-accentus); }' +
  'mensa-scida.sectum, mensa-scidula.sectum, mensa-imago.sectum,' +
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

  /* theca: icon plicae NUDUM - sine charta (petitio Franis v2.2):
   * glyphus + titulus soli; fundamentum commune superscribitur */
  'mensa-theca {' +
  '  width: 7em; padding: .5em .3em .3em .3em;' +
  '  text-align: center;' +
  '  background: none; border: none; box-shadow: none;' +
  '}' +
  /* suscipiens: depositio imminens - accenditur sub tracto */
  'mensa-theca.suscipiens {' +
  '  outline: 2px dashed var(--mensa-accentus);' +
  '  outline-offset: 2px; border-radius: 6px;' +
  '}' +
  'mensa-theca.suscipiens .glyphus,' +
  'mensa-theca.suscipiens .glyphus::before {' +
  '  background: var(--mensa-accentus);' +
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
  /* theca retro: margo qua venisti - obliqua et muta. STRATUM
   * CHROME: z-index supra chartas levatas (ordo levationis per
   * prehensiones crescit - 900000 numquam realiter attingitur);
   * ante ordinem retro semper ultima appendebatur, hoc id servat */
  'mensa-theca[retro] {' +
  '  font-style: italic; opacity: .7; cursor: pointer;' +
  '  z-index: 900000;' +
  '}' +
  'mensa-theca[retro] .glyphus,' +
  'mensa-theca[retro] .glyphus::before {' +
  '  background: var(--mensa-textus-secundus);' +
  '}' +

  /* imago: charta imaginis - latitudo sola regitur, altitudo
   * sequitur (ratio aspectus semper servata) */
  'mensa-imago { padding: .35em; line-height: 0; }' +
  'mensa-imago img {' +
  '  width: 100%; height: auto; display: block;' +
  '  border-radius: 4px; pointer-events: none;' +
  '  -webkit-user-drag: none;' +
  '}' +
  'mensa-imago .ansa {' +
  '  position: absolute; right: -2px; bottom: -2px;' +
  '  width: 14px; height: 14px; cursor: nwse-resize;' +
  '  border-right: 3px solid var(--mensa-accentus);' +
  '  border-bottom: 3px solid var(--mensa-accentus);' +
  '  border-radius: 0 0 5px 0; opacity: 0;' +
  '}' +
  'mensa-imago:hover .ansa { opacity: .85; }' +

  /* titulus tabulae: chrome plani summo medio - gestus transeunt
   * (stratum chrome, vide retro) */
  '.mensa-titulus-tabulae {' +
  '  position: absolute; top: 1em; left: 50%;' +
  '  z-index: 900001;' +
  '  transform: translateX(-50%);' +
  '  color: var(--mensa-textus-secundus);' +
  '  font-size: .95em; letter-spacing: .22em;' +
  '  text-transform: uppercase;' +
  '  pointer-events: none;' +
  '  user-select: none; -webkit-user-select: none;' +
  '}' +

  /* orbis: menu radiale - numquam eligibile; supra chrome omne */
  'mensa-orbis {' +
  '  position: absolute; transform: translate(-50%, -50%);' +
  '  z-index: 999999;' +
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
    var motum, movere, solvere, planum_levans;

    if (this.hasAttribute('fixa')) { return; }
    if (this.classList.contains('editans')) { return; }
    if (e.target.closest(
        'button, input, textarea, select, a, [contenteditable=true]')) {
      return;
    }
    if (!this.parentElement) { return; }
    mensura = this.parentElement.getBoundingClientRect();
    if (mensura.width === 0 || mensura.height === 0) { return; }

    /* prehensio levat: charta ad frontem (persistens per levatum) */
    planum_levans = this.closest('mensa-planum');
    if (planum_levans && planum_levans.levare) {
      planum_levans.levare(this);
    }
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
      var theca;

      if (!motum) {
        if (dx * dx + dy * dy < 16) { return; }   /* limen 4pt */
        motum = true;
        ipse.classList.add('tractans');
      }
      ipse.ponePositum(
        Math.max(0, Math.min(97, origoX + dx / mensura.width * 100)),
        Math.max(0, Math.min(95, origoY + dy / mensura.height * 100)));

      /* affordantia depositionis: theca sub punctore accenditur */
      theca = ipse._subTheca(ev);
      if (theca !== ipse._suscipiens_nota) {
        if (ipse._suscipiens_nota) {
          ipse._suscipiens_nota.classList.remove('suscipiens');
        }
        ipse._suscipiens_nota = theca;
        if (theca) { theca.classList.add('suscipiens'); }
      }
    };
    solvere = function (ev) {
      var planum = ipse.closest('mensa-planum');
      var theca;

      ipse.classList.remove('tractans');
      ipse.removeEventListener('pointermove', movere);
      ipse.removeEventListener('pointerup', solvere);
      ipse.removeEventListener('pointercancel', solvere);
      if (ipse._suscipiens_nota) {
        ipse._suscipiens_nota.classList.remove('suscipiens');
        ipse._suscipiens_nota = null;
      }

      if (!motum) {
        /* click purus = selectio */
        if (planum) { planum.eligere(ipse); }
        return;
      }
      /* depositio super thecam = motio in tabulam eius (theca
       * retro = motio in tabulam PETITAM - sursum uno gradu) */
      theca = ipse._subTheca(ev);
      if (theca) {
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

  /* theca sub punctore - charta tracta punctorem sequitur, ergo
   * elementFromPoint EAM redderet: abscondita-proba-restituta
   * (synchronum - numquam pingitur). Decipula classica DnD,
   * manibus Franis capta ("depositio non operatur"). */
  _subTheca(ev) {
    var sub, theca;

    this.style.visibility = 'hidden';
    sub = document.elementFromPoint(ev.clientX, ev.clientY);
    this.style.visibility = '';
    theca = sub ? sub.closest('mensa-theca') : null;
    return (theca && theca !== this) ? theca : null;
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

/* ================ ex lib/mensa_assets/fontes/imago.js ================ */
/* imago.js - <mensa-imago>: charta imaginis.
 *
 * EST scida (tractus/selectio/depositio hereditantur). Imago intra
 * <img latitudo 100%> vivit - altitudo sequitur, ergo mutatio
 * magnitudinis (ansa anguli) LATITUDINEM SOLAM movet et ratio
 * aspectus SEMPER servatur. Contentum in massis voluminis
 * (sigillum in statu; praebitor imaginum a persistentia iniectus -
 * componenta pontem numquam nominant, lex DI). */

class MensaImago extends MensaScida {
  connectedCallback() {
    var ipse;
    if (this._paratum) { return; }
    super.connectedCallback();
    ipse = this;
    if (!this.getAttribute('latitudo')) {
      this.style.width = '24%';
    }

    this._img = document.createElement('img');
    this._img.alt = '';
    this.appendChild(this._img);

    this._ansa = document.createElement('div');
    this._ansa.className = 'ansa';
    this._ansa.addEventListener('pointerdown',
      this._magnitudinemPrehendere.bind(this));
    this.appendChild(this._ansa);
    void ipse;
  }

  /* rehydratio/paste - sine emissione */
  imaginemPonere(dataUrl) {
    if (this._img) { this._img.src = dataUrl; }
  }

  latitudinemPonere(pct) {
    this.style.width = pct + '%';
  }

  _magnitudinemPrehendere(e) {
    var ipse = this;
    var planum_mensura, initX, origo_lat, movere, solvere, lat;

    e.stopPropagation();   /* numquam tractus chartae */
    if (!this.parentElement) { return; }
    planum_mensura = this.parentElement.getBoundingClientRect();
    if (planum_mensura.width === 0) { return; }
    initX = e.clientX;
    origo_lat = this.getBoundingClientRect().width
      / planum_mensura.width * 100;
    lat = origo_lat;
    this._ansa.setPointerCapture(e.pointerId);

    movere = function (ev) {
      lat = Math.max(5, Math.min(90,
        origo_lat + (ev.clientX - initX)
          / planum_mensura.width * 100));
      ipse.latitudinemPonere(lat);
    };
    solvere = function () {
      ipse._ansa.removeEventListener('pointermove', movere);
      ipse._ansa.removeEventListener('pointerup', solvere);
      ipse._ansa.removeEventListener('pointercancel', solvere);
      ipse.actumMittere('magnitudo',
        { latitudo: Math.round(lat * 100) / 100 });
    };
    this._ansa.addEventListener('pointermove', movere);
    this._ansa.addEventListener('pointerup', solvere);
    this._ansa.addEventListener('pointercancel', solvere);
  }
}

if (typeof customElements !== 'undefined') {
  customElements.define('mensa-imago', MensaImago);
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
 * sine itinere ad pontem recta est.
 *
 * Ordo (v4): prehensio chartam levat (levare -> actum 'levatum'
 * {ordo: summus+1}); redditio ordinem ut z-index applicat; chartae
 * novae in summo nascuntur. Chrome (retro/titulus/orbis) strato
 * superiore in themate. */

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
    document.addEventListener('paste', this._glutinare.bind(this));

    this._praebitor = null;   /* imaginum (persistentia iniectat) */
    this._genera = {};        /* factores chartarum ab apps */
    this.reddere();
  }

  /* genera registrata (lex DI): apps chartas proprias in plicam
   * inserunt sine mensa de eis sciente. factor(id, datum) ->
   * elementum; planum id/positionem/registrum ipse curat. Sine
   * registro genus ignotum tacite praeteritur (ut semper). */
  genusRegistrare(genus, factor) {
    this._genera[genus] = factor;
  }

  /* charta ab app creata (genus registratum) ad clientXY: ordo in
   * summo, creatum emissum - via eadem ac _creareAd sed datum ab
   * app venit (e.g. visus plagulae e tractu extra listam) */
  chartamCreare(datum, clientX, clientY) {
    var mensura = this.getBoundingClientRect();
    var x = Math.max(0, Math.min(95,
      Math.round((clientX - mensura.left)
        / mensura.width * 10000) / 100));
    var y = Math.max(0, Math.min(93,
      Math.round((clientY - mensura.top)
        / mensura.height * 10000) / 100));
    var ordo = mensaOrdoSummus(this._elementa) + 1;
    var id = datum.genus_elementi + '-' + Date.now().toString(36);
    var emissum = {};
    var node, clavis;

    datum.x = x;
    datum.y = y;
    node = this._nodumCreare(id, datum);
    if (!node) { return null; }
    this.appendChild(node);
    node.ponePositum(x, y);
    node.style.zIndex = String(ordo);
    for (clavis in datum) { emissum[clavis] = datum[clavis]; }
    emissum.ordo = ordo;
    emissum.tabula = this._tabula;
    node.actumMittere('creatum', emissum);
    return node;
  }

  /* praebitor imaginum: {condere(b64)->Promise(sigillum),
   * promere(sigillum)->Promise(b64)} - lex DI: componenta pontem
   * numquam nominant; sine praebitore paste iners, imagines vacuae */
  imaginesPraebere(praebitor) {
    this._praebitor = praebitor;
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
      if (typeof datum.latitudo === 'number'
          && node.latitudinemPonere) {
        node.latitudinemPonere(datum.latitudo);
      }
      if (typeof datum.altitudo === 'number'
          && node.altitudinemPonere) {
        node.altitudinemPonere(datum.altitudo);
      }
      if (datum.imago && node.imaginemPonere) {
        this._imaginemImplere(node, datum);
      }
      /* stratum e statu: ordo -> z-index (chartae sine ordine = 0,
       * inter se ordine DOM stant) */
      node.style.zIndex = String(
        typeof datum.ordo === 'number' ? datum.ordo : 0);
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

    /* titulus tabulae (chrome plani, non charta; pointer-events
     * none - gestus subter transeunt) */
    if (this._titulusTabulae
        && this._titulusTabulae.parentElement === this) {
      this.removeChild(this._titulusTabulae);
      this._titulusTabulae = null;
    }
    {
      var titulus = mensaTitulus(this._elementa, this._tabula);

      if (titulus !== null) {
        this._titulusTabulae = document.createElement('div');
        this._titulusTabulae.className = 'mensa-titulus-tabulae';
        this._titulusTabulae.textContent = titulus;
        this.appendChild(this._titulusTabulae);
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

    if (this._genera[datum.genus_elementi]) {
      node = this._genera[datum.genus_elementi](id, datum);
    } else if (datum.genus_elementi === 'scidula'
        || datum.genus_elementi === 'nota') {
      node = document.createElement('mensa-scidula');
      node.setAttribute('textus', datum.textus || '');
    } else if (datum.genus_elementi === 'theca') {
      node = document.createElement('mensa-theca');
      node.setAttribute('titulus', datum.titulus || 'theca');
    } else if (datum.genus_elementi === 'imago') {
      node = document.createElement('mensa-imago');
      if (typeof datum.latitudo === 'number') {
        node.setAttribute('latitudo', String(datum.latitudo));
      }
    } else {
      return null;   /* genus ignotum aut scida declarata absens */
    }
    if (!node) { return null; }   /* factor recusavit */
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

  /* ---------- levatio ---------- */

  /* charta prehensa ad frontem: decisio in logica (mensaLevatio -
   * null = iam sola summa, nihil scribitur), effectus hic (z-index
   * statim + actum 'levatum' - fusio generica, C nihil mutandum) */
  levare(node) {
    var ordo = mensaLevatio(this._elementa, node.id);

    if (ordo === null) { return; }
    node.style.zIndex = String(ordo);
    node.actumMittere('levatum', { ordo: ordo });
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
    var ordo = mensaOrdoSummus(this._elementa) + 1;
    var id, node;

    this._ultimum = genus;
    if (genus === 'theca') {
      id = 'theca-' + Date.now().toString(36);
      node = this._nodumCreare(id,
        { genus_elementi: 'theca', titulus: 'nova', x: x, y: y });
      this.appendChild(node);
      node.ponePositum(x, y);
      node.style.zIndex = String(ordo);
      node.actumMittere('creatum', {
        genus_elementi: 'theca', titulus: 'nova',
        x: x, y: y, ordo: ordo, tabula: this._tabula
      });
      node.nominare();
    } else {
      id = 'scidula-' + Date.now().toString(36);
      node = this._nodumCreare(id,
        { genus_elementi: 'scidula', textus: '', x: x, y: y });
      this.appendChild(node);
      node.ponePositum(x, y);
      node.style.zIndex = String(ordo);
      node.actumMittere('creatum', {
        genus_elementi: 'scidula', textus: '',
        x: x, y: y, ordo: ordo, tabula: this._tabula
      });
      node.editionemAperire();
    }
  }

  /* ---------- imagines ---------- */

  _imaginemImplere(node, datum) {
    if (!this._praebitor || node._impleta) { return; }
    node._impleta = true;
    this._praebitor.promere(datum.imago)
      .then(function (b64) {
        node.imaginemPonere('data:' + (datum.mimen || 'image/png')
          + ';base64,' + b64);
      })
      .catch(function (err) {
        node._impleta = false;
        console.log('mensa: imago non prompta: ' + err.message);
      });
  }

  /* glutinatio: imago in tabellario -> charta imaginis (contentum
   * in massas voluminis per praebitorem, sigillum in statum) */
  _glutinare(e) {
    var ipse = this;
    var res = e.clipboardData && e.clipboardData.items;
    var index, lima, mimen, lector;

    if (!res || !this._praebitor) { return; }
    for (index = 0; index < res.length; index = index + 1) {
      if (res[index].type.indexOf('image/') !== 0) { continue; }
      lima = res[index].getAsFile();
      if (!lima) { continue; }
      mimen = res[index].type;
      lector = new FileReader();
      lector.onload = function () {
        var dataUrl = lector.result;
        var b64 = dataUrl.slice(dataUrl.indexOf(',') + 1);

        ipse._praebitor.condere(b64)
          .then(function (sigillum) {
            var id = 'imago-' + Date.now().toString(36);
            var x = Math.round((28 + Math.random() * 14) * 100) / 100;
            var y = Math.round((22 + Math.random() * 14) * 100) / 100;
            var ordo = mensaOrdoSummus(ipse._elementa) + 1;
            var node = ipse._nodumCreare(id, {
              genus_elementi: 'imago', imago: sigillum,
              mimen: mimen, x: x, y: y, latitudo: 24
            });

            ipse.appendChild(node);
            node.ponePositum(x, y);
            node.latitudinemPonere(24);
            node.style.zIndex = String(ordo);
            node._impleta = true;          /* iam habemus */
            node.imaginemPonere(dataUrl);
            node.actumMittere('creatum', {
              genus_elementi: 'imago', imago: sigillum,
              mimen: mimen, x: x, y: y, latitudo: 24,
              ordo: ordo, tabula: ipse._tabula
            });
          })
          .catch(function (err) {
            console.log('mensa: imago non condita: ' + err.message);
          });
      };
      lector.readAsDataURL(lima);
      e.preventDefault();
      return;
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

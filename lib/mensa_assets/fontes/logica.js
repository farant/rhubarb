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

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

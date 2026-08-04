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
  /* theca retro: margo qua venisti - obliqua et muta */
  'mensa-theca[retro] {' +
  '  font-style: italic; opacity: .7; cursor: pointer;' +
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

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
  'mensa-scidula .delere:hover { color: var(--mensa-accentus); }' +
  'mensa-scidula .corpus { cursor: inherit; }' +
  'mensa-scidula.editans { cursor: default; }' +
  'mensa-scidula.editans .corpus {' +
  '  cursor: text; outline: 1px dashed var(--mensa-accentus);' +
  '  user-select: text; -webkit-user-select: text;' +
  '}' +
  /* selectio + secare */
  'mensa-scida.electum, mensa-scidula.electum, mensa-theca.electum {' +
  '  outline: 2px solid var(--mensa-accentus);' +
  '}' +
  'mensa-scida.sectum, mensa-scidula.sectum, mensa-theca.sectum {' +
  '  opacity: .45; outline-style: dashed;' +
  '}' +
  /* theca: icon plicae */
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
  /* orbis: menu radiale */
  'mensa-orbis {' +
  '  position: absolute; transform: translate(-50%, -50%);' +
  '  z-index: 99;' +
  '}' +
  'mensa-orbis .petalum {' +
  '  position: absolute; transform: translate(-50%, -50%);' +
  '  background: var(--mensa-charta); color: var(--mensa-textus);' +
  '  border: 1px solid var(--mensa-accentus);' +
  '  border-radius: 999px; padding: .5em 1em; font: inherit;' +
  '  cursor: pointer; white-space: nowrap;' +
  '  box-shadow: var(--mensa-umbra);' +
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
    for (i = 0; i < this.attributes.length; i++) {
      a = this.attributes[i];
      this.style.setProperty('--mensa-' + a.name, a.value);
    }
  }
}

if (typeof customElements !== 'undefined') {
  customElements.define('mensa-thema', MensaThema);
}

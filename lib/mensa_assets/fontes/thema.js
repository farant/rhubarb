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

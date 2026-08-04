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

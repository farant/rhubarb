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

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

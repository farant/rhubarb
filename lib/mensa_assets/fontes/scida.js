/* scida.js - <mensa-scida>: charta tractabilis in plano.
 *
 * Positio in CENTESIMIS plani (attributa x/y; latitudo optionalis) -
 * responsio = %, non puncta. Tractus per eventa punctoris; in fine
 * tractus 'mensa-actum' {genus:'positum'} ebullit - persistentia.js
 * solum pontem tangit, componenta sine ponte usibilia manent. */

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
    var mensura, initX, initY, origoX, origoY, ipse, movere, solvere;
    /* elementa activa tractum non incipiunt */
    if (e.target.closest(
        'button, input, textarea, select, a, [contenteditable]')) {
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
    this.classList.add('tractans');
    this.setPointerCapture(e.pointerId);
    movere = function(ev) {
      var dx = (ev.clientX - initX) / mensura.width * 100;
      var dy = (ev.clientY - initY) / mensura.height * 100;
      ipse.ponePositum(
        Math.max(0, Math.min(97, origoX + dx)),
        Math.max(0, Math.min(95, origoY + dy)));
    };
    solvere = function() {
      ipse.classList.remove('tractans');
      ipse.removeEventListener('pointermove', movere);
      ipse.removeEventListener('pointerup', solvere);
      ipse.removeEventListener('pointercancel', solvere);
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

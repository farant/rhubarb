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

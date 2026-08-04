/* theca.js - <mensa-theca>: plica in plano - EST scida (tractus
 * hereditatur) et IPSA TABULAM NOMINAT: id thecae = id tabulae
 * intra eam. Duplex-click intrat; res in eam tractae moventur
 * (scida.js probat casum in solvere).
 *
 * Theca 'retro' (synthetica, a plano nata, numquam condita):
 * attributa retro + fixa + petens=<tabula> - margo qua venisti,
 * non parens conditus (electio graph-compatibilis). */

class MensaTheca extends MensaScida {
  connectedCallback() {
    var ipse;
    if (this._paratum) { return; }
    super.connectedCallback();
    ipse = this;

    this._glyphus = document.createElement('div');
    this._glyphus.className = 'glyphus';
    this.appendChild(this._glyphus);

    this._titulus = document.createElement('div');
    this._titulus.className = 'titulus';
    this._titulus.textContent = this.getAttribute('titulus') || '';
    this.appendChild(this._titulus);

    this.addEventListener('dblclick', function (e) {
      var planum;
      if (ipse._nominans) { return; }
      e.stopPropagation();
      planum = ipse.closest('mensa-planum');
      if (!planum) { return; }
      if (ipse.hasAttribute('retro')) {
        planum.exire();
      } else {
        planum.intrare(ipse.id);
      }
    });
  }

  titulumPonere(titulus) {
    if (this._titulus) { this._titulus.textContent = titulus; }
  }

  /* editio nominis (post creationem vocatur - Enter/blur committit) */
  nominare() {
    var ipse = this;
    var arca = document.createElement('input');

    if (this._nominans) { return; }
    this._nominans = true;
    arca.value = this._titulus.textContent;
    this._titulus.textContent = '';
    this._titulus.appendChild(arca);
    arca.focus();
    arca.select();

    function committere() {
      var novum = arca.value || 'theca';
      ipse._nominans = false;
      ipse._titulus.textContent = novum;
      ipse.actumMittere('titulus', { titulus: novum });
    }
    arca.addEventListener('blur', committere);
    arca.addEventListener('keydown', function (e) {
      if (e.key === 'Enter') { arca.blur(); }
      e.stopPropagation();
    });
  }
}

if (typeof customElements !== 'undefined') {
  customElements.define('mensa-theca', MensaTheca);
}

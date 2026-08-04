/* scida.js - <mensa-scida>: charta tractabilis in plano.
 *
 * Positio in CENTESIMIS plani (attributa x/y; latitudo optionalis).
 * Tractus incipit post limen motus (4 puncta) - sine motu, click =
 * selectio (planum.eligere). In fine tractus VERI: si punctor super
 * theca stat, 'collocatum' (motio in tabulam eius); alioquin
 * 'positum'. Attributum 'fixa' = numquam tractabilis (theca retro).
 * persistentia.js solum pontem tangit - componenta sine ponte
 * usibilia manent. */

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
    var mensura, initX, initY, origoX, origoY, ipse;
    var motum, movere, solvere;

    if (this.hasAttribute('fixa')) { return; }
    if (this.classList.contains('editans')) { return; }
    if (e.target.closest(
        'button, input, textarea, select, a, [contenteditable=true]')) {
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
    motum = false;
    this.setPointerCapture(e.pointerId);

    movere = function (ev) {
      var dx = ev.clientX - initX;
      var dy = ev.clientY - initY;
      var theca;

      if (!motum) {
        if (dx * dx + dy * dy < 16) { return; }   /* limen 4pt */
        motum = true;
        ipse.classList.add('tractans');
      }
      ipse.ponePositum(
        Math.max(0, Math.min(97, origoX + dx / mensura.width * 100)),
        Math.max(0, Math.min(95, origoY + dy / mensura.height * 100)));

      /* affordantia depositionis: theca sub punctore accenditur */
      theca = ipse._subTheca(ev);
      if (theca !== ipse._suscipiens_nota) {
        if (ipse._suscipiens_nota) {
          ipse._suscipiens_nota.classList.remove('suscipiens');
        }
        ipse._suscipiens_nota = theca;
        if (theca) { theca.classList.add('suscipiens'); }
      }
    };
    solvere = function (ev) {
      var planum = ipse.closest('mensa-planum');
      var theca;

      ipse.classList.remove('tractans');
      ipse.removeEventListener('pointermove', movere);
      ipse.removeEventListener('pointerup', solvere);
      ipse.removeEventListener('pointercancel', solvere);
      if (ipse._suscipiens_nota) {
        ipse._suscipiens_nota.classList.remove('suscipiens');
        ipse._suscipiens_nota = null;
      }

      if (!motum) {
        /* click purus = selectio */
        if (planum) { planum.eligere(ipse); }
        return;
      }
      /* depositio super thecam = motio in tabulam eius (theca
       * retro = motio in tabulam PETITAM - sursum uno gradu) */
      theca = ipse._subTheca(ev);
      if (theca) {
        ipse.actumMittere('collocatum', {
          tabula: theca.hasAttribute('retro')
            ? theca.getAttribute('petens') : theca.id
        });
        return;
      }
      ipse.actumMittere('positum', {
        x: Math.round(ipse._x * 100) / 100,
        y: Math.round(ipse._y * 100) / 100
      });
    };
    this.addEventListener('pointermove', movere);
    this.addEventListener('pointerup', solvere);
    this.addEventListener('pointercancel', solvere);
  }

  /* theca sub punctore - charta tracta punctorem sequitur, ergo
   * elementFromPoint EAM redderet: abscondita-proba-restituta
   * (synchronum - numquam pingitur). Decipula classica DnD,
   * manibus Franis capta ("depositio non operatur"). */
  _subTheca(ev) {
    var sub, theca;

    this.style.visibility = 'hidden';
    sub = document.elementFromPoint(ev.clientX, ev.clientY);
    this.style.visibility = '';
    theca = sub ? sub.closest('mensa-theca') : null;
    return (theca && theca !== this) ? theca : null;
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

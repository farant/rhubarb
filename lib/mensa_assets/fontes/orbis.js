/* orbis.js - <mensa-orbis>: menu radiale (preme-et-tene in plano).
 * Petala duo pro nunc (theca / nota); planum aperit et claudit
 * (click-alibi et Escape ibi tractantur - orbis ipse solum petala
 * pingit et delectum refert). */

class MensaOrbis extends HTMLElement {
  /* x/y in centesimis plani; petala = [{genus, titulus}];
   * delectus(genus) vocatur ante clausuram */
  aperire(x, y, petala, delectus) {
    var ipse = this;
    var index;

    this.style.left = x + '%';
    this.style.top = y + '%';
    this.textContent = '';
    for (index = 0; index < petala.length; index = index + 1) {
      (function (petalum, situs) {
        var bulla = document.createElement('button');

        bulla.className = 'petalum petalum-' + situs;
        bulla.textContent = petalum.titulus;
        bulla.addEventListener('click', function (e) {
          e.stopPropagation();
          delectus(petalum.genus);
        });
        ipse.appendChild(bulla);
      })(petala[index], index);
    }
  }
}

if (typeof customElements !== 'undefined') {
  customElements.define('mensa-orbis', MensaOrbis);
}

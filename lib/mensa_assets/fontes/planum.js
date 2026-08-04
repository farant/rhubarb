/* planum.js - <mensa-planum>: superficies scriptoriorum NIDIFICATORUM.
 *
 * v2: planum tabulam currentem E STATU reddit (non iam "HTML
 * declaratum hydrata"). Registrum nodorum (declarati capiuntur in
 * ortu, creati e statu nascuntur); navigatio = re-redditio filtro
 * alio. LEX: numquam volvitur.
 *
 * Gestus (arbiter in logica.js, probatus sine DOM):
 *   preme-et-tene (450ms) in plano vacuo -> orbis (theca/nota)
 *   duplex-click in plano vacuo -> genus ULTIMUM creatum repetere
 *   click in scida -> selectio; ⌘X secat (intentio), ⌘V inserit
 *   (transactio - collocatum in tabulam currentem); Escape omnia
 *   claudit. Numquam insertum = numquam factum.
 *
 * Cache statūs: auscultator proprius 'mensa-actum' omnem eventum
 * in cache fundit (mensaFundere = speculum plicae C) - re-redditio
 * sine itinere ad pontem recta est. */

var MENSA_TEMPUS_TENENDI = 450;

class MensaPlanum extends HTMLElement {
  connectedCallback() {
    var ipse, index, pueri;
    if (this._paratum) { return; }
    this._paratum = true;
    ipse = this;

    this._elementa = {};
    this._tabula = 'radix';
    this._acervus = [];
    this._nodi = {};
    this._ultimum = 'nota';
    this._electum = null;
    this._sectum = null;
    this._orbis = null;
    this._retro = null;
    this._arbiter = new MensaArbiter(6);
    this._tempus = null;

    /* nodos declaratos capere (scidae app - radix implicita) */
    pueri = Array.prototype.slice.call(this.children);
    for (index = 0; index < pueri.length; index = index + 1) {
      if (pueri[index].id) {
        this._nodi[pueri[index].id] = pueri[index];
      }
    }

    /* cache semper recens: omnis actus (e scidis aut ex ipso) */
    this.addEventListener('mensa-actum', function (e) {
      mensaFundere(ipse._elementa, e.detail.genus, e.detail.datum);
      if (e.detail.genus === 'collocatum'
          || e.detail.genus === 'deletum') {
        ipse.reddere();
      }
    });

    this.addEventListener('pointerdown',
      this._deorsum.bind(this));
    this.addEventListener('pointermove', function (e) {
      ipse._agere(ipse._arbiter.motus(e.clientX, e.clientY));
    });
    this.addEventListener('pointerup', function () {
      ipse._agere(ipse._arbiter.sursum());
    });
    this.addEventListener('dblclick', this._duplex.bind(this));
    document.addEventListener('keydown', this._clavis.bind(this));

    this.reddere();
  }

  /* ---------- status + redditio ---------- */

  statumPonere(elementa) {
    var internum = elementa['internum-activa'];

    this._elementa = elementa || {};
    this._acervus = [];
    if (internum && internum.activa) {
      this._tabula = internum.activa;
    }
    this.reddere();
  }

  reddere() {
    var ipse = this;
    var id, ordo, index, node, datum;

    this._orbemClaudere();
    this.eligere(null);

    /* omnes nodos registri detachere */
    for (id in this._nodi) {
      node = this._nodi[id];
      if (node.parentElement === this) { this.removeChild(node); }
    }
    if (this._retro && this._retro.parentElement === this) {
      this.removeChild(this._retro);
      this._retro = null;
    }

    /* elementa tabulae currentis e statu */
    ordo = mensaQuaeRedenda(this._elementa, this._tabula);
    for (index = 0; index < ordo.length; index = index + 1) {
      id = ordo[index];
      datum = this._elementa[id];
      node = this._nodi[id] || this._nodumCreare(id, datum);
      if (!node) { continue; }
      this.appendChild(node);
      if (typeof datum.x === 'number' && typeof datum.y === 'number'
          && node.ponePositum) {
        node.ponePositum(datum.x, datum.y);
      }
      if (typeof datum.textus === 'string' && node.textumPonere) {
        node.textumPonere(datum.textus);
      }
      if (typeof datum.titulus === 'string' && node.titulumPonere) {
        node.titulumPonere(datum.titulus);
      }
    }

    /* declarati sine statu: in radice suo loco HTML manent */
    if (this._tabula === 'radix') {
      for (id in this._nodi) {
        if (!(id in this._elementa)
            && this._nodi[id].parentElement !== this) {
          this.appendChild(this._nodi[id]);
        }
      }
    }

    /* theca retro synthetica (numquam condita) */
    if (this._tabula !== 'radix') {
      var quo = mensaRetro(this._elementa, this._tabula,
        this._acervus);
      var retro = document.createElement('mensa-theca');

      retro.setAttribute('retro', '');
      retro.setAttribute('fixa', '');
      retro.setAttribute('petens', quo);
      retro.setAttribute('titulus', '↩ retro');
      retro.setAttribute('x', '2');
      retro.setAttribute('y', '3');
      this._retro = retro;
      this.appendChild(retro);
      void ipse;
    }
  }

  _nodumCreare(id, datum) {
    var node = null;

    if (datum.genus_elementi === 'scidula'
        || datum.genus_elementi === 'nota') {
      node = document.createElement('mensa-scidula');
      node.setAttribute('textus', datum.textus || '');
    } else if (datum.genus_elementi === 'theca') {
      node = document.createElement('mensa-theca');
      node.setAttribute('titulus', datum.titulus || 'theca');
    } else {
      return null;   /* genus ignotum aut scida declarata absens */
    }
    node.id = id;
    node.setAttribute('x',
      String(typeof datum.x === 'number' ? datum.x : 10));
    node.setAttribute('y',
      String(typeof datum.y === 'number' ? datum.y : 10));
    this._nodi[id] = node;
    return node;
  }

  /* ---------- navigatio ---------- */

  intrare(tabula) {
    this._acervus.push(this._tabula);
    this._tabula = tabula;
    this._visumMittere();
    this.reddere();
  }

  exire() {
    var quo = mensaRetro(this._elementa, this._tabula,
      this._acervus);

    if (quo === null) { return; }
    if (this._acervus.length > 0) { this._acervus.pop(); }
    this._tabula = quo;
    this._visumMittere();
    this.reddere();
  }

  _visumMittere() {
    this.actumMittere('visum', {
      id: 'internum-activa', genus_elementi: 'internum',
      activa: this._tabula
    });
  }

  /* ---------- selectio + secare/inserere ---------- */

  eligere(node) {
    if (this._electum && this._nodi[this._electum]) {
      this._nodi[this._electum].classList.remove('electum');
    }
    this._electum = node ? node.id : null;
    if (node) { node.classList.add('electum'); }
  }

  _clavis(e) {
    var editans = document.activeElement
      && (document.activeElement.isContentEditable
        || document.activeElement.tagName === 'INPUT');

    if (e.key === 'Escape') {
      this._orbemClaudere();
      if (editans) { document.activeElement.blur(); }
      if (this._sectum && this._nodi[this._sectum]) {
        this._nodi[this._sectum].classList.remove('sectum');
      }
      this._sectum = null;
      this.eligere(null);
      return;
    }
    if (editans || !e.metaKey) { return; }
    if (e.key === 'x' && this._electum) {
      if (this._sectum && this._nodi[this._sectum]) {
        this._nodi[this._sectum].classList.remove('sectum');
      }
      this._sectum = this._electum;
      this._nodi[this._sectum].classList.add('sectum');
      e.preventDefault();
    } else if (e.key === 'v' && this._sectum) {
      /* transactio: motio in tabulam currentem */
      if (this._nodi[this._sectum]) {
        this._nodi[this._sectum].classList.remove('sectum');
      }
      this.actumMittere('collocatum', {
        id: this._sectum, tabula: this._tabula, x: 42, y: 38
      });
      this._sectum = null;
      e.preventDefault();
    }
  }

  /* ---------- gestus plani ---------- */

  _deorsum(e) {
    var ipse = this;

    if (this._orbis && !this._orbis.contains(e.target)) {
      this._orbemClaudere();
      return;
    }
    if (e.target === this) { this.eligere(null); }
    this._agere(this._arbiter.deorsum(e.clientX, e.clientY,
      e.target === this));
    void ipse;
  }

  _duplex(e) {
    var f = this._arbiter.duplex(e.clientX, e.clientY,
      e.target === this);

    if (f && f.actio === 'duplex_planum') {
      this._creareAd(this._ultimum, e.clientX, e.clientY);
    }
  }

  _agere(f) {
    var ipse = this;

    if (!f) { return; }
    if (f.actio === 'tempus_incipe') {
      this._tempus = setTimeout(function () {
        ipse._tempus = null;
        ipse._agere(ipse._arbiter.tempus_teneri());
      }, MENSA_TEMPUS_TENENDI);
    } else if (f.actio === 'tempus_dele') {
      if (this._tempus) {
        clearTimeout(this._tempus);
        this._tempus = null;
      }
    } else if (f.actio === 'orbis_aperi') {
      this._orbemAperire(f.x, f.y);
    }
  }

  _orbemAperire(clientX, clientY) {
    var ipse = this;
    var mensura = this.getBoundingClientRect();
    var x = (clientX - mensura.left) / mensura.width * 100;
    var y = (clientY - mensura.top) / mensura.height * 100;

    this._orbis = document.createElement('mensa-orbis');
    this.appendChild(this._orbis);
    this._orbis.aperire(x, y,
      [{ genus: 'theca', titulus: 'theca' },
       { genus: 'nota', titulus: 'nota' }],
      function (genus) {
        ipse._orbemClaudere();
        ipse._creareAd(genus, clientX, clientY);
      });
  }

  _orbemClaudere() {
    if (this._orbis) {
      this.removeChild(this._orbis);
      this._orbis = null;
    }
    this._arbiter.orbis_clausus();
  }

  /* ---------- creatio ---------- */

  _creareAd(genus, clientX, clientY) {
    var mensura = this.getBoundingClientRect();
    var x = Math.round((clientX - mensura.left)
      / mensura.width * 10000) / 100;
    var y = Math.round((clientY - mensura.top)
      / mensura.height * 10000) / 100;
    var id, node;

    this._ultimum = genus;
    if (genus === 'theca') {
      id = 'theca-' + Date.now().toString(36);
      node = this._nodumCreare(id,
        { genus_elementi: 'theca', titulus: 'nova', x: x, y: y });
      this.appendChild(node);
      node.ponePositum(x, y);
      node.actumMittere('creatum', {
        genus_elementi: 'theca', titulus: 'nova',
        x: x, y: y, tabula: this._tabula
      });
      node.nominare();
    } else {
      id = 'scidula-' + Date.now().toString(36);
      node = this._nodumCreare(id,
        { genus_elementi: 'scidula', textus: '', x: x, y: y });
      this.appendChild(node);
      node.ponePositum(x, y);
      node.actumMittere('creatum', {
        genus_elementi: 'scidula', textus: '',
        x: x, y: y, tabula: this._tabula
      });
      node.editionemAperire();
    }
  }

  /* emissio ex ipso plano (visum, insertum) */
  actumMittere(genus, datum) {
    this.dispatchEvent(new CustomEvent('mensa-actum', {
      bubbles: true,
      detail: { genus: genus, datum: datum }
    }));
  }
}

if (typeof customElements !== 'undefined') {
  customElements.define('mensa-planum', MensaPlanum);
}

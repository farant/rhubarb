/* repositorium.js - chartae VCS in tabula mensae (v3: lista
 * plagularum SPAWNER facta).
 *
 * Chartae declaratae:
 *   <silex-conditiones>  cauda conditionum, paginata (lista)
 *   <silex-plagulae>     plicatura ad conditionem electam (lista +
 *                        SPAWNER: click = nexus in-loco ad
 *                        contentum; TRACTUS extra listam = charta
 *                        permanens <silex-visus> nascitur - phantasma
 *                        punctorem sequitur, depositio eam creat)
 *   <silex-contentum>    visus nexus plagulae electae
 * Charta nata:
 *   <silex-visus>        visus permanens plagulae (via+seq in plica
 *                        mensae - restitutionem superstes; genus
 *                        registratum per planum.genusRegistrare)
 *
 * VISUS LINEARUM (contentum + visus): involutio mollis per
 * characterem (monospatium + break-all -> ordines involuti =
 * arithmetica pura, logica.js), mutatio magnitudinis AMBABUS axibus,
 * paginatio recomputata inter tractum ipsum. REGULA ANCORAE
 * (Franis): linea summa quam legebas ancora est - post reflexionem
 * salta ad paginam quae eam continet (etiamsi media iam iacet);
 * inter tractum linea ancorae accenditur (nisi in pagina prima).
 *
 * CAVE SELECTORIBUS (lex v2.1): tags novae fundamentum chartae hic
 * replicatum accipiunt. Pons per SILEX_VCS solum (lex DI). */

var SILEX_VCS = null;
var SILEX_VCS_EXSPECTANTES = [];

/* vocare fn cum praebitor adest (chartae natae ante initium
 * connecti possunt - rehydratio ordinem non promittit) */
function silexVcsCum(fn) {
  if (SILEX_VCS) { fn(SILEX_VCS); }
  else { SILEX_VCS_EXSPECTANTES.push(fn); }
}

var SILEX_REPOSITORIUM_STILI =
  /* fundamentum chartae replicatum (vide CAVE supra) */
  'silex-conditiones, silex-plagulae, silex-contentum,' +
  'silex-visus {' +
  '  display: block; position: absolute;' +
  '  background: var(--mensa-charta);' +
  '  border: 1px solid var(--mensa-margo);' +
  '  border-radius: 6px; padding: .9em 1.1em;' +
  '  box-shadow: var(--mensa-umbra);' +
  '  cursor: grab;' +
  '  user-select: none; -webkit-user-select: none;' +
  '}' +
  'silex-conditiones.tractans, silex-plagulae.tractans,' +
  'silex-contentum.tractans, silex-visus.tractans {' +
  '  cursor: grabbing; opacity: .92;' +
  '}' +
  'silex-conditiones.electum, silex-plagulae.electum,' +
  'silex-contentum.electum, silex-visus.electum {' +
  '  outline: 2px solid var(--mensa-accentus);' +
  '}' +
  'silex-conditiones.sectum, silex-plagulae.sectum,' +
  'silex-contentum.sectum, silex-visus.sectum {' +
  '  opacity: .45; outline-style: dashed;' +
  '}' +
  'silex-conditiones h2, silex-plagulae h2, silex-contentum h2,' +
  'silex-visus h2 {' +
  '  color: var(--mensa-textus-secundus); font-size: .85em;' +
  '  margin: 0 0 .5em 0; text-transform: uppercase;' +
  '  letter-spacing: .1em; white-space: nowrap;' +
  '  overflow: hidden; text-overflow: ellipsis;' +
  '}' +

  /* visus linearum: columna flexilis, corpus praecisum (numquam
   * volvitur - paginatur); altitudo a domino data */
  'silex-contentum, silex-visus {' +
  '  display: flex; flex-direction: column;' +
  '}' +
  'silex-contentum .corpus-linearum,' +
  'silex-visus .corpus-linearum {' +
  '  flex: 1 1 auto; min-height: 0; overflow: hidden;' +
  '}' +
  '.corpus-linearum .linea {' +
  '  font: 11px/1.5 monospace; white-space: pre-wrap;' +
  '  word-break: break-all;' +
  '  user-select: text; -webkit-user-select: text; cursor: text;' +
  '}' +
  '.corpus-linearum .linea.ancora {' +
  '  background: color-mix(in srgb, var(--mensa-accentus) 22%,' +
  '  transparent);' +
  '}' +

  /* ordines listarum */
  '.silex-vcs-res {' +
  '  padding: .25em .5em; border-radius: 4px; cursor: pointer;' +
  '}' +
  '.silex-vcs-res:hover { background: var(--mensa-fundus); }' +
  '.silex-vcs-res.electa {' +
  '  background: var(--mensa-fundus);' +
  '  border-left: 3px solid var(--mensa-accentus);' +
  '  padding-left: calc(.5em - 3px);' +
  '}' +
  '.silex-vcs-res .linea-prima {' +
  '  display: block; white-space: nowrap; overflow: hidden;' +
  '  text-overflow: ellipsis;' +
  '}' +
  '.silex-vcs-res .linea-secunda {' +
  '  display: block; color: var(--mensa-textus-secundus);' +
  '  font-size: .8em; white-space: nowrap; overflow: hidden;' +
  '  text-overflow: ellipsis;' +
  '}' +

  /* phantasma tractus (spawner): chartula punctorem sequens.
   * CAVE: intra mensa-thema appendenda - in body variabiles
   * --mensa-* extra scopum sunt (fundus evanuit, textus niger:
   * manibus Franis captum) */
  '.silex-vcs-phantasma {' +
  '  position: fixed; z-index: 1000010; pointer-events: none;' +
  '  background: var(--mensa-charta, #101216);' +
  '  color: var(--mensa-textus, #d8d4cc);' +
  '  border: 1px dashed var(--mensa-accentus, #e8a04c);' +
  '  border-radius: 6px; box-shadow: var(--mensa-umbra);' +
  '  padding: .35em .9em; font: 12px monospace; opacity: .92;' +
  '  transform: translate(-50%, -50%);' +
  '}' +

  /* pes: sagittae paginationis + status */
  '.silex-vcs-pes {' +
  '  display: flex; align-items: center; gap: .6em;' +
  '  margin-top: .5em; justify-content: center; flex: 0 0 auto;' +
  '}' +
  '.silex-vcs-pes button {' +
  '  background: none; border: 1px solid var(--mensa-margo);' +
  '  border-radius: 4px; color: var(--mensa-textus);' +
  '  font: inherit; cursor: pointer; padding: 0 .6em;' +
  '}' +
  '.silex-vcs-pes button:hover {' +
  '  border-color: var(--mensa-accentus);' +
  '  color: var(--mensa-accentus);' +
  '}' +
  '.silex-vcs-pes button:disabled {' +
  '  opacity: .3; cursor: default;' +
  '}' +
  '.silex-vcs-pes .status {' +
  '  color: var(--mensa-textus-secundus); font-size: .8em;' +
  '}' +

  '.vacuum {' +
  '  color: var(--mensa-textus-secundus); font-style: italic;' +
  '}' +

  /* claudere (visus natus): parva x in capite */
  'silex-visus .claudere {' +
  '  position: absolute; top: .2em; right: .4em;' +
  '  background: none; border: none; cursor: pointer;' +
  '  color: var(--mensa-textus-secundus); font: inherit;' +
  '}' +
  'silex-visus .claudere:hover { color: var(--mensa-accentus); }' +

  /* ansa magnitudinis */
  '.silex-vcs-ansa {' +
  '  position: absolute; right: -2px; bottom: -2px;' +
  '  width: 14px; height: 14px;' +
  '  border-right: 3px solid var(--mensa-accentus);' +
  '  border-bottom: 3px solid var(--mensa-accentus);' +
  '  border-radius: 0 0 5px 0; opacity: 0; cursor: ew-resize;' +
  '}' +
  'silex-contentum .silex-vcs-ansa,' +
  'silex-visus .silex-vcs-ansa { cursor: nwse-resize; }' +
  'silex-conditiones:hover .silex-vcs-ansa,' +
  'silex-plagulae:hover .silex-vcs-ansa,' +
  'silex-contentum:hover .silex-vcs-ansa,' +
  'silex-visus:hover .silex-vcs-ansa { opacity: .85; }';

/* fundamentum commune: caput + corpus + pes + ansa. Subclasses
 * _anteire/_posterire implent; axes ansae _ansaAxes regit. */
class SilexChartaVcs extends MensaScida {
  connectedCallback() {
    var ipse;
    if (this._paratum) { return; }
    super.connectedCallback();
    ipse = this;

    this._titulus = document.createElement('h2');
    this._titulus.textContent = this._titulusPrimus();
    this.appendChild(this._titulus);
    this._corpus = document.createElement('div');
    this.appendChild(this._corpus);

    this._pes = document.createElement('div');
    this._pes.className = 'silex-vcs-pes';
    this._ante = document.createElement('button');
    this._ante.textContent = '‹';
    this._ante.addEventListener('click', function () {
      ipse._anteire();
    });
    this._statusPes = document.createElement('span');
    this._statusPes.className = 'status';
    this._post = document.createElement('button');
    this._post.textContent = '›';
    this._post.addEventListener('click', function () {
      ipse._posterire();
    });
    this._pes.appendChild(this._ante);
    this._pes.appendChild(this._statusPes);
    this._pes.appendChild(this._post);
    this.appendChild(this._pes);

    this._ansa = document.createElement('div');
    this._ansa.className = 'silex-vcs-ansa';
    this._ansa.addEventListener('pointerdown',
      this._magnitudinemPrehendere.bind(this));
    this.appendChild(this._ansa);
  }

  _titulusPrimus() { return 'vcs'; }
  _ansaAxes() { return 'x'; }
  _anteire() {}
  _posterire() {}
  _remensa() {}   /* visus linearum inter tractum recomputat */

  titulumPonere(textus) {
    if (this._titulus) { this._titulus.textContent = textus; }
  }

  nuntiumMonstrare(textus) {
    var d = document.createElement('div');

    this._corpus.innerHTML = '';
    d.className = 'vacuum';
    d.textContent = textus;
    this._corpus.appendChild(d);
    this._pes.style.display = 'none';
  }

  _pedemPonere(pagina, paginae) {
    this._pes.style.display = paginae > 1 ? 'flex' : 'none';
    this._statusPes.textContent = (pagina + 1) + '/' + paginae;
    this._ante.disabled = pagina === 0;
    this._post.disabled = pagina + 1 >= paginae;
  }

  latitudinemPonere(pct) {
    this.style.width = pct + '%';
    this._remensa();
  }

  altitudinemPonere(pct) {
    this.style.height = pct + '%';
    this._remensa();
  }

  _magnitudinemPrehendere(e) {
    var ipse = this;
    var mensura, initX, initY, origoLat, origoAlt;
    var movere, solvere;

    e.stopPropagation();
    if (!this.parentElement) { return; }
    mensura = this.parentElement.getBoundingClientRect();
    if (mensura.width === 0 || mensura.height === 0) { return; }
    initX = e.clientX;
    initY = e.clientY;
    origoLat = this.getBoundingClientRect().width
      / mensura.width * 100;
    origoAlt = this.getBoundingClientRect().height
      / mensura.height * 100;
    this._inTractu = true;
    this._ansa.setPointerCapture(e.pointerId);

    movere = function (ev) {
      var lat = Math.max(12, Math.min(90,
        origoLat + (ev.clientX - initX) / mensura.width * 100));

      ipse.style.width = lat + '%';
      if (ipse._ansaAxes() === 'xy') {
        var alt = Math.max(12, Math.min(92,
          origoAlt + (ev.clientY - initY)
            / mensura.height * 100));

        ipse.style.height = alt + '%';
      }
      ipse._remensa();
    };
    solvere = function () {
      var emissum;

      ipse._ansa.removeEventListener('pointermove', movere);
      ipse._ansa.removeEventListener('pointerup', solvere);
      ipse._ansa.removeEventListener('pointercancel', solvere);
      ipse._inTractu = false;
      ipse._remensa();
      emissum = {
        latitudo: Math.round(ipse.getBoundingClientRect().width
          / mensura.width * 10000) / 100
      };
      if (ipse._ansaAxes() === 'xy') {
        emissum.altitudo = Math.round(
          ipse.getBoundingClientRect().height
            / mensura.height * 10000) / 100;
      }
      ipse.actumMittere('magnitudo', emissum);
    };
    this._ansa.addEventListener('pointermove', movere);
    this._ansa.addEventListener('pointerup', solvere);
    this._ansa.addEventListener('pointercancel', solvere);
  }
}

/* ---------- chartae listarum (conditiones, plagulae) ---------- */

class SilexChartaListae extends SilexChartaVcs {
  connectedCallback() {
    if (this._paratum) { return; }
    super.connectedCallback();
    this.ordo = [];
    this.paginator = new MensaPaginator(this._quantum());
  }

  _quantum() { return 8; }

  ordinemPonere(ordo) {
    this.ordo = ordo;
    this.paginator.pagina = 0;
    this._reddere();
  }

  _anteire() {
    if (this.paginator.ante()) { this._reddere(); }
  }
  _posterire() {
    if (this.paginator.post(this.ordo.length)) { this._reddere(); }
  }

  _reddere() {
    var fenestra = this.paginator.fenestra(this.ordo);
    var index;

    this._corpus.innerHTML = '';
    for (index = 0; index < fenestra.length; index = index + 1) {
      this._corpus.appendChild(this._resCreare(fenestra[index]));
    }
    this._pedemPonere(this.paginator.pagina,
      this.paginator.paginae(this.ordo.length));
  }

  /* ordo unius rei: click = electio; pointerdown sistitur (charta
   * capite trahitur) - subclasses tractum proprium addere possunt */
  _resCreare(res) {
    var ipse = this;
    var d = document.createElement('div');
    var l1 = document.createElement('span');
    var l2 = document.createElement('span');

    d.className = 'silex-vcs-res';
    l1.className = 'linea-prima';
    l1.textContent = this._lineaPrima(res);
    l2.className = 'linea-secunda';
    l2.textContent = this._lineaSecunda(res);
    d.appendChild(l1);
    d.appendChild(l2);
    this._resArmare(d, res);
    return d;
  }

  _resArmare(d, res) {
    var ipse = this;

    d.addEventListener('pointerdown', function (e) {
      e.stopPropagation();
    });
    d.addEventListener('click', function () {
      ipse._electio(res, d);
    });
  }

  _lineaPrima(res) { return String(res); }
  _lineaSecunda(res) { return ''; }
  _electio(res, elementum) {}

  _electamSignare(elementum) {
    var index;
    var omnes = this._corpus.querySelectorAll('.silex-vcs-res');

    for (index = 0; index < omnes.length; index = index + 1) {
      omnes[index].classList.toggle('electa',
        omnes[index] === elementum);
    }
  }
}

class SilexConditiones extends SilexChartaListae {
  _titulusPrimus() { return 'conditiones'; }
  _quantum() { return 6; }

  onerare() {
    var ipse = this;

    silexVcsCum(function (vcs) {
      vcs.historia()
        .then(function (r) {
          var ordo = r.ordo.slice().reverse();

          ordo.unshift({ seq: 0, nuntius: 'arbor praesens',
            momentum: '(plica manifesti hodierna)', tactae: null });
          ipse.ordinemPonere(ordo);
          ipse._electio(ordo[0], ipse._corpus.firstChild);
        })
        .catch(function () {
          ipse.nuntiumMonstrare('proiectum sub cwd non inventum -'
            + ' aperi silex e directorio proiecti');
        });
    });
  }

  _lineaPrima(c) {
    return c.seq === 0 ? c.nuntius : '[' + c.seq + '] ' + c.nuntius;
  }
  _lineaSecunda(c) {
    return c.tactae === null ? c.momentum
      : c.momentum + ' · ' + c.tactae + ' plagulae';
  }
  _electio(c, elementum) {
    this._electamSignare(elementum);
    document.dispatchEvent(new CustomEvent('vcs-conditio-electa', {
      detail: { seq: c.seq }
    }));
  }
}

class SilexPlagulae extends SilexChartaListae {
  connectedCallback() {
    var ipse;
    if (this._paratum) { return; }
    super.connectedCallback();
    ipse = this;
    this._seq = 0;
    this.nuntiumMonstrare('elige conditionem');
    document.addEventListener('vcs-conditio-electa', function (e) {
      ipse._onerare(e.detail.seq);
    });
  }

  _titulusPrimus() { return 'plagulae'; }
  _quantum() { return 9; }

  _onerare(seq) {
    var ipse = this;

    this._seq = seq;
    this.titulumPonere(seq === 0 ? 'plagulae · praesens'
      : 'plagulae · seq ' + seq);
    silexVcsCum(function (vcs) {
      vcs.plica(seq)
        .then(function (r) { ipse.ordinemPonere(r.ordo); })
        .catch(function (e) {
          ipse.nuntiumMonstrare('CULPA: ' + e.message);
        });
    });
  }

  _lineaPrima(p) { return p.via; }
  _lineaSecunda(p) { return p.origo; }
  _electio(p, elementum) {
    this._electamSignare(elementum);
    document.dispatchEvent(new CustomEvent('vcs-plagula-electa', {
      detail: { seq: this._seq, via: p.via }
    }));
  }

  /* SPAWNER: pointerdown + motus > limen = phantasma punctorem
   * sequitur; depositio chartam <silex-visus> permanentem creat
   * (planum.chartamCreare - in plicam, restitutionem superstes).
   * Sine motu = click = nexus in-loco ut supra. */
  _resArmare(d, res) {
    var ipse = this;

    d.addEventListener('pointerdown', function (e) {
      var initX = e.clientX;
      var initY = e.clientY;
      var phantasma = null;
      var movere, solvere;

      e.stopPropagation();
      d.setPointerCapture(e.pointerId);
      movere = function (ev) {
        var dx = ev.clientX - initX;
        var dy = ev.clientY - initY;

        if (!phantasma) {
          if (dx * dx + dy * dy < 36) { return; }   /* limen 6pt */
          phantasma = document.createElement('div');
          phantasma.className = 'silex-vcs-phantasma';
          phantasma.textContent = res.via;
          /* intra thema (vide CAVE stilorum); fixum manet -
           * positio a fenestra, variabiles a themate */
          (ipse.closest('mensa-thema') || document.body)
            .appendChild(phantasma);
        }
        phantasma.style.left = ev.clientX + 'px';
        phantasma.style.top = ev.clientY + 'px';
      };
      solvere = function (ev) {
        d.removeEventListener('pointermove', movere);
        d.removeEventListener('pointerup', solvere);
        d.removeEventListener('pointercancel', solvere);
        if (!phantasma) {
          ipse._electio(res, d);   /* click purus */
          return;
        }
        if (phantasma.parentElement) {
          phantasma.parentElement.removeChild(phantasma);
        }
        var planum = ipse.closest('mensa-planum');

        if (planum && planum.chartamCreare) {
          planum.chartamCreare({
            genus_elementi: 'silex-visus',
            via: res.via, seq: ipse._seq,
            latitudo: 30, altitudo: 42
          }, ev.clientX, ev.clientY);
        }
      };
      d.addEventListener('pointermove', movere);
      d.addEventListener('pointerup', solvere);
      d.addEventListener('pointercancel', solvere);
    });
  }
}

/* ---------- visus linearum (contentum + visus natus) ---------- */

class SilexVisusLinearum extends SilexChartaVcs {
  connectedCallback() {
    if (this._paratum) { return; }
    super.connectedCallback();
    this._corpus.className = 'corpus-linearum';
    this._lineae = [];
    this._initia = [0];
    this._pagina = 0;
    this._ancora = 0;
    this._charLat = 0;
    this._lineaAlt = 0;
    /* altitudo NECESSARIA: columna flexilis sine altitudine data
     * corpus ad contentum collabitur - pagina unius ordinis,
     * plagula in trecentas paginas fracta. Ordinaria hic, plica
     * per altitudinemPonere superscribit. */
    if (!this.style.height) {
      this.style.height = (this.getAttribute('altitudo') || '42')
        + '%';
    }
    if (!this.style.width) {
      this.style.width = (this.getAttribute('latitudo') || '30')
        + '%';
    }
  }

  _ansaAxes() { return 'xy'; }

  lineasPonere(textus) {
    this._lineae = textus.split('\n');
    this._ancora = 0;
    this._pagina = 0;
    this._recomputare();
  }

  /* metra monospatii semel (probe decem M) */
  _metiri() {
    var probe = document.createElement('div');

    probe.className = 'linea';
    probe.style.position = 'absolute';
    probe.style.visibility = 'hidden';
    probe.textContent = 'MMMMMMMMMM';
    this._corpus.appendChild(probe);
    this._charLat = probe.getBoundingClientRect().width / 10;
    this._lineaAlt = probe.getBoundingClientRect().height;
    this._corpus.removeChild(probe);
  }

  _recomputare() {
    var columnae, ordinesPaginae, ordines, index;

    if (this._lineae.length === 0) { return; }
    if (this._charLat === 0) { this._metiri(); }
    if (this._charLat === 0) { return; }
    columnae = Math.max(8,
      Math.floor(this._corpus.clientWidth / this._charLat));
    ordinesPaginae = Math.max(1,
      Math.floor(this._corpus.clientHeight / this._lineaAlt));
    ordines = [];
    for (index = 0; index < this._lineae.length;
        index = index + 1) {
      ordines.push(mensaOrdinesLineae(this._lineae[index].length,
        columnae));
    }
    this._initia = mensaPartitioLinearum(ordines, ordinesPaginae);
    this._pagina = mensaPaginaAncorae(this._initia, this._ancora);
    this._reddereLineas();
  }

  _remensa() {
    var ipse = this;

    if (this._remensaPetita) { return; }
    this._remensaPetita = true;
    requestAnimationFrame(function () {
      ipse._remensaPetita = false;
      ipse._recomputare();
    });
  }

  _reddereLineas() {
    var a = this._initia[this._pagina];
    var b = this._pagina + 1 < this._initia.length
      ? this._initia[this._pagina + 1] : this._lineae.length;
    var index, d;

    this._corpus.innerHTML = '';
    for (index = a; index < b; index = index + 1) {
      d = document.createElement('div');
      d.className = 'linea';
      d.textContent = this._lineae[index] === '' ? ' '
        : this._lineae[index];
      /* regula ancorae: inter tractum linea tua accenditur (nisi
       * prima pagina - ibi nihil amitti potest) */
      if (this._inTractu && index === this._ancora
          && this._ancora > 0) {
        d.classList.add('ancora');
      }
      d.addEventListener('pointerdown', function (e) {
        e.stopPropagation();   /* textus eligibilis, non tractus */
      });
      this._corpus.appendChild(d);
    }
    this._pedemPonere(this._pagina, this._initia.length);
  }

  _anteire() {
    if (this._pagina > 0) {
      this._pagina = this._pagina - 1;
      this._ancora = this._initia[this._pagina];
      this._reddereLineas();
    }
  }
  _posterire() {
    if (this._pagina + 1 < this._initia.length) {
      this._pagina = this._pagina + 1;
      this._ancora = this._initia[this._pagina];
      this._reddereLineas();
    }
  }

  _onerarePlagulam(seq, via) {
    var ipse = this;

    this.titulumPonere(via + ' · '
      + (seq === 0 ? 'praesens' : 'seq ' + seq));
    silexVcsCum(function (vcs) {
      vcs.plagula(seq, via)
        .then(function (r) { ipse.lineasPonere(r.contentum); })
        .catch(function (e) {
          ipse.nuntiumMonstrare('CULPA: ' + e.message);
        });
    });
  }
}

/* visus nexus: plagulam electam sequitur (in-loco) */
class SilexContentum extends SilexVisusLinearum {
  connectedCallback() {
    var ipse;
    if (this._paratum) { return; }
    super.connectedCallback();
    ipse = this;
    this.nuntiumMonstrare('elige plagulam');
    document.addEventListener('vcs-plagula-electa', function (e) {
      ipse._onerarePlagulam(e.detail.seq, e.detail.via);
    });
  }

  _titulusPrimus() { return 'contentum'; }
}

/* visus natus (spawner): plagula UNA fixa, permanens in plica -
 * attributa via/seq a factore, restitutio eadem via */
class SilexVisus extends SilexVisusLinearum {
  connectedCallback() {
    var ipse;
    if (this._paratum) { return; }
    super.connectedCallback();
    ipse = this;

    this._claudere = document.createElement('button');
    this._claudere.className = 'claudere';
    this._claudere.textContent = '×';
    this._claudere.addEventListener('click', function () {
      ipse.actumMittere('deletum', {});
    });
    this.appendChild(this._claudere);

    this._onerarePlagulam(
      parseInt(this.getAttribute('seq') || '0', 10),
      this.getAttribute('via') || '');
  }

  _titulusPrimus() { return this.getAttribute('via') || 'visus'; }
}

if (typeof customElements !== 'undefined') {
  customElements.define('silex-conditiones', SilexConditiones);
  customElements.define('silex-plagulae', SilexPlagulae);
  customElements.define('silex-contentum', SilexContentum);
  customElements.define('silex-visus', SilexVisus);
}

/* initium: stilos inicere, genus natum registrare, praebitorem
 * ponere (exspectantes fluunt), onerationem incipere */
function silexVcs(praebitor) {
  var stili, planum, conditiones, index;

  if (!document.getElementById('silex-repositorium-stili')) {
    stili = document.createElement('style');
    stili.id = 'silex-repositorium-stili';
    stili.textContent = SILEX_REPOSITORIUM_STILI;
    document.head.appendChild(stili);
  }
  planum = document.querySelector('mensa-planum');
  if (planum && planum.genusRegistrare) {
    planum.genusRegistrare('silex-visus', function (id, datum) {
      var el = document.createElement('silex-visus');

      el.setAttribute('via', datum.via || '');
      el.setAttribute('seq', String(datum.seq || 0));
      /* magnitudo in ortu - creatum non re-reddit, ergo nodus
       * vivus attributis nascitur (plica easdem servat) */
      if (typeof datum.latitudo === 'number') {
        el.setAttribute('latitudo', String(datum.latitudo));
      }
      if (typeof datum.altitudo === 'number') {
        el.setAttribute('altitudo', String(datum.altitudo));
      }
      return el;
    });
  }
  SILEX_VCS = praebitor;
  for (index = 0; index < SILEX_VCS_EXSPECTANTES.length;
      index = index + 1) {
    SILEX_VCS_EXSPECTANTES[index](praebitor);
  }
  SILEX_VCS_EXSPECTANTES = [];
  conditiones = document.querySelector('silex-conditiones');
  if (conditiones && conditiones.onerare) { conditiones.onerare(); }
}

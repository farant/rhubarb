/* componentia.js - BIBLIOTHECA componentium (stratum commune tertium).
 *
 * NATA 0008: consumens marcationis solius componentem ferre non
 * potest, ergo componentia alicubi COMMUNI vivere debent. Stratum
 * commune hactenus signa sola erat (thema.css); nunc tria:
 *
 *   thema.css        signa    - mutatum thema mutare DEBET
 *   componentia.*    mores    - vitium sanatum consumentes omnes
 *                               sanare DEBET (id est bibliotheca)
 *   experimenta/     archivum - IMMOTA, numquam 'meliorata'
 *
 * Discrimen: bibliotheca VIVIT et interfaciem stabilem debet
 * (Eskil: interfaciem primam recte, implementationem semper emendare
 * licet); experimenta congelantur. Consumens marcationis vetus
 * probationem fumi habet, ergo mutatio bibliothecae quae eum
 * frangit CAPITUR - archivum suita regressionis bibliothecae fit.
 *
 * FIGERE PER SCRUTATIONEM, non per customElements: attributum
 * 'data-componens' in elemento QUOLIBET - nulla coactio nominis
 * (kebab), nulla montatio magica per innerHTML, ES5 purum.
 * (Elementa consueta - customElements.define - furca futura si
 * cyclus vitae sponte necessarius fiet.)
 *
 * CONTRACTUS CONSUMENTIS (arbor):
 *   <div data-componens="arbor" data-fons="#sel-insulae"
 *        [data-numeri="verum"] [data-remontabile="verum"]>
 *     [<script type="text/stml-ephemera">...</script>]  (semen optionale)
 *   </div>
 *   <script type="text/stml" id="...">...arbor STML...</script>
 *
 * Mores ordinarii NIHIL JS a consumente poscunt - id est propositum.
 * TRIA GENERA STATUS (0007): durabilis in insula fontis; ephemerus
 * in insula intra elementum (creatur si abest; semen honoratur);
 * kineticus (volutio) in eadem insula, scriptus in QUIETE.
 */

window.componentia = (function () {

  /* ============================================================
   * PARSER STML MINIMUS (ex 0007 in bibliothecam levatus)
   * ============================================================ */
  function insulam_legere(textus) {
    var pos = 0;
    var radix = null;
    var acervus = [];

    function culpa(cur) {
      throw new Error('insula legi non potuit (' + cur + ', pos '
                      + pos + ')');
    }
    function albus() {
      while (pos < textus.length && textus.charAt(pos) <= ' ') { pos++; }
    }
    function verbum() {
      var a = pos;
      while (pos < textus.length
             && /[A-Za-z0-9_.-]/.test(textus.charAt(pos))) { pos++; }
      if (pos === a) { culpa('verbum vacuum'); }
      return textus.substring(a, pos);
    }

    albus();
    while (pos < textus.length) {
      if (textus.charAt(pos) !== '<') { culpa('exspectatum <'); }
      pos++;
      if (textus.charAt(pos) === '/') {
        pos++;
        verbum();
        if (textus.charAt(pos) !== '>') { culpa('exspectatum >'); }
        pos++;
        if (!acervus.length) { culpa('clausura sine apertura'); }
        acervus.pop();
      } else {
        var nomen_tag = verbum();
        var attributa = {};
        albus();
        while (textus.charAt(pos) !== '>' && textus.charAt(pos) !== '/') {
          var clavis = verbum();
          if (textus.charAt(pos) !== '=') { culpa('exspectatum ='); }
          pos++;
          if (textus.charAt(pos) !== '"') { culpa('exspectatum "'); }
          pos++;
          var a = pos;
          while (pos < textus.length && textus.charAt(pos) !== '"') { pos++; }
          attributa[clavis] = textus.substring(a, pos);
          pos++;
          albus();
        }
        var clausum = (textus.charAt(pos) === '/');
        if (clausum) { pos++; }
        if (textus.charAt(pos) !== '>') { culpa('exspectatum >'); }
        pos++;

        var pater = acervus.length ? acervus[acervus.length - 1] : null;
        var nodus;
        if (nomen_tag === 'arbor') {
          if (radix) { culpa('arbor duplex'); }
          radix = { electum: attributa.electum || '', filii: [] };
          nodus = radix;
        } else {
          if (!pater) { culpa('nodus extra arborem'); }
          nodus = {
            genus: (nomen_tag === 'dir') ? 'dir' : 'plagula',
            nomen: attributa.nomen || '',
            apertus: attributa.apertus === 'verum',
            mensura: parseInt(attributa.mensura || '0', 10),
            gradus: acervus.length - 1,
            via: (pater === radix ? '' : pater.via + '/')
                 + (attributa.nomen || ''),
            filii: (nomen_tag === 'dir') ? [] : null
          };
          pater.filii.push(nodus);
        }
        if (!clausum && nomen_tag !== 'plagula') { acervus.push(nodus); }
      }
      albus();
    }
    if (acervus.length) { culpa('apertura sine clausura'); }
    if (!radix) { culpa('arbor abest'); }
    return radix;
  }

  function altitudoOrdinis() {
    var v = getComputedStyle(document.documentElement)
              .getPropertyValue('--ordo-altitudo');
    var n = parseInt(v, 10);
    return (n > 0) ? n : 24;
  }

  /* ============================================================
   * PARSER STML GENERALIS - arbor nodorum {tag, att, filii}
   * ============================================================
   * Sine semantica: quisque componens arborem suam interpretatur.
   * (Migrationes 0010/0011: parser arboris supra proprius est;
   * hic generalis pro componentibus novis - candidatus strati
   * adiutorum, arbor postea huc migranda.) */
  function _stml_generale_legere(textus) {
    var pos = 0;
    var radix = { tag: '(radix)', att: {}, filii: [] };
    var acervus = [radix];

    function culpa(cur) {
      throw new Error('insula legi non potuit (' + cur + ', pos '
                      + pos + ')');
    }
    function albus() {
      while (pos < textus.length && textus.charAt(pos) <= ' ') { pos++; }
    }
    function verbum() {
      var a = pos;
      while (pos < textus.length
             && /[A-Za-z0-9_.-]/.test(textus.charAt(pos))) { pos++; }
      if (pos === a) { culpa('verbum vacuum'); }
      return textus.substring(a, pos);
    }

    albus();
    while (pos < textus.length) {
      if (textus.charAt(pos) !== '<') {
        /* TEXTUS filius (0021 - notae marginalium): cursus ad
           '<' colligitur in nodi '.textus', entitates solutae
           (&amp; POSTREMA ne effugium bis solvatur). Alba
           extrema a structura tonduntur (albus() inter tags);
           alba INTERNA - lineae plures notae - verbatim vivunt. */
        var a3 = pos;
        var n3 = acervus[acervus.length - 1];
        while (pos < textus.length && textus.charAt(pos) !== '<') {
          pos++;
        }
        n3.textus = (n3.textus || '')
          + textus.substring(a3, pos)
            .replace(/&lt;/g, '<').replace(/&gt;/g, '>')
            .replace(/&quot;/g, '"').replace(/&amp;/g, '&');
        continue;
      }
      pos++;
      if (textus.charAt(pos) === '/') {
        pos++;
        verbum();
        if (textus.charAt(pos) !== '>') { culpa('exspectatum >'); }
        pos++;
        if (acervus.length < 2) { culpa('clausura sine apertura'); }
        acervus.pop();
      } else {
        var nodus = { tag: verbum(), att: {}, filii: [] };
        albus();
        while (textus.charAt(pos) !== '>' && textus.charAt(pos) !== '/') {
          var clavis = verbum();
          if (textus.charAt(pos) !== '=') { culpa('exspectatum ='); }
          pos++;
          if (textus.charAt(pos) !== '"') { culpa('exspectatum "'); }
          pos++;
          var a = pos;
          while (pos < textus.length && textus.charAt(pos) !== '"') { pos++; }
          nodus.att[clavis] = textus.substring(a, pos);
          pos++;
          albus();
        }
        var clausum = (textus.charAt(pos) === '/');
        if (clausum) { pos++; }
        if (textus.charAt(pos) !== '>') { culpa('exspectatum >'); }
        pos++;
        acervus[acervus.length - 1].filii.push(nodus);
        if (!clausum) { acervus.push(nodus); }
      }
      albus();
    }
    if (acervus.length !== 1) { culpa('apertura sine clausura'); }
    if (!radix.filii.length) { culpa('insula vacua'); }
    return radix.filii[0];
  }

  /* Attributa citata serere (adiutor scriptorum insularum). */
  function _att_serere(att, ordo) {
    var s = '', i;
    for (i = 0; i < ordo.length; i++) {
      s += ' ' + ordo[i] + '="' + (att[ordo[i]] || '') + '"';
    }
    return s;
  }

  /* ============================================================
   * STRATUM ADIUTORUM (0017) - INSTRUMENTA, NON COMPAGES
   * ============================================================
   * Fabricae adiutores VOCANT; nihil umquam fabricam vocat (idem
   * mos quo atrium: gyrus fabricae manet). Privationes liberae:
   * tabella kinetica caret, optio virtualizatione, color
   * ephemeris - iure omnes, nulla vexilla.
   *
   * GRADUATA (evidentia n=6): resolutio fontis + culpa, codex
   * ephemerarum (inveni-aut-crea semine honorato + genera
   * camporum), nuntius status, eventus 'electum'.
   * DILATA nominatim (variantes MENSURATAE, non uniendae ante
   * consumentem septimum): pictura virtualis (arbor arborea,
   * index plana), regio cribri (index appendit / optio reponit /
   * arbor ictu commutat), par disciplinarum kineticarum (color
   * manu, harmonia transcripta - mathematica iam communis). */

  /* culpa in ELEMENTO - quod usor non videt probatio non videt */
  function _culpam_ponere(el, nuntius) {
    el.innerHTML = '<p class="ca-culpa">CULPA: ' + nuntius + '</p>';
  }

  /* fons durabilis: resolutio data-fons + recusationes clarae */
  function _fontem_resolvere(el) {
    var sel = el.getAttribute('data-fons');
    if (!sel) {
      _culpam_ponere(el, 'data-fons abest');
      return null;
    }
    var insula = document.querySelector(sel);
    if (!insula) {
      _culpam_ponere(el, 'fons non inventus: ' + sel);
      return null;
    }
    return insula;
  }

  /* codex insulae ephemerarum: inveni-aut-crea (semen consumentis
     honoratur - lectio 0009b), campi typati.
     campi = [{titulus, genus 'c'|'n'|'b', praefinitum}] ordine
     serendi; legere() obiectum typatum, scribere(obiectum) formam
     canonicam ' k="v"' (eandem quam manus veteres - paritas
     octetorum). */
  function _ephemeras_parare(el, campi) {
    var insula = null;
    var l = el.querySelectorAll('script[type="text/stml-ephemera"]');
    if (l.length) { insula = l[0]; }
    else {
      insula = document.createElement('script');
      insula.type = 'text/stml-ephemera';
      var att = {}, ordo = [], i;
      for (i = 0; i < campi.length; i++) {
        att[campi[i].titulus] = campi[i].praefinitum;
        ordo.push(campi[i].titulus);
      }
      insula.textContent = '<ephemera' + _att_serere(att, ordo) + '/>';
      el.appendChild(insula);
    }
    function legere() {
      var t = insula.textContent, e = {}, i, c, m, v;
      for (i = 0; i < campi.length; i++) {
        c = campi[i];
        m = t.match(new RegExp(c.titulus + '="([^"]*)"'));
        v = m ? m[1] : c.praefinitum;
        if (c.genus === 'n') { e[c.titulus] = parseInt(v || '0', 10) || 0; }
        else if (c.genus === 'b') { e[c.titulus] = v === 'verum'; }
        else { e[c.titulus] = v; }
      }
      return e;
    }
    function scribere(e) {
      var att = {}, ordo = [], i, c, v;
      for (i = 0; i < campi.length; i++) {
        c = campi[i];
        v = e[c.titulus];
        if (c.genus === 'n') { v = String(v); }
        else if (c.genus === 'b') { v = v ? 'verum' : 'falsum'; }
        att[c.titulus] = v;
        ordo.push(c.titulus);
      }
      insula.textContent = '<ephemera' + _att_serere(att, ordo) + '/>';
    }
    return { legere: legere, scribere: scribere };
  }

  /* nuntius status crustae (in PORTIS vocandus - lectio 0007) */
  function _nuntiare_statum() {
    if (window.laboratorium && laboratorium.status_nuntiare) {
      laboratorium.status_nuntiare();
    }
  }

  /* eventus exitus componentis idiomaticus (unificatus 0017 -
     divergentia 0013 graduata: arbor quoque spargit) */
  function _eventum_electum(el, detail) {
    el.dispatchEvent(new CustomEvent('electum', {
      bubbles: true,
      detail: detail
    }));
  }

  /* ============================================================
   * COMPOSITOR STRATORUM (0022) - purus
   * ============================================================
   * (longitudo, tecta) -> segmenta: strata tectorum superposita
   * ad OMNES fines secantur; quodque segmentum acervum classium
   * ORDINE tectorum fert (ordo stratorum = ordo picturae) et
   * attributa ordine fusa (posterius vincit). Disiunctio regula
   * AUCTORIS strati singuli est (lumina inter se recusant), non
   * praesumptio pictoris - strata diversa (color syntaxis,
   * diagnostica, cautiones) libere superponuntur.
   *
   * SEMANTICA, NON COLORES: segmenta classes ferunt; colores in
   * CSS vivunt (variabiles cum praefinitis - themabile ab alto).
   * PURUS: sine DOM, sine statu - oracula unitatis directa.
   * Tecta extra [0, longitudo) tonduntur; segmenta plana (sine
   * classibus) spatia intecta tegunt - totum [0, longitudo)
   * semper redditur. */
  /* columnae OCTETORUM (decretum 01M0ATF1E1: C octetos emittit)
     -> indices chordae JS (UTF-16) - conversio ad limitem, JS se
     flectit, numquam C. ASCII identitas; ultra id ambulatio
     punctorum codicis (surrogata = 2 indices, 4 octeti). */
  function _octeti_ad_indices(linea_t, ab_oct, ad_oct) {
    var i = 0, oct = 0, cp, ab = -1, ad = -1;
    while (true) {
      if (ab < 0 && oct >= ab_oct) { ab = i; }
      if (oct >= ad_oct) { ad = i; break; }
      if (i >= linea_t.length) {
        if (ab < 0) { ab = i; }
        ad = i;
        break;
      }
      cp = linea_t.codePointAt(i);
      oct += cp < 0x80 ? 1 : cp < 0x800 ? 2 : cp < 0x10000 ? 3 : 4;
      i += cp < 0x10000 ? 1 : 2;
    }
    return { ab: ab, ad: ad };
  }

  /* conversio reversa - index chordae JS (UTF-16) -> columna
     octetorum. Ancorae CONDITAE octetos ferunt (decretum
     01M0ATF1E1), sed electio machinae in indicibus DOM vivit:
     limes creationis hic est, ut limes picturae supra. */
  function _octeti_ex_indice(linea_t, index) {
    var i = 0, oct = 0, cp;
    while (i < index && i < linea_t.length) {
      cp = linea_t.codePointAt(i);
      oct += cp < 0x80 ? 1 : cp < 0x800 ? 2 : cp < 0x10000 ? 3 : 4;
      i += cp < 0x10000 ? 1 : 2;
    }
    return oct;
  }

  function stratificare(longitudo, tecta) {
    var fines = [0, longitudo];
    var unica = [];
    var segmenta = [];
    var i, k, cl, t, ab, ad, seg;
    for (i = 0; i < tecta.length; i++) {
      ab = tecta[i].ab < 0 ? 0 : tecta[i].ab;
      ad = tecta[i].ad > longitudo ? longitudo : tecta[i].ad;
      if (ab < longitudo) { fines.push(ab); }
      if (ad > 0) { fines.push(ad); }
    }
    fines.sort(function (x, y) { return x - y; });
    for (i = 0; i < fines.length; i++) {
      if (!unica.length || unica[unica.length - 1] !== fines[i]) {
        unica.push(fines[i]);
      }
    }
    for (k = 0; k + 1 < unica.length; k++) {
      seg = { ab: unica[k], ad: unica[k + 1],
              classes: [], attributa: {} };
      if (seg.ab >= seg.ad) { continue; }
      for (i = 0; i < tecta.length; i++) {
        t = tecta[i];
        ab = t.ab < 0 ? 0 : t.ab;
        ad = t.ad > longitudo ? longitudo : t.ad;
        if (ab <= seg.ab && seg.ad <= ad) {
          seg.classes.push(t.classis);
          if (t.attributa) {
            for (cl in t.attributa) {
              seg.attributa[cl] = t.attributa[cl];
            }
          }
        }
      }
      segmenta.push(seg);
    }
    return segmenta;
  }

  /* ============================================================
   * ARBOR - componens primus bibliothecae (ex 0007 transcriptus)
   * ============================================================ */
  function arborem_figere(el) {
    var numeri_visibiles = (el.getAttribute('data-numeri') === 'verum');
    var remontabile = (el.getAttribute('data-remontabile') === 'verum');

    var insula_d = _fontem_resolvere(el);
    if (!insula_d) { return null; }

    var insulae_e = _ephemeras_parare(el, [
      { titulus: 'electio',   genus: 'c', praefinitum: '' },
      { titulus: 'quaesitum', genus: 'c', praefinitum: '' },
      { titulus: 'volutio',   genus: 'n', praefinitum: '0' }
    ]);

    var lectiones    = 0;
    var scripturae_d = 0;
    var scripturae_e = 0;
    var picturae     = 0;

    var graphus  = null;
    var ephemera = null;
    var NODI     = 0;
    var plana    = [];
    var electus  = 0;

    var proiectio  = null;
    var prospectus = null;
    var spatium    = null;
    var ordinesEl  = null;
    var campus     = null;
    var numeriEl   = null;
    var ALT        = 24;
    var MARGO      = 4;
    var quies_index = null;

    function ephemeram_legere() { return insulae_e.legere(); }
    function ephemeram_scribere() {
      insulae_e.scribere(ephemera);
      scripturae_e = scripturae_e + 1;
    }
    function insulam_durabilem_scribere() {
      var l = [];
      l.push('<arbor electum="' + graphus.electum + '">');
      (function ire(nodi, ind) {
        var i, n;
        for (i = 0; i < nodi.length; i++) {
          n = nodi[i];
          if (n.genus === 'dir') {
            l.push(ind + '<dir nomen="' + n.nomen + '" apertus="'
                   + (n.apertus ? 'verum' : 'falsum') + '">');
            ire(n.filii, ind + '  ');
            l.push(ind + '</dir>');
          } else {
            l.push(ind + '<plagula nomen="' + n.nomen + '"'
                   + ' mensura="' + n.mensura + '"/>');
          }
        }
      }(graphus.filii, '  '));
      l.push('</arbor>');
      insula_d.textContent = l.join('\n');
      scripturae_d = scripturae_d + 1;
    }

    /* Instrumenta in PORTIS pinguntur, non in pictura sola -
       lectio 0007: scriptura quietis extra scaenam accidit et
       instrumentum quod cum scaena solum repingitur mentitur. */
    function numeros_pingere() {
      if (!numeriEl) { return; }
      numeriEl.textContent =
          'nodi: ' + NODI
        + '  visibiles: ' + plana.length
        + '  lectiones: ' + lectiones
        + '  scripturae-d: ' + scripturae_d
        + '  scripturae-e: ' + scripturae_e
        + '  picturae: ' + picturae;
    }

    function mutare_durabile(f) {
      f(graphus);
      insulam_durabilem_scribere();
      numeros_pingere();
      _nuntiare_statum();
    }
    function mutare_ephemera(f) {
      f(ephemera);
      ephemeram_scribere();
      numeros_pingere();
      _nuntiare_statum();
    }

    function planare() {
      plana = [];
      if (ephemera.quaesitum !== '') {
        (function ire(nodi) {
          var i, n;
          for (i = 0; i < nodi.length; i++) {
            n = nodi[i];
            if (n.genus === 'plagula'
                && n.nomen.indexOf(ephemera.quaesitum) >= 0) {
              plana.push(n);
            }
            if (n.filii) { ire(n.filii); }
          }
        }(graphus.filii));
      } else {
        (function ire(nodi) {
          var i, n;
          for (i = 0; i < nodi.length; i++) {
            n = nodi[i];
            plana.push(n);
            if (n.genus === 'dir' && n.apertus && n.filii) { ire(n.filii); }
          }
        }(graphus.filii));
      }
      electus = 0;
      if (ephemera.electio !== '') {
        var i;
        for (i = 0; i < plana.length; i++) {
          if (plana[i].via === ephemera.electio) { electus = i; break; }
        }
      }
    }

    function pingere() {
      picturae = picturae + 1;
      var cribratum = (ephemera.quaesitum !== '');

      var summitas = prospectus.scrollTop;
      var altPros  = prospectus.clientHeight;

      spatium.style.height = (plana.length * ALT) + 'px';

      var primus  = Math.max(0, Math.floor(summitas / ALT) - MARGO);
      var quot    = Math.ceil(altPros / ALT) + (MARGO * 2);
      var ultimus = Math.min(plana.length, primus + quot);

      var frag = document.createDocumentFragment();
      var i, n, o, sig, nom, men, tenetur;

      for (i = primus; i < ultimus; i++) {
        n = plana[i];
        tenetur = (i === electus && n.via === ephemera.electio);
        o = document.createElement('div');
        o.className = 'ca-ordo' + (tenetur ? ' ca-electus' : '');
        o.style.top = (i * ALT) + 'px';
        o.style.paddingLeft = (6 + (cribratum ? 0 : n.gradus) * 16) + 'px';
        o.setAttribute('data-index', String(i));
        o.setAttribute('role', 'treeitem');
        if (n.genus === 'dir') {
          o.setAttribute('aria-expanded', n.apertus ? 'true' : 'false');
        }
        sig = document.createElement('span');
        sig.className = 'ca-signum';
        sig.textContent = (n.genus === 'dir') ? (n.apertus ? '▾' : '▸') : ' ';
        nom = document.createElement('span');
        nom.className = 'ca-nomen';
        nom.textContent = cribratum ? n.via : n.nomen;
        men = document.createElement('span');
        men.className = 'ca-mensura';
        men.textContent = (n.genus === 'plagula') ? (n.mensura + ' B') : '';
        o.appendChild(sig);
        o.appendChild(nom);
        o.appendChild(men);
        frag.appendChild(o);
      }

      ordinesEl.innerHTML = '';
      ordinesEl.appendChild(frag);
      numeros_pingere();
    }

    function eligere(i, volvere) {
      var novus = Math.max(0, Math.min(plana.length - 1, i));
      if (!plana.length) { return; }
      electus = novus;
      mutare_ephemera(function (e) { e.electio = plana[novus].via; });
      if (volvere) {
        var summa = novus * ALT;
        var ima   = summa + ALT;
        if (summa < prospectus.scrollTop) {
          prospectus.scrollTop = summa;
        } else if (ima > prospectus.scrollTop + prospectus.clientHeight) {
          prospectus.scrollTop = ima - prospectus.clientHeight;
        }
      }
      pingere();
    }

    function commutare(i) {
      var n = plana[i];
      if (!n || n.genus !== 'dir') { return false; }
      mutare_durabile(function () { n.apertus = !n.apertus; });
      planare();
      pingere();
      return true;
    }

    function electum_ponere(i) {
      var n = plana[i];
      if (!n || n.genus !== 'plagula') { return false; }
      mutare_durabile(function (g) { g.electum = n.via; });
      pingere();
      /* graduata divergentia 0013: arbor spargit ut ceteri */
      _eventum_electum(el, { via: n.via });
      return true;
    }

    function parens(i) {
      var n = plana[i], k;
      if (!n) { return -1; }
      for (k = i - 1; k >= 0; k--) {
        if (plana[k].gradus < n.gradus) { return k; }
      }
      return -1;
    }

    function adClavem(ev) {
      var cribratum = (ephemera.quaesitum !== '');
      var altPros = prospectus.clientHeight;
      var salire  = Math.max(1, Math.floor(altPros / ALT) - 1);
      var n = plana[electus];
      var p;

      if (ev.key === 'ArrowDown')      { eligere(electus + 1, true); }
      else if (ev.key === 'ArrowUp')   { eligere(electus - 1, true); }
      else if (ev.key === 'PageDown')  { eligere(electus + salire, true); }
      else if (ev.key === 'PageUp')    { eligere(electus - salire, true); }
      else if (ev.key === 'Home')      { eligere(0, true); }
      else if (ev.key === 'End')       { eligere(plana.length - 1, true); }
      else if (ev.key === 'ArrowRight') {
        if (cribratum) { return; }
        if (n && n.genus === 'dir' && !n.apertus) { commutare(electus); }
        else { eligere(electus + 1, true); }
      }
      else if (ev.key === 'ArrowLeft') {
        if (cribratum) { return; }
        if (n && n.genus === 'dir' && n.apertus) { commutare(electus); }
        else {
          p = parens(electus);
          if (p >= 0) { eligere(p, true); }
        }
      }
      else if (ev.key === 'Enter' || ev.key === ' ') {
        if (n && n.genus === 'dir') {
          if (!commutare(electus)) { return; }
        } else if (!electum_ponere(electus)) { return; }
      }
      else { return; }
      ev.preventDefault();
    }

    function adIctum(ev) {
      var o = ev.target;
      while (o && o !== ordinesEl && !o.getAttribute('data-index')) {
        o = o.parentNode;
      }
      if (o && o.getAttribute) {
        var i = parseInt(o.getAttribute('data-index'), 10);
        if (!isNaN(i)) {
          if (plana[i] && plana[i].genus === 'dir') { commutare(i); }
          else if (plana[i]) { eligere(i, false); }
        }
      }
    }

    function adLitteram() {
      prospectus.scrollTop = 0;
      mutare_ephemera(function (e) {
        e.quaesitum = campus.value;
        e.electio = '';
      });
      planare();
      pingere();
    }

    function adClavemCampi(ev) {
      if (ev.key === 'ArrowDown') {
        prospectus.focus();
        ev.preventDefault();
      } else if (ev.key === 'Escape' && campus.value !== '') {
        campus.value = '';
        adLitteram();
        ev.preventDefault();
      }
    }

    function adVolutionem() {
      pingere();
      if (quies_index !== null) { clearTimeout(quies_index); }
      quies_index = setTimeout(function () {
        var v = Math.round(prospectus.scrollTop);
        quies_index = null;
        if (v !== ephemera.volutio) {
          mutare_ephemera(function (e) { e.volutio = v; });
        }
      }, 300);
    }

    function adMutationemFenestrae() { pingere(); }

    function construere() {
      proiectio = document.createElement('div');
      proiectio.className = 'ca-proiectio';
      proiectio.innerHTML =
          (numeri_visibiles
            ? '<p class="ca-numeri"></p>' : '')
        + (remontabile
            ? '<button type="button" class="ca-remontare">componentem'
              + ' remontare (ex insulis solis)</button>' : '')
        + '<input type="text" class="ca-quaerere" autocomplete="off"'
        +        ' placeholder="plagulas cribrare"'
        +        ' aria-label="plagulas cribrare">'
        + '<div class="ca-prospectus" tabindex="0" role="tree"'
        +      ' aria-label="plagulae ex insulis">'
        +   '<div class="ca-spatium">'
        +     '<div class="ca-ordines"></div>'
        +   '</div>'
        + '</div>';
      el.appendChild(proiectio);

      prospectus = proiectio.querySelector('.ca-prospectus');
      spatium    = proiectio.querySelector('.ca-spatium');
      ordinesEl  = proiectio.querySelector('.ca-ordines');
      campus     = proiectio.querySelector('.ca-quaerere');
      numeriEl   = proiectio.querySelector('.ca-numeri');
      ALT        = altitudoOrdinis();

      lectiones = lectiones + 1;
      graphus  = insulam_legere(insula_d.textContent);
      ephemera = ephemeram_legere();

      NODI = (function numerare(nodi) {
        var n = 0, i;
        for (i = 0; i < nodi.length; i++) {
          n = n + 1;
          if (nodi[i].filii) { n = n + numerare(nodi[i].filii); }
        }
        return n;
      }(graphus.filii));

      campus.value = ephemera.quaesitum;

      prospectus.addEventListener('scroll', adVolutionem);
      prospectus.addEventListener('keydown', adClavem);
      prospectus.addEventListener('click', adIctum);
      campus.addEventListener('input', adLitteram);
      campus.addEventListener('keydown', adClavemCampi);
      window.addEventListener('resize', adMutationemFenestrae);

      var remo = proiectio.querySelector('.ca-remontare');
      if (remo) {
        remo.addEventListener('click', function () {
          destruere();
          construere();
        });
      }

      planare();
      pingere();
      prospectus.scrollTop = ephemera.volutio;
    }

    function destruere() {
      if (quies_index !== null) {
        clearTimeout(quies_index);
        quies_index = null;
      }
      window.removeEventListener('resize', adMutationemFenestrae);
      if (proiectio && proiectio.parentNode) {
        proiectio.parentNode.removeChild(proiectio);
      }
      proiectio = null;
      graphus = null;
      ephemera = null;
    }

    try {
      construere();
    } catch (e) {
      el.innerHTML = '<p class="ca-culpa">CULPA: ' + e.message + '</p>';
      return null;
    }
    return destruere;
  }

  /* ============================================================
   * INDEX - migratio 0002 in bibliothecam (experimentum 0010)
   * ============================================================
   * Insula: <index electum=""><res sigla titulus nota?/></index>
   * Ephemera: electio (SIGLA - cursor identitatem sequitur, 0007),
   * quaesitum, volutio (kinetica, in quiete).
   * Styli: classes arboris ('ca-') REUSAE - index est arbor sine
   * gradibus; stylus novus NULLUS.
   * Enter: electum durabile + CustomEvent 'electum' (bulliens) -
   * exitus componentis idiomaticus; marcatio effectus NOMINAT,
   * numquam computat. */
  function indicem_figere(el) {
    var numeri_visibiles = (el.getAttribute('data-numeri') === 'verum');
    var remontabile = (el.getAttribute('data-remontabile') === 'verum');

    var insula_d = _fontem_resolvere(el);
    if (!insula_d) { return null; }

    var insulae_e = _ephemeras_parare(el, [
      { titulus: 'electio',   genus: 'c', praefinitum: '' },
      { titulus: 'quaesitum', genus: 'c', praefinitum: '' },
      { titulus: 'volutio',   genus: 'n', praefinitum: '0' }
    ]);

    var lectiones    = 0;
    var scripturae_d = 0;
    var scripturae_e = 0;
    var picturae     = 0;

    var graphus  = null;   /* {electum, res:[{sigla,titulus,nota}]} */
    var ephemera = null;
    var plana    = [];
    var electus  = 0;

    var proiectio  = null;
    var prospectus = null;
    var spatium    = null;
    var ordinesEl  = null;
    var campus     = null;
    var numeriEl   = null;
    var ALT        = 24;
    var MARGO      = 4;
    var quies_index = null;

    function ephemeram_legere() { return insulae_e.legere(); }
    function ephemeram_scribere() {
      insulae_e.scribere(ephemera);
      scripturae_e = scripturae_e + 1;
    }
    function insulam_durabilem_scribere() {
      var l = ['<index electum="' + graphus.electum + '">'], i, r;
      for (i = 0; i < graphus.res.length; i++) {
        r = graphus.res[i];
        l.push('  <res'
          + _att_serere({ sigla: r.sigla, titulus: r.titulus,
                          nota: r.nota },
                        ['sigla', 'titulus', 'nota'])
          + '/>');
      }
      l.push('</index>');
      insula_d.textContent = l.join('\n');
      scripturae_d = scripturae_d + 1;
    }

    function numeros_pingere() {
      if (!numeriEl) { return; }
      numeriEl.textContent =
          'res: ' + graphus.res.length
        + '  visibiles: ' + plana.length
        + '  lectiones: ' + lectiones
        + '  scripturae-d: ' + scripturae_d
        + '  scripturae-e: ' + scripturae_e
        + '  picturae: ' + picturae;
    }
    function mutare_durabile(f) {
      f(graphus);
      insulam_durabilem_scribere();
      numeros_pingere();
      _nuntiare_statum();
    }
    function mutare_ephemera(f) {
      f(ephemera);
      ephemeram_scribere();
      numeros_pingere();
      _nuntiare_statum();
    }

    function planare() {
      var i, r, q = ephemera.quaesitum;
      plana = [];
      for (i = 0; i < graphus.res.length; i++) {
        r = graphus.res[i];
        if (q === ''
            || r.titulus.indexOf(q) >= 0 || r.nota.indexOf(q) >= 0) {
          plana.push(r);
        }
      }
      electus = 0;
      if (ephemera.electio !== '') {
        for (i = 0; i < plana.length; i++) {
          if (plana[i].sigla === ephemera.electio) { electus = i; break; }
        }
      }
    }

    function pingere() {
      picturae = picturae + 1;
      var summitas = prospectus.scrollTop;
      var altPros  = prospectus.clientHeight;

      spatium.style.height = (plana.length * ALT) + 'px';

      var primus  = Math.max(0, Math.floor(summitas / ALT) - MARGO);
      var quot    = Math.ceil(altPros / ALT) + (MARGO * 2);
      var ultimus = Math.min(plana.length, primus + quot);

      var frag = document.createDocumentFragment();
      var i, r, o, nom, men, tenetur;

      for (i = primus; i < ultimus; i++) {
        r = plana[i];
        tenetur = (i === electus && r.sigla === ephemera.electio);
        o = document.createElement('div');
        o.className = 'ca-ordo' + (tenetur ? ' ca-electus' : '');
        o.style.top = (i * ALT) + 'px';
        o.style.paddingLeft = '6px';
        o.setAttribute('data-index', String(i));
        o.setAttribute('role', 'option');
        nom = document.createElement('span');
        nom.className = 'ca-nomen';
        nom.textContent = r.titulus;
        men = document.createElement('span');
        men.className = 'ca-mensura';
        men.textContent = r.nota;
        o.appendChild(nom);
        o.appendChild(men);
        frag.appendChild(o);
      }

      ordinesEl.innerHTML = '';
      ordinesEl.appendChild(frag);
      numeros_pingere();
      if (numeriEl) {
        numeriEl.textContent = numeriEl.textContent
          + '  picti: ' + (ultimus - primus);
      }
    }

    function eligere(i, volvere) {
      var novus = Math.max(0, Math.min(plana.length - 1, i));
      if (!plana.length) { return; }
      electus = novus;
      mutare_ephemera(function (e) { e.electio = plana[novus].sigla; });
      if (volvere) {
        var summa = novus * ALT;
        var ima   = summa + ALT;
        if (summa < prospectus.scrollTop) {
          prospectus.scrollTop = summa;
        } else if (ima > prospectus.scrollTop + prospectus.clientHeight) {
          prospectus.scrollTop = ima - prospectus.clientHeight;
        }
      }
      pingere();
    }

    function electum_ponere(i) {
      var r = plana[i];
      if (!r) { return false; }
      mutare_durabile(function (g) { g.electum = r.sigla; });
      pingere();
      _eventum_electum(el, { sigla: r.sigla, titulus: r.titulus });
      return true;
    }

    function adClavem(ev) {
      var altPros = prospectus.clientHeight;
      var salire  = Math.max(1, Math.floor(altPros / ALT) - 1);

      if (ev.key === 'ArrowDown')      { eligere(electus + 1, true); }
      else if (ev.key === 'ArrowUp')   { eligere(electus - 1, true); }
      else if (ev.key === 'PageDown')  { eligere(electus + salire, true); }
      else if (ev.key === 'PageUp')    { eligere(electus - salire, true); }
      else if (ev.key === 'Home')      { eligere(0, true); }
      else if (ev.key === 'End')       { eligere(plana.length - 1, true); }
      else if (ev.key === 'Enter') {
        if (!electum_ponere(electus)) { return; }
      }
      else { return; }
      ev.preventDefault();
    }

    function adIctum(ev) {
      var o = ev.target;
      while (o && o !== ordinesEl && !o.getAttribute('data-index')) {
        o = o.parentNode;
      }
      if (o && o.getAttribute) {
        var i = parseInt(o.getAttribute('data-index'), 10);
        if (!isNaN(i)) { eligere(i, false); }
      }
    }

    function adLitteram() {
      prospectus.scrollTop = 0;
      mutare_ephemera(function (e) {
        e.quaesitum = campus.value;
        e.electio = '';
      });
      planare();
      pingere();
    }
    function adClavemCampi(ev) {
      if (ev.key === 'ArrowDown') {
        prospectus.focus();
        ev.preventDefault();
      } else if (ev.key === 'Escape' && campus.value !== '') {
        campus.value = '';
        adLitteram();
        ev.preventDefault();
      }
    }
    function adVolutionem() {
      pingere();
      if (quies_index !== null) { clearTimeout(quies_index); }
      quies_index = setTimeout(function () {
        var v = Math.round(prospectus.scrollTop);
        quies_index = null;
        if (v !== ephemera.volutio) {
          mutare_ephemera(function (e) { e.volutio = v; });
        }
      }, 300);
    }
    function adMutationemFenestrae() { pingere(); }

    function construere() {
      proiectio = document.createElement('div');
      proiectio.className = 'ca-proiectio';
      proiectio.innerHTML =
          (numeri_visibiles ? '<p class="ca-numeri"></p>' : '')
        + (remontabile
            ? '<button type="button" class="ca-remontare">componentem'
              + ' remontare (ex insulis solis)</button>' : '')
        + '<input type="text" class="ca-quaerere" autocomplete="off"'
        +        ' placeholder="res cribrare" aria-label="res cribrare">'
        + '<div class="ca-prospectus" tabindex="0" role="listbox"'
        +      ' aria-label="index ex insulis">'
        +   '<div class="ca-spatium"><div class="ca-ordines"></div></div>'
        + '</div>';
      el.appendChild(proiectio);

      prospectus = proiectio.querySelector('.ca-prospectus');
      spatium    = proiectio.querySelector('.ca-spatium');
      ordinesEl  = proiectio.querySelector('.ca-ordines');
      campus     = proiectio.querySelector('.ca-quaerere');
      numeriEl   = proiectio.querySelector('.ca-numeri');
      ALT        = altitudoOrdinis();

      lectiones = lectiones + 1;
      var arbor_g = _stml_generale_legere(insula_d.textContent);
      if (arbor_g.tag !== 'index') {
        throw new Error('fons non est <index>: <' + arbor_g.tag + '>');
      }
      graphus = { electum: arbor_g.att.electum || '', res: [] };
      (function () {
        var i, f;
        for (i = 0; i < arbor_g.filii.length; i++) {
          f = arbor_g.filii[i];
          graphus.res.push({ sigla: f.att.sigla || '',
                             titulus: f.att.titulus || '',
                             nota: f.att.nota || '' });
        }
      }());
      ephemera = ephemeram_legere();
      campus.value = ephemera.quaesitum;

      prospectus.addEventListener('scroll', adVolutionem);
      prospectus.addEventListener('keydown', adClavem);
      prospectus.addEventListener('click', adIctum);
      campus.addEventListener('input', adLitteram);
      campus.addEventListener('keydown', adClavemCampi);
      window.addEventListener('resize', adMutationemFenestrae);

      var remo = proiectio.querySelector('.ca-remontare');
      if (remo) {
        remo.addEventListener('click', function () {
          destruere();
          construere();
        });
      }

      planare();
      pingere();
      prospectus.scrollTop = ephemera.volutio;
    }

    function destruere() {
      if (quies_index !== null) {
        clearTimeout(quies_index);
        quies_index = null;
      }
      window.removeEventListener('resize', adMutationemFenestrae);
      if (proiectio && proiectio.parentNode) {
        proiectio.parentNode.removeChild(proiectio);
      }
      proiectio = null;
      graphus = null;
      ephemera = null;
    }

    try {
      construere();
    } catch (e) {
      el.innerHTML = '<p class="ca-culpa">CULPA: ' + e.message + '</p>';
      return null;
    }
    return destruere;
  }

  /* ============================================================
   * OPTIO - capsa electionis (experimentum 0014)
   * ============================================================
   * Insula: <optiones electum=""><optio sigla titulus/></optiones>
   * Ephemera: apertum, quaesitum, electio (cursor SIGLA).
   * Kinetica NULLA et virtualizatio NULLA consulto: index
   * optionum brevis est (privatio nominata, non omissio).
   *
   * CONTRACTUS NOVI bis:
   * (1) PYXIS ANCORATA sub campo (non velum modale tabellae) -
   *     aperitur litteris/ArrowDown/ictu pyxidis, clauditur
   *     Enter/Escape/amissione foci.
   * (2) COMMITTERE aut REVERTERE - Enter committit (electum
   *     durabile + eventus 'electum' + campus = titulus);
   *     Escape et amissio foci REVERTUNT ad titulum commissum -
   *     textus dimidius NUMQUAM committitur.
   * Focus campi textum totum eligit (litterae priores REPONUNT,
   * non appendunt - mos capsarum electionis ordinarius). Ictus
   * in pyxide focum campi NON rapit (mousedown praeventus). */
  function optionem_figere(el) {
    var numeri_visibiles = (el.getAttribute('data-numeri') === 'verum');
    var remontabile = (el.getAttribute('data-remontabile') === 'verum');

    var insula_d = _fontem_resolvere(el);
    if (!insula_d) { return null; }

    var insulae_e = _ephemeras_parare(el, [
      { titulus: 'apertum',   genus: 'b', praefinitum: 'falsum' },
      { titulus: 'quaesitum', genus: 'c', praefinitum: '' },
      { titulus: 'electio',   genus: 'c', praefinitum: '' }
    ]);

    var lectiones    = 0;
    var scripturae_d = 0;
    var scripturae_e = 0;
    var picturae     = 0;

    var graphus  = null;   /* {electum, optiones:[{sigla,titulus}]} */
    var ephemera = null;
    var plana    = [];
    var electus  = 0;

    var proiectio = null;
    var campus    = null;
    var pyxis     = null;
    var ordinesEl = null;
    var numeriEl  = null;

    function ephemeram_legere() { return insulae_e.legere(); }
    function ephemeram_scribere() {
      insulae_e.scribere(ephemera);
      scripturae_e = scripturae_e + 1;
    }
    function insulam_durabilem_scribere() {
      var l = ['<optiones electum="' + graphus.electum + '">'], i, o;
      for (i = 0; i < graphus.optiones.length; i++) {
        o = graphus.optiones[i];
        l.push('  <optio'
          + _att_serere({ sigla: o.sigla, titulus: o.titulus },
                        ['sigla', 'titulus'])
          + '/>');
      }
      l.push('</optiones>');
      insula_d.textContent = l.join('\n');
      scripturae_d = scripturae_d + 1;
    }

    function numeros_pingere() {
      if (!numeriEl) { return; }
      numeriEl.textContent =
          'optiones: ' + graphus.optiones.length
        + '  visibiles: ' + plana.length
        + '  apertum: ' + (ephemera.apertum ? 'verum' : 'falsum')
        + '  lectiones: ' + lectiones
        + '  scripturae-d: ' + scripturae_d
        + '  scripturae-e: ' + scripturae_e
        + '  picturae: ' + picturae;
    }
    function mutare_durabile(f) {
      f(graphus);
      insulam_durabilem_scribere();
      numeros_pingere();
      _nuntiare_statum();
    }
    function mutare_ephemera(f) {
      f(ephemera);
      ephemeram_scribere();
      numeros_pingere();
      _nuntiare_statum();
    }

    function titulus_electi() {
      var i;
      for (i = 0; i < graphus.optiones.length; i++) {
        if (graphus.optiones[i].sigla === graphus.electum) {
          return graphus.optiones[i].titulus;
        }
      }
      return '';
    }

    function planare() {
      var i, o, q = ephemera.quaesitum;
      plana = [];
      for (i = 0; i < graphus.optiones.length; i++) {
        o = graphus.optiones[i];
        if (q === '' || o.titulus.indexOf(q) >= 0) {
          plana.push(o);
        }
      }
      electus = 0;
      if (ephemera.electio !== '') {
        for (i = 0; i < plana.length; i++) {
          if (plana[i].sigla === ephemera.electio) { electus = i; break; }
        }
      }
    }

    function pingere() {
      picturae = picturae + 1;
      var frag = document.createDocumentFragment();
      var i, o, ord;

      for (i = 0; i < plana.length; i++) {
        o = plana[i];
        ord = document.createElement('div');
        ord.className = 'ca-optio-ordo'
          + (i === electus ? ' ca-electus' : '');
        ord.setAttribute('data-index', String(i));
        ord.setAttribute('role', 'option');
        ord.textContent = o.titulus;
        frag.appendChild(ord);
      }
      ordinesEl.innerHTML = '';
      ordinesEl.appendChild(frag);
      pyxis.hidden = !ephemera.apertum || plana.length === 0;
      numeros_pingere();
    }

    function aperire() {
      if (!ephemera.apertum) {
        mutare_ephemera(function (e) { e.apertum = true; });
      }
      pingere();
    }
    function claudere() {
      if (ephemera.apertum) {
        mutare_ephemera(function (e) { e.apertum = false; });
      }
      pingere();
    }

    function eligere(i) {
      if (!plana.length) { return; }
      electus = Math.max(0, Math.min(plana.length - 1, i));
      mutare_ephemera(function (e) { e.electio = plana[electus].sigla; });
      pingere();
    }

    function committere(i) {
      if (!plana.length) { return false; }
      var o = plana[Math.max(0, Math.min(plana.length - 1, i))];
      mutare_durabile(function (g) { g.electum = o.sigla; });
      campus.value = o.titulus;
      mutare_ephemera(function (e) {
        e.quaesitum = '';
        e.electio = o.sigla;
      });
      planare();
      claudere();
      _eventum_electum(el, { sigla: o.sigla, titulus: o.titulus });
      return true;
    }

    function revertere() {
      campus.value = titulus_electi();
      mutare_ephemera(function (e) { e.quaesitum = ''; });
      planare();
      claudere();
    }

    function adLitteram() {
      mutare_ephemera(function (e) {
        e.quaesitum = campus.value;
        e.apertum = true;
        e.electio = '';
      });
      planare();
      pingere();
    }

    function adClavemCampi(ev) {
      if (ev.key === 'ArrowDown') {
        if (!ephemera.apertum) { aperire(); }
        else { eligere(electus + 1); }
      } else if (ev.key === 'ArrowUp') {
        eligere(electus - 1);
      } else if (ev.key === 'Enter') {
        if (ephemera.apertum) { committere(electus); }
        else { aperire(); }
      } else if (ev.key === 'Escape') {
        revertere();
      } else { return; }
      ev.preventDefault();
    }

    function adFocum() { campus.select(); }
    function adAmissionem() {
      /* amissio foci REVERTIT - textus dimidius numquam manet.
         (Ictus in pyxide focum non amittit: mousedown praeventus.) */
      revertere();
    }

    function adIctumPyxidis(ev) {
      var o = ev.target;
      while (o && o !== ordinesEl && !o.getAttribute('data-index')) {
        o = o.parentNode;
      }
      if (o && o.getAttribute) {
        var i = parseInt(o.getAttribute('data-index'), 10);
        if (!isNaN(i)) { committere(i); }
      }
    }

    function construere() {
      proiectio = document.createElement('div');
      proiectio.className = 'ca-proiectio';
      proiectio.innerHTML =
          (numeri_visibiles ? '<p class="ca-numeri"></p>' : '')
        + (remontabile
            ? '<button type="button" class="ca-remontare">componentem'
              + ' remontare (ex insulis solis)</button>' : '')
        + '<span class="ca-optio-involucrum">'
        +   '<input type="text" class="ca-optio-campus"'
        +          ' autocomplete="off" role="combobox"'
        +          ' aria-label="optionem eligere">'
        +   '<button type="button" class="ca-optio-vertere"'
        +          ' aria-label="pyxidem vertere">▾</button>'
        +   '<div class="ca-optio-pyxis" role="listbox" hidden>'
        +     '<div class="ca-optio-ordines"></div>'
        +   '</div>'
        + '</span>';
      el.appendChild(proiectio);

      campus    = proiectio.querySelector('.ca-optio-campus');
      pyxis     = proiectio.querySelector('.ca-optio-pyxis');
      ordinesEl = proiectio.querySelector('.ca-optio-ordines');
      numeriEl  = proiectio.querySelector('.ca-numeri');

      lectiones = lectiones + 1;
      var arbor_g = _stml_generale_legere(insula_d.textContent);
      if (arbor_g.tag !== 'optiones') {
        throw new Error('fons non est <optiones>: <' + arbor_g.tag
          + '>');
      }
      graphus = { electum: arbor_g.att.electum || '', optiones: [] };
      (function () {
        var i, f;
        for (i = 0; i < arbor_g.filii.length; i++) {
          f = arbor_g.filii[i];
          graphus.optiones.push({ sigla: f.att.sigla || '',
                                  titulus: f.att.titulus || '' });
        }
      }());
      ephemera = ephemeram_legere();
      campus.value = ephemera.quaesitum !== ''
        ? ephemera.quaesitum : titulus_electi();

      campus.addEventListener('input', adLitteram);
      campus.addEventListener('keydown', adClavemCampi);
      campus.addEventListener('focus', adFocum);
      campus.addEventListener('blur', adAmissionem);
      pyxis.addEventListener('mousedown', function (ev) {
        ev.preventDefault();
      });
      pyxis.addEventListener('click', adIctumPyxidis);
      var vertere = proiectio.querySelector('.ca-optio-vertere');
      vertere.addEventListener('mousedown', function (ev) {
        ev.preventDefault();
      });
      vertere.addEventListener('click', function () {
        if (ephemera.apertum) { claudere(); }
        else { campus.focus(); aperire(); }
      });

      var remo = proiectio.querySelector('.ca-remontare');
      if (remo) {
        remo.addEventListener('click', function () {
          destruere();
          construere();
        });
      }

      planare();
      pingere();
    }

    function destruere() {
      if (proiectio && proiectio.parentNode) {
        proiectio.parentNode.removeChild(proiectio);
      }
      proiectio = null;
      graphus = null;
      ephemera = null;
    }

    try {
      construere();
    } catch (e) {
      el.innerHTML = '<p class="ca-culpa">CULPA: ' + e.message + '</p>';
      return null;
    }
    return destruere;
  }

  /* ============================================================
   * TABELLA - migratio 0004 in bibliothecam (experimentum 0011)
   * ============================================================
   * Insula: <mandata><mandatum nomen="..." actio="..."/></mandata>
   * Ephemera: aperta, quaesitum, electio (NOMEN), ultimum (ACTIO
   * ultima exsecuta - effectus sine JS visibilis). Kinetica NULLA
   * consulto: tabella in apertura se reponit, volutio non manet.
   *
   * ACTIONES NOMINANTUR, numquam computantur (novacula canonis:
   * nulla lingua expressionum in marcatione). Exsecutio = eventus
   * 'mandatum' (bulliens, detail {actio, nomen}) + ultimum in
   * ephemera scriptum. Hospes audit et implet; marcatio nominat.
   *
   * AUDITOR DOCUMENTI (Cmd+K) a cyclo vitae possidetur: connexio
   * addit, disconnexio aufert - classis defectuum quam disciplina
   * tollendi manu custodiebat nunc structuralis (0009). */
  function tabellam_figere(el) {

    var insula_d = _fontem_resolvere(el);
    if (!insula_d) { return null; }

    var insulae_e = _ephemeras_parare(el, [
      { titulus: 'aperta',    genus: 'b', praefinitum: 'falsum' },
      { titulus: 'quaesitum', genus: 'c', praefinitum: '' },
      { titulus: 'electio',   genus: 'c', praefinitum: '' },
      { titulus: 'ultimum',   genus: 'c', praefinitum: '' }
    ]);

    var mandata   = [];      /* [{nomen, actio}] - durabilis, immota */
    var ephemera  = null;
    var congruentiae = [];   /* [{index, positiones}] */
    var electus   = 0;
    var priorFocus = null;

    var velum = null, tabella = null, campus = null;
    var indexEl = null, numerusEl = null, aperireEl = null;

    function ephemeram_legere() { return insulae_e.legere(); }
    function ephemeram_scribere() {
      insulae_e.scribere(ephemera);
    }
    function mutare_ephemera(f) {
      f(ephemera);
      ephemeram_scribere();
      _nuntiare_statum();
    }

    /* Subsequentia minimi ambitus - ex 0004 transcripta verbatim
     * (ordo: ambitus, sedes, ordo pristinus; praefixum sponte
     * vincit). */
    function congruere(quaesitum, nomen) {
      var q = quaesitum.toLowerCase();
      var n = nomen.toLowerCase();
      var optima = null;
      var s, i, k, pos, ambitus;
      if (q.length === 0) { return { positiones: [] }; }
      for (s = 0; s < n.length; s++) {
        if (n.charAt(s) !== q.charAt(0)) { continue; }
        pos = [s];
        k = 1;
        for (i = s + 1; i < n.length && k < q.length; i++) {
          if (n.charAt(i) === q.charAt(k)) { pos.push(i); k = k + 1; }
        }
        if (k < q.length) { break; }
        ambitus = pos[pos.length - 1] - pos[0] + 1;
        if (!optima || ambitus < optima.ambitus) {
          optima = { ambitus: ambitus, prima: pos[0], positiones: pos };
        }
      }
      return optima;
    }

    function cribrare() {
      var i, c;
      congruentiae = [];
      for (i = 0; i < mandata.length; i++) {
        c = congruere(ephemera.quaesitum, mandata[i].nomen);
        if (c) {
          congruentiae.push({ index: i,
                              ambitus: c.ambitus || 0,
                              prima: c.prima || 0,
                              positiones: c.positiones });
        }
      }
      congruentiae.sort(function (a, b) {
        if (a.ambitus !== b.ambitus) { return a.ambitus - b.ambitus; }
        if (a.prima !== b.prima)     { return a.prima - b.prima; }
        return a.index - b.index;
      });
      electus = 0;
      if (ephemera.electio !== '') {
        for (i = 0; i < congruentiae.length; i++) {
          if (mandata[congruentiae[i].index].nomen === ephemera.electio) {
            electus = i;
            break;
          }
        }
      }
      pingere();
    }

    function pingere() {
      var i, k, c, li, nomen, cursor, frag, span, nihil;
      frag = document.createDocumentFragment();
      for (i = 0; i < congruentiae.length; i++) {
        c = congruentiae[i];
        nomen = mandata[c.index].nomen;
        li = document.createElement('li');
        li.className = 'cm-mandatum' + (i === electus ? ' cm-electus' : '');
        li.setAttribute('data-ordo', String(i));
        li.setAttribute('role', 'option');
        cursor = 0;
        for (k = 0; k < c.positiones.length; k++) {
          if (c.positiones[k] > cursor) {
            li.appendChild(document.createTextNode(
              nomen.substring(cursor, c.positiones[k])));
          }
          span = document.createElement('span');
          span.className = 'cm-congrua';
          span.textContent = nomen.charAt(c.positiones[k]);
          li.appendChild(span);
          cursor = c.positiones[k] + 1;
        }
        if (cursor < nomen.length) {
          li.appendChild(document.createTextNode(nomen.substring(cursor)));
        }
        frag.appendChild(li);
      }
      indexEl.innerHTML = '';
      if (congruentiae.length === 0) {
        nihil = document.createElement('li');
        nihil.className = 'cm-nihil';
        nihil.textContent = 'nihil congruit';
        indexEl.appendChild(nihil);
      } else {
        indexEl.appendChild(frag);
      }
      numerusEl.textContent =
        congruentiae.length + ' / ' + mandata.length;
      var e2 = indexEl.querySelector('.cm-electus');
      if (e2 && e2.scrollIntoView) { e2.scrollIntoView({ block: 'nearest' }); }
    }

    function aperta() { return velum.className.indexOf('cm-aperta') >= 0; }

    function aperire() {
      priorFocus = document.activeElement;
      velum.className = 'cm-velum cm-aperta';
      campus.value = '';
      mutare_ephemera(function (e) {
        e.aperta = true;
        e.quaesitum = '';
        e.electio = '';
      });
      cribrare();
      campus.focus();
    }
    function claudere() {
      velum.className = 'cm-velum';
      mutare_ephemera(function (e) { e.aperta = false; });
      if (priorFocus && priorFocus.focus
          && document.contains(priorFocus)) {
        priorFocus.focus();
      }
      priorFocus = null;
    }

    function exsequi(ordo) {
      var c = congruentiae[ordo];
      var mandatum;
      if (!c) { return; }
      mandatum = mandata[c.index];
      mutare_ephemera(function (e) { e.ultimum = mandatum.actio; });
      el.dispatchEvent(new CustomEvent('mandatum', {
        bubbles: true,
        detail: { actio: mandatum.actio, nomen: mandatum.nomen }
      }));
      claudere();
    }

    function adClavemGlobalem(ev) {
      if (ev.metaKey && !ev.ctrlKey && !ev.altKey && !ev.shiftKey
          && (ev.key === 'k' || ev.key === 'K')) {
        ev.preventDefault();
        if (aperta()) { claudere(); } else { aperire(); }
        return;
      }
      if (ev.key === 'Escape' && aperta()) {
        ev.preventDefault();
        claudere();
      }
    }
    function adClavemTabellae(ev) {
      if (ev.key === 'ArrowDown') {
        if (congruentiae.length) {
          electus = (electus + 1) % congruentiae.length;
          mutare_ephemera(function (e) {
            e.electio = mandata[congruentiae[electus].index].nomen;
          });
          pingere();
        }
      } else if (ev.key === 'ArrowUp') {
        if (congruentiae.length) {
          electus = (electus - 1 + congruentiae.length)
                  % congruentiae.length;
          mutare_ephemera(function (e) {
            e.electio = mandata[congruentiae[electus].index].nomen;
          });
          pingere();
        }
      } else if (ev.key === 'Enter') {
        if (congruentiae.length) { exsequi(electus); }
      } else if (ev.key === 'Escape') {
        claudere();
      } else { return; }
      ev.preventDefault();
    }
    function adLitteram() {
      mutare_ephemera(function (e) {
        e.quaesitum = campus.value;
        e.electio = '';
      });
      cribrare();
    }
    function adIctumIndicis(ev) {
      var o = ev.target;
      while (o && o !== indexEl
             && !(o.getAttribute && o.getAttribute('data-ordo'))) {
        o = o.parentNode;
      }
      if (o && o !== indexEl && o.getAttribute) {
        var i = parseInt(o.getAttribute('data-ordo'), 10);
        if (!isNaN(i)) { exsequi(i); }
      }
    }
    function adIctumVeli(ev) {
      if (ev.target === velum) { claudere(); }
    }

    function construere() {
      var arbor_g = _stml_generale_legere(insula_d.textContent);
      var i, f;
      if (arbor_g.tag !== 'mandata') {
        throw new Error('fons non est <mandata>: <' + arbor_g.tag + '>');
      }
      mandata = [];
      for (i = 0; i < arbor_g.filii.length; i++) {
        f = arbor_g.filii[i];
        mandata.push({ nomen: f.att.nomen || '',
                       actio: f.att.actio || '' });
      }
      ephemera = ephemeram_legere();

      var proiectio = document.createElement('div');
      proiectio.className = 'cm-proiectio';
      proiectio.innerHTML =
          '<button type="button" class="cm-aperire">tabellam aperire'
        + ' (Cmd+K)</button>'
        + '<div class="cm-velum">'
        +   '<div class="cm-tabella" role="dialog"'
        +        ' aria-label="tabella mandatorum">'
        +     '<input type="text" class="cm-quaerere" autocomplete="off"'
        +          ' aria-label="mandatum quaerere">'
        +     '<ul class="cm-index" role="listbox"></ul>'
        +     '<div class="cm-numerus"></div>'
        +   '</div>'
        + '</div>';
      el.appendChild(proiectio);

      velum     = proiectio.querySelector('.cm-velum');
      tabella   = proiectio.querySelector('.cm-tabella');
      campus    = proiectio.querySelector('.cm-quaerere');
      indexEl   = proiectio.querySelector('.cm-index');
      numerusEl = proiectio.querySelector('.cm-numerus');
      aperireEl = proiectio.querySelector('.cm-aperire');

      document.addEventListener('keydown', adClavemGlobalem);
      campus.addEventListener('keydown', adClavemTabellae);
      campus.addEventListener('input', adLitteram);
      indexEl.addEventListener('click', adIctumIndicis);
      velum.addEventListener('click', adIctumVeli);
      aperireEl.addEventListener('click', function () { aperire(); });

      /* Rehydratio: 'aperta' seminata tabellam statim aperit
       * (quaesitum et electio ex insula). */
      if (ephemera.aperta) {
        priorFocus = document.activeElement;
        velum.className = 'cm-velum cm-aperta';
        campus.value = ephemera.quaesitum;
        cribrare();
        campus.focus();
      }
    }

    function destruere() {
      document.removeEventListener('keydown', adClavemGlobalem);
      var p = el.querySelector('.cm-proiectio');
      if (p && p.parentNode) { p.parentNode.removeChild(p); }
    }

    try {
      construere();
    } catch (e) {
      el.innerHTML = '<p class="ca-culpa">CULPA: ' + e.message + '</p>';
      return null;
    }
    return destruere;
  }

  /* ============================================================
   * MATHEMATICA COLORUM COMMUNIS (0016; lector 0015 suas adhuc
   * privatim fert - stratum adiutorum eas uniet)
   * ============================================================ */
  function _color_duae(n) {
    var s = Math.round(n).toString(16);
    return s.length < 2 ? '0' + s : s;
  }
  function _color_hsv_ad_hex(c) {
    var h = c.h / 60, i = Math.floor(h) % 6;
    var f = h - Math.floor(h);
    var p = c.v * (1 - c.s);
    var q = c.v * (1 - f * c.s);
    var t = c.v * (1 - (1 - f) * c.s);
    var r, g, b;
    if (i === 0)      { r = c.v; g = t;   b = p; }
    else if (i === 1) { r = q;   g = c.v; b = p; }
    else if (i === 2) { r = p;   g = c.v; b = t; }
    else if (i === 3) { r = p;   g = q;   b = c.v; }
    else if (i === 4) { r = t;   g = p;   b = c.v; }
    else              { r = c.v; g = p;   b = q; }
    return '#' + _color_duae(r * 255) + _color_duae(g * 255)
      + _color_duae(b * 255);
  }
  function _color_hex_ad_hsv(x) {
    var m = /^#([0-9a-fA-F]{6})$/.exec(x);
    if (!m) { return null; }
    var n = parseInt(m[1], 16);
    var r = ((n >> 16) & 255) / 255;
    var g = ((n >> 8) & 255) / 255;
    var b = (n & 255) / 255;
    var max = Math.max(r, g, b), min = Math.min(r, g, b);
    var d = max - min;
    var h = 0;
    if (d > 0) {
      if (max === r)      { h = 60 * (((g - b) / d) % 6); }
      else if (max === g) { h = 60 * ((b - r) / d + 2); }
      else                { h = 60 * ((r - g) / d + 4); }
      if (h < 0) { h = h + 360; }
    }
    return { h: h, s: max === 0 ? 0 : d / max, v: max };
  }

  /* ROTA ARTISTICA (RYB): complementum caeruleo AURANTIUM est,
   * non flavum - pictorum rota, cor theoriae Palettonis. Ancorae
   * par (physica RGB -> artistica RYB), interpolatio linearis
   * fragmentis, inversa eadem tabula. */
  var _ROTA_ANCORAE = [
    [0, 0], [35, 60], [60, 122], [120, 165], [180, 218],
    [240, 275], [300, 330], [360, 360]
  ];
  function _rota_interpolari(h, a, b) {
    var i;
    for (i = 0; i + 1 < _ROTA_ANCORAE.length; i++) {
      var x0 = _ROTA_ANCORAE[i][a],     y0 = _ROTA_ANCORAE[i][b];
      var x1 = _ROTA_ANCORAE[i + 1][a], y1 = _ROTA_ANCORAE[i + 1][b];
      if (h >= x0 && h <= x1) {
        return x1 === x0 ? y0 : y0 + (h - x0) / (x1 - x0) * (y1 - y0);
      }
    }
    return h;
  }
  function _rota_ad_artisticam(h) { return _rota_interpolari(h, 0, 1); }
  function _rota_ad_physicam(h)   { return _rota_interpolari(h, 1, 0); }

  /* ============================================================
   * COLOR - lector coloris (experimentum 0015)
   * ============================================================
   * Insula: <color valor="#rrggbb"/>. Ephemera NULLA consulto
   * (privatio nominata): focus a navigatro tenetur, status
   * transitorius alius nullus.
   *
   * GRADUS TRES, DISCIPLINAE DUAE - probatio doctrinae:
   * - TRACTUS (kineticus): planum SV / regula coloris tracta -
   *   praevisus VIVUS (thumb, quadrum, campus hex), insula
   *   INTACTA; in QUIETE (mouseup) scriptura durabilis UNA.
   * - CLAVES (discretae): sagittae in plano/regula = actio
   *   discreta, scriptura durabilis STATIM quaque pressione.
   * - Campus hex: Enter/mutatio committit (#rrggbb validum);
   *   invalidum CULPAM in elemento nominat, insulam non tangit.
   * Eventus 'electum' (detail {valor}) quaque scriptura durabili.
   */
  function colorem_figere(el) {
    var numeri_visibiles = (el.getAttribute('data-numeri') === 'verum');
    var remontabile = (el.getAttribute('data-remontabile') === 'verum');

    var insula_d = _fontem_resolvere(el);
    if (!insula_d) { return null; }

    var lectiones    = 0;
    var scripturae_d = 0;
    var picturae     = 0;

    var hsv = { h: 0, s: 1, v: 1 };   /* veritas operis */

    var proiectio = null;
    var planum    = null;
    var regula    = null;
    var quadrum   = null;
    var campusHex = null;
    var numeriEl  = null;
    var culpaEl   = null;
    var thumbP    = null;
    var thumbR    = null;
    var trahens   = null;   /* 'planum' | 'regula' | null */

    /* mathematica communis moduli (_color_*) - privatae abolitae
       in graduatione 0017 */

    function numeros_pingere() {
      if (!numeriEl) { return; }
      numeriEl.textContent =
          'valor: ' + _color_hsv_ad_hex(hsv)
        + '  lectiones: ' + lectiones
        + '  scripturae-d: ' + scripturae_d
        + '  picturae: ' + picturae;
    }

    /* praevisus vivus - gradus kineticus: insula INTACTA */
    function pingere() {
      picturae = picturae + 1;
      var hexHue = _color_hsv_ad_hex({ h: hsv.h, s: 1, v: 1 });
      var hex    = _color_hsv_ad_hex(hsv);
      planum.style.background =
        'linear-gradient(to top, #000, rgba(0,0,0,0)), '
        + 'linear-gradient(to right, #fff, ' + hexHue + ')';
      thumbP.style.left = (hsv.s * 100) + '%';
      thumbP.style.top  = ((1 - hsv.v) * 100) + '%';
      thumbR.style.left = (hsv.h / 360 * 100) + '%';
      quadrum.style.background = hex;
      if (document.activeElement !== campusHex) {
        campusHex.value = hex;
      }
      culpaEl.hidden = true;
      numeros_pingere();
    }

    /* scriptura durabilis - porta unica insulae */
    function committere() {
      var hex = _color_hsv_ad_hex(hsv);
      insula_d.textContent = '<color valor="' + hex + '"/>';
      scripturae_d = scripturae_d + 1;
      campusHex.value = hex;
      numeros_pingere();
      _nuntiare_statum();
      _eventum_electum(el, { valor: hex });
    }

    function e_puncto_plani(ev) {
      var q = planum.getBoundingClientRect();
      var s = (ev.clientX - q.left) / q.width;
      var v = 1 - (ev.clientY - q.top) / q.height;
      hsv.s = Math.max(0, Math.min(1, s));
      hsv.v = Math.max(0, Math.min(1, v));
    }
    function e_puncto_regulae(ev) {
      var q = regula.getBoundingClientRect();
      var h = (ev.clientX - q.left) / q.width * 360;
      hsv.h = Math.max(0, Math.min(359.9, h));
    }

    function adMotum(ev) {
      if (trahens === 'planum') { e_puncto_plani(ev); }
      else if (trahens === 'regula') { e_puncto_regulae(ev); }
      else { return; }
      pingere();
    }
    function adQuietem() {
      /* QUIES tractus = scriptura durabilis una (doctrina
         kinetica: perfectum in quiete) */
      if (trahens === null) { return; }
      trahens = null;
      document.removeEventListener('mousemove', adMotum);
      document.removeEventListener('mouseup', adQuietem);
      committere();
    }
    function tractum_incipere(quid, ev) {
      trahens = quid;
      document.addEventListener('mousemove', adMotum);
      document.addEventListener('mouseup', adQuietem);
      adMotum(ev);
      ev.preventDefault();
    }

    /* claves = actiones discretae: scriptura durabilis STATIM */
    function adClavemPlani(ev) {
      var gradus = ev.shiftKey ? 0.1 : 0.02;
      if (ev.key === 'ArrowRight')     { hsv.s = Math.min(1, hsv.s + gradus); }
      else if (ev.key === 'ArrowLeft') { hsv.s = Math.max(0, hsv.s - gradus); }
      else if (ev.key === 'ArrowUp')   { hsv.v = Math.min(1, hsv.v + gradus); }
      else if (ev.key === 'ArrowDown') { hsv.v = Math.max(0, hsv.v - gradus); }
      else { return; }
      pingere();
      committere();
      ev.preventDefault();
    }
    function adClavemRegulae(ev) {
      var gradus = ev.shiftKey ? 30 : 6;
      if (ev.key === 'ArrowRight')     { hsv.h = Math.min(359.9, hsv.h + gradus); }
      else if (ev.key === 'ArrowLeft') { hsv.h = Math.max(0, hsv.h - gradus); }
      else { return; }
      pingere();
      committere();
      ev.preventDefault();
    }

    function adHex() {
      var c = _color_hex_ad_hsv(campusHex.value);
      if (!c) {
        culpaEl.textContent = 'CULPA: hex invalidum ("'
          + campusHex.value + '" - forma #rrggbb)';
        culpaEl.hidden = false;
        return;
      }
      hsv = c;
      pingere();
      committere();
    }
    function adClavemHex(ev) {
      if (ev.key === 'Enter') { adHex(); ev.preventDefault(); }
    }

    function construere() {
      proiectio = document.createElement('div');
      proiectio.className = 'ca-proiectio';
      proiectio.innerHTML =
          (numeri_visibiles ? '<p class="ca-numeri"></p>' : '')
        + (remontabile
            ? '<button type="button" class="ca-remontare">componentem'
              + ' remontare (ex insulis solis)</button>' : '')
        + '<div class="ca-color-planum" tabindex="0" role="slider"'
        +      ' aria-label="saturatio et valor">'
        +   '<div class="ca-color-thumb"></div>'
        + '</div>'
        + '<div class="ca-color-regula" tabindex="0" role="slider"'
        +      ' aria-label="color">'
        +   '<div class="ca-color-thumb"></div>'
        + '</div>'
        + '<p class="ca-color-linea">'
        +   '<span class="ca-color-quadrum"></span>'
        +   '<input type="text" class="ca-color-hex"'
        +          ' autocomplete="off" aria-label="valor hex"'
        +          ' maxlength="7">'
        + '</p>'
        + '<p class="ca-culpa ca-color-culpa" hidden></p>';
      el.appendChild(proiectio);

      planum    = proiectio.querySelector('.ca-color-planum');
      regula    = proiectio.querySelector('.ca-color-regula');
      quadrum   = proiectio.querySelector('.ca-color-quadrum');
      campusHex = proiectio.querySelector('.ca-color-hex');
      numeriEl  = proiectio.querySelector('.ca-numeri');
      culpaEl   = proiectio.querySelector('.ca-color-culpa');
      thumbP    = planum.querySelector('.ca-color-thumb');
      thumbR    = regula.querySelector('.ca-color-thumb');

      lectiones = lectiones + 1;
      var arbor_g = _stml_generale_legere(insula_d.textContent);
      if (arbor_g.tag !== 'color') {
        throw new Error('fons non est <color>: <' + arbor_g.tag
          + '>');
      }
      var c = _color_hex_ad_hsv(arbor_g.att.valor || '');
      hsv = c || { h: 0, s: 1, v: 1 };

      planum.addEventListener('mousedown', function (ev) {
        planum.focus();
        tractum_incipere('planum', ev);
      });
      regula.addEventListener('mousedown', function (ev) {
        regula.focus();
        tractum_incipere('regula', ev);
      });
      planum.addEventListener('keydown', adClavemPlani);
      regula.addEventListener('keydown', adClavemRegulae);
      campusHex.addEventListener('keydown', adClavemHex);
      campusHex.addEventListener('change', adHex);

      var remo = proiectio.querySelector('.ca-remontare');
      if (remo) {
        remo.addEventListener('click', function () {
          destruere();
          construere();
        });
      }

      pingere();
    }

    function destruere() {
      document.removeEventListener('mousemove', adMotum);
      document.removeEventListener('mouseup', adQuietem);
      trahens = null;
      if (proiectio && proiectio.parentNode) {
        proiectio.parentNode.removeChild(proiectio);
      }
      proiectio = null;
    }

    try {
      construere();
    } catch (e) {
      el.innerHTML = '<p class="ca-culpa">CULPA: ' + e.message + '</p>';
      return null;
    }
    return destruere;
  }

  /* ============================================================
   * HARMONIA - genitor thematum colorum (experimentum 0016)
   * ============================================================
   * Insula: <harmonia basis="#rrggbb" ratio="..." rota="..."/> -
   * INITUS SOLI. Paletta PROIECTIO est: functio pura insulae,
   * numquam condita (remontatio eandem palettam ad octetum
   * reddere DEBET - id fumus asserit).
   *
   * Rationes (anguli in rota laborante): monochromatica [0],
   * complementaria [0,180], scissa [0,150,210], analoga
   * [0,330,30], triadica [0,120,240], tetradica [0,90,180,270].
   * Quisque color quinque gradus fert (pallidiores/obscuriores ex
   * s/v basis). Rota artistica = spatium RYB (complementum
   * caeruleo AURANTIUM): anguli in spatio picto rotantur, dein ad
   * physicam revertuntur.
   *
   * Exportatio: variabiles CSS (--harmonia-N-M) in area legenda -
   * thema sublevabile, et superficies quam manus asserit.
   * Regula coloris disciplinas 0015 fert (tractus in quiete,
   * claves statim). */
  function harmoniam_figere(el) {
    var numeri_visibiles = (el.getAttribute('data-numeri') === 'verum');
    var remontabile = (el.getAttribute('data-remontabile') === 'verum');

    var insula_d = _fontem_resolvere(el);
    if (!insula_d) { return null; }

    var RATIONES = {
      monochromatica: [0],
      complementaria: [0, 180],
      scissa:         [0, 150, 210],
      analoga:        [0, 330, 30],
      triadica:       [0, 120, 240],
      tetradica:      [0, 90, 180, 270]
    };
    var ORDO_RATIONUM = ['monochromatica', 'complementaria',
      'scissa', 'analoga', 'triadica', 'tetradica'];
    var GRADUS = [
      function (s, v) { return { s: s, v: v }; },
      function (s, v) { return { s: s * 0.45,
                                 v: Math.min(1, v * 1.2) }; },
      function (s, v) { return { s: s * 0.7,
                                 v: Math.min(1, v * 1.08) }; },
      function (s, v) { return { s: Math.min(1, s * 1.05),
                                 v: v * 0.65 }; },
      function (s, v) { return { s: Math.min(1, s * 1.1),
                                 v: v * 0.4 }; }
    ];

    var lectiones    = 0;
    var scripturae_d = 0;
    var picturae     = 0;

    var veritas = null;   /* {basis(hex), ratio, rota} - veritas */

    var proiectio  = null;
    var regula     = null;
    var thumbR     = null;
    var tabula     = null;
    var exportatio = null;
    var numeriEl   = null;
    var trahens    = false;

    function proicere() {
      /* PALETTA = functio pura insulae */
      var c = _color_hex_ad_hsv(veritas.basis)
        || { h: 0, s: 1, v: 1 };
      var artistica = veritas.rota === 'artistica';
      var h_op = artistica ? _rota_ad_artisticam(c.h) : c.h;
      var anguli = RATIONES[veritas.ratio] || RATIONES.triadica;
      var colores = [];
      var i, k, hw, hr, g;

      for (i = 0; i < anguli.length; i++) {
        hw = (h_op + anguli[i]) % 360;
        hr = artistica ? _rota_ad_physicam(hw) : hw;
        var gradus = [];
        for (k = 0; k < GRADUS.length; k++) {
          g = GRADUS[k](c.s, c.v);
          gradus.push(_color_hsv_ad_hex(
            { h: hr, s: g.s, v: g.v }));
        }
        colores.push(gradus);
      }
      return colores;
    }

    function exportationem_serere(colores) {
      var l = ['/* harmonia: ' + veritas.ratio + ', rota '
               + veritas.rota + ', basis ' + veritas.basis + ' */'];
      var i, k;
      for (i = 0; i < colores.length; i++) {
        for (k = 0; k < colores[i].length; k++) {
          l.push('--harmonia-' + (i + 1) + '-' + k + ': '
            + colores[i][k] + ';');
        }
      }
      return l.join('\n');
    }

    function numeros_pingere() {
      if (!numeriEl) { return; }
      numeriEl.textContent =
          'basis: ' + veritas.basis
        + '  ratio: ' + veritas.ratio
        + '  rota: ' + veritas.rota
        + '  lectiones: ' + lectiones
        + '  scripturae-d: ' + scripturae_d
        + '  picturae: ' + picturae;
    }

    function pingere() {
      picturae = picturae + 1;
      var colores = proicere();
      var c = _color_hex_ad_hsv(veritas.basis)
        || { h: 0, s: 1, v: 1 };
      var frag = document.createDocumentFragment();
      var i, k, ordo, q;

      thumbR.style.left = (c.h / 360 * 100) + '%';
      for (i = 0; i < colores.length; i++) {
        ordo = document.createElement('div');
        ordo.className = 'ca-harmonia-ordo';
        for (k = 0; k < colores[i].length; k++) {
          q = document.createElement('span');
          q.className = 'ca-harmonia-quadrum';
          q.style.background = colores[i][k];
          q.title = colores[i][k];
          ordo.appendChild(q);
        }
        frag.appendChild(ordo);
      }
      tabula.innerHTML = '';
      tabula.appendChild(frag);
      exportatio.value = exportationem_serere(colores);

      var pyxides = proiectio.querySelectorAll('.ca-harmonia-ratio');
      for (i = 0; i < pyxides.length; i++) {
        var activa = pyxides[i].getAttribute('data-ratio')
          === veritas.ratio;
        pyxides[i].className = 'ca-harmonia-ratio'
          + (activa ? ' ca-electus' : '');
      }
      proiectio.querySelector('.ca-harmonia-rota').textContent =
        'rota: ' + veritas.rota;
      numeros_pingere();
    }

    function committere() {
      insula_d.textContent = '<harmonia'
        + _att_serere({ basis: veritas.basis, ratio: veritas.ratio,
                        rota: veritas.rota },
                      ['basis', 'ratio', 'rota'])
        + '/>';
      scripturae_d = scripturae_d + 1;
      numeros_pingere();
      _nuntiare_statum();
      _eventum_electum(el, { basis: veritas.basis,
        ratio: veritas.ratio, rota: veritas.rota });
    }

    function basim_ex_h(h) {
      var c = _color_hex_ad_hsv(veritas.basis)
        || { h: 0, s: 1, v: 1 };
      veritas.basis = _color_hsv_ad_hex(
        { h: h, s: c.s, v: c.v });
    }

    function e_puncto_regulae(ev) {
      var q = regula.getBoundingClientRect();
      var h = (ev.clientX - q.left) / q.width * 360;
      basim_ex_h(Math.max(0, Math.min(359.9, h)));
    }
    function adMotum(ev) {
      if (!trahens) { return; }
      e_puncto_regulae(ev);
      pingere();
    }
    function adQuietem() {
      if (!trahens) { return; }
      trahens = false;
      document.removeEventListener('mousemove', adMotum);
      document.removeEventListener('mouseup', adQuietem);
      committere();
    }
    function adClavemRegulae(ev) {
      var c = _color_hex_ad_hsv(veritas.basis)
        || { h: 0, s: 1, v: 1 };
      var gradus = ev.shiftKey ? 30 : 6;
      if (ev.key === 'ArrowRight') {
        basim_ex_h(Math.min(359.9, c.h + gradus));
      } else if (ev.key === 'ArrowLeft') {
        basim_ex_h(Math.max(0, c.h - gradus));
      } else { return; }
      pingere();
      committere();
      ev.preventDefault();
    }

    function construere() {
      var i;
      var rationesHtml = '';
      for (i = 0; i < ORDO_RATIONUM.length; i++) {
        rationesHtml += '<button type="button"'
          + ' class="ca-harmonia-ratio" data-ratio="'
          + ORDO_RATIONUM[i] + '">' + ORDO_RATIONUM[i]
          + '</button>';
      }
      proiectio = document.createElement('div');
      proiectio.className = 'ca-proiectio';
      proiectio.innerHTML =
          (numeri_visibiles ? '<p class="ca-numeri"></p>' : '')
        + (remontabile
            ? '<button type="button" class="ca-remontare">componentem'
              + ' remontare (ex insulis solis)</button>' : '')
        + '<div class="ca-color-regula" tabindex="0" role="slider"'
        +      ' aria-label="color basis">'
        +   '<div class="ca-color-thumb"></div>'
        + '</div>'
        + '<p class="ca-harmonia-rationes">' + rationesHtml
        +   '<button type="button" class="ca-harmonia-rota"></button>'
        + '</p>'
        + '<div class="ca-harmonia-tabula"></div>'
        + '<textarea class="ca-harmonia-exportatio" readonly'
        +          ' rows="9" aria-label="thema CSS"></textarea>';
      el.appendChild(proiectio);

      regula     = proiectio.querySelector('.ca-color-regula');
      thumbR     = regula.querySelector('.ca-color-thumb');
      tabula     = proiectio.querySelector('.ca-harmonia-tabula');
      exportatio = proiectio.querySelector('.ca-harmonia-exportatio');
      numeriEl   = proiectio.querySelector('.ca-numeri');

      lectiones = lectiones + 1;
      var arbor_g = _stml_generale_legere(insula_d.textContent);
      if (arbor_g.tag !== 'harmonia') {
        throw new Error('fons non est <harmonia>: <' + arbor_g.tag
          + '>');
      }
      veritas = {
        basis: /^#[0-9a-fA-F]{6}$/.test(arbor_g.att.basis || '')
          ? arbor_g.att.basis : '#ff0000',
        ratio: RATIONES[arbor_g.att.ratio] ? arbor_g.att.ratio
          : 'triadica',
        rota: arbor_g.att.rota === 'physica' ? 'physica'
          : 'artistica'
      };

      regula.addEventListener('mousedown', function (ev) {
        regula.focus();
        trahens = true;
        document.addEventListener('mousemove', adMotum);
        document.addEventListener('mouseup', adQuietem);
        e_puncto_regulae(ev);
        pingere();
        ev.preventDefault();
      });
      regula.addEventListener('keydown', adClavemRegulae);
      proiectio.querySelector('.ca-harmonia-rationes')
        .addEventListener('click', function (ev) {
          var r = ev.target.getAttribute
            && ev.target.getAttribute('data-ratio');
          if (r && RATIONES[r]) {
            veritas.ratio = r;
            pingere();
            committere();
          }
        });
      proiectio.querySelector('.ca-harmonia-rota')
        .addEventListener('click', function () {
          veritas.rota = veritas.rota === 'artistica'
            ? 'physica' : 'artistica';
          pingere();
          committere();
        });

      var remo = proiectio.querySelector('.ca-remontare');
      if (remo) {
        remo.addEventListener('click', function () {
          destruere();
          construere();
        });
      }

      pingere();
    }

    function destruere() {
      document.removeEventListener('mousemove', adMotum);
      document.removeEventListener('mouseup', adQuietem);
      trahens = false;
      if (proiectio && proiectio.parentNode) {
        proiectio.parentNode.removeChild(proiectio);
      }
      proiectio = null;
      veritas = null;
    }

    try {
      construere();
    } catch (e) {
      el.innerHTML = '<p class="ca-culpa">CULPA: ' + e.message + '</p>';
      return null;
    }
    return destruere;
  }

  /* ============================================================
   * LECTOR - textus fontis lineatim, electio classificata (0019)
   * ============================================================
   * Cibus TEXTUS est, non plagula (decretum Frannis): insula cruda
   * (data-textus, script) fontem VERBATIM fert - unde venerit
   * (plagula per pontem, conditio silicis, massa git, textus
   * genitus) lectoris non interest. Intra <script> grammatica
   * entitates numquam legit (lectio texturae), ergo '<' et '&'
   * fontis sine effugio vivunt; limes solus: fons '</script'
   * continere non potest.
   *
   * Insulae: data-fons = <lector nomen=""/> (durabilis - lumina
   * et notae in gradibus posterioribus hic habitabunt); ephemera
   * <ephemera initium="l:c" finis="l:c"/> = electio SEDATA
   * (ordine documenti normata; vacua = nulla).
   *
   * ELECTIO ALIENA IGNORATUR: apparatus electionis paginae unus
   * est, sed lector electionem SUAM meminit ut campus textus
   * nativus - electio alibi facta statum eius non delet. Mutant
   * eam solae electiones intra elementum (collapsa intra = clara).
   * Rehydratio: semen insulae marginem pingit SINE apparatu -
   * insula veritas picturae est, apparatus manus inserendi sola.
   *
   * Classificatio per lineam (regula una, sine casibus primae/
   * ultimae): tectum [ab, ad] contra longitudinem - totus (linea
   * tota), caput (a fronte), cauda (ad finem), medius (interior).
   *
   * Disciplinae graduum: pictura marginis VIVA per eventum
   * (kinetica); insula in QUIETE scribitur (200ms - scripturae-e
   * eam numerat). Privationes nominatae: sine virtualizatione
   * (0002 eam novit - compositio posterior), sine colore
   * syntaxis (silva per pontem, gradus posterior).
   *
   * LUMINA (0020, gradus scholaris II): filii <lumen initium
   * finis> insulae durabilis = illuminationes persistentes,
   * segmentis DOM pictae (non arithmetica pixelorum - veritas
   * structurae asseri potest). Enter electione APPARATUS viva
   * (memorata pingit, non armat - lectores plures componuntur)
   * lumen creat; superpositio RECUSATUR clare; ictus cum Alt
   * delet. Ancorae columnas a nativitate ferunt - vitium
   * primae-occursionis scholaris natum mortuum. Sigillum textus
   * (deflexio fontis sub luminibus) privatio nominata - gradus
   * posterior.
   *
   * MARGINALIA (0021, gradus scholaris III): filii <nota initium
   * finis>textus</nota> (ancorae LINEATIM - sectiones lineae
   * integrae sunt) = commentarii iuxta codicem. SECTIONES
   * PROIECTIO SUNT, non repositum (decretum Sententiarum):
   * scholar sectionem sectam CONDEBAT (ante/creata/post mutatis
   * ordinibus); hic veritas = notae solae, sectiones quaque
   * pictura ex (lineis, notis) derivantur - parser crescit, data
   * numquam migrant. Shift+Enter electione apparatus notam creat
   * (vacuam, scriptorio statim aperto); superpositio recusatur;
   * bis-ictus chartam edit; Alt+ictus delet. CONTRACTUS
   * SCRIPTORII MULTILINEARIS (novus, alter ab optione): CAECITAS
   * committit, Escape revertit - Enter enim contentum est (lineae
   * novae), non actus. Eventa 'nota' {actus nata|mutata|deleta}. */
  function lectorem_figere(el) {
    var numeri_visibiles = (el.getAttribute('data-numeri') === 'verum');

    var insula_d = _fontem_resolvere(el);
    if (!insula_d) { return null; }

    var sel_t = el.getAttribute('data-textus');
    var insula_t = sel_t ? document.querySelector(sel_t) : null;
    if (!insula_t) {
      _culpam_ponere(el, sel_t ? ('textus non inventus: ' + sel_t)
                               : 'data-textus abest');
      return null;
    }

    var insulae_e = _ephemeras_parare(el, [
      { titulus: 'initium', genus: 'c', praefinitum: '' },
      { titulus: 'finis',   genus: 'c', praefinitum: '' }
    ]);

    /* STRATUM DERIVATUM (0023): data-color methodum pontis
       NOMINAT (marcatio nominat, numquam computat) - absente
       attributo, color abest (privatio libera). Responsa cum
       stampa aliena ABICIUNTUR (lectio Xi: strata derivata
       versione stampata, nucleus synchronus, asynchronia ad
       limitem pontis solum). Colores numquam in insulam - stratum
       derivatum proiectio est, non veritas. */
    var methodus_coloris = el.getAttribute('data-color');

    var nomen        = '';
    var lineae       = [];     /* textus per lineam (crudus) */
    var electio      = null;   /* {a:{linea,columna}, b:{...}} | null */
    var lumina       = [];     /* durabilia - [{a, b}] ex insula */
    var notae        = [];     /* durabilia - [{initium, finis, textus}] */
    var notam_edens  = null;   /* nota CUIUS scriptorium apertum est */
    var colores      = null;   /* derivata - [[{ab,ad,cl}]] indices JS */
    var tecta_c      = 0;
    var lectiones    = 0;
    var scripturae_e = 0;
    var scripturae_d = 0;

    var proiectio = null, corpusEl = null, numeriEl = null;
    var nuntiusEl = null;
    var mora_sedationis = null;

    function textum_legere() {
      var l = insula_t.textContent.split('\n');
      /* forma auctorata: linea prima (post tag) et novissima
         (ante clausulam) vacuae artefacta marcationis sunt */
      if (l.length && l[0] === '') { l.shift(); }
      if (l.length && l[l.length - 1] === '') { l.pop(); }
      return l;
    }

    function electionem_solvere(e) {
      function pars(s) {
        var m = /^(\d+):(\d+)$/.exec(s);
        if (!m) { return null; }
        return { linea: parseInt(m[1], 10),
                 columna: parseInt(m[2], 10) };
      }
      var a = pars(e.initium), b = pars(e.finis);
      if (!a || !b) { return null; }
      return { a: a, b: b };
    }

    /* tectum lineae sub pari ancorarum: [ab, ad) contra textum.
       Ancorae octetos ferunt (decretum 01M0ATF1E1); pictura in
       indicibus JS vivit - conversio HIC, ad limitem (vitium ab
       0025 repertum: ante hoc columnae octetorum ut indices
       adhibebantur - 'æ' ante lumen segmentum transponebat;
       0019-0024 lumina in lineis ASCII solis serebant, ubi
       conversio identitas est) */
    function _tectum(linea_n, par) {
      var t = lineae[linea_n - 1] || '';
      var ab_o = (linea_n === par.a.linea) ? par.a.columna : 0;
      var ad_o = (linea_n === par.b.linea) ? par.b.columna : Infinity;
      var par2 = _octeti_ad_indices(t, ab_o, ad_o);
      return { ab: par2.ab, ad: par2.ad, longitudo: t.length };
    }

    function classis(linea_n) {
      var t = _tectum(linea_n, electio);
      if (t.ab === 0 && t.ad >= t.longitudo) { return 'totus'; }
      if (t.ab === 0) { return 'caput'; }
      if (t.ad >= t.longitudo) { return 'cauda'; }
      return 'medius';
    }

    function _comparare(p, q) {
      if (p.linea !== q.linea) { return p.linea - q.linea; }
      return p.columna - q.columna;
    }
    function _intersecat(x, y) {
      return _comparare(x.a, y.b) < 0 && _comparare(y.a, x.b) < 0;
    }

    function numeros_pingere() {
      var electae = electio
        ? (electio.b.linea - electio.a.linea + 1) : 0;
      if (!numeriEl) { return; }
      numeriEl.textContent = 'lineae: ' + lineae.length
        + ' · electae: ' + electae
        + ' · lumina: ' + lumina.length
        + ' · notae: ' + notae.length
        + ' · sectiones: ' + sectiones_derivare().length
        + ' · tecta-c: ' + tecta_c
        + ' · lectiones: ' + lectiones
        + ' · scripturae-d: ' + scripturae_d
        + ' · scripturae-e: ' + scripturae_e;
    }

    /* SECTIONES = PROIECTIO (decretum Sententiarum): ex (lineis,
       notis) quaque pictura derivantur, numquam conduntur -
       scholar sectionem sectam condebat et migratio datorum eum
       sequebatur. Notae per initium ordinatae; hiatus sectiones
       anonymae. */
    function sectiones_derivare() {
      var ordinatae = notae.slice().sort(function (x, y) {
        return x.initium - y.initium;
      });
      var s = [];
      var cursor = 1;
      var i, n;
      for (i = 0; i < ordinatae.length; i++) {
        n = ordinatae[i];
        if (n.initium > cursor) {
          s.push({ ab: cursor, ad: n.initium - 1, nota: null });
        }
        s.push({ ab: n.initium, ad: n.finis, nota: n });
        cursor = n.finis + 1;
      }
      if (cursor <= lineae.length) {
        s.push({ ab: cursor, ad: lineae.length, nota: null });
      }
      return s;
    }

    function marginem_pingere() {
      var ordines = corpusEl.querySelectorAll('.ca-lector-linea');
      var i, n, cls, numerus;
      for (i = 0; i < ordines.length; i++) {
        n = i + 1;
        cls = 'ca-lector-numerus';
        if (electio && n >= electio.a.linea && n <= electio.b.linea) {
          cls += ' ca-lc-' + classis(n);
        }
        numerus = ordines[i].firstChild;
        if (numerus.className !== cls) { numerus.className = cls; }
      }
      numeros_pingere();
    }

    /* lumina segmentis DOM pinguntur: spatium textus lineae per
       COMPOSITOREM (stratificare) secatur - veritas structurae
       (textus segmenti asseri potest), non arithmetica pixelorum.
       Stratum unum hodie (lumina); color syntaxis, diagnostica,
       cautiones = tecta plura eidem compositori tradita, pictore
       immutato. */
    function omnes_lineas_pingere() {
      var ordines = corpusEl.querySelectorAll('.ca-lector-linea');
      var n, i, k, cl, sp, t, tecta, seg, segmenta, frag, nodus;
      for (n = 1; n <= ordines.length; n++) {
        sp = ordines[n - 1].querySelector('.ca-lector-textus');
        t = lineae[n - 1] || '';
        tecta = [];
        /* stratum coloris PRIMUM (sub luminibus in acervo) */
        if (colores && colores[n - 1]) {
          for (i = 0; i < colores[n - 1].length; i++) {
            tecta.push({ ab: colores[n - 1][i].ab,
                         ad: colores[n - 1][i].ad,
                         classis: 'ca-sy-' + colores[n - 1][i].cl });
          }
        }
        for (i = 0; i < lumina.length; i++) {
          if (n >= lumina[i].a.linea && n <= lumina[i].b.linea) {
            seg = _tectum(n, lumina[i]);
            tecta.push({ ab: seg.ab, ad: seg.ad,
                         classis: 'ca-lumen',
                         attributa: { 'data-lumen': String(i) } });
          }
        }
        if (!tecta.length) {
          if (sp.childNodes.length !== 1
              || sp.firstChild.nodeType !== 3
              || sp.firstChild.nodeValue !== t) {
            sp.textContent = t;
          }
          continue;
        }
        segmenta = stratificare(t.length, tecta);
        frag = document.createDocumentFragment();
        for (k = 0; k < segmenta.length; k++) {
          seg = segmenta[k];
          if (!seg.classes.length) {
            frag.appendChild(document.createTextNode(
              t.substring(seg.ab, seg.ad)));
          } else {
            nodus = document.createElement('span');
            nodus.className = seg.classes.join(' ');
            for (cl in seg.attributa) {
              nodus.setAttribute(cl, seg.attributa[cl]);
            }
            nodus.textContent = t.substring(seg.ab, seg.ad);
            frag.appendChild(nodus);
          }
        }
        sp.innerHTML = '';
        sp.appendChild(frag);
      }
    }

    function omnes_pingere() {
      marginem_pingere();
      omnes_lineas_pingere();
    }

    /* corpus totum ex proiectione sectionum reficitur.
       ORDO DOCUMENTI != ORDO VISUS (mensura Frannis, 0022b):
       forma prior [margo | lineae] per sectionem chartam INTER
       ordines lineatos ponebat, et electio nativa trans finem
       sectionis chartam VERRIT - textus notae in electionem et
       exscriptionem codicis cadebat. Nunc CRATIS (grid): ordines
       lineati OMNES contigui in documento (electio codicis
       chartam attingere non potest), chartae POST omnes ordines,
       collocatione explicita in columnam marginis positae.
       Ordines = semitae fixae (1.5em); post sectionem notatam
       semita 'auto' spatiosa - charta alta sectionem SEQUENTEM
       deprimit, lineas suas non dispergit (virtus scholaris
       servata, sine mensura). Chartae seorsum eligibiles manent
       (user-select:none consulto VITATUM - nota contentum est). */
    function corpus_pingere() {
      var s = sectiones_derivare();
      var habet_margines = notae.length > 0;
      var semitae = [];    /* grid-template-rows */
      var cartae = [];     /* {nota, ab, ad} - ordines gridis */
      var k, i, sect, ordo, numerus, textusEl, charta, scr;
      var ordo_gridis = 1;
      var columna_codicis = habet_margines ? '2' : '1';

      corpusEl.innerHTML = '';
      corpusEl.style.gridTemplateColumns = '';
      corpusEl.style.gridTemplateRows = '';
      corpusEl.className = 'ca-lector-corpus'
        + (habet_margines ? ' ca-lector-marginatus' : '');

      /* ordines lineati PRIMI, contigui */
      for (k = 0; k < s.length; k++) {
        sect = s[k];
        var ab_gridis = ordo_gridis;
        for (i = sect.ab; i <= sect.ad; i++) {
          ordo = document.createElement('div');
          ordo.className = 'ca-lector-linea';
          ordo.setAttribute('data-linea', String(i));
          if (habet_margines) {
            ordo.style.gridRow = String(ordo_gridis);
            ordo.style.gridColumn = columna_codicis;
          }
          semitae.push('1.5em');
          ordo_gridis = ordo_gridis + 1;
          numerus = document.createElement('span');
          numerus.className = 'ca-lector-numerus';
          numerus.textContent = String(i);
          textusEl = document.createElement('span');
          textusEl.className = 'ca-lector-textus';
          textusEl.textContent = lineae[i - 1];
          ordo.appendChild(numerus);
          ordo.appendChild(textusEl);
          corpusEl.appendChild(ordo);
        }
        if (sect.nota) {
          /* semita spatiosa: excessum chartae altae absorbet */
          semitae.push('auto');
          cartae.push({ nota: sect.nota, ab: ab_gridis,
                        ad: ordo_gridis });
          ordo_gridis = ordo_gridis + 1;
        }
      }

      /* chartae POST ordines omnes (ordo documenti); collocatio
         explicita eas in marginem VISU ponit */
      for (k = 0; k < cartae.length; k++) {
        sect = cartae[k];
        if (notam_edens === sect.nota) {
          scr = document.createElement('textarea');
          scr.className = 'ca-nota-scriptorium';
          scr.rows = 3;
          scr.value = sect.nota.textus;
          scr.addEventListener('keydown', adClavemScriptorii);
          scr.addEventListener('blur', adCaecitatemScriptorii);
          charta = scr;
        } else {
          charta = document.createElement('div');
          charta.className = 'ca-nota-charta'
            + (sect.nota.textus === '' ? ' ca-nota-vacua' : '');
          charta.setAttribute('data-nota',
            String(notae.indexOf(sect.nota)));
          charta.textContent = sect.nota.textus === ''
            ? 'nota vacua - bis-ictu scribe' : sect.nota.textus;
        }
        charta.style.gridColumn = '1';
        charta.style.gridRow = sect.ab + ' / ' + (sect.ad + 1);
        corpusEl.appendChild(charta);
      }
      if (habet_margines) {
        corpusEl.style.gridTemplateColumns = '16em 1fr';
        corpusEl.style.gridTemplateRows = semitae.join(' ');
      }

      omnes_pingere();
      scr = corpusEl.querySelector('.ca-nota-scriptorium');
      if (scr) { scr.focus(); }
    }

    function nuntium_ponere(t) {
      if (nuntiusEl) { nuntiusEl.textContent = t; }
    }

    function _textum_effugere(t) {
      return String(t).replace(/&/g, '&amp;')
        .replace(/</g, '&lt;').replace(/>/g, '&gt;');
    }

    /* insula durabilis rescripta - porta scribendi (0007) */
    function insulam_scribere() {
      var l = ['<lector' + _att_serere({ nomen: nomen }, ['nomen'])
               + '>'];
      var i, p;
      for (i = 0; i < lumina.length; i++) {
        p = lumina[i];
        l.push('  <lumen' + _att_serere({
          initium: p.a.linea + ':' + p.a.columna,
          finis:   p.b.linea + ':' + p.b.columna
        }, ['initium', 'finis']) + '/>');
      }
      for (i = 0; i < notae.length; i++) {
        p = notae[i];
        l.push('  <nota' + _att_serere({
          initium: String(p.initium),
          finis:   String(p.finis)
        }, ['initium', 'finis']) + '>'
          + _textum_effugere(p.textus) + '</nota>');
      }
      l.push('</lector>');
      insula_d.textContent = l.join('\n');
      scripturae_d = scripturae_d + 1;
      numeros_pingere();
      _nuntiare_statum();
    }

    function _eventum_lumen(actus, par) {
      el.dispatchEvent(new CustomEvent('lumen', {
        bubbles: true,
        detail: { actus: actus,
                  initium: par.a.linea + ':' + par.a.columna,
                  finis: par.b.linea + ':' + par.b.columna }
      }));
    }

    function lumen_creare() {
      var i;
      if (!electio) { return; }
      for (i = 0; i < lumina.length; i++) {
        if (_intersecat(electio, lumina[i])) {
          nuntium_ponere('lumen superpositum recusatum');
          return;   /* RECUSATIO CLARA - superpositio non fit */
        }
      }
      lumina.push({ a: electio.a, b: electio.b });
      insulam_scribere();
      _eventum_lumen('natum', lumina[lumina.length - 1]);
      nuntium_ponere('');
      /* electio consumpta: memoria ET apparatus vacantur */
      electio = null;
      if (document.getSelection()) {
        document.getSelection().removeAllRanges();
      }
      omnes_pingere();
      sedare();
    }

    function lumen_delere(index) {
      var p = lumina[index];
      if (!p) { return; }
      lumina.splice(index, 1);
      insulam_scribere();
      _eventum_lumen('deletum', p);
      nuntium_ponere('');
      omnes_pingere();
    }

    /* ---------- notae (marginalia, 0021) ---------- */

    function _eventum_nota(actus, n) {
      el.dispatchEvent(new CustomEvent('nota', {
        bubbles: true,
        detail: { actus: actus,
                  initium: String(n.initium),
                  finis: String(n.finis) }
      }));
    }

    function nota_creare() {
      var i, nova;
      if (!electio) { return; }
      for (i = 0; i < notae.length; i++) {
        if (electio.a.linea <= notae[i].finis
            && notae[i].initium <= electio.b.linea) {
          nuntium_ponere('nota superposita recusata');
          return;
        }
      }
      nova = { initium: electio.a.linea, finis: electio.b.linea,
               textus: '' };
      notae.push(nova);
      insulam_scribere();
      _eventum_nota('nata', nova);
      nuntium_ponere('');
      electio = null;
      if (document.getSelection()) {
        document.getSelection().removeAllRanges();
      }
      /* scriptorium statim aperitur (mos scholaris) */
      notam_edens = nova;
      corpus_pingere();
      sedare();
    }

    function nota_delere(index) {
      var n = notae[index];
      if (!n) { return; }
      if (notam_edens === n) { notam_edens = null; }
      notae.splice(index, 1);
      insulam_scribere();
      _eventum_nota('deleta', n);
      nuntium_ponere('');
      corpus_pingere();
    }

    /* CONTRACTUS SCRIPTORII MULTILINEARIS (alter ab optione 0014,
       consulto): CAECITAS committit, Escape revertit - Enter enim
       hic contentum est (lineae novae), non actus. */
    function adCaecitatemScriptorii(ev) {
      var n = notam_edens;
      if (!n) { return; }
      notam_edens = null;
      if (ev.target.value !== n.textus) {
        n.textus = ev.target.value;
        insulam_scribere();
        _eventum_nota('mutata', n);
      }
      corpus_pingere();
    }

    function adClavemScriptorii(ev) {
      if (ev.key === 'Escape') {
        /* revertere: notam_edens ANTE vacatur ne remotio
           scriptorii per caecitatem committat */
        notam_edens = null;
        corpus_pingere();
        ev.stopPropagation();
      }
    }

    /* insula in QUIETE - non per eventum (disciplina kinetica) */
    function sedare() {
      if (mora_sedationis) { clearTimeout(mora_sedationis); }
      mora_sedationis = setTimeout(function () {
        mora_sedationis = null;
        insulae_e.scribere({
          initium: electio
            ? electio.a.linea + ':' + electio.a.columna : '',
          finis: electio
            ? electio.b.linea + ':' + electio.b.columna : ''
        });
        scripturae_e = scripturae_e + 1;
        numeros_pingere();
        _nuntiare_statum();
      }, 200);
    }

    function _sedes(nodus, offset) {
      var n = nodus;
      var columna = 0;
      var p, s, linea_n;
      while (n && n !== el
             && !(n.getAttribute && n.getAttribute('data-linea'))) {
        n = n.parentNode;
      }
      if (!n || n === el || !n.getAttribute) { return null; }
      if (nodus.nodeType === 3) {
        /* columna = offset + praecedentia intra spatium textus
           (segmenta luminum textum in nodos plures secant) */
        columna = offset;
        p = nodus;
        while (p.parentNode
               && !(p.parentNode.className && String(
                   p.parentNode.className)
                   .indexOf('ca-lector-textus') >= 0)) {
          p = p.parentNode;
        }
        s = p.previousSibling;
        while (s) {
          columna = columna + (s.textContent || '').length;
          s = s.previousSibling;
        }
      }
      /* columna adhuc index DOM est - in octetos ad limitem
         (ancorae octetos ferunt, decretum 01M0ATF1E1) */
      linea_n = parseInt(n.getAttribute('data-linea'), 10);
      return { linea: linea_n,
               columna: _octeti_ex_indice(
                 lineae[linea_n - 1] || '', columna) };
    }

    function adElectionem() {
      var s = document.getSelection();
      var a, b, t;
      if (!s || s.rangeCount === 0
          || !el.contains(s.anchorNode)
          || !el.contains(s.focusNode)) {
        return;   /* aliena - lector suam meminit */
      }
      if (s.isCollapsed) {
        electio = null;
      } else {
        a = _sedes(s.anchorNode, s.anchorOffset);
        b = _sedes(s.focusNode, s.focusOffset);
        if (!a || !b) {
          electio = null;
        } else {
          if (b.linea < a.linea
              || (b.linea === a.linea && b.columna < a.columna)) {
            t = a; a = b; b = t;
          }
          electio = { a: a, b: b };
        }
      }
      marginem_pingere();   /* VIVUM per eventum */
      sedare();             /* insula in quiete */
    }

    /* Enter = lumen, Shift+Enter = nota - ambo ex electione
       APPARATUS viva. Memorata NON armat (semen rehydratum nihil
       parit) - apparatus unus est, ergo lector unus solus quovis
       tempore armatus: compositio gratis. */
    function adClavem(ev) {
      var t, s;
      if (ev.key !== 'Enter' || ev.metaKey || ev.ctrlKey
          || ev.altKey) { return; }
      t = document.activeElement ? document.activeElement.tagName : '';
      if (t === 'INPUT' || t === 'TEXTAREA' || t === 'SELECT') {
        return;
      }
      s = document.getSelection();
      if (!s || s.rangeCount === 0 || s.isCollapsed
          || !el.contains(s.anchorNode)
          || !el.contains(s.focusNode)) { return; }
      if (ev.shiftKey) { nota_creare(); }
      else { lumen_creare(); }
    }

    /* ictus cum Alt = deletio (mos scholaris): in segmento
       luminis aut in charta notae; sine Alt ictus ordinarius */
    function adIctum(ev) {
      var t = ev.target;
      if (!ev.altKey) { return; }
      while (t && t !== corpusEl
             && !(t.className && String(t.className)
                 .indexOf('ca-lumen') >= 0)) {
        t = t.parentNode;
      }
      if (t && t !== corpusEl && t.getAttribute) {
        lumen_delere(parseInt(t.getAttribute('data-lumen'), 10));
        return;
      }
      t = ev.target;
      while (t && t !== corpusEl
             && !(t.className && String(t.className)
                 .indexOf('ca-nota-charta') >= 0)) {
        t = t.parentNode;
      }
      if (t && t !== corpusEl && t.getAttribute) {
        nota_delere(parseInt(t.getAttribute('data-nota'), 10));
      }
    }

    /* bis-ictus in charta = scriptorium aperire */
    function adBisIctum(ev) {
      var t = ev.target;
      while (t && t !== corpusEl
             && !(t.className && String(t.className)
                 .indexOf('ca-nota-charta') >= 0)) {
        t = t.parentNode;
      }
      if (!t || t === corpusEl || !t.getAttribute) { return; }
      notam_edens = notae[parseInt(t.getAttribute('data-nota'), 10)];
      corpus_pingere();
    }

    function construere() {
      var arbor_g = _stml_generale_legere(insula_d.textContent);
      var i, ordo, numerus, textus, caput, f, par;
      if (arbor_g.tag !== 'lector') {
        throw new Error('fons non est <lector>: <' + arbor_g.tag
          + '>');
      }
      nomen = arbor_g.att.nomen || '';
      lumina = [];
      notae  = [];
      for (i = 0; i < arbor_g.filii.length; i++) {
        f = arbor_g.filii[i];
        if (f.tag === 'lumen') {
          par = electionem_solvere({ initium: f.att.initium || '',
                                     finis: f.att.finis || '' });
          if (!par) {
            throw new Error('lumen sine ancoris (initium/finis)');
          }
          lumina.push(par);
        } else if (f.tag === 'nota') {
          var ini = parseInt(f.att.initium || '', 10);
          var fin = parseInt(f.att.finis || '', 10);
          if (!(ini >= 1) || !(fin >= ini)) {
            throw new Error('nota sine ancoris (initium/finis)');
          }
          notae.push({ initium: ini, finis: fin,
                       textus: f.textus || '' });
        } else {
          throw new Error('filius ignotus in <lector>: <'
            + f.tag + '>');
        }
      }
      lineae = textum_legere();
      lectiones = lectiones + 1;

      proiectio = document.createElement('div');
      proiectio.className = 'ca-lector';
      if (nomen !== '') {
        caput = document.createElement('div');
        caput.className = 'ca-lector-caput';
        caput.textContent = nomen;
        proiectio.appendChild(caput);
      }
      corpusEl = document.createElement('div');
      corpusEl.className = 'ca-lector-corpus';
      proiectio.appendChild(corpusEl);
      nuntiusEl = document.createElement('div');
      nuntiusEl.className = 'ca-lector-nuntius';
      proiectio.appendChild(nuntiusEl);
      if (numeri_visibiles) {
        numeriEl = document.createElement('div');
        numeriEl.className = 'ca-numeri';
        proiectio.appendChild(numeriEl);
      }
      el.appendChild(proiectio);

      /* rehydratio: semen insulae pingit SINE apparatu electionis
         - insula veritas picturae, apparatus manus inserendi.
         corpus ex proiectione sectionum (0021) construitur. */
      electio = electionem_solvere(insulae_e.legere());
      corpus_pingere();

      corpusEl.addEventListener('click', adIctum);
      corpusEl.addEventListener('dblclick', adBisIctum);
      document.addEventListener('selectionchange', adElectionem);
      document.addEventListener('keydown', adClavem);

      /* stratum derivatum petere (asynchronia ad limitem pontis;
         nucleus iam pictus - color adveniens repingit) */
      if (methodus_coloris && window.internuntius) {
        (function (stampa) {
          internuntius.vocare(methodus_coloris, {
            textus: lineae.join('\n'),
            stampa: stampa
          }).then(function (r) {
            var n2, j, orig, conv, par;
            if (stampa !== lectiones || r.stampa !== stampa) {
              return;   /* stalia abiecta (lectio Xi) */
            }
            colores = [];
            tecta_c = 0;
            for (n2 = 0; n2 < lineae.length; n2++) {
              orig = (r.lineae && r.lineae[n2]) || [];
              conv = [];
              for (j = 0; j < orig.length; j++) {
                /* octeti C -> indices JS (decretum columnarum) */
                par = _octeti_ad_indices(lineae[n2],
                  orig[j].ab, orig[j].ad);
                conv.push({ ab: par.ab, ad: par.ad,
                            cl: orig[j].cl });
                tecta_c = tecta_c + 1;
              }
              colores.push(conv);
            }
            omnes_lineas_pingere();
            numeros_pingere();
          }).catch(function (e) {
            nuntium_ponere('color: ' + e.message);
          });
        }(lectiones));
      }
    }

    function destruere() {
      document.removeEventListener('selectionchange', adElectionem);
      document.removeEventListener('keydown', adClavem);
      if (mora_sedationis) {
        clearTimeout(mora_sedationis);
        mora_sedationis = null;
      }
      if (proiectio && proiectio.parentNode) {
        proiectio.parentNode.removeChild(proiectio);
      }
      proiectio = null;
    }

    try {
      construere();
    } catch (e) {
      el.innerHTML = '<p class="ca-culpa">CULPA: ' + e.message + '</p>';
      return null;
    }
    return destruere;
  }

  /* ============================================================
   * FIGERE - scrutatio et dispensatio
   * ============================================================ */
  var fabricae = { arbor: arborem_figere, index: indicem_figere,
                   tabella: tabellam_figere,
                   optio: optionem_figere,
                   color: colorem_figere,
                   harmonia: harmoniam_figere,
                   lector: lectorem_figere };

  function figere(radix) {
    var l = radix.querySelectorAll('[data-componens]');
    var tollenda = [];
    var i, e, nomen, f, t;
    for (i = 0; i < l.length; i++) {
      e = l[i];
      nomen = e.getAttribute('data-componens');
      f = fabricae[nomen];
      if (!f) {
        /* CULPA IN ELEMENTO, non in consola sola. */
        e.innerHTML = '<p class="ca-culpa">CULPA: componens ignotum "'
          + nomen + '" (nota: ' + Object.keys(fabricae).join(', ')
          + ')</p>';
        continue;
      }
      t = f(e);
      if (typeof t === 'function') { tollenda.push(t); }
    }
    return function () {
      var k;
      for (k = 0; k < tollenda.length; k++) {
        try { tollenda[k](); } catch (err) {
          if (window.console) { console.error('tollere: ' + err.message); }
        }
      }
    };
  }

  /* ============================================================
   * ELEMENTUM CONSUETUM (0009) - eadem fabrica, cyclus vitae gratis
   * ============================================================
   * <arbor-insularum> = arborem_figere in veste elementi consueti.
   * UNA implementatio, DUO modi figendi: scrutatio (data-componens,
   * figere manuale) et elementum consuetum (connectedCallback figit,
   * disconnectedCallback tollit - disciplina tollendi tota in
   * remotionem DOM structuraliter ligata).
   *
   * SINE UMBRA: attachShadow numquam - filii in DOM lucis, textura
   * omnia videt, manus videns manet. Elementa consueta et umbra res
   * ORTHOGONALES sunt quae confundi solent.
   *
   * Classis ES6 hic sola (customElements v1 constructorem verum
   * poscit); interiora ES5 manent. Attributa eadem data-* ac modus
   * scrutationis - contractus unus; ergonomia postea iteratur.
   *
   * __arbor_viva: numerus instantiarum CONNEXARUM - mensura cycli
   * vitae, a probatione legenda. Reconexio remontat (tollere nullo
   * posito), ergo elementum MOTUM ex insulis suis sponte resurgit. */
  /* NOMINA ELEMENTORUM NON HIC DEFINIUNTUR (ab vendicatione,
   * 2026-08-17): customElements.define SEMEL per nomen per paginam
   * currit, et exemplaria vendita plura idem nomen ferrent. Nomina
   * contractus paginae GLOBALES sunt, ergo involucra dispensantia in
   * CRUSTA vivunt (laboratorium.js) et per window.componentia ad
   * exemplar experimentis ACTIVI mittunt - nomina stabilia, mores ex
   * plica. */

  return { figere: figere, _fabricae: fabricae,
           stratificare: stratificare };
}());

/* REGISTRATIO PRO VENDICATIONE: exemplar hoc se sub id experimentis
 * onerantis registrat (clavis ab oneratore posita). Extra
 * laboratorium (usus futurus alibi) window.componentia sufficit. */
if (window.laboratorium && window.laboratorium.componentiam_ponere) {
  window.laboratorium.componentiam_ponere(window.componentia);
}

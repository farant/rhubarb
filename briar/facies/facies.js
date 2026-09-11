/* facies.js - unicum scriptum paginae (briar -html, spec par. 4.6).
 *
 * MUNUS UNUM: symbolum derivatum tactum declarationem suam ostendit.
 * Cetera omnia - nexus fragmentorum, usus, margo, plicae contexti -
 * markup et CSS sunt et SINE hoc scripto vivunt. Si hoc deficit,
 * pagina integra manet: id est lex.
 *
 * TRES GRADUS (par. 4.7 S4), quisque in suo loco:
 *   pons adest (spectator) -> C definitionem VERAM ex corpore parsat
 *   insula sola (navigator) -> caput et signatura ex pagina ipsa
 *   neutrum -> pagina adhuc legitur, ancoris suis
 * Ergo plagulae hae TRES eaedem spectatori et navigatori serviunt.
 *
 * Nulla bibliotheca, nihil petitum, nihil repositum.
 */
(function () {
    'use strict';

    var insula = document.getElementById('fr-symbola');
    var symbola;
    var tabella = null;

    if (!insula) { return; }
    try {
        symbola = JSON.parse(insula.textContent || '{}');
    } catch (e) {
        return;                     /* insula fracta: pagina sufficit */
    }
    if (!symbola || typeof symbola !== 'object') { return; }

    function tabellam_parare () {
        var t = document.createElement('div');
        var claudere = document.createElement('button');

        t.id = 'fr-tabella';
        t.hidden = true;
        t.setAttribute('role', 'dialog');
        claudere.type = 'button';
        claudere.className = 'fr-t-claudere';
        claudere.textContent = '×';
        claudere.setAttribute('aria-label', 'claudere');
        claudere.addEventListener('click', abscondere);
        t.appendChild(claudere);
        document.body.appendChild(t);
        return t;
    }

    function abscondere () {
        if (tabella) { tabella.hidden = true; }
    }

    function ostendere (titulus, datum) {
        var nomen;
        var caput;
        var typus;

        if (!tabella) { tabella = tabellam_parare(); }
        while (tabella.childNodes.length > 1) {
            tabella.removeChild(tabella.lastChild);
        }
        nomen = document.createElement('span');
        nomen.className = 'fr-t-nomen';
        nomen.textContent = titulus;
        tabella.appendChild(nomen);

        if (datum.caput) {
            caput = document.createElement('span');
            caput.className = 'fr-t-caput';
            caput.textContent = '  —  ' + datum.caput;
            tabella.appendChild(caput);
        }
        if (datum.typus) {
            typus = document.createElement('code');
            typus.className = 'fr-t-typus';
            typus.textContent = datum.typus;
            tabella.appendChild(typus);
        }
        tabella.hidden = false;
        definitionem_petere(titulus, datum);
    }

    /* gradus primus: pons spectatoris. Absente eo nihil fit et
     * insula (iam ostensa) sufficit. */
    function definitionem_petere (titulus, datum) {
        if (typeof internuntius === 'undefined'
                || !internuntius.vocare || !datum.caput) {
            return;
        }
        internuntius.vocare('facies.symbolum',
            {titulus: titulus, caput: datum.caput}).then(
            function (r) {
                var corpus;

                if (!r || r.absens || !r.definitio) { return; }
                if (tabella.hidden) { return; }
                corpus = document.createElement('pre');
                corpus.className = 'fr-t-definitio';
                corpus.textContent = r.definitio;
                tabella.appendChild(corpus);
            },
            function () { /* pons tacuit: insula sufficit */ });
    }

    document.addEventListener('click', function (eventus) {
        var nodus = eventus.target;

        while (nodus && nodus !== document.body) {
            if (nodus.hasAttribute && nodus.hasAttribute('data-s')) {
                var titulus = nodus.getAttribute('data-s');

                if (Object.prototype.hasOwnProperty.call(symbola,
                        titulus)) {
                    ostendere(titulus, symbola[titulus]);
                    eventus.preventDefault();
                }
                return;
            }
            if (nodus === tabella) { return; }
            nodus = nodus.parentNode;
        }
        abscondere();
    });

    document.addEventListener('keydown', function (eventus) {
        if (eventus.key === 'Escape') { abscondere(); }
    });
}());

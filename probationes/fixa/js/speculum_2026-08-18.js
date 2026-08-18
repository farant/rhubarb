/* speculum.js - velamen modi-debug (Phasis B)
 *
 * Contractus cum latere C (lib/speculum.c, stipes se-sanans):
 *   - si window.speculum_praesens iam verum, stipes solum
 *     speculum_commutare() vocat - hic corpus non iterum currit
 *   - window.speculum_stylus_fons = CSS ut chorda (ante nos posita)
 *   - effusio datorum: internuntius.vocare("speculum_obtinere")
 *     -> { proventus, fontes, documenta, exclusa }
 *
 * Stylus: identificatores Latine, snake_case; claves JS Anglice
 * (lingua aliena est - vide worklog). Sine shadow DOM; tags
 * consuetae (fons-arbor, fons-visor) SINE registratione - elementa
 * ignota styli capiunt, registratio cum bibliotheca componentium
 * graduabitur.
 */

window.speculum_praesens = true;

(function () {
    var res = {
        effusio: null,
        plagulae: [],          /* {via, corpus, magnitudo, genus} */
        per_viam: {},
        structum: false,
        colligens: false,
        apertum: false,
        velamen: null,
        arbor: null,
        visor: null,
        de_tabella: null,
        corpus_fons: null,
        quaestio_campus: null,
        via_aperta: null,
        linea_electa: null,
        plagula_electa: null,
        minuscula: {},         /* via -> corpus minusculum (pigre) */
        lineae_initia: {},     /* via -> [offsets initiorum linearum] */
        claves_latinae: null,
        mora_quaestionis: null
    };
    window.speculum_res = res;

    var CLAVES_C = {};
    (function () {
        var index = ("if else switch case default for while do break "
            + "continue goto return struct union enum typedef sizeof "
            + "static extern const volatile auto register signed "
            + "unsigned char short int long float double void").split(" ");
        var i;
        for (i = 0; i < index.length; i++) { CLAVES_C[index[i]] = true; }
    })();

    /* ---------------- adiutores ---------------- */

    function elementum(tag, classis, textus) {
        var e = document.createElement(tag);
        if (classis) { e.className = classis; }
        if (textus !== undefined) { e.textContent = textus; }
        return e;
    }

    function html_effugere(t) {
        return t.replace(/&/g, "&amp;").replace(/</g, "&lt;")
                .replace(/>/g, "&gt;");
    }

    function stylus_inserere() {
        var st;
        if (document.getElementById("speculum-stylus")) { return; }
        st = document.createElement("style");
        st.id = "speculum-stylus";
        st.textContent = window.speculum_stylus_fons || "";
        document.head.appendChild(st);
    }

    function claves_latinas_colligere() {
        var copia = {};
        var plag = res.per_viam["include/latina.h"];
        var re, m;
        if (plag) {
            re = /#define\s+([A-Za-z_][A-Za-z0-9_]*)/g;
            while ((m = re.exec(plag.corpus)) !== null) {
                copia[m[1]] = true;
            }
        }
        return copia;
    }

    /* ---------------- illuminatio ----------------
     * per lineam, status commentarii trans lineas portatus.
     * genus signi: com (commentarium), cho (chorda), cla (clavis C),
     * lat (macro latina), pra (praeprocessor). */

    function lineam_illuminare(linea, status) {
        var exitus = "";
        var i = 0;
        var n = linea.length;
        var initium_visum = false;

        function purum(t) { return html_effugere(t); }
        function signatum(t, genus) {
            return "<span class=\"speculum-il-" + genus + "\">"
                + html_effugere(t) + "</span>";
        }

        while (i < n) {
            var c = linea.charAt(i);
            if (status.in_commentario) {
                var finis = linea.indexOf("*/", i);
                if (finis === -1) {
                    exitus += signatum(linea.slice(i), "com");
                    i = n;
                } else {
                    exitus += signatum(linea.slice(i, finis + 2), "com");
                    i = finis + 2;
                    status.in_commentario = false;
                }
                continue;
            }
            if (c === "/" && linea.charAt(i + 1) === "*") {
                status.in_commentario = true;
                continue;
            }
            if (c === "\"" || c === "'") {
                var claudens = c;
                var j = i + 1;
                while (j < n) {
                    if (linea.charAt(j) === "\\") { j += 2; continue; }
                    if (linea.charAt(j) === claudens) { j++; break; }
                    j++;
                }
                exitus += signatum(linea.slice(i, j), "cho");
                i = j;
                initium_visum = true;
                continue;
            }
            if (c === "#" && !initium_visum) {
                var j2 = i + 1;
                while (j2 < n && /[a-z]/.test(linea.charAt(j2))) { j2++; }
                exitus += signatum(linea.slice(i, j2), "pra");
                i = j2;
                initium_visum = true;
                continue;
            }
            if (/[A-Za-z_]/.test(c)) {
                var j3 = i;
                while (j3 < n && /[A-Za-z0-9_]/.test(linea.charAt(j3))) { j3++; }
                var verbum = linea.slice(i, j3);
                if (CLAVES_C[verbum]) {
                    exitus += signatum(verbum, "cla");
                } else if (res.claves_latinae && res.claves_latinae[verbum]) {
                    exitus += signatum(verbum, "lat");
                } else {
                    exitus += purum(verbum);
                }
                i = j3;
                initium_visum = true;
                continue;
            }
            if (!/\s/.test(c)) { initium_visum = true; }
            exitus += purum(c);
            i++;
        }
        return exitus;
    }

    function illuminandum_est(via) {
        return /\.(c|h|m)$/.test(via);
    }

    /* ---------------- visor ---------------- */

    function plagulam_aperire(via, linea_saliendi) {
        var plag = res.per_viam[via];
        var caput, codex, lineae, status, i, corpus_div;
        if (!plag) { return; }
        res.via_aperta = via;
        res.visor.textContent = "";

        caput = elementum("div", "speculum-visor-caput");
        caput.appendChild(elementum("span", "speculum-visor-via", plag.via));
        caput.appendChild(elementum("span", "speculum-visor-magnitudo",
            plag.magnitudo + "B"));
        var copiare = elementum("button", "speculum-copiare", "copiare");
        copiare.onclick = function () { in_forulum_copiare(plag.corpus); };
        caput.appendChild(copiare);
        res.visor.appendChild(caput);

        codex = elementum("div", "speculum-codex");
        lineae = plag.corpus.split("\n");
        status = { in_commentario: false };
        for (i = 0; i < lineae.length; i++) {
            var linea_div = elementum("div", "speculum-linea");
            var numerus = elementum("span", "speculum-numerus",
                String(i + 1));
            var textus = elementum("span", "speculum-textus");
            if (illuminandum_est(via)) {
                textus.innerHTML = lineam_illuminare(lineae[i], status)
                    || "\u200b";
            } else {
                textus.textContent = lineae[i] || "\u200b";
            }
            linea_div.appendChild(numerus);
            linea_div.appendChild(textus);
            codex.appendChild(linea_div);
        }
        res.visor.appendChild(codex);

        if (res.plagula_electa) {
            res.plagula_electa.classList.remove("speculum-plagula-electa");
            res.plagula_electa = null;
        }
        var buttona = res.arbor.querySelector(
            "[data-via=\"" + via.replace(/"/g, "\\\"") + "\"]");
        if (buttona) {
            buttona.classList.add("speculum-plagula-electa");
            res.plagula_electa = buttona;
        }

        if (linea_saliendi !== undefined) {
            var electa = codex.children[linea_saliendi - 1];
            if (electa) {
                electa.classList.add("speculum-linea-electa");
                res.linea_electa = electa;
                electa.scrollIntoView({ block: "center" });
            }
        } else {
            res.visor.scrollTop = 0;
        }
    }

    function in_forulum_copiare(textus) {
        try {
            if (navigator.clipboard && navigator.clipboard.writeText) {
                navigator.clipboard.writeText(textus);
                return;
            }
        } catch (culpa) { /* infra reccidimus */ }
        var area = document.createElement("textarea");
        area.value = textus;
        document.body.appendChild(area);
        area.select();
        try { document.execCommand("copy"); } catch (culpa2) {}
        document.body.removeChild(area);
    }

    /* ---------------- arbor ---------------- */

    function filtrum_valor() {
        return res.filtrum_campus
            ? res.filtrum_campus.value.toLowerCase() : "";
    }

    function grex_viae(plag) {
        var pos;
        if (plag.genus === "documentum") { return "documenta"; }
        pos = plag.via.indexOf("/");
        if (pos === -1) { return "radix"; }
        return plag.via.slice(0, pos);
    }

    function arborem_struere(filtrum) {
        var greges = {};
        var nomina = [];
        var i, g;
        res.arbor_index.textContent = "";
        for (i = 0; i < res.plagulae.length; i++) {
            var plag = res.plagulae[i];
            if (filtrum
                && plag.via.toLowerCase().indexOf(filtrum) === -1) {
                continue;
            }
            g = grex_viae(plag);
            if (!greges[g]) { greges[g] = []; nomina.push(g); }
            greges[g].push(plag);
        }
        nomina.sort();
        for (i = 0; i < nomina.length; i++) {
            var caput = elementum("div", "speculum-grex", nomina[i]);
            res.arbor_index.appendChild(caput);
            var index = greges[nomina[i]];
            var j;
            for (j = 0; j < index.length; j++) {
                (function (plag2) {
                    var b = elementum("button", "speculum-plagula",
                        plag2.via);
                    b.setAttribute("data-via", plag2.via);
                    b.onclick = function () {
                        plagulam_aperire(plag2.via);
                    };
                    res.arbor_index.appendChild(b);
                })(index[j]);
            }
        }
    }

    /* ---------------- quaestio plena ---------------- */

    function lineae_initia_capere(via) {
        var initia = res.lineae_initia[via];
        var corpus, i;
        if (initia) { return initia; }
        initia = [0];
        corpus = res.per_viam[via].corpus;
        for (i = 0; i < corpus.length; i++) {
            if (corpus.charAt(i) === "\n") { initia.push(i + 1); }
        }
        res.lineae_initia[via] = initia;
        return initia;
    }

    function lineam_ex_offset(via, offset) {
        var initia = lineae_initia_capere(via);
        var imus = 0;
        var summus = initia.length - 1;
        while (imus < summus) {
            var medius = (imus + summus + 1) >> 1;
            if (initia[medius] <= offset) { imus = medius; }
            else { summus = medius - 1; }
        }
        return imus + 1;
    }

    function minusculum_capere(via) {
        var m = res.minuscula[via];
        if (m === undefined) {
            m = res.per_viam[via].corpus.toLowerCase();
            res.minuscula[via] = m;
        }
        return m;
    }

    function quaestionem_exsequi(textus) {
        var acus = textus.toLowerCase();
        var congruentiae = [];
        var LIMES = 500;
        var i;
        if (acus.length < 2) { arborem_struere(filtrum_valor()); return; }
        for (i = 0; i < res.plagulae.length
            && congruentiae.length < LIMES; i++) {
            var plag = res.plagulae[i];
            var corpus_min = minusculum_capere(plag.via);
            var pos = corpus_min.indexOf(acus);
            while (pos !== -1 && congruentiae.length < LIMES) {
                congruentiae.push({ via: plag.via, offset: pos });
                pos = corpus_min.indexOf(acus, pos + acus.length);
            }
        }
        res.arbor_index.textContent = "";
        res.arbor_index.appendChild(elementum("div", "speculum-grex",
            "congruentiae: " + congruentiae.length
            + (congruentiae.length >= LIMES ? "+" : "")));
        for (i = 0; i < congruentiae.length; i++) {
            (function (congr) {
                var linea = lineam_ex_offset(congr.via, congr.offset);
                var initia = lineae_initia_capere(congr.via);
                var corpus = res.per_viam[congr.via].corpus;
                var initium = initia[linea - 1];
                var finis = corpus.indexOf("\n", initium);
                if (finis === -1) { finis = corpus.length; }
                var contextus = corpus.slice(initium, finis).replace(/^\s+/, "");
                if (contextus.length > 60) {
                    contextus = contextus.slice(0, 60) + "…";
                }
                var b = elementum("button", "speculum-congruentia");
                var titulus = document.createElement("b");
                titulus.textContent = congr.via + ":" + linea;
                var textus_span = document.createElement("span");
                textus_span.textContent = "  " + contextus;
                b.appendChild(titulus);
                b.appendChild(textus_span);
                b.onclick = function () {
                    plagulam_aperire(congr.via, linea);
                };
                res.arbor_index.appendChild(b);
            })(congruentiae[i]);
        }
    }

    /* ---------------- tabula DE ---------------- */

    function de_struere() {
        var effusio = res.effusio;
        var clavis, i;
        var summa_fontium = 0;
        res.de_tabella.textContent = "";

        for (i = 0; i < effusio.fontes.length; i++) {
            summa_fontium += effusio.fontes[i].magnitudo;
        }
        res.de_tabella.appendChild(elementum("h2", null, "provenientia"));
        var tabula = elementum("table", "speculum-de-tabula");
        for (clavis in effusio.proventus) {
            if (!Object.prototype.hasOwnProperty.call(
                effusio.proventus, clavis)) { continue; }
            var ordo = document.createElement("tr");
            ordo.appendChild(elementum("th", null, clavis));
            ordo.appendChild(elementum("td", null,
                effusio.proventus[clavis]));
            tabula.appendChild(ordo);
        }
        res.de_tabella.appendChild(tabula);

        res.de_tabella.appendChild(elementum("h2", null, "summa"));
        res.de_tabella.appendChild(elementum("div", "speculum-de-summa",
            effusio.fontes.length + " fontes (" + summa_fontium
            + "B) + " + effusio.documenta.length + " documenta + "
            + effusio.exclusa.length + " exclusa"));

        res.de_tabella.appendChild(elementum("h2", null, "exclusa"));
        var tabula2 = elementum("table", "speculum-de-tabula");
        var caput_ordo = document.createElement("tr");
        var columnae = ["via", "magnitudo", "digestum", "causa"];
        for (i = 0; i < columnae.length; i++) {
            caput_ordo.appendChild(elementum("th", null, columnae[i]));
        }
        tabula2.appendChild(caput_ordo);
        for (i = 0; i < effusio.exclusa.length; i++) {
            var exc = effusio.exclusa[i];
            var ordo2 = document.createElement("tr");
            var j;
            for (j = 0; j < columnae.length; j++) {
                ordo2.appendChild(elementum("td", null,
                    exc[columnae[j]] || ""));
            }
            tabula2.appendChild(ordo2);
        }
        res.de_tabella.appendChild(tabula2);
    }

    /* ---------------- structura velaminis ---------------- */

    function tabulam_activare(quae) {
        var fons_est = (quae === "fons");
        res.corpus_fons.style.display = fons_est ? "flex" : "none";
        res.de_tabella.style.display = fons_est ? "none" : "block";
        res.tab_fons.className = "speculum-tabula"
            + (fons_est ? " speculum-tabula-activa" : "");
        res.tab_de.className = "speculum-tabula"
            + (fons_est ? "" : " speculum-tabula-activa");
    }

    function velamen_struere() {
        var velamen = elementum("div", "speculum-velamen");
        var caput = elementum("div", "speculum-caput");

        caput.appendChild(elementum("span", "speculum-titulus",
            "speculum"));
        res.tab_fons = elementum("button",
            "speculum-tabula speculum-tabula-activa", "FONS");
        res.tab_fons.onclick = function () { tabulam_activare("fons"); };
        res.tab_de = elementum("button", "speculum-tabula", "DE");
        res.tab_de.onclick = function () { tabulam_activare("de"); };
        caput.appendChild(res.tab_fons);
        caput.appendChild(res.tab_de);

        res.quaestio_campus = document.createElement("input");
        res.quaestio_campus.className = "speculum-quaestio";
        res.quaestio_campus.placeholder = "quaerere in fontibus…";
        res.quaestio_campus.oninput = function () {
            var valor = res.quaestio_campus.value;
            if (res.mora_quaestionis) {
                clearTimeout(res.mora_quaestionis);
            }
            res.mora_quaestionis = setTimeout(function () {
                quaestionem_exsequi(valor);
            }, 250);
        };
        caput.appendChild(res.quaestio_campus);

        var claudere = elementum("button", "speculum-claudere",
            "×");
        claudere.onclick = function () { commutare(); };
        caput.appendChild(claudere);
        velamen.appendChild(caput);

        res.corpus_fons = elementum("div", "speculum-corpus");
        res.arbor = document.createElement("fons-arbor");
        res.filtrum_campus = document.createElement("input");
        res.filtrum_campus.className = "speculum-filtrum";
        res.filtrum_campus.placeholder = "filtrare arborem…";
        res.filtrum_campus.oninput = function () {
            if (res.quaestio_campus.value.length < 2) {
                arborem_struere(filtrum_valor());
            }
        };
        res.arbor.appendChild(res.filtrum_campus);
        res.arbor_index = elementum("div", "speculum-arbor-index");
        res.arbor.appendChild(res.arbor_index);
        res.visor = document.createElement("fons-visor");
        res.corpus_fons.appendChild(res.arbor);
        res.corpus_fons.appendChild(res.visor);
        velamen.appendChild(res.corpus_fons);

        res.de_tabella = elementum("div", "speculum-de");
        res.de_tabella.style.display = "none";
        velamen.appendChild(res.de_tabella);

        document.body.appendChild(velamen);
        res.velamen = velamen;

        arborem_struere("");
        de_struere();
        res.visor.appendChild(elementum("div", "speculum-vacuum",
            "elige plagulam sinistra - aut quaere supra"));
    }

    function effusionem_digerere(effusio) {
        var i;
        res.effusio = effusio;
        res.plagulae = [];
        res.per_viam = {};
        for (i = 0; i < effusio.fontes.length; i++) {
            var f = effusio.fontes[i];
            f.genus = "fons";
            res.plagulae.push(f);
            res.per_viam[f.via] = f;
        }
        for (i = 0; i < effusio.documenta.length; i++) {
            var d = effusio.documenta[i];
            d.genus = "documentum";
            res.plagulae.push(d);
            res.per_viam[d.via] = d;
        }
        res.claves_latinae = claves_latinas_colligere();
    }

    function effusionem_colligere() {
        if (res.colligens) { return; }
        res.colligens = true;
        internuntius.vocare("speculum_obtinere", {}).then(
            function (effusio) {
                effusionem_digerere(effusio);
                velamen_struere();
                res.structum = true;
                res.apertum = true;
                res.colligens = false;
            }
        ).catch(function (culpa) {
            res.colligens = false;
            console.log("[speculum] effusio fracta: " + culpa.message);
        });
    }

    function commutare() {
        if (!res.structum) { effusionem_colligere(); return; }
        res.apertum = !res.apertum;
        res.velamen.style.display = res.apertum ? "flex" : "none";
    }

    window.speculum_commutare = commutare;

    document.addEventListener("keydown", function (ev) {
        if (!res.apertum) { return; }
        if (ev.key !== "Escape") { return; }
        if (document.activeElement === res.quaestio_campus
            && res.quaestio_campus.value) {
            res.quaestio_campus.value = "";
            arborem_struere(filtrum_valor());
            ev.stopPropagation();
            return;
        }
        commutare();
    });

    stylus_inserere();
    commutare();
})();

console.log("[speculum] stipes iniectus - bona ex capsula");

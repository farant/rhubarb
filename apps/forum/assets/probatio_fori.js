/* probatio_fori.js - probationes velaminis fori sine navigatro
 * (JXA/JSC). Exemplar: lib/speculum_assets/probatio_velaminis.js.
 *
 * DOM fictum minimum; SCRIPTUM VERUM ex index.html extractum
 * evaluatur; deinde logica pura exercetur.
 *
 * CUR: velamen fori logicam veram fert quae in C non stat -
 * derivatio tituli personae, aequatio ADDITIVA definitionum
 * viventium, cribrum indicis librorum. Aequatio praesertim tacite
 * fallit: si optiones electionis non augentur, species nova a
 * machina recusatur ('valor extra optiones electionis') et causa
 * in velamine non apparet.
 *
 * Cursus: osascript -l JavaScript apps/forum/assets/probatio_fori.js
 */
ObjC.import("Foundation");

/* ---- DOM fictum ---- */
function nodum(tag) {
    var e = {
        tag: tag || "", children: [], style: {},
        dataset: {},
        classList: { add: function () {}, remove: function () {} },
        appendChild: function (f) { e.children.push(f); return f; },
        removeChild: function (f) {
            var p = e.children.indexOf(f);
            if (p !== -1) { e.children.splice(p, 1); }
        },
        remove: function () {},
        setAttribute: function () {},
        addEventListener: function () {},
        querySelector: function () { return null; },
        scrollIntoView: function () {},
        select: function () {}, focus: function () {},
        scrollTop: 0, onclick: null, oninput: null, onkeydown: null,
        onchange: null, value: "", className: "", id: "",
        placeholder: "", hidden: false, disabled: false, title: "",
        rows: 0, checked: false
    };
    var t = "", h = "";
    /* textContent CONCATENAT filios, ut in DOM vero. Sine hoc nodus
     * ex spans compositus '' reddit, et adsertio 'auctores in
     * corpore' rubet dum codex recte pingit - vitium harnesii a
     * vitio operis indiscernibile (secunda vice hodie). Scriptio
     * filios purgat, ergo duo numquam simul stant. */
    Object.defineProperty(e, "textContent", {
        get: function () {
            var s = t;
            e.children.forEach(function (f) { s += f.textContent; });
            return s;
        },
        set: function (v) { t = v; e.children = []; }
    });
    Object.defineProperty(e, "innerHTML", {
        get: function () { return h; },
        set: function (v) { h = v; e.children = []; }
    });
    return e;
}

var nodi = {};
var document = {
    createElement: function (tag) { return nodum(tag); },
    createTextNode: function (t) {
        var n = nodum("#text");
        n.textContent = t;
        return n;
    },
    /* nodum MEMORATUM reddere: scriptum idem elementum bis petit
       (semel ut manubrium servet, semel ut scribat) */
    getElementById: function (id) {
        if (!nodi[id]) { nodi[id] = nodum("div"); nodi[id].id = id; }
        return nodi[id];
    },
    querySelector: function () { return nodum("div"); },
    head: nodum("head"), body: nodum("body"),
    documentElement: nodum("html"),
    activeElement: null,
    addEventListener: function () {},
    execCommand: function () { return true; }
};
var window = { getComputedStyle: function () { return {}; } };
var navigator = {};
var localStorage = {
    getItem: function () { return null; }, setItem: function () {}
};
function setTimeout() { return 1; }      /* NIHIL agit: probationes
                                            synchronae sunt */
function clearTimeout() {}
function setInterval() { return 1; }
function clearInterval() {}

/* PROMISSUM SYNCHRONUM - decipula JXA mensurata (vide MEMORY.md):
 * microtasks in osascript FORTASSE NUMQUAM effunduntur, ergo
 * chorda .then() vera tacite dimidiata manet et probatio VIRIDIS
 * apparet quia adsertiones numquam currunt. Ideo Promise proprium:
 * omnia SYNCHRONE solvuntur, et post ultimam lineam nihil pendet.
 *
 * Codex applicationis mutationem non sentit - eandem interfaciem
 * (then/catch/resolve/all) videt. */
function Sync(v, err, reiecta) {
    this.v = v; this.err = err; this.reiecta = !!reiecta;
}
Sync.prototype.then = function (f, g) {
    if (this.reiecta) {
        if (!g) { return this; }
        try { return Sync.resolve(g(this.err)); }
        catch (e) { return new Sync(undefined, e, true); }
    }
    if (!f) { return this; }
    try { return Sync.resolve(f(this.v)); }
    catch (e) { return new Sync(undefined, e, true); }
};
Sync.prototype["catch"] = function (g) { return this.then(null, g); };
Sync.resolve = function (v) {
    return (v && typeof v.then === "function") ? v : new Sync(v);
};
Sync.reject = function (e) { return new Sync(undefined, e, true); };
Sync.all = function (arr) {
    var fructus = [];
    var culpa = null;
    (arr || []).forEach(function (p) {
        Sync.resolve(p).then(function (v) { fructus.push(v); },
            function (e) { culpa = e; });
    });
    return culpa ? Sync.reject(culpa) : new Sync(fructus);
};
var Promise = Sync;

/* ---- pons fictus: vocationes memorat ---- */
var vocationes = [];
var responsa = {};          /* methodus|instrumentum -> fructus */
var internuntius = {
    vocare: function (methodus, argumenta) {
        vocationes.push({ methodus: methodus, argumenta: argumenta });
        var clavis = methodus;
        if (methodus === "transmittere" && argumenta) {
            clavis = "tool:" + argumenta.instrumentum;
        }
        if (methodus === "res_legere" && argumenta) {
            clavis = "legere:" + argumenta.genus;
        }
        var r = responsa[clavis];
        return Promise.resolve(
            typeof r === "function" ? r(argumenta) : (r || {}));
    },
    audire: function () {}
};

/* ---- adiutores ---- */
var fracta = [];
var probata = 0;
function proba(conditio, titulus) {
    probata++;
    if (!conditio) { fracta.push(titulus); }
}
function aequale(actum, exspectatum, titulus) {
    probata++;
    if (actum !== exspectatum) {
        fracta.push(titulus + " (actum: " + JSON.stringify(actum)
            + ", exspectatum: " + JSON.stringify(exspectatum) + ")");
    }
}
/* Argumenta INTERIORA reddit: 'instrumentum(n, a)' involucrum
 * 'transmittere {instrumentum, argumenta}' mittit, ergo qui
 * v.argumenta.actus legit SEMPER undefined videt - et probatio
 * tunc 'nihil mutatum' nuntiat dum omnia recte mutata sunt. */
function mutationes(instrumentum_nomen) {
    return vocationes.filter(function (v) {
        return v.methodus === "transmittere"
            && v.argumenta
            && v.argumenta.instrumentum === instrumentum_nomen;
    }).map(function (v) { return v.argumenta.argumenta || {}; });
}

/* ---- scriptum verum ex index.html ---- */
function legere_plagulam(via) {
    return $.NSString.stringWithContentsOfFileEncodingError(
        via, $.NSUTF8StringEncoding, null).js;
}

var via_index = "apps/forum/assets/index.html";
var pagina = legere_plagulam(via_index);
if (!pagina) {
    throw new Error("FRACTA: index.html non lectum (" + via_index + ")");
}
var i_a = pagina.indexOf("<script>");
var i_b = pagina.lastIndexOf("</script>");
if (i_a < 0 || i_b < 0) {
    throw new Error("FRACTA: <script> in index.html non inventum");
}
var scriptum = pagina.slice(i_a + 8, i_b);

/* In ambitu globali evaluare ut functiones visibiles sint */
try {
    (0, eval)(scriptum);
} catch (e) {
    throw new Error("FRACTA: scriptum non evaluatum: " + e.message);
}

/* ================================================================
 * I. DERIVATIO TITULI - 'name' ex praenomine et cognomine
 * ================================================================ */
var sp_persona = species_de("persona");
aequale(titulus_entis(sp_persona,
    { first_name: "Erasmus", last_name: "Darwin" }),
    "Erasmus Darwin", "titulus personae ex duobus");
aequale(titulus_entis(sp_persona, { last_name: "Darwin" }),
    "Darwin", "titulus personae sine praenomine");
aequale(titulus_entis(sp_persona, { first_name: "Erasmus" }),
    "Erasmus", "titulus personae sine cognomine");
aequale(titulus_entis(species_de("locus"),
    { titulus_loci: "Birmingham" }), "Birmingham",
    "titulus loci ex campo tituli");
aequale(titulus_entis(species_de("scriptum"),
    { title: "Zoonomia" }), "Zoonomia", "titulus scripti");

/* ================================================================
 * II. SUMMARIUM - numquam vacuum ('addere' titulum poscit)
 * ================================================================ */
proba(summarium_adnotationis(species_de("persona"), "", "") !== "",
    "summarium sine textu et sine subiecto non vacuum");
aequale(summarium_adnotationis(species_de("persona"), "",
    "Erasmus Darwin"), "Erasmus Darwin",
    "summarium subiectum praefert");
aequale(summarium_adnotationis(species_de("nota"), "  nota   mea  ",
    ""), "nota mea", "summarium spatia cogit");
proba(summarium_adnotationis(species_de("nota"),
    new Array(200).join("x"), "").length <= 71,
    "summarium longum praeciditur");

/* ================================================================
 * III. SPECIES - tabula, non codex
 * ================================================================ */
aequale(species_de("ignota").clavis, "nota",
    "species ignota ad notam recidit");
["nota", "citatio", "persona", "eventus", "locus", "terminus",
 "societas", "scriptum", "inventum", "quaestio"].forEach(
    function (k) {
        aequale(species_de(k).clavis, k, "species " + k + " adest");
    });
aequale(species_de("scriptum").ens, "book",
    "scriptum genus 'book' adhibet");
proba(species_de("scriptum").valores_fixa.owned === false,
    "scriptum owned=false scribit");
proba(species_de("quaestio").ens === null,
    "quaestio ens mundanum non habet");
proba(species_de("quaestio").campi_adnotationis.length === 1,
    "quaestio responsum in adnotatione fert");
/* genera quae species poscit (ens + destinationes relationum) */
var g_inv = genera_speciei(species_de("inventum"));
proba(g_inv.indexOf("inventum") >= 0 && g_inv.indexOf("person") >= 0,
    "inventum et personam poscit (inventores)");

/* ================================================================
 * IV. CRIBRUM INDICIS LIBRORUM
 * ================================================================ */
proba(liber_in_indice({ datum: { owned: true } }),
    "possessum in indice");
proba(liber_in_indice({ datum: { reading_status: "reading" } }),
    "status lectionis positus in indice");
proba(!liber_in_indice({ datum: { owned: false } }),
    "opus memoratum (owned false) celatur");
proba(!liber_in_indice({ datum: { owned: false,
    reading_status: "" } }), "status vacuus non sufficit");
proba(!liber_in_indice({}), "datum absens celatur");

/* ================================================================
 * III bis. CORPUS ET LATUS SUBIECTI - quid ex ENTE in chartam venit
 * ================================================================ */
aequale(subiectum_corpus(species_de("terminus"),
    { datum: { definitio_termini: "principium ignis putatum" } }),
    "principium ignis putatum", "definitio termini in corpore");
aequale(subiectum_corpus(species_de("persona"),
    { datum: { description: "medicus" } }), "",
    "species sine campo corporis nihil reddit");
aequale(subiectum_corpus(species_de("terminus"), null), "",
    "ens absens (copia nondum onerata) nihil reddit, non frangit");
aequale(subiectum_latus(species_de("eventus"),
    { datum: { annus: -753 } }), "753 a.C.n.",
    "annus eventus negativus ut a.C.n.");
aequale(subiectum_latus(species_de("locus"),
    { datum: { coordinatae: "52.4862, -1.8904" } }),
    "52.4862, -1.8904", "coordinatae loci in latere");
aequale(subiectum_latus(species_de("nota"), { datum: {} }), "",
    "species sine latere nihil reddit");

/* ================================================================
 * III quater. PAGINATIO - GEMINUS lib/paginatio.c
 *
 * C oraculum est. Casus hi IIDEM sunt qui in probatio_paginatio.c
 * stant: geminus sine casibus communibus tacite divergit, et
 * divergentia hic = pagina in ordine falso, quod nemo videt donec
 * index notarum absurdus fiat.
 * ================================================================ */
/* numeri Romani: eadem forma stricta */
aequale(numerus_romanus_legere("xii"), 12, "romana minuscula");
aequale(numerus_romanus_legere("XIV"), 14, "romana maiuscula");
aequale(numerus_romanus_legere("xlii"), 42, "xlii = XLII");
aequale(numerus_romanus_legere("MCMXCIV"), 1994, "MCMXCIV");
aequale(numerus_romanus_legere("XXXIX"), 39, "XXXIX");
aequale(numerus_romanus_legere("iiii"), null, "cursus nimius");
aequale(numerus_romanus_legere("vv"), null, "v bis");
aequale(numerus_romanus_legere("il"), null, "par illicitum");
aequale(numerus_romanus_legere("ixx"), null, "post ix nihil");
aequale(numerus_romanus_legere("Xii"), null, "casus MIXTUS respuitur");
aequale(numerus_romanus_legere("did"), null, "verbum Anglicum 'did'");
aequale(numerus_romanus_legere("mild"), null, "verbum 'mild'");
aequale(numerus_romanus_legere("civic"), null, "verbum 'civic'");
aequale(numerus_romanus_legere("mix"), 1009, "MIX numerus verus est");
aequale(numerus_romanus_legere(""), null, "vacua");

/* designationes */
aequale(paginatio_legere("xii").genus, "romana", "xii romana");
aequale(paginatio_legere("42").genus, "arabica", "42 arabica");
aequale(paginatio_legere(" 42 ").genus, "arabica", "spatia praecisa");
aequale(paginatio_legere(42).genus, "arabica",
    "INTEGER acceptus (paginae iam scriptae)");
aequale(paginatio_legere("0").genus, null, "pagina nulla non est");
aequale(paginatio_legere("12a").genus, null, "'12a' respuitur");
aequale(paginatio_legere("p. 12").genus, null, "'p. 12' respuitur");
aequale(paginatio_legere("-5").genus, null, "negativa respuitur");
aequale(paginatio_legere("").genus, null, "vacua");
proba(paginatio_valet("xii") && paginatio_valet("42")
    && !paginatio_valet("nugae"), "paginatio_valet congruit");

/* ORDO - causa ipsa typi */
proba(paginatio_clavis("i") < paginatio_clavis("ii"),
    "intra Romanas numerice");
proba(paginatio_clavis("ix") < paginatio_clavis("x"), "ix < x");
proba(paginatio_clavis("9") < paginatio_clavis("10"),
    "intra Arabicas NUMERICE, non lexice");
proba(paginatio_clavis("99") < paginatio_clavis("100"), "99 < 100");
proba(paginatio_clavis("xlii") < paginatio_clavis("1"),
    "PROOEMIUM ANTE CORPUS (xlii < 1)");
proba(paginatio_clavis("m") < paginatio_clavis("1"),
    "omnis Romana ante omnem Arabicam");
proba(paginatio_clavis("") > paginatio_clavis("380"),
    "pagina nulla ULTIMA");
aequale(paginatio_clavis("XII"), paginatio_clavis("xii"),
    "casus ordinem non mutat");
aequale(paginatio_clavis(42), paginatio_clavis("42"),
    "integer et chorda eandem clavem dant");

/* subiecti_campi: quid ex ente in corpus chartae venit.
   Probatur per speciem SYNTHETICAM quia species verae hodie
   campum veritatis non ferunt - adsertio contra scriptum
   VACUA esset (mensuratum: calibratio non rubuit). */
(function () {
    var sp_ficta = { campus_tituli: 'nomen',
        campi: [
            { clavis: 'nomen', monstrans: 'Nomen', typus: 'textus' },
            { clavis: 'annus', monstrans: 'Annus', typus: 'annus' },
            { clavis: 'possessum', monstrans: 'Possessum',
                typus: 'veritas' },
            { clavis: 'nota', monstrans: 'Nota', typus: 'textus' }
        ] };
    function claves(ordines) {
        return ordines.map(function (o) { return o.monstrans; });
    }
    var plena = subiecti_campi(sp_ficta, { datum: {
        nomen: "X", annus: -753, possessum: true, nota: "n" } });
    aequale(claves(plena).join(","), "Annus,Possessum,Nota",
        "campus tituli OMITTITUR, ceteri manent");
    aequale(plena[0].valor, "753 a.C.n.", "annus per aeram redditur");
    aequale(plena[1].valor, "ita", "veritas VERA ut 'ita'");

    var falsa = subiecti_campi(sp_ficta, { datum: {
        nomen: "X", possessum: false, nota: "" } });
    aequale(claves(falsa).join(","), "",
        "veritas FALSA et chorda vacua OMITTUNTUR");

    /* species cum campo corporis nudo campos titulatos NON reddit */
    aequale(subiecti_campi(species_de("terminus"),
        { datum: { vocabulum: "x", definitio_termini: "y" } }).length,
        0, "campus corporis nudus campos titulatos excludit");
    aequale(subiecti_campi(sp_ficta, null).length, 0,
        "ens absens nihil reddit, non frangit");
}());

/* ================================================================
 * III quinquies. COORDINATAE + ANNUS NOTAE + LOCUS NOTAE
 * ================================================================ */
aequale(JSON.stringify(coordinatas_legere("52.4862, -1.8904")),
    "[52.4862,-1.8904]", "coordinatae normales");
aequale(JSON.stringify(coordinatas_legere("  36.0606 , 102.8268 ")),
    "[36.0606,102.8268]", "spatia tolerantur");
aequale(JSON.stringify(coordinatas_legere("0,0")), "[0,0]",
    "zephyrum licet (insula nulla)");
aequale(coordinatas_legere("Birmingham"), null, "nomen respuitur");
aequale(coordinatas_legere("52.4862"), null, "una sola respuitur");
aequale(coordinatas_legere("52.4862, -1.8904, 7"), null,
    "tres respuuntur");
aequale(coordinatas_legere("91, 0"), null, "latitudo extra limites");
aequale(coordinatas_legere("0, 181"), null,
    "longitudo extra limites");
aequale(JSON.stringify(coordinatas_legere("-90, 180")),
    "[-90,180]", "limites ipsi LICENT (non extra)");
aequale(coordinatas_legere(""), null, "vacua");
aequale(coordinatas_legere(null), null, "nulla non frangit");

/* annus EX ENTE, per campum speciei nominatum */
(function () {
    adnot_scopi["eventus"] = [{ res_id: "01E", datum: {
        titulus: "condensator", descriptio: "condensator",
        annus: 1765 } }];
    adnot_scopi["person"] = [{ res_id: "01P", datum: {
        titulus: "Erasmus Darwin", year_of_birth: 1731 } }];
    var ev = { datum: { species: "eventus" },
        nexus: [{ verbum: "eventus", ad: "01E" }] };
    var pe = { datum: { species: "persona" },
        nexus: [{ verbum: "persona", ad: "01P" }] };
    var no = { datum: { species: "nota", textus: "x" }, nexus: [] };
    aequale(annus_notae(ev), 1765, "annus eventus ex ente");
    aequale(annus_notae(pe), 1731, "annus personae = annus natalis");
    aequale(annus_notae(no), null,
        "nota simplex annum non habet (cribrum ordinis)");
    aequale(annus_notae({ datum: { species: "eventus" },
        nexus: [] }), null, "eventus sine subiecto: nullus annus");

    /* locus: per speciem loci IPSAM et per eventum MONSTRANTEM */
    adnot_scopi["locus"] = [{ res_id: "01L", datum: {
        titulus: "Birmingham", titulus_loci: "Birmingham",
        coordinatae: "52.4862, -1.8904", zoom: 5 } }];
    var lo = { datum: { species: "locus" },
        nexus: [{ verbum: "locus", ad: "01L" }] };
    aequale(locus_notae(lo).titulus, "Birmingham",
        "species loci mappam suam fert");
    aequale(locus_notae(lo).zoom, 5, "zoom auctoris servatur");
    aequale(locus_notae(no), null, "nota simplex mappam non habet");

    /* eventus -> locus (decisio Franis: pagina eventuum id probat) */
    adnot_scopi["eventus"] = [{ res_id: "01E2", datum: {
        titulus: "conventus", descriptio: "conventus", annus: 1765 },
        nexus: [{ verbum: "locus", ad: "01L" }] }];
    var ev2 = { datum: { species: "eventus" },
        nexus: [{ verbum: "eventus", ad: "01E2" }] };
    aequale(locus_notae(ev2).titulus, "Birmingham",
        "eventus mappam per locum suum fert");
    aequale(locus_notae(ev2).coordinatae, "52.4862, -1.8904",
        "coordinatae ex loco ligato");

    /* locus sine coordinatis: nulla mappa, non fractura */
    adnot_scopi["locus"] = [{ res_id: "01L", datum: {
        titulus: "Alexandria", titulus_loci: "Alexandria" } }];
    aequale(locus_notae(lo), null,
        "locus sine coordinatis mappam non petit");
}());

/* mappam_facere: coordinatae pravae mappam NULLAM dant (mappa
   loci ALIENI peior est quam nulla - lector mappam credit) */
aequale(mappam_facere("nugae", 4, "X"), null,
    "coordinatae pravae mappam non pariunt");
proba(mappam_facere("52.4862, -1.8904", 5, "Birmingham") !== null,
    "coordinatae bonae mappam pariunt");

/* ================================================================
 * III ter. CHARTA IPSA - quid usor revera videt
 * ================================================================ */
(function () {
    function classes(el) {
        return el.children.map(function (c) { return c.className; });
    }
    function textus_classis(el, cl) {
        var t = "";
        el.children.forEach(function (c) {
            if (c.className === cl) { t = c.textContent; }
        });
        return t;
    }

    adnot_scopi["terminus"] = [{ res_id: "01T", datum: {
        titulus: "phlogiston", vocabulum: "phlogiston",
        definitio_termini: "principium ignis putatum" } }];
    var charta = elementum_adnotationis({
        res_id: "01A", titulus: "nota",
        datum: { species: "terminus", textus: "Priestley eo utitur",
            pagina: 92 },
        nexus: [{ verbum: "terminus", ad: "01T",
            ad_titulus: "phlogiston" }] });
    var cl = classes(charta);
    proba(cl.indexOf("definitio-a") >= 0,
        "charta termini definitionem fert");
    aequale(textus_classis(charta, "definitio-a"),
        "principium ignis putatum", "definitio ex ENTE reddita");
    aequale(textus_classis(charta, "textus-a"), "Priestley eo utitur",
        "nota propria seorsum manet");
    proba(cl.indexOf("definitio-a") < cl.indexOf("textus-a"),
        "definitio ANTE notam stat");

    /* copia nondum onerata: nomen manet (per nexum), definitio abest
       - informatio MINUITUR, charta non frangitur */
    adnot_scopi["terminus"] = null;
    var sine = elementum_adnotationis({
        res_id: "01A", titulus: "nota",
        datum: { species: "terminus", textus: "nota mea" },
        nexus: [{ verbum: "terminus", ad: "01T",
            ad_titulus: "phlogiston" }] });
    proba(classes(sine).indexOf("definitio-a") < 0,
        "sine copia definitio omittitur");
    aequale(textus_classis(sine, "textus-a"), "nota mea",
        "sine copia nota propria manet");

    /* SCRIPTUM: campi entis non-vacui in corpore, cum titulis
       (campus tituli omittitur - in capite iam stat; veritas FALSA
       omittitur - 'owned: non' de opere memorato strepitus est) */
    adnot_scopi["book"] = [{ res_id: "01B", datum: {
        titulus: "Experiments on Air", title: "Experiments on Air",
        year_published: 1774, owned: false },
        nexus: [{ verbum: "authors", ad: "01P",
            ad_titulus: "Joseph Priestley" }] }];
    var scr = elementum_adnotationis({ res_id: "01S", titulus: "s",
        datum: { species: "scriptum", textus: "hic oxygenium",
            pagina: "xii" },
        nexus: [{ verbum: "scriptum", ad: "01B",
            ad_titulus: "Experiments on Air" }] });
    var t_campi = textus_classis(scr, "campi-a");
    proba(t_campi.indexOf("Joseph Priestley") >= 0,
        "auctores scripti in corpore");
    proba(t_campi.indexOf("1774") >= 0, "annus editionis in corpore");
    proba(t_campi.indexOf("Experiments on Air") < 0,
        "campus TITULI omittitur (in capite stat)");
    proba(classes(scr).indexOf("definitio-a") < 0,
        "scriptum campum corporis nudum non habet");

    /* PERSONA: caput iam 'Erasmus Darwin (1731)' fert, ergo corpus
       'praenomen: ... cognomen: ... annus natalis: ...' eadem tria
       BIS diceret. Corpus VACUUM esse debet. */
    adnot_scopi["person"] = [{ res_id: "01P2", datum: {
        titulus: "Erasmus Darwin", name: "Erasmus Darwin",
        first_name: "Erasmus", last_name: "Darwin",
        year_of_birth: 1731 } }];
    var per = elementum_adnotationis({ res_id: "01PA", titulus: "p",
        datum: { species: "persona", textus: "medicus et poeta" },
        nexus: [{ verbum: "persona", ad: "01P2",
            ad_titulus: "Erasmus Darwin" }] });
    proba(classes(per).indexOf("campi-a") < 0,
        "charta personae corpus camporum NON fert");
    proba(per.textContent.indexOf("praenomen") < 0
        && per.textContent.indexOf("cognomen") < 0,
        "praenomen/cognomen non repetuntur (titulum componunt)");
    proba(per.textContent.indexOf("1731") >= 0,
        "annus natalis SEMEL adest (in latere capitis)");
    aequale(per.textContent.split("1731").length - 1, 1,
        "annus natalis SEMEL, non bis");
    aequale(textus_classis(per, "textus-a"), "medicus et poeta",
        "nota propria manet");

    /* citatio uncinis cingitur; nota simplex non */
    var cit = elementum_adnotationis({ res_id: "01C", titulus: "c",
        datum: { species: "citatio", textus: "Omnia ex conchis" },
        nexus: [] });
    aequale(textus_classis(cit, "textus-a"), "“Omnia ex conchis”",
        "citatio uncinis cingitur");

    /* quaestio sine responso se apertam nuntiat */
    var q = elementum_adnotationis({ res_id: "01Q", titulus: "q",
        datum: { species: "quaestio", textus: "cur?" }, nexus: [] });
    aequale(textus_classis(q, "pagina-a"), "— sine responso",
        "quaestio aperta se nuntiat");
    var qr = elementum_adnotationis({ res_id: "01Q", titulus: "q",
        datum: { species: "quaestio", textus: "cur?",
            responsum: "quia" }, nexus: [] });
    aequale(textus_classis(qr, "responsum-a"), "quia",
        "responsum redditum");
}());

/* ================================================================
 * III sexies. ORDO TEMPORUM - phrasis dicit quid annus SIGNIFICET
 * ================================================================ */
(function () {
    function phrasis(sp_clavis, titulus) {
        var sp = species_de(sp_clavis);
        return sp.phrasis_temporis
            ? sp.phrasis_temporis.replace('{}',
                function () { return titulus; })
            : titulus;
    }
    aequale(phrasis("persona", "Erasmus Darwin"),
        "ortus: Erasmus Darwin", "annus personae ORTUS eius est");
    aequale(phrasis("scriptum", "Zoonomia"), "«Zoonomia» editum",
        "annus scripti EDITIO eius est");
    aequale(phrasis("societas", "Lunar Society"),
        "condita: Lunar Society", "annus societatis CONDITIO");
    aequale(phrasis("eventus", "Watt patens accipit"),
        "Watt patens accipit",
        "eventus phrasin non poscit - descriptio eventus IPSA est");
    aequale(phrasis("inventum", "condensator separatus"),
        "condensator separatus", "inventum titulo suo stat");
    /* titulus cum '$&': substitutio per functionem, non per
       chordam - aliter replace id ut exemplar legeret */
    aequale(phrasis("persona", "A $& B"), "ortus: A $& B",
        "titulus cum '$&' intactus manet");

    /* charta ordinis: phrasis, SINE sigillo speciei */
    adnot_scopi["person"] = [{ res_id: "01P", datum: {
        titulus: "Erasmus Darwin", year_of_birth: 1731 } }];
    var el = elementum_temporis({ annus: 1731, nota: {
        res_id: "01A", datum: { species: "persona", textus: "" },
        nexus: [{ verbum: "persona", ad: "01P",
            ad_titulus: "Erasmus Darwin" }] } });
    var t = el.textContent;
    proba(t.indexOf("ortus: Erasmus Darwin") >= 0,
        "charta ordinis phrasin fert");
    proba(t.indexOf("persona") < 0,
        "sigillum speciei ABEST (more imaginis)");
    proba(t.indexOf("1731") >= 0 && t.indexOf("A.D.") >= 0,
        "annus et aera in margine");
    var neg = elementum_temporis({ annus: -753, nota: {
        res_id: "01B", datum: { species: "nota", textus: "urbs" },
        nexus: [] } });
    proba(neg.textContent.indexOf("753") >= 0
        && neg.textContent.indexOf("a.C.n.") >= 0,
        "annus negativus ut a.C.n., sine signo");
}());

/* ================================================================
 * IV bis. VESTIGIUM REDITUS - unde in ens venimus
 *
 * Vetustas structuraliter impossibilis esse debet: vestigium ad
 * res_id ligatum est, ergo ens ALIUD id non heredat. Sine hac
 * comparatione usor ens quodlibet apertum premens ad capitulum
 * alienum mitteretur - error qui ut 'app confusa est' apparet,
 * numquam ut error.
 * ================================================================ */
(function () {
    var itum = 0;
    res_ens_id = "01ENS_A";
    res_reditus = { res_id: "01ENS_A", titulus: "Riots",
        ire: function () { itum++; } };
    proba(reditus_valet(), "vestigium ad ens SUUM valet");
    reddere_reditum();
    aequale(document.getElementById("ens-retro").textContent,
        "← Riots", "titulus vestigii DESTINATIONEM nominat");

    /* ens aliud eadem sessione apertum: vestigium NON valet */
    res_ens_id = "01ENS_B";
    proba(!reditus_valet(), "vestigium ad ens alienum NON valet");
    reddere_reditum();
    aequale(document.getElementById("ens-retro").textContent,
        "← tabula", "sine vestigio valido titulus ordinarius");

    /* tabulam Rerum relinquens vestigium relinquit */
    res_ens_id = "01ENS_A";
    proba(reditus_valet(), "vestigium redit cum ens suum redit");
    vertere("pipata");
    proba(res_reditus === null,
        "vertere ex tabula Rerum vestigium purgat");
    vertere("res");
    proba(res_reditus === null,
        "vestigium purgatum manet post reditum in tabulam Rerum");
    aequale(itum, 0, "vestigium nondum adhibitum");
}());

/* ================================================================
 * V. AEQUATIO ADDITIVA - probatio quae plurimum valet
 *
 * Definitio VIVA optiones veteres fert; codex novas poscit. Sine
 * augmento species nova a machina recusatur et velamen causam non
 * monstrat.
 * ================================================================ */
function definitio(clavis, campi) {
    return { res_id: "01DEF" + clavis, titulus: clavis,
        datum: { clavis: clavis, titulus_monstrans: clavis,
            campi: campi } };
}

function curare_cum(defs) {
    vocationes = [];
    responsa["legere:definitio"] = { res: defs };
    responsa["legere:book"] = { res: [] };
    responsa["tool:addere"] = { res_id: "01NOVUM" };
    responsa["tool:gerere"] = {};
    return libri_genera_curare();
}

/* conditorium VETUS: adnotatio cum optionibus V et sine responso */
var defs_vetera = [
    definitio("person", [
        { clavis: "name", monstrans: "Name", typus: "textus" },
        { clavis: "year_of_birth", monstrans: "Year of birth",
            typus: "annus" }]),
    definitio("book", [
        { clavis: "title", monstrans: "Title", typus: "textus" }]),
    definitio("capitulum", []),
    definitio("locus", []), definitio("eventus", []),
    definitio("terminus", []), definitio("societas", []),
    definitio("inventum", []),
    definitio("adnotatio", [
        { clavis: "species", monstrans: "Species", typus: "electio",
            optiones: ["nota", "citatio", "persona", "eventus",
                "locus"] },
        { clavis: "textus", monstrans: "Textus", typus: "area" },
        { clavis: "campus_meus", monstrans: "Meus",
            typus: "textus" }])
];

curare_cum(defs_vetera).then(function () {
    var mut = mutationes("gerere").filter(function (v) {
        return v.actus === "mutatio";
    });
    var per_res = {};
    mut.forEach(function (v) {
        per_res[v.res] = JSON.parse(v.datum);
    });

    /* (a) persona: praenomen/cognomen addita, vetera SERVATA */
    var p = per_res["01DEFperson"];
    proba(!!p, "persona emendata");
    if (p) {
        var cp = p.campi.map(function (c) { return c.clavis; });
        proba(cp.indexOf("first_name") >= 0
            && cp.indexOf("last_name") >= 0,
            "praenomen et cognomen addita");
        proba(cp.indexOf("name") >= 0
            && cp.indexOf("year_of_birth") >= 0,
            "campi VETERES personae servati");
    }

    /* (b) adnotatio: optiones AUCTAE, non substitutae */
    var a = per_res["01DEFadnotatio"];
    proba(!!a, "adnotatio emendata");
    if (a) {
        var camp_sp = null;
        var claves = a.campi.map(function (c) { return c.clavis; });
        a.campi.forEach(function (c) {
            if (c.clavis === "species") { camp_sp = c; }
        });
        proba(!!camp_sp, "campus specierum adest");
        if (camp_sp) {
            ["nota", "citatio", "persona", "eventus", "locus",
             "terminus", "societas", "scriptum", "inventum",
             "quaestio"].forEach(function (o) {
                proba(camp_sp.optiones.indexOf(o) >= 0,
                    "optio '" + o + "' in definitione viva");
            });
            aequale(camp_sp.optiones.length, 10,
                "optiones decem, sine duplicibus");
            aequale(camp_sp.optiones[0], "nota",
                "ordo optionum veterum servatus");
        }
        proba(claves.indexOf("responsum") >= 0,
            "responsum additum");
        proba(claves.indexOf("scriptum") >= 0
            && claves.indexOf("inventum") >= 0,
            "campi relationis novi additi");
        /* LEX ADDITIVA: campus alienus SUPERSTES */
        proba(claves.indexOf("campus_meus") >= 0,
            "campus non-desideratus SERVATUS (emendatio additiva)");
        proba(claves.indexOf("textus") >= 0,
            "campus textus servatus");
    }

    /* (c) IDEMPOTENTIA: definitio iam aequata nihil mutat */
    /* mutationes OMNES reapplicare, non genera NOMINATIM: aliter
       aequare novum (eventus, locus, ...) fixturam frangit et
       'non idempotens' nuntiat dum codex rectus est */
    var defs_nova = JSON.parse(JSON.stringify(defs_vetera));
    defs_nova.forEach(function (d) {
        var m = per_res[d.res_id];
        if (m && m.campi) { d.datum.campi = m.campi; }
    });
    return curare_cum(defs_nova).then(function () {
        var mut2 = mutationes("gerere").filter(function (v) {
            return v.actus === "mutatio";
        });
        aequale(mut2.length, 0,
            "aequatio bis vocata nihil mutat (idempotens)");
        var addita = mutationes("addere");
        aequale(addita.length, 0,
            "genera praesentia non re-conduntur");
    });
}).catch(function (e) {
    fracta.push("exceptio: " + (e && (e.message || e)));
});

/* Synchrona omnia (vide Sync supra), ergo hic tuto iudicamus.
 * IACTUS, non $.exit: $.exit in hoc ambitu non exstat, et iactus
 * osascript exitu non-zephyro terminat (porta exitus, ut
 * probatio_velaminis facit - numquam grep summarii). */
if (fracta.length) {
    throw new Error("FRACTA (" + fracta.length + "): "
        + fracta.join(" | "));
}
"FORUM-VELAMEN-BONUM (adsertiones " + probata + ")";

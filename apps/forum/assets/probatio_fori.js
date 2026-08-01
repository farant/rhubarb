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
    Object.defineProperty(e, "textContent", {
        get: function () { return t; },
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
    var defs_nova = JSON.parse(JSON.stringify(defs_vetera));
    defs_nova.forEach(function (d) {
        if (d.datum.clavis === "person") {
            d.datum.campi = p.campi;
        }
        if (d.datum.clavis === "adnotatio") { d.datum.campi = a.campi; }
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

/* probatio velaminis speculi - cursus sine navigatro (JXA/JSC)
 * DOM fictum minimum; speculum.js verum evaluatur; deinde
 * plagula aperitur + quaestio exercetur; FRACTA/BONA imprimitur. */
ObjC.import("Foundation");

function nodum(tag) {
    var e = {
        tag: tag || "", children: [], style: {},
        classList: {
            add: function () {}, remove: function () {}
        },
        appendChild: function (f) { e.children.push(f); return f; },
        removeChild: function (f) {
            var p = e.children.indexOf(f);
            if (p !== -1) { e.children.splice(p, 1); }
        },
        setAttribute: function () {},
        querySelector: function () { return null; },
        scrollIntoView: function () {},
        select: function () {},
        scrollTop: 0, onclick: null, oninput: null,
        value: "", className: "", id: "", placeholder: ""
    };
    var t = "";
    var h = "";
    Object.defineProperty(e, "textContent", {
        get: function () { return t; },
        set: function (v) { t = v; e.children = []; }
    });
    Object.defineProperty(e, "innerHTML", {
        get: function () { return h; },
        set: function (v) { h = v; }
    });
    return e;
}

var fracta = [];
function proba(conditio, titulus) {
    if (!conditio) { fracta.push(titulus); }
}

/* ---- ambitus fictus ---- */
var window = {};
var document = {
    createElement: function (tag) { return nodum(tag); },
    getElementById: function () { return null; },
    head: nodum("head"),
    body: nodum("body"),
    activeElement: null,
    addEventListener: function () {},
    execCommand: function () { return true; }
};
var navigator = {};
function setTimeout(f) { f(); return 1; }
function clearTimeout() {}

var latina_h = "#define si if\n#define redde return\n#define interior static\n";
var exemplum_c = "/* commentarium\n   pergit */\n#include \"latina.h\"\ninterior i32\n_summa(i32 a)\n{\n    si (a > 0) { redde a; }\n    redde 0;  /* cauda */\n}\n";
var effusio = {
    proventus: { app: "probatio", commissum: "abc", sordidum: "verum" },
    fontes: [
        { via: "include/latina.h", magnitudo: latina_h.length, corpus: latina_h },
        { via: "lib/exemplum.c", magnitudo: exemplum_c.length, corpus: exemplum_c }
    ],
    documenta: [
        { via: "lib/exemplum.worklog.md", magnitudo: 10, corpus: "# worklog\n" }
    ],
    exclusa: [
        { via: "vendor/sqlite3.c", magnitudo: "9", digestum: "1", causa: "vendor" }
    ]
};
var internuntius = {
    vocare: function (methodus) {
        return {
            then: function (f) { f(effusio); return { "catch": function () {} }; }
        };
    }
};

/* ---- speculum.js verum evaluare ---- */
var via_plagulae = "/Users/francisarant/Documents/projects/rhubarb/lib/speculum_assets/speculum.js";
var fons = ObjC.unwrap($.NSString.stringWithContentsOfFileEncodingError(
    via_plagulae, $.NSUTF8StringEncoding, null));
eval(fons);

/* ---- probationes ---- */
var res = window.speculum_res;
proba(window.speculum_praesens === true, "praesens");
proba(typeof window.speculum_commutare === "function", "commutare functio");
proba(res.structum === true, "structum post primam vocationem");
proba(res.apertum === true, "apertum post primam vocationem");
proba(res.arbor_index.children.length > 3, "arbor structa");

/* plagulam aperire per buttonem arboris */
var button_exempli = null;
for (var i = 0; i < res.arbor_index.children.length; i++) {
    if (res.arbor_index.children[i].textContent === "lib/exemplum.c") {
        button_exempli = res.arbor_index.children[i];
    }
}
proba(button_exempli !== null, "button exempli in arbore");
if (button_exempli) { button_exempli.onclick(); }
proba(res.via_aperta === "lib/exemplum.c", "plagula aperta");

/* illuminatio: codex = filius secundus visoris (post caput) */
var codex = res.visor.children[1];
proba(codex && codex.children.length === 10, "numerus linearum (10)");
if (codex) {
    proba(codex.children[0].children[1].innerHTML.indexOf("speculum-il-com") !== -1,
        "commentarium illuminatum");
    proba(codex.children[1].children[1].innerHTML.indexOf("speculum-il-com") !== -1,
        "commentarium trans lineas");
    proba(codex.children[2].children[1].innerHTML.indexOf("speculum-il-pra") !== -1,
        "praeprocessor illuminatus");
    proba(codex.children[2].children[1].innerHTML.indexOf("speculum-il-cho") !== -1,
        "chorda inclusionis signata (cho)");
    proba(codex.children[6].children[1].innerHTML.indexOf("speculum-il-lat") !== -1,
        "clavis latina (si) illuminata");
}

/* quaestio plena */
res.quaestio_campus.value = "redde";
res.quaestio_campus.oninput();
proba(res.arbor_index.children.length >= 3,
    "congruentiae quaestionis (caput + 2)");
proba(res.arbor_index.children[0].textContent.indexOf("congruentiae") === 0,
    "caput congruentiarum");
if (res.arbor_index.children.length > 1) {
    /* congruentia prima = include/latina.h (#define redde return) */
    res.arbor_index.children[1].onclick();
    proba(res.via_aperta === "include/latina.h", "saltus ad congruentiam");
}

/* filtrum restituit */
res.quaestio_campus.value = "";
res.filtrum_campus.value = "latina";
res.filtrum_campus.oninput();
var visibiles = 0;
for (var j = 0; j < res.arbor_index.children.length; j++) {
    if (res.arbor_index.children[j].className === "speculum-plagula") { visibiles++; }
}
proba(visibiles === 1, "filtrum arborem coartat (1 plagula)");

if (fracta.length > 0) {
    /* iactus -> osascript exitu non-zephyro exit (porta exitus,
     * numquam grep summarii) */
    throw new Error("FRACTA: " + fracta.join(" | "));
}
"VELAMEN-BONUM (probationes 17)";

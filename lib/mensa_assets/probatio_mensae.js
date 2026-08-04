/* probatio logicae mensae - cursus sine navigatro (JXA/JSC).
 * logica.js verum evaluatur (SINE DOM consulto scriptum);
 * fundere/quaeRedenda/retro/arbiter exercentur.
 * Curre: osascript -l JavaScript lib/mensa_assets/probatio_mensae.js */
ObjC.import("Foundation");

var fracta = [];
var summa = 0;
function proba(conditio, titulus) {
    summa = summa + 1;
    if (!conditio) { fracta.push(titulus); }
}

/* ---- fontem verum legere et evaluare ---- */
(function () {
    var via = "lib/mensa_assets/fontes/logica.js";
    var textus = ObjC.unwrap($.NSString.stringWithContentsOfFileEncodingError(
        via, $.NSUTF8StringEncoding, null));
    if (!textus) {
        throw new Error("logica.js legi non potuit (curre e radice repositorii)");
    }
    eval(textus);

    /* ================= fundere ================= */
    var e = {};
    mensaFundere(e, 'creatum', { id: 'a', x: 10, y: 20, genus_elementi: 'scidula' });
    proba(e.a && e.a.x === 10, "fundere: creatum ponit");
    mensaFundere(e, 'positum', { id: 'a', x: 55, y: 60 });
    proba(e.a.x === 55 && e.a.y === 60, "fundere: positum superscribit");
    proba(e.a.genus_elementi === 'scidula', "fundere: claves aliae manent");
    mensaFundere(e, 'textus', { id: 'a', textus: 'salve' });
    proba(e.a.textus === 'salve', "fundere: textus additur");
    mensaFundere(e, 'deletum', { id: 'a' });
    proba(!e.a, "fundere: deletum removet");
    mensaFundere(e, 'creatum', { id: 'a', x: 1, y: 2 });
    proba(e.a && e.a.x === 1 && e.a.textus === undefined,
        "fundere: resurrectio pura (non vetus)");
    mensaFundere(e, 'positum', { x: 9 });
    proba(e.a.x === 1, "fundere: sine id nihil agit");

    /* ================= quaeRedenda ================= */
    var el2 = {
        a: { x: 1 },                                /* tabula absens = radix */
        b: { tabula: 'radix' },
        c: { tabula: 'theca-1' },
        i: { genus_elementi: 'internum', activa: 'theca-1' }
    };
    var radix = mensaQuaeRedenda(el2, 'radix');
    proba(radix.indexOf('a') !== -1 && radix.indexOf('b') !== -1,
        "quaeRedenda: radix implicita + explicita");
    proba(radix.indexOf('c') === -1, "quaeRedenda: aliena tabula exclusa");
    proba(radix.indexOf('i') === -1, "quaeRedenda: internum numquam");
    var intus = mensaQuaeRedenda(el2, 'theca-1');
    proba(intus.length === 1 && intus[0] === 'c', "quaeRedenda: tabula nidificata");

    /* ================= retro ================= */
    proba(mensaRetro(el2, 'radix', []) === null, "retro: in radice nullum");
    proba(mensaRetro(el2, 'theca-1', ['radix']) === 'radix',
        "retro: acervus vincit");
    var el3 = { 'theca-1': { tabula: 'theca-parens' } };
    proba(mensaRetro(el3, 'theca-1', []) === 'theca-parens',
        "retro: sedes thecae acervo vacuo");
    proba(mensaRetro({}, 'theca-ignota', []) === 'radix',
        "retro: orphana ad radicem");
    var el4 = { 'theca-1': { x: 5 } };
    proba(mensaRetro(el4, 'theca-1', []) === 'radix',
        "retro: theca in radice -> radix");

    /* ================= arbiter ================= */
    var arb = new MensaArbiter(6);
    var f;

    /* tene sine motu -> orbis */
    f = arb.deorsum(100, 100, true);
    proba(f && f.actio === 'tempus_incipe', "arbiter: deorsum incipit tempus");
    f = arb.motus(102, 102);
    proba(f === null, "arbiter: motus intra limen toleratur");
    f = arb.tempus_teneri();
    proba(f && f.actio === 'orbis_aperi' && f.x === 100 && f.y === 100,
        "arbiter: tempus -> orbis ad sedem deorsi");
    proba(arb.status === 'orbis', "arbiter: status orbis");
    f = arb.duplex(50, 50, true);
    proba(f === null, "arbiter: duplex suppressum orbe aperto");
    f = arb.deorsum(10, 10, true);
    proba(f === null, "arbiter: deorsum suppressum orbe aperto");
    arb.orbis_clausus();
    proba(arb.status === 'quies', "arbiter: orbis clausus -> quies");

    /* motus magnus -> tractus, non orbis */
    arb.deorsum(100, 100, true);
    f = arb.motus(120, 100);
    proba(f && f.actio === 'tempus_dele', "arbiter: motus magnus delet tempus");
    f = arb.tempus_teneri();
    proba(f === null, "arbiter: tempus post motum nihil agit");

    /* sursum ante tempus -> deletio */
    arb.deorsum(5, 5, true);
    f = arb.sursum();
    proba(f && f.actio === 'tempus_dele', "arbiter: sursum ante tempus delet");

    /* deorsum extra planum -> nihil */
    f = arb.deorsum(1, 1, false);
    proba(f === null, "arbiter: deorsum in scida non nostrum");
    f = arb.tempus_teneri();
    proba(f === null, "arbiter: tempus sine tenendo nihil");

    /* duplex in plano quiete -> creatio */
    f = arb.duplex(30, 40, true);
    proba(f && f.actio === 'duplex_planum' && f.x === 30,
        "arbiter: duplex in plano creat");
    f = arb.duplex(30, 40, false);
    proba(f === null, "arbiter: duplex in scida non nostrum");
})();

if (fracta.length > 0) {
    console.log("FRACTA (" + fracta.length + "/" + summa + "):");
    fracta.forEach(function (t) { console.log("  " + t); });
    throw new Error("probatio mensae fracta");
}
console.log("BONA: " + summa + " probationes logicae mensae");

/* js_syntaxis.js - oraculum syntaxis JS (JXA; per js_syntaxis.sh voca)
 *
 * CUR OMNINO: haec domus JS duobus modis parit, et NEUTER a
 * compilatore custoditur -
 *   (a) manus eum ex chordis C CONCATENAT ('"return{ok:false,visum:\'"');
 *   (b) laboratorium plagulas staticas fert quas nemo compilat.
 *
 * VITIA VERA quae hoc peperit (2026-08-15/17):
 *   - manus_focus_ponere: '}' intra litteras JS clausum, ergo
 *     obiectum numquam clausum. Probatio simulacri asserta in
 *     CONTENTO facit et vitium SYNTAXIS capere structuraliter non
 *     potest; sola applicatio viva 'Unexpected identifier' dixit.
 *   - experimentum 0003: paren impar a renominatione relictus.
 * Bis in duobus diebus. Ergo porta.
 *
 * 'new Function(corpus)' PARSAT sed NON exsequitur - quod exacte
 * opus est: vitium syntaxis iacit, effectus nulli fiunt. (Nullum
 * API 'parsa solum' in JS est; hoc idioma stat pro eo.)
 *
 * LIMES HONESTUS: corpus FUNCTIONIS parsatur, non modulus. 'import'
 * et 'export' ergo hic reicerentur - haec domus eis non utitur. Et
 * syntaxis sola iudicatur: 'q(' non definitum syntaxis recta est.
 *
 * Effusio: linea per plagulam, deinde 'SUMMA <omnes> <pravae>'.
 * Crusta exitum decernit - osascript enim exitum suum ex iactu
 * regit, quod nuntios plurium plagularum perderet.
 */

ObjC.import('Foundation');

function _legere (via) {
    var s = $.NSString.stringWithContentsOfFileEncodingError(
        $(via), $.NSUTF8StringEncoding, $());
    if (!s) { return null; }
    var t = ObjC.unwrap(s);
    return (typeof t === 'string') ? t : null;
}

function run (argv) {
    var lineae = [];
    var pravae = 0;
    var i, via, fons;

    for (i = 0; i < argv.length; i++) {
        via = argv[i];
        fons = _legere(via);

        if (fons === null) {
            lineae.push('ILLEGIBILIS ' + via);
            pravae++;
            continue;
        }
        /* Plagula VACUA valida est, sed fere semper vitium fabricae
         * significat (globus qui nihil cepit, scriptura interrupta).
         * Ergo nominatur, non tacetur - sed non numeratur inter
         * pravas, quia syntaxis eius culpa non est. */
        if (fons.replace(/\s/g, '').length === 0) {
            lineae.push('VACUA ' + via);
            continue;
        }
        try {
            /* jshint evil:true */
            new Function(fons);
            lineae.push('VALIDUM ' + via);
        } catch (e) {
            lineae.push('PRAVUM ' + via + ': ' + e.message);
            pravae++;
        }
    }

    lineae.push('SUMMA ' + argv.length + ' ' + pravae);
    return lineae.join('\n');
}

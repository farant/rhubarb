#ifndef ARGUMENTA_H
#define ARGUMENTA_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"

/* ==================================================
 * ARGUMENTA - Parsitio Argumentorum Lineae Mandati
 *
 * Bibliotheca pro parsendo argumenta lineae mandati.
 * Sustinet vexilla, optiones cum valoribus, et
 * argumenta positionalia.
 *
 * PHILOSOPHIA:
 * - API aedificatoris (builder pattern)
 * - Omnia ex piscina allocata
 * - Generatio auxilii automatica
 * - Errores clari et utiles
 * - Simplex et composabilis
 *
 * EXEMPLUM SIMPLEX:
 *   ArgumentaParser* parser = argumenta_creare(piscina);
 *
 *   argumenta_addere_vexillum(parser, "-v", "--verbose",
 *       "Enable verbose output");
 *   argumenta_addere_optionem(parser, "-o", "--output",
 *       "Output file path");
 *   argumenta_addere_positionalem(parser, "input",
 *       "Input file", VERUM);
 *
 *   ArgumentaFructus* fructus = argumenta_parsere(parser, argc, argv);
 *
 *   b32 verbose = argumenta_habet_vexillum(fructus, "--verbose");
 *   chorda output = argumenta_obtinere_optionem(fructus, "--output", piscina);
 *   chorda input = argumenta_obtinere_positionalem(fructus, ZEPHYRUM, piscina);
 *
 * SYNTAX SUSTENTA:
 *   --verbose                    (vexillum longum)
 *   -v                          (vexillum breve)
 *   --output=file.txt           (optio cum valore, equals)
 *   --output file.txt           (optio cum valore, spatium)
 *   -o file.txt                 (optio brevis cum valore)
 *   mytool input.txt            (positionalis)
 *   mytool -v input.txt         (vexillum et positionalis)
 *
 * ================================================== */


/* ==================================================
 * Typi Opaci
 * ================================================== */

nomen structura ArgumentaParser ArgumentaParser;
nomen structura ArgumentaFructus ArgumentaFructus;


/* ==================================================
 * Creatio
 * ================================================== */

/* Creare parserem novum
 *
 * piscina: piscina pro omnibus allocationibus
 *
 * Reddit: parserem novum vel NIHIL si allocatio fracta
 */
ArgumentaParser*
argumenta_creare (
	Piscina* piscina);


/* ==================================================
 * Definitio - Vexilla
 * ================================================== */

/* Addere vexillum (commutator binarius)
 *
 * Vexillum est optio sine valore (praesens vel absens).
 *
 * parser:      parser ad quem addere
 * nomen_brevis: nomen breve (e.g., "-v") vel NIHIL
 * nomen_longus: nomen longum (e.g., "--verbose") vel NIHIL
 * descriptio:  descriptio pro auxilio
 *
 * NOTA: Saltem unum ex nomen_brevis vel nomen_longus debet non-NIHIL esse.
 *
 * EXEMPLA:
 *   argumenta_addere_vexillum(p, "-v", "--verbose", "Verbose output");
 *   argumenta_addere_vexillum(p, NIHIL, "--help", "Show help");
 *   argumenta_addere_vexillum(p, "-q", NIHIL, "Quiet mode");
 */
vacuum
argumenta_addere_vexillum (
	        ArgumentaParser* parser,
	constans     character* nomen_brevis,
	constans     character* nomen_longus,
	constans     character* descriptio);


/* ==================================================
 * Definitio - Optiones
 * ================================================== */

/* Addere optionem (vexillum cum valore)
 *
 * Optio est vexillum quod valorem accipit.
 *
 * parser:       parser ad quem addere
 * nomen_brevis: nomen breve (e.g., "-o") vel NIHIL
 * nomen_longus: nomen longum (e.g., "--output") vel NIHIL
 * descriptio:   descriptio pro auxilio
 *
 * SYNTAX ACCEPTA:
 *   --output=file.txt
 *   --output file.txt
 *   -o file.txt
 *
 * EXEMPLA:
 *   argumenta_addere_optionem(p, "-o", "--output", "Output file");
 *   argumenta_addere_optionem(p, NIHIL, "--config", "Config file");
 */
vacuum
argumenta_addere_optionem (
	        ArgumentaParser* parser,
	constans     character* nomen_brevis,
	constans     character* nomen_longus,
	constans     character* descriptio);


/* ==================================================
 * Definitio - Positionalia
 * ================================================== */

/* Addere argumentum positionale
 *
 * Argumenta positionalia sunt argumenta sine nomina
 * (e.g., input file, output file).
 *
 * parser:      parser ad quem addere
 * nomen:       nomen pro auxilio et errores
 * descriptio:  descriptio pro auxilio
 * necessarius: VERUM si necessarius, FALSUM si optionalis
 *
 * NOTA: Positionalia necessaria debent venire ante optionalia.
 *
 * EXEMPLA:
 *   argumenta_addere_positionalem(p, "input", "Input file", VERUM);
 *   argumenta_addere_positionalem(p, "output", "Output file", FALSUM);
 */
vacuum
argumenta_addere_positionalem (
	        ArgumentaParser* parser,
	constans     character* nomen,
	constans     character* descriptio,
	                   b32  necessarius);


/* ==================================================
 * Definitio - Auxilium Manuale
 * ================================================== */

/* Ponere descriptionem programmi
 *
 * Descriptio apparet ad initium auxilii.
 *
 * parser:     parser
 * descriptio: descriptio brevis programmi
 *
 * EXEMPLUM:
 *   argumenta_ponere_descriptionem(p,
 *       "mytool - A utility for processing files");
 */
vacuum
argumenta_ponere_descriptionem (
	        ArgumentaParser* parser,
	constans     character* descriptio);

/* Addere exemplum usu
 *
 * Exempla apparent post descriptionem vexillorum in auxilio.
 * Potest vocari multipliciter pro exemplis multiplicibus.
 *
 * parser:   parser
 * exemplum: linea exempli (sine titulo "EXEMPLUM:")
 *
 * EXEMPLUM:
 *   argumenta_ponere_exemplum(p, "mytool -v input.txt");
 *   argumenta_ponere_exemplum(p, "mytool --output=out.txt input.txt");
 */
vacuum
argumenta_addere_exemplum (
	        ArgumentaParser* parser,
	constans     character* exemplum);


/* ==================================================
 * Parsitio
 * ================================================== */

/* Parsere argumenta (fatalis si errore)
 *
 * Parsit argumenta ex argc/argv. Si errore occurrit,
 * imprimet nuntium erroris et exibit programma.
 *
 * parser: parser cum definitionibus
 * argc:   numerus argumentorum
 * argv:   tabula argumentorum
 *
 * Reddit: fructus parsitionis (numquam NIHIL)
 *
 * NOTA: Hoc est modus normalis. Pro erroribus non-fatalibus,
 *       usa argumenta_conari_parsere.
 */
ArgumentaFructus*
argumenta_parsere (
	ArgumentaParser* parser,
	             i32  argc,
	      character** argv);

/* Parsere argumenta (reddit NIHIL si errore)
 *
 * Parsit argumenta ex argc/argv. Si errore occurrit,
 * reddit NIHIL sine exitio.
 *
 * parser: parser cum definitionibus
 * argc:   numerus argumentorum
 * argv:   tabula argumentorum
 *
 * Reddit: fructus parsitionis vel NIHIL si errore
 *
 * EXEMPLUM:
 *   ArgumentaFructus* f = argumenta_conari_parsere(p, argc, argv);
 *   si (!f) {
 *       argumenta_imprimere_errorem(p);
 *       redde I;
 *   }
 */
ArgumentaFructus*
argumenta_conari_parsere (
	ArgumentaParser* parser,
	             i32  argc,
	      character** argv);


/* ==================================================
 * Interrogatio - Vexilla
 * ================================================== */

/* Verificare si vexillum praesens est
 *
 * fructus: fructus parsitionis
 * nomen:   nomen vexilli (brevis vel longus)
 *
 * Reddit: VERUM si vexillum datum, FALSUM alioquin
 *
 * EXEMPLA:
 *   si (argumenta_habet_vexillum(f, "--verbose")) { ... }
 *   si (argumenta_habet_vexillum(f, "-v")) { ... }
 *
 * NOTA: Potest quaerere per nomen breve vel longum.
 */
b32
argumenta_habet_vexillum (
	constans ArgumentaFructus* fructus,
	constans        character* nomen);


/* ==================================================
 * Interrogatio - Optiones
 * ================================================== */

/* Obtinere valorem optionis
 *
 * fructus: fructus parsitionis
 * nomen:   nomen optionis (brevis vel longus)
 * piscina: piscina pro allocare chordam
 *
 * Reddit: chordam cum valore, vel chordam vacuum (mensura=0)
 *         si optio non data
 *
 * EXEMPLA:
 *   chorda output = argumenta_obtinere_optionem(f, "--output", p);
 *   si (output.mensura > ZEPHYRUM) {
 *       // optio data
 *   }
 *
 * NOTA: Potest quaerere per nomen breve vel longum.
 */
chorda
argumenta_obtinere_optionem (
	constans ArgumentaFructus* fructus,
	constans        character* nomen,
	                 Piscina* piscina);


/* ==================================================
 * Interrogatio - Positionalia
 * ================================================== */

/* Obtinere argumentum positionale per indicem
 *
 * fructus: fructus parsitionis
 * index:   index argumenti (0-based)
 * piscina: piscina pro allocare chordam
 *
 * Reddit: chordam cum valore, vel chordam vacuum si index invalidus
 *
 * EXEMPLUM:
 *   chorda input = argumenta_obtinere_positionalem(f, ZEPHYRUM, p);
 *   chorda output = argumenta_obtinere_positionalem(f, I, p);
 */
chorda
argumenta_obtinere_positionalem (
	constans ArgumentaFructus* fructus,
	                       i32  index,
	                  Piscina* piscina);

/* Obtinere numerum argumentorum positionalium
 *
 * fructus: fructus parsitionis
 *
 * Reddit: numerus argumentorum positionalium datorum
 *
 * EXEMPLUM:
 *   i32 numerus = argumenta_numerus_positionalium(f);
 *   per (i32 i = ZEPHYRUM; i < numerus; i++) {
 *       chorda arg = argumenta_obtinere_positionalem(f, i, p);
 *       // ...
 *   }
 */
i32
argumenta_numerus_positionalium (
	constans ArgumentaFructus* fructus);


/* ==================================================
 * Auxilium et Errores
 * ================================================== */

/* Imprimere textum auxilii
 *
 * Imprimet textum auxilii generatum ex definitionibus
 * ad stdout.
 *
 * parser: parser cum definitionibus
 *
 * EXEMPLUM:
 *   si (argumenta_habet_vexillum(f, "--help")) {
 *       argumenta_imprimere_auxilium(parser);
 *       exire(ZEPHYRUM);
 *   }
 */
vacuum
argumenta_imprimere_auxilium (
	constans ArgumentaParser* parser);

/* Imprimere nuntium erroris recens
 *
 * Imprimet nuntium erroris recens ad stderr.
 * Utile post argumenta_conari_parsere reddit NIHIL.
 *
 * parser: parser (potest continere informationem erroris)
 *
 * EXEMPLUM:
 *   ArgumentaFructus* f = argumenta_conari_parsere(p, argc, argv);
 *   si (!f) {
 *       argumenta_imprimere_errorem(p);
 *       redde I;
 *   }
 */
vacuum
argumenta_imprimere_errorem (
	constans ArgumentaParser* parser);

#endif /* ARGUMENTA_H */

/* materia_pictor.h - Diagnosticum -> textus humanus
 * (project-specs/exemplaria-c-spec.md E4)
 *
 * Forma quam usor legit:
 *
 *   plagula.sh:2:1: [erratum] crusta:grex/tok_clausura
 *     '}' exspectata
 *      1 | echo a; { echo b
 *        |         ^~~~~~~~ hic coepit
 *      2 |
 *        | ^ hic exspectatur
 *
 * ==================================================
 * CHORDAM REDDIT, NUMQUAM SCRIBIT
 * ==================================================
 *
 * I/O instrumenti est, non substrati: hoc chordam componit, et
 * instrumentum eam ad stdout scribit (aut ad LSP mittit, aut in
 * acta ponit). Ergo consumptor quisque formam eandem habet sine
 * officio exitus.
 *
 * ==================================================
 * CUR HIC, CUM 'PICTURA MATERIAE NON SIT'
 * ==================================================
 *
 * materia_diagnostica.h:11 et materia/CLAUDE.md dicunt picturam
 * materiae non esse. Regula ANGUSTIOR est quam videtur, et
 * include/excerptum.h causam suam dicit: 'sedem materia dat, formam
 * haec; ergo nihil de arboribus scit et A QUOLIBET FONTE vocari
 * potest'. Vetat ergo formam IN MATERIA REFINGERE - non componere.
 *
 * Hoc nihil refingit: lineam capitis texit et excerptum_scribere_multa
 * vocat. Et infra materiam poni NON potest, quia MateriaDiagnosticum
 * poscit et lib/ materiam includere non debet.
 */

#ifndef MATERIA_PICTOR_H
#define MATERIA_PICTOR_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "materia_diagnostica.h"

/* Diagnosticum unum -> chorda.
 *
 * 'grammatica' codici sine ':' praefigitur (codices substrati et
 * lintris eum iam ferunt, ergo intacti transeunt); NIHIL = nullum
 * praefixum.
 *
 * 'fons'/'mensura' excerptum secant. 'excerptum' FALSUM aut tractus
 * sine sede (initium < 0) caput solum reddit.
 *
 * Sedes relatae cum primaria in bloco UNO pinguntur, ordine octetorum
 * (materia-sedes par. III). Sedes relata INSANA praetermittitur et
 * primaria superest: pictor seriem totam recusaret, et excerptum
 * TOTUM perdere peius est quam sedem unam.
 *
 * Chorda vacua (mensura ZEPHYRUM) = argumentum abest aut memoria
 * defecit. */
chorda
materia_pictor_scribere (
                     Piscina* piscina,
constans MateriaDiagnosticum* d,
          constans character* via,
          constans character* grammatica,
          constans character* fons,
                         i32  mensura,
                         b32  excerptum);

#endif /* MATERIA_PICTOR_H */

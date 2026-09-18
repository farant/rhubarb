/* materia_excusatio.h - Excusationes declaratae super diagnostica
 * (project-specs/excusatio-declarata-spec.md par. V; desideratum
 * 01M2SD93PV)
 *
 * Auctor intentionem DECLARAT, lintrum eam non infert:
 *
 *   # <tolera codex="lint:nt-aequalitas" (>caput recentissimum
 *   if [ -n "$X" -a "$X" -nt "$bin" ]; then
 *
 * Inventum EXCUSATUR si sedes PRIMARIA eius intra 'scopus'
 * annotationis iacet ET codices AEQUALES sunt. Sedes relatae non
 * participant: inventum uno loco EST (SM1 specificationis sedium
 * multiplicium), et sedes relata locus alter eiusdem inventi est,
 * non inventum alterum.
 *
 * CLAVIS EST CODEX QUEM PICTOR PINGIT. Arcus adaptatoris codicem
 * 'lint:<nomen>' omni invento lintris dedit, et declarata
 * '<grammatica>:<codex>' iam ferebant - ergo auctor id scribit quod
 * in nuntio erroris VIDIT. Nihil novum inventum est.
 *
 * ==================================================
 * CUR CAPUT SEPARATUM, NON materia_diagnostica.h
 * ==================================================
 *
 * materia_diagnostica.h latina + piscina + xar + nodus + registrum
 * solum includit. Excusationes stml (per annotationes) poscunt, ergo
 * functio illic posita stml in CONTRACTUM DIAGNOSTICORUM traheret -
 * cliens qui diagnostica derivat et annotationes non vult parsatorem
 * STML nihilominus ferret. materia_registrum.h paupertatem suam
 * MERITUM vocat ('NIHIL INCLUDIT PRAETER latina.h'); eadem ratio hic.
 * Specificatio 'materia_diagnostica_excusare' nominabat; divergentia
 * C plani, in par. IX specificationis notanda.
 *
 * ==================================================
 * EXCUSATIO MORTUA VITIUM EST
 * ==================================================
 *
 * Lectio eslint, quam silva_c89_semantica.c:1084 iam aedificavit:
 * suppressiones putrescunt. Excusatio quae NIHIL absorbuit, aut sine
 * causa est, aut parsari nequit, diagnosticum SUUM parit, super
 * COMMENTARIO positum - aliter exemptio consulta a rancida distingui
 * non potest.
 *
 * QUOD NONDUM IUDICATUR: codex 'lint:...' ignotus. Registrum codices
 * gradus I omnes novit (MateriaTabDiagnosticum.codex), ergo
 * '<tolera codex="crusta:grex/tok_clausura">' HODIE probatur; nomina
 * lintris registrum nullum habent donec cursor regularum exsistat.
 * Signum nominatum: ille cursor (EX8).
 */

#ifndef MATERIA_EXCUSATIO_H
#define MATERIA_EXCUSATIO_H

#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "materia_annotationes.h"
#include "materia_diagnostica.h"
#include "materia_registrum.h"

/* Elementum quod excusat. Familiae aliae (nid, intentio, contractus)
 * per idem collectorum veniunt et hic TACITE praetereuntur - annotatio
 * inconsumpta excusatio mortua NON est. */
#define MATERIA_EXCUSATIO_TAG  "tolera"

#define MATERIA_CODEX_EXCUSATIO_MORTUA \
    "materia:excusatio-mortua"
#define MATERIA_CODEX_EXCUSATIO_SINE_CAUSA \
    "materia:excusatio-sine-causa"
#define MATERIA_CODEX_EXCUSATIO_FRACTA \
    "materia:excusatio-fracta"
#define MATERIA_CODEX_EXCUSATIO_IGNOTA \
    "materia:excusatio-ignota"

/* Diagnostica per annotationes excusare.
 *
 * 'grammatica' (NIHIL licet) nomen clientis est ("crusta", "css"):
 * PICTOR id codici sine ':' praefigit, ergo auctor
 * 'crusta:grex/tok_clausura' VIDIT et id scribit. Sine hoc EX1
 * ('codex quem pictor pingit') pro codicibus gradus I FALSUM esset -
 * inventa a codicibus nudis excusarentur solum, et forma quam usor
 * legit tacite non congrueret. Ambae formae accipiuntur.
 *
 * 'declarata' (NIHIL licet) codices gradus I novit; cum adest, codex
 * qui ':' fert sed in tabula non est IGNOTUS nominatur. Praefixum
 * 'lint:' semper praeteritur (vide supra).
 *
 * Xar NOVUM reddit, ordine (tractus.initium, codex) ut derivare -
 * diagnostica excusationum ipsarum in commentario sedent, ergo ante
 * inventa quae excusarent plerumque cadunt et ordo servandus est.
 * Argumentum non mutatur. NIHIL si argumentum necessarium abest aut
 * memoria defecit. */
Xar*
materia_excusatio_applicare (
                             Piscina* piscina,
                        constans Xar* diagnostica,
                        constans Xar* annotationes,
    constans MateriaDiagnosticaCocta* declarata,
                  constans character* grammatica,
                        constans Xar* lintres);

/* 'lintres' (EX8; NIHIL licet): NOMINA lintrium quae REVERA
 * CUCURRERUNT (materia_exemplaria_lintres). Excusatio cuius codex
 * 'lint:X' est mortua nominatur SOLUM si X inter ea est.
 *
 * NOMINA, NON VEXILLUM. Forma prior 'copiam plenam' per b32 nuntiabat,
 * et fallebat: copia plena DIRECTORII ALTERII regulam nominatam non
 * continet. Mensuratum 2026-09-18, eodem die quo scripta est: sub
 * lintro alieno XIV annotationes domus VERAE mortuae nominatae sunt.
 * 'Mortua' significare debet 'regula eius CUCURRIT et nihil absorbuit',
 * numquam 'regula eius non cucurrit' - quod est ipsum responsum falsum
 * fidenter datum quod EX8 vitare volebat.
 *
 * NIHIL aut vacuum = nulla lintris iudicatur: cursus gradus I solus,
 * aut tabula aliunde lecta et iam cribrata ('-lege').
 *
 * Causa et parsura excusationis NIHILOMINUS semper iudicantur: eas
 * vocans quisque VIDET, regulis nullis opus est. */

#endif /* MATERIA_EXCUSATIO_H */

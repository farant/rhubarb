/* crusta_facies.h - Facies diagnosticorum crustae
 * (project-specs/exemplaria-c-spec.md par. IV; res 01M2RYR3JJ)
 *
 * TRES GRADUS, quisque statio legitima. Consumptor unum eligit et
 * machinam materiae NUMQUAM videt:
 *
 *   I   crusta_diagnostica_textus  chorda humana (caput, causa,
 *                                  excerptum '^~~~')
 *   II  crusta_diagnostica_omnia   recorda (MateriaDiagnosticum)
 *   III crusta_arbor_parsare       arbor ipsa (IAM EXSTAT, immota)
 *
 * ==================================================
 * PENDENTIA: CONSUMPTOR -> CRUSTA -> MATERIA -> LIB
 * ==================================================
 *
 * Crusta materiam semper petebat; quod MUTATUR est quod consumptor
 * eam non iam petit. Ergo si crustae diagnostica mutantur - gradus II
 * additus, excusatio declarata, pictor novus - consumptor nihil
 * emendat. Facies lingua propria loquitur; ORCHESTRATIO abscondita
 * est, VOCABULARIUM (MateriaDiagnosticum) commune manet - aliter
 * consumptor quisque translationem suam scriberet et dispositor
 * trans clientes omnino perire.
 *
 * ==================================================
 * GRADUS AMBO, SEMPER
 * ==================================================
 *
 * 'omnia' omnia significat: errata DECLARATA (registrum, gradus I) et
 * inventa LINTRIS (regulae, gradus II), parsura una, collectio
 * annotationum una, excusatio una. Instrumentum lintris SEPARATUM non
 * exstat quia hoc id subsumit (E3).
 *
 * ==================================================
 * PORCELLANA NON HIC
 * ==================================================
 *
 * Glob, listae plagularum, codices exitus, numeri ad INSTRUMENTUM
 * pertinent (crusta/facies.sh, tools/diagnostica). Vocatio una
 * plagulam unam tractat - forma bibliothecae haec est. Instrumentum
 * quod plagulas multas vult ansam scribit.
 */

#ifndef CRUSTA_FACIES_H
#define CRUSTA_FACIES_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "materia_diagnostica.h"

/* Directorium regularum ordinarium, ad radicem repositorii relatum.
 * Consumptor qui aliunde currit 'optiones.lintrum' ponat. */
#define CRUSTA_LINTRUM "crusta/lintrum"

/* Omnia NIHIL/ZEPHYRUM (aut 'optiones' ipsum NIHIL) = mos ordinarius:
 * regulae omnes in crusta/lintrum/, excusationes applicatae,
 * subtractiones declaratae applicatae. */
nomen structura {
    /* Xar de StmlNodus* IAM LECTARUM; NIHIL = quidquid in 'lintrum'
     * iacet. Regulas dare significat materiam nihil legere. */
                    Xar* regulae;
    /* Directorium regularum; NIHIL = CRUSTA_LINTRUM. */
     constans character* lintrum;
    /* LEX INTERNAMENTI (materia/CLAUDE.md): si 'regulae' datae sunt,
     * hoc internamentum IDEM esse debet quo lectae sunt, aliter
     * comparatio identitatis internatae exemplaria OMNIA MUTA reddit.
     * NIHIL = novum (et tunc 'regulae' NIHIL sint). */
    InternamentumChorda* intern;
    /* VERUM = excusationes declaratas ('<tolera>') NON applicare. */
                    b32 sine_excusatione;
    /* VERUM = subtractiones declaratas ('<relatum minuit=>') NON
     * applicare: bracchia CRUDA, ut auctor regulae ea scripsit. */
                    b32 crudum;
} CrustaOptiones;

/* GRADUS II - recorda, ordine (initium, codex).
 *
 * 'causa' (NIHIL licet) refutationem NOMINAT: regula fracta,
 * directorium absens, bracchia vaga. NIHIL sine causa = memoria
 * aut fons absens.
 *
 * Xar vacuum SANUM est. NIHIL = refutatio (vide 'causa'). */
Xar*
crusta_diagnostica_omnia (
                Piscina*  piscina,
     constans character*  fons,
                    i32   mensura,
constans CrustaOptiones*  optiones,
     constans character** causa);

/* GRADUS I - textus humanus omnium inventorum, ordine eodem.
 *
 * 'via' in capite lineae apparet. 'excerptum' FALSUM = caput et causa
 * solae (sine fonte et '^~~~').
 *
 * Chorda vacua = nihil inventum. Refutatio: chorda vacua cum 'causa'
 * posita - ergo vocans 'causa' inspiciat, non mensuram. */
chorda
crusta_diagnostica_textus (
                Piscina*  piscina,
     constans character*  via,
     constans character*  fons,
                    i32   mensura,
                    b32   excerptum,
constans CrustaOptiones*  optiones,
     constans character** causa);

/* Regulas ex directorio legere: '*.stml', ORDINE TITULORUM (ordo quem
 * systema fert non est ordo stabilis). Xar de StmlNodus*.
 *
 * Facies hoc sponte vocat cum 'optiones->regulae' NIHIL est; vocans
 * qui regulas semel legere et pluries adhibere vult eam ipse vocet.
 *
 * NIHIL = directorium absens aut regula fracta ('causa' posita). */
Xar*
crusta_regulae_legere (
                Piscina*  piscina,
     constans character*  directorium,
    InternamentumChorda*  intern,
     constans character** causa);

#endif /* CRUSTA_FACIES_H */

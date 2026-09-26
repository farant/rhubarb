/* materia_diagnostica.h - Diagnostica DERIVATA ex arbore
 *
 * Declarationes registri (materia_registrum.h, B1 plani
 * materia-sedes) tabulam coctam dant; hic ambulator UNUS eas super
 * arborem quamlibet applicat - parsatam, ex STML relectam, aut
 * visionem. Cliens codicem diagnosticorum nullum scribit: quod
 * declaratum est derivatur, et quod vestigium in arbore non relinquit
 * (bracchium conditionale sine lexemate) a parsatore per 'emissa'
 * infertur.
 *
 * Quod HIC non est, consulto: pictura (excerptum, caret) et
 * instrumentum. Substratum sedes dat, non formam.
 *
 * Ordo exitus: tractus.initium, deinde codex (stabilis).
 */
#ifndef MATERIA_DIAGNOSTICA_H
#define MATERIA_DIAGNOSTICA_H

#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "internamentum.h"
#include "materia_lexicon.h"
#include "materia_nodus.h"
#include "materia_registrum.h"
#include "materia_diagnosticum.h"

/* Forma diagnostici (MateriaDiagnosticum, MateriaSedesRelata, codices
 * et notae substrati): materia_diagnosticum.h - caput sine corpore,
 * ut emittentes (parsatores) formam sine machina includant (T15). */

/* Xar de MateriaDiagnosticum, ordine (tractus.initium, codex).
 * 'emissa' (NIHIL licet): Xar de MateriaDiagnosticum a parsatore
 * emissa (classes sine vestigio in arbore) - tractus computatur si
 * initium -I et nodus adest. Emissum cum nodo et codice diagnostici
 * DECLARATI eiusdem nodi id SUPERAT (declaratum omittitur): parsator
 * eundem defectum accuratius novit (T15, mors GLR C89). Xar vacuum =
 * sanum; NIHIL = memoria defecit aut argumentum necessarium abest. */
Xar*
materia_diagnostica_derivare (
                            Piscina* piscina,
              constans MateriaNodus* radix,
    constans MateriaRegistrumCoctum* genera,
   constans MateriaDiagnosticaCocta* diagnostica,
         constans MateriaOrigoUncus* uncus,
                                Xar* emissa);


/* ==================================================
 * Sequentia plena - quod cliens quisque aliter exscriberet
 * ==================================================
 *
 * Gradus I derivatus, gradus II per regulam quamque extractus,
 * annotationes semel collectae, excusationes applicatae, ordo. Centum
 * lineae quas facies quaeque repeteret, SEMEL scriptae.
 *
 * ==================================================
 * 'ratio' A CLIENTE DATUR - MATERIA EAM NON TENET
 * ==================================================
 *
 * Adumbratio prior tabulam clientium INTRA materiam posuit, suffixo
 * dispositam. Ea putredo est quam M8 nominat, et Fran eam recusavit.
 * Structura superest, dominium invertitur, et probatio quae sola
 * refert haec est: CLIENTEM ADDERE HOC CAPUT NON EMENDAT. materia
 * nihil enumerat, nihil disponit, nomen nullius clientis novit -
 * cliens se ipsum nominat cum vocat.
 *
 * STML HIC NON APPARET consulto: 'regulae' Xar OPACUM est (arbores
 * StmlNodus* iam lectae), ergo contractus diagnosticorum parsatorem
 * STML non trahit. Cliens regulas ipse invenit et legit - quae
 * plagulae, ubi sitae, res clientis est.
 */

nomen structura {
                  constans character* grammatica;  /* "crusta" */
     /* 'registrum' macro latinae est (= register) */
     constans MateriaRegistrumCoctum* tabularium;
        constans MateriaLexiconRatum* lexicon;
    constans MateriaDiagnosticaCocta* declarata;
                  /* "#"; NIHIL = annotationes nullae */
                  constans character* praefixum;
                  /* NIHIL = gradus I solus */
                                 Xar* regulae;
    /* INTERNAMENTUM QUO REGULAE LECTAE SUNT. Lex domus
     * (materia/CLAUDE.md): proiectio in EODEM internamento ac
     * exemplaria aedificetur, aliter comparatio identitatis internatae
     * exemplaria OMNIA MUTA reddit. Mensuratum 2026-09-18: gradus II
     * ordines ZERO reddebat, proiectione et regula ambabus rectis.
     * NIHIL licet si 'regulae' NIHIL est. */
             InternamentumChorda* intern;
    /* VERUM = subtractiones DECLARATAS ('<relatum minuit="X">', vide
     * materia_exemplaria.h) NON applicare: bracchia CRUDA reddit, ut
     * auctor regulae ea scripsit. Numeri per bracchium et differentia
     * contra extractorem priorem hoc poscunt; via ordinaria FALSUM. */
                                 b32 crudum;
} MateriaDiagnosticaRatio;

/* Xar NOVUM de MateriaDiagnosticum, ordine (initium, codex).
 * 'emissa' (NIHIL licet) diagnostica a parsatore nata fert - quod
 * arbor non servat, solus parsator scit.
 *
 * 'causa' (NIHIL licet) refutationem NOMINAT ubi una est - hodie sola
 * bracchia vaga (materia_exemplaria_minuere). NIHIL tacitum refutatio
 * muta esset, et regula fracta numerum minorem tacite redderet.
 *
 * NIHIL = argumentum necessarium abest, memoria defecit, aut regula
 * refutata est ('causa' posita). */
Xar*
materia_diagnostica_plena (
                             Piscina*  piscina,
               constans MateriaNodus*  radix,
    constans MateriaDiagnosticaRatio*  ratio,
                                 Xar*  emissa,
                  constans character** causa);

#endif /* MATERIA_DIAGNOSTICA_H */

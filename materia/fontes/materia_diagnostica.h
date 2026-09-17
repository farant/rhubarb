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
#include "materia_nodus.h"
#include "materia_registrum.h"

/* Codices substrati: ':' eos ab codicibus clientis ('grex/liberi')
 * distinguit - clientis codex titulos registri suos fert. */
#define MATERIA_CODEX_ORDO       "materia:ordo-octetorum"
#define MATERIA_CODEX_SCRIPTURA  "materia:scriptura"

nomen structura {
                       s32  gravitas;   /* MateriaGravitas */
        constans character* codex;      /* "grex/tok_clausura" */
        constans character* causa;
     constans MateriaNodus* nodus;      /* NIHIL pro ordine rupto */
     constans MateriaToken* lexema;     /* ordo ruptus solum */
            MateriaTractus  tractus;    /* initium -I = nulla sedes */
} MateriaDiagnosticum;

/* Xar de MateriaDiagnosticum, ordine (tractus.initium, codex).
 * 'emissa' (NIHIL licet): Xar de MateriaDiagnosticum a parsatore
 * emissa (classes sine vestigio in arbore) - tractus computatur si
 * initium -I et nodus adest. Xar vacuum = sanum; NIHIL = memoria
 * defecit aut argumentum necessarium abest. */
Xar*
materia_diagnostica_derivare (
                            Piscina* piscina,
              constans MateriaNodus* radix,
    constans MateriaRegistrumCoctum* genera,
   constans MateriaDiagnosticaCocta* diagnostica,
         constans MateriaOrigoUncus* uncus,
                                Xar* emissa);

#endif /* MATERIA_DIAGNOSTICA_H */

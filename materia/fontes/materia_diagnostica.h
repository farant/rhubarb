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

/* Notae substrati, et solae quas substratum dat. VERBA FINITA sunt,
 * numquam participia: participium cum subiecto convenit et subiectum
 * per genera variat (grex masculinum, redirectio femininum), ergo
 * participium unum fixum plerumque falsum esset ubi ostenditur.
 * Verbum cum nullo convenit. */
#define MATERIA_NOTA_COEPIT       "hic coepit"
#define MATERIA_NOTA_EXSPECTATUR  "hic exspectatur"

/* Sedes cognata: locus alter quem diagnosticum nominat, cum nota sua.
 * Forma eadem ac LSP 'DiagnosticRelatedInformation' et notarum clang
 * - locus et sententia, sine munere enumerato: munus notam ordinariam
 * solum eligeret et in transitu abiceretur. */
nomen structura {
        MateriaTractus  tractus;
    constans character* nota;      /* NIHIL licet */
} MateriaSedesRelata;

/* Sedes PRIMARIA campus planus manet: caput lineae unam sedem poscit,
 * et clavis ordinis (tractus.initium, codex) eam recta legit.
 * 'relata' NIHIL = sedes una, ut ante. */
nomen structura {
                            s32  gravitas;   /* MateriaGravitas */
             constans character* codex;      /* "grex/tok_clausura" */
             constans character* causa;
          constans MateriaNodus* nodus;      /* NIHIL: ordo ruptus */
          constans MateriaToken* lexema;     /* ordo ruptus solum */
                 MateriaTractus  tractus;    /* -I: nulla sedes */
             constans character* nota;       /* NIHIL licet */
    constans MateriaSedesRelata* relata;
                            i32  numerus_relatorum;
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

/* materia_diagnosticum.h - Diagnosticum ipsum: forma, non machina
 *
 * Typi et codices quos diagnosticum FERT (sedes, gravitas, codex,
 * causa, sedes relatae), sine ambulatore qui ea derivat
 * (materia_diagnostica.h). Separata a silva-migratio T15: parsator
 * (silva_frons, mortes GLR) diagnostica EMITTIT et formam solam
 * poscit; inclusio machinae per caput eius corpus quoque in clausuram
 * aedilis trahebat (materia_diagnostica.c, excusatio, exemplaria -
 * machina lintris tota in amalgama silvae). Caput SINE corpore, ut
 * materia_registrum.h.
 */
#ifndef MATERIA_DIAGNOSTICUM_H
#define MATERIA_DIAGNOSTICUM_H

#include "latina.h"
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

#endif /* MATERIA_DIAGNOSTICUM_H */

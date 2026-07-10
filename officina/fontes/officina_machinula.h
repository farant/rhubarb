/* officina_machinula.h - Machinula: interpres medullae (M2b)
 *
 * DECISUS (officina-m2-interview.md):
 * - LATITUDO (Q4): canonicum-in-scriptura - omnis operatio verbum
 *   PLENUM 64-bit scribit, extensum per signum suffixi (movere.s32
 *   signum extendit, legere.i8 zephyrum); f64 = figura bitorum;
 *   f32 = figura 32-bit zephyrum-extensa. Lectiones registrorum
 *   verbum UT EST sumunt (productori creditur); immediata in
 *   lectione canonicantur. Pons variadicus verba plana legit.
 * - Dispensatio commutatione (C89 -pedantic goto computatum vetat;
 *   tectum ad M5).
 * - RECORDATOR VOLATUS SEMPER ACTIVUS (Q16): anulus figurarum
 *   (functio/bloccus/instructio); relatio halitus in quovis exitu
 *   abnormali: positio + registra + stiva vocationum + cauda anuli,
 *   cum via:linea per lineas distillatas, ad stderr.
 * - Probatio stivae explicita in vocare (Q10): fractura = halitus
 *   honestus.
 * - Aedificata: tabula statica interna (Q11; series publica ad M4);
 *   OMNES vocationes per CANALEM UNUM (_aedificatum_vocare - sedes
 *   memoriae/reddendi v2 nominata, Q12). Ligatio: decipulae notae
 *   in AEDIFICATUM rescribuntur (signum solum, in situ).
 * - exit = vexillum halitus (sine longjmp); redde principalis =
 *   status processus.
 * - sistere exsecutum = halitus cum causa + positione (+relatio).
 *
 * Numeratores (diagnostica #5): per-operationem, vocationes,
 * aedificata, apex stivae - statio tabularum saltus his decernitur.
 */

#ifndef OFFICINA_MACHINULA_H
#define OFFICINA_MACHINULA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "officina_medulla.h"
#include "officina_regio.h"
#include "officina_conexio.h"

nomen enumeratio {
    MACHINULA_BENE = 0,       /* redde ex initio aut exit vocatum */
    MACHINULA_SISTERE,        /* sistere exsecutum (causa) */
    MACHINULA_DECIPULA,       /* symbolum insolutum vocatum */
    MACHINULA_VITIUM          /* stiva/memoria/vocatio insana */
} MachinulaExitusGenus;

nomen structura {
    s32    genus;             /* MachinulaExitusGenus */
    s64    codex;             /* valor redditus / codex exitus */
    chorda nuntius;           /* causa/titulus/vitium (vacua BENE) */
} MachinulaExitus;

nomen structura Machinula Machinula;

/* ligat aedificata (decipulae notae -> AEDIFICATUM in situ) et
 * cellas (ansae/errno) capit; NIHIL si quid deest */
Machinula* machinula_creare (Piscina* piscina, Conexio* conexio,
    Regio* regio);

/* lineae distillatae pro relatione halitus (NIHIL licet - positio
 * tunc sine via:linea) */
vacuum machinula_lineas_praebere (Machinula* machinula,
    s32 modulus_index, constans MedullaLineae* lineae);

/* functionem nominatam currit (functio sine parametris aut
 * parametra zephyro implentur); status stivae/halitus purgatur -
 * currere iterabile, globalia region PERSISTUNT */
MachinulaExitus machinula_currere (Machinula* machinula,
    chorda titulus_functionis);

/* census */
i64 machinula_numerus_instructionum (constans Machinula* machinula);
i64 machinula_numerus_op (constans Machinula* machinula, s32 op);
i64 machinula_numerus_vocationum (constans Machinula* machinula);
i64 machinula_numerus_aedificatorum (constans Machinula* machinula);
memoriae_index machinula_stiva_apex (constans Machinula* machinula);

#endif /* OFFICINA_MACHINULA_H */

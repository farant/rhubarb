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
 *   (functio/index planus); relatio halitus in quovis exitu
 *   abnormali: positio + stiva vocationum + cauda anuli, cum
 *   via:linea per lineas distillatas, ad stderr (registra NON in
 *   relatione - inspectio per vindicem, M3; auditum milliarii
 *   commentum mendax correxit).
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

#include <stdio.h>   /* FILE (ansam_ponere - sutura capturae M4b) */

nomen enumeratio {
    MACHINULA_BENE = 0,       /* redde ex initio aut exit vocatum */
    MACHINULA_SISTERE,        /* sistere exsecutum (causa) */
    MACHINULA_DECIPULA,       /* symbolum insolutum vocatum */
    MACHINULA_VITIUM,         /* stiva/memoria/vocatio insana */
    MACHINULA_PAUSA,          /* punctum tactum - resumabile
                               * (pergere; positio intacta, M3) */
    MACHINULA_RECUSATIO       /* aedilis in modo sessionis recusatus
                               * (M4b - politica, non defectus) */
} MachinulaExitusGenus;

/* vexilla recusationum (M4b sessio: determinismus replicationis).
 * custos: officina.h amalgami eadem valoribus arabicis definit */
#ifndef MACHINULA_RECUSARE_SCRIPTURAS
#define MACHINULA_RECUSARE_SCRIPTURAS  I    /* fopen w/a, remove... */
#define MACHINULA_RECUSARE_TEMPUS      II   /* time/clock/gettimeofday */
#define MACHINULA_RECUSARE_INITUM      IV   /* lectiones stdin */
#endif

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

/* M4b sessio: flumina norma redirigere (ansa 0/1/2; NIHIL =
 * flumen hospitis ordinarium restitutum). Sutura capturae actorum. */
vacuum machinula_ansam_ponere (Machinula* machinula, s32 ansa,
    FILE* plagula);

/* M4b sessio: vexilla recusationum ponere (MACHINULA_RECUSARE_*;
 * 0 = omnia licita). Aedilis recusatus = halitus RECUSATIO. */
vacuum machinula_recusationes_ponere (Machinula* machinula,
    i32 vexilla);

/* M4b: ansae 3+ (plagulae fopen programmatis) claudere - hygiene
 * demolitionis generationis (C6; fd non effluunt) */
vacuum machinula_ansas_claudere (Machinula* machinula);

/* functionem nominatam currit (functio sine parametris aut
 * parametra zephyro implentur); status stivae/halitus purgatur -
 * currere iterabile, globalia region PERSISTUNT */
MachinulaExitus machinula_currere (Machinula* machinula,
    chorda titulus_functionis);

/* M3 chunk 4: gressus + puncta + inspectio (superficies vindicis).
 * aperire = status purgatus + tabulatum primum (VERUM si paratum);
 * gradus = instructio UNA (VERUM = pergendum); pergere = usque ad
 * halitum aut pausam, genus halitus redditur; resumptio ex pausa
 * punctum sub cursore transit (restitue-grade-repone). currere =
 * involucrum compatibile (aperire + pergere + relatio + exitus). */
b32 machinula_aperire (Machinula* machinula,
    chorda titulus_functionis);
b32 machinula_gradus (Machinula* machinula);
s32 machinula_pergere (Machinula* machinula);

/* puncta: patching exemplaris CONGELATI (proprium machinulae - IR
 * intactum; nullum pretium in via calida). Sedes = (index functionis
 * conexionis, index instructionis planus) - indicium easdem
 * coordinatas dat. */
b32 machinula_punctum_ponere (Machinula* machinula,
    s32 functio_index, i32 instructio);
b32 machinula_punctum_tollere (Machinula* machinula,
    s32 functio_index, i32 instructio);

/* inspectio (VM pausata): tabulatum_index 0 = imum,
 * numerus - I = cacumen */
s64 machinula_halitus_codex (constans Machinula* machinula);
i32 machinula_tabulata_numerus (constans Machinula* machinula);
b32 machinula_positionem_inspicere (constans Machinula* machinula,
    i32 tabulatum_index, s32* functio_index_out,
    i32* instructio_out);
b32 machinula_registrum_legere (constans Machinula* machinula,
    i32 tabulatum_index, i32 index_registri, i64* valor_out);
/* anulus volatus: retro_index 0 = novissima figura, crescens =
 * vetustior; FALSUM ultra caudam (tabula anuli vindicis) */
b32 machinula_anulum_inspicere (constans Machinula* machinula,
    i32 retro_index, s32* functio_index_out, i32* instructio_out);

/* census */
i64 machinula_numerus_instructionum (constans Machinula* machinula);
i64 machinula_numerus_op (constans Machinula* machinula, s32 op);
/* numerus instructionum in exemplari CONGELATO functionis (index =
 * index functionis conexionis; 0 si extra fines). Probatio figens
 * pacti ambulationis canonicae (M3): numerus indicii ex ambulatione
 * collectionis == hic ex ambulatione congelationis. */
i32 machinula_numerus_instructionum_planarum (
    constans Machinula* machinula, s32 functio_index);
i64 machinula_numerus_vocationum (constans Machinula* machinula);
i64 machinula_numerus_aedificatorum (constans Machinula* machinula);
memoriae_index machinula_stiva_apex (constans Machinula* machinula);

#endif /* OFFICINA_MACHINULA_H */

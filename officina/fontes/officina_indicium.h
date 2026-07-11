/* officina_indicium.h - INDICIUM: forma debug plana (M3)
 *
 * Plagula plana sectionum typatarum, exemplo RDI: caput parvum +
 * directorium sectionum genere-indexatum + acies planae; omnes
 * references trans sectiones = indices i32 (numquam monstratores in
 * disco). Lector totam plagulam in unum blocum legit et INDEXAT -
 * nihil convertendum, nihil reficiendum (parse-on-demand).
 *
 * Provenientia MEMORIA-SOLA est (origo = SilvaNodus*, catena
 * tokenorum in piscina silvae) - indicium tempore compilationis
 * coquendum, dum parsura vivit; forma textus medullae nihil fert et
 * reficere non potest. Scriptor per-TU colligit (fenestra
 * collige-ante-destrue, ut demissio_lineas_colligere), scribere post
 * nexum iungit: functio_idx indicii == index functionis CONEXIONIS
 * (== index planae machinulae) - nulla translatio ad tempus cursus.
 *
 * Tabula linearum PRORSUM (per functionem): acies parallelae -
 * indices instructionum ordinati (+ introitus claudens) et res
 * {via, linea, profunditas, nomen_macro}. Intervallum
 * [idx[i], idx[i+1]) -> res[i]. SEDES MACRONUM = introitus
 * consecutivi eodem indice instructionis, RADIX PRIMA (profunditas
 * 0 = invocatio in plagula usoris), profundiores = corpora
 * macronum (exemplar inline-site RDI; acies expansionis sine fine
 * fixo). Introitus emittitur solum ubi acies mutatur (intervalla);
 * instructiones sine origine intervallum praecedens haereditant.
 *
 * Tabula RETRO (per viam): lineae RADICIS ordinatae -> intervalla
 * -> situs {functio, instructio} (instructio prima intervalli
 * lineae = primitivum puncti sistendi).
 *
 * VARIABILIA (per functionem): {titulus fontis, registrum, tag
 * medullae, typus scriptus}. Fons = registra nominata × symbola
 * semanticae (probatio ancestriae per pater); umbrae (x_37) sine
 * typo = lacuna honesta (DECISUS interrogationis M3); typi RESOLUTI
 * monstrantur ("unsigned int", non "i32" - typedef in C89 alias
 * purum est).
 *
 * Endian HOSPITIS, catena instrumentorum haec sola - documentatum,
 * non negotiabile (rota RDI eadem). Versio dure reiecta.
 */

#ifndef OFFICINA_INDICIUM_H
#define OFFICINA_INDICIUM_H

#include "officina_medulla.h"
#include "officina_conexio.h"
#include "silva.h"

#define INDICIUM_MAGICA          "INDICIUM"   /* VIII octeti exacti */
#define INDICIUM_VERSIO          1
#define INDICIUM_TYPUS_IGNOTUS   (-2)   /* typus_medulla ignotus
                                         * (MEDULLA_TYPUS_NIHIL = -1
                                         * vacuum significat) */

/* ==================================================
 * Sectiones. ORDO SIGILLATUS: acies magnitudinum elementorum in
 * officina_indicium.c et enum vanilla in amalgama/officina.h eundem
 * ordinem EXACTE sequuntur (exemplar domus: superficies manu
 * speculata, deriva per asserta probationis + portas capta).
 * ================================================== */

nomen enumeratio {
    INDICIUM_SECTIO_CHORDAE_DATA = 0,  /* i8 blob */
    INDICIUM_SECTIO_CHORDAE_OFFSETA,   /* i32; N+1 */
    INDICIUM_SECTIO_VIAE,              /* IndiciumVia */
    INDICIUM_SECTIO_MODULI,            /* IndiciumModulus */
    INDICIUM_SECTIO_FUNCTIONES,        /* IndiciumFunctio */
    INDICIUM_SECTIO_LINEAE_INDICES,    /* i32 */
    INDICIUM_SECTIO_LINEAE_RES,        /* IndiciumLinea */
    INDICIUM_SECTIO_RETRO_LINEAE,      /* i32 */
    INDICIUM_SECTIO_RETRO_INTERVALLA,  /* i32 */
    INDICIUM_SECTIO_RETRO_SITUS,       /* IndiciumSitus */
    INDICIUM_SECTIO_VARIABILIA,        /* IndiciumVariabile */
    INDICIUM_SECTIO_NUMERUS
} IndiciumSectioGenus;

/* ==================================================
 * Res in disco (structurae fixae, i32/s32 solum)
 * ================================================== */

/* Caput plagulae + directorium (genus sectionis EST index) */
nomen structura {
    i8  magica[8];
    i32 versio;
    i32 numerus_sectionum;         /* INDICIUM_SECTIO_NUMERUS */
} IndiciumCaput;

nomen structura {
    i32 offset;                    /* ab initio plagulae; 8-ordinatum */
    i32 magnitudo_octetorum;
} IndiciumSectio;

/* Chordae: datum blob + offseta (N+1; chorda i =
 * [off[i], off[i+1]) - LONGITUDO delta est, NON terminata nullo:
 * chorda in disco). Index 0 = chorda nihili (vacua). */

nomen structura {
    i32 titulus;                   /* chorda viae (campus 'chorda'
                                    * vetitus: renominatio typorum
                                    * amalgamatoris per signa) */
    i32 retro_lineae_primus;       /* in RETRO_LINEAE */
    i32 retro_intervalla_primus;   /* in RETRO_INTERVALLA
                                    * (numerus + I introitus) */
    i32 retro_numerus;             /* lineae distinctae */
} IndiciumVia;

nomen structura {
    i32 titulus;                   /* chorda */
} IndiciumModulus;

nomen structura {
    i32 titulus;                   /* chorda */
    i32 modulus;                   /* index moduli conexionis */
    i32 instructiones_numerus;     /* ambulatio canonica (PACTUM
                                    * officina_medulla.h) */
    i32 lineae_indices_primus;     /* in LINEAE_INDICES; cursus
                                    * habet lineae_numerus + I
                                    * introitus (ultimus claudens) */
    i32 lineae_res_primus;         /* in LINEAE_RES */
    i32 lineae_numerus;
    i32 variabilia_prima;          /* in VARIABILIA */
    i32 variabilia_numerus;
} IndiciumFunctio;

nomen structura {
    i32 via;                       /* index in VIAE */
    i32 linea;                     /* 1-basata */
    i32 profunditas;               /* 0 = radix (invocatio) */
    i32 nomen_macro;               /* chorda; 0 = nulla (brachium
                                    * proprium tokeni - FONS = 0) */
} IndiciumLinea;

nomen structura {
    i32 functio;                   /* index functionis (conexio) */
    i32 instructio;                /* index planus intra functionem */
} IndiciumSitus;

nomen structura {
    i32 titulus;                   /* chorda: nomen fontis */
    i32 index_registri;            /* registrum medullae */
    s32 typus_medulla;             /* MedullaTypus aut
                                    * INDICIUM_TYPUS_IGNOTUS */
    i32 typus_scriptus;            /* chorda: typus C redditus;
                                    * 0 = ignotus */
} IndiciumVariabile;

/* ==================================================
 * Scriptor (coquere tempore compilationis)
 * ================================================== */

nomen structura IndiciumScriptor IndiciumScriptor;

IndiciumScriptor* indicium_scriptor_creare (Piscina* piscina);

/* Colligere unum modulum DUM parsura vivit (fenestra
 * collige-ante-destrue - eadem ac demissio_lineas_colligere).
 * sem pro mensa variabilium (typi); NIHIL licet (variabilia sine
 * typis). */
b32 indicium_modulum_colligere (IndiciumScriptor* scriptor,
    constans MedullaModulus* modulus,
    constans SilvaParsura* parsura, SilvaSemantica* sem);

/* Scribere plagulam pro mundo NEXO: ordo functionum = ordo
 * conexionis (iunctura per monstratores functionum). Functiones
 * non collectae (moduli textuales) introitus vacuos accipiunt.
 * Deterministicum: idem mundus -> eadem octeti. */
b32 indicium_scribere (IndiciumScriptor* scriptor,
    constans Conexio* conexio, constans character* via);

/* ==================================================
 * Lector (plagula -> quaestiones; nulla conversio)
 * ================================================== */

nomen structura IndiciumLector IndiciumLector;

/* Legit totam plagulam in blocum piscinae; magica/versio/fines
 * sectionum validat (versio aliena dure reiecta). NIHIL si
 * invalida. */
IndiciumLector* indicium_aperire (Piscina* piscina,
    constans character* via);

chorda indicium_chorda (constans IndiciumLector* lector, i32 index);
/* via (IndiciumLinea.via = index in VIAE, non chorda) -> titulus */
chorda indicium_via_chorda (constans IndiciumLector* lector,
    i32 via_index);
i32 indicium_functiones_numerus (constans IndiciumLector* lector);
constans IndiciumFunctio* indicium_functio (
    constans IndiciumLector* lector, i32 index);
/* -I si non inventa (nomine) */
s32 indicium_functionem_quaerere (constans IndiciumLector* lector,
    chorda titulus);

/* Acies linearum pro instructione: intervallum continens indicem
 * quaesitum; *acies_out = introitus primus (radix prima), redditur
 * numerus introituum aciei (0 = ante primum intervallum / functio
 * sine lineis). */
i32 indicium_lineas_de_instructione (constans IndiciumLector* lector,
    i32 functio_index, i32 instructio,
    constans IndiciumLinea** acies_out);

/* Situs pro (via, linea RADICIS) - congruentia lineae exacta;
 * *acies_out = situs primus, redditur numerus. Primitivum puncti:
 * situs[k].instructio = instructio prima intervalli. */
i32 indicium_situs_de_linea (constans IndiciumLector* lector,
    chorda via, i32 linea, constans IndiciumSitus** acies_out);

i32 indicium_variabilia_functionis (constans IndiciumLector* lector,
    i32 functio_index, constans IndiciumVariabile** acies_out);

#endif /* OFFICINA_INDICIUM_H */

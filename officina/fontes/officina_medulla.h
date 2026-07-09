/* officina_medulla.h - Medulla: IR registrorum linearis (M1a B)
 *
 * Vocabularium SIGNATUM: officina/vocabularium-medullae.md (DECISUS
 * 2026-07-09) - nomina operationum, suffixa typorum (.s/.i/.f =
 * conventio domus), sigilla (%/$/@). Consilium: officina-m1-spec.md
 * §II; complicationes simulatio-3 C2 (operanda signata + stiva
 * operandorum pro arietate vocare), C3 (origo SEMPER in memoria -
 * pignus visionis; forma textualis eam optionalem fert), C8
 * (MedullaDatum {imago, relocationes} a die primo).
 *
 * FIGURA: modulus = TU unum (conexio ad M2); functio = blocci
 * nominati (@) + registra virtualia nominata (%) + stiva
 * operandorum; instructiones VALORE in bloccis (xar segmentatum -
 * monstratores stabiles). Registra tenent verba 64-bit; TYPI IN
 * OPERATIONIBUS, non in registris. Referentiae per INDICES (blocci,
 * registra, symbola, causae) - textus eas nominibus reddit (C).
 *
 * Disciplina terminatorum: omnis bloccus uno terminatore finitur
 * (salire/ramus/redde/sistere) - medulla_emittere eam custodit.
 * Nulla probatio typorum hic: demissio annotationes semanticae
 * oboedit; lamina exsecutionis futura iudicat (sistere = sedes
 * eius exsecutionis).
 */

#ifndef OFFICINA_MEDULLA_H
#define OFFICINA_MEDULLA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"

/* provenientia - silva.h NON requiritur (tag solum; demissio
 * plena silva.h includet) */
structura SilvaNodus;

/* ==================================================
 * Typi operationum (suffixa: conventio domus i/s/f)
 * ================================================== */

nomen enumeratio {
    MEDULLA_TYPUS_I8 = 0,    /* insignatus */
    MEDULLA_TYPUS_I16,
    MEDULLA_TYPUS_I32,
    MEDULLA_TYPUS_I64,
    MEDULLA_TYPUS_S8,        /* signatus */
    MEDULLA_TYPUS_S16,
    MEDULLA_TYPUS_S32,
    MEDULLA_TYPUS_S64,
    MEDULLA_TYPUS_F32,
    MEDULLA_TYPUS_F64,
    MEDULLA_TYPUS_NUMERUS
} MedullaTypus;

#define MEDULLA_TYPUS_NIHIL (-I)   /* operatio sine typo (salire...) */

/* ==================================================
 * Operationes (vocabularium-medullae.md §III - SIGNATUM)
 * ================================================== */

nomen enumeratio {
    /* motus */
    MEDULLA_OP_MOVERE = 0,
    /* arithmetica */
    MEDULLA_OP_ADDERE,
    MEDULLA_OP_SUBTRAHERE,
    MEDULLA_OP_MULTIPLICARE,
    MEDULLA_OP_DIVIDERE,
    MEDULLA_OP_RESIDUUM,          /* integralis solum */
    MEDULLA_OP_NEGARE,
    /* bitalia (integralia; logica classica: vel inclusivum,
     * aut EXCLUSIVUM) */
    MEDULLA_OP_ET,
    MEDULLA_OP_VEL,
    MEDULLA_OP_AUT,
    MEDULLA_OP_COMPLEMENTUM,
    MEDULLA_OP_SINISTRORSUM,
    MEDULLA_OP_DEXTRORSUM,        /* .s arithmeticum, .i logicum */
    /* comparationes (fructus i32: 0 aut 1) */
    MEDULLA_OP_AEQUALIS,
    MEDULLA_OP_INAEQUALIS,
    MEDULLA_OP_MINOR,
    MEDULLA_OP_MINOR_AEQUALIS,
    MEDULLA_OP_MAIOR,
    MEDULLA_OP_MAIOR_AEQUALIS,
    /* conversiones (suffixa duo .FONS.FINIS; extensio signum a
     * FONTE sumit) */
    MEDULLA_OP_EXTENDERE,
    MEDULLA_OP_AMPUTARE,
    MEDULLA_OP_FLUITARE,          /* integer -> fluitans */
    MEDULLA_OP_INTEGRARE,         /* fluitans -> integer */
    MEDULLA_OP_DILATARE,          /* f32 -> f64 */
    MEDULLA_OP_ARTARE,            /* f64 -> f32 */
    /* memoria */
    MEDULLA_OP_LEGERE,            /* onus typatum */
    MEDULLA_OP_SCRIBERE,
    MEDULLA_OP_ARCA,              /* locellus arcae: a=magnitudo,
                                   * b=ordinatio -> inscriptio */
    MEDULLA_OP_LOCUS,             /* inscriptio symboli ($) */
    MEDULLA_OP_COPIA,             /* a=finis, b=fons, c=magnitudo */
    /* fluxus (terminatores praeter vocare) */
    MEDULLA_OP_SALIRE,
    MEDULLA_OP_RAMUS,             /* a=conditio (non-zephyrum ->
                                   * b=@verum), c=@falsum */
    MEDULLA_OP_REDDE,             /* a=valor aut NIHIL (vacuum) */
    MEDULLA_OP_VOCARE,            /* a=vocatus ($ aut %); argumenta
                                   * in stiva (extra_index/numerus) */
    MEDULLA_OP_SISTERE,           /* a=causa (index internatus) -
                                   * diagnosticum + situs; ruit
                                   * SOLUM SI exsecutum */
    MEDULLA_OP_NUMERUS
} MedullaOp;

/* ==================================================
 * Operandum (C2: structura maxime copiata - signatum, 16 octeti)
 * ================================================== */

nomen enumeratio {
    MEDULLA_OPERANDUM_NIHIL = 0,   /* absens */
    MEDULLA_OPERANDUM_REGISTRUM,   /* %titulus (index registri) */
    MEDULLA_OPERANDUM_IMMEDIATUM,  /* litterale integrale */
    MEDULLA_OPERANDUM_IMMEDIATUM_F,/* litterale fluitans */
    MEDULLA_OPERANDUM_SYMBOLUM,    /* $titulus (index symboli) */
    MEDULLA_OPERANDUM_BLOCCUS,     /* @titulus (index blocci) */
    MEDULLA_OPERANDUM_CAUSA        /* causa sistendi (internata) */
} MedullaOperandumGenus;

nomen structura {
    s32 genus;                     /* MedullaOperandumGenus */
    unio {
        s32 index;                 /* registrum/bloccus/symbolum/causa */
        s64 immediatum;
        f64 immediatum_f;
    } datum;
} MedullaOperandum;

/* ==================================================
 * Instructio (origo SEMPER fertur - provenientia est merx)
 * ================================================== */

nomen structura {
    s32 op;                        /* MedullaOp */
    s32 typus;                     /* suffixum primum; MEDULLA_TYPUS_
                                    * NIHIL si operatio sine typo */
    s32 typus_secundus;            /* suffixum alterum (conversiones:
                                    * typus=FONS, typus_secundus=
                                    * FINIS); NIHIL aliter */
    s32 destinatio;                /* index registri; -I = nulla */
    s32 extra_index;               /* vocare: initium argumentorum
                                    * in stiva operandorum; -I */
    s32 extra_numerus;
    MedullaOperandum a;
    MedullaOperandum b;
    MedullaOperandum c;
    constans structura SilvaNodus* origo;   /* NIHIL licet (textus
                                             * manu scriptus) */
} MedullaInstructio;

/* ==================================================
 * Bloccus / functio / datum / modulus
 * ================================================== */

nomen structura {
    chorda titulus;                /* @titulus (sine sigillo) */
    Xar*   instructiones;          /* MedullaInstructio valore */
    b32    terminatus;
} MedullaBloccus;

nomen structura {
    s32 index;                     /* registri */
    s32 typus;                     /* MedullaTypus */
} MedullaParametrum;

nomen structura {
    chorda titulus;                /* $titulus (sine sigillo) */
    s32    typus_reditus;          /* MEDULLA_TYPUS_NIHIL = vacuum */
    b32    est_variadica;
    Xar*   parametra;              /* MedullaParametrum valore */
    Xar*   blocci;                 /* MedullaBloccus valore */
    Xar*   registra;               /* chorda (tituli) valore -
                                    * mensura 0 numquam (tN auto) */
    Xar*   operanda;               /* stiva: MedullaOperandum valore
                                    * (argumenta vocationum) */
    constans structura SilvaNodus* origo;
} MedullaFunctio;

nomen enumeratio {
    MEDULLA_SYMBOLUM_EXTERNUM = 0, /* declaratum, non definitum
                                    * (aedificata machinulae aut
                                    * conexio M2) */
    MEDULLA_SYMBOLUM_FUNCTIO,
    MEDULLA_SYMBOLUM_DATUM
} MedullaSymbolumGenus;

nomen structura {
    chorda titulus;
    s32    genus;                  /* MedullaSymbolumGenus */
    s32    index;                  /* in functiones/data; -I externum */
} MedullaSymbolum;

nomen structura {
    i32 offset;                    /* in imaginem (locellus 8 octeti) */
    s32 symbolum;                  /* index symboli moduli */
} MedullaRelocatio;

nomen structura {
    chorda titulus;
    i32    magnitudo_octetorum;    /* NUMQUAM "magnitudo" - macro! */
    i32    ordinatio;
    i8*    imago;                  /* octeti (zephyrum-pleni initio) */
    Xar*   relocationes;           /* MedullaRelocatio valore */
    constans structura SilvaNodus* origo;
} MedullaDatum;

nomen structura {
    Piscina* piscina;
    chorda   titulus;              /* "lib/chorda.c" */
    Xar*     functiones;           /* MedullaFunctio* */
    Xar*     data;                 /* MedullaDatum* */
    Xar*     symbola;              /* MedullaSymbolum valore */
    Xar*     causae;               /* chorda valore (sistere) */
} MedullaModulus;

/* ==================================================
 * Fabrica (interfacies prima - Eskil)
 * ================================================== */

MedullaModulus* medulla_modulum_creare (Piscina* piscina,
    chorda titulus);

/* Internare per titulum: idem titulus -> idem index. Novum =
 * EXTERNUM; functionem/datum_creare genus concretum ponit. */
s32 medulla_symbolum_internare (MedullaModulus* modulus,
    chorda titulus);
constans MedullaSymbolum* medulla_symbolum_obtinere (
    constans MedullaModulus* modulus, s32 index);

s32 medulla_causam_internare (MedullaModulus* modulus, chorda causa);
constans chorda* medulla_causam_obtinere (
    constans MedullaModulus* modulus, s32 index);

/* NIHIL si titulus iam definitionem concretam habet */
MedullaFunctio* medulla_functionem_creare (MedullaModulus* modulus,
    chorda titulus, s32 typus_reditus, b32 est_variadica);

/* -> index registri novi (parametra = registra prima) */
s32 medulla_parametrum_addere (MedullaFunctio* functio,
    chorda titulus, s32 typus);
/* titulus mensura 0 -> "tN" automaticum */
s32 medulla_registrum_novum (MedullaFunctio* functio, chorda titulus);
constans chorda* medulla_registrum_titulus (
    constans MedullaFunctio* functio, s32 index);

s32 medulla_bloccum_creare (MedullaFunctio* functio, chorda titulus);
MedullaBloccus* medulla_bloccum_obtinere (
    constans MedullaFunctio* functio, s32 index);

/* argumenta vocationis in stivam -> index initii (extra_index) */
s32 medulla_operanda_addere (MedullaFunctio* functio,
    constans MedullaOperandum* operanda, s32 numerus);

/* fabricae operandorum (valore) */
MedullaOperandum medulla_op_nihil (vacuum);
MedullaOperandum medulla_op_registrum (s32 index);
MedullaOperandum medulla_op_immediatum (s64 valor);
MedullaOperandum medulla_op_immediatum_f (f64 valor);
MedullaOperandum medulla_op_symbolum (s32 index);
MedullaOperandum medulla_op_bloccum (s32 index);
MedullaOperandum medulla_op_causa (s32 index);

/* Custos disciplinae: FALSUM si bloccus invalidus/terminatus, op
 * extra fines, aut destinatio registrum ignotum. Terminator
 * bloccum claudit. Instructio VALORE copiatur. */
b32 medulla_emittere (MedullaFunctio* functio, s32 bloccus,
    constans MedullaInstructio* instructio);

b32 medulla_op_terminator (s32 op);
/* omnes blocci terminati et saltem unus */
b32 medulla_functio_terminata (constans MedullaFunctio* functio);

/* NIHIL si titulus iam definitionem concretam habet aut
 * magnitudo_octetorum < I. Imago allocatur et zephyrum-pletur. */
MedullaDatum* medulla_datum_creare (MedullaModulus* modulus,
    chorda titulus, i32 magnitudo_octetorum, i32 ordinatio);
b32 medulla_datum_scribere (MedullaDatum* datum, i32 offset,
    constans i8* octeti, i32 numerus);
/* locellus relocationis = 8 octeti intra imaginem */
b32 medulla_relocationem_addere (MedullaDatum* datum, i32 offset,
    s32 symbolum);

#endif /* OFFICINA_MEDULLA_H */

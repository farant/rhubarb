/* silva_c89_semantica.h - Semantica C89: typi + forma + aestimator
 * (M0a Chunk A; consilium = project-specs/silva-semantica-design.md,
 * DECISUS 2026-07-08)
 *
 * Typi ut TABULAE PARALLELAE super arbores commissas immutabiles
 * (exemplar responsorum oraculi). Ansa typi = monstrator UNUS
 * (TypusC89*): typi derivati (monstrator/acies/functio/qualificatus)
 * INTERNANTUR - aequalitas monstratorum EST identitas; typi tag
 * (structura/unio/enumeratus) NOMINALES - identitas per sedem
 * declarationis (C89). Qualificati = involucra internata (unus
 * monstrator ubique, non par {typus, vexilla}).
 *
 * FORMA pigra (magnitudo/ordinatio/offsets) - arm64 LP64 Apple:
 * long double = 8 (duplex!), long long = 8 (EXTENSIO recepta
 * consulto: corpus s64 = long long, -Wno-long-long). Aestimator
 * (expressiones constantes integrae) et forma MUTUO recursivi per
 * contextum communem (magnitudo-typi in mensuris aciei).
 *
 * CHUNK A: tabulae nominum PLANAE (typedef/tag/constantes -
 * scopus Chunk B eas gradibus substituet; superficies functionum =
 * contractus, non numerus TU). Diagnostica venenata: TYPUS_ERROR
 * absorbet sine cascata, analysis semper completur (regula
 * tree-sitter ad semanticam extensa).
 *
 * PARCA NOMINATA (consilium §IX/XI + INTENTIO): campi (bitfields) -
 * forma tag venenatur, membrum typatur; magnitudo-expressionis in
 * aestimatore (typum expressionis postulat = M0b); initiatores non
 * probantur (M0b); K&R; vide phase-log.
 */

#ifndef SILVA_C89_SEMANTICA_H
#define SILVA_C89_SEMANTICA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "tabula_dispersa.h"
#include "silva_token.h"
#include "silva_nodus.h"

/* ==================================================
 * Genera typorum
 * ================================================== */

nomen enumeratio {
    TYPUS_C89_PRIMITIVUS = 0,
    TYPUS_C89_MONSTRATOR,     /* T* */
    TYPUS_C89_ACIES,          /* T[n] */
    TYPUS_C89_FUNCTIO,        /* T (params) */
    TYPUS_C89_STRUCTURA,
    TYPUS_C89_UNIO,
    TYPUS_C89_ENUMERATUS,
    TYPUS_C89_QUALIFICATUS,   /* const/volatile involucrum */
    TYPUS_C89_ERROR           /* venenum - absorbet sine cascata */
} TypusC89Genus;

/* Primitivi canonici (multisets multi -> canonicus unus;
 * char / signed char / unsigned char = TRES typi distincti) */
nomen enumeratio {
    PRIMITIVUM_VACUUM = 0,
    PRIMITIVUM_CHARACTER,
    PRIMITIVUM_CHARACTER_SIGNATUM,
    PRIMITIVUM_CHARACTER_INSIGNATUM,
    PRIMITIVUM_BREVIS,
    PRIMITIVUM_BREVIS_INSIGNATUM,
    PRIMITIVUM_INTEGER,
    PRIMITIVUM_INTEGER_INSIGNATUM,
    PRIMITIVUM_LONGUS,
    PRIMITIVUM_LONGUS_INSIGNATUM,
    PRIMITIVUM_LONGUS_LONGUS,             /* extensio (s64) */
    PRIMITIVUM_LONGUS_LONGUS_INSIGNATUM,  /* extensio */
    PRIMITIVUM_FLUITANS,
    PRIMITIVUM_DUPLEX,
    PRIMITIVUM_DUPLEX_LONGUS,             /* = 8 in Apple arm64! */
    PRIMITIVUM_NUMERUS
} TypusC89Primitivum;

#define QUALIS_CONSTANS   1
#define QUALIS_VOLATILIS  2

/* ==================================================
 * Typus
 * ================================================== */

nomen structura TypusC89 TypusC89;

nomen structura {
    chorda    titulus;      /* copia; mensura 0 = anonymum */
    TypusC89* typus;
    i32       offset;       /* octeti; a forma computatus */
    b32       est_campus;   /* bitfield (forma tag parcata) */
} TypusC89Membrum;

structura TypusC89 {
    s32 genus;              /* TypusC89Genus */

    /* forma pigra */
    b32 forma_computata;
    b32 in_computatione;    /* custos cycli (S invalida recursiva) */
    i32 magnitudo_octetorum;
    i32 ordinatio;          /* alignment */

    unio {
        s32 primitivum;     /* TypusC89Primitivum */
        structura {
            TypusC89* internum;
        } monstrator;
        structura {
            TypusC89* elementum;
            s32       numerus;    /* -1 = incompleta [] */
        } acies;
        structura {
            TypusC89*  reditus;
            TypusC89** parametra;             /* series in piscina */
            i32        numerus_parametrorum;
            b32        est_variadica;         /* ... */
            b32        est_prototypata;       /* FALSUM = () */
        } functio;
        structura {
            chorda               titulus;     /* mensura 0 = anonyma */
            b32                  completa;
            TypusC89Membrum*     membra;      /* series in piscina */
            i32                  numerus_membrorum;
            constans SilvaNodus* declarans;   /* sedes identitatis */
            b32                  habet_campos; /* bitfields (parca) */
        } tag;              /* STRUCTURA + UNIO */
        structura {
            chorda               titulus;
            b32                  completa;
            constans SilvaNodus* declarans;
        } enumeratus;
        structura {
            TypusC89* internum;
            i32       quales;    /* QUALIS_* vexilla */
        } qualificatus;
    } datum;
};

/* ==================================================
 * Semantica (contextus communis typus/forma/aestimator)
 * ================================================== */

nomen structura {
    constans SilvaNodus* nodus;   /* NIHIL licet */
    constans character*  causa;   /* litterae staticae */
} SemanticaDiagnosticum;

nomen structura {
    Piscina* piscina;

    /* singuli */
    TypusC89* primitivi[PRIMITIVUM_NUMERUS];
    TypusC89* typus_erroris;

    /* internamentum derivatorum (series linearis - complete
     * primum, optima post; percursus mensurabit) */
    Xar* derivati;              /* TypusC89* */

    /* tabulae nominum PLANAE (Chunk A; scopus B substituet) */
    TabulaDispersa* nomina_typorum;   /* typedef titulus -> TypusC89* */
    TabulaDispersa* tags;             /* tag titulus -> TypusC89* */
    TabulaDispersa* constantes;       /* enum const titulus -> s64* */

    Xar* diagnostica;           /* SemanticaDiagnosticum (valore) */
} SilvaSemantica;

SilvaSemantica* silva_c89_semantica_creare (Piscina* piscina);

vacuum silva_c89_diagnosticum_addere (SilvaSemantica* sem,
    constans SilvaNodus* nodus, constans character* causa);

/* ==================================================
 * Constructio typorum (fabricae internantes)
 * ================================================== */

TypusC89* silva_c89_typus_primitivum (SilvaSemantica* sem,
    s32 primitivum);
TypusC89* silva_c89_typus_monstrator (SilvaSemantica* sem,
    TypusC89* internum);
TypusC89* silva_c89_typus_acies (SilvaSemantica* sem,
    TypusC89* elementum, s32 numerus);
TypusC89* silva_c89_typus_qualificatus (SilvaSemantica* sem,
    TypusC89* internum, i32 quales);
TypusC89* silva_c89_typus_functio (SilvaSemantica* sem,
    TypusC89* reditus, TypusC89** parametra, i32 numerus,
    b32 est_variadica, b32 est_prototypata);

/* ==================================================
 * Constructio ex arbore
 * ================================================== */

/* Specificatores (LISTA_MIXTA) -> typus basis. Qualificatores
 * specificatorum in basim PLICANTUR (const int *p = monstrator ad
 * const-int - recte). TYPEDEF vexillum per est_typedef_out (NIHIL
 * licet). Nullus specificator typi = int implicitum (C89 legale).
 * Ignotum typus-nominatus = venenum + diagnosticum. */
TypusC89* silva_c89_typus_ex_specificatoribus (SilvaSemantica* sem,
    SilvaValor specificatores, b32* est_typedef_out);

/* Catena declaratoris (internum deorsum) super basim - regula C
 * intus-foras. declarator NIHIL aut abstractus = basis ipsa.
 * declarator-initiatus perspicue involvitur (initiator NON
 * probatur - M0b parca). titulus_out (NIHIL licet) = lexema
 * nominis aut NIHIL si abstractus. */
TypusC89* silva_c89_typus_ex_declaratore (SilvaSemantica* sem,
    TypusC89* basis, constans SilvaNodus* declarator,
    SilvaToken** titulus_out);

/* species-typi (conversio/magnitudo-typi) -> typus */
TypusC89* silva_c89_typus_ex_specie (SilvaSemantica* sem,
    constans SilvaNodus* species);

/* Declarationem integram tractare: typedefs registrat, tags
 * definit, constantes enumerorum aestimat - nomina TYPICA sola
 * (symbola variabilium = scopus, Chunk B). Numerum declaratorum
 * typatorum reddit. */
i32 silva_c89_declarationem_tractare (SilvaSemantica* sem,
    constans SilvaNodus* declaratio);

/* Registratio plana (Chunk A) */
b32 silva_c89_typedef_registrare (SilvaSemantica* sem,
    chorda titulus, TypusC89* typus);
TypusC89* silva_c89_typedef_invenire (SilvaSemantica* sem,
    chorda titulus);

/* ==================================================
 * Forma (pigra; arm64 LP64 Apple)
 * ================================================== */

/* VERUM si magnitudo_octetorum/ordinatio (et offsets membrorum)
 * validae post vocationem. FALSUM (+ diagnosticum) pro vacuo,
 * functione, acie incompleta, tag incompleto, cyclo, campis. */
b32 silva_c89_formam_computare (SilvaSemantica* sem,
    TypusC89* typus);

/* ==================================================
 * Aestimator (expressiones constantes integrae C89)
 * ================================================== */

/* folium-integer/-character, identificator (constantes enumerorum),
 * unarium, binarium, ternarius, parenthesis, conversio (ad typos
 * integros), magnitudo-typi (per formam - recursio mutua consulto).
 * magnitudo-expressionis = parca M0b (FALSUM + diagnosticum). */
b32 silva_c89_constans_aestimare (SilvaSemantica* sem,
    constans SilvaNodus* expressio, s64* valor_out);

#endif /* SILVA_C89_SEMANTICA_H */

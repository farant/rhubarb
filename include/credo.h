/* credo.h - probationes: assertiones CREDO_* (unit tests) */
#ifndef CREDO_H
#define CREDO_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"


/* ============================================
 * Creatio Recodationis
 * ============================================ */

nomen structura CredoNotatio {
                                            chorda genus;
                                            chorda expressio;
                                            chorda valor_primus;
                                            chorda valor_secundus;
                                            chorda filum;
                                               s32 versus;
                                               b32 praeteritus;
                                    memoriae_index ordo;
    structura CredoNotatio* sequens;
    /* credo II: CREDO_GRADUS currens (vacua si nullus) */
                                            chorda gradus;
} CredoNotatio;


/* ============================================
 * Globalis Apparatus
 * ============================================ */

vacuum
credo_aperire (
        Piscina* piscina);

vacuum
credo_claudere (
        vacuum);


/* ============================================
 * Interrogatio
 * ============================================ */

memoriae_index
credo_numerus_totalis (
        vacuum);

memoriae_index
credo_numerus_praeteriti (
        vacuum);

memoriae_index
credo_numerus_fracti (
        vacuum);

/* VERUM si assertio ulla cucurrit ET nulla fracta est. Totalis
 * ZEPHYRUM = FALSUM ('NIHIL PROBATUM'): probatio quae nihil asserit
 * non praeteriit (2026-09-02). */
b32
credo_omnia_praeterierunt (
        vacuum);

vacuum
credo_imprimere_compendium (
        vacuum);

CredoNotatio*
credo_invenire_fractos (
        memoriae_index* numerus_fructus);


/* ============================================
 * Macros - Nucleum
 * ============================================ */

/* Culpa declarata: fractura sine expressione, pro exitu praematuro
 * ('corpus non apertum', 'plica absens') qui olim imprimere + redde I
 * erat et compendium 'OMNIA PRAETERIERUNT' supra se relinquebat.
 * Numerat, ergo compendium et exitus consentiunt. */
#define CREDO_CULPA(nuntius) \
    _credo_notare( \
            "credo_culpa", \
            (nuntius), \
            "culpa declarata", \
            "nulla", \
            __FILE__, \
            __LINE__, \
            FALSUM \
    )

#define CREDO_VERUM(x) \
    _credo_notare( \
            "credo_verum", \
            #x, \
            #x, \
            "", \
            __FILE__, \
            __LINE__, \
            (x) != FALSUM \
    )

#define CREDO_FALSUM(x) \
    _credo_notare( \
            "credo_falsum", \
            #x, \
            #x, \
            "", \
            __FILE__, \
            __LINE__, \
            (x) == FALSUM \
    )

#define CREDO_NON_NIHIL(p) \
    _credo_notare( \
            "credo_non_nihil", \
            #p, \
            #p, \
            "", \
            __FILE__, \
            __LINE__, \
            (p) != NIHIL \
    )

#define CREDO_NIHIL(p) \
    _credo_notare( \
            "credo_nihil", \
            #p, \
            #p, \
            "", \
            __FILE__, \
            __LINE__, \
            (p) == NIHIL \
    )

#define CREDO_AEQUALIS_PTR(a, b) \
    _credo_notare( \
            "credo_aequalis_ptr", \
            #a " == " #b, \
            #a, \
            #b, \
            __FILE__, \
            __LINE__, \
            (a) == (b) \
    )

#define CREDO_INAEQUALITAS_PTR(a, b) \
    _credo_notare( \
            "credo_inaequalitas_ptr", \
            #a " != " #b, \
            #a, \
            #b, \
            __FILE__, \
            __LINE__, \
            (a) != (b) \
    )

#define CREDO_AEQUALIS_S32(a, b) \
    _credo_notare_integrum( \
            "credo_aequalis", \
            #a " == " #b, \
            (a), \
            (b), \
            __FILE__, \
            __LINE__ \
    )

#define CREDO_AEQUALIS_I32(a, b) \
    _credo_notare_integrum_i32( \
            "credo_aequalis", \
            #a " == " #b, \
            (a), \
            (b), \
            __FILE__, \
            __LINE__ \
    )

#define CREDO_AEQUALIS_S64(a, b) \
    _credo_notare_integrum_s64( \
            "credo_aequalis", \
            #a " == " #b, \
            (a), \
            (b), \
            __FILE__, \
            __LINE__ \
    )

#define CREDO_INAEQUALITAS_S32(a, b) \
    _credo_notare_integrum_inaequalitas( \
            "credo_inaequalitas", \
            #a " != " #b, \
            (a), \
            (b), \
            __FILE__, \
            __LINE__ \
    )

#define CREDO_INAEQUALITAS_I32(a, b) \
    _credo_notare_integrum_inaequalitas_i32( \
            "credo_inaequalitas", \
            #a " != " #b, \
            (a), \
            (b), \
            __FILE__, \
            __LINE__ \
    )

/* NOMEN FALLIT: litteras C (character*) accipit, non chordas - forma
 * nova CREDO_LITERAE_AEQUALES (NIHIL tuta, valores effugiti). Manet ut
 * suitae veteres currant. */
#define CREDO_CHORDAE_AEQUALES(s1, s2) \
    _credo_notare_chorda( \
            "credo_chordae_aequales", \
            #s1 " == " #s2, \
            (s1), \
            (s2), \
            __FILE__, \
            __LINE__ \
    )


/* ============================================
 * Macros - Chorda Convenientiae
 * ============================================ */

/* credo III (2026-10-19): valores AMBO in fractura citantur (olim per
 * CREDO_VERUM: expressio sola). Receptus = primum argumentum, Speratus
 * = secundum (acus, prefixum, suffixum). Chorda per MENSURAM legitur
 * (NUL intra licet), effugitur, longa per fenestram circa differentiam
 * primam ostenditur; aequalitas lineam 'Differentia:' addit. */
#define CREDO_COMPARATIO_AEQUALIS    0
#define CREDO_COMPARATIO_CONTINET    1
#define CREDO_COMPARATIO_INCIPIT     2
#define CREDO_COMPARATIO_TERMINATUR  3

#define CREDO_CHORDA_AEQUALIS(a, b) \
    _credo_notare_chordae( \
            "credo_chorda_aequalis", \
            #a " == " #b, \
            (a), \
            (b), \
            CREDO_COMPARATIO_AEQUALIS, \
            __FILE__, \
            __LINE__ \
    )

#define CREDO_CHORDA_CONTINET(fenum, acus) \
    _credo_notare_chordae( \
            "credo_chorda_continet", \
            #fenum " continet " #acus, \
            (fenum), \
            (acus), \
            CREDO_COMPARATIO_CONTINET, \
            __FILE__, \
            __LINE__ \
    )

#define CREDO_CHORDA_INCIPIT(s, prefixum) \
    _credo_notare_chordae( \
            "credo_chorda_incipit", \
            #s " incipit " #prefixum, \
            (s), \
            (prefixum), \
            CREDO_COMPARATIO_INCIPIT, \
            __FILE__, \
            __LINE__ \
    )

#define CREDO_CHORDA_TERMINATUR(s, suffixum) \
    _credo_notare_chordae( \
            "credo_chorda_terminatur", \
            #s " terminatur " #suffixum, \
            (s), \
            (suffixum), \
            CREDO_COMPARATIO_TERMINATUR, \
            __FILE__, \
            __LINE__ \
    )

#define CREDO_CHORDA_AEQUALIS_LITERIS(s, literis) \
    _credo_notare_chorda_literis( \
            "credo_chorda_aequalis_literis", \
            #s " == " #literis, \
            (s), \
            (literis), \
            __FILE__, \
            __LINE__ \
    )

#define CREDO_CHORDA_VACUA(s) \
    _credo_notare( \
            "credo_chorda_vacua", \
            #s ".mensura == 0", \
            #s, \
            "", \
            __FILE__, \
            __LINE__, \
            (s).mensura == ZEPHYRUM \
    )

#define CREDO_CHORDA_NON_VACUA(s) \
    _credo_notare( \
            "credo_chorda_non_vacua", \
            #s ".mensura > 0", \
            #s, \
            "", \
            __FILE__, \
            __LINE__, \
            (s).mensura > ZEPHYRUM \
    )


/* ============================================
 * Macros - Comparationes Integrae
 * ============================================ */

#define CREDO_MAIOR_S32(a, b) \
    _credo_notare_integrum_maior( \
            "credo_maior", \
            #a " > " #b, \
            (a), \
            (b), \
            __FILE__, \
            __LINE__ \
    )

#define CREDO_MAIOR_I32(a, b) \
    _credo_notare_integrum_maior_i32( \
            "credo_maior", \
            #a " > " #b, \
            (a), \
            (b), \
            __FILE__, \
            __LINE__ \
    )

#define CREDO_MINOR_S32(a, b) \
    _credo_notare_integrum_minor( \
            "credo_minor", \
            #a " < " #b, \
            (a), \
            (b), \
            __FILE__, \
            __LINE__ \
    )

#define CREDO_MINOR_I32(a, b) \
    _credo_notare_integrum_minor_i32( \
            "credo_minor", \
            #a " < " #b, \
            (a), \
            (b), \
            __FILE__, \
            __LINE__ \
    )

#define CREDO_MAIOR_AUT_AEQUALIS_S32(a, b) \
    _credo_notare_integrum_maior_aut_aequalis( \
            "credo_maior_aut_aequalis", \
            #a " >= " #b, \
            (a), \
            (b), \
            __FILE__, \
            __LINE__ \
    )

#define CREDO_MAIOR_AUT_AEQUALIS_I32(a, b) \
    _credo_notare_integrum_maior_aut_aequalis_i32( \
            "credo_maior_aut_aequalis", \
            #a " >= " #b, \
            (a), \
            (b), \
            __FILE__, \
            __LINE__ \
    )

#define CREDO_MINOR_AUT_AEQUALIS_S32(a, b) \
    _credo_notare_integrum_minor_aut_aequalis( \
            "credo_minor_aut_aequalis", \
            #a " <= " #b, \
            (a), \
            (b), \
            __FILE__, \
            __LINE__ \
    )

#define CREDO_MINOR_AUT_AEQUALIS_I32(a, b) \
    _credo_notare_integrum_minor_aut_aequalis_i32( \
            "credo_minor_aut_aequalis", \
            #a " <= " #b, \
            (a), \
            (b), \
            __FILE__, \
            __LINE__ \
    )


/* ============================================
 * Macros - Comparationes Fluitantes
 * ============================================ */

#define CREDO_F64_PROXIMUS(a, b, epsilon) \
    _credo_notare_f64_proximus( \
            "credo_f64_proximus", \
            #a " ≈ " #b, \
            (a), \
            (b), \
            (epsilon), \
            __FILE__, \
            __LINE__ \
    )


/* ==================================================
 * Macros - credo III (2026-10-19)
 * ================================================== */

/* Litterae C (character*), NIHIL tutae: ambae NIHIL = aequales, una
 * NIHIL = fracta ('NIHIL' citatum). */
#define CREDO_LITERAE_AEQUALES(a, b) \
    _credo_notare_literae( \
            "credo_literae_aequales", \
            #a " == " #b, \
            (a), \
            (b), \
            __FILE__, \
            __LINE__ \
    )

/* n octeti ut memcmp; fractura offset primae differentiae et
 * fenestram hex ('[ff]' = octetus diversus) nominat. */
#define CREDO_OCTETI_AEQUALES(a, b, n) \
    _credo_notare_octeti( \
            "credo_octeti_aequales", \
            #a " == " #b " (" #n " octeti)", \
            (a), \
            (b), \
            (memoriae_index)(n), \
            __FILE__, \
            __LINE__ \
    )

/* Familiae LXIV: S64 cum signo, I64 sine (memoriae_index eiusdem
 * latitudinis sine casti accipitur). */
#define CREDO_RELATIO_AEQUALIS            0
#define CREDO_RELATIO_INAEQUALIS          1
#define CREDO_RELATIO_MAIOR               2
#define CREDO_RELATIO_MINOR               3
#define CREDO_RELATIO_MAIOR_AUT_AEQUALIS  4
#define CREDO_RELATIO_MINOR_AUT_AEQUALIS  5

/* expressio in macro EXTERIORE stringificatur: argumenta macro
 * interiori data iam expansa sunt (V -> 5) */
#define CREDO_S64_INTERNUM(genus, expressio, relatio, a, b) \
    _credo_notare_s64(genus, expressio, (a), (b), relatio, __FILE__, \
        __LINE__)
#define CREDO_I64_INTERNUM(genus, expressio, relatio, a, b) \
    _credo_notare_i64(genus, expressio, (a), (b), relatio, __FILE__, \
        __LINE__)

#define CREDO_INAEQUALITAS_S64(a, b) \
    CREDO_S64_INTERNUM("credo_inaequalitas", #a " != " #b,\
        CREDO_RELATIO_INAEQUALIS, a, b)
#define CREDO_MAIOR_S64(a, b) \
    CREDO_S64_INTERNUM("credo_maior", #a " > " #b,\
        CREDO_RELATIO_MAIOR, a, b)
#define CREDO_MINOR_S64(a, b) \
    CREDO_S64_INTERNUM("credo_minor", #a " < " #b,\
        CREDO_RELATIO_MINOR, a, b)
#define CREDO_MAIOR_AUT_AEQUALIS_S64(a, b) \
    CREDO_S64_INTERNUM("credo_maior_aut_aequalis", #a " >= " #b, \
        CREDO_RELATIO_MAIOR_AUT_AEQUALIS, a, b)
#define CREDO_MINOR_AUT_AEQUALIS_S64(a, b) \
    CREDO_S64_INTERNUM("credo_minor_aut_aequalis", #a " <= " #b, \
        CREDO_RELATIO_MINOR_AUT_AEQUALIS, a, b)

#define CREDO_AEQUALIS_I64(a, b) \
    CREDO_I64_INTERNUM("credo_aequalis", #a " == " #b,\
        CREDO_RELATIO_AEQUALIS, a, b)
#define CREDO_INAEQUALITAS_I64(a, b) \
    CREDO_I64_INTERNUM("credo_inaequalitas", #a " != " #b,\
        CREDO_RELATIO_INAEQUALIS, a, b)
#define CREDO_MAIOR_I64(a, b) \
    CREDO_I64_INTERNUM("credo_maior", #a " > " #b,\
        CREDO_RELATIO_MAIOR, a, b)
#define CREDO_MINOR_I64(a, b) \
    CREDO_I64_INTERNUM("credo_minor", #a " < " #b,\
        CREDO_RELATIO_MINOR, a, b)
#define CREDO_MAIOR_AUT_AEQUALIS_I64(a, b) \
    CREDO_I64_INTERNUM("credo_maior_aut_aequalis", #a " >= " #b, \
        CREDO_RELATIO_MAIOR_AUT_AEQUALIS, a, b)
#define CREDO_MINOR_AUT_AEQUALIS_I64(a, b) \
    CREDO_I64_INTERNUM("credo_minor_aut_aequalis", #a " <= " #b, \
        CREDO_RELATIO_MINOR_AUT_AEQUALIS, a, b)

/* f32: per f64 comparatur (promotio exacta). */
#define CREDO_F32_PROXIMUS(a, b, epsilon) \
    _credo_notare_f64_proximus( \
            "credo_f32_proximus", \
            #a " ≈ " #b, \
            (f64)(a), \
            (f64)(b), \
            (f64)(epsilon), \
            __FILE__, \
            __LINE__ \
    )

/* |a - b| <= r * max(|a|, |b|): epsilon absolutum pro magnitudinibus
 * extremis falsum est (gtest, Catch2 Approx). */
#define CREDO_F64_RELATIVE(a, b, r) \
    _credo_notare_f64_relative( \
            "credo_f64_relative", \
            #a " ≈ " #b, \
            (a), \
            (b), \
            (r), \
            __FILE__, \
            __LINE__ \
    )

/* distantia in numeris repraesentabilibus (ULP) <= n; -0.0 == +0.0,
 * NaN numquam aequalis (gtest: IV ULP ordinarium). */
#define CREDO_F64_ULP(a, b, n) \
    _credo_notare_f64_ulp( \
            "credo_f64_ulp", \
            #a " ≈ " #b, \
            (a), \
            (b), \
            (i64)(n), \
            __FILE__, \
            __LINE__ \
    )


/* ============================================
 * Adiutores Interni
 * ============================================ */

vacuum
_credo_notare (
                      constans character* genus,
                      constans character* expressio,
                      constans character* valor_primus,
                      constans character* valor_secundus,
                      constans character* filum,
                                     s32  versus,
                                     b32  praeteritus);


vacuum
_credo_notare_integrum (
                      constans character* genus,
                      constans character* expressio,
                                     s32  valor_primus,
                                     s32  valor_secundus,
                      constans character* filum,
                                     s32  versus);

vacuum
_credo_notare_integrum_i32 (
                      constans character* genus,
                      constans character* expressio,
                                     i32  valor_primus,
                                     i32  valor_secundus,
                      constans character* filum,
                                     s32  versus);

vacuum
_credo_notare_integrum_s64 (
                      constans character* genus,
                      constans character* expressio,
                                     s64  valor_primus,
                                     s64  valor_secundus,
                      constans character* filum,
                                     s32  versus);

vacuum
_credo_notare_integrum_inaequalitas (
                      constans character* genus,
                      constans character* expressio,
                                     s32  valor_primus,
                                     s32  valor_secundus,
                      constans character* filum,
                                     s32  versus);

vacuum
_credo_notare_integrum_inaequalitas_i32 (
                      constans character* genus,
                      constans character* expressio,
                                     i32  valor_primus,
                                     i32  valor_secundus,
                      constans character* filum,
                                     s32  versus);

vacuum
_credo_notare_chorda (
                      constans character* genus,
                      constans character* expressio,
                      constans character* valor_primus,
                      constans character* valor_secundus,
                      constans character* filum,
                                     s32  versus);

vacuum
_credo_notare_chorda_literis (
                      constans character* genus,
                      constans character* expressio,
                                  chorda  valor_primus,
                      constans character* valor_secundus,
                      constans character* filum,
                                     s32  versus);

vacuum
_credo_notare_integrum_maior (
                      constans character* genus,
                      constans character* expressio,
                                     s32  valor_primus,
                                     s32  valor_secundus,
                      constans character* filum,
                                     s32  versus);

vacuum
_credo_notare_integrum_maior_i32 (
                      constans character* genus,
                      constans character* expressio,
                                     i32  valor_primus,
                                     i32  valor_secundus,
                      constans character* filum,
                                     s32  versus);

vacuum
_credo_notare_integrum_minor (
                      constans character* genus,
                      constans character* expressio,
                                     s32  valor_primus,
                                     s32  valor_secundus,
                      constans character* filum,
                                     s32  versus);

vacuum
_credo_notare_integrum_minor_i32 (
                      constans character* genus,
                      constans character* expressio,
                                     i32  valor_primus,
                                     i32  valor_secundus,
                      constans character* filum,
                                     s32  versus);

vacuum
_credo_notare_integrum_maior_aut_aequalis (
                      constans character* genus,
                      constans character* expressio,
                                     s32  valor_primus,
                                     s32  valor_secundus,
                      constans character* filum,
                                     s32  versus);

vacuum
_credo_notare_integrum_maior_aut_aequalis_i32 (
                      constans character* genus,
                      constans character* expressio,
                                     i32  valor_primus,
                                     i32  valor_secundus,
                      constans character* filum,
                                     s32  versus);

vacuum
_credo_notare_integrum_minor_aut_aequalis (
                      constans character* genus,
                      constans character* expressio,
                                     s32  valor_primus,
                                     s32  valor_secundus,
                      constans character* filum,
                                     s32  versus);

vacuum
_credo_notare_integrum_minor_aut_aequalis_i32 (
                      constans character* genus,
                      constans character* expressio,
                                     i32  valor_primus,
                                     i32  valor_secundus,
                      constans character* filum,
                                     s32  versus);

vacuum
_credo_notare_chordae (
                      constans character* genus,
                      constans character* expressio,
                                  chorda  receptus,
                                  chorda  speratus,
                                     s32  comparatio,
                      constans character* filum,
                                     s32  versus);

vacuum
_credo_notare_literae (
                      constans character* genus,
                      constans character* expressio,
                      constans character* receptus,
                      constans character* speratus,
                      constans character* filum,
                                     s32  versus);

vacuum
_credo_notare_octeti (
                        constans character* genus,
                        constans character* expressio,
                           constans vacuum* receptus,
                           constans vacuum* speratus,
                            memoriae_index  mensura,
                        constans character* filum,
                                       s32  versus);

vacuum
_credo_notare_s64 (
                      constans character* genus,
                      constans character* expressio,
                                     s64  valor_primus,
                                     s64  valor_secundus,
                                     s32  relatio,
                      constans character* filum,
                                     s32  versus);

vacuum
_credo_notare_i64 (
                      constans character* genus,
                      constans character* expressio,
                                     i64  valor_primus,
                                     i64  valor_secundus,
                                     s32  relatio,
                      constans character* filum,
                                     s32  versus);

vacuum
_credo_notare_f64_relative (
                      constans character* genus,
                      constans character* expressio,
                                     f64  valor_primus,
                                     f64  valor_secundus,
                                     f64  relativa,
                      constans character* filum,
                                     s32  versus);

vacuum
_credo_notare_f64_ulp (
                      constans character* genus,
                      constans character* expressio,
                                     f64  valor_primus,
                                     f64  valor_secundus,
                                     i64  distantia,
                      constans character* filum,
                                     s32  versus);

vacuum
_credo_notare_f64_proximus (
                      constans character* genus,
                      constans character* expressio,
                                     f64  valor_primus,
                                     f64  valor_secundus,
                                     f64  epsilon,
                      constans character* filum,
                                     s32  versus);


/* ==================================================
 * Assertiones processus - "hoc NON ruit, hoc NON pendet"
 * ==================================================
 *
 * Expressio in filio FURCATO curritur, ergo ruina aut gyrus
 * probationem non occidit. Spec: stml-instrumentum-spec.md par.
 * 7.5.0.
 *
 * CUR MACRO SENTENTIAE, NON VOCATIO FUNCTIONIS: argumenta apud
 * VOCANTEM aestimantur. Si expressio argumentum esset, ruina in
 * PARENTE fieret et suitem totam secum traheret - id ipsum quod
 * vitare conamur. Ergo furca PRIMA, aestimatio in filio SOLO.
 *
 * NUMQUAM SIGSEGV intra processum capere (manubrium signi +
 * longjmp): mos ille INDEFINITUS est - a manubrio exceptionis
 * computatricis redire non licet, et post culpam veram status
 * piscinae atque allocatoris ignotus est. Praeterea gyros omnino
 * non videt. Furca sola sana est.
 */

nomen structura {
    b32 in_filio;   /* VERUM in filio furcato SOLO */
    s32 pid;        /* in parente: pid filii; -I si furca defecit */
} CredoProcessusFructus;

/* fflush(NIHIL) deinde fork(). In filio {VERUM, ZEPHYRUM} redit. */
CredoProcessusFructus
credo_processus_incipere (vacuum);

/* _exit(0) - NUMQUAM exit(): filius buffra parentis hereditavit,
 * et exit() ea eiceret, ergo omnis linea prior BIS emitteretur. */
vacuum
credo_processus_filium_finire (vacuum);

/* In parente: filium ad terminum exspectat, deinde iudicat.
 * signum_exspectatum == ZEPHYRUM => praeterit si NORMALITER exiit.
 * signum_exspectatum != ZEPHYRUM => praeterit si EO signo periit. */
vacuum
credo_processus_iudicare (
                CredoProcessusFructus* fructus,
                   constans character* genus,
                   constans character* expressio,
                                  i32  mora_ms,
                                  s32  signum_exspectatum,
                   constans character* filum,
                                  s32  versus);

#define CREDO_MORA_ORDINARIA_MS 5000

#define CREDO_NON_RUIT(expressio) \
    fac { \
        CredoProcessusFructus _cpf = credo_processus_incipere(); \
        si (_cpf.in_filio) \
        { \
            (expressio); \
            credo_processus_filium_finire(); \
        } \
        credo_processus_iudicare(&_cpf, "credo_non_ruit", #expressio, \
            CREDO_MORA_ORDINARIA_MS, ZEPHYRUM, __FILE__, __LINE__); \
    } dum (FALSUM)

#define CREDO_NON_PENDET(expressio, mora_ms) \
    fac { \
        CredoProcessusFructus _cpf = credo_processus_incipere(); \
        si (_cpf.in_filio) \
        { \
            (expressio); \
            credo_processus_filium_finire(); \
        } \
        credo_processus_iudicare(&_cpf, "credo_non_pendet", \
            #expressio, (mora_ms), ZEPHYRUM, __FILE__, __LINE__); \
    } dum (FALSUM)

/* _INTRA: terminum nominat. Pro filio qui NUMQUAM redit (gyrus
 * infinitus, SIGKILL exspectatum) forma nuda V secunda dormit
 * antequam occidat - terminus brevis ibi non est somnus sed
 * mensura: quam cito detector eum capiat. */
#define CREDO_RUIT_CUM_INTRA(expressio, signum, mora_ms) \
    fac { \
        CredoProcessusFructus _cpf = credo_processus_incipere(); \
        si (_cpf.in_filio) \
        { \
            (expressio); \
            credo_processus_filium_finire(); \
        } \
        credo_processus_iudicare(&_cpf, "credo_ruit_cum", #expressio, \
            (mora_ms), (signum), __FILE__, __LINE__); \
    } dum (FALSUM)

#define CREDO_RUIT_CUM(expressio, signum) \
    CREDO_RUIT_CUM_INTRA(expressio, signum, CREDO_MORA_ORDINARIA_MS)


/* ============================================
 * Sectiones (credo v2): suita ut tabula sectionum nominatarum
 * ============================================
 *
 * Additum, non mutatio: suitae veteres (credo_aperire, assertiones in
 * principale, compendium) immutatae currunt; transeunt cum tanguntur.
 *
 * Sectio quaeque: piscinam PROPRIAM accipit (destruitur post purgare),
 * parare -> probare -> purgare (purgare SEMPER, etiam post NECESSE
 * fractum), verdictum suum (numeri, tempus, fractura prima).
 * Sectiones ordine liberae esse debent (status communis = vitium).
 */

nomen structura CredoContextus {
                Piscina* piscina;   /* sectionis propria */
                 vacuum* datum;     /* parare ponit, probare/purgare
                                     * legunt */
     constans character* titulus;   /* sectio currens */
                 vacuum* suita;     /* (II) datum parare_suitae;
                                     * sectiones LEGUNT, numquam mutant
                                     * (examen 94) praeter internationem
                                     * in internamentum fixurae (vide
                                     * credo_suitam_currere_cum); NIHIL
                                     * sine fixura */
} CredoContextus;

nomen vacuum (*CredoFunctio)(CredoContextus* c);

nomen structura CredoSectio {
    constans character* titulus;           /* unicus in suita: clavis
                                            * verdicti */
           CredoFunctio probare;
           CredoFunctio parare;            /* NIHIL = nihil */
           CredoFunctio purgare;           /* NIHIL = nihil; SEMPER
                                            * vocatur */
    constans character* signa;             /* (II) verba spatio separata
                                            * ex vocabulario CLAUSO:
                                            * lentus rete fenestra
                                            * repositorium gravis
                                            * (tectum memoriae
                                            * CREDO_MEMORIA_GRAVIS_MB);
                                            * ignotum ->
                                            * suita FRACTA nominans.
                                            * CREDO_SIGNA_OMITTE="rete"
                                            * -> sectio OMISSA (in
                                            * plagula, nominata) */
    constans character* exspectata_fracta; /* (II) CAUSA in fonte
                                            * scripta (non vacua; id
                                            * tabularii intra licet,
                                            * numquam legitur): fracta
                                            * -> NOTA_FRACTA (suita
                                            * transit), transiens ->
                                            * INOPINATA (suita FRACTA:
                                            * linea delenda). Census:
                                            * tools/credo_fracturae_
                                            * notae.py */
} CredoSectio;

/* TECTUM MEMORIAE (credo memoria T4): sectione finita pondus = maior
 * apicis arenae et incrementi apicis vitae; pondus > tectum = fractura
 * nominata 'credo_memoria' (sectio FRACTA, suita I). Tectum: signum
 * 'gravis' -> CREDO_MEMORIA_GRAVIS_MB; ceterae CREDO_MEMORIA_MB ex
 * ambitu si ponitur (0 = sine tecto), aliter ORDINARIA. Valores ex
 * mensura 2026-10-20 (mensor quaestio graves, MMMCXVI sectiones):
 * sectio normalis gravissima CXXXV MiB, duae supra CCLVI signum
 * gravis ferunt. Fugam intra sectionem NON sistit (credo solum in
 * fine imperium habet) - id custos cursoris est (spec C2). */
#define CREDO_MEMORIA_ORDINARIA_MB  CCLVI
#define CREDO_MEMORIA_GRAVIS_MB     MXXIV

/* Suitam currere: sectiones ordine usque ad titulum NIHIL. Ambitus:
 *   CREDO_SECTIO=<titulus>  sectio sola (ceterae non currunt, non
 *                           numerantur); titulus ignotus = I, nihil
 *                           cursum, nominatum
 *   CREDO_SIGNA_OMITTE=<verba>  sectiones cum signo quovis horum
 *                           OMISSAE (credo II); verbum ignotum = I,
 *                           nihil cursum, nominatum
 *   CREDO_ORDO=mixtus[:<semen>]  ordo sectionum permutatus (credo
 *                           II; Fisher-Yates, semen IX cifrarum ad
 *                           summum; sine semine ex tempore electum) -
 *                           semen NOMINATUR ("=== ORDO mixtus semen N
 *                           ===", linea ORDO) ut cursus iteretur;
 *                           instrumentum status communis occulti, non
 *                           ordinarium. "tabula"/vacuum = ordo tabulae;
 *                           valor alius = I, nihil cursum, nominatum
 *   CREDO_VERDICTA=<via>    plagula verdictorum (TSV) scribitur
 *   CREDO_MEMORIA_MB=<n>    tectum memoriae sectionum ordinariarum
 *                           (credo memoria T4; 0 = nullum); signum
 *                           gravis CREDO_MEMORIA_GRAVIS_MB servat
 * Plagula: linea per sectionem STATIM post eam (effluxa), deinde linea
 * SUITA - linea SUITA absens = suita ruit (sectiones priores tamen
 * notae). Campi tabulatione separati; '\t' '\n' '\\' in valoribus
 * effugiuntur:
 *   SECTIO  suita  titulus  exitus  praeteriti  totales  ms
 *           filum:versus  genus  expressio  gradus  (fractura prima;
 *           vacua si transiit; gradus = CREDO_GRADUS currens, credo II)
 *           arena_kib  vestigium_kib  apex_auctus_kib  (credo memoria
 *           T2: apex usus piscinae sectionis, vestigium processus in
 *           fine, incrementum apicis vitae dum sectio currit - KiB;
 *           0 si omissa; effusio humana NIHIL de memoria dicit)
 *   SUITA   suita  exitus  transeuntes  sectiones  praeteriti  totales
 *           ms
 *   ORDO    suita  mixtus  semen          (credo II; ante sectiones,
 *                                         solum si CREDO_ORDO mixtus)
 *   SECTIONES suita  numerus  (analytica T4; linea PRIMA: sectiones
 *           declaratae, filtro respecto)
 *   INCIPIT suita  titulus     (analytica T4; ante sectionem quamque,
 *           EFFLUXA - ruina vera nihil effluit: incepta sine SECTIO =
 *           sectio rupta, lector eam RUIT nominat)
 * exitus sectionis: TRANSIIT | FRACTA | VACUA (nulla assertio = non
 * transiit, ut NIHIL PROBATUM) | ABORTA (NECESSE fractum) | OMISSA
 * (signum omissum, credo II) | NOTA_FRACTA (fractura exspectata, suita
 * transit) | INOPINATA (exspectata fracta transiit - suita FRACTA).
 * exitus suitae: TRANSIIT | FRACTA | VACUA (nulla sectio cursa).
 * Effusio humana ut olim (--- Probans <titulus> ---, puncta, FRACTA,
 * compendium). Redde ZEPHYRUM si omnes sectiones cursae transierunt, I
 * aliter. */
s32
credo_suitam_currere (
      constans character* titulus_suitae,
    constans CredoSectio* sectiones);

/* GRADUS intra sectionem (credo II): nomen gradus currentis; fractura
 * prima eum fert ("... at f:12 (in gradu VI: nuntius)", campus XI
 * plagulae SECTIO) - nominatio sola, nulla isolatio. Gradus ad initium
 * cuiusque sectionis vacatur; titulus durare debet (litterae). */
#define CREDO_GRADUS(titulus) credo_gradum_ponere(titulus)

vacuum
credo_gradum_ponere (
    constans character* titulus);

/* AREA (credo II): directorium proprium contextus sub TMPDIR -
 * sectionis '<TMPDIR>/<suita>.<n>.<pid>' (n = ordo in tabula, ab I),
 * suitae (in parare_suitae/purgare_suitae) '<suita>.suita.<pid>';
 * titulus suitae sanatur (praeter [A-Za-z0-9_-] -> '_'). Creatur PIGRE
 * ad vocationem primam (eadem via deinde), deletur tota post purgare
 * (etiam post NECESSE, etiam si filius CREDO_NON_RUIT eam creavit).
 * Sub fabrica TMPDIR = area membri. NIHIL si creari non potest aut c
 * contextus currens non est. Via usque ad deletionem valet.
 *
 * TMPDIR SUITAE (2026-10-23): credo_suitam_currere[_cum] TMPDIR in
 * aream SUITAE ponit (filii hereditant) et post suitam restituit -
 * quidquid temporarium suita creat suum est, suitae parallelae se non
 * vident. Post sectionem quamque '*.volumen' ibi relictum = fractura
 * 'credo_volumen' ILLIUS sectionis (volumen temporarium non clausum),
 * deinde deletum; volumina post parare_suitae viva (fixurae suitae)
 * sectionibus praetereuntur et post purgare_suitae iudicantur
 * ('(suita)'). Pretium ~XXX-XL us per sectionem (mensuratum: chorda
 * L sectiones +8.3 M instructiones). Suitae extra credo_suitam_currere
 * (credo_aperire nudum) TMPDIR intactum habent. */
constans character*
credo_area (
    CredoContextus* c);

/* SUITA CUM FIXURA COMMUNI (credo II): parare_suitae SEMEL ante
 * sectiones (contextus suitae: piscina sua; datum eius -> c->suita
 * omnium sectionum), purgare_suitae SEMPER post ultimam - etiam post
 * sectionem abortam aut parare_suitae fractum. parare_suitae fractum
 * (assertio aut NECESSE) -> nulla sectio currit, '(parare suitae)' in
 * plagula notatur, suita FRACTA. NIHIL licet (= credo_suitam_currere).
 * Fixura cara (daemon, applicatio) semel aedificatur; sectiones tamen
 * ordine liberae manent quia statum suitae non mutant.
 *
 * LECTIO SOLA, EXCEPTIO UNA (decretum Frani 2026-10-19): sectiones in
 * internamentum FIXURAE internare licet (InternamentumChorda* quod
 * parare_suitae creavit, quovis vocato traditum). Chordae internatae per
 * monstratorem comparantur, ergo internamentum proprium sectionis
 * identitatem cum nominibus fixurae tacite frangeret; et internatio
 * ordine libera est (textus idem -> chorda eadem, quaecumque sectio
 * prima internat). Ubi comparatio per contentum fit, internamentum
 * proprium sectionis (in c->piscina) ordinarium manet. Cetera mutatio
 * fixurae vetita.
 * Examen 94 c->suita ipsum (gradus primus) ET membra monstratoria
 * fixurae (Xar*, Piscina*, ... per 'constans T*' lecta) intra
 * sectionem iudicat: scriptio per ea aut traditio ad parametrum non
 * constans accusatur. LIMES: fixura adiutori per parametrum tradita
 * non sequitur; mutatio celata intra bibliothecam (arena possessoris
 * crescens) per vocatum solum videtur - CREDO_ORDO=mixtus
 * dependentiam ordinis prodit. */
s32
credo_suitam_currere_cum (
      constans character* titulus_suitae,
    constans CredoSectio* sectiones,
            CredoFunctio  parare_suitae,
            CredoFunctio  purgare_suitae);

/* NECESSE: assertio FATALIS - fracta sectionem finit (longjmp ad
 * cursorem; purgare tamen vocatur), loco custodiarum
 * 'si (p != NIHIL) { ... }'. Extra sectionem = CREDO_VERUM. In filio
 * CREDO_NON_RUIT fracta filium finit (_exit), numquam sectiones
 * sequentes in filio currit (assertiones filii, ut semper, a parente
 * non numerantur). */
#define CREDO_NECESSE(x) \
    _credo_necesse( \
            "credo_necesse", \
            #x, \
            __FILE__, \
            __LINE__, \
            (x) != FALSUM \
    )

#define CREDO_NECESSE_NON_NIHIL(p) \
    _credo_necesse( \
            "credo_necesse_non_nihil", \
            #p, \
            __FILE__, \
            __LINE__, \
            (p) != NIHIL \
    )

vacuum
_credo_necesse (
    constans character* genus,
    constans character* expressio,
    constans character* filum,
                   s32  versus,
                   b32  praeteritus);

#endif /* CREDO_H */

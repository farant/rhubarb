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
#include "silva_parsare.h"
#include "silva_commissio.h"

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
    b32 ex_systemate;       /* provenientia (haruspex: de typis
                             * systematis solum opacitas asseritur) */

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
 * Symbola + scopi (Chunk B): spatia nominum C89 - ordinaria
 * (variabiles, functiones, typedefs, CONSTANTES enumerorum - unum
 * spatium!), tags (structura/unio/enumeratio - unum spatium tag),
 * membra (in TypusC89 ipso), tituli saltuum (lint, non hic)
 * ================================================== */

nomen enumeratio {
    SYMBOLUM_VARIABILE = 0,
    SYMBOLUM_FUNCTIO,
    SYMBOLUM_TYPEDEF,
    SYMBOLUM_CONSTANS,     /* enumeratoris (valor impletum) */
    SYMBOLUM_PARAMETRUM
} SemanticaSymbolumGenus;

#define REPOSITIO_STATICA   1
#define REPOSITIO_EXTERNA   2
#define REPOSITIO_AUTOMATA  4
#define REPOSITIO_REGISTRI  8

nomen structura {
    s32                  genus;        /* SemanticaSymbolumGenus */
    chorda               titulus;      /* copia in piscinam */
    TypusC89*            typus;
    s64                  valor;        /* CONSTANS solum */
    i32                  repositio;    /* REPOSITIO_* vexilla */
    i32                  profunditas;  /* 0 = scopus fili */
    b32                  ex_systemate; /* provenientia */
    b32                  est_implicitum; /* extern int implicitum
                                         * synthetizatum (vocatus
                                         * ignotus C89 6.3.2.2) -
                                         * signum quod lamina
                                         * exsecutionis postulat;
                                         * post-hoc irrecuperabile
                                         * (M0b C, emendatio) */
    constans SilvaNodus* declarans;   /* pro implicitis: SEDES
                                       * VOCATIONIS, non declaratio */
    SilvaToken*          lexema;       /* nomen; NIHIL licet */
} SemanticaSymbolum;

nomen structura SemanticaScopus SemanticaScopus;
structura SemanticaScopus {
    SemanticaScopus* pater;       /* NIHIL in summo */
    TabulaDispersa*  ordinaria;   /* titulus -> SemanticaSymbolum* */
    TabulaDispersa*  tags;        /* titulus -> TypusC89* */
    i32              profunditas;
};

/* ==================================================
 * Semantica (contextus communis typus/forma/aestimator/scopus)
 * ================================================== */

nomen structura {
    constans SilvaNodus* nodus;   /* NIHIL licet */
    constans character*  causa;   /* litterae staticae */
} SemanticaDiagnosticum;

/* Titulo signata: silva.h eam OPACAM praenuntiat (exemplar
 * SilvaOraculum) - interna (scopi, derivati) post amalgamam
 * celantur; hospites per functiones quaerunt */
nomen structura SilvaSemantica SilvaSemantica;
structura SilvaSemantica {
    Piscina* piscina;

    /* singuli */
    TypusC89* primitivi[PRIMITIVUM_NUMERUS];
    TypusC89* typus_erroris;

    /* internamentum derivatorum (series linearis - complete
     * primum, optima post; percursus mensurabit) */
    Xar* derivati;              /* TypusC89* */

    /* scopi (Chunk B - regula sanationis: registra-ante-usum in
     * AMBULATIONE UNA ordine fontis) */
    SemanticaScopus* scopus_summus;    /* fili (profunditas 0) */
    SemanticaScopus* scopus_currens;

    /* index: OMNIA symbola creata (cum profunditate) - fons
     * indicis proiecti (Chunk D superficiem dat) */
    Xar* symbola;               /* SemanticaSymbolum* */

    Xar* diagnostica;           /* SemanticaDiagnosticum (valore) */

    /* typatio expressionum (M0b): clavis = octeti valoris
     * monstratoris nodi (lectionis canonicae, non involucri) ->
     * SemanticaTypatio (in .c definita - superficies infra).
     * FNV super octetos: chorda binaria licet, fabrica _chorda
     * ordinaria sufficit. */
    TabulaDispersa* typationes;

    /* nexus symbolorum (M1a Chunk A): sedes usus -> symbolum -
     * clavis eadem ac typationes (octeti monstratoris nodi).
     * Registratur in ipsa resolutione (folium-identificator +
     * vocatus vocationis); demissio officinae per
     * silva_c89_symbolum_nodi legit. */
    TabulaDispersa* nexus;

    /* typus reditus functionis currentis (M0b B: conversio
     * valoris redde annotatur; NIHIL extra corpora) */
    TypusC89* reditus_currens;

    b32 in_systemate;           /* vexillum ambulationis (provenientia) */
};

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

/* Registratio typedef in scopum CURRENTEM (B: scopus-conscia) */
b32 silva_c89_typedef_registrare (SilvaSemantica* sem,
    chorda titulus, TypusC89* typus);
TypusC89* silva_c89_typedef_invenire (SilvaSemantica* sem,
    chorda titulus);

/* ==================================================
 * Ambulatio integra (Chunk B): analysis totius fili
 * ================================================== */

/* Ambulatio ordine fontis super radicem commissam: declarationes +
 * definitiones functionum (parametra + corpora, scopi nidificati),
 * conditionalis per ramos SUMPTOS (omissi = cruda, praetermissi -
 * DECISUS 6), AMBIGUUS per canonicam (cave: canonica furcarum
 * decl/expr est lectio EXPRESSIONIS), ERROR praetermissus.
 * declarationes-kr = parca nominata (diagnosticum). Symbola omnia
 * in sem->symbola accumulantur (index). Numquam ruit - venenum +
 * diagnostica (regula tree-sitter). */
SilvaSemantica* silva_c89_semantica_analysare (Piscina* piscina,
    constans SilvaParsura* parsura);

/* Quaestio symboli a scopo currenti foras (post analysem = scopus
 * fili). NIHIL si absens. */
SemanticaSymbolum* silva_c89_symbolum_invenire (SilvaSemantica* sem,
    chorda titulus);

/* ==================================================
 * Systema (Chunk C): canalis B - arbor systematis SEMEL parsata
 * (silva/fontes/systema_c89.h), symbola eius in scopum summum
 * harvestata (ex_systemate signata) ANTE ambulationem usoris;
 * parsurae usoris intactae manent (stratum proprium quaerbile).
 * ================================================== */

/* Ut analysare, sed symbola systematis prius harvestat (systema
 * NIHIL licet = analysare simplex). Arbor systematis piscinam
 * SUAM habere potest - vivat quamdiu sem vivit. */
SilvaSemantica* silva_c89_semantica_analysare_cum_systemate (
    Piscina* piscina, constans SilvaParsura* parsura,
    constans SilvaParsura* systema);

/* Symbola semanticae in oraculum fundere: TYPEDEF -> typi noti,
 * cetera -> NON-typi noti (trivalens). Duplex usus: praeoneratio
 * (sem systematis -> oraculum per plagulam) et clausura (sem
 * plagulae -> oraculum ante silva_recanonicare - vide percursum).
 * Numerum nominum fusorum reddit. */
i32 silva_c89_semantica_oraculum_augere (
    constans SilvaSemantica* sem, SilvaOraculum* oraculum);

/* Residuum clausurae: quot ambigua retenta ADHUC INDECISA sint
 * (resolutor victorem non dat) sub oraculo dato. NB: responsa
 * stipata leguntur - voca silva_oraculum_responsa_vacare ante, si
 * status RECENS oraculi quaeritur. "Decisum" != "versum":
 * resolutio quae canonicae iam congruit nihil vertit sed decisa
 * est (recanonicare solum versiones numerat). */
i32 silva_c89_ambigua_indecisa_numerare (
    constans SilvaCommissio* commissio,
    constans SilvaOraculum*  oraculum);

/* ==================================================
 * Index (Chunk D): superficies quaestionis symbolorum - primus
 * consumptor nominatus = saltuarius (saltus typorum, v0.1);
 * secundus = demissio officinae. In memoria; persistentia parcata
 * donec consumptor frigidus existat (DECISUS 8).
 * ================================================== */

i32 silva_c89_symbola_numerus (constans SilvaSemantica* sem);
constans SemanticaSymbolum* silva_c89_symbolum_per_indicem (
    constans SilvaSemantica* sem, i32 index);

i32 silva_c89_diagnostica_numerus (constans SilvaSemantica* sem);
constans SemanticaDiagnosticum* silva_c89_diagnosticum_per_indicem (
    constans SilvaSemantica* sem, i32 index);

/* ==================================================
 * Typatio expressionum (M0b Chunk A): tabula parallela
 * nodus -> {typus naturalis, typus conversus} - contractus
 * demissionis (phase-log M0b INTENTIO DECISUS 1). Naturalis =
 * typus proprius synthetizatus SINE lapsu (acies manet acies,
 * functio manet functio - sizeof rectum per constructionem);
 * conversus = conversio scopo applicata (lapsus/UAC/assignatio/
 * argumentum - B/C) aut NIHIL. Quaestiones _canonicum consciae:
 * involucrum ambiguum aut lectionem des, idem redditur.
 * NB typatio RELATIVA CANONICAE est - post recanonicare analysim
 * novam fac (INTENTIO DECISUS 2; percursus bis analysat).
 * ================================================== */

constans TypusC89* silva_c89_typus_expressionis (
    constans SilvaSemantica* sem, constans SilvaNodus* nodus);

/* NIHIL = nulla conversio scopo applicata */
constans TypusC89* silva_c89_conversio_expressionis (
    constans SilvaSemantica* sem, constans SilvaNodus* nodus);

i32 silva_c89_typationes_numerus (constans SilvaSemantica* sem);

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

/* ==================================================
 * Exporta demissionis (M1a Chunk A; officina-m1-spec §II):
 * nexus symbolorum + octeti chordae decodati
 * ================================================== */

/* Symbolum sedis usus (folium-identificator aut vocatus
 * vocationis). _canonicum-conscia ut typatio. NIHIL si sedes
 * non resoluta. NB RELATIVA CANONICAE ut typatio - post
 * recanonicare analysim novam fac. */
constans SemanticaSymbolum* silva_c89_symbolum_nodi (
    constans SilvaSemantica* sem, constans SilvaNodus* nodus);

i32 silva_c89_nexus_numerus (constans SilvaSemantica* sem);

/* Octeti chordae litteralis DECODATI (fugae solutae, fragmenta
 * adiacentia coniuncta). SINE nullo terminali - materia data
 * (acies.numerus = octeti + I; demissio nullum ex typo scribit).
 * FALSUM: L-chorda (parca), fuga invalida, nodus non chorda. */
b32 silva_c89_chorda_decodere (Piscina* piscina,
    constans SilvaNodus* nodus, chorda* octeti_out);

#endif /* SILVA_C89_SEMANTICA_H */

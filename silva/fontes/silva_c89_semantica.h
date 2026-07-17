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
 * aestimatore (typum expressionis postulat = M0b); completio aciei
 * per initiatorem FACTA (2026-07-10), VALIDATIO initiatorum parca
 * (project-specs/c89-lacunae.md); K&R; vide phase-log.
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

/* SYMBOLA = REGISTRATIONES, NON ENTIA (acta sunt fructus):
 * declaratio quaeque symbolum PROPRIUM registrat - prototypus +
 * definitio eiusdem functionis = symbola DUO, quodque cum declarans
 * suo. Tabula scopi recentissimum tenet (resolutio nominum);
 * enumeratio (symbolum_per_indicem) OMNIA reddit. Consumptor qui
 * "ens" vult (extentum definitionis...) registrationes ipse plicet,
 * definitione praelata - probatio: definitio_functionis_corpus
 * (declarans) generis alieni SILVA_VALOR_NIHIL reddit. Umbra
 * prototypi in legato inde orta est (2026-07-14): primus-congruens
 * prototypum capiebat. */
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
 * Examen (M4a chunk A): severitas + codices diagnosticorum
 * ================================================== */

/* Gradus severitatis (interrogatio M4a Q2/Q3 + emendatio chunki A):
 * VIOLATIO = constrictio C89 (linea standard-stricta); SUSPECTUM =
 * licitum sed suspectum; DOMESTICUM = regula domus; INFRA = lacuna
 * instrumentorum, NUMQUAM iudicium (generalizat LEXICON_DEEST -
 * verdicta ordines INFRA excludunt, ut provisionales). */
nomen enumeratio {
    EXAMEN_VIOLATIO = 0,
    EXAMEN_SUSPECTUM,
    EXAMEN_DOMESTICUM,
    EXAMEN_INFRA
} ExamenSeveritas;

/* Codex stabilis per nuntium - FONS UNICUS (tabula in .c dat causam
 * + severitatem per codicem). Ordo tabulae = hic ordo: NE INSERE -
 * adde ante NUMERUS (codices stabiles trans versiones pro vecte
 * differentiali et pinnis EXSPECTA). */
nomen enumeratio {
    EXAMEN_CODEX_REDECLARATIO_GENERIS = 0,
    EXAMEN_CODEX_TAG_SINE_CORPORE,
    EXAMEN_CODEX_ENUMERATIO_SINE_CORPORE,
    EXAMEN_CODEX_ENUMERATOR_NON_CONSTANS,
    EXAMEN_CODEX_TYPUS_NOMINATUS_IGNOTUS,   /* INFRA: LEXICON_DEEST */
    EXAMEN_CODEX_ERROR_IN_SPECIFICATORIBUS,
    EXAMEN_CODEX_SPECIFICATORES_MIXTI,
    EXAMEN_CODEX_MULTISET_PRIMITIVORUM,
    EXAMEN_CODEX_MENSURA_ACIEI,
    EXAMEN_CODEX_ERROR_IN_DECLARATORE,
    EXAMEN_CODEX_GENUS_DECLARATORIS,        /* INFRA: cautela */
    EXAMEN_CODEX_SPECIES_TYPI_EXSPECTATA,
    EXAMEN_CODEX_DECLARATIONES_KR,          /* INFRA: parca */
    EXAMEN_CODEX_CYCLUS_FORMAE,
    EXAMEN_CODEX_FORMA_ACIEI_INCOMPLETAE,
    EXAMEN_CODEX_FORMA_FUNCTIONIS,
    EXAMEN_CODEX_CAMPI_PARCATI,             /* INFRA: parca */
    EXAMEN_CODEX_CHORDA_LATA,               /* INFRA: parca */
    EXAMEN_CODEX_FUGA_INVALIDA,
    EXAMEN_CODEX_LITTERALE_INVALIDUM,
    EXAMEN_CODEX_IDENTIFICATOR_IGNOTUS,
    EXAMEN_CODEX_TYPEDEF_IN_EXPRESSIONE,
    EXAMEN_CODEX_TYPEDEF_VOCATUM,
    EXAMEN_CODEX_OPERANDUM_UNARII,
    EXAMEN_CODEX_OPERANDUM_TILDE,
    EXAMEN_CODEX_DEIECTIO_NON_MONSTRATORIS,
    EXAMEN_CODEX_CREMENTUM_NON_SCALARE,
    EXAMEN_CODEX_MULTIPLICATIVA,
    EXAMEN_CODEX_OPERANDA_NON_INTEGRALIA,
    EXAMEN_CODEX_ADDITIVA_INCOMPATIBILIA,
    EXAMEN_CODEX_SUBTRACTIONIS_INCOMPATIBILIA,
    EXAMEN_CODEX_MOTUS_NON_INTEGRALIA,
    EXAMEN_CODEX_COMPARATIO_INCOMPATIBILIUM,
    EXAMEN_CODEX_BRACCHIA_TERNARII,
    EXAMEN_CODEX_SUBSCRIPTIO_NON_MONSTRATORIS,
    EXAMEN_CODEX_VOCATUS_NON_FUNCTIO,
    EXAMEN_CODEX_SAGITTA_NON_MONSTRATORIS,
    EXAMEN_CODEX_ACCESSUS_NON_STRUCTURAE,
    EXAMEN_CODEX_ACCESSUS_INCOMPLETAE,
    EXAMEN_CODEX_MEMBRUM_IGNOTUM,
    EXAMEN_CODEX_ELISIO_UNCORUM,            /* INFRA: parca */
    /* chunk B (relatio compatibilitatis) - flagrant a chunk C */
    EXAMEN_CODEX_ASSIGNATIO_INCOMPATIBILIS,
    EXAMEN_CODEX_MONSTRATORES_INCOMPATIBILES,
    EXAMEN_CODEX_QUALES_ABIECTI,
    EXAMEN_CODEX_MONSTRATOR_INTEGER,
    /* chunk C (sedes) */
    EXAMEN_CODEX_ASSIGNATIO_NON_LOCUS,
    EXAMEN_CODEX_LOCUS_IMMUTABILIS,
    EXAMEN_CODEX_NUMERUS_ARGUMENTORUM,
    EXAMEN_CODEX_REDDE_CUM_VALORE_IN_VACUO,
    EXAMEN_CODEX_REDDE_SINE_VALORE,     /* SUSPECTUM: legale C89 */
    EXAMEN_CODEX_CONDICIO_NON_SCALARIS,
    EXAMEN_CODEX_VOCATIO_IMPLICITA,     /* SUSPECTUM: est_implicitum */
    EXAMEN_CODEX_REDECLARATIO_INCOMPATIBILIS,
    /* sequela M4a (2026-07-14): macro domesticum in capite alieno
     * (vendor) expansum - ordo inclusionis significationem tacite
     * mutat ('char C' -> 'char 100'); causa IN PISCINA structa */
    EXAMEN_CODEX_MACRO_DOMESTICUM_IN_ALIENO,  /* SUSPECTUM */
    /* gradus conversionis signi (2026-07-16): conversio implicita
     * integralis quae signum mutat et valores non servat (murus
     * i32-insignatus - decipula domus frequentissima). Legale C89;
     * vexilla domus (-Wsign-conversion -Werror) reiciunt - examen
     * ANTE compilatorem nominat. Suppressiones: cast explicita,
     * contextus comparationis (phasis II), constans capit finem.
     * Causa IN PISCINA structa (typos ambos nominat). */
    EXAMEN_CODEX_CONVERSIO_SIGNI,             /* DOMESTICUM */
    /* gradus SEVERI (TOLERA, 2026-07-17): analysis stricta SUPRA
     * paritatem oraculi. SEVERA = analysis sana securitatem probare
     * non potest ubi heuristica clang tacet (v1: subtractio numquam
     * non-negativa) - sedes flagrat 54 AUT severa, numquam ambo;
     * TOLERA numquam silet quod clang reiceret. Suppressibilis
     * commentario "TOLERA CONVERSIO_SIGNI_SEVERA: causa" (linea
     * eadem aut praecedens; causa OBLIGATORIA). IRRITUM = TOLERA
     * quod nihil absorbet aut sine causa aut codicem ignotum
     * nominat (lectio eslint: suppressiones putrescunt - quae nihil
     * absorbet signum est, non nihil). */
    EXAMEN_CODEX_CONVERSIO_SIGNI_SEVERA,      /* DOMESTICUM */
    EXAMEN_CODEX_TOLERA_IRRITUM,              /* DOMESTICUM */
    /* phasis II conversionis signi (2026-07-17): comparationes.
     * SIGNORUM = paritas -Wsign-compare (commune UAC insignatum,
     * latus signatum convertitur; constans non-negativa et
     * intervallum nonneg supprimunt - calibratio viva). VANA =
     * comparatio tautologica contra zephyrum (typus communis
     * insignatus AUT intervallum SANUM nonneg -> semper verum/
     * falsum; TOLERA-suppressibilis) - vexilla domus hanc classem
     * NUMQUAM tegunt et clang tautologica in macris supprimit
     * (ZEPHYRUM!): sic cohibitio mortua delineare vixit. */
    EXAMEN_CODEX_COMPARATIO_SIGNORUM,         /* DOMESTICUM */
    EXAMEN_CODEX_COMPARATIO_VANA,             /* DOMESTICUM */
    EXAMEN_CODEX_NUMERUS
} ExamenCodex;

/* Licentia assignationis (iudicium directionale 6.3.16.1) */
nomen enumeratio {
    EXAMEN_LICET = 0,          /* nulla conversio (typi identici) */
    EXAMEN_LICET_CONVERSIO,    /* licita cum conversione ad finem */
    EXAMEN_VETITUM             /* violatio - codex_out dicit quae */
} ExamenLicentia;

/* ==================================================
 * Semantica (contextus communis typus/forma/aestimator/scopus)
 * ================================================== */

/* Diagnosticum v2 (M4a chunk A): positio materializata TEMPORE
 * ADDITIONIS (ambulatio radicis lexematis; via per parsuram
 * ambulationis currentis). Vita viae = vita parsurae. */
nomen structura {
    constans SilvaNodus* nodus;        /* NIHIL licet */
    constans character*  causa;        /* litterae staticae (ex tabula);
                                        * MACRO_DOMESTICUM_IN_ALIENO in
                                        * piscina sem struit (nominat) */
    s32                  codex;        /* ExamenCodex */
    s32                  severitas;    /* ExamenSeveritas (ex tabula) */
    b32                  provisionale; /* sub AMBIGUO retento -
                                        * canonicum = coniectura */
    chorda               via;          /* radicis; vacua si ignota */
    i32                  linea;        /* 1-basata; 0 si ignota */
    i32                  columna;      /* 1-basata; 0 si ignota */
    i32                  longitudo;    /* octeti lexematis primi
                                        * (radicis); 0 si ignota */
    constans SilvaNodus* socius;       /* sedes cognata (e.g.
                                        * declaratio prior); NIHIL */
} SemanticaDiagnosticum;

/* Redditor typorum (LEGATUS chunk 0, ex sessione promotus): typus
 * -> textus latinus C-stili in buffer datum (NUL appenso).
 * Functiones = signatura "reditus(parametra)" (additio agitationis
 * legati); acies = "T[n]" / "T[]"; aggregata anonyma = "<anonyma>"
 * (typedef tamen tag anonymum BAPTIZAT titulo suo - agitatio
 * 2026-07-13). Redde octetos scriptos; 0 = irreddibilis (error /
 * spatium deficiens) - buffer tunc vacuus. */
insignatus integer silva_c89_typum_scribere (constans TypusC89* t,
    character* buffer, insignatus integer capacitas);

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

    /* parsura ambulationis currentis (M4a chunk A): analysare eam
     * ponit per ambulationem (systema, deinde usoris) - fons viae
     * diagnosticorum; NIHIL licet (positio tunc sine via) */
    constans SilvaParsura* parsura_currens;

    /* tabula alienitatis fontium (conversio signi, 2026-07-16):
     * pigre computata per parsuram (clavis = monstrator parsurae);
     * NIHIL = TU sine vendore. Iudicia DOMESTICA codicem alienum
     * numquam iudicant. */
    constans SilvaParsura* alieni_parsura;
    b32*                   fontes_alieni;
    i32                    fontes_alieni_numerus;

    /* tabula TOLERA (gradus severi, 2026-07-17): commentaria
     * suppressionis ex triviis lexematum pigre collecta per
     * parsuram; elementa ExamenTolera (in .c). Post ambulationem
     * inutilia IRRITUM flagrant. */
    constans SilvaParsura* tolerae_parsura;
    Xar*                   tolerae;
};

SilvaSemantica* silva_c89_semantica_creare (Piscina* piscina);

/* Diagnosticum addere (M4a chunk A): codex -> {causa, severitas} ex
 * tabula unica; positio/provisionale materializantur hic. Socius =
 * sedes cognata (declaratio prior, etc.). */
vacuum silva_c89_diagnosticum_addere (SilvaSemantica* sem,
    constans SilvaNodus* nodus, s32 codex);
vacuum silva_c89_diagnosticum_addere_cum_socio (SilvaSemantica* sem,
    constans SilvaNodus* nodus, s32 codex,
    constans SilvaNodus* socius);

/* ==================================================
 * Relatio compatibilitatis (examen chunk B)
 * ================================================== */

/* Relatio symmetrica C89 6.1.2.6, pura super typis internatis.
 * ERROR absorbet (compatibilis cum omnibus - regula recuperationis).
 * Quales ut copiae comparantur per gradum; monstrator = pointee
 * compatibilis QUALIBUS INCLUSIS; acies = elementum + sentinella
 * mensurae (-1 ignota); functio = reditus + parametra (mixta
 * prototypata/K&R: promotione ordinaria immota, non variadica);
 * tags nominales (identitas per declarans); enumeratus <-> int. */
b32 silva_c89_typi_compatibiles (TypusC89* a, TypusC89* b);

/* Iudicium directionale assignationis simplicis (6.3.16.1; idem
 * pro initiatore scalari / argumento prototypato / redde). Quales
 * summi finis exuuntur (conversio valoris); nodus_valoris pro
 * constante nulla (NIHIL licet). Redditur ExamenLicentia; sub
 * VETITO codex_out (NIHIL licet) codicem violationis fert. */
s32 silva_c89_assignationem_iudicare (SilvaSemantica* sem,
    constans SilvaNodus* nodus_valoris, TypusC89* finis,
    TypusC89* valoris, s32* codex_out);

/* Tabula codicum quaerenda (percursus census = consumptor primus;
 * effusor/vectis chunk D sequuntur). Codex extra fines: causa
 * "codex ignotus", severitas INFRA. */
constans character* silva_c89_codicis_causa (s32 codex);
s32 silva_c89_codicis_severitas (s32 codex);

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

/* Registratio typedef in scopum CURRENTEM (B: scopus-conscia).
 * Tag anonymum titulo typedef baptizat (vide _typedef_baptizare) */
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

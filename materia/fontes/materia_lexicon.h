/* materia_lexicon.h - Descriptor vocabularii lexicalis
 *
 * Materia genera NODORUM ut indices opacos in registrum tractat
 * (exemplar: silva_registrum.h). Genera LEXEMATUM eodem modo
 * tractanda sunt - et hoc caput illam symmetriam perficit.
 *
 * CUR EXSISTIT. Silva TRES tabulas manu servatas ferebat, unam
 * quaestionem varie respondentes:
 *
 *   ORTHOGRAPHIAE[]   silva_arbor.c:35            genus -> textus
 *   NOMINA_GENERUM[]  silva_token.c:356           genus -> nomen
 *   _textum_fert()    silva_canon_coquere.c:103   genus -> textum fert?
 *
 * Tertia exsistit quia ORTHOGRAPHIAE hic_manens est; probatio per
 * genera XCV currens eas ne divergant custodit. CUSTOS SUPER
 * DUPLICATIONEM POSITUS ABSTRACTIONEM DEESSE SIGNIFICAT.
 *
 * MENSURATUM 2026-08-27, ante furcam, in silva gelata:
 *   - sedes coniunctionis lexicalis XXXII in substrato; omnes
 *     his DUOBUS axibus (species, munus) respondentur
 *   - species==VERBATIM adsignata REGULA (ex ORTHOGRAPHIAE +
 *     commutatione _trivium_scribere) _textum_fert praedicit,
 *     listam manu scriptam in plagula ALIA: X genera, DIFFERENTIA
 *     NULLA super genera XCV
 *   - silva_arbor_valor_portandus ab ea divergit in generibus
 *     QUATTUOR praecise - SPATIA TABULAE NOVA_LINEA CONTINUATIO -
 *     id est exacte "carve-out triviorum" quem commentarium
 *     canon_coquere.c:100 nominat
 *
 * Modellum ergo non solum CONGRUIT ubi artificia congruunt sed
 * DIVERGENTIAM EXPLICAT ubi differunt. Oraculum separans.
 */

#ifndef MATERIA_LEXICON_H
#define MATERIA_LEXICON_H

#include "latina.h"


/* ==================================================
 * Species - QUOMODO octeti generis determinantur.
 *
 * Campus unus 'orthographia' quattuor modis legitur. Hoc est
 * cor descriptoris: quattuor species enumeratae substituunt
 * absentiam (orthographia==NIHIL) quae antea quaestioni
 * "an textum ferat" per casus speciales respondebat.
 * ================================================== */

nomen enumeratio {
    MATERIA_LEX_VERBATIM = 0,   /* contentum arbitrarium; ut textus latum.
                                 * orthographia NIHIL SEMPER. */
    MATERIA_LEX_FIXUM,          /* contentum = orthographia tota.
                                 * FINIS: orthographia "" (vacua, non NIHIL). */
    MATERIA_LEX_REPETITUM,      /* contentum = orthographia (character UNUS)
                                 * n vicibus. Numerus in STML ut n="4". */
    MATERIA_LEX_TERMINATOR,     /* contentum = orthographia, vel eius
                                 * variatio CRLF ('\r' ante '\n' ultimum).
                                 * Vexillum in STML ut crlf. */

    MATERIA_LEX_NUMERUS_SPECIERUM
} MateriaLexSpecies;


/* ==================================================
 * Munus - QUID genus in structura textus agit.
 *
 * Axis a specie INDEPENDENS: IDENTIFICATOR et COMMENTUM ambo
 * VERBATIM sunt sed munere differunt; SPATIA et NOVA_LINEA ambo
 * trivia sunt sed specie differunt. Ergo duo campi, non unus.
 *
 * LINEA lineam LOGICAM incipit; LAMINA physicam solam (C89
 * CONTINUATIO: linea physica crescit, logica perstat).
 * ================================================== */

nomen enumeratio {
    MATERIA_MUNUS_SUBSTANTIVUM = 0, /* contentum verum */
    MATERIA_MUNUS_SPATIUM,          /* spatium horizontale */
    MATERIA_MUNUS_LINEA,            /* terminator lineae logicae */
    MATERIA_MUNUS_LAMINA,           /* terminator physicus solus */
    MATERIA_MUNUS_COMMENTUM,
    MATERIA_MUNUS_FINIS,            /* EOF */

    MATERIA_MUNUS_NUMERUS_MUNERUM
} MateriaLexMunus;

/* Vexillum unius muneris, ad larvam capacitatis componendam. */
#define MATERIA_MUNUS_VEXILLUM(m) ((i32)((i32)I << (i32)(m)))


/* ==================================================
 * Genus unum
 * ================================================== */

nomen structura {
    constans character* titulus;       /* nomen; tag inde mangulatur */
    constans character* orthographia;  /* PER SPECIEM legendum */
    s32                 species;       /* MateriaLexSpecies */
    s32                 munus;         /* MateriaLexMunus */
} MateriaLexGenus;


/* ==================================================
 * Lexicon coctum - datum GENERATUM, semper constans.
 *
 * Larva capacitatis hic NON vivit: derivatur, ergo in
 * MateriaLexiconRatum vivit (infra), ne datum generatum
 * mutabile fieri debeat.
 * ================================================== */

nomen structura {
    constans MateriaLexGenus* genera;
    i32                       numerus_generum;  /* ET sentinella non-inventi */
    constans character*       praefixum_tagi;   /* e.g. "lex-" */
    s32                       genus_spatii;     /* SPATIUM canonicum;
                                                 * -I si lingua nullum habet */
} MateriaLexiconCoctum;


/* ==================================================
 * Vitia - porta CLAMAT, causam nominans
 * ================================================== */

nomen enumeratio {
    MATERIA_LEX_SANUM = 0,
    MATERIA_LEX_VITIUM_NIHIL,       /* lexicon aut genera NIHIL */
    MATERIA_LEX_VITIUM_NUMERUS,     /* numerus_generum ZEPHYRUM */
    MATERIA_LEX_VITIUM_PRAEFIXUM,   /* praefixum_tagi NIHIL */
    MATERIA_LEX_VITIUM_TITULUS,     /* genus sine titulo */
    MATERIA_LEX_VITIUM_SPECIES,     /* species extra fines */
    MATERIA_LEX_VITIUM_MUNUS,       /* munus extra fines */
    MATERIA_LEX_VITIUM_INVARIANS,   /* VERBATIM <=> orthographia NIHIL fractum */
    MATERIA_LEX_VITIUM_REPETITUM,   /* REPETITUM cuius unitas non character I */
    MATERIA_LEX_VITIUM_TERMINATOR,  /* TERMINATOR non in '\n' desinens */
    MATERIA_LEX_VITIUM_SPATIUM      /* genus_spatii pravum */
} MateriaLexVitium;

nomen structura {
    s32 vitium;   /* MateriaLexVitium */
    i32 genus;    /* index generis rei; numerus_generum si non ad genus pertinet */
} MateriaLexIudicium;


/* ==================================================
 * Lexicon ratum - forma post portam.
 *
 * POLITICA DEGRADATIONIS (decretum Fran 2026-08-27, optio B):
 * capacitas ad tempus ONERIS iudicatur, non per vocationem.
 * Adsignatio munerum linguae propria est et immutabilis; ergo
 * quaestio "an lingua lineas distinguat" CONSTANS est et semel
 * respondenda.
 *
 * Casus qui hoc poposcit: inventor blocci commentorum lineas
 * vacuas per munus LINEA numerat. CSS unum genus spatiorum habet
 * quod lineas novas absorbet (CSS_LEX_SPATIA = "spatia, tabulae,
 * lineae novae"), ergo nullum genus CSS munus LINEA fert. Sine
 * porta inventor omnes commentarios usque ad initium plagulae
 * congereret - RESPONSUM FALSUM FIDENTER DATUM, non error.
 *
 * Larva NON verdictum perpetuum de lingua est: dicit hoc lexicon
 * lineas non distinguere. Lingua id mutare potest lexando aliter.
 * ================================================== */

nomen structura {
    constans MateriaLexiconCoctum* lexicon;
    i32                            munera_praesentia;  /* larva vexillorum */
    b32                            ratum;
} MateriaLexiconRatum;


/* ==================================================
 * Porta
 * ================================================== */

/* Lexicon iudicare et ratum reddere. FALSUM = recusatum;
 * iudicium causam et genus reum nominat. */
b32
materia_lexicon_ratum_facere (
        MateriaLexiconRatum* ratum,
    constans MateriaLexiconCoctum* lexicon,
         MateriaLexIudicium* iudicium);

/* Nomen vitii ad nuntios. NUMQUAM NIHIL. */
constans character*
materia_lexicon_vitium_nomen (
    MateriaLexVitium vitium);


/* ==================================================
 * Capacitas
 * ================================================== */

b32
materia_lexicon_munus_habet (
    constans MateriaLexiconRatum* ratum,
              MateriaLexMunus  munus);

/* Larva postulatorum: OMNIA adesse debent. */
b32
materia_lexicon_munera_habet (
    constans MateriaLexiconRatum* ratum,
                            i32  munera_postulata);


/* ==================================================
 * Accessus - substituunt ORTHOGRAPHIAE, NOMINA_GENERUM,
 * _textum_fert, silva_arbor_valor_portandus
 * ================================================== */

constans character*
materia_lexicon_titulus (
    constans MateriaLexiconRatum* ratum,
                             s32  genus);

constans character*
materia_lexicon_orthographia (
    constans MateriaLexiconRatum* ratum,
                             s32  genus);

MateriaLexSpecies
materia_lexicon_species (
    constans MateriaLexiconRatum* ratum,
                             s32  genus);

MateriaLexMunus
materia_lexicon_munus (
    constans MateriaLexiconRatum* ratum,
                             s32  genus);

/* An genus contentum ut TEXTUM ferat. Aequivalens
 * species==VERBATIM; substituit et _textum_fert et
 * silva_arbor_valor_portandus (quae in generibus IV divergebant). */
b32
materia_lexicon_textum_fert (
    constans MateriaLexiconRatum* ratum,
                             s32  genus);

/* An genus trivium sit (munus SPATIUM|LINEA|LAMINA|COMMENTUM). */
b32
materia_lexicon_trivium_est (
    constans MateriaLexiconRatum* ratum,
                             s32  genus);

#endif /* MATERIA_LEXICON_H */

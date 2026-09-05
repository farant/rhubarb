/* materia_arbor.h - Arbor <-> STML canonicum
 *
 * Ambulatio grammaticae-IGNARA: forma ex registro cocto sumitur,
 * nulla scientia linguae hic. Quod frons privatum habet - campi
 * lexematis proprii, catenae originis - per UNCUM venit, numquam
 * per ramum in hoc modulo.
 *
 * ============================================================
 * DUAE SUTURAE, ET CUR DUAE
 * ============================================================
 *
 * MateriaOrigoUncus (materia_nodus.h) respondet quaestionibus de
 * SEDE et de RADICE emissionis - quae materia ipsa poscit ut
 * ancoram ponat et lexemata derivata agnoscat.
 *
 * MateriaArborFrons (hic) respondet quaestioni ALIA: quid frons in
 * DOCUMENTUM scribat quod materia nescit. C89 tria habet - campum
 * 'standard', scissuras, catenam originis nestatam - quorum nullum
 * materiae quicquam significat. Uncus dicit UBI lexema sit; frons
 * dicit QUID DE EO SCRIBENDUM SIT.
 *
 * Distinctio non aesthetica est: sedes materiae opus est (ancora
 * sine ea mentitur), ornamentum non est. Si ambo unus uncus essent,
 * lingua sine derivatione uncum implere deberet ut ancoram haberet.
 *
 * ============================================================
 * LEX ANCORAE (portata ex silva_arbor.c:715-760, MENSURATA)
 * ============================================================
 *
 * ANCORA EST UBI EMISSIO INCIPIT, NON UBI LEXEMA EST. Emissio
 * TRIVIIS DUCENTIBUS incipit, ergo si lexema primum spatia_ante
 * fert, ancora ex TRIVIO PRIMO sumenda est. Aliter lector cursorem
 * ad lexema ponit, deinde trivia ante id emittit, et sedes omnes
 * longitudine indentationis labuntur.
 *
 * Silva id mensuravit: lexemate solo adhibito, CLXXVIII
 * divergentiae 'lexema/offset' super corpus. Probatio parva id NON
 * cepit, quia 'int n = 0;' lexema primum ad offset 0 sine ullo
 * trivio ducente habet - casus in quo vitium EVANESCIT. Portatur
 * hic cum eadem cautione: probatio quae ancoram sine triviis
 * ducentibus solum probat NIHIL de hac lege dicit.
 */

#ifndef MATERIA_ARBOR_H
#define MATERIA_ARBOR_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "stml.h"
#include "internamentum.h"
#include "materia_token.h"
#include "materia_nodus.h"
#include "materia_registrum.h"
#include "materia_lexicon.h"

#define MATERIA_ARBOR_TAG_CAPACITAS   64
#define MATERIA_ARBOR_TAG_ANTE        "ante"
#define MATERIA_ARBOR_TAG_POST        "post"
#define MATERIA_ARBOR_TAG_ENVOLUCRI   "arbor"
#define MATERIA_ARBOR_SIGILLI_LONGITUDO 8


/* ==================================================
 * Fructus
 * ================================================== */

nomen structura {
    constans vacuum* clavis;      /* MateriaNodus* aut MateriaToken* */
                b32  est_lexema;
                i32  initium;     /* offset octetorum, INCLUSIVUS */
                i32  finis;       /* offset octetorum, EXCLUSIVUS */
} MateriaArborSedes;

/* Census compressionis - ambulatio tegumentum SUUM metiatur.
 * Familia templorum mortua exitum VALIDUM incompressum relinquit:
 * portae octetorum virides manent, sola magnitudo crescit. Ergo
 * PRAESENTIA asserenda est, numquam numeri pincti. */
nomen structura {
    i32 spatia_vocationes;
} MateriaArborCensus;

nomen structura {
                       b32  successus;
                    chorda  textus;   /* octeti STML; vacua in fractura */
        constans character* causa;
     constans MateriaNodus* sedes;
                       Xar* sedes_valorum;   /* MateriaArborSedes */
        MateriaArborCensus  census;
} MateriaArborScriptura;


/* ==================================================
 * Scriptor - opacus; frons per accessus agit
 * ================================================== */

nomen structura MateriaArborScriptor MateriaArborScriptor;
nomen structura MateriaArborLector   MateriaArborLector;

Piscina*
materia_arbor_scriptor_piscina (
    MateriaArborScriptor* scriptor);

InternamentumChorda*
materia_arbor_scriptor_intern (
    MateriaArborScriptor* scriptor);

/* Causam fracturae ponere (prima vincit, ut in silva). */
vacuum
materia_arbor_scriptor_recusare (
    MateriaArborScriptor* scriptor,
      constans character* causa);

/* Attributum numeri - frons id saepissime poscit; sine eo omnis
 * frons decimale suum sine stdio rescriberet. */
b32
materia_arbor_attributum_numeri (
    MateriaArborScriptor* scriptor,
               StmlNodus* elementum,
      constans character* titulus,
                     i32  numerus);

/* SUTURA RECURSIONIS. Catenae originis lexemata INTRA lexemata
 * nestant (C89: <expansio> invocationem suam plenam fert, cum
 * triviis), ergo frons scriptorem/lectorem lexematis RE-INTRARE
 * potest. Sine his uncus 'liberos_ornare' elementum lexematis
 * fabricare NON posset et frons scriptorem suum duplicaret -
 * secunda descriptio quae a prima divergeret.
 *
 * Fragmenta et transclusiones per has vias UNA machina tractantur:
 * lexema iam scriptum transclusionem reddit, identitate servata. */
StmlNodus*
materia_arbor_lexema_scribere (
    MateriaArborScriptor* scriptor,
   constans MateriaToken* lexema);

MateriaToken*
materia_arbor_lexema_legere (
    MateriaArborLector* lector,
             StmlNodus* elementum,
                chorda* fragmenti_id);

/* Tag lexematis ex genere mangulatum (praefixum lexici + titulus
 * minusculus, '_' -> '-'). ZEPHYRUM = genus ignotum. */
i32
materia_arbor_lexema_tag (
    constans MateriaLexiconRatum* ratum,
                             s32  genus,
                       character* buffer,
                             i32  capacitas);

/* Inversum, per mangulationem ANTRORSAM omnium generum - ergo
 * directiones divergere NON possunt. Reddit numerus_generum si
 * ignotum. */
s32
materia_arbor_lexema_ex_tag (
    constans MateriaLexiconRatum* ratum,
              constans character* tag,
                             i32  mensura);


/* Cursor: Frons utramque partem (scripturam et lectionem) tangit,
 * ergo ante utramque stare debet. */
nomen structura {
    s32 offset;
    i32 linea;
    i32 columna;
    b32 post_lineam;   /* nova linea LOGICA visa */
    b32 sedes_notae;   /* ancora adest: ordinatas scribere licet */
} MateriaArborCursor;


/* ==================================================
 * Frons - quod lingua de lexemate scribit quod materia nescit
 * ================================================== */

nomen structura {
    vacuum* datum;

    /* Passus I: lexemata quae ORIGO huius nominat quoque numeranda
     * sunt (invocatio una plura lexemata expansa gignere potest;
     * si quodque suam copiam scriberet, identitas duplicata
     * mentiretur). Frons ea per 'numerare' reddit. NIHIL = nulla. */
    vacuum (*origo_numerare)(
        vacuum* datum,
        constans MateriaToken* lexema,
        vacuum (*numerare)(vacuum* ctx, constans MateriaToken*),
        vacuum* ctx);

    /* Passus II, DUO uncus quia ORDO OCTETIM VISIBILIS EST.
     *
     * Attributa et liberi listae SEPARATAE sunt, ergo unus uncus
     * post omnia non sufficeret: silva 'standard' ANTE 'f' scribit
     * et scissuras/originem POST <post>. Attributa igitur PRIMA
     * vocantur, liberi ULTIMI, et ordo intra utramque listam
     * silvae congruit.
     *
     * Ordo attributorum: [frons] standard · [materia] f, b, linea,
     *   columna, linea-initium
     * Ordo liberorum:    [materia] ante, textus, post ·
     *   [frons] scissurae, origo
     *
     * Uterque FALSUM in fractura; causam per
     * materia_arbor_scriptor_recusare ponat. NIHIL = nihil addendum. */
    b32 (*attributa_ornare)(
        vacuum* datum,
        MateriaArborScriptor* scriptor,
        StmlNodus* elementum,
        constans MateriaToken* lexema);

    b32 (*liberos_ornare)(
        vacuum* datum,
        MateriaArborScriptor* scriptor,
        StmlNodus* elementum,
        constans MateriaToken* lexema);

    /* --- lectio, specula scripturae --- */

    /* Attributa frontis propria ex elemento (C89: standard). */
    b32 (*attributa_legere)(
        vacuum* datum,
        MateriaArborLector* lector,
        constans StmlNodus* elementum,
        MateriaToken* lexema);

    /* Liberum quod materia non agnoscit (C89: scissura, expansio,
     * pasta, stringificatio, api). Tri-status - vide
     * MateriaLectioFructus. */
    s32 (*liberum_legere)(
        vacuum* datum,
        MateriaArborLector* lector,
        constans StmlNodus* liberum,
        MateriaToken* lexema);

    /* Cursorem per octetos lexematis movere. VERUM = frons id egit
     * (C89: scissurae laminas reinserunt, ergo plus quam valor);
     * FALSUM = materia valorem simpliciter percurrat. */
    b32 (*cursorem_movere)(
        vacuum* datum,
        MateriaArborCursor* cursor,
        constans MateriaToken* lexema);

        /* Post arborem lectam et positiones derivatas. NIHIL licet. */
    b32 (*perficere)(
        vacuum* datum,
        MateriaArborLector* lector,
        MateriaNodus* radix);

    /* SCRIPTURA NODI: attributa frontis in elemento NODI (non
     * lexematis), post creationem elementi, ante locos. Lector
     * attributa nodi IGNORAT - derivata sunt, arbor ex locis
     * reconstruitur (oratio: classes/linguae vocabuli ut selectio
     * '[classes~=verbum]' congruat). Uncus substrati secundus post
     * md (oratio gradus III, 2026-09-04). NIHIL = nihil addendum;
     * campus ULTIMUS ut initiatores positionales vetusti maneant. */
    b32 (*nodum_ornare)(
        vacuum* datum,
        MateriaArborScriptor* scriptor,
        StmlNodus* elementum,
        constans MateriaNodus* nodus);
} MateriaArborFrons;


/* ==================================================
 * Sigillum registri
 * ================================================== */

chorda
materia_arbor_sigillum (
                            Piscina* piscina,
    constans MateriaRegistrumCoctum* tabularium);

s32
materia_arbor_genus_index (
    constans MateriaRegistrumCoctum* tabularium,
                 constans character* titulus,
                                i32  mensura);


/* ==================================================
 * Consilium + ingressus
 * ================================================== */

nomen structura {
    constans MateriaRegistrumCoctum* tabularium;
       constans MateriaLexiconRatum* lexicon;
                 constans character* grammatica;  /* nomen in involucro */
          constans MateriaOrigoUncus* origo;      /* NIHIL licet */
           constans MateriaArborFrons* frons;     /* NIHIL licet */
                InternamentumChorda*  intern;     /* NIHIL = pigre */
                                 b32  templa_activa;
                                 b32  sedes_colligere;

    /* LECTIO: Xar de MateriaLacuna, per offset ORDINATUS. NIHIL
     * licet (casus subarboris). */
                                Xar* lacunae;

    /* LECTIO: forma lexematum creandorum. Sine hac lector lexemata
     * SINE cauda creat et frons quod legit reponere NON habet -
     * uncus 'liberum_legere' scribere posset sed non conservare.
     * Hiatus API inventus dum shim C89 scriberetur: probatio quae
     * unum latus solum exercet eum non ostendisset. */
                  MateriaTokenForma forma;
} MateriaArborConsilium;

vacuum
materia_arbor_consilium_nudum (
              MateriaArborConsilium* consilium,
    constans MateriaRegistrumCoctum* tabularium,
       constans MateriaLexiconRatum* lexicon,
                 constans character* grammatica);

MateriaArborScriptura
materia_arbor_scribere_nodum (
                           Piscina* piscina,
             constans MateriaNodus* nodus,
    constans MateriaArborConsilium* consilium);


/* ==================================================
 * LECTOR: STML canonicum -> arbor
 * ================================================== */

nomen structura {
    constans character* causa;   /* NIHIL = sanum */
                   i32  linea;   /* linea documenti */
} MateriaArborVitium;

/* LACUNA: intervallum octetorum quod ARBORI non pertinet sed in
 * fluxu INTER lexemata eius iacet. Gemella reinserendorum
 * (materia_scribere.h) in latere LECTIONIS: illa octetos alienos
 * SCRIBIT, haec cursorem trans eos SALIRE facit.
 *
 * CUR OPUS SIT: arbor octetos suos CONTIGUOS emittit, sed in
 * plagula vera lexemata eius interrumpi possunt. Sine lacunis
 * lexema post interruptionem sedem nimis parvam accipit.
 *
 * FONS NECESSARIUS: offset sine fonte SENSU CARET - octetus CXCII
 * plagulae II et octetus CXCII plagulae VI nihil commune habent.
 * Silva id dura via didicit (MENSURATUM: IV plagulae, delta
 * DCCCXXXV, quia lacuna plagulae principis lexemati capitis
 * applicabatur). */
nomen structura {
    s32 offset;                 /* initium */
    s32 finis;                  /* post ultimum octetum */
    i32 linea_finalis;
    i32 columna_finalis;
    b32 post_lineam_finalis;
    s32 fons;                   /* -I = quaelibet */
} MateriaLacuna;

/* Cursor derivationis (MateriaArborCursor) supra declaratus est:
 * documentum positiones NON fert - involucrum ancoram solam - ergo
 * eas hic reficimus. Lex: documentum canonicum mentiri non possit.
 * PUBLICUS quia frons eum movere potest (C89: scissurae plus quam
 * valorem tegunt). */

Piscina*
materia_arbor_lector_piscina (
    MateriaArborLector* lector);

/* Semper FALSUM reddit, ut vocantes 'redde ...recusare(...)'
 * scribere possint. PRIMA causa vincit - profundissima est et
 * proxima vero vitio. */
b32
materia_arbor_lector_recusare (
     MateriaArborLector* lector,
     constans character* causa,
                    i32  linea);

/* Numerum decimalem ex chorda attributi; FALSUM si non totus. */
b32
materia_arbor_numerus_ex_chorda (
    constans chorda* valor,
                i32* exitus);

/* Tri-status: frons 'ignotum' a 'fractum' DISCERNERE debet -
 * aliter materia elementum ignotum tacite acciperet aut frontis
 * vitium ut suum nuntiaret. */
nomen enumeratio {
    MATERIA_LECTIO_IGNOTUM = 0,  /* frons hoc non agnoscit */
    MATERIA_LECTIO_ACCEPTUM,     /* agnitum et lectum */
    MATERIA_LECTIO_FRACTUM       /* agnitum sed pravum */
} MateriaLectioFructus;

MateriaNodus*
materia_arbor_legere (
                           Piscina* piscina,
               InternamentumChorda* intern,
                            chorda  textus,
    constans MateriaArborConsilium* consilium,
                MateriaArborVitium* vitium);


/* ==================================================
 * COMPARATOR ARBORUM (portatus ex silva_arbor_aequalitas.c)
 *
 * Oraculum SECUNDUM praeter octetos: dislocatio dominii triviorum
 * (commentarium ex 'post' prioris in 'ante' sequentis migrans)
 * fluxum octetorum EXACTE servat - sola arbor eam videt.
 *
 * MODI, MENSURATI ex silva (non praesumpti): STRUCTURALIS confert
 * genus nodi, numerum locorum, genus valoris per locum, listas per
 * mensuram PROSPECTUS (numquam xar_numerus repositorii - commune
 * inter furcas est), lexemata per genus + octetos valoris +
 * fons_index + initium_lineae + PROVENIENTIAM (signum byte_offset,
 * utroque modo - custodia quae aliter transiret quia subiectum
 * abesset), seriem triviorum (genus + valor, ORDINE, ambae
 * series). FIDELITAS his addit: byte_offset, linea, columna.
 *
 * QUAE SILVA CONFERT ET HIC ABSUNT, CONSULTO: 'standard',
 * 'scissurae', 'longitudo' - in cauda frontis vivunt, cuius formam
 * materia non videt. Sedes descensus NOMINATA: uncus comparationis
 * frontis, phasis V (migratio C89 eum poscet; CSS et HTML numquam).
 *
 * PATER: nullitas sola, et INTERIORIBUS solis (profunditas > 0).
 * Radices conferre CIX divergentias falsas dedit - subarbor electa
 * patrem habet, arbor seorsum lecta habere non potest.
 *
 * QUOD NON VIDET, CONSULTO: dominium GEMINUM triviorum (lexema in
 * duabus seriebus spatia_*) - id oraculum octetorum capit. Noli
 * hunc comparatorem 'emendare' ut id capiat: oracula duo classes
 * defectuum DISIUNCTAS tegunt.
 * ================================================== */

#define MATERIA_ARBOR_VIA_CAPACITAS 256

nomen enumeratio {
    MATERIA_ARBOR_COMPARATIO_STRUCTURALIS = 0,
    MATERIA_ARBOR_COMPARATIO_FIDELITAS
} MateriaArborComparatioModus;

/* Divergentia PRIMA nominata. Booleanum solum inutile est -
 * 'inaequales' sine campo et via bisectionem manualem petit. */
nomen structura {
      constans character* campus;    /* nomen campi divergentis */
   constans MateriaNodus* nodus_a;   /* nodi continentes */
   constans MateriaNodus* nodus_b;
   constans MateriaToken* lexema_a;  /* NIHIL nisi lexicalis */
   constans MateriaToken* lexema_b;
                     s32  locus;     /* index loci; -I absens */
                     s32  index;     /* index seriei; -I absens */
               character  via[MATERIA_ARBOR_VIA_CAPACITAS];
} MateriaArborDifferentia;

/* Arbores duas conferre. VERUM si aequales. differentia NIHIL
 * licet; alioquin in inaequalitate impletur (in aequalitate
 * campus = NIHIL ponitur). */
b32
materia_arbor_aequalis (
          constans MateriaNodus* a,
          constans MateriaNodus* b,
    MateriaArborComparatioModus  modus,
        MateriaArborDifferentia* differentia);

/* Patres omnium filiorum interiorum ponere (ambulatio generalis).
 * Lector eam ipse adhibet; parsatores linguarum eam post
 * constructionem vocent ut politicae reconstructionis congruant -
 * aliter comparator arbores parsatas contra lectas falso
 * divergentes nuntiat (nodus/pater-nullitas). */
vacuum
materia_arbor_patres_figere (
         Piscina* piscina,
    MateriaNodus* radix);

#endif /* MATERIA_ARBOR_H */

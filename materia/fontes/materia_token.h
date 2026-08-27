/* materia_token.h - Lexema substrati
 *
 * UNUM typum lexematis materia fert. Quod frons privatum habet
 * (origo macro, laminae, standard, quidquid) in CAUDAM eiusdem
 * allocationis it - materia mensuram scit, formam numquam.
 *
 * CUR CAUDA, NON PUNCTATOR. silva_token.h:9 decisionem notat quam
 * generatio praecedens carissime emit: "origo INSERTA (non allocata
 * separatim): numquam NIHIL, localitas melior" - quia arbor2
 * Lexema/Token scidit, et scissio permisit ut origo ad limitem
 * parsatoris PERDERETUR. Punctator opacus illam proprietatem in
 * consuetudinem verteret; cauda eam STRUCTURALEM servat, et
 * allocationem UNAM, dum materia formam ignorat.
 *
 * MENSURA (macOS/clang, 2026-08-27):
 *   SilvaToken hodiernus         CXXVIII octeti
 *   MateriaToken                 LXIV     - dimidium
 *   C89 per materiam    LXIV + XLVIII cauda = CXII, allocatione UNA
 *
 * Quae quattuor res ex lexemate abeunt, et cur nunc gratis:
 *   origo (XLVIII)  RP XVI · scissurae (VIII) RP XVII
 *   longitudo       RP XVII · standard
 * RP XVI DILATA est obiectione sine responso: "lucrum infra II% dum
 * reparatio quinque constructores omnes tangit et omnem consumptorem
 * originis". Obiectio ad FURCAM NON PERTINET - phasis I.3 illos
 * ipsos tangit UTIQUE. Sumptus mersus, lucrum gratis.
 *
 * LONGITUDO ABEST CONSULTO, ET NON QUIA REDUNDANS EST. RP XVII eam
 * redundantem vocat (aequalis valor.mensura in LVI,DCLXXXVI ex
 * LVI,DCLXXXVI). Mensura vera, conclusio periculosa:
 * silva_lexema.c:349 ponit longitudo = positus - initium cum
 * commentario "longitudo CRUDA" - spatium FONTIS crudum, dum
 * valor.mensura valorem MUNDUM mensurat. Differunt praecise cum
 * lexema laminas continet (int\<nl>x). Aequales erant quia scissurae
 * ZERO ex LVI,DCLXXXVI fuerunt in illo corpore - non quia idem
 * significant. Ergo longitudo cum scissuris SIMUL in caudam it.
 *
 * SPATIUM FONTIS: materia valor.mensura tenet. Frons quae laminas
 * habet spatium suum ex cauda computet; materia id fingere NON
 * potest et non conatur.
 *
 * TRIVIA EXACTA, NON XAR (RP XV). Xar unius elementi DCCIV octetos
 * poscit ut VIII servet. Praecondicio in codice probata et fortior
 * quam RP dicit: silva_lexema.c:950-1000 trivia in Xar scratch
 * cumulat, deinde 'divisio' computat - ergo NUMERUS UTRIUSQUE PARTIS
 * ANTE ALLOCATIONEM NOTUS EST, et ansa copiae iam agitur. Allocatio
 * exacta SIMPLICIOR est quam quod adest.
 */

#ifndef MATERIA_TOKEN_H
#define MATERIA_TOKEN_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"


/* ==================================================
 * Vexilla - bit, non octeti quattuor pro uno.
 * CAUDA vexillum est ut materia_token_cauda punctatorem
 * ultra allocationem numquam reddat.
 * ================================================== */

#define MATERIA_TOKEN_INITIUM_LINEAE  ((i32)I)
#define MATERIA_TOKEN_CAUDA           ((i32)II)


/* ==================================================
 * Forma - quod frons de omni lexemate suo declarat
 * ================================================== */

nomen structura {
    i32 mensura_caudae;   /* octeti privati frontis; ZEPHYRUM = nulli */
} MateriaTokenForma;


/* ==================================================
 * MateriaToken - LXIV octeti
 * ================================================== */

nomen structura MateriaToken MateriaToken;

structura MateriaToken {
    chorda         valor;           /* visus in textum; NON nul-terminatus */
    MateriaToken** spatia_ante;     /* series EXACTA; NIHIL si nulla */
    MateriaToken** spatia_post;
    i32            numerus_ante;
    i32            numerus_post;
    s32            genus;           /* index in lexicon; vide materia_lexicon.h */
    s32            byte_offset;     /* intra fontem; -I si syntheticum */
    i32            linea;           /* I-basata */
    i32            columna;         /* I-basata */
    s32            fons_index;      /* in tabulam fontium */
    i32            vexilla;         /* MATERIA_TOKEN_* */
};


/* ==================================================
 * Constructio
 * ================================================== */

/* Lexema novum. Cauda (si forma eam poscit) in EADEM allocatione
 * sequitur, ordinata et ad zephyrum posita. */
MateriaToken*
materia_token_creare (
                       Piscina* piscina,
    constans MateriaTokenForma* forma,
                            s32 genus,
                         chorda valor,
                            s32 byte_offset,
                            i32 linea,
                            i32 columna,
                            s32 fons_index);

/* Datum privatum frontis. NIHIL si forma nullam caudam poposcit -
 * ergo punctator ultra allocationem reddi NON potest. */
vacuum*
materia_token_cauda (
    constans MateriaToken* token);


/* ==================================================
 * Trivia - SEMEL posita, numquam appensa
 * ================================================== */

/* Seriem exactam allocat et copiat. numerus ZEPHYRUM licet
 * (punctator NIHIL, numerus ZEPHYRUM). */
b32
materia_token_trivia_ante_ponere (
        MateriaToken* token,
             Piscina* piscina,
        MateriaToken** fonte,
                  i32  numerus);

b32
materia_token_trivia_post_ponere (
        MateriaToken* token,
             Piscina* piscina,
        MateriaToken** fonte,
                  i32  numerus);


/* ==================================================
 * Vexilla
 * ================================================== */

b32
materia_token_initium_lineae (
    constans MateriaToken* token);

vacuum
materia_token_initium_lineae_ponere (
    MateriaToken* token,
              b32 valor);

#endif /* MATERIA_TOKEN_H */

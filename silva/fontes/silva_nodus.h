/* silva_nodus.h - Nodus uniformis + valor signatus (spec-v2 §9.1, S21)
 *
 * FACIES MATERIAE (phasis V, silva-migratio T8, 2026-09-25): nodus et
 * valor silvae SUNT MateriaNodus et MateriaValor - campi nominibus
 * idem (genus numerus_locorum loci pater; genus datum.{nodus token
 * lista index}), constructores et verba eadem semantica (materia ex
 * hoc modulo portata est). Hic nomina silvae SOLA manent, ut
 * sedes circa septem milia (silva et consumptores, mensuratae T8)
 * intactae transeant; ea deleri possunt cum sigillo (T13) aut
 * purgatione (gradus VI).
 *
 * SilvaNodus MACRO est, non nomen: officina 'structura SilvaNodus'
 * ut tabulam opacam scribit (XIII sedes) et contra AMALGAMA GELATAM
 * quoque compilat, ubi structura SilvaNodus vera est. Macro utrumque
 * servat; nomen structuram novam incompletam crearet.
 *
 * PATER (S26/S27): materia_nodus_ponere/appendere patrem filii in
 * constructione figunt; silva numquam figebat (furcae GLR vivae
 * subarbores communicant). Innocuum: silva_commissio omnem nodum
 * attingibilem desuper refigit (radices bracchiorum non canonicorum
 * ad NIHIL) - patres post commissionem idem sunt. Politica bracchii
 * non canonici (silva NIHIL, lector materiae totam arborem parentat)
 * ad T10 nominata manet (silva_frons.c).
 *
 * Quae silvae PROPRIA manent (infra): quinque familiae quaestionum
 * quae catenam ORIGINIS C89 per silva_token_radix ambulant -
 * extensio, extensio linearum, puritas fontis, geometria fida,
 * commentarium ducens.
 */

#ifndef SILVA_NODUS_H
#define SILVA_NODUS_H

#include "latina.h"
#include "materia_nodus.h"
#include "silva_token.h"


/* ==================================================
 * Facies: nomina silvae -> materia
 * ================================================== */

#define SilvaNodus               MateriaNodus
nomen MateriaValor               SilvaValor;
nomen MateriaValorGenus          SilvaValorGenus;
nomen MateriaLocusSpecies        SilvaLocusSpecies;
nomen MateriaListaProspectus     SilvaListaProspectus;

#define SILVA_VALOR_NIHIL        MATERIA_VALOR_NIHIL
#define SILVA_VALOR_NODUS        MATERIA_VALOR_NODUS
#define SILVA_VALOR_TOKEN        MATERIA_VALOR_TOKEN
#define SILVA_VALOR_LISTA        MATERIA_VALOR_LISTA
#define SILVA_VALOR_INDEX        MATERIA_VALOR_INDEX

#define SILVA_LOCUS_NODUS        MATERIA_LOCUS_NODUS
#define SILVA_LOCUS_TOKEN        MATERIA_LOCUS_TOKEN
#define SILVA_LOCUS_LISTA_NODUS  MATERIA_LOCUS_LISTA_NODUS
#define SILVA_LOCUS_LISTA_TOKEN  MATERIA_LOCUS_LISTA_TOKEN
#define SILVA_LOCUS_LISTA_MIXTA  MATERIA_LOCUS_LISTA_MIXTA
#define SILVA_LOCUS_INDEX        MATERIA_LOCUS_INDEX

#define silva_valor_nihil            materia_valor_nihil
#define silva_valor_nodus            materia_valor_nodus
#define silva_valor_token            materia_valor_token
#define silva_valor_index            materia_valor_index
#define silva_valor_lista            materia_valor_lista
#define silva_valor_lista_nova       materia_valor_lista_nova
#define silva_valor_lista_appendere  materia_valor_lista_appendere
#define silva_valor_lista_numerus    materia_valor_lista_numerus
#define silva_valor_lista_obtinere   materia_valor_lista_obtinere
#define silva_nodus_creare           materia_nodus_creare
#define silva_nodus_ponere           materia_nodus_ponere
#define silva_nodus_appendere        materia_nodus_appendere
#define silva_valor_congruit         materia_valor_congruit
#define silva_nodus_liberi           materia_nodus_liberi


/* ==================================================
 * Quaestiones originis C89 (silvae propriae)
 * ================================================== */

/* Extensio fontis (LEGATUS chunk 0, ex sessione promota): min/max
 * octetorum super lexemata subarboris in fonte dato, per RADICEM
 * originis (lexemata expansa synthetica - byte_offset -1 - omissa;
 * sedes invocationis numeratur). *minimum initia < 0, *maximum
 * initia 0; *minimum manet < 0 si nihil inventum. */
/* <contractus param="minimum" modus="accumulat"/>
 * <contractus param="maximum" modus="accumulat"/> */
vacuum
silva_valor_extensionem (
    SilvaValor  valor,
           s32  fons_index,
           s32* minimum,
           s32* maximum);

/* <contractus param="minimum" modus="accumulat"/>
 * <contractus param="maximum" modus="accumulat"/> */
vacuum
silva_nodus_extensionem (
    constans SilvaNodus* nodus,
                    s32  fons_index,
                    s32* minimum,
                    s32* maximum);

/* Variantia linearum (pro LSP): initium = minimum (linea,columna)
 * lexicographicum, finis = maximum (linea, columna+longitudo -
 * approximatio uni-linearis finis lexematis). Omnia 1-basata;
 * exitus intus zerantur; *linea_a == 0 post reditum = nihil
 * inventum. */
vacuum
silva_nodus_extensionem_lineis (
    constans SilvaNodus* nodus,
                    s32  fons_index,
                    i32* linea_a,
                    i32* columna_a,
                    i32* linea_b,
                    i32* columna_b);

/* Puritas fontis: VERUM si lexemata subarboris OMNIA origine FONS
 * sunt (stratum 0 - nihil ex expansione/pasta/stringificatione/
 * API). fons_index >= 0: lexemata plagulae datae esse debent;
 * < 0 = plagula quaelibet. Subarbor sine lexematis = VERUM
 * (vacue). Geometria subarboris purae ipsa lexemata sunt -
 * extensiones sine mendacio ullo. CAUTIO: in codice latinizato
 * verba clavium (si->if) expansa sunt - sententiae fere numquam
 * purae; vide geometria_fida pro quaestione laxiore. */
b32
silva_valor_est_fons_purus (
    SilvaValor valor,
           s32 fons_index);

b32
silva_nodus_est_fons_purus (
    constans SilvaNodus* nodus,
                    s32  fons_index);

/* Geometria fida: VERUM si sedes fontis (per radicem originis)
 * lexematum subarboris DISTINCTAE sunt. Expansio 1:1 (macros
 * latinae: si->if) fida manet - lexema unum, sedes invocationis
 * una et vera; expansio 1:N lexemata plura ad sedem UNAM collabit
 * et geometriam mentitur (extensiones verisimiles sed degeneres -
 * venationes formatoris 2026-08-19). Lexema radice synthetica
 * (byte_offset < 0, e.g. origo API) = FALSUM statim, sedes
 * inscibilis. Lexemata radice in plagula alia (fons_index >= 0)
 * omittuntur - geometriae plagulae datae non pertinent. Lexema
 * IDEM bis visum (bracchia ambigua subarbores communicant)
 * collapsus NON est - identitas lexematis intra sedem comparatur.
 * Piscina pro tabula sedium efficaci (quaestio pura, arbor
 * intacta). */
b32
silva_valor_geometria_fida (
       Piscina* piscina,
    SilvaValor  valor,
           s32  fons_index);

b32
silva_nodus_geometria_fida (
                Piscina* piscina,
    constans SilvaNodus* nodus,
                    s32  fons_index);

/* Commentarium ducens: bloccus commentorum "arcte-supra" nodum
 * (regula arbor2-comment-spec: bloccus contiguus sine linea vacua
 * inter finem eius et nodum; linea vacua intra = bloccus superior
 * cadit). Extenta BYTES in fonte dato, radice originis soluta
 * (declarationes macris initiatae: invocatio trivia fert).
 * Redde I si praesens (vista impleta), ZEPHYRUM si absens. */
nomen structura
{
                   s32 initium;   /* BYTES in fonte; -1 = absens */
                   s32 finis;     /* exclusivum; -1 = absens */
    insignatus integer linea;     /* 1-basata (commenti primi) */
} SilvaCommentariumVista;

integer
silva_commentarium_ducens (
       constans SilvaNodus* nodus,
                       s32  fons_index,
    SilvaCommentariumVista* vista);

#endif /* SILVA_NODUS_H */

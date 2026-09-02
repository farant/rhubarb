/* silva_nodus.h - Nodus uniformis + valor signatus (spec-v2 §9.1, S21)
 *
 * Nodus = caput fixum + series locorum signatorum per genus. NULLA
 * unio per genus (arbor2 cuique nodo pretium bracchi maximi imputabat
 * - R5). Accessores generati + children() tabulis ducti. Haec est
 * repraesentatio codex-ut-datorum quam solarium petit.
 *
 * PURITAS (S26/S27): constructiones GLR speculativae sunt - pater
 * NUMQUAM in constructione assignatur (furcae vivae communem subarborem
 * habere possunt); passus post-acceptum eum figit. Campi spatii/fontis
 * veniunt cum fluxu vero (Phase 4).
 *
 * S32: silva_nodus_ponere signum valoris contra speciem loci probat et
 * semel-tantum-scribere imponit - errores annotationum in prima parsura
 * apparent, non ut arbores corruptae.
 */

#ifndef SILVA_NODUS_H
#define SILVA_NODUS_H

#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "silva_token.h"


/* ==================================================
 * Species loci (vocabularium formae v0)
 * ================================================== */

nomen enumeratio {
    SILVA_LOCUS_NODUS = 0,
    SILVA_LOCUS_TOKEN,
    SILVA_LOCUS_LISTA_NODUS,
    SILVA_LOCUS_LISTA_TOKEN,
    SILVA_LOCUS_LISTA_MIXTA,
    SILVA_LOCUS_INDEX
} SilvaLocusSpecies;


/* ==================================================
 * Valor signatus (contractus reductionum GLR)
 * ================================================== */

nomen structura SilvaNodus SilvaNodus;

nomen enumeratio {
    SILVA_VALOR_NIHIL = 0,
    SILVA_VALOR_NODUS,
    SILVA_VALOR_TOKEN,
    SILVA_VALOR_LISTA,    /* Xar de SilvaValor (per valorem) */
    SILVA_VALOR_INDEX
} SilvaValorGenus;

nomen structura SilvaValor SilvaValor;

/* Prospectus listae (A½, spec-v2 par 12.1): valor listae est PROSPECTUS
 * {repositorium, mensura} - forma chordae pro valoribus. Repositorium
 * append-only; prospectus mensuram SUAM fert, ergo furca quae ultra
 * prospectum meum scripsit me non laedit (appendere purum infra:
 * in-loco si prospectus ad finem vivum stat, alioquin copia-in-
 * divergentia). Numquam xar_numerus in repositorio lege - mensuram
 * prospectus semper adhibe (silva_valor_lista_numerus/obtinere). */
nomen structura {
    Xar* xar;       /* repositorium commune (append-only in usu) */
    i32  mensura;   /* quot elementa HUIC prospectui pertinent */
} SilvaListaProspectus;

structura SilvaValor {
    SilvaValorGenus genus;
    unio {
                  SilvaNodus* nodus;
                  SilvaToken* token;
        SilvaListaProspectus  lista;
                         s32  index;
    } datum;
};


/* ==================================================
 * Nodus uniformis
 * ================================================== */

structura SilvaNodus {
           s32  genus;            /* index in registro generum */
           i32  numerus_locorum;
    SilvaValor* loci;             /* series signata (layout per genus) */
    SilvaNodus* pater;            /* post-acceptum SOLUM (S27) */
};


/* ==================================================
 * Constructores valorum
 * ================================================== */

SilvaValor
silva_valor_nihil (vacuum);
SilvaValor
silva_valor_nodus (
    SilvaNodus* nodus);
SilvaValor
silva_valor_token (
    SilvaToken* token);
SilvaValor
silva_valor_index (
    s32 index);

/* Prospectus super Xar existentem (mensura = numerus currens) */
SilvaValor
silva_valor_lista (
    Xar* lista);

/* Lista nova vacua (Xar de SilvaValor, prospectus 0) */
SilvaValor
silva_valor_lista_nova (
    Piscina* piscina);

/* Appendere PURUM (A½): prospectus alieni numquam laeduntur. Si
 * prospectus ad finem vivum repositorii stat, in loco appendit (O(1),
 * casus communis); alioquin furca divergens praefixum suum in
 * repositorium recens copiat. Prospectum NOVUM reddit (mensura + 1);
 * nihil-valorem in errore aut inputo non-lista. */
SilvaValor
silva_valor_lista_appendere (
       Piscina* piscina,
    SilvaValor  lista,
    SilvaValor  elementum);

/* Numerus elementorum PROSPECTUS (0 si non lista) */
i32
silva_valor_lista_numerus (
    SilvaValor lista);

/* Elementum intra prospectum (NIHIL extra mensuram prospectus) */
SilvaValor*
silva_valor_lista_obtinere (
    SilvaValor lista,
           i32 index);


/* ==================================================
 * Nodus
 * ================================================== */

/* Creare nodum: loci omnes SILVA_VALOR_NIHIL */
SilvaNodus*
silva_nodus_creare (
    Piscina* piscina,
        s32  genus,
        i32  numerus_locorum);

/* Ponere valorem in locum - S32: signum contra speciem probatur,
 * semel-tantum scribere imponitur. Reddit FALSUM in violatione
 * (et nihil scribit). */
b32
silva_nodus_ponere (
           SilvaNodus* nodus,
                  i32  locus,
           SilvaValor  valor,
    SilvaLocusSpecies  species);

/* Appendere in locum listae (creat listam si NIHIL) */
b32
silva_nodus_appendere (
              Piscina* piscina,
           SilvaNodus* nodus,
                  i32  locus,
           SilvaValor  valor,
    SilvaLocusSpecies  species);

/* Congruitne signum valoris cum specie loci? */
b32
silva_valor_congruit (
           SilvaValor valor,
    SilvaLocusSpecies species);

/* Liberi nodales: NODUS loci + elementa NODUS listarum, ordine locorum.
 * Tabulis non eget - valores signati sunt. Xar de SilvaNodus*. */
Xar*
silva_nodus_liberi (
                Piscina* piscina,
    constans SilvaNodus* nodus);

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

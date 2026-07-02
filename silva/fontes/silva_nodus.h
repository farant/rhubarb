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
        SilvaNodus*          nodus;
        SilvaToken*          token;
        SilvaListaProspectus lista;
        s32                  index;
    } datum;
};


/* ==================================================
 * Nodus uniformis
 * ================================================== */

structura SilvaNodus {
    s32         genus;            /* index in registro generum */
    i32         numerus_locorum;
    SilvaValor* loci;             /* series signata (layout per genus) */
    SilvaNodus* pater;            /* post-acceptum SOLUM (S27) */
};


/* ==================================================
 * Constructores valorum
 * ================================================== */

SilvaValor silva_valor_nihil (vacuum);
SilvaValor silva_valor_nodus (SilvaNodus* nodus);
SilvaValor silva_valor_token (SilvaToken* token);
SilvaValor silva_valor_index (s32 index);

/* Prospectus super Xar existentem (mensura = numerus currens) */
SilvaValor silva_valor_lista (Xar* lista);

/* Lista nova vacua (Xar de SilvaValor, prospectus 0) */
SilvaValor silva_valor_lista_nova (Piscina* piscina);

/* Appendere PURUM (A½): prospectus alieni numquam laeduntur. Si
 * prospectus ad finem vivum repositorii stat, in loco appendit (O(1),
 * casus communis); alioquin furca divergens praefixum suum in
 * repositorium recens copiat. Prospectum NOVUM reddit (mensura + 1);
 * nihil-valorem in errore aut inputo non-lista. */
SilvaValor
silva_valor_lista_appendere (
    Piscina*   piscina,
    SilvaValor lista,
    SilvaValor elementum);

/* Numerus elementorum PROSPECTUS (0 si non lista) */
i32 silva_valor_lista_numerus (SilvaValor lista);

/* Elementum intra prospectum (NIHIL extra mensuram prospectus) */
SilvaValor* silva_valor_lista_obtinere (SilvaValor lista, i32 index);


/* ==================================================
 * Nodus
 * ================================================== */

/* Creare nodum: loci omnes SILVA_VALOR_NIHIL */
SilvaNodus*
silva_nodus_creare (
    Piscina* piscina,
    s32      genus,
    i32      numerus_locorum);

/* Ponere valorem in locum - S32: signum contra speciem probatur,
 * semel-tantum scribere imponitur. Reddit FALSUM in violatione
 * (et nihil scribit). */
b32
silva_nodus_ponere (
    SilvaNodus*       nodus,
    i32               locus,
    SilvaValor        valor,
    SilvaLocusSpecies species);

/* Appendere in locum listae (creat listam si NIHIL) */
b32
silva_nodus_appendere (
    Piscina*          piscina,
    SilvaNodus*       nodus,
    i32               locus,
    SilvaValor        valor,
    SilvaLocusSpecies species);

/* Congruitne signum valoris cum specie loci? */
b32
silva_valor_congruit (
    SilvaValor        valor,
    SilvaLocusSpecies species);

/* Liberi nodales: NODUS loci + elementa NODUS listarum, ordine locorum.
 * Tabulis non eget - valores signati sunt. Xar de SilvaNodus*. */
Xar*
silva_nodus_liberi (
    Piscina*           piscina,
    constans SilvaNodus* nodus);

#endif /* SILVA_NODUS_H */

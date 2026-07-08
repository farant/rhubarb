/* silva_tabulae_sceleti.h - GENERATUM per silva_coquere ex grammatica/sceletum.stml
 * NOLI MANU MUTARE - regenera per silva/generare.sh.
 */

#ifndef SILVA_TABULAE_SCELETI_H
#define SILVA_TABULAE_SCELETI_H

#include "silva_tabulae.h"
#include "silva_nodus.h"

externus constans SilvaTabulaCocta SILVA_SCELETUM_TABULA;
externus constans SilvaRegistrumCoctum SILVA_SCELETUM_REGISTRUM;

/* Genera nodorum (registrum unum: grammatica + extra) */
#ifndef SILVA_SCELETUM_GENERA_CUSTOS
#define SILVA_SCELETUM_GENERA_CUSTOS
enumeratio {
    SILVA_SCELETUM_GENUS_DECLARATIO = 0,
    SILVA_SCELETUM_GENUS_TYPUS_PRIMITIVUS = 1,
    SILVA_SCELETUM_GENUS_TYPUS_NOMINATUS = 2,
    SILVA_SCELETUM_GENUS_DECLARATOR_TITULUS = 3,
    SILVA_SCELETUM_GENUS_DECLARATOR_MONSTRATOR = 4,
    SILVA_SCELETUM_GENUS_SENTENTIA_EXPRESSIONIS = 5,
    SILVA_SCELETUM_GENUS_BINARIUM = 6,
    SILVA_SCELETUM_GENUS_FOLIUM_IDENTIFICATOR = 7,
    SILVA_SCELETUM_GENUS_FOLIUM_INTEGER = 8,
    SILVA_SCELETUM_GENUS_PARENTHESIS = 9,
    SILVA_SCELETUM_GENUS_ERROR = 10,
    SILVA_SCELETUM_GENUS_AMBIGUUS = 11,
    SILVA_SCELETUM_GENUS_CONDITIONALIS = 12,
    SILVA_SCELETUM_GENUS_RAMUS_SUMPTUS = 13,
    SILVA_SCELETUM_GENUS_RAMUS_OMISSUS = 14
};
#endif /* SILVA_SCELETUM_GENERA_CUSTOS */

SilvaValor silva_sceletum_declaratio_typus (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_declaratio_declarator (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_declaratio_tok_terminator (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_typus_primitivus_tok_verbum (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_typus_nominatus_tok_titulus (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_declarator_titulus_tok_titulus (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_declarator_monstrator_tok_stella (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_declarator_monstrator_internum (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_sententia_expressionis_expressio (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_sententia_expressionis_tok_terminator (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_binarium_sinister (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_binarium_tok_operator (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_binarium_dexter (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_folium_identificator_tok_valor (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_folium_integer_tok_valor (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_parenthesis_tok_apertum (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_parenthesis_internum (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_parenthesis_tok_clausum (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_error_tokens (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_ambiguus_interpretationes (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_ambiguus_canonica (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_conditionalis_rami (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_conditionalis_finis (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_ramus_sumptus_directiva (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_ramus_sumptus_contentum (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_ramus_sumptus_conditio_id (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_ramus_omissus_directiva (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_ramus_omissus_cruda (constans SilvaNodus* nodus);
SilvaValor silva_sceletum_ramus_omissus_conditio_id (constans SilvaNodus* nodus);

/* Constructio ex reductione GLR: PURA (S26) - allocat
 * et implet solum; pater post-acceptum (S27) */
SilvaValor silva_sceletum_construere (
    Piscina*             piscina,
    s32                  productio,
    constans SilvaValor* valores);

/* Fabrica ambigui: involucrum AMBIGUUS pro motore
 * GLR (interpretationes = lista valorum, canonica = index
 * spinae canonicae) */
SilvaValor silva_sceletum_ambiguum_fabricare (
    Piscina*   piscina,
    SilvaValor interpretationes,
    s32        canonica);

#endif /* SILVA_TABULAE_SCELETI_H */

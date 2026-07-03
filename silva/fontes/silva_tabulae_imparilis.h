/* silva_tabulae_imparilis.h - GENERATUM per silva_coquere ex grammatica/sceletum_imparilis.stml
 * NOLI MANU MUTARE - regenera per silva/generare.sh.
 */

#ifndef SILVA_TABULAE_IMPARILIS_H
#define SILVA_TABULAE_IMPARILIS_H

#include "silva_tabulae.h"
#include "silva_nodus.h"

externus constans SilvaTabulaCocta SILVA_IMPARILIS_TABULA;
externus constans SilvaRegistrumCoctum SILVA_IMPARILIS_REGISTRUM;

/* Genera nodorum (registrum unum: grammatica + extra) */
enumeratio {
    SILVA_IMPARILIS_GENUS_DECLARATIO = 0,
    SILVA_IMPARILIS_GENUS_TYPUS_PRIMITIVUS = 1,
    SILVA_IMPARILIS_GENUS_TYPUS_NOMINATUS = 2,
    SILVA_IMPARILIS_GENUS_DECLARATOR_TITULUS = 3,
    SILVA_IMPARILIS_GENUS_DECLARATOR_MONSTRATOR = 4,
    SILVA_IMPARILIS_GENUS_SENTENTIA_EXPRESSIONIS = 5,
    SILVA_IMPARILIS_GENUS_BINARIUM = 6,
    SILVA_IMPARILIS_GENUS_FOLIUM_IDENTIFICATOR = 7,
    SILVA_IMPARILIS_GENUS_FOLIUM_INTEGER = 8,
    SILVA_IMPARILIS_GENUS_PARENTHESIS = 9,
    SILVA_IMPARILIS_GENUS_ERROR = 10,
    SILVA_IMPARILIS_GENUS_AMBIGUUS = 11,
    SILVA_IMPARILIS_GENUS_CONDITIONALIS = 12,
    SILVA_IMPARILIS_GENUS_RAMUS_SUMPTUS = 13,
    SILVA_IMPARILIS_GENUS_RAMUS_OMISSUS = 14
};

SilvaValor silva_imparilis_declaratio_typus (constans SilvaNodus* nodus);
SilvaValor silva_imparilis_declaratio_declarator (constans SilvaNodus* nodus);
SilvaValor silva_imparilis_declaratio_tok_terminator (constans SilvaNodus* nodus);
SilvaValor silva_imparilis_typus_primitivus_tok_verbum (constans SilvaNodus* nodus);
SilvaValor silva_imparilis_typus_nominatus_tok_titulus (constans SilvaNodus* nodus);
SilvaValor silva_imparilis_declarator_titulus_tok_titulus (constans SilvaNodus* nodus);
SilvaValor silva_imparilis_declarator_monstrator_tok_stella (constans SilvaNodus* nodus);
SilvaValor silva_imparilis_declarator_monstrator_internum (constans SilvaNodus* nodus);
SilvaValor silva_imparilis_sententia_expressionis_expressio (constans SilvaNodus* nodus);
SilvaValor silva_imparilis_sententia_expressionis_tok_terminator (constans SilvaNodus* nodus);
SilvaValor silva_imparilis_binarium_sinister (constans SilvaNodus* nodus);
SilvaValor silva_imparilis_binarium_tok_operator (constans SilvaNodus* nodus);
SilvaValor silva_imparilis_binarium_dexter (constans SilvaNodus* nodus);
SilvaValor silva_imparilis_folium_identificator_tok_valor (constans SilvaNodus* nodus);
SilvaValor silva_imparilis_folium_integer_tok_valor (constans SilvaNodus* nodus);
SilvaValor silva_imparilis_parenthesis_tok_apertum (constans SilvaNodus* nodus);
SilvaValor silva_imparilis_parenthesis_internum (constans SilvaNodus* nodus);
SilvaValor silva_imparilis_parenthesis_tok_clausum (constans SilvaNodus* nodus);
SilvaValor silva_imparilis_error_tokens (constans SilvaNodus* nodus);
SilvaValor silva_imparilis_ambiguus_interpretationes (constans SilvaNodus* nodus);
SilvaValor silva_imparilis_ambiguus_canonica (constans SilvaNodus* nodus);
SilvaValor silva_imparilis_conditionalis_rami (constans SilvaNodus* nodus);
SilvaValor silva_imparilis_conditionalis_finis (constans SilvaNodus* nodus);
SilvaValor silva_imparilis_ramus_sumptus_directiva (constans SilvaNodus* nodus);
SilvaValor silva_imparilis_ramus_sumptus_contentum (constans SilvaNodus* nodus);
SilvaValor silva_imparilis_ramus_sumptus_conditio_id (constans SilvaNodus* nodus);
SilvaValor silva_imparilis_ramus_omissus_directiva (constans SilvaNodus* nodus);
SilvaValor silva_imparilis_ramus_omissus_cruda (constans SilvaNodus* nodus);
SilvaValor silva_imparilis_ramus_omissus_conditio_id (constans SilvaNodus* nodus);

/* Constructio ex reductione GLR: PURA (S26) - allocat
 * et implet solum; pater post-acceptum (S27) */
SilvaValor silva_imparilis_construere (
    Piscina*             piscina,
    s32                  productio,
    constans SilvaValor* valores);

/* Fabrica ambigui: involucrum AMBIGUUS pro motore
 * GLR (interpretationes = lista valorum, canonica = index
 * spinae canonicae) */
SilvaValor silva_imparilis_ambiguum_fabricare (
    Piscina*   piscina,
    SilvaValor interpretationes,
    s32        canonica);

#endif /* SILVA_TABULAE_IMPARILIS_H */

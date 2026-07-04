/* silva_tabulae_c89.h - GENERATUM per silva_coquere ex grammatica/c89.stml
 * NOLI MANU MUTARE - regenera per silva/generare.sh.
 */

#ifndef SILVA_TABULAE_C89_H
#define SILVA_TABULAE_C89_H

#include "silva_tabulae.h"
#include "silva_nodus.h"

externus constans SilvaTabulaCocta SILVA_C89_TABULA;
externus constans SilvaRegistrumCoctum SILVA_C89_REGISTRUM;

/* Genera nodorum (registrum unum: grammatica + extra) */
enumeratio {
    SILVA_C89_GENUS_SENTENTIA_EXPRESSIONIS = 0,
    SILVA_C89_GENUS_VIRGULA = 1,
    SILVA_C89_GENUS_ASSIGNATIO = 2,
    SILVA_C89_GENUS_TERNARIUS = 3,
    SILVA_C89_GENUS_BINARIUM = 4,
    SILVA_C89_GENUS_CONVERSIO = 5,
    SILVA_C89_GENUS_UNARIUM = 6,
    SILVA_C89_GENUS_MAGNITUDO_EXPRESSIONIS = 7,
    SILVA_C89_GENUS_MAGNITUDO_TYPI = 8,
    SILVA_C89_GENUS_SPECIES_TYPI = 9,
    SILVA_C89_GENUS_TYPUS_PRIMITIVUS = 10,
    SILVA_C89_GENUS_TYPUS_NOMINATUS = 11,
    SILVA_C89_GENUS_DECLARATOR_ABSTRACTUS = 12,
    SILVA_C89_GENUS_SUBSCRIPTIO = 13,
    SILVA_C89_GENUS_VOCATIO = 14,
    SILVA_C89_GENUS_ACCESSUS = 15,
    SILVA_C89_GENUS_POSTCREMENTUM = 16,
    SILVA_C89_GENUS_FOLIUM_IDENTIFICATOR = 17,
    SILVA_C89_GENUS_FOLIUM_INTEGER = 18,
    SILVA_C89_GENUS_FOLIUM_FLUITANS = 19,
    SILVA_C89_GENUS_FOLIUM_CHARACTER = 20,
    SILVA_C89_GENUS_FOLIUM_CHORDA = 21,
    SILVA_C89_GENUS_PARENTHESIS = 22,
    SILVA_C89_GENUS_ERROR = 23,
    SILVA_C89_GENUS_AMBIGUUS = 24,
    SILVA_C89_GENUS_CONDITIONALIS = 25,
    SILVA_C89_GENUS_RAMUS_SUMPTUS = 26,
    SILVA_C89_GENUS_RAMUS_OMISSUS = 27
};

SilvaValor silva_c89_sententia_expressionis_expressio (constans SilvaNodus* nodus);
SilvaValor silva_c89_sententia_expressionis_tok_terminator (constans SilvaNodus* nodus);
SilvaValor silva_c89_virgula_sinister (constans SilvaNodus* nodus);
SilvaValor silva_c89_virgula_tok_operator (constans SilvaNodus* nodus);
SilvaValor silva_c89_virgula_dexter (constans SilvaNodus* nodus);
SilvaValor silva_c89_assignatio_sinister (constans SilvaNodus* nodus);
SilvaValor silva_c89_assignatio_tok_operator (constans SilvaNodus* nodus);
SilvaValor silva_c89_assignatio_dexter (constans SilvaNodus* nodus);
SilvaValor silva_c89_ternarius_conditio (constans SilvaNodus* nodus);
SilvaValor silva_c89_ternarius_tok_quaestio (constans SilvaNodus* nodus);
SilvaValor silva_c89_ternarius_verum (constans SilvaNodus* nodus);
SilvaValor silva_c89_ternarius_tok_colon (constans SilvaNodus* nodus);
SilvaValor silva_c89_ternarius_falsum (constans SilvaNodus* nodus);
SilvaValor silva_c89_binarium_sinister (constans SilvaNodus* nodus);
SilvaValor silva_c89_binarium_tok_operator (constans SilvaNodus* nodus);
SilvaValor silva_c89_binarium_dexter (constans SilvaNodus* nodus);
SilvaValor silva_c89_conversio_tok_apertum (constans SilvaNodus* nodus);
SilvaValor silva_c89_conversio_typus (constans SilvaNodus* nodus);
SilvaValor silva_c89_conversio_tok_clausum (constans SilvaNodus* nodus);
SilvaValor silva_c89_conversio_internum (constans SilvaNodus* nodus);
SilvaValor silva_c89_unarium_tok_operator (constans SilvaNodus* nodus);
SilvaValor silva_c89_unarium_internum (constans SilvaNodus* nodus);
SilvaValor silva_c89_magnitudo_expressionis_tok_verbum (constans SilvaNodus* nodus);
SilvaValor silva_c89_magnitudo_expressionis_internum (constans SilvaNodus* nodus);
SilvaValor silva_c89_magnitudo_typi_tok_verbum (constans SilvaNodus* nodus);
SilvaValor silva_c89_magnitudo_typi_tok_apertum (constans SilvaNodus* nodus);
SilvaValor silva_c89_magnitudo_typi_typus (constans SilvaNodus* nodus);
SilvaValor silva_c89_magnitudo_typi_tok_clausum (constans SilvaNodus* nodus);
SilvaValor silva_c89_species_typi_specificatores (constans SilvaNodus* nodus);
SilvaValor silva_c89_species_typi_declarator (constans SilvaNodus* nodus);
SilvaValor silva_c89_typus_primitivus_tok_verba (constans SilvaNodus* nodus);
SilvaValor silva_c89_typus_nominatus_tok_titulus (constans SilvaNodus* nodus);
SilvaValor silva_c89_declarator_abstractus_tok_stella (constans SilvaNodus* nodus);
SilvaValor silva_c89_declarator_abstractus_internum (constans SilvaNodus* nodus);
SilvaValor silva_c89_subscriptio_basis (constans SilvaNodus* nodus);
SilvaValor silva_c89_subscriptio_tok_apertum (constans SilvaNodus* nodus);
SilvaValor silva_c89_subscriptio_index (constans SilvaNodus* nodus);
SilvaValor silva_c89_subscriptio_tok_clausum (constans SilvaNodus* nodus);
SilvaValor silva_c89_vocatio_functio (constans SilvaNodus* nodus);
SilvaValor silva_c89_vocatio_tok_apertum (constans SilvaNodus* nodus);
SilvaValor silva_c89_vocatio_argumenta (constans SilvaNodus* nodus);
SilvaValor silva_c89_vocatio_tok_clausum (constans SilvaNodus* nodus);
SilvaValor silva_c89_accessus_basis (constans SilvaNodus* nodus);
SilvaValor silva_c89_accessus_tok_operator (constans SilvaNodus* nodus);
SilvaValor silva_c89_accessus_tok_titulus (constans SilvaNodus* nodus);
SilvaValor silva_c89_postcrementum_internum (constans SilvaNodus* nodus);
SilvaValor silva_c89_postcrementum_tok_operator (constans SilvaNodus* nodus);
SilvaValor silva_c89_folium_identificator_tok_valor (constans SilvaNodus* nodus);
SilvaValor silva_c89_folium_integer_tok_valor (constans SilvaNodus* nodus);
SilvaValor silva_c89_folium_fluitans_tok_valor (constans SilvaNodus* nodus);
SilvaValor silva_c89_folium_character_tok_valor (constans SilvaNodus* nodus);
SilvaValor silva_c89_folium_chorda_tok_valor (constans SilvaNodus* nodus);
SilvaValor silva_c89_parenthesis_tok_apertum (constans SilvaNodus* nodus);
SilvaValor silva_c89_parenthesis_internum (constans SilvaNodus* nodus);
SilvaValor silva_c89_parenthesis_tok_clausum (constans SilvaNodus* nodus);
SilvaValor silva_c89_error_tokens (constans SilvaNodus* nodus);
SilvaValor silva_c89_ambiguus_interpretationes (constans SilvaNodus* nodus);
SilvaValor silva_c89_ambiguus_canonica (constans SilvaNodus* nodus);
SilvaValor silva_c89_conditionalis_rami (constans SilvaNodus* nodus);
SilvaValor silva_c89_conditionalis_finis (constans SilvaNodus* nodus);
SilvaValor silva_c89_ramus_sumptus_directiva (constans SilvaNodus* nodus);
SilvaValor silva_c89_ramus_sumptus_contentum (constans SilvaNodus* nodus);
SilvaValor silva_c89_ramus_sumptus_conditio_id (constans SilvaNodus* nodus);
SilvaValor silva_c89_ramus_omissus_directiva (constans SilvaNodus* nodus);
SilvaValor silva_c89_ramus_omissus_cruda (constans SilvaNodus* nodus);
SilvaValor silva_c89_ramus_omissus_conditio_id (constans SilvaNodus* nodus);

/* Constructio ex reductione GLR: PURA (S26) - allocat
 * et implet solum; pater post-acceptum (S27) */
SilvaValor silva_c89_construere (
    Piscina*             piscina,
    s32                  productio,
    constans SilvaValor* valores);

/* Fabrica ambigui: involucrum AMBIGUUS pro motore
 * GLR (interpretationes = lista valorum, canonica = index
 * spinae canonicae) */
SilvaValor silva_c89_ambiguum_fabricare (
    Piscina*   piscina,
    SilvaValor interpretationes,
    s32        canonica);

#endif /* SILVA_TABULAE_C89_H */

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
    SILVA_C89_GENUS_DECLARATIO = 0,
    SILVA_C89_GENUS_TYPUS_NOMINATUS = 1,
    SILVA_C89_GENUS_TYPUS_PRIMITIVUS = 2,
    SILVA_C89_GENUS_DECLARATOR_INITIATUS = 3,
    SILVA_C89_GENUS_CONGERIES = 4,
    SILVA_C89_GENUS_STRUCTURA = 5,
    SILVA_C89_GENUS_UNIO = 6,
    SILVA_C89_GENUS_MEMBRUM = 7,
    SILVA_C89_GENUS_CAMPUS = 8,
    SILVA_C89_GENUS_ENUMERATIO = 9,
    SILVA_C89_GENUS_ENUMERATOR = 10,
    SILVA_C89_GENUS_DECLARATOR_MONSTRATOR = 11,
    SILVA_C89_GENUS_DECLARATOR_TITULUS = 12,
    SILVA_C89_GENUS_PARENTHESIS = 13,
    SILVA_C89_GENUS_DECLARATOR_ACIEI = 14,
    SILVA_C89_GENUS_DECLARATOR_FUNCTIONIS = 15,
    SILVA_C89_GENUS_PARAMETRUM = 16,
    SILVA_C89_GENUS_SENTENTIA_EXPRESSIONIS = 17,
    SILVA_C89_GENUS_VIRGULA = 18,
    SILVA_C89_GENUS_ASSIGNATIO = 19,
    SILVA_C89_GENUS_TERNARIUS = 20,
    SILVA_C89_GENUS_BINARIUM = 21,
    SILVA_C89_GENUS_CONVERSIO = 22,
    SILVA_C89_GENUS_UNARIUM = 23,
    SILVA_C89_GENUS_MAGNITUDO_EXPRESSIONIS = 24,
    SILVA_C89_GENUS_MAGNITUDO_TYPI = 25,
    SILVA_C89_GENUS_SPECIES_TYPI = 26,
    SILVA_C89_GENUS_DECLARATOR_ABSTRACTUS = 27,
    SILVA_C89_GENUS_SUBSCRIPTIO = 28,
    SILVA_C89_GENUS_VOCATIO = 29,
    SILVA_C89_GENUS_ACCESSUS = 30,
    SILVA_C89_GENUS_POSTCREMENTUM = 31,
    SILVA_C89_GENUS_FOLIUM_IDENTIFICATOR = 32,
    SILVA_C89_GENUS_FOLIUM_INTEGER = 33,
    SILVA_C89_GENUS_FOLIUM_FLUITANS = 34,
    SILVA_C89_GENUS_FOLIUM_CHARACTER = 35,
    SILVA_C89_GENUS_FOLIUM_CHORDA = 36,
    SILVA_C89_GENUS_ERROR = 37,
    SILVA_C89_GENUS_AMBIGUUS = 38,
    SILVA_C89_GENUS_CONDITIONALIS = 39,
    SILVA_C89_GENUS_RAMUS_SUMPTUS = 40,
    SILVA_C89_GENUS_RAMUS_OMISSUS = 41
};

SilvaValor silva_c89_declaratio_specificatores (constans SilvaNodus* nodus);
SilvaValor silva_c89_declaratio_declaratores (constans SilvaNodus* nodus);
SilvaValor silva_c89_declaratio_tok_terminator (constans SilvaNodus* nodus);
SilvaValor silva_c89_typus_nominatus_tok_titulus (constans SilvaNodus* nodus);
SilvaValor silva_c89_typus_primitivus_tok_verba (constans SilvaNodus* nodus);
SilvaValor silva_c89_declarator_initiatus_declarator (constans SilvaNodus* nodus);
SilvaValor silva_c89_declarator_initiatus_tok_operator (constans SilvaNodus* nodus);
SilvaValor silva_c89_declarator_initiatus_initiator (constans SilvaNodus* nodus);
SilvaValor silva_c89_congeries_tok_aperta (constans SilvaNodus* nodus);
SilvaValor silva_c89_congeries_elementa (constans SilvaNodus* nodus);
SilvaValor silva_c89_congeries_tok_clausa (constans SilvaNodus* nodus);
SilvaValor silva_c89_structura_tok_verbum (constans SilvaNodus* nodus);
SilvaValor silva_c89_structura_tok_titulus (constans SilvaNodus* nodus);
SilvaValor silva_c89_structura_tok_aperta (constans SilvaNodus* nodus);
SilvaValor silva_c89_structura_membra (constans SilvaNodus* nodus);
SilvaValor silva_c89_structura_tok_clausa (constans SilvaNodus* nodus);
SilvaValor silva_c89_unio_tok_verbum (constans SilvaNodus* nodus);
SilvaValor silva_c89_unio_tok_titulus (constans SilvaNodus* nodus);
SilvaValor silva_c89_unio_tok_aperta (constans SilvaNodus* nodus);
SilvaValor silva_c89_unio_membra (constans SilvaNodus* nodus);
SilvaValor silva_c89_unio_tok_clausa (constans SilvaNodus* nodus);
SilvaValor silva_c89_membrum_specificatores (constans SilvaNodus* nodus);
SilvaValor silva_c89_membrum_declaratores (constans SilvaNodus* nodus);
SilvaValor silva_c89_membrum_tok_terminator (constans SilvaNodus* nodus);
SilvaValor silva_c89_campus_declarator (constans SilvaNodus* nodus);
SilvaValor silva_c89_campus_tok_colon (constans SilvaNodus* nodus);
SilvaValor silva_c89_campus_latitudo (constans SilvaNodus* nodus);
SilvaValor silva_c89_enumeratio_tok_verbum (constans SilvaNodus* nodus);
SilvaValor silva_c89_enumeratio_tok_titulus (constans SilvaNodus* nodus);
SilvaValor silva_c89_enumeratio_tok_aperta (constans SilvaNodus* nodus);
SilvaValor silva_c89_enumeratio_enumeratores (constans SilvaNodus* nodus);
SilvaValor silva_c89_enumeratio_tok_clausa (constans SilvaNodus* nodus);
SilvaValor silva_c89_enumerator_tok_titulus (constans SilvaNodus* nodus);
SilvaValor silva_c89_enumerator_tok_operator (constans SilvaNodus* nodus);
SilvaValor silva_c89_enumerator_valor (constans SilvaNodus* nodus);
SilvaValor silva_c89_declarator_monstrator_tok_stella (constans SilvaNodus* nodus);
SilvaValor silva_c89_declarator_monstrator_qualificatores (constans SilvaNodus* nodus);
SilvaValor silva_c89_declarator_monstrator_internum (constans SilvaNodus* nodus);
SilvaValor silva_c89_declarator_titulus_tok_titulus (constans SilvaNodus* nodus);
SilvaValor silva_c89_parenthesis_tok_apertum (constans SilvaNodus* nodus);
SilvaValor silva_c89_parenthesis_internum (constans SilvaNodus* nodus);
SilvaValor silva_c89_parenthesis_tok_clausum (constans SilvaNodus* nodus);
SilvaValor silva_c89_declarator_aciei_internum (constans SilvaNodus* nodus);
SilvaValor silva_c89_declarator_aciei_tok_apertum (constans SilvaNodus* nodus);
SilvaValor silva_c89_declarator_aciei_mensura (constans SilvaNodus* nodus);
SilvaValor silva_c89_declarator_aciei_tok_clausum (constans SilvaNodus* nodus);
SilvaValor silva_c89_declarator_functionis_internum (constans SilvaNodus* nodus);
SilvaValor silva_c89_declarator_functionis_tok_apertum (constans SilvaNodus* nodus);
SilvaValor silva_c89_declarator_functionis_parametra (constans SilvaNodus* nodus);
SilvaValor silva_c89_declarator_functionis_tok_clausum (constans SilvaNodus* nodus);
SilvaValor silva_c89_parametrum_specificatores (constans SilvaNodus* nodus);
SilvaValor silva_c89_parametrum_declarator (constans SilvaNodus* nodus);
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
SilvaValor silva_c89_declarator_abstractus_tok_stella (constans SilvaNodus* nodus);
SilvaValor silva_c89_declarator_abstractus_qualificatores (constans SilvaNodus* nodus);
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

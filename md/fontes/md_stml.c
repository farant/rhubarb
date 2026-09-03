/* md_stml.c - Vide md_stml.h. */

#include "md_stml.h"
#include "md_registrum.h"
#include "md_lexicon.h"
#include "materia_nodus.h"
#include "materia_token.h"

/* Radix derivatorum: lexema vacuum, fonte plagulae, 'iam positum'
 * (byte_offset ZEPHYRUM) - lector eam praeterit sine promotione.
 * NON constans: lector eam ponere potest si ancora deest; innocuum. */
hic_manens MateriaToken RADIX_DERIVATORUM;
hic_manens b32          RADIX_PARATA = FALSUM;

interior vacuum
_sedes_quaerere (
                   vacuum* datum,
    constans MateriaToken* lexema,
             MateriaSedes* sedes)
{
    (vacuum)datum;
    sedes->byte_offset = lexema->byte_offset;
    sedes->linea = lexema->linea;
    sedes->columna = lexema->columna;
    sedes->fons_index = lexema->fons_index;
    sedes->est_fons = (b32)(lexema->fons_index != MD_FONS_DERIVATUS);
}

interior MateriaToken*
_radix_quaerere (
                vacuum* datum,
          MateriaToken* lexema,
    constans character** causa)
{
    (vacuum)datum;
    *causa = NIHIL;
    si (lexema->fons_index != MD_FONS_DERIVATUS)
    {
        redde lexema;   /* se ipsum emittit */
    }
    si (!RADIX_PARATA)
    {
        RADIX_DERIVATORUM.valor.datum    = NIHIL;
        RADIX_DERIVATORUM.valor.mensura  = ZEPHYRUM;
        RADIX_DERIVATORUM.genus          = (s32)MD_LEX_DERIVATUM;
        RADIX_DERIVATORUM.byte_offset    = ZEPHYRUM;
        RADIX_DERIVATORUM.linea          = I;
        RADIX_DERIVATORUM.columna        = I;
        RADIX_DERIVATORUM.fons_index     = MD_FONS_PLAGULAE;
        RADIX_PARATA                     = VERUM;
    }
    redde &RADIX_DERIVATORUM;
}

hic_manens constans MateriaOrigoUncus ORIGO_MD = {
    NIHIL,
    _sedes_quaerere,
    _radix_quaerere,
    NIHIL
};

vacuum
md_stml_consilium (
           MateriaArborConsilium* consilium,
    constans MateriaLexiconRatum* ratum)
{
    materia_arbor_consilium_nudum(consilium, &MD_REGISTRUM, ratum,
        "md");
    consilium->origo = &ORIGO_MD;
}

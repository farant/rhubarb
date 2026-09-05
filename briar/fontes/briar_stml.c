/* briar_stml.c - Vide briar_stml.h (exemplar md_stml.c). */

#include "briar_stml.h"
#include "briar_registrum.h"
#include "briar_lexicon.h"
#include "materia_nodus.h"
#include "materia_token.h"

/* Radix derivatorum: lexema vacuum, fonte plagulae, 'iam positum'
 * (byte_offset ZEPHYRUM) - lector eam praeterit sine promotione. */
hic_manens MateriaToken RADIX_DERIVATORUM;
hic_manens b32          RADIX_PARATA = FALSUM;

interior vacuum
_sedes_quaerere (
                   vacuum* datum,
    constans MateriaToken* lexema,
             MateriaSedes* sedes)
{
    (vacuum)datum;
    sedes->byte_offset  = lexema->byte_offset;
    sedes->linea        = lexema->linea;
    sedes->columna      = lexema->columna;
    sedes->fons_index   = lexema->fons_index;
    sedes->est_fons = (b32)(lexema->fons_index
        != BRIAR_FONS_DERIVATUS);
}

interior MateriaToken*
_radix_quaerere (
                vacuum* datum,
          MateriaToken* lexema,
    constans character** causa)
{
    (vacuum)datum;
    *causa = NIHIL;
    si (lexema->fons_index != BRIAR_FONS_DERIVATUS)
    {
        redde lexema;   /* se ipsum emittit */
    }
    si (!RADIX_PARATA)
    {
        RADIX_DERIVATORUM.valor.datum    = NIHIL;
        RADIX_DERIVATORUM.valor.mensura  = ZEPHYRUM;
        RADIX_DERIVATORUM.genus          = (s32)BRIAR_LEX_DERIVATUM;
        RADIX_DERIVATORUM.byte_offset    = ZEPHYRUM;
        RADIX_DERIVATORUM.linea          = I;
        RADIX_DERIVATORUM.columna        = I;
        RADIX_DERIVATORUM.fons_index     = BRIAR_FONS_PLAGULAE;
        RADIX_PARATA                     = VERUM;
    }
    redde &RADIX_DERIVATORUM;
}

hic_manens constans MateriaOrigoUncus ORIGO_BRIAR = {
    NIHIL,
    _sedes_quaerere,
    _radix_quaerere,
    NIHIL
};

vacuum
briar_stml_consilium (
           MateriaArborConsilium* consilium,
    constans MateriaLexiconRatum* ratum)
{
    materia_arbor_consilium_nudum(consilium, &BRIAR_REGISTRUM, ratum,
        "briar");
    consilium->origo = &ORIGO_BRIAR;
}

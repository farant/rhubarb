/* oratio_stml.c - Vide oratio_stml.h (speculum md_stml.c). */

#include "oratio_stml.h"
#include "oratio_registrum.h"
#include "oratio_lexicon.h"
#include "materia_nodus.h"
#include "materia_token.h"

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
        != ORATIO_FONS_DERIVATUS);
}

interior MateriaToken*
_radix_quaerere (
                vacuum* datum,
          MateriaToken* lexema,
    constans character** causa)
{
    (vacuum)datum;
    *causa = NIHIL;
    si (lexema->fons_index != ORATIO_FONS_DERIVATUS)
    {
        redde lexema;   /* se ipsum emittit */
    }
    si (!RADIX_PARATA)
    {
        RADIX_DERIVATORUM.valor.datum    = NIHIL;
        RADIX_DERIVATORUM.valor.mensura  = ZEPHYRUM;
        RADIX_DERIVATORUM.genus          = (s32)ORATIO_LEX_DERIVATUM;
        RADIX_DERIVATORUM.byte_offset    = ZEPHYRUM;
        RADIX_DERIVATORUM.linea          = I;
        RADIX_DERIVATORUM.columna        = I;
        RADIX_DERIVATORUM.fons_index     = ORATIO_FONS_PLAGULAE;
        RADIX_PARATA                     = VERUM;
    }
    redde &RADIX_DERIVATORUM;
}

hic_manens constans MateriaOrigoUncus ORIGO_ORATIONIS = {
    NIHIL,
    _sedes_quaerere,
    _radix_quaerere,
    NIHIL
};

vacuum
oratio_stml_consilium (
           MateriaArborConsilium* consilium,
    constans MateriaLexiconRatum* ratum)
{
    materia_arbor_consilium_nudum(consilium, &ORATIO_REGISTRUM, ratum,
        "oratio");
    consilium->origo = &ORIGO_ORATIONIS;
}

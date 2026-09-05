/* oratio_stml.c - Vide oratio_stml.h (speculum md_stml.c). */

#include "oratio_stml.h"
#include "oratio_registrum.h"
#include "oratio_lexicon.h"
#include "materia_arbor.h"
#include "materia_nodus.h"
#include "materia_token.h"
#include "stml.h"

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

/* Uncus nodi (T12, uncus substrati secundus): compendia derivata
 * CLASSES et LINGUAE vocabuli etiam ut ATTRIBUTA elementi <vocabulum>
 * scribuntur, ut selectio '[classes~=verbum]' congruat. Lector attributa
 * nodi ignorat: loci veritas sunt, attributum speculum. */
interior b32
_nodum_ornare (
                   vacuum* datum,
     MateriaArborScriptor* scriptor,
                StmlNodus* elementum,
    constans MateriaNodus* nodus)
{
    hic_manens constans i32 loci[II] = {
        (i32)ORATIO_VOCABULUM_CLASSES, (i32)ORATIO_VOCABULUM_LINGUAE
    };
    hic_manens constans character* constans tituli[II] = {
        "classes", "linguae"
    };
    i32 k;

    (vacuum)datum;
    si (nodus->genus != (s32)ORATIO_GENUS_VOCABULUM)
    {
        redde VERUM;
    }
    per (k = ZEPHYRUM; k < (i32)II; k++)
    {
        constans MateriaValor* v = &nodus->loci[loci[k]];

        si (   v->genus                      != MATERIA_VALOR_TOKEN
            || v->datum.token                == NIHIL
            || v->datum.token->valor.mensura == ZEPHYRUM)
        {
            perge;
        }
        si (!stml_attributum_addere_chorda(elementum,
                materia_arbor_scriptor_piscina(scriptor),
                materia_arbor_scriptor_intern(scriptor), tituli[k],
                v->datum.token->valor))
        {
            materia_arbor_scriptor_recusare(scriptor,
                "attributum classium scribi non potuit");
            redde FALSUM;
        }
    }
    redde VERUM;
}

hic_manens constans MateriaArborFrons FRONS_ORATIONIS = {
    NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL,
    _nodum_ornare
};

vacuum
oratio_stml_consilium (
           MateriaArborConsilium* consilium,
    constans MateriaLexiconRatum* ratum)
{
    materia_arbor_consilium_nudum(consilium, &ORATIO_REGISTRUM, ratum,
        "oratio");
    consilium->origo = &ORIGO_ORATIONIS;
    consilium->frons = &FRONS_ORATIONIS;
}

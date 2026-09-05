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

/* Ordinalis nodi intra listam patris cuiuslibet (loci listarum
 * patris percurruntur); -I sine patre aut absens */
interior s32
_ordinalis (
    constans MateriaNodus* nodus)
{
    constans MateriaNodus* pater = nodus->pater;
                      i32  l;

    si (pater == NIHIL)
    {
        redde (s32)-I;
    }
    per (l = ZEPHYRUM; l < pater->numerus_locorum; l++)
    {
        constans MateriaValor* v = &pater->loci[l];
                          i32  n;
                          i32  k;

        si (v->genus != MATERIA_VALOR_LISTA)
        {
            perge;
        }
        n = materia_valor_lista_numerus(*v);
        per (k = ZEPHYRUM; k < n; k++)
        {
            constans MateriaValor* e = materia_valor_lista_obtinere(*v,
                k);

            si (   e != NIHIL && e->genus == MATERIA_VALOR_NODUS
                && e->datum.nodus == nodus)
            {
                redde (s32)k;
            }
        }
    }
    redde (s32)-I;
}

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

    /* T17: ordinalis 'n' intra listam patris (sententia, elementa,
     * analyses), lingua documenti in documento et sententia */
    si (   nodus->genus == (s32)ORATIO_GENUS_SENTENTIA
        || nodus->genus == (s32)ORATIO_GENUS_VOCABULUM
        || nodus->genus == (s32)ORATIO_GENUS_INTERPUNCTIO
        || nodus->genus == (s32)ORATIO_GENUS_NUMERUS
        || nodus->genus >= (s32)ORATIO_GENUS_ANALYSIS_PRIMUM)
    {
        s32 ordinalis = _ordinalis(nodus);

        si (   ordinalis >= ZEPHYRUM
            && !materia_arbor_attributum_numeri(scriptor, elementum,
            "n",
                (i32)ordinalis))
        {
            materia_arbor_scriptor_recusare(scriptor,
                "attributum ordinalis scribi non potuit");
            redde FALSUM;
        }
    }
    si (   datum                                        != NIHIL
        && ((constans OratioStmlOrnatus*)datum)->lingua != NIHIL
        && (   nodus->genus == (s32)ORATIO_GENUS_DOCUMENTUM
            || nodus->genus == (s32)ORATIO_GENUS_SENTENTIA)
        && !stml_attributum_addere(elementum,
            materia_arbor_scriptor_piscina(scriptor),
            materia_arbor_scriptor_intern(scriptor), "lingua",
            ((constans OratioStmlOrnatus*)datum)->lingua))
    {
        materia_arbor_scriptor_recusare(scriptor,
            "attributum linguae scribi non potuit");
        redde FALSUM;
    }
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

vacuum
oratio_stml_consilium_ornatum (
           MateriaArborConsilium* consilium,
    constans MateriaLexiconRatum* ratum,
               OratioStmlOrnatus* ornatus,
              constans character* lingua)
{
    oratio_stml_consilium(consilium, ratum);
    ornatus->frons        = FRONS_ORATIONIS;
    ornatus->frons.datum  = ornatus;
    ornatus->lingua       = lingua;
    consilium->frons      = &ornatus->frons;
}

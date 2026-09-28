/* toml.c - API publica toml (vide toml.h)
 *
 * Involucrum tenue: parsare (toml_arbor) -> coquere (toml_coctum) ->
 * tenere. Via clavium per lectorem ipsum legitur (modus CLAVIS), ergo
 * quod in fonte clavis est, in quaestione clavis est - una grammatica.
 */

#include "toml.h"
#include "toml_arbor.h"
#include "toml_coctum.h"
#include "toml_lector.h"
#include "toml_scalaris.h"
#include "materia_pictor.h"
#include "chorda_aedificator.h"
#include <string.h>

structura TomlDocumentum {
                 Piscina* piscina;
      constans character* via;
                  chorda  textus;
            MateriaNodus* arbor;
             TomlParsura  parsura;
              TomlCoctum  coctum;
};


/* ==================================================
 * Legere
 * ================================================== */

TomlDocumentum*
toml_legere (
                 chorda  textus,
     constans character* via,
                Piscina* piscina)
{
    TomlDocumentum* doc;

    si (piscina == NIHIL)
    {
        redde NIHIL;
    }
    doc = (TomlDocumentum*)piscina_allocare(piscina,
        (i64)magnitudo(TomlDocumentum));
    si (doc == NIHIL)
    {
        redde NIHIL;
    }
    memset(doc, ZEPHYRUM, magnitudo(TomlDocumentum));
    doc->piscina  = piscina;
    doc->via      = via != NIHIL ? via : "<toml>";
    doc->textus   = textus;
    doc->arbor   = toml_arbor_parsare(piscina,
        (constans character*)textus.datum, (s32)textus.mensura,
        &doc->parsura);
    si (doc->arbor == NIHIL)
    {
        redde NIHIL;
    }
    doc->coctum = toml_coquere(piscina, doc->arbor, &doc->parsura);
    si (doc->coctum.radix == NIHIL || doc->coctum.diagnostica == NIHIL)
    {
        redde NIHIL;
    }
    redde doc;
}

b32
toml_successus (
    constans TomlDocumentum* doc)
{
    redde doc != NIHIL && doc->coctum.sanum;
}

Xar*
toml_diagnostica (
    constans TomlDocumentum* doc)
{
    redde doc != NIHIL ? doc->coctum.diagnostica : NIHIL;
}

chorda
toml_diagnostica_scribere (
                    Piscina* piscina,
    constans TomlDocumentum* doc,
                        b32  excerptum)
{
     ChordaAedificator* ae;
                chorda  vacua;
                   i32  k;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    si (piscina == NIHIL || doc == NIHIL)
    {
        redde vacua;
    }
    ae = chorda_aedificator_creare(piscina, MXXIV);
    si (ae == NIHIL)
    {
        redde vacua;
    }
    per (k = ZEPHYRUM; k < xar_numerus(doc->coctum.diagnostica); k++)
    {
        chorda bloc = materia_pictor_scribere(piscina,
            (constans MateriaDiagnosticum*)xar_obtinere(
                doc->coctum.diagnostica, k),
            doc->via, "toml", (constans character*)doc->textus.datum,
            doc->textus.mensura, excerptum);

        chorda_aedificator_appendere_chorda(ae, bloc);
    }
    redde chorda_aedificator_finire(ae);
}

constans TomlValor*
toml_radix (
    constans TomlDocumentum* doc)
{
    redde doc != NIHIL ? doc->coctum.radix : NIHIL;
}


/* ==================================================
 * Via clavium
 * ================================================== */

/* proximum lexema substantivum (spatia omissa) */
interior MateriaToken*
_proximum (
    TomlLector* l)
{
    dum (VERUM)
    {
        MateriaToken* t = toml_lector_proximum(l, TOML_MODUS_CLAVIS);

        si (t == NIHIL || t->genus != (s32)TOML_LEX_SPATIUM)
        {
            redde t;
        }
    }
}

interior b32
_segmentum (
    s32 genus)
{
    redde genus == (s32)TOML_LEX_CLAVIS_NUDA
        || genus == (s32)TOML_LEX_CLAVIS_GEMINA
        || genus == (s32)TOML_LEX_CLAVIS_SIMPLEX;
}

constans TomlValor*
toml_quaerere (
    constans TomlDocumentum* doc,
         constans character* via_clavium)
{
    TomlLector  l;
     TomlValor* v;
           i32  n;

    si (doc == NIHIL || via_clavium == NIHIL)
    {
        redde NIHIL;
    }
    n = (i32)strlen(via_clavium);
    si (!toml_lector_incipere(&l, doc->piscina, via_clavium, (s32)n))
    {
        redde NIHIL;
    }
    v = doc->coctum.radix;
    dum (VERUM)
    {
        MateriaToken* t = _proximum(&l);
              chorda  clavis;

        /* segmentum */
        si (t == NIHIL || !_segmentum(t->genus))
        {
            redde NIHIL;
        }
        si (!toml_chordam_coquere(doc->piscina, t, &clavis, NIHIL))
        {
            redde NIHIL;   /* effugium pravum aut quotatio aperta */
        }
        v = toml_tabulae_filius(v, clavis);
        si (v == NIHIL)
        {
            redde NIHIL;
        }
        /* punctum aut finis */
        t = _proximum(&l);
        si (t == NIHIL)
        {
            redde NIHIL;
        }
        si (t->genus == (s32)TOML_LEX_FINIS)
        {
            redde v;
        }
        si (t->genus != (s32)TOML_LEX_PUNCTUM)
        {
            redde NIHIL;
        }
        si (v->genus != TOML_VALOR_TABULA)
        {
            redde NIHIL;   /* per seriem aut in scalare */
        }
    }
}

interior constans TomlValor*
_generis (
     constans TomlDocumentum* doc,
          constans character* via_clavium,
            TomlGenusValoris  genus)
{
    constans TomlValor* v = toml_quaerere(doc, via_clavium);

    redde v != NIHIL && v->genus == genus ? v : NIHIL;
}

b32
toml_chorda (
    constans TomlDocumentum* doc,
         constans character* via_clavium,
                     chorda* exitus)
{
    constans TomlValor* v = _generis(doc, via_clavium,
        TOML_VALOR_CHORDA);

    si (v == NIHIL || exitus == NIHIL)
    {
        redde FALSUM;
    }
    *exitus = v->datum.chorda_valor;
    redde VERUM;
}

b32
toml_integer (
    constans TomlDocumentum* doc,
         constans character* via_clavium,
                        s64* exitus)
{
    constans TomlValor* v = _generis(doc, via_clavium,
        TOML_VALOR_INTEGER);

    si (v == NIHIL || exitus == NIHIL)
    {
        redde FALSUM;
    }
    *exitus = v->datum.integer_valor;
    redde VERUM;
}

b32
toml_fluitans (
    constans TomlDocumentum* doc,
         constans character* via_clavium,
                        f64* exitus)
{
    constans TomlValor* v = _generis(doc, via_clavium,
                                TOML_VALOR_FLUITANS);

    si (v == NIHIL || exitus == NIHIL)
    {
        redde FALSUM;
    }
    *exitus = v->datum.fluitans_valor;
    redde VERUM;
}

b32
toml_boolean (
    constans TomlDocumentum* doc,
         constans character* via_clavium,
                        b32* exitus)
{
    constans TomlValor* v = _generis(doc, via_clavium,
        TOML_VALOR_BOOLEAN);

    si (v == NIHIL || exitus == NIHIL)
    {
        redde FALSUM;
    }
    *exitus = v->datum.boolean_valor;
    redde VERUM;
}

b32
toml_tempus (
    constans TomlDocumentum* doc,
         constans character* via_clavium,
                 TomlTempus* exitus)
{
    constans TomlValor* v = _generis(doc, via_clavium,
        TOML_VALOR_TEMPUS);

    si (v == NIHIL || exitus == NIHIL)
    {
        redde FALSUM;
    }
    *exitus = v->datum.tempus_valor;
    redde VERUM;
}


/* ==================================================
 * Enumeratio
 * ================================================== */

i32
toml_tabulae_numerus (
    constans TomlValor* tabula)
{
    redde tabula != NIHIL && tabula->genus == TOML_VALOR_TABULA
        ? xar_numerus(tabula->datum.tabula.claves) : ZEPHYRUM;
}

chorda
toml_tabulae_clavis (
    constans TomlValor* tabula,
                   i32  index)
{
    chorda vacua;

    si (index < toml_tabulae_numerus(tabula))
    {
        redde *(chorda*)xar_obtinere(tabula->datum.tabula.claves,
            index);
    }
    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    redde vacua;
}

constans TomlValor*
toml_tabulae_valor (
    constans TomlValor* tabula,
                   i32  index)
{
    redde index < toml_tabulae_numerus(tabula)
        ? *(TomlValor**)xar_obtinere(tabula->datum.tabula.valores,
        index)
        : NIHIL;
}

i32
toml_seriei_numerus (
    constans TomlValor* series)
{
    redde series != NIHIL && series->genus == TOML_VALOR_SERIES
        ? xar_numerus(series->datum.series) : ZEPHYRUM;
}

constans TomlValor*
toml_seriei_elementum (
    constans TomlValor* series,
                   i32  index)
{
    redde index < toml_seriei_numerus(series)
        ? *(TomlValor**)xar_obtinere(series->datum.series, index)
        : NIHIL;
}

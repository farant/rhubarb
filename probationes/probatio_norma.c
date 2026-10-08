/* probatio_norma.c - norma: genera, fines, formae, objecta, viae,
 * discrimen, limes, pravitas, visus (norma-plan-2 N2), exportatio (N3) */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "json.h"
#include "chorda_aedificator.h"
#include "norma.h"

#include <stdio.h>
#include <string.h>

interior NormaIudicium
_iud (
                 Norma* n,
    constans character* json,
               Piscina* p)
{
    redde norma_iudicare(n, json_legere_literis(json, p).radix, p);
}

interior NormaVitium*
_vit (
    NormaIudicium j,
              i32 i)
{
    redde (NormaVitium*)xar_obtinere(j.vitia, i);
}

interior NormaVitium*
_nota (
    NormaIudicium j,
              i32 i)
{
    redde (NormaVitium*)xar_obtinere(j.notae, i);
}

/* vitium unum exspectatum: causa + via */
interior vacuum
_unum (NormaIudicium j, NormaCausa causa, constans character* via)
{
    CREDO_FALSUM(j.validum);
    CREDO_AEQUALIS_I32(xar_numerus(j.vitia), I);
    si (xar_numerus(j.vitia) == I)
    {
        CREDO_VERUM(_vit(j, 0)->causa == causa);
        CREDO_CHORDA_AEQUALIS_LITERIS(_vit(j, 0)->via, via);
    }
}

interior vacuum
probatio_genera(Piscina* p)
{
    imprimere("\n--- Probans genera ---\n");
    CREDO_VERUM(_iud(norma_integer(p), "7", p).validum);
    _unum(_iud(norma_integer(p), "7.5", p), NORMA_CAUSA_GENUS, "$");
    _unum(_iud(norma_integer(p), "\"x\"", p), NORMA_CAUSA_GENUS, "$");
    _unum(_iud(norma_integer(p), "null", p), NORMA_CAUSA_GENUS, "$");
    CREDO_VERUM(_iud(norma_aut_nullum(norma_integer(p)), "null",
        p).validum);
    CREDO_VERUM(_iud(norma_numerus(p), "7", p).validum);
    CREDO_VERUM(_iud(norma_numerus(p), "7.5", p).validum);
    CREDO_VERUM(_iud(norma_boolean(p), "true", p).validum);
    CREDO_VERUM(_iud(norma_nullum(p), "null", p).validum);
    _unum(_iud(norma_nullum(p), "1", p), NORMA_CAUSA_GENUS, "$");
    CREDO_VERUM(_iud(norma_liberum(p), "null", p).validum);
    CREDO_VERUM(_iud(norma_liberum(p), "{\"a\":[1,2]}", p).validum);
    CREDO_VERUM(_iud(norma_textus(p), "\"x\"", p).validum);
    _unum(_iud(norma_textus(p), "1", p), NORMA_CAUSA_GENUS, "$");
}

interior vacuum
probatio_fines_et_formae(Piscina* p)
{
    constans character* constans licita[] = { "a", "b", NIHIL };

    imprimere("\n--- Probans fines, electio, formae ---\n");
    _unum(_iud(norma_intra(norma_integer(p), I, X), "0", p),
        NORMA_CAUSA_MINIMUM, "$");
    _unum(_iud(norma_intra(norma_integer(p), I, X), "11", p),
        NORMA_CAUSA_MAXIMUM, "$");
    CREDO_VERUM(_iud(norma_intra(norma_integer(p), I, X), "10",
        p).validum);
    _unum(_iud(norma_intra_fluitans(norma_numerus(p), 0.1, 0.3), "0.05",
        p),
          NORMA_CAUSA_MINIMUM, "$");
    _unum(_iud(norma_intra_fluitans(norma_numerus(p), 0.1, 0.3), "0.31",
        p),
          NORMA_CAUSA_MAXIMUM, "$");
    CREDO_VERUM(_iud(norma_intra_fluitans(norma_numerus(p), 0.1, 0.3),
        "0.3", p).validum);
    /* longitudo in RUNIS: "āē" = II runae, IV octeti */
    CREDO_VERUM(_iud(norma_longitudo(norma_textus(p), II, III),
                     "\"\xc4\x81\xc4\x93\"", p).validum);
    _unum(_iud(norma_longitudo(norma_textus(p), II, III), "\"a\"", p),
          NORMA_CAUSA_LONGITUDO, "$");
    _unum(_iud(norma_longitudo(norma_tabulatum(p, norma_integer(p)), I,
        II), "[]", p),
          NORMA_CAUSA_LONGITUDO, "$");
    _unum(_iud(norma_electio(norma_textus(p), licita), "\"c\"", p),
          NORMA_CAUSA_ELECTIO, "$");
    CREDO_VERUM(_iud(norma_forma(norma_textus(p), "date-time"),
                     "\"2026-10-08T10:27:15Z\"", p).validum);
    _unum(_iud(norma_forma(norma_textus(p), "date-time"), "\"heri\"",
        p),
          NORMA_CAUSA_FORMA, "$");
    CREDO_VERUM(_iud(norma_forma(norma_textus(p), "uuid"),
                     "\"123e4567-e89b-42d3-a456-426614174000\"",
                     p).validum);
    _unum(_iud(norma_forma(norma_textus(p), "uuid"), "\"123e4567\"", p),
          NORMA_CAUSA_FORMA, "$");
    CREDO_VERUM(_iud(norma_forma(norma_textus(p), "email"), "\"a@b.c\"",
        p).validum);
    _unum(_iud(norma_forma(norma_textus(p), "email"), "\"a@b\"", p),
          NORMA_CAUSA_FORMA, "$");
    CREDO_VERUM(_iud(norma_forma(norma_textus(p), "uri"),
        "\"https://x\"", p).validum);
    _unum(_iud(norma_forma(norma_textus(p), "uri"), "\"sine\"", p),
          NORMA_CAUSA_FORMA, "$");
    /* forma ignota: indicium solum, iudex tacet */
    CREDO_VERUM(_iud(norma_forma(norma_textus(p), "color"),
        "\"quidvis\"", p).validum);
}

interior vacuum
probatio_objecta_et_viae(Piscina* p)
{
            Norma* o = norma_objectum(p);
            Norma* apertum;
            Norma* notandum;
            Norma* vacuum_o;
    NormaIudicium  j;

    imprimere("\n--- Probans objecta, modi, viae ---\n");
    norma_campus(o, "id", norma_integer(p), VERUM);
    norma_campus(o, "n", norma_integer(p), FALSUM);
    CREDO_VERUM(_iud(o, "{\"id\":1}", p).validum);
    _unum(_iud(o, "{}", p), NORMA_CAUSA_DEEST, "$.id");
    _unum(_iud(o, "{\"id\":1,\"x\":2}", p), NORMA_CAUSA_EXTRA, "$.x");
    apertum = norma_modus(norma_campus(norma_objectum(p), "id",
        norma_integer(p), VERUM),
                          NORMA_APERTUM);
    j = _iud(apertum, "{\"id\":1,\"x\":2}", p);
    CREDO_VERUM(j.validum);
    CREDO_AEQUALIS_I32(xar_numerus(j.notae), 0);
    notandum = norma_modus(norma_campus(norma_objectum(p), "id",
        norma_integer(p), VERUM),
                           NORMA_NOTANDUM);
    j = _iud(notandum, "{\"id\":1,\"x\":2}", p);
    CREDO_VERUM(j.validum);
    CREDO_AEQUALIS_I32(xar_numerus(j.notae), I);
    CREDO_CHORDA_AEQUALIS_LITERIS(_nota(j, 0)->via, "$.x");
    CREDO_VERUM(_nota(j, 0)->causa == NORMA_CAUSA_EXTRA);
    /* viae cum claves rarae */
    vacuum_o = norma_objectum(p);
    _unum(_iud(vacuum_o, "{\"a.b\":1}", p), NORMA_CAUSA_EXTRA,
        "$[\"a.b\"]");
    _unum(_iud(vacuum_o, "{\"q\\\"\":1}", p), NORMA_CAUSA_EXTRA,
        "$[\"q\\\"\"]");
    _unum(_iud(vacuum_o, "{\"cl\xc4\x81vis\":1}", p), NORMA_CAUSA_EXTRA,
          "$[\"cl\xc4\x81vis\"]");
    _unum(_iud(norma_tabulatum(p, o), "[{\"id\":\"x\"}]", p),
        NORMA_CAUSA_GENUS, "$[0].id");
}

interior vacuum
probatio_discrimen(Piscina* p)
{
            Norma* textus   = norma_objectum(p);
            Norma* petitum  = norma_objectum(p);
            Norma* d        = norma_discrimen(p, "type");
            Norma* dn;
    NormaIudicium  j;

    imprimere("\n--- Probans discrimen ---\n");
    norma_campus(textus, "text", norma_textus(p), VERUM);
    norma_campus(petitum, "id", norma_textus(p), VERUM);
    norma_variatio(d, "text", textus);
    norma_variatio(d, "tool_use", petitum);
    CREDO_VERUM(_iud(d, "{\"type\":\"text\",\"text\":\"t\"}",
        p).validum);
    _unum(_iud(d, "{\"type\":\"tool_use\"}", p), NORMA_CAUSA_DEEST,
        "$.id");
    _unum(_iud(d, "{\"type\":\"novum\"}", p), NORMA_CAUSA_VARIATIO,
        "$.type");
    _unum(_iud(d, "{\"text\":\"t\"}", p), NORMA_CAUSA_DISCRIMEN,
        "$.type");
    _unum(_iud(d, "{\"type\":3}", p), NORMA_CAUSA_DISCRIMEN, "$.type");
    /* NOTANDUM: variatio ignota = nota, valor validus, fratres iudicati */
    dn = norma_modus(norma_discrimen(p, "type"), NORMA_NOTANDUM);
    norma_variatio(dn, "text", textus);
    j = _iud(norma_tabulatum(p, dn),
             "[{\"type\":\"novum\"},{\"type\":\"text\",\"text\":\"t\"}]",
             p);
    CREDO_VERUM(j.validum);
    CREDO_AEQUALIS_I32(xar_numerus(j.notae), I);
    CREDO_CHORDA_AEQUALIS_LITERIS(_nota(j, 0)->via, "$[0].type");
    j = _iud(norma_tabulatum(p, dn),
        "[{\"type\":\"novum\"},{\"type\":\"text\"}]", p);
    _unum(j, NORMA_CAUSA_DEEST, "$[1].text");
    CREDO_AEQUALIS_I32(xar_numerus(j.notae), I);
}

interior vacuum
probatio_limes(Piscina* p)
{
    ChordaAedificator* aed = chorda_aedificator_creare(p, M * VIII);
        NormaIudicium  j;
                  i32  i;

    imprimere("\n--- Probans limes CCLVI ---\n");
    chorda_aedificator_appendere_character(aed, '[');
    per (i = 0; i < M; i++)
    {
        chorda_aedificator_appendere_literis(aed,
            i ? ",\"x\"" : "\"x\"");
    }
    chorda_aedificator_appendere_character(aed, ']');
    j = norma_iudicare(norma_tabulatum(p, norma_integer(p)),
                       json_legere(chorda_aedificator_finire(aed),
                       p).radix, p);
    CREDO_FALSUM(j.validum);
    CREDO_AEQUALIS_I32(xar_numerus(j.vitia), CCLVII);
    CREDO_VERUM(_vit(j, CCLVI)->causa == NORMA_CAUSA_LIMES);
}

interior vacuum
probatio_pravitas_et_visus(Piscina* p)
{
         Norma* o = norma_objectum(p);
         Norma* n;
    NormaVisus  v;

    imprimere("\n--- Probans pravitas et visus ---\n");
    norma_campus(o, "a", NIHIL, VERUM);
    _unum(_iud(o, "{}", p), NORMA_CAUSA_SCHEMA_PRAVA, "$");
    _unum(_iud(norma_intra(norma_integer(p), V, I), "3", p),
        NORMA_CAUSA_SCHEMA_PRAVA, "$");
    _unum(_iud(norma_campus(norma_integer(p), "a", norma_textus(p),
        VERUM), "1", p),
          NORMA_CAUSA_SCHEMA_PRAVA, "$");
    o = norma_objectum(p);
    norma_campus(o, "a", norma_textus(p), VERUM);
    norma_campus(o, "a", norma_textus(p), VERUM);
    _unum(_iud(o, "{\"a\":\"x\"}", p), NORMA_CAUSA_SCHEMA_PRAVA, "$");
    /* pravitas in nodo filio quoque invenitur */
    _unum(_iud(norma_tabulatum(p, norma_intra(norma_integer(p), V, I)),
        "[]", p),
          NORMA_CAUSA_SCHEMA_PRAVA, "$");

    n = norma_descriptio(norma_intra(norma_integer(p), II, IX),
        "numerus");
    v = norma_visus(n);
    CREDO_VERUM(v.genus == NORMA_INTEGER);
    CREDO_VERUM(v.habet_intra);
    CREDO_AEQUALIS_S64(v.minimum, II);
    CREDO_AEQUALIS_S64(v.maximum, IX);
    CREDO_CHORDA_AEQUALIS_LITERIS(v.descriptio, "numerus");
    CREDO_CHORDA_VACUA(v.error_schematis);
    v = norma_visus(norma_intra(norma_integer(p), V, I));
    CREDO_CHORDA_NON_VACUA(v.error_schematis);
    o = norma_objectum(p);
    norma_campus(o, "a", norma_textus(p), VERUM);
    v = norma_visus(o);
    CREDO_AEQUALIS_I32(xar_numerus(v.campi), I);
}

s32
principale (vacuum)
{
    Piscina* p = piscina_generare_dynamicum("probatio_norma", M * M);
        b32  successus;

    credo_aperire(p);
    /* N2 */
    probatio_genera(p);
    probatio_fines_et_formae(p);
    probatio_objecta_et_viae(p);
    probatio_discrimen(p);
    probatio_limes(p);
    probatio_pravitas_et_visus(p);
    /* N3 addit hic vocationem suam */
    credo_imprimere_compendium();
    successus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(p);
    redde successus ? 0 : I;
}

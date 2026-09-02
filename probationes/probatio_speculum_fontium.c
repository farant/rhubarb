/* probatio_speculum_fontium.c - Probatio artificii generatoris speculi
 *
 * Probat capsulam a tools/speculum_generare.sh emissam (Phasis 0):
 * apertio, praesentia fontium clausurae, corpora vera (gzip
 * circumitus), documenta (worklogs), meta (proventus/exclusa/
 * fontes.txt), scripta. Artificium generatur a compile_tests.sh
 * (run_speculum) ANTE probationes - build/speculum/hospes/.
 *
 * Spec: project-specs/speculum-spec-v2.md
 */

#include "credo.h"
#include "piscina.h"
#include "chorda.h"
#include "capsula.h"

#include <stdio.h>
#include <string.h>

/* Symbolum ex capsula generata (build/speculum/hospes/) - externus
 * directus pro capite generato: nomen symboli est contractus
 * stabilis, et includere per "../build/" silva non resolvit */
/* <aedilis obiectum="build/speculum/hospes/capsula_speculi_hospes.c"/> */
externus constans CapsulaEmbed capsula_speculi_hospes;


/* ========================================================================
 * ADIUTORES
 * ======================================================================== */

interior b32
_continet (
                chorda  textus,
    constans character* acus)
{
    i32 acus_mensura;
    i32 i;

    acus_mensura = (i32)strlen(acus);
    si (acus_mensura == 0 || textus.mensura < acus_mensura)
    {
        redde FALSUM;
    }
    per (i = 0; i + acus_mensura <= textus.mensura; i++)
    {
        si (memcmp(textus.datum + i, acus, (size_t)acus_mensura) == 0)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}


/* ========================================================================
 * PROBATIONES
 * ======================================================================== */

interior vacuum
probatio_aperire(Capsula* cap)
{
    imprimere("  apertio et numerus...\n");
    CREDO_NON_NIHIL(cap);
    /* copia HONESTA ab aedile (clausura nexus, ~80), non mundus
     * necte-omnia (~300) - pinna utrimque limitata */
    CREDO_MAIOR_I32(capsula_numerus(cap), L);
    CREDO_MINOR_I32(capsula_numerus(cap), CC);
}

interior vacuum
probatio_fontes_praesentes(Capsula* cap)
{
    imprimere("  fontes clausurae praesentes...\n");
    CREDO_VERUM(capsula_habet(cap, "lib/piscina.c"));
    CREDO_VERUM(capsula_habet(cap, "include/piscina.h"));
    CREDO_VERUM(capsula_habet(cap, "include/latina.h"));
    CREDO_VERUM(capsula_habet(cap, "lib/hospitium.c"));
    CREDO_VERUM(capsula_habet(cap, "lib/vitrea_macos.m"));
    CREDO_VERUM(capsula_habet(cap,
        "probationes/probatio_vitrea_hospes.c"));
}

interior vacuum
probatio_corpus_verum(Capsula* cap, Piscina* piscina)
{
    CapsulaFructus  fructus;
    CapsulaIndexum* tabella;

    imprimere("  corpus verum (gzip circumitus)...\n");
    fructus = capsula_legere(cap, "lib/piscina.c", piscina);
    CREDO_AEQUALIS_I32((i32)fructus.status, (i32)CAPSULA_OK);
    CREDO_MAIOR_I32(fructus.datum.mensura, M);
    CREDO_VERUM(_continet(fructus.datum, "piscina_generare_dynamicum"));

    tabella = capsula_invenire(cap, "lib/piscina.c");
    CREDO_NON_NIHIL(tabella);
    si (tabella != NIHIL)
    {
        CREDO_VERUM(tabella->compressa);
        CREDO_AEQUALIS_I32(tabella->mensura_cruda,
            fructus.datum.mensura);
    }
}

interior vacuum
probatio_documenta(Capsula* cap, Piscina* piscina)
{
    CapsulaFructus fructus;

    imprimere("  documenta (worklogs)...\n");
    CREDO_VERUM(capsula_habet(cap, "lib/hospitium.worklog.md"));
    fructus = capsula_legere(cap, "lib/hospitium.worklog.md", piscina);
    CREDO_AEQUALIS_I32((i32)fructus.status, (i32)CAPSULA_OK);
    CREDO_CHORDA_NON_VACUA(fructus.datum);
}

interior vacuum
probatio_meta(Capsula* cap, Piscina* piscina)
{
    CapsulaFructus proventus;
    CapsulaFructus exclusa;
    CapsulaFructus fontes;

    imprimere("  meta: proventus / exclusa / fontes.txt...\n");
    proventus = capsula_legere(cap, "speculum/proventus.txt", piscina);
    CREDO_AEQUALIS_I32((i32)proventus.status, (i32)CAPSULA_OK);
    CREDO_VERUM(_continet(proventus.datum, "app=hospes"));
    CREDO_VERUM(_continet(proventus.datum, "commissum="));
    CREDO_VERUM(_continet(proventus.datum, "vexilla=-std=c89"));
    CREDO_VERUM(_continet(proventus.datum, "tempus="));
    /* mandatum = scriptum aedilis (via recompilationis vera,
     * porta probata), non linea clang manu structa */
    CREDO_VERUM(_continet(proventus.datum, "mandatum=bash"));
    CREDO_VERUM(_continet(proventus.datum, "manifestum:"));

    exclusa = capsula_legere(cap, "speculum/exclusa.txt", piscina);
    CREDO_AEQUALIS_I32((i32)exclusa.status, (i32)CAPSULA_OK);
    /* clausura honesta hospitis nec vendor nec >1MB fert - introitus
     * quine solus semper adest */
    CREDO_VERUM(_continet(exclusa.datum, "generatum"));
    CREDO_VERUM(_continet(exclusa.datum,
        "capsula_speculi_hospes.c"));

    /* manifestum aedilis in capsula - speculum veritatem
     * dependentiarum suarum secum fert */
    fontes = capsula_legere(cap, "speculum/manifestum.stml",
        piscina);
    CREDO_AEQUALIS_I32((i32)fontes.status, (i32)CAPSULA_OK);
    CREDO_VERUM(_continet(fontes.datum, "aedilis-manifestum"));
    CREDO_VERUM(_continet(fontes.datum, "lib/vitrea_macos.m"));

    fontes = capsula_legere(cap, "speculum/fontes.txt", piscina);
    CREDO_AEQUALIS_I32((i32)fontes.status, (i32)CAPSULA_OK);
    CREDO_VERUM(_continet(fontes.datum, "lib/piscina.c"));
}

interior vacuum
probatio_scripta(Capsula* cap)
{
    imprimere("  scripta (fabrica + generator)...\n");
    CREDO_VERUM(capsula_habet(cap, "compile_tests.sh"));
    CREDO_VERUM(capsula_habet(cap, "tools/speculum_generare.sh"));
}


/* ========================================================================
 * PRINCIPALE
 * ======================================================================== */

s32
principale (vacuum)
{
    Piscina* piscina;
    Capsula* cap;

    piscina = piscina_generare_dynamicum("probatio_speculum", 4194304);
    si (piscina == NIHIL)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);

    imprimere("PROBATIO SPECULUM FONTIUM\n");
    imprimere("=========================\n");

    cap = capsula_aperire(&capsula_speculi_hospes, piscina);
    probatio_aperire(cap);
    si (cap != NIHIL)
    {
        probatio_fontes_praesentes(cap);
        probatio_corpus_verum(cap, piscina);
        probatio_documenta(cap, piscina);
        probatio_meta(cap, piscina);
        probatio_scripta(cap);
    }

    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}

/* probatio_speculum.c - Probatio bibliothecae speculi (sine GUI)
 *
 * Missor fictus + aestimator fictus (mos probatio_internuntius):
 * tractatorem verum per internuntius_tractare agitamus, responsum
 * captum probamus; tangere cum eventibus fictis - stipes captus.
 * Capsula vera hospitis adhibetur (build/speculum/hospes/,
 * symbolum externum directum - mos consumptoris speculi).
 *
 * Spec: project-specs/speculum-spec-v2.md, Phasis A.
 */

#include "credo.h"
#include "piscina.h"
#include "chorda.h"
#include "capsula.h"
#include "internuntius.h"
#include "speculum.h"
#include "fenestra.h"

#include <stdio.h>
#include <string.h>

/* aedilis: obiectum build/speculum/hospes/capsula_speculi_hospes.c */
externus constans CapsulaEmbed capsula_speculi_hospes;


/* ========================================================================
 * FICTA (missor + aestimator - eadem forma)
 * ======================================================================== */

nomen structura {
    Piscina* piscina;
    chorda   ultima;
    i32      numerus;
} Captor;

interior vacuum
_captor_capere(vacuum* datum, chorda textus)
{
    Captor* captor;
    i8*     copia;

    captor = (Captor*)datum;
    copia = (i8*)piscina_allocare(captor->piscina,
        (memoriae_index)textus.mensura + I);
    memcpy(copia, textus.datum, (size_t)textus.mensura);
    captor->ultima.datum = copia;
    captor->ultima.mensura = textus.mensura;
    captor->numerus++;
}

interior b32
_continet(chorda textus, constans character* acus)
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

interior Eventus
_eventus_clavis(i32 clavis, i32 modificantes)
{
    Eventus eventus;

    memset(&eventus, 0, magnitudo(eventus));
    eventus.genus = EVENTUS_CLAVIS_DEPRESSUS;
    eventus.datum.clavis.clavis = (clavis_t)clavis;
    eventus.datum.clavis.modificantes = modificantes;
    redde eventus;
}


/* ========================================================================
 * PROBATIONES
 * ======================================================================== */

interior vacuum
probatio_creare(Speculum* spec)
{
    imprimere("  creatio...\n");
    CREDO_NON_NIHIL(spec);
}

interior vacuum
probatio_praebere_duplicatum(Piscina* piscina, Internuntius* inx)
{
    Speculum* alter;

    imprimere("  duplicatum recusatur...\n");
    alter = speculum_creare(piscina, &capsula_speculi_hospes, inx,
        NIHIL, NIHIL);
    CREDO_NIHIL(alter);
}

interior vacuum
probatio_obtinere(Internuntius* inx, Captor* missor, Piscina* piscina)
{
    chorda   petitio;
    Piscina* pv;

    imprimere("  effusio una (speculum_obtinere)...\n");
    pv = piscina_generare_dynamicum("probatio_speculum_pv", 16777216);
    CREDO_NON_NIHIL(pv);
    petitio = chorda_ex_literis(
        "{\"id\":1,\"methodus\":\"speculum_obtinere\",\"argumenta\":{}}",
        piscina);
    internuntius_tractare(inx, petitio, pv);

    CREDO_AEQUALIS_I32(missor->numerus, (i32)I);
    CREDO_VERUM(_continet(missor->ultima, "\"fructus\""));
    CREDO_VERUM(_continet(missor->ultima, "\"proventus\""));
    CREDO_VERUM(_continet(missor->ultima, "\"app\":\"hospes\""));
    CREDO_VERUM(_continet(missor->ultima, "\"commissum\""));
    CREDO_VERUM(_continet(missor->ultima, "\"fontes\""));
    CREDO_VERUM(_continet(missor->ultima, "lib/piscina.c"));
    CREDO_VERUM(_continet(missor->ultima, "piscina_generare_dynamicum"));
    CREDO_VERUM(_continet(missor->ultima, "\"documenta\""));
    CREDO_VERUM(_continet(missor->ultima, "hospitium.worklog.md"));
    CREDO_VERUM(_continet(missor->ultima, "\"exclusa\""));
    CREDO_VERUM(_continet(missor->ultima, "vendor/sqlite3.c"));
    CREDO_VERUM(_continet(missor->ultima, "magnitudo>1MB"));

    piscina_destruere(pv);
}

interior vacuum
probatio_tangere(Speculum* spec, Captor* aestimator)
{
    Eventus eventus;
    b32     consumptus;

    imprimere("  tangere (Cmd+Shift+D)...\n");

    /* clavis sine modificantibus - non consumptus */
    eventus = _eventus_clavis((i32)'D', 0);
    consumptus = speculum_tangere(spec, &eventus);
    CREDO_FALSUM(consumptus);
    CREDO_AEQUALIS_I32(aestimator->numerus, 0);

    /* Cmd+D sine Shift - non consumptus */
    eventus = _eventus_clavis((i32)'D', (i32)MOD_SUPER);
    consumptus = speculum_tangere(spec, &eventus);
    CREDO_FALSUM(consumptus);

    /* Cmd+Shift+D - consumptus, stipes evaluatus */
    eventus = _eventus_clavis((i32)'D',
        (i32)MOD_SUPER | (i32)MOD_SHIFT);
    consumptus = speculum_tangere(spec, &eventus);
    CREDO_VERUM(consumptus);
    CREDO_AEQUALIS_I32(aestimator->numerus, (i32)I);
    CREDO_VERUM(_continet(aestimator->ultima, "speculum_praesens"));
    CREDO_VERUM(_continet(aestimator->ultima, "speculum_commutare"));
    CREDO_VERUM(_continet(aestimator->ultima, "bona ex capsula"));
    CREDO_VERUM(_continet(aestimator->ultima, "speculum_stylus_fons"));

    /* iterum - stipes idem, se-sanans (nulla memoria C de pagina) */
    eventus = _eventus_clavis((i32)'D',
        (i32)MOD_SUPER | (i32)MOD_SHIFT);
    consumptus = speculum_tangere(spec, &eventus);
    CREDO_VERUM(consumptus);
    CREDO_AEQUALIS_I32(aestimator->numerus, (i32)II);

    /* alia clavis cum modificantibus - non consumpta */
    eventus = _eventus_clavis((i32)'A',
        (i32)MOD_SUPER | (i32)MOD_SHIFT);
    consumptus = speculum_tangere(spec, &eventus);
    CREDO_FALSUM(consumptus);
    CREDO_AEQUALIS_I32(aestimator->numerus, (i32)II);
}


/* ========================================================================
 * PRINCIPALE
 * ======================================================================== */

s32 principale(vacuum)
{
    Piscina*      piscina;
    Internuntius* inx;
    Speculum*     spec;
    Captor        missor;
    Captor        aestimator;

    piscina = piscina_generare_dynamicum("probatio_speculum", 33554432);
    si (piscina == NIHIL)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);

    imprimere("PROBATIO SPECULUM\n");
    imprimere("=================\n");

    memset(&missor, 0, magnitudo(missor));
    memset(&aestimator, 0, magnitudo(aestimator));
    missor.piscina = piscina;
    aestimator.piscina = piscina;

    inx = internuntius_creare(piscina, _captor_capere, &missor);
    spec = speculum_creare(piscina, &capsula_speculi_hospes, inx,
        _captor_capere, &aestimator);

    probatio_creare(spec);
    si (spec != NIHIL)
    {
        probatio_praebere_duplicatum(piscina, inx);
        probatio_obtinere(inx, &missor, piscina);
        probatio_tangere(spec, &aestimator);
    }

    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}

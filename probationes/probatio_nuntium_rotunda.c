/* generare: nuntium_schema_generare probationes/persona.nuntium probationes/persona_gen.h probationes/persona_gen.c */

/* probatio_nuntium_rotunda.c - Probatio Rotunda (Round-Trip)
 *
 * Probat serialisationem et deserialisationem Persona nuntii:
 * struct -> scribere -> bytes -> legere -> struct -> verificare
 *
 * Hoc est primum usum reale generare + nuntium_schema_generare.
 */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "nuntium.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#include "persona_gen.h"
#include "persona_gen.c"

s32 principale(vacuum)
{
    Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_nuntium_rotunda", MMMMXCVI);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ========================================================
     * PROBARE I: Scribere et legere — campi basici
     * ======================================================== */
    {
        NuntiumScriptor* scriptor;
        NuntiumLector*   lector;
        Persona          originale;
        Persona*         lecta;
        chorda           datum;
        chorda           id_chorda;
        chorda           appellatio_chorda;

        imprimere("\n--- Probans I: campi basici rotunda ---\n");

        scriptor = nuntium_scriptor_creare(piscina, CCLVI);
        CREDO_NON_NIHIL(scriptor);

        /* Praeparare chordas */
        id_chorda = chorda_ex_literis("ABC", piscina);
        appellatio_chorda = chorda_ex_literis("Marcus", piscina);

        /* Populare originale */
        memset(&originale, ZEPHYRUM, magnitudo(Persona));
        originale.id = &id_chorda;
        originale.aetas = XXV;
        originale.appellatio = &appellatio_chorda;

        /* Serialisare */
        CREDO_VERUM(persona_scribere(scriptor, &originale));
        datum = nuntium_scriptor_datum(scriptor);
        CREDO_VERUM(datum.mensura > ZEPHYRUM);

        /* Deserialisare */
        lector = nuntium_lector_creare(piscina, datum);
        CREDO_NON_NIHIL(lector);

        lecta = persona_legere(lector, piscina);
        CREDO_NON_NIHIL(lecta);

        /* Verificare */
        CREDO_NON_NIHIL(lecta->id);
        CREDO_VERUM(chorda_aequalis(*lecta->id, *originale.id));
        CREDO_AEQUALIS_S64((s64)lecta->aetas, (s64)originale.aetas);
        CREDO_NON_NIHIL(lecta->appellatio);
        CREDO_VERUM(chorda_aequalis(*lecta->appellatio, *originale.appellatio));

        imprimere("  campi basici -> OK\n");
    }


    /* ========================================================
     * PROBARE II: Campi vacui (nulls)
     * ======================================================== */
    {
        NuntiumScriptor* scriptor;
        NuntiumLector*   lector;
        Persona          originale;
        Persona*         lecta;
        chorda           datum;

        imprimere("\n--- Probans II: campi vacui ---\n");

        scriptor = nuntium_scriptor_creare(piscina, CCLVI);
        CREDO_NON_NIHIL(scriptor);

        /* Persona vacua — textus campi sunt NIHIL, aetas = 0 */
        memset(&originale, ZEPHYRUM, magnitudo(Persona));
        originale.id = NIHIL;
        originale.aetas = ZEPHYRUM;
        originale.appellatio = NIHIL;

        /* Serialisare */
        CREDO_VERUM(persona_scribere(scriptor, &originale));
        datum = nuntium_scriptor_datum(scriptor);

        /* Aetas = 0 adhuc serialisatur (campus numericus semper scribitur) */
        CREDO_VERUM(datum.mensura > ZEPHYRUM);

        /* Deserialisare */
        lector = nuntium_lector_creare(piscina, datum);
        CREDO_NON_NIHIL(lector);

        lecta = persona_legere(lector, piscina);
        CREDO_NON_NIHIL(lecta);

        /* Textus campi debent esse NIHIL quia non serialisati */
        CREDO_NIHIL(lecta->id);
        CREDO_NIHIL(lecta->appellatio);
        CREDO_AEQUALIS_S64((s64)lecta->aetas, (s64)ZEPHYRUM);

        imprimere("  campi vacui -> OK\n");
    }


    /* ========================================================
     * PROBARE III: Campi longi
     * ======================================================== */
    {
        NuntiumScriptor* scriptor;
        NuntiumLector*   lector;
        Persona          originale;
        Persona*         lecta;
        chorda           datum;
        chorda           id_longa;
        chorda           appellatio_longa;
        character        buffer_id[CCLVI];
        character        buffer_app[CCLVI];
        i32              i;

        imprimere("\n--- Probans III: campi longi ---\n");

        /* Creare chordas longas */
        per (i = ZEPHYRUM; i < CCLV; i++)
        {
            buffer_id[i] = 'A';
            buffer_app[i] = 'Z';
        }
        buffer_id[CCLV] = '\0';
        buffer_app[CCLV] = '\0';

        id_longa = chorda_ex_literis(buffer_id, piscina);
        appellatio_longa = chorda_ex_literis(buffer_app, piscina);

        scriptor = nuntium_scriptor_creare(piscina, M);
        CREDO_NON_NIHIL(scriptor);

        memset(&originale, ZEPHYRUM, magnitudo(Persona));
        originale.id = &id_longa;
        originale.aetas = CMLXXIX;
        originale.appellatio = &appellatio_longa;

        /* Serialisare */
        CREDO_VERUM(persona_scribere(scriptor, &originale));
        datum = nuntium_scriptor_datum(scriptor);
        CREDO_VERUM(datum.mensura > ZEPHYRUM);

        /* Deserialisare */
        lector = nuntium_lector_creare(piscina, datum);
        CREDO_NON_NIHIL(lector);

        lecta = persona_legere(lector, piscina);
        CREDO_NON_NIHIL(lecta);

        /* Verificare */
        CREDO_NON_NIHIL(lecta->id);
        CREDO_AEQUALIS_I32(lecta->id->mensura, CCLV);
        CREDO_VERUM(chorda_aequalis(*lecta->id, *originale.id));
        CREDO_AEQUALIS_S64((s64)lecta->aetas, (s64)CMLXXIX);
        CREDO_NON_NIHIL(lecta->appellatio);
        CREDO_AEQUALIS_I32(lecta->appellatio->mensura, CCLV);
        CREDO_VERUM(chorda_aequalis(*lecta->appellatio, *originale.appellatio));

        imprimere("  campi longi -> OK\n");
    }


    /* ========================================================
     * PROBARE IV: Scribere bis (reuse scriptor cum purgare)
     * ======================================================== */
    {
        NuntiumScriptor* scriptor;
        NuntiumLector*   lector;
        Persona          persona_a;
        Persona          persona_b;
        Persona*         lecta_a;
        Persona*         lecta_b;
        chorda           datum_a;
        chorda           datum_b;
        chorda           id_a;
        chorda           id_b;
        chorda           app_a;
        chorda           app_b;

        imprimere("\n--- Probans IV: scribere bis cum purgare ---\n");

        scriptor = nuntium_scriptor_creare(piscina, CCLVI);
        CREDO_NON_NIHIL(scriptor);

        /* Prima persona */
        id_a = chorda_ex_literis("P001", piscina);
        app_a = chorda_ex_literis("Gaius", piscina);

        memset(&persona_a, ZEPHYRUM, magnitudo(Persona));
        persona_a.id = &id_a;
        persona_a.aetas = XXX;
        persona_a.appellatio = &app_a;

        CREDO_VERUM(persona_scribere(scriptor, &persona_a));
        datum_a = nuntium_scriptor_datum(scriptor);

        /* Copiare datum_a ante purgare (datum internale erit invalidum) */
        datum_a = chorda_transcribere(datum_a, piscina);
        CREDO_VERUM(datum_a.mensura > ZEPHYRUM);

        /* Purgare et scribere secundam */
        nuntium_scriptor_purgare(scriptor);

        id_b = chorda_ex_literis("P002", piscina);
        app_b = chorda_ex_literis("Titus", piscina);

        memset(&persona_b, ZEPHYRUM, magnitudo(Persona));
        persona_b.id = &id_b;
        persona_b.aetas = XLV;
        persona_b.appellatio = &app_b;

        CREDO_VERUM(persona_scribere(scriptor, &persona_b));
        datum_b = nuntium_scriptor_datum(scriptor);
        CREDO_VERUM(datum_b.mensura > ZEPHYRUM);

        /* Deserialisare ambas */
        lector = nuntium_lector_creare(piscina, datum_a);
        lecta_a = persona_legere(lector, piscina);
        CREDO_NON_NIHIL(lecta_a);

        lector = nuntium_lector_creare(piscina, datum_b);
        lecta_b = persona_legere(lector, piscina);
        CREDO_NON_NIHIL(lecta_b);

        /* Verificare prima */
        CREDO_NON_NIHIL(lecta_a->id);
        CREDO_VERUM(chorda_aequalis_literis(*lecta_a->id, "P001"));
        CREDO_AEQUALIS_S64((s64)lecta_a->aetas, (s64)XXX);
        CREDO_NON_NIHIL(lecta_a->appellatio);
        CREDO_VERUM(chorda_aequalis_literis(*lecta_a->appellatio, "Gaius"));

        /* Verificare secunda */
        CREDO_NON_NIHIL(lecta_b->id);
        CREDO_VERUM(chorda_aequalis_literis(*lecta_b->id, "P002"));
        CREDO_AEQUALIS_S64((s64)lecta_b->aetas, (s64)XLV);
        CREDO_NON_NIHIL(lecta_b->appellatio);
        CREDO_VERUM(chorda_aequalis_literis(*lecta_b->appellatio, "Titus"));

        imprimere("  scribere bis -> OK\n");
    }


    /* ========================================================
     * PROBARE V: Bytes crudos verificare
     * ======================================================== */
    {
        NuntiumScriptor* scriptor;
        NuntiumLector*   lector;
        Persona          originale;
        Persona*         lecta;
        chorda           datum;
        chorda           id_chorda;

        imprimere("\n--- Probans V: bytes crudos verificare ---\n");

        scriptor = nuntium_scriptor_creare(piscina, CCLVI);
        CREDO_NON_NIHIL(scriptor);

        id_chorda = chorda_ex_literis("XYZ", piscina);

        memset(&originale, ZEPHYRUM, magnitudo(Persona));
        originale.id = &id_chorda;
        originale.aetas = VII;
        originale.appellatio = NIHIL;

        /* Serialisare */
        CREDO_VERUM(persona_scribere(scriptor, &originale));
        datum = nuntium_scriptor_datum(scriptor);

        /* Verificare bytes non vacui */
        CREDO_VERUM(datum.mensura > ZEPHYRUM);
        CREDO_NON_NIHIL(datum.datum);

        /* Deserialisare et verificare */
        lector = nuntium_lector_creare(piscina, datum);
        CREDO_NON_NIHIL(lector);

        lecta = persona_legere(lector, piscina);
        CREDO_NON_NIHIL(lecta);

        CREDO_NON_NIHIL(lecta->id);
        CREDO_VERUM(chorda_aequalis_literis(*lecta->id, "XYZ"));
        CREDO_AEQUALIS_S64((s64)lecta->aetas, (s64)VII);
        CREDO_NIHIL(lecta->appellatio);

        imprimere("  bytes crudos -> OK\n");
    }


    /* ========================================================
     * Compendium
     * ======================================================== */
    imprimere("\n");
    credo_imprimere_compendium();

    piscina_destruere(piscina);

    redde (credo_omnia_praeterierunt() ? ZEPHYRUM : I);
}

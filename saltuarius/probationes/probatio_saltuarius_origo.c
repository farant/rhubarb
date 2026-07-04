/* probatio_saltuarius_origo.c - Catena originis + vestigium
 * identitatis (Chunk C2)
 *
 * Fixum: "#define G(x) ((x)+(x))\nG(2);\n" - probantur: catena
 * DEORSUM in strato I (lexema expansum -> expansio G cum situ
 * definitionis -> scriptum), quaestio PRORSUM in strato 0 (cursor
 * in G -> quid fit), navigatio/saltus tabellae, vestigium cursoris
 * trans strata AMBOBUS directionibus.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "saltuarius_origo.h"
#include <stdio.h>
#include <string.h>

s32 principale (vacuum)
{
    b32      praeteritus;
    Piscina* piscina;
    SaltuariusNexus* nexus;
    SaltuariusLiber* liber;
    SaltuariusOrigo* origo;
    constans character* FONS = "#define G(x) ((x)+(x))\nG(2);\n";

    piscina = piscina_generare_dynamicum("probatio_salt_origo",
        8388608);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    nexus = saltuarius_nexus_creare(piscina);
    CREDO_NON_NIHIL (nexus);
    CREDO_AEQUALIS_S32 (
        saltuarius_nexus_silvam_parare(nexus, NIHIL, ZEPHYRUM),
        ZEPHYRUM);
    liber = saltuarius_liber_aperire(piscina, nexus,
        chorda_ex_literis("probe.c", piscina),
        chorda_ex_literis(FONS, piscina));
    CREDO_NON_NIHIL (liber);
    CREDO_NON_NIHIL (liber->parsura);
    origo = saltuarius_origo_creare(piscina);
    CREDO_NON_NIHIL (origo);

    /* ========================================================
     * PROBARE: catena DEORSUM (stratum I, cursor in expanso)
     * ======================================================== */
    {
        constans SaltuariusStratum* stratum;
        constans character* textus_literis;
        constans character* plus_situs;

        imprimere("\n--- Probans catenam deorsum ---\n");

        saltuarius_liber_stratum_transferre(liber, nexus, I);
        stratum = saltuarius_liber_stratum(liber, nexus);
        CREDO_VERUM (stratum->parata);
        textus_literis = chorda_ut_cstr(stratum->textus, piscina);
        plus_situs = strstr(textus_literis, "+");
        CREDO_NON_NIHIL ((constans vacuum*)plus_situs);

        /* cursor in '+' (lexema expansum ex corpore G) */
        saltuarius_liber_cursor_ad_offset(liber,
            (s32)(plus_situs - textus_literis));
        CREDO_VERUM (saltuarius_origo_aedificare(origo, liber,
            nexus));
        CREDO_VERUM (origo->apertum);
        imprimere("  gradus: %d\n", (int)origo->numerus);
        {
            i32 k;

            per (k = ZEPHYRUM; k < origo->numerus; k++)
            {
                imprimere("  [%d] %.*s\n", (int)k,
                    (int)origo->gradus[k].titulus.mensura,
                    (constans character*)
                        origo->gradus[k].titulus.datum);
            }
        }
        /* forma: CAPUT, EXPANSIO (G, saltabile, def linea I),
         * SCRIPTUM (invocatio linea II) */
        CREDO_MAIOR_I32 ((i32)origo->numerus, II);
        CREDO_AEQUALIS_I32 ((i32)origo->gradus[ZEPHYRUM].genus,
            (i32)SALT_GRADUS_CAPUT);
        CREDO_AEQUALIS_I32 ((i32)origo->gradus[I].genus,
            (i32)SALT_GRADUS_EXPANSIO);
        CREDO_VERUM (origo->gradus[I].saltabile);
        CREDO_AEQUALIS_I32 (origo->gradus[I].linea, I);
        CREDO_AEQUALIS_I32 (
            (i32)origo->gradus[origo->numerus - I].genus,
            (i32)SALT_GRADUS_SCRIPTUM);
        CREDO_AEQUALIS_I32 (
            origo->gradus[origo->numerus - I].linea, II);

        /* navigatio + saltus: selectio 0 (CAPUT) non saltabilis */
        CREDO_AEQUALIS_PTR (
            (constans vacuum*)saltuarius_origo_saltus(origo),
            (constans vacuum*)NIHIL);
        saltuarius_origo_movere(origo, I);
        CREDO_NON_NIHIL (
            (constans vacuum*)saltuarius_origo_saltus(origo));
        CREDO_AEQUALIS_I32 (saltuarius_origo_saltus(origo)->linea,
            I);
        saltuarius_origo_movere(origo, C);
        CREDO_AEQUALIS_S32 (origo->selectio,
            (s32)origo->numerus - I);
    }

    /* ========================================================
     * PROBARE: quaestio PRORSUM (stratum 0, cursor in G)
     * ======================================================== */
    {
        constans character* g_situs = strstr(FONS, "G(2)");

        imprimere("\n--- Probans quaestionem prorsum ---\n");

        saltuarius_liber_stratum_transferre(liber, nexus,
            ZEPHYRUM);
        saltuarius_liber_cursor_ad_offset(liber,
            (s32)(g_situs - FONS));
        CREDO_VERUM (saltuarius_origo_aedificare(origo, liber,
            nexus));
        CREDO_AEQUALIS_I32 ((i32)origo->gradus[ZEPHYRUM].genus,
            (i32)SALT_GRADUS_FIT);
        CREDO_MAIOR_I32 ((i32)origo->numerus, I);

        /* cursor in loco sine expansione: nihil narrabile */
        saltuarius_liber_cursor_ad_offset(liber, ZEPHYRUM);
        CREDO_FALSUM (saltuarius_origo_aedificare(origo, liber,
            nexus));
        CREDO_FALSUM (origo->apertum);
    }

    /* ========================================================
     * PROBARE: vestigium identitatis trans strata
     * ======================================================== */
    {
        constans character* g_situs = strstr(FONS, "G(2)");
        s32 offset;

        imprimere("\n--- Probans vestigium identitatis ---\n");

        /* 0 -> I: cursor in G -> in expansione eius */
        saltuarius_liber_stratum_ponere(liber, ZEPHYRUM);
        saltuarius_liber_cursor_ad_offset(liber,
            (s32)(g_situs - FONS));
        saltuarius_liber_stratum_transferre(liber, nexus, I);
        offset = saltuarius_liber_cursor_offset(liber);
        CREDO_VERUM (offset >= ZEPHYRUM);
        {
            constans SaltuariusStratum* stratum =
                saltuarius_liber_stratum(liber, nexus);

            CREDO_AEQUALIS_I32 (
                (i32)stratum->textus.datum[offset], (i32)'(');
        }

        /* I -> 0: retro ad radicem (G) */
        saltuarius_liber_stratum_transferre(liber, nexus,
            ZEPHYRUM);
        offset = saltuarius_liber_cursor_offset(liber);
        CREDO_VERUM (offset >= ZEPHYRUM);
        CREDO_AEQUALIS_I32 ((i32)liber->textus.datum[offset],
            (i32)'G');
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}

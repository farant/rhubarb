/* probatio_piscina.c - Probationes Piscinae */
#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>

s32 principale (vacuum)
{
               b32  praeteritus;
    memoriae_index  usus;
           Piscina* piscina;

    
    /* Aperire credo et piscina */
    piscina = piscina_generare_dynamicum("probatio_piscina", 4096);
    si (!piscina) 
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ========================================================
     * PROBARE: piscina_generare_dynamicum
     * ======================================================== */

    {
        Piscina* p;
        
        imprimere("\n--- Probans piscina_generare_dynamicum ---\n");

        p = piscina_generare_dynamicum("test_dynamica",  CCLVI);
        CREDO_NON_NIHIL (p);

        usus = piscina_summa_usus(p);
        CREDO_AEQUALIS_I32 ((i32)usus, ZEPHYRUM);

        piscina_destruere(p);
    }


    /* ========================================================
     * PROBARE: piscina_generare_certae_magnitudinis
     * ======================================================== */

    {

        Piscina* p;

        imprimere("\n--- Probans piscina_generare_certae_magnitudinis ---\n");
        p = piscina_generare_certae_magnitudinis("probare_fixa", DXII);

        usus = piscina_summa_usus(p);
        CREDO_AEQUALIS_I32 ((i32)usus, ZEPHYRUM);

        piscina_destruere(p);
    }


    /* ========================================================
     * PROBARE: piscina_allocare - allocatio singularis
     * ======================================================== */

    {

        Piscina* p;
         vacuum* allocatus;

        imprimere("\n--- Probans piscina_allocare (singularis) ---\n");

        p = piscina_generare_dynamicum("probatio_alloc1", CCLVI);
        CREDO_NON_NIHIL (p);

        allocatus = piscina_allocare(p, CXXVIII);
        CREDO_NON_NIHIL (allocatus);

        usus = piscina_summa_usus(p);
        CREDO_AEQUALIS_I32 ((i32)usus, CXXVIII);

        piscina_destruere(p);
    }


    /* ========================================================
     * PROBARE: piscina_allocare - allocationes multiplices
     * ======================================================== */

    {
        Piscina* p;
         vacuum* a1;
         vacuum* a2;
         vacuum* a3;

        imprimere("\n--- Probans piscina_allocare (multiplex) ---\n");

        p = piscina_generare_dynamicum("probatio_alloc_multi", CCLVI);
        CREDO_NON_NIHIL (p);

        a1 = piscina_allocare(p, LXIV);
        CREDO_NON_NIHIL (a1);

        a2 = piscina_allocare(p, LXIV);
        CREDO_NON_NIHIL (a2);

        a3 = piscina_allocare(p, LXIV);
        CREDO_NON_NIHIL (a3);

        usus = piscina_summa_usus(p);
        CREDO_AEQUALIS_I32 ((i32)usus, CXCII);

        piscina_destruere(p);
    }


    /* ========================================================
     * PROBARE: piscina_allocare - crescentia dynamica
     * ======================================================== */

    {
        Piscina* p;
         vacuum* allocatus;

        imprimere("\n--- Probans piscina_allocare (cresco) ---\n");

        p = piscina_generare_dynamicum("probatio_crescentia", CXXVIII);
        CREDO_NON_NIHIL (p);

        /* Allocare maiorem quam alveum initialem */
        allocatus = piscina_allocare(p, CCLVI);
        CREDO_NON_NIHIL (allocatus);

        usus = piscina_summa_usus(p);
        CREDO_AEQUALIS_I32 ((i32)usus, CCLVI);

        piscina_destruere(p);
    }


    /* ========================================================
     * PROBARE: piscina_conari_allocare - casus successus
     * ======================================================== */

    {
        Piscina* p;
         vacuum* allocatus;

        imprimere("\n--- Probans piscina_conari_allocare (successus) ---\n");

        p = piscina_generare_dynamicum("probare_conari_allocare", CCLVI);

        allocatus = piscina_conari_allocare(p, CXXVIII);
        CREDO_NON_NIHIL (allocatus);
        
        piscina_destruere(p);
    }

    /* ========================================================
     * PROBARE: piscina_conari_allocare - casus fracti (certae)
     * ======================================================== */

    {
        Piscina* p;
         vacuum* allocatus;

        imprimere("\n--- Probans piscina_conari_allocare (fracta) ---\n");

        p = piscina_generare_certae_magnitudinis("probare_conari_allocare", CXXVIII);
        CREDO_NON_NIHIL (p);

        /* Allocare plus quam capacitate */
        allocatus = piscina_conari_allocare(p, CCLVI);
        CREDO_NIHIL (allocatus);

        piscina_destruere(p);
    }


    /* ========================================================
     * PROBARE: piscina_allocare_ordinatum - ordinatum
     * ======================================================== */

    {
               Piscina* p;
                vacuum* ptr_non_ordinatum;
                vacuum* ptr_ordinatum;
        memoriae_index  offset_ordinatum;

        imprimere("\n--- Probans piscina_allocare_ordinatum ---\n");

        p = piscina_generare_dynamicum("test_alignment", DXII);

        CREDO_NON_NIHIL (p);

        /* Allocare non-ordinatum primum */
        ptr_non_ordinatum  = piscina_allocare(p, VII);
        offset_ordinatum = (memoriae_index)(void*)ptr_non_ordinatum;

        /* Allocare XVI-byte ordinatum */
        ptr_ordinatum    = piscina_allocare_ordinatum(p, LXIV, XVI);
        offset_ordinatum = (memoriae_index)(void*)ptr_ordinatum;

        /* Confer si ordinatum aptatum est */
        CREDO_NON_NIHIL    (ptr_ordinatum);
        CREDO_AEQUALIS_S32 (offset_ordinatum % XVI, ZEPHYRUM);

        piscina_destruere(p);
    }


    /* ========================================================
     * PROBARE: piscina_vacare 
     * ======================================================== */

    {
               Piscina* p;
                vacuum* a1;
                vacuum* a2;
        memoriae_index  ante;
        memoriae_index  post;

        imprimere("\n--- Probans piscina_vacare ---\n");

        p = piscina_generare_dynamicum("probatio_reliqua", CCLVI);
        CREDO_NON_NIHIL (p);

        a1   = piscina_allocare(p, C);
        ante = piscina_summa_usus(p);
        CREDO_NON_NIHIL    (a1);
        CREDO_AEQUALIS_I32 ((i32)ante, C);

        piscina_vacare(p);
        post = piscina_summa_usus(p);
        CREDO_AEQUALIS_I32 ((i32)post, ZEPHYRUM);

        /* Debet allocare iterum ex initio */
        a2 = piscina_allocare(p, L);
        CREDO_NON_NIHIL    (a2);
        CREDO_AEQUALIS_I32 ((i32)piscina_summa_usus(p), L);

        piscina_destruere(p);
    }



    /* ========================================================
     * PROBARE: piscina_summa_inutilis_allocatus
     * ======================================================== */

    {
               Piscina* p;
        memoriae_index  usus;
        memoriae_index  inutilis;
        memoriae_index  totalis;

        imprimere("\n--- Probans piscina_summa_inutilis_allocatus ---\n");

        p = piscina_generare_dynamicum("probatio_inutilis", CCLVI);
        CREDO_NON_NIHIL (p);

        piscina_allocare(p, C);

        usus     = piscina_summa_usus(p);
        inutilis = piscina_summa_inutilis_allocatus(p);

        /* usus + inutilis = CCLVI */
        totalis = usus + inutilis;
        CREDO_AEQUALIS_I32 ((i32)totalis, CCLVI);

        piscina_destruere(p);
    }


    /* ========================================================
     * PROBARE: piscina_reliqua_antequam_cresca_alvei
     * ======================================================== */

    {
               Piscina* p;
        memoriae_index  reliqua_initia;
        memoriae_index  reliqua_post;

        imprimere("\n--- Probans piscina_reliqua_antequam_cresca_alvei ---\n");

        p = piscina_generare_dynamicum("probatio_reliqua", CCLVI);
        CREDO_NON_NIHIL (p);

        reliqua_initia = piscina_reliqua_antequam_cresca_alvei(p);
        CREDO_AEQUALIS_I32 ((i32)reliqua_initia, CCLVI);

        piscina_allocare(p, C);
        reliqua_post = piscina_reliqua_antequam_cresca_alvei(p);
        CREDO_AEQUALIS_I32 ((i32)reliqua_post, CLVI);

        piscina_destruere(p);
    }

    /* ========================================================
     * PROBARE: piscina_summa_apex_usus
     * ======================================================== */

    {
               Piscina* p;
        memoriae_index  apex1;
        memoriae_index  apex2;

        imprimere("\n--- Probans piscina_summa_apex_usus ---\n");

        p = piscina_generare_dynamicum("probatio_apex", CCLVI);
        CREDO_NON_NIHIL (p);

        apex1 = piscina_summa_apex_usus(p);
        CREDO_AEQUALIS_I32 ((i32)apex1, ZEPHYRUM);

        piscina_allocare(p, CL);
        apex2 = piscina_summa_apex_usus(p);
        CREDO_AEQUALIS_I32 ((i32)apex2, CL);

        piscina_vacare(p);
        /* apex debet permanere */
        apex2 = piscina_summa_apex_usus(p);
        CREDO_AEQUALIS_I32 ((i32)apex2, CL);

        piscina_destruere(p);
    }

    /* ========================================================
     * PROBARE: NIHIL input handling
     * ======================================================== */

    {
        memoriae_index result;

        imprimere("\n--- Probans tutelae initus NIHIL ---\n");

        result = piscina_summa_usus(NIHIL);
        CREDO_AEQUALIS_I32 ((i32)result, ZEPHYRUM);

        result = piscina_summa_inutilis_allocatus(NIHIL);
        CREDO_AEQUALIS_I32 ((i32)result, ZEPHYRUM);

        result = piscina_summa_apex_usus(NIHIL);
        CREDO_AEQUALIS_I32 ((i32)result, ZEPHYRUM);
    }

    /* ========================================================
     * Compendium
     * ======================================================== */

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}




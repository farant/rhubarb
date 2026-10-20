/* probatio_piscina.c - Probationes Piscinae */
#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>


    /* ==================================================
     * PROBARE: piscina_generare_dynamicum
     * ================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    (vacuum)c;
    {
               Piscina* p;
        memoriae_index  usus;

        p = piscina_generare_dynamicum("test_dynamica",  CCLVI);
        CREDO_NON_NIHIL (p);

        usus = piscina_summa_usus(p);
        CREDO_AEQUALIS_I32 ((i32)usus, ZEPHYRUM);

        piscina_destruere(p);
    }
}


    /* ==================================================
     * PROBARE: piscina_generare_certae_magnitudinis
     * ================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    (vacuum)c;
    {

               Piscina* p;
        memoriae_index  usus;

        p = piscina_generare_certae_magnitudinis("probare_fixa", DXII);

        usus = piscina_summa_usus(p);
        CREDO_AEQUALIS_I32 ((i32)usus, ZEPHYRUM);

        piscina_destruere(p);
    }
}


    /* ==================================================
     * PROBARE: piscina_allocare - allocatio singularis
     * ================================================== */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    (vacuum)c;
    {

               Piscina* p;
                vacuum* allocatus;
        memoriae_index  usus;

        p = piscina_generare_dynamicum("probatio_alloc1", CCLVI);
        CREDO_NON_NIHIL (p);

        allocatus = piscina_allocare(p, CXXVIII);
        CREDO_NON_NIHIL (allocatus);

        usus = piscina_summa_usus(p);
        CREDO_AEQUALIS_I32 ((i32)usus, CXXVIII);

        piscina_destruere(p);
    }
}


    /* ==================================================
     * PROBARE: piscina_allocare - allocationes multiplices
     * ================================================== */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
    (vacuum)c;
    {
               Piscina* p;
                vacuum* a1;
                vacuum* a2;
                vacuum* a3;
        memoriae_index  usus;

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
}


    /* ==================================================
     * PROBARE: piscina_allocare - crescentia dynamica
     * ================================================== */

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
    (vacuum)c;
    {
               Piscina* p;
                vacuum* allocatus;
        memoriae_index  usus;

        p = piscina_generare_dynamicum("probatio_crescentia", CXXVIII);
        CREDO_NON_NIHIL (p);

        /* Allocare maiorem quam alveum initialem */
        allocatus = piscina_allocare(p, CCLVI);
        CREDO_NON_NIHIL (allocatus);

        usus = piscina_summa_usus(p);
        CREDO_AEQUALIS_I32 ((i32)usus, CCLVI);

        piscina_destruere(p);
    }
}


    /* ==================================================
     * PROBARE: piscina_conari_allocare - casus successus
     * ================================================== */

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
    (vacuum)c;
    {
        Piscina* p;
         vacuum* allocatus;

        p = piscina_generare_dynamicum("probare_conari_allocare",
            CCLVI);

        allocatus = piscina_conari_allocare(p, CXXVIII);
        CREDO_NON_NIHIL (allocatus);

        piscina_destruere(p);
    }
}


    /* ==================================================
     * PROBARE: piscina_conari_allocare - casus fracti (certae)
     * ================================================== */

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
    (vacuum)c;
    {
        Piscina* p;
         vacuum* allocatus;

        p =
            piscina_generare_certae_magnitudinis("probare_conari_allocare",
            CXXVIII);
        CREDO_NON_NIHIL (p);

        /* Allocare plus quam capacitate */
        allocatus = piscina_conari_allocare(p, CCLVI);
        CREDO_NIHIL (allocatus);

        piscina_destruere(p);
    }
}


    /* ==================================================
     * PROBARE: piscina_allocare_ordinatum - ordinatum
     * ================================================== */

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
    (vacuum)c;
    {
               Piscina* p;
                vacuum* ptr_non_ordinatum;
                vacuum* ptr_ordinatum;
        memoriae_index  offset_ordinatum;

        p = piscina_generare_dynamicum("test_alignment", DXII);

        CREDO_NON_NIHIL (p);

        /* Allocare non-ordinatum primum */
        ptr_non_ordinatum  = piscina_allocare(p, VII);
        offset_ordinatum   = (memoriae_index)(void*)ptr_non_ordinatum;

        /* Allocare XVI-byte ordinatum */
        ptr_ordinatum     = piscina_allocare_ordinatum(p, LXIV, XVI);
        offset_ordinatum  = (memoriae_index)(void*)ptr_ordinatum;

        /* Confer si ordinatum aptatum est */
        CREDO_NON_NIHIL    (ptr_ordinatum);
        CREDO_AEQUALIS_S32 (offset_ordinatum % XVI, ZEPHYRUM);

        piscina_destruere(p);
    }
}


    /* ==================================================
     * PROBARE: piscina_vacare 
     * ================================================== */

interior vacuum
_sectio_nona (
    CredoContextus* c)
{
    (vacuum)c;
    {
               Piscina* p;
                vacuum* a1;
                vacuum* a2;
        memoriae_index  ante;
        memoriae_index  post;

        p = piscina_generare_dynamicum("probatio_reliqua", CCLVI);
        CREDO_NON_NIHIL (p);

        a1    = piscina_allocare(p, C);
        ante  = piscina_summa_usus(p);
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
}


    /* ==================================================
     * PROBARE: piscina_summa_inutilis_allocatus
     * ================================================== */

interior vacuum
_sectio_decima (
    CredoContextus* c)
{
    (vacuum)c;
    {
               Piscina* p;
        memoriae_index  usus;
        memoriae_index  inutilis;
        memoriae_index  totalis;

        p = piscina_generare_dynamicum("probatio_inutilis", CCLVI);
        CREDO_NON_NIHIL (p);

        piscina_allocare(p, C);

        usus      = piscina_summa_usus(p);
        inutilis  = piscina_summa_inutilis_allocatus(p);

        /* usus + inutilis = CCLVI */
        totalis = usus + inutilis;
        CREDO_AEQUALIS_I32 ((i32)totalis, CCLVI);

        piscina_destruere(p);
    }
}


    /* ==================================================
     * PROBARE: piscina_reliqua_antequam_cresca_alvei
     * ================================================== */

interior vacuum
_sectio_decima_prima (
    CredoContextus* c)
{
    (vacuum)c;
    {
               Piscina* p;
        memoriae_index  reliqua_initia;
        memoriae_index  reliqua_post;

        p = piscina_generare_dynamicum("probatio_reliqua", CCLVI);
        CREDO_NON_NIHIL (p);

        reliqua_initia = piscina_reliqua_antequam_cresca_alvei(p);
        CREDO_AEQUALIS_I32 ((i32)reliqua_initia, CCLVI);

        piscina_allocare(p, C);
        reliqua_post = piscina_reliqua_antequam_cresca_alvei(p);
        CREDO_AEQUALIS_I32 ((i32)reliqua_post, CLVI);

        piscina_destruere(p);
    }
}


    /* ==================================================
     * PROBARE: piscina_summa_apex_usus
     * ================================================== */

interior vacuum
_sectio_decima_secunda (
    CredoContextus* c)
{
    (vacuum)c;
    {
               Piscina* p;
        memoriae_index  apex1;
        memoriae_index  apex2;

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
}


    /* ==================================================
     * PROBARE: NIHIL input handling
     * ================================================== */

interior vacuum
_sectio_decima_tertia (
    CredoContextus* c)
{
    (vacuum)c;
    {
        memoriae_index result;

        result = piscina_summa_usus(NIHIL);
        CREDO_AEQUALIS_I32 ((i32)result, ZEPHYRUM);

        result = piscina_summa_inutilis_allocatus(NIHIL);
        CREDO_AEQUALIS_I32 ((i32)result, ZEPHYRUM);

        result = piscina_summa_apex_usus(NIHIL);
        CREDO_AEQUALIS_I32 ((i32)result, ZEPHYRUM);
    }
}


    /* ==================================================
     * PROBARE: piscina_notare et piscina_reficere
     * ================================================== */

interior vacuum
_sectio_decima_quarta (
    CredoContextus* c)
{
    (vacuum)c;
    {
               Piscina* p;
        PiscinaNotatio  nota;
                vacuum* a1;
                vacuum* a2;
                vacuum* a3;
        memoriae_index  usus_ante;
        memoriae_index  usus_post;

        p = piscina_generare_dynamicum("probatio_notare", CCLVI);
        CREDO_NON_NIHIL (p);

        /* Allocare aliquid primum */
        a1 = piscina_allocare(p, L);
        CREDO_NON_NIHIL (a1);

        /* Notare positionem */
        nota       = piscina_notare(p);
        usus_ante  = piscina_summa_usus(p);
        CREDO_AEQUALIS_I32 ((i32)usus_ante, L);

        /* Allocare plus post notationem */
        a2 = piscina_allocare(p, C);
        CREDO_NON_NIHIL (a2);
        /* L, deinde ordinatio ad VIII (LVI), deinde C: CLVI */
        CREDO_AEQUALIS_I32 ((i32)piscina_summa_usus(p), CLVI);

        /* Reficere ad notationem */
        piscina_reficere(p, nota);
        usus_post = piscina_summa_usus(p);
        CREDO_AEQUALIS_I32 ((i32)usus_post, L);

        /* Allocare denuo - debet reusare memoriam */
        a3 = piscina_allocare(p, C);
        CREDO_NON_NIHIL (a3);
        /* a3 debet esse ubi a2 erat */
        CREDO_AEQUALIS_PTR (a3, a2);

        piscina_destruere(p);
    }
}


    /* ==================================================
     * PROBARE: piscina_notare nested (stack pattern)
     * ================================================== */

interior vacuum
_sectio_decima_quinta (
    CredoContextus* c)
{
    (vacuum)c;
    {
               Piscina* p;
        PiscinaNotatio  nota_exterior;
        PiscinaNotatio  nota_interior;
                vacuum* a1;
                vacuum* a2;
                vacuum* a3;

        p = piscina_generare_dynamicum("probatio_nested", DXII);
        CREDO_NON_NIHIL (p);

        /* Allocare primum */
        a1 = piscina_allocare(p, L);
        CREDO_NON_NIHIL (a1);

        /* Nota exterior */
        nota_exterior = piscina_notare(p);

        /* Allocare secundum */
        a2 = piscina_allocare(p, L);
        CREDO_NON_NIHIL (a2);

        /* Nota interior */
        nota_interior = piscina_notare(p);

        /* Allocare tertium */
        a3 = piscina_allocare(p, L);
        CREDO_NON_NIHIL (a3);
        /* L | LVI+L = CVI | CXII+L = CLXII (ordinatio ad VIII) */
        CREDO_AEQUALIS_I32 ((i32)piscina_summa_usus(p), CLXII);

        /* Reficere ad interiorem - liberare a3 */
        piscina_reficere(p, nota_interior);
        CREDO_AEQUALIS_I32 ((i32)piscina_summa_usus(p), CVI);

        /* Reficere ad exteriorem - liberare a2 et a3 */
        piscina_reficere(p, nota_exterior);
        CREDO_AEQUALIS_I32 ((i32)piscina_summa_usus(p), L);

        piscina_destruere(p);
    }
}


    /* ==================================================
     * PROBARE: ordinatio ordinaria (2026-10-05)
     *
     * piscina_allocare ad PISCINA_ORDINATIO_ORDINARIA ordinat, etiam
     * post allocationem octeti unius; _ordinatum(..., I) arte stipat.
     * ================================================== */

interior vacuum
_sectio_decima_sexta (
    CredoContextus* c)
{
    (vacuum)c;
    {
        Piscina* p;
             i8* unus;
             i8* alter;
         vacuum* ordinatum;

        p = piscina_generare_dynamicum("probatio_ordinatio", CCLVI);
        CREDO_NON_NIHIL (p);
        unus       = (i8*)piscina_allocare(p, I);
        ordinatum  = piscina_allocare(p, VIII);
        CREDO_AEQUALIS_I32 ((i32)((memoriae_index)ordinatum
            % PISCINA_ORDINATIO_ORDINARIA), ZEPHYRUM);
        CREDO_AEQUALIS_I32 ((i32)((i8*)ordinatum - unus), VIII);

        /* stipatio arta explicita: octeti contigui */
        unus   = (i8*)piscina_allocare_ordinatum(p, I, I);
        alter  = (i8*)piscina_allocare_ordinatum(p, I, I);
        CREDO_AEQUALIS_I32 ((i32)(alter - unus), I);

        piscina_destruere(p);
    }
}


    /* ==================================================
     * PROBARE: piscina_potesne_allocare
     * ================================================== */

interior vacuum
_sectio_decima_septima (
    CredoContextus* c)
{
    (vacuum)c;
    {
        Piscina* p_dyn;
        Piscina* p_fix;
            b32  potest;

        /* Dynamica - semper potest */
        p_dyn = piscina_generare_dynamicum("probatio_potesne_dyn",
            CCLVI);
        CREDO_NON_NIHIL (p_dyn);

        potest = piscina_potesne_allocare(p_dyn, M);
        CREDO_VERUM (potest);

        piscina_destruere(p_dyn);

        /* Certae magnitudinis - limitata */
        p_fix =
            piscina_generare_certae_magnitudinis("probatio_potesne_fix",
            CCLVI);
        CREDO_NON_NIHIL (p_fix);

        /* Intra capacitatem */
        potest = piscina_potesne_allocare(p_fix, C);
        CREDO_VERUM (potest);

        /* Extra capacitatem */
        potest = piscina_potesne_allocare(p_fix, D);
        CREDO_FALSUM (potest);

        /* Post allocatio - verificare spatium reliquum */
        piscina_allocare(p_fix, CC);
        potest = piscina_potesne_allocare(p_fix, C);
        CREDO_FALSUM (potest);

        potest = piscina_potesne_allocare(p_fix, L);
        CREDO_VERUM (potest);

        piscina_destruere(p_fix);
    }
}


    /* ==================================================
     * PROBARE: piscina_numerus_alveorum / _allocationum (2026-09-02:
     * instrumentum computus - imago memoriae parseris)
     * ================================================== */

interior vacuum
_sectio_decima_octava (
    CredoContextus* c)
{
    (vacuum)c;
    {
         Piscina* p_num;
             i32  i;

        p_num = piscina_generare_dynamicum("probatio_numeri", CCLVI);
        CREDO_NON_NIHIL (p_num);
        CREDO_AEQUALIS_I32 ((i32)piscina_numerus_alveorum(p_num), I);
        CREDO_AEQUALIS_I32 ((i32)piscina_numerus_allocationum(p_num),
            ZEPHYRUM);

        /* XL allocationes C octetorum in alveo CCLVI: alvei crescunt,
         * numerus allocationum exacte XL */
        per (i = ZEPHYRUM; i < XL; i++)
        {
            CREDO_NON_NIHIL (piscina_allocare(p_num, C));
        }
        CREDO_AEQUALIS_I32 ((i32)piscina_numerus_allocationum(p_num),
            XL);
        CREDO_VERUM ((i32)piscina_numerus_alveorum(p_num) >= II);
        /* commissum = usus + inutilis, per alveos omnes */
        CREDO_VERUM (piscina_summa_usus(p_num)
            >= (memoriae_index)(XL * C));

        /* reficere usum minuit, historiam allocationum non */
        piscina_vacare(p_num);
        CREDO_AEQUALIS_I32 ((i32)piscina_numerus_allocationum(p_num),
            XL);

        /* apex incrementalis (2026-09-02) == summa offsetuum ambulata:
         * crescens, post notare/reficere, post allocationes novas */
        {
            PiscinaNotatio nota;
            memoriae_index apex_ante;

            CREDO_VERUM (piscina_summa_apex_usus(p_num)
                >= piscina_summa_usus(p_num));
            piscina_vacare(p_num);
            CREDO_AEQUALIS_I32 ((i32)piscina_summa_usus(p_num),
                ZEPHYRUM);
            per (i = ZEPHYRUM; i < XX; i++)
            {
                (vacuum)piscina_allocare(p_num, C);
            }
            nota = piscina_notare(p_num);
            per (i = ZEPHYRUM; i < XX; i++)
            {
                (vacuum)piscina_allocare(p_num, C);
            }
            apex_ante = piscina_summa_apex_usus(p_num);
            CREDO_VERUM (apex_ante >= piscina_summa_usus(p_num));
            piscina_reficere(p_num, nota);
            CREDO_VERUM (piscina_summa_usus(p_num) < apex_ante);
            CREDO_VERUM (piscina_summa_apex_usus(p_num) == apex_ante);
            per (i = ZEPHYRUM; i < V; i++)
            {
                (vacuum)piscina_allocare(p_num, C);
            }
            CREDO_VERUM (piscina_summa_apex_usus(p_num) == apex_ante);
            per (i = ZEPHYRUM; i < XXX; i++)
            {
                (vacuum)piscina_allocare(p_num, C);
            }
            CREDO_VERUM (piscina_summa_apex_usus(p_num) > apex_ante);
            CREDO_VERUM (piscina_summa_apex_usus(p_num)
                == piscina_summa_usus(p_num));
        }

        /* allocatio fracta (piscina fixa) NON numeratur */
        piscina_destruere(p_num);
        p_num =
            piscina_generare_certae_magnitudinis("probatio_numeri_fixa",
            CXXVIII);
        CREDO_NON_NIHIL (p_num);
        CREDO_NON_NIHIL (piscina_conari_allocare(p_num, LXIV));
        CREDO_NIHIL     (piscina_conari_allocare(p_num, CCLVI));
        CREDO_AEQUALIS_I32 ((i32)piscina_numerus_allocationum(p_num),
            I);
        CREDO_AEQUALIS_I32 ((i32)piscina_numerus_alveorum(p_num), I);
        piscina_destruere(p_num);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "piscina_generare_dynamicum", _sectio_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "piscina_generare_certae_magnitudinis", _sectio_secunda, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "piscina_allocare (singularis)", _sectio_tertia, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "piscina_allocare (multiplex)", _sectio_quarta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "piscina_allocare (cresco)", _sectio_quinta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "piscina_conari_allocare (successus)", _sectio_sexta, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "piscina_conari_allocare (fracta)", _sectio_septima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "piscina_allocare_ordinatum", _sectio_octava, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "piscina_vacare", _sectio_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "piscina_summa_inutilis_allocatus", _sectio_decima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "piscina_reliqua_antequam_cresca_alvei", _sectio_decima_prima,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "piscina_summa_apex_usus", _sectio_decima_secunda, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "tutelae initus NIHIL", _sectio_decima_tertia, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "piscina_notare_reficere", _sectio_decima_quarta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "piscina_notare_nested", _sectio_decima_quinta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "ordinationem ordinariam", _sectio_decima_sexta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "piscina_potesne_allocare", _sectio_decima_septima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "numerus_alveorum / numerus_allocationum", _sectio_decima_octava,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("piscina", SECTIONES);
}

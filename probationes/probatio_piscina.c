/* probatio_piscina.c - Probationes Piscinae */
#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>


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

/* piscina_conari_ampliare (2026-10-21): via I in loco, via II alveus
 * solus motus, recusationes cum allocatione intacta */
interior vacuum
_sectio_decima_nona (
    CredoContextus* c)
{
    (vacuum)c;
    {
               Piscina* p;
                    i8* a;
                    i8* b;
                    i8* x;
        memoriae_index  usus;
        memoriae_index  numerus_ante;
                   i32  i;
                   b32  recte;

        /* I. allocatio ultima alvei: in loco, idem monstrator, usus
         * per incrementum, numerus allocationum immutatus */
        p = piscina_generare_dynamicum("ampliatio", CCLVI);
        CREDO_NON_NIHIL (p);
        a = (i8*)piscina_allocare(p, XVI);
        memset(a, 'q', XVI);
        usus          = piscina_summa_usus(p);
        numerus_ante  = piscina_numerus_allocationum(p);
        CREDO_VERUM (piscina_conari_ampliare(p, a, XVI, XXXII) == a);
        CREDO_AEQUALIS_I32 ((i32)(piscina_summa_usus(p) - usus), XVI);
        CREDO_AEQUALIS_I32 ((i32)piscina_numerus_allocationum(p),
            (i32)numerus_ante);
        /* allocatio sequens POST partem auctam cadit */
        b = (i8*)piscina_allocare(p, VIII);
        CREDO_VERUM (b >= a + XXXII);

        /* II. non ultima: NIHIL, contenta intacta */
        CREDO_NIHIL (piscina_conari_ampliare(p, a, XXXII, LXIV));
        recte = VERUM;
        per (i = ZEPHYRUM; i < XVI; i++)
        {
            si (a[i] != 'q')
            {
                recte = FALSUM;
            }
        }
        CREDO_VERUM (recte);
        /* mensura vetus non exacta: finis non congruit, NIHIL */
        CREDO_NIHIL (piscina_conari_ampliare(p, b, IV, XVI));
        CREDO_NIHIL (piscina_conari_ampliare(p, b, VIII, IV));
        CREDO_NIHIL (piscina_conari_ampliare(p, NIHIL, ZEPHYRUM, VIII));
        CREDO_NIHIL (piscina_conari_ampliare(NIHIL, b, VIII, XVI));
        piscina_destruere(p);

        /* III. allocatio SOLA alvei, piscina dynamica: alveus ipse
         * crescit (fortasse motus), contenta servata, alveus nullus
         * novus */
        p = piscina_generare_dynamicum("ampliatio_sola", LXIV);
        CREDO_NON_NIHIL (p);
        x = (i8*)piscina_allocare(p, LXIV);
        per (i = ZEPHYRUM; i < LXIV; i++)
        {
            x[i] = (i8)i;
        }
        x = (i8*)piscina_conari_ampliare(p, x, LXIV, IV * MXXIV);
        CREDO_NON_NIHIL (x);
        recte = (x != NIHIL);
        per (i = ZEPHYRUM; recte && i < LXIV; i++)
        {
            si (x[i] != (i8)i)
            {
                recte = FALSUM;
            }
        }
        CREDO_VERUM (recte);
        CREDO_AEQUALIS_I32 ((i32)piscina_numerus_alveorum(p), I);
        CREDO_AEQUALIS_I32 ((i32)piscina_summa_usus(p), IV * MXXIV);
        CREDO_VERUM (piscina_summa_apex_usus(p)
            >= (memoriae_index)(IV * MXXIV));
        /* alveus plenus: allocatio sequens alveum novum petit */
        CREDO_NON_NIHIL (piscina_allocare(p, VIII));
        CREDO_AEQUALIS_I32 ((i32)piscina_numerus_alveorum(p), II);
        piscina_destruere(p);

        /* IV. piscina certae magnitudinis: in loco solum, numquam
         * motus */
        p = piscina_generare_certae_magnitudinis("ampliatio_fixa",
            CXXVIII);
        CREDO_NON_NIHIL (p);
        x = (i8*)piscina_allocare(p, LXIV);
        CREDO_VERUM (piscina_conari_ampliare(p, x, LXIV, CXXVIII) == x);
        CREDO_NIHIL (piscina_conari_ampliare(p, x, CXXVIII, CCLVI));
        piscina_destruere(p);
    }
}


    /* ==================================================
     * PROBARE: exundatio mensurae (parcum …1Q6158) - petitio
     * prope maximum memoriae_index NIHIL reddit, piscina intacta;
     * olim offset + mensura circumibat: offset retro, allocatio
     * proxima vivam superponebat (aut alvei minimi sine fine)
     * ================================================== */

interior vacuum
_exundationes_corpus (
    CredoContextus* c)
{
               Piscina* p;
                    i8* a;
                    i8* b;
        memoriae_index  maximus;
        memoriae_index  usus;
        memoriae_index  alvei;
                   i32  k;

    (vacuum)c;
    maximus = (memoriae_index)~(memoriae_index)ZEPHYRUM;

    /* I. offset + mensura circumiret: NIHIL, usus et alvei idem,
     *    allocatio viva intacta post allocationem proximam */
    p = piscina_generare_dynamicum("exundatio", CCLVI);
    CREDO_NECESSE_NON_NIHIL (p);
    a = (i8*)piscina_allocare(p, LXIV);
    memset(a, 0x5A, LXIV);
    usus   = piscina_summa_usus(p);
    alvei  = (memoriae_index)piscina_numerus_alveorum(p);
    CREDO_NIHIL (piscina_conari_allocare(p, maximus - XV));
    CREDO_NIHIL (piscina_conari_allocare(p, maximus));
    CREDO_VERUM (piscina_summa_usus(p) == usus);
    CREDO_VERUM ((memoriae_index)piscina_numerus_alveorum(p) == alvei);
    b = (i8*)piscina_allocare(p, LXIV);
    CREDO_NECESSE_NON_NIHIL (b);
    memset(b, 0x33, LXIV);
    per (k = ZEPHYRUM; k < LXIV && a[k] == 0x5A; k++)
    {
    }
    CREDO_AEQUALIS_I32 (k, LXIV);

    /* III. ordinatio immanis (offset + ordinatio - I circumiret) */
    CREDO_NIHIL (piscina_conari_allocare_ordinatum(p, VIII, maximus));
    CREDO_NIHIL (piscina_conari_allocare_ordinatum(p, VIII,
        maximus / II + I));
    CREDO_VERUM (piscina_summa_usus(p) == usus + LXIV);
    piscina_destruere(p);

    /* II. ab offset nullo: necessaria (= mensura) capit, sed
     *     capacitas nova (necessaria + basis) circumiret - olim
     *     alveus parvus et gyrus sine fine. Et ordinatio nulla:
     *     solum ad offset nullum non a spatio reliquo capitur
     *     (ordinatio - I = maximus), ergo divisio per nihil */
    p = piscina_generare_dynamicum("exundatio_nova", CCLVI);
    CREDO_NECESSE_NON_NIHIL (p);
    CREDO_NIHIL (piscina_conari_allocare(p, maximus - C));
    CREDO_NIHIL (piscina_conari_allocare_ordinatum(p, VIII, ZEPHYRUM));
    CREDO_VERUM (piscina_summa_usus(p) == ZEPHYRUM);
    CREDO_AEQUALIS_I32 ((i32)piscina_numerus_alveorum(p), I);
    piscina_destruere(p);

    /* IV. certae magnitudinis: eadem refutatio, nihil motum */
    p = piscina_generare_certae_magnitudinis("exundatio_fixa", CCLVI);
    CREDO_NECESSE_NON_NIHIL (p);
    a = (i8*)piscina_allocare(p, LXIV);
    memset(a, 0x5A, LXIV);
    CREDO_NIHIL (piscina_conari_allocare(p, maximus - XV));
    b = (i8*)piscina_conari_allocare(p, LXIV);
    CREDO_NECESSE_NON_NIHIL (b);
    memset(b, 0x33, LXIV);
    per (k = ZEPHYRUM; k < LXIV && a[k] == 0x5A; k++)
    {
    }
    CREDO_AEQUALIS_I32 (k, LXIV);
    piscina_destruere(p);

    /* V. ampliare: offset + incrementum circumiret - NIHIL,
     *    allocatio et usus intacta */
    p = piscina_generare_certae_magnitudinis("exundatio_ampliare",
        CCLVI);
    CREDO_NECESSE_NON_NIHIL (p);
    /* allocatio prior: 'a' ad offset LXIV, ergo LXIV + (maximus -
     * LXIV) + LXIV circumit (ad offset nullum summa = maximus) */
    (vacuum)piscina_allocare(p, LXIV);
    a     = (i8*)piscina_allocare(p, LXIV);
    usus  = piscina_summa_usus(p);
    CREDO_NIHIL (piscina_conari_ampliare(p, a, LXIV, maximus));
    CREDO_VERUM (piscina_summa_usus(p) == usus);
    piscina_destruere(p);
}

/* corpus in filio primum (gyrus sine fine aut SIGFPE = fractura
 * nominata, non suita mortua), in parente solum si filius transiit */
interior vacuum
_sectio_vicesima (
    CredoContextus* c)
{
    memoriae_index fracti_ante = credo_numerus_fracti();

    CREDO_NON_PENDET (_exundationes_corpus(c), V * M);
    si (credo_numerus_fracti() == fracti_ante)
    {
        _exundationes_corpus(c);
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
    { "piscina_conari_ampliare", _sectio_decima_nona, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "exundatio mensurae", _sectio_vicesima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("piscina", SECTIONES);
}

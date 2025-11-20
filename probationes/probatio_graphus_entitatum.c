/* probatio_graphus_entitatum.c - Probationes GraphusEntitatum */
#include "latina.h"
#include "graphus_entitatum.h"
#include "uuid.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>

s32 principale(vacuum)
{
                Piscina* piscina;
       GraphusEntitatum* graphus;
    InternamentumChorda* intern;
                    b32  praeteritus;

    /* Aperire piscinam et credonem */
    piscina = piscina_generare_dynamicum("probatio_graphus", M);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    /* Creare graphum */
    graphus = graphus_entitatum_creare(piscina);
    CREDO_NON_NIHIL(graphus);

    intern = graphus_entitatum_internamentum(graphus);
    CREDO_NON_NIHIL(intern);

    /* ==================================================
     * Probare creatio graphus
     * ================================================== */

    {
        imprimere("\n--- Probans graphus_entitatum_creare ---\n");

        CREDO_AEQUALIS_I32(graphus_entitatum_numerus(graphus), ZEPHYRUM);
    }

    /* ==================================================
     * Probare addere entitates
     * ================================================== */

    {
        Entitas* ent1;
        Entitas* ent2;
         chorda* id1;
         chorda* id2;
         chorda* genus;

        imprimere("\n--- Probans graphus_entitatum_addere_entitatem ---\n");

        /* Creare duas entitates */
        id1   = chorda_internare_ex_literis(intern, "ent-1");
        id2   = chorda_internare_ex_literis(intern, "ent-2");
        genus = chorda_internare_ex_literis(intern, "document");

        ent1 = entitas_creare(piscina, id1, genus);
        ent2 = entitas_creare(piscina, id2, genus);

        /* Addere ad graphum */
        CREDO_VERUM(graphus_entitatum_addere_entitatem(graphus, ent1));
        CREDO_VERUM(graphus_entitatum_addere_entitatem(graphus, ent2));

        CREDO_AEQUALIS_I32(graphus_entitatum_numerus(graphus), II);

        /* Tentare addere duplicatum */
        CREDO_FALSUM(graphus_entitatum_addere_entitatem(graphus, ent1));
    }

    /* ==================================================
     * Probare providor capere_entitatem
     * ================================================== */

    {
        EntitasProvidor* providor;
                Entitas* ent_found;
                 chorda* id;

        imprimere("\n--- Probans providor capere_entitatem ---\n");

        providor = graphus_entitatum_providor_creare(graphus);
        CREDO_NON_NIHIL(providor);

        /* Capere entitatem existentem */
        id = chorda_internare_ex_literis(intern, "ent-1");
        ent_found = providor->capere_entitatem(providor->datum, id);
        CREDO_NON_NIHIL(ent_found);
        CREDO_VERUM(ent_found->id == id);

        /* Capere entitatem non existentem */
        id = chorda_internare_ex_literis(intern, "non-existent");
        ent_found = providor->capere_entitatem(providor->datum, id);
        CREDO_NIHIL(ent_found);
    }

    /* ==================================================
     * Probare providor capere_entitates_relatae
     * ================================================== */

    {
        EntitasProvidor* providor;
        Entitas*         parens;
        Entitas*         filius1;
        Entitas*         filius2;
        chorda*          id_parens;
        chorda*          id_filius1;
        chorda*          id_filius2;
        chorda*          genus_relatio;
        Xar*             filii;

        imprimere("\n--- Probans providor capere_entitates_relatae ---\n");

        /* Creare parens cum duobus filiis */
        id_parens  = chorda_internare_ex_literis(intern, "folder-1");
        id_filius1 = chorda_internare_ex_literis(intern, "file-1");
        id_filius2 = chorda_internare_ex_literis(intern, "file-2");

        parens  = entitas_creare(piscina, id_parens,
                                 chorda_internare_ex_literis(intern, "folder"));
        filius1 = entitas_creare(piscina, id_filius1,
                                 chorda_internare_ex_literis(intern, "file"));
        filius2 = entitas_creare(piscina, id_filius2,
                                 chorda_internare_ex_literis(intern, "file"));

        /* Addere ad graphum */
        graphus_entitatum_addere_entitatem(graphus, parens);
        graphus_entitatum_addere_entitatem(graphus, filius1);
        graphus_entitatum_addere_entitatem(graphus, filius2);

        /* Addere relationes */
        genus_relatio = chorda_internare_ex_literis(intern, "contains");
        entitas_relatio_addere(parens, genus_relatio, id_filius1);
        entitas_relatio_addere(parens, genus_relatio, id_filius2);

        /* Capere entitates relatas */
        providor = graphus_entitatum_providor_creare(graphus);
        filii = providor->capere_entitates_relatae(providor->datum, parens, genus_relatio);

        CREDO_NON_NIHIL(filii);
        CREDO_AEQUALIS_I32(xar_numerus(filii), II);
    }

    /* ==================================================
     * Probare providor quaerere_cum_nota
     * ================================================== */

    {
        EntitasProvidor* providor;
        Entitas*         ent1;
        Entitas*         ent2;
        Entitas*         ent3;
        chorda*          nota_urgent;
        Xar*             resultus;

        imprimere("\n--- Probans providor quaerere_cum_nota ---\n");

        /* Creare entitates cum notis */
        ent1 = entitas_creare(piscina,
                              chorda_internare_ex_literis(intern, "task-1"),
                              chorda_internare_ex_literis(intern, "task"));
        ent2 = entitas_creare(piscina,
                              chorda_internare_ex_literis(intern, "task-2"),
                              chorda_internare_ex_literis(intern, "task"));
        ent3 = entitas_creare(piscina,
                              chorda_internare_ex_literis(intern, "task-3"),
                              chorda_internare_ex_literis(intern, "task"));

        nota_urgent = chorda_internare_ex_literis(intern, "#urgent");

        entitas_nota_addere(ent1, nota_urgent);
        entitas_nota_addere(ent3, nota_urgent);

        graphus_entitatum_addere_entitatem(graphus, ent1);
        graphus_entitatum_addere_entitatem(graphus, ent2);
        graphus_entitatum_addere_entitatem(graphus, ent3);

        /* Quaerere cum nota */
        providor = graphus_entitatum_providor_creare(graphus);
        resultus = providor->quaerere_cum_nota(providor->datum, nota_urgent);

        CREDO_NON_NIHIL(resultus);
        CREDO_AEQUALIS_I32(xar_numerus(resultus), II);
    }

    /* ==================================================
     * Probare providor quaerere_cum_praefixo_notae
     * ================================================== */

    {
        EntitasProvidor* providor;
        Entitas*         ent1;
        Entitas*         ent2;
        Xar*             resultus;

        imprimere("\n--- Probans providor quaerere_cum_praefixo_notae ---\n");

        /* Creare entitates cum notis namespace */
        ent1 = entitas_creare(piscina,
                              chorda_internare_ex_literis(intern, "proj-1"),
                              chorda_internare_ex_literis(intern, "project"));
        ent2 = entitas_creare(piscina,
                              chorda_internare_ex_literis(intern, "proj-2"),
                              chorda_internare_ex_literis(intern, "project"));

        entitas_nota_addere(ent1,
                            chorda_internare_ex_literis(intern, "#project::alpha"));
        entitas_nota_addere(ent2,
                            chorda_internare_ex_literis(intern, "#project::beta"));

        graphus_entitatum_addere_entitatem(graphus, ent1);
        graphus_entitatum_addere_entitatem(graphus, ent2);

        /* Quaerere cum praefixo */
        providor = graphus_entitatum_providor_creare(graphus);
        resultus = providor->quaerere_cum_praefixo_notae(providor->datum, "#project::");

        CREDO_NON_NIHIL(resultus);
        CREDO_AEQUALIS_I32(xar_numerus(resultus), II);
    }

    /* ==================================================
     * Probare providor quaerere_textum
     * ================================================== */

    {
        EntitasProvidor* providor;
        Entitas*         ent1;
        Entitas*         ent2;
        Entitas*         ent3;
        Xar*             resultus;

        imprimere("\n--- Probans providor quaerere_textum ---\n");

        /* Creare entitates cum proprietatibus */
        ent1 = entitas_creare(piscina,
                              chorda_internare_ex_literis(intern, "apple-doc"),
                              chorda_internare_ex_literis(intern, "document"));
        ent2 = entitas_creare(piscina,
                              chorda_internare_ex_literis(intern, "banana-doc"),
                              chorda_internare_ex_literis(intern, "document"));
        ent3 = entitas_creare(piscina,
                              chorda_internare_ex_literis(intern, "cherry-doc"),
                              chorda_internare_ex_literis(intern, "document"));

        entitas_proprietas_addere(ent1,
                                   chorda_internare_ex_literis(intern, "content"),
                                   chorda_internare_ex_literis(intern, "apple pie recipe"));
        entitas_proprietas_addere(ent2,
                                   chorda_internare_ex_literis(intern, "content"),
                                   chorda_internare_ex_literis(intern, "banana bread"));
        entitas_proprietas_addere(ent3,
                                   chorda_internare_ex_literis(intern, "content"),
                                   chorda_internare_ex_literis(intern, "cherry tart"));

        graphus_entitatum_addere_entitatem(graphus, ent1);
        graphus_entitatum_addere_entitatem(graphus, ent2);
        graphus_entitatum_addere_entitatem(graphus, ent3);

        /* Quaerere per ID substring */
        providor = graphus_entitatum_providor_creare(graphus);
        resultus = providor->quaerere_textum(providor->datum, "apple");

        CREDO_NON_NIHIL(resultus);
        CREDO_AEQUALIS_I32(xar_numerus(resultus), I);  /* apple-doc */

        /* Quaerere per proprietas valor substring */
        resultus = providor->quaerere_textum(providor->datum, "bread");

        CREDO_NON_NIHIL(resultus);
        CREDO_AEQUALIS_I32(xar_numerus(resultus), I);  /* banana-doc */
    }

    /* ==================================================
     * Probare providor capere_radices
     * ================================================== */

    {
        EntitasProvidor* providor;
        Entitas*         root1;
        Entitas*         root2;
        Entitas*         non_root;
        chorda*          nota_root;
        Xar*             radices;

        imprimere("\n--- Probans providor capere_radices ---\n");

        /* Creare entitates, aliquae cum nota #root */
        root1 = entitas_creare(piscina,
                               chorda_internare_ex_literis(intern, "root-1"),
                               chorda_internare_ex_literis(intern, "folder"));
        root2 = entitas_creare(piscina,
                               chorda_internare_ex_literis(intern, "root-2"),
                               chorda_internare_ex_literis(intern, "folder"));
        non_root = entitas_creare(piscina,
                                  chorda_internare_ex_literis(intern, "child-1"),
                                  chorda_internare_ex_literis(intern, "file"));

        nota_root = chorda_internare_ex_literis(intern, "#root");
        entitas_nota_addere(root1, nota_root);
        entitas_nota_addere(root2, nota_root);

        graphus_entitatum_addere_entitatem(graphus, root1);
        graphus_entitatum_addere_entitatem(graphus, root2);
        graphus_entitatum_addere_entitatem(graphus, non_root);

        /* Capere radices */
        providor = graphus_entitatum_providor_creare(graphus);
        radices = providor->capere_radices(providor->datum);

        CREDO_NON_NIHIL(radices);
        CREDO_AEQUALIS_I32(xar_numerus(radices), II);
    }

    /* ==================================================
     * Compendium
     * ================================================== */

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();

    /* Purificatio */
    credo_claudere();
    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}

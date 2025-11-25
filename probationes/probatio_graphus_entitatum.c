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
     * Probare repositorium capere_entitatem
     * ================================================== */

    {
        EntitasRepositorium* repositorium;
                    Entitas* ent_found;
                     chorda* id;

        imprimere("\n--- Probans repositorium capere_entitatem ---\n");

        repositorium = graphus_entitatum_repositorium_creare(graphus);
        CREDO_NON_NIHIL(repositorium);

        /* Capere entitatem existentem */
        id = chorda_internare_ex_literis(intern, "ent-1");
        ent_found = repositorium->capere_entitatem(repositorium->datum, id);
        CREDO_NON_NIHIL(ent_found);
        CREDO_VERUM(ent_found->id == id);

        /* Capere entitatem non existentem */
        id = chorda_internare_ex_literis(intern, "non-existent");
        ent_found = repositorium->capere_entitatem(repositorium->datum, id);
        CREDO_NIHIL(ent_found);
    }

    /* ==================================================
     * Probare repositorium capere_entitates_relatae
     * ================================================== */

    {
        EntitasRepositorium* repositorium;
        Entitas*             parens;
        Entitas*             filius1;
        Entitas*             filius2;
        chorda*              id_parens;
        chorda*              id_filius1;
        chorda*              id_filius2;
        chorda*              genus_relatio;
        Xar*                 filii;

        imprimere("\n--- Probans repositorium capere_entitates_relatae ---\n");

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
        entitas_relatio_addere(parens, piscina, intern, genus_relatio, id_filius1);
        entitas_relatio_addere(parens, piscina, intern, genus_relatio, id_filius2);

        /* Capere entitates relatas */
        repositorium = graphus_entitatum_repositorium_creare(graphus);
        filii = repositorium->capere_entitates_relatae(repositorium->datum, parens, genus_relatio);

        CREDO_NON_NIHIL(filii);
        CREDO_AEQUALIS_I32(xar_numerus(filii), II);
    }

    /* ==================================================
     * Probare repositorium quaerere_cum_nota
     * ================================================== */

    {
        EntitasRepositorium* repositorium;
        Entitas*             ent1;
        Entitas*             ent2;
        Entitas*             ent3;
        chorda*              nota_urgent;
        Xar*                 resultus;

        imprimere("\n--- Probans repositorium quaerere_cum_nota ---\n");

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
        repositorium = graphus_entitatum_repositorium_creare(graphus);
        resultus = repositorium->quaerere_cum_nota(repositorium->datum, nota_urgent);

        CREDO_NON_NIHIL(resultus);
        CREDO_AEQUALIS_I32(xar_numerus(resultus), II);
    }

    /* ==================================================
     * Probare repositorium quaerere_cum_praefixo_notae
     * ================================================== */

    {
        EntitasRepositorium* repositorium;
        Entitas*             ent1;
        Entitas*             ent2;
        Xar*                 resultus;

        imprimere("\n--- Probans repositorium quaerere_cum_praefixo_notae ---\n");

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
        repositorium = graphus_entitatum_repositorium_creare(graphus);
        resultus = repositorium->quaerere_cum_praefixo_notae(repositorium->datum, "#project::");

        CREDO_NON_NIHIL(resultus);
        CREDO_AEQUALIS_I32(xar_numerus(resultus), II);
    }

    /* ==================================================
     * Probare repositorium quaerere_textum
     * ================================================== */

    {
        EntitasRepositorium* repositorium;
        Entitas*             ent1;
        Entitas*             ent2;
        Entitas*             ent3;
        Xar*                 resultus;

        imprimere("\n--- Probans repositorium quaerere_textum ---\n");

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
        repositorium = graphus_entitatum_repositorium_creare(graphus);
        resultus = repositorium->quaerere_textum(repositorium->datum, "apple");

        CREDO_NON_NIHIL(resultus);
        CREDO_AEQUALIS_I32(xar_numerus(resultus), I);  /* apple-doc */

        /* Quaerere per proprietas valor substring */
        resultus = repositorium->quaerere_textum(repositorium->datum, "bread");

        CREDO_NON_NIHIL(resultus);
        CREDO_AEQUALIS_I32(xar_numerus(resultus), I);  /* banana-doc */
    }

    /* ==================================================
     * Probare repositorium capere_radices
     * ================================================== */

    {
        EntitasRepositorium* repositorium;
        Entitas*             root1;
        Entitas*             root2;
        Entitas*             non_root;
        chorda*              nota_root;
        Xar*                 radices;

        imprimere("\n--- Probans repositorium capere_radices ---\n");

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
        repositorium = graphus_entitatum_repositorium_creare(graphus);
        radices = repositorium->capere_radices(repositorium->datum);

        CREDO_NON_NIHIL(radices);
        CREDO_AEQUALIS_I32(xar_numerus(radices), II);
    }

    /* ==================================================
     * Probare registrare/deregistrare relatio
     * ================================================== */

    {
        Entitas*         ent_a;
        Entitas*         ent_b;
        Relatio*         relatio;
        chorda*          genus_rel;

        imprimere("\n--- Probans registrare/deregistrare relatio ---\n");

        /* Creare duas entitates */
        ent_a = entitas_creare(piscina,
                               chorda_internare_ex_literis(intern, "node-a"),
                               chorda_internare_ex_literis(intern, "node"));
        ent_b = entitas_creare(piscina,
                               chorda_internare_ex_literis(intern, "node-b"),
                               chorda_internare_ex_literis(intern, "node"));

        graphus_entitatum_addere_entitatem(graphus, ent_a);
        graphus_entitatum_addere_entitatem(graphus, ent_b);

        /* Addere relationem */
        genus_rel = chorda_internare_ex_literis(intern, "links_to");
        relatio = entitas_relatio_addere(ent_a, piscina, intern, genus_rel, ent_b->id);
        CREDO_NON_NIHIL(relatio);
        CREDO_NON_NIHIL(relatio->id);

        /* Registrare in indice graphi */
        CREDO_VERUM(graphus_entitatum_registrare_relatio(graphus, relatio));

        /* Non potest registrare duplicatum */
        CREDO_FALSUM(graphus_entitatum_registrare_relatio(graphus, relatio));

        /* Deregistrare */
        CREDO_VERUM(graphus_entitatum_deregistrare_relatio(graphus, relatio->id));

        /* Non potest deregistrare iterum */
        CREDO_FALSUM(graphus_entitatum_deregistrare_relatio(graphus, relatio->id));
    }

    /* ==================================================
     * Probare repositorium capere_relatio
     * ================================================== */

    {
        EntitasRepositorium* repositorium;
        Entitas*             ent_x;
        Entitas*             ent_y;
        Relatio*             relatio;
        Relatio*             relatio_found;
        chorda*              genus_rel;

        imprimere("\n--- Probans repositorium capere_relatio ---\n");

        /* Creare entitates et relationem */
        ent_x = entitas_creare(piscina,
                               chorda_internare_ex_literis(intern, "node-x"),
                               chorda_internare_ex_literis(intern, "node"));
        ent_y = entitas_creare(piscina,
                               chorda_internare_ex_literis(intern, "node-y"),
                               chorda_internare_ex_literis(intern, "node"));

        graphus_entitatum_addere_entitatem(graphus, ent_x);
        graphus_entitatum_addere_entitatem(graphus, ent_y);

        genus_rel = chorda_internare_ex_literis(intern, "connects");
        relatio = entitas_relatio_addere(ent_x, piscina, intern, genus_rel, ent_y->id);
        CREDO_NON_NIHIL(relatio);

        /* Registrare in indice */
        graphus_entitatum_registrare_relatio(graphus, relatio);

        /* Capere per repositorium */
        repositorium = graphus_entitatum_repositorium_creare(graphus);
        relatio_found = repositorium->capere_relatio(repositorium->datum, relatio->id);

        CREDO_NON_NIHIL(relatio_found);
        CREDO_AEQUALIS_PTR(relatio_found, relatio);
        CREDO_AEQUALIS_PTR(relatio_found->origo_id, ent_x->id);
        CREDO_AEQUALIS_PTR(relatio_found->destinatio_id, ent_y->id);

        /* Capere non existentem */
        relatio_found = repositorium->capere_relatio(
            repositorium->datum,
            chorda_internare_ex_literis(intern, "non-existent-rel"));
        CREDO_NIHIL(relatio_found);
    }

    /* ==================================================
     * Probare repositorium capere_relationes_ad
     * ================================================== */

    {
        EntitasRepositorium* repositorium;
        Entitas*             target;
        Entitas*             src1;
        Entitas*             src2;
        Relatio*             rel1;
        Relatio*             rel2;
        chorda*              genus_rel;
        Xar*                 relationes_ad;

        imprimere("\n--- Probans repositorium capere_relationes_ad ---\n");

        /* Creare target et duas fontes */
        target = entitas_creare(piscina,
                                chorda_internare_ex_literis(intern, "target-node"),
                                chorda_internare_ex_literis(intern, "node"));
        src1 = entitas_creare(piscina,
                              chorda_internare_ex_literis(intern, "source-1"),
                              chorda_internare_ex_literis(intern, "node"));
        src2 = entitas_creare(piscina,
                              chorda_internare_ex_literis(intern, "source-2"),
                              chorda_internare_ex_literis(intern, "node"));

        graphus_entitatum_addere_entitatem(graphus, target);
        graphus_entitatum_addere_entitatem(graphus, src1);
        graphus_entitatum_addere_entitatem(graphus, src2);

        /* Addere relationes AD target */
        genus_rel = chorda_internare_ex_literis(intern, "points_to");
        rel1 = entitas_relatio_addere(src1, piscina, intern, genus_rel, target->id);
        rel2 = entitas_relatio_addere(src2, piscina, intern, genus_rel, target->id);

        CREDO_NON_NIHIL(rel1);
        CREDO_NON_NIHIL(rel2);

        /* Registrare in indice */
        graphus_entitatum_registrare_relatio(graphus, rel1);
        graphus_entitatum_registrare_relatio(graphus, rel2);

        /* Capere relationes AD target */
        repositorium = graphus_entitatum_repositorium_creare(graphus);
        relationes_ad = repositorium->capere_relationes_ad(repositorium->datum, target->id);

        CREDO_NON_NIHIL(relationes_ad);
        CREDO_AEQUALIS_I32(xar_numerus(relationes_ad), II);
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

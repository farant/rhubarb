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
    EntitasRepositorium* repo;
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

    /* Creare graphum et repositorium */
    graphus = graphus_entitatum_creare(piscina);
    CREDO_NON_NIHIL(graphus);

    repo = graphus_entitatum_repositorium_creare(graphus);
    CREDO_NON_NIHIL(repo);

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
     * Probare entitas_creare per repositorium
     * ================================================== */

    {
        Entitas* ent1;
        Entitas* ent2;

        imprimere("\n--- Probans repositorium entitas_creare ---\n");

        /* Creare duas entitates per repositorium */
        ent1 = repo->entitas_creare(repo->datum, "document");
        ent2 = repo->entitas_creare(repo->datum, "document");

        CREDO_NON_NIHIL(ent1);
        CREDO_NON_NIHIL(ent2);
        CREDO_NON_NIHIL(ent1->id);
        CREDO_NON_NIHIL(ent2->id);

        /* IDs debent esse diversae */
        CREDO_VERUM(ent1->id != ent2->id);

        CREDO_AEQUALIS_I32(graphus_entitatum_numerus(graphus), II);
    }

    /* ==================================================
     * Probare repositorium capere_entitatem
     * ================================================== */

    {
        Entitas* ent_created;
        Entitas* ent_found;

        imprimere("\n--- Probans repositorium capere_entitatem ---\n");

        /* Creare entitatem */
        ent_created = repo->entitas_creare(repo->datum, "file");
        CREDO_NON_NIHIL(ent_created);

        /* Capere entitatem per ID */
        ent_found = repo->capere_entitatem(repo->datum, ent_created->id);
        CREDO_NON_NIHIL(ent_found);
        CREDO_AEQUALIS_PTR(ent_found, ent_created);

        /* Capere entitatem non existentem */
        ent_found = repo->capere_entitatem(
            repo->datum,
            chorda_internare_ex_literis(intern, "non-existent"));
        CREDO_NIHIL(ent_found);
    }

    /* ==================================================
     * Probare repositorium capere_entitates_relatae
     * ================================================== */

    {
        Entitas*  parens;
        Entitas*  filius1;
        Entitas*  filius2;
        Relatio*  rel1;
        Relatio*  rel2;
        chorda*   genus_relatio;
        Xar*      filii;

        imprimere("\n--- Probans repositorium capere_entitates_relatae ---\n");

        /* Creare parens cum duobus filiis */
        parens  = repo->entitas_creare(repo->datum, "folder");
        filius1 = repo->entitas_creare(repo->datum, "file");
        filius2 = repo->entitas_creare(repo->datum, "file");

        CREDO_NON_NIHIL(parens);
        CREDO_NON_NIHIL(filius1);
        CREDO_NON_NIHIL(filius2);

        /* Addere relationes per repositorium */
        genus_relatio = chorda_internare_ex_literis(intern, "contains");
        rel1 = repo->relatio_addere(repo->datum, parens, "contains", filius1->id);
        rel2 = repo->relatio_addere(repo->datum, parens, "contains", filius2->id);

        CREDO_NON_NIHIL(rel1);
        CREDO_NON_NIHIL(rel2);

        /* Capere entitates relatas */
        filii = repo->capere_entitates_relatae(repo->datum, parens, genus_relatio);

        CREDO_NON_NIHIL(filii);
        CREDO_AEQUALIS_I32(xar_numerus(filii), II);
    }

    /* ==================================================
     * Probare repositorium quaerere_cum_nota
     * ================================================== */

    {
        Entitas* ent1;
        Entitas* ent2;
        Entitas* ent3;
        chorda*  nota_urgent;
        Xar*     resultus;

        imprimere("\n--- Probans repositorium quaerere_cum_nota ---\n");

        /* Creare entitates */
        ent1 = repo->entitas_creare(repo->datum, "task");
        ent2 = repo->entitas_creare(repo->datum, "task");
        ent3 = repo->entitas_creare(repo->datum, "task");

        CREDO_NON_NIHIL(ent1);
        CREDO_NON_NIHIL(ent2);
        CREDO_NON_NIHIL(ent3);

        /* Addere notas per repositorium */
        CREDO_VERUM(repo->nota_addere(repo->datum, ent1, "#urgent"));
        CREDO_VERUM(repo->nota_addere(repo->datum, ent3, "#urgent"));

        /* Quaerere cum nota */
        nota_urgent = chorda_internare_ex_literis(intern, "#urgent");
        resultus = repo->quaerere_cum_nota(repo->datum, nota_urgent);

        CREDO_NON_NIHIL(resultus);
        CREDO_AEQUALIS_I32(xar_numerus(resultus), II);
    }

    /* ==================================================
     * Probare repositorium quaerere_cum_praefixo_notae
     * ================================================== */

    {
        Entitas* ent1;
        Entitas* ent2;
        Xar*     resultus;

        imprimere("\n--- Probans repositorium quaerere_cum_praefixo_notae ---\n");

        /* Creare entitates */
        ent1 = repo->entitas_creare(repo->datum, "project");
        ent2 = repo->entitas_creare(repo->datum, "project");

        CREDO_NON_NIHIL(ent1);
        CREDO_NON_NIHIL(ent2);

        /* Addere notas namespace per repositorium */
        CREDO_VERUM(repo->nota_addere(repo->datum, ent1, "#project::alpha"));
        CREDO_VERUM(repo->nota_addere(repo->datum, ent2, "#project::beta"));

        /* Quaerere cum praefixo */
        resultus = repo->quaerere_cum_praefixo_notae(repo->datum, "#project::");

        CREDO_NON_NIHIL(resultus);
        CREDO_AEQUALIS_I32(xar_numerus(resultus), II);
    }

    /* ==================================================
     * Probare repositorium quaerere_textum
     * ================================================== */

    {
        Entitas* ent1;
        Entitas* ent2;
        Entitas* ent3;
        Xar*     resultus;

        imprimere("\n--- Probans repositorium quaerere_textum ---\n");

        /* Creare entitates */
        ent1 = repo->entitas_creare(repo->datum, "document");
        ent2 = repo->entitas_creare(repo->datum, "document");
        ent3 = repo->entitas_creare(repo->datum, "document");

        CREDO_NON_NIHIL(ent1);
        CREDO_NON_NIHIL(ent2);
        CREDO_NON_NIHIL(ent3);

        /* Addere proprietates per repositorium */
        CREDO_VERUM(repo->proprietas_ponere(repo->datum, ent1, "content", "apple pie recipe"));
        CREDO_VERUM(repo->proprietas_ponere(repo->datum, ent2, "content", "banana bread"));
        CREDO_VERUM(repo->proprietas_ponere(repo->datum, ent3, "content", "cherry tart"));

        /* Quaerere per proprietas valor substring */
        resultus = repo->quaerere_textum(repo->datum, "apple");
        CREDO_NON_NIHIL(resultus);
        CREDO_AEQUALIS_I32(xar_numerus(resultus), I);

        resultus = repo->quaerere_textum(repo->datum, "bread");
        CREDO_NON_NIHIL(resultus);
        CREDO_AEQUALIS_I32(xar_numerus(resultus), I);
    }

    /* ==================================================
     * Probare repositorium capere_radices
     * ================================================== */

    {
        Entitas* root1;
        Entitas* root2;
        Entitas* non_root;
        Xar*     radices;

        imprimere("\n--- Probans repositorium capere_radices ---\n");

        /* Creare entitates */
        root1    = repo->entitas_creare(repo->datum, "folder");
        root2    = repo->entitas_creare(repo->datum, "folder");
        non_root = repo->entitas_creare(repo->datum, "file");

        CREDO_NON_NIHIL(root1);
        CREDO_NON_NIHIL(root2);
        CREDO_NON_NIHIL(non_root);

        /* Addere nota #root per repositorium */
        CREDO_VERUM(repo->nota_addere(repo->datum, root1, "#root"));
        CREDO_VERUM(repo->nota_addere(repo->datum, root2, "#root"));

        /* Capere radices */
        radices = repo->capere_radices(repo->datum);

        CREDO_NON_NIHIL(radices);
        CREDO_AEQUALIS_I32(xar_numerus(radices), II);
    }

    /* ==================================================
     * Probare registrare/deregistrare relatio (low-level)
     * Haec probatio utitur functiones directas quia probat
     * functiones internae graphi, non repositorium
     * ================================================== */

    {
        Entitas*  ent_a;
        Entitas*  ent_b;
        Relatio*  relatio;

        imprimere("\n--- Probans registrare/deregistrare relatio ---\n");

        /* Creare duas entitates per repositorium */
        ent_a = repo->entitas_creare(repo->datum, "node");
        ent_b = repo->entitas_creare(repo->datum, "node");

        CREDO_NON_NIHIL(ent_a);
        CREDO_NON_NIHIL(ent_b);

        /* Addere relationem per repositorium (auto-registrat) */
        relatio = repo->relatio_addere(repo->datum, ent_a, "links_to", ent_b->id);
        CREDO_NON_NIHIL(relatio);
        CREDO_NON_NIHIL(relatio->id);

        /* Non potest registrare duplicatum (iam registrata per relatio_addere) */
        CREDO_FALSUM(graphus_entitatum_registrare_relatio(graphus, relatio));

        /* Deregistrare */
        CREDO_VERUM(graphus_entitatum_deregistrare_relatio(graphus, relatio->id));

        /* Non potest deregistrare iterum */
        CREDO_FALSUM(graphus_entitatum_deregistrare_relatio(graphus, relatio->id));

        /* Re-registrare pro usu in aliis probationibus */
        CREDO_VERUM(graphus_entitatum_registrare_relatio(graphus, relatio));
    }

    /* ==================================================
     * Probare repositorium capere_relatio
     * ================================================== */

    {
        Entitas* ent_x;
        Entitas* ent_y;
        Relatio* relatio;
        Relatio* relatio_found;

        imprimere("\n--- Probans repositorium capere_relatio ---\n");

        /* Creare entitates per repositorium */
        ent_x = repo->entitas_creare(repo->datum, "node");
        ent_y = repo->entitas_creare(repo->datum, "node");

        CREDO_NON_NIHIL(ent_x);
        CREDO_NON_NIHIL(ent_y);

        /* Addere relationem per repositorium */
        relatio = repo->relatio_addere(repo->datum, ent_x, "connects", ent_y->id);
        CREDO_NON_NIHIL(relatio);

        /* Capere per repositorium */
        relatio_found = repo->capere_relatio(repo->datum, relatio->id);

        CREDO_NON_NIHIL(relatio_found);
        CREDO_AEQUALIS_PTR(relatio_found, relatio);
        CREDO_AEQUALIS_PTR(relatio_found->origo_id, ent_x->id);
        CREDO_AEQUALIS_PTR(relatio_found->destinatio_id, ent_y->id);

        /* Capere non existentem */
        relatio_found = repo->capere_relatio(
            repo->datum,
            chorda_internare_ex_literis(intern, "non-existent-rel"));
        CREDO_NIHIL(relatio_found);
    }

    /* ==================================================
     * Probare repositorium capere_relationes_ad
     * ================================================== */

    {
        Entitas* target;
        Entitas* src1;
        Entitas* src2;
        Relatio* rel1;
        Relatio* rel2;
        Xar*     relationes_ad;

        imprimere("\n--- Probans repositorium capere_relationes_ad ---\n");

        /* Creare target et duas fontes per repositorium */
        target = repo->entitas_creare(repo->datum, "node");
        src1   = repo->entitas_creare(repo->datum, "node");
        src2   = repo->entitas_creare(repo->datum, "node");

        CREDO_NON_NIHIL(target);
        CREDO_NON_NIHIL(src1);
        CREDO_NON_NIHIL(src2);

        /* Addere relationes AD target per repositorium */
        rel1 = repo->relatio_addere(repo->datum, src1, "points_to", target->id);
        rel2 = repo->relatio_addere(repo->datum, src2, "points_to", target->id);

        CREDO_NON_NIHIL(rel1);
        CREDO_NON_NIHIL(rel2);

        /* Capere relationes AD target */
        relationes_ad = repo->capere_relationes_ad(repo->datum, target->id);

        CREDO_NON_NIHIL(relationes_ad);
        CREDO_AEQUALIS_I32(xar_numerus(relationes_ad), II);
    }

    /* ==================================================
     * Probare repositorium entitas_delere
     * ================================================== */

    {
        Entitas* ent_to_delete;
        Entitas* ent_found;
        chorda*  id_saved;
        i32      numerus_ante;
        i32      numerus_post;

        imprimere("\n--- Probans repositorium entitas_delere ---\n");

        numerus_ante = graphus_entitatum_numerus(graphus);

        /* Creare entitatem */
        ent_to_delete = repo->entitas_creare(repo->datum, "temporary");
        CREDO_NON_NIHIL(ent_to_delete);

        id_saved = ent_to_delete->id;

        CREDO_AEQUALIS_I32(graphus_entitatum_numerus(graphus), numerus_ante + I);

        /* Delere entitatem */
        CREDO_VERUM(repo->entitas_delere(repo->datum, id_saved));

        numerus_post = graphus_entitatum_numerus(graphus);
        CREDO_AEQUALIS_I32(numerus_post, numerus_ante);

        /* Verificare non potest capere */
        ent_found = repo->capere_entitatem(repo->datum, id_saved);
        CREDO_NIHIL(ent_found);

        /* Delere non existentem */
        CREDO_FALSUM(repo->entitas_delere(
            repo->datum,
            chorda_internare_ex_literis(intern, "non-existent")));
    }

    /* ==================================================
     * Probare repositorium proprietas_delere
     * ================================================== */

    {
        Entitas* ent;
        chorda*  clavis;
        chorda*  valor;

        imprimere("\n--- Probans repositorium proprietas_delere ---\n");

        /* Creare entitatem cum proprietate */
        ent = repo->entitas_creare(repo->datum, "document");
        CREDO_NON_NIHIL(ent);

        CREDO_VERUM(repo->proprietas_ponere(repo->datum, ent, "title", "Test Document"));

        /* Verificare proprietatem existit */
        clavis = chorda_internare_ex_literis(intern, "title");
        valor = entitas_proprietas_capere(ent, clavis);
        CREDO_NON_NIHIL(valor);

        /* Delere proprietatem */
        CREDO_VERUM(repo->proprietas_delere(repo->datum, ent, "title"));

        /* Verificare proprietatem non existit */
        valor = entitas_proprietas_capere(ent, clavis);
        CREDO_NIHIL(valor);
    }

    /* ==================================================
     * Probare repositorium relatio_delere
     * ================================================== */

    {
        Entitas* ent_from;
        Entitas* ent_to;
        Relatio* relatio;
        Relatio* relatio_found;
        chorda*  relatio_id;

        imprimere("\n--- Probans repositorium relatio_delere ---\n");

        /* Creare entitates et relationem */
        ent_from = repo->entitas_creare(repo->datum, "node");
        ent_to   = repo->entitas_creare(repo->datum, "node");

        CREDO_NON_NIHIL(ent_from);
        CREDO_NON_NIHIL(ent_to);

        relatio = repo->relatio_addere(repo->datum, ent_from, "temp_link", ent_to->id);
        CREDO_NON_NIHIL(relatio);

        relatio_id = relatio->id;

        /* Verificare relatio existit */
        relatio_found = repo->capere_relatio(repo->datum, relatio_id);
        CREDO_NON_NIHIL(relatio_found);

        /* Delere relationem */
        CREDO_VERUM(repo->relatio_delere(repo->datum, relatio_id));

        /* Verificare relatio non existit in indice */
        relatio_found = repo->capere_relatio(repo->datum, relatio_id);
        CREDO_NIHIL(relatio_found);
    }

    /* ==================================================
     * Probare repositorium nota_delere
     * ================================================== */

    {
        Entitas* ent;
        chorda*  nota;

        imprimere("\n--- Probans repositorium nota_delere ---\n");

        /* Creare entitatem cum nota */
        ent = repo->entitas_creare(repo->datum, "task");
        CREDO_NON_NIHIL(ent);

        CREDO_VERUM(repo->nota_addere(repo->datum, ent, "#deletable"));

        /* Verificare nota existit */
        nota = chorda_internare_ex_literis(intern, "#deletable");
        CREDO_VERUM(entitas_nota_habet(ent, nota));

        /* Delere notam */
        CREDO_VERUM(repo->nota_delere(repo->datum, ent, "#deletable"));

        /* Verificare nota non existit */
        CREDO_FALSUM(entitas_nota_habet(ent, nota));
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

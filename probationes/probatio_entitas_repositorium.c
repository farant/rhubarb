/* probatio_entitas_repositorium.c - Probationes EntitasRepositorium
 *
 * Probare unified repositorium cum ambobus backends:
 * - persistentia_memoria (in-memory)
 * - persistentia_nuntium (file-based)
 */
#include "latina.h"
#include "persistentia.h"
#include "entitas_repositorium.h"
#include "internamentum.h"
#include "piscina.h"
#include "uuid.h"
#include "credo.h"
#include <stdio.h>
#include <unistd.h>  /* pro unlink */

/* Via temporalis pro file tests */
constans character* VIA_PROBATIO = "/tmp/probatio_repositorium.log";

/* ==================================================
 * Auxiliares
 * ================================================== */

/* Creare repositorium cum memoria backend */
interior EntitasRepositorium*
_creare_repo_memoria(Piscina* piscina)
{
    Persistentia* pers;

    pers = persistentia_memoria_creare(piscina);
    si (!pers)
    {
        redde NIHIL;
    }

    redde entitas_repositorium_creare(piscina, pers);
}

/* Creare repositorium cum nuntium backend (novum file) */
interior EntitasRepositorium*
_creare_repo_nuntium(Piscina* piscina)
{
    Persistentia* pers;

    /* Delere file si existit */
    unlink(VIA_PROBATIO);

    pers = persistentia_nuntium_creare(piscina, VIA_PROBATIO);
    si (!pers)
    {
        redde NIHIL;
    }

    redde entitas_repositorium_creare(piscina, pers);
}

/* ==================================================
 * Probationes Communes (currunt pro ambobus backends)
 * ================================================== */

interior vacuum
probare_entitas_creare(EntitasRepositorium* repo)
{
    Entitas* entitas;

    imprimere("  entitas_creare...\n");

    entitas = repo->entitas_creare(repo->datum, "Document");
    CREDO_NON_NIHIL(entitas);
    CREDO_NON_NIHIL(entitas->id);
    CREDO_VERUM(chorda_aequalis_literis(*entitas->genus, "Document"));

    /* Capere per ID debet invenire */
    CREDO_AEQUALIS_PTR(repo->capere_entitatem(repo->datum, entitas->id), entitas);
}

interior vacuum
probare_entitas_scaffoldare(EntitasRepositorium* repo)
{
    Entitas* entitas_1;
    Entitas* entitas_2;
    Entitas* entitas_3;

    imprimere("  entitas_scaffoldare...\n");

    /* Prima vocatio creat */
    entitas_1 = repo->entitas_scaffoldare(repo->datum, "Page", "home");
    CREDO_NON_NIHIL(entitas_1);
    CREDO_VERUM(chorda_aequalis_literis(*entitas_1->genus, "Page"));

    /* Secunda vocatio reddit eadem */
    entitas_2 = repo->entitas_scaffoldare(repo->datum, "Page", "home");
    CREDO_AEQUALIS_PTR(entitas_1, entitas_2);

    /* Differens slug creat novam */
    entitas_3 = repo->entitas_scaffoldare(repo->datum, "Page", "about");
    CREDO_NON_NIHIL(entitas_3);
    CREDO_VERUM(entitas_1 != entitas_3);
}

interior vacuum
probare_proprietas_ponere(EntitasRepositorium* repo)
{
    Entitas* entitas;
    chorda*  valor;

    imprimere("  proprietas_ponere...\n");

    entitas = repo->entitas_scaffoldare(repo->datum, "Item", "test-props");
    CREDO_NON_NIHIL(entitas);

    /* Ponere proprietas */
    CREDO_VERUM(repo->proprietas_ponere(repo->datum, entitas, "name", "Test Item"));
    CREDO_VERUM(repo->proprietas_ponere(repo->datum, entitas, "count", "42"));

    /* Verificare */
    valor = entitas_proprietas_capere(entitas, chorda_internare_ex_literis(
        internamentum_globale(), "name"));
    CREDO_NON_NIHIL(valor);
    CREDO_VERUM(chorda_aequalis_literis(*valor, "Test Item"));

    /* Superscribere */
    CREDO_VERUM(repo->proprietas_ponere(repo->datum, entitas, "name", "Updated"));
    valor = entitas_proprietas_capere(entitas, chorda_internare_ex_literis(
        internamentum_globale(), "name"));
    CREDO_VERUM(chorda_aequalis_literis(*valor, "Updated"));
}

interior vacuum
probare_proprietas_delere(EntitasRepositorium* repo)
{
    Entitas* entitas;
    chorda*  clavis;

    imprimere("  proprietas_delere...\n");

    entitas = repo->entitas_scaffoldare(repo->datum, "Item", "test-del-props");
    repo->proprietas_ponere(repo->datum, entitas, "temp", "value");

    clavis = chorda_internare_ex_literis(internamentum_globale(), "temp");
    CREDO_VERUM(entitas_proprietas_habet(entitas, clavis));

    CREDO_VERUM(repo->proprietas_delere(repo->datum, entitas, "temp"));
    CREDO_FALSUM(entitas_proprietas_habet(entitas, clavis));
}

interior vacuum
probare_relatio_addere(EntitasRepositorium* repo)
{
    Entitas* parent;
    Entitas* child_1;
    Entitas* child_2;
    Relatio* rel;
    Xar*     relatae;

    imprimere("  relatio_addere...\n");

    parent = repo->entitas_scaffoldare(repo->datum, "Folder", "parent");
    child_1 = repo->entitas_scaffoldare(repo->datum, "File", "child1");
    child_2 = repo->entitas_scaffoldare(repo->datum, "File", "child2");

    rel = repo->relatio_addere(repo->datum, parent, "contains", child_1->id);
    CREDO_NON_NIHIL(rel);
    CREDO_VERUM(chorda_aequalis_literis(*rel->genus, "contains"));
    CREDO_AEQUALIS_PTR(rel->destinatio_id, child_1->id);

    repo->relatio_addere(repo->datum, parent, "contains", child_2->id);

    /* Verificare capere_entitates_relatae */
    relatae = repo->capere_entitates_relatae(
        repo->datum, parent,
        chorda_internare_ex_literis(internamentum_globale(), "contains"));
    CREDO_NON_NIHIL(relatae);
    CREDO_AEQUALIS_I32(xar_numerus(relatae), II);
}

interior vacuum
probare_relatio_delere(EntitasRepositorium* repo)
{
    Entitas* a;
    Entitas* b;
    Relatio* rel;
    Xar*     relatae;
    chorda*  contains;

    imprimere("  relatio_delere...\n");

    a = repo->entitas_scaffoldare(repo->datum, "Node", "a");
    b = repo->entitas_scaffoldare(repo->datum, "Node", "b");

    rel = repo->relatio_addere(repo->datum, a, "links", b->id);
    CREDO_NON_NIHIL(rel);

    contains = chorda_internare_ex_literis(internamentum_globale(), "links");
    relatae = repo->capere_entitates_relatae(repo->datum, a, contains);
    CREDO_AEQUALIS_I32(xar_numerus(relatae), I);

    CREDO_VERUM(repo->relatio_delere(repo->datum, rel->id));

    relatae = repo->capere_entitates_relatae(repo->datum, a, contains);
    CREDO_AEQUALIS_I32(xar_numerus(relatae), ZEPHYRUM);
}

interior vacuum
probare_nota_addere(EntitasRepositorium* repo)
{
    Entitas* entitas;
    Xar*     resultus;

    imprimere("  nota_addere...\n");

    entitas = repo->entitas_scaffoldare(repo->datum, "Task", "task1");

    CREDO_VERUM(repo->nota_addere(repo->datum, entitas, "#important"));
    CREDO_VERUM(repo->nota_addere(repo->datum, entitas, "#urgent"));

    CREDO_AEQUALIS_I32(entitas_numerus_notarum(entitas), II);

    /* Quaerere per nota */
    resultus = repo->quaerere_cum_nota(
        repo->datum,
        chorda_internare_ex_literis(internamentum_globale(), "#important"));
    CREDO_NON_NIHIL(resultus);
    CREDO_AEQUALIS_I32(xar_numerus(resultus), I);
}

interior vacuum
probare_nota_delere(EntitasRepositorium* repo)
{
    Entitas* entitas;
    Xar*     resultus;
    chorda*  nota;

    imprimere("  nota_delere...\n");

    entitas = repo->entitas_scaffoldare(repo->datum, "Task", "task-del");
    repo->nota_addere(repo->datum, entitas, "#temp");

    nota = chorda_internare_ex_literis(internamentum_globale(), "#temp");
    resultus = repo->quaerere_cum_nota(repo->datum, nota);
    CREDO_AEQUALIS_I32(xar_numerus(resultus), I);

    CREDO_VERUM(repo->nota_delere(repo->datum, entitas, "#temp"));

    resultus = repo->quaerere_cum_nota(repo->datum, nota);
    CREDO_AEQUALIS_I32(xar_numerus(resultus), ZEPHYRUM);
}

interior vacuum
probare_capere_radices(EntitasRepositorium* repo)
{
    Entitas* root;
    Xar*     radices;

    imprimere("  capere_radices...\n");

    /* Scaffoldare root cum nota #root */
    root = repo->entitas_scaffoldare(repo->datum, "Root", "system");
    repo->nota_addere(repo->datum, root, "#root");

    radices = repo->capere_radices(repo->datum);
    CREDO_NON_NIHIL(radices);
    CREDO_AEQUALIS_I32(xar_numerus(radices), I);
    CREDO_AEQUALIS_PTR(*(Entitas**)xar_obtinere(radices, ZEPHYRUM), root);
}

interior vacuum
probare_genus_hierarchia(EntitasRepositorium* repo)
{
    Entitas* article;
    Entitas* genus_content;
    Entitas* genus_document;
    Entitas* genus_article;

    imprimere("  genus_hierarchia...\n");

    /* Creare entitas cum genus hierarchico */
    article = repo->entitas_scaffoldare(
        repo->datum, "Content::Document::Article", "post1");
    CREDO_NON_NIHIL(article);

    /* Verificare genus entitates auto-scaffoldatae */
    genus_content = repo->entitas_scaffoldare(repo->datum, "Genus", "Content");
    genus_document = repo->entitas_scaffoldare(
        repo->datum, "Genus", "Content::Document");
    genus_article = repo->entitas_scaffoldare(
        repo->datum, "Genus", "Content::Document::Article");

    CREDO_NON_NIHIL(genus_content);
    CREDO_NON_NIHIL(genus_document);
    CREDO_NON_NIHIL(genus_article);

    /* Verificare relationes inter genus entitates */
    /* genus_content --contains--> genus_document --contains--> genus_article */
}

interior vacuum
probare_entitas_delere(EntitasRepositorium* repo)
{
    Entitas* entitas;
    chorda*  id;

    imprimere("  entitas_delere...\n");

    entitas = repo->entitas_scaffoldare(repo->datum, "Temp", "to-delete");
    CREDO_NON_NIHIL(entitas);

    id = entitas->id;
    CREDO_NON_NIHIL(repo->capere_entitatem(repo->datum, id));

    CREDO_VERUM(repo->entitas_delere(repo->datum, id));
    CREDO_NIHIL(repo->capere_entitatem(repo->datum, id));
}

interior vacuum
probare_quaerere_textum(EntitasRepositorium* repo)
{
    Entitas* e1;
    Entitas* e2;
    Xar*     resultus;

    imprimere("  quaerere_textum...\n");

    e1 = repo->entitas_scaffoldare(repo->datum, "Doc", "search1");
    e2 = repo->entitas_scaffoldare(repo->datum, "Doc", "search2");

    repo->proprietas_ponere(repo->datum, e1, "title", "Hello World");
    repo->proprietas_ponere(repo->datum, e2, "title", "Goodbye World");

    resultus = repo->quaerere_textum(repo->datum, "World");
    CREDO_NON_NIHIL(resultus);
    CREDO_AEQUALIS_I32(xar_numerus(resultus), II);

    resultus = repo->quaerere_textum(repo->datum, "Hello");
    CREDO_AEQUALIS_I32(xar_numerus(resultus), I);

    resultus = repo->quaerere_textum(repo->datum, "NotFound");
    CREDO_AEQUALIS_I32(xar_numerus(resultus), ZEPHYRUM);
}

interior vacuum
probare_quaerere_cum_praefixo_notae(EntitasRepositorium* repo)
{
    Entitas* e1;
    Entitas* e2;
    Entitas* e3;
    Xar*     resultus;

    imprimere("  quaerere_cum_praefixo_notae...\n");

    e1 = repo->entitas_scaffoldare(repo->datum, "Project", "alpha");
    e2 = repo->entitas_scaffoldare(repo->datum, "Project", "beta");
    e3 = repo->entitas_scaffoldare(repo->datum, "Task", "other");

    repo->nota_addere(repo->datum, e1, "#project::alpha");
    repo->nota_addere(repo->datum, e2, "#project::beta");
    repo->nota_addere(repo->datum, e3, "#task::other");

    /* Quaerere cum praefixo "#project::" */
    resultus = repo->quaerere_cum_praefixo_notae(repo->datum, "#project::");
    CREDO_NON_NIHIL(resultus);
    CREDO_AEQUALIS_I32(xar_numerus(resultus), II);

    /* Quaerere cum praefixo "#task::" */
    resultus = repo->quaerere_cum_praefixo_notae(repo->datum, "#task::");
    CREDO_NON_NIHIL(resultus);
    CREDO_AEQUALIS_I32(xar_numerus(resultus), I);

    /* Praefixum non existens */
    resultus = repo->quaerere_cum_praefixo_notae(repo->datum, "#nonexistent::");
    CREDO_AEQUALIS_I32(xar_numerus(resultus), ZEPHYRUM);
}

interior vacuum
probare_capere_relatio(EntitasRepositorium* repo)
{
    Entitas* a;
    Entitas* b;
    Relatio* rel;
    Relatio* found;
    chorda*  fake_id;

    imprimere("  capere_relatio...\n");

    a = repo->entitas_scaffoldare(repo->datum, "Node", "rel-test-a");
    b = repo->entitas_scaffoldare(repo->datum, "Node", "rel-test-b");

    rel = repo->relatio_addere(repo->datum, a, "points_to", b->id);
    CREDO_NON_NIHIL(rel);
    CREDO_NON_NIHIL(rel->id);

    /* Capere per ID */
    found = repo->capere_relatio(repo->datum, rel->id);
    CREDO_NON_NIHIL(found);
    CREDO_AEQUALIS_PTR(found, rel);
    CREDO_AEQUALIS_PTR(found->origo_id, a->id);
    CREDO_AEQUALIS_PTR(found->destinatio_id, b->id);

    /* Capere non existentem */
    fake_id = chorda_internare_ex_literis(internamentum_globale(), "fake-rel-id");
    found = repo->capere_relatio(repo->datum, fake_id);
    CREDO_NIHIL(found);
}

interior vacuum
probare_capere_relationes_ad(EntitasRepositorium* repo)
{
    Entitas* target;
    Entitas* src1;
    Entitas* src2;
    Xar*     relationes_ad;
    i32      numerus_ante;

    imprimere("  capere_relationes_ad...\n");

    target = repo->entitas_scaffoldare(repo->datum, "BackRefTarget", "unique-target");

    /* Capere numerum relationum ante addere */
    relationes_ad = repo->capere_relationes_ad(repo->datum, target->id);
    numerus_ante = relationes_ad ? xar_numerus(relationes_ad) : ZEPHYRUM;

    src1 = repo->entitas_scaffoldare(repo->datum, "BackRefSource", "unique-src1");
    src2 = repo->entitas_scaffoldare(repo->datum, "BackRefSource", "unique-src2");

    /* Duae relationes AD target */
    repo->relatio_addere(repo->datum, src1, "backref_test", target->id);
    repo->relatio_addere(repo->datum, src2, "backref_test", target->id);

    /* Capere relationes AD target (back-references) */
    relationes_ad = repo->capere_relationes_ad(repo->datum, target->id);
    CREDO_NON_NIHIL(relationes_ad);
    CREDO_AEQUALIS_I32(xar_numerus(relationes_ad), numerus_ante + II);
}

interior vacuum
probare_scaffoldare_uuidv5(EntitasRepositorium* repo)
{
    Entitas* ent;

    imprimere("  scaffoldare_uuidv5...\n");

    ent = repo->entitas_scaffoldare(repo->datum, "Test", "uuid-check");
    CREDO_NON_NIHIL(ent);

    /* Scaffoldata entitas debet habere UUIDv5 (version digit = 5) */
    CREDO_VERUM(uuid_est_v5(*ent->id));
    CREDO_FALSUM(uuid_est_v7(*ent->id));
}

interior vacuum
probare_genus_contains_relationes(EntitasRepositorium* repo)
{
    Entitas* genus_radix;
    Entitas* genus_unique;
    Entitas* genus_unique_deep;
    chorda*  contains_genus;
    Xar*     relationes;

    imprimere("  genus_contains_relationes...\n");

    /* Usare genus unicum pro evitare conflictus cum aliis tests */
    /* Scaffoldare entitatem cum genere profundo pro triggere hierarchiam */
    repo->entitas_scaffoldare(repo->datum, "UniqueGenusTest::Deep::Hierarchy", "trigger");

    /* Genus::Genus (radix) */
    genus_radix = repo->entitas_scaffoldare(repo->datum, "Genus", "Genus");
    CREDO_NON_NIHIL(genus_radix);

    contains_genus = chorda_internare_ex_literis(internamentum_globale(), "contains");

    /* Genus::Genus debet habere "contains" relationes */
    relationes = repo->capere_entitates_relatae(repo->datum, genus_radix, contains_genus);
    CREDO_NON_NIHIL(relationes);
    CREDO_VERUM(xar_numerus(relationes) >= I);

    /* Genus::UniqueGenusTest debet habere "contains" ad Genus::UniqueGenusTest::Deep */
    genus_unique = repo->entitas_scaffoldare(repo->datum, "Genus", "UniqueGenusTest");
    CREDO_NON_NIHIL(genus_unique);

    relationes = repo->capere_entitates_relatae(repo->datum, genus_unique, contains_genus);
    CREDO_NON_NIHIL(relationes);
    /* Saltem unam relationem (potest habere plus si test currit iterum) */
    CREDO_VERUM(xar_numerus(relationes) >= I);

    /* Genus::UniqueGenusTest::Deep debet habere "contains" ad Genus::UniqueGenusTest::Deep::Hierarchy */
    genus_unique_deep = repo->entitas_scaffoldare(repo->datum, "Genus", "UniqueGenusTest::Deep");
    CREDO_NON_NIHIL(genus_unique_deep);

    relationes = repo->capere_entitates_relatae(repo->datum, genus_unique_deep, contains_genus);
    CREDO_NON_NIHIL(relationes);
    CREDO_VERUM(xar_numerus(relationes) >= I);
}

/* ==================================================
 * Currere omnes probationes pro uno backend
 * ================================================== */

interior vacuum
currere_probationes_backend(
    EntitasRepositorium* repo,
    constans character*  nomen_backend)
{
    imprimere("\n=== Backend: %s ===\n", nomen_backend);

    probare_entitas_creare(repo);
    probare_entitas_scaffoldare(repo);
    probare_proprietas_ponere(repo);
    probare_proprietas_delere(repo);
    probare_relatio_addere(repo);
    probare_relatio_delere(repo);
    probare_nota_addere(repo);
    probare_nota_delere(repo);
    probare_capere_radices(repo);
    probare_genus_hierarchia(repo);
    probare_entitas_delere(repo);
    probare_quaerere_textum(repo);
    probare_quaerere_cum_praefixo_notae(repo);
    probare_capere_relatio(repo);
    probare_capere_relationes_ad(repo);
    probare_scaffoldare_uuidv5(repo);
    probare_genus_contains_relationes(repo);
}

/* ==================================================
 * Probare Replay (solum pro nuntium backend)
 * ================================================== */

interior vacuum
probare_replay(Piscina* piscina)
{
    EntitasRepositorium* repo;
    chorda*              valor;
    Xar*                 relatae;

    imprimere("\n=== Probare Replay (Nuntium) ===\n");

    /* Delere file antiquum */
    unlink(VIA_PROBATIO);

    /* PHASE 1: Creare et populare */
    imprimere("  Phase 1: Creare datum...\n");
    {
        Persistentia* pers;
        Entitas*      root;
        Entitas*      page;

        pers = persistentia_nuntium_creare(piscina, VIA_PROBATIO);
        CREDO_NON_NIHIL(pers);

        repo = entitas_repositorium_creare(piscina, pers);
        CREDO_NON_NIHIL(repo);

        /* Creare entitates */
        root = repo->entitas_scaffoldare(repo->datum, "Root", "replay-root");
        repo->nota_addere(repo->datum, root, "#root");
        repo->proprietas_ponere(repo->datum, root, "name", "Replay Root");

        page = repo->entitas_scaffoldare(repo->datum, "Page", "replay-page");
        repo->proprietas_ponere(repo->datum, page, "title", "Test Page");

        repo->relatio_addere(repo->datum, root, "contains", page->id);

        /* Sync ad file */
        pers->sync(pers->datum);
        pers->claudere(pers->datum);
    }

    /* PHASE 2: Aperire et verificare replay */
    imprimere("  Phase 2: Aperire et verificare...\n");
    {
        Persistentia* pers;
        Entitas*      root;
        Entitas*      page;
        chorda*       contains;

        /* Nova piscina pro simulare fresh start */
        /* (in realitate usamus eadem piscina sed repo est novum) */

        pers = persistentia_nuntium_aperire(piscina, VIA_PROBATIO);
        CREDO_NON_NIHIL(pers);

        repo = entitas_repositorium_creare(piscina, pers);
        CREDO_NON_NIHIL(repo);

        /* Verificare root existit */
        root = repo->entitas_scaffoldare(repo->datum, "Root", "replay-root");
        CREDO_NON_NIHIL(root);

        valor = entitas_proprietas_capere(root, chorda_internare_ex_literis(
            internamentum_globale(), "name"));
        CREDO_NON_NIHIL(valor);
        CREDO_VERUM(chorda_aequalis_literis(*valor, "Replay Root"));

        /* Verificare page existit */
        page = repo->entitas_scaffoldare(repo->datum, "Page", "replay-page");
        CREDO_NON_NIHIL(page);

        valor = entitas_proprietas_capere(page, chorda_internare_ex_literis(
            internamentum_globale(), "title"));
        CREDO_NON_NIHIL(valor);
        CREDO_VERUM(chorda_aequalis_literis(*valor, "Test Page"));

        /* Verificare relatio existit */
        contains = chorda_internare_ex_literis(internamentum_globale(), "contains");
        relatae = repo->capere_entitates_relatae(repo->datum, root, contains);
        CREDO_NON_NIHIL(relatae);
        CREDO_AEQUALIS_I32(xar_numerus(relatae), I);

        imprimere("  Replay verificatum!\n");

        pers->claudere(pers->datum);
    }

    /* Cleanup */
    unlink(VIA_PROBATIO);
}

interior vacuum
probare_continuare_post_replay(Piscina* piscina)
{
    imprimere("\n=== Probare Continuare Post Replay ===\n");

    /* Delere file antiquum */
    unlink(VIA_PROBATIO);

    /* PHASE 1: Creare initiale datum */
    imprimere("  Phase 1: Creare initiale...\n");
    {
        Persistentia*        pers;
        EntitasRepositorium* repo;

        pers = persistentia_nuntium_creare(piscina, VIA_PROBATIO);
        CREDO_NON_NIHIL(pers);

        repo = entitas_repositorium_creare(piscina, pers);
        CREDO_NON_NIHIL(repo);

        repo->entitas_scaffoldare(repo->datum, "Entity", "first");
        repo->entitas_scaffoldare(repo->datum, "Entity", "second");

        pers->sync(pers->datum);
        pers->claudere(pers->datum);
    }

    /* PHASE 2: Aperire, addere plus, claudere */
    imprimere("  Phase 2: Aperire et addere...\n");
    {
        Persistentia*        pers;
        EntitasRepositorium* repo;
        Entitas*             third;

        pers = persistentia_nuntium_aperire(piscina, VIA_PROBATIO);
        CREDO_NON_NIHIL(pers);

        repo = entitas_repositorium_creare(piscina, pers);
        CREDO_NON_NIHIL(repo);

        /* Verificare duo existunt */
        CREDO_NON_NIHIL(repo->entitas_scaffoldare(repo->datum, "Entity", "first"));
        CREDO_NON_NIHIL(repo->entitas_scaffoldare(repo->datum, "Entity", "second"));

        /* Addere tertiam */
        third = repo->entitas_scaffoldare(repo->datum, "Entity", "third");
        CREDO_NON_NIHIL(third);
        repo->proprietas_ponere(repo->datum, third, "added", "in-phase-2");

        pers->sync(pers->datum);
        pers->claudere(pers->datum);
    }

    /* PHASE 3: Aperire et verificare omnia */
    imprimere("  Phase 3: Verificare omnia...\n");
    {
        Persistentia*        pers;
        EntitasRepositorium* repo;
        Entitas*             first;
        Entitas*             second;
        Entitas*             third;
        chorda*              valor;

        pers = persistentia_nuntium_aperire(piscina, VIA_PROBATIO);
        CREDO_NON_NIHIL(pers);

        repo = entitas_repositorium_creare(piscina, pers);
        CREDO_NON_NIHIL(repo);

        /* Verificare omnes tres existunt */
        first = repo->entitas_scaffoldare(repo->datum, "Entity", "first");
        second = repo->entitas_scaffoldare(repo->datum, "Entity", "second");
        third = repo->entitas_scaffoldare(repo->datum, "Entity", "third");

        CREDO_NON_NIHIL(first);
        CREDO_NON_NIHIL(second);
        CREDO_NON_NIHIL(third);

        /* Verificare proprietas addita in phase 2 */
        valor = entitas_proprietas_capere(third, chorda_internare_ex_literis(
            internamentum_globale(), "added"));
        CREDO_NON_NIHIL(valor);
        CREDO_VERUM(chorda_aequalis_literis(*valor, "in-phase-2"));

        imprimere("  Continuare verificatum!\n");

        pers->claudere(pers->datum);
    }

    /* Cleanup */
    unlink(VIA_PROBATIO);
}

/* ==================================================
 * Probare Event Log Read Functions
 * ================================================== */

interior vacuum
probare_lectio_eventorum(Piscina* piscina)
{
    Persistentia*        pers;
    EntitasRepositorium* repo;
    Entitas*             ent_a;
    Entitas*             ent_b;
    Xar*                 eventus;
    Eventum*             e;
    i32                  numerus;

    imprimere("\n=== Probare Lectio Eventorum ===\n");

    /* Creare repo */
    pers = persistentia_memoria_creare(piscina);
    CREDO_NON_NIHIL(pers);
    repo = entitas_repositorium_creare(piscina, pers);
    CREDO_NON_NIHIL(repo);

    /* Creare aliquot entitates */
    ent_a = repo->entitas_creare(repo->datum, "Page");
    CREDO_NON_NIHIL(ent_a);
    repo->proprietas_ponere(repo->datum, ent_a, "titulus", "Pagina A");

    ent_b = repo->entitas_creare(repo->datum, "Document");
    CREDO_NON_NIHIL(ent_b);
    repo->proprietas_ponere(repo->datum, ent_b, "titulus", "Documentum B");
    repo->nota_addere(repo->datum, ent_b, "#important");

    /* === Probare numerus_eventorum === */
    imprimere("  numerus_eventorum...\n");
    numerus = repo->numerus_eventorum(repo->datum);
    /* 2 CREARE + 2 PROPRIETAS + 1 NOTA = 5 */
    CREDO_AEQUALIS_I32(numerus, V);

    /* === Probare legere_omnes_eventus === */
    imprimere("  legere_omnes_eventus...\n");
    eventus = repo->legere_omnes_eventus(repo->datum);
    CREDO_NON_NIHIL(eventus);
    CREDO_AEQUALIS_I32(xar_numerus(eventus), V);

    /* Verificare primus eventus est CREARE */
    e = *(Eventum**)xar_obtinere(eventus, ZEPHYRUM);
    CREDO_NON_NIHIL(e);
    CREDO_AEQUALIS_I32((i32)e->genus, EVENTUS_CREARE_ENTITAS);

    /* === Probare legere_eventus_entitatis === */
    imprimere("  legere_eventus_entitatis...\n");
    eventus = repo->legere_eventus_entitatis(repo->datum, ent_a->id);
    CREDO_NON_NIHIL(eventus);
    /* ent_a: 1 CREARE + 1 PROPRIETAS = 2 */
    CREDO_AEQUALIS_I32(xar_numerus(eventus), II);

    eventus = repo->legere_eventus_entitatis(repo->datum, ent_b->id);
    CREDO_NON_NIHIL(eventus);
    /* ent_b: 1 CREARE + 1 PROPRIETAS + 1 NOTA = 3 */
    CREDO_AEQUALIS_I32(xar_numerus(eventus), III);

    /* === Probare legere_eventus_post_indicem === */
    imprimere("  legere_eventus_post_indicem...\n");
    eventus = repo->legere_eventus_post_indicem(repo->datum, III);
    CREDO_NON_NIHIL(eventus);
    /* Events 3, 4 (0-indexed) = 2 events */
    CREDO_AEQUALIS_I32(xar_numerus(eventus), II);

    eventus = repo->legere_eventus_post_indicem(repo->datum, ZEPHYRUM);
    CREDO_NON_NIHIL(eventus);
    CREDO_AEQUALIS_I32(xar_numerus(eventus), V);

    /* === Probare legere_eventus_recentes === */
    imprimere("  legere_eventus_recentes...\n");
    eventus = repo->legere_eventus_recentes(repo->datum, II);
    CREDO_NON_NIHIL(eventus);
    CREDO_AEQUALIS_I32(xar_numerus(eventus), II);

    /* Verificare ultimus est NOTA */
    e = *(Eventum**)xar_obtinere(eventus, I);
    CREDO_NON_NIHIL(e);
    CREDO_AEQUALIS_I32((i32)e->genus, EVENTUS_ADDERE_NOTA);

    /* === Probare legere_eventus_generis_entitatis === */
    imprimere("  legere_eventus_generis_entitatis...\n");
    eventus = repo->legere_eventus_generis_entitatis(repo->datum, "Page");
    CREDO_NON_NIHIL(eventus);
    /* Page: 1 CREARE + 1 PROPRIETAS = 2 */
    CREDO_AEQUALIS_I32(xar_numerus(eventus), II);

    eventus = repo->legere_eventus_generis_entitatis(repo->datum, "Document");
    CREDO_NON_NIHIL(eventus);
    /* Document: 1 CREARE + 1 PROPRIETAS + 1 NOTA = 3 */
    CREDO_AEQUALIS_I32(xar_numerus(eventus), III);

    eventus = repo->legere_eventus_generis_entitatis(repo->datum, "NonExistent");
    CREDO_NON_NIHIL(eventus);
    CREDO_AEQUALIS_I32(xar_numerus(eventus), ZEPHYRUM);

    /* === Probare legere_eventus_typi === */
    imprimere("  legere_eventus_typi...\n");
    eventus = repo->legere_eventus_typi(repo->datum, EVENTUS_CREARE_ENTITAS);
    CREDO_NON_NIHIL(eventus);
    CREDO_AEQUALIS_I32(xar_numerus(eventus), II);

    eventus = repo->legere_eventus_typi(repo->datum, EVENTUS_PONERE_PROPRIETAS);
    CREDO_NON_NIHIL(eventus);
    CREDO_AEQUALIS_I32(xar_numerus(eventus), II);

    eventus = repo->legere_eventus_typi(repo->datum, EVENTUS_ADDERE_NOTA);
    CREDO_NON_NIHIL(eventus);
    CREDO_AEQUALIS_I32(xar_numerus(eventus), I);

    eventus = repo->legere_eventus_typi(repo->datum, EVENTUS_DELERE_ENTITAS);
    CREDO_NON_NIHIL(eventus);
    CREDO_AEQUALIS_I32(xar_numerus(eventus), ZEPHYRUM);

    imprimere("  Lectio eventorum verificata!\n");
}

/* ==================================================
 * Principale
 * ================================================== */

s32 principale(vacuum)
{
    Piscina*             piscina;
    EntitasRepositorium* repo;
    b32                  praeteritus;

    /* Initiare */
    piscina = piscina_generare_dynamicum("probatio_repo", M * II);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }

    credo_aperire(piscina);

    /* === Backend: Memoria === */
    repo = _creare_repo_memoria(piscina);
    CREDO_NON_NIHIL(repo);
    currere_probationes_backend(repo, "Memoria");

    /* === Backend: Nuntium === */
    repo = _creare_repo_nuntium(piscina);
    CREDO_NON_NIHIL(repo);
    currere_probationes_backend(repo, "Nuntium");

    /* === Probare Replay === */
    probare_replay(piscina);

    /* === Probare Continuare Post Replay === */
    probare_continuare_post_replay(piscina);

    /* === Probare Lectio Eventorum === */
    probare_lectio_eventorum(piscina);

    /* Cleanup file */
    unlink(VIA_PROBATIO);

    /* Compendium */
    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();

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

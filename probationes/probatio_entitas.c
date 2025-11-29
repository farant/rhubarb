/* probatio_entitas.c - Probationes Entitatis */
#include "latina.h"
#include "entitas.h"
#include "internamentum.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>

s32 principale(vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
                    b32  praeteritus;

    /* Aperire piscinam et credonem */
    piscina = piscina_generare_dynamicum("probatio_entitas", CDLVI);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    /* Creare internamentum pro chordas */
    intern = internamentum_creare(piscina);
    si (!intern)
    {
        imprimere("FRACTA: internamentum_creare\n");
        redde I;
    }

    /* ==================================================
     * Probare entitas_creare
     * ================================================== */

    {
        Entitas* entitas;
         chorda* id;
         chorda* genus;

        imprimere("\n--- Probans entitas_creare ---\n");

        id    = chorda_internare_ex_literis(intern, "entity-1");
        genus = chorda_internare_ex_literis(intern, "document");

        entitas = entitas_creare(piscina, id, genus);
        CREDO_NON_NIHIL(entitas);
        CREDO_AEQUALIS_PTR(entitas->id, id);
        CREDO_AEQUALIS_PTR(entitas->genus, genus);
        CREDO_AEQUALIS_I32(entitas_numerus_proprietatum(entitas), ZEPHYRUM);
        CREDO_AEQUALIS_I32(entitas_numerus_relationum(entitas), ZEPHYRUM);
        CREDO_AEQUALIS_I32(entitas_numerus_notarum(entitas), ZEPHYRUM);
    }

    /* ==================================================
     * Probare proprietates
     * ================================================== */

    {
        Entitas* entitas;
         chorda* id;
         chorda* genus;
         chorda* clavis_nomen;
         chorda* valor_nomen;
         chorda* clavis_color;
         chorda* valor_color;
         chorda* resultus;

        imprimere("\n--- Probans proprietates ---\n");

        id    = chorda_internare_ex_literis(intern, "entity-2");
        genus = chorda_internare_ex_literis(intern, "file");

        entitas = entitas_creare(piscina, id, genus);

        /* Addere proprietates */
        clavis_nomen = chorda_internare_ex_literis(intern, "name");
        valor_nomen  = chorda_internare_ex_literis(intern, "test.txt");
        CREDO_VERUM(entitas_proprietas_ponere(entitas, clavis_nomen, valor_nomen));

        clavis_color = chorda_internare_ex_literis(intern, "color");
        valor_color  = chorda_internare_ex_literis(intern, "blue");
        CREDO_VERUM(entitas_proprietas_ponere(entitas, clavis_color, valor_color));

        CREDO_AEQUALIS_I32(entitas_numerus_proprietatum(entitas), II);

        /* Capere proprietates */
        resultus = entitas_proprietas_capere(entitas, clavis_nomen);
        CREDO_NON_NIHIL(resultus);
        CREDO_AEQUALIS_PTR(resultus, valor_nomen);

        resultus = entitas_proprietas_capere(entitas, clavis_color);
        CREDO_AEQUALIS_PTR(resultus, valor_color);

        /* Verificare habet */
        CREDO_VERUM(entitas_proprietas_habet(entitas, clavis_nomen));
        CREDO_VERUM(entitas_proprietas_habet(entitas, clavis_color));

        /* Verificare non existens */
        resultus = entitas_proprietas_capere(
            entitas,
            chorda_internare_ex_literis(intern, "nonexistent"));
        CREDO_NIHIL(resultus);
    }

    /* ==================================================
     * Probare renovatio proprietatis
     * ================================================== */

    {
        Entitas* entitas;
         chorda* clavis;
         chorda* valor1;
         chorda* valor2;
         chorda* resultus;

        imprimere("\n--- Probans renovatio proprietatis ---\n");

        entitas = entitas_creare(
            piscina,
            chorda_internare_ex_literis(intern, "entity-3"),
            chorda_internare_ex_literis(intern, "config"));

        clavis = chorda_internare_ex_literis(intern, "version");
        valor1 = chorda_internare_ex_literis(intern, "1.0");
        valor2 = chorda_internare_ex_literis(intern, "2.0");

        /* Addere proprietatem */
        CREDO_VERUM(entitas_proprietas_ponere(entitas, clavis, valor1));
        CREDO_AEQUALIS_I32(entitas_numerus_proprietatum(entitas), I);

        resultus = entitas_proprietas_capere(entitas, clavis);
        CREDO_AEQUALIS_PTR(resultus, valor1);

        /* Renovare proprietatem */
        CREDO_VERUM(entitas_proprietas_ponere(entitas, clavis, valor2));
        CREDO_AEQUALIS_I32(entitas_numerus_proprietatum(entitas), I);  /* Adhuc I */

        resultus = entitas_proprietas_capere(entitas, clavis);
        CREDO_AEQUALIS_PTR(resultus, valor2);  /* Renovatus */
    }

    /* ==================================================
     * Probare relationes
     * ================================================== */

    {
        Entitas* entitas;
         chorda* genus_continet;
         chorda* genus_referit;
         chorda* dest1;
         chorda* dest2;
         chorda* dest3;

        imprimere("\n--- Probans relationes ---\n");

        entitas = entitas_creare(
            piscina,
            chorda_internare_ex_literis(intern, "folder-1"),
            chorda_internare_ex_literis(intern, "folder"));

        genus_continet = chorda_internare_ex_literis(intern, "contains");
        genus_referit  = chorda_internare_ex_literis(intern, "references");

        dest1 = chorda_internare_ex_literis(intern, "file-1");
        dest2 = chorda_internare_ex_literis(intern, "file-2");
        dest3 = chorda_internare_ex_literis(intern, "doc-1");

        /* Addere relationes */
        CREDO_NON_NIHIL(entitas_relatio_addere(entitas, piscina, intern, genus_continet, dest1));
        CREDO_NON_NIHIL(entitas_relatio_addere(entitas, piscina, intern, genus_continet, dest2));
        CREDO_NON_NIHIL(entitas_relatio_addere(entitas, piscina, intern, genus_referit, dest3));

        CREDO_AEQUALIS_I32(entitas_numerus_relationum(entitas), III);
    }

    /* ==================================================
     * Probare entitas_relatio_addere_cum_id
     * ================================================== */

    {
        Entitas* entitas;
        Relatio* relatio;
         chorda* relatio_id;
         chorda* genus;
         chorda* dest_id;

        imprimere("\n--- Probans entitas_relatio_addere_cum_id ---\n");

        entitas = entitas_creare(
            piscina,
            chorda_internare_ex_literis(intern, "folder-cum-id"),
            chorda_internare_ex_literis(intern, "folder"));

        /* Creare ID specificum pro relatione (simulans replay) */
        relatio_id = chorda_internare_ex_literis(intern, "relatio-id-12345");
        genus      = chorda_internare_ex_literis(intern, "contains");
        dest_id    = chorda_internare_ex_literis(intern, "file-xyz");

        /* Addere relationem cum ID specifico */
        relatio = entitas_relatio_addere_cum_id(
            entitas, piscina, intern, relatio_id, genus, dest_id);

        CREDO_NON_NIHIL(relatio);
        CREDO_AEQUALIS_PTR(relatio->id, relatio_id);
        CREDO_AEQUALIS_PTR(relatio->genus, genus);
        CREDO_AEQUALIS_PTR(relatio->destinatio_id, dest_id);
        CREDO_AEQUALIS_PTR(relatio->origo_id, entitas->id);

        CREDO_AEQUALIS_I32(entitas_numerus_relationum(entitas), I);

        /* Verificare quod relatio invenitur per ID */
        CREDO_NON_NIHIL(entitas_relatio_capere(entitas, relatio_id));

        /* Addere secundam relationem cum alio ID specifico */
        relatio = entitas_relatio_addere_cum_id(
            entitas,
            piscina,
            intern,
            chorda_internare_ex_literis(intern, "relatio-id-67890"),
            genus,
            chorda_internare_ex_literis(intern, "file-abc"));

        CREDO_NON_NIHIL(relatio);
        CREDO_AEQUALIS_I32(entitas_numerus_relationum(entitas), II);
    }

    /* ==================================================
     * Probare relationes_generis_capere
     * ================================================== */

    {
        Entitas* entitas;
         chorda* genus_continet;
         chorda* genus_referit;
         chorda* dest1;
         chorda* dest2;
         chorda* dest3;
            Xar* filtratae;
        Relatio* relatio;

        imprimere("\n--- Probans relationes_generis_capere ---\n");

        entitas = entitas_creare(
            piscina,
            chorda_internare_ex_literis(intern, "folder-2"),
            chorda_internare_ex_literis(intern, "folder"));

        genus_continet = chorda_internare_ex_literis(intern, "contains");
        genus_referit  = chorda_internare_ex_literis(intern, "references");

        dest1 = chorda_internare_ex_literis(intern, "file-1");
        dest2 = chorda_internare_ex_literis(intern, "file-2");
        dest3 = chorda_internare_ex_literis(intern, "doc-1");

        entitas_relatio_addere(entitas, piscina, intern, genus_continet, dest1);
        entitas_relatio_addere(entitas, piscina, intern, genus_continet, dest2);
        entitas_relatio_addere(entitas, piscina, intern, genus_referit, dest3);

        /* Filtrare per genus "contains" */
        filtratae = entitas_relationes_generis_capere(entitas, genus_continet, piscina);
        CREDO_NON_NIHIL(filtratae);
        CREDO_AEQUALIS_I32(xar_numerus(filtratae), II);

        /* Verificare prima relatio */
        relatio = (Relatio*)xar_obtinere(filtratae, ZEPHYRUM);
        CREDO_NON_NIHIL(relatio);
        CREDO_AEQUALIS_PTR(relatio->genus, genus_continet);
        CREDO_AEQUALIS_PTR(relatio->destinatio_id, dest1);

        /* Filtrare per genus "references" */
        filtratae = entitas_relationes_generis_capere(entitas, genus_referit, piscina);
        CREDO_NON_NIHIL(filtratae);
        CREDO_AEQUALIS_I32(xar_numerus(filtratae), I);

        relatio = (Relatio*)xar_obtinere(filtratae, ZEPHYRUM);
        CREDO_AEQUALIS_PTR(relatio->destinatio_id, dest3);
    }

    /* ==================================================
     * Probare notae
     * ================================================== */

    {
        Entitas* entitas;
         chorda* nota1;
         chorda* nota2;
         chorda* nota3;

        imprimere("\n--- Probans notae ---\n");

        entitas = entitas_creare(
            piscina,
            chorda_internare_ex_literis(intern, "task-1"),
            chorda_internare_ex_literis(intern, "task"));

        nota1 = chorda_internare_ex_literis(intern, "#urgent");
        nota2 = chorda_internare_ex_literis(intern, "#project::alpha");
        nota3 = chorda_internare_ex_literis(intern, "#project::beta");

        /* Addere notae */
        CREDO_VERUM(entitas_nota_addere(entitas, nota1));
        CREDO_VERUM(entitas_nota_addere(entitas, nota2));
        CREDO_VERUM(entitas_nota_addere(entitas, nota3));

        CREDO_AEQUALIS_I32(entitas_numerus_notarum(entitas), III);

        /* Verificare habet */
        CREDO_VERUM(entitas_nota_habet(entitas, nota1));
        CREDO_VERUM(entitas_nota_habet(entitas, nota2));
        CREDO_VERUM(entitas_nota_habet(entitas, nota3));

        /* Verificare non habet */
        CREDO_FALSUM(entitas_nota_habet(
            entitas,
            chorda_internare_ex_literis(intern, "#other")));
    }

    /* ==================================================
     * Probare duplicatae notae
     * ================================================== */

    {
        Entitas* entitas;
         chorda* nota;

        imprimere("\n--- Probans duplicatae notae ---\n");

        entitas = entitas_creare(
            piscina,
            chorda_internare_ex_literis(intern, "task-2"),
            chorda_internare_ex_literis(intern, "task"));

        nota = chorda_internare_ex_literis(intern, "#important");

        /* Addere notam bis */
        CREDO_VERUM(entitas_nota_addere(entitas, nota));
        CREDO_VERUM(entitas_nota_addere(entitas, nota));  /* Debet succedere */

        /* Debet adhuc habere tantum unam */
        CREDO_AEQUALIS_I32(entitas_numerus_notarum(entitas), I);
    }

    /* ==================================================
     * Probare nota cum praefixo
     * ================================================== */

    {
        Entitas* entitas;
         chorda* nota1;
         chorda* nota2;
         chorda* nota3;

        imprimere("\n--- Probans nota cum praefixo ---\n");

        entitas = entitas_creare(
            piscina,
            chorda_internare_ex_literis(intern, "task-3"),
            chorda_internare_ex_literis(intern, "task"));

        nota1 = chorda_internare_ex_literis(intern, "#project::alpha");
        nota2 = chorda_internare_ex_literis(intern, "#project::beta");
        nota3 = chorda_internare_ex_literis(intern, "#urgent");

        entitas_nota_addere(entitas, nota1);
        entitas_nota_addere(entitas, nota2);
        entitas_nota_addere(entitas, nota3);

        /* Verificare praefixum "#project::" */
        CREDO_VERUM(entitas_nota_cum_praefixo_habet(entitas, "#project::"));

        /* Verificare praefixum "#urgent" */
        CREDO_VERUM(entitas_nota_cum_praefixo_habet(entitas, "#urgent"));

        /* Verificare praefixum non existens */
        CREDO_FALSUM(entitas_nota_cum_praefixo_habet(entitas, "#work::"));

        /* Verificare praefixum vacuum */
        CREDO_VERUM(entitas_nota_cum_praefixo_habet(entitas, "#"));
    }

    /* ==================================================
     * Probare entitas_imprimere
     * ================================================== */

    {
        Entitas* entitas;

        imprimere("\n--- Probans entitas_imprimere ---\n");

        entitas = entitas_creare(
            piscina,
            chorda_internare_ex_literis(intern, "demo-entity"),
            chorda_internare_ex_literis(intern, "demo"));

        entitas_proprietas_ponere(
            entitas,
            chorda_internare_ex_literis(intern, "name"),
            chorda_internare_ex_literis(intern, "Demo Entity"));

        entitas_proprietas_ponere(
            entitas,
            chorda_internare_ex_literis(intern, "version"),
            chorda_internare_ex_literis(intern, "1.0"));

        entitas_relatio_addere(
            entitas,
            piscina,
            intern,
            chorda_internare_ex_literis(intern, "contains"),
            chorda_internare_ex_literis(intern, "child-1"));

        entitas_nota_addere(
            entitas,
            chorda_internare_ex_literis(intern, "#important"));

        entitas_nota_addere(
            entitas,
            chorda_internare_ex_literis(intern, "#project::demo"));

        /* Imprimere pro inspectione */
        entitas_imprimere(entitas);
    }

    /* ==================================================
     * Probare entitas complexa
     * ================================================== */

    {
          Entitas* entitas;
              i32  i;
           chorda* clavis;
           chorda* valor;
        character  buffer[XXXII];

        imprimere("\n--- Probans entitas complexa ---\n");

        entitas = entitas_creare(
            piscina,
            chorda_internare_ex_literis(intern, "complex-entity"),
            chorda_internare_ex_literis(intern, "complex"));

        /* Addere X proprietates */
        per (i = ZEPHYRUM; i < X; i++)
        {
            sprintf(buffer, "key-%d", i);
            clavis = chorda_internare_ex_literis(intern, buffer);

            sprintf(buffer, "value-%d", i);
            valor = chorda_internare_ex_literis(intern, buffer);

            CREDO_VERUM(entitas_proprietas_ponere(entitas, clavis, valor));
        }

        CREDO_AEQUALIS_I32(entitas_numerus_proprietatum(entitas), X);

        /* Addere V relationes */
        per (i = ZEPHYRUM; i < V; i++)
        {
            sprintf(buffer, "rel-%d", i);
            CREDO_NON_NIHIL(entitas_relatio_addere(
                entitas,
                piscina,
                intern,
                chorda_internare_ex_literis(intern, "link"),
                chorda_internare_ex_literis(intern, buffer)));
        }

        CREDO_AEQUALIS_I32(entitas_numerus_relationum(entitas), V);

        /* Addere V notae */
        per (i = ZEPHYRUM; i < V; i++)
        {
            sprintf(buffer, "#tag-%d", i);
            CREDO_VERUM(entitas_nota_addere(
                entitas,
                chorda_internare_ex_literis(intern, buffer)));
        }

        CREDO_AEQUALIS_I32(entitas_numerus_notarum(entitas), V);
    }

    /* ==================================================
     * Probare delere proprietates
     * ================================================== */

    {
        Entitas* entitas;
        chorda*  clavis1;
        chorda*  clavis2;
        chorda*  clavis3;
        chorda*  valor;

        imprimere("\n--- Probans delere proprietates ---\n");

        entitas = entitas_creare(
            piscina,
            chorda_internare_ex_literis(intern, "del-props"),
            chorda_internare_ex_literis(intern, "test"));

        clavis1 = chorda_internare_ex_literis(intern, "first");
        clavis2 = chorda_internare_ex_literis(intern, "second");
        clavis3 = chorda_internare_ex_literis(intern, "third");

        /* Addere tres proprietates */
        entitas_proprietas_ponere(entitas, clavis1, chorda_internare_ex_literis(intern, "1"));
        entitas_proprietas_ponere(entitas, clavis2, chorda_internare_ex_literis(intern, "2"));
        entitas_proprietas_ponere(entitas, clavis3, chorda_internare_ex_literis(intern, "3"));

        CREDO_AEQUALIS_I32(entitas_numerus_proprietatum(entitas), III);

        /* Delere secundam */
        CREDO_VERUM(entitas_proprietas_delere(entitas, clavis2));
        CREDO_AEQUALIS_I32(entitas_numerus_proprietatum(entitas), II);
        CREDO_FALSUM(entitas_proprietas_habet(entitas, clavis2));

        /* Prima et tertia adhuc existunt */
        CREDO_VERUM(entitas_proprietas_habet(entitas, clavis1));
        CREDO_VERUM(entitas_proprietas_habet(entitas, clavis3));

        /* Delere non existentem */
        CREDO_FALSUM(entitas_proprietas_delere(
            entitas,
            chorda_internare_ex_literis(intern, "nonexistent")));

        /* Delere primam */
        CREDO_VERUM(entitas_proprietas_delere(entitas, clavis1));
        CREDO_AEQUALIS_I32(entitas_numerus_proprietatum(entitas), I);

        /* Tantum tertia remanet */
        valor = entitas_proprietas_capere(entitas, clavis3);
        CREDO_NON_NIHIL(valor);
    }

    /* ==================================================
     * Probare delere relationes
     * ================================================== */

    {
        Entitas* entitas;
        Relatio* relatio1;
        Relatio* relatio2;
        Relatio* relatio3;
        chorda*  id1;
        chorda*  id2;
        chorda*  id3;

        imprimere("\n--- Probans delere relationes ---\n");

        entitas = entitas_creare(
            piscina,
            chorda_internare_ex_literis(intern, "del-rels"),
            chorda_internare_ex_literis(intern, "test"));

        /* Addere tres relationes */
        relatio1 = entitas_relatio_addere(
            entitas,
            piscina,
            intern,
            chorda_internare_ex_literis(intern, "link"),
            chorda_internare_ex_literis(intern, "dest1"));
        CREDO_NON_NIHIL(relatio1);

        relatio2 = entitas_relatio_addere(
            entitas,
            piscina,
            intern,
            chorda_internare_ex_literis(intern, "link"),
            chorda_internare_ex_literis(intern, "dest2"));
        CREDO_NON_NIHIL(relatio2);

        relatio3 = entitas_relatio_addere(
            entitas,
            piscina,
            intern,
            chorda_internare_ex_literis(intern, "link"),
            chorda_internare_ex_literis(intern, "dest3"));
        CREDO_NON_NIHIL(relatio3);

        /* Salvare IDs ante swap-and-pop (quod potest superscribere structuras) */
        id1 = relatio1->id;
        id2 = relatio2->id;
        id3 = relatio3->id;

        CREDO_AEQUALIS_I32(entitas_numerus_relationum(entitas), III);

        /* Delere secundam per ID */
        CREDO_VERUM(entitas_relatio_delere(entitas, id2));
        CREDO_AEQUALIS_I32(entitas_numerus_relationum(entitas), II);

        /* Verificare quod relatio2 non amplius invenitur */
        CREDO_NIHIL(entitas_relatio_capere(entitas, id2));

        /* Prima et tertia adhuc existunt */
        CREDO_NON_NIHIL(entitas_relatio_capere(entitas, id1));
        CREDO_NON_NIHIL(entitas_relatio_capere(entitas, id3));

        /* Delere ID non existens */
        CREDO_FALSUM(entitas_relatio_delere(
            entitas,
            chorda_internare_ex_literis(intern, "nonexistent-id")));

        /* Delere primam */
        CREDO_VERUM(entitas_relatio_delere(entitas, id1));
        CREDO_AEQUALIS_I32(entitas_numerus_relationum(entitas), I);

        /* Delere ultimam */
        CREDO_VERUM(entitas_relatio_delere(entitas, id3));
        CREDO_AEQUALIS_I32(entitas_numerus_relationum(entitas), ZEPHYRUM);
    }

    /* ==================================================
     * Probare delere notae
     * ================================================== */

    {
        Entitas* entitas;
        chorda*  nota1;
        chorda*  nota2;
        chorda*  nota3;

        imprimere("\n--- Probans delere notae ---\n");

        entitas = entitas_creare(
            piscina,
            chorda_internare_ex_literis(intern, "del-tags"),
            chorda_internare_ex_literis(intern, "test"));

        nota1 = chorda_internare_ex_literis(intern, "#tag1");
        nota2 = chorda_internare_ex_literis(intern, "#tag2");
        nota3 = chorda_internare_ex_literis(intern, "#tag3");

        /* Addere tres notae */
        entitas_nota_addere(entitas, nota1);
        entitas_nota_addere(entitas, nota2);
        entitas_nota_addere(entitas, nota3);

        CREDO_AEQUALIS_I32(entitas_numerus_notarum(entitas), III);

        /* Delere secundam */
        CREDO_VERUM(entitas_nota_delere(entitas, nota2));
        CREDO_AEQUALIS_I32(entitas_numerus_notarum(entitas), II);
        CREDO_FALSUM(entitas_nota_habet(entitas, nota2));

        /* Prima et tertia adhuc existunt */
        CREDO_VERUM(entitas_nota_habet(entitas, nota1));
        CREDO_VERUM(entitas_nota_habet(entitas, nota3));

        /* Delere non existentem */
        CREDO_FALSUM(entitas_nota_delere(
            entitas,
            chorda_internare_ex_literis(intern, "#nonexistent")));

        /* Delere omnes */
        CREDO_VERUM(entitas_nota_delere(entitas, nota1));
        CREDO_VERUM(entitas_nota_delere(entitas, nota3));
        CREDO_AEQUALIS_I32(entitas_numerus_notarum(entitas), ZEPHYRUM);
    }

    /* ==================================================
     * Probare entitas_titulum_capere
     * ================================================== */

    {
        Entitas* entitas;
        chorda*  titulum;

        imprimere("\n--- Probans entitas_titulum_capere ---\n");

        /* Casus 1: Entitas cum "name" proprietate */
        entitas = entitas_creare(
            piscina,
            chorda_internare_ex_literis(intern, "titulum-test-1"),
            chorda_internare_ex_literis(intern, "test"));

        entitas_proprietas_ponere(
            entitas,
            chorda_internare_ex_literis(intern, "name"),
            chorda_internare_ex_literis(intern, "My Entity Name"));

        titulum = entitas_titulum_capere(entitas);
        CREDO_NON_NIHIL(titulum);
        CREDO_VERUM(chorda_aequalis_literis(*titulum, "My Entity Name"));

        /* Casus 2: Entitas cum "title" proprietate (sine "name") */
        entitas = entitas_creare(
            piscina,
            chorda_internare_ex_literis(intern, "titulum-test-2"),
            chorda_internare_ex_literis(intern, "test"));

        entitas_proprietas_ponere(
            entitas,
            chorda_internare_ex_literis(intern, "title"),
            chorda_internare_ex_literis(intern, "My Entity Title"));

        titulum = entitas_titulum_capere(entitas);
        CREDO_NON_NIHIL(titulum);
        CREDO_VERUM(chorda_aequalis_literis(*titulum, "My Entity Title"));

        /* Casus 3: Entitas cum "name" et "title" - "name" habet prioritatem */
        entitas = entitas_creare(
            piscina,
            chorda_internare_ex_literis(intern, "titulum-test-3"),
            chorda_internare_ex_literis(intern, "test"));

        entitas_proprietas_ponere(
            entitas,
            chorda_internare_ex_literis(intern, "title"),
            chorda_internare_ex_literis(intern, "Title Value"));

        entitas_proprietas_ponere(
            entitas,
            chorda_internare_ex_literis(intern, "name"),
            chorda_internare_ex_literis(intern, "Name Value"));

        titulum = entitas_titulum_capere(entitas);
        CREDO_NON_NIHIL(titulum);
        CREDO_VERUM(chorda_aequalis_literis(*titulum, "Name Value"));

        /* Casus 4: Entitas sine "name" vel "title" - redde id */
        entitas = entitas_creare(
            piscina,
            chorda_internare_ex_literis(intern, "titulum-test-4"),
            chorda_internare_ex_literis(intern, "test"));

        entitas_proprietas_ponere(
            entitas,
            chorda_internare_ex_literis(intern, "other"),
            chorda_internare_ex_literis(intern, "Some Value"));

        titulum = entitas_titulum_capere(entitas);
        CREDO_NON_NIHIL(titulum);
        CREDO_VERUM(chorda_aequalis_literis(*titulum, "titulum-test-4"));

        /* Casus 5: Entitas NIHIL */
        titulum = entitas_titulum_capere(NIHIL);
        CREDO_NIHIL(titulum);
    }

    /* ==================================================
     * Probare proprietates typificatae (typed accessors)
     * ================================================== */

    {
        Entitas*    entitas;
        Proprietas* prop;
        s32         val_s32;
        s64         val_s64;
        f64         val_f64;
        b32         val_b32;

        imprimere("\n--- Probans proprietates typificatae ---\n");

        entitas = entitas_creare(
            piscina,
            chorda_internare_ex_literis(intern, "typed-props"),
            chorda_internare_ex_literis(intern, "test"));

        /* Probare s32 */
        entitas_proprietas_ponere(
            entitas,
            chorda_internare_ex_literis(intern, "count"),
            chorda_internare_ex_literis(intern, "42"));

        CREDO_VERUM(entitas_proprietas_capere_s32(
            entitas,
            chorda_internare_ex_literis(intern, "count"),
            &val_s32));
        CREDO_AEQUALIS_S32(val_s32, 42);

        /* Verificare caching */
        prop = entitas_proprietas_capere_plena(
            entitas,
            chorda_internare_ex_literis(intern, "count"));
        CREDO_NON_NIHIL(prop);
        CREDO_VERUM(prop->parsitus_validus);
        CREDO_AEQUALIS_I32((i32)prop->typus_literalis, (i32)TYPUS_S32);

        /* Probare s32 negativum */
        entitas_proprietas_ponere(
            entitas,
            chorda_internare_ex_literis(intern, "offset"),
            chorda_internare_ex_literis(intern, "-123"));

        CREDO_VERUM(entitas_proprietas_capere_s32(
            entitas,
            chorda_internare_ex_literis(intern, "offset"),
            &val_s32));
        CREDO_AEQUALIS_S32(val_s32, -123);

        /* Probare s32 invalidum */
        entitas_proprietas_ponere(
            entitas,
            chorda_internare_ex_literis(intern, "bad_num"),
            chorda_internare_ex_literis(intern, "not-a-number"));

        CREDO_FALSUM(entitas_proprietas_capere_s32(
            entitas,
            chorda_internare_ex_literis(intern, "bad_num"),
            &val_s32));

        /* Probare s64 */
        entitas_proprietas_ponere(
            entitas,
            chorda_internare_ex_literis(intern, "big_number"),
            chorda_internare_ex_literis(intern, "9223372036854775807"));

        CREDO_VERUM(entitas_proprietas_capere_s64(
            entitas,
            chorda_internare_ex_literis(intern, "big_number"),
            &val_s64));
        CREDO_AEQUALIS_S64(val_s64, 9223372036854775807LL);

        /* Probare s64 negativum */
        entitas_proprietas_ponere(
            entitas,
            chorda_internare_ex_literis(intern, "neg_big"),
            chorda_internare_ex_literis(intern, "-1000000000000"));

        CREDO_VERUM(entitas_proprietas_capere_s64(
            entitas,
            chorda_internare_ex_literis(intern, "neg_big"),
            &val_s64));
        CREDO_AEQUALIS_S64(val_s64, -1000000000000LL);

        /* Probare f64 */
        entitas_proprietas_ponere(
            entitas,
            chorda_internare_ex_literis(intern, "price"),
            chorda_internare_ex_literis(intern, "19.99"));

        CREDO_VERUM(entitas_proprietas_capere_f64(
            entitas,
            chorda_internare_ex_literis(intern, "price"),
            &val_f64));
        /* Comparare cum epsilon */
        CREDO_VERUM(val_f64 > 19.98 && val_f64 < 20.0);

        /* Probare f64 cum exponente */
        entitas_proprietas_ponere(
            entitas,
            chorda_internare_ex_literis(intern, "scientific"),
            chorda_internare_ex_literis(intern, "1.5e10"));

        CREDO_VERUM(entitas_proprietas_capere_f64(
            entitas,
            chorda_internare_ex_literis(intern, "scientific"),
            &val_f64));
        CREDO_VERUM(val_f64 > 1.4e10 && val_f64 < 1.6e10);

        /* Probare b32 - "true" */
        entitas_proprietas_ponere(
            entitas,
            chorda_internare_ex_literis(intern, "active"),
            chorda_internare_ex_literis(intern, "true"));

        CREDO_VERUM(entitas_proprietas_capere_b32(
            entitas,
            chorda_internare_ex_literis(intern, "active"),
            &val_b32));
        CREDO_VERUM(val_b32);

        /* Probare b32 - "false" */
        entitas_proprietas_ponere(
            entitas,
            chorda_internare_ex_literis(intern, "disabled"),
            chorda_internare_ex_literis(intern, "false"));

        CREDO_VERUM(entitas_proprietas_capere_b32(
            entitas,
            chorda_internare_ex_literis(intern, "disabled"),
            &val_b32));
        CREDO_FALSUM(val_b32);

        /* Probare b32 - "verum" */
        entitas_proprietas_ponere(
            entitas,
            chorda_internare_ex_literis(intern, "validus"),
            chorda_internare_ex_literis(intern, "verum"));

        CREDO_VERUM(entitas_proprietas_capere_b32(
            entitas,
            chorda_internare_ex_literis(intern, "validus"),
            &val_b32));
        CREDO_VERUM(val_b32);

        /* Probare b32 - "0" */
        entitas_proprietas_ponere(
            entitas,
            chorda_internare_ex_literis(intern, "flag"),
            chorda_internare_ex_literis(intern, "0"));

        CREDO_VERUM(entitas_proprietas_capere_b32(
            entitas,
            chorda_internare_ex_literis(intern, "flag"),
            &val_b32));
        CREDO_FALSUM(val_b32);

        /* Probare b32 invalidum */
        entitas_proprietas_ponere(
            entitas,
            chorda_internare_ex_literis(intern, "bad_bool"),
            chorda_internare_ex_literis(intern, "maybe"));

        CREDO_FALSUM(entitas_proprietas_capere_b32(
            entitas,
            chorda_internare_ex_literis(intern, "bad_bool"),
            &val_b32));

        /* Probare tempus (milliseconds) */
        entitas_proprietas_ponere(
            entitas,
            chorda_internare_ex_literis(intern, "timestamp"),
            chorda_internare_ex_literis(intern, "1701388800000"));

        CREDO_VERUM(entitas_proprietas_capere_tempus(
            entitas,
            chorda_internare_ex_literis(intern, "timestamp"),
            &val_s64));
        CREDO_AEQUALIS_S64(val_s64, 1701388800000LL);

        /* Probare proprietas non existens */
        CREDO_FALSUM(entitas_proprietas_capere_s32(
            entitas,
            chorda_internare_ex_literis(intern, "nonexistent"),
            &val_s32));

        /* Probare renovatio proprietatis resets cache */
        entitas_proprietas_ponere(
            entitas,
            chorda_internare_ex_literis(intern, "count"),
            chorda_internare_ex_literis(intern, "100"));

        prop = entitas_proprietas_capere_plena(
            entitas,
            chorda_internare_ex_literis(intern, "count"));
        CREDO_NON_NIHIL(prop);
        CREDO_FALSUM(prop->parsitus_validus);  /* Reset post renovatio */

        /* Re-parse debet laborare */
        CREDO_VERUM(entitas_proprietas_capere_s32(
            entitas,
            chorda_internare_ex_literis(intern, "count"),
            &val_s32));
        CREDO_AEQUALIS_S32(val_s32, 100);
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

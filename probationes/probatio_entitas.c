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
        CREDO_VERUM(entitas_proprietas_addere(entitas, clavis_nomen, valor_nomen));

        clavis_color = chorda_internare_ex_literis(intern, "color");
        valor_color  = chorda_internare_ex_literis(intern, "blue");
        CREDO_VERUM(entitas_proprietas_addere(entitas, clavis_color, valor_color));

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
        CREDO_VERUM(entitas_proprietas_addere(entitas, clavis, valor1));
        CREDO_AEQUALIS_I32(entitas_numerus_proprietatum(entitas), I);

        resultus = entitas_proprietas_capere(entitas, clavis);
        CREDO_AEQUALIS_PTR(resultus, valor1);

        /* Renovare proprietatem */
        CREDO_VERUM(entitas_proprietas_addere(entitas, clavis, valor2));
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

        entitas_proprietas_addere(
            entitas,
            chorda_internare_ex_literis(intern, "name"),
            chorda_internare_ex_literis(intern, "Demo Entity"));

        entitas_proprietas_addere(
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

            CREDO_VERUM(entitas_proprietas_addere(entitas, clavis, valor));
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
        entitas_proprietas_addere(entitas, clavis1, chorda_internare_ex_literis(intern, "1"));
        entitas_proprietas_addere(entitas, clavis2, chorda_internare_ex_literis(intern, "2"));
        entitas_proprietas_addere(entitas, clavis3, chorda_internare_ex_literis(intern, "3"));

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

/* probatio_quaestiones_lectio.c - lector GENERATUS contra corpus
 * verum (differentia nativitatis generatoris canon_coquere)
 *
 * Lector generatus (include/quaestiones_lectio.h) plagulam veram
 * silva/quaestiones.stml legit; assertiones contra facta corporis
 * NOTA (IV quaestiones, tituli, gradus, parametra) - non contra
 * lectorem manu scriptum in processu (silva_quaestiones.c
 * tabularium silvae poscit et selectores COMPILAT - officium
 * aliud), sed contra idem corpus quod ille legit. Lex corporis:
 * instrumentum in copia vera curre antequam credis.
 */
#include "latina.h"
#include "quaestiones_lectio.h"
#include "credo.h"
#include "filum.h"
#include <stdio.h>

s32 principale (vacuum)
{
                     b32  praeteritus;
                Piscina*  piscina;
    InternamentumChorda*  intern;

    piscina = piscina_generare_dynamicum(
        "probatio_quaestiones_lectio", 262144);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    intern = internamentum_creare(piscina);
    credo_aperire(piscina);


    /* ========================================================
     * PROBARE: corpus verum per lectorem generatum
     * ======================================================== */

    {
        chorda         fons;
        chorda         causa;
        QsQuaestiones* bib;

        imprimere("\n--- Probans corpus verum (generatum) ---\n");

        fons = filum_legere_totum("silva/quaestiones.stml",
                                  piscina);
        si (fons.mensura == ZEPHYRUM)
        {
            imprimere("FRACTA: silva/quaestiones.stml legi nequit "
                      "(currendum e radice arboris)\n");
            CREDO_VERUM (fons.mensura > ZEPHYRUM);
        }
        alioquin
        {
            bib = qs_quaestiones_legere(fons, piscina, intern,
                                        &causa);
            CREDO_NON_NIHIL (bib);
            CREDO_AEQUALIS_I32 ((i32)xar_numerus(bib->quaestio),
                                IV);

            /* prima: vocantes-continentes */
            {
                QsQuaestio* q;

                q = *(QsQuaestio**)xar_obtinere(bib->quaestio,
                                                ZEPHYRUM);
                CREDO_NON_NIHIL (q->titulus);
                CREDO_CHORDA_AEQUALIS_LITERIS (*q->titulus,
                    "vocantes-continentes");
                CREDO_AEQUALIS_I32 ((i32)q->gradus,
                    (i32)QS_QUAESTIO_GRADUS_OCULI);
                CREDO_NON_NIHIL (q->selector);
                CREDO_CHORDA_AEQUALIS_LITERIS (q->selector->textus,
                    "definitio-functionis:vocat($functio)");
                CREDO_NON_NIHIL (q->causa);
                CREDO_CHORDA_NON_VACUA (q->causa->textus);
                CREDO_AEQUALIS_I32 (
                    (i32)xar_numerus(q->parametrum), I);
                {
                    QsParametrum* p;

                    p = *(QsParametrum**)xar_obtinere(
                        q->parametrum, ZEPHYRUM);
                    CREDO_NON_NIHIL (p->titulus);
                    CREDO_CHORDA_AEQUALIS_LITERIS (*p->titulus,
                                                   "functio");
                }

                /* profunditas hybrida: nodus + linea gratis */
                CREDO_NON_NIHIL (q->nodus);
                CREDO_AEQUALIS_I32 (q->nodus->linea, XVII);
            }

            /* quarta: invarians sine parametris */
            {
                QsQuaestio* q;

                q = *(QsQuaestio**)xar_obtinere(bib->quaestio,
                                                III);
                CREDO_NON_NIHIL (q->titulus);
                CREDO_CHORDA_AEQUALIS_LITERIS (*q->titulus,
                    "exsequi-in-pulsatione");
                CREDO_AEQUALIS_I32 ((i32)q->gradus,
                    (i32)QS_QUAESTIO_GRADUS_INVARIANS);
                CREDO_AEQUALIS_I32 (
                    (i32)xar_numerus(q->parametrum), ZEPHYRUM);
                CREDO_NON_NIHIL (q->selector);
                CREDO_CHORDA_CONTINET (q->selector->textus,
                    chorda_ex_literis("processus_exsequi",
                                      piscina));
            }
        }
    }


    /* ========================================================
     * PROBARE: margines - absentia et ignota
     * ======================================================== */

    {
        chorda         causa;
        QsQuaestiones* bib;

        imprimere("\n--- Probans margines lectoris ---\n");

        /* gradus absens -> ABSENS; selector absens -> NIHIL;
         * elementum ignotum praetermissum (lector folium) */
        bib = qs_quaestiones_legere(chorda_ex_literis(
            "<quaestiones>\n"
            "  <quaestio titulus=\"nuda\">\n"
            "    <ignotum/>\n"
            "    <causa>c</causa>\n"
            "  </quaestio>\n"
            "</quaestiones>\n", piscina), piscina, intern, &causa);
        CREDO_NON_NIHIL (bib);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(bib->quaestio), I);
        {
            QsQuaestio* q;

            q = *(QsQuaestio**)xar_obtinere(bib->quaestio,
                                            ZEPHYRUM);
            CREDO_AEQUALIS_I32 ((i32)q->gradus,
                (i32)QS_QUAESTIO_GRADUS_ABSENS);
            CREDO_NIHIL (q->selector);
            CREDO_NON_NIHIL (q->causa);
        }

        /* radix aliena -> NIHIL + causa */
        bib = qs_quaestiones_legere(chorda_ex_literis("<alia/>",
            piscina), piscina, intern, &causa);
        CREDO_NIHIL (bib);
        CREDO_CHORDA_NON_VACUA (causa);
    }


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

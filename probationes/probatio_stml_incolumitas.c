/* probatio_stml_incolumitas.c - Inputa male formata parsatorem NEC
 * occidere NEC impedire debent.
 *
 * Omnia haec ante reparationes 01M16YP7W1 (gyri), 01M16Z03YE (ruina
 * commenti/processionis) et 01M171YAEP (ruina tituli NIHIL) aut
 * pendebant aut ruebant. Assertiones per FURCAM currunt (credo par.
 * 7.5.0), ergo regressio suitem non trahit nec cursorem impedit.
 *
 * Spec: project-specs/stml-instrumentum-spec.md par. 7.5.1-7.5.3
 */
#include "postulata_posix.h"
#include "latina.h"
#include "credo.h"
#include "stml.h"
#include "piscina.h"
#include "internamentum.h"
#include <stdio.h>


/* Parsare et abicere. Fructus non interest - sola quaestio est an
 * stml_legere REDEAT omnino. */
vacuum
probatio_stml_incolumitas_parsare (
    constans character* fons);

vacuum
probatio_stml_incolumitas_parsare (
    constans character* fons)
{
                Piscina* p;
    InternamentumChorda* it;

    p = piscina_generare_dynamicum("incolumitas", 1048576);
    si (!p)
    {
        redde;
    }
    it = internamentum_creare(p);
    (vacuum)stml_legere_ex_literis(fons, p, it);
    piscina_destruere(p);
}

s32
principale (vacuum)
{
         b32  praeteritus;
     Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_stml_incolumitas",
                                         65536);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ==================================================
     * GYRI porta I: contentum crudum non clausum
     * ================================================== */

    imprimere("\n--- Gyri I: contentum crudum non clausum ---\n");

    CREDO_NON_PENDET(probatio_stml_incolumitas_parsare("<x!>a"), 3000);
    CREDO_NON_PENDET(probatio_stml_incolumitas_parsare("<x!>a</y>"),
                     3000);
    CREDO_NON_PENDET(probatio_stml_incolumitas_parsare("<x!>a</>"),
                     3000);
    CREDO_NON_PENDET(probatio_stml_incolumitas_parsare("<x!>a</#>"),
                     3000);


    /* ==================================================
     * GYRI porta II: lapsus '<!'
     *
     * Haec non recta pendent: ut tagus crudus tituli VACUI male
     * lexantur, deinde in portam I cadunt.
     * ================================================== */

    imprimere("\n--- Gyri II: lapsus '<!' ---\n");

    CREDO_NON_PENDET(probatio_stml_incolumitas_parsare("<!x>"), 3000);
    CREDO_NON_PENDET(probatio_stml_incolumitas_parsare("<!!>"), 3000);
    CREDO_NON_PENDET(
        probatio_stml_incolumitas_parsare("<!doctype html>"), 3000);
    CREDO_NON_PENDET(
        probatio_stml_incolumitas_parsare("<![CDATA[x]]>"), 3000);
    CREDO_NON_PENDET(
        probatio_stml_incolumitas_parsare("<!ENTITY a \"b\">"), 3000);


    /* ==================================================
     * CUSTODES: haec parsare DEBENT, ante et post
     * ================================================== */

    imprimere("\n--- Custodes gyrorum ---\n");

    CREDO_NON_PENDET(
        probatio_stml_incolumitas_parsare("<!DOCTYPE html>"), 3000);
    CREDO_NON_PENDET(probatio_stml_incolumitas_parsare("<!-- x -->"),
                     3000);
    CREDO_NON_PENDET(probatio_stml_incolumitas_parsare("<x!>a</x>"),
                     3000);


    /* ==================================================
     * RUINAE: commentum / processio non clausa
     *
     * Valor SOLUM intra ramum terminatoris assignabatur, ergo ad
     * EOF locale stacki NON INITIALIZATUM reddebatur, quod
     * chorda_internare dereferenciabat. V octeti sufficiebant.
     * ================================================== */

    imprimere("\n--- Ruinae: commentum / processio non clausa ---\n");

    CREDO_NON_RUIT(probatio_stml_incolumitas_parsare("<!--x"));
    CREDO_NON_RUIT(probatio_stml_incolumitas_parsare("<?x"));
    CREDO_NON_RUIT(
        probatio_stml_incolumitas_parsare("<r><!-- x</r>"));
    CREDO_NON_RUIT(
        probatio_stml_incolumitas_parsare("<r><?php echo 1; </r>"));


    /* ==================================================
     * CUSTODES: haec CLAUSA sunt et parsare DEBENT
     *
     * Terminator ubicumque sedet invenitur; lineolae superfluae
     * contentum sunt. '<!DOCTYPE x' non clausum iam transibat -
     * _tok_legere_doctype extra ansam assignat, forma recta quam
     * duo alia sequi debent.
     * ================================================== */

    imprimere("\n--- Custodes commentorum ---\n");

    CREDO_NON_RUIT(probatio_stml_incolumitas_parsare("<!-- x -->"));
    CREDO_NON_RUIT(probatio_stml_incolumitas_parsare("<!-- x --->"));
    CREDO_NON_RUIT(probatio_stml_incolumitas_parsare("<!-- x ---->"));
    CREDO_NON_RUIT(
        probatio_stml_incolumitas_parsare("<!-- a -- b -->"));
    CREDO_NON_RUIT(probatio_stml_incolumitas_parsare("<!---->"));
    CREDO_NON_RUIT(probatio_stml_incolumitas_parsare("<r><?x?></r>"));
    CREDO_NON_RUIT(probatio_stml_incolumitas_parsare("<!DOCTYPE x"));


    /* ==================================================
     * RUINAE: tagum apertum sine nomine
     *
     * '<' spatio sequente nomen VACUUM dat; chorda_internare vacuae
     * NIHIL reddit; comparatio clausurae *titulus_ptr sine custode
     * dereferenciabat. Prosa vulgaris hoc producit.
     * ================================================== */

    imprimere("\n--- Ruinae: tagum sine nomine ---\n");

    CREDO_NON_RUIT(probatio_stml_incolumitas_parsare("<p>a < b</p>"));
    CREDO_NON_RUIT(probatio_stml_incolumitas_parsare("<p>x < 10</p>"));
    CREDO_NON_RUIT(probatio_stml_incolumitas_parsare("<p>a < </p>"));


    /* ==================================================
     * CUSTODES: formae anonymae LEGITIMAE
     *
     * Hae ante et post transire DEBENT. '</>' praesertim: per
     * _tok_legere_tag cadit et nomen VACUUM eius est ipsum signum
     * clausurae anonymae - recusatio ad tempus lexationis eam
     * frangeret (IX usus in probatio_stml_macros.c solo).
     * ================================================== */

    imprimere("\n--- Custodes formarum anonymarum ---\n");

    CREDO_NON_RUIT(
        probatio_stml_incolumitas_parsare("<r><a>x</></r>"));
    CREDO_NON_RUIT(probatio_stml_incolumitas_parsare("<r><>a</></r>"));
    CREDO_NON_RUIT(
        probatio_stml_incolumitas_parsare("<r><(> <a/></r>"));
    CREDO_NON_RUIT(probatio_stml_incolumitas_parsare(
        "<r><tag><>a</><>b</></tag></r>"));
    CREDO_NON_RUIT(probatio_stml_incolumitas_parsare("<p>a <b</p>"));
    CREDO_NON_RUIT(
        probatio_stml_incolumitas_parsare("<p>5 &lt; 3</p>"));


    /* ==================================================
     * Compendium
     * ================================================== */

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

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


/* Statum parsationis reddere (ZEPHYRUM si successit). Pro sectione
 * recusationum, ubi non RUINA sed IUDICIUM probatur - haec inputa
 * iam non ruunt, ergo furca hic supervacua est. */
s32
probatio_stml_incolumitas_status (
    constans character* fons);

s32
probatio_stml_incolumitas_status (
    constans character* fons)
{
                 Piscina* p;
     InternamentumChorda* it;
            StmlResultus  r;
                     s32  status;

    p = piscina_generare_dynamicum("incolumitas_status", 1048576);
    si (!p)
    {
        redde -I;
    }
    it      = internamentum_creare(p);
    r       = stml_legere_ex_literis(fons, p, it);
    status  = r.successus ? (s32)ZEPHYRUM : (s32)r.status;
    piscina_destruere(p);
    redde status;
}


    /* ==================================================
     * GYRI porta I: contentum crudum non clausum
     * ================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    (vacuum)c;
    CREDO_NON_PENDET(probatio_stml_incolumitas_parsare("<x!>a"), 3000);
    CREDO_NON_PENDET(probatio_stml_incolumitas_parsare("<x!>a</y>"),
                     3000);
    CREDO_NON_PENDET(probatio_stml_incolumitas_parsare("<x!>a</>"),
                     3000);
    CREDO_NON_PENDET(probatio_stml_incolumitas_parsare("<x!>a</#>"),
                     3000);
}


    /* ==================================================
     * GYRI porta II: lapsus '<!'
     *
     * Haec non recta pendent: ut tagus crudus tituli VACUI male
     * lexantur, deinde in portam I cadunt.
     * ================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    (vacuum)c;
    CREDO_NON_PENDET(probatio_stml_incolumitas_parsare("<!x>"), 3000);
    CREDO_NON_PENDET(probatio_stml_incolumitas_parsare("<!!>"), 3000);
    CREDO_NON_PENDET(
        probatio_stml_incolumitas_parsare("<!doctype html>"), 3000);
    CREDO_NON_PENDET(
        probatio_stml_incolumitas_parsare("<![CDATA[x]]>"), 3000);
    CREDO_NON_PENDET(
        probatio_stml_incolumitas_parsare("<!ENTITY a \"b\">"), 3000);
}


    /* ==================================================
     * CUSTODES: haec parsare DEBENT, ante et post
     * ================================================== */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    (vacuum)c;
    CREDO_NON_PENDET(
        probatio_stml_incolumitas_parsare("<!DOCTYPE html>"), 3000);
    CREDO_NON_PENDET(probatio_stml_incolumitas_parsare("<!-- x -->"),
                     3000);
    CREDO_NON_PENDET(probatio_stml_incolumitas_parsare("<x!>a</x>"),
                     3000);
}


    /* ==================================================
     * RUINAE: commentum / processio non clausa
     *
     * Valor SOLUM intra ramum terminatoris assignabatur, ergo ad
     * EOF locale stacki NON INITIALIZATUM reddebatur, quod
     * chorda_internare dereferenciabat. V octeti sufficiebant.
     * ================================================== */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
    (vacuum)c;
    CREDO_NON_RUIT(probatio_stml_incolumitas_parsare("<!--x"));
    CREDO_NON_RUIT(probatio_stml_incolumitas_parsare("<?x"));
    CREDO_NON_RUIT(
        probatio_stml_incolumitas_parsare("<r><!-- x</r>"));
    CREDO_NON_RUIT(
        probatio_stml_incolumitas_parsare("<r><?php echo 1; </r>"));
}


    /* ==================================================
     * CUSTODES: haec CLAUSA sunt et parsare DEBENT
     *
     * Terminator ubicumque sedet invenitur; lineolae superfluae
     * contentum sunt. '<!DOCTYPE x' non clausum iam transibat -
     * _tok_legere_doctype extra ansam assignat, forma recta quam
     * duo alia sequi debent.
     * ================================================== */

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
    (vacuum)c;
    CREDO_NON_RUIT(probatio_stml_incolumitas_parsare("<!-- x -->"));
    CREDO_NON_RUIT(probatio_stml_incolumitas_parsare("<!-- x --->"));
    CREDO_NON_RUIT(probatio_stml_incolumitas_parsare("<!-- x ---->"));
    CREDO_NON_RUIT(
        probatio_stml_incolumitas_parsare("<!-- a -- b -->"));
    CREDO_NON_RUIT(probatio_stml_incolumitas_parsare("<!---->"));
    CREDO_NON_RUIT(probatio_stml_incolumitas_parsare("<r><?x?></r>"));
    CREDO_NON_RUIT(probatio_stml_incolumitas_parsare("<!DOCTYPE x"));
}


    /* ==================================================
     * RUINAE: tagum apertum sine nomine
     *
     * '<' spatio sequente nomen VACUUM dat; chorda_internare vacuae
     * NIHIL reddit; comparatio clausurae *titulus_ptr sine custode
     * dereferenciabat. Prosa vulgaris hoc producit.
     * ================================================== */

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
    (vacuum)c;
    CREDO_NON_RUIT(probatio_stml_incolumitas_parsare("<p>a < b</p>"));
    CREDO_NON_RUIT(probatio_stml_incolumitas_parsare("<p>x < 10</p>"));
    CREDO_NON_RUIT(probatio_stml_incolumitas_parsare("<p>a < </p>"));
}


    /* ==================================================
     * CUSTODES: formae anonymae LEGITIMAE
     *
     * Hae ante et post transire DEBENT. '</>' praesertim: per
     * _tok_legere_tag cadit et nomen VACUUM eius est ipsum signum
     * clausurae anonymae - recusatio ad tempus lexationis eam
     * frangeret (IX usus in probatio_stml_macros.c solo).
     * ================================================== */

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
    (vacuum)c;
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
}


    /* ==================================================
     * RECUSATIONES: constructio NON CLAUSA erratum est
     *
     * Elementum ordinarium non clausum ('<a>x') TAG_NON_CLAUSUM iam
     * semper dedit. Crudum, commentum, processio, doctypus autem
     * TACITE succedebant et scriptor terminatorem INVENIEBAT
     * ('<!--x' -> '<!--x-->'). Periculosum: qui '-->' inter
     * edendum omittit, formatorem commentum pro se CLAUDENTEM
     * accipit, quidquid sequebatur devoratum.
     *
     * Nunc omnia quattuor consentiunt.
     * ================================================== */

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
    (vacuum)c;
    /* norma iam exsistens - custos ne mutetur */
    CREDO_AEQUALIS_S32(
        probatio_stml_incolumitas_status("<a>x"),
        (s32)STML_ERROR_TAG_NON_CLAUSUM);

    /* crudum */
    CREDO_AEQUALIS_S32(
        probatio_stml_incolumitas_status("<x!>a"),
        (s32)STML_ERROR_TAG_NON_CLAUSUM);

    /* commentum */
    CREDO_AEQUALIS_S32(
        probatio_stml_incolumitas_status("<!--x"),
        (s32)STML_ERROR_TAG_NON_CLAUSUM);

    /* processio */
    CREDO_AEQUALIS_S32(
        probatio_stml_incolumitas_status("<?x"),
        (s32)STML_ERROR_TAG_NON_CLAUSUM);

    /* doctypus */
    CREDO_AEQUALIS_S32(
        probatio_stml_incolumitas_status("<!DOCTYPE x"),
        (s32)STML_ERROR_TAG_NON_CLAUSUM);
}


    /* ==================================================
     * CUSTODES: formae CLAUSAE succedere DEBENT
     * ================================================== */

interior vacuum
_sectio_nona (
    CredoContextus* c)
{
    (vacuum)c;
    CREDO_AEQUALIS_S32(
        probatio_stml_incolumitas_status("<a>x</a>"), ZEPHYRUM);
    CREDO_AEQUALIS_S32(
        probatio_stml_incolumitas_status("<x!>a</x>"), ZEPHYRUM);
    CREDO_AEQUALIS_S32(
        probatio_stml_incolumitas_status("<!-- x -->"), ZEPHYRUM);
    CREDO_AEQUALIS_S32(
        probatio_stml_incolumitas_status("<r><?x?></r>"), ZEPHYRUM);
    CREDO_AEQUALIS_S32(
        probatio_stml_incolumitas_status("<!DOCTYPE html>"), ZEPHYRUM);
    CREDO_AEQUALIS_S32(
        probatio_stml_incolumitas_status("<x!>a</x><y/>"), ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "Gyri I: contentum crudum non clausum", _sectio_prima, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "Gyri II: lapsus '<!'", _sectio_secunda, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "Custodes gyrorum", _sectio_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Ruinae: commentum / processio non clausa", _sectio_quarta, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "Custodes commentorum", _sectio_quinta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "Ruinae: tagum sine nomine", _sectio_sexta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "Custodes formarum anonymarum", _sectio_septima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "Recusationes: non clausum = erratum", _sectio_octava, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "Custodes recusationum", _sectio_nona, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("stml_incolumitas", SECTIONES);
}

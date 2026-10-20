/* probatio_exemplar.c - Probationes Bibliothecae Exemplarium */

#include "latina.h"
#include "exemplar.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>


/* ==================================================
 * Probationes: Characteres Literales
 * ================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
            Piscina* piscina;
           Exemplar* ex;
    ExemplarFructus  f;

    piscina = c->piscina;

    /* Congruentia exacta */
    ex = exemplar_compilare(chorda_ex_literis("hello", piscina), NIHIL,
        piscina);
    CREDO_NON_NIHIL(ex);

    CREDO_VERUM(exemplar_congruit_totum(ex, chorda_ex_literis("hello",
        piscina)));
    CREDO_FALSUM(exemplar_congruit_totum(ex, chorda_ex_literis("hello!",
        piscina)));
    CREDO_FALSUM(exemplar_congruit_totum(ex, chorda_ex_literis("hell",
        piscina)));

    /* Invenire in chorda */
    f = exemplar_invenire(ex, chorda_ex_literis("say hello world",
        piscina));
    CREDO_VERUM(f.congruit);
    CREDO_AEQUALIS_I32(f.initium, 4);
    CREDO_AEQUALIS_I32(f.longitudo, 5);
}


/* ==================================================
 * Probationes: Punctum (.)
 * ================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
     Piscina* piscina;
    Exemplar* ex;

    piscina = c->piscina;

    ex = exemplar_compilare(chorda_ex_literis("a.c", piscina), NIHIL,
        piscina);
    CREDO_NON_NIHIL(ex);

    CREDO_VERUM(exemplar_congruit_totum(ex, chorda_ex_literis("abc",
        piscina)));
    CREDO_VERUM(exemplar_congruit_totum(ex, chorda_ex_literis("a1c",
        piscina)));
    CREDO_VERUM(exemplar_congruit_totum(ex, chorda_ex_literis("a c",
        piscina)));
    CREDO_FALSUM(exemplar_congruit_totum(ex, chorda_ex_literis("ac",
        piscina)));
    CREDO_FALSUM(exemplar_congruit_totum(ex, chorda_ex_literis("abbc",
        piscina)));

    /* Punctum non congruit cum newline */
    CREDO_FALSUM(exemplar_congruit_totum(ex, chorda_ex_literis("a\nc",
        piscina)));
}


/* ==================================================
 * Probationes: Asteriscus (*)
 * ================================================== */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
     Piscina* piscina;
    Exemplar* ex;

    piscina = c->piscina;

    ex = exemplar_compilare(chorda_ex_literis("ab*c", piscina), NIHIL,
        piscina);
    CREDO_NON_NIHIL(ex);

    CREDO_VERUM(exemplar_congruit_totum(ex, chorda_ex_literis("ac",
        piscina)));
    CREDO_VERUM(exemplar_congruit_totum(ex, chorda_ex_literis("abc",
        piscina)));
    CREDO_VERUM(exemplar_congruit_totum(ex, chorda_ex_literis("abbc",
        piscina)));
    CREDO_VERUM(exemplar_congruit_totum(ex, chorda_ex_literis("abbbc",
        piscina)));
    CREDO_FALSUM(exemplar_congruit_totum(ex, chorda_ex_literis("adc",
        piscina)));

    /* .* pro quolibet */
    ex = exemplar_compilare(chorda_ex_literis("a.*c", piscina), NIHIL,
        piscina);
    CREDO_NON_NIHIL(ex);

    CREDO_VERUM(exemplar_congruit_totum(ex, chorda_ex_literis("ac",
        piscina)));
    CREDO_VERUM(exemplar_congruit_totum(ex, chorda_ex_literis("abc",
        piscina)));
    CREDO_VERUM(exemplar_congruit_totum(ex, chorda_ex_literis("aXYZc",
        piscina)));
}


/* ==================================================
 * Probationes: Plus (+)
 * ================================================== */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
     Piscina* piscina;
    Exemplar* ex;

    piscina = c->piscina;

    ex = exemplar_compilare(chorda_ex_literis("ab+c", piscina), NIHIL,
        piscina);
    CREDO_NON_NIHIL(ex);

    CREDO_FALSUM(exemplar_congruit_totum(ex, chorda_ex_literis("ac",
        piscina)));
    CREDO_VERUM(exemplar_congruit_totum(ex, chorda_ex_literis("abc",
        piscina)));
    CREDO_VERUM(exemplar_congruit_totum(ex, chorda_ex_literis("abbc",
        piscina)));
    CREDO_VERUM(exemplar_congruit_totum(ex, chorda_ex_literis("abbbc",
        piscina)));
}


/* ==================================================
 * Probationes: Quaestionis (?)
 * ================================================== */

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
     Piscina* piscina;
    Exemplar* ex;

    piscina = c->piscina;

    ex = exemplar_compilare(chorda_ex_literis("colou?r", piscina),
        NIHIL, piscina);
    CREDO_NON_NIHIL(ex);

    CREDO_VERUM(exemplar_congruit_totum(ex, chorda_ex_literis("color",
        piscina)));
    CREDO_VERUM(exemplar_congruit_totum(ex, chorda_ex_literis("colour",
        piscina)));
    CREDO_FALSUM(exemplar_congruit_totum(ex,
        chorda_ex_literis("colouur", piscina)));
}


/* ==================================================
 * Probationes: Classes Characterum
 * ================================================== */

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
     Piscina* piscina;
    Exemplar* ex;

    piscina = c->piscina;

    /* Classis simplex */
    ex = exemplar_compilare(chorda_ex_literis("[aeiou]", piscina),
        NIHIL, piscina);
    CREDO_NON_NIHIL(ex);

    CREDO_VERUM(exemplar_congruit_totum(ex, chorda_ex_literis("a",
        piscina)));
    CREDO_VERUM(exemplar_congruit_totum(ex, chorda_ex_literis("e",
        piscina)));
    CREDO_FALSUM(exemplar_congruit_totum(ex, chorda_ex_literis("b",
        piscina)));
    CREDO_FALSUM(exemplar_congruit_totum(ex, chorda_ex_literis("ae",
        piscina)));

    /* Intervallum */
    ex = exemplar_compilare(chorda_ex_literis("[a-z]", piscina), NIHIL,
        piscina);
    CREDO_NON_NIHIL(ex);

    CREDO_VERUM(exemplar_congruit_totum(ex, chorda_ex_literis("a",
        piscina)));
    CREDO_VERUM(exemplar_congruit_totum(ex, chorda_ex_literis("m",
        piscina)));
    CREDO_VERUM(exemplar_congruit_totum(ex, chorda_ex_literis("z",
        piscina)));
    CREDO_FALSUM(exemplar_congruit_totum(ex, chorda_ex_literis("A",
        piscina)));
    CREDO_FALSUM(exemplar_congruit_totum(ex, chorda_ex_literis("0",
        piscina)));

    /* Intervalla multiplicia */
    ex = exemplar_compilare(chorda_ex_literis("[A-Za-z0-9]", piscina),
        NIHIL, piscina);
    CREDO_NON_NIHIL(ex);

    CREDO_VERUM(exemplar_congruit_totum(ex, chorda_ex_literis("A",
        piscina)));
    CREDO_VERUM(exemplar_congruit_totum(ex, chorda_ex_literis("z",
        piscina)));
    CREDO_VERUM(exemplar_congruit_totum(ex, chorda_ex_literis("5",
        piscina)));
    CREDO_FALSUM(exemplar_congruit_totum(ex, chorda_ex_literis("!",
        piscina)));
}


/* ==================================================
 * Probationes: Classis Negata
 * ================================================== */

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
     Piscina* piscina;
    Exemplar* ex;

    piscina = c->piscina;

    ex = exemplar_compilare(chorda_ex_literis("[^0-9]", piscina), NIHIL,
        piscina);
    CREDO_NON_NIHIL(ex);

    CREDO_FALSUM(exemplar_congruit_totum(ex, chorda_ex_literis("5",
        piscina)));
    CREDO_VERUM(exemplar_congruit_totum(ex, chorda_ex_literis("a",
        piscina)));
    CREDO_VERUM(exemplar_congruit_totum(ex, chorda_ex_literis("!",
        piscina)));
}


/* ==================================================
 * Probationes: Ancorae
 * ================================================== */

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
            Piscina* piscina;
           Exemplar* ex;
    ExemplarFructus  f;

    piscina = c->piscina;

    /* Ancora initii */
    ex = exemplar_compilare(chorda_ex_literis("^hello", piscina), NIHIL,
        piscina);
    CREDO_NON_NIHIL(ex);

    f = exemplar_invenire(ex, chorda_ex_literis("hello world",
        piscina));
    CREDO_VERUM(f.congruit);
    CREDO_AEQUALIS_I32(f.initium, 0);

    f = exemplar_invenire(ex, chorda_ex_literis("say hello", piscina));
    CREDO_FALSUM(f.congruit);

    /* Ancora finis */
    ex = exemplar_compilare(chorda_ex_literis("world$", piscina), NIHIL,
        piscina);
    CREDO_NON_NIHIL(ex);

    f = exemplar_invenire(ex, chorda_ex_literis("hello world",
        piscina));
    CREDO_VERUM(f.congruit);
    CREDO_AEQUALIS_I32(f.initium, 6);

    f = exemplar_invenire(ex, chorda_ex_literis("world hello",
        piscina));
    CREDO_FALSUM(f.congruit);

    /* Ambae ancorae */
    ex = exemplar_compilare(chorda_ex_literis("^exact$", piscina),
        NIHIL, piscina);
    CREDO_NON_NIHIL(ex);

    CREDO_VERUM(exemplar_congruit_totum(ex, chorda_ex_literis("exact",
        piscina)));

    f = exemplar_invenire(ex, chorda_ex_literis("exact", piscina));
    CREDO_VERUM(f.congruit);

    f = exemplar_invenire(ex, chorda_ex_literis("not exact", piscina));
    CREDO_FALSUM(f.congruit);
}


/* ==================================================
 * Probationes: Effugium
 * ================================================== */

interior vacuum
_sectio_nona (
    CredoContextus* c)
{
     Piscina* piscina;
    Exemplar* ex;

    piscina = c->piscina;

    /* Effugere punctum */
    ex = exemplar_compilare(chorda_ex_literis("a\\.b", piscina), NIHIL,
        piscina);
    CREDO_NON_NIHIL(ex);

    CREDO_VERUM(exemplar_congruit_totum(ex, chorda_ex_literis("a.b",
        piscina)));
    CREDO_FALSUM(exemplar_congruit_totum(ex, chorda_ex_literis("aXb",
        piscina)));

    /* Effugere asteriscum */
    ex = exemplar_compilare(chorda_ex_literis("a\\*b", piscina), NIHIL,
        piscina);
    CREDO_NON_NIHIL(ex);

    CREDO_VERUM(exemplar_congruit_totum(ex, chorda_ex_literis("a*b",
        piscina)));

    /* Effugere bracketas */
    ex = exemplar_compilare(chorda_ex_literis("\\[test\\]", piscina),
        NIHIL, piscina);
    CREDO_NON_NIHIL(ex);

    CREDO_VERUM(exemplar_congruit_totum(ex, chorda_ex_literis("[test]",
        piscina)));
}


/* ==================================================
 * Probationes: Exemplaria Complexa
 * ================================================== */

interior vacuum
_sectio_decima (
    CredoContextus* c)
{
            Piscina* piscina;
           Exemplar* ex;
    ExemplarFructus  f;

    piscina = c->piscina;

    /* Annus: [0-9][0-9][0-9][0-9] */
    ex = exemplar_compilare(chorda_ex_literis("[0-9][0-9][0-9][0-9]",
        piscina), NIHIL, piscina);
    CREDO_NON_NIHIL(ex);

    f = exemplar_invenire(ex,
        chorda_ex_literis("Published in 1847 by...", piscina));
    CREDO_VERUM(f.congruit);
    CREDO_AEQUALIS_I32(f.initium, 13);
    CREDO_AEQUALIS_I32(f.longitudo, 4);

    /* Digiti: [0-9]+ */
    ex = exemplar_compilare(chorda_ex_literis("[0-9]+", piscina), NIHIL,
        piscina);
    CREDO_NON_NIHIL(ex);

    f = exemplar_invenire(ex, chorda_ex_literis("Page 42", piscina));
    CREDO_VERUM(f.congruit);
    CREDO_AEQUALIS_I32(f.initium, 5);
    CREDO_AEQUALIS_I32(f.longitudo, 2);

    /* Verbum capitalizatum: [A-Z][a-z]* */
    ex = exemplar_compilare(chorda_ex_literis("[A-Z][a-z]*", piscina),
        NIHIL, piscina);
    CREDO_NON_NIHIL(ex);

    f = exemplar_invenire(ex, chorda_ex_literis("hello World",
        piscina));
    CREDO_VERUM(f.congruit);
    CREDO_AEQUALIS_I32(f.initium, 6);
    CREDO_AEQUALIS_I32(f.longitudo, 5);

    /* Filum .txt: .*\\.txt$ */
    ex = exemplar_compilare(chorda_ex_literis(".*\\.txt$", piscina),
        NIHIL, piscina);
    CREDO_NON_NIHIL(ex);

    CREDO_VERUM(exemplar_congruit_totum(ex,
        chorda_ex_literis("document.txt", piscina)));
    CREDO_VERUM(exemplar_congruit_totum(ex,
        chorda_ex_literis("my.file.txt", piscina)));
    CREDO_FALSUM(exemplar_congruit_totum(ex,
        chorda_ex_literis("document.txt.bak", piscina)));
}


/* ==================================================
 * Probationes: Invenire Omnes
 * ================================================== */

interior vacuum
_sectio_decima_prima (
    CredoContextus* c)
{
            Piscina* piscina;
           Exemplar* ex;
    ExemplarFructus* fructus;
                i32  numerus;

    piscina = c->piscina;

    ex = exemplar_compilare(chorda_ex_literis("[0-9]+", piscina), NIHIL,
        piscina);
    CREDO_NON_NIHIL(ex);

    numerus = exemplar_invenire_omnes(ex,
        chorda_ex_literis("In 1847, chapter 12, page 345", piscina),
        &fructus, piscina);

    CREDO_AEQUALIS_I32(numerus, 3);
    CREDO_AEQUALIS_I32(fructus[0].initium, 3);   /* 1847 */
    CREDO_AEQUALIS_I32(fructus[0].longitudo, 4);
    CREDO_AEQUALIS_I32(fructus[1].initium, 17);  /* 12 */
    CREDO_AEQUALIS_I32(fructus[1].longitudo, 2);
    CREDO_AEQUALIS_I32(fructus[2].initium, 26);  /* 345 */
    CREDO_AEQUALIS_I32(fructus[2].longitudo, 3);
}


/* ==================================================
 * Probationes: Errores Compilationis
 * ================================================== */

interior vacuum
_sectio_decima_secunda (
    CredoContextus* c)
{
           Piscina* piscina;
          Exemplar* ex;
    ExemplarStatus  status;

    piscina = c->piscina;

    /* Classis non clausa */
    ex = exemplar_compilare(chorda_ex_literis("[abc", piscina), &status,
        piscina);
    CREDO_NIHIL(ex);
    CREDO_AEQUALIS_I32((i32)status,
        (i32)EXEMPLAR_ERROR_CLASSIS_NON_CLAUSA);

    /* Effugium ad finem */
    ex = exemplar_compilare(chorda_ex_literis("test\\", piscina),
        &status, piscina);
    CREDO_NIHIL(ex);
    CREDO_AEQUALIS_I32((i32)status, (i32)EXEMPLAR_ERROR_EFFUGIUM_FINIS);

    /* Ordo malus */
    ex = exemplar_compilare(chorda_ex_literis("[z-a]", piscina),
        &status, piscina);
    CREDO_NIHIL(ex);
    CREDO_AEQUALIS_I32((i32)status, (i32)EXEMPLAR_ERROR_ORDO_MALUS);

    /* Quantificator sine praecedente */
    ex = exemplar_compilare(chorda_ex_literis("*abc", piscina), &status,
        piscina);
    CREDO_NIHIL(ex);
    CREDO_AEQUALIS_I32((i32)status, (i32)EXEMPLAR_ERROR_SYNTAXIS);
}


/* ==================================================
 * Probationes: Functio Commoda
 * ================================================== */

interior vacuum
_sectio_decima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;

    CREDO_VERUM(exemplar_congruit_literis("[0-9]+",
        chorda_ex_literis("test123", piscina), piscina));

    CREDO_FALSUM(exemplar_congruit_literis("[0-9]+",
        chorda_ex_literis("no digits", piscina), piscina));
}


/* ==================================================
 * Probationes: Status Descriptio
 * ================================================== */

interior vacuum
_sectio_decima_quarta (
    CredoContextus* c)
{
    (vacuum)c;

    CREDO_NON_NIHIL(exemplar_status_descriptio(EXEMPLAR_OK));
    CREDO_NON_NIHIL(exemplar_status_descriptio(EXEMPLAR_ERROR_SYNTAXIS));
    CREDO_NON_NIHIL(exemplar_status_descriptio(EXEMPLAR_ERROR_CLASSIS_NON_CLAUSA));
}


hic_manens constans CredoSectio SECTIONES[] = {
    { "characteres literales", _sectio_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "punctum (.)", _sectio_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "asteriscum (*)", _sectio_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "plus (+)", _sectio_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "quaestionis (?)", _sectio_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "classes characterum", _sectio_sexta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "classem negatam", _sectio_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ancoras", _sectio_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "effugium", _sectio_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "exemplaria complexa", _sectio_decima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "invenire omnes", _sectio_decima_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "errores compilationis", _sectio_decima_secunda, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "functionem commodam", _sectio_decima_tertia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "status descriptio", _sectio_decima_quarta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};


/* ==================================================
 * Principale
 * ================================================== */

s32
principale (vacuum)
{
    redde credo_suitam_currere("exemplar", SECTIONES);
}

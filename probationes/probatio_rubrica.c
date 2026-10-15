/* probatio_rubrica.c - rubricae (tags) communes (nomenclator-plan
 * P1a): nomen validum, ordo canonicus, copia lecta (ordinata, sine
 * iteratis), iuncta, continentia */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "rubrica.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior chorda
c_ (
    constans character* s,
               Piscina* p)
{
    redde chorda_ex_literis(s, p);
}

/* copia lecta et iuncta aequalis exspectatae ("" = vacua) ? */
interior b32
lecta_est (
    constans character* textus,
    constans character* exspectata,
               Piscina* p)
{
       Xar* l;
    chorda  s;

    l = rubricas_legere(c_(textus, p), p);
    si (!l)
    {
        redde FALSUM;
    }
    s = rubricas_iungere(l, p);
    si (   s.mensura != (i32)strlen(exspectata)
        || (s.mensura > ZEPHYRUM && memcmp(s.datum, exspectata,
               (size_t)s.mensura) != ZEPHYRUM))
    {
        imprimere("  lecta '%s': '%.*s'\n", textus, (integer)s.mensura,
            s.datum ? (constans character*)s.datum : "");
        redde FALSUM;
    }
    redde VERUM;
}

interior vacuum
_probare_validam (
    CredoContextus* c)
{
    CREDO_VERUM(rubrica_valida(c_("domus", c->piscina)));
    CREDO_VERUM(rubrica_valida(c_("house-projects", c->piscina)));
    CREDO_VERUM(rubrica_valida(c_("a1-2", c->piscina)));
    CREDO_FALSUM(rubrica_valida(c_("", c->piscina)));
    CREDO_FALSUM(rubrica_valida(c_("Domus", c->piscina)));
    CREDO_FALSUM(rubrica_valida(c_("a b", c->piscina)));
    CREDO_FALSUM(rubrica_valida(c_("a_b", c->piscina)));
    CREDO_FALSUM(rubrica_valida(c_("a/b", c->piscina)));
}

interior vacuum
_probare_ordinem (
    CredoContextus* c)
{
    CREDO_VERUM(rubricas_comparare(c_("a", c->piscina), c_("b",
        c->piscina)) < ZEPHYRUM);
    CREDO_VERUM(rubricas_comparare(c_("b", c->piscina), c_("a",
        c->piscina)) > ZEPHYRUM);
    /* praefixum brevius prius */
    CREDO_VERUM(rubricas_comparare(c_("dom", c->piscina), c_("domus",
        c->piscina)) < ZEPHYRUM);
    CREDO_AEQUALIS_S32(rubricas_comparare(c_("opus", c->piscina),
        c_("opus", c->piscina)), ZEPHYRUM);
}

/* legere: ordinata, sine iteratis, spatia plura; mala -> NIHIL; vacua */
interior vacuum
_probare_legere (
    CredoContextus* c)
{
    Xar* l;

    CREDO_VERUM(lecta_est("opus domus", "domus opus", c->piscina));
    CREDO_VERUM(lecta_est("  b  a b   c ", "a b c", c->piscina));
    CREDO_VERUM(lecta_est("", "", c->piscina));
    CREDO_NIHIL(rubricas_legere(c_("domus Opus", c->piscina),
        c->piscina));
    l = rubricas_legere(c_("", c->piscina), c->piscina);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_AEQUALIS_I32(xar_numerus(l), ZEPHYRUM);
    CREDO_VERUM(rubricas_iungere(l, c->piscina).datum == NIHIL);
}

interior vacuum
_probare_habere (
    CredoContextus* c)
{
    Xar* l;

    l = rubricas_legere(c_("domus opus", c->piscina), c->piscina);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_VERUM(rubricam_habet(l, c_("opus", c->piscina)));
    CREDO_FALSUM(rubricam_habet(l, c_("op", c->piscina)));
    CREDO_FALSUM(rubricam_habet(NIHIL, c_("opus", c->piscina)));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "valida", _probare_validam, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ordo", _probare_ordinem, NIHIL, NIHIL, NIHIL, NIHIL },
    { "legere", _probare_legere, NIHIL, NIHIL, NIHIL, NIHIL },
    { "habere", _probare_habere, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("rubrica", SECTIONES);
}

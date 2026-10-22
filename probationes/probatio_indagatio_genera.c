/* probatio_indagatio_genera.c - GENERA QUAERENDA domus (indagatio S2):
 * res agendae, notae dierum, liber communis, personae cum notis -
 * tituli, contextus, semitae; renominatio, perfectio, deletio,
 * revocatio; paginae plures; rescriptio clavis unius; verificatio
 * post acta mixta; lectio sola. */

#include "latina.h"
#include "credo.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "xar.h"
#include "volumen.h"
#include "moneta.h"
#include "fasti.h"
#include "sors.h"
#include "agenda.h"
#include "personae.h"
#include "tabula_characterum.h"
#include "scriba_documentum.h"
#include "scriba_liber.h"
#include "conspectus.h"
#include "indagatio.h"
#include "indagatio_genera.h"

#include <stdio.h>
#include <string.h>


/* ==================================================
 * Mundus
 * ================================================== */

nomen structura {
                Volumen* volumen;
    InternamentumChorda* intern;
       AgendaDocumentum* agenda;
     PersonaeDocumentum* personae;
        IndagatioFontes  fontes;
    ConspectusRegistrum* repertorium;
              Indagatio* ix;
                Piscina* piscina;
                    i32  numerator_agendae;
                    i32  numerator_personarum;
} Mundus;

interior chorda
lit (
    constans character* s,
               Piscina* piscina)
{
    redde chorda_ex_literis(s, piscina);
}

/* registrum cum fontibus datis (NIHIL licet) - parare et indagatio */
interior b32
registrum_parare (
                Mundus* m,
      AgendaDocumentum* agenda,
    PersonaeDocumentum* personae,
               Volumen* libri)
{
    Xar* vitia;

    m->fontes.agenda    = agenda;
    m->fontes.personae  = personae;
    m->fontes.volumen   = libri;
    m->fontes.intern    = m->intern;
    m->repertorium = conspectus_registrum_creare(m->piscina, m->intern,
        m->volumen, ZEPHYRUM);
    si (   !m->repertorium
        || !indagatio_genera_registrare(m->repertorium, &m->fontes,
               &vitia)
        || !conspectus_parare(m->repertorium, &vitia))
    {
        redde FALSUM;
    }
    m->ix = indagatio_creare(m->piscina, m->repertorium);
    redde m->ix != NIHIL;
}

interior vacuum
mundum_creare (
    CredoContextus* c)
{
    Mundus* m;

    m = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(m, ZEPHYRUM, magnitudo(Mundus));
    c->datum    = m;
    m->piscina  = c->piscina;
    m->intern   = internamentum_creare(c->piscina);
    m->volumen  = volumen_temporarium(c->piscina,
        "probatio_indagatio_genera");
    si (!m->volumen)
    {
        redde;
    }
    m->agenda = agenda_aperire(c->piscina, m->intern, m->volumen,
        "agenda", ZEPHYRUM);
    agenda_monetam_ponere(m->agenda, moneta_numerator,
        &m->numerator_agendae);
    m->personae = personae_aperire(c->piscina, m->intern, m->volumen,
        "personae", ZEPHYRUM);
    personae_monetam_ponere(m->personae, moneta_numerator,
        &m->numerator_personarum);
}

/* mundus cum generibus omnibus */
interior vacuum
mundum_parare (
    CredoContextus* c)
{
    Mundus* m;

    mundum_creare(c);
    m = (Mundus*)c->datum;
    si (m->volumen)
    {
        (vacuum)registrum_parare(m, m->agenda, m->personae, m->volumen);
    }
}

interior vacuum
mundum_purgare (
    CredoContextus* c)
{
    Mundus* m;

    m = (Mundus*)c->datum;
    si (m && m->ix)
    {
        indagatio_destruere(m->ix);
    }
    si (m && m->repertorium)
    {
        conspectus_registrum_destruere(m->repertorium);
    }
    si (m && m->volumen)
    {
        volumen_claudere(m->volumen);
    }
}


/* ==================================================
 * Auxilia
 * ================================================== */

interior Dies
dies_iso (
    constans character* s,
               Piscina* piscina)
{
    Dies d;

    memset(&d, ZEPHYRUM, magnitudo(d));
    (vacuum)fasti_parsare_iso(chorda_ex_literis(s, piscina), &d);
    redde d;
}

/* pagina libri (praefixum NIHIL = communis) cum textu toto; condita si
 * deest */
interior b32
paginam_scribere (
                 Mundus* m,
     constans character* praefixum,
     constans character* titulus,
     constans character* textus)
{
          ScribaLiber* l;
     ScribaDocumentum* d;
    TabulaCharacterum  folium;

    l = praefixum ? scriba_liber_aperire_in_spatio(m->piscina,
        m->intern, m->volumen, praefixum, XL, X)
        : scriba_liber_aperire(m->piscina, m->intern, m->volumen, XL,
        X);
    d = l ? scriba_liber_paginam_condere(l, lit(titulus, m->piscina))
        : NIHIL;
    si (!d)
    {
        redde FALSUM;
    }
    tabula_ex_literis_cum_dimensionibus(&folium, m->piscina,
        d->latitudo, d->altitudo, textus);
    redde scriba_documentum_committere(d, &folium) > ZEPHYRUM;
}

interior IndagatioFructus*
quaerere (
                 Mundus* m,
     constans character* verba,
     constans character* conspectus,
                Piscina* piscina)
{
    IndagatioPetitio p;

    p.verba       = lit(verba, piscina);
    p.conspectus  = lit(conspectus ? conspectus : "", piscina);
    p.pagina      = ZEPHYRUM;
    p.capacitas   = XX;
    p.ordo        = INDAGATIO_ORDO_ALPHABETICUS;
    redde indagatio_quaerere(m->ix, &p, piscina);
}

interior constans IndagatioGrex*
grex (
    constans IndagatioFructus* f,
           constans character* conspectus)
{
    i32 i;

    per (i = ZEPHYRUM; f && f->greges
        && i < xar_numerus(f->greges); i++)
    {
        constans IndagatioGrex* g =
            (constans IndagatioGrex*)xar_obtinere(f->greges, i);

        si (chorda_aequalis_literis(g->conspectus, conspectus))
        {
            redde g;
        }
    }
    redde NIHIL;
}

/* numerus inventorum generis pro verbis */
interior i32
numerus (
                 Mundus* m,
     constans character* verba,
     constans character* conspectus,
                Piscina* piscina)
{
    constans IndagatioGrex* g;

    g = grex(quaerere(m, verba, conspectus, piscina), conspectus);
    redde g ? g->numerus : ZEPHYRUM;
}

/* inventum primum generis pro verbis; NIHIL si nullum */
interior constans IndagatioInventum*
inventum (
                 Mundus* m,
     constans character* verba,
     constans character* conspectus,
                Piscina* piscina)
{
    constans IndagatioGrex* g;

    g = grex(quaerere(m, verba, conspectus, piscina), conspectus);
    redde g && xar_numerus(g->inventa) > ZEPHYRUM
        ? (constans IndagatioInventum*)xar_obtinere(g->inventa,
        ZEPHYRUM)
        : NIHIL;
}

/* inventum generis cum titulo dato */
interior constans IndagatioInventum*
inventum_tituli (
                 Mundus* m,
     constans character* verba,
     constans character* conspectus,
     constans character* titulus,
                Piscina* piscina)
{
    constans IndagatioGrex* g;
                       i32  i;

    g = grex(quaerere(m, verba, conspectus, piscina), conspectus);
    per (i = ZEPHYRUM; g && i < xar_numerus(g->inventa); i++)
    {
        constans IndagatioInventum* v =
            (constans IndagatioInventum*)xar_obtinere(g->inventa, i);

        si (chorda_aequalis_literis(v->titulus, titulus))
        {
            redde v;
        }
    }
    redde NIHIL;
}

interior i32
discrepantes (
    Mundus* m)
{
    redde indagatio_verificare(m->ix, NIHIL);
}


/* ==================================================
 * Sectiones
 * ================================================== */

/* fontes absentes: genera eorum absunt; omnes: quattuor */
interior vacuum
_probare_fontes (
    CredoContextus* c)
{
     Mundus* m;
        Xar* q;

    mundum_creare(c);
    m = (Mundus*)c->datum;
    CREDO_NECESSE(m->volumen != NIHIL);

    CREDO_VERUM(registrum_parare(m, m->agenda, NIHIL, NIHIL));
    q = conspectus_quaerenda(m->repertorium, c->piscina);
    CREDO_AEQUALIS_I32(xar_numerus(q), I);
    CREDO_CHORDA_AEQUALIS_LITERIS(((ConspectusQuaerendum*)xar_obtinere(
        q, ZEPHYRUM))->titulus, "res");
    indagatio_destruere(m->ix);
    conspectus_registrum_destruere(m->repertorium);

    CREDO_VERUM(registrum_parare(m, NIHIL, m->personae, NIHIL));
    q = conspectus_quaerenda(m->repertorium, c->piscina);
    CREDO_AEQUALIS_I32(xar_numerus(q), I);
    CREDO_CHORDA_AEQUALIS_LITERIS(((ConspectusQuaerendum*)xar_obtinere(
        q, ZEPHYRUM))->titulus, "personae");
    indagatio_destruere(m->ix);
    conspectus_registrum_destruere(m->repertorium);

    CREDO_VERUM(registrum_parare(m, NIHIL, NIHIL, m->volumen));
    q = conspectus_quaerenda(m->repertorium, c->piscina);
    CREDO_AEQUALIS_I32(xar_numerus(q), II);
    indagatio_destruere(m->ix);
    conspectus_registrum_destruere(m->repertorium);

    CREDO_VERUM(registrum_parare(m, m->agenda, m->personae,
        m->volumen));
    q = conspectus_quaerenda(m->repertorium, c->piscina);
    CREDO_AEQUALIS_I32(xar_numerus(q), IV);
    CREDO_FALSUM(indagatio_genera_registrare(NIHIL, &m->fontes, NIHIL));
    CREDO_FALSUM(indagatio_genera_registrare(m->repertorium, NIHIL,
        NIHIL));
}

/* res agendae: titulus, contextus, semita; notae et rubricae in
 * corpore; renominatio, perfectio, deletio, revocatio */
interior vacuum
_probare_res (
    CredoContextus* c)
{
                        Mundus* m;
                        chorda  a;
                        chorda  b;
                          Dies  d;
    constans IndagatioInventum* v;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->ix);
    a = agenda_rem_creare(m->agenda, lit("Rhubarb tart", c->piscina));
    b = agenda_rem_creare(m->agenda, lit("Call the plumber",
        c->piscina));
    d = dies_iso("2026-10-21", c->piscina);
    CREDO_VERUM(agenda_diem_ponere(m->agenda, a, &d) > ZEPHYRUM);
    CREDO_VERUM(agenda_rubricas_ponere(m->agenda, a, lit("domus opus",
        c->piscina)) > ZEPHYRUM);
    CREDO_VERUM(agenda_notam_addere(m->agenda, a, dies_iso(
        "2026-10-20", c->piscina), lit("buy sugar at the market",
        c->piscina))
        > ZEPHYRUM);

    CREDO_GRADUS("titulus, contextus, semita");
    v = inventum(m, "rhubarb", "res", c->piscina);
    CREDO_NECESSE_NON_NIHIL(v);
    CREDO_CHORDA_AEQUALIS_LITERIS(v->titulus, "Rhubarb tart");
    CREDO_CHORDA_AEQUALIS_LITERIS(v->contextus,
        "due Wed 21 Oct 2026 - #domus #opus");
    CREDO_CHORDA_AEQUALIS_LITERIS(v->semita,
        "calendarium/dies/2026-10-21?res_electa=1");
    v = inventum(m, "plumber", "res", c->piscina);
    CREDO_NECESSE_NON_NIHIL(v);
    CREDO_CHORDA_AEQUALIS_LITERIS(v->contextus, "no date");
    CREDO_CHORDA_AEQUALIS_LITERIS(v->semita,
        "calendarium/agenda?res_electa=2");

    CREDO_GRADUS("notae et rubricae in corpore");
    CREDO_AEQUALIS_I32(numerus(m, "sugar market", "res", c->piscina),
        I);
    CREDO_AEQUALIS_I32(numerus(m, "opus", "res", c->piscina), I);

    CREDO_GRADUS("perfecta: contextus 'done'");
    CREDO_VERUM(agenda_perficere(m->agenda, a, dies_iso("2026-10-01",
        c->piscina))
        > ZEPHYRUM);
    v = inventum(m, "rhubarb", "res", c->piscina);
    CREDO_NECESSE_NON_NIHIL(v);
    CREDO_CHORDA_AEQUALIS_LITERIS(v->contextus,
        "done Thu 1 Oct 2026 - #domus #opus");

    CREDO_GRADUS("renominatio: titulus novus solus");
    CREDO_VERUM(agenda_titulum_ponere(m->agenda, a, lit("Apple pie",
        c->piscina)) > ZEPHYRUM);
    CREDO_AEQUALIS_I32(numerus(m, "rhubarb", "res", c->piscina),
        ZEPHYRUM);
    CREDO_AEQUALIS_I32(numerus(m, "apple", "res", c->piscina), I);

    CREDO_GRADUS("deletio et revocatio");
    CREDO_VERUM(agenda_rem_delere(m->agenda, b) > ZEPHYRUM);
    CREDO_AEQUALIS_I32(numerus(m, "plumber", "res", c->piscina),
        ZEPHYRUM);
    CREDO_VERUM(agenda_revocare(m->agenda));
    CREDO_AEQUALIS_I32(numerus(m, "plumber", "res", c->piscina), I);
    CREDO_AEQUALIS_I32(discrepantes(m), ZEPHYRUM);
}

/* notae dierum: ordo per paginam, "page n of x" cum paginis pluribus,
 * semita cum pagina (D5 S3c: ?notae.pagina=); pagina vacua nullum
 * ordinem */
interior vacuum
_probare_notas (
    CredoContextus* c)
{
                        Mundus* m;
    constans IndagatioInventum* v;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->ix);
    /* ante aedificationem: enumeratio librum invenit */
    CREDO_VERUM(paginam_scribere(m, "dies/2026-10-19", "1",
        "alpha rhubarb\n  second line"));

    CREDO_GRADUS("pagina una");
    v = inventum(m, "rhubarb", "notae", c->piscina);
    CREDO_NECESSE_NON_NIHIL(v);
    CREDO_CHORDA_AEQUALIS_LITERIS(v->titulus, "Mon 19 Oct 2026");
    CREDO_AEQUALIS_I32(v->contextus.mensura, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(v->semita,
        "calendarium/dies/2026-10-19?notae.pagina=1");
    CREDO_AEQUALIS_I32(numerus(m, "second", "notae", c->piscina), I);

    CREDO_GRADUS("paginae duae: tituli ambo mutantur");
    CREDO_VERUM(paginam_scribere(m, "dies/2026-10-19", "2",
        "beta rhubarb"));
    CREDO_AEQUALIS_I32(numerus(m, "rhubarb", "notae", c->piscina), II);
    CREDO_NON_NIHIL(inventum_tituli(m, "alpha", "notae",
        "Mon 19 Oct 2026 - page 1 of 2", c->piscina));
    CREDO_NON_NIHIL(inventum_tituli(m, "beta", "notae",
        "Mon 19 Oct 2026 - page 2 of 2", c->piscina));
    /* D5 S3c: semita paginam suam fert */
    v = inventum(m, "beta", "notae", c->piscina);
    CREDO_NECESSE_NON_NIHIL(v);
    CREDO_CHORDA_AEQUALIS_LITERIS(v->semita,
        "calendarium/dies/2026-10-19?notae.pagina=2");

    CREDO_GRADUS("pagina vacua, textus deletus");
    (vacuum)paginam_scribere(m, "dies/2026-10-20", "1", "");
    CREDO_VERUM(paginam_scribere(m, "dies/2026-10-19", "2", ""));
    CREDO_AEQUALIS_I32(numerus(m, "beta", "notae", c->piscina),
        ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)indagatio_mensura(m->ix).ordines, I);
    CREDO_AEQUALIS_I32(discrepantes(m), ZEPHYRUM);
}

/* liber communis: clavis per paginam; titulus linea prima; scriptio
 * in pagina una ordinem unum rescribit */
interior vacuum
_probare_librum (
    CredoContextus* c)
{
                        Mundus* m;
    constans IndagatioInventum* v;
                           i64  ante;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->ix);
    CREDO_VERUM(paginam_scribere(m, NIHIL, "1",
        "   Groceries\nrhubarb, sugar"));
    CREDO_VERUM(paginam_scribere(m, NIHIL, "2", "Garden plan\nbeans"));
    CREDO_VERUM(paginam_scribere(m, NIHIL, "notae-x", "Ideas\nkites"));

    CREDO_GRADUS("titulus, contextus, semita");
    v = inventum(m, "rhubarb", "liber", c->piscina);
    CREDO_NECESSE_NON_NIHIL(v);
    CREDO_CHORDA_AEQUALIS_LITERIS(v->titulus, "Groceries");
    CREDO_CHORDA_AEQUALIS_LITERIS(v->contextus, "page 1");
    CREDO_CHORDA_AEQUALIS_LITERIS(v->semita, "scriba/1");
    v = inventum(m, "kites", "liber", c->piscina);
    CREDO_NECESSE_NON_NIHIL(v);
    CREDO_CHORDA_AEQUALIS_LITERIS(v->contextus, "page 3");
    CREDO_CHORDA_AEQUALIS_LITERIS(v->semita, "scriba/notae-x");
    /* liber diei non est liber communis */
    CREDO_VERUM(paginam_scribere(m, "dies/2026-10-19", "1", "kites"));
    CREDO_AEQUALIS_I32(numerus(m, "kites", "liber", c->piscina), I);

    CREDO_GRADUS("scriptio in pagina una: ordo unus rescriptus");
    ante = (i64)indagatio_mensura(m->ix).rescripta;
    CREDO_VERUM(paginam_scribere(m, NIHIL, "2",
        "Garden plan\nbeans, peas"));
    CREDO_AEQUALIS_I32(numerus(m, "peas", "liber", c->piscina), I);
    CREDO_AEQUALIS_S64((s64)(indagatio_mensura(m->ix).rescripta - ante),
        (s64)I);
    CREDO_AEQUALIS_I32(discrepantes(m), ZEPHYRUM);
}

/* personae: nomen, contextus, semita; epistulae, telephona (cifrae a
 * quovis grege), rubricae, notae - inventum unum per personam */
interior vacuum
_probare_personas (
    CredoContextus* c)
{
                        Mundus* m;
                        chorda  a;
                        chorda  s;
                           Xar* t;
    constans IndagatioInventum* v;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->ix);
    a = personae_hominem_creare(m->personae, lit("Anna", c->piscina),
        lit("Rossi", c->piscina));
    t = xar_creare(c->piscina, (i32)magnitudo(chorda));
    *(chorda*)xar_addere(t) = lit("anna.rossi@example.com", c->piscina);
    CREDO_VERUM(personae_epistulas_ponere(m->personae, a, t)
        > ZEPHYRUM);
    t = xar_creare(c->piscina, (i32)magnitudo(chorda));
    *(chorda*)xar_addere(t) = lit("+1 (555) 123-4567", c->piscina);
    CREDO_VERUM(personae_telephona_ponere(m->personae, a, t)
        > ZEPHYRUM);
    CREDO_VERUM(personae_rubricas_ponere(m->personae, a,
        lit("familia", c->piscina)) > ZEPHYRUM);
    s = personae_societatem_creare(m->personae, lit("Acme Bakery",
        c->piscina));
    CREDO_VERUM(paginam_scribere(m, "persona/1", "1",
        "likes rhubarb\nbirthday cake"));

    CREDO_GRADUS("titulus, contextus, semita");
    v = inventum(m, "anna", "personae", c->piscina);
    CREDO_NECESSE_NON_NIHIL(v);
    CREDO_CHORDA_AEQUALIS_LITERIS(v->titulus, "Anna Rossi");
    CREDO_CHORDA_AEQUALIS_LITERIS(v->contextus,
        "anna.rossi@example.com - +1 (555) 123-4567");
    CREDO_CHORDA_AEQUALIS_LITERIS(v->semita, "nomenclator/1");
    v = inventum(m, "bakery", "personae", c->piscina);
    CREDO_NECESSE_NON_NIHIL(v);
    CREDO_CHORDA_AEQUALIS_LITERIS(v->titulus, "Acme Bakery");
    CREDO_AEQUALIS_I32(v->contextus.mensura, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(v->semita, "nomenclator/2");

    CREDO_GRADUS("campi et notae: inventum unum");
    CREDO_AEQUALIS_I32(numerus(m, "anna.rossi@example.com", "personae",
        c->piscina), I);
    CREDO_AEQUALIS_I32(numerus(m, "familia", "personae", c->piscina),
        I);
    CREDO_AEQUALIS_I32(numerus(m, "rhubarb", "personae", c->piscina),
        I);
    CREDO_AEQUALIS_I32(numerus(m, "anna rhubarb", "personae",
        c->piscina), I);
    CREDO_AEQUALIS_I32(numerus(m, "555", "personae", c->piscina), I);
    CREDO_AEQUALIS_I32(numerus(m, "5551234", "personae", c->piscina),
        I);
    CREDO_AEQUALIS_I32(numerus(m, "1234567", "personae", c->piscina),
        I);
    CREDO_AEQUALIS_I32(numerus(m, "15551234567", "personae",
        c->piscina), I);
    CREDO_AEQUALIS_I32(numerus(m, "2345", "personae", c->piscina),
        ZEPHYRUM);

    CREDO_GRADUS("notae mutatae, nomen mutatum, deletio");
    CREDO_VERUM(paginam_scribere(m, "persona/1", "1", "likes pie"));
    CREDO_AEQUALIS_I32(numerus(m, "rhubarb", "personae", c->piscina),
        ZEPHYRUM);
    CREDO_VERUM(personae_nomen_ponere(m->personae, a, lit("Anna",
        c->piscina), lit("Bianchi", c->piscina)) > ZEPHYRUM);
    CREDO_AEQUALIS_I32(numerus(m, "rossi", "personae", c->piscina),
        I);   /* epistula manet */
    CREDO_NON_NIHIL(inventum_tituli(m, "bianchi", "personae",
        "Anna Bianchi", c->piscina));
    CREDO_VERUM(personae_delere(m->personae, s) > ZEPHYRUM);
    CREDO_AEQUALIS_I32(numerus(m, "bakery", "personae", c->piscina),
        ZEPHYRUM);
    CREDO_AEQUALIS_I32(discrepantes(m), ZEPHYRUM);
}

/* sine volumine librorum: personae sine notis */
interior vacuum
_probare_sine_libris (
    CredoContextus* c)
{
    Mundus* m;
    chorda  a;

    mundum_creare(c);
    m = (Mundus*)c->datum;
    CREDO_NECESSE(m->volumen != NIHIL);
    a = personae_hominem_creare(m->personae, lit("Anna", c->piscina),
        lit("Rossi", c->piscina));
    CREDO_VERUM(a.mensura > ZEPHYRUM);
    CREDO_VERUM(paginam_scribere(m, "persona/1", "1", "likes rhubarb"));
    CREDO_VERUM(registrum_parare(m, NIHIL, m->personae, NIHIL));
    CREDO_AEQUALIS_I32(numerus(m, "anna", "personae", c->piscina), I);
    CREDO_AEQUALIS_I32(numerus(m, "rhubarb", "personae", c->piscina),
        ZEPHYRUM);
}

/* acta mixta fortuita per genera omnia: index == reaedificatio; lectio
 * sola (quaestiones nihil scribunt) */
interior vacuum
_probare_verificationem (
    CredoContextus* c)
{
     Mundus* m;
        Xar* res;
        Xar* personae;
       Sors  s;
        i32  i;
        s64  acta;
  character  buffer[LXIV];
  character  praefixum[XXXII];

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->ix);
    res       = xar_creare(c->piscina, (i32)magnitudo(chorda));
    personae  = xar_creare(c->piscina, (i32)magnitudo(chorda));
    sors_seminare(&s, (i64)XI, ZEPHYRUM);
    per (i = ZEPHYRUM; i < CL; i++)
    {
        i32 actio = sors_intra(&s, IX);

        sprintf(buffer, "verbum%d alia%d", (integer)sors_intra(&s, XX),
            (integer)i);
        si (actio == ZEPHYRUM || xar_numerus(res) == ZEPHYRUM)
        {
            *(chorda*)xar_addere(res) = chorda_transcribere(
                agenda_rem_creare(m->agenda, lit(buffer, c->piscina)),
                c->piscina);
        }
        alioquin si (actio == I)
        {
            (vacuum)agenda_titulum_ponere(m->agenda, *(chorda*)
                xar_obtinere(res, sors_intra(&s, xar_numerus(res))),
                lit(buffer, c->piscina));
        }
        alioquin si (actio == II)
        {
            Dies d = fasti_dies(MMXXVI, X, (s32)(I + sors_intra(&s,
                XXVIII)));

            (vacuum)agenda_perficere(m->agenda, *(chorda*)xar_obtinere(
                res, sors_intra(&s, xar_numerus(res))), d);
        }
        alioquin si (actio == III)
        {
            (vacuum)agenda_revocare(m->agenda);
        }
        alioquin si (actio == IV)
        {
            sprintf(praefixum, "dies/2026-10-%02d", (integer)(I
                + sors_intra(&s, V)));
            (vacuum)paginam_scribere(m, praefixum, sors_intra(&s, II)
                ? "1" : "2", buffer);
        }
        alioquin si (actio == V)
        {
            (vacuum)paginam_scribere(m, NIHIL, sors_intra(&s, II) ? "1"
                : "2", buffer);
        }
        alioquin si (actio == VI)
        {
            chorda p = personae_hominem_creare(m->personae, lit(buffer,
                c->piscina), lit("X", c->piscina));

            si (p.mensura > ZEPHYRUM)
            {
                *(chorda*)xar_addere(personae) = chorda_transcribere(p,
                    c->piscina);
            }
        }
        alioquin si (actio == VII && xar_numerus(personae) > ZEPHYRUM)
        {
            sprintf(praefixum, "persona/%.*s", (integer)((chorda*)
                xar_obtinere(personae, ZEPHYRUM))->mensura,
                (constans character*)((chorda*)xar_obtinere(personae,
                ZEPHYRUM))->datum);
            (vacuum)paginam_scribere(m, praefixum, "1", buffer);
        }
        alioquin
        {
            (vacuum)quaerere(m, "verbum1", NIHIL, c->piscina);
        }
    }
    acta = volumen_summa_actorum(m->volumen);
    CREDO_AEQUALIS_I32(discrepantes(m), ZEPHYRUM);
    (vacuum)quaerere(m, "verbum alia", NIHIL, c->piscina);
    CREDO_AEQUALIS_S64(volumen_summa_actorum(m->volumen), acta);
    CREDO_VERUM(indagatio_mensura(m->ix).ordines > X);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "fontes",        _probare_fontes,          NIHIL,
        mundum_purgare, NIHIL, NIHIL },
    { "res",           _probare_res,             mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "notae",         _probare_notas,           mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "liber",         _probare_librum,          mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "personae",      _probare_personas,        mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "sine libris",   _probare_sine_libris,     NIHIL,
        mundum_purgare, NIHIL, NIHIL },
    { "verificatio",   _probare_verificationem,  mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("indagatio_genera", SECTIONES);
}

/* probatio_indagatio.c - INDAGATIO (genera-datorum D5, S1): verba
 * plana, aedificatio pigra, rescriptio sordidarum, greges, paginae,
 * verificatio.
 *
 * Genera FIXA (probatio sola):
 *   res      agenda per-rem, clavis = id; ordo unus (titulus, notae)
 *   nota     spatium nota/{clavis}: acta 'nota/<k>/textus' = paginae
 *            (clavis una, ordines plures)
 *   homines  personae omnia; ordo unus (nomen, epistulae)
 *   titulus  spatium titulus/{clavis}: actum ultimum = ordo unus
 *            (memoria: documentum in memoria nullum crescit) */

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
#include "conspectus.h"
#include "indagatio.h"
#include "memoria_processus.h"

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
    ConspectusRegistrum* repertorium;
              Indagatio* ix;
                    Xar* notae;       /* chorda: claves notarum */
                    Xar* tituli;      /* chorda: claves titulorum */
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

interior IndagatioConspectus*
conspectum_novum (
    Piscina* piscina)
{
    IndagatioConspectus* v;

    v = (IndagatioConspectus*)piscina_allocare(piscina,
        magnitudo(IndagatioConspectus));
    v->ordines = xar_creare(piscina, (i32)magnitudo(IndagatioOrdo));
    redde v;
}

interior vacuum
ordinem_addere (
    IndagatioConspectus* v,
                 chorda  titulus,
                 chorda  contextus,
                 chorda  corpus,
                 chorda  semita,
                Piscina* piscina)
{
    IndagatioOrdo* o;

    o             = (IndagatioOrdo*)xar_addere(v->ordines);
    o->titulus    = chorda_transcribere(titulus, piscina);
    o->contextus  = chorda_transcribere(contextus, piscina);
    o->corpus     = chorda_transcribere(corpus, piscina);
    o->semita     = chorda_transcribere(semita, piscina);
}


/* ==================================================
 * Genus 'res' (agenda)
 * ================================================== */

interior vacuum*
res_computare (
                 vacuum* ctx,
    ConspectusRegistrum* repertorium,
                 chorda  clavis,
                Piscina* piscina)
{
                 Mundus* m;
     constans AgendaRes* r;
    IndagatioConspectus* v;
                 chorda  corpus;
                    i32  i;

    (vacuum)repertorium;
    m = (Mundus*)ctx;
    r = agenda_res(m->agenda, clavis);
    si (!r)
    {
        redde NIHIL;
    }
    corpus = lit("", piscina);
    per (i = ZEPHYRUM; r->notae && i < xar_numerus(r->notae); i++)
    {
        corpus = chorda_concatenare(corpus, ((AgendaNota*)xar_obtinere(
            r->notae, i))->textus, piscina);
        corpus = chorda_concatenare(corpus, lit("\n", piscina),
            piscina);
    }
    v = conspectum_novum(piscina);
    ordinem_addere(v, r->titulus, lit(r->perfecta ? "perfecta"
        : "aperta", piscina), corpus, chorda_concatenare(lit("agenda/",
        piscina), clavis, piscina), piscina);
    redde v;
}

interior i32
res_numerus (
    constans vacuum* documentum)
{
    redde agenda_numerus_rerum((constans AgendaDocumentum*)documentum);
}

interior chorda
res_id (
    constans vacuum* documentum,
                i32  index,
            Piscina* piscina)
{
    (vacuum)piscina;
    redde agenda_res_index((constans AgendaDocumentum*)documentum,
        index)->id;
}

interior chorda
res_vestigium (
    constans vacuum* documentum,
                i32  index,
            Piscina* piscina)
{
    redde agenda_rem_codificare((constans AgendaDocumentum*)documentum,
        index, piscina);
}

interior vacuum
res_claves (
             vacuum* ctx,
    constans vacuum* documentum,
                i32  index,
                Xar* claves,
            Piscina* piscina)
{
    (vacuum)ctx;
    *(chorda*)xar_addere(claves) = chorda_transcribere(
        agenda_res_index((constans AgendaDocumentum*)documentum,
        index)->id, piscina);
}

interior Xar*
res_enumerare (
                 vacuum* ctx,
    ConspectusRegistrum* repertorium,
                Piscina* piscina)
{
    Mundus* m;
       Xar* claves;
       i32  i;

    (vacuum)repertorium;
    m       = (Mundus*)ctx;
    claves  = xar_creare(piscina, (i32)magnitudo(chorda));
    per (i = ZEPHYRUM; i < agenda_numerus_rerum(m->agenda); i++)
    {
        *(chorda*)xar_addere(claves) = chorda_transcribere(
            agenda_res_index(m->agenda, i)->id, piscina);
    }
    redde claves;
}


/* ==================================================
 * Genus 'nota' (paginae ex actis)
 * ================================================== */

interior vacuum*
nota_computare (
                 vacuum* ctx,
    ConspectusRegistrum* repertorium,
                 chorda  clavis,
                Piscina* piscina)
{
                 Mundus* m;
    IndagatioConspectus* v;
                    Xar* acta;
                    i32  i;
              character  genus[CXXVIII];
              character  titulus[CXXVIII];
     constans character* genera[II];

    (vacuum)repertorium;
    m = (Mundus*)ctx;
    sprintf(genus, "nota/%.*s/textus", (integer)clavis.mensura,
        (constans character*)clavis.datum);
    genera[ZEPHYRUM]  = genus;
    genera[I]         = NIHIL;
    acta = volumen_acta_generum_legere(m->volumen, ZEPHYRUM, genera,
        piscina);
    v = conspectum_novum(piscina);
    per (i = ZEPHYRUM; acta && i < xar_numerus(acta); i++)
    {
        VolumenActum* a = (VolumenActum*)xar_obtinere(acta, i);
           character  semita[CXXVIII];

        sprintf(titulus, "%.*s - pagina %u de %u",
            (integer)clavis.mensura, (constans character*)clavis.datum,
            (insignatus integer)(i + I),
            (insignatus integer)xar_numerus(acta));
        sprintf(semita, "nota/%.*s?pagina=%u", (integer)clavis.mensura,
            (constans character*)clavis.datum, (insignatus integer)(i
                + I));
        ordinem_addere(v, lit(titulus, piscina), lit("", piscina),
            a->datum, lit(semita, piscina), piscina);
    }
    redde v;
}

interior Xar*
notas_enumerare (
                 vacuum* ctx,
    ConspectusRegistrum* repertorium,
                Piscina* piscina)
{
    Mundus* m;
       Xar* claves;
       i32  i;

    (vacuum)repertorium;
    m       = (Mundus*)ctx;
    claves  = xar_creare(piscina, (i32)magnitudo(chorda));
    per (i = ZEPHYRUM; i < xar_numerus(m->notae); i++)
    {
        *(chorda*)xar_addere(claves) = chorda_transcribere(
            *(chorda*)xar_obtinere(m->notae, i), piscina);
    }
    redde claves;
}


/* ==================================================
 * Genus 'homines' (personae, omnia)
 * ================================================== */

interior vacuum*
homines_computare (
                 vacuum* ctx,
    ConspectusRegistrum* repertorium,
                 chorda  clavis,
                Piscina* piscina)
{
                 Mundus* m;
       constans Persona* p;
    IndagatioConspectus* v;
                 chorda  corpus;
                    i32  i;

    (vacuum)repertorium;
    m = (Mundus*)ctx;
    p = personae_persona(m->personae, clavis);
    si (!p)
    {
        redde NIHIL;
    }
    corpus = lit("", piscina);
    per (i = ZEPHYRUM; p->epistulae
        && i < xar_numerus(p->epistulae); i++)
    {
        corpus = chorda_concatenare(corpus, *(chorda*)xar_obtinere(
            p->epistulae, i), piscina);
        corpus = chorda_concatenare(corpus, lit("\n", piscina),
            piscina);
    }
    v = conspectum_novum(piscina);
    ordinem_addere(v, persona_nomen(p, piscina), lit("", piscina),
        corpus,
        chorda_concatenare(lit("personae/", piscina), clavis, piscina),
        piscina);
    redde v;
}

interior Xar*
homines_enumerare (
                 vacuum* ctx,
    ConspectusRegistrum* repertorium,
                Piscina* piscina)
{
    Mundus* m;
       Xar* claves;
       i32  i;

    (vacuum)repertorium;
    m       = (Mundus*)ctx;
    claves  = xar_creare(piscina, (i32)magnitudo(chorda));
    per (i = ZEPHYRUM; i < personae_numerus(m->personae); i++)
    {
        *(chorda*)xar_addere(claves) = chorda_transcribere(
            personae_index(m->personae, i)->id, piscina);
    }
    redde claves;
}


/* ==================================================
 * Genus 'titulus' (actum ultimum spatii)
 * ================================================== */

interior vacuum*
titulus_computare (
                 vacuum* ctx,
    ConspectusRegistrum* repertorium,
                 chorda  clavis,
                Piscina* piscina)
{
                 Mundus* m;
    IndagatioConspectus* v;
                    Xar* acta;
                Piscina* scheda;
              character  genus[CXXVIII];
     constans character* genera[II];

    (vacuum)repertorium;
    m = (Mundus*)ctx;
    sprintf(genus, "titulus/%.*s/textus", (integer)clavis.mensura,
        (constans character*)clavis.datum);
    genera[ZEPHYRUM]  = genus;
    genera[I]         = NIHIL;
    /* acta omnia in scheda (crescunt): conspectus actum ultimum solum
     * copiat - aliter cache cum actis cresceret */
    scheda  = piscina_generare_dynamicum("probatio_titulus",
        IV * MXXIV);
    acta    = scheda ? volumen_acta_generum_legere(m->volumen, ZEPHYRUM,
        genera, scheda) : NIHIL;
    v = conspectum_novum(piscina);
    si (acta && xar_numerus(acta) > ZEPHYRUM)
    {
        ordinem_addere(v, ((VolumenActum*)xar_obtinere(acta,
            xar_numerus(acta) - I))->datum, lit("", piscina),
            lit("", piscina), chorda_concatenare(lit("titulus/",
            piscina),
            clavis, piscina), piscina);
    }
    si (scheda)
    {
        piscina_destruere(scheda);
    }
    redde v;
}

interior Xar*
titulos_enumerare (
                 vacuum* ctx,
    ConspectusRegistrum* repertorium,
                Piscina* piscina)
{
    Mundus* m;
       Xar* claves;
       i32  i;

    (vacuum)repertorium;
    m       = (Mundus*)ctx;
    claves  = xar_creare(piscina, (i32)magnitudo(chorda));
    per (i = ZEPHYRUM; i < xar_numerus(m->tituli); i++)
    {
        *(chorda*)xar_addere(claves) = chorda_transcribere(
            *(chorda*)xar_obtinere(m->tituli, i), piscina);
    }
    redde claves;
}


/* ==================================================
 * Declarationes et registrum
 * ================================================== */

hic_manens constans character* constans DECLARATIO_RERUM =
    "<conspectus nomen=\"res\" versio=\"1\">\n"
    "  <clavis typus=\"textus\"/>\n"
    "  <fons documentum=\"agenda\" irritat=\"per-rem\"/>\n"
    "  <quaerendum genus=\"Agenda\"/>\n"
    "</conspectus>\n";

hic_manens constans character* constans DECLARATIO_NOTAE =
    "<conspectus nomen=\"nota\" versio=\"1\">\n"
    "  <clavis typus=\"textus\"/>\n"
    "  <fons spatium=\"nota/{clavis}\"/>\n"
    "  <quaerendum genus=\"Notae\"/>\n"
    "</conspectus>\n";

hic_manens constans character* constans DECLARATIO_TITULI =
    "<conspectus nomen=\"titulus\" versio=\"1\">\n"
    "  <clavis typus=\"textus\"/>\n"
    "  <fons spatium=\"titulus/{clavis}\"/>\n"
    "  <quaerendum genus=\"Tituli\"/>\n"
    "</conspectus>\n";

hic_manens constans character* constans DECLARATIO_HOMINUM =
    "<conspectus nomen=\"homines\" versio=\"1\">\n"
    "  <clavis typus=\"textus\"/>\n"
    "  <fons documentum=\"personae\" irritat=\"omnia\"/>\n"
    "  <quaerendum genus=\"Personae\"/>\n"
    "</conspectus>\n";

interior ConspectusOpera
opera (
    vacuum* (*computare)(vacuum*, ConspectusRegistrum*, chorda,
                         Piscina*),
    vacuum* ctx)
{
    ConspectusOpera o;

    o.computare   = computare;
    o.codificare  = indagatio_conspectum_codificare;
    o.ctx         = ctx;
    redde o;
}

interior b32
registrum_parare (
    Mundus* m)
{
    ConspectusRegistrum* r;
       ConspectusPerRem  p;
                    Xar* vitia;

    r = conspectus_registrum_creare(m->piscina, m->intern, m->volumen,
        ZEPHYRUM);
    si (   !r
        || !conspectus_declarare(r, lit(DECLARATIO_RERUM, m->piscina),
               &vitia)
        || !conspectus_declarare(r, lit(DECLARATIO_NOTAE, m->piscina),
               &vitia)
        || !conspectus_declarare(r, lit(DECLARATIO_HOMINUM, m->piscina),
               &vitia)
        || !conspectus_declarare(r, lit(DECLARATIO_TITULI, m->piscina),
               &vitia))
    {
        redde FALSUM;
    }
    (vacuum)conspectus_opera_ponere(r, "res", opera(res_computare, m));
    (vacuum)conspectus_opera_ponere(r, "nota", opera(nota_computare,
        m));
    (vacuum)conspectus_opera_ponere(r, "homines", opera(
        homines_computare, m));
    (vacuum)conspectus_opera_ponere(r, "titulus", opera(
        titulus_computare, m));
    p.numerus    = res_numerus;
    p.id         = res_id;
    p.vestigium  = res_vestigium;
    p.claves     = res_claves;
    p.ctx        = NIHIL;
    (vacuum)conspectus_per_rem_ponere(r, "res", "agenda", p);
    (vacuum)conspectus_documentum_ligare(r, "agenda", m->agenda);
    (vacuum)conspectus_documentum_ligare(r, "personae", m->personae);
    (vacuum)conspectus_enumerationem_ponere(r, "res", res_enumerare, m);
    (vacuum)conspectus_enumerationem_ponere(r, "nota", notas_enumerare,
        m);
    (vacuum)conspectus_enumerationem_ponere(r, "homines",
        homines_enumerare, m);
    (vacuum)conspectus_enumerationem_ponere(r, "titulus",
        titulos_enumerare, m);
    si (!conspectus_parare(r, &vitia))
    {
        redde FALSUM;
    }
    m->repertorium  = r;
    m->ix           = indagatio_creare(m->piscina, r);
    redde m->ix != NIHIL;
}

interior vacuum
mundum_parare (
    CredoContextus* c)
{
    Mundus* m;

    m = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(m, ZEPHYRUM, magnitudo(Mundus));
    c->datum    = m;
    m->piscina  = c->piscina;
    m->intern   = internamentum_creare(c->piscina);
    m->notae    = xar_creare(c->piscina, (i32)magnitudo(chorda));
    m->tituli   = xar_creare(c->piscina, (i32)magnitudo(chorda));
    m->volumen  = volumen_temporarium(c->piscina, "probatio_indagatio");
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
    (vacuum)registrum_parare(m);
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

interior chorda
rem_creare (
                Mundus* m,
    constans character* titulus)
{
    redde agenda_rem_creare(m->agenda, lit(titulus, m->piscina));
}

/* pagina nova notae k (actum 'nota/<k>/textus') */
interior vacuum
notam_scribere (
                Mundus* m,
    constans character* k,
    constans character* textus)
{
    character genus[CXXVIII];
          i32 i;

    sprintf(genus, "nota/%s/textus", k);
    (vacuum)volumen_actum_appendere(m->volumen, genus, lit(textus,
        m->piscina));
    per (i = ZEPHYRUM; i < xar_numerus(m->notae); i++)
    {
        si (chorda_aequalis_literis(*(chorda*)xar_obtinere(m->notae, i),
                k))
        {
            redde;
        }
    }
    *(chorda*)xar_addere(m->notae) = lit(k, m->piscina);
}

interior IndagatioFructus*
quaerere (
                 Mundus* m,
     constans character* verba,
     constans character* conspectus,
                    i32  pagina,
                    i32  capacitas,
    IndagatioOrdoGregum  ordo,
                Piscina* piscina)
{
    IndagatioPetitio p;

    p.verba       = lit(verba, piscina);
    p.conspectus  = lit(conspectus ? conspectus : "", piscina);
    p.pagina      = pagina;
    p.capacitas   = capacitas;
    p.ordo        = ordo;
    redde indagatio_quaerere(m->ix, &p, piscina);
}

interior IndagatioFructus*
quaerere_simpliciter (
                 Mundus* m,
     constans character* verba,
                Piscina* piscina)
{
    redde quaerere(m, verba, NIHIL, ZEPHYRUM, X,
        INDAGATIO_ORDO_ALPHABETICUS, piscina);
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
            (constans IndagatioGrex*)xar_obtinere(
            f->greges, i);

        si (chorda_aequalis_literis(g->conspectus, conspectus))
        {
            redde g;
        }
    }
    redde NIHIL;
}

interior b32
titulus_inventus (
    constans IndagatioGrex* g,
        constans character* titulus)
{
    i32 i;

    per (i = ZEPHYRUM; g && i < xar_numerus(g->inventa); i++)
    {
        si (chorda_aequalis_literis(((IndagatioInventum*)xar_obtinere(
                g->inventa, i))->titulus, titulus))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior i64
rescripta (
    Mundus* m)
{
    redde indagatio_mensura(m->ix).rescripta;
}


/* ==================================================
 * Sectiones
 * ================================================== */

/* verba plana -> MATCH; nihil fallit syntaxi */
interior vacuum
_probare_verba (
    CredoContextus* c)
{
                Mundus* m;
                  Sors  s;
                   i32  i;
                   i32  k;
             character  buffer[XVI];
    constans character* signa =
        " \"*()-:^+.,'aZ9\xc3\xa9\xe2\x80\x94";   /* e acutum, linea */

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->ix);
    CREDO_CHORDA_AEQUALIS_LITERIS(indagatio_verba_formare(lit(
        "rhub tart", c->piscina), c->piscina), "\"rhub\"* \"tart\"*");
    CREDO_AEQUALIS_I32(indagatio_verba_formare(lit("   ", c->piscina),
        c->piscina).mensura, ZEPHYRUM);
    CREDO_AEQUALIS_I32(indagatio_verba_formare(lit("", c->piscina),
        c->piscina).mensura, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(indagatio_verba_formare(lit(
        "a\"b", c->piscina), c->piscina), "\"a\"\"b\"*");
    CREDO_CHORDA_AEQUALIS_LITERIS(indagatio_verba_formare(lit(
        "OR NOT", c->piscina), c->piscina), "\"OR\"* \"NOT\"*");

    /* operatores = textus: nulla quaestio fallit */
    (vacuum)rem_creare(m, "emere OR NOT rhubarbum");
    CREDO_NON_NIHIL(quaerere_simpliciter(m, "OR", c->piscina));
    CREDO_NON_NIHIL(quaerere_simpliciter(m, "\"", c->piscina));
    CREDO_NON_NIHIL(quaerere_simpliciter(m, "(rhub", c->piscina));
    CREDO_NON_NIHIL(quaerere_simpliciter(m, "\xe2\x80\x94",
        c->piscina));
    CREDO_NON_NIHIL(quaerere_simpliciter(m, "-", c->piscina));
    CREDO_NON_NIHIL(quaerere_simpliciter(m, "*", c->piscina));

    /* fuzz: MM verba fortuita ex signis */
    sors_seminare(&s, (i64)XLII, ZEPHYRUM);
    per (i = ZEPHYRUM; i < MM; i++)
    {
        i32 longitudo = sors_intra(&s, XII);

        per (k = ZEPHYRUM; k < longitudo; k++)
        {
            buffer[k] = signa[sors_intra(&s, (i32)strlen(signa))];
        }
        buffer[longitudo] = '\0';
        si (!quaerere_simpliciter(m, buffer, c->piscina))
        {
            imprimere("  verba fallentia: '%s' (%s)\n", buffer,
                indagatio_error(m->ix) ? indagatio_error(m->ix) : "?");
            CREDO_VERUM(FALSUM);
            frange;
        }
    }
}

/* aedificatio prima: pigra (quaestione prima), greges, excerpta */
interior vacuum
_probare_aedificationem (
    CredoContextus* c)
{
                        Mundus* m;
     constans IndagatioFructus* f;
        constans IndagatioGrex* g;
             IndagatioInventum* inv;
                        chorda  alpha;
                        chorda  anna;
                           Xar* epistulae;
                           i32  i;
                           b32  signatum;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->ix);
    alpha = rem_creare(m, "emere rhubarbum");
    (vacuum)rem_creare(m, "vocare matrem");
    (vacuum)agenda_notam_addere(m->agenda, alpha, fasti_dies(MMXXVI, X,
        XXI), lit("in foro", c->piscina));
    (vacuum)agenda_perficere(m->agenda, alpha, fasti_dies(MMXXVI, X,
        XXI));
    notam_scribere(m, "liber", "prima pagina de hortis");
    notam_scribere(m, "liber", "secunda pagina de rhubarbo crudo");
    anna = personae_hominem_creare(m->personae, lit("Anna", c->piscina),
        lit("Rossi", c->piscina));
    epistulae = xar_creare(c->piscina, (i32)magnitudo(chorda));
    *(chorda*)xar_addere(epistulae) = lit("anna@rhubarbum.it",
        c->piscina);
    (vacuum)personae_epistulas_ponere(m->personae, anna, epistulae);

    CREDO_FALSUM(indagatio_mensura(m->ix).aedificatus);
    f = quaerere_simpliciter(m, "rhubarb", c->piscina);
    CREDO_NECESSE_NON_NIHIL(f);
    CREDO_VERUM(indagatio_mensura(m->ix).aedificatus);
    /* res II + notae paginae II + persona I */
    CREDO_AEQUALIS_I32((i32)indagatio_mensura(m->ix).ordines, V);
    CREDO_AEQUALIS_I32(xar_numerus(f->greges), III);

    g = grex(f, "res");
    CREDO_NECESSE_NON_NIHIL(g);
    CREDO_CHORDA_AEQUALIS_LITERIS(g->genus, "Agenda");
    CREDO_AEQUALIS_I32(g->numerus, I);
    inv = (IndagatioInventum*)xar_obtinere(g->inventa, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(inv->titulus, "emere rhubarbum");
    CREDO_CHORDA_AEQUALIS_LITERIS(inv->contextus, "perfecta");
    CREDO_CHORDA_AEQUALIS(inv->semita, chorda_concatenare(lit("agenda/",
        c->piscina), alpha, c->piscina));

    /* clavis una, ordines plures: pagina secunda sola congruit */
    g = grex(f, "nota");
    CREDO_NECESSE_NON_NIHIL(g);
    CREDO_AEQUALIS_I32(g->numerus, I);
    inv = (IndagatioInventum*)xar_obtinere(g->inventa, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(inv->titulus,
        "liber - pagina 2 de 2");
    CREDO_CHORDA_AEQUALIS_LITERIS(inv->semita, "nota/liber?pagina=2");
    /* excerptum: pars signata = verbum congruens */
    signatum = FALSUM;
    per (i = ZEPHYRUM; i < xar_numerus(inv->excerptum); i++)
    {
        IndagatioPars* p = (IndagatioPars*)xar_obtinere(inv->excerptum,
            i);

        si (p->congruit)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS(p->textus, "rhubarbo");
            signatum = VERUM;
        }
    }
    CREDO_VERUM(signatum);

    /* epistula in corpore personae */
    g = grex(f, "homines");
    CREDO_NECESSE_NON_NIHIL(g);
    CREDO_VERUM(titulus_inventus(g, "Anna Rossi"));

    /* verba nulla, aut nihil congruens: greges nulli */
    f = quaerere_simpliciter(m, "  ", c->piscina);
    CREDO_NECESSE(f && xar_numerus(f->greges) == ZEPHYRUM);
    f = quaerere_simpliciter(m, "zzzqx", c->piscina);
    CREDO_NECESSE(f && xar_numerus(f->greges) == ZEPHYRUM);
    /* diacritica: 'rhubárbum' invenit */
    f = quaerere_simpliciter(m, "rhub\xc3\xa1rbum", c->piscina);
    CREDO_NECESSE(f && grex(f, "res"));
}

/* pigritia: acta sine quaestione nihil rescribunt; quaestio claves
 * sordidas solas rescribit */
interior vacuum
_probare_pigritiam (
    CredoContextus* c)
{
                       Mundus* m;
    constans IndagatioFructus* f;
                       chorda  alpha;
                       chorda  beta;
                          i64  ante;
                          s64  versio;
                          i32  i;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->ix);
    alpha  = rem_creare(m, "alpha");
    beta   = rem_creare(m, "beta cum rhubarbo");
    notam_scribere(m, "liber", "pagina prima");
    (vacuum)personae_hominem_creare(m->personae, lit("Anna",
        c->piscina),
        lit("Rossi", c->piscina));
    (vacuum)personae_hominem_creare(m->personae, lit("Marcus",
        c->piscina), lit("Aurelius", c->piscina));
    CREDO_NECESSE_NON_NIHIL(quaerere_simpliciter(m, "alpha",
        c->piscina));
    ante    = rescripta(m);
    versio  = indagatio_mensura(m->ix).versio;

    /* quaestio iterata sine actis: nihil rescriptum, versio eadem */
    CREDO_NECESSE_NON_NIHIL(quaerere_simpliciter(m, "alpha",
        c->piscina));
    CREDO_AEQUALIS_S64((s64)rescripta(m), (s64)ante);
    CREDO_AEQUALIS_S64(indagatio_mensura(m->ix).versio, versio);

    /* X nomina sine quaestione: nihil rescriptum */
    per (i = ZEPHYRUM; i < X; i++)
    {
        (vacuum)agenda_titulum_ponere(m->agenda, alpha, lit(i % II
            ? "alpha secunda" : "alpha prima", c->piscina));
    }
    CREDO_AEQUALIS_S64((s64)rescripta(m), (s64)ante);
    /* quaestio: clavis UNA (ordo unus) rescribitur */
    f = quaerere_simpliciter(m, "secunda", c->piscina);
    CREDO_AEQUALIS_S64((s64)rescripta(m), (s64)ante + I);
    CREDO_MAIOR_S64(indagatio_mensura(m->ix).versio, versio);
    CREDO_VERUM(titulus_inventus(grex(f, "res"), "alpha secunda"));
    f = quaerere_simpliciter(m, "prima", c->piscina);
    CREDO_FALSUM(titulus_inventus(grex(f, "res"), "alpha prima"));

    /* deleta: abit; revocata: redit */
    (vacuum)agenda_rem_delere(m->agenda, beta);
    f = quaerere_simpliciter(m, "rhubarbo", c->piscina);
    CREDO_NIHIL(grex(f, "res"));
    CREDO_VERUM(agenda_revocare(m->agenda));
    f = quaerere_simpliciter(m, "rhubarbo", c->piscina);
    CREDO_VERUM(titulus_inventus(grex(f, "res"), "beta cum rhubarbo"));

    /* spatium: pagina nova notae - ordines clavis (II) rescribuntur */
    ante = rescripta(m);
    notam_scribere(m, "liber", "pagina altera");
    f = quaerere_simpliciter(m, "altera", c->piscina);
    CREDO_AEQUALIS_S64((s64)rescripta(m), (s64)ante + II);
    CREDO_VERUM(titulus_inventus(grex(f, "nota"),
        "liber - pagina 2 de 2"));

    /* omnia: persona nova - genus totum (III) rescribitur */
    ante = rescripta(m);
    (vacuum)personae_hominem_creare(m->personae, lit("Tullius",
        c->piscina), lit("Cicero", c->piscina));
    f = quaerere_simpliciter(m, "Cicero", c->piscina);
    CREDO_AEQUALIS_S64((s64)rescripta(m), (s64)ante + III);
    CREDO_VERUM(titulus_inventus(grex(f, "homines"), "Tullius Cicero"));

    /* renovare sine quaestione (pulsus otiosus) */
    (vacuum)agenda_titulum_ponere(m->agenda, alpha, lit("alpha tertia",
        c->piscina));
    CREDO_AEQUALIS_I32(indagatio_renovare(m->ix), I);
    CREDO_AEQUALIS_I32(indagatio_renovare(m->ix), ZEPHYRUM);
}

/* greges: ordo, capacitas, paginae gregis unius */
interior vacuum
_probare_greges (
    CredoContextus* c)
{
                       Mundus* m;
    constans IndagatioFructus* f;
       constans IndagatioGrex* g;
                          i32  i;
                    character  buffer[LXIV];

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->ix);
    /* 'vinum' in titulo personae (gravius), in corpore rei et notae */
    (vacuum)personae_hominem_creare(m->personae, lit("Vinum",
        c->piscina), lit("Rubrum", c->piscina));
    (vacuum)agenda_notam_addere(m->agenda, rem_creare(m, "emere"),
        fasti_dies(MMXXVI, X, XXI), lit("vinum et panem et alia multa "
        "verba ut corpus longum sit", c->piscina));
    per (i = ZEPHYRUM; i < V; i++)
    {
        sprintf(buffer, "pagina %d vinum album", (integer)i);
        notam_scribere(m, "cella", buffer);
    }

    /* alphabetice: Agenda, Notae, Personae */
    f = quaerere(m, "vinum", NIHIL, ZEPHYRUM, II,
        INDAGATIO_ORDO_ALPHABETICUS, c->piscina);
    CREDO_NECESSE(f && xar_numerus(f->greges) == III);
    CREDO_CHORDA_AEQUALIS_LITERIS(((IndagatioGrex*)xar_obtinere(
        f->greges, ZEPHYRUM))->genus, "Agenda");
    CREDO_CHORDA_AEQUALIS_LITERIS(((IndagatioGrex*)xar_obtinere(
        f->greges, II))->genus, "Personae");
    /* capacitas II: notae V congruunt, II redduntur */
    g = grex(f, "nota");
    CREDO_NECESSE_NON_NIHIL(g);
    CREDO_AEQUALIS_I32(g->numerus, V);
    CREDO_AEQUALIS_I32(xar_numerus(g->inventa), II);

    /* optimus: persona (in titulo) prima */
    f = quaerere(m, "vinum", NIHIL, ZEPHYRUM, II,
        INDAGATIO_ORDO_OPTIMUS, c->piscina);
    CREDO_NECESSE(f && xar_numerus(f->greges) == III);
    CREDO_CHORDA_AEQUALIS_LITERIS(((IndagatioGrex*)xar_obtinere(
        f->greges, ZEPHYRUM))->conspectus, "homines");

    /* grex unus, paginae II: II, II, I, deinde nulla */
    per (i = ZEPHYRUM; i < IV; i++)
    {
        f = quaerere(m, "vinum", "nota", i, II,
            INDAGATIO_ORDO_ALPHABETICUS, c->piscina);
        CREDO_NECESSE(f && xar_numerus(f->greges) == I);
        g = (constans IndagatioGrex*)xar_obtinere(f->greges, ZEPHYRUM);
        CREDO_AEQUALIS_I32(g->numerus, V);
        CREDO_AEQUALIS_I32(xar_numerus(g->inventa), i < II ? II
            : i == II ? I : ZEPHYRUM);
    }
}

/* verificatio: index == reaedificatio post acta fortuita */
interior vacuum
_probare_verificationem (
    CredoContextus* c)
{
     Mundus* m;
        Xar* discrepantes;
        Xar* res;
       Sors  s;
        i32  i;
  character  buffer[LXIV];

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->ix);
    res = xar_creare(c->piscina, (i32)magnitudo(chorda));
    sors_seminare(&s, (i64)VII, ZEPHYRUM);
    per (i = ZEPHYRUM; i < CC; i++)
    {
        i32 actio = sors_intra(&s, VI);

        sprintf(buffer, "verbum%d alia%d", (integer)sors_intra(&s, XX),
            (integer)i);
        si (actio == ZEPHYRUM || xar_numerus(res) == ZEPHYRUM)
        {
            *(chorda*)xar_addere(res) =
                chorda_transcribere(rem_creare(m,
                buffer), c->piscina);
        }
        alioquin si (actio == I)
        {
            (vacuum)agenda_titulum_ponere(m->agenda, *(chorda*)
                xar_obtinere(res, sors_intra(&s,
                (i32)xar_numerus(res))),
                lit(buffer, c->piscina));
        }
        alioquin si (actio == II)
        {
            (vacuum)agenda_revocare(m->agenda);
        }
        alioquin si (actio == III)
        {
            notam_scribere(m, sors_intra(&s, II) ? "a" : "b", buffer);
        }
        alioquin si (actio == IV)
        {
            (vacuum)personae_hominem_creare(m->personae, lit(buffer,
                c->piscina), lit("X", c->piscina));
        }
        alioquin
        {
            (vacuum)quaerere_simpliciter(m, "verbum1", c->piscina);
        }
    }
    discrepantes = xar_creare(c->piscina, (i32)magnitudo(chorda));
    CREDO_AEQUALIS_I32(indagatio_verificare(m->ix, discrepantes),
        ZEPHYRUM);
    per (i = ZEPHYRUM; i < xar_numerus(discrepantes); i++)
    {
        chorda d = *(chorda*)xar_obtinere(discrepantes, i);

        imprimere("  discrepans: %.*s\n", (integer)d.mensura,
            (constans character*)d.datum);
    }
    CREDO_MAIOR_I32((i32)indagatio_mensura(m->ix).ordines, ZEPHYRUM);

    /* verificatio discrepantiam VIDET: auditore 'res' sublato, nomen
     * mutatum index non audit - res una nominatur */
    CREDO_NECESSE(xar_numerus(res) > ZEPHYRUM);
    CREDO_NECESSE(conspectus_auditorem_ponere(m->repertorium, "res",
        NIHIL, NIHIL));
    (vacuum)agenda_titulum_ponere(m->agenda, *(chorda*)xar_obtinere(res,
        ZEPHYRUM), lit("nomen ignotum indici", c->piscina));
    xar_vacare(discrepantes);
    CREDO_AEQUALIS_I32(indagatio_verificare(m->ix, discrepantes), I);
    CREDO_NECESSE(xar_numerus(discrepantes) == I);
    CREDO_CHORDA_AEQUALIS(*(chorda*)xar_obtinere(discrepantes,
        ZEPHYRUM),
        chorda_concatenare(lit("res/", c->piscina),
        *(chorda*)xar_obtinere(
        res, ZEPHYRUM), c->piscina));
}

/* memoria: cycli (actum tituli, quaestio) - arenae sordium et operis
 * post exhaustionem redduntur. Genus 'titulus' (actum ultimum ex
 * volumine): nullum documentum in memoria crescit (agenda crescebat
 * ~II KiB per actum - strepitus mensuram obruebat). */
interior i64
vestigium (vacuum)
{
    MemoriaProcessus mp;

    redde memoria_processus_legere(ZEPHYRUM, &mp) ? mp.vestigium
        : ZEPHYRUM;
}

interior vacuum
cyclos_currere (
    Mundus* m,
       i32  n,
       i32  initium)
{
     Piscina* p;
         i32  i;
   character  genus[XXXII];
   character  textus[XXXII];

    p = piscina_generare_dynamicum("probatio_cycli", IV * MXXIV);
    per (i = ZEPHYRUM; p && i < n; i++)
    {
        sprintf(genus, "titulus/t%d/textus", (integer)((initium + i)
            % L));
        sprintf(textus, "titulus %d", (integer)(initium + i));
        (vacuum)volumen_actum_appendere(m->volumen, genus, lit(textus,
            p));
        (vacuum)quaerere_simpliciter(m, "titulus", p);
        piscina_vacare(p);
    }
    si (p)
    {
        piscina_destruere(p);
    }
}

interior vacuum
_probare_memoriam (
    CredoContextus* c)
{
    Mundus* m;
       i64  v1;
       i64  v2;
       i32  i;
 character  clavis[XVI];

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->ix);
    per (i = ZEPHYRUM; i < L; i++)
    {
        sprintf(clavis, "t%d", (integer)i);
        *(chorda*)xar_addere(m->tituli) = lit(clavis, c->piscina);
    }
    cyclos_currere(m, D, ZEPHYRUM);
    v1 = vestigium();
    cyclos_currere(m, MMM, D);
    v2 = vestigium();
    v2 = v2 > v1 ? v2 - v1 : ZEPHYRUM;
    imprimere("memoria: cycli MMM %ld KiB\n", (longus)(v2 / MXXIV));
    /* mensuratum 2026-10-21: sanum 0.9-1.8 MiB (appendere solum
     * 0.5-0.9); plantae infra in worklog. Limes V MiB. */
    CREDO_MINOR_I64(v2, (i64)(V * MXXIV * MXXIV));
    CREDO_AEQUALIS_I32((i32)indagatio_mensura(m->ix).ordines, L);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "verba",        _probare_verba,           mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "aedificatio",  _probare_aedificationem,  mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "pigritia",     _probare_pigritiam,       mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "greges",       _probare_greges,          mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "verificatio",  _probare_verificationem,  mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "memoria",      _probare_memoriam,        mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("indagatio", SECTIONES);
}

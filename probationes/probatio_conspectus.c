/* probatio_conspectus.c - CONSPECTUS (proiectiones D4a): declaratio,
 * lectio, irritatio per fontes (omnia, per-rem, spatium, pura),
 * dependentiae, verificatio, tectum, memoria.
 *
 * Conspectus FIXI (probatio sola; nullus in applicatione):
 *   dies      agenda per-rem, personae omnia, spatium dies/{clavis},
 *             pura liturgia - res diei, perfectae, numerus personarum,
 *             acta sub spatio diei
 *   hebdomas  sine fontibus: dies VII legit (dependentiae)
 *   sanctus   pura sola
 *   circulus  se ipsum legit (circulus recusatur)
 *   falsa     pura PROFESSA, agendam tamen legit (irritatio omissa -
 *             verificatio eam invenire debet) */

#include "latina.h"
#include "credo.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "xar.h"
#include "volumen.h"
#include "scrinium.h"
#include "moneta.h"
#include "fasti.h"
#include "agenda.h"
#include "personae.h"
#include "conspectus.h"
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
                    i32  numerator_agendae;
                    i32  numerator_personarum;
                    i32  computationes;      /* omnium conspectuum */
                    i32  computationes_diei;
} Mundus;

interior chorda
lit (
    constans character* s,
               Piscina* piscina)
{
    redde chorda_ex_literis(s, piscina);
}

interior Dies
dies_ex_clavi (
    chorda clavis)
{
         Dies d;
    character buffer[XVI];
      integer annus;
      integer mensis;
      integer dies;

    memset(&d, ZEPHYRUM, magnitudo(Dies));
    si (clavis.mensura != X)
    {
        redde d;
    }
    memcpy(buffer, clavis.datum, X);
    buffer[X] = '\0';
    si (sscanf(buffer, "%d-%d-%d", &annus, &mensis, &dies) == III)
    {
        d = fasti_dies((s32)annus, (s32)mensis, (s32)dies);
    }
    redde d;
}

interior chorda
clavis_diei (
       Dies  d,
    Piscina* piscina)
{
    character buffer[XVI];

    sprintf(buffer, "%04d-%02d-%02d", (integer)d.annus,
        (integer)d.mensis, (integer)d.dies);
    redde chorda_ex_literis(buffer, piscina);
}

interior chorda
diem (
        s32  dies,
    Piscina* piscina)
{
    redde clavis_diei(fasti_dies(MMXXVI, X, dies), piscina);
}

interior chorda
vacua_probationis (vacuum)
{
    chorda c;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    redde c;
}


/* ==================================================
 * Conspectus 'dies'
 * ================================================== */

nomen structura {
    chorda  clavis;
       Xar* res;         /* chorda: tituli rerum diei (copiati) */
       Xar* perfectae;   /* chorda: tituli perfectarum eo die */
       i32  personae;
       i32  acta_spatii;
} DiesConspectus;

interior vacuum*
dies_computare (
                 vacuum* ctx,
    ConspectusRegistrum* repertorium,
                 chorda  clavis,
                Piscina* piscina)
{
                Mundus* m;
        DiesConspectus* v;
                  Dies  d;
                   Xar* l;
                   i32  i;
             character  praefixum[XXXII];
    ScriniumEnuntiatum* e;

    (vacuum)repertorium;
    m = (Mundus*)ctx;
    m->computationes++;
    m->computationes_diei++;
    d = dies_ex_clavi(clavis);
    si (!fasti_dies_valida(d))
    {
        redde NIHIL;
    }
    v = (DiesConspectus*)piscina_allocare(piscina,
        magnitudo(DiesConspectus));
    v->clavis     = chorda_transcribere(clavis, piscina);
    v->res        = xar_creare(piscina, (i32)magnitudo(chorda));
    v->perfectae  = xar_creare(piscina, (i32)magnitudo(chorda));
    l             = agenda_res_diei(m->agenda, d, piscina);
    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        constans AgendaRes* r = *(constans AgendaRes**)xar_obtinere(l,
            i);

        *(chorda*)xar_addere(v->res) = chorda_transcribere(r->titulus,
            piscina);
    }
    l = agenda_perfectae_die(m->agenda, d, piscina);
    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        constans AgendaRes* r = *(constans AgendaRes**)xar_obtinere(l,
            i);

        *(chorda*)xar_addere(v->perfectae) = chorda_transcribere(
            r->titulus, piscina);
    }
    v->personae = personae_numerus(m->personae);
    /* intervallum [praefixum/, praefixum0) per indicem acta(genus, seq)
     * - LIKE omnia acta perscrutabatur (probatio X milia computationum
     * super acta crescentia: VII s) */
    sprintf(praefixum, "dies/%.*s/", (integer)clavis.mensura,
        (constans character*)clavis.datum);
    e = scrinium_praeparare(volumen_scrinium(m->volumen),
        "SELECT count(*) FROM acta WHERE genus >= ?1 AND genus < ?2");
    si (e)
    {
        chorda infima   = chorda_ex_literis(praefixum, piscina);
        chorda suprema  = chorda_transcribere(infima, piscina);

        suprema.datum[suprema.mensura - I] = '0';   /* '/' + I */
        (vacuum)scrinium_ligare_textum(e, I, infima);
        (vacuum)scrinium_ligare_textum(e, II, suprema);
        si (scrinium_gradi(e) == SCRINIUM_ORDO)
        {
            v->acta_spatii = (i32)scrinium_columna_numerus(e, ZEPHYRUM);
        }
        scrinium_finire(e);
    }
    redde v;
}

interior chorda
dies_codificare (
             vacuum* ctx,
    constans vacuum* conspectus,
            Piscina* piscina)
{
    constans DiesConspectus* v;
                     chorda  c;
                        i32  i;
                  character  buffer[LXXX];

    (vacuum)ctx;
    v = (constans DiesConspectus*)conspectus;
    sprintf(buffer,
        "<dies clavis=\"%.*s\" personae=\"%u\" acta=\"%u\">",
        (integer)v->clavis.mensura,
        (constans character*)v->clavis.datum,
        (insignatus integer)v->personae,
        (insignatus integer)v->acta_spatii);
    c = chorda_ex_literis(buffer, piscina);
    per (i = ZEPHYRUM; i < xar_numerus(v->res); i++)
    {
        c = chorda_concatenare(c, lit("<res>", piscina), piscina);
        c = chorda_concatenare(c, *(chorda*)xar_obtinere(v->res, i),
            piscina);
        c = chorda_concatenare(c, lit("</res>", piscina), piscina);
    }
    per (i = ZEPHYRUM; i < xar_numerus(v->perfectae); i++)
    {
        c = chorda_concatenare(c, lit("<perfecta>", piscina), piscina);
        c = chorda_concatenare(c, *(chorda*)xar_obtinere(v->perfectae,
            i), piscina);
        c = chorda_concatenare(c, lit("</perfecta>", piscina), piscina);
    }
    redde chorda_concatenare(c, lit("</dies>", piscina), piscina);
}

/* per-rem agendae: claves quas res tangit (dies posita, perfectio) */
interior i32
agenda_numerus (
    constans vacuum* documentum)
{
    redde agenda_numerus_rerum((constans AgendaDocumentum*)documentum);
}

interior chorda
agenda_id (
    constans vacuum* documentum,
                i32  index,
            Piscina* piscina)
{
    (vacuum)piscina;
    redde agenda_res_index((constans AgendaDocumentum*)documentum,
        index)->id;
}

/* vestigium: campi quos 'dies' legit (titulus, dies, perfectio) */
interior chorda
agenda_vestigium (
    constans vacuum* documentum,
                i32  index,
            Piscina* piscina)
{
    constans AgendaRes* r;
             character  buffer[LXIV];

    r = agenda_res_index((constans AgendaDocumentum*)documentum, index);
    sprintf(buffer, "|%d|%04d-%02d-%02d|%d|%04d-%02d-%02d",
        (integer)r->dies_posita, (integer)r->dies.annus,
        (integer)r->dies.mensis, (integer)r->dies.dies,
        (integer)r->perfecta, (integer)r->dies_perfectionis.annus,
        (integer)r->dies_perfectionis.mensis,
        (integer)r->dies_perfectionis.dies);
    redde chorda_concatenare(r->titulus, chorda_ex_literis(buffer,
        piscina), piscina);
}

interior vacuum
agenda_claves (
             vacuum* ctx,
    constans vacuum* documentum,
                i32  index,
                Xar* claves,
            Piscina* piscina)
{
    constans AgendaRes* r;

    (vacuum)ctx;
    r = agenda_res_index((constans AgendaDocumentum*)documentum, index);
    si (r->dies_posita)
    {
        *(chorda*)xar_addere(claves) = clavis_diei(r->dies, piscina);
    }
    si (r->perfecta)
    {
        *(chorda*)xar_addere(claves) = clavis_diei(r->dies_perfectionis,
            piscina);
    }
}


/* ==================================================
 * Conspectus 'hebdomas', 'sanctus', 'circulus', 'falsa'
 * ================================================== */

/* hebdomas: summa rerum dierum VII a clave (dies conspectus legit) */
interior vacuum*
hebdomas_computare (
                 vacuum* ctx,
    ConspectusRegistrum* repertorium,
                 chorda  clavis,
                Piscina* piscina)
{
    Mundus* m;
       i32* summa;
      Dies  d;
       i32  i;

    m = (Mundus*)ctx;
    m->computationes++;
    summa   = (i32*)piscina_allocare(piscina, magnitudo(i32));
    *summa  = ZEPHYRUM;
    d       = dies_ex_clavi(clavis);
    per (i = ZEPHYRUM; i < VII; i++)
    {
        constans DiesConspectus* v;

        v = (constans DiesConspectus*)conspectus_legere(repertorium,
            "dies", clavis_diei(fasti_addere_dies(d, (s32)i), piscina));
        si (v)
        {
            *summa += (i32)xar_numerus(v->res);
        }
    }
    redde summa;
}

interior chorda
numerum_codificare (
             vacuum* ctx,
    constans vacuum* conspectus,
            Piscina* piscina)
{
    character buffer[XXXII];

    (vacuum)ctx;
    sprintf(buffer, "<n>%u</n>",
        (insignatus integer)*(constans i32*)conspectus);
    redde chorda_ex_literis(buffer, piscina);
}

/* sanctus: functio clavis solius (dies mensis) */
interior vacuum*
sanctus_computare (
                 vacuum* ctx,
    ConspectusRegistrum* repertorium,
                 chorda  clavis,
                Piscina* piscina)
{
    Mundus* m;
       i32* n;

    (vacuum)repertorium;
    m = (Mundus*)ctx;
    m->computationes++;
    n   = (i32*)piscina_allocare(piscina, magnitudo(i32));
    *n  = (i32)dies_ex_clavi(clavis).dies;
    redde n;
}

/* circulus: se ipsum legit - lectio interior NIHIL reddere debet */
interior vacuum*
circulus_computare (
                 vacuum* ctx,
    ConspectusRegistrum* repertorium,
                 chorda  clavis,
                Piscina* piscina)
{
    i32* n;

    (vacuum)ctx;
    n   = (i32*)piscina_allocare(piscina, magnitudo(i32));
    *n  = conspectus_legere(repertorium, "circulus", clavis) ? I : II;
    redde n;
}

/* falsa: pura professa, agendam tamen legit */
interior vacuum*
falsa_computare (
                 vacuum* ctx,
    ConspectusRegistrum* repertorium,
                 chorda  clavis,
                Piscina* piscina)
{
    Mundus* m;
       i32* n;

    (vacuum)repertorium;
    (vacuum)clavis;
    m   = (Mundus*)ctx;
    n   = (i32*)piscina_allocare(piscina, magnitudo(i32));
    *n  = agenda_numerus_rerum(m->agenda);
    redde n;
}


/* ==================================================
 * Declarationes fixae
 * ================================================== */

hic_manens constans character* constans DECLARATIO_DIEI =
    "<conspectus nomen=\"dies\" versio=\"1\">\n"
    "  <clavis typus=\"dies\"/>\n"
    "  <fons documentum=\"agenda\" irritat=\"per-rem\"/>\n"
    "  <fons documentum=\"personae\" irritat=\"omnia\"/>\n"
    "  <fons spatium=\"dies/{clavis}\"/>\n"
    "  <fons pura=\"liturgia\"/>\n"
    "</conspectus>\n";

hic_manens constans character* constans DECLARATIO_HEBDOMADIS =
    "<conspectus nomen=\"hebdomas\" versio=\"1\">\n"
    "  <clavis typus=\"dies\"/>\n"
    "</conspectus>\n";

hic_manens constans character* constans DECLARATIO_SANCTI =
    "<conspectus nomen=\"sanctus\" versio=\"1\">\n"
    "  <clavis typus=\"dies\"/>\n"
    "  <fons pura=\"liturgia\"/>\n"
    "</conspectus>\n";

hic_manens constans character* constans DECLARATIO_CIRCULI =
    "<conspectus nomen=\"circulus\" versio=\"1\">\n"
    "  <clavis typus=\"dies\"/>\n"
    "</conspectus>\n";

hic_manens constans character* constans DECLARATIO_FALSAE =
    "<conspectus nomen=\"falsa\" versio=\"1\">\n"
    "  <clavis typus=\"dies\"/>\n"
    "  <fons pura=\"nihil\"/>\n"
    "</conspectus>\n";

interior b32
declarare (
    ConspectusRegistrum* r,
     constans character* textus,
                Piscina* piscina)
{
    Xar* vitia;

    redde conspectus_declarare(r, chorda_ex_literis(textus, piscina),
        &vitia);
}

interior ConspectusOpera
opera (
    vacuum* (*computare)(vacuum*, ConspectusRegistrum*, chorda,
                         Piscina*),
    chorda  (*codificare)(vacuum*, constans vacuum*, Piscina*),
    vacuum* ctx)
{
    ConspectusOpera o;

    o.computare   = computare;
    o.codificare  = codificare;
    o.ctx         = ctx;
    redde o;
}

interior ConspectusPerRem
per_rem_agendae (vacuum)
{
    ConspectusPerRem p;

    p.numerus    = agenda_numerus;
    p.id         = agenda_id;
    p.vestigium  = agenda_vestigium;
    p.claves     = agenda_claves;
    p.ctx        = NIHIL;
    redde p;
}

/* registrum plenum: omnes conspectus fixi declarati, opera posita,
 * documenta ligata, paratum. tectum 0 = ordinarium */
interior ConspectusRegistrum*
registrum_plenum (
            Mundus* m,
           Piscina* piscina,
    memoriae_index  tectum)
{
    ConspectusRegistrum* r;
                    Xar* vitia;

    r = conspectus_registrum_creare(piscina, m->intern, m->volumen,
        tectum);
    si (!r)
    {
        redde NIHIL;
    }
    si (   !declarare(r, DECLARATIO_DIEI, piscina)
        || !declarare(r, DECLARATIO_HEBDOMADIS, piscina)
        || !declarare(r, DECLARATIO_SANCTI, piscina)
        || !declarare(r, DECLARATIO_CIRCULI, piscina)
        || !declarare(r, DECLARATIO_FALSAE, piscina))
    {
        redde NIHIL;
    }
    (vacuum)conspectus_opera_ponere(r, "dies", opera(dies_computare,
        dies_codificare, m));
    (vacuum)conspectus_opera_ponere(r, "hebdomas", opera(
        hebdomas_computare, numerum_codificare, m));
    (vacuum)conspectus_opera_ponere(r, "sanctus",
        opera(sanctus_computare,
        numerum_codificare, m));
    (vacuum)conspectus_opera_ponere(r, "circulus", opera(
        circulus_computare, numerum_codificare, m));
    (vacuum)conspectus_opera_ponere(r, "falsa", opera(falsa_computare,
        numerum_codificare, m));
    (vacuum)conspectus_per_rem_ponere(r, "dies", "agenda",
        per_rem_agendae());
    (vacuum)conspectus_documentum_ligare(r, "agenda", m->agenda);
    (vacuum)conspectus_documentum_ligare(r, "personae", m->personae);
    si (!conspectus_parare(r, &vitia))
    {
        redde NIHIL;
    }
    redde r;
}

interior vacuum
mundum_parare (
    CredoContextus* c)
{
    Mundus* m;

    m = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(m, ZEPHYRUM, magnitudo(Mundus));
    c->datum = m;
    m->intern = internamentum_creare(c->piscina);
    m->volumen = volumen_temporarium(c->piscina, "probatio_conspectus");
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
    m->repertorium = registrum_plenum(m, c->piscina, ZEPHYRUM);
}

interior vacuum
mundum_purgare (
    CredoContextus* c)
{
    Mundus* m;

    m = (Mundus*)c->datum;
    si (m && m->volumen)
    {
        volumen_claudere(m->volumen);
    }
}


/* ==================================================
 * Auxilia
 * ================================================== */

interior constans DiesConspectus*
dies_legere (
     Mundus* m,
        s32  dies,
    Piscina* piscina)
{
    redde (constans DiesConspectus*)conspectus_legere(m->repertorium,
        "dies", diem(dies, piscina));
}

interior ConspectusStatus
status_diei (
     Mundus* m,
        s32  dies,
    Piscina* piscina)
{
    redde conspectus_status(m->repertorium, "dies", diem(dies,
        piscina));
}

/* dies 10..20 legere (omnes recentes fiunt) */
interior vacuum
dies_legere_omnes (
     Mundus* m,
    Piscina* piscina)
{
    s32 d;

    per (d = X; d <= XX; d++)
    {
        (vacuum)dies_legere(m, d, piscina);
    }
}

interior chorda
rem_creare_die (
                Mundus* m,
    constans character* titulus,
                   s32  dies,
               Piscina* piscina)
{
    chorda id;
      Dies d;

    id  = agenda_rem_creare(m->agenda, lit(titulus, piscina));
    d   = fasti_dies(MMXXVI, X, dies);
    (vacuum)agenda_diem_ponere(m->agenda, id, &d);
    redde id;
}


/* ==================================================
 * Sectiones
 * ================================================== */

interior vacuum
_probare_declarationem (
    CredoContextus* c)
{
                 Mundus* m;
    ConspectusRegistrum* r;
                    Xar* vitia;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->volumen);
    r = conspectus_registrum_creare(c->piscina, m->intern, m->volumen,
        ZEPHYRUM);
    CREDO_NECESSE_NON_NIHIL(r);

    /* sana */
    CREDO_VERUM(conspectus_declarare(r, lit(DECLARATIO_DIEI,
        c->piscina),
        &vitia));
    CREDO_AEQUALIS_I32(xar_numerus(vitia), ZEPHYRUM);

    /* nomen bis */
    CREDO_FALSUM(conspectus_declarare(r, lit(DECLARATIO_DIEI,
        c->piscina), &vitia));
    CREDO_MAIOR_I32(xar_numerus(vitia), ZEPHYRUM);
    CREDO_CHORDA_CONTINET(*(chorda*)xar_obtinere(vitia, ZEPHYRUM),
        lit("bis", c->piscina));

    /* documentum sine irritat */
    CREDO_FALSUM(conspectus_declarare(r, lit(
        "<conspectus nomen=\"a\" versio=\"1\"><clavis typus=\"dies\"/>"
        "<fons documentum=\"agenda\"/></conspectus>", c->piscina),
        &vitia));
    CREDO_CHORDA_CONTINET(*(chorda*)xar_obtinere(vitia, ZEPHYRUM),
        lit("irritat", c->piscina));

    /* spatium sine {clavis} */
    CREDO_FALSUM(conspectus_declarare(r, lit(
        "<conspectus nomen=\"b\" versio=\"1\"><clavis typus=\"dies\"/>"
        "<fons spatium=\"dies\"/></conspectus>", c->piscina), &vitia));
    CREDO_CHORDA_CONTINET(*(chorda*)xar_obtinere(vitia, ZEPHYRUM),
        lit("{clavis}", c->piscina));

    /* fons generum duorum */
    CREDO_FALSUM(conspectus_declarare(r, lit(
        "<conspectus nomen=\"d\" versio=\"1\"><clavis typus=\"dies\"/>"
        "<fons documentum=\"agenda\" irritat=\"omnia\" pura=\"x\"/>"
        "</conspectus>", c->piscina), &vitia));
    CREDO_CHORDA_CONTINET(*(chorda*)xar_obtinere(vitia, ZEPHYRUM),
        lit("genere uno", c->piscina));

    /* irritat sine documento */
    CREDO_FALSUM(conspectus_declarare(r, lit(
        "<conspectus nomen=\"e\" versio=\"1\"><clavis typus=\"dies\"/>"
        "<fons spatium=\"dies/{clavis}\" irritat=\"omnia\"/>"
        "</conspectus>", c->piscina), &vitia));
    CREDO_CHORDA_CONTINET(*(chorda*)xar_obtinere(vitia, ZEPHYRUM),
        lit("irritat", c->piscina));

    /* canon: attributum ignotum, clavis absens */
    CREDO_FALSUM(conspectus_declarare(r, lit(
        "<conspectus nomen=\"f\" versio=\"1\"><clavis typus=\"dies\"/>"
        "<fons ignotum=\"x\"/></conspectus>", c->piscina), &vitia));
    CREDO_MAIOR_I32(xar_numerus(vitia), ZEPHYRUM);
    CREDO_FALSUM(conspectus_declarare(r, lit(
        "<conspectus nomen=\"g\" versio=\"1\"/>", c->piscina), &vitia));
    CREDO_MAIOR_I32(xar_numerus(vitia), ZEPHYRUM);

    /* STML fractum */
    CREDO_FALSUM(conspectus_declarare(r, lit("<conspectus", c->piscina),
        &vitia));
    CREDO_MAIOR_I32(xar_numerus(vitia), ZEPHYRUM);
}

interior vacuum
_probare_parare (
    CredoContextus* c)
{
                 Mundus* m;
    ConspectusRegistrum* r;
                    Xar* vitia;
                 chorda  omnia;
                    i32  i;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->volumen);
    r = conspectus_registrum_creare(c->piscina, m->intern, m->volumen,
        ZEPHYRUM);
    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_VERUM(declarare(r, DECLARATIO_DIEI, c->piscina));

    /* opera absentia, opera sine declaratione, per-rem absens,
     * documentum non ligatum: omnia nominantur */
    (vacuum)conspectus_opera_ponere(r, "ignotum",
        opera(sanctus_computare,
        numerum_codificare, m));
    CREDO_FALSUM(conspectus_parare(r, &vitia));
    omnia = lit("", c->piscina);
    per (i = ZEPHYRUM; i < xar_numerus(vitia); i++)
    {
        omnia = chorda_concatenare(omnia, *(chorda*)xar_obtinere(vitia,
            i), c->piscina);
    }
    CREDO_CHORDA_CONTINET(omnia,
        lit("opera", c->piscina));
    CREDO_CHORDA_CONTINET(omnia,
        lit("ignotum", c->piscina));
    CREDO_CHORDA_CONTINET(omnia,
        lit("per-rem", c->piscina));
    CREDO_CHORDA_CONTINET(omnia,
        lit("agenda", c->piscina));

    /* lectio ante parationem: NIHIL */
    CREDO_NIHIL(conspectus_legere(r, "dies", diem(XIV, c->piscina)));

    /* registrum plenum paratur */
    CREDO_NECESSE_NON_NIHIL(m->repertorium);
}

interior vacuum
_probare_lectionem (
    CredoContextus* c)
{
                     Mundus* m;
    constans DiesConspectus* v;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->repertorium);
    (vacuum)rem_creare_die(m, "panem emere", XIV, c->piscina);

    CREDO_AEQUALIS_I32((i32)status_diei(m, XIV, c->piscina),
        (i32)CONSPECTUS_ABSENS);
    v = dies_legere(m, XIV, c->piscina);
    CREDO_NECESSE_NON_NIHIL(v);
    CREDO_AEQUALIS_I32(xar_numerus(v->res), I);
    CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(v->res,
        ZEPHYRUM), "panem emere");
    CREDO_AEQUALIS_I32(m->computationes_diei, I);
    CREDO_AEQUALIS_I32((i32)status_diei(m, XIV, c->piscina),
        (i32)CONSPECTUS_RECENS);

    /* lectio iterata: ex cache */
    CREDO_AEQUALIS_PTR(dies_legere(m, XIV, c->piscina), v);
    CREDO_AEQUALIS_I32(m->computationes_diei, I);

    /* clavis typo non congruens, nomen ignotum */
    CREDO_NIHIL(conspectus_legere(m->repertorium, "dies", lit(
        "2026-13-45", c->piscina)));
    CREDO_NIHIL(conspectus_legere(m->repertorium, "dies", lit("heri?",
        c->piscina)));
    CREDO_NIHIL(conspectus_legere(m->repertorium, "ignotum", diem(XIV,
        c->piscina)));
}

interior vacuum
_probare_per_rem (
    CredoContextus* c)
{
                     Mundus* m;
                     chorda  a;
                     chorda  b;
                       Dies  d;
    constans DiesConspectus* v;
                        s32  k;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->repertorium);
    a = rem_creare_die(m, "alpha", XIV, c->piscina);
    b = rem_creare_die(m, "beta", XV, c->piscina);
    per (k = XIII; k <= XVII; k++)
    {
        CREDO_NON_NIHIL(dies_legere(m, k, c->piscina));
    }

    /* alpha 14 -> 16: 14 et 16 stala, ceteri recentes */
    d = fasti_dies(MMXXVI, X, XVI);
    CREDO_MAIOR_S64(agenda_diem_ponere(m->agenda, a, &d), ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)status_diei(m, XIV, c->piscina),
        (i32)CONSPECTUS_STALUM);
    CREDO_AEQUALIS_I32((i32)status_diei(m, XVI, c->piscina),
        (i32)CONSPECTUS_STALUM);
    CREDO_AEQUALIS_I32((i32)status_diei(m, XIII, c->piscina),
        (i32)CONSPECTUS_RECENS);
    CREDO_AEQUALIS_I32((i32)status_diei(m, XV, c->piscina),
        (i32)CONSPECTUS_RECENS);
    CREDO_AEQUALIS_I32((i32)status_diei(m, XVII, c->piscina),
        (i32)CONSPECTUS_RECENS);
    v = dies_legere(m, XIV, c->piscina);
    CREDO_AEQUALIS_I32(xar_numerus(v->res), ZEPHYRUM);
    v = dies_legere(m, XVI, c->piscina);
    CREDO_AEQUALIS_I32(xar_numerus(v->res), I);

    /* beta (15) die 17 perfecta: 15 et 17, non 14 aut 16 */
    CREDO_MAIOR_S64(agenda_perficere(m->agenda, b, fasti_dies(MMXXVI, X,
        XVII)), ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)status_diei(m, XV, c->piscina),
        (i32)CONSPECTUS_STALUM);
    CREDO_AEQUALIS_I32((i32)status_diei(m, XVII, c->piscina),
        (i32)CONSPECTUS_STALUM);
    CREDO_AEQUALIS_I32((i32)status_diei(m, XIV, c->piscina),
        (i32)CONSPECTUS_RECENS);
    CREDO_AEQUALIS_I32((i32)status_diei(m, XVI, c->piscina),
        (i32)CONSPECTUS_RECENS);
    v = dies_legere(m, XVII, c->piscina);
    CREDO_AEQUALIS_I32(xar_numerus(v->perfectae), I);

    /* revocare: perfectio tollitur - 15 et 17 iterum */
    (vacuum)dies_legere(m, XV, c->piscina);
    CREDO_VERUM(agenda_revocare(m->agenda));
    CREDO_AEQUALIS_I32((i32)status_diei(m, XV, c->piscina),
        (i32)CONSPECTUS_STALUM);
    CREDO_AEQUALIS_I32((i32)status_diei(m, XVII, c->piscina),
        (i32)CONSPECTUS_STALUM);
    v = dies_legere(m, XVII, c->piscina);
    CREDO_AEQUALIS_I32(xar_numerus(v->perfectae), ZEPHYRUM);

    /* res sine die: nihil stalum */
    (vacuum)dies_legere(m, XV, c->piscina);
    CREDO_CHORDA_NON_VACUA(agenda_rem_creare(m->agenda, lit("gamma",
        c->piscina)));
    per (k = XIII; k <= XVII; k++)
    {
        CREDO_AEQUALIS_I32((i32)status_diei(m, k, c->piscina),
            (i32)CONSPECTUS_RECENS);
    }

    /* titulus mutatus, claves eaedem: vestigium mutatum - 16 solus */
    CREDO_MAIOR_S64(agenda_titulum_ponere(m->agenda, a,
        lit("alpha nova",
        c->piscina)), ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)status_diei(m, XVI, c->piscina),
        (i32)CONSPECTUS_STALUM);
    CREDO_AEQUALIS_I32((i32)status_diei(m, XV, c->piscina),
        (i32)CONSPECTUS_RECENS);
    v = dies_legere(m, XVI, c->piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(v->res,
        ZEPHYRUM), "alpha nova");
}

interior vacuum
_probare_omnia (
    CredoContextus* c)
{
                     Mundus* m;
    constans DiesConspectus* v;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->repertorium);
    v = dies_legere(m, XIV, c->piscina);
    CREDO_AEQUALIS_I32(v->personae, ZEPHYRUM);
    (vacuum)dies_legere(m, XV, c->piscina);

    /* spatium alienum cum praefixo eodem: nihil */
    CREDO_MAIOR_S64(volumen_actum_appendere(m->volumen,
        "personaealia/x", lit("<x/>", c->piscina)), ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)status_diei(m, XIV, c->piscina),
        (i32)CONSPECTUS_RECENS);

    CREDO_CHORDA_NON_VACUA(personae_hominem_creare(m->personae, lit(
        "Anna", c->piscina), lit("Rossi", c->piscina)));
    CREDO_AEQUALIS_I32((i32)status_diei(m, XIV, c->piscina),
        (i32)CONSPECTUS_STALUM);
    CREDO_AEQUALIS_I32((i32)status_diei(m, XV, c->piscina),
        (i32)CONSPECTUS_STALUM);
    v = dies_legere(m, XIV, c->piscina);
    CREDO_AEQUALIS_I32(v->personae, I);

    /* revocare personae quoque */
    CREDO_VERUM(personae_revocare(m->personae));
    CREDO_AEQUALIS_I32((i32)status_diei(m, XIV, c->piscina),
        (i32)CONSPECTUS_STALUM);
    v = dies_legere(m, XIV, c->piscina);
    CREDO_AEQUALIS_I32(v->personae, ZEPHYRUM);
}

interior vacuum
_probare_spatium (
    CredoContextus* c)
{
                     Mundus* m;
    constans DiesConspectus* v;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->repertorium);
    (vacuum)dies_legere(m, XIX, c->piscina);
    (vacuum)dies_legere(m, XX, c->piscina);
    (vacuum)dies_legere(m, I, c->piscina);

    /* actum sub spatio diei (scriptor quilibet: volumen ipsum) */
    CREDO_MAIOR_S64(volumen_actum_appendere(m->volumen,
        "dies/2026-10-19/paginae/1/scriba", lit("<mutatio/>",
        c->piscina)), ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)status_diei(m, XIX, c->piscina),
        (i32)CONSPECTUS_STALUM);
    CREDO_AEQUALIS_I32((i32)status_diei(m, XX, c->piscina),
        (i32)CONSPECTUS_RECENS);
    CREDO_AEQUALIS_I32((i32)status_diei(m, I, c->piscina),
        (i32)CONSPECTUS_RECENS);
    v = dies_legere(m, XIX, c->piscina);
    CREDO_AEQUALIS_I32(v->acta_spatii, I);

    /* plagula sub spatio diei (index libri) */
    CREDO_VERUM(volumen_plagulam_condere(m->volumen, lit(
        "dies/2026-10-20/paginae", c->piscina), lit("<paginae/>",
        c->piscina), "genita"));
    CREDO_AEQUALIS_I32((i32)status_diei(m, XX, c->piscina),
        (i32)CONSPECTUS_STALUM);
    CREDO_AEQUALIS_I32((i32)status_diei(m, XIX, c->piscina),
        (i32)CONSPECTUS_RECENS);

    /* spatium alienum: nihil */
    (vacuum)dies_legere(m, XX, c->piscina);
    CREDO_MAIOR_S64(volumen_actum_appendere(m->volumen,
        "diesalia/2026-10-20/x", lit("<x/>", c->piscina)), ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)status_diei(m, XX, c->piscina),
        (i32)CONSPECTUS_RECENS);

    /* renovare: cursor ad finem */
    CREDO_AEQUALIS_S64(conspectus_renovare(m->repertorium),
        volumen_summa_actorum(m->volumen));
}

interior vacuum
_probare_puram (
    CredoContextus* c)
{
    Mundus* m;
    chorda  cl;
       i32  ante;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->repertorium);
    cl = diem(XIV, c->piscina);
    CREDO_NON_NIHIL(conspectus_legere(m->repertorium, "sanctus", cl));
    ante = m->computationes;
    (vacuum)rem_creare_die(m, "alpha", XIV, c->piscina);
    CREDO_CHORDA_NON_VACUA(personae_hominem_creare(m->personae, lit(
        "Anna", c->piscina), lit("Rossi", c->piscina)));
    CREDO_AEQUALIS_I32((i32)conspectus_status(m->repertorium, "sanctus",
        cl), (i32)CONSPECTUS_RECENS);
    CREDO_AEQUALIS_I32(*(constans i32*)conspectus_legere(m->repertorium,
        "sanctus", cl), XIV);
    CREDO_AEQUALIS_I32(m->computationes, ante);
}

interior vacuum
_probare_dependentias (
    CredoContextus* c)
{
    Mundus* m;
    chorda  a;
    chorda  h11;
    chorda  h18;
      Dies  d;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->repertorium);
    a   = rem_creare_die(m, "alpha", XIV, c->piscina);
    /* dies 10..20 ante: hebdomas XI eos EX CACHE legit (dependentia
     * etiam in lectione recentis registranda); XVIII dies 21-24
     * computat */
    dies_legere_omnes(m, c->piscina);
    h11 = diem(XI, c->piscina);
    h18 = diem(XVIII, c->piscina);
    CREDO_AEQUALIS_I32(*(constans i32*)conspectus_legere(m->repertorium,
        "hebdomas", h11), I);
    CREDO_AEQUALIS_I32(*(constans i32*)conspectus_legere(m->repertorium,
        "hebdomas", h18), ZEPHYRUM);

    /* dies 22 primum INTRA hebdomadem XVIII computatus: dependentia in
     * computatione registranda */
    (vacuum)rem_creare_die(m, "beta", XXII, c->piscina);
    CREDO_AEQUALIS_I32((i32)conspectus_status(m->repertorium,
        "hebdomas",
        h18), (i32)CONSPECTUS_STALUM);
    CREDO_AEQUALIS_I32((i32)conspectus_status(m->repertorium,
        "hebdomas",
        h11), (i32)CONSPECTUS_RECENS);
    CREDO_AEQUALIS_I32(*(constans i32*)conspectus_legere(m->repertorium,
        "hebdomas", h18), I);

    /* alpha 14 -> 19: hebdomas utraque (per dies 14 et 19) */
    d = fasti_dies(MMXXVI, X, XIX);
    CREDO_MAIOR_S64(agenda_diem_ponere(m->agenda, a, &d), ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)conspectus_status(m->repertorium,
        "hebdomas",
        h11), (i32)CONSPECTUS_STALUM);
    CREDO_AEQUALIS_I32((i32)conspectus_status(m->repertorium,
        "hebdomas",
        h18), (i32)CONSPECTUS_STALUM);
    CREDO_AEQUALIS_I32(*(constans i32*)conspectus_legere(m->repertorium,
        "hebdomas", h11), ZEPHYRUM);
    CREDO_AEQUALIS_I32(*(constans i32*)conspectus_legere(m->repertorium,
        "hebdomas", h18), II);

    /* dies 25 (extra hebdomadas): hebdomadae recentes manent */
    d = fasti_dies(MMXXVI, X, XXV);
    CREDO_MAIOR_S64(agenda_diem_ponere(m->agenda, a, &d), ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)conspectus_status(m->repertorium,
        "hebdomas",
        h11), (i32)CONSPECTUS_RECENS);
    CREDO_AEQUALIS_I32((i32)conspectus_status(m->repertorium,
        "hebdomas",
        h18), (i32)CONSPECTUS_STALUM);

    /* circulus: lectio interior NIHIL, nulla recursio infinita */
    CREDO_AEQUALIS_I32(*(constans i32*)conspectus_legere(m->repertorium,
        "circulus", h11), II);
}

interior vacuum
_probare_verificationem (
    CredoContextus* c)
{
    Mundus* m;
       Xar* discrepantes;
    chorda  a;
    chorda  b;
      Dies  d;
       i32  gradus;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->repertorium);
    discrepantes  = xar_creare(c->piscina, (i32)magnitudo(chorda));
    a             = lit("", c->piscina);
    b             = lit("", c->piscina);
    per (gradus = ZEPHYRUM; gradus < XIV; gradus++)
    {
        commutatio (gradus)
        {
        casus ZEPHYRUM:
            a = rem_creare_die(m, "alpha", XII, c->piscina);
            frange;
        casus I:
            b = rem_creare_die(m, "beta", XIII, c->piscina);
            frange;
        casus II:
            d = fasti_dies(MMXXVI, X, XVIII);
            (vacuum)agenda_diem_ponere(m->agenda, a, &d);
            frange;
        casus III:
            (vacuum)agenda_perficere(m->agenda, b, fasti_dies(MMXXVI, X,
                XV));
            frange;
        casus IV:
            (vacuum)agenda_perfectionem_tollere(m->agenda, b);
            frange;
        casus V:
            (vacuum)agenda_titulum_ponere(m->agenda, a,
                lit("alpha nova",
                c->piscina));
            frange;
        casus VI:
            (vacuum)agenda_revocare(m->agenda);
            frange;
        casus VII:
            (vacuum)agenda_reficere(m->agenda);
            frange;
        casus VIII:
            (vacuum)agenda_revocare(m->agenda);
            frange;
        casus IX:
            /* ramus post revocationem */
            d = fasti_dies(MMXXVI, X, XI);
            (vacuum)agenda_diem_ponere(m->agenda, b, &d);
            frange;
        casus X:
            (vacuum)agenda_rem_delere(m->agenda, a);
            frange;
        casus XI:
            (vacuum)personae_hominem_creare(m->personae, lit("Anna",
                c->piscina), lit("Rossi", c->piscina));
            frange;
        casus XII:
            (vacuum)volumen_actum_appendere(m->volumen,
                "dies/2026-10-16/paginae/1/scriba", lit("<m/>",
                c->piscina));
            frange;
        ordinarius:
            (vacuum)agenda_diem_ponere(m->agenda, b, NIHIL);
            frange;
        }
        dies_legere_omnes(m, c->piscina);
        (vacuum)conspectus_legere(m->repertorium, "hebdomas", diem(XI,
            c->piscina));
        CREDO_AEQUALIS_I32(conspectus_verificare(m->repertorium,
            discrepantes), ZEPHYRUM);
    }

    /* irritatio omissa: 'falsa' pura professa agendam legit */
    CREDO_NON_NIHIL(conspectus_legere(m->repertorium, "falsa", diem(XIV,
        c->piscina)));
    (vacuum)rem_creare_die(m, "delta", XIV, c->piscina);
    CREDO_AEQUALIS_I32(conspectus_verificare(m->repertorium,
        discrepantes), I);
    CREDO_AEQUALIS_I32(xar_numerus(discrepantes), I);
    CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(discrepantes,
        ZEPHYRUM), "falsa/2026-10-14");
}

interior vacuum
_probare_tectum (
    CredoContextus* c)
{
                 Mundus* m;
    ConspectusRegistrum* r;
         memoriae_index  tectum;
                    s32  k;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->volumen);
    /* tectum = XVI introitus, ex magnitudine introitus MENSA (non
     * octeti fixi: StmlNodus in 15c01bb4 minor factus est, et XVI KiB
     * fixa XXXI dies sine eiectione tenebant) */
    r = registrum_plenum(m, c->piscina, ZEPHYRUM);
    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_NON_NIHIL(conspectus_legere(r, "dies", diem(I, c->piscina)));
    CREDO_VERUM(conspectus_summa_usus(r) > ZEPHYRUM);
    tectum  = (memoriae_index)(XVI * conspectus_summa_usus(r));
    r       = registrum_plenum(m, c->piscina, tectum);
    CREDO_NECESSE_NON_NIHIL(r);
    per (k = I; k <= XXXI; k++)
    {
        CREDO_NON_NIHIL(conspectus_legere(r, "dies", diem(k,
            c->piscina)));
        CREDO_MINOR_AUT_AEQUALIS_I64((i64)conspectus_summa_usus(r),
            (i64)tectum);
    }
    /* primi eiecti, ultimus recens */
    CREDO_AEQUALIS_I32((i32)conspectus_status(r, "dies", diem(I,
        c->piscina)), (i32)CONSPECTUS_ABSENS);
    CREDO_AEQUALIS_I32((i32)conspectus_status(r, "dies", diem(XXXI,
        c->piscina)), (i32)CONSPECTUS_RECENS);
}

/* acta N tituli alterni; lectio post quodque si legere */
interior vacuum
titulos_mutare (
                 Mundus* m,
    ConspectusRegistrum* r,
                 chorda  a,
                    i32  n,
                    b32  legere,
                Piscina* piscina)
{
    i32 i;

    per (i = ZEPHYRUM; i < n; i++)
    {
        (vacuum)agenda_titulum_ponere(m->agenda, a, lit((i % II) ? "a"
            : "b", piscina));
        si (legere)
        {
            (vacuum)conspectus_legere(r, "dies", diem(XIV, piscina));
        }
    }
}

/* lectiones n sine actis; clavis in piscina propria vacata (probatio
 * ipsa non crescit) */
interior vacuum
lectiones_sine_actis (
    ConspectusRegistrum* r,
                    i32  n,
                Piscina* piscina)
{
    Piscina* p;
        i32  i;

    (vacuum)piscina;
    p = piscina_generare_dynamicum("probatio_lectiones", MXXIV);
    per (i = ZEPHYRUM; p && i < n; i++)
    {
        (vacuum)conspectus_legere(r, "dies", diem(XIV, p));
        piscina_vacare(p);
    }
    si (p)
    {
        piscina_destruere(p);
    }
}

/* D4c: registra n creare, dies III legere, destruere - piscina
 * vocatoris propria et destructa, ergo sola memoria quam registrum
 * ipsum possidet (tempus, introitus, res memoratae) restare potest */
interior vacuum
registra_iterare (
    Mundus* m,
       i32  n)
{
     ConspectusRegistrum* r;
                 Piscina* q;
                     i32  i;
                     s32  d;

    per (i = ZEPHYRUM; i < n; i++)
    {
        q = piscina_generare_dynamicum("probatio_registra",
            XVI * MXXIV);
        si (!q)
        {
            redde;
        }
        r = registrum_plenum(m, q, ZEPHYRUM);
        per (d = X; r && d <= XII; d++)
        {
            (vacuum)conspectus_legere(r, "dies", diem(d, q));
        }
        conspectus_registrum_destruere(r);
        piscina_destruere(q);
    }
}

interior i64
vestigium (vacuum)
{
    MemoriaProcessus mp;

    redde memoria_processus_legere(ZEPHYRUM, &mp) ? mp.vestigium
        : ZEPHYRUM;
}

interior vacuum
_probare_memoriam (
    CredoContextus* c)
{
                 Mundus* m;
    ConspectusRegistrum* r;
                Piscina* propria;
                 chorda  a;
         memoriae_index  post_primum;
                    s64  acta_ante;
                    i64  v1;
                    i64  v2;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->volumen);
    propria = piscina_generare_dynamicum("probatio_conspectus_memoria",
        IV * MXXIV);
    CREDO_NECESSE_NON_NIHIL(propria);
    r = registrum_plenum(m, propria, ZEPHYRUM);
    CREDO_NECESSE_NON_NIHIL(r);
    a = rem_creare_die(m, "alpha", XIV, c->piscina);
    (vacuum)conspectus_legere(r, "dies", diem(XIV, c->piscina));
    post_primum  = piscina_summa_usus(propria);
    acta_ante    = volumen_summa_actorum(m->volumen);

    /* gradus A: acta sine lectione; B1: lectio post omne actum
     * (differentia per-rem et recomputatio omni actu) - calefactio
     * allocatoris; B2 idem, MENSURATUM. Mensuratum 2026-10-21: B1
     * ~IV MiB (calefactio), B2 ~D KiB; memoria per-rem non liberata
     * (planta) ~VII MiB in B2. */
    titulos_mutare(m, r, a, (i32)(V * M), FALSUM, c->piscina);
    titulos_mutare(m, r, a, (i32)(V * M), VERUM, c->piscina);
    v1 = vestigium();
    titulos_mutare(m, r, a, (i32)(V * M), VERUM, c->piscina);
    v2 = vestigium();
    v2 = v2 > v1 ? v2 - v1 : ZEPHYRUM;   /* i64 insignatum */
    imprimere("memoria: B2 %ld KiB\n", (longus)(v2 / MXXIV));
    CREDO_MINOR_I64(v2, (i64)(IV * MXXIV * MXXIV));

    /* C (D4b): lectiones multae SINE actis - via brevis renovationis
     * (nihil novum) piscinam temporariam quoque vacare debet */
    v1 = vestigium();
    lectiones_sine_actis(r, (i32)(D * M), c->piscina);
    v2 = vestigium();
    v2 = v2 > v1 ? v2 - v1 : ZEPHYRUM;
    imprimere("memoria: C %ld KiB\n", (longus)(v2 / MXXIV));
    /* mensuratum 2026-10-21 (D milia lectionum): 0 KiB; piscina
     * temporaria non vacata (planta) ~XIX octeti per lectionem = IX
     * MiB. L milia non sufficiebant: alvei post vacationem manent,
     * damnum ~I MiB intra capacitatem priorem latebat (planta
     * viridis) */
    CREDO_MINOR_I64(v2, (i64)(MXXIV * MXXIV));

    CREDO_MAIOR_AUT_AEQUALIS_S64(volumen_summa_actorum(m->volumen)
        - acta_ante, (s64)(XV * M));
    /* registrum non crescit cum actis */
    CREDO_MINOR_I64((i64)(piscina_summa_usus(propria) - post_primum),
        (i64)(LXIV * MXXIV));
    CREDO_MINOR_I64((i64)conspectus_summa_usus(r), (i64)(LXIV * MXXIV));
    conspectus_registrum_destruere(r);
    piscina_destruere(propria);

    /* D (D4c): registra creata et destructa - arenae quas registrum
     * possidet redduntur. Calefactio prius. Mensuratum 2026-10-21 (MM
     * registra, dies III): sanum 320-1104 KiB (strepitus allocatoris,
     * non crescit cum numero; CCC registra post XX calefactionis
     * 0-1552 KiB - limes I MiB ibi FLUCTUABAT); plantae: destruere
     * nihil 33 MiB, introitus 19 MiB, tempus 8.2 MiB, memoria per-rem
     * 6.1 MiB (damnum cum numero crescit). Limes III MiB inter
     * utrumque. */
    registra_iterare(m, C);
    v1 = vestigium();
    registra_iterare(m, MM);
    v2 = vestigium();
    v2 = v2 > v1 ? v2 - v1 : ZEPHYRUM;
    imprimere("memoria: D %ld KiB\n", (longus)(v2 / MXXIV));
    CREDO_MINOR_I64(v2, (i64)(III * MXXIV * MXXIV));
}


/* ==================================================
 * D5 (indagatio): declarationes quaerendae, enumeratio, auditor
 * ================================================== */

hic_manens constans character* constans DECLARATIO_RERUM =
    "<conspectus nomen=\"res\" versio=\"1\">\n"
    "  <clavis typus=\"textus\"/>\n"
    "  <fons documentum=\"agenda\" irritat=\"per-rem\"/>\n"
    "  <quaerendum genus=\"Agenda\"/>\n"
    "</conspectus>\n";

hic_manens constans character* constans DECLARATIO_LIBRI =
    "<conspectus nomen=\"liber\" versio=\"1\">\n"
    "  <clavis typus=\"dies\"/>\n"
    "  <fons spatium=\"dies/{clavis}\"/>\n"
    "  <quaerendum genus=\"Libri\"/>\n"
    "</conspectus>\n";

hic_manens constans character* constans DECLARATIO_HOMINUM =
    "<conspectus nomen=\"homines\" versio=\"1\">\n"
    "  <clavis typus=\"textus\"/>\n"
    "  <fons documentum=\"personae\" irritat=\"omnia\"/>\n"
    "  <quaerendum genus=\"Personae\"/>\n"
    "</conspectus>\n";

/* per-rem 'res': clavis rei = id eius */
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

interior Xar*
libros_enumerare (
                 vacuum* ctx,
    ConspectusRegistrum* repertorium,
                Piscina* piscina)
{
    Xar* claves;

    (vacuum)ctx;
    (vacuum)repertorium;
    claves = xar_creare(piscina, (i32)magnitudo(chorda));
    *(chorda*)xar_addere(claves) = diem(XIX, piscina);
    redde claves;
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

/* auditor probationis: claves auditas copiat ("" = omnes) */
nomen structura {
    Piscina* piscina;
        Xar* claves;     /* chorda */
} Auditio;

interior vacuum
audire (
    vacuum* ctx,
    chorda  clavis)
{
    Auditio* a = (Auditio*)ctx;

    *(chorda*)xar_addere(a->claves) = chorda_transcribere(clavis,
        a->piscina);
}

interior b32
auditum (
    Auditio* a,
     chorda  clavis)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(a->claves); i++)
    {
        si (chorda_aequalis(*(chorda*)xar_obtinere(a->claves, i),
                clavis))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* registrum cum dies (non quaerendus) et res, liber, homines */
interior ConspectusRegistrum*
registrum_quaerendum (
      Mundus* m,
     Piscina* piscina,
         b32  cum_enumeratione)
{
    ConspectusRegistrum* r;
                    Xar* vitia;

    r = conspectus_registrum_creare(piscina, m->intern, m->volumen,
        ZEPHYRUM);
    si (   !r || !declarare(r, DECLARATIO_DIEI, piscina)
        || !declarare(r, DECLARATIO_RERUM, piscina)
        || !declarare(r, DECLARATIO_LIBRI, piscina)
        || !declarare(r, DECLARATIO_HOMINUM, piscina))
    {
        redde NIHIL;
    }
    (vacuum)conspectus_opera_ponere(r, "dies", opera(dies_computare,
        dies_codificare, m));
    (vacuum)conspectus_opera_ponere(r, "res", opera(sanctus_computare,
        numerum_codificare, m));
    (vacuum)conspectus_opera_ponere(r, "liber", opera(sanctus_computare,
        numerum_codificare, m));
    (vacuum)conspectus_opera_ponere(r, "homines", opera(
        sanctus_computare, numerum_codificare, m));
    (vacuum)conspectus_per_rem_ponere(r, "dies", "agenda",
        per_rem_agendae());
    {
        ConspectusPerRem p = per_rem_agendae();

        p.claves = res_claves;
        (vacuum)conspectus_per_rem_ponere(r, "res", "agenda", p);
    }
    (vacuum)conspectus_documentum_ligare(r, "agenda", m->agenda);
    (vacuum)conspectus_documentum_ligare(r, "personae", m->personae);
    si (cum_enumeratione)
    {
        (vacuum)conspectus_enumerationem_ponere(r, "res", res_enumerare,
            m);
        (vacuum)conspectus_enumerationem_ponere(r, "liber",
            libros_enumerare, m);
        (vacuum)conspectus_enumerationem_ponere(r, "homines",
            homines_enumerare, m);
    }
    redde conspectus_parare(r, &vitia) ? r : NIHIL;
}

interior vacuum
_probare_quaerenda (
    CredoContextus* c)
{
                 Mundus* m;
    ConspectusRegistrum* r;
                    Xar* vitia;
                    Xar* q;
                    Xar* claves;
                 chorda  alpha;
                 chorda  beta;
                Auditio  res_a;
                Auditio  liber_a;
                Auditio  homines_a;
                Auditio  dies_a;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->volumen);

    /* canon: genus necessarium, quaerendum semel */
    r = conspectus_registrum_creare(c->piscina, m->intern, m->volumen,
        ZEPHYRUM);
    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_FALSUM(conspectus_declarare(r, lit(
        "<conspectus nomen=\"q1\" versio=\"1\"><clavis typus=\"dies\"/>"
        "<quaerendum/></conspectus>", c->piscina), &vitia));
    CREDO_MAIOR_I32(xar_numerus(vitia), ZEPHYRUM);
    CREDO_FALSUM(conspectus_declarare(r, lit(
        "<conspectus nomen=\"q2\" versio=\"1\"><clavis typus=\"dies\"/>"
        "<quaerendum genus=\"A\"/><quaerendum genus=\"B\"/>"
        "</conspectus>", c->piscina), &vitia));
    CREDO_MAIOR_I32(xar_numerus(vitia), ZEPHYRUM);

    /* parare: quaerendum sine enumeratione recusatur, nominatim */
    alpha  = rem_creare_die(m, "alpha", XIV, c->piscina);
    beta   = rem_creare_die(m, "beta", XV, c->piscina);
    CREDO_NIHIL(registrum_quaerendum(m, c->piscina, FALSUM));
    r = conspectus_registrum_creare(c->piscina, m->intern, m->volumen,
        ZEPHYRUM);
    CREDO_NECESSE(declarare(r, DECLARATIO_LIBRI, c->piscina));
    (vacuum)conspectus_opera_ponere(r, "liber", opera(sanctus_computare,
        numerum_codificare, m));
    CREDO_FALSUM(conspectus_parare(r, &vitia));
    CREDO_NECESSE(xar_numerus(vitia) > ZEPHYRUM);
    CREDO_CHORDA_CONTINET(*(chorda*)xar_obtinere(vitia, ZEPHYRUM),
        lit("enumer", c->piscina));

    r = registrum_quaerendum(m, c->piscina, VERUM);
    CREDO_NECESSE_NON_NIHIL(r);

    /* quaerenda: ordine declarationis, 'dies' non */
    q = conspectus_quaerenda(r, c->piscina);
    CREDO_NECESSE(q && xar_numerus(q) == III);
    CREDO_CHORDA_AEQUALIS_LITERIS(((ConspectusQuaerendum*)xar_obtinere(
        q, ZEPHYRUM))->titulus, "res");
    CREDO_CHORDA_AEQUALIS_LITERIS(((ConspectusQuaerendum*)xar_obtinere(
        q, ZEPHYRUM))->genus, "Agenda");
    CREDO_CHORDA_AEQUALIS_LITERIS(((ConspectusQuaerendum*)xar_obtinere(
        q, I))->titulus, "liber");
    CREDO_CHORDA_AEQUALIS_LITERIS(((ConspectusQuaerendum*)xar_obtinere(
        q, II))->genus, "Personae");

    /* enumerare */
    claves = conspectus_enumerare(r, "res", c->piscina);
    CREDO_NECESSE(claves && xar_numerus(claves) == II);
    CREDO_CHORDA_AEQUALIS(*(chorda*)xar_obtinere(claves, ZEPHYRUM),
        alpha);
    CREDO_NIHIL(conspectus_enumerare(r, "dies", c->piscina));
    CREDO_NIHIL(conspectus_enumerare(r, "ignotum", c->piscina));

    /* auditor: claves NUMQUAM lectae quoque audiuntur */
    res_a.piscina      = c->piscina;
    res_a.claves       = xar_creare(c->piscina, (i32)magnitudo(chorda));
    liber_a.piscina    = c->piscina;
    liber_a.claves     = xar_creare(c->piscina, (i32)magnitudo(chorda));
    homines_a.piscina  = c->piscina;
    homines_a.claves   = xar_creare(c->piscina, (i32)magnitudo(chorda));
    dies_a.piscina     = c->piscina;
    dies_a.claves      = xar_creare(c->piscina, (i32)magnitudo(chorda));
    CREDO_NECESSE(conspectus_auditorem_ponere(r, "res", audire,
        &res_a));
    CREDO_NECESSE(conspectus_auditorem_ponere(r, "liber", audire,
        &liber_a));
    CREDO_NECESSE(conspectus_auditorem_ponere(r, "homines", audire,
        &homines_a));
    CREDO_NECESSE(conspectus_auditorem_ponere(r, "dies", audire,
        &dies_a));
    CREDO_FALSUM(conspectus_auditorem_ponere(r, "ignotum", audire,
        &dies_a));

    (vacuum)agenda_titulum_ponere(m->agenda, beta, lit("beta nova",
        c->piscina));
    (vacuum)conspectus_renovare(r);
    CREDO_VERUM(auditum(&res_a, beta));
    CREDO_FALSUM(auditum(&res_a, alpha));
    CREDO_AEQUALIS_I32(xar_numerus(liber_a.claves), ZEPHYRUM);
    CREDO_AEQUALIS_I32(xar_numerus(homines_a.claves), ZEPHYRUM);
    /* 'dies' (per-rem, claves dies) suas audit, non ids */
    CREDO_VERUM(auditum(&dies_a, diem(XV, c->piscina)));
    CREDO_FALSUM(auditum(&dies_a, beta));

    /* spatium: clavis ex spatio */
    (vacuum)volumen_actum_appendere(m->volumen, "dies/2026-10-19/nota",
        lit("x", c->piscina));
    (vacuum)conspectus_renovare(r);
    CREDO_VERUM(auditum(&liber_a, diem(XIX, c->piscina)));

    /* omnia: clavis vacua */
    (vacuum)personae_hominem_creare(m->personae, lit("Anna",
        c->piscina),
        lit("Rossi", c->piscina));
    (vacuum)conspectus_renovare(r);
    CREDO_VERUM(auditum(&homines_a, vacua_probationis()));

    /* auditor sublatus: nihil */
    CREDO_NECESSE(conspectus_auditorem_ponere(r, "res", NIHIL, NIHIL));
    xar_vacare(res_a.claves);
    (vacuum)agenda_titulum_ponere(m->agenda, alpha, lit("alpha nova",
        c->piscina));
    (vacuum)conspectus_renovare(r);
    CREDO_AEQUALIS_I32(xar_numerus(res_a.claves), ZEPHYRUM);
    conspectus_registrum_destruere(r);
}

/* actum alterius replicae (communio) importare: genus datum */
interior VolumenImportatio
alienum_importare (
                Mundus* m,
                chorda  replica,
    constans character* genus,
               Piscina* piscina)
{
             Xar* propria;
    VolumenActum  f;
    VolumenActum* ultimum;

    propria = volumen_acta_legere(m->volumen, ZEPHYRUM, piscina);
    si (!propria || xar_numerus(propria) == ZEPHYRUM)
    {
        redde VOLUMEN_IMPORTATIO_MALA;
    }
    ultimum = (VolumenActum*)xar_obtinere(propria,
        xar_numerus(propria) - I);
    memset(&f, ZEPHYRUM, magnitudo(f));
    f.genus              = lit(genus, piscina);
    f.datum              = lit("<mutatio/>", piscina);
    f.momentum           = ultimum->momentum;
    f.identitas.hora     = ultimum->identitas.hora;
    f.identitas.machina  = lit("aliena", piscina);
    f.identitas.replica  = replica;
    f.seq_originis       = I;
    redde volumen_actum_importare(m->volumen, &f);
}

/* communio plan-2 T1: acta ALIENA (importata, alterius replicae)
 * conspectus localem non irritant - nec spatium diei nec documentum
 * 'omnia'; cursor tamen ad finem voluminis pervenit */
interior vacuum
_probare_alienum (
    CredoContextus* c)
{
       Mundus* m;
    character  replica[MONETA_ULID_MENSURA];

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->repertorium);
    moneta_ulid(replica);
    (vacuum)dies_legere(m, XIX, c->piscina);
    (vacuum)dies_legere(m, XX, c->piscina);
    CREDO_AEQUALIS_I32((i32)alienum_importare(m, lit(replica,
        c->piscina), "dies/2026-10-19/paginae/1/scriba", c->piscina),
        (i32)VOLUMEN_IMPORTATUM);
    CREDO_AEQUALIS_I32((i32)status_diei(m, XIX, c->piscina),
        (i32)CONSPECTUS_RECENS);
    CREDO_AEQUALIS_I32((i32)status_diei(m, XX, c->piscina),
        (i32)CONSPECTUS_RECENS);

    CREDO_GRADUS("documentum 'omnia' alienum");
    moneta_ulid(replica);
    CREDO_AEQUALIS_I32((i32)alienum_importare(m, lit(replica,
        c->piscina), "personae/ignota/actum", c->piscina),
        (i32)VOLUMEN_IMPORTATUM);
    CREDO_AEQUALIS_I32((i32)status_diei(m, XX, c->piscina),
        (i32)CONSPECTUS_RECENS);

    CREDO_GRADUS("cursor ad finem");
    CREDO_AEQUALIS_S64(conspectus_renovare(m->repertorium),
        volumen_seq_ultimum(m->volumen));

    CREDO_GRADUS("proprium tamen irritat");
    CREDO_MAIOR_S64(volumen_actum_appendere(m->volumen,
        "dies/2026-10-19/paginae/1/scriba", lit("<mutatio/>",
        c->piscina)), ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)status_diei(m, XIX, c->piscina),
        (i32)CONSPECTUS_STALUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "declaratio",   _probare_declarationem,  mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "paratio",      _probare_parare,     mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "lectio",       _probare_lectionem,      mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "per-rem",      _probare_per_rem,        mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "omnia",        _probare_omnia,          mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "spatium",      _probare_spatium,        mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "pura",         _probare_puram,          mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "dependentiae", _probare_dependentias,   mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "verificatio",  _probare_verificationem, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "tectum",       _probare_tectum,         mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "memoria",      _probare_memoriam,       mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "quaerenda",    _probare_quaerenda,      mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "acta aliena",  _probare_alienum,        mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("conspectus", SECTIONES);
}

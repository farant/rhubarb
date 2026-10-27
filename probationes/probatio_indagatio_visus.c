/* probatio_indagatio_visus.c - INDAGATIO in latere vici (S3a;
 * include/indagatio_visus.h). Index verus super generibus domus
 * (indagatio_genera): res agendae (quinque "Rhubarb ..." rubrica
 * "kitchen", "Call the plumber"), persona cum notis, liber diei, liber
 * communis. Semita (URI, verba nuda, vacuum, mala), conspectus gregum
 * et "+ k more", quaestio viva et commissa, electio et porta, greges
 * paginati, Esc gradibus, excerptum signatum. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "insula.h"
#include "componens.h"
#include "dispensator.h"
#include "eventus.h"
#include "figura.h"
#include "mandatum.h"
#include "thema.h"
#include "volumen.h"
#include "moneta.h"
#include "fasti.h"
#include "agenda.h"
#include "personae.h"
#include "tabula_characterum.h"
#include "scriba_documentum.h"
#include "scriba_liber.h"
#include "conspectus.h"
#include "indagatio.h"
#include "indagatio_genera.h"
#include "indagatio_visus.h"
#include "semita.h"
#include "organum_montatio.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

nomen structura {
                Volumen* volumen;
    InternamentumChorda* intern;
       AgendaDocumentum* agenda;
     PersonaeDocumentum* personae;
        IndagatioFontes  fontes;
    ConspectusRegistrum* repertorium;
              Indagatio* index;
     InsulaRepositorium* repo;
      IndagatioMontatio  m;
            Dispensator* d;
       constans Organum* organum;
                 chorda  apertum;     /* semita portae ultima */
                    i32  aperta;      /* vocationes portae */
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

/* porta: semita notatur */
interior b32
porta_aperire (
     vacuum* ctx,
     chorda  semita)
{
    Mundus* w;

    w           = (Mundus*)ctx;
    w->apertum  = chorda_transcribere(semita, w->piscina);
    w->aperta++;
    redde VERUM;
}

interior b32
paginam_scribere (
                 Mundus* w,
     constans character* praefixum,
     constans character* textus)
{
          ScribaLiber* l;
     ScribaDocumentum* d;
    TabulaCharacterum  folium;

    l = praefixum ? scriba_liber_aperire_in_spatio(w->piscina,
        w->intern, w->volumen, praefixum, XL, X)
        : scriba_liber_aperire(w->piscina, w->intern, w->volumen, XL,
        X);
    d = l ? scriba_liber_pagina(l, lit("1", w->piscina)) : NIHIL;
    si (!d)
    {
        redde FALSUM;
    }
    tabula_ex_literis_cum_dimensionibus(&folium, w->piscina,
        d->latitudo, d->altitudo, textus);
    redde scriba_documentum_committere(d, &folium) > ZEPHYRUM;
}

interior b32
fixturam_scribere (
     Mundus* w)
{
    constans character* constans tituli[] = {
        "Rhubarb tart", "Rhubarb jam", "Rhubarb pie", "Rhubarb wine",
        "Rhubarb soup"
    };
     chorda id;
       Dies d;
        i32 i;

    per (i = ZEPHYRUM; i < V; i++)
    {
        id = agenda_rem_creare(w->agenda, lit(tituli[i], w->piscina));
        si (   id.mensura == ZEPHYRUM
            || !agenda_rubricas_ponere(w->agenda, id, lit("kitchen",
                   w->piscina)))
        {
            redde FALSUM;
        }
    }
    d = fasti_dies(MMXXVI, X, XXI);
    (vacuum)agenda_diem_ponere(w->agenda, lit("1", w->piscina), &d);
    (vacuum)agenda_rem_creare(w->agenda, lit("Call the plumber",
        w->piscina));
    (vacuum)personae_hominem_creare(w->personae, lit("Anna",
        w->piscina), lit("Rossi", w->piscina));
    redde paginam_scribere(w, "persona/1", "likes rhubarb")
        && paginam_scribere(w, "dies/2026-10-19",
               "rhubarb crumble recipe")
        && paginam_scribere(w, NIHIL, "Groceries\nrhubarb, sugar");
}

/* latus CCC x altitudo super indicem verum; porta notat (aut NIHIL) */
interior Mundus*
mundus (
    CredoContextus* c,
               i32  altitudo,
               b32  porta)
{
    Mundus* w;
     Motus* motus;
       Xar* vitia;

    thema_initiare();
    w = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(w, ZEPHYRUM, magnitudo(Mundus));
    c->datum    = w;
    w->piscina  = c->piscina;
    w->intern   = internamentum_creare(c->piscina);
    w->volumen  = volumen_temporarium(c->piscina,
        "probatio_indagatio_visus");
    si (!w->volumen)
    {
        redde NIHIL;
    }
    w->agenda = agenda_aperire(c->piscina, w->intern, w->volumen,
        "agenda", ZEPHYRUM);
    agenda_monetam_ponere(w->agenda, moneta_numerator,
        &w->numerator_agendae);
    w->personae = personae_aperire(c->piscina, w->intern, w->volumen,
        "personae", ZEPHYRUM);
    personae_monetam_ponere(w->personae, moneta_numerator,
        &w->numerator_personarum);
    si (!fixturam_scribere(w))
    {
        redde NIHIL;
    }
    w->fontes.agenda    = w->agenda;
    w->fontes.personae  = w->personae;
    w->fontes.volumen   = w->volumen;
    w->fontes.intern    = w->intern;
    w->repertorium = conspectus_registrum_creare(c->piscina, w->intern,
        w->volumen, ZEPHYRUM);
    si (   !w->repertorium
        || !indagatio_genera_registrare(w->repertorium, &w->fontes,
               &vitia)
        || !conspectus_parare(w->repertorium, &vitia))
    {
        redde NIHIL;
    }
    w->index = indagatio_creare(c->piscina, w->repertorium);
    w->repo  = insula_repositorium_creare(c->piscina, w->intern,
        "<hospes/>");
    si (   !w->index || !w->repo
        || !indagatio_visus_montare(&w->m, c->piscina, w->intern,
               w->repo, "x1", w->index, porta ? porta_aperire : NIHIL,
               w, CCC, altitudo, NIHIL))
    {
        redde NIHIL;
    }
    w->d = dispensator_creare(c->piscina, w->intern, w->repo,
        w->m.actiones, indagatio_visus_componere, &w->m, CCC);
    si (!w->d)
    {
        redde NIHIL;
    }
    motus         = dispensator_motus(w->d);
    motus->ramus  = w->m.ramus;
    dispensator_recomponere(w->d);
    redde w;
}

interior vacuum
mundum_purgare (
    CredoContextus* c)
{
    Mundus* w;

    w = (Mundus*)c->datum;
    si (w && w->index)
    {
        indagatio_destruere(w->index);
    }
    si (w && w->repertorium)
    {
        conspectus_registrum_destruere(w->repertorium);
    }
    si (w && w->volumen)
    {
        volumen_claudere(w->volumen);
    }
}

interior vacuum
clavis (
       Mundus* w,
    character  typus)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                  = EVENTUS_CLAVIS_DEPRESSUS;
    e.datum.clavis.producta  = (s32)(insignatus character)typus;
    e.datum.clavis.clavis    = (clavis_t)typus;
    e.datum.clavis.runa      = (s32)typus;
    dispensator_tractare(w->d, &e);
    dispensator_recomponere(w->d);
}

interior vacuum
clavis_nominata (
       Mundus* w,
     clavis_t  k,
          s32  producta)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                  = EVENTUS_CLAVIS_DEPRESSUS;
    e.datum.clavis.clavis    = k;
    e.datum.clavis.producta  = producta;
    dispensator_tractare(w->d, &e);
    dispensator_recomponere(w->d);
}

interior vacuum
scribere (
                 Mundus* w,
     constans character* textus)
{
    dum (*textus)
    {
        clavis(w, *textus);
        textus++;
    }
}

interior vacuum
reditus (
    Mundus* w)
{
    clavis_nominata(w, CLAVIS_REDITUS, XIII);
}

interior vacuum
effugium (
    Mundus* w)
{
    clavis_nominata(w, CLAVIS_EFFUGIUM, XXVII);
}

interior b32
campus_est (
                 Mundus* w,
     constans character* titulus,
     constans character* valor)
{
    chorda* a;

    a = insula_ramus_attributum(&w->m.ramus, titulus);
    si (valor == NIHIL)
    {
        redde a == NIHIL || a->mensura == ZEPHYRUM;
    }
    redde a && chorda_aequalis_literis(*a, valor);
}

interior b32
argumentum (
                 Mundus* w,
     constans character* a)
{
    b32 r;

    r = indagatio_visus_argumentum_ponere(&w->m, a);
    dispensator_recomponere(w->d);
    redde r;
}

interior Mandata*
reddere (
     Mundus* w,
    Piscina* p)
{
    Mandata* md;

    md = mandata_creare(p, w->intern);
    pingere(dispensator_arbor(w->d), w->m.figurae, ZEPHYRUM, md);
    redde md;
}

/* textus pictus continens fragmentum (exacte: aequalis)? */
interior b32
pictum_forma (
                 Mundus* w,
     constans character* fragmentum,
                    b32  exacte,
                Piscina* p)
{
                Mandata* md;
      constans Mandatum* x;
                    i32  i;

    md = reddere(w, p);
    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   x->genus == MANDATUM_TEXTUS
            && (exacte ? chorda_aequalis_literis(x->textus, fragmentum)
                : chorda_continet(x->textus, chorda_ex_literis(
                fragmentum, p))))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior b32
pictum (
                 Mundus* w,
     constans character* fragmentum,
                Piscina* p)
{
    redde pictum_forma(w, fragmentum, FALSUM, p);
}

interior constans Organum*
organa (
     vacuum* ctx,
     chorda  genus)
{
    redde chorda_aequalis_literis(genus, "indagatio")
        ? ((Mundus*)ctx)->organum : NIHIL;
}

interior chorda
semita_currens (
     Mundus* w,
    Piscina* p)
{
    si (!w->organum)
    {
        w->organum = organum_legere_genus(p, w->intern, "indagatio",
            "probatio");
    }
    redde semita_ad_uri(organa, w, semita_legere(organa, w,
        &w->m.ramus, "indagatio", p, w->intern), p);
}


/* ==================================================
 * Sectiones
 * ================================================== */

/* semita: URI, verba nuda (cognomen), vacuum; semita tota (campi
 * absentes tolluntur); mala nihil mutant */
interior vacuum
_sectio_semita (
    CredoContextus* c)
{
    Mundus* w;

    w = mundus(c, CCLVI, VERUM);
    CREDO_NECESSE_NON_NIHIL(w);
    CREDO_CHORDA_AEQUALIS_LITERIS(semita_currens(w, c->piscina),
        "indagatio");
    CREDO_VERUM(argumentum(w,
        "indagatio?q=rhubarb&genus=res&pagina=1"));
    CREDO_VERUM(campus_est(w, "q", "rhubarb"));
    CREDO_VERUM(campus_est(w, "genus", "res"));
    CREDO_VERUM(campus_est(w, "pagina", "1"));
    CREDO_CHORDA_AEQUALIS_LITERIS(semita_currens(w, c->piscina),
        "indagatio?q=rhubarb&genus=res&pagina=1");
    /* semita tota: genus et pagina absentes tolluntur */
    CREDO_VERUM(argumentum(w, "indagatio?q=tart"));
    CREDO_VERUM(campus_est(w, "q", "tart"));
    CREDO_VERUM(campus_est(w, "genus", NIHIL));
    CREDO_VERUM(campus_est(w, "pagina", NIHIL));
    /* verba nuda: cognomen */
    CREDO_VERUM(argumentum(w, "rhubarb tart"));
    CREDO_VERUM(campus_est(w, "q", "rhubarb tart"));
    CREDO_CHORDA_AEQUALIS_LITERIS(semita_currens(w, c->piscina),
        "indagatio?q=rhubarb%20tart");
    /* vacuum: quaestio nulla */
    CREDO_VERUM(argumentum(w, ""));
    CREDO_VERUM(campus_est(w, "q", NIHIL));
    /* mala: nihil mutatum */
    CREDO_VERUM(argumentum(w, "indagatio?q=abc"));
    CREDO_FALSUM(argumentum(w, "indagatio?nemo=1"));
    CREDO_FALSUM(argumentum(w, "indagatio?pagina=x"));
    CREDO_VERUM(campus_est(w, "q", "abc"));
}

/* conspectus: greges, inventa prima, "+ k more", contextus, summa;
 * quaestio vacua; nihil inventum */
interior vacuum
_sectio_conspectus (
    CredoContextus* c)
{
    Mundus* w;

    w = mundus(c, D, VERUM);
    CREDO_NECESSE_NON_NIHIL(w);
    CREDO_VERUM(pictum(w, "Search", c->piscina));
    CREDO_VERUM(pictum(w, "Press / to search", c->piscina));
    CREDO_VERUM(argumentum(w, "rhubarb"));
    CREDO_VERUM(pictum(w, "Agenda", c->piscina));
    CREDO_VERUM(pictum(w, "Contacts", c->piscina));
    CREDO_VERUM(pictum(w, "Day notes", c->piscina));
    CREDO_VERUM(pictum(w, "Notebook", c->piscina));
    CREDO_VERUM(pictum(w, "Rhubarb tart", c->piscina));
    CREDO_VERUM(pictum(w, "due Wed 21 Oct 2026", c->piscina));
    CREDO_VERUM(pictum(w, "Anna Rossi", c->piscina));
    CREDO_VERUM(pictum(w, "Mon 19 Oct 2026", c->piscina));
    CREDO_VERUM(pictum(w, "Groceries", c->piscina));
    CREDO_VERUM(pictum(w, "+ 2 more", c->piscina));
    CREDO_VERUM(!pictum(w, "Rhubarb soup", c->piscina));
    CREDO_VERUM(pictum(w, "8 results", c->piscina));
    CREDO_VERUM(argumentum(w, "zzz"));
    CREDO_VERUM(pictum(w, "No results", c->piscina));
    /* fructus servatus: actum novum (versio indicis) videtur */
    CREDO_VERUM(argumentum(w, "plumb"));
    CREDO_VERUM(pictum(w, "1 result", c->piscina));
    CREDO_VERUM(agenda_rem_creare(w->agenda, lit("Plumber visit",
        c->piscina)).mensura > ZEPHYRUM);
    dispensator_recomponere(w->d);
    CREDO_VERUM(pictum(w, "2 results", c->piscina));
}

/* quaestio: / viva (q intacta), Enter committit (pagina et electus
 * tolluntur, genus manet), Esc q intactam relinquit;
 * quaestionem_aperire */
interior vacuum
_sectio_quaestio (
    CredoContextus* c)
{
    Mundus* w;

    w = mundus(c, D, VERUM);
    CREDO_NECESSE_NON_NIHIL(w);
    clavis(w, '/');
    CREDO_VERUM(campus_est(w, "prompta", "quaestio"));
    scribere(w, "plumb");
    CREDO_VERUM(pictum(w, "Call the plumber", c->piscina));
    CREDO_VERUM(campus_est(w, "q", NIHIL));
    reditus(w);
    CREDO_VERUM(campus_est(w, "q", "plumb"));
    CREDO_VERUM(campus_est(w, "prompta", NIHIL)
        || campus_est(w, "prompta", "nulla"));
    CREDO_VERUM(pictum(w, "Call the plumber", c->piscina));
    /* Esc in prompta: q intacta */
    clavis(w, '/');
    CREDO_VERUM(campus_est(w, "prompta_textus", "plumb"));
    scribere(w, "zz");
    effugium(w);
    CREDO_VERUM(campus_est(w, "q", "plumb"));
    CREDO_VERUM(campus_est(w, "prompta", NIHIL)
        || campus_est(w, "prompta", "nulla"));
    /* Enter: pagina et electus tolluntur, genus manet */
    CREDO_VERUM(argumentum(w,
        "indagatio?q=kitchen&genus=res&pagina=1"));
    clavis(w, 'j');
    clavis(w, '/');
    reditus(w);
    CREDO_VERUM(campus_est(w, "q", "kitchen"));
    CREDO_VERUM(campus_est(w, "genus", "res"));
    CREDO_VERUM(campus_est(w, "pagina", NIHIL));
    CREDO_VERUM(campus_est(w, "electus", NIHIL));
    /* Cmd+Shift+F */
    CREDO_VERUM(indagatio_visus_quaestionem_aperire(&w->m));
    CREDO_VERUM(campus_est(w, "prompta", "quaestio"));
    CREDO_VERUM(campus_est(w, "prompta_textus", "kitchen"));
    /* semita applicata promptam claudit */
    CREDO_VERUM(argumentum(w, "plumb"));
    CREDO_VERUM(campus_est(w, "prompta", NIHIL)
        || campus_est(w, "prompta", "nulla"));
}

/* electio: j k, Enter per portam; "+ k more" et Tab gregem aperiunt;
 * porta nulla: Enter nihil */
interior vacuum
_sectio_electio (
    CredoContextus* c)
{
    Mundus* w;

    w = mundus(c, D, VERUM);
    CREDO_NECESSE_NON_NIHIL(w);
    CREDO_VERUM(argumentum(w, "tart"));
    reditus(w);
    CREDO_AEQUALIS_I32(w->aperta, I);
    CREDO_CHORDA_AEQUALIS_LITERIS(w->apertum,
        "calendarium/dies/2026-10-21?res_electa=1");
    /* kitchen: Agenda sola - tart jam pie, + 2 more */
    CREDO_VERUM(argumentum(w, "kitchen"));
    clavis(w, 'k');
    CREDO_VERUM(campus_est(w, "electus", NIHIL)
        || campus_est(w, "electus", "0"));
    clavis(w, 'j');
    CREDO_VERUM(campus_est(w, "electus", "1"));
    /* sagittae ut j / k */
    clavis_nominata(w, CLAVIS_DEORSUM, ZEPHYRUM);
    CREDO_VERUM(campus_est(w, "electus", "2"));
    clavis_nominata(w, CLAVIS_SURSUM, ZEPHYRUM);
    CREDO_VERUM(campus_est(w, "electus", "1"));
    reditus(w);
    CREDO_AEQUALIS_I32(w->aperta, II);
    CREDO_CHORDA_AEQUALIS_LITERIS(w->apertum,
        "calendarium/agenda?res_electa=2");
    clavis(w, 'j');
    clavis(w, 'j');
    clavis(w, 'j');
    CREDO_VERUM(campus_est(w, "electus", "3"));
    reditus(w);
    CREDO_AEQUALIS_I32(w->aperta, II);
    CREDO_VERUM(campus_est(w, "genus", "res"));
    CREDO_VERUM(campus_est(w, "electus", NIHIL));
    CREDO_VERUM(pictum(w, "Rhubarb soup", c->piscina));
    /* Tab: grex electi */
    CREDO_VERUM(argumentum(w, "crumble"));
    clavis(w, '\t');
    CREDO_VERUM(campus_est(w, "genus", "notae"));
}

interior vacuum
_sectio_sine_porta (
    CredoContextus* c)
{
    Mundus* w;

    w = mundus(c, D, FALSUM);
    CREDO_NECESSE_NON_NIHIL(w);
    CREDO_VERUM(argumentum(w, "tart"));
    reditus(w);
    CREDO_AEQUALIS_I32(w->aperta, ZEPHYRUM);
    CREDO_VERUM(pictum(w, "Rhubarb tart", c->piscina));
}

/* grex paginatus (latus humile: inventa II per paginam), ] [ et j
 * trans paginam; Esc gradibus: grex -> conspectus -> q vacua ->
 * nihil */
interior vacuum
_sectio_grex (
    CredoContextus* c)
{
    Mundus* w;

    w = mundus(c, CXX, VERUM);
    CREDO_NECESSE_NON_NIHIL(w);
    CREDO_VERUM(argumentum(w, "indagatio?q=kitchen&genus=res"));
    CREDO_VERUM(pictum(w, "1/3", c->piscina));
    CREDO_VERUM(pictum(w, "Rhubarb tart", c->piscina));
    CREDO_VERUM(!pictum(w, "Rhubarb pie", c->piscina));
    clavis(w, ']');
    CREDO_VERUM(campus_est(w, "pagina", "1"));
    CREDO_VERUM(pictum(w, "2/3", c->piscina));
    CREDO_VERUM(pictum(w, "Rhubarb pie", c->piscina));
    clavis(w, ']');
    clavis(w, ']');
    CREDO_VERUM(campus_est(w, "pagina", "2"));
    clavis(w, '[');
    CREDO_VERUM(campus_est(w, "pagina", "1"));
    clavis(w, '[');
    CREDO_VERUM(campus_est(w, "pagina", NIHIL)
        || campus_est(w, "pagina", "0"));
    /* j trans finem paginae */
    clavis(w, 'j');
    CREDO_VERUM(campus_est(w, "electus", "1"));
    clavis(w, 'j');
    CREDO_VERUM(campus_est(w, "pagina", "1"));
    CREDO_VERUM(campus_est(w, "electus", NIHIL)
        || campus_est(w, "electus", "0"));
    clavis(w, 'k');
    CREDO_VERUM(campus_est(w, "pagina", NIHIL)
        || campus_est(w, "pagina", "0"));
    CREDO_VERUM(campus_est(w, "electus", "1"));
    /* fructus servatus: genus aliud, verba eadem */
    CREDO_VERUM(argumentum(w, "indagatio?q=rhubarb&genus=res"));
    CREDO_VERUM(pictum(w, "Rhubarb tart", c->piscina));
    CREDO_VERUM(argumentum(w, "indagatio?q=rhubarb&genus=notae"));
    CREDO_VERUM(pictum(w, "Mon 19 Oct 2026", c->piscina));
    CREDO_VERUM(!pictum(w, "Rhubarb tart", c->piscina));
    CREDO_VERUM(argumentum(w, "indagatio?q=kitchen&genus=res"));
    /* Esc gradibus */
    effugium(w);
    CREDO_VERUM(campus_est(w, "genus", NIHIL));
    CREDO_VERUM(campus_est(w, "pagina", NIHIL));
    CREDO_VERUM(campus_est(w, "q", "kitchen"));
    effugium(w);
    CREDO_VERUM(campus_est(w, "q", NIHIL));
    effugium(w);
    CREDO_VERUM(campus_est(w, "q", NIHIL));
}

/* excerptum: verbum quaesitum pars sua (signata), reliquum alia */
interior vacuum
_sectio_excerptum (
    CredoContextus* c)
{
    Mundus* w;

    w = mundus(c, D, VERUM);
    CREDO_NECESSE_NON_NIHIL(w);
    CREDO_VERUM(argumentum(w, "crumble"));
    CREDO_VERUM(pictum_forma(w, "crumble", VERUM, c->piscina));
    CREDO_VERUM(pictum(w, "recipe", c->piscina));
    CREDO_VERUM(!pictum_forma(w, "rhubarb crumble recipe", VERUM,
        c->piscina));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "semita",      _sectio_semita,      NIHIL, mundum_purgare, NIHIL,
        NIHIL },
    { "conspectus",  _sectio_conspectus,  NIHIL, mundum_purgare, NIHIL,
        NIHIL },
    { "quaestio",    _sectio_quaestio,    NIHIL, mundum_purgare, NIHIL,
        NIHIL },
    { "electio",     _sectio_electio,     NIHIL, mundum_purgare, NIHIL,
        NIHIL },
    { "sine porta",  _sectio_sine_porta,  NIHIL, mundum_purgare, NIHIL,
        NIHIL },
    { "grex",        _sectio_grex,        NIHIL, mundum_purgare, NIHIL,
        NIHIL },
    { "excerptum",   _sectio_excerptum,   NIHIL, mundum_purgare, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("indagatio_visus", SECTIONES);
}

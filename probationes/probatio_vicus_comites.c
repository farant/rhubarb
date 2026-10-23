/* probatio_vicus_comites.c - comites P1 (project-specs/comites.md):
 * LOCUS lateris et titulus eius per vicum. Compositio VERA
 * (vicus_applicatio, tabula 3), horologium fictum (hodie = Mon 19 Oct
 * 2026), persona "1" Anna Rossi, pictura "x1", sancti ex fixura.
 * Calendarium: hodie concreta, rotundatio ad unitatem visus (dies,
 * hebdomas a dominica, mensis, annus, agenda); nomenclator (persona,
 * index), sanctus, pictura, indagatio, admonitiones, colloquia. P3:
 * Cmd+T liber comes in latere sinistro (titulus vectis, focus
 * sinister), sequitur frontem dextram, pagina sua cuique comiti,
 * reditus ad librum communem, terminale focatum, reapertura, vectis
 * pingitur solum cum comite. P3b: liber quisque historiam suam habet
 * (Ctrl+O in comite librum numquam mutat). P5a: Cmd+F = aditus
 * "quaestio" lateris focati (scriba, calendarium, nomenclator, sancti,
 * indagatio; terminale nihil). P5b: comites quaeruntur - nota comitis
 * (titulus, semita visus), inventum comitis: comes sinister ad
 * paginam, visus comitatus dexter; Cmd+F in comite = liber ipse;
 * comes terminalis (sine semita): sinister solus. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "thema.h"
#include "volumen.h"
#include "insula.h"
#include "dispensator.h"
#include "moneta.h"
#include "personae.h"
#include "pictor_documentum.h"
#include "vicus.h"
#include "vicus_applicatio.h"
#include "scriba_applicatio.h"
#include "scriba_liber.h"
#include "scriba_documentum.h"
#include "tabula_characterum.h"
#include "manus_ludus.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

nomen structura {
            Volumen* volumen;
    VicusApplicatio  app;
              Vicus* v;
        VicusTempus  tempus;
         ManusLudus* m;
} Mundus;

interior vacuum
horologium_fictum (
         vacuum* ctx,
    VicusTempus* tempus)
{
    *tempus = *(VicusTempus*)ctx;
}

interior vacuum
mundum_parare (
    CredoContextus* c)
{
                 Mundus* x;
    InternamentumChorda* intern;
     PersonaeDocumentum* doc;
                    i32  numerator;

    x = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(x, ZEPHYRUM, magnitudo(Mundus));
    c->datum = x;
    thema_initiare();
    (vacuum)setenv("SHELL", "/bin/sh", I);
    (vacuum)setenv("RHUBARB_SANCTI", "probationes/fixa/sancti", I);
    intern = internamentum_creare(c->piscina);
    x->volumen = volumen_temporarium(c->piscina,
        "probatio_vicus_comites");
    si (!x->volumen)
    {
        redde;
    }
    doc = personae_aperire(c->piscina, intern, x->volumen, "personae",
        ZEPHYRUM);
    numerator = ZEPHYRUM;
    si (doc)
    {
        personae_monetam_ponere(doc, moneta_numerator, &numerator);
        (vacuum)personae_hominem_creare(doc, chorda_ex_literis("Anna",
            c->piscina), chorda_ex_literis("Rossi", c->piscina));
        (vacuum)personae_hominem_creare(doc, chorda_ex_literis("Bruno",
            c->piscina), chorda_ex_literis("Verdi", c->piscina));
    }
    (vacuum)pictor_documentum_creare(c->piscina, intern, x->volumen,
        "x1", C, LX, LXIV);
    si (!vicus_applicatio_aedificare(&x->app, c->piscina, intern,
            x->volumen, CDLXXX, CDLXXX + VICUS_ALTITUDO_TABULARUM))
    {
        redde;
    }
    x->v              = x->app.vicus;
    x->tempus.annus   = MMXXVI;
    x->tempus.mensis  = X;
    x->tempus.dies    = XIX;
    vicus_horologium_ponere(x->v, horologium_fictum, &x->tempus);
    (vacuum)vicus_activam_ponere(x->v, "3");
    (vacuum)vicus_pulsare(x->v);
    x->m = manus_ludus_creare(c->piscina, x->app.d);
    dispensator_recomponere(x->app.d);
}

interior vacuum
mundum_purgare (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    si (x && x->volumen)
    {
        volumen_claudere(x->volumen);
    }
}

/* latus in frontem (aperire aut mutare), pulsus */
interior vacuum
ostendere (
                 Mundus* x,
     constans character* genus,
     constans character* argumentum)
{
    (vacuum)vicus_acervo_mutare(x->v, genus, argumentum);
    (vacuum)vicus_pulsare(x->v);
    (vacuum)vicus_pulsare(x->v);
}

interior b32
locus_est (
                 Mundus* x,
     constans character* locus,
     constans character* titulus,
                Piscina* p)
{
    VicusLatus* l;
        chorda  a;
        chorda  b;

    l = vicus_latus(vicus_activa(x->v), VICUS_DEXTRUM);
    a = vicus_latus_locus(x->v, l, p);
    b = vicus_latus_locus_titulus(x->v, l, p);
    si (   !chorda_aequalis_literis(a, locus)
        || !chorda_aequalis_literis(b, titulus))
    {
        fprintf(stderr, "  locus '%.*s' titulus '%.*s'\n",
            (int)a.mensura, (constans character*)a.datum,
            (int)b.mensura, (constans character*)b.datum);
        redde FALSUM;
    }
    redde VERUM;
}

interior vacuum
_sectio_calendarium (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(x && x->v);

    CREDO_GRADUS("dies hodierna (absens): concreta");
    ostendere(x, "calendarium", "calendarium/dies");
    CREDO_VERUM(locus_est(x, "calendarium/dies/2026-10-19",
        "Mon 19 Oct 2026", c->piscina));

    CREDO_GRADUS("hebdomas: a dominica");
    ostendere(x, "calendarium", "calendarium/hebdomas/2026-10-21");
    CREDO_VERUM(locus_est(x, "calendarium/hebdomas/2026-10-18",
        "Week of 18 Oct 2026", c->piscina));

    CREDO_GRADUS("mensis, annus, agenda");
    ostendere(x, "calendarium", "calendarium/mensis/2026-10-19");
    CREDO_VERUM(locus_est(x, "calendarium/mensis/2026-10",
        "October 2026", c->piscina));
    ostendere(x, "calendarium", "calendarium/mensis/2026-10-03");
    CREDO_VERUM(locus_est(x, "calendarium/mensis/2026-10",
        "October 2026", c->piscina));
    ostendere(x, "calendarium", "calendarium/annus/2026-10-19");
    CREDO_VERUM(locus_est(x, "calendarium/annus/2026", "2026",
        c->piscina));
    ostendere(x, "calendarium", "calendarium/agenda");
    CREDO_VERUM(locus_est(x, "calendarium/agenda", "Agenda",
        c->piscina));
}

interior vacuum
_sectio_cetera (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(x && x->v);

    CREDO_GRADUS("nomenclator: persona, index");
    ostendere(x, "nomenclator", "nomenclator/1");
    CREDO_VERUM(locus_est(x, "nomenclator/1", "Anna Rossi",
        c->piscina));
    ostendere(x, "nomenclator", "nomenclator/");
    CREDO_VERUM(locus_est(x, "nomenclator", "Contacts", c->piscina));

    CREDO_GRADUS("sanctus");
    ostendere(x, "sancti", "sancti/sanctus/thomas-aquinas");
    CREDO_VERUM(locus_est(x, "sancti/sanctus/thomas-aquinas",
        "Thomas Aquinas", c->piscina));

    CREDO_GRADUS("pictura");
    ostendere(x, "pictor", "x1");
    CREDO_VERUM(locus_est(x, "pictor/x1", "x1", c->piscina));

    CREDO_GRADUS("indagatio, admonitiones, colloquia");
    ostendere(x, "indagatio", "rhubarb");
    CREDO_VERUM(locus_est(x, "indagatio", "Search", c->piscina));
    CREDO_VERUM(vicus_acervo_aperire(x->v, "admonitiones", NIHIL));
    (vacuum)vicus_pulsare(x->v);
    CREDO_VERUM(locus_est(x, "admonitiones", "Notifications",
        c->piscina));
    CREDO_VERUM(vicus_acervo_aperire(x->v, "colloquia", NIHIL));
    (vacuum)vicus_pulsare(x->v);
    CREDO_VERUM(locus_est(x, "colloquia", "Sessions", c->piscina));
}

/* scriba sinistra tabulae activae */
interior ScribaMontatio*
scriba_sinistra (
    Mundus* x)
{
    VicusLatus* l;

    l = vicus_latus(vicus_activa(x->v), VICUS_SINISTRUM);
    redde l ? (ScribaMontatio*)l->montatio : NIHIL;
}

/* comes sinister: praefixum libri, titulus vectis (NIHIL = nullus),
 * pagina */
interior b32
comes_est (
                 Mundus* x,
     constans character* praefixum,
     constans character* titulus,
     constans character* pagina)
{
    ScribaMontatio* m;
            chorda* a;
            chorda* t;
            chorda  pr;

    m = scriba_sinistra(x);
    si (!m || !m->liber)
    {
        redde FALSUM;
    }
    pr  = scriba_liber_praefixum(m->liber);
    a   = insula_ramus_attributum(&m->ramus, "pagina");
    t   = insula_ramus_attributum(&m->ramus, "liber_titulus");
    si (   !chorda_aequalis_literis(pr, praefixum)
        || !a || !chorda_aequalis_literis(*a, pagina)
        || (titulus ? !(t && chorda_aequalis_literis(*t, titulus))
                    : (t && t->mensura > ZEPHYRUM)))
    {
        fprintf(stderr, "  liber '%.*s' pagina '%.*s' titulus '%.*s'\n",
            (int)pr.mensura, (constans character*)pr.datum,
            a ? (int)a->mensura : 0, a ? (constans character*)a->datum
            : "", t ? (int)t->mensura : 0, t ? (constans character*)
            t->datum : "");
        redde FALSUM;
    }
    redde VERUM;
}

interior b32
focus_sinister (
    Mundus* x)
{
    redde vicus_latus_focatum(x->v)
        == vicus_latus(vicus_activa(x->v), VICUS_SINISTRUM);
}

/* clavis cum modificantibus, deinde pulsus */
interior vacuum
clavis (
       Mundus* x,
    character  typus,
          i32  modi)
{
    (vacuum)manus_ludus_clavem(x->m, typus, modi);
    (vacuum)vicus_pulsare(x->v);
    dispensator_recomponere(x->app.d);
}

/* Ctrl+Shift+Dextra: pagina proxima scribae focatae */
interior vacuum
pagina_proxima (
    Mundus* x)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                      = EVENTUS_CLAVIS_DEPRESSUS;
    e.datum.clavis.clavis        = CLAVIS_DEXTER;
    e.datum.clavis.modificantes  = (i32)(MOD_IMPERIUM | MOD_SHIFT);
    dispensator_tractare(x->app.d, &e);
    (vacuum)vicus_pulsare(x->v);
    dispensator_recomponere(x->app.d);
}

/* comites P3: Cmd+T, sequitur, pagina sua, reditus, vectis */
interior vacuum
_sectio_comes (
    CredoContextus* c)
{
             Mundus* x;
    VicusApplicatio  altera;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(x && x->v);
    CREDO_VERUM(vicus_focum_ponere(x->v, VICUS_DEXTRUM));
    ostendere(x, "nomenclator", "nomenclator/1");
    dispensator_recomponere(x->app.d);
    CREDO_VERUM(comes_est(x, "", NIHIL, "1"));
    CREDO_VERUM(manus_ludus_invenire(x->m, "#vectis") == NIHIL);

    CREDO_GRADUS("Cmd+Shift+T: nihil");
    clavis(x, 't', (i32)(MOD_SUPER | MOD_SHIFT));
    CREDO_VERUM(comes_est(x, "", NIHIL, "1"));

    CREDO_GRADUS("Cmd+T: comes personae, focus sinister, vectis");
    clavis(x, 't', (i32)MOD_SUPER);
    CREDO_VERUM(comes_est(x, "comes/nomenclator/1",
        "Notes \302\267 Anna Rossi", "1"));
    CREDO_VERUM(focus_sinister(x));
    CREDO_VERUM(manus_ludus_invenire(x->m, "#vectis") != NIHIL);
    pagina_proxima(x);
    CREDO_VERUM(comes_est(x, "comes/nomenclator/1",
        "Notes \302\267 Anna Rossi", "2"));

    CREDO_GRADUS("sequitur: persona altera, dies");
    ostendere(x, "nomenclator", "nomenclator/2");
    CREDO_VERUM(comes_est(x, "comes/nomenclator/2",
        "Notes \302\267 Bruno Verdi", "1"));
    ostendere(x, "calendarium", "calendarium/dies");
    CREDO_VERUM(comes_est(x, "comes/calendarium/dies/2026-10-19",
        "Notes \302\267 Mon 19 Oct 2026", "1"));
    ostendere(x, "nomenclator", "nomenclator/1");
    CREDO_VERUM(comes_est(x, "comes/nomenclator/1",
        "Notes \302\267 Anna Rossi", "2"));

    CREDO_GRADUS("Cmd+T iterum: liber communis, vectis nulla");
    clavis(x, 't', (i32)MOD_SUPER);
    CREDO_VERUM(comes_est(x, "", NIHIL, "1"));
    CREDO_VERUM(focus_sinister(x));
    CREDO_VERUM(manus_ludus_invenire(x->m, "#vectis") == NIHIL);
    /* non sequitur dum communis */
    ostendere(x, "nomenclator", "nomenclator/2");
    CREDO_VERUM(comes_est(x, "", NIHIL, "1"));

    CREDO_GRADUS("terminale focatum: Cmd+T");
    CREDO_VERUM(vicus_activam_ponere(x->v, "1"));
    CREDO_VERUM(vicus_focum_ponere(x->v, VICUS_DEXTRUM));
    dispensator_recomponere(x->app.d);
    clavis(x, 't', (i32)MOD_SUPER);
    CREDO_VERUM(comes_est(x, "comes/terminale/1_dextrum_terminale",
        "Notes \302\267 Terminal", "1"));
    clavis(x, 't', (i32)MOD_SUPER);
    CREDO_VERUM(comes_est(x, "", NIHIL, "1"));

    CREDO_GRADUS("reapertura: comes et sequela redeunt");
    CREDO_VERUM(vicus_activam_ponere(x->v, "3"));
    CREDO_VERUM(vicus_focum_ponere(x->v, VICUS_DEXTRUM));
    dispensator_recomponere(x->app.d);
    ostendere(x, "nomenclator", "nomenclator/1");
    clavis(x, 't', (i32)MOD_SUPER);
    CREDO_VERUM(comes_est(x, "comes/nomenclator/1",
        "Notes \302\267 Anna Rossi", "2"));
    memset(&altera, ZEPHYRUM, magnitudo(altera));
    CREDO_NECESSE(vicus_applicatio_aedificare(&altera, c->piscina,
        internamentum_creare(c->piscina), x->volumen, CDLXXX,
        CDLXXX + VICUS_ALTITUDO_TABULARUM));
    x->app  = altera;
    x->v    = altera.vicus;
    vicus_horologium_ponere(x->v, horologium_fictum, &x->tempus);
    CREDO_VERUM(vicus_activam_ponere(x->v, "3"));
    (vacuum)vicus_pulsare(x->v);
    (vacuum)vicus_pulsare(x->v);
    CREDO_VERUM(comes_est(x, "comes/nomenclator/1",
        "Notes \302\267 Anna Rossi", "2"));
}

/* comites P3b: historia per librum - Ctrl+O in comite intra eum
 * manet; liber communis historiam suam servat; mutatio libri locum
 * non facit */
interior vacuum
_sectio_historia_comitis (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(x && x->v);
    CREDO_VERUM(vicus_focum_ponere(x->v, VICUS_DEXTRUM));
    ostendere(x, "nomenclator", "nomenclator/1");
    CREDO_VERUM(vicus_focum_ponere(x->v, VICUS_SINISTRUM));
    dispensator_recomponere(x->app.d);
    pagina_proxima(x);
    CREDO_VERUM(comes_est(x, "", NIHIL, "2"));

    CREDO_GRADUS("comes: paginae 1 -> 2, Ctrl+O intra comitem");
    clavis(x, 't', (i32)MOD_SUPER);
    CREDO_VERUM(comes_est(x, "comes/nomenclator/1",
        "Notes \302\267 Anna Rossi", "1"));
    pagina_proxima(x);
    CREDO_VERUM(comes_est(x, "comes/nomenclator/1",
        "Notes \302\267 Anna Rossi", "2"));
    clavis(x, 'o', (i32)MOD_IMPERIUM);
    CREDO_VERUM(comes_est(x, "comes/nomenclator/1",
        "Notes \302\267 Anna Rossi", "1"));
    clavis(x, 'o', (i32)MOD_IMPERIUM);
    CREDO_VERUM(comes_est(x, "comes/nomenclator/1",
        "Notes \302\267 Anna Rossi", "1"));
    clavis(x, 'i', (i32)MOD_IMPERIUM);
    CREDO_VERUM(comes_est(x, "comes/nomenclator/1",
        "Notes \302\267 Anna Rossi", "2"));

    CREDO_GRADUS("comes alius (sequela): historia sua");
    ostendere(x, "nomenclator", "nomenclator/2");
    CREDO_VERUM(vicus_focum_ponere(x->v, VICUS_SINISTRUM));
    dispensator_recomponere(x->app.d);
    CREDO_VERUM(comes_est(x, "comes/nomenclator/2",
        "Notes \302\267 Bruno Verdi", "1"));
    clavis(x, 'o', (i32)MOD_IMPERIUM);
    CREDO_VERUM(comes_est(x, "comes/nomenclator/2",
        "Notes \302\267 Bruno Verdi", "1"));

    CREDO_GRADUS("liber communis: historia sua");
    clavis(x, 't', (i32)MOD_SUPER);
    CREDO_VERUM(comes_est(x, "", NIHIL, "2"));
    clavis(x, 'o', (i32)MOD_IMPERIUM);
    CREDO_VERUM(comes_est(x, "", NIHIL, "1"));
    clavis(x, 'i', (i32)MOD_IMPERIUM);
    CREDO_VERUM(comes_est(x, "", NIHIL, "2"));

    CREDO_GRADUS("reditus ad comitem primum: historia integra");
    CREDO_VERUM(vicus_focum_ponere(x->v, VICUS_DEXTRUM));
    ostendere(x, "nomenclator", "nomenclator/1");
    clavis(x, 't', (i32)MOD_SUPER);
    CREDO_VERUM(comes_est(x, "comes/nomenclator/1",
        "Notes \302\267 Anna Rossi", "2"));
    clavis(x, 'o', (i32)MOD_IMPERIUM);
    CREDO_VERUM(comes_est(x, "comes/nomenclator/1",
        "Notes \302\267 Anna Rossi", "1"));
}

/* campus rami lateris frontalis (genus eius) = valor */
interior b32
frontis_campus_est (
                 Mundus* x,
     constans character* genus,
     constans character* titulus,
     constans character* valor)
{
     VicusLatus* l;
    InsulaRamus  r;
         chorda* a;
      character  id[CXXVIII];

    l = vicus_latus(vicus_activa(x->v), VICUS_DEXTRUM);
    si (   !l || !l->montata
        || !chorda_aequalis_literis(l->genus, genus)
        || l->id.mensura >= (i32)magnitudo(id))
    {
        redde FALSUM;
    }
    sprintf(id, "%.*s", (int)l->id.mensura,
        (constans character*)l->id.datum);
    r = insula_ramus(x->v->repo, genus, id);
    a = insula_ramus_attributum(&r, titulus);
    redde a && chorda_aequalis_literis(*a, valor);
}

/* genus frontis dextrae */
interior b32
frons_generis_est (
                 Mundus* x,
     constans character* genus)
{
    VicusLatus* l;

    l = vicus_latus(vicus_activa(x->v), VICUS_DEXTRUM);
    redde l && chorda_aequalis_literis(l->genus, genus);
}

interior b32
focus_dexter (
    Mundus* x)
{
    redde vicus_latus_focatum(x->v)
        == vicus_latus(vicus_activa(x->v), VICUS_DEXTRUM);
}

/* comites P5a: Cmd+F = aditus "quaestio"; Esc in prompta advocata
 * aditum revocat (frons prior, focus unde venit) */
interior vacuum
_sectio_aditus (
    CredoContextus* c)
{
        Mundus* x;
    VicusLatus* l;
     character  prior[CXXVIII];
           i32  n;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(x && x->v);
    l = vicus_latus(vicus_activa(x->v), VICUS_DEXTRUM);
    CREDO_NECESSE(l && l->id.mensura < (i32)magnitudo(prior));
    sprintf(prior, "%.*s", (int)l->id.mensura,
        (constans character*)l->id.datum);
    n = (i32)xar_numerus(vicus_activa(x->v)->acervus);

    CREDO_GRADUS("scriba (liber communis): quaestio libri");
    CREDO_VERUM(vicus_focum_ponere(x->v, VICUS_SINISTRUM));
    clavis(x, 'f', (i32)MOD_SUPER);
    CREDO_VERUM(frontis_campus_est(x, "indagatio", "genus", "liber"));
    CREDO_VERUM(frontis_campus_est(x, "indagatio", "prompta",
        "quaestio"));
    CREDO_VERUM(focus_dexter(x));

    CREDO_GRADUS("Esc: Cmd+F revocatur (frons prior, focus sinister)");
    clavis(x, (character)XXVII, ZEPHYRUM);
    (vacuum)vicus_pulsare(x->v);
    l = vicus_latus(vicus_activa(x->v), VICUS_DEXTRUM);
    CREDO_VERUM(l && chorda_aequalis_literis(l->id, prior));
    CREDO_VERUM(focus_sinister(x));
    CREDO_AEQUALIS_I32((i32)xar_numerus(vicus_activa(x->v)->acervus),
        n + I);

    CREDO_GRADUS("scriba iterum (indagatio in acervo): focus dexter");
    clavis(x, 'f', (i32)MOD_SUPER);
    CREDO_VERUM(frontis_campus_est(x, "indagatio", "genus", "liber"));
    CREDO_VERUM(focus_dexter(x));

    CREDO_GRADUS("Enter: quaestio manet (Cmd+F non revocatur)");
    (vacuum)manus_ludus_scribere(x->m, "rhubarb");
    clavis(x, (character)XIII, ZEPHYRUM);
    (vacuum)vicus_pulsare(x->v);
    CREDO_VERUM(frontis_campus_est(x, "indagatio", "q", "rhubarb"));
    CREDO_VERUM(focus_dexter(x));

    CREDO_GRADUS("indagatio ipsa: prompta sua, Esc eam solum claudit");
    clavis(x, 'f', (i32)MOD_SUPER);
    CREDO_VERUM(frontis_campus_est(x, "indagatio", "prompta",
        "quaestio"));
    CREDO_VERUM(frontis_campus_est(x, "indagatio", "genus", "liber"));
    clavis(x, (character)XXVII, ZEPHYRUM);
    (vacuum)vicus_pulsare(x->v);
    CREDO_VERUM(frontis_campus_est(x, "indagatio", "q", "rhubarb"));
    CREDO_VERUM(focus_dexter(x));

    CREDO_GRADUS("calendarium: res agendae, Esc reddit calendarium");
    ostendere(x, "calendarium", "calendarium/dies");
    CREDO_VERUM(vicus_focum_ponere(x->v, VICUS_DEXTRUM));
    dispensator_recomponere(x->app.d);
    clavis(x, 'f', (i32)MOD_SUPER);
    CREDO_VERUM(frontis_campus_est(x, "indagatio", "genus", "res"));
    CREDO_VERUM(frontis_campus_est(x, "indagatio", "prompta",
        "quaestio"));
    clavis(x, (character)XXVII, ZEPHYRUM);
    (vacuum)vicus_pulsare(x->v);
    CREDO_VERUM(frons_generis_est(x, "calendarium"));
    CREDO_VERUM(focus_dexter(x));

    CREDO_GRADUS("ictus muris aditum finit");
    clavis(x, 'f', (i32)MOD_SUPER);
    CREDO_VERUM(frontis_campus_est(x, "indagatio", "prompta",
        "quaestio"));
    CREDO_VERUM(vicus_activa(x->v)->aditus_gradus != ZEPHYRUM);
    (vacuum)manus_ludus_premere_ad(x->m, CDXL, CCCXC);
    (vacuum)vicus_pulsare(x->v);
    CREDO_VERUM(vicus_activa(x->v)->aditus_gradus == ZEPHYRUM);
    clavis(x, (character)XXVII, ZEPHYRUM);

    CREDO_GRADUS("nomenclator: quaestio sua");
    ostendere(x, "nomenclator", "nomenclator/");
    CREDO_VERUM(vicus_focum_ponere(x->v, VICUS_DEXTRUM));
    dispensator_recomponere(x->app.d);
    n = (i32)xar_numerus(vicus_activa(x->v)->acervus);
    clavis(x, 'f', (i32)MOD_SUPER);
    CREDO_VERUM(frontis_campus_est(x, "nomenclator", "prompta",
        "quaestio"));
    CREDO_AEQUALIS_I32((i32)xar_numerus(vicus_activa(x->v)->acervus),
        n);
    clavis(x, (character)XXVII, ZEPHYRUM);
    CREDO_VERUM(frons_generis_est(x, "nomenclator"));

    CREDO_GRADUS("sancti: index cum quaestione, Esc reddit sanctum");
    ostendere(x, "sancti", "sancti/sanctus/thomas-aquinas");
    CREDO_VERUM(vicus_focum_ponere(x->v, VICUS_DEXTRUM));
    dispensator_recomponere(x->app.d);
    clavis(x, 'f', (i32)MOD_SUPER);
    CREDO_VERUM(frontis_campus_est(x, "sancti", "visus", "index"));
    CREDO_VERUM(frontis_campus_est(x, "sancti", "prompta", "quaestio"));
    clavis(x, (character)XXVII, ZEPHYRUM);
    (vacuum)vicus_pulsare(x->v);
    CREDO_VERUM(frontis_campus_est(x, "sancti", "visus", "sanctus"));
    CREDO_VERUM(frontis_campus_est(x, "sancti", "sanctus_apertus",
        "thomas-aquinas"));

    CREDO_GRADUS("terminale: nihil");
    CREDO_VERUM(vicus_activam_ponere(x->v, "1"));
    CREDO_VERUM(vicus_focum_ponere(x->v, VICUS_DEXTRUM));
    dispensator_recomponere(x->app.d);
    n = (i32)xar_numerus(vicus_activa(x->v)->acervus);
    clavis(x, 'f', (i32)MOD_SUPER);
    CREDO_AEQUALIS_I32((i32)xar_numerus(vicus_activa(x->v)->acervus),
        n);
    CREDO_VERUM(frons_generis_est(x, "terminale"));
}

/* paginam currentem comitis sinistri textu dato scribere */
interior b32
comitem_scribere (
                 Mundus* x,
     constans character* textus,
                Piscina* piscina)
{
       ScribaMontatio* m;
    TabulaCharacterum  folium;

    m = scriba_sinistra(x);
    si (!m || !m->doc)
    {
        redde FALSUM;
    }
    tabula_ex_literis_cum_dimensionibus(&folium, piscina,
        m->doc->latitudo, m->doc->altitudo, textus);
    redde scriba_documentum_committere(m->doc, &folium) > ZEPHYRUM;
}

/* Cmd+Shift+F, verba, Enter (prompta), deinde Enter (inventum primum) */
interior vacuum
quaerere_et_aperire (
                 Mundus* x,
     constans character* verba)
{
    clavis(x, 'f', (i32)(MOD_SUPER | MOD_SHIFT));
    (vacuum)manus_ludus_scribere(x->m, verba);
    clavis(x, (character)XIII, ZEPHYRUM);
    clavis(x, (character)XIII, ZEPHYRUM);
    (vacuum)vicus_pulsare(x->v);
    dispensator_recomponere(x->app.d);
}

interior b32
incipit (
                 chorda  c,
     constans character* praefixum)
{
    redde c.mensura >= (i32)strlen(praefixum)
        && memcmp(c.datum, praefixum, strlen(praefixum)) == ZEPHYRUM;
}

/* comites P5b: nota comitis, inventum comitis (sinister + dexter),
 * Cmd+F in comite, comes sine semita */
interior vacuum
_sectio_quaerendi (
    CredoContextus* c)
{
                 Mundus* x;
    InternamentumChorda* intern;
                 chorda  titulus;
                 chorda  semita;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(x && x->v);
    intern = internamentum_creare(c->piscina);

    CREDO_GRADUS("Cmd+T in die: nota comitis (titulus, semita)");
    CREDO_VERUM(vicus_focum_ponere(x->v, VICUS_DEXTRUM));
    ostendere(x, "calendarium", "calendarium/dies/2026-10-22");
    dispensator_recomponere(x->app.d);
    clavis(x, 't', (i32)MOD_SUPER);
    CREDO_VERUM(comes_est(x, "comes/calendarium/dies/2026-10-22",
        "Notes \302\267 Thu 22 Oct 2026", "1"));
    CREDO_VERUM(scriba_liber_comitem_legere(c->piscina, intern,
        x->volumen, "comes/calendarium/dies/2026-10-22", &titulus,
        &semita));
    CREDO_CHORDA_AEQUALIS_LITERIS(titulus,
        "Notes \302\267 Thu 22 Oct 2026");
    CREDO_VERUM(incipit(semita, "calendarium/dies/2026-10-22"));
    CREDO_VERUM(comitem_scribere(x, "rhubarb crumble for Thursday",
        c->piscina));
    /* pagina secunda per librum applicationis ipsum */
    {
           ScribaMontatio* m;
         ScribaDocumentum* d;
        TabulaCharacterum  folium;

        m = scriba_sinistra(x);
        CREDO_NECESSE(m && m->liber);
        d = scriba_liber_paginam_condere(m->liber,
            chorda_ex_literis("jam",
            c->piscina));
        CREDO_NECESSE_NON_NIHIL(d);
        tabula_ex_literis_cum_dimensionibus(&folium, c->piscina,
            d->latitudo, d->altitudo, "gooseberry fool");
        CREDO_VERUM(scriba_documentum_committere(d, &folium)
            > ZEPHYRUM);
    }

    CREDO_GRADUS("inventum comitis: comes ad paginam, dies dexter");
    clavis(x, 't', (i32)MOD_SUPER);
    CREDO_VERUM(comes_est(x, "", NIHIL, "1"));
    CREDO_VERUM(vicus_focum_ponere(x->v, VICUS_DEXTRUM));
    ostendere(x, "nomenclator", "nomenclator/1");
    dispensator_recomponere(x->app.d);
    quaerere_et_aperire(x, "gooseberry");
    CREDO_VERUM(comes_est(x, "comes/calendarium/dies/2026-10-22",
        "Notes \302\267 Thu 22 Oct 2026", "jam"));
    CREDO_VERUM(locus_est(x, "calendarium/dies/2026-10-22",
        "Thu 22 Oct 2026", c->piscina));
    CREDO_VERUM(focus_sinister(x));

    CREDO_GRADUS("Cmd+F in comite: liber ipse");
    clavis(x, 'f', (i32)MOD_SUPER);
    CREDO_VERUM(frontis_campus_est(x, "indagatio", "genus", "comites"));
    CREDO_VERUM(frontis_campus_est(x, "indagatio", "clavis",
        "calendarium/dies/2026-10-22"));
    CREDO_VERUM(frontis_campus_est(x, "indagatio", "prompta",
        "quaestio"));
    clavis(x, (character)XXVII, ZEPHYRUM);

    CREDO_GRADUS("comes terminalis (sine semita): sinister solus");
    CREDO_VERUM(vicus_activam_ponere(x->v, "1"));
    CREDO_VERUM(vicus_focum_ponere(x->v, VICUS_DEXTRUM));
    (vacuum)vicus_pulsare(x->v);
    dispensator_recomponere(x->app.d);
    clavis(x, 't', (i32)MOD_SUPER);
    CREDO_VERUM(comes_est(x, "comes/terminale/1_dextrum_terminale",
        "Notes \302\267 Terminal", "1"));
    CREDO_VERUM(scriba_liber_comitem_legere(c->piscina, intern,
        x->volumen, "comes/terminale/1_dextrum_terminale", &titulus,
        &semita));
    CREDO_AEQUALIS_I32(semita.mensura, ZEPHYRUM);
    CREDO_VERUM(comitem_scribere(x, "plumber xyzzy", c->piscina));
    clavis(x, 't', (i32)MOD_SUPER);
    CREDO_VERUM(vicus_focum_ponere(x->v, VICUS_DEXTRUM));
    dispensator_recomponere(x->app.d);
    quaerere_et_aperire(x, "xyzzy");
    CREDO_VERUM(comes_est(x, "comes/terminale/1_dextrum_terminale",
        "Notes \302\267 Terminal", "1"));
    CREDO_VERUM(frons_generis_est(x, "indagatio"));
    (vacuum)vicus_pulsare(x->v);
    CREDO_VERUM(comes_est(x, "comes/terminale/1_dextrum_terminale",
        "Notes \302\267 Terminal", "1"));

    CREDO_GRADUS("Cmd+F in comite terminalis: alterius liber non");
    clavis(x, 'f', (i32)MOD_SUPER);
    CREDO_VERUM(frontis_campus_est(x, "indagatio", "clavis",
        "terminale/1_dextrum_terminale"));
    (vacuum)manus_ludus_scribere(x->m, "gooseberry");
    clavis(x, (character)XIII, ZEPHYRUM);
    clavis(x, (character)XIII, ZEPHYRUM);
    (vacuum)vicus_pulsare(x->v);
    CREDO_VERUM(comes_est(x, "comes/terminale/1_dextrum_terminale",
        "Notes \302\267 Terminal", "1"));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "calendarium", _sectio_calendarium, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { "cetera", _sectio_cetera, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "comes", _sectio_comes, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "historia_comitis", _sectio_historia_comitis, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "quaerendi", _sectio_quaerendi, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { "aditus", _sectio_aditus, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("vicus_comites", SECTIONES);
}

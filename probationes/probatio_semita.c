/* probatio_semita.c - semitae R1 (project-specs/semitae-spec.md):
 * applicare (ligatio, resetum ambitus ligationis, status, liberi,
 * scriptores plures), legere (forma canonica), recusationes nominatae
 * (nihil scriptum), URI utrimque (via, quaestio punctata, effugium),
 * lista URI (ligatio propria); D5 S3c: rami liberorum
 * "<id parentis>_nomen" (conventio vici); comites P1: locus (ligatio
 * radicis sola). Fixtura: organa proba
 * (radix), folium (liber, ligatio hereditaria), folium_liber (ligatio
 * propria). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "stml.h"
#include "insula.h"
#include "organum.h"
#include "organum_montatio.h"
#include "filum.h"
#include "semita.h"
#include "credo.h"
#include <string.h>

hic_manens constans character manifestum_probae[] =
    "<organum genus=\"proba\" versio=\"1\">"
    "<actiones><actio nomen=\"proba.agere\"/>"
    "<actio nomen=\"proba.ordo\"/></actiones>"
    "<status>"
    "<campus nomen=\"visus\" typus=\"electio\" vita=\"persistens\""
    " absens=\"index\" semita=\"ligatio\" historia=\"impelle\""
    " scriptor=\"proba.agere\"><optio>index</optio><optio>res</optio>"
    "</campus>"
    "<campus nomen=\"res\" typus=\"textus\" vita=\"persistens\""
    " semita=\"ligatio\" scriptor=\"proba.agere\"/>"
    "<campus nomen=\"ordo\" typus=\"electio\" vita=\"persistens\""
    " absens=\"nomen\" semita=\"status\" scriptor=\"proba.ordo\">"
    "<optio>nomen</optio><optio>dies</optio></campus>"
    "<campus nomen=\"pagina\" typus=\"numerus\" vita=\"persistens\""
    " absens=\"0\" semita=\"status\" ambitus=\"ligatio\""
    " scriptor=\"proba.agere\"/>"
    "<campus nomen=\"cursor\" typus=\"numerus\" vita=\"transiens\""
    " scriptor=\"proba.agere\"/>"
    "</status>"
    "<montationes><montatio nomen=\"notae\" genus=\"folium\"/>"
    "<montatio nomen=\"alter\" genus=\"folium_liber\"/></montationes>"
    "<cognomina>"
    "<cognomen nomen=\"7\" semita=\"proba/res/septem\"/>"
    "<cognomen semita=\"proba/\"/>"
    "<cognomen typus=\"numerus\" semita=\"proba/res/{}\"/>"
    "<cognomen typus=\"textus\" semita=\"proba/index/{}\"/>"
    "</cognomina>"
    "</organum>";

hic_manens constans character manifestum_folii[] =
    "<organum genus=\"folium\" versio=\"1\">"
    "<ligamina><ligamen nomen=\"liber\" genus=\"liber\" a=\"parens\"/>"
    "</ligamina>"
    "<actiones><actio nomen=\"folium.agere\"/></actiones>"
    "<status>"
    "<campus nomen=\"liber\" typus=\"textus\" vita=\"persistens\""
    " semita=\"ligatio\" scriptor=\"folium.agere\"/>"
    "<campus nomen=\"pagina\" typus=\"numerus\" vita=\"persistens\""
    " absens=\"0\" semita=\"status\" scriptor=\"folium.agere\"/>"
    "</status></organum>";

hic_manens constans character manifestum_folii_liberi[] =
    "<organum genus=\"folium_liber\" versio=\"1\">"
    "<actiones><actio nomen=\"folium_liber.agere\"/></actiones>"
    "<status>"
    "<campus nomen=\"dies\" typus=\"dies\" vita=\"persistens\""
    " semita=\"ligatio\" scriptor=\"folium_liber.agere\"/>"
    "<campus nomen=\"pagina\" typus=\"numerus\" vita=\"persistens\""
    " absens=\"0\" semita=\"status\" scriptor=\"folium_liber.agere\"/>"
    "</status></organum>";

/* R4d: ligatio partim transiens (visus persistens, dies transiens) */
hic_manens constans character manifestum_tabellae[] =
    "<organum genus=\"tabella\" versio=\"1\">"
    "<actiones><actio nomen=\"tabella.agere\"/></actiones>"
    "<status>"
    "<campus nomen=\"visus\" typus=\"electio\" vita=\"persistens\""
    " absens=\"mensis\" semita=\"ligatio\" scriptor=\"tabella.agere\">"
    "<optio>mensis</optio><optio>dies</optio></campus>"
    "<campus nomen=\"dies\" typus=\"dies\" vita=\"transiens\""
    " semita=\"ligatio\" scriptor=\"tabella.agere\"/>"
    "<campus nomen=\"filtrum\" typus=\"textus\" vita=\"persistens\""
    " semita=\"status\" scriptor=\"tabella.agere\"/>"
    "<campus nomen=\"pagina\" typus=\"numerus\" vita=\"transiens\""
    " semita=\"status\" scriptor=\"tabella.agere\"/>"
    "</status>"
    "<cognomina><cognomen typus=\"dies\" semita=\"tabella/dies/{}\"/>"
    "</cognomina></organum>";

/* R4d: ligatio tota transiens (persona), status persistens (modus) */
hic_manens constans character manifestum_catalogi[] =
    "<organum genus=\"catalogus\" versio=\"1\">"
    "<actiones><actio nomen=\"catalogus.agere\"/></actiones>"
    "<status>"
    "<campus nomen=\"modus\" typus=\"textus\" vita=\"persistens\""
    " semita=\"status\" scriptor=\"catalogus.agere\"/>"
    "<campus nomen=\"persona\" typus=\"numerus\" vita=\"transiens\""
    " semita=\"ligatio\" scriptor=\"catalogus.agere\"/>"
    "</status>"
    "<cognomina><cognomen typus=\"textus\""
    " semita=\"catalogus?modus={}\"/></cognomina></organum>";

nomen structura {
     InternamentumChorda* intern;
      InsulaRepositorium* repo;
                 Organum* proba;
                 Organum* folium;
                 Organum* folium_liber;
                 Organum* tabella;
                 Organum* catalogus;
             InsulaRamus  ramus;
             InsulaRamus  notae;
             InsulaRamus  alter;
} Mundus;

/* SemitaOrganum fixturae */
interior constans Organum*
organa (
     vacuum* ctx,
     chorda  genus)
{
    Mundus* w = (Mundus*)ctx;

    si (chorda_aequalis_literis(genus, "proba"))
    {
        redde w->proba;
    }
    si (chorda_aequalis_literis(genus, "folium"))
    {
        redde w->folium;
    }
    si (chorda_aequalis_literis(genus, "folium_liber"))
    {
        redde w->folium_liber;
    }
    si (chorda_aequalis_literis(genus, "tabella"))
    {
        redde w->tabella;
    }
    si (chorda_aequalis_literis(genus, "catalogus"))
    {
        redde w->catalogus;
    }
    redde NIHIL;
}

/* manifestum fixturae cum basi communi (ut organa vera:
 * organum_legere_viam) - rami parati campos basis scribunt */
interior Organum*
manifestum (
     constans character* fons,
                 Mundus* w,
                Piscina* p)
{
    Organum* o;
    Organum* basis;

    o      = organum_legere(chorda_ex_literis(fons, p), p, w->intern,
        NIHIL);
    basis  = organum_legere(filum_legere_totum(
        "apps/organum_commune.stml", p), p, w->intern, NIHIL);
    redde (o && basis && organum_basim_addere(o, basis)) ? o : NIHIL;
}

/* repositorium cum ramis parati (domini installati); liberi = montare
 * liberos? */
interior Mundus*
mundus (
    CredoContextus* c,
               b32  liberi)
{
    Mundus* w;

    w = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(w, ZEPHYRUM, magnitudo(Mundus));
    w->intern        = internamentum_creare(c->piscina);
    w->repo          = insula_repositorium_creare(c->piscina, w->intern,
        "<hospes/>");
    w->proba   = manifestum(manifestum_probae, w, c->piscina);
    w->folium  = manifestum(manifestum_folii, w, c->piscina);
    w->folium_liber  = manifestum(manifestum_folii_liberi, w,
        c->piscina);
    w->tabella    = manifestum(manifestum_tabellae, w, c->piscina);
    w->catalogus  = manifestum(manifestum_catalogi, w, c->piscina);
    si (   !w->repo || !w->proba || !w->folium || !w->folium_liber
        || !w->tabella || !w->catalogus)
    {
        redde NIHIL;
    }
    w->ramus = insula_ramus(w->repo, "proba", "p1");
    si (!organum_ramum_parare(w->proba, &w->ramus, NIHIL, NIHIL, NIHIL,
            c->piscina, w->intern))
    {
        redde NIHIL;
    }
    si (liberi)
    {
        w->notae  = insula_ramus_liberi(&w->ramus, "folium", "notae");
        w->alter  = insula_ramus_liberi(&w->ramus, "folium_liber",
            "alter");
        si (   !organum_ramum_parare(w->folium, &w->notae, NIHIL, NIHIL,
                   NIHIL, c->piscina, w->intern)
            || !organum_ramum_parare(w->folium_liber, &w->alter, NIHIL,
                   NIHIL, NIHIL, c->piscina, w->intern))
        {
            redde NIHIL;
        }
    }
    redde w;
}

interior StmlNodus*
arbor (
     constans character* textus,
                 Mundus* w,
                Piscina* p)
{
    StmlResultus r;

    r = stml_legere_ex_literis(textus, p, w->intern);
    redde r.successus ? r.elementum_radix : NIHIL;
}

interior b32
applicare (
                 Mundus* w,
     constans character* textus,
                Piscina* p,
                 chorda* vitium)
{
    redde semita_applicare(organa, w, &w->ramus, arbor(textus, w, p), p,
        vitium);
}

/* attributum rami = valor (NIHIL = absens) */
interior b32
campus_est (
    constans InsulaRamus* ramus,
      constans character* titulus,
      constans character* valor)
{
    chorda* a;

    a = insula_ramus_attributum(ramus, titulus);
    si (valor == NIHIL)
    {
        redde a == NIHIL || a->mensura == ZEPHYRUM;
    }
    redde a && chorda_aequalis_literis(*a, valor);
}

/* attributum rami scribere scriptore dato (ut actio widgeti) */
interior b32
ponere_scriptore (
    constans InsulaRamus* ramus,
      constans character* scriptor,
      constans character* titulus,
      constans character* valor,
                 Piscina* p);

interior chorda
uri (
                 Mundus* w,
     constans character* textus,
                Piscina* p)
{
    redde semita_ad_uri(organa, w, arbor(textus, w, p), p);
}

nomen structura {
    constans character* titulus;
    constans character* valor;
} ParProbae;

interior vacuum
_par_ponere (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    ParProbae* par = (ParProbae*)ctx;

    insula_attributum_ponere(radix, p, in, par->titulus, par->valor);
}

interior b32
ponere_scriptore (
    constans InsulaRamus* ramus,
      constans character* scriptor,
      constans character* titulus,
      constans character* valor,
                 Piscina* p)
{
    ParProbae par;
       chorda prior;
          b32 ok;

    par.titulus  = titulus;
    par.valor    = valor;
    prior        = ramus->repo->scriptor;
    insula_scriptorem_ponere(ramus->repo, chorda_ex_literis(scriptor,
        p));
    ok = mutare_ramum(ramus, _par_ponere, &par);
    insula_scriptorem_ponere(ramus->repo, prior);
    redde ok;
}

/* applicare: ligatio, status, liberi; scriptores duo (ordo proprius);
 * legere = forma canonica (absentes omissi, hereditaria omissa) */
interior vacuum
_sectio_applicare (
    CredoContextus* c)
{
      Mundus* w;
      chorda  vitium;
   StmlNodus* s;

    w = mundus(c, VERUM);
    CREDO_NECESSE_NON_NIHIL(w);
    /* status ordinarius: ligatio sola nominata (R4d: campus primus ad
     * absens), liberi nulli */
    s = semita_legere(organa, w, &w->ramus, "proba", c->piscina,
        w->intern);
    CREDO_NECESSE_NON_NIHIL(s);
    CREDO_CHORDA_AEQUALIS_LITERIS(semita_ad_uri(organa, w, s,
        c->piscina), "proba/index");
    CREDO_VERUM(!s->liberi || xar_numerus(s->liberi) == ZEPHYRUM);
    /* ligatio hereditaria posita (a parente): legendo omittitur */
    CREDO_NECESSE(ponere_scriptore(&w->notae, "folium.agere", "liber",
        "dies-1", c->piscina));
    CREDO_VERUM(applicare(w, "<proba visus=\"res\" res=\"alpha\""
        " ordo=\"dies\" pagina=\"2\"><notae pagina=\"4\"/>"
        "<alter dies=\"2026-10-19\" pagina=\"3\"/></proba>", c->piscina,
        &vitium));
    CREDO_VERUM(campus_est(&w->ramus, "visus", "res"));
    CREDO_VERUM(campus_est(&w->ramus, "res", "alpha"));
    CREDO_VERUM(campus_est(&w->ramus, "ordo", "dies"));
    CREDO_VERUM(campus_est(&w->ramus, "pagina", "2"));
    CREDO_VERUM(campus_est(&w->notae, "pagina", "4"));
    CREDO_VERUM(campus_est(&w->alter, "dies", "2026-10-19"));
    CREDO_VERUM(campus_est(&w->alter, "pagina", "3"));
    /* legere: canonica; ordo manifesti */
    s = semita_legere(organa, w, &w->ramus, "proba", c->piscina,
        w->intern);
    CREDO_NECESSE_NON_NIHIL(s);
    CREDO_CHORDA_AEQUALIS_LITERIS(semita_ad_uri(organa, w, s,
        c->piscina), "proba/res/alpha?ordo=dies&pagina=2&notae.pagina=4"
        "&alter.dies=2026-10-19&alter.pagina=3");
    /* valor absens (ordo nomen) omittitur; cursor (extra semitam)
     * numquam */
    CREDO_VERUM(applicare(w, "<proba ordo=\"nomen\"/>", c->piscina,
        &vitium));
    s = semita_legere(organa, w, &w->ramus, "proba", c->piscina,
        w->intern);
    CREDO_CHORDA_AEQUALIS_LITERIS(semita_ad_uri(organa, w, s,
        c->piscina), "proba/res/alpha?pagina=2&notae.pagina=4"
        "&alter.dies=2026-10-19&alter.pagina=3");
    /* idempotens: semitam suam applicare nihil mutat */
    CREDO_VERUM(semita_applicare(organa, w, &w->ramus, s, c->piscina,
        &vitium));
    CREDO_CHORDA_AEQUALIS_LITERIS(semita_ad_uri(organa, w,
        semita_legere(organa, w, &w->ramus, "proba", c->piscina,
        w->intern), c->piscina), "proba/res/alpha?pagina=2"
        "&notae.pagina=4&alter.dies=2026-10-19&alter.pagina=3");
}

/* absentes: ambitus ligationis resettitur si ligatio mutata, aliter
 * servatur; ambitus organi semper servatur */
interior vacuum
_sectio_ambitus (
    CredoContextus* c)
{
     Mundus* w;
     chorda  vitium;

    w = mundus(c, VERUM);
    CREDO_NECESSE_NON_NIHIL(w);
    CREDO_VERUM(applicare(w, "<proba visus=\"res\" res=\"alpha\""
        " ordo=\"dies\" pagina=\"5\"/>", c->piscina, &vitium));
    /* ligatio eadem: pagina servatur */
    CREDO_VERUM(applicare(w, "<proba visus=\"res\" res=\"alpha\"/>",
        c->piscina, &vitium));
    CREDO_VERUM(campus_est(&w->ramus, "pagina", "5"));
    /* ligatio mutata: pagina ad absens, ordo servatur */
    CREDO_VERUM(applicare(w, "<proba visus=\"res\" res=\"beta\"/>",
        c->piscina, &vitium));
    CREDO_VERUM(campus_est(&w->ramus, "pagina", NIHIL));
    CREDO_VERUM(campus_est(&w->ramus, "ordo", "dies"));
    /* ligatio mutata sed pagina data: data vincit */
    CREDO_VERUM(applicare(w, "<proba visus=\"res\" res=\"gamma\""
        " pagina=\"7\"/>", c->piscina, &vitium));
    CREDO_VERUM(campus_est(&w->ramus, "pagina", "7"));
    /* valor vacuus = ad absens (clarus) */
    CREDO_VERUM(applicare(w, "<proba ordo=\"\"/>", c->piscina,
        &vitium));
    CREDO_VERUM(campus_est(&w->ramus, "ordo", NIHIL));
}

/* recusationes: vitium nominat; NIHIL scriptum (attributum bonum ante
 * malum non ponitur) */
interior vacuum
_sectio_recusationes (
    CredoContextus* c)
{
     Mundus* w;
     Mundus* sine;
     chorda  vitium;

    w = mundus(c, VERUM);
    CREDO_NECESSE_NON_NIHIL(w);
    CREDO_VERUM(applicare(w, "<proba ordo=\"dies\"/>", c->piscina,
        &vitium));
    CREDO_FALSUM(applicare(w, "<nulla/>", c->piscina, &vitium));
    CREDO_CHORDA_CONTINET(vitium, chorda_ex_literis("nulla",
        c->piscina));
    CREDO_FALSUM(applicare(w, "<proba ordo=\"nomen\" nemo=\"x\"/>",
        c->piscina, &vitium));
    CREDO_CHORDA_CONTINET(vitium, chorda_ex_literis("nemo",
        c->piscina));
    CREDO_VERUM(campus_est(&w->ramus, "ordo", "dies"));
    CREDO_FALSUM(applicare(w, "<proba cursor=\"3\"/>", c->piscina,
        &vitium));
    CREDO_CHORDA_CONTINET(vitium, chorda_ex_literis("cursor",
        c->piscina));
    CREDO_FALSUM(applicare(w, "<proba visus=\"tabula\"/>", c->piscina,
        &vitium));
    CREDO_CHORDA_CONTINET(vitium, chorda_ex_literis("visus",
        c->piscina));
    CREDO_FALSUM(applicare(w, "<proba pagina=\"x\"/>", c->piscina,
        &vitium));
    CREDO_CHORDA_CONTINET(vitium, chorda_ex_literis("pagina",
        c->piscina));
    CREDO_FALSUM(applicare(w, "<proba><nemo/></proba>", c->piscina,
        &vitium));
    CREDO_CHORDA_CONTINET(vitium, chorda_ex_literis("nemo",
        c->piscina));
    /* montatio bis in semita */
    CREDO_FALSUM(applicare(w, "<proba><notae pagina=\"1\"/><notae"
        " pagina=\"2\"/></proba>", c->piscina, &vitium));
    CREDO_CHORDA_CONTINET(vitium, chorda_ex_literis("bis", c->piscina));
    /* ligatio hereditaria (folium: ligamen a parente) */
    CREDO_FALSUM(applicare(w, "<proba><notae liber=\"x\"/></proba>",
        c->piscina, &vitium));
    CREDO_CHORDA_CONTINET(vitium, chorda_ex_literis("liber",
        c->piscina));
    /* liber non montatus */
    sine = mundus(c, FALSUM);
    CREDO_NECESSE_NON_NIHIL(sine);
    CREDO_FALSUM(applicare(sine, "<proba ordo=\"dies\"><notae"
        " pagina=\"2\"/></proba>", c->piscina, &vitium));
    CREDO_CHORDA_CONTINET(vitium, chorda_ex_literis("non montata",
        c->piscina));
    /* nihil scriptum: radix quoque intacta (initium = absens) */
    CREDO_VERUM(campus_est(&sine->ramus, "ordo", "nomen"));
    /* liber malus post radicem bonam: radix non scripta */
    CREDO_FALSUM(applicare(w,
        "<proba ordo=\"nomen\"><alter dies=\"x\"/>"
        "</proba>", c->piscina, &vitium));
    CREDO_VERUM(campus_est(&w->ramus, "ordo", "dies"));
}

/* URI: via (ligatio ordine manifesti; media absens = absens; ultimae
 * vacuae omissae), quaestio punctata ordine arboris, effugium */
interior vacuum
_sectio_uri (
    CredoContextus* c)
{
      Mundus* w;
      chorda  vitium;
      chorda  u;
   StmlNodus* s;

    w = mundus(c, VERUM);
    CREDO_NECESSE_NON_NIHIL(w);
    CREDO_CHORDA_AEQUALIS_LITERIS(uri(w, "<proba/>", c->piscina),
        "proba");
    CREDO_CHORDA_AEQUALIS_LITERIS(uri(w, "<proba visus=\"res\"/>",
        c->piscina), "proba/res");
    CREDO_CHORDA_AEQUALIS_LITERIS(uri(w, "<proba res=\"alpha\"/>",
        c->piscina), "proba/index/alpha");
    /* ordo arboris, non ordo attributorum datorum */
    CREDO_CHORDA_AEQUALIS_LITERIS(uri(w, "<proba pagina=\"2\""
        " ordo=\"dies\" res=\"a\" visus=\"res\"><alter pagina=\"1\""
        " dies=\"2026-10-19\"/><notae pagina=\"4\"/></proba>",
        c->piscina), "proba/res/a?ordo=dies&pagina=2&notae.pagina=4"
        "&alter.dies=2026-10-19&alter.pagina=1");
    /* effugium: extra A-Z a-z 0-9 - _ . ~ omnia %XX */
    CREDO_CHORDA_AEQUALIS_LITERIS(uri(w, "<proba res=\"a b/c&d?\303"
        "\251\"/>", c->piscina), "proba/index/a%20b%2Fc%26d%3F%C3%A9");
    /* ex URI et retro: eadem */
    u = chorda_ex_literis("proba/res/a%20b%2Fc?ordo=dies&notae.pagina=4"
        "&alter.dies=2026-10-19", c->piscina);
    s = semita_ex_uri(organa, w, u, c->piscina, w->intern, &vitium);
    CREDO_NECESSE_NON_NIHIL(s);
    CREDO_CHORDA_AEQUALIS_LITERIS(semita_ad_uri(organa, w, s,
        c->piscina), "proba/res/a%20b%2Fc?ordo=dies&notae.pagina=4"
        "&alter.dies=2026-10-19");
    CREDO_VERUM(semita_applicare(organa, w, &w->ramus, s, c->piscina,
        &vitium));
    CREDO_VERUM(campus_est(&w->ramus, "res", "a b/c"));
    CREDO_VERUM(campus_est(&w->notae, "pagina", "4"));
    /* ex URI: vitia nominata */
    CREDO_NIHIL(semita_ex_uri(organa, w, chorda_ex_literis("nulla/x",
        c->piscina), c->piscina, w->intern, &vitium));
    CREDO_CHORDA_CONTINET(vitium, chorda_ex_literis("nulla",
        c->piscina));
    CREDO_NIHIL(semita_ex_uri(organa, w, chorda_ex_literis(
        "proba/res/a/plus", c->piscina), c->piscina, w->intern,
        &vitium));
    CREDO_CHORDA_CONTINET(vitium, chorda_ex_literis("plus",
        c->piscina));
    CREDO_NIHIL(semita_ex_uri(organa, w, chorda_ex_literis(
        "proba?notae.nemo=1", c->piscina), c->piscina, w->intern,
        &vitium));
    CREDO_CHORDA_CONTINET(vitium, chorda_ex_literis("nemo",
        c->piscina));
    CREDO_NIHIL(semita_ex_uri(organa, w, chorda_ex_literis(
        "proba?ordo=dies&ordo=nomen", c->piscina), c->piscina,
        w->intern,
        &vitium));
    CREDO_CHORDA_CONTINET(vitium, chorda_ex_literis("ordo",
        c->piscina));
    CREDO_NIHIL(semita_ex_uri(organa, w, chorda_ex_literis(
        "proba/index/a%G1", c->piscina), c->piscina, w->intern,
        &vitium));
    CREDO_NIHIL(semita_ex_uri(organa, w, chorda_ex_literis(
        "proba?ordo=tabula", c->piscina), c->piscina, w->intern,
        &vitium));
    CREDO_CHORDA_CONTINET(vitium, chorda_ex_literis("ordo",
        c->piscina));
}

/* lista: URI per nodum ligationis propriae; hereditarii in URI
 * parentis */
interior vacuum
_sectio_lista (
    CredoContextus* c)
{
    Mundus* w;
       Xar* l;

    w = mundus(c, VERUM);
    CREDO_NECESSE_NON_NIHIL(w);
    l = semita_ad_uri_lista(organa, w, arbor("<proba visus=\"res\""
        " res=\"a\" ordo=\"dies\"><notae pagina=\"2\"/><alter"
        " dies=\"2026-10-19\" pagina=\"3\"/></proba>", w, c->piscina),
        c->piscina);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_AEQUALIS_I32(xar_numerus(l), II);
    CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(l, ZEPHYRUM),
        "proba/res/a?ordo=dies&notae.pagina=2");
    CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(l, I),
        "folium_liber/2026-10-19?pagina=3");
    l = semita_ad_uri_lista(organa, w, arbor("<proba res=\"a\"><notae"
        " pagina=\"2\"/></proba>", w, c->piscina), c->piscina);
    CREDO_AEQUALIS_I32(xar_numerus(l), I);
}

/* plena (R3, historia): campi semitae OMNES, absentes expliciti;
 * restitutio exacta etiam ubi canonica "serva" significaret */
interior vacuum
_sectio_plena (
    CredoContextus* c)
{
      Mundus* w;
      chorda  vitium;
   StmlNodus* plena;

    w = mundus(c, VERUM);
    CREDO_NECESSE_NON_NIHIL(w);
    /* status ordinarius: plena omnia nominat, canonica nihil */
    plena = semita_legere_plenam(organa, w, &w->ramus, "proba",
        c->piscina, w->intern);
    CREDO_NECESSE_NON_NIHIL(plena);
    CREDO_CHORDA_AEQUALIS_LITERIS(semita_ad_uri(organa, w, plena,
        c->piscina), "proba/index?ordo=nomen&pagina=0&notae.pagina=0"
        "&alter.pagina=0");
    /* restitutio exacta: plena status X super statum Y = X */
    CREDO_VERUM(applicare(w, "<proba visus=\"res\" res=\"alpha\""
        " ordo=\"dies\" pagina=\"3\"><notae pagina=\"4\"/></proba>",
        c->piscina, &vitium));
    plena = semita_legere_plenam(organa, w, &w->ramus, "proba",
        c->piscina, w->intern);
    CREDO_VERUM(applicare(w, "<proba visus=\"index\" res=\"beta\""
        " ordo=\"nomen\"><notae pagina=\"9\"/></proba>", c->piscina,
        &vitium));
    CREDO_VERUM(semita_applicare(organa, w, &w->ramus, plena,
        c->piscina,
        &vitium));
    CREDO_VERUM(campus_est(&w->ramus, "visus", "res"));
    CREDO_VERUM(campus_est(&w->ramus, "res", "alpha"));
    CREDO_VERUM(campus_est(&w->ramus, "ordo", "dies"));
    CREDO_VERUM(campus_est(&w->ramus, "pagina", "3"));
    CREDO_VERUM(campus_est(&w->notae, "pagina", "4"));
    /* et retro ad ordinarium: plena mundi novi (omnia absentia,
     * scripta) super statum mutatum */
    {
        Mundus* novus;

        novus = mundus(c, VERUM);
        CREDO_NECESSE_NON_NIHIL(novus);
        plena = semita_legere_plenam(organa, novus, &novus->ramus,
            "proba", c->piscina, novus->intern);
        CREDO_VERUM(semita_applicare(organa, w, &w->ramus, plena,
            c->piscina, &vitium));
        CREDO_VERUM(campus_est(&w->ramus, "visus", "index"));
        CREDO_VERUM(campus_est(&w->ramus, "res", NIHIL));
        CREDO_VERUM(campus_est(&w->ramus, "ordo", "nomen"));
        CREDO_VERUM(campus_est(&w->notae, "pagina", "0"));
        CREDO_CHORDA_AEQUALIS_LITERIS(semita_ad_uri(organa, w,
            semita_legere(organa, w, &w->ramus, "proba", c->piscina,
            w->intern), c->piscina), "proba/index");
    }
}

/* R4d (V-a): ligatio TOTA nominatur - campus ligationis ullus (etiam
 * vacuus) ceteros non nominatos ad absens mittit; nullus: ligatio
 * servatur. URI: "genus/" nominat, "genus" et "genus?..." non */
interior vacuum
_sectio_ligatio_tota (
    CredoContextus* c)
{
      Mundus* w;
      chorda  vitium;
   StmlNodus* s;

    w = mundus(c, VERUM);
    CREDO_NECESSE_NON_NIHIL(w);
    CREDO_VERUM(applicare(w, "<proba visus=\"res\" res=\"alpha\""
        " ordo=\"dies\" pagina=\"5\"/>", c->piscina, &vitium));
    /* visus solum nominatus: res ad absens (ligatio mutata: pagina
     * quoque), ordo (status) servatur */
    CREDO_VERUM(applicare(w, "<proba visus=\"res\"/>", c->piscina,
        &vitium));
    CREDO_VERUM(campus_est(&w->ramus, "visus", "res"));
    CREDO_VERUM(campus_est(&w->ramus, "res", NIHIL));
    CREDO_VERUM(campus_est(&w->ramus, "pagina", NIHIL));
    CREDO_VERUM(campus_est(&w->ramus, "ordo", "dies"));
    /* res solum nominatus: visus ad absens (index) */
    CREDO_VERUM(applicare(w, "<proba res=\"beta\" pagina=\"2\"/>",
        c->piscina, &vitium));
    CREDO_VERUM(campus_est(&w->ramus, "visus", NIHIL));
    CREDO_VERUM(campus_est(&w->ramus, "res", "beta"));
    /* ligatio non nominata: servatur, status ponitur */
    CREDO_VERUM(applicare(w, "<proba ordo=\"nomen\"/>", c->piscina,
        &vitium));
    CREDO_VERUM(campus_est(&w->ramus, "res", "beta"));
    CREDO_VERUM(campus_est(&w->ramus, "pagina", "2"));
    CREDO_VERUM(campus_est(&w->ramus, "ordo", "nomen"));
    /* nominata vacua: omnia ad absens */
    CREDO_VERUM(applicare(w, "<proba visus=\"\"/>", c->piscina,
        &vitium));
    CREDO_VERUM(campus_est(&w->ramus, "visus", NIHIL));
    CREDO_VERUM(campus_est(&w->ramus, "res", NIHIL));
    /* URI: "proba/" nominat, "proba?..." servat */
    CREDO_CHORDA_AEQUALIS_LITERIS(uri(w, "<proba visus=\"\"/>",
        c->piscina), "proba/");
    CREDO_CHORDA_AEQUALIS_LITERIS(uri(w, "<proba/>", c->piscina),
        "proba");
    CREDO_VERUM(applicare(w, "<proba visus=\"res\" res=\"gamma\"/>",
        c->piscina, &vitium));
    s = semita_ex_uri(organa, w, chorda_ex_literis("proba?ordo=dies",
        c->piscina), c->piscina, w->intern, &vitium);
    CREDO_NECESSE(s && semita_applicare(organa, w, &w->ramus, s,
        c->piscina, &vitium));
    CREDO_VERUM(campus_est(&w->ramus, "res", "gamma"));
    s = semita_ex_uri(organa, w, chorda_ex_literis("proba/",
        c->piscina),
        c->piscina, w->intern, &vitium);
    CREDO_NECESSE(s && semita_applicare(organa, w, &w->ramus, s,
        c->piscina, &vitium));
    CREDO_VERUM(campus_est(&w->ramus, "visus", NIHIL));
    CREDO_VERUM(campus_est(&w->ramus, "res", NIHIL));
    CREDO_VERUM(campus_est(&w->ramus, "ordo", "dies"));
    /* canonica a statu X super statum Y: ligatio X reddita */
    CREDO_VERUM(applicare(w, "<proba visus=\"res\" res=\"delta\"/>",
        c->piscina, &vitium));
    {
        Mundus* novus;

        novus = mundus(c, VERUM);
        CREDO_NECESSE_NON_NIHIL(novus);
        s = semita_legere(organa, novus, &novus->ramus, "proba",
            c->piscina, novus->intern);
        CREDO_VERUM(semita_applicare(organa, w, &w->ramus, s,
            c->piscina,
            &vitium));
        CREDO_VERUM(campus_est(&w->ramus, "visus", "index"));
        CREDO_VERUM(campus_est(&w->ramus, "res", NIHIL));
    }
}

/* ramus generis fixturae paratus */
interior b32
ramum_parare (
                  Mundus* w,
             InsulaRamus* r,
      constans character* genus,
                 Piscina* p)
{
    *r = insula_ramus(w->repo, genus, "x1");
    redde organum_ramum_parare(organa(w, chorda_ex_literis(genus, p)),
        r,
        NIHIL, NIHIL, NIHIL, p, w->intern);
}

/* URI semitae persistentis rami */
interior chorda
uri_persistens (
                     Mundus* w,
       constans InsulaRamus* r,
         constans character* genus,
                    Piscina* p)
{
    redde semita_ad_uri(organa, w, semita_legere_persistentem(organa, w,
        r, genus, p, w->intern), p);
}

/* R4d (VI-e): semita persistens - campi transientes omissi; ligatio
 * nominata si campus ligationis persistens ullus (applicata transientes
 * ad absens mittit), aliter non nominata */
interior vacuum
_sectio_persistens (
    CredoContextus* c)
{
         Mundus* w;
    InsulaRamus  t;
    InsulaRamus  k;
         chorda  vitium;
      StmlNodus* s;

    w = mundus(c, FALSUM);
    CREDO_NECESSE_NON_NIHIL(w);
    CREDO_NECESSE(ramum_parare(w, &t, "tabella", c->piscina));
    CREDO_NECESSE(ramum_parare(w, &k, "catalogus", c->piscina));
    CREDO_CHORDA_AEQUALIS_LITERIS(uri_persistens(w, &t, "tabella",
        c->piscina), "tabella/mensis");
    CREDO_VERUM(semita_applicare(organa, w, &t, arbor("<tabella"
        " visus=\"dies\" dies=\"2026-10-19\" filtrum=\"domus\""
        " pagina=\"3\"/>", w, c->piscina), c->piscina, &vitium));
    CREDO_CHORDA_AEQUALIS_LITERIS(uri_persistens(w, &t, "tabella",
        c->piscina), "tabella/dies?filtrum=domus");
    /* applicata: dies (transiens) ad absens - "hodie" */
    s = semita_legere_persistentem(organa, w, &t, "tabella", c->piscina,
        w->intern);
    CREDO_VERUM(semita_applicare(organa, w, &t, s, c->piscina,
        &vitium));
    CREDO_VERUM(campus_est(&t, "visus", "dies"));
    CREDO_VERUM(campus_est(&t, "dies", NIHIL));
    CREDO_VERUM(campus_est(&t, "filtrum", "domus"));
    /* ligatio tota transiens: non nominata */
    CREDO_VERUM(semita_applicare(organa, w, &k, arbor("<catalogus"
        " persona=\"7\" modus=\"rubricae\"/>", w, c->piscina),
        c->piscina, &vitium));
    CREDO_CHORDA_AEQUALIS_LITERIS(uri_persistens(w, &k, "catalogus",
        c->piscina), "catalogus?modus=rubricae");
    /* canonica (non persistens) ligationem transientem nominat */
    CREDO_CHORDA_AEQUALIS_LITERIS(semita_ad_uri(organa, w,
        semita_legere(
        organa, w, &k, "catalogus", c->piscina, w->intern), c->piscina),
        "catalogus/7?modus=rubricae");
}

/* URI argumenti (vacua si NIHIL) */
interior chorda
uri_argumenti (
                 Mundus* w,
     constans character* genus,
     constans character* argumentum,
                Piscina* p)
{
     StmlNodus* s;
        chorda  vitium;

    s = semita_ex_argumento(organa, w, genus, argumentum, p, w->intern,
        &vitium);
    redde s ? semita_ad_uri(organa, w, s, p) : chorda_ex_literis("", p);
}

/* resolutor probationis: hodie 2026-10-20, heri 2026-10-19; cras
 * recusatur; vocationes numerantur (ctx = i32*) */
interior b32
resolutor_probationis (
                    vacuum* ctx,
    constans OrganumCampus* campus,
                    chorda  valor,
                   Piscina* piscina,
                    chorda* concretus)
{
    (vacuum)campus;
    (*(i32*)ctx)++;
    si (chorda_aequalis_literis(valor, "hodie"))
    {
        *concretus = chorda_ex_literis("2026-10-20", piscina);
        redde VERUM;
    }
    si (chorda_aequalis_literis(valor, "heri"))
    {
        *concretus = chorda_ex_literis("2026-10-19", piscina);
        redde VERUM;
    }
    redde FALSUM;
}

/* R4e (V-b): argumentum -> semita per cognomina manifesti (ordine:
 * fixum, neutrum, typata), URI generis directe; valores symbolici ad
 * limen resolvuntur, non resoluti recusantur */
interior vacuum
_sectio_cognomina (
    CredoContextus* c)
{
         Mundus* w;
    InsulaRamus  t;
         chorda  vitium;
      StmlNodus* s;
            i32  vocationes;

    w = mundus(c, FALSUM);
    CREDO_NECESSE_NON_NIHIL(w);
    /* fixum ante typatum */
    CREDO_CHORDA_AEQUALIS_LITERIS(uri_argumenti(w, "proba", "7",
        c->piscina), "proba/res/septem");
    CREDO_CHORDA_AEQUALIS_LITERIS(uri_argumenti(w, "proba", "8",
        c->piscina), "proba/res/8");
    /* neutrum = argumentum vacuum */
    CREDO_CHORDA_AEQUALIS_LITERIS(uri_argumenti(w, "proba", "",
        c->piscina), "proba/");
    /* typatum textus: effugium substituti */
    CREDO_CHORDA_AEQUALIS_LITERIS(uri_argumenti(w, "proba", "a b/c",
        c->piscina), "proba/index/a%20b%2Fc");
    /* URI generis directe (non cognomen) */
    CREDO_CHORDA_AEQUALIS_LITERIS(uri_argumenti(w, "proba",
        "proba?ordo=dies", c->piscina), "proba?ordo=dies");
    CREDO_NIHIL(semita_ex_argumento(organa, w, "proba", "proba/res/a/b",
        c->piscina, w->intern, &vitium));
    CREDO_CHORDA_CONTINET(vitium, chorda_ex_literis("superfluum",
        c->piscina));
    /* nullum congruit: NIHIL (vocans computata sua tentat) */
    CREDO_NIHIL(semita_ex_argumento(organa, w, "tabella", "x",
        c->piscina, w->intern, &vitium));
    /* typatum argumentum vacuum numquam accipit (etiam textus) */
    CREDO_CHORDA_AEQUALIS_LITERIS(uri_argumenti(w, "catalogus", "x",
        c->piscina), "catalogus?modus=x");
    CREDO_NIHIL(semita_ex_argumento(organa, w, "catalogus", "",
        c->piscina, w->intern, &vitium));
    /* symbolicus: typatum dies accipit; applicare non resolutum
     * recusat */
    CREDO_NECESSE(ramum_parare(w, &t, "tabella", c->piscina));
    s = semita_ex_argumento(organa, w, "tabella", "hodie", c->piscina,
        w->intern, &vitium);
    CREDO_NECESSE_NON_NIHIL(s);
    CREDO_FALSUM(semita_applicare(organa, w, &t, s, c->piscina,
        &vitium));
    CREDO_CHORDA_CONTINET(vitium, chorda_ex_literis("symbolicus",
        c->piscina));
    CREDO_VERUM(campus_est(&t, "dies", NIHIL));
    /* resolutor deest: FALSUM */
    CREDO_FALSUM(semita_resolvere(organa, w, s, NIHIL, NIHIL,
        c->piscina,
        &vitium));
    vocationes = ZEPHYRUM;
    CREDO_VERUM(semita_resolvere(organa, w, s, resolutor_probationis,
        &vocationes, c->piscina, &vitium));
    CREDO_AEQUALIS_I32(vocationes, I);
    CREDO_VERUM(semita_applicare(organa, w, &t, s, c->piscina,
        &vitium));
    CREDO_VERUM(campus_est(&t, "dies", "2026-10-20"));
    /* resolutor recusat (cras): FALSUM, vitium nominat campum */
    s = semita_ex_argumento(organa, w, "tabella", "cras", c->piscina,
        w->intern, &vitium);
    CREDO_NECESSE_NON_NIHIL(s);
    CREDO_FALSUM(semita_resolvere(organa, w, s, resolutor_probationis,
        &vocationes, c->piscina, &vitium));
    CREDO_CHORDA_CONTINET(vitium, chorda_ex_literis("dies",
        c->piscina));
    /* concretus: resolutor non vocatur */
    vocationes = ZEPHYRUM;
    s = semita_ex_argumento(organa, w, "tabella", "2026-10-01",
        c->piscina, w->intern, &vitium);
    CREDO_NECESSE(s && semita_resolvere(organa, w, s,
        resolutor_probationis, &vocationes, c->piscina, &vitium));
    CREDO_AEQUALIS_I32(vocationes, ZEPHYRUM);
}

/* D5 S3c: liberi in ramis "<id parentis>_nomen" (vicus: calendarium
 * "<id>_notae", id unicum in volumine) - applicare et legere ut
 * conventio nuda; ramus "nomen" nudus non tangitur */
interior vacuum
_sectio_liberi_praefixi (
    CredoContextus* c)
{
         Mundus* w;
         chorda  vitium;
      StmlNodus* s;
    InsulaRamus  nudus;

    w = mundus(c, FALSUM);
    CREDO_NECESSE_NON_NIHIL(w);
    w->notae  = insula_ramus_liberi(&w->ramus, "folium", "p1_notae");
    w->alter  = insula_ramus_liberi(&w->ramus, "folium_liber",
        "p1_alter");
    CREDO_NECESSE(   organum_ramum_parare(w->folium, &w->notae, NIHIL,
                         NIHIL, NIHIL, c->piscina, w->intern)
                  && organum_ramum_parare(w->folium_liber, &w->alter,
                         NIHIL, NIHIL, NIHIL, c->piscina, w->intern));
    CREDO_VERUM(applicare(w, "<proba visus=\"res\" res=\"alpha\">"
        "<notae pagina=\"4\"/><alter dies=\"2026-10-19\""
        " pagina=\"3\"/></proba>", c->piscina, &vitium));
    CREDO_VERUM(campus_est(&w->notae, "pagina", "4"));
    CREDO_VERUM(campus_est(&w->alter, "pagina", "3"));
    /* ramus nudus "notae" non creatus */
    nudus = insula_ramus_liberi(&w->ramus, "folium", "notae");
    CREDO_VERUM(insula_ramus_nodus(&nudus) == NIHIL);
    s = semita_legere(organa, w, &w->ramus, "proba", c->piscina,
        w->intern);
    CREDO_NECESSE_NON_NIHIL(s);
    CREDO_CHORDA_AEQUALIS_LITERIS(semita_ad_uri(organa, w, s,
        c->piscina), "proba/res/alpha?notae.pagina=4"
        "&alter.dies=2026-10-19&alter.pagina=3");
}

/* comites P1: locus = ligatio radicis (absens nominatum), status et
 * liberi omissi; genus ignotum: vacua */
interior vacuum
_sectio_locus (
    CredoContextus* c)
{
    Mundus* w;
    chorda  vitium;

    w = mundus(c, VERUM);
    CREDO_NECESSE_NON_NIHIL(w);
    CREDO_CHORDA_AEQUALIS_LITERIS(semita_locus(organa, w, &w->ramus,
        "proba", c->piscina), "proba/index");
    CREDO_VERUM(applicare(w, "<proba visus=\"res\" res=\"alpha\""
        " ordo=\"dies\" pagina=\"2\"><notae pagina=\"4\"/>"
        "<alter dies=\"2026-10-19\" pagina=\"3\"/></proba>", c->piscina,
        &vitium));
    CREDO_CHORDA_AEQUALIS_LITERIS(semita_locus(organa, w, &w->ramus,
        "proba", c->piscina), "proba/res/alpha");
    /* valor effugitur ut in URI */
    CREDO_VERUM(applicare(w, "<proba visus=\"res\" res=\"a b/c\"/>",
        c->piscina, &vitium));
    CREDO_CHORDA_AEQUALIS_LITERIS(semita_locus(organa, w, &w->ramus,
        "proba", c->piscina), "proba/res/a%20b%2Fc");
    CREDO_AEQUALIS_I32(semita_locus(organa, w, &w->ramus, "ignotum",
        c->piscina).mensura, ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "applicare", _sectio_applicare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "locus", _sectio_locus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "liberi_praefixi", _sectio_liberi_praefixi, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "ambitus", _sectio_ambitus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "recusationes", _sectio_recusationes, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "uri", _sectio_uri, NIHIL, NIHIL, NIHIL, NIHIL },
    { "lista", _sectio_lista, NIHIL, NIHIL, NIHIL, NIHIL },
    { "plena", _sectio_plena, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ligatio_tota", _sectio_ligatio_tota, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "persistens", _sectio_persistens, NIHIL, NIHIL, NIHIL, NIHIL },
    { "cognomina", _sectio_cognomina, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("semita", SECTIONES);
}

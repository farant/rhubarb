/* probatio_semita.c - semitae R1 (project-specs/semitae-spec.md):
 * applicare (ligatio, resetum ambitus ligationis, status, liberi,
 * scriptores plures), legere (forma canonica), recusationes nominatae
 * (nihil scriptum), URI utrimque (via, quaestio punctata, effugium),
 * lista URI (ligatio propria). Fixtura: organa proba (radix),
 * folium (liber, ligatio hereditaria), folium_liber (ligatio
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

nomen structura {
     InternamentumChorda* intern;
      InsulaRepositorium* repo;
                 Organum* proba;
                 Organum* folium;
                 Organum* folium_liber;
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
    si (!w->repo || !w->proba || !w->folium || !w->folium_liber)
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
    /* status ordinarius: semita vacua (nec attributa nec liberi) */
    s = semita_legere(organa, w, &w->ramus, "proba", c->piscina,
        w->intern);
    CREDO_NECESSE_NON_NIHIL(s);
    CREDO_VERUM(!s->attributa || xar_numerus(s->attributa) == ZEPHYRUM);
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
    CREDO_VERUM(applicare(w, "<proba res=\"alpha\"/>", c->piscina,
        &vitium));
    CREDO_VERUM(campus_est(&w->ramus, "pagina", "5"));
    /* ligatio mutata: pagina ad absens, ordo servatur */
    CREDO_VERUM(applicare(w, "<proba res=\"beta\"/>", c->piscina,
        &vitium));
    CREDO_VERUM(campus_est(&w->ramus, "pagina", NIHIL));
    CREDO_VERUM(campus_est(&w->ramus, "ordo", "dies"));
    /* ligatio mutata sed pagina data: data vincit */
    CREDO_VERUM(applicare(w, "<proba res=\"gamma\" pagina=\"7\"/>",
        c->piscina, &vitium));
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
            w->intern), c->piscina), "proba");
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "applicare", _sectio_applicare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ambitus", _sectio_ambitus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "recusationes", _sectio_recusationes, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "uri", _sectio_uri, NIHIL, NIHIL, NIHIL, NIHIL },
    { "lista", _sectio_lista, NIHIL, NIHIL, NIHIL, NIHIL },
    { "plena", _sectio_plena, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("semita", SECTIONES);
}

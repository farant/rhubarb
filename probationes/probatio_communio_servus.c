/* probatio_communio_servus.c - communio_servus (communio-plan-2):
 * T3 registrum apparatuum - membra spatii, apparatus per digestum
 * tesserae (tessera ipsa numquam servatur), verificatio, revocatio,
 * replica ligata. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "volumen.h"
#include "cives.h"
#include "moneta.h"
#include "scrinium.h"
#include "communio_servus.h"
#include "communio_forma.h"
#include "json.h"
#include "reactor.h"
#include "hospitium.h"
#include "http.h"
#include "http_vocatio.h"
#include "credo.h"
#include <string.h>
#include <stdio.h>

nomen structura {
           Volumen* volumen;
             Cives* cives;
    CommunioServus* servus;
            chorda  franciscus;
            chorda  amanda;
} Mundus;

hic_manens Piscina* piscina;

interior chorda
lit (
    constans character* s)
{
    redde chorda_ex_literis(s, piscina);
}

interior vacuum
_parare (
    CredoContextus* c)
{
    Mundus* m;

    piscina  = c->piscina;
    m        = (Mundus*)piscina_allocare(piscina, magnitudo(Mundus));
    memset(m, ZEPHYRUM, magnitudo(Mundus));
    c->datum = m;
    m->volumen = volumen_temporarium(piscina,
        "probatio_communio_servus");
    si (!m->volumen)
    {
        redde;
    }
    m->cives = cives_aperire(m->volumen, NIHIL, piscina);
    si (!m->cives || !volumen_transactionem_incipere(m->volumen))
    {
        redde;
    }
    (vacuum)cives_creare(m->cives, lit("franciscus"), lit("Franciscus"),
        &m->franciscus, piscina);
    (vacuum)cives_creare(m->cives, lit("amanda"), lit("Amanda"),
        &m->amanda, piscina);
    (vacuum)volumen_transactionem_committere(m->volumen);
    m->servus = communio_servum_creare(piscina, m->volumen, m->cives,
        NIHIL);
}

interior vacuum
_purgare (
    CredoContextus* c)
{
    Mundus* m;

    m = (Mundus*)c->datum;
    si (m && m->servus)
    {
        communio_servum_destruere(m->servus);
    }
    si (m && m->volumen)
    {
        volumen_claudere(m->volumen);
    }
    piscina = NIHIL;
}

/* apparatum Francisci in familia: tessera et id in exitus */
interior b32
apparatum_primum (
       Mundus* m,
    character* tessera,
    character* id)
{
    character digestum[LXV];

    redde communio_membrum_addere(m->servus, lit("familia"),
               m->franciscus)
        && communio_tesseram_facere(tessera, digestum)
        && communio_apparatum_addere(m->servus, m->franciscus,
               lit("familia"), lit("Macintosh"), lit(digestum), id);
}

interior vacuum
_sectio_apparatus (
    CredoContextus* c)
{
               Mundus* m;
            character  tessera[XLIV];
            character  alia[XLIV];
            character  digestum[LXV];
            character  id[MONETA_ULID_MENSURA];
    CommunioApparatus  a;
               chorda  t;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->servus);
    CREDO_NECESSE(apparatum_primum(m, tessera, id));
    CREDO_AEQUALIS_I32((i32)strlen(tessera), XLIII);
    CREDO_AEQUALIS_I32((i32)strlen(id), XXVI);

    CREDO_GRADUS("tessera vera");
    CREDO_VERUM(communio_apparatum_verificare(m->servus, lit(tessera),
        &a, piscina));
    CREDO_CHORDA_AEQUALIS_LITERIS(a.id, id);
    CREDO_CHORDA_AEQUALIS(a.civis, m->franciscus);
    CREDO_CHORDA_AEQUALIS_LITERIS(a.spatium, "familia");
    CREDO_CHORDA_AEQUALIS_LITERIS(a.titulus, "Macintosh");
    CREDO_AEQUALIS_I32(a.replica.mensura, ZEPHYRUM);
    CREDO_FALSUM(a.revocatus);

    CREDO_GRADUS("tesserae falsae");
    CREDO_NECESSE(communio_tesseram_facere(alia, digestum));
    CREDO_FALSUM(communio_apparatum_verificare(m->servus, lit(alia), &a,
        piscina));
    memcpy(alia, tessera, (size_t)XLIV);
    alia[XLII] = alia[XLII] == 'A' ? 'B' : 'A';
    CREDO_FALSUM(communio_apparatum_verificare(m->servus, lit(alia), &a,
        piscina));
    t          = lit(tessera);
    t.mensura  = XLII;
    CREDO_FALSUM(communio_apparatum_verificare(m->servus, t, &a,
        piscina));
    CREDO_FALSUM(communio_apparatum_verificare(m->servus, lit(""), &a,
        piscina));
}

interior vacuum
_sectio_revocatus (
    CredoContextus* c)
{
               Mundus* m;
            character  tessera[XLIV];
            character  id[MONETA_ULID_MENSURA];
    CommunioApparatus  a;
                  Xar* l;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->servus);
    CREDO_NECESSE(apparatum_primum(m, tessera, id));
    CREDO_VERUM(communio_apparatum_revocare(m->servus, lit(id)));
    CREDO_FALSUM(communio_apparatum_verificare(m->servus, lit(tessera),
        &a, piscina));
    l = communio_apparatus_enumerare(m->servus, piscina);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_AEQUALIS_I32(xar_numerus(l), I);
    CREDO_VERUM(((CommunioApparatus*)xar_obtinere(l, ZEPHYRUM))
        ->revocatus);
    CREDO_FALSUM(communio_apparatum_revocare(m->servus,
        lit("01ZZZZZZZZZZZZZZZZZZZZZZZZ")));
}

interior vacuum
_sectio_suspensa (
    CredoContextus* c)
{
               Mundus* m;
            character  tessera[XLIV];
            character  id[MONETA_ULID_MENSURA];
    CommunioApparatus  a;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->servus);
    CREDO_NECESSE(apparatum_primum(m, tessera, id));
    CREDO_NECESSE(volumen_transactionem_incipere(m->volumen));
    CREDO_NECESSE(cives_statum_ponere(m->cives, m->franciscus,
        CIVIS_SUSPENSUS));
    CREDO_NECESSE(volumen_transactionem_committere(m->volumen));
    CREDO_FALSUM(communio_apparatum_verificare(m->servus, lit(tessera),
        &a, piscina));
}

interior vacuum
_sectio_non_membrum (
    CredoContextus* c)
{
       Mundus* m;
    character  tessera[XLIV];
    character  digestum[LXV];
    character  id[MONETA_ULID_MENSURA];

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->servus);
    CREDO_NECESSE(communio_membrum_addere(m->servus, lit("familia"),
        m->franciscus));
    CREDO_NECESSE(communio_tesseram_facere(tessera, digestum));
    CREDO_FALSUM(communio_apparatum_addere(m->servus, m->amanda,
        lit("familia"), lit("iPhone"), lit(digestum), id));
    CREDO_FALSUM(communio_apparatum_addere(m->servus, m->franciscus,
        lit("alia"), lit("iPhone"), lit(digestum), id));
    CREDO_FALSUM(communio_apparatum_addere(m->servus,
        lit("01ZZZZZZZZZZZZZZZZZZZZZZZZ"), lit("familia"),
        lit("iPhone"),
        lit(digestum), id));
    CREDO_FALSUM(communio_membrum_addere(m->servus, lit("familia"),
        lit("01ZZZZZZZZZZZZZZZZZZZZZZZZ")));

    CREDO_GRADUS("membrum iteratum: idem");
    CREDO_VERUM(communio_membrum_addere(m->servus, lit("familia"),
        m->franciscus));
    CREDO_VERUM(communio_membrum_addere(m->servus, lit("familia"),
        m->amanda));
    CREDO_VERUM(communio_apparatum_addere(m->servus, m->amanda,
        lit("familia"), lit("iPhone"), lit(digestum), id));
}

interior vacuum
_sectio_digestum (
    CredoContextus* c)
{
       Mundus* m;
    character  tessera[XLIV];
    character  digestum[LXV];
    character  id[MONETA_ULID_MENSURA];
    character  maiusculum[LXV];
          i32  i;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->servus);
    CREDO_NECESSE(communio_membrum_addere(m->servus, lit("familia"),
        m->franciscus));
    CREDO_NECESSE(communio_tesseram_facere(tessera, digestum));
    CREDO_NECESSE(communio_apparatum_addere(m->servus, m->franciscus,
        lit("familia"), lit("Macintosh"), lit(digestum), id));
    CREDO_FALSUM(communio_apparatum_addere(m->servus, m->franciscus,
        lit("familia"), lit("Alius"), lit(digestum), id));

    CREDO_GRADUS("digestum malum");
    CREDO_NECESSE(communio_tesseram_facere(tessera, digestum));
    memcpy(maiusculum, digestum, (size_t)LXV);
    per (i = ZEPHYRUM; i < LXIV; i++)
    {
        si (maiusculum[i] >= 'a' && maiusculum[i] <= 'f')
        {
            maiusculum[i] = (character)(maiusculum[i] - 'a' + 'A');
        }
    }
    CREDO_FALSUM(communio_apparatum_addere(m->servus, m->franciscus,
        lit("familia"), lit("Alius"), lit(maiusculum), id));
    digestum[LXIII] = '\0';
    CREDO_FALSUM(communio_apparatum_addere(m->servus, m->franciscus,
        lit("familia"), lit("Alius"), lit(digestum), id));
}

interior vacuum
_sectio_ligare (
    CredoContextus* c)
{
       Mundus* m;
    character  tessera[XLIV];
    character  digestum[LXV];
    character  primum[MONETA_ULID_MENSURA];
    character  secundum[MONETA_ULID_MENSURA];
    character  r1[MONETA_ULID_MENSURA];
    character  r2[MONETA_ULID_MENSURA];

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->servus);
    CREDO_NECESSE(apparatum_primum(m, tessera, primum));
    CREDO_NECESSE(communio_tesseram_facere(tessera, digestum));
    CREDO_NECESSE(communio_apparatum_addere(m->servus, m->franciscus,
        lit("familia"), lit("iPhone"), lit(digestum), secundum));
    moneta_ulid(r1);
    moneta_ulid(r2);
    CREDO_AEQUALIS_I32((i32)communio_replicam_ligare(m->servus,
        lit(primum), lit(r1)), ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)communio_replicam_ligare(m->servus,
        lit(primum), lit(r1)), ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)communio_replicam_ligare(m->servus,
        lit(primum), lit(r2)), I);
    CREDO_AEQUALIS_I32((i32)communio_replicam_ligare(m->servus,
        lit(secundum), lit(r1)), II);
    CREDO_VERUM(communio_replicam_ligare(m->servus, lit(secundum),
        lit("minuscula")) < ZEPHYRUM);
    CREDO_VERUM(communio_replicam_ligare(m->servus,
        lit("01ZZZZZZZZZZZZZZZZZZZZZZZZ"), lit(r2)) < ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)communio_replicam_ligare(m->servus,
        lit(secundum), lit(r2)), ZEPHYRUM);
}

/* chorda continet subchordam? */
interior b32
continet (
     chorda c,
     chorda sub)
{
    i32 i;

    per (i = ZEPHYRUM; sub.mensura > ZEPHYRUM
         && i + sub.mensura <= c.mensura; i++)
    {
        si (memcmp(c.datum + i, sub.datum, (size_t)sub.mensura)
            == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
_sectio_secretum (
    CredoContextus* c)
{
                Mundus* m;
             character  tessera[XLIV];
             character  id[MONETA_ULID_MENSURA];
             character  r1[MONETA_ULID_MENSURA];
     CommunioApparatus  a;
                   Xar* acta;
    ScriniumEnuntiatum* e;
                   i32  i;
                   i32  k;
                   i32  additi;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->servus);
    CREDO_NECESSE(apparatum_primum(m, tessera, id));
    moneta_ulid(r1);
    (vacuum)communio_replicam_ligare(m->servus, lit(id), lit(r1));
    (vacuum)communio_apparatum_verificare(m->servus, lit(tessera), &a,
        piscina);
    (vacuum)communio_apparatum_revocare(m->servus, lit(id));

    CREDO_GRADUS("acta: nec tessera nec digestum");
    acta = volumen_acta_legere(m->volumen, ZEPHYRUM, piscina);
    CREDO_NECESSE_NON_NIHIL(acta);
    additi = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(acta); i++)
    {
        VolumenActum* x = (VolumenActum*)xar_obtinere(acta, i);

        CREDO_FALSUM(continet(x->datum, lit(tessera)));
        si (chorda_aequalis_literis(x->genus,
                "communio.apparatus-additus"))
        {
            additi++;
            CREDO_FALSUM(continet(x->datum, lit("digestum")));
        }
    }
    CREDO_AEQUALIS_I32(additi, I);

    CREDO_GRADUS("tabula: tessera nusquam");
    e = scrinium_praeparare(volumen_scrinium(m->volumen),
        "SELECT id, civis, spatium, titulus, digestum, replica"
        " FROM communio_apparatus");
    CREDO_NECESSE_NON_NIHIL(e);
    dum (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        per (k = ZEPHYRUM; k < VI; k++)
        {
            CREDO_FALSUM(continet(scrinium_columna_textus(e, (integer)k,
                piscina), lit(tessera)));
        }
    }
    scrinium_finire(e);
}


/* ==================================================
 * T4: protocollum per HTTP (hospitium et http_vocare in reactore uno)
 * ================================================== */

nomen structura {
              Reactor* reactor;
            Hospitium* hospitium;
                  i32  portus;
              Volumen* vx;          /* X: Francisci, familia */
              Volumen* vy;          /* Y: Amandae, familia */
              Volumen* vz;          /* Z: Francisci, alia */
            character  tx[XLIV];
            character  ty[XLIV];
            character  tz[XLIV];
            character  tw[XLIV];     /* W: revocatus */
} Rete;

nomen structura {
       i32 status;
    chorda corpus;
} Responsum;

interior vacuum
_finis (
         vacuum* datum,
    HttpVocatio* vocatio)
{
    (vacuum)vocatio;
    *(b32*)datum = VERUM;
}

/* petitio una; tessera NIHIL = sine Authorization; status 0 = defectus
 * vecturae */
interior Responsum
rogare (
                 Rete* r,
         HttpMethodus  methodus,
   constans character* via,
   constans character* tessera,
               chorda  corpus)
{
     HttpPetitio* pet;
     HttpVocatio* voc;
    HttpResultus  res;
       Responsum  exitus;
       character  url[CCLVI];
       character  auctoritas[LXIV];
             b32  finita;
             i32  gyri;

    exitus.status          = ZEPHYRUM;
    exitus.corpus.datum    = NIHIL;
    exitus.corpus.mensura  = ZEPHYRUM;
    sprintf(url, "http://127.0.0.1:%d%s", (integer)r->portus, via);
    pet = http_petitio_creare(piscina, methodus, url);
    si (!pet)
    {
        redde exitus;
    }
    si (tessera)
    {
        sprintf(auctoritas, "Bearer %s", tessera);
        http_petitio_caput_addere(pet, "Authorization", auctoritas);
    }
    si (corpus.mensura > ZEPHYRUM)
    {
        http_petitio_caput_addere(pet, "Content-Type",
            methodus == HTTP_PUT ? "application/octet-stream"
                                 : "application/json");
        http_petitio_corpus_ponere_chorda(pet, corpus);
    }
    finita = FALSUM;
    voc = http_vocare(r->reactor, pet, NIHIL, NIHIL, _finis, &finita,
        NIHIL, piscina);
    per (gyri = ZEPHYRUM; voc && !finita && gyri < M; gyri++)
    {
        (vacuum)reactor_poll(r->reactor, X);
    }
    si (!voc || !finita)
    {
        redde exitus;
    }
    res = http_vocatio_resultus(voc);
    si (res.successus && res.responsum)
    {
        exitus.status = res.responsum->status;
        exitus.corpus = res.responsum->corpus;
    }
    redde exitus;
}

interior Responsum
rogare_json (
                 Rete* r,
         HttpMethodus  methodus,
   constans character* via,
   constans character* tessera,
   constans character* corpus)
{
    redde rogare(r, methodus, via, tessera, corpus ? lit(corpus)
                                                   : lit(""));
}

interior JsonValor*
radix (
    Responsum q)
{
    JsonResultus j;

    j = json_legere(q.corpus, piscina);
    redde j.successus ? j.radix : NIHIL;
}

/* numerus actorum replicae in volumine */
interior i32
acta_replicae (
    Volumen* v,
     chorda  replica)
{
    Xar* l;
    i32  i;
    i32  n;

    l = volumen_acta_legere(v, ZEPHYRUM, piscina);
    n = ZEPHYRUM;
    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        si (chorda_aequalis(((VolumenActum*)xar_obtinere(l, i))
                ->identitas.replica, replica))
        {
            n++;
        }
    }
    redde n;
}

/* apparatum inscribere et replicam voluminis eius ligare */
interior b32
apparatum_inscribere (
            Mundus* m,
            chorda  civis,
constans character* spatium,
constans character* titulus,
           Volumen* v,
         character* tessera)
{
    character digestum[LXV];
    character id[MONETA_ULID_MENSURA];

    redde communio_membrum_addere(m->servus, lit(spatium), civis)
        && communio_tesseram_facere(tessera, digestum)
        && communio_apparatum_addere(m->servus, civis, lit(spatium),
               lit(titulus), lit(digestum), id)
        && (!v || communio_replicam_ligare(m->servus, lit(id),
               volumen_replica(v)) == ZEPHYRUM);
}

interior Rete*
rete_parare (
    Mundus* m)
{
                     Rete* r;
    HospitiumConfiguratio  cfg;
                character  digestum[LXV];
                character  id[MONETA_ULID_MENSURA];

    r = (Rete*)piscina_allocare(piscina, magnitudo(Rete));
    memset(r, ZEPHYRUM, magnitudo(Rete));
    r->reactor = reactor_creare(piscina);
    memset(&cfg, ZEPHYRUM, magnitudo(cfg));
    cfg.reactor = r->reactor;
    r->hospitium = r->reactor ? hospitium_creare(piscina, &cfg) : NIHIL;
    si (   !r->hospitium || !communio_servum_montare(m->servus,
            r->hospitium))
    {
        redde NIHIL;
    }
    r->portus  = (i32)hospitium_portus(r->hospitium);
    r->vx      = volumen_temporarium(piscina, "probatio_communio_rx");
    r->vy      = volumen_temporarium(piscina, "probatio_communio_ry");
    r->vz      = volumen_temporarium(piscina, "probatio_communio_rz");
    si (   !r->vx || !r->vy || !r->vz
        || !apparatum_inscribere(m, m->franciscus, "familia",
               "Macintosh", r->vx, r->tx)
        || !apparatum_inscribere(m, m->amanda, "familia", "iPhone",
               r->vy, r->ty)
        || !apparatum_inscribere(m, m->franciscus, "alia", "Tabula",
               r->vz, r->tz)
        || !communio_tesseram_facere(r->tw, digestum)
        || !communio_apparatum_addere(m->servus, m->amanda,
               lit("familia"), lit("Revocatus"), lit(digestum), id)
        || !communio_apparatum_revocare(m->servus, lit(id)))
    {
        redde NIHIL;
    }
    redde r;
}

interior vacuum
rete_purgare (
    Rete* r)
{
    si (!r)
    {
        redde;
    }
    si (r->hospitium)
    {
        hospitium_destruere(r->hospitium);
    }
    si (r->reactor)
    {
        reactor_destruere(r->reactor);
    }
    si (r->vx)
    {
        volumen_claudere(r->vx);
    }
    si (r->vy)
    {
        volumen_claudere(r->vy);
    }
    si (r->vz)
    {
        volumen_claudere(r->vz);
    }
}

/* acta propria voluminis post seq ut corpus {"acta":[...]} */
interior chorda
corpus_actorum (
    Volumen* v,
        s64  post)
{
    redde communio_acta_scribere(volumen_acta_propria_legere(v, post,
        piscina), piscina);
}

interior s64
acta_addere (
    Volumen* v,
        i32  n)
{
    s64 ultimum;
    i32 i;

    ultimum = ZEPHYRUM;
    per (i = ZEPHYRUM; i < n; i++)
    {
        ultimum = volumen_actum_appendere(v, "x/actum",
            lit("{\"n\":1}"));
    }
    redde ultimum;
}

interior vacuum
_sectio_tessera_http (
    CredoContextus* c)
{
                Mundus* m;
                  Rete* r;
             character  via[CXXVIII];
                   i32  i;
    constans character* viae[V];
          HttpMethodus  methodi[V];

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->servus);
    r = rete_parare(m);
    CREDO_NECESSE_NON_NIHIL(r);
    sprintf(via, "/communio/massae/%064d", ZEPHYRUM);
    viae[ZEPHYRUM]     = "/communio/acta?post=0";
    viae[I]            = "/communio/acta";
    viae[II]           = "/communio/apparatus";
    viae[III]          = "/communio/massae/desunt";
    viae[IV]           = via;
    methodi[ZEPHYRUM]  = HTTP_GET;
    methodi[I]         = HTTP_POST;
    methodi[II]        = HTTP_GET;
    methodi[III]       = HTTP_POST;
    methodi[IV]        = HTTP_GET;
    per (i = ZEPHYRUM; i < V; i++)
    {
        CREDO_AEQUALIS_I32(rogare_json(r, methodi[i], viae[i], NIHIL,
            "{}").status, CDI);
        CREDO_AEQUALIS_I32(rogare_json(r, methodi[i], viae[i], r->tw,
            "{}").status, CDI);
        CREDO_AEQUALIS_I32(rogare_json(r, methodi[i], viae[i],
            "nonsensus", "{}").status, CDI);
    }
    CREDO_AEQUALIS_I32(rogare_json(r, HTTP_PUT, via, NIHIL, "x").status,
        CDI);
    CREDO_AEQUALIS_I32(rogare_json(r, HTTP_POST, "/communio/apparatus",
        NIHIL, "{}").status, CDI);
    rete_purgare(r);
}

interior vacuum
_sectio_ligare_http (
    CredoContextus* c)
{
       Mundus* m;
         Rete* r;
    Responsum  q;
    character  corpus[CXXVIII];
    character  tessera[XLIV];
    character  replica[MONETA_ULID_MENSURA];

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->servus);
    r = rete_parare(m);
    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_NECESSE(apparatum_inscribere(m, m->amanda, "familia", "Nova",
        NIHIL, tessera));
    moneta_ulid(replica);
    sprintf(corpus, "{\"replica\":\"%s\"}", replica);
    q = rogare_json(r, HTTP_POST, "/communio/apparatus", tessera,
        corpus);
    CREDO_AEQUALIS_I32(q.status, CC);
    CREDO_CHORDA_AEQUALIS_LITERIS(json_capere_chorda(radix(q),
        "spatium",
        lit("")), "familia");
    CREDO_AEQUALIS_I32(rogare_json(r, HTTP_POST, "/communio/apparatus",
        tessera, corpus).status, CC);
    sprintf(corpus, "{\"replica\":\"%.26s\"}",
        (constans character*)volumen_replica(r->vx).datum);
    CREDO_AEQUALIS_I32(rogare_json(r, HTTP_POST, "/communio/apparatus",
        tessera, corpus).status, CDIX);
    CREDO_AEQUALIS_I32(rogare_json(r, HTTP_POST, "/communio/apparatus",
        tessera, "{\"replica\":\"minuscula\"}").status, CD);
    CREDO_AEQUALIS_I32(rogare_json(r, HTTP_POST, "/communio/apparatus",
        tessera, "non json").status, CD);

    CREDO_GRADUS("auctores: apparatus spatii");
    q = rogare_json(r, HTTP_GET, "/communio/apparatus", r->ty, NIHIL);
    CREDO_AEQUALIS_I32(q.status, CC);
    {
         JsonValor* l = json_objectum_capere(radix(q), "apparatus");
               i32  i;
               b32  inventum = FALSUM;

        CREDO_NECESSE_NON_NIHIL(l);
        per (i = ZEPHYRUM; i < json_tabulatum_numerus(l); i++)
        {
            JsonValor* a = json_tabulatum_obtinere(l, i);

            CREDO_FALSUM(json_objectum_capere(a, "digestum") != NIHIL);
            si (chorda_aequalis(json_capere_chorda(a, "replica",
                lit("")),
                    volumen_replica(r->vx)))
            {
                inventum = VERUM;
                CREDO_CHORDA_AEQUALIS_LITERIS(json_capere_chorda(a,
                    "persona", lit("")), "Franciscus");
                CREDO_CHORDA_AEQUALIS_LITERIS(json_capere_chorda(a,
                    "titulus", lit("")), "Macintosh");
                CREDO_CHORDA_AEQUALIS(json_capere_chorda(a, "civis",
                    lit("")), m->franciscus);
            }
            CREDO_FALSUM(chorda_aequalis(json_capere_chorda(a,
                "replica", lit("")), volumen_replica(r->vz)));
        }
        CREDO_VERUM(inventum);
    }
    rete_purgare(r);
}

interior vacuum
_sectio_acta_http (
    CredoContextus* c)
{
       Mundus* m;
         Rete* r;
    Responsum  q;
       chorda  corpus;
          Xar* propria;
          Xar* lecta;
          i32  i;
          i32  ante;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->servus);
    r = rete_parare(m);
    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_NECESSE(acta_addere(r->vx, III) > ZEPHYRUM);
    corpus = corpus_actorum(r->vx, ZEPHYRUM);

    CREDO_GRADUS("replica aliena recusatur");
    q = rogare(r, HTTP_POST, "/communio/acta", r->ty, corpus);
    CREDO_AEQUALIS_I32(q.status, CDIII);
    CREDO_AEQUALIS_I32(acta_replicae(m->volumen,
        volumen_replica(r->vx)),
        ZEPHYRUM);

    CREDO_GRADUS("acta missa");
    q = rogare(r, HTTP_POST, "/communio/acta", r->tx, corpus);
    CREDO_AEQUALIS_I32(q.status, CC);
    propria = volumen_acta_propria_legere(r->vx, ZEPHYRUM, piscina);
    CREDO_AEQUALIS_S64(json_capere_integer(radix(q), "accepta", -I),
        (s64)xar_numerus(propria));
    ante = acta_replicae(m->volumen, volumen_replica(r->vx));
    CREDO_AEQUALIS_I32(ante, xar_numerus(propria));

    CREDO_GRADUS("tractio: Y videt, X non, Z non");
    q = rogare_json(r, HTTP_GET, "/communio/acta?post=0", r->ty, NIHIL);
    CREDO_AEQUALIS_I32(q.status, CC);
    lecta = communio_acta_legere(q.corpus, NIHIL, piscina);
    CREDO_NECESSE_NON_NIHIL(lecta);
    CREDO_AEQUALIS_I32(xar_numerus(lecta), xar_numerus(propria));
    per (i = ZEPHYRUM; i < xar_numerus(lecta); i++)
    {
        VolumenActum* a = (VolumenActum*)xar_obtinere(lecta, i);
        VolumenActum* b = (VolumenActum*)xar_obtinere(propria, i);

        CREDO_AEQUALIS_S64(a->identitas.hora, b->identitas.hora);
        CREDO_CHORDA_AEQUALIS(a->identitas.replica,
            b->identitas.replica);
        CREDO_CHORDA_AEQUALIS(a->datum, b->datum);
        CREDO_CHORDA_AEQUALIS(a->genus, b->genus);
        CREDO_AEQUALIS_S64(a->seq_originis, b->seq);
    }
    CREDO_FALSUM(json_capere_boolean(radix(q), "plura", VERUM));
    CREDO_VERUM(json_capere_integer(radix(q), "cursor", ZEPHYRUM)
        > ZEPHYRUM);
    q = rogare_json(r, HTTP_GET, "/communio/acta?post=0", r->tx, NIHIL);
    CREDO_AEQUALIS_I32(xar_numerus(communio_acta_legere(q.corpus, NIHIL,
        piscina)), ZEPHYRUM);
    q = rogare_json(r, HTTP_GET, "/communio/acta?post=0", r->tz, NIHIL);
    CREDO_AEQUALIS_I32(xar_numerus(communio_acta_legere(q.corpus, NIHIL,
        piscina)), ZEPHYRUM);
    CREDO_AEQUALIS_I32(rogare_json(r, HTTP_GET, "/communio/acta?post=x",
        r->ty, NIHIL).status, CD);

    CREDO_GRADUS("iterum post abruptionem");
    q = rogare(r, HTTP_POST, "/communio/acta", r->tx, corpus);
    CREDO_AEQUALIS_I32(q.status, CC);
    CREDO_AEQUALIS_S64(json_capere_integer(radix(q), "accepta", -I),
        (s64)xar_numerus(propria));
    CREDO_AEQUALIS_I32(acta_replicae(m->volumen,
        volumen_replica(r->vx)),
        ante);

    CREDO_GRADUS("invalidum: via sub aliena/");
    {
        VolumenActum  malum;
                 Xar* l;

        malum = *(VolumenActum*)xar_obtinere(propria, ZEPHYRUM);
        malum.genus = lit("plagula-condita");
        malum.datum = lit("{\"via\":\"aliena/X/y\"}");
        malum.identitas.hora = malum.identitas.hora + I;
        l = xar_creare(piscina, magnitudo(VolumenActum));
        *(VolumenActum*)xar_addere(l) = malum;
        q = rogare(r, HTTP_POST, "/communio/acta", r->tx,
            communio_acta_scribere(l, piscina));
        CREDO_AEQUALIS_I32(q.status, CD);
        CREDO_AEQUALIS_I32(acta_replicae(m->volumen,
            volumen_replica(r->vx)), ante);
    }
    CREDO_AEQUALIS_I32(rogare_json(r, HTTP_POST, "/communio/acta",
        r->tx,
        "non json").status, CD);
    rete_purgare(r);
}

interior vacuum
_sectio_massae_http (
    CredoContextus* c)
{
        Mundus* m;
          Rete* r;
     Responsum  q;
           Xar* propria;
           Xar* l;
        chorda  contentum;
        chorda  hex;
     character  via[CXXVIII];
     character  corpus[CCLVI];
           b32  inventum;
     JsonValor* massae;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->servus);
    r = rete_parare(m);
    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_NECESSE(volumen_plagulam_condere(r->vx, lit("p/documentum"),
        lit("<documentum latitudo=\"7\"/>"), "probatio"));
    propria = volumen_acta_propria_legere(r->vx, ZEPHYRUM, piscina);
    CREDO_NECESSE_NON_NIHIL(propria);
    l = communio_massas_nominatas((VolumenActum*)xar_obtinere(propria,
        xar_numerus(propria) - I), piscina);
    CREDO_NECESSE(l && xar_numerus(l) == I);
    hex = *(chorda*)xar_obtinere(l, ZEPHYRUM);

    CREDO_GRADUS("massa prius");
    q = rogare(r, HTTP_POST, "/communio/acta", r->tx,
        corpus_actorum(r->vx, ZEPHYRUM));
    CREDO_AEQUALIS_I32(q.status, CC);
    CREDO_CHORDA_AEQUALIS_LITERIS(json_capere_chorda(
        json_objectum_capere(radix(q), "refusum"), "causa", lit("")),
        "massa-deest");
    sprintf(corpus, "{\"massae\":[\"%.64s\"]}",
        (constans character*)hex.datum);
    q = rogare_json(r, HTTP_POST, "/communio/massae/desunt", r->tx,
        corpus);
    CREDO_AEQUALIS_I32(q.status, CC);
    CREDO_AEQUALIS_I32(json_tabulatum_numerus(json_objectum_capere(
        radix(q), "desunt")), I);
    contentum = volumen_massam_promere(r->vx, hex, piscina, &inventum);
    CREDO_NECESSE(inventum);
    sprintf(via, "/communio/massae/%.64s",
        (constans character*)hex.datum);

    CREDO_GRADUS("massa falsa");
    q = rogare(r, HTTP_PUT, via, r->tx, lit("octeti alii"));
    CREDO_AEQUALIS_I32(q.status, CD);
    CREDO_FALSUM(volumen_massam_habet(m->volumen, hex));

    CREDO_GRADUS("massa vera, deinde acta");
    q = rogare(r, HTTP_PUT, via, r->tx, contentum);
    CREDO_AEQUALIS_I32(q.status, CCI);
    CREDO_VERUM(volumen_massam_habet(m->volumen, hex));
    q = rogare(r, HTTP_POST, "/communio/acta", r->tx,
        corpus_actorum(r->vx, ZEPHYRUM));
    CREDO_AEQUALIS_I32(q.status, CC);
    CREDO_VERUM(json_objectum_capere(radix(q), "refusum") == NIHIL
        || json_objectum_capere(radix(q), "refusum")->genus
           == JSON_NULLUM);
    q = rogare_json(r, HTTP_POST, "/communio/massae/desunt", r->tx,
        corpus);
    CREDO_AEQUALIS_I32(json_tabulatum_numerus(json_objectum_capere(
        radix(q), "desunt")), ZEPHYRUM);

    CREDO_GRADUS("Y: massae nominatae et lectio");
    q = rogare_json(r, HTTP_GET, "/communio/acta?post=0", r->ty, NIHIL);
    massae = json_objectum_capere(radix(q), "massae");
    CREDO_NECESSE(massae && json_tabulatum_numerus(massae) == I);
    CREDO_CHORDA_AEQUALIS(json_tabulatum_obtinere(massae, ZEPHYRUM)
        ->datum.chorda_valor, hex);
    q = rogare_json(r, HTTP_GET, via, r->ty, NIHIL);
    CREDO_AEQUALIS_I32(q.status, CC);
    CREDO_CHORDA_AEQUALIS(q.corpus, contentum);
    sprintf(via, "/communio/massae/%064d", ZEPHYRUM);
    CREDO_AEQUALIS_I32(rogare_json(r, HTTP_GET, via, r->ty,
        NIHIL).status,
        CDIV);
    rete_purgare(r);
}

interior vacuum
_sectio_paginae_http (
    CredoContextus* c)
{
       Mundus* m;
         Rete* r;
    Responsum  q;
          Xar* l;
          s64  cursor;
    character  via[LXIV];

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->servus);
    r = rete_parare(m);
    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_NECESSE(acta_addere(r->vx, CCL) > ZEPHYRUM);
    l = volumen_acta_propria_legere(r->vx, ZEPHYRUM, piscina);
    CREDO_NECESSE(l && xar_numerus(l) > CC);
    /* missio in duabus partibus (CC ad plurimum per petitionem) */
    {
        Xar* prima   = xar_creare(piscina, magnitudo(VolumenActum));
        Xar* altera  = xar_creare(piscina, magnitudo(VolumenActum));
        i32  i;

        per (i = ZEPHYRUM; i < xar_numerus(l); i++)
        {
            *(VolumenActum*)xar_addere(i < CC ? prima : altera) =
                *(VolumenActum*)xar_obtinere(l, i);
        }
        CREDO_AEQUALIS_I32(rogare(r, HTTP_POST, "/communio/acta", r->tx,
            communio_acta_scribere(prima, piscina)).status, CC);
        CREDO_AEQUALIS_I32(rogare(r, HTTP_POST, "/communio/acta", r->tx,
            communio_acta_scribere(altera, piscina)).status, CC);
    }
    q = rogare_json(r, HTTP_GET, "/communio/acta?post=0", r->ty, NIHIL);
    CREDO_AEQUALIS_I32(xar_numerus(communio_acta_legere(q.corpus, NIHIL,
        piscina)), CC);
    CREDO_VERUM(json_capere_boolean(radix(q), "plura", FALSUM));
    cursor = json_capere_integer(radix(q), "cursor", ZEPHYRUM);
    sprintf(via, "/communio/acta?post=%ld", (longus)cursor);
    q = rogare_json(r, HTTP_GET, via, r->ty, NIHIL);
    CREDO_AEQUALIS_I32(xar_numerus(communio_acta_legere(q.corpus, NIHIL,
        piscina)), xar_numerus(l) - CC);
    CREDO_FALSUM(json_capere_boolean(radix(q), "plura", VERUM));
    rete_purgare(r);
}


/* ==================================================
 * T5: tractio viva (long-poll)
 * ================================================== */

nomen structura {
    HttpVocatio* vocatio;
            b32  finita;
} Exspectatio;

/* petitionem GET incipere sine exspectatione (reactor postea agitur) */
interior b32
exspectare (
                Rete* r,
  constans character* via,
  constans character* tessera,
         Exspectatio* e)
{
     HttpPetitio* pet;
       character  url[CCLVI];
       character  auctoritas[LXIV];

    e->finita = FALSUM;
    sprintf(url, "http://127.0.0.1:%d%s", (integer)r->portus, via);
    pet = http_petitio_creare(piscina, HTTP_GET, url);
    si (!pet)
    {
        redde FALSUM;
    }
    sprintf(auctoritas, "Bearer %s", tessera);
    http_petitio_caput_addere(pet, "Authorization", auctoritas);
    e->vocatio = http_vocare(r->reactor, pet, NIHIL, NIHIL, _finis,
        &e->finita, NIHIL, piscina);
    redde e->vocatio != NIHIL;
}

/* reactorem agere: gyri X ms */
interior vacuum
pulsare (
    Rete* r,
     i32  gyri)
{
    i32 i;

    per (i = ZEPHYRUM; i < gyri; i++)
    {
        (vacuum)reactor_poll(r->reactor, X);
    }
}

/* actorum numerus responsi exspectationis finitae (-1 si non CC) */
interior s32
acta_responsi (
    Exspectatio* e)
{
    HttpResultus  res;
             Xar* l;

    res = http_vocatio_resultus(e->vocatio);
    si (!res.successus || !res.responsum || res.responsum->status != CC)
    {
        redde -I;
    }
    l = communio_acta_legere(res.responsum->corpus, NIHIL, piscina);
    redde l ? (s32)xar_numerus(l) : -I;
}

interior vacuum
_sectio_exspectat (
    CredoContextus* c)
{
         Mundus* m;
           Rete* r;
    Exspectatio  y;
    Exspectatio  w;
    Exspectatio  x;
      character  tessera_quarta[XLIV];
        Volumen* volumen_quartum;
            i32  gyri;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->servus);
    r = rete_parare(m);
    CREDO_NECESSE_NON_NIHIL(r);
    volumen_quartum = volumen_temporarium(piscina,
        "probatio_communio_rw");
    CREDO_NECESSE_NON_NIHIL(volumen_quartum);
    CREDO_NECESSE(apparatum_inscribere(m, m->amanda, "familia", "iPad",
        volumen_quartum, tessera_quarta));
    CREDO_NECESSE(acta_addere(r->vx, III) > ZEPHYRUM);

    CREDO_GRADUS("exspectat donec X mittit; duo exspectantes");
    CREDO_NECESSE(exspectare(r, "/communio/acta?post=0&mora=5", r->ty,
        &y));
    CREDO_NECESSE(exspectare(r, "/communio/acta?post=0&mora=5",
        tessera_quarta,
        &w));
    pulsare(r, X);
    CREDO_FALSUM(y.finita);
    CREDO_FALSUM(w.finita);
    CREDO_AEQUALIS_I32(rogare(r, HTTP_POST, "/communio/acta", r->tx,
        corpus_actorum(r->vx, ZEPHYRUM)).status, CC);
    per (gyri = ZEPHYRUM; (!y.finita || !w.finita) && gyri < C; gyri++)
    {
        pulsare(r, I);
    }
    CREDO_VERUM(y.finita);
    CREDO_VERUM(w.finita);
    CREDO_AEQUALIS_S64((s64)acta_responsi(&y), (s64)xar_numerus(
        volumen_acta_propria_legere(r->vx, ZEPHYRUM, piscina)));
    CREDO_AEQUALIS_S64((s64)acta_responsi(&w), (s64)acta_responsi(&y));

    CREDO_GRADUS("propria non excitat");
    CREDO_NECESSE(exspectare(r, "/communio/acta?post=0&mora=1", r->tx,
        &x));
    pulsare(r, V);
    CREDO_NECESSE(acta_addere(r->vx, I) > ZEPHYRUM);
    CREDO_AEQUALIS_I32(rogare(r, HTTP_POST, "/communio/acta", r->tx,
        corpus_actorum(r->vx, ZEPHYRUM)).status, CC);
    pulsare(r, V);
    CREDO_FALSUM(x.finita);
    per (gyri = ZEPHYRUM; !x.finita && gyri < CC; gyri++)
    {
        pulsare(r, I);
    }
    CREDO_VERUM(x.finita);
    CREDO_AEQUALIS_S64((s64)acta_responsi(&x), ZEPHYRUM);

    CREDO_GRADUS("mora mala");
    CREDO_AEQUALIS_I32(rogare_json(r, HTTP_GET,
        "/communio/acta?post=0&mora=x", r->ty, NIHIL).status, CD);
    volumen_claudere(volumen_quartum);
    rete_purgare(r);
}

interior vacuum
_sectio_tempus (
    CredoContextus* c)
{
         Mundus* m;
           Rete* r;
    Exspectatio  y;
            i32  gyri;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->servus);
    r = rete_parare(m);
    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_NECESSE(exspectare(r, "/communio/acta?post=0&mora=1", r->ty,
        &y));
    pulsare(r, L);
    CREDO_FALSUM(y.finita);
    per (gyri = ZEPHYRUM; !y.finita && gyri < CCC; gyri++)
    {
        pulsare(r, I);
    }
    CREDO_VERUM(y.finita);
    CREDO_AEQUALIS_S64((s64)acta_responsi(&y), ZEPHYRUM);
    rete_purgare(r);
}

interior vacuum
_sectio_abrumpere (
    CredoContextus* c)
{
         Mundus* m;
           Rete* r;
    Exspectatio  y;
      Responsum  q;
            i32  horae;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->servus);
    r = rete_parare(m);
    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_NECESSE(exspectare(r, "/communio/acta?post=0&mora=5", r->ty,
        &y));
    pulsare(r, V);
    http_vocationem_abrumpere(y.vocatio);
    pulsare(r, V);
    CREDO_NECESSE(acta_addere(r->vx, II) > ZEPHYRUM);
    CREDO_AEQUALIS_I32(rogare(r, HTTP_POST, "/communio/acta", r->tx,
        corpus_actorum(r->vx, ZEPHYRUM)).status, CC);
    q = rogare_json(r, HTTP_GET, "/communio/acta?post=0", r->ty, NIHIL);
    CREDO_AEQUALIS_I32(q.status, CC);
    CREDO_VERUM(xar_numerus(communio_acta_legere(q.corpus, NIHIL,
        piscina)) > ZEPHYRUM);

    CREDO_GRADUS("destruere: nullum tempus relictum");
    CREDO_NECESSE(exspectare(r, "/communio/acta?post=999999&mora=5",
        r->ty, &y));
    pulsare(r, V);
    horae = reactor_numerus_timer(r->reactor);
    communio_servum_destruere(m->servus);
    CREDO_AEQUALIS_I32(reactor_numerus_timer(r->reactor), horae - I);
    http_vocationem_abrumpere(y.vocatio);
    hospitium_destruere(r->hospitium);
    r->hospitium = NIHIL;
    CREDO_AEQUALIS_I32(reactor_numerus_timer(r->reactor), ZEPHYRUM);
    rete_purgare(r);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "apparatus", _sectio_apparatus, _parare, _purgare, NIHIL, NIHIL },
    { "revocatus", _sectio_revocatus, _parare, _purgare, NIHIL, NIHIL },
    { "persona suspensa", _sectio_suspensa, _parare, _purgare, NIHIL,
        NIHIL },
    { "non membrum", _sectio_non_membrum, _parare, _purgare, NIHIL,
        NIHIL },
    { "digestum iteratum et malum", _sectio_digestum, _parare, _purgare,
        NIHIL, NIHIL },
    { "ligare", _sectio_ligare, _parare, _purgare, NIHIL, NIHIL },
    { "secretum nusquam", _sectio_secretum, _parare, _purgare, NIHIL,
        NIHIL },
    { "tessera (HTTP)", _sectio_tessera_http, _parare, _purgare, NIHIL,
        NIHIL },
    { "ligare et auctores (HTTP)", _sectio_ligare_http, _parare,
        _purgare, NIHIL, NIHIL },
    { "acta et tractio (HTTP)", _sectio_acta_http, _parare, _purgare,
        NIHIL, NIHIL },
    { "massae (HTTP)", _sectio_massae_http, _parare, _purgare, NIHIL,
        NIHIL },
    { "paginae (HTTP)", _sectio_paginae_http, _parare, _purgare, NIHIL,
        NIHIL },
    { "exspectat (long-poll)", _sectio_exspectat, _parare, _purgare,
        NIHIL, NIHIL },
    { "tempus (long-poll)", _sectio_tempus, _parare, _purgare, NIHIL,
        NIHIL },
    { "cliens abiit (long-poll)", _sectio_abrumpere, _parare,
        _purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("communio_servus", SECTIONES);
}

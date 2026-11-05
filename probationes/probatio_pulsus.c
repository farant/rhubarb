/* probatio_pulsus.c - pulsus (pulsus-plan T2): inscriptiones, greges
 * per mittentem et clavem, quies (hora ficta), statim, sine fine,
 * restitutio post initium novum, destinatarii (persona mittentis,
 * revocati, spatia alia), signa mortua, probatio. Mittens fictus
 * missiones servat et statim respondet.
 * T3: viae per HTTP (hospitium verum in 127.0.0.1, hora vera:
 * 401/204/202, hora reactoris gregem mittit) et bombae (petitiones
 * malae 400, tabulae et memoria immutatae). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "json.h"
#include "volumen.h"
#include "cives.h"
#include "communio_forma.h"
#include "communio_servus.h"
#include "pulsus_forma.h"
#include "pulsus.h"
#include "scrinium.h"
#include "reactor.h"
#include "hospitium.h"
#include "http.h"
#include "http_vocatio.h"
#include "credo.h"
#include <string.h>
#include <stdio.h>
#include <sys/time.h>

/* apparatus mundi (indices in Mundus.apparatus) */
#define F_TELEPHONUM  ZEPHYRUM
#define F_MAC     I
#define A_TELEPHONUM  II
#define A_TABULA    III
#define A_VETUS   IV
#define P_TELEPHONUM  V
#define APPARATUS VI

nomen structura {
    chorda signum;
    chorda thema;
    chorda corpus;
    chorda apparatus;
} Missa;

nomen structura {
           Volumen* volumen;
             Cives* cives;
    CommunioServus* servus;
            Pulsus* pulsus;
            chorda  fran;
            chorda  amanda;
            chorda  petrus;
         character  apparatus[APPARATUS][XXVII];
         character  tesserae[APPARATUS][XLIV];  /* Bearer communionis */
               Xar* missae;           /* Missa */
            chorda  mortuum;          /* signum cui MORTUUM respondetur */
} Mundus;

hic_manens Piscina* piscina;
hic_manens     s64  hora_ficta;
hic_manens  Mundus* mundus;
/* VERUM: pulsus horologio vero (viae: hora reactoris vera) */
hic_manens     b32  hora_vera;

interior s64
_hora (vacuum)
{
    redde hora_ficta;
}

interior chorda
lit (
    constans character* s)
{
    redde chorda_ex_literis(s, piscina);
}

interior chorda
copia (
    chorda s)
{
    redde chorda_transcribere(s, piscina);
}

/* mittens fictus: missionem copiat, statim respondet */
interior b32
_mittere (
                   vacuum* sui,
    constans PulsusMissio* m,
              PulsusFinis  finis,
                   vacuum* ctx)
{
          Mundus* w;
           Missa* x;
    PulsusMissio  r;

    w             = (Mundus*)sui;
    x             = (Missa*)xar_addere(w->missae);
    x->signum     = copia(m->signum);
    x->thema      = copia(m->thema);
    x->corpus     = copia(m->corpus);
    x->apparatus  = copia(m->apparatus);
    r             = *m;
    finis(ctx, &r, chorda_aequalis(m->signum, w->mortuum)
        ? PULSUS_SIGNUM_MORTUUM : PULSUS_TRADITUM, lit(""));
    redde VERUM;
}

interior vacuum
_apparatum (
                Mundus* m,
                chorda  civis,
    constans character* spatium,
    constans character* titulus,
                   i32  index)
{
    character digestum[LXV];

    CREDO_NECESSE(communio_membrum_addere(m->servus, lit(spatium),
        civis));
    CREDO_NECESSE(communio_tesseram_facere(m->tesserae[index],
        digestum));
    CREDO_NECESSE(communio_apparatum_addere(m->servus, civis,
        lit(spatium), lit(titulus), lit(digestum),
        m->apparatus[index]));
}

interior chorda
app (
    Mundus* m,
       i32  index)
{
    redde lit(m->apparatus[index]);
}

/* signa: f1 f2 a1 a2 a3 p1 (hex) */
interior vacuum
_inscribere_omnes (
    Mundus* m)
{
    hic_manens constans character* constans SIGNA[APPARATUS] = {
        "f1", "f2", "a1", "a2", "a3", "b1"
    };
    i32 i;

    per (i = ZEPHYRUM; i < APPARATUS; i++)
    {
        CREDO_NECESSE(pulsus_inscribere(m->pulsus, app(m, i),
            lit(SIGNA[i]), lit("sandbox"), lit("earth.golden.pictor")));
    }
}

interior vacuum
_parare (
    CredoContextus* c)
{
    Mundus* m;

    piscina     = c->piscina;
    hora_ficta  = (s64)M * M;
    m           = (Mundus*)piscina_allocare(piscina, magnitudo(Mundus));
    memset(m, ZEPHYRUM, magnitudo(Mundus));
    mundus      = m;
    c->datum    = m;
    m->missae   = xar_creare(piscina, magnitudo(Missa));
    m->mortuum  = lit("");
    m->volumen  = volumen_temporarium(piscina, "probatio_pulsus");
    CREDO_NECESSE(m->volumen != NIHIL);
    m->cives = cives_aperire(m->volumen, NIHIL, piscina);
    CREDO_NECESSE(m->cives != NIHIL);
    CREDO_NECESSE(volumen_transactionem_incipere(m->volumen));
    (vacuum)cives_creare(m->cives, lit("fran"), lit("Fran"), &m->fran,
        piscina);
    (vacuum)cives_creare(m->cives, lit("amanda"), lit("Amanda"),
        &m->amanda, piscina);
    (vacuum)cives_creare(m->cives, lit("petrus"), lit("Petrus"),
        &m->petrus, piscina);
    (vacuum)volumen_transactionem_committere(m->volumen);
    m->servus = communio_servum_creare(piscina, m->volumen, m->cives,
        _hora);
    CREDO_NECESSE(m->servus != NIHIL);
    _apparatum(m, m->fran, "familia", "iPhone", F_TELEPHONUM);
    _apparatum(m, m->fran, "familia", "Macintosh", F_MAC);
    _apparatum(m, m->amanda, "familia", "iPhone", A_TELEPHONUM);
    _apparatum(m, m->amanda, "familia", "iPad", A_TABULA);
    _apparatum(m, m->amanda, "familia", "vetus", A_VETUS);
    _apparatum(m, m->petrus, "alia", "iPhone", P_TELEPHONUM);
    CREDO_NECESSE(communio_apparatum_revocare(m->servus,
        app(m, A_VETUS)));
    m->pulsus = pulsus_creare(piscina, m->volumen, m->servus, m->cives,
        hora_vera ? NIHIL : _hora);
    CREDO_NECESSE(m->pulsus != NIHIL);
    pulsus_mittentem_ponere(m->pulsus, _mittere, m);
    _inscribere_omnes(m);
}

interior vacuum
_parare_verum (
    CredoContextus* c)
{
    hora_vera = VERUM;
    _parare(c);
}

interior vacuum
_purgare (
    CredoContextus* c)
{
    Mundus* m;

    m = (Mundus*)c->datum;
    si (m && m->pulsus)
    {
        pulsus_destruere(m->pulsus);
    }
    si (m && m->servus)
    {
        communio_servum_destruere(m->servus);
    }
    si (m && m->volumen)
    {
        volumen_claudere(m->volumen);
    }
    piscina    = NIHIL;
    mundus     = NIHIL;
    hora_vera  = FALSUM;
}

/* eventus pictoris cum rebus ids (NIHIL-terminatis) */
interior PulsusEventus
eventus (
    constans character* constans* ids,
                             b32  statim)
{
    PulsusEventus e;
              i32 i;

    memset(&e, ZEPHYRUM, magnitudo(e));
    e.clavis  = lit("pictor.picturae");
    e.thema   = lit("earth.golden.pictor");
    e.res     = xar_creare(piscina, magnitudo(chorda));
    per (i = ZEPHYRUM; ids && ids[i]; i++)
    {
        *(chorda*)xar_addere(e.res) = lit(ids[i]);
    }
    e.mora_s  = CCC;
    e.unus    = lit("{persona} shared a picture");
    e.plures  = lit("{persona} shared {numerus} pictures");
    e.statim  = statim;
    redde e;
}

interior b32
nuntiare (
                Mundus* m,
                   i32  apparatus,
    constans character* id,
                   b32  statim)
{
    constans character* ids[II];
         PulsusEventus  e;

    ids[ZEPHYRUM]  = id;
    ids[I]         = NIHIL;
    e              = eventus(id ? ids : NIHIL, statim);
    redde pulsus_eventum_addere(m->pulsus, app(m, apparatus), &e);
}

interior Missa*
missa (
    Mundus* m,
       i32  i)
{
    redde (Missa*)xar_obtinere(m->missae, i);
}

/* missae ad signa data (ordine quovis) */
interior b32
ad_signa (
                Mundus* m,
    constans character* a,
    constans character* b)
{
    si (xar_numerus(m->missae) != II)
    {
        redde FALSUM;
    }
    redde (   chorda_aequalis_literis(missa(m, ZEPHYRUM)->signum, a)
           && chorda_aequalis_literis(missa(m, I)->signum, b))
        || (   chorda_aequalis_literis(missa(m, ZEPHYRUM)->signum, b)
            && chorda_aequalis_literis(missa(m, I)->signum, a));
}

interior vacuum
_sectio_quies (
    CredoContextus* c)
{
          Mundus* m;
    PulsusTactus  t;

    m = (Mundus*)c->datum;
    CREDO_VERUM(nuntiare(m, F_TELEPHONUM, "R:a", FALSUM));
    hora_ficta += (s64)CXX * M;
    CREDO_VERUM(nuntiare(m, F_TELEPHONUM, "R:b", FALSUM));
    hora_ficta += (s64)CXX * M;
    CREDO_VERUM(nuntiare(m, F_MAC, "R:c", FALSUM));
    CREDO_VERUM(nuntiare(m, F_TELEPHONUM, "R:a", FALSUM));
    hora_ficta += (s64)CCXCIX * M;
    CREDO_AEQUALIS_S32((s32)pulsus_maturos_mittere(m->pulsus),
        ZEPHYRUM);
    CREDO_AEQUALIS_I32(xar_numerus(m->missae), ZEPHYRUM);
    hora_ficta += (s64)M;
    CREDO_AEQUALIS_I32(pulsus_maturos_mittere(m->pulsus), I);
    CREDO_VERUM(ad_signa(m, "a1", "a2"));
    CREDO_CHORDA_CONTINET(missa(m, ZEPHYRUM)->corpus,
        lit("\"body\":\"Fran shared 3 pictures\""));
    CREDO_CHORDA_CONTINET(missa(m, ZEPHYRUM)->corpus,
        lit("\"title\":\"pictor\""));
    CREDO_VERUM(pulsus_tactum_legere(missa(m, ZEPHYRUM)->corpus,
        piscina,
        &t));
    CREDO_AEQUALIS_I32(xar_numerus(t.res), III);
    CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(t.res,
        ZEPHYRUM),
        "R:a");
    CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(t.res, II),
        "R:c");
    CREDO_CHORDA_AEQUALIS(t.a, m->fran);
    CREDO_AEQUALIS_S64(pulsus_proximum_ms(m->pulsus), (s64)-I);
    CREDO_AEQUALIS_I32(pulsus_maturos_mittere(m->pulsus), ZEPHYRUM);
}

interior vacuum
_sectio_statim (
    CredoContextus* c)
{
    Mundus* m;

    m = (Mundus*)c->datum;
    CREDO_VERUM(nuntiare(m, F_TELEPHONUM, "R:a", VERUM));
    CREDO_VERUM(ad_signa(m, "a1", "a2"));
    CREDO_CHORDA_CONTINET(missa(m, ZEPHYRUM)->corpus,
        lit("Fran shared a picture"));
    CREDO_AEQUALIS_S64(pulsus_proximum_ms(m->pulsus), (s64)-I);

    /* statim sine rebus et sine grege: nihil */
    CREDO_VERUM(nuntiare(m, F_TELEPHONUM, NIHIL, VERUM));
    CREDO_AEQUALIS_I32(xar_numerus(m->missae), II);

    /* statim sine rebus cum grege pendente: grex mittitur */
    CREDO_VERUM(nuntiare(m, F_TELEPHONUM, "R:b", FALSUM));
    CREDO_VERUM(nuntiare(m, F_MAC, NIHIL, VERUM));
    CREDO_AEQUALIS_I32(xar_numerus(m->missae), IV);
}

interior vacuum
_sectio_per_mittentem (
    CredoContextus* c)
{
    Mundus* m;
       i32  i;
       i32  ad_mittentem;

    m = (Mundus*)c->datum;
    CREDO_VERUM(nuntiare(m, F_TELEPHONUM, "R:a", FALSUM));
    CREDO_VERUM(nuntiare(m, A_TELEPHONUM, "S:a", FALSUM));
    CREDO_VERUM(nuntiare(m, F_MAC, "R:b", FALSUM));
    hora_ficta += (s64)CCC * M;
    CREDO_AEQUALIS_I32(pulsus_maturos_mittere(m->pulsus), II);
    CREDO_AEQUALIS_I32(xar_numerus(m->missae), IV);
    ad_mittentem = ZEPHYRUM;
    per (i = ZEPHYRUM; i < IV; i++)
    {
        si (chorda_incipit(missa(m, i)->signum, lit("f")))
        {
            ad_mittentem++;
            CREDO_CHORDA_CONTINET(missa(m, i)->corpus,
                lit("Amanda shared a picture"));
        }
        alioquin
        {
            CREDO_CHORDA_CONTINET(missa(m, i)->corpus,
                lit("Fran shared 2 pictures"));
        }
    }
    CREDO_AEQUALIS_I32(ad_mittentem, II);
}

interior vacuum
_sectio_per_clavem (
    CredoContextus* c)
{
           Mundus* m;
    PulsusEventus  e;

    m = (Mundus*)c->datum;
    CREDO_VERUM(nuntiare(m, F_TELEPHONUM, "R:a", FALSUM));
    e                            = eventus(NIHIL, FALSUM);
    e.clavis                     = lit("alia.res");
    *(chorda*)xar_addere(e.res)  = lit("Q:1");
    CREDO_VERUM(pulsus_eventum_addere(m->pulsus, app(m, F_TELEPHONUM),
        &e));
    hora_ficta += (s64)CCC * M;
    CREDO_AEQUALIS_I32(pulsus_maturos_mittere(m->pulsus), II);
    CREDO_AEQUALIS_I32(xar_numerus(m->missae), IV);
}

interior vacuum
_sectio_sine_fine (
    CredoContextus* c)
{
       Mundus* m;
          i32  i;
    character  id[XVI];

    m = (Mundus*)c->datum;
    per (i = ZEPHYRUM; i < XVIII; i++)
    {
        sprintf(id, "R:%u", (unsigned)i);
        CREDO_VERUM(nuntiare(m, F_TELEPHONUM, id, FALSUM));
        hora_ficta += (s64)CC * M;
        CREDO_AEQUALIS_I32(pulsus_maturos_mittere(m->pulsus), ZEPHYRUM);
    }
    hora_ficta += (s64)C * M;
    CREDO_AEQUALIS_I32(pulsus_maturos_mittere(m->pulsus), I);
    CREDO_CHORDA_CONTINET(missa(m, ZEPHYRUM)->corpus,
        lit("Fran shared 18 pictures"));
}

interior vacuum
_sectio_restitutio (
    CredoContextus* c)
{
     Mundus* m;
     Pulsus* secundus;
     Pulsus* tertius;
        s64  debitum;

    m = (Mundus*)c->datum;
    CREDO_VERUM(nuntiare(m, F_TELEPHONUM, "R:a", FALSUM));
    debitum = hora_ficta + (s64)CCC * M;
    CREDO_AEQUALIS_S64(pulsus_proximum_ms(m->pulsus), debitum);
    pulsus_destruere(m->pulsus);
    m->pulsus = NIHIL;
    secundus = pulsus_creare(piscina, m->volumen, m->servus, m->cives,
        _hora);
    CREDO_NECESSE(secundus != NIHIL);
    pulsus_mittentem_ponere(secundus, _mittere, m);
    CREDO_AEQUALIS_S64(pulsus_proximum_ms(secundus), debitum);
    hora_ficta = debitum + (s64)M * M;
    CREDO_AEQUALIS_I32(pulsus_maturos_mittere(secundus), I);
    CREDO_VERUM(ad_signa(m, "a1", "a2"));
    pulsus_destruere(secundus);
    tertius = pulsus_creare(piscina, m->volumen, m->servus, m->cives,
        _hora);
    CREDO_NECESSE(tertius != NIHIL);
    pulsus_mittentem_ponere(tertius, _mittere, m);
    CREDO_AEQUALIS_I32(pulsus_maturos_mittere(tertius), ZEPHYRUM);
    CREDO_AEQUALIS_I32(xar_numerus(m->missae), II);
    m->pulsus = tertius;
}

interior vacuum
_sectio_inscriptio_renovata (
    CredoContextus* c)
{
    Mundus* m;
       Xar* l;
       i32  i;

    m = (Mundus*)c->datum;
    CREDO_VERUM(pulsus_inscribere(m->pulsus, app(m, A_TELEPHONUM),
        lit("a9a9a9a9a9a9"), lit("productio"),
        lit("earth.golden.pictor")));
    CREDO_VERUM(nuntiare(m, F_TELEPHONUM, "R:a", VERUM));
    CREDO_VERUM(ad_signa(m, "a9a9a9a9a9a9", "a2"));
    l = pulsus_inscriptiones_enumerare(m->pulsus, piscina);
    CREDO_NECESSE(l != NIHIL);
    per (i = ZEPHYRUM; i < xar_numerus(l); i++)
    {
        PulsusInscriptio* r = (PulsusInscriptio*)xar_obtinere(l, i);

        CREDO_FALSUM(chorda_aequalis_literis(r->tessera, "a1"));
        si (chorda_aequalis_literis(r->apparatus,
            m->apparatus[A_TELEPHONUM]))
        {
            CREDO_CHORDA_AEQUALIS_LITERIS(r->tessera, "a9a9a9a9");
            CREDO_CHORDA_AEQUALIS_LITERIS(r->ambitus, "productio");
        }
    }
    /* Amanda nuntiat: ad Fran duo, numquam ad Amandam */
    CREDO_VERUM(nuntiare(m, A_TABULA, "S:a", VERUM));
    CREDO_AEQUALIS_I32(xar_numerus(m->missae), IV);
    CREDO_VERUM(chorda_incipit(missa(m, II)->signum, lit("f")));
    CREDO_VERUM(chorda_incipit(missa(m, III)->signum, lit("f")));
}

interior vacuum
_sectio_mortuum (
    CredoContextus* c)
{
    Mundus* m;

    m           = (Mundus*)c->datum;
    m->mortuum  = lit("a1");
    CREDO_VERUM(nuntiare(m, F_TELEPHONUM, "R:a", VERUM));
    CREDO_VERUM(ad_signa(m, "a1", "a2"));
    m->mortuum = lit("");
    CREDO_VERUM(nuntiare(m, F_TELEPHONUM, "R:b", VERUM));
    CREDO_AEQUALIS_I32(xar_numerus(m->missae), III);
    CREDO_CHORDA_AEQUALIS_LITERIS(missa(m, II)->signum, "a2");
}

interior vacuum
_sectio_revocatus (
    CredoContextus* c)
{
    Mundus* m;
       Xar* l;
       i32  i;

    m = (Mundus*)c->datum;
    CREDO_VERUM(nuntiare(m, F_TELEPHONUM, "R:a", VERUM));
    per (i = ZEPHYRUM; i < xar_numerus(m->missae); i++)
    {
        CREDO_FALSUM(chorda_aequalis_literis(missa(m, i)->signum,
            "a3"));
        CREDO_FALSUM(chorda_aequalis_literis(missa(m, i)->signum,
            "b1"));
    }
    l = pulsus_inscriptiones_enumerare(m->pulsus, piscina);
    CREDO_NECESSE(l != NIHIL);
    CREDO_AEQUALIS_I32(xar_numerus(l), V);
    per (i = ZEPHYRUM; i < xar_numerus(l); i++)
    {
        CREDO_FALSUM(chorda_aequalis_literis(
            ((PulsusInscriptio*)xar_obtinere(l, i))->apparatus,
            m->apparatus[A_VETUS]));
    }
    /* apparatus revocatus nuntiare non potest */
    CREDO_FALSUM(nuntiare(m, A_VETUS, "S:a", VERUM));
}

interior vacuum
_sectio_probare (
    CredoContextus* c)
{
    Mundus* m;

    m = (Mundus*)c->datum;
    CREDO_VERUM(pulsus_probare(m->pulsus, m->amanda));
    CREDO_VERUM(ad_signa(m, "a1", "a2"));
    CREDO_CHORDA_CONTINET(missa(m, ZEPHYRUM)->corpus,
        lit("\"clavis\":\"pulsus.probatio\""));
    CREDO_FALSUM(pulsus_probare(m->pulsus, lit("IGNOTUS")));
}


/* ==================================================
 * T3: viae per HTTP (hospitium et http_vocare in reactore uno)
 * ================================================== */

#define VIA_INSCRIPTIONIS "/communio/pulsus/inscriptio"
#define VIA_EVENTUS       "/communio/pulsus/eventus"
#define INSCRIPTIO_NOVA \
    "{\"tessera\":\"abcdef0123\",\"ambitus\":\"productio\"," \
    "\"thema\":\"earth.golden.pictor\"}"
#define EVENTUS_MORA_I \
    "{\"clavis\":\"pictor.picturae\",\"thema\":\"earth.golden.pictor\"," \
    "\"res\":[\"R:a\"],\"mora\":1," \
    "\"unus\":\"{persona} shared a picture\"," \
    "\"plures\":\"{persona} shared {numerus} pictures\"}"

nomen structura {
      Reactor* reactor;
    Hospitium* hospitium;
          i32  portus;
} Rete;

nomen structura {
       i32 status;
    chorda corpus;
} Responsum;

interior s64
_nunc_verum (vacuum)
{
    structura timeval tv;

    gettimeofday(&tv, NIHIL);
    redde (s64)tv.tv_sec * (s64)M + (s64)(tv.tv_usec / M);
}

interior vacuum
_vocatio_finita (
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
    http_petitio_caput_addere(pet, "Content-Type", "application/json");
    http_petitio_corpus_ponere_chorda(pet, corpus);
    finita = FALSUM;
    voc = http_vocare(r->reactor, pet, NIHIL, NIHIL, _vocatio_finita,
        &finita, NIHIL, piscina);
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

/* hospitium cum corpore usque ad IV MB in memoria (II MB ad pulsum
 * pervenit, non ad limitem hospitii) */
interior Rete*
rete_parare (
    Mundus* m)
{
                     Rete* r;
    HospitiumConfiguratio  cfg;

    r = (Rete*)piscina_allocare(piscina, magnitudo(Rete));
    memset(r, ZEPHYRUM, magnitudo(Rete));
    r->reactor = reactor_creare(piscina);
    memset(&cfg, ZEPHYRUM, magnitudo(cfg));
    cfg.reactor = r->reactor;
    cfg.petitio_maxima = (i32)IV * MXXIV * MXXIV;
    cfg.corpus_in_memoria_maximum = (i32)IV * MXXIV * MXXIV;
    r->hospitium = r->reactor ? hospitium_creare(piscina, &cfg) : NIHIL;
    si (   !r->hospitium
        || !communio_servum_montare(m->servus, r->hospitium)
        || !pulsus_montare(m->pulsus, r->hospitium))
    {
        redde NIHIL;
    }
    r->portus = (i32)hospitium_portus(r->hospitium);
    redde r;
}

/* pulsus ANTE reactorem (hora eius in reactore) */
interior vacuum
rete_purgare (
    Mundus* m,
      Rete* r)
{
    si (m->pulsus)
    {
        pulsus_destruere(m->pulsus);
        m->pulsus = NIHIL;
    }
    si (r && r->hospitium)
    {
        hospitium_destruere(r->hospitium);
    }
    si (r && r->reactor)
    {
        reactor_destruere(r->reactor);
    }
}

interior s64
ordines (
                Mundus* m,
    constans character* sql)
{
    ScriniumEnuntiatum* e;
                   s64  n;

    n = (s64)-I;
    e = scrinium_praeparare(volumen_scrinium(m->volumen), sql);
    si (e)
    {
        si (scrinium_gradi(e) == SCRINIUM_ORDO)
        {
            n = scrinium_columna_numerus(e, ZEPHYRUM);
        }
        scrinium_finire(e);
    }
    redde n;
}

interior vacuum
_sectio_viae (
    CredoContextus* c)
{
       Mundus* m;
         Rete* r;
    Responsum  q;
          Xar* l;
          i32  i;
          s64  initium;
          b32  inventa;

    m = (Mundus*)c->datum;
    r = rete_parare(m);
    CREDO_NECESSE(r != NIHIL);

    CREDO_GRADUS("sine tessera aut tessera falsa: 401");
    q = rogare(r, HTTP_PUT, VIA_INSCRIPTIONIS, NIHIL,
        lit(INSCRIPTIO_NOVA));
    CREDO_AEQUALIS_I32(q.status, CDI);
    q = rogare(r, HTTP_POST, VIA_EVENTUS, "nonsensus",
        lit(EVENTUS_MORA_I));
    CREDO_AEQUALIS_I32(q.status, CDI);
    CREDO_AEQUALIS_S64(ordines(m,
        "SELECT COUNT(*) FROM pulsus_pendentes"), (s64)ZEPHYRUM);

    CREDO_GRADUS("inscriptio: 204, ordo substitutus");
    q = rogare(r, HTTP_PUT, VIA_INSCRIPTIONIS,
        m->tesserae[A_TELEPHONUM], lit(INSCRIPTIO_NOVA));
    CREDO_AEQUALIS_I32(q.status, CCIV);
    l        = pulsus_inscriptiones_enumerare(m->pulsus, piscina);
    inventa  = FALSUM;
    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        PulsusInscriptio* x = (PulsusInscriptio*)xar_obtinere(l, i);

        si (chorda_aequalis_literis(x->apparatus,
            m->apparatus[A_TELEPHONUM]))
        {
            inventa = VERUM;
            CREDO_CHORDA_AEQUALIS_LITERIS(x->tessera, "abcdef01");
            CREDO_CHORDA_AEQUALIS_LITERIS(x->ambitus, "productio");
        }
    }
    CREDO_VERUM(inventa);

    CREDO_GRADUS("eventus: 202, grex post horam reactoris");
    q = rogare(r, HTTP_POST, VIA_EVENTUS, m->tesserae[F_TELEPHONUM],
        lit(EVENTUS_MORA_I));
    CREDO_AEQUALIS_I32(q.status, CCII);
    initium = _nunc_verum();
    CREDO_AEQUALIS_I32(xar_numerus(m->missae), ZEPHYRUM);
    dum (   xar_numerus(m->missae) == ZEPHYRUM
         && _nunc_verum() - initium < (s64)MMM)
    {
        (vacuum)reactor_poll(r->reactor, X);
    }
    CREDO_VERUM(ad_signa(m, "abcdef0123", "a2"));
    CREDO_MAIOR_S64(_nunc_verum() - initium, (s64)DCCC);
    CREDO_AEQUALIS_S64(ordines(m,
        "SELECT COUNT(*) FROM pulsus_pendentes"), (s64)ZEPHYRUM);
    rete_purgare(m, r);
}

/* corpus eventus cum forma 'unus' data (JSON iam effugata) */
interior chorda
eventus_cum_forma (
    constans character* unus)
{
    character* s;

    s = (character*)piscina_allocare(piscina,
        (memoriae_index)strlen(unus) + CCLVI);
    sprintf(s, "{\"clavis\":\"pictor.picturae\","
        "\"thema\":\"earth.golden.pictor\",\"res\":[\"R:a\"],"
        "\"unus\":\"%s\",\"plures\":\"{numerus}\"}", unus);
    redde lit(s);
}

/* Review Focus 5: petitiones malae - 400 singulae, nihil servatur,
 * memoria servi non crescit per M petitiones */
interior vacuum
_sectio_bombae (
    CredoContextus* c)
{
             Mundus* m;
               Rete* r;
          character* longa;
          character* ids;
          character* magnum;
             chorda  malae[III];
     memoriae_index  usus_pulsus;
     memoriae_index  usus_servi;
                s64  inscriptiones;
                i32  i;
                i32  n;
                i32  bonae;

    m = (Mundus*)c->datum;
    r = rete_parare(m);
    CREDO_NECESSE(r != NIHIL);
    inscriptiones = ordines(m,
        "SELECT COUNT(*) FROM pulsus_inscriptiones");
    CREDO_AEQUALIS_S64(inscriptiones, (s64)VI);

    /* forma CCC octetorum */
    longa = (character*)piscina_allocare(piscina, CCCI);
    memset(longa, 'x', CCC);
    longa[CCC]       = '\0';
    malae[ZEPHYRUM]  = eventus_cum_forma(longa);
    /* signum moderans in forma */
    malae[I]        = eventus_cum_forma("{persona} \\u0007 shared");
    /* tessera non hex */
    malae[II]       = lit("{\"tessera\":\"zz\",\"ambitus\":\"sandbox\","
        "\"thema\":\"earth.golden.pictor\"}");

    CREDO_GRADUS("forma longa, signum moderans, tessera zz: 400");
    CREDO_AEQUALIS_I32(rogare(r, HTTP_POST, VIA_EVENTUS,
        m->tesserae[F_TELEPHONUM], malae[ZEPHYRUM]).status, CD);
    CREDO_AEQUALIS_I32(rogare(r, HTTP_POST, VIA_EVENTUS,
        m->tesserae[F_TELEPHONUM], malae[I]).status, CD);
    CREDO_AEQUALIS_I32(rogare(r, HTTP_PUT, VIA_INSCRIPTIONIS,
        m->tesserae[F_TELEPHONUM], malae[II]).status, CD);

    CREDO_GRADUS("X M res: 400");
    ids = (character*)piscina_allocare(piscina, (memoriae_index)C * M
        + CCLVI);
    n = (i32)sprintf(ids, "{\"clavis\":\"pictor.picturae\","
        "\"thema\":\"earth.golden.pictor\",\"unus\":\"a\","
        "\"plures\":\"b\",\"res\":[");
    per (i = ZEPHYRUM; i < X * M; i++)
    {
        n += (i32)sprintf(ids + n, "%s\"R:%u\"", i ? "," : "",
            (unsigned)i);
    }
    (vacuum)sprintf(ids + n, "]}");
    CREDO_AEQUALIS_I32(rogare(r, HTTP_POST, VIA_EVENTUS,
        m->tesserae[F_TELEPHONUM], lit(ids)).status, CD);

    CREDO_GRADUS("corpus II MB: 400");
    magnum = (character*)piscina_allocare(piscina,
        (memoriae_index)II * MXXIV * MXXIV + LXIV);
    n = (i32)sprintf(magnum,
        "{\"clavis\":\"pictor.picturae\",\"x\":\"");
    memset(magnum + n, 'a', (size_t)II * MXXIV * MXXIV);
    n += (i32)II * MXXIV * MXXIV;
    (vacuum)sprintf(magnum + n, "\"}");
    CREDO_AEQUALIS_I32(rogare(r, HTTP_POST, VIA_EVENTUS,
        m->tesserae[F_TELEPHONUM], lit(magnum)).status, CD);
    CREDO_AEQUALIS_I32(rogare(r, HTTP_PUT, VIA_INSCRIPTIONIS,
        m->tesserae[F_TELEPHONUM], lit(magnum)).status, CD);

    CREDO_GRADUS("JSON fractum et vacuum: 400");
    CREDO_AEQUALIS_I32(rogare(r, HTTP_POST, VIA_EVENTUS,
        m->tesserae[F_TELEPHONUM], lit("{")).status, CD);
    CREDO_AEQUALIS_I32(rogare(r, HTTP_PUT, VIA_INSCRIPTIONIS,
        m->tesserae[F_TELEPHONUM], lit("")).status, CD);

    CREDO_GRADUS("tabulae immutatae");
    CREDO_AEQUALIS_S64(ordines(m,
        "SELECT COUNT(*) FROM pulsus_pendentes"), (s64)ZEPHYRUM);
    CREDO_AEQUALIS_S64(ordines(m,
        "SELECT COUNT(*) FROM pulsus_inscriptiones"), inscriptiones);
    CREDO_AEQUALIS_S64(ordines(m,
        "SELECT COUNT(*) FROM pulsus_inscriptiones WHERE tessera = 'zz'"),
        (s64)ZEPHYRUM);

    CREDO_GRADUS("M petitiones malae: memoria stabilis");
    usus_pulsus  = pulsus_usus(m->pulsus);
    usus_servi   = communio_servi_usus(m->servus);
    bonae        = ZEPHYRUM;
    per (i = ZEPHYRUM; i < M; i++)
    {
        si (rogare(r, i % III == II ? HTTP_PUT : HTTP_POST,
                i % III == II ? VIA_INSCRIPTIONIS : VIA_EVENTUS,
                m->tesserae[F_TELEPHONUM], malae[i % III]).status == CD)
        {
            bonae++;
        }
    }
    CREDO_AEQUALIS_I32(bonae, M);
    CREDO_AEQUALIS_I32((i32)pulsus_usus(m->pulsus), (i32)usus_pulsus);
    CREDO_AEQUALIS_I32((i32)communio_servi_usus(m->servus),
        (i32)usus_servi);
    CREDO_AEQUALIS_S64(ordines(m,
        "SELECT COUNT(*) FROM pulsus_pendentes"), (s64)ZEPHYRUM);
    CREDO_AEQUALIS_I32(xar_numerus(m->missae), ZEPHYRUM);
    rete_purgare(m, r);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "quies", _sectio_quies, _parare, _purgare, NIHIL, NIHIL },
    { "statim", _sectio_statim, _parare, _purgare, NIHIL, NIHIL },
    { "per mittentem", _sectio_per_mittentem, _parare, _purgare, NIHIL,
        NIHIL },
    { "per clavem", _sectio_per_clavem, _parare, _purgare, NIHIL,
        NIHIL },
    { "sine fine", _sectio_sine_fine, _parare, _purgare, NIHIL, NIHIL },
    { "restitutio", _sectio_restitutio, _parare, _purgare, NIHIL,
        NIHIL },
    { "inscriptio renovata", _sectio_inscriptio_renovata, _parare,
        _purgare, NIHIL, NIHIL },
    { "mortuum", _sectio_mortuum, _parare, _purgare, NIHIL, NIHIL },
    { "revocatus", _sectio_revocatus, _parare, _purgare, NIHIL, NIHIL },
    { "probare", _sectio_probare, _parare, _purgare, NIHIL, NIHIL },
    { "viae", _sectio_viae, _parare_verum, _purgare, NIHIL, NIHIL },
    { "bombae", _sectio_bombae, _parare_verum, _purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("pulsus", SECTIONES);
}

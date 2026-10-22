/* probatio_horreum_cliens.c - stratum obstans horrei (horreum-plan T3)
 * contra S3 fictum (horreum_simulacrum.h): iter rotundum, signatura a
 * servo iudicata, iterationes cum mora et Retry-After, '412 post
 * iterationem' (iam_nostrum contra conflictum verum), horologium
 * correctum, deletio plurium partialis, paginae enumerationis, corpus
 * nimium. Horologium et somnus ficti: nihil dormit, morae notantur. */
#include "postulata_posix.h"
#include "latina.h"
#include "horreum.h"
#include "horreum_cliens.h"
#include "horreum_simulacrum.h"
#include "chorda.h"
#include "piscina.h"
#include "credo.h"
#include "filum.h"
#include "iter_directoria.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define SECRETUM        "SECRETUM-SIMULACRI-0123456789"
#define SECRETUM_MALUM  "SECRETUM-ALIENUM-9876543210"
#define CLAVIS_ID       "AKIDSIMULACRI"
#define TEMPUS          SIMULACRUM_TEMPUS
#define ETAG_SALVE      "d21a891233c1e4d010c92d60592138fd"
#define ETAG_VACUI      "d41d8cd98f00b204e9800998ecf8427e"
#define CLAVIS_MIRA     "a&b<c>\"d'"
#define CLAVES_MULTAE   MMD

nomen structura {
    MomentumSec tempus;
            i32 morae[XVI];
            i32 numerus;
} Horologium;

interior chorda
_l (
    constans character* l,
               Piscina* p)
{
    redde chorda_ex_literis(l, p);
}

interior MomentumSec
_nunc (
    vacuum* datum)
{
    redde ((Horologium*)datum)->tempus;
}

interior vacuum
_dormire (
    vacuum* datum,
       i32  ms)
{
    Horologium* horologium = (Horologium*)datum;

    si (horologium->numerus < XVI)
    {
        horologium->morae[horologium->numerus] = ms;
        horologium->numerus++;
    }
}

interior Simulacrum*
_simulacrum (
    Piscina* p)
{
    redde simulacrum_creare(_l(CLAVIS_ID, p), _l(SECRETUM, p),
        _l("us-east-1", p), p);
}

interior Horreum*
_horreum_cum_vectura (
            HttpVectura  vectura,
             Horologium* horologium,
     constans character* secretum,
                Piscina* p)
{
        HorreumConfiguratio  c;
               HorreumError  e;
    HorreumClientisOptiones  o;
                    Horreum* h;

    memset(&c, ZEPHYRUM, magnitudo(c));
    c.provisor   = "generic";
    c.terminus   = _l("http://simulacrum.invalid", p);
    c.regio      = _l("us-east-1", p);
    c.situla     = _l(SIMULACRUM_SITULA, p);
    c.clavis_id  = _l(CLAVIS_ID, p);
    c.secretum   = _l(secretum, p);
    h            = horreum_creare(&c, &e, p);
    memset(&o, ZEPHYRUM, magnitudo(o));
    o.vectura  = vectura;
    o.nunc     = _nunc;
    o.dormire  = _dormire;
    o.datum    = horologium;
    o.semen    = VII;
    horreum_cliens_parare(h, &o, p);
    redde h;
}

interior Horreum*
_horreum (
            Simulacrum* s,
            Horologium* horologium,
    constans character* secretum,
               Piscina* p)
{
    memset(horologium, ZEPHYRUM, magnitudo(*horologium));
    horologium->tempus = TEMPUS;
    redde _horreum_cum_vectura(simulacrum_vectura(s), horologium,
        secretum, p);
}

interior SimulacrumVitium
_vitium (
    SimulacrumVitiumGenus  genus,
             HttpMethodus  methodus,
       constans character* clavis,
                      i32  status,
       constans character* codex,
                      s32  secundae,
                      i32  numerus)
{
    SimulacrumVitium v;

    v.genus       = genus;
    v.methodus    = methodus;
    v.clavis      = clavis;
    v.parametrum  = NIHIL;
    v.status      = status;
    v.codex       = codex;
    v.secundae    = secundae;
    v.numerus     = numerus;
    redde v;
}

interior b32
_tacitus (
    constans HorreumError* e,
                  Piscina* p)
{
    chorda bonum = _l(SECRETUM, p);
    chorda malum = _l(SECRETUM_MALUM, p);

    redde !chorda_continet(e->codex, bonum)
        && !chorda_continet(e->codex, malum)
        && !chorda_continet(e->nuntius, bonum)
        && !chorda_continet(e->nuntius, malum)
        && !chorda_continet(e->petitio_id, bonum)
        && !chorda_continet(e->petitio_id, malum);
}

/* ===== iter ===== */
interior vacuum
_sectio_iter (
    CredoContextus* c)
{
            Piscina* p = c->piscina;
         Horologium  horologium;
         Simulacrum* s       = _simulacrum(p);
            Horreum* h       = _horreum(s, &horologium, SECRETUM, p);
             chorda  situla  = _l(SIMULACRUM_SITULA, p);
     HorreumPonendi  ponendi;
        HorreumMeta  meta[I];
    HorreumObjectum  ob;
       HorreumError  e;
    HorreumCapiendi  ca;
      HorreumPagina  pg;
             chorda  corpus;

    memset(&ponendi, ZEPHYRUM, magnitudo(ponendi));
    meta[ZEPHYRUM].titulus  = _l("auctor", p);
    meta[ZEPHYRUM].valor    = _l("fran", p);
    ponendi.meta            = meta;
    ponendi.meta_numerus    = I;
    ponendi.genus_contenti  = _l("text/plain", p);
    CREDO_VERUM(horreum_ponere(h, situla, _l("a/b.txt", p), _l("salve",
        p), &ponendi, &ob, &e, p));
    CREDO_CHORDA_AEQUALIS_LITERIS(ob.etag, ETAG_SALVE);

    CREDO_VERUM(horreum_inspicere(h, situla, _l("a/b.txt", p), &ob, &e,
        p));
    CREDO_AEQUALIS_S64((s64)ob.mensura, (s64)V);
    CREDO_CHORDA_AEQUALIS_LITERIS(ob.etag, ETAG_SALVE);
    CREDO_AEQUALIS_S64((s64)ob.mutatum, (s64)TEMPUS);
    CREDO_CHORDA_AEQUALIS_LITERIS(ob.genus_contenti, "text/plain");
    CREDO_AEQUALIS_I32(ob.meta_numerus, I);
    si (ob.meta_numerus == I)
    {
        CREDO_CHORDA_AEQUALIS_LITERIS(ob.meta[ZEPHYRUM].titulus,
            "auctor");
        CREDO_CHORDA_AEQUALIS_LITERIS(ob.meta[ZEPHYRUM].valor, "fran");
    }

    CREDO_VERUM(horreum_capere(h, situla, _l("a/b.txt", p), NIHIL,
        &corpus, &ob, &e, p));
    CREDO_CHORDA_AEQUALIS_LITERIS(corpus, "salve");

    memset(&ca, ZEPHYRUM, magnitudo(ca));
    ca.frustum  = VERUM;
    ca.ab       = I;
    ca.ad       = III;
    CREDO_VERUM(horreum_capere(h, situla, _l("a/b.txt", p), &ca,
        &corpus,
        &ob, &e, p));
    CREDO_CHORDA_AEQUALIS_LITERIS(corpus, "alv");

    CREDO_VERUM(horreum_copiare(h, situla, _l("a/b.txt", p), situla,
        _l("c.txt", p), NIHIL, &ob, &e, p));
    CREDO_CHORDA_AEQUALIS_LITERIS(ob.etag, ETAG_SALVE);
    CREDO_AEQUALIS_S64((s64)ob.mutatum, (s64)TEMPUS);
    CREDO_VERUM(horreum_capere(h, situla, _l("c.txt", p), NIHIL,
        &corpus,
        &ob, &e, p));
    CREDO_CHORDA_AEQUALIS_LITERIS(corpus, "salve");
    CREDO_AEQUALIS_I32(ob.meta_numerus, I);

    CREDO_VERUM(horreum_enumerare(h, situla, NIHIL, &pg, &e, p));
    CREDO_AEQUALIS_I32(pg.obiecta_numerus, II);
    CREDO_FALSUM(pg.truncata);
    si (pg.obiecta_numerus == II)
    {
        CREDO_CHORDA_AEQUALIS_LITERIS(pg.obiecta[ZEPHYRUM].clavis,
            "a/b.txt");
        CREDO_CHORDA_AEQUALIS_LITERIS(pg.obiecta[I].clavis, "c.txt");
        CREDO_AEQUALIS_S64((s64)pg.obiecta[I].mensura, (s64)V);
        CREDO_CHORDA_AEQUALIS_LITERIS(pg.obiecta[I].etag, ETAG_SALVE);
    }

    CREDO_VERUM(horreum_delere(h, situla, _l("a/b.txt", p), &e, p));
    CREDO_FALSUM(horreum_inspicere(h, situla, _l("a/b.txt", p), &ob, &e,
        p));
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)HORREUM_ABEST);

    /* review focus 2: obiectum vacuum et signum directorii */
    CREDO_VERUM(horreum_ponere(h, situla, _l("vacuum/", p), _l("", p),
        NIHIL, &ob, &e, p));
    CREDO_CHORDA_AEQUALIS_LITERIS(ob.etag, ETAG_VACUI);
    CREDO_VERUM(horreum_inspicere(h, situla, _l("vacuum/", p), &ob, &e,
        p));
    CREDO_AEQUALIS_S64((s64)ob.mensura, (s64)ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(ob.etag, ETAG_VACUI);
    CREDO_VERUM(horreum_capere(h, situla, _l("vacuum/", p), NIHIL,
        &corpus, &ob, &e, p));
    CREDO_AEQUALIS_I32(corpus.mensura, ZEPHYRUM);
    CREDO_AEQUALIS_I32(horologium.numerus, ZEPHYRUM);
}

/* ===== signatura a servo iudicata ===== */
interior vacuum
_sectio_signatura_verificata (
    CredoContextus* c)
{
         Piscina* p = c->piscina;
      Horologium  horologium;
      Horologium  horologium_alienum;
      Simulacrum* s = _simulacrum(p);
         Horreum* h = _horreum(s, &horologium, SECRETUM, p);
         Horreum* malum = _horreum(s, &horologium_alienum,
             SECRETUM_MALUM, p);
          chorda situla = _l(SIMULACRUM_SITULA, p);
    HorreumError e;

    CREDO_FALSUM(horreum_ponere(malum, situla, _l("x.txt", p), _l("x",
        p), NIHIL, NIHIL, &e, p));
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)HORREUM_VETITUM);
    CREDO_AEQUALIS_I32(e.status, CDIII);
    CREDO_CHORDA_AEQUALIS_LITERIS(e.codex, "SignatureDoesNotMatch");
    CREDO_CHORDA_NON_VACUA(e.petitio_id);
    CREDO_VERUM(_tacitus(&e, p));
    CREDO_AEQUALIS_I32(simulacrum_petitiones(s), I);
    CREDO_AEQUALIS_I32(horologium_alienum.numerus, ZEPHYRUM);

    CREDO_VERUM(horreum_ponere(h, situla, _l("x.txt", p), _l("x", p),
        NIHIL, NIHIL, &e, p));
    CREDO_AEQUALIS_I32(simulacrum_petitiones(s), II);
}

/* ===== transitoria ===== */
interior vacuum
_sectio_transitorium (
    CredoContextus* c)
{
          Piscina* p = c->piscina;
       Horologium  horologium;
       Simulacrum* s       = _simulacrum(p);
          Horreum* h       = _horreum(s, &horologium, SECRETUM, p);
           chorda  situla  = _l(SIMULACRUM_SITULA, p);
      HorreumOpus  o;
    HorreumExitus  x;
     HorreumError  e;
              i32  n0;

    simulacrum_vitium_addere(s, _vitium(SIMULACRUM_VITIUM_STATUS,
        HTTP_PUT,
        NIHIL, DIII, "SlowDown", ZEPHYRUM, II));
    CREDO_VERUM(horreum_opus_ponere(h, situla, _l("t.txt", p), _l("t",
        p), NIHIL, &o, &e, p));
    CREDO_VERUM(horreum_exsequi(h, &o, &x, NIHIL, &e, p));
    CREDO_VERUM(x.successus);
    CREDO_AEQUALIS_I32(x.conatus, III);
    CREDO_AEQUALIS_I32(horologium.numerus, II);
    CREDO_MAIOR_AUT_AEQUALIS_I32(horologium.morae[ZEPHYRUM], CC);
    CREDO_MINOR_I32(horologium.morae[ZEPHYRUM], CD);
    CREDO_MAIOR_AUT_AEQUALIS_I32(horologium.morae[I], CD);
    CREDO_MINOR_I32(horologium.morae[I], DCCC);

    horologium.numerus  = ZEPHYRUM;
    n0                  = simulacrum_petitiones(s);
    simulacrum_vitium_addere(s, _vitium(SIMULACRUM_VITIUM_STATUS,
        HTTP_PUT,
        NIHIL, DIII, "SlowDown", ZEPHYRUM, III));
    CREDO_FALSUM(horreum_exsequi(h, &o, &x, NIHIL, &e, p));
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)HORREUM_TRANSITORIUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(e.codex, "SlowDown");
    CREDO_AEQUALIS_I32(simulacrum_petitiones(s) - n0, III);
    CREDO_AEQUALIS_I32(x.conatus, III);
    CREDO_AEQUALIS_I32(horologium.numerus, II);
    CREDO_NON_NIHIL(x.responsum);
    si (x.responsum)
    {
        CREDO_AEQUALIS_I32(x.responsum->status, DIII);
    }
}

interior vacuum
_sectio_mora_servi (
    CredoContextus* c)
{
          Piscina* p = c->piscina;
       Horologium  horologium;
       Simulacrum* s = _simulacrum(p);
          Horreum* h = _horreum(s, &horologium, SECRETUM, p);
     HorreumError  e;

    simulacrum_vitium_addere(s, _vitium(SIMULACRUM_VITIUM_STATUS,
        HTTP_PUT,
        NIHIL, DIII, "SlowDown", II, I));
    CREDO_VERUM(horreum_ponere(h, _l(SIMULACRUM_SITULA, p), _l("r.txt",
        p), _l("r", p), NIHIL, NIHIL, &e, p));
    CREDO_AEQUALIS_I32(horologium.numerus, I);
    CREDO_MAIOR_AUT_AEQUALIS_I32(horologium.morae[ZEPHYRUM], MM);
}

/* ===== 412 post iterationem (II.5) ===== */
interior vacuum
_sectio_iam_nostrum (
    CredoContextus* c)
{
            Piscina* p = c->piscina;
         Horologium  horologium;
         Simulacrum* s       = _simulacrum(p);
            Horreum* h       = _horreum(s, &horologium, SECRETUM, p);
             chorda  situla  = _l(SIMULACRUM_SITULA, p);
     HorreumPonendi  ponendi;
        HorreumOpus  o;
      HorreumExitus  x;
    HorreumObjectum  ob;
       HorreumError  e;
             chorda  corpus;

    memset(&ponendi, ZEPHYRUM, magnitudo(ponendi));
    ponendi.solum_si_abest = VERUM;
    simulacrum_vitium_addere(s, _vitium(
        SIMULACRUM_VITIUM_SCRIBERE_ABRUMPERE, HTTP_PUT, "nova.txt",
        ZEPHYRUM, NIHIL, ZEPHYRUM, I));
    CREDO_VERUM(horreum_opus_ponere(h, situla, _l("nova.txt", p), _l(
        "salve", p), &ponendi, &o, &e, p));
    CREDO_VERUM(horreum_exsequi(h, &o, &x, &ob, &e, p));
    CREDO_VERUM(x.successus);
    CREDO_VERUM(x.iam_nostrum);
    CREDO_VERUM(ob.iam_nostrum);
    CREDO_AEQUALIS_I32(x.conatus, II);
    CREDO_AEQUALIS_I32(simulacrum_petitiones(s), III);
    CREDO_AEQUALIS_S64((s64)ob.mensura, (s64)V);
    CREDO_CHORDA_AEQUALIS_LITERIS(ob.etag, ETAG_SALVE);
    CREDO_VERUM(horreum_capere(h, situla, _l("nova.txt", p), NIHIL,
        &corpus, NIHIL, &e, p));
    CREDO_CHORDA_AEQUALIS_LITERIS(corpus, "salve");
}

interior vacuum
_sectio_conflictus_verus (
    CredoContextus* c)
{
            Piscina* p = c->piscina;
         Horologium  horologium;
         Simulacrum* s       = _simulacrum(p);
            Horreum* h       = _horreum(s, &horologium, SECRETUM, p);
             chorda  situla  = _l(SIMULACRUM_SITULA, p);
     HorreumPonendi  ponendi;
        HorreumOpus  o;
      HorreumExitus  x;
    HorreumObjectum  ob;
       HorreumError  e;
             chorda  corpus;
                i32  n0;

    simulacrum_obiectum_ponere(s, _l("alienum.txt", p), _l("alienum",
        p));
    memset(&ponendi, ZEPHYRUM, magnitudo(ponendi));
    ponendi.solum_si_abest = VERUM;
    simulacrum_vitium_addere(s, _vitium(SIMULACRUM_VITIUM_STATUS,
        HTTP_PUT,
        NIHIL, DIII, "SlowDown", ZEPHYRUM, I));
    CREDO_VERUM(horreum_opus_ponere(h, situla, _l("alienum.txt", p), _l(
        "nostrum", p), &ponendi, &o, &e, p));
    CREDO_FALSUM(horreum_exsequi(h, &o, &x, &ob, &e, p));
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)HORREUM_CONDITIO);
    CREDO_AEQUALIS_I32(e.status, CDXII);
    CREDO_FALSUM(x.iam_nostrum);
    CREDO_FALSUM(ob.iam_nostrum);
    CREDO_AEQUALIS_I32(simulacrum_petitiones(s), III);
    CREDO_VERUM(horreum_capere(h, situla, _l("alienum.txt", p), NIHIL,
        &corpus, NIHIL, &e, p));
    CREDO_CHORDA_AEQUALIS_LITERIS(corpus, "alienum");

    /* sine vitio: 412 primo conatu, nullum HEAD */
    n0 = simulacrum_petitiones(s);
    CREDO_FALSUM(horreum_exsequi(h, &o, &x, &ob, &e, p));
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)HORREUM_CONDITIO);
    CREDO_AEQUALIS_I32(simulacrum_petitiones(s) - n0, I);
    CREDO_AEQUALIS_I32(x.conatus, I);
}

interior vacuum
_sectio_si_etag_iterum (
    CredoContextus* c)
{
            Piscina* p = c->piscina;
         Horologium  horologium;
         Simulacrum* s       = _simulacrum(p);
            Horreum* h       = _horreum(s, &horologium, SECRETUM, p);
             chorda  situla  = _l(SIMULACRUM_SITULA, p);
     HorreumPonendi  ponendi;
    HorreumObjectum  ob;
       HorreumError  e;
             chorda  corpus;

    CREDO_VERUM(horreum_ponere(h, situla, _l("v.txt", p), _l("una", p),
        NIHIL, &ob, &e, p));
    memset(&ponendi, ZEPHYRUM, magnitudo(ponendi));
    ponendi.solum_si_etag = ob.etag;
    simulacrum_vitium_addere(s, _vitium(
        SIMULACRUM_VITIUM_SCRIBERE_ABRUMPERE, HTTP_PUT, "v.txt",
        ZEPHYRUM, NIHIL, ZEPHYRUM, I));
    CREDO_VERUM(horreum_ponere(h, situla, _l("v.txt", p), _l("duae", p),
        &ponendi, &ob, &e, p));
    CREDO_VERUM(ob.iam_nostrum);
    CREDO_VERUM(horreum_capere(h, situla, _l("v.txt", p), NIHIL,
        &corpus,
        NIHIL, &e, p));
    CREDO_CHORDA_AEQUALIS_LITERIS(corpus, "duae");
}

/* ===== horologium ===== */
interior vacuum
_sectio_horologium (
    CredoContextus* c)
{
          Piscina* p = c->piscina;
       Horologium  horologium;
       Simulacrum* s       = _simulacrum(p);
          Horreum* h       = _horreum(s, &horologium, SECRETUM, p);
           chorda  situla  = _l(SIMULACRUM_SITULA, p);
      HorreumOpus  o;
    HorreumExitus  x;
     HorreumError  e;
              i32  n0;

    simulacrum_horam_ponere(s, TEMPUS + (MoraSec)MCC);
    CREDO_VERUM(horreum_opus_ponere(h, situla, _l("h.txt", p), _l("h",
        p), NIHIL, &o, &e, p));
    CREDO_VERUM(horreum_exsequi(h, &o, &x, NIHIL, &e, p));
    CREDO_AEQUALIS_S64(x.deviatio_s, (s64)MCC);
    CREDO_AEQUALIS_I32(x.conatus, II);
    CREDO_AEQUALIS_I32(horologium.numerus, ZEPHYRUM);

    /* deviatio in sede manet: vocatio proxima primo conatu */
    n0 = simulacrum_petitiones(s);
    CREDO_VERUM(horreum_exsequi(h, &o, &x, NIHIL, &e, p));
    CREDO_AEQUALIS_I32(simulacrum_petitiones(s) - n0, I);
    CREDO_AEQUALIS_S64(x.deviatio_s, (s64)MCC);
}

/* ===== deletio plurium ===== */
interior vacuum
_sectio_delere_plura_partialis (
    CredoContextus* c)
{
            Piscina* p = c->piscina;
         Horologium  horologium;
         Simulacrum* s       = _simulacrum(p);
            Horreum* h       = _horreum(s, &horologium, SECRETUM, p);
             chorda  situla  = _l(SIMULACRUM_SITULA, p);
             chorda  claves[III];
     HorreumDeletio* d = NIHIL;
                i32  n = ZEPHYRUM;
    HorreumObjectum  ob;
       HorreumError  e;

    claves[ZEPHYRUM]  = _l("a", p);
    claves[I]         = _l("b", p);
    claves[II]        = _l("c", p);
    simulacrum_obiectum_ponere(s, claves[ZEPHYRUM], _l("1", p));
    simulacrum_obiectum_ponere(s, claves[I], _l("2", p));
    simulacrum_obiectum_ponere(s, claves[II], _l("3", p));
    simulacrum_vitium_addere(s, _vitium(SIMULACRUM_VITIUM_STATUS,
        HTTP_POST, "b", CDIII, "AccessDenied", ZEPHYRUM, I));
    CREDO_VERUM(horreum_delere_plura(h, situla, claves, III, &d, &n, &e,
        p));
    CREDO_AEQUALIS_I32(n, III);
    si (n == III)
    {
        CREDO_CHORDA_AEQUALIS_LITERIS(d[ZEPHYRUM].clavis, "a");
        CREDO_VERUM(d[ZEPHYRUM].deletum);
        CREDO_CHORDA_AEQUALIS_LITERIS(d[I].clavis, "b");
        CREDO_FALSUM(d[I].deletum);
        CREDO_AEQUALIS_I32((i32)d[I].error.genus, (i32)HORREUM_VETITUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(d[I].error.codex, "AccessDenied");
        CREDO_VERUM(d[II].deletum);
    }
    CREDO_VERUM(horreum_inspicere(h, situla, claves[I], &ob, &e, p));
    CREDO_FALSUM(horreum_inspicere(h, situla, claves[ZEPHYRUM], &ob, &e,
        p));
}

/* ===== paginae (review focus 3) ===== */
interior vacuum
_sectio_enumerare_paginae (
    CredoContextus* c)
{
              Piscina* p = c->piscina;
           Horologium  horologium;
           Simulacrum* s       = _simulacrum(p);
              Horreum* h       = _horreum(s, &horologium, SECRETUM, p);
               chorda  situla  = _l(SIMULACRUM_SITULA, p);
    HorreumEnumerandi  en;
        HorreumPagina  pg;
         HorreumError  e;
                  b32* visae;
                  i32  i;
                  i32  paginae  = ZEPHYRUM;
                  i32  summa    = ZEPHYRUM;
                  i32  bis      = ZEPHYRUM;
                  i32  ignotae  = ZEPHYRUM;
                  b32  mira     = FALSUM;

    visae = (b32*)piscina_allocare(p, (memoriae_index)(CLAVES_MULTAE
        * (i32)magnitudo(b32)));
    memset(visae, ZEPHYRUM, (size_t)CLAVES_MULTAE * magnitudo(b32));
    per (i = ZEPHYRUM; i < CLAVES_MULTAE - I; i++)
    {
        character t[XVI];

        sprintf(t, "k%04u", (insignatus integer)i);
        simulacrum_obiectum_ponere(s, _l(t, p), _l("x", p));
    }
    simulacrum_obiectum_ponere(s, _l(CLAVIS_MIRA, p), _l("x", p));

    memset(&en, ZEPHYRUM, magnitudo(en));
    en.magnitudo_paginae = M;
    fac
    {
        CREDO_VERUM(horreum_enumerare(h, situla, &en, &pg, &e, p));
        paginae++;
        per (i = ZEPHYRUM; i < pg.obiecta_numerus; i++)
        {
            chorda k = pg.obiecta[i].clavis;
               s32 numerus;

            summa++;
            si (chorda_aequalis_literis(k, CLAVIS_MIRA))
            {
                bis += visae[CLAVES_MULTAE - I] ? I : ZEPHYRUM;
                visae[CLAVES_MULTAE - I] = VERUM;
                mira = VERUM;
            }
            alioquin si (   k.mensura == V && k.datum[ZEPHYRUM] == 'k'
                         && chorda_ut_s32(chorda_sectio(k, I, V),
                                &numerus)
                         && numerus   >= ZEPHYRUM
                         && numerus < (s32)(CLAVES_MULTAE - I))
            {
                bis             += visae[numerus] ? I : ZEPHYRUM;
                visae[numerus]  = VERUM;
            }
            alioquin
            {
                ignotae++;
            }
        }
        si (paginae == I)
        {
            CREDO_AEQUALIS_I32(pg.obiecta_numerus, M);
            CREDO_VERUM(pg.truncata);
            CREDO_CHORDA_NON_VACUA(pg.signum_proximum);
        }
        en.signum_continuationis = pg.signum_proximum;
    }
    dum (pg.truncata && paginae < X);
    CREDO_AEQUALIS_I32(paginae, III);
    CREDO_AEQUALIS_I32(summa, CLAVES_MULTAE);
    CREDO_AEQUALIS_I32(bis, ZEPHYRUM);
    CREDO_AEQUALIS_I32(ignotae, ZEPHYRUM);
    CREDO_VERUM(mira);

    /* praefixum et delimitator: praefixa communia */
    simulacrum_obiectum_ponere(s, _l("dir/x/1", p), _l("x", p));
    simulacrum_obiectum_ponere(s, _l("dir/x/2", p), _l("x", p));
    simulacrum_obiectum_ponere(s, _l("dir/y", p), _l("x", p));
    memset(&en, ZEPHYRUM, magnitudo(en));
    en.praefixum    = _l("dir/", p);
    en.delimitator  = _l("/", p);
    CREDO_VERUM(horreum_enumerare(h, situla, &en, &pg, &e, p));
    CREDO_AEQUALIS_I32(pg.obiecta_numerus, I);
    CREDO_AEQUALIS_I32(pg.praefixa_numerus, I);
    si (pg.obiecta_numerus == I && pg.praefixa_numerus == I)
    {
        CREDO_CHORDA_AEQUALIS_LITERIS(pg.obiecta[ZEPHYRUM].clavis,
            "dir/y");
        CREDO_CHORDA_AEQUALIS_LITERIS(pg.praefixa_communia[ZEPHYRUM],
            "dir/x/");
    }
}

/* ===== CopyObject: 200 cum <Error> ===== */
interior vacuum
_sectio_copiare_error_cc (
    CredoContextus* c)
{
          Piscina* p = c->piscina;
       Horologium  horologium;
       Simulacrum* s       = _simulacrum(p);
          Horreum* h       = _horreum(s, &horologium, SECRETUM, p);
           chorda  situla  = _l(SIMULACRUM_SITULA, p);
      HorreumOpus  o;
    HorreumExitus  x;
     HorreumError  e;

    simulacrum_obiectum_ponere(s, _l("fons.txt", p), _l("salve", p));
    simulacrum_vitium_addere(s, _vitium(SIMULACRUM_VITIUM_STATUS,
        HTTP_PUT,
        "d.txt", CC, "InternalError", ZEPHYRUM, I));
    CREDO_VERUM(horreum_opus_copiare(h, situla, _l("fons.txt", p),
        situla,
        _l("d.txt", p), NIHIL, &o, &e, p));
    CREDO_VERUM(horreum_exsequi(h, &o, &x, NIHIL, &e, p));
    CREDO_AEQUALIS_I32(x.conatus, II);

    simulacrum_vitium_addere(s, _vitium(SIMULACRUM_VITIUM_STATUS,
        HTTP_PUT,
        "d.txt", CC, "AccessDenied", ZEPHYRUM, I));
    CREDO_FALSUM(horreum_exsequi(h, &o, &x, NIHIL, &e, p));
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)HORREUM_VETITUM);
    CREDO_AEQUALIS_I32(e.status, CC);
    CREDO_CHORDA_AEQUALIS_LITERIS(e.codex, "AccessDenied");
    CREDO_AEQUALIS_I32(x.conatus, I);
}

/* ===== corpus nimium (review focus 4, P1) ===== */
interior HttpResultus
_nimium_exsequi (
    HttpPetitio* petitio,
        Piscina* p,
         vacuum* datum)
{
    HttpResultus res;

    (vacuum)petitio;
    (vacuum)datum;
    memset(&res, ZEPHYRUM, magnitudo(res));
    res.error             = HTTP_ERROR_PARSE;
    res.error_descriptio  = _l("corpus nimium", p);
    redde res;
}

interior vacuum
_sectio_capere_maius (
    CredoContextus* c)
{
          Piscina* p = c->piscina;
       Horologium  horologium;
      HttpVectura  v;
          Horreum* h;
      HorreumOpus  o;
    HorreumExitus  x;
     HorreumError  e;

    memset(&horologium, ZEPHYRUM, magnitudo(horologium));
    horologium.tempus = TEMPUS;
    v.exsequi = _nimium_exsequi;
    v.datum = NIHIL;
    h = _horreum_cum_vectura(v, &horologium, SECRETUM, p);
    CREDO_VERUM(horreum_opus_capere(h, _l(SIMULACRUM_SITULA, p), _l(
        "magnum.bin", p), NIHIL, &o, &e, p));
    CREDO_FALSUM(horreum_exsequi(h, &o, &x, NIHIL, &e, p));
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)HORREUM_MALUM);
    CREDO_CHORDA_CONTINET(e.nuntius, _l("horreum_capere_in_filum", p));
    CREDO_AEQUALIS_I32(x.conatus, I);
    CREDO_AEQUALIS_I32(horologium.numerus, ZEPHYRUM);
}

/* ===== T4: multipars et fila ===== */
#define MIB_T4  ((i64)HORREUM_MIB)

interior SimulacrumVitium
_vitium_ad (
       SimulacrumVitium  v,
     constans character* parametrum)
{
    v.parametrum = parametrum;
    redde v;
}

/* exemplar octetorum non periodicum per MiB (frusta permutata
 * deprehenduntur) */
interior i8
_octetus (
    i64 i)
{
    redde (i8)((i * VII + i / (i64)(IV * M + XCIII)) & 0xFF);
}

interior chorda
_corpus_exemplaris (
         i64  ab,
         i64  n,
     Piscina* p)
{
     i8* t = (i8*)piscina_allocare(p, (memoriae_index)n);
    i64  i;

    per (i = ZEPHYRUM; i < n; i++)
    {
        t[i] = _octetus(ab + i);
    }
    redde chorda_ex_buffer(t, (i32)n);
}

interior b32
_exemplar_congruit (
    chorda c,
       i64 mensura)
{
    i64 i;

    si ((i64)c.mensura != mensura)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < mensura; i++)
    {
        si (c.datum[i] != _octetus(i))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* /tmp/horreum-<pid>-<titulus>, vacuum */
interior constans character*
_directorium (
    constans character* titulus,
               Piscina* p)
{
    character* via = (character*)piscina_allocare(p, CXXVIII);

    sprintf(via, "/tmp/horreum-%ld-%s", (longus)getpid(), titulus);
    (vacuum)filum_arborem_delere(via);
    (vacuum)filum_directorium_creare_cum_parentibus(via);
    redde via;
}

interior constans character*
_via (
    constans character* directorium,
    constans character* titulus,
               Piscina* p)
{
    character* via = (character*)piscina_allocare(p, CCLVI);

    sprintf(via, "%s/%s", directorium, titulus);
    redde via;
}

interior b32
_filum_exemplaris (
    constans character* via,
                   i64  mensura,
               Piscina* p)
{
    FilumScriptor* sc = filum_scriptor_aperire(via, FILUM_MODUS_CREARE,
        p);
              i64 ab;
              b32 bene = sc != NIHIL;

    per (ab = ZEPHYRUM; bene && ab < mensura; ab += MIB_T4)
    {
            i64  n = mensura - ab < MIB_T4 ? mensura - ab : MIB_T4;
        Piscina* q = piscina_generare_dynamicum("exemplar",
            (memoriae_index)
            (n + MXXIV));
        chorda c = _corpus_exemplaris(ab, n, q);

        bene = filum_scriptor_octetos_scribere(sc, c.datum, c.mensura);
        piscina_destruere(q);
    }
    si (sc)
    {
        filum_scriptor_claudere(sc);
    }
    redde bene;
}

/* introitus directorii praeter . et .. */
interior i32
_introitus (
    constans character* directorium,
               Piscina* p)
{
    DirectoriumIterator* it = directorium_iterator_aperire(directorium,
        p);
   DirectoriumIntroitus* t;
                    i32  n = ZEPHYRUM;

    si (!it)
    {
        redde ZEPHYRUM;
    }
    dum ((t = directorium_iterator_proximum(it)) != NIHIL)
    {
        si (   !chorda_aequalis_literis(t->titulus, ".")
            && !chorda_aequalis_literis(t->titulus, ".."))
        {
            n++;
        }
    }
    directorium_iterator_claudere(it);
    redde n;
}

interior i32
_pendentes (
               Horreum* h,
    constans character* praefixum,
               Piscina* p)
{
    HorreumOnusPendens* t = NIHIL;
                   i32  n = ZEPHYRUM;
          HorreumError  e;

    si (!horreum_mp_pendentes(h, _l(SIMULACRUM_SITULA, p), _l(praefixum,
            p), &t, &n, &e, p))
    {
        redde (i32)CMXCIX;
    }
    redde n;
}

interior vacuum
_sectio_partium_iter (
    CredoContextus* c)
{
            Piscina* p = c->piscina;
         Horologium  horologium;
         Simulacrum* s       = _simulacrum(p);
            Horreum* h       = _horreum(s, &horologium, SECRETUM, p);
             chorda  situla  = _l(SIMULACRUM_SITULA, p);
             chorda  clavis  = _l("mp.bin", p);
             chorda  id;
        HorreumPars  partes[III];
        HorreumPars* lectae  = NIHIL;
                i32  n       = ZEPHYRUM;
    HorreumObjectum  ob;
       HorreumError  e;
             chorda  corpus;
                i64  ab = ZEPHYRUM;
                i32  k;

    CREDO_VERUM(horreum_mp_incipere(h, situla, clavis, NIHIL, &id, &e,
        p));
    CREDO_CHORDA_NON_VACUA(id);
    per (k = ZEPHYRUM; k < III; k++)
    {
        i64 m = k < II ? V * MIB_T4 : MIB_T4;

        CREDO_VERUM(horreum_mp_partem_mittere(h, situla, clavis, id, k
            + I,
            _corpus_exemplaris(ab, m, p), &partes[k], &e, p));
        CREDO_AEQUALIS_S64((s64)partes[k].mensura, (s64)m);
        ab += m;
    }
    CREDO_VERUM(horreum_mp_partes(h, situla, clavis, id, &lectae, &n,
        &e,
        p));
    CREDO_AEQUALIS_I32(n, III);
    si (n == III)
    {
        CREDO_AEQUALIS_I32(lectae[II].numerus, III);
        CREDO_AEQUALIS_S64((s64)lectae[II].mensura, (s64)MIB_T4);
        CREDO_VERUM(chorda_aequalis(lectae[I].etag, partes[I].etag));
    }
    CREDO_VERUM(horreum_mp_perficere(h, situla, clavis, id, partes, III,
        &ob, &e, p));
    CREDO_CHORDA_TERMINATUR(ob.etag, _l("-3", p));
    CREDO_FALSUM(ob.iam_nostrum);
    CREDO_VERUM(horreum_inspicere(h, situla, clavis, &ob, &e, p));
    CREDO_AEQUALIS_S64((s64)ob.mensura, (s64)(XI * MIB_T4));
    CREDO_VERUM(horreum_capere(h, situla, clavis, NIHIL, &corpus, NIHIL,
        &e, p));
    CREDO_VERUM(_exemplar_congruit(corpus, XI * MIB_T4));
    CREDO_AEQUALIS_I32(_pendentes(h, "", p), ZEPHYRUM);
    simulacrum_claudere(s);
}

interior vacuum
_sectio_ex_filo_parvum (
    CredoContextus* c)
{
            Piscina* p = c->piscina;
         Horologium  horologium;
         Simulacrum* s    = _simulacrum(p);
            Horreum* h    = _horreum(s, &horologium, SECRETUM, p);
 constans character* d    = _directorium("parvum", p);
 constans character* via  = _via(d, "parvum.bin", p);
    HorreumObjectum  ob;
       HorreumError  e;

    CREDO_VERUM(_filum_exemplaris(via, MIB_T4, p));
    CREDO_VERUM(horreum_ponere_ex_filo(h, _l(SIMULACRUM_SITULA, p), _l(
        "parvum.bin", p), via, NIHIL, NIHIL, &ob, &e, p));
    CREDO_AEQUALIS_I32(simulacrum_petitiones(s), I);
    CREDO_FALSUM(chorda_continet(ob.etag, _l("-", p)));
    CREDO_VERUM(_exemplar_congruit(simulacrum_obiectum_corpus(s, _l(
        "parvum.bin", p)), MIB_T4));
    simulacrum_claudere(s);
    CREDO_VERUM(filum_arborem_delere(d));
}

interior vacuum
_sectio_ex_filo_magnum (
    CredoContextus* c)
{
            Piscina* p = c->piscina;
         Horologium  horologium;
         Simulacrum* s    = _simulacrum(p);
            Horreum* h    = _horreum(s, &horologium, SECRETUM, p);
 constans character* d    = _directorium("magnum", p);
 constans character* via  = _via(d, "magnum.bin", p);
    HorreumObjectum  ob;
       HorreumError  e;

    CREDO_VERUM(_filum_exemplaris(via, XL * MIB_T4, p));
    CREDO_VERUM(horreum_ponere_ex_filo(h, _l(SIMULACRUM_SITULA, p), _l(
        "magnum.bin", p), via, NIHIL, NIHIL, &ob, &e, p));
    /* incipere, partes XVI + XVI + VIII, perficere */
    CREDO_AEQUALIS_I32(simulacrum_petitiones(s), V);
    CREDO_AEQUALIS_I32(simulacrum_corpus_maximum(s),
        (i32)(XVI * MIB_T4));
    CREDO_CHORDA_TERMINATUR(ob.etag, _l("-3", p));
    CREDO_AEQUALIS_S64((s64)ob.mensura, (s64)(XL * MIB_T4));
    CREDO_VERUM(_exemplar_congruit(simulacrum_obiectum_corpus(s, _l(
        "magnum.bin", p)), XL * MIB_T4));
    CREDO_AEQUALIS_I32(_pendentes(h, "", p), ZEPHYRUM);
    simulacrum_claudere(s);
    CREDO_VERUM(filum_arborem_delere(d));
}

interior vacuum
_sectio_pars_iterata (
    CredoContextus* c)
{
                Piscina* p = c->piscina;
             Horologium  horologium;
             Simulacrum* s    = _simulacrum(p);
                Horreum* h    = _horreum(s, &horologium, SECRETUM, p);
     constans character* d    = _directorium("iterata", p);
     constans character* via  = _via(d, "iterata.bin", p);
    HorreumFiliOptiones  fo;
        HorreumObjectum  ob;
           HorreumError  e;

    CREDO_VERUM(_filum_exemplaris(via, XI * MIB_T4, p));
    memset(&fo, ZEPHYRUM, magnitudo(fo));
    fo.pars_mensura = V * MIB_T4;
    simulacrum_vitium_addere(s,
        _vitium_ad(_vitium(SIMULACRUM_VITIUM_STATUS,
        HTTP_PUT, NIHIL, DIII, "SlowDown", ZEPHYRUM, I),
        "partNumber=2"));
    CREDO_VERUM(horreum_ponere_ex_filo(h, _l(SIMULACRUM_SITULA, p), _l(
        "iterata.bin", p), via, NIHIL, &fo, &ob, &e, p));
    /* incipere, pars I, pars II bis, pars III, perficere */
    CREDO_AEQUALIS_I32(simulacrum_petitiones(s), VI);
    CREDO_AEQUALIS_I32(horologium.numerus, I);
    CREDO_VERUM(_exemplar_congruit(simulacrum_obiectum_corpus(s, _l(
        "iterata.bin", p)), XI * MIB_T4));
    simulacrum_claudere(s);
    CREDO_VERUM(filum_arborem_delere(d));
}

interior vacuum
_sectio_abortus (
    CredoContextus* c)
{
                Piscina* p = c->piscina;
             Horologium  horologium;
             Simulacrum* s    = _simulacrum(p);
                Horreum* h    = _horreum(s, &horologium, SECRETUM, p);
     constans character* d    = _directorium("abortus", p);
     constans character* via  = _via(d, "abortus.bin", p);
    HorreumFiliOptiones  fo;
        HorreumObjectum  ob;
           HorreumError  e;

    CREDO_VERUM(_filum_exemplaris(via, XI * MIB_T4, p));
    memset(&fo, ZEPHYRUM, magnitudo(fo));
    fo.pars_mensura = V * MIB_T4;
    simulacrum_vitium_addere(s,
        _vitium_ad(_vitium(SIMULACRUM_VITIUM_STATUS,
        HTTP_PUT, NIHIL, DIII, "SlowDown", ZEPHYRUM, III),
        "partNumber=2"));
    CREDO_FALSUM(horreum_ponere_ex_filo(h, _l(SIMULACRUM_SITULA, p), _l(
        "abortus.bin", p), via, NIHIL, &fo, &ob, &e, p));
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)HORREUM_TRANSITORIUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(e.codex, "SlowDown");
    /* incipere, pars I, pars II ter, abicere */
    CREDO_AEQUALIS_I32(simulacrum_petitiones(s), VI);
    CREDO_AEQUALIS_I32(_pendentes(h, "", p), ZEPHYRUM);
    CREDO_FALSUM(horreum_inspicere(h, _l(SIMULACRUM_SITULA, p), _l(
        "abortus.bin", p), &ob, &e, p));
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)HORREUM_ABEST);
    simulacrum_claudere(s);
    CREDO_VERUM(filum_arborem_delere(d));
}

interior vacuum
_sectio_perficere_iam_nostrum (
    CredoContextus* c)
{
            Piscina* p = c->piscina;
         Horologium  horologium;
         Simulacrum* s       = _simulacrum(p);
            Horreum* h       = _horreum(s, &horologium, SECRETUM, p);
             chorda  situla  = _l(SIMULACRUM_SITULA, p);
             chorda  clavis  = _l("perfectum.bin", p);
             chorda  id;
        HorreumPars  partes[II];
    HorreumObjectum  ob;
       HorreumError  e;

    CREDO_VERUM(horreum_mp_incipere(h, situla, clavis, NIHIL, &id, &e,
        p));
    CREDO_VERUM(horreum_mp_partem_mittere(h, situla, clavis, id, I,
        _corpus_exemplaris(ZEPHYRUM, V * MIB_T4, p), &partes[ZEPHYRUM],
        &e,
        p));
    CREDO_VERUM(horreum_mp_partem_mittere(h, situla, clavis, id, II,
        _corpus_exemplaris(V * MIB_T4, MIB_T4, p), &partes[I], &e, p));
    simulacrum_vitium_addere(s, _vitium_ad(_vitium(
        SIMULACRUM_VITIUM_SCRIBERE_ABRUMPERE, HTTP_POST,
        "perfectum.bin",
        ZEPHYRUM, NIHIL, ZEPHYRUM, I), "uploadId"));
    CREDO_VERUM(horreum_mp_perficere(h, situla, clavis, id, partes, II,
        &ob,
        &e, p));
    CREDO_VERUM(ob.iam_nostrum);
    CREDO_CHORDA_TERMINATUR(ob.etag, _l("-2", p));
    /* incipere, II partes, perficere bis, HEAD */
    CREDO_AEQUALIS_I32(simulacrum_petitiones(s), VI);
    CREDO_VERUM(_exemplar_congruit(simulacrum_obiectum_corpus(s,
        clavis),
        VI * MIB_T4));
    simulacrum_claudere(s);
}

interior vacuum
_sectio_pendentes (
    CredoContextus* c)
{
               Piscina* p = c->piscina;
            Horologium  horologium;
            Simulacrum* s       = _simulacrum(p);
               Horreum* h       = _horreum(s, &horologium, SECRETUM, p);
                chorda  situla  = _l(SIMULACRUM_SITULA, p);
                chorda  id;
    HorreumOnusPendens* t = NIHIL;
                   i32  n = ZEPHYRUM;
                   i32  k;
          HorreumError  e;

    CREDO_VERUM(horreum_mp_incipere(h, situla, _l("x/a", p), NIHIL, &id,
        &e,
        p));
    CREDO_VERUM(horreum_mp_incipere(h, situla, _l("x/b", p), NIHIL, &id,
        &e,
        p));
    CREDO_VERUM(horreum_mp_incipere(h, situla, _l("y/c", p), NIHIL, &id,
        &e,
        p));
    CREDO_VERUM(horreum_mp_pendentes(h, situla, _l("x/", p), &t, &n, &e,
        p));
    CREDO_AEQUALIS_I32(n, II);
    per (k = ZEPHYRUM; k < n; k++)
    {
        CREDO_CHORDA_INCIPIT(t[k].clavis, _l("x/", p));
        CREDO_AEQUALIS_S64((s64)t[k].initium, (s64)TEMPUS);
        CREDO_VERUM(horreum_mp_abicere(h, situla, t[k].clavis,
            t[k].id_oneris, &e, p));
    }
    CREDO_AEQUALIS_I32(_pendentes(h, "x/", p), ZEPHYRUM);
    CREDO_AEQUALIS_I32(_pendentes(h, "", p), I);
    CREDO_VERUM(horreum_mp_abicere(h, situla, _l("y/c", p), id, &e, p));
    CREDO_AEQUALIS_I32(_pendentes(h, "", p), ZEPHYRUM);
    CREDO_FALSUM(horreum_mp_abicere(h, situla, _l("y/c", p), id, &e,
        p));
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)HORREUM_ABEST);
    simulacrum_claudere(s);
}

interior vacuum
_sectio_capere_in_filum (
    CredoContextus* c)
{
            Piscina* p = c->piscina;
         Horologium  horologium;
         Simulacrum* s    = _simulacrum(p);
            Horreum* h    = _horreum(s, &horologium, SECRETUM, p);
 constans character* d    = _directorium("capere", p);
 constans character* via  = _via(d, "destinatio.bin", p);
    HorreumObjectum  ob;
       HorreumError  e;

    simulacrum_obiectum_ponere(s, _l("viginti.bin", p),
        _corpus_exemplaris(
        ZEPHYRUM, XX * MIB_T4, p));
    CREDO_VERUM(horreum_capere_in_filum(h, _l(SIMULACRUM_SITULA, p), _l(
        "viginti.bin", p), via, &ob, &e, p));
    /* HEAD, deinde frusta VIII + VIII + IV MiB, quodque cum If-Match */
    CREDO_AEQUALIS_I32(simulacrum_petitiones(s), IV);
    CREDO_AEQUALIS_I32(simulacrum_si_etag_visa(s), III);
    CREDO_AEQUALIS_S64((s64)ob.mensura, (s64)(XX * MIB_T4));
    CREDO_VERUM(_exemplar_congruit(filum_legere_totum(via, p),
        XX * MIB_T4));
    CREDO_AEQUALIS_I32(_introitus(d, p), I);
    simulacrum_claudere(s);
    CREDO_VERUM(filum_arborem_delere(d));
}

interior vacuum
_sectio_capere_mutatum (
    CredoContextus* c)
{
            Piscina* p = c->piscina;
         Horologium  horologium;
         Simulacrum* s    = _simulacrum(p);
            Horreum* h    = _horreum(s, &horologium, SECRETUM, p);
 constans character* d    = _directorium("mutatum", p);
 constans character* via  = _via(d, "destinatio.bin", p);
    HorreumObjectum  ob;
       HorreumError  e;

    simulacrum_obiectum_ponere(s, _l("mutabile.bin", p),
        _corpus_exemplaris(
        ZEPHYRUM, XX * MIB_T4, p));
    simulacrum_vitium_addere(s, _vitium(SIMULACRUM_VITIUM_MUTARE,
        HTTP_GET,
        "mutabile.bin", ZEPHYRUM, NIHIL, ZEPHYRUM, I));
    CREDO_FALSUM(horreum_capere_in_filum(h, _l(SIMULACRUM_SITULA, p),
        _l(
        "mutabile.bin", p), via, &ob, &e, p));
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)HORREUM_CONDITIO);
    CREDO_FALSUM(filum_existit(via));
    CREDO_AEQUALIS_I32(_introitus(d, p), ZEPHYRUM);
    simulacrum_claudere(s);
    CREDO_VERUM(filum_arborem_delere(d));
}

interior vacuum
_sectio_capere_mensura_mala (
    CredoContextus* c)
{
            Piscina* p = c->piscina;
         Horologium  horologium;
         Simulacrum* s    = _simulacrum(p);
            Horreum* h    = _horreum(s, &horologium, SECRETUM, p);
 constans character* d    = _directorium("mensura", p);
 constans character* via  = _via(d, "destinatio.bin", p);
    HorreumObjectum  ob;
       HorreumError  e;

    CREDO_VERUM(filum_scribere_literis(via, "vetus"));
    simulacrum_obiectum_ponere(s, _l("breve.bin", p),
        _corpus_exemplaris(
        ZEPHYRUM, IX * MIB_T4, p));
    simulacrum_vitium_addere(s, _vitium(SIMULACRUM_VITIUM_TRUNCARE,
        HTTP_GET,
        "breve.bin", ZEPHYRUM, NIHIL, ZEPHYRUM, I));
    CREDO_FALSUM(horreum_capere_in_filum(h, _l(SIMULACRUM_SITULA, p),
        _l(
        "breve.bin", p), via, &ob, &e, p));
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)HORREUM_MALUM);
    CREDO_CHORDA_CONTINET(e.nuntius, _l("mensura", p));
    CREDO_CHORDA_AEQUALIS_LITERIS(filum_legere_totum(via, p), "vetus");
    CREDO_AEQUALIS_I32(_introitus(d, p), I);
    simulacrum_claudere(s);
    CREDO_VERUM(filum_arborem_delere(d));
}

interior vacuum
_sectio_md5_etag (
    CredoContextus* c)
{
            Piscina* p = c->piscina;
         Horologium  horologium;
         Simulacrum* s    = _simulacrum(p);
            Horreum* h    = _horreum(s, &horologium, SECRETUM, p);
 constans character* d    = _directorium("md5", p);
 constans character* via  = _via(d, "destinatio.bin", p);
    HorreumObjectum  ob;
       HorreumError  e;

    simulacrum_obiectum_ponere(s, _l("parvum.bin", p),
        _corpus_exemplaris(
        ZEPHYRUM, MXXIV, p));
    simulacrum_etag_ponere(s, _l("parvum.bin", p),
        "0123456789abcdef0123456789abcdef");
    CREDO_FALSUM(horreum_capere_in_filum(h, _l(SIMULACRUM_SITULA, p),
        _l(
        "parvum.bin", p), via, &ob, &e, p));
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)HORREUM_MALUM);
    CREDO_CHORDA_CONTINET(e.nuntius, _l("md5", p));
    CREDO_FALSUM(filum_existit(via));
    CREDO_AEQUALIS_I32(_introitus(d, p), ZEPHYRUM);
    simulacrum_claudere(s);
    CREDO_VERUM(filum_arborem_delere(d));
}

/* ===== observator ===== */
nomen structura {
             Piscina* piscina;
                 i32  numerus;
    HorreumOpusGenus  genera[VIII];
                 i32  status[VIII];        /* 0 = transportus */
                 i32  conatus[VIII];
                 b32  error_nominatus[VIII];
                 b32  secretum_visum;
} Observata;

interior vacuum
_observare (
                        vacuum* datum,
    constans HorreumObservatio* o)
{
    Observata* ob  = (Observata*)datum;
       chorda  t   = horreum_observationem_describere(o, ob->piscina);

    si (   chorda_continet(t, _l(SECRETUM, ob->piscina))
        || chorda_continet(t, _l("Credential=", ob->piscina)))
    {
        ob->secretum_visum = VERUM;
    }
    si (ob->numerus < VIII)
    {
        ob->genera[ob->numerus]           = o->opus->genus;
        ob->status[ob->numerus]           = o->responsum
                                            ? o->responsum->status
                                            : ZEPHYRUM;
        ob->conatus[ob->numerus]          = o->conatus;
        ob->error_nominatus[ob->numerus]  =
            o->error_transportus.mensura > ZEPHYRUM;
    }
    ob->numerus++;
}

/* omnis conatus observatur: transportus perit, 412 post iterationem,
 * HEAD iam_nostrum - et transcriptio secretum non continet */
interior vacuum
_sectio_observator (
    CredoContextus* c)
{
                    Piscina* p = c->piscina;
                 Horologium  horologium;
                 Simulacrum* s       = _simulacrum(p);
                    Horreum* h       = _horreum(s, &horologium,
                        SECRETUM,
                                           p);
                     chorda situla  = _l(SIMULACRUM_SITULA, p);
    HorreumClientisOptiones co;
                  Observata ob;
             HorreumPonendi ponendi;
               HorreumError e;

    memset(&ob, ZEPHYRUM, magnitudo(ob));
    ob.piscina = p;
    memset(&co, ZEPHYRUM, magnitudo(co));
    co.vectura             = simulacrum_vectura(s);
    co.nunc                = _nunc;
    co.dormire             = _dormire;
    co.datum               = &horologium;
    co.semen               = VII;
    co.observator          = _observare;
    co.observatoris_datum  = &ob;
    horreum_cliens_parare(h, &co, p);

    memset(&ponendi, ZEPHYRUM, magnitudo(ponendi));
    ponendi.solum_si_abest = VERUM;
    simulacrum_vitium_addere(s, _vitium(
        SIMULACRUM_VITIUM_SCRIBERE_ABRUMPERE, HTTP_PUT, "visa.txt",
        ZEPHYRUM, NIHIL, ZEPHYRUM, I));
    CREDO_VERUM(horreum_ponere(h, situla, _l("visa.txt", p), _l("salve",
        p), &ponendi, NIHIL, &e, p));
    CREDO_AEQUALIS_I32(ob.numerus, III);
    CREDO_AEQUALIS_I32((i32)ob.genera[ZEPHYRUM],
        (i32)HORREUM_OPUS_PONERE);
    CREDO_AEQUALIS_I32(ob.status[ZEPHYRUM], ZEPHYRUM);
    CREDO_VERUM(ob.error_nominatus[ZEPHYRUM]);
    CREDO_AEQUALIS_I32(ob.conatus[ZEPHYRUM], I);
    CREDO_AEQUALIS_I32((i32)ob.genera[I], (i32)HORREUM_OPUS_PONERE);
    CREDO_AEQUALIS_I32(ob.status[I], CDXII);
    CREDO_AEQUALIS_I32(ob.conatus[I], II);
    CREDO_AEQUALIS_I32((i32)ob.genera[II],
        (i32)HORREUM_OPUS_INSPICERE);
    CREDO_AEQUALIS_I32(ob.status[II], CC);
    CREDO_AEQUALIS_I32(ob.conatus[II], I);
    CREDO_FALSUM(ob.secretum_visum);
    simulacrum_claudere(s);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "iter", _sectio_iter, NIHIL, NIHIL, NIHIL, NIHIL },
    { "signatura_verificata", _sectio_signatura_verificata, NIHIL,
        NIHIL,
        NIHIL, NIHIL },
    { "transitorium", _sectio_transitorium, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "retry_after", _sectio_mora_servi, NIHIL, NIHIL, NIHIL, NIHIL },
    { "iam_nostrum", _sectio_iam_nostrum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "conflictus_verus", _sectio_conflictus_verus, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "si_etag_iterum", _sectio_si_etag_iterum, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "horologium", _sectio_horologium, NIHIL, NIHIL, NIHIL, NIHIL },
    { "delere_plura_partialis", _sectio_delere_plura_partialis, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "enumerare_paginae", _sectio_enumerare_paginae, NIHIL, NIHIL,
        NIHIL,
        NIHIL },
    { "copiare_error_cc", _sectio_copiare_error_cc, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "capere_maius", _sectio_capere_maius, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "multipars_iter", _sectio_partium_iter, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "ex_filo_parvum", _sectio_ex_filo_parvum, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "ex_filo_magnum", _sectio_ex_filo_magnum, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "pars_iterata", _sectio_pars_iterata, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "abortus", _sectio_abortus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "perficere_iam_nostrum", _sectio_perficere_iam_nostrum, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "pendentes", _sectio_pendentes, NIHIL, NIHIL, NIHIL, NIHIL },
    { "capere_in_filum", _sectio_capere_in_filum, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "capere_mutatum", _sectio_capere_mutatum, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "capere_mensura_mala", _sectio_capere_mensura_mala, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "md5_etag", _sectio_md5_etag, NIHIL, NIHIL, NIHIL, NIHIL },
    { "observator", _sectio_observator, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("horreum_cliens", SECTIONES);
}

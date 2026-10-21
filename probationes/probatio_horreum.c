/* probatio_horreum.c - nucleus horrei (horreum-plan T2): provisores,
 * opera, signatio, responsa, politica, tabella. Nihil retis: petitiones
 * signatae per http_petitio_visus inspiciuntur et signatura ex filo
 * (via, quaestio, capita signata) per signatura_aws recomputatur - ut
 * servus faceret; responsa ex probationes/fixa/horreum/aws_documenta. */
#include "postulata_posix.h"
#include "latina.h"
#include "horreum.h"
#include "signatura_aws.h"
#include "friatio.h"
#include "base64.h"
#include "filum.h"
#include "url.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define VIA_RESPONSORUM  "probationes/fixa/horreum/aws_documenta"
#define SECRETUM         "SECRETUM-PLANTATUM-0123456789"
#define TEMPUS           ((MomentumSec)1440938160L)   /* 2015-08-30T12:36:00Z */

interior chorda
_l (
    constans character* l,
               Piscina* p)
{
    redde chorda_ex_literis(l, p);
}

interior Horreum*
_horreum (
    constans character* provisor,
    constans character* terminus,
    constans character* regio,
               Piscina* p)
{
    HorreumConfiguratio c;
           HorreumError e;

    memset(&c, ZEPHYRUM, magnitudo(c));
    c.provisor     = provisor;
    c.terminus     = _l(terminus ? terminus : "", p);
    c.regio        = _l(regio ? regio : "", p);
    c.rationarium  = _l("abc123", p);
    c.situla       = _l("situla", p);
    c.clavis_id    = _l("AKIDEXAMPLE", p);
    c.secretum     = _l(SECRETUM, p);
    redde horreum_creare(&c, &e, p);
}

interior chorda
_caput_operis (
    constans HorreumOpus* o,
      constans character* titulus,
                 Piscina* p)
{
    i32 i;

    per (i = ZEPHYRUM; i < o->capita_numerus; i++)
    {
        si (chorda_aequalis_case_insensitivus(o->capita[i].titulus,
                _l(titulus, p)))
        {
            redde o->capita[i].valor;
        }
    }
    redde chorda_ex_buffer(NIHIL, ZEPHYRUM);
}

interior chorda
_caput_visus (
    constans HttpPetitioVisus* v,
           constans character* titulus,
                      Piscina* p)
{
    i32 i;

    per (i = ZEPHYRUM; i < v->capita_numerus; i++)
    {
        si (chorda_aequalis_case_insensitivus(v->capita[i].titulus,
                _l(titulus, p)))
        {
            redde v->capita[i].valor;
        }
    }
    redde chorda_ex_buffer(NIHIL, ZEPHYRUM);
}

/* "20150830T123600Z" -> MomentumSec */
interior MomentumSec
_dies_amz (
     chorda  s,
    Piscina* p)
{
    character iso[XXI];
     DiesHora dh;

    si (s.mensura != XVI)
    {
        redde ZEPHYRUM;
    }
    sprintf(iso, "%.4s-%.2s-%.2sT%.2s:%.2s:%.2sZ",
        (constans character*)s.datum,
        (constans character*)s.datum + IV, (constans character*)s.datum
            + VI,
        (constans character*)s.datum + IX, (constans character*)s.datum
            + XI,
        (constans character*)s.datum + XIII);
    si (!fasti_ex_iso(_l(iso, p), &dh))
    {
        redde ZEPHYRUM;
    }
    redde fasti_ad_unix(dh);
}

/* ut servus: signaturam ex filo recomputat (via decodificata, quaestio
 * decodificata, capita in SignedHeaders praeter host / x-amz-date /
 * x-amz-content-sha256) et cum Authorization comparat */
interior b32
_signatura_verificata (
    HttpPetitio* pet,
         chorda  regio,
        Piscina* p)
{
     HttpPetitioVisus v = http_petitio_visus(pet);
               chorda auctoritas = _caput_visus(&v, "Authorization", p);
               chorda via = v.via;
               chorda quaestio = chorda_ex_buffer(NIHIL, ZEPHYRUM);
                  s32 q = chorda_invenire_index(v.via, _l("?", p));
     SignaturaPetitio sp;
         SignaturaPar par_q[XXXII];
         SignaturaPar par_c[XXXII];
                  i32 nq = ZEPHYRUM;
                  i32 nc = ZEPHYRUM;
               chorda signata;
                  s32 i;
    SignaturaResultus r;
               chorda expectata;

    si (auctoritas.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (q >= ZEPHYRUM)
    {
        via       = chorda_sectio(v.via, ZEPHYRUM, (i32)q);
        quaestio  = chorda_sectio(v.via, (i32)q + I, v.via.mensura);
    }
    dum (quaestio.mensura > ZEPHYRUM && nq < XXXII)
    {
           s32 et  = chorda_invenire_index(quaestio, _l("&", p));
        chorda par = et < ZEPHYRUM ? quaestio
                                   : chorda_sectio(quaestio, ZEPHYRUM,
                                   (i32)et);
           s32 aeq = chorda_invenire_index(par, _l("=", p));

        par_q[nq].titulus = url_decodificare(aeq < ZEPHYRUM ? par
            : chorda_sectio(par, ZEPHYRUM, (i32)aeq), p);
        par_q[nq].valor = aeq < ZEPHYRUM ? _l("", p)
            : url_decodificare(chorda_sectio(par, (i32)aeq + I,
            par.mensura),
                  p);
        nq++;
        si (et < ZEPHYRUM)
        {
            frange;
        }
        quaestio = chorda_sectio(quaestio, (i32)et + I,
            quaestio.mensura);
    }
    i = chorda_invenire_index(auctoritas, _l("SignedHeaders=",
        p));
    signata = chorda_sectio(auctoritas, (i32)i + XIV,
        auctoritas.mensura);
    signata = chorda_sectio(signata, ZEPHYRUM,
        (i32)chorda_invenire_index(signata, _l(",", p)));
    dum (signata.mensura > ZEPHYRUM)
    {
           s32 sc = chorda_invenire_index(signata, _l(";", p));
        chorda t  = sc < ZEPHYRUM ? signata
                                  : chorda_sectio(signata, ZEPHYRUM,
                                  (i32)sc);

        si (   !chorda_aequalis_literis(t, "host")
            && !chorda_aequalis_literis(t, "x-amz-date")
            && !chorda_aequalis_literis(t, "x-amz-content-sha256"))
        {
            par_c[nc].titulus = t;
            par_c[nc].valor = _caput_visus(&v, chorda_ut_cstr(t, p),
                p);
            nc++;
        }
        si (sc < ZEPHYRUM)
        {
            frange;
        }
        signata = chorda_sectio(signata, (i32)sc + I, signata.mensura);
    }
    memset(&sp, ZEPHYRUM, magnitudo(sp));
    sp.methodus          = http_methodus_nomen(v.methodus);
    sp.hospes            = _caput_visus(&v, "Host", p);
    sp.via               = url_decodificare(via, p);
    sp.quaestio          = par_q;
    sp.quaestio_numerus  = nq;
    sp.capita            = par_c;
    sp.capita_numerus    = nc;
    sp.corpus_sigillum   = _caput_visus(&v, "x-amz-content-sha256", p);
    sp.corpus_capite     = VERUM;
    sp.tempus = _dies_amz(_caput_visus(&v, "x-amz-date", p),
        p);
    sp.regio      = regio;
    sp.servitium  = _l("s3", p);
    sp.clavis_id  = _l("AKIDEXAMPLE", p);
    sp.secretum   = _l(SECRETUM, p);
    r             = signatura_aws_capitibus(&sp, p);
    expectata     = r.auctoritas;
    redde r.successus && chorda_aequalis(expectata, auctoritas);
}

/* nullum caput, nec via, secretum continet */
interior b32
_petitio_tacita (
    HttpPetitio* pet,
        Piscina* p)
{
    HttpPetitioVisus v = http_petitio_visus(pet);
              chorda s = _l(SECRETUM, p);
                 i32 i;

    si (chorda_continet(v.via, s) || chorda_continet(v.hospes, s))
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < v.capita_numerus; i++)
    {
        si (chorda_continet(v.capita[i].valor, s))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior HttpPetitio*
_signare (
         Horreum* h,
    HorreumOpus* o,
        Piscina* p)
{
    HorreumError e;

    redde horreum_petitionem_signare(h, o, TEMPUS, &e, p);
}

/* ===== provisores ===== */

interior vacuum
_sectio_provisores (
    CredoContextus* c)
{
                Piscina* p = c->piscina;
                Horreum* h;
            HorreumOpus  o;
           HorreumError  e;
            HttpPetitio* pet;
    HorreumConfiguratio  cfg;

    CREDO_NON_NIHIL(horreum_provisor("digitalocean"));
    CREDO_NON_NIHIL(horreum_provisor("r2"));
    CREDO_NON_NIHIL(horreum_provisor("b2"));
    CREDO_NON_NIHIL(horreum_provisor("generic"));
    CREDO_NIHIL(horreum_provisor("aws9"));

    h = _horreum("digitalocean", NIHIL, "nyc3", p);
    CREDO_NON_NIHIL(h);
    CREDO_VERUM(horreum_opus_inspicere(h, _l("situla", p), _l("a.txt",
        p),
        &o, &e, p));
    pet = _signare(h, &o, p);
    CREDO_NON_NIHIL(pet);
    CREDO_CHORDA_AEQUALIS_LITERIS(http_petitio_visus(pet).hospes,
        "situla.nyc3.digitaloceanspaces.com");
    CREDO_VERUM(http_petitio_est_https(pet));
    CREDO_VERUM(_signatura_verificata(pet, _l("nyc3", p), p));

    h = _horreum("r2", NIHIL, NIHIL, p);
    CREDO_NON_NIHIL(h);
    CREDO_VERUM(horreum_opus_inspicere(h, _l("situla", p), _l("a.txt",
        p),
        &o, &e, p));
    pet = _signare(h, &o, p);
    CREDO_CHORDA_AEQUALIS_LITERIS(http_petitio_visus(pet).hospes,
        "situla.abc123.r2.cloudflarestorage.com");
    CREDO_VERUM(_signatura_verificata(pet, _l("auto", p), p));

    h = _horreum("r2", "https://abc123.eu.r2.cloudflarestorage.com",
        NIHIL, p);
    CREDO_NON_NIHIL(h);
    CREDO_VERUM(horreum_opus_inspicere(h, _l("situla", p), _l("a.txt",
        p),
        &o, &e, p));
    pet = _signare(h, &o, p);
    CREDO_CHORDA_AEQUALIS_LITERIS(http_petitio_visus(pet).hospes,
        "situla.abc123.eu.r2.cloudflarestorage.com");

    /* DO sine regione; generic sine termino; http pro DO */
    memset(&cfg, ZEPHYRUM, magnitudo(cfg));
    cfg.provisor   = "digitalocean";
    cfg.clavis_id  = _l("AKIDEXAMPLE", p);
    cfg.secretum   = _l(SECRETUM, p);
    CREDO_NIHIL(horreum_creare(&cfg, &e, p));
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)HORREUM_MALUM);
    CREDO_CHORDA_CONTINET(e.nuntius, _l("regio", p));
    cfg.provisor = "generic";
    CREDO_NIHIL(horreum_creare(&cfg, &e, p));
    CREDO_CHORDA_CONTINET(e.nuntius, _l("terminus", p));
    cfg.provisor  = "digitalocean";
    cfg.regio     = _l("nyc3", p);
    cfg.terminus  = _l("http://nyc3.digitaloceanspaces.com", p);
    CREDO_NIHIL(horreum_creare(&cfg, &e, p));
    CREDO_CHORDA_CONTINET(e.nuntius, _l("http", p));
    cfg.provisor = "generic";
    cfg.terminus = _l("http://127.0.0.1:9000", p);
    CREDO_NON_NIHIL(horreum_creare(&cfg, &e, p));
    cfg.secretum = _l("", p);
    CREDO_NIHIL(horreum_creare(&cfg, &e, p));
    CREDO_CHORDA_CONTINET(e.nuntius, _l("secretum", p));
}

/* ===== stilus ===== */

interior vacuum
_sectio_stilus (
    CredoContextus* c)
{
         Piscina* p = c->piscina;
         Horreum* h = _horreum("digitalocean", NIHIL, "nyc3", p);
         Horreum* g = _horreum("generic", "http://127.0.0.1:9000",
             "us-east-1",
                         p);
         HorreumOpus  o;
        HorreumError  e;
         HttpPetitio* pet;
    HttpPetitioVisus  v;

    CREDO_VERUM(horreum_opus_inspicere(h, _l("situla", p), _l("a.txt",
        p),
        &o, &e, p));
    v = http_petitio_visus(_signare(h, &o, p));
    CREDO_CHORDA_AEQUALIS_LITERIS(v.via, "/a.txt");

    CREDO_VERUM(horreum_opus_inspicere(g, _l("situla", p), _l("a.txt",
        p),
        &o, &e, p));
    pet  = _signare(g, &o, p);
    v    = http_petitio_visus(pet);
    CREDO_CHORDA_AEQUALIS_LITERIS(v.hospes, "127.0.0.1");
    CREDO_AEQUALIS_I32(v.portus, IX * M);
    CREDO_CHORDA_AEQUALIS_LITERIS(v.via, "/situla/a.txt");
    CREDO_CHORDA_AEQUALIS_LITERIS(_caput_visus(&v, "Host", p),
        "127.0.0.1:9000");
    CREDO_FALSUM(http_petitio_est_https(pet));
    CREDO_VERUM(_signatura_verificata(pet, _l("us-east-1", p), p));

    /* situla cum puncto: DO (HOSPES, TLS) recusat, generic (VIA) accipit */
    CREDO_FALSUM(horreum_opus_inspicere(h, _l("a.b", p), _l("x", p), &o,
        &e,
        p));
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)HORREUM_MALUM);
    CREDO_CHORDA_CONTINET(e.nuntius, _l("DNS", p));
    CREDO_VERUM(horreum_opus_inspicere(g, _l("a.b", p), _l("x", p), &o,
        &e,
        p));
    CREDO_FALSUM(horreum_opus_inspicere(h, _l("AB", p), _l("x", p), &o,
        &e,
        p));
    CREDO_FALSUM(horreum_opus_inspicere(g, _l("ab", p), _l("x", p), &o,
        &e,
        p));
    CREDO_FALSUM(horreum_opus_inspicere(g, _l("-ab", p), _l("x", p), &o,
        &e,
        p));
}

/* ===== via_clavis (review focus 1) ===== */

interior vacuum
_sectio_via_clavis (
    CredoContextus* c)
{
    nomen structura {
        constans character* clavis;
        constans character* via;
    } Casus;
    hic_manens constans Casus casus_clavium[] = {
        { "a b+c%d?e#f&g", "/situla/a%20b%2Bc%25d%3Fe%23f%26g" },
        { "\xE1\x88\xB4/x", "/situla/%E1%88%B4/x" },
        { "dir//x", "/situla/dir//x" },
        { "dir/", "/situla/dir/" },
        { "./a/../b", "/situla/./a/../b" },
        { NIHIL, NIHIL }
    };
         Piscina* p = c->piscina;
         Horreum* g = _horreum("generic", "http://127.0.0.1:9000",
             "us-east-1",
                         p);
     HorreumOpus o;
    HorreumError e;
             i32 k;

    per (k = ZEPHYRUM; casus_clavium[k].clavis; k++)
    {
        HttpPetitio* pet;

        CREDO_VERUM(horreum_opus_capere(g, _l("situla", p),
            _l(casus_clavium[k].clavis, p), NIHIL, &o, &e, p));
        pet = _signare(g, &o, p);
        CREDO_NON_NIHIL(pet);
        CREDO_CHORDA_AEQUALIS_LITERIS(http_petitio_visus(pet).via,
            casus_clavium[k].via);
        CREDO_VERUM(_signatura_verificata(pet, _l("us-east-1", p), p));
    }
}

/* ===== ponere_capita (review focus 2) ===== */

interior vacuum
_sectio_ponere_capita (
    CredoContextus* c)
{
             Piscina* p = c->piscina;
             Horreum* d = _horreum("digitalocean", NIHIL, "nyc3", p);
             Horreum* r = _horreum("r2", NIHIL, NIHIL, p);
         HorreumOpus  o;
        HorreumError  e;
      HorreumPonendi  ponendi;
         HorreumMeta  meta[II];
         HttpPetitio* pet;
    HttpPetitioVisus  v;

    meta[ZEPHYRUM].titulus  = _l("auctor", p);
    meta[ZEPHYRUM].valor    = _l("Franciscus", p);
    meta[I].titulus         = _l("genus", p);
    meta[I].valor           = _l("probatio", p);
    memset(&ponendi, ZEPHYRUM, magnitudo(ponendi));
    ponendi.genus_contenti  = _l("text/plain", p);
    ponendi.cache_control   = _l("max-age=60", p);
    ponendi.meta            = meta;
    ponendi.meta_numerus    = II;
    ponendi.acl             = HORREUM_ACL_PUBLICA_LECTIO;
    CREDO_VERUM(horreum_opus_ponere(d, _l("situla", p), _l("a.txt", p),
        _l("salve", p), &ponendi, &o, &e, p));
    pet  = _signare(d, &o, p);
    v    = http_petitio_visus(pet);
    CREDO_CHORDA_AEQUALIS_LITERIS(_caput_visus(&v, "Content-Type", p),
        "text/plain");
    CREDO_CHORDA_AEQUALIS_LITERIS(_caput_visus(&v, "Cache-Control", p),
        "max-age=60");
    CREDO_CHORDA_AEQUALIS_LITERIS(_caput_visus(&v, "x-amz-meta-auctor",
        p),
        "Franciscus");
    CREDO_CHORDA_AEQUALIS_LITERIS(_caput_visus(&v, "x-amz-meta-genus",
        p),
        "probatio");
    CREDO_CHORDA_AEQUALIS_LITERIS(_caput_visus(&v, "x-amz-acl", p),
        "public-read");
    CREDO_CHORDA_AEQUALIS_LITERIS(_caput_visus(&v,
        "x-amz-content-sha256", p),
        "f9afe0ca8d925f21494d6832ca2b3d7841caaed2df478889f8478cd750aaeb34");
    CREDO_CHORDA_VACUA(_caput_visus(&v, "x-amz-meta-sha256", p));
    CREDO_CHORDA_VACUA(_caput_visus(&v, "If-None-Match", p));
    CREDO_CHORDA_AEQUALIS_LITERIS(v.corpus, "salve");
    CREDO_VERUM(_signatura_verificata(pet, _l("nyc3", p), p));
    CREDO_VERUM(_petitio_tacita(pet, p));

    /* R2: condicionalis cum periculo, checksum CRC32 */
    memset(&ponendi, ZEPHYRUM, magnitudo(ponendi));
    ponendi.solum_si_abest      = VERUM;
    ponendi.periculum_accipere  = VERUM;
    ponendi.checksum            = HORREUM_CHECKSUM_CRC32;
    CREDO_VERUM(horreum_opus_ponere(r, _l("situla", p), _l("a.txt", p),
        _l("salve", p), &ponendi, &o, &e, p));
    CREDO_VERUM(o.condicionale);
    pet  = _signare(r, &o, p);
    v    = http_petitio_visus(pet);
    CREDO_CHORDA_AEQUALIS_LITERIS(_caput_visus(&v, "If-None-Match", p),
        "*");
    CREDO_CHORDA_AEQUALIS_LITERIS(_caput_visus(&v, "x-amz-meta-sha256",
        p),
        "f9afe0ca8d925f21494d6832ca2b3d7841caaed2df478889f8478cd750aaeb34");
    CREDO_CHORDA_AEQUALIS_LITERIS(_caput_visus(&v,
        "x-amz-checksum-crc32", p),
        "/rqEhw==");
    CREDO_VERUM(_signatura_verificata(pet, _l("auto", p), p));

    memset(&ponendi, ZEPHYRUM, magnitudo(ponendi));
    ponendi.solum_si_etag  = _l("0cc175b9c0f1b6a831c399e269772661", p);
    ponendi.checksum       = HORREUM_CHECKSUM_SHA256;
    CREDO_VERUM(horreum_opus_ponere(r, _l("situla", p), _l("a.txt", p),
        _l("salve", p), &ponendi, &o, &e, p));
    v = http_petitio_visus(_signare(r, &o, p));
    CREDO_CHORDA_AEQUALIS_LITERIS(_caput_visus(&v, "If-Match", p),
        "\"0cc175b9c0f1b6a831c399e269772661\"");
    CREDO_CHORDA_NON_VACUA(_caput_visus(&v, "x-amz-meta-sha256", p));
    CREDO_CHORDA_AEQUALIS_LITERIS(_caput_visus(&v,
        "x-amz-checksum-sha256",
        p), "+a/gyo2SXyFJTWgyyis9eEHKrtLfR4iJ+EeM11Cq6zQ=");

    /* corpus vacuum: Content-Length 0, SHA-256 vacui */
    CREDO_VERUM(horreum_opus_ponere(d, _l("situla", p), _l("dir/", p),
        _l("", p), NIHIL, &o, &e, p));
    pet  = _signare(d, &o, p);
    v    = http_petitio_visus(pet);
    CREDO_CHORDA_AEQUALIS_LITERIS(_caput_visus(&v, "Content-Length", p),
        "0");
    CREDO_CHORDA_AEQUALIS_LITERIS(_caput_visus(&v,
        "x-amz-content-sha256", p),
        "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
    CREDO_VERUM(_signatura_verificata(pet, _l("nyc3", p), p));
}

/* ===== incapax ===== */

interior vacuum
_sectio_incapax (
    CredoContextus* c)
{
          Piscina* p = c->piscina;
          Horreum* d = _horreum("digitalocean", NIHIL, "nyc3", p);
          Horreum* r = _horreum("r2", NIHIL, NIHIL, p);
          Horreum* b = _horreum("b2", NIHIL, "us-west-004", p);
      HorreumOpus  o;
      HorreumOpus  intactum;
     HorreumError  e;
   HorreumPonendi  ponendi;
           chorda  claves[I];

    memset(&o, 0x5A, magnitudo(o));
    memset(&intactum, 0x5A, magnitudo(intactum));
    memset(&ponendi, ZEPHYRUM, magnitudo(ponendi));
    ponendi.acl = HORREUM_ACL_PUBLICA_LECTIO;
    CREDO_FALSUM(horreum_opus_ponere(r, _l("situla", p), _l("a", p),
        _l("x", p), &ponendi, &o, &e, p));
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)HORREUM_INCAPAX);
    CREDO_VERUM(memcmp(&o, &intactum, magnitudo(o)) == ZEPHYRUM);

    memset(&ponendi, ZEPHYRUM, magnitudo(ponendi));
    ponendi.solum_si_abest = VERUM;
    CREDO_FALSUM(horreum_opus_ponere(r, _l("situla", p), _l("a", p),
        _l("x", p), &ponendi, &o, &e, p));
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)HORREUM_INCAPAX);
    CREDO_CHORDA_CONTINET(e.nuntius, _l("probat", p));
    ponendi.periculum_accipere = VERUM;
    CREDO_VERUM(horreum_opus_ponere(r, _l("situla", p), _l("a", p),
        _l("x", p), &ponendi, &o, &e, p));

    memset(&ponendi, ZEPHYRUM, magnitudo(ponendi));
    ponendi.checksum = HORREUM_CHECKSUM_CRC32;
    CREDO_FALSUM(horreum_opus_ponere(d, _l("situla", p), _l("a", p),
        _l("x", p), &ponendi, &o, &e, p));
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)HORREUM_INCAPAX);

    claves[ZEPHYRUM] = _l("a", p);
    CREDO_VERUM(horreum_opus_delere_plura(d, _l("situla", p), claves, I,
        &o,
        &e, p));
    memset(&ponendi, ZEPHYRUM, magnitudo(ponendi));
    ponendi.solum_si_etag = _l("e", p);
    CREDO_FALSUM(horreum_opus_ponere(b, _l("situla", p), _l("a", p),
        _l("x", p), &ponendi, &o, &e, p));
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)HORREUM_INCAPAX);
}

/* ===== delere_plura_corpus ===== */

interior vacuum
_sectio_delere_plura_corpus (
    CredoContextus* c)
{
             Piscina* p = c->piscina;
             Horreum* d = _horreum("digitalocean", NIHIL, "nyc3", p);
         HorreumOpus  o;
        HorreumError  e;
              chorda* claves;
                 i32  k;
                  i8  md5[MD5_DIGEST_MENSURA];
         HttpPetitio* pet;
    HttpPetitioVisus  v;

    claves = (chorda*)piscina_allocare(p,
                 (memoriae_index)((M + I) * (i32)magnitudo(chorda)));
    claves[ZEPHYRUM]  = _l("a", p);
    claves[I]         = _l("a&b", p);
    claves[II]        = _l("c", p);
    CREDO_VERUM(horreum_opus_delere_plura(d, _l("situla", p), claves,
        III, &o,
        &e, p));
    CREDO_CHORDA_AEQUALIS_LITERIS(o.corpus,
        "<Delete><Quiet>false</Quiet><Object><Key>a</Key></Object>"
        "<Object><Key>a&amp;b</Key></Object><Object><Key>c</Key></Object>"
        "</Delete>");
    md5_friare(o.corpus.datum, o.corpus.mensura, md5);
    CREDO_CHORDA_AEQUALIS(_caput_operis(&o, "Content-MD5", p),
        base64_codificare(md5, MD5_DIGEST_MENSURA, p));
    CREDO_CHORDA_AEQUALIS_LITERIS(_caput_operis(&o, "Content-MD5", p),
        "lTuNe16gOfvsqCAJtE8Bjw==");
    pet  = _signare(d, &o, p);
    v    = http_petitio_visus(pet);
    CREDO_AEQUALIS_I32((i32)v.methodus, (i32)HTTP_POST);
    CREDO_CHORDA_AEQUALIS_LITERIS(v.via, "/?delete=");
    CREDO_VERUM(_signatura_verificata(pet, _l("nyc3", p), p));

    per (k = ZEPHYRUM; k <= M; k++)
    {
        claves[k] = _l("k", p);
    }
    CREDO_FALSUM(horreum_opus_delere_plura(d, _l("situla", p), claves, M
        + I,
        &o, &e, p));
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)HORREUM_MALUM);
    CREDO_FALSUM(horreum_opus_delere_plura(d, _l("situla", p), claves,
        ZEPHYRUM, &o, &e, p));
}

/* ===== enumerare_quaestio ===== */

interior vacuum
_sectio_enumerare_quaestio (
    CredoContextus* c)
{
            Piscina* p = c->piscina;
            Horreum* d = _horreum("digitalocean", NIHIL, "nyc3", p);
        HorreumOpus  o;
       HorreumError  e;
  HorreumEnumerandi  en;
        HttpPetitio* pet;

    memset(&en, ZEPHYRUM, magnitudo(en));
    en.praefixum              = _l("dir/", p);
    en.delimitator            = _l("/", p);
    en.signum_continuationis  = _l("T/X=", p);
    CREDO_VERUM(horreum_opus_enumerare(d, _l("situla", p), &en, &o, &e,
        p));
    pet = _signare(d, &o, p);
    CREDO_CHORDA_AEQUALIS_LITERIS(http_petitio_visus(pet).via,
        "/?continuation-token=T%2FX%3D&delimiter=%2F&list-type=2"
        "&max-keys=1000&prefix=dir%2F");
    CREDO_VERUM(_signatura_verificata(pet, _l("nyc3", p), p));

    memset(&en, ZEPHYRUM, magnitudo(en));
    en.magnitudo_paginae = L;
    CREDO_VERUM(horreum_opus_enumerare(d, _l("situla", p), &en, &o, &e,
        p));
    CREDO_CHORDA_AEQUALIS_LITERIS(http_petitio_visus(_signare(d, &o,
        p)).via,
        "/?list-type=2&max-keys=50");
}

/* ===== praesignare ===== */

interior vacuum
_sectio_praesignare (
    CredoContextus* c)
{
              Piscina* p = c->piscina;
              Horreum* r = _horreum("r2", NIHIL, NIHIL, p);
         HorreumError  e;
               chorda  u;
               chorda  praefixum;
                  s32  i;
     SignaturaPetitio  sp;
    SignaturaResultus  sr;

    u = horreum_praesignare(r, HTTP_GET, _l("situla", p), _l("a b.txt",
        p),
            MMMDC, _l("", p), TEMPUS, &e, p);
    praefixum =
        _l("https://situla.abc123.r2.cloudflarestorage.com/a%20b.txt"
        "?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Credential=AKIDEXAMPLE%2F"
        "20150830%2Fauto%2Fs3%2Faws4_request&X-Amz-Date=20150830T123600Z"
        "&X-Amz-Expires=3600&X-Amz-SignedHeaders=host&X-Amz-Signature=",
        p);
    CREDO_VERUM(chorda_incipit(u, praefixum));
    CREDO_AEQUALIS_I32(u.mensura, praefixum.mensura + LXIV);
    CREDO_FALSUM(chorda_continet(u, _l(SECRETUM, p)));

    /* eadem signatura ex UNSIGNED-PAYLOAD recomputata */
    memset(&sp, ZEPHYRUM, magnitudo(sp));
    sp.methodus = "GET";
    sp.hospes = _l("situla.abc123.r2.cloudflarestorage.com",
        p);
    sp.via              = _l("/a b.txt", p);
    sp.corpus_sigillum  = _l("UNSIGNED-PAYLOAD", p);
    sp.tempus           = TEMPUS;
    sp.regio            = _l("auto", p);
    sp.servitium        = _l("s3", p);
    sp.clavis_id        = _l("AKIDEXAMPLE", p);
    sp.secretum         = _l(SECRETUM, p);
    sp.duratio          = MMMDC;
    sr                  = signatura_aws_quaestione(&sp, p);
    CREDO_VERUM(sr.successus);
    i = chorda_invenire_index(u, _l("X-Amz-Signature=", p));
    CREDO_CHORDA_AEQUALIS(chorda_sectio(u, (i32)i + XVI, u.mensura),
        sr.signatura);

    u = horreum_praesignare(r, HTTP_PUT, _l("situla", p), _l("a.txt",
        p),
            LX, _l("image/png", p), TEMPUS, &e, p);
    CREDO_CHORDA_CONTINET(u,
        _l("X-Amz-SignedHeaders=content-type%3Bhost",
        p));
    u = horreum_praesignare(r, HTTP_GET, _l("situla", p), _l("a", p),
            ZEPHYRUM, _l("", p), TEMPUS, &e, p);
    CREDO_AEQUALIS_I32(u.mensura, ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)HORREUM_MALUM);
    u = horreum_praesignare(r, HTTP_GET, _l("situla", p), _l("a", p),
            SIGNATURA_AWS_DURATIO_MAXIMA + I, _l("", p), TEMPUS, &e, p);
    CREDO_AEQUALIS_I32(u.mensura, ZEPHYRUM);
}

/* ===== tempora (P7) ===== */

interior vacuum
_sectio_tempora (
    CredoContextus* c)
{
    Piscina* p = c->piscina;

    CREDO_AEQUALIS_S64((s64)horreum_tempus_iso(
        _l("2026-10-21T14:09:00.000Z", p)), 1792591740LL);
    CREDO_AEQUALIS_S64((s64)horreum_tempus_iso(
        _l("2026-10-21T14:09:00Z", p)), 1792591740LL);
    CREDO_AEQUALIS_S64((s64)horreum_tempus_iso(
        _l("2026-10-21T14:09:02.123Z", p)), 1792591742LL);
    CREDO_AEQUALIS_S64((s64)horreum_tempus_http(
        _l("Wed, 21 Oct 2026 14:09:00 GMT", p)), 1792591740LL);
    CREDO_AEQUALIS_S64((s64)horreum_tempus_http(
        _l("Sun, 30 Aug 2015 12:36:00 GMT", p)), (s64)TEMPUS);
    CREDO_AEQUALIS_S64((s64)horreum_tempus_iso(_l("heri", p)), 0LL);
    CREDO_AEQUALIS_S64((s64)horreum_tempus_http(_l("Wed, 99 Foo", p)),
        0LL);
}

/* ===== responsa ===== */

interior chorda
_fixum (
    constans character* titulus,
               Piscina* p)
{
    character via[CCLVI];

    sprintf(via, "%s/%s", VIA_RESPONSORUM, titulus);
    redde filum_legere_totum(via, p);
}

/* resultus fictus: status, corpus, capita "Titulus", "valor", ... */
interior HttpResultus
_resultus (
                   i32   status,
                chorda   corpus,
    constans character** capita,
                   i32   numerus,
               Piscina*  p)
{
     HttpResultus  r;
    HttpResponsum* s = (HttpResponsum*)piscina_allocare(p,
                           (memoriae_index)magnitudo(HttpResponsum));
              i32 i;

    memset(&r, ZEPHYRUM, magnitudo(r));
    memset(s, ZEPHYRUM, magnitudo(*s));
    s->status = status;
    s->corpus = corpus;
    s->capita = (HttpCaput*)piscina_allocare(p,
        (memoriae_index)((numerus + I) * (i32)magnitudo(HttpCaput)));
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        s->capita[i].titulus  = _l(capita[i * II], p);
        s->capita[i].valor    = _l(capita[i * II + I], p);
    }
    s->capita_numerus  = numerus;
    r.successus        = VERUM;
    r.responsum        = s;
    redde r;
}

interior vacuum
_sectio_responsa_errorum (
    CredoContextus* c)
{
               Piscina* p = c->piscina;
               Horreum* d = _horreum("digitalocean", NIHIL, "nyc3",
                   p);
           HorreumOpus  o;
          HorreumError  e;
       HorreumObjectum  ob;
          HttpResultus  res;
    constans character* cap_id[] = { "x-amz-request-id", "HDR-ID" };
    constans character* cap_dies[] = { "Date",
                                       "Sun, 30 Aug 2015 12:56:00 GMT" };

    CREDO_VERUM(horreum_opus_capere(d, _l("situla", p), _l("nulla.txt",
        p),
        NIHIL, &o, &e, p));

    res = _resultus(CDIV, _fixum("error_nosuchkey.xml", p), NIHIL,
        ZEPHYRUM,
              p);
    CREDO_FALSUM(horreum_responsum_legere(&o, res, TEMPUS, &ob, &e, p));
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)HORREUM_ABEST);
    CREDO_AEQUALIS_I32(e.status, CDIV);
    CREDO_CHORDA_AEQUALIS_LITERIS(e.codex, "NoSuchKey");
    CREDO_CHORDA_AEQUALIS_LITERIS(e.nuntius,
        "The specified key does not exist.");
    CREDO_CHORDA_AEQUALIS_LITERIS(e.petitio_id, "4442587FB7D0A2F9");

    res = _resultus(CDIV, _fixum("error_nosuchkey.xml", p), cap_id, I,
        p);
    CREDO_FALSUM(horreum_responsum_legere(&o, res, TEMPUS, &ob, &e, p));
    CREDO_CHORDA_AEQUALIS_LITERIS(e.petitio_id, "HDR-ID");

    res = _resultus(CDXII, _fixum("error_precondition.xml", p), NIHIL,
              ZEPHYRUM, p);
    CREDO_FALSUM(horreum_responsum_legere(&o, res, TEMPUS, &ob, &e, p));
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)HORREUM_CONDITIO);

    res = _resultus(CDIII, _fixum("error_signature.xml", p), NIHIL,
        ZEPHYRUM,
              p);
    CREDO_FALSUM(horreum_responsum_legere(&o, res, TEMPUS, &ob, &e, p));
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)HORREUM_VETITUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(e.codex, "SignatureDoesNotMatch");

    res = _resultus(DIII, _fixum("error_slowdown.xml", p), NIHIL,
        ZEPHYRUM,
              p);
    CREDO_FALSUM(horreum_responsum_legere(&o, res, TEMPUS, &ob, &e, p));
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)HORREUM_TRANSITORIUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(e.codex, "SlowDown");

    res = _resultus(CDIII, _fixum("error_skewed.xml", p), cap_dies, I,
        p);
    CREDO_FALSUM(horreum_responsum_legere(&o, res, TEMPUS, &ob, &e, p));
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)HORREUM_TRANSITORIUM);
    CREDO_AEQUALIS_S64(e.deviatio_s, (s64)MCC);

    /* HEAD 404 sine corpore */
    CREDO_VERUM(horreum_opus_inspicere(d, _l("situla", p), _l("nulla",
        p),
        &o, &e, p));
    res = _resultus(CDIV, _l("", p), NIHIL, ZEPHYRUM, p);
    CREDO_FALSUM(horreum_responsum_legere(&o, res, TEMPUS, &ob, &e, p));
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)HORREUM_ABEST);

    /* rete; corpus nimium (P1) */
    memset(&res, ZEPHYRUM, magnitudo(res));
    res.successus         = FALSUM;
    res.error             = HTTP_ERROR_CONNEXIO;
    res.error_descriptio  = _l("connexio recusata", p);
    CREDO_FALSUM(horreum_responsum_legere(&o, res, TEMPUS, &ob, &e, p));
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)HORREUM_TRANSITORIUM);
    CREDO_AEQUALIS_I32(e.status, ZEPHYRUM);
    res.error             = HTTP_ERROR_PARSE;
    res.error_descriptio  = _l("corpus nimium", p);
    CREDO_FALSUM(horreum_responsum_legere(&o, res, TEMPUS, &ob, &e, p));
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)HORREUM_MALUM);
    CREDO_CHORDA_CONTINET(e.nuntius, _l("horreum_capere_in_filum", p));
}

interior vacuum
_sectio_objectum_ex_capitibus (
    CredoContextus* c)
{
               Piscina* p = c->piscina;
               Horreum* d = _horreum("digitalocean", NIHIL, "nyc3",
                   p);
           HorreumOpus  o;
          HorreumError  e;
       HorreumObjectum  ob;
    constans character* capita[] = {
        "ETag", "\"abc\"",
        "Content-Length", "42",
        "Last-Modified", "Wed, 21 Oct 2026 14:09:00 GMT",
        "Content-Type", "text/plain",
        "x-amz-version-id", "v1",
        "x-amz-meta-Auctor", "x",
        "x-amz-checksum-crc32", "/rqEhw=="
    };

    CREDO_VERUM(horreum_opus_inspicere(d, _l("situla", p), _l("a.txt",
        p),
        &o, &e, p));
    CREDO_VERUM(horreum_responsum_legere(&o, _resultus(CC, _l("", p),
        capita,
        VII, p), TEMPUS, &ob, &e, p));
    CREDO_CHORDA_AEQUALIS_LITERIS(ob.clavis, "a.txt");
    CREDO_CHORDA_AEQUALIS_LITERIS(ob.etag, "abc");
    CREDO_AEQUALIS_S64((s64)ob.mensura, XLII);
    CREDO_AEQUALIS_S64((s64)ob.mutatum, 1792591740LL);
    CREDO_CHORDA_AEQUALIS_LITERIS(ob.genus_contenti, "text/plain");
    CREDO_CHORDA_AEQUALIS_LITERIS(ob.versio, "v1");
    CREDO_CHORDA_AEQUALIS_LITERIS(ob.checksum_crc32, "/rqEhw==");
    CREDO_AEQUALIS_I32(ob.meta_numerus, I);
    CREDO_CHORDA_AEQUALIS_LITERIS(ob.meta[ZEPHYRUM].titulus, "auctor");
    CREDO_CHORDA_AEQUALIS_LITERIS(ob.meta[ZEPHYRUM].valor, "x");
}

/* ===== enumeratio_legere (review focus 3) ===== */

interior vacuum
_sectio_enumeratio_legere (
    CredoContextus* c)
{
          Piscina* p = c->piscina;
    HorreumPagina  pg;
     HorreumError  e;

    CREDO_VERUM(horreum_paginam_legere(_fixum("enumeratio.xml", p), &pg,
        &e,
        p));
    CREDO_AEQUALIS_I32(pg.obiecta_numerus, III);
    CREDO_CHORDA_AEQUALIS_LITERIS(pg.obiecta[ZEPHYRUM].clavis,
        "dir/a.txt");
    CREDO_CHORDA_AEQUALIS_LITERIS(pg.obiecta[ZEPHYRUM].etag,
        "0cc175b9c0f1b6a831c399e269772661");
    CREDO_AEQUALIS_S64((s64)pg.obiecta[ZEPHYRUM].mensura, I);
    CREDO_AEQUALIS_S64((s64)pg.obiecta[ZEPHYRUM].mutatum, 1792591740LL);
    CREDO_CHORDA_AEQUALIS_LITERIS(pg.obiecta[I].clavis,
        "dir/a&b<c>\"d'");
    CREDO_AEQUALIS_S64((s64)pg.obiecta[I].mensura, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(pg.obiecta[II].clavis,
        "dir/\xE1\x88\xB4\rx");
    CREDO_CHORDA_AEQUALIS_LITERIS(pg.obiecta[II].etag,
        "9b2cf535f27731c974343645a3985328-3");
    CREDO_AEQUALIS_S64((s64)pg.obiecta[II].mensura, 11534336LL);
    CREDO_AEQUALIS_S64((s64)pg.obiecta[II].mutatum, 1792591742LL);
    CREDO_AEQUALIS_I32(pg.praefixa_numerus, II);
    CREDO_CHORDA_AEQUALIS_LITERIS(pg.praefixa_communia[ZEPHYRUM],
        "dir/sub/");
    CREDO_CHORDA_AEQUALIS_LITERIS(pg.praefixa_communia[I], "dir/alia/");
    CREDO_VERUM(pg.truncata);
    CREDO_CHORDA_AEQUALIS_LITERIS(pg.signum_proximum,
        "1ueGcxLPRx1Tr/XYExHnhbYLgveDs2J/wm36Hy4vbOwM=");

    CREDO_VERUM(horreum_paginam_legere(_fixum("enumeratio_ultima.xml",
        p),
        &pg, &e, p));
    CREDO_AEQUALIS_I32(pg.obiecta_numerus, ZEPHYRUM);
    CREDO_FALSUM(pg.truncata);
    CREDO_CHORDA_VACUA(pg.signum_proximum);

    CREDO_FALSUM(horreum_paginam_legere(_l("<non", p), &pg, &e, p));
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)HORREUM_MALUM);
}

interior vacuum
_sectio_partes_legere (
    CredoContextus* c)
{
              Piscina* p = c->piscina;
         HorreumError  e;
               chorda  t;
          HorreumPars* partes;
   HorreumOnusPendens* pend;
       HorreumDeletio* del;
                  i32  n;

    CREDO_VERUM(horreum_id_oneris_legere(_fixum("multipars_initium.xml",
        p),
        &t, &e, p));
    CREDO_CHORDA_AEQUALIS_LITERIS(t,
        "VXBsb2FkIElEIGZvciA2aWWpbmcncyBteS1tb3ZpZS5tMnRzIHVwbG9hZA");
    CREDO_VERUM(horreum_etag_perfecti_legere(_fixum("multipars_finis.xml",
        p),
        &t, &e, p));
    CREDO_CHORDA_AEQUALIS_LITERIS(t,
        "3858f62230ac3c915f300c664312c11f-9");
    CREDO_FALSUM(horreum_etag_perfecti_legere(
        _fixum("multipars_finis_error.xml", p), &t, &e, p));
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)HORREUM_TRANSITORIUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(e.codex, "InternalError");

    CREDO_VERUM(horreum_partes_legere(_fixum("multipars_partes.xml", p),
        &partes, &n, &e, p));
    CREDO_AEQUALIS_I32(n, II);
    CREDO_AEQUALIS_I32(partes[ZEPHYRUM].numerus, I);
    CREDO_CHORDA_AEQUALIS_LITERIS(partes[ZEPHYRUM].etag,
        "7778aef83f66abc1fa1e8477f296d394");
    CREDO_AEQUALIS_S64((s64)partes[ZEPHYRUM].mensura, 5242880LL);
    CREDO_AEQUALIS_I32(partes[I].numerus, II);

    CREDO_VERUM(horreum_pendentes_legere(
        _fixum("multipartes_pendentes.xml", p), &pend, &n, &e, p));
    CREDO_AEQUALIS_I32(n, II);
    CREDO_CHORDA_AEQUALIS_LITERIS(pend[ZEPHYRUM].clavis, "x/unum");
    CREDO_CHORDA_AEQUALIS_LITERIS(pend[ZEPHYRUM].id_oneris, "ID-UNUM");
    CREDO_AEQUALIS_S64((s64)pend[ZEPHYRUM].initium, 1792490400LL);
    CREDO_CHORDA_AEQUALIS_LITERIS(pend[I].id_oneris, "ID-DUO");

    CREDO_VERUM(horreum_deletiones_legere(_fixum("delere_plura.xml", p),
        &del,
        &n, &e, p));
    CREDO_AEQUALIS_I32(n, III);
    CREDO_CHORDA_AEQUALIS_LITERIS(del[ZEPHYRUM].clavis, "a");
    CREDO_VERUM(del[ZEPHYRUM].deletum);
    CREDO_CHORDA_AEQUALIS_LITERIS(del[I].clavis, "b&c");
    CREDO_FALSUM(del[I].deletum);
    CREDO_AEQUALIS_I32((i32)del[I].error.genus, (i32)HORREUM_VETITUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(del[I].error.codex, "AccessDenied");
    CREDO_VERUM(del[II].deletum);
}

/* ===== iterandum ===== */

interior vacuum
_sectio_iterandum (
    CredoContextus* c)
{
         Piscina* p = c->piscina;
         Horreum* d = _horreum("digitalocean", NIHIL, "nyc3", p);
     HorreumOpus  o;
    HorreumError  e;
             i32  mora;
          chorda  nulla = _l("", p);

    CREDO_VERUM(horreum_opus_inspicere(d, _l("situla", p), _l("a", p),
        &o, &e,
        p));
    memset(&e, ZEPHYRUM, magnitudo(e));
    e.genus = HORREUM_TRANSITORIUM;
    CREDO_VERUM(horreum_iterandum(&o, &e, nulla, I, III, C, ZEPHYRUM,
        &mora));
    CREDO_AEQUALIS_I32(mora, C);
    CREDO_VERUM(horreum_iterandum(&o, &e, nulla, I, III, C,
        (i32)0xFFFFFFFFU,
        &mora));
    CREDO_VERUM(mora >= C && mora < CC);
    CREDO_VERUM(horreum_iterandum(&o, &e, nulla, II, III, C, (i32)12345,
        &mora));
    CREDO_VERUM(mora >= CC && mora < CD);
    CREDO_FALSUM(horreum_iterandum(&o, &e, nulla, III, III, C, ZEPHYRUM,
        &mora));
    CREDO_VERUM(horreum_iterandum(&o, &e, _l("2", p), I, III, C,
        ZEPHYRUM,
        &mora));
    CREDO_VERUM(mora >= MM);
    e.genus = HORREUM_ABEST;
    CREDO_FALSUM(horreum_iterandum(&o, &e, nulla, I, III, C, ZEPHYRUM,
        &mora));
    e.genus = HORREUM_VETITUM;
    CREDO_FALSUM(horreum_iterandum(&o, &e, nulla, I, III, C, ZEPHYRUM,
        &mora));
    e.genus = HORREUM_MALUM;
    CREDO_FALSUM(horreum_iterandum(&o, &e, nulla, I, III, C, ZEPHYRUM,
        &mora));
    e.genus = HORREUM_INCAPAX;
    CREDO_FALSUM(horreum_iterandum(&o, &e, nulla, I, III, C, ZEPHYRUM,
        &mora));
    e.genus = HORREUM_CONDITIO;
    CREDO_FALSUM(horreum_iterandum(&o, &e, nulla, I, III, C, ZEPHYRUM,
        &mora));
}

interior vacuum
_sectio_partis_mensura (
    CredoContextus* c)
{
    i64 mib = (i64)HORREUM_MIB;

    (vacuum)c;
    CREDO_AEQUALIS_S64((s64)horreum_partis_mensura(X * mib, ZEPHYRUM),
        (s64)(XVI * mib));
    CREDO_AEQUALIS_S64((s64)horreum_partis_mensura(MXXIV * mib,
        IV * mib),
        (s64)(V * mib));
    CREDO_AEQUALIS_S64((s64)horreum_partis_mensura(200000LL * mib,
        XVI * mib), (s64)(XX * mib));
    CREDO_AEQUALIS_S64((s64)horreum_partis_mensura(200001LL * mib,
        XVI * mib), (s64)(XXI * mib));
    CREDO_AEQUALIS_S64((s64)horreum_partis_mensura(XVII * mib + I,
        (i64)(VII * mib + I)), (s64)(VIII * mib));
}

/* ===== tabella_vitia (review focus 5) ===== */

interior constans character*
_tabella (
     constans character* contentum,
                integer  modus,
                    i32  n,
                Piscina* p)
{
     character* via = (character*)piscina_allocare(p, CXXVIII);
     character  dir[XCVI];

    sprintf(dir, "/tmp/horreum-%d-%d", (integer)getpid(), (integer)n);
    (vacuum)filum_directorium_creare_cum_modo(dir, 0700);
    sprintf(via, "%s/tabella.clavis", dir);
    (vacuum)filum_scribere_literis(via, contentum);
    (vacuum)filum_modum_ponere(via, modus);
    redde via;
}

interior vacuum
_tabellam_delere (
    constans character* via)
{
    character  dir[CXXVIII];
    character* f;

    (vacuum)filum_delere(via);
    strcpy(dir, via);
    f = strrchr(dir, '/');
    si (f)
    {
        *f = '\0';
        (vacuum)filum_delere(dir);
    }
}

#define TABELLA_BONA \
    "provisor r2\n# commentum\nrationarium abc123\n" \
    "regio auto   # nota\nsitula probatio\nclavis_id AKIDEXAMPLE\n" \
    "secretum " SECRETUM "\n"

interior vacuum
_sectio_tabella_vitia (
    CredoContextus* c)
{
    nomen structura {
        constans character* contentum;
                   integer  modus;
        constans character* nominat;
    } Vitium;
    hic_manens constans Vitium vitia[] = {
        { TABELLA_BONA, 0644, "chmod 600" },
        { "provisor r2\nrationarium abc123\nclavis_id AKIDEXAMPLE\n"
          "secretvm " SECRETUM "\n", 0600, "secretvm" },
        { "provisor r2\nrationarium abc123\nclavis_id AKIDEXAMPLE\n",
            0600,
          "secretum" },
        { "provisor aws9\nclavis_id AKIDEXAMPLE\nsecretum " SECRETUM "\n",
          0600, "aws9" },
        { "AKIDEXAMPLE\n" SECRETUM "\nhttps://abc123.r2.cloudflarestorage.com\n",
          0600, "linea" },
        { NIHIL, 0, NIHIL }
    };
             Piscina* p = c->piscina;
             Piscina* q;
        HorreumError  e;
             Horreum* h;
                 i32  k;
  constans character* via;
                  i8* octeti;
                 i32  j;
                 b32  inventum  = FALSUM;
              chorda  s         = _l(SECRETUM, p);

    per (k = ZEPHYRUM; vitia[k].contentum; k++)
    {
        via = _tabella(vitia[k].contentum, vitia[k].modus, k, p);
        memset(&e, ZEPHYRUM, magnitudo(e));
        h = horreum_ex_tabella(via, &e, p);
        imprimere("  [vitium %d] %.*s\n", (integer)k,
            (integer)e.nuntius.mensura,
            (constans character*)e.nuntius.datum);
        CREDO_NIHIL(h);
        CREDO_CHORDA_CONTINET(e.nuntius, _l(vitia[k].nominat, p));
        CREDO_FALSUM(chorda_continet(e.nuntius, s));
        CREDO_FALSUM(chorda_continet(e.nuntius, _l("AKIDEXAMPLE", p))
            && k == IV);
        _tabellam_delere(via);
    }

    /* bonum; post claudere secretum in memoria non manet */
    q   = piscina_generare_certae_magnitudinis("horreum",
              (memoriae_index)(LXIV * MXXIV));
    via  = _tabella(TABELLA_BONA, 0600, X, p);
    h    = horreum_ex_tabella(via, &e, q);
    CREDO_NON_NIHIL(h);
    CREDO_CHORDA_AEQUALIS_LITERIS(horreum_situla_ordinaria(h),
        "probatio");
    horreum_claudere(h);
    octeti = (i8*)h;
    per (j = ZEPHYRUM; j + s.mensura < (i32)(II * HORREUM_CAPSA_CLAVIS
            + MXXIV); j++)
    {
        si (memcmp(octeti + j, s.datum, (size_t)s.mensura) == ZEPHYRUM)
        {
            inventum = VERUM;
        }
    }
    CREDO_FALSUM(inventum);
    piscina_destruere(q);
    _tabellam_delere(via);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "provisores", _sectio_provisores, NIHIL, NIHIL, NIHIL, NIHIL },
    { "stilus", _sectio_stilus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "via_clavis", _sectio_via_clavis, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ponere_capita", _sectio_ponere_capita, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "incapax", _sectio_incapax, NIHIL, NIHIL, NIHIL, NIHIL },
    { "delere_plura_corpus", _sectio_delere_plura_corpus, NIHIL, NIHIL,
        NIHIL,
        NIHIL },
    { "enumerare_quaestio", _sectio_enumerare_quaestio, NIHIL, NIHIL,
        NIHIL,
        NIHIL },
    { "praesignare", _sectio_praesignare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "tempora", _sectio_tempora, NIHIL, NIHIL, NIHIL, NIHIL },
    { "responsa_errorum", _sectio_responsa_errorum, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "objectum_ex_capitibus", _sectio_objectum_ex_capitibus, NIHIL,
        NIHIL,
        NIHIL, NIHIL },
    { "enumeratio_legere", _sectio_enumeratio_legere, NIHIL, NIHIL,
        NIHIL,
        NIHIL },
    { "multipars_legere", _sectio_partes_legere, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "iterandum", _sectio_iterandum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "partis_mensura", _sectio_partis_mensura, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "tabella_vitia", _sectio_tabella_vitia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("horreum", SECTIONES);
}

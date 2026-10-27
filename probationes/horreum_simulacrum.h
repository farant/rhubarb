/* horreum_simulacrum.h - S3 FICTUM in memoria pro probationibus horrei
 * (horreum-plan T3, P8): HttpVectura quae petitionem signatam accipit
 * et responsum S3 verum (XML, capita) reddit, sine rete.
 *
 * Situla una (SIMULACRUM_SITULA), stilus viae "/situla/clavis"
 * (provisor generic). Quaeque petitio ut servus verus iudicatur:
 * signatura ex filo recomputatur (via et quaestio decodificatae,
 * capita in SignedHeaders) cum secreto noto - discrepantia = 403
 * SignatureDoesNotMatch; x-amz-content-sha256 cum corpore comparatur;
 * hora posita (simulacrum_horam_ponere) x-amz-date ultra CM secundas
 * = 403 RequestTimeTooSkewed cum capite Date. Petitiones praesignatae
 * (X-Amz-Signature in quaestione) eodem modo iudicantur; summae
 * x-amz-checksum-crc32/-sha256 cum corpore comparantur (BadDigest) et
 * redduntur. Stilus hospitis ("situla.hospes") quoque accipitur.
 * Claves ordine octetorum
 * servantur; signum continuationis = base64 ultimi nominis redditi.
 *
 * Operationes: PUT (If-None-Match *, If-Match, x-amz-meta-*,
 * x-amz-copy-source + metadata-directive), GET (Range), HEAD, DELETE,
 * POST ?delete (Content-MD5 probatur; vitium per clavem intra
 * fasciculum), GET ?list-type=2 (prefix, delimiter, max-keys,
 * continuation-token), multipars (POST ?uploads, PUT ?partNumber&
 * uploadId, POST ?uploadId cum ordine/ETag/mensura partium probatis,
 * DELETE ?uploadId, GET ?uploadId, GET ?uploads). ETag = md5 hex;
 * multipars md5(digesta partium) + "-N".
 *
 * Vitia (cauda, ordine addita, quaeque 'numerus' vicibus): STATUS
 * reddit status + codicem S3 (secundae > 0 = Retry-After; status CC =
 * '200 cum <Error>'); SCRIBERE_ABRUMPERE operationem PERFICIT, deinde
 * responsum perit (error transportus); TRUNCARE corpus GET dimidiat
 * (Content-Length congruum); MUTARE post responsum obiectum mutat.
 * 'parametrum' vitium ad partem quaestionis restringit
 * ("partNumber=2", "uploadId").
 *
 * Functiones INTERIORES: includens simulacrum_creare et
 * simulacrum_claudere vocare debet; ceteri aditus in tabula
 * simulacrum_aditus stant quam claudere tangit, ergo includens solum
 * quibus eget vocat (-Wunused-function sub gcc tacet). */
#ifndef HORREUM_SIMULACRUM_H
#define HORREUM_SIMULACRUM_H

#include "latina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "piscina.h"
#include "http.h"
#include "signatura_aws.h"
#include "friatio.h"
#include "sigillum.h"
#include "base64.h"
#include "url.h"
#include "xml.h"
#include "xar.h"
#include "internamentum.h"
#include "fasti.h"
#include <stdio.h>
#include <string.h>

#define SIMULACRUM_SITULA         "situla"
#define SIMULACRUM_VITIA_MAXIMA   XVI
#define SIMULACRUM_PARIA_MAXIMA   XXXII
#define SIMULACRUM_CAPITA_MAXIMA  XXXII
#define SIMULACRUM_ONERA_MAXIMA   XVI
#define SIMULACRUM_PARTES_MAXIMAE XVI
/* 2024-03-15T12:00:00Z: tempus mutationis si hora non posita */
#define SIMULACRUM_TEMPUS         ((MomentumSec)1710504000L)

nomen enumeratio {
    SIMULACRUM_VITIUM_STATUS = 0,          /* status + codex S3 */
    SIMULACRUM_VITIUM_SCRIBERE_ABRUMPERE,  /* operatio fit, responsum
                                            * perit */
    SIMULACRUM_VITIUM_TRUNCARE,            /* GET: corpus dimidium */
    SIMULACRUM_VITIUM_MUTARE               /* post responsum obiectum
                                            * mutatur */
} SimulacrumVitiumGenus;

nomen structura {
    SimulacrumVitiumGenus genus;
             HttpMethodus methodus;   /* HTTP_METHODUS_IGNOTUS =
                                         * quaelibet */
       constans character* clavis;     /* NIHIL = quaelibet */
       constans character* parametrum; /* NIHIL = quaelibet; "nomen"
                                         * aut "nomen=valor" */
                      i32  status;
       constans character* codex;
                      s32  secundae;   /* Retry-After si > 0 */
                      i32  numerus;    /* quoties */
} SimulacrumVitium;

nomen structura {
        chorda  clavis;
        chorda  corpus;
        chorda  etag;
        chorda  genus_contenti;
     HttpCaput* meta;                  /* titulus sine x-amz-meta- */
           i32  meta_numerus;
   MomentumSec  mutatum;
           b32  publicum;              /* x-amz-acl public-read */
} SimulacrumObiectum;

nomen structura {
       i32 numerus;
    chorda corpus;
    chorda etag;
        i8 digestum[MD5_DIGEST_MENSURA];
} SimulacrumPars;

/* onus multipartis: piscina propria, deletur cum perficitur aut
 * abicitur */
nomen structura {
           b32  vivum;
       Piscina* piscina;
        chorda  clavis;
        chorda  id;
        chorda  genus_contenti;
     HttpCaput* meta;
           i32  meta_numerus;
   MomentumSec  initium;
SimulacrumPars  partes[SIMULACRUM_PARTES_MAXIMAE];
           i32  partes_numerus;
} SimulacrumOnus;

nomen structura {
               Piscina* piscina;
                chorda  clavis_id;
                chorda  secretum;
                chorda  regio;
    SimulacrumObiectum* obiecta;
                   i32  numerus;
                   i32  capacitas;
      SimulacrumVitium  vitia[SIMULACRUM_VITIA_MAXIMA];
                   i32  vitia_numerus;
                   i32  petitiones;
           MomentumSec  hora;
        SimulacrumOnus  onera[SIMULACRUM_ONERA_MAXIMA];
                   i32  onera_numerus;
                   i32  corpus_maximum;
                   i32  si_etag_visa;
    /* provisor qui neglegit (probatio refutationis, campi directe
     * positi): If-None-Match/If-Match in PUT; summae x-amz-checksum */
                   b32 neglegere_condiciones;
                   b32 neglegere_summas;
    /* x-amz-acl acceptum sed sine effectu (ut R2 creditur) */
                   b32 neglegere_acl;
    /* ut R2 (2026-10-22): ListMultipartUploads id ALIUM reddit
     * ("L-" + id); utrumque in uploadId valet */
                   b32 id_oneris_alienum;
    /* ListMultipartUploads nihil reddit (causa nominata probatur) */
                   b32 celare_onera;
} Simulacrum;

/* ===== auxilia ===== */
interior chorda
_sim_l (
    constans character* l,
               Piscina* p)
{
    redde chorda_ex_literis(l, p);
}

interior chorda
_sim_caput (
    constans HttpPetitioVisus* v,
           constans character* titulus,
                      Piscina* p)
{
    i32 i;

    per (i = ZEPHYRUM; i < v->capita_numerus; i++)
    {
        si (chorda_aequalis_case_insensitivus(v->capita[i].titulus,
                _sim_l(titulus, p)))
        {
            redde v->capita[i].valor;
        }
    }
    redde chorda_ex_buffer(NIHIL, ZEPHYRUM);
}

/* index paris in quaestione aut -I */
interior s32
_sim_par (
    constans SignaturaPar* q,
                      i32  n,
       constans character* titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < n; i++)
    {
        si (chorda_aequalis_literis(q[i].titulus, titulus))
        {
            redde (s32)i;
        }
    }
    redde -I;
}

interior chorda
_sim_hex (
    constans i8* octeti,
            i32  n,
        Piscina* p)
{
    hic_manens constans character  cifrae[] = "0123456789abcdef";
                        character* t = (character*)piscina_allocare(p,
                            (memoriae_index)(n * II + I));
           i32 i;

    per (i = ZEPHYRUM; i < n; i++)
    {
        t[i * II]      = cifrae[((i32)octeti[i] >> IV) & 0xF];
        t[i * II + I]  = cifrae[(i32)octeti[i] & 0xF];
    }
    redde chorda_ex_buffer((i8*)t, n * II);
}

interior chorda
_sim_etag (
     chorda  corpus,
    Piscina* p)
{
    i8 d[MD5_DIGEST_MENSURA];

    md5_friare(corpus.datum, corpus.mensura, d);
    redde _sim_hex(d, MD5_DIGEST_MENSURA, p);
}

interior chorda
_sim_numerus (
        s64  n,
    Piscina* p)
{
    character capsa[XXIV];

    sprintf(capsa, "%ld", (longus)n);
    redde _sim_l(capsa, p);
}

interior chorda
_sim_tempus_http (
    MomentumSec  t,
        Piscina* p)
{
    hic_manens constans character* dies_hebdomadis[] = {
        "Thu", "Fri", "Sat", "Sun", "Mon", "Tue", "Wed"
    };
    hic_manens constans character* menses[] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep",
        "Oct", "Nov", "Dec"
    };
     DiesHora dh = fasti_ex_unix(t);
    character capsa[XL];
          s64 dies = (s64)t / (s64)86400L;

    sprintf(capsa, "%s, %02d %s %04d %02d:%02d:%02d GMT",
        dies_hebdomadis[(i32)(dies % VII)], (integer)dh.dies.dies,
        menses[dh.dies.mensis - I], (integer)dh.dies.annus,
        (integer)dh.hora.hora, (integer)dh.hora.minutum,
        (integer)dh.hora.secundum);
    redde _sim_l(capsa, p);
}

interior chorda
_sim_tempus_iso (
    MomentumSec  t,
        Piscina* p)
{
     DiesHora dh = fasti_ex_unix(t);
    character capsa[XL];

    sprintf(capsa, "%04d-%02d-%02dT%02d:%02d:%02d.000Z",
        (integer)dh.dies.annus, (integer)dh.dies.mensis,
        (integer)dh.dies.dies, (integer)dh.hora.hora,
        (integer)dh.hora.minutum, (integer)dh.hora.secundum);
    redde _sim_l(capsa, p);
}

interior vacuum
_sim_effugere (
    ChordaAedificator* a,
               chorda  s)
{
    i32 i;

    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        commutatio (s.datum[i])
        {
            casus '&': chorda_aedificator_appendere_literis(a, "&amp;");
                frange;
            casus '<': chorda_aedificator_appendere_literis(a, "&lt;");
                frange;
            casus '>': chorda_aedificator_appendere_literis(a, "&gt;");
                frange;
            casus '"': chorda_aedificator_appendere_literis(a,
                           "&quot;");
                frange;
            ordinarius:
                chorda_aedificator_appendere_character(a,
                    (character)s.datum[i]);
        }
    }
}

/* "YYYYMMDDTHHMMSSZ" -> MomentumSec; 0 si malum */
interior MomentumSec
_sim_dies_amz (
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
        (constans character*)s.datum + IV,
        (constans character*)s.datum + VI,
        (constans character*)s.datum + IX,
        (constans character*)s.datum + XI,
        (constans character*)s.datum + XIII);
    si (!fasti_ex_iso(_sim_l(iso, p), &dh))
    {
        redde ZEPHYRUM;
    }
    redde fasti_ad_unix(dh);
}

/* ===== responsa ===== */
interior HttpResponsum*
_sim_responsum (
        i32  status,
    Piscina* p)
{
    HttpResponsum* r = (HttpResponsum*)piscina_allocare(p,
        (memoriae_index)magnitudo(HttpResponsum));

    memset(r, ZEPHYRUM, magnitudo(*r));
    r->status  = status;
    r->capita  = (HttpCaput*)piscina_allocare(p,
        (memoriae_index)(SIMULACRUM_CAPITA_MAXIMA
            * (i32)magnitudo(HttpCaput)));
    r->corpus  = chorda_ex_buffer(NIHIL, ZEPHYRUM);
    redde r;
}

interior vacuum
_sim_caput_addere (
          HttpResponsum* r,
     constans character* titulus,
                 chorda  valor,
                Piscina* p)
{
    si (r->capita_numerus >= SIMULACRUM_CAPITA_MAXIMA)
    {
        redde;
    }
    r->capita[r->capita_numerus].titulus  = _sim_l(titulus, p);
    r->capita[r->capita_numerus].valor    = valor;
    r->capita_numerus++;
}

interior HttpResultus
_sim_bene (
    HttpResponsum* r)
{
    HttpResultus res;

    memset(&res, ZEPHYRUM, magnitudo(res));
    res.successus = VERUM;
    res.responsum = r;
    redde res;
}

interior HttpResultus
_sim_error (
            Simulacrum* s,
                   i32  status,
    constans character* codex,
                   s32  secundae,
               Piscina* p)
{
        HttpResponsum* r   = _sim_responsum(status, p);
    ChordaAedificator* a   = chorda_aedificator_creare(p, CCLVI);
               chorda  id  = _sim_numerus((s64)s->petitiones, p);

    chorda_aedificator_appendere_literis(a,
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<Error><Code>");
    chorda_aedificator_appendere_literis(a, codex);
    chorda_aedificator_appendere_literis(a,
        "</Code><Message>simulacrum: ");
    chorda_aedificator_appendere_literis(a, codex);
    chorda_aedificator_appendere_literis(a, "</Message><RequestId>SIM");
    chorda_aedificator_appendere_chorda(a, id);
    chorda_aedificator_appendere_literis(a, "</RequestId></Error>");
    r->corpus = chorda_aedificator_finire(a);
    _sim_caput_addere(r, "Content-Type", _sim_l("application/xml", p),
        p);
    _sim_caput_addere(r, "x-amz-request-id", chorda_concatenare(
        _sim_l("SIM", p), id, p), p);
    si (secundae > ZEPHYRUM)
    {
        _sim_caput_addere(r, "Retry-After", _sim_numerus((s64)secundae,
            p), p);
    }
    si (s->hora > ZEPHYRUM)
    {
        _sim_caput_addere(r, "Date", _sim_tempus_http(s->hora, p), p);
    }
    redde _sim_bene(r);
}

/* ===== repositorium (ordo octetorum) ===== */
/* VERUM si inventa; *index = locus (aut locus inserendi) */
interior b32
_sim_invenire (
    Simulacrum* s,
        chorda  clavis,
           i32* index)
{
    s32 ab = ZEPHYRUM;
    s32 ad = (s32)s->numerus - I;

    dum (ab <= ad)
    {
        s32 medium = ab + (ad - ab) / II;
        s32 c = chorda_comparare(s->obiecta[medium].clavis, clavis);

        si (c == ZEPHYRUM)
        {
            *index = (i32)medium;
            redde VERUM;
        }
        si (c < ZEPHYRUM)
        {
            ab = medium + I;
        }
        alioquin
        {
            ad = medium - I;
        }
    }
    *index = (i32)ab;
    redde FALSUM;
}

interior SimulacrumObiectum*
_sim_locus (
    Simulacrum* s,
        chorda  clavis)
{
    i32 i;

    si (_sim_invenire(s, clavis, &i))
    {
        redde &s->obiecta[i];
    }
    si (s->numerus >= s->capacitas)
    {
               i32 nova = s->capacitas > ZEPHYRUM ? s->capacitas * II
                                                   : LXIV;
        SimulacrumObiectum* t = (SimulacrumObiectum*)piscina_allocare(
            s->piscina, (memoriae_index)(nova
                * (i32)magnitudo(SimulacrumObiectum)));

        si (s->numerus > ZEPHYRUM)
        {
            memcpy(t, s->obiecta, (size_t)s->numerus
                * magnitudo(SimulacrumObiectum));
        }
        s->obiecta    = t;
        s->capacitas  = nova;
    }
    memmove(&s->obiecta[i + I], &s->obiecta[i], (size_t)(s->numerus - i)
        * magnitudo(SimulacrumObiectum));
    s->numerus++;
    memset(&s->obiecta[i], ZEPHYRUM, magnitudo(SimulacrumObiectum));
    s->obiecta[i].clavis = chorda_transcribere(clavis, s->piscina);
    redde &s->obiecta[i];
}

interior vacuum
_sim_scribere (
            Simulacrum* s,
                 chorda  clavis,
                 chorda  corpus,
                 chorda  genus_contenti,
    constans HttpCaput* meta,
                    i32  meta_numerus)
{
    SimulacrumObiectum* o = _sim_locus(s, clavis);
                   i32  i;

    o->corpus  = chorda_transcribere(corpus, s->piscina);
    o->etag    = _sim_etag(corpus, s->piscina);
    o->genus_contenti = chorda_transcribere(genus_contenti,
        s->piscina);
    o->mutatum = s->hora
        > ZEPHYRUM ? s->hora : SIMULACRUM_TEMPUS;
    o->publicum      = FALSUM;
    o->meta_numerus  = meta_numerus;
    o->meta            = (HttpCaput*)piscina_allocare(s->piscina,
        (memoriae_index)((meta_numerus
            + I) * (i32)magnitudo(HttpCaput)));
    per (i = ZEPHYRUM; i < meta_numerus; i++)
    {
        o->meta[i].titulus = chorda_transcribere(meta[i].titulus,
            s->piscina);
        o->meta[i].valor = chorda_transcribere(meta[i].valor,
            s->piscina);
    }
}

interior vacuum
_sim_removere (
    Simulacrum* s,
        chorda  clavis)
{
    i32 i;

    si (!_sim_invenire(s, clavis, &i))
    {
        redde;
    }
    memmove(&s->obiecta[i], &s->obiecta[i + I], (size_t)(s->numerus - i
        - I) * magnitudo(SimulacrumObiectum));
    s->numerus--;
}

/* vitium primum quod congruit (numerus > 0) consumitur */
interior SimulacrumVitium*
_sim_vitium (
               Simulacrum* s,
             HttpMethodus  methodus,
                   chorda  clavis,
    constans SignaturaPar* q,
                      i32  nq)
{
    i32 i;

    per (i = ZEPHYRUM; i < s->vitia_numerus; i++)
    {
        SimulacrumVitium* v = &s->vitia[i];

        si (v->numerus == ZEPHYRUM)
        {
            perge;
        }
        si (   v->methodus != HTTP_METHODUS_IGNOTUS
            && v->methodus != methodus)
        {
            perge;
        }
        si (v->clavis && !chorda_aequalis_literis(clavis, v->clavis))
        {
            perge;
        }
        si (v->parametrum)
        {
             constans character* aeq = strchr(v->parametrum, '=');
                      character  titulus[LXIV];
                            s32  k;

            sprintf(titulus, "%.*s", aeq ? (integer)(aeq
                - v->parametrum)
                : (integer)strlen(v->parametrum), v->parametrum);
            k = _sim_par(q, nq, titulus);
            si (   k < ZEPHYRUM
                || (aeq
                    && !chorda_aequalis_literis(q[k].valor, aeq + I)))
            {
                perge;
            }
        }
        v->numerus--;
        redde v;
    }
    redde NIHIL;
}

/* ===== iudicium petitionis ===== */
/* ut servus: signaturam ex filo recomputat et cum Authorization
 * comparat */
interior b32
_sim_signatura_valida (
                   Simulacrum* s,
    constans HttpPetitioVisus* v,
                       chorda  via,
        constans SignaturaPar* q,
                          i32  nq,
                      Piscina* p)
{
               chorda auctoritas = _sim_caput(v, "Authorization", p);
         SignaturaPar par_c[SIMULACRUM_PARIA_MAXIMA];
                  i32 nc = ZEPHYRUM;
                  s32 i;
               chorda signata;
     SignaturaPetitio sp;
    SignaturaResultus r;

    i = chorda_invenire_index(auctoritas, _sim_l("SignedHeaders=", p));
    si (i < ZEPHYRUM)
    {
        redde FALSUM;
    }
    signata = chorda_sectio(auctoritas, (i32)i + XIV,
        auctoritas.mensura);
    i = chorda_invenire_index(signata, _sim_l(",", p));
    si (i < ZEPHYRUM)
    {
        redde FALSUM;
    }
    signata = chorda_sectio(signata, ZEPHYRUM, (i32)i);
    dum (signata.mensura > ZEPHYRUM && nc < SIMULACRUM_PARIA_MAXIMA)
    {
           s32 sc = chorda_invenire_index(signata, _sim_l(";", p));
        chorda t  = sc < ZEPHYRUM ? signata
                                  : chorda_sectio(signata, ZEPHYRUM,
                                      (i32)sc);

        si (   !chorda_aequalis_literis(t, "host")
            && !chorda_aequalis_literis(t, "x-amz-date")
            && !chorda_aequalis_literis(t, "x-amz-content-sha256"))
        {
            par_c[nc].titulus  = t;
            par_c[nc].valor    = _sim_caput(v, chorda_ut_cstr(t, p), p);
            nc++;
        }
        si (sc < ZEPHYRUM)
        {
            frange;
        }
        signata = chorda_sectio(signata, (i32)sc + I, signata.mensura);
    }
    memset(&sp, ZEPHYRUM, magnitudo(sp));
    sp.methodus          = http_methodus_nomen(v->methodus);
    sp.hospes            = _sim_caput(v, "Host", p);
    sp.via               = via;
    sp.quaestio          = q;
    sp.quaestio_numerus  = nq;
    sp.capita            = par_c;
    sp.capita_numerus    = nc;
    sp.corpus_sigillum   = _sim_caput(v, "x-amz-content-sha256", p);
    sp.corpus_capite     = VERUM;
    sp.tempus            = _sim_dies_amz(_sim_caput(v, "x-amz-date", p),
        p);
    sp.regio      = s->regio;
    sp.servitium  = _sim_l("s3", p);
    sp.clavis_id  = s->clavis_id;
    sp.secretum   = s->secretum;
    r             = signatura_aws_capitibus(&sp, p);
    memset(&sp, ZEPHYRUM, magnitudo(sp));
    redde r.successus && chorda_aequalis(r.auctoritas, auctoritas);
}

/* praesignata (X-Amz-Signature in quaestione, sine Authorization):
 * signatura ex quaestione sine X-Amz-* recomputatur */
interior b32
_sim_praesignata_valida (
                   Simulacrum* s,
    constans HttpPetitioVisus* v,
                       chorda  via,
        constans SignaturaPar* q,
                          i32  nq,
                      Piscina* p)
{
         SignaturaPar base[SIMULACRUM_PARIA_MAXIMA];
         SignaturaPar par_c[SIMULACRUM_PARIA_MAXIMA];
                  i32 nb = ZEPHYRUM;
                  i32 nc = ZEPHYRUM;
                  i32 i;
                  s32 duratio  = ZEPHYRUM;
                  s32 k_sig    = _sim_par(q, nq, "X-Amz-Signature");
                  s32 k_cap    = _sim_par(q, nq, "X-Amz-SignedHeaders");
                  s32 k_exp    = _sim_par(q, nq, "X-Amz-Expires");
                  s32 k_dies   = _sim_par(q, nq, "X-Amz-Date");
                  s32 k_cred   = _sim_par(q, nq, "X-Amz-Credential");
               chorda signata;
     SignaturaPetitio sp;
    SignaturaResultus r;

    si (   k_sig < ZEPHYRUM || k_cap < ZEPHYRUM || k_exp < ZEPHYRUM
        || k_dies < ZEPHYRUM || k_cred < ZEPHYRUM
        || !chorda_incipit(q[k_cred].valor, s->clavis_id)
        || !chorda_ut_s32(q[k_exp].valor, &duratio))
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < nq; i++)
    {
        si (!chorda_incipit(q[i].titulus, _sim_l("X-Amz-", p)))
        {
            base[nb] = q[i];
            nb++;
        }
    }
    signata = q[k_cap].valor;
    dum (signata.mensura > ZEPHYRUM && nc < SIMULACRUM_PARIA_MAXIMA)
    {
           s32 sc = chorda_invenire_index(signata, _sim_l(";", p));
        chorda t  = sc < ZEPHYRUM ? signata
                                  : chorda_sectio(signata, ZEPHYRUM,
                                      (i32)sc);

        si (!chorda_aequalis_literis(t, "host"))
        {
            par_c[nc].titulus  = t;
            par_c[nc].valor    = _sim_caput(v, chorda_ut_cstr(t, p), p);
            nc++;
        }
        si (sc < ZEPHYRUM)
        {
            frange;
        }
        signata = chorda_sectio(signata, (i32)sc + I, signata.mensura);
    }
    memset(&sp, ZEPHYRUM, magnitudo(sp));
    sp.methodus          = http_methodus_nomen(v->methodus);
    sp.hospes            = _sim_caput(v, "Host", p);
    sp.via               = via;
    sp.quaestio          = base;
    sp.quaestio_numerus  = nb;
    sp.capita            = par_c;
    sp.capita_numerus    = nc;
    sp.corpus_sigillum   = _sim_l("UNSIGNED-PAYLOAD", p);
    sp.tempus            = _sim_dies_amz(q[k_dies].valor, p);
    sp.duratio           = (i32)duratio;
    sp.regio             = s->regio;
    sp.servitium         = _sim_l("s3", p);
    sp.clavis_id         = s->clavis_id;
    sp.secretum          = s->secretum;
    r                    = signatura_aws_quaestione(&sp, p);
    memset(&sp, ZEPHYRUM, magnitudo(sp));
    redde r.successus && chorda_aequalis(r.signatura, q[k_sig].valor);
}

/* x-amz-checksum-*: comparatur cum corpore; VERUM si abest aut aequat */
interior b32
_sim_summae_validae (
    constans HttpPetitioVisus* v,
                      Piscina* p)
{
      chorda crc = _sim_caput(v, "x-amz-checksum-crc32", p);
      chorda sha = _sim_caput(v, "x-amz-checksum-sha256", p);
         i32 c;
          i8 be[IV];
    Sigillum sg;
      chorda computata;

    si (crc.mensura > ZEPHYRUM)
    {
        c = crc32_calculare(v->corpus.datum, v->corpus.mensura);
        be[ZEPHYRUM] = (i8)((c >> XXIV) & 0xFFu);
        be[I] = (i8)((c >> XVI) & 0xFFu);
        be[II] = (i8)((c >> VIII) & 0xFFu);
        be[III] = (i8)(c & 0xFFu);
        computata = base64_codificare(be, IV, p);
        si (!chorda_aequalis(computata, crc))
        {
            redde FALSUM;
        }
    }
    si (sha.mensura > ZEPHYRUM)
    {
        sg = sigillum_computare(v->corpus.datum,
            (memoriae_index)v->corpus.mensura);
        computata = base64_codificare(sg.octeti, SIGILLUM_OCTETI, p);
        si (!chorda_aequalis(computata, sha))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* ===== operationes ===== */
interior vacuum
_sim_capita_obiecti (
          HttpResponsum* r,
     SimulacrumObiectum* o,
                    i64  mensura,
                Piscina* p)
{
     ChordaAedificator* a = chorda_aedificator_creare(p, XLVIII);
                   i32  i;

    chorda_aedificator_appendere_character(a, '"');
    chorda_aedificator_appendere_chorda(a, o->etag);
    chorda_aedificator_appendere_character(a, '"');
    _sim_caput_addere(r, "ETag", chorda_aedificator_finire(a), p);
    _sim_caput_addere(r, "Content-Length", _sim_numerus((s64)mensura,
        p),
        p);
    _sim_caput_addere(r, "Last-Modified", _sim_tempus_http(o->mutatum,
        p), p);
    si (o->genus_contenti.mensura > ZEPHYRUM)
    {
        _sim_caput_addere(r, "Content-Type", o->genus_contenti, p);
    }
    per (i = ZEPHYRUM; i < o->meta_numerus; i++)
    {
        _sim_caput_addere(r, chorda_ut_cstr(chorda_concatenare(_sim_l(
            "x-amz-meta-", p), o->meta[i].titulus, p), p),
            o->meta[i].valor, p);
    }
}

/* capita x-amz-meta-* petitionis -> paria (titulus minusculus) */
interior i32
_sim_meta_petitionis (
    constans HttpPetitioVisus* v,
                    HttpCaput* meta,
                      Piscina* p)
{
    i32 i;
    i32 n = ZEPHYRUM;

    per (i = ZEPHYRUM; i < v->capita_numerus && n < XVI; i++)
    {
        chorda t = chorda_minuscula(v->capita[i].titulus, p);

        si (chorda_incipit(t, _sim_l("x-amz-meta-", p)))
        {
            meta[n].titulus  = chorda_sectio(t, XI, t.mensura);
            meta[n].valor    = v->capita[i].valor;
            n++;
        }
    }
    redde n;
}

interior HttpResultus
_sim_ponere (
                   Simulacrum* s,
    constans HttpPetitioVisus* v,
                       chorda  clavis,
                      Piscina* p)
{
                 chorda  nisi_etag  = _sim_caput(v, "If-None-Match", p);
                 chorda  si_etag    = _sim_caput(v, "If-Match", p);
                    i32  index;
                    b32  est = _sim_invenire(s, clavis, &index);
              HttpCaput  meta[XVI];
                    i32  n;
          HttpResponsum* r;
      ChordaAedificator* a;

    si (s->neglegere_condiciones)
    {
        nisi_etag  = chorda_ex_buffer(NIHIL, ZEPHYRUM);
        si_etag    = nisi_etag;
    }
    si (chorda_aequalis_literis(nisi_etag, "*") && est)
    {
        redde _sim_error(s, CDXII, "PreconditionFailed", ZEPHYRUM, p);
    }
    si (si_etag.mensura > ZEPHYRUM)
    {
        si (!est)
        {
            redde _sim_error(s, CDIV, "NoSuchKey", ZEPHYRUM, p);
        }
        si (   si_etag.mensura < II
            || !chorda_aequalis(chorda_sectio(si_etag, I,
            si_etag.mensura - I),
                    s->obiecta[index].etag))
        {
            redde _sim_error(s, CDXII, "PreconditionFailed", ZEPHYRUM,
                p);
        }
    }
    si (!s->neglegere_summas && !_sim_summae_validae(v, p))
    {
        redde _sim_error(s, CD, "BadDigest", ZEPHYRUM, p);
    }
    n = _sim_meta_petitionis(v, meta, p);
    _sim_scribere(s, clavis, v->corpus, _sim_caput(v, "Content-Type",
        p),
        meta, n);
    si (   !s->neglegere_acl
        && chorda_aequalis_literis(_sim_caput(v, "x-amz-acl", p),
            "public-read")
        && _sim_invenire(s, clavis, &index))
    {
        s->obiecta[index].publicum = VERUM;
    }
    r = _sim_responsum(CC, p);
    a = chorda_aedificator_creare(p, XLVIII);
    chorda_aedificator_appendere_character(a, '"');
    chorda_aedificator_appendere_chorda(a, _sim_etag(v->corpus, p));
    chorda_aedificator_appendere_character(a, '"');
    _sim_caput_addere(r, "ETag", chorda_aedificator_finire(a), p);
    si (   !s->neglegere_summas
        && _sim_caput(v, "x-amz-checksum-crc32", p).mensura > ZEPHYRUM)
    {
        _sim_caput_addere(r, "x-amz-checksum-crc32", _sim_caput(v,
            "x-amz-checksum-crc32", p), p);
    }
    si (   !s->neglegere_summas
        && _sim_caput(v, "x-amz-checksum-sha256", p).mensura > ZEPHYRUM)
    {
        _sim_caput_addere(r, "x-amz-checksum-sha256", _sim_caput(v,
            "x-amz-checksum-sha256", p), p);
    }
    redde _sim_bene(r);
}

interior HttpResultus
_sim_copiare (
                   Simulacrum* s,
    constans HttpPetitioVisus* v,
                       chorda  clavis,
                      Piscina* p)
{
                 chorda fons = url_decodificare(_sim_caput(v,
                              "x-amz-copy-source", p), p);
                 chorda  radix = _sim_l("/" SIMULACRUM_SITULA "/", p);
                    i32  index;
     SimulacrumObiectum  o;
              HttpCaput  meta[XVI];
                    i32  n;
          HttpResponsum* r;
      ChordaAedificator* a;

    si (!chorda_incipit(fons, radix))
    {
        redde _sim_error(s, CDIV, "NoSuchBucket", ZEPHYRUM, p);
    }
    fons = chorda_sectio(fons, radix.mensura, fons.mensura);
    si (!_sim_invenire(s, fons, &index))
    {
        redde _sim_error(s, CDIV, "NoSuchKey", ZEPHYRUM, p);
    }
    o = s->obiecta[index];
    si (chorda_aequalis_literis(_sim_caput(v,
        "x-amz-metadata-directive",
            p), "REPLACE"))
    {
        n = _sim_meta_petitionis(v, meta, p);
        _sim_scribere(s, clavis, o.corpus, _sim_caput(v, "Content-Type",
            p), meta, n);
    }
    alioquin
    {
        _sim_scribere(s, clavis, o.corpus, o.genus_contenti, o.meta,
            o.meta_numerus);
    }
    (vacuum)_sim_invenire(s, clavis, &index);
    r = _sim_responsum(CC, p);
    a = chorda_aedificator_creare(p, CCLVI);
    chorda_aedificator_appendere_literis(a,
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<CopyObjectResult><LastModified>");
    chorda_aedificator_appendere_chorda(a, _sim_tempus_iso(
        s->obiecta[index].mutatum, p));
    chorda_aedificator_appendere_literis(a,
        "</LastModified><ETag>&quot;");
    chorda_aedificator_appendere_chorda(a, s->obiecta[index].etag);
    chorda_aedificator_appendere_literis(a,
        "&quot;</ETag></CopyObjectResult>");
    r->corpus = chorda_aedificator_finire(a);
    redde _sim_bene(r);
}

interior HttpResultus
_sim_capere (
                   Simulacrum* s,
    constans HttpPetitioVisus* v,
                       chorda  clavis,
                          b32  solum_capita,
                      Piscina* p)
{
                    i32  index;
     SimulacrumObiectum* o;
                 chorda  ambitus = _sim_caput(v, "Range", p);
          HttpResponsum* r;

    si (!_sim_invenire(s, clavis, &index))
    {
        si (solum_capita)
        {
            redde _sim_bene(_sim_responsum(CDIV, p));
        }
        redde _sim_error(s, CDIV, "NoSuchKey", ZEPHYRUM, p);
    }
    o = &s->obiecta[index];
    si (_sim_caput(v, "If-Match", p).mensura > ZEPHYRUM)
    {
        chorda si_etag = _sim_caput(v, "If-Match", p);

        s->si_etag_visa++;
        si (   si_etag.mensura < II
            || !chorda_aequalis(chorda_sectio(si_etag, I,
            si_etag.mensura
                - I), o->etag))
        {
            redde _sim_error(s, CDXII, "PreconditionFailed", ZEPHYRUM,
                p);
        }
    }
    si (chorda_incipit(ambitus, _sim_l("bytes=", p)) && !solum_capita)
    {
           s32 linea = chorda_invenire_index(ambitus, _sim_l("-", p));
           s32 ab;
           s32 ad;
        chorda par;
        chorda cr;

        si (linea < ZEPHYRUM)
        {
            redde _sim_error(s, CD, "InvalidRange", ZEPHYRUM, p);
        }
        si (   !chorda_ut_s32(chorda_sectio(ambitus, VI, (i32)linea),
                    &ab)
            || !chorda_ut_s32(chorda_sectio(ambitus, (i32)linea + I,
                    ambitus.mensura), &ad))
        {
            redde _sim_error(s, CD, "InvalidRange", ZEPHYRUM, p);
        }
        si (ad >= (s32)o->corpus.mensura)
        {
            ad = (s32)o->corpus.mensura - I;
        }
        si (ab < ZEPHYRUM || ab > ad)
        {
            redde _sim_error(s, CDXVI, "InvalidRange", ZEPHYRUM, p);
        }
        par  = chorda_sectio(o->corpus, (i32)ab, (i32)ad + I);
        r    = _sim_responsum(CCVI, p);
        _sim_capita_obiecti(r, o, (i64)par.mensura, p);
        cr = chorda_concatenare(_sim_l("bytes ", p), chorda_sectio(
            ambitus, VI, ambitus.mensura), p);
        cr = chorda_concatenare(cr, _sim_l("/", p), p);
        _sim_caput_addere(r, "Content-Range", chorda_concatenare(cr,
            _sim_numerus((s64)o->corpus.mensura, p), p), p);
        r->corpus = par;
        redde _sim_bene(r);
    }
    r = _sim_responsum(CC, p);
    _sim_capita_obiecti(r, o, (i64)o->corpus.mensura, p);
    si (!solum_capita)
    {
        r->corpus = o->corpus;
    }
    redde _sim_bene(r);
}

interior HttpResultus
_sim_delere_plura (
                   Simulacrum* s,
    constans HttpPetitioVisus* v,
                      Piscina* p)
{
                i8  md5[MD5_DIGEST_MENSURA];
       XmlResultus  x;
               Xar* obiecta;
               i32  i;
     HttpResponsum* r;
 ChordaAedificator* a;

    md5_friare(v->corpus.datum, v->corpus.mensura, md5);
    si (!chorda_aequalis(base64_codificare(md5, MD5_DIGEST_MENSURA, p),
            _sim_caput(v, "Content-MD5", p)))
    {
        redde _sim_error(s, CD, "BadDigest", ZEPHYRUM, p);
    }
    x = xml_legere(v->corpus, p, internamentum_creare(p));
    si (!x.successus || !x.radix)
    {
        redde _sim_error(s, CD, "MalformedXML", ZEPHYRUM, p);
    }
    obiecta  = xml_invenire_omnes_liberos(x.radix, "Object", p);
    a        = chorda_aedificator_creare(p, MXXIV);
    chorda_aedificator_appendere_literis(a,
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<DeleteResult>");
    per (i = ZEPHYRUM; obiecta && i < (i32)xar_numerus(obiecta); i++)
    {
               XmlNodus* n = *(XmlNodus**)xar_obtinere(obiecta, i);
               XmlNodus* k = xml_invenire_liberum(n, "Key");
                 chorda  clavis = k ? xml_textus_internus(k, p)
                                    : chorda_ex_buffer(NIHIL, ZEPHYRUM);
       SimulacrumVitium* vi = _sim_vitium(s, HTTP_POST, clavis,
            NIHIL, ZEPHYRUM);

        si (vi && vi->genus == SIMULACRUM_VITIUM_STATUS)
        {
            chorda_aedificator_appendere_literis(a, "<Error><Key>");
            _sim_effugere(a, clavis);
            chorda_aedificator_appendere_literis(a, "</Key><Code>");
            chorda_aedificator_appendere_literis(a, vi->codex);
            chorda_aedificator_appendere_literis(a,
                "</Code><Message>simulacrum</Message></Error>");
            perge;
        }
        _sim_removere(s, clavis);
        chorda_aedificator_appendere_literis(a, "<Deleted><Key>");
        _sim_effugere(a, clavis);
        chorda_aedificator_appendere_literis(a, "</Key></Deleted>");
    }
    chorda_aedificator_appendere_literis(a, "</DeleteResult>");
    r          = _sim_responsum(CC, p);
    r->corpus  = chorda_aedificator_finire(a);
    redde _sim_bene(r);
}

/* nomen redditum pro clave: praefixum commune si delimitator post
 * praefixum occurrit, aliter clavis */
interior chorda
_sim_nomen_redditum (
    chorda  clavis,
    chorda  praefixum,
    chorda  delimitator,
       b32* commune)
{
    chorda reliquum = chorda_sectio(clavis, praefixum.mensura,
        clavis.mensura);
       s32 d;

    *commune = FALSUM;
    si (delimitator.mensura == ZEPHYRUM)
    {
        redde clavis;
    }
    d = chorda_invenire_index(reliquum, delimitator);
    si (d < ZEPHYRUM)
    {
        redde clavis;
    }
    *commune = VERUM;
    redde chorda_sectio(clavis, ZEPHYRUM, praefixum.mensura + (i32)d
        + delimitator.mensura);
}

interior HttpResultus
_sim_enumerare (
               Simulacrum* s,
    constans SignaturaPar* q,
                      i32  nq,
                  Piscina* p)
{
                s32 ip = _sim_par(q, nq, "prefix");
                s32 id = _sim_par(q, nq, "delimiter");
                s32 index_maximi = _sim_par(q, nq, "max-keys");
                s32 it = _sim_par(q, nq, "continuation-token");
             chorda praefixum = ip >= ZEPHYRUM ? q[ip].valor
                                 : chorda_ex_buffer(NIHIL, ZEPHYRUM);
             chorda delimitator = id >= ZEPHYRUM ? q[id].valor
                                   : chorda_ex_buffer(NIHIL, ZEPHYRUM);
                s32  maximum       = M;
             chorda  ultimum       = chorda_ex_buffer(NIHIL, ZEPHYRUM);
                b32  post_ultimum  = FALSUM;
                i32  numerus       = ZEPHYRUM;
                b32  truncata      = FALSUM;
                i32  i;
  ChordaAedificator* contenta  = chorda_aedificator_creare(p, MXXIV);
  ChordaAedificator* a         = chorda_aedificator_creare(p, MXXIV);
      HttpResponsum* r;

    si (   index_maximi >= ZEPHYRUM
        && (!chorda_ut_s32(q[index_maximi].valor, &maximum)
        || maximum < I
            || maximum > M))
    {
        maximum = M;
    }
    si (it >= ZEPHYRUM)
    {
        Base64Fructus b = base64_decodificare(q[it].valor, p);

        ultimum       = chorda_ex_buffer(b.datum, b.mensura);
        post_ultimum  = VERUM;
    }
    per (i = ZEPHYRUM; i < s->numerus; i++)
    {
        SimulacrumObiectum* o = &s->obiecta[i];
                       b32  commune;
                    chorda  n;

        /* chorda_incipit praefixum vacuum sine datum recusat */
        si (   praefixum.mensura > ZEPHYRUM
            && !chorda_incipit(o->clavis, praefixum))
        {
            perge;
        }
        n = _sim_nomen_redditum(o->clavis, praefixum, delimitator,
            &commune);
        si (post_ultimum && chorda_comparare(n, ultimum) <= ZEPHYRUM)
        {
            perge;
        }
        si ((s32)numerus == maximum)
        {
            truncata = VERUM;
            frange;
        }
        si (commune)
        {
            chorda_aedificator_appendere_literis(contenta,
                "<CommonPrefixes><Prefix>");
            _sim_effugere(contenta, n);
            chorda_aedificator_appendere_literis(contenta,
                "</Prefix></CommonPrefixes>");
        }
        alioquin
        {
            chorda_aedificator_appendere_literis(contenta,
                "<Contents><Key>");
            _sim_effugere(contenta, n);
            chorda_aedificator_appendere_literis(contenta,
                "</Key><LastModified>");
            chorda_aedificator_appendere_chorda(contenta,
                _sim_tempus_iso(o->mutatum, p));
            chorda_aedificator_appendere_literis(contenta,
                "</LastModified><ETag>&quot;");
            chorda_aedificator_appendere_chorda(contenta, o->etag);
            chorda_aedificator_appendere_literis(contenta,
                "&quot;</ETag><Size>");
            chorda_aedificator_appendere_chorda(contenta, _sim_numerus(
                (s64)o->corpus.mensura, p));
            chorda_aedificator_appendere_literis(contenta,
                "</Size><StorageClass>STANDARD</StorageClass>"
                "</Contents>");
        }
        ultimum       = n;
        post_ultimum  = VERUM;
        numerus++;
    }
    chorda_aedificator_appendere_literis(a,
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<ListBucketResult "
        "xmlns=\"http://s3.amazonaws.com/doc/2006-03-01/\"><Name>"
        SIMULACRUM_SITULA "</Name><Prefix>");
    _sim_effugere(a, praefixum);
    chorda_aedificator_appendere_literis(a, "</Prefix><KeyCount>");
    chorda_aedificator_appendere_chorda(a, _sim_numerus((s64)numerus,
        p));
    chorda_aedificator_appendere_literis(a, "</KeyCount><MaxKeys>");
    chorda_aedificator_appendere_chorda(a, _sim_numerus((s64)maximum,
        p));
    chorda_aedificator_appendere_literis(a, "</MaxKeys><IsTruncated>");
    chorda_aedificator_appendere_literis(a,
        truncata ? "true" : "false");
    chorda_aedificator_appendere_literis(a, "</IsTruncated>");
    si (truncata)
    {
        chorda_aedificator_appendere_literis(a,
            "<NextContinuationToken>");
        chorda_aedificator_appendere_chorda(a, base64_codificare(
            ultimum.datum, ultimum.mensura, p));
        chorda_aedificator_appendere_literis(a,
            "</NextContinuationToken>");
    }
    chorda_aedificator_appendere_chorda(a, chorda_aedificator_finire(
        contenta));
    chorda_aedificator_appendere_literis(a, "</ListBucketResult>");
    r          = _sim_responsum(CC, p);
    r->corpus  = chorda_aedificator_finire(a);
    _sim_caput_addere(r, "Content-Type", _sim_l("application/xml", p),
        p);
    redde _sim_bene(r);
}

/* ===== multipars ===== */
/* onus vivum per id (NIHIL si abest) */
interior SimulacrumOnus*
_sim_onus (
                Simulacrum* s,
     constans SignaturaPar* q,
                       i32  nq,
                    chorda  clavis)
{
    s32 iu = _sim_par(q, nq, "uploadId");
    i32 i;

    si (iu < ZEPHYRUM)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < s->onera_numerus; i++)
    {
        SimulacrumOnus* o = &s->onera[i];

        si (   o->vivum
            && (   chorda_aequalis(o->id, q[iu].valor)
                || (   s->id_oneris_alienum
                    && chorda_aequalis(chorda_concatenare(_sim_l("L-",
                        s->piscina), o->id, s->piscina), q[iu].valor)))
            && chorda_aequalis(o->clavis, clavis))
        {
            redde o;
        }
    }
    redde NIHIL;
}

interior vacuum
_sim_onus_finire (
    SimulacrumOnus* o)
{
    si (o->vivum)
    {
        piscina_destruere(o->piscina);
        o->piscina  = NIHIL;
        o->vivum    = FALSUM;
    }
}

interior HttpResultus
_sim_mp_incipere (
                Simulacrum* s,
    constans HttpPetitioVisus* v,
                    chorda  clavis,
                   Piscina* p)
{
       SimulacrumOnus* o;
            HttpCaput  meta[XVI];
                  i32  n;
                  i32  i;
        HttpResponsum* r;
    ChordaAedificator* a;
            character  id[XXXII];

    si (s->onera_numerus >= SIMULACRUM_ONERA_MAXIMA)
    {
        redde _sim_error(s, DIII, "SlowDown", ZEPHYRUM, p);
    }
    o = &s->onera[s->onera_numerus];
    memset(o, ZEPHYRUM, magnitudo(*o));
    s->onera_numerus++;
    sprintf(id, "onus-%u", (insignatus integer)s->onera_numerus);
    o->piscina         = piscina_generare_dynamicum("simulacrum_onus",
        (memoriae_index)(LXIV * MXXIV));
    o->vivum   = VERUM;
    o->clavis  = chorda_transcribere(clavis, o->piscina);
    o->id      = chorda_transcribere(_sim_l(id, p), o->piscina);
    o->genus_contenti  = chorda_transcribere(_sim_caput(v,
        "Content-Type",
        p), o->piscina);
    o->initium  = s->hora
        > ZEPHYRUM ? s->hora : SIMULACRUM_TEMPUS;
    n           = _sim_meta_petitionis(v, meta, p);
    o->meta            = (HttpCaput*)piscina_allocare(o->piscina,
        (memoriae_index)((n + I) * (i32)magnitudo(HttpCaput)));
    per (i = ZEPHYRUM; i < n; i++)
    {
        o->meta[i].titulus = chorda_transcribere(meta[i].titulus,
            o->piscina);
        o->meta[i].valor = chorda_transcribere(meta[i].valor,
            o->piscina);
    }
    o->meta_numerus  = n;
    a                = chorda_aedificator_creare(p, CCLVI);
    chorda_aedificator_appendere_literis(a,
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<InitiateMultipartUploadResult><Bucket>" SIMULACRUM_SITULA
        "</Bucket><Key>");
    _sim_effugere(a, clavis);
    chorda_aedificator_appendere_literis(a, "</Key><UploadId>");
    chorda_aedificator_appendere_literis(a, id);
    chorda_aedificator_appendere_literis(a,
        "</UploadId></InitiateMultipartUploadResult>");
    r          = _sim_responsum(CC, p);
    r->corpus  = chorda_aedificator_finire(a);
    redde _sim_bene(r);
}

interior HttpResultus
_sim_mp_pars (
                   Simulacrum* s,
    constans HttpPetitioVisus* v,
        constans SignaturaPar* q,
                          i32  nq,
                       chorda  clavis,
                      Piscina* p)
{
    SimulacrumOnus* o        = _sim_onus(s, q, nq, clavis);
               s32  ip       = _sim_par(q, nq, "partNumber");
               s32  numerus  = ZEPHYRUM;
               i32  i;
    SimulacrumPars* t = NIHIL;
     HttpResponsum* r;

    si (!o)
    {
        redde _sim_error(s, CDIV, "NoSuchUpload", ZEPHYRUM, p);
    }
    si (   ip < ZEPHYRUM || !chorda_ut_s32(q[ip].valor, &numerus)
        || numerus < I || numerus > (s32)(X * M))
    {
        redde _sim_error(s, CD, "InvalidArgument", ZEPHYRUM, p);
    }
    per (i = ZEPHYRUM; i < o->partes_numerus; i++)
    {
        si (o->partes[i].numerus == (i32)numerus)
        {
            t = &o->partes[i];
        }
    }
    si (!t)
    {
        si (o->partes_numerus >= SIMULACRUM_PARTES_MAXIMAE)
        {
            redde _sim_error(s, CD, "InvalidArgument", ZEPHYRUM, p);
        }
        t = &o->partes[o->partes_numerus];
        o->partes_numerus++;
    }
    t->numerus  = (i32)numerus;
    t->corpus   = chorda_transcribere(v->corpus, o->piscina);
    md5_friare(v->corpus.datum, v->corpus.mensura, t->digestum);
    t->etag  = _sim_hex(t->digestum, MD5_DIGEST_MENSURA, o->piscina);
    r        = _sim_responsum(CC, p);
    _sim_caput_addere(r, "ETag", chorda_concatenare(chorda_concatenare(
        _sim_l("\"", p), t->etag, p), _sim_l("\"", p), p), p);
    redde _sim_bene(r);
}

interior SimulacrumPars*
_sim_pars_numeri (
    SimulacrumOnus* o,
               i32  numerus)
{
    i32 i;

    per (i = ZEPHYRUM; i < o->partes_numerus; i++)
    {
        si (o->partes[i].numerus == numerus)
        {
            redde &o->partes[i];
        }
    }
    redde NIHIL;
}

interior HttpResultus
_sim_mp_perficere (
                Simulacrum* s,
    constans HttpPetitioVisus* v,
    constans SignaturaPar* q,
                       i32  nq,
                    chorda  clavis,
                   Piscina* p)
{
       SimulacrumOnus* o = _sim_onus(s, q, nq, clavis);
          XmlResultus  x;
                  Xar* partes;
                  i32  n;
                  i32  i;
                  i32  prior  = ZEPHYRUM;
                  i64  tota   = ZEPHYRUM;
       SimulacrumPars* ordo[SIMULACRUM_PARTES_MAXIMAE];
                   i8* corpus;
                  i64  ab = ZEPHYRUM;
         MD5Contextus  ctx;
                   i8  digestum[MD5_DIGEST_MENSURA];
            character  suffixum[XVI];
   SimulacrumObiectum* ob;
        HttpResponsum* r;
    ChordaAedificator* a;

    si (!o)
    {
        redde _sim_error(s, CDIV, "NoSuchUpload", ZEPHYRUM, p);
    }
    x = xml_legere(v->corpus, p, internamentum_creare(p));
    si (!x.successus || !x.radix)
    {
        redde _sim_error(s, CD, "MalformedXML", ZEPHYRUM, p);
    }
    partes  = xml_invenire_omnes_liberos(x.radix, "Part", p);
    n       = partes ? (i32)xar_numerus(partes) : ZEPHYRUM;
    si (n == ZEPHYRUM || n > SIMULACRUM_PARTES_MAXIMAE)
    {
        redde _sim_error(s, CD, "MalformedXML", ZEPHYRUM, p);
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
              XmlNodus* nd = *(XmlNodus**)xar_obtinere(partes, i);
              XmlNodus* pn = xml_invenire_liberum(nd, "PartNumber");
              XmlNodus* pe = xml_invenire_liberum(nd, "ETag");
                   s32  numerus = ZEPHYRUM;
                chorda  etag;
        SimulacrumPars* t;

        si (   !pn || !pe
            || !chorda_ut_s32(xml_textus_internus(pn, p), &numerus))
        {
            redde _sim_error(s, CD, "MalformedXML", ZEPHYRUM, p);
        }
        si ((i32)numerus <= prior)
        {
            redde _sim_error(s, CD, "InvalidPartOrder", ZEPHYRUM, p);
        }
        prior  = (i32)numerus;
        etag   = xml_textus_internus(pe, p);
        si (etag.mensura >= II && etag.datum[ZEPHYRUM] == '"')
        {
            etag = chorda_sectio(etag, I, etag.mensura - I);
        }
        t = _sim_pars_numeri(o, (i32)numerus);
        si (!t || !chorda_aequalis(t->etag, etag))
        {
            redde _sim_error(s, CD, "InvalidPart", ZEPHYRUM, p);
        }
        si (   i < n - I
            && (i64)t->corpus.mensura < (i64)(V * MXXIV * MXXIV))
        {
            redde _sim_error(s, CD, "EntityTooSmall", ZEPHYRUM, p);
        }
        ordo[i]  = t;
        tota     += (i64)t->corpus.mensura;
    }
    corpus = (i8*)piscina_allocare(s->piscina, (memoriae_index)(tota
        + I));
    md5_initiare(&ctx);
    per (i = ZEPHYRUM; i < n; i++)
    {
        memcpy(corpus + ab, ordo[i]->corpus.datum,
            (size_t)ordo[i]->corpus.mensura);
        ab += (i64)ordo[i]->corpus.mensura;
        md5_addere(&ctx, ordo[i]->digestum, MD5_DIGEST_MENSURA);
    }
    md5_finire(&ctx, digestum);
    sprintf(suffixum, "-%u", (insignatus integer)n);
    ob          = _sim_locus(s, clavis);
    ob->corpus  = chorda_ex_buffer(corpus, (i32)tota);
    ob->etag            = chorda_concatenare(_sim_hex(digestum,
        MD5_DIGEST_MENSURA, p), _sim_l(suffixum, p), s->piscina);
    ob->genus_contenti  = chorda_transcribere(o->genus_contenti,
        s->piscina);
    ob->mutatum         = s->hora > ZEPHYRUM ? s->hora
                                             : SIMULACRUM_TEMPUS;
    ob->meta_numerus    = o->meta_numerus;
    ob->meta            = (HttpCaput*)piscina_allocare(s->piscina,
        (memoriae_index)((o->meta_numerus + I)
            * (i32)magnitudo(HttpCaput)));
    per (i = ZEPHYRUM; i < o->meta_numerus; i++)
    {
        ob->meta[i].titulus = chorda_transcribere(o->meta[i].titulus,
            s->piscina);
        ob->meta[i].valor = chorda_transcribere(o->meta[i].valor,
            s->piscina);
    }
    a = chorda_aedificator_creare(p, CCLVI);
    chorda_aedificator_appendere_literis(a,
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<CompleteMultipartUploadResult><Location>http://simulacrum."
        "invalid/" SIMULACRUM_SITULA "/</Location><Bucket>"
        SIMULACRUM_SITULA "</Bucket><Key>");
    _sim_effugere(a, clavis);
    chorda_aedificator_appendere_literis(a, "</Key><ETag>&quot;");
    chorda_aedificator_appendere_chorda(a, ob->etag);
    chorda_aedificator_appendere_literis(a,
        "&quot;</ETag></CompleteMultipartUploadResult>");
    _sim_onus_finire(o);
    r          = _sim_responsum(CC, p);
    r->corpus  = chorda_aedificator_finire(a);
    redde _sim_bene(r);
}

interior HttpResultus
_sim_mp_partes (
                Simulacrum* s,
     constans SignaturaPar* q,
                       i32  nq,
                    chorda  clavis,
                   Piscina* p)
{
       SimulacrumOnus* o = _sim_onus(s, q, nq, clavis);
                  i32  i;
        HttpResponsum* r;
    ChordaAedificator* a;

    si (!o)
    {
        redde _sim_error(s, CDIV, "NoSuchUpload", ZEPHYRUM, p);
    }
    a = chorda_aedificator_creare(p, MXXIV);
    chorda_aedificator_appendere_literis(a,
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<ListPartsResult>"
        "<Bucket>" SIMULACRUM_SITULA "</Bucket><Key>");
    _sim_effugere(a, clavis);
    chorda_aedificator_appendere_literis(a, "</Key><UploadId>");
    chorda_aedificator_appendere_chorda(a, o->id);
    chorda_aedificator_appendere_literis(a,
        "</UploadId><IsTruncated>false</IsTruncated>");
    per (i = ZEPHYRUM; i < o->partes_numerus; i++)
    {
        chorda_aedificator_appendere_literis(a, "<Part><PartNumber>");
        chorda_aedificator_appendere_chorda(a, _sim_numerus((s64)
            o->partes[i].numerus, p));
        chorda_aedificator_appendere_literis(a,
            "</PartNumber><ETag>&quot;");
        chorda_aedificator_appendere_chorda(a, o->partes[i].etag);
        chorda_aedificator_appendere_literis(a, "&quot;</ETag><Size>");
        chorda_aedificator_appendere_chorda(a, _sim_numerus((s64)
            o->partes[i].corpus.mensura, p));
        chorda_aedificator_appendere_literis(a, "</Size></Part>");
    }
    chorda_aedificator_appendere_literis(a, "</ListPartsResult>");
    r          = _sim_responsum(CC, p);
    r->corpus  = chorda_aedificator_finire(a);
    redde _sim_bene(r);
}

interior HttpResultus
_sim_mp_pendentes (
                Simulacrum* s,
     constans SignaturaPar* q,
                       i32  nq,
                   Piscina* p)
{
                  s32 ip = _sim_par(q, nq, "prefix");
               chorda praefixum = ip >= ZEPHYRUM ? q[ip].valor
                                  : chorda_ex_buffer(NIHIL, ZEPHYRUM);
                  i32  i;
        HttpResponsum* r;
    ChordaAedificator* a = chorda_aedificator_creare(p, MXXIV);

    chorda_aedificator_appendere_literis(a,
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<ListMultipartUploadsResult><Bucket>" SIMULACRUM_SITULA
        "</Bucket><Prefix>");
    _sim_effugere(a, praefixum);
    chorda_aedificator_appendere_literis(a,
        "</Prefix><IsTruncated>false</IsTruncated>");
    per (i = ZEPHYRUM; i < s->onera_numerus; i++)
    {
        SimulacrumOnus* o = &s->onera[i];

        si (   !o->vivum || s->celare_onera
            || (   praefixum.mensura > ZEPHYRUM
                && !chorda_incipit(o->clavis, praefixum)))
        {
            perge;
        }
        chorda_aedificator_appendere_literis(a, "<Upload><Key>");
        _sim_effugere(a, o->clavis);
        chorda_aedificator_appendere_literis(a, "</Key><UploadId>");
        si (s->id_oneris_alienum)
        {
            chorda_aedificator_appendere_literis(a, "L-");
        }
        chorda_aedificator_appendere_chorda(a, o->id);
        chorda_aedificator_appendere_literis(a,
            "</UploadId><Initiated>");
        chorda_aedificator_appendere_chorda(a,
            _sim_tempus_iso(o->initium,
            p));
        chorda_aedificator_appendere_literis(a,
            "</Initiated></Upload>");
    }
    chorda_aedificator_appendere_literis(a,
        "</ListMultipartUploadsResult>");
    r          = _sim_responsum(CC, p);
    r->corpus  = chorda_aedificator_finire(a);
    redde _sim_bene(r);
}

/* TRUNCARE: corpus dimidium cum Content-Length congruo (servus qui
 * minus mittit quam petitum, sine errore transportus) */
interior vacuum
_sim_truncare (
    HttpResultus* res,
         Piscina* p)
{
    HttpResponsum* r = res->responsum;
              i32  i;

    si (!res->successus || !r || r->corpus.mensura == ZEPHYRUM)
    {
        redde;
    }
    r->corpus = chorda_sectio(r->corpus, ZEPHYRUM, r->corpus.mensura
        / II);
    per (i = ZEPHYRUM; i < r->capita_numerus; i++)
    {
        si (chorda_aequalis_literis(r->capita[i].titulus,
            "Content-Length"))
        {
            r->capita[i].valor = _sim_numerus((s64)r->corpus.mensura,
                p);
        }
    }
}

/* MUTARE: post responsum obiectum alio corpore (eadem mensura)
 * substituitur - ETag novum */
interior vacuum
_sim_mutare (
    Simulacrum* s,
        chorda  clavis)
{
                   i32  index;
    SimulacrumObiectum* o;
                    i8* t;

    si (!_sim_invenire(s, clavis, &index))
    {
        redde;
    }
    o = &s->obiecta[index];
    si (o->corpus.mensura == ZEPHYRUM)
    {
        redde;
    }
    t = (i8*)piscina_allocare(s->piscina,
        (memoriae_index)o->corpus.mensura);
    memcpy(t, o->corpus.datum, (size_t)o->corpus.mensura);
    t[ZEPHYRUM]  = (i8)(t[ZEPHYRUM] ^ 0xFF);
    o->corpus    = chorda_ex_buffer(t, o->corpus.mensura);
    o->etag      = _sim_etag(o->corpus, s->piscina);
}

interior HttpResultus
_sim_exsequi (
    HttpPetitio* pet,
        Piscina* p,
         vacuum* datum)
{
            Simulacrum* s         = (Simulacrum*)datum;
      HttpPetitioVisus  v         = http_petitio_visus(pet);
                chorda  via       = v.via;
                chorda  quaestio  = chorda_ex_buffer(NIHIL, ZEPHYRUM);
                chorda  radix     = _sim_l("/" SIMULACRUM_SITULA, p);
                chorda  clavis    = chorda_ex_buffer(NIHIL, ZEPHYRUM);
                   s32  signum = chorda_invenire_index(v.via,
                       _sim_l("?",
                              p));
            SignaturaPar  q[SIMULACRUM_PARIA_MAXIMA];
                     i32  nq = ZEPHYRUM;
        SimulacrumVitium* vitium;
            HttpResultus  res;
                     b32  abrumpere;
                Sigillum  sg;
               character  hex[LXV];
                     b32  praesignata;
                     b32  anonyma;
                     i32  locus_anonymi;

    s->petitiones++;
    si ((i32)v.corpus.mensura > s->corpus_maximum)
    {
        s->corpus_maximum = (i32)v.corpus.mensura;
    }
    si (signum >= ZEPHYRUM)
    {
        via = chorda_sectio(v.via, ZEPHYRUM, (i32)signum);
        quaestio = chorda_sectio(v.via, (i32)signum + I,
            v.via.mensura);
    }
    dum (quaestio.mensura > ZEPHYRUM && nq < SIMULACRUM_PARIA_MAXIMA)
    {
           s32 et  = chorda_invenire_index(quaestio, _sim_l("&", p));
        chorda par = et < ZEPHYRUM ? quaestio
                                   : chorda_sectio(quaestio, ZEPHYRUM,
                                       (i32)et);
           s32 aeq = chorda_invenire_index(par, _sim_l("=", p));

        q[nq].titulus = url_decodificare(aeq < ZEPHYRUM ? par
            : chorda_sectio(par, ZEPHYRUM, (i32)aeq), p);
        q[nq].valor = aeq < ZEPHYRUM ? chorda_ex_buffer(NIHIL, ZEPHYRUM)
            : url_decodificare(chorda_sectio(par, (i32)aeq + I,
                par.mensura), p);
        nq++;
        si (et < ZEPHYRUM)
        {
            frange;
        }
        quaestio = chorda_sectio(quaestio, (i32)et + I,
            quaestio.mensura);
    }
    via = url_decodificare(via, p);
    praesignata = _sim_par(q, nq, "X-Amz-Signature") >= ZEPHYRUM
        && _sim_caput(&v, "Authorization", p).mensura == ZEPHYRUM;
    /* sine ulla signatura: solum GET obiecti publici */
    anonyma = !praesignata
        && _sim_caput(&v, "Authorization", p).mensura == ZEPHYRUM;

    si (s->hora > ZEPHYRUM && !praesignata && !anonyma)
    {
        s64 t = (s64)_sim_dies_amz(_sim_caput(&v, "x-amz-date", p), p);
        s64 d = t - (s64)s->hora;

        si (d > (s64)CM || d < -(s64)CM)
        {
            redde _sim_error(s, CDIII, "RequestTimeTooSkewed", ZEPHYRUM,
                p);
        }
    }
    si (   !anonyma
        && (praesignata ? !_sim_praesignata_valida(s, &v, via, q, nq, p)
                        : !_sim_signatura_valida(s, &v, via, q, nq, p)))
    {
        redde _sim_error(s, CDIII, "SignatureDoesNotMatch", ZEPHYRUM,
            p);
    }
    sg = sigillum_computare(v.corpus.datum,
        (memoriae_index)v.corpus.mensura);
    sigillum_hex(&sg, hex);
    si (   !praesignata && !anonyma
        && !chorda_aequalis_literis(_sim_caput(&v,
        "x-amz-content-sha256",
            p), hex))
    {
        redde _sim_error(s, CD, "XAmzContentSHA256Mismatch", ZEPHYRUM,
            p);
    }
    /* stilus hospitis (situla.hospes/clavis) -> stilus viae */
    si (chorda_incipit(_sim_caput(&v, "Host", p),
            _sim_l(SIMULACRUM_SITULA ".", p)))
    {
        via = chorda_concatenare(radix, via, p);
    }
    si (!chorda_incipit(via, radix))
    {
        redde _sim_error(s, CDIV, "NoSuchBucket", ZEPHYRUM, p);
    }
    via = chorda_sectio(via, radix.mensura, via.mensura);
    si (via.mensura > ZEPHYRUM && via.datum[ZEPHYRUM] != '/')
    {
        redde _sim_error(s, CDIV, "NoSuchBucket", ZEPHYRUM, p);
    }
    si (via.mensura > ZEPHYRUM)
    {
        clavis = chorda_sectio(via, I, via.mensura);
    }
    si (anonyma)
    {
        si (   v.methodus == HTTP_GET
            && _sim_invenire(s, clavis, &locus_anonymi)
            && s->obiecta[locus_anonymi].publicum)
        {
            redde _sim_capere(s, &v, clavis, FALSUM, p);
        }
        redde _sim_error(s, CDIII, "AccessDenied", ZEPHYRUM, p);
    }

    vitium = _sim_vitium(s, v.methodus, clavis, q, nq);
    si (vitium && vitium->genus == SIMULACRUM_VITIUM_STATUS)
    {
        redde _sim_error(s, vitium->status, vitium->codex,
            vitium->secundae, p);
    }
    abrumpere = vitium && vitium->genus
        == SIMULACRUM_VITIUM_SCRIBERE_ABRUMPERE;

    commutatio (v.methodus)
    {
        casus HTTP_PUT:
            si (_sim_par(q, nq, "uploadId") >= ZEPHYRUM)
            {
                res = _sim_mp_pars(s, &v, q, nq, clavis, p);
            }
            alioquin si (_sim_caput(&v, "x-amz-copy-source", p).mensura
                         > ZEPHYRUM)
            {
                res = _sim_copiare(s, &v, clavis, p);
            }
            alioquin
            {
                res = _sim_ponere(s, &v, clavis, p);
            }
            frange;
        casus HTTP_GET:
            si (   clavis.mensura             == ZEPHYRUM
                && _sim_par(q, nq, "uploads") >= ZEPHYRUM)
            {
                res = _sim_mp_pendentes(s, q, nq, p);
            }
            alioquin si (_sim_par(q, nq, "uploadId") >= ZEPHYRUM)
            {
                res = _sim_mp_partes(s, q, nq, clavis, p);
            }
            alioquin si (   clavis.mensura               == ZEPHYRUM
                         && _sim_par(q, nq, "list-type") >= ZEPHYRUM)
            {
                res = _sim_enumerare(s, q, nq, p);
            }
            alioquin
            {
                res = _sim_capere(s, &v, clavis, FALSUM, p);
            }
            frange;
        casus HTTP_HEAD:
            res = _sim_capere(s, &v, clavis, VERUM, p);
            frange;
        casus HTTP_DELETE:
            si (_sim_par(q, nq, "uploadId") >= ZEPHYRUM)
            {
                SimulacrumOnus* o = _sim_onus(s, q, nq, clavis);

                si (o)
                {
                    _sim_onus_finire(o);
                    res = _sim_bene(_sim_responsum(CCIV, p));
                }
                alioquin
                {
                    res = _sim_error(s, CDIV, "NoSuchUpload", ZEPHYRUM,
                        p);
                }
                frange;
            }
            _sim_removere(s, clavis);
            res = _sim_bene(_sim_responsum(CCIV, p));
            frange;
        casus HTTP_POST:
            si (_sim_par(q, nq, "delete") >= ZEPHYRUM)
            {
                res = _sim_delere_plura(s, &v, p);
            }
            alioquin si (_sim_par(q, nq, "uploads") >= ZEPHYRUM)
            {
                res = _sim_mp_incipere(s, &v, clavis, p);
            }
            alioquin si (_sim_par(q, nq, "uploadId") >= ZEPHYRUM)
            {
                res = _sim_mp_perficere(s, &v, q, nq, clavis, p);
            }
            alioquin
            {
                res = _sim_error(s, CD, "NotImplemented", ZEPHYRUM, p);
            }
            frange;
        ordinarius:
            res = _sim_error(s, CDV, "MethodNotAllowed", ZEPHYRUM, p);
            frange;
    }
    si (vitium && vitium->genus == SIMULACRUM_VITIUM_TRUNCARE)
    {
        _sim_truncare(&res, p);
    }
    si (vitium && vitium->genus == SIMULACRUM_VITIUM_MUTARE)
    {
        _sim_mutare(s, clavis);
    }
    si (abrumpere)
    {
        memset(&res, ZEPHYRUM, magnitudo(res));
        res.error = HTTP_ERROR_IO;
        res.error_descriptio = _sim_l("simulacrum: connexio abrupta",
            p);
    }
    redde res;
}

/* ===== aditus ===== */
interior Simulacrum*
simulacrum_creare (
     chorda  clavis_id,
     chorda  secretum,
     chorda  regio,
    Piscina* p)
{
    Simulacrum* s = (Simulacrum*)piscina_allocare(p,
        (memoriae_index)magnitudo(Simulacrum));

    memset(s, ZEPHYRUM, magnitudo(*s));
    s->piscina    = p;
    s->clavis_id  = chorda_transcribere(clavis_id, p);
    s->secretum   = chorda_transcribere(secretum, p);
    s->regio      = chorda_transcribere(regio, p);
    redde s;
}

interior vacuum
simulacrum_vitium_addere (
          Simulacrum* s,
    SimulacrumVitium  v)
{
    si (s->vitia_numerus < SIMULACRUM_VITIA_MAXIMA)
    {
        s->vitia[s->vitia_numerus] = v;
        s->vitia_numerus++;
    }
}

/* horologium servi; 0 = deviatio non probatur */
interior vacuum
simulacrum_horam_ponere (
     Simulacrum* s,
    MomentumSec  hora)
{
    s->hora = hora;
}

interior HttpVectura
simulacrum_vectura (
    Simulacrum* s)
{
    HttpVectura v;

    v.exsequi  = _sim_exsequi;
    v.datum    = s;
    redde v;
}

interior i32
simulacrum_petitiones (
    Simulacrum* s)
{
    redde s->petitiones;
}

/* sine HTTP (sine meta) */
interior vacuum
simulacrum_obiectum_ponere (
    Simulacrum* s,
        chorda  clavis,
        chorda  corpus)
{
    _sim_scribere(s, clavis, corpus, chorda_ex_buffer(NIHIL, ZEPHYRUM),
        NIHIL, ZEPHYRUM);
}

/* maximum corpus petitionis visum (pars una in memoria) */
interior i32
simulacrum_corpus_maximum (
    Simulacrum* s)
{
    redde s->corpus_maximum;
}

/* petitiones GET/HEAD cum If-Match */
interior i32
simulacrum_si_etag_visa (
    Simulacrum* s)
{
    redde s->si_etag_visa;
}

/* corpus repositum (vacuum si abest) - sine HTTP */
interior chorda
simulacrum_obiectum_corpus (
    Simulacrum* s,
        chorda  clavis)
{
    i32 index;

    si (!_sim_invenire(s, clavis, &index))
    {
        redde chorda_ex_buffer(NIHIL, ZEPHYRUM);
    }
    redde s->obiecta[index].corpus;
}

/* ETag repositum substituit (corruptio pro probatione md5) */
interior vacuum
simulacrum_etag_ponere (
             Simulacrum* s,
                 chorda  clavis,
     constans character* etag)
{
    i32 index;

    si (_sim_invenire(s, clavis, &index))
    {
        s->obiecta[index].etag = _sim_l(etag, s->piscina);
    }
}

nomen structura {
         vacuum (*vitium_addere)(Simulacrum* s, SimulacrumVitium v);
         vacuum (*horam_ponere)(Simulacrum* s, MomentumSec hora);
    HttpVectura (*vectura)(Simulacrum* s);
            i32 (*petitiones)(Simulacrum* s);
         vacuum (*obiectum_ponere)(Simulacrum* s, chorda clavis,
                     chorda corpus);
            i32 (*corpus_maximum)(Simulacrum* s);
            i32 (*si_etag_visa)(Simulacrum* s);
         chorda (*obiectum_corpus)(Simulacrum* s, chorda clavis);
         vacuum (*etag_ponere)(Simulacrum* s, chorda clavis,
                     constans character* etag);
} SimulacrumAditus;

hic_manens constans SimulacrumAditus simulacrum_aditus = {
    simulacrum_vitium_addere,
    simulacrum_horam_ponere,
    simulacrum_vectura,
    simulacrum_petitiones,
    simulacrum_obiectum_ponere,
    simulacrum_corpus_maximum,
    simulacrum_si_etag_visa,
    simulacrum_obiectum_corpus,
    simulacrum_etag_ponere
};

/* onera reliqua (piscinae propriae) deletur */
interior vacuum
simulacrum_claudere (
    Simulacrum* s)
{
    i32 i;

    (vacuum)simulacrum_aditus.vectura;
    per (i = ZEPHYRUM; i < s->onera_numerus; i++)
    {
        _sim_onus_finire(&s->onera[i]);
    }
}

#endif /* HORREUM_SIMULACRUM_H */

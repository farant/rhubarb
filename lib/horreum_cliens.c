/* horreum_cliens.c - stratum obstans horrei (vide horreum_cliens.h;
 * horreum-plan T3)
 *
 * Sedes (HorreumCliens) in piscina parandi: optiones impletae, sors
 * morarum, deviatio horologii inventa. Sine sede quaeque vocatio
 * ordinarias in acervo habet (deviatio tunc non manet).
 *
 * Ansa exsequendi: tempus = nunc + deviatio; signatur DE NOVO;
 * mittitur; legitur. Error -> (a) RequestTimeTooSkewed cum deviatione
 * et nondum correctum: deviatio servatur, iteratur sine mora; (b) 412
 * in opere
 * condicionali POST iterationem: HEAD, x-amz-meta-sha256 comparatur -
 * nostrum = successus iam_nostrum, aliter CONDITIO verus; (c) politica
 * horreum_iterandum (transitoria, idempotentia, conatus) -> dormire,
 * iterare; ceterum finis. (d) CompleteMultipartUpload NoSuchUpload
 * post iterationem: HEAD, ETag == expectatum -> iam_nostrum.
 *
 * Fila: pars una (aut frustum unum) in piscina propria, post missionem
 * destructa - memoria fili magnitudine non crescit. */
#include "postulata_posix.h"
#include "latina.h"
#include "horreum_cliens.h"
#include "sors.h"
#include "friatio.h"
#include "filum.h"

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

nomen structura {
    HorreumClientisOptiones optiones;
                       Sors sors;
                        s64 deviatio_s;
} HorreumCliens;

interior MomentumSec
_nunc_ordinarium (
    vacuum* datum)
{
    (vacuum)datum;
    redde (MomentumSec)time(NIHIL);
}

interior vacuum
_dormire_ordinarium (
    vacuum* datum,
       i32  ms)
{
    structura timespec pausa;

    (vacuum)datum;
    pausa.tv_sec   = (time_t)(ms / M);
    pausa.tv_nsec  = (longus)(ms % M) * (longus)M * (longus)M;
    (vacuum)nanosleep(&pausa, NIHIL);
}

interior vacuum
_cliens_implere (
                       HorreumCliens* c,
    constans HorreumClientisOptiones* o)
{
    memset(c, ZEPHYRUM, magnitudo(*c));
    si (o)
    {
        c->optiones = *o;
    }
    si (!c->optiones.vectura.exsequi)
    {
        c->optiones.vectura = http_vectura_ordinaria();
    }
    si (!c->optiones.nunc)
    {
        c->optiones.nunc = _nunc_ordinarium;
    }
    si (!c->optiones.dormire)
    {
        c->optiones.dormire = _dormire_ordinarium;
    }
    si (c->optiones.conatus_maximi == ZEPHYRUM)
    {
        c->optiones.conatus_maximi = III;
    }
    si (c->optiones.mora_basis_ms == ZEPHYRUM)
    {
        c->optiones.mora_basis_ms = CC;
    }
    sors_seminare(&c->sors, c->optiones.semen != ZEPHYRUM
        ? c->optiones.semen : (i64)time(NIHIL), ZEPHYRUM);
}

vacuum
horreum_cliens_parare (
                             Horreum* h,
    constans HorreumClientisOptiones* o,
                             Piscina* p)
{
    HorreumCliens* c;

    si (!h || !p)
    {
        redde;
    }
    c = (HorreumCliens*)piscina_allocare(p,
        (memoriae_index)magnitudo(HorreumCliens));
    _cliens_implere(c, o);
    horreum_sedem_ponere(h, c);
}

/* obiectum fert x-amz-meta-sha256 == sigillum nostrum? */
interior b32
_sigillum_nostrum (
    constans HorreumObjectum* ob,
                      chorda  sigillum)
{
    i32 i;

    per (i = ZEPHYRUM; i < ob->meta_numerus; i++)
    {
        si (   chorda_aequalis_literis(ob->meta[i].titulus, "sha256")
            && chorda_aequalis(ob->meta[i].valor, sigillum))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* II.5: 412 post iterationem - HEAD clavis; facta HEAD in ob si
 * nostrum */
interior b32
_iam_nostrum (
                 Horreum* h,
    constans HorreumOpus* opus,
         HorreumObjectum* ob,
                 Piscina* p)
{
        HorreumOpus inspectio;
       HorreumError e;
    HorreumObjectum facta;

    si (   !horreum_opus_inspicere(h, opus->situla, opus->clavis,
                &inspectio, &e, p)
        || !horreum_exsequi(h, &inspectio, NIHIL, &facta, &e, p)
        || !_sigillum_nostrum(&facta, opus->corpus_sha256_hex))
    {
        redde FALSUM;
    }
    facta.iam_nostrum = VERUM;
    si (ob)
    {
        *ob = facta;
    }
    redde VERUM;
}

/* II.5: CompleteMultipartUpload NoSuchUpload post iterationem - HEAD,
 * ETag expectatum = perfectum nostrum */
interior b32
_perfectum_nostrum (
                 Horreum* h,
    constans HorreumOpus* opus,
         HorreumObjectum* ob,
                 Piscina* p)
{
        HorreumOpus inspectio;
       HorreumError e;
    HorreumObjectum facta;

    si (   !horreum_opus_inspicere(h, opus->situla, opus->clavis,
                &inspectio, &e, p)
        || !horreum_exsequi(h, &inspectio, NIHIL, &facta, &e, p)
        || !chorda_aequalis(facta.etag,
        opus->perficere_etag_expectatum))
    {
        redde FALSUM;
    }
    facta.iam_nostrum = VERUM;
    si (ob)
    {
        *ob = facta;
    }
    redde VERUM;
}

b32
horreum_exsequi (
                 Horreum* h,
    constans HorreumOpus* opus,
           HorreumExitus* x,
         HorreumObjectum* ob,
            HorreumError* e,
                 Piscina* p)
{
     HorreumCliens  locus;
     HorreumCliens* c;
     HorreumExitus  x_locus;
      HorreumError  e_locus;
               b32  correctum = FALSUM;

    si (!x)
    {
        x = &x_locus;
    }
    si (!e)
    {
        e = &e_locus;
    }
    memset(x, ZEPHYRUM, magnitudo(*x));
    memset(e, ZEPHYRUM, magnitudo(*e));
    si (!h || !opus)
    {
        e->genus    = HORREUM_MALUM;
        e->nuntius  = chorda_ex_literis(
            "horreum: exsequi sine horreo aut opere", p);
        redde FALSUM;
    }
    c = (HorreumCliens*)horreum_sedes(h);
    si (!c)
    {
        _cliens_implere(&locus, NIHIL);
        c = &locus;
    }
    dum (VERUM)
    {
        MomentumSec  nunc    = c->optiones.nunc(c->optiones.datum);
        MomentumSec  tempus  = nunc + (MoraSec)c->deviatio_s;
        HttpPetitio* pet = horreum_petitionem_signare(h, opus, tempus,
            e,
            p);
        HttpResultus res;
              chorda mora_servi = chorda_ex_buffer(NIHIL, ZEPHYRUM);
                 i32 mora;

        si (!pet)
        {
            redde FALSUM;
        }
        res            = http_vectura_exsequi(c->optiones.vectura, pet,
            p);
        x->conatus++;
        x->responsum   = res.responsum;
        x->deviatio_s  = c->deviatio_s;
        si (horreum_responsum_legere(opus, res, tempus, ob, e, p))
        {
            x->successus = VERUM;
            redde VERUM;
        }
        si (   e->genus      == HORREUM_TRANSITORIUM
            && e->deviatio_s != ZEPHYRUM
            && !correctum)
        {
            c->deviatio_s  += e->deviatio_s;
            x->deviatio_s  = c->deviatio_s;
            correctum      = VERUM;
            perge;
        }
        si (   e->genus == HORREUM_CONDITIO && opus->condicionale
            && x->conatus > I)
        {
            HorreumError conditio = *e;

            si (_iam_nostrum(h, opus, ob, p))
            {
                memset(e, ZEPHYRUM, magnitudo(*e));
                x->successus    = VERUM;
                x->iam_nostrum  = VERUM;
                redde VERUM;
            }
            *e = conditio;
            redde FALSUM;
        }
        si (   e->genus    == HORREUM_ABEST
            && opus->genus == HORREUM_OPUS_MP_PERFICERE
            && x->conatus > I
            && opus->perficere_etag_expectatum.mensura > ZEPHYRUM)
        {
            HorreumError abest = *e;

            si (_perfectum_nostrum(h, opus, ob, p))
            {
                memset(e, ZEPHYRUM, magnitudo(*e));
                x->successus    = VERUM;
                x->iam_nostrum  = VERUM;
                redde VERUM;
            }
            *e = abest;
            redde FALSUM;
        }
        si (res.responsum)
        {
            mora_servi = http_responsum_caput(res.responsum,
                "Retry-After");
        }
        si (!horreum_iterandum(opus, e, mora_servi, x->conatus,
                c->optiones.conatus_maximi, c->optiones.mora_basis_ms,
                sors_proximum(&c->sors), &mora))
        {
            redde FALSUM;
        }
        c->optiones.dormire(c->optiones.datum, mora);
    }
}

/* ===== vocationes nominatae ===== */
b32
horreum_ponere (
                    Horreum* h,
                     chorda  situla,
                     chorda  clavis,
                     chorda  corpus,
    constans HorreumPonendi* optiones,
            HorreumObjectum* ob,
               HorreumError* e,
                    Piscina* p)
{
    HorreumOpus o;

    redde horreum_opus_ponere(h, situla, clavis, corpus, optiones, &o,
        e,
        p)
        && horreum_exsequi(h, &o, NIHIL, ob, e, p);
}

b32
horreum_capere (
                     Horreum* h,
                      chorda  situla,
                      chorda  clavis,
    constans HorreumCapiendi* optiones,
                      chorda* corpus,
             HorreumObjectum* ob,
                HorreumError* e,
                     Piscina* p)
{
      HorreumOpus o;
    HorreumExitus x;

    si (corpus)
    {
        *corpus = chorda_ex_buffer(NIHIL, ZEPHYRUM);
    }
    si (   !horreum_opus_capere(h, situla, clavis, optiones, &o, e, p)
        || !horreum_exsequi(h, &o, &x, ob, e, p))
    {
        redde FALSUM;
    }
    si (corpus && x.responsum)
    {
        *corpus = x.responsum->corpus;
    }
    redde VERUM;
}

b32
horreum_inspicere (
            Horreum* h,
             chorda  situla,
             chorda  clavis,
    HorreumObjectum* ob,
       HorreumError* e,
            Piscina* p)
{
    HorreumOpus o;

    redde horreum_opus_inspicere(h, situla, clavis, &o, e, p)
        && horreum_exsequi(h, &o, NIHIL, ob, e, p);
}

b32
horreum_delere (
         Horreum* h,
          chorda  situla,
          chorda  clavis,
    HorreumError* e,
         Piscina* p)
{
    HorreumOpus o;

    redde horreum_opus_delere(h, situla, clavis, &o, e, p)
        && horreum_exsequi(h, &o, NIHIL, NIHIL, e, p);
}

b32
horreum_delere_plura (
            Horreum*  h,
             chorda   situla,
    constans chorda*  claves,
                i32   numerus,
     HorreumDeletio** exitus,
                i32*  exitus_numerus,
       HorreumError*  e,
            Piscina*  p)
{
      HorreumOpus o;
    HorreumExitus x;

    si (!exitus || !exitus_numerus)
    {
        redde FALSUM;
    }
    *exitus          = NIHIL;
    *exitus_numerus  = ZEPHYRUM;
    si (   !horreum_opus_delere_plura(h, situla, claves, numerus, &o, e,
        p)
        || !horreum_exsequi(h, &o, &x, NIHIL, e, p))
    {
        redde FALSUM;
    }
    redde horreum_deletiones_legere(x.responsum->corpus, exitus,
        exitus_numerus, e, p);
}

b32
horreum_copiare (
                    Horreum* h,
                     chorda  situla_fons,
                     chorda  clavis_fons,
                     chorda  situla,
                     chorda  clavis,
    constans HorreumPonendi* optiones,
            HorreumObjectum* ob,
               HorreumError* e,
                    Piscina* p)
{
    HorreumOpus o;

    redde horreum_opus_copiare(h, situla_fons, clavis_fons, situla,
        clavis,
        optiones, &o, e, p)
        && horreum_exsequi(h, &o, NIHIL, ob, e, p);
}

b32
horreum_enumerare (
                       Horreum* h,
                        chorda  situla,
    constans HorreumEnumerandi* optiones,
                 HorreumPagina* pagina,
                  HorreumError* e,
                       Piscina* p)
{
      HorreumOpus o;
    HorreumExitus x;

    si (!pagina)
    {
        redde FALSUM;
    }
    memset(pagina, ZEPHYRUM, magnitudo(*pagina));
    si (   !horreum_opus_enumerare(h, situla, optiones, &o, e, p)
        || !horreum_exsequi(h, &o, &x, NIHIL, e, p))
    {
        redde FALSUM;
    }
    redde horreum_paginam_legere(x.responsum->corpus, pagina, e, p);
}

/* ===== multipars ===== */
b32
horreum_mp_incipere (
                    Horreum* h,
                     chorda  situla,
                     chorda  clavis,
    constans HorreumPonendi* optiones,
                     chorda* id,
               HorreumError* e,
                    Piscina* p)
{
      HorreumOpus o;
    HorreumExitus x;

    si (!id)
    {
        redde FALSUM;
    }
    *id = chorda_ex_buffer(NIHIL, ZEPHYRUM);
    redde horreum_opus_mp_incipere(h, situla, clavis, optiones, &o, e,
        p)
        && horreum_exsequi(h, &o, &x, NIHIL, e, p)
        && horreum_id_oneris_legere(x.responsum->corpus, id, e, p);
}

b32
horreum_mp_partem_mittere (
         Horreum* h,
          chorda  situla,
          chorda  clavis,
          chorda  id,
             i32  numerus,
          chorda  corpus,
     HorreumPars* exitus,
    HorreumError* e,
         Piscina* p)
{
        HorreumOpus o;
    HorreumObjectum ob;

    si (!exitus)
    {
        redde FALSUM;
    }
    memset(exitus, ZEPHYRUM, magnitudo(*exitus));
    si (   !horreum_opus_mp_pars(h, situla, clavis, id, numerus, corpus,
        &o,
                e, p)
        || !horreum_exsequi(h, &o, NIHIL, &ob, e, p))
    {
        redde FALSUM;
    }
    exitus->numerus  = numerus;
    exitus->etag     = ob.etag;
    exitus->mensura  = (i64)corpus.mensura;
    redde VERUM;
}

b32
horreum_mp_perficere (
                 Horreum* h,
                  chorda  situla,
                  chorda  clavis,
                  chorda  id,
    constans HorreumPars* partes,
                     i32  numerus,
         HorreumObjectum* ob,
            HorreumError* e,
                 Piscina* p)
{
    HorreumOpus o;

    redde horreum_opus_mp_perficere(h, situla, clavis, id, partes,
        numerus,
        &o, e, p)
        && horreum_exsequi(h, &o, NIHIL, ob, e, p);
}

b32
horreum_mp_abicere (
         Horreum* h,
          chorda  situla,
          chorda  clavis,
          chorda  id,
    HorreumError* e,
         Piscina* p)
{
    HorreumOpus o;

    redde horreum_opus_mp_abicere(h, situla, clavis, id, &o, e, p)
        && horreum_exsequi(h, &o, NIHIL, NIHIL, e, p);
}

b32
horreum_mp_partes (
          Horreum*  h,
           chorda   situla,
           chorda   clavis,
           chorda   id,
      HorreumPars** exitus,
              i32*  numerus,
     HorreumError*  e,
          Piscina*  p)
{
      HorreumOpus o;
    HorreumExitus x;

    si (!exitus || !numerus)
    {
        redde FALSUM;
    }
    *exitus   = NIHIL;
    *numerus  = ZEPHYRUM;
    redde horreum_opus_mp_partes(h, situla, clavis, id, &o, e, p)
        && horreum_exsequi(h, &o, &x, NIHIL, e, p)
        && horreum_partes_legere(x.responsum->corpus, exitus, numerus,
        e,
            p);
}

b32
horreum_mp_pendentes (
                 Horreum*  h,
                  chorda   situla,
                  chorda   praefixum,
      HorreumOnusPendens** exitus,
                     i32*  numerus,
            HorreumError*  e,
                 Piscina*  p)
{
      HorreumOpus o;
    HorreumExitus x;

    si (!exitus || !numerus)
    {
        redde FALSUM;
    }
    *exitus   = NIHIL;
    *numerus  = ZEPHYRUM;
    redde horreum_opus_mp_pendentes(h, situla, praefixum, &o, e, p)
        && horreum_exsequi(h, &o, &x, NIHIL, e, p)
        && horreum_pendentes_legere(x.responsum->corpus, exitus,
        numerus, e,
            p);
}

/* ===== fila ===== */
/* errorem in piscinam durabilem (piscina partis mox destruitur) */
interior vacuum
_errorem_transcribere (
    HorreumError* e,
         Piscina* p)
{
    si (!e)
    {
        redde;
    }
    e->codex       = chorda_transcribere(e->codex, p);
    e->nuntius     = chorda_transcribere(e->nuntius, p);
    e->petitio_id  = chorda_transcribere(e->petitio_id, p);
}

interior b32
_malum (
          HorreumError* e,
    constans character* nuntius,
               Piscina* p)
{
    si (e)
    {
        memset(e, ZEPHYRUM, magnitudo(*e));
        e->genus    = HORREUM_MALUM;
        e->nuntius  = chorda_ex_literis(nuntius, p);
    }
    redde FALSUM;
}

/* n octetos ex lectore in capsam piscinae q; FALSUM si filum brevius */
interior b32
_legere_plene (
    FilumOctetorumLector* l,
                     i64  n,
                  chorda* corpus,
                 Piscina* q)
{
     i8* t      = (i8*)piscina_allocare(q, (memoriae_index)(n + I));
    i64  lecta  = ZEPHYRUM;

    dum (lecta < n)
    {
        i64 frustum = n - lecta < (i64)HORREUM_MIB ? n - lecta
                                                   : (i64)HORREUM_MIB;
        s32 r = filum_octeti_legere(l, t + lecta, (i32)frustum);

        si (r <= ZEPHYRUM)
        {
            redde FALSUM;
        }
        lecta += (i64)r;
    }
    *corpus = chorda_ex_buffer(t, (i32)n);
    redde VERUM;
}

/* onus relictum abicitur; error eius neglegitur (error primus manet) */
interior vacuum
_abicere_tacite (
    Horreum* h,
     chorda  situla,
     chorda  clavis,
     chorda  id,
    Piscina* p)
{
    HorreumError ignotum;

    (vacuum)horreum_mp_abicere(h, situla, clavis, id, &ignotum, p);
}

b32
horreum_ponere_ex_filo (
                         Horreum* h,
                          chorda  situla,
                          chorda  clavis,
             constans character* via,
         constans HorreumPonendi* optiones,
    constans HorreumFiliOptiones* fo,
                 HorreumObjectum* ob,
                    HorreumError* e,
                         Piscina* p)
{
             FilumStatus  st;
    FilumOctetorumLector* l;
                     i64  mensura;
                     i64  pars;
                     i64  numerus;
                     i64  k;
                  chorda  id;
             HorreumPars* partes;

    si (!via || !filum_status(via, &st) || !st.est_filum)
    {
        redde _malum(e, "horreum: filum non legitur (ponere_ex_filo)",
            p);
    }
    mensura  = (i64)st.mensura;
    pars     = horreum_partis_mensura(mensura, fo ? fo->pars_mensura
                                                  : ZEPHYRUM);
    si (pars > (i64)(DXII * HORREUM_MIB))
    {
        redde _malum(e, "horreum: pars maior quam DXII MiB", p);
    }
    l = filum_octeti_aperire(via, p);
    si (!l)
    {
        redde _malum(e, "horreum: filum non aperitur (ponere_ex_filo)",
            p);
    }
    si (mensura <= pars)
    {
        chorda corpus;
           b32 bene;

        si (!_legere_plene(l, mensura, &corpus, p))
        {
            filum_octeti_claudere(l);
            redde _malum(e, "horreum: lectio fili defecit", p);
        }
        filum_octeti_claudere(l);
        bene = horreum_ponere(h, situla, clavis, corpus, optiones, ob,
            e,
            p);
        si (bene && ob)
        {
            ob->mensura = mensura;
        }
        redde bene;
    }
    si (!horreum_mp_incipere(h, situla, clavis, optiones, &id, e, p))
    {
        filum_octeti_claudere(l);
        redde FALSUM;
    }
    numerus  = (mensura + pars - I) / pars;
    partes   = (HorreumPars*)piscina_allocare(p,
        (memoriae_index)(numerus
        * (i64)magnitudo(HorreumPars)));
    per (k = ZEPHYRUM; k < numerus; k++)
    {
             i64 m = mensura - k * pars < pars ? mensura
                 - k * pars : pars;
         Piscina* q = piscina_generare_dynamicum("horreum_pars",
                         (memoriae_index)(m + LXIV * MXXIV));
          chorda corpus;
             b32 lectum = _legere_plene(l, m, &corpus, q);
             b32 bene = lectum && horreum_mp_partem_mittere(h, situla,
                         clavis, id, (i32)(k + I), corpus, &partes[k],
                         e, q);

        si (bene)
        {
            partes[k].etag = chorda_transcribere(partes[k].etag, p);
        }
        alioquin si (lectum)
        {
            _errorem_transcribere(e, p);
        }
        piscina_destruere(q);
        si (!bene)
        {
            filum_octeti_claudere(l);
            _abicere_tacite(h, situla, clavis, id, p);
            redde lectum ? FALSUM
                         : _malum(e, "horreum: lectio fili defecit", p);
        }
    }
    filum_octeti_claudere(l);
    si (!horreum_mp_perficere(h, situla, clavis, id, partes,
        (i32)numerus,
            ob, e, p))
    {
        HorreumError primus = *e;

        _abicere_tacite(h, situla, clavis, id, p);
        *e = primus;
        redde FALSUM;
    }
    si (ob)
    {
        ob->clavis   = clavis;
        ob->mensura  = mensura;
    }
    redde VERUM;
}

/* ETag partis unius = XXXII hex (sine '-N') */
interior b32
_etag_md5 (
    chorda etag)
{
    i32 i;

    si (etag.mensura != XXXII)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < XXXII; i++)
    {
        character c = (character)etag.datum[i];

        si (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f')
              || (c >= 'A' && c <= 'F')))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior chorda
_hex_md5 (
    constans i8* d,
        Piscina* p)
{
    hic_manens constans character cifrae[] = "0123456789abcdef";
                        character t[XXXIII];
                              i32 i;

    per (i = ZEPHYRUM; i < MD5_DIGEST_MENSURA; i++)
    {
        t[i * II]      = cifrae[((i32)d[i] >> IV) & 0xF];
        t[i * II + I]  = cifrae[(i32)d[i] & 0xF];
    }
    t[XXXII] = '\0';
    redde chorda_ex_literis(t, p);
}

/* "<via>.horreum-<pid>-<signum>": idem directorium, ergo filum_movere
 * renominatio est */
interior constans character*
_via_temporaria (
               Horreum* h,
    constans character* via,
               Piscina* p)
{
    HorreumCliens* c = (HorreumCliens*)horreum_sedes(h);
              i32  signum = c ? sors_proximum(&c->sors)
                              : (i32)time(NIHIL);
        character* t = (character*)piscina_allocare(p,
                           (memoriae_index)(strlen(via) + LXIV));

    sprintf(t, "%s.horreum-%ld-%08x", via, (longus)getpid(),
        (insignatus integer)signum);
    redde t;
}

b32
horreum_capere_in_filum (
                Horreum* h,
                 chorda  situla,
                 chorda  clavis,
    constans character* via,
        HorreumObjectum* ob,
           HorreumError* e,
                Piscina* p)
{
       HorreumObjectum  caput;
    constans character* temp;
         FilumScriptor* sc;
          MD5Contextus  ctx;
                    i8  digestum[MD5_DIGEST_MENSURA];
                   i64  ab;
                   i64  scripta  = ZEPHYRUM;
                   b32  bene     = VERUM;

    si (!via)
    {
        redde _malum(e, "horreum: via destinationis NIHIL", p);
    }
    si (!horreum_inspicere(h, situla, clavis, &caput, e, p))
    {
        redde FALSUM;
    }
    temp  = _via_temporaria(h, via, p);
    sc    = filum_scriptor_aperire(temp, FILUM_MODUS_CREARE, p);
    si (!sc)
    {
        redde _malum(e, "horreum: filum temporarium non creatur", p);
    }
    md5_initiare(&ctx);
    per (ab = ZEPHYRUM; bene && ab < caput.mensura;
         ab += (i64)HORREUM_FRUSTUM_LECTIONIS)
    {
        HorreumCapiendi  ca;
                 chorda  corpus;
                Piscina* q =
                    piscina_generare_dynamicum("horreum_frustum",
                             (memoriae_index)(HORREUM_FRUSTUM_LECTIONIS
                                 + LXIV * MXXIV));

        memset(&ca, ZEPHYRUM, magnitudo(ca));
        ca.frustum  = VERUM;
        ca.ab       = ab;
        ca.ad       = (ab + (i64)HORREUM_FRUSTUM_LECTIONIS
            < caput.mensura
                          ? ab + (i64)HORREUM_FRUSTUM_LECTIONIS
                          : caput.mensura) - I;
        ca.si_etag  = caput.etag;
        bene        = horreum_capere(h, situla, clavis, &ca, &corpus,
            NIHIL,
            e, q);
        si (!bene)
        {
            _errorem_transcribere(e, p);
        }
        alioquin si (!filum_scriptor_octetos_scribere(sc, corpus.datum,
                         corpus.mensura))
        {
            bene = _malum(e,
                "horreum: scriptio fili temporarii defecit",
                p);
        }
        alioquin
        {
            md5_addere(&ctx, corpus.datum, corpus.mensura);
            scripta += (i64)corpus.mensura;
        }
        piscina_destruere(q);
    }
    filum_scriptor_claudere(sc);
    si (bene && scripta != caput.mensura)
    {
        bene = _malum(e,
            "horreum: mensura recepta non congruit (capere_in_filum)",
            p);
    }
    si (bene && _etag_md5(caput.etag))
    {
        md5_finire(&ctx, digestum);
        si (!chorda_aequalis_case_insensitivus(_hex_md5(digestum, p),
                caput.etag))
        {
            bene = _malum(e,
                "horreum: md5 recepti non congruit ETag (capere_in_"
                "filum)",
                p);
        }
    }
    si (bene && !filum_movere(temp, via))
    {
        bene = _malum(e, "horreum: filum temporarium non movetur", p);
    }
    si (!bene)
    {
        (vacuum)filum_delere(temp);
        redde FALSUM;
    }
    si (ob)
    {
        *ob = caput;
    }
    redde VERUM;
}

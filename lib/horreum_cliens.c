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
 * iterare; ceterum finis. Multipars (NoSuchUpload post iterationem):
 * T4. */
#include "postulata_posix.h"
#include "latina.h"
#include "horreum_cliens.h"
#include "sors.h"

#include <string.h>
#include <time.h>

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

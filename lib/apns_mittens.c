/* apns_mittens.c - mittens APNs non obstruens super reactorem. Vide
 * include/apns_mittens.h. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "json.h"
#include "reactor.h"
#include "hpack.h"
#include "http2_nexus.h"
#include "tessera_es256.h"
#include "apns.h"
#include "pulsus_missio.h"
#include "apns_mittens.h"
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#define AMBITUS_PRODUCTIO     ZEPHYRUM
#define AMBITUS_SANDBOX       I
#define AMBITUS_NUMERUS       II
#define MORA_CONNEXIONIS_MS   (XV * M)
#define SIGNUM_MAXIMUM        CCLVI
#define AUCTORITAS_MAXIMA     DXII
#define DIES_S                ((s64)LXXXVI * M + CD)
#define PISCINA_MISSIONIS     (VIII * MXXIV)

nomen structura Missio Missio;

/* connexio una per ambitum; generatio nexus novi quemque nuntium
 * status veterem ignorare sinit */
nomen structura {
      Http2Nexus* nexus;
         Piscina* piscina;
             b32  mortuus;
             i32  generatio;
       character  hospes[CCLVI];
} Connexio;

nomen structura {
    ApnsMittens* mittens;
            i32  index;
            i32  generatio;
} NexusContextus;

structura Missio {
    ApnsMittens* mittens;
        Piscina* piscina;
    PulsusMissio copia;
     PulsusFinis finis;
          vacuum* ctx;
             i32  index;          /* ambitus */
       character  via[CCLXXX];
       character* auctoritas;    /* tampon AUCTORITAS_MAXIMA, zephyratur */
             i32  iterationes;
             b32  renovata;
             i32  generatio;      /* tesserae in petitione ultima */
   ReactorTimerId hora;
             b32  hora_viva;
         Missio*  prior;
         Missio*  proxima;
};

structura ApnsMittens {
         Reactor* reactor;
              s64 (*nunc_ms)(vacuum);
              i8* clavis;
             i32  clavis_mensura;
          chorda  clavis_id;
          chorda  turma;
             i32  portus;
       character* ancora;
        Connexio  connexiones[AMBITUS_NUMERUS];
       character  tessera[AUCTORITAS_MAXIMA];
             i32  tessera_mensura;
             s64  tessera_ms;
             i32  generatio;
             i32  morae[APNS_MORAE_MAXIMAE];
             i32  morae_numerus;
     ApnsVectura  vectura;
          vacuum* vectura_sui;
          Missio* primae;
             i32  pendentes;
             b32  claudens;
};


/* ==================================================
 * Tempus et tessera provisoris
 * ================================================== */

interior s64
_nunc (
    constans ApnsMittens* m)
{
    structura timeval tv;

    si (m->nunc_ms)
    {
        redde m->nunc_ms();
    }
    gettimeofday(&tv, NIHIL);
    redde (s64)tv.tv_sec * M + (s64)tv.tv_usec / M;
}

/* tessera nova in tamponem proprium; piscina temporaria zephyrata et
 * reddita */
interior b32
_tesseram_signare (
    ApnsMittens* m)
{
    Piscina* t;
     chorda  tessera;
        s64  nunc;
        b32  bene;

    t = piscina_generare_dynamicum("apns_tessera", IV * MXXIV);
    si (!t)
    {
        redde FALSUM;
    }
    nunc     = _nunc(m);
    tessera  = tessera_apns_facere(m->clavis, m->clavis_mensura,
        m->clavis_id, m->turma, nunc / M, t);
    bene     = tessera.mensura > ZEPHYRUM
        && tessera.mensura < AUCTORITAS_MAXIMA - VIII;
    si (bene)
    {
        memset(m->tessera, ZEPHYRUM, magnitudo(m->tessera));
        memcpy(m->tessera, tessera.datum, (size_t)tessera.mensura);
        m->tessera_mensura  = tessera.mensura;
        m->tessera_ms       = nunc;
        m->generatio++;
    }
    si (tessera.mensura > ZEPHYRUM)
    {
        memset(tessera.datum, ZEPHYRUM, (size_t)tessera.mensura);
    }
    piscina_destruere(t);
    redde bene;
}


/* ==================================================
 * Connexiones
 * ================================================== */

interior vacuum
_status (
    vacuum* ctx,
       b32  vivus,
    chorda  causa)
{
    NexusContextus* k = (NexusContextus*)ctx;
          Connexio* c;

    c = &k->mittens->connexiones[k->index];
    si (vivus || k->generatio != c->generatio)
    {
        redde;
    }
    c->mortuus = VERUM;
    si (!k->mittens->claudens)
    {
        fprintf(stderr, "apns: connexio %s mortua (%.*s)\n", c->hospes,
            (int)causa.mensura, (constans character*)causa.datum);
    }
}

interior vacuum
_connexionem_dimittere (
    Connexio* c)
{
    Http2Nexus* x;
       Piscina* p;

    x           = c->nexus;
    p           = c->piscina;
    c->nexus    = NIHIL;
    c->piscina  = NIHIL;
    c->mortuus  = FALSUM;
    c->generatio++;
    si (x)
    {
        http2_nexum_claudere(x);
    }
    si (p)
    {
        piscina_destruere(p);
    }
}

/* nexus ambitus: pigre apertus, post mortem renatus */
interior Http2Nexus*
_nexus (
    ApnsMittens* m,
            i32  index)
{
              Connexio* c = &m->connexiones[index];
        NexusContextus* k;
    Http2NexusOptiones  opt;

    si (c->nexus && c->mortuus)
    {
        _connexionem_dimittere(c);
    }
    si (c->nexus)
    {
        redde c->nexus;
    }
    c->piscina = piscina_generare_dynamicum("apns_nexus", XVI * MXXIV);
    si (!c->piscina)
    {
        redde NIHIL;
    }
    k = (NexusContextus*)piscina_allocare(c->piscina,
        magnitudo(NexusContextus));
    si (!k)
    {
        _connexionem_dimittere(c);
        redde NIHIL;
    }
    k->mittens    = m;
    k->index      = index;
    k->generatio  = c->generatio;
    memset(&opt, ZEPHYRUM, magnitudo(opt));
    opt.mora_connexionis_ms  = MORA_CONNEXIONIS_MS;
    opt.mora_otii_ms         = MORA_CONNEXIONIS_MS;
    opt.tls_ancora_via       = m->ancora;
    c->nexus = http2_nexum_aperire(m->reactor, c->hospes, m->portus,
        &opt,
        _status, k, c->piscina);
    si (!c->nexus)
    {
        _connexionem_dimittere(c);
    }
    redde c->nexus;
}


/* ==================================================
 * Missiones
 * ================================================== */

interior vacuum
_finire (
          Missio* x,
    PulsusExitus  exitus,
          chorda  ratio)
{
    ApnsMittens* m = x->mittens;

    si (x->prior)
    {
        x->prior->proxima = x->proxima;
    }
    alioquin
    {
        m->primae = x->proxima;
    }
    si (x->proxima)
    {
        x->proxima->prior = x->prior;
    }
    m->pendentes--;
    si (x->hora_viva)
    {
        (vacuum)reactor_timer_cancellare(m->reactor, x->hora);
        x->hora_viva = FALSUM;
    }
    x->finis(x->ctx, &x->copia, exitus, ratio);
    memset(x->auctoritas, ZEPHYRUM, AUCTORITAS_MAXIMA);
    piscina_destruere(x->piscina);
}

interior vacuum
_petere (
    Missio* x);

interior vacuum
_ictus (
    vacuum* data)
{
    Missio* x = (Missio*)data;

    x->hora_viva = FALSUM;
    _petere(x);
}

/* iteratio post moram proximam, aut DEFECTUS si nulla manet */
interior vacuum
_iterare (
      Missio* x,
      chorda  ratio)
{
    ApnsMittens* m = x->mittens;

    si (m->claudens || x->iterationes >= m->morae_numerus)
    {
        _finire(x, PULSUS_DEFECTUS, ratio);
        redde;
    }
    x->hora = reactor_timer_adicere(m->reactor,
        m->morae[x->iterationes],
        FALSUM, _ictus, x);
    x->iterationes++;
    si (x->hora == REACTOR_TIMER_ERROR)
    {
        _finire(x, PULSUS_DEFECTUS, chorda_ex_literis(
            "apns: hora iterationis poni nequit", x->piscina));
        redde;
    }
    x->hora_viva = VERUM;
}

interior chorda
_rationem_legere (
                     Missio* x,
    constans Http2Responsum* r)
{
    JsonResultus j;
          chorda vacua;
       character status[XXXII];

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    si (!r->successus)
    {
        redde chorda_concatenare(chorda_ex_literis("apns: transportus - ",
            x->piscina), r->causa.mensura > ZEPHYRUM ? r->causa
            : chorda_ex_literis("fractus", x->piscina), x->piscina);
    }
    si (r->corpus.mensura > ZEPHYRUM)
    {
        j = json_legere(r->corpus, x->piscina);
        si (j.successus)
        {
            vacua = json_capere_chorda(j.radix, "reason", vacua);
        }
    }
    si (vacua.mensura > ZEPHYRUM)
    {
        redde vacua;
    }
    sprintf(status, "status %u", (insignatus integer)r->status);
    redde chorda_ex_literis(status, x->piscina);
}

interior vacuum
_responsum (
                     vacuum* ctx,
    constans Http2Responsum* r)
{
          Missio* x = (Missio*)ctx;
     ApnsMittens* m = x->mittens;
          chorda  ratio;

    ratio = _rationem_legere(x, r);
    si (m->claudens)
    {
        _finire(x, PULSUS_DEFECTUS, ratio);
        redde;
    }
    si (r->successus && r->status == CC)
    {
        _finire(x, PULSUS_TRADITUM, ratio);
        redde;
    }
    si (   r->successus
        && (r->status == CDX || (r->status == CD
            && chorda_aequalis_literis(ratio, "BadDeviceToken"))))
    {
        _finire(x, PULSUS_SIGNUM_MORTUUM, ratio);
        redde;
    }
    si (   r->successus && r->status == CDIII && !x->renovata
        && (   chorda_aequalis_literis(ratio, "ExpiredProviderToken")
            || chorda_aequalis_literis(ratio, "InvalidProviderToken")))
    {
        /* tessera semel renovatur - nisi alia missio iam renovavit */
        x->renovata = VERUM;
        si (x->generatio == m->generatio)
        {
            (vacuum)_tesseram_signare(m);
        }
        x->hora = reactor_timer_adicere(m->reactor, ZEPHYRUM, FALSUM,
            _ictus, x);
        si (x->hora == REACTOR_TIMER_ERROR)
        {
            _finire(x, PULSUS_DEFECTUS, ratio);
            redde;
        }
        x->hora_viva = VERUM;
        redde;
    }
    si (   !r->successus || r->retentabilis || r->status == CDXXIX
        || (r->status >= D && r->status < DC))
    {
        _iterare(x, ratio);
        redde;
    }
    _finire(x, PULSUS_DEFECTUS, ratio);
}

interior HpackCaput
_caput (
    constans character* titulus,
                chorda  valor,
                   b32  numquam,
               Piscina* p)
{
    HpackCaput h;

    h.titulus  = chorda_ex_literis(titulus, p);
    h.valor    = valor;
    h.numquam  = numquam;
    redde h;
}

interior vacuum
_petere (
    Missio* x)
{
     ApnsMittens* m = x->mittens;
      Http2Nexus* nexus;
      HpackCaput  h[V];
       character  terminus[XXXII];
          chorda  auctoritas;
             s64  nunc;
             b32  bene;

    nunc = _nunc(m);
    si (nunc - m->tessera_ms >= APNS_TESSERA_AETAS_MS)
    {
        (vacuum)_tesseram_signare(m);
    }
    sprintf(terminus, "%ld", (long)(nunc / M + DIES_S));
    memset(x->auctoritas, ZEPHYRUM, AUCTORITAS_MAXIMA);
    memcpy(x->auctoritas, "bearer ", VII);
    memcpy(x->auctoritas + VII, m->tessera, (size_t)m->tessera_mensura);
    auctoritas.datum    = (i8*)x->auctoritas;
    auctoritas.mensura  = VII + m->tessera_mensura;
    x->generatio        = m->generatio;
    h[ZEPHYRUM] = _caput("apns-push-type", chorda_ex_literis("alert",
        x->piscina), FALSUM, x->piscina);
    h[I]    = _caput("apns-topic", x->copia.thema, FALSUM, x->piscina);
    h[II]   = _caput("apns-priority", chorda_ex_literis("10",
        x->piscina), FALSUM, x->piscina);
    h[III]  = _caput("apns-expiration", chorda_ex_literis(terminus,
        x->piscina), FALSUM, x->piscina);
    h[IV]   = _caput("authorization", auctoritas, VERUM, x->piscina);
    si (m->vectura)
    {
        bene = m->vectura(m->vectura_sui, x->copia.ambitus, x->via, h,
            V,
            x->copia.corpus, _responsum, x, x->piscina);
    }
    alioquin
    {
        nexus  = _nexus(m, x->index);
        bene   = nexus
            && http2_nexus_petere(nexus, "POST", x->via, h, V,
            x->copia.corpus, _responsum, x, x->piscina);
    }
    si (!bene)
    {
        _iterare(x, chorda_ex_literis("apns: petitio non accepta",
            x->piscina));
    }
}

interior b32
_signum_bonum (
    chorda s)
{
    i32 i;

    si (   s.mensura < II || s.mensura > SIGNUM_MAXIMUM
        || (s.mensura % II) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        character c = (character)s.datum[i];

        si (!(   (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f')
              || (c >= 'A' && c <= 'F')))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

b32
apns_mittens_mittere (
                  vacuum* sui,
    constans PulsusMissio* missio,
             PulsusFinis  finis,
                  vacuum* ctx)
{
     ApnsMittens* m = (ApnsMittens*)sui;
          Missio* x;
         Piscina* p;
             i32  index;

    si (!m || !missio || !finis || m->claudens)
    {
        redde FALSUM;
    }
    si (chorda_aequalis_literis(missio->ambitus, "productio"))
    {
        index = AMBITUS_PRODUCTIO;
    }
    alioquin si (chorda_aequalis_literis(missio->ambitus, "sandbox"))
    {
        index = AMBITUS_SANDBOX;
    }
    alioquin
    {
        redde FALSUM;
    }
    si (   !_signum_bonum(missio->signum)
        || missio->thema.mensura  == ZEPHYRUM
        || missio->corpus.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (m->connexiones[index].hospes[ZEPHYRUM] == '\0')
    {
        fprintf(stderr,
            "apns: ambitus sandbox sine hospes_sandbox - non "
            "mittitur (%.8s)\n",
            (constans character*)missio->signum.datum);
        redde FALSUM;
    }
    p = piscina_generare_dynamicum("apns_missio", PISCINA_MISSIONIS);
    si (!p)
    {
        redde FALSUM;
    }
    x = (Missio*)piscina_allocare(p, magnitudo(Missio));
    si (!x)
    {
        piscina_destruere(p);
        redde FALSUM;
    }
    memset(x, ZEPHYRUM, magnitudo(Missio));
    x->mittens          = m;
    x->piscina          = p;
    x->finis            = finis;
    x->ctx              = ctx;
    x->index            = index;
    x->copia.signum     = chorda_transcribere(missio->signum, p);
    x->copia.ambitus    = chorda_transcribere(missio->ambitus, p);
    x->copia.thema      = chorda_transcribere(missio->thema, p);
    x->copia.corpus     = chorda_transcribere(missio->corpus, p);
    x->copia.apparatus  = chorda_transcribere(missio->apparatus, p);
    x->auctoritas         = (character*)piscina_allocare(p,
        AUCTORITAS_MAXIMA);
    si (!x->auctoritas || !x->copia.corpus.datum)
    {
        piscina_destruere(p);
        redde FALSUM;
    }
    sprintf(x->via, "/3/device/%.*s", (int)x->copia.signum.mensura,
        (constans character*)x->copia.signum.datum);
    x->proxima = m->primae;
    si (m->primae)
    {
        m->primae->prior = x;
    }
    m->primae = x;
    m->pendentes++;
    _petere(x);
    redde VERUM;
}


/* ==================================================
 * Vita
 * ================================================== */

interior b32
_hospitem_copiare (
     character* d,
        chorda  s)
{
    si (s.mensura >= CCLVI)
    {
        redde FALSUM;
    }
    si (s.mensura > ZEPHYRUM)
    {
        memcpy(d, s.datum, (size_t)s.mensura);
    }
    d[s.mensura] = '\0';
    redde VERUM;
}

ApnsMittens*
apns_mittentem_aperire (
                    Reactor* reactor,
    constans ApnsConfiguratio* configuratio,
                constans i8* clavis_der,
                        i32  mensura,
                        s64  (*nunc_ms)(vacuum),
                    Piscina* piscina)
{
    ApnsMittens* m;

    si (   !reactor || !configuratio || !clavis_der
        || mensura == ZEPHYRUM
        || !piscina || configuratio->hospes.mensura == ZEPHYRUM)
    {
        redde NIHIL;
    }
    m = (ApnsMittens*)piscina_allocare(piscina, magnitudo(ApnsMittens));
    si (!m)
    {
        redde NIHIL;
    }
    memset(m, ZEPHYRUM, magnitudo(ApnsMittens));
    m->reactor         = reactor;
    m->nunc_ms         = nunc_ms;
    m->clavis_mensura  = mensura;
    m->clavis          = (i8*)piscina_allocare(piscina,
        (memoriae_index)mensura);
    m->clavis_id       = chorda_transcribere(configuratio->clavis_id,
        piscina);
    m->turma           = chorda_transcribere(configuratio->turma,
        piscina);
    m->portus          = configuratio->portus != ZEPHYRUM
        ? configuratio->portus : CDXLIII;
    m->ancora          = configuratio->ancora.mensura > ZEPHYRUM
        ? chorda_ut_cstr(configuratio->ancora, piscina) : NIHIL;
    m->morae[ZEPHYRUM]  = M;
    m->morae[I]         = V * M;
    m->morae[II]        = XXX * M;
    m->morae_numerus    = III;
    si (   !m->clavis
        || !_hospitem_copiare(m->connexiones[AMBITUS_PRODUCTIO].hospes,
            configuratio->hospes)
        || !_hospitem_copiare(m->connexiones[AMBITUS_SANDBOX].hospes,
            configuratio->hospes_sandbox))
    {
        redde NIHIL;
    }
    memcpy(m->clavis, clavis_der, (size_t)mensura);
    si (!_tesseram_signare(m))
    {
        memset(m->clavis, ZEPHYRUM, (size_t)mensura);
        redde NIHIL;
    }
    redde m;
}

vacuum
apns_mittentis_moras_ponere (
      ApnsMittens* mittens,
     constans i32* moras_ms,
              i32  n)
{
    i32 i;

    si (   !mittens || n > APNS_MORAE_MAXIMAE
        || (n > ZEPHYRUM && !moras_ms))
    {
        redde;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        mittens->morae[i] = moras_ms[i];
    }
    mittens->morae_numerus = n;
}

vacuum
apns_mittentis_vecturam_ponere (
    ApnsMittens* mittens,
    ApnsVectura  petere,
         vacuum* sui)
{
    si (mittens)
    {
        mittens->vectura      = petere;
        mittens->vectura_sui  = sui;
    }
}

i32
apns_mittentis_pendentes (
    constans ApnsMittens* mittens)
{
    redde mittens ? mittens->pendentes : ZEPHYRUM;
}

vacuum
apns_mittentem_claudere (
    ApnsMittens* mittens)
{
    character textus[] = "apns: mittens clauditur";
       chorda causa;
          i32 i;

    si (!mittens)
    {
        redde;
    }
    mittens->claudens = VERUM;
    per (i = ZEPHYRUM; i < AMBITUS_NUMERUS; i++)
    {
        _connexionem_dimittere(&mittens->connexiones[i]);
    }
    causa.datum    = (i8*)textus;
    causa.mensura  = (i32)strlen(textus);
    dum (mittens->primae)
    {
        _finire(mittens->primae, PULSUS_DEFECTUS, causa);
    }
    memset(mittens->clavis, ZEPHYRUM, (size_t)mittens->clavis_mensura);
    memset(mittens->tessera, ZEPHYRUM, magnitudo(mittens->tessera));
    mittens->tessera_mensura = ZEPHYRUM;
}

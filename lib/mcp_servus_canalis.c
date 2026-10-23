/* mcp_servus_canalis.c - MCP super canalem localem (canalis-spec II.2)
 *
 * Connexio: alvei lectionis et scriptionis in piscina connexionis
 * (crescunt duplicando, compactantur - summa lectionis ~ II MiB); omnis
 * linea piscinam petitionis suam habet (responso dilato vivit).
 *
 * FINIS CONNEXIONIS: EOF lectionis = dimidia clausura (pons SHUT_WR
 * facit et responsa exspectat): lectio cessat, quae debentur (cauda
 * scriptionis, responsa dilata) traduntur, deinde clauditur. Si nihil
 * legendum aut scribendum manet sed responsum dilatum pendet, fossa e
 * reactore removetur - POLLHUP perpetuum aliter cyclum vacuum
 * volveret; responsum serum directe scribitur (EPIPE -> claudere).
 * REACTOR_ERROR -> statim clauditur. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "json.h"
#include "reactor.h"
#include "tcp.h"
#include "canalis.h"
#include "mcp_servus.h"
#include "mcp_servus_canalis.h"

#include <errno.h>
#include <poll.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#define ALVEUS_INITIUM      (IV * MXXIV)
#define FRUSTUM_LECTIONIS   (XVI * MXXIV)
#define PISCINA_PETITIONIS  (LXIV * MXXIV)
#define PISCINA_CONNEXIONIS (XVI * MXXIV)

nomen structura {
     i8* datum;
    i32  longitudo;
    i32  capacitas;
} Alveus;

nomen structura McpCanalisConnexio McpCanalisConnexio;

structura McpCanalisConnexio {
       McpCanalis* canalis;
      TcpConnexio* tcp;
          integer  fd;
              i32  locus;
              i32  generatio;
              b32  vivus;
              b32  in_reactore;
              b32  in_lectione;
              b32  claudenda;
              b32  finis_lectionis;
              i32  pendentes;       /* responsa dilata nondum reddita */
          Piscina* piscina;
           Alveus  lectio;
           Alveus  scriptio;
              i32  scripta;         /* scriptionis iam missum */
};

structura McpCanalis {
                McpServus* servus;
                  Reactor* reactor;
           CanalisAuditor* auditor;
                  Piscina* piscina;
               McpPersona  persona;
                      b32  clausus;
       McpCanalisConnexio  connexiones[MCP_CANALIS_CONNEXIONES];
};

/* petitio una: piscina sua; dilata -> _reddere eam destruit */
nomen structura {
    McpCanalis* canalis;
           i32  locus;
           i32  generatio;
       Piscina* piscina;
           b32  dilata;
} McpCanalisPetitio;


/* ==================================================
 * Alvei
 * ================================================== */

interior b32
_alveus_addere (
          Alveus* a,
         Piscina* p,
    constans i8* d,
            i32  n)
{
    si (a->longitudo + n > a->capacitas)
    {
        i32 nova = a->capacitas
            > ZEPHYRUM ? a->capacitas : ALVEUS_INITIUM;
         i8* novum;

        dum (nova < a->longitudo + n)
        {
            nova *= II;
        }
        novum = (i8*)piscina_allocare(p, (memoriae_index)nova);
        si (!novum)
        {
            redde FALSUM;
        }
        si (a->longitudo > ZEPHYRUM)
        {
            memcpy(novum, a->datum, (memoriae_index)a->longitudo);
        }
        a->datum      = novum;
        a->capacitas  = nova;
    }
    memcpy(a->datum + a->longitudo, d, (memoriae_index)n);
    a->longitudo += n;
    redde VERUM;
}

/* primos 'n' octetos removere (compactio) */
interior vacuum
_alveus_consumere (
    Alveus* a,
       i32  n)
{
    si (n >= a->longitudo)
    {
        a->longitudo = ZEPHYRUM;
        redde;
    }
    memmove(a->datum, a->datum + n, (memoriae_index)(a->longitudo - n));
    a->longitudo -= n;
}


/* ==================================================
 * Connexiones (servus)
 * ================================================== */

interior vacuum
_connexio_cb (
    integer  fd,
        i32  eventus,
     vacuum* data);

interior vacuum
_connexionem_claudere (
    McpCanalisConnexio* k)
{
    si (!k->vivus)
    {
        redde;
    }
    si (k->in_reactore)
    {
        (vacuum)reactor_removere(k->canalis->reactor, k->fd);
    }
    tcp_claudere(k->tcp);
    piscina_destruere(k->piscina);
    k->tcp              = NIHIL;
    k->piscina          = NIHIL;
    k->fd               = -I;
    k->vivus            = FALSUM;
    k->in_reactore      = FALSUM;
    k->claudenda        = FALSUM;
    k->finis_lectionis  = FALSUM;
    k->pendentes        = ZEPHYRUM;
    k->generatio++;
    memset(&k->lectio, ZEPHYRUM, magnitudo(k->lectio));
    memset(&k->scriptio, ZEPHYRUM, magnitudo(k->scriptio));
    k->scripta           = ZEPHYRUM;
}

/* caudam scriptionis mittere quantum socket accipit */
interior vacuum
_scriptio_urgere (
    McpCanalisConnexio* k)
{
    dum (k->scripta < k->scriptio.longitudo)
    {
        s32 n = tcp_mittere(k->tcp, k->scriptio.datum + k->scripta,
                    k->scriptio.longitudo - k->scripta);

        si (n < ZEPHYRUM)
        {
            k->claudenda = VERUM;
            redde;
        }
        si (n == ZEPHYRUM)
        {
            frange;
        }
        k->scripta += (i32)n;
    }
    _alveus_consumere(&k->scriptio, k->scripta);
    k->scripta = ZEPHYRUM;
}

/* interest in reactore secundum statum (canalis P2) */
interior vacuum
_interesse (
    McpCanalisConnexio* k)
{
    i32 eventus = ZEPHYRUM;

    si (!k->finis_lectionis)
    {
        eventus |= (i32)REACTOR_LEGERE;
    }
    si (k->scriptio.longitudo > ZEPHYRUM)
    {
        eventus |= (i32)REACTOR_SCRIBERE;
    }
    si (eventus == ZEPHYRUM)
    {
        si (k->in_reactore)
        {
            (vacuum)reactor_removere(k->canalis->reactor, k->fd);
            k->in_reactore = FALSUM;
        }
        redde;
    }
    si (k->in_reactore)
    {
        (vacuum)reactor_modificare(k->canalis->reactor, k->fd, eventus);
    }
    alioquin si (reactor_adicere(k->canalis->reactor, k->fd, eventus,
                 _connexio_cb, k))
    {
        k->in_reactore = VERUM;
    }
    alioquin
    {
        k->claudenda = VERUM;
    }
}

interior vacuum
_fortasse_claudere (
    McpCanalisConnexio* k)
{
    si (   k->claudenda
        || (   k->finis_lectionis
            && k->pendentes == ZEPHYRUM
            && k->scriptio.longitudo == ZEPHYRUM))
    {
        _connexionem_claudere(k);
    }
}

interior vacuum
_reddere (
                 vacuum* datum,
    constans McpReditus* r)
{
     McpCanalisPetitio* q   = (McpCanalisPetitio*)datum;
            McpCanalis* mc  = q->canalis;
    McpCanalisConnexio* k   = &mc->connexiones[q->locus];

    si (!mc->clausus && k->vivus && k->generatio == q->generatio)
    {
        si (q->dilata && k->pendentes > ZEPHYRUM)
        {
            k->pendentes--;
        }
        si (r->corpus.mensura > ZEPHYRUM)
        {
            si (   !_alveus_addere(&k->scriptio, k->piscina,
                       r->corpus.datum, r->corpus.mensura)
                || !_alveus_addere(&k->scriptio, k->piscina,
                       (constans i8*)"\n", I))
            {
                k->claudenda = VERUM;
            }
            alioquin
            {
                _scriptio_urgere(k);
            }
        }
        si (!k->in_lectione)
        {
            si (!k->claudenda)
            {
                _interesse(k);
            }
            _fortasse_claudere(k);
        }
    }
    si (q->dilata)
    {
        piscina_destruere(q->piscina);
    }
}

interior vacuum
_lineam_tractare (
    McpCanalisConnexio* k,
          constans i8* d,
                  i32  n)
{
            McpCanalis* mc = k->canalis;
               Piscina* p;
     McpCanalisPetitio* q;
                chorda  linea;

    p = piscina_generare_dynamicum("mcp_canalis_petitio",
        (memoriae_index)PISCINA_PETITIONIS);
    si (!p)
    {
        k->claudenda = VERUM;
        redde;
    }
    q           = (McpCanalisPetitio*)piscina_allocare(p,
                      (memoriae_index)magnitudo(McpCanalisPetitio));
    linea.datum = (i8*)piscina_allocare(p, (memoriae_index)n);
    si (!q || !linea.datum)
    {
        piscina_destruere(p);
        k->claudenda = VERUM;
        redde;
    }
    memcpy(linea.datum, d, (memoriae_index)n);
    linea.mensura  = n;
    q->canalis     = mc;
    q->locus       = k->locus;
    q->generatio   = k->generatio;
    q->piscina     = p;
    q->dilata      = FALSUM;
    si (mcp_servus_tractare(mc->servus, linea, &mc->persona, VERUM,
            _reddere, q, p))
    {
        piscina_destruere(p);
    }
    alioquin
    {
        q->dilata = VERUM;
        k->pendentes++;
    }
}

/* lineas integras tractare; residuum (sine '\n') manet */
interior vacuum
_lineas_tractare (
    McpCanalisConnexio* k)
{
    i32 initium = ZEPHYRUM;
    i32 j;

    per (j = ZEPHYRUM; j < k->lectio.longitudo && !k->claudenda; j++)
    {
        si (k->lectio.datum[j] == (i8)'\n')
        {
            si (j > initium)
            {
                _lineam_tractare(k, k->lectio.datum + initium,
                    j - initium);
            }
            initium = j + I;
        }
    }
    _alveus_consumere(&k->lectio, initium);
    si (k->lectio.longitudo > MCP_CANALIS_LINEA_MAXIMA)
    {
        k->claudenda = VERUM;
    }
}

interior vacuum
_legere (
    McpCanalisConnexio* k)
{
    i8 frustum[FRUSTUM_LECTIONIS];

    dum (!k->claudenda && !k->finis_lectionis)
    {
        /* recv crudum, NON tcp_recipere: illud ad EOF connexionem
         * 'clausam' signat et tcp_mittere deinde nihil mittit -
         * responsa post dimidiam clausuram debita perirent (inventum
         * recensionis canalis; probatio pons_dilatum, pons_magnum) */
        ssize_t n = recv(k->fd, frustum, magnitudo(frustum), ZEPHYRUM);

        si (n < ZEPHYRUM)
        {
            si (errno == EINTR)
            {
                perge;
            }
            si (errno != EAGAIN && errno != EWOULDBLOCK)
            {
                k->claudenda = VERUM;
            }
            frange;
        }
        si (n == ZEPHYRUM)
        {
            /* dimidia clausura: residuum sine '\n' omittitur */
            k->finis_lectionis = VERUM;
            frange;
        }
        si (!_alveus_addere(&k->lectio, k->piscina, frustum, (i32)n))
        {
            k->claudenda = VERUM;
            frange;
        }
        _lineas_tractare(k);
    }
}

interior vacuum
_connexio_cb (
    integer  fd,
        i32  eventus,
     vacuum* data)
{
    McpCanalisConnexio* k = (McpCanalisConnexio*)data;

    (vacuum)fd;
    si (!k->vivus)
    {
        redde;
    }
    k->in_lectione = VERUM;
    si (eventus & (i32)REACTOR_ERROR)
    {
        k->claudenda = VERUM;
    }
    si (!k->claudenda && (eventus & (i32)REACTOR_SCRIBERE))
    {
        _scriptio_urgere(k);
    }
    si (   !k->claudenda
        && (eventus & ((i32)REACTOR_LEGERE | (i32)REACTOR_CLAUSUM)))
    {
        _legere(k);
    }
    si (eventus & (i32)REACTOR_CLAUSUM)
    {
        k->finis_lectionis = VERUM;
    }
    k->in_lectione = FALSUM;
    si (!k->claudenda)
    {
        _interesse(k);
    }
    _fortasse_claudere(k);
}

interior vacuum
_accipere_cb (
    integer  fd,
        i32  eventus,
     vacuum* data)
{
    McpCanalis* mc = (McpCanalis*)data;

    (vacuum)fd;
    (vacuum)eventus;
    dum (!mc->clausus)
    {
                   Piscina* p;
               TcpResultus  r;
        McpCanalisConnexio* k = NIHIL;
                       i32  j;

        p = piscina_generare_dynamicum("mcp_canalis_connexio",
            (memoriae_index)PISCINA_CONNEXIONIS);
        si (!p)
        {
            frange;
        }
        r = canalis_accipere(mc->auditor, p);
        si (!r.successus)
        {
            piscina_destruere(p);
            frange;
        }
        si (!canalis_par_admissus(r.connexio))
        {
            fprintf(stderr, "canalis: par recusatus\n");
            tcp_claudere(r.connexio);
            piscina_destruere(p);
            perge;
        }
        per (j = ZEPHYRUM; j < MCP_CANALIS_CONNEXIONES && !k; j++)
        {
            si (!mc->connexiones[j].vivus)
            {
                k = &mc->connexiones[j];
            }
        }
        si (!k)
        {
            /* nonus: loca plena */
            tcp_claudere(r.connexio);
            piscina_destruere(p);
            perge;
        }
        k->tcp              = r.connexio;
        k->fd               = (integer)tcp_obtinere_fd(r.connexio);
        k->piscina          = p;
        k->vivus            = VERUM;
        k->in_reactore      = FALSUM;
        k->in_lectione      = FALSUM;
        k->claudenda        = FALSUM;
        k->finis_lectionis  = FALSUM;
        k->pendentes        = ZEPHYRUM;
        k->scripta          = ZEPHYRUM;
        memset(&k->lectio, ZEPHYRUM, magnitudo(k->lectio));
        memset(&k->scriptio, ZEPHYRUM, magnitudo(k->scriptio));
        _interesse(k);
        _fortasse_claudere(k);
    }
}

McpCanalis*
mcp_canalis_praebere (
              McpServus* servus,
                Reactor* reactor,
     constans character* via,
                 chorda* error,
                Piscina* piscina)
{
    CanalisResultus  a;
         McpCanalis* mc;
                i32  j;

    si (error && piscina)
    {
        *error = chorda_ex_literis("", piscina);
    }
    si (!servus || !reactor || !via || !piscina)
    {
        si (error && piscina)
        {
            *error = chorda_ex_literis("argumenta canalis desunt",
                piscina);
        }
        redde NIHIL;
    }
    a = canalis_audire(via, piscina);
    si (!a.successus)
    {
        si (error)
        {
            *error = a.error_descriptio;
        }
        redde NIHIL;
    }
    mc = (McpCanalis*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(McpCanalis));
    si (!mc)
    {
        canalis_claudere(a.auditor);
        si (error)
        {
            *error = chorda_ex_literis("memoria deest", piscina);
        }
        redde NIHIL;
    }
    memset(mc, ZEPHYRUM, magnitudo(*mc));
    mc->servus          = servus;
    mc->reactor         = reactor;
    mc->auditor         = a.auditor;
    mc->piscina         = piscina;
    mc->persona.usor    = chorda_ex_literis("administratio", piscina);
    mc->persona.cliens  = chorda_ex_literis("canalis", piscina);
    mc->persona.scopi   = chorda_ex_literis("administratio", piscina);
    per (j = ZEPHYRUM; j < MCP_CANALIS_CONNEXIONES; j++)
    {
        mc->connexiones[j].canalis  = mc;
        mc->connexiones[j].locus    = j;
        mc->connexiones[j].fd       = -I;
    }
    si (!reactor_adicere(reactor, canalis_fossa(a.auditor),
            (i32)REACTOR_LEGERE, _accipere_cb, mc))
    {
        canalis_claudere(a.auditor);
        si (error)
        {
            *error = chorda_ex_literis("reactor fossam canalis non "
                "accipit", piscina);
        }
        redde NIHIL;
    }
    redde mc;
}

vacuum
mcp_canalis_claudere (
    McpCanalis* canalis)
{
    i32 j;

    si (!canalis || canalis->clausus)
    {
        redde;
    }
    per (j = ZEPHYRUM; j < MCP_CANALIS_CONNEXIONES; j++)
    {
        _connexionem_claudere(&canalis->connexiones[j]);
    }
    (vacuum)reactor_removere(canalis->reactor,
        canalis_fossa(canalis->auditor));
    canalis_claudere(canalis->auditor);
    canalis->clausus = VERUM;
}


/* ==================================================
 * Clientes
 * ================================================== */

/* fossae crudae (non socket): omnia aut FALSUM */
interior b32
_omnia_scribere (
         integer  fd,
     constans i8* d,
         ssize_t  n)
{
    dum (n > ZEPHYRUM)
    {
        ssize_t k = write(fd, d, (size_t)n);

        si (k < ZEPHYRUM && errno == EINTR)
        {
            perge;
        }
        si (k <= ZEPHYRUM)
        {
            redde FALSUM;
        }
        d += k;
        n -= k;
    }
    redde VERUM;
}

s32
mcp_canalis_pons (
    constans character* via,
               integer  intra,
               integer  extra)
{
        Piscina* p;
    TcpResultus  r;
        integer  s;
            b32  intra_aperta  = VERUM;
            s32  status        = ZEPHYRUM;
             i8  frustum[FRUSTUM_LECTIONIS];

    p = piscina_generare_dynamicum("mcp_canalis_pons",
        (memoriae_index)PISCINA_CONNEXIONIS);
    si (!p)
    {
        redde I;
    }
    r = canalis_connectere(via, p);
    si (!r.successus)
    {
        piscina_destruere(p);
        redde II;
    }
    s = (integer)tcp_obtinere_fd(r.connexio);
    dum (VERUM)
    {
        structura pollfd  pf[II];
                  nfds_t numerus = I;
                 integer k;

        pf[ZEPHYRUM].fd       = s;
        pf[ZEPHYRUM].events   = POLLIN;
        pf[ZEPHYRUM].revents  = ZEPHYRUM;
        si (intra_aperta)
        {
            pf[I].fd       = intra;
            pf[I].events   = POLLIN;
            pf[I].revents  = ZEPHYRUM;
            numerus        = II;
        }
        k = poll(pf, numerus, -I);
        si (k < ZEPHYRUM)
        {
            si (errno == EINTR)
            {
                perge;
            }
            status = I;
            frange;
        }
        si (pf[ZEPHYRUM].revents & (POLLIN | POLLHUP | POLLERR))
        {
            ssize_t m = read(s, frustum, magnitudo(frustum));

            si (m < ZEPHYRUM && errno == EINTR)
            {
                perge;
            }
            si (m < ZEPHYRUM)
            {
                status = I;
                frange;
            }
            si (m == ZEPHYRUM)
            {
                /* servus finivit aut mortuus est */
                frange;
            }
            si (!_omnia_scribere(extra, frustum, m))
            {
                status = I;
                frange;
            }
        }
        si (   intra_aperta
            && (pf[I].revents & (POLLIN | POLLHUP | POLLERR)))
        {
            ssize_t m = read(intra, frustum, magnitudo(frustum));

            si (m < ZEPHYRUM && errno == EINTR)
            {
                perge;
            }
            si (m <= ZEPHYRUM)
            {
                intra_aperta = FALSUM;
                (vacuum)shutdown(s, SHUT_WR);
            }
            alioquin si (!tcp_mittere_omnia(r.connexio, frustum,
                         (i32)m))
            {
                status = I;
                frange;
            }
        }
    }
    tcp_claudere(r.connexio);
    piscina_destruere(p);
    redde status;
}

interior s64
_nunc_ms (vacuum)
{
    structura timespec t;

    si (clock_gettime(CLOCK_MONOTONIC, &t) != ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    redde (s64)t.tv_sec * (s64)M + (s64)(t.tv_nsec / (signatus longus)
        (M * M));
}

interior b32
_lineam_mittere (
    TcpConnexio* c,
      JsonValor* v,
        Piscina* p)
{
    chorda s = json_scribere(v, p);

    redde    tcp_mittere_omnia(c, s.datum, s.mensura)
          && tcp_mittere_omnia(c, (constans i8*)"\n", I);
}

/* responsum vocationis (id II) iudicare */
interior s32
_responsum (
    JsonValor* radix,
       chorda* textus,
      Piscina* p)
{
    JsonValor* error = json_objectum_capere(radix, "error");
    JsonValor* t;

    si (error)
    {
        *textus = json_capere_chorda(error, "message",
            chorda_ex_literis("error protocolli", p));
        redde III;
    }
    t        = json_via(radix, "result.content.0.text");
    *textus  = t ? json_ad_chorda(t) : chorda_ex_literis("", p);
    redde json_capere_boolean(json_objectum_capere(radix, "result"),
        "isError", FALSUM) ? I : ZEPHYRUM;
}

s32
mcp_canalis_vocare (
    constans character* via,
    constans character* instrumentum,
             JsonValor* argumenta,
                 chorda* textus,
                Piscina* piscina)
{
    TcpResultus  r;
      JsonValor* v;
      JsonValor* params;
      JsonValor* ci;
         Alveus  lectio;
            s64  finis;
            s32  status = III;
        integer  fd;

    si (!textus || !piscina || !via || !instrumentum)
    {
        redde III;
    }
    *textus  = chorda_ex_literis("", piscina);
    r        = canalis_connectere(via, piscina);
    si (!r.successus)
    {
        *textus = r.error_descriptio;
        redde II;
    }

    /* initialize */
    v       = json_objectum_creare(piscina);
    params  = json_objectum_creare(piscina);
    ci      = json_objectum_creare(piscina);
    json_objectum_ponere(v, "jsonrpc",
        json_chorda_creare_literis(piscina,
        "2.0"));
    json_objectum_ponere(v, "id", json_integer_creare(piscina, I));
    json_objectum_ponere(v, "method",
        json_chorda_creare_literis(piscina,
        "initialize"));
    json_objectum_ponere(params, "protocolVersion",
        json_chorda_creare_literis(piscina,
        MCP_PROTOCOLLUM_NOVISSIMUM));
    json_objectum_ponere(params, "capabilities",
        json_objectum_creare(piscina));
    json_objectum_ponere(ci, "name", json_chorda_creare_literis(piscina,
        "mcp_canalis_vocare"));
    json_objectum_ponere(ci, "version", json_chorda_creare_literis(
        piscina, "1"));
    json_objectum_ponere(params, "clientInfo", ci);
    json_objectum_ponere(v, "params", params);
    si (!_lineam_mittere(r.connexio, v, piscina))
    {
        tcp_claudere(r.connexio);
        *textus = chorda_ex_literis("scriptio fracta", piscina);
        redde III;
    }

    /* notifications/initialized */
    v = json_objectum_creare(piscina);
    json_objectum_ponere(v, "jsonrpc",
        json_chorda_creare_literis(piscina,
        "2.0"));
    json_objectum_ponere(v, "method",
        json_chorda_creare_literis(piscina,
        "notifications/initialized"));
    (vacuum)_lineam_mittere(r.connexio, v, piscina);

    /* tools/call */
    v       = json_objectum_creare(piscina);
    params  = json_objectum_creare(piscina);
    json_objectum_ponere(v, "jsonrpc",
        json_chorda_creare_literis(piscina,
        "2.0"));
    json_objectum_ponere(v, "id", json_integer_creare(piscina, II));
    json_objectum_ponere(v, "method",
        json_chorda_creare_literis(piscina,
        "tools/call"));
    json_objectum_ponere(params, "name", json_chorda_creare_literis(
        piscina, instrumentum));
    json_objectum_ponere(params, "arguments", argumenta ? argumenta :
        json_objectum_creare(piscina));
    json_objectum_ponere(v, "params", params);
    si (!_lineam_mittere(r.connexio, v, piscina))
    {
        tcp_claudere(r.connexio);
        *textus = chorda_ex_literis("scriptio fracta", piscina);
        redde III;
    }

    /* lineae usque ad responsum id II */
    memset(&lectio, ZEPHYRUM, magnitudo(lectio));
    fd       = (integer)tcp_obtinere_fd(r.connexio);
    finis    = _nunc_ms() + (s64)MCP_CANALIS_MORA_VOCATIONIS_MS;
    *textus  = chorda_ex_literis("tempus vocationis exiit", piscina);
    dum (VERUM)
    {
        structura pollfd  pf;
                     s64 reliquum = finis - _nunc_ms();
                     s32 n;
                     i32 j;
                     i32 initium = ZEPHYRUM;
                     b32 decisum = FALSUM;
                      i8 frustum[FRUSTUM_LECTIONIS];

        si (reliquum <= ZEPHYRUM)
        {
            frange;
        }
        pf.fd       = fd;
        pf.events   = POLLIN;
        pf.revents  = ZEPHYRUM;
        si (poll(&pf, I, (integer)reliquum) <= ZEPHYRUM)
        {
            perge;
        }
        n = tcp_recipere(r.connexio, frustum, (i32)magnitudo(frustum));
        si (n == TCP_ITERUM)
        {
            perge;
        }
        si (n <= ZEPHYRUM)
        {
            *textus =
                chorda_ex_literis("connexio clausa ante responsum",
                piscina);
            frange;
        }
        si (!_alveus_addere(&lectio, piscina, frustum, (i32)n))
        {
            frange;
        }
        per (j = ZEPHYRUM; j < lectio.longitudo && !decisum; j++)
        {
            si (lectio.datum[j] == (i8)'\n')
            {
                      chorda linea;
                JsonResultus resultus;

                linea.datum    = lectio.datum + initium;
                linea.mensura  = j - initium;
                initium        = j + I;
                resultus       = json_legere(linea, piscina);
                si (   resultus.successus
                    && json_capere_integer(resultus.radix, "id", -I)
                        == II)
                {
                    status = _responsum(resultus.radix, textus,
                        piscina);
                    decisum = VERUM;
                }
            }
        }
        si (decisum)
        {
            frange;
        }
        _alveus_consumere(&lectio, initium);
    }
    tcp_claudere(r.connexio);
    redde status;
}

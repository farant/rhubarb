/* horreum_simulacrum_rete.h - IANUA RETIS simulacri S3 (horreum-plan
 * T5, P8): idem exemplar (horreum_simulacrum.h) post tcp_servus in
 * 127.0.0.1 (portus ephemerus), in EODEM reactore ac cliens probatus.
 *
 * Connexiones SINGULAE (ut servus_scriptus.h): dum una aperta est,
 * ceterae clauduntur. Petitio per http_parser colligitur, in
 * HttpPetitio reficitur (URI cruda, capita omnia praeter Connection,
 * corpus), simulacro datur; responsum serializatur cum "Connection:
 * close" et connexio clauditur. SCRIBERE_ABRUMPERE = connexio sine
 * responso clauditur. HEAD: Content-Length exemplaris servatur, nullum
 * corpus.
 *
 * Functiones INTERIORES: includens AMBOS aditus vocare debet -
 * simulacrum_rete_aperire et simulacrum_rete_claudere. */
#ifndef HORREUM_SIMULACRUM_RETE_H
#define HORREUM_SIMULACRUM_RETE_H

#include "horreum_simulacrum.h"
#include "reactor.h"
#include "tcp.h"

#define SIMULACRUM_RETE_CAPSA      (IV * MXXIV)
#define SIMULACRUM_RETE_PETITIO    (XXXII * MXXIV * MXXIV)

nomen structura {
     Simulacrum* simulacrum;
        Reactor* reactor;
        Piscina* piscina;
      TcpServus* servus;
        integer  fossa_auscultans;
    TcpConnexio* cliens;
        integer  fossa_clientis;
     HttpParser* parser;
            i32  portus;
            i32  acceptae;
} SimulacrumRete;

interior vacuum
_rete_clientem_claudere (
    SimulacrumRete* r)
{
    si (r->fossa_clientis >= ZEPHYRUM)
    {
        (vacuum)reactor_removere(r->reactor, r->fossa_clientis);
        r->fossa_clientis = -I;
    }
    si (r->cliens)
    {
        tcp_claudere(r->cliens);
        r->cliens = NIHIL;
    }
}

/* HttpPetitioServeri -> HttpPetitio quam simulacrum iudicat */
interior HttpPetitio*
_rete_petitio (
                 SimulacrumRete* r,
    constans HttpPetitioServeri* ps)
{
    ChordaAedificator* u = chorda_aedificator_creare(r->piscina, CCLVI);
          HttpPetitio* pet;
                  i32  i;

    chorda_aedificator_appendere_literis(u, "http://127.0.0.1:");
    chorda_aedificator_appendere_i32(u, r->portus);
    chorda_aedificator_appendere_chorda(u, ps->uri);
    pet = http_petitio_creare(r->piscina, ps->methodus,
        chorda_ut_cstr(chorda_aedificator_finire(u), r->piscina));
    si (!pet)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < ps->capita_numerus; i++)
    {
        si (chorda_aequalis_case_insensitivus(ps->capita[i].titulus,
                _sim_l("Connection", r->piscina)))
        {
            perge;
        }
        http_petitio_caput_addere(pet,
            chorda_ut_cstr(ps->capita[i].titulus, r->piscina),
            chorda_ut_cstr(ps->capita[i].valor, r->piscina));
    }
    si (ps->corpus.mensura > ZEPHYRUM)
    {
        http_petitio_corpus_ponere_chorda(pet, ps->corpus);
    }
    redde pet;
}

interior vacuum
_rete_respondere (
                 SimulacrumRete* r,
    constans HttpPetitioServeri* ps)
{
     HttpPetitio* pet = _rete_petitio(r, ps);
    HttpResultus  res;
             s64  longitudo;
          chorda  capita;

    si (!pet)
    {
        redde;
    }
    res = _sim_exsequi(pet, r->piscina, r->simulacrum);
    si (!res.successus || !res.responsum)
    {
        redde;                          /* SCRIBERE_ABRUMPERE */
    }
    longitudo = (s64)res.responsum->corpus.mensura;
    si (   ps->methodus == HTTP_HEAD
        && !chorda_ut_s64(http_responsum_caput(res.responsum,
            "Content-Length"), &longitudo))
    {
        longitudo = ZEPHYRUM;
    }
    capita = http_responsum_capita_serialize(res.responsum, FALSUM,
        (i64)longitudo, r->piscina);
    (vacuum)tcp_mittere_omnia(r->cliens, capita.datum,
        (i32)capita.mensura);
    si (   ps->methodus != HTTP_HEAD && res.responsum->corpus.mensura
        > ZEPHYRUM)
    {
        (vacuum)tcp_mittere_omnia(r->cliens,
            res.responsum->corpus.datum,
            (i32)res.responsum->corpus.mensura);
    }
}

interior vacuum
_rete_lectio (
    integer  fossa,
        i32  eventus,
     vacuum* datum)
{
       SimulacrumRete* r = (SimulacrumRete*)datum;
                   i8  capsa[SIMULACRUM_RETE_CAPSA];
                  s32  n;
    HttpParseResultus  pr;

    (vacuum)fossa;
    (vacuum)eventus;
    si (!r->cliens)
    {
        redde;
    }
    n = tcp_recipere(r->cliens, capsa, SIMULACRUM_RETE_CAPSA);
    si (n > ZEPHYRUM)
    {
        pr = http_parser_adicere(r->parser, (constans character*)capsa,
            (i32)n);
        si (!pr.successus)
        {
            _rete_clientem_claudere(r);
            redde;
        }
        si (pr.completa)
        {
            _rete_respondere(r, pr.petitio);
            _rete_clientem_claudere(r);
        }
        redde;
    }
    si (n == ZEPHYRUM)
    {
        _rete_clientem_claudere(r);     /* cliens clausit */
    }
}

interior vacuum
_rete_acceptio (
    integer  fossa,
        i32  eventus,
     vacuum* datum)
{
    SimulacrumRete* r = (SimulacrumRete*)datum;
       TcpResultus  acc;

    (vacuum)fossa;
    (vacuum)eventus;
    acc = tcp_servus_accipere(r->servus, r->piscina);
    si (!acc.successus)
    {
        redde;
    }
    r->acceptae++;
    si (r->cliens)
    {
        tcp_claudere(acc.connexio);     /* una simul */
        redde;
    }
    r->parser = http_parser_creare_cum_limitibus(r->piscina,
        SIMULACRUM_RETE_PETITIO, ZEPHYRUM);
    r->cliens          = acc.connexio;
    r->fossa_clientis  = (integer)tcp_obtinere_fd(acc.connexio);
    (vacuum)reactor_adicere(r->reactor, r->fossa_clientis,
        REACTOR_LEGERE, _rete_lectio, r);
}

interior SimulacrumRete*
simulacrum_rete_aperire (
    Simulacrum* s,
       Reactor* reactor,
       Piscina* p)
{
       SimulacrumRete* r;
    TcpServusOptiones  o = tcp_servus_optiones_default();
    TcpServusResultus  sr;

    r = (SimulacrumRete*)piscina_allocare(p,
        (memoriae_index)magnitudo(SimulacrumRete));
    memset(r, ZEPHYRUM, magnitudo(*r));
    r->simulacrum = s;
    r->reactor = reactor;
    r->piscina = p;
    r->fossa_clientis = -I;
    sr = tcp_servus_creare_cum_optionibus("127.0.0.1", ZEPHYRUM, &o, p);
    si (   !sr.successus
        || tcp_servus_auscultare(sr.servus, VIII) != TCP_OK)
    {
        redde NIHIL;
    }
    r->servus            = sr.servus;
    r->portus            = tcp_servus_obtinere_portum(sr.servus);
    r->fossa_auscultans  = tcp_servus_obtinere_fd(sr.servus);
    (vacuum)reactor_adicere(reactor, r->fossa_auscultans,
        REACTOR_LEGERE,
        _rete_acceptio, r);
    redde r;
}

interior vacuum
simulacrum_rete_claudere (
    SimulacrumRete* r)
{
    si (!r)
    {
        redde;
    }
    _rete_clientem_claudere(r);
    si (r->servus)
    {
        (vacuum)reactor_removere(r->reactor, r->fossa_auscultans);
        tcp_servus_claudere(r->servus);
        r->servus = NIHIL;
    }
}

#endif /* HORREUM_SIMULACRUM_RETE_H */

/* servus_scriptus.h - servus HTTP SCRIPTUS pro probationibus clientium
 * non-obstruentium (http-vocatio-spec III)
 *
 * tcp_servus in 127.0.0.1 (portus ephemerus) in EODEM reactore ac
 * cliens probatus: processus unus, ansa una, ordo determinatus.
 * Connexiones SINGULAE: dum una aperta est, ceterae clauduntur; post
 * clausuram connexio proxima scriptum AB INITIO recipit (iteratio
 * clientis idem scriptum videt - acceptae numerat). Petitio colligitur
 * (capita + corpus Content-Length), deinde gradus scripti: post mora_ms
 * octeti mittuntur; claudere: connexio post gradum clauditur. Gradus
 * nullus = silentium (cliens tempus suum experitur).
 *
 * Functiones INTERIORES (more ludus_toy.h): includens AMBOS aditus
 * vocare debet - servum_scriptum_creare et servum_scriptum_claudere -
 * qui ceteras omnes trahunt. */
#ifndef SERVUS_SCRIPTUS_H
#define SERVUS_SCRIPTUS_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "reactor.h"
#include "tcp.h"
#include <string.h>

#define SERVUS_CAPSA DXII

nomen structura {
    constans character* octeti;     /* NIHIL = nihil mittitur */
                   i32  mensura;    /* 0 = strlen(octeti) */
                   i32  mora_ms;    /* post gradum priorem */
                   b32  claudere;   /* post hunc gradum */
} ServusGradus;

nomen structura {
               Reactor* reactor;
               Piscina* piscina;
             TcpServus* servus;
               integer  fossa_auscultans;
           TcpConnexio* cliens;
               integer  fossa_clientis;
 constans ServusGradus* gradus;
                   i32  numerus;
                   i32  index;
     ChordaAedificator* petitio;     /* octeti petitionis recepti */
                   b32  petitio_plena;
                   i32  portus;
                   i32  acceptae;
        ReactorTimerId  mora;
                   b32  mora_posita;
} ServusScriptus;

interior vacuum
_servi_gradus (
    vacuum* datum);

interior vacuum
_servi_clientem_claudere (
    ServusScriptus* s)
{
    si (s->fossa_clientis >= ZEPHYRUM)
    {
        (vacuum)reactor_removere(s->reactor, s->fossa_clientis);
        s->fossa_clientis = -I;
    }
    si (s->cliens)
    {
        tcp_claudere(s->cliens);
        s->cliens = NIHIL;
    }
}

interior vacuum
_servi_proximum (
    ServusScriptus* s)
{
    si (s->index >= s->numerus)
    {
        redde;
    }
    s->mora = reactor_timer_adicere(s->reactor,
        s->gradus[s->index].mora_ms, FALSUM, _servi_gradus, s);
    s->mora_posita = VERUM;
}

interior vacuum
_servi_gradus (
    vacuum* datum)
{
           ServusScriptus* s = (ServusScriptus*)datum;
    constans ServusGradus* g;
                      i32  n;

    s->mora_posita = FALSUM;
    si (s->index >= s->numerus || !s->cliens)
    {
        redde;
    }
    g = &s->gradus[s->index];
    s->index++;
    si (g->octeti)
    {
        n = g->mensura > ZEPHYRUM ? g->mensura : (i32)strlen(g->octeti);
        si (n > ZEPHYRUM)
        {
            (vacuum)tcp_mittere_omnia(s->cliens,
                (constans i8*)g->octeti,
                n);
        }
    }
    si (g->claudere)
    {
        _servi_clientem_claudere(s);
        redde;
    }
    _servi_proximum(s);
}

/* littera parva ASCII sine ctype */
interior character
_servi_parva (
    character x)
{
    redde (x >= 'A' && x <= 'Z') ? (character)(x + ('a' - 'A')) : x;
}

/* capita finita et corpus Content-Length totum receptum? */
interior b32
_servi_petitio_plena (
    ServusScriptus* s)
{
                chorda p =
                    chorda_aedificator_spectare(s->petitio);
                   s32  finis_capitum = -I;
                   s32  k;
                   s32  j;
                   s32  longitudo  = ZEPHYRUM;
    constans character* clavis     = "content-length:";

    per (k = ZEPHYRUM; k + III < (s32)p.mensura; k++)
    {
        si (   p.datum[k]      == '\r' && p.datum[k + I] == '\n'
            && p.datum[k + II] == '\r' && p.datum[k + III] == '\n')
        {
            finis_capitum = k;
            frange;
        }
    }
    si (finis_capitum < ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k + XV <= finis_capitum; k++)
    {
        per (j = ZEPHYRUM; j < XV
            && _servi_parva((character)p.datum[k + j])
                == clavis[j]; j++)
        {
        }
        si (j == XV)
        {
            per (j = k + XV; j < finis_capitum
                && (p.datum[j] == ' ' || (p.datum[j] >= '0'
                && p.datum[j] <= '9')); j++)
            {
                si (p.datum[j] != ' ')
                {
                    longitudo = longitudo * X + (p.datum[j] - '0');
                }
            }
            frange;
        }
    }
    redde (s32)p.mensura >= finis_capitum + IV + longitudo;
}

interior vacuum
_servi_lectio (
    integer  fossa,
        i32  eventus,
     vacuum* datum)
{
     ServusScriptus* s = (ServusScriptus*)datum;
                 i8  capsa[SERVUS_CAPSA];
                s32  n;
                s32  k;

    (vacuum)fossa;
    (vacuum)eventus;
    si (!s->cliens)
    {
        redde;
    }
    n = tcp_recipere(s->cliens, capsa, SERVUS_CAPSA);
    si (n > ZEPHYRUM)
    {
        per (k = ZEPHYRUM; k < n; k++)
        {
            chorda_aedificator_appendere_character(s->petitio,
                (character)capsa[k]);
        }
        si (!s->petitio_plena && _servi_petitio_plena(s))
        {
            s->petitio_plena = VERUM;
            _servi_proximum(s);
        }
        redde;
    }
    si (n == ZEPHYRUM)
    {
        _servi_clientem_claudere(s);     /* cliens clausit */
    }
}

interior vacuum
_servi_acceptio (
    integer  fossa,
        i32  eventus,
     vacuum* datum)
{
     ServusScriptus* s = (ServusScriptus*)datum;
        TcpResultus  acc;

    (vacuum)fossa;
    (vacuum)eventus;
    acc = tcp_servus_accipere(s->servus, s->piscina);
    si (!acc.successus)
    {
        redde;
    }
    s->acceptae++;
    si (s->cliens)
    {
        tcp_claudere(acc.connexio);      /* una simul */
        redde;
    }
    /* connexio nova: scriptum ab initio */
    si (s->mora_posita)
    {
        (vacuum)reactor_timer_cancellare(s->reactor, s->mora);
        s->mora_posita = FALSUM;
    }
    s->index           = ZEPHYRUM;
    s->petitio_plena   = FALSUM;
    s->petitio         = chorda_aedificator_creare(s->piscina, D);
    s->cliens          = acc.connexio;
    s->fossa_clientis  = (integer)tcp_obtinere_fd(acc.connexio);
    (vacuum)reactor_adicere(s->reactor, s->fossa_clientis,
        REACTOR_LEGERE,
        _servi_lectio, s);
}

interior ServusScriptus*
servum_scriptum_creare (
                  Reactor* reactor,
    constans ServusGradus* gradus,
                      i32  numerus,
                  Piscina* piscina)
{
       ServusScriptus* s;
    TcpServusOptiones  o = tcp_servus_optiones_default();
    TcpServusResultus  sr;

    s = (ServusScriptus*)piscina_allocare(piscina,
        magnitudo(ServusScriptus));
    memset(s, ZEPHYRUM, magnitudo(ServusScriptus));
    s->reactor         = reactor;
    s->piscina         = piscina;
    s->gradus          = gradus;
    s->numerus         = numerus;
    s->fossa_clientis  = -I;
    s->petitio         = chorda_aedificator_creare(piscina, D);
    sr = tcp_servus_creare_cum_optionibus("127.0.0.1", ZEPHYRUM, &o,
        piscina);
    si (   !sr.successus
        || tcp_servus_auscultare(sr.servus, VIII) != TCP_OK)
    {
        redde NIHIL;
    }
    s->servus            = sr.servus;
    s->portus            = tcp_servus_obtinere_portum(sr.servus);
    s->fossa_auscultans  = tcp_servus_obtinere_fd(sr.servus);
    (vacuum)reactor_adicere(reactor, s->fossa_auscultans,
        REACTOR_LEGERE,
        _servi_acceptio, s);
    redde s;
}

interior vacuum
servum_scriptum_claudere (
    ServusScriptus* s)
{
    si (!s)
    {
        redde;
    }
    si (s->mora_posita)
    {
        (vacuum)reactor_timer_cancellare(s->reactor, s->mora);
        s->mora_posita = FALSUM;
    }
    _servi_clientem_claudere(s);
    si (s->servus)
    {
        (vacuum)reactor_removere(s->reactor, s->fossa_auscultans);
        tcp_servus_claudere(s->servus);
        s->servus = NIHIL;
    }
}

#endif /* SERVUS_SCRIPTUS_H */

/* tls_tls13.c - VECTURA TLS 1.3 propria (tls13-spec II.1, tls13-plan
 * T6)
 *
 * Machina tls13.c fossam non tangit: capsae recordorum (exitus,
 * introitus) et datorum. _antlia eam super tcp_mittere/tcp_recipere
 * agit - EADEM pro modo gradatim (fossa non-blocans: 'exspecta' =
 * LEGERE/SCRIBERE) et obstruente (fossa cum moris tcp: 'exspecta' =
 * tempus excessum), ut tls_bearssl.c functio pro functione.
 *
 * Fiducia: reditus machinae = fiducia_iudicare cum ancora connexionis
 * (platforma iudicat). Errores machinae -> TlsError per tabulam spec
 * II.4: FIDUCIA et SUBSCRIPTIO -> CERTIFICATUM, ceteri -> HANDSHAKE;
 * causa machinae (FIDUCIA: ratio fiduciae) servatur. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "tcp.h"
#include "tls.h"
#include "fiducia.h"
#include "tls13.h"
#include "tls_tls13.h"
#include <string.h>

nomen enumeratio {
    ANTLIA_PARATA = 0,   /* status quaesitus adest */
    ANTLIA_LEGERE,       /* fossa legibilis exspectanda (obstruens:
                          * tempus excessum) */
    ANTLIA_SCRIBERE,     /* fossa scribibilis exspectanda */
    ANTLIA_CLAUSA,       /* close_notify, aut EOF post pactionem */
    ANTLIA_ERROR         /* connexio->erratum nominat */
} Antlia;

structura TlsTls13 {
          Tls13Machina* m;
           TcpConnexio* tcp;
               Piscina* piscina;
    constans character* ancora_via;   /* NIHIL = repositorium */
                   b32  clausa;
                   b32  reiecta;      /* FIDUCIA aut SUBSCRIPTIO */
                   i32  pendens;      /* mittere: octeti accepti,
                                       * recorda nondum missa */
                chorda erratum;
};

/* ---- fiducia ---- */

interior b32
_fiducia (
                          vacuum* datum,
    constans FiduciaCertificatum* catena,
                             i32  numerus,
              constans character* hospes,
                          chorda* causa,
                         Piscina* piscina)
{
    TlsTls13* c = (TlsTls13*)datum;

    redde fiducia_iudicare(catena, numerus, hospes, c->ancora_via,
        causa,
        piscina);
}

/* ---- antlia ---- */

interior vacuum
_errorem_ponere (
               TlsTls13* c,
     constans character* nuntius)
{
    si (c->erratum.mensura == ZEPHYRUM)
    {
        c->erratum = chorda_ex_literis(nuntius, c->piscina);
    }
}

/* error machinae in connexionem: causa et categoria (reiecta) */
interior vacuum
_errorem_machinae (
    TlsTls13* c)
{
    Tls13Error e = tls13_machina_error(c->m);

    c->reiecta = e.codex == TLS13_ERROR_FIDUCIA
        || e.codex == TLS13_ERROR_SUBSCRIPTIO;
    si (c->erratum.mensura == ZEPHYRUM)
    {
        c->erratum = e.causa.mensura > ZEPHYRUM ? e.causa
            : chorda_ex_literis("machina TLS 1.3 fallita", c->piscina);
    }
}

/* machinam agere donec status 'sitis' adest, aut exspectandum, aut
 * clausa/error. Recorda mittenda SEMPER primum (etiam alerta post
 * errorem, conatu optimo). */
interior Antlia
_antlia (
    TlsTls13* c,
         i32  sitis)
{
    dum (VERUM)
    {
        i32 status = tls13_machina_status(c->m);
        b32 fracta = tls13_machina_error(c->m).codex
            != TLS13_ERROR_NULLUS;

        si (status & TLS13_RECORDA_MITTENDA)
        {
            i32  n = ZEPHYRUM;
             i8* b = tls13_recorda_exitus(c->m, &n);
            s32  w = tcp_mittere(c->tcp, (constans i8*)b, n);

            si (w < ZEPHYRUM)
            {
                si (fracta)
                {
                    _errorem_machinae(c);
                }
                _errorem_ponere(c, "scriptio fossae fallita");
                redde ANTLIA_ERROR;
            }
            si (w == ZEPHYRUM)
            {
                redde ANTLIA_SCRIBERE;
            }
            tls13_recorda_exitus_ack(c->m, (i32)w);
            perge;
        }
        si (fracta)
        {
            _errorem_machinae(c);
            redde ANTLIA_ERROR;
        }
        si (status & sitis)
        {
            redde ANTLIA_PARATA;
        }
        si (status & TLS13_CLAUSA)
        {
            redde ANTLIA_CLAUSA;
        }
        si (status & TLS13_RECORDA_EXSPECTAT)
        {
            i32  n = ZEPHYRUM;
             i8* b = tls13_recorda_introitus(c->m, &n);
            s32  r;

            si (b == NIHIL || n == ZEPHYRUM)
            {
                _errorem_ponere(c, "capsa introitus plena");
                redde ANTLIA_ERROR;
            }
            r = tcp_recipere(c->tcp, b, n);
            si (r == TCP_ITERUM)
            {
                redde ANTLIA_LEGERE;
            }
            si (r == ZEPHYRUM)
            {
                /* EOF sine close_notify: post pactionem finis (ut
                 * ceterae vecturae); in pactione error */
                si (tls13_machina_versio(c->m) != ZEPHYRUM)
                {
                    redde ANTLIA_CLAUSA;
                }
                _errorem_ponere(c,
                    "connexio a pare clausa in pactione");
                redde ANTLIA_ERROR;
            }
            si (r < ZEPHYRUM)
            {
                _errorem_ponere(c, "lectio fossae fallita");
                redde ANTLIA_ERROR;
            }
            tls13_recorda_introitus_ack(c->m, (i32)r);
            perge;
        }
        _errorem_ponere(c, "machina TLS 1.3 immota");
        redde ANTLIA_ERROR;
    }
}

/* ---- praeparatio ---- */

interior constans character*
_copiare (
    constans character* s,
               Piscina* piscina)
{
        size_t  n = strlen(s) + I;
     character* d = (character*)piscina_allocare(piscina, (i64)n);

    si (d != NIHIL)
    {
        memcpy(d, s, n);
    }
    redde d;
}

interior TlsTls13Resultus
_error (
              TlsError  error,
    constans character* nuntius,
               Piscina* piscina)
{
    TlsTls13Resultus r;

    memset(&r, ZEPHYRUM, magnitudo(r));
    r.successus         = FALSUM;
    r.connexio          = NIHIL;
    r.error             = error;
    r.error_descriptio  = chorda_ex_literis(nuntius, piscina);
    redde r;
}

/* connexio nova super tcp (possessa: clauditur si defectus) */
interior TlsTls13Resultus
_creare (
            TcpConnexio* tcp,
     constans character* hospes,
   constans TlsOptiones* optiones,
                Piscina* piscina)
{
    TlsTls13Resultus  r;
            TlsTls13* c;
       Tls13Optiones  o;

    memset(&r, ZEPHYRUM, magnitudo(r));
    si (tcp == NIHIL || hospes == NIHIL || piscina == NIHIL)
    {
        si (tcp)
        {
            tcp_claudere(tcp);
        }
        redde _error(TLS_ERROR_TCP, "Argumenta invalida", piscina);
    }
    c = (TlsTls13*)piscina_allocare(piscina, (i64)magnitudo(TlsTls13));
    si (c == NIHIL)
    {
        tcp_claudere(tcp);
        redde _error(TLS_ERROR_TCP, "Allocatio fallita", piscina);
    }
    memset(c, ZEPHYRUM, magnitudo(TlsTls13));
    c->tcp      = tcp;
    c->piscina  = piscina;
    si (optiones && optiones->ancora_via)
    {
        c->ancora_via = _copiare(optiones->ancora_via, piscina);
        si (c->ancora_via == NIHIL)
        {
            tcp_claudere(tcp);
            redde _error(TLS_ERROR_HANDSHAKE, "Allocatio fallita",
                piscina);
        }
        si (!filum_existit(c->ancora_via))
        {
            tcp_claudere(tcp);
            redde _error(TLS_ERROR_HANDSHAKE, "ancora legi non potest",
                piscina);
        }
    }
    memset(&o, ZEPHYRUM, magnitudo(o));
    o.hospes         = hospes;
    o.verificare     = optiones ? optiones->verificare_certificatum
                                : VERUM;
    o.fiducia        = _fiducia;
    o.fiducia_datum  = c;
    c->m             = tls13_machina_creare(&o, piscina);
    si (c->m == NIHIL)
    {
        tcp_claudere(tcp);
        redde _error(TLS_ERROR_HANDSHAKE, "machina TLS 1.3 creari non "
            "potest", piscina);
    }
    si (tls13_machina_error(c->m).codex != TLS13_ERROR_NULLUS)
    {
        chorda causa = tls13_machina_error(c->m).causa;

        tcp_claudere(tcp);
        r.successus         = FALSUM;
        r.connexio          = NIHIL;
        r.error             = TLS_ERROR_HANDSHAKE;
        r.error_descriptio  = causa;
        redde r;
    }
    r.successus                 = VERUM;
    r.connexio                  = c;
    r.error                     = TLS_OK;
    r.error_descriptio.datum    = NIHIL;
    r.error_descriptio.mensura  = ZEPHYRUM;
    redde r;
}

/* ---- publica (tls_tls13.h) ---- */

TlsTls13Resultus
tls_tls13_incipere (
             TcpConnexio* tcp,
      constans character* hospes,
    constans TlsOptiones* optiones,
                 Piscina* piscina)
{
    redde _creare(tcp, hospes, optiones, piscina);
}

/* obstruens: tcp cum moris (timeout_ms -> moris fossae), pactio per
 * antliam usque ad finem. 'Exspecta' in modo obstruente = tempus
 * excessum: TLS_ERROR_TCP, ut ceterae vecturae */
TlsTls13Resultus
tls_tls13_connectere (
      constans character* hospes,
                     i32  portus,
    constans TlsOptiones* optiones,
                 Piscina* piscina)
{
         TcpOptiones tcp_opt = tcp_optiones_default();
         TcpResultus tcp_res;
    TlsTls13Resultus r;

    memset(&r, ZEPHYRUM, magnitudo(r));
    si (hospes == NIHIL || piscina == NIHIL)
    {
        redde _error(TLS_ERROR_TCP, "Argumenta invalida", piscina);
    }
    si (optiones && optiones->timeout_ms > ZEPHYRUM)
    {
        tcp_opt.timeout_ms = optiones->timeout_ms;
    }
    tcp_res = tcp_connectere_cum_optionibus(hospes, portus, &tcp_opt,
        piscina);
    si (!tcp_res.successus)
    {
        r.successus         = FALSUM;
        r.connexio          = NIHIL;
        r.error             = TLS_ERROR_TCP;
        r.error_descriptio  = tcp_res.error_descriptio;
        redde r;
    }
    r = _creare(tcp_res.connexio, hospes, optiones, piscina);
    si (!r.successus)
    {
        redde r;
    }
    commutatio (_antlia(r.connexio, TLS13_DATA_MITTI_POSSUNT))
    {
        casus ANTLIA_PARATA:
            redde r;
        casus ANTLIA_LEGERE:
        casus ANTLIA_SCRIBERE:
            tls_tls13_claudere(r.connexio);
            redde _error(TLS_ERROR_TCP, "Tempus handshake excessum",
                piscina);
        ordinarius:
        {
               b32 reiecta  = r.connexio->reiecta;
            chorda causa    = r.connexio->erratum;

            si (tls_tls13_solum_1_2(r.connexio))
            {
                /* connexio cum fossa servatur: tls.c descendit */
                r.successus         = FALSUM;
                r.error             = TLS_ERROR_HANDSHAKE;
                r.error_descriptio  = causa;
                r.solum_1_2         = VERUM;
                redde r;
            }
            tls_tls13_claudere(r.connexio);
            r.successus  = FALSUM;
            r.connexio   = NIHIL;
            r.error             = reiecta ? TLS_ERROR_CERTIFICATUM
                                          : TLS_ERROR_HANDSHAKE;
            r.error_descriptio  = causa.mensura > ZEPHYRUM ? causa
                : chorda_ex_literis("pactio fallita", piscina);
            redde r;
        }
    }
}

TlsGradus
tls_tls13_gradus (
    TlsTls13* c)
{
    si (c == NIHIL || c->clausa)
    {
        redde TLS_GRADUS_ERROR;
    }
    commutatio (_antlia(c, TLS13_DATA_MITTI_POSSUNT))
    {
        casus ANTLIA_PARATA:
            redde TLS_GRADUS_PERFECTUS;
        casus ANTLIA_LEGERE:
            redde TLS_GRADUS_LEGERE;
        casus ANTLIA_SCRIBERE:
            redde TLS_GRADUS_SCRIBERE;
        casus ANTLIA_CLAUSA:
            _errorem_ponere(c, "connexio clausa in pactione");
            redde TLS_GRADUS_ERROR;
        ordinarius:
            redde TLS_GRADUS_ERROR;
    }
}

chorda
tls_tls13_gradus_error (
    TlsTls13* c)
{
    chorda vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    redde c ? c->erratum : vacua;
}

/* mittere: octeti in capsam datorum, effundere, recorda emittuntur.
 * Contractus tls.h: TLS_ITERUM = idem iterum vocandum; octeti iam
 * accepti (pendens) tunc redduntur cum recorda eorum missa sunt */
s32
tls_tls13_mittere (
       TlsTls13* c,
    constans i8* data,
            i32  mensura)
{
    Antlia a;

    si (c == NIHIL || c->clausa)
    {
        redde -I;
    }
    si (data == NIHIL || mensura == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    si (c->pendens == ZEPHYRUM)
    {
        i32  capax = ZEPHYRUM;
         i8* b;
        i32  n;

        a = _antlia(c, TLS13_DATA_MITTI_POSSUNT);
        si (a == ANTLIA_LEGERE || a == ANTLIA_SCRIBERE)
        {
            redde TLS_ITERUM;
        }
        si (a != ANTLIA_PARATA)
        {
            redde -I;
        }
        b = tls13_data_exitus(c->m, &capax);
        si (b == NIHIL || capax == ZEPHYRUM)
        {
            redde -I;
        }
        n = mensura < capax ? mensura : capax;
        memcpy(b, data, n);
        tls13_data_exitus_ack(c->m, n);
        tls13_data_effundere(c->m);
        c->pendens = n;
    }
    a = _antlia(c, TLS13_DATA_MITTI_POSSUNT);   /* recorda effundere */
    si (a == ANTLIA_SCRIBERE || a == ANTLIA_LEGERE)
    {
        redde TLS_ITERUM;
    }
    si (a != ANTLIA_PARATA)
    {
        redde -I;
    }
    {
        i32 n = c->pendens;

        c->pendens = ZEPHYRUM;
        redde (s32)n;
    }
}

s32
tls_tls13_recipere (
    TlsTls13* c,
          i8* buffer,
         i32  capacitas)
{
    si (c == NIHIL || c->clausa)
    {
        redde -I;
    }
    si (buffer == NIHIL || capacitas == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    commutatio (_antlia(c, TLS13_DATA_ADSUNT))
    {
        casus ANTLIA_PARATA:
        {
            i32  n = ZEPHYRUM;
             i8* b = tls13_data_introitus(c->m, &n);

            si (n > capacitas)
            {
                n = capacitas;
            }
            memcpy(buffer, b, n);
            tls13_data_introitus_ack(c->m, n);
            redde (s32)n;
        }
        casus ANTLIA_LEGERE:
        casus ANTLIA_SCRIBERE:
            redde TLS_ITERUM;   /* obstruens: tempus excessum
                                 * (TCP_ITERUM) */
        casus ANTLIA_CLAUSA:
            c->clausa = VERUM;
            redde ZEPHYRUM;
        ordinarius:
            redde -I;
    }
}

vacuum
tls_tls13_claudere (
    TlsTls13* c)
{
    si (c == NIHIL)
    {
        redde;
    }
    si (!c->clausa && c->m != NIHIL && c->tcp != NIHIL)
    {
        i32 k;

        /* close_notify (aut alerta erroris pendens), conatu uno */
        tls13_machina_claudere(c->m);
        per (k = ZEPHYRUM; k < VIII && (tls13_machina_status(c->m)
             & TLS13_RECORDA_MITTENDA); k++)
        {
            i32  n = ZEPHYRUM;
             i8* b = tls13_recorda_exitus(c->m, &n);
            s32  w = tcp_mittere(c->tcp, (constans i8*)b, n);

            si (w <= ZEPHYRUM)
            {
                frange;
            }
            tls13_recorda_exitus_ack(c->m, (i32)w);
        }
    }
    si (c->tcp != NIHIL)
    {
        tcp_claudere(c->tcp);
        c->tcp = NIHIL;
    }
    c->clausa = VERUM;
}

b32
tls_tls13_solum_1_2 (
    TlsTls13* c)
{
    redde c != NIHIL && c->m != NIHIL
        && tls13_machina_error(c->m).codex == TLS13_ERROR_SOLUM_1_2;
}

TcpConnexio*
tls_tls13_tcp_relinquere (
    TlsTls13* c)
{
    TcpConnexio* tcp;

    si (c == NIHIL)
    {
        redde NIHIL;
    }
    tcp        = c->tcp;
    c->tcp     = NIHIL;
    c->clausa  = VERUM;
    redde tcp;
}

i32
tls_tls13_versio (
    TlsTls13* c)
{
    redde c != NIHIL && c->m != NIHIL ? tls13_machina_versio(c->m)
                                       : ZEPHYRUM;
}

b32
tls_tls13_est_valida (
    TlsTls13* c)
{
    redde c != NIHIL && !c->clausa && c->tcp != NIHIL
        && tcp_est_valida(c->tcp);
}

/* folium catenae machinae (DER in piscina) ->
 * fiducia_folium_describere - idem codex ac ceterae vecturae */
b32
tls_tls13_certificatum (
           TlsTls13* c,
    TlsCertificatum* exitus)
{
                   FiduciaFolium  ff;
    constans FiduciaCertificatum* catena;
                             i32  numerus = ZEPHYRUM;

    si (exitus == NIHIL)
    {
        redde FALSUM;
    }
    memset(exitus, ZEPHYRUM, magnitudo(*exitus));
    si (c == NIHIL || c->clausa)
    {
        redde FALSUM;
    }
    catena = tls13_machina_catena(c->m, &numerus);
    si (catena == NIHIL || !fiducia_folium_describere(&catena[ZEPHYRUM],
        &ff, c->piscina))
    {
        redde FALSUM;
    }
    exitus->valida     = ff.valida;
    exitus->non_ante   = ff.non_ante;
    exitus->non_post   = ff.non_post;
    exitus->subiectum  = ff.subiectum;
    redde exitus->valida;
}

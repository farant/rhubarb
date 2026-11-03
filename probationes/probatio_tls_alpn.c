/* probatio_tls_alpn.c - ALPN in tls.h (RFC 7301; http2-spec III.4,
 * http2-plan T7): extensio in ClientHello nostro, lectio ex
 * EncryptedExtensions, oblatum probatum, et vivum contra openssl
 * s_server -alpn per vecturas TLS 1.3 propriam et BearSSL (etiam per
 * descensum). Sine indice protocollorum nihil mutatur. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "tcp.h"
#include "tls.h"
#include "tls13_nuntii.h"
#include "filum.h"
#include "processus.h"
#include "servus_tls.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

nomen structura {
    character dir[DXII];
    character fixa[DXII];
    character ancora[MXXIV];
} Locus;

hic_manens constans character* constans PROTOCOLLA[II] = { "h2",
    "http/1.1" };

interior vacuum
_suitam_parare (
    CredoContextus* c)
{
                  Locus* l = (Locus*)piscina_allocare(c->piscina,
                      magnitudo(Locus));
     constans character* area = credo_area(c);
     constans character* argumenta[III];
      ProcessusResultus  pr;

    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_NECESSE_NON_NIHIL(area);
    strcpy(l->dir, area);
    sprintf(l->fixa, "%s/certa", area);
    sprintf(l->ancora, "%s/ca.der", l->fixa);
    argumenta[ZEPHYRUM] = "probationes/fixa/tls/generare.sh";
    argumenta[I] = l->fixa;
    argumenta[II] = NIHIL;
    pr = processus_exsequi(argumenta, XXX * M, c->piscina);
    CREDO_NECESSE(pr.successus && pr.codex_exitus == ZEPHYRUM);
    CREDO_NECESSE(filum_existit(l->ancora));
    c->datum = l;
}

interior vacuum
_suitam_purgare (
    CredoContextus* c)
{
    (vacuum)c;
}

/* octeti 'acus' in 'acervus'? */
interior b32
continet (
    constans i8* acervus,
            i32  n,
    constans i8* acus,
            i32  m)
{
    i32 i;

    per (i = ZEPHYRUM; i + m <= n; i++)
    {
        si (memcmp(acervus + i, acus, (size_t)m) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior i32
salutationem_scribere (
    constans character* constans* protocolla,
                             i32  numerus,
                             i8* exitus)
{
    Tls13SalutatioParametri p;
                         i8 temere[XXXII];
                         i8 clavis[XXXII];

    memset(temere, 0x11, XXXII);
    memset(clavis, 0x22, XXXII);
    memset(&p, ZEPHYRUM, magnitudo(p));
    p.hospes              = "exemplum.example.org";
    p.temere              = temere;
    p.grex                = TLS13_GREX_X25519;
    p.clavis_publica      = clavis;
    p.clavis_n            = XXXII;
    p.protocolla          = protocolla;
    p.protocolla_numerus  = numerus;
    redde tls13_salutatio_scribere(&p, exitus, MXXIV);
}

interior vacuum
_sectio_salutatio (
    CredoContextus* c)
{
    hic_manens constans i8 ALPN[XVIII] = {
        0x00, 0x10, 0x00, 0x0e, 0x00, 0x0c, 0x02, 'h', '2', 0x08,
        'h', 't', 't', 'p', '/', '1', '.', '1'
    };
     i8 cum[MXXIV];
     i8 sine[MXXIV];
    i32 n_cum;
    i32 n_sine;

    (vacuum)c;
    CREDO_GRADUS("extensio 0x0010 cum h2, http/1.1");
    n_cum = salutationem_scribere(PROTOCOLLA, II, cum);
    CREDO_VERUM(n_cum > ZEPHYRUM);
    CREDO_VERUM(continet(cum, n_cum, ALPN, XVIII));

    CREDO_GRADUS("sine indice: extensio abest, longitudo XVIII minor");
    n_sine = salutationem_scribere(NIHIL, ZEPHYRUM, sine);
    CREDO_AEQUALIS_I32(n_sine + XVIII, n_cum);
    CREDO_FALSUM(continet(sine, n_sine, ALPN, IV));

    CREDO_GRADUS("nomen vacuum aut nimium: nulla salutatio");
    {
        hic_manens constans character* constans MALA[I] = { "" };

        CREDO_AEQUALIS_I32(salutationem_scribere(MALA, I, cum),
            ZEPHYRUM);
    }
}

interior vacuum
_sectio_extensiones (
    CredoContextus* c)
{
    hic_manens constans i8 EE_H2[XI] = {
        0x00, 0x09, 0x00, 0x10, 0x00, 0x05, 0x00, 0x03, 0x02, 'h', '2'
    };
    hic_manens constans i8 EE_ALIA[XV] = {
        0x00, 0x0d, 0x00, 0x0a, 0x00, 0x00,
        0x00, 0x10, 0x00, 0x05, 0x00, 0x03, 0x02, 'h', '2'
    };
    hic_manens constans i8 EE_DUO[XIV] = {
        0x00, 0x0c, 0x00, 0x10, 0x00, 0x08, 0x00, 0x06, 0x02, 'h', '2',
        0x02, 'h', '3'
    };
    hic_manens constans i8 EE_VACUUM[IX] = {
        0x00, 0x07, 0x00, 0x10, 0x00, 0x03, 0x00, 0x01, 0x00
    };
    hic_manens constans i8  EE_NULLA[II] = { 0x00, 0x00 };
               constans i8* p;
                       i32  n;

    (vacuum)c;
    CREDO_GRADUS("ALPN h2, inter alias, abest");
    CREDO_VERUM(tls13_extensiones_legere(EE_H2, XI, &p, &n));
    CREDO_AEQUALIS_I32(n, II);
    CREDO_VERUM(memcmp(p, "h2", II) == ZEPHYRUM);
    CREDO_VERUM(tls13_extensiones_legere(EE_ALIA, XV, &p, &n));
    CREDO_AEQUALIS_I32(n, II);
    CREDO_VERUM(tls13_extensiones_legere(EE_NULLA, II, &p, &n));
    CREDO_AEQUALIS_I32(n, ZEPHYRUM);
    CREDO_VERUM(tls13_extensiones_legere(EE_H2, XI, NIHIL, NIHIL));

    CREDO_GRADUS("duo nomina aut nomen vacuum: mala forma (RFC 7301 3.1)");
    CREDO_FALSUM(tls13_extensiones_legere(EE_DUO, XIV, &p, &n));
    CREDO_FALSUM(tls13_extensiones_legere(EE_VACUUM, IX, &p, &n));

    CREDO_GRADUS("oblatum");
    CREDO_VERUM(tls13_protocollum_oblatum(PROTOCOLLA, II,
        (constans i8*)"h2", II));
    CREDO_VERUM(tls13_protocollum_oblatum(PROTOCOLLA, II,
        (constans i8*)"http/1.1", VIII));
    CREDO_FALSUM(tls13_protocollum_oblatum(PROTOCOLLA, II,
        (constans i8*)"h3", II));
    CREDO_FALSUM(tls13_protocollum_oblatum(NIHIL, ZEPHYRUM,
        (constans i8*)"h2", II));
}

/* servus cum vexillis; protocollum electum (aut "" ) */
interior b32
vivum (
          CredoContextus* c,
       constans character* vexilla,
              TlsVectura  vectura,
    constans character* constans* protocolla,
                     i32  numerus,
              TlsVectura* vectura_facta,
                  chorda* electum)
{
     constans Locus* l = (constans Locus*)c->suita;
          ServusTls  s;
        TlsOptiones  opt = tls_optiones_default();
        TlsResultus  tr;
                b32  bene;

    opt.vectura             = vectura;
    opt.ancora_via          = l->ancora;
    opt.timeout_ms          = V * M;
    opt.protocolla          = protocolla;
    opt.protocolla_numerus  = numerus;
    si (vexilla[ZEPHYRUM] == '\0'
        ? !servum_tls_incipere(&s, l->dir, l->fixa, "localhost",
              c->piscina)
        : !servum_tls_incipere_cum(&s, l->dir, l->fixa, "localhost",
              vexilla, c->piscina))
    {
        redde FALSUM;
    }
    tr   = tls_connectere_cum_optionibus("localhost", s.portus, &opt,
        c->piscina);
    bene = tr.successus;
    si (bene)
    {
        *vectura_facta  = tls_vectura(tr.connexio);
        *electum        = tls_protocollum(tr.connexio);
        tls_claudere(tr.connexio);
    }
    servum_tls_claudere(&s);
    redde bene;
}

interior vacuum
_sectio_tls13_vivum (
    CredoContextus* c)
{
    TlsVectura v;
        chorda e;

    CREDO_GRADUS("servus h2,http/1.1: h2");
    CREDO_VERUM(vivum(c, "-tls1_3 -alpn h2,http/1.1", TLS_VECTURA_TLS13,
        PROTOCOLLA, II, &v, &e));
    CREDO_AEQUALIS_I32((i32)v, (i32)TLS_VECTURA_TLS13);
    CREDO_CHORDA_AEQUALIS_LITERIS(e, "h2");
    CREDO_GRADUS("servus http/1.1 solum: http/1.1");
    CREDO_VERUM(vivum(c, "-tls1_3 -alpn http/1.1", TLS_VECTURA_TLS13,
        PROTOCOLLA, II, &v, &e));
    CREDO_CHORDA_AEQUALIS_LITERIS(e, "http/1.1");
    CREDO_GRADUS("servus sine ALPN: nihil electum");
    CREDO_VERUM(vivum(c, "", TLS_VECTURA_TLS13, PROTOCOLLA, II, &v,
        &e));
    CREDO_AEQUALIS_I32(e.mensura, ZEPHYRUM);
    CREDO_GRADUS("cliens sine indice: ut antea");
    CREDO_VERUM(vivum(c, "-tls1_3 -alpn h2", TLS_VECTURA_TLS13, NIHIL,
        ZEPHYRUM, &v, &e));
    CREDO_AEQUALIS_I32(e.mensura, ZEPHYRUM);
}

interior vacuum
_sectio_bearssl_vivum (
    CredoContextus* c)
{
    TlsVectura v;
        chorda e;

    CREDO_GRADUS("BearSSL TLS 1.2: h2");
    CREDO_VERUM(vivum(c, "-tls1_2 -alpn h2", TLS_VECTURA_BEARSSL,
        PROTOCOLLA, II, &v, &e));
    CREDO_AEQUALIS_I32((i32)v, (i32)TLS_VECTURA_BEARSSL);
    CREDO_CHORDA_AEQUALIS_LITERIS(e, "h2");
    CREDO_GRADUS("descensus TLS13 -> BearSSL: h2 manet");
    CREDO_VERUM(vivum(c, "-tls1_2 -alpn h2", TLS_VECTURA_TLS13,
        PROTOCOLLA, II, &v, &e));
    CREDO_AEQUALIS_I32((i32)v, (i32)TLS_VECTURA_BEARSSL);
    CREDO_CHORDA_AEQUALIS_LITERIS(e, "h2");
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "salutatio", _sectio_salutatio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "extensiones", _sectio_extensiones, NIHIL, NIHIL, NIHIL, NIHIL },
    { "tls13 vivum", _sectio_tls13_vivum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "bearssl vivum", _sectio_bearssl_vivum, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("tls_alpn", SECTIONES,
        _suitam_parare, _suitam_purgare);
}

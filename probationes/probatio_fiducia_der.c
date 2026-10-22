/* probatio_fiducia_der.c - partes certificati ex DER (fiducia-linux
 * T1) contra certificata probationis IN TEMPORE generata
 * (probationes/fixa/tls/generare.sh): series cum 0x00 initiali et XX
 * octetorum, clavis eadem in duobus certificatis (instar
 * transsignati), nomina alterna IP (IPv4, IPv6), folium sine SAN, DER
 * truncatum et malum (numquam extra legitur). Sine rete. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "filum.h"
#include "processus.h"
#include "fiducia_der.h"
#include <stdio.h>
#include <string.h>

nomen structura {
    character fixa[DXII];
} Locus;

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
    sprintf(l->fixa, "%s/certa", area);
    argumenta[ZEPHYRUM] = "probationes/fixa/tls/generare.sh";
    argumenta[I] = l->fixa;
    argumenta[II] = NIHIL;
    pr = processus_exsequi(argumenta, LX * M, c->piscina);
    CREDO_NECESSE(pr.successus && pr.codex_exitus == ZEPHYRUM);
    c->datum = l;
}

interior chorda
_der (
         CredoContextus* c,
    constans character* titulus)
{
     constans Locus* l = (constans Locus*)c->suita;
          character  via[MXXIV];   /* fixa (DXII) + titulus: gcc
                                   * -Wformat-overflow */
             chorda der;

    sprintf(via, "%s/%s.der", l->fixa, titulus);
    der = filum_legere_totum(via, c->piscina);
    CREDO_NECESSE(der.mensura > ZEPHYRUM);
    redde der;
}

interior b32
_partes (
         CredoContextus* c,
     constans character* titulus,
          FiduciaPartes* p)
{
                 chorda  der    = _der(c, titulus);
     constans character* causa  = NIHIL;

    redde fiducia_partes_legere(der.datum, der.mensura, p, &causa);
}

interior b32
_continet_literas (
          FiduciaSectio  s,
     constans character* literae)
{
    i32 k = (i32)strlen(literae);
    i32 i;

    per (i = ZEPHYRUM; i + k <= s.mensura; i++)
    {
        si (memcmp(s.octeti + i, literae, (size_t)k) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior b32
_aequales (
    FiduciaSectio a,
    FiduciaSectio b)
{
    redde a.mensura == b.mensura
        && memcmp(a.octeti, b.octeti, (size_t)a.mensura) == ZEPHYRUM;
}

/* emittens, series, subiectum, clavis; clavis eadem in geminus */
interior vacuum
_sectio_partes (
    CredoContextus* c)
{
    FiduciaPartes p;
    FiduciaPartes q;

    CREDO_NECESSE(_partes(c, "localhost", &p));
    CREDO_VERUM(_continet_literas(p.subiectum, "localhost"));
    CREDO_VERUM(_continet_literas(p.emittens, "rhubarb probatio CA"));
    CREDO_AEQUALIS_I32((i32)p.subiectum.octeti[ZEPHYRUM], 0x30);
    CREDO_AEQUALIS_I32((i32)p.clavis_publica.octeti[ZEPHYRUM], 0x30);
    CREDO_VERUM(p.extensiones.mensura > ZEPHYRUM);
    CREDO_VERUM(p.habet_alterna);

    CREDO_NECESSE(_partes(c, "serie_alta", &q));
    CREDO_AEQUALIS_I32(q.series.mensura, III);
    CREDO_AEQUALIS_I32((i32)q.series.octeti[ZEPHYRUM], 0x00);
    CREDO_AEQUALIS_I32((i32)q.series.octeti[I], 0x80);
    CREDO_AEQUALIS_I32((i32)q.series.octeti[II], 0x01);

    CREDO_NECESSE(_partes(c, "serie_longa", &q));
    CREDO_AEQUALIS_I32(q.series.mensura, XX);
    CREDO_AEQUALIS_I32((i32)q.series.octeti[ZEPHYRUM], 0x7F);
    CREDO_AEQUALIS_I32((i32)q.series.octeti[XIX], 0x13);

    /* geminus: clavis localhost.key iterum, series alia */
    CREDO_NECESSE(_partes(c, "geminus", &q));
    CREDO_VERUM(_aequales(p.clavis_publica, q.clavis_publica));
    CREDO_FALSUM(_aequales(p.series, q.series));
    CREDO_VERUM(_aequales(p.emittens, q.emittens));
}

interior vacuum
_sectio_alterna_ip (
    CredoContextus* c)
{
    hic_manens constans i8 quattuor[IV] = { 127, 0, 0, 1 };
    hic_manens constans i8 sex[XVI] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 1 };
    hic_manens constans i8 alia[IV] = { 127, 0, 0, 2 };
             FiduciaPartes p;

    CREDO_NECESSE(_partes(c, "localhost", &p));
    CREDO_VERUM(fiducia_alterna_ip_continet(&p, quattuor, IV));
    CREDO_FALSUM(fiducia_alterna_ip_continet(&p, alia, IV));
    CREDO_FALSUM(fiducia_alterna_ip_continet(&p, sex, XVI));

    CREDO_NECESSE(_partes(c, "ip_solum", &p));
    CREDO_VERUM(fiducia_alterna_ip_continet(&p, quattuor, IV));
    CREDO_VERUM(fiducia_alterna_ip_continet(&p, sex, XVI));

    CREDO_NECESSE(_partes(c, "alienus", &p));
    CREDO_VERUM(p.habet_alterna);
    CREDO_FALSUM(fiducia_alterna_ip_continet(&p, quattuor, IV));

    CREDO_NECESSE(_partes(c, "sine_san", &p));
    CREDO_FALSUM(p.habet_alterna);
    CREDO_FALSUM(fiducia_alterna_ip_continet(&p, quattuor, IV));
}

/* DER truncatum (omnis praefixus), longitudo ultra finem, longitudo
 * indefinita, octeti nulli: FALSUM cum causa, numquam ruina */
interior vacuum
_sectio_der_malum (
    CredoContextus* c)
{
                 chorda  der = _der(c, "localhost");
                     i8* malum;
          FiduciaPartes  p;
     constans character* causa;
                    i32  n;
                    i32  falsa = ZEPHYRUM;

    malum = (i8*)piscina_allocare(c->piscina,
        (memoriae_index)der.mensura);
    per (n = ZEPHYRUM; n < der.mensura; n++)
    {
        causa = NIHIL;
        memcpy(malum, der.datum, (size_t)n);
        si (   !fiducia_partes_legere(malum, n, &p, &causa)
            && causa != NIHIL)
        {
            falsa++;
        }
    }
    CREDO_AEQUALIS_I32(falsa, der.mensura);

    /* longitudo exterior ultra finem: 30 82 hh ll -> 30 82 FF FF */
    memcpy(malum, der.datum, (size_t)der.mensura);
    CREDO_NECESSE((i32)malum[I] == 0x82);
    malum[II]   = 0xFF;
    malum[III]  = 0xFF;
    causa       = NIHIL;
    CREDO_FALSUM(fiducia_partes_legere(malum, der.mensura, &p, &causa));
    CREDO_NON_NIHIL(causa);

    /* longitudo indefinita (0x80) */
    malum[I]  = 0x80;
    causa     = NIHIL;
    CREDO_FALSUM(fiducia_partes_legere(malum, der.mensura, &p, &causa));
    CREDO_NON_NIHIL(causa);

    causa = NIHIL;
    CREDO_FALSUM(fiducia_partes_legere(NIHIL, ZEPHYRUM, &p, &causa));
    CREDO_NON_NIHIL(causa);

    /* longitudo INTERIOR ultra parentem: tbsCertificate C octetos ultra
     * finem certificati promittit, exterior integer manet. Post finem
     * zephyri (00 00 = TLV vacuum) - sine limite parentis lectio in eos
     * procederet et VERUM redderet */
    {
        i8* amplum = (i8*)piscina_allocare(c->piscina,
            (memoriae_index)(der.mensura + CCLVI));
        i32 t;

        memset(amplum, ZEPHYRUM, (size_t)(der.mensura + CCLVI));
        memcpy(amplum, der.datum, (size_t)der.mensura);
        CREDO_NECESSE((i32)amplum[IV] == 0x30
            && (i32)amplum[V] == 0x82);
        t            = ((i32)amplum[VI] << VIII | (i32)amplum[VII]) + C;
        amplum[VI]   = (i8)(t >> VIII);
        amplum[VII]  = (i8)(t & 0xFF);
        causa        = NIHIL;
        CREDO_FALSUM(fiducia_partes_legere(amplum, der.mensura, &p,
            &causa));
        CREDO_NON_NIHIL(causa);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "partes", _sectio_partes, NIHIL, NIHIL, NIHIL, NIHIL },
    { "san_ip", _sectio_alterna_ip, NIHIL, NIHIL, NIHIL, NIHIL },
    { "der_malum", _sectio_der_malum, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("fiducia_der", SECTIONES,
        _suitam_parare, NIHIL);
}

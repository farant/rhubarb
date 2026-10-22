/* probatio_fiducia.c - fiducia_iudicare / fiducia_folium_describere
 * contra certificata probationis IN TEMPORE generata
 * (probationes/fixa/tls/generare.sh): ancora sola (folia EC et RSA,
 * nomen et IP), hospes alienus, expiratus, CA ignota, repositorium
 * systematis (CA probationis ignorat), non-DER, ancora absens;
 * summarium folii. Sine rete. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "filum.h"
#include "processus.h"
#include "fiducia.h"
#include "base64.h"
#include "chorda_aedificator.h"
#include <stdio.h>
#include <string.h>

nomen structura {
    character fixa[DXII];
    character ancora[MXXIV];    /* fixa + "/ca.der" (gcc
                                 * -Wformat-overflow) */
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
    sprintf(l->ancora, "%s/ca.der", l->fixa);
    argumenta[ZEPHYRUM] = "probationes/fixa/tls/generare.sh";
    argumenta[I] = l->fixa;
    argumenta[II] = NIHIL;
    pr = processus_exsequi(argumenta, XXX * M, c->piscina);
    CREDO_NECESSE(pr.successus && pr.codex_exitus == ZEPHYRUM);
    c->datum = l;
}

/* <titulus>.der -> FiduciaCertificatum in piscina sectionis */
interior FiduciaCertificatum
_legere (
         CredoContextus* c,
     constans character* titulus)
{
         constans Locus* l = (constans Locus*)c->suita;
    FiduciaCertificatum  fc;
              character  via[MXXIV];   /* fixa (DXII) + titulus: gcc
                                        * -Wformat-overflow */
                 chorda der;

    sprintf(via, "%s/%s.der", l->fixa, titulus);
    der         = filum_legere_totum(via, c->piscina);
    fc.octeti   = der.datum;
    fc.mensura  = der.mensura;
    redde fc;
}

/* iudicium unius folii (ancora probationis aut repositorium) */
interior b32
_iudicare (
         CredoContextus* c,
    constans character* titulus,
    constans character* hospes,
                   b32  cum_ancora,
                chorda* causa)
{
         constans Locus* l   = (constans Locus*)c->suita;
    FiduciaCertificatum  fc  = _legere(c, titulus);

    CREDO_NECESSE(fc.mensura > ZEPHYRUM);
    redde fiducia_iudicare(&fc, I, hospes,
        cum_ancora ? l->ancora : NIHIL,
        causa, c->piscina);
}

interior b32
_continet (
                 chorda  c,
     constans character* fragmentum)
{
    i32 k = (i32)strlen(fragmentum);
    i32 i;

    per (i = ZEPHYRUM; i + k <= c.mensura; i++)
    {
        si (memcmp(c.datum + i, fragmentum, k) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
_sectio_ancora (
    CredoContextus* c)
{
    chorda causa;

    causa.datum    = NIHIL;
    causa.mensura  = ZEPHYRUM;
    CREDO_VERUM(_iudicare(c, "localhost", "localhost", VERUM, &causa));
    CREDO_VERUM(_iudicare(c, "localhost_rsa", "localhost", VERUM,
        &causa));
    CREDO_VERUM(_iudicare(c, "localhost", "127.0.0.1", VERUM, &causa));
}

interior vacuum
_reiectum (
         CredoContextus* c,
    constans character* titulus,
                   b32  cum_ancora)
{
    chorda causa;

    causa.datum    = NIHIL;
    causa.mensura  = ZEPHYRUM;
    CREDO_FALSUM(_iudicare(c, titulus, "localhost", cum_ancora,
        &causa));
    CREDO_VERUM(_continet(causa, "certificatum reiectum"));
}

interior vacuum
_sectio_alienus (
    CredoContextus* c)
{
    _reiectum(c, "alienus", VERUM);
}

interior vacuum
_sectio_expiratus (
    CredoContextus* c)
{
    _reiectum(c, "expiratus", VERUM);
}

interior vacuum
_sectio_ignotus (
    CredoContextus* c)
{
    _reiectum(c, "ignotus", VERUM);
}

/* sine ancora: repositorium systematis CA probationis ignorat */
interior vacuum
_sectio_sine_ancora (
    CredoContextus* c)
{
    _reiectum(c, "localhost", FALSUM);
}

interior vacuum
_sectio_non_der (
    CredoContextus* c)
{
    FiduciaCertificatum fc;
                 chorda causa;

    fc.octeti      = (constans i8*)"non certificatum";
    fc.mensura     = (i32)strlen("non certificatum");
    causa.datum    = NIHIL;
    causa.mensura  = ZEPHYRUM;
    CREDO_FALSUM(fiducia_iudicare(&fc, I, "localhost", NIHIL, &causa,
        c->piscina));
    CREDO_VERUM(_continet(causa, "DER"));
}

interior vacuum
_sectio_ancora_absens (
    CredoContextus* c)
{
    FiduciaCertificatum fc = _legere(c, "localhost");
                 chorda causa;

    causa.datum    = NIHIL;
    causa.mensura  = ZEPHYRUM;
    CREDO_FALSUM(fiducia_iudicare(&fc, I, "localhost",
        "/nusquam/ancora.der", &causa, c->piscina));
    CREDO_VERUM(_continet(causa, "ancora legi non potest"));
}

/* folium: subiectum CN, validitas CCCLXV dierum (± diem) */
interior vacuum
_sectio_folium (
    CredoContextus* c)
{
    FiduciaCertificatum fc = _legere(c, "localhost");
          FiduciaFolium ff;
                    s64 dies;

    CREDO_NECESSE(fiducia_folium_describere(&fc, &ff, c->piscina));
    CREDO_VERUM(ff.valida);
    CREDO_CHORDA_AEQUALIS_LITERIS(ff.subiectum, "localhost");
    dies = (ff.non_post - ff.non_ante) / ((s64)LXXXVI * M + CD);
    CREDO_VERUM(dies >= CCCLXIV && dies <= CCCLXVI);
}

/* diffisa vera per fiducia_iudicare IPSAM (omnis platforma): intermedium
 * eMudhra transsignatum (AAA Certificate Services) causam "clavis
 * diffisa" fert, non iudicium systematis - indicium PRIMUM consulitur */
interior vacuum
_sectio_diffisa_vera (
    CredoContextus* c)
{
    FiduciaCertificatum fc;
                 chorda der = filum_legere_totum(
                     "probationes/fixa/fiducia/emudhra/em_dv_g3b_aaa.der",
                     c->piscina);
                 chorda causa;

    CREDO_NECESSE(der.mensura > ZEPHYRUM);
    fc.octeti      = der.datum;
    fc.mensura     = der.mensura;
    causa.datum    = NIHIL;
    causa.mensura  = ZEPHYRUM;
    CREDO_FALSUM(fiducia_iudicare(&fc, I, "exemplum.invalid", NIHIL,
        &causa, c->piscina));
    CREDO_VERUM(_continet(causa, "clavis diffisa"));
    CREDO_VERUM(_continet(causa, "2083382"));
}

/* causa refutationis continet fragmentum (Linux: causae domus; macOS:
 * SecTrust suas dat - ibi solum refutatio) */
interior vacuum
_reiectum_causa (
         CredoContextus* c,
    constans character* titulus,
    constans character* hospes,
    constans character* fragmentum)
{
    chorda causa;

    causa.datum    = NIHIL;
    causa.mensura  = ZEPHYRUM;
    CREDO_FALSUM(_iudicare(c, titulus, hospes, VERUM, &causa));
    CREDO_VERUM(_continet(causa, "certificatum reiectum"));
#ifndef __APPLE__
    CREDO_VERUM(_continet(causa, fragmentum));
#else
    (vacuum)fragmentum;
#endif
}

/* SHA-1 non registratum; RSA-1024 debilis; folium sine SAN (BearSSL CN
 * adhiberet - politica domus SAN solum) */
interior vacuum
_sectio_algorithmi (
    CredoContextus* c)
{
    _reiectum_causa(c, "sha1", "localhost", "algorithmus");
    _reiectum_causa(c, "rsa_parva", "localhost", "clavis debilis");
    _reiectum_causa(c, "sine_san", "localhost", "SAN");
}

/* causae nominatae casuum veterum (Linux) */
interior vacuum
_sectio_causae (
    CredoContextus* c)
{
    _reiectum_causa(c, "expiratus", "localhost", "expiratum");
    _reiectum_causa(c, "alienus", "localhost", "nomen hospitis");
    _reiectum_causa(c, "ignotus", "localhost", "non fide dignus");
}

/* hospes IP: iPAddress SAN (IPv4, IPv6); folium DNS solum refutatur */
interior vacuum
_sectio_hospes_ip (
    CredoContextus* c)
{
    chorda causa;

    causa.datum    = NIHIL;
    causa.mensura  = ZEPHYRUM;
    CREDO_VERUM(_iudicare(c, "ip_solum", "127.0.0.1", VERUM, &causa));
    CREDO_VERUM(_iudicare(c, "ip_solum", "::1", VERUM, &causa));
    _reiectum_causa(c, "alienus", "127.0.0.1", "nomen hospitis");
    _reiectum_causa(c, "localhost", "::1", "nomen hospitis");
}

#ifndef __APPLE__
/* fasciculus PEM in area scriptus: titulus.der fixorum, aut DER datum */
interior vacuum
_obiectum_addere (
     ChordaAedificator* a,
    constans character* nomen_obiecti,
                chorda  der,
                   b32  crlf,
               Piscina* p)
{
    chorda b64 = base64_codificare(der.datum, der.mensura, p);
       i32 i;

    chorda_aedificator_appendere_literis(a, "-----BEGIN ");
    chorda_aedificator_appendere_literis(a, nomen_obiecti);
    chorda_aedificator_appendere_literis(a, crlf ? "-----\r\n"
                                                : "-----\n");
    per (i = ZEPHYRUM; i < b64.mensura; i += LXIV)
    {
        chorda_aedificator_appendere_chorda(a, chorda_sectio(b64, i,
            i + LXIV < b64.mensura ? i + LXIV : b64.mensura));
        chorda_aedificator_appendere_literis(a, crlf ? "\r\n" : "\n");
    }
    chorda_aedificator_appendere_literis(a, "-----END ");
    chorda_aedificator_appendere_literis(a, nomen_obiecti);
    chorda_aedificator_appendere_literis(a, crlf ? "-----\r\n"
                                                : "-----\n");
}

#endif

/* fasciculus systematis: inventus, ancorae multae; macOS solum POSIX */
interior vacuum
_sectio_fasciculus (
    CredoContextus* c)
{
       i32 ancorae = ZEPHYRUM;
       i32 omissae = ZEPHYRUM;
    chorda via;
    chorda causa;

    causa.datum    = NIHIL;
    causa.mensura  = ZEPHYRUM;
#ifndef __APPLE__
    CREDO_NECESSE(fiducia_fasciculum_describere(NIHIL, &ancorae,
        &omissae, &via, &causa, c->piscina));
    CREDO_VERUM(ancorae > L);
    CREDO_VERUM(via.mensura > ZEPHYRUM);
    imprimere("  fasciculus %.*s: ancorae %u, omissae %u\n",
        (integer)via.mensura, (constans character*)via.datum,
        (insignatus integer)ancorae, (insignatus integer)omissae);
#else
    CREDO_FALSUM(fiducia_fasciculum_describere(NIHIL, &ancorae,
        &omissae,
        &via, &causa, c->piscina));
    CREDO_VERUM(_continet(causa, "solum POSIX"));
#endif
}

#ifndef __APPLE__
/* ancora diffisa in fasciculo omittitur (servi radicem saepe non
 * mittunt: diffisa in catena numquam appareret) */
interior vacuum
_sectio_ancora_diffisa (
    CredoContextus* c)
{
       constans Locus* l = (constans Locus*)c->suita;
    ChordaAedificator* a = chorda_aedificator_creare(c->piscina, MXXIV);
             character via[MXXIV];
                   i32 ancorae = ZEPHYRUM;
                   i32 omissae = ZEPHYRUM;
                chorda via_usa;
                chorda causa;

    _obiectum_addere(a, "CERTIFICATE", filum_legere_totum(l->ancora,
        c->piscina), FALSUM, c->piscina);
    _obiectum_addere(a, "CERTIFICATE", filum_legere_totum(
        "probationes/fixa/fiducia/emudhra/em_dv_g3b_emsign.der",
        c->piscina), FALSUM, c->piscina);
    sprintf(via, "%s/diffisa.pem", l->fixa);
    CREDO_NECESSE(filum_scribere(via, chorda_aedificator_finire(a)));
    CREDO_NECESSE(fiducia_fasciculum_describere(via, &ancorae, &omissae,
        &via_usa, &causa, c->piscina));
    CREDO_AEQUALIS_I32(ancorae, I);
    CREDO_AEQUALIS_I32(omissae, I);
}

/* CRLF, obiectum TRUSTED CERTIFICATE, obiectum corruptum: praetereunt
 * et numerantur, ceterae manent */
interior vacuum
_sectio_fasciculus_mixtus (
    CredoContextus* c)
{
       constans Locus* l = (constans Locus*)c->suita;
    ChordaAedificator* a = chorda_aedificator_creare(c->piscina, MXXIV);
                chorda ca = filum_legere_totum(l->ancora, c->piscina);
             character via[MXXIV];
                   i32 ancorae = ZEPHYRUM;
                   i32 omissae = ZEPHYRUM;
                chorda via_usa;
                chorda causa;

    _obiectum_addere(a, "TRUSTED CERTIFICATE", ca, VERUM, c->piscina);
    chorda_aedificator_appendere_literis(a,
        "-----BEGIN CERTIFICATE-----\r\n"
        "!!!! non base64 !!!!\r\n-----END CERTIFICATE-----\r\n");
    _obiectum_addere(a, "CERTIFICATE", ca, VERUM, c->piscina);
    sprintf(via, "%s/mixtus.pem", l->fixa);
    CREDO_NECESSE(filum_scribere(via, chorda_aedificator_finire(a)));
    CREDO_NECESSE(fiducia_fasciculum_describere(via, &ancorae, &omissae,
        &via_usa, &causa, c->piscina));
    CREDO_AEQUALIS_I32(ancorae, I);
    CREDO_AEQUALIS_I32(omissae, II);
}
#endif

hic_manens constans CredoSectio SECTIONES[] = {
    { "ancora", _sectio_ancora, NIHIL, NIHIL, NIHIL, NIHIL },
    { "alienus", _sectio_alienus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "expiratus", _sectio_expiratus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ignotus", _sectio_ignotus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sine ancora", _sectio_sine_ancora, NIHIL, NIHIL, NIHIL, NIHIL },
    { "non der", _sectio_non_der, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ancora absens", _sectio_ancora_absens, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "folium", _sectio_folium, NIHIL, NIHIL, NIHIL, NIHIL },
    { "diffisa vera", _sectio_diffisa_vera, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "algorithmi", _sectio_algorithmi, NIHIL, NIHIL, NIHIL, NIHIL },
    { "causae", _sectio_causae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "hospes ip", _sectio_hospes_ip, NIHIL, NIHIL, NIHIL, NIHIL },
    { "fasciculus", _sectio_fasciculus, NIHIL, NIHIL, NIHIL, NIHIL },
#ifndef __APPLE__
    { "ancora diffisa", _sectio_ancora_diffisa, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "fasciculus mixtus", _sectio_fasciculus_mixtus, NIHIL, NIHIL,
        NIHIL, NIHIL },
#endif
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("fiducia", SECTIONES, _suitam_parare,
        NIHIL);
}

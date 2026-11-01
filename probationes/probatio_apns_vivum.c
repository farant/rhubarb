/* probatio_apns_vivum.c - apns_mittere contra servum Go (route
 * /3/device/, http2-plan T11): forma petitionis (apns-topic,
 * apns-push-type, authorization "bearer <tessera>" cum caput JWT
 * {"alg":"ES256","kid":"PROBA12345"}), authorization numquam
 * indicatum (tabula Go "(sensitive)"), BadDeviceToken transit, regula
 * iterandi (-modus iterare), sine tessera MissingProviderToken. Clavis
 * P-256 in probatione ex semine facta. Sine 'go': II (NIHIL CURSUM). */
#include "postulata_posix.h"
#include "../vendor/bearssl_amalgama.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "base64.h"
#include "filum.h"
#include "processus.h"
#include "hpack.h"
#include "tessera_es256.h"
#include "apns.h"
#include "servus_oraculi.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

nomen structura {
    character dir[DXII];
    character fixa[DXII];
    character ancora[MXXIV];
    character binarium[MXXIV];
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

    CREDO_NECESSE(l && area && strlen(area) < CD);
    strcpy(l->dir, area);
    sprintf(l->fixa, "%s/certa", area);
    sprintf(l->ancora, "%s/ca.der", l->fixa);
    sprintf(l->binarium, "%s/http2_servus", area);
    argumenta[ZEPHYRUM] = "probationes/fixa/tls/generare.sh";
    argumenta[I] = l->fixa;
    argumenta[II] = NIHIL;
    pr = processus_exsequi(argumenta, XXX * M, c->piscina);
    CREDO_NECESSE(pr.successus && pr.codex_exitus == ZEPHYRUM);
    CREDO_NECESSE(servum_oraculi_construere(area, l->binarium,
        c->piscina));
    c->datum = l;
}

interior vacuum
_suitam_purgare (
    CredoContextus* c)
{
    (vacuum)c;
}

/* tessera APNs ex clavi P-256 semine facta */
interior chorda
tessera (
     Piscina* p)
{
    br_hmac_drbg_context  fors;
       br_ec_private_key  privata;
        br_ec_public_key  publica;
           unsigned char  capsa_privata[BR_EC_KBUF_PRIV_MAX_SIZE];
           unsigned char  capsa_publica[BR_EC_KBUF_PUB_MAX_SIZE];
      constans character* semen = "apns probatio semen";
                  size_t  n;
                      i8* der;
                  chorda  vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    br_hmac_drbg_init(&fors, &br_sha256_vtable, semen, strlen(semen));
    si (   br_ec_keygen(&fors.vtable, br_ec_get_default(), &privata,
            capsa_privata, BR_EC_secp256r1) == ZEPHYRUM
        || br_ec_compute_pub(br_ec_get_default(), &publica,
        capsa_publica,
               &privata) == ZEPHYRUM)
    {
        redde vacua;
    }
    n    = br_encode_ec_pkcs8_der(NIHIL, &privata, &publica);
    der  = (i8*)piscina_allocare(p, (memoriae_index)n);
    si (!der || br_encode_ec_pkcs8_der(der, &privata, &publica) != n)
    {
        redde vacua;
    }
    redde tessera_apns_facere(der, (i32)n,
        chorda_ex_literis("PROBA12345",
        p), chorda_ex_literis("TURMA12345", p), 1792800000, p);
}

interior ApnsConfiguratio
configuratio (
    CredoContextus* c,
               i32  portus)
{
      constans Locus* l = (constans Locus*)c->suita;
    ApnsConfiguratio  cfg;

    memset(&cfg, ZEPHYRUM, magnitudo(cfg));
    cfg.clavis_id  = chorda_ex_literis("PROBA12345", c->piscina);
    cfg.turma      = chorda_ex_literis("TURMA12345", c->piscina);
    cfg.thema = chorda_ex_literis("earth.golden.pictor",
        c->piscina);
    cfg.hospes = chorda_ex_literis("localhost", c->piscina);
    cfg.portus = portus;
    cfg.ancora = chorda_ex_literis(l->ancora, c->piscina);
    redde cfg;
}

interior chorda
caput_responsi (
    constans ApnsResponsum* r,
        constans character* titulus)
{
    chorda vacua;
       i32 i;

    per (i = ZEPHYRUM; r->capita && i < xar_numerus(r->capita); i++)
    {
        HpackCaput* h = (HpackCaput*)xar_obtinere(r->capita, i);

        si (chorda_aequalis_literis(h->titulus, titulus))
        {
            redde h->valor;
        }
    }
    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    redde vacua;
}

interior vacuum
_sectio_forma (
    CredoContextus* c)
{
      constans Locus* l = (constans Locus*)c->suita;
       ServusOraculi  s;
    ApnsConfiguratio  cfg;
       ApnsResponsum  r;
              chorda  t;
              chorda  auctoritas;
              chorda  caput_tesserae;
       Base64Fructus  caput;
              chorda  tabula;
           character  via[MXXIV];

    sprintf(via, "%s/forma.go.log", l->dir);
    t = tessera(c->piscina);
    CREDO_NECESSE(t.mensura > ZEPHYRUM);
    CREDO_NECESSE(servum_oraculi_incipere(&s, l->binarium, l->fixa, via,
        "",
        c->piscina));
    cfg = configuratio(c, s.portus);

    CREDO_GRADUS("BadDeviceToken transit; capita accepta");
    CREDO_VERUM(apns_mittere(&cfg, t, APNS_SIGNUM_NULLUM, &r,
        c->piscina));
    CREDO_VERUM(r.transita);
    CREDO_AEQUALIS_I32(r.status, CD);
    CREDO_CHORDA_AEQUALIS_LITERIS(r.ratio, "BadDeviceToken");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.apns_id,
        "00000000-0000-0000-0000-000000000001");
    CREDO_AEQUALIS_I32((i32)apns_iudicare(&r, VERUM),
        (i32)APNS_TRANSITUS);
    CREDO_CHORDA_AEQUALIS_LITERIS(caput_responsi(&r, "x-thema"),
        "earth.golden.pictor");
    CREDO_CHORDA_AEQUALIS_LITERIS(caput_responsi(&r, "x-genus"),
        "alert");

    CREDO_GRADUS("authorization: bearer + JWT cum kid");
    auctoritas = caput_responsi(&r, "x-auctoritas");
    CREDO_VERUM(auctoritas.mensura > VII
        && memcmp(auctoritas.datum, "bearer ", VII) == ZEPHYRUM);
    caput_tesserae.datum    = auctoritas.datum + VII;
    caput_tesserae.mensura  = ZEPHYRUM;
    dum (   caput_tesserae.mensura + VII < auctoritas.mensura
         && caput_tesserae.datum[caput_tesserae.mensura] != '.')
    {
        caput_tesserae.mensura++;
    }
    caput = base64url_decodificare(caput_tesserae, c->piscina);
    CREDO_VERUM(caput.datum && caput.mensura == (i32)strlen(
        "{\"alg\":\"ES256\",\"kid\":\"PROBA12345\"}")
        && memcmp(caput.datum,
        "{\"alg\":\"ES256\",\"kid\":\"PROBA12345\"}",
               (size_t)caput.mensura) == ZEPHYRUM);

    CREDO_GRADUS("authorization numquam indicatum (tabula Go)");
    servum_oraculi_claudere(&s);
    tabula = filum_legere_totum(via, c->piscina);
    CREDO_VERUM(chorda_continet(tabula, chorda_ex_literis(
        "\"authorization\" = \"bearer ", c->piscina)));
    CREDO_VERUM(chorda_continet(tabula, chorda_ex_literis("(sensitive)",
        c->piscina)));
}

interior vacuum
_sectio_iterare (
    CredoContextus* c)
{
      constans Locus* l = (constans Locus*)c->suita;
       ServusOraculi  s;
    ApnsConfiguratio  cfg;
       ApnsResponsum  r;
              chorda  t;
           character  via[MXXIV];
           character  signum[LXV];

    sprintf(via, "%s/iterare.go.log", l->dir);
    t = tessera(c->piscina);
    CREDO_NECESSE(t.mensura > ZEPHYRUM);
    CREDO_NECESSE(servum_oraculi_incipere(&s, l->binarium, l->fixa, via,
        "-modus iterare", c->piscina));
    cfg = configuratio(c, s.portus);
    CREDO_GRADUS("signum nullum: ratio alia -> iterandum");
    CREDO_VERUM(apns_mittere(&cfg, t, APNS_SIGNUM_NULLUM, &r,
        c->piscina));
    CREDO_CHORDA_AEQUALIS_LITERIS(r.ratio, "DeviceTokenNotForTopic");
    CREDO_AEQUALIS_I32((i32)apns_iudicare(&r, VERUM),
        (i32)APNS_ITERANDUM);
    CREDO_GRADUS("signum fortuitum: BadDeviceToken -> transit");
    apns_signum_fortuitum(signum, 1792800000);
    CREDO_VERUM(apns_mittere(&cfg, t, signum, &r, c->piscina));
    CREDO_CHORDA_AEQUALIS_LITERIS(r.ratio, "BadDeviceToken");
    CREDO_AEQUALIS_I32((i32)apns_iudicare(&r, FALSUM),
        (i32)APNS_TRANSITUS);
    servum_oraculi_claudere(&s);
}

interior vacuum
_sectio_sine_tessera (
    CredoContextus* c)
{
      constans Locus* l = (constans Locus*)c->suita;
       ServusOraculi  s;
    ApnsConfiguratio  cfg;
       ApnsResponsum  r;
           character  via[MXXIV];

    sprintf(via, "%s/sine.go.log", l->dir);
    CREDO_NECESSE(servum_oraculi_incipere(&s, l->binarium, l->fixa, via,
        "",
        c->piscina));
    cfg = configuratio(c, s.portus);
    CREDO_GRADUS("tessera vacua: MissingProviderToken -> defectus");
    CREDO_VERUM(apns_mittere(&cfg, chorda_ex_literis("", c->piscina),
        APNS_SIGNUM_NULLUM, &r, c->piscina));
    CREDO_AEQUALIS_I32(r.status, CDIII);
    CREDO_CHORDA_AEQUALIS_LITERIS(r.ratio, "MissingProviderToken");
    CREDO_AEQUALIS_I32((i32)apns_iudicare(&r, VERUM),
        (i32)APNS_DEFECTUS);
    servum_oraculi_claudere(&s);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "forma petitionis", _sectio_forma, NIHIL, NIHIL, NIHIL, NIHIL },
    { "iterare", _sectio_iterare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sine tessera", _sectio_sine_tessera, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    si (oraculi_via_quaerere() == NIHIL)
    {
        imprimere("NIHIL CURSUM: go deest (oraculum http2_servus)\n");
        redde II;
    }
    redde credo_suitam_currere_cum("apns_vivum", SECTIONES,
        _suitam_parare, _suitam_purgare);
}

/* probatio_tls13_machina.c - machina clientis TLS 1.3 (tls13-spec
 * II.2, tls13-plan T4): RFC 8448 §3 (1-RTT, RSA-PSS) et §7 (modus
 * compatibilitatis, CCS) octetim repetita cum fortibus RFC injectis et
 * fiducia ficta; recorda servi singulis octetis data; NewSessionTicket
 * omissum, data ordine tradita, close_notify; volatus servi a
 * probatione resignatus (subscriptio aut Finished mutata) -> errores
 * nominati cum alertis obsignatis. Oraculum: textus RFC immutatus. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "fiducia.h"
#include "tls13.h"
#include "tls13_recordum.h"
#include "rfc8448.h"
#include <string.h>

#define VIA_RFC "probationes/fixa/tls13/rfc8448.txt"
#define CAPSA   (VIII * MXXIV)

#define S_HS "derive secret \"tls13 s hs traffic\""
#define C_HS "derive secret \"tls13 c hs traffic\""
#define S_AP "derive secret \"tls13 s ap traffic\""

interior vacuum
_suitam_parare (CredoContextus* c)
{
    Rfc8448* r = (Rfc8448*)piscina_allocare(c->piscina,
        magnitudo(Rfc8448));

    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_NECESSE(rfc8448_legere(r, VIA_RFC, c->piscina));
    c->datum = r;
}

interior chorda
_octeti (CredoContextus* c, i32 sectio, character latus,
    constans character* actio, i32 ordo, constans character* campus)
{
    constans Rfc8448Campus* f = rfc8448_quaerere(
        (constans Rfc8448*)c->suita, sectio, latus, actio, ordo,
        campus);

    CREDO_NECESSE_NON_NIHIL(f);
    redde f->octeti;
}

/* fors fixa: chordae ordine vocationum (tls13.h ORDO FORTIS) */
nomen structura {
    chorda octeti[IV];
       i32 numerus;
       i32 vocationes;
       b32 inopinata;    /* mensura aliena aut vocatio superflua */
} ForsFixa;

interior vacuum
_fors_fixa (
    vacuum* datum,
        i8* exitus,
       i32  mensura)
{
    ForsFixa* f = (ForsFixa*)datum;

    si (   f->vocationes < f->numerus
        && f->octeti[f->vocationes].mensura == mensura)
    {
        memcpy(exitus, f->octeti[f->vocationes].datum, mensura);
    }
    alioquin
    {
        memset(exitus, 0xEE, mensura);
        f->inopinata = VERUM;
    }
    f->vocationes++;
}

/* fiducia ficta: catenam notat, responsum fixum reddit */
nomen structura {
                    b32 responsum;
                    i32 vocationes;
                    i32 numerus;
    FiduciaCertificatum folium;
                    b32 hospes_rectus;
} FiduciaFicta;

interior b32
_fiducia_ficta (
                          vacuum* datum,
    constans FiduciaCertificatum* catena,
                             i32  numerus,
              constans character* hospes,
                          chorda* causa,
                         Piscina* piscina)
{
    FiduciaFicta* f = (FiduciaFicta*)datum;

    f->vocationes++;
    f->numerus = numerus;
    si (numerus > ZEPHYRUM)
    {
        f->folium = catena[ZEPHYRUM];
    }
    f->hospes_rectus = hospes != NIHIL
        && strcmp(hospes, "server") == ZEPHYRUM;
    si (!f->responsum)
    {
        *causa = chorda_ex_literis("ficta", piscina);
    }
    redde f->responsum;
}

nomen structura {
          ForsFixa  fors;
      FiduciaFicta  fiducia;
      Tls13Machina* m;
} Scaena;

/* machina cum ClientHello sectionis fixo et clave privata RFC */
interior vacuum
_scaenam_parare (CredoContextus* c, Scaena* s, i32 sectio)
{
    Tls13Optiones o;
           chorda ch = _octeti(c, sectio, 'c',
               "construct a ClientHello", ZEPHYRUM, "ClientHello");

    memset(s, ZEPHYRUM, magnitudo(*s));
    s->fors.octeti[ZEPHYRUM] = _octeti(c, sectio, 'c',
        "create an ephemeral x25519 key pair", ZEPHYRUM, "private key");
    s->fors.numerus       = I;
    s->fiducia.responsum  = VERUM;
    memset(&o, ZEPHYRUM, magnitudo(o));
    o.hospes = "server";
    o.verificare = VERUM;
    o.fiducia = _fiducia_ficta;
    o.fiducia_datum = &s->fiducia;
    o.fors = _fors_fixa;
    o.fors_datum = &s->fors;
    o.salutationes_fixae[ZEPHYRUM] = ch.datum;
    o.salutationes_fixae_n[ZEPHYRUM] = ch.mensura;
    s->m = tls13_machina_creare(&o, c->piscina);
    CREDO_NECESSE_NON_NIHIL(s->m);
}

/* octetos in introitum, omnes simul aut singulos; reddit numerum
 * acceptum */
interior i32
_ingerere (
    Tls13Machina* m,
     constans i8* octeti,
             i32  n,
             b32  per_octetum)
{
    i32 k = ZEPHYRUM;

    dum (k < n)
    {
        i32  capacitas  = ZEPHYRUM;
         i8* in         = tls13_recorda_introitus(m, &capacitas);
        i32  q;

        si (in == NIHIL || capacitas == ZEPHYRUM)
        {
            frange;
        }
        q = per_octetum ? I : n - k;
        si (q > capacitas)
        {
            q = capacitas;
        }
        memcpy(in, octeti + k, q);
        tls13_recorda_introitus_ack(m, q);
        k += q;
    }
    redde k;
}

/* omnia exitus in capsa; reddit numerum */
interior i32
_exhaurire (
    Tls13Machina* m,
              i8* capsa,
             i32  capacitas)
{
    i32 summa = ZEPHYRUM;

    dum (VERUM)
    {
        i32  n   = ZEPHYRUM;
         i8* ex  = tls13_recorda_exitus(m, &n);

        si (ex == NIHIL || n == ZEPHYRUM || summa + n > capacitas)
        {
            frange;
        }
        memcpy(capsa + summa, ex, n);
        tls13_recorda_exitus_ack(m, n);
        summa += n;
    }
    redde summa;
}

interior b32
_aequales (
    constans i8* a,
            i32  n,
         chorda  b)
{
    redde n == b.mensura && memcmp(a, b.datum, n) == ZEPHYRUM;
}

/* ClientHello exhaustum et recorda servi data; reddit exitum post
 * volatum servi */
interior i32
_pactio (CredoContextus* c, Scaena* s, i32 sectio, chorda volatus,
    b32 per_octetum, i8* exitus)
{
        i8 capsa[CAPSA];
    chorda sh = _octeti(c, sectio, 's', "send handshake record",
        ZEPHYRUM, "complete record");
    i32 n = _exhaurire(s->m, capsa, CAPSA);

    CREDO_NECESSE(_aequales(capsa, n, _octeti(c, sectio, 'c',
        "send handshake record", ZEPHYRUM, "complete record")));
    CREDO_AEQUALIS_I32(_ingerere(s->m, sh.datum, sh.mensura,
        per_octetum), sh.mensura);
    si (sectio == VII)
    {
        chorda ccs = _octeti(c, VII, 's',
            "send change_cipher_spec record", ZEPHYRUM,
            "complete record");

        CREDO_AEQUALIS_I32(_ingerere(s->m, ccs.datum, ccs.mensura,
            per_octetum), ccs.mensura);
    }
    CREDO_AEQUALIS_I32(_ingerere(s->m, volatus.datum, volatus.mensura,
        per_octetum), volatus.mensura);
    redde _exhaurire(s->m, exitus, CAPSA);
}

interior chorda
_volatus (
    CredoContextus* c,
               i32  sectio)
{
    redde _octeti(c, sectio, 's', "send handshake record", I,
        "complete record");
}

interior vacuum
_sectio_s3_salutatio (CredoContextus* c)
{
    Scaena s;
        i8 capsa[CAPSA];
       i32 n;
       i32 st;

    _scaenam_parare(c, &s, III);
    st = tls13_machina_status(s.m);
    CREDO_VERUM((st & TLS13_RECORDA_MITTENDA) != ZEPHYRUM);
    CREDO_VERUM((st & TLS13_DATA_MITTI_POSSUNT) == ZEPHYRUM);
    n = _exhaurire(s.m, capsa, CAPSA);
    CREDO_VERUM(_aequales(capsa, n, _octeti(c, III, 'c',
        "send handshake record", ZEPHYRUM, "complete record")));
    st = tls13_machina_status(s.m);
    CREDO_VERUM((st & TLS13_RECORDA_MITTENDA) == ZEPHYRUM);
    CREDO_VERUM((st & TLS13_RECORDA_EXSPECTAT) != ZEPHYRUM);
    /* salutatio fixa: sola clavis x25519 a forte */
    CREDO_AEQUALIS_I32(s.fors.vocationes, I);
    CREDO_FALSUM(s.fors.inopinata);
    CREDO_AEQUALIS_I32(tls13_machina_versio(s.m), ZEPHYRUM);
}

interior vacuum
_pactionem_verificare (CredoContextus* c, Scaena* s, i32 sectio,
    b32 per_octetum)
{
                         i8 capsa[CAPSA];
                        i32 n = _pactio(c, s, sectio,
                            _volatus(c, sectio), per_octetum, capsa);
                     chorda cert = _octeti(c, sectio, 's',
                         "construct a Certificate", ZEPHYRUM,
                         "Certificate");
                     chorda finis = _octeti(c, sectio, 'c',
                         "send handshake record", I, "complete record");
                             i32  numerus = ZEPHYRUM;
    constans FiduciaCertificatum* catena;
                             i32  st;

    CREDO_AEQUALIS_I32(tls13_machina_error(s->m).codex,
        TLS13_ERROR_NULLUS);
    /* fiducia semel, catena unius certificati RFC, hospes "server" */
    CREDO_AEQUALIS_I32(s->fiducia.vocationes, I);
    CREDO_AEQUALIS_I32(s->fiducia.numerus, I);
    CREDO_VERUM(s->fiducia.hospes_rectus);
    /* Certificate: caput IV, contextus I, lista III, longitudo III */
    CREDO_AEQUALIS_I32(s->fiducia.folium.mensura,
        cert.mensura - IV - I - III - III - II);
    CREDO_VERUM(memcmp(s->fiducia.folium.octeti,
        cert.datum + XI, s->fiducia.folium.mensura) == ZEPHYRUM);
    catena = tls13_machina_catena(s->m, &numerus);
    CREDO_NON_NIHIL(catena);
    CREDO_AEQUALIS_I32(numerus, I);
    /* volatus clientis: (CCS si sessio non vacua) + Finished */
    si (sectio == VII)
    {
        chorda ccs = _octeti(c, VII, 'c',
            "send change_cipher_spec record", ZEPHYRUM,
            "complete record");

        CREDO_VERUM(n == ccs.mensura + finis.mensura);
        CREDO_VERUM(n >= ccs.mensura
            && _aequales(capsa, ccs.mensura, ccs));
        CREDO_VERUM(n >= ccs.mensura && _aequales(capsa + ccs.mensura,
            n - ccs.mensura, finis));
    }
    alioquin
    {
        CREDO_VERUM(_aequales(capsa, n, finis));
    }
    st = tls13_machina_status(s->m);
    CREDO_VERUM((st & TLS13_DATA_MITTI_POSSUNT) != ZEPHYRUM);
    CREDO_VERUM((st & TLS13_RECORDA_EXSPECTAT) != ZEPHYRUM);
    CREDO_AEQUALIS_I32(tls13_machina_versio(s->m), 0x0304);
}

interior vacuum
_sectio_s3_pactio (
    CredoContextus* c)
{
    Scaena s;

    _scaenam_parare(c, &s, III);
    _pactionem_verificare(c, &s, III, FALSUM);
}

/* Review focus 1: recorda servi singulis octetis */
interior vacuum
_sectio_s3_per_octetum (
    CredoContextus* c)
{
    Scaena s;

    _scaenam_parare(c, &s, III);
    _pactionem_verificare(c, &s, III, VERUM);
}

/* post pactionem: data nostra obsignata = RFC; NewSessionTicket
 * omissum, data servi tradita (Review focus 4) */
interior vacuum
_data_permutare (CredoContextus* c, Scaena* s)
{
        i8 capsa[CAPSA];
    chorda nostra = _octeti(c, III, 'c', "send application_data record",
        ZEPHYRUM, "payload");
    chorda servi = _octeti(c, III, 's', "send application_data record",
        ZEPHYRUM, "payload");
    chorda nst = _octeti(c, III, 's', "send handshake record", II,
        "complete record");
    chorda recordum_servi = _octeti(c, III, 's',
        "send application_data record",
        ZEPHYRUM, "complete record");
        i8* d;
       i32  n = ZEPHYRUM;

    d = tls13_data_exitus(s->m, &n);
    CREDO_NECESSE_NON_NIHIL(d);
    CREDO_NECESSE(n >= nostra.mensura);
    memcpy(d, nostra.datum, nostra.mensura);
    tls13_data_exitus_ack(s->m, nostra.mensura);
    tls13_data_effundere(s->m);
    n = _exhaurire(s->m, capsa, CAPSA);
    CREDO_VERUM(_aequales(capsa, n, _octeti(c, III, 'c',
        "send application_data record", ZEPHYRUM, "complete record")));
    CREDO_AEQUALIS_I32(_ingerere(s->m, nst.datum, nst.mensura,
        FALSUM), nst.mensura);
    CREDO_VERUM((tls13_machina_status(s->m) & TLS13_DATA_ADSUNT)
        == ZEPHYRUM);
    CREDO_AEQUALIS_I32(_ingerere(s->m, recordum_servi.datum,
        recordum_servi.mensura,
        FALSUM), recordum_servi.mensura);
    CREDO_VERUM((tls13_machina_status(s->m) & TLS13_DATA_ADSUNT)
        != ZEPHYRUM);
    n = ZEPHYRUM;
    d = tls13_data_introitus(s->m, &n);
    CREDO_NECESSE_NON_NIHIL(d);
    CREDO_VERUM(_aequales(d, n, servi));
    tls13_data_introitus_ack(s->m, n);
    CREDO_VERUM((tls13_machina_status(s->m) & TLS13_DATA_ADSUNT)
        == ZEPHYRUM);
    CREDO_AEQUALIS_I32(tls13_machina_error(s->m).codex,
        TLS13_ERROR_NULLUS);
}

interior vacuum
_sectio_s3_data (
    CredoContextus* c)
{
    Scaena s;

    _scaenam_parare(c, &s, III);
    _pactionem_verificare(c, &s, III, FALSUM);
    _data_permutare(c, &s);
}

/* NewSessionTicket et duo recorda datorum in UNO introitu: secundum
 * non aperitur dum primum non lectum (capsa una, ordo servatus) */
interior vacuum
_sectio_data_coniuncta (CredoContextus* c)
{
             Scaena s;
             chorda s_ap = _octeti(c, III, 's', S_AP, ZEPHYRUM,
                 "expanded");
             chorda nst = _octeti(c, III, 's', "send handshake record",
                 II, "payload");
             chorda data = _octeti(c, III, 's',
                 "send application_data record", ZEPHYRUM, "payload");
    Tls13Protectio* pr = tls13_protectio_creare(
        TLS13_AES_128_GCM_SHA256, s_ap.datum, c->piscina);
    hic_manens constans i8 SECUNDA[VII] = {
        's', 'e', 'c', 'u', 'n', 'd', 'a'
    };
                 i8  flumen[CAPSA];
                i32  n = ZEPHYRUM;
                i32  k;
                 i8* d;

    CREDO_NECESSE_NON_NIHIL(pr);
    _scaenam_parare(c, &s, III);
    _pactionem_verificare(c, &s, III, FALSUM);
    k = tls13_obsignare(pr, TLS13_CONTENTUM_PACTIO, nst.datum,
        nst.mensura, flumen, CAPSA);
    CREDO_NECESSE(_aequales(flumen, k, _octeti(c, III, 's',
        "send handshake record", II, "complete record")));
    n += k;
    k = tls13_obsignare(pr, TLS13_CONTENTUM_DATA, data.datum,
        data.mensura, flumen + n, CAPSA - n);
    CREDO_NECESSE(_aequales(flumen + n, k, _octeti(c, III, 's',
        "send application_data record", ZEPHYRUM, "complete record")));
    n += k;
    k = tls13_obsignare(pr, TLS13_CONTENTUM_DATA, SECUNDA, VII,
        flumen + n, CAPSA - n);
    CREDO_NECESSE(k > ZEPHYRUM);
    n += k;
    CREDO_AEQUALIS_I32(_ingerere(s.m, flumen, n, FALSUM), n);
    k = ZEPHYRUM;
    d = tls13_data_introitus(s.m, &k);
    CREDO_NECESSE_NON_NIHIL(d);
    CREDO_VERUM(_aequales(d, k, data));
    tls13_data_introitus_ack(s.m, k);
    k = ZEPHYRUM;
    d = tls13_data_introitus(s.m, &k);
    CREDO_NECESSE_NON_NIHIL(d);
    CREDO_AEQUALIS_I32(k, VII);
    CREDO_VERUM(memcmp(d, SECUNDA, VII) == ZEPHYRUM);
    tls13_data_introitus_ack(s.m, k);
    CREDO_VERUM((tls13_machina_status(s.m) & TLS13_DATA_ADSUNT)
        == ZEPHYRUM);
    CREDO_AEQUALIS_I32(tls13_machina_error(s.m).codex,
        TLS13_ERROR_NULLUS);
}

/* close_notify servi -> CLAUSA; nostrum = RFC (c_ap sequentia 1) */
interior vacuum
_sectio_s3_clausura (CredoContextus* c)
{
     Scaena s;
         i8 capsa[CAPSA];
     chorda alerta = _octeti(c, III, 's', "send alert record", ZEPHYRUM,
         "complete record");
        i32 n;

    _scaenam_parare(c, &s, III);
    _pactionem_verificare(c, &s, III, FALSUM);
    _data_permutare(c, &s);
    CREDO_AEQUALIS_I32(_ingerere(s.m, alerta.datum, alerta.mensura,
        FALSUM), alerta.mensura);
    CREDO_VERUM((tls13_machina_status(s.m) & TLS13_CLAUSA) != ZEPHYRUM);
    CREDO_AEQUALIS_I32(tls13_machina_error(s.m).codex,
        TLS13_ERROR_NULLUS);
    tls13_machina_claudere(s.m);
    n = _exhaurire(s.m, capsa, CAPSA);
    CREDO_VERUM(_aequales(capsa, n, _octeti(c, III, 'c',
        "send alert record", ZEPHYRUM, "complete record")));
}

/* §7: sessio non vacua -> CCS servi omissum, nostrum ante Finished */
interior vacuum
_sectio_s7_ccs (
    CredoContextus* c)
{
    Scaena s;

    _scaenam_parare(c, &s, VII);
    _pactionem_verificare(c, &s, VII, FALSUM);
}

/* volatus §3 a probatione resignatus sub s hs traffic (sequentia 0):
 * octetus 'mutare' nuntii 'typus' (ab initio corporis) flectitur; -I =
 * nihil mutatum (tunc = RFC, quod probatio ipsa verificat) */
interior chorda
_volatum_resignare (CredoContextus* c, i32 typus, s32 mutare)
{
          chorda planum = _octeti(c, III, 's', "send handshake record",
              I,
              "payload");
          chorda s_hs = _octeti(c, III, 's', S_HS, ZEPHYRUM,
              "expanded");
              i8* p = (i8*)piscina_allocare(c->piscina,
                  (memoriae_index)planum.mensura);
                i8* ex = (i8*)piscina_allocare(c->piscina, CAPSA);
    Tls13Protectio* pr = tls13_protectio_creare(
        TLS13_AES_128_GCM_SHA256, s_hs.datum, c->piscina);
             i32 k = ZEPHYRUM;
             i32 n;
          chorda r;

    CREDO_NECESSE(p != NIHIL && ex != NIHIL && pr != NIHIL);
    memcpy(p, planum.datum, planum.mensura);
    dum (mutare >= ZEPHYRUM && k + IV <= planum.mensura)
    {
        i32 longitudo = ((i32)p[k + I] << XVI) | ((i32)p[k + II]
            << VIII)
            | (i32)p[k + III];

        si ((i32)p[k] == typus)
        {
            CREDO_NECESSE((i32)mutare < longitudo);
            p[k + IV + (i32)mutare] ^= 0x01;
            frange;
        }
        k += IV + longitudo;
    }
    n = tls13_obsignare(pr, TLS13_CONTENTUM_PACTIO, p, planum.mensura,
        ex, CAPSA);
    CREDO_NECESSE(n > ZEPHYRUM);
    r = chorda_ex_buffer(ex, n);
    si (mutare < ZEPHYRUM)
    {
        CREDO_NECESSE(chorda_aequalis(r, _volatus(c, III)));
    }
    redde r;
}

/* exitus = unum recordum alertae sub c hs traffic: descriptio */
interior s32
_alerta_missa (CredoContextus* c, constans i8* exitus, i32 n)
{
            chorda c_hs = _octeti(c, III, 's', C_HS, ZEPHYRUM,
                "expanded");
    Tls13Protectio* pr = tls13_protectio_creare(
        TLS13_AES_128_GCM_SHA256, c_hs.datum, c->piscina);
                i8 planum[LXIV];
               i32 genus = ZEPHYRUM;
               s32 m;

    CREDO_NECESSE_NON_NIHIL(pr);
    m = tls13_aperire(pr, exitus, n, planum, LXIV, &genus);
    CREDO_AEQUALIS_S32(m, II);
    CREDO_AEQUALIS_I32(genus, TLS13_CONTENTUM_ALERTA);
    si (m != II)
    {
        redde -I;
    }
    CREDO_AEQUALIS_I32((i32)planum[ZEPHYRUM], II);   /* fatalis */
    redde (s32)planum[I];
}

interior vacuum
_errorem_verificare (CredoContextus* c, Scaena* s, chorda volatus,
    Tls13ErrorCodex codex, i32 alerta)
{
            i8 capsa[CAPSA];
           i32 n   = _pactio(c, s, III, volatus, FALSUM, capsa);
    Tls13Error e   = tls13_machina_error(s->m);
           i32 st  = tls13_machina_status(s->m);

    CREDO_AEQUALIS_I32(e.codex, codex);
    CREDO_AEQUALIS_I32(e.alerta, alerta);
    CREDO_AEQUALIS_S32(_alerta_missa(c, capsa, n), (s32)alerta);
    CREDO_VERUM((st & (TLS13_DATA_MITTI_POSSUNT
        | TLS13_RECORDA_EXSPECTAT))
        == ZEPHYRUM);
    CREDO_AEQUALIS_I32(tls13_machina_versio(s->m), ZEPHYRUM);
}

interior vacuum
_sectio_subscriptio_falsa (CredoContextus* c)
{
    Scaena s;

    _scaenam_parare(c, &s, III);
    /* CertificateVerify: schema II, longitudo II, subscriptio... */
    _errorem_verificare(c, &s, _volatum_resignare(c, XV, XIV),
        TLS13_ERROR_SUBSCRIPTIO, LI);
    CREDO_AEQUALIS_I32(s.fiducia.vocationes, I);
}

interior vacuum
_sectio_fiducia_negata (CredoContextus* c)
{
    Scaena s;

    _scaenam_parare(c, &s, III);
    s.fiducia.responsum = FALSUM;
    _errorem_verificare(c, &s, _volatum_resignare(c, ZEPHYRUM, -I),
        TLS13_ERROR_FIDUCIA, XLII);
    CREDO_CHORDA_CONTINET(tls13_machina_error(s.m).causa,
        chorda_ex_literis("ficta", c->piscina));
}

interior vacuum
_sectio_finis_falsus (
    CredoContextus* c)
{
    Scaena s;

    _scaenam_parare(c, &s, III);
    _errorem_verificare(c, &s, _volatum_resignare(c, XX, XXXI),
        TLS13_ERROR_FINIS, LI);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "s3_salutatio", _sectio_s3_salutatio, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "s3_pactio", _sectio_s3_pactio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "s3_per_octetum", _sectio_s3_per_octetum, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "s3_data", _sectio_s3_data, NIHIL, NIHIL, NIHIL, NIHIL },
    { "data_coniuncta", _sectio_data_coniuncta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "s3_clausura", _sectio_s3_clausura, NIHIL, NIHIL, NIHIL, NIHIL },
    { "s7_compatibilitas", _sectio_s7_ccs, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "subscriptio_falsa", _sectio_subscriptio_falsa, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "fiducia_negata", _sectio_fiducia_negata, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "finis_falsus", _sectio_finis_falsus, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("tls13_machina", SECTIONES,
        _suitam_parare, NIHIL);
}

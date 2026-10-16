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
#include "tls13_clavis.h"
#include "tls13_recordum.h"
#include "rfc8448.h"
#include <string.h>

#define VIA_RFC "probationes/fixa/tls13/rfc8448.txt"
#define CAPSA   (VIII * MXXIV)

#define S_HS "derive secret \"tls13 s hs traffic\""
#define C_HS "derive secret \"tls13 c hs traffic\""
#define S_AP "derive secret \"tls13 s ap traffic\""
#define C_AP "derive secret \"tls13 c ap traffic\""

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
    si (sectio == V)
    {
        chorda ch2 = _octeti(c, V, 'c', "construct a ClientHello", I,
            "ClientHello");

        s->fors.octeti[I] = _octeti(c, V, 'c',
            "create an ephemeral P-256 key pair", ZEPHYRUM,
            "private key");
        s->fors.numerus            = II;
        o.salutationes_fixae[I]    = ch2.datum;
        o.salutationes_fixae_n[I]  = ch2.mensura;
    }
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
    redde _octeti(c, sectio, 's', "send handshake record",
        sectio == V ? II : I, "complete record");
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

/* planum volatus servi sectionis sub s hs traffic obsignatum
 * (sequentia 0) */
interior chorda
_sigillare (CredoContextus* c, i32 sectio, constans i8* planum, i32 n)
{
             chorda s_hs = _octeti(c, sectio, 's', S_HS, ZEPHYRUM,
                 "expanded");
                 i8* ex = (i8*)piscina_allocare(c->piscina, CAPSA);
     Tls13Protectio* pr = tls13_protectio_creare(
         TLS13_AES_128_GCM_SHA256, s_hs.datum, c->piscina);
                i32 k;

    CREDO_NECESSE(ex != NIHIL && pr != NIHIL);
    k = tls13_obsignare(pr, TLS13_CONTENTUM_PACTIO, planum, n, ex,
        CAPSA);
    CREDO_NECESSE(k > ZEPHYRUM);
    redde chorda_ex_buffer(ex, k);
}

/* index nuntii 'typus' in plano volatus (caput IV octetorum); -I si
 * abest */
interior s32
_nuntium_invenire (
    constans i8* p,
            i32  n,
            i32  typus)
{
    i32 k = ZEPHYRUM;

    dum (k + IV <= n)
    {
        si ((i32)p[k] == typus)
        {
            redde (s32)k;
        }
        k += IV + (((i32)p[k + I] << XVI) | ((i32)p[k + II] << VIII)
            | (i32)p[k + III]);
    }
    redde -I;
}

/* volatus sectionis a probatione resignatus: octetus 'mutare' corporis
 * nuntii 'typus' flectitur; -I = nihil mutatum (tunc = RFC, quod
 * probatio ipsa verificat) */
interior chorda
_volatum_resignare (CredoContextus* c, i32 sectio, i32 typus,
    s32 mutare)
{
    chorda planum = _octeti(c, sectio, 's', "send handshake record",
        sectio == V ? II : I, "payload");
       i8* p = (i8*)piscina_allocare(c->piscina,
           (memoriae_index)planum.mensura);
    chorda r;

    CREDO_NECESSE_NON_NIHIL(p);
    memcpy(p, planum.datum, planum.mensura);
    si (mutare >= ZEPHYRUM)
    {
        s32 k = _nuntium_invenire(p, planum.mensura, typus);

        CREDO_NECESSE(k >= ZEPHYRUM);
        p[(i32)k + IV + (i32)mutare] ^= 0x01;
    }
    r = _sigillare(c, sectio, p, planum.mensura);
    si (mutare < ZEPHYRUM)
    {
        CREDO_NECESSE(chorda_aequalis(r, _volatus(c, sectio)));
    }
    redde r;
}

/* exitus = unum recordum alertae fatalis sub 'secretum' sectionis:
 * descriptio */
interior s32
_alerta_missa (CredoContextus* c, i32 sectio,
    constans character* secretum, constans i8* exitus, i32 n)
{
             chorda clavis = _octeti(c, sectio, 's', secretum,
                 ZEPHYRUM,
                 "expanded");
    Tls13Protectio* pr = tls13_protectio_creare(
        TLS13_AES_128_GCM_SHA256, clavis.datum, c->piscina);
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

/* exitus = alerta fatalis plana (ante claves): descriptio, aut -I */
interior s32
_alerta_plana (
    constans i8* exitus,
            i32  n)
{
    hic_manens constans i8 caput[VI] = { XXI, III, III, ZEPHYRUM, II,
        II };

    si (n != VII || memcmp(exitus, caput, VI) != ZEPHYRUM)
    {
        redde -I;
    }
    redde (s32)exitus[VI];
}

interior vacuum
_errorem_verificare (CredoContextus* c, Scaena* s, i32 sectio,
    chorda volatus, Tls13ErrorCodex codex, i32 alerta)
{
            i8 capsa[CAPSA];
           i32 n   = _pactio(c, s, sectio, volatus, FALSUM, capsa);
    Tls13Error e   = tls13_machina_error(s->m);
           i32 st  = tls13_machina_status(s->m);

    CREDO_AEQUALIS_I32(e.codex, codex);
    CREDO_AEQUALIS_I32(e.alerta, alerta);
    CREDO_AEQUALIS_S32(_alerta_missa(c, sectio, C_HS, capsa, n),
        (s32)alerta);
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
    _errorem_verificare(c, &s, III, _volatum_resignare(c, III, XV, XIV),
        TLS13_ERROR_SUBSCRIPTIO, LI);
    CREDO_AEQUALIS_I32(s.fiducia.vocationes, I);
}

interior vacuum
_sectio_fiducia_negata (CredoContextus* c)
{
    Scaena s;

    _scaenam_parare(c, &s, III);
    s.fiducia.responsum = FALSUM;
    _errorem_verificare(c, &s, III, _volatum_resignare(c, III, ZEPHYRUM,
        -I),
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
    _errorem_verificare(c, &s, III, _volatum_resignare(c, III, XX,
        XXXI),
        TLS13_ERROR_FINIS, LI);
}

/* §5: CH1 -> HelloRetryRequest (P-256) -> CH2 fixum -> ServerHello
 * -> volatus -> Finished nostrum = RFC; transcriptum cum message_hash
 * (tls13_transcriptum_hrr) */
interior vacuum
_sectio_s5_hrr (CredoContextus* c)
{
    Scaena s;
        i8 capsa[CAPSA];
       i32 n;
    chorda hrr = _octeti(c, V, 's', "send handshake record", ZEPHYRUM,
        "complete record");
    chorda sh = _octeti(c, V, 's', "send handshake record", I,
        "complete record");
    chorda volatus = _volatus(c, V);

    _scaenam_parare(c, &s, V);
    n = _exhaurire(s.m, capsa, CAPSA);
    CREDO_VERUM(_aequales(capsa, n, _octeti(c, V, 'c',
        "send handshake record", ZEPHYRUM, "complete record")));
    CREDO_AEQUALIS_I32(_ingerere(s.m, hrr.datum, hrr.mensura, FALSUM),
        hrr.mensura);
    n = _exhaurire(s.m, capsa, CAPSA);
    CREDO_VERUM(_aequales(capsa, n, _octeti(c, V, 'c',
        "send handshake record", I, "complete record")));
    CREDO_AEQUALIS_I32(s.fors.vocationes, II);
    CREDO_AEQUALIS_I32(_ingerere(s.m, sh.datum, sh.mensura, FALSUM),
        sh.mensura);
    CREDO_AEQUALIS_I32(_ingerere(s.m, volatus.datum, volatus.mensura,
        FALSUM), volatus.mensura);
    n = _exhaurire(s.m, capsa, CAPSA);
    CREDO_AEQUALIS_I32(tls13_machina_error(s.m).codex,
        TLS13_ERROR_NULLUS);
    CREDO_VERUM(_aequales(capsa, n, _octeti(c, V, 'c',
        "send handshake record", II, "complete record")));
    CREDO_AEQUALIS_I32(tls13_machina_versio(s.m), 0x0304);
    CREDO_AEQUALIS_I32(s.fiducia.vocationes, I);
    CREDO_AEQUALIS_I32(s.fors.vocationes, II);
    CREDO_FALSUM(s.fors.inopinata);
    tls13_machina_claudere(s.m);
    n = _exhaurire(s.m, capsa, CAPSA);
    CREDO_VERUM(_aequales(capsa, n, _octeti(c, V, 'c',
        "send alert record", ZEPHYRUM, "complete record")));
}

/* §5 usque ad CH2 missum; reddit machinam */
interior vacuum
_post_hrr (CredoContextus* c, Scaena* s)
{
        i8 capsa[CAPSA];
    chorda hrr = _octeti(c, V, 's', "send handshake record", ZEPHYRUM,
        "complete record");

    _scaenam_parare(c, s, V);
    (vacuum)_exhaurire(s->m, capsa, CAPSA);
    CREDO_AEQUALIS_I32(_ingerere(s->m, hrr.datum, hrr.mensura, FALSUM),
        hrr.mensura);
    CREDO_NECESSE(_exhaurire(s->m, capsa, CAPSA) > ZEPHYRUM);
}

/* HelloRetryRequest secundum: unexpected_message (RFC 8446 4.1.4) */
interior vacuum
_sectio_hrr_bis (CredoContextus* c)
{
    Scaena s;
        i8 capsa[CAPSA];
       i32 n;
    chorda hrr = _octeti(c, V, 's', "send handshake record", ZEPHYRUM,
        "complete record");

    _post_hrr(c, &s);
    CREDO_AEQUALIS_I32(_ingerere(s.m, hrr.datum, hrr.mensura, FALSUM),
        hrr.mensura);
    n = _exhaurire(s.m, capsa, CAPSA);
    CREDO_AEQUALIS_I32(tls13_machina_error(s.m).codex,
        TLS13_ERROR_PARAMETRUM);
    CREDO_AEQUALIS_I32(tls13_machina_error(s.m).alerta, X);
    CREDO_AEQUALIS_S32(_alerta_plana(capsa, n), X);
}

/* HRR cum grege 'grex' loco secp256r1 -> PARAMETRUM, alerta 47 */
interior vacuum
_hrr_grex (CredoContextus* c, i32 grex)
{
     Scaena s;
         i8 capsa[CAPSA];
         i8 mutatum[CCLVI];
        i32 n;
        i32 k;
        b32 inventum = FALSUM;
     chorda hrr = _octeti(c, V, 's', "send handshake record", ZEPHYRUM,
         "complete record");
    hic_manens constans i8 EXTENSIO_CLAVIUM[VI] = {
        ZEPHYRUM, 0x33, ZEPHYRUM, II, ZEPHYRUM, 0x17
    };

    CREDO_NECESSE(hrr.mensura <= CCLVI);
    memcpy(mutatum, hrr.datum, hrr.mensura);
    per (k = ZEPHYRUM; k + VI <= hrr.mensura; k++)
    {
        si (memcmp(mutatum + k, EXTENSIO_CLAVIUM, VI) == ZEPHYRUM)
        {
            mutatum[k + IV]  = (i8)(grex >> VIII);
            mutatum[k + V]   = (i8)grex;
            inventum         = VERUM;
            frange;
        }
    }
    CREDO_NECESSE(inventum);
    _scaenam_parare(c, &s, V);
    (vacuum)_exhaurire(s.m, capsa, CAPSA);
    CREDO_AEQUALIS_I32(_ingerere(s.m, mutatum, hrr.mensura, FALSUM),
        hrr.mensura);
    n = _exhaurire(s.m, capsa, CAPSA);
    CREDO_AEQUALIS_I32(tls13_machina_error(s.m).codex,
        TLS13_ERROR_PARAMETRUM);
    CREDO_AEQUALIS_S32(_alerta_plana(capsa, n), XLVII);
    CREDO_AEQUALIS_I32(s.fors.vocationes, I);
}

interior vacuum
_sectio_hrr_grex_ignotus (
    CredoContextus* c)
{
    _hrr_grex(c, 0x0018);   /* secp384r1: non oblatus */
    _hrr_grex(c, 0x001d);   /* x25519: iam missus */
}

/* alerta plana in responsum ClientHello */
interior Tls13Error
_alerta_accepta (CredoContextus* c, Scaena* s, i32 descriptio,
    i32* exitus_n)
{
    i8 capsa[CAPSA];
    i8 alerta[VII];

    alerta[ZEPHYRUM]  = XXI;
    alerta[I]         = III;
    alerta[II]        = III;
    alerta[III]       = ZEPHYRUM;
    alerta[IV]        = II;
    alerta[V]         = II;
    alerta[VI]        = (i8)descriptio;
    _scaenam_parare(c, s, III);
    (vacuum)_exhaurire(s->m, capsa, CAPSA);
    CREDO_AEQUALIS_I32(_ingerere(s->m, alerta, VII, FALSUM), VII);
    *exitus_n = _exhaurire(s->m, capsa, CAPSA);
    redde tls13_machina_error(s->m);
}

/* protocol_version ad CH1 -> SOLUM_1_2, nihil remissum */
interior vacuum
_sectio_alerta_70 (CredoContextus* c)
{
        Scaena s;
           i32 n = ZEPHYRUM;
    Tls13Error e = _alerta_accepta(c, &s, LXX, &n);

    CREDO_AEQUALIS_I32(e.codex, TLS13_ERROR_SOLUM_1_2);
    CREDO_AEQUALIS_I32(n, ZEPHYRUM);
    CREDO_AEQUALIS_I32(tls13_machina_status(s.m), ZEPHYRUM);
}

interior vacuum
_sectio_alerta_40 (CredoContextus* c)
{
        Scaena s;
           i32 n = ZEPHYRUM;
    Tls13Error e = _alerta_accepta(c, &s, XL, &n);

    CREDO_AEQUALIS_I32(e.codex, TLS13_ERROR_ALERTA_RECEPTA);
    CREDO_AEQUALIS_I32(e.alerta, XL);
    CREDO_CHORDA_CONTINET(e.causa, chorda_ex_literis(
        "40 (handshake_failure)", c->piscina));
    CREDO_AEQUALIS_I32(n, ZEPHYRUM);
}

/* KeyUpdate(update_requested) sub s ap: claves lectionis renovatae;
 * KeyUpdate(update_not_requested) noster sub c ap VETERE; deinde
 * scriptio sub c ap nova. Postea KeyUpdate(non rogatum): solum
 * lectio. */
interior vacuum
_sectio_keyupdate (CredoContextus* c)
{
             Scaena s;
                 i8 capsa[CAPSA];
                 i8 recordum[CAPSA];
                 i8 planum[CAPSA];
             chorda s_ap = _octeti(c, III, 's', S_AP, ZEPHYRUM,
                 "expanded");
             chorda c_ap = _octeti(c, III, 's', C_AP, ZEPHYRUM,
                 "expanded");
    Tls13Protectio* servus = tls13_protectio_creare(
        TLS13_AES_128_GCM_SHA256, s_ap.datum, c->piscina);
    Tls13Protectio* cliens = tls13_protectio_creare(
        TLS13_AES_128_GCM_SHA256, c_ap.datum, c->piscina);
    hic_manens constans i8 ROGATUM[V] = { XXIV, ZEPHYRUM, ZEPHYRUM, I,
        I };
    hic_manens constans i8 NON_ROGATUM[V] = { XXIV, ZEPHYRUM, ZEPHYRUM,
        I, ZEPHYRUM };
    hic_manens constans i8  POST[IV] = { 'p', 'o', 's', 't' };
                       i32  n;
                       i32  genus = ZEPHYRUM;
                       s32  k;
                        i8* d;

    CREDO_NECESSE(servus != NIHIL && cliens != NIHIL);
    _scaenam_parare(c, &s, III);
    _pactionem_verificare(c, &s, III, FALSUM);
    n = tls13_obsignare(servus, TLS13_CONTENTUM_PACTIO, ROGATUM, V,
        recordum, CAPSA);
    CREDO_AEQUALIS_I32(_ingerere(s.m, recordum, n, FALSUM), n);
    n = _exhaurire(s.m, capsa, CAPSA);
    k = tls13_aperire(cliens, capsa, n, planum, CAPSA, &genus);
    CREDO_AEQUALIS_S32(k, V);
    CREDO_AEQUALIS_I32(genus, TLS13_CONTENTUM_PACTIO);
    CREDO_VERUM(k == V && memcmp(planum, NON_ROGATUM, V) == ZEPHYRUM);
    tls13_protectio_renovare(servus);
    tls13_protectio_renovare(cliens);
    /* data servi sub s ap nova */
    n = tls13_obsignare(servus, TLS13_CONTENTUM_DATA, POST, IV,
        recordum, CAPSA);
    CREDO_AEQUALIS_I32(_ingerere(s.m, recordum, n, FALSUM), n);
    n = ZEPHYRUM;
    d = tls13_data_introitus(s.m, &n);
    CREDO_NECESSE_NON_NIHIL(d);
    CREDO_VERUM(n == IV && memcmp(d, POST, IV) == ZEPHYRUM);
    tls13_data_introitus_ack(s.m, n);
    /* data nostra sub c ap nova */
    d = tls13_data_exitus(s.m, &n);
    CREDO_NECESSE(d != NIHIL && n >= IV);
    memcpy(d, POST, IV);
    tls13_data_exitus_ack(s.m, IV);
    tls13_data_effundere(s.m);
    n = _exhaurire(s.m, capsa, CAPSA);
    k = tls13_aperire(cliens, capsa, n, planum, CAPSA, &genus);
    CREDO_AEQUALIS_S32(k, IV);
    CREDO_AEQUALIS_I32(genus, TLS13_CONTENTUM_DATA);
    /* KeyUpdate non rogatum: nihil mittitur, lectio renovatur */
    n = tls13_obsignare(servus, TLS13_CONTENTUM_PACTIO, NON_ROGATUM, V,
        recordum, CAPSA);
    CREDO_AEQUALIS_I32(_ingerere(s.m, recordum, n, FALSUM), n);
    CREDO_AEQUALIS_I32(_exhaurire(s.m, capsa, CAPSA), ZEPHYRUM);
    tls13_protectio_renovare(servus);
    n = tls13_obsignare(servus, TLS13_CONTENTUM_DATA, POST, IV,
        recordum, CAPSA);
    CREDO_AEQUALIS_I32(_ingerere(s.m, recordum, n, FALSUM), n);
    n = ZEPHYRUM;
    CREDO_NON_NIHIL(tls13_data_introitus(s.m, &n));
    CREDO_AEQUALIS_I32(n, IV);
    CREDO_AEQUALIS_I32(tls13_machina_error(s.m).codex,
        TLS13_ERROR_NULLUS);
}

/* post ServerHello: caput recordi 2^14 + 257 -> MOLES, alerta 22 */
interior vacuum
_sectio_recordum_moles (
    CredoContextus* c)
{
           Scaena s;
               i8 caput[V] = { XVII, III, III, 0x41, I };

    _scaenam_parare(c, &s, III);
    _errorem_verificare(c, &s, III, chorda_ex_buffer(caput, V),
        TLS13_ERROR_MOLES, XXII);
}

/* volatus cum signo (tag) flexo -> MAC, alerta 20 */
interior vacuum
_sectio_recordum_mac (CredoContextus* c)
{
    Scaena  s;
    chorda  v = _volatus(c, III);
        i8* mutatum = (i8*)piscina_allocare(c->piscina,
            (memoriae_index)v.mensura);

    CREDO_NECESSE_NON_NIHIL(mutatum);
    memcpy(mutatum, v.datum, v.mensura);
    mutatum[v.mensura - I] ^= 0x01;
    _scaenam_parare(c, &s, III);
    _errorem_verificare(c, &s, III, chorda_ex_buffer(mutatum,
        v.mensura), TLS13_ERROR_MAC, XX);
}

/* §6: CertificateRequest sine identitate - Certificate vacuum +
 * Finished (a probatione computatum); clausura nostra = RFC octetim
 * (c ap a certificato clientis non pendet). Servus ECDSA P-256. */
interior vacuum
_sectio_s6_sine_identitate (CredoContextus* c)
{
                Scaena s;
                    i8 capsa[CAPSA];
                    i8 planum[CAPSA];
                    i8 hash_transcripti[XXXII];
                    i8 exspectata[XLIV];
                   i32 n;
                   i32 genus = ZEPHYRUM;
                   s32 k;
                chorda c_hs = _octeti(c, VI, 's', C_HS, ZEPHYRUM,
                    "expanded");
       Tls13Protectio* pr = tls13_protectio_creare(
           TLS13_AES_128_GCM_SHA256, c_hs.datum, c->piscina);
    Tls13Transcriptum* t = tls13_transcriptum_creare(TLS13_SHA256,
        c->piscina);
    hic_manens constans i8 VACUUM[VIII] = { XI, ZEPHYRUM, ZEPHYRUM, IV,
        ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM };
    hic_manens constans character* constans NUNTII[VII][II] = {
        { "construct a ClientHello", "ClientHello" },
        { "construct a ServerHello", "ServerHello" },
        { "construct an EncryptedExtensions", "EncryptedExtensions" },
        { "construct a CertificateRequest", "CertificateRequest" },
        { "construct a Certificate ", "Certificate" },
        { "construct a CertificateVerify", "CertificateVerify" },
        { "construct a Finished", "Finished" }
    };
                   i32 j;

    CREDO_NECESSE(pr != NIHIL && t != NIHIL);
    _scaenam_parare(c, &s, VI);
    n = _pactio(c, &s, VI, _volatus(c, VI), FALSUM, capsa);
    CREDO_AEQUALIS_I32(tls13_machina_error(s.m).codex,
        TLS13_ERROR_NULLUS);
    CREDO_AEQUALIS_I32(tls13_machina_versio(s.m), 0x0304);
    CREDO_AEQUALIS_I32(s.fiducia.vocationes, I);
    /* Finished exspectatum: transcriptum RFC + Certificate vacuum */
    per (j = ZEPHYRUM; j < VII; j++)
    {
        chorda m = _octeti(c, VI, j == ZEPHYRUM ? 'c' : 's',
            NUNTII[j][ZEPHYRUM], ZEPHYRUM, NUNTII[j][I]);

        tls13_transcriptum_addere(t, m.datum, m.mensura);
    }
    tls13_transcriptum_addere(t, VACUUM, VIII);
    tls13_transcriptum_digerere(t, hash_transcripti);
    exspectata[ZEPHYRUM]  = XX;
    exspectata[I]         = ZEPHYRUM;
    exspectata[II]        = ZEPHYRUM;
    exspectata[III]       = XXXII;
    tls13_finis_calculare(TLS13_SHA256, c_hs.datum, hash_transcripti,
        exspectata + IV);
    k = tls13_aperire(pr, capsa, n, planum, CAPSA, &genus);
    CREDO_AEQUALIS_S32(k, (s32)(VIII + XXXVI));
    CREDO_AEQUALIS_I32(genus, TLS13_CONTENTUM_PACTIO);
    CREDO_VERUM(k == (s32)(VIII + XXXVI)
        && memcmp(planum, VACUUM, VIII) == ZEPHYRUM);
    CREDO_VERUM(k == (s32)(VIII + XXXVI)
        && memcmp(planum + VIII, exspectata, XXXVI) == ZEPHYRUM);
    tls13_machina_claudere(s.m);
    n = _exhaurire(s.m, capsa, CAPSA);
    CREDO_VERUM(_aequales(capsa, n, _octeti(c, VI, 'c',
        "send alert record", ZEPHYRUM, "complete record")));
}

/* CertificateRequest cum vectore extensionum longiore quam reliquum
 * -> FORMA, alerta 50 */
interior vacuum
_sectio_cr_forma (
    CredoContextus* c)
{
    Scaena s;

    _scaenam_parare(c, &s, VI);
    /* corpus: contextus I, longitudo extensionum II (0x0024 -> 25) */
    _errorem_verificare(c, &s, VI, _volatum_resignare(c, VI, XIII, II),
        TLS13_ERROR_FORMA, L);
}

/* contextus CertificateRequest in pactione vacuus esse DEBET (RFC 8446
 * 4.3.2) -> PARAMETRUM, alerta 47 */
interior vacuum
_sectio_cr_contextus (CredoContextus* c)
{
    Scaena s;
    chorda planum = _octeti(c, VI, 's', "send handshake record", I,
        "payload");
       i8* p = (i8*)piscina_allocare(c->piscina,
           (memoriae_index)planum.mensura + I);
       s32 k;
       i32 j;
       i32 longitudo;

    CREDO_NECESSE_NON_NIHIL(p);
    k = _nuntium_invenire(planum.datum, planum.mensura, XIII);
    CREDO_NECESSE(k >= ZEPHYRUM);
    j = (i32)k;
    /* contextus unius octeti 0xab post longitudinem eius inseritur */
    memcpy(p, planum.datum, j + V);
    p[j + V] = 0xab;
    memcpy(p + j + VI, planum.datum + j + V, planum.mensura - j - V);
    p[j + IV] = I;
    longitudo = (((i32)p[j + I] << XVI) | ((i32)p[j + II] << VIII)
        | (i32)p[j + III]) + I;
    p[j + I]    = (i8)(longitudo >> XVI);
    p[j + II]   = (i8)(longitudo >> VIII);
    p[j + III]  = (i8)longitudo;
    _scaenam_parare(c, &s, VI);
    _errorem_verificare(c, &s, VI, _sigillare(c, VI, p,
        planum.mensura + I), TLS13_ERROR_PARAMETRUM, XLVII);
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
    { "s5_hrr", _sectio_s5_hrr, NIHIL, NIHIL, NIHIL, NIHIL },
    { "hrr_bis", _sectio_hrr_bis, NIHIL, NIHIL, NIHIL, NIHIL },
    { "hrr_grex_ignotus", _sectio_hrr_grex_ignotus, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "alerta_70", _sectio_alerta_70, NIHIL, NIHIL, NIHIL, NIHIL },
    { "alerta_40", _sectio_alerta_40, NIHIL, NIHIL, NIHIL, NIHIL },
    { "keyupdate", _sectio_keyupdate, NIHIL, NIHIL, NIHIL, NIHIL },
    { "recordum_moles", _sectio_recordum_moles, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "recordum_mac", _sectio_recordum_mac, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "s6_sine_identitate", _sectio_s6_sine_identitate, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "cr_forma", _sectio_cr_forma, NIHIL, NIHIL, NIHIL, NIHIL },
    { "cr_contextus", _sectio_cr_contextus, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("tls13_machina", SECTIONES,
        _suitam_parare, NIHIL);
}

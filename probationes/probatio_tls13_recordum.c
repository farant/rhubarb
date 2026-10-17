/* probatio_tls13_recordum.c - stratum recordorum TLS 1.3 (tls13-spec
 * II.1, tls13-plan T2): sigillatio et apertio contra RFC 8448 §3
 * octetim (AES-128-GCM), numerus sequentiae per recorda plura, vitia
 * (MAC, ordo, moles, forma), ChaCha20-Poly1305 et AES-256-GCM per
 * circuitum, KeyUpdate. Oraculum: textus RFC immutatus. */
#include "../vendor/bearssl_amalgama.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "tls13_clavis.h"
#include "tls13_recordum.h"
#include "rfc8448.h"
#include <string.h>

#define VIA_RFC "probationes/fixa/tls13/rfc8448.txt"

interior vacuum
_suitam_parare (CredoContextus* c)
{
    Rfc8448* r = (Rfc8448*)piscina_allocare(c->piscina,
        magnitudo(Rfc8448));

    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_NECESSE(rfc8448_legere(r, VIA_RFC, c->piscina));
    c->datum = r;
}

/* campus actionis §3 (ordo 0 nisi aliter) - NECESSE adest */
interior constans Rfc8448Campus*
_campus (CredoContextus* c, character latus, constans character* actio,
    i32 ordo, constans character* campus)
{
    constans Rfc8448Campus* f = rfc8448_quaerere(
        (constans Rfc8448*)c->suita, III, latus, actio, ordo, campus);

    CREDO_NECESSE_NON_NIHIL(f);
    redde f;
}

/* protectio ex secreto 'derive secret "tls13 <titulus>"' (servus) */
interior Tls13Protectio*
_protectio (CredoContextus* c, constans character* actio)
{
    constans Rfc8448Campus* s = _campus(c, 's', actio, ZEPHYRUM,
        "expanded");
    Tls13Protectio* pr =
        tls13_protectio_creare(TLS13_AES_128_GCM_SHA256,
        s->octeti.datum, c->piscina);

    CREDO_NECESSE_NON_NIHIL(pr);
    redde pr;
}

#define S_HS "derive secret \"tls13 s hs traffic\""
#define C_AP "derive secret \"tls13 c ap traffic\""
#define S_AP "derive secret \"tls13 s ap traffic\""

/* sigillat 'payload' actionis cum genere et comparat cum 'complete
 * record' */
interior vacuum
_sigillare_comparare (CredoContextus* c, Tls13Protectio* pr,
    character latus, constans character* actio, i32 ordo, i32 genus)
{
    constans Rfc8448Campus* p = _campus(c, latus, actio, ordo,
        "payload");
    constans Rfc8448Campus* r = _campus(c, latus, actio, ordo,
        "complete record");
     i8 exitus[TLS13_RECORDUM_MAXIMUM];
    i32 n;

    n = tls13_obsignare(pr, genus, p->octeti.datum, p->octeti.mensura,
        exitus, TLS13_RECORDUM_MAXIMUM);
    CREDO_AEQUALIS_I32(n, r->octeti.mensura);
    CREDO_VERUM(memcmp(exitus, r->octeti.datum, r->octeti.mensura)
                == ZEPHYRUM);
}

/* aperit 'complete record' et comparat cum 'payload' et genere */
interior vacuum
_aperire_comparare (CredoContextus* c, Tls13Protectio* pr,
    character latus, constans character* actio, i32 ordo, i32 genus)
{
    constans Rfc8448Campus* p = _campus(c, latus, actio, ordo,
        "payload");
    constans Rfc8448Campus* r = _campus(c, latus, actio, ordo,
        "complete record");
     i8 exitus[TLS13_RECORDUM_MAXIMUM];
    i32 g = ZEPHYRUM;
    s32 n;

    n = tls13_aperire(pr, r->octeti.datum, r->octeti.mensura, exitus,
        TLS13_RECORDUM_MAXIMUM, &g);
    CREDO_AEQUALIS_S32(n, (s32)p->octeti.mensura);
    CREDO_AEQUALIS_I32(g, genus);
    CREDO_VERUM(n > ZEPHYRUM
                && memcmp(exitus, p->octeti.datum, (i32)n) == ZEPHYRUM);
}

/* volatus servi cifratus (EE, Certificate, CV, Finished): sequentia 0
 * sub s hs traffic */
interior vacuum
_sectio_obsignare_servus (
    CredoContextus* c)
{
    _sigillare_comparare(c, _protectio(c, S_HS), 's',
        "send handshake record", I, TLS13_CONTENTUM_PACTIO);
}

interior vacuum
_sectio_aperire_servus (
    CredoContextus* c)
{
    _aperire_comparare(c, _protectio(c, S_HS), 's',
        "send handshake record", I, TLS13_CONTENTUM_PACTIO);
}

/* recorda plura sub eodem secreto: sequentia 0, 1, 2 - nonce per
 * numerum verificatur (recordum 0 solum numerum nullum habet) */
interior vacuum
_sectio_data (
    CredoContextus* c)
{
    Tls13Protectio* cliens = _protectio(c, C_AP);
    Tls13Protectio* servus = _protectio(c, S_AP);

    _sigillare_comparare(c, cliens, 'c', "send application_data record",
        ZEPHYRUM, TLS13_CONTENTUM_DATA);
    _sigillare_comparare(c, cliens, 'c', "send alert record", ZEPHYRUM,
        TLS13_CONTENTUM_ALERTA);
    /* servus: NewSessionTicket (0), data (1), alerta (2) */
    _aperire_comparare(c, servus, 's', "send handshake record", II,
        TLS13_CONTENTUM_PACTIO);
    _aperire_comparare(c, servus, 's', "send application_data record",
        ZEPHYRUM, TLS13_CONTENTUM_DATA);
    _aperire_comparare(c, servus, 's', "send alert record", ZEPHYRUM,
        TLS13_CONTENTUM_ALERTA);
}

/* tag mutatus -> MAC */
interior vacuum
_sectio_vitium_mac (CredoContextus* c)
{
    constans Rfc8448Campus* r = _campus(c, 's', "send handshake record",
        I, "complete record");
     i8 recordum[TLS13_RECORDUM_MAXIMUM];
     i8 exitus[TLS13_RECORDUM_MAXIMUM];
    i32 g = ZEPHYRUM;

    memcpy(recordum, r->octeti.datum, r->octeti.mensura);
    recordum[r->octeti.mensura - I] = (i8)(recordum[r->octeti.mensura
        - I]
                                           ^ I);
    CREDO_AEQUALIS_S32(tls13_aperire(_protectio(c, S_HS), recordum,
        r->octeti.mensura, exitus, TLS13_RECORDUM_MAXIMUM, &g),
        TLS13_RECORDUM_MAC);
}

/* idem recordum bis: sequentia progressa -> MAC (replay) */
interior vacuum
_sectio_vitium_ordo (CredoContextus* c)
{
    constans Rfc8448Campus* r = _campus(c, 's', "send handshake record",
        I, "complete record");
    Tls13Protectio* pr = _protectio(c, S_HS);
                i8  exitus[TLS13_RECORDUM_MAXIMUM];
               i32  g = ZEPHYRUM;

    CREDO_VERUM(tls13_aperire(pr, r->octeti.datum, r->octeti.mensura,
        exitus, TLS13_RECORDUM_MAXIMUM, &g) > ZEPHYRUM);
    CREDO_AEQUALIS_S32(tls13_aperire(pr, r->octeti.datum,
        r->octeti.mensura, exitus, TLS13_RECORDUM_MAXIMUM, &g),
        TLS13_RECORDUM_MAC);
}

/* longitudo capitis 16641 (> 2^14 + 256) -> MOLES ante AEAD */
interior vacuum
_sectio_vitium_moles (CredoContextus* c)
{
    i32  corpus = XVI * MXXIV + CCLVI + I;
     i8* recordum = (i8*)piscina_allocare(c->piscina,
         (memoriae_index)(V + corpus));
    i8* exitus = (i8*)piscina_allocare(c->piscina,
        (memoriae_index)corpus);
    i32 g = ZEPHYRUM;

    CREDO_NECESSE(recordum && exitus);
    memset(recordum, ZEPHYRUM, V + corpus);
    recordum[ZEPHYRUM]  = (i8)TLS13_CONTENTUM_DATA;
    recordum[I]         = (i8)0x03;
    recordum[II]        = (i8)0x03;
    recordum[III]       = (i8)(corpus >> VIII);
    recordum[IV]        = (i8)(corpus & 0xff);
    CREDO_AEQUALIS_S32(tls13_aperire(_protectio(c, S_HS), recordum,
        V + corpus, exitus, corpus, &g), TLS13_RECORDUM_MOLES);
}

/* TLSInnerPlaintext ex zeris solis (genus 0, data nulla) -> FORMA;
 * capite recordi truncato -> FORMA */
interior vacuum
_sectio_vitium_forma (CredoContextus* c)
{
    Tls13Protectio* a = _protectio(c, S_HS);
    Tls13Protectio* b = _protectio(c, S_HS);
                i8  data[VIII];
                i8  recordum[LXIV];
                i8  exitus[LXIV];
               i32  g = ZEPHYRUM;
               i32  n;

    memset(data, ZEPHYRUM, VIII);
    n = tls13_obsignare(a, ZEPHYRUM, data, VIII, recordum, LXIV);
    CREDO_AEQUALIS_I32(n, V + VIII + I + XVI);
    CREDO_AEQUALIS_S32(tls13_aperire(b, recordum, n, exitus, LXIV, &g),
        TLS13_RECORDUM_FORMA);
    CREDO_AEQUALIS_S32(tls13_aperire(b, recordum, IV, exitus, LXIV, &g),
        TLS13_RECORDUM_FORMA);
}

/* cifrae aliae (sine vectore RFC; interoperatio in T6): circuitus et
 * tag mutatus */
interior vacuum
_circuitus (CredoContextus* c, i32 cifra)
{
                i8  secretum[TLS13_HASH_MAXIMUS];
                i8  data[C];
                i8  recordum[CC];
                i8  exitus[CC];
               i32  g = ZEPHYRUM;
               i32  k;
               i32  n;
               s32  m;
    Tls13Protectio* a;
    Tls13Protectio* b;
    Tls13Protectio* d;

    per (k = ZEPHYRUM; k < TLS13_HASH_MAXIMUS; k++)
    {
        secretum[k] = (i8)(k * VII);
    }
    per (k = ZEPHYRUM; k < C; k++)
    {
        data[k] = (i8)(k + III);
    }
    a = tls13_protectio_creare(cifra, secretum, c->piscina);
    b = tls13_protectio_creare(cifra, secretum, c->piscina);
    d = tls13_protectio_creare(cifra, secretum, c->piscina);
    CREDO_NECESSE(a && b && d);
    per (k = ZEPHYRUM; k < III; k++)
    {
        n = tls13_obsignare(a, TLS13_CONTENTUM_DATA, data, C, recordum,
            CC);
        CREDO_AEQUALIS_I32(n, V + C + I + XVI);
        m = tls13_aperire(b, recordum, n, exitus, CC, &g);
        CREDO_AEQUALIS_S32(m, (s32)C);
        CREDO_AEQUALIS_I32(g, TLS13_CONTENTUM_DATA);
        CREDO_VERUM(memcmp(exitus, data, C) == ZEPHYRUM);
    }
    n = tls13_obsignare(a, TLS13_CONTENTUM_DATA, data, C, recordum, CC);
    recordum[n - I] = (i8)(recordum[n - I] ^ 0x80);
    CREDO_AEQUALIS_S32(tls13_aperire(b, recordum, n, exitus, CC, &g),
        TLS13_RECORDUM_MAC);
    /* sequentia 0 sub d: idem recordum quod a sub sequentia 3 non
     * aperitur */
    recordum[n - I] = (i8)(recordum[n - I] ^ 0x80);
    CREDO_AEQUALIS_S32(tls13_aperire(d, recordum, n, exitus, CC, &g),
        TLS13_RECORDUM_MAC);
}

interior vacuum
_sectio_chacha (CredoContextus* c)
{
    _circuitus(c, TLS13_CHACHA20_POLY1305_SHA256);
    CREDO_AEQUALIS_I32(
        tls13_cifra_genus(TLS13_CHACHA20_POLY1305_SHA256),
        TLS13_SHA256);
}

interior vacuum
_sectio_aes256 (CredoContextus* c)
{
    _circuitus(c, TLS13_AES_256_GCM_SHA384);
    CREDO_AEQUALIS_I32(tls13_cifra_genus(TLS13_AES_256_GCM_SHA384),
        TLS13_SHA384);
    CREDO_AEQUALIS_I32(tls13_cifra_genus(0x1304), ZEPHYRUM);
    CREDO_VERUM(tls13_protectio_creare(0x1304, (constans i8*)"x",
        c->piscina) == NIHIL);
}

/* KeyUpdate: post renovationem utrimque recorda circumeunt; recordum
 * sub clavibus veteribus sigillatum post renovationem non aperitur;
 * secretum novum = 'traffic upd' veteris */
interior vacuum
_sectio_renovare (CredoContextus* c)
{
    constans Rfc8448Campus* s = _campus(c, 's', C_AP, ZEPHYRUM,
        "expanded");
    Tls13Protectio* a = tls13_protectio_creare(TLS13_AES_128_GCM_SHA256,
        s->octeti.datum, c->piscina);
    Tls13Protectio* b = tls13_protectio_creare(TLS13_AES_128_GCM_SHA256,
        s->octeti.datum, c->piscina);
    Tls13Protectio* d;
                i8  proximum[XXXII];
                i8  vetus[LXIV];
                i8  novum[LXIV];
                i8  exitus[LXIV];
               i32  nv;
               i32  nn;
               i32  g = ZEPHYRUM;

    CREDO_NECESSE(a && b);
    nv = tls13_obsignare(a, TLS13_CONTENTUM_DATA, (constans i8*)"vetus",
        V, vetus, LXIV);
    tls13_protectio_renovare(a);
    tls13_protectio_renovare(b);
    nn = tls13_obsignare(a, TLS13_CONTENTUM_DATA, (constans i8*)"novum",
        V, novum, LXIV);
    CREDO_AEQUALIS_S32(tls13_aperire(b, novum, nn, exitus, LXIV, &g),
        (s32)V);
    CREDO_VERUM(memcmp(exitus, "novum", V) == ZEPHYRUM);
    CREDO_AEQUALIS_S32(tls13_aperire(b, vetus, nv, exitus, LXIV, &g),
        TLS13_RECORDUM_MAC);
    /* clavis nova = protectio ex secreto proximo, sequentia 0 */
    tls13_secretum_proximum(TLS13_SHA256, s->octeti.datum, proximum);
    d = tls13_protectio_creare(TLS13_AES_128_GCM_SHA256, proximum,
        c->piscina);
    CREDO_NECESSE_NON_NIHIL(d);
    CREDO_AEQUALIS_S32(tls13_aperire(d, novum, nn, exitus, LXIV, &g),
        (s32)V);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "obsignare_servus", _sectio_obsignare_servus, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "aperire_servus", _sectio_aperire_servus, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "data", _sectio_data, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vitium_mac", _sectio_vitium_mac, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vitium_ordo", _sectio_vitium_ordo, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vitium_moles", _sectio_vitium_moles, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "vitium_forma", _sectio_vitium_forma, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "chacha", _sectio_chacha, NIHIL, NIHIL, NIHIL, NIHIL },
    { "aes256", _sectio_aes256, NIHIL, NIHIL, NIHIL, NIHIL },
    { "renovare", _sectio_renovare, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("tls13_recordum", SECTIONES,
        _suitam_parare, NIHIL);
}

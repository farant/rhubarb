/* tls13_nuntii.c - nuntii pactionis TLS 1.3 (RFC 8446 §4): cursor
 * lectionis qui fines numquam excedit (omnis lectio ultra reliquum
 * 'fractus' ponit et nihil reddit) et scriptor cum acervo longitudinum
 * pro vectoribus inclusis. Soli adiutores cursoris octetos legunt. */
#include "latina.h"
#include "fiducia.h"
#include "tls13_nuntii.h"
#include <string.h>


/* ================================================== cursor */

nomen structura {
     constans i8* octeti;
             i32  mensura;
             i32  locus;
             b32  fractus;
} Cursor;

interior vacuum
_cursor (
         Cursor* c,
    constans i8* octeti,
            i32  mensura)
{
    c->octeti   = octeti;
    c->mensura  = mensura;
    c->locus    = ZEPHYRUM;
    c->fractus  = FALSUM;
}

/* n octeti adsunt? aliter fractus */
interior b32
_adsunt (
    Cursor* c,
       i32  n)
{
    si (c->fractus || n > c->mensura - c->locus)
    {
        c->fractus = VERUM;
        redde FALSUM;
    }
    redde VERUM;
}

interior b32
_finitus (
    constans Cursor* c)
{
    redde !c->fractus && c->locus == c->mensura;
}

interior i32
_u8 (
    Cursor* c)
{
    i32 v;

    si (!_adsunt(c, I))
    {
        redde ZEPHYRUM;
    }
    v         = (i32)c->octeti[c->locus];
    c->locus  += I;
    redde v;
}

interior i32
_u16 (
    Cursor* c)
{
    i32 v;

    si (!_adsunt(c, II))
    {
        redde ZEPHYRUM;
    }
    v = ((i32)c->octeti[c->locus] << VIII) | (i32)c->octeti[c->locus
        + I];
    c->locus += II;
    redde v;
}

interior i32
_u24 (
    Cursor* c)
{
    i32 v;

    si (!_adsunt(c, III))
    {
        redde ZEPHYRUM;
    }
    v = ((i32)c->octeti[c->locus] << XVI)
        | ((i32)c->octeti[c->locus + I] << VIII)
        | (i32)c->octeti[c->locus + II];
    c->locus += III;
    redde v;
}

interior constans i8*
_octeti (
    Cursor* c,
       i32  n)
{
    constans i8* p;

    si (!_adsunt(c, n))
    {
        redde NIHIL;
    }
    p         = c->octeti + c->locus;
    c->locus  += n;
    redde p;
}

/* vector cum longitudine latitudinis I/II/III: cursor sub eo; fractus
 * si longitudo reliquum excedit */
interior Cursor
_vector (
    Cursor* c,
       i32  latitudo)
{
         Cursor  v;
            i32  l;
    constans i8* p;

    l = latitudo == I ? _u8(c) : latitudo == II ? _u16(c) : _u24(c);
    p = _octeti(c, l);
    _cursor(&v, p != NIHIL ? p : c->octeti, p != NIHIL ? l : ZEPHYRUM);
    v.fractus = c->fractus;
    redde v;
}


/* ================================================== scriptor */

#define SCRIPTOR_PROFUNDITAS  VIII

nomen structura {
     i8* o;
    i32  capacitas;
    i32  n;
    b32  fractus;
    i32  initia[SCRIPTOR_PROFUNDITAS];
    i32  latitudines[SCRIPTOR_PROFUNDITAS];
    i32  profunditas;
} Scriptor;

interior vacuum
_scr_u8 (
    Scriptor* s,
         i32  v)
{
    si (s->fractus || s->n >= s->capacitas)
    {
        s->fractus = VERUM;
        redde;
    }
    s->o[s->n++] = (i8)(v & 0xff);
}

interior vacuum
_scr_u16 (
    Scriptor* s,
         i32  v)
{
    _scr_u8(s, v >> VIII);
    _scr_u8(s, v);
}

interior vacuum
_scr_octeti (
       Scriptor* s,
    constans i8* p,
            i32  n)
{
    si (n == ZEPHYRUM)
    {
        redde;
    }
    si (s->fractus || p == NIHIL || n > s->capacitas - s->n)
    {
        s->fractus = VERUM;
        redde;
    }
    memcpy(s->o + s->n, p, n);
    s->n += n;
}

/* vector incipit: latitudo octetorum longitudinis reservatur */
interior vacuum
_scr_aperire (
    Scriptor* s,
         i32  latitudo)
{
    i32 k;

    si (s->profunditas >= SCRIPTOR_PROFUNDITAS)
    {
        s->fractus = VERUM;
        redde;
    }
    s->initia[s->profunditas]       = s->n;
    s->latitudines[s->profunditas]  = latitudo;
    s->profunditas++;
    per (k = ZEPHYRUM; k < latitudo; k++)
    {
        _scr_u8(s, ZEPHYRUM);
    }
}

/* vector finitur: longitudo in locum reservatum scribitur */
interior vacuum
_scr_claudere (
    Scriptor* s)
{
    i32 initium;
    i32 latitudo;
    i32 l;
    i32 k;

    si (s->profunditas == ZEPHYRUM)
    {
        s->fractus = VERUM;
        redde;
    }
    s->profunditas--;
    si (s->fractus)
    {
        redde;
    }
    initium   = s->initia[s->profunditas];
    latitudo  = s->latitudines[s->profunditas];
    l         = s->n - initium - latitudo;
    per (k = ZEPHYRUM; k < latitudo; k++)
    {
        s->o[initium + k] = (i8)((l >> (VIII * (latitudo - I - k)))
                                 & 0xff);
    }
}

/* IP litteralis (RFC 6066 3: server_name non IP): ':' (IPv6) aut soli
 * digiti et puncta (IPv4) */
interior b32
_est_ip (
    constans character* hospes)
{
    constans character* p;

    si (strchr(hospes, ':') != NIHIL)
    {
        redde VERUM;
    }
    per (p = hospes; *p; p++)
    {
        si (!((*p >= '0' && *p <= '9') || *p == '.'))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}


/* ================================================== ClientHello */

i32
tls13_salutatio_scribere (
    constans Tls13SalutatioParametri* sp,
                                  i8* exitus,
                                 i32  capacitas)
{
    Scriptor s;

    memset(&s, ZEPHYRUM, magnitudo(s));
    s.o          = exitus;
    s.capacitas  = capacitas;
    _scr_u8(&s, TLS13_NUNTIUS_CLIENTHELLO);
    _scr_aperire(&s, III);
    _scr_u16(&s, 0x0303);                        /* legacy_version */
    _scr_octeti(&s, sp->temere, XXXII);
    _scr_aperire(&s, I);                         /* legacy_session_id */
    _scr_octeti(&s, sp->sessio, sp->sessio_n);
    _scr_claudere(&s);
    _scr_aperire(&s, II);                        /* cipher_suites */
    _scr_u16(&s, 0x1301);
    _scr_u16(&s, 0x1302);
    _scr_u16(&s, 0x1303);
    /* cifrae 1.2 (sex BearSSL, ECDHE + AEAD) quoque: servus 1.3
     * ignorans unam eligit et ServerHello 1.2 reddit -> descensus
     * (SOLUM_1_2), non handshake_failure (tls13-plan R23). Machina 1.3
     * eas numquam pacisci potest. */
    _scr_u16(&s, 0xc02b);
    _scr_u16(&s, 0xc02f);
    _scr_u16(&s, 0xc02c);
    _scr_u16(&s, 0xc030);
    _scr_u16(&s, 0xcca9);
    _scr_u16(&s, 0xcca8);
    _scr_claudere(&s);
    _scr_u8(&s, I);                              /* compressio nulla */
    _scr_u8(&s, ZEPHYRUM);
    _scr_aperire(&s, II);                        /* extensiones */
    si (   sp->hospes != NIHIL && sp->hospes[ZEPHYRUM] != '\0'
        && !_est_ip(sp->hospes))
    {
        _scr_u16(&s, 0x0000);                    /* server_name */
        _scr_aperire(&s, II);
        _scr_aperire(&s, II);
        _scr_u8(&s, ZEPHYRUM);                   /* host_name */
        _scr_aperire(&s, II);
        _scr_octeti(&s, (constans i8*)sp->hospes,
            (i32)strlen(sp->hospes));
        _scr_claudere(&s);
        _scr_claudere(&s);
        _scr_claudere(&s);
    }
    _scr_u16(&s, 0x000a);                        /* supported_groups */
    _scr_aperire(&s, II);
    _scr_aperire(&s, II);
    _scr_u16(&s, TLS13_GREX_X25519);
    _scr_u16(&s, TLS13_GREX_SECP256R1);
    _scr_claudere(&s);
    _scr_claudere(&s);
    /* signature_algorithms */
    _scr_u16(&s, 0x000d);
    _scr_aperire(&s, II);
    _scr_aperire(&s, II);
    /* ecdsa_secp256r1_sha256 */
    _scr_u16(&s, 0x0403);
    /* ecdsa_secp384r1_sha384 */
    _scr_u16(&s, 0x0503);
    /* rsa_pss_rsae_sha256 */
    _scr_u16(&s, 0x0804);
    _scr_u16(&s, 0x0805);
    _scr_u16(&s, 0x0806);
    /* rsa_pkcs1_sha256/384/512: solum pro servis 1.2 (ServerKeyExchange
     * cum folio RSA) - in 1.3 CertificateVerify eas reicit
     * (_classis_schematis); sine eis servus 1.2 strictus
     * handshake_failure reddit, non ServerHello (recensio I2) */
    _scr_u16(&s, 0x0401);
    _scr_u16(&s, 0x0501);
    _scr_u16(&s, 0x0601);
    _scr_claudere(&s);
    _scr_claudere(&s);
    /* supported_versions */
    _scr_u16(&s, 0x002b);
    _scr_aperire(&s, II);
    _scr_aperire(&s, I);
    _scr_u16(&s, 0x0304);
    _scr_claudere(&s);
    _scr_claudere(&s);
    si (sp->crustulum_n > ZEPHYRUM)
    {
        _scr_u16(&s, 0x002c);                    /* cookie */
        _scr_aperire(&s, II);
        _scr_aperire(&s, II);
        _scr_octeti(&s, sp->crustulum, sp->crustulum_n);
        _scr_claudere(&s);
        _scr_claudere(&s);
    }
    _scr_u16(&s, 0x0033);                        /* key_share */
    _scr_aperire(&s, II);
    _scr_aperire(&s, II);
    _scr_u16(&s, sp->grex);
    _scr_aperire(&s, II);
    _scr_octeti(&s, sp->clavis_publica, sp->clavis_n);
    _scr_claudere(&s);
    _scr_claudere(&s);
    _scr_claudere(&s);
    _scr_claudere(&s);                           /* extensiones */
    _scr_claudere(&s);                           /* corpus */
    si (s.fractus || s.profunditas != ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    redde s.n;
}


/* ================================================== ServerHello */

/* RFC 8446 4.1.3: SHA-256("HelloRetryRequest") */
hic_manens constans i8 HRR_TEMERE[XXXII] = {
    0xcf, 0x21, 0xad, 0x74, 0xe5, 0x9a, 0x61, 0x11, 0xbe, 0x1d, 0x8c,
    0x02, 0x1e, 0x65, 0xb8, 0x91, 0xc2, 0xa2, 0x11, 0x16, 0x7a, 0xbb,
    0x8c, 0x5e, 0x07, 0x9e, 0x09, 0xe2, 0xc8, 0xa8, 0x33, 0x9c };

/* RFC 8446 4.1.3: "DOWNGRD" + 01 (TLS 1.2) aut 00 (antea) */
hic_manens constans i8 DESCENSUS[VII] = { 0x44, 0x4f, 0x57, 0x4e, 0x47,
    0x52, 0x44 };

b32
tls13_responsum_legere (
            constans i8* corpus,
                    i32  n,
    Tls13ResponsumServi* r)
{
         Cursor  c;
         Cursor  sessio;
         Cursor  ext;
    constans i8* temere;
            i32  compressio;
            b32  sv = FALSUM;

    memset(r, ZEPHYRUM, magnitudo(*r));
    _cursor(&c, corpus, n);
    (vacuum)_u16(&c);                            /* legacy_version */
    temere      = _octeti(&c, XXXII);
    sessio      = _vector(&c, I);
    r->cifra    = _u16(&c);
    compressio  = _u8(&c);
    si (   c.fractus || temere == NIHIL || compressio != ZEPHYRUM
        || sessio.mensura > XXXII)
    {
        redde FALSUM;
    }
    r->hrr = memcmp(temere, HRR_TEMERE, XXXII) == ZEPHYRUM;
    r->signum_descensus = memcmp(temere + XXIV, DESCENSUS, VII)
        == ZEPHYRUM
        && (temere[XXXI] == ZEPHYRUM || temere[XXXI] == I);
    r->sessio    = sessio.octeti;
    r->sessio_n  = sessio.mensura;
    si (c.locus == c.mensura)
    {
        /* sine extensionibus */
        r->versio_1_2 = VERUM;
        redde VERUM;
    }
    ext = _vector(&c, II);
    si (!_finitus(&c))
    {
        redde FALSUM;
    }
    dum (!ext.fractus && ext.locus < ext.mensura)
    {
           i32 typus  = _u16(&ext);
        Cursor e      = _vector(&ext, II);

        si (e.fractus)
        {
            redde FALSUM;
        }
        /* supported_versions */
        si (typus == 0x002b)
        {
            r->versio  = _u16(&e);
            sv         = VERUM;
        }
        alioquin si (typus == 0x0033)            /* key_share */
        {
            r->grex = _u16(&e);
            si (!r->hrr)
            {
                Cursor k = _vector(&e, II);

                r->clavis    = k.octeti;
                r->clavis_n  = k.mensura;
            }
        }
        alioquin si (typus == 0x002c)            /* cookie */
        {
            Cursor k = _vector(&e, II);

            r->crustulum    = k.octeti;
            r->crustulum_n  = k.mensura;
        }
        alioquin
        {
            /* ignota praetereuntur */
            e.locus = e.mensura;
        }
        si (!_finitus(&e))
        {
            redde FALSUM;
        }
    }
    si (ext.fractus)
    {
        redde FALSUM;
    }
    r->versio_1_2 = !sv;
    redde VERUM;
}


/* ================================================== EE, Certificate,
 * CertificateVerify, KeyUpdate */

b32
tls13_extensiones_legere (
     constans i8* corpus,
             i32  n)
{
    Cursor c;
    Cursor v;

    _cursor(&c, corpus, n);
    v = _vector(&c, II);
    si (!_finitus(&c))
    {
        redde FALSUM;
    }
    dum (!v.fractus && v.locus < v.mensura)
    {
        (vacuum)_u16(&v);
        (vacuum)_vector(&v, II);
    }
    redde !v.fractus;
}

b32
tls13_certificata_legere (
            constans i8* corpus,
                    i32  n,
    FiduciaCertificatum* exitus,
                    i32  maximum,
                    i32* numerus)
{
    Cursor c;
    Cursor contextus;
    Cursor lista;

    *numerus = ZEPHYRUM;
    _cursor(&c, corpus, n);
    contextus  = _vector(&c, I);
    lista      = _vector(&c, III);
    si (!_finitus(&c) || contextus.mensura != ZEPHYRUM)
    {
        redde FALSUM;
    }
    dum (!lista.fractus && lista.locus < lista.mensura)
    {
        Cursor datum = _vector(&lista, III);

        (vacuum)_vector(&lista, II);             /* extensiones folii */
        si (   lista.fractus || datum.mensura == ZEPHYRUM
            || *numerus >= maximum)
        {
            redde FALSUM;
        }
        exitus[*numerus].octeti   = datum.octeti;
        exitus[*numerus].mensura  = datum.mensura;
        (*numerus)++;
    }
    redde !lista.fractus && *numerus > ZEPHYRUM;
}

b32
tls13_cv_legere (
      constans i8*  corpus,
              i32   n,
              i32*  schema,
      constans i8** subscriptio,
              i32*  subscriptio_n)
{
    Cursor c;
    Cursor s;

    _cursor(&c, corpus, n);
    *schema  = _u16(&c);
    s        = _vector(&c, II);
    si (!_finitus(&c) || s.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    *subscriptio    = s.octeti;
    *subscriptio_n  = s.mensura;
    redde VERUM;
}

b32
tls13_keyupdate_legere (
     constans i8* corpus,
             i32  n,
             b32* rogatur)
{
    si (   n != I
        || (corpus[ZEPHYRUM] != ZEPHYRUM && corpus[ZEPHYRUM] != I))
    {
        redde FALSUM;
    }
    *rogatur = corpus[ZEPHYRUM] == I;
    redde VERUM;
}

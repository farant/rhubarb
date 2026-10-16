/* probatio_tls13_nuntii.c - nuntii pactionis TLS 1.3 (tls13-spec II.1,
 * tls13-plan T3): ServerHello / HelloRetryRequest (cum crustulo vero
 * RFC 8448 §5), EncryptedExtensions, Certificate, CertificateVerify,
 * KeyUpdate legere; ClientHello nostrum scribere et extensiones eius
 * octetim verificare; ServerHello 1.2 cum signo DOWNGRD; verritio
 * truncationis (omnis praefixum fractum FALSUM, numquam ruina).
 * Oraculum: textus RFC immutatus. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "fiducia.h"
#include "tls13_nuntii.h"
#include "rfc8448.h"
#include <string.h>
#include <sys/mman.h>

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

interior constans Rfc8448Campus*
_campus (CredoContextus* c, i32 sectio, character latus,
    constans character* actio, i32 ordo, constans character* campus)
{
    constans Rfc8448Campus* f = rfc8448_quaerere(
        (constans Rfc8448*)c->suita, sectio, latus, actio, ordo,
        campus);

    CREDO_NECESSE_NON_NIHIL(f);
    redde f;
}

/* corpus nuntii sine capite IV octetorum */
interior chorda
_corpus (CredoContextus* c, i32 sectio, character latus,
    constans character* actio, i32 ordo, constans character* campus)
{
    constans Rfc8448Campus* f = _campus(c, sectio, latus, actio, ordo,
        campus);

    CREDO_NECESSE(f->octeti.mensura > IV);
    redde chorda_sectio(f->octeti, IV, f->octeti.mensura);
}

interior i32
_u16 (
    constans i8* o)
{
    redde ((i32)o[ZEPHYRUM] << VIII) | (i32)o[I];
}

interior i32
_u24 (
    constans i8* o)
{
    redde ((i32)o[ZEPHYRUM] << XVI) | ((i32)o[I] << VIII) | (i32)o[II];
}

interior vacuum
_sectio_serverhello (CredoContextus* c)
{
    chorda sh = _corpus(c, III, 's', "construct a ServerHello",
        ZEPHYRUM,
        "ServerHello");
    constans Rfc8448Campus* clavis = _campus(c, III, 's',
        "create an ephemeral x25519 key pair", ZEPHYRUM, "public key");
    Tls13ResponsumServi r;

    CREDO_NECESSE(tls13_responsum_legere(sh.datum, sh.mensura, &r));
    CREDO_FALSUM(r.hrr);
    CREDO_FALSUM(r.versio_1_2);
    CREDO_FALSUM(r.signum_descensus);
    CREDO_AEQUALIS_I32(r.versio, 0x0304);
    CREDO_AEQUALIS_I32(r.cifra, 0x1301);
    CREDO_AEQUALIS_I32(r.grex, TLS13_GREX_X25519);
    CREDO_AEQUALIS_I32(r.sessio_n, ZEPHYRUM);
    CREDO_AEQUALIS_I32(r.clavis_n, XXXII);
    CREDO_VERUM(memcmp(r.clavis, clavis->octeti.datum, XXXII)
        == ZEPHYRUM);
    CREDO_AEQUALIS_I32(r.crustulum_n, ZEPHYRUM);
}

/* §5: HelloRetryRequest petit P-256 et crustulum fert */
interior vacuum
_sectio_hrr (CredoContextus* c)
{
    chorda hrr = _corpus(c, V, 's', "construct a ServerHello", ZEPHYRUM,
        "ServerHello");
    Tls13ResponsumServi r;

    CREDO_NECESSE(tls13_responsum_legere(hrr.datum, hrr.mensura, &r));
    CREDO_VERUM(r.hrr);
    CREDO_FALSUM(r.versio_1_2);
    CREDO_AEQUALIS_I32(r.versio, 0x0304);
    CREDO_AEQUALIS_I32(r.cifra, 0x1301);
    CREDO_AEQUALIS_I32(r.grex, TLS13_GREX_SECP256R1);
    CREDO_AEQUALIS_I32(r.clavis_n, ZEPHYRUM);
    CREDO_AEQUALIS_I32(r.crustulum_n, CXIV);
}

/* index extensionis 'typus' in ClientHello toto (cum capite); -I si
 * abest. Lector proprius probationis, a lib independens. */
interior s32
_extensio (
    constans i8* ch,
            i32  n,
            i32  typus,
            i32* mensura)
{
    i32 k = IV + II + XXXII;
    i32 finis;

    k      += I + (i32)ch[k];                  /* sessio */
    k      += II + (i32)_u16(ch + k);          /* cifrae */
    k      += I + (i32)ch[k];                  /* compressio */
    finis  = k + II + (i32)_u16(ch + k);
    k      += II;
    dum (k + IV <= finis && finis <= n)
    {
        i32 t = _u16(ch + k);
        i32 l = _u16(ch + k + II);

        si (t == typus)
        {
            *mensura = l;
            redde (s32)(k + IV);
        }
        k += IV + l;
    }
    redde -I;
}

/* CH2 cum crustulo ex HRR: extensio cookie octetim aequalis illi
 * ClientHello secundi RFC 8448 §5 (Review focus 2) */
interior vacuum
_sectio_hrr_crustulum (CredoContextus* c)
{
    chorda hrr = _corpus(c, V, 's', "construct a ServerHello", ZEPHYRUM,
        "ServerHello");
    constans Rfc8448Campus* ch2 = _campus(c, V, 'c',
        "construct a ClientHello", I, "ClientHello");
    constans Rfc8448Campus* p256 = _campus(c, V, 'c',
        "create an ephemeral P-256 key pair", ZEPHYRUM, "public key");
        Tls13ResponsumServi r;
    Tls13SalutatioParametri sp;
                         i8 temere[XXXII];
                         i8 exitus[MXXIV];
                        i32 n;
                        i32 l_nostra  = ZEPHYRUM;
                        i32 l_rfc     = ZEPHYRUM;
                        s32 k_nostra;
                        s32 k_rfc;

    CREDO_NECESSE(tls13_responsum_legere(hrr.datum, hrr.mensura, &r));
    memset(temere, 0x11, XXXII);
    memset(&sp, ZEPHYRUM, magnitudo(sp));
    sp.hospes          = "server";
    sp.temere          = temere;
    sp.grex            = TLS13_GREX_SECP256R1;
    sp.clavis_publica  = p256->octeti.datum;
    sp.clavis_n        = p256->octeti.mensura;
    sp.crustulum       = r.crustulum;
    sp.crustulum_n     = r.crustulum_n;
    n                  = tls13_salutatio_scribere(&sp, exitus, MXXIV);
    CREDO_NECESSE(n > ZEPHYRUM);
    k_nostra = _extensio(exitus, n, 0x002c, &l_nostra);
    k_rfc    = _extensio(ch2->octeti.datum, ch2->octeti.mensura, 0x002c,
        &l_rfc);
    CREDO_NECESSE(k_nostra >= ZEPHYRUM && k_rfc >= ZEPHYRUM);
    CREDO_AEQUALIS_I32(l_nostra, CXVI);
    CREDO_AEQUALIS_I32(l_nostra, l_rfc);
    CREDO_VERUM(memcmp(exitus + k_nostra, ch2->octeti.datum + k_rfc,
        l_rfc) == ZEPHYRUM);
    /* key_share CH2: P-256, LXV octeti */
    k_nostra = _extensio(exitus, n, 0x0033, &l_nostra);
    CREDO_NECESSE(k_nostra >= ZEPHYRUM);
    CREDO_AEQUALIS_I32(l_nostra, II + II + II + LXV);
    CREDO_AEQUALIS_I32(_u16(exitus + k_nostra + II),
        TLS13_GREX_SECP256R1);
}

interior vacuum
_sectio_certificata (CredoContextus* c)
{
    constans Rfc8448Campus* f = _campus(c, III, 's',
        "construct a Certificate", ZEPHYRUM, "Certificate");
    FiduciaCertificatum catena[IV];
                 chorda cert;
                    i32 numerus = ZEPHYRUM;

    cert = chorda_sectio(f->octeti, IV, f->octeti.mensura);
    CREDO_NECESSE(tls13_certificata_legere(cert.datum, cert.mensura,
        catena, IV, &numerus));
    CREDO_AEQUALIS_I32(numerus, I);
    /* contextus vacuus (I) + longitudo indicis (III) + longitudo
     * certificati (III) */
    CREDO_AEQUALIS_I32(catena[ZEPHYRUM].mensura, _u24(cert.datum + IV));
    CREDO_VERUM(catena[ZEPHYRUM].octeti == cert.datum + VII);
    CREDO_AEQUALIS_I32((i32)catena[ZEPHYRUM].octeti[ZEPHYRUM], 0x30);
    CREDO_AEQUALIS_I32((i32)catena[ZEPHYRUM].octeti[I], 0x82);
    /* maximum minus quam numerus -> FALSUM */
    CREDO_FALSUM(tls13_certificata_legere(cert.datum, cert.mensura,
        catena, ZEPHYRUM, &numerus));
}

interior vacuum
_sectio_cv (CredoContextus* c)
{
    chorda cv = _corpus(c, III, 's', "construct a CertificateVerify",
        ZEPHYRUM, "CertificateVerify");
    constans i8* subscriptio  = NIHIL;
            i32  schema       = ZEPHYRUM;
            i32  sn           = ZEPHYRUM;

    CREDO_NECESSE(tls13_cv_legere(cv.datum, cv.mensura, &schema,
        &subscriptio, &sn));
    CREDO_AEQUALIS_I32(schema, 0x0804);
    CREDO_AEQUALIS_I32(sn, CXXVIII);
    CREDO_VERUM(subscriptio == cv.datum + IV);
}

interior vacuum
_sectio_ee (CredoContextus* c)
{
    chorda ee = _corpus(c, III, 's', "construct an EncryptedExtensions",
        ZEPHYRUM, "EncryptedExtensions");
    i8 mala[VI];

    CREDO_VERUM(tls13_extensiones_legere(ee.datum, ee.mensura));
    /* extensio cuius longitudo vectorem excedit */
    mala[ZEPHYRUM]  = ZEPHYRUM;
    mala[I]         = IV;
    mala[II]        = ZEPHYRUM;
    mala[III]       = ZEPHYRUM;
    mala[IV]        = ZEPHYRUM;
    mala[V]         = I;
    CREDO_FALSUM(tls13_extensiones_legere(mala, VI));
}

/* extensio ClientHello exspectata: typus et corpus octetim */
nomen structura {
                   i32  typus;
    constans character* hex;
} ExtensioExspectata;

interior i32
_ex_hex (
    constans character* h,
                    i8* o)
{
    i32 n = ZEPHYRUM;

    dum (h[ZEPHYRUM] && h[I])
    {
        constans character* d = "0123456789abcdef";

        o[n++] = (i8)(((i32)(strchr(d, h[ZEPHYRUM]) - d) << IV)
                      | (i32)(strchr(d, h[I]) - d));
        h += II;
    }
    redde n;
}

/* ClientHello nostrum: ordo et corpora extensionum, cifrae, sessio */
interior vacuum
_sectio_salutatio (CredoContextus* c)
{
    constans Rfc8448Campus* clavis = _campus(c, III, 'c',
        "create an ephemeral x25519 key pair", ZEPHYRUM, "public key");
    Tls13SalutatioParametri sp;
                         i8 temere[XXXII];
                         i8 sessio[XXXII];
                         i8 exitus[MXXIV];
                         i8 exspectatum[CXXVIII];
                        i32 n;
                        i32 k;
                        i32 m;
                        i32 finis;
                        i32 e = ZEPHYRUM;
         ExtensioExspectata ext[V];

    ext[ZEPHYRUM].typus  = 0x0000;
    ext[ZEPHYRUM].hex    = "0009000006736572766572";
    ext[I].typus         = 0x000a;
    ext[I].hex           = "0004001d0017";
    ext[II].typus        = 0x000d;
    ext[II].hex          = "000a0403050308040805" "0806";
    ext[III].typus       = 0x002b;
    ext[III].hex         = "020304";
    ext[IV].typus        = 0x0033;
    ext[IV].hex          = "0024001d0020";
    memset(temere, 0x22, XXXII);
    memset(sessio, 0x33, XXXII);
    memset(&sp, ZEPHYRUM, magnitudo(sp));
    sp.hospes          = "server";
    sp.temere          = temere;
    sp.sessio          = sessio;
    sp.sessio_n        = XXXII;
    sp.grex            = TLS13_GREX_X25519;
    sp.clavis_publica  = clavis->octeti.datum;
    sp.clavis_n        = XXXII;
    n                  = tls13_salutatio_scribere(&sp, exitus, MXXIV);
    CREDO_NECESSE(n > XXXVIII);
    CREDO_AEQUALIS_I32((i32)exitus[ZEPHYRUM],
        TLS13_NUNTIUS_CLIENTHELLO);
    CREDO_AEQUALIS_I32(_u24(exitus + I), n - IV);
    CREDO_AEQUALIS_I32(_u16(exitus + IV), 0x0303);
    CREDO_VERUM(memcmp(exitus + VI, temere, XXXII) == ZEPHYRUM);
    k = VI + XXXII;
    CREDO_AEQUALIS_I32((i32)exitus[k], XXXII);
    CREDO_VERUM(memcmp(exitus + k + I, sessio, XXXII) == ZEPHYRUM);
    k += I + XXXII;
    m = _ex_hex("0006130113021303", exspectatum);
    CREDO_VERUM(memcmp(exitus + k, exspectatum, m) == ZEPHYRUM);
    k += m;
    /* compressio nulla */
    CREDO_AEQUALIS_I32(_u16(exitus + k), 0x0100);
    k      += II;
    finis  = k + II + _u16(exitus + k);
    CREDO_AEQUALIS_I32(finis, n);
    k += II;
    dum (k < finis)
    {
        i32 t = _u16(exitus + k);
        i32 l = _u16(exitus + k + II);

        CREDO_NECESSE(e < V);
        CREDO_AEQUALIS_I32(t, ext[e].typus);
        m = _ex_hex(ext[e].hex, exspectatum);
        si (t == 0x0033)
        {
            CREDO_AEQUALIS_I32(l, m + XXXII);
            CREDO_VERUM(memcmp(exitus + k + IV + m,
                clavis->octeti.datum,
                XXXII) == ZEPHYRUM);
        }
        alioquin
        {
            CREDO_AEQUALIS_I32(l, m);
        }
        CREDO_VERUM(memcmp(exitus + k + IV, exspectatum, m)
            == ZEPHYRUM);
        k += IV + l;
        e++;
    }
    CREDO_AEQUALIS_I32(e, V);
    CREDO_AEQUALIS_I32(k, finis);
    /* capacitas deficiens -> 0 */
    CREDO_AEQUALIS_I32(tls13_salutatio_scribere(&sp, exitus, n - I),
        ZEPHYRUM);
}

/* hospes IP: sine server_name (RFC 6066 3) */
interior vacuum
_sectio_ip (CredoContextus* c)
{
    Tls13SalutatioParametri sp;
                         i8 temere[XXXII];
                         i8 clavis[XXXII];
                         i8 exitus[MXXIV];
                        i32 n;
                        i32 l = ZEPHYRUM;

    (vacuum)c;
    memset(temere, I, XXXII);
    memset(clavis, II, XXXII);
    memset(&sp, ZEPHYRUM, magnitudo(sp));
    sp.temere          = temere;
    sp.grex            = TLS13_GREX_X25519;
    sp.clavis_publica  = clavis;
    sp.clavis_n        = XXXII;
    sp.hospes          = "127.0.0.1";
    n                  = tls13_salutatio_scribere(&sp, exitus, MXXIV);
    CREDO_NECESSE(n > ZEPHYRUM);
    CREDO_AEQUALIS_S32(_extensio(exitus, n, 0x0000, &l), -I);
    CREDO_VERUM(_extensio(exitus, n, 0x0033, &l) >= ZEPHYRUM);
    sp.hospes  = "::1";
    n          = tls13_salutatio_scribere(&sp, exitus, MXXIV);
    CREDO_AEQUALIS_S32(_extensio(exitus, n, 0x0000, &l), -I);
    sp.hospes  = "example.com";
    n          = tls13_salutatio_scribere(&sp, exitus, MXXIV);
    CREDO_VERUM(_extensio(exitus, n, 0x0000, &l) >= ZEPHYRUM);
}

/* ServerHello TLS 1.2 manu: legacy 0303, temere finiens DOWNGRD\01,
 * cifra c02c, sine extensionibus */
interior i32
_responsum_vetus (
     i8* o,
    b32  cum_extensione)
{
    i32 n = ZEPHYRUM;
    i32 k;

    o[n++] = (i8)0x03;
    o[n++] = (i8)0x03;
    per (k = ZEPHYRUM; k < XXIV; k++)
    {
        o[n++] = (i8)(k + I);
    }
    memcpy(o + n, "DOWNGRD\001", VIII);
    n       += VIII;
    o[n++]  = ZEPHYRUM;                 /* sessio vacua */
    o[n++]  = (i8)0xc0;
    o[n++]  = (i8)0x2c;
    o[n++]  = ZEPHYRUM;                 /* compressio */
    si (cum_extensione)
    {
        /* renegotiation_info (ff01) vacua */
        o[n++] = ZEPHYRUM;
        o[n++] = (i8)V;
        o[n++] = (i8)0xff;
        o[n++] = (i8)0x01;
        o[n++] = ZEPHYRUM;
        o[n++] = I;
        o[n++] = ZEPHYRUM;
    }
    redde n;
}

interior vacuum
_sectio_versio_1_2 (CredoContextus* c)
{
                     i8 sh[LXIV];
                    i32 n;
    Tls13ResponsumServi r;

    (vacuum)c;
    n = _responsum_vetus(sh, FALSUM);
    CREDO_NECESSE(tls13_responsum_legere(sh, n, &r));
    CREDO_VERUM(r.versio_1_2);
    CREDO_VERUM(r.signum_descensus);
    CREDO_AEQUALIS_I32(r.cifra, 0xc02c);
    n = _responsum_vetus(sh, VERUM);
    CREDO_NECESSE(tls13_responsum_legere(sh, n, &r));
    CREDO_VERUM(r.versio_1_2);
    CREDO_VERUM(r.signum_descensus);
    /* DOWNGRD\02 non est signum */
    sh[XXXIII] = II;
    CREDO_NECESSE(tls13_responsum_legere(sh, n, &r));
    CREDO_FALSUM(r.signum_descensus);
}

interior vacuum
_sectio_keyupdate (CredoContextus* c)
{
     i8 o[II];
    b32 rogatur = FALSUM;

    (vacuum)c;
    o[ZEPHYRUM] = ZEPHYRUM;
    CREDO_VERUM(tls13_keyupdate_legere(o, I, &rogatur));
    CREDO_FALSUM(rogatur);
    o[ZEPHYRUM] = I;
    CREDO_VERUM(tls13_keyupdate_legere(o, I, &rogatur));
    CREDO_VERUM(rogatur);
    o[ZEPHYRUM] = II;
    CREDO_FALSUM(tls13_keyupdate_legere(o, I, &rogatur));
    o[ZEPHYRUM]  = ZEPHYRUM;
    o[I]         = ZEPHYRUM;
    CREDO_FALSUM(tls13_keyupdate_legere(o, II, &rogatur));
    CREDO_FALSUM(tls13_keyupdate_legere(o, ZEPHYRUM, &rogatur));
}

/* VERRITIO: omne praefixum proprium FALSUM reddere debet. Exceptio
 * una: ServerHello truncatum statim post compressionem est ServerHello
 * 1.2 sine extensionibus legitimum (versio_1_2) - 'licitum' id
 * nominat. Reddit numerum praefixorum perperam acceptorum.
 *
 * CUSTODIA (custodita VERUM): praefixum ad finem regionis cui regio
 * NON MAPPATA sequitur (mmap duplex, dimidium posterius munmap) -
 * lectio vel unius octeti ultra finem = SIGSEGV, quem CREDO_NON_RUIT
 * videt. Sine custodia lectio ultra praefixum in reliquum nuntii
 * integri caderet (memoria valida) et lector tamen FALSUM redderet
 * (consumptio exacta): verdictum solum limites non probat (tls13-plan
 * R11). Spatium LXIV KiB = multiplex paginae (4 et 16 KiB). */
nomen enumeratio {
    VERRERE_RESPONSUM = 0, VERRERE_EE, VERRERE_CERT, VERRERE_CV,
    VERRERE_KEYUPDATE
} GenusVerrendi;

#define CUSTODIA_SPATIUM (LXIV * MXXIV)

interior i8*
_custodiam_creare (vacuum)
{
    i8* sedes = (i8*)mmap(NIHIL, (size_t)(II * CUSTODIA_SPATIUM),
        PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -I, ZEPHYRUM);

    si ((vacuum*)sedes == MAP_FAILED)
    {
        redde NIHIL;
    }
    (vacuum)munmap(sedes + CUSTODIA_SPATIUM, (size_t)CUSTODIA_SPATIUM);
    redde sedes;
}

interior i32
_verrere (GenusVerrendi genus, constans i8* integrum, i32 n,
    s32 licitum, b32 custodita)
{
            i32  k;
            i32  mala      = ZEPHYRUM;
             i8* custodia  = NIHIL;
    constans i8* o         = integrum;

    si (custodita)
    {
        custodia = _custodiam_creare();
        si (custodia == NIHIL || n > CUSTODIA_SPATIUM)
        {
            redde n;
        }
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        Tls13ResponsumServi  r;
        FiduciaCertificatum  catena[IV];
                        i32  numerus   = ZEPHYRUM;
                        i32  schema    = ZEPHYRUM;
                constans i8* s         = NIHIL;
                        i32  sn        = ZEPHYRUM;
                        b32  rogatur   = FALSUM;
                        b32  acceptum  = FALSUM;

        si (custodia != NIHIL)
        {
            o = custodia + CUSTODIA_SPATIUM - k;
            memcpy(custodia + CUSTODIA_SPATIUM - k, integrum, k);
        }
        commutatio (genus)
        {
            casus VERRERE_RESPONSUM:
                acceptum = tls13_responsum_legere(o, k, &r);
                si (acceptum && (s32)k == licitum && r.versio_1_2)
                {
                    acceptum = FALSUM;
                }
                frange;
            casus VERRERE_EE:
                acceptum = tls13_extensiones_legere(o, k);
                frange;
            casus VERRERE_CERT:
                acceptum = tls13_certificata_legere(o, k, catena, IV,
                    &numerus);
                frange;
            casus VERRERE_CV:
                acceptum = tls13_cv_legere(o, k, &schema, &s, &sn);
                frange;
            casus VERRERE_KEYUPDATE:
                acceptum = tls13_keyupdate_legere(o, k, &rogatur);
                frange;
        }
        si (acceptum)
        {
            mala++;
        }
    }
    si (custodia != NIHIL)
    {
        (vacuum)munmap(custodia, (size_t)CUSTODIA_SPATIUM);
    }
    redde mala;
}

/* positio post compressionem ServerHello (corpus): 2 + 32 + 1 + sessio
 * + 2 + 1 */
interior s32
_post_compressionem (
    chorda sh)
{
    redde (s32)(XXXV + (i32)sh.datum[XXXIV] + III);
}

interior vacuum
_sectio_praecisio (CredoContextus* c)
{
    chorda sh = _corpus(c, III, 's', "construct a ServerHello",
        ZEPHYRUM,
        "ServerHello");
    chorda hrr = _corpus(c, V, 's', "construct a ServerHello", ZEPHYRUM,
        "ServerHello");
    chorda ee = _corpus(c, III, 's', "construct an EncryptedExtensions",
        ZEPHYRUM, "EncryptedExtensions");
    chorda cert = _corpus(c, III, 's', "construct a Certificate",
        ZEPHYRUM, "Certificate");
    chorda cv = _corpus(c, III, 's', "construct a CertificateVerify",
        ZEPHYRUM, "CertificateVerify");
     i8 vetus[LXIV];
     i8 renovatio[I];
    i32 n_vetus = _responsum_vetus(vetus, VERUM);

    renovatio[ZEPHYRUM] = I;
    /* ruina (lectio ultra finem): in filio, praefixa custodita */
    CREDO_NON_RUIT((vacuum)_verrere(VERRERE_RESPONSUM, sh.datum,
        sh.mensura, -I, VERUM));
    CREDO_NON_RUIT((vacuum)_verrere(VERRERE_RESPONSUM, hrr.datum,
        hrr.mensura, -I, VERUM));
    CREDO_NON_RUIT((vacuum)_verrere(VERRERE_RESPONSUM, vetus, n_vetus,
        -I, VERUM));
    CREDO_NON_RUIT((vacuum)_verrere(VERRERE_EE, ee.datum, ee.mensura,
        -I, VERUM));
    CREDO_NON_RUIT((vacuum)_verrere(VERRERE_CERT, cert.datum,
        cert.mensura, -I, VERUM));
    CREDO_NON_RUIT((vacuum)_verrere(VERRERE_CV, cv.datum, cv.mensura,
        -I, VERUM));
    CREDO_NON_RUIT((vacuum)_verrere(VERRERE_KEYUPDATE, renovatio, I,
        -I, VERUM));
    /* verdictum: in parente, praefixa in nuntio integro */
    CREDO_AEQUALIS_I32(_verrere(VERRERE_RESPONSUM, sh.datum, sh.mensura,
        _post_compressionem(sh), FALSUM), ZEPHYRUM);
    CREDO_AEQUALIS_I32(_verrere(VERRERE_RESPONSUM, hrr.datum,
        hrr.mensura, _post_compressionem(hrr), FALSUM), ZEPHYRUM);
    CREDO_AEQUALIS_I32(_verrere(VERRERE_RESPONSUM, vetus, n_vetus,
        (s32)(n_vetus - VII), FALSUM), ZEPHYRUM);
    CREDO_AEQUALIS_I32(_verrere(VERRERE_EE, ee.datum, ee.mensura, -I,
        FALSUM), ZEPHYRUM);
    CREDO_AEQUALIS_I32(_verrere(VERRERE_CERT, cert.datum, cert.mensura,
        -I, FALSUM), ZEPHYRUM);
    CREDO_AEQUALIS_I32(_verrere(VERRERE_CV, cv.datum, cv.mensura, -I,
        FALSUM), ZEPHYRUM);
    CREDO_AEQUALIS_I32(_verrere(VERRERE_KEYUPDATE, renovatio, I, -I,
        FALSUM), ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "serverhello", _sectio_serverhello, NIHIL, NIHIL, NIHIL, NIHIL },
    { "hrr", _sectio_hrr, NIHIL, NIHIL, NIHIL, NIHIL },
    { "hrr_crustulum", _sectio_hrr_crustulum, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "certificata", _sectio_certificata, NIHIL, NIHIL, NIHIL, NIHIL },
    { "cv", _sectio_cv, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ee", _sectio_ee, NIHIL, NIHIL, NIHIL, NIHIL },
    { "salutatio", _sectio_salutatio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ip", _sectio_ip, NIHIL, NIHIL, NIHIL, NIHIL },
    { "versio_1_2", _sectio_versio_1_2, NIHIL, NIHIL, NIHIL, NIHIL },
    { "keyupdate", _sectio_keyupdate, NIHIL, NIHIL, NIHIL, NIHIL },
    { "praecisio", _sectio_praecisio, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("tls13_nuntii", SECTIONES,
        _suitam_parare, NIHIL);
}

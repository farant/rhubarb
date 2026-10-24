/* fiducia_posix.c - fiducia certificatorum sub POSIX (fiducia-linux-spec
 * II.2-II.3): fasciculus CA distributionis + BearSSL br_x509_minimal.
 *
 * Ordo: argumenta -> indicium diffisarum domus (fiducia_diffisa.h) ->
 * forma DER cuiusque certificati (fiducia_der) -> BearSSL (catena ad
 * ancoram, tempora, nomen DNS) -> SAN solum (BearSSL CN sine SAN
 * adhiberet, vendor/bearssl_amalgama.h:8955) -> hospes IP contra
 * iPAddress SAN (BearSSL id non extrahit).
 *
 * Repositorium: via nota prima exstans (nulla substitutio per ambitum;
 * ancora_via = illud DER SOLUM); obiecta PEM CERTIFICATE -> ancorae,
 * cetera (alia nomina, mala, diffisa) omissa et numerata. Algorithmi:
 * RSA >= MMXLVIII bitorum, ECDSA; digestiones SHA-256/384/512 solae
 * (SHA-1 non registratum). Catena ut missa iudicatur; intermedia
 * absentia non arcessuntur; revocatio nulla (spec IV). */
#include "postulata_posix.h"
#include "../vendor/bearssl_amalgama.h"   /* ANTE latina.h: numeri Romani
                                           * nomina parametrorum vendoris
                                           * frangerent */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "fiducia.h"
#include "fiducia_der.h"
#include "fiducia_diffisa.h"
#include "fiducia_diffisa_tabula.h"
#include "sigillum.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

#define ANCORAE_INITIALES  CCLVI
#define DIES_AD_UNIX       (DCCXIX * M + DXXVIII)  /* 0000-01-01 ..
                                                  * 1970-01-01 */
#define SECUNDA_DIEI       ((s64)(LXXXVI * M + CD))

/* viae notae fasciculi distributionis, ordine (spec II.2) */
hic_manens constans character* constans VIAE_FASCICULI[] = {
    "/etc/ssl/certs/ca-certificates.crt",
    "/etc/pki/tls/certs/ca-bundle.crt",
    "/etc/pki/ca-trust/extracted/pem/tls-ca-bundle.pem",
    "/etc/ssl/ca-bundle.pem",
    "/etc/ssl/cert.pem",
    NIHIL
};

nomen structura {
                 Piscina* piscina;
    br_x509_trust_anchor* ancorae;
                     i32  numerus;
                     i32  capacitas;
                     i32  omissae;
} Repositorium;

/* capsa crescens pro octetis a BearSSL traditis (DN, DER ex PEM) */
nomen structura {
    Piscina* piscina;
         i8* datum;
     size_t  n;
     size_t  capacitas;
} Collector;

interior vacuum
_collectorem_parare (
    Collector* k,
      Piscina* p)
{
    k->piscina    = p;
    k->datum      = NIHIL;
    k->n          = ZEPHYRUM;
    k->capacitas  = ZEPHYRUM;
}

interior vacuum
_colligere (
             vacuum* ctx,
    constans vacuum* buf,
             size_t  len)
{
    Collector* k = (Collector*)ctx;

    si (k->n + len > k->capacitas)
    {
        size_t  nova = k->capacitas ? k->capacitas * II : MMXLVIII;
            i8* d;

        dum (nova < k->n + len)
        {
            nova *= II;
        }
        d = (i8*)piscina_allocare(k->piscina, (memoriae_index)nova);
        si (k->n > ZEPHYRUM)
        {
            memcpy(d, k->datum, k->n);
        }
        k->datum      = d;
        k->capacitas  = nova;
    }
    memcpy(k->datum + k->n, buf, len);
    k->n += len;
}

interior i8*
_copiare (
            Piscina* p,
    constans vacuum* fons,
             size_t  n)
{
    i8* d = (i8*)piscina_allocare(p, (memoriae_index)(n ? n : I));

    si (n > ZEPHYRUM)
    {
        memcpy(d, fons, n);
    }
    redde d;
}

/* DER -> ancora in repositorio (copiae in piscina eius). FALSUM si non
 * certificatum aut diffisum */
interior b32
_ancoram_addere (
    Repositorium* r,
     constans i8* der,
             i32  mensura)
{
     br_x509_decoder_context dc;
                   Collector k;
               br_x509_pkey* clavis_lecta;
        br_x509_trust_anchor* ta;
          FiduciaCertificatum fc;
                          i32 positio;

    fc.octeti   = der;
    fc.mensura  = mensura;
    si (fiducia_diffisam_invenire(&fc, I, fiducia_diffisae,
            fiducia_diffisae_numerus, &positio) >= ZEPHYRUM)
    {
        redde FALSUM;
    }
    _collectorem_parare(&k, r->piscina);
    br_x509_decoder_init(&dc, _colligere, &k);
    br_x509_decoder_push(&dc, der, (size_t)mensura);
    clavis_lecta = br_x509_decoder_get_pkey(&dc);
    si (clavis_lecta == NIHIL)
    {
        redde FALSUM;
    }
    si (r->numerus >= r->capacitas)
    {
        i32 nova = r->capacitas ? r->capacitas * II : ANCORAE_INITIALES;
        br_x509_trust_anchor* n =
            (br_x509_trust_anchor*)piscina_allocare(
            r->piscina, (memoriae_index)nova
                * (memoriae_index)magnitudo(br_x509_trust_anchor));

        si (r->numerus > ZEPHYRUM)
        {
            memcpy(n, r->ancorae, (size_t)r->numerus
                * magnitudo(br_x509_trust_anchor));
        }
        r->ancorae    = n;
        r->capacitas  = nova;
    }
    ta = &r->ancorae[r->numerus];
    memset(ta, ZEPHYRUM, magnitudo(*ta));
    ta->dn.data  = k.datum ? k.datum : _copiare(r->piscina, NIHIL,
        ZEPHYRUM);
    ta->dn.len         = k.n;
    ta->flags          = br_x509_decoder_isCA(&dc) ? BR_X509_TA_CA : 0;
    ta->pkey.key_type  = clavis_lecta->key_type;
    si (clavis_lecta->key_type == BR_KEYTYPE_RSA)
    {
        ta->pkey.key.rsa.n     = _copiare(r->piscina,
            clavis_lecta->key.rsa.n,
            clavis_lecta->key.rsa.nlen);
        ta->pkey.key.rsa.nlen  = clavis_lecta->key.rsa.nlen;
        ta->pkey.key.rsa.e     = _copiare(r->piscina,
            clavis_lecta->key.rsa.e,
            clavis_lecta->key.rsa.elen);
        ta->pkey.key.rsa.elen  = clavis_lecta->key.rsa.elen;
    }
    alioquin si (clavis_lecta->key_type == BR_KEYTYPE_EC)
    {
        ta->pkey.key.ec.curve  = clavis_lecta->key.ec.curve;
        ta->pkey.key.ec.q      = _copiare(r->piscina,
            clavis_lecta->key.ec.q,
            clavis_lecta->key.ec.qlen);
        ta->pkey.key.ec.qlen   = clavis_lecta->key.ec.qlen;
    }
    alioquin
    {
        redde FALSUM;
    }
    r->numerus++;
    redde VERUM;
}

/* PEM -> ancorae; obiecta non CERTIFICATE et mala omissa */
interior vacuum
_textum_ancorarum_legere (
    Repositorium* r,
          chorda  textus)
{
    br_pem_decoder_context  pc;
                 Collector  k;
                       b32  in_certificato  = FALSUM;
               constans i8* p               = textus.datum;
                    size_t  reliquum        = (size_t)textus.mensura;

    /* CRLF: decodificator PEM BearSSL '\r' non tolerat (inventum
     * fiducia-linux T3) - textus sine eo exscribitur */
    si (memchr(textus.datum, '\r', (size_t)textus.mensura) != NIHIL)
    {
         i8* d = (i8*)piscina_allocare(r->piscina,
                     (memoriae_index)textus.mensura);
        i32 j = ZEPHYRUM;
        i32 i;

        per (i = ZEPHYRUM; i < textus.mensura; i++)
        {
            si (textus.datum[i] != '\r')
            {
                d[j++] = textus.datum[i];
            }
        }
        p         = d;
        reliquum  = (size_t)j;
    }
    _collectorem_parare(&k, r->piscina);
    br_pem_decoder_init(&pc);
    dum (reliquum > ZEPHYRUM)
    {
        size_t n = br_pem_decoder_push(&pc, p, reliquum);

        p         += n;
        reliquum  -= n;
        commutatio (br_pem_decoder_event(&pc))
        {
            casus BR_PEM_BEGIN_OBJ:
                in_certificato = strcmp(br_pem_decoder_name(&pc),
                    "CERTIFICATE") == ZEPHYRUM;
                k.n = ZEPHYRUM;
                br_pem_decoder_setdest(&pc, _colligere, &k);
                si (!in_certificato)
                {
                    r->omissae++;
                }
                frange;
            casus BR_PEM_END_OBJ:
                si (in_certificato)
                {
                    si (!_ancoram_addere(r, k.datum, (i32)k.n))
                    {
                        r->omissae++;
                    }
                }
                in_certificato = FALSUM;
                frange;
            casus BR_PEM_ERROR:
                si (in_certificato)
                {
                    r->omissae++;
                }
                in_certificato = FALSUM;
                br_pem_decoder_init(&pc);
                frange;
            ordinarius:
                frange;
        }
    }
}

/* textus PEM in repositorium novum (piscina propria) */
interior b32
_repositorium_struere (
    Repositorium* r,
          chorda  textus)
{
    memset(r, ZEPHYRUM, magnitudo(*r));
    r->piscina = piscina_generare_dynamicum("fiducia_repositorium",
        (memoriae_index)(DXII * MXXIV));
    si (r->piscina == NIHIL)
    {
        redde FALSUM;
    }
    _textum_ancorarum_legere(r, textus);
    redde VERUM;
}

/* fasciculus (via) in repositorium novum; FALSUM si legi nequit */
interior b32
_fasciculum_legere (
          Repositorium* r,
    constans character* via)
{
    Piscina* lectio = piscina_generare_dynamicum("fiducia_lectio",
                          (memoriae_index)(CCLVI * MXXIV));
      chorda textus;
         b32 bene;

    si (lectio == NIHIL)
    {
        redde FALSUM;
    }
    textus = filum_legere_totum(via, lectio);
    bene = textus.mensura > ZEPHYRUM
        && _repositorium_struere(r, textus);
    piscina_destruere(lectio);
    redde bene;
}

/* MEMORIA fasciculi systematis (fiducia-linux T3: lectio plena ~XXVIII
 * ms - horreum connexionem per partem aperit). Clavis: via + mensura +
 * SHA-256 contenti (filum_status tempus mutationis non praebet);
 * quaeque vocatio fasciculum legit et digerit (< I ms) et solum si
 * contentum mutatum est denuo struit. Processus unus, fila nulla. */
hic_manens Repositorium  memoria_repositorii;
hic_manens          b32  memoria_parata = FALSUM;
hic_manens    character  memoria_via[CCLVI];
hic_manens          i32  memoria_mensura = ZEPHYRUM;
hic_manens     Sigillum  memoria_sigillum;

interior constans Repositorium*
_repositorium_systematis (
    constans character* via)
{
    Piscina* lectio = piscina_generare_dynamicum("fiducia_lectio",
                          (memoriae_index)(CCLVI * MXXIV));
      chorda textus;
    Sigillum sig;

    si (lectio == NIHIL)
    {
        redde NIHIL;
    }
    textus = filum_legere_totum(via, lectio);
    si (textus.mensura == ZEPHYRUM || strlen(via) >= CCLVI)
    {
        piscina_destruere(lectio);
        redde NIHIL;
    }
    sig = sigillum_computare(textus.datum,
        (memoriae_index)textus.mensura);
    si (   !memoria_parata || strcmp(memoria_via, via) != ZEPHYRUM
        || memoria_mensura != textus.mensura
        || memcmp(memoria_sigillum.octeti, sig.octeti, SIGILLUM_OCTETI)
            != ZEPHYRUM)
    {
        Repositorium novum;

        si (!_repositorium_struere(&novum, textus))
        {
            piscina_destruere(lectio);
            redde NIHIL;
        }
        si (memoria_parata)
        {
            piscina_destruere(memoria_repositorii.piscina);
        }
        memoria_repositorii  = novum;
        memoria_parata       = VERUM;
        strcpy(memoria_via, via);
        memoria_mensura   = textus.mensura;
        memoria_sigillum  = sig;
    }
    piscina_destruere(lectio);
    redde &memoria_repositorii;
}

interior constans character*
_via_systematis (vacuum)
{
    i32 i;

    per (i = ZEPHYRUM; VIAE_FASCICULI[i] != NIHIL; i++)
    {
        si (filum_existit(VIAE_FASCICULI[i]))
        {
            redde VIAE_FASCICULI[i];
        }
    }
    redde NIHIL;
}

interior vacuum
_causam_ponere (
                chorda* causa,
               Piscina* piscina,
    constans character* textus)
{
    si (causa && piscina)
    {
        *causa = chorda_ex_literis(textus, piscina);
    }
}

interior constans character*
_ratio_bearssl (
    integer erratum)
{
    commutatio (erratum)
    {
        casus BR_ERR_X509_NOT_TRUSTED:
            redde "auctor non fide dignus (ancora ignota)";
        casus BR_ERR_X509_EXPIRED:
            redde "expiratum aut nondum validum";
        casus BR_ERR_X509_BAD_SERVER_NAME:
            redde "nomen hospitis non congruit";
        casus BR_ERR_X509_CRITICAL_EXTENSION:
            redde "extensio critica ignota";
        casus BR_ERR_X509_UNSUPPORTED:
            redde "algorithmus non sustentus (SHA-1 aut ignotus)";
        casus BR_ERR_X509_WEAK_PUBLIC_KEY:
            redde "clavis debilis (RSA < MMXLVIII bitorum)";
        casus BR_ERR_X509_BAD_SIGNATURE:
            redde "signatura mala";
        casus BR_ERR_X509_NOT_CA:
            redde "emittens non est CA";
        casus BR_ERR_X509_FORBIDDEN_KEY_USAGE:
            redde "usus clavis vetitus";
        casus BR_ERR_X509_WRONG_KEY_TYPE:
            redde "genus clavis non congruit";
        casus BR_ERR_X509_DN_MISMATCH:
            redde "catena mala (emittens non congruit)";
        casus BR_ERR_X509_EMPTY_CHAIN:
            redde "catena vacua";
        casus BR_ERR_X509_LIMIT_EXCEEDED:
            redde "catena nimis longa";
        ordinarius:
            redde NIHIL;
    }
}

interior b32
_catenam_iudicare (
       constans FiduciaCertificatum* catena,
                                i32  numerus,
                 constans character* nomen_dns,
              constans Repositorium* r,
                             chorda* causa,
                            Piscina* piscina)
{
    br_x509_minimal_context  contextus;
                        i32  k;
                    integer  erratum;
                  character  nuntius[CCLVI];
         constans character* ratio;

    br_x509_minimal_init(&contextus, &br_sha256_vtable, r->ancorae,
        (size_t)r->numerus);
    br_x509_minimal_set_hash(&contextus, br_sha256_ID,
        &br_sha256_vtable);
    br_x509_minimal_set_hash(&contextus, br_sha384_ID,
        &br_sha384_vtable);
    br_x509_minimal_set_hash(&contextus, br_sha512_ID,
        &br_sha512_vtable);
    br_x509_minimal_set_rsa(&contextus,
        br_rsa_pkcs1_vrfy_get_default());
    br_x509_minimal_set_ecdsa(&contextus, br_ec_get_default(),
        br_ecdsa_vrfy_asn1_get_default());
    br_x509_minimal_set_minrsa(&contextus, CCLVI);
    contextus.vtable->start_chain(&contextus.vtable, nomen_dns);
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        contextus.vtable->start_cert(&contextus.vtable,
            (uint32_t)catena[k].mensura);
        contextus.vtable->append(&contextus.vtable, catena[k].octeti,
            (size_t)catena[k].mensura);
        contextus.vtable->end_cert(&contextus.vtable);
        si (   contextus.err != ZEPHYRUM
            && contextus.err != BR_ERR_X509_OK)
        {
            frange;
        }
    }
    erratum = (integer)contextus.vtable->end_chain(&contextus.vtable);
    si (erratum == ZEPHYRUM)
    {
        redde VERUM;
    }
    ratio = _ratio_bearssl(erratum);
    si (k >= numerus)
    {
        k = numerus - I;
    }
    si (ratio)
    {
        sprintf(nuntius, "certificatum reiectum: %s (catena [%u])",
            ratio,
            (insignatus integer)k);
    }
    alioquin
    {
        sprintf(nuntius, "certificatum reiectum: BearSSL erratum %d "
            "(catena [%u])", erratum, (insignatus integer)k);
    }
    _causam_ponere(causa, piscina, nuntius);
    redde FALSUM;
}

b32
fiducia_iudicare (
    constans FiduciaCertificatum* catena,
                             i32  numerus,
              constans character* hospes,
              constans character* ancora_via,
                          chorda* causa,
                         Piscina* piscina)
{
          Repositorium  r;
                   i32  k;
                    i8  ip[XVI];
                   i32  ip_mensura = ZEPHYRUM;
                   b32  bene;
         FiduciaPartes  folium;
    constans character* defectus = NIHIL;

    memset(&folium, ZEPHYRUM, magnitudo(folium));

    si (causa == NIHIL || piscina == NIHIL)
    {
        redde FALSUM;
    }
    si (catena == NIHIL || numerus == ZEPHYRUM || hospes == NIHIL)
    {
        _causam_ponere(causa, piscina, "fiducia: argumenta invalida");
        redde FALSUM;
    }
    /* indicium diffisarum PRIMUM: causa propria */
    si (!fiducia_diffisas_iudicare(catena, numerus, NIHIL, ZEPHYRUM,
            causa, piscina))
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        FiduciaPartes p;

        si (!fiducia_partes_legere(catena[k].octeti, catena[k].mensura,
                &p, &defectus))
        {
            _causam_ponere(causa, piscina,
                "certificatum reiectum: certificatum catenae non est DER");
            redde FALSUM;
        }
        si (k == ZEPHYRUM)
        {
            folium = p;
        }
    }
    si (inet_pton(AF_INET, hospes, ip) == I)
    {
        ip_mensura = IV;
    }
    alioquin si (inet_pton(AF_INET6, hospes, ip) == I)
    {
        ip_mensura = XVI;
    }

    /* repositorium: ancora sola aut fasciculus systematis */
    si (ancora_via != NIHIL)
    {
        chorda der;

        memset(&r, ZEPHYRUM, magnitudo(r));
        r.piscina  = piscina;
        der        = filum_legere_totum(ancora_via, piscina);
        si (der.mensura == ZEPHYRUM)
        {
            _causam_ponere(causa, piscina,
                "certificatum reiectum: ancora legi non potest");
            redde FALSUM;
        }
        si (!_ancoram_addere(&r, der.datum, der.mensura))
        {
            _causam_ponere(causa, piscina,
                "certificatum reiectum: ancora non est certificatum DER");
            redde FALSUM;
        }
        bene = _catenam_iudicare(catena, numerus,
            ip_mensura ? NIHIL : hospes, &r, causa, piscina);
    }
    alioquin
    {
        constans character* via = _via_systematis();
     constans Repositorium* rs = via ? _repositorium_systematis(via)
                                      : NIHIL;

        si (rs == NIHIL)
        {
            _causam_ponere(causa, piscina, "certificatum reiectum: "
                "fasciculus certificatorum systematis non inventus "
                "(/etc/ssl/certs/ca-certificates.crt, "
                "/etc/pki/tls/certs/ca-bundle.crt, ...)");
            redde FALSUM;
        }
        bene = _catenam_iudicare(catena, numerus,
            ip_mensura ? NIHIL : hospes, rs, causa, piscina);
    }
    si (!bene)
    {
        redde FALSUM;
    }

    /* SAN solum: BearSSL nomen commune sine SAN adhiberet */
    si (!folium.habet_alterna)
    {
        _causam_ponere(causa, piscina, "certificatum reiectum: nomen "
            "hospitis: SAN deest (nomen commune non admittitur)");
        redde FALSUM;
    }
    si (   ip_mensura && !fiducia_alterna_ip_continet(&folium, ip,
            ip_mensura))
    {
        _causam_ponere(causa, piscina, "certificatum reiectum: nomen "
            "hospitis non congruit (IP non in SAN)");
        redde FALSUM;
    }
    redde VERUM;
}

b32
fiducia_fasciculum_describere (
    constans character* via,
                   i32* ancorae,
                   i32* omissae,
                chorda* via_usa,
                chorda* causa,
               Piscina* piscina)
{
          Repositorium  r;
    constans character* v = via ? via : _via_systematis();

    si (ancorae)
    {
        *ancorae = ZEPHYRUM;
    }
    si (omissae)
    {
        *omissae = ZEPHYRUM;
    }
    si (via_usa)
    {
        via_usa->datum    = NIHIL;
        via_usa->mensura  = ZEPHYRUM;
    }
    si (v == NIHIL)
    {
        _causam_ponere(causa, piscina,
            "fasciculus certificatorum systematis non inventus");
        redde FALSUM;
    }
    si (!_fasciculum_legere(&r, v))
    {
        _causam_ponere(causa, piscina, "fasciculus legi non potest");
        redde FALSUM;
    }
    si (ancorae)
    {
        *ancorae = r.numerus;
    }
    si (omissae)
    {
        *omissae = r.omissae;
    }
    si (via_usa && piscina)
    {
        *via_usa = chorda_ex_literis(v, piscina);
    }
    piscina_destruere(r.piscina);
    redde VERUM;
}

b32
fiducia_folium_describere (
    constans FiduciaCertificatum* folium,
                   FiduciaFolium* exitus,
                         Piscina* piscina)
{
    br_x509_decoder_context  dc;
              FiduciaPartes  partes;
              FiduciaSectio  cn;
         constans character* causa = NIHIL;

    si (exitus == NIHIL)
    {
        redde FALSUM;
    }
    memset(exitus, ZEPHYRUM, magnitudo(*exitus));
    si (   folium == NIHIL
        || !fiducia_partes_legere(folium->octeti, folium->mensura,
            &partes, &causa))
    {
        redde FALSUM;
    }
    br_x509_decoder_init(&dc, NIHIL, NIHIL);
    br_x509_decoder_push(&dc, folium->octeti, (size_t)folium->mensura);
    si (br_x509_decoder_get_pkey(&dc) == NIHIL)
    {
        redde FALSUM;
    }
    exitus->non_ante = ((s64)dc.notbefore_days - DIES_AD_UNIX)
        * SECUNDA_DIEI + (s64)dc.notbefore_seconds;
    exitus->non_post = ((s64)dc.notafter_days - DIES_AD_UNIX)
        * SECUNDA_DIEI + (s64)dc.notafter_seconds;
    si (fiducia_nomen_commune(&partes, &cn))
    {
        exitus->subiectum = chorda_ex_buffer(_copiare(piscina,
            cn.octeti,
            (size_t)cn.mensura), cn.mensura);
    }
    exitus->valida = (exitus->non_post > ZEPHYRUM);
    redde exitus->valida;
}

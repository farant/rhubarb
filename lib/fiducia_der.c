/* fiducia_der.c - partes certificati X.509 ex DER (vide caput).
 *
 * Certificate ::= SEQUENCE { tbsCertificate, signatureAlgorithm,
 * signatureValue }; tbsCertificate ::= SEQUENCE { [0] version
 * OPTIONALIS, serialNumber INTEGER, signature, issuer Name, validity,
 * subject Name, subjectPublicKeyInfo, [1] [2] OPTIONALES, [3]
 * extensions OPTIONALES }. Extension ::= SEQUENCE { extnID OID,
 * critical BOOLEAN OPTIONALIS, extnValue OCTET STRING }. SAN = extnID
 * 2.5.29.17 (55 1D 11); extnValue = SEQUENCE OF GeneralName, iPAddress
 * = [7] primitivum (0x87). */
#include "fiducia_der.h"
#include <string.h>

#define TAG_INTEGER      0x02
#define TAG_OCTETI       0x04
#define TAG_IDENT          0x06
#define TAG_SEQUENTIA    0x30
#define TAG_VERSIO       0xA0
#define TAG_EXTENSIONES  0xA3
#define TAG_ALTERNA_IP       0x87

/* TLV unum ad initium p[0..n): tag, contentum, mensura tota. Solum
 * longitudines definitae (<= IV octeti), quaeque intra fines */
interior b32
_elementum (
    constans i8*  p,
            i32   n,
            i32*  tag,
    constans i8** contentum,
            i32*  longitudo,
            i32*  totum)
{
    i32 caput;
    i32 l;
    i32 k;
    i32 i;

    si (p == NIHIL || n < II)
    {
        redde FALSUM;
    }
    *tag = (i32)p[ZEPHYRUM];
    /* numerus tag altus: non in X.509 */
    si ((*tag & 0x1F) == 0x1F)
    {
        redde FALSUM;
    }
    l = (i32)p[I];
    si (l < 0x80)
    {
        caput = II;
    }
    alioquin
    {
        k = l & 0x7F;
        si (k == ZEPHYRUM || k > IV || II + k > n)
        {
            redde FALSUM;           /* indefinita aut nimis longa */
        }
        l = ZEPHYRUM;
        per (i = ZEPHYRUM; i < k; i++)
        {
            si (l > 0x00FFFFFF)
            {
                redde FALSUM;
            }
            l = (l << VIII) | (i32)p[II + i];
        }
        caput = II + k;
    }
    si (l > n - caput)
    {
        redde FALSUM;
    }
    *contentum  = p + caput;
    *longitudo  = l;
    *totum      = caput + l;
    redde VERUM;
}

/* extensiones (contentum SEQUENCE) perlustrat; SAN extnValue reddit
 * si adest. FALSUM si forma mala */
interior b32
_extensiones_legere (
     constans i8*  ext,
             i32   n,
             b32*  habet_alterna,
     constans i8** alterna,
             i32*  alterna_mensura)
{
    *habet_alterna = FALSUM;
    dum (n > ZEPHYRUM)
    {
                 i32  tag;
                 i32  l;
                 i32  totum;
         constans i8* e;
                 i32  el;
                 i32  t2;
         constans i8* ident;
                 i32  ident_longitudo;
                 i32  ident_totum;

        si (   !_elementum(ext, n, &tag, &e, &l, &totum)
            || tag != TAG_SEQUENTIA)
        {
            redde FALSUM;
        }
        el = l;
        si (   !_elementum(e, el, &t2, &ident, &ident_longitudo,
            &ident_totum) || t2 != TAG_IDENT)
        {
            redde FALSUM;
        }
        e   += ident_totum;
        el  -= ident_totum;
        si (el > ZEPHYRUM && (i32)e[ZEPHYRUM] == 0x01)
        {
             constans i8* b;
                     i32  bl;
                     i32  b_totum;

            si (!_elementum(e, el, &t2, &b, &bl, &b_totum))
            {
                redde FALSUM;
            }
            e   += b_totum;
            el  -= b_totum;
        }
        {
             constans i8* v;
                     i32  valor_longitudo;
                     i32  valor_totum;

            si (   !_elementum(e, el, &t2, &v, &valor_longitudo,
                &valor_totum) || t2 != TAG_OCTETI)
            {
                redde FALSUM;
            }
            si (   ident_longitudo == III && ident[ZEPHYRUM] == 0x55
                && ident[I]        == 0x1D && ident[II] == 0x11)
            {
                *habet_alterna    = VERUM;
                *alterna          = v;
                *alterna_mensura  = valor_longitudo;
            }
        }
        ext  += totum;
        n    -= totum;
    }
    redde VERUM;
}

b32
fiducia_partes_legere (
           constans i8*  der,
                   i32   mensura,
         FiduciaPartes*  partes,
    constans character** causa)
{
             i32  tag;
             i32  l;
             i32  totum;
     constans i8* cert;
             i32  cert_longitudo;
     constans i8* signandum;
             i32  n;
     constans i8* c;
     constans i8* alterna;
             i32  alterna_mensura;

    memset(partes, ZEPHYRUM, magnitudo(*partes));
    si (   !_elementum(der, mensura, &tag, &cert, &cert_longitudo,
        &totum)
        || tag != TAG_SEQUENTIA || totum != mensura)
    {
        *causa = "certificatum non est DER (SEQUENCE exterior)";
        redde FALSUM;
    }
    si (   !_elementum(cert, cert_longitudo, &tag, &signandum, &n,
        &totum)
        || tag != TAG_SEQUENTIA)
    {
        *causa = "certificatum non est DER (tbsCertificate)";
        redde FALSUM;
    }
    /* [0] versio optionalis */
    si (n > ZEPHYRUM && (i32)signandum[ZEPHYRUM] == TAG_VERSIO)
    {
        si (!_elementum(signandum, n, &tag, &c, &l, &totum))
        {
            *causa = "certificatum non est DER (versio)";
            redde FALSUM;
        }
        signandum  += totum;
        n          -= totum;
    }
    si (   !_elementum(signandum, n, &tag, &c, &l, &totum)
        || tag != TAG_INTEGER
        || l   == ZEPHYRUM)
    {
        *causa = "certificatum non est DER (series)";
        redde FALSUM;
    }
    partes->series.octeti   = c;
    partes->series.mensura  = l;
    signandum               += totum;
    n                       -= totum;
    /* signature AlgorithmIdentifier */
    si (   !_elementum(signandum, n, &tag, &c, &l, &totum)
        || tag != TAG_SEQUENTIA)
    {
        *causa = "certificatum non est DER (algorithmus)";
        redde FALSUM;
    }
    signandum  += totum;
    n          -= totum;
    si (   !_elementum(signandum, n, &tag, &c, &l, &totum)
        || tag != TAG_SEQUENTIA)
    {
        *causa = "certificatum non est DER (emittens)";
        redde FALSUM;
    }
    partes->emittens.octeti   = signandum;
    partes->emittens.mensura  = totum;
    signandum                 += totum;
    n                         -= totum;
    si (   !_elementum(signandum, n, &tag, &c, &l, &totum)
        || tag != TAG_SEQUENTIA)
    {
        *causa = "certificatum non est DER (validitas)";
        redde FALSUM;
    }
    signandum  += totum;
    n          -= totum;
    si (   !_elementum(signandum, n, &tag, &c, &l, &totum)
        || tag != TAG_SEQUENTIA)
    {
        *causa = "certificatum non est DER (subiectum)";
        redde FALSUM;
    }
    partes->subiectum.octeti   = signandum;
    partes->subiectum.mensura  = totum;
    signandum                  += totum;
    n                          -= totum;
    si (   !_elementum(signandum, n, &tag, &c, &l, &totum)
        || tag != TAG_SEQUENTIA)
    {
        *causa = "certificatum non est DER (clavis publica)";
        redde FALSUM;
    }
    partes->clavis_publica.octeti   = signandum;
    partes->clavis_publica.mensura  = totum;
    signandum                       += totum;
    n                               -= totum;
    /* [1] [2] identificatores unici optionales, [3] extensiones */
    dum (n > ZEPHYRUM)
    {
        si (!_elementum(signandum, n, &tag, &c, &l, &totum))
        {
            *causa = "certificatum non est DER (campi optionales)";
            redde FALSUM;
        }
        si (tag == TAG_EXTENSIONES)
        {
             constans i8* s;
                     i32  sl;
                     i32  st;

            si (   !_elementum(c, l, &tag, &s, &sl, &st)
                || tag != TAG_SEQUENTIA
                || !_extensiones_legere(s, sl, &partes->habet_alterna,
                &alterna,
                    &alterna_mensura))
            {
                *causa = "certificatum non est DER (extensiones)";
                memset(partes, ZEPHYRUM, magnitudo(*partes));
                redde FALSUM;
            }
            partes->extensiones.octeti   = s;
            partes->extensiones.mensura  = sl;
        }
        signandum  += totum;
        n          -= totum;
    }
    redde VERUM;
}

b32
fiducia_alterna_ip_continet (
    constans FiduciaPartes* partes,
               constans i8* ip,
                       i32  mensura)
{
             b32  habet;
     constans i8* alterna  = NIHIL;
             i32  sl       = ZEPHYRUM;
             i32  tag;
     constans i8* nomina;
             i32  n;
             i32  totum;

    si (   partes == NIHIL || ip == NIHIL
        || (mensura != IV && mensura != XVI)
        || !_extensiones_legere(partes->extensiones.octeti,
            partes->extensiones.mensura, &habet, &alterna, &sl)
        || !habet)
    {
        redde FALSUM;
    }
    si (   !_elementum(alterna, sl, &tag, &nomina, &n, &totum)
        || tag != TAG_SEQUENTIA)
    {
        redde FALSUM;
    }
    dum (n > ZEPHYRUM)
    {
         constans i8* v;
                 i32  valor_longitudo;

        si (!_elementum(nomina, n, &tag, &v, &valor_longitudo, &totum))
        {
            redde FALSUM;
        }
        si (   tag == TAG_ALTERNA_IP && valor_longitudo == mensura
            && memcmp(v, ip, (size_t)mensura) == ZEPHYRUM)
        {
            redde VERUM;
        }
        nomina  += totum;
        n       -= totum;
    }
    redde FALSUM;
}

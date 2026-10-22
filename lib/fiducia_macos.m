/* fiducia_macos.m - fiducia.h per Security.framework (tls-bearssl T2)
 *
 * SecCertificate ex quoque DER, SecTrust cum politia SSL nomen hospitis
 * ferente (SecPolicyCreateSSL), ancora SOLA si data,
 * SecTrustEvaluateWithError; causa ex CFError nominata. Ex tls_macos.m
 * translata: lectio ancorae, iudicium SecTrust (politia expressa),
 * _tempus_ex_valore, _proprietatem_capere. Indicium diffisarum domus
 * (fiducia_diffisa.h, fiducia-linux T2) ANTE SecTrust consulitur.
 *
 * NOTA: capita systematis ANTE latina.h ('character' macro cum
 * CoreFoundation confligit). */
#import <Security/Security.h>
#include <CoreFoundation/CoreFoundation.h>
#include <string.h>

#include "fiducia.h"
#include "fiducia_diffisa.h"
#include "filum.h"

interior SecCertificateRef
_certificatum_ex_der (
    constans i8* octeti,
            i32  mensura)
{
            CFDataRef  data;
    SecCertificateRef  cert;

    si (octeti == NIHIL || mensura == 0)
    {
        redde NULL;
    }
    data = CFDataCreate(NULL, (constans UInt8*)octeti, (CFIndex)mensura);
    si (data == NULL)
    {
        redde NULL;
    }
    cert = SecCertificateCreateWithData(NULL, data);
    CFRelease(data);
    redde cert;
}

/* ancora DER ex via (per filum: fabrica lectionem videt); NULL + *causa
 * si nequit */
interior SecCertificateRef
_ancoram_legere (
     constans character*  via,
                 Piscina* piscina,
    constans character** causa)
{
               chorda  der = filum_legere_totum(via, piscina);
    SecCertificateRef  cert;

    si (der.mensura == 0)
    {
        *causa = "ancora legi non potest";
        redde NULL;
    }
    cert = _certificatum_ex_der(der.datum, der.mensura);
    si (cert == NULL)
    {
        *causa = "ancora non est certificatum DER";
    }
    redde cert;
}

/* "<praefixum>: <descriptio CFError>" in piscina */
interior chorda
_causa (
    constans character* praefixum,
             CFErrorRef erratum,
               Piscina* piscina)
{
      character  nuntius[CCLVI];
    CFStringRef  descriptio = erratum ? CFErrorCopyDescription(erratum)
                                      : NULL;

    strcpy(nuntius, praefixum);
    si (descriptio)
    {
        strcat(nuntius, ": ");
        CFStringGetCString(descriptio, nuntius + strlen(nuntius),
            (CFIndex)(sizeof(nuntius) - strlen(nuntius)),
            kCFStringEncodingUTF8);
        CFRelease(descriptio);
    }
    redde chorda_ex_literis(nuntius, piscina);
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
      CFMutableArrayRef  certa = NULL;
            CFStringRef  hospes_cf = NULL;
           SecPolicyRef  politia = NULL;
            SecTrustRef  fiducia = NULL;
             CFErrorRef  erratum = NULL;
    constans character*  defectus = NIHIL;
                    b32  bene = FALSUM;
                    i32  i;

    si (causa == NIHIL || piscina == NIHIL)
    {
        redde FALSUM;
    }
    si (catena == NIHIL || numerus == 0 || hospes == NIHIL)
    {
        *causa = chorda_ex_literis("fiducia: argumenta invalida", piscina);
        redde FALSUM;
    }
    /* indicium diffisarum domus PRIMUM (fiducia-linux T2): diffisa
     * causam suam fert, ante iudicium SecTrust */
    si (!fiducia_diffisas_iudicare(catena, (i32)numerus, NIHIL, 0, causa,
            piscina))
    {
        redde FALSUM;
    }
    certa     = CFArrayCreateMutable(NULL, (CFIndex)numerus,
        &kCFTypeArrayCallBacks);
    hospes_cf = CFStringCreateWithCString(NULL, hospes,
        kCFStringEncodingUTF8);
    si (certa == NULL || hospes_cf == NULL)
    {
        defectus = "fiducia: allocatio CoreFoundation fallita";
    }
    per (i = 0; defectus == NIHIL && i < numerus; i++)
    {
        SecCertificateRef c = _certificatum_ex_der(catena[i].octeti,
            catena[i].mensura);

        si (c == NULL)
        {
            defectus = "certificatum catenae non est DER";
            frange;
        }
        CFArrayAppendValue(certa, c);
        CFRelease(c);
    }
    si (defectus == NIHIL)
    {
        politia = SecPolicyCreateSSL(true, hospes_cf);
        si (SecTrustCreateWithCertificates(certa, politia, &fiducia)
            != errSecSuccess)
        {
            defectus = "SecTrustCreateWithCertificates fallita";
        }
    }
    si (defectus == NIHIL && ancora_via != NIHIL)
    {
        SecCertificateRef ancora = _ancoram_legere(ancora_via, piscina,
            &defectus);

        si (ancora != NULL)
        {
            constans void* valores[I];
                CFArrayRef ancorae;

            valores[0] = ancora;
            ancorae = CFArrayCreate(NULL, valores, 1,
                &kCFTypeArrayCallBacks);
            CFRelease(ancora);
            si (ancorae == NULL)
            {
                defectus = "ancora: CFArrayCreate fallita";
            }
            alioquin
            {
                SecTrustSetAnchorCertificates(fiducia, ancorae);
                SecTrustSetAnchorCertificatesOnly(fiducia, true);
                CFRelease(ancorae);
            }
        }
    }
    si (defectus != NIHIL)
    {
        *causa = chorda_ex_literis(defectus, piscina);
    }
    alioquin
    {
        bene = SecTrustEvaluateWithError(fiducia, &erratum) ? VERUM
                                                             : FALSUM;
        si (!bene)
        {
            *causa = _causa("certificatum reiectum", erratum, piscina);
        }
    }
    si (erratum)
    {
        CFRelease(erratum);
    }
    si (fiducia)
    {
        CFRelease(fiducia);
    }
    si (politia)
    {
        CFRelease(politia);
    }
    si (hospes_cf)
    {
        CFRelease(hospes_cf);
    }
    si (certa)
    {
        CFRelease(certa);
    }
    redde bene;
}

/* CFAbsoluteTime (secunda ab 2001-01-01) -> epocha unix.
 * Valores validitatis ex SecCertificateCopyValues ut CFNumber
 * (absolutum) plerumque veniunt, ut CFDate interdum - ambo
 * tractantur ne versio systematis nos frangat. */
interior s64
_tempus_ex_valore (
    CFTypeRef valor)
{
    si (valor == NULL)
    {
        redde 0;
    }
    si (CFGetTypeID(valor) == CFNumberGetTypeID())
    {
        double absolutum = 0.0;

        si (CFNumberGetValue((CFNumberRef)valor, kCFNumberDoubleType,
                &absolutum))
        {
            redde (s64)(absolutum + kCFAbsoluteTimeIntervalSince1970);
        }
        redde 0;
    }
    si (CFGetTypeID(valor) == CFDateGetTypeID())
    {
        redde (s64)(CFDateGetAbsoluteTime((CFDateRef)valor)
            + kCFAbsoluteTimeIntervalSince1970);
    }
    redde 0;
}

/* valorem ex dictionario valorum certificati:
 * { OID: { kSecPropertyKeyValue: <valor> } } */
interior CFTypeRef
_proprietatem_capere (
    CFDictionaryRef valores,
          CFTypeRef oid)
{
    CFDictionaryRef proprietas;

    si (valores == NULL)
    {
        redde NULL;
    }
    proprietas = (CFDictionaryRef)CFDictionaryGetValue(valores, oid);
    si (proprietas == NULL
        || CFGetTypeID(proprietas) != CFDictionaryGetTypeID())
    {
        redde NULL;
    }
    redde CFDictionaryGetValue(proprietas, kSecPropertyKeyValue);
}

b32
fiducia_folium_describere (
    constans FiduciaCertificatum* folium,
                   FiduciaFolium* exitus,
                         Piscina* piscina)
{
    SecCertificateRef  cert;
      CFDictionaryRef  valores = NULL;
           CFArrayRef  claves;
            CFTypeRef  oids[II];
          CFStringRef  summarium;

    si (exitus == NIHIL)
    {
        redde FALSUM;
    }
    memset(exitus, 0, sizeof(*exitus));
    cert = folium ? _certificatum_ex_der(folium->octeti, folium->mensura)
                  : NULL;
    si (cert == NULL)
    {
        redde FALSUM;
    }
    oids[0] = kSecOIDX509V1ValidityNotBefore;
    oids[I] = kSecOIDX509V1ValidityNotAfter;
    claves  = CFArrayCreate(NULL, oids, II, &kCFTypeArrayCallBacks);
    si (claves != NULL)
    {
        valores = SecCertificateCopyValues(cert, claves, NULL);
        CFRelease(claves);
    }
    si (valores != NULL)
    {
        exitus->non_ante = _tempus_ex_valore(_proprietatem_capere(valores,
            kSecOIDX509V1ValidityNotBefore));
        exitus->non_post = _tempus_ex_valore(_proprietatem_capere(valores,
            kSecOIDX509V1ValidityNotAfter));
        CFRelease(valores);
    }
    summarium = SecCertificateCopySubjectSummary(cert);
    si (summarium != NULL)
    {
        character capsa[CCLVI];

        si (CFStringGetCString(summarium, capsa, (CFIndex)CCLVI,
                kCFStringEncodingUTF8))
        {
            exitus->subiectum = chorda_ex_literis(capsa, piscina);
        }
        CFRelease(summarium);
    }
    CFRelease(cert);
    exitus->valida = (exitus->non_post > 0);
    redde exitus->valida;
}

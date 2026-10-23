/* fiducia_vivum.c - probatio VIVA fiduciae (fiducia-linux-plan T4):
 * connexiones TLS verae (vectura ex RHUBARB_TLS aut ordinaria) ad
 * hospites datos, repositorio systematis.
 *
 * Usus: fiducia_vivum ARGUMENTUM...
 *   +hospes[:portus]            accipi debet
 *   -hospes[:portus]=fragmentum reici debet, causa fragmentum continens
 *   ~hospes[:portus]            solum nuntiatur (e.g. revocatum: revocatio
 *                               non iudicatur, spec IV)
 *
 * Effusio: linea capitis (fasciculus, ancorae, omissae, diffisae et
 * dies instantaneae), deinde linea per hospitem:
 *   hospes <TAB> bene|REIECTUM <TAB> subiectum aut causa <TAB> ut
 *   exspectatum|CONTRA EXSPECTATUM
 * Exitus: 0 omnia ut exspectata; I aliquid contra; II usus.
 * Instrumentum manu (rete!), non porta. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "tls.h"
#include "fiducia.h"
#include "fiducia_diffisa.h"
#include "fiducia_diffisa_tabula.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

interior b32
_continet (
                 chorda  c,
     constans character* fragmentum)
{
    i32 k = (i32)strlen(fragmentum);
    i32 i;

    per (i = ZEPHYRUM; i + k <= c.mensura; i++)
    {
        si (memcmp(c.datum + i, fragmentum, (size_t)k) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* "+hospes:portus=fragmentum" -> partes; FALSUM si forma mala */
interior b32
_argumentum (
    constans character* a,
             character* signum,
             character* hospes,
                   i32* portus,
             character* fragmentum)
{
     constans character* aequale;
     constans character* colon;
                 size_t  n;

    si (a[ZEPHYRUM] != '+' && a[ZEPHYRUM] != '-' && a[ZEPHYRUM] != '~')
    {
        redde FALSUM;
    }
    *signum = a[ZEPHYRUM];
    a++;
    aequale               = strchr(a, '=');
    fragmentum[ZEPHYRUM]  = '\0';
    si (aequale)
    {
        si (strlen(aequale + I) >= CCLVI)
        {
            redde FALSUM;
        }
        strcpy(fragmentum, aequale + I);
    }
    n = aequale ? (size_t)(aequale - a) : strlen(a);
    si (n == ZEPHYRUM || n >= CCLVI)
    {
        redde FALSUM;
    }
    memcpy(hospes, a, n);
    hospes[n]  = '\0';
    *portus    = CDXLIII;
    colon      = strchr(hospes, ':');
    si (colon)
    {
        *portus                 = (i32)atoi(colon + I);
        hospes[colon - hospes]  = '\0';
    }
    redde *portus > ZEPHYRUM && *portus < (i32)LXV * M + DXXXVI;
}

s32
principale (
      integer   argc,
    character** argv)
{
     Piscina* p;
     integer  i;
         s32  exitus   = ZEPHYRUM;
         i32  ancorae  = ZEPHYRUM;
         i32  omissae  = ZEPHYRUM;
      chorda  via;
      chorda  causa;

    si (argc < II)
    {
        fprintf(stderr,
            "usus: fiducia_vivum +hospes | -hospes=fragmentum"
            " | ~hospes ...\n");
        redde II;
    }
    p = piscina_generare_dynamicum("fiducia_vivum",
        (memoriae_index)(DXII * MXXIV));
    si (fiducia_fasciculum_describere(NIHIL, &ancorae, &omissae, &via,
            &causa, p))
    {
        imprimere("fasciculus %.*s: ancorae %u, omissae %u; diffisae %u"
            " (OneCRL %s); vectura %s\n", (integer)via.mensura,
            (constans character*)via.datum, (insignatus integer)ancorae,
            (insignatus integer)omissae,
            (insignatus integer)fiducia_diffisae_numerus,
            fiducia_diffisae_tempus,
            getenv("RHUBARB_TLS") ? getenv("RHUBARB_TLS") : "ordinaria");
    }
    alioquin
    {
        imprimere("fasciculus: %.*s\n", (integer)causa.mensura,
            (constans character*)causa.datum);
    }
    per (i = I; i < argc; i++)
    {
          character signum;
          character hospes[CCLVI];
          character fragmentum[CCLVI];
                i32 portus;
        TlsOptiones o = tls_optiones_default();
        TlsResultus r;
                b32 ut_exspectatum;

        si (!_argumentum(argv[i], &signum, hospes, &portus, fragmentum))
        {
            fprintf(stderr, "fiducia_vivum: argumentum malum: %s\n",
                argv[i]);
            redde II;
        }
        o.timeout_ms = XV * M;
        r = tls_connectere_cum_optionibus(hospes, portus, &o, p);
        si (r.successus)
        {
            TlsCertificatum cert;

            memset(&cert, ZEPHYRUM, magnitudo(cert));
            (vacuum)tls_certificatum_obtinere(r.connexio, &cert);
            ut_exspectatum = (signum != '-');
            imprimere("%s\tbene\t%.*s\t%s\n", hospes,
                (integer)cert.subiectum.mensura,
                (constans character*)cert.subiectum.datum,
                signum == '~' ? "nuntiatum"
                : (ut_exspectatum ? "ut exspectatum"
                                  : "CONTRA EXSPECTATUM"));
            tls_claudere(r.connexio);
        }
        alioquin
        {
            ut_exspectatum = (signum == '~') || (signum == '-'
                && (fragmentum[ZEPHYRUM] == '\0'
                    || _continet(r.error_descriptio, fragmentum)));
            imprimere("%s\tREIECTUM\t%.*s\t%s\n", hospes,
                (integer)r.error_descriptio.mensura,
                (constans character*)r.error_descriptio.datum,
                signum == '~' ? "nuntiatum"
                : (ut_exspectatum ? "ut exspectatum"
                                  : "CONTRA EXSPECTATUM"));
        }
        si (!ut_exspectatum)
        {
            exitus = I;
        }
    }
    piscina_destruere(p);
    redde exitus;
}

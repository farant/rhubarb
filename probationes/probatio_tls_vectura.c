/* probatio_tls_vectura.c - electio vecturae TLS (tls-bearssl-spec II.5,
 * R7): optiones explicitae vincunt; ORDINARIA -> RHUBARB_TLS
 * ('bearssl' | 'securetransport'), aliter ordinaria domus; connexio
 * vecturam suam meminit. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "processus.h"
#include "tls.h"
#include "servus_tls.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ordinaria domus: T3 SECURETRANSPORT, T6 BEARSSL */
#define VECTURA_DOMUS TLS_VECTURA_SECURETRANSPORT

nomen structura {
    character dir[DXII];
    character fixa[DXII];
    character ancora[DXII];
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

    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_NECESSE_NON_NIHIL(area);
    strcpy(l->dir, area);
    sprintf(l->fixa, "%s/certa", area);
    sprintf(l->ancora, "%s/ca.der", l->fixa);
    argumenta[ZEPHYRUM] = "probationes/fixa/tls/generare.sh";
    argumenta[I] = l->fixa;
    argumenta[II] = NIHIL;
    pr = processus_exsequi(argumenta, XXX * M, c->piscina);
    CREDO_NECESSE(pr.successus && pr.codex_exitus == ZEPHYRUM);
    c->datum = l;
}

/* ambitus mundus post omnem sectionem */
interior vacuum
_purgare (
    CredoContextus* c)
{
    (vacuum)c;
    (vacuum)unsetenv("RHUBARB_TLS");
}

interior vacuum
_sectio_ordinaria (
    CredoContextus* c)
{
    TlsOptiones opt = tls_optiones_default();

    (vacuum)c;
    CREDO_AEQUALIS_I32((i32)opt.vectura, (i32)TLS_VECTURA_ORDINARIA);
    (vacuum)unsetenv("RHUBARB_TLS");
    CREDO_AEQUALIS_I32((i32)tls_vecturam_eligere(&opt),
        (i32)VECTURA_DOMUS);
    CREDO_AEQUALIS_I32((i32)tls_vecturam_eligere(NIHIL),
        (i32)VECTURA_DOMUS);
}

interior vacuum
_sectio_ambitus (
    CredoContextus* c)
{
    TlsOptiones opt = tls_optiones_default();

    (vacuum)c;
    (vacuum)setenv("RHUBARB_TLS", "securetransport", I);
    CREDO_AEQUALIS_I32((i32)tls_vecturam_eligere(&opt),
        (i32)TLS_VECTURA_SECURETRANSPORT);
    (vacuum)setenv("RHUBARB_TLS", "bearssl", I);
    CREDO_AEQUALIS_I32((i32)tls_vecturam_eligere(&opt),
        (i32)TLS_VECTURA_BEARSSL);
    (vacuum)setenv("RHUBARB_TLS", "nugae", I);
    CREDO_AEQUALIS_I32((i32)tls_vecturam_eligere(&opt),
        (i32)VECTURA_DOMUS);
    /* explicita vincit ambitum */
    (vacuum)setenv("RHUBARB_TLS", "bearssl", I);
    opt.vectura = TLS_VECTURA_SECURETRANSPORT;
    CREDO_AEQUALIS_I32((i32)tls_vecturam_eligere(&opt),
        (i32)TLS_VECTURA_SECURETRANSPORT);
}

interior vacuum
_sectio_meminit (
    CredoContextus* c)
{
    constans Locus* l = (constans Locus*)c->suita;
         ServusTls  s;
       TlsOptiones  opt = tls_optiones_default();
       TlsResultus  tr;

    CREDO_AEQUALIS_I32((i32)tls_vectura(NIHIL),
        (i32)TLS_VECTURA_ORDINARIA);
    opt.ancora_via  = l->ancora;
    opt.timeout_ms  = V * M;
    opt.vectura     = TLS_VECTURA_SECURETRANSPORT;
    CREDO_NECESSE(servum_tls_incipere(&s, l->dir, l->fixa, "localhost",
        c->piscina));
    tr = tls_connectere_cum_optionibus("localhost", s.portus, &opt,
        c->piscina);
    CREDO_NECESSE(tr.successus);
    CREDO_AEQUALIS_I32((i32)tls_vectura(tr.connexio),
        (i32)TLS_VECTURA_SECURETRANSPORT);
    tls_claudere(tr.connexio);
    servum_tls_claudere(&s);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "ordinaria", _sectio_ordinaria, NIHIL, _purgare, NIHIL, NIHIL },
    { "ambitus", _sectio_ambitus, NIHIL, _purgare, NIHIL, NIHIL },
    { "meminit", _sectio_meminit, NIHIL, _purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("tls_vectura", SECTIONES,
        _suitam_parare, NIHIL);
}

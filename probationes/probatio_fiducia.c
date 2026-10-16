/* probatio_fiducia.c - fiducia_iudicare / fiducia_folium_describere
 * contra certificata probationis IN TEMPORE generata
 * (probationes/fixa/tls/generare.sh): ancora sola (folia EC et RSA,
 * nomen et IP), hospes alienus, expiratus, CA ignota, repositorium
 * systematis (CA probationis ignorat), non-DER, ancora absens;
 * summarium folii. Sine rete. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "filum.h"
#include "processus.h"
#include "fiducia.h"
#include <stdio.h>
#include <string.h>

nomen structura {
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
    sprintf(l->fixa, "%s/certa", area);
    sprintf(l->ancora, "%s/ca.der", l->fixa);
    argumenta[ZEPHYRUM] = "probationes/fixa/tls/generare.sh";
    argumenta[I] = l->fixa;
    argumenta[II] = NIHIL;
    pr = processus_exsequi(argumenta, XXX * M, c->piscina);
    CREDO_NECESSE(pr.successus && pr.codex_exitus == ZEPHYRUM);
    c->datum = l;
}

/* <titulus>.der -> FiduciaCertificatum in piscina sectionis */
interior FiduciaCertificatum
_legere (
         CredoContextus* c,
     constans character* titulus)
{
         constans Locus* l = (constans Locus*)c->suita;
    FiduciaCertificatum  fc;
              character  via[DXII];
                 chorda  der;

    sprintf(via, "%s/%s.der", l->fixa, titulus);
    der         = filum_legere_totum(via, c->piscina);
    fc.octeti   = der.datum;
    fc.mensura  = der.mensura;
    redde fc;
}

/* iudicium unius folii (ancora probationis aut repositorium) */
interior b32
_iudicare (
         CredoContextus* c,
    constans character* titulus,
    constans character* hospes,
                   b32  cum_ancora,
                chorda* causa)
{
         constans Locus* l   = (constans Locus*)c->suita;
    FiduciaCertificatum  fc  = _legere(c, titulus);

    CREDO_NECESSE(fc.mensura > ZEPHYRUM);
    redde fiducia_iudicare(&fc, I, hospes,
        cum_ancora ? l->ancora : NIHIL,
        causa, c->piscina);
}

interior b32
_continet (
                 chorda  c,
     constans character* fragmentum)
{
    i32 k = (i32)strlen(fragmentum);
    i32 i;

    per (i = ZEPHYRUM; i + k <= c.mensura; i++)
    {
        si (memcmp(c.datum + i, fragmentum, k) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
_sectio_ancora (
    CredoContextus* c)
{
    chorda causa;

    causa.datum    = NIHIL;
    causa.mensura  = ZEPHYRUM;
    CREDO_VERUM(_iudicare(c, "localhost", "localhost", VERUM, &causa));
    CREDO_VERUM(_iudicare(c, "localhost_rsa", "localhost", VERUM,
        &causa));
    CREDO_VERUM(_iudicare(c, "localhost", "127.0.0.1", VERUM, &causa));
}

interior vacuum
_reiectum (
         CredoContextus* c,
    constans character* titulus,
                   b32  cum_ancora)
{
    chorda causa;

    causa.datum    = NIHIL;
    causa.mensura  = ZEPHYRUM;
    CREDO_FALSUM(_iudicare(c, titulus, "localhost", cum_ancora,
        &causa));
    CREDO_VERUM(_continet(causa, "certificatum reiectum"));
}

interior vacuum
_sectio_alienus (
    CredoContextus* c)
{
    _reiectum(c, "alienus", VERUM);
}

interior vacuum
_sectio_expiratus (
    CredoContextus* c)
{
    _reiectum(c, "expiratus", VERUM);
}

interior vacuum
_sectio_ignotus (
    CredoContextus* c)
{
    _reiectum(c, "ignotus", VERUM);
}

/* sine ancora: repositorium systematis CA probationis ignorat */
interior vacuum
_sectio_sine_ancora (
    CredoContextus* c)
{
    _reiectum(c, "localhost", FALSUM);
}

interior vacuum
_sectio_non_der (
    CredoContextus* c)
{
    FiduciaCertificatum fc;
                 chorda causa;

    fc.octeti      = (constans i8*)"non certificatum";
    fc.mensura     = (i32)strlen("non certificatum");
    causa.datum    = NIHIL;
    causa.mensura  = ZEPHYRUM;
    CREDO_FALSUM(fiducia_iudicare(&fc, I, "localhost", NIHIL, &causa,
        c->piscina));
    CREDO_VERUM(_continet(causa, "DER"));
}

interior vacuum
_sectio_ancora_absens (
    CredoContextus* c)
{
    FiduciaCertificatum fc = _legere(c, "localhost");
                 chorda causa;

    causa.datum    = NIHIL;
    causa.mensura  = ZEPHYRUM;
    CREDO_FALSUM(fiducia_iudicare(&fc, I, "localhost",
        "/nusquam/ancora.der", &causa, c->piscina));
    CREDO_VERUM(_continet(causa, "ancora legi non potest"));
}

/* folium: subiectum CN, validitas CCCLXV dierum (± diem) */
interior vacuum
_sectio_folium (
    CredoContextus* c)
{
    FiduciaCertificatum fc = _legere(c, "localhost");
          FiduciaFolium ff;
                    s64 dies;

    CREDO_NECESSE(fiducia_folium_describere(&fc, &ff, c->piscina));
    CREDO_VERUM(ff.valida);
    CREDO_CHORDA_AEQUALIS_LITERIS(ff.subiectum, "localhost");
    dies = (ff.non_post - ff.non_ante) / ((s64)LXXXVI * M + CD);
    CREDO_VERUM(dies >= CCCLXIV && dies <= CCCLXVI);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "ancora", _sectio_ancora, NIHIL, NIHIL, NIHIL, NIHIL },
    { "alienus", _sectio_alienus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "expiratus", _sectio_expiratus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ignotus", _sectio_ignotus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sine ancora", _sectio_sine_ancora, NIHIL, NIHIL, NIHIL, NIHIL },
    { "non der", _sectio_non_der, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ancora absens", _sectio_ancora_absens, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "folium", _sectio_folium, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("fiducia", SECTIONES, _suitam_parare,
        NIHIL);
}

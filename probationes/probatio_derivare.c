/* probatio_derivare.c - derivatio eventuum: duplex ex tempore;
 * (A5) tractus ex limine IV pixelorum */
#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "fenestra.h"
#include "derivare.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior Eventus
mus_depressus (
    s64 tempus,
    s32 x,
    s32 y)
{
    Eventus e;
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus        = EVENTUS_MUS_DEPRESSUS;
    e.tempus       = tempus;
    e.datum.mus.x  = x;
    e.datum.mus.y  = y;
    redde e;
}

interior Eventus
mus_eventum (
    eventus_genus_t genus,
                s64 tempus,
                s32 x,
                s32 y)
{
    Eventus e;

    e                   = mus_depressus(tempus, x, y);
    e.genus             = genus;
    e.datum.mus.botton  = MUS_SINISTER;
    redde e;
}

interior eventus_genus_t
genus_ad (
    Xar* x,
    i32  i)
{
    redde ((Eventus*)xar_obtinere(x, i))->genus;
}

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    Derivator  d;
          Xar* effusio;
      Eventus  e;
      Eventus* ultimus;
    effusio = xar_creare(c->piscina, (i32)magnitudo(Eventus));
    derivator_initiare(&d, CCC, IV);

    e = mus_depressus(M, X, X);
    derivare(&d, &e, effusio);
    CREDO_AEQUALIS_I32 (xar_numerus(effusio), I);
    e = mus_depressus(M + CC, XII, XI);
    derivare(&d, &e, effusio);
    /* depressus + DUPLEX */
    CREDO_AEQUALIS_I32 (xar_numerus(effusio), III);
    ultimus = (Eventus*)xar_obtinere(effusio, II);
    CREDO_VERUM (ultimus->genus == EVENTUS_MUS_DUPLEX);
    CREDO_VERUM (ultimus->tempus == M + CC);
}

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    Derivator  d;
          Xar* effusio;
      Eventus  e;
    effusio = xar_creare(c->piscina, (i32)magnitudo(Eventus));
    derivator_initiare(&d, CCC, IV);

    /* status ex sectione prima: duo ictus eius iterantur (sine
     * assertionibus), ut derivator et effusio idem teneant */
    e = mus_depressus(M, X, X);
    derivare(&d, &e, effusio);
    e = mus_depressus(M + CC, XII, XI);
    derivare(&d, &e, effusio);

    e = mus_depressus(M + CCL, XII, XI);
    derivare(&d, &e, effusio);
    /* solum depressus */
    CREDO_AEQUALIS_I32 (xar_numerus(effusio), IV);
}

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    Derivator  d;
          Xar* effusio;
      Eventus  e;
    effusio = xar_creare(c->piscina, (i32)magnitudo(Eventus));
    derivator_initiare(&d, CCC, IV);

    xar_vacare(effusio);
    e = mus_depressus(V * M, X, X);
    derivare(&d, &e, effusio);
    e = mus_depressus(V * M + DC, X, X);
    derivare(&d, &e, effusio);
    CREDO_AEQUALIS_I32 (xar_numerus(effusio), II);
}

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
    Derivator  d;
          Xar* effusio;
      Eventus  e;
    effusio = xar_creare(c->piscina, (i32)magnitudo(Eventus));
    derivator_initiare(&d, CCC, IV);

    xar_vacare(effusio);
    e = mus_depressus(X * M, X, X);
    derivare(&d, &e, effusio);
    e = mus_depressus(X * M + C, C, C);
    derivare(&d, &e, effusio);
    CREDO_AEQUALIS_I32 (xar_numerus(effusio), II);
}

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
    Derivator  d;
          Xar* effusio;
      Eventus  e;
    effusio = xar_creare(c->piscina, (i32)magnitudo(Eventus));
    derivator_initiare(&d, CCC, IV);

    /* M2c: menu ad indicatorem aperitur - dexter deinde sinister cito
     * in re prima duplex non est (aliter duplex sub menu clauso
     * caderet) */
    xar_vacare(effusio);
    e                   = mus_depressus(XV * M, X, X);
    e.datum.mus.botton  = MUS_DEXTER;
    derivare(&d, &e, effusio);
    e                   = mus_depressus(XV * M + C, X, X);
    e.datum.mus.botton  = MUS_SINISTER;
    derivare(&d, &e, effusio);
    CREDO_AEQUALIS_I32 (xar_numerus(effusio), II);
}

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
    Derivator  d;
          Xar* effusio;
      Eventus  e;
      Eventus* ultimus;
    effusio = xar_creare(c->piscina, (i32)magnitudo(Eventus));
    derivator_initiare(&d, CCC, IV);

    xar_vacare(effusio);
    e                   = mus_depressus(XX * M, X, X);
    e.datum.mus.botton  = MUS_DEXTER;
    derivare(&d, &e, effusio);
    e                   = mus_depressus(XX * M + C, X, X);
    e.datum.mus.botton  = MUS_DEXTER;
    derivare(&d, &e, effusio);
    CREDO_AEQUALIS_I32 (xar_numerus(effusio), III);
    ultimus = (Eventus*)xar_obtinere(effusio, II);
    CREDO_VERUM (ultimus->genus == EVENTUS_MUS_DUPLEX);
    CREDO_VERUM (ultimus->datum.mus.botton == MUS_DEXTER);
}

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
    Derivator  d;
          Xar* effusio;
      Eventus  e;
    effusio = xar_creare(c->piscina, (i32)magnitudo(Eventus));
    derivator_initiare(&d, CCC, IV);

    xar_vacare(effusio);
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus   = EVENTUS_CLAVIS_DEPRESSUS;
    e.tempus  = XX * M;
    derivare(&d, &e, effusio);
    CREDO_AEQUALIS_I32 (xar_numerus(effusio), I);
}

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
    Derivator  d;
          Xar* effusio;
      Eventus  e;
      Eventus* ultimus;
    effusio = xar_creare(c->piscina, (i32)magnitudo(Eventus));
    derivator_initiare(&d, CCC, IV);

    derivator_initiare(&d, CCC, IV);
    xar_vacare(effusio);
    e = mus_eventum(EVENTUS_MUS_DEPRESSUS, XXX * M, X, X);
    derivare(&d, &e, effusio);
    /* IV pixela praecise: NONDUM tractus (limen = ultra IV) */
    e = mus_eventum(EVENTUS_MUS_MOTUS, XXX * M + X, XIV, VI);
    derivare(&d, &e, effusio);
    CREDO_AEQUALIS_I32 (xar_numerus(effusio), II);
    /* V: tractus incipit - post motum crudum, x/y = ORIGO */
    e = mus_eventum(EVENTUS_MUS_MOTUS, XXX * M + XX, XV, X);
    derivare(&d, &e, effusio);
    CREDO_AEQUALIS_I32 (xar_numerus(effusio), IV);
    CREDO_VERUM (genus_ad(effusio, II) == EVENTUS_MUS_MOTUS);
    CREDO_VERUM (genus_ad(effusio, III) == EVENTUS_TRACTUS_INCIPIT);
    ultimus = (Eventus*)xar_obtinere(effusio, III);
    CREDO_AEQUALIS_S32 (ultimus->datum.mus.x, X);
    CREDO_AEQUALIS_S32 (ultimus->datum.mus.y, X);
    CREDO_VERUM (ultimus->tempus == XXX * M + XX);
    CREDO_VERUM (ultimus->datum.mus.botton == MUS_SINISTER);
    /* motus sequens: TRACTUS cum positione currente */
    e = mus_eventum(EVENTUS_MUS_MOTUS, XXX * M + XXX, XL, -V);
    derivare(&d, &e, effusio);
    CREDO_AEQUALIS_I32 (xar_numerus(effusio), VI);
    CREDO_VERUM (genus_ad(effusio, V) == EVENTUS_TRACTUS);
    ultimus = (Eventus*)xar_obtinere(effusio, V);
    CREDO_AEQUALIS_S32 (ultimus->datum.mus.x, XL);
    CREDO_AEQUALIS_S32 (ultimus->datum.mus.y, -V);
    /* liberatio: TRACTUS_FINIT */
    e = mus_eventum(EVENTUS_MUS_LIBERATUS, XXX * M + XL, XL, -V);
    derivare(&d, &e, effusio);
    CREDO_AEQUALIS_I32 (xar_numerus(effusio), VIII);
    CREDO_VERUM (genus_ad(effusio, VII) == EVENTUS_TRACTUS_FINIT);
    /* motus post liberationem: nullus tractus */
    e = mus_eventum(EVENTUS_MUS_MOTUS, XXX * M + L, C, C);
    derivare(&d, &e, effusio);
    CREDO_AEQUALIS_I32 (xar_numerus(effusio), IX);
}

interior vacuum
_sectio_nona (
    CredoContextus* c)
{
    Derivator  d;
          Xar* effusio;
      Eventus  e;
    effusio = xar_creare(c->piscina, (i32)magnitudo(Eventus));
    derivator_initiare(&d, CCC, IV);

    xar_vacare(effusio);
    e = mus_eventum(EVENTUS_MUS_DEPRESSUS, XL * M, X, X);
    derivare(&d, &e, effusio);
    e = mus_eventum(EVENTUS_MUS_LIBERATUS, XL * M + X, XII, XI);
    derivare(&d, &e, effusio);
    CREDO_AEQUALIS_I32 (xar_numerus(effusio), II);
}

interior vacuum
_sectio_decima (
    CredoContextus* c)
{
    Derivator  d;
          Xar* effusio;
      Eventus  e;
    effusio = xar_creare(c->piscina, (i32)magnitudo(Eventus));
    derivator_initiare(&d, CCC, IV);

    xar_vacare(effusio);
    e = mus_eventum(EVENTUS_MUS_MOTUS, L * M, X, X);
    derivare(&d, &e, effusio);
    e = mus_eventum(EVENTUS_MUS_MOTUS, L * M + X, CC, CC);
    derivare(&d, &e, effusio);
    CREDO_AEQUALIS_I32 (xar_numerus(effusio), II);
}

interior vacuum
_sectio_decima_prima (
    CredoContextus* c)
{
    Derivator  d;
          Xar* effusio;
      Eventus  e;
    effusio = xar_creare(c->piscina, (i32)magnitudo(Eventus));
    derivator_initiare(&d, CCC, IV);

    /* pressio -> tractus -> liberatio, deinde pressio cito prope
     * originem: NON duplex (tractus non est ictus primus) */
    xar_vacare(effusio);
    derivator_initiare(&d, CCC, IV);
    e = mus_eventum(EVENTUS_MUS_DEPRESSUS, LX * M, X, X);
    derivare(&d, &e, effusio);
    e = mus_eventum(EVENTUS_MUS_MOTUS, LX * M + X, XXX, X);
    derivare(&d, &e, effusio);
    e = mus_eventum(EVENTUS_MUS_LIBERATUS, LX * M + XX, X, X);
    derivare(&d, &e, effusio);
    xar_vacare(effusio);
    e = mus_eventum(EVENTUS_MUS_DEPRESSUS, LX * M + L, X, X);
    derivare(&d, &e, effusio);
    CREDO_AEQUALIS_I32 (xar_numerus(effusio), I);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "Duo ictus intra CCC ms et IV px -> DUPLEX", _sectio_prima, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "Tertius ictus statim NON triplex-ut-duplex", _sectio_secunda,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "Duo ictus longe tempore -> nullus DUPLEX", _sectio_tertia, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "Duo ictus longe in spatio -> nullus DUPLEX", _sectio_quarta,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "Dexter deinde sinister -> nullus DUPLEX", _sectio_quinta, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "Dexter bis -> DUPLEX dexter", _sectio_sexta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "Eventus non-mus transit immutatus", _sectio_septima, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "A5: tractus - limen IV, origo, finis", _sectio_octava, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "A5: ictus sine motu: nullus tractus", _sectio_nona, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "A5: motus sine pressione: nullus tractus", _sectio_decima, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "A5: tractus duplicem non parat", _sectio_decima_prima, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("derivare", SECTIONES);
}

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

s32 principale (vacuum)
{
      Piscina* piscina;
    Derivator  d;
          Xar* effusio;
      Eventus  e;
      Eventus* ultimus;

    piscina = piscina_generare_dynamicum("probatio_derivare", XVI * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    effusio = xar_creare(piscina, (i32)magnitudo(Eventus));
    derivator_initiare(&d, CCC, IV);

    imprimere("\n--- Duo ictus intra CCC ms et IV px -> DUPLEX ---\n");
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

    imprimere("\n--- Tertius ictus statim NON triplex-ut-duplex ---\n");
    e = mus_depressus(M + CCL, XII, XI);
    derivare(&d, &e, effusio);
    /* solum depressus */
    CREDO_AEQUALIS_I32 (xar_numerus(effusio), IV);
    imprimere("\n--- Duo ictus longe tempore -> nullus DUPLEX ---\n");
    xar_vacare(effusio);
    e = mus_depressus(V * M, X, X);
    derivare(&d, &e, effusio);
    e = mus_depressus(V * M + DC, X, X);
    derivare(&d, &e, effusio);
    CREDO_AEQUALIS_I32 (xar_numerus(effusio), II);

    imprimere("\n--- Duo ictus longe in spatio -> nullus DUPLEX ---\n");
    xar_vacare(effusio);
    e = mus_depressus(X * M, X, X);
    derivare(&d, &e, effusio);
    e = mus_depressus(X * M + C, C, C);
    derivare(&d, &e, effusio);
    CREDO_AEQUALIS_I32 (xar_numerus(effusio), II);

    imprimere("\n--- Eventus non-mus transit immutatus ---\n");
    xar_vacare(effusio);
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus   = EVENTUS_CLAVIS_DEPRESSUS;
    e.tempus  = XX * M;
    derivare(&d, &e, effusio);
    CREDO_AEQUALIS_I32 (xar_numerus(effusio), I);

    imprimere("\n--- A5: tractus - limen IV, origo, finis ---\n");
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

    imprimere("\n--- A5: ictus sine motu: nullus tractus ---\n");
    xar_vacare(effusio);
    e = mus_eventum(EVENTUS_MUS_DEPRESSUS, XL * M, X, X);
    derivare(&d, &e, effusio);
    e = mus_eventum(EVENTUS_MUS_LIBERATUS, XL * M + X, XII, XI);
    derivare(&d, &e, effusio);
    CREDO_AEQUALIS_I32 (xar_numerus(effusio), II);

    imprimere("\n--- A5: motus sine pressione: nullus tractus ---\n");
    xar_vacare(effusio);
    e = mus_eventum(EVENTUS_MUS_MOTUS, L * M, X, X);
    derivare(&d, &e, effusio);
    e = mus_eventum(EVENTUS_MUS_MOTUS, L * M + X, CC, CC);
    derivare(&d, &e, effusio);
    CREDO_AEQUALIS_I32 (xar_numerus(effusio), II);

    imprimere("\n--- A5: tractus duplicem non parat ---\n");
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

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}

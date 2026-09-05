/* probatio_derivare.c - derivatio eventuum: duplex ex tempore */
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
    i32 x,
    i32 y)
{
    Eventus e;
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus        = EVENTUS_MUS_DEPRESSUS;
    e.tempus       = tempus;
    e.datum.mus.x  = x;
    e.datum.mus.y  = y;
    redde e;
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

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}

/* praeconium_cantus.c - eventus 'cantus' (praeconium_cantus.h) */
#include "praeconium_cantus.h"

b32
praeconium_cantum_parare (
               chorda  textus,
                  b32  intactus,
    PraeconiumEventus* e,
              Piscina* piscina,
       CantualeVitium* vitium)
{
    Cantio cantio;

    si (textus.mensura > PRAECONIUM_NUNTIUS_MAXIMUS)
    {
        vitium->titulus.datum    = NIHIL;
        vitium->titulus.mensura  = ZEPHYRUM;
        vitium->offset           = PRAECONIUM_NUNTIUS_MAXIMUS;
        vitium->causa            = "textus longior quam CC octeti";
        redde FALSUM;
    }
    si (!cantionem_legere(textus, intactus, piscina, &cantio, vitium))
    {
        redde FALSUM;
    }
    e->eventus = chorda_ex_literis("cantus", piscina);
    e->genus = chorda_ex_literis(intactus ? "intactus" : "",
        piscina);
    e->nuntius = chorda_transcribere(textus, piscina);
    redde VERUM;
}

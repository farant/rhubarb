/* sarcina.h - arca ZIP SOLUM LEGENDA (lector L1; spec
 * project-specs/lector-spec.md): directorium centrale, introitus per
 * nomen, data reposita (modus 0) aut DEFLATE (modus VIII, flatura),
 * CRC probatur. Arca tota in memoria (chorda; vocans filum legit). Non
 * sustinet: ZIP64, cryptographiam, modos alios, scripturam. Numquam
 * extra arcam legit: arca mala -> vitium (Anglice), numquam ruina. */
#ifndef SARCINA_H
#define SARCINA_H

/* <aedilis corpus="lib/sarcina.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"

nomen structura Sarcina Sarcina;   /* opaca */

nomen structura {
    chorda titulus;            /* nomen intra arcam ("META-INF/
                                 * container.xml"); in arca ipsa */
       i32 modus;              /* 0 repositum, VIII DEFLATE, alius =
                                 * non legitur */
       i32 mensura;            /* octeti expliciti */
       i32 mensura_compressa;  /* octeti in arca */
       i32 crc;
       i32 positio;            /* offset capitis localis */
       b32 directorium;        /* titulus in '/' desinit */
} SarcinaIntroitus;

/* arcam aperire: directorium centrale legitur; NIHIL et *vitium si
 * arca non est aut mala (vitium in piscina) */
Sarcina*
sarcina_aperire (
      chorda  datum,
     Piscina* piscina,
      chorda* vitium);

i32
sarcina_numerus (
    constans Sarcina* s);

/* introitus k (ordine directorii); NIHIL extra ambitum */
constans SarcinaIntroitus*
sarcina_introitus (
    constans Sarcina* s,
                 i32  k);

/* introitus per nomen exactum; NIHIL si abest */
constans SarcinaIntroitus*
sarcina_invenire (
    constans Sarcina* s,
              chorda  titulus);

/* data introitus (explicita, CRC probata) in piscina; FALSUM et
 * *vitium si modus ignotus, cryptographia, fines mali, CRC aut
 * mensura non congruunt */
b32
sarcina_legere (
             constans Sarcina* s,
    constans SarcinaIntroitus* e,
                      Piscina* piscina,
                       chorda* exitus,
                       chorda* vitium);

#endif /* SARCINA_H */

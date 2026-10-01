/* probatio_eventus_cauda.c - cauda eventuum fontis (eventus A3a):
 * anulus FIFO, plena -> amissa, textus COPIATUS, vita visuum per
 * lectionem, truncatio. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "eventus.h"
#include "eventus_cauda.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior Eventus
_eventum (
    eventus_genus_t genus,
                s64 tempus)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus   = genus;
    e.tempus  = tempus;
    redde e;
}

s32 principale (vacuum)
{
          Piscina* piscina;
     EventusCauda* c;
          Eventus  e;
          Eventus  primus;
              i32  k;
           chorda  visus;
        character  fons[XVI];

    piscina = piscina_generare_dynamicum("probatio_eventus_cauda",
        M * M);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);
    c = (EventusCauda*)piscina_allocare_ordinatum(piscina,
        magnitudo(EventusCauda), VIII);
    CREDO_NON_NIHIL (c);
    eventus_caudam_initiare(c);

    imprimere("\n--- I. FIFO ---\n");
    e = _eventum(EVENTUS_MUS_DEPRESSUS, I);
    CREDO_VERUM (eventus_caudae_impellere(c, &e));
    e = _eventum(EVENTUS_MUS_LIBERATUS, II);
    CREDO_VERUM (eventus_caudae_impellere(c, &e));
    CREDO_VERUM (eventus_caudae_extrahere(c, &e));
    CREDO_VERUM (e.genus == EVENTUS_MUS_DEPRESSUS && e.tempus == I);
    CREDO_VERUM (eventus_caudae_extrahere(c, &e));
    CREDO_VERUM (e.genus == EVENTUS_MUS_LIBERATUS);
    CREDO_FALSUM (eventus_caudae_extrahere(c, &e));

    imprimere("\n--- II. plena -> amissa ---\n");
    per (k = ZEPHYRUM; k < EVENTUS_CAUDA_CAPACITAS; k++)
    {
        e = _eventum(EVENTUS_MUS_MOTUS, (s64)k);
        CREDO_VERUM (eventus_caudae_impellere(c, &e));
    }
    e = _eventum(EVENTUS_MUS_DEPRESSUS, M);
    CREDO_FALSUM (eventus_caudae_impellere(c, &e));
    CREDO_AEQUALIS_I32 (c->amissa, I);
    CREDO_VERUM (eventus_caudae_extrahere(c, &primus));
    CREDO_VERUM (primus.tempus == ZEPHYRUM);      /* antiquissimum */
    dum (eventus_caudae_extrahere(c, &e))
    {
    }
    CREDO_VERUM (e.tempus == (s64)(EVENTUS_CAUDA_CAPACITAS - I));

    imprimere("\n--- III. textus COPIATUS ---\n");
    eventus_cauda_lectio_incipit(c);
    strcpy(fons, "a\xC3\xA9 \"x\"");                 /* 'aé "x"' */
    CREDO_VERUM (eventus_caudae_textum_impellere(c, V,
        (constans i8*)fons,
        (i32)strlen(fons), EVENTUS_ORIGO_SCRIPTA));
    fons[0] = 'Z';                                    /* fons mutatur */
    CREDO_VERUM (eventus_caudae_extrahere(c, &e));
    CREDO_VERUM (e.genus == EVENTUS_TEXTUS && e.tempus == V);
    CREDO_VERUM (chorda_aequalis_literis(e.datum.textus.contentum,
        "a\xC3\xA9 \"x\""));
    CREDO_VERUM (e.datum.textus.genus == EVENTUS_TEXTUS_COMMISSUM);
    CREDO_VERUM (e.datum.textus.origo == EVENTUS_ORIGO_SCRIPTA);
    CREDO_FALSUM (e.datum.textus.truncatum);
    CREDO_FALSUM (eventus_caudae_textum_impellere(c, VI,
        (constans i8*)"",
        ZEPHYRUM, EVENTUS_ORIGO_SCRIPTA));

    imprimere("\n--- IV. vita visuum: cauda non vacua servat ---\n");
    eventus_cauda_lectio_incipit(c);                  /* vacua: vacatur */
    CREDO_AEQUALIS_I32 (c->textus_mensura, ZEPHYRUM);
    CREDO_VERUM (eventus_caudae_textum_impellere(c, VII,
        (constans i8*)"primum", VI, EVENTUS_ORIGO_SCRIPTA));
    /* lectio nova dum eventum 'primum' nondum extractum: NON vacatur */
    eventus_cauda_lectio_incipit(c);
    CREDO_VERUM (eventus_caudae_textum_impellere(c, VIII,
        (constans i8*)"secundum", VIII, EVENTUS_ORIGO_GLUTINATA));
    CREDO_VERUM (eventus_caudae_extrahere(c, &e));
    visus = e.datum.textus.contentum;
    CREDO_VERUM (chorda_aequalis_literis(visus, "primum"));
    CREDO_VERUM (eventus_caudae_extrahere(c, &e));
    CREDO_VERUM (chorda_aequalis_literis(e.datum.textus.contentum,
        "secundum"));
    CREDO_VERUM (e.datum.textus.origo == EVENTUS_ORIGO_GLUTINATA);
    CREDO_VERUM (chorda_aequalis_literis(visus, "primum")); /* adhuc */

    imprimere("\n--- V. truncatio ---\n");
    eventus_cauda_lectio_incipit(c);
    {
        i8* magnum = (i8*)piscina_allocare(piscina,
            (memoriae_index)EVENTUS_CAUDA_TEXTUS + C);

        memset(magnum, 'q', (memoriae_index)EVENTUS_CAUDA_TEXTUS + C);
        CREDO_VERUM (eventus_caudae_textum_impellere(c, IX,
            (constans i8*)magnum, EVENTUS_CAUDA_TEXTUS + C,
            EVENTUS_ORIGO_GLUTINATA));
        CREDO_VERUM (eventus_caudae_extrahere(c, &e));
        CREDO_VERUM (e.datum.textus.truncatum);
        CREDO_AEQUALIS_I32 (e.datum.textus.contentum.mensura,
            EVENTUS_CAUDA_TEXTUS);
    }

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}

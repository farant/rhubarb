/* probatio_eventus_cauda.c - cauda eventuum fontis (eventus A3a):
 * anulus FIFO, plena -> amissa, textus COPIATUS, vita visuum per
 * lectionem, truncatio; (A3b) motus coalitus cum exemplis, residuum
 * rotulae. */
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

interior Eventus
_motum (
    s32 x,
    s32 y,
    s64 tempus,
    i32 modificantes)
{
    Eventus e;

    e                         = _eventum(EVENTUS_MUS_MOTUS, tempus);
    e.datum.mus.x             = (i32)x;
    e.datum.mus.y             = (i32)y;
    e.datum.mus.modificantes  = modificantes;
    e.datum.mus.pressio       = EVENTUS_PRESSIO_IGNOTA;
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
    eventus_cauda_lectio_incipit(c);            /* vacua: vacatur */
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

    imprimere("\n--- VI. motus coalitus: X -> I cum IX exemplis ---\n");
    dum (eventus_caudae_extrahere(c, &e))
    {
    }
    eventus_cauda_lectio_incipit(c);
    per (k = ZEPHYRUM; k < X; k++)
    {
        e = _motum((s32)k, (s32)(II * k), (s64)(C + k), ZEPHYRUM);
        CREDO_VERUM (eventus_caudae_motum_impellere(c, &e));
    }
    CREDO_AEQUALIS_I32 (c->numerus, I);
    CREDO_VERUM (eventus_caudae_extrahere(c, &e));
    CREDO_VERUM (e.genus == EVENTUS_MUS_MOTUS);
    CREDO_AEQUALIS_I32 (e.datum.mus.x, IX);           /* ultima vera */
    CREDO_AEQUALIS_I32 (e.datum.mus.y, XVIII);
    CREDO_VERUM (e.tempus == (s64)(C + IX));
    CREDO_AEQUALIS_I32 (e.datum.mus.numerus_exemplorum, IX);
    CREDO_NON_NIHIL (e.datum.mus.exempla);
    CREDO_VERUM (e.datum.mus.exempla[0].x == 0
        && e.datum.mus.exempla[0].y == 0
        && e.datum.mus.exempla[0].tempus == (s64)C);
    CREDO_VERUM (e.datum.mus.exempla[VIII].x == VIII
        && e.datum.mus.exempla[VIII].y == XVI
        && e.datum.mus.exempla[VIII].tempus == (s64)(C + VIII));

    imprimere("\n--- VII. coalitio terminos servat ---\n");
    eventus_cauda_lectio_incipit(c);
    /* modificantes diversi: duo eventa */
    e = _motum(I, I, I, ZEPHYRUM);
    CREDO_VERUM (eventus_caudae_motum_impellere(c, &e));
    e = _motum(II, II, II, MOD_SHIFT);
    CREDO_VERUM (eventus_caudae_motum_impellere(c, &e));
    CREDO_AEQUALIS_I32 (c->numerus, II);
    /* eventus alius inter motus: numquam trans eum */
    e                   = _eventum(EVENTUS_MUS_DEPRESSUS, III);
    e.datum.mus.botton  = MUS_SINISTER;
    CREDO_VERUM (eventus_caudae_impellere(c, &e));
    e = _motum(IV, IV, IV, MOD_SHIFT);
    CREDO_VERUM (eventus_caudae_motum_impellere(c, &e));
    CREDO_AEQUALIS_I32 (c->numerus, IV);
    /* botton diversus: novum eventum */
    e                   = _motum(V, V, V, MOD_SHIFT);
    e.datum.mus.botton  = MUS_SINISTER;
    CREDO_VERUM (eventus_caudae_motum_impellere(c, &e));
    CREDO_AEQUALIS_I32 (c->numerus, V);
    /* indicator diversus (stilus): novum eventum */
    e                      = _motum(VI, VI, VI, MOD_SHIFT);
    e.datum.mus.botton     = MUS_SINISTER;
    e.datum.mus.indicator  = I;
    CREDO_VERUM (eventus_caudae_motum_impellere(c, &e));
    CREDO_AEQUALIS_I32 (c->numerus, VI);
    dum (eventus_caudae_extrahere(c, &e))
    {
        CREDO_AEQUALIS_I32 (e.datum.mus.numerus_exemplorum, ZEPHYRUM);
    }

    imprimere("\n--- VIII. LXIV exempla: antiquissima servantur ---\n");
    eventus_cauda_lectio_incipit(c);
    per (k = ZEPHYRUM; k < C; k++)
    {
        e = _motum((s32)k, ZEPHYRUM, (s64)k, ZEPHYRUM);
        CREDO_VERUM (eventus_caudae_motum_impellere(c, &e));
    }
    CREDO_VERUM (eventus_caudae_extrahere(c, &e));
    CREDO_AEQUALIS_I32 (e.datum.mus.numerus_exemplorum,
        EVENTUS_EXEMPLA_MAXIMA);
    CREDO_AEQUALIS_I32 (e.datum.mus.x, XCIX);         /* finis verus */
    CREDO_VERUM (e.datum.mus.exempla[0].x == 0);
    CREDO_VERUM (e.datum.mus.exempla[LXIII].x == LXIII);

    imprimere("\n--- IX. vita exemplorum per lectionem ---\n");
    eventus_cauda_lectio_incipit(c);            /* vacua: vacatur */
    CREDO_AEQUALIS_I32 (c->exempla_mensura, ZEPHYRUM);
    per (k = ZEPHYRUM; k < III; k++)
    {
        e = _motum((s32)(L + k), ZEPHYRUM, (s64)k, ZEPHYRUM);
        CREDO_VERUM (eventus_caudae_motum_impellere(c, &e));
    }
    CREDO_AEQUALIS_I32 (c->exempla_mensura, II);
    /* lectio nova, motus nondum extractus: exempla NON vacantur */
    eventus_cauda_lectio_incipit(c);
    CREDO_AEQUALIS_I32 (c->exempla_mensura, II);
    e = _eventum(EVENTUS_CLAVIS_DEPRESSUS, X);
    CREDO_VERUM (eventus_caudae_impellere(c, &e));
    per (k = ZEPHYRUM; k < III; k++)
    {
        e = _motum((s32)(LX + k), ZEPHYRUM, (s64)(XX + k), ZEPHYRUM);
        CREDO_VERUM (eventus_caudae_motum_impellere(c, &e));
    }
    CREDO_VERUM (eventus_caudae_extrahere(c, &primus));
    CREDO_AEQUALIS_I32 (primus.datum.mus.numerus_exemplorum, II);
    CREDO_VERUM (eventus_caudae_extrahere(c, &e));    /* clavis */
    CREDO_VERUM (eventus_caudae_extrahere(c, &e));
    CREDO_AEQUALIS_I32 (e.datum.mus.numerus_exemplorum, II);
    CREDO_VERUM (e.datum.mus.exempla[0].x == LX
        && e.datum.mus.exempla[I].x == LXI);
    /* visus primi intactus post secundum */
    CREDO_VERUM (primus.datum.mus.exempla[0].x == L
        && primus.datum.mus.exempla[I].x == LI);
    eventus_cauda_lectio_incipit(c);
    CREDO_AEQUALIS_I32 (c->exempla_mensura, ZEPHYRUM);

    imprimere("\n--- X. residuum rotulae ---\n");
    {
        f64 r = 0.0;

        CREDO_AEQUALIS_S32 (eventus_residuum_integrare(&r, 0.4), 0);
        CREDO_AEQUALIS_S32 (eventus_residuum_integrare(&r, 0.4), 0);
        CREDO_AEQUALIS_S32 (eventus_residuum_integrare(&r, 0.4), 1);
        CREDO_VERUM (r > 0.19 && r < 0.21);
        CREDO_AEQUALIS_S32 (eventus_residuum_integrare(&r, -2.5), -2);
        CREDO_VERUM (r < -0.29 && r > -0.31);
        CREDO_AEQUALIS_S32 (eventus_residuum_integrare(&r, 7.0), 6);
        CREDO_VERUM (r > 0.69 && r < 0.71);
    }

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}

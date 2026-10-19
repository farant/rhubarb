/* probatio_eventus_cauda.c - cauda eventuum fontis (eventus A3a):
 * anulus FIFO, plena -> amissa, textus COPIATUS, vita visuum per
 * lectionem, truncatio; (A3b) motus coalitus cum exemplis, residuum
 * rotulae; (B3b) viae depositionis COPIATAE. */
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
    e.datum.mus.x             = x;
    e.datum.mus.y             = y;
    e.datum.mus.modificantes  = modificantes;
    e.datum.mus.pressio       = EVENTUS_PRESSIO_IGNOTA;
    redde e;
}

/* Cauda UNA per omnes gradus I-XI transit: gradus quisque statum
 * caudae a priore relictum accipit (IV et IX 'vacua: vacatur' textum et
 * exempla priorum gradus vacare probant), ergo sectio una cum
 * CREDO_GRADUS, non sectiones separatae */
interior vacuum
_sectio_prima (
    CredoContextus* contextus)
{
          Piscina* piscina;
     EventusCauda* c;
          Eventus  e;
          Eventus  primus;
              i32  k;
           chorda  visus;
        character  fons[XVI];

    piscina = contextus->piscina;
    c = (EventusCauda*)piscina_allocare_ordinatum(piscina,
        magnitudo(EventusCauda), VIII);
    CREDO_NON_NIHIL (c);
    eventus_caudam_initiare(c);

    CREDO_GRADUS("I. FIFO");
    e = _eventum(EVENTUS_MUS_DEPRESSUS, I);
    CREDO_VERUM (eventus_caudae_impellere(c, &e));
    e = _eventum(EVENTUS_MUS_LIBERATUS, II);
    CREDO_VERUM (eventus_caudae_impellere(c, &e));
    CREDO_VERUM (eventus_caudae_extrahere(c, &e));
    CREDO_VERUM (e.genus == EVENTUS_MUS_DEPRESSUS && e.tempus == I);
    CREDO_VERUM (eventus_caudae_extrahere(c, &e));
    CREDO_VERUM (e.genus == EVENTUS_MUS_LIBERATUS);
    CREDO_FALSUM (eventus_caudae_extrahere(c, &e));

    CREDO_GRADUS("II. plena -> amissa");
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

    CREDO_GRADUS("III. textus COPIATUS");
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

    CREDO_GRADUS("IV. vita visuum: cauda non vacua servat");
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

    CREDO_GRADUS("V. truncatio");
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

    CREDO_GRADUS("VI. motus coalitus: X -> I cum IX exemplis");
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
    CREDO_AEQUALIS_S32 (e.datum.mus.x, IX);           /* ultima vera */
    CREDO_AEQUALIS_S32 (e.datum.mus.y, XVIII);
    CREDO_VERUM (e.tempus == (s64)(C + IX));
    CREDO_AEQUALIS_I32 (e.datum.mus.numerus_exemplorum, IX);
    CREDO_NON_NIHIL (e.datum.mus.exempla);
    CREDO_VERUM (e.datum.mus.exempla[0].x == 0
        && e.datum.mus.exempla[0].y == 0
        && e.datum.mus.exempla[0].tempus == (s64)C);
    CREDO_VERUM (e.datum.mus.exempla[VIII].x == VIII
        && e.datum.mus.exempla[VIII].y == XVI
        && e.datum.mus.exempla[VIII].tempus == (s64)(C + VIII));

    CREDO_GRADUS("VII. coalitio terminos servat");
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

    CREDO_GRADUS("VIII. LXIV exempla: antiquissima servantur");
    eventus_cauda_lectio_incipit(c);
    per (k = ZEPHYRUM; k < C; k++)
    {
        e = _motum((s32)k, ZEPHYRUM, (s64)k, ZEPHYRUM);
        CREDO_VERUM (eventus_caudae_motum_impellere(c, &e));
    }
    CREDO_VERUM (eventus_caudae_extrahere(c, &e));
    CREDO_AEQUALIS_I32 (e.datum.mus.numerus_exemplorum,
        EVENTUS_EXEMPLA_MAXIMA);
    CREDO_AEQUALIS_S32 (e.datum.mus.x, XCIX);         /* finis verus */
    CREDO_VERUM (e.datum.mus.exempla[0].x == 0);
    CREDO_VERUM (e.datum.mus.exempla[LXIII].x == LXIII);

    CREDO_GRADUS("IX. vita exemplorum per lectionem");
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

    CREDO_GRADUS("X. residuum rotulae");
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

    CREDO_GRADUS("XI. depositio: viae COPIATAE (eventus B3b)");
    {
        character viae[XVI];

        dum (eventus_caudae_extrahere(c, &e))
        {
        }
        eventus_cauda_lectio_incipit(c);
        strcpy(viae, "/a\n/b c");
        e = _eventum(EVENTUS_DEPOSITIO, XL);
        e.datum.depositio.x = VII;
        e.datum.depositio.y = IX;
        e.datum.depositio.viae.datum = (i8*)viae;
        e.datum.depositio.viae.mensura = (i32)strlen(viae);
        e.datum.depositio.numerus = II;
        e.datum.depositio.promota = VERUM;
        CREDO_VERUM (eventus_caudae_depositionem_impellere(c, &e));
        memset(viae, 'x', VIII);          /* fons vocantis mutatur */
        CREDO_VERUM (eventus_caudae_extrahere(c, &e));
        CREDO_VERUM (e.genus == EVENTUS_DEPOSITIO);
        CREDO_VERUM (chorda_aequalis_literis(e.datum.depositio.viae,
            "/a\n/b c"));
        CREDO_VERUM (e.datum.depositio.viae.datum == c->textus);
        CREDO_AEQUALIS_I32 (e.datum.depositio.numerus, II);
        CREDO_VERUM (e.datum.depositio.x == VII && e.datum.depositio.y
            == IX && e.datum.depositio.promota);
        /* tabula sine loco: viae truncari nequeunt - recusatur */
        eventus_cauda_lectio_incipit(c);
        c->textus_mensura = EVENTUS_CAUDA_TEXTUS - II;
        strcpy(viae, "/a/b");
        e.datum.depositio.viae.datum    = (i8*)viae;
        e.datum.depositio.viae.mensura  = IV;
        CREDO_FALSUM (eventus_caudae_depositionem_impellere(c, &e));
        CREDO_AEQUALIS_I32 (c->numerus, ZEPHYRUM);
        eventus_cauda_lectio_incipit(c);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "cauda una per gradus I-XI", _sectio_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("eventus_cauda", SECTIONES);
}

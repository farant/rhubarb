/* eventus_cauda.c - Vide eventus_cauda.h */

#include "eventus_cauda.h"
#include <string.h>

vacuum
eventus_caudam_initiare (
    EventusCauda* cauda)
{
    cauda->caput            = ZEPHYRUM;
    cauda->finis            = ZEPHYRUM;
    cauda->numerus          = ZEPHYRUM;
    cauda->amissa           = ZEPHYRUM;
    cauda->textus_mensura   = ZEPHYRUM;
    cauda->exempla_mensura  = ZEPHYRUM;
}

vacuum
eventus_cauda_lectio_incipit (
    EventusCauda* cauda)
{
    /* visus eventuum nondum extractorum in tabulas monstrant: vacare
     * solum si nullum restat */
    si (cauda->numerus == ZEPHYRUM)
    {
        cauda->textus_mensura   = ZEPHYRUM;
        cauda->exempla_mensura  = ZEPHYRUM;
    }
}

b32
eventus_caudae_impellere (
          EventusCauda* cauda,
      constans Eventus* eventus)
{
    si (cauda->numerus >= EVENTUS_CAUDA_CAPACITAS)
    {
        cauda->amissa++;
        redde FALSUM;
    }
    cauda->eventus[cauda->finis] = *eventus;
    cauda->finis = (cauda->finis + I) % EVENTUS_CAUDA_CAPACITAS;
    cauda->numerus++;
    redde VERUM;
}

b32
eventus_caudae_textum_impellere (
       EventusCauda* cauda,
                s64  tempus,
        constans i8* octeti,
                i32  mensura,
       EventusOrigo  origo)
{
    Eventus e;
        i32 locus;
        i32 capit;

    si (mensura == ZEPHYRUM || octeti == NIHIL)
    {
        redde FALSUM;
    }
    locus = EVENTUS_CAUDA_TEXTUS - cauda->textus_mensura;
    capit = (mensura < locus) ? mensura : locus;
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                   = EVENTUS_TEXTUS;
    e.tempus                  = tempus;
    e.datum.textus.genus      = EVENTUS_TEXTUS_COMMISSUM;
    e.datum.textus.origo      = origo;
    e.datum.textus.truncatum  = (b32)(capit < mensura);
    e.datum.textus.contentum.datum
        = cauda->textus + cauda->textus_mensura;
    e.datum.textus.contentum.mensura = capit;
    memcpy(cauda->textus + cauda->textus_mensura, octeti,
        (memoriae_index)capit);
    cauda->textus_mensura += capit;
    redde eventus_caudae_impellere(cauda, &e);
}

b32
eventus_caudae_depositionem_impellere (
          EventusCauda* cauda,
      constans Eventus* eventus)
{
    Eventus e;
        i32 mensura = eventus->datum.depositio.viae.mensura;

    si (mensura > EVENTUS_CAUDA_TEXTUS - cauda->textus_mensura)
    {
        redde FALSUM;
    }
    e = *eventus;
    e.datum.depositio.viae.datum = cauda->textus
        + cauda->textus_mensura;
    si (mensura > ZEPHYRUM)
    {
        memcpy(cauda->textus + cauda->textus_mensura,
            eventus->datum.depositio.viae.datum,
            (memoriae_index)mensura);
    }
    cauda->textus_mensura += mensura;
    redde eventus_caudae_impellere(cauda, &e);
}

b32
eventus_caudae_motum_impellere (
          EventusCauda* cauda,
      constans Eventus* eventus)
{
    Eventus* ultimus;

    si (cauda->numerus == ZEPHYRUM)
    {
        redde eventus_caudae_impellere(cauda, eventus);
    }
    ultimus = &cauda->eventus[(cauda->finis + EVENTUS_CAUDA_CAPACITAS
        - I)
        % EVENTUS_CAUDA_CAPACITAS];
    si (   ultimus->genus               != EVENTUS_MUS_MOTUS
        || ultimus->datum.mus.botton    != eventus->datum.mus.botton
        || ultimus->datum.mus.modificantes
               != eventus->datum.mus.modificantes
        || ultimus->datum.mus.indicator != eventus->datum.mus.indicator
        || ultimus->datum.mus.indicator_genus
               != eventus->datum.mus.indicator_genus)
    {
        redde eventus_caudae_impellere(cauda, eventus);
    }

    /* positio ultimi exemplum fit - si capit (D5: antiquissima
     * servantur). Exempla ultimi in fine tabulae iacent: nullus
     * eventus post eum exempla addidit. */
    si (   ultimus->datum.mus.numerus_exemplorum
        < EVENTUS_EXEMPLA_MAXIMA
        && cauda->exempla_mensura < EVENTUS_CAUDA_EXEMPLA)
    {
        EventusExemplum* ex = &cauda->exempla[cauda->exempla_mensura];

        si (ultimus->datum.mus.numerus_exemplorum == ZEPHYRUM)
        {
            ultimus->datum.mus.exempla = ex;
        }
        ex->x       = ultimus->datum.mus.x;
        ex->y       = ultimus->datum.mus.y;
        ex->tempus  = ultimus->tempus;
        cauda->exempla_mensura++;
        ultimus->datum.mus.numerus_exemplorum++;
    }
    ultimus->datum.mus.x        = eventus->datum.mus.x;
    ultimus->datum.mus.y        = eventus->datum.mus.y;
    ultimus->datum.mus.pressio  = eventus->datum.mus.pressio;
    ultimus->tempus             = eventus->tempus;
    redde VERUM;
}

s32
eventus_residuum_integrare (
     f64* residuum,
     f64  delta)
{
    s32 integra;

    /* f64 -> s32: C89 versus ZEPHYRUM truncat (definitum, dissimile
     * divisioni negativae) */
    *residuum  = *residuum + delta;
    integra    = (s32)*residuum;
    *residuum  = *residuum - (f64)integra;
    redde integra;
}

b32
eventus_caudae_extrahere (
    EventusCauda* cauda,
         Eventus* exitus)
{
    si (cauda->numerus == ZEPHYRUM)
    {
        redde FALSUM;
    }
    *exitus       = cauda->eventus[cauda->caput];
    cauda->caput  = (cauda->caput + I) % EVENTUS_CAUDA_CAPACITAS;
    cauda->numerus--;
    redde VERUM;
}

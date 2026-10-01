/* eventus_cauda.c - Vide eventus_cauda.h */

#include "eventus_cauda.h"
#include <string.h>

vacuum
eventus_caudam_initiare (
    EventusCauda* cauda)
{
    cauda->caput           = ZEPHYRUM;
    cauda->finis           = ZEPHYRUM;
    cauda->numerus         = ZEPHYRUM;
    cauda->amissa          = ZEPHYRUM;
    cauda->textus_mensura  = ZEPHYRUM;
}

vacuum
eventus_cauda_lectio_incipit (
    EventusCauda* cauda)
{
    /* visus eventuum nondum extractorum in tabulas monstrant: vacare
     * solum si nullum restat */
    si (cauda->numerus == ZEPHYRUM)
    {
        cauda->textus_mensura = ZEPHYRUM;
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

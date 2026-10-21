/* conspectus_dies.h - CONSPECTUS 'dies' (proiectiones D4b): quod
 * cellula mensis et columna hebdomadis de die uno pingunt, semel
 * collectum et COPIATUM (declaratio lib/conspectus_dies.stml).
 *
 * Fontes: agenda per-rem (vestigium agenda_rem_codificare), personae
 * omnia (natales per fasti_fontes), libri dierum (spatium
 * dies/{clavis}), liturgia et fontes dierum purae. Nec hodie nec
 * electa nec latitudo: pictura eas addit (proiectiones I.6). */

#ifndef CONSPECTUS_DIES_H
#define CONSPECTUS_DIES_H

/* <aedilis corpus="lib/conspectus_dies.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "xar.h"
#include "volumen.h"
#include "fasti.h"
#include "fasti_fontes.h"
#include "agenda.h"
#include "calendarium_liturgicum.h"
#include "conspectus.h"

nomen structura {
             Dies dies;
           chorda celebratio;    /* hebdomas: principalis si memoria
                                   * ad libitum aut maior, aliter ad
                                   * libitum prima; vacua si nulla */
           chorda festum;        /* mensis: titulus brevis principalis
                                   * si memoria aut maior nec
                                   * dominica */
  ColorLiturgicus  color;         /* valet solum si liturgia */
              b32  liturgia;      /* VERUM si liturgia posita */
              Xar* natales;       /* chorda: "Anna Rossi turns 40" */
              Xar* apertae;       /* chorda: tituli rerum apertarum
                                   * diei, ordine creationis */
     AgendaSignum  signum;        /* assignatae, apertae, perfectae */
              Xar* lineae_libri;  /* chorda: vacuae omissae,
                                   * indentatio communis remota */
} DiesConspectus;

nomen structura {
         AgendaDocumentum* agenda;     /* NIHIL licet */
     constans FastiFontes* fontes;     /* NIHIL licet */
    CalendariumLiturgicum* liturgia;   /* NIHIL licet */
                  Volumen* volumen;    /* libri dierum; NIHIL licet */
      InternamentumChorda* intern;
} DiesFontes;

/* declarationem infixam legere, opera et per-rem ponere, agendam
 * ligare (fontes a vocante possessi, vivant dum registrum). */
b32
conspectus_dies_registrare (
    ConspectusRegistrum*  repertorium,
             DiesFontes*  fontes,
                    Xar** vitia);

/* functio PURA ipsa, sine cache (visus sine volumine; verificatio) */
DiesConspectus*
conspectus_dies_computare (
    constans DiesFontes* fontes,
                   Dies  dies,
                Piscina* piscina);

/* codex canonicus (STML) */
chorda
conspectus_dies_codificare (
    constans DiesConspectus* c,
                    Piscina* piscina);

#endif /* CONSPECTUS_DIES_H */

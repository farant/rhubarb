/* saltuarius_liber.h - Filum apertum (Phase B: lexatio sola)
 *
 * ARENAE DUAE per librum: rhubarb (textus TRANSCRIPTUS + lineae +
 * tabula classium) et silvae (lexemata - contractus vitae:
 * lexemata textum aliant, ergo textus et lexemata co-vivunt,
 * ambae arenae simul destructae in evictione). Structura ipsa
 * arenam persistentem vocatoris habitat (parva; evictio eam
 * relinquit - stillicidium acceptum, notatum).
 *
 * Textus INICITUR (limes/principale discum legit; probationes
 * octetos tradunt). Cursor in RUNIS (columna = index runae).
 */

#ifndef SALTUARIUS_LIBER_H
#define SALTUARIUS_LIBER_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "silva.h"
#include "saltuarius_nexus.h"

nomen structura {
    i32 offset;    /* in textu */
    i32 mensura;   /* sine \n; \r caudae tonsa */
} SaltuariusLinea;

/* Positio lexematis in strato materializato (mappa: octeti <->
 * lexemata; ordine octetorum) */
nomen structura {
    SilvaToken* token;
           i32  initium;   /* octeti in textu strati */
           i32  finis;     /* exclusivus */
} SaltuariusPositio;

/* Stratum = (textus, lineae, classis) triplum + mappa positionum.
 * Stratum 0 = plagula ut scripta (mappa NIHIL - non opus);
 * strata k>0 MATERIALIZATA ex parsura->strata[k-1] (pigre). */
nomen structura {
                  b32  parata;
               chorda  textus;
      SaltuariusLinea* lineae;
                  i32  numerus_linearum;
                   i8* classis;      /* NIHIL -> nativus */
    SaltuariusPositio* positiones; /* NIHIL pro strato 0 */
                  i32  numerus_positionum;
} SaltuariusStratum;

nomen structura {
            Piscina* piscina;        /* arena libri (rhubarb) */
       SilvaPiscina* arena_silvae;   /* lexemata; NIHIL si merus */
             chorda  via;            /* in arena libri */
             chorda  textus;         /* transcriptus in arenam */
    SaltuariusLinea* lineae;
                i32  numerus_linearum;
                 i8* classis;        /* per octetum; NIHIL si merus */
                b32  est_colorata;
                s32  cursor_linea;   /* 0-basata */
                s32  cursor_columna; /* index runae in linea */
                s32  volumen_y;      /* prima linea visibilis */
                s32  volumen_x;      /* prima runa visibilis */
    /* Phase C: parse + strata */
         SilvaParsura* parsura;        /* NIHIL si non parsata */
    SaltuariusStratum* strata_visus; /* [numerus_stratorum] */
                  i32  numerus_stratorum; /* >= 1 (stratum 0 semper) */
                  s32  stratum_currens;   /* 0-basatum */
} SaltuariusLiber;

/* Aperire librum ex octetis iniectis: textus transcribitur in
 * arenam novam libri; .c/.h lexantur et colorantur (nexus), alia
 * mera manent. */
SaltuariusLiber*
saltuarius_liber_aperire (
            Piscina* persistens,
    SaltuariusNexus* nexus,
             chorda  via,
             chorda  textus);

/* Stratum activum SINE materializatione (paratum ex constructione
 * post transferre/ponere+stratum; pro pictoribus) */
constans SaltuariusStratum*
saltuarius_liber_stratum_activum (
    constans SaltuariusLiber* liber);

/* Stratum activum (materializat pigre si opus; nexus pro classi) */
constans SaltuariusStratum*
saltuarius_liber_stratum (
             SaltuariusLiber* liber,
    constans SaltuariusNexus* nexus);

/* Stratum ponere (clausum ad [0, numerus-1]); cursor NON hic
 * transfertur - vestigium identitatis C2 est */
vacuum
saltuarius_liber_stratum_ponere (
    SaltuariusLiber* liber,
                s32  stratum);

/* Offset octeti cursoris in textu strati ACTIVI; -1 si extra */
s32
saltuarius_liber_cursor_offset (
    constans SaltuariusLiber* liber);

/* Cursorem ad offset octeti ponere (in strato activo) */
vacuum
saltuarius_liber_cursor_ad_offset (
    SaltuariusLiber* liber,
                s32  offset);

/* Commutatio strati cum VESTIGIO IDENTITATIS (interview Q6):
 * cursor lexema trans strata sequitur (per catenam originis /
 * extenta FONS); refugium = linea proportionalis. Materializat
 * si opus. */
vacuum
saltuarius_liber_stratum_transferre (
             SaltuariusLiber* liber,
    constans SaltuariusNexus* nexus,
                         s32  stratum_novum);

/* Arenas destruere (evictio LRU / exitus) */
vacuum
saltuarius_liber_destruere (
    SaltuariusLiber* liber);

/* Textus lineae (sine \n, \r caudae tonsa); chorda vacua si extra */
chorda
saltuarius_liber_linea (
    constans SaltuariusLiber* liber,
                         s32  index);

/* Runae lineae (ambulatu utf8) */
i32
saltuarius_liber_linea_runae (
    constans SaltuariusLiber* liber,
                         s32  index);

/* Cursorem movere per lineas (columna ad runas lineae novae
 * clausa) / per columnas (clausa ad [0, runae]) */
vacuum
saltuarius_liber_movere (
    SaltuariusLiber* liber,
                s32  delta_linea);

vacuum
saltuarius_liber_movere_col (
    SaltuariusLiber* liber,
                s32  delta);

vacuum
saltuarius_liber_primum (
    SaltuariusLiber* liber);

vacuum
saltuarius_liber_ultimum (
    SaltuariusLiber* liber);

/* Volumina aptare ut cursor intra fenestram (alt lineae, lat
 * runae) visibilis sit */
vacuum
saltuarius_liber_aptare (
    SaltuariusLiber* liber,
                i32  altitudo_fenestrae,
                i32  latitudo_fenestrae);

#endif /* SALTUARIUS_LIBER_H */
